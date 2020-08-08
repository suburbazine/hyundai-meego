/*
    File:       SessionTelephony.cpp
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

#include "SessionTelephony.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

namespace IAP2
{

SessionTelephony::SessionTelephony()
{
    LOGD("SessionTelephony()");
}

SessionTelephony::~SessionTelephony()
{
    LOGD("~SessionTelephony()");
}

// Send to iOS
int SessionTelephony::handleTelephonyCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    int result = kIap2NoErr;
    LOGD("handleTelephonyCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);

    if (pRxData == NULL) {
        LOGE("    ERROR : pRxData is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR : pLnkCtrler is NULL");
        return kIap2ParamErr;
    }

    switch(cmd) {
        case kCmdStartTelephonyCallStateInformation : {
            StartTelephonyCallStateInformationArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            LOGD("    rcv kCmdStartTelephonyCallStateInformation");
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.bCallStatevCardFileTransferID = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

            sendStartTelephonyCallStateInformation(pLnkCtrler, &arg);
            break;
        }

        case kCmdStopTelephonyCallStateInformation : {
            uint32_t dummyValue = 0;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            dummyValue = iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    rcv kCmdStopTelephonyCallStateInformation dummyValue[0x%x]", dummyValue);

            sendStopTelephonyCallStateInformation(pLnkCtrler);
            break;
        }

        case kCmdStartTelephonyUpdates : {
            StartTelephonyUpdatesArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            LOGD("    rcv kCmdStartTelephonyUpdates");
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.bTelephonySignalStrength = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bTelephonyRegisStatus = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bTelephonyAirPlaneModeStatus = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bTelephonyTTYStatus = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bTMobileOperator = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

            sendStartTelephonyUpdates(pLnkCtrler, &arg);
            break;
        }

        case kCmdStopTelephonyUpdates : {
            uint32_t dummyValue = 0;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            dummyValue = iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    rcv kCmdStopTelephonyUpdates dummyValue[0x%x]", dummyValue);

            sendStopTelephonyUpdates(pLnkCtrler);
            break;
        }
        
        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}

void SessionTelephony::sendStartTelephonyCallStateInformation(LinkController* pLnkCtrler,/*Transport* pTransport, struct iAP2Link_st* link, uint8_t session,*/ StartTelephonyCallStateInformationArg* pArg)
{
    LOGD("sendStartTelephonyCallStateInformation()");

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

    createStartTelephonyCallStateInformationParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartTelephonyCallStateInformation, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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

void SessionTelephony::sendStopTelephonyCallStateInformation(LinkController* pLnkCtrler)
{
    LOGD("sendStopTelephonyCallStateInformation()");

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
    
    // NoParam to iOS
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopTelephonyCallStateInformation, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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


void SessionTelephony::sendStartTelephonyUpdates(LinkController* pLnkCtrler,/*Transport* pTransport, struct iAP2Link_st* link, uint8_t session,*/ StartTelephonyUpdatesArg* pArg)
{
    LOGD("sendStartTelephonyUpdates()");

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

    createStartTelephonyUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartTelephonyUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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

void SessionTelephony::sendStopTelephonyUpdates(LinkController* pLnkCtrler)
{
    LOGD("sendStopTelephonyUpdates()");

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
    
    // NoParam to iOS
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopTelephonyUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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



void SessionTelephony::createStartTelephonyCallStateInformationParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StartTelephonyCallStateInformationArg* pArg)
{
    LOGD("createStartTelephonyCallStateInformationParamList()");

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    LOGD("    bCallStatevCardFileTransferID[%d] ", pArg->bCallStatevCardFileTransferID);
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    if (pArg->bCallStatevCardFileTransferID == TRUE)
            DataEncoder::appendNoneParam(kParamIdCallStatevCardTransferID, nParamIdList, paramDataList, nParamDataLenList);

}

void SessionTelephony::createStartTelephonyUpdatesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StartTelephonyUpdatesArg* pArg)
{
    LOGD("createStartTelephonyUpdatesParamList()");

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    LOGD("    bTelephonySignalStrength[%d],  bTelephonyRegisStatus[%d], bTelephonyAirPlaneModeStatus[%d], bTelephonyTTYStatus[%d], bTMobileOperator[%d]", 
        pArg->bTelephonySignalStrength, pArg->bTelephonyRegisStatus, pArg->bTelephonyAirPlaneModeStatus, pArg->bTelephonyTTYStatus, pArg->bTMobileOperator);
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    if (pArg->bTelephonySignalStrength == TRUE)
            DataEncoder::appendNoneParam(kParamIdTelephonySignalStrength, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->bTelephonySignalStrength == TRUE)
        DataEncoder::appendNoneParam(kParamIdTelephonyRegisStatus, nParamIdList, paramDataList, nParamDataLenList);
    
    if (pArg->bTelephonySignalStrength == TRUE)
            DataEncoder::appendNoneParam(kParamIdTelephonyAirplaneModeStatus, nParamIdList, paramDataList, nParamDataLenList);
    
    if (pArg->bTelephonySignalStrength == TRUE)
            DataEncoder::appendNoneParam(kParamIdTelephonyTTYStatus, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->bTelephonySignalStrength == TRUE)
        DataEncoder::appendNoneParam(kParamIdMobileOperator, nParamIdList, paramDataList, nParamDataLenList);
}


}// namespace IAP2

