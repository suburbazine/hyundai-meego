/*
    File:       iAp2AppFrameworkExternalAccessoryProtocol.c
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
#include "iAp2AppFrameworkExternalAccessoryProtocol.h"
#include "iAp2AppFrameworkCommon.h"
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
//    iAp2AppStatusExternalAccessoryProtocolSession
//===========================================================================================================================
int iAp2AppStatusExternalAccessoryProtocolSession(iAp2AppRef inRef, StatusExternalAccessoryProtocolSessionArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppStatusExternalAccessoryProtocolSession() ");

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

    int mallocSize = sizeof(CommandHeader) + sizeof(StatusExternalAccessoryProtocolSessionArg) + sizeof(uint32_t)*(10);
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    //LOGD("inArg->nExternalAccessroyProtocolSessionIdentifier : %u", inArg->nExternalAccessroyProtocolSessionIdentifier);
    //LOGD("inArg->eExternalAccessoryProtocolSessionStatus : %u", inArg->eExternalAccessoryProtocolSessionStatus); 

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->nExternalAccessroyProtocolSessionIdentifier);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->eExternalAccessoryProtocolSessionStatus);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdStatusExternalAccessoryProtocolSession;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAp2AppExternalAccessoryProtocolData
//===========================================================================================================================
int iAp2AppExternalAccessoryProtocolData(iAp2AppRef inRef, ExternalAccessoryProtocolDataArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppExternalAccessoryProtocolData() inRef[%p]", inRef);

    if (inArg == NULL) {
        LOGE("ERROR : inArg is NULL");
        return kIap2ParamErr;
    }

    if(inArg->size <= 0) {
        LOGE("ExternalAccessoryProtocl data size 0");
        return kIap2ValueErr;
    }

    if(inArg->value == NULL) {
        LOGE("ExternalAccessoryProtocl data is NULL");
        return kIap2ValueErr;
    }

    uint32_t byteNum = 0;

    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    //uint32_t ipcData[512/4];
    int mallocSize = sizeof(CommandHeader) + sizeof(ExternalAccessoryProtocolDataArg) + inArg->size;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    CommandHeader* pCommandHeader = (CommandHeader*)pIpcData;
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutBytes(&iAp2Marshal, inArg->value, inArg->size);
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->nExternalAccessroyProtocolSessionIdentifier);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pCommandHeader->cmd = kCmdExternalAccessoryProtocolData;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;
    
    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}



//===========================================================================================================================
//    iAp2ConectionHandleEventStartExternalAccessoryProtocolSession
//===========================================================================================================================
int iAp2ConectionHandleEventStartExternalAccessoryProtocolSession(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventStartExternalAccessoryProtocolSession() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    StartExternalAccessoryProtocolSessionEvt evt;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    evt.ExternalAccessoryProtocolIdentifier = (uint8_t)iAp2MarshalGetUint32(&iAp2Marshal);
    evt.ExternalAccessoryProtocolSessionIdentifier = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);

    if ((handle->delegate != NULL) && (handle->delegate->StartExternalAccessoryProtocolSession_f != NULL)) {
        result = handle->delegate->StartExternalAccessoryProtocolSession_f(handle->delegate->context, &evt);
    }
    else {
        LOGW("    WARING : delegate.StartExternalAccessoryProtocolSession_f is NULL");
    }
    
    return result;
}


//===========================================================================================================================
//    iAp2ConectionHandleEventStopExternalAccessoryProtocolSession
//===========================================================================================================================
int iAp2ConectionHandleEventStopExternalAccessoryProtocolSession(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventStopExternalAccessoryProtocolSession() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    StopExternalAccessoryProtocolSessionEvt evt;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    evt.ExternalAccessoryProtocolSessionIdentifier = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);

    if ((handle->delegate != NULL) && (handle->delegate->StopExternalAccessoryProtocolSession_f != NULL)) {
        result = handle->delegate->StopExternalAccessoryProtocolSession_f(handle->delegate->context, &evt);
    }
    else {
        LOGW("    WARING : delegate.StartExternalAccessoryProtocolSession_f is NULL");
    }
    
    return result;
}


//===========================================================================================================================
//    iAp2ConectionHandleExternalAccessoryProtocolData
//===========================================================================================================================
int iAp2ConectionHandleExternalAccessoryProtocolData(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleExternalAccessoryProtocolData() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    ExternalAccessoryProtocolData evt;
    uint8_t* AllEAPDatagram;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    /* byte is SessionIdentifier */
    AllEAPDatagram = iAp2MarshalGetBytes(&iAp2Marshal, &evt.size);
    if ( AllEAPDatagram != NULL) {
        evt.ExternalAccessoryProtocolSessionIdentifier = AllEAPDatagram[0];
        evt.ExternalAccessoryProtocolSessionIdentifier = evt.ExternalAccessoryProtocolSessionIdentifier << 8;
        evt.ExternalAccessoryProtocolSessionIdentifier = evt.ExternalAccessoryProtocolSessionIdentifier |AllEAPDatagram[1];
    
        evt.value = AllEAPDatagram + 2;
        evt.size = evt.size - 2;
        //LOGD("evt.ExternalAccessoryProtocolSessionIdentifier : %u", evt.ExternalAccessoryProtocolSessionIdentifier);
        //LOGD("evt.value : %s", evt.value);
        //LOGD("evt.size : %u", evt.size);
   
        if ((handle->delegate != NULL) && (handle->delegate->ExternalAccessoryProtocolData_f != NULL)) {
            result = handle->delegate->ExternalAccessoryProtocolData_f(handle->delegate->context, &evt);
        }
        else {
            LOGW("    WARING : delegate.ExternalAccessoryProtocolData_f is NULL");
        }
    }
    else {
        LOGE("evt.AllEAPDatagram is NULL , Don't send data to application");
    }      
    
    return result;
}

