/*
    File:       iAp2AppFrameworkPlayMediaLibrary.c
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
#include "iAp2AppFrameworkPlayMediaLibrary.h"
#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkPrivate.h"
#include "iAp2Connection.h"
#include "iAp2Marshal.h"
#include "iAp2Log.h"

//===========================================================================================================================
//    Definitions
//===========================================================================================================================


//===========================================================================================================================
//    Declarations
//===========================================================================================================================


//===========================================================================================================================
//    iAP2AppPlayLibraryCurrentSelection
//===========================================================================================================================
int iAp2AppPlayMediaLibraryCurrentSelection(iAp2AppRef inRef, PlayMediaLibraryCurrentSelectionArg* inArg)
{
    int retValue = kIap2NoErr;
    LOGD("iAP2AppPlayLibraryCurrentSelection() inRef[%p]", inRef);

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
    
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*50;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;
    
    if (pIpcData == NULL) {
    LOGE("    ERROR : malloc error. pIpcData is NULL");
    return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutBytes(&iAp2Marshal, inArg->MediaLibraryUniqueIdentifier_CurrentSelection.value, inArg->MediaLibraryUniqueIdentifier_CurrentSelection.size);

    LOGD("inArg->MediaLibraryUniqueIdentifier_CurrentSelection.value : %s", inArg->MediaLibraryUniqueIdentifier_CurrentSelection.value);
    LOGD("inArg->MediaLibraryUniqueIdentifier_CurrentSelection.size : %d", inArg->MediaLibraryUniqueIdentifier_CurrentSelection.size);

    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

       pCommandHeader->cmd = kCmdPlayMediaLibraryCurrentSelection;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAP2AppPlayLibraryItems
//===========================================================================================================================
int iAp2AppPlayMediaLibraryItems(iAp2AppRef inRef, PlayMediaLibraryItemsArg* inArg)
{
    int retValue = kIap2NoErr;
    LOGD("iAP2AppPlayLibraryItems() inRef[%p]", inRef);

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
    
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*100 +  inArg->ItemsPersistentIdentifier.size + inArg->MediaLibraryUniqueIdentifier_Item.size;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;
    
    if (pIpcData == NULL) {
    LOGE("    ERROR : malloc error. pIpcData is NULL");
    return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutBytes(&iAp2Marshal, inArg->ItemsPersistentIdentifier.value, inArg->ItemsPersistentIdentifier.size);
    iAp2MarshalPutUint32(&iAp2Marshal,inArg->ItemStartingIndex.bPresent);
    if(inArg->ItemStartingIndex.bPresent == TRUE){
        iAp2MarshalPutUint32(&iAp2Marshal,inArg->ItemStartingIndex.value);
    }
    iAp2MarshalPutBytes(&iAp2Marshal, inArg->MediaLibraryUniqueIdentifier_Item.value, inArg->MediaLibraryUniqueIdentifier_Item.size);
    
    LOGD("inArg->ItemsPersistentIdentifier.size : %u", inArg->ItemsPersistentIdentifier.size);
    LOGD("inArg->ItemStartingIndex.bPresent : %u", inArg->ItemStartingIndex.bPresent);
    if(inArg->ItemStartingIndex.bPresent == TRUE){
        LOGD("inArg->ItemStartingIndex.value : %d", inArg->ItemStartingIndex.value);
    }
    LOGD("inArg->MediaLibraryUniqueIdentifier_Item.value : %s", inArg->MediaLibraryUniqueIdentifier_Item.value);
    LOGD("inArg->MediaLibraryUniqueIdentifier_Item.size : %u", inArg->MediaLibraryUniqueIdentifier_Item.size);

    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdPlayMediaLibraryItems;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAP2AppPlayLibraryCollection
//===========================================================================================================================
int iAp2AppPlayMediaLibraryCollection(iAp2AppRef inRef, PlayMediaLibraryCollectionArg* inArg)
{
    int retValue = kIap2NoErr;
    LOGD("iAP2AppPlayLibraryCollection() inRef[%p]", inRef);

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

    LOGD("inArg->CollectionPersistentIdentifier : %llu", inArg->CollectionPersistentIdentifier);
    LOGD("inArg->inArg->CollectionType : %u", inArg->CollectionType);
    iAp2MarshalPutUint64(&iAp2Marshal, inArg->CollectionPersistentIdentifier);
    iAp2MarshalPutUint32(&iAp2Marshal,inArg->CollectionType);

    iAp2MarshalPutUint32(&iAp2Marshal,inArg->CollectionStartingIndex.bPresent);
    if(inArg->CollectionStartingIndex.bPresent == TRUE){
        iAp2MarshalPutUint32(&iAp2Marshal,inArg->CollectionStartingIndex.value);
    }

    LOGD("inArg->MediaLibraryUniqueIdentifier_Collection.value : %s", inArg->MediaLibraryUniqueIdentifier_Collection.value);
    iAp2MarshalPutBytes(&iAp2Marshal, inArg->MediaLibraryUniqueIdentifier_Collection.value, inArg->MediaLibraryUniqueIdentifier_Collection.size);


    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdPlayMediaLibraryCollection;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAP2AppPlayLibrarySpecial
//===========================================================================================================================
int iAP2AppPlayMediaLibrarySpecial(iAp2AppRef inRef, PlayMediaLibrarySpecialArg* inArg)
{
    int retValue = kIap2NoErr;
    LOGD("iAP2AppPlayLibrarySpecial() inRef[%p]", inRef);

    if (inArg == NULL) {
    LOGE("ERROR : inArg is NULL");
    return kIap2ParamErr;
    }

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
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

    LOGD("inArg->MediaLibraryUniqueIdentifier_Special.value : %s", inArg->MediaLibraryUniqueIdentifier_Special.value);
    iAp2MarshalPutBytes(&iAp2Marshal, inArg->MediaLibraryUniqueIdentifier_Special.value, inArg->MediaLibraryUniqueIdentifier_Special.size);

    LOGD("inArg->inArg->AllSongs : %u", inArg->AllSongs);    
    iAp2MarshalPutUint32(&iAp2Marshal,inArg->AllSongs);

    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdPlayMediaLibrarySpecial;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

