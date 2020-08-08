/*
    File:       SessionNowPlaying.cpp
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

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>


#include "SessionNowPlaying.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

#define MARSHALING_BUFFER (40000)
namespace IAP2
{

SessionNowPlaying::SessionNowPlaying() 
{
    LOGD("SessionNowPlaying()");
}

SessionNowPlaying::~SessionNowPlaying()
{
    LOGD("~SessionNowPlaying()");
}

int SessionNowPlaying::handleNowPlayingCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    LOGD("handleNowPlayingCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);
    int result = kIap2NoErr;
    
    if (pRxData == NULL) {
        LOGE("    ERROR : pRxData is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR : pLnkCtrler is NULL");
        return kIap2ParamErr;
    }

    switch(cmd) {
        case kCmdStartNowPlayingUpdates : {
            StartNowPlayingArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            // mediaAttrReqGr
            arg.bMiPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.bMiPresent == TRUE) {
                ParsingNowPlayingUpdateCmdmediaAttrReqGr(&iAp2Marshal, &arg);
            }
            
            // PlaybackAttrReqGroup
            arg.bPbPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.bPbPresent == TRUE) {
                ParsingNowPlayingUpdateCmdplaybackAttrReqGr(&iAp2Marshal, &arg);
            }
            sendStartNowPlayingUpdates(pLnkCtrler, &arg);
            break;
        }

        case kCmdStopNowPlayingUpdates : {
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            iAp2MarshalGetUint32(&iAp2Marshal);

            sendStopNowPlayingUpdates(pLnkCtrler);
            break;
        }

        case kCmdSetNowPlayingInformation : {
            SetNowPlayingInformationArg arg;
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.nElapsedTimeParam.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.nElapsedTimeParam.bPresent == TRUE) {
                arg.nElapsedTimeParam.value = (uint32_t)iAp2MarshalGetUint32(&iAp2Marshal);
            }
            
            arg.nPlayBackQueueIndexParam.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.nPlayBackQueueIndexParam.bPresent == TRUE) {
                arg.nPlayBackQueueIndexParam.value = (uint32_t)iAp2MarshalGetUint32(&iAp2Marshal);
            }
            
            sendSetNowPlayingInformation(pLnkCtrler, &arg);
            break;
        }

        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}



void SessionNowPlaying::sendStartNowPlayingUpdates(LinkController* pLnkCtrler, StartNowPlayingArg* pArg)
{
    LOGD("sendStartNowPlayingUpdates()");
    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;
    
    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;
    
    createStartNowPlayingUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);    
    
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartNowPlayingUpdates,
                          nParamIdList, paramDataList, nParamDataLenList, &encodedData);
    
    ByteArray encodedDaraArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDaraArr);        
    
    DataEncoder::deleteParamList(nParamIdList,paramDataList, nParamDataLenList );
    delete [] encodedData;

}
void SessionNowPlaying::sendStopNowPlayingUpdates(LinkController* pLnkCtrler)
{
    LOGD("sendStopNowPlayingUpdates()");
    
     struct iAP2Link_st* link;
     if (pLnkCtrler->m_piAP2LinkRunLoop)
         link = pLnkCtrler->m_piAP2LinkRunLoop->link;
     else {
         LOGE("  ERROR :  m_piAP2LinkRunLoop is NULL");
         return;
     }
    
     uint32_t nEncodedDataLength;
     uint8_t* encodedData = NULL;
    
     std::vector<int> nParamIdList;
     std::vector<const uint8_t*> paramDataList;
     std::vector<uint32_t> nParamDataLenList;
    
     nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopNowPlayingUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);
    
     ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

     int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
     if (session == 0) {
         LOGE("  ERROR : sessionId is 0");
         return;
     }         
     pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);

     DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
     delete [] encodedData;

}


void SessionNowPlaying::sendSetNowPlayingInformation(LinkController* pLnkCtrler, SetNowPlayingInformationArg* pArg)
{
    LOGD("sendSetNowPlayingInformation()");
    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;
    
    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;
    
    createSetNowPlayingInformationParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);    
    
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdSetNowPlayingInformation,
                          nParamIdList, paramDataList, nParamDataLenList, &encodedData);
    
    ByteArray encodedDaraArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDaraArr);        
    
    DataEncoder::deleteParamList(nParamIdList,paramDataList, nParamDataLenList );
    delete [] encodedData;

}

void SessionNowPlaying::createStartNowPlayingUpdatesParamList(std::vector<int>& nParamIdList,
                                                     std::vector<const uint8_t *>& paramDataList,
                                                     std::vector<uint32_t>& nParamDataLenList,
                                                     StartNowPlayingArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    uint8_t* groupParamBytes = NULL;
    uint32_t nNumOfBytes;

    nNumOfBytes = createGroupParamBytesForMediaItemAttributes(&groupParamBytes, pArg);

    DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdNowPlayingMediaItemAttributes, nParamIdList, paramDataList, nParamDataLenList);
    delete [] groupParamBytes;

    nNumOfBytes = createGroupParamBytesForPlaybackAttributes(&groupParamBytes, pArg);

    DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdNowPlayingPlaybackAttributes, nParamIdList, paramDataList, nParamDataLenList);
    delete [] groupParamBytes;
}



uint32_t SessionNowPlaying::createGroupParamBytesForMediaItemAttributes(uint8_t** encodedBytes, StartNowPlayingArg* pArg)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;
    uint8_t* encodedDataArr;

    createMediaItemAttributesParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList, pArg);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];
    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);
    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);
    *encodedBytes = encodedDataArr;
    return nTotalLength;
}


uint32_t SessionNowPlaying::createGroupParamBytesForPlaybackAttributes(uint8_t** encodedBytes, StartNowPlayingArg* pArg)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;
    uint8_t* encodedDataArr;

    createPlaybackAttributesParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList, pArg);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];
    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);
    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);
    *encodedBytes = encodedDataArr;
    return nTotalLength;
}

void SessionNowPlaying::createMediaItemAttributesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                                              std::vector<uint32_t> &nParamDataLenList, StartNowPlayingArg* pArg)
{
#ifndef TEST_CODE    
        if (pArg == NULL) {
            LOGE("    ERROR :  pArg is NULL");
            return;
        }
#endif // #ifndef TEST_CODE

    if (pArg->bPbPresent == TRUE) {
        if (pArg->mediaAttrReqGr.bPersistentIdentifier == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemPersistId, nParamIdList, paramDataList, nParamDataLenList);
        
        if (pArg->mediaAttrReqGr.bTitle == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemTitle, nParamIdList, paramDataList, nParamDataLenList);
    
        if (pArg->mediaAttrReqGr.bPlaybackDurationMs == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemPlaybackDurationInMs, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bAlbumTitle == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemAlbumTitle, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bAlbumTrackNo == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemAlbumTrackNumber, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bAlbumTrackCount == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemAlbumTrackCount, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bAlbumDiscNumber == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemAlbumDiscNumber, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bAlbumDiscCount == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemAlbumDiscCount, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bArtist == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemArtist, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bGenre == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemGenre, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bComposer == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemComposer, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bPropertyIsLikeSupported == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemIsLikeSupported, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bPropertyIsLikeBanSupported == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemIsBanSupported, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bPropertyIsLiked == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemIsLiked, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bPropertyIsLikeBanned == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemIsBanned, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bArtworkFileTransferId == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemArtworkFileTransferIdentifier, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mediaAttrReqGr.bChapterCount == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingMediaItemChapterCount, nParamIdList, paramDataList, nParamDataLenList);
    }

}

void SessionNowPlaying::createPlaybackAttributesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                                              std::vector<uint32_t> &nParamDataLenList, StartNowPlayingArg* pArg)
{
#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    if (pArg->bPbPresent == TRUE) {
        if (pArg->playbackAttrReqGr.bStatus== TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPlaybackStatus, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bElapsedTimeMs == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPlaybackElapsedTimeMs, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bQueueIdx== TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPlaybackQueueIndex, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bQueueCount== TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPlaybackQueueCount, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bQueueChapterIdx == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPlaybackQueueChapterIndex, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bShuffleMode == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPlaybackShuffleMode, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bRepeatMode == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPlaybackRepeatMode, nParamIdList, paramDataList, nParamDataLenList);

        //if (pArg->playbackAttrReqGr.bAppName == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPlaybackAppName, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bPbMediaLibUniqueId == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPlaybackMediaLibraryUniqueIdentifier, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bPbiTunesRadioAd == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPBiTunesRadioAd, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bPbiTunesRadioStationName == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPBiTunesRadioStationName, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bPbiTunesRadioStationMediaPlaylistPersistentID == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPBiTunesRadioStationMediaPlaylistPersistentID, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bPbSpeed == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPBSpeed, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bSetElapsedTimeAvailable == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingSetElapsedTimeAvailable, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bPbQueueListAvail == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPBQueueListAvail, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->playbackAttrReqGr.bPbQueueListTransferID == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdNowPlayingPBQueueListTransferID, nParamIdList, paramDataList, nParamDataLenList);

        //if (pArg->playbackAttrReqGr.bPbAppBundleID == TRUE)
               DataEncoder::appendNoneParam(kSubParamIdNowPlayingPBAppBundleID, nParamIdList, paramDataList, nParamDataLenList);
    }

}

void SessionNowPlaying::ParsingNowPlayingUpdateCmdmediaAttrReqGr(MarshalIo* iAp2Marshal, StartNowPlayingArg* arg)
{
    arg->mediaAttrReqGr.bPersistentIdentifier = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bTitle = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bPlaybackDurationMs = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bAlbumTitle = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bAlbumTrackNo = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bAlbumTrackCount = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bAlbumDiscNumber = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bAlbumDiscCount = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bArtist = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bGenre = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bComposer = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bPropertyIsLikeSupported = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bPropertyIsLikeBanSupported = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bPropertyIsLiked = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bPropertyIsLikeBanned = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bArtworkFileTransferId = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->mediaAttrReqGr.bChapterCount = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
}

void SessionNowPlaying::ParsingNowPlayingUpdateCmdplaybackAttrReqGr(MarshalIo* iAp2Marshal, StartNowPlayingArg* arg)
{
    arg->playbackAttrReqGr.bStatus = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bElapsedTimeMs = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bQueueIdx = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bQueueCount = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bQueueChapterIdx = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bShuffleMode = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bRepeatMode = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bAppName = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bPbMediaLibUniqueId = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bPbiTunesRadioAd = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bPbiTunesRadioStationName = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bPbiTunesRadioStationMediaPlaylistPersistentID = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bPbSpeed = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bSetElapsedTimeAvailable = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bPbQueueListAvail = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bPbQueueListTransferID = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    arg->playbackAttrReqGr.bPbAppBundleID = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
}



void SessionNowPlaying::createSetNowPlayingInformationParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, SetNowPlayingInformationArg* pArg)
{
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    if (pArg->nElapsedTimeParam.bPresent == TRUE) {
        DataEncoder::appendNumericParam(pArg->nElapsedTimeParam.value, 4, kParamIdSetNowPlayingInformation_ElapsedTime, nParamIdList, paramDataList, nParamDataLenList);
    }    

    if (pArg->nPlayBackQueueIndexParam.bPresent == TRUE) {
        DataEncoder::appendNumericParam(pArg->nPlayBackQueueIndexParam.value, 4, kParamIdSetNowPlayingInformation_PlayBackQueueIndex, nParamIdList, paramDataList, nParamDataLenList);
    }    
}

}// namespace IAP2
