/*
    File:       SessionHID.cpp
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

#include "SessionHID.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

namespace IAP2
{

SessionHID::SessionHID()
{
    LOGD("SessionHID()");
}

SessionHID::~SessionHID()
{
    LOGD("~SessionHID()");
}

int SessionHID::handleHIDCmd(unsigned int cmd, unsigned int handle, unsigned char* pRxData, LinkController* pLnkCtrler)
{
    LOGD("handleHIDCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);
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
            case kCmdStartHID : {
            StartHIDArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.HIDCompnetIdentifier = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.VendorIdentifier = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.ProductIdentifer = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.LocalizedKeyBoardCountryCode.bPresent= (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.LocalizedKeyBoardCountryCode.bPresent == TRUE)
                arg.LocalizedKeyBoardCountryCode.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.HIDReportDescriptor.value = iAp2MarshalGetBytes(&iAp2Marshal,&arg.HIDReportDescriptor.size); // this part may be chage type to blob
            
            sendStartHID(pLnkCtrler, &arg);
            break;
        }
        case kCmdAccessoryHIDReport : {
             AccessoryHIDReportArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.HIDCompnetIdentifier = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);            
            arg.HIDReport.value = iAp2MarshalGetBytes(&iAp2Marshal,&arg.HIDReport.size); // this part may be chage type to blob
        
            sendAccessoryHIDReport(pLnkCtrler, &arg);
            break;
        }
        case kCmdStopHID : {
             StopHIDArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.HIDCompnetIdentifier = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
        
            sendStopHID(pLnkCtrler, &arg);
            break;
        }  
          

        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}

void SessionHID::sendStartHID(LinkController* pLnkCtrler, StartHIDArg* pArg)
{
    LOGD("sendStartHID()");

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

    createStartHIDParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartHID,
                          nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedData_arr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }        
    pLnkCtrler->processOutBoundTraffic(link, session, encodedData_arr);

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}

void SessionHID::sendAccessoryHIDReport(LinkController* pLnkCtrler, AccessoryHIDReportArg* pArg)
{
    LOGD("sendAccessoryHIDReport()");

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

    createAccessoryHIDReportParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdAccessoryHIDReport,
                          nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedData_arr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }        
    pLnkCtrler->processOutBoundTraffic(link, session, encodedData_arr);

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionHID::sendStopHID(LinkController* pLnkCtrler, StopHIDArg* pArg)
{
    LOGD("sendStopHID()");

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

    createStopHIDParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopHID,
                          nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedData_arr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }        
    pLnkCtrler->processOutBoundTraffic(link, session, encodedData_arr);

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionHID::createStartHIDParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StartHIDArg* pArg)
{
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();


    DataEncoder::appendNumericParam(pArg->HIDCompnetIdentifier, 2, kParamIDHIDComponentIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNumericParam(pArg->VendorIdentifier, 2, kParamIDVendorIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNumericParam(pArg->ProductIdentifer, 2, kParamIDProductIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    if (pArg->LocalizedKeyBoardCountryCode.bPresent == TRUE)
        DataEncoder::appendNumericParam(pArg->LocalizedKeyBoardCountryCode.value, 1, kParamIDLocalizedKeyboardCountryCode, nParamIdList, paramDataList, nParamDataLenList);
    
    DataEncoder::appendBytesParam(pArg->HIDReportDescriptor.value, pArg->HIDReportDescriptor.size, kParamIDHIDReportDescriptor, nParamIdList, paramDataList, nParamDataLenList);// must change type to blob
}

void SessionHID::createAccessoryHIDReportParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, AccessoryHIDReportArg* pArg)
{
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();


    DataEncoder::appendNumericParam(pArg->HIDCompnetIdentifier, 2, kParamIDHIDComponentIdentifier, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendBytesParam(pArg->HIDReport.value, pArg->HIDReport.size, kParamIDHIDReport, nParamIdList, paramDataList, nParamDataLenList);// must change type to blob
}

void SessionHID::createStopHIDParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StopHIDArg* pArg)
{
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();
    DataEncoder::appendNumericParam(pArg->HIDCompnetIdentifier, 2, kParamIDHIDComponentIdentifier, nParamIdList, paramDataList, nParamDataLenList);
}

}// namespace IAP2

