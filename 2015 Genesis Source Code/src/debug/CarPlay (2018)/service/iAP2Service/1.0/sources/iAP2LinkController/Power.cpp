/*
    File:       Power.cpp
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

#include <vector>

#include "Power.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

Power::Power()
{
    LOGD("Power()");
}

Power::~Power()
{
    LOGD("~Power()");
}

void Power::handlePowerUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    PowerUpdateEvt event;

    LOGD("handlePowerUpdate() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodePowerUpdates(pData, &event);
    
    result = handlePowerUpdateEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handlePowerUpdate(). result[%d]", result);
}

void Power::decodePowerUpdates(uint8_t* pData, PowerUpdateEvt* pEvt)
{
    int msgLen;

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    if (pEvt == NULL) {
        LOGE("    Error : pEvt is NULL");
        return;
    }

    msgLen = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);

    int paramStartIdx = 6;
    int paramId;

    while (msgLen > paramStartIdx) {
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
        paramStartIdx += 4;

        switch (paramId) {
            case kParamIdPowerMaximumCurrentDrawnFromAccessory : {
                pEvt->maxCurrent.bPresent = TRUE;
                pEvt->maxCurrent.value = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
                paramStartIdx += 2;
                break;
            }

            case kParamIdPowerDeviceBatteryWillChargeIfPowerIsPresent : {
                pEvt->devBatteryCharge.bPresent = TRUE;
                pEvt->devBatteryCharge.value = DataDecoder::getUintFromBytes(pData, paramStartIdx, 1);
                paramStartIdx++;            
                break;
            }

            case kParamIdPowerAccessoryPowerMode : {
                pEvt->accPowerMode.bPresent = TRUE;
                pEvt->accPowerMode.value = (AccessoryPowerModes)DataDecoder::getUintFromBytes(pData, paramStartIdx, 1);                
                paramStartIdx++;
                break;
            }

            case kParamIdIsExternalChargerConnected : {
                pEvt->IsExternalChargerConnected = (BOOL)DataDecoder::getUintFromBytes(pData, paramStartIdx, 1);                
                paramStartIdx++;
                break;
            }

            case kParamIdBatteryChargingState : {
                pEvt->BatteryChargingState.bPresent = TRUE;
                pEvt->BatteryChargingState.value = (BatteryChargingState)DataDecoder::getUintFromBytes(pData, paramStartIdx, 1);
                paramStartIdx++;
                break;
            }

            case kParamIdBatteryChargeLevel : {
                pEvt->BatteryChargeLevel.bPresent = TRUE;
                pEvt->BatteryChargeLevel.value = (uint16_t)DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);                
                paramStartIdx += 2;
                break;
            }

        }

    }
}


int Power::handlePowerUpdateEvent(LinkController* pLnkCtrler, int8_t session, PowerUpdateEvt* pEvent)
{
    int result = kIap2NoErr;

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return kIap2ParamErr;
    }
    
    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*12);
    
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    EventHeader* mEventHeader = (EventHeader*) pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*12, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->maxCurrent.bPresent);
    if (pEvent->maxCurrent.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->maxCurrent.value);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->devBatteryCharge.bPresent);
    if (pEvent->devBatteryCharge.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->devBatteryCharge.value);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->accPowerMode.bPresent);
    if (pEvent->accPowerMode.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->accPowerMode.value);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->IsExternalChargerConnected);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->BatteryChargingState.bPresent);
    if (pEvent->BatteryChargingState.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->BatteryChargingState.value);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->BatteryChargeLevel.bPresent);
    if (pEvent->BatteryChargeLevel.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->BatteryChargeLevel.value);
    
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtPowerUpdate;
    mEventHeader->dataSize = byteNum;

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(session, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSendEventToApp is NULL");
    }

    if (result != kIap2NoErr) {
        LOGE("    ERROR : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}

void Power::sendStartPowerUpdates(LinkController* pLnkCtrler, StartPowerUpdateArg* pArg)
{
        LOGD("sendStartPowerUpdates()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE

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

    createStartPowerUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartPowerUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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

void Power::sendPowerSourceUpdate(LinkController* pLnkCtrler, PowerSourceUpdateArg* pArg)
{
    LOGD("sendPowerSourceUpdate()");
    
    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE

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

    createPowerSourceUpdateParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdPowerSourceUpdate,
                          nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDaraArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }        
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDaraArr);

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}

void Power::createStartPowerUpdatesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StartPowerUpdateArg* pArg)
{
#ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

#ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
    if (pArg->bMaxCurrentDrawnFromAcc == TRUE)
        DataEncoder::appendNoneParam(kParamIdPowerMaximumCurrentDrawnFromAccessory, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->bDevBatteryCharge == TRUE)
        DataEncoder::appendNoneParam(kParamIdPowerDeviceBatteryWillChargeIfPowerIsPresent, nParamIdList, paramDataList, nParamDataLenList);

    // In general, below parameter is not used.
    if (pArg->bAccPowerMode == TRUE)
        DataEncoder::appendNoneParam(kParamIdPowerAccessoryPowerMode, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->bIsExternalChargerConnected == TRUE)
        DataEncoder::appendNoneParam(kParamIdIsExternalChargerConnected, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->bBatteryChargingState == TRUE)
        DataEncoder::appendNoneParam(kParamIdBatteryChargingState, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->bBatteryChargeLevel == TRUE)
        DataEncoder::appendNoneParam(kParamIdBatteryChargeLevel, nParamIdList, paramDataList, nParamDataLenList);
    
#else // #ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
    DataEncoder::appendNoneParam(kParamIdPowerMaximumCurrentDrawnFromAccessory, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kParamIdPowerDeviceBatteryWillChargeIfPowerIsPresent, nParamIdList, paramDataList, nParamDataLenList);
    //DataEncoder::appendNoneParam(kParamIdPowerAccessoryPowerMode, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kParamIdIsExternalChargerConnected, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kParamIdBatteryChargingState, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kParamIdBatteryChargeLevel, nParamIdList, paramDataList, nParamDataLenList);
#endif // #ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
}

void Power::createPowerSourceUpdateParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, PowerSourceUpdateArg* pArg)
{
#ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

#ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
    if (pArg->availCurrent.bPresent == TRUE)
        DataEncoder::appendNumericParam(pArg->availCurrent.value, 2, kParamIdPowerAvailableCurrentForDevice, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->devBatteryCharge.bPresent == TRUE)
        DataEncoder::appendNumericParam(pArg->devBatteryCharge.value, 1, kParamIdPowerDeviceBatteryShouldChargeIfPowerIsPresent, nParamIdList, paramDataList, nParamDataLenList);
#else // #ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
    DataEncoder::appendNumericParam(1000, 2, kParamIdPowerAvailableCurrentForDevice, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNumericParam(1, 1, kParamIdPowerDeviceBatteryShouldChargeIfPowerIsPresent, nParamIdList, paramDataList, nParamDataLenList);
#endif // #ifndef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
}


} // namespace IAP2
