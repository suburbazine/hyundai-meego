/*
	File:       CarPlayAppFrameworkServerCallbacks.c
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
#include "CarPlayAppFrameworkServerCallbacks.h"
#include "CarPlayAppFrameworkPrivate.h"
#include "CarPlayAppFrameworkSessionCallbacks.h"

//===========================================================================================================================
//	_CarPlayHandleServerCopyProperty
//===========================================================================================================================

OSStatus _CarPlayHandleServerControl(
		AirPlayReceiverServerRef	inServer,
		CFStringRef					inCommand,
		CFTypeRef					inQualifier,
		CFDictionaryRef				inParams,
		CFDictionaryRef *			outParams,
		void *						inContext )
{
	struct CarPlayAppFrameworkPrivate *handle = inContext;
	OSStatus		err = kNotHandledErr;

	(void) inQualifier;
	(void) inParams;
	(void) outParams;

	if (inCommand != NULL)
		app_ulog(kLogLevelNotice, "%s: inCommand = %s\n", __FUNCTION__, CFStringGetCStringPtr(inCommand, kCFStringEncodingUTF8));

	if( 0 ) {}

	// Start Server

	else if( CFEqual( inCommand, CFSTR( kAirPlayCommand_StartServer ) ) ) {
		if (CHECK_DELEGATE(handle, serverStarted_f)) {
			CALL_DELEGATE(handle, serverStarted_f, (void*)inServer);
		}
	} else {
		err = kNotHandledErr;
		goto exit;
	}

	err = kNoErr;

exit:
	return err;
}

//===========================================================================================================================
//	_CarPlayHandleServerCopyProperty
//===========================================================================================================================

CFTypeRef _CarPlayHandleServerCopyProperty(
		AirPlayReceiverServerRef	inServer,
		CFStringRef					inProperty,
		CFTypeRef					inQualifier,
		OSStatus *					outErr,
		void *						inContext)
{
	struct CarPlayAppFrameworkPrivate *handle = inContext;
	CFTypeRef		value = NULL;
	OSStatus		err = kNotHandledErr;

	(void) inServer;
	(void) inQualifier;

	require(handle != NULL, exit);
	require(handle->config != NULL, exit);

	if (inProperty != NULL)
		app_ulog(kLogLevelWarning, "%s: inProperty = %s\n", __FUNCTION__, CFStringGetCStringPtr(inProperty, kCFStringEncodingUTF8));

	if (0) {

	} else if (CFEqual(inProperty, CFSTR(kAirPlayKey_DeviceID))) {
		// Device ID
		if (handle->deviceID == NULL && handle->config->bluetoothIDs != NULL) {
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
						// Get device ID from BT Address
						handle->deviceID = CFStringCreateWithCString(NULL, token, kCFStringEncodingUTF8);
						break;
					} else {
						app_ulog(kLogLevelError, "%s : Bluetooth ID's length isn't matched(%s - length = %d)\n", __FUNCTION__, token, lenToken);
					}
				}
				free(marker);
			}
		}

		if (handle->deviceID) {
			value = handle->deviceID;
			require_action_quiet(value, exit, err = kNotHandledErr);
			CFRetain(value);
		} else {
			err = kNotHandledErr;
			goto exit;
		}
	} else if (CFEqual(inProperty, CFSTR(kAirPlayProperty_BluetoothIDs))) {
		// BluetoothIDs
		if (handle->bluetoothIDs == NULL && handle->config->bluetoothIDs != NULL) {
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
					} else {
						app_ulog(kLogLevelError, "%s : Bluetooth ID's length isn't matched(%s - length = %d)\n", __FUNCTION__, token, lenToken);
					}
				}
				free(marker);
			}
		}

		if (handle->bluetoothIDs) {
			value = handle->bluetoothIDs;
			require_action_quiet(value, exit, err = kNotHandledErr);
			CFRetain(value);
		} else {
			err = kNotHandledErr;
			goto exit;
		}
	} else if (CFEqual(inProperty, CFSTR(kAirPlayKey_RightHandDrive))) {
		// RightHandleDrive
		value = handle->config->rightHandDrive ? kCFBooleanTrue : kCFBooleanFalse;
        require_action_quiet(value, exit, err = kNotHandledErr);
        CFRetain(value);
	} else if (CFEqual(inProperty, CFSTR(kAirPlayKey_NightMode))) {
		// NightMode(Negative value means not support)
		if (handle->config->nightMode >= 0) {
			value = handle->config->nightMode ? kCFBooleanTrue : kCFBooleanFalse;
			require_action_quiet(value, exit, err = kNotHandledErr);
			CFRetain(value);
		} else {
			err = kNotHandledErr;
			goto exit;
		}
	} else if (CFEqual(inProperty, CFSTR(kAirPlayKey_LimitedUI))) {
		// LimitedUI(Negative value means not support)
		if (handle->config->limitedUI >= 0) {
			value = handle->config->limitedUI ? kCFBooleanTrue : kCFBooleanFalse;
			require_action_quiet(value, exit, err = kNotHandledErr);
			CFRetain(value);
		} else {
			err = kNotHandledErr;
			goto exit;
		}
	} else if (CFEqual(inProperty, CFSTR(kAirPlayKey_LimitedUIElements))) {
		// LimitedUIElements
		CFIndex		n;
		n = handle->limitedUIElements ? CFArrayGetCount( handle->limitedUIElements ) : 0;
		CFRange range = CFRangeMake(0, n);

		if (handle->config->limitedUIElements & kCarPlaylimitedUIElement_SoftKeyboard)
		{
			if ((n == 0) || !CFArrayContainsValue(handle->limitedUIElements, range, CFSTR(kAirPlayLimitedUIElement_SoftKeyboard))) {
				err = CFArrayEnsureCreatedAndAppendCString(&handle->limitedUIElements, kAirPlayLimitedUIElement_SoftKeyboard, strlen(kAirPlayLimitedUIElement_SoftKeyboard));
				check_noerr(err);
			}
		}
		if (handle->config->limitedUIElements & kCarPlaylimitedUIElement_SoftPhoneKeypad)
		{
			if ((n == 0) || !CFArrayContainsValue(handle->limitedUIElements, range, CFSTR(kAirPlayLimitedUIElement_SoftPhoneKeypad))) {
				err = CFArrayEnsureCreatedAndAppendCString(&handle->limitedUIElements, kAirPlayLimitedUIElement_SoftPhoneKeypad, strlen(kAirPlayLimitedUIElement_SoftPhoneKeypad));
				check_noerr(err);
			}
		}
		if (handle->config->limitedUIElements & kCarPlaylimitedUIElement_NonMusicLists)
		{
			if ((n == 0) || !CFArrayContainsValue(handle->limitedUIElements, range, CFSTR(kAirPlayLimitedUIElement_NonMusicLists))) {
				err = CFArrayEnsureCreatedAndAppendCString(&handle->limitedUIElements, kAirPlayLimitedUIElement_NonMusicLists, strlen(kAirPlayLimitedUIElement_NonMusicLists));
				check_noerr(err);
			}
		}
		if (handle->config->limitedUIElements & kCarPlaylimitedUIElement_MusicLists)
		{
			if ((n == 0) || !CFArrayContainsValue(handle->limitedUIElements, range, CFSTR(kAirPlayLimitedUIElement_MusicLists))) {
				err = CFArrayEnsureCreatedAndAppendCString(&handle->limitedUIElements, kAirPlayLimitedUIElement_MusicLists, strlen(kAirPlayLimitedUIElement_MusicLists));
				check_noerr(err);
			}
		}
		if (handle->config->limitedUIElements & kCarPlaylimitedUIElement_JapanMaps)
		{
			if ((n == 0) || !CFArrayContainsValue(handle->limitedUIElements, range, CFSTR(kAirPlayLimitedUIElement_JapanMaps))) {
				err = CFArrayEnsureCreatedAndAppendCString(&handle->limitedUIElements, kAirPlayLimitedUIElement_JapanMaps, strlen(kAirPlayLimitedUIElement_JapanMaps));
				check_noerr(err);
			}
		}
		value = handle->limitedUIElements;
		require_action_quiet(value, exit, err = kNotHandledErr);
		CFRetain(value);
	} else if (CFEqual(inProperty, CFSTR(kAirPlayKey_Manufacturer))) {
		// Manufacturer
		require_action_quiet(handle->config->manufacturer, exit, err = kNotHandledErr);
		value = CFStringCreateWithCString(NULL, (handle->config->manufacturer) ? (handle->config->manufacturer) : "LG Electronics Inc.", kCFStringEncodingUTF8);
		require_action_quiet(value, exit, err = kNotHandledErr);
		CFRetain(value);
	} else if (CFEqual(inProperty, CFSTR(kAirPlayKey_Model))) {
		// Model
		require_action_quiet(handle->config->modelName, exit, err = kNotHandledErr);
		value = CFStringCreateWithCString(NULL, (handle->config->modelName) ? handle->config->modelName : "LGE_CarPlay1,0", kCFStringEncodingUTF8);
		require_action_quiet(value, exit, err = kNotHandledErr);
		CFRetain(value);
	} else if (CFEqual(inProperty, CFSTR(kAirPlayProperty_OEMIconVisible))) {
		// OEM Icon Visible
		value = handle->config->oemIconVisible ? kCFBooleanTrue : kCFBooleanFalse;
		require_action_quiet(value, exit, err = kNotHandledErr);
		CFRetain(value);
	} else if (CFEqual(inProperty, CFSTR(kAirPlayProperty_OEMIconLabel))) {
		// OEMIconLabel
		if (handle->config->oemIconVisible) {
			require_action_quiet(handle->config->oemIconLabel, exit, err = kNotHandledErr);
			value = CFStringCreateWithCString(NULL, handle->config->oemIconLabel, kCFStringEncodingUTF8);
			require_action_quiet(value, exit, err = kNotHandledErr);
			CFRetain(value);
		} else {
			err = kNotHandledErr;
			goto exit;
		}
	} else if( CFEqual( inProperty, CFSTR( kAirPlayProperty_OEMIcons ) ) ) {
		// OEMIcons
		int i;
		CFTypeRef obj;
		CFMutableDictionaryRef iconDict;
		CFMutableArrayRef iconsArray;

		if (handle->config->oemIconVisible) {
			iconsArray = CFArrayCreateMutable( NULL, 0, &kCFTypeArrayCallBacks );
			require_action_quiet( iconsArray, exit, err = kNotHandledErr );

			for (i=0; i<3; i++) {
				// Add icons for each required size
				obj = CFDataCreateWithFilePath( handle->config->oemIcons[i].filePath, NULL );
				if( obj ) {
					iconDict = CFDictionaryCreateMutable( NULL, 0,  &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks );
					if( iconDict ) {
						CFDictionarySetInt64( iconDict, CFSTR( kAirPlayOEMIconKey_WidthPixels ), handle->config->oemIcons[i].widthPixel );
						CFDictionarySetInt64( iconDict, CFSTR( kAirPlayOEMIconKey_HeightPixels ), handle->config->oemIcons[i].heightPixel );
						CFDictionarySetBoolean( iconDict, CFSTR( kAirPlayOEMIconKey_Prerendered ), handle->config->oemIcons[i].prerendered ? true : false);
						CFDictionarySetValue( iconDict, CFSTR( kAirPlayOEMIconKey_ImageData ), obj );
						CFArrayAppendValue( iconsArray, iconDict );
						CFRelease( iconDict );
						iconDict = NULL;
					}
					CFRelease( obj );
				}
			}

			if( CFArrayGetCount( iconsArray ) > 0) {
				value = iconsArray;
			} else {
				CFRelease( iconsArray );
				err = kNotHandledErr;
				goto exit;
			}
		} else {
			err = kNotHandledErr;
			goto exit;
		}
	} else if (CFEqual(inProperty, CFSTR(kAirPlayProperty_OEMIcon))) {
		// OEMIcon
		if (handle->config->oemIconVisible) {
			require_action_quiet(handle->config->oemIconFilePath, exit, err = kNotHandledErr);
			value = CFDataCreateWithFilePath(handle->config->oemIconFilePath, &err);
			require_action_quiet(value, exit, err = kNotHandledErr);
			CFRetain(value);
		} else {
			err = kNotHandledErr;
			goto exit;
		}
	} else if( CFEqual( inProperty, CFSTR( kAirPlayProperty_ExtendedFeatures ) ) ) {
		// EnhancedRequestCarUI
		CFMutableArrayRef extendedFeatures;

		extendedFeatures = CFArrayCreateMutable( NULL, 0, &kCFTypeArrayCallBacks );
		require_action_quiet( extendedFeatures, exit, err = kNotHandledErr );

		CFArrayAppendValue( extendedFeatures, CFSTR( kAirPlayExtendedFeature_VocoderInfo ) );

		// TODO : Get use or not EnhancedRequestCarUI
		if( 0 ) {
			CFArrayAppendValue( extendedFeatures, CFSTR( kAirPlayExtendedFeature_EnhancedRequestCarUI ) );
		}
		if( CFArrayGetCount( extendedFeatures ) == 0 ) {
			ForgetCF( &extendedFeatures );
		} else {
			value = extendedFeatures;
		}
	} else {
		err = kNotHandledErr;
		goto exit;
	}
	err = kNoErr;

exit:
	if (outErr) *outErr = err;
	return value;
}

//===========================================================================================================================
//	_CarPlayHandleServerSetProperty
//===========================================================================================================================

OSStatus _CarPlayHandleServerSetProperty(
		AirPlayReceiverServerRef	inServer,
		CFStringRef					inProperty,
		CFTypeRef					inQualifier,
		CFTypeRef					inValue,
		void *						inContext)
{
	struct CarPlayAppFrameworkPrivate *handle = inContext;
	OSStatus		err = kNotHandledErr;

	(void) inServer;
	(void) inQualifier;

	if (inProperty != NULL)
		app_ulog(kLogLevelNotice, "%s: inProperty = %s\n", __FUNCTION__, CFStringGetCStringPtr(inProperty, kCFStringEncodingUTF8));

	if (0) {

	} else if (CFEqual(inProperty, CFSTR(kAirPlayKey_Name))) {
		// Name
		strcpy(handle->clientName, CFStringGetCStringPtr((CFStringRef)inValue, kCFStringEncodingUTF8));
	} else if (CFEqual(inProperty, CFSTR(kAirPlayKey_Model))) {
		// Model
		strcpy(handle->clientModel, CFStringGetCStringPtr((CFStringRef)inValue, kCFStringEncodingUTF8));
	} else if (CFEqual(inProperty, CFSTR(kAirPlayKey_OSBuildVersion))) {
		// OS Build Version
		strcpy(handle->clientOSBuildVersion, CFStringGetCStringPtr((CFStringRef)inValue, kCFStringEncodingUTF8));
	} else if (CFEqual(inProperty, CFSTR(kAirPlayKey_ClientDeviceID))) {
		// client Device ID
		handle->clientDeviceID = CFGetInt64(inValue, NULL);
	} else {
		err = kNotHandledErr;
		goto exit;
	}

	err = kNoErr;

exit:

	return err;
}


//===========================================================================================================================
//	_CarPlayHandleSessionCreated
//===========================================================================================================================

void _CarPlayHandleSessionCreated(
		AirPlayReceiverServerRef	inServer,
		AirPlayReceiverSessionRef	inSession,
		void *						inContext)
{
	struct CarPlayAppFrameworkPrivate *handle = inContext;
	AirPlayReceiverSessionDelegate	delegate;

	(void) inServer;

	app_ulog(kLogLevelWarning, "AirPlay session created\n");
	handle->airPlaySession = inSession;

	// Register ourself as a delegate to receive session-level events, such as modes changes.
	AirPlayReceiverSessionDelegateInit(&delegate);
	delegate.context		= inContext;
	delegate.finalize_f		= _CarPlayHandleSessionFinalized;
	delegate.copyProperty_f	= _CarPlayHandleSessionCopyProperty;
	delegate.modesChanged_f	= _CarPlayHandleModesChanged;
	delegate.requestUI_f	= _CarPlayHandleRequestUI;
	delegate.control_f		= _CarPlayHandleSessionControl;
	delegate.duckAudio_f	= _CarPlayHandleDuckAudio;
	delegate.unduckAudio_f	= _CarPlayHandleUnduckAudio;
	delegate.setupRequest_f	= _CarPlayHandleSetupRequest;
	delegate.disableBluetooth_f = _CarPlayHandleDisableBluetooth;
	AirPlayReceiverSessionSetDelegate(inSession, &delegate);

	if (CHECK_DELEGATE(handle, sessionCreated_f)) {
		CALL_DELEGATE(handle, sessionCreated_f, (void*)inSession);
	}
}

//===========================================================================================================================
//	_CarPlayHandleSessionFailed
//===========================================================================================================================

void _CarPlayHandleSessionFailed(
		AirPlayReceiverServerRef	inServer,
		OSStatus					inReason,
		void *						inContext)
{
	(void) inServer;
	(void) inContext;

	app_ulog(kLogLevelError, "%s called(inReason = %d)\n", __FUNCTION__, inReason);

	return;
}
