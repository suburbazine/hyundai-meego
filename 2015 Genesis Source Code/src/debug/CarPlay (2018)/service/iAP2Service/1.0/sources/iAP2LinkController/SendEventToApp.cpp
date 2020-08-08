/*
    File:       SendEventToApp.cpp
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

#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "iAp2ServiceCommon.h"
#include "SendEventToApp.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
namespace IAP2
{

SendEventToApp::SendEventToApp(std::vector<int>* pSockFdVector) : mpSockFdVector(pSockFdVector), mServerSockFd(0)
{
    LOGD("SendEventToApp()");
}

SendEventToApp::~SendEventToApp()
{
    LOGD("~SendEventToApp()");
}


int SendEventToApp::sendEvent(int8_t session, uint32_t* pData, unsigned int length)
{
    int result = kIap2NoErr;
    int sendSize = 0;

    int vectorSize = 0;
    //LOGD("SendEvent() session[%d]", session);

    // TODO : should handle multi-application.
    // AppContext* = getAppContxt(session);
    AppContext appContext;
    appContext.appHandle = 0x12345678;

    vectorSize = mpSockFdVector->size();

    (void) session;
        
    if (vectorSize < 1) {
        LOGE("    ===> ERROR : vectorSize[%d] is 0", vectorSize);
        return kIap2UnknownErr;
    }

    if (vectorSize < 2) {
        LOGE("    ===> sendEvent() any client is not connected");
        return kIap2NoErr;
    }
    
    if (pData == NULL) {
        LOGE("ERROR : inArg is NULL");
        return kIap2ParamErr;
    }

    if (length == 0) {
        LOGE("ERROR : length is 0");
        return kIap2ParamErr;
    }
    
    pData[1] = appContext.appHandle;
    
      for (unsigned int i = 0; i < mpSockFdVector->size(); i++) {
        appContext.sockFd = mpSockFdVector->at(i);
        
        if (appContext.sockFd == -1) {
                LOGE("ERROR : sockFd is invalid");
                continue;
        }
        if (mServerSockFd != appContext.sockFd) {
        
            //LOGD("    appContext.sockFd [%d]", appContext.sockFd);
            sendSize = send(appContext.sockFd, (char*) pData, length, MSG_DONTWAIT);
        
            //LOGD("pData[%p] length[%d] sendSize[%p]", pData, length, sendSize);
            if (sendSize == -1) {
                LOGE("    WARNING : socket send failed. sockFd[%d] error[%d][%s]", appContext.sockFd, errno, strerror(errno));
                result = kIap2WriteErr;
            }
        }
    }
    return result;

}

int SendEventToApp::sendData(int fd, uint32_t* pData, unsigned int length)
{
    int result = kIap2NoErr;
    int sendSize = 0;

    if (pData == NULL) {
        LOGE("ERROR : inArg is NULL");
        return kIap2ParamErr;
    }

    if (length == 0) {
        LOGE("ERROR : length is 0");
        return kIap2ParamErr;
    }

    sendSize = send(fd, (char*) pData, length, MSG_DONTWAIT);
    
    if (sendSize == -1) {
        LOGE("    ERROR : socket send failed. fd[%d] error[%d][%s]", fd, errno, strerror(errno));
        result = kIap2WriteErr;
    }
    return result;

}


}// namespace IAP2
