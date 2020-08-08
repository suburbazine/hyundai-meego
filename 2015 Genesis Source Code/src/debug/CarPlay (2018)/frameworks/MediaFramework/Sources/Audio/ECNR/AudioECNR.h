/*
	File:       AudioECNR.h
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

#ifndef __AUDIO_ECNR_H__
#define __AUDIO_ECNR_H__

#include <stdlib.h>
#include <unistd.h>

class AudioECNR {
public:
	virtual ~AudioECNR() {}

	virtual int start() = 0;
	virtual void stop() = 0;
	virtual int prepare() = 0;

	virtual void enable(Boolean enable) = 0;
	virtual Boolean isEnabled() = 0;

	virtual size_t getSampleFrames() = 0;
	virtual void setSampleFrames(size_t size) = 0;

	virtual void setBytesPerFrame(size_t size) = 0;
	virtual void setSampleRate(size_t size) = 0;
	virtual void setSiri(Boolean siri) = 0;
	virtual Boolean getSiri() = 0;

	virtual void * insertMicInBuffer(uint8_t *buf, size_t cnt) = 0;
	virtual void * insertRecvInBuffer(uint8_t *buf, size_t cnt) = 0;

	virtual int extractMicOutBuffer(uint8_t *buf, size_t cnt) = 0;
	virtual int extractRecvOutBuffer(uint8_t *buf, size_t cnt) = 0;

	virtual void signal() = 0;

	virtual Boolean isBufferingComplete(void) = 0;

	virtual void setConfigFilePath(const char * path) = 0; // must be called before prepare()
};

#endif // __AUDIO_ECNR_H__

