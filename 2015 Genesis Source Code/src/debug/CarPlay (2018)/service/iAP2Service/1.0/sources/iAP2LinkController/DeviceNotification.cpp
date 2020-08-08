/*
    File:       DeviceNotification.cpp
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

#include "DeviceNotification.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

DeviceNotification::DeviceNotification()
{
    LOGD("DeviceNotification()");
}

DeviceNotification::~DeviceNotification()
{
    LOGD("~DeviceNotification()");
}


void DeviceNotification::handleDeviceInformationUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    DeviceInformationUpdateEvt event;

    LOGD("handleDeviceInformationUpdate() session[%d]");
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeDeviceInformationUpdates(pData, &event);
    
        result = handleDeviceInformationUpdateEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleDeviceInformationUpdate(). result[%d]", result);
    

}

void DeviceNotification::handleDeviceLanguageUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    DeviceLanguageUpdateEvt event;

    LOGD("handleDeviceLanguageUpdate() session[%d]");
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeDeviceLanguageUpdates(pData, &event);
    
    result = handleDeviceLanguageUpdateEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleDeviceLanguageUpdate(). result[%d]", result);
    

}



void DeviceNotification::decodeDeviceInformationUpdates(uint8_t* pData, DeviceInformationUpdateEvt* pEvt)
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
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
            paramStartIdx += 4;

        switch (paramId) {
            case kParamIdDeviceName : {
                pEvt->bPresent = TRUE;
                pEvt->size = paramLength-4;
                pEvt->value = (uint8_t*)&pData[paramStartIdx];
                LOGD("decodeDeviceInformationUpdates() bPresent[%d] :: size[%d] :: value[%s]", pEvt->bPresent, pEvt->size, pEvt->value);
                paramStartIdx = paramStartIdx + pEvt->size;
                break;
            }

            default : 
                break;
        }

    }
}

void DeviceNotification::decodeDeviceLanguageUpdates(uint8_t* pData, DeviceLanguageUpdateEvt* pEvt)
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
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
            paramStartIdx += 4;

        switch (paramId) {
            case kParamIdDeviceLanguage : {
                pEvt->bPresent = TRUE;
                pEvt->size = paramLength-4;
                pEvt->value = (uint8_t*)&pData[paramStartIdx];
                
                LOGD("decodeDeviceLanguageUpdates() bPresent[%d] :: size[%d] :: value[%s]", pEvt->bPresent, pEvt->size, pEvt->value);
                paramStartIdx = paramStartIdx + pEvt->size;
                break;
            }

            default : 
                break;
        }

    }
}


int DeviceNotification::handleDeviceInformationUpdateEvent(LinkController* pLnkCtrler, int8_t session, DeviceInformationUpdateEvt* pEvent)
{
    int result = kIap2NoErr;

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(DeviceInformationUpdateEvt) + pEvent->size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bPresent);
    if (pEvent->bPresent)
    {
        LOGD("title size[%u] value[%p]", pEvent->size, pEvent->value);
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->value, pEvent->size);
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt =kEvtDeviceInformationUpdate;
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


int DeviceNotification::handleDeviceLanguageUpdateEvent(LinkController* pLnkCtrler, int8_t session, DeviceLanguageUpdateEvt* pEvent)
{
    int result = kIap2NoErr;

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(DeviceLanguageUpdateEvt) + pEvent->size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bPresent);
    if (pEvent->bPresent)
    {
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->value, pEvent->size);
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtDeviceLanguageUpdate;
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


