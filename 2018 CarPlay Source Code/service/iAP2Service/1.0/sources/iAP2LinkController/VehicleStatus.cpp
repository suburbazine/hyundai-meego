/*
    File:       VehicleStatus.cpp
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

#include "VehicleStatus.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

VehicleStatus::VehicleStatus()
{
    LOGD("VehicleStatus()");
}

VehicleStatus::~VehicleStatus()
{
    LOGD("~VehicleStatus()");
}


void VehicleStatus::handleStartVehicleStatusUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    StartVehicleStatusUpdatesEvt event;

    LOGD("handleStartVehicleStatus() session[%d]");
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeStartVehicleStatusUpdates(pData, &event);

    result = handleStartVehicleStatusUpdateEvent(pLnkCtrler, session, &event);
    
    if (result != kIap2NoErr)
        LOGE("    ERROR : handleStartVehicleStatusUpdateEvent(). result[%d]", result);


}

void VehicleStatus::handleStopVehicleStatusUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;

    LOGD("handleStopVehicleStatus() session[%d]");
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }
    
    result = handleStopVehicleStatusUpdateEvent(pLnkCtrler, session);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleStopVehicleStatus(). result[%d]", result);

}


void VehicleStatus::decodeStartVehicleStatusUpdates(uint8_t* pData, StartVehicleStatusUpdatesEvt* pEvt)
{
    int msgLen;

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    if (pEvt == NULL) {
        LOGE("    Error : pEvt is NULL");
        return;
    }

    msgLen = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);

    LOGD("decodeStartVehicleStatusUpdates() nMsgLen[%d]", msgLen);

    int paramStartIdx = 6;
    int paramId = 0; 
    int paramLength = 0;

    while (msgLen > paramStartIdx) {
            paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
            paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
            paramStartIdx += 4;

        switch (paramId) {
            case kParamIdVehicleStatusRange : {
                pEvt->bRange= TRUE;
                LOGD("kParamIdVehicleStatusRange");
                break;
            }

            case kParamIdVehicleStatusOutSideTemperature : {
                pEvt->bOutSideTemperature = TRUE;        
                LOGD("kParamIdVehicleStatusOutSideTemperature");
                break;
            }

            case kParamIdVehicleStatusRangeWarning : {
                pEvt->bRangeWarning = TRUE;    
                LOGD("kParamIdVehicleStatusOutSideTemperature");
                break;
            }            
        }
        paramStartIdx += paramLength;

    }
}
int VehicleStatus::handleStartVehicleStatusUpdateEvent(LinkController* pLnkCtrler,int8_t session, StartVehicleStatusUpdatesEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleStartVehicleStatusUpdateEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*3);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*3, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bRange);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bOutSideTemperature);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bRangeWarning);
    LOGD("pEvent->bRange : %u", pEvent->bRange);
    LOGD("pEvent->bOutSideTemperature : %u", pEvent->bOutSideTemperature);
    LOGD("pEvent->bRangeWarning : %u", pEvent->bRangeWarning);    
        
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    mEventHeader->evt = kEvtStartVehicleStatusUpdates;
    mEventHeader->dataSize = byteNum;

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(session, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGE("    ERROR : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}


int VehicleStatus::handleStopVehicleStatusUpdateEvent(LinkController* pLnkCtrler, int8_t session)
{
    int result = kIap2NoErr;

    LOGD("handleStophicleStatusUpdateEvent() session[%d]", session);

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*1);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    uint32_t dummyValue = 0xFFFFFFFF;

    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*1, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, dummyValue);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtStopVehicleStatusUpdates;
    mEventHeader->dataSize = byteNum;

    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(session, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGE("    ERROR : send event fail. result[%d]", result);
    }

    free(pIpcData);
    return result;
}


} // namespace IAP2

