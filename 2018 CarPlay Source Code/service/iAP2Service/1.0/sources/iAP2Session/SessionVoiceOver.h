/*
    File:       SessionVoiceOver.h
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

#ifndef SESSIONVOICEOVER_H_
#define SESSIONVOICEOVER_H_

#include "LinkController.h"
#include <iAp2Marshal.h>
namespace IAP2
{
    class SessionVoiceOver
    {
        public :
            SessionVoiceOver();
            ~SessionVoiceOver();

              int handleVoiceOverCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler);
            
        private:
            void sendStartVoiceOver(LinkController* pLnkCtrler);
            void sendStopVoiceOver(LinkController* pLnkCtrler);
            void sendRequestVoiceOverMoveCursor(LinkController* pLnkCtrler, RequestVoiceOverMoveCursorArg* pArg);
            void sendRequestVoiceOverActivateCursor(LinkController* pLnkCtrler);
            void sendRequestVoiceOverScrollPage(LinkController* pLnkCtrler, RequestVoiceOverScrollPageArg* pArg);
            void sendRequestVoiceOverSpeakText(LinkController* pLnkCtrler, RequestVoiceOverSpeakTextArg* pArg);
            void sendRequestVoiceOverPauseText(LinkController* pLnkCtrler);
            void sendRequestVoiceOverResumeText(LinkController* pLnkCtrler);
            void sendStartVoiceOverUpdates(LinkController* pLnkCtrler, StartVoiceOverUpdatesArg* pArg);
            void sendStopVoiceOverUpdates(LinkController* pLnkCtrler);
            void sendRequestVoiceOverConfiguration(LinkController* pLnkCtrler, RequestVoiceOverConfigurationArg* pArg);
            void sendStartVoiceOverCursorUpdates(LinkController* pLnkCtrler, StartVoiceOverCursorUpdatesArg* pArg);
            void sendStopVoiceOverCursorUpdates(LinkController* pLnkCtrler);

            void createRequestVoiceOverMoveCursorParamList(std::vector<int>& nParamIdList,
                                                                     std::vector<const uint8_t *>& paramDataList,
                                                                     std::vector<uint32_t>& nParamDataLenList,
                                                                     RequestVoiceOverMoveCursorArg* pArg);    
            void createRequestVoiceOverScrollPageParamList(std::vector<int>& nParamIdList,
                                                                     std::vector<const uint8_t *>& paramDataList,
                                                                     std::vector<uint32_t>& nParamDataLenList,
                                                                     RequestVoiceOverScrollPageArg* pArg);    
            void createRequestVoiceOverSpeakTextParamList(std::vector<int>& nParamIdList,
                                                                     std::vector<const uint8_t *>& paramDataList,
                                                                     std::vector<uint32_t>& nParamDataLenList,
                                                                     RequestVoiceOverSpeakTextArg* pArg);    
            void createStartVoiceOverUpdatesParamList(std::vector<int>& nParamIdList,
                                                                     std::vector<const uint8_t *>& paramDataList,
                                                                     std::vector<uint32_t>& nParamDataLenList,
                                                                     StartVoiceOverUpdatesArg* pArg);    
            void createRequestVoiceOverConfigurationParamList(std::vector<int>& nParamIdList,
                                                                     std::vector<const uint8_t *>& paramDataList,
                                                                     std::vector<uint32_t>& nParamDataLenList,
                                                                     RequestVoiceOverConfigurationArg* pArg);    
            void createStartVoiceOverCursorUpdatesParamList(std::vector<int>& nParamIdList,
                                                                     std::vector<const uint8_t *>& paramDataList,
                                                                     std::vector<uint32_t>& nParamDataLenList,
                                                                     StartVoiceOverCursorUpdatesArg* pArg);

    };

} // namespace IAP2

#endif // #ifdef SESSIONVOICEOVER_H_

