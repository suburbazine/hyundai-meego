/*
    File:       iAp2AppFrameworkMediaLibraryUpdates.h
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

#ifndef IAP2APPFRAMEWORKMEDIALIBRARYUPDATES_H_
#define IAP2APPFRAMEWORKMEDIALIBRARYUPDATES_H_

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
 * @defgroup MediaLibararyAccess
 * @brief Group about Media Libarary Access in the iAP2 Message 
 * @{
 */
/*!
 * @brief This function is callback to send MediaLibraryUpdates message to Application.
 * @param inContext handle value
 * @param inCbArg Payload Data of MediaLibraryUpdates Message, Refer @ref MediaLibraryUpdatesCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2MediaLibraryUpdates_f)(void* inContext, MediaLibraryUpdatesCbArg* inCbArg);


//===========================================================================================================================
//    Declarations
//===========================================================================================================================
/*!
 * @brief     Send StartMediaLibraryUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of StartMediaLibraryUpdates, Refer @ref StartMediaLibraryUpdatesArg type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStartMediaLibraryUpdates(iAp2AppRef inRef, StartMediaLibraryUpdatesArg* inArg);

/*!
 * @brief     Send StopMediaLibraryUpdates message to Apple Device.
 * @param     inRef Application handle value  
 * @param   inArg Payload Data of StopMediaLibraryUpdates, Refer @ref StopMediaLibraryUpdates type
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStopMediaLibraryUpdates(iAp2AppRef inRef, StopMediaLibraryUpdatesArg* inArg);
/*!
 * @brief     Free mediaLibraryUpdatesCbArg struct address
 * @param   inArg Address of MediaLibraryUpdatesCbArg struct.
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppMediaLibraryUpdatesFree(MediaLibraryUpdatesCbArg* inArg);
/*! @} */

int iAp2ConectionHandleEventMediaLibraryUpdates(iAp2AppRef inRef, unsigned char* pRxData);
void ParsingMediaLibraryUpdatesEventMediaItemGroup(MarshalIo* iAp2Marshal, MediaItemGroup* pMediaItemGroup);
void ParsingMediaLibraryUpdatesEventMediaPlaylistGroup(MarshalIo* iAp2Marshal, MediaPlaylistGroup* pMediaPlaylistGroup);
#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKMEDIALIBRARYUPDATES_H_ */
