/*
    File:       VoiceOver.cpp
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

#include "VoiceOver.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

VoiceOver::VoiceOver()
{
    LOGD("VoiceOver()");
}

VoiceOver::~VoiceOver()
{
    LOGD("~VoiceOver()");
}


void VoiceOver::handleVoiceOverUpdates(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    VoiceOverUpdatesEvt event;

    LOGD("handleVoiceOverUpdate() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeVoiceOverUpdates(pData, &event);
    
    result = handleVoiceOverUpdatesEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handlePowerUpdate(). result[%d]", result);
    
}

void VoiceOver::decodeVoiceOverUpdates(uint8_t* pData, VoiceOverUpdatesEvt* pEvt)
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

    LOGD("decodeVoiceOverUpdates() nMsgLen[%d]", msgLen);

    int paramStartIdx = 6;
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
        switch (paramId) {
            case kParamIDSpeakingVolume : {
                pEvt->SpeakingVolume.bPresent = TRUE;
                 pEvt->SpeakingVolume.value = DataDecoder::getUintFromBytes(pData, paramStartIdx + 4, 1);                
                break;
            }

            case kParamIDSpeakingRate : {
                pEvt->SpeakingRate.bPresent = TRUE;
                  pEvt->SpeakingRate.value = DataDecoder::getUintFromBytes(pData, paramStartIdx + 4, 1);
                break;
            }

            case kParamIDEnabled : {
                pEvt->Enabled = TRUE;
                break;
            }

        }
        paramStartIdx += paramLength;

    }
}



void VoiceOver::handleVoiceOverCursorUpdates(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    VoiceOverCursorUpdatesEvt event;

    LOGD("handleVoiceOverCursorUpdates() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeVoiceOverCursorUpdates(pData, &event);

    result = handleVoiceOverCursorUpdatesEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handlePowerUpdate(). result[%d]", result);

}

void VoiceOver::decodeVoiceOverCursorUpdates(uint8_t* pData, VoiceOverCursorUpdatesEvt* pEvt)
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

    LOGD("decodeVoiceOverCursorUpdates() nMsgLen[%d]", msgLen);

    int paramStartIdx = 6;
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
        switch (paramId) {
            case kParamIDLabel : {
                pEvt->Label.bPresent = TRUE;
                pEvt->Label.size = paramLength -4;
                pEvt->Label.value =  (uint8_t*)&pData[paramStartIdx+4];
                LOGD("pEvt->Label.value() [%s]", pEvt->Label.value);
                break;
            }

            case kParamIDValue : {
                pEvt->Value.bPresent = TRUE;
                pEvt->Value.size = paramLength -4;
                pEvt->Value.value =  (uint8_t*)&pData[paramStartIdx+4];
                LOGD("pEvt->Value.value() [%s]", pEvt->Value.value);
                break;
            }

            case kParamIDHint : {
                pEvt->Hint.bPresent = TRUE;
                pEvt->Hint.size = paramLength -4;
                pEvt->Hint.value =  (uint8_t*)&pData[paramStartIdx+4];
                LOGD("pEvt->Hint.value() [%s]", pEvt->Hint.value);                
                break;
            }

            case kParamIDTraits : {                
                pEvt->Traits.size =    paramLength - 4;        
                LOGD("pEvt->Traits.size() [%d]", pEvt->Traits.size);
                if(pEvt->Traits.size > 0) {
                    pEvt->Traits.bPresent = TRUE;                    
                    pEvt->Traits.value = (uint16_t*)&pData[paramStartIdx+4]; 
                }
                break;
            }

        }
        
        paramStartIdx += paramLength;

    }
}

int VoiceOver::handleVoiceOverUpdatesEvent(LinkController* pLnkCtrler, int8_t session, VoiceOverUpdatesEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleVoiceOverUpdateEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*5);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*5, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->SpeakingVolume.bPresent);
    if (pEvent->SpeakingVolume.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->SpeakingVolume.value);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->SpeakingRate.bPresent);
    if (pEvent->SpeakingRate.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->SpeakingRate.value);
    }
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->Enabled);
    
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    
    mEventHeader->evt = kEvtVoiceOverUpdates;
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

int VoiceOver::handleVoiceOverCursorUpdatesEvent(LinkController* pLnkCtrler, int8_t session, VoiceOverCursorUpdatesEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleVoiceOverCursorUpdateEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    //uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*15);
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(VoiceOverCursorUpdatesEvt) + pEvent->Label.size + pEvent->Value.size + pEvent->Hint.size + pEvent->Traits.size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    //iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*15, sizeof(EventHeader));
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->Label.bPresent);
    LOGD("pEvent->Label.bPresent() [%d]", pEvent->Label.bPresent);
    if (pEvent->Label.bPresent == TRUE) {
        LOGD("pEvent->Label.bPresent() [%s]", pEvent->Label.value);
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->Label.value, pEvent->Label.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->Value.bPresent);
    LOGD("pEvent->Value.bPresent() [%d]", pEvent->Value.bPresent);
    if (pEvent->Value.bPresent == TRUE) {
        LOGD("pEvent->Value.bPresent() [%s]", pEvent->Value.value);
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->Value.value, pEvent->Value.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->Hint.bPresent);
    LOGD("pEvent->Hint.bPresent() [%d]", pEvent->Hint.bPresent);
    if (pEvent->Hint.bPresent == TRUE) {
        LOGD("pEvent->Hint.bPresent() [%s]", pEvent->Hint.value);
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->Hint.value, pEvent->Hint.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->Traits.bPresent);
    LOGD("pEvent->Traits.bPresent() [%d]", pEvent->Traits.bPresent);
    if (pEvent->Traits.bPresent == TRUE) {
        LOGD("pEvent->Traits.size : %d", pEvent->Traits.size);
        iAp2MarshalPutBytes(&iAp2Marshal, (uint8_t*)pEvent->Traits.value, pEvent->Traits.size);    
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtVoiceOverCursorUpdates;
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
