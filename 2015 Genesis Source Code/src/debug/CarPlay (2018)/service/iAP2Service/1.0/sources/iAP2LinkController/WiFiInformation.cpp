/*
    File:       WiFiInformation.cpp
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

#include "WiFiInformation.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

WiFiInformation::WiFiInformation()
{
    LOGD("WiFiInformation()");
}

WiFiInformation::~WiFiInformation()
{
    LOGD("~WiFiInformation()");
}




void WiFiInformation::handleWiFiInformation(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    WiFiInformationEvt event;

    LOGD("handleWiFiInformation() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeWiFiInformation(pData, &event);

    result = handleWiFiInformationEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleWiFiInformation(). result[%d]", result);
    
}

void WiFiInformation::decodeWiFiInformation(uint8_t* pData, WiFiInformationEvt* pEvt)
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

    LOGD("decodeWiFiInformation() nMsgLen[%d]", msgLen);

    int paramStartIdx = 6;
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
        switch (paramId) {
            case kParamIDRequestStatus : {
                pEvt->RequestStatus = (WiFiInfoRequestStatus)DataDecoder::getUintFromBytes(pData, paramStartIdx + 4, 1);
                break;
            }

            case kParamIDWiFiSSID : {
                        pEvt->WiFiSSID.bPresent = TRUE;
                pEvt->WiFiSSID.size = paramLength -4;
                pEvt->WiFiSSID.value =  (uint8_t*)&pData[paramStartIdx+4];
                break;
            }

            case kParamIDWiFiPassphras : {
                pEvt->WifiPassphrase.bPresent = TRUE;
                pEvt->WifiPassphrase.size = paramLength -4;
                pEvt->WifiPassphrase.value =  (uint8_t*)&pData[paramStartIdx+4];
                break;
            }

        }
        paramStartIdx += paramLength;

    }
}


int WiFiInformation::handleWiFiInformationEvent(LinkController* pLnkCtrler, int8_t session, WiFiInformationEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleWiFiInformationEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(WiFiInformationEvt) + pEvent->WiFiSSID.size + pEvent->WifiPassphrase.size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* pEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->RequestStatus);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->WiFiSSID.bPresent);
    if (pEvent->WiFiSSID.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->WiFiSSID.value, pEvent->WiFiSSID.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->WifiPassphrase.bPresent);
    if (pEvent->WifiPassphrase.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->WifiPassphrase.value, pEvent->WifiPassphrase.size);
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pEventHeader->evt = kEvtWiFiInformation;
    pEventHeader->dataSize = byteNum;

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
