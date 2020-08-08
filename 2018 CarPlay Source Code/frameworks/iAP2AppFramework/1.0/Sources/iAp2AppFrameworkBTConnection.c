/*
    File:       iAp2AppFrameworkBTConnection.c
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
#include "iAp2AppFrameworkBTConnection.h"
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
//    iAp2AppBTComponentInformation
//===========================================================================================================================
int iAp2AppBTComponentInformation(iAp2AppRef inRef, BTComponentInformationArg* inArg)
{
    int retValue = kIap2NoErr;
    LOGD("iAp2AppBTComponentInformation() inRef[%p]", inRef);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(BTComponentInformationArg) + sizeof(BTComponentStatusParam)*inArg->nBTComponentStatusCount;
    mallocSize = mallocSize * 4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bPresent);
    iAp2MarshalPutBytes(&iAp2Marshal, (uint8_t*)inArg->btComponentStatus, sizeof(BTComponentStatusParam)*inArg->nBTComponentStatusCount);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdBTComponentInformation;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

//===========================================================================================================================
//    iAp2AppStartBTConnectionUpdates
//===========================================================================================================================
int iAp2AppStartBTConnectionUpdates(iAp2AppRef inRef, StartBTConnectionUpdatesArg* inArg)
{
    int retValue = kIap2NoErr;
    LOGD("iAp2AppStartBTConnectionUpdates() inRef[%p]", inRef);

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
    int mallocSize = sizeof(CommandHeader) + sizeof(StartBTConnectionUpdatesArg) + sizeof(uint16_t)*inArg->nBTTransportComponentIDCount;
    mallocSize = mallocSize * 4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->bPresent);    
    iAp2MarshalPutBytes(&iAp2Marshal, (uint8_t*)inArg->BTTransportComponentID, sizeof(uint16_t)*inArg->nBTTransportComponentIDCount);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdStartBTConnectionUpdates;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAp2AppStopBTConnectionUpdates
//===========================================================================================================================
int iAp2AppStopBTConnectionUpdates(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStopBTConnectionUpdates() inRef[%p]", inRef);

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

    ipcData[0] = kCmdStopBTConnectionUpdates;
    ipcData[1] = (uint32_t)inRef;
    ipcData[2] = byteNum;

    LOGD("cmd[%u] appHandle[%p] dataSize[%u] dummyValue[0x%x]", ipcData[0], ipcData[1], ipcData[2], ipcData[3]);

    retValue = iAp2ConnectionSendCmd(inRef, ipcData, byteNum);
    return retValue;

}


//===========================================================================================================================
//    iAp2ConectionHandleEventBTConnectionUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventBTConnectionUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventBTConnectionUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->btConnectionUpdate_f == NULL) {
        LOGW("    WARING : delegate->BTConnectionUpdate is NULL");
        return kIap2NoErr;
    }

    BTConnectionUpdateCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    memset(&cbArg, 0x00, sizeof(cbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.BTTransPortComponentID = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.mConnectedBTProfiles.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    if( cbArg.mConnectedBTProfiles.bPresent == TRUE ) {
        cbArg.mConnectedBTProfiles.BTHandsFree.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
        if (cbArg.mConnectedBTProfiles.BTHandsFree.bPresent == TRUE) {
            cbArg.mConnectedBTProfiles.BTHandsFree.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("cbArg.mConnectedBTProfiles.BTHandsFree.value : %u", cbArg.mConnectedBTProfiles.BTHandsFree.value);
        }
        cbArg.mConnectedBTProfiles.BTPhoneAddressAccess.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
        if (cbArg.mConnectedBTProfiles.BTPhoneAddressAccess.bPresent == TRUE) {
            cbArg.mConnectedBTProfiles.BTPhoneAddressAccess.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("cbArg.mConnectedBTProfiles.BTPhoneAddressAccess.value : %u", cbArg.mConnectedBTProfiles.BTHandsFree.value);
        }
        cbArg.mConnectedBTProfiles.BTAudioVideoRemoteControl.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
        if (cbArg.mConnectedBTProfiles.BTAudioVideoRemoteControl.bPresent == TRUE) {
            cbArg.mConnectedBTProfiles.BTAudioVideoRemoteControl.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("cbArg.mConnectedBTProfiles.BTAudioVideoRemoteControl.value : %u", cbArg.mConnectedBTProfiles.BTHandsFree.value);
        }
        cbArg.mConnectedBTProfiles.BTAdvanceAudioDistribution.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
        if (cbArg.mConnectedBTProfiles.BTAdvanceAudioDistribution.bPresent == TRUE) {
            cbArg.mConnectedBTProfiles.BTAdvanceAudioDistribution.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("cbArg.mConnectedBTProfiles.BTAdvanceAudioDistribution.value : %u", cbArg.mConnectedBTProfiles.BTHandsFree.value);
        }
        cbArg.mConnectedBTProfiles.BTHumanInterfaceDevice.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
        if (cbArg.mConnectedBTProfiles.BTHumanInterfaceDevice.bPresent == TRUE) {
            cbArg.mConnectedBTProfiles.BTHumanInterfaceDevice.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("cbArg.mConnectedBTProfiles.BTHumanInterfaceDevice.value : %u", cbArg.mConnectedBTProfiles.BTHandsFree.value);
        }
        cbArg.mConnectedBTProfiles.BTiAP2Link.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
        if (cbArg.mConnectedBTProfiles.BTiAP2Link.bPresent == TRUE) {
            cbArg.mConnectedBTProfiles.BTiAP2Link.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("cbArg.mConnectedBTProfiles.BTiAP2Link.value : %u", cbArg.mConnectedBTProfiles.BTHandsFree.value);
        }
        cbArg.mConnectedBTProfiles.BTPANAP.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
        if (cbArg.mConnectedBTProfiles.BTPANAP.bPresent == TRUE) {
            cbArg.mConnectedBTProfiles.BTPANAP.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("cbArg.mConnectedBTProfiles.BTPANAP.value : %u", cbArg.mConnectedBTProfiles.BTHandsFree.value);
        }
        cbArg.mConnectedBTProfiles.BTMessageAccess.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
        if (cbArg.mConnectedBTProfiles.BTMessageAccess.bPresent == TRUE) {
            cbArg.mConnectedBTProfiles.BTMessageAccess.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("cbArg.mConnectedBTProfiles.BTMessageAccess.value : %u", cbArg.mConnectedBTProfiles.BTHandsFree.value);
        }
        cbArg.mConnectedBTProfiles.BTPANC.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
        if (cbArg.mConnectedBTProfiles.BTPANC.bPresent == TRUE) {
            cbArg.mConnectedBTProfiles.BTPANC.value = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            LOGD("cbArg.mConnectedBTProfiles.BTPANC.value : %u", cbArg.mConnectedBTProfiles.BTHandsFree.value);
        }
        
    }

    //LOGD("    componentID [%d] ", cbArg.componentID);
    //LOGD("    connectedBTProfiles.bPresent [%d] value[%u]", cbArg.connectedBTProfiles.bPresent , cbArg.connectedBTProfiles.value);

    result = handle->delegate->btConnectionUpdate_f(handle->delegate->context, &cbArg);
    return result;
}



