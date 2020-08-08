/*
    File:       NowPlaying.cpp
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

#include <vector>

#include "NowPlaying.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

NowPlaying::NowPlaying()
{
    LOGD("NowPlaying()");
}

NowPlaying::~NowPlaying()
{
    LOGD("~NowPlaying()");
}


void NowPlaying::handleNowPlayingUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    NowPlayingUpdateEvt event;
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeNowPlayingUpdates(pData, &event);


    if (event.mediaAttrResGr.ArtworkFileTransferID.bPresent == TRUE) {
        LOGD(" handleNowPlayingUpdate :: FileTransfer :: AlbumArtworkID = %d", event.mediaAttrResGr.ArtworkFileTransferID.value);
        pLnkCtrler->mFileTransfer.createIAP2FileTransferCreate(pLnkCtrler, event.mediaAttrResGr.ArtworkFileTransferID.value, pLnkCtrler->mConnection.mConnPath, FILETRANSFERMODE_NOWPLAYING_ARTWORK);
        //pLnkCtrler->createIAP2FileTransferCreate(event.mediaAttrResGr.ArtworkFileTransferID.value);
    }

      result = handleNowPlayingUpdateEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleNowPlayingUpdate(). result[%d]", result);
}
/*
// #ifdef iAP2SERVICE_FILETRANSFER
//void NowPlaying::handleNowPlayingAlbumArtworkUpdate(iAP2FileTransfer_st* fileXfer, void* userInfo)
void NowPlaying::handleNowPlayingAlbumArtworkUpdate(iAP2FileTransfer_st* fileXfer)
{
    LOGD("handleNowPlayingAlbumArtworkUpdate()");

    int result = kIap2NoErr;
    NowPlayingAlbumArtworkEvt event;

    iAP2Link_st* pLink = (iAP2Link_st*)(fileXfer->link);
    LinkController* pInstance = (LinkController*)(LinkController*)((iAP2LinkRunLoop_t*)(pLink->context))->context;

    // Confirm State
    LOGD("handleNowPlayingAlbumArtworkUpdate() state[%u]", fileXfer->state);
    
    if (pInstance == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (fileXfer->pBuffer == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }
    
    memset(&event, 0x00, sizeof(event));
    decodeNowPlayingAlbumArtworkUpdate(fileXfer, &event);
    
    result = handleNowPlayingAlbumArtworkUpdateEvent(pInstance, fileXfer->session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleNowPlayingAlbumArtworkUpdateEvent(). result[%d]", result);

// #endif
} */
void NowPlaying::decodeNowPlayingUpdates(uint8_t* pData, NowPlayingUpdateEvt* pEvt)
{
    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    if (pEvt == NULL) {
        LOGE("    Error : pEvt is NULL");
        return;
    }

    int nMsgLength;
    nMsgLength = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);

    int nParamId, nParamLength;
    int nParamStartIdx = 6;
    int nParamNum = 0;
    int nSubParamId, nSubParamLength;

    while(nMsgLength != nParamStartIdx) {
        nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);
        nParamId = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 2, 2);

        if (nParamId == kParamIdNowPlayingMediaItemAttributes)
            pEvt->bMiPresent = TRUE;
        else if (nParamId == kParamIdNowPlayingPlaybackAttributes)
            pEvt->bPbPresent = TRUE;

        int nSubParamStartIdx = nParamStartIdx + 4;
        int nSubParamNum = 0;

        //LOGD("nParamLength[%d] nParamId[0x%x] nParamStartIdx[%d] nSubParamStartIdx[%d]", nParamLength, nParamId, nParamStartIdx, nSubParamStartIdx);

        while(nParamLength != (nSubParamStartIdx -nParamStartIdx)) {
            nSubParamLength = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx, 2);
            nSubParamId = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+2, 2);

            //LOGD("nSubParamLength[%d], nSubParamId[0x%x] nParamStartIdx[%d] nSubParamStartIdx[%d]", nSubParamLength, nSubParamId, nParamStartIdx, nSubParamStartIdx);

            switch(nParamId) {
                case kParamIdNowPlayingMediaItemAttributes: {
                    //LOGD("kParamIdNowPlayingMediaItemAttributes");
                    decodeNowPlayingUpdateMediaItemAttributes(pData, pEvt, nSubParamId,  nSubParamStartIdx, nSubParamLength);
                    break;
                }
                
                case kParamIdNowPlayingPlaybackAttributes : {
                    //LOGD("kParamIdNowPlayingPlaybackAttributes");
                    decodeNowPlayingUpdatePlaybackAttributes(pData, pEvt, nSubParamId,  nSubParamStartIdx, nSubParamLength);
                    break;
                }
                    
                default:
                    LOGD("    WARNING : default break nParamId: 0x%x", nParamId);
                    break;
            }
            nSubParamStartIdx = nSubParamLength + nSubParamStartIdx;
            nSubParamNum++;
        }

        nParamStartIdx = nParamLength + nParamStartIdx;
        nParamNum++;
    }
}

void NowPlaying::decodeNowPlayingUpdateMediaItemAttributes(uint8_t* pData, NowPlayingUpdateEvt* pEvt, int nSubParamId,  int nSubParamStartIdx, int nSubParamLength)
{
    //LOGD("decodeNowPlayingUpdateSubparams()");    
    switch(nSubParamId) {
        case kSubParamIdNowPlayingMediaItemPersistId: {
            pEvt->mediaAttrResGr.persistId.bPresent = TRUE;
            pEvt->mediaAttrResGr.persistId.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx+4, 8);
            break;
        }
                                
        case kSubParamIdNowPlayingMediaItemTitle: {
            pEvt->mediaAttrResGr.title.bPresent = TRUE;
            pEvt->mediaAttrResGr.title.size = nSubParamLength-4;
            pEvt->mediaAttrResGr.title.value = (uint8_t*)&pData[nSubParamStartIdx+4];
            break;
        }

        case kSubParamIdNowPlayingMediaItemMediaType: {
            //pEvt->mediaAttrResGr.mediaType.bPresent = TRUE;
            //pEvt->mediaAttrResGr.mediaType.size = nSubParamLength-4;
            //pEvt->mediaAttrResGr.mediaType.value = (uint8_t*)&pData[nSubParamStartIdx+4];

            //pMediaItemAttrMediaType->value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
            //pMediaItemAttrMediaType = pMediaItemAttrMediaType + 1;
            break;
        }

        case kSubParamIdNowPlayingMediaItemRating: {
            pEvt->mediaAttrResGr.rating.bPresent = TRUE;
            pEvt->mediaAttrResGr.rating.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }
        

        case kSubParamIdNowPlayingMediaItemPlaybackDurationInMs: {
            pEvt->mediaAttrResGr.playbackDurMs.bPresent = TRUE;
            pEvt->mediaAttrResGr.playbackDurMs.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 4);
            break;
        }                

        case kSubParamIdNowPlayingMediaItemPersistIdentifier: {
            pEvt->mediaAttrResGr.albumPersistId.bPresent = TRUE;
            pEvt->mediaAttrResGr.albumPersistId.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx+4, 8);
            break;
        }
                                
        case kSubParamIdNowPlayingMediaItemAlbumTitle: {
            pEvt->mediaAttrResGr.albumTitle.bPresent = TRUE; 
            pEvt->mediaAttrResGr.albumTitle.size = nSubParamLength-4;
            pEvt->mediaAttrResGr.albumTitle.value = (uint8_t*)&pData[nSubParamStartIdx+4];
            break;
        }

        case kSubParamIdNowPlayingMediaItemAlbumTrackNumber: {
            pEvt->mediaAttrResGr.albumTrackNo.bPresent = TRUE;
            pEvt->mediaAttrResGr.albumTrackNo.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 2);
            break;
        }

        case kSubParamIdNowPlayingMediaItemAlbumTrackCount: {
            pEvt->mediaAttrResGr.albumTrackCount.bPresent = TRUE;
            pEvt->mediaAttrResGr.albumTrackCount.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 2);
            break;
        }

        case kSubParamIdNowPlayingMediaItemAlbumDiscNumber: {
            pEvt->mediaAttrResGr.albumDiscNo.bPresent = TRUE;
            pEvt->mediaAttrResGr.albumDiscNo.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 2);
            break;
        }

        case kSubParamIdNowPlayingMediaItemAlbumDiscCount: {
            pEvt->mediaAttrResGr.albumDiscCount.bPresent = TRUE;
            pEvt->mediaAttrResGr.albumDiscCount.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 2);
            break;
        }

        case kSubParamIdNowPlayingMediaItemArtistPersistentId: {
            pEvt->mediaAttrResGr.artistPersistId.bPresent = TRUE;
            pEvt->mediaAttrResGr.artistPersistId.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx+4, 8);
            break;
        }

        case kSubParamIdNowPlayingMediaItemArtist: {
            pEvt->mediaAttrResGr.artist.bPresent = TRUE; 
            pEvt->mediaAttrResGr.artist.size = nSubParamLength-4;
            pEvt->mediaAttrResGr.artist.value = (uint8_t*)&pData[nSubParamStartIdx+4];
            break;
        }

        case kSubParamIdNowPlayingMediaItemAlbumArtistPersistentId: {
            pEvt->mediaAttrResGr.albumArtistPersistId.bPresent = TRUE;
            pEvt->mediaAttrResGr.albumArtistPersistId.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx+4, 8);
            break;
        }

        case kSubParamIdNowPlayingMediaItemAlbumArtist: {
            pEvt->mediaAttrResGr.albumArtist.bPresent = TRUE; 
            pEvt->mediaAttrResGr.albumArtist.size = nSubParamLength-4;
            pEvt->mediaAttrResGr.albumArtist.value = (uint8_t*)&pData[nSubParamStartIdx+4];
            break;
        }

        case kSubParamIdNowPlayingMediaItemGenrePersistentId: {
            pEvt->mediaAttrResGr.genrePersistId.bPresent = TRUE;
            pEvt->mediaAttrResGr.genrePersistId.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx+4, 8);
            break;
        }

        case kSubParamIdNowPlayingMediaItemGenre: {
            pEvt->mediaAttrResGr.genre.bPresent = TRUE;
            pEvt->mediaAttrResGr.genre.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingMediaItemComposerPersistId: {
            pEvt->mediaAttrResGr.composerPersistId.bPresent = TRUE;
            pEvt->mediaAttrResGr.composerPersistId.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx+4, 8);
            break;
        }

        case kSubParamIdNowPlayingMediaItemComposer: {
            pEvt->mediaAttrResGr.composer.bPresent = TRUE; 
            pEvt->mediaAttrResGr.composer.size = nSubParamLength-4;
            pEvt->mediaAttrResGr.composer.value = (uint8_t*)&pData[nSubParamStartIdx+4];
            break;
        }

        case kSubParamIdNowPlayingMediaItemIsPartOfCompilation: {
            pEvt->mediaAttrResGr.isPartofCompilation.bPresent = TRUE;
            pEvt->mediaAttrResGr.isPartofCompilation.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingMediaItemIsLikeSupported: {
            pEvt->mediaAttrResGr.isLikeSurpported.bPresent = TRUE;
            pEvt->mediaAttrResGr.isLikeSurpported.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingMediaItemIsBanSupported: {
            pEvt->mediaAttrResGr.isBanSupported.bPresent = TRUE;
            pEvt->mediaAttrResGr.isBanSupported.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingMediaItemIsLiked: {
            pEvt->mediaAttrResGr.isLiked.bPresent = TRUE;
            pEvt->mediaAttrResGr.isLiked.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingMediaItemIsBanned: {
            pEvt->mediaAttrResGr.isBanned.bPresent = TRUE;
            pEvt->mediaAttrResGr.isBanned.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingMediaItemIsResidentOnDevice: {
            pEvt->mediaAttrResGr.isResidOnDev.bPresent = TRUE;
            pEvt->mediaAttrResGr.isResidOnDev.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingMediaItemArtworkFileTransferIdentifier: {
            pEvt->mediaAttrResGr.ArtworkFileTransferID.bPresent = TRUE;
            pEvt->mediaAttrResGr.ArtworkFileTransferID.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingMediaItemChapterCount: {
            pEvt->mediaAttrResGr.ChapterCount.bPresent = TRUE;
            pEvt->mediaAttrResGr.ChapterCount.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 2);
            break;
        }
        
        default:
            LOGD("    WARNING : unknowd nSubParamId[0x%x]", nSubParamId);
            break;
    }
}

int NowPlaying::getMediItemTypeCount(uint8_t* pData, int nParamStartIdx)
{
    //LOGD("getMediItemTypeCount()");

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return 0;
    }

    int nParamLength;
    int count = 0;
    nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);

    int nSubParamId, nSubParamLength;
    int nSubParamStartIdx = nParamStartIdx+4;
    
    while(nParamLength > (nSubParamStartIdx - nParamStartIdx)) {
        nSubParamLength = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx, 2);
        nSubParamId = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 2, 2);
        
        if (nSubParamId == kParamIDMediaItemMediaType) {
            count++;
        }
        nSubParamStartIdx = nSubParamStartIdx + nSubParamLength;
    }
    //LOGD("getMediItemTypeCount(): %u",count);
    return count;
}

void NowPlaying::decodeNowPlayingUpdatePlaybackAttributes(uint8_t* pData, NowPlayingUpdateEvt* pEvt, int nSubParamId,  int nSubParamStartIdx, int nSubParamLength)
{
    //LOGD("decodeNowPlayingUpdatePlaybackAttributes()");
    switch(nSubParamId) {
        case kSubParamIdNowPlayingPlaybackStatus: {
            pEvt->playbackAttrResGr.status.bPresent = TRUE;
            pEvt->playbackAttrResGr.status.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingPlaybackElapsedTimeMs: {
            pEvt->playbackAttrResGr.elapsedTimeMs.bPresent = TRUE;
            pEvt->playbackAttrResGr.elapsedTimeMs.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 4);
            break;
        }

        case kSubParamIdNowPlayingPlaybackQueueIndex: {
            pEvt->playbackAttrResGr.queueIdx.bPresent = TRUE;
            pEvt->playbackAttrResGr.queueIdx.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 4);
            break;
        }
        
        case kSubParamIdNowPlayingPlaybackQueueCount: {
            pEvt->playbackAttrResGr.queueCount.bPresent = TRUE;
            pEvt->playbackAttrResGr.queueCount.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 4);
            break;
        }

        case kSubParamIdNowPlayingPlaybackQueueChapterIndex: {
            pEvt->playbackAttrResGr.queueChapterIdx.bPresent = TRUE;
            pEvt->playbackAttrResGr.queueChapterIdx.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 4);
            break;
        }

        case kSubParamIdNowPlayingPlaybackShuffleMode: {
            pEvt->playbackAttrResGr.shuffleMode.bPresent = TRUE;
            pEvt->playbackAttrResGr.shuffleMode.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingPlaybackRepeatMode: {
            pEvt->playbackAttrResGr.repeatMode.bPresent = TRUE;
            pEvt->playbackAttrResGr.repeatMode.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingPlaybackAppName: {
            pEvt->playbackAttrResGr.appName.bPresent = TRUE; 
            pEvt->playbackAttrResGr.appName.size = nSubParamLength-4;
            pEvt->playbackAttrResGr.appName.value = (uint8_t*)&pData[nSubParamStartIdx+4];
            break;
        }

        case kSubParamIdNowPlayingPlaybackMediaLibraryUniqueIdentifier: {
            pEvt->playbackAttrResGr.pbMediaLibUniqueId.bPresent = TRUE;
            pEvt->playbackAttrResGr.pbMediaLibUniqueId.size = nSubParamLength-4;
            pEvt->playbackAttrResGr.pbMediaLibUniqueId.value = (uint8_t*)&pData[nSubParamStartIdx+4];
            break;
        }

        case kSubParamIdNowPlayingPBiTunesRadioAd: {
            pEvt->playbackAttrResGr.pbiTunesRadioAd.bPresent = TRUE;
            pEvt->playbackAttrResGr.pbiTunesRadioAd.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingPBiTunesRadioStationName: {
            pEvt->playbackAttrResGr.pbiTunesRadioStationName.bPresent = TRUE;            
            pEvt->playbackAttrResGr.pbiTunesRadioStationName.size = nSubParamLength-4;
            pEvt->playbackAttrResGr.pbiTunesRadioStationName.value = (uint8_t*)&pData[nSubParamStartIdx+4];
            break;
        }

        case kSubParamIdNowPlayingPBiTunesRadioStationMediaPlaylistPersistentID: {
            pEvt->playbackAttrResGr.pbiTunesRadioStationMediaPlaylistPersistentID.bPresent = TRUE;
            pEvt->playbackAttrResGr.pbiTunesRadioStationMediaPlaylistPersistentID.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx+4, 8);
            break;
        }
        
        case kSubParamIdNowPlayingPBSpeed: {
            pEvt->playbackAttrResGr.Speed.bPresent = TRUE;
            pEvt->playbackAttrResGr.Speed.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 2);
            break;
        }

        case kSubParamIdNowPlayingSetElapsedTimeAvailable: {
            pEvt->playbackAttrResGr.SetElapsedTimeAvailable = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingPBQueueListAvail: {
            pEvt->playbackAttrResGr.PlaybackQueueListAvail = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }

        case kSubParamIdNowPlayingPBQueueListTransferID: {
            pEvt->playbackAttrResGr.QueueListTransferID.bPresent = TRUE;
            pEvt->playbackAttrResGr.QueueListTransferID.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
            break;
        }    

        case kSubParamIdNowPlayingPBAppBundleID: {
            pEvt->playbackAttrResGr.AppBundleID.bPresent = TRUE; 
            pEvt->playbackAttrResGr.AppBundleID.size = nSubParamLength-4;
            pEvt->playbackAttrResGr.AppBundleID.value = (uint8_t*)&pData[nSubParamStartIdx+4];
            break;
        }    
        
                                
        default:
            LOGD("    default break nSubParamId: 0x%x", nSubParamId);
            break;
    }
}

// #ifdef iAP2SERVICE_FILETRANSFER


int NowPlaying::handleNowPlayingUpdateEvent(LinkController* pLnkCtrler, int8_t session, NowPlayingUpdateEvt* pEvent)
{
    int result = kIap2NoErr;    
    #define MARSHALING_BUFFER (40000)

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(MARSHALING_BUFFER);
    
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, MARSHALING_BUFFER, sizeof(EventHeader));

    // MediaItemAttrResGroup
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bMiPresent);
    if (pEvent->bMiPresent == TRUE) {
        MediaItemAttrResGroup* pMiAttrResGr = &pEvent->mediaAttrResGr;
        ParsingNowPlayingUpdateEventpMiAttrResGr(pMiAttrResGr, &iAp2Marshal);        
    }
    
    // MediaItemAttrResGroup
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bPbPresent);
    if (pEvent->bPbPresent == TRUE) {
        PlaybackAttrResGroup* pPbAttrResGr = &pEvent->playbackAttrResGr;
        ParsingNowPlayingUpdateEventpPbAttrResGr(pPbAttrResGr, &iAp2Marshal);        
    }

    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtNowPlayingUpdate;
    mEventHeader->dataSize = byteNum;

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(session, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGE("    ERROR : send event fail. result[%d]", result);
    }
    //free(pEvent->mediaAttrResGr.mediaType);
    free(pIpcData);
    return result;
}
/*
//#ifdef iAP2SERVICE_FILETRANSFER
int NowPlaying::handleNowPlayingAlbumArtworkUpdateEvent(LinkController* pLnkCtrler, int8_t session, NowPlayingAlbumArtworkEvt* pEvent)
{
    
    int result = kIap2NoErr;

    LOGD("handleNowPlayingAlbumArtworkUpdateEvent() ");
    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(NowPlayingAlbumArtworkEvt) + (uint32_t)pEvent->buffSize;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    LOGD("pEvent->state[%u]", pEvent->state);
    LOGD("pEvent->totSize[%u]", pEvent->totSize);
    LOGD("pEvent->bPresent : %u", pEvent->bPresent);
    LOGD("pEvent->buffSize : %u", pEvent->buffSize);      
    LOGD("pEvent->bufferID [%u]", pEvent->bufferID);
    LOGD("pEvent->mFileTransferMode [%u]", pEvent->mFileTransferMode);
    pEvent->mFileTransferMode = FILETRANSFERMODE_NOWPLAYING_ARTWORK;
    iAp2MarshalPutUint64(&iAp2Marshal, pEvent->state);
    iAp2MarshalPutUint64(&iAp2Marshal, pEvent->totSize);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bPresent);
    if(pEvent->bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->value, pEvent->buffSize);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bufferID);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mFileTransferMode);        

    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal); 

    mEventHeader->evt = kEvtNowPlayingAlbumArtworkUpdate;
    mEventHeader->dataSize = byteNum;
    
    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(session, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGE("    ERROR : send event fail. result[%d]", result);
    }
    
    free(pEvent->value);
    free(pIpcData);
    return result;
}
//#endif // #ifdef iAP2SERVICE_FILETRANSFER

*/
    
void NowPlaying::ParsingNowPlayingUpdateEventpMiAttrResGr(MediaItemAttrResGroup* pMiAttrResGr, MarshalIo* iAp2Marshal)
{
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->persistId.bPresent);
    if (pMiAttrResGr->persistId.bPresent == TRUE)
        iAp2MarshalPutUint64(iAp2Marshal, pMiAttrResGr->persistId.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->title.bPresent);
    if (pMiAttrResGr->title.bPresent == TRUE) 
        iAp2MarshalPutBytes(iAp2Marshal, pMiAttrResGr->title.value, pMiAttrResGr->title.size);
    
    //iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->mediaType.bPresent);
    //if (pMiAttrResGr->mediaType.bPresent == TRUE) 
    //    iAp2MarshalPutBytes(iAp2Marshal, pMiAttrResGr->mediaType.value, pMiAttrResGr->mediaType.size);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->MediaItemMediaTypeCount);
    if(0 < pMiAttrResGr->MediaItemMediaTypeCount )
    {
        iAp2MarshalPutBytes(iAp2Marshal, (uint8_t*)pMiAttrResGr->mediaType, pMiAttrResGr->MediaItemMediaTypeSize);
    }
        
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->rating.bPresent);
    if (pMiAttrResGr->rating.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->rating.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->playbackDurMs.bPresent);
    if (pMiAttrResGr->playbackDurMs.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->playbackDurMs.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumPersistId.bPresent);
    if (pMiAttrResGr->albumPersistId.bPresent == TRUE)
        iAp2MarshalPutUint64(iAp2Marshal, pMiAttrResGr->albumPersistId.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumTitle.bPresent);
    if (pMiAttrResGr->albumTitle.bPresent == TRUE)
        iAp2MarshalPutBytes(iAp2Marshal, pMiAttrResGr->albumTitle.value, pMiAttrResGr->albumTitle.size);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumTrackNo.bPresent);
    if (pMiAttrResGr->albumTrackNo.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumTrackNo.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumTrackCount.bPresent);
    if (pMiAttrResGr->albumTrackCount.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumTrackCount.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumDiscNo.bPresent);
    if (pMiAttrResGr->albumDiscNo.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumDiscNo.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumDiscCount.bPresent);
    if (pMiAttrResGr->albumDiscCount.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumDiscCount.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->artistPersistId.bPresent);
    if (pMiAttrResGr->artistPersistId.bPresent == TRUE)
        iAp2MarshalPutUint64(iAp2Marshal, pMiAttrResGr->artistPersistId.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->artist.bPresent);
    if (pMiAttrResGr->artist.bPresent == TRUE) 
            iAp2MarshalPutBytes(iAp2Marshal, pMiAttrResGr->artist.value, pMiAttrResGr->artist.size);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumArtistPersistId.bPresent);
    if (pMiAttrResGr->albumArtistPersistId.bPresent == TRUE)
        iAp2MarshalPutUint64(iAp2Marshal, pMiAttrResGr->albumArtistPersistId.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->albumArtist.bPresent);
    if (pMiAttrResGr->albumArtist.bPresent == TRUE) 
            iAp2MarshalPutBytes(iAp2Marshal, pMiAttrResGr->albumArtist.value, pMiAttrResGr->albumArtist.size);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->genrePersistId.bPresent);
    if (pMiAttrResGr->genrePersistId.bPresent == TRUE)
        iAp2MarshalPutUint64(iAp2Marshal, pMiAttrResGr->genrePersistId.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->genre.bPresent);
    if (pMiAttrResGr->genre.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->genre.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->composerPersistId.bPresent);
    if (pMiAttrResGr->composerPersistId.bPresent == TRUE)
        iAp2MarshalPutUint64(iAp2Marshal, pMiAttrResGr->composerPersistId.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->composer.bPresent);
    if (pMiAttrResGr->composer.bPresent == TRUE)
        iAp2MarshalPutBytes(iAp2Marshal, pMiAttrResGr->composer.value, pMiAttrResGr->composer.size);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isPartofCompilation.bPresent);
    if (pMiAttrResGr->isPartofCompilation.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isPartofCompilation.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isLikeSurpported.bPresent);
    if (pMiAttrResGr->isLikeSurpported.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isLikeSurpported.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isBanSupported.bPresent);
    if (pMiAttrResGr->isBanSupported.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isBanSupported.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isLiked.bPresent);
    if (pMiAttrResGr->isLiked.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isLiked.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isBanned.bPresent);
    if (pMiAttrResGr->isBanned.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isBanned.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isResidOnDev.bPresent);
    if (pMiAttrResGr->isResidOnDev.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->isResidOnDev.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->ArtworkFileTransferID.bPresent);
    if (pMiAttrResGr->ArtworkFileTransferID.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->ArtworkFileTransferID.value);

    iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->ChapterCount.bPresent);
    if (pMiAttrResGr->ChapterCount.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pMiAttrResGr->ChapterCount.value);
}

void NowPlaying::ParsingNowPlayingUpdateEventpPbAttrResGr(PlaybackAttrResGroup* pPbAttrResGr, MarshalIo* iAp2Marshal)
{
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->status.bPresent);
    if (pPbAttrResGr->status.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->status.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->elapsedTimeMs.bPresent);
    if (pPbAttrResGr->elapsedTimeMs.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->elapsedTimeMs.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->queueIdx.bPresent);
    if (pPbAttrResGr->queueIdx.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->queueIdx.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->queueCount.bPresent);
    if (pPbAttrResGr->queueCount.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->queueCount.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->queueChapterIdx.bPresent);
    if (pPbAttrResGr->queueChapterIdx.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->queueChapterIdx.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->shuffleMode.bPresent);
    if (pPbAttrResGr->shuffleMode.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->shuffleMode.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->repeatMode.bPresent);
    if (pPbAttrResGr->repeatMode.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->repeatMode.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->appName.bPresent);
    if (pPbAttrResGr->appName.bPresent == TRUE) 
        iAp2MarshalPutBytes(iAp2Marshal, pPbAttrResGr->appName.value, pPbAttrResGr->appName.size);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->pbMediaLibUniqueId.bPresent);
    if (pPbAttrResGr->pbMediaLibUniqueId.bPresent == TRUE)
        iAp2MarshalPutBytes(iAp2Marshal, pPbAttrResGr->pbMediaLibUniqueId.value, pPbAttrResGr->pbMediaLibUniqueId.size);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->pbiTunesRadioAd.bPresent);
    if (pPbAttrResGr->pbiTunesRadioAd.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->pbiTunesRadioAd.value);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->pbiTunesRadioStationName.bPresent);
    if (pPbAttrResGr->pbiTunesRadioStationName.bPresent == TRUE)
        iAp2MarshalPutBytes(iAp2Marshal, pPbAttrResGr->pbiTunesRadioStationName.value, pPbAttrResGr->pbiTunesRadioStationName.size);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->pbiTunesRadioStationMediaPlaylistPersistentID.bPresent);
    if (pPbAttrResGr->pbiTunesRadioStationMediaPlaylistPersistentID.bPresent == TRUE)
        iAp2MarshalPutUint64(iAp2Marshal, pPbAttrResGr->pbiTunesRadioStationMediaPlaylistPersistentID.value);

    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->Speed.bPresent);
    if (pPbAttrResGr->Speed.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->Speed.value);

    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->SetElapsedTimeAvailable);
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->PlaybackQueueListAvail);
    
    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->QueueListTransferID.bPresent);
    if (pPbAttrResGr->QueueListTransferID.bPresent == TRUE)
        iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->QueueListTransferID.value);

    iAp2MarshalPutUint32(iAp2Marshal, pPbAttrResGr->AppBundleID.bPresent);
    if (pPbAttrResGr->AppBundleID.bPresent == TRUE)
        iAp2MarshalPutBytes(iAp2Marshal, pPbAttrResGr->AppBundleID.value, pPbAttrResGr->AppBundleID.size);
    
}
/*

// #ifdef iAP2SERVICE_FILETRANSFER
void NowPlaying::decodeNowPlayingAlbumArtworkUpdate(iAP2FileTransfer_st* fileXfer, NowPlayingAlbumArtworkEvt* pEvt)
{
    LOGD("decodeNowPlayingAlbumArtworkUpdate()");

    if (fileXfer->pBuffer == NULL) {
        LOGE("    Error : pBuffer is NULL");
        return;
    }

    if (pEvt == NULL) {
        LOGE("    Error : pEvt is NULL");
        return;
    }
    
    pEvt->state = (FileTransferState)fileXfer->state;
    pEvt->totSize = fileXfer->totalSize;
    pEvt->buffSize= fileXfer->buffSize;
    pEvt->bPresent = TRUE;
    if(fileXfer->pBuffer) {
        pEvt->bPresent = TRUE;
        pEvt->value = (uint8_t*) malloc(sizeof(uint8_t) * fileXfer->buffSize);
        memcpy(pEvt->value, fileXfer->pBuffer, fileXfer->buffSize);
    }
    pEvt->bufferID = fileXfer->bufferID;
}
// #endif // #ifdef iAP2SERVICE_FILETRANSFER

*/
} // namespace IAP2
