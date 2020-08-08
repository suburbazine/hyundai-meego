/*
    File:       SessionNowPlaying.h
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

#ifndef SESSIONNOWPLAYING_H_
#define SESSIONNOWPLAYING_H_

#include "LinkController.h"
#include <iAp2Marshal.h>
namespace IAP2
{
    class SessionNowPlaying
    {
        public :
            SessionNowPlaying();
            ~SessionNowPlaying();

        int handleNowPlayingCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler);
    
            
        
        private:
        void sendStartNowPlayingUpdates(LinkController* pLnkCtrler, StartNowPlayingArg* pArg);
        void sendStopNowPlayingUpdates(LinkController* pLnkCtrler);
        void sendSetNowPlayingInformation(LinkController* pLnkCtrler, SetNowPlayingInformationArg* pArg);
            
        void createStartNowPlayingUpdatesParamList(std::vector<int>& nParamIdList,
                                                                 std::vector<const uint8_t *>& paramDataList,
                                                                 std::vector<uint32_t>& nParamDataLenList,
                                                                 StartNowPlayingArg* pArg);

        uint32_t createGroupParamBytesForPlaybackAttributes(uint8_t** encodedBytes, StartNowPlayingArg* pArg);
        uint32_t createGroupParamBytesForMediaItemAttributes(uint8_t** encodedBytes, StartNowPlayingArg* pArg);
        void createMediaItemAttributesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                                                      std::vector<uint32_t> &nParamDataLenList, StartNowPlayingArg* pArg);
        void createPlaybackAttributesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                                                          std::vector<uint32_t> &nParamDataLenList, StartNowPlayingArg* pArg);
            
         void ParsingNowPlayingUpdateCmdmediaAttrReqGr(MarshalIo* iAp2Marshal, StartNowPlayingArg* arg);
        void ParsingNowPlayingUpdateCmdplaybackAttrReqGr(MarshalIo* iAp2Marshal, StartNowPlayingArg* arg);
        
        void createSetNowPlayingInformationParamList(std::vector<int> &nParamIdList, 
                                                        std::vector<const uint8_t *> &paramDataList,
                                                        std::vector<uint32_t> &nParamDataLenList, 
                                                        SetNowPlayingInformationArg* pArg);
    };

} // namespace IAP2

#endif // #ifdef SESSIONNOWPLAYING_

