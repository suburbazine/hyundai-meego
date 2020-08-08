/*
    File:       iAp2AppFrameworkMeidaLibraryInformation.c
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
#include "iAp2AppFrameworkMediaLibraryInformation.h"
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
//    iAp2AppStartMediaLibraryInformation
//===========================================================================================================================
int iAp2AppStartMediaLibraryInformation(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStartMediaLibraryInformation() inRef[%p]", inRef);

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
    
    pCommandHeader->cmd = kCmdStartMediaLibraryInformaion;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;

}

//===========================================================================================================================
//    iAp2AppStopMediaLibraryInformation
//===========================================================================================================================
int iAp2AppStopMediaLibraryInformation(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStopMediaLibraryInformation() ");

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

    pCommandHeader->cmd = kCmdStopMediaLibraryInformaion;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;

}

//===========================================================================================================================
//    iAp2ConectionHandleEventMeidaLibraryInformation
//===========================================================================================================================
int iAp2ConectionHandleEventMeidaLibraryInformation(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventMeidaLibraryInformation() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->mediaLibraryInformation_f == NULL) {
        LOGW("    WARING : delegate->mediaLibraryInformation_f is NULL");
        return kIap2NoErr;
    }

    MeidaLibraryInformatonCbArg cbArg;
    uint32_t nCountGroup = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    memset(&cbArg, 0x00, sizeof(MeidaLibraryInformatonCbArg));
    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    // MeidaLibraryInformationGroup
    cbArg.nCountGroupParam = (uint32_t)iAp2MarshalGetUint32(&iAp2Marshal);
    nCountGroup = cbArg.nCountGroupParam;
    //LOGD(" cbArg.nCountGroupParam : %d",  cbArg.nCountGroupParam);
    cbArg.pMediaLibraryInformationGr = malloc(sizeof(MediaLibraryInformationGroup) * nCountGroup);
    if( cbArg.pMediaLibraryInformationGr != NULL) {
        memset(cbArg.pMediaLibraryInformationGr, 0x00, sizeof(MediaLibraryInformationGroup) * nCountGroup);
        MediaLibraryInformationGroup* pMediaLibraryInformation = (MediaLibraryInformationGroup*) cbArg.pMediaLibraryInformationGr;
    
        while(nCountGroup)
        {            
            pMediaLibraryInformation->mMediaLibraryName.value = iAp2MarshalGetBytes(&iAp2Marshal, &pMediaLibraryInformation->mMediaLibraryName.size);
            pMediaLibraryInformation->mMediaLibraryUniqueIdentifier.value = iAp2MarshalGetBytes(&iAp2Marshal, &pMediaLibraryInformation->mMediaLibraryUniqueIdentifier.size);
            pMediaLibraryInformation->mMediaLibraryType.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
            //LOGD(" pMeidaLibraryInformationGroup->mMediaLibraryName.value : %s",  pMeidaLibraryInformation->mMediaLibraryName.value);    
            //LOGD(" pMeidaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.value : %s",  pMeidaLibraryInformation->mMediaLibraryUniqueIdentifier.value);
            //LOGD(" pMeidaLibraryInformationGroup->mMediaLibraryType.value : %u",  pMeidaLibraryInformation->mMediaLibraryType.value);    
            //LOGD(" pMeidaLibraryInformationGroup->mMediaLibraryName.size : %u",  pMeidaLibraryInformation->mMediaLibraryName.size);    
            //LOGD(" pMeidaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.size : %u",  pMeidaLibraryInformation->mMediaLibraryUniqueIdentifier.size);

            pMediaLibraryInformation = pMediaLibraryInformation + 1;
            nCountGroup--;
        }
    }
    else {
        LOGE("malloc Failed!!   pMediaLibraryInformationGr is NULL");
    }
     
    if ((handle->delegate != NULL) && (handle->delegate->mediaLibraryInformation_f != NULL)) {
        result = handle->delegate->mediaLibraryInformation_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.mediaLibraryInformation_f is NULL");
    }

    //LOGD("### free cbArg.pMediaLibraryInformationGr : %p### ", cbArg.pMediaLibraryInformationGr);
    if( cbArg.pMediaLibraryInformationGr != NULL) {
        free(cbArg.pMediaLibraryInformationGr);
    }
    return result;
}

