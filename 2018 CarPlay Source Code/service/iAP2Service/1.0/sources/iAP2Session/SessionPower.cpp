/*
    File:       SessionPower.cpp
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

//#include "iAp2ServiceCommon.h"
#include "SessionPower.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

namespace IAP2
{

SessionPower::SessionPower()
{
    LOGD("SessionPower()");
}

SessionPower::~SessionPower()
{
    LOGD("~SessionPower()");
}

int SessionPower::handlePowerCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    LOGD("handlePowerCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);
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
        case kCmdStartPowerUpdates : {
            StartPowerUpdateArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.bMaxCurrentDrawnFromAcc = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bDevBatteryCharge = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bAccPowerMode = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bIsExternalChargerConnected = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bBatteryChargingState = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bBatteryChargeLevel = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

            sendStartPowerUpdates(pLnkCtrler, &arg);
            break;
        }

        case kCmdStopPowerUpdates : {            
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            iAp2MarshalGetUint32(&iAp2Marshal);

            sendStopPowerUpdates(pLnkCtrler);
            break;
        }

        case kCmdPowerSourceUpdate : {
            PowerSourceUpdateArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.availCurrent.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if(arg.availCurrent.bPresent == TRUE) {
                arg.availCurrent.value= (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
            }

            
            arg.devBatteryCharge.bPresent= (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if( arg.devBatteryCharge.bPresent == TRUE) {
                arg.devBatteryCharge.value= (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            }

            sendPowerSourceUpdate(pLnkCtrler, &arg);
            break;
        }

        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}


void SessionPower::sendStartPowerUpdates(LinkController* pLnkCtrler, StartPowerUpdateArg* pArg)
{
        LOGD("sendStartPowerUpdates()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

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

    createStartPowerUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartPowerUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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

void SessionPower::sendPowerSourceUpdate(LinkController* pLnkCtrler, PowerSourceUpdateArg* pArg)
{
    LOGD("sendPowerSourceUpdate()");
    
    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

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

    createPowerSourceUpdateParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdPowerSourceUpdate,
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


void SessionPower::sendStopPowerUpdates(LinkController* pLnkCtrler)
{
    LOGD("sendStopPowerUpdates()");

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

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopPowerUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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



void SessionPower::createStartPowerUpdatesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StartPowerUpdateArg* pArg)
{
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    if (pArg->bMaxCurrentDrawnFromAcc == TRUE)
        DataEncoder::appendNoneParam(kParamIdPowerMaximumCurrentDrawnFromAccessory, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->bDevBatteryCharge == TRUE)
        DataEncoder::appendNoneParam(kParamIdPowerDeviceBatteryWillChargeIfPowerIsPresent, nParamIdList, paramDataList, nParamDataLenList);

    // In general, below parameter is not used.
    if (pArg->bAccPowerMode == TRUE)
        DataEncoder::appendNoneParam(kParamIdPowerAccessoryPowerMode, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->bIsExternalChargerConnected == TRUE)
        DataEncoder::appendNoneParam(kParamIdIsExternalChargerConnected, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->bBatteryChargingState == TRUE)
        DataEncoder::appendNoneParam(kParamIdBatteryChargingState, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->bBatteryChargeLevel == TRUE)
        DataEncoder::appendNoneParam(kParamIdBatteryChargeLevel, nParamIdList, paramDataList, nParamDataLenList);
    


}

void SessionPower::createPowerSourceUpdateParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, PowerSourceUpdateArg* pArg)
{
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();
    
    if (pArg->availCurrent.bPresent == TRUE)
        DataEncoder::appendNumericParam(pArg->availCurrent.value, 2, kParamIdPowerAvailableCurrentForDevice, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->devBatteryCharge.bPresent == TRUE)
        DataEncoder::appendNumericParam(pArg->devBatteryCharge.value, 1, kParamIdPowerDeviceBatteryShouldChargeIfPowerIsPresent, nParamIdList, paramDataList, nParamDataLenList);

}


}// namespace IAP2
