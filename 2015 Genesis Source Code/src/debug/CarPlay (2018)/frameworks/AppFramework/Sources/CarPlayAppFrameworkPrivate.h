/*
	File:       CarPlayAppFrameworkPrivate.h
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

#ifndef CARPLAYAPPFRAMEWORKPRIVATE_H_
#define CARPLAYAPPFRAMEWORKPRIVATE_H_

#include "CarPlayAppFramework.h"
#include "CarPlayAppFrameworkCommon.h"

#include <pthread.h>

// inSession: AirPlay Session
// inStream: AirPlay Stream
// inContext: Application Framework Handle

typedef void (*CarPlayPlatrformAudioStreamStarted_f)(void *inSession, void *inStream, void *inContext, AirPlayStreamType inStreamType, CFStringRef inAudioType, const void *inFormat);
typedef void (*CarPlayPlatrformAudioStreamStopped_f)(void *inSession, void *inStream, void *inContext, AirPlayStreamType inStreamType, CFStringRef inAudioType);
typedef void (*CarPlayPlatrformAudioSessionTornDown_f)(void *inSession, void *inContext);
typedef void (*CarPlayPlatrformScreenStreamStarted_f)(void *inSession, void *inStream, void *inContext);
typedef void (*CarPlayPlatrformScreenStreamPlayed_f)(void *inSession, void *inStream, void *inContext);
typedef void (*CarPlayPlatrformScreenStreamStopped_f)(void *inSession, void *inStream, void *inContext);
#if defined(KOECN)
typedef void (*CarPlayPlatrformScreenStreamProcessData_f)(void *inSession, void *inStream, uint8_t *data, int32_t size, void *inContext);
#endif
typedef Boolean (*CarPlayPlatrformCopyDeviceAvailable_f)(void *inSession, void *inContext, AirPlayStreamType inStreamType);
typedef void (*CarPlayPlatrformReportDeviceError_f)(void *inSession, void *inContext, AirPlayStreamType inStreamType, int32_t inErrorCode);

typedef struct {
	void *										context;
	CarPlayPlatrformAudioStreamStarted_f		audioStreamStarted_f;
	CarPlayPlatrformAudioStreamStopped_f		audioStreamStopped_f;
	CarPlayPlatrformAudioSessionTornDown_f		audioSessionTornDown_f;
	CarPlayPlatrformScreenStreamStarted_f		screenStreamStarted_f;
	CarPlayPlatrformScreenStreamPlayed_f		screenStreamPlayed_f;
	CarPlayPlatrformScreenStreamStopped_f		screenStreamStopped_f;
#if defined(KOECN)
	CarPlayPlatrformScreenStreamProcessData_f		screenStreamProcessData_f;
#endif
	CarPlayPlatrformCopyDeviceAvailable_f		copyDeviceAvailable_f;
	CarPlayPlatrformReportDeviceError_f			reportDeviceError_f;
} CarPlayAppFrameworkPlatformDelegate;

struct CarPlayAppFrameworkPrivate {
	pthread_t								carPlayThread;

	CarPlayAppFrameworkConfig *				config;
	CarPlayAppFrameworkDelegate				delegate;
	CarPlayAppFrameworkPlatformDelegate		platformDelegate;

	AirPlayModeChanges						initialModesRaw;
	CFDictionaryRef							initialModes;
	CarPlayAppFrameworkModeState            modeState;
	ScreenRef								mainScreen;
	HIDDeviceRef							touchHID;
	HIDDeviceRef							multiTouchHID;
	HIDDeviceRef							dPadHID;
	HIDDeviceRef							knobHID;
	HIDDeviceRef							buttonHID;
	HIDDeviceRef							telephonyHID;
	HIDDeviceRef							trackpadHID;
	AirPlayReceiverServerRef				airPlayServer;
	AirPlayReceiverSessionRef				airPlaySession;
	CFStringRef								deviceID;
	CFMutableArrayRef						bluetoothIDs;
	CFMutableArrayRef						limitedUIElements;

	CarPlayAppFrameworkAudioConfig *		audioConfig;
	Boolean									isAudioConfigured;
	Boolean 								isAudioMICMuted;

	pthread_mutex_t							pendingTaskLock;
	pthread_mutex_t	*						pendingTaskLockPtr;
	CFMutableDictionaryRef					pendingTaskMap;

    pthread_mutex_t                         pendingRespLock;
    pthread_cond_t                          pendingRespCond;
    AirPlayStreamType                       pendingRespStreamType;

	CarPlayAppFrameworkHIDInfoList *		hidList;

	char									clientName[128];
	char									clientModel[32];
	char									clientOSBuildVersion[32];
	uint64_t								clientDeviceID;
	uint32_t                                     volInfo;
	Boolean                                     isAltAudioMuted;
};

#define CHECK_DELEGATE(h, f)		((h) && (&(h)->delegate) && (h)->delegate.f)
#define CALL_DELEGATE(h, f, ...)	(h)->delegate.f(__VA_ARGS__)

#define CONVERT_ERROR(x) ((x) >= 0 ? kCarPlayResult_OK : kCarPlayResult_Error)

// Do NOT use keys below, it is used internally.
// [Pointer]
#define kFrameworkHandle			"frameworkHandle"

#define kLastestiOS8OSBuildVersion	"12H321"

#define BLUETOOTH_ID_LENGTH (12 + 5)	// HH:HH:HH:HH:HH:HH

#define AirPlayStreamTypeToCarPlayStreamType( X ) \
	( ( (X) == kAirPlayStreamType_GeneralAudio )	? kCarPlayStreamType_GeneralAudio	: \
	  ( (X) == kAirPlayStreamType_MainAudio )		? kCarPlayStreamType_MainAudio		: \
	  ( (X) == kAirPlayStreamType_AltAudio )		? kCarPlayStreamType_AltAudio		: \
	  ( (X) == kAirPlayStreamType_Screen )			? kCarPlayStreamType_Screen			: \
			  	  	  	  	  	  	  	  	  	  	  kCarPlayStreamType_Invalid )

#define CarPlayStreamTypeToAirPlayStreamType( X ) \
    ( ( (X) == kCarPlayStreamType_GeneralAudio )    ? kAirPlayStreamType_GeneralAudio   : \
      ( (X) == kCarPlayStreamType_MainAudio )       ? kAirPlayStreamType_MainAudio      : \
      ( (X) == kCarPlayStreamType_AltAudio )        ? kAirPlayStreamType_AltAudio       : \
      ( (X) == kCarPlayStreamType_Screen )          ? kAirPlayStreamType_Screen         : \
                                                      kAirPlayStreamType_Invalid )

#define CarPlayAudioTypeFromCFString( X )	MapCFStringToValue( (X), kCarPlayAudioType_Invalid, \
	CFSTR( kAirPlayAudioType_Default ),				kCarPlayAudioType_Default, \
	CFSTR( kAirPlayAudioType_Alert ),				kCarPlayAudioType_Alert, \
	CFSTR( kAirPlayAudioType_Media ),				kCarPlayAudioType_Media, \
	CFSTR( kAirPlayAudioType_Measurement ),			kCarPlayAudioType_Measurement, \
	CFSTR( kAirPlayAudioType_Telephony ),			kCarPlayAudioType_Telephony, \
	CFSTR( kAirPlayAudioType_SpeechRecognition ),	kCarPlayAudioType_SpeechRecognition, \
	CFSTR( kAirPlayAudioType_SpokenAudio ),			kCarPlayAudioType_SpokenAudio, \
	NULL )

#endif /* CARPLAYAPPFRAMEWORKPRIVATE_H_ */
