/*
    File:       DeviceNotification.h
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

#ifndef DEVICENOTIFICATION_H_
#define DEVICENOTIFICATION_H_

#include "iAP2LinkRunLoop.h"
#include "DataDecoder.h"
#include "DataEncoder.h"
#include "Transport.h"
#include <iAp2Marshal.h>

namespace IAP2
{

    class LinkController;
    class DeviceNotification
    {
        public:
            DeviceNotification();
            ~DeviceNotification();

            void handleDeviceInformationUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData);
            void handleDeviceLanguageUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData);
        

        private:
            int handleDeviceInformationUpdateEvent(LinkController* pLnkCtrler, int8_t session, DeviceInformationUpdateEvt* pEvent);
            int handleDeviceLanguageUpdateEvent(LinkController* pLnkCtrler, int8_t session, DeviceLanguageUpdateEvt* pEvent);
            void decodeDeviceInformationUpdates(uint8_t* pData, DeviceInformationUpdateEvt* pEvt);
            void decodeDeviceLanguageUpdates(uint8_t* pData, DeviceLanguageUpdateEvt* pEvt);

        public :        
    };


} // namespace IAP2

#endif // #ifndef DEVICENOTIFICATION_H_


