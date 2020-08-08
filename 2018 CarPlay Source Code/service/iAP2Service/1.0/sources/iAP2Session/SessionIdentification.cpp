/*
    File:       SessionIdentification.cpp
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


#include "SessionIdentification.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

namespace IAP2
{

SessionIdentification::SessionIdentification()
{
    LOGD("SessionIdentification()");
}

SessionIdentification::~SessionIdentification()
{
    LOGD("~SessionIdentification()");
}

int SessionIdentification::handleIdentificationCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    LOGD("handleIdentificationCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);
    int result = kIap2NoErr;

    if (pRxData == NULL) {
        LOGE("    ERROR : pRxData is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR : pLnkCtrler is NULL");
        return kIap2ParamErr;
    }

    switch(cmd) {
        case kCmdIdentificationUpdate : {
            IdentificationUpdateArg arg;
            memset(&arg, 0x00, sizeof(IdentificationUpdateArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            arg.name.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.name.bPresent == TRUE) {
                arg.name.value =  iAp2MarshalGetBytes(&iAp2Marshal, &arg.name.size);
            }

            arg.modelIdentifier.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.modelIdentifier.bPresent == TRUE) {
                arg.modelIdentifier.value =  iAp2MarshalGetBytes(&iAp2Marshal, &arg.modelIdentifier.size);
            }

            arg.modelIdentifier.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.modelIdentifier.bPresent == TRUE) {
                arg.manufacturer.value =  iAp2MarshalGetBytes(&iAp2Marshal, &arg.manufacturer.size);
            }

            arg.serialNumber.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.serialNumber.bPresent == TRUE) {
                arg.serialNumber.value =  iAp2MarshalGetBytes(&iAp2Marshal, &arg.serialNumber.size);
            }

            arg.firmwareVersion.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.firmwareVersion.bPresent == TRUE) {
                arg.firmwareVersion.value =  iAp2MarshalGetBytes(&iAp2Marshal, &arg.firmwareVersion.size);
            }

            arg.hardwareVersion.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.hardwareVersion.bPresent == TRUE) {
                arg.hardwareVersion.value =  iAp2MarshalGetBytes(&iAp2Marshal, &arg.hardwareVersion.size);
            }

            arg.currentLanguage.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.currentLanguage.bPresent == TRUE) {
                arg.currentLanguage.value =  iAp2MarshalGetBytes(&iAp2Marshal, &arg.currentLanguage.size);
            }      
                        
            if(arg.name.value != NULL && (strcmp((const char*)arg.name.value, "") != 0)) {
                memset(pLnkCtrler->mIdentification.mIdentificationInformation.Name, 0x00, sizeof(pLnkCtrler->mIdentification.mIdentificationInformation.Name));
                strcpy(pLnkCtrler->mIdentification.mIdentificationInformation.Name, (const char*) arg.name.value);
            }
            else {
                LOGW("arg.name.value is NULL");
                arg.name.value = (uint8_t*)pLnkCtrler->mIdentification.mIdentificationInformation.Name;
            }
                
            if(arg.modelIdentifier.value != NULL && (strcmp((const char*)arg.modelIdentifier.value, "") != 0)) {
                memset(pLnkCtrler->mIdentification.mIdentificationInformation.ModelIdentifier, 0x00, sizeof(pLnkCtrler->mIdentification.mIdentificationInformation.ModelIdentifier));
                strcpy(pLnkCtrler->mIdentification.mIdentificationInformation.ModelIdentifier,(const char*) arg.modelIdentifier.value);
            }
            else {
                LOGW("arg.modelIdentifier.value is NULL");
                arg.modelIdentifier.value = (uint8_t*)pLnkCtrler->mIdentification.mIdentificationInformation.ModelIdentifier;
            }
                
            if(arg.manufacturer.value != NULL && (strcmp((const char*)arg.manufacturer.value, "") != 0)){
                memset(pLnkCtrler->mIdentification.mIdentificationInformation.Manufacturer, 0x00, sizeof(pLnkCtrler->mIdentification.mIdentificationInformation.Manufacturer));
                strcpy(pLnkCtrler->mIdentification.mIdentificationInformation.Manufacturer, (const char*) arg.manufacturer.value);
            }
            else {
                LOGW("arg.manufacturer.value is NULL");
                arg.manufacturer.value = (uint8_t*)pLnkCtrler->mIdentification.mIdentificationInformation.Manufacturer;
            }

            if(arg.serialNumber.value != NULL && (strcmp((const char*)arg.serialNumber.value, "") != 0)){
                memset(pLnkCtrler->mIdentification.mIdentificationInformation.SerialNumber, 0x00, sizeof(pLnkCtrler->mIdentification.mIdentificationInformation.SerialNumber));
                strcpy(pLnkCtrler->mIdentification.mIdentificationInformation.SerialNumber, (const char*) arg.serialNumber.value);
            }
            else {
                LOGW("arg.serialNumber.value is NULL");
                arg.serialNumber.value = (uint8_t*)pLnkCtrler->mIdentification.mIdentificationInformation.SerialNumber;
            }
    
            if(arg.firmwareVersion.value != NULL && (strcmp((const char*)arg.firmwareVersion.value, "") != 0)){
                memset(pLnkCtrler->mIdentification.mIdentificationInformation.FirmwareVersion, 0x00, sizeof(pLnkCtrler->mIdentification.mIdentificationInformation.FirmwareVersion));
                strcpy(pLnkCtrler->mIdentification.mIdentificationInformation.FirmwareVersion, (const char*) arg.firmwareVersion.value);
            }
            else {
                LOGW("arg.firmwareVersion.value is NULL");
                arg.firmwareVersion.value = (uint8_t*)pLnkCtrler->mIdentification.mIdentificationInformation.FirmwareVersion;
            }

            if(arg.hardwareVersion.value != NULL && (strcmp((const char*)arg.hardwareVersion.value, "") != 0)){
                memset(pLnkCtrler->mIdentification.mIdentificationInformation.HardwareVersion, 0x00, sizeof(pLnkCtrler->mIdentification.mIdentificationInformation.HardwareVersion));
                strcpy(pLnkCtrler->mIdentification.mIdentificationInformation.HardwareVersion, (const char*) arg.hardwareVersion.value);
            }
            else {
                LOGW("arg.hardwareVersion.value is NULL");
                arg.hardwareVersion.value = (uint8_t*)pLnkCtrler->mIdentification.mIdentificationInformation.HardwareVersion;
            }

            if(arg.currentLanguage.value != NULL && (strcmp((const char*)arg.currentLanguage.value, "") != 0)){
                memset(pLnkCtrler->mIdentification.mIdentificationInformation.CurrentLanguage, 0x00, sizeof(pLnkCtrler->mIdentification.mIdentificationInformation.CurrentLanguage));
                strcpy(pLnkCtrler->mIdentification.mIdentificationInformation.CurrentLanguage, (const char*) arg.currentLanguage.value);
            }
            else {
                LOGW("arg.currentLanguage.value is NULL");
                arg.currentLanguage.value = (uint8_t*)pLnkCtrler->mIdentification.mIdentificationInformation.CurrentLanguage;
            }
            
            LOGD("    ProductName : %s ", pLnkCtrler->mIdentification.mIdentificationInformation.Name);
            LOGD("    ModelIdentifier : %s ", pLnkCtrler->mIdentification.mIdentificationInformation.ModelIdentifier);
            LOGD("    Manufacturer : %s ", pLnkCtrler->mIdentification.mIdentificationInformation.Manufacturer);
            LOGD("    SerialNumber : %s ", pLnkCtrler->mIdentification.mIdentificationInformation.SerialNumber);
            LOGD("    FirmwareVersion : %s ", pLnkCtrler->mIdentification.mIdentificationInformation.FirmwareVersion);
            LOGD("    HardwareVersion : %s ", pLnkCtrler->mIdentification.mIdentificationInformation.HardwareVersion);
            LOGD("    CurrentLanguage : %s ", pLnkCtrler->mIdentification.mIdentificationInformation.CurrentLanguage);
            
            if(pLnkCtrler->mConnection.mConnState >= CS_POWER_UPDATED)
                sendIdentificationUpdate(pLnkCtrler, &arg);
            else
                LOGD("Don't send IdentificationUpdae because state is not power update");
            break;
        }

        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}

void SessionIdentification::sendIdentificationUpdate(LinkController* pLnkCtrler, IdentificationUpdateArg* pArg)
{
    LOGD("sendIdentificationUpdate()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;

    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    createIdentificationUpdateParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdIdentificationInformationUpdate, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        


    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionIdentification::createIdentificationUpdateParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, IdentificationUpdateArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendBytesParam(pArg->name.value, pArg->name.size, kParamIdIdentificationInformationUpdate_Name, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendBytesParam(pArg->modelIdentifier.value, pArg->modelIdentifier.size, kParamIdIdentificationInformationUpdate_ModelIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendBytesParam(pArg->manufacturer.value, pArg->manufacturer.size, kParamIdIdentificationInformationUpdate_Manufacturer, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendBytesParam(pArg->serialNumber.value, pArg->serialNumber.size, kParamIdIdentificationInformationUpdate_SerialNumber, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendBytesParam(pArg->firmwareVersion.value, pArg->firmwareVersion.size, kParamIdIdentificationInformationUpdate_FirmwareVersion, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendBytesParam(pArg->hardwareVersion.value, pArg->hardwareVersion.size, kParamIdIdentificationInformationUpdate_HardwareVersion, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendBytesParam(pArg->currentLanguage.value, pArg->currentLanguage.size, kParamIdIdentificationInformationUpdate_CurrentLanguage, nParamIdList, paramDataList, nParamDataLenList);
    

}


}// namespace IAP2
