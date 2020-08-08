/*
    File:       iAp2AppFrameworkCommunications.h
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

#ifndef IAP2APPFRAMEWORKCOMMUNICATIONS_H_
#define IAP2APPFRAMEWORKCOMMUNICATIONS_H_

#include <stdint.h>
#include <stddef.h>

#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkExternal.h"
#include "iAp2AppFrameworkExternal_command.h"
#include "iAp2AppFrameworkExternal_event.h"
#include "iAp2Marshal.h"

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================================================================
//    Definitions
//===========================================================================================================================

/*!
 * @defgroup Communications
 * @brief Group about Communication in the iAP2 Message 
 * @{
 */
 
/*!
 * @brief This function is callback to send CallStateUpdates data to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of CallStateUpdates Message, Refer @ref CallStateUpdatesCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2CallStateUpdates_f)(void* inContext, CallStateUpdatesCBArg* inCbArg);

//===========================================================================================================================
//    Definitions
//===========================================================================================================================
/*!
 * @brief This function is callback to send CommunicationsUpdates data to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of CommunicationsUpdates Message, Refer @ref CommunicationsUpdatesCBArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2CommunicationsUpdates_f)(void* inContext, CommunicationsUpdatesCBArg* inCbArg);

//===========================================================================================================================
//    Definitions
//===========================================================================================================================
/*!
 * @brief This function is callback to send ListUpdate data to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of ListUpdate Message, Refer @ref ListUpdateCBArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2ListUpdate_f)(void* inContext, ListUpdateCBArg* inCbArg);




//===========================================================================================================================
//    Declarations
//===========================================================================================================================
/*!
 * @brief     Send StartCallStateUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of StartCallStateUpdates, Refer @ref StartCallStateUpdatesArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStartCallStateUpdates(iAp2AppRef inRef, StartCallStateUpdatesArg* inArg);


/*!
 * @brief     Send StopCallStateUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStopCallStateUpdates(iAp2AppRef inRef);

/*!
 * @brief     Send StartCommunicationsUpdate message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of StartCommunicationsUpdate, Refer @ref StartCommunicationsUpdatesArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStartCommunicationsUpdate(iAp2AppRef inRef, StartCommunicationsUpdatesArg* inArg);

/*!
 * @brief     Send StopCommunicationsUpdate message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStopCommunicationsUpdate(iAp2AppRef inRef);


/*!
 * @brief     Send InitiateCall message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of InitiateCall, Refer @ref InitiateCallArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppInitiateCall(iAp2AppRef inRef, InitiateCallArg* inArg);

/*!
 * @brief     Send iAp2AppAcceptCall message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of AcceptCall, Refer @ref AcceptCallArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppAcceptCall(iAp2AppRef inRef, AcceptCallArg* inArg);

/*!
 * @brief     Send iAp2AppEndCall message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of EndCall, Refer @ref EndCallArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppEndCall(iAp2AppRef inRef, EndCallArg* inArg);

/*!
 * @brief     Send SwapCalls message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppSwapCalls(iAp2AppRef inRef);

/*!
 * @brief     Send MergeCalls message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppMergeCalls(iAp2AppRef inRef);


/*!
 * @brief     Send HoldStatusUpdate message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of HoldStatusUpdate, Refer @ref HoldStatusUpdateArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppHoldStatusUpdate(iAp2AppRef inRef, HoldStatusUpdateArg* inArg);

/*!
 * @brief     Send MuteStatusUpdate message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of MuteStatusUpdate, Refer @ref MuteStatusUpdateArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppMuteStatusUpdate(iAp2AppRef inRef, MuteStatusUpdateArg* inArg);

/*!
 * @brief     Send SendDTMF message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of SendDTMF, Refer @ref SendDTMFArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppSendDTMF(iAp2AppRef inRef, SendDTMFArg* inArg);

/*!
 * @brief     Send StartListUpdate message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of StartListUpdate, Refer @ref StartListUpdatesArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStartListUpdate(iAp2AppRef inRef, StartListUpdatesArg* inArg);

/*!
 * @brief     Send StopListUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStopListUpdates(iAp2AppRef inRef);
/*! @} */
    
int iAp2ConectionHandleEventCallStateUpdates(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleEventCommunicationsUpdates(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleEventListUpdate(iAp2AppRef inRef, unsigned char* pRxData);
    
void ParsingListUpdateEventRecentListGroup(MarshalIo* iAp2Marshal, RecentListGroup* pRecentListGroup);
void ParsingListUpdateEventFavoritesListGroup(MarshalIo* iAp2Marshal, FavoritesListGroup* pFavoritesListGroup);

#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKCOMMUNICATIONS_H_ */


