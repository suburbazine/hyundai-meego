/*
 ******************************************************************
 * COPYRIGHT © 2011, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE : DHAVN_QCANController								  *
 * PROGRAMMER : Sambit Patnaik									  *
 * DATE : 8 August 2011                                    		  *
 * VERSION : 1.7												  *
 *--------------------------------------------------------------- *
 * MODULE SUMMARY : This file defines the class
 *                  CQCANController_CANTxRx  which handles the    *
 *                  request from the CQCANController_ApplnTxRx    *
 *                  and forward it to RAW/BCM Socket. This class  *
 *                  will handle all the response from MICOM CAN   *
 *                  Driver layer and convert the data into the    *
 *                  type that is understandable by the application*
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS * 
 * VERSION   DATE            PROGRAMMER               COMMENT
 ******************************************************************
 * 1.7                                               cr 5128
 * 1.6                                               cr 4906
 * 1.5                                               cr 2226
 * 1.4						     cr 1781
 * 1.3						     cr 1291
 * 1.2                                               cr 673
 * 1.1                                               cr 302
 * 1.0       8 August 2011  Sambit Patnaik           Draft version
 ******************************************************************/


#ifndef DHAVN_QCANCONTROLLER_CANTXRX_H
#define DHAVN_QCANCONTROLLER_CANTXRX_H

#include "DHAVN_QCANController_ApplnTxRxStruct.h"

//This has been typedef'ed as this is not available in the system can.h file
typedef unsigned short  sa_family_t;

#include <linux/can.h>
#include <linux/can/bcm.h>
#include <linux/can/raw.h>
#include <QSocketNotifier>

/** This structure hold the bcm_msg_head and pointer to can_frame structure */
struct SQCanBCMStruct {
    struct bcm_msg_head qbcmhead;   /**< CAN bcm head messge */
    struct can_frame qcanframes[1];   /**< Array of CAN frame structures */
};

/**
 * This class will handle the request from the CQCANController_ApplnTxRx
 * and forward it to RAW/BCM Socket. This class will handle all the response from
 * MICOM CAN Driver layer and convert the data into the type that is understandable
 * by the application.
 */
class CQCANController_CANTxRx : public QObject
{
    Q_OBJECT

public:

    /**
     * Default Constructor
     */
            CQCANController_CANTxRx();

    /**
     * Initialize the member variables
     */
    bool Init();

    /**
     * Default destructor
     */
    ~CQCANController_CANTxRx();

public slots:
    /**
     * This API will write the CAN frame to the CAN RAW Socket
     */
    void Can_send_data(const SQCanFrame &c_data);

    /**
     * This API will write the BCM Message to the CAN BCM Socket
     */
    void Can_send_bcm_data(const SQBCMHead &c_bcm_data, const SQCanFrameList &c_data);

    /**
     * This API will be called when the application triggers to receive a BCM
     * message. This API will set the BCM socket option with the CAN ID for
     * which the application has requested a BCM message
     */
    void Can_receive_bcm_data(const SQBCMHead &c_bcm_data);

    /**
     * This API will be called when CAN Data is available in the CAN RAW/BCM socket
     * This will be called when socket notifier gets a call indicating some data
     * is available
     */
    void Can_receive_data(const SQCanFrameList &c_data);

    /**
     * This SLOT will be called when there is some data available for the read
     * from the CAN RAW socket. This is connected to the SIGNAL activated() which
     * socket notifier emits when data is available. This API basically will
     * trigger a call to application requesting for the CAN ID's
     */
    void Get_can_id();

    /**
     * This SIGNAL will notify the CQCANController_ApplnTxRx to fetch the CAN
     * BCM message from the application
     */
    void Get_can_bcm_msg();

    /**
     * This function will be called from CQCANController_ApplnTxRx with the list of
     * CAN ID's using filter
     */
    void Can_read_data_raw(const SQCanIdList &c_can_id_list);

    /**
     * This SLOT will be called from CQCANController_ApplnTxRx with the BCM message
     */
    void Can_read_bcm_data_raw(const SQBCMHead &c_bcm_data);

signals:
    /**
     * This SIGNAL will notify the CQCANController_ApplnTxRx with the data
     */
    void Can_data_received (const SQCanFrameList &c_data);

    /**
     * This SIGNAL will notify the CQCANController_ApplnTxRx to fetch the list of
     * CAN ID's from the application
     */
    void Filter_can_id();

    /**
     * This SIGNAL will notify the CQCANController_ApplnTxRx to fetch the BCM
     * message from the application
     */
    void Filter_can_bcm_msg();

private:

    /**
     * Initializes the CAN RAW socket
     */
    bool Initialize_raw_socket();

    /**
     * Initializes the CAN BCM socket
     */
    bool Initialize_bcm_socket();

    /**
     * Connects the CAN RAW socket with QSocketNotifier
     */
    void Connect_read_socket_notifier();

    /**
     * Connects the CAN BCM socket with QSocketNotifier
     */
    void Connect_read_bcm_socket_notifier();

private:
    /**
     * CAN RAW socket identifier
     */
    int m_iCanSocket;

    /**
     * CAN BCM socket identifier
     */
    int m_iBcmSocket;

    /**
     * Structure that holds list of CAN Id's
     */
    SQCanIdList *m_CanIdList;

    /**
     * Pointer to the CAN RAW Socket Notifier
     */
    QSocketNotifier *m_pCanSocketNotifier;

    /**
     * Pointer to the CAN BCM Socket Notifier
     */
    QSocketNotifier *m_pCanBCMSocketNotifier;
};
#endif // DHAVN_QCANCONTROLLER_CANTXRX_H
