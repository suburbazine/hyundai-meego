/*
    File:       iAp2AppFrameworkUSBDeviceModeAudio.c
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
#include "iAp2AppFrameworkUSBDeviceModeAudio.h"
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
//    iAP2AppStartUSBDeviceModeAudio
//===========================================================================================================================
int iAP2AppStartUSBDeviceModeAudio(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppStartUSBDeviceModeAudio() inRef[%p]", inRef);

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

    ipcData[0] = kCmdStartUSBDeviceModeAudio;
    ipcData[1] = (uint32_t)inRef;
    ipcData[2] = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, ipcData, byteNum);
    return retValue;
}


//===========================================================================================================================
//    iAP2AppStopUSBDeviceModeAudio
//===========================================================================================================================
int iAP2AppStopUSBDeviceModeAudio(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppStartUSBDeviceModeAudio() inRef[%p]", inRef);

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

    ipcData[0] = kCmdStopUSBDeviceModeAudio;
    ipcData[1] = (uint32_t)inRef;
    ipcData[2] = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, ipcData, byteNum);
    return retValue;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventUSBDeviceModeAudioInformation
//===========================================================================================================================
int iAp2ConectionHandleEventUSBDeviceModeAudioInformation(iAp2AppRef inRef, unsigned char* pRxData)
{
       int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventUSBDeviceModeAudioInformation() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->USBDeviceMdoeAudioInformation_f == NULL) {
        LOGW("    WARING : delegate->USBDeviceMdoeAudioInformation_f is NULL");
        return kIap2NoErr;
    }

    USBDeviceModeAudioInformationCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    memset(&cbArg, 0x00, sizeof(cbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.SampleRate = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);

    //LOGD("    maxCurrent.bPresent[%d] value[%u]", cbArg.maxCurrent.bPresent, cbArg.maxCurrent.value);
    //LOGD("    devBatteryCharge.bPresent[%d] value[%u]", cbArg.devBatteryCharge.bPresent, cbArg.devBatteryCharge.value);
    //LOGD("    accPowerMode.bPresent[%d] value[%u]", cbArg.accPowerMode.bPresent, cbArg.accPowerMode.value);

    result = handle->delegate->USBDeviceMdoeAudioInformation_f (handle->delegate->context, &cbArg);
    return result;
}



