/*
    File:       SessionCommunications.cpp
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

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <fcntl.h>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "SessionCommunications.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "SessionDefine.h"

namespace IAP2
{

SessionCommunications::SessionCommunications()
{
    LOGD("SessionTelephony()");
}

SessionCommunications::~SessionCommunications()
{
    LOGD("~SessionTelephony()");
}

// Send to iOS
int SessionCommunications::handleCommunicationsCmd(uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler)
{
    LOGD("handleCommunicationsCmd() cmd[%u] handle[0x%x] pRxData[%p] pLnkCtrler[%p]", cmd, handle, pRxData, pLnkCtrler);
    int result = kIap2NoErr;

    if (pRxData == NULL) {
        LOGE("    ERROR : pRxData is NULL");
        return kIap2ParamErr;
    }

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR : pLnkCtrler is NULL");
        return kIap2ParamErr;
    }

    switch(cmd) {
        case kCmdStartCallStateUpdates : {
            StartCallStateUpdatesArg arg;
            memset(&arg, 0x00, sizeof(StartCallStateUpdatesArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.bAddressBookID = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bLabel = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bDisconnectReason = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

            sendStartCallStateUpdates(pLnkCtrler, &arg);
            break;
        }
        case kCmdStopCallStateUpdates : {
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            iAp2MarshalGetUint32(&iAp2Marshal);

            sendStopCallStateUpdates(pLnkCtrler);
            break;
        }
        case kCmdStartCommunicationsUpdates : {
            StartCommunicationsUpdatesArg arg;
            memset(&arg, 0x00, sizeof(StartCommunicationsUpdatesArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.bSignalStrength = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bRegistrationStatus = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bAirplaneModeStatus = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bCarrierName = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bCelluarSupported = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bTelephonyEnabled = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bFaceTimeAudioEnabled = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bFaceTimeVideoEnabled = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bMuteStatus = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bCurrentCallCount = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bNewVoicemailCount = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bInitiateCallAvailable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bEndAndAccptAvailable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bHoldAndAcceptAvailable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bSwapAvailable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            arg.bHoldAvailable = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);            

            sendStartCommunicationsUpdates(pLnkCtrler, &arg);
            break;
        }
        case kCmdStopCommunicationsUpdates : {
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            iAp2MarshalGetUint32(&iAp2Marshal);

            sendStopCommunicationsUpdates(pLnkCtrler);
            break;
        }
        case kCmdInitiateCall : {
            InitiateCallArg arg;
            memset(&arg, 0x00, sizeof(InitiateCallArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.eType = (InitiateCallType)iAp2MarshalGetUint32(&iAp2Marshal);

            arg.mDestinationID.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.mDestinationID.bPresent == TRUE) {
                arg.mDestinationID.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.mDestinationID.size);
            }    

            arg.mService.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.mService.bPresent == TRUE) {
                arg.mService.value = (InitiateCallService)iAp2MarshalGetUint32(&iAp2Marshal);
            }    

            arg.mAddressBookID.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.mAddressBookID.bPresent == TRUE) {
                arg.mAddressBookID.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.mAddressBookID.size);
            }
            
            sendInitialCall(pLnkCtrler, &arg);
            break;
        }
        case kCmdAcceptCall : {
            AcceptCallArg arg;
            memset(&arg, 0x00, sizeof(AcceptCallArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.eAcceptAction = (AcceptActionEnum)iAp2MarshalGetUint32(&iAp2Marshal);

            arg.mCallUUID.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.mCallUUID.bPresent == TRUE) {
                arg.mCallUUID.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.mCallUUID.size);
            }    
            
            sendAcceptCall(pLnkCtrler, &arg);
            break;
        }
        case kCmdEndCall : {
            EndCallArg arg;
            memset(&arg, 0x00, sizeof(EndCallArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.eEndAction = (EndActionEnum)iAp2MarshalGetUint32(&iAp2Marshal);

            arg.mCallUUID.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.mCallUUID.bPresent == TRUE) {
                arg.mCallUUID.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.mCallUUID.size);
            }    
            
            sendEndCall(pLnkCtrler, &arg);
            break;
        }
        case kCmdSwapCalls : {
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            iAp2MarshalGetUint32(&iAp2Marshal);

            sendSwapCalls(pLnkCtrler);
            break;
        }
        case kCmdMergeCalls : {            
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            iAp2MarshalGetUint32(&iAp2Marshal);

            sendMergeCalls(pLnkCtrler);
            break;
        }
        case kCmdHoldStatusUpdate : {
            HoldStatusUpdateArg arg;
            memset(&arg, 0x00, sizeof(HoldStatusUpdateArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.bHoldStatus = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);

            arg.mCallUUID.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.mCallUUID.bPresent == TRUE) {
                arg.mCallUUID.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.mCallUUID.size);
            }    
            
            sendHoldStatusUpdate(pLnkCtrler, &arg);
            
            break;
        }
        case kCmdMuteStatusUpdate : {            
            MuteStatusUpdateArg arg;
            memset(&arg, 0x00, sizeof(MuteStatusUpdateArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.bMuteStatus= (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            sendMuteStatusUpdate(pLnkCtrler, &arg);
            break;
        }        
        case kCmdSendDTMF : {
            SendDTMFArg arg;
            memset(&arg, 0x00, sizeof(SendDTMFArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.eTone= (SendDTMFToneEnum)iAp2MarshalGetUint32(&iAp2Marshal);

            arg.mCallUUID.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.mCallUUID.bPresent == TRUE) {
                arg.mCallUUID.value = iAp2MarshalGetBytes(&iAp2Marshal, &arg.mCallUUID.size);
            }    
            
            sendSendDTMF(pLnkCtrler, &arg);
            break;
        }

        case kCmdStartListUpdates : {
            StartListUpdatesArg arg;
            memset(&arg, 0x00, sizeof(StartListUpdatesArg));
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            arg.mRecenttsListPropoerties.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if(arg.mRecenttsListPropoerties.bPresent == TRUE)
            {
                arg.mRecenttsListPropoerties.bLabel = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
                arg.mRecenttsListPropoerties.bAdressBookID = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
                arg.mRecenttsListPropoerties.bUnixTimestamp = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
                arg.mRecenttsListPropoerties.bDuration = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            }

            arg.mRecentsListMax.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if(arg.mRecentsListMax.bPresent == TRUE)
            {
                arg.mRecentsListMax.value = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
            }

            arg.bRecentsListCombine = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            
            arg.mFavoritesListProerties.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if (arg.mFavoritesListProerties.bPresent == TRUE) {
                arg.mFavoritesListProerties.bLabel = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
                arg.mFavoritesListProerties.bAdressBookID = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            }    

            arg.mFavoritesListMax.bPresent = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
            if(arg.mFavoritesListMax.bPresent == TRUE)
            {
                arg.mFavoritesListMax.value = (uint16_t)iAp2MarshalGetUint32(&iAp2Marshal);
            }
            
            sendStartListUpdates(pLnkCtrler, &arg);
            break;
        }

        case kCmdStopListUpdates : {
            
            MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
            
            iAp2MarshalInitFromTxn(&iAp2Marshal, (CommandHeader*)pRxData);

            iAp2MarshalGetUint32(&iAp2Marshal);

            sendStopListUpdates(pLnkCtrler);
            break;
        }
        
        
        default :
            LOGE("    ERROR : unknown cmd[%d]", kIap2ParamErr);
            result = kIap2ParamErr;
            break;
    }
    
    return result;
}

void SessionCommunications::sendStartCallStateUpdates(LinkController* pLnkCtrler, StartCallStateUpdatesArg* pArg)
{
    LOGD("sendStartCallStateUpdates()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    createStartCallStateUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartCallStateUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}



void SessionCommunications::sendStopCallStateUpdates(LinkController* pLnkCtrler)
{
    LOGD("sendStopCallStateUpdates()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopCallStateUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}

void SessionCommunications::sendStartCommunicationsUpdates(LinkController* pLnkCtrler, StartCommunicationsUpdatesArg* pArg)
{
    LOGD("sendStartcommunicationsUpdates()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    createStartcommunicationsUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartCommunicationsUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionCommunications::sendStopCommunicationsUpdates(LinkController* pLnkCtrler)
{
    LOGD("sendStopCallStateUpdates()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopCommunicationsUpdates, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionCommunications::sendInitialCall(LinkController* pLnkCtrler, InitiateCallArg* pArg)
{
   LOGD("sendInitialCall()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    createInitialCallParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdInitialCall, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionCommunications::sendAcceptCall(LinkController* pLnkCtrler, AcceptCallArg* pArg)
{
   LOGD("sendAcceptCall()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    createAcceptCallParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdAcceptCall, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}



void SessionCommunications::sendEndCall(LinkController* pLnkCtrler, EndCallArg* pArg)
{
   LOGD("sendEndCall()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    createEndCallParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdEndCall, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionCommunications::sendSwapCalls(LinkController* pLnkCtrler)
{
    LOGD("sendSwapCalls()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdSwapCalls, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionCommunications::sendMergeCalls(LinkController* pLnkCtrler)
{
    LOGD("sendMergeCalls()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdMergeCalls, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionCommunications::sendHoldStatusUpdate(LinkController* pLnkCtrler, HoldStatusUpdateArg* pArg)
{
   LOGD("sendHoldStatusUpdate()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    createHoldStatusUpdateParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdHoldStatusUpdate, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}



void SessionCommunications::sendMuteStatusUpdate(LinkController* pLnkCtrler, MuteStatusUpdateArg* pArg)
{
   LOGD("sendMuteStatusUpdate()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    createMuteStatusUpdateParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdMuteStatusUpdate, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionCommunications::sendSendDTMF(LinkController* pLnkCtrler,SendDTMFArg* pArg)
{
   LOGD("sendSendDTMF()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    createSendDTMFParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdSendDTMF, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionCommunications::sendStartListUpdates(LinkController* pLnkCtrler,StartListUpdatesArg* pArg)
{
   LOGD("sendStartListUpdate()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

#ifndef TEST_CODE
    if (pArg == NULL) {
        LOGE("    ERROR :  pArg is NULL");
        return;
    }
#endif // #ifndef TEST_CODE

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    createStartListUpdatesParamList(nParamIdList, paramDataList, nParamDataLenList, pArg);
    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStartListUpdate, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionCommunications::sendStopListUpdates(LinkController* pLnkCtrler)
{
    LOGD("sendStopListUpdates()");

    if (pLnkCtrler == NULL) {
        LOGE("    ERROR :  pLnkCtrler is NULL");
        return;
    }

    struct iAP2Link_st* link;
    if (pLnkCtrler->m_piAP2LinkRunLoop)
        link = pLnkCtrler->m_piAP2LinkRunLoop->link;
    else {
        LOGE("    ERROR :  m_piAP2LinkRunLoop is NULL");
        return;
    }

    uint32_t nEncodedDataLength;
    uint8_t* encodedData = NULL;

    std::vector<int> nParamIdList;
    std::vector<const uint8_t*> paramDataList;
    std::vector<uint32_t> nParamDataLenList;

    nEncodedDataLength = DataEncoder::encodeMsg(kIAP2PacketServiceTypeControl, kMsgIdStopListUpdate, nParamIdList, paramDataList, nParamDataLenList, &encodedData);

    ByteArray encodedDataArr((char*)encodedData, nEncodedDataLength);

    int8_t session = pLnkCtrler->getSessionId(kIAP2PacketServiceTypeControl);
    if (session == 0) {
        LOGE("    ERROR : sessionId is 0");
        return;
    }
    pLnkCtrler->processOutBoundTraffic(link, session, encodedDataArr);        

    DataEncoder::deleteParamList(nParamIdList, paramDataList, nParamDataLenList);
    delete [] encodedData;
}


void SessionCommunications::createStartCallStateUpdatesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StartCallStateUpdatesArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendNoneParam(kParamIdStartCallStateUpdates_RemoteID, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kParamIdStartCallStateUpdates_DisplayName, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kParamIdStartCallStateUpdates_Status, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kParamIdStartCallStateUpdates_Direction, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kParamIdStartCallStateUpdates_CallUUID, nParamIdList, paramDataList, nParamDataLenList);
    
    if(pArg->bAddressBookID == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCallStateUpdates_AddressBookID, nParamIdList, paramDataList, nParamDataLenList);
    }

    if(pArg->bLabel == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCallStateUpdates_Label, nParamIdList, paramDataList, nParamDataLenList);
    }
    
    DataEncoder::appendNoneParam(kParamIdStartCallStateUpdates_Service, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kParamIdStartCallStateUpdates_IsConferenced, nParamIdList, paramDataList, nParamDataLenList);
    DataEncoder::appendNoneParam(kParamIdStartCallStateUpdates_ConferenceGroup, nParamIdList, paramDataList, nParamDataLenList);

    if(pArg->bDisconnectReason== TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCallStateUpdates_DisconnectReason, nParamIdList, paramDataList, nParamDataLenList);
    }
}



void SessionCommunications::createStartcommunicationsUpdatesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StartCommunicationsUpdatesArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();
    
    if(pArg->bSignalStrength == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_SignalStrength, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bRegistrationStatus == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_RegistrationStatus, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bAirplaneModeStatus == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_AirplaneModeStatus, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bCarrierName == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_CarrierName, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bCelluarSupported == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_CellularSupported, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bTelephonyEnabled == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_TelephonyEnabled, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bFaceTimeAudioEnabled == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_FaceTimeAudioEnabled, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bFaceTimeVideoEnabled == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_FaceTimeVideoEnabled, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bMuteStatus == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_MuteStatus, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bCurrentCallCount == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_CurrentCallCount, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bNewVoicemailCount == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_NewVoicemailCount, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bInitiateCallAvailable == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_InitiateCallAvailable, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bEndAndAccptAvailable == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_EndAndAcceptAvailable, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bHoldAndAcceptAvailable == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_HoldAndAcceptAvailable, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bSwapAvailable == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_SwapAvailable, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bMergeAvailable == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_MergeAvailable, nParamIdList, paramDataList, nParamDataLenList);
    }
    if(pArg->bHoldAvailable == TRUE)
    {
        DataEncoder::appendNoneParam(kParamIdStartCommunicationsUpdates_HoldAvailable, nParamIdList, paramDataList, nParamDataLenList);
    }
    
    
}


void SessionCommunications::createInitialCallParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, InitiateCallArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendNumericParam(pArg->eType,1, kParamIdInitalCall_Type, nParamIdList, paramDataList, nParamDataLenList);
     
    if (pArg->mDestinationID.bPresent== TRUE) {        
        DataEncoder::appendBytesParam(pArg->mDestinationID.value, pArg->mDestinationID.size, kParamIdInitalCall_DestinationID, nParamIdList, paramDataList, nParamDataLenList);
    }

    if (pArg->mService.bPresent== TRUE) {
        DataEncoder::appendNumericParam(pArg->mService.value,1, kParamIdInitalCall_Servicee, nParamIdList, paramDataList, nParamDataLenList);     
    }
    
    if (pArg->mAddressBookID.bPresent== TRUE) {        
        DataEncoder::appendBytesParam(pArg->mAddressBookID.value, pArg->mAddressBookID.size, kParamIdInitalCall_AddressBookID, nParamIdList, paramDataList, nParamDataLenList);
    }

}


void SessionCommunications::createAcceptCallParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, AcceptCallArg* pArg)
{

    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendNumericParam(pArg->eAcceptAction,1, kParamIdAcceptCall_AcceptAction, nParamIdList, paramDataList, nParamDataLenList);
     
    if (pArg->mCallUUID.bPresent== TRUE) {        
        DataEncoder::appendBytesParam(pArg->mCallUUID.value, pArg->mCallUUID.size, kParamIdAcceptCall_CallUUID, nParamIdList, paramDataList, nParamDataLenList);
    }

}



void SessionCommunications::createEndCallParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, EndCallArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendNumericParam(pArg->eEndAction,1, kParamIdEndCall_EndAction, nParamIdList, paramDataList, nParamDataLenList);
     
    if (pArg->mCallUUID.bPresent== TRUE) {        
        DataEncoder::appendBytesParam(pArg->mCallUUID.value, pArg->mCallUUID.size, kParamIdEndCall_CallUUID, nParamIdList, paramDataList, nParamDataLenList);
    }

}

void SessionCommunications::createHoldStatusUpdateParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, HoldStatusUpdateArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendNumericParam(pArg->bHoldStatus,1, kParamIdHoldStateUpdate_HoldStatus, nParamIdList, paramDataList, nParamDataLenList);
     
    if (pArg->mCallUUID.bPresent== TRUE) {        
        DataEncoder::appendBytesParam(pArg->mCallUUID.value, pArg->mCallUUID.size, kParamIdHoldStateUpdate_CallUUID, nParamIdList, paramDataList, nParamDataLenList);
    }

}

void SessionCommunications::createMuteStatusUpdateParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, MuteStatusUpdateArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendNumericParam(pArg->bMuteStatus,1, kParamIdMuteStateUpdate_MuteStatus, nParamIdList, paramDataList, nParamDataLenList);
}

void SessionCommunications::createSendDTMFParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, SendDTMFArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    DataEncoder::appendNumericParam(pArg->eTone,1, kParamIdSendDTMF_Tone, nParamIdList, paramDataList, nParamDataLenList);
     
    if (pArg->mCallUUID.bPresent== TRUE) {        
        DataEncoder::appendBytesParam(pArg->mCallUUID.value, pArg->mCallUUID.size, kParamIdSendDTMF_CallUUID, nParamIdList, paramDataList, nParamDataLenList);
    }

}


void SessionCommunications::createStartListUpdatesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t *> &paramDataList, std::vector<uint32_t> &nParamDataLenList, StartListUpdatesArg* pArg)
{
    nParamIdList.clear();
    paramDataList.clear();
    nParamDataLenList.clear();

    
    uint8_t* groupParamBytes = NULL;
    uint32_t nNumOfBytes;

    if(pArg->mRecenttsListPropoerties.bPresent == TRUE)
    {        
        nNumOfBytes = createGroupParamBytesForRecentsListProperties(&groupParamBytes, pArg);

        DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdStartListUpdates_RecentsListProperties, nParamIdList, paramDataList, nParamDataLenList);
        delete [] groupParamBytes;
    }

    if(pArg->mRecentsListMax.bPresent == TRUE) 
    {    
        DataEncoder::appendNumericParam(pArg->mRecentsListMax.value,2, kParamIdStartListUpdates_RecentsListmax, nParamIdList, paramDataList, nParamDataLenList);
    }

    if(pArg->bRecentsListCombine == TRUE)
    {
        DataEncoder::appendNumericParam(pArg->bRecentsListCombine,1, kParamIdStartListUpdates_RecentsListCombine, nParamIdList, paramDataList, nParamDataLenList);
    }

    if(pArg->mFavoritesListProerties.bPresent == TRUE) 
    {
        nNumOfBytes = createGroupParamBytesForFavoritesListProperties(&groupParamBytes, pArg);

        DataEncoder::appendBytesParam(groupParamBytes, nNumOfBytes, kParamIdStartListUpdates_FavoritesListProperties, nParamIdList, paramDataList, nParamDataLenList);
        delete [] groupParamBytes;
    }

    if(pArg->mFavoritesListMax.bPresent == TRUE) 
    {    
        DataEncoder::appendNumericParam(pArg->mFavoritesListMax.value,2, kParamIdStartListUpdates_FavoritesListMax, nParamIdList, paramDataList, nParamDataLenList);
    }

}


uint32_t SessionCommunications::createGroupParamBytesForRecentsListProperties(uint8_t** encodedBytes, StartListUpdatesArg* pArg)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;
    uint8_t* encodedDataArr;

    createRecentsListPropertiesParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList, pArg);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];
    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);
    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);
    *encodedBytes = encodedDataArr;
    return nTotalLength;
}

uint32_t SessionCommunications::createGroupParamBytesForFavoritesListProperties(uint8_t** encodedBytes, StartListUpdatesArg* pArg)
{
    unsigned int i, nTotalLength;
    std::vector<int> nSubParamIdList;
    std::vector<const uint8_t *> subParamDataList;
    std::vector<uint32_t> nSubParamDataLenList;
    uint8_t* encodedDataArr;

    createFavoritesListPropertiesParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList, pArg);

    nTotalLength = 0;
    for(i = 0; i < nSubParamIdList.size(); i++) {
        nTotalLength = nTotalLength + 2 + 2 + nSubParamDataLenList.at(i);
    }

    encodedDataArr = new uint8_t[nTotalLength];
    DataEncoder::encodeParam(&encodedDataArr[0], nSubParamIdList, subParamDataList, nSubParamDataLenList);
    DataEncoder::deleteParamList(nSubParamIdList, subParamDataList, nSubParamDataLenList);
    *encodedBytes = encodedDataArr;
    return nTotalLength;
}

void SessionCommunications::createRecentsListPropertiesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                                              std::vector<uint32_t> &nParamDataLenList, StartListUpdatesArg* pArg)
{
    if (pArg->mRecenttsListPropoerties.bPresent == TRUE) 
    {
        DataEncoder::appendNoneParam(kSubParamIdRecentListPropoperties_Index, nParamIdList, paramDataList, nParamDataLenList);
        DataEncoder::appendNoneParam(kSubParamIdRecentListPropoperties_RemoteID, nParamIdList, paramDataList, nParamDataLenList);
        DataEncoder::appendNoneParam(kSubParamIdRecentListPropoperties_DisplayName, nParamIdList, paramDataList, nParamDataLenList);
            
        if (pArg->mRecenttsListPropoerties.bLabel == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdRecentListPropoperties_Label, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mRecenttsListPropoerties.bAdressBookID == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdRecentListPropoperties_AddressBookID, nParamIdList, paramDataList, nParamDataLenList);

        DataEncoder::appendNoneParam(kSubParamIdRecentListPropoperties_Service, nParamIdList, paramDataList, nParamDataLenList);
        DataEncoder::appendNoneParam(kSubParamIdRecentListPropoperties_Type, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mRecenttsListPropoerties.bUnixTimestamp == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdRecentListPropoperties_UnixTimestamp, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mRecenttsListPropoerties.bDuration == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdRecentListPropoperties_Duration, nParamIdList, paramDataList, nParamDataLenList);

        DataEncoder::appendNoneParam(kSubParamIdRecentListPropoperties_Occurrences, nParamIdList, paramDataList, nParamDataLenList);    
    }
}

void SessionCommunications::createFavoritesListPropertiesParamList(std::vector<int> &nParamIdList, std::vector<const uint8_t*> &paramDataList,
                                                              std::vector<uint32_t> &nParamDataLenList, StartListUpdatesArg* pArg)
{
    if (pArg->mFavoritesListProerties.bPresent == TRUE) 
    {
        DataEncoder::appendNoneParam(kSubParamIdFavoritesListPropoperties_Index, nParamIdList, paramDataList, nParamDataLenList);
        DataEncoder::appendNoneParam(kSubParamIdFavoritesListPropoperties_RemoteID, nParamIdList, paramDataList, nParamDataLenList);
        DataEncoder::appendNoneParam(kSubParamIdFavoritesListPropoperties_DisplayName, nParamIdList, paramDataList, nParamDataLenList);
            
        if (pArg->mFavoritesListProerties.bLabel == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdFavoritesListPropoperties_Label, nParamIdList, paramDataList, nParamDataLenList);

        if (pArg->mFavoritesListProerties.bAdressBookID == TRUE)
            DataEncoder::appendNoneParam(kSubParamIdFavoritesListPropoperties_AddressBookID, nParamIdList, paramDataList, nParamDataLenList);

        DataEncoder::appendNoneParam(kSubParamIdFavoritesListPropoperties_Service, nParamIdList, paramDataList, nParamDataLenList);
        
    }
}


}// namespace IAP2

