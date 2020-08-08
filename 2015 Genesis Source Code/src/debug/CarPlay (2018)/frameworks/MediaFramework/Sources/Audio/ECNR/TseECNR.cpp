/*
	File:       TseECNR.cpp
	Package: 	LG CarPlay MediaFramework
	Author:     Jaeheon Kim (jaeheon.kim@lge.com)

	Copyright © [2014-2016] by LG Electronics Inc.

	This program or software including the accompanying associated documentation
	(“Software”) is the proprietary software of LG Electronics Inc. and or its
	licensors, and may only be used, duplicated, modified or distributed pursuant
	to the terms and conditions of a separate written license agreement between you
	and LG Electronics Inc. (“Authorized License”). Except as set forth in an
	Authorized License, LG Electronics Inc. grants no license (express or implied),
	rights to use, or waiver of any kind with respect to the Software, and LG
	Electronics Inc. expressly reserves all rights in and to the Software and all
	intellectual property therein. If you have no Authorized License, then you have
	no rights to use the Software in any ways, and should immediately notify LG
	Electronics Inc. and discontinue all use of the Software.
*/

#include "AudioUtils.h"

#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#if defined(MIBGP)
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#endif

#include "AirPlayCommon.h"
#include "AirPlaySettings.h"
#include "APSCommonServices.h"
#include "APSDebugServices.h"
#include "AtomicUtils.h"
#include "TickUtils.h"
#include "ThreadUtils.h"

#include "tseFilter.h"
#include "tseFilter_priv.h"

#include "AudioECNR.h"
#include "TseECNR.h"

// ulog_define(TseECNR, kLogLevelNotice, kLogFlags_Default, "TseECNR", "TseECNR:rate=5;1000");
ulog_define(TseECNR, kLogLevelNotice, kLogFlags_Default, "TseECNR", NULL);
#define as_dlog(LEVEL, ...)     dlogc(&log_category_from_name(TseECNR), (LEVEL), __VA_ARGS__)
#define as_ulog(LEVEL, ...)     ulog(&log_category_from_name(TseECNR), (LEVEL), __VA_ARGS__)

#define FRAMES_TO_NSEC(rate, frame)     (frame * UINT64_C_safe(kNanosecondsPerSecond) / rate)
#define _BUFFER_SIZE_RATIO              8           // Maximum delay is 82~128msec

#define _PCM_DATA_DUMP                  0
#define _FILE_PATH                      DUMP_FILE_PATH      // path with write permission. This string must end "/".

#define TSEWarningCodeToString_( X ) \
    ( ( (X) == TSE_WARN_TSF_BUFFER_NULL )       ? "Buffer is Null"          : \
      ( (X) == TSE_WARN_TSF_CONVERT_FAILED )    ? "Convert failed"          : \
      ( (X) == TSE_WARN_TSF_CRC_CHECK_FAILED )  ? "CRC check failed"        : \
      ( (X) == TSE_WARN_TSF_DECODE_FAILED )     ? "Decode failed"           : \
      ( (X) == TSE_WARN_TSF_VERSION_MISMATCH )  ? "Version Mismatched"      : \
      ( (X) == TSE_WARN_TSF_FILE_OPEN_FAILED )  ? "Tsf file open failed"    : \
                                                  "Invalied warning code" )

#if (_PCM_DATA_DUMP)
FILE *                      micInFd = NULL;
FILE *                      micOutFd = NULL;
FILE *                      recvInFd = NULL;
FILE *                      recvOutFd = NULL;
char                        file_name[100];
#endif

TseECNR::TseECNR() :
	mSemECNRProcess(NULL), mSemBufferingRecvOut(NULL),
    mThread(0), mEnabled(false),
    mMicInBuf(NULL), mMicOutBuf(NULL),
    mRecvInBuf(NULL), mRecvOutBuf(NULL),
	mSampleFrames(0), mSampleBytes(0), mBufferSize(0),
	mBytesPerFrame(0), mSampleRate(0), mSiriEnabled(false),
	mTseCtx(NULL), mBufferingRecvOut(true), mBufferingMicOut(true),
	mConfigFilePath(NULL), mpVolInfo(NULL)
{
}

TseECNR::~TseECNR()
{
	if (mConfigFilePath) {
		free(mConfigFilePath);
		mConfigFilePath = NULL;
	}
}

int TseECNR::start()
{
#if defined(MIBGP)
    int   shmid = 0;
    void* phoneVolumeSharedMemory = (void *)0;

    shmid = shmget((key_t)8533, sizeof(int), 0666|IPC_CREAT);
    phoneVolumeSharedMemory = shmat(shmid, (void *)0, 0);
    mPhoneVol = (int *)phoneVolumeSharedMemory;
#endif

    return pthread_create(&mThread, NULL, ecnrThread, this);
}

void TseECNR::stop()
{
    mEnabled = false;

    if (mSemECNRProcess)        dispatch_semaphore_signal(mSemECNRProcess);
    if (mSemBufferingRecvOut)   dispatch_semaphore_signal(mSemBufferingRecvOut);

    if (mThread) {
        pthread_join(mThread, NULL);
        mThread = 0;
    }
    if (mSemECNRProcess) {
        dispatch_release(mSemECNRProcess);
        mSemECNRProcess = NULL;
    }
    if (mSemBufferingRecvOut) {
        dispatch_release(mSemBufferingRecvOut);
        mSemBufferingRecvOut = NULL;
    }

    destroyRingbuffer(&mMicInBuf);
    destroyRingbuffer(&mMicOutBuf);
    destroyRingbuffer(&mRecvInBuf);
    destroyRingbuffer(&mRecvOutBuf);

	pthread_mutex_destroy(&mMutex);

    if (mTseCtx != NULL) {
    	LGTSE_FilterDestroy(mTseCtx);
    	mTseCtx = NULL;
    }

#if (_PCM_DATA_DUMP)
    if (micInFd)    fclose(micInFd);
    if (micOutFd)   fclose(micOutFd);
    if (recvInFd)   fclose(recvInFd);
    if (recvOutFd)  fclose(recvOutFd);
#endif

    as_ulog(kLogLevelWarning, "TseECNR::%s: stream = %p\n", __FUNCTION__, this);
}

int TseECNR::prepare()
{
    int err = 0;
    int mode = CARPLAY_TELEPHONY_MODE;

    mSampleBytes = mSampleFrames * mBytesPerFrame;
    mBufferSize = mSampleBytes * _BUFFER_SIZE_RATIO;

    // ready to audio data
    /* Create ring buffer */
    err = createRingbuffer(&mMicInBuf, mBufferSize);
    require(iRINGBUFFER_OK == err, exit);

    err = createRingbuffer(&mMicOutBuf, mBufferSize);
    require(iRINGBUFFER_OK == err, exit);

    err = createRingbuffer(&mRecvInBuf, mBufferSize);
    require(iRINGBUFFER_OK == err, exit);

    err = createRingbuffer(&mRecvOutBuf, mBufferSize);
    require(iRINGBUFFER_OK == err, exit);

    mSemECNRProcess = dispatch_semaphore_create(0);
    require_action(mSemECNRProcess, exit, err = kNoMemoryErr);

    mSemBufferingRecvOut = dispatch_semaphore_create(0);
    require_action(mSemBufferingRecvOut, exit, err = kNoMemoryErr);

    pthread_mutex_init(&mMutex, NULL);

    // 1) Create object
    mTseCtx = LGTSE_FilterCreate();
    if(mTseCtx == NULL) {
        as_ulog(kLogLevelError, "%s: LGTSE_FilterCreate Error\n", __FUNCTION__);
        require_noerr(err, exit);
    }

    // 2) Set *.tsf files path
    if (mConfigFilePath != NULL) {
        err = LGTSE_SetTsfPath(mTseCtx, mConfigFilePath);
        if (err != TSE_ERR_OK) {
            as_ulog(kLogLevelError, "%s: LGTSE_SetTsfPath Error\n", __FUNCTION__ );
        }
    }

    // 3) Set Data & Initialize
    // !!!!  1 frame = 16bit = 2byte  // Avoiding buffer overruns
    if ( mSiriEnabled ) 	mode = CARPLAY_SIRI_MODE;
    else					mode = CARPLAY_TELEPHONY_MODE;

    err = LGTSE_FilterInit(mTseCtx, mode, mSampleRate, 1, mSampleFrames);
    if (err != TSE_ERR_OK) {
        if (err > TSE_WARN_START && err < TSE_WARN_END ) {
            as_ulog(kLogLevelWarning, "%s: LGTSE_FilterInit Warning, message = %s.\n", __FUNCTION__, TSEWarningCodeToString_(err));
        } else {
            as_ulog(kLogLevelError, "%s: LGTSE_FilterInit Error\n", __FUNCTION__ );
            require_noerr(err, exit);
        }
    }

    as_ulog(kLogLevelNotice, "%s: LGTSE_FilterInit complete\n", __FUNCTION__);

#if (_PCM_DATA_DUMP)
    as_ulog(kLogLevelWarning, "TseECNR::%s: stream = %p\n", __FUNCTION__, this);

    sprintf(file_name, _FILE_PATH "(%p)mic_input_%d.pcm", this, mSampleRate );
    micInFd = fopen(file_name, "w+");

    sprintf(file_name, _FILE_PATH "(%p)mic_output_%d.pcm", this, mSampleRate );
    micOutFd = fopen(file_name, "w+");

    sprintf(file_name, _FILE_PATH "(%p)recv_input_%d.pcm", this, mSampleRate );
    recvInFd = fopen(file_name, "w+");

    sprintf(file_name, _FILE_PATH "(%p)recv_output_%d.pcm", this, mSampleRate );
    recvOutFd = fopen(file_name, "w+");
#endif

exit:
    if (err) stop();

	return 0;
}

void TseECNR::enable(Boolean _enable)
{
	mEnabled = _enable;
}

Boolean TseECNR::isEnabled()
{
	return mEnabled;
}

size_t TseECNR::getSampleFrames()
{
	return mSampleFrames;
}

void TseECNR::setSampleFrames(size_t size)
{
	mSampleFrames = size;
}

void TseECNR::setBytesPerFrame(size_t size)
{
	mBytesPerFrame = size;
}

void TseECNR::setSampleRate(size_t size)
{
	mSampleRate = size;
}

void TseECNR::setSiri(Boolean _siri)
{
	mSiriEnabled = _siri;
}

void * TseECNR::insertMicInBuffer(uint8_t *buf, size_t cnt)
{
    int freeBytes;
    uint8_t *  tmpBuf = NULL;
    bool       locked = false;

    require(mMicInBuf != NULL, exit);

    pthread_mutex_lock(&mMutex);
    locked = true;
    /* push process */
    freeBytes = getFreeBytesRingbuffer(mMicInBuf);
    require(freeBytes >= 0, exit);
    if (freeBytes < (int)cnt) {
        tmpBuf = (uint8_t *)malloc(cnt);
        require(tmpBuf != NULL, exit);

        extractFromRingbuffer(mMicInBuf, (char*)tmpBuf, (int)cnt - freeBytes);
        as_ulog(kLogLevelError, "%s: Mic-in buffer is full. Droping %i bytes\n", __FUNCTION__, cnt - freeBytes);
    }
    insertIntoRingbuffer(mMicInBuf, (char*)buf, cnt);

    dispatch_semaphore_signal(mSemECNRProcess);

exit:
    if (locked) {
        pthread_mutex_unlock(&mMutex);
    }
    if (tmpBuf != NULL) {
        free(tmpBuf);
        tmpBuf = NULL;
    }

	return (NULL);
}

void * TseECNR::insertRecvInBuffer(uint8_t *buf, size_t cnt)
{
    int freeBytes;
    uint8_t *  tmpBuf = NULL;
    bool       locked = false;

    require(mRecvInBuf != NULL, exit);

    pthread_mutex_lock(&mMutex);
    locked = true;
    /* push process */
    freeBytes = getFreeBytesRingbuffer(mRecvInBuf);
    require(freeBytes >= 0, exit);
    if (freeBytes < (int)cnt) {
        tmpBuf = (uint8_t *)malloc(cnt);
        require(tmpBuf != NULL, exit);

        extractFromRingbuffer(mRecvInBuf, (char*)tmpBuf, cnt - freeBytes);
        as_ulog(kLogLevelError, "%s: Recv-in buffer is full. Droping %i bytes\n", __FUNCTION__, cnt - freeBytes);
    }

    insertIntoRingbuffer(mRecvInBuf, (char*)buf, cnt);

    dispatch_semaphore_signal(mSemECNRProcess);
exit:
    if (locked) {
        pthread_mutex_unlock(&mMutex);
    }
    if (tmpBuf != NULL) {
        free(tmpBuf);
        tmpBuf = NULL;
    }

	return (NULL);
}

int TseECNR::extractMicOutBuffer(uint8_t *buf, size_t cnt)
{
	int        retVal = 0;
	size_t     availableBytes;

    pthread_mutex_lock(&mMutex);

    require(mMicOutBuf != NULL, exit);

    availableBytes = mBufferSize - getFreeBytesRingbuffer(mMicOutBuf);

    require_quiet(availableBytes>=cnt, exit);

    retVal = extractFromRingbuffer(mMicOutBuf, (char*)buf, cnt);

    if (0 > retVal) {
        as_ulog(kLogLevelError, "%s: Error Mic-out buffer\n", __FUNCTION__);
    }
exit:
    pthread_mutex_unlock(&mMutex);

	return retVal;
}

int TseECNR::extractRecvOutBuffer(uint8_t *buf, size_t cnt)
{
	int        retVal = 0;
	size_t     availableBytes;

    require(mRecvOutBuf != NULL, exit);

	pthread_mutex_lock(&mMutex);
    availableBytes = mBufferSize - getFreeBytesRingbuffer(mRecvOutBuf);
    pthread_mutex_unlock(&mMutex);

    if (mBufferingRecvOut) {
        // Wait for first ECNR processed data ready.
        if (availableBytes >= mSampleBytes) {
            mBufferingRecvOut = false;

            // for reducing receiving delay
            pthread_mutex_lock(&mMutex);
            retVal = extractFromRingbuffer(mRecvOutBuf, (char*)buf, cnt);
            pthread_mutex_unlock(&mMutex);
        }
    } else {
        if (availableBytes == 0) {
            // Wait for Recv Processed Data
            // as_ulog(kLogLevelWarning, "%s: Insufficient Recv Out Buffer, wait......\n", __FUNCTION__);
            long err = dispatch_semaphore_wait( mSemBufferingRecvOut,
                                           dispatch_time( DISPATCH_TIME_NOW,
                                                          FRAMES_TO_NSEC(mSampleRate, mSampleFrames))
                                         );
            if (err == ETIMEDOUT) goto exit;
        }
        pthread_mutex_lock(&mMutex);
        retVal = extractFromRingbuffer(mRecvOutBuf, (char*)buf, cnt);
        pthread_mutex_unlock(&mMutex);
        if (0 > retVal) {
            as_ulog(kLogLevelError, "%s: Error Recv-out buffer\n", __FUNCTION__);
        }
    }

exit:
	return retVal;
}

void TseECNR::ecnrProcess(uint8_t *micIn, uint8_t *micOut, uint8_t *recvIn, uint8_t *recvOut)
{
    size_t     freeBytes = 0;
    size_t     micInAvailableBytes = 0;
    size_t     recvInAvailableBytes = 0;

    pthread_mutex_lock(&mMutex);

    micInAvailableBytes = mBufferSize - getFreeBytesRingbuffer(mMicInBuf);
    recvInAvailableBytes = mBufferSize - getFreeBytesRingbuffer(mRecvInBuf);

    pthread_mutex_unlock(&mMutex);

	while ( (micInAvailableBytes >= mSampleBytes) && (recvInAvailableBytes >= mSampleBytes) ) {

	    pthread_mutex_lock(&mMutex);

        extractFromRingbuffer(mMicInBuf, (char*)micIn, mSampleBytes);
        extractFromRingbuffer(mRecvInBuf, (char*)recvIn, mSampleBytes);

		pthread_mutex_unlock(&mMutex);

#if (_PCM_DATA_DUMP)
        if (micInFd)    fwrite(micIn,   1, mSampleBytes, micInFd);
        if (recvInFd)   fwrite(recvIn,  1, mSampleBytes, recvInFd);
#endif
	// ECNR
#if 1
    #if defined(MIBGP)
        LGTSE_FilterProcess(mTseCtx, (char*)micIn, (char*)micOut, (char*)recvIn, (char*)recvOut, (unsigned int*)mPhoneVol);
	#else
		if(mpVolInfo != NULL) {
			LGTSE_FilterProcess(mTseCtx, (char*)micIn, (char*)micOut, (char*)recvIn, (char*)recvOut, (unsigned int*)mpVolInfo);
		} else {
			LGTSE_FilterProcess(mTseCtx, (char*)micIn, (char*)micOut, (char*)recvIn, (char*)recvOut, NULL);
		}		
    #endif
#else
		memcpy(micOut, micIn, mSampleBytes);
		memcpy(recvOut, recvIn, mSampleBytes);
#endif

#if (_PCM_DATA_DUMP)
        if (micOutFd)   fwrite(micOut,  1, mSampleBytes, micOutFd);
        if (recvOutFd)  fwrite(recvOut, 1, mSampleBytes, recvOutFd);
#endif

        pthread_mutex_lock(&mMutex);

        freeBytes = getFreeBytesRingbuffer(mMicOutBuf);
        if (freeBytes < mSampleBytes) {
            extractFromRingbuffer(mMicOutBuf, (char*)micIn, mSampleBytes - freeBytes);
            as_ulog(kLogLevelError, "%s: Mic-out buffer is full. Droping %i bytes\n", __FUNCTION__, mSampleBytes - freeBytes);
        }
        insertIntoRingbuffer(mMicOutBuf, (char*)micOut, mSampleBytes);

        freeBytes = getFreeBytesRingbuffer(mRecvOutBuf);
        if (freeBytes < mSampleBytes) {
            extractFromRingbuffer(mRecvOutBuf, (char*)recvIn, mSampleBytes - freeBytes);
            as_ulog(kLogLevelError, "%s: Recv-out buffer is full. Droping %i bytes\n", __FUNCTION__, mSampleBytes - freeBytes);
        }
        insertIntoRingbuffer(mRecvOutBuf, (char*)recvOut, mSampleBytes);

        // Update size
        micInAvailableBytes = mBufferSize - getFreeBytesRingbuffer(mMicInBuf);
        recvInAvailableBytes = mBufferSize - getFreeBytesRingbuffer(mRecvInBuf);

        // Set end buffering.
        mBufferingRecvOut = false;

	    pthread_mutex_unlock(&mMutex);
	}

    dispatch_semaphore_signal(mSemBufferingRecvOut);
}

void TseECNR::setConfigFilePath(const char * path)
{
	require(path != NULL, exit);

	if (mConfigFilePath) {
		free(mConfigFilePath);
		mConfigFilePath = NULL;
	}
	mConfigFilePath = strdup(path);
	as_ulog(kLogLevelWarning, "%s: ECNR configuration file path is %s \n", __FUNCTION__, mConfigFilePath);

exit:
	return;
}

void TseECNR::setVolInfoPtr(uint32_t * pVolInfo) {
	mpVolInfo = pVolInfo;
    as_ulog(kLogLevelWarning, "%s: mpVolInfo : %p, *(mpVolInfo) = %d\n", __FUNCTION__, mpVolInfo, (*mpVolInfo));	
}


void TseECNR::signal()
{
	return;
}

void * TseECNR::ecnrThread(void *arg)
{
	TseECNR *me = (TseECNR*)arg;

	uint8_t *  micIn = NULL;
	uint8_t *  micOut = NULL;
	uint8_t *  recvIn = NULL;
	uint8_t *  recvOut = NULL;

    SetCurrentThreadPriority(kAirPlayThreadPriority_AudioReceiver+1);

	micIn = (uint8_t *) calloc(1, me->mSampleBytes);
	require(micIn != NULL, exit);

	micOut = (uint8_t *) calloc(1, me->mSampleBytes);
	require(micOut != NULL, exit);

	recvIn = (uint8_t *) calloc(1, me->mSampleBytes);
	require(recvIn != NULL, exit);

	recvOut = (uint8_t *) calloc(1, me->mSampleBytes);
	require(recvOut != NULL, exit);

	while (me->mEnabled) {
	    dispatch_semaphore_wait(me->getSemaphore(), DISPATCH_TIME_FOREVER);
		me->ecnrProcess(micIn, micOut, recvIn, recvOut);
	}

exit:
	as_ulog(kLogLevelNotice, "%s: exit, ECNR is %s\n", __FUNCTION__, me->mEnabled ? "on" : "off");

	if (micIn != NULL)
		free(micIn);
	if (micOut != NULL)
		free(micOut);
	if (recvIn != NULL)
		free(recvIn);
	if (recvOut != NULL)
		free(recvOut);

    pthread_exit(NULL);

    return (NULL);
}
