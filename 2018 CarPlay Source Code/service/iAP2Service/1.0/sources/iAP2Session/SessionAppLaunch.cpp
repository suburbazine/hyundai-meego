/*
    File:       SessionAppLaunch.cpp
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


#include "SessionAppLaunch.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

namespace IAP2
{

SessionAppLaunch::SessionAppLaunch()
{
    LOGD("SessionAppLaunch()");
}

SessionAppLaunch::~SessionAppLaunch()
{
    LOGD("~SessionAppLaunch()");
}

int SessionAppLaunch::handleAppLaunchCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    int result = kIap2NoErr;
    LOGD("SessionAppLaunch() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);

    if (pRxData == NULL) {
        LOGE("    ERROR : pRxData is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR : pLnkCtrler is NULL");
        return kIap2ParamErr;
    }

    switch(cmd) {
        case kCmdRequestAppLaunch : {
            RequestAppLaunchArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

            LOGD("    rcv kCmdRequestAppLaunch");
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);
            
            arg.AppBundleID.value =  iAp2MarshalGetBytes(&iAp2Marshal, &arg.AppBundleID.size);    

            LOGD("    arg.AppBundleID.value : %s", arg.AppBundleID.value);
            arg.mLaunchAlert.bPresent =  (BOOL) iAp2MarshalGetUint32(&iAp2Marshal);    
            if(arg.mLaunchAlert.bPresent == TRUE) {
                arg.mLaunchAlert.eLaunchAlert = (LaunchAlert)iAp2MarshalGetUint32(&iAp2Marshal);
            }
            
            sendRequestAppLaunch(pLnkCtrler, &arg);
            break;
        }

        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}

void SessionAppLaunch::sendRequestAppLaunch (LinkController* pLnkCtrler, RequestAppLaunchArg* pArg)
{
    LOGD("sendRequestAppLaunch()");

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

    createRequestAppLaunchParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdRequestAppLaunch, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

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


void SessionAppLaunch::createRequestAppLaunchParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, RequestAppLaunchArg* pArg)
{
    LOGD("createRequestAppLaunchParamList()");

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendBytesParam(pArg->AppBundleID.value, pArg->AppBundleID.size, kParamIDAppBundleID, nParamIdList, paramDataList, nParamDataLenList);

    if (pArg->mLaunchAlert.bPresent == TRUE) {
        DataEncoder::appendNumericParam(pArg->mLaunchAlert.eLaunchAlert , 1, kParamIDLaunchAlert, nParamIdList, paramDataList, nParamDataLenList);
    }
        

}


}// namespace IAP2
