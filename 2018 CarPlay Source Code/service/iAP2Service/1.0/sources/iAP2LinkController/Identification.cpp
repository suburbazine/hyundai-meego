/*
    File:       Identification.cpp
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

#include "Identification.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include <stdlib.h>
namespace IAP2
{

Identification::Identification()
{
    LOGD("Identification()");

    //init Value;
    memset(&mIdentificationInformation, 0x00 , sizeof(mIdentificationInformation));
    CountRetryIdentfication = 2;
    
    memset(&mIdentficationMsgSelect, 0x00, sizeof(IdentficationMsgSelectArg));
    
    //basic use control message
    mIdentficationMsgSelect.Power = TRUE;
    mIdentficationMsgSelect.Location = TRUE;
    //mIdentficationMsgSelect.NowPlaying = TRUE;
    //mIdentficationMsgSelect.Communications_CallStateUpdate = TRUE;
    mIdentficationMsgSelect.UseEAWithCarPlay = TRUE;
    mIdentficationMsgSelect.iAP2HIDComponentFunction = MEDIAPLAYBACKREMOTE;
#if defined(MIBGP)
    mIdentficationMsgSelect.EAiAP2Session = TRUE;
#endif
    strcpy(mIdentificationInformation.Name,"IAP2");
    strcpy(mIdentificationInformation.ModelIdentifier,"IAP2");
    strcpy(mIdentificationInformation.Manufacturer,"LG Electronics");
    strcpy(mIdentificationInformation.SerialNumber,"0123456789");
    strcpy(mIdentificationInformation.FirmwareVersion,"IAP2Version2");
    strcpy(mIdentificationInformation.HardwareVersion,"9876543210");
    strcpy(mIdentificationInformation.CurrentLanguage,"ko");
}

Identification::~Identification()
{
    LOGD("~Identification()");
}

void Identification::sendIdentificationInformation(LinkController* pLnkCtrler, struct iAP2Link_st* link, uint8_t session, IdentificationRejectedEvt* pEvent)
{
    LOGD("sendIdentificationInformation()");
    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    if (pLnkCtrler->mpDevMonitor) {
        createIdentificationInformationParamList(pLnkCtrler, pLnkCtrler->mpDevMonitor->mpTransport, pEvent, nParamIdList, paramDataList, nParamDataLenList);
    }
    else {
        LOGE("    ERROR : mpDevMonitor is NULL");
    }    

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdIdentificationInformation, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDaraArr((char*)encodedData, nEncodedDataLength);
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDaraArr);

    DataEncoder::deleteParamList(nParamIdList,paramDataList, nParamDataLenList );
    delete [] encodedData;

}

void Identification::createIdentificationInformationParamList(LinkController* pLnkCtrler, Transport* pTransport, IdentificationRejectedEvt* pEvent, std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList)
{
    LOGD("createIdentificationInformationParamList()");
    
    if (pTransport == NULL) {
        LOGE("ERROR : createIdentificationInformationParamList() pTransport is NULL");
        return;
    }
    uint8_t* groupParamBytes = NULL;
    uint32_t nNumOfBytes;
    LOGD("    mAccUSbMode[%d]", pTransport->mAccUSbMode);

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();
    
    createIdentificaitonInformationProductParamList(pEvent, nParamIdList, paramDataList, nParamDataLenList);
    
    createIdentificationInformationParamList_MsgSent(pLnkCtrler, pEvent, pTransport, nParamIdList, paramDataList, nParamDataLenList);
    
    createIdentificationInformationParamList_MsgRecv(pLnkCtrler, pEvent, pTransport, nParamIdList, paramDataList, nParamDataLenList);
    
    DataEncoder::appendNumericParam(2, 1, kParamIdIdentification_PowerSourceType, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNumericParam(0, 2, kParamIdIdentification_MaximumCurrent, nParamIdList, paramDataList, nParamDataLenList);

    if(mIdentficationMsgSelect.EAiAP2Session && pEvent->mRejectIdentficationMsgSelect.EAiAP2Session && (pEvent->SupportedExternalAccessoryProtocl == FALSE)) {
        if(!((mIdentficationMsgSelect.UseEAWithCarPlay == FALSE) && (pLnkCtrler->mConnection.mCarplaySupport == CCS_CARPLAY_SUPPORTED) ))
        {
            nNumOfBytes = createGroupParamBytesForSupportedExternalAccessroyProtocolComponent(&groupParamBytes);
            DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdIdentification_SupportedEAP, nParamIdList, paramDataList, nParamDataLenList);
            delete [] groupParamBytes;
            if(mIdentficationMsgSelect.AppMatchTeamID.bPresent == TRUE)
                DataEncoder::appendStringParam((char*)mIdentficationMsgSelect.AppMatchTeamID.value, kParamIdIdentification_AppMatchTeamID, nParamIdList, paramDataList, nParamDataLenList);
            else
                DataEncoder::appendStringParam("156165pyi", kParamIdIdentification_AppMatchTeamID, nParamIdList, paramDataList, nParamDataLenList);                
        }
    }

    if(pEvent->CurrentLanguage == TRUE || mIdentificationInformation.CurrentLanguage == NULL) {
        if ((mIdentficationMsgSelect.supportedLanaguage.bPresent == TRUE) && (mIdentficationMsgSelect.supportedLanaguage.value != NULL) && (pEvent->SupportedLanguage == FALSE)) {
            char* pPosition = NULL;
            char* tmpCurrentSupportedLanaguage = NULL;
            tmpCurrentSupportedLanaguage = (char*)malloc(strlen((char*)mIdentficationMsgSelect.supportedLanaguage.value)+1);
            memset(tmpCurrentSupportedLanaguage, 0x00, strlen((char*)mIdentficationMsgSelect.supportedLanaguage.value)+1);
            LOGD("supportedLanguage->value : %s", mIdentficationMsgSelect.supportedLanaguage.value );
            strcpy(tmpCurrentSupportedLanaguage, (char*)mIdentficationMsgSelect.supportedLanaguage.value);
            LOGD("tmpCurrentSupportedLanaguage : %s", tmpCurrentSupportedLanaguage );
            pPosition = strtok( (char*)tmpCurrentSupportedLanaguage, " ");
            if( pPosition == NULL ) {
                DataEncoder::appendStringParam("en", kParamIdIdentification_CurrentLanguage, nParamIdList, paramDataList, nParamDataLenList);
            }
            else {
                DataEncoder::appendStringParam(pPosition, kParamIdIdentification_CurrentLanguage, nParamIdList, paramDataList, nParamDataLenList);
            }
            free(tmpCurrentSupportedLanaguage);
        }
        else {
            DataEncoder::appendStringParam("en", kParamIdIdentification_CurrentLanguage, nParamIdList, paramDataList, nParamDataLenList);
        }
    }
    else {
        LOGD("mIdentificationInformation.CurrentLanguage : %s ", mIdentificationInformation.CurrentLanguage);
        DataEncoder::appendStringParam(mIdentificationInformation.CurrentLanguage, kParamIdIdentification_CurrentLanguage, nParamIdList, paramDataList, nParamDataLenList);
    }
    
    createCurrentSupportLanguage(pEvent, mIdentficationMsgSelect.supportedLanaguage, nParamIdList, paramDataList, nParamDataLenList);

    if (pTransport->mAccUSbMode == LINKCTRL_AUB_DEVICE) {
        nNumOfBytes = createGroupParamBytesForUsbHostTransportComponent(&groupParamBytes);
        DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdIdentification_USBHostTransportComponent, nParamIdList, paramDataList, nParamDataLenList);
        delete [] groupParamBytes;
    }
    else {
        nNumOfBytes = createGroupParamBytesForUsbDeviceTransportComponent(&groupParamBytes);
        DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdIdentification_USBDeviceTransportComponent, nParamIdList, paramDataList, nParamDataLenList);
        delete [] groupParamBytes;
    }

    if(mIdentficationMsgSelect.HIDiAP2SessionMediaPlaybackRemote && pEvent->mRejectIdentficationMsgSelect.HIDiAP2SessionMediaPlaybackRemote)
    {    
        LOGD("Set HID Component in the Identification Message");
        nNumOfBytes = createGroupParamBytesForIap2HidComponent(&groupParamBytes);
    
        //LOGD("    groupParamBytes kParamIdIdentification_iAP2HIDComponent");
        //LOGD("    groupParamBytes[%s] nNumOfBytes[%u]", (const char*)groupParamBytes, nNumOfBytes);

        DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdIdentification_iAP2HIDComponent, nParamIdList, paramDataList, nParamDataLenList);
        delete [] groupParamBytes;
    }

    if(mIdentficationMsgSelect.Location && pEvent->mRejectIdentficationMsgSelect.Location)
    {
        LOGD("Set Location Component in the Identification Message");
        nNumOfBytes = createGroupParamBytesForLocationInformationComponent(&groupParamBytes);

        //LOGD("    groupParamBytes kParamIdIdentification_LocationInformationComponent");
        //LOGD("    groupParamBytes[%s] nNumOfBytes[%u]", (const char*)groupParamBytes, nNumOfBytes);
    
        DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdIdentification_LocationInformationComponent, nParamIdList, paramDataList, nParamDataLenList);
        delete [] groupParamBytes;
    }

    if(mIdentficationMsgSelect.BluetoothConnection && (pEvent->BluetoothTransportComponent == FALSE))
    {
        nNumOfBytes = createGroupParamBytesForBTTransportComponent(&groupParamBytes);
    
        DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdIdentification_BluetoothTransportComponent, nParamIdList, paramDataList, nParamDataLenList);
        delete [] groupParamBytes;
     }     

    if(mIdentficationMsgSelect.VehicleStatus && pEvent->mRejectIdentficationMsgSelect.VehicleStatus)
    {
        LOGD("Set vehicle Status Component in the Identification Message");
       
        nNumOfBytes = createGroupParamBytesForVehicleStatusComponent(&groupParamBytes);

        //LOGD("    groupParamBytes kParamIdIdentification_VehicleStatusComponent");
        //LOGD("    groupParamBytes[%s] nNumOfBytes[%u]", (const char*)groupParamBytes, nNumOfBytes);

        DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdIdentification_VehicleStatusComponent, nParamIdList, paramDataList, nParamDataLenList);
        delete [] groupParamBytes;
    }

    if (pLnkCtrler->mConnection.mCarplaySupport == CCS_CARPLAY_SUPPORTED || (mIdentficationMsgSelect.VehicleInformation && pEvent->VehicleInformationComponent)) {
        LOGD("Set VehicleInformation Component in the Identification Message");
        nNumOfBytes = createGroupParamBytesForVehicleInformationComponent(&groupParamBytes);
        DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdIdentification_VehicleInformationComponent, nParamIdList, paramDataList, nParamDataLenList);
        delete [] groupParamBytes;
    }
}

void Identification::createIdentificaitonInformationProductParamList(IdentificationRejectedEvt* pEvent, std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList)
{    
    LOGD("createIdentificaitonInformationProductParamList()");

    LOGD("ProductName : %s", mIdentificationInformation.Name);
    LOGD("ModelIdentifier : %s", mIdentificationInformation.ModelIdentifier);
    LOGD("Manufacturer : %s", mIdentificationInformation.Manufacturer);
    LOGD("SerialNumber : %s", mIdentificationInformation.SerialNumber);
    LOGD("FirmwareVersion : %s", mIdentificationInformation.FirmwareVersion);
    LOGD("HardwareVersion : %s", mIdentificationInformation.HardwareVersion);
    if(pEvent->Name == TRUE || strlen(mIdentificationInformation.Name) == 0)
        DataEncoder::appendStringParam("IAP2Module", kParamIdIdentification_Name, nParamIdList, paramDataList, nParamDataLenList);        
    else
        DataEncoder::appendStringParam(mIdentificationInformation.Name, kParamIdIdentification_Name, nParamIdList, paramDataList, nParamDataLenList);

    if(pEvent->ModelIdentifier == TRUE || strlen(mIdentificationInformation.ModelIdentifier) == 0)
        DataEncoder::appendStringParam("IAP2", kParamIdIdentification_ModelIdentifier, nParamIdList, paramDataList, nParamDataLenList);        
    else
        DataEncoder::appendStringParam(mIdentificationInformation.ModelIdentifier, kParamIdIdentification_ModelIdentifier, nParamIdList, paramDataList, nParamDataLenList);

    if(pEvent->Manufacturer == TRUE || strlen(mIdentificationInformation.Manufacturer) == 0)
        DataEncoder::appendStringParam("LG Electronics", kParamIdIdentification_Manufacturer, nParamIdList, paramDataList, nParamDataLenList);        
    else
        DataEncoder::appendStringParam(mIdentificationInformation.Manufacturer, kParamIdIdentification_Manufacturer, nParamIdList, paramDataList, nParamDataLenList);

    if(pEvent->SerialNumber == TRUE || strlen(mIdentificationInformation.SerialNumber) == 0)
        DataEncoder::appendStringParam("1234567890", kParamIdIdentification_SerialNumber, nParamIdList, paramDataList, nParamDataLenList);        
    else
        DataEncoder::appendStringParam(mIdentificationInformation.SerialNumber, kParamIdIdentification_SerialNumber, nParamIdList, paramDataList, nParamDataLenList);

    if(pEvent->FirmwareVersion == TRUE || strlen(mIdentificationInformation.FirmwareVersion) == 0)
        DataEncoder::appendStringParam("IAP2 Firmawre Version 1.0", kParamIdIdentification_FirmwareVersion, nParamIdList, paramDataList, nParamDataLenList);        
    else
        DataEncoder::appendStringParam(mIdentificationInformation.FirmwareVersion, kParamIdIdentification_FirmwareVersion, nParamIdList, paramDataList, nParamDataLenList);
    
    if(pEvent->HardwareVersion == TRUE || strlen(mIdentificationInformation.HardwareVersion) == 0)
        DataEncoder::appendStringParam("IAP2 HardWareVersion 1.0", kParamIdIdentification_HardwareVersion, nParamIdList, paramDataList, nParamDataLenList);        
    else
        DataEncoder::appendStringParam(mIdentificationInformation.HardwareVersion, kParamIdIdentification_HardwareVersion, nParamIdList, paramDataList, nParamDataLenList);        
}    

void Identification::createIdentificationInformationParamList_MsgSent(LinkController* pLnkCtrler, IdentificationRejectedEvt* pEvent, Transport* pTransport, std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList)
{
    // Msg Sent by Accessory
    LOGD("createIdentificationInformationParamList_MsgSent");
    uint8_t* bytesData = new uint8_t[256];
    int nStartPosition = 0;

   // if((mIdentficationMsgSelect.Power && pEvent->mRejectIdentficationMsgSelect.Power) || pLnkCtrler->mConnection.mCarplaySupport == CCS_CARPLAY_SUPPORTED) {
        LOGD("Set Send Power Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartPowerUpdates);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdPowerSourceUpdate);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopPowerUpdates);
  //  }

    if((mIdentficationMsgSelect.Location && pEvent->mRejectIdentficationMsgSelect.Location) || pLnkCtrler->mConnection.mCarplaySupport == CCS_CARPLAY_SUPPORTED) {
        LOGD("Set Send Location Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdLocationInformation);
    }

    if(mIdentficationMsgSelect.NowPlaying && pEvent->mRejectIdentficationMsgSelect.NowPlaying) {
        LOGD("Set Send NowPlaying Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartNowPlayingUpdates);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopNowPlayingUpdates);
//        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdSetNowPlayingInformation);
    }

    if(mIdentficationMsgSelect.Communications_CallControls && pEvent->mRejectIdentficationMsgSelect.Communications_CallControls) {
        LOGD("Set Send Communications_CallControls Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdInitialCall);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdAcceptCall);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdEndCall);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdSwapCalls);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdMergeCalls);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdHoldStatusUpdate);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdMuteStatusUpdate);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdSendDTMF);
    }

    if(mIdentficationMsgSelect.Communications_SendDTMFUse && pEvent->mRejectIdentficationMsgSelect.Communications_SendDTMFUse){
        LOGD("Set Send Communications_SendDTMF Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdSendDTMF);
    }

    if(mIdentficationMsgSelect.Communications_CallStateUpdate && pEvent->mRejectIdentficationMsgSelect.Communications_CallStateUpdate) {
        LOGD("Set Send Communications_CallStateUpdate Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartCallStateUpdates);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopCallStateUpdates);
    }

    if(mIdentficationMsgSelect.Communications_CommunicationsUpdate && pEvent->mRejectIdentficationMsgSelect.Communications_CommunicationsUpdate) {
        LOGD("Set Send Communications_CommunicationsUpdate Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartCommunicationsUpdates);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopCommunicationsUpdates);
    }

    if(mIdentficationMsgSelect.Communications_ListUpdates && pEvent->mRejectIdentficationMsgSelect.Communications_ListUpdates) {
        LOGD("Set Send Communications_CommunicationsUpdate Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartListUpdate);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopListUpdate);
    }

    if(mIdentficationMsgSelect.HIDiAP2SessionMediaPlaybackRemote && pEvent->mRejectIdentficationMsgSelect.HIDiAP2SessionMediaPlaybackRemote) {
        LOGD("Set Send HIDiAP2SessionMediaPlaybackRemote Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartHID);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdAccessoryHIDReport);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopHID);
    }
    
    if(mIdentficationMsgSelect.MediaLibraryInformation && pEvent->mRejectIdentficationMsgSelect.MediaLibraryInformation) {
        LOGD("Set Send MediaLibraryInformation Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartMediaLibraryInformation);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopMediaLibraryInformation);
    }    

    if(mIdentficationMsgSelect.MediaLibraryUpdates && pEvent->mRejectIdentficationMsgSelect.MediaLibraryUpdates) {
        LOGD("Set Send MediaLibraryUpdates Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartMediaLibraryUpdates);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopMediaLibraryUpdates);
    }

    if(mIdentficationMsgSelect.MediaLibraryPlayback && pEvent->mRejectIdentficationMsgSelect.MediaLibraryPlayback) {
        LOGD("Set Send MediaLibraryPlayback Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdPlayMediaLibraryCurrentSelection);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdPlayMediaLibraryItems);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdPlayMediaLibraryCollection);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdPlayMediaLibrarySpecial);
    }

    if(mIdentficationMsgSelect.AppLaunch && pEvent->mRejectIdentficationMsgSelect.AppLaunch) {
        LOGD("Set Send AppLaunch Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdRequestAppLaunch);
    }

    if(mIdentficationMsgSelect.AssistiveTouch && pEvent->mRejectIdentficationMsgSelect.AssistiveTouch) {
        LOGD("Set Send AssistiveTouch Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartAssistiveTouch);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopAssistiveTouch);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartAssistiveTouchInformation);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopAssistiveTouchInformation);
    }

    if(mIdentficationMsgSelect.BluetoothConnection && pEvent->mRejectIdentficationMsgSelect.BluetoothConnection) {
        //TODO Don't develop BluetoothConnection yet.
        //LOGD("Set Send BluetoothConnection Message in the Identificaiton Message");
        //nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdBluetoothComponentInformation);
        //nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartBluetoothConnectionUpdates);
        //nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopBluetoothConnectionUpdates);
        //nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStatusExternalAccessoryProtocolSession);
    }

    if(mIdentficationMsgSelect.EAiAP2Session && pEvent->mRejectIdentficationMsgSelect.EAiAP2Session && (pEvent->SupportedExternalAccessoryProtocl == FALSE)) {
        if(!((mIdentficationMsgSelect.UseEAWithCarPlay == FALSE) && (pLnkCtrler->mConnection.mCarplaySupport == CCS_CARPLAY_SUPPORTED) )) {
            LOGD("Set Send EAiAP2Session Message in the Identificaiton Message");
            nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStatusExternalAccessoryProtocolSession);
        }
    }

    if(mIdentficationMsgSelect.EAiAP2Session && pEvent->mRejectIdentficationMsgSelect.EAiAP2Session && (pEvent->SupportedExternalAccessoryProtocl == FALSE)) {
        //TODO Don't develop EAiAP2Session yet.
    }

    if(mIdentficationMsgSelect.EANativeTransPort && pEvent->mRejectIdentficationMsgSelect.EANativeTransPort) {
        //TODO Don't develop EANativeTransPort yet.
    }

    if(mIdentficationMsgSelect.VehicleStatus && pEvent->mRejectIdentficationMsgSelect.VehicleStatus) {
        LOGD("Set Send VehicleStatus Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdVehicleStatusUpdate);
    }
    

    if(mIdentficationMsgSelect.VoiceOver && pEvent->mRejectIdentficationMsgSelect.VoiceOver) {
        LOGD("Set Send VoiceOver Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartVoiceOver);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopVoiceOver);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdRequestVoiceOverMoveCursor);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdRequestVoiceOverActivateCursor);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdRequestVoiceOverScrollPage);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdRequestVoiceOverSpeakText);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdRequestVoiceOverPauseText);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdRequestVoiceOverResumeText);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartVoiceOverUpdates);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopVoiceOverUpdates);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdRequestVoiceOverConfiguration);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartVoiceOverCursorUpdates);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopVoiceOverCursorUpdates);
    }

    if(mIdentficationMsgSelect.WiFiInformationSharing && pEvent->mRejectIdentficationMsgSelect.WiFiInformationSharing) {
        LOGD("Set Send WiFiInformationSharing Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdRequestWiFiInformation);
    }

    if(mIdentficationMsgSelect.USBDigitalAudioDeviceMode && pEvent->mRejectIdentficationMsgSelect.USBDigitalAudioDeviceMode) {
        
        if (pTransport->mAccUSbMode == LINKCTRL_AUB_HOST) {
            LOGD("Set Send USBDigitalAudioDeviceMode Message in the Identificaiton Message");
            nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartUSBDeviceModeAudio);
            nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopUSBDeviceModeAudio);
        }
    }
    
    DataEncoder::appendBytesParam(bytesData, nStartPosition, kParamIdIdentification_MsgSentByAccessory, nParamIdList, paramDataList, nParamDataLenList);
    delete [] bytesData;
}


void Identification::createIdentificationInformationParamList_MsgRecv(LinkController* pLnkCtrler, IdentificationRejectedEvt* pEvent, Transport* pTransport, std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList)
{
    // Msg Sent by Accessory
    uint8_t* bytesData = new uint8_t[256];
    int nStartPosition = 0;

 //   if((mIdentficationMsgSelect.Power && pEvent->mRejectIdentficationMsgSelect.Power) || pLnkCtrler->mConnection.mCarplaySupport == CCS_CARPLAY_SUPPORTED) {
        LOGD("Set Recv Power Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdPowerUpdate);  
 //   }

    if((mIdentficationMsgSelect.Location && pEvent->mRejectIdentficationMsgSelect.Location) || pLnkCtrler->mConnection.mCarplaySupport == CCS_CARPLAY_SUPPORTED) {
        LOGD("Set Recv Location Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartLocationInformation);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopLocationInformation); 
    }
    
    if(mIdentficationMsgSelect.NowPlaying && pEvent->mRejectIdentficationMsgSelect.NowPlaying) {
        LOGD("Set Recv NowPlaying Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdNowPlayingUpdates);
    }

    if(mIdentficationMsgSelect.Communications_CallStateUpdate && pEvent->mRejectIdentficationMsgSelect.Communications_CallStateUpdate) {
        LOGD("Set Recv Communications_CallStateUpdate Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdCallStateUpdates);
    }

    if(mIdentficationMsgSelect.Communications_CommunicationsUpdate && pEvent->mRejectIdentficationMsgSelect.Communications_CommunicationsUpdate) {
        LOGD("Set Recv Communications_CommunicationsUpdate Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdCommunicationsUpdates);
    }

    if(mIdentficationMsgSelect.Communications_ListUpdates && pEvent->mRejectIdentficationMsgSelect.Communications_ListUpdates) {
        LOGD("Set Recv Communications_ListUpdates Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdListUpdate);    
    }

    if(mIdentficationMsgSelect.HIDiAP2SessionMediaPlaybackRemote && pEvent->mRejectIdentficationMsgSelect.HIDiAP2SessionMediaPlaybackRemote) {
        LOGD("Set Recv HIDiAP2SessionMediaPlaybackRemote Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdDeviceHIDReport);    
    }

    if(mIdentficationMsgSelect.MediaLibraryInformation && pEvent->mRejectIdentficationMsgSelect.MediaLibraryInformation) {
        LOGD("Set Recv MediaLibraryInformation Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdMediaLibraryInformation);    
    }

    if(mIdentficationMsgSelect.MediaLibraryUpdates && pEvent->mRejectIdentficationMsgSelect.MediaLibraryUpdates) {
        LOGD("Set Recv MediaLibraryUpdates Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdMediaLibraryUpdate);    
    }

    if(mIdentficationMsgSelect.DeviceInformationUpdate && pEvent->mRejectIdentficationMsgSelect.DeviceInformationUpdate) {
        LOGD("Set Recv DeviceInformationUpdate Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdDeviceInformationUpdate); 
    }

    if(mIdentficationMsgSelect.DeviceLanguageUpdate && pEvent->mRejectIdentficationMsgSelect.DeviceLanguageUpdate) {
        LOGD("Set Recv DeviceLanguageUpdate Message in the Identificaiton Message"); 
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdDeviceLanguageUpdate);
    }

    if(mIdentficationMsgSelect.DeviceTimeUpdate && pEvent->mRejectIdentficationMsgSelect.DeviceTimeUpdate) {
        LOGD("Set Recv DeviceTimeUpdate Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdDeviceTimeUpdate); 
    }

    if(mIdentficationMsgSelect.DeviceUUIDUpdate && pEvent->mRejectIdentficationMsgSelect.DeviceUUIDUpdate) {
        LOGD("Set Recv DeviceUUIDUpdate Message in the Identificaiton Message"); 
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdDeviceUUIDUpdate);
    }
    
    if(mIdentficationMsgSelect.AssistiveTouch && pEvent->mRejectIdentficationMsgSelect.AssistiveTouch) {
        LOGD("Set Recv AssistiveTouch Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdAssistiveTouchInformation);
    }

    if(mIdentficationMsgSelect.BluetoothConnection && pEvent->mRejectIdentficationMsgSelect.BluetoothConnection) {
        //TODO Don't develop BluetoothConnection yet.
        //LOGD("Set Recv BluetoothConnection Message in the Identificaiton Message");
        //nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdBluetoothConnectionUpdate);
    }

    if(mIdentficationMsgSelect.EAiAP2Session && pEvent->mRejectIdentficationMsgSelect.EAiAP2Session && (pEvent->SupportedExternalAccessoryProtocl == FALSE)) {
        if(!((mIdentficationMsgSelect.UseEAWithCarPlay == FALSE) && (pLnkCtrler->mConnection.mCarplaySupport == CCS_CARPLAY_SUPPORTED) ))
        {
            LOGD("Set Recv EAiAP2Session Message in the Identificaiton Message");
            nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartExternalAccessoryProtocolSession);
            nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopExternalAccessoryProtocolSession);
        }
    }

    if(mIdentficationMsgSelect.VehicleStatus && pEvent->mRejectIdentficationMsgSelect.VehicleStatus) {
        LOGD("Set Recv VehicleStatus Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStartVehicleStatusUpdates);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdStopVehicleStatusUpdates);
    }

    if(mIdentficationMsgSelect.VoiceOver && pEvent->mRejectIdentficationMsgSelect.VoiceOver) {
        LOGD("Set Recv VoiceOver Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdVoiceOverUpdate);
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdVoiceOverCursorUpdate);
    }

    if(mIdentficationMsgSelect.WiFiInformationSharing && pEvent->mRejectIdentficationMsgSelect.WiFiInformationSharing) {
        LOGD("Set Recv WiFiInformationSharing Message in the Identificaiton Message");
        nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdWiFiInformation);
    }

    if(mIdentficationMsgSelect.USBDigitalAudioDeviceMode && pEvent->mRejectIdentficationMsgSelect.USBDigitalAudioDeviceMode) {
        LOGD("Set Recv USBDigitalAudioDeviceMode Message in the Identificaiton Message");
        if (pTransport->mAccUSbMode == LINKCTRL_AUB_HOST) {    
            nStartPosition = DataEncoder::setBytes(bytesData, nStartPosition, 2, kMsgIdUSBDeviceModeAudioInformation);
        }
    }
    

    DataEncoder::appendBytesParam(bytesData, nStartPosition, kParamIdIdentification_MsgReceivedFromDevice, nParamIdList, paramDataList, nParamDataLenList);
    delete [] bytesData;
}

uint32_t Identification::createGroupParamBytesForUsbDeviceTransportComponent(uint8_t** encodedBytes)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;

    uint8_t* encodedDataArr;

    createUsbDeviceTransportComponentParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];

    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);

    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    *encodedBytes = encodedDataArr;

    return nTotalLength;
}

uint32_t Identification::createGroupParamBytesForUsbHostTransportComponent(uint8_t** encodedBytes)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;

    uint8_t* encodedDataArr;

    createUsbHostTransportComponentParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];

    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);

    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    *encodedBytes = encodedDataArr;

    return nTotalLength;
}

uint32_t Identification::createGroupParamBytesForIap2HidComponent(uint8_t** encodedBytes)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;

    uint8_t* encodedDataArr;

    createiAp2HidComponentParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];

    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);

    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    *encodedBytes = encodedDataArr;

    return nTotalLength;
}


uint32_t Identification::createGroupParamBytesForBTTransportComponent(uint8_t** encodedBytes)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;

    uint8_t* encodedDataArr;

    createBTTransportComponentParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];

    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);

    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    *encodedBytes = encodedDataArr;

    return nTotalLength;
}

uint32_t Identification::createGroupParamBytesForVehicleInformationComponent(uint8_t** encodedBytes)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;

    uint8_t* encodedDataArr;

    createVehicleInformationComponentParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];

    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);

    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    *encodedBytes = encodedDataArr;

    return nTotalLength;
}

uint32_t Identification::createGroupParamBytesForVehicleStatusComponent(uint8_t** encodedBytes)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;

    uint8_t* encodedDataArr;

    createVehicleStatusComponentParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];

    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);

    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    *encodedBytes = encodedDataArr;

    return nTotalLength;
}

uint32_t Identification::createGroupParamBytesForLocationInformationComponent(uint8_t** encodedBytes)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;

    uint8_t* encodedDataArr;

    createLocationInformationComponentParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];

    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);

    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    *encodedBytes = encodedDataArr;

    return nTotalLength;
}

uint32_t Identification::createGroupParamBytesForSupportedExternalAccessroyProtocolComponent(uint8_t** encodedBytes )
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;

    uint8_t* encodedDataArr;

    createSupportedExternalAccessroyProtocolParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];

    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);

    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);

    *encodedBytes = encodedDataArr;

    return nTotalLength;
}



void Identification::createUsbDeviceTransportComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList)
{
    DataEncoder::appendNumericParam(0x0011, 2, kSubParamIdIdentificationTransportComponentIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    if( mIdentficationMsgSelect.USBDeviceTransportComponentName.bPresent == TRUE ) {
        DataEncoder::appendStringParam((char*)mIdentficationMsgSelect.USBDeviceTransportComponentName.value, kSubParamIdIdentificationTransportComponentName, nParamIdList, paramDataList, nParamDataLenList);
    }
   else {
        DataEncoder::appendStringParam("iAP2", kSubParamIdIdentificationTransportComponentName, nParamIdList, paramDataList, nParamDataLenList);
    }
    DataEncoder::appendNoneParam(kSubParamIdIdentificationTransportSupportsiAP2Connection, nParamIdList, paramDataList, nParamDataLenList);

    if( mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate8000Hz == TRUE ) {
        DataEncoder::appendNumericParam(SAMPLELATE_8000, 1, kSubParamIdIdentificationUsbDeviceSupportedAudioSampleRate, nParamIdList, paramDataList, nParamDataLenList);
    }
    if( mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate11025Hz == TRUE ) {
        DataEncoder::appendNumericParam(SAMPLELATE_11025, 1, kSubParamIdIdentificationUsbDeviceSupportedAudioSampleRate, nParamIdList, paramDataList, nParamDataLenList);
    }
    if( mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate12000Hz == TRUE ) {
        DataEncoder::appendNumericParam(SAMPLELATE_12000, 1, kSubParamIdIdentificationUsbDeviceSupportedAudioSampleRate, nParamIdList, paramDataList, nParamDataLenList);
    }
    if( mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate16000Hz == TRUE ) {
        DataEncoder::appendNumericParam(SAMPLELATE_16000, 1, kSubParamIdIdentificationUsbDeviceSupportedAudioSampleRate, nParamIdList, paramDataList, nParamDataLenList);
    }
    if( mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate22050Hz == TRUE ) {
        DataEncoder::appendNumericParam(SAMPLELATE_22050, 1, kSubParamIdIdentificationUsbDeviceSupportedAudioSampleRate, nParamIdList, paramDataList, nParamDataLenList);
    }
    if( mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate24000Hz == TRUE ) {
        DataEncoder::appendNumericParam(SAMPLELATE_24000, 1, kSubParamIdIdentificationUsbDeviceSupportedAudioSampleRate, nParamIdList, paramDataList, nParamDataLenList);
    }
    //if( mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate32000Hz == TRUE ) { //Accessory must at least 32000, 44100 and 48000 Hz sample rate.
        DataEncoder::appendNumericParam(SAMPLELATE_32000, 1, kSubParamIdIdentificationUsbDeviceSupportedAudioSampleRate, nParamIdList, paramDataList, nParamDataLenList);
   // }
   // if( mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate44100Hz == TRUE ) {
        DataEncoder::appendNumericParam(SAMPLELATE_44100, 1, kSubParamIdIdentificationUsbDeviceSupportedAudioSampleRate, nParamIdList, paramDataList, nParamDataLenList);
    //}
    //if( mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate48000Hz == TRUE ) {
        DataEncoder::appendNumericParam(SAMPLELATE_48000, 1, kSubParamIdIdentificationUsbDeviceSupportedAudioSampleRate, nParamIdList, paramDataList, nParamDataLenList);
    //}
    
}

void Identification::createUsbHostTransportComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList)
{
    DataEncoder::appendNumericParam(0x1234, 2, kSubParamIdIdentificationHostTransportComponentIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    if( mIdentficationMsgSelect.USBHostTransportComponentName.bPresent == TRUE ) {
        DataEncoder::appendStringParam((char*)mIdentficationMsgSelect.USBHostTransportComponentName.value, kSubParamIdIdentificationHostTransportComponentName, nParamIdList, paramDataList, nParamDataLenList);
    }
    else {
        DataEncoder::appendStringParam("iAP2", kSubParamIdIdentificationHostTransportComponentName, nParamIdList, paramDataList, nParamDataLenList);
    }
    DataEncoder::appendNoneParam(kSubParamIdIdentificationHostTransportSupportsiAP2Connection, nParamIdList, paramDataList, nParamDataLenList);    
    
    DataEncoder::appendNumericParam(1, 1, kSubParamIdIdentificationUsbHostTransportCarplayInterfaceNumber, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kSubParamIdIdentificationUsbHostTransportSupportsCarPlay, nParamIdList, paramDataList, nParamDataLenList);
}


void Identification::createiAp2HidComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList)
{
    DataEncoder::appendNumericParam(0x0000, 2, kSubParamIdIdentificationHidComponentIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    if( mIdentficationMsgSelect.iAP2HIDComponentName.bPresent == TRUE ) {
        DataEncoder::appendStringParam((char*)mIdentficationMsgSelect.iAP2HIDComponentName.value, kSubParamIdIdentificationHidComponentName, nParamIdList, paramDataList, nParamDataLenList);
    }
    else {
        DataEncoder::appendStringParam("Media Playback Remote", kSubParamIdIdentificationHidComponentName, nParamIdList, paramDataList, nParamDataLenList);
    }
    DataEncoder::appendNumericParam(mIdentficationMsgSelect.iAP2HIDComponentFunction, 1, kSubParamIdIdentificationHidComponentFunction, nParamIdList, paramDataList, nParamDataLenList);
}

void Identification::createBTTransportComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList)
{
    // ToDoVibianne
    DataEncoder::appendNumericParam(0x0022, 2, kSubParamIdIdentificationBTTransportIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    if( mIdentficationMsgSelect.BluetoothTransportComponentName.bPresent == TRUE ) {
        DataEncoder::appendStringParam((char*)mIdentficationMsgSelect.BluetoothTransportComponentName.value, kSubParamIdIdentificationBTTransportName, nParamIdList, paramDataList, nParamDataLenList);
    }
    else {
        DataEncoder::appendStringParam("DH", kSubParamIdIdentificationBTTransportName, nParamIdList, paramDataList, nParamDataLenList);
    }
    DataEncoder::appendNoneParam(kSubParamIdIdentificationBTTransportSupportsiAP2Connection, nParamIdList, paramDataList, nParamDataLenList);

    if( mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.bPresent == TRUE ) {
        DataEncoder::appendStringParam((char*)mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.value, kSubParamIdIdentificationBTTransportMediaAccessControlAddress, nParamIdList, paramDataList, nParamDataLenList);
    }
    else {
        DataEncoder::appendStringParam("000E34020F2F", kSubParamIdIdentificationBTTransportMediaAccessControlAddress, nParamIdList, paramDataList, nParamDataLenList);
    }
}

void Identification::createVehicleInformationComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList)
{
    
    DataEncoder::appendNumericParam(0x0077, 2, kSubParamIdIdentificationVehicleInformationIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    if( mIdentficationMsgSelect.VehicleInformationComponentName.bPresent == TRUE ) {
        DataEncoder::appendStringParam((char*)mIdentficationMsgSelect.VehicleInformationComponentName.value, kSubParamIdIdentificationVehicleInformationName, nParamIdList, paramDataList, nParamDataLenList);
    }
    else {
        DataEncoder::appendStringParam("Vehicle Information", kSubParamIdIdentificationVehicleInformationName, nParamIdList, paramDataList, nParamDataLenList);
    }

    if(mIdentficationMsgSelect.VehicleInformationEngineTypes.bPresent == TRUE) {
        DataEncoder::appendNumericParam(mIdentficationMsgSelect.VehicleInformationEngineTypes.value, 1, kSubParamIdIdentificationVehicleInformationEngineType, nParamIdList, paramDataList, nParamDataLenList);
    }
    
    if( mIdentficationMsgSelect.VehicleInformationComponentDisplayName.bPresent == TRUE ) {
        DataEncoder::appendStringParam((char*)mIdentficationMsgSelect.VehicleInformationComponentDisplayName.value, kSubParamIdIdentificationVehicleInformationDisPlayName, nParamIdList, paramDataList, nParamDataLenList);
    }
    else {
        DataEncoder::appendStringParam("Vehicle Information DisplayName", kSubParamIdIdentificationVehicleInformationDisPlayName, nParamIdList, paramDataList, nParamDataLenList);
    }
}

void Identification::createVehicleStatusComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList)
{
    // ToDoVibianne
    DataEncoder::appendNumericParam(0x0088, 2, kSubParamIdIdentificationVehicleStatusIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    
    if( mIdentficationMsgSelect.VehicleStatusComponentName.bPresent == TRUE ) {
        DataEncoder::appendStringParam((char*)mIdentficationMsgSelect.VehicleStatusComponentName.value, kSubParamIdIdentificationVehicleStatusName, nParamIdList, paramDataList, nParamDataLenList);
    }
    else {
        DataEncoder::appendStringParam("VehicleStatus", kSubParamIdIdentificationVehicleStatusName, nParamIdList, paramDataList, nParamDataLenList);
    }

    if (mIdentficationMsgSelect.VehicleStatusRange == FALSE && mIdentficationMsgSelect.VehicleStatusOutSideTemperature == FALSE &&  mIdentficationMsgSelect.VehicleStatusRangeWarning == FALSE )
    {
        //vehicleStatusComponent should include one param at least.
        DataEncoder::appendNoneParam(kSubParamIdIdentificationVehicleStatusRange, nParamIdList, paramDataList, nParamDataLenList);
    }

    if ( mIdentficationMsgSelect.VehicleStatusRange == TRUE)
        DataEncoder::appendNoneParam(kSubParamIdIdentificationVehicleStatusRange, nParamIdList, paramDataList, nParamDataLenList);
    
    if ( mIdentficationMsgSelect.VehicleStatusOutSideTemperature == TRUE)
        DataEncoder::appendNoneParam(kSubParamIdIdentificationVehicleStatusOutsideTemp, nParamIdList, paramDataList, nParamDataLenList);

    if ( mIdentficationMsgSelect.VehicleStatusRangeWarning == TRUE)
        DataEncoder::appendNoneParam(kSubParamIdIdentificationVehicleStatusRangeWarning, nParamIdList, paramDataList, nParamDataLenList);
}

void Identification::createLocationInformationComponentParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList)
{
    BOOL bLocationParamSet = FALSE;
    DataEncoder::appendNumericParam(0x0099, 2, kSubParamIdIdentificationLocationIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    if( mIdentficationMsgSelect.VehicleStatusComponentName.bPresent == TRUE ) {
        DataEncoder::appendStringParam((char*)mIdentficationMsgSelect.LocationInformationName.value, kSubParamIdIdentificationLocationName, nParamIdList, paramDataList, nParamDataLenList);
    }
    else {
        DataEncoder::appendStringParam("LocationInformation", kSubParamIdIdentificationLocationName, nParamIdList, paramDataList, nParamDataLenList);
    }

    if ( mIdentficationMsgSelect.LocationInformationGPGGA == TRUE) {
        DataEncoder::appendNoneParam(kSubParamIdIdentificationLocationGPSFixData, nParamIdList, paramDataList, nParamDataLenList);
        bLocationParamSet = TRUE;
    }
    if ( mIdentficationMsgSelect.LocationInformationGPRMC == TRUE) {
        DataEncoder::appendNoneParam(kSubParamIdIdentificationLocationRecommendedMinSpecGPSTransitData, nParamIdList, paramDataList, nParamDataLenList);
        bLocationParamSet = TRUE;
    }
    if ( mIdentficationMsgSelect.LocationInformationGPGSV == TRUE) {
        DataEncoder::appendNoneParam(kSubParamIdIdentificationLocationGPSStaellitesInView, nParamIdList, paramDataList, nParamDataLenList);
        bLocationParamSet = TRUE;
    }
    if ( mIdentficationMsgSelect.LocationInformationPASCD == TRUE) {
        DataEncoder::appendNoneParam(kSubParamIdIdentificationLocationVehicleSpeedData, nParamIdList, paramDataList, nParamDataLenList);
        bLocationParamSet = TRUE;
    }
    if ( mIdentficationMsgSelect.LocationInformationPAGCD == TRUE) {
        DataEncoder::appendNoneParam(kSubParamIdIdentificationLocationVehicleGyroData, nParamIdList, paramDataList, nParamDataLenList);
        bLocationParamSet = TRUE;
    }
    if ( mIdentficationMsgSelect.LocationInformationPAACD == TRUE) {
        DataEncoder::appendNoneParam(kSubParamIdIdentificationLocationVehicleAcceelData, nParamIdList, paramDataList, nParamDataLenList);
        bLocationParamSet = TRUE;
    }
    if ( mIdentficationMsgSelect.LocationInformationGPHDT == TRUE) {
        DataEncoder::appendNoneParam(kSubParamIdIdentificationLocationVehicleHeadingData, nParamIdList, paramDataList, nParamDataLenList);
        bLocationParamSet = TRUE;
    }

    if (bLocationParamSet == FALSE) {
        DataEncoder::appendNoneParam(kSubParamIdIdentificationLocationGPSFixData, nParamIdList, paramDataList, nParamDataLenList);
        DataEncoder::appendNoneParam(kSubParamIdIdentificationLocationRecommendedMinSpecGPSTransitData, nParamIdList, paramDataList, nParamDataLenList);
        DataEncoder::appendNoneParam(kSubParamIdIdentificationLocationVehicleSpeedData, nParamIdList, paramDataList, nParamDataLenList);
    }
}

//YI add

void Identification::createSupportedExternalAccessroyProtocolParamList(std::vector<int>& nParamIdList, std::vector<const uint8_t*>& paramDataList, std::vector<uint32_t>& nParamDataLenList)
{
    DataEncoder::appendNumericParam(0x03, 1, kParamIDExternalAccessoryProtocolSessionIdentifier_Identification, nParamIdList, paramDataList, nParamDataLenList);
    if(mIdentficationMsgSelect.ExternalAccessoryProtocolName.bPresent == TRUE) {
        DataEncoder::appendStringParam((char*)mIdentficationMsgSelect.ExternalAccessoryProtocolName.value, kParamIDExternalAccessoryProtocolName, nParamIdList, paramDataList, nParamDataLenList);
    }
    else {
        DataEncoder::appendStringParam("com.pyi.infortainmet.carplay.exlap", kParamIDExternalAccessoryProtocolName, nParamIdList, paramDataList, nParamDataLenList);
    }            
    DataEncoder::appendNumericParam(mIdentficationMsgSelect.ExternalAccessoryProtocolMatchAction, 1, kParamIDExternalAccessoryProtocolMatchAction, nParamIdList, paramDataList, nParamDataLenList);
//DataEncoder::appendNumericParam(0x1234, 2, kParamIDNativeTransportComponentIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kParamIDExternalAccessoryProtocolCarPlay, nParamIdList, paramDataList, nParamDataLenList);
}

int Identification::GetIdentificationInformation()
{
    LOGD("GetIdentificationInformation");
    char buffer[1000];
    char* pParameterName = NULL;
    char* pParameterValue = NULL;
    memset(&buffer, 0x00, sizeof(buffer));

#if (defined(MFI_AUTH_DEVICE_IAP2IDENTIFICATIONINFO))
#define IDENTIFICATIONINFO MFI_AUTH_DEVICE_IAP2IDENTIFICATIONINFO
#else
#define IDENTIFICATIONINFO "/tmp/IdentificationInfo"
#endif
    FILE *fp = fopen(IDENTIFICATIONINFO, "r");
    if(fp == NULL)
    {
        LOGE("fp is NULL");        
        return kIap2NotReadableErr;
    }
    else
    {
        LOGD("Start Setting Identification");
        while(fgets(buffer, sizeof(buffer), fp) != NULL) {
            pParameterName = strtok(buffer, "=");
            pParameterValue = strtok(NULL, "=");
            pParameterValue[strlen(pParameterValue) - 1] = '\0';    
            if(pParameterName == NULL )
            {
                LOGE("pParameterName is NULL!!");
                continue;
            }        
            else if (pParameterValue == NULL)
            {
                LOGE("pParameterValue is NULL!!");    
                continue;
            }
            SetParameterValue(pParameterName, pParameterValue);            
        }    
        LOGD("Finish Setting Identification");
        fclose(fp);
        return kIap2NoErr;
    }
}

void Identification::SetParameterValue(char* pParameterName, char* pParameterValue)
{    
     if(pParameterName && (strcmp("", pParameterName))) {
        if(!strcmp("ProductName", pParameterName)) {
            memset(mIdentificationInformation.Name, 0x00, sizeof(mIdentificationInformation.Name));
            strcpy(mIdentificationInformation.Name, pParameterValue);
            LOGD("Product Name : %s",mIdentificationInformation.Name);
        }
        else if (!strcmp("ModelIdentifier", pParameterName)) {
            memset(mIdentificationInformation.ModelIdentifier, 0x00, sizeof(mIdentificationInformation.ModelIdentifier));
            strcpy(mIdentificationInformation.ModelIdentifier, pParameterValue);
            LOGD("ModelIdentifier : %s",mIdentificationInformation.ModelIdentifier);
        }
        else if (!strcmp("Manufacturer", pParameterName)) {
            memset(mIdentificationInformation.Manufacturer, 0x00, sizeof(mIdentificationInformation.Manufacturer));
            strcpy(mIdentificationInformation.Manufacturer, pParameterValue);
            LOGD("Manufacturer : %s",mIdentificationInformation.Manufacturer);
        }
        else if (!strcmp("SerialNumber", pParameterName)) {
            memset(mIdentificationInformation.SerialNumber, 0x00, sizeof(mIdentificationInformation.SerialNumber));
            strcpy(mIdentificationInformation.SerialNumber, pParameterValue);
            LOGD("SerialNumber : %s",mIdentificationInformation.SerialNumber);
        }
        else if (!strcmp("FirmwareVersion", pParameterName)) {
            memset(mIdentificationInformation.FirmwareVersion, 0x00, sizeof(mIdentificationInformation.FirmwareVersion));
            strcpy(mIdentificationInformation.FirmwareVersion, pParameterValue);
            LOGD("FirmwareVersion : %s",mIdentificationInformation.FirmwareVersion);
        }
        else if (!strcmp("HardwareVersion", pParameterName)) {
            memset(mIdentificationInformation.HardwareVersion, 0x00, sizeof(mIdentificationInformation.HardwareVersion));
            strcpy(mIdentificationInformation.HardwareVersion, pParameterValue);
            LOGD("HardwareVersion : %s",mIdentificationInformation.HardwareVersion);
        }
        else if (!strcmp("CurrentLanguage", pParameterName)) {
            memset(mIdentificationInformation.CurrentLanguage, 0x00, sizeof(mIdentificationInformation.CurrentLanguage));
            strcpy(mIdentificationInformation.CurrentLanguage, pParameterValue);
            LOGD("CurrentLanguage : %s",mIdentificationInformation.CurrentLanguage);
        }
        else if (!strcmp("SupportedLanguage", pParameterName)) {
            mIdentficationMsgSelect.supportedLanaguage.bPresent = TRUE;
            mIdentficationMsgSelect.supportedLanaguage.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.supportedLanaguage.value = (uint8_t*)malloc(mIdentficationMsgSelect.supportedLanaguage.size);
            if(  mIdentficationMsgSelect.supportedLanaguage.value != NULL ) {
                memset(mIdentficationMsgSelect.supportedLanaguage.value, 0x00, mIdentficationMsgSelect.supportedLanaguage.size);
                strncpy((char*)mIdentficationMsgSelect.supportedLanaguage.value, pParameterValue, strlen(pParameterValue));
                LOGD("supportedLanaguage : %s",mIdentficationMsgSelect.supportedLanaguage.value);
            }
            else {
                LOGE ( "Failed malloc !! supportedLanaguage " );
            } 
        }
        else if (!strcmp("Power", pParameterName)) {
            LOGD("Power : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.Power = TRUE;
        }
        else if (!strcmp("Location", pParameterName)) {
            LOGD("Location : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.Location = TRUE;
        }
        else if(!strcmp("LocationInformationName", pParameterName)) {
            mIdentficationMsgSelect.LocationInformationName.bPresent = TRUE;
            mIdentficationMsgSelect.LocationInformationName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.LocationInformationName.value = (uint8_t*)malloc(mIdentficationMsgSelect.LocationInformationName.size);
            if(  mIdentficationMsgSelect.LocationInformationName.value != NULL ) {
                memset(mIdentficationMsgSelect.LocationInformationName.value, 0x00, mIdentficationMsgSelect.LocationInformationName.size);
                strncpy((char*)mIdentficationMsgSelect.LocationInformationName.value, pParameterValue, strlen(pParameterValue));
                LOGD("LocationInformationName : %s",mIdentficationMsgSelect.LocationInformationName.value);
            }
            else {
                LOGE ( "Failed malloc !! LocationInformationName " );
            }              
        }
        else if (!strcmp("LocationInformationGPGGA", pParameterName)) {
            LOGD("LocationInformationGPGGA : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.LocationInformationGPGGA = TRUE;
        }
        else if (!strcmp("LocationInformationGPRMC", pParameterName)) {
            LOGD("LocationInformationGPRMC : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.LocationInformationGPRMC = TRUE;
        }
        else if (!strcmp("LocationInformationGPGSV", pParameterName)) {
            LOGD("LocationInformationGPGSV : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.LocationInformationGPGSV = TRUE;
        }
        else if (!strcmp("LocationInformationPASCD", pParameterName)) {
            LOGD("LocationInformationPASCD : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.LocationInformationPASCD = TRUE;
        }
        else if (!strcmp("LocationInformationPAGCD", pParameterName)) {
            LOGD("LocationInformationPAGCD : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.LocationInformationPAGCD = TRUE;
        }
        else if (!strcmp("LocationInformationPAACD", pParameterName)) {
            LOGD("LocationInformationPAACD : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.LocationInformationPAACD = TRUE;
        }
        else if (!strcmp("LocationInformationGPHDT", pParameterName)) {
            LOGD("LocationInformationGPHDT : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.LocationInformationGPHDT = TRUE;
        }        
        else if (!strcmp("NowPlaying", pParameterName)) {
            LOGD("NowPlaying : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.NowPlaying = TRUE;
        }
        else if (!strcmp("Communications_CallControls", pParameterName)) {
            LOGD("Communications_CallControls : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.Communications_CallControls = TRUE;
        }
        else if (!strcmp("Communications_SendDTMFUse", pParameterName)) {
            LOGD("Communications_SendDTMFUse : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.Communications_SendDTMFUse = TRUE;
        }
        else if (!strcmp("Communications_CallStateUpdate", pParameterName)) {
            LOGD("Communications_CallStateUpdate : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.Communications_CallStateUpdate = TRUE;
        }
        else if (!strcmp("Communications_CommunicationsUpdate", pParameterName)) {
            LOGD("Communications_CommunicationsUpdate : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.Communications_CommunicationsUpdate = TRUE;
        }
        else if (!strcmp("Communications_ListUpdates", pParameterName)) {
            LOGD("Communications_ListUpdates : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.Communications_ListUpdates = TRUE;
        }
        else if (!strcmp("HIDiAP2SessionMediaPlaybackRemote", pParameterName)) {
            LOGD("HIDiAP2SessionMediaPlaybackRemote : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDiAP2SessionMediaPlaybackRemote = TRUE;
        }
        else if (!strcmp("HIDiAP2SessionAssistiveSwitchControl", pParameterName)) {
            LOGD("HIDiAP2SessionAssistiveSwitchControl : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDiAP2SessionAssistiveSwitchControl = TRUE;
        }
        else if (!strcmp("HIDiAP2SessionAssistiveTouchPointer", pParameterName)) {
            LOGD("HIDiAP2SessionAssistiveTouchPointer : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDiAP2SessionAssistiveTouchPointer = TRUE;
        }
        else if (!strcmp("HIDiAP2SessionGameControllerExtened", pParameterName)) {
            LOGD("HIDiAP2SessionGameControllerExtened : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDiAP2SessionGameControllerExtened = TRUE;
        }
        else if (!strcmp("HIDiAP2SessionAssistiveTouchStandard", pParameterName)) {
            LOGD("HIDiAP2SessionAssistiveTouchStandard : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDiAP2SessionAssistiveTouchStandard = TRUE;
        }
        else if (!strcmp("HIDiAP2SessionAssistiveHeadPhone", pParameterName)) {
            LOGD("HIDiAP2SessionAssistiveHeadPhone : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDiAP2SessionAssistiveHeadPhone = TRUE;
        }
        else if (!strcmp("HIDiAP2SessionKeyBoard", pParameterName)) {
            LOGD("HIDiAP2SessionKeyBoard : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDiAP2SessionKeyBoard = TRUE;
        }
        else if (!strcmp("MediaLibraryInformation", pParameterName)) {
            LOGD("MediaLibraryInformation : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.MediaLibraryInformation = TRUE;
        }
        else if (!strcmp("MediaLibraryUpdates", pParameterName)) {
            LOGD("MediaLibraryUpdates : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.MediaLibraryUpdates = TRUE;
        }
        else if (!strcmp("MediaLibraryPlayback", pParameterName)) {
            LOGD("MediaLibraryPlayback : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.MediaLibraryPlayback = TRUE;
        }
        else if (!strcmp("DeviceInformationUpdate", pParameterName)) {
            LOGD("DeviceInformationUpdate : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.DeviceInformationUpdate = TRUE;
        }
        else if (!strcmp("DeviceLanguageUpdate", pParameterName)) {
            LOGD("DeviceLanguageUpdate : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.DeviceLanguageUpdate = TRUE;
        }
        else if (!strcmp("DeviceTimeUpdate", pParameterName)) {
            LOGD("DeviceTimeUpdate : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.DeviceTimeUpdate = TRUE;
        }
        else if (!strcmp("DeviceUUIDUpdate", pParameterName)) {
            LOGD("DeviceUUIDUpdate : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.DeviceUUIDUpdate = TRUE;
        }
        else if (!strcmp("AppLaunch", pParameterName)) {
            LOGD("AppLaunch : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.AppLaunch = TRUE;
        }
        else if (!strcmp("AssistiveTouch", pParameterName)) {
            LOGD("AssistiveTouch : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.AssistiveTouch = TRUE;
        }
        else if (!strcmp("BluetoothConnection", pParameterName)) {
            LOGD("BluetoothConnection : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.BluetoothConnection = TRUE;
        }
        else if (!strcmp("EAiAP2Session", pParameterName)) {
            LOGD("EAiAP2Session : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.EAiAP2Session = TRUE;
        }
        else if (!strcmp("EAiAP2Session_MultipleSession", pParameterName)) {
            LOGD("EAiAP2Session_MultipleSession : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.EAiAP2Session_MultipleSession = TRUE;
        }
        else if(!strcmp("ExternalAccessoryProtocolName", pParameterName)) {
            mIdentficationMsgSelect.ExternalAccessoryProtocolName.bPresent = TRUE;
            mIdentficationMsgSelect.ExternalAccessoryProtocolName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.ExternalAccessoryProtocolName.value = (uint8_t*)malloc(mIdentficationMsgSelect.ExternalAccessoryProtocolName.size);
            if(  mIdentficationMsgSelect.ExternalAccessoryProtocolName.value != NULL ) {
                memset(mIdentficationMsgSelect.ExternalAccessoryProtocolName.value, 0x00, mIdentficationMsgSelect.ExternalAccessoryProtocolName.size);
                strncpy((char*)mIdentficationMsgSelect.ExternalAccessoryProtocolName.value, pParameterValue, strlen(pParameterValue));
                LOGD("ExternalAccessoryProtocolName : %s",mIdentficationMsgSelect.ExternalAccessoryProtocolName.value);
            }
            else {
                LOGE ( "Failed malloc !! ExternalAccessoryProtocolName " );
            }              
        }
        else if (!strcmp("ExternalAccessoryProtocolMatchAction", pParameterName)) {
            LOGD("ExternalAccessoryProtocolMatchAction : %s", pParameterValue);
            mIdentficationMsgSelect.ExternalAccessoryProtocolMatchAction= NO_ACTION;
            if(!strcmp("OPTIONAL_ACTION", pParameterValue))
                mIdentficationMsgSelect.ExternalAccessoryProtocolMatchAction= OPTIONNAL_ACTION;            
            else if(!strcmp("NO_ALERT", pParameterValue))
                mIdentficationMsgSelect.ExternalAccessoryProtocolMatchAction= NO_ALERT;

             LOGD("mIdentficationMsgSelect.ExternalAccessoryProtocolMatchAction : %u", mIdentficationMsgSelect.ExternalAccessoryProtocolMatchAction);
        }
        else if (!strcmp("AppMatchTeamID", pParameterName)) {
            mIdentficationMsgSelect.AppMatchTeamID.bPresent = TRUE;
            mIdentficationMsgSelect.AppMatchTeamID.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.AppMatchTeamID.value = (uint8_t*)malloc(mIdentficationMsgSelect.AppMatchTeamID.size);
            if(  mIdentficationMsgSelect.AppMatchTeamID.value != NULL ) {
                memset(mIdentficationMsgSelect.AppMatchTeamID.value, 0x00, mIdentficationMsgSelect.AppMatchTeamID.size);
                strncpy((char*)mIdentficationMsgSelect.AppMatchTeamID.value, pParameterValue, strlen(pParameterValue));      
                LOGD("AppMatchTeamID : %s",mIdentficationMsgSelect.AppMatchTeamID.value);
            }
            else {
                LOGE ( "Failed malloc !! AppMatchTeamID " );
            }
        }
        else if (!strcmp("UseEAWithCarPlay", pParameterName)) {
            LOGD("UseEAWithCarPlay : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.UseEAWithCarPlay = TRUE;
        }   
        else if (!strcmp("EANativeTransPort", pParameterName)) {
            LOGD("EANativeTransPort : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.EANativeTransPort = TRUE;
        }    
        else if (!strcmp("HIDNativeTransportMediaPlaybackRemote", pParameterName)) {
            LOGD("HIDNativeTransportMediaPlaybackRemote : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDNativeTransportMediaPlaybackRemote = TRUE;
        }
        else if (!strcmp("HIDNativeTransportAssistiveSwitchControl", pParameterName)) {
            LOGD("HIDNativeTransportAssistiveSwitchControl : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDNativeTransportAssistiveSwitchControl = TRUE;
        }
        else if (!strcmp("HIDNativeTransportAssistiveTouchPointer", pParameterName)) {
            LOGD("HIDNativeTransportAssistiveTouchPointer : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDNativeTransportAssistiveTouchPointer = TRUE;
        }
        else if (!strcmp("HIDNativeTransportGameControllerExtened", pParameterName)) {
            LOGD("HIDNativeTransportGameControllerExtened : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDNativeTransportGameControllerExtened = TRUE;
        }
        else if (!strcmp("HIDNativeTransportAssistiveTouchStandard", pParameterName)) {
            LOGD("HIDNativeTransportAssistiveTouchStandard : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDNativeTransportAssistiveTouchStandard = TRUE;
        }
        else if (!strcmp("HIDNativeTransportAssistiveHeadPhone", pParameterName)) {
            LOGD("HIDNativeTransportAssistiveHeadPhone : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDNativeTransportAssistiveHeadPhone = TRUE;
        }
        else if (!strcmp("HIDNativeTransportKeyBoard", pParameterName)) {
            LOGD("HIDNativeTransportKeyBoard : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.HIDNativeTransportKeyBoard = TRUE;
        }
        else if (!strcmp("VehicleStatus", pParameterName)) {
            LOGD("VehicleStatus : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.VehicleStatus = TRUE;
        }
         else if (!strcmp("VehicleStatusComponentName", pParameterName)) {
            mIdentficationMsgSelect.VehicleStatusComponentName.bPresent = TRUE;
            mIdentficationMsgSelect.VehicleStatusComponentName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.VehicleStatusComponentName.value = (uint8_t*)malloc(mIdentficationMsgSelect.VehicleStatusComponentName.size);
            if(  mIdentficationMsgSelect.VehicleStatusComponentName.value != NULL ) {
                memset(mIdentficationMsgSelect.VehicleStatusComponentName.value, 0x00, mIdentficationMsgSelect.VehicleStatusComponentName.size);
                strncpy((char*)mIdentficationMsgSelect.VehicleStatusComponentName.value, pParameterValue, strlen(pParameterValue));      
                LOGD("VehicleStatusComponentName : %s",mIdentficationMsgSelect.VehicleStatusComponentName.value);
            }
            else {
                LOGE ( "Failed malloc !! VehicleStatusComponentName " );
            }
        }
        else if (!strcmp("VehicleStatusRange", pParameterName)) {
            LOGD("VehicleStatusRange : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.VehicleStatusRange = TRUE;
        }
         else if (!strcmp("VehicleStatusOutSideTemperature", pParameterName)) {
            LOGD("VehicleStatusOutSideTemperature : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.VehicleStatusOutSideTemperature = TRUE;
        }
        else if (!strcmp("VehicleStatusRangeWarning", pParameterName)) {
            LOGD("VehicleStatusRangeWarning : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.VehicleStatusRangeWarning = TRUE;
        }
        else if (!strcmp("VehicleInformation", pParameterName)) {
            LOGD("VehicleInformation : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.VehicleInformation = TRUE;
        }
        else if (!strcmp("VehicleInformationComponentName", pParameterName)) {
            mIdentficationMsgSelect.VehicleInformationComponentName.bPresent = TRUE;
            mIdentficationMsgSelect.VehicleInformationComponentName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.VehicleInformationComponentName.value = (uint8_t*)malloc(mIdentficationMsgSelect.VehicleInformationComponentName.size);
            if(  mIdentficationMsgSelect.VehicleInformationComponentName.value != NULL ) {
                memset(mIdentficationMsgSelect.VehicleInformationComponentName.value, 0x00, mIdentficationMsgSelect.VehicleInformationComponentName.size);
                strncpy((char*)mIdentficationMsgSelect.VehicleInformationComponentName.value, pParameterValue, strlen(pParameterValue));      
                LOGD("VehicleInformationComponentName : %s",mIdentficationMsgSelect.VehicleInformationComponentName.value);
            }
            else {
                LOGE ( "Failed malloc !! VehicleInformationComponentName " );
            }
        }
        else if (!strcmp("VehicleInformationEngineTypesUse", pParameterName)) {
            LOGD("VehicleInformationEngineTypesUse : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.VehicleInformationEngineTypes.bPresent = TRUE;
        }

        else if (!strcmp("VehicleInformationEngineTypes", pParameterName)) {
            LOGD("VehicleInformationEngineTypes : %s", pParameterValue);
            if(!strcmp("0", pParameterValue))
                mIdentficationMsgSelect.VehicleInformationEngineTypes.value = GASOLINE;
            if(!strcmp("1", pParameterValue))
                mIdentficationMsgSelect.VehicleInformationEngineTypes.value = DIESEL;
            if(!strcmp("2", pParameterValue))
                mIdentficationMsgSelect.VehicleInformationEngineTypes.value = ELECTRIC;
            if(!strcmp("3", pParameterValue))
                mIdentficationMsgSelect.VehicleInformationEngineTypes.value = CNG;
        }
        else if (!strcmp("VehicleInformationComponentDisplayName", pParameterName)) {
            mIdentficationMsgSelect.VehicleInformationComponentDisplayName.bPresent = TRUE;
            mIdentficationMsgSelect.VehicleInformationComponentDisplayName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.VehicleInformationComponentDisplayName.value = (uint8_t*)malloc(mIdentficationMsgSelect.VehicleInformationComponentDisplayName.size);
            if(  mIdentficationMsgSelect.VehicleInformationComponentDisplayName.value != NULL ) {
                memset(mIdentficationMsgSelect.VehicleInformationComponentDisplayName.value, 0x00, mIdentficationMsgSelect.VehicleInformationComponentDisplayName.size);
                strncpy((char*)mIdentficationMsgSelect.VehicleInformationComponentDisplayName.value, pParameterValue, strlen(pParameterValue));      
                LOGD("VehicleInformationComponentDisplayName : %s",mIdentficationMsgSelect.VehicleInformationComponentDisplayName.value);
            }
            else {
                LOGE ( "Failed malloc !! VehicleInformationComponentDisplayName " );
            }
        }
        else if (!strcmp("VoiceOver", pParameterName)) {
            LOGD("VoiceOver : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.VoiceOver = TRUE;
        }
        else if (!strcmp("WiFiInformationSharing", pParameterName)) {
            LOGD("WiFiInformationSharing : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.WiFiInformationSharing = TRUE;
        }
        else if (!strcmp("DeviceAuthentication", pParameterName)) {
            LOGD("DeviceAuthentication : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.DeviceAuthentication = TRUE;
        }
        else if (!strcmp("USBDigitalAudioDeviceMode", pParameterName)) {
            LOGD("USBDigitalAudioDeviceMode : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.USBDigitalAudioDeviceMode = TRUE;
        }
        else if (!strcmp("USBDigitalAudioHostMode", pParameterName)) {
            LOGD("USBDigitalAudioHostMode : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.USBDigitalAudioHostMode = TRUE;
        }
        else if (!strcmp("DevicePowered", pParameterName)) {
            LOGD("DevicePowered : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.DevicePowered = TRUE;
        }
        else if (!strcmp("USBDeviceTransportComponentName", pParameterName)) {
            mIdentficationMsgSelect.USBDeviceTransportComponentName.bPresent = TRUE;
            mIdentficationMsgSelect.USBDeviceTransportComponentName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.USBDeviceTransportComponentName.value = (uint8_t*)malloc(mIdentficationMsgSelect.USBDeviceTransportComponentName.size);
            if(  mIdentficationMsgSelect.USBDeviceTransportComponentName.value != NULL ) {
                memset(mIdentficationMsgSelect.USBDeviceTransportComponentName.value, 0x00, mIdentficationMsgSelect.USBDeviceTransportComponentName.size);
                strncpy((char*)mIdentficationMsgSelect.USBDeviceTransportComponentName.value, pParameterValue, strlen(pParameterValue));      
                LOGD("USBDeviceTransportComponentName : %s",mIdentficationMsgSelect.USBDeviceTransportComponentName.value);
            }
            else {
                LOGE ( "Failed malloc !! USBDeviceTransportComponentName " );
            }
        }
        else if (!strcmp("USBDeviceSurpportedAudioSampleRate8000Hz", pParameterName)) {
            LOGD("USBDeviceSurpportedAudioSampleRate8000Hz : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate8000Hz= TRUE;
        }
        else if (!strcmp("USBDeviceSurpportedAudioSampleRate11025Hz", pParameterName)) {
            LOGD("USBDeviceSurpportedAudioSampleRate11025Hz : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate11025Hz= TRUE;
        }
        else if (!strcmp("USBDeviceSurpportedAudioSampleRate12000Hz", pParameterName)) {
            LOGD("USBDeviceSurpportedAudioSampleRate12000Hz : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate12000Hz= TRUE;
        }
        else if (!strcmp("USBDeviceSurpportedAudioSampleRate16000Hz", pParameterName)) {
            LOGD("USBDeviceSurpportedAudioSampleRate16000Hz : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate16000Hz= TRUE;
        }
        else if (!strcmp("USBDeviceSurpportedAudioSampleRate22050Hz", pParameterName)) {
            LOGD("USBDeviceSurpportedAudioSampleRate22050Hz : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate22050Hz= TRUE;
        }
        else if (!strcmp("USBDeviceSurpportedAudioSampleRate24000Hz", pParameterName)) {
            LOGD("USBDeviceSurpportedAudioSampleRate24000Hz : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate24000Hz= TRUE;
        }
        else if (!strcmp("USBDeviceSurpportedAudioSampleRate32000Hz", pParameterName)) {
            LOGD("USBDeviceSurpportedAudioSampleRate32000Hz : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate32000Hz= TRUE;
        }else if (!strcmp("USBDeviceSurpportedAudioSampleRate44100Hz", pParameterName)) {
            LOGD("USBDeviceSurpportedAudioSampleRate44100Hz : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate44100Hz= TRUE;
        }
        else if (!strcmp("USBDeviceSurpportedAudioSampleRate48000Hz", pParameterName)) {
            LOGD("USBDeviceSurpportedAudioSampleRate48000Hz : %s", pParameterValue);
            if(!strcmp("YES", pParameterValue))
                mIdentficationMsgSelect.USBDeviceSurpportedAudioSampleRate48000Hz= TRUE;
        }
        else if (!strcmp("USBHostTransportComponentName", pParameterName)) {
            mIdentficationMsgSelect.USBHostTransportComponentName.bPresent = TRUE;
            mIdentficationMsgSelect.USBHostTransportComponentName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.USBHostTransportComponentName.value = (uint8_t*)malloc(mIdentficationMsgSelect.USBHostTransportComponentName.size);
            if(  mIdentficationMsgSelect.USBHostTransportComponentName.value != NULL ) {
                memset(mIdentficationMsgSelect.USBHostTransportComponentName.value, 0x00, mIdentficationMsgSelect.USBHostTransportComponentName.size);
                strncpy((char*)mIdentficationMsgSelect.USBHostTransportComponentName.value, pParameterValue, strlen(pParameterValue));      
                LOGD("USBHostTransportComponentName : %s",mIdentficationMsgSelect.USBHostTransportComponentName.value);
            }
            else {
                LOGE ( "Failed malloc !! USBHostTransportComponentName " );
            }
        }
        else if (!strcmp("SerialTransportComponentName", pParameterName)) {
            mIdentficationMsgSelect.SerialTransportComponentName.bPresent = TRUE;
            mIdentficationMsgSelect.SerialTransportComponentName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.SerialTransportComponentName.value = (uint8_t*)malloc(mIdentficationMsgSelect.SerialTransportComponentName.size);
            if(  mIdentficationMsgSelect.SerialTransportComponentName.value != NULL ) {
                memset(mIdentficationMsgSelect.SerialTransportComponentName.value, 0x00, mIdentficationMsgSelect.SerialTransportComponentName.size);
                strncpy((char*)mIdentficationMsgSelect.SerialTransportComponentName.value, pParameterValue, strlen(pParameterValue));      
                LOGD("SerialTransportComponentName : %s",mIdentficationMsgSelect.SerialTransportComponentName.value);
            }
            else {
                LOGE ( "Failed malloc !! SerialTransportComponentName " );
            }
        }
        else if (!strcmp("BluetoothTransportComponentName", pParameterName)) {
            mIdentficationMsgSelect.BluetoothTransportComponentName.bPresent = TRUE;
            mIdentficationMsgSelect.BluetoothTransportComponentName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.BluetoothTransportComponentName.value = (uint8_t*)malloc(mIdentficationMsgSelect.BluetoothTransportComponentName.size);
            if(  mIdentficationMsgSelect.BluetoothTransportComponentName.value != NULL ) {
                memset(mIdentficationMsgSelect.BluetoothTransportComponentName.value, 0x00, mIdentficationMsgSelect.BluetoothTransportComponentName.size);
                strncpy((char*)mIdentficationMsgSelect.BluetoothTransportComponentName.value, pParameterValue, strlen(pParameterValue));      
                LOGD("BluetoothTransportComponentName : %s",mIdentficationMsgSelect.BluetoothTransportComponentName.value);
            }
            else {
                LOGE ( "Failed malloc !! BluetoothTransportComponentName " );
            }
        }
        else if (!strcmp("BluetoothTransportMediaAccessControlAddress", pParameterName)) {
            mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.bPresent = TRUE;
            mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.value = (uint8_t*)malloc(mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.size);
            if(  mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.value != NULL ) {
                memset(mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.value, 0x00, mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.size);
                strncpy((char*)mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.value, pParameterValue, strlen(pParameterValue));      
                LOGD("BluetoothTransportMediaAccessControlAddress : %s",mIdentficationMsgSelect.BluetoothTransportMediaAccessControlAddress.value);
            }
            else {
                LOGE ( "Failed malloc !! BluetoothTransportMediaAccessControlAddress " );
            }
        }
        else if (!strcmp("iAP2HIDComponentName", pParameterName)) {
            mIdentficationMsgSelect.iAP2HIDComponentName.bPresent = TRUE;
            mIdentficationMsgSelect.iAP2HIDComponentName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.iAP2HIDComponentName.value = (uint8_t*)malloc(mIdentficationMsgSelect.iAP2HIDComponentName.size);
            if(  mIdentficationMsgSelect.iAP2HIDComponentName.value != NULL ) {
                memset(mIdentficationMsgSelect.iAP2HIDComponentName.value, 0x00, mIdentficationMsgSelect.iAP2HIDComponentName.size);
                strncpy((char*)mIdentficationMsgSelect.iAP2HIDComponentName.value, pParameterValue, strlen(pParameterValue));      
                LOGD("iAP2HIDComponentName : %s",mIdentficationMsgSelect.iAP2HIDComponentName.value);
            }
            else {
                LOGE ( "Failed malloc !! iAP2HIDComponentName " );
            }
        }
        else if (!strcmp("iAP2HIDComponentFunction", pParameterName)) {
            LOGD("iAP2HIDComponentFunction : %s", pParameterValue);
            if(!strcmp("0", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= KEYBOARD;
            else if (!strcmp("1", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= MEDIAPLAYBACKREMOTE;
            else if (!strcmp("2", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= ASSISTIVETOUCHPOINTER;
            else if (!strcmp("3", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= RESERVED;
            else if (!strcmp("4", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= GAMEPAD;
            else if (!strcmp("6", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= GAMEPAD_NON_FORM_FITTING;
            else if (!strcmp("7", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= ASSISTIVESWITCHCONTROL;    
            else if (!strcmp("8", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= HEADSET;
            else
                LOGE("iAP2HIDComponentFunction unknown param : %s", pParameterValue);
        }
        else if (!strcmp("WirelessCarPlayTransPortComponentName", pParameterName)) {
            mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.bPresent = TRUE;
            mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.value = (uint8_t*)malloc(mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.size);
            if(  mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.value != NULL ) {
                memset(mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.value, 0x00, mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.size);
                strncpy((char*)mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.value, pParameterValue, strlen(pParameterValue));      
                LOGD("WirelessCarPlayTransPortComponentName : %s",mIdentficationMsgSelect.WirelessCarPlayTransPortComponentName.value);
            }
            else {
                LOGE ( "Failed malloc !! WirelessCarPlayTransPortComponentName " );
            }
        }
        else if (!strcmp("BluetoothHIDComponentName", pParameterName)) {
            mIdentficationMsgSelect.BluetoothHIDComponentName.bPresent = TRUE;
            mIdentficationMsgSelect.BluetoothHIDComponentName.size = strlen(pParameterValue) + 1;
            mIdentficationMsgSelect.BluetoothHIDComponentName.value = (uint8_t*)malloc(mIdentficationMsgSelect.BluetoothHIDComponentName.size);
            if(  mIdentficationMsgSelect.BluetoothHIDComponentName.value != NULL ) {
                memset(mIdentficationMsgSelect.BluetoothHIDComponentName.value, 0x00, mIdentficationMsgSelect.BluetoothHIDComponentName.size);
                strncpy((char*)mIdentficationMsgSelect.BluetoothHIDComponentName.value, pParameterValue, strlen(pParameterValue));      
                LOGD("BluetoothHIDComponentName : %s",mIdentficationMsgSelect.BluetoothHIDComponentName.value);
            }
            else {
                LOGE ( "Failed malloc !! BluetoothHIDComponentName " );
            }
        }
        else if (!strcmp("BluetoothHIDComponentFunction", pParameterName)) {
            LOGD("BluetoothHIDComponentFunction : %s", pParameterValue);
             if(!strcmp("0", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= KEYBOARD;
            else if (!strcmp("1", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= MEDIAPLAYBACKREMOTE;
            else if (!strcmp("2", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= ASSISTIVETOUCHPOINTER;
            else if (!strcmp("3", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= RESERVED;
            else if (!strcmp("4", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= GAMEPAD;
            else if (!strcmp("6", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= GAMEPAD_NON_FORM_FITTING;
            else if (!strcmp("7", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= ASSISTIVESWITCHCONTROL;    
            else if (!strcmp("8", pParameterValue))
                mIdentficationMsgSelect.iAP2HIDComponentFunction= HEADSET;
            else
                LOGE("BluetoothHIDComponentFunction unknown param : %s", pParameterValue);
        }
        else
            LOGE("UnKnown pParameterName : %s", pParameterName);
    }
}


void Identification::sendCancelIdentification(LinkController* pLnkCtrler, struct iAP2Link_st* link, uint8_t session)
{
    LOGD("sendCancelIdentification()");
    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;
    
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdCancelIdentification, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDaraArr((char*)encodedData, nEncodedDataLength);
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDaraArr);

    DataEncoder::deleteParamList(nParamIdList,paramDataList, nParamDataLenList );
    delete [] encodedData;

}

void Identification::handleIdentificationRejected(LinkController* pLnkCtrler, struct iAP2Link_st* link, int8_t session, uint8_t* pData)
{

    LOGD("handleIdentifiCationRejected() deprecated. pLnkCtrler[%p] session[%d] pData[%p]", session, pLnkCtrler, pData);

    int result = kIap2NoErr;
    
    IdentificationRejectedEvt event;
    memset(&event, 0x00, sizeof(IdentificationRejectedEvt));
    memset(&event.mRejectIdentficationMsgSelect, 0x01, sizeof(IdentficationMsgSelectArg));
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }
        
    
    decodeIdentifiCationRejected(pData, &event);
    pLnkCtrler->mCommunications.bLegacy = !(event.mRejectIdentficationMsgSelect.Communications_SendDTMFUse);
    LOGD("pLnkCtrler->mCommunications.bLegacy : %u" , pLnkCtrler->mCommunications.bLegacy);
    sendIdentificationInformation(pLnkCtrler, link,  session,  &event);
    
    if (result != kIap2NoErr)
        LOGE("    ERROR : handleIdentifiCationRejected(). result[%d]", result);
}

void Identification::decodeIdentifiCationRejected(uint8_t* pData, IdentificationRejectedEvt* pEvt)
{
    int msgLen;

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }
    

    msgLen = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);

    LOGD("decodeIdentifiCationRejected() nMsgLen[%d]", msgLen);

    int paramStartIdx = 6;
    int paramId, paramLength;
    int dataStart = 4;
    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
        switch (paramId) {
            case kParamIdIdentificationRecjected_Name : {
	            LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_Name");
                pEvt->Name = TRUE;
                break;
            }

            case kParamIdIdentificationRecjected_ModelIdentifier : {
		        LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_ModelIdentifier");
                pEvt->ModelIdentifier = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_Manufacturer : {
		        LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_Manufacturer");
                pEvt->Manufacturer = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_SerialNumber : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_SerialNumber");
                pEvt->SerialNumber = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_FirmwareVersion : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_FirmwareVersion");
                pEvt->FirmwareVersion = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_HardwareVersion : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_HardwareVersion");
                pEvt->HardwareVersion = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_MsgSentByAccessory : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_MsgSentByAccessory");
                pEvt->MessagesSentByAccessroy.bPresent = TRUE;
                pEvt->MessagesSentByAccessroy.size = paramLength-4;
                pEvt->MessagesSentByAccessroy.value = (uint8_t*)&pData[paramStartIdx];    
                parsingMessagesByAccessory(pEvt, pEvt->MessagesSentByAccessroy.value+dataStart, pEvt->MessagesSentByAccessroy.size);
                break;
            }
            
            case kParamIdIdentificationRecjected_MsgReceivedFromDevice : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_MsgReceivedFromDevice");
                pEvt->MessagesRecviedFromDevice.bPresent = TRUE;
                pEvt->MessagesRecviedFromDevice.size = paramLength-4;
                pEvt->MessagesRecviedFromDevice.value = (uint8_t*)&pData[paramStartIdx];
                parsingMessagesByAccessory(pEvt, pEvt->MessagesRecviedFromDevice.value+dataStart, pEvt->MessagesRecviedFromDevice.size);
                break;
            }
            
            case kParamIdIdentificationRecjected_PowerSourceType : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_PowerSourceType");
                pEvt->PowerProvidingCapability = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_MaximumCurrent : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_MaximumCurrent");
                pEvt->MaximumCurrentDrawnFromDevice = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_SupportedEAP : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_SupportedEAP");
                pEvt->SupportedExternalAccessoryProtocl = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_AppMatchTeamID : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_AppMatchTeamID");
                pEvt->AppMatchTeamID = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_CurrentLanguage : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_CurrentLanguage");
                pEvt->CurrentLanguage = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_SupportedLanguage : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_SupportedLanguage");
                pEvt->SupportedLanguage = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_SerialTransportComponent : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_SerialTransportComponent");
                pEvt->SerialTransportComponent = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_USBDeviceTransportComponent : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_USBDeviceTransportComponent");
                pEvt->USBDeviceTransportComponent = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_USBHostTransportComponent : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_USBHostTransportComponent");
                pEvt->USBHostTransportComponent = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_BluetoothTransportComponent : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_BluetoothTransportComponent");
                pEvt->BluetoothTransportComponent = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_iAP2HIDComponent : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_iAP2HIDComponent");
                pEvt->iAP2HIDComponent = TRUE;
                break;
            }
            
            case kParamIdIdentificationRecjected_VehicleInformationComponent : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_VehicleInformationComponent");
                pEvt->VehicleInformationComponent = TRUE;
                break;
            }

            case kParamIdIdentificationRecjected_VehicleStatusComponent : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_VehicleStatusComponent");
                pEvt->VehicleStatusComponent = TRUE;
                break;
            }

            case kParamIdIdentificationRecjected_LocationInformationComponent : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_LocationInformationComponent");
                pEvt->LocationInformationComponent = TRUE;
                break;
            }
                
            case kParamIdIdentificationRecjected_USBHostHIDComponent : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_USBHostHIDComponent");
                pEvt->USBHostHIDComponent = TRUE;
                break;
            }

            case kParamIdIdentificationRecjected_BluetoothHIDComponent : {
                LOGI("IdentificationInformation Reject Parm kParamIdIdentificationRecjected_BluetoothHIDComponent");
                pEvt->BluetoothHIDComponent = TRUE;
                break;
            }            
        }        
        paramStartIdx += paramLength;
    }    
}

void Identification::parsingMessagesByAccessory(IdentificationRejectedEvt* pEvt, uint8_t* pMessageIdentifier, uint32_t CountIdentifier)
{
    LOGD("parsingMessagesByAccessory");
    LOGD("CountIdentifier : %u", CountIdentifier / 2);
    uint16_t pUnsupportIdentifer = 0;
    uint32_t StartIndex = 0;
    
    while(CountIdentifier > StartIndex) 
    {
        pUnsupportIdentifer = DataDecoder::getUintFromBytes(pMessageIdentifier, StartIndex, 2);
        LOGD("pUnsupportIdentifer : %x", pUnsupportIdentifer);    
        switch (pUnsupportIdentifer) 
        {
            LOGD("pUnsupportIdentifer : %x", pUnsupportIdentifer);    
            case kMsgIdRequestAppLaunch : {    
                pEvt->mRejectIdentficationMsgSelect.AppLaunch= FALSE;
                break;
            }

            case kMsgIdStartAssistiveTouch : 
            case kMsgIdStopAssistiveTouch : 
            case kMsgIdStartAssistiveTouchInformation : 
            case kMsgIdAssistiveTouchInformation : 
            case kMsgIdStopAssistiveTouchInformation :     
            { 
                pEvt->mRejectIdentficationMsgSelect.AssistiveTouch = FALSE;
                break;
            }
            
            case kMsgIdBluetoothComponentInformation : 
            case kMsgIdStartBluetoothConnectionUpdates : 
            case kMsgIdBluetoothConnectionUpdate : 
            case kMsgIdStopBluetoothConnectionUpdates : 
            { 
                pEvt->mRejectIdentficationMsgSelect.BluetoothConnection = FALSE;
                break;
            }
            
            case kMsgIdRequestDeviceAuthenticationCertificate :
            case kMsgIdDeviceAuthenticationCertificate :
            case kMsgIdRequestDeviceAuthenticationChallengeResponse :                
            case kMsgIdDeviceAuthenticationResponse :                
            case kMsgIdDeviceAuthenticationFailed :
            case kMsgIdDeviceAuthenticationSucceeded :
            { 
                pEvt->mRejectIdentficationMsgSelect.DeviceAuthentication = FALSE;
                break;
            }

            case kMsgIdDeviceInformationUpdate :
            {
                pEvt->mRejectIdentficationMsgSelect.DeviceInformationUpdate = FALSE;    
                break;
            }
            case kMsgIdDeviceLanguageUpdate :
            { 
                pEvt->mRejectIdentficationMsgSelect.DeviceLanguageUpdate = FALSE;
                break;
            }

            case kMsgIdDeviceTimeUpdate :
            { 
                pEvt->mRejectIdentficationMsgSelect.DeviceTimeUpdate = FALSE;
                break;
            }

            case kMsgIdDeviceUUIDUpdate :
            { 
                pEvt->mRejectIdentficationMsgSelect.DeviceUUIDUpdate =FALSE;
                break;
            }

            case kMsgIdStartExternalAccessoryProtocolSession :
            case kMsgIdStopExternalAccessoryProtocolSession :
            case kMsgIdStatusExternalAccessoryProtocolSession :    
            { 
                pEvt->mRejectIdentficationMsgSelect.EAiAP2Session = FALSE;
                break;
            }

            case kMsgIdStartHID :
            case kMsgIdDeviceHIDReport :
            case kMsgIdAccessoryHIDReport :                
            case kMsgIdStopHID :
            { 
                pEvt->mRejectIdentficationMsgSelect.HIDiAP2SessionMediaPlaybackRemote = FALSE;
                break;
            }

            case kMsgIdStartLocationInformation :
            case kMsgIdLocationInformation :
            case kMsgIdStopLocationInformation :    
            { 
                pEvt->mRejectIdentficationMsgSelect.Location = FALSE;
                break;
            }

            case kMsgIdStartMediaLibraryInformation :
            case kMsgIdMediaLibraryInformation     :
            case kMsgIdStopMediaLibraryInformation :
            {
                pEvt->mRejectIdentficationMsgSelect.MediaLibraryInformation = FALSE;
                break;    
            }
            case kMsgIdStartMediaLibraryUpdates :                
            case kMsgIdMediaLibraryUpdate :
            case kMsgIdStopMediaLibraryUpdates :        
            { 
                pEvt->mRejectIdentficationMsgSelect.MediaLibraryUpdates = FALSE;
                break;
            }
            case kMsgIdPlayMediaLibraryCurrentSelection :
            case kMsgIdPlayMediaLibraryItems :    
            case kMsgIdPlayMediaLibraryCollection :
            case kMsgIdPlayMediaLibrarySpecial :
            {
                pEvt->mRejectIdentficationMsgSelect.MediaLibraryPlayback = FALSE;
                break;
            }

            case kMsgIdStartNowPlayingUpdates :
            case kMsgIdNowPlayingUpdates :
            case kMsgIdStopNowPlayingUpdates :    
            { 
                pEvt->mRejectIdentficationMsgSelect.NowPlaying = FALSE;
                break;
            }

            case kMsgIdStartPowerUpdates :
            case kMsgIdPowerUpdate :
            case kMsgIdStopPowerUpdates :    
            case kMsgIdPowerSourceUpdate :
            { 
                pEvt->mRejectIdentficationMsgSelect.Power = FALSE;
                break;
            }

            case kMsgIdStartUSBDeviceModeAudio :
            case kMsgIdUSBDeviceModeAudioInformation :
            case kMsgIdStopUSBDeviceModeAudio :
            { 
                pEvt->mRejectIdentficationMsgSelect.USBDigitalAudioDeviceMode = FALSE;
                break;
            }
/*
            case kMsgIdStartTelephonyCallStateInformation :
            case kMsgIdTelephonyCallStateInformation :
            case kMsgIdStopTelephonyCallStateInformation :                
            case kMsgIdStartTelephonyUpdates :                
            case kMsgIdTelephonyUpdate :
            case kMsgIdStopTelephonyUpdates :
            { 
                pEvt->mIdentficationMsgSelect.VehicleStatus = FALSE;
                break;
            }
*/
            case kMsgIdStartVehicleStatusUpdates :
            case kMsgIdVehicleStatusUpdate :
            case kMsgIdStopVehicleStatusUpdates :
            { 
                pEvt->mRejectIdentficationMsgSelect.VehicleStatus = FALSE;
                break;
            }

            case kMsgIdStartVoiceOver :
            case kMsgIdStopVoiceOver :
            case kMsgIdRequestVoiceOverMoveCursor :                
            case kMsgIdRequestVoiceOverActivateCursor :                
            case kMsgIdRequestVoiceOverScrollPage :
            case kMsgIdRequestVoiceOverSpeakText :
            case kMsgIdRequestVoiceOverPauseText :
            case kMsgIdRequestVoiceOverResumeText :
            case kMsgIdStartVoiceOverUpdates :
            case kMsgIdVoiceOverUpdate :
            case kMsgIdStopVoiceOverUpdates :
            case kMsgIdRequestVoiceOverConfiguration :    
            case kMsgIdStartVoiceOverCursorUpdates :    
            case kMsgIdVoiceOverCursorUpdate :    
            case kMsgIdStopVoiceOverCursorUpdates :                
            { 
                pEvt->mRejectIdentficationMsgSelect.VoiceOver = FALSE;
                break;
            }

            case kMsgIdRequestWiFiInformation :
            case kMsgIdWiFiInformation :                
            { 
                pEvt->mRejectIdentficationMsgSelect.WiFiInformationSharing = FALSE;
                break;
            }

            case kMsgIdStartCallStateUpdates :
            case kMsgIdCallStateUpdates :    
            case kMsgIdStopCallStateUpdates :
            {
                pEvt->mRejectIdentficationMsgSelect.Communications_CallStateUpdate = FALSE;
                break;
            }
            case kMsgIdStartCommunicationsUpdates :        
            case kMsgIdCommunicationsUpdates :        
            case kMsgIdStopCommunicationsUpdates :    
            { 
                pEvt->mRejectIdentficationMsgSelect.Communications_CommunicationsUpdate = FALSE;
                break;
            }
            
            case kMsgIdInitialCall :        
            case kMsgIdAcceptCall :        
            case kMsgIdEndCall :        
            case kMsgIdSwapCalls :        
            case kMsgIdMergeCalls :        
            case kMsgIdHoldStatusUpdate :        
            case kMsgIdMuteStatusUpdate :        
            case kMsgIdSendDTMF :
            { 
                pEvt->mRejectIdentficationMsgSelect.Communications_CallControls = FALSE;
                if (pUnsupportIdentifer == kMsgIdSendDTMF) {
                    pEvt->mRejectIdentficationMsgSelect.Communications_SendDTMFUse = FALSE;
                }
                break;
            }
            
            case kMsgIdStartListUpdate :    
            case kMsgIdListUpdate :    
            case kMsgIdStopListUpdate :
            { 
                pEvt->mRejectIdentficationMsgSelect.Communications_ListUpdates = FALSE;
                break;
            }

            default:
            {
                LOGD("    WARNING : unknowd Msg[0x%x]", pUnsupportIdentifer);
                break;
            }
        }
        StartIndex = StartIndex + 2;
    }
}

int Identification::handleIdentificationFailedUpdateEvent(LinkController* pLnkCtrler)
{
    int result = kIap2NoErr;

    //LOGD("handleIdentificationFailedUpdateEvent()");

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*1);
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*1, sizeof(EventHeader));

    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtIdentificationFailed;
    mEventHeader->dataSize = byteNum;    

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(/*session*/0, pIpcData, byteNum);
    }
    else {
        LOGD("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGW("    WARNING : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}

void Identification::createCurrentSupportLanguage(IdentificationRejectedEvt* pEvent, CmdSaveUtf8Param supportedLanguage ,std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList)
{  
    char* pPosition = NULL;
    uint32_t CountsupportedLanaguage = 0;
    char* tmpCurrentSupportedLanaguage = NULL;  
        
    if ((supportedLanguage.bPresent == TRUE) && (supportedLanguage.value != NULL) && (pEvent->SupportedLanguage == FALSE)) {
        tmpCurrentSupportedLanaguage = (char*)malloc(strlen((char*)mIdentficationMsgSelect.supportedLanaguage.value)+1);
        memset(tmpCurrentSupportedLanaguage, 0x00, strlen((char*)mIdentficationMsgSelect.supportedLanaguage.value)+1);
        LOGD("createCurrentSupportLanguage supportedLanguage->value : %s", supportedLanguage.value );
        strcpy(tmpCurrentSupportedLanaguage, (char*)supportedLanguage.value);
        LOGD("tmpCurrentSupportedLanaguage : %s", tmpCurrentSupportedLanaguage );
        pPosition = strtok( (char*)tmpCurrentSupportedLanaguage, " ");            
        if( pPosition == NULL ) {
            DataEncoder::appendStringParam("ko", kParamIdIdentification_SupportedLanguage, nParamIdList, paramDataList, nParamDataLenList);  
            DataEncoder::appendStringParam("en", kParamIdIdentification_SupportedLanguage, nParamIdList, paramDataList, nParamDataLenList);
        }
        else {
            DataEncoder::appendStringParam(pPosition, kParamIdIdentification_SupportedLanguage, nParamIdList, paramDataList, nParamDataLenList);  
        }
        while( (pPosition = strtok( NULL, " ")) )
        {   
            CountsupportedLanaguage++;
            DataEncoder::appendStringParam(pPosition, kParamIdIdentification_SupportedLanguage, nParamIdList, paramDataList, nParamDataLenList);               
        }
        CountsupportedLanaguage++;
        LOGD("CountsupportedLanaguage : %u" ,CountsupportedLanaguage);
    }
    else {
        LOGW("Warning supportedLanguage is NULL, set basic support language en, ko ");
        DataEncoder::appendStringParam("ko", kParamIdIdentification_SupportedLanguage, nParamIdList, paramDataList, nParamDataLenList);  
        DataEncoder::appendStringParam("en", kParamIdIdentification_SupportedLanguage, nParamIdList, paramDataList, nParamDataLenList);
    }

    free(tmpCurrentSupportedLanaguage);
}
}
// namespace IAP2
