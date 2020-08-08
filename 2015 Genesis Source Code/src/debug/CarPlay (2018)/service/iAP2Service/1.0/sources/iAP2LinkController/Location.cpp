/*
    File:       Location.cpp
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

#include "Location.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

Location::Location()
{
    LOGD("Location()");
}

Location::~Location()
{
    LOGD("~Location()");
}

void Location::handleStartLocationInformation(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    StartLocationInformationEvt event;

    LOGD("handleStartLocationInformation() session[%d]");
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeStartLocationInformation(pData, &event);

    result = handleStartLocationInformationEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleStartLocationInformation(). result[%d]", result);    
}

void Location::handleStopLocationInformation(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;

    LOGD("handleStopLocationInformation() session[%d]");
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }    
    
        result = handleStopLocationInformationEvent(pLnkCtrler, session);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleStopLocationInformation(). result[%d]", result);

}


void Location::decodeStartLocationInformation(uint8_t* pData, StartLocationInformationEvt* pEvt)
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

    int paramStartIdx = 6;
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
            paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
            paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);            

        switch (paramId) {
            case kParamIdGPSFixData : {
                pEvt->bGPSFixData = TRUE;
                LOGD("bGPSFixData");                
                break;
            }

            case kParamIdRecommMinSpecificGPSTransitData : {
                pEvt->bRecommMinSpecificGPSTransitData = TRUE;
                LOGD("bRecommMinSpecificGPSTransitData");
                break;
            }

            case kParamIdGPSStatelliteInView : {
                pEvt->bGPSStatelliteInView = TRUE;
                LOGD("bGPSStatelliteInView");
                break;
            }

            case kParamIdVehicleSpeedData : {
                pEvt->bVehicleSpeedData = TRUE;
                LOGD("bVehicleSpeedData");
                break;
            }

            case kParamIdVehicleGyroData : {
                pEvt->bVehicleGyroData = TRUE;
                LOGD("bVehicleGyroData");
                break;
            }

            case kParamIdVehicleAccelData : {
                pEvt->bVehicleAccelData = TRUE;
                LOGD("bVehicleAccelData");
                break;
            }

            case kParamIdVehicleHeadingData : {
                pEvt->bVehicleHeadingData = TRUE;
                LOGD("bVehicleHeadingData");
                break;
            }

        }
        paramStartIdx += paramLength;

    }
}


int Location::handleStartLocationInformationEvent(LinkController* pLnkCtrler, int8_t session, StartLocationInformationEvt* pEvent)
{
    int result = kIap2NoErr;

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*7);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*7, sizeof(EventHeader));


    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bGPSFixData);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bRecommMinSpecificGPSTransitData);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bGPSStatelliteInView);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bVehicleSpeedData);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bVehicleGyroData);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bVehicleAccelData);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bVehicleHeadingData);

    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtStartLocationInformation;
    mEventHeader->dataSize = byteNum;

    LOGD("    evt[%u] dataSize[%u]", pIpcData[0], pIpcData[2]);

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


int Location::handleStopLocationInformationEvent(LinkController* pLnkCtrler, int8_t session)
{
    int result = kIap2NoErr;

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

//    iAp2MarshalPutUint32(&iAp2Marshal, 0);
    iAp2MarshalPutUint32(&iAp2Marshal, dummyValue);
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    mEventHeader->evt = kEvtStopLocationInformation;
    mEventHeader->dataSize = byteNum;

    LOGD("    evt[%u] dataSize[%u]", pIpcData[0], pIpcData[2]);

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


