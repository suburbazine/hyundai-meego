/*
    File:       SessionVehicleStatus.cpp
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

#include "SessionVehicleStatus.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"
namespace IAP2
{

SessionVehicleStatus::SessionVehicleStatus()
{
    LOGD("SessionVehicleStatus()");
}

SessionVehicleStatus::~SessionVehicleStatus()
{
    LOGD("~SessionVehicleStatus()");
}

int SessionVehicleStatus::handleVehicleStatusCmd(unsigned int cmd, unsigned int handle, unsigned char* pRxData, LinkController* pLnkCtrler)
{
    int result = kIap2NoErr;
    LOGD("handleVehicleStatusCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);

    if (pRxData == NULL) {
        LOGE("    ERROR : pRxData is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR : pLnkCtrler is NULL");
        return kIap2ParamErr;
    }

    switch(cmd) {
        case kCmdVehicleStatusUpdate : {
            VehicleStatusUpdateArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            LOGD("    rcv kCmdVehicleStatusUpdate");
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.range.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    arg.range.bPresent : %u", arg.range.bPresent);
            if(arg.range.bPresent == TRUE) {
                arg.range.value = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
                LOGD("    arg.range.value : %u", arg.range.value);
            }

            arg.outSideTemp.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    arg.outSideTemp.bPresent : %u", arg.outSideTemp.bPresent);
            if( arg.outSideTemp.bPresent == TRUE) {                
                arg.outSideTemp.value = (int16_t)iAp2MarshalGetUint32(&iAp2Marshal);
                LOGD("    arg.outSideTemp.value : %u", arg.outSideTemp.value);
            }
            arg.rangeWarning.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("    arg.rangeWarning.bPresent : %u", arg.rangeWarning.bPresent);
            if( arg.rangeWarning.bPresent == TRUE) {        
                arg.rangeWarning.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
                LOGD("    arg.rangeWarning.value : %y", arg.rangeWarning.value);
            }

            sendVehicleStatusUpdate(pLnkCtrler, &arg);
            break;
        }

        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}


void SessionVehicleStatus::sendVehicleStatusUpdate(LinkController* pLnkCtrler, VehicleStatusUpdateArg* pArg)
{
    LOGD("sendVehicleStatusUpdate()");

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

    createVehicleStatusUpdateParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdVehicleStatusUpdate,
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



void SessionVehicleStatus::createVehicleStatusUpdateParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, VehicleStatusUpdateArg* pArg)
{
    LOGD("createVehicleStatusUpdateParamList()");

    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    LOGD("    Range bPresent[%d] [%d] OutSideTemperature bPresent[%d] [%d] RangeWarning bPresent[%d] [%d]", pArg->range.bPresent, pArg->range.value, pArg->outSideTemp.bPresent, pArg->outSideTemp.value, pArg->rangeWarning.bPresent, pArg->rangeWarning.value);

    if (pArg->range.bPresent == TRUE)
        DataEncoder::appendNumericParam(pArg->range.value, 2, kParamIdVehicleStatusRange, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->outSideTemp.bPresent == TRUE)
        DataEncoder::appendNumericParam(pArg->outSideTemp.value, 2, kParamIdVehicleStatusOutSideTemperature, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->rangeWarning.bPresent == TRUE)
        DataEncoder::appendNumericParam(pArg->rangeWarning.value, 1, kParamIdVehicleStatusRangeWarning, nParamIdList, paramDataList, nParamDataLenList);
}




}// namespace IAP2

