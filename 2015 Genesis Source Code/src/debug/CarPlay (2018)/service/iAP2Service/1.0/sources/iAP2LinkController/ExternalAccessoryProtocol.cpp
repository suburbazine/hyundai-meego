/*
    File:       ExternalAccessoryProtocol.cpp
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

#include "ExternalAccessoryProtocol.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

ExternalAccessoryProtocol::ExternalAccessoryProtocol()
{
    LOGD("ExternalAccessoryProtocol()");
}

ExternalAccessoryProtocol::~ExternalAccessoryProtocol()
{
    LOGD("~ExternalAccessoryProtocol()");
}


void ExternalAccessoryProtocol::handleStartExternalAccessoryProtocolSession(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    StartExternalAccessoryProtocolSessionEvt event;

    LOGD("handleStarExternalAccessoryProtocolUpdate() session[%d]");
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeStartExternalAccessoryProtocolSession(pData, &event);

    result = handleStartExternalAccessoryProtocolSessionEvent(pLnkCtrler, session, &event);
    
    if (result != kIap2NoErr)
        LOGE("    ERROR : handleStartExternalAccessoryProtocolSessionEvent(). result[%d]", result);

#if 0 // TEST Code
    char data[1000] ;
    memset(data, 0x00, sizeof(1000));
    strcpy(data, "<exlapBeacon> <ServiceBeacon id=\"lVzcR4rukQ0Yn5NtST6EpFVfi6VjRtSx\" address=\"socket://127.0.0.1:25010\" service=\"VW SAI-Server STD\" status=\"active\" description=\"Dies ist der VW Standard Application Interface Server. API Level 4\" version=\"4.0\" envelope=\"pretty\"/> </exlapBeacon>");
    uint8_t* EAPData = new uint8_t[strlen(data) + 1 + 2];
    uint16_t* EAPHeader = NULL;
    EAPHeader = (uint16_t*)EAPData;
    //*(EAPData) = 0;
    //*(EAPData + 1) = 3;
    *(EAPHeader) = DataDecoder::getUintFromBytes((uint8_t*)&event.ExternalAccessoryProtocolSessionIdentifier, 0, 2);
    
    memcpy(EAPData + 2, data, strlen(data) + 1);
    LOGD("EAP DATA : %s", data);

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }
    session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeEA);
    pLnkCtrler->processOutBoundTrafficEAPData(link, session, EAPData, strlen(data) + 1 + 2);    
#endif
    
}

void ExternalAccessoryProtocol::decodeStartExternalAccessoryProtocolSession(uint8_t* pData, StartExternalAccessoryProtocolSessionEvt* pEvt)
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

    LOGD("decodeStartExternalAccessoryProtocolSession() nMsgLen[%d]", msgLen);

    int paramStartIdx = 6;
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);

        switch (paramId) {
            case kParamIDExternalAccessoryProtocolIdentifier : {                
                pEvt->ExternalAccessoryProtocolIdentifier = DataDecoder::getUintFromBytes(pData, paramStartIdx+4,1); // 8
                break;
            }

            case kParamIDExternalAccessoryProtocolSessionIdentifier_start : {
                pEvt->ExternalAccessoryProtocolSessionIdentifier = DataDecoder::getUintFromBytes(pData, paramStartIdx+4,2); // 16                
                break;
            }

            default : {
                LOGD("unknown paramID [%d]", paramId);
                break;
            }
                

        }
        paramStartIdx += paramLength ;
    }
}


void ExternalAccessoryProtocol::handleStopExternalAccessoryProtocolSession(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    StopExternalAccessoryProtocolSessionEvt event;

    LOGD("handleStopExternalAccessoryProtocolSession() session[%d]");
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeStopExternalAccessoryProtocolSession(pData, &event);
    
    result = handleStopExternalAccessoryProtocolSessionEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleStopExternalAccessoryProtocolSession(). result[%d]", result);
    
}

void ExternalAccessoryProtocol::decodeStopExternalAccessoryProtocolSession(uint8_t* pData, StopExternalAccessoryProtocolSessionEvt* pEvt)
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

    LOGD("decodeStopExternalAccessoryProtocolSession() nMsgLen[%d]", msgLen);

    int paramStartIdx = 6;
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);

        switch (paramId) {
            case kParamIDExternalAccessoryProtocolSessionIdentifier_stop: {                
                pEvt->ExternalAccessoryProtocolSessionIdentifier = DataDecoder::getUintFromBytes(pData, paramStartIdx+4,2); // 8
                break;
            }

            default : {
                LOGD("unknown paramID [%d]", paramId);
                break;
            }             
        }
        paramStartIdx += paramLength ;
    }
}

int ExternalAccessoryProtocol::handleStartExternalAccessoryProtocolSessionEvent (LinkController* pLnkCtrler, int8_t session, StartExternalAccessoryProtocolSessionEvt * pEvent)
{
    int result = kIap2NoErr;

    LOGD("HandleStartExternalAccessoryProtocolSessionEvt() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(uint32_t)*6 + sizeof(StartExternalAccessoryProtocolSessionEvt);
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->ExternalAccessoryProtocolIdentifier);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->ExternalAccessoryProtocolSessionIdentifier);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtStartExternalAccessoryProtcolSession;
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

int ExternalAccessoryProtocol::handleStopExternalAccessoryProtocolSessionEvent(LinkController* pLnkCtrler, int8_t session, StopExternalAccessoryProtocolSessionEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleStopExternalAccessoryProtocolSessionEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(uint32_t)*6 + sizeof(StopExternalAccessoryProtocolSessionEvt);
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->ExternalAccessoryProtocolSessionIdentifier);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtStopExternalAccessoryProtcolSession;
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

int ExternalAccessoryProtocol::handleExternalAccessoryProtocolRecvData(LinkController* pLnkCtrler, int8_t session, uint8_t* data, uint32_t dataLen)
{
    int result = kIap2NoErr;

    LOGD("handleExternalAccessoryProtocolRecvData() session[%d]", session);

    if (data == NULL) {
        LOGE("ERROR : data is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(uint32_t)*6 + dataLen;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* pEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));    
    
    iAp2MarshalPutBytes(&iAp2Marshal, data, dataLen);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pEventHeader->evt = kEvtExternalAccessoryProtcolData;
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

