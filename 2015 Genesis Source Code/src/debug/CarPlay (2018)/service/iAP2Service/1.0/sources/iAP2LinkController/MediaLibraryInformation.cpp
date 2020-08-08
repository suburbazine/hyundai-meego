/*
    File:       MediaLibraryInformation.cpp
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

#include "MediaLibraryInformation.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

MediaLibraryInformation::MediaLibraryInformation()
{
    LOGD("MediaLibraryInformation()");
}

MediaLibraryInformation::~MediaLibraryInformation()
{
    LOGD("~MediaLibraryInformation()");
}


void MediaLibraryInformation::handleMediaLibraryInformation(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    LOGD("handleMediaLibraryInformation()");

    int result = kIap2NoErr;
    MediaLibraryInformationEvt event;

    LOGD("handleMediaLibraryInformation() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeMediaLibraryInformation(pData, &event);

    result = handleMeidaLibraryInformationEvent(pLnkCtrler, session, &event);
    if (result != kIap2NoErr)
        LOGE("    ERROR : handleMediaLibraryInformation(). result[%d]", result);

    //LOGE("### free event.pMediaLibraryInformationGr : %p ### ", event.pMediaLibraryInformationGr);
    free(event.pMediaLibraryInformationGr);
    
}



void MediaLibraryInformation::decodeMediaLibraryInformation(uint8_t* pData, MediaLibraryInformationEvt* pEvt)
{
    LOGD("decodeMediaLibraryInformation()");

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    if (pEvt == NULL) {
        LOGE("    Error : pEvt is NULL");
        return;
    }

    int nMsgLength;
    nMsgLength = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);    

    int nParamId, nParamLength;
    int nParamStartIdx = 6;
    int nSubParamId, nSubParamLength;
    int nParamCount = 0, nSubGroupParamLength = 6, nCount = 0;

    //LOGD("nMsgLength : %u ", nMsgLength);
    //LOGD("nParamLength : %u ", nParamLength);
    //LOGD("nSubGroupParamLength : %u ", nSubGroupParamLength);
    while(nSubGroupParamLength < nMsgLength){
        nParamCount++;
        nSubGroupParamLength += DataDecoder::getUintFromBytes(pData, nSubGroupParamLength, 2);
        //LOGD("nSubGroupParamLength : %u ", nSubGroupParamLength);
        
    }
    LOGD("nNumSubGroupParam : %u ", nParamCount);
    pEvt->nCountGroupParam = nParamCount;
    nCount = nParamCount;

    pEvt->pMediaLibraryInformationGr = (MediaLibraryInformationGroup*) malloc( sizeof(MediaLibraryInformationGroup)* nParamCount );
    memset(pEvt->pMediaLibraryInformationGr, 0x00, sizeof(MediaLibraryInformationGroup) *  nParamCount);
    
    nParamId = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 2, 2);

    if (nParamId == kParamIDMediaLibraryInformation)
    {
        //LOGD("nParamLength[%d] nParamId[0x%x] nParamStartIdx[%d] nSubParamStartIdx[%d]", nParamLength, nParamId, nParamStartIdx, nSubParamStartIdx);
        MediaLibraryInformationGroup* pMediaLibraryInformationGroup = (MediaLibraryInformationGroup*)pEvt->pMediaLibraryInformationGr;
        while(nCount){
            nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);
            
            int nSubParamStartIdx = nParamStartIdx + 4;        
            int nSubParamNum = 0;
            while(nParamLength != (nSubParamStartIdx - nParamStartIdx)) {
                nSubParamLength = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx, 2);
                nSubParamId = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+2, 2);

                //LOGD("nSubParamLength[%d], nSubParamId[0x%x] nParamStartIdx[%d] nSubParamStartIdx[%d]", nSubParamLength, nSubParamId, nParamStartIdx, nSubParamStartIdx);                    
                switch(nSubParamId) {
                    case kParamIDMediaLibraryName: {
                        pMediaLibraryInformationGroup->mMediaLibraryName.size = nSubParamLength-4;
                        pMediaLibraryInformationGroup->mMediaLibraryName.value = (uint8_t*)&pData[nSubParamStartIdx+4];    
                        //LOGD("decodeMediaLibraryInformation pMeidaLibraryInformationGroup->mMediaLibraryName.value : %s",  pMeidaLibraryInformationGroup->mMediaLibraryName.value);    
                        //LOGD("decodeMediaLibraryInformation pMeidaLibraryInformationGroup->mMediaLibraryName.size : %u",  pMeidaLibraryInformationGroup->mMediaLibraryName.size);
                        break;
                    }
                                            
                    case kParamIDMediaLibraryUniqueIdentifier_Info: {
                        pMediaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.size = nSubParamLength-4;
                        pMediaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.value = (uint8_t*)&pData[nSubParamStartIdx+4];
                        //LOGD("decodeMediaLibraryInformation pMeidaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.value : %s",  pMeidaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.value);    
                        //LOGD("decodeMediaLibraryInformation pMeidaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.size : %u",  pMeidaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.size);
                        break;
                    }

                    case kParamIDMediaLibraryType: {                        
                        pMediaLibraryInformationGroup->mMediaLibraryType.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx+4, 1);
                        //LOGD("decodeMediaLibraryInformation pMeidaLibraryInformationGroup->mMediaLibraryType.value : %u",  pMeidaLibraryInformationGroup->mMediaLibraryType.value);    
                    }
                    
                    default:
                        LOGD("    WARNING : unknowd nSubParamId[0x%x]", nSubParamId);
                        break;
                }
                nSubParamStartIdx += nSubParamLength;
                nSubParamNum++;
            }
            pMediaLibraryInformationGroup = (MediaLibraryInformationGroup*)(pMediaLibraryInformationGroup + 1);
            nParamStartIdx += nParamLength;
            nCount--;
        }
    }
    else
    {
        LOGD("    WARNING : default break nParamId: 0x%x", nParamId);
    }
}



int MediaLibraryInformation::handleMeidaLibraryInformationEvent(LinkController* pLnkCtrler, int8_t session, MediaLibraryInformationEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleMeidaLibraryInformationEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0, nCountGroup = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(sizeof(EventHeader) + sizeof(uint32_t)*100);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, sizeof(EventHeader) + sizeof(uint32_t)*100, sizeof(EventHeader));

    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->nCountGroupParam);

    if (0 < pEvent->nCountGroupParam )
    {
        MediaLibraryInformationGroup* pMediaLibraryInformationGroup = (MediaLibraryInformationGroup*)pEvent->pMediaLibraryInformationGr;
        nCountGroup = pEvent->nCountGroupParam;
        //iAp2MarshalPutUint32(&iAp2Marshal, pEvent->nCountGroupParam);
        //LOGD("nCountGroup %u", nCountGroup);            
        while(nCountGroup) 
        {    
            //LOGD("handleMeidaLibraryInformationEvent pMeidaLibraryInformationGroup->mMediaLibraryName.value : %s",  pMeidaLibraryInformationGroup->mMediaLibraryName.value);    
            //LOGD("handleMeidaLibraryInformationEvent pMeidaLibraryInformationGroup->mMediaLibraryName.size : %u",  pMeidaLibraryInformationGroup->mMediaLibraryName.size);    
            iAp2MarshalPutBytes(&iAp2Marshal, pMediaLibraryInformationGroup->mMediaLibraryName.value, pMediaLibraryInformationGroup->mMediaLibraryName.size);
            //LOGD("handleMeidaLibraryInformationEvent pMeidaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.value : %s",  pMeidaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.value);    
            //LOGD("handleMeidaLibraryInformationEvent pMeidaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.size : %u",  pMeidaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.size);
            iAp2MarshalPutBytes(&iAp2Marshal, pMediaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.value, pMediaLibraryInformationGroup->mMediaLibraryUniqueIdentifier.size);
            //LOGD("handleMeidaLibraryInformationEvent pMeidaLibraryInformationGroup->mMediaLibraryType.value : %u",  pMeidaLibraryInformationGroup->mMediaLibraryType.value);    
            iAp2MarshalPutUint32(&iAp2Marshal, pMediaLibraryInformationGroup->mMediaLibraryType.value);
            nCountGroup--;
            pMediaLibraryInformationGroup = (MediaLibraryInformationGroup*)(pMediaLibraryInformationGroup + 1);
        }
        
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    mEventHeader->evt = kEvtMediaLibraryInformation;
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
