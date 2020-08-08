/*
    File:       Power.h
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

#ifndef POWER_H_
#define POWER_H_

#include "iAP2LinkRunLoop.h"
//#include "Certificate.h"
#include "DataDecoder.h"
#include "DataEncoder.h"
#include "Transport.h"

namespace IAP2
{

    class LinkController;
    class Power
    {
        public:
            Power();
            ~Power();

            void handlePowerUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData);
            void sendStartPowerUpdates(LinkController* pLnkCtrler, StartPowerUpdateArg* pArg);
            void sendPowerSourceUpdate(LinkController* pLnkCtrler, PowerSourceUpdateArg* pArg);

        private:
            void decodePowerUpdates(uint8_t* pData, PowerUpdateEvt* pEvt);
            
            int handlePowerUpdateEvent(LinkController* pLnkCtrler, int8_t session, PowerUpdateEvt* pEvent);
            void createStartPowerUpdatesParamList(std::vector<int>& nParamIdList,
                                                                     std::vector<const uint8_t *>& paramDataList,
                                                                     std::vector<uint32_t>& nParamDataLenList,
                                                                     StartPowerUpdateArg* pArg);
            void createPowerSourceUpdateParamList(std::vector<int>& nParamIdList,
                                                                     std::vector<const uint8_t *>& paramDataList,
                                                                     std::vector<uint32_t>& nParamDataLenList,
                                                                     PowerSourceUpdateArg* pArg);    

    };


} // namespace IAP2

#endif // #ifndef POWER_H_
