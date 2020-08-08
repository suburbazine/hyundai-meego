/*
    File:       Telephony.cpp
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

#include "Telephony.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>


namespace IAP2
{

Telephony::Telephony()
{
    LOGD("Telephony()");
}

Telephony::~Telephony()
{
    LOGD("~Telephony()");
}




void Telephony::handleTelephonyCallStateInformation(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    TelephonyCallStateInformationEvt event;

    LOGD("handleTelephonyCallStateInformation() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeTelephonyCallStateInformation(pData, &event);
    
        result = handleTelephonyCallStateInformationEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleTelephonyUpdate(). result[%d]", result);
    
}

void Telephony::handleTelephonyUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    TelephonyUpdateEvt event;

    LOGD("handleTelephonyUpdate() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeTelephonyUpdate(pData, &event);
    
        result = handleTelephonyUpdateEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleTelephonyUpdate(). result[%d]", result);
    
}


void Telephony::decodeTelephonyCallStateInformation(uint8_t* pData, TelephonyCallStateInformationEvt* pEvt)
{
    int msgLen;

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pEvt == NULL) {
        LOGE("    Error : pEvt is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    msgLen = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);

    LOGD("decodeTelephonyCallStateInformation() nMsgLen[%d]", msgLen);

    int paramStartIdx = 6;
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
        paramStartIdx += 4;

        switch (paramId) {
            case kParamIdCallStatePhoneNumber : {
                pEvt->callStatePhoneNum.bPresent = TRUE;
                        pEvt->callStatePhoneNum.size = paramLength-4;
                pEvt->callStatePhoneNum.value = (uint8_t*)&pData[paramStartIdx];
                
                LOGD("kParamIdCallStatePhoneNumber() bPresent[%d] :: size[%d] :: value[%s]", pEvt->callStatePhoneNum.bPresent, pEvt->callStatePhoneNum.size, pEvt->callStatePhoneNum.value);                
                paramStartIdx = paramStartIdx + pEvt->callStatePhoneNum.size;
                break;
                }

            case kParamIdCallStateNetSentCallerNumber : {
                pEvt->callStateNetSentCallerName.bPresent = TRUE;
                        pEvt->callStateNetSentCallerName.size = paramLength-4;
                pEvt->callStateNetSentCallerName.value = (uint8_t*)&pData[paramStartIdx];
                
                LOGD("kParamIdCallStateNetSentCallerNumber() bPresent[%d] :: size[%d] :: value[%s]", pEvt->callStateNetSentCallerName.bPresent, pEvt->callStateNetSentCallerName.size, pEvt->callStateNetSentCallerName.value);
                paramStartIdx = paramStartIdx + pEvt->callStateNetSentCallerName.size;
                break;
                }
                
            case kParamIdCallStateStatus : {
                pEvt->callStateStatus.bPresent = TRUE;
                pEvt->callStateStatus.value = DataDecoder::getUintFromBytes(pData, paramStartIdx, 1);
                
                LOGD("kParamIdCallStateStatus() bPresent[%d] :: value[%d]", pEvt->callStateStatus.bPresent, pEvt->callStateStatus.value);
                paramStartIdx++;
                break;
                }

            case kParamIdCallStateDirection : {
                pEvt->callStateDirection.bPresent = TRUE;
                pEvt->callStateDirection.value = DataDecoder::getUintFromBytes(pData, paramStartIdx, 1);
                
                LOGD("kParamIdCallStateDirection() bPresent[%d] :: value[%d]", pEvt->callStateDirection.bPresent, pEvt->callStateDirection.value);
                paramStartIdx++;
                break;
                }

            case kParamIdUniqueCallID : {
                pEvt->uniqueCallID.bPresent = TRUE;
                        pEvt->uniqueCallID.size = paramLength-4;
                pEvt->uniqueCallID.value = (uint8_t*)&pData[paramStartIdx];
                
                LOGD("kParamIdUniqueCallID() bPresent[%d] :: size[%d] :: value[%s]", pEvt->uniqueCallID.bPresent, pEvt->uniqueCallID.size, pEvt->uniqueCallID.value);
                paramStartIdx = paramStartIdx + pEvt->uniqueCallID.size;
                break;
                }

            case kParamIdCallStatevCardFTID : {
                pEvt->callStatevCardFTID.bPresent = TRUE;
                pEvt->callStatevCardFTID.value = DataDecoder::getUintFromBytes(pData, paramStartIdx, 1);
                
                LOGD("kParamIdCallStatevCardFTID() bPresent[%d] :: value[%d]", pEvt->callStatevCardFTID.bPresent, pEvt->callStatevCardFTID.value);
                paramStartIdx++;
                break;
                }        

            default : 
                break;
        }

    }
}

void Telephony::decodeTelephonyUpdate(uint8_t* pData, TelephonyUpdateEvt* pEvt)
{
    int msgLen;

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pEvt == NULL) {
        LOGE("    Error : pEvt is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    msgLen = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);

    LOGD("decodeTelephonyUpdate() nMsgLen[%d]", msgLen);

    int paramStartIdx = 6;
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
        paramStartIdx += 4;

        switch (paramId) {
            case kParamIdTelephonySignalStrength : {
                pEvt->telephonySignalStrength.bPresent = TRUE;
                pEvt->telephonySignalStrength.value = DataDecoder::getUintFromBytes(pData, paramStartIdx, 1);
                
                LOGD("kParamIdTelephonySignalStrength() bPresent[%d] :: value[%d]", pEvt->telephonySignalStrength.bPresent, pEvt->telephonySignalStrength.value);
                paramStartIdx++;
                break;
                }

            case kParamIdTelephonyRegisStatus : {                
                pEvt->telephonyRegisStatus.bPresent = TRUE;
                 pEvt->telephonyRegisStatus.value = DataDecoder::getUintFromBytes(pData, paramStartIdx, 1);
                 
                LOGD("kParamIdTelephonyRegisStatus() bPresent[%d] :: value[%d]", pEvt->telephonyRegisStatus.bPresent, pEvt->telephonyRegisStatus.value);
                paramStartIdx++;
                break;
                }
                
            case kParamIdTelephonyAirplaneModeStatus : {
                pEvt->telephonyAirPlaneModeStatus.bPresent = TRUE;
                pEvt->telephonyAirPlaneModeStatus.value = (BOOL)DataDecoder::getUintFromBytes(pData, paramStartIdx, 1);
                
                LOGD("kParamIdTelephonyAirplaneModeStatus() bPresent[%d] :: value[%d]", pEvt->telephonyAirPlaneModeStatus.bPresent, pEvt->telephonyAirPlaneModeStatus.value);
                paramStartIdx++;
                break;
                }

            case kParamIdTelephonyTTYStatus : {
                pEvt->telephonyTTYStatus.bPresent = TRUE;
                pEvt->telephonyTTYStatus.value = (BOOL)DataDecoder::getUintFromBytes(pData, paramStartIdx, 1);
                
                LOGD("kParamIdTelephonyTTYStatus() bPresent[%d] :: value[%d]", pEvt->telephonyTTYStatus.bPresent, pEvt->telephonyTTYStatus.value);
                paramStartIdx++;
                break;
                }

            case kParamIdMobileOperator : {
                pEvt->mobileOperator.bPresent = TRUE;
                        pEvt->mobileOperator.size = paramLength-4;
                pEvt->mobileOperator.value = (uint8_t*)&pData[paramStartIdx];
                
                LOGD("kParamIdMobileOperator() bPresent[%d] :: size[%d] :: value[%s]", pEvt->mobileOperator.bPresent, pEvt->mobileOperator.size, pEvt->mobileOperator.value);
                paramStartIdx = paramStartIdx + pEvt->mobileOperator.size;
                break;
                }

            default : 
                break;
        }

    }
}




int Telephony::handleTelephonyCallStateInformationEvent(LinkController* pLnkCtrler, int8_t session, TelephonyCallStateInformationEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleTelephonyCallStateInformationEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(TelephonyCallStateInformationEvt) + pEvent->callStatePhoneNum.size + pEvent->callStateNetSentCallerName.size + pEvent->uniqueCallID.size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->callStatePhoneNum.bPresent);
    if (pEvent->callStatePhoneNum.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->callStatePhoneNum.value, pEvent->callStatePhoneNum.size);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->callStateNetSentCallerName.bPresent);
    if (pEvent->callStateNetSentCallerName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->callStateNetSentCallerName.value, pEvent->callStateNetSentCallerName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->callStateStatus.bPresent);
    if (pEvent->callStateStatus.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->callStateStatus.value);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->callStateDirection.bPresent);
    if (pEvent->callStateDirection.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->callStateDirection.value);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->uniqueCallID.bPresent);
    if (pEvent->uniqueCallID.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->uniqueCallID.value, pEvent->uniqueCallID.size);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->callStatevCardFTID.bPresent);
    if (pEvent->callStatevCardFTID.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->callStatevCardFTID.value);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    mEventHeader->evt = kEvtTelephonyCallStateInformation;
    mEventHeader->dataSize = byteNum;
    
//    LOGD("kParamIdCallStatePhoneNumber() bPresent[%d] :: size[%d] :: value[%s]", pEvent->callStatePhoneNum.bPresent, pEvent->callStatePhoneNum.size, pEvent->callStatePhoneNum.value);
//    LOGD("kParamIdCallStateNetSentCallerNumber() bPresent[%d] :: size[%d] :: value[%s]", pEvent->callStateNetSentCallerName.bPresent, pEvent->callStateNetSentCallerName.size, pEvent->callStateNetSentCallerName.value);
//    LOGD("kParamIdCallStateStatus() bPresent[%d] :: value[%d]", pEvent->callStateStatus.bPresent, pEvent->callStateStatus.value);
//    LOGD("kParamIdCallStateDirection() bPresent[%d] :: value[%d]", pEvent->callStateDirection.bPresent, pEvent->callStateDirection.value);
//    LOGD("kParamIdUniqueCallID() bPresent[%d] :: size[%d] :: value[%s]", pEvent->uniqueCallID.bPresent, pEvent->uniqueCallID.size, pEvent->uniqueCallID.value);
//    LOGD("kParamIdCallStatevCardFTID() bPresent[%d] :: value[%d]", pEvent->callStatevCardFTID.bPresent, pEvent->callStatevCardFTID.value);

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(session, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGE("    ERROR : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}

int Telephony::handleTelephonyUpdateEvent(LinkController* pLnkCtrler, int8_t session, TelephonyUpdateEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleTelephonyUpdateEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(TelephonyUpdateEvt) + pEvent->mobileOperator.size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->telephonySignalStrength.bPresent);
    if (pEvent->telephonySignalStrength.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->telephonySignalStrength.value);

//    LOGD("kParamIdTelephonySignalStrength() bPresent[%d] :: value[%d]", pEvent->telephonySignalStrength.bPresent, pEvent->telephonySignalStrength.value);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->telephonyRegisStatus.bPresent);
    if (pEvent->telephonyRegisStatus.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->telephonyRegisStatus.value);

//    LOGD("kParamIdTelephonyRegisStatus() bPresent[%d] :: value[%d]", pEvent->telephonyRegisStatus.bPresent, pEvent->telephonyRegisStatus.value);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->telephonyAirPlaneModeStatus.bPresent);
    if (pEvent->telephonyAirPlaneModeStatus.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->telephonyAirPlaneModeStatus.value);

//    LOGD("kParamIdTelephonyAirplaneModeStatus() bPresent[%d] :: value[%d]", pEvent->telephonyAirPlaneModeStatus.bPresent, pEvent->telephonyAirPlaneModeStatus.value);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->telephonyTTYStatus.bPresent);
    if (pEvent->telephonyTTYStatus.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->telephonyTTYStatus.value);

//    LOGD("kParamIdTelephonyTTYStatus() bPresent[%d] :: value[%d]", pEvent->telephonyTTYStatus.bPresent, pEvent->telephonyTTYStatus.value);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mobileOperator.bPresent);
    if (pEvent->mobileOperator.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->mobileOperator.value, pEvent->mobileOperator.size);
    
//    LOGD("kParamIdMobileOperator() bPresent[%d] :: size[%d] :: value[%s]", pEvent->mobileOperator.bPresent, pEvent->mobileOperator.size, pEvent->mobileOperator.value);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtTelephonyUpdate;
    mEventHeader->dataSize = byteNum;

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(session, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGE("    ERROR : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}




} // namespace IAP2

