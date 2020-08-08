/*
    File:       iAp2AppFrameworkWiFiInforamtion.c
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
#include "iAp2AppFrameworkWiFiInformation.h"
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
int iAP2AppRequestWifiInformation(iAp2AppRef inRef)
{
        int retValue = kIap2NoErr;

    LOGD("iAP2AppRequestWifiInformation() inRef[%p]", inRef);

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

    pCommandHeader->cmd = kCmdRequestWiFiInformation;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAp2ConectionHandleEventVoiceOverCursorUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventWiFiInformation(iAp2AppRef inRef, unsigned char* pRxData)
{
       int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventWiFiInformation() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->WiFiInformation_f == NULL) {
        LOGW("    WARING : delegate->WiFiInformation_f is NULL");
        return kIap2NoErr;
    }

    WiFiInformationCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    memset(&cbArg, 0x00, sizeof(cbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.RequestStatus = (WiFiInfoRequestStatus)iAp2MarshalGetUint32(&iAp2Marshal);
    
    if(cbArg.WiFiSSID.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, cbArg.WiFiSSID.value, cbArg.WiFiSSID.size);
    }

    if(cbArg.WifiPassphrase.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, cbArg.WifiPassphrase.value, cbArg.WifiPassphrase.size);
    }    

    if ((handle->delegate != NULL) && (handle->delegate->WiFiInformation_f != NULL)) {
        result = handle->delegate->WiFiInformation_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.WiFiInformation_f is NULL");
    }
    
    return result;
}



