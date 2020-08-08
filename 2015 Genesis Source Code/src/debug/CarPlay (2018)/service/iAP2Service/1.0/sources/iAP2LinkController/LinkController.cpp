/*
    File:       LinkController.cpp
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

#include "LinkController.h"
#include "DataDecoder.h"
#include "DataEncoder.h"
#include "stdlib.h"
#include "iAp2ServiceLog.h"
#include <utils.h>
#include <iAP2LinkRunLoop.h>
#ifdef MIBGP
#include "bscommon.h"
#include "bsipc.h"
#endif
namespace IAP2
{

static void usbConnStateCbFunc(void* pData, UsbConnectState connState, ConnectionPath connPath)
{
    LinkController* pLinkCtrl = (LinkController*) pData;

    if (pLinkCtrl != NULL)
        pLinkCtrl->usbConnStateCb(connState, connPath);
}

static void usbConnModeCbFunc(void* pData, AccUsbMode connMode)
{
    LinkController* pLinkCtrl = (LinkController*) pData;

    if (pLinkCtrl != NULL)
        pLinkCtrl->usbConnModeCb(connMode);
}

static void carplaySupportCbFunc(void* pData, ConnectionCarPlaySupport carPlaySupport)
{
    LinkController* pLinkCtrl = (LinkController*) pData;

    if (pLinkCtrl != NULL)
        pLinkCtrl->carPlaySupportCb(carPlaySupport);
}

static void iap2SupportCbFunc(void* pData, ConnectionIap2Support iap2Support)
{
    LinkController* pLinkCtrl = (LinkController*) pData;

    if (pLinkCtrl != NULL)
        pLinkCtrl->iap2SupportCb(iap2Support);
}

#ifdef ANDROID
static void UsbDeviceInfoCbFunc(void* pData, int productId, int vendorId, char *product)
{
    LinkController* pLinkCtrl = (LinkController*) pData;

    if (pLinkCtrl != NULL)
             pLinkCtrl->usbDeviceInfoCb(productId, vendorId, product);
}
#endif

static BOOL getCarplayOnOffStateFunc(void* pData)
{
    LinkController* pLinkCtrl = (LinkController*) pData;

    LOGI("getCaplayOnOffStateFunc() pLinkCtrl->mConnection.CarplayOnoff : %d",pLinkCtrl->mConnection.CarplayOnoff);

    return pLinkCtrl->mConnection.CarplayOnoff;
}

static BOOL getCarPlayClientStateFunc(void* pData)
{
    LinkController* pLinkCtrl = (LinkController*) pData;

    LOGI("getCarPlayClientStateFunc() pLinkCtrl->mConnection.mCarPlayClientState : %d",pLinkCtrl->mConnection.mCarPlayClientState);

    return pLinkCtrl->mConnection.mCarPlayClientState;
}


static iAP2LinkRunLoop_t* getiAP2LinkRunLoopFunc(void* pData)
{
    LinkController* pLinkCtrl = (LinkController*) pData;

    LOGI("getiAP2LinkRunLoopFunc() pLinkCtrl->m_piAP2LinkRunLoop : %p",pLinkCtrl->m_piAP2LinkRunLoop);

    return pLinkCtrl->m_piAP2LinkRunLoop;
}

static void createIAP2LinkRunLoopForAccessoryFunc(void* pData)
{
    LinkController* pLinkCtrl = (LinkController*) pData;

    if (pLinkCtrl != NULL)
             pLinkCtrl->createIAP2LinkRunLoopForAccessory();
}

static void DeleteiAP2LinkRunLoopFunc(void* pData)
{
    LinkController* pLinkCtrl = (LinkController*) pData;

    if (pLinkCtrl != NULL)
             pLinkCtrl->DeleteiAP2LinkRunLoop();
}


LinkController::LinkController(SendEventToApp* pSendEventToApp) : mTid(0), mIsTerminated(false), mNumSessionInfo(0), m_piAP2LinkRunLoop(NULL), mpSendEventToApp(pSendEventToApp), mpDevMonitor(NULL)
{
    LOGD("LinkController()");

    memset(&m_synParam, 0x00, sizeof(iAP2PacketSYNData_t));
    memset(&m_linkRLBuffer[0], 0x00, sizeof(m_linkRLBuffer));
    memset(&mSessionInfo[0], 0x00, sizeof(mSessionInfo));
    memset(&miAP2FileTransfer_Buffer, 0x00, sizeof(miAP2FileTransfer_Buffer));

    mUserInfo = 0;
    m_piAP2FileTransfer = NULL;     
    mLikCtrlConnCbs.pData = this;
    mLikCtrlConnCbs.connStateCb = usbConnStateCbFunc;
    mLikCtrlConnCbs.connModeCb = usbConnModeCbFunc;
    mLikCtrlConnCbs.carplaySupportCb = carplaySupportCbFunc;
    mLikCtrlConnCbs.iap2SupportCb = iap2SupportCbFunc;
    
    #ifdef ANDROID
    mLikCtrlConnCbs.deviceInfoCb = UsbDeviceInfoCbFunc;
    #endif

    mLikCtrlConnCbs.getCarplayOnOffstateCb = getCarplayOnOffStateFunc;
    mLikCtrlConnCbs.createIAP2LinkRunLoopForAccessoryCb = createIAP2LinkRunLoopForAccessoryFunc;
    mLikCtrlConnCbs.DeleteiAP2LinkRunLoopCb = DeleteiAP2LinkRunLoopFunc;
    mLikCtrlConnCbs.getiAP2LinkRunLoopCB = getiAP2LinkRunLoopFunc;
    mLikCtrlConnCbs.getCarPlayClientStateCb = getCarPlayClientStateFunc;
}


LinkController::~LinkController()
{
    LOGV("~LinkController()");

    if (mpDevMonitor != NULL)
    {
        delete mpDevMonitor;
        mpDevMonitor = NULL;
    }
}

void LinkController::initSynParam(iAP2PacketSYNData_t &synParam)
{
    synParam.version = 0x01;
    synParam.maxOutstandingPackets = 3;
    synParam.maxPacketSize = 0x2800;//x1000; // 10K
    synParam.retransmitTimeout= 0x07d0;
    synParam.cumAckTimeout = 1000;
    synParam.maxRetransmissions = 0x1e;
    synParam.maxCumAck = 3;
    synParam.sessionInfo[0].id = 0x01;
    synParam.sessionInfo[0].type = 0x00;
    synParam.sessionInfo[0].version = 0x02;
    synParam.numSessionInfo = 0x01;
    synParam.peerMaxPacketSize = 0x80;
    synParam.peerMaxOutstandingPackets = 0x05;
    synParam.numSessionInfo = 0x01;

    // File Transfer
    if(mIdentification.mIdentficationMsgSelect.NowPlaying || mIdentification.mIdentficationMsgSelect.MediaLibraryUpdates) {
        synParam.sessionInfo[1].id = 0x02;
        synParam.sessionInfo[1].type = 0x01;
        synParam.sessionInfo[1].version = 0x01;
        synParam.numSessionInfo = 0x02;
    }
    

    // EA 
    if(mIdentification.mIdentficationMsgSelect.EAiAP2Session ) {
        if(!((mIdentification.mIdentficationMsgSelect.UseEAWithCarPlay == FALSE) && (mConnection.mCarplaySupport == CCS_CARPLAY_SUPPORTED) ))
        {
            synParam.sessionInfo[2].id = 0x03;
            synParam.sessionInfo[2].type = 0x02;
            synParam.sessionInfo[2].version = 0x01;
            synParam.numSessionInfo = 0x03;
        }        
    }
    
}

uint8_t LinkController::getSessionId(iAP2PacketServiceType_t sessionType)
{
    int i;
    for (i=0; i < mNumSessionInfo; i++) {
        if (mSessionInfo[i].type == sessionType) {
            return mSessionInfo[i].id;
        }
    }

    return 0; // error
}

void LinkController::createIAP2LinkRunLoopForAccessory()
{
    initSynParam(m_synParam);

    m_piAP2LinkRunLoop = iAP2LinkRunLoopCreateAccessory(&m_synParam,
                            this, //perfecto!
                            sendPacketCB_Caller,
                            recvDataCB_Caller,
                            connectedCB_Caller,
                            sendDetectCB_Caller,
                            0,
                            255, //set from 1 to 255 because Memory Rick Occurs when send 2 packet  at once
                            m_linkRLBuffer
                            );
    LOGI("createIAP2LinkRunLoopForAccessory() m_piAP2LinkRunLoop[%p] this[%p]", m_piAP2LinkRunLoop, this);
}

void LinkController::DeleteiAP2LinkRunLoop()
{
    LOGI("DeleteiAP2LinkRunLoop() m_piAP2LinkRunLoop : %p", m_piAP2LinkRunLoop);
    
    iAP2LinkRunLoopDelete(m_piAP2LinkRunLoop);
}

void LinkController::sendDetectCB_Caller(struct iAP2Link_st* link, BOOL bBad)
{
    if (link && link->context) {
        LinkController* pInstance = (LinkController*)((iAP2LinkRunLoop_t*)(link->context))->context;
        //LOGD("sendDetectCB_Caller() link[%p] pInstance[%p] bBad[%d]", link, pInstance, bBad);

        pInstance->sendDetectCB(link, bBad);
    }
    else {
        LOGE("    ERROR: link[%p] or link->context is NULL", link);
    }
}

void LinkController::sendPacketCB_Caller(struct iAP2Link_st* link, iAP2Packet_t* packet)
{
    if (link && link->context) {
        LinkController* pInstance = (LinkController*)((iAP2LinkRunLoop_t*)(link->context))->context;
        //LOGD("sendPacketCB_Caller() link[%p] pInstance[%p]", link, pInstance);

        pInstance->sendPacketCB(link, packet);
    }
    else {
        LOGE("    ERROR: link[%p] or link->context is NULL", link);
    }
}

BOOL LinkController::recvDataCB_Caller(struct iAP2Link_st* link, uint8_t* data, uint32_t dataLen, uint8_t session)
{
    if (link && link->context) {
        LinkController* pInstance = (LinkController*)((iAP2LinkRunLoop_t*)(link->context))->context;
        //LOGD("recvDataCB_Caller() link[%p] pInstance[%p] data[%p] dataLen[%d] session[%d]", link, pInstance, data, dataLen, session);

        pInstance->recvDataCB(link, data, dataLen, session);
    }
    else {
        LOGE("    ERROR: link[%p] or link->context is NULL", link);
        return FALSE;
    }

    return TRUE;
}

void LinkController::connectedCB_Caller(struct iAP2Link_st* link, BOOL bConnected)
{
    if (link && link->context) {
        LinkController* pInstance = (LinkController*)((iAP2LinkRunLoop_t*)(link->context))->context;
        //LOGD("connectedCB_Caller() link[%p] pInstance[%p] bConnected[%d]", link, pInstance, bConnected);

        pInstance->connectedCB(link, bConnected);
    }
    else {
        LOGE("    ERROR: link[%p] or link->context is NULL", link);
    }
}

void LinkController::sendDetectCB(struct iAP2Link_st* link, BOOL bBad)
{
    //LOGD("sendDetectCB()");
    (void) link;
    (void) bBad;
    if (mConnection.mCarplaySupport == TRUE){ 
        if (mpDevMonitor && mpDevMonitor->mpTransport) {
            unsigned char iAp2SupportPck[6] = { 0xFF, 0x55, 0x02, 0x00, 0xEE, 0x10};
            ByteArray sendPacket((char*)iAp2SupportPck, sizeof(iAp2SupportPck));

            mpDevMonitor->mpTransport->sendDataToDevice(sendPacket, true);
        }
        else {
            LOGE("    ERROR : mpDevMonitor[%p]", mpDevMonitor);
        }
    }

}


/**
sendPacketCB callback function.
linkRunLoop will call this function to send the final data to the device
*/
void LinkController::sendPacketCB(struct iAP2Link_st* link, iAP2Packet_t* packet)
{
    (void) link;
    //LOGD("sendPacketCB() link[%p] packet[%p]", link, packet);

    if (mConnection.mCarplaySupport == TRUE){ 
        if (mpDevMonitor && mpDevMonitor->mpTransport) {
            ByteArray sendPacket;
            sendPacket.reserve(packet->dataCurLen+10);
            
            iAP2PacketCalcHeaderChecksum(packet);
            
            sendPacket.append(packet->pckData->sop1);
            sendPacket.append(packet->pckData->sop2);
            sendPacket.append(packet->pckData->len1);
            sendPacket.append(packet->pckData->len2);
            sendPacket.append(packet->pckData->ctl);
            sendPacket.append(packet->pckData->seq);
            sendPacket.append(packet->pckData->ack);
            sendPacket.append(packet->pckData->sess);
            sendPacket.append(packet->pckData->chk);

            if (packet->dataCurLen > 0) {
                iAP2PacketCalcPayloadChecksum(packet);        
                sendPacket.append((char*)packet->pckData->data, packet->dataCurLen);
                sendPacket.append(packet->dataChecksum);
            }

            mpDevMonitor->mpTransport->sendDataToDevice(sendPacket, false);
        }
        else
        {
            LOGE("    ERROR : mpDevMonitor[%p]", mpDevMonitor);
        }
    }
}


BOOL LinkController::recvDataCB(struct iAP2Link_st* link, uint8_t* data, uint32_t dataLen, uint8_t session)
{
    //LOGD("recvDataCB() session[%d]", session);
    
    if (session == 1) // Control Session
        handleReceivedData(link, data, dataLen, session);

    else if (session == 2) // File Transfer Session
    {
        LOGD("LinkController::recvDataCB() link[%p] data[%p] dataLen[%u]", link, data, dataLen);
        if (link && data && (dataLen > 0)) {
            if (iAP2FileTransferValidateBufferID(link, session, data[kiAP2FileTransferHdrIdxID]) == TRUE) {
                stFileTransferInform* mstFileTransferInform = NULL;
                mstFileTransferInform = mFileTransferManage.searchDatafrombufferID(data[kiAP2FileTransferHdrIdxID]);
                if(mstFileTransferInform != NULL) {
                   // mstFileTransferInform->bufferID = data[kiAP2FileTransferHdrIdxID];
                    mstFileTransferInform->m_piAP2FileTransfer->link = link;
                    BOOL deleteBuffer = FALSE;
                    deleteBuffer = iAP2FileTransferHandleRecv (mstFileTransferInform->m_piAP2FileTransfer, data, dataLen);
                    if(deleteBuffer == TRUE) {
                        uint8_t control = data [kiAP2FileTransferHdrIdxControl];
                        BOOL isLast  = ((control & kiAP2FileTransferControlLast) != 0);
                        if(isLast == TRUE) {
                            LOGD("iAP2FileTransferSuccess"); 
                            iAP2FileTransferSuccess (mstFileTransferInform->m_piAP2FileTransfer);
                            LOGD("mstFileTransferInform->mFiletransferMode : %u, mstFileTransferInform->m_piAP2FileTransfer : %p", mstFileTransferInform->mFiletransferMode, mstFileTransferInform->m_piAP2FileTransfer);
                            mFileTransfer.iAP2FileTransferEndCB_Caller(mstFileTransferInform->m_piAP2FileTransfer, (void*)&mstFileTransferInform->mFiletransferMode);    
                        }
                        else
                             mFileTransfer.iAP2FileTransferEndCB_Caller(mstFileTransferInform->m_piAP2FileTransfer, (void*)&mstFileTransferInform->mFiletransferMode); 
                    }
                }     
            }
            else {
                LOGE(" recvDataCB :: Wrong Direction :: iAP2FileTransferCancelSetup() ");
                iAP2FileTransferCancelSetup(link, session, data[kiAP2FileTransferHdrIdxID]);
            }
        }
        else
        {
            LOGE(" recvDataCB :: NULL Pointer!!!!!!!!!!!!!!!!!!!!!!!!!!!! ");
        }
    }
    else if (session == 3) // EA Session 
    {
        LOGD("####### EA Session Incomming #####");
        handleEAPReceivedData(link, data, dataLen, session);
    }
    return 0;
}

// Callback function to call when link connection is UP/DOWN. // When receives SYNC ACK
void LinkController::connectedCB(struct iAP2Link_st* link, BOOL bConnected)
{
    //LOGD("connectedCB() link[%d] bConnected[%d]", link, bConnected);
    if (bConnected == TRUE) {
        if (link != NULL) {
            mNumSessionInfo = link->negotiatedParam.numSessionInfo;
            memcpy(&mSessionInfo, &link->negotiatedParam.sessionInfo[0], sizeof(iAP2PacketSessionInfo_t)*mNumSessionInfo);
        }
        else {
            LOGE("    ERROR : link is NULL");
        }
    }
    else {
        mNumSessionInfo = 0;
        memset(&mSessionInfo[0], 0x00, sizeof(mSessionInfo));
    }
}

void LinkController::handleReceivedData(iAP2Link_st* link, uint8_t* data, uint32_t dataLen, uint8_t session)
{
    //Check message header.
    int nMsgId;
    nMsgId = DataDecoder::getMsgId(data, dataLen);
    

    switch(nMsgId)
    {
        case kMsgIdRequestAuthenticationCertificate:
            LOGD("    kMsgIdRequestAuthenticationCertificate");
#ifdef MIBGP
            bs_global_sem_wait(GLOBAL_SEM_ID_APPLE_CP);
#endif
            mAuthentication.sendAuthenticationCertificate(this, link, session);
#ifdef MIBGP
            bs_global_sem_post(GLOBAL_SEM_ID_APPLE_CP);
#endif
            break;
            
        case kMsgIdRequestAuthenticationChallengeResponse:
            LOGD("    kMsgIdRequestAuthenticationChallengeResponse");
#ifdef MIBGP
            bs_global_sem_wait(GLOBAL_SEM_ID_APPLE_CP);
#endif
            mAuthentication.sendAuthenticationChallengeResponse(this, link, session, DataDecoder::getParamData(data, dataLen,0));
#ifdef MIBGP
            bs_global_sem_post(GLOBAL_SEM_ID_APPLE_CP);
#endif
             break;
            
        case kMsgIdAuthenticationFailed:
            LOGD("    == authentication is failed");
            mAuthentication.handleAuthenticationFailedUpdateEvent(this);
            break;
            
        case kMsgIdAuthenticationSucceed:
            LOGD("    == authentication is finished");
               mConnection.handleConnectionStateUpdate(this, CS_AUTHENTICATED);
#ifdef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
            mConnection.bAuthentication = TRUE;
            if (mConnection.bIdentification) {
                mPower.sendStartPowerUpdates(this, NULL); // p 439 jungyeon.kim
                mPower.sendPowerSourceUpdate(this, NULL);
                mConnection.bAuthentication = FALSE;
                mConnection.bIdentification = FALSE;
                mConnection.handleConnectionStateUpdate(this, CS_POWER_UPDATED);
            }
#endif // #ifdef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE            
            break;
            
        case kMsgIdStartIdentification:
            LOGD("    kMsgIdStartIdentification");    
            IdentificationRejectedEvt event;
            memset(&event, 0x00, sizeof(IdentificationRejectedEvt));
            memset(&event.mRejectIdentficationMsgSelect, 0x01, sizeof(IdentficationMsgSelectArg));            
            mCommunications.bLegacy = FALSE;
            mIdentification.sendIdentificationInformation(this, link, session, &event);
            break;  
            
        case kMsgIdIdentificationAccepted:
            LOGD("    == identification is finished. session[%d] link[%d]", session, (int)link);
            mConnection.handleConnectionStateUpdate(this, CS_IDENTIFIED);
#ifdef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
            mConnection.bIdentification = TRUE;
            if (mConnection.bAuthentication == TRUE) {
                mPower.sendStartPowerUpdates(this, NULL); // p 439 jungyeon.kim
                mPower.sendPowerSourceUpdate(this, NULL);
                mConnection.bAuthentication = FALSE;
                mConnection.bIdentification = FALSE;
                mConnection.handleConnectionStateUpdate(this, CS_POWER_UPDATED);
            }
            mIdentification.CountRetryIdentfication = 2;
#endif // #ifdef _IAP2SERVICE_HANDLE_POWER_FEATURE_IN_IAP2SERVICE
            break;
        case kMsgIdNowPlayingUpdates:
            LOGD("    kMsgIdNowPlayingUpdates");
            mNowPlaying.handleNowPlayingUpdate(this, session, data);
            break;
        
        case kMsgIdIdentificationRejected:
            LOGD("    == identification is rejected");
            if(mIdentification.CountRetryIdentfication > 0) {
                mIdentification.handleIdentificationRejected(this, link, session, data);
                //mIdentification.sendIdentificationInformation(this, link, session);                
                mIdentification.CountRetryIdentfication--;
            }
            else {
                mIdentification.sendCancelIdentification(this, link, session);
                mIdentification.handleIdentificationFailedUpdateEvent(this);
                mIdentification.CountRetryIdentfication = 2;
            }
            break;

        case kMsgIdPowerUpdate:
            LOGD("    kMsgIdPowerUpdate");        
            mPower.handlePowerUpdate(this, session, data);
            break;
            
        case kMsgIdDeviceInformationUpdate:
            LOGD("    kMsgIdDeviceInformationUpdate");
            mDeviceNotification.handleDeviceInformationUpdate(this, session, data);
            break;

        case kMsgIdDeviceLanguageUpdate:
            LOGD("    kMsgIdDeviceLanguageUpdate");
            mDeviceNotification.handleDeviceLanguageUpdate(this, session, data);
            break;

#if 0
		case kMsgIdTelephonyCallStateInformation :
			LOGD("	kMsgIdTelephonyCallStateInformation");
			mTelephony.handleTelephonyCallStateInformation(this, session, data);
			break;
		
		case kMsgIdTelephonyUpdate :
			LOGD("	kMsgIdTelephonyUpdate");
			mTelephony.handleTelephonyUpdate(this, session, data);
			break;
#endif
        case kMsgIdStartLocationInformation:
            LOGD("    kMsgIdStartLocationInformation");
            mLocation.handleStartLocationInformation(this, session, data);
            break;

        case kMsgIdStopLocationInformation:
            LOGD("    kMsgIdStopLocationInformation");
            mLocation.handleStopLocationInformation(this, session, data);
            break;

        case kMsgIdMediaLibraryInformation://YI add
            LOGD("    kMsgIdMediaLibraryInformation");
            mMediaLibraryInformation.handleMediaLibraryInformation(this, session, data);
            break;    

          case kMsgIdMediaLibraryUpdate://YI add
            LOGD("    kMsgIdMediaLibraryUpdate");
            mMediaLibraryUpdates.handleMediaLibraryUpdates(this, session, data);
            break;    
            
        case kMsgIdDeviceHIDReport: //YI add
            LOGD("    kMsgIdDeviceHIDReport");
            mHID.handleDeviceHIDReport(this, session, data);
            break;        
            
        case kMsgIdStartVehicleStatusUpdates:
            LOGD("    kMsgIdStartVehicleStatusUpdates");
            mVehicleStatus.handleStartVehicleStatusUpdate(this, session, data);
            break;

        case kMsgIdBluetoothConnectionUpdate:
            LOGD("    kMsgIdBluetoothConnectionUpdate");
            mBTConnection.handleBTConnectionUpdate(this, session, data);
            break;
            
        case kMsgIdAssistiveTouchInformation: //YI add
            LOGD("    kMsgIdAssistiveTouchInformation");
            mAssistiveTouch.handleAssistiveTouchInformation(this, session, data);
            break;
            
        case kMsgIdStartExternalAccessoryProtocolSession: //YI add
            LOGD("    kMsgIdStartExternalAccessoryProtocolSession");
            mExternalAccessoryProtocol.handleStartExternalAccessoryProtocolSession(this, session, data);
            break;        

        case kMsgIdStopExternalAccessoryProtocolSession: //YI add
            LOGD("    kMsgIdStopExternalAccessoryProtocolSession");
            mExternalAccessoryProtocol.handleStopExternalAccessoryProtocolSession(this, session, data);
            break;    
            
        case kMsgIdUSBDeviceModeAudioInformation: //YI add
            LOGD("    kMsgIdUSBDeviceModeAudioInformation");
            mUSBDeviceModeAudio.handleUSBDeviceModeAudioInforamation(this, session, data);
            break;

        case kMsgIdVoiceOverUpdate: //YI add
            LOGD("    kMsgIdVoiceOverUpdate");
            mVoiceOver.handleVoiceOverUpdates(this, session, data);
            break;

        case kMsgIdVoiceOverCursorUpdate: //YI add
            LOGD("    kMsgIdVoiceOverCursorUpdate");
            mVoiceOver.handleVoiceOverCursorUpdates(this, session, data);
            break;

        case kMsgIdWiFiInformation: //YI add
            LOGD("    kMsgIdWiFiInformation");
            mWiFiInformation.handleWiFiInformation(this, session, data);
            break;    

        case kMsgIdCallStateUpdates: 
            LOGD("    kMsgIdCallStateUpdates");
            mCommunications.handleCallStateUpdates(this, session, data);
            break;    

        case kMsgIdCommunicationsUpdates: 
            LOGD("    kMsgIdCommunicationsUpdates");
            mCommunications.handleCommunicationsUpdates(this, session, data);
            break;

        case kMsgIdListUpdate: 
            LOGD("    kMsgIdListUpdate");
            mCommunications.handleListUpdate(this, session, data);
            break;

        default:
            LOGD("    default break nMsgId: 0x%x", nMsgId);
            break;
    }
}


void LinkController::handleEAPReceivedData(iAP2Link_st* link, uint8_t* data, uint32_t dataLen, uint8_t session)
{
    LOGD("handleEAPReceivedData");    
    (void) link;
    mExternalAccessoryProtocol.handleExternalAccessoryProtocolRecvData(this, session, data, dataLen);
}
/*
Outbound iAP2 traffic
---------------------

The accessory should perform the following on outgoing iAP2 traffic:

1a) Create a data buffer containing the session data, or
1b) Create iAP2Packet by calling iAP2PacketCreateEmptySendPacket
    and fill the payload with session data.
2a) Call iAP2LinkRunLoopQueueSendData or iAP2LinkQueueSendData
    with a session data buffer, or
2b) Call iAP2LinkQueueSendDataPacket with iAP2Packet send packet
    with a payload containing session data.

Once the packet is sent out successfully and has been ACK'd,
iAP2PacketDelete will be called by iAP2Link.
*/
void LinkController::processOutBoundTraffic(struct iAP2Link_st* link, uint8_t session, ByteArray& rSendPck)
{
    (void) link;

    if (m_piAP2LinkRunLoop) {
        iAP2LinkRunLoopQueueSendData(m_piAP2LinkRunLoop, (const uint8_t*)rSendPck.data(), rSendPck.length(), session, this/*super (LinkController)*/, NULL);
    }
}

void LinkController::processOutBoundTrafficEAPData(struct iAP2Link_st* link, uint8_t session, uint8_t* pdata, uint32_t size)
{
    (void) link;
    LOGD("processOutBoundTrafficEAPData session : %u",session);
    if (m_piAP2LinkRunLoop) {
        iAP2LinkRunLoopQueueSendData(m_piAP2LinkRunLoop, (const uint8_t*)pdata, size, session, this/*super (LinkController)*/, NULL);
    }
}

void LinkController::usbConnStateCb(UsbConnectState connState, ConnectionPath connPath)
{
    LOGI("usbConnStateCb() connState[%u] connPath[%u]", connState, connPath);

    switch(connState) {
        case LINKCTRL_USB_NOT_CONNECTED : {
            mConnection.handleConnectionStateAndPathUpdate(this, CS_USB_NOT_CONNECTED, connPath);
            break;
        }

        case LINKCTRL_USB_CONNECTED : {
            mConnection.handleConnectionStateAndPathUpdate(this, CS_USB_CONNECTED, connPath);
            break;
        }

        default :
            LOGE("    ERROR : unused connState[%u] connPath[%u]", connState, connPath);
            break;
    }

}

void LinkController::usbConnModeCb(AccUsbMode usbMode)
{
    LOGI("usbConnModeCb() usbMode[%u]", usbMode);

    switch(usbMode) {
        case LINKCTRL_AUB_HOST : {
            mConnection.handleConnectionModeUpdate(this, CM_AUB_HOST);
            break;
        }

        case LINKCTRL_AUB_DEVICE : {
            mConnection.handleConnectionModeUpdate(this, CM_AUB_DEVICE);
            break;
        }

        default :
            LOGE("    ERROR : unused usbMode[%u]", usbMode);
            break;
    }

}

void LinkController::carPlaySupportCb(ConnectionCarPlaySupport carPlaySupport)
{
    LOGI("carPlaySupportCb() carPlaySupport[%u]", carPlaySupport);
    mConnection.handleConnectionCarplaySupportUpdate(this, carPlaySupport);
}

void LinkController::iap2SupportCb(ConnectionIap2Support iap2Support)
{
    LOGI("iap2SupportCb() iap2Support[%u]", iap2Support);
    mConnection.handleConnectionIap2SupportUpdate(this, iap2Support);
}

#ifdef ANDROID
void LinkController::usbDeviceInfoCb(int productId, int vendorId, char *product)
{
    LOGI("usbDeviceInfoCb() productId:0x%x  vendorId: 0x%x  product: %s", productId, vendorId, product);
       mConnection.handleConnectionDeviceInfoUpdate(this, productId, vendorId, product);
}
#endif

/*
void LinkController::createIAP2FileTransferCreate(uint8_t bufferID)
{
    LOGD("createIAP2FileTransferCreate() : ID (%d)", bufferID);
    mUserInfo = FILETRANSFERMODE_NOWPLAYING_ARTWORK;
    m_piAP2FileTransfer = iAP2FileTransferCreate(m_piAP2LinkRunLoop->link,
                                        m_synParam.sessionInfo[1].id,
                                        bufferID,
                                        mFileTransfer.iAP2FileTransferGotDataCB_Caller,
                                        mFileTransfer.iAP2FileTransferEndCB_Caller,
                                        &mUserInfo,
                                        TRUE,
                                        &miAP2FileTransfer_Buffer
                                        );
#if 0
    // Use For Transfer Data ((Accessory -> iOS))
    iAP2FileTransferStart(m_piAP2FileTransfer,
                    m_piAP2FileTransfer->pBuffer,
                    m_piAP2FileTransfer->buffSize,
                    m_piAP2FileTransfer->totalSize,
                    iAP2FileTransferEndCB_Caller,
                    m_piAP2FileTransfer->gotCBUserInfo,
                    m_piAP2FileTransfer->bStream,
                    m_piAP2FileTransfer->bDeleteBuffOnFinish
                    );
#endif
}
*/

void LinkController::handleFileTransferReceivedData(iAP2FileTransfer_st* fileXfer, void* userInfo)
{
    LOGD("    handleFileTransferReceivedData :: ENTER : %p" , userInfo);    
    
//    mNowPlaying.handleNowPlayingAlbumArtworkUpdate(pInstance, fileXfer->pBuffer, fileXfer->session, (uint32_t)fileXfer->state);
    mFileTransfer.handleFileTransferUpdate(fileXfer, FILETRANSFERMODE_NOWPLAYING_ARTWORK);
}

BOOL LinkController::recvFileTransferDataCB(struct iAP2FileTransfer_st* fileXfer, void* userInfo)
{
    LOGD("    recvFileTransferDataCB :: ENTER");
    handleFileTransferReceivedData(fileXfer, userInfo);
    return TRUE;
}



}// namespace IAP2

