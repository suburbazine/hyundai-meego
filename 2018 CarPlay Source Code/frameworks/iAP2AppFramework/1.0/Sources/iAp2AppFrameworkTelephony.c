/*
    File:       iAp2AppFrameworkTelephony.c
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
#include "iAp2AppFrameworkTelephony.h"
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
//    iAp2AppStartTelephonyCallStateInformation
//===========================================================================================================================
int iAp2AppStartTelephonyCallStateInformation(iAp2AppRef inRef, StartTelephonyCallStateInformationArg* inArg)
{
        int retValue = kIap2NoErr;

    LOGD("iAp2AppStartTelephonyCallStateInformation() inRef[%p]", inRef);

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

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bCallStatevCardFileTransferID);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdStartTelephonyCallStateInformation;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

    LOGD("cmd[%u] appHandle[%p] dataSize[%u]", pIpcData[0], pIpcData[1], pIpcData[2]);
    LOGD("bCallStatevCardFileTransferID [%u]", pIpcData[3]);

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppStopTelephonyCallStateInformation
//===========================================================================================================================
int iAp2AppStopTelephonyCallStateInformation(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStopTelephonyCallStateInformation() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    uint32_t ipcData[16/4];
    uint32_t dummyValue = 0xFFFFFFFF;

    iAp2MarshalInit(&iAp2Marshal, ipcData, 16, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, dummyValue);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    ipcData[0] = kCmdStopTelephonyCallStateInformation;
    ipcData[1] = (uint32_t)inRef;
    ipcData[2] = byteNum;

    LOGD("cmd[%u] appHandle[%p] dataSize[%u] dummyValue[0x%x]", ipcData[0], ipcData[1], ipcData[2], ipcData[3]);

    retValue = iAp2ConnectionSendCmd(inRef, ipcData, byteNum);
    return retValue;

}


//===========================================================================================================================
//    iAp2AppStartTelephonyUpdates
//===========================================================================================================================
int iAp2AppStartTelephonyUpdates(iAp2AppRef inRef, StartTelephonyUpdatesArg* inArg)
{
        int retValue = kIap2NoErr;

    LOGD("iAp2AppStartTelephonyUpdates() inRef[%p]", inRef);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*5;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bTelephonySignalStrength);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bTelephonyRegisStatus);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bTelephonyAirPlaneModeStatus);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bTelephonyTTYStatus);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bTMobileOperator);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdStartTelephonyUpdates;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

    LOGD("cmd[%u] appHandle[%p] dataSize[%u]", pIpcData[0], pIpcData[1], pIpcData[2]);
    LOGD("CallStatevCardFileTransferID bPresent[%u]", pIpcData[3]);

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppStopTelephonyUpdates
//===========================================================================================================================
int iAp2AppStopTelephonyUpdates(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStopTelephonyUpdates() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    uint32_t ipcData[16/4];
    uint32_t dummyValue = 0xFFFFFFFF;

    iAp2MarshalInit(&iAp2Marshal, ipcData, 16, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, dummyValue);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    ipcData[0] = kCmdStopTelephonyUpdates;
    ipcData[1] = (uint32_t)inRef;
    ipcData[2] = byteNum;

    LOGD("cmd[%u] appHandle[%p] dataSize[%u] dummyValue[0x%x]", ipcData[0], ipcData[1], ipcData[2], ipcData[3]);

    retValue = iAp2ConnectionSendCmd(inRef, ipcData, byteNum);
    return retValue;

}


//===========================================================================================================================
//    iAp2ConectionHandleEventTelephonyCallStateInformation
//===========================================================================================================================
int iAp2ConectionHandleEventTelephonyCallStateInformation(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventTelephonyCallStateInformation() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    TelephonyCallStateInformationCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
        memset(&cbArg, 0x00, sizeof(cbArg));
    
    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.callStatePhoneNum.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.callStatePhoneNum.bPresent == TRUE)
        cbArg.callStatePhoneNum.value =  iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.callStatePhoneNum.size);
    cbArg.callStateNetSentCallerName.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.callStateNetSentCallerName.bPresent == TRUE)
        cbArg.callStateNetSentCallerName.value =  iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.callStateNetSentCallerName.size);
    cbArg.callStateStatus.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.callStateStatus.bPresent == TRUE)
        cbArg.callStateStatus.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.callStateDirection.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.callStateDirection.bPresent == TRUE)
        cbArg.callStateDirection.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.uniqueCallID.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.uniqueCallID.bPresent == TRUE)
        cbArg.uniqueCallID.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.uniqueCallID.size);
    cbArg.callStatevCardFTID.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.callStatevCardFTID.bPresent == TRUE)
        cbArg.callStatevCardFTID.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);

//    LOGD("kParamIdCallStatePhoneNumber() bPresent[%d] :: size[%d] :: value[%s]", cbArg.callStatePhoneNum.bPresent, cbArg.callStatePhoneNum.size, cbArg.callStatePhoneNum.value);
//    LOGD("kParamIdCallStateNetSentCallerNumber() bPresent[%d] :: size[%d] :: value[%s]", cbArg.callStateNetSentCallerName.bPresent, cbArg.callStateNetSentCallerName.size, cbArg.callStateNetSentCallerName.value);
//    LOGD("kParamIdCallStateStatus() bPresent[%d] :: value[%d]", cbArg.callStateStatus.bPresent, cbArg.callStateStatus.value);
//    LOGD("kParamIdCallStateDirection() bPresent[%d] :: value[%d]", cbArg.callStateDirection.bPresent, cbArg.callStateDirection.value);
//    LOGD("kParamIdUniqueCallID() bPresent[%d] :: size[%d] :: value[%s]", cbArg.uniqueCallID.bPresent, cbArg.uniqueCallID.size, cbArg.uniqueCallID.value);
//    LOGD("kParamIdCallStatevCardFTID() bPresent[%d] :: value[%d]", cbArg.callStatevCardFTID.bPresent, cbArg.callStatevCardFTID.value);
    
    if ((handle->delegate != NULL) && (handle->delegate->telephonyCallStateInformation_f != NULL)) {
        result = handle->delegate->telephonyCallStateInformation_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.telephonyCallStateInformation_f is NULL");
    }
    
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventTelephonyUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventTelephonyUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventTelephonyUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    TelephonyUpdateCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    memset(&cbArg, 0x00, sizeof(cbArg));
    
    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.telephonySignalStrength.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.telephonySignalStrength.bPresent == TRUE)
        cbArg.telephonySignalStrength.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.telephonyRegisStatus.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.telephonyRegisStatus.bPresent == TRUE)
        cbArg.telephonyRegisStatus.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.telephonyAirPlaneModeStatus.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.telephonyAirPlaneModeStatus.bPresent == TRUE)
        cbArg.telephonyAirPlaneModeStatus.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.telephonyTTYStatus.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.telephonyTTYStatus.bPresent == TRUE)
        cbArg.telephonyTTYStatus.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.mobileOperator.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.mobileOperator.bPresent == TRUE)
        cbArg.mobileOperator.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.mobileOperator.size);


//    LOGD("    telephonySignalStrength ::: bPresent[%d] value[%d]", cbArg.telephonySignalStrength.bPresent, cbArg.telephonySignalStrength.value);
//    LOGD("    telephonyRegisStatus ::: bPresent[%d] value[%d]", cbArg.telephonyRegisStatus.bPresent, cbArg.telephonyRegisStatus.value);
//    LOGD("    telephonyAirPlaneModeStatus :: bPresent[%d] value[%d]", cbArg.telephonyAirPlaneModeStatus.bPresent, cbArg.telephonyAirPlaneModeStatus.value);
//    LOGD("    telephonyTTYStatus ::: bPresent[%d] value[%d]", cbArg.telephonyTTYStatus.bPresent, cbArg.telephonyTTYStatus.value);
//    LOGD("    mobileOperator ::: bPresent[%d] size[%d] value[%s]", cbArg.mobileOperator.bPresent, cbArg.mobileOperator.size, cbArg.mobileOperator.value);

     if ((handle->delegate != NULL) && (handle->delegate->telephonyUpdate_f != NULL)) {
        result = handle->delegate->telephonyUpdate_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.telephonyUpdate_f is NULL");
    }
    
    return result;
}




