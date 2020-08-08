/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Venkatesh Aeturi                                 *
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
 *                  SpeechRecognition Fblock control, responsible *
 *                  to create instance of and Shadow(0x01)        *
 *                  configure it and add to most stack.           *
 *                  Listeners and handlers will be part of this   *
 *                  class.                                        *
 *                  Communicates with ACS daemon                  *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 29 Aug 2011  Venkatesh Aeturi          Draft version           *
 * 15 Sep 2011  Amit Koparde              Added Debug Statement   *
 * 06 Feb 2012  Neelima Khedad           Updated the extraction of data  QbyteArray  from QByteArray::fromrawdata()
 * 08 Feb 2011  Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 ******************************************************************
 */

#include "DHAVN_MOSTManager_SpeechRecognitionFblockControl.h"
//#include "DHAVN_MOSTManager_SpeechRecognitionFBlock.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"

/*
 *******************************************************************
 * Function:      CSpeechRecognitionFblockControl
 *
 * Description :  This is constructor of CSpeechRecognitionFblockControl class.
 *
 * Parameters :
 *    context     rw   It holds the instance of CMarshaledACSApplication.
 *
 * Returns :      none
 *
 *******************************************************************
 */

CSpeechRecognitionFblockControl::CSpeechRecognitionFblockControl(CMarshaledACSApplication *context)
    : CControlThreadBase(context, LOG_TAG_SPREC_CNT)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_SpeechRecognitionFblock = new CSpeechRecognition(0x01);
    m_pMOSTElement = m_SpeechRecognitionFblock;

    m_TTSData = new CTextToSpeech (m_SpeechRecognitionFblock);
    m_CMethodHandler = new CSpeechHandler(this);

    m_ManualInput = new CManualInput(m_SpeechRecognitionFblock);
}

/*
 *******************************************************************
 * Function:      ~CSpeechRecognitionFblockControl
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

CSpeechRecognitionFblockControl::~CSpeechRecognitionFblockControl()
{
    delete m_SpeechRecognitionFblock;

    delete m_TTSData;
    delete m_CMethodHandler;

    delete m_ManualInput;
}

/*
 *******************************************************************
 * Function:     Initialize
 *
 * Description : Function initializes the MOST elements to SpeechRecognition
 *              FBlock and adds to MOST.
 *
 * Parameters :none
 *
 *
 * Returns :    true or false
 *
 *******************************************************************
 */

bool CSpeechRecognitionFblockControl::Initialize()
{
    if(m_SpeechRecognitionFblock->InitializeMOSTElement())
    {
        m_TTSData->SetHandler(m_CMethodHandler);
        m_Initialized = true;
    }

    return m_Initialized;
}

bool CSpeechRecognitionFblockControl::FBlockVisible()
{
    return GetIsRegistered();
}

/*
 *******************************************************************
 * Function:TtsResult
 *
 * Description :Function called by the SpeechRecognition controller to set the ttsResult.
 *
 * Parameters :
 *    senderHandle    rw   It holds the senderHandle set by the user. It is unsigned word value.
 *
 *    length          rw   It holds the length set by the user. It is unsigned word value.
 *
 *    sourceDeviceID  rw   It holds the device id of the source.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSpeechRecognitionFblockControl::SetTTSResult(const uint senderHandle, const uint length, const uint deviceid, const uint state)
{   

    if(state == SPEECH_RESULT_ACK)
    {
        m_TTSData->SenderHandleValue(senderHandle);
        m_TTSData->LengthValue(length);
// 2014.08.11, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_SPREC_CNT << " TextToSpeech_ResultAck --> : senderHandle = " 
	<< senderHandle << " Length = " << length << MOST_LOG_ENDL;	
/*		
        MOST_LOG_SIGNAL << LOG_TAG_SPREC_CNT << "SetTTSResult API got called :: SPEECH_RESULT_ACK" << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_SPREC_CNT << " SetTTSResult:Sender Handle = " << senderHandle << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_SPREC_CNT << " SetTTSResult:Length = " << length << MOST_LOG_ENDL;
*/
        return m_TTSData->ResultAck(deviceid);
    }
    else if(state == SPEECH_PROCESSING_ACK)
    {
        k2l::acs::protocols::most::DeviceID_t deviceID = deviceid;
        uint16_t handle = senderHandle;
/*
        MOST_LOG_SIGNAL << LOG_TAG_SPREC_CNT << "SetTTSResult API got called :: SPEECH_PROCESSING_ACK" << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_SPREC_CNT << " SetTTSResult:Sender Handle = " << senderHandle << MOST_LOG_ENDL;
*/
        return m_TTSData->ProcessingAck(deviceID,handle);
    }

    return false;
}
/*
 *******************************************************************
 * Function:SetManualInput
 *
 * Description :Function called by the SpeechRecognition controller to set the ttsResult.
 *
 * Parameters :
 *    inputTarget    rw   It holds the inputTarget set by the user.
 *
 *    inputKeyWord   rw   It holds the inputKeyWord set by the user.
 *
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSpeechRecognitionFblockControl::SetManualInput(const uint inputTarget, const QString inputKeyWord)
{
// 2014.08.11, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_SPREC_CNT << " ManualInput_Status --> : InputTarget = " 
	<< inputTarget << " InputKeyword = " << inputKeyWord.toAscii().data() << MOST_LOG_ENDL;	
/*
    MOST_LOG_SIGNAL << LOG_TAG_SPREC_CNT << " SetManualInput API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SPREC_CNT << " SetManualInput::InputTarget " << inputTarget << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SPREC_CNT << " SetManualInput::InputKeyword " << inputKeyWord.toAscii().data() << MOST_LOG_ENDL;
*/    
    m_ManualInput->InputTargetValue((CManualInput::TInputTarget)inputTarget);
    m_ManualInput->InputKeywordString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                     reinterpret_cast<const uint8_t*>(inputKeyWord.toUtf8().data()),
                                     inputKeyWord.length()+1);
    return m_ManualInput->UpdateShadows();

}

/*
 *******************************************************************
 * Function:TTSDataHandler
 *
 * Description :Function called by the SpeechRecognition controller.
 *
 * Parameters :
 *    payloadLength   rw   It holds the payloadLength value.
 *
 *    payload         rw   It holds the payload value.
 *
 *    sourceDeviceID  rw   It holds the device id of the source.
 *
 * Returns : none
 *
 * Emits: TTSData Signal
 *
 *******************************************************************
 */

void CSpeechRecognitionFblockControl::TTSDataHandler(const quint32 sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uint state)
{

    if (state == SPEECH_START_ACK)
    {
//        MOST_LOG_SIGNAL << LOG_TAG_SPREC_CNT << "TTSDataHandler signal for StartResultACK got received" << MOST_LOG_ENDL;

        m_TTSData->ParseStartResultAck(payloadLength, payload);
        const uint8_t * ttsData = m_TTSData->TTSDataValue();
        uint length = m_TTSData->LengthValue();
        QByteArray data((const char *)ttsData,length);		//hyunyong.sung 2013.2.18
// 2014.08.11, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_SPREC_CNT << " TextToSpeech_StartResultAck <-- : senderhandle = " 
	<< (uint)m_TTSData->SenderHandleValue() << " Length = " << length << " TTSData = " 
	<< data.toHex().data() << MOST_LOG_ENDL;	
/*
        MOST_LOG_INFO << LOG_TAG_SPREC_CNT << " StartTTSData:Sender Handle = " << (uint)m_TTSData->SenderHandleValue() << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_SPREC_CNT << " StartTTSData:Length = " << length << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_SPREC_CNT << " StartTTSData:TTS Data(Byte Array as Hex) = " << data.toHex().data() << MOST_LOG_ENDL;
*/
        emit StartTTSData(sourceDeviceID,m_TTSData->SenderHandleValue(), m_TTSData->LengthValue(),data);
    }

    if (state == SPEECH_ABORT_ACK)
    {
//        MOST_LOG_SIGNAL << LOG_TAG_SPREC_CNT << "TTSDataHandler signal for AbortACK got received" << MOST_LOG_ENDL;

        m_TTSData->ParseAbortAck(payloadLength,payload);

        MOST_LOG_INFO << LOG_TAG_SPREC_CNT << " AbortTTS:Sender Handle = " << (uint)m_TTSData->SenderHandleValue() << MOST_LOG_ENDL;

        emit AbortTTS(sourceDeviceID,m_TTSData->SenderHandleValue());
    }
}

/*
 *******************************************************************
 * Function:CSpeechHandler
 *
 * Description :Is a Constructor.
 *
 * Parameters :
 *    context     rw   It holds the instance of CMarshaledACSApplication.
 *
 * Returns : none
 *
 *******************************************************************
 */

CSpeechHandler::CSpeechHandler(CSpeechRecognitionFblockControl *context)
{
    m_CSpeechRecognitionFblockControlContext = context;
}

/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :
 *
 * Parameters :
 *    sourceDeviceID    rw   It holds the sourceDeviceID value.
 *
 *    payloadLength     rw   It holds the payloadLength value.
 *
 *    payload           rw   It holds the payload value.
 *
 * Returns : none
 *
 *******************************************************************
 */

void CSpeechHandler::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload)
{   
    m_CSpeechRecognitionFblockControlContext->SpeechHandler(sourceDeviceID,payloadLength,payload,STATUS_STARTRESULTACK);
}

/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :
 *
 * Parameters :
 *    sourceDeviceID    rw   It holds the sourceDeviceID value.
 *
 *    payloadLength     rw   It holds the payloadLength value.
 *
 *    payload           rw   It holds the payload value.
 *
 * Returns : none
 *
 *******************************************************************
 */

void CSpeechHandler::StartResult(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload)
{
    m_CSpeechRecognitionFblockControlContext->SpeechHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}


/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :
 *
 * Parameters :
 *    sourceDeviceID    rw   It holds the sourceDeviceID value.
 *
 *    payloadLength     rw   It holds the payloadLength value.
 *
 *    payload           rw   It holds the payload value.
 *
 * Returns : none
 *
 *******************************************************************
 */

void CSpeechHandler::AbortAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_CSpeechRecognitionFblockControlContext->SpeechHandler(sourceDeviceID,payloadLength,payload,STATUS_ABORTACK);
}

/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :
 *
 * Parameters :
 *    sourceDeviceID    rw   It holds the sourceDeviceID value.
 *
 *    payloadLength     rw   It holds the payloadLength value.
 *
 *    payload           rw   It holds the payload value.
 *
 * Returns : none
 *
 *******************************************************************
 */

void CSpeechHandler::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_CSpeechRecognitionFblockControlContext->SpeechHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :
 *
 * Parameters :
 *    sourceDeviceID    rw   It holds the sourceDeviceID value.
 *
 *    payloadLength     rw   It holds the payloadLength value.
 *
 *    payload           rw   It holds the payload value.
 *
 * Returns : none
 *
 *******************************************************************
 */

void CSpeechHandler:: StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_CSpeechRecognitionFblockControlContext->SpeechHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :
 *
 * Parameters :
 *    sourceDeviceID    rw   It holds the sourceDeviceID value.
 *
 *    payloadLength     rw   It holds the payloadLength value.
 *
 *    payload           rw   It holds the payload value.
 *
 * Returns : none
 *
 *******************************************************************
 */

void CSpeechHandler::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_CSpeechRecognitionFblockControlContext->SpeechHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :
 *
 * Parameters :
 *    sourceDeviceID    rw   It holds the sourceDeviceID value.
 *
 *    payloadLength     rw   It holds the payloadLength value.
 *
 *    payload           rw   It holds the payload value.
 *
 * Returns : none
 *
 *******************************************************************
 */

void CSpeechHandler::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_CSpeechRecognitionFblockControlContext->SpeechHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}


void CSpeechRecognitionFblockControl::SpeechHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
    switch(state)
    {
        case STATUS_ABORTACK:
            TTSDataHandler(sourceDeviceID,payloadLength, payload,SPEECH_ABORT_ACK);
            break;

        case STATUS_STARTRESULTACK:
            TTSDataHandler(sourceDeviceID,payloadLength, payload,SPEECH_START_ACK);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            break;
    }
}
