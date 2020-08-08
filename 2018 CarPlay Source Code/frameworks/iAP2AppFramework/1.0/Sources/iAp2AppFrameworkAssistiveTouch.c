/*
    File:       iAp2AppFrameworkAssistiveTouch.c
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
#include "iAp2AppFrameworkAssistiveTouch.h"
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
//    iAp2AppStartAssistiveTouch
//===========================================================================================================================
int iAp2AppStartAssistiveTouch(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAp2AppStartAssistiveTouch() inRef[%p]", inRef);

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

    ipcData[0] = kCmdStartAssistiveTouch;
    ipcData[1] = (uint32_t)inRef;
    ipcData[2] = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, ipcData, byteNum);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppStopAssistiveTouch
//===========================================================================================================================
int iAp2AppStopAssistiveTouch(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAp2AppStopAssistiveTouch() inRef[%p]", inRef);

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

    ipcData[0] = kCmdStopAssistiveTouch;
    ipcData[1] = (uint32_t)inRef;
    ipcData[2] = byteNum;

    LOGD("cmd[%u] appHandle[%p] dataSize[%u] dummyValue[0x%x]", ipcData[0], ipcData[1], ipcData[2], ipcData[3]);

    retValue = iAp2ConnectionSendCmd(inRef, ipcData, byteNum);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppStartAssistiveTouchInformation
//===========================================================================================================================
int iAp2AppStartAssistiveTouchInformation(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAp2AppStartAssistiveTouchInformation() inRef[%p]", inRef);

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

    ipcData[0] = kCmdStartAssistiveTouchInformation;
    ipcData[1] = (uint32_t)inRef;
    ipcData[2] = byteNum;

    LOGD("cmd[%u] appHandle[%p] dataSize[%u] dummyValue[0x%x]", ipcData[0], ipcData[1], ipcData[2], ipcData[3]);

    retValue = iAp2ConnectionSendCmd(inRef, ipcData, byteNum);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppStopAssistiveTouchInformation
//===========================================================================================================================
int iAp2AppStopAssistiveTouchInformation(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAp2AppStopAssistiveTouchInformation() inRef[%p]", inRef);

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

    ipcData[0] = kCmdStopAssistiveTouchInformation;
    ipcData[1] = (uint32_t)inRef;
    ipcData[2] = byteNum;

    LOGD("cmd[%u] appHandle[%p] dataSize[%u] dummyValue[0x%x]", ipcData[0], ipcData[1], ipcData[2], ipcData[3]);

    retValue = iAp2ConnectionSendCmd(inRef, ipcData, byteNum);
    return retValue;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventAssistiveTouchInformation
//===========================================================================================================================
int iAp2ConectionHandleEventAssistiveTouchInformation(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventAssistiveTouchInformation() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->assistiveTouchInformation_f == NULL) {
        LOGW("    WARING : delegate->assistiveTouchInformation_f is NULL");
        return kIap2NoErr;
    }

    AssistiveTouchInformationCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    memset(&cbArg, 0x00, sizeof(cbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.IsEnabled = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    
    result = handle->delegate->assistiveTouchInformation_f(handle->delegate->context, &cbArg);
    return result;
}


