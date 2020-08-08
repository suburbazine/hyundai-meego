/*
    File:       iAp2AppFrameworkExternalAccessoryProtocol.h
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

#ifndef IAP2APPFRAMEWORKEXTERNALACCESSORYPROTOCOL_H_
#define IAP2APPFRAMEWORKEXTERNALACCESSORYPROTOCOL_H_

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
 * @defgroup ExternalAccessoryProtocol
 * @brief Group about ExternalAccessoryProtocol in the iAP2 Message 
 * @{
 */
/*!
 * @brief This function is callback to send StartExternalAccessoryProtocolSession message to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of StartExternalAccessoryProtocolSession Message, Refer @ref StartExternalAccessoryProtocolSessionCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2StartExternalAccessoryProtocolSession_f)(void* inContext, StartExternalAccessoryProtocolSessionCbArg* inCbArg);

/*!
 * @brief This function is callback to send StopExternalAccessoryProtocolSession message to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of StopExternalAccessoryProtocolSession Message, Refer @ref StopExternalAccessoryProtocolSessionCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAP2StopExternalAccessoryProtocolSession_f)(void* inContext, StopExternalAccessoryProtocolSessionCbArg* inCbArg);

/*!
 * @brief This function is callback to send ExternalAccessoryProtocolData to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of  ExternalAccessoryProtocolData Message, Refer @ref ExternalAccessoryProtocolDataCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAP2ExternalAccessoryProtocolData_f)(void* inContext, ExternalAccessoryProtocolDataCbArg* inCbArg);

//===========================================================================================================================
//    Declarations
//===========================================================================================================================
/*!
 * @brief     Send StatusExternalAccessoryProtocolSession message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of StatusExternalAccessoryProtocolSession, Refer @ref StatusExternalAccessoryProtocolSessionArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStatusExternalAccessoryProtocolSession(iAp2AppRef inRef, StatusExternalAccessoryProtocolSessionArg* inArg);

/*!
 * @brief     Send ExternalAccessoryProtocol Data to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Data of ExternalAccessoryProtocol, Refer @ref ExternalAccessoryProtocolDataArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppExternalAccessoryProtocolData(iAp2AppRef inRef, ExternalAccessoryProtocolDataArg* inArg);
/*! @} */

int iAp2ConectionHandleEventStartExternalAccessoryProtocolSession(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleEventStopExternalAccessoryProtocolSession(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleExternalAccessoryProtocolData(iAp2AppRef inRef, unsigned char* pRxData);
#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKVEHICLESTATUS_H_ */

