/*
    File:       Connection.h
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

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "iAP2LinkRunLoop.h"
#include "Transport.h"
#include "iAp2AppFrameworkExternal.h"
#include "iAp2AppFrameworkExternal_command.h"
#include "iAp2AppFrameworkExternal_event.h"
#include <iAp2Marshal.h>

namespace IAP2
{

    class LinkController;
    class Connection
    {
        private:          
            ConnectionIap2Support mIap2Support;
            

        public:
            ConnectionPath mConnPath;
            ConnectionMode mConnMode;
            ConnectionCarPlaySupport mCarplaySupport;
            BOOL bAuthentication;
            BOOL bIdentification;
            BOOL CarplayOnoff;
            ConnectionMode mRequestUSBMode;
            CarPlayClientState mCarPlayClientState;
            ConnectionState mConnState;
            
        public:
            Connection();
            ~Connection();
            
            

            void handleConnectionStateUpdate(LinkController* pLnkCtrler, ConnectionState connState);
            void handleConnectionStateAndPathUpdate(LinkController* pLnkCtrler, ConnectionState connState, ConnectionPath connPath);
            void handleConnectionModeUpdate(LinkController* pLnkCtrler, ConnectionMode connMode);
            void handleConnectionCarplaySupportUpdate(LinkController* pLnkCtrler, ConnectionCarPlaySupport carplaySupport);
            void handleConnectionIap2SupportUpdate(LinkController* pLnkCtrler, ConnectionIap2Support iap2Support);
#ifdef ANDROID    
            void handleConnectionDeviceInfoUpdate(LinkController* pLnkCtrler, int productId, int vendorId, char *product);
#endif

               
            int handleConnectionStateUpdateEvent(LinkController* pLnkCtrler,ConnectionStateUpdateEvt* pEvent);
            int handleConnectionStateAndPathUpdateEvent(LinkController* pLnkCtrler,ConnectionStateAndPathUpdateEvt* pEvent);
            int handleConnectionModeUpdateEvent(LinkController* pLnkCtrler,ConnectionModeUpdateEvt* pEvent);
            int handleConnectionCarplaySupportUpdateEvent(LinkController* pLnkCtrler, ConnectionCarplaySupportUpdateEvt* pEvent);
            int handleConnectionIap2SupportUpdateEvent(LinkController* pLnkCtrler, ConnectionIap2SupportUpdateEvt* pEvent);
            #ifdef ANDROID
            int handleConnectionDeviceInfoUpdateEvent(LinkController* pLnkCtrler, ConnectionDeviceInfoUpdateEvt* pEvent);
            #endif

            int CmdGetInitConnectionData(LinkController* pLnkCtrler, int currentFd, uint8_t* pRxData );
        private:
            
            void getInitConnectionData(InitConnectionData* pOutArg);

            
    };


} // namespace IAP2

#endif // #ifndef CONNECTION_H_
