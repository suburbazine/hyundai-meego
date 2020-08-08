/*
    File:       iAp2AppFrameworkPower.c
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
#include "iAp2AppFrameworkPower.h"
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
//    iAp2AppStartPowerUpdates
//===========================================================================================================================
int iAp2AppStartPowerUpdates(iAp2AppRef inRef, StartPowerUpdateArg* inArg)
{
#ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_
    LOGD("iAp2AppStartPowerUpdates() inRef[%p] inArg[%p]", inRef, inArg);
    return kIap2DeprecatedErr;

#else // #ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_
    int retValue = kIap2NoErr;
    LOGD("iAp2AppStartPowerUpdates() inRef[%p] inArg[%p]", inRef, inArg);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*10;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bMaxCurrentDrawnFromAcc);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bDevBatteryCharge);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bAccPowerMode);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bIsExternalChargerConnected);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bBatteryChargingState);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bBatteryChargeLevel);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdStartPowerUpdates;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;    

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
#endif // #ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_
}

//===========================================================================================================================
//    iAp2AppStopPowerUpdates
//===========================================================================================================================
int iAp2AppStopPowerUpdates(iAp2AppRef inRef)
{
#ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_
    LOGD("iAp2AppStopPowerUpdates() inRef[%p]", inRef);
    return kIap2DeprecatedErr;
    
#else // #ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_

    int retValue = kIap2NoErr;

    LOGD("iAp2AppStopPowerUpdates() inRef[%p]", inRef);

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

    ipcData[0] = kCmdStopPowerUpdates;
    ipcData[1] = (uint32_t)inRef;
    ipcData[2] = byteNum;

    LOGD("cmd[%u] appHandle[%p] dataSize[%u] dummyValue[0x%x]", ipcData[0], ipcData[1], ipcData[2], ipcData[3]);

    retValue = iAp2ConnectionSendCmd(inRef, ipcData, byteNum);
    return retValue;
#endif // #ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_
}


//===========================================================================================================================
//    iAp2AppPowerSourceUpdate
//===========================================================================================================================
int iAp2AppPowerSourceUpdate(iAp2AppRef inRef, PowerSourceUpdateArg* inArg)
{
#ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_
    LOGD("iAp2AppPowerSourceUpdate() inRef[%p] inArg[%p]", inRef, inArg);
    return kIap2DeprecatedErr;
    
#else // #ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_

    int retValue = kIap2NoErr;

    LOGD("iAp2AppPowerSourceUpdate() inRef[%p]", inRef);

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

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->availCurrent.bPresent);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->availCurrent.value);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->devBatteryCharge.bPresent);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->devBatteryCharge.value);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdPowerSourceUpdate;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

    LOGD("cmd[%u] appHandle[%p] dataSize[%u]", pIpcData[0], pIpcData[1], pIpcData[2]);
    LOGD("availCurrent bPresent[%u] value[%u] devBatteryCharge bPresent[%d] value[%u]", pIpcData[3], pIpcData[4], pIpcData[5], pIpcData[6]);

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    
    return retValue;
#endif // #ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_
}

//===========================================================================================================================
//    iAp2ConectionHandleEventPowerUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventPowerUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
#ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_
    LOGD("iAp2ConectionHandleEventPowerUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);
    return kIap2DeprecatedErr;
    
#else // #ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_

    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventPowerUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->powerUpdate_f == NULL) {
        LOGW("    WARING : delegate->powerUpdate_f is NULL");
        return kIap2NoErr;
    }

    PowerUpdateCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    memset(&cbArg, 0x00, sizeof(cbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.maxCurrent.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.maxCurrent.bPresent == TRUE)
        cbArg.maxCurrent.value = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.devBatteryCharge.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.devBatteryCharge.bPresent == TRUE)
        cbArg.devBatteryCharge.value = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
    
    cbArg.accPowerMode.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.accPowerMode.bPresent == TRUE)
        cbArg.accPowerMode.value = (AccessoryPowerModes)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.IsExternalChargerConnected = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.BatteryChargingState.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.BatteryChargingState.bPresent == TRUE)
        cbArg.BatteryChargingState.value = (BatteryChargingState)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.BatteryChargeLevel.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.BatteryChargeLevel.bPresent == TRUE)
        cbArg.BatteryChargeLevel.value = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);    

    result = handle->delegate->powerUpdate_f(handle->delegate->context, &cbArg);
    return result;
#endif // #ifdef _IAP2APPFW_POWER_FEATURE_IS_DEPRECATED_
}


