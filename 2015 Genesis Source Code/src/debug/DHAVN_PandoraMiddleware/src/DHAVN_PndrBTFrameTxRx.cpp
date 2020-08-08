/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :   BT Frame Tx Rx                                  *
 * PROGRAMMER :   Satyanarayana B                                   *
 * DATE       :   07/12/2011                                      *
 * VERSION    :   0.1                                             *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module will connect to BT device. Send and receive data 
 *  over BT connection
 *
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  07/12/2011  0.1 Satyanarayana B Cpp file created
 ******************************************************************
 */

#include <QVariant>
#include "DHAVN_PndrBTFrameTxRx.h"
#include "DHAVN_PndrBTDBusInterface.h"

//BT supports 1024 ,so payload len max possible size for Pandora is kept //1000
const quint32 c_Max_Payload_Length_BT = 4000;


/**
 *******************************************************************
 * Default constructor
 *
 * @param   None
 *
 * @return  Instance of CPndrFrameTxRx
 *
 *******************************************************************
 */
CPndrBTFrameTxRx::CPndrBTFrameTxRx():m_pDBusInterface(0),
    m_eInitialized(INIT_NOT_COMPLETED) //esjang - Static Test 6763, uninit_member 2012.11.01
{
    LOG_INIT_V2("Pandora");
    LOG_TRACE << "In CPndrBTFrameTxRx:: CPndrBTFrameTxRx() Entry "<< LOG_ENDL;

    LOG_TRACE << "In CPndrBTFrameTxRx:: CPndrBTFrameTxRx() Exit"<< LOG_ENDL;


}

/**
 *******************************************************************
 * Destructor
 *
 * @param   None
 *
 * @return  None
 *
 *******************************************************************
 */
CPndrBTFrameTxRx::~CPndrBTFrameTxRx()
{
    LOG_TRACE << "In CPndrBTFrameTxRx::~ CPndrBTFrameTxRx() "<< LOG_ENDL;

    if(m_pDBusInterface)
    {
        delete m_pDBusInterface;
        m_pDBusInterface = NULL;
    }

    m_FailedReqArray.clear();

    LOG_TRACE << "In CPndrBTFrameTxRx::~ CPndrBTFrameTxRx() Exit"<< LOG_ENDL;


}

/**
 *******************************************************************
 * Initializes the BT transport layers
 *
 * @param   None
 *
 * @return  EPndrError
 *
 *******************************************************************
 */
EPndrError CPndrBTFrameTxRx::Init()
{
    LOG_TRACE << "In CPndrBTFrameTxRx:: Init() Entry"<< LOG_ENDL;

#ifdef DHAVN_PANDORA_BT_ENABLED
    m_eInitialized = INIT_NOT_COMPLETED;

    EPndrError ret  = E_SUCCESS;

    m_pDBusInterface = new CPandoraBTDBusInterface(CPandoraBTDBusInterface::staticServiceName(),
                                                 CPandoraBTDBusInterface::staticObjectName(),
                                                 QDBusConnection::sessionBus(),0);
    if(m_pDBusInterface && m_pDBusInterface->isValid())
    {
        LOG_INFO<< "connection to dbus is valid "<< LOG_ENDL;
        qint8 dBusSignalConnect = connect(m_pDBusInterface,SIGNAL(SPPConnected(bool)),this,SLOT(onSPPConnected(bool)));
        dBusSignalConnect &= connect(m_pDBusInterface,SIGNAL(SPPDisconnected(bool)),this,SLOT(onSPPDisconnected(bool)));
        dBusSignalConnect &= connect(m_pDBusInterface,SIGNAL(SPPDataReadComplete(const QByteArray)),this,SLOT(onSPPDataReadComplete(const QByteArray)));

        dBusSignalConnect &= connect(m_pDBusInterface,
                             SIGNAL(SPPDataWriteToBTStatus(const int )),this,SLOT(onSPPDataWriteToBTStatus(const int)));

        LOG_INFO << "DBus signal connections : " << dBusSignalConnect << LOG_ENDL;

        if(!dBusSignalConnect)
        {
            ret = E_INIT_FAILED;
            LOG_CRITICAL<< "could not connect to Dbus signal "<< LOG_ENDL;
        }
    }
    else
    {

        ret = E_INIT_FAILED;
        LOG_CRITICAL<< "connection to dbus is not valid "<< LOG_ENDL;

    }
#else

    EPndrError ret  = E_SUCCESS;
#endif
    LOG_TRACE << "In CPndrBTFrameTxRx:: Init() Exit"<< LOG_ENDL;

    return ret;
}


/**
 *******************************************************************
 *  DeInit Function for releasing memory allocated
 *
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */

void CPndrBTFrameTxRx::DeInit()
{
    LOG_TRACE << "In CPndrBTFrameTxRx:: DeInit() Entry"<< LOG_ENDL;

    if(m_pDBusInterface && m_pDBusInterface->isValid())
    {
        m_pDBusInterface->SPPDisconnect();
    }

    if(m_pDBusInterface)
    {
        delete m_pDBusInterface;
        m_pDBusInterface = NULL;
    }

    //Reset the initialization state of DBus
    m_eInitialized = INIT_NOT_COMPLETED;

    LOG_TRACE << "In CPndrBTFrameTxRx:: DeInit() Exit"<< LOG_ENDL;

}

/**
 *******************************************************************
 * Connect BT
 *
 * @param      None
 * @return     true on success else false
 *
 *******************************************************************
 */
bool CPndrBTFrameTxRx::Connect()
{
    LOG_TRACE << "In CPndrBTFrameTxRx:: Connect() Entry"<< LOG_ENDL;

#ifdef DHAVN_PANDORA_BT_ENABLED

    //Call the SPPConnect of BTController

    bool ret = true;

    m_pDBusInterface->SPPConnect();

    LOG_TRACE << "In CPndrBTFrameTxRx:: Connect() Exit"<< LOG_ENDL;
#else
    bool ret = true;
#endif
    return ret;

}

/**
 *******************************************************************
 * Send the data over currently active transport
 *
 * @param[in]   inData: The data to be sent
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrBTFrameTxRx::Send(const QByteArray& inData)
{
    LOG_TRACE << "In CPndrBTFrameTxRx:: Send() Entry ,CMD ID=" << inData.toHex().mid(14, 2) << " , inData = " << inData.toHex().left(32) << LOG_ENDL;
#ifdef DHAVN_PANDORA_BT_ENABLED

    LOG_INFO<< "CPndrBTFrameTxRx::Send:m_eInitialized : " << m_eInitialized <<LOG_ENDL;
    LOG_INFO<< "m_pDBusInterface->isValid() : " << m_pDBusInterface->isValid() <<LOG_ENDL;
    LogCommandID(inData, true);

    if(m_pDBusInterface->isValid() && m_eInitialized == INIT_COMPLETED)
    {

        //Append the req to a queue so that in case of error req can be resend
        //Do not append to BT queue if already its the current req being processed
        //i.e, when On timeout in validator, it is resending the same request
        if(!m_FailedReqArray.contains (inData))
        {
            m_FailedReqArray.append(inData);
            LOG_INFO<<"APPENDED to BT Queue = " << inData.toHex().left(24)<<LOG_ENDL;
        }
        if(m_FailedReqArray.count() > 1)  // Modified because the log is many.
            LOG_CRITICAL<<"CPndrBTFrameTxRx::m_FailedReqArray.count = " << m_FailedReqArray.count()<<LOG_ENDL;

        //Call Send of BTController and propagate the error if any
        m_pDBusInterface->SPPDataWriteToBT(inData);
    }
    else
    {
        LOG_CRITICAL<<"##### [Send Fail] DBus for BTController Not Connected #####"<<LOG_ENDL;
    }
#endif

    //LOG_TRACE << "In CPndrBTFrameTxRx:: Send() exit"<< LOG_ENDL;

}

/**
 *******************************************************************
 * Disconnect this device
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrBTFrameTxRx::Disconnect()
{
    LOG_TRACE << "In CPndrBTFrameTxRx:: Disconnect() Entry"<< LOG_ENDL;

    m_pDBusInterface->SPPDisconnect();
    LOG_TRACE << "In CPndrBTFrameTxRx:: Disconnect() Exit"<< LOG_ENDL;

}


/**
 *******************************************************************
 * Function for returning the max payload length supported by the
 * BT transport
 *
 * @param[in]   None
 *
 * @return     quint32
 *
 *******************************************************************
 */
quint32 CPndrBTFrameTxRx::GetMaxPayloadLength()
{
    return c_Max_Payload_Length_BT ;
}

/**
 *******************************************************************
 * onSPPConnected
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrBTFrameTxRx::onSPPConnected(bool inConnect)
{
    LOG_SLOT<< "In CPndrBTFrameTxRx:: onSPPConnected() Entry : "<< inConnect << LOG_ENDL;

    if(inConnect)
    {
        //On Success
        m_eInitialized = INIT_COMPLETED;
        emit BTDeviceStatusUpdated(DEVICE_TYPE_SMARTPHONE,true);
    }
    else
    {
        //On error
        m_eInitialized = INIT_NOT_COMPLETED;
        emit BTDeviceStatusUpdated(DEVICE_TYPE_SMARTPHONE,false);
    }
    LOG_SLOT << "In CPndrBTFrameTxRx:: onSPPConnected() Exit"<< LOG_ENDL;


}

/**
 *******************************************************************
 * onSPPDisconnected
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrBTFrameTxRx::onSPPDisconnected(bool inDisconnect)
{
    LOG_SLOT << "In CPndrBTFrameTxRx:: onSPPDisconnected() Entry "<< inDisconnect <<LOG_ENDL;

    //On error
    m_eInitialized = INIT_NOT_COMPLETED;
    emit BTDeviceStatusUpdated(DEVICE_TYPE_SMARTPHONE,false);

    LOG_SLOT << "In CPndrBTFrameTxRx:: onSPPDisconnected() Exit"<< LOG_ENDL;

}

/**
 *******************************************************************
 * onSPPDataReadComplete
 *
 * @param      QByteArray
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrBTFrameTxRx::onSPPDataReadComplete(const QByteArray dataArray)
{
    //LOG_INFO << "In CPndrBTFrameTxRx:: onSPPDataReadComplete() Entry"<< LOG_ENDL;
    if(dataArray[0] ==  0x7e)
    {
        LOG_LOW << "In CPndrBTFrameTxRx:: onSPPDataReadComplete() Entry  ,CMD ID=" << dataArray.toHex().mid(14, 2) << " -> dataArray received is "<< dataArray.toHex().left(24)<< LOG_ENDL;
        LogCommandID(dataArray, true);
    }
    emit BTDataReceived(dataArray);

    //LOG_INFO << "In CPndrBTFrameTxRx:: onSPPDataReadComplete() Exit"<< LOG_ENDL;
}


/**
 *******************************************************************
 * onSPPDataWriteToBTStatus
 *
 * @param      error - Errorcode if any during SPPDataWrite
 *              0- Success, 1- SPPDisconnected, 2- Wrong Packet Size, 3- Invalid Status
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrBTFrameTxRx::onSPPDataWriteToBTStatus(const int error)
{
    if(error != 0) // Modified because the log is many.
        LOG_SLOT << "In CPndrBTFrameTxRx:: onSPPDataWriteToBTStatus() Entry : " << "error = " <<error<< LOG_ENDL;

    switch(error)
    {
    case SPP_DATA_WRITE_SUCCESS:
        {
            LOG_INFO <<"SPPDataWrite Success. " << "error = " <<error<< LOG_ENDL;
            //Remove the req from req queue
            if(m_FailedReqArray.count())
            {
                LOG_INFO <<"Req sent. Remove req from queue.  " << m_FailedReqArray.at(0).toHex().left(24)<< LOG_ENDL;
                m_FailedReqArray.removeAt(0);

                LOG_INFO << "m_FailedReqArray.count() = " <<m_FailedReqArray.count()<< LOG_ENDL;
            }
            break;
        }
    case ERR_SPP_DISCONNECTED:
        {
            LOG_INFO <<"SPPDataWrite error. SPPDisconnected. " << "error = " <<error<< LOG_ENDL;
            //Handled in onSPPDisconected slot
            break;
        }
    case ERR_SPP_INCORRECT_PACKET_SIZE:
        {
            LOG_INFO <<"SPPDataWrite error. Wrong packet size. " << "error = " <<error<< LOG_ENDL;
            break;
        }
    case ERR_SPP_INVALID_STATUS:
        {
            LOG_INFO <<"SPPDataWrite error. Invalid status. " << "error = " <<error<< LOG_ENDL;
            //If BT Write has failed, then re-send the request
            if(m_FailedReqArray.count())
            {
                LOG_CRITICAL<<" Resending request:" << m_FailedReqArray.at(0).toHex().left(24)<<LOG_ENDL;
                Send (m_FailedReqArray.at(0));
            }
            break;
        }
    default:
        {
            LOG_INFO <<"SPPDataWrite Unknown err." << "error = " <<error<< LOG_ENDL;
            break;
        }
    }


    //LOG_INFO << "In CPndrBTFrameTxRx:: onSPPDataWriteToBTStatus() Exit"<< LOG_ENDL;

}

//End Of File
