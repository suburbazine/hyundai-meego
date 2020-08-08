/*
    File:       iAp2ExternelLogUtils.h
    Package:    LG iAP2 Application Framework
    Author:     Yongil Park (yongil.park@lge.com)
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

#ifndef _IAP2EXTERNELLOGUTILS_H_
#define _IAP2EXTERNELLOGUTILS_H_
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include "iAp2Log.h"

typedef int32_t OSStatus;
typedef int32_t LogLevel;
void iAp2ExternelLogWriteAddQueue(LogLevel inLevel, const char *inStr, int inLen);
OSStatus iAp2ExternelLogWriteInit(void);
void iAp2ExternelLogWriteStop(void);

#endif /* _IAP2EXTERNELLOGUTILS_H_ */
