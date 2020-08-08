/*
	File:       CarPlayAppFrameworkSessionCallbacks.c
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

#include <errno.h>

#include "CarPlayAppFrameworkSessionCallbacks.h"
#include "CarPlayAppFrameworkPrivate.h"
#include "CarPlayAppFrameworkSessionCallbacks.h"
#include "SettingUtils.h"

//===========================================================================================================================
//	_CarPlayHandleSessionGetSetupResponseDelay
//===========================================================================================================================

#define __DEFAULT_SETUP_REQUEST_DELAY	2
static uint8_t	_CarPlayHandleSessionGetSetupResponseDelay( void )
{
	uint8_t ret = __DEFAULT_SETUP_REQUEST_DELAY;
#if( defined( SETUP_REQUEST_DELAY_SEC ) )
	ret = SETUP_REQUEST_DELAY_SEC;
#endif
	return ret;
}

//===========================================================================================================================
//	_CarPlayHandleSessionFinalized
//===========================================================================================================================

void _CarPlayHandleSessionFinalized(AirPlayReceiverSessionRef inSession, void *inContext)
{
	struct CarPlayAppFrameworkPrivate *handle = inContext;

    // Update Initial Mode's app state.
    if (handle->modeState.speech == kAirPlayEntity_Accessory)  handle->initialModesRaw.speech = handle->modeState.speechMode;
    else                                                       handle->initialModesRaw.speech = kAirPlaySpeechMode_None;

    if (handle->modeState.phoneCall == kAirPlayEntity_Accessory)  handle->initialModesRaw.phoneCall = kAirPlayTriState_True;
    else                                                       handle->initialModesRaw.phoneCall = kAirPlayTriState_False;

    if (handle->modeState.turnByTurn == kAirPlayEntity_Accessory)  handle->initialModesRaw.turnByTurn = kAirPlayTriState_True;
    else                                                       handle->initialModesRaw.turnByTurn = kAirPlayTriState_False;

    // Session is NLL.
	handle->airPlaySession = NULL;

	// Cleaning Client Information
	memset(handle->clientName, 			 0, sizeof(handle->clientName));
	memset(handle->clientModel, 		 0, sizeof(handle->clientModel));
	memset(handle->clientOSBuildVersion, 0, sizeof(handle->clientOSBuildVersion));

	app_ulog(kLogLevelWarning, "AirPlay session ended\n");

	if (CHECK_DELEGATE(handle, sessionFinalized_f)) {
		CALL_DELEGATE(handle, sessionFinalized_f, (void *)inSession);
	}
}

//===========================================================================================================================
//	_CarPlayHandleSessionCopyProperty
//===========================================================================================================================

CFTypeRef _CarPlayHandleSessionCopyProperty(
		AirPlayReceiverSessionRef	inSession,
		CFStringRef					inProperty,
		CFTypeRef					inQualifier,
		OSStatus *					outErr,
		void *						inContext)
{
	struct CarPlayAppFrameworkPrivate *handle = inContext;

	CFTypeRef	value = NULL;
	OSStatus	err;

	(void) inSession;
	(void) inQualifier;

	if (inProperty != NULL)
		app_ulog(kLogLevelNotice, "%s: inProperty = %s\n", __FUNCTION__, CFStringGetCStringPtr(inProperty, kCFStringEncodingUTF8));

	// Modes

	if (CFEqual(inProperty, CFSTR(kAirPlayProperty_Modes))) {
		handle->initialModes = (CFDictionaryRef)AirPlayCreateModesDictionary(&handle->initialModesRaw, NULL, &err);
		require_noerr(err, exit);

		value = handle->initialModes;
		require_action_quiet(value, exit, err = kNotHandledErr);
		CFRetain(value);
	} else {
		err = kNotHandledErr;
		goto exit;
	}
	err = kNoErr;

exit:
	if (outErr) *outErr = err;
	return(value);
}

//===========================================================================================================================
//	_CarPlayHandleModesChanged
//===========================================================================================================================

void _CarPlayHandleModesChanged(
		AirPlayReceiverSessionRef 	inSession,
		const AirPlayModeState *	inState,
		void *						inContext)
{
	struct CarPlayAppFrameworkPrivate *handle = inContext;

	app_ulog(kLogLevelNotice, "Modes changed: screen %s, mainAudio %s, speech %s (%s), phone %s, turns %s\n",
		AirPlayEntityToString(inState->screen),
		AirPlayEntityToString(inState->mainAudio),
		AirPlayEntityToString(inState->speech.entity),
		AirPlaySpeechModeToString(inState->speech.mode),
		AirPlayEntityToString(inState->phoneCall),
		AirPlayEntityToString(inState->turnByTurn));

	if (CHECK_DELEGATE(handle, modesChanged_f)) {
		CarPlayAppFrameworkModeState state;
		state.screen = inState->screen;
		state.mainAudio = inState->mainAudio;
		state.speech = inState->speech.entity;
		state.speechMode = inState->speech.mode;
		state.phoneCall = inState->phoneCall;
		state.turnByTurn = inState->turnByTurn;

		memcpy(&handle->modeState, &state, sizeof(CarPlayAppFrameworkModeState));

		CALL_DELEGATE(handle, modesChanged_f, (void *)inSession, &state);
	}
}

//===========================================================================================================================
//	_CarPlayHandleRequestUI
//===========================================================================================================================

void _CarPlayHandleRequestUI(AirPlayReceiverSessionRef inSession, void *inContext)
{
	struct CarPlayAppFrameworkPrivate *handle = inContext;

	app_ulog(kLogLevelNotice, "Request accessory UI\n");

	if (CHECK_DELEGATE(handle, requestUI_f)) {
		CALL_DELEGATE(handle, requestUI_f, (void *)inSession);
	}
}

//===========================================================================================================================
//	_CarPlayHandleSessionControl
//===========================================================================================================================

OSStatus _CarPlayHandleSessionControl(
		AirPlayReceiverSessionRef 	inSession,
 		CFStringRef         		inCommand,
        CFTypeRef           		inQualifier,
        CFDictionaryRef     		inParams,
        CFDictionaryRef *   		outParams,
		void *						inContext)
{
	struct CarPlayAppFrameworkPrivate *handle = inContext;

	OSStatus 		err;
	CarPlayCommand	inRequest = 0;

    (void) inQualifier;
    (void) inParams;
    (void) outParams;

	if (CFEqual(inCommand, CFSTR(kAirPlayCommand_ConfigChanged))) {
		app_ulog(kLogLevelNotice, "Config Changed session control request\n");
		inRequest = kCarPlayCommand_ConfigChanged;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_UpdateBonjour))) {
		app_ulog(kLogLevelNotice, "Update Bonjour session control request\n");
		inRequest = kCarPlayCommand_UpdateBonjour;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_SetConfig))) {
		app_ulog(kLogLevelNotice, "Set Config session control request\n");
		inRequest = kCarPlayCommand_SetConfig;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_ApplyConfig))) {
		app_ulog(kLogLevelNotice, "Apply Config session control request\n");
		inRequest = kCarPlayCommand_ApplyConfig;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_DuckAudio))) {
		app_ulog(kLogLevelNotice, "Duck Audio session control request\n");
		inRequest = kCarPlayCommand_DuckAudio;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_UnduckAudio))) {
		app_ulog(kLogLevelNotice, "Unduck audio session control request\n");
		inRequest = kCarPlayCommand_UnduckAudio;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_FlushAudio))) {
		app_ulog(kLogLevelNotice, "Flush Audio session control request\n");
		inRequest = kCarPlayCommand_FlushAudio;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_DisableBluetooth))) {
		app_ulog(kLogLevelNotice, "Disable Bluetooth session control request\n");
		inRequest = kCarPlayCommand_DisableBluetooth;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_UpdateFeedback))) {
		app_ulog(kLogLevelNotice, "Update Feedback session control request\n");
		inRequest = kCarPlayCommand_UpdateFeedback;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_HIDSendReport))) {
		app_ulog(kLogLevelNotice, "HID Send Report session control request\n");
		inRequest = kCarPlayCommand_HIDSendReport;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_SetHIDReport))) {
		app_ulog(kLogLevelNotice, "Set HID Report session control request\n");
		inRequest = kCarPlayCommand_SetHIDReport;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_HIDCopyInputMode))) {
		app_ulog(kLogLevelNotice, "HID Copy Input Mode session control request\n");
		inRequest = kCarPlayCommand_HIDCopyInputMode;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_HIDSetInputMode))) {
		app_ulog(kLogLevelNotice, "HID Set Input Mode session control request\n");
		inRequest = kCarPlayCommand_HIDSetInputMode;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_ForceKeyFrame))) {
		app_ulog(kLogLevelNotice, "Force Key Frame session control request\n");
		inRequest = kCarPlayCommand_ForceKeyFrame;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_ChangeModes))) {
		app_ulog(kLogLevelNotice, "Change Modes session control request\n");
		inRequest = kCarPlayCommand_ChangeModes;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_ModesChanged))) {
		app_ulog(kLogLevelNotice, "Modes Changed session control request\n");
		inRequest = kCarPlayCommand_ModesChanged;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_UpdatePrefs))) {
		app_ulog(kLogLevelNotice, "Update Prefs session control request\n");
		inRequest = kCarPlayCommand_UpdatePrefs;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_RequestSiri))) {
		app_ulog(kLogLevelNotice, "Request Siri session control request\n");
		inRequest = kCarPlayCommand_RequestSiri;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_RequestUI))) {
		app_ulog(kLogLevelNotice, "Request UI session control request\n");
		inRequest = kCarPlayCommand_RequestUI;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_SetNightMode))) {
		app_ulog(kLogLevelNotice, "Set Night Mode session control request\n");
		inRequest = kCarPlayCommand_SetNightMode;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_SetLimitedUI))) {
		app_ulog(kLogLevelNotice, "Set Limited UI session control request\n");
		inRequest = kCarPlayCommand_SetLimitedUI;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_StartServer))) {
		app_ulog(kLogLevelNotice, "Start Server session control request\n");
		inRequest = kCarPlayCommand_StartServer;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_StopServer))) {
		app_ulog(kLogLevelNotice, "Stop Server session control request\n");
		inRequest = kCarPlayCommand_StopServer;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_SessionDied))) {
		app_ulog(kLogLevelNotice, "Session Died session control request\n");
		inRequest = kCarPlayCommand_SessionDied;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_StartSession))) {
		app_ulog(kLogLevelNotice, "Start Session session control request\n");
		inRequest = kCarPlayCommand_StartSession;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_StopSession))) {
		app_ulog(kLogLevelNotice, "Stop Session session control request\n");
		inRequest = kCarPlayCommand_StopSession;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_SetUpStreams))) {
		app_ulog(kLogLevelNotice, "Set Up Streams session control request\n");
		inRequest = kCarPlayCommand_SetUpStreams;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_TearDownStreams))) {
		app_ulog(kLogLevelNotice, "Tear Down Streams session control request\n");
		inRequest = kCarPlayCommand_TearDownStreams;
		err = kNoErr;
	} else if (CFEqual(inCommand, CFSTR(kAirPlayCommand_UpdateTimestamps))) {
		app_ulog(kLogLevelNotice, "Update Timestamps session control request\n");
		inRequest = kCarPlayCommand_UpdateTimestamps;
		err = kNoErr;
	} else {
		app_ulog(kLogLevelError, "Unsupported session control request\n");
		err = kNotHandledErr;
	}

	if (err != kNotHandledErr) {
		if (CHECK_DELEGATE(handle, sessionControl_f)) {
			CALL_DELEGATE(handle, sessionControl_f, (void *)inSession, inRequest);
		}
	}

	return (err);
}

//===========================================================================================================================
//	_CarPlayHandleDuckAudio
//===========================================================================================================================

void _CarPlayHandleDuckAudio(
		AirPlayReceiverSessionRef	inSession,
		double						inDurationSecs,
		double						inVolume,
		void *						inContext)
{
	struct CarPlayAppFrameworkPrivate *handle = inContext;

	app_ulog(kLogLevelNotice, "Duck audio\n");

	if (CHECK_DELEGATE(handle, duckAudio_f)) {
		CALL_DELEGATE(handle, duckAudio_f, (void *)inSession, inDurationSecs, inVolume);
	}

	return;
}

//===========================================================================================================================
//	_CarPlayHandleUnduckAudio
//===========================================================================================================================

void _CarPlayHandleUnduckAudio(
		AirPlayReceiverSessionRef	inSession,
		double						inDurationSecs,
		void *						inContext)
{
	struct CarPlayAppFrameworkPrivate *handle = inContext;

    app_ulog(kLogLevelNotice, "Unduck audio\n");

	if (CHECK_DELEGATE(handle, unduckAudio_f)) {
		CALL_DELEGATE(handle, unduckAudio_f, (void *)inSession, inDurationSecs);
	}

	return;
}

//===========================================================================================================================
//	_CarPlayHandleSetupRequest
//===========================================================================================================================

void _CarPlayHandleSetupRequest(
		AirPlayReceiverSessionRef	inSession,
		AirPlayStreamType			inStreamType,
		CFStringRef					inAudioType,
		Boolean						inInput,
		AudioStreamBasicDescription *inFormat)
{
	struct CarPlayAppFrameworkPrivate *handle = NULL;
	OSStatus				        err;
	CarPlayAudioType		        audioType;
    CarPlayAudioStreamDescription   format;
	int64_t					        value 		= 0;
	int32_t					        steamReady 	= kCarPlayStreamReadyType_Ready;
    uint64_t                        startTime   = 0;
	struct timespec 		        ts;

	// Set framework handle

	value = CarPlaySettings_GetInt64(kFrameworkHandle, &err);
	require_noerr(err, exit);

	handle = (struct CarPlayAppFrameworkPrivate *)((uintptr_t)value);

	// Set audio Type to CarPlayAudioType

	audioType = inAudioType ? CarPlayAudioTypeFromCFString(inAudioType) : kCarPlayAudioType_Invalid;

	// Call delegate callback function
    app_ulog(kLogLevelNotice, "### %s Setup request called : %s(0x%4x) type\n",
    		AirPlayStreamTypeToString(inStreamType),
			inAudioType ? CFStringGetCStringPtr(inAudioType, kCFStringEncodingUTF8) : "Not Audio",
			audioType);

    // Set audio stream format
    if (inFormat !=NULL) {
        format.mSampleRate       = inFormat->mSampleRate;
        format.mBytesPerPacket   = inFormat->mBytesPerPacket;
        format.mFramesPerPacket  = inFormat->mFramesPerPacket;
        format.mBytesPerFrame    = inFormat->mBytesPerFrame;
        format.mChannelsPerFrame = inFormat->mChannelsPerFrame;
        format.mBitsPerChannel   = inFormat->mBitsPerChannel;
    }

    pthread_mutex_lock(&handle->pendingRespLock);

	if (CHECK_DELEGATE(handle, setupRequest_f)){
		steamReady = CALL_DELEGATE(handle, setupRequest_f,
						(void *)inSession,
						AirPlayStreamTypeToCarPlayStreamType(inStreamType),
						audioType,
						inInput ?  1 : 0,
						&format);
	}

    handle->pendingRespStreamType = AirPlayStreamTypeToCarPlayStreamType(inStreamType);
    require_action_quiet(handle->pendingRespStreamType != kCarPlayStreamType_Invalid, exit, err = kUnsupportedErr);

    // Wait for ready to audio or screen
    if (steamReady != kCarPlayStreamReadyType_Ready) {
		app_ulog(kLogLevelWarning, "### wait for %s Setup response\n", AirPlayStreamTypeToString(inStreamType));

        startTime = UpMilliseconds();
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += _CarPlayHandleSessionGetSetupResponseDelay();

        // Wait to ready devices
        err = pthread_cond_timedwait(&handle->pendingRespCond, &handle->pendingRespLock, &ts);
        if (err == ETIMEDOUT) {
			app_ulog(kLogLevelError, "### [Timeout!] %s Setup response is delayed %llu msec over.\n",
					AirPlayStreamTypeToString(inStreamType),
					(UpMilliseconds() - startTime));
		} else {
			app_ulog(kLogLevelWarning, "### %s Setup response %llu msec delayed\n",
					AirPlayStreamTypeToString(inStreamType),
					(UpMilliseconds() - startTime));
		}
    }
    handle->pendingRespStreamType = kCarPlayStreamType_Invalid;
    
    pthread_mutex_unlock(&handle->pendingRespLock);

    err = kNoErr;

exit:

    if (err)
        app_ulog(kLogLevelError, "%s : No delayed response(%d)\n", __FUNCTION__, err);

    return;
}

//===========================================================================================================================
//	_CarPlayHandleDisableBluetooth
//===========================================================================================================================

void _CarPlayHandleDisableBluetooth(
		AirPlayReceiverSessionRef	inSession,
		CFStringRef					inDeviceID)
{
	struct CarPlayAppFrameworkPrivate *handle = NULL;
	OSStatus				err;
	int64_t					value 	 = 0;
	const char *			deviceID = NULL;

	// Set framework handle

	value = CarPlaySettings_GetInt64(kFrameworkHandle, &err);
	require_noerr(err, exit);

	handle = (struct CarPlayAppFrameworkPrivate *)((uintptr_t)value);
	require(handle != NULL, exit);

	// Set bluetooth device ID

	deviceID = CFStringGetCStringPtr(inDeviceID, kCFStringEncodingUTF8);

    app_ulog(kLogLevelNotice, "### Disable Bluetooth called! device ID (%s)\n", deviceID);

	// Call delegate callback function

	if (CHECK_DELEGATE(handle, disableBluetooth_f)){
		CALL_DELEGATE(handle, disableBluetooth_f,
				(void *)inSession,
				deviceID);
	}

exit:

    return;
}
