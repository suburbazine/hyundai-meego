/*
	File:       DeviceALSA.cpp
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

#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <alsa/asoundlib.h>
#include <unistd.h>

#include "AirPlayCommon.h"
#include "APSDebugServices.h"
#include "DeviceALSA.h"

#define MAX_READ_TRIAL  3
#define MAX_WRITE_TRIAL 3

#define _DEFAULT_ALSA_BUFFER_TIME       80000       // usec unit
#define _DEFAULT_AUDIO_OUTPUT_LATENCY   35000       // usec unit
#define USEC_TO_FRAMES(rate, usec)      (rate * usec / UINT64_C_safe(kMicrosecondsPerSecond))

ulog_define(DeviceALSA, kLogLevelTrace, kLogFlags_Default, "DeviceALSA", NULL);
#define as_dlog(LEVEL, ...)     dlogc(&log_category_from_name(DeviceALSA), (LEVEL), __VA_ARGS__)
#define as_ulog(LEVEL, ...)     ulog(&log_category_from_name(DeviceALSA), (LEVEL), __VA_ARGS__)

DeviceALSA::DeviceALSA()
    : mPcmHandle(NULL), mDeviceName(NULL),
    mIsInput(false), mUseMmap(false), mIsReady(false),
    mPeriodTime(0), mPeriodSize(0), mBufferTime(0), mBufferSize(0),
    mSampleRate(0), mChannels(0), mBytesPerFrame(0), mBitsPerChannel(0)
{
    as_ulog(kLogLevelNotice, "DeviceALSA::%s\n", __FUNCTION__);
}

DeviceALSA::~DeviceALSA()
{
    as_ulog(kLogLevelNotice, "%s\n", __FUNCTION__);

    if (mDeviceName != NULL) {
        free(mDeviceName);
        mDeviceName = NULL;
    }
}

int32_t DeviceALSA::open()
{
    as_ulog(kLogLevelNotice, "DeviceALSA::%s\n", __FUNCTION__);

    snd_pcm_stream_t stream =
        mIsInput ? SND_PCM_STREAM_CAPTURE : SND_PCM_STREAM_PLAYBACK;

    int sndErr = -1;
    require(mDeviceName != NULL, exit);

    sndErr = snd_pcm_open(&mPcmHandle, mDeviceName, stream, 0);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: snd_pcm_open failed(%s)\n",
            __FUNCTION__, snd_strerror(sndErr));

    if (mPcmHandle != NULL)
        mIsReady = true;

exit:
    return sndErr;
}

int32_t DeviceALSA::close()
{
    as_ulog(kLogLevelNotice, "DeviceALSA::%s\n", __FUNCTION__);

    int sndErr = -1;
    require(mPcmHandle != NULL, exit);

#if defined(USE_DMIX_PLUGIN)
    sndErr = snd_pcm_drain(mPcmHandle);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: snd_pcm_drain failed(%s)\n",
            __FUNCTION__, snd_strerror(sndErr));
#else
    sndErr = snd_pcm_drop(mPcmHandle);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: snd_pcm_drop failed(%s)\n",
            __FUNCTION__, snd_strerror(sndErr));
#endif
    sndErr = snd_pcm_close(mPcmHandle);
    mPcmHandle = NULL;
    mIsReady = false;

exit:
    return sndErr;
}

bool DeviceALSA::isReady()
{
    return mIsReady;
}

int32_t DeviceALSA::set(char *name, bool isInput, uint32_t sampleRate, uint32_t channels,
    uint32_t bitsPerChannel, uint32_t periodTime, uint32_t bufferTime)
{
    as_ulog(kLogLevelNotice, "DeviceALSA::%s\n", __FUNCTION__);

    int32_t err = -1;
    require(name != NULL, exit);

    mDeviceName     = strdup(name);
    mIsInput        = isInput;
    mSampleRate     = sampleRate;
    mChannels       = channels;
    mBitsPerChannel = bitsPerChannel;
    mPeriodTime     = periodTime;
    mBufferTime     = bufferTime;

    mBytesPerFrame  = mChannels * mBitsPerChannel / 8;
    mPeriodSize     = mPeriodTime * mSampleRate / 1000000;
    mBufferSize     = mBufferTime * mSampleRate / 1000000;

    as_ulog(kLogLevelWarning, "%s: period_%dus(%dframes), buffer_%dus(%dframes)\n",
            __FUNCTION__, mPeriodTime, mPeriodSize, mBufferTime, mBufferSize);

    err = 0;

exit:
    return (err);
}

int32_t DeviceALSA::prepare()
{
    as_ulog(kLogLevelNotice, "DeviceALSA::%s\n", __FUNCTION__);

    int sndErr = -1;
    require(mPcmHandle != NULL, exit);

    sndErr = setHwParams();
    require(sndErr == 0, exit);

    sndErr = setSwParams();
    require(sndErr == 0, exit);

    as_ulog(kLogLevelWarning, "%s: period_%dus(%dframes), buffer_%dus(%dframes)\n",
            __FUNCTION__, mPeriodTime, mPeriodSize, mBufferTime, mBufferSize);

    sndErr = snd_pcm_prepare(mPcmHandle);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: snd_pcm_prepare failed(%s)\n",
            __FUNCTION__, snd_strerror(sndErr));

exit:
    return sndErr;
}

int32_t DeviceALSA::write(uint8_t *buffer, int reqFrames)
{
    int leftFrames = reqFrames;
    uint8_t *p = buffer;
    int retVal = -1;
    int errCount = 0;

    while (leftFrames > 0 && errCount < MAX_WRITE_TRIAL) {
        if (mUseMmap && (0 != mPcmHandle)) {
            retVal = snd_pcm_mmap_writei(mPcmHandle, p, leftFrames);
        } else if ( 0 != mPcmHandle ) {
            retVal = snd_pcm_writei(mPcmHandle, p, leftFrames);
        } else {
         	as_ulog(kLogLevelError, "%s: mPcmHandle is NULL !!! skip snd_pcm_writei\n", __FUNCTION__);
        }

        if ((retVal == -EPIPE) && (0 != mPcmHandle)) {
            snd_pcm_prepare(mPcmHandle);
            as_ulog(kLogLevelError, "%s: snd_pcm_writei failed(%s) - EPIPE\n",
                    __FUNCTION__, snd_strerror(retVal));
            return retVal;
        } else if (retVal < 0) {
            as_ulog(kLogLevelError, "%s: snd_pcm_writei failed(%s)\n",
                    __FUNCTION__, snd_strerror(retVal));
            return retVal;
        } else {
            errCount++;
            p += (retVal * mBytesPerFrame);
            leftFrames -= retVal;
        }
    }

    return reqFrames - leftFrames;
}

int32_t DeviceALSA::read(uint8_t * buffer, int reqFrames)
{
    int leftFrames = reqFrames;
    uint8_t *p = buffer;
    int retVal = -1;
    int errCount = 0;

    while (leftFrames > 0 && errCount < MAX_READ_TRIAL) {
        if (mUseMmap && ( 0 != mPcmHandle)) {
            retVal = snd_pcm_mmap_readi(mPcmHandle, p, leftFrames);
        } else if ( 0 != mPcmHandle ){
            retVal = snd_pcm_readi(mPcmHandle, p, leftFrames);
        } else {
         	as_ulog(kLogLevelError, "%s: mPcmHandle is NULL !!! skip snd_pcm_readi\n", __FUNCTION__);
        }
 
        if ((retVal == -EPIPE) && (0 != mPcmHandle)) {
            snd_pcm_prepare(mPcmHandle);
            as_ulog(kLogLevelError, "%s: snd_pcm_readi failed(%s) - EPIPE\n",
                    __FUNCTION__, snd_strerror(retVal));
            return retVal;
        } else if (retVal < 0) {
            as_ulog(kLogLevelError, "%s: snd_pcm_readi failed(%s)\n",
                    __FUNCTION__, snd_strerror(retVal));
            return retVal;
        } else {
            errCount++;
            p += (retVal * mBytesPerFrame);
            leftFrames -= retVal;
        }
    }

    return retVal;
}

int32_t DeviceALSA::setHwParams()
{
    as_ulog(kLogLevelNotice, "DeviceALSA::%s\n", __FUNCTION__);

    snd_pcm_hw_params_t * hwParams = NULL;
    int                   sndErr = -1;
    OSStatus              err = -1;

    snd_pcm_hw_params_alloca(&hwParams);
    require(hwParams != NULL, exit);

    sndErr = snd_pcm_hw_params_any(mPcmHandle, hwParams);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Broken configuration: no configurations available: %s\n\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    /* set hardware resampling : 0 = disable, 1 = enable (alsa default) rate resampling */
    sndErr = snd_pcm_hw_params_set_rate_resample(mPcmHandle, hwParams, 1);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Resampling setup failed: %s\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

#if 0 // org
    sndErr = snd_pcm_hw_params_set_access(mPcmHandle, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: snd_pcm_hw_params_set_access failed(%s)\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);
#else
    snd_pcm_access_mask_t *mask;
    mask = (snd_pcm_access_mask_t *)malloc(snd_pcm_access_mask_sizeof());
    snd_pcm_access_mask_none(mask);
    snd_pcm_access_mask_set(mask, SND_PCM_ACCESS_MMAP_INTERLEAVED);
    snd_pcm_access_mask_set(mask, SND_PCM_ACCESS_MMAP_NONINTERLEAVED);
    snd_pcm_access_mask_set(mask, SND_PCM_ACCESS_MMAP_COMPLEX);
    sndErr = snd_pcm_hw_params_set_access_mask(mPcmHandle, hwParams, mask);
    free(mask);
    if (sndErr < 0) {
        as_ulog(kLogLevelError, "%s: Access type not available: %s\n", __FUNCTION__, snd_strerror(sndErr));
        mUseMmap = false;

        sndErr = snd_pcm_hw_params_set_access(mPcmHandle, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED);
        if (sndErr != 0) as_ulog(kLogLevelError, "%s: Access type not available: %s\n", __FUNCTION__, snd_strerror(sndErr));
        require(sndErr == 0, exit);
    } else {
        mUseMmap = true;
    }
#endif

    switch (mBitsPerChannel) {
    case 8:
        sndErr = snd_pcm_hw_params_set_format(mPcmHandle, hwParams, SND_PCM_FORMAT_S8);
        break;
    case 16:
        sndErr = snd_pcm_hw_params_set_format(mPcmHandle, hwParams, SND_PCM_FORMAT_S16_LE);
        break;
    case 24:
        sndErr = snd_pcm_hw_params_set_format(mPcmHandle, hwParams, SND_PCM_FORMAT_S24_3LE);
        break;
    case 32:
        sndErr = snd_pcm_hw_params_set_format(mPcmHandle, hwParams, SND_PCM_FORMAT_S32_LE);
        break;
    default:
        sndErr = -1;
        break;
    }
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Sample format not available: %s\n\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    sndErr = snd_pcm_hw_params_set_channels(mPcmHandle, hwParams, mChannels);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Channels count (%i) not available for playbacks: %s\n",
            __FUNCTION__, mChannels, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    sndErr = snd_pcm_hw_params_set_rate(mPcmHandle, hwParams, mSampleRate, 0);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Rate %iHz not available: %s\n",
            __FUNCTION__, mSampleRate, snd_strerror(sndErr));

    if (sndErr != 0) {
        uint32_t sampleRate;
        sndErr = snd_pcm_hw_params_set_rate_near(mPcmHandle, hwParams, &sampleRate, NULL);
        if (sndErr != 0) as_ulog(kLogLevelError, "%s: Rate %iHz near not available: %s\n",
                __FUNCTION__, snd_strerror(sndErr));
        require(sndErr == 0, exit);

        if (sampleRate != mSampleRate) {
            as_ulog(kLogLevelWarning, "%s: sample rate is different(req: %d, res: %d)\n",
                    __FUNCTION__, mSampleRate, sampleRate);
            mSampleRate = sampleRate;
        }
    }

    sndErr = snd_pcm_hw_params_set_period_time_near(mPcmHandle, hwParams, &mPeriodTime, NULL);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to set period time %i for playback: %s\n",
            __FUNCTION__, mPeriodTime, snd_strerror(sndErr));
    require(sndErr == 0, exit);

#if defined(MEEGO)
    if (mIsInput) {
        if (mPeriodTime <= 2000)
            mBufferTime = mPeriodTime * 64;
        else if (mPeriodTime <= 4000)
            mBufferTime = mPeriodTime * 32;
        else if (mPeriodTime <= 8000)
            mBufferTime = mPeriodTime * 16;
        else if (mPeriodTime <= 10000)
            mBufferTime = mPeriodTime * 8;
        else
            mBufferTime = mPeriodTime * 4;
    } else {
        if (mPeriodTime <= 2000)
            mBufferTime = mPeriodTime * 32;
        else if (mPeriodTime <= 10000)
            mBufferTime = mPeriodTime * 8;
        else
            mBufferTime = mPeriodTime * 4;
    }
#else
    if (mBufferTime == 0) mBufferTime = _DEFAULT_ALSA_BUFFER_TIME;
#endif

    sndErr = snd_pcm_hw_params_set_buffer_time_near(mPcmHandle, hwParams, &mBufferTime, NULL);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to set buffer time %i for playback: %s\n",
            __FUNCTION__, mBufferTime, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    sndErr = snd_pcm_hw_params(mPcmHandle, hwParams);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to set hw params: %s\n\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    sndErr = snd_pcm_hw_params_get_buffer_size(hwParams, &mBufferSize);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to get buffer size: %s\n\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    sndErr = snd_pcm_hw_params_get_period_size(hwParams, &mPeriodSize, NULL);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to get period size: %s\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    err = 0;

exit:
    return (err);
}

int32_t DeviceALSA::setSwParams()
{
    snd_pcm_uframes_t startThreshold;
    snd_pcm_uframes_t boundary;
    snd_pcm_sw_params_t * swParams = NULL;
    int sndErr = -1;
    OSStatus err = -1;

    as_ulog(kLogLevelNotice, "DeviceALSA::%s\n", __FUNCTION__);

    snd_pcm_sw_params_alloca(&swParams);
    require(swParams != NULL, exit);

    sndErr = snd_pcm_sw_params_current(mPcmHandle, swParams);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to determine current swparams: %s\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    if (mIsInput) {
        // For capture, configure ALSA to start the transfer on the
        // first frame.
        startThreshold = 1;
    } else {
        // For playback, configure ALSA to start the transfer when the
        // buffer is full or buffer is about "_DEFAULT_AUDIO_OUTPUT_LATENCY"
        if (mBufferSize > USEC_TO_FRAMES(mSampleRate, _DEFAULT_AUDIO_OUTPUT_LATENCY))
            startThreshold = USEC_TO_FRAMES(mSampleRate, _DEFAULT_AUDIO_OUTPUT_LATENCY);
        else
            startThreshold = mBufferSize;
    }

    sndErr = snd_pcm_sw_params_set_start_threshold(mPcmHandle, swParams, startThreshold);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to set start threshold mode: %s\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    sndErr = snd_pcm_sw_params_get_boundary(swParams, &boundary);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to get boundary: %s\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);
    as_ulog(kLogLevelNotice, "%s: boundary : %llu\n", __FUNCTION__, boundary );

    // Automatic stop will be disabled(cause stop thresould == boundary)
    sndErr = snd_pcm_sw_params_set_stop_threshold(mPcmHandle, swParams, boundary);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to set stop threshold mode: %s\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    // allow the transfer when at least period_size samples can be processed
    sndErr = snd_pcm_sw_params_set_avail_min(mPcmHandle, swParams, mPeriodSize);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to set avail min: %s\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

#if 0       // Not Use slience settings.
    // playback buffer is overwritten with silence when playback underrun is nearer than silence threshold
    // Note: silence_threshold must be set to zero
    sndErr = snd_pcm_sw_params_set_silence_size(mPcmHandle, swParams, boundary);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to set silence size: %s\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    // disable for using snd_pcm_sw_params_set_silence_size()
    sndErr = snd_pcm_sw_params_set_silence_threshold(mPcmHandle, swParams, 0);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to set silence threshold: %s\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);
#endif

    sndErr = snd_pcm_sw_params(mPcmHandle, swParams);
    if (sndErr != 0) as_ulog(kLogLevelError, "%s: Unable to set sw params: %s\n\n",
            __FUNCTION__, snd_strerror(sndErr));
    require(sndErr == 0, exit);

    err = 0;

exit:
    return (err);
}

int32_t DeviceALSA::getDelay()
{
    int err;
    snd_pcm_status_t *status;
    snd_pcm_sframes_t delay;

    snd_pcm_avail_update(mPcmHandle);

    snd_pcm_status_alloca(&status);
    err = snd_pcm_status(mPcmHandle, status);
    if (err < 0) {
        as_ulog(kLogLevelError, "%s: snd_pcm_status failed(%d)\n", __FUNCTION__, err);
        return kUnknownErr;
    }

    delay = snd_pcm_status_get_delay(status);

    if (delay < 0) {
        as_ulog(kLogLevelError, "%s: snd_pcm_status_get_delay return negative. Prepare PCM handle.\n",__FUNCTION__);
        err = snd_pcm_prepare(mPcmHandle);
        if (err < 0)
            as_ulog(kLogLevelError, "%s: Can't prepare from not underrun, prepare failed: %s\n", __FUNCTION__, snd_strerror(err));
        delay = 0;
    }
    return delay;
}

uint32_t DeviceALSA::getAvail()
{
    int err;
    snd_pcm_status_t *status;
    snd_pcm_uframes_t avail;

    snd_pcm_avail_update(mPcmHandle);

    snd_pcm_status_alloca(&status);
    err = snd_pcm_status(mPcmHandle, status);
    if (err < 0) {
        as_ulog(kLogLevelError, "%s: snd_pcm_status failed(%d)\n", __FUNCTION__, err);
        return kUnknownErr;
    }

    avail = snd_pcm_status_get_avail(status);

    return avail;
}
