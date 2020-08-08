/*
	File:       CarPlayAudioALSA.cpp
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

#include "AudioUtils.h"

#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <alsa/asoundlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include "CarPlayAppFrameworkPrivate.h"

#include "AirPlayCommon.h"
#include "SettingUtils.h"
#include "APSCommonServices.h"
#include "APSDebugServices.h"
#include "AtomicUtils.h"
#include "TickUtils.h"
#include "ThreadUtils.h"

#if defined(MIBGP)
#include "DeviceMIBSoundManager.h"
#else
#include "DeviceALSA.h"
#endif

#if defined(ECNR_FUNCTION)
#include "AudioECNR.h"
#include "TseECNR.h"
#endif

#include CF_HEADER
#include LIBDISPATCH_HEADER

#define USEC_TO_FRAMES(rate, usec)      (rate * usec / UINT64_C_safe(kMicrosecondsPerSecond))
#define FRAMES_TO_USEC(rate, frame)     (frame * UINT64_C_safe(kMicrosecondsPerSecond) / rate)

// ===========================================================================================================================
//	AudioStream
//===========================================================================================================================

typedef struct AudioStreamImp *			AudioStreamImpRef;
struct AudioStreamImp {
	Boolean								prepared;				// True if AudioStreamPrepare has been called (and stop hasn't yet).

	AudioStreamInputCallback_f			inputCallbackPtr;		// Function to call to write input audio.
	void *								inputCallbackCtx;		// Context to pass to audio input callback function.
	AudioStreamOutputCallback_f			outputCallbackPtr;		// Function to call to read audio to output.
	void *								outputCallbackCtx;		// Context to pass to audio output callback function.
	Boolean								input;					// Enable audio input.
	AudioStreamBasicDescription			format;					// Format of the audio data.
	uint32_t							preferredLatencyMics;	// Max latency the app can tolerate.
	uint32_t							streamType;				// AirPlay Stream type (e.g. main, alt).

	CFStringRef							audioType;
	pthread_t 							inputThread;
	pthread_t							outputThread;

	AudioALSA *							inputAlsa;
	AudioALSA *							outputAlsa;

	Boolean								isInputDeviceAvalable;
	Boolean								isOutputDeviceAvalable;

	struct CarPlayAppFrameworkPrivate *	frameworkHandle;
	char *								threadName;         // Name to use when creating thread.
	int									threadPriority;     // Priority to run thread.

#if defined(ECNR_FUNCTION)
	AudioECNR *							audioEcnr;
#endif
#if defined(MEEGO)
	pthread_mutex_t 					mutex;
	pthread_mutex_t * 					mutexPtr;
#endif
};

#define _AudioStreamGetImp(STREAM)	((AudioStreamImpRef)AudioStreamGetContext((STREAM)))

#define LOG_INTERVAL	    0	                // print interval of host time and sample time
#define DUMP_INPUT_STREAM   0                   // Dump input PCM steam : request by CARPLAY-61
#define DUMP_OUTPUT_STREAM  0                   // Dump output PCM steam
#define FILE_PATH           DUMP_FILE_PATH      // path with write permission. This path define in Makefiles

//===========================================================================================================================
//	Logging
//===========================================================================================================================

ulog_define(CarPlayAudioALSA, kLogLevelTrace, kLogFlags_Default, "CarPlayAudioALSA", NULL);
#define as_dlog(LEVEL, ...)		dlogc(&log_category_from_name(CarPlayAudioALSA), (LEVEL), __VA_ARGS__)
#define as_ulog(LEVEL, ...)		ulog(&log_category_from_name(CarPlayAudioALSA), (LEVEL), __VA_ARGS__)

//===========================================================================================================================
//	Declarations
//===========================================================================================================================

OSStatus AudioStreamInitialize(AudioStreamRef inStream, CFDictionaryRef inOptions);
void AudioStreamFinalize(AudioStreamRef inStream);

static void * _AudioOutputThread(void *inParam);
static void * _AudioInputThread(void *inParam);
//===========================================================================================================================
//	Constants
//===========================================================================================================================
#define kDefaultOutputTimeInterval  (10000ull)     // Audio write time interval
#define kDefaultOutputBufferTime    (80000ull)     // buffer time in us
#define kDefaultOutputPeriodTime    (10000ull)     // period time in us

#define kDefaultInputTimeInterval   (8000ull)      // Audio read time interval
#define kDefaultInputBufferTime     (80000ull)     // buffer time in us
#define kDefaultInputPeriodTime     (8000ull)      // period time in us

#define kDefaultMaxRetry			(50)		// max number of retrying to open device
#define kDefaultRetryInterval		(100000)	// retrying interval in microseconds

#define kECNRSampleSize_8K			(128)	    // telephony period size in frame
#define kECNRSampleSize_16K_24K		(256)	    // Speech recognition period size in frame

#define kECNRSendLatency_8K_16K     (64000)     // Telephony mode send latency
#define kECNRRecvLatency_8K_16K     (16000)     // Telephony mode receive latency
#
#define kECNRSendLatency_24K        (42667)     // Siri(speech recognition) mode send latency
#define kECNRRecvLatency_24K        (10667)     // Siri(speech recognition) mode receive latency

//===========================================================================================================================
//	Declarations
//===========================================================================================================================

static OSStatus _CheckAudioDeviceAvailable(AudioStreamRef inStream, Boolean * inDeviceAvailable);

//===========================================================================================================================
//	AudioStreamInitialize
//===========================================================================================================================

OSStatus AudioStreamInitialize(AudioStreamRef inStream, CFDictionaryRef inOptions)
{
	OSStatus				err;
	AudioStreamImpRef		me;
	int64_t					value = 0;
	as_ulog(kLogLevelWarning, "%s: inStream = %p\n", __FUNCTION__, inStream);

	(void) inOptions;

	require_action(AudioStreamGetContext(inStream) == NULL, exit, err = kAlreadyInitializedErr);

	me = (AudioStreamImpRef)calloc(1, sizeof(*me));
	require_action(me, exit, err = kNoMemoryErr);

	// $$$ TODO: Other initialization goes here.
	// This function is called (instead of AudioStreamCreate()) when AudioUtils is built as a standalone shared object
	// that is loaded dynamically by AirPlay at runtime, so the initialization code should look very similar
	// to that in AudioStreamCreate().

	AudioStreamSetContext(inStream, me);

#if defined(MEEGO)
	err = pthread_mutex_init(&me->mutex, NULL);
	require_noerr(err, exit);
	me->mutexPtr = &me->mutex;
#endif

	// Set framework handle

	value = CarPlaySettings_GetInt64(kFrameworkHandle, &err);
	require_noerr(err, exit);

	me->frameworkHandle = (struct CarPlayAppFrameworkPrivate *)((uintptr_t)value);
	require(me->frameworkHandle != NULL, exit);

#if defined(ECNR_FUNCTION)
	me->audioEcnr = new TseECNR();
	require_action(me->audioEcnr, exit, err = kNoMemoryErr);

	((TseECNR*)(me->audioEcnr))->setVolInfoPtr(&(me->frameworkHandle->volInfo));
#endif

	err = kNoErr;
exit:
	return (err);
}

//===========================================================================================================================
//	AudioStreamFinalize
//===========================================================================================================================

void AudioStreamFinalize(AudioStreamRef inStream)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);

	as_ulog(kLogLevelWarning, "%s: inStream = %p\n", __FUNCTION__, inStream);

	if (!me) return;

	// $$$ TODO: Last chance to free any resources allocated by this object.
	// This function is called (instead of _AudioStreamFinalize()) when AudioUtils is built as a standalone shared object
	// that is loaded dynamically by AirPlay at runtime, so the finalization code should look very similar to that in
	// _AudioStreamFinalize().
	// It is automatically invoked, when the retain count of an AudioStream object goes to zero.

	if (me->outputAlsa != NULL) {
		delete me->outputAlsa;
		me->outputAlsa = NULL;
	}

	if (me->inputAlsa != NULL) {
		delete me->inputAlsa;
		me->inputAlsa = NULL;
	}

#if defined(ECNR_FUNCTION)
	if (me->audioEcnr != NULL) {
		delete me->audioEcnr;
		me->audioEcnr = NULL;
	}
#endif

	if (me->threadName != NULL) {
		free(me->threadName);
		me->threadName = NULL;
	}

#if defined(MEEGO)
	if (me->mutexPtr != NULL) {
		pthread_mutex_destroy(me->mutexPtr);
		me->mutexPtr = NULL;
	}
#endif

	if (me->audioType) CFRelease(me->audioType);
	free(me);
	AudioStreamSetContext(inStream, NULL);
}

//===========================================================================================================================
//	AudioStreamSetInputCallback
//===========================================================================================================================

void AudioStreamSetInputCallback(AudioStreamRef inStream, AudioStreamInputCallback_f inFunc, void *inContext)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);

	me->inputCallbackPtr = inFunc;
	me->inputCallbackCtx = inContext;

	as_ulog(kLogLevelNotice, "%s: func = %p, ctx = %p\n", __FUNCTION__, inFunc, inContext);
}

//===========================================================================================================================
//	AudioStreamSetOutputCallback
//===========================================================================================================================

void AudioStreamSetOutputCallback(AudioStreamRef inStream, AudioStreamOutputCallback_f inFunc, void *inContext)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);

	me->outputCallbackPtr = inFunc;
	me->outputCallbackCtx = inContext;

	as_ulog(kLogLevelNotice, "%s: func = %p, ctx = %p\n", __FUNCTION__, inFunc, inContext);
}

//===========================================================================================================================
//	_AudioStreamCopyProperty
//===========================================================================================================================

CFTypeRef _AudioStreamCopyProperty(
		CFTypeRef		inObject,
		CFObjectFlags	inFlags,
		CFStringRef		inProperty,
		CFTypeRef		inQualifier,
		OSStatus *		outErr)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp((AudioStreamRef)inObject);
	OSStatus					err;
	CFTypeRef					value = NULL;

	as_ulog(kLogLevelNotice, "%s: inStream = %p\n", __FUNCTION__, inObject);

	(void) inFlags;
	(void) inQualifier;

	// AudioType
	if (CFEqual(inProperty, kAudioStreamProperty_AudioType)) {
		// $$$ TODO: Return the current audio type.
	}
	// Format
	else if (CFEqual(inProperty, kAudioStreamProperty_Format)) {
		value = CFDataCreate(NULL, (const uint8_t *)&me->format, sizeof(me->format));
		require_action(value, exit, err = kNoMemoryErr);
	}
	// Input
	else if (CFEqual(inProperty, kAudioStreamProperty_Input)) {
		value = me->input ? kCFBooleanTrue : kCFBooleanFalse;
		CFRetain(value);
	}
	// PreferredLatency
	else if (CFEqual(inProperty, kAudioStreamProperty_PreferredLatency)) {
		value = CFNumberCreateInt64(me->preferredLatencyMics);
		require_action(value, exit, err = kNoMemoryErr);
	}
	// StreamType
	else if (CFEqual(inProperty, kAudioStreamProperty_StreamType)) {
		value = CFNumberCreateInt64(me->streamType);
		require_action(value, exit, err = kNoMemoryErr);
	}
	// ThreadName
	else if (CFEqual(inProperty, kAudioStreamProperty_ThreadName)) {
		// $$$ TODO: If your implementation uses a helper thread, return its name here.
		if (me->threadName) {
			value = CFStringCreateWithCString( NULL, (const char *) me->threadName, kCFStringEncodingUTF8 );
			require_action( value, exit, err = kNoMemoryErr );
		}
	}
	// ThreadPriority
	else if (CFEqual(inProperty, kAudioStreamProperty_ThreadPriority)) {
		// $$$ TODO: If your implementation uses a helper thread, return its priority here.
		value = CFNumberCreateInt64( me->threadPriority );
		require_action( value, exit, err = kNoMemoryErr );
	}
	// Other
	else {
		err = kNotHandledErr;
		goto exit;
	}
	err = kNoErr;

exit:
	if (outErr) *outErr = err;
	return(value);
}

//===========================================================================================================================
//	_AudioStreamSetProperty
//===========================================================================================================================

OSStatus _AudioStreamSetProperty(
		CFTypeRef		inObject,
		CFObjectFlags	inFlags,
		CFStringRef		inProperty,
		CFTypeRef		inQualifier,
		CFTypeRef		inValue)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp((AudioStreamRef)inObject);
	OSStatus					err;

	char 						strbuf[256] = {0, };
	as_ulog(kLogLevelNotice, "%s: inStream = %p, %s\n", __FUNCTION__, inObject, CFGetCString(inProperty, strbuf, 256));

	(void) inFlags;
	(void) inQualifier;

	// Properties may only be set before AudioStreamPrepare is called.
	require_action(!me->prepared, exit, err = kStateErr);

	// AudioType
	if (CFEqual(inProperty, kAudioStreamProperty_AudioType)) {
		// $$$ TODO: Use the audio type to enable certain types of audio processing.
		// For example, if the audio type is "telephony", echo cancellation should be enabled;
		// if the audio type is "speech recognition", non-linear processing algorithms should be disabled.
		char buffer[256] = {0, };
		size_t maxLen = sizeof(buffer);
		char * strAudioType = NULL;

		strAudioType = CFGetCString(inValue, buffer, maxLen);
		require(strAudioType != NULL, exit);

		me->audioType = CFStringCreateWithCString(NULL, strAudioType, kCFStringEncodingUTF8);
		require(me->audioType != NULL, exit);

		as_ulog(kLogLevelWarning, "%s: audioType = %s\n", __FUNCTION__, strAudioType);
	}
	// Format
	else if (CFEqual(inProperty, kAudioStreamProperty_Format)) {
		CFGetData(inValue, &me->format, sizeof(me->format), NULL, &err);
		require_noerr(err, exit);
	}
	// Input
	else if (CFEqual(inProperty, kAudioStreamProperty_Input)) {
		me->input = CFGetBoolean(inValue, NULL);
	}
	// PreferredLatency
	else if (CFEqual(inProperty, kAudioStreamProperty_PreferredLatency)) {
		me->preferredLatencyMics = (uint32_t) CFGetInt64(inValue, &err);
		require_noerr(err, exit);
	}
	// StreamType
	else if (CFEqual(inProperty, kAudioStreamProperty_StreamType)) {
		me->streamType = (uint32_t) CFGetInt64(inValue, &err);
		require_noerr(err, exit);

		as_ulog(kLogLevelWarning, "%s: streamType = %s\n", __FUNCTION__, AirPlayStreamTypeToString(me->streamType));
	}
	// ThreadName
	else if (CFEqual(inProperty, kAudioStreamProperty_ThreadName)) {
		me->threadName = CFCopyCString(inValue, &err);
		require_noerr(err, exit);
	}
	// ThreadPriority
	else if (CFEqual(inProperty, kAudioStreamProperty_ThreadPriority)) {
		me->threadPriority = (uint32_t) CFGetInt64(inValue, &err);
		require_noerr(err, exit);
	}
	// Other
	else {
		err = kNotHandledErr;
		goto exit;
	}
	err = kNoErr;

exit:
	return (err);
}

//===========================================================================================================================
//	AudioStreamRampVolume
//===========================================================================================================================

OSStatus AudioStreamRampVolume(
		AudioStreamRef		inStream,
		double				inFinalVolume,
		double				inDurationSecs,
		dispatch_queue_t	inQueue)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);
	OSStatus					err;

	// $$$ TODO: The volume of the audio should be ramped to inFinalVolume over inDurationSecs.
	// To be consistent when the rest of the accessory's user experience, inFinalVolume may be replaced with a more
	// canonical value where appropriate (i.e., when this routine is called to perform the audio duck).
	(void) inFinalVolume;
	(void) inDurationSecs;
	(void) inQueue;
	(void) me;

	as_ulog(kLogLevelNotice, "%s: inStream = %p, inFinalVolume = %5.3f, inDurationSecs = %5.3f\n",
			__FUNCTION__, inStream, inFinalVolume, inDurationSecs);

	err = kNoErr;

	return (err);
}

#if 0
#pragma mark -
#endif

//===========================================================================================================================
//	_GetCurrentAudioOutputDevice
//===========================================================================================================================

static OSStatus _GetCurrentAudioOutputDevice(AudioStreamRef inStream, char * outDevice)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);

	char		buffer[256] = {0, };
	size_t		maxLen = sizeof(buffer);
	char*		strAudioType = NULL;
	char*		strAudioDevice = NULL;
	OSStatus	err = kUnknownErr;

	if (me->streamType == kAirPlayStreamType_MainAudio) {
		strAudioType = CFGetCString(me->audioType, buffer, maxLen);
		as_ulog(kLogLevelNotice, "%s: AudioType = %s\n", __FUNCTION__, strAudioType);
		if (strAudioType != NULL) {
			if (strcmp(strAudioType, kAirPlayAudioType_Default) == 0) {
				strAudioDevice = me->frameworkHandle->audioConfig->deflt.outputDevice;
			} else if (strcmp(strAudioType, kAirPlayAudioType_Media) == 0) {
				strAudioDevice = me->frameworkHandle->audioConfig->media.outputDevice;
			} else if (strcmp(strAudioType, kAirPlayAudioType_Telephony) == 0) {
				strAudioDevice = me->frameworkHandle->audioConfig->telephony.outputDevice;
			} else if (strcmp(strAudioType, kAirPlayAudioType_SpeechRecognition) == 0) {
				strAudioDevice = me->frameworkHandle->audioConfig->speechRecognition.outputDevice;
			} else if (strcmp(strAudioType, kAirPlayAudioType_Alert) == 0) {
				strAudioDevice = me->frameworkHandle->audioConfig->alert.outputDevice;
			} else {
				as_ulog(kLogLevelError, "%s: unknown audio type(%s)\n", __FUNCTION__, strAudioType);
			}
		}
	} else {
		strAudioDevice = me->frameworkHandle->audioConfig->alt.outputDevice;
	}

	if (strAudioDevice != NULL) {
		strcpy(outDevice, strAudioDevice);
		err = kNoErr;
		as_ulog(kLogLevelNotice, "%s: Device(%s) selected\n", __FUNCTION__, outDevice);
	}

	return (err);
}

//===========================================================================================================================
//	_GetCurrentAudioInputDevice
//===========================================================================================================================

static OSStatus _GetCurrentAudioInputDevice(AudioStreamRef inStream, char * outDevice)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);

	char		buffer[256] = {0, };
	size_t		maxLen = sizeof(buffer);
	char*		strAudioType = NULL;
	char*		strAudioDevice = NULL;
	OSStatus	err = kUnknownErr;
#if defined(ECNR_FUNCTION)
	AudioECNR * ecnr = me->audioEcnr;
#endif

	if (me->streamType == kAirPlayStreamType_MainAudio) {
		strAudioType = CFGetCString(me->audioType, buffer, maxLen);
		as_ulog(kLogLevelNotice, "%s: AudioType = %s\n", __FUNCTION__, strAudioType);
#if defined(ECNR_FUNCTION)
		ecnr->setSampleFrames(
				me->format.mSampleRate == 8000 ?
				kECNRSampleSize_8K : kECNRSampleSize_16K_24K);
		ecnr->setSampleRate(me->format.mSampleRate);
		ecnr->setBytesPerFrame(me->format.mBytesPerFrame);
		if (me->frameworkHandle->config->ecnrConfigPath)
			ecnr->setConfigFilePath(me->frameworkHandle->config->ecnrConfigPath);
#endif
		if (strAudioType != NULL) {
			if (strcmp(strAudioType, kAirPlayAudioType_Default) == 0) {
				strAudioDevice = me->frameworkHandle->audioConfig->deflt.inputDevice;
			} else if (strcmp(strAudioType, kAirPlayAudioType_Media) == 0) {
				strAudioDevice = me->frameworkHandle->audioConfig->media.inputDevice;
			} else if (strcmp(strAudioType, kAirPlayAudioType_Telephony) == 0) {
				strAudioDevice = me->frameworkHandle->audioConfig->telephony.inputDevice;
#if defined(ECNR_FUNCTION)
				ecnr->enable(true);
				ecnr->setSiri(false);
#endif
			} else if (strcmp(strAudioType, kAirPlayAudioType_SpeechRecognition) == 0) {
				strAudioDevice = me->frameworkHandle->audioConfig->speechRecognition.inputDevice;
#if defined(ECNR_FUNCTION)
				ecnr->enable(true);
				ecnr->setSiri(true);
#endif
			} else if (strcmp(strAudioType, kAirPlayAudioType_Alert) == 0) {
				strAudioDevice = me->frameworkHandle->audioConfig->alert.inputDevice;
			} else {
				as_ulog(kLogLevelError, "%s: unknown audio type(%s)\n", __FUNCTION__, strAudioType);
			}
		}
	} else {
		strAudioDevice = me->frameworkHandle->audioConfig->alt.inputDevice;
	}

	if (strAudioDevice != NULL) {
		strcpy(outDevice, strAudioDevice);
		err = kNoErr;
		as_ulog(kLogLevelNotice, "%s: Device(%s) selected\n", __FUNCTION__, outDevice);
	}

	return (err);
}

//===========================================================================================================================
//	_CheckAudioDeviceAvailable
//===========================================================================================================================

static OSStatus _CheckAudioDeviceAvailable(AudioStreamRef inStream, Boolean* inDeviceAvailable) {
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);
	OSStatus					err;

	// If copyDeviceAvailable_f delegate is not assigned, it means device is always available
	if (me->frameworkHandle && me->frameworkHandle->platformDelegate.copyDeviceAvailable_f != NULL) {
		Boolean ret = me->frameworkHandle->platformDelegate.copyDeviceAvailable_f(
				me->frameworkHandle->airPlaySession,
				me->frameworkHandle,
				me->streamType);
		if (!ret) {
			*inDeviceAvailable = false;
			return kNotPreparedErr;
		} else {
			*inDeviceAvailable = true;
		}
	} else {
		*inDeviceAvailable = true;
	}

	err = kNoErr;
	return err;
}

//===========================================================================================================================
//	_OpenAudioDevice
//===========================================================================================================================

static OSStatus _OpenAudioDevice(AudioStreamRef inStream, Boolean inInput)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);
	OSStatus					err = kUnknownErr;
	int							sndErr;
	int							i = 0;
	int							maxRetry = kDefaultMaxRetry;
	__useconds_t				retryInterval = kDefaultRetryInterval;
	AudioALSA *					alsa = inInput ? me->inputAlsa : me->outputAlsa;
	CFNumberRef					key = NULL;

	sndErr = alsa->open();
	if (sndErr == -EBUSY) {
		CFDictionaryRef 	pendingTaskMap = me->frameworkHandle->pendingTaskMap;
		pthread_mutex_t	*	pendingTaskLockPtr = me->frameworkHandle->pendingTaskLockPtr;

		key = CFNumberCreateInt64(((uintptr_t)me));
		require(key != NULL, exit);

		// Register current instance into task map
		pthread_mutex_lock(pendingTaskLockPtr);
		CFDictionaryAddValue(pendingTaskMap, key, kCFBooleanFalse);
		pthread_mutex_unlock(pendingTaskLockPtr);

		for (i = 0; i < maxRetry && sndErr == -EBUSY; i++) {
			as_ulog(kLogLevelError, "%s: %s snd_pcm_open failed(%s), retry(%d)\n",
					__FUNCTION__,
					inInput ? "Input audio device" : "Output audio device",
					snd_strerror(sndErr), i);

			// Sleep during specific interval
			usleep(retryInterval);

			// Trying to open audio device again
			sndErr = alsa->open();
			if (sndErr == -EBUSY) {
				CFBooleanRef flag;

				// Check task map whether interrupt is occurred from the application
				pthread_mutex_lock(pendingTaskLockPtr);
				flag = (CFBooleanRef)CFDictionaryGetValue(pendingTaskMap, key);
				pthread_mutex_unlock(pendingTaskLockPtr);

				if (flag != NULL && CFBooleanGetValue(flag)) {
					as_ulog(kLogLevelError, "%s: Retrying is interrupted by application\n", __FUNCTION__);
					i = maxRetry;
				}
			} else if (sndErr == 0) {
				as_ulog(kLogLevelError, "%s: Successfully opened the %s audio device by retrying\n", __FUNCTION__, inInput ? "input" : "output");
			}
		}

		// Unregister current instance into task map
		pthread_mutex_lock(pendingTaskLockPtr);
		CFDictionaryRemoveValue(pendingTaskMap, key);
		pthread_mutex_unlock(pendingTaskLockPtr);
	}

	if (sndErr != 0) {
		as_ulog(kLogLevelError, "%s: %s snd_pcm_open failed(%s)\n",
				__FUNCTION__,
				inInput ? "Input audio device" : "Output audio device",
				snd_strerror(sndErr));
	}

	require_action(sndErr == 0, exit, err = kOpenErr);

	err = kNoErr;
exit:
	CFReleaseNullSafe(key);

	return (err);
}

//===========================================================================================================================
//	_InitAudioDevice
//===========================================================================================================================

static OSStatus _InitAudioDevice(AudioStreamRef inStream, Boolean inInput)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);
	OSStatus					err;
	char strAudioDevice[256] = {0, };
	AudioALSA *					alsa = inInput ? me->inputAlsa : me->outputAlsa;

#if defined(MIBGP)
	char		buffer[256] = {0, };
	size_t		maxLen = sizeof(buffer);
	char*		strAudioType = NULL;

	strAudioType = CFGetCString(me->audioType, buffer, maxLen);
#endif

	if (inInput) {
		err = _GetCurrentAudioInputDevice(inStream, strAudioDevice);
		require_noerr(err, exit);

		alsa->set(strAudioDevice, true,
				me->format.mSampleRate,
#if defined(FORCE_MIC_TO_STEREO)
				2,
#else
				me->format.mChannelsPerFrame,
#endif
				me->format.mBitsPerChannel,
				kDefaultInputPeriodTime, kDefaultInputBufferTime
#if defined(MIBGP)
				, strAudioType
#endif
				);
	} else {
		err = _GetCurrentAudioOutputDevice(inStream, strAudioDevice);
		require_noerr(err, exit);

		alsa->set(strAudioDevice, false,
				me->format.mSampleRate,
				me->format.mChannelsPerFrame,
				me->format.mBitsPerChannel,
				kDefaultOutputPeriodTime, kDefaultOutputBufferTime
#if defined(MIBGP)
				, strAudioType
#endif
				);
	}

	err = kNoErr;
exit:
	return (err);
}

//===========================================================================================================================
//	_SetupAudioDevice
//===========================================================================================================================

static OSStatus _SetupAudioDevice(AudioStreamRef inStream, Boolean inInput)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);
	OSStatus					err;
	int							sndErr = 0;
	AudioALSA *					alsa = inInput ? me->inputAlsa : me->outputAlsa;

	if (inInput) {
		err = _OpenAudioDevice(inStream, true);
		require_noerr(err, exit);

		sndErr = alsa->prepare();
		require(sndErr == 0, exit);
	} else {
		err = _OpenAudioDevice(inStream, false);
		require_noerr(err, exit);

		sndErr = alsa->prepare();
		require(sndErr == 0, exit);
	}

	err = kNoErr;
exit:
	return (err);
}

//===========================================================================================================================
//  AudioStreamPrepare
//===========================================================================================================================

OSStatus AudioStreamPrepare(AudioStreamRef inStream)
{
    AudioStreamImpRef const me = _AudioStreamGetImp(inStream);
    OSStatus                err = kUnknownErr;
    CarPlayAudioStreamDescription format;

    // $$$ TODO: This is where the audio processing chain should be set up based on the properties previously set on the
    // AudioStream object:
    //  me->format specifies the sample rate, channel count, and bit-depth.
    //  me->input specifies whether or not the processing chain should be set up to record audio from the accessory's
    //            microphone(s).
    // Audio output should always be prepared.
    // If the audio processing chain is successfully set up, me->prepared should be set to true.

    as_ulog(kLogLevelNotice, "%s: inStream = %p\n", __FUNCTION__, inStream);

    as_ulog(kLogLevelNotice, "%s: format.mBytesPerPacket = %d\n", __FUNCTION__, me->format.mBytesPerPacket);
    as_ulog(kLogLevelNotice, "%s: format.mSampleRate = %d\n", __FUNCTION__, me->format.mSampleRate);
    as_ulog(kLogLevelNotice, "%s: format.mBytesPerFrame = %d\n", __FUNCTION__, me->format.mBytesPerFrame);
    as_ulog(kLogLevelNotice, "%s: format.mBitsPerChannel = %d\n", __FUNCTION__, me->format.mBitsPerChannel);
    as_ulog(kLogLevelNotice, "%s: format.mChannelsPerFrame = %d\n", __FUNCTION__, me->format.mChannelsPerFrame);
    as_ulog(kLogLevelNotice, "%s: format.mFormatFlags = %d\n", __FUNCTION__, me->format.mFormatFlags);
    as_ulog(kLogLevelNotice, "%s: format.mFormatID = %d\n", __FUNCTION__, me->format.mFormatID);
    as_ulog(kLogLevelNotice, "%s: format.mFramesPerPacket = %d\n", __FUNCTION__, me->format.mFramesPerPacket);
    as_ulog(kLogLevelNotice, "%s: format.mReserved = %d\n", __FUNCTION__, me->format.mReserved);

    // Set audio stream format
    format.mSampleRate       = me->format.mSampleRate;
    format.mBytesPerPacket   = me->format.mBytesPerPacket;
    format.mFramesPerPacket  = me->format.mFramesPerPacket;
    format.mBytesPerFrame    = me->format.mBytesPerFrame;
    format.mChannelsPerFrame = me->format.mChannelsPerFrame;
    format.mBitsPerChannel   = me->format.mBitsPerChannel;

    if (me->frameworkHandle->platformDelegate.audioStreamStarted_f != NULL) {
        me->frameworkHandle->platformDelegate.audioStreamStarted_f(
                me->frameworkHandle->airPlaySession,
                inStream,
                me->frameworkHandle,
                me->streamType,
                me->audioType,
                &format);
    }

    // Setup for Input(Input device open directly for use assp)
    if (me->input) {
#if defined(MIBGP)
        me->inputAlsa = new DeviceMIBSoundManager();
#else
        me->inputAlsa = new DeviceALSA();
#endif
        require_action(me->inputAlsa, exit, err = kNoMemoryErr);

        err = _InitAudioDevice(inStream, true);
        require_noerr(err, exit);

        if (_CheckAudioDeviceAvailable(inStream, &me->isInputDeviceAvalable) == kNoErr) {
            err = _SetupAudioDevice(inStream, true);
            require_noerr(err, exit);
        }

#if defined(ECNR_FUNCTION)
        if ((me->audioEcnr != NULL) && (me->audioEcnr->isEnabled())) {
	        me->audioEcnr->prepare();
	        err = me->audioEcnr->start();
	        require(err == 0, exit);
	    }
#endif
    }

    // Setup for output(Output device open late for timing issue)
#if defined(MIBGP)
        me->outputAlsa = new DeviceMIBSoundManager();
#else
        me->outputAlsa = new DeviceALSA();
#endif
    require_action(me->outputAlsa, exit, err = kNoMemoryErr);

    err = _InitAudioDevice(inStream, false);
    require_noerr(err, exit);

    me->prepared = true;

    err = kNoErr;

exit:

    return (err);
}

//===========================================================================================================================
//	AudioStreamStart
//===========================================================================================================================

OSStatus AudioStreamStart(AudioStreamRef inStream)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);
	OSStatus					err = kUnknownErr;

	as_ulog(kLogLevelNotice, "%s: called\n", __FUNCTION__);


	if (!me->prepared) {
		err = AudioStreamPrepare(inStream);
		require_noerr(err, exit);
	}

	// $$$ TODO: This is where the audio processing chain should be started.
	//
	// me->outputCallbackPtr should be invoked periodically to retrieve a continuous stream of samples to be output.
	// When calling me->outputCallbackPtr(), a buffer is provided for the caller to write into.  Equally important
	// is the inSampleTime and inHostTime arguments.  It is important that accurate { inSampleTime, inHostTime } pairs
	// be provided to the caller.  inSampleTime should be a (reasonably) current running total of the number of samples
	// that have hit the speaker since AudioStreamStart() was called.  inHostTime is the system time, in units of ticks,
	// corresponding to inSampleTime (see TickUtils.h).  This information will be returned to the controller and is
	// a key piece in allowing the controller to derive the relationship between the controller's system clock and the
	// accessory's audio (DAC) clock for A/V sync.
	//
	// If input has been requested (me->input == true), then me->inputCallbackPtr should also be invoked periodically
	// to provide a continuous stream of samples from the accessory's microphone (possibly with some processing, depending
	// on the audio type, see kAudioStreamProperty_AudioType).  If no audio samples are available for whatever reason,
	// the me->inputCallbackPtr should be called with a buffer of zeroes.

	// Play for output
	err = pthread_create(&me->outputThread, NULL, _AudioOutputThread, inStream);
	require(err == 0, exit);

    // Play for Input
    if (me->input) {
    	err = pthread_create(&me->inputThread, NULL, _AudioInputThread, inStream);
		require(err == 0, exit);
    }

	err = kNoErr;

exit:
	if (err) {
		AudioStreamStop(inStream, false);

		if (me->frameworkHandle && me->frameworkHandle->platformDelegate.reportDeviceError_f != NULL) {
			me->frameworkHandle->platformDelegate.reportDeviceError_f(
					me->frameworkHandle->airPlaySession,
					me->frameworkHandle,
					me->streamType,
					err);
		}
	}
	return (err);
}

//===========================================================================================================================
//	AudioStreamStop
//===========================================================================================================================
void AudioStreamStop(AudioStreamRef inStream, Boolean inDrain)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp(inStream);

	// $$$ TODO: This is where the audio processing chain should be stopped, and the audio processing chain torn down.
	// When AudioStreamStop() returns, the object should return to the state similar to before AudioStreamPrepare()
	// was called, so this function is responsible for undoing any resource allocation performed in AudioStreamPrepare().
	(void) inDrain;

	as_ulog(kLogLevelNotice, "%s: called\n", __FUNCTION__);

#if defined(MEEGO)
	pthread_mutex_lock(me->mutexPtr);
#endif
	
	me->prepared = false;

	if (me->outputThread) {
		pthread_join(me->outputThread, NULL);
		me->outputThread = 0;
	}

    if (me->outputAlsa) {
        if (me->outputAlsa->isReady())
            me->outputAlsa->close();
    }

	if (me->inputThread) {
		pthread_join(me->inputThread, NULL);
		me->inputThread = 0;
	}

    if (me->inputAlsa) {
        if (me->inputAlsa->isReady()) {
            me->inputAlsa->close();
        }
    }

#if defined(ECNR_FUNCTION)
	if (me->input) {
        if ((me->audioEcnr != NULL) && (me->audioEcnr->isEnabled())) {
            me->audioEcnr->stop();
        }
    }
#endif

	if (me->frameworkHandle && me->frameworkHandle->platformDelegate.audioStreamStopped_f != NULL) {
		me->frameworkHandle->platformDelegate.audioStreamStopped_f(
                me->frameworkHandle->airPlaySession,
                inStream,
                me->frameworkHandle,
                me->streamType,
                me->audioType);
	}

#if defined(MEEGO)
	pthread_mutex_unlock(me->mutexPtr);
#endif	

	as_ulog(kLogLevelNotice, "%s: ended\n", __FUNCTION__);
}

#if 0
#pragma mark -
#endif

//===========================================================================================================================
//	AudioSessionSetEventHandler
//===========================================================================================================================

void AudioSessionSetEventHandler(AudioSessionEventHandler_f inHandler, void *inContext)
{
	(void) inHandler;
	(void) inContext;

	// This implementation should remain empty.
	as_ulog(kLogLevelNotice, "%s: called\n", __FUNCTION__);
}

//===========================================================================================================================
//	AudioSessionEnsureSetup
//===========================================================================================================================

void AudioSessionEnsureSetup(
		Boolean		inHasInput,
		uint32_t	inPreferredSystemSampleRate,
		uint32_t	inPreferredSystemBufferSizeMicros)
{
	(void) inHasInput;
	(void) inPreferredSystemSampleRate;
	(void) inPreferredSystemBufferSizeMicros;

	// This implementation should remain empty.
	as_ulog(kLogLevelNotice, "%s: called\n", __FUNCTION__);
}

//===========================================================================================================================
//	AudioSessionEnsureTornDown
//===========================================================================================================================

void AudioSessionEnsureTornDown(void)
{
	struct CarPlayAppFrameworkPrivate *	handle = NULL;
	int64_t								value = 0;
	OSStatus							err = kUnknownErr;

	as_ulog(kLogLevelNotice, "%s: called\n", __FUNCTION__);

	value = CarPlaySettings_GetInt64(kFrameworkHandle, &err);
	require_noerr(err, exit);

	handle = (struct CarPlayAppFrameworkPrivate *)((uintptr_t)value);
	require(handle != NULL, exit);

	if (handle->platformDelegate.audioSessionTornDown_f != NULL) {
		handle->platformDelegate.audioSessionTornDown_f(handle->airPlaySession, handle);
	}

exit:
	return;
}

//===========================================================================================================================
//	AudioSessionGetSupportedInputFormats
//===========================================================================================================================

AudioSessionAudioFormat	AudioSessionGetSupportedInputFormats(AudioStreamType inStreamType, CFStringRef inAudioType )
{
	AudioSessionAudioFormat				formats = 0;
	OSStatus							err = 0;
	struct CarPlayAppFrameworkPrivate *	handle = NULL;
	int64_t								value = 0;

	as_ulog(kLogLevelNotice, "%s: called(%s)\n", __FUNCTION__, AirPlayStreamTypeToString(inStreamType));

	value = CarPlaySettings_GetInt64(kFrameworkHandle, &err);
	require_noerr(err, exit);

	handle = (struct CarPlayAppFrameworkPrivate *)((uintptr_t)value);
	require(handle != NULL, exit);

	// $$$ TODO: This is where the accessory provides a list of audio input formats it supports in hardware.
	// It is important that, at a minimum, all sample rates required by the specification are included here.

	switch( inStreamType )
	{
		case kAudioStreamType_MainAudio:
			if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Compatibility ) ) )
				formats = 	handle->audioConfig->deflt.inputFormat
						 |	handle->audioConfig->media.inputFormat
						 |	handle->audioConfig->speechRecognition.inputFormat
						 |	handle->audioConfig->telephony.inputFormat
						 |	handle->audioConfig->alert.inputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Default ) ) )
				formats = 	handle->audioConfig->deflt.inputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Media ) ) )
				formats = 	handle->audioConfig->media.inputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Telephony ) ) )
				formats = 	handle->audioConfig->telephony.inputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_SpeechRecognition ) ) )
				formats = 	handle->audioConfig->speechRecognition.inputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Alert ) ) )
				formats = 	handle->audioConfig->alert.inputFormat;
			else
				formats = 0;
			break;

		case kAudioStreamType_AltAudio:
			if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Compatibility ) ) )
				formats =	handle->audioConfig->alt.inputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Default ) ) )
				formats =	handle->audioConfig->alt.inputFormat;
			else
				formats = 0;
			break;

		default:
			formats = 0;
			break;
	}

	err = kNoErr;

	as_ulog(kLogLevelNotice, "%s: return format - 0x%08x\n", __FUNCTION__, formats);

exit:
	if (err) {
		as_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return (formats);
}

//===========================================================================================================================
//	AudioSessionGetSupportedOutputFormats
//===========================================================================================================================

AudioSessionAudioFormat AudioSessionGetSupportedOutputFormats(AudioStreamType inStreamType, CFStringRef inAudioType)
{
	AudioSessionAudioFormat				formats = 0;
	OSStatus							err = 0;
	struct CarPlayAppFrameworkPrivate *	handle = NULL;
	int64_t								value = 0;

	as_ulog(kLogLevelNotice, "%s: called(%s)\n", __FUNCTION__, AirPlayStreamTypeToString(inStreamType));

	value = CarPlaySettings_GetInt64(kFrameworkHandle, &err);
	require_noerr(err, exit);

	handle = (struct CarPlayAppFrameworkPrivate *)((uintptr_t)value);
	require(handle != NULL, exit);

	// $$$ TODO: This is where the accessory provides a list of audio output formats it supports in hardware.
	// It is expected that the list of supported audio output formats is a superset of the supported audio
	// input formats.  As with input formats, it is important that, at a minimum, all sample rates required
	// by the specification are included here.
	formats = AudioSessionGetSupportedInputFormats(inStreamType, inAudioType);

	switch( inStreamType )
	{
		case kAudioStreamType_MainAudio:
			if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Compatibility ) ) )
				formats |= 	handle->audioConfig->deflt.outputFormat
						 |	handle->audioConfig->media.outputFormat
						 |	handle->audioConfig->speechRecognition.outputFormat
						 |	handle->audioConfig->telephony.outputFormat
						 |	handle->audioConfig->alert.outputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Default ) ) )
				formats |= 	handle->audioConfig->deflt.outputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Media ) ) )
				formats |= 	handle->audioConfig->media.outputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Telephony ) ) )
				formats |= 	handle->audioConfig->telephony.outputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_SpeechRecognition ) ) )
				formats |= 	handle->audioConfig->speechRecognition.outputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Alert ) ) )
				formats |= 	handle->audioConfig->alert.outputFormat;
			else
				formats |= 0;
			break;

		case kAudioStreamType_AltAudio:
			if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Compatibility ) ) )
				formats |=	handle->audioConfig->alt.outputFormat;
			else if( CFEqual( inAudioType, CFSTR( kAirPlayAudioType_Default ) ) )
				formats |=	handle->audioConfig->alt.outputFormat;
			else
				formats |= 0;
			break;

		default:
			formats = 0;
			break;
	}

	err = kNoErr;

	as_ulog(kLogLevelNotice, "%s: return format - 0x%08x\n", __FUNCTION__, formats);

exit:
	if (err) {
		as_ulog(kLogLevelError, "%s failed(%d)\n", __FUNCTION__, err);
	}

	return (formats);
}

//===========================================================================================================================
//	_CreateLatencyDictionary
//===========================================================================================================================
static CFDictionaryRef
_CreateLatencyDictionary(
						 AudioStreamType inStreamType,
						 CFStringRef inAudioType,
						 uint32_t inSampleRate,
						 uint32_t inSampleSize,
						 uint32_t inChannels,
						 uint32_t inInputLatency,
						 uint32_t inOutputLatency,
						 OSStatus *outErr )
{
	CFDictionaryRef						result = NULL;
	OSStatus							err;
	CFMutableDictionaryRef				latencyDict;

	latencyDict = CFDictionaryCreateMutable( NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks );
	require_action( latencyDict, exit, err = kNoMemoryErr );

	if( inStreamType != kAudioStreamType_Invalid )	CFDictionarySetInt64( latencyDict, kAudioSessionKey_Type, inStreamType );
	if( inAudioType )		CFDictionarySetValue( latencyDict, kAudioSessionKey_AudioType, inAudioType );
	if( inSampleRate > 0 )	CFDictionarySetInt64( latencyDict, kAudioSessionKey_SampleRate, inSampleRate );
	if( inSampleSize > 0 )	CFDictionarySetInt64( latencyDict, kAudioSessionKey_SampleSize, inSampleSize );
	if( inChannels > 0 )	CFDictionarySetInt64( latencyDict, kAudioSessionKey_Channels, inChannels );
	if( inInputLatency > 0) CFDictionarySetInt64( latencyDict, kAudioSessionKey_InputLatencyMicros, inInputLatency );
	CFDictionarySetInt64( latencyDict, kAudioSessionKey_OutputLatencyMicros, inOutputLatency );

	result = latencyDict;
	latencyDict = NULL;
	err = kNoErr;
exit:
	CFReleaseNullSafe( latencyDict );
	if( outErr ) *outErr = err;
	return( result );
}

//===========================================================================================================================
// AudioSessionCopyLatencies
//===========================================================================================================================

CFArrayRef AudioSessionCopyLatencies( OSStatus *outErr )
{
	CFArrayRef							result = NULL;
	OSStatus							err;
	CFMutableArrayRef					audioLatenciesArray;
	CFDictionaryRef						dict = NULL;
	struct CarPlayAppFrameworkPrivate *	handle = NULL;
	int64_t								value = 0;
	uint64_t							inputLatency, outputLatency;

	value = CarPlaySettings_GetInt64(kFrameworkHandle, &err);
	require_noerr(err, exit);

	handle = (struct CarPlayAppFrameworkPrivate *)((uintptr_t)value);
	require(handle != NULL, exit);

	audioLatenciesArray = CFArrayCreateMutable( NULL, 0, &kCFTypeArrayCallBacks );
	require_action( audioLatenciesArray, exit, err = kNoMemoryErr );

	// default(media) latencies

    outputLatency =  handle->audioConfig->media.outputLatency             ? handle->audioConfig->media.outputLatency :
                     handle->audioConfig->alert.outputLatency             ? handle->audioConfig->alert.outputLatency :
                     handle->audioConfig->telephony.outputLatency         ? handle->audioConfig->telephony.outputLatency :
                     handle->audioConfig->speechRecognition.outputLatency ? handle->audioConfig->speechRecognition.outputLatency :
                     handle->audioConfig->deflt.outputLatency             ? handle->audioConfig->deflt.outputLatency :
                                                                            handle->audioConfig->alt.outputLatency;

	dict = _CreateLatencyDictionary(
									kAudioStreamType_Invalid,					// inStreamType
									NULL,										// inAudioType
									0,											// inSampleRate,
									0,											// inSampleSize,
									0,											// inChannels,
									0,								            // inInputLatency,
									outputLatency,								// inOutputLatency,
									&err );
	require_noerr( err, exit );

	CFArrayAppendValue( audioLatenciesArray, dict );
	ForgetCF( &dict );

	// telephony latencies - set real latency

    inputLatency  = handle->audioConfig->telephony.inputLatency  + kECNRSendLatency_8K_16K;
    outputLatency = handle->audioConfig->telephony.outputLatency + kECNRRecvLatency_8K_16K;

	dict = _CreateLatencyDictionary(
									kAudioStreamType_MainAudio,					// inStreamType
									kAudioStreamAudioType_Telephony,			// inAudioType
									0,											// inSampleRate,
									0,											// inSampleSize,
									0,											// inChannels,
                                    inputLatency,                               // inInputLatency,
                                    outputLatency,                              // inOutputLatency,
									&err );
	require_noerr( err, exit );

	CFArrayAppendValue( audioLatenciesArray, dict );
	ForgetCF( &dict );

    // telephony latencies - set real latency

    inputLatency  = handle->audioConfig->telephony.inputLatency + kECNRSendLatency_24K ;
    outputLatency = handle->audioConfig->telephony.outputLatency + kECNRRecvLatency_24K;

    dict = _CreateLatencyDictionary(
                                    kAudioStreamType_MainAudio,                 // inStreamType
                                    kAudioStreamAudioType_Telephony,            // inAudioType
                                    24000,                                      // inSampleRate,
                                    0,                                          // inSampleSize,
                                    0,                                          // inChannels,
                                    inputLatency,                               // inInputLatency,
                                    outputLatency,                              // inOutputLatency,
                                    &err );
    require_noerr( err, exit );

    CFArrayAppendValue( audioLatenciesArray, dict );
    ForgetCF( &dict );

	// SpeechRecognition latencies - set real latency

    inputLatency  = handle->audioConfig->speechRecognition.inputLatency + kECNRSendLatency_24K;
    outputLatency = handle->audioConfig->speechRecognition.outputLatency;


	dict = _CreateLatencyDictionary(
									kAudioStreamType_MainAudio,					// inStreamType
									kAudioStreamAudioType_SpeechRecognition,	// inAudioType
									0,											// inSampleRate,
									0,											// inSampleSize,
									0,											// inChannels,
                                    inputLatency,                               // inInputLatency,
                                    outputLatency,                              // inOutputLatency,
									&err );
	require_noerr( err, exit );

	CFArrayAppendValue( audioLatenciesArray, dict );
	ForgetCF( &dict );

	// default audio type latencies
	if ((handle->audioConfig->deflt.inputLatency + handle->audioConfig->deflt.outputLatency) > 0) {

        inputLatency  = handle->audioConfig->deflt.inputLatency + kECNRSendLatency_24K;
        outputLatency = handle->audioConfig->deflt.outputLatency;

		dict = _CreateLatencyDictionary(
										kAudioStreamType_MainAudio,					// inStreamType
										kAudioStreamAudioType_Default,				// inAudioType
										0,											// inSampleRate,
										0,											// inSampleSize,
										0,											// inChannels,
                                        inputLatency,                               // inInputLatency,
                                        outputLatency,                              // inOutputLatency,
										&err );
		require_noerr( err, exit );

		CFArrayAppendValue( audioLatenciesArray, dict );
		ForgetCF( &dict );
	}

	// alert audio type latencies
	if ((handle->audioConfig->alert.inputLatency + handle->audioConfig->alert.outputLatency) > 0) {
		dict = _CreateLatencyDictionary(
										kAudioStreamType_MainAudio,					// inStreamType
										kAudioStreamAudioType_Alert,				// inAudioType
										0,											// inSampleRate,
										0,											// inSampleSize,
										0,											// inChannels,
										handle->audioConfig->alert.inputLatency,	// inInputLatency,
										handle->audioConfig->alert.outputLatency,	// inOutputLatency,
										&err );
		require_noerr( err, exit );

		CFArrayAppendValue( audioLatenciesArray, dict );
		ForgetCF( &dict );
	}

	// alt stream type latencies - set 0 latencies for now
	if ((handle->audioConfig->alt.inputLatency + handle->audioConfig->alt.outputLatency) > 0) {
		dict = _CreateLatencyDictionary(
										kAudioStreamType_AltAudio,					// inStreamType
										kAudioStreamType_Invalid,					// inAudioType
										0,											// inSampleRate,
										0,											// inSampleSize,
										0,											// inChannels,
										handle->audioConfig->alt.inputLatency,	    // inInputLatency,
										handle->audioConfig->alt.outputLatency,	    // inOutputLatency,
										&err );
		require_noerr( err, exit );

		CFArrayAppendValue( audioLatenciesArray, dict );
		ForgetCF( &dict );
	}

	result = audioLatenciesArray;
	audioLatenciesArray = NULL;
	err = kNoErr;

exit:
	CFReleaseNullSafe( dict );
	CFReleaseNullSafe( audioLatenciesArray );
	if( outErr ) *outErr = err;
	return( result );
}

#if 0
#pragma mark -
#endif

//===========================================================================================================================
//	_ConvertStereoToMono
//===========================================================================================================================

static OSStatus _ConvertStereoToMono(uint16_t *inoutBuffer, snd_pcm_uframes_t inFrame, uint32_t inBitsPerChannel)
{
	OSStatus err = kUnknownErr;
	uint16_t *mono = inoutBuffer;
	uint16_t *stereo = inoutBuffer;

	require(inoutBuffer != NULL, exit);
	require(inFrame > 0, exit);
	require(inBitsPerChannel == 16, exit);

	while (inFrame > 0) {
		*(mono++) = *(stereo++);
		++stereo;
		--inFrame;
	}

	err = kNoErr;
exit:
	return err;
}

//===========================================================================================================================
//	_AudioOutputRunLoop
//===========================================================================================================================

static void * _AudioOutputThread(void *inParam)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp((AudioStreamRef)inParam);
	AudioStreamRef 				stream = (AudioStreamRef)inParam;
	OSStatus					err;
	uint32_t					sampleTime  = 0;
	uint32_t					sampleTimeWithDelay = 0;
	uint64_t					hostTime = 0;
	uint8_t *					outputAudioBuffer = NULL;
	size_t						audioBufferLength = 0;
	size_t						audioFrameLength = 0;
	snd_pcm_uframes_t			reqFrames = 0;
	snd_pcm_sframes_t			delay = 0;
	AudioALSA *					alsa = me->outputAlsa;
#if defined(ECNR_FUNCTION)
	size_t						readBytes = 0;
	AudioECNR *					ecnr = me->audioEcnr;
#endif

#if (LOG_INTERVAL)
	uint64_t					prevHostTime = 0;
	uint32_t					prevSampleTime = 0;
	FILE *						log_fd = NULL;
	char 						file_name[100];
#endif

#if (DUMP_OUTPUT_STREAM)
    FILE *                      pcm_fd = NULL;
    char                        file_name[100];
#endif

    SetThreadName(me->threadName ? me->threadName : "CarPlayAudioOutput");
    SetCurrentThreadPriority(me->threadPriority ? me->threadPriority : kAirPlayThreadPriority_AudioReceiver);

	as_ulog(kLogLevelWarning, "%s: started, inStream = %p\n", __FUNCTION__, inParam);
	as_ulog(kLogLevelWarning, "%s: sampleRate = %d\n", __FUNCTION__, me->format.mSampleRate);

#if defined(ECNR_FUNCTION)
	if (ecnr->isEnabled()) {
		audioFrameLength = ecnr->getSampleFrames();
	} else
#endif
	{
		audioFrameLength = USEC_TO_FRAMES(me->format.mSampleRate, kDefaultOutputTimeInterval);
	}
	audioBufferLength = audioFrameLength * me->format.mBytesPerFrame;
	as_ulog(kLogLevelNotice, "%s: audioBufferLength = %d\n", __FUNCTION__, audioBufferLength);
	as_ulog(kLogLevelNotice, "%s: audioFrameLength = %d\n", __FUNCTION__, audioFrameLength);

#if (LOG_INTERVAL)
	sprintf( file_name, FILE_PATH "%s_0x%x_%d_output.log",
	        me->audioType?CFStringGetCStringPtr(me->audioType, kCFStringEncodingUTF8):"AltAudio",
	        (unsigned int)inParam, me->format.mSampleRate );
	log_fd = fopen(file_name, "w+");

	if(log_fd == NULL)
		as_ulog(kLogLevelError, "%s: CANNOT open log file.\n", __FUNCTION__);
	else {
	    as_ulog(kLogLevelNotice, "%s: create %s\n", __FUNCTION__, file_name);
		fprintf( log_fd, "HostTime\t SampleTimeDiff\t HostTimeDiff\t tDiff\n");
	}
#endif

#if (DUMP_OUTPUT_STREAM)
    sprintf(file_name, FILE_PATH "%s_0x%x_%d_output.pcm",
            me->audioType?CFStringGetCStringPtr(me->audioType, kCFStringEncodingUTF8):"AltAudio",
            (unsigned int)inParam, me->format.mSampleRate );
    pcm_fd = fopen(file_name, "w+");

    if(pcm_fd == NULL)
        as_ulog(kLogLevelError, "%s: CANNOT open pcm file.\n", __FUNCTION__);
    else
        as_ulog(kLogLevelNotice, "%s: create %s\n", __FUNCTION__, file_name);
#endif

	as_ulog(kLogLevelNotice, "%s: me->frameworkHandle->isAltAudioMuted - %s\n", __FUNCTION__, me->frameworkHandle->isAltAudioMuted ? "true" : "false");
	while (me->prepared) {
		if (me->isOutputDeviceAvalable) {
			/* Set Audio Buffer */
			if (outputAudioBuffer == NULL) {
				outputAudioBuffer = (uint8_t *)calloc(1, audioBufferLength);
				require(outputAudioBuffer != NULL, exit);
			}

			memset(outputAudioBuffer, 0, audioBufferLength);

			reqFrames = audioFrameLength;
#if defined(ECNR_FUNCTION)
			if (ecnr->isEnabled() && !ecnr->getSiri()) {
				readBytes = ecnr->extractRecvOutBuffer(outputAudioBuffer, audioBufferLength);

				if (readBytes) {
					hostTime = UpTicks();
					delay = alsa->getDelay();
					sampleTimeWithDelay = sampleTime - delay;	// Set here for LOG Inverval print.

					if(me->streamType == kAirPlayStreamType_AltAudio && me->frameworkHandle->isAltAudioMuted) {
						memset(outputAudioBuffer, 0, audioBufferLength);
					}
					alsa->write(outputAudioBuffer, readBytes / me->format.mBytesPerFrame);
					sampleTime += (readBytes / me->format.mBytesPerFrame);
				} else{ usleep(100); }
			} else
#endif
			{
				hostTime = UpTicks();
				delay = alsa->getDelay();
				sampleTimeWithDelay = sampleTime - delay;		// Set here for LOG Inverval print.

				me->outputCallbackPtr(sampleTimeWithDelay, hostTime, outputAudioBuffer,
					audioBufferLength, me->outputCallbackCtx);

				if(me->streamType == kAirPlayStreamType_AltAudio && me->frameworkHandle->isAltAudioMuted) {
					memset(outputAudioBuffer, 0, audioBufferLength);
				}					
				alsa->write(outputAudioBuffer, reqFrames);
				sampleTime += reqFrames;
			}
#if (LOG_INTERVAL)
			if (prevHostTime != hostTime && prevSampleTime != sampleTimeWithDelay) {
				uint64_t hostTimeDiff = (hostTime - prevHostTime);
				int64_t sampleTimeDiff = (int64_t)((int64_t)sampleTimeWithDelay
						- (int64_t)prevSampleTime) * UpTicksPerSecond() / (uint64_t)me->format.mSampleRate;
				int64_t diff = hostTimeDiff - sampleTimeDiff;
				fprintf( log_fd, "%llu\t %lld\t %llu\t %lld\n",hostTime, sampleTimeDiff, hostTimeDiff, diff);

	            prevHostTime = hostTime;
	            prevSampleTime = sampleTimeWithDelay;
			}
#endif

#if (DUMP_OUTPUT_STREAM)
			if (readBytes && pcm_fd) {
                if(fwrite(outputAudioBuffer, 1, audioBufferLength, pcm_fd) == 0) {
                    as_ulog(kLogLevelError, "%s: pcm fwrite failed..(frame: %d)\n", __FUNCTION__, reqFrames);
                }
            }
#endif /* #if defined(DUMP_OUTPUT_STREAM) */

#if defined(MEEGO)
			// Meego want to check device available when audio sound playing.
			// If device is not available, send "0" like mute. jaeheon.kim@lge.com, 2015. 08. 19
			_CheckAudioDeviceAvailable(stream, &me->isOutputDeviceAvalable);
#endif
		} else {
			uint32_t timeToSleep = 0;

#if defined(ECNR_FUNCTION)
			if (!ecnr->isEnabled() || (ecnr->isEnabled() && ecnr->isBufferingComplete()))
#endif
			{
				if (_CheckAudioDeviceAvailable(stream, &me->isOutputDeviceAvalable) == kNoErr) {
					if (!alsa->isReady()) {
						err = _SetupAudioDevice(stream, false);
						require_noerr(err, exit);
					}
					continue;
				}
			}

			// Set 5 msec loop
			reqFrames = USEC_TO_FRAMES(me->format.mSampleRate, 5000);

			uint8_t * tempBuffer = (uint8_t *)calloc(1, reqFrames * me->format.mBytesPerFrame);
			require(tempBuffer != NULL, exit);

			hostTime = UpTicks();

			me->outputCallbackPtr(sampleTime, hostTime, tempBuffer,
					reqFrames * me->format.mBytesPerFrame, me->outputCallbackCtx);
			sampleTime += reqFrames;

			if (tempBuffer != NULL) free(tempBuffer);

			timeToSleep = (uint32_t)((reqFrames * 1000.0f * 1000.0f) / me->format.mSampleRate);
			usleep(timeToSleep);
		}
	}

exit:
	as_ulog(kLogLevelWarning, "%s: exit, inStream = %p\n", __FUNCTION__, inParam);

	if (outputAudioBuffer != NULL)
		free(outputAudioBuffer);

#if (LOG_INTERVAL)
	if(log_fd)
		fclose(log_fd);
#endif

#if (DUMP_OUTPUT_STREAM)
    if (pcm_fd)
        fclose(pcm_fd);
#endif

    pthread_exit(NULL);

    return (NULL);
}

//===========================================================================================================================
//	_AudioInputRunLoop
//===========================================================================================================================

static void * _AudioInputThread(void *inParam)
{
	AudioStreamImpRef const		me = _AudioStreamGetImp((AudioStreamRef)inParam);
	AudioStreamRef 				stream = (AudioStreamRef)inParam;
	OSStatus					err;
	uint32_t					sampleTime  = 0;
	uint32_t					sampleTimeWithDelay = 0;
	uint64_t					hostTime = 0;
	uint8_t *					inputAudioBuffer = NULL;
	size_t						audioBufferLength = 0;
	size_t						audioFrameLength = 0;
	snd_pcm_sframes_t			retVal = 0;
	snd_pcm_uframes_t			delay = 0;
	snd_pcm_uframes_t			reqFrames = 0;
	Boolean						isForceMicToStereo = false;

	AudioALSA *					alsa = me->inputAlsa;
#if defined(ECNR_FUNCTION)
	size_t						readBytes = 0;
	uint8_t *					outputAudioBuffer = NULL;
	AudioECNR *					ecnr = me->audioEcnr;
#endif

#if (LOG_INTERVAL)
	uint64_t					prevHostTime = 0;
	uint32_t					prevSampleTime = 0;
	FILE *						log_fd = NULL;
	char 						file_name[100];
#endif

#if (DUMP_INPUT_STREAM)
	FILE *						pcm_fd = NULL;
	FILE *						stereo_pcm_fd = NULL;
    char                        file_name[100];
#endif

    SetThreadName(me->threadName ? me->threadName : "CarPlayAudioInput");
    SetCurrentThreadPriority(me->threadPriority ? me->threadPriority : kAirPlayThreadPriority_AudioReceiver);

	as_ulog(kLogLevelWarning, "%s: started, inStream = %p\n", __FUNCTION__, inParam);
	as_ulog(kLogLevelWarning, "%s: sampleRate = %d\n", __FUNCTION__, me->format.mSampleRate);

	// Get audio frame length
	if (alsa->getPeriodSize() < USEC_TO_FRAMES(me->format.mSampleRate, kDefaultInputTimeInterval)) {
		if ((USEC_TO_FRAMES(me->format.mSampleRate, kDefaultInputTimeInterval)%alsa->getPeriodSize()) == 0) {
			audioFrameLength = USEC_TO_FRAMES(me->format.mSampleRate, kDefaultInputTimeInterval);
		} else {
			audioFrameLength = alsa->getPeriodSize() *
							   (USEC_TO_FRAMES(me->format.mSampleRate, kDefaultInputTimeInterval)/alsa->getPeriodSize());
		}
	} else {
		audioFrameLength = alsa->getPeriodSize();
	}
	audioBufferLength = audioFrameLength * me->format.mBytesPerFrame;
	as_ulog(kLogLevelNotice, "%s: audioBufferLength = %d\n", __FUNCTION__, audioBufferLength);
	as_ulog(kLogLevelNotice, "%s: audioFrameLength = %d\n", __FUNCTION__, audioFrameLength);

#if (LOG_INTERVAL)
    sprintf( file_name, FILE_PATH "%s_0x%x_%d_input.log",
            me->audioType?CFStringGetCStringPtr(me->audioType, kCFStringEncodingUTF8):"AltAudio",
            (unsigned int)inParam, me->format.mSampleRate );
    log_fd = fopen(file_name, "w+");

	if (log_fd == NULL)
		as_ulog(kLogLevelError, "%s: CANNOT open log file.\n", __FUNCTION__);
	else {
	    as_ulog(kLogLevelNotice, "%s: create %s\n", __FUNCTION__, file_name);
        fprintf( log_fd, "HostTime\t SampleTimeDiff\t HostTimeDiff\t tDiff\n");
	}
#endif

#if (DUMP_INPUT_STREAM)
    sprintf(file_name, FILE_PATH "%s_0x%x_%d_input.pcm",
            me->audioType?CFStringGetCStringPtr(me->audioType, kCFStringEncodingUTF8):"AltAudio",
            (unsigned int)inParam, me->format.mSampleRate );
    pcm_fd = fopen(file_name, "w+");

    if(pcm_fd == NULL)
        as_ulog(kLogLevelError, "%s: CANNOT open pcm file.\n", __FUNCTION__);
    else
        as_ulog(kLogLevelNotice, "%s: create %s\n", __FUNCTION__, file_name);

    #if defined(FORCE_MIC_TO_STEREO)
    sprintf(file_name, FILE_PATH "%s_0x%x%d_input_stereo.pcm",
            me->audioType?CFStringGetCStringPtr(me->audioType, kCFStringEncodingUTF8):"AltAudio",
            (unsigned int)inParam, me->format.mSampleRate );
    stereo_pcm_fd = fopen(file_name, "w+");

    if(stereo_pcm_fd == NULL)
        as_ulog(kLogLevelError, "%s: CANNOT open pcm file.\n", __FUNCTION__);
    else
        as_ulog(kLogLevelNotice, "%s: create %s\n", __FUNCTION__, file_name);
    #endif
#endif

#if defined(FORCE_MIC_TO_STEREO)
	if (me->format.mChannelsPerFrame == 1) {
		as_ulog(kLogLevelNotice, "%s: audio input format is forced to stereo from mono, input audio buffer length will be twice.\n", __FUNCTION__);
		isForceMicToStereo = true;
	}
#endif

	while (me->prepared) {
		if (me->isInputDeviceAvalable) {
			/* Set Audio Buffer */
#if defined(ECNR_FUNCTION)
			if (ecnr->isEnabled()) {
				if (outputAudioBuffer == NULL) {
					outputAudioBuffer = (uint8_t *)calloc(1, audioBufferLength);
					require(outputAudioBuffer != NULL, exit);
				}
				memset(outputAudioBuffer, 0, audioBufferLength);
			}
#endif
			if (inputAudioBuffer == NULL) {
				if (isForceMicToStereo)
					inputAudioBuffer = (uint8_t *)calloc(1, 2*audioBufferLength);
				else
					inputAudioBuffer = (uint8_t *)calloc(1, audioBufferLength);
				require(inputAudioBuffer != NULL, exit);
			}
			memset(inputAudioBuffer, 0, audioBufferLength); // it is not consider about 2 times length, bcuz used area need to initialization.

			retVal = alsa->read(inputAudioBuffer, audioFrameLength);
			if (retVal > 0) {
				if (isForceMicToStereo) {
#if (DUMP_INPUT_STREAM)
					if (stereo_pcm_fd) {
						if (fwrite(inputAudioBuffer, 1, audioBufferLength, stereo_pcm_fd) == 0) {
							as_ulog(kLogLevelError, "%s: stereo pcm fwrite failed..(frame: %d)\n", __FUNCTION__, audioBufferLength);
						}
					}
#endif
					_ConvertStereoToMono((uint16_t *)inputAudioBuffer, audioFrameLength, me->format.mBitsPerChannel);
				}

				hostTime = UpTicks();
#if defined (MEEGO)
                delay = alsa->getAvail();       // Meego has not work delay correctly.
#else
                delay = alsa->getDelay();
#endif
                sampleTimeWithDelay = sampleTime + delay;

#if defined(ECNR_FUNCTION)
				if (ecnr->isEnabled()) {
					// If audiotype is speech recongnition, recv in buffer use empty buffer.
					if (!ecnr->getSiri()) {
						me->outputCallbackPtr(sampleTimeWithDelay, hostTime, outputAudioBuffer,
								retVal * me->format.mBytesPerFrame, me->outputCallbackCtx);
					}

					ecnr->insertMicInBuffer(inputAudioBuffer, retVal * me->format.mBytesPerFrame);
					ecnr->insertRecvInBuffer(outputAudioBuffer, retVal * me->format.mBytesPerFrame);

					// If audiotype is speech recongnition, extract recv out buffer here.
                    if (ecnr->getSiri()) {
                        ecnr->extractRecvOutBuffer(outputAudioBuffer, retVal * me->format.mBytesPerFrame);
                    }
					readBytes = ecnr->extractMicOutBuffer(inputAudioBuffer, retVal * me->format.mBytesPerFrame);
				} else {
					readBytes = retVal * me->format.mBytesPerFrame;
				}

				if (readBytes > 0)
#endif /* defined(ECNR_FUNCTION) */
				{
	                // 1. When MIC Mute Flag is set. jaeheon.kim@lge.co, 2015. 5. 7.
	                if (me->frameworkHandle->isAudioMICMuted       == true) {
	                    memset(inputAudioBuffer, 0, audioFrameLength * me->format.mBytesPerFrame);
	                }

					me->inputCallbackPtr(sampleTimeWithDelay, hostTime, inputAudioBuffer,
							retVal * me->format.mBytesPerFrame, me->inputCallbackCtx);
				}

#if (LOG_INTERVAL)
				if (prevHostTime != hostTime && prevSampleTime != sampleTimeWithDelay) {
					uint64_t hostTimeDiff = (hostTime - prevHostTime);
					int64_t sampleTimeDiff = (int64_t)((int64_t)sampleTimeWithDelay
							- (int64_t)prevSampleTime) * UpTicksPerSecond() / (uint64_t)me->format.mSampleRate;
					int64_t diff = hostTimeDiff - sampleTimeDiff;
					fprintf( log_fd, "%llu\t %lld\t %llu\t %lld\n",hostTime, sampleTimeDiff, hostTimeDiff, diff);

	                prevHostTime = hostTime;
	                prevSampleTime = sampleTimeWithDelay;
				}

#endif /* #if (LOG_INTERVAL) */

#if (DUMP_INPUT_STREAM)
				if (pcm_fd) {
					if(fwrite(inputAudioBuffer, 1, audioFrameLength * me->format.mBytesPerFrame, pcm_fd) == 0) {
						as_ulog(kLogLevelError, "%s: pcm fwrite failed..(frame: %d)\n", __FUNCTION__, audioFrameLength);
					}
				}
#endif /* #if defined(DUMP_INPUT_STREAM) */
				sampleTime += retVal;
#if defined(MEEGO)
			// Meego want to check device available when audio sound playing.
			// If device is not available, send "0" like mute. jaeheon.kim@lge.com, 2015. 08. 19
			_CheckAudioDeviceAvailable(stream, &me->isInputDeviceAvalable);
#endif
			}
		} else {
			uint32_t timeToSleep = 0;

			if (_CheckAudioDeviceAvailable(stream, &me->isInputDeviceAvalable) == kNoErr) {
				if (!alsa->isReady()) {
					err = _SetupAudioDevice(stream, true);
					require_noerr(err, exit);
				}

				continue;
			}

			// Set 5 msec loop
			reqFrames = USEC_TO_FRAMES(me->format.mSampleRate, 5000);

			uint8_t * tempBuffer =  (uint8_t *)calloc(1, reqFrames * me->format.mBytesPerFrame);
			require(tempBuffer != NULL, exit);

			hostTime = UpTicks();

			me->inputCallbackPtr(sampleTime, hostTime, tempBuffer,
					reqFrames * me->format.mBytesPerFrame, me->inputCallbackCtx);
			sampleTime += reqFrames;

			if (tempBuffer != NULL) free(tempBuffer);

			timeToSleep = (uint32_t)(((float)reqFrames * 1000 * 1000) / me->format.mSampleRate);
			usleep(timeToSleep);
		}
	}

exit:
	as_ulog(kLogLevelWarning, "%s: exit, inStream = %p\n", __FUNCTION__, inParam);

	if (inputAudioBuffer != NULL)
		free(inputAudioBuffer);

#if defined(ECNR_FUNCTION)
	if (outputAudioBuffer != NULL)
		free(outputAudioBuffer);
#endif

#if (LOG_INTERVAL)
	if (log_fd)
		fclose(log_fd);
#endif

#if (DUMP_INPUT_STREAM)
	if (pcm_fd)
		fclose(pcm_fd);

	if (stereo_pcm_fd)
		fclose(stereo_pcm_fd);
#endif

    pthread_exit(NULL);

    return (NULL);
}

