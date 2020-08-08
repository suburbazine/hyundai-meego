/*
    File:       iAp2AppFrameworkPower.h
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

#ifndef IAP2APPFRAMEWORKPOWER_H_
#define IAP2APPFRAMEWORKPOWER_H_

#include <stdint.h>
#include <stddef.h>

#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkExternal.h"
#include "iAp2AppFrameworkExternal_command.h"
#include "iAp2AppFrameworkExternal_event.h"

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================================================================
//    Definitions
//===========================================================================================================================
/*!
 * @defgroup Power
 * @brief Group about Power in the iAP2 Message 
 * @{
 */
 
//#define _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_
/*!
 * @brief This function is callback to send PowerUpdate data to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of PowerUpdate Message, Refer @ref PowerUpdateCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2AppPowerUpdate_f)(void* inContext, PowerUpdateCbArg* inCbArg);


//===========================================================================================================================
//    Declarations
//===========================================================================================================================
/*!
 * @brief     Send StartPowerUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of StartPowerUpdates, Refer @ref StartPowerUpdateArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStartPowerUpdates(iAp2AppRef inRef, StartPowerUpdateArg* inArg);

/*!
 * @brief     Send StopPowerUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStopPowerUpdates(iAp2AppRef inRef);

/*!
 * @brief     Send PowerSourceUpdate message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of PowerSourceUpdate, Refer @ref PowerSourceUpdateArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppPowerSourceUpdate(iAp2AppRef inRef, PowerSourceUpdateArg* inArg);
/*! @} */


int iAp2ConectionHandleEventPowerUpdate(iAp2AppRef inRef, unsigned char* pRxData);
#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKPOWER_H_ */
