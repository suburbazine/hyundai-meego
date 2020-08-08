/*
    File:       IAP2PredefinedValues.h
    Package:    LG iAP2 Service
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

#ifndef IAP2PREDEFINEDVALUES_H
#define IAP2PREDEFINEDVALUES_H

#include <linux/types.h>
#include <string>

#define STR_Identification_Name "AVN"
#define STR_Identification_ModelIdentifier "LGE KOREA"
#define STR_Identification_Manufacturer "LG Electronics"
#define STR_Identification_SerialNumber "1234ABCDE"
#define STR_Identification_FirmwareVersion "1.0"
#define STR_Identification_HardwareVersion "1.0"
#define STR_Identification_CurrentLanguage "ko"

enum {
    kSubParamIdIdentificationTransportComponentIdentifier = 0,
    kSubParamIdIdentificationTransportComponentName = 1,
    kSubParamIdIdentificationTransportSupportsiAP2Connection = 2,
    kSubParamIdIdentificationUsbDeviceSupportedAudioSampleRate = 3,

    kSubParamIdIdentificationHostTransportComponentIdentifier = 0,
    kSubParamIdIdentificationHostTransportComponentName = 1,
    kSubParamIdIdentificationHostTransportSupportsiAP2Connection = 2,
    kSubParamIdIdentificationUsbHostTransportCarplayInterfaceNumber = 3,
    kSubParamIdIdentificationUsbHostTransportSupportsCarPlay = 4,
    
    kSubParamIdIdentificationExternalAccessoryProtocolIdentifier = 0,
    kSubParamIdIdentificationExternalAccessoryProtocolName = 1,
    kSubParamIdIdentificationExternalAccessoryProtocolMatchAction = 2,
    kSubParamIdIdentificationNativeTransportComponentIdentifier = 3,

    kSubParamIdIdentificationHidComponentIdentifier = 0,
    kSubParamIdIdentificationHidComponentName = 1,
    kSubParamIdIdentificationHidComponentFunction = 2,

    kSubParamIdIdentificationBTTransportIdentifier = 0,
    kSubParamIdIdentificationBTTransportName = 1,
    kSubParamIdIdentificationBTTransportSupportsiAP2Connection = 2,
    kSubParamIdIdentificationBTTransportMediaAccessControlAddress = 3,

    kSubParamIdIdentificationVehicleInformationIdentifier = 0,
    kSubParamIdIdentificationVehicleInformationName = 1,
    kSubParamIdIdentificationVehicleInformationEngineType = 2,
    kSubParamIdIdentificationVehicleInformationDisPlayName = 6,

    kSubParamIdIdentificationVehicleStatusIdentifier = 0,
    kSubParamIdIdentificationVehicleStatusName = 1,
    kSubParamIdIdentificationVehicleStatusRange = 3,
    kSubParamIdIdentificationVehicleStatusOutsideTemp = 4,
    kSubParamIdIdentificationVehicleStatusRangeWarning = 6,

    kSubParamIdIdentificationLocationIdentifier = 0,
    kSubParamIdIdentificationLocationName = 1,
    kSubParamIdIdentificationLocationGPSFixData = 17,
    kSubParamIdIdentificationLocationRecommendedMinSpecGPSTransitData = 18,
    kSubParamIdIdentificationLocationGPSStaellitesInView = 19,
    kSubParamIdIdentificationLocationVehicleSpeedData = 20,
    kSubParamIdIdentificationLocationVehicleGyroData = 21,
    kSubParamIdIdentificationLocationVehicleAcceelData = 22,
    kSubParamIdIdentificationLocationVehicleHeadingData = 23,

    kSubParamIdNowPlayingMediaItemPersistId = 0,
    kSubParamIdNowPlayingMediaItemTitle = 1,
    kSubParamIdNowPlayingMediaItemMediaType = 2,
    kSubParamIdNowPlayingMediaItemRating = 3,
    kSubParamIdNowPlayingMediaItemPlaybackDurationInMs = 4,
    kSubParamIdNowPlayingMediaItemPersistIdentifier = 5,
    kSubParamIdNowPlayingMediaItemAlbumTitle = 6,
    kSubParamIdNowPlayingMediaItemAlbumTrackNumber = 7,
    kSubParamIdNowPlayingMediaItemAlbumTrackCount = 8,
    kSubParamIdNowPlayingMediaItemAlbumDiscNumber = 9,
    kSubParamIdNowPlayingMediaItemAlbumDiscCount = 10,
    kSubParamIdNowPlayingMediaItemArtistPersistentId = 11,
    kSubParamIdNowPlayingMediaItemArtist = 12,
    kSubParamIdNowPlayingMediaItemAlbumArtistPersistentId = 13,
    kSubParamIdNowPlayingMediaItemAlbumArtist = 14,
    kSubParamIdNowPlayingMediaItemGenrePersistentId = 15,
    kSubParamIdNowPlayingMediaItemGenre = 16,
    kSubParamIdNowPlayingMediaItemComposerPersistId = 17,
    kSubParamIdNowPlayingMediaItemComposer = 18,
    kSubParamIdNowPlayingMediaItemIsPartOfCompilation = 19,
    kSubParamIdNowPlayingMediaItemIsLikeSupported = 21,
    kSubParamIdNowPlayingMediaItemIsBanSupported = 22,
    kSubParamIdNowPlayingMediaItemIsLiked = 23,
    kSubParamIdNowPlayingMediaItemIsBanned = 24,
    kSubParamIdNowPlayingMediaItemIsResidentOnDevice = 25,
    kSubParamIdNowPlayingMediaItemArtworkFileTransferIdentifier = 26,
    kSubParamIdNowPlayingMediaItemChapterCount = 27,

    kSubParamIdNowPlayingPlaybackStatus = 0,
    kSubParamIdNowPlayingPlaybackElapsedTimeMs = 1,
    kSubParamIdNowPlayingPlaybackQueueIndex = 2,
    kSubParamIdNowPlayingPlaybackQueueCount = 3,
    kSubParamIdNowPlayingPlaybackQueueChapterIndex = 4,
    kSubParamIdNowPlayingPlaybackShuffleMode = 5,
    kSubParamIdNowPlayingPlaybackRepeatMode = 6,
    kSubParamIdNowPlayingPlaybackAppName = 7,
    kSubParamIdNowPlayingPlaybackMediaLibraryUniqueIdentifier = 8,
    kSubParamIdNowPlayingPBiTunesRadioAd = 9,
    kSubParamIdNowPlayingPBiTunesRadioStationName = 10,
    kSubParamIdNowPlayingPBiTunesRadioStationMediaPlaylistPersistentID = 11,
    kSubParamIdNowPlayingPBSpeed = 12,
    kSubParamIdNowPlayingSetElapsedTimeAvailable = 13,
    kSubParamIdNowPlayingPBQueueListAvail = 14,
    kSubParamIdNowPlayingPBQueueListTransferID = 15,
    kSubParamIdNowPlayingPBAppBundleID = 16,
    
    kSubParamIdRecentListPropoperties_Index = 0,
    kSubParamIdRecentListPropoperties_RemoteID = 1,
    kSubParamIdRecentListPropoperties_DisplayName = 2,
    kSubParamIdRecentListPropoperties_Label = 3,
    kSubParamIdRecentListPropoperties_AddressBookID = 4,
    kSubParamIdRecentListPropoperties_Service = 5,
    kSubParamIdRecentListPropoperties_Type = 6,
    kSubParamIdRecentListPropoperties_UnixTimestamp = 7,
    kSubParamIdRecentListPropoperties_Duration = 8,
    kSubParamIdRecentListPropoperties_Occurrences = 9,

    kSubParamIdFavoritesListPropoperties_Index = 0,
    kSubParamIdFavoritesListPropoperties_RemoteID = 1,
    kSubParamIdFavoritesListPropoperties_DisplayName = 2,
    kSubParamIdFavoritesListPropoperties_Label = 3,
    kSubParamIdFavoritesListPropoperties_AddressBookID = 4,
    kSubParamIdFavoritesListPropoperties_Service = 5
};

enum {
    kParamIdIdentification_Name = 0,
    kParamIdIdentification_ModelIdentifier = 1,
    kParamIdIdentification_Manufacturer = 2,
    kParamIdIdentification_SerialNumber = 3,
    kParamIdIdentification_FirmwareVersion = 4,
    kParamIdIdentification_HardwareVersion = 5,
    kParamIdIdentification_MsgSentByAccessory = 6,
    kParamIdIdentification_MsgReceivedFromDevice = 7,
    kParamIdIdentification_PowerSourceType = 8,
    kParamIdIdentification_MaximumCurrent = 9,
    kParamIdIdentification_SupportedEAP = 10,
    kParamIdIdentification_AppMatchTeamID = 11,
    kParamIdIdentification_CurrentLanguage = 12,
    kParamIdIdentification_SupportedLanguage = 13,
    kParamIdIdentification_SerialTransportComponent = 14,
    kParamIdIdentification_USBDeviceTransportComponent = 15,
    kParamIdIdentification_USBHostTransportComponent = 16,
    kParamIdIdentification_BluetoothTransportComponent = 17,
    kParamIdIdentification_iAP2HIDComponent = 18,
    kParamIdIdentification_VehicleInformationComponent = 20,
    kParamIdIdentification_VehicleStatusComponent = 21,
    kParamIdIdentification_LocationInformationComponent = 22,
    kParamIdIdentification_USBHostHIDComponent = 23,

    kParamIdNowPlayingMediaItemAttributes = 0,
    kParamIdNowPlayingPlaybackAttributes = 1,
    kParamIdSetNowPlayingInformation_ElapsedTime = 0,
    kParamIdSetNowPlayingInformation_PlayBackQueueIndex = 1,

    kParamIdHidHIDComponentIdentifier = 0,
    kParamIdHidHIDReport = 1,
    kParamIdHidVendorIdentifier = 1,
    kParamIdHidProductIdentifier = 2,
    kParamIdHidLocalizedKeyboardCountryCode = 3,
    kParamIdHidHIDReportDescriptor = 4,

    kParamIdPowerMaximumCurrentDrawnFromAccessory = 0,
    kParamIdPowerDeviceBatteryWillChargeIfPowerIsPresent = 1,
    kParamIdPowerAccessoryPowerMode = 2,
    kParamIdIsExternalChargerConnected = 4,
    kParamIdBatteryChargingState = 5,
    kParamIdBatteryChargeLevel = 6,

    kParamIdPowerAvailableCurrentForDevice = 0,
    kParamIdPowerDeviceBatteryShouldChargeIfPowerIsPresent = 1,

    kParamIdVehicleStatusRange = 3,
    kParamIdVehicleStatusOutSideTemperature = 4,
    kParamIdVehicleStatusRangeWarning = 6,

    kParamIdDeviceName = 0,

    kParamIdDeviceLanguage = 0,

    kParamIdCallStatevCardTransferID = 5,

    kParamIdTelephonySignalStrength = 0,
    kParamIdTelephonyRegisStatus = 1,
    kParamIdTelephonyAirplaneModeStatus = 2,
    kParamIdTelephonyTTYStatus = 3,
    kParamIdMobileOperator = 4,

    kParamIdCallStatePhoneNumber = 0,
    kParamIdCallStateNetSentCallerNumber = 1,
    kParamIdCallStateStatus = 2,
    kParamIdCallStateDirection = 3,
    kParamIdUniqueCallID = 4,
    kParamIdCallStatevCardFTID = 5,

    kParamIdBTComponentStatus = 0,

    kParamIdComponentID = 0,
    kParamIdComponentEnabled = 1,

    kParamIdTransportComponentID = 0,
    kParamIdTransportComponentName = 1,
    kParamIdTransportSupportsiAP2Connection = 2,
    kParamIdBTtransportMediaAccessControlAddress = 3,

    kParamIdConnectedBTProfiles = 1,

    kParamBTHansFree = 0,
    kParamBTPhoneBookAccess = 1,
    kParamBTAudioVideoRemoteControl = 3,
    kParamBTAdvanceAudioDistribution = 4,
    kParamBTHumanInterfaceDevice = 5,
    kParamBTiAP2Link = 7,
    kParamBTPANAP = 8,
    kParamMessageAccess = 9,
    kParamBTPANC = 12,
    

    kParamIdGPSFixData = 1,
    kParamIdRecommMinSpecificGPSTransitData = 2,
    kParamIdGPSStatelliteInView = 3,
    kParamIdVehicleSpeedData = 4,
    kParamIdVehicleGyroData = 5,
    kParamIdVehicleAccelData = 6,
    kParamIdVehicleHeadingData = 7,

//ADD Yongil 2014.10.23
    // HID
    kParamIDHIDComponentIdentifier = 0,
    kParamIDVendorIdentifier = 1,
    kParamIDProductIdentifier = 2,
    kParamIDLocalizedKeyboardCountryCode = 3,
    kParamIDHIDReportDescriptor= 4,

    // Accessory / Device HID Report
    kParamIDHIDReport = 1,

    // MediaLibraryInformation
    kParamIDMediaLibraryInformation = 0,
    kParamIDMediaLibraryName = 0,
    kParamIDMediaLibraryUniqueIdentifier_Info  = 1,
    kParamIDMediaLibraryType = 2,

    //StartMediaLibraryUpdate
    kParamIDMediaLibraryUniqueIdentifier_startupdate = 0,
    kParamIDLastKnownMediaLibraryRevision = 1,
    kParamIDMediaItemProperties  = 2,
    kParamIDMediaPlayListProperties = 3,
    kParamIDMediaLibraryUpdateProgress_Start = 4,
    kParamIDMediaLibraryIsHidingRemoteItems_Start = 5,

    //PlayMediaLibraryCurrentSelection
    kParamMediaLibraryUniqueIdentifier_CurrentSelection = 0,

    //PlayMediaLibraryItems
    kParamItemsPersistentIdentifier = 0,
    kParamItemsStartingIndex = 1,
    kParamMediaLibraryUniqueIdentifier_Items = 2,

    //PlayMediaLibraryCollections
    kParamCollectionPersistentIdentifier = 0,
    kParamCollectionType = 1,
    kParamCollectionStartingIndex = 2,
    kParamMediaLibraryUniqueIdentifier_Collection = 3,

     //PlayMediaLibraryCollections
    kParamMediaLibraryUniqueIdentifier_Special = 0,
    kParamSpecialAllSongs = 1,

    // StartMediaLibraryUpdate MediaItemProperties Group
    kParamIDMediaItemPropertyPersistentIdentifier = 0, // Always presents value
    kParamIDMediaItemPropertyTitle = 1,
    kParamIDMediaItemPropertyMediaType = 2,
    kParamIDMediaItemPropertyRating = 3,
    kParamIDMediaItemPropertyPlaybackDurationInMilliseconds = 4,
    kParamIDMediaItemPropertyAlbumPersistentIdentifier = 5,
    kParamIDMediaItemPropertyAlbumTitle  = 6,
    kParamIDMediaItemPropertyAlbumTrackNumber = 7,
    kParamIDMediaItemPropertyAlbumTrackCount = 8,
    kParamIDMediaItemPropertyAlbumDiscNumber = 9,
    kParamIDMediaItemPropertyAlbumDiscCount = 10,
    kParamIDMediaItemPropertyArtistPersistentIdentifier = 11,
    kParamIDMediaItemPropertyArtist = 12, 
    kParamIDMediaItemPropertyAlbumAritistPersistentIdentifier = 13, 
    kParamIDMediaItemPropertyAlbumAritist = 14,
    kParamIDMediaItemPropertyGenrePersistentIdentifier = 15,
    kParamIDMediaItemPropertyGenre = 16,    
    kParamIDMediaItemPropertyComposerPersistentIdentifier = 17,
    kParamIDMediaItemPropertyComposer = 18,    
    kParamIDMediaItemPropertyIsPartOfCompilation = 19,    
    kParamIDMediaItemPropertyIsResidentOnDevice = 25,    
    kParamIDMediaItemPropertyChapterCount = 27,    


    //PlayListProperties Group
    kParamIDMediaPlaylistPropertyPersistentIdentifier = 0,
    kParamIDMediaPlaylistPropertyName = 1,
    kParamIDMediaPlaylistPropertyParentPersistentIdentifier = 2,
    kParamIDMediaPlaylistPropertyIsGeniusMix = 3,
    kParamIDMediaPlaylistPropertyIsFolder = 4,
    kParamIDMediaPlaylistPropertyContainedMediaItemsFileTransferIdentifier = 5,
    kParamIDMediaPlaylistPropertyIsiTunesRadioStation = 6,

    // MediaLibraryUpdate MediaItem Group
    kParamIDMediaItemPersistentIdentifier = 0, // Always presents value
    kParamIDMediaItemTitle = 1,
    kParamIDMediaItemMediaType = 2,
    kParamIDMediaItemRating = 3,
    kParamIDMediaItemPlaybackDurationInMilliseconds = 4,
    kParamIDMediaItemAlbumPersistentIdentifier = 5,
    kParamIDMediaItemAlbumTitle  = 6,
    kParamIDMediaItemAlbumTrackNumber = 7,
    kParamIDMediaItemAlbumTrackCount = 8,
    kParamIDMediaItemAlbumDiscNumber = 9,
    kParamIDMediaItemAlbumDiscCount = 10,
    kParamIDMediaItemArtistPersistentIdentifier = 11,
    kParamIDMediaItemArtist = 12, 
    kParamIDMediaItemAlbumAritistPersistentIdentifier = 13, 
    kParamIDMediaItemAlbumAritist = 14,
    kParamIDMediaItemGenrePersistentIdentifier = 15,
    kParamIDMediaItemGenre = 16,    
    kParamIDMediaItemComposerPersistentIdentifier = 17,
    kParamIDMediaItemComposer = 18,    
    kParamIDMediaItemIsPartOfCompilation = 19,    
    kParamIDMediaItemIsLikeSupported = 21,    
    kParamIDMediaItemIsBanSupported = 22,    
    kParamIDMediaItemIsLiked = 23,    
    kParamIDMediaItemIsBanned = 24,    
    kParamIDMediaItemIsResidentOnDevice = 25,    
    kParamIDMediaItemArtworkFileTransferIdentifier = 26,
    kParamIDMediaItemChapterCount = 27,

    //MediaLibraryUpdate MediaPlayList Group
    kParamIDMediaPlaylisPersistentIdentifier = 0,
    kParamIDMediaPlaylistName = 1, // dupulicate
    kParamIDMediaPlaylistParentPersistentIdentifier = 2,
    kParamIDMediaPlaylistIsGeniusMix = 3,
    kParamIDMediaPlaylistIsFolder = 4,
    kParamIDMediaPlaylistContainedMediaItemsFileTransferIdentifier = 5,
    kParamIDMediaPlaylistIsiTunesRadioStation = 6,
    

    //MediaLibraryUpdate
    kParamIDMediaLibraryUniqueIdentifier_update = 0,
    kParamIDMediaLibraryRevision = 1,
    kParamIDMediaItem  = 2,
    kParamIDMediaPlayList = 3,
    kParamIDMediaItemDeletePersistentIdentifier = 4,
    kParamIDMediaPlaylistDeletePersistentIdentifier = 5,
    kParamIDMediaLibraryReset = 6,
    kParamIDMediaLibraryUpdateProgress_Update = 7,
    kParamIDMediaLibraryIsHidingRemoteItems_Update = 8,
    kParamIDPlayAllSongsCapable_Update = 9,

    //StopMediaLibraryUpdate
    kParamIDMediaLibraryUniqueIdentifier_stopupdate = 0,

    //RequestAppLaunch
    kParamIDAppBundleID = 0,
    kParamIDLaunchAlert = 1,


    //StartExternalAccessoryProtocolSession
    kParamIDExternalAccessoryProtocolIdentifier = 0,
    kParamIDExternalAccessoryProtocolSessionIdentifier_start = 1,

    //StopExternalAccessroyProtocolSession
    kParamIDExternalAccessoryProtocolSessionIdentifier_stop = 0,

    //Identification ExternalAccessoryProtoclSession
    kParamIDExternalAccessoryProtocolSessionIdentifier_Identification = 0,
    kParamIDExternalAccessoryProtocolName = 1,
    kParamIDExternalAccessoryProtocolMatchAction = 2,
    kParamIDNativeTransportComponentIdentifier = 3,
    kParamIDExternalAccessoryProtocolCarPlay = 4,

    
    //USBDeviceModeAudioInformation
    kParamIDUSBDeviceModeAudioInformation = 0,

    //RequestVoiceOverMoveCursor
    kParamIDCursorDirection = 0,

    //RequestVoiceOverScrollPage
    kParamIDScrollDirection = 0,

    //RequestVoiceOverSpeakText
    kParamIDTextToSpeak = 0,

    //StartVoiceOverUpdates
    kParamIDSpeakingVolume = 0,
    kParamIDSpeakingRate = 1,
    kParamIDEnabled = 2,

    //RequestVoiceOverConfiguration
    kParamIDSpeakingVolume_config = 0,
    kParamIDSpeakingRate_config = 1,

    // StartVoiceOverCursorUpdates
    kParamIDLabel = 0,
    kParamIDValue = 1,
    kParamIDHint = 2,
    kParamIDTraits = 3,

    // WiFiInformation
    kParamIDRequestStatus = 0,
    kParamIDWiFiSSID = 2,
    kParamIDWiFiPassphras = 3,

    // StatusExternalAccessoryProtocolSession
    kParamIDExternalAccessroyProtocolIdentifier = 0,
    kParamIDExternalAccessoryProtocolSessionIdentifier = 1,
// END Yongil

    //Identification
    kParamIdIdentificationRecjected_Name = 0,
    kParamIdIdentificationRecjected_ModelIdentifier = 1,
    kParamIdIdentificationRecjected_Manufacturer = 2,
    kParamIdIdentificationRecjected_SerialNumber = 3,
    kParamIdIdentificationRecjected_FirmwareVersion = 4,
    kParamIdIdentificationRecjected_HardwareVersion = 5,
    kParamIdIdentificationRecjected_MsgSentByAccessory = 6,
    kParamIdIdentificationRecjected_MsgReceivedFromDevice = 7,
    kParamIdIdentificationRecjected_PowerSourceType = 8,
    kParamIdIdentificationRecjected_MaximumCurrent = 9,
    kParamIdIdentificationRecjected_SupportedEAP = 10,
    kParamIdIdentificationRecjected_AppMatchTeamID = 11,
    kParamIdIdentificationRecjected_CurrentLanguage = 12,
    kParamIdIdentificationRecjected_SupportedLanguage = 13,
    kParamIdIdentificationRecjected_SerialTransportComponent = 14,
    kParamIdIdentificationRecjected_USBDeviceTransportComponent = 15,
    kParamIdIdentificationRecjected_USBHostTransportComponent = 16,
    kParamIdIdentificationRecjected_BluetoothTransportComponent = 17,
    kParamIdIdentificationRecjected_iAP2HIDComponent = 18,
    kParamIdIdentificationRecjected_VehicleInformationComponent = 20,
    kParamIdIdentificationRecjected_VehicleStatusComponent = 21,
    kParamIdIdentificationRecjected_LocationInformationComponent = 22,
    kParamIdIdentificationRecjected_USBHostHIDComponent = 23,
    kParamIdIdentificationRecjected_BluetoothHIDComponent = 29,
    
    kParamIdIdentificationInformationUpdate_Name = 0,
    kParamIdIdentificationInformationUpdate_ModelIdentifier = 1,
    kParamIdIdentificationInformationUpdate_Manufacturer = 2,
    kParamIdIdentificationInformationUpdate_SerialNumber = 3,
    kParamIdIdentificationInformationUpdate_FirmwareVersion = 4,
    kParamIdIdentificationInformationUpdate_HardwareVersion = 5,
    kParamIdIdentificationInformationUpdate_CurrentLanguage = 6,

    //StartCallStateUpdates and CallStatueUpdates
    kParamIdStartCallStateUpdates_RemoteID = 0,
    kParamIdStartCallStateUpdates_DisplayName = 1,
    kParamIdStartCallStateUpdates_Status = 2,
    kParamIdStartCallStateUpdates_Direction = 3,
    kParamIdStartCallStateUpdates_CallUUID = 4,
    kParamIdStartCallStateUpdates_AddressBookID = 6,
    kParamIdStartCallStateUpdates_Label = 7,
    kParamIdStartCallStateUpdates_Service = 8,
    kParamIdStartCallStateUpdates_IsConferenced = 9,
    kParamIdStartCallStateUpdates_ConferenceGroup = 10,
    kParamIdStartCallStateUpdates_DisconnectReason = 11,    

    //StartCallStateUpdates and CommunicationUpdates
    kParamIdStartCommunicationsUpdates_SignalStrength = 0,
    kParamIdStartCommunicationsUpdates_RegistrationStatus = 1,
    kParamIdStartCommunicationsUpdates_AirplaneModeStatus = 2,
    kParamIdStartCommunicationsUpdates_CarrierName = 4,
    kParamIdStartCommunicationsUpdates_CellularSupported = 5,
    kParamIdStartCommunicationsUpdates_TelephonyEnabled = 6,
    kParamIdStartCommunicationsUpdates_FaceTimeAudioEnabled = 7,
    kParamIdStartCommunicationsUpdates_FaceTimeVideoEnabled = 8,
    kParamIdStartCommunicationsUpdates_MuteStatus = 9,
    kParamIdStartCommunicationsUpdates_CurrentCallCount = 10,
    kParamIdStartCommunicationsUpdates_NewVoicemailCount = 11,
    kParamIdStartCommunicationsUpdates_InitiateCallAvailable = 12,
    kParamIdStartCommunicationsUpdates_EndAndAcceptAvailable = 13,    
    kParamIdStartCommunicationsUpdates_HoldAndAcceptAvailable = 14,    
    kParamIdStartCommunicationsUpdates_SwapAvailable = 15,    
    kParamIdStartCommunicationsUpdates_MergeAvailable = 16,
    kParamIdStartCommunicationsUpdates_HoldAvailable = 17,    

    //InitalCall
    kParamIdInitalCall_Type = 0,
    kParamIdInitalCall_DestinationID = 1,
    kParamIdInitalCall_Servicee = 2,
    kParamIdInitalCall_AddressBookID = 3,    

    //AcceptCall
    kParamIdAcceptCall_AcceptAction = 0,
    kParamIdAcceptCall_CallUUID = 1,

    //EndCall
    kParamIdEndCall_EndAction = 0,
    kParamIdEndCall_CallUUID = 1,

    //HoldStateUpdate
    kParamIdHoldStateUpdate_HoldStatus = 0,
    kParamIdHoldStateUpdate_CallUUID = 1,

    //MuteStateUpdate
    kParamIdMuteStateUpdate_MuteStatus = 0,
    
    //SendDTMF
    kParamIdSendDTMF_Tone = 0,
    kParamIdSendDTMF_CallUUID = 1,
    
    //StartListUpdates
    kParamIdStartListUpdates_RecentsListProperties = 1,
    kParamIdStartListUpdates_RecentsListmax = 3,
    kParamIdStartListUpdates_RecentsListCombine= 4,
    kParamIdStartListUpdates_FavoritesListProperties = 6,
    kParamIdStartListUpdates_FavoritesListMax = 8,

    //ListUpdates
    kParamIdListUpdates_RecentsListAvailable = 0,
    kParamIdListUpdates_RecentsList = 1,
    kParamIdListUpdates_RecentsListCount = 2,
    kParamIdListUpdates_FavoritesListAvailabble = 5,
    kParamIdListUpdates_FavoritesList = 6,
    kParamIdListUpdates_FavoritesListCount = 7,

    kParamIdListUpdateGroup_Index = 0,
    kParamIdListUpdateGroup_RemoteID = 1,
    kParamIdListUpdateGroup_DisplayName = 2,
    kParamIdListUpdateGroup_Label = 3,
    kParamIdListUpdateGroup_AddressBookID = 4,
    kParamIdListUpdateGroup_Service = 5,
    kParamIdListUpdateGroup_Type = 6,
    kParamIdListUpdateGroup_UnixTimestamp = 7,
    kParamIdListUpdateGroup_Duration = 8,
    kParamIdListUpdateGroup_Occurrences = 9,

    kParamIdListUpdateRecentListGroup_Index = 0,
    kParamIdListUpdateRecentListGroup_RemoteID = 1,
    kParamIdListUpdateRecentListGroup_DisplayName = 2,
    kParamIdListUpdateRecentListGroup_Label = 3,
    kParamIdListUpdateRecentListGroup_AddressBookID = 4,
    kParamIdListUpdateRecentListGroup_Service = 5,
    kParamIdListUpdateRecentListGroup_Type = 6,
    kParamIdListUpdateRecentListGroup_UnixTimestamp = 7,
    kParamIdListUpdateRecentListGroup_Duration = 8,
    kParamIdListUpdateRecentListGroup_Occurrences = 9,
    
    kParamIdListUpdateFavoritesListGroup_Index = 0,
    kParamIdListUpdateFavoritesListGroup_RemoteID = 1,
    kParamIdListUpdateFavoritesListGroup_DisplayName = 2,
    kParamIdListUpdateFavoritesListGroup_Label = 3,
    kParamIdListUpdateFavoritesListGroup_AddressBookID = 4,
    kParamIdListUpdateFavoritesListGroup_Service = 5    
};

enum {
    kMsgIdNotUse = 0x0000,
            
    //Authentication Msgs
    kMsgIdRequestAuthenticationCertificate = 0xAA00,
    kMsgIdAuthenticationCertificate = 0xAA01,
    kMsgIdRequestAuthenticationChallengeResponse = 0xAA02,
    kMsgIdAuthenticationResponse = 0xAA03,
    kMsgIdAuthenticationFailed = 0xAA04,
    kMsgIdAuthenticationSucceed = 0xAA05,

    //Identification Msgs
    kMsgIdStartIdentification = 0x1D00,
    kMsgIdIdentificationInformation = 0x1D01,
    kMsgIdIdentificationAccepted = 0x1D02,
    kMsgIdIdentificationRejected = 0x1D03,
    kMsgIdCancelIdentification = 0x1D05,
    kMsgIdIdentificationInformationUpdate = 0x1D06,

    // App Launch
    kMsgIdRequestAppLaunch = 0xEA02,

    // Assistive Touch
    kMsgIdStartAssistiveTouch = 0x5400,
    kMsgIdStopAssistiveTouch = 0x5401,
    kMsgIdStartAssistiveTouchInformation = 0x5402,
    kMsgIdAssistiveTouchInformation = 0x5403,
    kMsgIdStopAssistiveTouchInformation = 0x5404,

    // Bluetooth Connection
    kMsgIdBluetoothComponentInformation = 0x4E01,
    kMsgIdStartBluetoothConnectionUpdates = 0x4E03,
    kMsgIdBluetoothConnectionUpdate = 0x4E04,
    kMsgIdStopBluetoothConnectionUpdates = 0x4E05,
    
    // Device Authentication
    kMsgIdRequestDeviceAuthenticationCertificate = 0xAA10,
    kMsgIdDeviceAuthenticationCertificate = 0xAA11,
    kMsgIdRequestDeviceAuthenticationChallengeResponse = 0xAA12,
    kMsgIdDeviceAuthenticationResponse = 0xAA13,
    kMsgIdDeviceAuthenticationFailed = 0xAA14,
    kMsgIdDeviceAuthenticationSucceeded = 0xAA15,    

    // Device Notification
    kMsgIdDeviceInformationUpdate = 0x4E09,
    kMsgIdDeviceLanguageUpdate = 0x4E0A,
    kMsgIdDeviceTimeUpdate = 0x4E0B,
    kMsgIdDeviceUUIDUpdate = 0x4E0C,
    
    // External Accessory Protocol
    kMsgIdStartExternalAccessoryProtocolSession = 0xEA00,
    kMsgIdStopExternalAccessoryProtocolSession = 0xEA01,
    kMsgIdStatusExternalAccessoryProtocolSession = 0xEA03,
    
    // HID
    kMsgIdStartHID = 0x6800,
    kMsgIdDeviceHIDReport = 0x6801,
    kMsgIdAccessoryHIDReport = 0x6802,
    kMsgIdStopHID = 0x6803,

    // Location
    kMsgIdStartLocationInformation = 0xFFFA,
    kMsgIdLocationInformation = 0xFFFB,
    kMsgIdStopLocationInformation = 0xFFFC,

    // Media Library Access
    kMsgIdStartMediaLibraryInformation = 0x4C00,
    kMsgIdMediaLibraryInformation = 0x4C01,
    kMsgIdStopMediaLibraryInformation = 0x4C02,
    kMsgIdStartMediaLibraryUpdates = 0x4C03,
    kMsgIdMediaLibraryUpdate = 0x4C04,
    kMsgIdStopMediaLibraryUpdates = 0x4C05,
    kMsgIdPlayMediaLibraryCurrentSelection = 0x4C06,
    kMsgIdPlayMediaLibraryItems = 0x4C07,
    kMsgIdPlayMediaLibraryCollection = 0x4C08,
    kMsgIdPlayMediaLibrarySpecial = 0x4C09,
    
    //Now Playing updates
    kMsgIdStartNowPlayingUpdates = 0x5000,
    kMsgIdNowPlayingUpdates = 0x5001,
    kMsgIdStopNowPlayingUpdates = 0x5002,
    kMsgIdSetNowPlayingInformation = 0x5003,
    
    // Power
    kMsgIdStartPowerUpdates = 0xAE00,
    kMsgIdPowerUpdate = 0xAE01,
    kMsgIdStopPowerUpdates = 0xAE02,
    kMsgIdPowerSourceUpdate = 0xAE03,

    //USB Device Mode Audio
    kMsgIdStartUSBDeviceModeAudio = 0xDA00,
    kMsgIdUSBDeviceModeAudioInformation = 0xDA01,
    kMsgIdStopUSBDeviceModeAudio = 0xDA02,
    
    // Telephony
    kMsgIdStartTelephonyCallStateInformation = 0x4154,
    kMsgIdTelephonyCallStateInformation = 0x4155,
    kMsgIdStopTelephonyCallStateInformation = 0x4156,
    kMsgIdStartTelephonyUpdates = 0x4157,
    kMsgIdTelephonyUpdate = 0x4158,
    kMsgIdStopTelephonyUpdates = 0x4159,
    
    // Vehicle Status
    kMsgIdStartVehicleStatusUpdates = 0xA100,
    kMsgIdVehicleStatusUpdate = 0xA101,
    kMsgIdStopVehicleStatusUpdates = 0xA102,

    // Voice Over
    kMsgIdStartVoiceOver = 0x5612,
    kMsgIdStopVoiceOver = 0x5613,
    kMsgIdRequestVoiceOverMoveCursor = 0x5601,
    kMsgIdRequestVoiceOverActivateCursor = 0x5602,
    kMsgIdRequestVoiceOverScrollPage = 0x5603,
    kMsgIdRequestVoiceOverSpeakText = 0x5606,
    kMsgIdRequestVoiceOverPauseText = 0x5608,
    kMsgIdRequestVoiceOverResumeText = 0x5609,
    kMsgIdStartVoiceOverUpdates = 0x560B,
    kMsgIdVoiceOverUpdate = 0x560C,
    kMsgIdStopVoiceOverUpdates = 0x560D,
    kMsgIdRequestVoiceOverConfiguration = 0x560E,
    kMsgIdStartVoiceOverCursorUpdates = 0x560F,
    kMsgIdVoiceOverCursorUpdate = 0x5610,
    kMsgIdStopVoiceOverCursorUpdates = 0x5611,
    
    // Wi-Fi Information Sharing
    kMsgIdRequestWiFiInformation = 0x5700,
    kMsgIdWiFiInformation = 0x5701,

    // Communications
    kMsgIdStartCallStateUpdates  = 0x4154,
    kMsgIdCallStateUpdates = 0x4155,
    kMsgIdStopCallStateUpdates = 0x4156,
    kMsgIdStartCommunicationsUpdates = 0x4157,
    kMsgIdCommunicationsUpdates = 0x4158,
    kMsgIdStopCommunicationsUpdates = 0x4159,
    kMsgIdInitialCall = 0x415A,
    kMsgIdAcceptCall = 0x415B,
    kMsgIdEndCall = 0x415C,
    kMsgIdSwapCalls = 0x415D,
    kMsgIdMergeCalls = 0x415E,
    kMsgIdHoldStatusUpdate = 0x415F,
    kMsgIdMuteStatusUpdate = 0x4160,
    kMsgIdSendDTMF = 0x4161,
    kMsgIdStartListUpdate = 0x4170,
    kMsgIdListUpdate = 0x4171,
    kMsgIdStopListUpdate = 0x4172,    
};

enum {
    kControlSessionMsgStartMsb = 0x40,
    kControlSessionMsgStartLsb = 0x40
};

enum {
    kIndexControlMsgStartMsb = 0,
    kIndexControlMsgStartLsb,
    kIndexControlMsgLenMsb,
    kIndexControlMsgLenLsb,
    kIndexControlMsgMsgIdMsb,
    kIndexControlMsgMsgIdLsb,
    kIndexControlMsgMinimumLength
};

enum {
    kIndexControlMsgParamLenMsb,
    kIndexControlMsgParamLenLsb,
    kIndexControlMsgParamIdMsb,
    kIndexControlMsgParamIdLsb,
    kIndexControlMsgParamData
};

enum {
    kIndexEAMsgSessionIdentifierMsb = 0,
    kIndexEAMsgSessionIdentifierLsb
};

enum {
    kIndexEAMsgMinimumLength = 2
};

//coprocessor data
#ifdef MM2014
#define I2C_DEV_FILE_NAME                        "/dev/i2c-2"
#elif defined(KOECN)
#define I2C_DEV_FILE_NAME                        "/dev/i2c-0"
#elif (defined(MFI_AUTH_DEVICE_PATH))
#define I2C_DEV_FILE_NAME                        MFI_AUTH_DEVICE_PATH
#else
#define I2C_DEV_FILE_NAME                        "/dev/i2c-0"
#endif

#if (defined(MFI_AUTH_DEVICE_ADDRESS))
#define I2C_CP_SLAVE_ADDRESS                    MFI_AUTH_DEVICE_ADDRESS
#else
#define I2C_CP_SLAVE_ADDRESS                    0x11
#endif

#define DEV_VERSION_REG_ADDR                    0X0000
#define ACCES_CERTIFICATE_START_PAGE_SIZE        128
#define ACCES_CERTIFICATE_DATA_LEN_REG_ADDR        0X0030
#define ACCES_CERTIFICATE_DATA_LEN_REG_SIZE        2
#define ACCES_CERTIFICATE_NUM_PAGES                15
#define ACCES_CERTIFICATE_START_PAGE_REG_ADDR    0X0031
#define USLEEP_TIME                                (10000)
#define CHALLENGE_DATA_LEN_REG_ADDR                0X0020
#define CHALLENGE_DATA_LEN_SIZE                    2
#define CHALLENGE_DATA_REG_ADDR                    0X0021
#define CHALLENGE_DATA_SIZE_MAX                    20
#define AUTH_CONTROL_STATUS_REG_ADDR            0X0010
#define AUTH_CONTROL_STATUS_SIZE                1
#define ERROR_CODE_REG_ADDR                        0X0005
#define ERROR_CODE_REG_SIZE                        1
#define SIG_DATA_LEN_REG_ADDR                    0X0011
#define SIG_DATA_LEN_REG_SIZE                    2
#define SIG_DATA_REG_ADDR                        0X0012
#define SIG_DATA_REG_SIZE                        128


/* ----- commands for the ioctl like i2c_command call:
** note that additional calls are defined in the algorithm and hw
**       dependent layers - these can be listed here, or see the
* *        corresponding header files.
* */                                    /* -> bit-adapter specific ioctls */
#define I2C_RETRIES             0x0701    /* number of times a device address */
                                        /* should be polled when not */
                                        /* acknowledging */
#define I2C_TIMEOUT             0x0702    /* set timeout - call with int */


/* this is for i2c-dev.c */
#define I2C_SLAVE                0x0703    /* Change slave address */
                                        /* Attn.: Slave address is 7 or 10 bits */
#define I2C_SLAVE_FORCE         0x0706    /* Change slave address */
                                        /* Attn.: Slave address is 7 or 10 bits */
                                        /* This changes the address, even if it */
                                        /* is already taken! */
#define I2C_TENBIT                0x0704    /* 0 for 7 bit addrs, != 0 for 10 bit */

#define I2C_FUNCS                0x0705    /* Get the adapter functionality */
#define I2C_RDWR                0x0707    /* Combined R/W transfer (one stop only) */
#define I2C_PEC                 0x0708    /* != 0 for SMBus PEC */

#define I2C_SMBUS                0x0720    /* SMBus-level access */


#define I2C_M_TEN                0x10    /* we have a ten bit chip address */
#define I2C_M_RD                0x01
#define I2C_M_NOSTART            0x4000
#define I2C_M_REV_DIR_ADDR        0x2000
#define I2C_M_IGNORE_NAK        0x1000
#define I2C_M_NO_RD_ACK         0x0800


/*
I2C Msg - used for pure i2c transaction, also from /dev interface
*/
struct i2c_msg {
    __u16 addr;                 /* slave address */
    unsigned short flags;
    short len;                    /* msg length */
    char *buf;                    /* pointer to msg data */
};

    struct SI2cRdWrIoctlData {
    struct i2c_msg *msgs;        /* pointers to i2c_msgs */
    int nmsgs;                    /* number of i2c_msgs */
};

#endif // IAP2PREDEFINEDVALUES_H
