/*
	File:       DeviceALSA.h
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

#ifndef __DEVICE_ALSA_H__
#define __DEVICE_ALSA_H__

#include <stdlib.h>
#include <unistd.h>
#include <alsa/asoundlib.h>
#include "AudioALSA.h"

class DeviceALSA : public AudioALSA {
public:
	DeviceALSA();

	virtual ~DeviceALSA();

	virtual int32_t open();
	virtual int32_t close();
	virtual bool isReady();

	virtual int32_t set(char *name, bool isInput, uint32_t sampleRate, uint32_t channels,
			uint32_t bitsPerChannel, uint32_t periodTime, uint32_t bufferTime);
	virtual int32_t set(char *name, bool isInput, uint32_t sampleRate, uint32_t channels,
			uint32_t bitsPerChannel, uint32_t periodTime, uint32_t bufferTime, char *audioType) {
		(void)name;
		(void)isInput;
		(void)sampleRate;
		(void)channels;
		(void)bitsPerChannel;
		(void)periodTime;
		(void)bufferTime;
		(void)audioType;
		return 0;
	}
	virtual int32_t prepare();

	virtual int32_t write(uint8_t *buffer, int reqFrames);
	virtual int32_t read(uint8_t * buffer, int reqFrames);

	virtual int32_t getDelay();
	virtual uint32_t getAvail();
	virtual uint32_t getSampleRate() { return mSampleRate; }
	virtual uint32_t getPeriodTime() { return mPeriodTime; }
	virtual uint32_t getPeriodSize() { return mPeriodSize; }
	virtual uint32_t getBufferTime() { return mBufferTime; }
	virtual uint32_t getBufferSize() { return mBufferSize; }
	virtual uint32_t getChannels()   { return mChannels; }

private:
	int32_t setHwParams();
	int32_t setSwParams();

	snd_pcm_t         *mPcmHandle;
	char              *mDeviceName;

	bool               mIsInput;
	bool               mUseMmap;
	bool               mIsReady;

	uint32_t           mPeriodTime; // usec
	snd_pcm_uframes_t  mPeriodSize; // frames

	uint32_t           mBufferTime; // usec
	snd_pcm_uframes_t  mBufferSize; // frames

	uint32_t           mSampleRate;
	uint32_t           mChannels;
	uint32_t           mBytesPerFrame;
	uint32_t           mBitsPerChannel;
};

#endif // __DEVICE_ALSA_H__

