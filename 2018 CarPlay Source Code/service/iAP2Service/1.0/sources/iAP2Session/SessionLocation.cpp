/*
    File:       SessionLocation.cpp
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

#include "SessionLocation.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

namespace IAP2
{

SessionLocation::SessionLocation()
{
    LOGD("SessionLocation()");
}

SessionLocation::~SessionLocation()
{
    LOGD("~SessionLocation()");
}

int SessionLocation::handleLocationCmd(unsigned int cmd, unsigned int handle, unsigned char* pRxData, LinkController* pLnkCtrler)
{
    LOGD("handleLocationCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);
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
        case kCmdLocationInformation : {
            LocationInformationArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.gpsFixData.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.gpsFixData.bPresent == TRUE) {
                arg.gpsFixData.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.gpsFixData.size);
                //LOGD("arg.gpsFixData.value : %s, size %d", arg.gpsFixData.value, arg.gpsFixData.size);
            }                
            
            arg.recommMinSpecificGPSTransitData.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.recommMinSpecificGPSTransitData.bPresent == TRUE) {
                arg.recommMinSpecificGPSTransitData.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.recommMinSpecificGPSTransitData.size);
                //LOGD("arg.recommMinSpecificGPSTransitData.value : %s, size %d", arg.recommMinSpecificGPSTransitData.value, arg.recommMinSpecificGPSTransitData.size);
            }    
            
            arg.gpsSatellitesInView.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.gpsSatellitesInView.bPresent == TRUE) {
                arg.gpsSatellitesInView.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.gpsSatellitesInView.size);
                //LOGD("arg.gpsSatellitesInView.value : %s, size %d", arg.gpsSatellitesInView.value, arg.gpsSatellitesInView.size);
            }    
        
            arg.vehicleSpeedData.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.vehicleSpeedData.bPresent == TRUE) {
                arg.vehicleSpeedData.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.vehicleSpeedData.size);
                //LOGD("arg.vehicleSpeedData.value : %s, size %d", arg.vehicleSpeedData.value, arg.vehicleSpeedData.size);
            }
    
            arg.vehicleGyroData.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.vehicleGyroData.bPresent == TRUE) {
                arg.vehicleGyroData.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.vehicleGyroData.size);
                //LOGD("arg.vehicleGyroData.value : %s, size %d", arg.vehicleGyroData.value, arg.vehicleGyroData.size);
            }

            arg.vehicleAccelData.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.vehicleAccelData.bPresent == TRUE) {
                arg.vehicleAccelData.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.vehicleAccelData.size);
                //LOGD("arg.vehicleAccelData.value : %s, size %d", arg.vehicleAccelData.value, arg.vehicleAccelData.size);
            }

            arg.vehicleHeadingData.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.vehicleHeadingData.bPresent == TRUE) {
                arg.vehicleHeadingData.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.vehicleHeadingData.size);
                //LOGD("arg.vehicleHeadingData.value : %s, size %d", arg.vehicleHeadingData.value, arg.vehicleHeadingData.size);
            }

            sendLocationInformation(pLnkCtrler, &arg);
            break;
        }

        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}

void SessionLocation::sendLocationInformation(LinkController* pLnkCtrler, LocationInformationArg* pArg)
{
    LOGD("sendLocationInformation()");


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

    createLocationInformationParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdLocationInformation,
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



void SessionLocation::createLocationInformationParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, LocationInformationArg* pArg)
{
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

//    LOGD("    Range bPresent[%d] OutSideTemperature bPresent[%d] RangeWarning bPresent[%d]", pArg->range.bPresent, pArg->outSideTemp.bPresent, pArg->rangeWarning.bPresent);

    if (pArg->gpsFixData.bPresent == TRUE) {
        //LOGD("pArg->gpsFixData.value : :%s, size : %d", pArg->gpsFixData.value, pArg->gpsFixData.size);
        DataEncoder::appendBytesParam(pArg->gpsFixData.value, pArg->gpsFixData.size, kParamIdComponentID, nParamIdList, paramDataList, nParamDataLenList);
    }

    if (pArg->recommMinSpecificGPSTransitData.bPresent == TRUE) {
        //LOGD("pArg->recommMinSpecificGPSTransitData.value : :%s, size : %d", pArg->recommMinSpecificGPSTransitData.value, pArg->recommMinSpecificGPSTransitData.size);
        DataEncoder::appendBytesParam(pArg->recommMinSpecificGPSTransitData.value, pArg->recommMinSpecificGPSTransitData.size, kParamIdComponentID, nParamIdList, paramDataList, nParamDataLenList);
    }

    if (pArg->gpsSatellitesInView.bPresent == TRUE) {
        //LOGD("pArg->gpsSatellitesInView.value : :%s, size : %d", pArg->gpsSatellitesInView.value, pArg->gpsSatellitesInView.size);
        DataEncoder::appendBytesParam(pArg->gpsSatellitesInView.value, pArg->gpsSatellitesInView.size, kParamIdComponentID, nParamIdList, paramDataList, nParamDataLenList);
    }
    
    if (pArg->vehicleSpeedData.bPresent == TRUE) {
        //LOGD("pArg->vehicleSpeedData.value : :%s, size : %d", pArg->vehicleSpeedData.value, pArg->vehicleSpeedData.size);
        DataEncoder::appendBytesParam(pArg->vehicleSpeedData.value, pArg->vehicleSpeedData.size, kParamIdComponentID, nParamIdList, paramDataList, nParamDataLenList);
    }
    
    if (pArg->vehicleGyroData.bPresent == TRUE) {
        //LOGD("pArg->vehicleGyroData.value : :%s, size : %d", pArg->vehicleGyroData.value, pArg->vehicleGyroData.size);
        DataEncoder::appendBytesParam(pArg->vehicleGyroData.value, pArg->vehicleGyroData.size, kParamIdComponentID, nParamIdList, paramDataList, nParamDataLenList);
    }
    
    if (pArg->vehicleAccelData.bPresent == TRUE) {
        //LOGD("pArg->vehicleAccelData.value : :%s, size : %d", pArg->vehicleAccelData.value, pArg->vehicleAccelData.size);
        DataEncoder::appendBytesParam(pArg->vehicleAccelData.value, pArg->vehicleAccelData.size, kParamIdComponentID, nParamIdList, paramDataList, nParamDataLenList);
    }
    
    if (pArg->vehicleHeadingData.bPresent == TRUE) {
        //LOGD("pArg->vehicleHeadingData.value : :%s, size : %d", pArg->vehicleHeadingData.value, pArg->vehicleHeadingData.size);
        DataEncoder::appendBytesParam(pArg->vehicleHeadingData.value, pArg->vehicleHeadingData.size, kParamIdComponentID, nParamIdList, paramDataList, nParamDataLenList);
    }
    
}

}// namespace IAP2


