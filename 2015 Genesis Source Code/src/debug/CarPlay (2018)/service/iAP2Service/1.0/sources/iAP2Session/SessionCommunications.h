/*
    File:       SessionCommunications.h
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

#ifndef SESSIONCOMMUNICATIONS_H_
#define SESSIONCOMMUNICATIONS_H_

#include "LinkController.h"
#include <iAp2Marshal.h>
namespace IAP2
{
    class SessionCommunications
    {
        public :
            SessionCommunications();
            ~SessionCommunications();

            int handleCommunicationsCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler);


        private:            
            void sendStartCallStateUpdates(LinkController* pLnkCtrler, StartCallStateUpdatesArg* pArg);
            void sendStopCallStateUpdates(LinkController* pLnkCtrler);
            void sendStartCommunicationsUpdates(LinkController* pLnkCtrler, StartCommunicationsUpdatesArg* pArg);
            void sendStopCommunicationsUpdates(LinkController* pLnkCtrler);
            void sendInitialCall(LinkController* pLnkCtrler, InitiateCallArg* pArg);
            void sendAcceptCall(LinkController* pLnkCtrler, AcceptCallArg* pArg);
            void sendEndCall(LinkController* pLnkCtrler, EndCallArg* pArg);
            void sendSwapCalls(LinkController* pLnkCtrler);
            void sendMergeCalls(LinkController* pLnkCtrler);
            void sendHoldStatusUpdate(LinkController* pLnkCtrler, HoldStatusUpdateArg* pArg);
            void sendMuteStatusUpdate(LinkController* pLnkCtrler, MuteStatusUpdateArg* pArg);
            void sendSendDTMF(LinkController* pLnkCtrler, SendDTMFArg* pArg);
            void sendStartListUpdates(LinkController* pLnkCtrler, StartListUpdatesArg* pArg);
            void sendStopListUpdates(LinkController* pLnkCtrler);
            void createStartCallStateUpdatesParamList(std::vector<int> &nParamIdList, 
                                                      std::vector<const uint8_t *> &paramDataList, 
                                                      std::vector<uint32_t> &nParamDataLenList, 
                                                      StartCallStateUpdatesArg* pArg);

            void createStartcommunicationsUpdatesParamList(std::vector<int> &nParamIdList, 
                                                      std::vector<const uint8_t *> &paramDataList, 
                                                      std::vector<uint32_t> &nParamDataLenList, 
                                                      StartCommunicationsUpdatesArg* pArg);
            void createInitialCallParamList(std::vector<int> &nParamIdList, 
                                                      std::vector<const uint8_t *> &paramDataList, 
                                                      std::vector<uint32_t> &nParamDataLenList, 
                                                      InitiateCallArg* pArg);
            void createAcceptCallParamList(std::vector<int> &nParamIdList, 
                                                      std::vector<const uint8_t *> &paramDataList, 
                                                      std::vector<uint32_t> &nParamDataLenList, 
                                                      AcceptCallArg* pArg);
            void createEndCallParamList(std::vector<int> &nParamIdList, 
                                                      std::vector<const uint8_t *> &paramDataList, 
                                                      std::vector<uint32_t> &nParamDataLenList, 
                                                      EndCallArg* pArg);
            void createHoldStatusUpdateParamList(std::vector<int> &nParamIdList, 
                                                      std::vector<const uint8_t *> &paramDataList, 
                                                      std::vector<uint32_t> &nParamDataLenList, 
                                                      HoldStatusUpdateArg* pArg);
            void createMuteStatusUpdateParamList(std::vector<int> &nParamIdList, 
                                                      std::vector<const uint8_t *> &paramDataList, 
                                                      std::vector<uint32_t> &nParamDataLenList, 
                                                      MuteStatusUpdateArg* pArg);
            void createSendDTMFParamList(std::vector<int> &nParamIdList, 
                                                      std::vector<const uint8_t *> &paramDataList, 
                                                      std::vector<uint32_t> &nParamDataLenList, 
                                                      SendDTMFArg* pArg);
            void createStartListUpdatesParamList(std::vector<int> &nParamIdList, 
                                                      std::vector<const uint8_t *> &paramDataList, 
                                                      std::vector<uint32_t> &nParamDataLenList, 
                                                      StartListUpdatesArg* pArg);
            
            uint32_t createGroupParamBytesForRecentsListProperties(uint8_t** encodedBytes, StartListUpdatesArg* pArg);
            uint32_t createGroupParamBytesForFavoritesListProperties(uint8_t** encodedBytes, StartListUpdatesArg* pArg);
            
            void createRecentsListPropertiesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                                              std::vector<uint32_t> &nParamDataLenList, StartListUpdatesArg* pArg); 
            void createFavoritesListPropertiesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                                              std::vector<uint32_t> &nParamDataLenList, StartListUpdatesArg* pArg);
            
    };

} // namespace IAP2

#endif // #ifdef SESSIONCOMMUNICATIONS_H_


