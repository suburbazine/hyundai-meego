/*
 ******************************************************************
 *        COPYRIGHT  2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : Pandora Frame Validator                           *
 * PROGRAMMER : Snigdha Satapathy                                     *
 * DATE       : 04/12/2011                                        *
 * VERSION    : 0.1                                               *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module will create a thread and will be responsible for
 *  maintaining the request, validiting the request in terms
 *  of duplication where ever applicable, eg thumbup, thumb down.
 *  sending the request over internet. Waiting for the reply, if
 *  didn't receive the resend the request. On reception of valid
 *  response forward to decoder
 *
 *  This class is expected to be singleton
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  07/12/2011  0.1 Snigdha Satapathy   cpp file created
 *
 ******************************************************************
 */

#include <QDataStream>
#include <QMetaType>
#include <QDebug>

#include <QTime>
#include "DHAVN_PndrFrameValidator.h"
#include "DHAVN_PndrFrameTxRx.h"
#include "DHAVN_PndrFrameDecoder.h"
#include "DHAVN_PndrLinkConstants.h"
#include "DHAVN_PndrFrameEncoder.h"

//Static variables initialization
//int CPndrFrameValidator::m_nInstanceCounter = 0;

//CPndrFrameValidator* CPndrFrameValidator::m_pInstance = 0;

const int cTimeoutInterval = 750;
const int cMaxReqCount = 10;

/**
 *******************************************************************
 * Return single instance of FrameValidtor
 *
 * @param    None
 *
 * @return     None
 *
 *******************************************************************
 */
/*
CPndrFrameValidator* CPndrFrameValidator::GetInstance()
{
    USE_LOG_UTIL;
    LOG_INIT_V2("Pandora");

    LOG_TRACE<< "CPndrFrameValidator::GetInstance:Entry" <<LOG_ENDL;

    if(m_pInstance == NULL)
    {
        m_pInstance = new CPndrFrameValidator();
    }

    //The counter is incremented only if m_pInstance allocation is succesfull.
    if(m_pInstance)
    {
        m_nInstanceCounter++;
        LOG_TRACE<< "CPndrFrameValidator::m_nInstanceCounter = " << m_nInstanceCounter<<LOG_ENDL;

    }
    else
    {
        //LOG_INFO << "Validator Init Failed." << LOG_ENDL;
    }

    LOG_TRACE<< "CPndrFrameValidator::GetInstance:Exit" <<LOG_ENDL;

    return m_pInstance;
}
*/
/**
 *******************************************************************
 * Releases instance of FrameValidator
 *
 * @param    None
 *
 * @return     None
 *
 *******************************************************************
 */
/*
void CPndrFrameValidator::ReleaseInstance()
{
//    USE_LOG_UTIL;
//    LOG_INIT_V2("Pandora");
    LOG_TRACE<< "CPndrFrameValidator::ReleaseInstance:Entry" <<LOG_ENDL;
    LOG_TRACE<< "Validator Instance to be deleted is m_nInstanceCounter = " << m_nInstanceCounter <<LOG_ENDL;

    m_nInstanceCounter--;
    if (!m_nInstanceCounter)
    {
        LOG_TRACE<< "CPndrFrameValidator::deleted" <<LOG_ENDL;
        delete m_pInstance;
        m_pInstance = NULL;
    }
  //  LOG_TRACE<< "CPndrFrameValidator::ReleaseInstance:Exit" <<LOG_ENDL;

}
*/

/**
*******************************************************************
* Default constructor made private to make it singleton
*******************************************************************
*/
CPndrFrameValidator::CPndrFrameValidator()
{
    LOG_INIT_V2("Pandora");
    //LOG_TRACE<< "CPndrFrameValidator::CPndrFrameValidator:Entry" <<LOG_ENDL;

    //Get Decoder Instance
    //This shall not fail since GetInstance of decoder
    //validation is already done in state handler
    m_pFrameDecoder = NULL; //CPndrFrameDecoder::GetInstance();
    m_pActiveTxRx =NULL;
//    m_pActiveTxRx = CPndrFrameTxRx::GetInstance();

//    //connect to the frame received signal of Transport class
//    connect(m_pActiveTxRx,SIGNAL(DataReceived(const QByteArray&)),this,SLOT(OnDataReceived(const QByteArray&)));

    qRegisterMetaType<QByteArray>("QByteArray");
    //connect to the frame received signal of Transport class
    connect(this,SIGNAL(FrameReceived(const QByteArray)),this,SLOT(OnFrameReceived(const QByteArray)),
            Qt::QueuedConnection);
    //connect to the timeout signal of Timer
    m_ReplyWaitTimer.setInterval(cTimeoutInterval);
    //m_ReplyWaitTimer.setSingleShot(true);//RK modified
    connect(&m_ReplyWaitTimer,SIGNAL(timeout()),this,SLOT(OnTimeout()));


    m_nReqCounter = 0;
    m_bRequestPending = false;
    m_nReceivedFrameSeqNo = 3;//modified by esjang 2013.05.16 for certification 
    m_nReceivedACKSeqNo = 0; //added by esjang 2013.05.13 for certification #2.3 termination
    m_LastResponseId = 0;
    //LOG_TRACE<< "CPndrFrameValidator::CPndrFrameValidator:Exit" <<LOG_ENDL;

}

void CPndrFrameValidator::SetFrameTxRX(CPndrFrameTxRx*& inFrameTxRx)
{
    m_pActiveTxRx =inFrameTxRx;
    connect(m_pActiveTxRx,SIGNAL(DataReceived(const QByteArray&)),this,SLOT(OnDataReceived(const QByteArray&)));

}

 void CPndrFrameValidator::SetDecoder(CPndrFrameDecoder* inFrameDecoder)
 {
    m_pFrameDecoder = inFrameDecoder;
    //LOG_INFO<<"esjang 0510 connect receivedStatus() <-" << LOG_ENDL;
    connect(m_pFrameDecoder, SIGNAL(ReceivedStatus()), this, SIGNAL(ReceivedStatus())); // added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
    connect(m_pFrameDecoder, SIGNAL(ReceivedBranding()), this, SIGNAL(ReceivedBranding())); 
    //} modified by yongkyun.lee 2014-08-04 
    //LOG_INFO<<"esjang 0510 connect receivedStatus() ->" << LOG_ENDL;

 }

/**
 *******************************************************************
 * Destructor
 *******************************************************************
 */
CPndrFrameValidator::~CPndrFrameValidator()
{
    LOG_TRACE<< "CPndrFrameValidator::~CPndrFrameValidator:Entry" <<LOG_ENDL;

//    if(m_pFrameDecoder)
//    {
//        m_pFrameDecoder->ReleaseInstance();
//        m_pFrameDecoder = NULL;

//    }
    m_pFrameDecoder = NULL;

    if(m_pActiveTxRx)
    {
       // m_pActiveTxRx->ReleaseInstance();
        m_pActiveTxRx = NULL;
    }

    if( m_ReplyWaitTimer.isActive())
    {
        m_ReplyWaitTimer.stop();
    }


    m_RequestList.clear();
    LOG_TRACE<< "CPndrFrameValidator::~CPndrFrameValidator:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Send the data to selected transport
 *
 * @param[in]   inData: The data to be sent over selected
 *                      transport layer
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameValidator::Send(const QByteArray& inData,bool inAppendToQueue)
{
    //LOG_TRACE<< "CPndrFrameValidator::Send:Entry" <<LOG_ENDL;

    if(inAppendToQueue)
    {
        //Create a reqPacket object
        TSRequestPacket reqPacket;
        reqPacket.m_Packet = inData;
        //Append the packet to the request queue
        m_RequestList.append(reqPacket);
        //LOG_INFO << "VALIDATOR:: APPEND:: " << inData.toHex() << LOG_ENDL;
    }

    //If the prev. request is not pending, send it to Transport layer
    if(!m_bRequestPending)
    {
        //Set the request as pending
        m_bRequestPending = true;
        //Send the data to transport layer       
        LOG_HIGH<<"[PandoraLink] VALIDATOR::SEND & Start timer, [Time = "<< (QTime::currentTime()).toString("hh.mm.ss.zzz")<<"] packet = "<<inData.toHex().left(32)<<LOG_ENDL;
        m_pActiveTxRx->Send(inData);

        quint8 seqNo = m_pFrameDecoder->GetSequenceNumber(inData);
        if(m_nReceivedACKSeqNo == seqNo)
        {
             LOG_INFO<<"VALIDATOR::DATA Sending Seq and received last Ack is same AckSeq = " << seqNo<<LOG_ENDL;
        }
        m_nReceivedACKSeqNo = seqNo;

        //Start the 750 ms timer
       // LOG_INFO<<"Validator:: Wait timer Starts"<<LOG_ENDL;
        m_ReplyWaitTimer.start();

        //Increment the request counter
        m_nReqCounter++;
    }
    else
    {
       LOG_HIGH << "VALIDATOR::APPEND packet = " << inData.toHex().left(32) << LOG_ENDL;
    }
    //LOG_TRACE<< "CPndrFrameValidator::Send:Exit" <<LOG_ENDL;

}


/**
*******************************************************************
* Slot for signal emitted by transport layer
*
* @param[in]   inData: The data received from transport layer
*
* @return     None
*
*******************************************************************
*/
void CPndrFrameValidator::OnDataReceived(const QByteArray& inData)
{
    //LOG_SLOT<< "CPndrFrameValidator::OnDataReceived:Entry" <<LOG_ENDL;
    QByteArray receivedData(inData);
    QByteArray frameEndByte;
    frameEndByte.append(PNDR_FRAME_END);
    QByteArray frameStartByte;
    frameStartByte.append(PNDR_FRAME_START);
    m_FrameBuffer.append(receivedData);

    //Data Received contains both start and End byte
    //check if multiple frames have come and emit each frame
    int noOfFrames = m_FrameBuffer.count(frameEndByte);
    //LOG_INFO << "Validator::No of data frames received = " << noOfFrames << LOG_ENDL;

    int len = 0;
    int indexOfFrame = 0;
    while(noOfFrames)
    {
        int startIndex = m_FrameBuffer.indexOf(frameStartByte,indexOfFrame);
        len = m_FrameBuffer.indexOf(frameEndByte,indexOfFrame) + 1;
        QByteArray frameData(m_FrameBuffer.mid(startIndex,len - startIndex));
        //LOG_INFO << "frameData re " << frameData.toHex()<<LOG_ENDL;
        emit FrameReceived(frameData);
        indexOfFrame = len;
        noOfFrames--;
    }
    if(len > 0)
    {
        m_FrameBuffer.remove(0,len);
    }

    //LOG_SLOT<< "CPndrFrameValidator::OnDataReceived:Exit" <<LOG_ENDL;
}

/**
*******************************************************************
* Slot for signal emitted once a complete frame is received
*
* @param[in]   inData: The frame received
*
* @return     None
*
*******************************************************************
*/
void CPndrFrameValidator::OnFrameReceived(const QByteArray inData)
{
    //LOG_INFO << "CPndrFrameValidator::OnFrameReceived:Entry - " << inData.toHex().left(32) <<LOG_ENDL;

    QByteArray receivedFrame(inData);

    //Remove the escaping first
    m_pFrameDecoder->RemoveEscape(receivedFrame);

    //one complete frame is received at one go

    //Process any frame only if its a complete frame
    if(m_pFrameDecoder->IsFrameComplete(receivedFrame))
    {
        //If frame received is ACK
        if(m_pFrameDecoder->IsAckFrame(receivedFrame))
        {
            
    			//{ added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    			/*
    			//Get the payload
    			QByteArray payload = m_RequestList.at(0).m_Packet.mid(ePayloadStartBytePos, 1);
    			QDataStream payloadDs(payload);
    			payloadDs.setByteOrder(QDataStream::BigEndian);

    			//Read the payload to get the response Id
    			quint8 responseId;
    			payloadDs >> responseId;
    			//LOG_INFO << "responseId " <<responseId.toHex() << LOG_ENDL;
    			QByteArray respID(1, responseId);//esjang test 
                */
    			//} added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    			
                //If SeqNo is valid && not duplicate
               
                if(IsValidAck(receivedFrame) && IsValidCRC(receivedFrame))
                {
                    LOG_INFO<<"VALIDATOR::Wait timer stoped , Ack recived = " << receivedFrame.toHex().left(32)<<LOG_ENDL;

                    //{ added by esjang 2013.05.14 for certification #2.3 session termination.
                    //If it is not a duplicate frame
                    #ifdef DHAVN_PANDORA_TESTFRAMEWORK
                    if(m_pFrameDecoder->GetSequenceNumber(receivedFrame) == (m_nReceivedACKSeqNo ^1))
                    {

                        //if(receivedFramePNDR_GET_TRACK_ALBUM_ART)

            			//Get the payload
            			if(m_RequestList.count())
            			{
                			QByteArray payload = m_RequestList.at(0).m_Packet.mid(ePayloadStartBytePos, 1);
                			QDataStream payloadDs(payload);
                			payloadDs.setByteOrder(QDataStream::BigEndian);

                			//Read the payload to get the response Id
                			quint8 responseId;
                			payloadDs >> responseId;
                			//LOG_INFO << "responseId " <<responseId.toHex() << LOG_ENDL;
                			QByteArray respID(1, responseId);//esjang test 

    	                    if(m_pFrameDecoder->GetRequestIDNumber(m_RequestList.at(0).m_Packet) == PNDR_SESSION_START)
    	                    {
    	                        LOG_INFO<<"Validator::OnFrameReceived:: Valid ACK for session start: "<< respID.toHex().left(32) <<LOG_ENDL;
    	                        emit SessionStartAcked();
    	                    }
                            else if()(m_pFrameDecoder->GetRequestIDNumber(m_RequestList.at(0).m_Packet) == PNDR_SESSION_TERMINATE)
                            {
    	                        LOG_INFO<<"Validator::OnFrameReceived:: Valid ACK for session Terminate: "<< respID.toHex().left(32) <<LOG_ENDL;
    	                        emit SessionTerminateAcked();
                            }
                            //{ modified by yongkyun.lee 2014-08-25 for : Branding Image 
                            else if(m_pFrameDecoder->GetRequestIDNumber(m_RequestList.at(0).m_Packet) == PNDR_RETURN_BRANDING_IMAGE_SEGMENT) 
                            {
                                emit NextBrandingImage(); 
                            }
                            //} modified by yongkyun.lee 2014-08-14 
//    	                    else
//    	                    {
//    	                        LOG_INFO<<"esjang 0510 Validator::OnFrameReceived:: Valid ACK for : "<< respID.toHex() <<LOG_ENDL;
//    	                    }
                        }

                        //Pass it to decoder
                        m_nReceivedACKSeqNo = m_pFrameDecoder->GetSequenceNumber(receivedFrame);
                    }
                    #else
                    LOG_HIGH <<"VALIDATOR::Previous Received ACK Seq = " << m_nReceivedACKSeqNo << ", Curr Ack frame = "
                             << receivedFrame.toHex().left(32) << LOG_ENDL;
                    if(m_pFrameDecoder->GetSequenceNumber(receivedFrame) != m_nReceivedACKSeqNo)
                    {
                        //LOG_TRACE << "Before sending ack" << LOG_ENDL;
                        //Get the payload
                        if(m_RequestList.count())
                        {
                            QByteArray payload = m_RequestList.at(0).m_Packet.mid(ePayloadStartBytePos, 1);
                            QDataStream payloadDs(payload);
                            payloadDs.setByteOrder(QDataStream::BigEndian);

                            //Read the payload to get the response Id
                            quint8 responseId;
                            payloadDs >> responseId;
                            //LOG_INFO << "responseId " <<responseId.toHex() << LOG_ENDL;
                            QByteArray respID(1, responseId);//esjang test

    	                    if(m_pFrameDecoder->GetRequestIDNumber(m_RequestList.at(0).m_Packet) == PNDR_SESSION_START)
    	                    {
                                LOG_INFO<<"VALIDATOR::Valid ACK for session start = "<< respID.toHex().left(32) <<LOG_ENDL;
    	                        emit SessionStartAcked();
    	                    }
                            else if(m_pFrameDecoder->GetRequestIDNumber(m_RequestList.at(0).m_Packet) == PNDR_SESSION_TERMINATE)
                            {
                                LOG_INFO<<"VALIDATOR::Valid ACK for session Terminate = " << respID.toHex().left(32)<<LOG_ENDL;
    	                        emit SessionTerminateAcked();
                            }
                            //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
                            else if((m_pFrameDecoder->GetRequestIDNumber(m_RequestList.at(0).m_Packet) == PNDR_RETURN_BRANDING_IMAGE_SEGMENT)) 
                            {
                                    emit NextBrandingImage(); 
                            }
                            //} modified by yongkyun.lee 2014-08-14                             
                            else
                            {
                                LOG_INFO<<"VALIDATOR::Valid ACK received resp ID = "<< respID.toHex().left(32) <<LOG_ENDL;
                            }
                        }
                        m_nReceivedACKSeqNo = m_pFrameDecoder->GetSequenceNumber(receivedFrame);
                    }
                    #endif
                    else
                    {
                        //else if duplicate ACK, ignore the ACK
                        LOG_HIGH<<"VALIDATOR::Duplicate ACK. ACK Dropped"<<LOG_ENDL;
                    }
                    //} added by esjang 2013.05.14 for certification #2.3 session termination.

                    //{ added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
			
                    //} added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack

                    //stop the timer
                    m_ReplyWaitTimer.stop();
                    //Set the request as not pending
                    m_bRequestPending = false;
                    //Reset the counter
                    m_nReqCounter = 0;
                    if(m_RequestList.count())
                    {
                        //Remove the request from the request list
                        m_RequestList.removeFirst();
                    }
                    //If there are other requests in the queue,
                    //then send the first one
                    //LOG_INFO << "Validator:: m_requestList :" << m_RequestList.count() << LOG_ENDL;
                    if(m_RequestList.count())
                    {
                       Send(m_RequestList.at(0).m_Packet,false);
                    }
            }
            else
            {
                    LOG_HIGH<<"Validator::Invalid ACK. Hence droped frame = "<< inData.toHex().left(32)<<LOG_ENDL;
            }

        }
        //frame received is data frame
        else
        {
            //Validate the CRC and length
            quint32 payloadLen = IsValidLength(receivedFrame);
            if(IsValidCRC(receivedFrame) && payloadLen)
            {
                //LOG_INFO << "Validator::Valid Data Received "<< LOG_ENDL;

                //Frame ok then send an ACK with corresponding SeqNo
                SendAck(receivedFrame);

                //If it is not a duplicate frame
#ifdef DHAVN_PANDORA_TESTFRAMEWORK
                if(m_pFrameDecoder->GetSequenceNumber(receivedFrame) == (m_nReceivedFrameSeqNo ^1))
                {

                    //if(receivedFramePNDR_GET_TRACK_ALBUM_ART)
                    //Pass it to decoder
                    m_pFrameDecoder->DecodeAndEmit(receivedFrame);
                    m_nReceivedFrameSeqNo = m_pFrameDecoder->GetSequenceNumber(receivedFrame);
                }
#else
                //Get the payload
                QByteArray payload = receivedFrame.mid(ePayloadStartBytePos, payloadLen);
                QDataStream payloadDs(payload);
                payloadDs.setByteOrder(QDataStream::BigEndian);

                //Read the payload to get the response Id
                quint8 responseId;
                payloadDs >> responseId;
                //QByteArray respID(1, responseId);//esjang test  //removed by cheolhwan 2014-05-13. Not use

                if(m_nReceivedFrameSeqNo == 3 || m_pFrameDecoder->GetSequenceNumber(receivedFrame) != m_nReceivedFrameSeqNo )
                {
                    //LOG_TRACE << "Before sending ack" << LOG_ENDL;
                    //Pass it to decoder
                    m_pFrameDecoder->DecodeAndEmit(receivedFrame);
                    m_LastResponseId = responseId;
                    m_nReceivedFrameSeqNo = m_pFrameDecoder->GetSequenceNumber(receivedFrame);
                }
#endif
                else
                {
                    //else if duplicate frame, ignore the frame
                    LOG_HIGH <<"VALIDATOR::Duplicate frame Recv accroding to Pre Seq No = " << m_nReceivedFrameSeqNo
                              << " , LastResponseId = " << m_LastResponseId <<" , Data Frame = "<< inData.toHex().left(32)<< LOG_ENDL;

                    if( responseId == PNDR_UPDATE_TRACK_ELAPSED  || m_LastResponseId != responseId)
                    {
                        m_pFrameDecoder->DecodeAndEmit(receivedFrame);
                        m_LastResponseId = responseId ;
                        m_nReceivedFrameSeqNo = m_pFrameDecoder->GetSequenceNumber(receivedFrame);
                    }
                }
            }
            else
            {
                //Ignore the frame if CRC or length is invalid
                LOG_HIGH<<"VALIDATOR::Invalid frame. Hence Dropped  frame = "<<inData.toHex().left(32)<<LOG_ENDL;
            }
        }
    }
    //Frame is not a complete frame.
    //So collate the packet till complete frame is formed.
    else
    {
        LOG_HIGH<<"VALIDATOR::Incomplete frame = "<<inData.toHex().left(32)<<LOG_ENDL;
    }
    //LOG_INFO<< "CPndrFrameValidator::OnFrameReceived:Exit" <<LOG_ENDL;

}


/**
*******************************************************************
* Timer slot to be listened to for check ack
*
* @param[in]   None
*
* @return     None
*
*******************************************************************
*/

void CPndrFrameValidator::OnTimeout()
{
    //LOG_INFO<< "CPndrFrameValidator::OnTimeout:Entry" <<LOG_ENDL;

    //reset the reqPending flag
    m_bRequestPending = false;

    //Resend Packet Request for first Req.
    if(m_RequestList.count())
    {
        LOG_HIGH<< "Timeout occured Hence resending packet "<< LOG_ENDL;
        ResendPacketRequest(m_RequestList.at(0).m_Packet);
    }
    else
    {
        m_ReplyWaitTimer.stop();
        LOG_CRITICAL << "VALIDATOR:: o req in validator queue.Still got timeout event"<< LOG_ENDL;
    }
   // LOG_INFO<< "CPndrFrameValidator::OnTimeout:Exit" <<LOG_ENDL;

}


/**
 *******************************************************************
 * Update that Ack is received. Class to take required action
 *
 * @param   inFrame The frame for which ACK is to be sent
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameValidator::SendAck(const QByteArray& inFrame)
{
   // LOG_TRACE<< "CPndrFrameValidator::SendAck:Entry" <<LOG_ENDL;

    //emit ACK
    QByteArray ackFrame;
    ackFrame[0]=  PNDR_FRAME_START;
    ackFrame[1]=  PNDR_FRAME_TYPE_ACK;
    ackFrame.append(m_pFrameDecoder->GetSequenceNumber(inFrame) ^ 1);

    QByteArray  payloadlen;
    payloadlen[0]= 0x00;
    payloadlen[1]= 0x00;
    payloadlen[2]= 0x00;
    payloadlen[3]= 0x00;

    ackFrame.append(payloadlen);

    QByteArray crcData;
    QDataStream crcDs(&crcData,QIODevice::WriteOnly);
    crcDs.setByteOrder(QDataStream::BigEndian);
    //Calculate the CRC for the encodedFrame excluding the first byte(start byte)
    QByteArray crcByteArray = ackFrame.right(ackFrame.size() - 1);
    quint16 crc = CPndrFrameEncoder::CalculateCRC(crcByteArray,
                               crcByteArray.size());
    crcDs << crc;
    ackFrame.append(crcData);
    ackFrame.append(PNDR_FRAME_END);

    //LOG_INFO << "VALIDATOR::Sending ACK = " << ackFrame.toHex()<<LOG_ENDL;
    m_pActiveTxRx->Send(ackFrame);
   // LOG_TRACE<< "CPndrFrameValidator::SendAck:Exit" <<LOG_ENDL;

    //TO DO:  Check if Handle escaping here is needed
}

/**
 *******************************************************************
 * Check if received frame is valid by checking CRC
 *
 * @param   None
 *
 * @return  None
 *
 *******************************************************************
*/
bool CPndrFrameValidator::IsValidCRC(const QByteArray& inFrame)
{
   // LOG_TRACE<< "CPndrFrameValidator::IsValidCRC:Entry" <<LOG_ENDL;

    bool validCrc = false;

    QByteArray crcData(inFrame);
    crcData.remove(0,1);
    //CRC to be calculated excluding the last 3 bytes of frame(2CRC bytes and end Byte)
    quint16 CalculatedCRC = CPndrFrameEncoder::CalculateCRC(crcData,crcData.length()-3);
    quint16 encodedCRC = m_pFrameDecoder->GetCRC(inFrame);

    //LOG_INFO<< "CalculatedCRC= " << CalculatedCRC <<LOG_ENDL;
    //LOG_INFO<< "encodedCRC= "<< encodedCRC <<LOG_ENDL;

    if(CalculatedCRC == encodedCRC)
    {
         validCrc = true;
    }
    else
    {
        LOG_CRITICAL <<"VALIDATOR::CRC Received is Invalid" << LOG_ENDL;
    }
   // LOG_TRACE<< "CPndrFrameValidator::IsValidCRC:Exit" <<LOG_ENDL;

    return validCrc;
}

/**
 *******************************************************************
 * Check if received frame has valid length field
 *
 * @param   None
 *
 * @return  None
 *
 *******************************************************************
*/
quint32 CPndrFrameValidator::IsValidLength(const QByteArray& inData)
{
   // LOG_TRACE<< "Inside CPndrFrameValidator::IsValidLength" <<LOG_ENDL;

    return m_pFrameDecoder->GetPayloadLength(inData);
}

/**
 *******************************************************************
 * Check if received ack is valid by checking Seq No
 *
 * @param   None
 *
 * @return  true if Seq no is valid else false
 *
 *******************************************************************
*/
bool CPndrFrameValidator::IsValidAck(const QByteArray& inFrame)
{
   // LOG_TRACE<< "CPndrFrameValidator::IsValidAck:Entry: m_RequestList.count: "<< m_RequestList.count() <<LOG_ENDL;

    bool validSeqNo = true;

    if(m_RequestList.count())
    {
        //If seq no. of request frame and ack frame is same then ack is invalid
        if(m_pFrameDecoder->GetSequenceNumber(m_RequestList.at(0).m_Packet)
               == m_pFrameDecoder->GetSequenceNumber(inFrame))
        {
            //also eliminates duplicate ACK
            validSeqNo = false;
        }
    }
    LOG_TRACE<< "VALIDATOR::IsValidAck  returns = " << validSeqNo <<LOG_ENDL;

    return validSeqNo;
}

/**
 *******************************************************************
 * Resend the packet request to Transport layer if needed
 *
 * @param   None
 *
 * @return  None
 *
 *******************************************************************
*/
void CPndrFrameValidator::ResendPacketRequest(const QByteArray& inData)
{
    //LOG_TRACE<< "CPndrFrameValidator::ResendPacketRequest:Entry" <<LOG_ENDL;

    //Check if the request has already been sent for max times
    if(m_nReqCounter < cMaxReqCount)
    {
        //Re-Send the data to transport layer without appending it to queue.
        Send(inData,false);
    }
    else
    {
        LOG_CRITICAL<< "VALIDATOR::Max Frame drop , Pending Request count = "<< m_RequestList.count()<<LOG_ENDL;

        //Remove the req from req list
        if(m_RequestList.count())
        {
           // m_RequestList.removeFirst();
            m_RequestList.clear();
        }

        m_bRequestPending = false;
        //Reset the req counter
        m_nReqCounter = 0;

        //Cancel the timer
        if( m_ReplyWaitTimer.isActive())
        {
            m_ReplyWaitTimer.stop();
        }
       // LOG_TRACE<< "esjang 0514 CPndrFrameValidator::ResendPacketRequest:Set max Frame drops"<< m_RequestList.count()<<LOG_ENDL;
        //Send a flag for session termination disable.		
        emit MaxFrameDrops(TRUE); // added by esjang 2013.05.14 for certification #2.3 11 frame drops. Disable session termination.
        //Send a Network error to Decoder
        m_pFrameDecoder->EmitNetworkError();
    }
    //LOG_TRACE<< "CPndrFrameValidator::ResendPacketRequest:Exit" <<LOG_ENDL;
}

//End Of File
