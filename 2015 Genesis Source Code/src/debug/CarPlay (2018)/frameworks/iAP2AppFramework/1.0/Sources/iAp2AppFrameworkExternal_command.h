/*
    File:       iAp2AppFrameworkExternal_command.h
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

#ifndef IAP2APPFRAMEWORKEXTERNAL_COMMAND_H_
#define IAP2APPFRAMEWORKEXTERNAL_COMMAND_H_

#include <stdint.h>
#include <stddef.h>

#include "iAp2AppFrameworkCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------- Command -------------------------------------*/
/*!
 * @ingroup ConnectionInformation  
 * @{
 */
 /*!
 * @enum     ConnectionState
 * @brief     This enum value is used to ConnectionState
 */
typedef enum {
    CS_USB_NOT_CONNECTED = 0,     /**< Not connect apple device at the accessroy*/
    CS_USB_CONNECTED,            /**< connect apple device at the accessroy*/
    CS_IDENTIFIED,                /**< Finish Identification with apple device*/
    CS_AUTHENTICATED,            /**< Finish Authentication with apple device*/
    CS_POWER_UPDATED,            /**< Finish Power negotiation with apple device*/
} ConnectionState;

 /*!
 * @enum     ConnectionPath    
 * @brief     This enum value is used to ConnectionPath 
 */
typedef enum {
    CP_NONE = 0,                /* Don't know device path */
    USB_PORT1,                    /* NOMAL PATH (PORT1) */
    USB_PORT2,                    /* NOMAL PATH (PORT2)*/
} ConnectionPath;

/*!
 * @enum     ConnectionMode    
 * @brief     This enum value is used to ConnectionMode.
 */
typedef enum {
    CM_AUB_HOST = 0,             /**< Accessory is USB HOST MODE*/
    CM_AUB_DEVICE,               /**< Accessory is USB DEVICE MODE*/
} ConnectionMode;

/*!
 * @enum     ConnectionCarPlaySupport    
 * @brief     This enum value is used to ConnectionCarPlaySupport
 */
typedef enum {
    CCS_NONE = 0,                   /**< Can't judge whether carplay is supported or not.*/
    CCS_CARPLAY_SUPPORTED,          /**< Carplay is supported*/
    CCS_CARPLAY_NOT_SUPPORTED,      /**< Carplay is not supported*/
    CCS_CARPLAY_AVAILABLE,          /**< Carplay Capability / CarPlay Available*/
    CCS_CARPLAY_NOT_AVAILABLE       /**< Carplay Capability / CarPlay Not Available*/
} ConnectionCarPlaySupport;

/*!
 * @enum     ConnectionIap2Support
 * @brief     This enum value is used to ConnectionIap2Support
 */
typedef enum {
    CIS_NONE = 0,                /**< Can't judge whether IAP2 is supported or not.*/
    CIS_IAP1_SURPORTED,            /**< IAP1 is supported*/
    CIS_IAP2_SURPORTED            /**< IAP2 is supported*/
} ConnectionIap2Support;

#ifdef ANDROID
/*!
 * @struct     UsbDeviceInfo
 * @brief     This Struct is used to save device Information
 */
typedef struct {
    int vendorId;                /**< vendorId (0x05ac) */
    int productId;                /**< productId (0x12xx)  */
    BOOL bPresent;              /**< bPresent of BOOL type is used to confirm whether product is present, if product is present then set TRUE, otherwise FALSE*/
    uint8_t *product;            /**< Product information (iPhone)*/
    uint32_t size;               /**< size of product  */
} UsbDeviceInfo;
#endif

/*!
 * @struct     InitConnectionData
 * @brief     This Struct is used to save connection data initialized\n
 */
typedef struct {
    ConnectionState connState;                     /**< connState has one of @ref ConnectionState value*/    
    ConnectionPath connPath;                    /**< connState has one of @ref ConnectionPath value*/
    ConnectionMode connMode;                    /**< connState has one of @ref ConnectionMode value*/
    ConnectionCarPlaySupport carplaySupport;    /**< connState has one of @ref ConnectionCarPlaySupport value*/    
    ConnectionIap2Support iap2Support;            /**< connState has one of @ref ConnectionIap2Support value*/
} InitConnectionData;

/*!
 * @enum     CarPlayClientState    
 * @brief     This enum value is used to CarPlayClientState.
 */
typedef enum {
    CCS_NOT_READY = 0,         /**< CarPlay Client is not ready*/
    CCS_READY,                   /**< CarPlay Client is ready */
} CarPlayClientState;

/*! @}  */

/*!
 * @ingroup DataParam
 * @{
 */
/*!
 * @struct     uint32Param
 * @brief     This Struct is used to save unsigned int data .\n
 */
typedef struct {
    BOOL bPresent;            /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t value;            /**< value of uint32_t is used to set ElapsedTime from apple device */
} uint32Param;    

/*!
 * @struct     CmdSaveUtf8Param
 * @brief     This Struct is used to save utf8 data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save utf8 data */
}CmdSaveUtf8Param;
/*! @}  */

/*!
 * @ingroup Power
 * @{
 */
 
/*!
 * @struct     StartPowerUpdateArg
 * @brief     This Struct is used to save payload data of StartPowerUpdate Message.\n
             If application want to get information about power, then set TRUE each value.\n
 */
typedef struct {
    BOOL bMaxCurrentDrawnFromAcc;         /**< bMaxCurrentDrawnFromAcc of BOOL type is used to request AvailableCurrentForDevice from apple device */
    BOOL bDevBatteryCharge;                  /**< bDevBatteryCharge of BOOL type is used to request DevBatteryCharge from apple device */
    BOOL bAccPowerMode;                      /**< bAccPowerMode of BOOL type is used to request AccPowerMode from apple device */
    BOOL bIsExternalChargerConnected;     /**< bIsExternalChargerConnected of BOOL type is used to request IsExternalChargerConnected from apple device */
    BOOL bBatteryChargingState;            /**< bBatteryChargingState of BOOL type is used to request BatteryChargingState from apple device */
    BOOL bBatteryChargeLevel;            /**< bBatteryChargeLevel of BOOL type is used to request BatteryChargeLevel from apple device */
} StartPowerUpdateArg;

 
/*!
 * @struct     AvailableCurrentForDeviceParam
 * @brief     This Struct is used to save AvailableCurrentForDevice payload data of PowerSourceUpdate Message.\n
 */
typedef struct {
    BOOL bPresent;                   /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;                    /**< value of uint16_t type is used to set AvailableCurrentForDevice value by application */
} AvailableCurrentForDeviceParam;

/*!
 * @struct     DevBatteryChargeParam
 * @brief     This Struct is used to save DevcieBatteryShouldchargeIfPowerIsPresent payload data of PowerSourceUpdate Message
             or DevcieBatteryWillchargeIfPowerIsPresent payload data of PowerUpdate Message.\n
 */
typedef struct {
    BOOL bPresent;            /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    BOOL value;             /**< value of BOOL type is used to set DevcieBatteryShouldchargeIfPowerIsPresent value by application eBatteryWillchargeIfPowerIsPresent value by iAP2Service. */
} DevBatteryChargeParam;

/*!
 * @struct     PowerSourceUpdateArg
 * @brief     This Struct is used to save payload data of PowerSourceUpdate Message.\n 
 */
typedef struct {
    AvailableCurrentForDeviceParam availCurrent; /**< Refer @ref AvailableCurrentForDeviceParam */
    DevBatteryChargeParam devBatteryCharge;         /**< Refer @ref DevBatteryChargeParam */    
} PowerSourceUpdateArg;
/*! @} */



/*!
 * @ingroup NowPlaying
 * @{
 */ 
/*!
 * @struct     MediaItemAttrReqGroup
 * @brief     This Struct is used to save payload data of StartNowPlaying Message.\n
             If application want to get information about MediaItemAttributes of nowplaying, then set TRUE each value.\n
 */
typedef struct {
    BOOL bPersistentIdentifier;            /**< bPersistentIdentifier of BOOL type is used to request PersistentIdentifier from apple device */        
    BOOL bTitle;                        /**< bTitle of BOOL type is used to request MediaItemTitle from apple device */    
    BOOL bPlaybackDurationMs;            /**< bPlaybackDurationMs of BOOL type is used to request MediaItemPlaybackDurationInMilliseconds from apple device */
    BOOL bAlbumTitle;                    /**< bAlbumTitle of BOOL type is used to request MediaItemAlbumTitle from apple device */
    BOOL bAlbumTrackNo;                    /**< bAlbumTrackNo of BOOL type is used to request MediaItemAlbumTrackNumber from apple device */    
    BOOL bAlbumTrackCount;                /**< bAlbumTrackCount of BOOL type is used to request MediaItemAlbumCount from apple device */
    BOOL bAlbumDiscNumber;                /**< bAlbumDiscNumber of BOOL type is used to request MediaItemAlbumDiscNumber from apple device */
    BOOL bAlbumDiscCount;                /**< bAlbumDiscCount of BOOL type is used to request MediaItemAlbumDiscCount from apple device */
    BOOL bArtist;                        /**< bArtist of BOOL type is used to request MediaItemArtist from apple device */
    BOOL bGenre;                        /**< bGenre of BOOL type is used to request MediaItemGenre from apple device */
    BOOL bComposer;                        /**< bComposer of BOOL type is used to request MediaItemComposer from apple device */
    BOOL bPropertyIsLikeSupported;        /**< bPropertyIsLikeSupported of BOOL type is used to request MediaItemIsLikeSupported from apple device */    
    BOOL bPropertyIsLikeBanSupported;    /**< bPropertyIsLikeBanSupported of BOOL type is used to request MediaItemIsBanSupported from apple device */
    BOOL bPropertyIsLiked;                /**< bPropertyIsLiked of BOOL type is used to request MediaItemIsLiked from apple device */
    BOOL bPropertyIsLikeBanned;            /**< bPropertyIsLikeBanned of BOOL type is used to request MediaItemIsBanned from apple device */    
    BOOL bArtworkFileTransferId;        /**< bArtworkFileTransferId of BOOL type is used to request MediaItemArtworkFileTransferIdentifier from apple device */
    BOOL bChapterCount;                    /**< bChapterCount of BOOL type is used to request ChapterCount from apple device */
} MediaItemAttrReqGroup;

/*!
 * @struct     PlaybackAttrReqGroup
 * @brief     This Struct is used to save payload data of StartNowPlaying Message.\n
             If application want to get information about PlaybackAttribute of nowplaying, then set TRUE each value.\n
 */
typedef struct {
    BOOL bStatus;                                            /**< bStatus of BOOL type is used to request PlaybackStatus from apple device */
    BOOL bElapsedTimeMs;                                    /**< bElapsedTimeMs of BOOL type is used to request PlaybackElapsedTimeInMilliseconds from apple device */
    BOOL bQueueIdx;                                            /**< bQueueIdx of BOOL type is used to request PlaybackQueueIndex from apple device */
    BOOL bQueueCount;                                        /**< bQueueCount of BOOL type is used to request PlaybackQueuecount from apple device */
    BOOL bQueueChapterIdx;                                    /**< bQueueChapterIdx of BOOL type is used to request PlaybackQueueChapterIndex from apple device */
    BOOL bShuffleMode;                                        /**< bShuffleMode of BOOL type is used to request PlaybackShuffleMode from apple device */
    BOOL bRepeatMode;                                        /**< bRepeatMode of BOOL type is used to request PlaybackRepeatMode from apple device */
    BOOL bAppName;                                            /**< bAppName of BOOL type is used to request PlaybackAppName from apple device, Application must set True this value. */
    BOOL bPbMediaLibUniqueId;                                /**< bPbMediaLibUniqueId of BOOL type is used to request PlaybackMediaLibraryUniqueIdentifier from apple device */
    BOOL bPbiTunesRadioAd;                                    /**< bPbiTunesRadioAd of BOOL type is used to request PlaybackiTunesRadioAd from apple device */
    BOOL bPbiTunesRadioStationName;                            /**< bPbiTunesRadioStationName of BOOL type is used to request PlaybackiTunesRadioStationName from apple device */
    BOOL bPbiTunesRadioStationMediaPlaylistPersistentID;    /**< bPbiTunesRadioStationMediaPlaylistPersistentID of BOOL type is used to request PlaybackiTunesRadioMediaPlaylistPersistentID from apple device */
    BOOL bPbSpeed;                                            /**< bPbSpeed of BOOL type is used to request PlaybackSpeedD from apple device */    
    BOOL bSetElapsedTimeAvailable;                            /**< bSetElapsedTimeAvailable of BOOL type is used to request SetElapsedTimeAvailable from apple device */
    BOOL bPbQueueListAvail;                                    /**< bPbQueueListAvail of BOOL type is used to request PlaybackQueueListAvail from apple device */
    BOOL bPbQueueListTransferID;                            /**< bPbQueueListTransferID of BOOL type is used to request PlaybackQueueListTransferID from apple device */
    BOOL bPbAppBundleID;                                    /**< bPbAppBundleID of BOOL type is used to request PlaybackAppBundleID from apple device */
} PlaybackAttrReqGroup;                                        

/*!
 * @struct     StartNowPlayingArg
 * @brief     This Struct is used to save payload data of StartNowPlaying Message.\n
             If application want to get information about PlaybackAttribute of nowplaying, then set TRUE each value.\n
 */
typedef struct {
    BOOL bMiPresent;                         /**< bMiPresent of BOOL type is used to confirm whether mediaAttrReqGr is present, if value is present then set TRUE, otherwise FALSE*/    
    MediaItemAttrReqGroup mediaAttrReqGr;     /**< Refer @ref MediaItemAttrReqGroup */
    BOOL bPbPresent;                         /**< bPbPresent of BOOL type is used to confirm whether playbackAttrReqGr is present, if value is present then set TRUE, otherwise FALSE*/        
    PlaybackAttrReqGroup playbackAttrReqGr;     /**< Refer @ref PlaybackAttrReqGroup */
} StartNowPlayingArg;



/*!
 * @struct     SetNowPlayingInformationArg
 * @brief     This Struct is used to save payload data of SetNowPlayingInformation Message.\n
 */
typedef struct {    
    uint32Param nElapsedTimeParam;                 /**< nElapsedTimeParam of uint32Param type is used to save ElapsedTime*/
    uint32Param nPlayBackQueueIndexParam;         /**< nPlayBackQueueIndexParam of uint32Param type is used to save ElapsedTime*/
} SetNowPlayingInformationArg;
/*! @} */


/*!
 * @ingroup VehicleStatus
 * @{
 */
 
/*!
 * @struct     VehicleStatusRageParam
 * @brief     This Struct is used to save Rage payload data of VehicleStatusUpdate Message.\n
 */
typedef struct {
    BOOL bPresent;            /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;            /**< value of uint16_t type is used to set Rage value by application  */
} VehicleStatusRageParam;

/*!
 * @struct     VehicleStatusOutsideTemperatureParam
 * @brief     This Struct is used to save OutsideTemperature payload data of VehicleStatusUpdate Message.\n
 */
typedef struct {
    BOOL bPresent;            /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    int16_t value;            /**< value of uint16_t type is used to set OutsideTemperature value by application  */
} VehicleStatusOutsideTemperatureParam;

/*!
 * @struct     VehicleStatusRangeWarningParam
 * @brief     This Struct is used to save RangeWarning payload data of VehicleStatusUpdate Message.\n
 */
typedef struct {
    BOOL bPresent;            /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    BOOL value;                /**< value of uint16_t type is used to set RangeWarning value by application  */
} VehicleStatusRangeWarningParam;

/*!
 * @struct     VehicleStatusUpdateArg
 * @brief     This Struct is used to save payload data of VehicleStatusUpdate Message.\n
 */
typedef struct {
    VehicleStatusRageParam range;                        /**< Refer @ref VehicleStatusRageParam */
    VehicleStatusOutsideTemperatureParam outSideTemp;    /**< Refer @ref VehicleStatusOutsideTemperatureParam */
    VehicleStatusRangeWarningParam rangeWarning;        /**< Refer @ref VehicleStatusRangeWarningParam */
} VehicleStatusUpdateArg;
/*! @} */



/*!
 * @ingroup Location
 * @{
 */
 
/*!
 * @struct     GPSFixDataParam
 * @brief     This Struct is used to save GlobalPositioningSystemFixData payload data of LocationInformaion Message. \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value;    /**< value of uint8_t* type is used to save GlobalPositioningSystemFixData value by application*/
} GPSFixDataParam;

/*!
 * @struct     RecommMinSpecificGPSTransitDataParam
 * @brief     This Struct is used to save RecommendedMinimumSpecificGPSTransitData payload data of LocationInformaion Message. \n
 */ 
typedef struct {
    BOOL bPresent;     /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save RecommendedMinimumSpecificGPSTransitData value by application */
} RecommMinSpecificGPSTransitDataParam;

/*!
 * @struct     GPSSatellitesInViewParam
 * @brief     This Struct is used to save GPSSatellitesInView payload data of LocationInformaion Message. \n
 */ 
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save GPSSatellitesInView value by application */
} GPSSatellitesInViewParam;

/*!
 * @struct     VehicleSpeedDataParam
 * @brief     This Struct is used to save VehicleSpeedData payload data of LocationInformaion Message. \n
 */ 
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save VehicleSpeedData value by application */
} VehicleSpeedDataParam;

/*!
 * @struct     VehicleGyroDataParam
 * @brief     This Struct is used to save VehicleGyroData payload data of LocationInformaion Message.\n
 */ 
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save VehicleGyroData value by application */
} VehicleGyroDataParam;

/*!
 * @struct     VehicleAccelDataParam
 * @brief     This Struct is used to save VehicleAccelometerData payload data of LocationInformaion Message. \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save VehicleAccelometerData value by application */
} VehicleAccelDataParam;

/*!
 * @struct     VehicleHeadingDataParam
 * @brief     This Struct is used to save VehicleHeadingData payload data of LocationInformaion Message. \n
 */
typedef struct {
    BOOL bPresent;     /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save VehicleHeadingData value by application */
} VehicleHeadingDataParam;

/*!
 * @struct     LocationInformationArg
 * @brief     This Struct is used to save payload data of LocationInformation Message. \n
 */
typedef struct {
    GPSFixDataParam gpsFixData;                                                /**< Refer @ref GPSFixDataParam */
    RecommMinSpecificGPSTransitDataParam recommMinSpecificGPSTransitData;    /**< Refer @ref RecommMinSpecificGPSTransitDataParam */
    GPSSatellitesInViewParam gpsSatellitesInView;                            /**< Refer @ref GPSSatellitesInViewParam */
    VehicleSpeedDataParam vehicleSpeedData;                                    /**< Refer @ref VehicleSpeedDataParam */    
    VehicleGyroDataParam vehicleGyroData;                                    /**< Refer @ref VehicleGyroDataParam */
    VehicleAccelDataParam vehicleAccelData;                                    /**< Refer @ref VehicleAccelDataParam */
    VehicleHeadingDataParam vehicleHeadingData;                                /**< Refer @ref VehicleHeadingDataParam */
} LocationInformationArg;
/*! @} */



/*!
 * @ingroup Telephony
 * @{
 */
 
/*!
 * @struct     StartTelephonyCallStateInformationArg
 * @brief     This Struct is used to save CallStatevCardFileTransferIdentifier payload data of StartTelephonyCallStateInformation Message. \n
 */
typedef struct {
    BOOL bCallStatevCardFileTransferID;    /**< bCallStatevCardFileTransferID of BOOL type is used to request CallStatevCardFile data from apple device */
} StartTelephonyCallStateInformationArg;

/*!
 * @struct     StartTelephonyUpdatesArg
 * @brief     This Struct is used to save payload data of StartTelephonyCallStateInformation Message. \n
 */
typedef struct {
    BOOL bTelephonySignalStrength;        /**< bTelephonySignalStrength of BOOL type is used to request TelephonySignalStrength from apple device */
    BOOL bTelephonyRegisStatus;            /**< bTelephonyRegisStatus of BOOL type is used to request TelephonyRegistrationStatus from apple device */
    BOOL bTelephonyAirPlaneModeStatus;    /**< bTelephonyAirPlaneModeStatus of BOOL type is used to request TelephonyAirPlaneModeStatus from apple device */
    BOOL bTelephonyTTYStatus;            /**< bTelephonyTTYStatus of BOOL type is used to request TelephonyTTYStatus from apple device */
    BOOL bTMobileOperator;                /**< bTMobileOperator of BOOL type is used to request MobileOperator from apple device */
} StartTelephonyUpdatesArg;
/*! @} */



/*!
 * @ingroup BluetoothConnection
 * @{
 */
 
 /*!
 * @struct     BTComponentStatusParam
 * @brief     This Struct is used to save BlueToothComponentStatus payload data of BlueToothComponentStatus Message. \n
 */
typedef struct {
    uint16_t componentID;    /**< componentID of uint16_t type is used to send ComponentIdentifier data to apple device \n
                                This value is set by appplication in identification message*/
    BOOL componentEnabled;    /**< componentEnabled of BOOL type is used to send componentEnabled data to apple device\n
                                This value is set True if Bluetooth component is ready for connection  to the apple device */
} BTComponentStatusParam;

 /*!
 * @struct     BTComponentInformationArg
 * @brief     This Struct is used to save payload data of BTComponentInformation Message. \n
 */
typedef struct {
    BOOL bPresent;                                /**< bPresent of BOOL type is used to confirm whether btComponentStatus is present, if value is present then set TRUE, otherwise FALSE*/
    BTComponentStatusParam* btComponentStatus;    /**< Refer @ref BTComponentStatusParam */        
    uint32_t nBTComponentStatusCount;             /**< number of BTComponentStatusParam*/        
} BTComponentInformationArg;


 /*!
 * @struct     TransportComponentIDParam
 * @brief     This Struct is used to save TransportComponentIdentifier payload data of BluetoothTransportComponet Group. \n
 */
typedef struct {
    BOOL bPresent;                /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;                /**< value of uint16_t type is used to send TransportComponentIdentifier to apple device */
} TransportComponentIDParam;

/*!
 * @struct     TransportComponentNameParam
 * @brief     This Struct is used to save TransportComponentName payload data of BluetoothTransportComponet Group. \n
 */
typedef struct {
    BOOL bPresent;                /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;                /**< value of uint16_t type is used to send TransportComponentName to apple device\n */
} TransportComponentNameParam;


/*!
 * @struct     BTTransportMediaAccessControlAddParam
 * @brief     This Struct is used to save BluetoothTTransportMediaAccessControlAddress payload data of BluetoothTransportComponet Group. \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */    
    uint8_t* value;    /**< value of uint8_t* type is used to save BluetoothTTransportMediaAccessControlAddress value by application */
} BTTransportMediaAccessControlAddParam;


/*!
 * @struct     ComponentIDParam
 * @brief     This Struct is used to save payload data of BluetoothTransportComponet Group. \n
 */
typedef struct {
    TransportComponentIDParam transportComponentID;                                /**< Refer @ref TransportComponentIDParam */    
    TransportComponentNameParam transportComponentName;                            /**< Refer @ref TransportComponentNameParam */    
    BOOL bTransportSupportiAP2Connection;                                        /**< bTransportSupportiAP2Connection of BOOL type is used to save TransportSupportiAP2Connection data */    
    BTTransportMediaAccessControlAddParam btTransportMediaAccessControlAddess;    /**< Refer @ref BTTransportMediaAccessControlAddParam */    
} ComponentIDParam;

/*!
 * @struct     StartBTConnectionUpdatesArg
 * @brief     This Struct is used to save payload data of StartBTConnectionUpdates Message. \n
 */
typedef struct {
    BOOL bPresent;                         /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t* BTTransportComponentID;      /**< BTTransportComponentID of uint16_t type is used to send BluetoothTransportComponentIdentifier value to apple device */
    uint32_t nBTTransportComponentIDCount; /**< number of BTTransportComponentID */
} StartBTConnectionUpdatesArg;
/*! @} */



/*!
 * @ingroup HumanInterfaceDevice
 * @{
 */
 
 /*!
 * @struct     LocalizedKeyBoardCountryCodeParam
 * @brief     This Struct is used to save LocalizedKeyBoardCountryCode payload data of StartHID Message. \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save LocalizedKeyBoardCountryCode value  */
} LocalizedKeyBoardCountryCodeParam;

 /*!
 * @struct     HIDReportDescriptorParam
 * @brief     This Struct is used to save HIDReportDescriptor payload data of StartHID Message. \n
 */
typedef struct {
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */    
    uint8_t* value;    /**< value of uint8_t* type is used to save HIDReportDescriptor value */
} HIDReportDescriptorParam;

 /*!
 * @struct     StartHIDArg
 * @brief     This Struct is used to send payload data of StartHID Message to apple device. \n
 */
typedef struct {        
    uint16_t HIDCompnetIdentifier;                                    /**< HIDCompnetIdentifier of uint16_t type is used to save HIDCompnetIdentifier of StartHID Message */
    uint16_t VendorIdentifier;                                        /**< VendorIdentifier of uint16_t type is used to save VendorIdentifier of StartHID Message */
    uint16_t ProductIdentifer;                                        /**< ProductIdentifer of uint16_t type is used to save ProductIdentifer of StartHID Message */
    LocalizedKeyBoardCountryCodeParam LocalizedKeyBoardCountryCode; /**< Refer @ref LocalizedKeyBoardCountryCodeParam */    
    HIDReportDescriptorParam HIDReportDescriptor;                     /**< Refer @ref HIDReportDescriptorParam */    
} StartHIDArg;


 /*!
 * @struct     StopHIDArg
 * @brief     This Struct is used to send payload data of StopHID Message to apple device. \n
 */
typedef struct {
    uint16_t HIDCompnetIdentifier; /**< HIDCompnetIdentifier of uint16_t type is used to save HIDCompnetIdentifier of StartHID Message */
} StopHIDArg;

 /*!
 * @struct     HIDReportParam
 * @brief     This Struct is used to save HIDReport payload data of AccessoryHIDReport Message. \n
 */
typedef struct {
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save HIDReport value */
} HIDReportParam;

/*!
 * @struct     AccessoryHIDReportArg
 * @brief     This Struct is used to send payload data of AccessoryHIDReport Message to apple device. \n
 */
typedef struct {
    uint16_t HIDCompnetIdentifier;    /**< HIDCompnetIdentifier of uint16_t type is used to save HIDCompnetIdentifier of AccessoryHIDReport Message */
    HIDReportParam HIDReport; /**< Refer @ref HIDReportParam */    
} AccessoryHIDReportArg;
/*! @} */


/*!
 * @ingroup MediaLibararyAccess
 * @{
 */
 
 /*!
 * @struct     MediaLibraryUniqueIdentifierParam
 * @brief     This Struct is used to save MediaLibraryUniqueIdentifier payload data. \n
 */ 
typedef struct {
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save MediaLibraryUniqueIdentifier value */
} MediaLibraryUniqueIdentifierParam;

 /*!
 * @struct     LastKnownMediaLibraryRevisionParam
 * @brief     This Struct is used to save LastKnownMediaLibraryRevision payload data. \n
 */ 
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save LastKnownMediaLibraryRevision value */
} LastKnownMediaLibraryRevisionParam;


 /*!
 * @struct     MediaItemPropoertiesGroup
 * @brief     This Struct is used to request MediaItemPropoertie data using StartMediaLibraryUpdate Message from apple devices. \n
 */ 
typedef struct {
    BOOL bPresent;                                            /**< bPresent of BOOL type is used to confirm whether MediaItemPropoerties is present, if MediaItemPropoerties is present then set TRUE, otherwise FALSE*/
    BOOL MediaItemPropertyPersistentIdentifier;             /**< MediaItemPropertyPersistentIdentifier of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyTitle;                            /**< MediaItemPropertyTitle of BOOL type is used to request MediaItemPropertyTitle data from applde device*/
    BOOL MediaItemPropertyMediaType;                        /**< MediaItemPropertyMediaType of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyRating;                            /**< MediaItemPropertyRating of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/    
    BOOL MediaItemPropertyPlaybackDurationInMilliseconds;    /**< MediaItemPropertyPlaybackDurationInMilliseconds of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyAlbumPersistentIdentifier;        /**< MediaItemPropertyAlbumPersistentIdentifier of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyAlbumTitle;                        /**< MediaItemPropertyAlbumTitle of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyAlbumTrackNumber;                    /**< MediaItemPropertyAlbumTrackNumber of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyAlbumTrackCount;                    /**< MediaItemPropertyAlbumTrackCount of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyAlbumDiscNumber;                    /**< MediaItemPropertyAlbumDiscNumber of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyAlbumDiscCount;                    /**< MediaItemPropertyAlbumDiscCount of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyArtistPersistentIdentifier;        /**< MediaItemPropertyArtistPersistentIdentifier of BOOL type is used to request MediaItemPropertyArtistPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyArtist;                            /**< MediaItemPropertyArtist of BOOL type is used to request MediaItemPropertyArtist data from applde device*/
    BOOL MediaItemPropertyAlbumAritistPersistentIdentifier;    /**< MediaItemPropertyAlbumAritistPersistentIdentifier of BOOL type is used to request MediaItemPropertyAlbumAritistPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyAlbumAritist;                        /**< MediaItemPropertyAlbumAritist of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyGenrePersistentIdentifier;        /**< MediaItemPropertyGenrePersistentIdentifier of BOOL type is used to request MediaItemPropertyPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyGenre;                            /**< MediaItemPropertyGenre of BOOL type is used to request MediaItemPropertyGenre data from applde device*/
    BOOL MediaItemPropertyComposerPersistentIdentifier;        /**< MediaItemPropertyComposerPersistentIdentifier of BOOL type is used to request MediaItemPropertyComposerPersistentIdentifier data from applde device*/
    BOOL MediaItemPropertyComposer;                            /**< MediaItemPropertyComposer of BOOL type is used to request MediaItemPropertyComposer data from applde device*/
    BOOL MediaItemPropertyIsPartOfCompilation;                /**< MediaItemPropertyIsPartOfCompilation of BOOL type is used to request MediaItemPropertyIsPartOfCompilation data from applde device*/    
    BOOL MediaItemPropertyIsResidentOnDevice;                /**< MediaItemPropertyIsResidentOnDevice of BOOL type is used to request MediaItemPropertyIsResidentOnDevice data from applde device*/    
    BOOL MediaItemPropertyChapterCount;                        /**< MediaItemPropertyChapterCount of BOOL type is used to request MediaItemPropertyChapterCount data from applde device*/    
} MediaItemPropoertiesGroup;    

 /*!
 * @struct     MediaPlaylistPropertiesGroup
 * @brief     This Struct is used to request MediaPlaylistProperties data using StartMediaLibraryUpdate Message from apple devices. \n
 */ 
typedef struct {                                                            
    BOOL bPresent;                                                            /**< bPresent of BOOL type is used to confirm whether MediaPlaylistProperties is present, if MediaPlaylistProperties is present then set TRUE, otherwise FALSE*/                                                            /**< bPresent of BOOL type is used to confirm whether MediaPlaylistProperties is present, if MediaPlaylistProperties is present then set TRUE, otherwise FALSE*/        
    BOOL MediaPlaylistPropertyPersistentIdentifier;                            /**< MediaPlaylistPropertyPersistentIdentifier of BOOL type is used to request MediaPlaylistPropertyPersistentIdentifier data from applde device*/    
    BOOL MediaPlaylistPropertyName;                                            /**< MediaPlaylistPropertyName of BOOL type is used to request MediaPlaylistPropertyName data from applde device*/    
    BOOL MediaPlaylistPropertyParentPersistentIdentifier;                    /**< MediaPlaylistPropertyParentPersistentIdentifier of BOOL type is used to request MediaPlaylistPropertyParentPersistentIdentifier data from applde device*/    
    BOOL MediaPlaylistPropertyIsGeniusMix;                                    /**< MediaPlaylistPropertyIsGeniusMix of BOOL type is used to request MediaPlaylistPropertyIsGeniusMix data from applde device*/    
    BOOL MediaPlaylistPropertyIsFolder;                                        /**< MediaPlaylistPropertyIsFolder of BOOL type is used to request MediaPlaylistPropertyIsFolder data from applde device*/    
    BOOL MediaPlaylistPropertyContainedMediaItemsFileTransferIdentifier;    /**< MediaPlaylistPropertyContainedMediaItemsFileTransferIdentifier of BOOL type is used to request MediaPlaylistPropertyContainedMediaItemsFileTransferIdentifier data from applde device*/    
    BOOL MediaPlaylistPropertyIsiTunesRadioStation;                            /**< MediaPlaylistPropertyIsiTunesRadioStation of BOOL type is used to request MediaPlaylistPropertyIsiTunesRadioStation data from applde device*/    
} MediaPlaylistPropertiesGroup;


/*!
 * @struct     StartMediaLibraryUpdatesArg
 * @brief     This Struct is used to send StartMediaLibraryUpdate Message to apple devices. \n
 */ 
typedef struct {
    MediaLibraryUniqueIdentifierParam MediaLibraryUniqueIdentifier_Start;    /**< Refer @ref MediaLibraryUniqueIdentifierParam */    
    LastKnownMediaLibraryRevisionParam LastKnownMediaLibraryRevision;        /**< Refer @ref LastKnownMediaLibraryRevisionParam */    
    MediaItemPropoertiesGroup MediaItemProperties;                            /**< Refer @ref MediaItemPropoertiesGroup */    
    MediaPlaylistPropertiesGroup MediaPlaylistProperties;                    /**< Refer @ref MediaPlaylistPropertiesGroup */    
    BOOL MediaLibraryUpdateProgress;                                        /**< MediaLibraryUpdateProgress of BOOL type is used to request MediaLibraryUpdateProgress data from applde device*/    
    BOOL MediaLibraryIsHidingRemoteItems;                                    /**< MediaLibraryIsHidingRemoteItems of BOOL type is used to request MediaLibraryIsHidingRemoteItems data from applde device*/    
    BOOL PlayAllSongsCapable;                                                /**< PlayAllSongsCapable of BOOL type is used to request PlayAllSongsCapable data from applde device*/    
} StartMediaLibraryUpdatesArg;

/*!
 * @struct     StopMediaLibraryUpdatesArg
 * @brief     This Struct is used to send StopMediaLibraryUpdates Message to apple devices. \n
 */ 
typedef struct {
    MediaLibraryUniqueIdentifierParam MediaLibraryUniqueIdentifier_Stop;     /**< Refer @ref MediaLibraryUniqueIdentifierParam */    
} StopMediaLibraryUpdatesArg;

/*!
 * @struct     PlayMediaLibraryCurrentSelectionArg
 * @brief     This Struct is used to send PlayMediaLibraryCurrentSelection Message to apple devices. \n
 */ 
typedef struct {
    MediaLibraryUniqueIdentifierParam MediaLibraryUniqueIdentifier_CurrentSelection;    /**< Refer @ref MediaLibraryUniqueIdentifierParam */    
} PlayMediaLibraryCurrentSelectionArg;

/*!
 * @struct     ItemsPersistentIdentifierParam
 * @brief     This Struct is used to save ItemsPersistentIdentifier payload data. \n
 */ 
typedef struct {
    uint32_t size;        /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;     /**< value of uint8_t* type is used to save ItemsPersistentIdentifier value */
} ItemsPersistentIdentifierParam;

/*!
 * @struct     ItemsStartingIndexParam
 * @brief     This Struct is used to save ItemsStartingIndex payload data. \n
 */ 
typedef struct {
    BOOL bPresent;        /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t value;        /**< value of uint8_t* type is used to save ItemsStartingIndex value */
} ItemsStartingIndexParam;

/*!
 * @struct     PlayMediaLibraryItemsArg
 * @brief     This Struct is used to send PlayMediaLibraryItem message to apple device. \n
 */ 
typedef struct {
    ItemsPersistentIdentifierParam ItemsPersistentIdentifier;                /**< Refer @ref ItemsPersistentIdentifierParam */    
    ItemsStartingIndexParam ItemStartingIndex;                                /**< Refer @ref ItemsStartingIndexParam */    
    MediaLibraryUniqueIdentifierParam MediaLibraryUniqueIdentifier_Item;    /**< Refer @ref MediaLibraryUniqueIdentifierParam */    
} PlayMediaLibraryItemsArg;

/*!
 * @struct     CollectionStartingIndexParam
 * @brief     This Struct is used to save CollectionStartingIndex payload data.\n
 */ 
typedef struct {
    BOOL bPresent;     /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t value;     /**< value of uint8_t* type is used to save CollectionStartingIndex value */
} CollectionStartingIndexParam;

/*!
 * @struct     PlayMediaLibraryCollectionArg
 * @brief     This Struct is used to send PlayMediaLibraryCollection message to apple device.\n
 */ 
typedef struct {
    uint64_t CollectionPersistentIdentifier;                                     /**< CollectionPersistentIdentifier of uint64_t type is used to save CollectionPersistentIdentifier data. */    
    uint8_t CollectionType;                                                        /**< CollectionType of uint8_t type is used to save CollectionType data. */    
    CollectionStartingIndexParam CollectionStartingIndex;                        /**< Refer @ref CollectionStartingIndexParam */    
    MediaLibraryUniqueIdentifierParam MediaLibraryUniqueIdentifier_Collection;     /**< Refer @ref MediaLibraryUniqueIdentifierParam */    
} PlayMediaLibraryCollectionArg;


/*!
 * @struct     PlayMediaLibrarySpecialnArg
 * @brief     This Struct is used to send PlayMediaLibrarySpecial message to apple device.\n
 */ 
typedef struct {
    MediaLibraryUniqueIdentifierParam MediaLibraryUniqueIdentifier_Special;     /**< Refer @ref MediaLibraryUniqueIdentifierParam */    
    BOOL AllSongs;
} PlayMediaLibrarySpecialArg;
/*! @} */

/*!
 * @ingroup AppLaunch
 * @{
 */
 /*!
 * @struct     AppBundleIDParam
 * @brief     This Struct is used to save AppBundleID data.\n
 */ 
typedef struct{
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save AppBundleID value */
}AppBundleIDParam;

 /*!
 * @enum     LaunchAlert
 * @brief     This eum is value of LaunchAlert param.\n
 */ 
typedef enum {
    LaunchAlert_Launch_UserAlert = 0,       /**< Launch with user alert(default) */    
    LaunchAlert_Launch_WithoutAnyAlert = 1  /**< Launch without any alert, but device must be unlocked and on Home Screen or Music/video App */
}LaunchAlert;

 /*!
 * @struct     AppBundleIDParam
 * @brief     This Struct is used to save AppBundleID data.\n
 */ 
 typedef struct{
     BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    LaunchAlert eLaunchAlert;    /**< eLaunchAlert is used to save LaunchAlert */    
}LaunchAlertParam;

  /*!
 * @struct     RequestAppLaunchArg
 * @brief     This Struct is used to send RequestAppLaunch message to apple device.\n
 */ 
typedef struct {
    AppBundleIDParam AppBundleID;         /**< Refer @ref AppBundleIDParam */        
    LaunchAlertParam mLaunchAlert;         /**< Refer @ref LaunchAlertParam */        
} RequestAppLaunchArg;
/*! @} */

/*!
 * @ingroup VoiceOver
 * @{
 */
 /*!
 * @enum     VOCurrentDirection
 * @brief     This eum is value of CursorDirection param.\n
 */ 
typedef enum {
    VOCurrentDirection_Next = 0,             /**< Next */    
    VOCurrentDirection_Prev = 1,              /**< Previous */
    VOCurrentDirection_Escape = 2            /**< Escape */
}VOCurSorDirection;

 /*!
 * @struct     RequestVoiceOverMoveCursorArg
 * @brief     This Struct is used to send RequestVoiceOverMoveCursor Message to apple device.\n
 */ 
typedef struct {
    VOCurSorDirection CursorDirection;     /**< CursorDirection of uint8_t type is used to save CursorDirection value */
} RequestVoiceOverMoveCursorArg;


 /*!
 * @enum     VOScrollDirection
 * @brief     This eum is value of ScrollDirection param.\n
 */ 
typedef enum {
    VOScrollDirection_Left = 0,                 /**< Left */    
    VOScrollDirection_Right = 1,              /**< Right */
    VOScrollDirection_Up = 2,                /**< Up */
    VOScrollDirection_Down = 3                /**< Down */    
}VOScrollDirection;
 
 /*!
 * @struct     RequestVoiceOverScrollPageArg
 * @brief     This Struct is used to send RequestVoiceOverScrollPage Message to apple device.\n
 */ 
typedef struct {
    VOScrollDirection ScrollDirection;    /**< ScrollDirection of uint8_t type is used to save ScrollDirection value */
} RequestVoiceOverScrollPageArg;


 /*!
 * @struct     TextToSpeakParam
 * @brief     This Struct is used to save TextToSpeak data.\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save TextToSpeak value */
} TextToSpeakParam;

 /*!
 * @struct     RequestVoiceOverSpeakTextArg
 * @brief     This Struct is used to send RequestVoiceOverSpeakText message to apple device.\n
 */
typedef struct {
    TextToSpeakParam TextToSpeak;    /**< Refer @ref TextToSpeakParam */
} RequestVoiceOverSpeakTextArg;

/*!
 * @struct     StartVoiceOverUpdatesArg
 * @brief     This Struct is used to send StartVoiceOverUpdates message to apple device.\n
 */
typedef struct {
    BOOL SpeakingVolume;    /**< SpeakingVolume of BOOL type is used to request SpeakingVolume data from applde device*/    
    BOOL SpeakingRate;        /**< SpeakingRate of BOOL type is used to request SpeakingRate data from applde device*/    
    BOOL Enabled;            /**< Enabled of BOOL type is used to request Enabled data from applde device*/    
} StartVoiceOverUpdatesArg;


/*!
 * @struct     SpeakingVolumeParam
 * @brief     This Struct is used to save SpeakingVolume message data.\n
 */
typedef struct {
    BOOL bPresent;        /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;         /**< value of uint8_t* type is used to save SpeakingVolume data */
} SpeakingVolumeParam;

/*!
 * @struct     SpeakingRateParam
 * @brief     This Struct is used to save SpeakingRate message data.\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t* type is used to save SpeakingRate data */
} SpeakingRateParam;

/*!
 * @struct     RequestVoiceOverConfigurationArg
 * @brief     This Struct is used to send RequestVoiceOverConfiguration message to apple device.\n
 */
typedef struct {
    SpeakingVolumeParam SpeakingVolume;    /**< Refer @ref SpeakingVolumeParam */
    SpeakingRateParam SpeakingRate;        /**< Refer @ref SpeakingRateParam */
} RequestVoiceOverConfigurationArg;


/*!
 * @struct     StartVoiceOverCursorUpdatesArg
 * @brief     This Struct is used to send StartVoiceOverCursorUpdates message to apple device.\n
 */
typedef struct {
    BOOL Label;        /**< Label of BOOL type is used to request Label data from applde device*/    
    BOOL Value;        /**< Value of BOOL type is used to request Value data from applde device*/    
    BOOL Hint;        /**< Hint of BOOL type is used to request Hint data from applde device*/    
    BOOL Traits;    /**< Traits of BOOL type is used to request Traits data from applde device*/        
} StartVoiceOverCursorUpdatesArg;
/*! @} */

/*!
 * @ingroup Identification
 * @{
 */
 
/*!
 * @struct     IdentficationSelectArg
 * @brief     This Struct is used to set additional message in the identification Message.\n
 */
typedef struct {
    BOOL AppLaunch;                /**< AppLaunch of BOOL type is used to set On or Off for AppLaunch in the Idnetification message, True is setting Applaunch message.*/    
    BOOL AssistiveTouch;        /**< AssistiveTouch of BOOL type is used to set On or Off for AssistiveTouch in the Idnetification message, True is setting AssistiveTouch message.*/        
    BOOL MediaLibraryAccess;    /**< MediaLibraryAccess of BOOL type is used to set On or Off for MediaLibraryAccess in the Idnetification message, True is setting MediaLibraryAccess message.*/
    BOOL VehicleStatus;            /**< VehicleStatus of BOOL type is used to set On or Off for VehicleStatus in the Idnetification message, True is setting VehicleStatus message.*/
    BOOL VoiceOver;                /**< VoiceOver of BOOL type is used to set On or Off for VoiceOver in the Idnetification message, True is setting VoiceOver message.*/
    BOOL WifiInformation;        /**< WifiInformation of BOOL type is used to set On or Off for WifiInformation in the Idnetification message, True is setting WifiInformation message.*/    
}IdentficationSelectArg;    

/*!
 * @struct     NameParam
 * @brief     This Struct is used to save Name IdentificationUpdate message data.\n
 */
 typedef struct {    
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save Name value */
} NameParam;

/*!
 * @struct     ModelIdentifierParam
 * @brief     This Struct is used to save ModelIdentifier IdentificationUpdate message data.\n
 */
 typedef struct {    
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save ModelIdentifier value */
} ModelIdentifierParam;

/*!
 * @struct     ManufacturerParam
 * @brief     This Struct is used to save Manufacturer IdentificationUpdate message data.\n
 */
 typedef struct {    
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save Manufacturer value */
} ManufacturerParam;

/*!
 * @struct     SerialNumberParam
 * @brief     This Struct is used to save SerialNumber IdentificationUpdate message data.\n
 */
 typedef struct {    
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save SerialNumber value */
} SerialNumberParam;

/*!
 * @struct     FirmwareVersionParam
 * @brief     This Struct is used to save FirmwareVersion IdentificationUpdate message data.\n
 */
 typedef struct {    
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save FirmwareVersion value */
} FirmwareVersionParam;

/*!
 * @struct     HardwareVersionParam
 * @brief     This Struct is used to save HardwareVersion IdentificationUpdate message data.\n
 */
 typedef struct {    
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save HardwareVersion value */
} HardwareVersionParam;

/*!
 * @struct     CurrentLanguageParam
 * @brief     This Struct is used to save CurrentLanguage IdentificationUpdate message data.\n
 */
 typedef struct {    
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save CurrentLanguage value */
} CurrentLanguageParam;
 /*!
 * @struct     IdentificationUpdateArg
 * @brief     This Struct is used to save IdentificationUpdate data.\n
 */ 
typedef struct{
    CmdSaveUtf8Param name;                                    /**< Refer @ref NameParam */
    CmdSaveUtf8Param modelIdentifier;            /**< Refer @ref ModelIdentifierParam */
    CmdSaveUtf8Param manufacturer;                    /**< Refer @ref ManufacturerParam */
    CmdSaveUtf8Param serialNumber;                    /**< Refer @ref SerialNumberParam */
    CmdSaveUtf8Param firmwareVersion;            /**< Refer @ref FirmwareVersionParam */
    CmdSaveUtf8Param hardwareVersion;            /**< Refer @ref HardwareVersionParam */
    CmdSaveUtf8Param currentLanguage;            /**< Refer @ref CurrentLanguageParam */    
}IdentificationUpdateArg;

/*!
 * @enum     EAPMatchActionEnum    
 * @brief     This enum value is used to save ExternalAccessoryProtocol MatchAction Param of IdentificationInformation.
 */
typedef enum {
    NO_ACTION = 0,                     /**< NO_ACTION*/
    OPTIONNAL_ACTION = 1,       /**< OPTIONNAL_ACTION*/
    NO_ALERT = 2                          /**< NO_ALERT*/
}EAPMatchActionEnum;

/*!
 * @enum    HIDComponentFunctionEnum    
 * @brief     This enum value is used to set HIDComponentFunction Param of IdentificationInformation.
 */
typedef enum {
    KEYBOARD = 0,                                    /**< Key Board */
    MEDIAPLAYBACKREMOTE = 1,           /**< Media Playback Remote */
    ASSISTIVETOUCHPOINTER = 2,          /**< AssistiveTouch Pointer */
    RESERVED  = 3,                                   /**< Reserved */
    GAMEPAD = 4,                                     /**< Gamepad(form-Fitting) */
    GAMEPAD_NON_FORM_FITTING  = 6,     /**< Gamepad(non form-Fitting)  */
    ASSISTIVESWITCHCONTROL  = 7,        /**< Assistive Switch Control */
    HEADSET = 8                                      /**< Headset */    
}HIDComponentFunctionEnum;

/*!
 * @enum    USBDeviceModeAudioSampleRateEnum    
 * @brief     This enum value is used to set HIDComponentFunction Param of IdentificationInformation.
 */
typedef enum {
    SAMPLELATE_8000 = 0,                  /**< 8000 HZ */
    SAMPLELATE_11025 = 1,                /**< 11025 HZ */
    SAMPLELATE_12000 = 2,                /**< 12000 HZ */
    SAMPLELATE_16000 = 3,               /**< 16000 HZ */
    SAMPLELATE_22050 = 4,                /**< 22050 HZ */
    SAMPLELATE_24000 = 5,                /**< 24000 HZ */
    SAMPLELATE_32000 = 6,               /**< 32000 HZ */
    SAMPLELATE_44100 = 7,               /**< 44100 HZ */
    SAMPLELATE_48000 = 8,                /**< 48000 HZ */    
}USBDeviceModeAudioSampleRateEnum;

/*!
 * @enum    EngineTypesEnum    
 * @brief     This enum value is used to set EngineTypes Param of IdentificationInformation.
 */
typedef enum {
    GASOLINE  = 0,    /**< Gasoline  */
    DIESEL = 1,          /**< Diesel */
    ELECTRIC = 2,      /**< Electric */
    CNG = 3               /**< CNG */    
}EngineTypesEnum;


 /*!
 * @struct     VehicleInformationEngineTypesParam
 * @brief     This Struct is used to save VehicleInformationEngineType data.\n
 */ 
typedef struct{
    BOOL bPresent;                                                          /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    EngineTypesEnum value;  /**< value of EngineTypesEnum type use to save VehicleInformationEngineTypes  */
}VehicleInformationEngineTypesParam;

/*!
 * @struct     IdentficationMsgSelect
 * @brief     This Struct is used to select Identification Message .\n
 */ 
typedef struct {
    CmdSaveUtf8Param supportedLanaguage;                                              /**< supportedLanaguage of **uint8_t type use to save using of supportedLanaguagel   */
    BOOL Power;                                                                        /**< Power of BOOL type use to confirm using of Power Cotrol Message  */
    BOOL Location;                                                                    /**< Location of BOOL type use to confirm using of Location Cotrol Message  */
    CmdSaveUtf8Param LocationInformationName;                  /**< LocationInformationName of BOOL type use to confirm using of Location Cotrol Message  */
    BOOL LocationInformationGPGGA;                                     /**< Location of BOOL type use to confirm using of Location Cotrol Message  */
    BOOL LocationInformationGPRMC;                                     /**< Location of BOOL type use to confirm using of Location Cotrol Message  */
    BOOL LocationInformationGPGSV;                                     /**< Location of BOOL type use to confirm using of Location Cotrol Message  */
    BOOL LocationInformationPASCD;                                     /**< Location of BOOL type use to confirm using of Location Cotrol Message  */
    BOOL LocationInformationPAGCD;                                     /**< Location of BOOL type use to confirm using of Location Cotrol Message  */
    BOOL LocationInformationPAACD;                                     /**< Location of BOOL type use to confirm using of Location Cotrol Message  */
    BOOL LocationInformationGPHDT;                                     /**< Location of BOOL type use to confirm using of Location Cotrol Message  */    
    BOOL NowPlaying;                                    /**< NowPlaying of BOOL type use to confirm using of NowPlaying Cotrol Message  */
    BOOL Communications_CallControls;                    /**< Communications_CallControls of BOOL type use to confirm using of Communications CallControls Cotrol Message  */
    BOOL Communications_SendDTMFUse;                    /**< Communications_SendDTMFUse of BOOL type use to confirm using of Communications SendDTMFUse Cotrol Message  */
    BOOL Communications_CallStateUpdate;                /**< Communications_CallStateUpdate of BOOL type use to confirm using of Communications CallStateUpdate Message  */
    BOOL Communications_CommunicationsUpdate;            /**< Communications_CommunicationsUpdate of BOOL type use to confirm using of Communications CommunicationsUpdate Message  */
    BOOL Communications_ListUpdates;                    /**< Communications_ListUpdates of BOOL type use to confirm using of Communications ListUpdates Message  */
    BOOL HIDiAP2SessionMediaPlaybackRemote;                /**< HIDiAP2SessionMediaPlaybackRemote of BOOL type use to confirm using of HIDiAP2Session MediaPlaybackRemote Message  */
    BOOL HIDiAP2SessionAssistiveSwitchControl;            /**< HIDiAP2SessionAssistiveSwitchControl of BOOL type use to confirm using of HIDiAP2Session AssistiveSwitchControl Message  */
    BOOL HIDiAP2SessionAssistiveTouchPointer;            /**< HIDiAP2SessionAssistiveTouchPointer of BOOL type use to confirm using of HIDiAP2Session AssistiveTouchPointer Message  */
    BOOL HIDiAP2SessionGameControllerExtened;            /**< HIDiAP2SessionGameControllerExtened of BOOL type use to confirm using of HIDiAP2Session GameControllerExtened Message  */
    BOOL HIDiAP2SessionAssistiveTouchStandard;            /**< HIDiAP2SessionAssistiveTouchStandard of BOOL type use to confirm using of HIDiAP2Session AssistiveTouchStandard Message  */
    BOOL HIDiAP2SessionAssistiveHeadPhone;                /**< HIDiAP2SessionAssistiveHeadPhone of BOOL type use to confirm using of HIDiAP2Session AssistiveHeadPhone Message  */
    BOOL HIDiAP2SessionKeyBoard;                        /**< HIDiAP2SessionKeyBoard of BOOL type use to confirm using of HIDiAP2Session KeyBoard Message  */
    BOOL MediaLibraryInformation;                        /**< MediaLibraryInformation of BOOL type use to confirm using of MediaLibraryInformation Message  */
    BOOL MediaLibraryUpdates;                            /**< MediaLibraryUpdates of BOOL type use to confirm using of MediaLibraryUpdates Message  */
    BOOL MediaLibraryPlayback;                            /**< MediaLibraryPlayback of BOOL type use to confirm using of MediaLibraryPlayback Message  */
    BOOL DeviceInformationUpdate;                        /**< DeviceInformationUpdate of BOOL type use to confirm using of DeviceInformationUpdate Message  */
    BOOL DeviceLanguageUpdate;                            /**< DeviceLanguageUpdate of BOOL type use to confirm using of DeviceLanguageUpdate Message  */
    BOOL DeviceTimeUpdate;                                /**< DeviceTimeUpdate of BOOL type use to confirm using of DeviceTimeUpdate Message  */
    BOOL DeviceUUIDUpdate;                                /**< DeviceUUIDUpdate of BOOL type use to confirm using of DeviceUUIDUpdate Message  */
    BOOL AppLaunch;                                        /**< AppLaunch of BOOL type use to confirm using of AppLaunch Message  */
    BOOL AssistiveTouch;                                /**< AssistiveTouch of BOOL type use to confirm using of AssistiveTouch Message  */
    BOOL BluetoothConnection;                            /**< BluetoothConnection of BOOL type use to confirm using of BluetoothConnection Message  */
    BOOL EAiAP2Session;                                    /**< EAiAP2Session of BOOL type use to confirm using of EAiAP2Session Message  */
    BOOL EAiAP2Session_MultipleSession;                    /**< EAiAP2Session_MultipleSession of BOOL type use to confirm using of EAiAP2Session_MultipleSession Message  */
    CmdSaveUtf8Param  ExternalAccessoryProtocolName;     /**< ExternalAccessoryProtocolName of CmdSaveUtf8Param type use to save ExternalAccessoryProtocolName of IdentificationInformation Message*/    
    EAPMatchActionEnum ExternalAccessoryProtocolMatchAction;  /**< ExternalAccessoryProtocolMatchAction of EAPMatchActionEnum type use to save ExternalAccessoryProtocolMatchAction of IdentificationInformation Message  */
    CmdSaveUtf8Param AppMatchTeamID;
    BOOL UseEAWithCarPlay;                                   /**< UseEAWithCarPlay of BOOL type use to confirm using of  EAP with CarPlay  */
    BOOL EANativeTransPort;                                /**< EANativeTransPort of BOOL type use to confirm using of  HIDiAP2Session AssistiveHeadPhone Message  */
    BOOL HIDNativeTransportMediaPlaybackRemote;            /**< HIDNativeTransportMediaPlaybackRemote of BOOL type use to confirm using of HIDNativeTransport MediaPlaybackRemote Message  */
    BOOL HIDNativeTransportAssistiveSwitchControl;        /**< HIDNativeTransportAssistiveSwitchControl of BOOL type use to confirm using of HIDNativeTransport AssistiveSwitchControl Message  */
    BOOL HIDNativeTransportAssistiveTouchPointer;        /**< HIDNativeTransportAssistiveTouchPointer of BOOL type use to confirm using of HIDNativeTransport AssistiveTouchPointer Message  */
    BOOL HIDNativeTransportGameControllerExtened;        /**< HIDNativeTransportGameControllerExtened of BOOL type use to confirm using of HIDNativeTransport GameControllerExtened Message  */
    BOOL HIDNativeTransportAssistiveTouchStandard;        /**< HIDNativeTransportAssistiveTouchStandard of BOOL type use to confirm using of HIDNativeTransport AssistiveTouchStandard Message  */
    BOOL HIDNativeTransportAssistiveHeadPhone;            /**< HIDNativeTransportAssistiveHeadPhone of BOOL type use to confirm using of  HIDNativeTransport AssistiveHeadPhone Message  */
    BOOL HIDNativeTransportKeyBoard;                    /**< HIDNativeTransportKeyBoard of BOOL type use to confirm using of HIDNativeTransport KeyBoard Message  */
    BOOL VehicleStatus;                                    /**< VehicleStatus of BOOL type use to confirm using of VehicleStatus Message  */
    CmdSaveUtf8Param VehicleStatusComponentName;    /**< VehicleStatusComponentName of CmdSaveUtf8Param type use to save VehicleStatusComponentName  */
    BOOL VehicleStatusRange;                                         /**< VehicleStatusRange of BOOL type use to confirm using of VehicleStatusRange  */
    BOOL VehicleStatusOutSideTemperature;                                         /**< VehicleStatusOutSideTemperature of BOOL type use to confirm using of VehicleStatusOutSideTemperature  */
    BOOL VehicleStatusRangeWarning;                                         /**< VehicleStatusRangeWarning of BOOL type use to confirm using of VehicleStatusRangeWarning  */    
    BOOL VehicleInformation;                                    /**< VehicleInformation of BOOL type use to confirm using of VehicleInformationComponent  */
    CmdSaveUtf8Param VehicleInformationComponentName;  /**< VehicleInformationComponentName of CmdSaveUtf8Param type use to save VehicleInformationComponentName  */
    VehicleInformationEngineTypesParam VehicleInformationEngineTypes;  /**< VehicleInformationEngineTypes of VehicleInformationEngineTypesParam type use to save VehicleInformationEngineTypes  */
    CmdSaveUtf8Param VehicleInformationComponentDisplayName;  /**< VehicleInformationComponentDisplayName of CmdSaveUtf8Param type use to save VehicleInformationComponentDisplayName  */
    BOOL VoiceOver;                                        /**< VoiceOver of BOOL type use to confirm using of VoiceOver Message  */
    BOOL WiFiInformationSharing;                        /**< WiFiInformationSharing of BOOL type use to confirm using of WiFiInformationSharing Message  */
    BOOL USBDigitalAudioDeviceMode;                        /**< USBDigitalAudioDeviceMode of BOOL type use to confirm using of USBDigitalAudioDeviceMode Message  */
    BOOL USBDigitalAudioHostMode;                        /**< USBDigitalAudioHostMode of BOOL type use to confirm using of USBDigitalAudioHostMode Message  */
    BOOL DeviceAuthentication;                            /**< DeviceAuthentication of BOOL type use to confirm using of DeviceAuthentication Message  */
    BOOL DevicePowered;                                    /**< DevicePowered of BOOL type use to confirm using of DevicePowered Message  */
    CmdSaveUtf8Param  USBDeviceTransportComponentName;  /**< USBDeviceTransportComponentName of uint8_t* type use to save USBDeviceTransportComponentName  */
    BOOL USBDeviceSurpportedAudioSampleRate8000Hz;              /**< USBDeviceSurpportedAudioSampleRate8000Hz of BOOL type use to confirm using of USBDeviceSurpportedAudioSampleRate8000Hz  */
    BOOL USBDeviceSurpportedAudioSampleRate11025Hz;            /**< USBDeviceSurpportedAudioSampleRate11025Hz of BOOL type use to confirm using of USBDeviceSurpportedAudioSampleRate11025Hz  */
    BOOL USBDeviceSurpportedAudioSampleRate12000Hz;            /**< USBDeviceSurpportedAudioSampleRate12000Hz of BOOL type use to confirm using of USBDeviceSurpportedAudioSampleRate12000Hz  */
    BOOL USBDeviceSurpportedAudioSampleRate16000Hz;            /**< USBDeviceSurpportedAudioSampleRate16000Hz of BOOL type use to confirm using of USBDeviceSurpportedAudioSampleRate16000Hz  */
    BOOL USBDeviceSurpportedAudioSampleRate22050Hz;            /**< USBDeviceSurpportedAudioSampleRate22050Hz of BOOL type use to confirm using of USBDeviceSurpportedAudioSampleRate22050Hz  */
    BOOL USBDeviceSurpportedAudioSampleRate24000Hz;            /**< USBDeviceSurpportedAudioSampleRate24000Hz of BOOL type use to confirm using of USBDeviceSurpportedAudioSampleRate24000Hz  */
    BOOL USBDeviceSurpportedAudioSampleRate32000Hz;            /**< USBDeviceSurpportedAudioSampleRate32000Hz of BOOL type use to confirm using of USBDeviceSurpportedAudioSampleRate32000Hz  */
    BOOL USBDeviceSurpportedAudioSampleRate44100Hz;            /**< USBDeviceSurpportedAudioSampleRate44100Hz of BOOL type use to confirm using of USBDeviceSurpportedAudioSampleRate44100Hz  */
    BOOL USBDeviceSurpportedAudioSampleRate48000Hz;            /**< USBDeviceSurpportedAudioSampleRate48000Hz of BOOL type use to confirm using of USBDeviceSurpportedAudioSampleRate48000Hz  */
    CmdSaveUtf8Param  USBHostTransportComponentName;                             /**< USBHostTransportComponentName of uint8_t* type use to save USBHostTransportComponentName  */
    CmdSaveUtf8Param  SerialTransportComponentName;                                 /**< SerialTransportComponentName of uint8_t* type use to save SerialTransportComponentName  */
    CmdSaveUtf8Param  BluetoothTransportComponentName;                           /**< BluetoothTransportComponentName of uint8_t* type use to save BluetoothTransportComponentName  */
    CmdSaveUtf8Param  BluetoothTransportMediaAccessControlAddress;         /**< BluetoothTransportMediaAccessControlAddress of uint8_t* type use to save BluetoothTransportMediaAccessControlAddress  */
    CmdSaveUtf8Param  iAP2HIDComponentName;                                             /**< iAP2HIDComponentName of uint8_t* type use to save iAP2HIDComponentName  */
    HIDComponentFunctionEnum  iAP2HIDComponentFunction;           /**< iAP2HIDComponentFunction of HIDComponentFunctionEnum type use to save iAP2HIDComponentFunction  */
    CmdSaveUtf8Param WirelessCarPlayTransPortComponentName;                /**< WirelessCarPlayTransPortComponentName of uint8_t* type use to save WirelessCarPlayTransPortComponentName  */
    CmdSaveUtf8Param  BluetoothHIDComponentName;                                     /**< BluetoothHIDComponentName of uint8_t* type use to save BluetoothHIDComponentName  */
    HIDComponentFunctionEnum  BluetoothHIDComponentFunction;   /**< BluetoothHIDComponentFunction of HIDComponentFunctionEnum type use to save BluetoothHIDComponentFunction  */
}IdentficationMsgSelectArg; 

/*! @} */

/*!
 * @ingroup Communications
 * @{
 */

/*!
 * @struct     StartCallStateUpdatesArg
 * @brief     This Struct is used to save payload data of StartCallStateUpdates Message. \n
 */
typedef struct {
    //BOOL bRemoteID;                    /**< bRemoteID of BOOL type is used to request RemoteID data from apple device */
    //BOOL bDisplayName;                /**< bDisplayName of BOOL type is used to request DisplayName data from apple device */
    //BOOL bStatus;                    /**< bStatus of BOOL type is used to request Status data from apple device */
    //BOOL bDirection;                /**< bDirection of BOOL type is used to request Direction data from apple device */
    //BOOL bCallUUID;                    /**< bCallUUID of BOOL type is used to request CallUUID data from apple device */
    BOOL bAddressBookID;            /**< bAddressBookID of BOOL type is used to request AddressBookID data from apple device */
    BOOL bLabel;                    /**< bLabel of BOOL type is used to request Label data from apple device */
    //BOOL bService;                    /**< bService of BOOL type is used to request Service data from apple device */
    //BOOL bIsConferenced;            /**< bIsConferenced of BOOL type is used to request IsConferenced data from apple device */
    //BOOL bConferenceGroup;            /**< bConferenceGroup of BOOL type is used to request ConferenceGroup data from apple device */
    BOOL bDisconnectReason;            /**< bDisconnectReason of BOOL type is used to request DisconnectReason data from apple device */    
} StartCallStateUpdatesArg;

/*!
 * @struct     StartCommunicationsUpdatesArg
 * @brief     This Struct is used to save payload data of StartCommunicationsUpdates Message. \n
 */
typedef struct {    
    BOOL bSignalStrength;                /**< bSignalStrength of BOOL type is used to request SignalStrength data from apple device */    
    BOOL bRegistrationStatus;            /**< bRegistrationStatus of BOOL type is used to request RegistrationStatus data from apple device */    
    BOOL bAirplaneModeStatus;            /**< bAirplaneModeStatus of BOOL type is used to request AirplaneModeStatus data from apple device */    
    BOOL bCarrierName;                    /**< bCarrierName of BOOL type is used to request CarrierName data from apple device */    
    BOOL bCelluarSupported;                /**< bCelluarSupported of BOOL type is used to request CelluarSupported data from apple device */    
    BOOL bTelephonyEnabled;                /**< bTelephonyEnabled of BOOL type is used to request TelephonyEnabled data from apple device */    
    BOOL bFaceTimeAudioEnabled;            /**< bFaceTimeAudioEnabled of BOOL type is used to request FaceTimeAudioEnabled data from apple device */    
    BOOL bFaceTimeVideoEnabled;            /**< bFaceTimeVideoEnabled of BOOL type is used to request FaceTimeVideoEnabled data from apple device */    
    BOOL bMuteStatus;                    /**< bMuteStatus of BOOL type is used to request MuteStatus data from apple device */    
    BOOL bCurrentCallCount;                /**< bCurrentCallCount of BOOL type is used to request CurrentCallCount data from apple device */    
    BOOL bNewVoicemailCount;            /**< bNewVoicemailCount of BOOL type is used to request NewVoicemailCount data from apple device */    
    BOOL bInitiateCallAvailable;        /**< bInitiateCallAvailable of BOOL type is used to request InitiateCallAvailable data from apple device */    
    BOOL bEndAndAccptAvailable;            /**< bEndAndAccptAvailable of BOOL type is used to request EndAndAccptAvailable data from apple device */    
    BOOL bHoldAndAcceptAvailable;        /**< bHoldAndAcceptAvailable of BOOL type is used to request HoldAndAcceptAvailable data from apple device */    
    BOOL bSwapAvailable;                /**< bSwapAvailable of BOOL type is used to request SwapAvailable data from apple device */    
    BOOL bMergeAvailable;                /**< bMergeAvailable of BOOL type is used to request MergeAvailable data from apple device */    
    BOOL bHoldAvailable;                /**< bHoldAvailable of BOOL type is used to request HoldAvailable data from apple device */        
} StartCommunicationsUpdatesArg;


/*!
 * @enum     InitiateCallType    
 * @brief     This enum value is used to InitiateCallType.
 */
typedef enum {
    DESTINATION = 0,             /**< Destination */
    VOICEMAIL = 1,               /**< Voicemail */
    REDIAL = 2                    /**< Redial */
}InitiateCallType;

/*!
 * @enum     InitiateCallService    
 * @brief     This enum value is used to InitiateCallService.
 */
typedef enum {
    ICS_TELEPHONY = 1,                     /**< TELEPHONY */
    ICS_FACETIMEAUDIO = 2,               /**< FACETIMEAUDIO */
    ICS_FACETIMEVIDEO = 3                /**< FACETIMEVIDEO */
}InitiateCallService;

/*!
 * @struct     ServiceParam
 * @brief     This Struct is used to save Service of InitiateCall message.\n
 */
 typedef struct {    
    BOOL bPresent;                /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    InitiateCallService value;    /**< value of InitiateCallService type is used to save Service value */
} ServiceParam;


/*!
 * @struct     InitiateCallArg
 * @brief     This Struct is used to save payload data of InitiateCall Message. \n
 */
typedef struct {    
    InitiateCallType eType;                    /**< eType of BOOL type is used to request eType data from apple device */    
    CmdSaveUtf8Param mDestinationID;            /**< mDestinationID of SaveUtf8Param type is used to save DestinationID data from apple device */    
    ServiceParam mService;                    /**< mService of ServiceParam type is used to save Service data from apple device */    
    CmdSaveUtf8Param mAddressBookID;            /**< mDestinationID of SaveUtf8Param type is used to save DestinationID data from apple device */
} InitiateCallArg;


/*!
 * @enum     AcceptActionEnum    
 * @brief     This enum value is used to AcceptAction.
 */
typedef enum {
    AA_ACCEPT = 0,                     /**< Accept / Hold and Accept */
    AA_ENDANDACCEPT = 1               /**< End And Accept */    
}AcceptActionEnum;

/*!
 * @struct     AcceptCallArg
 * @brief     This Struct is used to save payload data of AcceptCall Message. \n
 */
typedef struct {    
    AcceptActionEnum eAcceptAction;            /**< eAcceptAction of AcceptActionEnum type is used to request AcceptAction data from apple device */    
    CmdSaveUtf8Param mCallUUID;            /**< mDCallUUID of CmdSaveUtf8Param type is used to save CallUUID data from apple device */        
} AcceptCallArg;


/*!
 * @enum     EndActionEnum    
 * @brief     This enum value is used to AcceptAction.
 */
typedef enum {
    EA_END = 0,                     /**< End/Decline */
    EA_ENDALL = 1                   /**< EndAll */    
}EndActionEnum;


/*!
 * @struct     EndCallArg
 * @brief     This Struct is used to save payload data of EndCall Message. \n
 */
typedef struct {    
    EndActionEnum eEndAction;            /**< eEndAction of EndActionEnum type is used to request EndActionEnum data from apple device */    
    CmdSaveUtf8Param mCallUUID;            /**< mDCallUUID of CmdSaveUtf8Param type is used to save CallUUID data from apple device */        
} EndCallArg;

/*!
 * @struct     HoldStatusUpdateArg
 * @brief     This Struct is used to save payload data of HoldStatusUpdate Message. \n
 */
typedef struct {    
    BOOL bHoldStatus;                    /**< bHoldStatus of BOOL type is used to save HoldStatus data from apple device */    
    CmdSaveUtf8Param mCallUUID;            /**< mDCallUUID of CmdSaveUtf8Param type is used to save CallUUID data from apple device */        
} HoldStatusUpdateArg;

/*!
 * @struct     MuteStatusUpdateArg
 * @brief     This Struct is used to save payload data of MuteStatusUpdate Message. \n
 */
typedef struct {    
    BOOL bMuteStatus;                    /**< bMuteStatus of BOOL type is used to save MuteStatus data from apple device */        
} MuteStatusUpdateArg;

/*!
 * @enum     EndActionEnum    
 * @brief     This enum value is used to AcceptAction.
 */
typedef enum {
    DTMF_NUMBER_0 = 0,                     /**< Number 0 */
    DTMF_NUMBER_1 = 1,                     /**< Number 1 */
    DTMF_NUMBER_2 = 2,                     /**< Number 2 */
    DTMF_NUMBER_3 = 3,                     /**< Number 3 */
    DTMF_NUMBER_4 = 4,                     /**< Number 4 */
    DTMF_NUMBER_5 = 5,                     /**< Number 5 */    
    DTMF_NUMBER_6 = 6,                     /**< Number 6 */    
    DTMF_NUMBER_7 = 7,                     /**< Number 7 */    
    DTMF_NUMBER_8 = 8,                     /**< Number 8 */    
    DTMF_NUMBER_9 = 9,                     /**< Number 9 */    
    DTMF_NUMBER_10 = 10,                 /**< Star(*) */    
    DTMF_NUMBER_11 = 11,                 /**< Pound(#) */        
}SendDTMFToneEnum;


/*!
 * @struct     SendDTMFArg
 * @brief     This Struct is used to save payload data of SendDTMF Message. \n
 */
typedef struct {    
    SendDTMFToneEnum eTone;                    /**< eTone of SendDTMFToneEnum type is used to save Tone data from apple device */    
    CmdSaveUtf8Param mCallUUID;                /**< mDCallUUID of CmdSaveUtf8Param type is used to save CallUUID data from apple device */        
} SendDTMFArg;


/*!
 * @struct     RecentsListProPertiesGroup
 * @brief     This Struct is used to save data of RecentsListProPertiesGroup message.\n
 */
 typedef struct {        
     BOOL bPresent;                            /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/    
    //BOOL bIndex;                            /**< bIndex of BOOL type is used to save Index value */
    //BOOL bRemoteID;                            /**< bRemoteID of BOOL type is used to save RemoteID value */    
    //BOOL bDisplayName;                        /**< bDisplayName of BOOL type is used to save DisplayName value */    
    BOOL bLabel;                            /**< bLabel of BOOL type is used to save Label value */    
    BOOL bAdressBookID;                        /**< bAdressBookID of BOOL type is used to save AdressBookID value */
    //BOOL bService;                            /**< bService of BOOL type is used to save Service value */
    //BOOL bType;                            /**< bType of BOOL type is used to save Type value */
    BOOL bUnixTimestamp;                    /**< bUnixTimestamp of BOOL type is used to save UnixTimestamp value */
    BOOL bDuration;                            /**< bDuration of BOOL type is used to save Service Duration */
    //BOOL bOccurrences;                        /**< bOccurrences of BOOL type is used to save Occurrences value */    
} RecentsListPropertiesGroup;

/*!
 * @struct     RecentsListProPertiesGroup
 * @brief     This Struct is used to save data of RecentsListProPertiesGroup message.\n
 */
 typedef struct {        
     BOOL bPresent;                            /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/    
    //BOOL bIndex;                            /**< bIndex of BOOL type is used to save Index value */
    //BOOL bRemoteID;                            /**< bRemoteID of BOOL type is used to save RemoteID value */    
    //BOOL bDisplayName;                        /**< bDisplayName of BOOL type is used to save DisplayName value */    
    BOOL bLabel;                            /**< bLabel of BOOL type is used to save Label value */    
    BOOL bAdressBookID;                        /**< bAdressBookID of BOOL type is used to save AdressBookID value */
    //BOOL bService;                            /**< bService of BOOL type is used to save Service value */
} FavoritesListProertiesGroup;


/*!
 * @struct     SaveCmdUint16Param
 * @brief     This Struct is used to save uint16_t data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/    
    uint16_t value;    /**< value of uint16_t type is used to save uint16_t data */
}SaveCmdUint16Param;

/*!
 * @struct     StartListUpdateArg
 * @brief     This Struct is used to save payload data of StartListUpdate Message. \n
 */
typedef struct {    
    RecentsListPropertiesGroup mRecenttsListPropoerties;        /**< mRecenttsListPropoerties of RecentsListProPertiesGroup type is used to save RecentsListProPerties Group data*/    
    SaveCmdUint16Param mRecentsListMax;                            /**< mRecentsListMax of SaveUint16Param type is used to save RecentsListMax data*/        
    BOOL bRecentsListCombine;                                    /**< bRecentsListCombine of BOOL type is used to save RecentsListCombine data*/        
    FavoritesListProertiesGroup mFavoritesListProerties;        /**< mFavoritesListProerties of FavoritesListProertiesGroup type is used to save FavoritesListProerties Group data  */    
    SaveCmdUint16Param mFavoritesListMax;                        /**< mFavoritesListMax of SaveUint16Param type is used to save FavoritesListMax data*/        
} StartListUpdatesArg;
/*! @} */

/*!
 * @ingroup ExternalAccessoryProtocol
 * @{
 */

/*!
 * @enum     ExternalAccessoryProtocolSessionStatusEnum    
 * @brief     This enum value is used to ExternalAccessoryProtocolSessionStatus.
 */
typedef enum {
    SessionStatusOK = 0,                     /**< Session OK*/
    SessionClose    = 1                        /**< Session Close*/
}ExternalAccessoryProtocolSessionStatusEnum;


/*!
 * @struct     StatusExternalAccessoryProtocolSessionArg
 * @brief     This Struct is used to save payload data of StatusExternalAccessoryProtocolSession Message. \n
 */
typedef struct {    
    uint16_t nExternalAccessroyProtocolSessionIdentifier;                                    /**< nExternalAccessroyProtocolSessionIdentifier of uint16_t type is used to save ExternalAccessroyProtocolSessionIdentifier data*/        
    ExternalAccessoryProtocolSessionStatusEnum eExternalAccessoryProtocolSessionStatus;     /**< eExternalAccessoryProtocolSessionStatus of ExternalAccessoryProtocolSessionStatusEnum type is used to save ExternalAccessoryProtocolSessionStatus data*/        
} StatusExternalAccessoryProtocolSessionArg;

/*!
 * @struct     StatusExternalAccessoryProtocolSessionArg
 * @brief     This Struct is used to save payload data of StatusExternalAccessoryProtocolSession Message. \n
 */
typedef struct {    
    uint32_t size;        /**< size of uint32_t type is used to save ExternalAccessoryProtocolData data size*/        
    uint8_t* value;     /**< value of uint8_t* type is used to save ExternalAccessoryProtocolData data*/
    uint16_t nExternalAccessroyProtocolSessionIdentifier; /**< nExternalAccessroyProtocolSessionIdentifier of uint16_t type is used to save Sending ExternalAccessroyProtocolSessionIdentifier */
} ExternalAccessoryProtocolDataArg;
/*! @} */


/*!
 * @ingroup FileTransfer
 * @{
 */
  /*!
    * @enum     FileTransferCMD    
    * @brief     This enum value is used to FileTransferCMD.
    * @enum     ExternalAccessoryProtocolSessionStatusEnum
    * @brief     This enum value is used to ExternalAccessoryProtocolSessionStatus.
    */
 typedef enum {
    FT_NONE = 0,                     /**< NONE */    
    FT_CANCEL = 1,                     /**< CANCEL */    
    FT_PAUSE = 2,                     /**< PAUSE */    
    FT_FAILURE = 3,                  /**< FAILURE */    
    FT_RESUME = 4                      /**< FT_RESUME */    
}FileTransferCMD;

/*! @} */

/*!
 * @defgroup iAP2Commandvalue
 * @brief iAP2 Command type and Comand list (Command is used to confirm message type.)
 * @{
 */
//===========================================================================================================================
// Commands
//===========================================================================================================================
#define kCommandBase                                        (10000)                        /**< Base of iAP2 Command value */
#define kInternalCommandBase                                (kCommandBase+1000)            /**< Base of iAP2 Internal Command value */
#define kExternalCommandBase                                (kCommandBase+2000)            /**< Base of iAP2 External Command value */

// Connection Data
#define kCmdGetInitConnectionData                            (kInternalCommandBase+1)    /**< value of GetInitConnectionData command */


//Carplay OnOff
#define kCmdCarplayOnOff                                     (kInternalCommandBase+11)    /**< value of CarplayOnOff  command */    

//IdentificationSelect
#define kCmdIdentificationSelect                             (kInternalCommandBase+22)    /**< value of IdentificationSelect command */    

//UsbModeChange
#define kCmdUsbModeChange                                     (kInternalCommandBase+31)     /**< value of UsbModeChange command */    

//CarPlayClientState
#define kCmdCarPlayClientState                                 (kInternalCommandBase+41)     /**< value of CarPlayClientState command */    


//StartCarPlay
#define kCmdStartCarPlay                                     (kInternalCommandBase+51)     /**< value of StartCarPlay command */    

//TerminateDaemon
#define kCmdTerminateDaemon                                    (kInternalCommandBase+61)     /**< value of TerminateDaemon command */

// Power
#define kCmdStartPowerUpdates                                (kExternalCommandBase+1)    /**< value of StartPowerUpdates command */    
#define kCmdStopPowerUpdates                                (kExternalCommandBase+2)    /**< value of StopPowerUpdates command */    
#define kCmdPowerSourceUpdate                                (kExternalCommandBase+3)    /**< value of kCmdPowerSourceUpdate command */

// Now Playing
#define kCmdStartNowPlayingUpdates                            (kExternalCommandBase+11)     /**< value of StartNowPlayingUpdates command */    
#define kCmdStopNowPlayingUpdates                            (kExternalCommandBase+12)    /**< value of StopNowPlayingUpdates command */    
#define kCmdSetNowPlayingInformation                        (kExternalCommandBase+13)    /**< value of kCmdSetNowPlayingInformation command */

// Vehicle Status
#define kCmdVehicleStatusUpdate                                (kExternalCommandBase+21)    /**< value of VehicleStatusUpdate command */    

// Location
#define kCmdLocationInformation                                (kExternalCommandBase+31)    /**< value of LocationInformation command */    

// Device Notification

// Telephony
#define kCmdStartTelephonyCallStateInformation                (kExternalCommandBase+51)    /**< value of StartTelephonyCallStateInformation command */    
#define kCmdStopTelephonyCallStateInformation                (kExternalCommandBase+52)    /**< value of StopTelephonyCallStateInformation command */    
#define kCmdStartTelephonyUpdates                            (kExternalCommandBase+53)    /**< value of StartTelephonyUpdates command */    
#define kCmdStopTelephonyUpdates                            (kExternalCommandBase+54)    /**< value of StopTelephonyUpdates command */    

// Bluetooth Connection
#define kCmdBTComponentInformation                            (kExternalCommandBase+61)    /**< value of BTComponentInformation command */    
#define kCmdStartBTConnectionUpdates                        (kExternalCommandBase+62)    /**< value of StartBTConnectionUpdates command */    
#define kCmdStopBTConnectionUpdates                            (kExternalCommandBase+63)    /**< value of StopBTConnectionUpdates command */    

// ADD yongil 2014.10.23

// HID 
#define kCmdStartHID            (kExternalCommandBase+71)                                /**< value of StartHID command */    
#define kCmdAccessoryHIDReport            (kExternalCommandBase+72)                        /**< value of AccessoryHIDReport command */    
#define kCmdStopHID            (kExternalCommandBase+73)                                    /**< value of StopHID command */    

// MediaLibraryInformation
#define kCmdStartMediaLibraryInformaion            (kExternalCommandBase+81)                /**< value of StartMediaLibraryInformaion command */    
#define kCmdStopMediaLibraryInformaion            (kExternalCommandBase+82)                /**< value of StopMediaLibraryInformaion command */    

// MediaLibraryInformation        
#define kCmdStartMediaLibraryUpdates        (kExternalCommandBase+91)                    /**< value of StartMediaLibraryUpdates command */    
#define kCmdStopMediaLibraryUpdates            (kExternalCommandBase+92)                    /**< value of StopMediaLibraryUpdates command */    

// RequestAppLaunch
#define kCmdRequestAppLaunch        (kExternalCommandBase+101)                            /**< value of RequestAppLaunch command */    

// AssistiveTouch
#define kCmdStartAssistiveTouch    (kExternalCommandBase+111)                                /**< value of StartAssistiveTouch command */    
#define kCmdStopAssistiveTouch    (kExternalCommandBase+112)                                /**< value of StopAssistiveTouch command */    
#define kCmdStartAssistiveTouchInformation    (kExternalCommandBase+113)                    /**< value of StartAssistiveTouchInformation command */    
#define kCmdStopAssistiveTouchInformation    (kExternalCommandBase+114)                    /**< value of StopAssistiveTouchInformation command */        


// USBDeviceModeAudio
#define kCmdStartUSBDeviceModeAudio    (kExternalCommandBase+121)                            /**< value of StartUSBDeviceModeAudio command */    
#define kCmdStopUSBDeviceModeAudio    (kExternalCommandBase+122)                            /**< value of StopUSBDeviceModeAudio command */    

// VoiceOver
#define kCmdStartVoiceOver        (kExternalCommandBase+131)                                /**< value of StartVoiceOver command */    
#define kCmdStopVoiceOver         (kExternalCommandBase+132)                                /**< value of StopVoiceOver command */    
#define kCmdRequestVoiceOverMoveCursor        (kExternalCommandBase+133)                    /**< value of RequestVoiceOverMoveCursor command */    
#define kCmdRequestVoiceOverActivateCursor        (kExternalCommandBase+134)                /**< value of RequestVoiceOverActivateCursor command */    
#define kCmdRequestVoiceOverScrollPage        (kExternalCommandBase+135)                    /**< value of RequestVoiceOverScrollPage command */        
#define kCmdRequestVoiceOverSpeakText        (kExternalCommandBase+136)                    /**< value of RequestVoiceOverSpeakText command */    
#define kCmdRequestVoiceOverPauseText        (kExternalCommandBase+137)                    /**< value of RequestVoiceOverPauseText command */    
#define kCmdRequestVoiceOverResumeText        (kExternalCommandBase+138)                    /**< value of RequestVoiceOverResumeText command */    
#define kCmdStartVoiceOverUpdates        (kExternalCommandBase+139)                        /**< value of StartVoiceOverUpdates command */        
#define kCmdStopVoiceOverUpdates        (kExternalCommandBase+140)                        /**< value of StopVoiceOverUpdates command */    
#define kCmdRequestVoiceOverConfiguration        (kExternalCommandBase+141)                /**< value of RequestVoiceOverConfiguration command */    
#define kCmdStartVoiceOverCursorUpdates        (kExternalCommandBase+142)                    /**< value of StartVoiceOverCursorUpdates command */    
#define kCmdStopVoiceOverCursorUpdates        (kExternalCommandBase+143)                    /**< value of StopVoiceOverCursorUpdates command */    

// RequestWiFiInformaion
#define kCmdRequestWiFiInformation        (kExternalCommandBase+151)                        /**< value of RequestWiFiInformation command */    


// PlayMediaLibrary
#define kCmdPlayMediaLibraryCurrentSelection        (kExternalCommandBase+161)            /**< value of PlayMediaLibraryCurrentSelection command */    
#define kCmdPlayMediaLibraryItems        (kExternalCommandBase+162)                        /**< value of PlayMediaLibraryItems command */    
#define kCmdPlayMediaLibraryCollection        (kExternalCommandBase+163)                    /**< value of PlayMediaLibraryCollection command */    
#define kCmdPlayMediaLibrarySpecial        (kExternalCommandBase+164)                        /**< value of kCmdPlayMediaLibrarySpecial command */    

//ExternalAccessoryProtocol
#define kCmdStatusExternalAccessoryProtocolSession         (kExternalCommandBase+171)         /**< value of StatusExternalAccessoryProtocolSession command */    
#define kCmdExternalAccessoryProtocolData                 (kExternalCommandBase+172)         /**< value of ExternalAccessoryProtocolData command */    

//IdentificaitonUpdate
#define kCmdIdentificationUpdate         (kExternalCommandBase+181)                         /**< value of IdentificationUpdate command */    


// Communications
#define kCmdStartCallStateUpdates                    (kExternalCommandBase+191)                /**< value of StartCallStateUpdates command */
#define kCmdStopCallStateUpdates                    (kExternalCommandBase+192)                /**< value of StopCallStateUpdates command */
#define kCmdStartCommunicationsUpdates                (kExternalCommandBase+193)                /**< value of StartCommunicationsUpdates command */
#define kCmdStopCommunicationsUpdates                (kExternalCommandBase+194)                /**< value of StopCommunicationsUpdates command */
#define kCmdInitiateCall                                (kExternalCommandBase+195)                /**< value of InitiateCall command */
#define kCmdAcceptCall                                (kExternalCommandBase+196)                /**< value of AcceptCall command */
#define kCmdEndCall                                    (kExternalCommandBase+197)                /**< value of EndCall command */
#define kCmdSwapCalls                                (kExternalCommandBase+198)                /**< value of SwapCalls command */
#define kCmdMergeCalls                                (kExternalCommandBase+199)                /**< value of MergeCalls command */
#define kCmdHoldStatusUpdate                        (kExternalCommandBase+200)                /**< value of HoldStatusUpdate command */
#define kCmdMuteStatusUpdate                        (kExternalCommandBase+201)                /**< value of MuteStatusUpdate command */
#define kCmdSendDTMF                                (kExternalCommandBase+202)                /**< value of SendDTMF command */
#define kCmdStartListUpdates                        (kExternalCommandBase+203)                /**< value of StartListUpdates command */
#define kCmdStopListUpdates                            (kExternalCommandBase+204)                /**< value of StopListUpdates command */

/*! @} */
#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKEXTERNAL_COMMAND_H_ */

