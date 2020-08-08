/*
    File:       SessionExternalAccessoryProtocol.cpp
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
#include "SessionExternalAccessoryProtocol.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

namespace IAP2
{

SessionExternalAccessoryProtocol::SessionExternalAccessoryProtocol()
{
    LOGD("SessionExternalAccessoryProtocol()");
}

SessionExternalAccessoryProtocol::~SessionExternalAccessoryProtocol()
{
    LOGD("~SessionExternalAccessoryProtocol()");
}

int SessionExternalAccessoryProtocol::handleExternalAccessoryProtocolCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    LOGD("handleExternalAccessoryProtocolCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);
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
        case kCmdStatusExternalAccessoryProtocolSession : {
            StatusExternalAccessoryProtocolSessionArg arg;
            memset(&arg, 0x00, sizeof(StatusExternalAccessoryProtocolSessionArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.nExternalAccessroyProtocolSessionIdentifier = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.eExternalAccessoryProtocolSessionStatus = (ExternalAccessoryProtocolSessionStatusEnum)iAp2MarshalGetUint32(&iAp2Marshal);
            sendStatusExternalAccessoryProtocolSession(pLnkCtrler, &arg);
            break;
        }

        case kCmdExternalAccessoryProtocolData : {
            ExternalAccessoryProtocolDataArg arg;

            struct iAP2Link_st* link;
            if (pLnkCtrler->m_piAP2LinkRunLoop)
                link = pLnkCtrler->m_piAP2LinkRunLoop->link;
            else {
                LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
                return kIap2ParamErr;
            }
    
            memset(&arg, 0x00, sizeof(ExternalAccessoryProtocolDataArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.size);
            arg.nExternalAccessroyProtocolSessionIdentifier = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);

            int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeEA);
            if (session == 0) {
                LOGE("    ERROR : sessionId is 0");
                return kIap2ParamErr;
            }
            else
            {
                uint8_t* EAPData = new uint8_t[arg.size + 2];

                if(EAPData == NULL) {
                    LOGE(" ERROR : EAPData Memory alloc failed");
                    return kIap2NoMemoryErr;
                }

                uint16_t* EAPHeader = NULL;
                EAPHeader = (uint16_t*)EAPData;
                *(EAPHeader) = DataDecoder::getUintFromBytes((uint8_t*)&arg.nExternalAccessroyProtocolSessionIdentifier, 0, 2);
                memcpy(EAPData + 2, arg.value, arg.size);
                pLnkCtrler->processOutBoundTrafficEAPData(link, session, EAPData, arg.size + 2);

                if(EAPData != NULL)
                    free(EAPData);

                EAPData = NULL;
            }
            
            break;
        }
        
        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}
void SessionExternalAccessoryProtocol::sendStatusExternalAccessoryProtocolSession(LinkController* pLnkCtrler, StatusExternalAccessoryProtocolSessionArg* pArg)
{
    LOGD("sendStatusExternalAccessoryProtocolSession()");
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
    
    createStatusExternalAccessoryProtocolSessionParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);    
    
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStatusExternalAccessoryProtocolSession,
                          nParamIdList, paramDataList, nParamDataLenList, &encodedData);
    
    ByteArray encodedDaraArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDaraArr);        
    
    DataEncoder::deleteParamList(nParamIdList,paramDataList, nParamDataLenList );
    delete [] encodedData;

}

void SessionExternalAccessoryProtocol::createStatusExternalAccessoryProtocolSessionParamList(std::vector<int>& nParamIdList,
                                                     std::vector<const uint8_t *>& paramDataList,
                                                     std::vector<uint32_t>& nParamDataLenList,
                                                     StatusExternalAccessoryProtocolSessionArg* pArg)
{
#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }    
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();
   
    DataEncoder::appendNumericParam(pArg->nExternalAccessroyProtocolSessionIdentifier, 2, kParamIDExternalAccessroyProtocolIdentifier, nParamIdList, paramDataList, nParamDataLenList);    
    DataEncoder::appendNumericParam(pArg->eExternalAccessoryProtocolSessionStatus, 1, kParamIDExternalAccessoryProtocolSessionIdentifier, nParamIdList, paramDataList, nParamDataLenList);    
}


}// namespace IAP2

