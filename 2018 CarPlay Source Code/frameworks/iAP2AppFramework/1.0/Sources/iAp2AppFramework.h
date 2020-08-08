/*
    File:       iAp2AppFramework.h
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

#ifndef IAP2APPFRAMEWORK_H_
#define IAP2APPFRAMEWORK_H_

#include <stdint.h>
#include <stddef.h>

#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkPower.h"
#include "iAp2AppFrameworkNowPlaying.h"
#include "iAp2AppFrameworkVehicleStatus.h"
#include "iAp2AppFrameworkDeviceNotification.h"
#include "iAp2AppFrameworkTelephony.h"
#include "iAp2AppFrameworkBTConnection.h"
#include "iAp2AppFrameworkLocation.h"


// ADD yongil 2014.10.23
#include "iAp2AppFrameworkHID.h"
#include "iAp2AppFrameworkMediaLibraryInformation.h"
#include "iAp2AppFrameworkExternalAccessoryProtocol.h"
#include "iAp2AppFrameworkMediaLibraryUpdates.h"
#include "iAp2AppFrameworkAssistiveTouch.h"
#include "iAp2AppFrameworkUSBDeviceModeAudio.h"
#include "iAp2AppFrameworkVoiceOver.h"
#include "iAp2AppFrameworkWiFiInformation.h"
#include "iAp2AppFrameworkCommunications.h"
#include "iAp2AppFrameworkIdentification.h"
#include "iAp2AppFrameworkFileTransfer.h"
// END Yongil
#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================================================================
//    Definitions
//===========================================================================================================================
/*!
 * @defgroup ConnectionInformation
 * @brief Group of connection information data  (This Group is used to connect with devices)
 * @{
 */
 
/*!
 * @brief This function is callback to send connection state information to Application.
 * @param inContext handle value
 * @param inCbArg Connecting state information value, @ref ConnectionStateUpdateEvt type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2AppConnectionStateUpdate_f)(void* inContext, ConnectionStateUpdateCbArg* inCbArg);

/*!
 * @brief This function is callback to send connection mode information to Application.
 * @param inContext handle value
 * @param inCbArg Connecting state information value, @ref ConnectionModeUpdateCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2AppConnectionModeUpdate_f)(void* inContext, ConnectionModeUpdateCbArg* inCbArg);

/*!
 * @brief This function is callback to send CarplaySupport information to Application.
 * @param inContext handle value
 * @param inCbArg Connecting state information value, @ref ConnectionCarplaySupportUpdateCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2AppConnectionCarPlaySupportUpdate_f)(void* inContext, ConnectionCarplaySupportUpdateCbArg* inCbArg);

/*!
 * @brief This function is callback to send Iap2Support information to Application.
 * @param inContext handle value
 * @param inCbArg Connecting state information value, @ref ConnectionIap2SupportUpdateCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2AppConnectionIap2SupportUpdate_f)(void* inContext, ConnectionIap2SupportUpdateCbArg* inCbArg);

#ifdef ANDROID

/*!
 * @brief This function is callback to send Iap2Device information to Application.
 * @param inContext handle value
 * @param inCbArg Connecting state information value, @ref ConnectionIap2DeviceInfoUpdateCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2AppDeviceInfoUpdate_f)(void* inContext, ConnectionIap2DeviceInfoUpdateCbArg* inCbArg);
#endif

/*!
 * @brief This function is callback to send StateAndPathUpdate information to Application.
 * @param inContext handle value
 * @param inCbArg StateAndPathUpdate information value, @ref ConnectionStateAndPathUpdateCbArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2AppConnectionStateAndPathUpdate_f)(void* inContext, ConnectionStateAndPathUpdateCbArg* inCbArg);

/*!
 * @brief This function is callback to send connection information to Application.
 * @param inContext handle value
 * @param inCbArg all connection information value, @ref ConnectionInfoUpdateCbArg* type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2AppConnectionInfoUpdate_f)(void* inContext, ConnectionInfoUpdateCbArg* inCbArg );

/*!
 * @brief This function is callback to send socketClose State to Application.
 * @param inContext handle value 
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2SocketClose_f)(void* inContext);

/*!
 * @brief This function is callback to send AuthenticationFail State to Application.
 * @param inContext handle value 
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2AuthenticationFailed_f)(void* inContext);

/*!
 * @brief This function is callback to send IdentificationFail to Application.
 * @param inContext handle value 
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
typedef int (*iAp2IdentificationFailed_f)(void* inContext);



/*!
 * @struct iAp2AppDelegate
 * @brief This struct has function to receive event from apple device. Each function needs registration using callback.
 * @{
 */
typedef struct {
    void*                                        context;
    iAp2AppConnectionStateUpdate_f                    connStateUpdate_f;                                 /**< This function is callback function called when connection state changes */
    iAp2AppConnectionStateAndPathUpdate_f            connStateAndPathUpdate_f;                        /**< This function is callback function called when StateAndPath changes */
    iAp2AppConnectionModeUpdate_f                    connModeUpdate_f;                                /**< This function is callback function called when connection mode changes */
    iAp2AppConnectionCarPlaySupportUpdate_f        carplaySupportUpdate_f;                            /**< This function is callback function called when carplaysupport state changes */
    iAp2AppConnectionIap2SupportUpdate_f            iap2SupportUpdate_f;                            /**< This function is callback function called when iAP2support state changes */
    #ifdef ANDROID
    iAp2AppDeviceInfoUpdate_f                        iap2DeviceInfoUpdate_f;                            /**< This function is callback function called when iap2DeviceInfo changes */
    #endif

    iAp2AppConnectionInfoUpdate_f                    connInfoUpdate_f;                                /**< This function is callback function called when ConnectionInfo changes */
    iAp2AppPowerUpdate_f                            powerUpdate_f;                                    /**< This function is callback function called when iap2Servcie receives powerUpdate message */
    iAp2AppNowPlayingUpdate_f                        nowPlayingUpdate_f;                                /**< This function is callback function called when iap2Servcie receives nowPlayingUpdate message */        
    iAp2AppStartVehicleStatusUpdate_f                startVehicleStatus_f;                            /**< This function is callback function called when iap2Servcie receives startVehicleStatus message */    
    iAp2AppStopVehicleStatusUpdate_f                stopVehicleStatus_f;                            /**< This function is callback function called when iap2Servcie receives stopVehicleStatus message */    
    iAp2AppDeviceInformationUpdate_f                deviceInformationUpdate_f;                        /**< This function is callback function called when iap2Servcie receives deviceInformationUpdate message */    
    iAp2App2DeviceLanguageUpdate_f                deviceLanguageUpdate_f;                        /**< This function is callback function called when iap2Servcie receives deviceLanguageUpdate message */    
    iAp2AppTelephonyCallStateInformation_f            telephonyCallStateInformation_f;                    /**< This function is callback function called when iap2Servcie receives telephonyCallStateInformation message */    
    iAp2AppTelephonyUpdate_f                        telephonyUpdate_f;                                /**< This function is callback function called when iap2Servcie receives telephonyUpdate message */    
    iAp2AppBTConnectionUpdate_f                    btConnectionUpdate_f;                            /**< This function is callback function called when iap2Servcie receives btConnectionUpdate message */    
    iAp2AppStartLocationInformation_f                startLocationInformation_f;                        /**< This function is callback function called when iap2Servcie receives startLocationInformation message */    
    iAp2AppStopLocationInformation_f                stopLocationInformation_f;                        /**< This function is callback function called when iap2Servcie receives stopLocationInformation message */    
    iAP2DeviceHIDReport_f                            deviceHIDReport_f;                                /**< This function is callback function called when iap2Servcie receives deviceHIDReport message */    
    iAp2MeidaLibraryInformation_f                    mediaLibraryInformation_f;                        /**< This function is callback function called when iap2Servcie receives mediaLibraryInformation message */    
    iAp2MediaLibraryUpdates_f                        mediaLibraryUpdates_f;                            /**< This function is callback function called when iap2Servcie receives mediaLibraryUpdates message */        
    iAp2AssistiveTouchInformation_f                    assistiveTouchInformation_f;                        /**< This function is callback function called when iap2Servcie receives assistiveTouchInformation message */    
    iAp2StartExternalAccessoryProtocolSession_f        StartExternalAccessoryProtocolSession_f;           /**< This function is callback function called when iap2Servcie receives StartExternalAccessoryProtocolSession message */     
        iAP2StopExternalAccessoryProtocolSession_f        StopExternalAccessoryProtocolSession_f;            /**< This function is callback function called when iap2Servcie receives StopExternalAccessoryProtocolSession message */        
    iAP2ExternalAccessoryProtocolData_f                ExternalAccessoryProtocolData_f;                /**< This function is callback function called when iap2Servcie receives ExternalAccessoryProtocolData_f message */        
    iAp2USBDeviceMdoeAudioInformation_f            USBDeviceMdoeAudioInformation_f;                /**< This function is callback function called when iap2Servcie receives USBDeviceMdoeAudioInformation message */    
    iAp2VoiceOverUpdates_f                        VoiceOverUpdates_f;                                /**< This function is callback function called when iap2Servcie receives VoiceOverUpdates message */                    
    iAp2VoiceOverCursorUpdates_f                    VoiceOverCursorUpdates_f;                        /**< This function is callback function called when iap2Servcie receives VoiceOverCursorUpdates message */    
    iAp2WiFiInformation_f                             WiFiInformation_f;                                /**< This function is callback function called when iap2Servcie receives WiFiInformation message */    
    iAp2AppFileTransferUpdate_f                        FileTransferUpdate_f;                            /**< This function is callback function called when iap2Servcie receives AlbumArtwork data */    
    iAp2CallStateUpdates_f                             CallStateUpdates_f;                                /**< This function is callback function called when iap2Servcie receives CallStateUpdates message */    
    iAp2CommunicationsUpdates_f                     CommunicationsUpdates_f;                        /**< This function is callback function called when iap2Servcie receives CommunicationsUpdates message */    
    iAp2ListUpdate_f                                 ListUpdate_f;                                    /**< This function is callback function called when iap2Servcie receives ListUpdate message */    
    iAp2SocketClose_f                                 SocketClose_f;                                    /**< This function is callback function called when socket is close */    
    iAp2AuthenticationFailed_f                         AuthenticationFailed_f;                            /**< This function is callback function called when Authentication is failed */    
    iAp2IdentificationFailed_f                        IdentificationFailed_f;                            /**< This function is callback function called when Identification is failed */    
} iAp2AppDelegate;
/*! @} */

/*!
 * @struct iAp2AppConfig
 * @brief This struct has configuring value about iAP2 Session 
 * @{
 */
typedef struct {
    BOOL bUseControlSession;                /**< Configure using ControllSession in the iAP2 Session */
    BOOL bUseFileTransferSession;            /**< Configure using FileTransferSession in the iAP2 Session */
    BOOL bUseExternalAccessorySession;        /**< Configure using ExternalAccessorySession in the iAP2 Session */
} iAp2AppConfig;
/*! @} */

//===========================================================================================================================
//    Declarations
//===========================================================================================================================
/*!
 * @def iAp2AppDelegateInit
 * @brief @ref iAp2AppDelegate struct value initialization
 * @param PTR iAp2AppDelegate struct value
 */
#define iAp2AppDelegateInit(PTR) memset((PTR), 0, sizeof(iAp2AppDelegate));

/*!
 * @def iAp2AppConfigInit
 * @brief @ref iAp2AppConfig struct value initialization
 * @param PTR iAp2AppConfig struct value
 */
#define iAp2AppConfigInit(PTR) memset((PTR), 0, sizeof(iAp2AppConfig));

//===========================================================================================================================
//    Life cycle
//===========================================================================================================================
/*!
 * @brief     Create handle, initialize handle and return handle
 * @param     outRef created handle vaule, @ref iAp2AppRef stuct value
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppInitialize(iAp2AppRef *outRef);

/*!
 * @brief     Set config value of handle
 * @param     inRef Application handle value
 * @param     inConfig Session configuration value of Application, inConfig(@ref iAp2AppConfig) is set by application.
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppSetConfig(iAp2AppRef inRef, const iAp2AppConfig *inConfig);

/*!
 * @brief     Set delegate value of handle
 * @param     inRef Application handle value
 * @param     inDelegate Callback functions are registered in this param, inDelegate(@ref iAp2AppDelegate) is set by application.
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppSetDelegate(iAp2AppRef inRef, const iAp2AppDelegate *inDelegate);

/*!
 * @brief     Connect to iap2service using socket and run eventHanlde Tread
 * @param     inRef Application handle value  
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStart(iAp2AppRef inRef);

/*!
 * @brief     Disconnect to iap2service and stop eventHanlde Tread
 * @param     inRef Application handle value  
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStop(iAp2AppRef inRef);

/*!
 * @brief     handle free
 * @param     inRef Application handle value  
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppTerminate(iAp2AppRef inRef);

/*!
 * @brief     Get current state information of iAP2Service
 * @param     inRef Application handle value  
 * @param     outConnState current state information, Refer @ref ConnectionState struct 
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppGetConnectionState(iAp2AppRef inRef, ConnectionState* outConnState);

/*!
 * @brief     Get current connection path information of iAP2Service
 * @param     inRef Application handle value  
 * @param     outConnPath current connection path information, Refer @ref ConnectionPath struct 
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppGetConnectionPath(iAp2AppRef inRef, ConnectionPath* outConnPath);

/*!
 * @brief     Get current mode information of iAP2Service
 * @param     inRef Application handle value  
 * @param     outConnMode current mode information, Refer @ref ConnectionMode struct 
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppGetConnectionMode(iAp2AppRef inRef, ConnectionMode* outConnMode);

/*!
 * @brief     Get current CarplaySupport information of iAP2Service
 * @param     inRef Application handle value  
 * @param     outCarplaySupport current CarplaySupport information, Refer @ref ConnectionCarPlaySupport struct 
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppGetCarplaySupport(iAp2AppRef inRef, ConnectionCarPlaySupport* outCarplaySupport);

/*!
 * @brief     Get current Iap2Support information of iAP2Service
 * @param     inRef Application handle value  
 * @param     outIap2Support current Iap2Support information, Refer @ref ConnectionIap2Support struct 
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppGetIap2Support(iAp2AppRef inRef, ConnectionIap2Support* outIap2Support);

/*!
 * @brief     Set On or Off about Carplay function.
 * @param     inRef Application handle value  
 * @param     CarplayOnOff Carplay On/Off Value in the iAP2 service
             \n If Carplay is support then set TRUE, If Carplay is not support then set FALSE. 
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppCarplayOnOff(iAp2AppRef inRef, BOOL CarplayOnOff);


/*!
 * @brief     Change USB mode by application.
 * @param     inRef Application handle value  
 * @param     RequestUSBMode This value is used to set USB Mode, Refer @ref IdentficationSelectArg struct
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppUSBModeChange(iAp2AppRef inRef, ConnectionMode RequestUSBMode);


/*!
 * @brief     Set CarPlay client State by application.
 * @param     inRef Application handle value  
 * @param     mCarPlayClientState This value is used to set CarPlay client state, Refer @ref CarPlayClientState enum
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppCarPlayClientState(iAp2AppRef inRef, CarPlayClientState mCarPlayClientState);


/*!    @fn            iAp2AppGetInitConnectionData
    @brief        Request current Connection information    
    @param        inRef                        (Input) Hanlde Value        
    @return        ApplicationMode     
    @discussion
*/
int iAp2AppGetInitConnectionData(iAp2AppRef inRef);




/*! @fn        iAp2AppStartCarPlay
 * @brief         Start CarPlay by application.
 * @param     inRef Application handle value  
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppStartCarPlay(iAp2AppRef inRef);

/*!
 * @brief     TerminateDaemon by application. (Android : iAP2Daemon, Linux : iAP2Service)
 * @param     inRef Application handle value
 * @return      0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
int iAp2AppTerminateDaemon(iAp2AppRef inRef);

/*! @}  */



int iAp2ConectionHandleEventConnectionInfoUpdate(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleEventConnectionStateUpdate(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleEventConnectionStateAndPathUpdate(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleEventConnectionModeUpdate(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleEventCarplaySupportUpdate(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleEventIap2SupportUpdate(iAp2AppRef inRef, unsigned char* pRxData);
#ifdef ANDROID
int iAp2ConectionHandleEventDeviceInfoUpdateUpdate(iAp2AppRef inRef, unsigned char* pRxData);
#endif
int iAp2ConectionHandleSocketClose(iAp2AppRef inRef);
int iAp2ConectionHandleEventAuthenticaitonFailed(iAp2AppRef inRef, unsigned char* pRxData);
int iAp2ConectionHandleEventIdentificationFailed(iAp2AppRef inRef, unsigned char* pRxData);


    
#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORK_H_ */
