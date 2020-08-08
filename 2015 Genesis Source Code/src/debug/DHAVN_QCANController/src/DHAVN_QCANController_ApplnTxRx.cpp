/*
 ******************************************************************
 * COPYRIGHT © 2011, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE : DHAVN_QCANController								  *
 * PROGRAMMER : Sambit Patnaik									  *
 * DATE : 8 August 2011                                           *
 * VERSION : 1.7												  *
 *--------------------------------------------------------------- *
 * MODULE SUMMARY : This file defines the constructor,destructor
 *                  for the class CQCANController_ApplnTxRx. It   *
 *                  also defines slots which are called when      *
 *                  application emits the respective signal and   *
 *                  the slots when data needs to be sent to       *
 *                  application.								  *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS * 
 * VERSION   DATE            PROGRAMMER               COMMENT
 ******************************************************************
 * 1.8                                               cr 5636
 * 1.7                                               cr 5128
 * 1.6                      Divya Ravindran          cr 4906
 * 1.5                                               cr 2226
 * 1.4						     cr 1781
 * 1.3						     cr 1291
 * 1.2                                               cr 673
 * 1.1                                               cr 302
 * 1.0       8 August 2011  Sambit Patnaik           Draft version
 ******************************************************************/

#include "DHAVN_QCANController_ApplnTxRx.h"
#include "DHAVN_QCANController_CANTxRx.h"

/*
 *******************************************************************
 * Function:CQCANController_ApplnTxRx(QObject *parent)
 *
 * Description :constructor
 *
 * Parameters :
 *  parent          r   Pointer to the parent application
 *
 * Returns :none
 *
 *******************************************************************
 */
CQCANController_ApplnTxRx::CQCANController_ApplnTxRx(QObject *parent):m_pCanTxRx(NULL)
{
    LW_LOG("[QCAN] CQCANController_ApplnTxRx constructor entry.");
    //store the pointer to the parent application
    m_parent = parent;
    bOngoingRequest = false;
    LW_LOG("[QCAN] CQCANController_ApplnTxRx constructor exit.");
}

/*
 *******************************************************************
 * Function:Init()
 *
 * Description :Initialize the member variables
 *
 * Parameters :none
 *
 * Returns :bool
 *
 *******************************************************************
 */
bool CQCANController_ApplnTxRx::Init()
{
    LW_LOG("[QCAN] CQCANController_ApplnTxRx::Init entry");
    //Instantiating the CQCANController_CANTxRx class object
    m_pCanTxRx = new(std::nothrow) CQCANController_CANTxRx();

    if(NULL != m_pCanTxRx && m_pCanTxRx->Init())
    {
        //Connect the SIGNALS and the SLOTS that are required to get motification from CQCANController_CANTxRx class
        connect(m_pCanTxRx,SIGNAL(Can_data_received(SQCanFrameList)),this,SLOT(Received_data_from_can_raw(SQCanFrameList)));
        connect(m_pCanTxRx,SIGNAL(Filter_can_id()),this,SLOT(Get_can_id()));
        connect(m_pCanTxRx,SIGNAL(Filter_can_bcm_msg()),this,SLOT(Get_can_bcm_msg()));
    }
    else
    {
        CR_LOG("[QCAN] CQCANController_ApplnTxRx::Init, Memory allocation failed !!!");
        //CQCANController_CANTxRx class could not be instantiated
        return false;
    }

    //Connect the SIGNALS that application uses for calling QCANController API
    connect(m_parent,SIGNAL(Can_data_send(SQCanFrame)),this,SLOT(Send_data_to_can_raw(SQCanFrame)));

    connect(m_parent,SIGNAL(Can_data_send_bcm(SQBCMHead,SQCanFrameList)),this,SLOT(Send_data_to_bcm(SQBCMHead,SQCanFrameList)));

    connect(m_parent,SIGNAL(Can_filter_msg_id(SQCanIdList)),this,SLOT(Received_can_filter_id(SQCanIdList)));

    connect(m_parent,SIGNAL(Can_filter_bcm_msg(SQBCMHead)),this,SLOT(Received_can_filter_bcm_msg(SQBCMHead)));

    connect(m_parent,SIGNAL(Can_data_receive_bcm(SQBCMHead)),this,SLOT(Receive_data_from_bcm(SQBCMHead)));

    LW_LOG("[QCAN] CQCANController_ApplnTxRx::Init exit");
    //Return success, when all objects are properly constructed
    return true;
}

/*
 *******************************************************************
 * Function:~CQCANController_ApplnTxRx()
 *
 * Description :Default destructor
 *
 * Parameters :none
 *
 * Returns :none
 *
 *******************************************************************
 */
CQCANController_ApplnTxRx::~CQCANController_ApplnTxRx()
{
    //cleanup code
    if(m_pCanTxRx)
    {
        delete m_pCanTxRx;
        m_pCanTxRx = NULL;
    }
}

/*
 *******************************************************************
 * Function:Send_data_to_can_raw(const SQCanFrame &c_send_data)
 *
 * Description :This slot will be called when the application signals
 *              Can_data_send. This API will send CAN data to the
 *              CQCANController_CANTxRx class.
 *
 * Parameters :
 *    c_send_data     r   This will have the CAN frame structure that has to
 *                     be sent to CAN RAW socket.
 *
 * Returns : void
 *
 *******************************************************************
 */
void CQCANController_ApplnTxRx::Send_data_to_can_raw(const SQCanFrame &c_send_data)
{
    LW_LOG("[QCAN] CQCANController_ApplnTxRx::Send_data_to_can_raw entry");
    //Send the SQCanFrame to the class that handles socket communication
    m_pCanTxRx->Can_send_data(c_send_data);
    LW_LOG("[QCAN] CQCANController_ApplnTxRx::Send_data_to_can_raw exit");
}

/*
 *******************************************************************
 * Function:Send_data_to_bcm(const SQBCMHead &c_bcm_data,
 *                           const SQCanFrameList &c_can_data);
 *
 * Description :This slot will be called when the application signals
 *              Can_data_send_bcm. This API will send CAN BCM data to
 *              the CQCANController_CANTxRx class.
 *
 * Parameters :
 *    c_bcm_data     r   This will have the BCM Head structure that has to
 *                    be sent to CAN BCM socket.
 *    c_can_data     r   This will have the list of CAN frames that has to be
 *                    written to CAN BCM socket
 *
 * Returns : void
 *
 *******************************************************************
 */
void CQCANController_ApplnTxRx::Send_data_to_bcm(const SQBCMHead &c_bcm_data,
                                                 const SQCanFrameList &c_can_data)
{
    //Send the CAN BCM data and the CAN data payload to the class that handles
    //socket communication
    m_pCanTxRx->Can_send_bcm_data(c_bcm_data, c_can_data);

}

/*
 *******************************************************************
 * Function:Received_data_from_can_raw(const SQCanFrameList &c_received_data)
 *
 * Description :This slot will be called when there is some data
 *              available in the CAN RAW socket and it has to be sent
 *              to the application. This API will be invoked when there
 *              is a signal Can_data_received. This will send the
 *              received data to the application using SIGNAL "Can_data_received"
 *
 * Parameters :
 *    c_received_data     r   This will have the list of CAN frame structures
 *                         that has to be sent to application
 *
 * Returns : void
 *
 *******************************************************************
 */
void CQCANController_ApplnTxRx::Received_data_from_can_raw(const SQCanFrameList &c_received_data)
{
    LW_LOG("[QCAN] CQCANController_ApplnTxRx::Can_data_received");
    //Signal the application, with the data
    emit Can_data_received(c_received_data);

    //Reset the ongoing request flag
    bOngoingRequest = false;
}

/*
 *******************************************************************
 * Function:Receive_data_from_bcm(const SQBCMHead &c_bcm_data)
 *
 * Description :This slot will be called when the application wants to
 *              receive CAN BCM data from the BCM socket
 *
 * Parameters :
 *    c_bcm_data     r   This will have the list of CAN frame structures
 *                    that has to be sent to application
 *
 * Returns : void
 *
 *******************************************************************
 */
void CQCANController_ApplnTxRx::Receive_data_from_bcm(const SQBCMHead &c_bcm_data)
{
    //Send the SQBCMHead structure to the class that handles socket communication
    m_pCanTxRx->Can_receive_bcm_data(c_bcm_data);
    LW_LOG("[QCAN] CQCANController_ApplnTxRx::Receive_data_from_bcm");
}

/*
 *******************************************************************
 * Function:Get_can_id()
 *
 * Description :This API will be called when the QCANController want the
 *              the list of CAN ID's for which it has to send data
 *              to the application. This is invoked when there is a
 *              SIGNAL Filter_can_id from CQCANController_CANTxRx class
 *
 * Parameters :none
 *
 * Returns :void
 *
 *******************************************************************
 */
void CQCANController_ApplnTxRx::Get_can_id()
{
    //Check the status of ongoing request flag
    if(!bOngoingRequest)
    {
        bOngoingRequest = true;

        LW_LOG("[QCAN] CQCANController_ApplnTxRx::Filter_can_id");
        //signal the application to send the list of CAN ID's
        emit Filter_can_id();
    }
}

/*
 *******************************************************************
 * Function:Get_can_bcm_msg()
 *
 * Description :This API will be called when the QCANController want the
 *              the BCM message for which it has to send BCM data
 *              to the application. This is invoked when there is a
 *              SIGNAL Filter_can_bcm_msg from CQCANController_CANTxRx class
 *
 * Parameters :none
 *
 * Returns :void
 *
 *******************************************************************
 */
void CQCANController_ApplnTxRx::Get_can_bcm_msg()
{
    //signal the application to send the CAN BCM information
    emit Filter_can_bcm_msg();
}

/*
 *******************************************************************
 * Function:Received_can_filter_id(const SQCanIdList &c_filter_id)
 *
 * Description :This slot will be called when the application sends the
 *              the list of CAN ID's for which it needs CAN frames. This
 *              SLOT will be invoked when there is SIGNAL Can_filter_msg_id
 *
 * Parameters :
 *    c_filter_id     r   This will have the list of CAN ID's for which
 *                     CAN frames have to be read from CAN RAW socket
 *
 * Returns : void
 *
 *******************************************************************
 */
void CQCANController_ApplnTxRx::Received_can_filter_id(const SQCanIdList &c_filter_id)
{
    //sends the data to CQCANController_CANTxRx class for it to read
    //from the RAW socket
    m_pCanTxRx->Can_read_data_raw(c_filter_id);
}

/*
 *******************************************************************
 * Function:Received_can_filter_bcm_msg(const SQBCMHead &c_filter_bcm_msg)
 *
 * Description :This slot will be called when the application sends the
 *              the BCM message for which it needs CAN frames. This
 *              SLOT will be invoked when there is SIGNAL Can_filter_bcm_msg
 *
 * Parameters :
 *    c_filter_bcm_msg     r   This will have the BCM message for which
 *                     CAN frames have to be read from CAN BCM socket
 *
 * Returns : void
 *
 *******************************************************************
 */
void CQCANController_ApplnTxRx::Received_can_filter_bcm_msg(const SQBCMHead &c_filter_bcm_msg)
{
    //sends the data to CQCANController_CANTxRx class for it to read
    //from the BCM socket
    m_pCanTxRx->Can_read_bcm_data_raw(c_filter_bcm_msg);
}
