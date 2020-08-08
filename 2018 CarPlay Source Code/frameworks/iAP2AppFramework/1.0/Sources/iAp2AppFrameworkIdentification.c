/*
    File:       iAp2AppFrameworkIdentification.c
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

#include <stdlib.h>

#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkIdentification.h"
#include "iAp2AppFrameworkPrivate.h"
#include "iAp2Connection.h"
#include "iAp2Marshal.h"
#include "iAp2Log.h"


//===========================================================================================================================
//    Definitions
//===========================================================================================================================

//===========================================================================================================================
//    Declarations
//===========================================================================================================================

//===========================================================================================================================
//    iAp2AppIdentificationUpdate
//===========================================================================================================================
int iAp2AppIdentificationUpdate(iAp2AppRef inRef, IdentificationUpdateArg* inArg)
{
    int retValue = kIap2NoErr;
    LOGD("iAp2AppIdentificationUpdate() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (inArg == NULL) {
        LOGE("ERROR : inArg is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    int mallocSize = sizeof(CommandHeader) + inArg->name.size + inArg->modelIdentifier.size + inArg->manufacturer.size + inArg->serialNumber.size + inArg->firmwareVersion.size + inArg->hardwareVersion.size + inArg->currentLanguage.size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->name.bPresent);
    if ( inArg->name.bPresent == TRUE ) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->name.value, inArg->name.size);
    }
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->modelIdentifier.bPresent);
    if ( inArg->modelIdentifier.bPresent == TRUE ) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->modelIdentifier.value, inArg->modelIdentifier.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->manufacturer.bPresent);
    if ( inArg->manufacturer.bPresent == TRUE ) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->manufacturer.value, inArg->manufacturer.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->serialNumber.bPresent);
    if ( inArg->serialNumber.bPresent == TRUE ) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->serialNumber.value, inArg->serialNumber.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->firmwareVersion.bPresent);
    if ( inArg->firmwareVersion.bPresent == TRUE ) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->firmwareVersion.value, inArg->firmwareVersion.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->hardwareVersion.bPresent);
    if ( inArg->hardwareVersion.bPresent == TRUE ) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->hardwareVersion.value, inArg->hardwareVersion.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->currentLanguage.bPresent);
    if ( inArg->currentLanguage.bPresent == TRUE ) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->currentLanguage.value, inArg->currentLanguage.size);
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdIdentificationUpdate;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSettingCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}



//===========================================================================================================================
//    iAp2AppIdentificationSetting
//===========================================================================================================================
int iAp2AppIdentificationSetting(iAp2AppRef inRef, IdentficationMsgSelectArg* inArg)
{
    int retValue = kIap2NoErr;
    LOGD("iAp2AppIdentificationSetting() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (inArg == NULL) {
        LOGE("ERROR : inArg is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    int mallocSize = sizeof(CommandHeader) + sizeof(IdentficationMsgSelectArg) + inArg->supportedLanaguage.size + inArg->LocationInformationName.size + inArg->ExternalAccessoryProtocolName.size 
                                + inArg->AppMatchTeamID.size + inArg->VehicleStatusComponentName.size + inArg->VehicleInformationComponentName.size + inArg->VehicleInformationComponentDisplayName.size
                                + inArg->USBDeviceTransportComponentName.size + inArg->USBHostTransportComponentName.size + inArg->SerialTransportComponentName.size + inArg->BluetoothTransportComponentName.size
                                + inArg->BluetoothTransportMediaAccessControlAddress.size + inArg->iAP2HIDComponentName.size + inArg->WirelessCarPlayTransPortComponentName.size + inArg->BluetoothHIDComponentName.size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->supportedLanaguage.bPresent);
    if(inArg->supportedLanaguage.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->supportedLanaguage.value, inArg->supportedLanaguage.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Power);
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Location);
     iAp2MarshalPutUint32(&iAp2Marshal, inArg->LocationInformationName.bPresent);
    if(inArg->LocationInformationName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->LocationInformationName.value, inArg->LocationInformationName.size);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->LocationInformationGPGGA);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->LocationInformationGPRMC);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->LocationInformationGPGSV);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->LocationInformationPASCD);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->LocationInformationPAGCD);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->LocationInformationPAACD);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->LocationInformationGPHDT);
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->NowPlaying);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Communications_CallControls);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Communications_SendDTMFUse);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Communications_CallStateUpdate);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Communications_CommunicationsUpdate);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Communications_ListUpdates);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDiAP2SessionMediaPlaybackRemote);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDiAP2SessionAssistiveSwitchControl);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDiAP2SessionAssistiveTouchPointer);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDiAP2SessionGameControllerExtened);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDiAP2SessionAssistiveTouchStandard);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDiAP2SessionAssistiveHeadPhone);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDiAP2SessionKeyBoard);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaLibraryInformation);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaLibraryUpdates);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaLibraryPlayback);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->DeviceInformationUpdate);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->DeviceLanguageUpdate);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->DeviceTimeUpdate);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->DeviceUUIDUpdate);    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->AppLaunch);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->AssistiveTouch);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->BluetoothConnection);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->EAiAP2Session);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->EAiAP2Session_MultipleSession);
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->ExternalAccessoryProtocolName.bPresent);
    if(inArg->ExternalAccessoryProtocolName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->ExternalAccessoryProtocolName.value, inArg->ExternalAccessoryProtocolName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->ExternalAccessoryProtocolMatchAction);

     iAp2MarshalPutUint32(&iAp2Marshal, inArg->AppMatchTeamID.bPresent);
    if(inArg->AppMatchTeamID.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->AppMatchTeamID.value, inArg->AppMatchTeamID.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->UseEAWithCarPlay);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->EANativeTransPort);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDNativeTransportMediaPlaybackRemote);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDNativeTransportAssistiveSwitchControl);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDNativeTransportAssistiveTouchPointer);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDNativeTransportGameControllerExtened);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDNativeTransportAssistiveTouchStandard);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDNativeTransportAssistiveHeadPhone);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDNativeTransportKeyBoard);
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->VehicleStatus);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->VehicleStatusComponentName.bPresent);
    if(inArg->VehicleStatusComponentName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->VehicleStatusComponentName.value, inArg->VehicleStatusComponentName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->VehicleStatusRange);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->VehicleStatusOutSideTemperature);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->VehicleStatusRangeWarning);
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->VehicleInformation);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->VehicleInformationComponentName.bPresent);
    if(inArg->VehicleInformationComponentName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->VehicleInformationComponentName.value, inArg->VehicleInformationComponentName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->VehicleInformationEngineTypes.bPresent);
    if(inArg->VehicleInformationEngineTypes.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->VehicleInformationEngineTypes.value);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->VehicleInformationComponentDisplayName.bPresent);
    if(inArg->VehicleInformationComponentDisplayName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->VehicleInformationComponentDisplayName.value, inArg->VehicleInformationComponentDisplayName.size);
        
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->VoiceOver);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->WiFiInformationSharing);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDigitalAudioDeviceMode);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDigitalAudioHostMode);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->DeviceAuthentication);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->DevicePowered);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDeviceTransportComponentName.bPresent);
    if(inArg->USBDeviceTransportComponentName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->USBDeviceTransportComponentName.value, inArg->USBDeviceTransportComponentName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDeviceSurpportedAudioSampleRate8000Hz);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDeviceSurpportedAudioSampleRate11025Hz);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDeviceSurpportedAudioSampleRate12000Hz);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDeviceSurpportedAudioSampleRate16000Hz);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDeviceSurpportedAudioSampleRate22050Hz);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDeviceSurpportedAudioSampleRate24000Hz);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDeviceSurpportedAudioSampleRate32000Hz);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDeviceSurpportedAudioSampleRate44100Hz);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBDeviceSurpportedAudioSampleRate48000Hz);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->USBHostTransportComponentName.bPresent);
    if(inArg->USBHostTransportComponentName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->USBHostTransportComponentName.value, inArg->USBHostTransportComponentName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->SerialTransportComponentName.bPresent);
    if(inArg->SerialTransportComponentName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->SerialTransportComponentName.value, inArg->SerialTransportComponentName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->BluetoothTransportComponentName.bPresent);
    if(inArg->BluetoothTransportComponentName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->BluetoothTransportComponentName.value, inArg->BluetoothTransportComponentName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->BluetoothTransportMediaAccessControlAddress.bPresent);
    if(inArg->BluetoothTransportMediaAccessControlAddress.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->BluetoothTransportMediaAccessControlAddress.value, inArg->BluetoothTransportMediaAccessControlAddress.size);
  
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->iAP2HIDComponentName.bPresent);
    if(inArg->iAP2HIDComponentName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->iAP2HIDComponentName.value, inArg->iAP2HIDComponentName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->iAP2HIDComponentFunction);

     iAp2MarshalPutUint32(&iAp2Marshal, inArg->iAP2HIDComponentName.bPresent);
    if(inArg->WirelessCarPlayTransPortComponentName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->WirelessCarPlayTransPortComponentName.value, inArg->WirelessCarPlayTransPortComponentName.size);

     iAp2MarshalPutUint32(&iAp2Marshal, inArg->BluetoothHIDComponentName.bPresent);
    if(inArg->BluetoothHIDComponentName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->BluetoothHIDComponentName.value, inArg->BluetoothHIDComponentName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->BluetoothHIDComponentFunction);
        
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    pCommandHeader->cmd = kCmdIdentificationSelect;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSettingCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    
    return retValue;
}
//===========================================================================================================================


