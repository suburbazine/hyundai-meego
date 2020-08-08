/*
	File:       CarPlayScreenGStreamer.c
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

// Write Stream to file function is used only test purpose.
// If you use this function, uncomment #define WRITE_STREAM_TO_FILE line.
// stream data changed to ANNEX-B type, Because every player need ANNEX-B type.
// jaeheon.kim@lge.com 2016. 8.13
//#define WRITE_STREAM_TO_FILE
#define STREAM_DUMP_FILE_PATH	DUMP_FILE_PATH

#if defined(MIBGP)
#define CONVERT_TO_ANNEX_B
#endif
 
#include "ScreenUtils.h"
#include "APSCommonServices.h"
#include "APSDebugServices.h"
#include "CFUtils.h"
#include "TickUtils.h"
#include "SettingUtils.h"
#include "ThreadUtils.h"

#include "CarPlayAppFrameworkPrivate.h"

#include <string.h>
#include <pthread.h>
#include <glib.h>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

#if defined(CONVERT_TO_ANNEX_B) || defined(WRITE_STREAM_TO_FILE)
#include "MiscUtils.h"
#endif

typedef struct ScreenStreamImp *	ScreenStreamImpRef;

//===========================================================================================================================
//	ScreenStream
//===========================================================================================================================

struct ScreenStreamImp {
	CFDataRef							avccData;				// AVCC data for the stream.
	uint32_t							widthPixels;			// Width of the screen in pixels.
	uint32_t							heightPixels;			// Height of the screen in pixels.
	uint32_t							platformLayer;

	pthread_t 							screenThread;
	dispatch_semaphore_t				doneSem;

	GstElement *						pipeline;
	GstElement * 						source;
	GMainLoop *							loop;

#if defined(CONVERT_TO_ANNEX_B) || defined(WRITE_STREAM_TO_FILE)
	uint8_t *							annexBHeaderPtr;
	size_t								annexBHeaderLen;
	size_t								nalSize;
	Boolean								firstStream;
	uint8_t *							convBuffer;
	size_t								convBufferLen;
	FILE*								outputFile;
#endif
	struct CarPlayAppFrameworkPrivate *	frameworkHandle;
	int32_t								deviceAvailable;
};

#define _ScreenStreamGetImp(STREAM)	((ScreenStreamImpRef)ScreenStreamGetContext((STREAM)))

//===========================================================================================================================
//	Logging
//===========================================================================================================================

ulog_define(CarPlayScreenGStreamer, kLogLevelNotice, kLogFlags_Default, "CarPlayScreenGStreamer", NULL);
#define ss_dlog(LEVEL, ...)	dlogc(&log_category_from_name(CarPlayScreenGStreamer), (LEVEL), __VA_ARGS__)
#define ss_ulog(LEVEL, ...)	ulog(&log_category_from_name(CarPlayScreenGStreamer), (LEVEL), __VA_ARGS__)

//===========================================================================================================================
//	Declarations
//===========================================================================================================================

OSStatus ScreenStreamInitialize(ScreenStreamRef inStream, CFDictionaryRef inOptions);
void ScreenStreamFinalize(ScreenStreamRef inStream);

static void * _ScreenStreamThread(void *inArgs);
OSStatus _CheckScreenDeviceAvailable(ScreenStreamRef inStream);

//===========================================================================================================================
//	ScreenStreamInitialize
//===========================================================================================================================

OSStatus ScreenStreamInitialize(ScreenStreamRef inStream, CFDictionaryRef inOptions)
{
	OSStatus				err = kUnknownErr;
	ScreenStreamImpRef		me = NULL;

	(void) inOptions;

	require_action(ScreenStreamGetContext(inStream) == NULL, exit, err = kAlreadyInitializedErr);

	me = (ScreenStreamImpRef)calloc(1, sizeof(*me));
	require_action(me, exit, err = kNoMemoryErr);

	// $$$ TODO: Other initialization goes here.
	// This function is called (instead of ScreenStreamCreate()) when ScreenUtils is built as a standalone shared object
	// that is loaded dynamically by AirPlay at runtime, so the initialization code should look very similar
	// to that in ScreenStreamCreate().

	ss_ulog(kLogLevelWarning, "### %s: called\n", __FUNCTION__);

	ScreenStreamSetContext(inStream, me);

	// Create Dispatch semaphore.
	me->doneSem = dispatch_semaphore_create(0);
	require_action(me->doneSem, exit, err = kUnknownErr);

	me->deviceAvailable = -1;
	if (!gst_is_initialized()) {
		gst_init(NULL, NULL);
	}
	err = kNoErr;

exit:
	return (err);
}

//===========================================================================================================================
//	ScreenStreamFinalize
//===========================================================================================================================

void ScreenStreamFinalize(ScreenStreamRef inStream)
{
	ScreenStreamImpRef const		me = _ScreenStreamGetImp(inStream);

	if (!me) return;

	// $$$ TODO: Last chance to free any resources allocated by this object.
	// This function is called (instead of _ScreenStreamFinalize()) when ScreenUtils is built as a standalone shared object
	// that is loaded dynamically by AirPlay at runtime, so the finalization code should look very similar to that in
	// _ScreenStreamFinalize().
	// It is automatically invoked, when the retain count of an ScreenStream object goes to zero.

	ss_ulog(kLogLevelWarning, "### %s: called\n", __FUNCTION__);

	if (me->doneSem) 	{dispatch_release(me->doneSem);   me->doneSem = NULL;}

	ForgetCF(&me->avccData);
	free(me);
	ScreenStreamSetContext(inStream, NULL);
}

//===========================================================================================================================
//	_ScreenStreamSetProperty
//===========================================================================================================================

OSStatus _ScreenStreamSetProperty(
		CFTypeRef		inObject,
		CFObjectFlags	inFlags,
		CFStringRef		inProperty,
		CFTypeRef		inQualifier,
		CFTypeRef		inValue)
{
	ScreenStreamImpRef const			me = _ScreenStreamGetImp((ScreenStreamRef)inObject);
	OSStatus							err;

	(void) inFlags;
	(void) inQualifier;

	ss_ulog(kLogLevelNotice, "### %s: called. inProperty = %s\n", __FUNCTION__, CFStringGetCStringPtr(inProperty, kCFStringEncodingUTF8));

	// AVCC
	if (CFEqual(inProperty, kScreenStreamProperty_AVCC)) {
		// $$$ TODO: If your video processing chain does not support AVCC, then you may need to convert this to
		// an Annex B header.  See H264ConvertAVCCtoAnnexBHeader() in MiscUtils.h.
#if defined(CONVERT_TO_ANNEX_B) || defined(WRITE_STREAM_TO_FILE)
		const uint8_t *			avccPtr;
		size_t					avccLen = 0;
#endif
		require_action(CFIsType(inValue, CFData), exit, err = kTypeErr);
		ReplaceCF(&me->avccData, inValue);
#if defined(CONVERT_TO_ANNEX_B) || defined(WRITE_STREAM_TO_FILE)
		avccLen = CFDataGetLength(me->avccData);
		require(avccLen != 0, exit);

		avccPtr = CFDataGetBytePtr(me->avccData);
		require(avccPtr != NULL, exit);

		err = H264ConvertAVCCtoAnnexBHeader( avccPtr, avccLen, NULL, 0, &me->annexBHeaderLen, NULL, NULL );
		require_noerr( err, exit );

		me->annexBHeaderPtr = (uint8_t *) malloc( me->annexBHeaderLen );
		require_action( me->annexBHeaderPtr, exit, err = kNoMemoryErr );

		err = H264ConvertAVCCtoAnnexBHeader( avccPtr, avccLen, me->annexBHeaderPtr, me->annexBHeaderLen, &me->annexBHeaderLen, &me->nalSize, NULL );
		require_noerr( err, exit );
#endif
	}
	// Pixel dimensions
	else if (CFEqual(inProperty, kScreenStreamProperty_WidthPixels)) {
		me->widthPixels = (uint32_t)CFGetInt64(inValue, &err);
		require_noerr(err, exit);
	}
	else if (CFEqual(inProperty, kScreenStreamProperty_HeightPixels)) {
		me->heightPixels = (uint32_t)CFGetInt64(inValue, &err);
		require_noerr(err, exit);
	}
	// Platform Layer
	else if (CFEqual(inProperty, kScreenStreamProperty_PlatformLayer)) {
		me->platformLayer = (uint32_t)CFGetInt64(inValue, &err);
		require_noerr(err, exit);
	}
	// Other
	else {
		ss_ulog(kLogLevelError, "### %s: Not Handled\n", __FUNCTION__);
		err = kNotHandledErr;
		goto exit;
	}
	err = kNoErr;

exit:
	return (err);
}

//===========================================================================================================================
//	ScreenStreamStart
//===========================================================================================================================

OSStatus ScreenStreamStart(ScreenStreamRef inStream)
{
	ScreenStreamImpRef const		me = _ScreenStreamGetImp(inStream);
	OSStatus						err = kUnknownErr;
	int64_t							value = 0;

	// $$$ TODO: This is where the video processing chain should be started.
	// Once this function returns, ScreenStreamProcessData() will be called continuously, providing H.264 bit-stream data
	// to be decoded and displayed.

	ss_ulog(kLogLevelNotice, "### %s: called\n", __FUNCTION__);

	// Set framework handle
	value = CarPlaySettings_GetInt64(kFrameworkHandle, &err);
	require_noerr(err, exit);

	me->frameworkHandle = (struct CarPlayAppFrameworkPrivate *)((uintptr_t)value);
	require(me->frameworkHandle != NULL, exit);
#if defined(CONVERT_TO_ANNEX_B) || defined(WRITE_STREAM_TO_FILE)
	me->firstStream = true;//please
#endif

	if (me->frameworkHandle && me->frameworkHandle->platformDelegate.screenStreamStarted_f != NULL) {
		me->frameworkHandle->platformDelegate.screenStreamStarted_f(
				me->frameworkHandle->airPlaySession,
				inStream,
				me->frameworkHandle);
	}

	err = kNoErr;

exit:
	if (err) {
		ss_ulog(kLogLevelError, "### %s: Start screen stream failed: %#m\n", __FUNCTION__, err);
		ScreenStreamStop(inStream);
	}

	return (err);
}

//===========================================================================================================================
//	ScreenStreamStop
//===========================================================================================================================

void ScreenStreamStop(ScreenStreamRef inStream)
{
	ScreenStreamImpRef const		me = _ScreenStreamGetImp(inStream);

	// $$$ TODO: This is where the video processing chain should be stopped.
	// This function is responsible for releasing any resources allocated in ScreenStreamStart().

	ss_ulog(kLogLevelNotice, "### %s: called\n", __FUNCTION__);

#if defined(WRITE_STREAM_TO_FILE)
	if (me->outputFile != NULL) {
		fclose(me->outputFile);
		me->outputFile = NULL;
	}
#endif

	if (me->loop != NULL) {
#if defined(MEEGO) || defined(MIBGP)
		// Meego want to quit main loop imediately.
		g_main_loop_quit(me->loop);
#else
		// Send signal to quit main loop
		GstFlowReturn	gstErr = GST_FLOW_ERROR;
		g_signal_emit_by_name(me->source, "end-of-stream", &gstErr);
		if (gstErr != GST_FLOW_OK ) {
			ss_ulog(kLogLevelError, "### %s: emit end-of-stream failed(gstErr = %d)\n", __FUNCTION__, gstErr);
			g_main_loop_quit(me->loop);
		}
#endif
	}

	if (me->screenThread) {
		pthread_join(me->screenThread, NULL);
		me->screenThread = 0;
	}

#if defined(CONVERT_TO_ANNEX_B) || defined(WRITE_STREAM_TO_FILE) // Fixed memory leak, mingon.chae@lgepartner.com
	if (me->convBuffer) {
		free(me->convBuffer);
		me->convBuffer = NULL;
	}

	if (me->annexBHeaderPtr) {
		free(me->annexBHeaderPtr);
		me->annexBHeaderPtr = NULL;
	}
#endif

	if (me->frameworkHandle && me->frameworkHandle->platformDelegate.screenStreamStopped_f != NULL) {
		me->frameworkHandle->platformDelegate.screenStreamStopped_f(
				me->frameworkHandle->airPlaySession,
				inStream,
				me->frameworkHandle);
	}
}

//===========================================================================================================================
//	ScreenStreamProcessData
//===========================================================================================================================

OSStatus ScreenStreamProcessData(
		ScreenStreamRef				inStream,
		uint8_t *					inData,
		size_t						inLen,
		uint64_t					inDisplayTicks,
		CFDictionaryRef				inOptions,
		ScreenStreamCompletion_f	inCompletion,
		void *						inContext)
{
	ScreenStreamImpRef const		me = _ScreenStreamGetImp(inStream);
	OSStatus						err = -1;
	GstBuffer *						buf = NULL;
	GstFlowReturn					gstErr = GST_FLOW_ERROR;

#if defined(CONVERT_TO_ANNEX_B) || defined(WRITE_STREAM_TO_FILE)
	size_t							len = 0;
	size_t							frameLen = 0;
	const uint8_t *					src;
	const uint8_t *					end;
	const uint8_t *					nalPtr;
	size_t							nalLen = 0;
	uint8_t *						dstOffset = NULL;
	uint8_t *						srcOffset = NULL;
	size_t							bufferUsed = 0;
#endif

	// $$$ TODO: Decode an H.264 frame and enqueue it for display at time inDisplayTicks.
	// After ScreenStreamStart() has been called, this function will be invoked any time there is a new H.264 frame
	// available for decode and display.
	// inDisplayTicks is the time, in units of ticks, that the provided frame is expected to be displayed on screen.
	// It is important that the video render chain honour this timestamp to ensure proper A/V sync.
	// Note that inDisplayTicks may need to be converted to another unit (like microseconds or nanoseconds) -- TickUtils.h
	// has a number of helper routines for doing this conversion.
	// The data will be provided in AVCC format.  If your video processing chain does not support AVCC, then the contents
	// of inData may need to be preprocessed here before sending it to the decoder.
	// This function currently expects the frame to be decoded (but not necessarily displayed) synchronously; that is,
	// when this function returns, there should be no expectation that the memory pointed to by inData will remain valid.

	(void) inOptions;

	if (_CheckScreenDeviceAvailable(inStream) != kNoErr) {
		if (inCompletion) inCompletion(inContext);
		return kNoErr;
	}

	require_action(inData != NULL, exit, err = kValueErr);
	require_action(inLen != 0, exit, err = kSizeErr);

	if (me->loop == NULL) {
		err = pthread_create(&me->screenThread, NULL, _ScreenStreamThread, inStream);
		require(err == 0, exit);

		dispatch_semaphore_wait(me->doneSem, DISPATCH_TIME_FOREVER);
	}

#if defined(CONVERT_TO_ANNEX_B) || defined(WRITE_STREAM_TO_FILE)
	if (inLen * 2 > me->convBufferLen) {
		size_t newBufferLen = inLen * 2;
		//ss_ulog(kLogLevelError, "### %s: inLen(%d) is bigger than expected, so increase conversion buffer(%d)\n", __FUNCTION__, inLen, newBufferLen);
     	// Fixed memory leak, mingon.chae@lgepartner.com
     	if (me->convBuffer) {
     		free(me->convBuffer);
     		me->convBuffer = NULL;
     	}
    		// ~Fixed memory leak, mingon.chae@lgepartner.com

		me->convBuffer = (uint8_t *)calloc(1, newBufferLen);
		require_action(me->convBuffer, exit, err = kNoMemoryErr);

		me->convBufferLen = newBufferLen;
	}

	dstOffset = me->convBuffer;
	srcOffset = inData;
	bufferUsed = 0;

	if (me->firstStream) {
		memcpy(dstOffset, me->annexBHeaderPtr, me->annexBHeaderLen);
		dstOffset += me->annexBHeaderLen;
		bufferUsed += me->annexBHeaderLen;

		me->firstStream = false;
	}

	for (;;) {
		uint8_t prefix[4] = { 0x00, 0x00, 0x00, 0x01 };

		if (me->nalSize == 1) {
			len = Read8(srcOffset);
		} else if (me->nalSize == 2) {
			len = ReadBig16(srcOffset);
		} else if (me->nalSize == 4) {
			len = ReadBig32(srcOffset);
		} else {
			dlogassert("Bad NAL size: %zu", me->nalSize);
			err = kSizeErr;
			goto exit;
		}

		frameLen = me->nalSize + len;

		src = srcOffset;
		end = src + frameLen;
		while (H264GetNextNALUnit(src, end, me->nalSize, &nalPtr, &nalLen, &src) == kNoErr) {
			memcpy(dstOffset, prefix, 4);
			memcpy(dstOffset + 4, nalPtr, nalLen);

			dstOffset += (4 + nalLen);
			bufferUsed += (4 + nalLen);
		}

		srcOffset += frameLen;
		if ((uintptr_t)srcOffset - (uintptr_t)inData >= inLen) {
			break;
		}
	}
#endif

#if defined(WRITE_STREAM_TO_FILE)
	if (me->outputFile == NULL) {
		char file_name[100];

		sprintf(file_name, STREAM_DUMP_FILE_PATH "carplay_%u.h264", (unsigned int)inStream);
		me->outputFile = fopen(file_name, "wb");
	}

	if (me->outputFile != NULL) {
		fwrite(me->convBuffer, 1, bufferUsed, me->outputFile);
	}
#endif

	buf = gst_buffer_new();
	require_action(buf != NULL, exit, err = kNoMemoryErr);

#if defined(CONVERT_TO_ANNEX_B)
	GST_BUFFER_SIZE(buf) = bufferUsed;
	GST_BUFFER_MALLOCDATA(buf) = GST_BUFFER_DATA(buf) = g_malloc(bufferUsed);
	uint8_t *data = GST_BUFFER_DATA(buf);
	require_action(data != NULL, exit, err = kNoMemoryErr);

	memcpy(data, me->convBuffer, bufferUsed);
#else
	GST_BUFFER_SIZE(buf) = inLen;
	GST_BUFFER_MALLOCDATA(buf) = GST_BUFFER_DATA(buf) = g_malloc(inLen);
	uint8_t *data = GST_BUFFER_DATA(buf);
	require_action(data != NULL, exit, err = kNoMemoryErr);

	memcpy(data, inData, inLen);
#endif
	// Fixed to screen stop issue.
    // int64_t timestamp = inDisplayTicks - gst_element_get_base_time((GstElement*)me->pipeline);
    // GST_BUFFER_TIMESTAMP(buf) = timestamp;

    g_signal_emit_by_name (me->source, "push-buffer", buf, &gstErr);
    require_action(gstErr >= 0, exit, err = kNotWritableErr);

	/* free the buffer */
	gst_buffer_unref (buf);

	err = kNoErr;

exit:
	if (inCompletion) inCompletion(inContext);
	if (err) {
		ss_ulog(kLogLevelError, "### %s: Screen stream process data failed: %#m\n", __FUNCTION__, err);

		if (me->frameworkHandle && me->frameworkHandle->platformDelegate.reportDeviceError_f != NULL) {
			me->frameworkHandle->platformDelegate.reportDeviceError_f(
					me->frameworkHandle->airPlaySession,
					me->frameworkHandle,
					kAirPlayStreamType_Screen,
					err);
		}
	}
	return (err);
}

//===========================================================================================================================
//	_CheckScreenDeviceAvailable
//===========================================================================================================================

OSStatus _CheckScreenDeviceAvailable(ScreenStreamRef inStream) {
	ScreenStreamImpRef const	me = _ScreenStreamGetImp(inStream);
	OSStatus					err;

//	if (me->deviceAvailable == -1) {
	if (me->deviceAvailable != 1) {
		if (me->frameworkHandle && me->frameworkHandle->platformDelegate.copyDeviceAvailable_f != NULL) {
			Boolean ret = false;
			ret = me->frameworkHandle->platformDelegate.copyDeviceAvailable_f(
					me->frameworkHandle->airPlaySession,
					me->frameworkHandle,
					kAirPlayStreamType_Screen);
			if (ret) {
				ss_ulog(kLogLevelNotice, "### %s: Screen device is available!\n", __FUNCTION__);
				me->deviceAvailable = 1;
			} else {
				ss_ulog(kLogLevelNotice, "### %s: Screen device is not available yet, so ignore this screen stream from now on\n", __FUNCTION__);
				me->deviceAvailable = 0;
			}
		} else {
			// If copyDeviceAvailable_f delegate is not assigned, it means device is always available
			me->deviceAvailable = 1;
		}
	}

	if (me->deviceAvailable == 1) {
		err = kNoErr;
	} else {
		err = kNotPreparedErr;
	}

	return err;
}

//===========================================================================================================================
//	_ScreenStreamThread
//===========================================================================================================================

static gboolean _ScreenBusCall(
		GstBus *		bus,
		GstMessage *	msg,
		gpointer 		data)
{
	gchar  *debug = NULL;
	GError *error = NULL;
	ScreenStreamImpRef const me = _ScreenStreamGetImp(data);

	(void) bus;

	switch (GST_MESSAGE_TYPE(msg)) {
	case GST_MESSAGE_EOS:
		ss_ulog(kLogLevelNotice, "### %s: End of stream\n", __FUNCTION__);
		g_main_loop_quit(me->loop);
		break;
	case GST_MESSAGE_ERROR:
		gst_message_parse_error(msg, &error, &debug);
		g_free(debug);

		ss_ulog(kLogLevelError, "### %s: Error: %s\n", __FUNCTION__, error->message);
		g_error_free(error);

		g_main_loop_quit(me->loop);
		break;
	case GST_MESSAGE_STATE_CHANGED:
		if (GST_MESSAGE_SRC (msg) == GST_OBJECT (me->pipeline)) {
			GstState old_state, new_state;

			gst_message_parse_state_changed (msg, &old_state, &new_state, NULL);

			ss_ulog(kLogLevelNotice, "Pipeline changed state from %s to %s.\n",
		        gst_element_state_get_name (old_state),
		        gst_element_state_get_name (new_state));

			if (new_state == GST_STATE_PLAYING) {
				// Assigned "Now GStreamer Playing"
				if (me->frameworkHandle && me->frameworkHandle->platformDelegate.screenStreamPlayed_f != NULL) {
					me->frameworkHandle->platformDelegate.screenStreamPlayed_f(
							me->frameworkHandle->airPlaySession,
							me,
							me->frameworkHandle);
				}
			}
		}
		break;
	default:
		break;
	}

	return (true);
}

static void * _ScreenStreamThread(void *inArgs) {
	ScreenStreamImpRef const me = _ScreenStreamGetImp(inArgs);

	GstElement *			decoder = NULL;
	GstElement *			sink = NULL;
	GstCaps *				caps = NULL;
	GstBuffer *				buf = NULL;
	GstBus *				bus = NULL;
	guint					bus_watch_id = 0;
	size_t					avccLen = 0;
	gboolean				retErr = false;
	GstStateChangeReturn	gstErr = GST_STATE_CHANGE_FAILURE;
	Boolean					signaled = false;

    SetThreadName("CarPlayScreen");
    SetCurrentThreadPriority(kAirPlayThreadPriority_ScreenReceiver);

	require(me->avccData != NULL, exit);

	// 1. Build pipeline
	me->pipeline = gst_pipeline_new("pipeline");
	require(me->pipeline != NULL, exit);

	me->source = gst_element_factory_make("appsrc", "appsrc");
	require(me->source != NULL, exit);

#if defined(MEEGO)
	decoder = gst_element_factory_make("MixVideoDecoderH264", "decoder");
	require(decoder != NULL, exit);

	sink = gst_element_factory_make("MixVideoSink", "sink");
	require(sink != NULL, exit);
#elif defined(MIBGP)
	decoder = gst_element_factory_make("vpudec", "decoder");
	require(decoder != NULL, exit);

	sink = gst_element_factory_make("mfw_v4lsink", "sink");
	require(sink != NULL, exit);
#endif

	me->loop = g_main_loop_new(NULL, FALSE);
	require(me->loop != NULL, exit);

	avccLen = CFDataGetLength(me->avccData);
	require(avccLen != 0, exit);

	buf = gst_buffer_new();
	require(buf != NULL, exit);

	GST_BUFFER_SIZE(buf) = avccLen;
	GST_BUFFER_MALLOCDATA(buf) = GST_BUFFER_DATA(buf) = g_malloc(avccLen);
	uint8_t *data = GST_BUFFER_DATA(buf);
	require(data != NULL, exit);

	memcpy(data, CFDataGetBytePtr(me->avccData), avccLen);
	
	caps = gst_caps_new_simple( "video/x-h264",
			"codec_data", GST_TYPE_BUFFER, buf,
#if defined(MIBGP)
			"stream-format", G_TYPE_STRING, "byte-stream",
			"framerate", GST_TYPE_FRACTION, me->frameworkHandle->config->maxFPS, 1,
#endif		
			"width", G_TYPE_INT, me->widthPixels,
			"height", G_TYPE_INT, me->heightPixels,
			NULL);
	require(caps != NULL, exit);

	gst_buffer_unref(buf);

	gst_app_src_set_caps(GST_APP_SRC(me->source), caps);

	g_object_set(G_OBJECT(me->source), "is-live", true,
	                                   "block", false,
	                                   "do-timestamp", false,
	                                   NULL); //latency

#if defined(MEEGO)
    ss_ulog(kLogLevelNotice, "### %s: Set Window ID(%d) to Sink\n", __FUNCTION__, me->platformLayer);
	g_object_set(G_OBJECT(sink), "WID", me->platformLayer,
											"clear-drawable", false,
											"stop-rendering", false,
											// Do not use "max-lateness" for screen stop issue.
											// "max-lateness", 500*kNanosecondsPerMillisecond,//(kNanosecondsPerSecond/me->frameworkHandle->config->maxFPS)*2,
											NULL);
#elif defined(MIBGP)
    g_object_set(G_OBJECT(decoder), "low-latency", true,
									"frame-plus", 2,
									NULL);

	ss_ulog(kLogLevelNotice, "### %s: Set Linux Monitor driver for Sink\n", __FUNCTION__);
    g_object_set(G_OBJECT(sink), "sync",  false,
                                 NULL);
#endif

	gst_bin_add_many(GST_BIN(me->pipeline), me->source, decoder, sink, NULL);
	retErr = gst_element_link_many(me->source, decoder, sink, NULL);
	require(retErr, exit);

	// 2. Set to watch pipeline event
	bus = gst_pipeline_get_bus(GST_PIPELINE(me->pipeline));
	require(bus != NULL, exit);

	bus_watch_id = gst_bus_add_watch(bus, _ScreenBusCall, inArgs);
	require(bus_watch_id != 0, exit);

	gst_object_unref(bus);

	// 3. Try to playing
	ss_ulog(kLogLevelNotice, "### %s: Trying to change state into GST_STATE_PLAYING\n", __FUNCTION__);
	gstErr = gst_element_set_state(me->pipeline, GST_STATE_PLAYING);
	require(gstErr != GST_STATE_CHANGE_FAILURE, exit);

	// We're ready to get frames
	dispatch_semaphore_signal(me->doneSem);
	signaled = true;

	ss_ulog(kLogLevelWarning, "### %s: g_main_loop_run started\n", __FUNCTION__);
	g_main_loop_run(me->loop);

//	err = kNoErr;

exit:
	ss_ulog(kLogLevelWarning, "### %s: g_main_loop_run ended\n", __FUNCTION__);
#if defined(MEEGO)
	g_object_set(G_OBJECT(sink), "clear-drawable", true,
								 "stop-rendering", true, NULL);
#elif defined(MIBGP)
    g_object_set(G_OBJECT(sink), "sync", true,
                                 "async", true, NULL);
#endif
	if (me->pipeline != NULL) {
		gstErr = gst_element_set_state(me->pipeline, GST_STATE_NULL);
		check(gstErr != GST_STATE_CHANGE_FAILURE);
		gst_object_unref(me->pipeline);
		me->pipeline = NULL;
	}

	if (me->loop != NULL) {
		g_main_loop_unref(me->loop);
		me->loop = NULL;
	}

	if (caps != NULL)
		gst_caps_unref(caps);

	if (bus_watch_id != 0) {
		retErr = g_source_remove(bus_watch_id);
		check(retErr);
	}

	if (signaled == false) {
		dispatch_semaphore_signal(me->doneSem);
	}

	pthread_exit(NULL);

    return (NULL);
}
