/*
    File:       CmdHandler.h
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

#ifndef CMDHANDLER_H_
#define CMDHANDLER_H_

#include <pthread.h>
#include <string>
#include <vector>

#include "ByteArray.h"
#include "iAP2LinkRunLoop.h"

#include "SessionConnection.h"
#include "SessionPower.h"
#include "SessionNowPlaying.h"
#include "SessionTelephony.h"
#include "SessionLocation.h"
#include "SessionMediaLibraryInformation.h"
#include "SessionMediaLibraryUpdates.h"
#include "SessionPlayMediaLibrary.h"
#include "SessionHID.h"
#include "SessionVehicleStatus.h"
#include "SessionBTConnection.h"
#include "SessionAppLaunch.h"
#include "SessionAssistiveTouch.h"
#include "SessionUSBDeviceModeAudio.h"
#include "SessionVoiceOver.h"
#include "SessionWiFiInformation.h"
#include "SessionExternalAccessoryProtocol.h"
#include "SessionIdentification.h"
#include "SessionCommunications.h"

namespace IAP2
{
        class CmdHandler {
        private : 
            int mTid;
            bool mIsTerminated;
            int mServerSockFd;
            // mSockFdVector includes both server socket fd and client socket fds
            // This condition and mutex is used for session server thread(serverThread).
            pthread_cond_t mCondSsThSync;
            pthread_mutex_t mMutexSsThSync;
            std::vector<int>* mpSockFdVector;
        
        public:
            pthread_t mThread;
            LinkController* miAP2LnkCtrler;    
            
            SessionConnection mSessionConn;
            SessionPower mSessionPower;
            SessionNowPlaying mSessionNowPlaying;
            SessionTelephony mSessionTelephony;
            SessionLocation mSessionLocation;
            SessionMeidaLibraryInformation mSessionMeidaLibraryInformation;
            SessionMeidaLibraryUpdates mSessionMeidaLibraryUpdates;
            SessionPlayMediaLibrary mSessionPlayMediaLibrary;
            SessionHID mSessionHID;
            SessionVehicleStatus mSessionVehicleStatus;                
            SessionBTConnection mSessionBTConnection;
            SessionAppLaunch mSessionAppLaunch;
            SessionAssistiveTouch mSessionAssistiveTouch;
            SessionUSBDeviceModeAudio mSessionUSBDeviceModeAudio;
            SessionVoiceOver mSessionVoiceOver;
            SessionWiFiInformation mSessionWiFiInformation;
            SessionExternalAccessoryProtocol mSessionExternalAccessoryProtocol;
            SessionIdentification mSessionIdentification;        
            SessionCommunications mSessionCommunications;
          
            

        private:
            int listenClient();
            int handleClientCmd(int currentFd, uint32_t cmd, uint32_t handle, uint8_t* pRxData);
            void setReuseAddressOption(int sockFd);  
            void setLingerOption(int sockFd);

        public:
            CmdHandler(LinkController* pLinkController, std::vector<int>* pSockFdVector);
            ~CmdHandler();
            void* serverThread(void* data);
            //int sendEvent(int8_t session, uint32_t* pData, unsigned int length);
            //int sendData(int fd, uint32_t* pData, unsigned int length);
    };

} // namespace IAP2

#endif // #ifdef CMDHANDLER_H_

