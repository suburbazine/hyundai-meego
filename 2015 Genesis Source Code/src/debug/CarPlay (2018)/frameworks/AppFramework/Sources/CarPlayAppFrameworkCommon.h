/*
	File:       CarPlayAppFrameworkCommon.h
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

#ifndef CARPLAYAPPFRAMEWORKCOMMON_H_
#define CARPLAYAPPFRAMEWORKCOMMON_H_

#include "AirPlayCommon.h"
#include "AirPlayReceiverServer.h"
#include "AirPlayReceiverSession.h"
#include "AirPlayUtils.h"
#include "AirPlayVersion.h"
#include "APSDebugServices.h"
#include "HIDUtils.h"
#include "ScreenUtils.h"
#include "MathUtils.h"
#include "TickUtils.h"

#define kDefaultUUID				CFSTR("e5f7a68d-7b0f-4305-984b-974f677a150b")

extern LogCategory					gLogCategory_CarPlayAppFramework;

#define app_ucat()					&log_category_from_name(CarPlayAppFramework)
#define app_ulog(LEVEL, ...)		ulog(app_ucat(), (LEVEL), __VA_ARGS__)
#define app_dlog(LEVEL, ...)		dlogc(app_ucat(), (LEVEL), __VA_ARGS__)

#endif /* CARPLAYAPPFRAMEWORKCOMMON_H_ */
