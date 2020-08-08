/*
	File:       CarPlayAppFramework.c
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

#include "CarPlayAppFramework.h"

#include "CarPlayAppFrameworkCommon.h"
#include "CarPlayAppFrameworkPrivate.h"
#include "CarPlayAppFrameworkServerCallbacks.h"
#include "CarPlayAppFrameworkPlatformCallbacks.h"
#include "CarPlayAppFrameworkHIDs.h"

#include "AirPlayCommon.h"
#include "AirPlayReceiverServer.h"
#include "AirPlayReceiverSession.h"
#include "AirPlayVersion.h"
#include "AirPlayUtils.h"
#include "APSDebugServices.h"
#include "HIDUtils.h"
#include "ScreenUtils.h"
#include "AudioUtils.h"
#include "MathUtils.h"
#include "UUIDUtils.h"
#include "StringUtils.h"
//#include "AirPlaySettings.h"
#include "SettingUtils.h"

#if defined( USE_EXTERNEL_LOG_SYSTEM )
	#include "ExternelLogUtils.h"
#endif

#if (!TARGET_OS_ANDROID)
	#include <bits/types.h>
#endif
#include <signal.h>
#include <pthread.h>

//===========================================================================================================================
//	Definitions
//===========================================================================================================================

ulog_define(CarPlayAppFramework, kLogLevelTrace, kLogFlags_Default, "CarPlayAppFramework", NULL);

typedef OSStatus (*HIDCreateDescriptor_f)(uint8_t **outDescriptor, size_t *outLen);

//===========================================================================================================================
//	Declarations
//===========================================================================================================================

static void _CarPlayHandleChangeModesComplete( OSStatus inStatus, CFDictionaryRef inResponse, void *inContext );
static void *	_CarPlayAppFrameworkThread(void* inArg);

OSStatus _CarPlayAppFrameworkRegisterDevice(
		HIDDeviceRef *inDevice,
		HIDCreateDescriptor_f inCreateDescriptorFunc,
		CFStringRef inUUID,
		CFStringRef inDeviceName,
		CarPlayAppFrameworkHIDInfo *hidInfo);

//===========================================================================================================================
//	CarPlayAppFrameworkInitialize
//===========================================================================================================================

int32_t	CarPlayAppFrameworkInitialize(CarPlayAppFrameworkRef *outRef)
{
	struct CarPlayAppFrameworkPrivate *	handle = NULL;
	OSStatus							err = kUnknownErr;

	require(outRef != NULL, exit);

#if defined( USE_EXTERNEL_LOG_SYSTEM )
	err = ExternelLogWriteInit();
	if (err) ExternelLogWriteStop();
#endif

	app_ulog(kLogLevelWarning, "%s: Carplay version %s\n", __FUNCTION__, kCarPlayVersionStr);

	app_ulog(kLogLevelWarning, "%s: AirPlay starting version %s\n", __FUNCTION__, kAirPlaySourceVersionStr);
	signal(SIGPIPE, SIG_IGN); // Ignore SIGPIPE signals so we get EPIPE errors from APIs instead of a signal.

	handle = (struct CarPlayAppFrameworkPrivate *)calloc(1, sizeof(struct CarPlayAppFrameworkPrivate));
	require(handle != NULL, exit);

	// This handle can be used only within same process!
	err = CarPlaySettings_SetInt64(kFrameworkHandle, (uintptr_t)handle);
	require_noerr(err, exit);
	//AirPlaySettings_Synchronize();

    // Initialize AirPlayModeChanges
    AirPlayModeChangesInit(&handle->initialModesRaw);
    handle->initialModesRaw.screen.type                             = kAirPlayTransferType_Take;
    handle->initialModesRaw.screen.priority                         = kAirPlayTransferPriority_UserInitiated;
    handle->initialModesRaw.screen.takeConstraint                   = kAirPlayConstraint_UserInitiated;
    handle->initialModesRaw.screen.borrowOrUnborrowConstraint       = kAirPlayConstraint_Anytime;

    handle->initialModesRaw.mainAudio.type							= kAirPlayTransferType_Take;
    handle->initialModesRaw.mainAudio.priority						= kAirPlayTransferPriority_UserInitiated;
    handle->initialModesRaw.mainAudio.takeConstraint				= kAirPlayConstraint_UserInitiated;
    handle->initialModesRaw.mainAudio.borrowOrUnborrowConstraint	= kAirPlayConstraint_Anytime;

    handle->initialModesRaw.speech 		= kAirPlaySpeechMode_None;
    handle->initialModesRaw.phoneCall 	= kAirPlayTriState_False;
    handle->initialModesRaw.turnByTurn 	= kAirPlayTriState_False;

	// Register platform specific delegates
	handle->platformDelegate.audioStreamStarted_f 	= _CarPlayHandlePlatformAudioStreamStarted;
	handle->platformDelegate.audioStreamStopped_f 	= _CarPlayHandlePlatformAudioStreamStopped;
	handle->platformDelegate.audioSessionTornDown_f = _CarPlayHandlePlatformAudioSessionTornDown;
	handle->platformDelegate.screenStreamStarted_f 	= _CarPlayHandlePlatformScreenStreamStarted;
	handle->platformDelegate.screenStreamStopped_f 	= _CarPlayHandlePlatformScreenStreamStopped;
#if defined(KOECN)
    handle->platformDelegate.screenStreamProcessData_f      = _CarPlayHandlePlatformScreenStreamProcessData;
#endif
	handle->platformDelegate.screenStreamPlayed_f 	= _CarPlayHandlePlatformScreenStreamPlayed;
	handle->platformDelegate.copyDeviceAvailable_f 	= _CarPlayHandlePlatformCopyDeviceAvailable;
	handle->platformDelegate.reportDeviceError_f	= _CarPlayHandlePlatformReportDeviceError;
	// The lock and map for pending task will interrupt pending tasks or finite loop.
	// NOTE: This lock and map can be used only within same process
	err = pthread_mutex_init(&handle->pendingTaskLock, NULL);
	require_noerr(err, exit);
	handle->pendingTaskLockPtr = &handle->pendingTaskLock;

	handle->pendingTaskMap = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	require_action(handle->pendingTaskMap, exit, err = kNoMemoryErr);

    // This lock and cond for pending task will response "SETUP Request".
    err = pthread_mutex_init(&handle->pendingRespLock, NULL);
    require_noerr(err, exit);

    err = pthread_cond_init(&handle->pendingRespCond, NULL);
    require_noerr(err, exit);

	*outRef = (void*)handle;

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkSetConfig
//===========================================================================================================================

int32_t	CarPlayAppFrameworkSetConfig(CarPlayAppFrameworkRef inRef, const CarPlayAppFrameworkConfig *inConfig)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus err = kUnknownErr;

	app_ulog(kLogLevelNotice, "%s\n", __FUNCTION__);

	require(handle != NULL, exit);
	require(inConfig != NULL, exit);

	handle->config = (CarPlayAppFrameworkConfig *)inConfig;

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

int32_t CarPlayAppFrameworkSetSurface(CarPlayAppFrameworkRef inRef, const uint32_t renderLayer)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus err = kUnknownErr;

	app_ulog(kLogLevelNotice, "%s\n", __FUNCTION__);

	require(handle != NULL, exit);
	require(handle->config != NULL, exit);

	handle->config->renderLayer = renderLayer;

	if (handle->mainScreen) {
		ScreenSetPropertyInt64(handle->mainScreen, kScreenProperty_PlatformLayer,
				NULL, handle->config->renderLayer);
	}

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//     CarPlayAppFrameworkSetRequiredAudioFormat
//===========================================================================================================================

int32_t CarPlayAppFrameworkSetRequiredAudioFormat(CarPlayAppFrameworkAudioConfig *outConfig)
{
    OSStatus err = kUnknownErr;

    app_ulog(kLogLevelNotice, "%s\n", __FUNCTION__);

    require(outConfig != NULL, exit);

    outConfig->media.outputFormat        		=   kCarPlayAudioFormat_PCM_44KHz_16Bit_Stereo
                                        		 |  kCarPlayAudioFormat_PCM_48KHz_16Bit_Stereo ;
    outConfig->media.inputFormat         		=   0;

    outConfig->telephony.outputFormat    		=   kCarPlayAudioFormat_PCM_8KHz_16Bit_Mono
                                        		 |  kCarPlayAudioFormat_PCM_16KHz_16Bit_Mono
                                        		 |  kCarPlayAudioFormat_PCM_24KHz_16Bit_Mono ;
    outConfig->telephony.inputFormat     		=   outConfig->telephony.outputFormat ;

    outConfig->speechRecognition.outputFormat    =   kCarPlayAudioFormat_PCM_24KHz_16Bit_Mono ;
    outConfig->speechRecognition.inputFormat     =   outConfig->speechRecognition.outputFormat ;

    outConfig->alert.outputFormat        		=   kCarPlayAudioFormat_PCM_44KHz_16Bit_Stereo
                                        		 |  kCarPlayAudioFormat_PCM_48KHz_16Bit_Stereo ;
    outConfig->alert.inputFormat         		=   0;

    outConfig->deflt.outputFormat        		=   kCarPlayAudioFormat_PCM_16KHz_16Bit_Mono
                                        		 |  kCarPlayAudioFormat_PCM_24KHz_16Bit_Mono ;
    outConfig->deflt.inputFormat         		=   outConfig->deflt.outputFormat;

    outConfig->alt.outputFormat          		=   kCarPlayAudioFormat_PCM_44KHz_16Bit_Stereo
                                        		 |  kCarPlayAudioFormat_PCM_48KHz_16Bit_Stereo ;
    outConfig->alt.inputFormat           		=   0;

    err = kNoErr;
exit:
    if (err) {
        app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
    }

    return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkSetAudioConfig
//===========================================================================================================================

int32_t CarPlayAppFrameworkSetAudioConfig(CarPlayAppFrameworkRef inRef, const CarPlayAppFrameworkAudioConfig *inConfig)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus err = kUnknownErr;

	app_ulog(kLogLevelNotice, "%s\n", __FUNCTION__);

	require(handle != NULL, exit);
	require(inConfig != NULL, exit);

	if (handle->audioConfig == NULL) {
		handle->audioConfig = (CarPlayAppFrameworkAudioConfig *)calloc(1, sizeof(CarPlayAppFrameworkAudioConfig));
		require(handle->audioConfig != NULL, exit);
	}

	memcpy (handle->audioConfig, inConfig, sizeof(CarPlayAppFrameworkAudioConfig));

	handle->isAudioConfigured 	= true;

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkSetAudioMICMute
//===========================================================================================================================

int32_t CarPlayAppFrameworkSetAudioMICMute(CarPlayAppFrameworkRef inRef, const int32_t inMICMute)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus err = kUnknownErr;

	require(handle != NULL, exit);

	handle->isAudioMICMuted = (Boolean)inMICMute;

	err = kNoErr;

exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkSetHIDList
//===========================================================================================================================

int32_t CarPlayAppFrameworkSetHIDInfoList(CarPlayAppFrameworkRef inRef, const CarPlayAppFrameworkHIDInfoList *inList)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus err = kUnknownErr;

	app_ulog(kLogLevelNotice, "%s\n", __FUNCTION__);

	require(handle != NULL, exit);
	require(inList != NULL, exit);

	handle->hidList = (CarPlayAppFrameworkHIDInfoList *)inList;

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkSetDelegate
//===========================================================================================================================

int32_t	CarPlayAppFrameworkSetDelegate(CarPlayAppFrameworkRef inRef, const CarPlayAppFrameworkDelegate *inDelegate)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus err = kUnknownErr;

	app_ulog(kLogLevelNotice, "%s\n", __FUNCTION__);

	require(handle != NULL, exit);
	require(inDelegate != NULL, exit);

	handle->delegate = *inDelegate;

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkGetClientInfo
//===========================================================================================================================

int32_t	CarPlayAppFrameworkGetClientInfo(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkClientInfo *outClientInfo)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus err = kUnknownErr;

	require(handle != NULL,					exit);
	require(handle->airPlaySession != NULL, exit);

	outClientInfo->name				= handle->clientName;
	outClientInfo->model 			= handle->clientModel;
	outClientInfo->osBuildVersion 	= handle->clientOSBuildVersion;

	app_ulog(kLogLevelNotice, "%s >>> Client Name : %s, Model : %s, OS Build Version : %s",
							__FUNCTION__,
							handle->clientName,
							handle->clientModel,
							handle->clientOSBuildVersion);
	err = kNoErr;

exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkTerminate
//===========================================================================================================================

int32_t	CarPlayAppFrameworkTerminate(CarPlayAppFrameworkRef inRef)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;

	app_ulog(kLogLevelNotice, "%s\n", __FUNCTION__);

	if (handle->audioConfig != NULL) {
		free(handle->audioConfig);
	}

	if (inRef != NULL) {
		free(inRef);
	}

#if defined( USE_EXTERNEL_LOG_SYSTEM )
	ExternelLogWriteStop();
#endif

	return CONVERT_ERROR(kNoErr);
}

//===========================================================================================================================
//	CarPlayAppFrameworkStart
//===========================================================================================================================

OSStatus _CarPlayAppFrameworkRegisterDevice(
		HIDDeviceRef *inDevice,
		HIDCreateDescriptor_f inCreateDescriptorFunc,
		CFStringRef inUUID,
		CFStringRef inDeviceName,
		CarPlayAppFrameworkHIDInfo *hidInfo)
{
	OSStatus		err = kUnknownErr;
	uint8_t *		descPtr;
	size_t			descLen;
	CFDataRef		descData;
	HIDDeviceRef	device = NULL;
	CFNumberRef		countryCode = NULL;
	CFNumberRef		hidVendorID = NULL;
	CFNumberRef		hidProductID = NULL;

	app_ulog(kLogLevelNotice, "%s\n", __FUNCTION__);

	require(inCreateDescriptorFunc != NULL, exit);

	err = HIDDeviceCreateVirtual(&device, NULL);
	require_noerr(err, exit);

	HIDDeviceSetProperty(device, kHIDDeviceProperty_DisplayUUID, NULL, inUUID);
	HIDDeviceSetProperty(device, kHIDDeviceProperty_Name, NULL, inDeviceName);

	countryCode = CFNumberCreateInt64(hidInfo->hidCountryCode);
	hidVendorID = CFNumberCreateInt64(hidInfo->hidVendorID);
	hidProductID = CFNumberCreateInt64(hidInfo->hidProductID);

	HIDDeviceSetProperty(device, kHIDDeviceProperty_CountryCode, NULL, countryCode);
	HIDDeviceSetProperty(device, kHIDDeviceProperty_VendorID, NULL, hidVendorID);
	HIDDeviceSetProperty(device, kHIDDeviceProperty_ProductID, NULL, hidProductID);

	err = inCreateDescriptorFunc(&descPtr, &descLen);
	require_noerr(err, exit);
	descData = CFDataCreate(NULL, descPtr, (CFIndex)descLen);
	free(descPtr);
	require_action(descData, exit, err = kNoMemoryErr);
	HIDDeviceSetProperty(device, kHIDDeviceProperty_ReportDescriptor, NULL, descData);
	CFRelease(descData);

	err = HIDRegisterDevice(device);
	require_noerr(err, exit);

	*inDevice = device;

	err = kNoErr;
exit:
	CFRelease(countryCode);
	CFRelease(hidVendorID);
	CFRelease(hidProductID);
	CFRelease(device);

	return (err);
}

int32_t CarPlayAppFrameworkStart(CarPlayAppFrameworkRef inRef)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;

	OSStatus	err = kUnknownErr;
	int32_t 	u32 = 0;
	int32_t 	threadErr = 0;
    uint8_t     uuid[ 16 ];
    char        cstr[ 128 ];
    CFStringRef displayUUID = NULL;
	CarPlayAppFrameworkHIDInfo hidDummy = {0, 0, 0};
	CarPlayAppFrameworkHIDInfo *hidInfo = NULL;

	app_ulog(kLogLevelNotice, "%s\n", __FUNCTION__);

	require(handle != NULL, exit);
	require(handle->config != NULL, exit);
	require(handle->isAudioConfigured, exit);

	handle->initialModes = (CFDictionaryRef)AirPlayCreateModesDictionary(&handle->initialModesRaw, NULL, &err);
	require_noerr(err, exit);

	// Create Screen
	err = ScreenCreate(&handle->mainScreen, NULL);
	require_noerr(err, exit);

	u32 = 0;
	if (handle->config->hasKnob)
		u32 |= kScreenFeature_Knobs;
	if (handle->config->hasHiFiTouch)
		u32 |= kScreenFeature_HighFidelityTouch;
	if (handle->config->hasLoFiTouch)
		u32 |= kScreenFeature_LowFidelityTouch;


	// Set display's properties
	if (handle->config->displayEDID) // This is not work, Do not set EDID.
		ScreenSetProperty(handle->mainScreen, kScreenProperty_EDID, NULL, handle->config->displayEDID);
	if (handle->config->displayUUID) {
        displayUUID = CFStringCreateWithCString( NULL, handle->config->displayUUID, kCFStringEncodingUTF8 );
		ScreenSetProperty(handle->mainScreen, kScreenProperty_UUID, NULL, displayUUID);
    } else {
        UUIDGet( uuid );
        UUIDtoCString( uuid, false, cstr );
        app_ulog(kLogLevelNotice, "%s: Screen UUID(%s)Created.\n", __FUNCTION__, cstr);
        displayUUID = CFStringCreateWithCString( NULL, cstr, kCFStringEncodingUTF8 );
        ScreenSetProperty(handle->mainScreen, kScreenProperty_UUID, NULL, displayUUID);
    }
	if (u32)
		ScreenSetPropertyInt64(handle->mainScreen, kScreenProperty_Features, NULL, u32);
	if (handle->config->widthPixels)
		ScreenSetPropertyInt64(handle->mainScreen, kScreenProperty_WidthPixels, NULL, handle->config->widthPixels);
	if (handle->config->heightPixels)
		ScreenSetPropertyInt64(handle->mainScreen, kScreenProperty_HeightPixels, NULL, handle->config->heightPixels);
	if (handle->config->widthPhysical)
		ScreenSetPropertyInt64(handle->mainScreen, kScreenProperty_WidthPhysical, NULL, handle->config->widthPhysical);
	if (handle->config->heightPhysical)
		ScreenSetPropertyInt64(handle->mainScreen, kScreenProperty_HeightPhysical, NULL, handle->config->heightPhysical);
	if (handle->config->maxFPS)
		ScreenSetPropertyInt64(handle->mainScreen, kScreenProperty_MaxFPS, NULL, handle->config->maxFPS);
	if (handle->config->renderLayer)
		ScreenSetPropertyInt64(handle->mainScreen, kScreenProperty_PlatformLayer, NULL, handle->config->renderLayer);
	if (handle->config->primaryInputDevice)
		ScreenSetPropertyInt64(handle->mainScreen, kScreenProperty_PrimaryInputDevice, NULL, handle->config->primaryInputDevice);

	err = ScreenRegister(handle->mainScreen);
	require_noerr(err, exit);

	// Create HIDs
	if (handle->config->hasLoFiTouch || handle->config->hasHiFiTouch) {
		if (handle->hidList != NULL) {
			hidInfo = &handle->hidList->hidTouch;
		} else {
			hidInfo = &hidDummy;
			app_ulog(kLogLevelError, "%s: Lack of Touch HID Information\n", __FUNCTION__, err);
		}

		// Single touch is enabled default.
		err = _CarPlayAppFrameworkRegisterDevice(&handle->touchHID,
				HIDCustomPointingCreateDescriptor,
				displayUUID,
				CFSTR("Touch Screen"),
				hidInfo);
		require_noerr(err, exit);

		// Multi touch is possible to use or not.
		if(handle->config->hasMultiTouch) {
			err = _CarPlayAppFrameworkRegisterDevice(&handle->multiTouchHID,
					HIDMultiPointingCreateDescriptor,
					displayUUID,
					CFSTR("Multi-Touch Screen"),
					hidInfo);
			require_noerr(err, exit);
		}
	}

	if (handle->config->hasDPad) {
		if (handle->hidList != NULL) {
			hidInfo = &handle->hidList->hidDPad;
		} else {
			hidInfo = &hidDummy;
			app_ulog(kLogLevelError, "%s: Lack of D-PAD HID Information\n", __FUNCTION__, err);
		}

		err = _CarPlayAppFrameworkRegisterDevice(&handle->dPadHID,
				HIDDPadCreateDescriptor,
				displayUUID,
				CFSTR("D-Pad Device"),
				hidInfo);
		require_noerr(err, exit);
	}

	if (handle->config->hasKnob) {
		if (handle->hidList != NULL) {
			hidInfo = &handle->hidList->hidKnob;
		} else {
			hidInfo = &hidDummy;
			app_ulog(kLogLevelError, "%s: Lack of Knob HID Information\n", __FUNCTION__, err);
		}

		err = _CarPlayAppFrameworkRegisterDevice(&handle->knobHID,
				HIDCustomKnobCreateDescriptor,
				displayUUID,
				CFSTR("Knob Device"),
				hidInfo);
		require_noerr(err, exit);
	}

	if (handle->config->hasButton) {
		if (handle->hidList != NULL) {
			hidInfo = &handle->hidList->hidButton;
		} else {
			hidInfo = &hidDummy;
			app_ulog(kLogLevelError, "%s: Lack of Button HID Information\n", __FUNCTION__, err);
		}

		err = _CarPlayAppFrameworkRegisterDevice(&handle->buttonHID,
				HIDCustomButtonsCreateDescriptor,
				displayUUID,
				CFSTR("Button Device"),
				hidInfo);
		require_noerr(err, exit);
	}

	if (handle->config->hasTelephony) {
		if (handle->hidList != NULL) {
			hidInfo = &handle->hidList->hidTelephony;
		} else {
			hidInfo = &hidDummy;
			app_ulog(kLogLevelError, "%s: Lack of telephony HID Information\n", __FUNCTION__, err);
		}

		err = _CarPlayAppFrameworkRegisterDevice(&handle->telephonyHID,
				HIDCustomTelephonyCreateDescriptor,
				displayUUID,
				CFSTR("Telephony Device"),
				hidInfo);
		require_noerr(err, exit);
	}

	if (handle->config->hasTrackpad) {
		if (handle->hidList != NULL) {
			hidInfo = &handle->hidList->hidTrackpad;
		} else {
			hidInfo = &hidDummy;
			app_ulog(kLogLevelError, "%s: Lack of trackpad HID Information\n", __FUNCTION__, err);
		}

		err = _CarPlayAppFrameworkRegisterDevice(&handle->trackpadHID,
				HIDTrackpadCreateDescriptor,
				displayUUID,
				CFSTR("Trackpad Device"),
				hidInfo);
		require_noerr(err, exit);
	}

	if (handle->config->bluetoothIDs != NULL) {
		size_t totalLen = strlen(handle->config->bluetoothIDs);
		char *marker = NULL;
		char *offset = NULL;
		char *token = NULL;

		if (totalLen >= BLUETOOTH_ID_LENGTH) {
			marker = strdup(handle->config->bluetoothIDs);
			require(marker != NULL, exit);

			offset = marker;
			while ((token = strsep(&offset, ";")) != NULL) {
				size_t lenToken = strlen(token);
				if (lenToken == BLUETOOTH_ID_LENGTH) {
					err = CFArrayEnsureCreatedAndAppendCString(&handle->bluetoothIDs, token, lenToken);
					check_noerr(err);
					// Get device ID from BT Address
					if (handle->deviceID == NULL)
						handle->deviceID = CFStringCreateWithCString(NULL, token, kCFStringEncodingUTF8);
				} else {
					app_ulog(kLogLevelError, "%s : Bluetooth ID's length isn't matched(%s - length = %d)\n", __FUNCTION__, token, lenToken);
				}
			}

			free(marker);
		} else {
			app_ulog(kLogLevelError, "%s : Bluetooth ID is too short(\"%s\" - length = %d)\n", __FUNCTION__, handle->config->bluetoothIDs, totalLen);
		}
	}
	threadErr = pthread_create(&handle->carPlayThread, NULL, _CarPlayAppFrameworkThread, (void*)inRef);
	require_action(threadErr == 0, exit, err = kNoMemoryErr);

	err = kNoErr;
exit:

    CFRelease(displayUUID);

	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkPostPointing
//===========================================================================================================================

int32_t CarPlayAppFrameworkPostPointing(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDPointing inPointing)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;

	OSStatus		err = kUnknownErr;
	uint8_t			report[5] = {0, 0, 0, 0, 0};
	const uint8_t	buttons = (inPointing.touch ? 1 : 0);
	double			scaledX = 0.0;
	double			scaledY = 0.0;

	require(handle != NULL, exit);
	require(handle->config != NULL, exit);
	require(handle->touchHID != NULL, exit);
	require(handle->airPlaySession != NULL, exit);

	require_action_quiet((inPointing.x >= 0) && (inPointing.x <= handle->config->actualWidthPixels), exit, err = kParamErr);
	require_action_quiet((inPointing.y >= 0) && (inPointing.y <= handle->config->actualHeightPixels), exit, err = kParamErr);

	scaledX = ((double)inPointing.x) / handle->config->actualWidthPixels;
	scaledY = ((double)inPointing.y) / handle->config->actualHeightPixels;

	HIDPointingFillReport(report, buttons, scaledX, scaledY);

	err = HIDDevicePostReport(handle->touchHID, report, sizeof(report));
	require_noerr(err, exit);

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkPostMultiPointing
//===========================================================================================================================

int32_t CarPlayAppFrameworkPostMultiPointing(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDPointing inPointing[2])
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;

	OSStatus	err = kUnknownErr;
	uint8_t		report[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t		ids[2] = { 0, 0 };
	uint8_t		buttons[2] = { 0, 0 };
    double		scaledX[2] = { 0.0, 0.0 };
    double		scaledY[2] = { 0.0, 0.0 };
	uint32_t	i = 0;

	require(handle != NULL, exit);
	require(handle->config != NULL, exit);
	require(handle->multiTouchHID != NULL, exit);
	require(handle->airPlaySession != NULL, exit);

	for (i = 0; i < 2; i++) {
		require_action_quiet((inPointing[i].x >= 0) && (inPointing[i].x <= handle->config->actualWidthPixels), exit, err = kParamErr);
		require_action_quiet((inPointing[i].y >= 0) && (inPointing[i].y <= handle->config->actualHeightPixels), exit, err = kParamErr);

		ids[i] = inPointing[i].id;
		buttons[i] = (inPointing[i].touch ? 1 : 0);
		scaledX[i] = ((double)inPointing[i].x) / handle->config->actualWidthPixels;
		scaledY[i] = ((double)inPointing[i].y) / handle->config->actualHeightPixels;
	}

	HIDMultiPointingFillReport(report, ids, buttons, scaledX, scaledY);

	err = HIDDevicePostReport(handle->multiTouchHID, report, sizeof(report));

	require_noerr(err, exit);

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}



//===========================================================================================================================
//	CarPlayAppFrameworkPostDPad
//===========================================================================================================================

int32_t CarPlayAppFrameworkPostDPad(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDDPad inDPad)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;

	OSStatus	err = kUnknownErr;
	uint8_t		report[2] = {0, 0};

	require(handle != NULL, exit);
	require(handle->config != NULL, exit);
	require(handle->dPadHID != NULL, exit);
	require(handle->airPlaySession != NULL, exit);

	HIDDPadFillReport(report, 	inDPad.up, 	  inDPad.down, inDPad.left, inDPad.right,
								inDPad.selec, inDPad.home, inDPad.menu, inDPad.back);

	err = HIDDevicePostReport(handle->dPadHID, report, sizeof(report));
	require_noerr(err, exit);

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}


//===========================================================================================================================
//	CarPlayAppFrameworkPostKnob
//===========================================================================================================================

int32_t CarPlayAppFrameworkPostKnob(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDKnob inKnob)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;

	OSStatus	err = kUnknownErr;
	uint8_t		report[4] = {0, 0, 0, 0};

	require(handle != NULL, exit);
	require(handle->config != NULL, exit);
	require(handle->knobHID != NULL, exit);
	require(handle->airPlaySession != NULL, exit);

    HIDCustomKnobFillReport(report, inKnob.button, inKnob.home, inKnob.back, inKnob.x, inKnob.y, inKnob.wheel);

	err = HIDDevicePostReport(handle->knobHID, report, sizeof(report));
	require_noerr(err, exit);

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkPostButton
//===========================================================================================================================

int32_t CarPlayAppFrameworkPostButton(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDButton inButton)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;

	OSStatus	err = kUnknownErr;
	uint8_t		report = {0};

	require(handle != NULL, exit);
	require(handle->config != NULL, exit);
	require(handle->buttonHID != NULL, exit);
	require(handle->airPlaySession != NULL, exit);

#if defined(GEELY)
    if (inButton.pressed) {
        if (inButton.scanNextTrack == 1) {
            report = (1<<0);
        } else if (inButton.scanPreviousTrack == 1) {
            report = (1<<1);
        } else if (inButton.playOrPause == 1) {
            report = (1<<2);
        } else {
            report = 0;
        }
    } else {
        report = 0;
    }
#elif defined(GEELY_KC1B)
    if (inButton.pressed) {
        if (inButton.scanNextTrack == 1) {
            report = (1<<0);
        } else if (inButton.scanPreviousTrack == 1) {
            report = (1<<1);
        } else if (inButton.playOrPause == 1) {
            report = (1<<2);
        } else if (inButton.back == 1) {
            report = (1<<3);
        } else {
            report = 0;
        }
    } else {
        report = 0;
    }
#elif defined(MM2014)
    if (inButton.pressed) {
        if (inButton.play == 1) {
            report = (1<<0);
        } else if (inButton.pause == 1) {
            report = (1<<1);
        } else if (inButton.scanNextTrack == 1) {
            report = (1<<2);
        } else if (inButton.scanPreviousTrack == 1) {
            report = (1<<3);
        } else if (inButton.playOrPause == 1) {
            report = (1<<4);
        } else {
            report = 0;
        }
    } else {
        report = 0;
    }
#elif defined(MEEGO)
    if (inButton.pressed) {
        if (inButton.play == 1) {
            report = (1<<0);
        } else if (inButton.scanNextTrack == 1) {
            report = (1<<1);
        } else if (inButton.scanPreviousTrack == 1) {
            report = (1<<2);
        } else if (inButton.playOrPause == 1) {
            report = (1<<3);
        } else {
            report = 0;
        }
    } else {
        report = 0;
    }
#else
    if (inButton.pressed) {
        if (inButton.play == 1) {
            report = (1<<0);
        } else if (inButton.pause == 1) {
            report = (1<<1);
        } else if (inButton.scanNextTrack == 1) {
            report = (1<<2);
        } else if (inButton.scanPreviousTrack == 1) {
            report = (1<<3);
        } else {
            report = 0;
        }
    } else {
        report = 0;
    }
#endif

	err = HIDDevicePostReport(handle->buttonHID, &report, sizeof(report));
	require_noerr(err, exit);

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkPostTelephony
//===========================================================================================================================

int32_t CarPlayAppFrameworkPostTelephony(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDTelephony inTelephony)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;

	OSStatus	err = kUnknownErr;
	uint8_t		report = {0};

	require(handle != NULL, exit);
	require(handle->config != NULL, exit);
	require(handle->telephonyHID != NULL, exit);
	require(handle->airPlaySession != NULL, exit);

#if defined(MM2014)
    if (inTelephony.pressed) {
        if (inTelephony.flash == 1) {
         report = (1<<0);
        } else {
            report = 0;
        }
    } else {
        report = 0;
    }
#else
    if (inTelephony.pressed) {
		if (inTelephony.hookSwitch == 1) {
			report = (1<<0);
		// } else if(inTelephony.flash == 1) {
		// 	report = 2;
		} else if (inTelephony.drop == 1) {
			report = (1<<1);
		// } else if(inTelephony.phoneKey0 == 1) {
		// 	report = 4;
		// } else if(inTelephony.phoneKey1 == 1) {
		// 	report = 5;
		// } else if(inTelephony.phoneKey2 == 1) {
		// 	report = 6;
		// } else if(inTelephony.phoneKey3 == 1) {
		// 	report = 7;
		// } else if(inTelephony.phoneKey4 == 1) {
		// 	report = 8;
		// } else if(inTelephony.phoneKey5 == 1) {
		// 	report = 9;
		// } else if(inTelephony.phoneKey6 == 1) {
		// 	report = 10;
		// } else if(inTelephony.phoneKey7 == 1) {
		// 	report = 11;
		// } else if(inTelephony.phoneKey8 == 1) {
		// 	report = 12;
		// } else if(inTelephony.phoneKey9 == 1) {
		// 	report = 13;
		// } else if(inTelephony.phoneKeyStar == 1) {
		// 	report = 14;
		// } else if(inTelephony.phoneKeyPound == 1) {
		// 	report = 15;
		} else if(inTelephony.phoneMute == 1) {
			report = (1<<2);
		} else {
			report = 0;
		}
    } else {
        report = 0;
    }
#endif


	err = HIDDevicePostReport(handle->telephonyHID, &report, sizeof(report));
	require_noerr(err, exit);

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkPostTrackpad
//===========================================================================================================================

int32_t CarPlayAppFrameworkPostTrackpad(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDTrackpad inTrackpad)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;

	OSStatus	err = kUnknownErr;
	uint8_t		report[19] = {0,};

	require(handle != NULL, exit);
	require(handle->config != NULL, exit);
	require(handle->trackpadHID != NULL, exit);
	require(handle->airPlaySession != NULL, exit);

	HIDTrackpadFillReport(report, 	inTrackpad.character1Length, 	inTrackpad.character1Data,
									inTrackpad.character1Quality, 	inTrackpad.character2Length,
									inTrackpad.character2Data, 		inTrackpad.character2Quality,
									inTrackpad.transducerState, 	inTrackpad.transducerX, inTrackpad.transducerY);

	err = HIDDevicePostReport(handle->trackpadHID, report, sizeof(report));
	require_noerr(err, exit);

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkPostStreamDeviceReady
//===========================================================================================================================

int32_t CarPlayAppFrameworkPostStreamDeviceReady(CarPlayAppFrameworkRef inRef, CarPlayStreamType inStreamType)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus	err = kUnknownErr;

	require(handle != NULL, exit);

	app_ulog(kLogLevelNotice, "%s called(%s stream)\n", __FUNCTION__,
	        AirPlayStreamTypeToString(CarPlayStreamTypeToAirPlayStreamType(inStreamType)));

	// Signal to wait setup response

    pthread_mutex_lock(&handle->pendingRespLock);

    if (handle->pendingRespStreamType == inStreamType) {
        pthread_cond_signal(&handle->pendingRespCond);
        err = kNoErr;
    } else if (CarPlayStreamTypeToAirPlayStreamType(inStreamType) == kAirPlayStreamType_Invalid) {
        err = kUnsupportedErr;
    }

    pthread_mutex_unlock(&handle->pendingRespLock);

exit:

	if (err == kUnknownErr) {
		app_ulog(kLogLevelWarning, "%s failed(%d: No pending SETUP response)\n", __FUNCTION__, err);
	} else if (err == kUnsupportedErr) {
	    app_ulog(kLogLevelError, "%s failed(%d: Unsupported stream type)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkStop
//===========================================================================================================================

int32_t CarPlayAppFrameworkStop(CarPlayAppFrameworkRef inRef)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus	err = kUnknownErr;
	int32_t 	threadErr = 0;

	app_ulog(kLogLevelNotice, "%s\n", __FUNCTION__);

	require(handle != NULL, exit);
	require(handle->carPlayThread != 0, exit);

	CFRunLoopStop(CFRunLoopGetMain());

	threadErr = pthread_join(handle->carPlayThread, NULL);
	require(threadErr == 0, exit);

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	// Release resources
	if (handle != NULL) {
		if (handle->touchHID) {
			HIDDeviceStop(handle->touchHID);
			HIDDeregisterDevice(handle->touchHID);
			CFReleaseNullSafe(handle->touchHID);
		}

		if (handle->multiTouchHID) {
			HIDDeviceStop(handle->multiTouchHID);
			HIDDeregisterDevice(handle->multiTouchHID);
			CFReleaseNullSafe(handle->multiTouchHID);
		}

		if (handle->dPadHID) {
			HIDDeviceStop(handle->dPadHID);
			HIDDeregisterDevice(handle->dPadHID);
			CFReleaseNullSafe(handle->dPadHID);
		}

		if (handle->knobHID) {
			HIDDeviceStop(handle->knobHID);
			HIDDeregisterDevice(handle->knobHID);
			CFReleaseNullSafe(handle->knobHID);
		}

		if (handle->buttonHID) {
			HIDDeviceStop(handle->buttonHID);
			HIDDeregisterDevice(handle->buttonHID);
			CFReleaseNullSafe(handle->buttonHID);
		}

		if (handle->telephonyHID) {
			HIDDeviceStop(handle->telephonyHID);
			HIDDeregisterDevice(handle->telephonyHID);
			CFReleaseNullSafe(handle->telephonyHID);
		}

		if (handle->trackpadHID) {
			HIDDeviceStop(handle->trackpadHID);
			HIDDeregisterDevice(handle->trackpadHID);
			CFReleaseNullSafe(handle->trackpadHID);
		}

		if (handle->mainScreen) {
			ScreenDeregister(handle->mainScreen);
			CFReleaseNullSafe(handle->mainScreen);
		}

		if (handle->pendingTaskLockPtr) {
			pthread_mutex_destroy(handle->pendingTaskLockPtr);
			handle->pendingTaskLockPtr = NULL;
		}

		pthread_mutex_destroy(&handle->pendingRespLock);
		pthread_cond_destroy(&handle->pendingRespCond);

        CFReleaseNullSafe(handle->deviceID);
		CFReleaseNullSafe(handle->initialModes);
		CFReleaseNullSafe(handle->bluetoothIDs);
        CFReleaseNullSafe(handle->limitedUIElements);
		CFReleaseNullSafe(handle->pendingTaskMap);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkSetNightMode
//===========================================================================================================================

int32_t CarPlayAppFrameworkSetNightMode(CarPlayAppFrameworkRef inRef, const int32_t inNightMode)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus err = kUnknownErr;

	app_ulog(kLogLevelNotice, "%s called(inNightMode = %d)\n", __FUNCTION__, inNightMode);

	require(handle != NULL, exit);
    require_quiet(inNightMode >= 0, exit);
    require_action_quiet( handle->config->nightMode >=0 , exit,
        app_ulog(kLogLevelWarning, "%s : Night Mode is not supported. If you want use Night mode, do not set negative value in config.\n", __FUNCTION__);
        );

	handle->config->nightMode = inNightMode; 	// Save night mode even session disconnected.

	if (handle->airPlaySession == NULL) {
		app_ulog(kLogLevelWarning, "%s : no session is established(inNightMode = %d). Keep nightMode\n", __FUNCTION__, inNightMode);
	} else {
		err = AirPlayReceiverSessionSetNightMode(handle->airPlaySession, inNightMode, NULL, NULL);
		require_noerr(err, exit);
	}

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkSetLimitedUI
//===========================================================================================================================

int32_t CarPlayAppFrameworkSetLimitedUI(CarPlayAppFrameworkRef inRef, const int32_t inLimitedUI)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus err = kUnknownErr;

	app_ulog(kLogLevelNotice, "%s called(inLimitedUI = %d)\n", __FUNCTION__, inLimitedUI);

	require(handle != NULL, exit);
    require_quiet(inLimitedUI >= 0, exit);
    require_action_quiet( handle->config->limitedUI >=0 , exit,
        app_ulog(kLogLevelWarning, "%s : LimitedUI is not supported. If you want use LimitedUI, do not set negative value in config.\n", __FUNCTION__);
        );

	handle->config->limitedUI = inLimitedUI;	// Save Limited UI even session disconnected.

	if (handle->airPlaySession == NULL) {
		app_ulog(kLogLevelWarning, "%s : no session is established(inLimitedUI = %d). Keep limitedUI\n", __FUNCTION__, inLimitedUI);
	} else {
		AirPlayReceiverSessionSetLimitedUI(handle->airPlaySession, inLimitedUI, NULL, NULL);
	}

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkRequestUI
//===========================================================================================================================

int32_t CarPlayAppFrameworkRequestUI(CarPlayAppFrameworkRef inRef, const char* inURL)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus 	err = kUnknownErr;
	CFStringRef	url = NULL;

	require(handle != NULL, exit);
	require(handle->airPlaySession != NULL, exit);

	if (inURL != NULL) {
		url = CFStringCreateWithCString(NULL, inURL, kCFStringEncodingUTF8);
	}

	app_ulog(kLogLevelNotice, "%s called(inURL = %s)\n", __FUNCTION__, inURL);

	AirPlayReceiverSessionRequestUI(handle->airPlaySession, url, NULL, NULL);

    CFReleaseNullSafe(url);

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkSetOwnership
//===========================================================================================================================

int32_t CarPlayAppFrameworkSetOwnership(CarPlayAppFrameworkRef inRef, CarPlayTransferType inTransferType, CarPlayOwnershipResource inResource)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus			err = kUnknownErr;
	AirPlayModeChanges	changes;
	AirPlayModeChanges*	modes = NULL;
	AirPlayReceiverSessionCommandCompletionFunc complete = NULL;

	bool isMainAudioResource = false;
	bool isScreenResource = false;

    require(handle != NULL, exit);

	isMainAudioResource = (inResource & kCarPlayOwnershipResource_MainAudio) == kCarPlayOwnershipResource_MainAudio ? true : false;
	isScreenResource = (inResource & kCarPlayOwnershipResource_Screen) == kCarPlayOwnershipResource_Screen ? true : false;
	require(isMainAudioResource || isScreenResource, exit);

	if (isScreenResource || isMainAudioResource) {
		AirPlayModeChangesInit(&changes);

		modes = handle->airPlaySession ? &changes : &handle->initialModesRaw;

		if (inTransferType == kCarPlayTransferType_Take) {
			// Screen
			if (isScreenResource) {
				modes->screen.type							= kAirPlayTransferType_Take;
				modes->screen.priority						= kAirPlayTransferPriority_UserInitiated;
				modes->screen.takeConstraint				= kAirPlayConstraint_UserInitiated;
				modes->screen.borrowOrUnborrowConstraint	= kAirPlayConstraint_Anytime;
			}
			// Main Audio
			if (isMainAudioResource) {
				modes->mainAudio.type						= kAirPlayTransferType_Take;
				modes->mainAudio.priority					= kAirPlayTransferPriority_UserInitiated;
				modes->mainAudio.takeConstraint				= kAirPlayConstraint_UserInitiated;
				modes->mainAudio.borrowOrUnborrowConstraint	= kAirPlayConstraint_Anytime;
			}

			app_ulog(kLogLevelNotice, "Take screen(%d) and main audio(%d)\n", isScreenResource, isMainAudioResource);
		} else if (inTransferType == kCarPlayTransferType_Untake) {
			// Screen
			if (isScreenResource) {
				modes->screen.type							= kAirPlayTransferType_Untake;
				modes->screen.priority						= kAirPlayTransferPriority_NotApplicable;
				modes->screen.takeConstraint				= kAirPlayConstraint_NotApplicable;
				modes->screen.borrowOrUnborrowConstraint	= kAirPlayConstraint_NotApplicable;
			}

			// Main Audio
			if (isMainAudioResource) {
				modes->mainAudio.type						= kAirPlayTransferType_Untake;
				modes->mainAudio.priority					= kAirPlayTransferPriority_NotApplicable;
				modes->mainAudio.takeConstraint				= kAirPlayConstraint_NotApplicable;
				modes->mainAudio.borrowOrUnborrowConstraint	= kAirPlayConstraint_NotApplicable;
			}

			app_ulog(kLogLevelNotice, "Untake screen(%d) and main audio(%d)\n", isScreenResource, isMainAudioResource);
		} else if (inTransferType == kCarPlayTransferType_Borrow) {
			// Screen
			if (isScreenResource) {
				modes->screen.type							= kAirPlayTransferType_Borrow;
				modes->screen.priority						= kAirPlayTransferPriority_UserInitiated;
				modes->screen.takeConstraint				= kAirPlayConstraint_NotApplicable;
				modes->screen.borrowOrUnborrowConstraint	= kAirPlayConstraint_Never;
			}

			// Main Audio
			if (isMainAudioResource) {
				modes->mainAudio.type						= kAirPlayTransferType_Borrow;
				modes->mainAudio.priority					= kAirPlayTransferPriority_UserInitiated;
				modes->mainAudio.takeConstraint				= kAirPlayConstraint_NotApplicable;
				modes->mainAudio.borrowOrUnborrowConstraint	= kAirPlayConstraint_Never;
			}

			app_ulog(kLogLevelNotice, "Borrow screen(%d) and main audio(%d)\n", isScreenResource, isMainAudioResource);
		} else if (inTransferType == kCarPlayTransferType_Unborrow) {
			// Screen
			if (isScreenResource) {
				modes->screen.type							= kAirPlayTransferType_Unborrow;
				modes->screen.priority						= kAirPlayTransferPriority_NotApplicable;
				modes->screen.takeConstraint				= kAirPlayConstraint_NotApplicable;
				modes->screen.borrowOrUnborrowConstraint	= kAirPlayConstraint_NotApplicable;
			}

			// Main Audio
			if (isMainAudioResource) {
				modes->mainAudio.type						= kAirPlayTransferType_Unborrow;
				modes->mainAudio.priority					= kAirPlayTransferPriority_NotApplicable;
				modes->mainAudio.takeConstraint			    = kAirPlayConstraint_NotApplicable;
				modes->mainAudio.borrowOrUnborrowConstraint	= kAirPlayConstraint_NotApplicable;
			}

			app_ulog(kLogLevelNotice, "Unborrow screen(%d) and main audio(%d)\n", isScreenResource, isMainAudioResource);
		}

	    if (handle->airPlaySession != NULL) {
	        if(handle->delegate.changeModeComplete_f != NULL) {
	            complete = _CarPlayHandleChangeModesComplete;
	        }

	        err = AirPlayReceiverSessionChangeModes(handle->airPlaySession, &changes, NULL, complete, inRef);
	        require_noerr(err, exit);
	    } else {
            app_ulog(kLogLevelNotice, "%s : no session is established. Keep Resource Mode\n", __FUNCTION__);
	    }

	}

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkForceKeyFrame
//===========================================================================================================================

int32_t CarPlayAppFrameworkForceKeyFrame(CarPlayAppFrameworkRef inRef)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus			err = kUnknownErr;

	require(handle != NULL, exit);
	require(handle->airPlaySession != NULL, exit);

	err = AirPlayReceiverSessionForceKeyFrame(handle->airPlaySession, NULL, NULL);
	require_noerr(err, exit);

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkRequestSiri
//===========================================================================================================================

int32_t CarPlayAppFrameworkRequestSiri(CarPlayAppFrameworkRef inRef, CarPlaySiriAction inAction)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus			err = kUnknownErr;
	AirPlaySiriAction	action = kAirPlaySiriAction_NotApplicable;

	require(handle != NULL, exit);
	require(handle->airPlaySession != NULL, exit);

	switch (inAction) {
	case kCarPlaySiriAction_Prewarm:
		action = kAirPlaySiriAction_Prewarm;
		break;
	case kCarPlaySiriAction_ButtonPressed:
		action = kAirPlaySiriAction_ButtonDown;
		break;
	case kCarPlaySiriAction_ButtonReleased:
		action = kAirPlaySiriAction_ButtonUp;
		break;
	}
	err = AirPlayReceiverSessionRequestSiriAction(handle->airPlaySession, action, NULL, NULL);
	require_noerr(err, exit);

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkUpdateVolInfo
//===========================================================================================================================

int32_t CarPlayAppFrameworkUpdateVolInfo(CarPlayAppFrameworkRef inRef, uint32_t volInfo)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus			err = kUnknownErr;

	require(handle != NULL, exit);

     handle->volInfo = volInfo;
     app_ulog(kLogLevelInfo, "%s: &(handle->volInfo) : %p, handle->volInfo = %d\n", __FUNCTION__, &handle->volInfo, handle->volInfo);	

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	CarPlayAppFrameworkSetAltAudioMute
//===========================================================================================================================

int32_t CarPlayAppFrameworkSetAltAudioMute(CarPlayAppFrameworkRef inRef, const int32_t outAltAudioMute)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus err = kUnknownErr;

	require(handle != NULL, exit);

	handle->isAltAudioMuted = (Boolean)outAltAudioMute;
     app_ulog(kLogLevelInfo, "%s: CarPlayAppFrameworkSetAltAudioMute(%s)\n", __FUNCTION__, handle->isAltAudioMuted ? "true" : "false");	

	err = kNoErr;

exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

#if defined(MEEGO)
//===========================================================================================================================
//	CarPlayAppFrameworkStopAudioStreamForce
//===========================================================================================================================

int32_t CarPlayAppFrameworkStopAudioStreamForce(CarPlayAppFrameworkRef inRef, void* inStream) {
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus err = kUnknownErr;

	require(handle != NULL, exit);

	app_ulog(kLogLevelNotice, "%s: inStream : %p\n", __FUNCTION__, inStream);
	AudioStreamStop( inStream, false );

	err = kNoErr;

exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}
#endif

//===========================================================================================================================
//	CarPlayAppFrameworkStopPendingTasks
//===========================================================================================================================

int32_t CarPlayAppFrameworkStopPendingTasks(CarPlayAppFrameworkRef inRef)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus			err = kUnknownErr;
	CFLIndex			count = 0;
	int 				i = 0;
	Boolean				isLocked = false;
	CFArrayRef 			keys = NULL;

	require(handle != NULL, exit);
	require(handle->airPlaySession != NULL, exit);
	require(handle->pendingTaskMap != NULL, exit);
	require(handle->pendingTaskLockPtr != NULL, exit);

	pthread_mutex_lock(handle->pendingTaskLockPtr);
	isLocked = true;

	// Check whether registered pending task exists or not
	// Pending task have to register into pending task map
	count = CFDictionaryGetCount(handle->pendingTaskMap);
	if (count > 0) {
		// Get all keys from pending task map
		keys = CFDictionaryCopyKeys(handle->pendingTaskMap, &err);
		require_noerr(err, exit);

		// Make true all records in pending task map
		for (i = 0; i < count; i++) {
			CFDictionarySetValue(handle->pendingTaskMap, CFArrayGetValueAtIndex(keys, (CFIndex)i), kCFBooleanTrue);
		}
	}

	err = kNoErr;
exit:
	if (isLocked) {
		pthread_mutex_unlock(handle->pendingTaskLockPtr);
	}

	CFReleaseNullSafe(keys);

	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}
//===========================================================================================================================
//	CarPlayAppFrameworkChangeAppState
//===========================================================================================================================

int32_t CarPlayAppFrameworkChangeAppState(CarPlayAppFrameworkRef inRef, CarPlayAppState inState, int32_t inEnable)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus			err = kUnknownErr;
	AirPlayReceiverSessionCommandCompletionFunc complete = NULL;
    require(handle != NULL, exit);

	if(handle->delegate.changeModeComplete_f != NULL) {
		complete = _CarPlayHandleChangeModesComplete;
	}

	if (inState == kCarPlayAppState_PhoneCall) {
		if (handle->airPlaySession != NULL) {
			err = AirPlayReceiverSessionChangePhoneCall(handle->airPlaySession, inEnable, NULL, complete, handle);
			require_noerr(err, exit);
		} else {
			handle->initialModesRaw.phoneCall = inEnable ? kAirPlayTriState_True : kAirPlayTriState_False;
		}
	} else if (inState == kCarPlayAppState_SpeechMode) {
		AirPlaySpeechMode mode = inEnable ? kAirPlaySpeechMode_Speaking : kAirPlaySpeechMode_None;
		if (handle->airPlaySession != NULL) {
			err = AirPlayReceiverSessionChangeSpeechMode(handle->airPlaySession, mode, NULL, complete, handle);
			require_noerr(err, exit);
		} else {
			handle->initialModesRaw.speech = mode;
		}
	} else if (inState == kCarPlayAppState_TurnByTurn) {
		if (handle->airPlaySession != NULL) {
			err = AirPlayReceiverSessionChangeTurnByTurn(handle->airPlaySession, inEnable, NULL, complete, handle);
			require_noerr(err, exit);
		} else {
			handle->initialModesRaw.turnByTurn = inEnable ? kAirPlayTriState_True : kAirPlayTriState_False;
		}
	} else if (inState == kCarPlayAppState_ResetState) {
		(void) inEnable;

		err = AirPlayReceiverSessionChangeAppState(handle->airPlaySession,
				kAirPlayTriState_True,
				kAirPlayTriState_True,
				kAirPlayTriState_NotApplicable,
				NULL, complete, NULL);
		require_noerr(err, exit);

		usleep(50 * 1000);

		err = AirPlayReceiverSessionChangeAppState(handle->airPlaySession,
				kAirPlayTriState_NotApplicable,
				kAirPlayTriState_NotApplicable,
				kAirPlayTriState_NotApplicable,
				NULL, complete, NULL);
		require_noerr(err, exit);
	}

	if (handle->airPlaySession == NULL) {
        app_ulog(kLogLevelNotice, "%s : no session is established. Keep AppState\n", __FUNCTION__);
	}

	err = kNoErr;
exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}
//===========================================================================================================================
//	CarPlayAppFrameworkChangeModes
//===========================================================================================================================

int32_t CarPlayAppFrameworkChangeModes(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkModeChanges inModes)
{
	struct CarPlayAppFrameworkPrivate *handle = inRef;
	OSStatus				err = kUnknownErr;
	AirPlayModeChanges		changes;
	AirPlayModeChanges	*	modes = NULL;
	AirPlayReceiverSessionCommandCompletionFunc complete = NULL;

	require(handle != NULL, exit);

	AirPlayModeChangesInit(&changes);

	modes = handle->airPlaySession ? &changes : &handle->initialModesRaw;

    switch (inModes.screen.type) {
        case kCarPlayTransferType_Take:
            modes->screen.type                       = inModes.screen.type;
            modes->screen.priority                   = inModes.screen.priority;
            modes->screen.takeConstraint             = inModes.screen.takeConstraint;
            modes->screen.borrowOrUnborrowConstraint = inModes.screen.borrowOrUnborrowConstraint;
            break;
        case kCarPlayTransferType_Borrow:
            modes->screen.type                       = inModes.screen.type;
            modes->screen.priority                   = inModes.screen.priority;
            modes->screen.takeConstraint             = kAirPlayConstraint_NotApplicable;
            modes->screen.borrowOrUnborrowConstraint = inModes.screen.borrowOrUnborrowConstraint;
            break;
        case kCarPlayTransferType_Untake:
            modes->screen.type                       = inModes.screen.type;
            modes->screen.priority                   = kAirPlayTransferPriority_NotApplicable;
            modes->screen.takeConstraint             = kAirPlayConstraint_NotApplicable;
            modes->screen.borrowOrUnborrowConstraint = kAirPlayConstraint_NotApplicable;
            break;
        case kCarPlayTransferType_Unborrow:
            modes->screen.type                       = inModes.screen.type;
            modes->screen.priority                   = kAirPlayTransferPriority_NotApplicable;
            modes->screen.takeConstraint             = kAirPlayConstraint_NotApplicable;
            modes->screen.borrowOrUnborrowConstraint = kAirPlayConstraint_NotApplicable;
            break;
        default:
            // Do nothing to update!
            break;
    }

    switch (inModes.mainAudio.type) {
        case kCarPlayTransferType_Take:
            modes->mainAudio.type                       = inModes.mainAudio.type;
            modes->mainAudio.priority                   = inModes.mainAudio.priority;
            modes->mainAudio.takeConstraint             = inModes.mainAudio.takeConstraint;
            modes->mainAudio.borrowOrUnborrowConstraint = inModes.mainAudio.borrowOrUnborrowConstraint;
            break;
        case kCarPlayTransferType_Borrow:
            modes->mainAudio.type                       = inModes.mainAudio.type;
            modes->mainAudio.priority                   = inModes.mainAudio.priority;
            modes->mainAudio.takeConstraint             = kAirPlayConstraint_NotApplicable;
            modes->mainAudio.borrowOrUnborrowConstraint = inModes.mainAudio.borrowOrUnborrowConstraint;
            break;
        case kCarPlayTransferType_Untake:
            modes->mainAudio.type                       = inModes.mainAudio.type;
            modes->mainAudio.priority                   = kAirPlayTransferPriority_NotApplicable;
            modes->mainAudio.takeConstraint             = kAirPlayConstraint_NotApplicable;
            modes->mainAudio.borrowOrUnborrowConstraint = kAirPlayConstraint_NotApplicable;
            break;
        case kCarPlayTransferType_Unborrow:
            modes->mainAudio.type                       = inModes.mainAudio.type;
            modes->mainAudio.priority                   = kAirPlayTransferPriority_NotApplicable;
            modes->mainAudio.takeConstraint             = kAirPlayConstraint_NotApplicable;
            modes->mainAudio.borrowOrUnborrowConstraint = kAirPlayConstraint_NotApplicable;
            break;
        default:
            // Do nothing to update!
            break;
    }

	if( inModes.speech     != kCarPlaySpeechMode_NotApplicable )   modes->speech       = inModes.speech;
	if( inModes.phoneCall  != kCarPlayTriState_NotApplicable )     modes->phoneCall    = inModes.phoneCall;
	if( inModes.turnByTurn != kCarPlayTriState_NotApplicable )     modes->turnByTurn   = inModes.turnByTurn;

	if (handle->airPlaySession != NULL) {
		if(handle->delegate.changeModeComplete_f != NULL) {
			complete = _CarPlayHandleChangeModesComplete;
		}

		err = AirPlayReceiverSessionChangeModes(handle->airPlaySession, &changes, NULL, complete, inRef);
		require_noerr(err, exit);
	} else {
        app_ulog(kLogLevelNotice, "%s : no session is established. Keep Resource Mode and App State\n", __FUNCTION__);
    }

	err = kNoErr;

exit:
	if (err) {
		app_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return CONVERT_ERROR(err);
}

//===========================================================================================================================
//	_CarPlayHandleChangeModesComplete
//===========================================================================================================================
// #define _CARPLAYHANDLECHANGEMODESCOMPLETE_LOG_
static void _CarPlayHandleChangeModesComplete( OSStatus inStatus, CFDictionaryRef inResponse, void *inContext )
{
	struct CarPlayAppFrameworkPrivate * handle = inContext;
	int32_t 							result = kCarPlayResult_Error;
	CFIndex								i;
	CarPlayAppFrameworkModeState		state;
	CFDictionaryRef						response;
	CFArrayRef							appStates;
	CFArrayRef							resources;
	CFDictionaryRef						tempDict	= NULL;
	int64_t 							tempValue   = 0;
	OSStatus                            err = kNoErr;

	memset(&state, 0, sizeof(CarPlayAppFrameworkModeState));

	// Check status is no err.
    require_noerr(inStatus, exit);

	// Get Keys
	response  = CFDictionaryGetCFDictionary( inResponse, CFSTR( kAirPlayKey_Params ), &err );
	require_noerr(err, exit);
	appStates = CFDictionaryGetCFArray( response, CFSTR( kAirPlayKey_AppStates ), &err );
	require_noerr(err, exit);
	resources = CFDictionaryGetCFArray( response, CFSTR( kAirPlayKey_Resources ), &err );
	require_noerr(err, exit);

	// App states
	for(i=0; i<CFArrayGetCount(appStates); i++) {
		tempDict	= CFArrayGetValueAtIndex(appStates, i);
		tempValue	= CFDictionaryGetInt64( tempDict, CFSTR( kAirPlayKey_AppStateID ), NULL);

		// App State : Phone call
		if (tempValue == kAirPlayAppStateID_PhoneCall) {
			state.phoneCall = CFDictionaryGetInt64( tempDict, CFSTR( kAirPlayKey_Entity ), NULL);
#ifdef _CARPLAYHANDLECHANGEMODESCOMPLETE_LOG_
			app_ulog(kLogLevelNotice, "kAirPlayAppStateID_PhoneCall data found~~~~!\n");
#endif
		}
		// App State : Speech
		else if (tempValue == kAirPlayAppStateID_Speech) {
			state.speech = CFDictionaryGetInt64( tempDict, CFSTR( kAirPlayKey_Entity ), NULL);
			state.speechMode = CFDictionaryGetInt64( tempDict, CFSTR( kAirPlayKey_SpeechMode), NULL);
#ifdef _CARPLAYHANDLECHANGEMODESCOMPLETE_LOG_
			app_ulog(kLogLevelNotice, "kAirPlayAppStateID_Speech data found~~~~!\n");
#endif
		}
		// App State : Turn by trun
		else if (tempValue == kAirPlayAppStateID_TurnByTurn) {
			state.turnByTurn = CFDictionaryGetInt64( tempDict, CFSTR( kAirPlayKey_Entity ), NULL);
#ifdef _CARPLAYHANDLECHANGEMODESCOMPLETE_LOG_
			app_ulog(kLogLevelNotice, "kAirPlayAppStateID_TurnByTurn data found~~~~!\n");
#endif
		}
		tempDict = NULL;
	}

	// resources
	for(i=0; i<CFArrayGetCount(resources); i++) {
		tempDict	= CFArrayGetValueAtIndex(resources, i);
		tempValue	= CFDictionaryGetInt64( tempDict, CFSTR( kAirPlayKey_ResourceID ), NULL);

		// Resource : Main Screen
		if (tempValue == kAirPlayResourceID_MainScreen) {
			state.screen = CFDictionaryGetInt64( tempDict, CFSTR( kAirPlayKey_Entity ), NULL);
#ifdef _CARPLAYHANDLECHANGEMODESCOMPLETE_LOG_
			app_ulog(kLogLevelNotice, "kAirPlayResourceID_MainScreen data found~~~~!\n");
#endif
		}
		// Resource : Main Audio
		else if (tempValue == kAirPlayResourceID_MainAudio) {
			state.mainAudio = CFDictionaryGetInt64( tempDict, CFSTR( kAirPlayKey_Entity ), NULL);
#ifdef _CARPLAYHANDLECHANGEMODESCOMPLETE_LOG_
			app_ulog(kLogLevelNotice, "kAirPlayResourceID_MainAudio data found~~~~!\n");
#endif
		}
		tempDict = NULL;
	}

    app_ulog(kLogLevelNotice, "change modes response: screen %s, mainAudio %s, speech %s (%s), phone %s, turns %s\n",
		AirPlayEntityToString(state.screen),
		AirPlayEntityToString(state.mainAudio),
		AirPlayEntityToString(state.speech),
		AirPlaySpeechModeToString(state.speechMode),
		AirPlayEntityToString(state.phoneCall),
		AirPlayEntityToString(state.turnByTurn));

    if( (handle != NULL) && (&handle->modeState != NULL) )
        memcpy(&handle->modeState, &state, sizeof(CarPlayAppFrameworkModeState));
    else
        app_ulog(kLogLevelWarning, "[Warning] Mode State is not updated. handle : %p, modeState : %p\n",
        handle, &handle->modeState);

	result = kCarPlayResult_OK;

exit:
	if (CHECK_DELEGATE(handle, changeModeComplete_f)) {
		CALL_DELEGATE(handle, changeModeComplete_f, handle->airPlaySession, result, &state, handle);
	}
}

//===========================================================================================================================
//	_CarPlayAppFrameworkThread
//===========================================================================================================================

static void * _CarPlayAppFrameworkThread(void *inArg)
{
	OSStatus							err;
	AirPlayReceiverServerDelegate		delegate;

	struct CarPlayAppFrameworkPrivate *handle = inArg;

	require(handle != NULL, exit);

	app_ulog(kLogLevelWarning, "%s started\n", __FUNCTION__);

	// Create the AirPlay server. This advertise via Bonjour and starts listening for connections.
	err = AirPlayReceiverServerCreate(&handle->airPlayServer);
	require_noerr(err, exit);

	// Register ourself as a delegate to receive server-level events, such as when a session is created.
	AirPlayReceiverServerDelegateInit(&delegate);
	delegate.context			= handle;
	delegate.control_f			= _CarPlayHandleServerControl;
	delegate.copyProperty_f		= _CarPlayHandleServerCopyProperty;
	delegate.setProperty_f		= _CarPlayHandleServerSetProperty;
	delegate.sessionCreated_f	= _CarPlayHandleSessionCreated;
	delegate.sessionFailed_f	= _CarPlayHandleSessionFailed;
	AirPlayReceiverServerSetDelegate(handle->airPlayServer, &delegate);

	// Start the server and run until the app quits.
	AirPlayReceiverServerStart(handle->airPlayServer);
	CFRunLoopRun();
	AirPlayReceiverServerStop(handle->airPlayServer);

	app_ulog(kLogLevelWarning, "%s finished\n", __FUNCTION__);
exit:
	if (handle != NULL)
		CFReleaseNullSafe(handle->airPlayServer);

	pthread_exit(NULL);

	return (NULL);
}
