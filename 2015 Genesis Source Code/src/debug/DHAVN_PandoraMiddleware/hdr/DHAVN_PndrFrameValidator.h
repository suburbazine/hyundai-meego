/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : Pandora Frame Validator                           *
 * PROGRAMMER : Vandana Ahuja                                     *
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
 *  04/12/2011  0.1 Vandana Ahuja   header file created
 *
 ******************************************************************
 */

#ifndef DHAVN_PNDR_FRAME_VALIDATOR_H
#define DHAVN_PNDR_FRAME_VALIDATOR_H

#include <QObject>
#include <QTimer>

#include "DHAVN_Pndr_Defs.h"
#include "DHAVN_Pndr_Internal_Defs.h"
#include "DHAVN_LogUtil.h"

//Forward Declaration
class CPndrFrameTxRx;
class CPndrFrameDecoder;

/**
* Singleton class for validating the incoming and outgoing frames
* This class will create thread for sending/receiving the frames
*/

class CPndrFrameValidator : public QObject
{
    Q_OBJECT

public:

    /**
    * Default constructor made private to make it singleton
    */
    CPndrFrameValidator();

    /**
     *******************************************************************
     * Destructor
     *******************************************************************
     */
    ~CPndrFrameValidator();

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
    //static CPndrFrameValidator* GetInstance();

    void SetFrameTxRX(CPndrFrameTxRx*& inFrameTxRx);
    void SetDecoder(CPndrFrameDecoder* inFrameDecoder);

    /**
     *******************************************************************
     * Releases instance of FrameValidtor
     *
     * @param    None
     *
     * @return     None
     *
     *******************************************************************
     */
   // void ReleaseInstance();


    
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
    void Send(const QByteArray& inData,bool inAppendToQueue = true);

signals:
    void FrameReceived(const QByteArray inFrame);
    void SessionStartAcked(); // added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    void SessionTerminateAcked(); // added by esjang 2013.05.15 for certification # 11frame drops. session start and returns valid ack
//{ modified by yongkyun.lee 2014-08-25 for : Branding Image
    void NextBrandingImage();
    void ReceivedBranding();
//} modified by yongkyun.lee 2014-08-04 

    void ReceivedStatus(); // added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    void MaxFrameDrops(bool inMaxFrameDrops); // added by esjang 2013.05.14 for certification #2.3 11 frame drops. Disable session termination.
public slots:

    /** Slot for signal emitted by transport layer */
    void OnDataReceived(const QByteArray& inData);

    /** Slot for signal emitted when one complete packet is found */
    void OnFrameReceived(const QByteArray inData);

    /** Timer slot to be listened to for check ack */
    void OnTimeout();

private:

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
    void SendAck(const QByteArray& inFrame);

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
    bool IsValidCRC(const QByteArray& inFrame);


    /**
     *******************************************************************
     * Check if received frame has valid length field
     *
     * @param   None
     *
     * @return  the length of the payload
     *
     *******************************************************************
    */
    quint32 IsValidLength(const QByteArray& inData);

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
    bool IsValidAck(const QByteArray& inFrame);

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
    void ResendPacketRequest(const QByteArray& inData);    

private:

    /**
    * Structure to keep track of Request being sent to check for which
    * packet reply have been received
    */
    typedef struct
    {
        QByteArray   m_Packet;        /** packet sent */
    }TSRequestPacket;

    /** List of request for which ack not received */
    QList<TSRequestPacket> m_RequestList;

    /** Current active transport based on currently selected transport */
    CPndrFrameTxRx*  m_pActiveTxRx;

    /** Timer to be used for waiting for ack reply*/
    QTimer m_ReplyWaitTimer;

    /** Instance of Decoder class */
    CPndrFrameDecoder* m_pFrameDecoder;

    /** Request counter */
    quint8  m_nReqCounter;

    /** Instance counter */
    //static int m_nInstanceCounter;

    /** Reference to self */
   // static CPndrFrameValidator* m_pInstance;

    /** Flag to indicate Request is pending */
    bool m_bRequestPending;

    /** seq no. of received frame */
    quint8 m_nReceivedFrameSeqNo;

    /** seq no. of received ACK frame */
    quint8 m_nReceivedACKSeqNo; //added by esjang 2013.05.14 for certification #2.3 Session Termination.

    QByteArray m_FrameBuffer;

    quint8 m_LastResponseId;

    USE_LOG_UTIL;
};

#endif //DHAVN_PNDR_FRAME_VALIDATOR_H
