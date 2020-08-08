/*
    File:       ExternalAccessoryProtocol.h
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

#ifndef EXTERNALACCESSORYPROTOCOL_H_
#define  EXTERNALACCESSORYPROTOCOL_H_

#include "iAP2LinkRunLoop.h"
//#include "Certificate.h"
#include "DataDecoder.h"
#include "DataEncoder.h"
#include "Transport.h"
#include <iAp2Marshal.h>
namespace IAP2
{

    class LinkController;
    class ExternalAccessoryProtocol
    {
        public:
            ExternalAccessoryProtocol();
            ~ExternalAccessoryProtocol();
 
            void handleStartExternalAccessoryProtocolSession(LinkController* pLnkCtrler, int8_t session, uint8_t* pData);
            void handleStopExternalAccessoryProtocolSession(LinkController* pLnkCtrler, int8_t session, uint8_t* pData);
            int handleExternalAccessoryProtocolRecvData(LinkController* pLnkCtrler, int8_t session, uint8_t* data, uint32_t dataLen);

        private:
            void decodeStartExternalAccessoryProtocolSession(uint8_t* pData, StartExternalAccessoryProtocolSessionEvt* pEvt);
            void decodeStopExternalAccessoryProtocolSession(uint8_t* pData, StopExternalAccessoryProtocolSessionEvt* pEvt);
            int handleStartExternalAccessoryProtocolSessionEvent(LinkController* pLnkCtrler, int8_t session, StartExternalAccessoryProtocolSessionEvt* pEvent);
            int handleStopExternalAccessoryProtocolSessionEvent(LinkController* pLnkCtrler, int8_t session, StopExternalAccessoryProtocolSessionEvt* pEvent);
    };


} // namespace IAP2

#endif // #ifndef EXTERNALACCESSORYPROTOCOL_H_

