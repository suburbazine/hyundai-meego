/*
    File:       iAp2AppFrameworkVehicleStatus.c
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
#include "iAp2AppFrameworkVehicleStatus.h"
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
//    iAP2AppVehicleStatusUpdate
//===========================================================================================================================
int iAP2AppVehicleStatusUpdate(iAp2AppRef inRef, VehicleStatusUpdateArg* inArg)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppVehicleStatusUpdate() inRef[%p]", inRef);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*6;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->range.bPresent);
    if(inArg->range.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->range.value);    
    }
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->outSideTemp.bPresent);
    if( inArg->outSideTemp.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->outSideTemp.value);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->rangeWarning.bPresent);
    
    if( inArg->rangeWarning.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->rangeWarning.value);
    }
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pCommandHeader->cmd = kCmdVehicleStatusUpdate;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAp2ConectionHandleEventStartVehicleStatusUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventStartVehicleStatusUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventStartVehicleStatusUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    StartVehicleStatusUpdatesCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    memset(&cbArg, 0x00, sizeof(cbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.bRange = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.bOutSideTemperature = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.bRangeWarning = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    LOGD("    cbArg.bRange[%u]", cbArg.bRangeWarning);
    LOGD("    cbArg.bOutSideTemperature[%u]", cbArg.bOutSideTemperature);
    LOGD("    cbArg.bRangeWarning[%u]", cbArg.bRangeWarning);

    if ((handle->delegate != NULL) && (handle->delegate->startVehicleStatus_f != NULL)) {
        result = handle->delegate->startVehicleStatus_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.startVehicleStatus_f is NULL");
    }
    
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventStopVehicleStatusUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventStopVehicleStatusUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventStopVehicleStatusUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    // No Param
    
    if ((handle->delegate != NULL) && (handle->delegate->stopVehicleStatus_f != NULL)) {
        result = handle->delegate->stopVehicleStatus_f(handle->delegate->context);
    }
    else {
        LOGW("    WARING : delegate.stopVehicleStatus_f is NULL");
    }
    
    return result;
}



