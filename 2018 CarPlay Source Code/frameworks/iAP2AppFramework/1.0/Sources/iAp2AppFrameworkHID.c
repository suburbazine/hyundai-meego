/*
    File:       iAp2AppFrameworkHID.c
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
#include "iAp2AppFrameworkHID.h"
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
//    iAp2AppStartHID
//===========================================================================================================================
int iAp2AppStartHID(iAp2AppRef inRef, StartHIDArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStartHID() inRef[%p]", inRef);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*(100); // Don't know discriptor size
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDCompnetIdentifier);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->VendorIdentifier);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->ProductIdentifer);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->LocalizedKeyBoardCountryCode.bPresent);
    if (inArg->LocalizedKeyBoardCountryCode.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->LocalizedKeyBoardCountryCode.value);
    iAp2MarshalPutBytes(&iAp2Marshal, inArg->HIDReportDescriptor.value,inArg->HIDReportDescriptor.size); 

    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdStartHID;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppStopHID
//===========================================================================================================================
int iAp2AppStopHID(iAp2AppRef inRef, StopHIDArg* inArg)
{
       int retValue = kIap2NoErr;

    LOGD("iAp2AppStopHID() inRef[%p]", inRef);

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
    
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*(1);
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDCompnetIdentifier);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdStopHID;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;

}

//===========================================================================================================================
//    iAp2AppAccessoryHIDReport
//===========================================================================================================================
int iAp2AppAccessoryHIDReport(iAp2AppRef inRef, AccessoryHIDReportArg* inArg)
{
       int retValue = kIap2NoErr;

    LOGD("iAp2AppAccessoryHIDReport() inRef[%p]", inRef);

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
    
    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*(100);
       uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->HIDCompnetIdentifier);
    iAp2MarshalPutBytes(&iAp2Marshal, inArg->HIDReport.value,inArg->HIDReport.size);

    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdAccessoryHIDReport;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;

}

//===========================================================================================================================
//    iAp2ConectionHandleEventDeviceHIDReport
//===========================================================================================================================
int iAp2ConectionHandleEventDeviceHIDReport(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventDeviceHIDReport() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

       if (handle->delegate->deviceHIDReport_f == NULL) {
        LOGW("    WARING : delegate->deviceHIDReport_f is NULL");
        return kIap2NoErr;
    }

    DeviceHIDReportCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    memset(&cbArg, 0x00, sizeof(DeviceHIDReportCbArg));
    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.HIDComponentIdentifier = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.HIDReport = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal); //must change type to blob
    
    if ((handle->delegate != NULL) && (handle->delegate->deviceHIDReport_f != NULL)) {
        result = handle->delegate->deviceHIDReport_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.deviceHIDReport_f is NULL");
    }
    
    return result;
}

