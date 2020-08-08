/*
    File:       iAp2AppFrameworkExternal_event.h
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

#ifndef IAP2APPFRAMEWORKEXTERNAL_EVENT_H_
#define IAP2APPFRAMEWORKEXTERNAL_EVENT_H_

#include <stdint.h>
#include <stddef.h>

#include "iAp2AppFrameworkCommon.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ---------------------------------------------- Event -------------------------------------*/
/*!
 * @defgroup DataParam
 * @brief Group of data type is save each type param 
 * @{
 */
/*!
 * @struct     EvtSaveBoolParam
 * @brief     This Struct is used to save utf8 data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    BOOL value;        /**< size of BOOL type is used to save size of value param */
}EvtSaveBoolParam;

/*!
 * @struct     EvtSaveUtf8Param
 * @brief     This Struct is used to save utf8 data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save utf8 data */
}EvtSaveUtf8Param;

/*!
 * @struct     EvtSaveUint8Param
 * @brief     This Struct is used to unsigned 8bit int data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/    
    uint8_t value;    /**< value of uint8_t type is used to save unsigned 8bit int data */
}EvtSaveUint8Param;

/*!
 * @struct     EvtSaveUint16Param
 * @brief     This Struct is used to save unsigned 16bit int data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/    
    uint16_t value;    /**< value of uint16_t type is used to save unsigned 16bit int data */
}EvtSaveUint16Param;

/*!
 * @struct     EvtSaveUint32Param
 * @brief     This Struct is used to save unsigned 16bit int data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/    
    uint32_t value;    /**< value of uint32_t type is used to save unsigned 32bit int data */
}EvtSaveUint32Param;

/*!
 * @struct     EvtSaveUint64Param
 * @brief     This Struct is used to save unsigned 64bit int data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/    
    uint64_t value;    /**< value of uint64_t type is used to save unsigned 64bit int data */
}EvtSaveUint64Param;
/*! @} */


/*!
 * @ingroup ConnectionInformation
 * @{
 */
 /*! 
 
 * @struct     ConnectionStateUpdateEvt
 * @brief     This Struct is used to save current connection state data\n  
 */
typedef struct {
    ConnectionState connState;    /**< connState of @ref ConnectionState type is saved current connection state */    
} ConnectionStateUpdateEvt;
 

 /*! 
 * @struct     ConnectionStateAndPathUpdateEvt
 * @brief     This Struct is used to save current connection state and usb path data\n
 */
typedef struct {
    ConnectionState connState;    /**< connState of @ref ConnectionState type is saved current connection state   */
    ConnectionPath connPath;    /**< connPath of @ref ConnectionPath type is saved current USB Path information  */
} ConnectionStateAndPathUpdateEvt;

 /*!
 * @struct     ConnectionModeUpdateEvt
 * @brief     This Struct is used to save current mode information data\n
 */
typedef struct {
    ConnectionMode connMode;    /**< connMode of @ref ConnectionMode type is saved current mode information   */
} ConnectionModeUpdateEvt;

 /*!
 * @struct     ConnectionCarplaySupportUpdateEvt
 * @brief     This Struct is used to save CaplaySupport of connected device information data\n
 *
 */
typedef struct {
    ConnectionCarPlaySupport carplaySupport; /**< carplaySupport of @ref ConnectionCarPlaySupport type is saved CaplaySupport of connected device information */
} ConnectionCarplaySupportUpdateEvt;


 /*!
 * @struct     ConnectionIap2SupportUpdateEvt
 * @brief     This Struct is used to save Iap2Support of connected device information data\n
 */
typedef struct {
    ConnectionIap2Support iap2Support;    /**< iap2Support of @ref ConnectionIap2Support type is saved Iap2Support of connected device information */
} ConnectionIap2SupportUpdateEvt;

#ifdef ANDROID
 /*!
 * @struct     ConnectionDeviceInfoUpdateEvt
 * @brief     This Struct is used to save devcie's information data of connected devices\n
 */
typedef struct {
    UsbDeviceInfo deviceinfo;    /**< deviceinfo of @ref UsbDeviceInfo type is saved devcie's information of connected device   */
} ConnectionDeviceInfoUpdateEvt;
#endif

 /*!
 * @struct     ConnectionInfoUpdateEvt
 * @brief     This Struct is used to save all state data of connected devices\n
 */
typedef struct {
    ConnectionState connState;                    /**< connState of @ref ConnectionState type is saved current state information   */
    ConnectionPath connPath;                    /**< connPath of @ref ConnectionPath type is saved current path information   */
    ConnectionMode connMode;                    /**< connMode of @ref ConnectionMode type is saved current mode information   */
    ConnectionCarPlaySupport carplaySupport;    /**< carplaySupport of @ref ConnectionCarPlaySupport type is saved current carplaysupport information   */
    ConnectionIap2Support iap2Support;            /**< iap2Support of @ref ConnectionIap2Support type is saved current iap2support information   */    
} ConnectionInfoUpdateEvt;

/*! @} */


/*!
 * @ingroup Power 
 * @{
 */ 

 /*!
 * @enum     AccessoryPowerModes    
 * @brief     This enum value is used to save AccessoyPowerMode Payload data value of PowerUpdate Message.
 */
typedef enum {
    APM_RESERVED = 0,                    /**< RESERVED */
    APM_LOW_POWER_MODE,                    /**< LOW_POWER_MODE */
    APM_INTERMEDIATE_HIGH_POWER_MODE    /**< INTERMEDIATE_HIGH_POWER_MODE*/    
} AccessoryPowerModes ;


 /*!
 * @struct     MaxCurrentDrawnFromAccParam
 * @brief     This Struct is used to save MaximumCurrentDrawnFromAccessory payload data of PowerUpdate Message.\n
 */
typedef struct {
    BOOL bPresent;        /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;        /**< value of uint16_t type is used to save MaximumCurrentDrawnFromAccessory data */
} MaxCurrentDrawnFromAccParam;


 /*!
 * @struct     AccPowerModeParam
 * @brief     This Struct is used to save AccessoryPowerMode payload data of PowerUpdate Message.\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    AccessoryPowerModes value;  /**< value of AccessoryPowerModes type is used to save AccessoryPowerMode data */
} AccPowerModeParam;


 /*!
 * @enum     BatteryChargingState    
 * @brief     This enum value is used to save BatteryChargingState Payload data value of PowerUpdate Message.
 */
typedef enum {
    BCS_DISABLED = 0,                /**< DISABLED */
    BCS_CHARGING,                    /**< CHARGING */
    BCS_CHARGED                        /**< CHARGED  */    
} BatteryChargingState ;

 
 /*!
 * @struct     BatteryChargingStateParam
 * @brief     This Struct is used to save BatteryChargingState payload data of PowerUpdate Message.\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    BatteryChargingState value;  /**< value of BatteryChargingState type is used to save BatteryChargingState data */
} BatteryChargingStateParam;


 /*!
 * @struct     BatteryChargeLevelParam
 * @brief     This Struct is used to save BatteryChargeLevel payload data of PowerUpdate Message.\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;  /**< value of uint16_t type is used to save BatteryChargeLevel data */
} BatteryChargeLevelParam;

 
/*!
 * @struct     PowerUpdateEvt
 * @brief     This Struct is used to send PowerUpdate payload data of PowerUpdate Message to Application.\n
 */
typedef struct {
    MaxCurrentDrawnFromAccParam maxCurrent;                    /**< Refer @ref MaxCurrentDrawnFromAccParam */
    DevBatteryChargeParam devBatteryCharge;                    /**< Refer @ref DevBatteryChargeParam */
    AccPowerModeParam accPowerMode;                            /**< Refer @ref AccPowerModeParam */
    BOOL IsExternalChargerConnected;                        /**< IsExternalChargerConnected of BOOL type is used to save IsExternalChargerConnected data */
    BatteryChargingStateParam BatteryChargingState;            /**< Refer @ref BatteryChargingStateParam */
    BatteryChargeLevelParam BatteryChargeLevel;                /**< Refer @ref BatteryChargeLevelParam */    
} PowerUpdateEvt;
/*! @} */


/*!
 * @ingroup NowPlaying 
 * @{
 */

/*!
 * @struct     MediaItemAttrPersistId
 * @brief     This Struct is used to save MediaItemPersistentIdentifier payload data of NowPlayingUpdate Message (ID : 0).\n
 */
typedef struct {     
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint64_t value;    /**< value of uint64_t type is used to save MediaItemPersistentIdentifier payload data  */
} MediaItemAttrPersistId;

/*!
 * @struct     MediaItemAttrTitle
 * @brief     This Struct is used to save MediaItemTitle payload data of NowPlayingUpdate Message (ID : 1).\n
 */
typedef struct {  
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value; /**< value of uint8_t* type is used to save MediaItemAttrTitle payload data  */
} MediaItemAttrTitle;

/*!
 * @enum     MediaType
 * @brief     This enum value is used to MediaType.
 */
typedef enum {
    kMediaTypeMusic            = 0,     /**< TypeMusic*/    
    kMediaTypePodcast        = 1,    /**< TypePodcast*/    
    kMediaTypeAudiobook        = 2,    /**< TypeAudiobook*/    
    kMediaTypeItunesu        = 3        /**< TypeItunesu*/    
} MediaType;

/*!
 * @struct     MediaItemAttrMediaType
 * @brief     This Struct is used to save MediaItemMediaType payload data of NowPlayingUpdate Message (ID : 2).\n
 */
typedef struct { 
    uint8_t value;    /**< value of uint8_t* type is used to save MediaItemMediaType data  */
} MediaItemAttrMediaType;

/*!
 * @struct     MediaItemAttrRating
 * @brief     This Struct is used to save MediaItemRating payload data of NowPlayingUpdate Message (ID : 3).\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save MediaItemRating payload data  */
} MediaItemAttrRating;

/*!
 * @struct     MediaItemAttrPlaybackDurationInMs
 * @brief     This Struct is used to save MediaItemPlaybackDurationInMilliseconds payload data of NowPlayingUpdate Message (ID : 4).\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t value;    /**< value of uint32_t type is used to save MediaItemPlaybackDurationInMilliseconds payload data  */
} MediaItemAttrPlaybackDurationInMs;

/*!
 * @struct     MediaItemAttrAlbumPersistId
 * @brief     This Struct is used to save MediaItemAlbumPersistIdentifier payload data of NowPlayingUpdate Message (ID : 5)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint64_t value;    /**< value of uint64_t type is used to save MediaItemAlbumPersistIdentifier payload data  */
} MediaItemAttrAlbumPersistId;

/*!
 * @struct     MediaItemAttrAlbumTitle
 * @brief     This Struct is used to save MediaItemAlbumTitle payload data of NowPlayingUpdate Message (ID : 6)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value; /**< value of uint8_t* type is used to save MediaItemAlbumTitle payload data  */
} MediaItemAttrAlbumTitle;

/*!
 * @struct     MediaItemAttrAlbumTrackNo
 * @brief     This Struct is used to save MediaItemAlbumTrackNumber payload data of NowPlayingUpdate Message (ID : 7)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;    /**< value of uint16_t type is used to save MediaItemAlbumTrackNumber payload data  */
} MediaItemAttrAlbumTrackNo;

/*!
 * @struct     MediaItemAttrAlbumTrackCount
 * @brief     This Struct is used to save MediaItemAlbumCount payload data of NowPlayingUpdate Message (ID : 8)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;    /**< value of uint16_t type is used to save MediaItemAlbumCount payload data  */
} MediaItemAttrAlbumTrackCount;

/*!
 * @struct     MediaItemAttrAlbumDiscNo
 * @brief     This Struct is used to save MediaItemAlbumDiscNumber payload data of NowPlayingUpdate Message (ID : 9)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;    /**< value of uint16_t type is used to save MediaItemAlbumDiscNumber payload data  */
} MediaItemAttrAlbumDiscNo;

/*!
 * @struct     MediaItemAttrAlbumDiscCount
 * @brief     This Struct is used to save MediaItemAlbumDiscCount payload data of NowPlayingUpdate Message (ID : 10)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;    /**< value of uint16_t type is used to save MediaItemAlbumDiscCount payload data  */
} MediaItemAttrAlbumDiscCount;

/*!
 * @struct     MediaItemAttrArtistPersistId
 * @brief     This Struct is used to save MediaItemArtistPersistIdentifier payload data of NowPlayingUpdate Message (ID : 11)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint64_t value;    /**< value of uint64_t type is used to save MediaItemArtistPersistIdentifier payload data  */
} MediaItemAttrArtistPersistId;

/*!
 * @struct     MediaItemAttrArtist
 * @brief     This Struct is used to save MediaItemArtist payload data of NowPlayingUpdate Message (ID : 12)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value; /**< value of uint8_t* type is used to save MediaItemArtist payload data  */
} MediaItemAttrArtist;

/*!
 * @struct     MediaItemAttrAlbumArtistPersistId
 * @brief     This Struct is used to save MediaItemAlbumArtistPersistIdentifier payload data of NowPlayingUpdate Message (ID : 13)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint64_t value;    /**< value of uint64_t type is used to save MediaItemAlbumArtistPersistIdentifier payload data  */
} MediaItemAttrAlbumArtistPersistId;

/*!
 * @struct     MediaItemAttrAlbumArtist
 * @brief     This Struct is used to save MediaItemAlbumArtist payload data of NowPlayingUpdate Message (ID : 14)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value; /**< value of uint8_t* type is used to save MediaItemAlbumArtist payload data  */
}MediaItemAttrAlbumArtist;

/*!
 * @struct     MediaItemAttrGenrePersistId
 * @brief     This Struct is used to save MediaItemGenrePersistIdentifier payload data of NowPlayingUpdate Message (ID : 15)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint64_t value;    /**< value of uint64_t type is used to save MediaItemGenrePersistIdentifier payload data  */
} MediaItemAttrGenrePersistId;

/*!
 * @struct     MediaItemAttrGenre
 * @brief     This Struct is used to save MediaItemGenre payload data of NowPlayingUpdate Message (ID : 16)\n
 */
typedef struct { // ID : 16
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save MediaItemGenre payload data  */
} MediaItemAttrGenre;

/*!
 * @struct     MediaItemAttrComposerPersistId
 * @brief     This Struct is used to save MediaItemComposerPersistIdentifier payload data of NowPlayingUpdate Message (ID : 17)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint64_t value;    /**< value of uint64_t type is used to save MediaItemComposerPersistIdentifier payload data  */
} MediaItemAttrComposerPersistId;

/*!
 * @struct     MediaItemAttrComposier
 * @brief     This Struct is used to save MediaItemComposer payload data of NowPlayingUpdate Message (ID : 18)\n
 */
typedef struct { // ID : 18
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value; /**< value of uint8_t type is used to save MediaItemComposer payload data  */
} MediaItemAttrComposier;

/*!
 * @struct     MediaItemAttrIsPartOfCompilation
 * @brief     This Struct is used to save MediaItemIsPartOfCompilation payload data of NowPlayingUpdate Message (ID : 19)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save MediaItemIsPartOfCompilation payload data  */
} MediaItemAttrIsPartOfCompilation;

/*!
 * @struct     MediaItemAttrIsLikeSupported
 * @brief     This Struct is used to save MediaItemIsLikeSupported payload data of NowPlayingUpdate Message (ID : 21)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save MediaItemIsLikeSupported payload data  */
} MediaItemAttrIsLikeSupported;

/*!
 * @struct     MediaItemAttrIsBanSupported
 * @brief     This Struct is used to save MediaItemIsBanSupported payload data of NowPlayingUpdate Message (ID : 22)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save MediaItemIsBanSupported payload data  */
} MediaItemAttrIsBanSupported;

/*!
 * @struct     MediaItemAttrIsLiked
 * @brief     This Struct is used to save MediaItemIsLiked payload data of NowPlayingUpdate Message (ID : 23)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save MediaItemIsLiked payload data  */
} MediaItemAttrIsLiked;

/*!
 * @struct     MediaItemAttrIsBaned
 * @brief     This Struct is used to save MediaItemIsBaned payload data of NowPlayingUpdate Message (ID : 24)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save MediaItemIsBaned payload data  */
} MediaItemAttrIsBaned;

/*!
 * @struct     MediaItemAttrIsResidOnDev
 * @brief     This Struct is used to save MediaItemIsResidOnDevice payload data of NowPlayingUpdate Message (ID : 25)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save MediaItemIsResidOnDevice payload data  */
} MediaItemAttrIsResidOnDev;

/*!
 * @struct     MediaItemAttrArtworkFileTransferId
 * @brief     This Struct is used to save MediaItemAttrArtworkFileTransferIdentifer payload data of NowPlayingUpdate Message (ID : 26)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save MediaItemAttrArtworkFileTransferIdentifer payload data  */
} MediaItemAttrArtworkFileTransferId;

/*!
 * @struct     MediaItemAttrChapterCount
 * @brief     This Struct is used to save MediaItemAttrChapterCount payload data of NowPlayingUpdate Message (ID : 27)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;    /**< value of uint8_t type is used to save MediaItemAttrChapterCount payload data  */
} MediaItemAttrChapterCount;

/*!
 * @struct     MediaItemAttrResGroup
 * @brief     This Struct is used to save MediaItemAttrAttributes payload data of NowPlayingUpdate Message \n
 */
typedef struct {
    MediaItemAttrPersistId persistId;                            /**< Refer @ref MediaItemAttrPersistId */
    MediaItemAttrTitle title;                                    /**< Refer @ref MediaItemAttrTitle */    
    uint32_t MediaItemMediaTypeCount;                            /**< Refer Count of MediaItemMediaType */    
    uint32_t MediaItemMediaTypeSize;                            /**< Refer size of MediaItemAttrMediaType */
    MediaItemAttrMediaType* mediaType;                            /**< Refer @ref MediaItemAttrMediaType */
    MediaItemAttrRating rating;                                    /**< Refer @ref MediaItemAttrRating */    
    MediaItemAttrPlaybackDurationInMs playbackDurMs;            /**< Refer @ref MediaItemAttrPlaybackDurationInMs */
    MediaItemAttrAlbumPersistId albumPersistId;                    /**< Refer @ref MediaItemAttrAlbumPersistId */
    MediaItemAttrAlbumTitle albumTitle;                            /**< Refer @ref MediaItemAttrAlbumTitle */
    MediaItemAttrAlbumTrackNo albumTrackNo;                        /**< Refer @ref MediaItemAttrAlbumTrackNo */
    MediaItemAttrAlbumTrackCount albumTrackCount;                /**< Refer @ref MediaItemAttrAlbumTrackCount */
    MediaItemAttrAlbumDiscNo albumDiscNo;                        /**< Refer @ref MediaItemAttrAlbumDiscNo */
    MediaItemAttrAlbumDiscCount albumDiscCount;                    /**< Refer @ref MediaItemAttrAlbumDiscCount */
    MediaItemAttrArtistPersistId artistPersistId;                /**< Refer @ref MediaItemAttrArtistPersistId */
    MediaItemAttrArtist artist;                                    /**< Refer @ref MediaItemAttrArtist */
    MediaItemAttrAlbumArtistPersistId albumArtistPersistId;        /**< Refer @ref MediaItemAttrAlbumArtistPersistId */
    MediaItemAttrAlbumArtist albumArtist;                        /**< Refer @ref MediaItemAttrAlbumArtist */
    MediaItemAttrGenrePersistId genrePersistId;                    /**< Refer @ref MediaItemAttrGenrePersistId */
    MediaItemAttrGenre genre;                                    /**< Refer @ref MediaItemAttrGenre */
    MediaItemAttrComposerPersistId composerPersistId;            /**< Refer @ref MediaItemAttrComposerPersistId */
    MediaItemAttrComposier composer;                            /**< Refer @ref MediaItemAttrComposier */
    MediaItemAttrIsPartOfCompilation isPartofCompilation;        /**< Refer @ref MediaItemAttrIsPartOfCompilation */
    MediaItemAttrIsLikeSupported isLikeSurpported;                /**< Refer @ref MediaItemAttrIsLikeSupported */
    MediaItemAttrIsBanSupported    isBanSupported;                    /**< Refer @ref MediaItemAttrIsBanSupported */
    MediaItemAttrIsLiked isLiked;                                /**< Refer @ref MediaItemAttrIsLiked */
    MediaItemAttrIsBaned isBanned;                                /**< Refer @ref MediaItemAttrIsBaned */
    MediaItemAttrIsResidOnDev isResidOnDev;                        /**< Refer @ref MediaItemAttrIsResidOnDev */
    MediaItemAttrArtworkFileTransferId ArtworkFileTransferID;    /**< Refer @ref MediaItemAttrArtworkFileTransferId */
    MediaItemAttrChapterCount ChapterCount;                        /**< Refer @ref MediaItemAttrChapterCount */
} MediaItemAttrResGroup;


/*!
 * @enum     PlaybackStatus
 * @brief     This enum value is used to PlaybackStatus.
 */
typedef enum {
    kPlaybackStatusStopped            = 0,    /**< Stopped */
    kPlaybackStatusPlaying            = 1,    /**< Playing */
    kPlaybackStatusPaused            = 2,    /**< Paused */
    kPlaybackStatusSeekForward        = 3,    /**< SeekForward */
    kPlaybackStatusSeekBackward        = 4        /**< SeekBackward */
}PlaybackStatus;

/*!
 * @struct     PlaybackAttrStatus
 * @brief     This Struct is used to save PlaybackStatus payload data of NowPlayingUpdate Message (ID : 0)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;     /**< value of uint8_t type is used to save PlaybackStatus payload data  */
} PlaybackAttrStatus;

/*!
 * @struct     PlaybackAttrElapsedTimeMs
 * @brief     This Struct is used to save PlaybackElapsedTimeInMilliseconds payload data of NowPlayingUpdate Message (ID : 1)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t value;    /**< value of uint32_t type is used to save PlaybackElapsedTimeInMilliseconds payload data  */
} PlaybackAttrElapsedTimeMs;

/*!
 * @struct     PlaybackAttrQueueIdx
 * @brief     This Struct is used to save PlaybackQueueIndex payload data of NowPlayingUpdate Message (ID : 2)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t value;    /**< value of uint32_t type is used to save PlaybackQueueIndex payload data  */
} PlaybackAttrQueueIdx;

/*!
 * @struct     PlaybackAttrQueueCount
 * @brief     This Struct is used to save PlaybackQueuecount payload data of NowPlayingUpdate Message (ID :3)\n
 */
typedef struct {     
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t value;    /**< value of uint32_t type is used to save PlaybackQueuecount payload data  */
} PlaybackAttrQueueCount;

/*!
 * @struct     PlaybackAttrQueueChapderIdx
 * @brief     This Struct is used to save PlaybackQueueChapterIndex payload data of NowPlayingUpdate Message (ID : 4)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t value;    /**< value of uint32_t type is used to save PlaybackQueueChapterIndex payload data  */
} PlaybackAttrQueueChapderIdx;

/*!
 * @enum     PlaybackShuffle
 * @brief     This enum value is used to PlaybackShuffle.
 */
typedef enum {
    kPlaybackShuffleOff            = 0,    /**< ShuffleOff */
    kPlaybackShuffleSongs        = 1,    /**< ShuffleSongs */
    kPlaybackShuffleAlbums        = 2        /**< ShuffleAlbums */
}PlaybackShuffle;

/*!
 * @struct     PlaybackAttrShuffleMode
 * @brief     This Struct is used to save PlaybackShuffleMode payload data of NowPlayingUpdate Message (ID : 5)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;     /**< value of uint8_t type is used to save PlaybackShuffleMode payload data  */
} PlaybackAttrShuffleMode;

/*!
 * @enum     PlaybackRepeat
 * @brief     This enum value is used to PlaybackRepeat.
 */
typedef enum {
    kPlaybackRepeatOff        = 0,    /**< RepeatOff */
    kPlaybackRepeatOne        = 1,    /**< RepeatOne */
    kPlaybackRepeatAll        = 2        /**< RepeatAll */
}PlaybackRepeat;

/*!
 * @struct     PlaybackAttrRepeatMode
 * @brief     This Struct is used to save PlaybackRepeatMode payload data of NowPlayingUpdate Message (ID : 6)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;     /**< value of uint8_t type is used to save PlaybackRepeatMode payload data  */
} PlaybackAttrRepeatMode;

/*!
 * @struct     PlaybackAttrAppName
 * @brief     This Struct is used to save PlaybackAppName payload data of NowPlayingUpdate Message (ID : 7)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t type is used to save PlaybackAppName payload data  */
} PlaybackAttrAppName;

/*!
 * @struct     PlaybackAttrPbMediaLibUniqueID
 * @brief     This Struct is used to save PlaybackMediaLibraryUniqueIdentifier payload data of NowPlayingUpdate Message (ID : 8)\n
 */
typedef struct { // ID : 8
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save PlaybackMediaLibraryUniqueIdentifier payload data  */
} PlaybackAttrPbMediaLibUniqueID;

/*!
 * @struct     PlaybackAttrPbiTunesRadioAd
 * @brief     This Struct is used to save PBiTunesRadioAd payload data of NowPlayingUpdate Message (ID : 9)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save PBiTunesRadioAd payload data  */
} PlaybackAttrPbiTunesRadioAd;

/*!
 * @struct     PlaybackAttrPbiTunesRadioStationName
 * @brief     This Struct is used to save PBiTunesRadioStationName payload data of NowPlayingUpdate Message (ID : 10)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save PBiTunesRadioStationName payload data  */
} PlaybackAttrPbiTunesRadioStationName;

/*!
 * @struct     PlaybackAttrPbiTunesRadioStationMediaPlaylistPersistId
 * @brief     This Struct is used to save PBiTunesRadioMediaPlaylistPersistentID payload data of NowPlayingUpdate Message (ID : 11)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint64_t value;    /**< value of uint8_t type is used to save PBiTunesRadioStationName payload data  */
} PlaybackAttrPbiTunesRadioStationMediaPlaylistPersistId;

/*!
 * @struct     PlaybackAttrPbSpeed
 * @brief     This Struct is used to save PlaybackAttrPbSpeed payload data of NowPlayingUpdate Message (ID : 11)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;    /**< value of uint8_t type is used to save PlaybackSpeed payload data  */
} PlaybackAttrPbSpeed;

/*!
 * @struct     PlaybackAttrPbQueueListTransferID
 * @brief     This Struct is used to save PlaybackQueueListTransferID payload data of NowPlayingUpdate Message (ID : 11)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save PlaybackQueueListTransferID payload data  */
} PlaybackAttrPbQueueListTransferID;

/*!
 * @struct     PlaybackAttrPbAppBundleID
 * @brief     This Struct is used to save PlaybackAppBundleID payload data of NowPlayingUpdate Message (ID : 11)\n
 */
typedef struct { 
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t type is used to save PlaybackAppName payload data  */
} PlaybackAttrPbAppBundleID;


/*!
 * @struct     PlaybackAttrResGroup
 * @brief     This Struct is used to save PlaybackAttributes payload data of NowPlayingUpdate Message \n
 */
typedef struct {
    PlaybackAttrStatus status;                                                                                /**< Refer @ref PlaybackAttrStatus */
    PlaybackAttrElapsedTimeMs elapsedTimeMs;                                                                /**< Refer @ref PlaybackAttrElapsedTimeMs */
    PlaybackAttrQueueIdx queueIdx;                                                                            /**< Refer @ref PlaybackAttrQueueIdx */
    PlaybackAttrQueueCount queueCount;                                                                        /**< Refer @ref PlaybackAttrQueueCount */            
    PlaybackAttrQueueChapderIdx queueChapterIdx;                                                            /**< Refer @ref PlaybackAttrQueueChapderIdx */
    PlaybackAttrShuffleMode shuffleMode;                                                                    /**< Refer @ref PlaybackAttrShuffleMode */    
    PlaybackAttrRepeatMode repeatMode;                                                                        /**< Refer @ref PlaybackAttrRepeatMode */
    PlaybackAttrAppName appName;                                                                            /**< Refer @ref PlaybackAttrAppName  */
    PlaybackAttrPbMediaLibUniqueID pbMediaLibUniqueId;                                                        /**< Refer @ref PlaybackAttrPbMediaLibUniqueID */        
    PlaybackAttrPbiTunesRadioAd pbiTunesRadioAd;                                                            /**< Refer @ref PlaybackAttrPbiTunesRadioAd */            
    PlaybackAttrPbiTunesRadioStationName pbiTunesRadioStationName;                                            /**< Refer @ref PlaybackAttrPbiTunesRadioStationName */    
    PlaybackAttrPbiTunesRadioStationMediaPlaylistPersistId pbiTunesRadioStationMediaPlaylistPersistentID;    /**< Refer @ref PlaybackAttrPbiTunesRadioStationMediaPlaylistPersistId */    
    PlaybackAttrPbSpeed Speed;                                                                                /**< Refer @ref PlaybackAttrPbSpeed */    
    BOOL SetElapsedTimeAvailable;                                                                            /**< value of SetElapsedTimeAvailable payload */    
    BOOL PlaybackQueueListAvail;                                                                            /**< value of PlaybackQueueListAvail payload */        
    PlaybackAttrPbQueueListTransferID QueueListTransferID;                                                    /**< Refer @ref PlaybackAttrPbQueueListTransferID */
    PlaybackAttrPbAppBundleID AppBundleID;                                                                    /**< Refer @ref PlaybackAttrPbAppBundleID */    
} PlaybackAttrResGroup;

/*!
 * @struct     NowPlayingUpdateEvt
 * @brief     This Struct is used to save payload data of NowPlayingUpdate Message \n
 */
typedef struct {
    BOOL bMiPresent;                        /**< bMiPresent of BOOL type is used to confirm whether value is present, if bMiPresent is present then set TRUE, otherwise FALSE*/
    MediaItemAttrResGroup mediaAttrResGr;    /**< Refer @ref MediaItemAttrResGroup */    
    BOOL bPbPresent;                        /**< bPbPresent of BOOL type is used to confirm whether value is present, if bPbPresent is present then set TRUE, otherwise FALSE*/
    PlaybackAttrResGroup playbackAttrResGr;    /**< Refer @ref PlaybackAttrResGroup */
} NowPlayingUpdateEvt;

// #ifdef iAP2SERVICE_FILETRANSFER

/*!
 * @enum    FileTransferState
 * @brief     This enum value  is used to FileTransferState.  
 */
 
typedef enum
{
    FileTransferStateIdle = 0,
    FileTransferStateSetup,
    FileTransferStateSend,//2 
    FileTransferStatePauseSend,//3
    FileTransferStateCancelSend, //4//4
    FileTransferStateWaitStatus,   //5
    FileTransferStateFinishSend,   //6
    FileTransferStateFailSend, //7
    FileTransferStateRecv, //8
    FileTransferStatePauseRecv,//9
    FileTransferStateCancelRecv,//10
    FileTransferStateFinishRecv,//11
    FileTransferStateFailRecv//12
} FileTransferState;

/*!
 * @enum     FILETRANSFERMODE
 * @brief     This enum value  is used to FILETRANSFERMODE.  
 */
typedef enum {
    FILETRANSFERMODE_NONE = 0,
    FILETRANSFERMODE_NOWPLAYING_ARTWORK = 1,
    FILETRANSFERMODE_NOWPLAYING_QUEUELIST = 2,
    FILETRANSFERMODE_MEDIALIBRARYUPDATE_ARTWORK = 3,
    FILETRANSFERMODE_MEDIALIBRARYUPDATE_CONTAINITEM = 4
}FILETRANSFERMODE;

/*!
 * @struct     NowPlayingAlbumArtworkEvt
 * @brief     This Struct is used to save filetransfer data of NowPlayingUpdate Message \n
 */
typedef struct {
    FileTransferState state;        /**< state of iAP2FileTransferState_t type is used to save state of Nowplaying filetransfer  */
    uint64_t totSize;    /**< totSize of uint64_t type is used to save totSize of file receiving from apple device  */
    BOOL bPresent;     /**< bPresent of BOOL type is used to confirm whether value is present, if bMiPresent is present then set TRUE, otherwise FALSE*/
    uint64_t buffSize;   /**< buffSize of uint64_t type is used to save size of value */
    uint8_t* value;       /**< value of uint8_t* type is used to save data receiving by filetransfer  */
    uint8_t bufferID;   /**< bufferID of uint8 type is used to bufferID data receiving by filetransfer  */
    FILETRANSFERMODE  mFileTransferMode;    /**< mFileTransferMode of FILETRANSFERMODE type is used to FileTransferMode data receiving by filetransfer  */
} FileTransferUpdateEvt;
// #endif
/*! @} */

/*!
 * @ingroup VehicleStatus
 * @{
 */
/*!
 * @struct     StartVehicleStatusUpdatesEvt
 * @brief     This Struct is used to save payload data of StartVehicleStatusUpdates Message \n
 */
typedef struct {
    BOOL bRange;                    /**< bRange of BOOL type is used to save Range data*/
    BOOL bOutSideTemperature;        /**< bOutSideTemperature of BOOL type is used to save OutSideTemperature data*/
    BOOL bRangeWarning;                /**< bRangeWarning of BOOL type is used to save RangeWarning data*/
} StartVehicleStatusUpdatesEvt;
/*! @} */



/*!
 * @ingroup Location
 * @{
 */
/*!
 * @struct     StartLocationInformationEvt
 * @brief     This Struct is used to save payload data of StartLocationInformation Message \n
 */
typedef struct {
    BOOL bGPSFixData;                            /**< bGPSFixData of BOOL type is used to save Range GPSFixData*/
    BOOL bRecommMinSpecificGPSTransitData;        /**< bRecommMinSpecificGPSTransitData of BOOL type is used to save RecommMinSpecificGPSTransitData data*/
    BOOL bGPSStatelliteInView;                    /**< bGPSStatelliteInView of BOOL type is used to save GPSStatelliteInView data*/
    BOOL bVehicleSpeedData;                        /**< bVehicleSpeedData of BOOL type is used to save VehicleSpeedData data*/
    BOOL bVehicleGyroData;                        /**< bVehicleGyroData of BOOL type is used to save VehicleGyroData data*/        
    BOOL bVehicleAccelData;                        /**< bVehicleAccelData of BOOL type is used to save VehicleAccelData data*/    
    BOOL bVehicleHeadingData;                    /**< bVehicleHeadingData of BOOL type is used to save VehicleHeadingData data*/            
} StartLocationInformationEvt;
/*! @} */


/*!
 * @ingroup DevcieNotification
 * @{
 */
/*!
 * @struct     DeviceInformationUpdateEvt
 * @brief     This Struct is used to save payload data of DeviceInformationUpdate Message \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value;    /**< value of uint8_t* type is used to save DeviceName data  */
} DeviceInformationUpdateEvt;

/*!
 * @struct     DeviceLanguageUpdateEvt
 * @brief     This Struct is used to save payload data of DeviceLanguageUpdate Message \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value;    /**< value of uint8_t* type is used to save DeviceLanguage data  */
} DeviceLanguageUpdateEvt;
/*! @} */



/*!
 * @ingroup Telephony
 * @{
 */
/*!
 * @struct     CallStatePhoneNumberParam
 * @brief     This Struct is used to save CallStatePhoneNumber payload data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value;    /**< value of uint8_t* type is used to save CallStatePhoneNumber data  */
} CallStatePhoneNumberParam;

/*!
 * @struct     CallStateNetSentCallerNameParam
 * @brief     This Struct is used to save CallStateNetSentCallerName payload data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value;    /**< value of uint8_t* type is used to save CallStateNetSentCallerName data  */
} CallStateNetSentCallerNameParam;

/*!
 * @enum     CALLSTATUS
 * @brief     This enum value  is used to CALLSTATUS.  
 */
typedef enum {
    CALLSTATUS_DISCONNECTED = 0,
    CALLSTATUS_ACTIVE = 1,
    CALLSTATUS_HODING = 2,
    CALLSTATUS_CONNECTING = 3
}CALLSTATUS;

/*!
 * @struct     CallStateStatusParam
 * @brief     This Struct is used to save CallStateStatus payload data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save CallStateStatus data  */
} CallStateStatusParam;

/*!
 * @enum     CALLDIRECTIONVALUE
 * @brief     This enum value  is used to CALLDIRECTIONVALUE.  
 */
typedef enum {
    CALLDIRECTIONVALUE_INCOMMING = 0,
    CALLDIRECTIONVALUE_OUTGOING = 1,
    CALLDIRECTIONVALUE_UNKNOWN = 2
}CALLDIRECTIONVALUE;

/*!
 * @struct     CallStateDirectionParam
 * @brief     This Struct is used to save CallStateDirection payload data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save CallStateDirection data  */
} CallStateDirectionParam;

/*!
 * @struct     CallStateDirectionParam
 * @brief     This Struct is used to save UniqueCallID payload data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value;    /**< value of uint8_t* type is used to save UniqueCallID data  */
} UniqueCallIDParam;

/*!
 * @struct     CallStatevCardFileTransferIDParam
 * @brief     This Struct is used to save UniqueCallID payload data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save CallStatevCardFileTransferID data  */
} CallStatevCardFileTransferIDParam;

/*!
 * @struct     TelephonyCallStateInformationEvt
 * @brief     This Struct is used to save payload data of TelephonyCallStateInformation message.  \n
 */
typedef struct {
    CallStatePhoneNumberParam callStatePhoneNum;                /**< Refer @ref CallStatePhoneNumberParam */
    CallStateNetSentCallerNameParam callStateNetSentCallerName;    /**< Refer @ref CallStateNetSentCallerNameParam */
    CallStateStatusParam callStateStatus;                        /**< Refer @ref CallStateStatusParam */
    CallStateDirectionParam callStateDirection;                    /**< Refer @ref CallStateDirectionParam */
    UniqueCallIDParam uniqueCallID;                                /**< Refer @ref UniqueCallIDParam */    
    CallStatevCardFileTransferIDParam callStatevCardFTID;        /**< Refer @ref CallStatevCardFileTransferIDParam */
} TelephonyCallStateInformationEvt;


/*!
 * @enum     TELEPHONYSIGNALSTRENTH
 * @brief     This enum value  is used to TELEPHONYSIGNALSTRENTH.  
 */
typedef enum {
    TELEPHONYSIGNALSTRENTH_0 = 0,
    TELEPHONYSIGNALSTRENTH_1 = 1,
    TELEPHONYSIGNALSTRENTH_2 = 2,
    TELEPHONYSIGNALSTRENTH_3 = 3,
    TELEPHONYSIGNALSTRENTH_4 = 4,
    TELEPHONYSIGNALSTRENTH_5 = 5
}TELEPHONYSIGNALSTRENTH;

/*!
 * @struct     TelephonySignalStrengthParam
 * @brief     This Struct is used to save payload data of TelephonyCallStateInformation message.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;     /**< value of uint8_t type is used to save TelephonySignalStrength data  */
} TelephonySignalStrengthParam;

/*!
 * @enum     TELEPHONYREGISSTATUS
 * @brief     This enum value  is used to TELEPHONYREGISSTATUS.   
 */
typedef enum {
    TELEPHONYREGISSTATUS_UNKNOWN = 0,
    TELEPHONYREGISSTATUS_NOTREGISTERED = 1,
    TELEPHONYREGISSTATUS_SEARCHING = 2,
    TELEPHONYREGISSTATUS_DENIED = 3,
    TELEPHONYREGISSTATUS_REGISTEREDHOME = 4,
    TELEPHONYREGISSTATUS_ROAMING = 5,
    TELEPHONYREGISSTATUS_EMERGENCYCALLONLY = 6
}TELEPHONYREGISSTATUS;

/*!
 * @struct     TelephonyRegisStatusParam
 * @brief     This Struct is used to save TelephonyRegisStatus payload data.  \n
 */
typedef struct {    
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;     /**< value of uint8_t type is used to save TelephonyRegisStatus data  */
} __attribute__ ((aligned(4)))TelephonyRegisStatusParam;

/*!
 * @struct     TelephonyAirPlaneModeStatusParam
 * @brief     This Struct is used to save TelephonyAirPlaneModeStatus payload data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    BOOL value;        /**< value of BOOL type is used to save TelephonyAirPlaneModeStatus data  */
} __attribute__ ((aligned(4)))TelephonyAirPlaneModeStatusParam;

/*!
 * @struct     TelephonyTTYStatusParam
 * @brief     This Struct is used to save TelephonyTTYStatus payload data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    BOOL value;        /**< value of BOOL type is used to save )TelephonyAirPlaneModeStatus data  */
} __attribute__ ((aligned(4)))TelephonyTTYStatusParam;

/*!
 * @struct     MobileOperatorParam
 * @brief     This Struct is used to save MobileOperator payload data.  \n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value;    /**< value of uint8_t* type is used to save MobileOperator data  */
} __attribute__ ((aligned(4)))MobileOperatorParam;

/*!
 * @struct     TelephonyUpdateEvt
 * @brief     This Struct is used to save payload data of TelephonyUpdate message.  \n
 */
typedef struct {
    TelephonySignalStrengthParam telephonySignalStrength;            /**< Refer @ref TelephonySignalStrengthParam */
    TelephonyRegisStatusParam telephonyRegisStatus;                    /**< Refer @ref TelephonyRegisStatusParam */
    TelephonyAirPlaneModeStatusParam telephonyAirPlaneModeStatus;    /**< Refer @ref TelephonyAirPlaneModeStatusParam */
    TelephonyTTYStatusParam telephonyTTYStatus;                        /**< Refer @ref TelephonyTTYStatusParam */
    MobileOperatorParam mobileOperator;                                /**< Refer @ref MobileOperatorParam */
} TelephonyUpdateEvt;
/*! @} */


/*!
 * @ingroup BluetoothConnection
 * @{
 */
 
/*!
 * @enum     BTCOMPONENTPROFILE
 * @brief     This enum value  is used to BTCOMPONENTPROFILE.   
 */
typedef enum {
    BTCOMPONENTPROFILE_HANSFREE = 0,                        /**< HANSFREE */
    BTCOMPONENTPROFILE_PBACESS = 1,                            /**< PBACESS */
    BTCOMPONENTPROFILE_AVREMOTECONTROL = 3,                    /**< AVREMOTECONTROL */
    BTCOMPONENTPROFILE_ADVANCEDDISTRIBUTION = 4,            /**< ADVANCEDDISTRIBUTION  */
    BTCOMPONENTPROFILE_HID = 5,                                /**< HID */
    BTCOMPONENTPROFILE_IAPLINK = 7,                            /**< IAPLINK */
    BTCOMPONENTPROFILE_PERSONALAREANETACCESSPOINT = 8,        /**< PERSONALAREANETACCESSPOINT */
    BTCOMPONENTPROFILE_MSGACESS = 9,                        /**< MSGACESS */
    BTCOMPONENTPROFILE_PERSONALAREANETCLIENT = 12            /**< PERSONALAREANETCLIENT */
}BTCOMPONENTPROFILE;

/*!
 * @struct     ConnectedBTProfilesParam
 * @brief     This Struct is used to save ConnectedBTProfiles payload data.  \n
 */
typedef struct {
    BOOL bPresent;                                      /**< bPresent of BOOL type is used to confirm whether connectedBTProfiles is present, if connectedBTProfiles is present then set TRUE, otherwise FALSE*/     
    EvtSaveBoolParam BTHandsFree;                       /** BluetoothHandsFree of EvtSaveBoolParam type is used to save BluetoothHandsFree */
    EvtSaveBoolParam BTPhoneAddressAccess;              /** BluetoothPhoneAddressAccess of EvtSaveBoolParam type is used to save BluetoothHandsFree */
    EvtSaveBoolParam BTAudioVideoRemoteControl;         /** BluetoothAudioVideoRemoteControl of EvtSaveBoolParam type is used to save BluetoothHandsFree */
    EvtSaveBoolParam BTAdvanceAudioDistribution;        /** BluetoothAdvanceAudioDistribution of EvtSaveBoolParam type is used to save BluetoothHandsFree */    
    EvtSaveBoolParam BTHumanInterfaceDevice;            /** BluetoothHumanInterfaceDevice of EvtSaveBoolParam type is used to save BluetoothHandsFree */
    EvtSaveBoolParam BTiAP2Link;                        /** BluetoothiAP2Link of EvtSaveBoolParam type is used to save BluetoothHandsFree */
    EvtSaveBoolParam BTPANAP;                           /** BluetoothPersonalAreaNetworkAccessPoint of EvtSaveBoolParam type is used to save BluetoothHandsFree */
    EvtSaveBoolParam BTMessageAccess;                   /** BluetoothMessageAccess of EvtSaveBoolParam type is used to save BluetoothHandsFree */
    EvtSaveBoolParam BTPANC;                            /** BluetoothPersnalAeraNetworkClient of EvtSaveBoolParam type is used to save BluetoothHandsFree */
} ConnectedBTProfilesParam;

/*!
 * @struct     BTConnectionUpdateEvt
 * @brief     This Struct is used to save payload data of BluetoothConnectionUpdate message. \n
 */
typedef struct {
    uint16_t BTTransPortComponentID;                     /**< componentID of uint16_t type is used to save BluetoothTransportcomponentIdentifier data  */
    ConnectedBTProfilesParam mConnectedBTProfiles;    /**< Refer @ref ConnectedBTProfilesParam */
} BTConnectionUpdateEvt;
/*! @} */


/*!
 * @ingroup HumanInterfaceDevice
 * @{
 */
 /*!
 * @struct     DeivceHIDReportEvt
 * @brief     This Struct is used to save payload data of DeivceHIDReport Message.\n
 */ 
typedef struct {
    uint16_t HIDComponentIdentifier;    /**< HIDComponentIdentifier of uint16_t type is used to save HIDComponentIdentifier data  */
    uint16_t HIDReport;             /**< Refer @ref HIDReportParam */
} DeivceHIDReportEvt;
/*! @} */


/*!
 * @ingroup MediaLibraryUpldate
 * @{
 */
 
 /*!
 * @struct     MediaLibraryName
 * @brief     This Struct is used to save MediaLibraryName data.\n
 */ 
typedef struct{
    //BOOL bPresent;
    uint32_t size;  /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value; /**< value of uint8_t* type is used to save MediaLibraryName data  */    
} MediaLibraryName;

 /*!
 * @struct     MediaLibraryUniqueIdentifier
 * @brief     This Struct is used to save MediaLibraryUniqueIdentifier data.\n
 */ 
typedef struct{
    //BOOL bPresent;
    uint32_t size;  /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value; /**< value of uint8_t* type is used to save MediaLibraryName data  */
} MediaLibraryUniqueIdentifier;

 /*!
 * @struct     MediaLibraryType
 * @brief     This Struct is used to save MediaLibraryType data.\n
 */ 
typedef struct { 
    uint8_t value; /**< value of uint8_t type is used to save MediaLibraryType data  */
} MediaLibraryType;

 /*!
 * @struct     MediaLibraryInformationGroup
 * @brief     This Struct is used to save MeidaLibraryInformationGroup data.\n
 */ 
typedef struct{
    MediaLibraryName mMediaLibraryName;                                /**< Refer @ref MediaLibraryName*/
    MediaLibraryUniqueIdentifier mMediaLibraryUniqueIdentifier;        /**< Refer @ref MediaLibraryUniqueIdentifier*/
    MediaLibraryType mMediaLibraryType;                                /**< Refer @ref MediaLibraryType*/
} MediaLibraryInformationGroup;

 /*!
 * @struct     MediaLibraryInformationEvt
 * @brief     This Struct is used to save payload data of MediaLibraryInformation message.
 */ 
typedef struct {
    uint32_t nCountGroupParam;
    MediaLibraryInformationGroup* pMediaLibraryInformationGr;
    //MeidaLibraryInformationGroup mMediaLibraryInformationGr;    /**< Refer @ref MeidaLibraryInformationGroup, TODO : 2 Group data recv parse*/
} MediaLibraryInformationEvt;


 /*!
 * @struct     MediaLibraryRevisionParam
 * @brief     This Struct is used to save MediaLibraryRevision payload data.\n
 */ 
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value; /**< value of uint8_t* type is used to save MediaLibraryName data  */
} MediaLibraryRevisionParam ;

/*!
 * @struct     MediaItemPersistentIdentifierParam
 * @brief     This Struct is used to save MediaItemPersistentIdentifier payload data.\n
 */ 
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint64_t value; /**< value of uint8_t* type is used to save MediaItemPersistentIdentifier data  */    
} MediaItemPersistentIdentifierParam ; // 0

/*!
 * @struct     MediaItemTitleParam
 * @brief     This Struct is used to save MediaItemTitle payload data. (ID : 1)\n
 */ 
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value; /**< value of uint8_t* type is used to save MediaItemTitle data  */
} MediaItemTitleParam ;

/*!
 * @struct     MediaItemMediaTypeParam
 * @brief     This Struct is used to save MediaItemMediaType payload data. (ID : 2)\n
 */ 
typedef struct {
    //BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t* type is used to save MediaItemMediaType data  */
} MediaItemMediaTypeParam ; 

/*!
 * @struct     MediaItemRatingParam
 * @brief     This Struct is used to save MediaItemRating payload data. (ID : 3)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint8_t value;     /**< value of uint8_t type is used to save MediaItemRating data  */
} MediaItemRatingParam ;

/*!
 * @struct     MediaItemPlaybackDurationInMillisecondsParam
 * @brief     This Struct is used to save MediaItemPlaybackDurationInMilliseconds payload data. (ID : 4)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint32_t value; /**< value of uint32_t type is used to save MediaItemPlaybackDurationInMilliseconds data  */    
} MediaItemPlaybackDurationInMillisecondsParam ; 

/*!
 * @struct     MediaItemAlbumPersistentIdentifierParam
 * @brief     This Struct is used to save MediaItemPlaybackDurationInMilliseconds payload data. (ID : 5)\n
 */
typedef struct {
    BOOL bPresent;
    uint64_t value; 
} MediaItemAlbumPersistentIdentifierParam ; 

/*!
 * @struct     MediaItemAlbumTitleParam
 * @brief     This Struct is used to save MediaItemAlbumTitle payload data. (ID : 6)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value; /**< value of uint8_t* type is used to save MediaItemAlbumTitle data  */
} MediaItemAlbumTitleParam ; 

/*!
 * @struct     MediaItemAlbumTrackNumberParam
 * @brief     This Struct is used to save MediaItemAlbumTrackNumber payload data. (ID : 7)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint16_t value; /**< value of uint16_t type is used to save MediaItemAlbumTrackNumber data  */    
} MediaItemAlbumTrackNumberParam ; // 7

/*!
 * @struct     MediaItemAlbumTrackCountParam
 * @brief     This Struct is used to save MediaItemAlbumTrackCount payload data. (ID : 8)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint16_t value; /**< value of uint16_t type is used to save MediaItemAlbumTrackCount data  */
} MediaItemAlbumTrackCountParam ; // 8

/*!
 * @struct     MediaItemAlbumDiscNumberParam
 * @brief     This Struct is used to save MediaItemAlbumDiscNumber payload data. (ID : 9)\n
 */
typedef struct {    
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint16_t value; /**< value of uint16_t type is used to save MediaItemAlbumDiscNumber data  */    
} MediaItemAlbumDiscNumberParam ; //9

/*!
 * @struct     MediaItemAlbumDiscCountParam
 * @brief     This Struct is used to save MediaItemAlbumDiscNumber payload data. (ID : 10)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint16_t value; /**< value of uint16_t type is used to save MediaItemAlbumDiscNumber data  */    
} MediaItemAlbumDiscCountParam ; 

/*!
 * @struct     MediaItemArtistPersistentIdentifierParam
 * @brief     This Struct is used to save MediaItemArtistPersistentIdentifier payload data. (ID : 11)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint64_t value; /**< value of uint64_t type is used to save MediaItemArtistPersistentIdentifier data  */    
} MediaItemArtistPersistentIdentifierParam ; 


/*!
 * @struct     MediaItemArtistParam
 * @brief     This Struct is used to save MediaItemArtist payload data. (ID : 12)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value; /**< value of uint8_t* type is used to save MediaItemArtist data  */    
} MediaItemArtistParam ; 

/*!
 * @struct     MediaItemAlbumAritistPersistentIdentifierParam
 * @brief     This Struct is used to save MediaItemAlbumAritistPersistentIdentifier payload data. (ID : 13)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint64_t value; /**< value of uint64_t type is used to save MediaItemAlbumAritistPersistentIdentifier  data  */    
} MediaItemAlbumAritistPersistentIdentifierParam ; 

/*!
 * @struct     MediaItemAlbumAritistParam
 * @brief     This Struct is used to save MediaItemAlbumAritist payload data. (ID : 14)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param*/
    uint8_t* value; /**< value of uint8_t* type is used to save MediaItemAlbumAritist data  */    
} MediaItemAlbumAritistParam ;  

/*!
 * @struct     MediaItemGenrePersistentIdentifierParam
 * @brief     This Struct is used to save MediaItemGenrePersistentIdentifier payload data. (ID : 15)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE*/
    uint64_t value; /**< value of uint64_t type is used to save MediaItemGenrePersistentIdentifier data  */    
} MediaItemGenrePersistentIdentifierParam ; 

/*!
 * @struct     MediaItemGenreParam
 * @brief     This Struct is used to save MediaItemGenre payload data. (ID : 16)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE */
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value; /**< value of uint8_t* type is used to save MediaItemGenrePersistentIdentifier data  */    
} MediaItemGenreParam ; 

/*!
 * @struct     MediaItemComposerPersistentIdentifierParam
 * @brief     This Struct is used to save MediaItemComposerPersistentIdentifier payload data. (ID : 17)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE */
    uint64_t value; /**< value of uint64_t type is used to save MediaItemComposerPersistentIdentifier data  */
} MediaItemComposerPersistentIdentifierParam ; 

/*!
 * @struct     MediaItemComposerParam
 * @brief     This Struct is used to save MediaItemComposer payload data. (ID : 18)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE */
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value; /**< value of uint8_t* type is used to save MediaItemComposer data  */
} MediaItemComposerParam ; 

/*!
 * @struct     MediaItemArtworkFileTransferIdentifierParam
 * @brief     This Struct is used to save MediaItemArtworkFileTransferIdentifier payload data. (ID : 26)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE */
    uint8_t value; /**< value of uint8_t* type is used to save MediaItemArtworkFileTransferIdentifier data  */
} MediaItemArtworkFileTransferIdentifierParam ; 

/*!
 * @struct     MediaItemChapterCountParam
 * @brief     This Struct is used to save MediaItemChapterCount payload data. (ID : 26)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if bPresent is present then set TRUE, otherwise FALSE */
    uint16_t value; /**< value of uint16_t type is used to save MediaItemChapterCount data  */
} MediaItemChapterCountParam ; 

/*!
 * @struct     MediaItemGroup
 * @brief     This Struct is used to save MediaItemGroup payload data. \n
 */
typedef struct {
    //BOOL bPresent;                                                                                /**< bPresent of BOOL type is used to confirm whether MediaItemGroup is present, if MediaItemGroup is present then set TRUE, otherwise FALSE */
    MediaItemPersistentIdentifierParam MediaItemPersistentIdentifier;                            /**< Refer @ref MediaItemPersistentIdentifierParam */
    MediaItemTitleParam MediaItemTitle;                                                            /**< Refer @ref MediaItemTitleParam */        
    uint32_t nMediaItemMediaTypeCount;                                                            /**< MediaItemMediaTypeCount of uint8_t type is used to save MediaItemMediaTypeCount data  */
    MediaItemMediaTypeParam* pMediaItemMediaType;                                                                /**< pMediaItemMediaType of uint32_t* is used to save MeidaType data */
    uint32_t nMediaItemMediaTypeSize;                                                                /**< pMediaItemMediaType of uint32_t* is used to save MeidaType data */
    //MediaItemMediaTypeParam MediaItemMediaType[4];                                                /**< Refer @ref MediaItemMediaTypeParam */
    MediaItemRatingParam MediaItemRating;                                                        /**< Refer @ref MediaItemRatingParam */
    MediaItemPlaybackDurationInMillisecondsParam MediaItemPlaybackDurationInMilliseconds;        /**< Refer @ref MediaItemPlaybackDurationInMillisecondsParam */
    MediaItemAlbumPersistentIdentifierParam MediaItemAlbumPersistentIdentifier;                    /**< Refer @ref MediaItemAlbumPersistentIdentifierParam */
    MediaItemAlbumTitleParam MediaItemAlbumTitle;                                                /**< Refer @ref MediaItemAlbumTitleParam */
    MediaItemAlbumTrackNumberParam MediaItemAlbumTrackNumber;                                    /**< Refer @ref MediaItemAlbumTrackNumberParam */
    MediaItemAlbumTrackCountParam MediaItemAlbumTrackCount;                                         /**< Refer @ref MediaItemAlbumTrackCountParam */                    
    MediaItemAlbumDiscNumberParam MediaItemAlbumDiscNumber;                                        /**< Refer @ref MediaItemAlbumDiscNumberParam */
    MediaItemAlbumDiscCountParam MediaItemAlbumDiscCount;                                        /**< Refer @ref MediaItemAlbumDiscCountParam */
    MediaItemArtistPersistentIdentifierParam MediaItemArtistPersistentIdentifier;                /**< Refer @ref MediaItemArtistPersistentIdentifierParam */
    MediaItemArtistParam MediaItemArtist;                                                        /**< Refer @ref MediaItemArtistParam */    
    MediaItemAlbumAritistPersistentIdentifierParam MediaItemAlbumAritistPersistentIdentifier;    /**< Refer @ref MediaItemAlbumAritistPersistentIdentifierParam */
    MediaItemAlbumAritistParam MediaItemAlbumAritist;                                            /**< Refer @ref MediaItemAlbumAritistParam */
    MediaItemGenrePersistentIdentifierParam MediaItemGenrePersistentIdentifier;                    /**< Refer @ref MediaItemGenrePersistentIdentifierParam */
    MediaItemGenreParam MediaItemGenre;                                                            /**< Refer @ref MediaItemGenreParam */    
    MediaItemComposerPersistentIdentifierParam MediaItemComposerPersistentIdentifier;            /**< Refer @ref MediaItemComposerPersistentIdentifierParam */
    MediaItemComposerParam MediaItemComposer;                                                    /**< Refer @ref MediaItemComposerParam */
    BOOL MediaItemIsPartOfCompilation;                                                            /**< MediaItemIsPartOfCompilation of BOOL type is used to save MediaItemIsPartOfCompilation data  */
    BOOL MediaItemIsLikeSupported;                                                                /**< MediaItemIsLikeSupported of BOOL type is used to save MediaItemIsLikeSupported data  */
    BOOL MediaItemIsBanSupported;                                                                /**< MediaItemIsBanSupported of BOOL type is used to save MediaItemIsBanSupported data  */
    BOOL MediaItemIsLiked;                                                                        /**< MediaItemIsLiked of BOOL type is used to save MediaItemIsLiked data  */    
    BOOL MediaItemIsBaned;                                                                        /**< MediaItemIsBaned of BOOL type is used to save MediaItemIsBaned data  */            
    BOOL MediaItemIsResidentOnDevice;                                                            /**< MediaItemIsResidentOnDevice of BOOL type is used to save MediaItemIsResidentOnDevice data  */    
    MediaItemArtworkFileTransferIdentifierParam MediaItemArtworkFileTransferIdentifier;            /**< Refer @ref MediaItemArtworkFileTransferIdentifierParam */
    MediaItemChapterCountParam MediaItemChapterCount;                                            /**< Refer @ref MediaItemChapterCountParam */
} MediaItemGroup ;

/*!
 * @struct     MediaPlaylistPersistentIdentifierParam
 * @brief     This Struct is used to save MediaPlaylistPersistentIdentifier payload data. (ID : 0)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint64_t value;    /**< value of uint64_t type is used to save MediaPlaylistPersistentIdentifier data */
} MediaPlaylistPersistentIdentifierParam ; 

/*!
 * @struct     MediaPlaylistNameParam
 * @brief     This Struct is used to save MediaPlaylistName payload data. (ID : 1)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;  /**< size of uint32_t type is used to save size of value param */
    uint8_t* value; /**< value of uint8_t* type is used to save MediaPlaylistName data */
} MediaPlaylistNameParam ; 

/*!
 * @struct     MediaPlaylistParentPersistentIdentifierParam
 * @brief     This Struct is used to save MediaPlaylistParentPersistentIdentifier payload data. (ID : 2)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint64_t value;    /**< value of uint64_t type is used to save MediaPlaylistParentPersistentIdentifier data */
} MediaPlaylistParentPersistentIdentifierParam ; 

/*!
 * @struct     MediaPlaylistContainedMediaItemsFileTransferIdentifierParam
 * @brief     This Struct is used to save MediaPlaylistContainedMediaItemsFileTransferIdentifier payload data. (ID : 5)\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t value;    /**< value of uint8_t type is used to save MediaPlaylistContainedMediaItemsFileTransferIdentifier data */
} MediaPlaylistContainedMediaItemsFileTransferIdentifierParam ;

/*!
 * @struct     MeidaPlaylistGroup
 * @brief     This Struct is used to save MeidaPlaylistGroup payload data.\n
 */
typedef struct {
    //BOOL bPresent;                                                                                                        /**< bPresent of BOOL type is used to confirm whether MeidaPlaylistGroup is present, if MeidaPlaylistGroup is present then set TRUE, otherwise FALSE*/
    MediaPlaylistPersistentIdentifierParam MediaPlaylistPersistentIdentifier;                                            /**< Refer @ref MediaLibraryUniqueIdentifierParam */
    MediaPlaylistNameParam MediaPlaylistName;                                                                            /**< Refer @ref MediaLibraryUniqueIdentifierParam */
    MediaPlaylistParentPersistentIdentifierParam MediaPlaylistParentPersistentIdentifier;                                /**< Refer @ref MediaLibraryUniqueIdentifierParam */    
    BOOL MediaPlaylistIsGeniusMix;                                                                                        /**< MediaPlaylistIsGeniusMix of BOOL type is used to save MediaPlaylistIsGeniusMix data */
    BOOL MediaPlaylistFolder;                                                                                            /**< MediaPlaylistFolder of BOOL type is used to save MediaPlaylistFolder data */
    MediaPlaylistContainedMediaItemsFileTransferIdentifierParam MediaPlaylistContainedMediaItemsFileTransferIdentifier;    /**< Refer @ref MediaLibraryUniqueIdentifierParam */
    BOOL MediaPlaylistiTunesRadioStation;
} MediaPlaylistGroup ;

/*!
 * @struct     MediaItemDeletePersistentIdentifierParam
 * @brief     This Struct is used to save MediaItemDeletePersistentIdentifier payload data.\n
 */
typedef struct {
    //BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint64_t value;    /**< value of uint64_t type is used to save MediaItemDeletePersistentIdentifier data */
} MediaItemDeletePersistentIdentifierParam ; 

/*!
 * @struct     MediaPlaylistDeletePersistentItdentifierParam
 * @brief     This Struct is used to save MediaPlaylistDeletePersistentItdentifier payload data.\n
 */
typedef struct {
    //BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint64_t value;    /**< value of uint64_t type is used to save MediaPlaylistDeletePersistentItdentifier data */
} MediaPlaylistDeletePersistentItdentifierParam ; 

/*!
 * @struct     MediaLibrarUpdateProgressParam
 * @brief     This Struct is used to save MediaLibrarUpdateProgress payload data.\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/    
    uint8_t value; /**< value of uint64_t type is used to save MediaLibrarUpdateProgress data */
} MediaLibrarUpdateProgressParam ;

/*!
 * @struct     MediaLibraryUpdatesEvt
 * @brief     This Struct is used to save MediaLibrarUpdateProgress payload data.\n
 */
typedef struct {
    MediaLibraryUniqueIdentifierParam MediaLibraryUniqueIdentifier_update;                            /**< Refer @ref MediaLibraryUniqueIdentifierParam */    
    MediaLibraryRevisionParam MediaLibraryRevision;                                                    /**< Refer @ref MediaLibraryRevisionParam */    
    uint32_t nMediaItemGroupCount;                                                                    /**< MediaItemGroupCount of uint8_t type is used to save MediaItemGroupCount data */
    MediaItemGroup* pMediaItemGroup;                                                                        /**< pMediaItemGroup of uint32_t* type is used to save MediaItemGroup  */        
    uint32_t nMediaItemGroupSize;
    //MediaItemGroup MediaItem[100];                                                                    /**< Refer @ref MediaItemGroup */
    uint32_t nMediaPlaylistGroupCount;                                                                /**< pMeidaPlaylistGroup of uint8_t type is used to save MediaPlaylistGroupCount data */
    MediaPlaylistGroup* pMediaPlaylistGroup;                                                                    /**< pMediaItemGroup of uint32_t* type is used to save MeidaPlaylistGroup */
    uint32_t nMediaPlaylistGroupSize;
    //MeidaPlaylistGroup MediaPlaylist[100];                                                            /**< Refer @ref MeidaPlaylistGroup */
    uint32_t nMediaItemDeletePersistentIdentifierCount;                                                /**< MediaItemDeletePersistentIdentifierCount of uint8_t type is used to save MediaItemDeletePersistentIdentifierCount data */
    MediaItemDeletePersistentIdentifierParam* pMediaItemDeletePersistentIdentifier;                                                    /**< pMediaItemDeletePersistentIdentifier of uint32_t* type is used to save MediaItemDeletePersistentIdentifier */
    uint32_t nMediaItemDeletePersistentIdentifierSize;    
    //MediaItemDeletePersistentIdentifierParam MediaItemDeletePersistentIdentifier[100];                /**< Refer @ref MediaItemDeletePersistentIdentifierParam */
    uint32_t nMediaPlaylistDeletePersistentItdentifierCount;                                            /**< MediaPlaylistDeletePersistentItdentifierCount of uint8_t type is used to save MediaPlaylistDeletePersistentItdentifierCount data */        
    MediaPlaylistDeletePersistentItdentifierParam* pMediaPlaylistDeletePersistentItdentifier;                                            /**< pMediaPlaylistDeletePersistentItdentifier of uint32_t* type is used to save MediaPlaylistDeletePersistentItdentifier */
    uint32_t nMediaPlaylistDeletePersistentItdentifierSize;    
    //MediaPlaylistDeletePersistentItdentifierParam MediaPlaylistDeletePersistentItdentifier[100];    /**< Refer @ref MediaPlaylistDeletePersistentItdentifierParam */
    BOOL MediaLibraryReset;                                                                            /**< MediaLibraryReset of uint8_t type is used to save MediaLibraryReset data */
    MediaLibrarUpdateProgressParam MediaLibraryUpdateProgress;                                        /**< Refer @ref MediaLibrarUpdateProgressParam */
    BOOL MediaLibraryIsHidingRemoteItems;                                                            /**< MediaLibraryIsHidingRemoteItems of uint8_t type is used to save MediaLibraryIsHidingRemoteItems data */
    BOOL PlayAllSongsCapable;                                                                        /**< PlayAllSongsCapable of BOOL type is used to save PlayAllSongsCapable data */
} MediaLibraryUpdatesEvt;
/*! @} */


/*!
 * @ingroup AssistiveTouch
 * @{
 */
 /*!
 * @struct     AssistiveTouchInformationEvt
 * @brief     This Struct is used to save payload data of AssistiveTouchInformation message.\n
 */ 
typedef struct {
    BOOL IsEnabled;    /**< IsEnabledof BOOL type is used to save IsEnabled value */
} AssistiveTouchInformationEvt;
/*! @} */


/*!
 * @ingroup ExternalAccessoryProtocol
 * @{
 */
 /*!
 * @struct     StartExternalAccessoryProtocolSessionEvt
 * @brief     This Struct is used to save payload data of StartExternalAccessoryProtocolSession message.\n
 */ 
typedef struct {
    uint8_t ExternalAccessoryProtocolIdentifier;            /**< ExternalAccessoryProtocolIdentifier of uint8_t type is used to save ExternalAccessoryProtocolIdentifier value */
    uint16_t ExternalAccessoryProtocolSessionIdentifier;    /**< ExternalAccessoryProtocolSessionIdentifier of uint16_t type is used to save ExternalAccessoryProtocolSessionIdentifier value */
} StartExternalAccessoryProtocolSessionEvt;

 /*!
 * @struct     StopExternalAccessoryProtocolSessionEvt
 * @brief     This Struct is used to save payload data of StopExternalAccessoryProtocolSession message.\n
 */ 
typedef struct {
    uint16_t ExternalAccessoryProtocolSessionIdentifier;    /**< ExternalAccessoryProtocolSessionIdentifier of uint16_t type is used to save ExternalAccessoryProtocolSessionIdentifier value */
} StopExternalAccessoryProtocolSessionEvt;

 /*!
 * @struct     ExternalAccessoryProtocolData
 * @brief     This Struct is used to save ExternalAccessoryProtocol Data .\n
 */ 
typedef struct {
    uint16_t ExternalAccessoryProtocolSessionIdentifier; /**< ExternalAccessoryProtocolSessionIdentifier of uint16_t type is used to save ExternalAccessoryProtocolSessionIdentifier value */
    uint8_t* value;    /**< value of uint8_t* type is used to save ExternalAccessoryProtocol data */
    uint32_t size;    /**< size of uint32_t type is used to save ExternalAccessoryProtocol data size*/
} ExternalAccessoryProtocolData;
/*! @} */


/*!
 * @ingroup USBDeviceModeAudio
 * @{
 */
 /*!
 * @struct     USBDeviceModeAudioInformationEvt
 * @brief     This Struct is used to save payload data of USBDeviceModeAudioInformation message.\n
 */ 
typedef struct {
    uint8_t SampleRate;    /**< SampleRate uint8_t type is used to save SampleRate data */
} USBDeviceModeAudioInformationEvt;
/*! @} */


/*!
 * @ingroup VoiceOver 
 * @{
 */
 /*!
 * @struct     VoiceOverUpdatesEvt
 * @brief     This Struct is used to save payload data of VoiceOverUpdates message.\n
 */
typedef struct {
    SpeakingVolumeParam SpeakingVolume;     /**< Refer @ref SpeakingVolumeParam */     
    SpeakingRateParam SpeakingRate;         /**< Refer @ref SpeakingRateParam */
    BOOL Enabled;                         /**< Enabled BOOL type is used to save Enabled data */    
} VoiceOverUpdatesEvt;


/*!
 * @struct     LableParam
 * @brief     This Struct is used to save Lable data.\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save Lable data */
} LableParam;

/*!
 * @struct     ValueParam
 * @brief     This Struct is used to save Value data.\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save Value data */
} ValueParam;

/*!
 * @struct     HintParam
 * @brief     This Struct is used to save Hint data.\n
 */
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save Hintdata */
} HintParam;

/*!
 * @struct     TraitsParam
 * @brief     This Struct is used to save Traits data.\n
 */
typedef struct {
    BOOL bPresent;            /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;
    uint16_t* value;        /**< value of uint16_t* type is used to save Traits */    
} TraitsParam;

/*!
 * @struct     VoiceOverCursorUpdatesEvt
 * @brief     This Struct is used to save payload data of VoiceOverCursorUpdates messagee.\n
 */
typedef struct {
    LableParam Label;        /**< Refer @ref LableParam */
    ValueParam Value;        /**< Refer @ref ValueParam */
    HintParam Hint;            /**< Refer @ref HintParam */
    TraitsParam Traits;        /**< Refer @ref TraitsParam */
} VoiceOverCursorUpdatesEvt;
/*! @} */


/*!
 * @ingroup WiFiInformationSharing
 * @{
 */
/*!
 * @enum     WiFiInfoRequestStatus
 * @brief     This enum value is used to save RequestStatus of WiFiInformation Message Parameter
 */
typedef enum {
    WiFiInfoRS_Success = 0,                /**< Success */
    WiFiInfoRS_UserDeclined,            /**< User Declined */
    WiFiInfoRS_NIU                        /**< Network Information Unavailable*/
} WiFiInfoRequestStatus;


 /*!
 * @struct     WiFiSSIDParam
 * @brief     This Struct is used to save WiFiSSID data.\n
 */ 
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save WiFiSSID data */
} WiFiSSIDParam;

 /*!
 * @struct     WiFiPassphraseParam
 * @brief     This Struct is used to save WiFiPassphrase data.\n
 */ 
typedef struct {
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save WiFiPassphrase data */
} WiFiPassphraseParam;

 /*!
 * @struct     WiFiInformationEvt
 * @brief     This Struct is used to save payload data of WiFiInformation message.\n
 */ 
typedef struct {
    WiFiInfoRequestStatus RequestStatus;
    WiFiSSIDParam WiFiSSID;
    WiFiPassphraseParam WifiPassphrase;
} WiFiInformationEvt;
/*! @} */



/*!
 * @ingroup ExternalAccessoryProtocol 
 * @{
 */
/*!
 * @struct     ExternalAccessoryProtocolNameParam
 * @brief     This Struct is used to save ExternalAccessoryProtocolNameParam data.\n
 */ 
typedef struct {
    uint32_t size;    /**< size of uint32_t type is used to save size of value param */
    uint8_t* value;    /**< value of uint8_t* type is used to save ExternalAccessoryProtocolName data */
}ExternalAccessoryProtocolNameParam;

/*!
 * @struct     NativeTransportComponentIdentifierParam
 * @brief     This Struct is used to save NativeTransportComponentIdentifierParam data.\n
 */ 
typedef struct{
    BOOL bPresent;    /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint16_t value;    /**< value of uint16_t type is used to save NativeTransportComponentIdentifier data */
} NativeTransportComponentIdentifierParam;

/*!
 * @struct     SupportedExternalAccessoryGroupParam
 * @brief     This Struct is used to save SupportedExternalAccessoryGroup data.\n
 */ 
typedef struct {
    uint8_t ExternalAccessoryProtocolIdentifier;                                /**< ExternalAccessoryProtocolIdentifier of uint8_t type is used to save ExternalAccessoryProtocolIdentifier data */
    ExternalAccessoryProtocolNameParam ExternalAccessoryProtocolName;            /**< Refer @ref ExternalAccessoryProtocolNameParam */
    uint8_t ExternalAccessoryProtocolMatchAction;                                /**< ExternalAccessoryProtocolMatchAction of uint8_t type is used to save ExternalAccessoryProtocolMatchAction data */
    NativeTransportComponentIdentifierParam NativeTransportComponentIdentifier;    /**< Refer @ref NativeTransportComponentIdentifierParam */
}SupportedExternalAccessoryGroupParam;


/*!
 * @struct     SupportedExternalAccessory
 * @brief     This Struct is used to save SupportedExternalAccessory data.\n
 */ 
typedef struct {
    BOOL bPresent;                                                            /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    SupportedExternalAccessoryGroupParam SupportedExternalAccessoryGroup;    /**< Refer @ref SupportedExternalAccessoryGroupParam */
} SupportedExternalAccessory;
/*! @} */

/*!
 * @ingroup Identification 
 * @{
 */
/*!
 * @struct     UnsupportIdentifierParam
 * @brief     This Struct is used to save UnsupportIdentifier data.\n
 */ 
typedef struct {
    BOOL bPresent;                                                            /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    uint8_t* value;                                            /**< pUnsupportIdentifier of uint8_t* type is used to save UnsupportIdentifier data */
    uint32_t size;                                                            /**< size of uint32_t type is used to save count unsupport identifier */
} UnsupportIdentifierParam;

/*!
 * @struct     IdentificationRejectedEvt
 * @brief     This Struct is used to save IdentificationRected data.\n
 */
typedef struct {
    BOOL Name;                                            /**< Name of BOOL type is used to save Name payload data of IdentificationRejected Message */
    BOOL ModelIdentifier;                                /**< ModelIdentifier of BOOL type is used to save ModelIdentifier payload data of IdentificationRejected Message */
    BOOL Manufacturer;                                    /**< Manufacturer of BOOL type is used to save Manufacturer payload data of IdentificationRejected Message */
    BOOL SerialNumber;                                    /**< SerialNumber of BOOL type is used to save SerialNumber payload data of IdentificationRejected Message */
    BOOL FirmwareVersion;                                /**< FirmwareVersion of BOOL type is used to save FirmwareVersion payload data of IdentificationRejected Message */
    BOOL HardwareVersion;                                /**< HardwareVersion of BOOL type is used to save HardwareVersion payload data of IdentificationRejected Message */
    UnsupportIdentifierParam MessagesSentByAccessroy;    /**< MessagesSentByAccessroy of UnsupportIdentifierParam type is used to save MessagesSentByAccessroy payload data of IdentificationRejected Message */
    UnsupportIdentifierParam MessagesRecviedFromDevice;    /**< MessagesRecviedFromDevice of UnsupportIdentifierParam type is used to save MessagesRecviedFromDevice payload data of IdentificationRejected Message */
    BOOL PowerProvidingCapability;                        /**< PowerProvidingCapability of BOOL type is used to save PowerProvidingCapability payload data of IdentificationRejected Message */
    BOOL MaximumCurrentDrawnFromDevice;                    /**< MaximumCurrentDrawnFromDevice of BOOL type is used to save MaximumCurrentDrawnFromDevice payload data of IdentificationRejected Message */
    BOOL SupportedExternalAccessoryProtocl;                /**< SupportedExternalAccessoryProtocl of BOOL type is used to save SupportedExternalAccessoryProtocl payload data of IdentificationRejected Message */
    BOOL AppMatchTeamID;                                /**< AppMatchTeamID of BOOL type is used to save AppMatchTeamID payload data of IdentificationRejected Message */
    BOOL CurrentLanguage;                                /**< CurrentLanguage of BOOL type is used to save CurrentLanguage payload data of IdentificationRejected Message */
    BOOL SupportedLanguage;                                /**< SupportedLanguage of BOOL type is used to save SupportedLanguage payload data of IdentificationRejected Message */
    BOOL SerialTransportComponent;                        /**< SerialTransportComponent of BOOL type is used to save SerialTransportComponent payload data of IdentificationRejected Message */
    BOOL USBDeviceTransportComponent;                    /**< USBDeviceTransportComponent of BOOL type is used to save USBDeviceTransportComponent payload data of IdentificationRejected Message */
    BOOL USBHostTransportComponent;                        /**< USBHostTransportComponent of BOOL type is used to save USBHostTransportComponent payload data of IdentificationRejected Message */
    BOOL BluetoothTransportComponent;                    /**< BluetoothTransportComponent of BOOL type is used to save BluetoothTransportComponent payload data of IdentificationRejected Message */
    BOOL iAP2HIDComponent;                                /**< TODO. May be need fixing param. iAP2HIDComponentCount of uint8_t type is used to save iAP2HIDComponentCount payload data of IdentificationRejected Message  */
    BOOL VehicleInformationComponent;                    /**< VehicleInformationComponent of BOOL type is used to save VehicleInformationComponent payload data of IdentificationRejected Message */
    BOOL VehicleStatusComponent;                        /**< VehicleStatusComponent of BOOL type is used to save VehicleStatusComponent payload data of IdentificationRejected Message */
    BOOL LocationInformationComponent;                    /**< LocationInformationComponent of uint8_t type is used to save LocationInformationComponent payload data of IdentificationRejected Message */
    BOOL USBHostHIDComponent;                            /**< TODO. May be need fixing param. USBHostHIDComponentCount of BOOL type is used to save USBHostHIDComponentCount payload data of IdentificationRejected Message */
    BOOL BluetoothHIDComponent;                            /**< BluetoothHIDComponent of BOOL type is used to save BluetoothHIDComponent payload data of IdentificationRejected Message */
    IdentficationMsgSelectArg mRejectIdentficationMsgSelect;
} IdentificationRejectedEvt;
/*! @} */





/*!
 * @ingroup Communications
 * @{
 */


/*!
 * @enum     CallStateUpdateStatus
 * @brief     This enum value  is used to CallStateUpdateStatus.   
 */
typedef enum {
    DISCONNECTED = 0,                /**< Disconnected */
    SENDING = 1,                    /**< Sending  */
    RINGING = 2,                    /**< Ringing */
    CONNECTING = 3,                    /**< Connecting */
    ACTIVE = 4,                        /**< Active  */
    HELD = 5,                        /**< Held */
    DISCONNECTING = 6                /**< Disconnecting */
}CallStateUpdateStatus;

/*!
 * @enum     CallStateUpdateDirection
 * @brief     This enum value  is used to CallStateUpdateDirection.   
 */
typedef enum {
    UNKNOWN_DIRECTION = 0,                    /**< Unknown */
    INCOMING = 1,                    /**< Incoming */
    OUTGOING = 2                    /**< Outgoing */
}CallStateUpdateDirection;

/*!
 * @enum     CallStateUpdateStatusLegacy
 * @brief     This enum value  is used to CallStateUpdateStatusLLegacy.   
 */
typedef enum {
    Legacy_DISCONNECTED = 0,                /**< Disconnected */
    Legacy_ACTIVE = 1,                             /**< Active  */
    Legacy_HELD = 2,                               /**< Held */
    Legacy_RINGINGANDSEND = 3            /**< RINGINGANDSEND */
}CallStateUpdateStatusLegacy;

/*!
 * @enum     CallStateUpdateDirectionLegacy
 * @brief     This enum value  is used to CallStateUpdateDirectionLegacy.   
 */
typedef enum {
    Legacy_INCOMING = 0,                        /**< Incoming */
    Legacy_OUTGOING = 1,                       /**< Outgoing */
    Legacy_UNKNOWN_DIRECTION = 2       /**< Unknown */
}CallStateUpdateDirectionLegacy;

/*!
 * @enum     CallStateUpdateService
 * @brief     This enum value  is used to CallStateUpdateService.   
 */
 
typedef enum {
    UNKNOWN_SERVICE = 0,                /**< Unknown */
    TELEPHONY = 1,                        /**< Telephony */
    FACETIMEAUDIO = 2,                    /**< FaceTimeAudio */
    FACETIMEVEDIO = 3                    /**< FaceTimeVedio */
}CallStateUpdateService;


/*!
 * @enum     CallStateUpdateDirection
 * @brief     This enum value  is used to CallStateUpdateDirection.   
 */
 
typedef enum {
    NO_REASON = 0,                        /**< No Reason (Call Ended) */
    CALL_DECLINED = 1,                    /**< Call Declined */
    CALL_FAILED = 2                        /**< Call Failed */
}CallStateUpdateDisconnectReason;

/*!
 * @struct     CallStateUpdateDisconnectReasonParam
 * @brief     This Struct is used to save CallStateUpdateDisconnectReason data.  \n
 */
typedef struct {
    BOOL bPresent;                                /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    CallStateUpdateDisconnectReason value;        /**< value of CallStateUpdateDisconnectReason type is used to save DisconnectReason param */
}CallStateUpdateDisconnectReasonParam;

/*!
 * @struct     CallStateUpdatesEvt
 * @brief     This Struct is used to save CallStateUpdates payload data.  \n
 */
typedef struct {
    EvtSaveUtf8Param mRemoteID;                                /**< mRemoteID of SaveUtf8Param type is used to save RemoteID data from apple device */
    EvtSaveUtf8Param mDisplayName;                            /**< mDisplayName of SaveUtf8Param type is used to save DisplayName data from apple device */
    BOOL bLegacy;                                                       /**< bLegacy of BOOL type is used to save support callstateupdatestatuslegacy and callstateupdatedirection parameter. TRUE is legacy informaion param, FALSE is not legacy informationparam, refer 521 page at the Accessory Interface Certification R22*/
    CallStateUpdateStatus eStatus;                            /**< eStatus of CallStateUpdateStatus type is used to save Status data from apple device */
    CallStateUpdateDirection eDirection;                    /**< eDirection of CallStateUpdateDirection type is used to save Direction data from apple device */
    EvtSaveUtf8Param mCallUUID;                                /**< mCallUUID of SaveUtf8Param type is used to save CallUUID data from apple device */
    EvtSaveUtf8Param mAddressBookID;                        /**< mAddressBookID of SaveUtf8Param type is used to save AddressBookID data from apple device */
    EvtSaveUtf8Param mLabel;                                /**< mLabel of SaveUtf8Param type is used to save Label data from apple device */
    CallStateUpdateService eService;                        /**< eService of BOOL type is used to save Service data from apple device */
    BOOL bIsConferenced;                                    /**< bIsConferenced of BOOL type is used to save IsConferenced data from apple device */
    uint8_t nConferenceGroup;                                /**< nConferenceGroup of BOOL type is used to save ConferenceGroup data from apple device */
    CallStateUpdateDisconnectReasonParam mDisconnectReason;    /**< mDisconnectReason of CallStateUpdateDisconnectReason type is used to save DisconnectReason data from apple device */
} CallStateUpdatesEvt;


/*!
 * @enum     SignalStrength
 * @brief     This enum value  is used to CommunicationsUpdateSignalStrength.   
 */
 
typedef enum {
    BARS_0_ = 0,                        /**< 0_Bars */
    BARS_1 = 1,                        /**< 1_Bars */
    BARS_2 = 2,                        /**< 2_Bars */
    BARS_3 = 3,                        /**< 2_Bars */
    BARS_4 = 4,                        /**< 2_Bars */
    BARS_5 = 5                        /**< 2_Bars */    
}SignalStrengthEnum;

/*!
 * @enum     RegistrationStatus
 * @brief     This enum value  is used to CommunicationsUpdateRegistrationStatus.   
 */
 
typedef enum {
    UNKNOWN = 0,                                /**< Unkown */
    NOT_REGISTERED = 1,                            /**< Not Registered */
    SEARCHING = 2,                                /**< Searching */
    DENIED = 3,                                    /**< Denied */
    REGISTERED_HOME = 4,                        /**< Registered Home */
    REGISTERED_ROAMING = 5,                        /**< Registered Roaming */    
    EMERGENCY_CALL_ONLY = 6                        /**< Emergency Call Only */        
}RegistrationStatusEnum;


/*!
 * @struct     SignalStrengthParam
 * @brief     This Struct is used to save SignalStrength data.  \n
 */
typedef struct {
    BOOL bPresent;                                /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    SignalStrengthEnum value;                    /**< value of SignalStrengthEnum type is used to save SignalStrength data.*/
}SignalStrengthParam;

/*!
 * @struct     RegistrationStatusParam
 * @brief     This Struct is used to save RegistrationStatus data.  \n
 */
typedef struct {
    BOOL bPresent;                                /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    RegistrationStatusEnum value;                /**< value of RegistrationStatusEnum type is used to save  RegistrationStatus data. */
}RegistrationStatusParam;

/*!
 * @struct     CommunicationsUpdatesEvt
 * @brief     This Struct is used to save CommunicationsUpdates payload data.  \n
 */
typedef struct {
    SignalStrengthParam mSignalStrength;            /**< eSignalStrength of SignalStrengthEnum type is used to save SignalStrength data from apple device */
    RegistrationStatusParam mRegistrationStatus;    /**< eRegistrationStatus of RegistrationStatusEnum type is used to save RegistrationStatus data from apple device */
    BOOL bAirplaneModeStatus;                    /**< bAirplaneModeStatus of BOOL type is used to save AirplaneModeStatus data from apple device */
    EvtSaveUtf8Param mCarrierName;                /**< mCarrierName of SaveUtf8Param type is used to save CarrierName data from apple device */
    BOOL bCelluarSupported;                        /**< bCelluarSupported of BOOL type is used to save CelluarSupported data from apple device */
    BOOL bTelephonyEnabled;                        /**< bTelephonyEnabled of BOOL type is used to save TelephonyEnabled data from apple device */
    BOOL bFaceTimeAudioEnabled;                    /**< bFaceTimeAudioEnabled of BOOL type is used to save FaceTimeAudioEnabled data from apple device */
    BOOL bFaceTimeVideoEnabled;                    /**< bFaceTimeVideoEnabled of BOOL type is used to save FaceTimeVideoEnabled data from apple device */
    BOOL bMuteStatus;                            /**< bMuteStatus of BOOL type is used to save MuteStatus data from apple device */
    EvtSaveUint8Param mCurrentCallCount;        /**< mCurrentCallCount of SaveUint8Param type is used to save CurrentCallCount data from apple device */
    EvtSaveUint8Param mNewVoicemailCount;        /**< mNewVoicemailCount of SaveUint8Param type is used to save NewVoicemailCount data from apple device */
    BOOL bInitiateCallAvailable;                /**< bInitiateCallAvailable of BOOL type is used to save InitiateCallAvailable data from apple device */
    BOOL bEndAndAcceptAvailable;                /**< bEndAndAcceptAvailable of BOOL type is used to save EndAndAcceptAvailable data from apple device */
    BOOL bHoldAndAccptAvailable;                /**< bHoldAndAccptAvailalbe of BOOL type is used to save HoldAndAccptAvailalbe data from apple device */
    BOOL bSwapAvailable;                        /**< bSwapAvailable of BOOL type is used to save SwapAvailable data from apple device */
    BOOL bMergeAvailable;                        /**< bMergeAvailable of BOOL type is used to save MergeAvailable data from apple device */
    BOOL bHoldAvailable;                        /**< bHoldAvailable of BOOL type is used to save HoldAvailable data from apple device */    
} CommunicationsUpdatesEvt;


/*!
 * @struct     SaveRecvBoolParam
 * @brief     This Struct is used to save SaveRecvBoolParam data.  \n
 */
typedef struct {
    BOOL bPresent;                                /**< bPresent of BOOL type is used to confirm whether value is present, if value is present then set TRUE, otherwise FALSE*/
    BOOL value;                                    /**< value of BOOL type is used to save  Boo data. */
}SaveRecvBoolParam;

/*!
 * @enum     ListUpdateServiceEnum
 * @brief     This enum value  is used to Service value of ListUpdate.   
 */
typedef enum {
    LUS_UNKNOWN = 0,                            /**< UNKNOWN */
    LUS_TELEPHONY = 1,                            /**< TELEPHONY */
    LUS_FACETIMEAUDIO = 2,                        /**< FACETIMEAUDIO */
    LUS_FACETIMEVIDEO = 3                        /**< FACETIMEVIDEO */
}ListUpdateServiceEnum;

/*!
 * @enum     ListUpdateServiceEnum
 * @brief     This enum value  is used to Service value of ListUpdate.   
 */
typedef enum {
    LUT_UNKNOWN = 0,                            /**< LUT_UNKNOWN */
    LUT_INCOMING = 1,                            /**< LUT_INCOMING */
    LUT_OUTGOING = 2,                            /**< LUT_OUTGOING */
    LUT_MISSED = 3                                /**< LUT_MISSED */
}ListUpdateTypeEnum;    


/*!
 * @struct     RecentListGroup
 * @brief     This Struct is used to save RecentListGroup payload data. \n
 */
typedef struct {
    BOOL bPresent;                        /**< bPresent of BOOL type is used to confirm whether MediaItemGroup is present, if RecentListGroup is present then set TRUE, otherwise FALSE */
    uint16_t nIndex;                     /**< nIndex of uint16_t type is used to save Index  */
    EvtSaveUtf8Param  mRemoteID;         /**< mRemoteID of EvtSaveUtf8Param type is used to save RemoteID  */
    EvtSaveUtf8Param  mDisplayName;     /**< mDisplayName of EvtSaveUtf8Param type is used to save DisplayName  */
    EvtSaveUtf8Param  mLabel;             /**< mLabel of EvtSaveUtf8Param type is used to save Lable  */
    EvtSaveUtf8Param  mAddressBookID;     /**< mAddressBookID of EvtSaveUtf8Param type is used to save AddressBookID  */
    ListUpdateServiceEnum eServcie;     /**< eServcie of ListUpdateServiceEnum type is used to save Servcie  */
    ListUpdateTypeEnum eType;             /**< eType of ListUpdateTypeEnum type is used to save Type  */
    EvtSaveUint64Param mUnixTimestamp;  /**< mUnixTimestamp of EvtSaveUint64Param type is used to save UnixTimestamp  */
    EvtSaveUint32Param mDuration;          /**< mDuration of EvtSaveUint32Param type is used to save Duration  */
    uint8_t  nOccurrences;                 /**< nOccurrences of uint8_t type is used to save Occurrences  */
} RecentListGroup ;


/*!
 * @struct     RecentListGroup
 * @brief     This Struct is used to save RecentListGroup payload data. \n
 */
typedef struct {
    BOOL bPresent;                        /**< bPresent of BOOL type is used to confirm whether MediaItemGroup is present, if RecentListGroup is present then set TRUE, otherwise FALSE */
    uint16_t nIndex;                     /**< nIndex of uint16_t type is used to save Index  */
    EvtSaveUtf8Param  mRemoteID;         /**< mRemoteID of EvtSaveUtf8Param type is used to save RemoteID  */
    EvtSaveUtf8Param  mDisplayName;     /**< mDisplayName of EvtSaveUtf8Param type is used to save DisplayName  */
    EvtSaveUtf8Param  mLabel;             /**< mLable of EvtSaveUtf8Param type is used to save Lable  */
    EvtSaveUtf8Param  mAddressBookID;     /**< mAddressBookID of EvtSaveUtf8Param type is used to save AddressBookID  */
    ListUpdateServiceEnum eServcie;     /**< eServcie of ListUpdateServiceEnum type is used to save Servcie  */
} FavoritesListGroup ;

/*!
 * @struct     ListUpdateEvt
 * @brief     This Struct is used to save ListUpdate payload data.  \n
 */
typedef struct {
    SaveRecvBoolParam mRectsListAavailable;                        /**< mRectsListAavailable of RegistrationStatusParam type is used to save RectsListAavailable data from apple device */
    uint32_t nRecentListSize;                                      /**< nRecentListSize of uint32_t type is used to save RecentListCount data */
    uint32_t nRecentListGroupCount;                                /**< nRecentListGroupCount of uint32_t type is used to save RecentListGroupCount data */
    RecentListGroup* pRecentListGroup;                             /**< pRecentListGroup of RecentListGroup* type is used to save RecentListGroup  */    
    EvtSaveUint16Param mRecentListCount;                           /**< mRecentListCount of EvtSaveUint16Param type is used to save RecentListCount data */
    SaveRecvBoolParam mFavoritesListAavailable;                    /**< mFavoritesListAavailable of SaveRecvBoolParam type is used to save FavoritesListAavailable data */
    uint32_t nFavoritesListSize;                                   /**< nFavoritesListSize of uint32_t type is used to save RecentListCount data */
    uint32_t nFavoritesListGroupCount;                             /**< nFavoritesListGroupCount of uint32_t type is used to save FavoritesListGroupCount data */
    FavoritesListGroup* pFavoritesListGroup;                       /**< pRecentListGroup of RecentListGroup* type is used to save RecentListGroup  */
    EvtSaveUint16Param mFavoritesListCount;                        /**< mFavoritesListCount of EvtSaveUint16Param type is used to save FavoritesListCount data */    
} ListUpdateEvt;
/*! @} */

// END Yongil


/*-------------------------------------------------------------------------------------------------------------------------------------------*/
// Event From iOS Device
typedef ConnectionStateUpdateEvt ConnectionStateUpdateCbArg;                                    /**< Refer @ref ConnectionStateUpdateEvt */
typedef ConnectionStateAndPathUpdateEvt ConnectionStateAndPathUpdateCbArg;                        /**< Refer @ref ConnectionStateAndPathUpdateEvt */    
typedef ConnectionModeUpdateEvt ConnectionModeUpdateCbArg;                                        /**< Refer @ref ConnectionModeUpdateEvt */
typedef ConnectionCarplaySupportUpdateEvt ConnectionCarplaySupportUpdateCbArg;                    /**< Refer @ref ConnectionCarplaySupportUpdateEvt */
typedef ConnectionIap2SupportUpdateEvt ConnectionIap2SupportUpdateCbArg;                        /**< Refer @ref ConnectionIap2SupportUpdateEvt */
typedef ConnectionInfoUpdateEvt ConnectionInfoUpdateCbArg;                                        /**< Refer @ref ConnectionInfoUpdateEvt */

#ifdef ANDROID
typedef ConnectionDeviceInfoUpdateEvt ConnectionIap2DeviceInfoUpdateCbArg;                        /**< Refer @ref ConnectionDeviceInfoUpdateEvt */
#endif // #ifdef ANDROID

typedef PowerUpdateEvt PowerUpdateCbArg;                                                        /**< Refer @ref PowerUpdateEvt */
typedef NowPlayingUpdateEvt NowPlayingUpdateCbArg;                                                /**< Refer @ref NowPlayingUpdateEvt */
// #ifdef iAP2SERVICE_FILETRANSFER

// #endif
typedef StartVehicleStatusUpdatesEvt StartVehicleStatusUpdatesCbArg;                            /**< Refer @ref StartVehicleStatusUpdatesEvt */
typedef StartLocationInformationEvt StartLocationInformationCbArg;                                /**< Refer @ref StartLocationInformationEvt */
typedef DeviceInformationUpdateEvt DeviceInformationUpdateCbArg;                                /**< Refer @ref DeviceInformationUpdateEvt */
typedef DeviceLanguageUpdateEvt DeviceLanguageUpdateCbArg;                                        /**< Refer @ref DeviceLanguageUpdateEvt */
typedef TelephonyCallStateInformationEvt TelephonyCallStateInformationCbArg;                    /**< Refer @ref TelephonyCallStateInformationEvt */
typedef TelephonyUpdateEvt TelephonyUpdateCbArg;                                                /**< Refer @ref TelephonyUpdateEvt */
typedef BTConnectionUpdateEvt BTConnectionUpdateCbArg;                                            /**< Refer @ref BTConnectionUpdateEvt */

//ADD Yongil 2014.10.23

typedef DeivceHIDReportEvt DeviceHIDReportCbArg;                                                /**< Refer @ref DeivceHIDReportEvt */
typedef MediaLibraryInformationEvt MeidaLibraryInformatonCbArg;                                    /**< Refer @ref MediaLibraryInformationEvt */    
typedef MediaLibraryUpdatesEvt MediaLibraryUpdatesCbArg;                                        /**< Refer @ref MediaLibraryUpdatesEvt */
typedef AssistiveTouchInformationEvt AssistiveTouchInformationCbArg;                            /**< Refer @ref AssistiveTouchInformationEvt */
typedef StartExternalAccessoryProtocolSessionEvt StartExternalAccessoryProtocolSessionCbArg;    /**< Refer @ref StartExternalAccessoryProtocolSessionEvt */    
typedef StopExternalAccessoryProtocolSessionEvt StopExternalAccessoryProtocolSessionCbArg;        /**< Refer @ref StopExternalAccessoryProtocolSessionEvt */
typedef ExternalAccessoryProtocolData ExternalAccessoryProtocolDataCbArg;                        /**< Refer @ref ExternalAccessoryProtocolData */
typedef USBDeviceModeAudioInformationEvt USBDeviceModeAudioInformationCbArg;                    /**< Refer @ref USBDeviceModeAudioInformationEvt */
typedef VoiceOverUpdatesEvt VoiceOverUpdatesCbArg;                                                /**< Refer @ref VoiceOverUpdatesEvt */
typedef VoiceOverCursorUpdatesEvt VoiceOverCursorUpdatesCbArg;                                    /**< Refer @ref VoiceOverCursorUpdatesEvt */
typedef WiFiInformationEvt WiFiInformationCbArg;                                                /**< Refer @ref WiFiInformationEvt */
typedef FileTransferUpdateEvt FileTransferUpdateArg;                                    /**< Refer @ref FileTransferEvt */
typedef CallStateUpdatesEvt CallStateUpdatesCBArg;                                                /**< Refer @ref CallStateUpdatesEvt */
typedef CommunicationsUpdatesEvt CommunicationsUpdatesCBArg;                                    /**< Refer @ref CommunicationsUpdatesEvt */
typedef ListUpdateEvt ListUpdateCBArg;                                                            /**< Refer @ref ListUpdateEvt */
// END Yongil        


/*!
 * @defgroup iAP2Eventvalue
 * @brief iAP2 Event type and Event list (Event is used to confirm message type.)
 * @{
 */
//===========================================================================================================================
// Events
//===========================================================================================================================
#define kEventBase                                            (20000)                                /**< Base of iAP2 Event value */
#define kInternalEventBase                                    (kEventBase+1000)                    /**< Base of iAP2 Internal Event value */
#define kExternalEventBase                                    (kEventBase+2000)                    /**< Base of iAP2 External Event value */

// Connection State
#define kEvtConnectionStateUpdate                                (kInternalEventBase+1)            /**< value of ConnectionStateUpdate Event */
#define kEvtConnectionStateAndPathUpdate                        (kInternalEventBase+2)            /**< value of ConnectionStateAndPathUpdate Event */

// Connection Mode
#define kEvtConnectionModeUpdate                                (kInternalEventBase+3)            /**< value of ConnectionModeUpdate Event */

// Carplay Support
#define kEvtConnectionCarplaySupportUpdate                        (kInternalEventBase+4)            /**< value of ConnectionCarplaySupportUpdate Event */

// iAP2 support
#define kEvtConnectionIap2SupportUpdate                        (kInternalEventBase+5)                /**< value of ConnectionIap2SupportUpdate Event */
#ifdef ANDROID
// iAP2 deviceinfo
#define kEvtConnectionIap2DeviceInfoUpdate                        (kInternalEventBase+6)            /**< value of ConnectionIap2DeviceInfoUpdate Event */
#endif

#define kEvtConnectionInfoUpdate                                    (kInternalEventBase+7)        /**< value of ConnectionInfoUpdate Event */
//Power
#define kEvtPowerUpdate                                        (kExternalEventBase+1)                /**< value of PowerUpdate Event */

// NowPlaying
#define kEvtNowPlayingUpdate                                    (kExternalEventBase+11)            /**< value of NowPlayingUpdate Event */
#define kEvtNowPlayingAlbumArtworkUpdate                        (kExternalEventBase+12)            /**< value of NowPlayingAlbumArtworkUpdate Event */

// Vehicle Status
#define kEvtStartVehicleStatusUpdates                            (kExternalEventBase+21)            /**< value of StartVehicleStatusUpdates Event */
#define kEvtStopVehicleStatusUpdates                            (kExternalEventBase+22)            /**< value of StopVehicleStatusUpdates Event */

// Location
#define kEvtStartLocationInformation                            (kExternalEventBase+31)            /**< value of StartLocationInformation Event */
#define kEvtStopLocationInformation                            (kExternalEventBase+32)                /**< value of tStopLocationInformation Event */

// Device Notification
#define kEvtDeviceInformationUpdate                            (kExternalEventBase+41)                /**< value of DeviceInformationUpdate Event */
#define kEvtDeviceLanguageUpdate                                (kExternalEventBase+42)            /**< value of DeviceLanguageUpdate Event */

// Telephony
#define kEvtTelephonyCallStateInformation                        (kExternalEventBase+51)            /**< value of TelephonyCallStateInformation Event */
#define kEvtTelephonyUpdate                                    (kExternalEventBase+52)                /**< value of TelephonyUpdate Event */

// Bluetooth Connection
#define kEvtBTConnectionUpdate                                    (kExternalEventBase+61)            /**< value of BTConnectionUpdate Event */

// ADD Yongil 2014.10.23

// HID
#define kEvtDeviceHIDReport                                    (kExternalEventBase+71)                /**< value of DeviceHIDReport Event */

// MediaLibraryInformation
#define kEvtMediaLibraryInformation                            (kExternalEventBase+81)                /**< value of MediaLibraryInformation Event */

// MediaLibraryUpdates
#define kEvtMediaLibraryUpdates                                (kExternalEventBase+91)                /**< value of MediaLibraryUpdates Event */

// AssistiveTouch
#define kEvtAssistiveTouchInformation                            (kExternalEventBase+101)        /**< value of AssistiveTouchInformation Event */

// ExternalAccessoryProtocol
#define kEvtStartExternalAccessoryProtcolSession                (kExternalEventBase+111)        /**< value of StartExternalAccessoryProtcolSession Event */    
#define kEvtStopExternalAccessoryProtcolSession                (kExternalEventBase+112)            /**< value of StopExternalAccessoryProtcolSession Event */
#define kEvtExternalAccessoryProtcolData                    (kExternalEventBase+113)            /**< value of kEvtExternalAccessoryProtcolData data */

//USBDeviceModeAudio
#define kEvtUSBDeviceModeAudioInformation                        (kExternalEventBase+121)        /**< value of USBDeviceModeAudioInformation Event */    

//VoiceOverUpdate
#define kEvtVoiceOverUpdates                                    (kExternalEventBase+131)        /**< value of VoiceOverUpdates Event */
#define kEvtVoiceOverCursorUpdates                                (kExternalEventBase+132)        /**< value of VoiceOverCursorUpdates Event */

//WiFiInformation
#define kEvtWiFiInformation                                    (kExternalEventBase+141)            /**< value of WiFiInformation Event */

//FileTransfer
#define kEvtFileTransferUpdate                        (kExternalEventBase+151)                    /**< value of FileTransferUpdate Event */

//Communications
#define kEvtCallStateUpdate                                    (kExternalEventBase+161)            /**< value of CallStateUpdate Event */
#define kEvtCommunicationsUpdate                            (kExternalEventBase+162)            /**< value of CommunicationsUpdate Event */
#define kEvtListUpdateUpdate                                (kExternalEventBase+163)            /**< value of ListUpdateUpdate Event */

//Authentication
#define kEvtAuthenticationFailed                            (kExternalEventBase+171)            /**< value of AuthenticationFailed Event */

//Identification
#define kEvtIdentificationFailed                            (kExternalEventBase+181)            /**< value of AuthenticationFailed Event */

/*! @} */
#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKEXTERNAL_EVENT_H_ */

