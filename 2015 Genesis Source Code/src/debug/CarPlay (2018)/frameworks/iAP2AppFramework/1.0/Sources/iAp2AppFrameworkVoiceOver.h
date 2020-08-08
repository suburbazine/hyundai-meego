/*
    File:       iAp2AppFrameworkVoiceOver.h
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

#ifndef IAP2APPFRAMEWORKVOICEOVER_H_
#define IAP2APPFRAMEWORKVOICEOVER_H_

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
 * @defgroup VoiceOver
 * @brief Group about VoiceOver in the iAP2 Message
 * @{
 */
 
/*!
 * @brief This function is callback to send VoiceOverUpdates data to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of VoiceOverUpdates Message, Refer @ref VoiceOverUpdatesCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2VoiceOverUpdates_f)(void* inContext, VoiceOverUpdatesCbArg* inCbArg);

/*!
 * @brief This function is callback to send VoiceOverCursorUpdates data to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of VoiceOverCursorUpdates Message, Refer @ref VoiceOverCursorUpdatesCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2VoiceOverCursorUpdates_f)(void* inContext, VoiceOverCursorUpdatesCbArg* inCbArg);

//===========================================================================================================================
//    Declarations
//===========================================================================================================================
/*!
 * @brief     Send StartVoiceOver message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppStartVoiceOver(iAp2AppRef inRef);

/*!
 * @brief     Send StopVoiceOver message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppStopVoiceOver(iAp2AppRef inRef);

/*!
 * @brief     Send RequestVoiceOverMoveCursor message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of RequestVoiceOverMoveCursor, Refer @ref RequestVoiceOverMoveCursorArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppRequestVoiceOverMoveCursor(iAp2AppRef inRef, RequestVoiceOverMoveCursorArg* inArg);

/*!
 * @brief     Send RequestVoicOverActivateCursor message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppRequestVoicOverActivateCursor(iAp2AppRef inRef);

/*!
 * @brief     Send RequestVoiceOverScrollPage message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of RequestVoiceOverScrollPage, Refer @ref RequestVoiceOverScrollPageArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppRequestVoiceOverScrollPage(iAp2AppRef inRef, RequestVoiceOverScrollPageArg* inArg);

/*!
 * @brief     Send RequestVoiceOverSpeakText message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of RequestVoiceOverSpeakText, Refer @ref RequestVoiceOverSpeakTextArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppRequestVoiceOverSpeakText(iAp2AppRef inRef, RequestVoiceOverSpeakTextArg* inArg);

/*!
 * @brief     Send RequestVoicOverPauseText message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppRequestVoicOverPauseText(iAp2AppRef inRef);

/*!
 * @brief     Send RequestVoicOverResumeText message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppRequestVoicOverResumeText(iAp2AppRef inRef);

/*!
 * @brief     Send StartVoiceOverUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of StartVoiceOverUpdates, Refer @ref StartVoiceOverUpdatesArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppStartVoiceOverUpdates(iAp2AppRef inRef, StartVoiceOverUpdatesArg* inArg);

/*!
 * @brief     Send StopVoiceOverUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppStopVoiceOverUpdates(iAp2AppRef inRef);

/*!
 * @brief     Send RequestVoiceOverConfiguration message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of RequestVoiceOverConfiguration, Refer @ref RequestVoiceOverConfigurationArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppRequestVoiceOverConfiguration(iAp2AppRef inRef, RequestVoiceOverConfigurationArg* inArg);

/*!
 * @brief     Send StartVoiceOverCursorUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of StartVoiceOverCursorUpdates, Refer @ref StartVoiceOverCursorUpdatesArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppStartVoiceOverCursorUpdates(iAp2AppRef inRef, StartVoiceOverCursorUpdatesArg* inArg);

/*!
 * @brief     Send StopVoiceOverCursorUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAP2AppStopVoiceOverCursorUpdates(iAp2AppRef inRef);
/*! @} */


int iAp2ConectionHandleEventVoiceOverUpdate(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleEventVoiceOverCursorUpdate(iAp2AppRef inRef, unsigned char* pRxData);

#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKVOICEOVER_H_ */

