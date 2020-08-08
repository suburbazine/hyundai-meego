/*
    File:       MediaLibraryUpdates.h
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

#ifndef MEDIALIBRARYUPDATES_H_
#define  MEDIALIBRARYUPDATES_H_

#include "iAP2LinkRunLoop.h"
#include "Certificate.h"
#include "DataDecoder.h"
#include "DataEncoder.h"
#include "Transport.h"
#include <iAp2Marshal.h>
namespace IAP2
{

    class LinkController;
    class MediaLibraryUpdates{

        public :
            MediaLibraryUpdates();
            ~MediaLibraryUpdates();

        public :
            void handleMediaLibraryUpdates(LinkController* pLnkCtrler, int8_t session, uint8_t* pData);
            

         private : 
             int handleMeidaLibraryUpdatesEvent(LinkController* pLnkCtrler, int8_t session, MediaLibraryUpdatesEvt* pEvent);    
               
             void ParsingMediaLibraryUpdateEventMediaItemGroup(MarshalIo* iAp2Marshal, MediaItemGroup* pMediaItemGroup);     
             void ParsingMediaLibraryUpdateEventMediaPlaylistGroup(MarshalIo* iAp2Marshal, MediaPlaylistGroup* pMeidaPlaylistGroup);            
                       
            void decodeMediaLibraryUpdates(uint8_t* pData, MediaLibraryUpdatesEvt* pEvt);
            void decordMediaLibraryUpdatesMediaItem(uint8_t* pData, int nParamStartIdx, MediaItemGroup* pMediaItemGroup);
            void decordMediaLibraryUpdatesMediaPlaylist(uint8_t* pData, int nParamStartIdx, MediaPlaylistGroup* pMediaPlaylistGroup);
            int getMediaItemGroupCount(uint8_t* pData);
            int getMediaPlaylistGroupCount(uint8_t* pData);
            int getMediaItemDeletePersistentIdentifierCount(uint8_t* pData);
            int getMediaPlaylistDeletePersistentIdentifierCount(uint8_t* pData);
            int getMediItemTypeCount(uint8_t* pData, int nParamStartIdx);
            
    };


} // namespace IAP2

#endif // #ifndef MEDIALIBRARYUPDATES_H_
