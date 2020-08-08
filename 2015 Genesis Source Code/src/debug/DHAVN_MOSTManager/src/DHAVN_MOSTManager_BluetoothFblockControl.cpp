/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Venkatesh                                        *
 * DATE       :  29 Aug 2011                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : MOST Manager is a middle ware component which *
 *                  talks to the applications and controller over *
 *                  QtDbus.The data sent over QtDbus is further   *
 *                  communicated to other MOST devices through ACS*
 *                  daemon.This module is also responsible for    *
 *                  commands from ACS daemon.                     *
 *                  This file contains the class definition for   *
 *                  Bluetooth FBlock control, responsible to      *
 *                  create instance of and Shadow(0x01)           *
 *                  configure it and add to most stack.           *
 *                  Listeners and handlers will be part of this   *
 *                  class.                                        *
 *                  Communicates with ACS daemon                  *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 *    DATE         PROGRAMMER               COMMENT               *
 ******************************************************************
 * 29 Aug 2011  Venkatesh Aeturi          Draft version           *
 * 15 Sep 2011  Amit Koparde              Added Debug Statement   *
 * 27 Sep 2011  Amit Koparde              Changed the parameters for API CurrentSongInfo of CBluetoothFblockControl
 * 14 Oct 2011  Neelima Khedad            Added API parameters for CallSate and CallDuration
 * 26 Oct 2011  Amit Koparde              Added new API's ModemLevel, UserMode, CallHistoryCopyStatus and MissedCallStatus
 * 08 Feb 2011  Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 ******************************************************************
 */

#include "DHAVN_MOSTManager_BluetoothFblockControl.h"
#include <QDBusVariant>
#include <QDBusArgument>
#include "DHAVN_MOSTManager_BluetoothFBlock.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"


/*
 *******************************************************************
 * Function:      CBluetoothFblockControl
 *
 * Description :  This is constructor of CBluetoothFblockControl class.
 *
 * Parameters :
 *    context     rw   It holds the instance of CMarshaledACSApplication.
 *
 * Returns :      none
 *
 *******************************************************************
 */

CBluetoothFblockControl::CBluetoothFblockControl(CMarshaledACSApplication *context)
    : CControlThreadBase(context, LOG_TAG_BT_CNT)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)    

    m_BluetoothFblock = new CBluetooth(0x01);
    m_pMOSTElement = m_BluetoothFblock;

    /** Instantiating Handlers for properties of Bluetooth */
    m_HangupCall = new CHangupCall(m_BluetoothFblock);
    m_HangupCallHandler = new CHangUpCallHandler(this);

    m_CallState = new CCallState(m_BluetoothFblock);

    m_DialNumber = new CDialNumber(m_BluetoothFblock);
    m_DialNumberHandlerBT = new CDialNumberHandlerBT(this);

    m_BTStatus = new CBTStatus(m_BluetoothFblock);

    m_UserMode = new CUserMode(m_BluetoothFblock);
    m_UserModeListenerBT = new CUserModeListenerBT(this);
}

/*
 *******************************************************************
 * Function:      ~CBluetoothFblockControl
 *
 * Description :  Is a Destructor.
 *
 * Parameters :   none
 *
 *
 * Returns :      none
 *
 *******************************************************************
 */

CBluetoothFblockControl::~CBluetoothFblockControl()
{
    delete m_HangupCallHandler;
    delete m_HangupCall;

    delete m_BluetoothFblock;    

    delete m_CallState;

    delete m_DialNumber;
    delete m_DialNumberHandlerBT;

    delete m_BTStatus;

    delete m_UserMode;
    delete m_UserModeListenerBT;
}

/*
 *******************************************************************
 * Function:     Initialize
 *
 * Description : Function initializes the MOST elements to Bluetooth
 *              FBlock and adds to MOST.
 *
 * Parameters :none
 *
 *
 * Returns :    true or false
 *
 *******************************************************************
 */

bool CBluetoothFblockControl::Initialize()
{
    if(m_BluetoothFblock->InitializeMOSTElement())
    {
        /** Setting the handlers for required parameters */
        m_HangupCall->SetHandler(m_HangupCallHandler);
        m_DialNumber->SetHandler(m_DialNumberHandlerBT);
        m_UserMode->SetListener(m_UserModeListenerBT);

        /** sending the notification value for callstate as invalid*/
        m_CallState->Size(1);
        m_CallState->DataCallStateValue((uint)0,(CCallState::TDataCallState)0xFF);
        m_CallState->DataCallTypeValue((uint)0,(CCallState::TDataCallType)0xFF);
        m_CallState->UpdateShadows();

        /** sending the notification value for BTStatus as invalid */
        m_BTStatus->Value(CBTStatus::BTModeInvalid);
        m_BTStatus->UpdateShadows();

        /** Sending the notification value for Usermode as invalid */
        m_UserMode->UserModeValue(CUserMode::UserModeInvalid);
        m_UserMode->MicOffValue(false);
        m_UserMode->UpdateShadows();

        m_Initialized = true;
    }   

    return m_Initialized;
}

bool CBluetoothFblockControl::FBlockVisible()
{
    return GetIsRegistered();
}


/*
 *******************************************************************
 * Function:CallState
 *
 * Description :Function called by the Bluetooth controller to set the CallState.
 *
 * Parameters :
 *    pos          rw   It holds the contact pos by the user. It is unsigned word value.
 *
 *    callState    rw   It holds the callState set by the user. It is unsigned byte value.
 *
 *    callType     rw   It holds the callType set by the user. It is unsigned byte value.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CBluetoothFblockControl::CallState(const QDBusVariant &callStatedata)
{
//    MOST_LOG_SIGNAL << LOG_TAG_BT_CNT << " CallState API got called " << MOST_LOG_ENDL;

    SCallStateDataList callStateDataList;
    callStatedata.variant().value<QDBusArgument>() >> callStateDataList;
    m_CallState->Size(callStateDataList.listCount);

    /** Store the Call State data in the list */
    for (uint index= 0; index < callStateDataList.listCount ;index++)
    {
        m_CallState->DataCallStateValue(index,(CCallState::TDataCallState)callStateDataList.callStateDatalist.at(index).callState);
        m_CallState->DataCallTypeValue(index,(CCallState::TDataCallType)callStateDataList.callStateDatalist.at(index).callType);
    }
/*
    MOST_LOG_HIGH << LOG_TAG_BT_CNT << " CallState:List Count = " << callStateDataList.listCount << MOST_LOG_ENDL;
    if(callStateDataList.listCount > 0)
    {
        MOST_LOG_HIGH << LOG_TAG_BT_CNT << " CallState:CallState = " << callStateDataList.callStateDatalist.at(0).callState << MOST_LOG_ENDL;
        MOST_LOG_HIGH << LOG_TAG_BT_CNT << " CallState:CallType = " << callStateDataList.callStateDatalist.at(0).callType << MOST_LOG_ENDL;
    }
*/
// 2014.7.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_BT_CNT << " CallState_Status --> : List Count = " << callStateDataList.listCount 
	<<" CallState = " << callStateDataList.callStateDatalist.at(0).callState << " CallType = " 	
	<< callStateDataList.callStateDatalist.at(0).callType << MOST_LOG_ENDL;

    return m_CallState->UpdateShadows();
}
/*
 *******************************************************************
 * Function:DialNumberResultIBOX
 *
 * Description :Function called by the Bluetooth controller to send the result for
 *              dialnumber request from ibox.
 *
 * Parameters :
 *    state            rw   It holds the state of the result message (resultAck/ProcessingAck)
 *    senderHandler    rw   It holds the unique value for the dial number request
 *    callId           rw   It holds the callId from IBOX.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CBluetoothFblockControl::DialNumberResultIBOX(const uint state, const uint senderHandle, const uint callID, const uint deviceID)
{
/*	
    MOST_LOG_SIGNAL << LOG_TAG_BT_CNT << " DialNumberResultIBOX API called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " senderHandle : " << senderHandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " state : " << state << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " callID : " << callID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " deviceID : " << deviceID << MOST_LOG_ENDL;
*/
// 2014.7.22, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_BT_CNT << " DialNumber_ResultAck --> :  senderHandle= " 
    << senderHandle    << " state = " << state << " callID = " << callID << " deviceID = " << deviceID <<  MOST_LOG_ENDL;

    bool retVal = false;
    if(state == HMI_PROCESSING_ACK)
    {
        uint16_t sender_Handle = senderHandle;
        retVal = m_DialNumber->ProcessingAck(deviceID,sender_Handle);
    }
    else if(state == HMI_RESULT_ACK)
    {
        m_DialNumber->SenderHandleValue(senderHandle);
        m_DialNumber->CallIdValue(callID);
        retVal = m_DialNumber->ResultAck(deviceID);
    }

    return retVal;
}
/*
 *******************************************************************
 * Function:SetBTStatus
 *
 * Description :Function called by the Bluetooth controller to when user sets the BT mode.
 *
 * Parameters :
 *    btMode     rw   It holds the BT mode value set by user.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CBluetoothFblockControl::SetBTStatus(const uint btMode)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_BT_CNT << " SetBTStatus API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " BTStatus : BTMode " << btMode << MOST_LOG_ENDL;
*/
// 2014.7.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_BT_CNT << " BTStatus_Status --> : BTStatus = " << btMode << MOST_LOG_ENDL;	

    m_BTStatus->Value((CBTStatus::TBTMode)btMode);
    return m_BTStatus->UpdateShadows();
}
/*
 *******************************************************************
 * Function:SetUserMode
 *
 * Description :Function called by the Bluetooth controller to when user sets the user mode.
 *
 * Parameters :
 *    usermode     rw   It holds the value set by user for BT.
 *    micOff       rw   It holds the callId from IBOX.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CBluetoothFblockControl::SetUserMode(const uint usermode,const bool micOff)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_BT_CNT << " SetUserMode API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " UserMode : " << usermode << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " micOff : " << micOff << MOST_LOG_ENDL;
*/
// 2014.7.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_BT_CNT << " UserMode_Status --> : UserMode = " << usermode
	<< " micOff = " << micOff << MOST_LOG_ENDL;	
	
    m_UserMode->UserModeValue((CUserMode::TUserMode)usermode);
    m_UserMode->MicOffValue(micOff);
    return m_UserMode->UpdateShadows();
}
/*
 *******************************************************************
 * Function:HangupCallResult
 *
 * Description :Function called by the Bluetooth controller to send the result of hangup request from IBOX
 *
 * Parameters :
 *    senderHandle     rw   It holds the value set by request message
 *    callId           rw   It holds the callId from IBOX.
 *    deviceID         rw   It holds the device ID of requesting device.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CBluetoothFblockControl::HangupCallResult(const uint senderHandle, const uint callId, const uint deviceID)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_BT_CNT << " HangupCallResult API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " HangupCallResult:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " HangupCallResult:Call ID = " << callId << MOST_LOG_ENDL;
*/
// 2014.7.22, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_BT_CNT << " HangupCall_ResultAck --> : SenderHandle = " <<  senderHandle
	<< " CallID = " << callId <<  MOST_LOG_ENDL;

    m_HangupCall->SenderHandleValue(senderHandle);
    m_HangupCall->CallIdValue(callId);
    return m_HangupCall->ResultAck(deviceID);
}
/*
 *******************************************************************
 * Function:StartHangupCall
 *
 * Description :Function called by the handler for HangupCall property when IBOX sends the StartResultAck
 *              message for HangupCall.
 *
 * Parameters :
 *    payload            rw   It holds the payload data of hangupCall request message
 *    payloadLength      rw   It holds the length of the payload data
 *    sourceDeviceID     rw   It holds the device ID of requesting device.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CBluetoothFblockControl::StartHangupCall(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_BT_CNT << " HangupCallResult Signal got received :: MOSTMSG_RESULT_ACK" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " HangupCallResult:SenderHandle = " << m_HangupCall->SenderHandleValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " HangupCallResult:Call ID = " << m_HangupCall->CallIdValue() << MOST_LOG_ENDL;
*/

// 2014.7.22, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_BT_CNT << " HangupCall_StartResultAck <-- : SenderHandle = "
    << m_HangupCall->SenderHandleValue() << " CallID = " << m_HangupCall->CallIdValue() << MOST_LOG_ENDL;

    m_HangupCall->ParseStartResultAck(payloadLength,payload);
    emit StartHangupCall(m_HangupCall->SenderHandleValue(),
                         m_HangupCall->CallIdValue(),
                         sourceDeviceID);
}
/*
 *******************************************************************
 * Function: CHangUpCallHandler
 *
 * Description :This is constructor of CHangUpCallHandler class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CBluetoothFblockControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CHangUpCallHandler::CHangUpCallHandler(CBluetoothFblockControl *context)
{
    m_BluetoothFblockControlContext = context;
}

/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CHangUpCallHandler::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_BluetoothFblockControlContext->HangUpCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CHangUpCallHandler::Start(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_BluetoothFblockControlContext->HangUpCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CHangUpCallHandler::Abort(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_BluetoothFblockControlContext->HangUpCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CHangUpCallHandler::StartResult(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_BluetoothFblockControlContext->HangUpCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck operation.
 *              Updates Start Result Ack to Bluetooth FBlock control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CHangUpCallHandler::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_BluetoothFblockControlContext->HangUpCallHandler(sourceDeviceID,payloadLength,payload,STATUS_STARTRESULTACK);
}
/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CHangUpCallHandler::AbortAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_BluetoothFblockControlContext->HangUpCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CHangUpCallHandler::StartAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_BluetoothFblockControlContext->HangUpCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function: CDialNumberHandlerBT
 *
 * Description :This is constructor of CDialNumberHandlerBT class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CBluetoothFblockControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CDialNumberHandlerBT::CDialNumberHandlerBT(CBluetoothFblockControl *context)
{
     m_BluetoothFblockControlContext = context;
}
/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CDialNumberHandlerBT::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_BluetoothFblockControlContext->DialNumberRequestIBOXSlot(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CDialNumberHandlerBT::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_BluetoothFblockControlContext->DialNumberRequestIBOXSlot(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CDialNumberHandlerBT::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_BluetoothFblockControlContext->DialNumberRequestIBOXSlot(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CDialNumberHandlerBT::StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_BluetoothFblockControlContext->DialNumberRequestIBOXSlot(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck operation.
 *              Updates Start Result Ack to Bluetooth FBlock control for DialNumber property.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CDialNumberHandlerBT::StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_BluetoothFblockControlContext->DialNumberRequestIBOXSlot(sourceDeviceID,payloadLength,payload,STATUS_STARTRESULTACK);
}
/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CDialNumberHandlerBT::AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_BluetoothFblockControlContext->DialNumberRequestIBOXSlot(sourceDeviceID,payloadLength,payload,STATUS_ABORTACK);
}
/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CDialNumberHandlerBT::StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_BluetoothFblockControlContext->DialNumberRequestIBOXSlot(sourceDeviceID,payloadLength,payload,STATUS_STARTACK);
}
/*
 *******************************************************************
 * Function:DialNumberRequestIBOXSlot
 *
 * Description :Function called by the handler for DialNumber property when IBOX sends the StartResultAck
 *              message.
 *
 * Parameters :
 *    payload            rw   It holds the payload data of hangupCall request message
 *    payloadLength      rw   It holds the length of the payload data
 *    sourceDeviceID     rw   It holds the device ID of requesting device.
 *    state              rw   It holds the type of call (StartResultAck/StartAck).
 *
 * Returns : none
 *
 *******************************************************************
 */
void CBluetoothFblockControl::DialNumberRequestIBOXSlot(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
    const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
    uint8_t* teleNumString = NULL;
    uint32_t teleNumStringLength = 0;
    uint16_t senderHandle = 0;
    uint mState = 0;

    switch(state)
    {
        case STATUS_STARTRESULTACK:
            m_DialNumber->ParseStartResultAck(payloadLength,payload);
            senderHandle = m_DialNumber->SenderHandleValue();
            m_DialNumber->TelNumberString(&pStringEncoding,&teleNumString,&teleNumStringLength);
            mState = HMI_STARTRESULT_ACK;
            break;
        case STATUS_STARTACK:
            m_DialNumber->ParseStartAck(payloadLength,payload);
            senderHandle = m_DialNumber->SenderHandleValue();
            m_DialNumber->TelNumberString(&pStringEncoding,&teleNumString,&teleNumStringLength);
            mState = HMI_START_ACK;
            break;
        case STATUS_UNDEFINED:
            break;
        default:
            break;
    }
    QString telephoneNum = QString::fromUtf8((const char *)teleNumString,teleNumStringLength);

/*	
    MOST_LOG_SIGNAL << LOG_TAG_BT_CNT << " DialNumberrequest from IBOX " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " senderHandle : " << senderHandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " telephoneNumber : " << telephoneNum.toAscii().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " sourceDeviceID : " << sourceDeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_BT_CNT << " state : " << state << MOST_LOG_ENDL;
*/    
// 2014.07.22, for log, hyunoyng.sung
    MOST_LOG_SIGNAL << LOG_TAG_BT_CNT << " DialNumber_StartResultAck <-- : SenderHandle = " 
	<< senderHandle    << " telephoneNumber = " << telephoneNum.toAscii().data() << MOST_LOG_ENDL;
	
    emit DialNumberRequestIBOX(senderHandle,telephoneNum,sourceDeviceID);
}
/*
 *******************************************************************
 * Function:UserModeSlot
 *
 * Description :Function called by the listener for UserMode property when IBOX sets the property.
 *
 * Parameters :
 *    none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CBluetoothFblockControl::UserModeSlot()
{    
//    MOST_LOG_SIGNAL << LOG_TAG_BT_CNT << " UserModeSlot is invoked from ibox " << MOST_LOG_ENDL;

    uint userMode = 0;
    bool micOFF = 0;
    userMode = m_UserMode->UserModeValue();
    micOFF = m_UserMode->MicOffValue();

// 2014.07.22, for log, hyunoyng.sung
    MOST_LOG_SIGNAL << LOG_TAG_BT_CNT << " UserMode_Set <-- : UserMode = " << userMode
	<< " MicOff = " << micOFF <<  MOST_LOG_ENDL;	
	
    emit UserMode(userMode,micOFF);
}
/*
 *******************************************************************
 * Function:CUserModeListenerBT
 *
 * Description :constructor of the listener calss CUserModeListenerBT.
 *
 * Parameters :
 *    context   rw  It holds the instance of CBluetoothFblockControl
 *
 * Returns : none
 *
 *******************************************************************
 */
CUserModeListenerBT::CUserModeListenerBT(CBluetoothFblockControl *context)
{
    m_BluetoothFblockControlContext = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description :Method invoked by ACS Daemon when property UserMode
 *               is changed by IBOX
 *
 * Parameters :
 *    none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CUserModeListenerBT::OnChange()
{
    m_BluetoothFblockControlContext->UserModeSlot();
}
/*
 *******************************************************************
 * Function:HangUpCallHandler
 *
 * Description :Function called by the handler for HangupCall method when IBOX sends the StartResultAck
 *              message.
 *
 * Parameters :
 *    payload            rw   It holds the payload data of hangupCall request message
 *    payloadLength      rw   It holds the length of the payload data
 *    sourceDeviceID     rw   It holds the device ID of requesting device.
 *    state              rw   It holds the type of call (StartResultAck/StartAck).
 *
 * Returns : none
 *
 *******************************************************************
 */
void CBluetoothFblockControl::HangUpCallHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
    switch(state)
    {
        case STATUS_STARTRESULTACK:
            StartHangupCall(sourceDeviceID,payloadLength,payload);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            break;
    }
}
