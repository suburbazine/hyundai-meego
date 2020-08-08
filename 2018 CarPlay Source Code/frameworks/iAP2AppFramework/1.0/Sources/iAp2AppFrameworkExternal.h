/*
    File:       iAp2AppFrameworkExternal.h
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

#ifndef IAP2APPFRAMEWORKEXTERNAL_H_
#define IAP2APPFRAMEWORKEXTERNAL_H_

#include <stdint.h>
#include <stddef.h>

#include "iAp2AppFrameworkCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================================================================
//    Definitions
//===========================================================================================================================
#ifdef ANDROID
#define SOCKET_FILE_PATH RW_FILE_PATH "/iAp2Socket"
#elif (defined(MFI_AUTH_DEVICE_IAP2SOCKETPATH))
#define SOCKET_FILE_PATH MFI_AUTH_DEVICE_IAP2SOCKETPATH
#else
#define SOCKET_FILE_PATH "/tmp/iAp2Socket"
#endif

/*!
 * @struct     CommandHeader
 * @brief     This Struct is used to save CommandHeader data.
 */ 
typedef struct {
    uint32_t cmd;            /**< cmd of uint32_t type is used to save command */
    uint32_t appHandle;        /**< appHandle of uint32_t type is used to save appHandle */    
    uint32_t dataSize;        /**< dataSize of uint32_t type is used to save Message data size */        
} CommandHeader;

/*!
 * @struct     EventHeader
 * @brief     This Struct is used to save EventHeader data.
 */ 
typedef struct {
    uint32_t evt;            /**< evt of uint32_t type is used to save Event */
    uint32_t appHandle;        /**< appHandle of uint32_t type is used to save appHandle */
    uint32_t dataSize;        /**< dataSize of uint32_t type is used to save  Message data size */
} EventHeader;


// END Yongil
#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKEXTERNAL_H_ */

