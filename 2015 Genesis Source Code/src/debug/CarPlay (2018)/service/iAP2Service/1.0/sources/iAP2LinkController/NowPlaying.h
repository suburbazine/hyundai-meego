/*
    File:       NowPlaying.h
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

#ifndef NOWPLAYING_H_
#define NOWPLAYING_H_

#include "iAP2LinkRunLoop.h"
#include "DataDecoder.h"
#include "DataEncoder.h"
#include "Transport.h"
// #ifdef iAP2SERVICE_FILETRANSFER
#include "iAP2FileTransfer.h"
// #endif
#include <iAp2Marshal.h>
namespace IAP2
{

    class LinkController;
    class NowPlaying {

        public :
            NowPlaying();
            ~NowPlaying();

        public :
            
            void handleNowPlayingUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData);
//#ifdef iAP2SERVICE_FILETRANSFER
//            void handleNowPlayingAlbumArtworkUpdate(iAP2FileTransfer_st* fileXfer, void* userInfo);
 //           void handleNowPlayingAlbumArtworkUpdate(iAP2FileTransfer_st* fileXfer);
//#endif

        private:
            int handleNowPlayingUpdateEvent(LinkController* pLnkCtrler, int8_t session, NowPlayingUpdateEvt* pEvent);
            //#ifdef iAP2SERVICE_FILETRANSFER
//            int handleNowPlayingAlbumArtworkUpdateEvent(LinkController* pLnkCtrler, int8_t session, NowPlayingAlbumArtworkEvt* pEvent);
            //#endif

    
            
            void decodeNowPlayingUpdates(uint8_t* pData, NowPlayingUpdateEvt* pEvt);
            void decodeNowPlayingUpdateMediaItemAttributes(uint8_t* pData, NowPlayingUpdateEvt* pEvt, int nSubParamId,  int nSubParamStartIdx, int nSubParamLength);
            void decodeNowPlayingUpdatePlaybackAttributes(uint8_t* pData, NowPlayingUpdateEvt* pEvt, int nSubParamId,  int nSubParamStartIdx, int nSubParamLength);            

// #ifdef iAP2SERVICE_FILETRANSFER
 //           void decodeNowPlayingAlbumArtworkUpdate(iAP2FileTransfer_st* fileXfer, NowPlayingAlbumArtworkEvt* pEvt);
//#endif

             void ParsingNowPlayingUpdateEventpMiAttrResGr(MediaItemAttrResGroup* pMiAttrResGr, MarshalIo* iAp2Marshal);
            void ParsingNowPlayingUpdateEventpPbAttrResGr(PlaybackAttrResGroup* pMiAttrResGr, MarshalIo* iAp2Marshal);
            int getMediItemTypeCount(uint8_t* pData, int nParamStartIdx);
    };


} // namespace IAP2

#endif // #ifndef NOWPLAYING_H_
