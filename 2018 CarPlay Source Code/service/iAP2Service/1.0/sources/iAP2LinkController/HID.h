/*
    File:       HID.h
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

#ifndef HID_H_
#define HID_H_

#include "iAP2LinkRunLoop.h"
//#include "Certificate.h"
#include "DataDecoder.h"
#include "DataEncoder.h"
#include "Transport.h"
#include <iAp2Marshal.h>
namespace IAP2
{

    class LinkController;
    class HID
    {
        public:
            HID();
            ~HID();

            
               void handleDeviceHIDReport(LinkController* pLnkCtrler, int8_t session, uint8_t* pData);

        private:
            int handleDeviceHIDReportEvent(LinkController* pLnkCtrler, int8_t session, DeivceHIDReportEvt* pEvent);
            void decodeDeviceHIDReport(uint8_t* pData, DeivceHIDReportEvt* pEvt);
           
    };


} // namespace IAP2

#endif // #ifndef LocationInformation_H_

