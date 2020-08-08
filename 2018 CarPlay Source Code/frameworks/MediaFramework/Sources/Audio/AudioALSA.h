/*
	File:       AudioALSA.h
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

#ifndef __AUDIO_ALSA_H__
#define __AUDIO_ALSA_H__

#include <stdlib.h>



#include <unistd.h>

class AudioALSA {
public:
	virtual ~AudioALSA() {}

	virtual int32_t open() = 0;
	virtual int32_t close() = 0;
	virtual bool isReady() = 0;

	virtual int32_t set(char *name, bool isInput, uint32_t sampleRate, uint32_t channels,
			uint32_t bitsPerChannel, uint32_t periodTime, uint32_t bufferTime) = 0;
	virtual int32_t set(char *name, bool isInput, uint32_t sampleRate, uint32_t channels,
			uint32_t bitsPerChannel, uint32_t periodTime, uint32_t bufferTime, char *audioType) = 0;
	virtual int32_t prepare() = 0;

	virtual int32_t write(uint8_t *buffer, int reqFrames) = 0;
	virtual int32_t read(uint8_t * buffer, int reqFrames) = 0;

	virtual int32_t getDelay() = 0;
	virtual uint32_t getAvail() = 0;
	virtual uint32_t getSampleRate() = 0;
	virtual uint32_t getPeriodTime() = 0;
	virtual uint32_t getPeriodSize() = 0;
	virtual uint32_t getBufferTime() = 0;
	virtual uint32_t getBufferSize() = 0;
	virtual uint32_t getChannels() = 0;
};

#endif // __AUDIO_ALSA_H__

