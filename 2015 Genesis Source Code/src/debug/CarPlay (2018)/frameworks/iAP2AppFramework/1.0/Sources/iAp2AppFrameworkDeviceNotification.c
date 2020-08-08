/*
    File:       iAp2AppFrameworkDeviceNotification.c
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

#include "iAp2AppFramework.h"

#include "iAp2AppFrameworkDeviceNotification.h"
#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkPrivate.h"
#include "iAp2Marshal.h"
#include "iAp2Log.h"


//===========================================================================================================================
//    Definitions
//===========================================================================================================================


//===========================================================================================================================
//    Declarations
//===========================================================================================================================



//===========================================================================================================================
//    iAp2ConectionHandleEventDeviceInformationUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventDeviceInformationUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventDeviceInformationUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

        DeviceInformationUpdateCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    memset(&cbArg, 0x00, sizeof(cbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.bPresent == TRUE)
    {
        cbArg.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.size);
        LOGD("Device Information size[%u] information[%s]", cbArg.size, cbArg.value);
    }

    if ((handle->delegate != NULL) && (handle->delegate->deviceInformationUpdate_f != NULL)) {
        result = handle->delegate->deviceInformationUpdate_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.deviceInformationUpdate_f is NULL");
    }
    
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventDeviceLanguageUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventDeviceLanguageUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventDeviceLanguageUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    DeviceLanguageUpdateCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    memset(&cbArg, 0x00, sizeof(cbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if (cbArg.bPresent == TRUE)
    {
        cbArg.value = iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.size);
        LOGD("Device Language size[%u] information[%s]", cbArg.size, cbArg.value);
    }

    if ((handle->delegate != NULL) && (handle->delegate->deviceLanguageUpdate_f != NULL)) {
        result = handle->delegate->deviceLanguageUpdate_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.deviceLanguageUpdate_f is NULL");
    }
    
    return result;
}




