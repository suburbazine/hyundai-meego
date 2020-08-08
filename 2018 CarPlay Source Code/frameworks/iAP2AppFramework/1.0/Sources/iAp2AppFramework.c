/*
    File:       iAp2AppFramework.c
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

#include "iAp2AppFramework.h"
#include "iAp2Marshal.h"
#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkPrivate.h"
#include "iAp2Connection.h"
#include "iAp2Log.h"
#include "iAp2ExternelLogUtils.h"

#include <signal.h>
#include <pthread.h>

//===========================================================================================================================
//    Definitions
//===========================================================================================================================



//===========================================================================================================================
//    Declarations
//===========================================================================================================================


//===========================================================================================================================
//    iAp2AppInitialize
//===========================================================================================================================
int    iAp2AppInitialize(iAp2AppRef* outRef)
{
    int result = kIap2NoErr;
    uint32_t err = kIap2NoErr;
    struct iAp2AppPrivate* handle = NULL;
    
#if defined( USE_EXTERNEL_LOG_SYSTEM )
    err = iAp2ExternelLogWriteInit();
    if (err) iAp2ExternelLogWriteStop();
#endif    

    LOGI("##### Appframework VERSION : %s #####", VERSION);
    LOGI("iAp2AppInitialize() outRef[%p]", outRef);
    
    signal(SIGPIPE, SIG_IGN); // Ignore SIGPIPE signals so we get EPIPE retValueors from APIs instead of a signal.

    if (outRef == NULL) {
        LOGE("ERROR : outRef is NULL");
        return kIap2ParamErr;
    }

    handle = (struct iAp2AppPrivate *)calloc(1, sizeof(struct iAp2AppPrivate));
    if (handle == NULL) {
        LOGE("ERROR : out of memory. handle is NULL");
        return kIap2NoMemoryErr;
    }

    handle->sockFd = -1;
    handle->connState = CS_USB_NOT_CONNECTED;
    //handle->connState = CS_IDENTIFIED;
    handle->connMode = CM_AUB_HOST;
    handle->carplaySupport = CCS_NONE;
    handle->iap2Support = CIS_NONE;
    
    *outRef = (void*)handle;
    
    return result;
}

//===========================================================================================================================
//    iAp2AppSetConfig
//===========================================================================================================================
int    iAp2AppSetConfig(iAp2AppRef inRef, const iAp2AppConfig* inConfig)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;

    LOGD("iAp2AppSetConfig() inRef[%p]", inRef);

    if (inRef == NULL || inConfig == NULL) {
        LOGE("ERROR : Input parameter is wrong. inRef[%p] inConfig[%p]", inRef, inConfig);
        return kIap2ParamErr;
    }

    handle->config = (iAp2AppConfig *)inConfig;

    return result;
}

//===========================================================================================================================
//    iAp2AppSetDelegate
//===========================================================================================================================
int    iAp2AppSetDelegate(iAp2AppRef inRef, const iAp2AppDelegate* inDelegate)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;

    LOGD("iAp2AppSetDelegate() inRef[%p]", inRef);

    if (inRef == NULL || inDelegate == NULL) {
        LOGE("ERROR : Input parameter is wrong. inRef[%p] inDelegate[%p]", inRef, inDelegate);
        return kIap2ParamErr;
    }

    handle->delegate = (iAp2AppDelegate*) inDelegate;

    return result;
}

//===========================================================================================================================
//    iAp2AppTerminate
//===========================================================================================================================
int    iAp2AppTerminate(iAp2AppRef inRef)
{
    LOGI("iAp2AppTerminate() inRef[%p]", inRef);

    if (inRef != NULL) {
        free(inRef);
    }
    else {
        LOGW("WARNING : inRef is already NULL");
    }
    
#if defined( USE_EXTERNEL_LOG_SYSTEM )        
    iAp2ExternelLogWriteStop();
#endif
    LOGI("iAp2AppTerminate END");
    return kIap2NoErr;
}

//===========================================================================================================================
//    iAp2AppStart
//===========================================================================================================================
int iAp2AppStart(iAp2AppRef inRef)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;


    LOGI("iAp2AppStart() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->config == NULL) {
        LOGE("ERROR : handle->config is NULL");
        return kIap2ParamErr;
    }

    result = iAp2ConnectionConnectToService(inRef);
    if (result < 0) {
        LOGE("    ERROR : iAp2ConnectionConnectToService() failed. result[%d]", result);
        return result;
    }



    result = iAp2ConnectionStartEventHandling(inRef);
    if (result < 0) {
        LOGE("    ERROR : iAp2ConnectionStartEventHandling() failed. result[%d]", result);
        return result;
    }

    // TODO : let's sync.. later.
    usleep(100*1000);

    return result;
}

//===========================================================================================================================
//    iAp2AppStop
//===========================================================================================================================
int iAp2AppStop(iAp2AppRef inRef)
{
    int    result = kIap2NoErr;
    LOGI("iAp2AppStop() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    result = iAp2ConnectionDisconnectToService(inRef);
    if (result < 0) {
        LOGE("    ERROR : iAp2ConnectionDisconnectToService() failed. result[%d]", result);
        return result;
    }

    result = iAp2ConnectionStopEventHandling(inRef);
    return result;
}

//===========================================================================================================================
//    iAp2AppGetConnectionState
//===========================================================================================================================
int iAp2AppGetConnectionState(iAp2AppRef inRef, ConnectionState* outConnState)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;

    LOGD("iAp2AppGetConnectionState() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (outConnState == NULL) {
        LOGE("ERROR : outConnState is NULL");
        return kIap2ParamErr;
    }

    *outConnState = handle->connState;
    LOGD("    *outConnState[%u]", *outConnState);

    return result;
}

//===========================================================================================================================
//    iAp2AppGetConnectionPath
//===========================================================================================================================
int iAp2AppGetConnectionPath(iAp2AppRef inRef, ConnectionPath* outConnPath)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;

    LOGD("iAp2AppGetConnectionPath() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (outConnPath == NULL) {
        LOGE("ERROR : outConnMode is NULL");
        return kIap2ParamErr;
    }

    *outConnPath = handle->connPath;
    LOGD("    *outConnPath[%u]", *outConnPath);

    return result;
}

//===========================================================================================================================
//    iAp2AppGetConnectionMode
//===========================================================================================================================
int iAp2AppGetConnectionMode(iAp2AppRef inRef, ConnectionMode* outConnMode)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;

    LOGD("iAp2AppGetConnectionMode() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (outConnMode == NULL) {
        LOGE("ERROR : outConnMode is NULL");
        return kIap2ParamErr;
    }

    *outConnMode = handle->connMode;
    LOGD("    *outConnMode[%u]", *outConnMode);

    return result;
}

//===========================================================================================================================
//    iAp2AppGetCarplaySupport
//===========================================================================================================================
int iAp2AppGetCarplaySupport(iAp2AppRef inRef, ConnectionCarPlaySupport* outCarplaySupport)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;

    LOGD("iAp2AppGetCarplaySupport() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (outCarplaySupport == NULL) {
        LOGE("ERROR : outCarplaySupport is NULL");
        return kIap2ParamErr;
    }

    *outCarplaySupport = handle->carplaySupport;
    LOGD("    *outCarplaySupport[%u]", *outCarplaySupport);

    return result;
}

//===========================================================================================================================
//    iAp2AppGetIap2Support
//===========================================================================================================================
int iAp2AppGetIap2Support(iAp2AppRef inRef, ConnectionIap2Support* outIap2Support)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;

    LOGD("iAp2AppGetIap2Support() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (outIap2Support == NULL) {
        LOGE("ERROR : outIap2Support is NULL");
        return kIap2ParamErr;
    }

    *outIap2Support = handle->iap2Support;
    LOGD("    *outIap2Support[%u]", *outIap2Support);

    return result;
}


//===========================================================================================================================
//    iAp2AppCarplayOnOff
//===========================================================================================================================
int iAp2AppCarplayOnOff(iAp2AppRef inRef, BOOL CarplayOnOff)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppCarplayOnOff() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*3;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }


    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, CarplayOnOff);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdCarplayOnOff;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

    LOGD("cmd[%u] appHandle[%p] dataSize[%u] dummyValue[0x%x]", pIpcData[0], pIpcData[1], pIpcData[2], pIpcData[3]);

    retValue = iAp2ConnectionSettingCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}



//===========================================================================================================================
//    iAp2AppUSBModeChange
//===========================================================================================================================
int iAp2AppUSBModeChange(iAp2AppRef inRef, ConnectionMode RequestUSBMode)
{
    int retValue = kIap2NoErr;
    
    LOGD("iAp2AppUSBModChange() inRef[%p] RequestUSBMode[%u]", inRef, RequestUSBMode);
    

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*30;
        uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, RequestUSBMode);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdUsbModeChange;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

    retValue = iAp2ConnectionSettingCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    
    return retValue;
}
//===========================================================================================================================


//===========================================================================================================================
//    iAp2AppCarPlayClientState
//===========================================================================================================================
int iAp2AppCarPlayClientState(iAp2AppRef inRef, CarPlayClientState mCarPlayClientState)
{
    int retValue = kIap2NoErr;
    
    LOGD("iAp2AppCarPlayClientState() inRef[%p] CarPlayClientState[%u]", inRef, mCarPlayClientState);
    

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*30;
        uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, mCarPlayClientState);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdCarPlayClientState;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

    retValue = iAp2ConnectionSettingCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    
    return retValue;
}
//===========================================================================================================================



//===========================================================================================================================
//    iAp2AppGetInitConnectionData
//===========================================================================================================================
int iAp2AppGetInitConnectionData(iAp2AppRef inRef)
{
    int retValue = kIap2NoErr;        

    LOGD("iAp2AppGetInitConnectionData()");

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

    
    pCommandHeader->cmd = kCmdGetInitConnectionData;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSettingCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    
    return retValue;
}
//===========================================================================================================================


//===========================================================================================================================
//    iAp2StartCarPlay
//===========================================================================================================================
int iAp2AppStartCarPlay(iAp2AppRef inRef)
{
   int retValue = kIap2NoErr;

    LOGI("iAp2StartCarPlay() inRef[%p]", inRef);

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
    
    pCommandHeader->cmd = kCmdStartCarPlay;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSettingCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}
//===========================================================================================================================


//===========================================================================================================================
//    iAp2ConectionHandleEventConnectionStateUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventConnectionInfoUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventConnectionInfoUpdate() ");

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
            return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->connInfoUpdate_f == NULL) {
        LOGW("    WARING : delegate->connInfoUpdate_f is NULL");
        return kIap2NoErr;
    }


    ConnectionInfoUpdateCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.connState = (ConnectionState)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.connPath  = (ConnectionPath)iAp2MarshalGetUint32(&iAp2Marshal);            
    cbArg.connMode  = (ConnectionMode)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.iap2Support  = (ConnectionIap2Support)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.carplaySupport  = (ConnectionCarPlaySupport)iAp2MarshalGetUint32(&iAp2Marshal);

    handle->connState = cbArg.connState;
    handle->connPath = cbArg.connPath;
    handle->connMode = cbArg.connMode;
    handle->iap2Support = cbArg.iap2Support;
    handle->carplaySupport = cbArg.carplaySupport;
    LOGD("    connState[%u] connMode[%u] carplaySupport[%u] iap2Support[%u]", cbArg.connState, cbArg.connMode, cbArg.carplaySupport, cbArg.iap2Support);
    result = handle->delegate->connInfoUpdate_f(handle->delegate->context, &cbArg);
    return result;
}
//===========================================================================================================================


//===========================================================================================================================
//    iAp2AppTerminateDaemon
//===========================================================================================================================
int iAp2AppTerminateDaemon(iAp2AppRef inRef)
{
   int retValue = kIap2NoErr;

    LOGI("iAp2AppTerminateDaemon() inRef[%p]", inRef);

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

    pCommandHeader->cmd = kCmdTerminateDaemon;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    retValue = iAp2ConnectionSettingCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}
//===========================================================================================================================



//===========================================================================================================================
//    iAp2ConectionHandleEventConnectionStateUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventConnectionStateUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventConnectionStateUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->connStateUpdate_f == NULL) {
        LOGW("    WARING : delegate->connStateUpdate_f is NULL");
        return kIap2NoErr;
    }
    
    ConnectionStateUpdateCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.connState = (ConnectionState)iAp2MarshalGetUint32(&iAp2Marshal);
    handle->connState= cbArg.connState;
    LOGD("    connState[%u]", cbArg.connState);

    result = handle->delegate->connStateUpdate_f(handle->delegate->context, &cbArg);
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventConnectionStateUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventConnectionStateAndPathUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventConnectionStateAndPathUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

#if 0 //Don't need this callback
     if (handle->delegate->connStateAndPathUpdate_f == NULL) {
        LOGW("    WARING : delegate->connStateAndPathUpdate_f is NULL");
        return kIap2NoErr;
    }
#endif

    
    if (handle->delegate->connStateUpdate_f == NULL) {
        LOGW("    WARING : delegate->connStateUpdate_f is NULL");
        return kIap2NoErr;
    }
    
    ConnectionStateAndPathUpdateCbArg cbArg;

    ConnectionStateUpdateCbArg cbArg_2;
    
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.connState = (ConnectionState)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.connPath  = (ConnectionPath)iAp2MarshalGetUint32(&iAp2Marshal);
    handle->connState = cbArg.connState;
    handle->connPath = cbArg.connPath;
    LOGI("    connState[%u] connPath[%u]", cbArg.connState, cbArg.connPath);

    cbArg_2.connState = cbArg.connState;

#if 0 //Don't need this callback    
    result = handle->delegate->connStateAndPathUpdate_f(handle->delegate->context, &cbArg);
#endif

    result = handle->delegate->connStateUpdate_f(handle->delegate->context, &cbArg_2);
    
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventConnectionModeUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventConnectionModeUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventConnectionModeUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->connModeUpdate_f == NULL) {
        LOGW("    WARING : delegate->connModeUpdate_f is NULL");
        return kIap2NoErr;
    }

    ConnectionModeUpdateCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.connMode= (ConnectionMode)iAp2MarshalGetUint32(&iAp2Marshal);
    handle->connMode = cbArg.connMode;
    LOGD("    connMode[%u]", cbArg.connMode);

    result = handle->delegate->connModeUpdate_f(handle->delegate->context, &cbArg);
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventCarplaySupportUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventCarplaySupportUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventCarplaySupportUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->carplaySupportUpdate_f == NULL) {
        LOGW("    WARING : delegate->carplaySupportUpdate_f is NULL");
        return kIap2NoErr;
    }

    ConnectionCarplaySupportUpdateCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.carplaySupport = (ConnectionCarPlaySupport)iAp2MarshalGetUint32(&iAp2Marshal);
    handle->carplaySupport = cbArg.carplaySupport;
    LOGI("    carplaySupport[%u]", cbArg.carplaySupport);

    result = handle->delegate->carplaySupportUpdate_f(handle->delegate->context, &cbArg);
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventIap2SupportUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventIap2SupportUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventIap2SupportUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->iap2SupportUpdate_f == NULL) {
        LOGW("    WARING : delegate->iap2SupportUpdate_f is NULL");
        return kIap2NoErr;
    }

    ConnectionIap2SupportUpdateEvt cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.iap2Support = (ConnectionIap2Support)iAp2MarshalGetUint32(&iAp2Marshal);
    handle->iap2Support = cbArg.iap2Support;
    LOGD("    iap2Support[%u]", cbArg.iap2Support);

    result = handle->delegate->iap2SupportUpdate_f(handle->delegate->context, &cbArg);
    return result;
}

#ifdef ANDROID
//===========================================================================================================================
//    iAp2ConectionHandleEventDeviceInfoUpdateUpdate
//===========================================================================================================================
int iAp2ConectionHandleEventDeviceInfoUpdateUpdate(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventDeviceInfoUpdateUpdate() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->iap2DeviceInfoUpdate_f == NULL) {
        LOGW("    WARING : delegate->iap2DeviceInfoUpdate_f is NULL");
        return kIap2NoErr;
    }

    ConnectionDeviceInfoUpdateEvt cbArg;
    memset(&cbArg, 0x00, sizeof(ConnectionDeviceInfoUpdateEvt));
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.deviceinfo.productId = (int)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.deviceinfo.vendorId = (int)iAp2MarshalGetUint32(&iAp2Marshal);

    cbArg.deviceinfo.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

    if ( cbArg.deviceinfo.bPresent == TRUE) {
        cbArg.deviceinfo.product = (uint8_t *)iAp2MarshalGetBytes(&iAp2Marshal, &cbArg.deviceinfo.size);
        LOGD("cbArg.deviceinfo.product : %s, cbArg.deviceinfo.size : %u", cbArg.deviceinfo.product, cbArg.deviceinfo.size);
    }
    else {
        LOGD(" Product name is NULL");
    }
    
    LOGD("    iap2productId[%x], iap2vendorId[%x]", cbArg.deviceinfo.productId, cbArg.deviceinfo.vendorId);

    result = handle->delegate->iap2DeviceInfoUpdate_f(handle->delegate->context, &cbArg);
    return result;
}
#endif

//===========================================================================================================================
//    iAp2ConectionHandleSocketClose
//===========================================================================================================================
int iAp2ConectionHandleSocketClose(iAp2AppRef inRef)
{
    int result = kIap2NoErr;
    
    struct iAp2AppPrivate* handle = inRef;
    
    LOGI("iAp2ConectionHandleSocketClose() inRef[0x%x]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->SocketClose_f == NULL) {
        LOGW("    WARING : delegate->SocketClose_f is NULL");
        return kIap2NoErr;
    }

    result = handle->delegate->SocketClose_f(handle->delegate->context);
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventAuthenticaitonFailed
//===========================================================================================================================
int iAp2ConectionHandleEventAuthenticaitonFailed(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGI("iAp2ConectionHandleEventAuthenticaitonFailed() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->AuthenticationFailed_f == NULL) {
        LOGW("    WARING : delegate->iAp2AuthenticationFailed_f is NULL");
        return kIap2NoErr;
    }

    result = handle->delegate->AuthenticationFailed_f(handle->delegate->context);
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventAuthenticaitonFailed
//===========================================================================================================================
int iAp2ConectionHandleEventIdentificationFailed(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGI("iAp2ConectionHandleEventIdentificationFailed() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (handle->delegate == NULL) {
        LOGE("    ERROR : delegate is NULL");
        return kIap2NotInitializedErr;
    }

    if (handle->delegate->IdentificationFailed_f == NULL) {
        LOGW("    WARING : delegate->iAp2IdentificationFailed_f is NULL");
        return kIap2NoErr;
    }

    result = handle->delegate->IdentificationFailed_f(handle->delegate->context);
    return result;
}

