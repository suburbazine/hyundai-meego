/*
    File:       iAp2AppFrameworkPrivate.h
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

#ifndef IAP2APPFRAMEWORKPRIVATE_H_
#define IAP2APPFRAMEWORKPRIVATE_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================================================================
//    Definitions
//===========================================================================================================================
#include "iAp2AppFramework.h"

#include <pthread.h>

/*!
 * @struct     iAp2AppPrivate
 * @brief     This Struct is used to save handle data.
 */ 
struct iAp2AppPrivate {
    pthread_t iAp2EvtHandleThread;                 /**< iAp2EvtHandleThread of pthread_t type is used to save iAp2EvtHandleThread data */
    int mTid;                                    /**< mTid of int type is used to save mTid data */
    BOOL mIsTerminated;                            /**< mIsTerminated of BOOL type is used to save EventHandler Thread Terminated data */

    iAp2AppConfig* config;                        /**< config of iAp2AppConfig* type is used to save @ref iAp2AppConfig structure data */
    iAp2AppDelegate* delegate;                    /**< delegate of iAp2AppDelegate* type is used to save @ref iAp2AppDelegate structure data */

    int sockFd;                                    /**< sockFd of int is used to save socket information data*/
    ConnectionState connState;                    /**< connState of @ref ConnectionState type is used to save current state data*/
    ConnectionPath connPath;                    /**< connPath of @ref ConnectionPath type is used to save current path data*/
    ConnectionMode connMode;                    /**< connMode of @ref ConnectionMode type is used to save current mode data*/
    ConnectionCarPlaySupport carplaySupport;    /**< carplaySupport of @ref ConnectionCarPlaySupport type is used to save current CarplaySupport data*/
    ConnectionIap2Support iap2Support;            /**< connState of @ref ConnectionIap2Support type is used to save current state data*/
    #ifdef ANDROID
    UsbDeviceInfo iapDeviceInfo;                /**< iapDeviceInfo of @ref UsbDeviceInfo type is used to save current deviceInformation data*/
    #endif
};

//===========================================================================================================================
//    Declarations
//===========================================================================================================================

#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKPRIVATE_H_ */

