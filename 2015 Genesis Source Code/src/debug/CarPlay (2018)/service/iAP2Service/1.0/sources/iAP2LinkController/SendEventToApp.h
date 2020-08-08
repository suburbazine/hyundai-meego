/*
    File:       SendEventToApp.h
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

#ifndef SENDEVENTTOAPP_H_
#define SENDEVENTTOAPP_H_

#include <pthread.h>
#include <string>
#include <vector>

#include "ByteArray.h"
#include "iAP2LinkRunLoop.h"
namespace IAP2
{
    struct AppContext {
        unsigned int appHandle;
        int sockFd;
    };
    
    class SendEventToApp {
        private : 
            std::vector<int>* mpSockFdVector;
            
        public:
            int mServerSockFd;
            
        private:
            

        public:
            SendEventToApp(std::vector<int>* pSockFdVector);
            ~SendEventToApp();
            void* serverThread(void* data);
            int sendEvent(int8_t session, uint32_t* pData, unsigned int length);
            int sendData(int fd, uint32_t* pData, unsigned int length);
    };

} // namespace IAP2

#endif // #ifdef SENDEVENTTOAPP_H_

