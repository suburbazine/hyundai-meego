/*
	File:       CarPlayAppFrameworkPlatformCallbacks.c
	Package: 	LG CarPlay Application Framework.
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

#include "CarPlayAppFrameworkPlatformCallbacks.h"
#include "CarPlayAppFrameworkPrivate.h"

void _CarPlayHandlePlatformAudioStreamStarted(void *inSession, void *inStream, void *inContext,
					AirPlayStreamType inStreamType, CFStringRef inAudioType, const void *inFormat)
{
	struct CarPlayAppFrameworkPrivate *	handle 	  = inContext;
	CarPlayAudioType        			audioType = kCarPlayAudioType_Invalid;

	app_ulog(kLogLevelNotice, "%s called(%s, %s)\n", __FUNCTION__, 
                                                     AirPlayStreamTypeToString(inStreamType), 
                                                     CFStringGetCStringPtr(inAudioType, kCFStringEncodingUTF8));

	audioType = inAudioType ? CarPlayAudioTypeFromCFString(inAudioType) : kCarPlayAudioType_Invalid;

	if (CHECK_DELEGATE(handle, audioStreamStarted_f)) {
		CALL_DELEGATE(handle, audioStreamStarted_f,
				(void*)inSession,
				(void *)inStream,
				AirPlayStreamTypeToCarPlayStreamType(inStreamType),
				audioType,
				(CarPlayAudioStreamDescription*)inFormat);
	}
	return;
}

void _CarPlayHandlePlatformAudioStreamStopped(void *inSession, void *inStream, void *inContext,
					AirPlayStreamType inStreamType, CFStringRef inAudioType)
{
	struct CarPlayAppFrameworkPrivate *	handle 	  = inContext;
	CarPlayAudioType        			audioType = kCarPlayAudioType_Invalid;

	app_ulog(kLogLevelNotice, "%s called(%s, %s)\n", __FUNCTION__, 
                                                     AirPlayStreamTypeToString(inStreamType), 
                                                     CFStringGetCStringPtr(inAudioType, kCFStringEncodingUTF8));

	audioType = inAudioType ? CarPlayAudioTypeFromCFString(inAudioType) : kCarPlayAudioType_Invalid;

	if (CHECK_DELEGATE(handle, audioStreamStopped_f)) {
		CALL_DELEGATE(handle, audioStreamStopped_f,
				(void*)inSession,
				(void *)inStream,
				AirPlayStreamTypeToCarPlayStreamType(inStreamType),
				audioType);
	}
	return;
}

void _CarPlayHandlePlatformAudioSessionTornDown(void *inSession, void *inContext)
{
	struct CarPlayAppFrameworkPrivate *	handle = inContext;

	if (CHECK_DELEGATE(handle, audioSessionTornDown_f)) {
		CALL_DELEGATE(handle, audioSessionTornDown_f, (void *)inSession);
	}
	return;
}

void _CarPlayHandlePlatformScreenStreamStarted(void *inSession, void *inStream, void *inContext)
{
	struct CarPlayAppFrameworkPrivate *	handle = inContext;

	if (CHECK_DELEGATE(handle, screenStreamStarted_f)) {
		CALL_DELEGATE(handle, screenStreamStarted_f, (void *)inSession, (void *)inStream);
	}
	return;
}

void _CarPlayHandlePlatformScreenStreamStopped(void *inSession, void *inStream, void *inContext)
{
	struct CarPlayAppFrameworkPrivate *	handle = inContext;

	if (CHECK_DELEGATE(handle, screenStreamStopped_f)) {
		CALL_DELEGATE(handle, screenStreamStopped_f, (void *)inSession, (void *)inStream);
	}
	return;
}

#if defined(KOECN)
void _CarPlayHandlePlatformScreenStreamProcessData(void *inSession, void *inStream, uint8_t *data, int32_t size, void *inContext)
{
	struct CarPlayAppFrameworkPrivate *	handle = inContext;

	if (CHECK_DELEGATE(handle, screenStreamProcessData_f)) {
		CALL_DELEGATE(handle, screenStreamProcessData_f, (void *)inSession, (void *)inStream, (uint8_t*)data, size);
	}
	return;
}
#endif

void _CarPlayHandlePlatformScreenStreamPlayed(void *inSession, void *inStream, void *inContext)
{
	struct CarPlayAppFrameworkPrivate *	handle = inContext;

	if (CHECK_DELEGATE(handle, screenStreamPlayed_f)) {
		CALL_DELEGATE(handle, screenStreamPlayed_f, (void *)inSession, (void *)inStream);
	}
	return;
}

Boolean _CarPlayHandlePlatformCopyDeviceAvailable(void *inSession, void *inContext, AirPlayStreamType inStreamType)
{
	struct CarPlayAppFrameworkPrivate *	handle = inContext;
	int32_t ret = 1;

	if (CHECK_DELEGATE(handle, copyDeviceAvailable_f)) {
		ret = CALL_DELEGATE(handle, copyDeviceAvailable_f, (void *)inSession, AirPlayStreamTypeToCarPlayStreamType(inStreamType));
	}

	return ret;
}

void _CarPlayHandlePlatformReportDeviceError(void *inSession, void *inContext, AirPlayStreamType inStreamType, int32_t inErrorCode)
{
	struct CarPlayAppFrameworkPrivate *	handle = inContext;

	if (CHECK_DELEGATE(handle, reportDeviceError_f)) {
		CALL_DELEGATE(handle, reportDeviceError_f, (void *)inSession, AirPlayStreamTypeToCarPlayStreamType(inStreamType), inErrorCode);
	}

	return;
}
