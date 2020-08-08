
#include "ahaBTFrameTxRx.h"
#include "ahaBTDBusInterface.h"

//BT supports 1024 ,so payload len max possible size for aha is kept //1000
const quint32 c_Max_Payload_Length_BT = 1000;


CAhaBTFrameTxRx::CAhaBTFrameTxRx()
    :m_pDBusInterface(NULL), m_eInitialized(INIT_NOT_COMPLETED)
{
LOG_INIT_V2("Aha");
LOG_HIGH << "In CAhaBTFrameTxRx::CAhaBTFrameTxRx() " <<LOG_ENDL;
}

CAhaBTFrameTxRx::~CAhaBTFrameTxRx()
{
    LOG_HIGH << "In CAhaBTFrameTxRx::~ CAhaBTFrameTxRx() " <<LOG_ENDL;

    if(m_pDBusInterface){
        delete m_pDBusInterface;
        m_pDBusInterface = NULL;
    }

    m_FailedReqArray.clear();

    LOG_HIGH << "In CAhaBTFrameTxRx::~ CAhaBTFrameTxRx() Exit" <<LOG_ENDL;
}

EAhaError CAhaBTFrameTxRx::Connect(EDeviceType inDeviceType)
{
    EAhaError nAhaError = E_SUCCESS;

    LOG_HIGH << "In CAhaBTFrameTxRx:: Connect() Entry" <<LOG_ENDL;

    m_pDBusInterface->SPPConnect();

    LOG_HIGH << "In CAhaBTFrameTxRx:: Connect() Exit" <<LOG_ENDL;

    return nAhaError;

}

void CAhaBTFrameTxRx::Disconnect()
{
    //disconnect signal so it will no longer receive any data
    LOG_HIGH << "In CAhaBTFrameTxRx:: Disconnect() ....................................." <<LOG_ENDL;
    m_pDBusInterface->SPPDisconnect();
}

EAhaError CAhaBTFrameTxRx::Init()
{
    CAhaAbsFrameTxRx::Init();
    LOG_HIGH << "In CAhaBTFrameTxRx:: Init() Entry" <<LOG_ENDL;

    m_eInitialized = INIT_NOT_COMPLETED;

    EAhaError ret  = E_SUCCESS;

    m_pDBusInterface = new CAhaBTDBusInterface(CAhaBTDBusInterface::staticServiceName(),
                                               CAhaBTDBusInterface::staticObjectName(),
                                               QDBusConnection::sessionBus(),0);

    if(m_pDBusInterface && m_pDBusInterface->isValid())
    {
        LOG_HIGH << "connection to dbus is valid " <<LOG_ENDL;
        qint8 dBusSignalConnect = connect(m_pDBusInterface,SIGNAL(SPPConnected(bool)),this,SLOT(onSPPConnected(bool)));
        dBusSignalConnect &=      connect(m_pDBusInterface,SIGNAL(SPPDisconnected(bool)),this,SLOT(onSPPDisconnected(bool)));
        dBusSignalConnect &=      connect(m_pDBusInterface,SIGNAL(SPPDataReadComplete(const QByteArray)),this,SLOT(onSPPDataReadComplete(const QByteArray)));
        dBusSignalConnect &=      connect(m_pDBusInterface,SIGNAL(SPPDataWriteToBTStatus(const int )),this,SLOT(onSPPDataWriteToBTStatus(const int)));
        LOG_HIGH << "DBus signal connections : %d\n" << dBusSignalConnect  <<LOG_ENDL;

        if(!dBusSignalConnect)
        {
            ret = E_INIT_FAILED;
            LOG_HIGH << "could not connect to Dbus signal " <<LOG_ENDL;
        }
    }
    else
    {
        ret = E_INIT_FAILED;
        LOG_HIGH << "connection to dbus is not valid " <<LOG_ENDL;
    }

    connect(this, SIGNAL(DataReceived(QByteArray)), this, SLOT(onDataReceived(QByteArray)));

    LOG_HIGH << "In CAhaBTFrameTxRx:: Init() Exit" <<LOG_ENDL;

    return ret;
}

void CAhaBTFrameTxRx::DeInit()
{
    CAhaAbsFrameTxRx::DeInit();
    LOG_HIGH << "In CAhaBTFrameTxRx:: DeInit() Entry" <<LOG_ENDL;

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
    LOG_HIGH << "In CAhaBTFrameTxRx:: DeInit() Exit" <<LOG_ENDL;

    disconnect(this, SIGNAL(DataReceived(QByteArray)), this, SLOT(onDataReceived(QByteArray)));
}

void CAhaBTFrameTxRx::requestApplicationLaunch()
{
//    if(m_pIPodAppController == NULL)
//        return;

//    if(!m_bLaunchRequested)
//    {
//        DEBUG_USBFRAME("Trying to launch the application!");
//        m_bLaunchRequested = true;
//        m_pIPodAppController->LaunchApplication(c_BundleAppId);
//    }
//    else
//    {
//        DEBUG_USBFRAME("WOW! How come it tries to launch again!\n");
//    }
}

void CAhaBTFrameTxRx::sendData(const QByteArray inData)
{
    //LOG_HIGH << "CAhaBTFrameTxRx::send2Data in.." <<LOG_ENDL;

    m_ReplyWaitTimer->start(TIME_REPLYWAIT_BT);

    if(m_pDBusInterface->isValid() && m_eInitialized == INIT_COMPLETED)
    {

        //Append the req to a queue so that in case of error req can be resend
        //Do not append to BT queue if already its the current req being processed
        //i.e, when On timeout in validator, it is resending the same request
        if(!m_FailedReqArray.contains (inData))
        {
            m_FailedReqArray.append(inData);
            //LOG_HIGH << "APPENDED to BT Queue " << inData.toHex() <<LOG_ENDL;
        }
        //LOG_HIGH << "m_FailedReqArray.count = " << m_FailedReqArray.count()  <<LOG_ENDL;

        //Call Send of BTController and propagate the error if any
        m_pDBusInterface->SPPDataWriteToBT(inData);
    }
    else
    {
        LOG_HIGH << "DBus for BTController Not Connected" <<LOG_ENDL;
        m_ReplyWaitTimer->stop();
    }
}

quint32 CAhaBTFrameTxRx::GetMaxPayloadLength()
{
    return c_Max_Payload_Length_BT ;
}

void CAhaBTFrameTxRx::onSPPConnected(bool inConnect)
{
    LOG_HIGH << "In CAhaBTFrameTxRx:: onSPPConnected() Entry - " << inConnect  <<LOG_ENDL;

    if(inConnect)
    {
        //On Success
        m_eInitialized = INIT_COMPLETED;
        //emit BTDeviceStatusUpdated(DEVICE_TYPE_SMARTPHONE,true);
        emit ahaAppInitCompleted();
    }
    else
    {
        //On error
        m_eInitialized = INIT_NOT_COMPLETED;
        //emit BTDeviceStatusUpdated(DEVICE_TYPE_SMARTPHONE,false);
        emit ahaSessionClosed();
    }
    LOG_HIGH << "In CAhaBTFrameTxRx:: onSPPConnected() Exit" <<LOG_ENDL;
}

void CAhaBTFrameTxRx::onSPPDisconnected(bool inDisconnect)
{
    LOG_HIGH << "In CAhaBTFrameTxRx:: onSPPDisconnected() Entry "<< inDisconnect <<LOG_ENDL;

    //On error
    m_eInitialized = INIT_NOT_COMPLETED;
    //emit BTDeviceStatusUpdated(DEVICE_TYPE_SMARTPHONE,false);
    emit ahaSessionClosed();

    LOG_HIGH << "In CAhaBTFrameTxRx:: onSPPDisconnected() Exit" <<LOG_ENDL;

}

void CAhaBTFrameTxRx::onSPPDataReadComplete(const QByteArray dataArray)
{
    //LOG_HIGH << "In CAhaBTFrameTxRx:: onSPPDataReadComplete() Entry" <<LOG_ENDL;

//    LOG_HIGH << "dataArray received is %x\n", dataArray.toHex());

    //emit BTDataReceived(dataArray);
    emit DataReceived(dataArray);

    LOG_HIGH << "In CAhaBTFrameTxRx:: onSPPDataReadComplete() Exit" <<LOG_ENDL;

}

void CAhaBTFrameTxRx::onSPPDataWriteToBTStatus(const int error)
{
    //LOG_HIGH << "In CAhaBTFrameTxRx:: onSPPDataWriteToBTStatus(taWriteToBTStatus() Entry" <<LOG_ENDL;

    switch(error)
    {
    case SPP_DATA_WRITE_SUCCESS:
        {
            LOG_HIGH << "SPPDataWrite Success. error = " << error <<LOG_ENDL;
            //Remove the req from req queue
            if(m_FailedReqArray.count())
            {
//                LOG_HIGH << "Req sent. Remove req from queue.  %x\n" , m_FailedReqArray.at(0).toHex());
                m_FailedReqArray.removeAt(0);

                LOG_HIGH << "m_FailedReqArray.count() = " << m_FailedReqArray.count()  <<LOG_ENDL;
            }

            break;
        }
    case ERR_SPP_DISCONNECTED:
        {
            LOG_HIGH << "SPPDataWrite error. SPPDisconnected. error = %d" << error <<LOG_ENDL;
            //Handled in onSPPDisconected slot
            break;
        }
    case ERR_SPP_INCORRECT_PACKET_SIZE:
        {
            LOG_HIGH << "SPPDataWrite error. Wrong packet size. error = %d" << error <<LOG_ENDL;
            break;
        }
    case ERR_SPP_INVALID_STATUS:
        {
            LOG_HIGH << "SPPDataWrite error. Invalid status. error = %d" << error <<LOG_ENDL;
            //If BT Write has failed, then re-send the request
            if(m_FailedReqArray.count())
            {
//                LOG_CRITICAL<<" Resending request:" << m_FailedReqArray.at(0).toHex()<<LOG_LOG_ENDL;
                sendData (m_FailedReqArray.at(0));
            }
            break;
        }
    default:
        {
            LOG_HIGH << "SPPDataWrite Unknown err.error = %d" << error <<LOG_ENDL;
            break;
        }
    }
    //LOG_HIGH << "In CAhaBTFrameTxRx:: onSPPDataWriteToBTStatus() Exit" <<LOG_ENDL;
}
