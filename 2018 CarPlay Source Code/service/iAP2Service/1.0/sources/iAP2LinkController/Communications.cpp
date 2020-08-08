/*
    File:       Communications.cpp
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

#include "Communications.h"
#include "iAp2ServiceLog.h"
#include "LinkController.h"
#include "SendEventToApp.h"
#include <stdlib.h>


namespace IAP2
{

Communications::Communications()
{
    LOGD("Communications()");
    bLegacy = FALSE;
}

Communications::~Communications()
{
    LOGD("~Communications()");
}




void Communications::handleCallStateUpdates(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    CallStateUpdatesEvt event;

    LOGD("handleCallStateUpdates() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeCallStateUpdate(pData, &event);
    
    result = handleCallStateUpdatesEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleTelephonyUpdate(). result[%d]", result);
    
}



void Communications::handleCommunicationsUpdates(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    CommunicationsUpdatesEvt event;

    LOGD("handleCommunicationsUpdates() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeCommunicationsUpdate(pData, &event);
    
    result = handleCommunicationsUpdatesEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleTelephonyUpdate(). result[%d]", result);
    
}

void Communications::handleListUpdate(LinkController* pLnkCtrler, int8_t session, uint8_t* pData)
{
    int result = kIap2NoErr;
    ListUpdateEvt event;

    LOGD("handleListUpdate() session[%d]", session);
    if (pLnkCtrler == NULL) {
        LOGE("    Error : pLnkCtrler is NULL");
        return;
    }

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    memset(&event, 0x00, sizeof(event));
    decodeListUpdate(pData, &event);
    
    result = handleListUpdateEvent(pLnkCtrler, session, &event);

    if (result != kIap2NoErr)
        LOGE("    ERROR : handleListUpdateEvent(). result[%d]", result);
    
}


void Communications::decodeCallStateUpdate(uint8_t* pData, CallStateUpdatesEvt* pEvt)
{
    int msgLen;

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    msgLen = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);

    int paramStartIdx = 6;
    int paramDataStartIdx = 0;
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
        paramDataStartIdx = paramStartIdx + 4;
        switch (paramId) {
            case kParamIdStartCallStateUpdates_RemoteID : {
                pEvt->mRemoteID.bPresent = TRUE;
                pEvt->mRemoteID.size = paramLength-4;
                pEvt->mRemoteID.value = (uint8_t*)&pData[paramDataStartIdx];                
                LOGD("mRemoteID [%s]", pEvt->mRemoteID.value);                
                break;
            }

            case kParamIdStartCallStateUpdates_DisplayName : {
                pEvt->mDisplayName.bPresent = TRUE;
                pEvt->mDisplayName.size = paramLength-4;
                pEvt->mDisplayName.value = (uint8_t*)&pData[paramDataStartIdx];                
                LOGD("mDisplayName [%s]", pEvt->mDisplayName.value);                
                break;
            }

            case kParamIdStartCallStateUpdates_Status : {
                pEvt->eStatus = (CallStateUpdateStatus)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("eStatus [%u]", pEvt->eStatus);                
                break;
            }

            case kParamIdStartCallStateUpdates_Direction : {
                pEvt->eDirection = (CallStateUpdateDirection)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("eDirection [%u]", pEvt->eDirection);                
                break;
            }

            case kParamIdStartCallStateUpdates_CallUUID : {
                pEvt->mCallUUID.bPresent = TRUE;
                pEvt->mCallUUID.size = paramLength-4;
                pEvt->mCallUUID.value = (uint8_t*)&pData[paramDataStartIdx];                
                LOGD("mCallUUID [%s]", pEvt->mCallUUID.value);                
                break;
            }

            case kParamIdStartCallStateUpdates_AddressBookID : {
                pEvt->mAddressBookID.bPresent = TRUE;
                pEvt->mAddressBookID.size = paramLength-4;
                pEvt->mAddressBookID.value = (uint8_t*)&pData[paramDataStartIdx];                
                LOGD("mAddressBookID [%s]", pEvt->mAddressBookID.value);                
                break;
            }

            case kParamIdStartCallStateUpdates_Label : {
                pEvt->mLabel.bPresent = TRUE;
                pEvt->mLabel.size = paramLength-4;
                pEvt->mLabel.value = (uint8_t*)&pData[paramDataStartIdx];                
                LOGD("mLabel [%s]", pEvt->mLabel.value);                
                break;
            }

            case kParamIdStartCallStateUpdates_Service : {
                pEvt->eService = (CallStateUpdateService)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("eService [%u]", pEvt->eService);                
                break;
            }

            case kParamIdStartCallStateUpdates_IsConferenced : {
                pEvt->bIsConferenced = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("IsConferenced [%u]", pEvt->bIsConferenced);                
                break;
            }

            case kParamIdStartCallStateUpdates_ConferenceGroup : {
                pEvt->nConferenceGroup = (uint8_t)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("nConferenceGroup [%u]", pEvt->nConferenceGroup);                
                break;
            }

            case kParamIdStartCallStateUpdates_DisconnectReason : {
                pEvt->mDisconnectReason.bPresent = TRUE;
                pEvt->mDisconnectReason.value = (CallStateUpdateDisconnectReason)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("eDisconnectReason [%u]", pEvt->mDisconnectReason.value);                
                break;
            }
            
            default : 
                LOGE("Unknown ParamID!! [%u]", paramId);    
                break;
        }
        paramStartIdx = paramStartIdx + paramLength;
    }
}



void Communications::decodeCommunicationsUpdate(uint8_t* pData, CommunicationsUpdatesEvt* pEvt)
{
    int msgLen;

    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    msgLen = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);
    
    int paramStartIdx = 6;
    int paramDataStartIdx = 0;
    int paramId, paramLength;

    while (msgLen > paramStartIdx) {
        paramLength = DataDecoder::getUintFromBytes(pData, paramStartIdx, 2);
        paramId = DataDecoder::getUintFromBytes(pData, paramStartIdx + 2, 2);
        paramDataStartIdx = paramStartIdx + 4;
        switch (paramId) {
            case kParamIdStartCommunicationsUpdates_SignalStrength : {
                pEvt->mSignalStrength.bPresent = TRUE;
                pEvt->mSignalStrength.value = (SignalStrengthEnum)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);            
                LOGD("eSignalStrength [%u]", pEvt->mSignalStrength.value);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_RegistrationStatus : {
                pEvt->mRegistrationStatus.bPresent = TRUE;
                pEvt->mRegistrationStatus.value = (RegistrationStatusEnum)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);            
                LOGD("eRegistrationStatus [%u]", pEvt->mRegistrationStatus.value);                    
                break;
            }

            case kParamIdStartCommunicationsUpdates_AirplaneModeStatus : {
                pEvt->bAirplaneModeStatus = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bAirplaneModeStatus [%u]", pEvt->bAirplaneModeStatus);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_CarrierName : {
                pEvt->mCarrierName.bPresent = TRUE;
                pEvt->mCarrierName.size = paramLength-4;
                pEvt->mCarrierName.value = (uint8_t*)&pData[paramDataStartIdx];                
                LOGD("mCarrierName [%s]", pEvt->mCarrierName.value);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_CellularSupported : {
                pEvt->bCelluarSupported = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bCelluarSupported [%u]", pEvt->bCelluarSupported);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_TelephonyEnabled : {
                pEvt->bTelephonyEnabled = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bTelephonyEnabled [%u]", pEvt->bTelephonyEnabled);            
                break;
            }

            case kParamIdStartCommunicationsUpdates_FaceTimeAudioEnabled : {
                pEvt->bFaceTimeAudioEnabled = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bFaceTimeAudioEnabled [%u]", pEvt->bFaceTimeAudioEnabled);                    
                break;
            }

            case kParamIdStartCommunicationsUpdates_FaceTimeVideoEnabled : {
                pEvt->bFaceTimeVideoEnabled = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bFaceTimeVideoEnabled [%u]", pEvt->bFaceTimeVideoEnabled);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_MuteStatus : {
                pEvt->bMuteStatus = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bMuteStatus [%u]", pEvt->bMuteStatus);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_CurrentCallCount : {
                pEvt->mCurrentCallCount.bPresent = TRUE;
                pEvt->mCurrentCallCount.value = (uint8_t)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);            
                LOGD("mCurrentCallCount.value [%d]", pEvt->mCurrentCallCount.value);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_NewVoicemailCount : {
                pEvt->mNewVoicemailCount.bPresent = TRUE;
                pEvt->mNewVoicemailCount.value = (uint8_t)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);            
                LOGD("mNewVoicemailCount.value [%d]", pEvt->mNewVoicemailCount.value);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_InitiateCallAvailable : {
                pEvt->bInitiateCallAvailable = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bInitiateCallAvailable [%u]", pEvt->bInitiateCallAvailable);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_EndAndAcceptAvailable : {
                pEvt->bEndAndAcceptAvailable = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bEndAndAcceptAvailable [%u]", pEvt->bEndAndAcceptAvailable);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_HoldAndAcceptAvailable : {
                pEvt->bHoldAndAccptAvailable = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bHoldAndAccptAvailalbe [%u]", pEvt->bHoldAndAccptAvailable);            
                break;
            }

            case kParamIdStartCommunicationsUpdates_SwapAvailable : {
                pEvt->bSwapAvailable = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bSwapAvailable [%u]", pEvt->bSwapAvailable);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_MergeAvailable : {
                pEvt->bMergeAvailable = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bMergeAvailable [%u]", pEvt->bMergeAvailable);                
                break;
            }

            case kParamIdStartCommunicationsUpdates_HoldAvailable : {
                pEvt->bHoldAvailable = (BOOL)DataDecoder::getUintFromBytes(pData, paramDataStartIdx, 1);                
                LOGD("bHoldAvailable [%u]", pEvt->bHoldAvailable);        
                break;
            }
            
            default : 
                LOGE("Unknown ParamID!! [%u]", paramId);    
                break;
        }
        paramStartIdx = paramStartIdx + paramLength;
    }
}



void Communications::decodeListUpdate(uint8_t* pData, ListUpdateEvt* pEvt)
{
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
    
    RecentListGroup* pRecentListGroup = NULL;
    FavoritesListGroup* pFavoritesListGroup = NULL;
    
    pEvt->nRecentListGroupCount = getGroupCount(pData,kParamIdListUpdates_RecentsList);
    LOGD("pEvt->nRecentListGroupCount : %u", pEvt->nRecentListGroupCount);
    
    if(pEvt->nRecentListGroupCount > 0)
    {        
        pEvt->pRecentListGroup = (RecentListGroup*)malloc(sizeof(RecentListGroup) * pEvt->mRecentListCount.value);
        memset(pEvt->pRecentListGroup, 0x00, sizeof(RecentListGroup) *  pEvt->nRecentListGroupCount);
        pRecentListGroup = (RecentListGroup*)pEvt->pRecentListGroup;
        pEvt->nRecentListSize = sizeof(RecentListGroup) * pEvt->nRecentListGroupCount;
        LOGD("pEvt->nRecentListSize : %u", pEvt->nRecentListSize);
    }

    pEvt->nFavoritesListGroupCount = getGroupCount(pData,kParamIdListUpdates_FavoritesList);
    LOGD("pEvt->nFavoritesListGroupCount : %u", pEvt->nFavoritesListGroupCount);
    
    if(pEvt->nFavoritesListGroupCount > 0)
    {        
        pEvt->pFavoritesListGroup = (FavoritesListGroup*)malloc(sizeof(FavoritesListGroup) * pEvt->mFavoritesListCount.value);
        memset(pEvt->pFavoritesListGroup, 0x00, sizeof(FavoritesListGroup) *  pEvt->nFavoritesListGroupCount);
        pFavoritesListGroup = (FavoritesListGroup*)pEvt->pFavoritesListGroup;
        pEvt->nFavoritesListSize = sizeof(FavoritesListGroup) * pEvt->nFavoritesListGroupCount;
        LOGD("pEvt->nFavoritesListSize : %u", pEvt->nFavoritesListSize);
    }
    
    while(nMsgLength > nParamStartIdx) {
        nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);
        nParamId = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 2, 2);

        switch (nParamId)    {
            case kParamIdListUpdates_RecentsListAvailable : {
                pEvt->mRectsListAavailable.bPresent= TRUE;
                pEvt->mRectsListAavailable.value = (BOOL)DataDecoder::getUintFromBytes(pData, nParamStartIdx + 4, 1);
                LOGD("pEvt->mRectsListAavailable.value : %u", pEvt->mRectsListAavailable.value);
                break;    
            }
            case kParamIdListUpdates_RecentsList : {
                decordListUpdatesRecentsList(pData, nParamStartIdx, pRecentListGroup);                
                pRecentListGroup = pRecentListGroup + 1;
                break;    
            }
            case kParamIdListUpdates_RecentsListCount : {
                pEvt->mRecentListCount.bPresent= TRUE;
                pEvt->mRecentListCount.value = (uint16_t)DataDecoder::getUintFromBytes(pData, nParamStartIdx + 4, 2);
                LOGD("pEvt->mRecentListCount.value : %u", pEvt->mRecentListCount.value);
                break;    
            }
            case kParamIdListUpdates_FavoritesListAvailabble : {
                 pEvt->mFavoritesListAavailable.bPresent= TRUE;
                pEvt->mFavoritesListAavailable.value = (BOOL)DataDecoder::getUintFromBytes(pData, nParamStartIdx + 4, 1);
                LOGD("pEvt->mFavoritesListAavailable.value : %u", pEvt->mFavoritesListAavailable.value);
                break;    
            }
            case kParamIdListUpdates_FavoritesList : {
                decordListUpdatesFavoritesList(pData, nParamStartIdx, pFavoritesListGroup);                
                pFavoritesListGroup = pFavoritesListGroup + 1;
                break;    
            }
            case kParamIdListUpdates_FavoritesListCount : {
                pEvt->mFavoritesListCount.bPresent= TRUE;
                pEvt->mFavoritesListCount.value = (uint16_t)DataDecoder::getUintFromBytes(pData, nParamStartIdx + 4, 2);
                LOGD("pEvt->mFavoritesListCount.value : %u", pEvt->mFavoritesListCount.value);
                break;    
            }
                
            default:
                LOGD("    WARNING : unknowd nParamId[0x%x]", nParamId);
                break;        
        
            }
            nParamStartIdx = nParamStartIdx + nParamLength;    
        }
}


void Communications::decordListUpdatesRecentsList(uint8_t* pData, int nParamStartIdx, RecentListGroup* pRecentListGroup)
{
    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    if (pRecentListGroup == NULL) {
        LOGE("    Error : pRecentListGroup is NULL");
        return;
    }

    int nParamLength;
    nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);

    int nSubParamId, nSubParamLength;
    int nSubParamStartIdx = nParamStartIdx+4;
    

    pRecentListGroup->bPresent = TRUE;

    while(nParamLength > (nSubParamStartIdx - nParamStartIdx)) {
        nSubParamLength = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx, 2);
        nSubParamId = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 2, 2);
        switch (nSubParamId)    {
            case kParamIdListUpdateRecentListGroup_Index : {
                pRecentListGroup->nIndex = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 2);
                LOGD("pRecentListGroup->nIndex: %u",pRecentListGroup->nIndex);
                break;    
            }
            case kParamIdListUpdateRecentListGroup_RemoteID : {
                pRecentListGroup->mRemoteID.bPresent =  TRUE;
                pRecentListGroup->mRemoteID.size = nSubParamLength-4;
                pRecentListGroup->mRemoteID.value = (uint8_t*)&pData[nSubParamStartIdx+4];            
                                
                LOGD("pRecentListGroup->mRemoteID.value: %s",pRecentListGroup->mRemoteID.value);
                break;    
            }

            case kParamIdListUpdateRecentListGroup_DisplayName: {
                pRecentListGroup->mDisplayName.bPresent =  TRUE;
                pRecentListGroup->mDisplayName.size = nSubParamLength-4;
                pRecentListGroup->mDisplayName.value = (uint8_t*)&pData[nSubParamStartIdx+4];            
                                
                LOGD("pRecentListGroup->mDisplayName.value: %s",pRecentListGroup->mDisplayName.value);
                break;    
            }    

            case kParamIdListUpdateRecentListGroup_Label : {
                pRecentListGroup->mLabel.bPresent =  TRUE;
                pRecentListGroup->mLabel.size = nSubParamLength-4;
                pRecentListGroup->mLabel.value = (uint8_t*)&pData[nSubParamStartIdx+4];            
                                
                LOGD("pRecentListGroup->mLabel.value: %s",pRecentListGroup->mLabel.value);
                break;    
            }

            case kParamIdListUpdateRecentListGroup_AddressBookID: {
                pRecentListGroup->mAddressBookID.bPresent =  TRUE;
                pRecentListGroup->mAddressBookID.size = nSubParamLength-4;
                pRecentListGroup->mAddressBookID.value = (uint8_t*)&pData[nSubParamStartIdx+4];            
                                
                LOGD("pRecentListGroup->mAddressBookID.value: %s",pRecentListGroup->mAddressBookID.value);
                break;    
            }

            case kParamIdListUpdateRecentListGroup_Service: {
                pRecentListGroup->eServcie = (ListUpdateServiceEnum)DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);        
                                
                LOGD("RecentListGroup->eServcie: %u",pRecentListGroup->eServcie);
                break;    
            }

            case kParamIdListUpdateRecentListGroup_Type: {
                pRecentListGroup->eType= (ListUpdateTypeEnum)DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);        
                                
                LOGD("pRecentListGroup->eType: %u",pRecentListGroup->eType);
                break;    
            }

            case kParamIdListUpdateRecentListGroup_UnixTimestamp: {
                pRecentListGroup->mUnixTimestamp.bPresent = TRUE;
                pRecentListGroup->mUnixTimestamp.value = (uint64_t)DataDecoder::getUint64FromBytes(pData, nSubParamStartIdx + 4, 8);        
                                
                LOGD("pRecentListGroup->mUnixTimestamp.value: %llu", pRecentListGroup->mUnixTimestamp.value);
                break;    
            }
            
            case kParamIdListUpdateRecentListGroup_Duration: {
                pRecentListGroup->mDuration.bPresent = TRUE;
                pRecentListGroup->mDuration.value = (uint32_t)DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 4);        
                                
                LOGD("pRecentListGroup->mDuration.value: %u", pRecentListGroup->mDuration.value);
                break;    
            }

            case kParamIdListUpdateRecentListGroup_Occurrences: {
                pRecentListGroup->nOccurrences = (uint8_t)DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);
                LOGD("pRecentListGroup->nOccurrences: %u", pRecentListGroup->nOccurrences);
                break;    
            }
            default:
                LOGD("    WARNING : unknowd nSubParamStartIdx[0x%x]", nSubParamStartIdx);
                break;        
        
            }
            nSubParamStartIdx = nSubParamStartIdx + nSubParamLength;
        }
}


void Communications::decordListUpdatesFavoritesList(uint8_t* pData, int nParamStartIdx, FavoritesListGroup* pFavoritesListGroup)
{
    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return;
    }

    if (pFavoritesListGroup == NULL) {
        LOGE("    Error : pMediaItemGroup is NULL");
        return;
    }

    int nParamLength;
    nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);

    int nSubParamId, nSubParamLength;
    int nSubParamStartIdx = nParamStartIdx+4;
    
    pFavoritesListGroup->bPresent = TRUE;
    
    while(nParamLength > (nSubParamStartIdx - nParamStartIdx)) {
        nSubParamLength = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx, 2);
        nSubParamId = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 2, 2);
        switch (nSubParamId)    {
            case kParamIdListUpdateFavoritesListGroup_Index : {
                pFavoritesListGroup->nIndex = DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 2);
                LOGD("pFavoritesListGroup->nIndex: %u",pFavoritesListGroup->nIndex);
                break;    
            }
            case kParamIdListUpdateFavoritesListGroup_RemoteID : {
                pFavoritesListGroup->mRemoteID.bPresent =  TRUE;
                pFavoritesListGroup->mRemoteID.size = nSubParamLength-4;
                pFavoritesListGroup->mRemoteID.value = (uint8_t*)&pData[nSubParamStartIdx+4];            
                                
                LOGD("pFavoritesListGroup->mRemoteID.value: %s",pFavoritesListGroup->mRemoteID.value);
                break;    
            }

            case kParamIdListUpdateFavoritesListGroup_DisplayName: {
                pFavoritesListGroup->mDisplayName.bPresent =  TRUE;
                pFavoritesListGroup->mDisplayName.size = nSubParamLength-4;
                pFavoritesListGroup->mDisplayName.value = (uint8_t*)&pData[nSubParamStartIdx+4];            
                                
                LOGD("pFavoritesListGroup->mDisplayName.value: %s",pFavoritesListGroup->mDisplayName.value);
                break;    
            }    

            case kParamIdListUpdateFavoritesListGroup_Label : {
                pFavoritesListGroup->mLabel.bPresent =  TRUE;
                pFavoritesListGroup->mLabel.size = nSubParamLength-4;
                pFavoritesListGroup->mLabel.value = (uint8_t*)&pData[nSubParamStartIdx+4];            
                                
                LOGD("pFavoritesListGroup->mLabel.value: %s",pFavoritesListGroup->mLabel.value);
                break;    
            }

            case kParamIdListUpdateFavoritesListGroup_AddressBookID: {
                pFavoritesListGroup->mAddressBookID.bPresent =  TRUE;
                pFavoritesListGroup->mAddressBookID.size = nSubParamLength-4;
                pFavoritesListGroup->mAddressBookID.value = (uint8_t*)&pData[nSubParamStartIdx+4];            
                                
                LOGD("pFavoritesListGroup->mAddressBookID.value: %s",pFavoritesListGroup->mAddressBookID.value);
                break;    
            }

            case kParamIdListUpdateFavoritesListGroup_Service: {
                pFavoritesListGroup->eServcie = (ListUpdateServiceEnum)DataDecoder::getUintFromBytes(pData, nSubParamStartIdx + 4, 1);        
                                
                LOGD("pFavoritesListGroup->eServcie: %u",pFavoritesListGroup->eServcie);
                break;    
            }

        
            default:
                LOGD("    WARNING : unknowd nSubParamStartIdx[0x%x]", nSubParamStartIdx);
                break;        
        
            }
            nSubParamStartIdx = nSubParamStartIdx + nSubParamLength;
        }
}

int Communications::getGroupCount(uint8_t* pData, int SearchID)
{    
    if (pData == NULL) {
        LOGE("    Error : pData is NULL");
        return 0;
    }

    int nMsgLength;
    nMsgLength = DataDecoder::getUintFromBytes(pData, kIndexControlMsgLenMsb, 2);
    
    int nParamId, nParamLength;
    int nParamStartIdx = 6;
    int count = 0;
    
    
    while(nMsgLength > nParamStartIdx) {
        nParamLength = DataDecoder::getUintFromBytes(pData, nParamStartIdx, 2);
        nParamId = DataDecoder::getUintFromBytes(pData, nParamStartIdx + 2, 2);
        if (nParamId == SearchID) {
            count++;
        }
        nParamStartIdx = nParamStartIdx + nParamLength;
    }
    LOGD("getGroupCount(): %u",count);
    return count;
}


int Communications::handleCallStateUpdatesEvent(LinkController* pLnkCtrler, int8_t session, CallStateUpdatesEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleCallStateUpdatesEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(CallStateUpdatesEvt) + pEvent->mRemoteID.size + pEvent->mDisplayName.size
                           + pEvent->mCallUUID.size + pEvent->mAddressBookID.size + pEvent->mLabel.size;
    mallocSize = mallocSize*4;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* pEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mRemoteID.bPresent);
    if (pEvent->mRemoteID.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->mRemoteID.value, pEvent->mRemoteID.size);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mDisplayName.bPresent);
    if (pEvent->mDisplayName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->mDisplayName.value, pEvent->mDisplayName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, bLegacy);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->eStatus);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->eDirection);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mCallUUID.bPresent);
    if (pEvent->mCallUUID.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->mCallUUID.value, pEvent->mCallUUID.size);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mAddressBookID.bPresent);
    if (pEvent->mAddressBookID.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->mAddressBookID.value, pEvent->mAddressBookID.size);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mLabel.bPresent);
    if (pEvent->mLabel.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->mLabel.value, pEvent->mLabel.size);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->eService);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bIsConferenced);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->nConferenceGroup);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mDisconnectReason.bPresent);
    if (pEvent->mDisconnectReason.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mDisconnectReason.value);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pEventHeader->evt = kEvtCallStateUpdate;
    pEventHeader->dataSize = byteNum;

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


int Communications::handleCommunicationsUpdatesEvent(LinkController* pLnkCtrler, int8_t session, CommunicationsUpdatesEvt* pEvent)
{
    int result = kIap2NoErr;

    LOGD("handleCallStateUpdatesEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    uint32_t mallocSize = 0;
    mallocSize = sizeof(EventHeader) + sizeof(CommunicationsUpdatesEvt) + pEvent->mCarrierName.size;
    mallocSize = mallocSize*4 + 100000;
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* pEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(EventHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mSignalStrength.bPresent);
    if (pEvent->mSignalStrength.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mSignalStrength.value);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mRegistrationStatus.bPresent);
    if (pEvent->mRegistrationStatus.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mRegistrationStatus.value);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bAirplaneModeStatus);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mCarrierName.bPresent);
    if (pEvent->mCarrierName.bPresent == TRUE)
        iAp2MarshalPutBytes(&iAp2Marshal, pEvent->mCarrierName.value, pEvent->mCarrierName.size);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bCelluarSupported);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bTelephonyEnabled);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bFaceTimeAudioEnabled);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bFaceTimeVideoEnabled);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bMuteStatus);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mCurrentCallCount.bPresent);
    if (pEvent->mCurrentCallCount.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mCurrentCallCount.value);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mNewVoicemailCount.bPresent);
    if (pEvent->mNewVoicemailCount.bPresent == TRUE)
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mNewVoicemailCount.value);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bInitiateCallAvailable);

	iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bEndAndAcceptAvailable);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bHoldAndAccptAvailable);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bSwapAvailable);

    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bMergeAvailable);
    
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->bHoldAvailable);
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);
    
    pEventHeader->evt = kEvtCommunicationsUpdate;
    pEventHeader->dataSize = byteNum;

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


int Communications::handleListUpdateEvent(LinkController* pLnkCtrler, int8_t session, ListUpdateEvt* pEvent)
{    
    uint32_t LISTUPDATE_MARSHALING_BUFFER  = 400000;
    int result = kIap2NoErr;
    uint32_t currentIndex = 0;
    LOGD("handleListUpdateEvent() session[%d]", session);

    if (pEvent == NULL) {
        LOGE("ERROR : pEvent is NULL");
        return kIap2ParamErr;
    }    
    
    uint32_t byteNum = 0;
    uint32_t* pIpcData = (uint32_t*) malloc(LISTUPDATE_MARSHALING_BUFFER);
    
    if (pIpcData == NULL) {
        LOGE("ERROR : pIpcData is NULL");
        return kIap2NoMemoryErr;
    }
    EventHeader* pEventHeader = (EventHeader*)pIpcData;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    iAp2MarshalInit(&iAp2Marshal, pIpcData, LISTUPDATE_MARSHALING_BUFFER, sizeof(EventHeader));

       //LOGD(" pEvent->mRectsListAavailable.bPresent %u", pEvent->mRectsListAavailable.bPresent);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mRectsListAavailable.bPresent);
    if(pEvent->mRectsListAavailable.bPresent == TRUE){
        //LOGD(" pEvent->mRectsListAavailable.value %u", pEvent->mRectsListAavailable.value);
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mRectsListAavailable.value);
        
    }

    //LOGD("pEvent->nRecentListSize %u", pEvent->nRecentListSize);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->nRecentListSize);

    //LOGD("pEvent->nRecentListGroupCount %u", pEvent->nRecentListGroupCount);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->nRecentListGroupCount);
        
    RecentListGroup* pRecentListGroup     = (RecentListGroup*) pEvent->pRecentListGroup;
    //LOGD("pEvent->pRecentListGroup %p", pEvent->pRecentListGroup);    
    if( 0 < pEvent->nRecentListSize)
    {        
        for(currentIndex = 0 ; currentIndex < pEvent->nRecentListGroupCount; currentIndex++)    
        {                        
            ParsingListUpdateEventRecentListGroup(&iAp2Marshal, pRecentListGroup);                
            pRecentListGroup = pRecentListGroup + 1;
        }        
        
    }        

    //LOGD("pEvent->mRecentListCount.bPresent %u", pEvent->mRecentListCount.bPresent); //RecentListCount maybe not present when receving message.
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mRecentListCount.bPresent);
    if ( pEvent->mRecentListCount.bPresent == TRUE) {
        //LOGD("pEvent->mRecentListCount.value %u", pEvent->mRecentListCount.value);
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mRecentListCount.value);
    }

    //LOGD("pEvent->mFavoritesListAavailable.bPresent %u", pEvent->mFavoritesListAavailable.bPresent);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mFavoritesListAavailable.bPresent);
    if(pEvent->mFavoritesListAavailable.bPresent == TRUE){
        //LOGD("pEvent->mFavoritesListAavailable.value %u", pEvent->mFavoritesListAavailable.value);
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mFavoritesListAavailable.value);
    }

    //LOGD("pEvent->nFavoritesListSize %u", pEvent->nFavoritesListSize);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->nFavoritesListSize);

    //LOGD("pEvent->nFavoritesListGroupCount %u", pEvent->nFavoritesListGroupCount);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->nFavoritesListGroupCount);  


    //LOGD(" pEvent->pFavoritesListGroup %p", pEvent->pFavoritesListGroup);
    FavoritesListGroup* pFavoritesListGroup     = (FavoritesListGroup*) pEvent->pFavoritesListGroup;
    if( 0 <  pEvent->nFavoritesListSize)
    {        
        for(currentIndex = 0 ; currentIndex < pEvent->nFavoritesListGroupCount; currentIndex++)    
        {                        
            ParsingListUpdateEventFavoritesListGroup(&iAp2Marshal, pFavoritesListGroup);                
            pFavoritesListGroup = pFavoritesListGroup + 1;
        }        
        
    }    
    //LOGD(" pEvent->mFavoritesListCount.bPresent %u", pEvent->mFavoritesListCount.bPresent);
    iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mFavoritesListCount.bPresent);  //FavoritesListCount maybe not present when receving message
    if (pEvent->mFavoritesListCount.bPresent == TRUE) {
        //LOGD(" pEvent->mFavoritesListCount.value %u", pEvent->mFavoritesListCount.value);
        iAp2MarshalPutUint32(&iAp2Marshal, pEvent->mFavoritesListCount.value);
    }
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pEventHeader->evt = kEvtListUpdateUpdate;
    pEventHeader->dataSize = byteNum;
    
    if (pLnkCtrler->mpSendEventToApp != NULL) {
        result = pLnkCtrler->mpSendEventToApp->sendEvent(session, pIpcData, byteNum);
    }
    else {
        LOGE("    ERROR : mpSessionServer is NULL");
    }

    if (result != kIap2NoErr) {
        LOGE("    ERROR : send event fail. result[%d]", result);
    }

    if( pEvent->pFavoritesListGroup != NULL) {
        free(pEvent->pFavoritesListGroup);
        //LOGD("pEvent->pFavoritesListGroup FREE!!");
    }
    
    if( pEvent->pRecentListGroup != NULL) {
        //LOGD("pEvent->pRecentListGroup FREE!!");
        free(pEvent->pRecentListGroup);
    }
    free(pIpcData);
    return result;
}


void Communications::ParsingListUpdateEventRecentListGroup(MarshalIo* iAp2Marshal, RecentListGroup* pRecentListGroup)
{
    
        iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->bPresent);        
        //LOGD("pRecentListGroup->bPresent :  %u", pRecentListGroup->bPresent);
        
        iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->nIndex);        
        //LOGD("pRecentListGroup->nIndex :  %u", pRecentListGroup->nIndex);

        iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->mRemoteID.bPresent);        
        //LOGD("pRecentListGroup->mRemoteID.bPresent :  %u", pRecentListGroup->mRemoteID.bPresent);        
        if (pRecentListGroup->mRemoteID.bPresent == TRUE) {
              iAp2MarshalPutBytes(iAp2Marshal, pRecentListGroup->mRemoteID.value, pRecentListGroup->mRemoteID.size);
            //LOGD("pRecentListGroup->mRemoteID.value : %s", pRecentListGroup->mRemoteID.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->mDisplayName.bPresent);        
        //LOGD("pRecentListGroup->mDisplayName.bPresent :  %u", pRecentListGroup->mDisplayName.bPresent);    
        if (pRecentListGroup->mDisplayName.bPresent == TRUE) {
              iAp2MarshalPutBytes(iAp2Marshal, pRecentListGroup->mDisplayName.value, pRecentListGroup->mDisplayName.size);
            //LOGD("pRecentListGroup->mDisplayName.value : %s", pRecentListGroup->mDisplayName.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->mLabel.bPresent);        
        //LOGD("pRecentListGroup->mLabel.bPresent :  %u", pRecentListGroup->mLabel.bPresent);
        if (pRecentListGroup->mLabel.bPresent == TRUE) {
              iAp2MarshalPutBytes(iAp2Marshal, pRecentListGroup->mLabel.value, pRecentListGroup->mLabel.size);
            //LOGD("pRecentListGroup->mLabel.value : %s", pRecentListGroup->mLabel.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->mAddressBookID.bPresent);        
        //LOGD("pRecentListGroup->mAddressBookID.bPresent :  %u", pRecentListGroup->mAddressBookID.bPresent);
        if (pRecentListGroup->mAddressBookID.bPresent == TRUE) {
              iAp2MarshalPutBytes(iAp2Marshal, pRecentListGroup->mAddressBookID.value, pRecentListGroup->mAddressBookID.size);
            //LOGD("pRecentListGroup->mAddressBookID.value : %s", pRecentListGroup->mAddressBookID.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->eServcie);        
        //LOGD("pRecentListGroup->eServcie :  %u", pRecentListGroup->eServcie);
        
        iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->eType);        
        //LOGD("pRecentListGroup->eType :  %u", pRecentListGroup->eType);

        iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->mUnixTimestamp.bPresent);        
        //LOGD("pRecentListGroup->mUnixTimestamp.bPresent :  %u", pRecentListGroup->mUnixTimestamp.bPresent);
        if (pRecentListGroup->mUnixTimestamp.bPresent== TRUE) {
            iAp2MarshalPutUint64(iAp2Marshal, pRecentListGroup->mUnixTimestamp.value);        
            //LOGD("pRecentListGroup->mUnixTimestamp.value :  %llu",pRecentListGroup->mUnixTimestamp.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->mDuration.bPresent);        
        //LOGD("pRecentListGroup->mDuration.bPresent :  %u", pRecentListGroup->mDuration.bPresent);
        if (pRecentListGroup->mDuration.bPresent== TRUE) {
            iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->mDuration.value);        
            //LOGD("pRecentListGroup->mDuration.value :  %u",pRecentListGroup->mDuration.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pRecentListGroup->nOccurrences);        
        //LOGD("pRecentListGroup->nOccurrences :  %u", pRecentListGroup->nOccurrences);
}


void Communications::ParsingListUpdateEventFavoritesListGroup(MarshalIo* iAp2Marshal, FavoritesListGroup* pFavoritesListGroup)
{

        iAp2MarshalPutUint32(iAp2Marshal, pFavoritesListGroup->bPresent);        
        //LOGD("pFavoritesListGroup->bPresent :  %u", pFavoritesListGroup->bPresent);
        
        iAp2MarshalPutUint32(iAp2Marshal, pFavoritesListGroup->nIndex);        
        //LOGD("pFavoritesListGroup->nIndex :  %u", pFavoritesListGroup->nIndex);

        iAp2MarshalPutUint32(iAp2Marshal, pFavoritesListGroup->mRemoteID.bPresent);        
        //LOGD("pFavoritesListGroup->mDuration.bPresent :  %u", pFavoritesListGroup->mRemoteID.bPresent);
        if (pFavoritesListGroup->mRemoteID.bPresent == TRUE) {
              iAp2MarshalPutBytes(iAp2Marshal, pFavoritesListGroup->mRemoteID.value, pFavoritesListGroup->mRemoteID.size);
        //    LOGD("pFavoritesListGroup->mRemoteID.value : %s", pFavoritesListGroup->mRemoteID.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pFavoritesListGroup->mDisplayName.bPresent);        
        //LOGD("pFavoritesListGroup->mDisplayName.bPresent :  %u", pFavoritesListGroup->mDisplayName.bPresent);
        if (pFavoritesListGroup->mDisplayName.bPresent == TRUE) {
              iAp2MarshalPutBytes(iAp2Marshal, pFavoritesListGroup->mDisplayName.value, pFavoritesListGroup->mDisplayName.size);
            //LOGD("pFavoritesListGroup->mDisplayName.value : %s", pFavoritesListGroup->mDisplayName.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pFavoritesListGroup->mLabel.bPresent);        
        //LOGD("pFavoritesListGroup->mLabel.bPresent :  %u", pFavoritesListGroup->mLabel.bPresent);
        if (pFavoritesListGroup->mLabel.bPresent == TRUE) {
              iAp2MarshalPutBytes(iAp2Marshal, pFavoritesListGroup->mLabel.value, pFavoritesListGroup->mLabel.size);
            //LOGD("pFavoritesListGroup->mLabel.value : %s", pFavoritesListGroup->mLabel.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pFavoritesListGroup->mAddressBookID.bPresent);        
        //LOGD("pFavoritesListGroup->mAddressBookID.bPresent :  %u", pFavoritesListGroup->mAddressBookID.bPresent);
        if (pFavoritesListGroup->mAddressBookID.bPresent == TRUE) {
              iAp2MarshalPutBytes(iAp2Marshal, pFavoritesListGroup->mAddressBookID.value, pFavoritesListGroup->mAddressBookID.size);
            //LOGD("pFavoritesListGroup->mAddressBookID.value : %s", pFavoritesListGroup->mAddressBookID.value);
        }

        iAp2MarshalPutUint32(iAp2Marshal, pFavoritesListGroup->eServcie);        
        //LOGD("pFavoritesListGroup->eServcie :  %u", pFavoritesListGroup->eServcie);        
}


} // namespace IAP2

