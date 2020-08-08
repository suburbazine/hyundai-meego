/*
    File:       iAp2AppFrameworkNowPlaying.c
    Package:    LG iAP2 Application Framework
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

#include <stdlib.h>

#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkNowPlaying.h"
#include "iAp2AppFrameworkPrivate.h"
#include "iAp2Connection.h"
#include "iAp2Log.h"


//===========================================================================================================================
//    Definitions
//===========================================================================================================================

//===========================================================================================================================
//    Declarations
//===========================================================================================================================

//===========================================================================================================================
//    iAp2AppStartNowPlaying
//===========================================================================================================================
int iAp2AppStartNowPlaying(iAp2AppRef inRef, StartNowPlayingArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStartNowPlaying() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (inArg == NULL) {
        LOGE("ERROR : inArg is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*100;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;

    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bMiPresent);
    if (inArg->bMiPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bPersistentIdentifier);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bTitle);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bPlaybackDurationMs);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bAlbumTitle);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bAlbumTrackNo);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bAlbumTrackCount);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bAlbumDiscNumber);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bAlbumDiscCount);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bArtist);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bComposer);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bGenre);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bPropertyIsLikeSupported);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bPropertyIsLikeBanSupported);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bPropertyIsLiked);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bPropertyIsLikeBanned);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bArtworkFileTransferId);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mediaAttrReqGr.bChapterCount);

    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bPbPresent);
    if (inArg->bPbPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bStatus);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bElapsedTimeMs);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bQueueIdx);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bQueueCount);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bQueueChapterIdx);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bShuffleMode);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bRepeatMode);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bAppName);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bPbMediaLibUniqueId);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bPbiTunesRadioAd);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bPbiTunesRadioStationName);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bPbiTunesRadioStationMediaPlaylistPersistentID);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bPbSpeed);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bSetElapsedTimeAvailable);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bPbQueueListAvail);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bPbQueueListTransferID);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->playbackAttrReqGr.bPbAppBundleID);
    }

    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pCommandHeader->cmd = kCmdStartNowPlayingUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppStopNowPlaying
//===========================================================================================================================
int iAp2AppStopNowPlaying(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStopNowPlaying() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    uint32_t dummyValue = 0xFFFFFFFF;

    int mallocSize = sizeof(CommandHeader) + sizeof(dummyValue);
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

        CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, dummyValue);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pCommandHeader->cmd = kCmdStopNowPlayingUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;

}


//===========================================================================================================================
//    iAp2AppSetNowPlayingInformation
//===========================================================================================================================
int iAp2AppSetNowPlayingInformation(iAp2AppRef inRef, SetNowPlayingInformationArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppSetNowPlayingInformation() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (inArg == NULL) {
        LOGE("ERROR : inArg is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*100;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;

    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->nElapsedTimeParam.bPresent);
    if (inArg->nElapsedTimeParam.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->nElapsedTimeParam.value);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->nPlayBackQueueIndexParam.bPresent);
    if (inArg->nPlayBackQueueIndexParam.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->nPlayBackQueueIndexParam.value);
    }
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pCommandHeader->cmd = kCmdSetNowPlayingInformation;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;

}



//===========================================================================================================================
//    iAp2ConectionHandleEventNowPlayingUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventNowPlayingUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;

    //LOGD("iAp2ConectionHandleEventNowPlayingUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->nowPlayingUpdate_f == NULL) {
        LOGW("    WARING : delegate->nowPlayingUpdate_f is NULL");
        return kIap2NoErr;
    }

    NowPlayingUpdateCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    memset(&cbArg, 0x00, sizeof(NowPlayingUpdateCbArg));
    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    // mediaAttrResGr
    cbArg.bMiPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.bMiPresent == TRUE) {
        ParsingEventMiAttrResGr(&cbArg.mediaAttrResGr, &iAp2Marshal);
    }

    // playbackAttrResGr
    cbArg.bPbPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.bPbPresent == TRUE) {
        ParsingEventPbAttrResGr(&cbArg.playbackAttrResGr, &iAp2Marshal);
    }

    if ((handle->delegate != NULL) && (handle->delegate->nowPlayingUpdate_f != NULL)) {
        result = handle->delegate->nowPlayingUpdate_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.nowPlayingUpdate_f is NULL");
    }

    return result;
}



void ParsingEventMiAttrResGr(MediaItemAttrResGroup* pMiAttrResGr, MarshalIo* iAp2Marshal)
{
        pMiAttrResGr->persistId.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->persistId.bPresent == TRUE) {
            pMiAttrResGr->persistId.value = iAp2MarshalGetUint64(iAp2Marshal);
            //LOGD("    pMiAttrResGr->persistId value[%llu]", pMiAttrResGr->title.size, pMiAttrResGr->title.value);
        }

        pMiAttrResGr->title.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->title.bPresent == TRUE) {
            pMiAttrResGr->title.value = iAp2MarshalGetBytes(iAp2Marshal, &pMiAttrResGr->title.size);
            LOGD("    pMiAttrResGr->title size[%u] value[%s]", pMiAttrResGr->title.size, pMiAttrResGr->title.value);
        }

        pMiAttrResGr->MediaItemMediaTypeCount = iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("pMediaItemGroup->nMediaItemMediaTypeCount : %u", pMediaItemGroup->nMediaItemMediaTypeCount);
        if(0 < pMiAttrResGr->MediaItemMediaTypeCount) {
                pMiAttrResGr->mediaType = (MediaItemAttrMediaType*)iAp2MarshalGetBytes(iAp2Marshal, &(pMiAttrResGr->MediaItemMediaTypeCount));
                MediaItemAttrMediaType* mediaType = (MediaItemAttrMediaType*)pMiAttrResGr->mediaType;
                uint32_t currentIndex = 0;
                for(currentIndex = 0; currentIndex < pMiAttrResGr->MediaItemMediaTypeCount ; currentIndex++)
                {
                    //LOGD("mediaType.value : %u", mediaType->value);
                    mediaType = mediaType + 1;
                }
                //mediaType = NULL;

        }

        pMiAttrResGr->rating.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->rating.bPresent == TRUE) {
            pMiAttrResGr->rating.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->rating value[%d]", pMiAttrResGr->rating.value);
        }

        pMiAttrResGr->playbackDurMs.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->playbackDurMs.bPresent == TRUE) {
            pMiAttrResGr->playbackDurMs.value = iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    playbackDurMs.value[%u]", pMiAttrResGr->playbackDurMs.value);
        }

        pMiAttrResGr->albumPersistId.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->albumPersistId.bPresent == TRUE) {
            pMiAttrResGr->albumPersistId.value = iAp2MarshalGetUint64(iAp2Marshal);
            //LOGD("    pMiAttrResGr->albumPersistId value[%llu]", pMiAttrResGr->albumPersistId.value);
        }

        pMiAttrResGr->albumTitle.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->albumTitle.bPresent == TRUE) {
            pMiAttrResGr->albumTitle.value = iAp2MarshalGetBytes(iAp2Marshal, &pMiAttrResGr->albumTitle.size);
            //LOGD("    pMiAttrResGr->albumTitle size[%u] value[%s]", pMiAttrResGr->albumTitle.size, pMiAttrResGr->albumTitle.value);
        }

        pMiAttrResGr->albumTrackNo.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->albumTrackNo.bPresent == TRUE) {
            pMiAttrResGr->albumTrackNo.value = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->albumTrackNo value[%u]", pMiAttrResGr->albumTrackNo.value);
        }

        pMiAttrResGr->albumTrackCount.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->albumTrackCount.bPresent == TRUE) {
            pMiAttrResGr->albumTrackCount.value = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->albumTrackCount value[%u]", pMiAttrResGr->albumTrackCount.value);
        }

        pMiAttrResGr->albumDiscNo.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->albumDiscNo.bPresent == TRUE) {
            pMiAttrResGr->albumDiscNo.value = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->albumDiscNo value[%u]", pMiAttrResGr->albumDiscNo.value);
        }

        pMiAttrResGr->albumDiscCount.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->albumDiscCount.bPresent == TRUE){
            pMiAttrResGr->albumDiscCount.value = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->albumDiscCount value[%u]", pMiAttrResGr->albumDiscCount.value);
        }
        pMiAttrResGr->artistPersistId.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->artistPersistId.bPresent == TRUE) {
            pMiAttrResGr->artistPersistId.value = iAp2MarshalGetUint64(iAp2Marshal);
            //LOGD("    pMiAttrResGr->artistPersistId value[%llu]", pMiAttrResGr->artistPersistId.value);
        }

        pMiAttrResGr->artist.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->artist.bPresent == TRUE) {
            pMiAttrResGr->artist.value = iAp2MarshalGetBytes(iAp2Marshal, &pMiAttrResGr->artist.size);
            //LOGD("    pMiAttrResGr->artist size[%u] value[%s]", pMiAttrResGr->artist.size, pMiAttrResGr->artist.value);
        }

        pMiAttrResGr->albumArtistPersistId.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->albumArtistPersistId.bPresent == TRUE) {
            pMiAttrResGr->albumArtistPersistId.value = iAp2MarshalGetUint64(iAp2Marshal);
            //LOGD("    pMiAttrResGr->albumArtistPersistId value[%llu]", pMiAttrResGr->albumArtistPersistId.value);
        }

        pMiAttrResGr->albumArtist.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->albumArtist.bPresent == TRUE) {
            pMiAttrResGr->albumArtist.value = iAp2MarshalGetBytes(iAp2Marshal, &pMiAttrResGr->albumArtist.size);
            //LOGD("    pMiAttrResGr->albumArtist size[%u] value[%s]", pMiAttrResGr->albumArtist.size, pMiAttrResGr->albumArtist.value);
        }

        pMiAttrResGr->genrePersistId.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->genrePersistId.bPresent == TRUE) {
            pMiAttrResGr->genrePersistId.value = iAp2MarshalGetUint64(iAp2Marshal);
            //LOGD("    pMiAttrResGr->genrePersistId value[%llu]", pMiAttrResGr->genrePersistId.value);
        }

        pMiAttrResGr->genre.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->genre.bPresent == TRUE) {
            pMiAttrResGr->genre.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->genre value[%u]", pMiAttrResGr->genre.value);
        }

        pMiAttrResGr->composerPersistId.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->composerPersistId.bPresent == TRUE) {
            pMiAttrResGr->composerPersistId.value = iAp2MarshalGetUint64(iAp2Marshal);
            //LOGD("    pMiAttrResGr->composerPersistId value[%llu]", pMiAttrResGr->composerPersistId.value);
        }

        pMiAttrResGr->composer.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->composer.bPresent == TRUE) {
            pMiAttrResGr->composer.value = iAp2MarshalGetBytes(iAp2Marshal, &pMiAttrResGr->composer.size);
            //LOGD("    pMiAttrResGr->composer size[%u] value[%s]", pMiAttrResGr->composer.size, pMiAttrResGr->composer.value);
        }

        pMiAttrResGr->isPartofCompilation.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->isPartofCompilation.bPresent == TRUE) {
            pMiAttrResGr->isPartofCompilation.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->isPartofCompilation value[%u]", pMiAttrResGr->isPartofCompilation.value);
        }

        pMiAttrResGr->isLikeSurpported.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->isLikeSurpported.bPresent == TRUE) {
            pMiAttrResGr->isLikeSurpported.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->isLikeSurpported value[%u]", pMiAttrResGr->isLikeSurpported.value);
        }

        pMiAttrResGr->isBanSupported.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->isBanSupported.bPresent == TRUE){
            pMiAttrResGr->isBanSupported.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->isBanSupported value[%u]", pMiAttrResGr->isBanSupported.value);
        }

        pMiAttrResGr->isLiked.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->isLiked.bPresent == TRUE) {
            pMiAttrResGr->isLiked.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->isLiked value[%u]", pMiAttrResGr->isLiked.value);
        }

        pMiAttrResGr->isBanned.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->isBanned.bPresent == TRUE) {
            pMiAttrResGr->isBanned.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->isBanned value[%u]", pMiAttrResGr->isBanned.value);
        }

        pMiAttrResGr->isResidOnDev.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->isResidOnDev.bPresent == TRUE) {
            pMiAttrResGr->isResidOnDev.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->isResidOnDev value[%u]", pMiAttrResGr->isResidOnDev.value);
        }

        pMiAttrResGr->ArtworkFileTransferID.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->ArtworkFileTransferID.bPresent == TRUE) {
            pMiAttrResGr->ArtworkFileTransferID.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            LOGD("    pMiAttrResGr->ArtworkFileTransferID value[%u]", pMiAttrResGr->ArtworkFileTransferID.value);
        }

        pMiAttrResGr->ChapterCount.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pMiAttrResGr->ChapterCount.bPresent == TRUE) {
            pMiAttrResGr->ChapterCount.value = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pMiAttrResGr->ArtworkFileTransferID value[%u]", pMiAttrResGr->ArtworkFileTransferID.value);
        }

}

void ParsingEventPbAttrResGr(PlaybackAttrResGroup* pPbAttrResGr, MarshalIo* iAp2Marshal)
{

        pPbAttrResGr->status.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->status.bPresent == TRUE) {
            pPbAttrResGr->status.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            LOGD("    pPbAttrResGr->status.value[%u]", pPbAttrResGr->status.value);
        }

        pPbAttrResGr->elapsedTimeMs.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->elapsedTimeMs.bPresent == TRUE) {
            pPbAttrResGr->elapsedTimeMs.value = iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pPbAttrResGr->elapsedTimeMs.value[%u]", pPbAttrResGr->elapsedTimeMs.value);
        }

        pPbAttrResGr->queueIdx.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->queueIdx.bPresent == TRUE) {
            pPbAttrResGr->queueIdx.value = iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pPbAttrResGr->queueIdx.value[%u]", pPbAttrResGr->queueIdx.value);
        }

        pPbAttrResGr->queueCount.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->queueCount.bPresent == TRUE) {
            pPbAttrResGr->queueCount.value = iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pPbAttrResGr->queueCount.value[%u]", pPbAttrResGr->queueCount.value);
        }

        pPbAttrResGr->queueChapterIdx.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->queueChapterIdx.bPresent == TRUE) {
            pPbAttrResGr->queueChapterIdx.value = iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pPbAttrResGr->queueChapterIdx.value[%u]", pPbAttrResGr->queueChapterIdx.value);
        }

        pPbAttrResGr->shuffleMode.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->shuffleMode.bPresent == TRUE) {
            pPbAttrResGr->shuffleMode.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pPbAttrResGr->shuffleMode.value[%u]", pPbAttrResGr->shuffleMode.value);
        }

        pPbAttrResGr->repeatMode.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->repeatMode.bPresent == TRUE) {
            pPbAttrResGr->repeatMode.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pPbAttrResGr->repeatMode.value[%u]", pPbAttrResGr->shuffleMode.value);
        }

        pPbAttrResGr->appName.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->appName.bPresent == TRUE) {
            pPbAttrResGr->appName.value = iAp2MarshalGetBytes(iAp2Marshal, &pPbAttrResGr->appName.size);
            //LOGD("    pPbAttrResGr->appName size[%u] value[%s]", pPbAttrResGr->appName.size, pPbAttrResGr->appName.value);
        }

        pPbAttrResGr->pbMediaLibUniqueId.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->pbMediaLibUniqueId.bPresent == TRUE) {
            pPbAttrResGr->pbMediaLibUniqueId.value = iAp2MarshalGetBytes(iAp2Marshal, &pPbAttrResGr->pbMediaLibUniqueId.size);
            //LOGD("    pPbAttrResGr->pbMediaLibUniqueId.value[%s]", pPbAttrResGr->pbMediaLibUniqueId.value);
        }

        pPbAttrResGr->pbiTunesRadioAd.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->pbiTunesRadioAd.bPresent == TRUE) {
            pPbAttrResGr->pbiTunesRadioAd.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pPbAttrResGr->pbiTunesRadioAd.value[%u]", pPbAttrResGr->pbiTunesRadioAd.value);
        }
        pPbAttrResGr->pbiTunesRadioStationName.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->pbiTunesRadioStationName.bPresent == TRUE) {
            pPbAttrResGr->pbiTunesRadioStationName.value = iAp2MarshalGetBytes(iAp2Marshal,&pPbAttrResGr->pbiTunesRadioStationName.size);
            //LOGD("    pPbAttrResGr->pbiTunesRadioStationName.value[%u]", pPbAttrResGr->pbiTunesRadioStationName.value);
        }

        pPbAttrResGr->pbiTunesRadioStationMediaPlaylistPersistentID.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->pbiTunesRadioStationMediaPlaylistPersistentID.bPresent == TRUE) {
            pPbAttrResGr->pbiTunesRadioStationMediaPlaylistPersistentID.value = iAp2MarshalGetUint64(iAp2Marshal);
            //LOGD("    pPbAttrResGr->pbiTunesRadioStationMediaPlaylistPersistentID.value[%llu]", pPbAttrResGr->pbiTunesRadioStationMediaPlaylistPersistentID.value)
        }

        pPbAttrResGr->Speed.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->Speed.bPresent == TRUE) {
            pPbAttrResGr->Speed.value = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
            //LOGD("    pPbAttrResGr->Speed.value[%u]",pPbAttrResGr->Speed.value);
        }

        pPbAttrResGr->SetElapsedTimeAvailable = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("    pPbAttrResGr->SetElapsedTimeAvailable [%u]", pPbAttrResGr->SetElapsedTimeAvailable);

        pPbAttrResGr->PlaybackQueueListAvail = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("    pPbAttrResGr->PlaybackQueueListAvail [%u]", pPbAttrResGr->PlaybackQueueListAvail);

        pPbAttrResGr->QueueListTransferID.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->QueueListTransferID.bPresent == TRUE) {
            pPbAttrResGr->QueueListTransferID.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
        //    LOGD("    pPbAttrResGr->QueueListTransferID.value[%u]", pPbAttrResGr->QueueListTransferID.value);
        }

        pPbAttrResGr->AppBundleID.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
        if (pPbAttrResGr->AppBundleID.bPresent == TRUE) {
            pPbAttrResGr->AppBundleID.value = iAp2MarshalGetBytes(iAp2Marshal, &pPbAttrResGr->AppBundleID.size);
            LOGD("    pPbAttrResGr->AppBundleID.value[%s]", pPbAttrResGr->AppBundleID.value);
        }

}


// #endif
