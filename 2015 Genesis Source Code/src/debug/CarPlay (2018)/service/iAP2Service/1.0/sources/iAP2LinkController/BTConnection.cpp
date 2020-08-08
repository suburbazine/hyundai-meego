/*
    File:       BTConnection.cpp
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

#include "BTConnection.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>

namespace IAP2
{

BTConnection::BTConnection()
{
    LOGD("BTConnection()");
}

BTConnection::~BTConnection()
{
    LOGD("~BTConnection()");
}


void BTConnection::handleBTConnectionUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    BTConnectionUpdateEvt event;

    LOGD("handleBTConnectionUpdate() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeBTConnectionUpdate(pData, &event);
    
    result = handleBTConnectionUpdateEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleBTConnectionUpdate(). result[%d]", result);
    
}

void BTConnection::decodeBTConnectionUpdate(uint8_t* pData, BTConnectionUpdateEvt* pEvt)
{
    int msgLen;

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pEvt == NULL) {
        LOGE("    Error : pEvt is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    msgLen = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);

    LOGD("decodeBTConnectionUpdate() nMsgLen[%d]", msgLen);

    int paramStartIdx = 6;
    int paramId; //, paramLength;


    while (msgLen > paramStartIdx) {
//        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
        paramStartIdx += 4;

        switch (paramId) {
            case kParamIdTransportComponentID : {
                pEvt->BTTransPortComponentID = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
                paramStartIdx ++;
                break;
            }

            case kParamIdConnectedBTProfiles : {
                pEvt->mConnectedBTProfiles.bPresent = TRUE;
                decordconnectedBTProfiles(pData, paramStartIdx, pEvt);
                paramStartIdx++;            
                break;
            }

        }

    }    
}

int BTConnection::handleBTConnectionUpdateEvent(LinkController* pLnkCtrler, int8_t session, BTConnectionUpdateEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleBTConnectionUpdateEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = sizeof(EventHeader) + sizeof(BTConnectionUpdateEvt);
    mallocSize = mallocSize * 4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* mEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    
    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->BTTransPortComponentID);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.bPresent);
    if (pEvent->mConnectedBTProfiles.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTHandsFree.bPresent);
        if (pEvent->mConnectedBTProfiles.BTHandsFree.bPresent == TRUE) {
            iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTHandsFree.value);
        }
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTPhoneAddressAccess.bPresent);
        if (pEvent->mConnectedBTProfiles.BTPhoneAddressAccess.bPresent == TRUE) {
            iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTPhoneAddressAccess.value);
        }
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTAudioVideoRemoteControl.bPresent);
        if (pEvent->mConnectedBTProfiles.BTAudioVideoRemoteControl.bPresent == TRUE) {
            iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTAudioVideoRemoteControl.value);
        }
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTAdvanceAudioDistribution.bPresent);
        if (pEvent->mConnectedBTProfiles.BTAdvanceAudioDistribution.bPresent == TRUE) {
            iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTAdvanceAudioDistribution.value);
        }
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTHumanInterfaceDevice.bPresent);
        if (pEvent->mConnectedBTProfiles.BTHumanInterfaceDevice.bPresent == TRUE) {
            iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTHumanInterfaceDevice.value);
        }
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTiAP2Link.bPresent);
        if (pEvent->mConnectedBTProfiles.BTiAP2Link.bPresent == TRUE) {
            iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTiAP2Link.value);
        }
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTPANAP.bPresent);
        if (pEvent->mConnectedBTProfiles.BTPANAP.bPresent == TRUE) {
            iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTPANAP.value);
        }
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTMessageAccess.bPresent);
        if (pEvent->mConnectedBTProfiles.BTMessageAccess.bPresent == TRUE) {
            iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTMessageAccess.value);
        }
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTPANC.bPresent);
        if (pEvent->mConnectedBTProfiles.BTPANC.bPresent == TRUE) {
            iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mConnectedBTProfiles.BTPANC.value);
        }
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    mEventHeader->evt = kEvtBTConnectionUpdate;
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

void BTConnection::decordconnectedBTProfiles(uint8_t* pData, int nParamStartIdx, BTConnectionUpdateEvt* pEvt)
{
     LOGD("decordconnectedBTProfiles()");

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    if (pEvt == NULL) {
        LOGE("    Error : pEvt is NULL");
        return;
    }

    int nParamLength;
    nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);

    int nSubParamId, nSubParamLength;
    int nSubParamStartIdx = nParamStartIdx+4; 
    
    while(nParamLength > (nSubParamStartIdx - nParamStartIdx)) {
        nSubParamLength = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx, 2);
        nSubParamId = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 2, 2);
        switch (nSubParamId)    {
            case kParamBTHansFree : {
                pEvt->mConnectedBTProfiles.BTHandsFree.bPresent = TRUE;
                pEvt->mConnectedBTProfiles.BTHandsFree.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);                
                LOGD("pEvt->mConnectedBTProfiles.BTHandsFree.value : %u",pEvt->mConnectedBTProfiles.BTHandsFree.value);
                break;    
            }
            case kParamBTPhoneBookAccess : {
                pEvt->mConnectedBTProfiles.BTPhoneAddressAccess.bPresent = TRUE;
                pEvt->mConnectedBTProfiles.BTPhoneAddressAccess.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);                
                LOGD("pEvt->mConnectedBTProfiles.BTPhoneAddressAccess.value : %u",pEvt->mConnectedBTProfiles.BTPhoneAddressAccess.value);
                break;    
            }
            case kParamBTAudioVideoRemoteControl : {
                pEvt->mConnectedBTProfiles.BTAudioVideoRemoteControl.bPresent = TRUE;
                pEvt->mConnectedBTProfiles.BTAudioVideoRemoteControl.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);                
                LOGD("pEvt->mConnectedBTProfiles.BTAudioVideoRemoteControl.value : %u",pEvt->mConnectedBTProfiles.BTAudioVideoRemoteControl.value);
                break;    
            }
            case kParamBTAdvanceAudioDistribution : {
                pEvt->mConnectedBTProfiles.BTAdvanceAudioDistribution.bPresent = TRUE;
                pEvt->mConnectedBTProfiles.BTAdvanceAudioDistribution.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);                
                LOGD("pEvt->mConnectedBTProfiles.BTAdvanceAudioDistribution.value : %u",pEvt->mConnectedBTProfiles.BTAdvanceAudioDistribution.value);
                break;    
            }
            case kParamBTHumanInterfaceDevice : {
                pEvt->mConnectedBTProfiles.BTHumanInterfaceDevice.bPresent = TRUE;
                pEvt->mConnectedBTProfiles.BTHumanInterfaceDevice.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);                
                LOGD("pEvt->mConnectedBTProfiles.BTHumanInterfaceDevice.value : %u",pEvt->mConnectedBTProfiles.BTHumanInterfaceDevice.value);
                break;    
            }
            case kParamBTiAP2Link : {
                pEvt->mConnectedBTProfiles.BTiAP2Link.bPresent = TRUE;
                pEvt->mConnectedBTProfiles.BTiAP2Link.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);                
                LOGD("pEvt->mConnectedBTProfiles.BTiAP2Link.value : %u",pEvt->mConnectedBTProfiles.BTiAP2Link.value);
                break;    
            }
            case kParamBTPANAP : {
                pEvt->mConnectedBTProfiles.BTPANAP.bPresent = TRUE;
                pEvt->mConnectedBTProfiles.BTPANAP.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);                
                LOGD("pEvt->mConnectedBTProfiles.BTPANAP.value : %u",pEvt->mConnectedBTProfiles.BTPANAP.value);
                break;    
            }
            case kParamMessageAccess : {
                pEvt->mConnectedBTProfiles.BTMessageAccess.bPresent = TRUE;
                pEvt->mConnectedBTProfiles.BTMessageAccess.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);                
                LOGD("pEvt->mConnectedBTProfiles.BTMessageAccess.value : %u",pEvt->mConnectedBTProfiles.BTMessageAccess.value);
                break;    
            }
            case kParamBTPANC : {
                pEvt->mConnectedBTProfiles.BTPANC.bPresent = TRUE;
                pEvt->mConnectedBTProfiles.BTPANC.value = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);                
                LOGD("pEvt->mConnectedBTProfiles.BTPANC.value : %u",pEvt->mConnectedBTProfiles.BTPANC.value);
                break;    
            }
            default:
                LOGD("    WARNING : unknowd nSubParamStartIdx[0x%x]", nSubParamStartIdx);
                break;        
        
            }
            nSubParamStartIdx = nSubParamStartIdx + nSubParamLength;
            
    }
}
} // namespace IAP2

