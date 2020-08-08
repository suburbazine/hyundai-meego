/*
    File:       iAp2AppFrameworkMediaLibraryUpdates.c
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
#include "iAp2AppFrameworkMediaLibraryUpdates.h"
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
//    iAp2AppFrameworkMediaLibraryUpdates
//===========================================================================================================================
int iAp2AppStartMediaLibraryUpdates(iAp2AppRef inRef, StartMediaLibraryUpdatesArg* inArg)
{
        int retValue = kIap2NoErr;
    LOGD("iAp2AppStartMediaLibraryUpdates() inRef[%p]", inRef);

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
    
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*200;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);

    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    //LOGD("inArg->MediaLibraryUniqueIdentifier_Start.value : %s", inArg->MediaLibraryUniqueIdentifier_Start.value);
    //LOGD("inArg->MediaLibraryUniqueIdentifier_Start.size : %u", inArg->MediaLibraryUniqueIdentifier_Start.size);
    iAp2MarshalPutBytes(&iAp2Marshal, inArg->MediaLibraryUniqueIdentifier_Start.value, inArg->MediaLibraryUniqueIdentifier_Start.size);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->LastKnownMediaLibraryRevision.bPresent);
    if(inArg->LastKnownMediaLibraryRevision.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->LastKnownMediaLibraryRevision.value, inArg->LastKnownMediaLibraryRevision.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.bPresent);
    if (inArg->MediaItemProperties.bPresent == TRUE){
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyPersistentIdentifier);    // Always present;
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyTitle);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyMediaType);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyRating);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyPlaybackDurationInMilliseconds);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyAlbumPersistentIdentifier);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyAlbumTitle);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyAlbumTrackNumber);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyAlbumTrackCount);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyAlbumDiscNumber);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyAlbumDiscCount);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyArtistPersistentIdentifier);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyArtist);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyAlbumAritistPersistentIdentifier);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyAlbumAritist);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyGenrePersistentIdentifier);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyGenre);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyComposerPersistentIdentifier);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyComposer);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyIsPartOfCompilation);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyIsResidentOnDevice);    
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaItemProperties.MediaItemPropertyChapterCount);    
    }
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaPlaylistProperties.bPresent);
    if (inArg->MediaPlaylistProperties.bPresent == TRUE){
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaPlaylistProperties.MediaPlaylistPropertyPersistentIdentifier); // always presents
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaPlaylistProperties.MediaPlaylistPropertyName);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaPlaylistProperties.MediaPlaylistPropertyParentPersistentIdentifier);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaPlaylistProperties.MediaPlaylistPropertyIsGeniusMix);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaPlaylistProperties.MediaPlaylistPropertyIsFolder);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaPlaylistProperties.MediaPlaylistPropertyContainedMediaItemsFileTransferIdentifier);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaPlaylistProperties.MediaPlaylistPropertyIsiTunesRadioStation);//28
    }    

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaLibraryUpdateProgress);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->MediaLibraryIsHidingRemoteItems);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->PlayAllSongsCapable);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdStartMediaLibraryUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    //LOGD("### free inArg->MediaLibraryUniqueIdentifier_Start.value : %p ###", inArg->MediaLibraryUniqueIdentifier_Start.value);
    //free(inArg->MediaLibraryUniqueIdentifier_Start.value);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppStopMediaLibraryUpdates
//===========================================================================================================================
int iAp2AppStopMediaLibraryUpdates(iAp2AppRef inRef, StopMediaLibraryUpdatesArg* inArg)
{
       int retValue = kIap2NoErr;
    LOGD("iAp2AppStopMediaLibraryUpdates() inRef[%p]", inRef);

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
    
    //uint32_t ipcData[512/4];
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*100;
      uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;
    
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutBytes(&iAp2Marshal, inArg->MediaLibraryUniqueIdentifier_Stop.value, inArg->MediaLibraryUniqueIdentifier_Stop.size);
        
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pCommandHeader->cmd = kCmdStopMediaLibraryUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;

}


//===========================================================================================================================
//    iAp2ConectionHandleEventMediaLibraryUpdates
//===========================================================================================================================
int iAp2ConectionHandleEventMediaLibraryUpdates(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    uint32_t currentIndex = 0;
    LOGD("iAp2ConectionHandleEventMediaLibraryUpdates()");

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->mediaLibraryUpdates_f == NULL) {
        LOGW("    WARING : delegate->mediaLibraryUpdates_f is NULL");
        return kIap2NoErr;
    }

    MediaLibraryUpdatesCbArg* cbArg = NULL;
    cbArg = (MediaLibraryUpdatesCbArg*)malloc(sizeof(MediaLibraryUpdatesCbArg));

    if ( cbArg == NULL ) {
        LOGE("    ERROR : MediaLibraryUpdates    CbArg is NULL");
        return kIap2NoMemoryErr;
    }
    
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    memset(cbArg, 0x00, sizeof(MediaLibraryUpdatesCbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);
        
    cbArg->MediaLibraryUniqueIdentifier_update.value= iAp2MarshalGetBytes(&iAp2Marshal, &cbArg->MediaLibraryUniqueIdentifier_update.size);
    //LOGD("[iAP2AppFramework Medialibrary] cbArg->MediaLibraryUniqueIdentifier_update.value : %s ", cbArg->MediaLibraryUniqueIdentifier_update.value);
    cbArg->MediaLibraryRevision.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if(cbArg->MediaLibraryRevision.bPresent == TRUE){
        cbArg->MediaLibraryRevision.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg->MediaLibraryRevision.size);
        //LOGD("[iAP2AppFramework Medialibrary] cbArg->MediaLibraryRevision.value : %s ", cbArg->MediaLibraryRevision.value);
    }
    
    cbArg->nMediaItemGroupCount = iAp2MarshalGetUint32(&iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary] cbArg->nMediaItemGroupCount : %u ", cbArg->nMediaItemGroupCount);
    if(0 < cbArg->nMediaItemGroupCount)
    {
        cbArg->pMediaItemGroup = (MediaItemGroup*)malloc(sizeof(MediaItemGroup) * cbArg->nMediaItemGroupCount);
        if( cbArg->pMediaItemGroup != NULL ) {
            memset(cbArg->pMediaItemGroup, 0x00, sizeof(MediaItemGroup) * cbArg->nMediaItemGroupCount);
            MediaItemGroup* pMediaItemGroup     = (MediaItemGroup*) cbArg->pMediaItemGroup;
            for(currentIndex = 0 ; currentIndex < cbArg->nMediaItemGroupCount; currentIndex++)
            {
                //LOGD("pMediaItemGroup : %p", pMediaItemGroup);
                ParsingMediaLibraryUpdatesEventMediaItemGroup(&iAp2Marshal, pMediaItemGroup);
                pMediaItemGroup = pMediaItemGroup + 1;    
            }
        }
        else {
            LOGE("ERROR : malloc failed! pMediaItemGroup is NULL");
        }
    }
    
    cbArg->nMediaPlaylistGroupCount = iAp2MarshalGetUint32(&iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary] cbArg->nMediaPlaylistGroupCount : %u ", cbArg->nMediaPlaylistGroupCount);
    if(0 < cbArg->nMediaPlaylistGroupCount)
    {
        cbArg->pMediaPlaylistGroup = (MediaPlaylistGroup*)malloc(sizeof(MediaPlaylistGroup) * cbArg->nMediaPlaylistGroupCount);
        if ( cbArg->pMediaPlaylistGroup != NULL ) {
            memset(cbArg->pMediaPlaylistGroup, 0x00, sizeof(MediaPlaylistGroup) * cbArg->nMediaPlaylistGroupCount);
            MediaPlaylistGroup* pMediaPlaylistGroup     = (MediaPlaylistGroup*) cbArg->pMediaPlaylistGroup;
            for(currentIndex = 0 ; currentIndex < cbArg->nMediaPlaylistGroupCount; currentIndex++)
            {
                ParsingMediaLibraryUpdatesEventMediaPlaylistGroup(&iAp2Marshal, pMediaPlaylistGroup);
                pMediaPlaylistGroup = pMediaPlaylistGroup  + 1;    
            }
        }
        else {
            LOGE("ERROR : malloc failed! pMediaPlaylistGroup is NULL");
        }
    }
    
    cbArg->nMediaItemDeletePersistentIdentifierCount = iAp2MarshalGetUint32(&iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary] cbArg->nMediaItemDeletePersistentIdentifierCount : %u ", cbArg->nMediaItemDeletePersistentIdentifierCount);
    if(0 < cbArg->nMediaItemDeletePersistentIdentifierCount)
    {
        cbArg->pMediaItemDeletePersistentIdentifier = (MediaItemDeletePersistentIdentifierParam*)iAp2MarshalGetBytes(&iAp2Marshal, &cbArg->nMediaItemDeletePersistentIdentifierSize);
        MediaItemDeletePersistentIdentifierParam* pMediaItemDeletePersistentIdentifier = NULL;
        pMediaItemDeletePersistentIdentifier = cbArg->pMediaItemDeletePersistentIdentifier;
         for(currentIndex = 0 ; currentIndex < cbArg->nMediaItemDeletePersistentIdentifierCount; currentIndex++) {
            //LOGD("[iAP2AppFramework Medialibrary] pMediaItemDeletePersistentIdentifier[%u] value : %llu", currentIndex, pMediaItemDeletePersistentIdentifier->value);
            pMediaItemDeletePersistentIdentifier = pMediaItemDeletePersistentIdentifier  + 1;    
        }         
    }

    cbArg->nMediaPlaylistDeletePersistentItdentifierCount = iAp2MarshalGetUint32(&iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary] cbArg->nMediaPlaylistDeletePersistentItdentifierCount : %u ", cbArg->nMediaPlaylistDeletePersistentItdentifierCount);
    if(0 < cbArg->nMediaPlaylistDeletePersistentItdentifierCount)
    {
        cbArg->pMediaPlaylistDeletePersistentItdentifier = (MediaPlaylistDeletePersistentItdentifierParam*)iAp2MarshalGetBytes(&iAp2Marshal, &cbArg->nMediaPlaylistDeletePersistentItdentifierSize);
        MediaPlaylistDeletePersistentItdentifierParam* pMediaPlaylistDeletePersistentItdentifier = NULL;
        pMediaPlaylistDeletePersistentItdentifier = cbArg->pMediaPlaylistDeletePersistentItdentifier;
         for(currentIndex = 0 ; currentIndex < cbArg->nMediaPlaylistDeletePersistentItdentifierCount; currentIndex++) {
            //LOGD("[iAP2AppFramework Medialibrary] pMediaPlaylistDeletePersistentItdentifier[%u] value : %llu", currentIndex, pMediaPlaylistDeletePersistentItdentifier->value);
            pMediaPlaylistDeletePersistentItdentifier = pMediaPlaylistDeletePersistentItdentifier  + 1;    
        }
    }

    cbArg->MediaLibraryReset = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary] cbArg->MediaLibraryReset : %u", cbArg->MediaLibraryReset);
    
    cbArg->MediaLibraryUpdateProgress.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);    
    if(cbArg->MediaLibraryUpdateProgress.bPresent == TRUE){        
        cbArg->MediaLibraryUpdateProgress.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary] cbArg->MediaLibraryUpdateProgress.value : %u", cbArg->MediaLibraryUpdateProgress.value);
    }

    cbArg->MediaLibraryIsHidingRemoteItems = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary] cbArg->cbArg->MediaLibraryIsHidingRemoteItems : %u", cbArg->MediaLibraryIsHidingRemoteItems);


    if ((handle->delegate != NULL) && (handle->delegate->mediaLibraryUpdates_f != NULL)) {
        result = handle->delegate->mediaLibraryUpdates_f(handle->delegate->context, cbArg);
    }
    else {
        LOGW("    WARING : delegate.nowPlayingUpdate_f is NULL");
    }        

    LOGD("### cbArg->pMediaItemGroup : %p ###", cbArg->pMediaItemGroup);
    LOGD("### cbArg->pMeidaPlaylistGroup : %p ###", cbArg->pMediaPlaylistGroup);
    LOGD("### cbArg : %p ###", cbArg);
#if !defined(GEELY) && !defined(MEEGO) //TODO : yongil will delete defined(MEEMGO) when yongil can test Geely platform.
    if ( cbArg->pMediaItemGroup != NULL ) {
        free(cbArg->pMediaItemGroup);
    }

    if ( cbArg->pMediaPlaylistGroup != NULL ) {
        free(cbArg->pMediaPlaylistGroup);
    }

    if ( cbArg != NULL ) {
        free(cbArg);
    }
#endif
    
    return result;
}


void ParsingMediaLibraryUpdatesEventMediaItemGroup(MarshalIo* iAp2Marshal, MediaItemGroup* pMediaItemGroup)
{

    //LOGD("ParsingMediaLibraryUpdatesEventMediaItemGroup");
    pMediaItemGroup->MediaItemPersistentIdentifier.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemPersistentIdentifier.bPresent == TRUE){
        pMediaItemGroup->MediaItemPersistentIdentifier.value = iAp2MarshalGetUint64(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemPersistentIdentifier.value : %llu ",  pMediaItemGroup->MediaItemPersistentIdentifier.value);
    }

    pMediaItemGroup->MediaItemTitle.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemTitle.bPresent == TRUE){
        pMediaItemGroup->MediaItemTitle.value = iAp2MarshalGetBytes(iAp2Marshal, &(pMediaItemGroup->MediaItemTitle.size));
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemTitle.value : %s ",  pMediaItemGroup->MediaItemTitle.value);
    }

    pMediaItemGroup->nMediaItemMediaTypeCount = iAp2MarshalGetUint32(iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->nMediaItemMediaTypeCount : %u",  pMediaItemGroup->nMediaItemMediaTypeCount);
    if(0 < pMediaItemGroup->nMediaItemMediaTypeCount) {        
        pMediaItemGroup->pMediaItemMediaType = (MediaItemMediaTypeParam*)iAp2MarshalGetBytes(iAp2Marshal, &(pMediaItemGroup->nMediaItemMediaTypeSize));
        MediaItemMediaTypeParam* pMediaItemMediaType = (MediaItemMediaTypeParam*)pMediaItemGroup->pMediaItemMediaType;
        uint32_t currentIndex = 0;
        for(currentIndex = 0; currentIndex < pMediaItemGroup->nMediaItemMediaTypeCount ; currentIndex++)
        {
            //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemMediaType[%u] : %u",currentIndex,  pMediaItemMediaType->value);
            pMediaItemMediaType = pMediaItemMediaType + 1;
        }
        //pMediaItemMediaType = NULL;
            
    }
    pMediaItemGroup->MediaItemRating.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemRating.bPresent == TRUE){
        pMediaItemGroup->MediaItemRating.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemRating.value : %u",  pMediaItemGroup->MediaItemRating.value); 
    }

    pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.bPresent == TRUE){
        pMediaItemGroup->MediaItemPlaybackDurationInMilliseconds.value = iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemRating.value : %u",  pMediaItemGroup->MediaItemRating.value); 
    }

    pMediaItemGroup->MediaItemAlbumPersistentIdentifier.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemAlbumPersistentIdentifier.bPresent == TRUE){
        pMediaItemGroup->MediaItemAlbumPersistentIdentifier.value = iAp2MarshalGetUint64(iAp2Marshal);
        LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemAlbumPersistentIdentifier.value : %llu",  pMediaItemGroup->MediaItemAlbumPersistentIdentifier.value); 
    }

    pMediaItemGroup->MediaItemAlbumTitle.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemAlbumTitle.bPresent == TRUE){
        pMediaItemGroup->MediaItemAlbumTitle.value = iAp2MarshalGetBytes(iAp2Marshal, &(pMediaItemGroup->MediaItemAlbumTitle.size));
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemAlbumTitle.value : %s",  pMediaItemGroup->MediaItemAlbumTitle.value); 
    }

    pMediaItemGroup->MediaItemAlbumTrackNumber.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemAlbumTrackNumber.bPresent == TRUE){
        pMediaItemGroup->MediaItemAlbumTrackNumber.value = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemAlbumTrackNumber.value : %u",  pMediaItemGroup->MediaItemAlbumTrackNumber.value); 
    }

    pMediaItemGroup->MediaItemAlbumTrackCount.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemAlbumTrackCount.bPresent == TRUE){
        pMediaItemGroup->MediaItemAlbumTrackCount.value = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemAlbumTrackCount.value : %u",  pMediaItemGroup->MediaItemAlbumTrackCount.value); 
    }

    pMediaItemGroup->MediaItemAlbumDiscNumber.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemAlbumDiscNumber.bPresent == TRUE){
        pMediaItemGroup->MediaItemAlbumDiscNumber.value = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemAlbumDiscNumber.value : %u",  pMediaItemGroup->MediaItemAlbumDiscNumber.value);
    }

    pMediaItemGroup->MediaItemAlbumDiscCount.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemAlbumDiscCount.bPresent == TRUE){
        pMediaItemGroup->MediaItemAlbumDiscCount.value = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemAlbumDiscCount.value : %u",  pMediaItemGroup->MediaItemAlbumDiscCount.value);
    }

    pMediaItemGroup->MediaItemArtistPersistentIdentifier.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemArtistPersistentIdentifier.bPresent == TRUE){
        pMediaItemGroup->MediaItemArtistPersistentIdentifier.value = iAp2MarshalGetUint64(iAp2Marshal);
        LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemArtistPersistentIdentifier.value : %llu",  pMediaItemGroup->MediaItemArtistPersistentIdentifier.value);
    }

    pMediaItemGroup->MediaItemArtist.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemArtist.bPresent == TRUE){
        pMediaItemGroup->MediaItemArtist.value = iAp2MarshalGetBytes(iAp2Marshal, &(pMediaItemGroup->MediaItemArtist.size));
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemArtist.value : %s",  pMediaItemGroup->MediaItemArtist.value);
    }

    pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.bPresent == TRUE){
        pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.value = iAp2MarshalGetUint64(iAp2Marshal);
        LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.value : %llu",  pMediaItemGroup->MediaItemAlbumAritistPersistentIdentifier.value);
    }

    pMediaItemGroup->MediaItemAlbumAritist.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemAlbumAritist.bPresent == TRUE){
        pMediaItemGroup->MediaItemAlbumAritist.value = iAp2MarshalGetBytes(iAp2Marshal, &pMediaItemGroup->MediaItemAlbumAritist.size);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemAlbumAritist.value : %s",  pMediaItemGroup->MediaItemAlbumAritist.value);
    }

    pMediaItemGroup->MediaItemGenrePersistentIdentifier.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemGenrePersistentIdentifier.bPresent == TRUE){
        pMediaItemGroup->MediaItemGenrePersistentIdentifier.value = iAp2MarshalGetUint64(iAp2Marshal);
        LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemGenrePersistentIdentifier.value : %llu",  pMediaItemGroup->MediaItemGenrePersistentIdentifier.value);
    }

    pMediaItemGroup->MediaItemGenre.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemGenre.bPresent == TRUE){
        pMediaItemGroup->MediaItemGenre.value = iAp2MarshalGetBytes(iAp2Marshal, &(pMediaItemGroup->MediaItemGenre.size));
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemGenre.value : %s",  pMediaItemGroup->MediaItemGenre.value);
    }

    pMediaItemGroup->MediaItemComposerPersistentIdentifier.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemComposerPersistentIdentifier.bPresent == TRUE){
        pMediaItemGroup->MediaItemComposerPersistentIdentifier.value = iAp2MarshalGetUint64(iAp2Marshal);
        LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemComposerPersistentIdentifier.value : %llu",  pMediaItemGroup->MediaItemComposerPersistentIdentifier.value);
    }

    pMediaItemGroup->MediaItemComposer.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemComposer.bPresent == TRUE){
        pMediaItemGroup->MediaItemComposer.value = iAp2MarshalGetBytes(iAp2Marshal, &(pMediaItemGroup->MediaItemGenre.size));
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemComposer.value : %s",  pMediaItemGroup->MediaItemComposer.value);
    }

    pMediaItemGroup->MediaItemIsPartOfCompilation = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemIsPartOfCompilation : %u",  pMediaItemGroup->MediaItemIsPartOfCompilation);
    
    pMediaItemGroup->MediaItemIsLikeSupported = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemIsLikeSupported : %u",  pMediaItemGroup->MediaItemIsLikeSupported);

    pMediaItemGroup->MediaItemIsBanSupported = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemIsBanSupported : %u",  pMediaItemGroup->MediaItemIsBanSupported);

    pMediaItemGroup->MediaItemIsLiked = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemIsLiked : %u",  pMediaItemGroup->MediaItemIsLiked);

    pMediaItemGroup->MediaItemIsBaned = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemIsBaned : %u",  pMediaItemGroup->MediaItemIsBaned);

    pMediaItemGroup->MediaItemIsResidentOnDevice = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemIsResidentOnDevice : %u",  pMediaItemGroup->MediaItemIsResidentOnDevice);

    pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.bPresent == TRUE){
        pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value : %u",  pMediaItemGroup->MediaItemArtworkFileTransferIdentifier.value);
    }

    pMediaItemGroup->MediaItemChapterCount.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaItemGroup->MediaItemChapterCount.bPresent == TRUE){
        pMediaItemGroup->MediaItemChapterCount.value = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaItemChapterCount.value : %u",  pMediaItemGroup->MediaItemChapterCount.value);
    }
}


void ParsingMediaLibraryUpdatesEventMediaPlaylistGroup(MarshalIo* iAp2Marshal, MediaPlaylistGroup* pMediaPlaylistGroup)
{
    pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.bPresent =  (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if( pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.bPresent == TRUE) {
        pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.value = iAp2MarshalGetUint64(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaPlaylistPersistentIdentifier.value : %llu",  pMediaPlaylistGroup->MediaPlaylistPersistentIdentifier.value);
    }

    pMediaPlaylistGroup->MediaPlaylistName.bPresent =  (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if( pMediaPlaylistGroup->MediaPlaylistName.bPresent == TRUE) {
        pMediaPlaylistGroup->MediaPlaylistName.value = iAp2MarshalGetBytes(iAp2Marshal, &(pMediaPlaylistGroup->MediaPlaylistName.size));
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaPlaylistName.value : %s",  pMediaPlaylistGroup->MediaPlaylistName.value);
    }

    pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.bPresent =  (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if( pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.bPresent == TRUE) {
        pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.value = iAp2MarshalGetUint64(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaPlaylistParentPersistentIdentifier.value : %llu",  pMediaPlaylistGroup->MediaPlaylistParentPersistentIdentifier.value);
    }

    pMediaPlaylistGroup->MediaPlaylistIsGeniusMix = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaPlaylistIsGeniusMix : %u",  pMediaPlaylistGroup->MediaPlaylistIsGeniusMix);

    pMediaPlaylistGroup->MediaPlaylistFolder = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
     //LOGD("[iAP2AppFramework Medialibrary]  pMediaItemGroup->MediaPlaylistFolder : %u",  pMediaPlaylistGroup->MediaPlaylistFolder);
    
    pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.bPresent == TRUE){
        pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
        //LOGD("[iAP2AppFramework Medialibrary]   pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value: %u",   pMediaPlaylistGroup->MediaPlaylistContainedMediaItemsFileTransferIdentifier.value);
   }

    pMediaPlaylistGroup->MediaPlaylistiTunesRadioStation = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    //LOGD("[iAP2AppFramework Medialibrary]   pMediaPlaylistGroup->MediaPlaylistiTunesRadioStation : %u",   pMediaPlaylistGroup->MediaPlaylistiTunesRadioStation);
}



//===========================================================================================================================
//    iAp2AppMediaLibraryUpdatesFree
//===========================================================================================================================
int iAp2AppMediaLibraryUpdatesFree(MediaLibraryUpdatesCbArg* inArg)
{
       int retValue = kIap2NoErr;
    LOGD("iAp2AppMediaLibraryUpdatesFree()");

    if (inArg == NULL) {
        LOGE("ERROR : inArg is NULL");
        return kIap2ParamErr;
    }
    
#if defined(GEELY) || defined(MEEGO) || defined(GEELY_KC1B) //TODO : yongil will delete defined(MEEMGO) when yongil can test Geely platform.
    LOGD("#####  inArg->pMediaItemGroup : %p #####", inArg->pMediaItemGroup);
    LOGD("#####  inArg->pMediaPlaylistGroup : %p #####", inArg->pMediaPlaylistGroup);
    LOGD("#####  inArg : %p #####",inArg); 
    LOGD("#####  Socket Memory address : %p #####",inArg->MediaLibraryUniqueIdentifier_update.value - (sizeof(EventHeader) + sizeof(uint32_t))); 
    
    if(inArg->pMediaItemGroup != NULL)
        free(inArg->pMediaItemGroup);    

    if(inArg->pMediaPlaylistGroup != NULL)
        free(inArg->pMediaPlaylistGroup);    
    
    if(inArg->MediaLibraryUniqueIdentifier_update.value - (sizeof(EventHeader) + sizeof(uint32_t)) != NULL)
        free(inArg->MediaLibraryUniqueIdentifier_update.value - (sizeof(EventHeader) + sizeof(uint32_t)));

    if(inArg != NULL)
        free(inArg);
#endif    
    return retValue;

}

