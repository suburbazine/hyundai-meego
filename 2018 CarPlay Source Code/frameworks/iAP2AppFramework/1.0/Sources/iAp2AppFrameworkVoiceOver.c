/*
    File:       iAp2AppFrameworkVoiceOver.h
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
#include "iAp2AppFrameworkVoiceOver.h"
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
//    iAP2AppStartVoiceOver
//===========================================================================================================================
int iAP2AppStartVoiceOver(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppStartVoiceOver() inRef[%p]", inRef);

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

    pCommandHeader->cmd = kCmdStartVoiceOver;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAP2AppStopVoiceOver
//===========================================================================================================================
int iAP2AppStopVoiceOver(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppStopVoiceOver() inRef[%p]", inRef);

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

    pCommandHeader->cmd = kCmdStopVoiceOver;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;    
}

//===========================================================================================================================
//    iAP2AppRequestVoiceOverMoveCursor
//===========================================================================================================================
int iAP2AppRequestVoiceOverMoveCursor(iAp2AppRef inRef, RequestVoiceOverMoveCursorArg* inArg)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppRequestVoiceOverMoveCursor() inRef[%p]", inRef);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*1;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->CursorDirection);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    pCommandHeader->cmd = kCmdRequestVoiceOverMoveCursor;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAP2AppRequestVoicOverActivateCursor
//===========================================================================================================================
int iAP2AppRequestVoicOverActivateCursor(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppRequestVoicOverActivateCursor() inRef[%p]", inRef);

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

    pCommandHeader->cmd = kCmdRequestVoiceOverActivateCursor;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);    
    return retValue;
}

//===========================================================================================================================
//    iAP2AppRequestVoiceOverScrollPage
//===========================================================================================================================
int iAP2AppRequestVoiceOverScrollPage(iAp2AppRef inRef, RequestVoiceOverScrollPageArg* inArg)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppRequestVoiceOverScrollPage() inRef[%p]", inRef);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*1;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->ScrollDirection);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdRequestVoiceOverScrollPage;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}



//===========================================================================================================================
//    iAP2AppRequestVoiceOverSpeakText
//===========================================================================================================================
int iAP2AppRequestVoiceOverSpeakText(iAp2AppRef inRef, RequestVoiceOverSpeakTextArg* inArg)
{
    #define MARSHALING_BUFFER (10000)

    int retValue = kIap2NoErr;

    LOGD("iAP2AppRequestVoiceOverSpeakText() inRef[%p]", inRef);

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
    
    uint32_t* pIpcData = (uint32_t*) malloc(MARSHALING_BUFFER);

    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;
    iAp2MarshalInit(&iAp2Marshal, pIpcData, MARSHALING_BUFFER, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->TextToSpeak.bPresent);
    if( inArg->TextToSpeak.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->TextToSpeak.value, inArg->TextToSpeak.size);
    }
        
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdRequestVoiceOverSpeakText;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAP2AppRequestVoicOverPauseText
//===========================================================================================================================
int iAP2AppRequestVoicOverPauseText(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppRequestVoicOverPauseText() inRef[%p]", inRef);

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

    pCommandHeader->cmd = kCmdRequestVoiceOverPauseText;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);    
    return retValue;
}

//===========================================================================================================================
//    iAP2AppRequestVoicOverResumeText
//===========================================================================================================================
int iAP2AppRequestVoicOverResumeText(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppRequestVoicOverResumeText() inRef[%p]", inRef);

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

    pCommandHeader->cmd = kCmdRequestVoiceOverResumeText;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAP2AppStartVoiceOverUpdates
//===========================================================================================================================
int iAP2AppStartVoiceOverUpdates(iAp2AppRef inRef, StartVoiceOverUpdatesArg* inArg)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppStartVoiceOverUpdates() inRef[%p]", inRef);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*3;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->SpeakingVolume);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->SpeakingRate);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Enabled);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdStartVoiceOverUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAP2AppStopVoiceOverUpdates
//===========================================================================================================================
int iAP2AppStopVoiceOverUpdates(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppStopVoiceOverUpdates() inRef[%p]", inRef);

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

    pCommandHeader->cmd = kCmdStopVoiceOverUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);    
    return retValue;
}


//===========================================================================================================================
//    iAP2AppRequestVoiceOverConfiguration
//===========================================================================================================================
int iAP2AppRequestVoiceOverConfiguration(iAp2AppRef inRef, RequestVoiceOverConfigurationArg* inArg)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppRequestVoiceOverConfiguration() inRef[%p]", inRef);

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
    

    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->SpeakingVolume.bPresent);
    if(inArg->SpeakingVolume.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->SpeakingVolume.value);
    }
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->SpeakingRate.bPresent);
    if(inArg->SpeakingRate.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->SpeakingRate.value);
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    pCommandHeader->cmd = kCmdRequestVoiceOverConfiguration;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAP2AppStartVoiceOverCursorUpdates
//===========================================================================================================================
int iAP2AppStartVoiceOverCursorUpdates(iAp2AppRef inRef, StartVoiceOverCursorUpdatesArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAP2AppStartVoiceOverCursorUpdates() inRef[%p]", inRef);

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
    
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*6;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Label);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Value);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Hint);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->Traits); 
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pCommandHeader->cmd = kCmdStartVoiceOverCursorUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAP2AppStopVoiceOverCursorUpdates
//===========================================================================================================================
int iAP2AppStopVoiceOverCursorUpdates(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppStopVoiceOverUpdates() inRef[%p]", inRef);

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

    pCommandHeader->cmd = kCmdStopVoiceOverCursorUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventVoiceOverUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventVoiceOverUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
       int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventVoiceOverUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->VoiceOverUpdates_f == NULL) {
        LOGW("    WARING : delegate->USBDeviceMdoeAudioInformation_f is NULL");
        return kIap2NoErr;
    }

    VoiceOverUpdatesCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    memset(&cbArg, 0x00, sizeof(cbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.SpeakingVolume.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if( cbArg.SpeakingVolume.bPresent == TRUE) {
        cbArg.SpeakingVolume.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal); 
    }
    
    cbArg.SpeakingRate.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if( cbArg.SpeakingRate.bPresent == TRUE) {
        cbArg.SpeakingRate.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal); 
    }    

    cbArg.Enabled = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    result = handle->delegate->VoiceOverUpdates_f (handle->delegate->context, &cbArg);
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventVoiceOverCursorUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventVoiceOverCursorUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
       int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventVoiceOverCursorUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->VoiceOverCursorUpdates_f == NULL) {
        LOGW("    WARING : delegate->VoiceOverCursorUpdates_f is NULL");
        return kIap2NoErr;
    }

    VoiceOverCursorUpdatesCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    memset(&cbArg, 0x00, sizeof(cbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.Label.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.Label.bPresent : %d", cbArg.Label.bPresent);
    if(cbArg.Label.bPresent == TRUE) {
        cbArg.Label.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.Label.size);
        LOGD("cbArg.Label.value : %s", cbArg.Label.value);
    }

    cbArg.Value.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.Value.bPresent : %d", cbArg.Value.bPresent);
    if(cbArg.Value.bPresent == TRUE) {
        cbArg.Value.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.Value.size);
        LOGD("cbArg.Value.value : %s", cbArg.Value.value);
    }

    cbArg.Hint.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.Hint.bPresent : %d", cbArg.Hint.bPresent);
    if(cbArg.Hint.bPresent == TRUE) {
        cbArg.Hint.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.Hint.size);
        LOGD("cbArg.Hint.value : %s", cbArg.Hint.value);
    }

    cbArg.Traits.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.Traits.bPresent : %d", cbArg.Traits.bPresent);
    if(cbArg.Traits.bPresent == TRUE) {

        cbArg.Traits.value = (uint16_t*)iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.Traits.size);
        uint16_t* value;
        uint8_t* temp;
        uint8_t fistData;
        value = cbArg.Traits.value;
        
        int i, size = cbArg.Traits.size/2;
        LOGD("size : %u", size);
        for(i = 0; i < size; i++) {
            
            temp = (uint8_t*)value;
            fistData = *temp;
            *temp = *(temp + 1);
            *(temp + 1) = fistData;
            LOGD("cbArg.Traits.value : %x", *value);
            LOGD("cbArg.Traits.value : %u", *value);
            value += 1;
        }
        
    }

    result = handle->delegate->VoiceOverCursorUpdates_f (handle->delegate->context, &cbArg);
    return result;
}



