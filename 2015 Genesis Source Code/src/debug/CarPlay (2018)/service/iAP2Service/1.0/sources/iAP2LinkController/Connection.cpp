/*
    File:       Connection.cpp
    Package:    LG iAP2 Service
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

#include <vector>

#include "Connection.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
//#include "iAp2ServiceCommon.h"
#include <stdlib.h>

namespace IAP2
{

Connection::Connection() : mIap2Support(CIS_NONE), mConnPath(CP_NONE),  mConnMode(CM_AUB_HOST), mCarplaySupport(CCS_NONE), bAuthentication(FALSE), bIdentification(FALSE), CarplayOnoff(TRUE), mRequestUSBMode(CM_AUB_HOST), mCarPlayClientState(CCS_NOT_READY),mConnState(CS_USB_NOT_CONNECTED)
{
    LOGD("Connection()");
}

Connection::~Connection()
{
    LOGD("~Connection()");
}

int Connection::CmdGetInitConnectionData(LinkController* pLnkCtrler, int currentFd, uint8_t* pRxData)
{
    uint32_t dummyValue = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    int result = kIap2NoErr;

    iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

    dummyValue = iAp2MarshalGetUint32(&iAp2Marshal);
    LOGI("    rcv kCmdGetInitConnectionData dummyValue[0x%x]", dummyValue);

    InitConnectionData initConnData;
    getInitConnectionData(&initConnData);

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*5);
    EventHeader* mEventHeader = (EventHeader*) pIpcData;
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*5, sizeof(EventHeader));
    
    iAp2MarshalPutUint32(&iAp2Marshal, initConnData.connState);
    iAp2MarshalPutUint32(&iAp2Marshal, initConnData.connPath);
    iAp2MarshalPutUint32(&iAp2Marshal, initConnData.connMode);
    iAp2MarshalPutUint32(&iAp2Marshal, initConnData.carplaySupport);
    iAp2MarshalPutUint32(&iAp2Marshal, initConnData.iap2Support);

    LOGD("    mConnState[%u] connPath[%u] mConnMode[%u] carplaySupport[%u] iap2Support[%u]", initConnData.connState, initConnData.connPath, initConnData.connMode, initConnData.carplaySupport, initConnData.iap2Support);
            
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtConnectionInfoUpdate;
    mEventHeader->dataSize = byteNum;
                    
    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendData(currentFd, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    free(pIpcData);
    return result;
}

void Connection::getInitConnectionData(InitConnectionData* pOutArg)
{
    LOGD("getInitConnectionData()");

    if (pOutArg == NULL) {
        LOGE("    Error : pOutArg is NULL");
        return;
    }

    pOutArg->connState = mConnState;
    pOutArg->connPath = mConnPath;
    pOutArg->connMode = mConnMode;
    pOutArg->carplaySupport = mCarplaySupport;
    pOutArg->iap2Support = mIap2Support;
}

void Connection::handleConnectionStateUpdate(LinkController* pLnkCtrler, ConnectionState connState)
{
    int result = kIap2NoErr;
    ConnectionStateUpdateEvt event;

    //LOGD("handleConnectionStateUpdate() connState[%u]", connState);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if(mConnState == connState) {
        LOGW("Connect State is same, mConnPath : %u,  mConnState : %u", mConnPath, mConnState);
        LOGW("Don't update connect state");
        return;
    }

    event.connState = connState;
    mConnState = connState;

    if (connState == CS_USB_NOT_CONNECTED) {
        mConnMode = CM_AUB_HOST;
        mConnPath = CP_NONE;
        mCarplaySupport = CCS_NONE;
        mIap2Support = CIS_NONE;        
    }
        
    handleConnectionStateUpdateEvent(pLnkCtrler, &event);

    if (result != kIap2NoErr)
    {
        LOGW("    WARNING : result[%d]", result);
    }
}

void Connection::handleConnectionStateAndPathUpdate(LinkController* pLnkCtrler, ConnectionState connState, ConnectionPath connPath)
{
    int result = kIap2NoErr;
    ConnectionStateAndPathUpdateEvt event;

    //LOGD("handleConnectionStateAndPathUpdate() connState[%u] connPath[%u]", connState, connPath);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }
    
    if(mConnState == connState) {
        LOGW("Connect State is same, mConnPath : %u,  mConnState : %u", mConnPath, mConnState);
        LOGW("Don't update connect state");
        return;
    }
    
    event.connState = mConnState = connState;
    event.connPath = mConnPath = connPath;

    if (connState == CS_USB_NOT_CONNECTED) {
        mConnMode = CM_AUB_HOST;
        mConnPath = CP_NONE;
        mCarplaySupport = CCS_NONE;
        mIap2Support = CIS_NONE;        
    }
        
    
        result = handleConnectionStateAndPathUpdateEvent(pLnkCtrler, &event);

    if (result != kIap2NoErr)
        LOGW("    WARNING : result[%d]", result);
    
}


void Connection::handleConnectionModeUpdate(LinkController* pLnkCtrler, ConnectionMode connMode)
{
    int result = kIap2NoErr;
    ConnectionModeUpdateEvt event;

    LOGD("handleConnectionModeUpdate() connMode[%u]", connMode);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    event.connMode = connMode;
    mConnMode = connMode;

    
        result = handleConnectionModeUpdateEvent(pLnkCtrler, &event);

        if (result != kIap2NoErr)
            LOGW("    WARNING : result[%d]", result);
    
}

void Connection::handleConnectionCarplaySupportUpdate(LinkController* pLnkCtrler, ConnectionCarPlaySupport carplaySupport)
{
    int result = kIap2NoErr;
    ConnectionCarplaySupportUpdateEvt event;

    LOGD("handleConnectionCarplaySupportUpdate() carplaySupport[%d]", carplaySupport);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    event.carplaySupport = carplaySupport;
    mCarplaySupport = carplaySupport;


        result = handleConnectionCarplaySupportUpdateEvent(pLnkCtrler, &event);

        if (result != kIap2NoErr)
            LOGW("    WARNING : result[%d]", result);

}

void Connection::handleConnectionIap2SupportUpdate(LinkController* pLnkCtrler, ConnectionIap2Support iap2Support)
{
    int result = kIap2NoErr;
    ConnectionIap2SupportUpdateEvt event;

    LOGD("handleConnectionIap2SupportUpdate() iap2Support[%d]", iap2Support);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    event.iap2Support = iap2Support;
    mIap2Support = iap2Support;

        result = handleConnectionIap2SupportUpdateEvent(pLnkCtrler, &event);

        if (result != kIap2NoErr)
            LOGW("    WARNING : result[%d]", result);
    
}

#ifdef ANDROID
void Connection::handleConnectionDeviceInfoUpdate(LinkController* pLnkCtrler, int productId, int vendorId, char *product)
{
    int result = kIap2NoErr;

    ConnectionDeviceInfoUpdateEvt event;
    memset(&event, 0x00, sizeof(ConnectionDeviceInfoUpdateEvt));

    LOGD("handleConnectionDeviceInfoUpdate() product : %s", product);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }    

    event.deviceinfo.productId = productId;
    event.deviceinfo.vendorId = vendorId;

    if (product != NULL) {
        event.deviceinfo.product = (uint8_t*) malloc(strlen(product) + 1);
        memset(event.deviceinfo.product , 0x00, strlen(product) + 1);

        if( event.deviceinfo.product != NULL) {
             strncpy((char*)event.deviceinfo.product, product, strlen(product));
             event.deviceinfo.size = (uint32_t)strlen(product) + 1;

             LOGD(" event.deviceinfo.product : %s", event.deviceinfo.product);
             LOGD(" event.deviceinfo.size : %u", event.deviceinfo.size);
        }
        else {
            LOGE("malloc failed !! handleConnectionDeviceInfoUpdate");
        }
    }
    else {
        LOGI("Product is null");
    }


    result = handleConnectionDeviceInfoUpdateEvent(pLnkCtrler, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : result[%d]", result);   

}
#endif





int Connection::handleConnectionStateUpdateEvent(LinkController* pLnkCtrler,/*int8_t session,*/ ConnectionStateUpdateEvt* pEvent)
{
    int result = kIap2NoErr;

    //LOGD("handleConnectionStateUpdateEvent()");

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*1);
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*1, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->connState);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt =kEvtConnectionStateUpdate;
    mEventHeader->dataSize = byteNum;    

    LOGD("handleConnectionStateUpdateEvent() connState[%u]", pEvent->connState);

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(/*session*/0, pIpcData, byteNum);
    }
    else {
        LOGD("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGW("    WARNING : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}

int Connection::handleConnectionStateAndPathUpdateEvent(LinkController* pLnkCtrler,/*int8_t session,*/ ConnectionStateAndPathUpdateEvt* pEvent)
{
    int result = kIap2NoErr;

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*2);
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*2, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->connState);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->connPath);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtConnectionStateAndPathUpdate;
    mEventHeader->dataSize = byteNum;

    LOGD("handleConnectionStateAndPathUpdateEvent() connState[%u] connPath[%u]", pEvent->connState, pEvent->connPath);

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(/*session*/0, pIpcData, byteNum);
    }
    else {
        LOGD("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGW("    WARNING : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}

int Connection::handleConnectionModeUpdateEvent(LinkController* pLnkCtrler,/*int8_t session,*/ ConnectionModeUpdateEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleConnectionModeUpdateEvent()");

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*1);
    
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*1, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->connMode);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    mEventHeader->evt =kEvtConnectionModeUpdate;
    mEventHeader->dataSize = byteNum;    

    LOGD("    connMode[%u]", pEvent->connMode);

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(/*session*/0, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGW("    WARNING : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}

int Connection::handleConnectionCarplaySupportUpdateEvent(LinkController* pLnkCtrler,/*int8_t session,*/ ConnectionCarplaySupportUpdateEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleConnectionCarplaySupportUpdateEvent()");

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*1);
    
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*1, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->carplaySupport);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt =kEvtConnectionCarplaySupportUpdate;
    mEventHeader->dataSize = byteNum;    

    LOGD("    carplaySupport[%u]", pEvent->carplaySupport);

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(/*session*/0, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGW("    WARNING : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}

int Connection::handleConnectionIap2SupportUpdateEvent(LinkController* pLnkCtrler,/*int8_t session,*/ ConnectionIap2SupportUpdateEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleConnectionIap2SupportUpdateEvent()");

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*1);
    
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*1, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->iap2Support);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt =kEvtConnectionIap2SupportUpdate;
    mEventHeader->dataSize = byteNum;    
    
    LOGD("    iap2Support[%u]", pEvent->iap2Support);

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(/*session*/0, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGW("    WARNING : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}

#ifdef ANDROID
int Connection::handleConnectionDeviceInfoUpdateEvent(LinkController* pLnkCtrler,/*int8_t session,*/ ConnectionDeviceInfoUpdateEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleConnectionDeviceInfoUpdateEvent()");

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = sizeof(ConnectionDeviceInfoUpdateEvt) + pEvent->deviceinfo.size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);

    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->deviceinfo.productId);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->deviceinfo.vendorId);
    
    if( pEvent->deviceinfo.size > 0 || pEvent->deviceinfo.product != NULL) {
        pEvent->deviceinfo.bPresent = TRUE;
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->deviceinfo.bPresent);    
        iAp2MarshalPutBytes(&iAp2Marshal, (uint8_t*)pEvent->deviceinfo.product, pEvent->deviceinfo.size);
        LOGD("pEvent->deviceinfo.product : %s, pEvent->deviceinfo.size : %u", pEvent->deviceinfo.product, pEvent->deviceinfo.size);        
    }
    else {
        pEvent->deviceinfo.bPresent = FALSE;
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->deviceinfo.bPresent);  
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);        

    mEventHeader->evt =kEvtConnectionIap2DeviceInfoUpdate;
    mEventHeader->dataSize = byteNum;        

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(/*session*/0, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSendEventToApp is NULL");
    }

    if (result != kIap2NoErr) {
        LOGW("    WARNING : send event fail. result[%d]", result);
    }
    
    if (pEvent->deviceinfo.product != NULL) {
        free(pEvent->deviceinfo.product);
    }
    
    free(pIpcData);

    return result;
}
#endif

} // namespace IAP2
