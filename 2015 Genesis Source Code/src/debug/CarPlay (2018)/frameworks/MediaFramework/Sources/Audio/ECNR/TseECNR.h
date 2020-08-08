/*
	File:       TseECNR.h
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

#ifndef __TSE_ECNR_H__
#define __TSE_ECNR_H__

#if defined (_USE_STL)
using namespace std;
#include <vector>
#else
#include "ringbuffer.h"
#endif

#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

#include "AirPlayCommon.h"
#include "AirPlaySettings.h"
#include "APSCommonServices.h"
#include "APSDebugServices.h"
#include "AtomicUtils.h"

#include "AudioECNR.h"
#include "tseFilter.h"

class TseECNR : public AudioECNR {
public:
	TseECNR();
	virtual ~TseECNR();

	virtual int start();
	virtual void stop();
	virtual int prepare();

	virtual void enable(Boolean enable);
	virtual Boolean isEnabled();

	virtual size_t getSampleFrames();
	virtual void setSampleFrames(size_t size);

	virtual void setBytesPerFrame(size_t size);
	virtual void setSampleRate(size_t size);
	virtual void setSiri(Boolean siri);
	virtual Boolean getSiri(void){return mSiriEnabled;}

	virtual void * insertMicInBuffer(uint8_t *buf, size_t cnt);
	virtual void * insertRecvInBuffer(uint8_t *buf, size_t cnt);

	virtual int extractMicOutBuffer(uint8_t *buf, size_t cnt);
	virtual int extractRecvOutBuffer(uint8_t *buf, size_t cnt);

	virtual void setConfigFilePath(const char * path);

	virtual void signal();

	dispatch_semaphore_t getSemaphore(void) {return mSemECNRProcess;}

	virtual Boolean isBufferingComplete(void) {return !mBufferingRecvOut;}

	void setVolInfoPtr(uint32_t * pVolInfo);

private:
	static void * ecnrThread(void *arg);

	TseECNR(const TseECNR&);
	TseECNR& operator=(const TseECNR&);

	dispatch_semaphore_t mSemECNRProcess;
	dispatch_semaphore_t mSemBufferingRecvOut;

    pthread_t        mThread;
	Boolean			 mEnabled;
#if defined (_USE_STL)
	vector<uint8_t>  mMicInBuf;
	vector<uint8_t>  mMicOutBuf;
	vector<uint8_t>  mRecvInBuf;
	vector<uint8_t>  mRecvOutBuf;
#else
    tsRingbuffer *   mMicInBuf;   // MIC input
    tsRingbuffer *   mMicOutBuf;  // MIC input ECNR processed
    tsRingbuffer *   mRecvInBuf;  // Speaker output
    tsRingbuffer *   mRecvOutBuf; // Speaker output ECNR processed
#endif

	size_t           mSampleFrames;
	size_t           mSampleBytes;
	size_t           mBufferSize;
	size_t           mBytesPerFrame;
	size_t           mSampleRate;
	Boolean			 mSiriEnabled;

	void ecnrProcess(uint8_t *micIn, uint8_t *micOut, uint8_t *recvIn, uint8_t *recvOut);

	pthread_mutex_t  mMutex;

	void *     		 mTseCtx;

	Boolean			 mBufferingRecvOut;
	Boolean			 mBufferingMicOut;

	char *           mConfigFilePath;
#if defined(MIBGP)
	int *            mPhoneVol;
#endif
     uint32_t *        mpVolInfo;
};

#endif // __TSE_ECNR_H__

