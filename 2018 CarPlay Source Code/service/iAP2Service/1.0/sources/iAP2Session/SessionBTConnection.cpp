/*
    File:       SessionBTConnection.cpp
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

#include "SessionBTConnection.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

namespace IAP2
{

SessionBTConnection::SessionBTConnection()
{
    LOGD("SessionBTConnection()");
}

SessionBTConnection::~SessionBTConnection()
{
    LOGD("~SessionBTConnection()");
}

int SessionBTConnection::handleBTConnectionCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    int result = kIap2NoErr;
    LOGD("handlePowerCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);

    if (pRxData == NULL) {
        LOGE("    ERROR : pRxData is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR : pLnkCtrler is NULL");
        return kIap2ParamErr;
    }

    switch(cmd) {
        case kCmdBTComponentInformation : {
            BTComponentInformationArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            LOGD("    rcv kCmdBTComponentInformation");
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.btComponentStatus = (BTComponentStatusParam*)iAp2MarshalGetBytes(&iAp2Marshal, &arg.nBTComponentStatusCount);
            if ( arg.btComponentStatus == NULL) {
                LOGD("ERROR arg.btComponentStatus is NULL");
                arg.bPresent = FALSE;
            }
            arg.nBTComponentStatusCount /= sizeof(BTComponentStatusParam);
            sendBTComponentInformation(pLnkCtrler, &arg);
            break;
        }

        case kCmdStartBTConnectionUpdates : {
            StartBTConnectionUpdatesArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            LOGD("    rcv kCmdStartBTConnectionUpdates");
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.bPresent= (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.BTTransportComponentID = (uint16_t*)iAp2MarshalGetBytes(&iAp2Marshal, &arg.nBTTransportComponentIDCount);
            if ( arg.BTTransportComponentID == NULL) {
                LOGD("ERROR arg.BTTransportComponentID is NULL");
                arg.bPresent = FALSE;
            }
            arg.nBTTransportComponentIDCount /= sizeof(uint16_t);
            sendStartBTConnectionUpdates(pLnkCtrler, &arg);
            break;
        }

        case kCmdStopBTConnectionUpdates : {
            uint32_t dummyValue = 0;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
                        
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            dummyValue = iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    rcv kCmdStopBTConnectionUpdates dummyValue[0x%x]", dummyValue);

            sendStopBTConnectionUpdates(pLnkCtrler);
            break;
        }

        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    return result;
}


void SessionBTConnection::sendBTComponentInformation(LinkController* pLnkCtrler,/*Transport* pTransport, struct iAP2Link_st* link, uint8_t session,*/ BTComponentInformationArg* pArg)
{
    LOGD("sendBTComponentInformation()");

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

    createBTComponentInformationParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdBluetoothComponentInformation, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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

void SessionBTConnection::sendStartBTConnectionUpdates(LinkController* pLnkCtrler, StartBTConnectionUpdatesArg* pArg)
{
    LOGD("sendStartBTConnectionUpdates()");

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

    createStartBTConnectionUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartBluetoothConnectionUpdates,
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


void SessionBTConnection::sendStopBTConnectionUpdates(LinkController* pLnkCtrler)
{
    LOGD("sendStopBTConnectionUpdates()");

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

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopBluetoothConnectionUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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


void SessionBTConnection::createBTComponentInformationParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, BTComponentInformationArg* pArg)
{
    LOGD("createBTComponentInformationParamList()");

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    
    uint8_t* groupParamBytes = NULL;
    uint32_t nNumOfBytes;

    if (pArg->bPresent == TRUE) {
        while(pArg->nBTComponentStatusCount-- > 0) {
            nNumOfBytes = createGroupParamBytesForBTComponentStatus(&groupParamBytes, pArg->btComponentStatus);
            LOGD("    kParamIdBTComponentStatus groupParamBytes");
            LOGD("    groupParamBytes[%s] nNumOfBytes[%u]", (const char*)groupParamBytes, nNumOfBytes);
            DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdComponentID, nParamIdList, paramDataList, nParamDataLenList);
            delete [] groupParamBytes;
            groupParamBytes = NULL;
            pArg->btComponentStatus += 1;
       }
    }
}

uint32_t SessionBTConnection::createGroupParamBytesForBTComponentStatus(uint8_t** encodedBytes, BTComponentStatusParam* pArg)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;
    uint8_t* encodedDataArr;

    LOGD("pArg->componentID : %u, pArg->componentEnabled : %u", pArg->componentID, pArg->componentEnabled);
    DataEncoder::appendNumericParam(pArg->componentID, 2, kParamIdComponentID, nSubParamIdList, subParamDataList, nSubParamDataLenList);
    if(pArg->componentEnabled == TRUE) {
        DataEncoder::appendNumericParam(pArg->componentEnabled, 1, kParamIdComponentEnabled, nSubParamIdList, subParamDataList, nSubParamDataLenList);
    }

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

void SessionBTConnection::createStartBTConnectionUpdatesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StartBTConnectionUpdatesArg* pArg)
{
    LOGD("createStartBTConnectionUpdatesParamList()");

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

//    LOGD("    availCurrent bPresent[%d] value[%u] bPresent[%d] value[%u]", pArg->availCurrent.bPresent, pArg->availCurrent.value, pArg->devBatteryCharge.bPresent, pArg->devBatteryCharge.value);
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    LOGD("pArg->nBTTransportComponentIDCount : %u", pArg->nBTTransportComponentIDCount);
    if (pArg->bPresent == TRUE) {
        while(pArg->nBTTransportComponentIDCount--){
            DataEncoder::appendNumericParam(*pArg->BTTransportComponentID, 2, kParamIdTransportComponentID, nParamIdList, paramDataList, nParamDataLenList);
        }
        pArg->BTTransportComponentID += 1;
    }

}


}// namespace IAP2

