/*
    File:       MediaLibraryUpdates.cpp
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

#include "MediaLibraryUpdates.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

MediaLibraryUpdates::MediaLibraryUpdates()
{
    LOGD("MediaLibraryUpdates()");
}

MediaLibraryUpdates::~MediaLibraryUpdates()
{
    LOGD("~MediaLibraryUpdates()");
}


void MediaLibraryUpdates::handleMediaLibraryUpdates(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    LOGD("handleMediaLibraryUpdates()");

    int result = kIap2NoErr;
    MediaLibraryUpdatesEvt event;

    LOGD("handleMediaLibraryUpdates() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    
    decodeMediaLibraryUpdates(pData, &event);

    result = handleMeidaLibraryUpdatesEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleMediaLibraryUpdates(). result[%d]", result);
}


int MediaLibraryUpdates::handleMeidaLibraryUpdatesEvent(LinkController* pLnkCtrler, int8_t session, MediaLibraryUpdatesEvt* pEvent)
{
    #define MARSHALING_BUFFER_MEDIALIBRARYUPDATE (100000)

    int result = kIap2NoErr;
    uint32_t currentIndex = 0;
    LOGD("handleMeidaLibraryUpdatesEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(MARSHALING_BUFFER_MEDIALIBRARYUPDATE);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, MARSHALING_BUFFER_MEDIALIBRARYUPDATE, sizeof(EventHeader));
    
    iAp2MarshalPutBytes(&iAp2Marshal, pEvent->MediaLibraryUniqueIdentifier_update.value, pEvent->MediaLibraryUniqueIdentifier_update.size);
    //LOGD("[MeidaLibraryUpdatesEvent] pEvent->MediaLibraryUniqueIdentifier_update.value  %s", pEvent->MediaLibraryUniqueIdentifier_update.value);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->MediaLibraryRevision.bPresent);
    if(pEvent->MediaLibraryRevision.bPresent == TRUE){
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->MediaLibraryRevision.value, pEvent->MediaLibraryRevision.size);
        //LOGD("[MeidaLibraryUpdatesEvent] pEvent->MediaLibraryRevision.value  %s", pEvent->MediaLibraryRevision.value);
    }
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->nMediaItemGroupCount);
    //LOGD("[MeidaLibraryUpdatesEvent] pEvent->nMediaItemGroupCount %u", pEvent->nMediaItemGroupCount);
    MediaItemGroup* pMediaItemGroup     = (MediaItemGroup*) pEvent->pMediaItemGroup;
    if( 0< pEvent->nMediaItemGroupCount)
    {
        //iAp2MarshalPutBytes(&iAp2Marshal, pEvent->pMediaItemGroup, pEvent->nMediaItemGroupSize);
        for(currentIndex = 0 ; currentIndex < pEvent->nMediaItemGroupCount; currentIndex++)    
        {            
            //LOGD("currentIndex %u", currentIndex);
            ParsingMediaLibraryUpdateEventMediaItemGroup(&iAp2Marshal, pMediaItemGroup);
            //LOGD("### free  pMediaItemGroup->pMediaItemMediaType : %p###", pMediaItemGroup->pMediaItemMediaType);
            free(pMediaItemGroup->pMediaItemMediaType);
            pMediaItemGroup = pMediaItemGroup + 1;
        }        
        
    }    
    //free(pEvent->pMediaItemGroup);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->nMediaPlaylistGroupCount);
    //LOGD("[MeidaLibraryUpdatesEvent] pEvent->nMediaPlaylistGroupCount %u", pEvent->nMediaPlaylistGroupCount);
    MediaPlaylistGroup* pMediaPlaylistGroup     = (MediaPlaylistGroup*) pEvent->pMediaPlaylistGroup;
    if( 0< pEvent->nMediaPlaylistGroupCount)
    {
        //iAp2MarshalPutBytes(&iAp2Marshal, pEvent->pMediaItemGroup, pEvent->nMediaItemGroupSize);
        for(currentIndex = 0 ; currentIndex < pEvent->nMediaPlaylistGroupCount; currentIndex++)    
        {                    
            ParsingMediaLibraryUpdateEventMediaPlaylistGroup(&iAp2Marshal, pMediaPlaylistGroup);
            pMediaPlaylistGroup = pMediaPlaylistGroup + 1;
        }
    }    
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->nMediaItemDeletePersistentIdentifierCount);
    //LOGD("[MeidaLibraryUpdatesEvent] pEvent->nMediaItemDeletePersistentIdentifierCount %u", pEvent->nMediaItemDeletePersistentIdentifierCount);
    MediaItemDeletePersistentIdentifierParam* pMediaItemDeletePersistentIdentifier = NULL;
    if(0 < pEvent->nMediaItemDeletePersistentIdentifierCount)
    {
        pMediaItemDeletePersistentIdentifier = pEvent->pMediaItemDeletePersistentIdentifier;
        iAp2MarshalPutBytes(&iAp2Marshal, (uint8_t*)pEvent->pMediaItemDeletePersistentIdentifier , pEvent->nMediaItemDeletePersistentIdentifierSize);
       
        for(currentIndex = 0 ; currentIndex < pEvent->nMediaItemDeletePersistentIdentifierCount; currentIndex++) {
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemDeletePersistentIdentifier[%u].value %llu",  currentIndex, pMediaItemDeletePersistentIdentifier->value);
            pMediaItemDeletePersistentIdentifier = pMediaItemDeletePersistentIdentifier + 1;            
        }
    }
    //free(pEvent->pMediaItemDeletePersistentIdentifier);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->nMediaPlaylistDeletePersistentItdentifierCount);
    //LOGD("[MeidaLibraryUpdatesEvent] pEvent->nMediaPlaylistDeletePersistentItdentifierCount %u", pEvent->nMediaPlaylistDeletePersistentItdentifierCount);
    MediaPlaylistDeletePersistentItdentifierParam* pMediaPlaylistDeletePersistentItdentifier = NULL;
    if(0 < pEvent->nMediaPlaylistDeletePersistentItdentifierCount)
    {
        pMediaPlaylistDeletePersistentItdentifier = pEvent->pMediaPlaylistDeletePersistentItdentifier;
        iAp2MarshalPutBytes(&iAp2Marshal, (uint8_t*)pEvent->pMediaPlaylistDeletePersistentItdentifier , pEvent->nMediaPlaylistDeletePersistentItdentifierSize);

        for(currentIndex = 0 ; currentIndex < pEvent->nMediaPlaylistDeletePersistentItdentifierCount; currentIndex++) {
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaPlaylistDeletePersistentItdentifier[%u].value %llu",  currentIndex, pMediaPlaylistDeletePersistentItdentifier->value);
            pMediaPlaylistDeletePersistentItdentifier = pMediaPlaylistDeletePersistentItdentifier + 1;
        }
    }
    //free(pEvent->pMediaPlaylistDeletePersistentItdentifier);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->MediaLibraryReset);
    //LOGD("[MeidaLibraryUpdatesEvent] pEvent->MediaLibraryReset %u", pEvent->MediaLibraryReset);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->MediaLibraryUpdateProgress.bPresent);
    if( pEvent->MediaLibraryUpdateProgress.bPresent == TRUE){
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->MediaLibraryUpdateProgress.value);
        //LOGD("[MeidaLibraryUpdatesEvent] pEvent->MediaLibraryUpdateProgress.value %u", pEvent->MediaLibraryUpdateProgress.value);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->MediaLibraryIsHidingRemoteItems);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtMediaLibraryUpdates;
    mEventHeader->dataSize = byteNum;

    LOGD("    evt[%u] dataSize[%u]", pIpcData[0], pIpcData[2]);

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(session, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGE("    ERROR : send event fail. result[%d]", result);
    }

    //LOGD("### free pEvent->pMediaItemGroup : %p", pEvent->pMediaItemGroup);
    //LOGD("### free pEvent->pMeidaPlaylistGroup : %p", pEvent->pMeidaPlaylistGroup);
    //LOGD("### free pEvent->pMediaItemDeletePersistentIdentifier : %p", pEvent->pMediaItemDeletePersistentIdentifier);
    //LOGD("### free pEvent->pMediaPlaylistDeletePersistentItdentifier : %p", pEvent->pMediaPlaylistDeletePersistentItdentifier);
    free(pEvent->pMediaItemGroup);
    free(pEvent->pMediaPlaylistGroup);
    free(pEvent->pMediaItemDeletePersistentIdentifier);
    free(pEvent->pMediaPlaylistDeletePersistentItdentifier);
    free(pIpcData);
    return result;
}


void MediaLibraryUpdates::ParsingMediaLibraryUpdateEventMediaItemGroup(MarshalIo* iAp2Marshal, MediaItemGroup* pMediaItemGroup)
{
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemPersistentIdentifier.bPresent);        
        if (pMediaItemGroup->MediaItemPersistentIdentifier.bPresent == TRUE) {
            iAp2MarshalPutUint64(iAp2Marshal, pMediaItemGroup->MediaItemPersistentIdentifier.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemPersistentIdentifier.value %llu", pMediaItemGroup->MediaItemPersistentIdentifier.value);
        }
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemTitle.bPresent);
        if (pMediaItemGroup->MediaItemTitle.bPresent == TRUE){
            iAp2MarshalPutBytes(iAp2Marshal, pMediaItemGroup->MediaItemTitle.value, pMediaItemGroup->MediaItemTitle.size);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemTitle.value %s", pMediaItemGroup->MediaItemTitle.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->nMediaItemMediaTypeCount);
        //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->nMediaItemMediaTypeCount %u", pMediaItemGroup->nMediaItemMediaTypeCount);
        if(0 < pMediaItemGroup->nMediaItemMediaTypeCount )
        {
            iAp2MarshalPutBytes(iAp2Marshal, (uint8_t*)pMediaItemGroup->pMediaItemMediaType, pMediaItemGroup->nMediaItemMediaTypeSize);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->pMediaItemMediaType %u", pMediaItemGroup->pMediaItemMediaType[0]);
        }
        //free(pMediaItemGroup->pMediaItemMediaType);
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemRating.bPresent);    
        if (pMediaItemGroup->MediaItemRating.bPresent == TRUE){
            iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemRating.value); 
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemRating.value %u", pMediaItemGroup->MediaItemRating.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.bPresent);    
        if (pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.bPresent == TRUE){
            iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.value %u", pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.value);            
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumPersistentIdentifier.bPresent);    
        if (pMediaItemGroup->MediaItemAlbumPersistentIdentifier.bPresent == TRUE){
            iAp2MarshalPutUint64(iAp2Marshal, pMediaItemGroup->MediaItemAlbumPersistentIdentifier.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemAlbumPersistentIdentifier.value %llu", pMediaItemGroup->MediaItemAlbumPersistentIdentifier.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumTitle.bPresent);    
        if (pMediaItemGroup->MediaItemAlbumTitle.bPresent == TRUE){
            iAp2MarshalPutBytes(iAp2Marshal, pMediaItemGroup->MediaItemAlbumTitle.value,  pMediaItemGroup->MediaItemAlbumTitle.size);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemAlbumTitle.value %s", pMediaItemGroup->MediaItemAlbumTitle.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumTrackNumber.bPresent);    
        if (pMediaItemGroup->MediaItemAlbumTrackNumber.bPresent == TRUE){
            iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumTrackNumber.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemAlbumTrackNumber.value %u", pMediaItemGroup->MediaItemAlbumTrackNumber.value);
        }    

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumTrackCount.bPresent);    
        if (pMediaItemGroup->MediaItemAlbumTrackCount.bPresent == TRUE){
            iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumTrackCount.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemAlbumTrackCount.value %u", pMediaItemGroup->MediaItemAlbumTrackCount.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumDiscNumber.bPresent);    
        if (pMediaItemGroup->MediaItemAlbumDiscNumber.bPresent == TRUE){
            iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumDiscNumber.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemAlbumDiscNumber.value %u", pMediaItemGroup->MediaItemAlbumDiscNumber.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumDiscCount.bPresent);    
        if (pMediaItemGroup->MediaItemAlbumDiscCount.bPresent == TRUE){
            iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumDiscCount.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemAlbumDiscCount.value %u", pMediaItemGroup->MediaItemAlbumDiscCount.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemArtistPersistentIdentifier.bPresent);    
        if (pMediaItemGroup->MediaItemArtistPersistentIdentifier.bPresent == TRUE){
            iAp2MarshalPutUint64(iAp2Marshal, pMediaItemGroup->MediaItemArtistPersistentIdentifier.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemArtistPersistentIdentifier.value %llu", pMediaItemGroup->MediaItemArtistPersistentIdentifier.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemArtist.bPresent);    
        if (pMediaItemGroup->MediaItemArtist.bPresent == TRUE){
            iAp2MarshalPutBytes(iAp2Marshal, pMediaItemGroup->MediaItemArtist.value, pMediaItemGroup->MediaItemArtist.size);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemArtist.value %s", pMediaItemGroup->MediaItemArtist.value);
        }
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.bPresent);    
        if (pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.bPresent == TRUE){
            iAp2MarshalPutUint64(iAp2Marshal, pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.value %llu", pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.value);
        }
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemAlbumAritist.bPresent);    
        if (pMediaItemGroup->MediaItemAlbumAritist.bPresent == TRUE){
            iAp2MarshalPutBytes(iAp2Marshal, pMediaItemGroup->MediaItemAlbumAritist.value, pMediaItemGroup->MediaItemAlbumAritist.size);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemAlbumAritist.value %s", pMediaItemGroup->MediaItemAlbumAritist.value);
        }
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemGenrePersistentIdentifier.bPresent);    
        if (pMediaItemGroup->MediaItemGenrePersistentIdentifier.bPresent == TRUE){
            iAp2MarshalPutUint64(iAp2Marshal, pMediaItemGroup->MediaItemGenrePersistentIdentifier.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemGenrePersistentIdentifier.value %llu", pMediaItemGroup->MediaItemGenrePersistentIdentifier.value);
        }
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemGenre.bPresent);    
        if (pMediaItemGroup->MediaItemGenre.bPresent == TRUE){
            iAp2MarshalPutBytes(iAp2Marshal, pMediaItemGroup->MediaItemGenre.value, pMediaItemGroup->MediaItemGenre.size);    
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemGenre.value %s", pMediaItemGroup->MediaItemGenre.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemComposerPersistentIdentifier.bPresent);    
        if (pMediaItemGroup->MediaItemComposerPersistentIdentifier.bPresent == TRUE){
            iAp2MarshalPutUint64(iAp2Marshal, pMediaItemGroup->MediaItemComposerPersistentIdentifier.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemComposerPersistentIdentifier.value %llu", pMediaItemGroup->MediaItemComposerPersistentIdentifier.value);
        }
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemComposer.bPresent);    
        if (pMediaItemGroup->MediaItemComposer.bPresent == TRUE){
            iAp2MarshalPutBytes(iAp2Marshal, pMediaItemGroup->MediaItemComposer.value, pMediaItemGroup->MediaItemComposer.size);    
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemComposer.value %s", pMediaItemGroup->MediaItemComposer.value);
        }


        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemIsPartOfCompilation);
        //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemIsPartOfCompilation %u", pMediaItemGroup->MediaItemIsPartOfCompilation);
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemIsLikeSupported);
        //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemIsLikeSupported %u", pMediaItemGroup->MediaItemIsLikeSupported);
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemIsBanSupported);
        //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemIsBanSupported %u", pMediaItemGroup->MediaItemIsBanSupported);
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemIsLiked);
        //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemIsLiked %u", pMediaItemGroup->MediaItemIsLiked);
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemIsBaned);
        //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemIsBaned %u", pMediaItemGroup->MediaItemIsBaned);
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemIsResidentOnDevice);
        //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemIsResidentOnDevice %u", pMediaItemGroup->MediaItemIsResidentOnDevice);
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.bPresent);
        
        if (pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.bPresent == TRUE){
                iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value);
                //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value %u", pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemChapterCount.bPresent);    
        if (pMediaItemGroup->MediaItemChapterCount.bPresent == TRUE){            
            iAp2MarshalPutUint32(iAp2Marshal, pMediaItemGroup->MediaItemChapterCount.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaItemGroup->MediaItemChapterCount.value :  %u", pMediaItemGroup->MediaItemChapterCount.value);
        }
}

void MediaLibraryUpdates::ParsingMediaLibraryUpdateEventMediaPlaylistGroup(MarshalIo* iAp2Marshal, MediaPlaylistGroup* pMediaPlaylistGroup)
{
        iAp2MarshalPutUint32(iAp2Marshal, pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.bPresent);
        if (pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.bPresent == TRUE) {
             iAp2MarshalPutUint64(iAp2Marshal, pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.value);
             //LOGD("[MeidaLibraryUpdatesEvent] pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.value :  %llu", pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.value);
        }
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaPlaylistGroup->MediaPlaylistName.bPresent);    
        if (pMediaPlaylistGroup->MediaPlaylistName.bPresent == TRUE){
            iAp2MarshalPutBytes(iAp2Marshal, pMediaPlaylistGroup->MediaPlaylistName.value, pMediaPlaylistGroup->MediaPlaylistName.size);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaPlaylistGroup->MediaPlaylistName.value :  %s", pMediaPlaylistGroup->MediaPlaylistName.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.bPresent);
        if (pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.bPresent == TRUE) {
            iAp2MarshalPutUint64(iAp2Marshal, pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.value :  %llu", pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaPlaylistGroup->MediaPlaylistIsGeniusMix);
        //LOGD("[MeidaLibraryUpdatesEvent] pMediaPlaylistGroup->MediaPlaylistIsGeniusMix :  %u", pMediaPlaylistGroup->MediaPlaylistIsGeniusMix);

        iAp2MarshalPutUint32(iAp2Marshal, pMediaPlaylistGroup->MediaPlaylistFolder);
        //LOGD("[MeidaLibraryUpdatesEvent] pMediaPlaylistGroup->MediaPlaylistFolder :  %u", pMediaPlaylistGroup->MediaPlaylistFolder);
        
        iAp2MarshalPutUint32(iAp2Marshal, pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.bPresent);
        if (pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.bPresent == TRUE) {
            iAp2MarshalPutUint32(iAp2Marshal, pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value);
            //LOGD("[MeidaLibraryUpdatesEvent] pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value :  %u", pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pMediaPlaylistGroup->MediaPlaylistiTunesRadioStation);
         //LOGD("[MeidaLibraryUpdatesEvent] pMediaPlaylistGroup->MediaPlaylistiTunesRadioStation :  %u", pMediaPlaylistGroup->MediaPlaylistiTunesRadioStation);
}


void MediaLibraryUpdates::decodeMediaLibraryUpdates(uint8_t* pData, MediaLibraryUpdatesEvt* pEvt)
{
    //LOGD("decodeMediaLibraryUpdates()");

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
    int nMediaItemGroupCount = 0, nMediaPlaylistGroupCount = 0;
    int nDeletePersistentIdentifierCount = 0, nMediaPlaylistDeletePersistentIdentifierCount = 0;
    int nParamStartIdx = 6;
    MediaItemGroup* pMediaItemGroup = NULL;
    MediaPlaylistGroup* pMediaPlaylistGroup = NULL;
    MediaItemDeletePersistentIdentifierParam* pMediaItemDeletePersistentIdentifier = NULL;
    MediaPlaylistDeletePersistentItdentifierParam* pMediaPlaylistDeletePersistentItdentifier = NULL;
    //MediaItemGroup init
    nMediaItemGroupCount = getMediaItemGroupCount(pData);
    pEvt->nMediaItemGroupCount = nMediaItemGroupCount;
    if(pEvt->nMediaItemGroupCount != 0)
    {
        //LOGD("pEvt->nMediaItemGroupCount : %u", pEvt->nMediaItemGroupCount);
        pEvt->pMediaItemGroup = (MediaItemGroup*)malloc(sizeof(MediaItemGroup) * nMediaItemGroupCount);
        memset(pEvt->pMediaItemGroup, 0x00, sizeof(MediaItemGroup) * nMediaItemGroupCount);
        pMediaItemGroup = (MediaItemGroup*)pEvt->pMediaItemGroup;
        pEvt->nMediaItemGroupSize = sizeof(MediaItemGroup) * nMediaItemGroupCount;
    }

    //MediaPlaylistGroup init
    nMediaPlaylistGroupCount = getMediaPlaylistGroupCount(pData);
    pEvt->nMediaPlaylistGroupCount = nMediaPlaylistGroupCount;
    if(pEvt->nMediaPlaylistGroupCount != 0)
    {
        //LOGD("pEvt->nMediaPlaylistGroupCount : %u", pEvt->nMediaPlaylistGroupCount);
        pEvt->pMediaPlaylistGroup = (MediaPlaylistGroup*)malloc(sizeof(MediaPlaylistGroup) * nMediaPlaylistGroupCount);
        memset(pEvt->pMediaPlaylistGroup, 0x00, sizeof(MediaPlaylistGroup) * nMediaPlaylistGroupCount);
        pMediaPlaylistGroup = (MediaPlaylistGroup*)pEvt->pMediaPlaylistGroup;
        pEvt->nMediaPlaylistGroupSize = sizeof(MediaPlaylistGroup) * nMediaPlaylistGroupCount;        
    }
    
    //MediaItemDeletePersistentIdentifier init
    nDeletePersistentIdentifierCount = getMediaItemDeletePersistentIdentifierCount(pData);
    pEvt->nMediaItemDeletePersistentIdentifierCount = nDeletePersistentIdentifierCount;
    if(pEvt->nMediaItemDeletePersistentIdentifierCount != 0)
    {
        //LOGD("pEvt->nDeletePersistentIdentifierCount : %u", pEvt->nMediaItemDeletePersistentIdentifierCount);
        pEvt->pMediaItemDeletePersistentIdentifier = (MediaItemDeletePersistentIdentifierParam*)malloc(sizeof(MediaItemDeletePersistentIdentifierParam) * nDeletePersistentIdentifierCount);
        memset(pEvt->pMediaItemDeletePersistentIdentifier, 0x00, sizeof(MediaItemDeletePersistentIdentifierParam) * nDeletePersistentIdentifierCount);
        pMediaItemDeletePersistentIdentifier = (MediaItemDeletePersistentIdentifierParam*)pEvt->pMediaItemDeletePersistentIdentifier;
        pEvt->nMediaItemDeletePersistentIdentifierSize = sizeof(MediaItemDeletePersistentIdentifierParam) * nDeletePersistentIdentifierCount;
    }
    
    //kParamIDMediaPlaylistDeletePersistentIdentifier init
    nMediaPlaylistDeletePersistentIdentifierCount = getMediaPlaylistDeletePersistentIdentifierCount(pData);
    pEvt->nMediaPlaylistDeletePersistentItdentifierCount = nMediaPlaylistDeletePersistentIdentifierCount;
    if(pEvt->nMediaPlaylistDeletePersistentItdentifierCount != 0)
    {
        //LOGD("pEvt->nMediaPlaylistDeletePersistentIdentifierCount : %u", pEvt->nMediaPlaylistDeletePersistentItdentifierCount);
        pEvt->pMediaPlaylistDeletePersistentItdentifier = (MediaPlaylistDeletePersistentItdentifierParam*)malloc(sizeof(MediaPlaylistDeletePersistentItdentifierParam) * nMediaPlaylistDeletePersistentIdentifierCount);
        memset(pEvt->pMediaPlaylistDeletePersistentItdentifier, 0x00, sizeof(MediaPlaylistDeletePersistentItdentifierParam) * nMediaPlaylistDeletePersistentIdentifierCount);
        pMediaPlaylistDeletePersistentItdentifier = (MediaPlaylistDeletePersistentItdentifierParam*)pEvt->pMediaPlaylistDeletePersistentItdentifier;
        pEvt->nMediaPlaylistDeletePersistentItdentifierSize = sizeof(MediaPlaylistDeletePersistentItdentifierParam) * nMediaPlaylistDeletePersistentIdentifierCount;
    }
    
    while(nMsgLength > nParamStartIdx) {
        nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);
        nParamId = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 2, 2);

        switch (nParamId)    {
            case kParamIDMediaLibraryUniqueIdentifier_update : {
                pEvt->MediaLibraryUniqueIdentifier_update.size  = nParamLength -4;
                pEvt->MediaLibraryUniqueIdentifier_update.value = (uint8_t*)&pData[nParamStartIdx+4];
                //LOGD("pEvt->MediaLibraryUniqueIdentifier_update.value : %s", pEvt->MediaLibraryUniqueIdentifier_update.value );
                break;    
            }
            case kParamIDMediaLibraryRevision : {
                pEvt->MediaLibraryRevision.bPresent = TRUE;
                pEvt->MediaLibraryRevision.size  = nParamLength -4;
                pEvt->MediaLibraryRevision.value = (uint8_t*)&pData[nParamStartIdx+4];
                //LOGD("pEvt->MediaLibraryRevision.value : %s", pEvt->MediaLibraryRevision.value );
                break;    
            }
            case kParamIDMediaItem : {                /*
                pEvt->nMediaItemGroupCount = pEvt->nMediaItemGroupCount + 1;                
                LOGD("pEvt->nMediaItemGroupCount : %u", pEvt->nMediaItemGroupCount);
                if(pEvt->nMediaItemGroupCount == 1)
                    pEvt->pMediaItemGroup = (MediaItemGroup*)realloc(NULL, sizeof(MediaItemGroup) * (pEvt->nMediaItemGroupCount));
                else
                    pEvt->pMediaItemGroup = (MediaItemGroup*)realloc(pEvt->pMediaItemGroup, sizeof(MediaItemGroup) * (pEvt->nMediaItemGroupCount));
                
                LOGD("pEvt->pMediaItemGroup : %p", pEvt->pMediaItemGroup);
                pMediaItemGroup = (MediaItemGroup *) pEvt->pMediaItemGroup;
                pMediaItemGroup = pMediaItemGroup + (pEvt->nMediaItemGroupCount - 1);
                memset(pMediaItemGroup, 0x00, sizeof(MediaItemGroup));
                LOGD("pMediaItemGroup : %p", pMediaItemGroup);
                decordMediaLibraryUpdatesMediaItem(pData, nParamStartIdx, pMediaItemGroup);    */
                decordMediaLibraryUpdatesMediaItem(pData, nParamStartIdx, pMediaItemGroup);                
                pMediaItemGroup = pMediaItemGroup + 1;
                break;    
            }
            case kParamIDMediaPlayList : {
                decordMediaLibraryUpdatesMediaPlaylist(pData, nParamStartIdx, pMediaPlaylistGroup);                
                pMediaPlaylistGroup = pMediaPlaylistGroup + 1;
                break;    
            }
            case kParamIDMediaItemDeletePersistentIdentifier : {
                pMediaItemDeletePersistentIdentifier->value = DataDecoder::getUint64FromBytes(pData, nParamStartIdx+4, 8);                
                //LOGD("pMediaItemDeletePersistentIdentifier->value : %llu", pMediaItemDeletePersistentIdentifier->value);
                pMediaItemDeletePersistentIdentifier = pMediaItemDeletePersistentIdentifier + 1;
                break;    
            }
            case kParamIDMediaPlaylistDeletePersistentIdentifier : {
                pMediaPlaylistDeletePersistentItdentifier->value = DataDecoder::getUint64FromBytes(pData, nParamStartIdx+4, 8);                
                //LOGD("pMediaPlaylistDeletePersistentItdentifier->value : %llu", pMediaPlaylistDeletePersistentItdentifier->value);
                pMediaPlaylistDeletePersistentItdentifier = pMediaPlaylistDeletePersistentItdentifier + 1;
                break;    
            }
            case  kParamIDMediaLibraryReset : {
                pEvt->MediaLibraryReset = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 4, 1);
                //LOGD("pEvt->MediaLibraryReset: %u", pEvt->MediaLibraryReset);
                break;    
            }
            case  kParamIDMediaLibraryUpdateProgress_Update : {
                pEvt->MediaLibraryUpdateProgress.bPresent = TRUE;
                pEvt->MediaLibraryUpdateProgress.value = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 4, 1);
                //LOGD("pEvt->MediaLibraryUpdateProgress.value: %u", pEvt->MediaLibraryUpdateProgress.value);
                break;    
            }
            case  kParamIDMediaLibraryIsHidingRemoteItems_Update : {
                pEvt->MediaLibraryIsHidingRemoteItems = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 4, 1);
                //LOGD("pEvt->MediaLibraryIsHidingRemoteItems : %u", pEvt->MediaLibraryIsHidingRemoteItems);
                break;    
            }    

            case  kParamIDPlayAllSongsCapable_Update : {
                pEvt->PlayAllSongsCapable = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 4, 1);
                //LOGD("pEvt->PlayAllSongsCapable : %u", pEvt->PlayAllSongsCapable);
                break;    
            }
            
            default:
                LOGD("    WARNING : unknowd nParamId[0x%x]", nParamId);
                break;        
        
            }
            nParamStartIdx = nParamStartIdx + nParamLength;    
        }
}

int MediaLibraryUpdates::getMediaItemGroupCount(uint8_t* pData)
{
    //LOGD("getMediaItemGroupCount()");
    
    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return 0;
    }

    int nMsgLength;
    nMsgLength = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);
    
    int nParamId, nParamLength;
    int nParamStartIdx = 6;
    int count = 0;
    
    
    while(nMsgLength > nParamStartIdx) {
        nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);
        nParamId = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 2, 2);

        if (nParamId == kParamIDMediaItem) {
            count++;
        }
        nParamStartIdx = nParamStartIdx + nParamLength;
    }
    //LOGD("getMediaItemGroupCount(): %u",count);
    return count;
}


int MediaLibraryUpdates::getMediaPlaylistGroupCount(uint8_t* pData)
{
    //LOGD("getMediaPlaylistGroupCount()");

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return 0;
    }

    int nMsgLength;
    nMsgLength = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);
    
    int nParamId, nParamLength;
    int nParamStartIdx = 6;
    int count = 0;    
    
    while(nMsgLength > nParamStartIdx) {
        nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);
        nParamId = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 2, 2);

        if (nParamId == kParamIDMediaPlayList) {
            count++;
        }
        nParamStartIdx = nParamStartIdx + nParamLength;
    }
    //LOGD("getMediaPlaylistGroupCount(): %u",count);
    return count;
}

int MediaLibraryUpdates::getMediaItemDeletePersistentIdentifierCount(uint8_t* pData)
{
    //LOGD("getMediaItemDeletePersistentIdentifierCount()");

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return 0;
    }

    int nMsgLength;
    nMsgLength = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);
    
    int nParamId, nParamLength;
    int nParamStartIdx = 6;
    int count = 0;    
    
    while(nMsgLength > nParamStartIdx) {
        nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);
        nParamId = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 2, 2);

        if (nParamId == kParamIDMediaItemDeletePersistentIdentifier) {
            count++;
        }
        nParamStartIdx = nParamStartIdx + nParamLength;
    }
    //LOGD("getMediaItemDeletePersistentIdentifierCount(): %u",count);
    return count;
}

int MediaLibraryUpdates::getMediaPlaylistDeletePersistentIdentifierCount(uint8_t* pData)
{
    //LOGD("getMediaPlaylistDeletePersistentIdentifierCount()");

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return 0;
    }

    int nMsgLength;
    nMsgLength = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);
    
    int nParamId, nParamLength;
    int nParamStartIdx = 6;
    int count = 0;    
    
    while(nMsgLength > nParamStartIdx) {
        nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);
        nParamId = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 2, 2);

        if (nParamId == kParamIDMediaPlaylistDeletePersistentIdentifier) {
            count++;
        }
        nParamStartIdx = nParamStartIdx + nParamLength;
    }
    //LOGD("getMediaPlaylistDeletePersistentIdentifierCount(): %u",count);
    return count;
}


int MediaLibraryUpdates::getMediItemTypeCount(uint8_t* pData, int nParamStartIdx)
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

void MediaLibraryUpdates::decordMediaLibraryUpdatesMediaItem(uint8_t* pData, int nParamStartIdx, MediaItemGroup* pMediaItemGroup)
{
    LOGD("decordMediaLibraryUpdatesMediaItem()");

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    if (pMediaItemGroup == NULL) {
        LOGE("    Error : pMediaItemGroup is NULL");
        return;
    }

    int nParamLength;
    nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);

    int nSubParamId, nSubParamLength;
    int nSubParamStartIdx = nParamStartIdx+4;
    int nMediItemTypeCount = 0;
    MediaItemMediaTypeParam* pMediaItemMediaType = NULL;
    
    //MediItemType init
    nMediItemTypeCount = getMediItemTypeCount(pData, nParamStartIdx);    
    pMediaItemGroup->nMediaItemMediaTypeCount = nMediItemTypeCount;    
    //LOGD("pMediaItemGroup->nMediaItemMediaTypeCount : %u", pMediaItemGroup->nMediaItemMediaTypeCount);
    pMediaItemGroup->pMediaItemMediaType = (MediaItemMediaTypeParam*)malloc(sizeof(MediaItemMediaTypeParam) * nMediItemTypeCount);
    memset(pMediaItemGroup->pMediaItemMediaType, 0x00, sizeof(MediaItemMediaTypeParam) * nMediItemTypeCount);
    pMediaItemGroup->nMediaItemMediaTypeSize = sizeof(MediaItemMediaTypeParam) * nMediItemTypeCount;
    
    pMediaItemMediaType = (MediaItemMediaTypeParam*)pMediaItemGroup->pMediaItemMediaType;
    
    
    while(nParamLength > (nSubParamStartIdx - nParamStartIdx)) {
        nSubParamLength = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx, 2);
        nSubParamId = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 2, 2);
        switch (nSubParamId)    {
            case kParamIDMediaItemPersistentIdentifier : {
                pMediaItemGroup->MediaItemPersistentIdentifier.bPresent = TRUE;
                pMediaItemGroup->MediaItemPersistentIdentifier.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx + 4, 8);                
                //LOGD("pMediaItemGroup->MediaItemPersistentIdentifier.value : %llu",pMediaItemGroup->MediaItemPersistentIdentifier.value);
                break;    
            }
            case kParamIDMediaItemTitle : {
                pMediaItemGroup->MediaItemTitle.bPresent =  TRUE;
                pMediaItemGroup->MediaItemTitle.size = nSubParamLength-4;
                pMediaItemGroup->MediaItemTitle.value = (uint8_t*)&pData[nSubParamStartIdx+4];
                //LOGD("pMediaItemGroup->MediaItemTitle.value: %s",pMediaItemGroup->MediaItemTitle.value);
                break;    
            }
            case kParamIDMediaItemMediaType : {/*
                pMediaItemGroup->nMediaItemMediaTypeCount = pMediaItemGroup->nMediaItemMediaTypeCount + 1;
                
                LOGD("pMediaItemGroup->nMediaItemMediaTypeCount : %u", pMediaItemGroup->nMediaItemMediaTypeCount);
                if(pMediaItemGroup->nMediaItemMediaTypeCount == 1)
                    pMediaItemGroup->pMediaItemMediaType = (uint8_t*)realloc(NULL, sizeof(uint8_t) * (pMediaItemGroup->nMediaItemMediaTypeCount));                        
                else
                    pMediaItemGroup->pMediaItemMediaType = (uint8_t*)realloc(pMediaItemGroup->pMediaItemMediaType, sizeof(uint8_t) * (pMediaItemGroup->nMediaItemMediaTypeCount));
                
                pMediaItemMediaType = (uint8_t*)pMediaItemGroup->pMediaItemMediaType;
                
                LOGD("pMediaItemMediaType 1: %p", pMediaItemMediaType);
                pMediaItemMediaType = pMediaItemMediaType + (pMediaItemGroup->nMediaItemMediaTypeCount - 1);
                memset(pMediaItemMediaType, 0x00, sizeof(MediaItemGroup));
                LOGD("pMediaItemMediaType 2: %p", pMediaItemMediaType);
                
                int MediaTypeValue = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                memcpy(pMediaItemMediaType, &MediaTypeValue, sizeof(uint8_t));*/
                //LOGD("pMediaItemMediaType->value : %u", pMediaItemMediaType->value);
                pMediaItemMediaType->value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                pMediaItemMediaType = pMediaItemMediaType + 1;
                //LOGD("pMediaItemMediaType->value : %u", pMediaItemMediaType->value);
                break;    
            }
            case kParamIDMediaItemRating : {
                pMediaItemGroup->MediaItemRating.bPresent =  TRUE;
                pMediaItemGroup->MediaItemRating.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                //LOGD("pMediaItemGroup->MediaItemRating.value : %u", pMediaItemGroup->MediaItemRating.value);
                break;    
            }
            case kParamIDMediaItemPlaybackDurationInMilliseconds : {
                pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.bPresent =  TRUE;
                pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 4);
                //LOGD("pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.value : %u", pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.value);
                break;    
            }
            case kParamIDMediaItemAlbumPersistentIdentifier : {
                pMediaItemGroup->MediaItemAlbumPersistentIdentifier.bPresent = TRUE;
                pMediaItemGroup->MediaItemAlbumPersistentIdentifier.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx + 4, 8);
                //LOGD("pMediaItemGroup->MediaItemAlbumPersistentIdentifier.value : %llu", pMediaItemGroup->MediaItemAlbumPersistentIdentifier.value);
                break;    
            }
            case kParamIDMediaItemAlbumTitle : {
                pMediaItemGroup->MediaItemAlbumTitle.bPresent =  TRUE;
                pMediaItemGroup->MediaItemAlbumTitle.size = nSubParamLength -4;
                pMediaItemGroup->MediaItemAlbumTitle.value = (uint8_t*)&pData[nSubParamStartIdx+4];
                //LOGD("pMediaItemGroup->MediaItemAlbumTitle.value : %s", pMediaItemGroup->MediaItemAlbumTitle.value);
                break;    
            }
            case kParamIDMediaItemAlbumTrackNumber : {
                pMediaItemGroup->MediaItemAlbumTrackNumber.bPresent =  TRUE;
                pMediaItemGroup->MediaItemAlbumTrackNumber.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 2);
                //LOGD("pMediaItemGroup->MediaItemAlbumTrackNumber.value : %u", pMediaItemGroup->MediaItemAlbumTrackNumber.value);
                break;    
            }
            case kParamIDMediaItemAlbumTrackCount : {
                pMediaItemGroup->MediaItemAlbumTrackCount.bPresent =  TRUE;
                pMediaItemGroup->MediaItemAlbumTrackCount.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 2);
                //LOGD("pMediaItemGroup->MediaItemAlbumTrackCount.value : %u", pMediaItemGroup->MediaItemAlbumTrackCount.value);
                break;    
            }
            case kParamIDMediaItemAlbumDiscNumber : {
                pMediaItemGroup->MediaItemAlbumDiscNumber.bPresent =  TRUE;
                pMediaItemGroup->MediaItemAlbumDiscNumber.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 2);
                //LOGD("pMediaItemGroup->MediaItemAlbumDiscNumber.value : %u", pMediaItemGroup->MediaItemAlbumDiscNumber.value);
                break;    
            }
            case kParamIDMediaItemAlbumDiscCount : {
                pMediaItemGroup->MediaItemAlbumDiscCount.bPresent =  TRUE;
                pMediaItemGroup->MediaItemAlbumDiscCount.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 2);
                //LOGD("pMediaItemGroup->MediaItemAlbumDiscCount.value : %u", pMediaItemGroup->MediaItemAlbumDiscCount.value);
                break;    
            }
            case kParamIDMediaItemArtistPersistentIdentifier : {
                pMediaItemGroup->MediaItemArtistPersistentIdentifier.bPresent = TRUE;
                pMediaItemGroup->MediaItemArtistPersistentIdentifier.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx + 4, 8);
                //LOGD("pMediaItemGroup->MediaItemArtistPersistentIdentifier.value : %llu", pMediaItemGroup->MediaItemArtistPersistentIdentifier.value);
                break;    
            }
            case kParamIDMediaItemArtist : {
                pMediaItemGroup->MediaItemArtist.bPresent = TRUE;
                pMediaItemGroup->MediaItemArtist.size = nSubParamLength -4;
                pMediaItemGroup->MediaItemArtist.value = (uint8_t*)&pData[nSubParamStartIdx+4];
                //LOGD("pMediaItemGroup->MediaItemArtist.value : %s", pMediaItemGroup->MediaItemArtist.value);
                break;    
            }
            case kParamIDMediaItemAlbumAritistPersistentIdentifier : {
                pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.bPresent = TRUE;
                pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx + 4, 8);
                //LOGD("pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.value : %llu", pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.value);
                break;    
            }
            case kParamIDMediaItemAlbumAritist : {
                pMediaItemGroup->MediaItemAlbumAritist.bPresent = TRUE;
                pMediaItemGroup->MediaItemAlbumAritist.size = nSubParamLength -4;
                pMediaItemGroup->MediaItemAlbumAritist.value = (uint8_t*)&pData[nSubParamStartIdx+4];
                //LOGD("pMediaItemGroup->MediaItemAlbumAritist.value : %s", pMediaItemGroup->MediaItemAlbumAritist.value);
                break;    
            }
            case kParamIDMediaItemGenrePersistentIdentifier : {
                pMediaItemGroup->MediaItemGenrePersistentIdentifier.bPresent = TRUE;
                pMediaItemGroup->MediaItemGenrePersistentIdentifier.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx + 4, 8);
                //LOGD("pMediaItemGroup->MediaItemGenrePersistentIdentifier.value : %llu", pMediaItemGroup->MediaItemGenrePersistentIdentifier.value);
                break;    
            }
            case kParamIDMediaItemGenre : {
                pMediaItemGroup->MediaItemGenre.bPresent = TRUE;
                pMediaItemGroup->MediaItemGenre.size = nSubParamLength -4;
                pMediaItemGroup->MediaItemGenre.value = (uint8_t*)&pData[nSubParamStartIdx+4];
                //LOGD("pMediaItemGroup->MediaItemGenre.value : %s", pMediaItemGroup->MediaItemGenre.value);
                break;    
            }
            case kParamIDMediaItemComposerPersistentIdentifier : {
                pMediaItemGroup->MediaItemComposerPersistentIdentifier.bPresent = TRUE;
                pMediaItemGroup->MediaItemComposerPersistentIdentifier.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx + 4, 8);
                //LOGD("pMediaItemGroup->MediaItemComposerPersistentIdentifier.value : %llu", pMediaItemGroup->MediaItemComposerPersistentIdentifier.value);
                break;    
            }
            case kParamIDMediaItemComposer : {
                pMediaItemGroup->MediaItemComposer.bPresent = TRUE;
                pMediaItemGroup->MediaItemComposer.size = nSubParamLength -4;
                pMediaItemGroup->MediaItemComposer.value = (uint8_t*)&pData[nSubParamStartIdx+4];
                //LOGD("pMediaItemGroup->MediaItemComposer.value : %s", pMediaItemGroup->MediaItemComposer.value);
                break;    
            }
            case kParamIDMediaItemIsPartOfCompilation : {
                pMediaItemGroup->MediaItemIsPartOfCompilation = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                //LOGD("pMediaItemGroup->MediaItemIsPartOfCompilation : %u", pMediaItemGroup->MediaItemIsPartOfCompilation);
                break;    
            }
            case kParamIDMediaItemIsLikeSupported : {
                pMediaItemGroup->MediaItemIsLikeSupported = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                //LOGD("pMediaItemGroup->MediaItemIsLikeSupported : %u", pMediaItemGroup->MediaItemIsLikeSupported);
                break;    
            }
            case kParamIDMediaItemIsBanSupported : {
                pMediaItemGroup->MediaItemIsBanSupported = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                //LOGD("pMediaItemGroup->MediaItemIsBanSupported : %u", pMediaItemGroup->MediaItemIsBanSupported);
                break;    
            }            
            case kParamIDMediaItemIsLiked : {
                pMediaItemGroup->MediaItemIsLiked = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                //LOGD("pMediaItemGroup->MediaItemIsLiked : %u", pMediaItemGroup->MediaItemIsLiked);
                break;    
            }
            case kParamIDMediaItemIsBanned : {
                pMediaItemGroup->MediaItemIsBaned = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                //LOGD("pMediaItemGroup->MediaItemIsBaned : %u", pMediaItemGroup->MediaItemIsBaned);
                break;    
            }
            case kParamIDMediaItemIsResidentOnDevice : {
                pMediaItemGroup->MediaItemIsResidentOnDevice = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);                
                break;    
            }    
            case kParamIDMediaItemArtworkFileTransferIdentifier : {
                pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.bPresent = TRUE;
                pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                //LOGD("pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value : %u", pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value);

#ifdef iAP2SERVICE_FILETRANSFER
                //LOGD(" pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value= %u", pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value);
                //pLnkCtrler->mFileTransfer.createIAP2FileTransferCreate(pLnkCtrler, pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value, RecvPath, FILETRANSFERMODE_MEDIALIBRARYUPDATE_ARTWORK);

#endif
                break;    
            }    
            case kParamIDMediaItemChapterCount : {
                pMediaItemGroup->MediaItemChapterCount.bPresent =  TRUE;
                pMediaItemGroup->MediaItemChapterCount.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 2);
                //LOGD("pMediaItemGroup->MediaItemChapterCount.value : %u", pMediaItemGroup->MediaItemChapterCount.value);
                break;
            }
    
            default:
                LOGD("    WARNING : unknowd nSubParamStartIdx[0x%x]", nSubParamStartIdx);
                break;        
        
            }
            nSubParamStartIdx = nSubParamStartIdx + nSubParamLength;
        }

}

void MediaLibraryUpdates::decordMediaLibraryUpdatesMediaPlaylist(uint8_t* pData, int nParamStartIdx, MediaPlaylistGroup* pMediaPlaylistGroup)
{
    LOGD("decordMediaLibraryUpdatesMediaPlaylist()");

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    if (pMediaPlaylistGroup == NULL) {
        LOGE("    Error : pMeidaPlaylistGroup is NULL");
        return;
    }

    int nParamLength;
    nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);

    int nSubParamId, nSubParamLength;
    int nSubParamStartIdx = nParamStartIdx + 4;

    while(nParamLength > (nSubParamStartIdx - nParamStartIdx)) {
        nSubParamLength = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx, 2);
        nSubParamId = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 2, 2);
        switch (nSubParamId)    {
            case kParamIDMediaPlaylisPersistentIdentifier : {
                pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.bPresent = TRUE;
                pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx + 4, 8);
                //LOGD(" pMeidaPlaylistGroup->MediaPlaylistPersistentIdentifier.value= %llu", pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.value);
                break;    
            }
            case kParamIDMediaPlaylistName : {
                pMediaPlaylistGroup->MediaPlaylistName.bPresent =  TRUE;
                pMediaPlaylistGroup->MediaPlaylistName.size = nSubParamLength -4;
                pMediaPlaylistGroup->MediaPlaylistName.value = (uint8_t*)&pData[nSubParamStartIdx+4];
                //LOGD(" pMediaPlaylistGroup->MediaPlaylistName.value= %s", pMediaPlaylistGroup->MediaPlaylistName.value);
                break;    
            }
            case kParamIDMediaPlaylistParentPersistentIdentifier : {
                pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.bPresent =  TRUE;
                pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.value = DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx + 4, 8);                
                //LOGD(" pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.value= %llu",pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.value);
                break;    
            }
            case kParamIDMediaPlaylistIsGeniusMix : {
                pMediaPlaylistGroup->MediaPlaylistIsGeniusMix =  DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                //LOGD("  pMediaPlaylistGroup->MediaPlaylistIsGeniusMix= %u",  pMediaPlaylistGroup->MediaPlaylistIsGeniusMix);
                break;    
            }
            case kParamIDMediaPlaylistIsFolder : {
                pMediaPlaylistGroup->MediaPlaylistFolder =  DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                //LOGD("  pMeidaPlaylistGroup->MediaPlaylistFolder= %u",  pMediaPlaylistGroup->MediaPlaylistFolder);
                break;    
            }
            case kParamIDMediaPlaylistContainedMediaItemsFileTransferIdentifier : {
                pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.bPresent = TRUE;
                pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                //LOGD(" pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value= %u", pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value);
#ifdef iAP2SERVICE_FILETRANSFER
                //LOGD(" pMeidaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value= %u", pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value);
                //pLnkCtrler->mFileTransfer.createIAP2FileTransferCreate(pLnkCtrler, pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value, RecvPath, FILETRANSFERMODE_MEDIALIBRARYUPDATE_CONTAINITEM);

#endif                
                break;    
            }
            case kParamIDMediaPlaylistIsiTunesRadioStation : {
                pMediaPlaylistGroup->MediaPlaylistiTunesRadioStation =  DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);                
                //LOGD(" pMeidaPlaylistGroup->MediaPlaylistiTunesRadioStatione= %u", pMediaPlaylistGroup->MediaPlaylistiTunesRadioStation);
                break;    
            }
            default:
                LOGD("    WARNING : unknowd nSubParamId[0x%x]", nSubParamId);
                break;        
        
            }
            nSubParamStartIdx = nSubParamStartIdx + nSubParamLength;
        }

}
} // namespace IAP2
