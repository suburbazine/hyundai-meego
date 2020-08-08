/*
    File:       SessionConnection.cpp
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

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <fcntl.h>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include <iAp2Marshal.h>
//#include "iAp2ServiceCommon.h"
#include "SessionConnection.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"

namespace IAP2
{

SessionConnection::SessionConnection()
{
    LOGD("SessionConnection()");
}

SessionConnection::~SessionConnection()
{
    LOGD("~SessionConnection()");
}

int SessionConnection::handleConnectionCmd(int currentFd, uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    int result = kIap2NoErr;
    LOGI("handleConnectionCmd() currentFd[%d] cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", currentFd, cmd, handle, pRxData, pLnkCtrler);

    if (pRxData == NULL) {
        LOGE("    ERROR : pRxData is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR : pLnkCtrler is NULL");
        return kIap2ParamErr;
    }

    switch(cmd) {
        
        case kCmdGetInitConnectionData : {            
            result = pLnkCtrler->mConnection.CmdGetInitConnectionData(pLnkCtrler, currentFd, pRxData);            
            break;
        }
        case kCmdCarplayOnOff: {
            BOOL CarplayOnOff;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};    
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            CarplayOnOff = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGI(" kCmdCarplayOnOff CarplayOnOff %d", CarplayOnOff);    
            pLnkCtrler->mConnection.CarplayOnoff = CarplayOnOff;
            break;
        }
        case kCmdIdentificationSelect: {
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.supportedLanaguage.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.supportedLanaguage.bPresent == TRUE) {
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.supportedLanaguage), &iAp2Marshal);                
            }
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.Power= (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.Location = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationName.bPresent == TRUE) {
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationName), &iAp2Marshal);                    
            }

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationGPGGA = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationGPRMC = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationGPGSV = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationPASCD = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationPAGCD = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationPAACD = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationGPHDT = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.NowPlaying = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.Communications_CallControls = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.Communications_SendDTMFUse = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.Communications_CallStateUpdate = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.Communications_CommunicationsUpdate = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.Communications_ListUpdates = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionMediaPlaybackRemote = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionAssistiveSwitchControl = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionAssistiveTouchPointer = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionGameControllerExtened = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionAssistiveTouchStandard = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionAssistiveHeadPhone = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionKeyBoard = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.MediaLibraryInformation = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.MediaLibraryUpdates = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.MediaLibraryPlayback = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.DeviceInformationUpdate = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.DeviceLanguageUpdate = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.DeviceTimeUpdate = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.DeviceUUIDUpdate = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);  
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.AppLaunch = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.AssistiveTouch = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothConnection = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.EAiAP2Session = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.EAiAP2Session_MultipleSession = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.ExternalAccessoryProtocolName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.ExternalAccessoryProtocolName.bPresent == TRUE) {
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.ExternalAccessoryProtocolName), &iAp2Marshal);                 
            }
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.ExternalAccessoryProtocolMatchAction = (EAPMatchActionEnum)iAp2MarshalGetUint32(&iAp2Marshal);

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.AppMatchTeamID.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.AppMatchTeamID.bPresent == TRUE) {                
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.AppMatchTeamID), &iAp2Marshal);                   
            }
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.UseEAWithCarPlay = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.EANativeTransPort = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportMediaPlaybackRemote = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportAssistiveSwitchControl = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportAssistiveTouchPointer = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportGameControllerExtened = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportAssistiveTouchStandard = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportAssistiveHeadPhone = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportKeyBoard = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatus = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatusComponentName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatusComponentName.bPresent == TRUE) {                
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatusComponentName), &iAp2Marshal);                     
            }

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatusRange = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatusOutSideTemperature = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatusRangeWarning = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformation = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationComponentName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationComponentName.bPresent == TRUE) {                
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationComponentName), &iAp2Marshal);                    
            }

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationEngineTypes.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationEngineTypes.bPresent == TRUE) {
                pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationEngineTypes.value = (EngineTypesEnum)iAp2MarshalGetUint32(&iAp2Marshal);
            }
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationComponentDisplayName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationComponentDisplayName.bPresent == TRUE) {                
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationComponentDisplayName), &iAp2Marshal);                    
            }
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.VoiceOver = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.WiFiInformationSharing = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal); //37 
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDigitalAudioDeviceMode = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDigitalAudioHostMode = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.DeviceAuthentication = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.DevicePowered = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceTransportComponentName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceTransportComponentName.bPresent == TRUE) {                
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceTransportComponentName), &iAp2Marshal);                    
            }

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate8000Hz = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate11025Hz = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate12000Hz = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate16000Hz = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate22050Hz = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate24000Hz = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate32000Hz = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate44100Hz = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate48000Hz = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBHostTransportComponentName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBHostTransportComponentName.bPresent == TRUE) {                
                 reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBHostTransportComponentName), &iAp2Marshal);                    
            }

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.SerialTransportComponentName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.SerialTransportComponentName.bPresent == TRUE) {                
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.SerialTransportComponentName), &iAp2Marshal);                     
            }

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothTransportComponentName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothTransportComponentName.bPresent == TRUE) {                
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothTransportComponentName), &iAp2Marshal);                     
            }

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.bPresent == TRUE) {                
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress), &iAp2Marshal);                    
            }

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.iAP2HIDComponentName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.iAP2HIDComponentName.bPresent == TRUE) {                
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.iAP2HIDComponentName), &iAp2Marshal);                    
            }
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.iAP2HIDComponentFunction =  (HIDComponentFunctionEnum)iAp2MarshalGetUint32(&iAp2Marshal);

            pLnkCtrler->mIdentification.mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.bPresent == TRUE) {                
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName), &iAp2Marshal);                    
            }
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothHIDComponentName.bPresent =  (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothHIDComponentName.bPresent == TRUE) {                
                reSettingUtf8Param(&(pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothHIDComponentName), &iAp2Marshal);                    
            }
            
            pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothHIDComponentFunction =  (HIDComponentFunctionEnum)iAp2MarshalGetUint32(&iAp2Marshal);
            
            LOGD("supportedLanaguage : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.supportedLanaguage.value);
            LOGD("Power : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.Power);
            LOGD("Location : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.Location);
            LOGD("LocationInformationName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationName.value);
            LOGD("LocationInformationGPGGA : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationGPGGA);
            LOGD("LocationInformationGPRMC : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationGPRMC);
            LOGD("LocationInformationGPGSV : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationGPGSV);
            LOGD("LocationInformationPASCD : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationPASCD);
            LOGD("LocationInformationPAGCD : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationPAGCD);
            LOGD("LocationInformationPAACD : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationPAACD);
            LOGD("LocationInformationGPHDT : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.LocationInformationGPHDT);            
            LOGD("NowPlaying : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.NowPlaying);
            LOGD("Communications_CallControls : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.Communications_CallControls);
            LOGD("Communications_SendDTMFUse : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.Communications_SendDTMFUse);
            LOGD("Communications_CallStateUpdate : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.Communications_CallStateUpdate);
            LOGD("Communications_CommunicationsUpdate : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.Communications_CommunicationsUpdate);
            LOGD("Communications_ListUpdates : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.Communications_ListUpdates);
            LOGD("HIDiAP2SessionMediaPlaybackRemote : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionMediaPlaybackRemote);
            LOGD("HIDiAP2SessionAssistiveSwitchControl : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionAssistiveSwitchControl);
            LOGD("HIDiAP2SessionAssistiveTouchPointer : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionAssistiveTouchPointer);
            LOGD("HIDiAP2SessionGameControllerExtened : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionGameControllerExtened);
            LOGD("HIDiAP2SessionAssistiveTouchStandard : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionAssistiveTouchStandard);
            LOGD("HIDiAP2SessionAssistiveHeadPhone : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionAssistiveHeadPhone);
            LOGD("HIDiAP2SessionKeyBoard : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDiAP2SessionKeyBoard);
            LOGD("MediaLibraryInformation : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.MediaLibraryInformation);
            LOGD("MediaLibraryUpdates : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.MediaLibraryUpdates);
            LOGD("MediaLibraryPlayback : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.MediaLibraryPlayback);
            LOGD("DeviceInformationUpdate : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.DeviceInformationUpdate);
            LOGD("DeviceLanguageUpdate : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.DeviceLanguageUpdate);
            LOGD("DeviceTimeUpdate : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.DeviceTimeUpdate);
            LOGD("DeviceUUIDUpdate : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.DeviceUUIDUpdate);            
            LOGD("AppLaunch : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.AppLaunch);
            LOGD("AssistiveTouch : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.AssistiveTouch);
            LOGD("BluetoothConnection : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothConnection);
            LOGD("EAiAP2Session : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.EAiAP2Session);
            LOGD("EAiAP2Session_MultipleSession : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.EAiAP2Session_MultipleSession);
            LOGD("ExternalAccessoryProtocolName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.ExternalAccessoryProtocolName.value);
            LOGD("ExternalAccessoryProtocolMatchAction : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.ExternalAccessoryProtocolMatchAction);
            LOGD("AppMatchTeamID : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.AppMatchTeamID.value);
            LOGD("UseEAWithCarPlay : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.UseEAWithCarPlay);
            LOGD("EANativeTransPort : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.EANativeTransPort);
            LOGD("HIDNativeTransportMediaPlaybackRemote : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportMediaPlaybackRemote);
            LOGD("HIDNativeTransportAssistiveSwitchControl : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportAssistiveSwitchControl);
            LOGD("HIDNativeTransportAssistiveTouchPointer : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportAssistiveTouchPointer);
            LOGD("HIDNativeTransportGameControllerExtened : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportGameControllerExtened);
            LOGD("HIDNativeTransportAssistiveTouchStandard : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportAssistiveTouchStandard);
            LOGD("HIDNativeTransportAssistiveHeadPhone : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportAssistiveHeadPhone);
            LOGD("HIDNativeTransportKeyBoard : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.HIDNativeTransportKeyBoard);
            LOGD("VehicleStatus : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatus);
            LOGD("VehicleStatusComponentName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatusComponentName.value);
            LOGD("VehicleStatusRange : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatusRange);
            LOGD("VehicleStatusOutSideTemperature : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatusOutSideTemperature);
            LOGD("VehicleStatusRangeWarning : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleStatusRangeWarning);            
            LOGD("VehicleInformation : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformation);
            LOGD("VehicleInformationComponentName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationComponentName.value);
            LOGD("VehicleInformationEngineTypes : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationEngineTypes.value);
            LOGD("VehicleInformationComponentDisplayName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.VehicleInformationComponentDisplayName.value);
            LOGD("VoiceOver : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.VoiceOver);
            LOGD("WiFiInformationSharing : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.WiFiInformationSharing);
            LOGD("USBDigitalAudioDeviceMode : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDigitalAudioDeviceMode);
            LOGD("USBDigitalAudioHostMode : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDigitalAudioHostMode);
            LOGD("DeviceAuthentication : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.DeviceAuthentication);
            LOGD("DevicePowered : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.DevicePowered);
            LOGD("USBDeviceTransportComponentName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceTransportComponentName.value);
            LOGD("USBDeviceSurpportedAudioSampleRate8000Hz : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate8000Hz);
            LOGD("USBDeviceSurpportedAudioSampleRate11025Hz : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate11025Hz);
            LOGD("USBDeviceSurpportedAudioSampleRate12000Hz : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate12000Hz);
            LOGD("USBDeviceSurpportedAudioSampleRate16000Hz : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate16000Hz);
            LOGD("USBDeviceSurpportedAudioSampleRate22050Hz : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate22050Hz);
            LOGD("USBDeviceSurpportedAudioSampleRate24000Hz : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate24000Hz);
            LOGD("USBDeviceSurpportedAudioSampleRate32000Hz : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate32000Hz);
            LOGD("USBDeviceSurpportedAudioSampleRate44100Hz : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate44100Hz);
            LOGD("USBDeviceSurpportedAudioSampleRate48000Hz : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate48000Hz);            
            LOGD("USBHostTransportComponentName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.USBHostTransportComponentName.value);
            LOGD("SerialTransportComponentName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.SerialTransportComponentName.value);
            LOGD("BluetoothTransportComponentName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothTransportComponentName.value);
            LOGD("BluetoothTransportMediaAccessControlAddress : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.value);
            LOGD("iAP2HIDComponentName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.iAP2HIDComponentName.value);
            LOGD("iAP2HIDComponentFunction : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.iAP2HIDComponentFunction);
            LOGD("WirelessCarPlayTransPortComponentName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.value);
            LOGD("BluetoothHIDComponentName : %s",pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothHIDComponentName.value);
            LOGD("BluetoothHIDComponentFunction : %u",pLnkCtrler->mIdentification.mIdentficationMsgSelect.BluetoothHIDComponentFunction);
            break;
        }
        case kCmdUsbModeChange: {
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
                    
            pLnkCtrler->mConnection.mRequestUSBMode = (ConnectionMode)iAp2MarshalGetUint32(&iAp2Marshal);

            LOGI("mRequestUSBMode : %u, mConnMode : %u", pLnkCtrler->mConnection.mRequestUSBMode, pLnkCtrler->mConnection.mConnMode);
            if( pLnkCtrler->mConnection.mRequestUSBMode != pLnkCtrler->mConnection.mConnMode)
                pLnkCtrler->mpDevMonitor->USBModeChange(pLnkCtrler->mConnection.mRequestUSBMode);
            
            break;
        }

        case kCmdCarPlayClientState: {
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
                    
            pLnkCtrler->mConnection.mCarPlayClientState = (CarPlayClientState)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGI("    pLnkCtrler->mConnection.mCarPlayClientState : %u", pLnkCtrler->mConnection.mCarPlayClientState);
            break;
        }

        case kCmdStartCarPlay: {
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            LOGI("    rcv kCmdStartCarPlay");
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            pLnkCtrler->mpDevMonitor->StartCarPlay();
            break;
        }

        case kCmdTerminateDaemon: {
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            LOGI("    rcv kCmdTerminateDaemon");

            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            pLnkCtrler->mpDevMonitor->TerminateDaemon();
            break;
        }
        
        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}

int SessionConnection::reSettingUtf8Param(CmdSaveUtf8Param* utf8Param, MarshalIo* iAp2Marshal) 
{
    int result = kIap2NoErr;
    if(utf8Param->value != NULL)  {
        free(utf8Param->value);
    }
    uint8_t* tmpString = iAp2MarshalGetBytes(iAp2Marshal, &utf8Param->size);
    utf8Param->value = (uint8_t*)malloc(utf8Param->size + 1);
    if(utf8Param->value == NULL) {
        LOGE("ERROR utf8Param Malloc Fail");
        result = kIap2NoMemoryErr;
        return result;
    }
    else {
        memset(utf8Param->value, 0x00, utf8Param->size + 1);
        strncpy((char*)utf8Param->value, (char*)tmpString, utf8Param->size);
    }   
    return result;
}

}// namespace IAP2
