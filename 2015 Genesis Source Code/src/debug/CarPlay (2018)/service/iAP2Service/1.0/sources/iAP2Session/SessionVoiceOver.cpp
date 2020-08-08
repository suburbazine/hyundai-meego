/*
    File:       SessionVoiceOver.cpp
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


#include "SessionVoiceOver.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

namespace IAP2
{

SessionVoiceOver::SessionVoiceOver()
{
    LOGD("SessionVoiceOver()");
}

SessionVoiceOver::~SessionVoiceOver()
{
    LOGD("SessionVoiceOver()");
}

int SessionVoiceOver::handleVoiceOverCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    int result = kIap2NoErr;
    LOGD("handleVoiceOverCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);

    if (pRxData == NULL) {
        LOGE("    ERROR : pRxData is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR : pLnkCtrler is NULL");
        return kIap2ParamErr;
    }

    switch(cmd) {
        case kCmdStartVoiceOver : {
            uint32_t dummyValue = 0;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};        
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            
            dummyValue = iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    rcv kCmdStartVoiceOver dummyValue[0x%x]", dummyValue);
            sendStartVoiceOver(pLnkCtrler);
            break;
        }

        case kCmdStopVoiceOver : {
            uint32_t dummyValue = 0;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            dummyValue = iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    rcv kCmdStopVoiceOver dummyValue[0x%x]", dummyValue);
            sendStopVoiceOver(pLnkCtrler);
            break;
        }

        case kCmdRequestVoiceOverMoveCursor : {
            RequestVoiceOverMoveCursorArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

            LOGD("    rcv kCmdRequestVoiceOverMoveCursor");            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            arg.CursorDirection = (VOCurSorDirection)iAp2MarshalGetUint32(&iAp2Marshal);
               sendRequestVoiceOverMoveCursor(pLnkCtrler, &arg);
            break;
        }

        case kCmdRequestVoiceOverActivateCursor : {
            uint32_t dummyValue = 0;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            dummyValue = iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    rcv kCmdRequestVoiceOverActivateCursor dummyValue[0x%x]", dummyValue);
            sendRequestVoiceOverActivateCursor(pLnkCtrler);
            break;
        }

        case kCmdRequestVoiceOverScrollPage : {
            RequestVoiceOverScrollPageArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

            LOGD("    rcv kCmdRequestVoiceOverScrollPage");            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            arg.ScrollDirection = (VOScrollDirection)iAp2MarshalGetUint32(&iAp2Marshal);
            sendRequestVoiceOverScrollPage(pLnkCtrler, &arg);
            break;
        }

        case kCmdRequestVoiceOverSpeakText : {
            RequestVoiceOverSpeakTextArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

            LOGD("    rcv kCmdRequestVoiceOverSpeakText");            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            arg.TextToSpeak.bPresent  = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if ( arg.TextToSpeak.bPresent == TRUE ) {
                arg.TextToSpeak.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.TextToSpeak.size);
            }            
            sendRequestVoiceOverSpeakText(pLnkCtrler, &arg);
            break;
        }

        case kCmdRequestVoiceOverPauseText : {
            uint32_t dummyValue = 0;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            dummyValue = iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    rcv kCmdRequestVoiceOverPauseText dummyValue[0x%x]", dummyValue);
            sendRequestVoiceOverPauseText(pLnkCtrler);
            break;
        }

        case kCmdRequestVoiceOverResumeText : {
            uint32_t dummyValue = 0;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            dummyValue = iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    rcv kCmdRequestVoiceOverResumeText dummyValue[0x%x]", dummyValue);
            sendRequestVoiceOverResumeText(pLnkCtrler);
            break;
        }

        case kCmdStartVoiceOverUpdates : {
            StartVoiceOverUpdatesArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

            LOGD("    rcv kCmdStartVoiceOverUpdates");            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            arg.SpeakingVolume = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.SpeakingRate = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);            
            arg.Enabled = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);            
            

            sendStartVoiceOverUpdates(pLnkCtrler, &arg);
            break;
        }

        case kCmdStopVoiceOverUpdates : {
            uint32_t dummyValue = 0;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            dummyValue = iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    rcv kCmdStopVoiceOverUpdates dummyValue[0x%x]", dummyValue);
            sendStopVoiceOverUpdates(pLnkCtrler);
            break;
        }

        case kCmdRequestVoiceOverConfiguration : {
            RequestVoiceOverConfigurationArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

            LOGD("    rcv kCmdRequestVoiceOverConfiguration");            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.SpeakingVolume.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if ( arg.SpeakingVolume.bPresent == TRUE) {
                arg.SpeakingVolume.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
            }
                
            arg.SpeakingRate.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if ( arg.SpeakingRate.bPresent == TRUE) {
                arg.SpeakingRate.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
            }

            sendRequestVoiceOverConfiguration(pLnkCtrler, &arg);
            break;
        }

        case kCmdStartVoiceOverCursorUpdates : {
            StartVoiceOverCursorUpdatesArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

            LOGD("    rcv kCmdStartVoiceOverCursorUpdates");            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            arg.Label = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.Value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.Hint = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.Traits = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);            
            sendStartVoiceOverCursorUpdates(pLnkCtrler, &arg);
            break;
        }

        case kCmdStopVoiceOverCursorUpdates : {
            uint32_t dummyValue = 0;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            dummyValue = iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    rcv kCmdStopVoiceOverUpdates dummyValue[0x%x]", dummyValue);
            sendStopVoiceOverCursorUpdates(pLnkCtrler);
            break;
        }


        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}

void SessionVoiceOver::sendStartVoiceOver (LinkController* pLnkCtrler)
{
    LOGD("sendStartVoiceOver()");

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

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartVoiceOver, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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

void SessionVoiceOver::sendStopVoiceOver (LinkController* pLnkCtrler)
{
    LOGD("sendStopVoiceOver()");

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

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopVoiceOver, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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

void SessionVoiceOver::sendRequestVoiceOverMoveCursor (LinkController* pLnkCtrler, RequestVoiceOverMoveCursorArg* pArg)
{
     LOGD("sendRequestVoiceOverMoveCursor()");

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

    createRequestVoiceOverMoveCursorParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdRequestVoiceOverMoveCursor,
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



void SessionVoiceOver::createRequestVoiceOverMoveCursorParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, RequestVoiceOverMoveCursorArg* pArg)
{
    LOGD("createRequestVoiceOverMoveCursorParamList()");

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }    
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendNumericParam(pArg->CursorDirection, 1,  kParamIDCursorDirection, nParamIdList, paramDataList, nParamDataLenList);
}


void SessionVoiceOver::sendRequestVoiceOverActivateCursor(LinkController * pLnkCtrler)
{
    LOGD("sendRequestVoiceOverActivateCursor()");

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

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdRequestVoiceOverActivateCursor, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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


void SessionVoiceOver::sendRequestVoiceOverScrollPage(LinkController * pLnkCtrler, RequestVoiceOverScrollPageArg * pArg)
{
     LOGD("sendRequestVoiceOverScrollPage()");

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

    createRequestVoiceOverScrollPageParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdRequestVoiceOverScrollPage,
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



void SessionVoiceOver::createRequestVoiceOverScrollPageParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, RequestVoiceOverScrollPageArg * pArg)
{
    LOGD("createRequestVoiceOverScrollPageParamList()");

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }    
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendNumericParam(pArg->ScrollDirection, 1,  kParamIDScrollDirection, nParamIdList, paramDataList, nParamDataLenList);
}

void SessionVoiceOver::sendRequestVoiceOverSpeakText(LinkController * pLnkCtrler, RequestVoiceOverSpeakTextArg * pArg)
{
     LOGD("sendRequestVoiceOverSpeakText()");

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

    createRequestVoiceOverSpeakTextParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdRequestVoiceOverSpeakText,
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



void SessionVoiceOver::createRequestVoiceOverSpeakTextParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, RequestVoiceOverSpeakTextArg * pArg)
{
    LOGD("createRequestVoiceOverSpeakTextParamList()");

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }    
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    if(pArg->TextToSpeak.bPresent == TRUE){
            DataEncoder::appendBytesParam(pArg->TextToSpeak.value, pArg->TextToSpeak.size, kParamIDTextToSpeak, nParamIdList, paramDataList, nParamDataLenList);
    }
}

void SessionVoiceOver::sendRequestVoiceOverPauseText (LinkController * pLnkCtrler)
{
    LOGD("sendRequestVoiceOverPauseText()");

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

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdRequestVoiceOverPauseText, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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

void SessionVoiceOver::sendRequestVoiceOverResumeText(LinkController * pLnkCtrler)
{
    LOGD("sendRequestVoiceOverResumeText()");

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

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdRequestVoiceOverResumeText, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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


void SessionVoiceOver::sendStartVoiceOverUpdates(LinkController * pLnkCtrler, StartVoiceOverUpdatesArg * pArg)
{
    LOGD("sendStartVoiceOverUpdates()");

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

    createStartVoiceOverUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartVoiceOverUpdates,
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



void SessionVoiceOver::createStartVoiceOverUpdatesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StartVoiceOverUpdatesArg * pArg)
{
    LOGD("createsendStartVoiceOverUpdateParamList()");

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }    
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    if(pArg->SpeakingVolume == TRUE){
        DataEncoder::appendNoneParam( kParamIDSpeakingVolume, nParamIdList, paramDataList, nParamDataLenList);
    }

    if(pArg->SpeakingRate == TRUE){
        DataEncoder::appendNoneParam( kParamIDSpeakingRate, nParamIdList, paramDataList, nParamDataLenList);
    }

    if(pArg->Enabled == TRUE){
        DataEncoder::appendNoneParam( kParamIDEnabled, nParamIdList, paramDataList, nParamDataLenList);
    }
}


void SessionVoiceOver::sendStopVoiceOverUpdates(LinkController * pLnkCtrler)
{
    LOGD("sendStopVoiceOverUpdates()");

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

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopVoiceOverUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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



void SessionVoiceOver::sendRequestVoiceOverConfiguration(LinkController * pLnkCtrler, RequestVoiceOverConfigurationArg * pArg)
{
         
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

    createRequestVoiceOverConfigurationParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdRequestVoiceOverConfiguration,
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



void SessionVoiceOver::createRequestVoiceOverConfigurationParamList( std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, RequestVoiceOverConfigurationArg * pArg)
{
    LOGD("createRequestVoiceOverConfigurationParamList()");

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }    
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    if(pArg->SpeakingVolume.bPresent== TRUE){
        DataEncoder::appendNumericParam( pArg->SpeakingVolume.value, 1 ,kParamIDSpeakingVolume_config, nParamIdList, paramDataList, nParamDataLenList);
    }

    if(pArg->SpeakingRate.bPresent== TRUE){
        DataEncoder::appendNumericParam( pArg->SpeakingRate.value, 1 ,kParamIDSpeakingRate_config, nParamIdList, paramDataList, nParamDataLenList);
    }

}



void SessionVoiceOver::sendStartVoiceOverCursorUpdates(LinkController * pLnkCtrler, StartVoiceOverCursorUpdatesArg * pArg)
{
    LOGD("sendStartVoiceOverCursorUpdates()");

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

    createStartVoiceOverCursorUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartVoiceOverCursorUpdates,
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



void SessionVoiceOver::createStartVoiceOverCursorUpdatesParamList( std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StartVoiceOverCursorUpdatesArg * pArg)
{
    LOGD("createStartVoiceOverCursorUpdatesParamList()");

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }    
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    if(pArg->Label == TRUE){
        DataEncoder::appendNoneParam( kParamIDLabel, nParamIdList, paramDataList, nParamDataLenList);
    }

    if(pArg->Value == TRUE){
        DataEncoder::appendNoneParam(kParamIDValue, nParamIdList, paramDataList, nParamDataLenList);
    }

    if(pArg->Hint == TRUE){
        DataEncoder::appendNoneParam(kParamIDHint, nParamIdList, paramDataList, nParamDataLenList);
    }

    if(pArg->Traits == TRUE){
        DataEncoder::appendNoneParam(kParamIDTraits, nParamIdList, paramDataList, nParamDataLenList);
    }
}


void SessionVoiceOver::sendStopVoiceOverCursorUpdates(LinkController * pLnkCtrler)
{
    LOGD("sendStopVoiceUpdates()");

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

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopVoiceOverCursorUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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




}// namespace IAP2
