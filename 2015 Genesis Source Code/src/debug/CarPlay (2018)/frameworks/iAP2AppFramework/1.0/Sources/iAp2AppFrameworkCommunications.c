/*
    File:       iAp2AppFrameworkCommunications.c
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
#include "iAp2AppFrameworkCommunications.h"
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
//    iAp2AppStartCallStateUpdates
//===========================================================================================================================
int iAp2AppStartCallStateUpdates(iAp2AppRef inRef, StartCallStateUpdatesArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStartCallStateUpdates() inRef[%p]", inRef);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(StartCallStateUpdatesArg);
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bAddressBookID);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bLabel);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bDisconnectReason);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    LOGD("inArg->bAddressBookID : %u", inArg->bAddressBookID);
    LOGD("inArg->bLabel : %u", inArg->bLabel);
    LOGD("inArg->bDisconnectReason : %u", inArg->bDisconnectReason);
    
    pCommandHeader->cmd = kCmdStartCallStateUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}



//===========================================================================================================================
//    iAp2AppStopCallStateUpdates
//===========================================================================================================================
int iAp2AppStopCallStateUpdates(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStopCallStateUpdates() inRef[%p]", inRef);

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
    
    pCommandHeader->cmd = kCmdStopCallStateUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}




//===========================================================================================================================
//    iAp2AppStartCommunicationsUpdate
//===========================================================================================================================
int iAp2AppStartCommunicationsUpdate(iAp2AppRef inRef, StartCommunicationsUpdatesArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStartCommunicationsUpdate() inRef[%p]", inRef);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(StartCommunicationsUpdatesArg);
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bSignalStrength);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bRegistrationStatus);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bAirplaneModeStatus);    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bCarrierName);    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bCelluarSupported);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bTelephonyEnabled);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bFaceTimeAudioEnabled);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bFaceTimeVideoEnabled);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bMuteStatus);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bCurrentCallCount);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bNewVoicemailCount);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bInitiateCallAvailable);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bEndAndAccptAvailable);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bHoldAndAcceptAvailable);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bSwapAvailable);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bMergeAvailable);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bHoldAvailable);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdStartCommunicationsUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAp2AppStopCommunicationsUpdate
//===========================================================================================================================
int iAp2AppStopCommunicationsUpdate(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStopCommunicationsUpdate() inRef[%p]", inRef);

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
    
    pCommandHeader->cmd = kCmdStopCommunicationsUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppIntialCall
//===========================================================================================================================
int iAp2AppInitiateCall(iAp2AppRef inRef, InitiateCallArg* inArg)
{
    int retValue = kIap2NoErr;

	LOGD("iAp2AppIntialCall() inRef[%p]", inRef);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(InitiateCallArg) + inArg->mDestinationID.size + inArg->mAddressBookID.size ;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->eType);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mDestinationID.bPresent);
    if (inArg->mDestinationID.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->mDestinationID.value, inArg->mDestinationID.size);    
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mService.bPresent);
    if (inArg->mService.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mService.value);
    }
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mAddressBookID.bPresent);
    if (inArg->mAddressBookID.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->mAddressBookID.value, inArg->mAddressBookID.size);    
    }

    
    //LOGD("inArg->mDestinationID.value : %s", inArg->mDestinationID.value);
    //LOGD("inArg->mDestinationID.size : %u", inArg->mDestinationID.size);
    //LOGD("inArg->mAddressBookID.value : %s", inArg->mAddressBookID.value);
    //LOGD("inArg->mAddressBookID.size : %u", inArg->mAddressBookID.size);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdInitiateCall;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAp2AppIntialCall
//===========================================================================================================================
int iAp2AppAcceptCall(iAp2AppRef inRef, AcceptCallArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppAcceptCall() inRef[%p]", inRef);

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
    
    int mallocSize = sizeof(CommandHeader) + sizeof(AcceptCallArg) + inArg->mCallUUID.size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->eAcceptAction);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mCallUUID.bPresent);
    if (inArg->mCallUUID.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->mCallUUID.value, inArg->mCallUUID.size);    
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdAcceptCall;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}




//===========================================================================================================================
//    iAp2AppIntialCall
//===========================================================================================================================
int iAp2AppEndCall(iAp2AppRef inRef, EndCallArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppEndCall() inRef[%p]", inRef);

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
    
    int mallocSize = sizeof(CommandHeader) + sizeof(EndCallArg) + inArg->mCallUUID.size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->eEndAction);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mCallUUID.bPresent);
    if (inArg->mCallUUID.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->mCallUUID.value, inArg->mCallUUID.size);    
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdEndCall;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAp2AppSwapCalls
//===========================================================================================================================
int iAp2AppSwapCalls(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppSwapCalls() inRef[%p]", inRef);

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
    
    pCommandHeader->cmd = kCmdSwapCalls;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppMergeCalls
//===========================================================================================================================
int iAp2AppMergeCalls(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppMergeCalls() inRef[%p]", inRef);

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
    
    pCommandHeader->cmd = kCmdMergeCalls;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAp2AppHoldStatusUpdate
//===========================================================================================================================
int iAp2AppHoldStatusUpdate(iAp2AppRef inRef, HoldStatusUpdateArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppHoldStatusUpdate() inRef[%p]", inRef);

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
    
    int mallocSize = sizeof(CommandHeader) + sizeof(HoldStatusUpdateArg) + inArg->mCallUUID.size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bHoldStatus);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mCallUUID.bPresent);
    if (inArg->mCallUUID.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->mCallUUID.value, inArg->mCallUUID.size);    
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdHoldStatusUpdate;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppMuteStatusUpdate
//===========================================================================================================================
int iAp2AppMuteStatusUpdate(iAp2AppRef inRef, MuteStatusUpdateArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppMuteStatusUpdate() inRef[%p]", inRef);

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
    
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*10;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bMuteStatus);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdMuteStatusUpdate;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}
//===========================================================================================================================
//    iAp2AppSendDTMF
//===========================================================================================================================
int iAp2AppSendDTMF(iAp2AppRef inRef, SendDTMFArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppSendDTMF() inRef[%p]", inRef);

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
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->eTone);

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mCallUUID.bPresent);
    if (inArg->mCallUUID.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->mCallUUID.value, inArg->mCallUUID.size);    
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdSendDTMF;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAp2AppStartListUpdate
//===========================================================================================================================
int iAp2AppStartListUpdate(iAp2AppRef inRef, StartListUpdatesArg* inArg)
{
    int retValue = kIap2NoErr;

	LOGD("iAp2AppStartListUpdate() inRef[%p]", inRef);

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
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mRecenttsListPropoerties.bPresent);
    if(inArg->mRecenttsListPropoerties.bPresent == TRUE) 
    {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mRecenttsListPropoerties.bLabel);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mRecenttsListPropoerties.bAdressBookID);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mRecenttsListPropoerties.bUnixTimestamp);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mRecenttsListPropoerties.bDuration);        
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mRecentsListMax.bPresent);
    if(inArg->mRecentsListMax.bPresent == TRUE) 
    {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mRecentsListMax.value);        
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bRecentsListCombine);
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mFavoritesListProerties.bPresent);            
    if(inArg->mFavoritesListProerties.bPresent == TRUE) 
    {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mFavoritesListProerties.bLabel);
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mFavoritesListProerties.bAdressBookID);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mFavoritesListMax.bPresent);
    if(inArg->mFavoritesListMax.bPresent == TRUE) 
    {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mFavoritesListMax.value);        
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdStartListUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAp2AppStopListUpdates
//===========================================================================================================================
int iAp2AppStopListUpdates(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStopListUpdates()");
    
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
    
    pCommandHeader->cmd = kCmdStopListUpdates;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}



//===========================================================================================================================
//    iAp2ConectionHandleEventCallStateUpdates
//===========================================================================================================================
int iAp2ConectionHandleEventCallStateUpdates(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventCallStateUpdates() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    CallStateUpdatesCBArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    memset(&cbArg, 0x00, sizeof(cbArg));
    
    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.mRemoteID.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.mRemoteID.bPresent == TRUE) {
        cbArg.mRemoteID.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.mRemoteID.size);
        LOGD("cbArg.mRemoteID.value %s", cbArg.mRemoteID.value);
    }

    cbArg.mDisplayName.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.mDisplayName.bPresent == TRUE) {
        cbArg.mDisplayName.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.mDisplayName.size);
        LOGD("cbArg.mDisplayName.value %s", cbArg.mDisplayName.value);
    }

    cbArg.bLegacy = (CallStateUpdateStatus)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.bLegacy %u", cbArg.bLegacy);
    
    cbArg.eStatus = (CallStateUpdateStatus)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.eStatus %u", cbArg.eStatus);
    
    cbArg.eDirection = (CallStateUpdateDirection)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.eDirection %u", cbArg.eDirection);
    
    cbArg.mCallUUID.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.mCallUUID.bPresent == TRUE) {
        cbArg.mCallUUID.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.mCallUUID.size);
        LOGD("cbArg.mCallUUID.value %s", cbArg.mCallUUID.value);
    }

    cbArg.mAddressBookID.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.mAddressBookID.bPresent == TRUE) {
        cbArg.mAddressBookID.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.mAddressBookID.size);
        LOGD("cbArg.mAddressBookID.value %s", cbArg.mAddressBookID.value);
    }
    
    cbArg.mLabel.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.mLabel.bPresent == TRUE) {
        cbArg.mLabel.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.mLabel.size);
        LOGD("cbArg.mLabel.value %s", cbArg.mLabel.value);
    }
    
    cbArg.eService = (CallStateUpdateService)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.eService %u", cbArg.eService);
    
    cbArg.bIsConferenced = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.bIsConferenced %u", cbArg.bIsConferenced);
    
    cbArg.nConferenceGroup = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.nConferenceGroup %u", cbArg.nConferenceGroup);
    
    cbArg.mDisconnectReason.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);    
    if (cbArg.mDisconnectReason.bPresent == TRUE) {
        cbArg.mDisconnectReason.value = (CallStateUpdateDisconnectReason)iAp2MarshalGetUint32(&iAp2Marshal);
        LOGD("cbArg.mDisconnectReason.value %u", cbArg.mDisconnectReason.value);
    }
    
     if ((handle->delegate != NULL) && (handle->delegate->CallStateUpdates_f  != NULL)) {
        result = handle->delegate->CallStateUpdates_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.CallStateUpdates_f is NULL");
    }
    
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventCommunicationsUpdates
//===========================================================================================================================
int iAp2ConectionHandleEventCommunicationsUpdates(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventCommunicationsUpdates() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    CommunicationsUpdatesCBArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    memset(&cbArg, 0x00, sizeof(cbArg));
    
    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.mSignalStrength.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.mSignalStrength.bPresent == TRUE)
        cbArg.mSignalStrength.value = (SignalStrengthEnum)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.mRegistrationStatus.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.mRegistrationStatus.bPresent == TRUE)
        cbArg.mRegistrationStatus.value = (RegistrationStatusEnum)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.bAirplaneModeStatus = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.mCarrierName.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.mCarrierName.bPresent == TRUE)
        cbArg.mCarrierName.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.mCarrierName.size);

    cbArg.bCelluarSupported = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.bTelephonyEnabled = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.bFaceTimeAudioEnabled = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.bFaceTimeVideoEnabled = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.bMuteStatus = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.mCurrentCallCount.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.mCurrentCallCount.bPresent == TRUE)
        cbArg.mCurrentCallCount.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.mNewVoicemailCount.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.mNewVoicemailCount.bPresent == TRUE)
        cbArg.mNewVoicemailCount.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.bInitiateCallAvailable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.bEndAndAcceptAvailable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.bHoldAndAccptAvailable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.bSwapAvailable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.bMergeAvailable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.bHoldAvailable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    
     if ((handle->delegate != NULL) && (handle->delegate->CommunicationsUpdates_f  != NULL)) {
        result = handle->delegate->CommunicationsUpdates_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.CommunicationsUpdates_f is NULL");
    }
    
    return result;
}


//===========================================================================================================================
//    iAp2ConectionHandleEventCommunicationsUpdates
//===========================================================================================================================
int iAp2ConectionHandleEventListUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventListUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }
    uint32_t currentIndex = 0;
    ListUpdateCBArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    memset(&cbArg, 0x00, sizeof(cbArg));
    
    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.mRectsListAavailable.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);    
    LOGD("cbArg.mRectsListAavailable.bPresent : %u", cbArg.mRectsListAavailable.bPresent);
    if (cbArg.mRectsListAavailable.bPresent == TRUE) {
        cbArg.mRectsListAavailable.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
        LOGD("cbArg.mRectsListAavailable.value : %u", cbArg.mRectsListAavailable.value);
    }
    
    cbArg.nRecentListSize = (uint32_t)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.nRecentListSize : %u", cbArg.nRecentListSize);

    cbArg.nRecentListGroupCount = (uint32_t)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.nRecentListGroupCount : %u", cbArg.nRecentListGroupCount);
    
    if(0 < cbArg.nRecentListSize)
    {
        cbArg.pRecentListGroup = (RecentListGroup*)malloc(cbArg.nRecentListSize);
        if( cbArg.pRecentListGroup != NULL)
        {
            memset(cbArg.pRecentListGroup, 0x00, cbArg.nRecentListSize);
            RecentListGroup* pRecentListGroup     = (RecentListGroup*) cbArg.pRecentListGroup;
            for(currentIndex = 0 ; currentIndex < cbArg.nRecentListGroupCount; currentIndex++)
            {    
                ParsingListUpdateEventRecentListGroup(&iAp2Marshal, pRecentListGroup);
                pRecentListGroup = pRecentListGroup + 1;    
            }
        }
        else {
            LOGE("malloc failed! pRecentListGroup is NULL");
        }

    }
    
    cbArg.mRecentListCount.bPresent= (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);    
    LOGD("cbArg.mRecentListCount.bPresent : %u", cbArg.mRecentListCount.bPresent);
    if( cbArg.mRecentListCount.bPresent == TRUE){
        cbArg.mRecentListCount.value = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
        LOGD("mRecentListCount.value : %u", cbArg.mRecentListCount.value);
    }
        
    cbArg.mFavoritesListAavailable.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.mFavoritesListAavailable.bPresent : %u", cbArg.mFavoritesListAavailable.bPresent);
    if (cbArg.mFavoritesListAavailable.bPresent == TRUE){
        cbArg.mFavoritesListAavailable.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
        LOGD("cbArg.mFavoritesListAavailable.value : %u", cbArg.mFavoritesListAavailable.value);
    }
    
    cbArg.nFavoritesListSize = (uint32_t)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.nFavoritesListSize : %u", cbArg.nFavoritesListSize);

    cbArg.nFavoritesListGroupCount = (uint32_t)iAp2MarshalGetUint32(&iAp2Marshal);
    LOGD("cbArg.nFavoritesListGroupCount : %u", cbArg.nFavoritesListGroupCount);
        
    if(0 < cbArg.nFavoritesListSize)
    {
        cbArg.pFavoritesListGroup = (FavoritesListGroup*)malloc(cbArg.nFavoritesListSize);
        if( cbArg.pFavoritesListGroup != NULL)
        {
            memset(cbArg.pFavoritesListGroup, 0x00, cbArg.nFavoritesListSize);
            FavoritesListGroup* pFavoritesListGroup     = (FavoritesListGroup*) cbArg.pFavoritesListGroup;
            for(currentIndex = 0 ; currentIndex < cbArg.nFavoritesListGroupCount; currentIndex++)
            {    
                ParsingListUpdateEventFavoritesListGroup(&iAp2Marshal, pFavoritesListGroup);
                pFavoritesListGroup = pFavoritesListGroup + 1;    
            }
        }
        else {
            LOGE("malloc failed! pFavoritesListGroup is NULL");
        }
    }    

    cbArg.mFavoritesListCount.bPresent= (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);    
    LOGD("cbArg.mFavoritesListCount.bPresent : %u", cbArg.mFavoritesListCount.bPresent);
    if(cbArg.mFavoritesListCount.bPresent == TRUE) {
        cbArg.mFavoritesListCount.value = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
        LOGD("cbArg.mFavoritesListCount.value : %u", cbArg.mFavoritesListCount.value);
    }
        
    
    if ((handle->delegate != NULL) && (handle->delegate->ListUpdate_f  != NULL)) {
        result = handle->delegate->ListUpdate_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.iAp2ListUpdate_f is NULL");
    }

    if(cbArg.pRecentListGroup != NULL)
        free(cbArg.pRecentListGroup);

    if(cbArg.pFavoritesListGroup != NULL)
        free(cbArg.pFavoritesListGroup);
    
    return result;
}

void ParsingListUpdateEventRecentListGroup(MarshalIo* iAp2Marshal, RecentListGroup* pRecentListGroup)
{

    pRecentListGroup->bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    //LOGD("pRecentListGroup->bPresent : %u", pRecentListGroup->bPresent);
    

    pRecentListGroup->nIndex = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
    LOGD("pRecentListGroup->nIndex : %u", pRecentListGroup->nIndex);

    pRecentListGroup->mRemoteID.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pRecentListGroup->mRemoteID.bPresent == TRUE){
        pRecentListGroup->mRemoteID.value = iAp2MarshalGetBytes(iAp2Marshal, &(pRecentListGroup->mRemoteID.size));
        LOGD("pRecentListGroup->mRemoteID.value : %s", pRecentListGroup->mRemoteID.value);
    }

    pRecentListGroup->mDisplayName.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
     if(pRecentListGroup->mDisplayName.bPresent == TRUE){
        pRecentListGroup->mDisplayName.value = iAp2MarshalGetBytes(iAp2Marshal, &(pRecentListGroup->mDisplayName.size));
        LOGD("pRecentListGroup->mDisplayName.value : %s", pRecentListGroup->mDisplayName.value);
    }

    pRecentListGroup->mLabel.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
     if(pRecentListGroup->mLabel.bPresent == TRUE){
        pRecentListGroup->mLabel.value = iAp2MarshalGetBytes(iAp2Marshal, &(pRecentListGroup->mLabel.size));
        LOGD("pRecentListGroup->mLabel.value : %s", pRecentListGroup->mLabel.value);
    }

    pRecentListGroup->mAddressBookID.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pRecentListGroup->mAddressBookID.bPresent == TRUE){
        pRecentListGroup->mAddressBookID.value = iAp2MarshalGetBytes(iAp2Marshal, &(pRecentListGroup->mAddressBookID.size));
        LOGD("pRecentListGroup->mAddressBookID.value : %s", pRecentListGroup->mAddressBookID.value);
    }

    pRecentListGroup->eServcie = (ListUpdateServiceEnum)iAp2MarshalGetUint32(iAp2Marshal);

    pRecentListGroup->eType = (ListUpdateTypeEnum)iAp2MarshalGetUint32(iAp2Marshal);

    pRecentListGroup->mUnixTimestamp.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pRecentListGroup->mUnixTimestamp.bPresent == TRUE){
        pRecentListGroup->mUnixTimestamp.value = iAp2MarshalGetUint64(iAp2Marshal);
        LOGD("pRecentListGroup->mUnixTimestamp.value : %llu", pRecentListGroup->mUnixTimestamp.value);
    }

    pRecentListGroup->mDuration.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pRecentListGroup->mDuration.bPresent == TRUE){
        pRecentListGroup->mDuration.value = iAp2MarshalGetUint32(iAp2Marshal);
        LOGD("pRecentListGroup->mDuration.value : %u", pRecentListGroup->mDuration.value);
    }

    pRecentListGroup->nOccurrences = (uint8_t)iAp2MarshalGetUint32(iAp2Marshal);
    LOGD("pRecentListGroup->nOccurrences : %u", pRecentListGroup->nOccurrences);
}


void ParsingListUpdateEventFavoritesListGroup(MarshalIo* iAp2Marshal, FavoritesListGroup* pFavoritesListGroup)
{

    pFavoritesListGroup->bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    LOGD("pFavoritesListGroup->bPresent : %u", pFavoritesListGroup->bPresent);
    
    pFavoritesListGroup->nIndex = (uint16_t)iAp2MarshalGetUint32(iAp2Marshal);
    LOGD("pFavoritesListGroup->nIndex : %u", pFavoritesListGroup->nIndex);

    pFavoritesListGroup->mRemoteID.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);    
    if(pFavoritesListGroup->mRemoteID.bPresent == TRUE){
        pFavoritesListGroup->mRemoteID.value = iAp2MarshalGetBytes(iAp2Marshal, &(pFavoritesListGroup->mRemoteID.size));
        LOGD("pFavoritesListGroup->mRemoteID.value : %s", pFavoritesListGroup->mRemoteID.value);
    }

    pFavoritesListGroup->mDisplayName.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
     if(pFavoritesListGroup->mDisplayName.bPresent == TRUE){
        pFavoritesListGroup->mDisplayName.value = iAp2MarshalGetBytes(iAp2Marshal, &(pFavoritesListGroup->mDisplayName.size));
        LOGD("pRecentListGroup->mDisplayName.value : %s", pFavoritesListGroup->mDisplayName.value);
    }

    pFavoritesListGroup->mLabel.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
     if(pFavoritesListGroup->mLabel.bPresent == TRUE){
        pFavoritesListGroup->mLabel.value = iAp2MarshalGetBytes(iAp2Marshal, &(pFavoritesListGroup->mLabel.size));
        LOGD("pFavoritesListGroup->mLabel.value : %s", pFavoritesListGroup->mLabel.value);
    }

    pFavoritesListGroup->mAddressBookID.bPresent = (BOOL)iAp2MarshalGetUint32(iAp2Marshal);
    if(pFavoritesListGroup->mAddressBookID.bPresent == TRUE){
        pFavoritesListGroup->mAddressBookID.value = iAp2MarshalGetBytes(iAp2Marshal, &(pFavoritesListGroup->mAddressBookID.size));
        LOGD("pFavoritesListGroup->mAddressBookID.value : %s", pFavoritesListGroup->mAddressBookID.value);
    }

    pFavoritesListGroup->eServcie = (ListUpdateServiceEnum)iAp2MarshalGetUint32(iAp2Marshal);
}


