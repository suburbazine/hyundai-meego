/*
 ******************************************************************
 * COPYRIGHT © 2011, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE : DHAVN_QCANController				  *
 * PROGRAMMER : Sambit Patnaik					  *
 * DATE : 8 August 2011                                    	  *
 * VERSION : 1.7												  *
 *--------------------------------------------------------------- *
 * MODULE SUMMARY : This file defines class                       *
 *                  CQCANController_ApplnTxRx which handles the   *
 *                  request from the application and forward it   *
 *                  to CQCANController_CANTxRx class			  *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS * 
 * VERSION   DATE            PROGRAMMER               COMMENT
 ******************************************************************
 * 1.7                                               cr 5128
 * 1.6                                               cr 4906
 * 1.5                                               cr 2226
 * 1.4						     cr 1781
 * 1.3					             cr 1291
 * 1.2                                               cr 673
 * 1.1                                               cr 302
 * 1.0       8 August 2011  Sambit Patnaik           Draft version
 ******************************************************************/


#ifndef DHAVN_QCANCONTROLLER_APPLNTXRX_H
#define DHAVN_QCANCONTROLLER_APPLNTXRX_H

#include "DHAVN_QCANController_ApplnTxRxStruct.h"

//Forward class Declaration
class CQCANController_CANTxRx;

typedef QList<uint> TUint_List;
Q_DECLARE_METATYPE(TUint_List)

/**
 * This class will handle the request from the application and forward it to
 * CQCANController_CANTxRx class. This class exposes the signals that the application
 * would be calling for any operation like sending data or receiving data
 */
class Q_DECL_EXPORT CQCANController_ApplnTxRx : public QObject
{
    Q_OBJECT

public:

    /**
     * Constructor
     */
            CQCANController_ApplnTxRx(QObject *parent);


    /**
     * Initialize the member variables
     */
    bool Init();

    /**
     * Default destructor
     */
    ~CQCANController_ApplnTxRx();

public slots:

    /**
     * This slot will be called when the application signals
     * Can_data_send. This API will send CAN data to the
     * CQCANController_CANTxRx class.
     */
    void Send_data_to_can_raw(const SQCanFrame &c_send_data);

    /**
     * This slot will be called when the application signals
     * Can_data_send_bcm. This API will send CAN BCM data to
     * the CQCANController_CANTxRx class.
     */
    void Send_data_to_bcm(const SQBCMHead &c_bcm_data,const SQCanFrameList &c_can_data);

    /**
     * This slot will be called when there is some data
     * available in the CAN RAW socket and it has to be sent
     * to the application. This API will be invoked when there
     * is a signal Can_data_received. This will send the
     * received data to the application using SIGNAL "Can_data_received"
     */
    void Received_data_from_can_raw(const SQCanFrameList &c_received_data);

    /**
     * This slot will be called when the application wants to
     * receive CAN BCM data from the BCM socket
     */
    void Receive_data_from_bcm(const SQBCMHead &c_bcm_data);

private slots:

    /**
     * This API will be called when the QCANController want the
     * the list of CAN ID's for which it has to send data
     * to the application. This is invoked when there is a
     * SIGNAL Filter_can_id from CQCANController_CANTxRx class
     */
    void Get_can_id();

    /**
     * This API will be called when the QCANController want the
     * the BCM message for which it has to send BCM data
     * to the application. This is invoked when there is a
     * SIGNAL Filter_can_bcm_msg from CQCANController_CANTxRx class
     */
    void Get_can_bcm_msg();

    /**
     * This slot will be called when the application sends the
     * the list of CAN ID's for which it needs CAN frames
     */
    void Received_can_filter_id(const SQCanIdList &c_filter_id);

    /**
     * This slot will be called when the application sends the
     * the BCM message for which it needs CAN frames
     */
    void Received_can_filter_bcm_msg(const SQBCMHead &c_filter_bcm_msg);

signals:
    /**
     * This SIGNAL will be used by the application to send
     * the list of CAN ID's for which it is expecting
     * CAN frames. This will invoke Received_can_filter_id SLOT
     */
    void Can_filter_msg_id(const SQCanIdList &c_data);

    /**
     * This SIGNAL will be used by the application to send
     * the BCM message for which it is expecting
     * CAN frames. This will invoke Received_can_filter_bcm_msg SLOT
     */
    void Can_filter_bcm_msg(const SQBCMHead &c_data);

    /**
     *This SIGNAL will be used by the application when it
     * wants to send CAN frames to CAN RAW socket. This will
     * invoke Send_data_to_can_raw SLOT
     */
    void Can_data_send(const SQCanFrame &c_data);

    /**
     * This SIGNAL will be used by the application when it
     * wants to send BCM data to CAN BCM socket. This will
     * invoke Send_data_to_bcm SLOT
     */
    void Can_data_send_bcm(const SQBCMHead &c_bcm_data,const SQCanFrameList &c_can_data);

    /**
     * This SIGNAL will be used by the application when it
     * wants to receive BCM data corresponding to a given
     * CAN ID. This would invoke Receive_data_from_bcm SLOT
     */
    void Can_data_receive_bcm(const SQBCMHead &c_bcm_data);

    /**
     * This SIGNAL will be used by the CQCANController_CANTxRx
     * when it received some data from the CAN RAW socket.
     * This would invoke Received_data_from_can_raw SLOT
     */
    void Can_data_received(const SQCanFrameList &c_data);

    /**
     * This SIGNAL will be used by the CQCANController_CANTxRx to send the
     * request for the list of CAN ID's. This would invoke Get_can_id SLOT
     */
    void Filter_can_id();

    /**
     * This SIGNAL will be used by the CQCANController_CANTxRx to send the
     * request for the BCM message. This would invoke Get_can_bcm_msg SLOT
     */
    void Filter_can_bcm_msg();

private:

    /**
     * Pointer to application class, takes ownership
     */
    QObject *m_parent;

    /**
     * Pointer to CQCANController_CANTxRx class, takes ownership
     */
    CQCANController_CANTxRx *m_pCanTxRx;

    /**
     * Flag to hold the status of the ongoing request
     * of sending CAN data to the application
     */
    bool bOngoingRequest;
};

#endif // DHAVN_QCANCONTROLLER_APPLNTXRX_H
