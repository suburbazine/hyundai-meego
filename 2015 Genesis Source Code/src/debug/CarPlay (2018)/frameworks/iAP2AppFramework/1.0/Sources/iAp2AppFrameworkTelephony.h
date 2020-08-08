/*
    File:       iAp2AppFrameworkTelephony.h
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

#ifndef IAP2APPFRAMEWORKTELEPHONY_H_
#define IAP2APPFRAMEWORKTELEPHONY_H_

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
 * @defgroup Telephony
 * @brief Group about Telephony in the iAP2 Message \n
           (Refer Telephony Part MFi Accessory Interface Sepciification R18)
 * @{
 */
 
/*!
 * @brief This function is callback to send TelephonyCallStateInformation data to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of TelephonyCallStateInformation Message, Refer @ref TelephonyCallStateInformationCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2AppTelephonyCallStateInformation_f)(void* inContext, TelephonyCallStateInformationCbArg* inCbArg);

/*!
 * @brief This function is callback to send TelephonyUpdate data to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of TelephonyUpdate Message, Refer @ref TelephonyUpdateCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2AppTelephonyUpdate_f)(void* inContext, TelephonyUpdateCbArg* inCbArg);


//===========================================================================================================================
//    Declarations
//===========================================================================================================================
/*!
 * @brief     Send StartTelephonyCallStateInformation message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of StartTelephonyCallStateInformation, Refer @ref StartTelephonyCallStateInformationArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStartTelephonyCallStateInformation(iAp2AppRef inRef, StartTelephonyCallStateInformationArg* inArg);

/*!
 * @brief     Send StopTelephonyCallStateInformation message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStopTelephonyCallStateInformation(iAp2AppRef inRef);

/*!
 * @brief     Send StartTelephonyUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of StartTelephonyUpdates, Refer @ref StartTelephonyUpdatesArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStartTelephonyUpdates(iAp2AppRef inRef, StartTelephonyUpdatesArg* inArg);

/*!
 * @brief     Send StopTelephonyUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStopTelephonyUpdates(iAp2AppRef inRef);
/*! @} */


int iAp2ConectionHandleEventTelephonyCallStateInformation(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleEventTelephonyUpdate(iAp2AppRef inRef, unsigned char* pRxData);
#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKTELEPHONY_H_ */


