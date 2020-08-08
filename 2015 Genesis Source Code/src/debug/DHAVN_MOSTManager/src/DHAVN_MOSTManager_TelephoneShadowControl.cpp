/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Neelima Khedad                                   *
 * DATE       :  06 Sep 2011                                      *
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
 *                  Telephone Shadow control, responsible to      *
 *                  create instance of and Shadow configure it    *
 *                  and add to most stack.                        *
 *                  If any listeners will be part of this class.  *
 *                  Communicates with ACS daemon                  *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 06 Sep 2011  Neelima Khedad           Draft version            *
 * 09 Sep 2011  Neelima Khedad           Added parameter index in the function call callDurationList.append
 *                                       in CallDuration method definition
 * 15 Sep 2011  Amit Koparde             Added Debug Statement    *
 * 27 Oct 2011  Neelima Khedad           Removed Signal NewSMS and added MissedCallStatus for Telephone
 * 26 Jan 2012  Neelima Khedad           Fixed Coverity Error.Initialized all structure varibles before using.
 * 08 Feb 2011  Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 10 Feb 2011  Neelima Khedad           Added new API and SIgnal for property AcceptCall and HangUpCall
 ******************************************************************
 */

#include "DHAVN_MOSTManager_TelephoneShadowControl.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"

/*
 *******************************************************************
 * Function:CTelephoneShadowControl
 *
 * Description :This is constructor of CTelephoneShadowControl class.
 *              creates the instances for all the defined parameters
 * Parameters :
 *    context    rw   It holds the instance of CMarshaledACSApplication.
 *
 * Returns :
 *
 *******************************************************************
 */

CTelephoneShadowControl ::CTelephoneShadowControl(CMarshaledACSApplication *context)
    : CControlThreadBase(context, LOG_TAG_TELE_CNT)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    /** Telephone Shadow Block Instance*/
    m_TelephoneShadow = new CTelephone(0x01);
    m_pMOSTElement = m_TelephoneShadow;

    /** Instantiating Listeners and Handlers for proeprties of telephone */
    m_CurrentNumber = new CCurrentNumber(m_TelephoneShadow);
    m_CurrentNumberListener = new CCurrentNumberListener(this);

    m_CallState = new CCallState(m_TelephoneShadow);
    m_CallStateListener = new CCallStateListener(this);

    m_CallDuration = new CCallDuration(m_TelephoneShadow);
    m_CallDurationListener = new CCallDurationListener(this);

    m_ModemLevel = new CModemLevel(m_TelephoneShadow);
    m_ModemLevelListener = new CModemLevelListener(this);

    m_RecipientInfo = new CRecipientInfo(m_TelephoneShadow);
    m_RecipientInfoListener = new CRecipientInfoListener(this);

    m_MissedCallStatus = new CMissedCallStatus(m_TelephoneShadow);
    m_MissedCallStatusListener = new CMissedCallStatusListener(this);

    m_UserMode = new CUserMode(m_TelephoneShadow);
    m_UserModeListener = new CUserModeListener(this);

    m_ModemDataStatus = new CModemDataStatus(m_TelephoneShadow);
    m_ModemDataStatusListener = new CModemDataStatusListener(this);

    m_CollectPBook = new CCollectPBook(m_TelephoneShadow);
    m_CollectPBookHandler = new CCollectPBookHandler(this);

    m_PBookStatus = new CPBookStatus(m_TelephoneShadow);
    m_PBookStatusListener = new CPBookStatusListener(this);

    m_CallHistoryInfo = new CCallHistoryInfo(m_TelephoneShadow);
    m_CallHistoryInfoListener = new CCallHistoryInfoListener(this);

    m_DialNumber = new CDialNumber(m_TelephoneShadow);
    m_DialNumberHandler = new CDialNumberHandler(this);

    m_AcceptCall = new CAcceptCall(m_TelephoneShadow);
    m_AcceptCallHandler = new CAcceptCallHandler(this);

    m_HangupCall = new CHangupCall(m_TelephoneShadow);
    m_HangupCallHandler = new CHangupCallHandler(this);

    m_TMService = new CTMService(m_TelephoneShadow);
    m_TMServiceListener = new CTMServiceListener(this);

    m_CallHistoryStatus = new CCallHistoryStatus(m_TelephoneShadow);
    m_CallHistoryStatusListener = new CCallHistoryStatusListener(this);
}

/*
 *******************************************************************
 * Function:~CTelephoneShadowControl
 *
 * Description :Is a Destructor,removes all the registered listeners.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CTelephoneShadowControl ::~CTelephoneShadowControl()
{
    RemoveListeners();

    delete m_HangupCallHandler;
    delete m_HangupCall;

    delete m_AcceptCallHandler;
    delete m_AcceptCall;

    delete m_DialNumberHandler;
    delete m_DialNumber;

    delete m_CallHistoryInfoListener;
    delete m_CallHistoryInfo;

    delete m_PBookStatusListener;
    delete m_PBookStatus;

    delete m_CollectPBookHandler;
    delete m_CollectPBook;

    delete m_ModemDataStatusListener;
    delete m_ModemDataStatus;

    delete m_UserModeListener;
    delete m_UserMode;

    delete m_MissedCallStatusListener;
    delete m_MissedCallStatus;

    delete m_RecipientInfoListener;
    delete m_RecipientInfo;

    delete m_ModemLevelListener;
    delete m_ModemLevel;

    delete m_CallDurationListener;
    delete m_CallDuration;

    delete m_CallStateListener;
    delete m_CallState;

    delete m_CurrentNumberListener;
    delete m_CurrentNumber;

    delete m_TelephoneShadow;

    delete m_TMService;
    delete m_TMServiceListener;

    delete m_CallHistoryStatus;
    delete m_CallHistoryStatusListener;
}

/*
 *******************************************************************
 * Function:Initialize
 *
 * Description :Function initializes the MOST elements of Telephone
 *              Shadow and adds to MOST stack.
 *
 * Parameters : None
 *
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CTelephoneShadowControl::Initialize()
{
     /** Initialze notification for all parameters on Telephone Shadow */
    if(m_TelephoneShadow->InitializeMOSTElement())
    {
        SetListeners();

        /** Setting the handlers for required parameters */
        m_CollectPBook->SetHandler(m_CollectPBookHandler);
        m_DialNumber->SetHandler(m_DialNumberHandler);
        m_AcceptCall->SetHandler(m_AcceptCallHandler);
        m_HangupCall->SetHandler(m_HangupCallHandler);

        m_Initialized = true;
    }

    return m_Initialized;
}

/*
 *******************************************************************
 * Function:SetListeners
 *
 * Description : Sets the listeners for all the properties defined
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CTelephoneShadowControl::SetListeners()
{
    m_PBookStatus->SetListener(m_PBookStatusListener);
    m_CurrentNumber->SetListener(m_CurrentNumberListener);    
    m_CallState->SetListener(m_CallStateListener);
    m_CallDuration->SetListener(m_CallDurationListener);
    m_ModemLevel->SetListener(m_ModemLevelListener);
    m_RecipientInfo->SetListener(m_RecipientInfoListener);
    m_MissedCallStatus->SetListener(m_MissedCallStatusListener);
    m_UserMode->SetListener(m_UserModeListener);
    m_ModemDataStatus->SetListener(m_ModemDataStatusListener);
    m_CollectPBook->SetListener(m_CallHistoryInfoListener);
    m_CallHistoryInfo->SetListener(m_CallHistoryInfoListener);
    m_TMService->SetListener(m_TMServiceListener);
    m_CallHistoryStatus->SetListener(m_CallHistoryStatusListener);
}

/*
 *******************************************************************
 * Function:RemoveListeners
 *
 * Description : Removes the listeners for all the properties registered.
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CTelephoneShadowControl::RemoveListeners()
{
    m_PBookStatus->RemoveListener(m_PBookStatusListener);
    m_CurrentNumber->RemoveListener(m_CurrentNumberListener);
    m_CallState->RemoveListener(m_CallStateListener);
    m_CallDuration->RemoveListener(m_CallDurationListener);
    m_ModemLevel->RemoveListener(m_ModemLevelListener);
    m_RecipientInfo->RemoveListener(m_RecipientInfoListener);
    m_MissedCallStatus->RemoveListener(m_MissedCallStatusListener);
    m_UserMode->RemoveListener(m_UserModeListener);
    m_ModemDataStatus->RemoveListener(m_ModemDataStatusListener);
    m_CollectPBook->RemoveListener(m_CallHistoryInfoListener);
    m_CallHistoryInfo->RemoveListener(m_CallHistoryInfoListener);
    m_TMService->RemoveListener(m_TMServiceListener);
    m_CallHistoryStatus->RemoveListener(m_CallHistoryStatusListener);
}

bool CTelephoneShadowControl::FBlockVisible()
{
    return m_TelephoneShadow->FunctionBlockVisible();
}

/*
 *******************************************************************
 * Function:CurrentNumber
 *
 * Description : Invoked by the Listener for CurrentNumber of Telephone
 *               Sends the signal to CtelephoneAdaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CTelephoneShadowControl::CurrentNumber()
{
//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CurrentNumber Signal got received " << MOST_LOG_ENDL;

    const k2l::acs::protocols::most::string::IStringEncoding* stringEncoding = NULL;
    uint8_t* currentNumString = NULL;
    uint32_t stringLength = 0;

    /** Get the string for Current number */
    if(m_CurrentNumber->String(&stringEncoding,&currentNumString,&stringLength)){

// 2014.08.12, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CurrentNumber_Status <-- : Number = " 
	<< reinterpret_cast<const char*>(currentNumString) << MOST_LOG_ENDL;	
//        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CurrentNumber:Telephone Number = " << reinterpret_cast<const char*>(currentNumString) << MOST_LOG_ENDL;

	/** Send the Siignal to the adaptor */
        emit CurrentNumber(reinterpret_cast<const char*>(currentNumString));
    }
}

/*
 *******************************************************************
 * Function:CallState
 *
 * Description : Invoked by the Listener for CallState of Telephone
 *               Sends the signal to CTelephoneAdaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CTelephoneShadowControl::CallState()
{
//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CallState Signal got received " << MOST_LOG_ENDL;

    SCallStateDataList callStateDataList;
    callStateDataList.listCount = 0;

    /** Store the Call State data in the list */
    for (uint index= 0; index < m_CallState->Size() ;index++)
    {
        struct SCallStateData callStateData;

        callStateData.callState = m_CallState->DataCallStateValue(index);
        callStateData.callType = m_CallState->DataCallTypeValue(index);
        callStateDataList.callStateDatalist.append(callStateData);
        callStateDataList.listCount++;
    }

//    MOST_LOG_HIGH << LOG_TAG_TELE_CNT << " CallState:ListCount = " << callStateDataList.listCount << MOST_LOG_ENDL;
// 2014.08.12, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CallState_Status <-- : ListCount = " << callStateDataList.listCount
	<< " callstate = " << callStateDataList.callStateDatalist.at(0).callState << " calltype = " 
	<< callStateDataList.callStateDatalist.at(0).callType <<  MOST_LOG_ENDL;	

	if(callStateDataList.listCount > 1)
	{
		for(uint i = 1; i < callStateDataList.listCount; i++)
		{
// 2014.08.12, for log, hyunyong.sung		
			MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CallState_Status <-- : CallState:CallState["<< i << "] = " 
			<< callStateDataList.callStateDatalist.at(i).callState << " CallState:CallType[" << i << "] = " 
			<< callStateDataList.callStateDatalist.at(i).callType << MOST_LOG_ENDL;
		}
	}
	
    /** Wrap the Structure List in QDbusVariant */
    QVariant callStateDataVar;
    QDBusVariant dBuscallStateDataVar;

    QDBusArgument callStateDataArg;
    callStateDataArg << callStateDataList;
    callStateDataVar.setValue(callStateDataArg);

    dBuscallStateDataVar.setVariant(callStateDataVar);

    /** Send the Signal to Telephone adaptor */
    emit CallState(dBuscallStateDataVar);
}

/*
 *******************************************************************
 * Function:CallDuration
 *
 * Description : Invoked by the Listener for CallDuration of Telephone
 *               Sends the signal to CTelephoneAdaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CTelephoneShadowControl::CallDuration()
{
//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CallDuration Signal got received " << MOST_LOG_ENDL;

    QList<ushort> callDurationList;

    /** Append the Data to QList */
    for (uint index =0; index < m_CallDuration->Size(); index++)
    {
        callDurationList.append(m_CallDuration->DataCallTimeValue(index));
    }   
/*
    if(callDurationList.count() > 0)
    {
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallDuration:CallTime[0] = " << callDurationList.at(0) << MOST_LOG_ENDL;
    }
*/
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CallDuration_Status <-- : CallTime = " 
	<< callDurationList.at(0) << MOST_LOG_ENDL;

    /** Wrap the Structure in QDbusVariant */
    QVariant callDurationDataVar;
    QDBusVariant dBuscallDurationDataVar;

    QDBusArgument callDurationDataArg;
    callDurationDataArg << callDurationList;
    callDurationDataVar.setValue(callDurationDataArg);

    dBuscallDurationDataVar.setVariant(callDurationDataVar);

    /** Send the Signal to IBOX adaptor */
    emit CallDuration(dBuscallDurationDataVar);
}

/*
 *******************************************************************
 * Function:ModemLevel
 *
 * Description : Invoked by the Listener for ModemLevel of Telephone
 *               Sends the signal to CTelephoneAdaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CTelephoneShadowControl::ModemLevel()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " ModemLevel Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " ModemLevel:Modem Level = " << (uint)m_ModemLevel->Value() << MOST_LOG_ENDL;
*/    
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " ModemLevel_Status <-- : modemlevel = " 
	<< (uint)m_ModemLevel->Value() << MOST_LOG_ENDL;
	
    emit ModemLevel(m_ModemLevel->Value());
}

/*
 *******************************************************************
 * Function:RecipientInfo
 *
 * Description : Invoked by the Listener for RecipientInfo of Telephone
 *               Sends the signal to CTelephoneAdaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CTelephoneShadowControl::RecipientInfo()
{
//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " RecipientInfo Signal got received " << MOST_LOG_ENDL;

    const k2l::acs::protocols::most::string::IStringEncoding* stringEncoding = NULL;
    uint8_t* recipientInfoString = NULL;
    uint32_t stringLength = 0;

    /** retrive the String  cantact value */
    if(m_RecipientInfo->NameString(&stringEncoding,&recipientInfoString,&stringLength)){
/*
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " RecipientInfo:Contact = " << (uint)m_RecipientInfo->ContactValue() << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " RecipientInfo:Name = " << reinterpret_cast<const char*>(recipientInfoString) << MOST_LOG_ENDL;
*/        
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " RecipientInfo_Status <-- : Contact = " 
	<< (uint)m_RecipientInfo->ContactValue() << " Name = " << reinterpret_cast<const char*>(recipientInfoString) << MOST_LOG_ENDL;
		
        QString name = QString::fromUtf8((const char *)recipientInfoString,stringLength);

        /** Send the signal */
        emit RecipientInfo(m_RecipientInfo->ContactValue(),name);
    }
}

/*
 *******************************************************************
 * Function:NewSMS
 *
 * Description : Invoked by the Listener for NewSMS of Telephone
 *               Sends the signal to CTelephoneAdaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CTelephoneShadowControl::MissedCallStatus()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " MissedCallStatus Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " MissedCallStatus:MSD Call Status = " << (uint)m_MissedCallStatus->MissedCallStatusValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " MissedCallStatus:MSD Call Count = " << (uint)m_MissedCallStatus->MissedCallCountValue() << MOST_LOG_ENDL;
*/
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " MissedCallStatus_Status <-- : missedcallstatus = " 
	<< (uint)m_MissedCallStatus->MissedCallStatusValue() << " missedcallcount = " << (uint)m_MissedCallStatus->MissedCallCountValue() << MOST_LOG_ENDL;

    emit MissedCallStatus(m_MissedCallStatus->MissedCallStatusValue(), m_MissedCallStatus->MissedCallCountValue());
}

/*
 *******************************************************************
 * Function:UserMode
 *
 * Description : Invoked by the Listener for UserMode of Telephone
 *               Sends the signal to CTelephoneAdaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CTelephoneShadowControl::UserMode()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " UserMode Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " UserMode:User Mode = " << m_UserMode->UserModeValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " UserMode:micOff = " << m_UserMode->MicOffValue() << MOST_LOG_ENDL;
*/
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " UserMode_Status <-- : usermode = " 
	<< m_UserMode->UserModeValue() << " micOff = " << m_UserMode->MicOffValue() << MOST_LOG_ENDL;	

    emit UserMode(m_UserMode->UserModeValue(),m_UserMode->MicOffValue());
}

/*
 *******************************************************************
 * Function:ModemDataStatus
 *
 * Description : Invoked by the Listener for ModemDataStatus of Telephone
 *               Sends the signal to CTelephoneAdaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CTelephoneShadowControl::ModemDataStatus()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " ModemDataStatus Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " ModemDataStatus:Modem Status = " << (uint)m_ModemDataStatus->ModemStatusValue() << MOST_LOG_ENDL;
*/    
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " ModemDataStatus_Status <-- : modemstatus = " 
	<< (uint)m_ModemDataStatus->ModemStatusValue() << MOST_LOG_ENDL;		
    emit ModemDataStatus(m_ModemDataStatus->ModemStatusValue());
}

/*
 *******************************************************************
 * Function:StartCollectPBook
 *
 * Description :Invoked by CTelephoneAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      pBookDataRequest     rw  Holds structure for Pbook data request
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */
bool CTelephoneShadowControl::StartCollectPBook(const uint senderHandle, const uint pBookType)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " StartCollectPBook API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " StartCollectPBook:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " StartCollectPBook:PBookType = " << pBookType << MOST_LOG_ENDL;
*/
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_HIGH << LOG_TAG_TELE_CNT << " CollectPBook_StartResultAck --> : SenderHandle = " 
	<< senderHandle << " PBookType = " << pBookType << MOST_LOG_ENDL;	

    m_CollectPBook->SenderHandleValue(senderHandle);
    m_CollectPBook->PBookTypeValue((CCollectPBook::TPBookType)pBookType);
    return m_CollectPBook->StartResultAck();
}


/*
 *******************************************************************
 * Function:StartDialNumber
 *
 * Description :Invoked by CTelephoneAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      dialNumData     rw  Holds structure for dial number request
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */

bool CTelephoneShadowControl::StartDialNumber(const SDialNumberData &dialNumData)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " StartDialNumber API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " StartDialNumber:SenderHandle = " << (uint)dialNumData.senderHandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " StartDialNumber:TelNumber = " << dialNumData.telNumber << MOST_LOG_ENDL;
*/
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_HIGH << LOG_TAG_TELE_CNT << " DialNumber_StartResultAck --> : SenderHandle = " 
	<< (uint)dialNumData.senderHandle << " TelNumber = " << dialNumData.telNumber << MOST_LOG_ENDL;	

    m_DialNumber->SenderHandleValue(dialNumData.senderHandle);

    /** Updates the parameter of StationNameInfo to IBox through ACS daemon */
    m_DialNumber->TelNumberString(
                &k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                reinterpret_cast<const uint8_t*>(dialNumData.telNumber.toUtf8().data()),
                dialNumData.telNumber.length()+1);

    return  m_DialNumber->StartResultAck();

}

/*
 *******************************************************************
 * Function:StartAcceptCall
 *
 * Description :Invoked by CTelephoneAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *    senderHandle  rw   holds the senderHandle value for Accept Call.
 *    callId        rw   holds the callId value for Accept Call.
 *
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */

bool CTelephoneShadowControl::StartAcceptCall(const uint senderHandle, const uint callId)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " StartAcceptCall API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " StartAcceptCall:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " StartAcceptCall:Call ID = " << callId << MOST_LOG_ENDL;
*/
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_HIGH << LOG_TAG_TELE_CNT << " AcceptCall_StartResultAck --> : SenderHandle = " 
	<< senderHandle << " callID = " << callId << MOST_LOG_ENDL;	

    m_AcceptCall->SenderHandleValue(senderHandle);
    m_AcceptCall->CallIdValue(callId);

    return m_AcceptCall->StartResultAck();
}

/*
 *******************************************************************
 * Function:StartHangupCall
 *
 * Description :Invoked by CTelephoneAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *    senderHandle  rw   holds the senderHandle value for Hangup Call.
 *    callId        rw   holds the callId value for Hangup Call.
 *
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */

bool CTelephoneShadowControl::StartHangupCall(const uint senderHandle, const uint callId)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " StartHangupCall API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " StartAcceptCall:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " StartAcceptCall:Call ID = " << callId << MOST_LOG_ENDL;
*/
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_HIGH << LOG_TAG_TELE_CNT << " HangupCall_StartResultAck --> : SenderHandle = " 
	<< senderHandle << " callID = " << callId << MOST_LOG_ENDL;	

    m_HangupCall->SenderHandleValue(senderHandle);
    m_HangupCall->CallIdValue(callId);

    return m_HangupCall->StartResultAck();
}
/*
 *******************************************************************
 * Function:GetModemDataStatus
 *
 * Description :Invoked by CTelephoneAdaptor to get ModemDataStatus over ACS daemon
 *               from Ibox
 * Parameters :
 *    none
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CTelephoneShadowControl::GetModemDataStatus(void)
{
    MOST_LOG_HIGH << LOG_TAG_TELE_CNT << " ModemDataStatus_Get --> " << MOST_LOG_ENDL;
    return m_ModemDataStatus->Get();
}

/*
 *******************************************************************
 * Function:CollectPBookResult
 *
 * Description : Invoked by the Handler for CollectPBookResult of Telephone
 *               Sends the signal to CTelephoneAdaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CTelephoneShadowControl::CollectPBookResult(const uint state, const uint32_t payloadLength, const uint8_t *payload)
{
    /** Initialize the Structure variables */
    SCollectPBook pBookResultData;
    pBookResultData.state = state;    
    pBookResultData.senderHandle = 0;
    pBookResultData.pBookType = 0xFF;
    pBookResultData.arrayCount = 0;
    pBookResultData.pBookDataList.clear();

    if(state == TELEPHONE_PROCESSING_ACK)
    {
        uint16_t senderHandle;
        m_CollectPBook->ParseSenderHandle(payloadLength,payload,&senderHandle);
        pBookResultData.senderHandle = senderHandle;
/*
        MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CollectPBookResult Signal got received :: MOSTMSG_PROCESSING_ACK" << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:Senderhandle = " << pBookResultData.senderHandle << MOST_LOG_ENDL;
*/        
    }
    else if(state == TELEPHONE_RESULT_ACK)
    {
//        MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CollectPBookResult Signal got received :: MOSTMSG_RESULT_ACK" << MOST_LOG_ENDL;

        if (m_CollectPBook->ParseResultAck(payloadLength, payload))
        {
            uint32_t index = 0;

            const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
            uint8_t *pString = NULL;
            uint32_t stringLength = 0;

            while (m_CollectPBook->PBookDataDataType().LoadNext())
            {                
                struct SPBookData pBookDataList;                                

                m_CollectPBook->NameString(&pEncoding, &pString, &stringLength);
                pBookDataList.name.append(reinterpret_cast<const char*>(pString));

                pBookDataList.contact1 = m_CollectPBook->Contact1Value();

                m_CollectPBook->TelNumber1String(&pEncoding, &pString, &stringLength);
                pBookDataList.teleNum1.append(reinterpret_cast<const char*>(pString));

                pBookDataList.contact2 = m_CollectPBook->Contact2Value();

                m_CollectPBook->TelNumber2String(&pEncoding, &pString, &stringLength);
                pBookDataList.teleNum2.append(reinterpret_cast<const char*>(pString));

                pBookDataList.contact3 = m_CollectPBook->Contact3Value();

                m_CollectPBook->TelNumber3String(&pEncoding, &pString, &stringLength);
                pBookDataList.teleNum3.append(reinterpret_cast<const char*>(pString));

                pBookDataList.contact4 = m_CollectPBook->Contact4Value();

                m_CollectPBook->TelNumber4String(&pEncoding, &pString, &stringLength);
                pBookDataList.teleNum4.append(reinterpret_cast<const char*>(pString));

                pBookDataList.contact5 = m_CollectPBook->Contact5Value();

                m_CollectPBook->TelNumber5String(&pEncoding, &pString, &stringLength);
                pBookDataList.teleNum5.append(reinterpret_cast<const char*>(pString));

                pBookResultData.pBookDataList.append(pBookDataList);
                ++index;
                pBookResultData.arrayCount = index;
            }            

            pBookResultData.senderHandle = m_CollectPBook->SenderHandleValue();           
/*
            MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:State = " << pBookResultData.state << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:SenderHandle = " << pBookResultData.senderHandle << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:pBookType = " << pBookResultData.pBookType << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:ArrayCount = " << pBookResultData.arrayCount << MOST_LOG_ENDL;
*/
// 2014.08.12, for log, hyunyong.sung		
		MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CollectPBook_ResultAck <-- : SenderHandle = " 
		<< pBookResultData.senderHandle << " pBookType = " << pBookResultData.pBookType 
		<< " ArrayCount = " << pBookResultData.arrayCount << MOST_LOG_ENDL;	

            if(pBookResultData.arrayCount > 0)
            {
// 2014.08.12, for log, hyunyong.sung		
			MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CollectPBook_ResultAck <-- : Name[0] = " 
			<< pBookResultData.pBookDataList.at(0).name.data() << " Contact1[0] = " << pBookResultData.pBookDataList.at(0).contact1
			<< " telenum1[0] = " << pBookResultData.pBookDataList.at(0).teleNum1.data() << " contact2[0] = " 
			<< pBookResultData.pBookDataList.at(0).contact2 << " telenum2[0] = " << pBookResultData.pBookDataList.at(0).teleNum2.data()
			<< " contact3[0] = " << pBookResultData.pBookDataList.at(0).contact3 << " telenum3[0] = " 
			<< pBookResultData.pBookDataList.at(0).teleNum3.data() << " contact4[0] = " << pBookResultData.pBookDataList.at(0).contact4
			<< " telenum4[0] = " << pBookResultData.pBookDataList.at(0).teleNum4.data() << " contact5[0] = "
			<< pBookResultData.pBookDataList.at(0).contact5 << " telenum5[0] = " << pBookResultData.pBookDataList.at(0).teleNum5.data()
			<< MOST_LOG_ENDL;	            
/*
                MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:Name[0] = " << pBookResultData.pBookDataList.at(0).name.data() << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:Contact1[0] = " << pBookResultData.pBookDataList.at(0).contact1 << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:TeleNum1[0] = " << pBookResultData.pBookDataList.at(0).teleNum1.data() << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:Contact2[0] = " << pBookResultData.pBookDataList.at(0).contact2 << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:TeleNum2[0] = " << pBookResultData.pBookDataList.at(0).teleNum2.data() << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:Contact3[0] = " << pBookResultData.pBookDataList.at(0).contact3 << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:TeleNum3[0] = " << pBookResultData.pBookDataList.at(0).teleNum3.data() << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:Contact4[0] = " << pBookResultData.pBookDataList.at(0).contact4 << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:TeleNum4[0] = " << pBookResultData.pBookDataList.at(0).teleNum4.data() << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:Contact5[0] = " << pBookResultData.pBookDataList.at(0).contact5 << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CollectPBookResult:TeleNum5[0] = " << pBookResultData.pBookDataList.at(0).teleNum5.data() << MOST_LOG_ENDL;
*/              
            }            

         }
    }

    qDBusRegisterMetaType<SPBookData>();
    qDBusRegisterMetaType<SCollectPBook>();

    QDBusArgument pBookResultDataArg;
    pBookResultDataArg << pBookResultData;
    QVariant pBookResultDataVar;
    pBookResultDataVar.setValue(pBookResultDataArg);
    QDBusVariant dBuspBookResultDataVar;
    dBuspBookResultDataVar.setVariant(pBookResultDataVar);

    emit CollectPBookResult(dBuspBookResultDataVar);
}

/*
 *******************************************************************
 * Function:SetPBookStatus
 *
 * Description :Invoked by CTelephoneAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      pbookstatus     rw  PBook Information Status
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */
/*
// Deleted in Fcat v1.22.6
bool CTelephoneShadowControl::SetPBookStatus(const uint pbookstatus)
{
    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " SetPBookStatus API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " SetPBookStatus:pbookstatus = " << pbookstatus << MOST_LOG_ENDL;
    m_PBookStatus->PBookStatusValue((CPBookStatus::TPBookStatus)pbookstatus);
    return m_PBookStatus->Set();
}
*/

bool CTelephoneShadowControl::GetPBookStatus(void)
{
//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " GetPBookStatus API got called " << MOST_LOG_ENDL;
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_HIGH << LOG_TAG_TELE_CNT << " PBookStatus_Get -->" << MOST_LOG_ENDL;		

    return m_PBookStatus->Get();
}

bool CTelephoneShadowControl::GetModemLevel(void)
{
//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " GetModemLevel API got called " << MOST_LOG_ENDL;
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_HIGH << LOG_TAG_TELE_CNT << " ModemLevel_Get -->" << MOST_LOG_ENDL;		
    return m_ModemLevel->Get();
}

bool CTelephoneShadowControl::GetTMSerivce(void)
{
//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " GetTMSerivce API got called " << MOST_LOG_ENDL;

// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_HIGH << LOG_TAG_TELE_CNT << " TMService_Get -->" << MOST_LOG_ENDL;		

    return m_TMService->Get();
}

bool CTelephoneShadowControl::GetCallHistoryStatus(void)
{
//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " GetCallHistoryStatus API got called " << MOST_LOG_ENDL;
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_HIGH << LOG_TAG_TELE_CNT << " CallHistoryStatus_Get -->" << MOST_LOG_ENDL;			

    return m_CallHistoryStatus->Get();
}


/*
 *******************************************************************
 * Function:PBookStatus
 *
 * Description :Invoked by CPBookStatusListener when received status message from IBOX
 * Parameters :
 *      pbookstatus     rw  PBook Information Status
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */
void CTelephoneShadowControl::PBookStatus()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " PBookStatus Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " PBookStatus:TotalPBEntry = " << (ushort)m_PBookStatus->TotalPBEntryValue() << MOST_LOG_ENDL;
*/    
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " PBookStatus_Status <-- : TotalPBEntry = " 
	<< (ushort)m_PBookStatus->TotalPBEntryValue() << MOST_LOG_ENDL;			
	
    emit PBookStatus(m_PBookStatus->TotalPBEntryValue());
}

/*
 *******************************************************************
 * Function:CallHistoryInfoRequest
 *
 * Description :Invoked by CTelephoneAdaptor to  request for Call history info
 * Parameters :
 *
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */

bool CTelephoneShadowControl::CallHistoryInfoRequest()
{
//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CallHistoryInfoRequest API got called " << MOST_LOG_ENDL;
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_HIGH << LOG_TAG_TELE_CNT << " CallHistoryInfo_Get --> " << MOST_LOG_ENDL;			

    return m_CallHistoryInfo->Get();
}

/*
 *******************************************************************
 * Function:CallHistoryInfoRequestStatus
 *
 * Description :Invoked by CTelephoneAdaptor to  send result for Call history info
 * Parameters :
 *
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */

void CTelephoneShadowControl::CallHistoryInfoRequestStatus()
{
//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus Signal got received " << MOST_LOG_ENDL;

    SCallHistoryInfoList infoList;

    const k2l::acs::protocols::most::string::IStringEncoding* encoding = NULL;
    uint8_t* data = NULL;
    uint32_t length = 0;

    infoList.listCount = 0;    
    /** Update the List with all thed call history info */
    for (uint index= 0; index < m_CallHistoryInfo->Size() ;index++)
    {
        struct SCallHistoryInfo callHistoryInfo;

        callHistoryInfo.mode = m_CallHistoryInfo->DataModeValue(index);
        callHistoryInfo.callDirection = m_CallHistoryInfo->DataCallDirectionValue(index);

        if (m_CallHistoryInfo->DataNameString(index,&encoding,&data,&length))
            callHistoryInfo.name.append(reinterpret_cast<const char*>(data));
        if (m_CallHistoryInfo->DataTelNumberString(index,&encoding,&data,&length))
            callHistoryInfo.telNumber.append(reinterpret_cast<const char*>(data));
        if (m_CallHistoryInfo->DataDateNTimeString(index,&encoding,&data,&length))
            callHistoryInfo.dateNTime.append(reinterpret_cast<const char*>(data));

        infoList.infoList.append(callHistoryInfo);
        infoList.listCount++;
    }

//    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus:ListCount = " << infoList.listCount << MOST_LOG_ENDL;
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CallHistoryInfo_Status <-- : ListCount = " << infoList.listCount << MOST_LOG_ENDL;			

#if 1
    if(infoList.listCount > 0)
    {
// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CallHistoryInfo_Status <-- : Mode = " 
	<<  infoList.infoList.at(0).mode << " CallDirection = " << infoList.infoList.at(0).callDirection << " Name = " 
	<<  infoList.infoList.at(0).name << " TelNumber = " <<  infoList.infoList.at(0).telNumber << " DateNTime = "
	<<  infoList.infoList.at(0).dateNTime << MOST_LOG_ENDL;			    
/*
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus:Mode = " << infoList.infoList.at(0).mode << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus:CallDirection = " << infoList.infoList.at(0).callDirection << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus:Name = " << infoList.infoList.at(0).name << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus:TelNumber = " << infoList.infoList.at(0).telNumber << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus:DateNTime = " << infoList.infoList.at(0).dateNTime << MOST_LOG_ENDL;
*/        
    }
#else
    for(int i=0; i<infoList.listCount; i++)
    {
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus:Mode[" << i << "] = " << infoList.infoList.at(i).mode << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus:CallDirection[" << i << "] = " << infoList.infoList.at(i).callDirection << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus:Name[" << i << "] = " << infoList.infoList.at(i).name << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus:TelNumber[" << i << "] = " << infoList.infoList.at(i).telNumber << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryInfoRequestStatus:DateNTime[" << i << "] = " << infoList.infoList.at(i).dateNTime << MOST_LOG_ENDL;
    }
#endif

    /** Wrap the Structure in QDbusVariant */
    qDBusRegisterMetaType<SCallHistoryInfo>();
    qDBusRegisterMetaType<SCallHistoryInfoList>();

    QVariant callHistoryInfoVar;
    QDBusVariant dBuscallHistoryInfoVar;

    QDBusArgument callHistoryInfoArg;
    callHistoryInfoArg << infoList;
    callHistoryInfoVar.setValue(callHistoryInfoArg);

    dBuscallHistoryInfoVar.setVariant(callHistoryInfoVar);

    /** Send the Signal to Telephone adaptor */
    emit CallHistoryInfo(dBuscallHistoryInfoVar);
}

/*
 *******************************************************************
 * Function:DialNumberResult
 *
 * Description :Invoked by CTelephoneAdaptor to  send result for Dial number
 * Parameters :
 *
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */
void CTelephoneShadowControl::DialNumberResult(const uint state, const uint32_t payloadLength,const uint8_t *payload)
{    
    /** Initialize the Structure variables */
    SDialNumberData dialNumData;
    dialNumData.state = state;
    dialNumData.senderHandle = 0;
    dialNumData.telNumber.clear();
    dialNumData.callId = 0;
    dialNumData.errorInfo.clear();
    dialNumData.errorCode = 0;

    if(state == TELEPHONE_PROCESSING_ACK)
    {
//        MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " DialNumberResult Signal got received :: MOSTMSG_PROCESSING_ACK" << MOST_LOG_ENDL;
        uint16_t senderHandle = 0;
         m_DialNumber->ParseSenderHandle(payloadLength,payload,&senderHandle);
         dialNumData.senderHandle = senderHandle;
//         MOST_LOG_INFO << LOG_TAG_TELE_CNT << " DialNumberResult:SenderHandle = " << dialNumData.senderHandle << MOST_LOG_ENDL;
    }
    else if(state == TELEPHONE_RESULT_ACK)
    {
//        MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " DialNumberResult Signal got received :: MOSTMSG_RESULT_ACK" << MOST_LOG_ENDL;

        m_DialNumber->ParseResultAck(payloadLength,payload);
        dialNumData.senderHandle = m_DialNumber->SenderHandleValue();
        dialNumData.callId = m_DialNumber->CallIdValue();
/*
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " DialNumberResult:SenderHandle = " << dialNumData.senderHandle << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " DialNumberResult:CallId = " << dialNumData.callId << MOST_LOG_ENDL;
*/        
// 2014.08.12, for log, hyunyong.sung		
		MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " DialNumber_ResultAck <-- : SenderHandle = " 
		<< dialNumData.senderHandle << " CallId = " << dialNumData.callId << MOST_LOG_ENDL;					
    }
    else if(state == TELEPHONE_ERROR_ACK)
    {
        //Parsing error ack
        k2l::acs::base::CBigEndianStreamReader reader(payload, payloadLength);
        uint16_t senderHandle = 0;
        uint8_t errorCode = 0;

        reader.Read16(&senderHandle);
        reader.Read8(&errorCode);

        uint8_t *errorInfo = new uint8_t[reader.Capacity() - reader.Position()];
        reader.ReadBlock(errorInfo, reader.Capacity() - reader.Position());

        dialNumData.senderHandle = senderHandle;
        dialNumData.errorCode = errorCode;
        dialNumData.errorInfo = reinterpret_cast<char *>(errorInfo);

// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " DialNumber_ErrorAck <-- : ErrorInfo = " 
	<< dialNumData.errorInfo << " ErrorCode = " << dialNumData.errorInfo << MOST_LOG_ENDL;
/*	
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " DialNumberResult:SenderHandle = " << dialNumData.senderHandle << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " DialNumberResult:ErrorInfo = " << dialNumData.errorInfo << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " DialNumberResult:ErrorCode = " << dialNumData.errorCode << MOST_LOG_ENDL;
*/        
    }

    /** Wrap the Structure in QDbusVariant */
    QVariant dialNumResultDataVar;
    QDBusVariant dBusdialNumResultDataVar;

    QDBusArgument dialNumResultDataArg;
    dialNumResultDataArg << dialNumData;
    dialNumResultDataVar.setValue(dialNumResultDataArg);

    dBusdialNumResultDataVar.setVariant(dialNumResultDataVar);

    /** Emit the signal to telephoneadaptor */
    emit DialNumberResult(dBusdialNumResultDataVar);
}

/*
 *******************************************************************
 * Function:AcceptCallResult
 *
 * Description :Invoked by CTelephoneAdaptor to  send result for Accept Call
 * Parameters :
 *
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */

void CTelephoneShadowControl::AcceptCallResult(const uint state, const uint32_t payloadLength, const uint8_t *payload)
{    
    if(state == TELEPHONE_RESULT_ACK)
    {
//        MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " AcceptCallResult Signal got received :: MOSTMSG_RESULT_ACK" << MOST_LOG_ENDL;

        m_AcceptCall->ParseResultAck(payloadLength,payload);

// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " AcceptCall_ResultAck <-- : SenderHandle = " 
	<< m_AcceptCall->SenderHandleValue() << " Callid = " << m_AcceptCall->CallIdValue() << MOST_LOG_ENDL;
/*
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " AcceptCallResult:SenderHandle = " << m_AcceptCall->SenderHandleValue() << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << " AcceptCallResult:Call ID = " << m_AcceptCall->CallIdValue() << MOST_LOG_ENDL;
*/
        emit AcceptCallResult(m_AcceptCall->SenderHandleValue(),
                              m_AcceptCall->CallIdValue());
    }
}

/*
 *******************************************************************
 * Function:HangupCallResult
 *
 * Description :Invoked by CTelephoneAdaptor to  send result for Hangup Call
 * Parameters :
 *
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */

void CTelephoneShadowControl::HangupCallResult(const uint state, const uint32_t payloadLength, const uint8_t *payload)
{   
    if(state == TELEPHONE_RESULT_ACK)
    {
//        MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " HangupCallResult Signal got received :: MOSTMSG_RESULT_ACK" << MOST_LOG_ENDL;

        m_HangupCall->ParseResultAck(payloadLength,payload);

// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " HangupCall_ResultAck <-- : SenderHandle = " 
	<< m_HangupCall->SenderHandleValue() << " Callid = " << m_HangupCall->CallIdValue() << MOST_LOG_ENDL;
/*
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << "HangupCallResult:SenderHandle = " << m_HangupCall->SenderHandleValue() << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_TELE_CNT << "HangupCallResult:Call ID = " << m_HangupCall->CallIdValue() << MOST_LOG_ENDL;
*/
        emit HangupCallResult(m_HangupCall->SenderHandleValue(), m_HangupCall->CallIdValue());
    }
}

/*
 *******************************************************************
 * Function:CCollectPBookHandler
 *
 * Description :Constructor for CCollectPBookHandler.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

CCollectPBookHandler::CCollectPBookHandler(CTelephoneShadowControl *context)
{
    m_TelephoneShadowControlContext = context;
}

void CCollectPBookHandler::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->CollectPBookHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
void CCollectPBookHandler::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->CollectPBookHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
void CCollectPBookHandler::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->CollectPBookHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
void CCollectPBookHandler::Processing(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->CollectPBookHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
void CCollectPBookHandler::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->CollectPBookHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CCollectPBookHandler::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
     m_TelephoneShadowControlContext->CollectPBookHandler(sourceDeviceID,payloadLength,payload,STATUS_RESULTACK);
}

/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CCollectPBookHandler::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->CollectPBookHandler(sourceDeviceID,payloadLength,payload,STATUS_PROCESSINGACK);
}

/*
 *******************************************************************
 * Function:CPBookStatusListener
 *
 * Description :Constructor for CPBookStatusListener.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CPBookStatusListener::CPBookStatusListener(CTelephoneShadowControl *context)
{
     m_TelephoneFblock = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property PBookStatus
 *               is changed.
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

 void CPBookStatusListener::OnChange()
 {
    m_TelephoneFblock->PBookStatus();
 }

 /*
  *******************************************************************
  * Function:CCallHistoryInfoListener
  *
  * Description :Constructor for CCallHistoryInfoListener.
  *              Stores the context of CTelephoneShadowControl
  * Parameters : context
  *
  * Returns :None
  *
  *******************************************************************
  */

 CCallHistoryInfoListener::CCallHistoryInfoListener(CTelephoneShadowControl *context)
 {
     m_TelephoneFblock = context;
 }

 /*
  *******************************************************************
  * Function:OnChange
  *
  * Description : Method invoked by ACS Daemon when property CallHistoryInfo
  *               is changed.
  * Parameters : context
  *
  * Returns :None
  *
  *******************************************************************
  */

  void CCallHistoryInfoListener::OnChange()
  {
    m_TelephoneFblock->CallHistoryInfoRequestStatus();
  }

  /*
   *******************************************************************
   * Function:CCurrentNumberListener
   *
   * Description :Constructor for CCurrentNumberListener.
   *              Stores the context of CTelephoneShadowControl
   * Parameters : context
   *
   * Returns :None
   *
   *******************************************************************
   */

 CCurrentNumberListener::CCurrentNumberListener(CTelephoneShadowControl *context)
 {
     m_TelephoneFblock = context;
 }

 /*
  *******************************************************************
  * Function:OnChange
  *
  * Description : Method invoked by ACS Daemon when property CurrentNumber
  *               is changed.
  * Parameters : context
  *
  * Returns :None
  *
  *******************************************************************
  */

void CCurrentNumberListener::OnChange()
{
    m_TelephoneFblock->CurrentNumber();
}

/*
 *******************************************************************
 * Function:CCallStateListener
 *
 * Description :Constructor for CCallStateListener.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

CCallStateListener::CCallStateListener(CTelephoneShadowControl *context)
{
    m_TelephoneFblock = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property CallState
 *               is changed.
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

void CCallStateListener::OnChange()
{
    m_TelephoneFblock->CallState();
}

/*
 *******************************************************************
 * Function:CCallDurationListener
 *
 * Description :Constructor for CCallDurationListener.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

CCallDurationListener::CCallDurationListener(CTelephoneShadowControl *context)
{
    m_TelephoneFblock = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property CallDuration
 *               is changed.
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

void CCallDurationListener::OnChange()
{
    m_TelephoneFblock->CallDuration();
}

/*
 *******************************************************************
 * Function:CModemLevelListener
 *
 * Description :Constructor for CModemLevelListener.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

CModemLevelListener::CModemLevelListener(CTelephoneShadowControl *context)
{
    m_TelephoneFblock = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property ModemLevel
 *               is changed.
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

void CModemLevelListener::OnChange()
{
    m_TelephoneFblock->ModemLevel();
}

/*
 *******************************************************************
 * Function:CRecipientInfoListener
 *
 * Description :Constructor for CRecipientInfoListener.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

CRecipientInfoListener::CRecipientInfoListener(CTelephoneShadowControl *context)
{
    m_TelephoneFblock = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property RecipientInfo
 *               is changed.
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

void CRecipientInfoListener::OnChange()
{
    m_TelephoneFblock->RecipientInfo();
}

/*
 *******************************************************************
 * Function:CMissedCallStatusListener
 *
 * Description :Constructor for CMissedCallStatusListener.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

CMissedCallStatusListener::CMissedCallStatusListener(CTelephoneShadowControl *context)
{
    m_TelephoneFblock = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property NewSMS
 *               is changed.
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

void CMissedCallStatusListener::OnChange()
{
    m_TelephoneFblock->MissedCallStatus();
}

/*
 *******************************************************************
 * Function:CUserModeListener
 *
 * Description :Constructor for CUserModeListener.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

CUserModeListener::CUserModeListener(CTelephoneShadowControl *context)
{
    m_TelephoneFblock = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property UserMode
 *               is changed.
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

void CUserModeListener::OnChange()
{
    m_TelephoneFblock->UserMode();
}

/*
 *******************************************************************
 * Function:CModemDataStatusListener
 *
 * Description :Constructor for CModemDataStatusListener.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

CModemDataStatusListener::CModemDataStatusListener(CTelephoneShadowControl *context)
{
    m_TelephoneFblock = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property ModemDataStatus
 *               is changed.
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

void CModemDataStatusListener::OnChange()
{
    m_TelephoneFblock->ModemDataStatus();
}


/*
 *******************************************************************
 * Function:CDialNumberHandler
 *
 * Description :Constructor for CDialNumberHandler.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

CDialNumberHandler::CDialNumberHandler(CTelephoneShadowControl *context)
{
    m_TelephoneShadowControlContext = context;
}


void CDialNumberHandler::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->DialNumberHandler(sourceDeviceID,payloadLength,payload,STATUS_ERRORACK);

}

void CDialNumberHandler::Processing(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->DialNumberHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CDialNumberHandler::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->DialNumberHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CDialNumberHandler::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->DialNumberHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CDialNumberHandler::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->DialNumberHandler(sourceDeviceID,payloadLength,payload,STATUS_PROCESSINGACK);
}

/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CDialNumberHandler::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->DialNumberHandler(sourceDeviceID,payloadLength,payload,STATUS_RESULTACK);

}

/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CDialNumberHandler::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->DialNumberHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:CAcceptCallHandler
 *
 * Description :Constructor for CAcceptCallHandler.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

CAcceptCallHandler::CAcceptCallHandler(CTelephoneShadowControl *context)
{
    m_TelephoneShadowControlContext = context;
}

void CAcceptCallHandler::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->AcceptCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CAcceptCallHandler::ErrorAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->AcceptCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CAcceptCallHandler::ResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->AcceptCallHandler(sourceDeviceID,payloadLength,payload,STATUS_RESULTACK);
}

void CAcceptCallHandler::Interface(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->AcceptCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CAcceptCallHandler::Processing(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->AcceptCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CAcceptCallHandler::ProcessingAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->AcceptCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CAcceptCallHandler::Error(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->AcceptCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:CHangupCallHandler
 *
 * Description :Constructor for CHangupCallHandler.
 *              Stores the context of CTelephoneShadowControl
 * Parameters : context
 *
 * Returns :None
 *
 *******************************************************************
 */

CHangupCallHandler::CHangupCallHandler(CTelephoneShadowControl *context)
{
    m_TelephoneShadowControlContext = context;
}

void CHangupCallHandler::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_TelephoneShadowControlContext->HangupCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CHangupCallHandler::ErrorAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->HangupCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CHangupCallHandler::ResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->HangupCallHandler(sourceDeviceID,payloadLength,payload,STATUS_RESULTACK);
}

void CHangupCallHandler::Interface(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->HangupCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CHangupCallHandler::Processing(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->HangupCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CHangupCallHandler::ProcessingAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->HangupCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CHangupCallHandler::Error(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_TelephoneShadowControlContext->HangupCallHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

CTMServiceListener::CTMServiceListener(CTelephoneShadowControl *context)
{
    m_TelephoneFblock = context;
}

void CTMServiceListener::OnChange()
{
    m_TelephoneFblock->TMServiceStatus();
}

CCallHistoryStatusListener::CCallHistoryStatusListener(CTelephoneShadowControl *context)
{
    m_TelephoneFblock = context;
}

void CCallHistoryStatusListener::OnChange()
{
    m_TelephoneFblock->CallHistoryStatus();
}

void CTelephoneShadowControl::CollectPBookHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
    switch(state)
    {
        case STATUS_PROCESSINGACK:
            CollectPBookResult(TELEPHONE_PROCESSING_ACK,payloadLength,payload);
            break;

        case STATUS_RESULTACK:
            CollectPBookResult(TELEPHONE_RESULT_ACK,payloadLength,payload);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            MOST_LOG_INFO << LOG_TAG_TELE_CNT << "Source Device ID :: " << sourceDeviceID << MOST_LOG_ENDL;
            break;
    }
}

void CTelephoneShadowControl::DialNumberHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
    switch(state)
    {
        case STATUS_PROCESSINGACK:
            DialNumberResult(TELEPHONE_PROCESSING_ACK,payloadLength,payload);
            break;

        case STATUS_RESULTACK:
            DialNumberResult(TELEPHONE_RESULT_ACK,payloadLength,payload);
            break;

        case STATUS_ERRORACK:
            DialNumberResult(TELEPHONE_ERROR_ACK,payloadLength,payload);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            MOST_LOG_INFO << LOG_TAG_TELE_CNT << "Source Device ID :: " << sourceDeviceID << MOST_LOG_ENDL;
            break;
    }
}

void CTelephoneShadowControl::AcceptCallHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
    switch(state)
    {
        case STATUS_RESULTACK:
            AcceptCallResult(TELEPHONE_RESULT_ACK,payloadLength,payload);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            MOST_LOG_INFO << LOG_TAG_TELE_CNT << "Source Device ID :: " << sourceDeviceID << MOST_LOG_ENDL;
            break;
    }
}

void CTelephoneShadowControl::HangupCallHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
    switch(state)
    {
        case STATUS_RESULTACK:
            HangupCallResult(TELEPHONE_RESULT_ACK,payloadLength,payload);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            MOST_LOG_INFO << LOG_TAG_TELE_CNT << "Source Device ID :: " << sourceDeviceID << MOST_LOG_ENDL;
            break;
    }
}

void CTelephoneShadowControl::TMServiceStatus()
{
    uint tmStatus = 0;
    uint provStatus = 0;

//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " TMServiceStatus Signal got received " << MOST_LOG_ENDL;

    tmStatus = m_TMService->TMServiceValue();
    provStatus = m_TMService->ProvStatusValue();

// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " TMService_Status <-- : TMService = " 
	<< tmStatus << " Provstatus = " << provStatus << MOST_LOG_ENDL;
/*	
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " TM Serivce Status = " << tmStatus << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " Provisioned Status = " << provStatus << MOST_LOG_ENDL;
*/
    emit TMServiceStatus(tmStatus, provStatus);
}


void CTelephoneShadowControl::CallHistoryStatus()
{
    uint status = 0;

//    MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CallHistoryStatus Signal got received " << MOST_LOG_ENDL;

    status = m_CallHistoryStatus->Value();

// 2014.08.12, for log, hyunyong.sung		
	MOST_LOG_SIGNAL << LOG_TAG_TELE_CNT << " CallHistoryStatus_Status <-- : CallHistoryStatus = " 
	<< status << MOST_LOG_ENDL;
/*		
    MOST_LOG_INFO << LOG_TAG_TELE_CNT << " CallHistoryStatus = " << status << MOST_LOG_ENDL;
*/
    emit CallHistoryStatus(status);
}
