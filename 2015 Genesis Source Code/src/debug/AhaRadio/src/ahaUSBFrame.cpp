
#include "ahaUSBFrame.h"

CIPodAppController* pStaticIPodAppController_1 = NULL;
CIPodAppController* pStaticIPodAppController_2 = NULL;

CAhaUSBFrameTxRx::CAhaUSBFrameTxRx()
{
    LOG_INIT_V2("Aha");
    m_pIPodAppController = NULL;

    LOG_INIT_V2("Aha");
}

CAhaUSBFrameTxRx::~CAhaUSBFrameTxRx()
{
    LOG_HIGH<<"[ CAhaUSBFrameTxRx::~CAhaUSBFrameTxRx ] call destructor "<<LOG_ENDL;
    m_pIPodAppController = NULL;
}

EAhaError CAhaUSBFrameTxRx::Connect(EDeviceType inDeviceType)
{
    EAhaError nAhaError;
    nAhaError = GetIpodAppControllerInstance(inDeviceType);
    return nAhaError;
}

void CAhaUSBFrameTxRx::Disconnect()
{
    //disconnect signal so it will no longer receive any data
    DisconnectIPodAppControllerSignal(m_pIPodAppController);
    m_pIPodAppController = NULL;
}

EAhaError CAhaUSBFrameTxRx::GetIpodAppControllerInstance(EDeviceType inDeviceType)
{
    EAhaError nAhaError = E_SUCCESS;

    switch(inDeviceType)
    {
    case DEVICE_TYPE_IPHONE_USB_FRONT:
        //make sure we disconnect others signal
        DisconnectIPodAppControllerSignal(pStaticIPodAppController_2);
        if(pStaticIPodAppController_1)
        {
            TAN_PRINTF("GetIpodAppControllerInstance: pStaticIPodAppController_1\n");
            m_pIPodAppController = pStaticIPodAppController_1;
            ConnectIPodAppControllerSignal(pStaticIPodAppController_1);
            onIPodAppControllerInitCompletion();
        }
        else
        {
            pStaticIPodAppController_1 = CreateNewIpodAppControllerInstance(eIPOD_PORT_1);
            m_pIPodAppController = pStaticIPodAppController_1;
            if(m_pIPodAppController == NULL)
            {
                nAhaError = E_INIT_FAILED;
                TAN_PRINTF("GetIpodAppControllerInstance: eIPOD_PORT_1 failed!\n");
            }
        }
        break;
    case DEVICE_TYPE_IPHONE_USB_REAR:
        //make sure we disconnect others signal
        DisconnectIPodAppControllerSignal(pStaticIPodAppController_1);
        if(pStaticIPodAppController_2)
        {
            TAN_PRINTF("GetIpodAppControllerInstance: pStaticIPodAppController_2!\n");
            m_pIPodAppController = pStaticIPodAppController_2;
            ConnectIPodAppControllerSignal(pStaticIPodAppController_2);
            onIPodAppControllerInitCompletion();
        }
        else
        {
            pStaticIPodAppController_2 = CreateNewIpodAppControllerInstance(eIPOD_PORT_2);
            m_pIPodAppController = pStaticIPodAppController_2;
            if(m_pIPodAppController == NULL)
            {
                nAhaError = E_INIT_FAILED;
                TAN_PRINTF("GetIpodAppControllerInstance: eIPOD_PORT_2 failed!\n");
            }
        }
        break;
    default:
        break;
    }
    return nAhaError;
}

CIPodAppController* CAhaUSBFrameTxRx::CreateNewIpodAppControllerInstance(EIPodPorts eIPodPort)
{
    //create m_pIPodAppController instances
    TAN_PRINTF("CreateNewIpodAppControllerInstance\n");
    CIPodAppController* pIpodAppInstance;
    EIPodAppErrors nIPodAppError;
    pIpodAppInstance = new CIPodAppController();
    if(pIpodAppInstance)
    {
        ConnectIPodAppControllerSignal(pIpodAppInstance);
        nIPodAppError = pIpodAppInstance->Init(c_AppId,c_BundleSeedId,eIPodPort, EAP_AHA_PROTOCOL_INDEX);
        TAN_PRINTF("CAhaUSBFrame::Connect eIPodPort: %d\n", eIPodPort);

        if(IPOD_APPSUCCESS == nIPodAppError)
        {
            emit ahaAppInitCompleted();
            TAN_PRINTF("nIPodAppError init success, %d\n", eIPodPort);
            return pIpodAppInstance;
        }
        else
        {
            TAN_PRINTF("nIPodAppError init failed with error code: %d\n", nIPodAppError);
            //emit iPodInitTimeOut(); <-- do not need to emit this.
            //put here for future reference.
            delete pIpodAppInstance;
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

void CAhaUSBFrameTxRx::DestroyIpodAppControllerInstance(EDeviceType inDeviceType)
{
    //1. make sure that the m_pIPodAppController does not point to the deleted instance
    //2. deinit the instance
    //3. delete the instance
    switch(inDeviceType)
    {
    case DEVICE_TYPE_IPHONE_USB_FRONT:
        if(m_pIPodAppController == pStaticIPodAppController_1)
        {
            m_pIPodAppController = NULL;
        }
        if(pStaticIPodAppController_1)
        {
            pStaticIPodAppController_1->D_Init();
            delete pStaticIPodAppController_1;
            pStaticIPodAppController_1 = NULL;
        }
        break;
    case DEVICE_TYPE_IPHONE_USB_REAR:
        if(m_pIPodAppController == pStaticIPodAppController_2)
        {
            m_pIPodAppController = NULL;
        }
        if(pStaticIPodAppController_2)
        {
            pStaticIPodAppController_2->D_Init();
            delete pStaticIPodAppController_2;
            pStaticIPodAppController_2 = NULL;
        }
        break;
    default:
        break;
    }
}

void CAhaUSBFrameTxRx::ConnectIPodAppControllerSignal(CIPodAppController* pIpodAppInstance)
{
    if(pIpodAppInstance)
    {
        //connect(pIpodAppInstance, SIGNAL(InitComplete()), this, SLOT(onIPodAppControllerInitCompletion()));
        connect(pIpodAppInstance, SIGNAL(DataReceived(QByteArray)), this , SLOT(onDataReceived(QByteArray)));
        connect(pIpodAppInstance, SIGNAL(DeviceDisconnected(int)), this, SLOT(onDeviceDisconnected(int)));
        connect(pIpodAppInstance, SIGNAL(NotifySessionClosed()), this , SLOT(onNotifySessionClosed()));
        connect(pIpodAppInstance, SIGNAL(AudioSampleRateChanged(int)), this , SLOT(onAudioSampleRateChanged(int)));
        connect(pIpodAppInstance, SIGNAL(SessionOpened(int)), this , SLOT(onSessionOpened(int)));
        connect(pIpodAppInstance, SIGNAL(Acknowledgement(EIPodAppErrors)), this ,SLOT(onAcknowledgement(EIPodAppErrors)));
    }
}

void CAhaUSBFrameTxRx::DisconnectIPodAppControllerSignal(CIPodAppController* pIpodAppInstance)
{
    if(pIpodAppInstance)
    {
        //disconnect(pIpodAppInstance, SIGNAL(InitComplete()), this, SLOT(onIPodAppControllerInitCompletion()));
        disconnect(pIpodAppInstance, SIGNAL(DataReceived(QByteArray)), this , SLOT(onDataReceived(QByteArray)));
        disconnect(pIpodAppInstance, SIGNAL(DeviceDisconnected(int)), this, SLOT(onDeviceDisconnected(int)));
        disconnect(pIpodAppInstance, SIGNAL(NotifySessionClosed()), this , SLOT(onNotifySessionClosed()));
        disconnect(pIpodAppInstance, SIGNAL(AudioSampleRateChanged(int)), this , SLOT(onAudioSampleRateChanged(int)));
        disconnect(pIpodAppInstance, SIGNAL(SessionOpened(int)), this , SLOT(onSessionOpened(int)));
        disconnect(pIpodAppInstance, SIGNAL(Acknowledgement(EIPodAppErrors)), this ,SLOT(onAcknowledgement(EIPodAppErrors)));
    }
}

void CAhaUSBFrameTxRx::requestApplicationLaunch()
{
    if(m_pIPodAppController == NULL)
        return;

    if(!m_bLaunchRequested)
    {
        LOG_HIGH <<"Trying to launch the application!" << LOG_ENDL;
        m_bLaunchRequested = true;
        m_pIPodAppController->LaunchApplication(c_BundleAppId);
    }
    else
    {
        LOG_HIGH <<"WOW! How come it tries to launch again!\n" << LOG_ENDL;
    }
}

void CAhaUSBFrameTxRx::sendData(const QByteArray data)
{
    if(NULL == m_pIPodAppController)
    {
        LOG_HIGH<<"[ CAhaUSBFrameTxRx::sendData ] m_pIPodAppController is null..."<<LOG_ENDL;
        return;
    }

    EIPodAppErrors err = m_pIPodAppController->Send(data);
//        LOG_HIGH<<"AHA_OPCODE_START_PEER_SESSION" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_START_PEER_SESSION \n");
//        LOG_HIGH<<"AHA_OPCODE_QUERY_CURRENT_STATION" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_QUERY_CURRENT_STATION \n");
//        LOG_HIGH<<"AHA_OPCODE_QUERY_STATIONS" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_QUERY_STATIONS \n");
//        LOG_HIGH<<"AHA_OPCODE_QUERY_STATION_PARAMETERS" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_QUERY_STATION_PARAMETERS \n");
//        LOG_HIGH<<"AHA_OPCODE_QUERY_STATION_CONTENT" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_QUERY_STATION_CONTENT \n");
//        LOG_HIGH<<"AHA_OPCODE_QUERY_CONTENT_PARAMETERS" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_QUERY_CONTENT_PARAMETERS \n");
//        LOG_HIGH<<"AHA_OPCODE_QUERY_BINARY_DATA" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_QUERY_BINARY_DATA \n");
//        LOG_HIGH<<"AHA_OPCODE_SELECT_STATION_FOR_PLAYBACK" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_SELECT_STATION_FOR_PLAYBACK \n");
//        LOG_HIGH<<"AHA_OPCODE_SELECT_CONTENT_FOR_PLAYBACK" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_SELECT_CONTENT_FOR_PLAYBACK \n");
//        LOG_HIGH<<"AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE \n");
//        LOG_HIGH<<"AHA_OPCODE_TIME_SHIFT_CONTENT" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_TIME_SHIFT_CONTENT \n");
//        LOG_HIGH<<"AHA_OPCODE_PERFORM_ACTION" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_PERFORM_ACTION \n");
//        LOG_HIGH<<"AHA_OPCODE_REQUEST_CURRENT_CONTENT_INFORMATION" << LOG_ENDL;
//        TAN_PRINTF("AHA_OPCODE_REQUEST_CURRENT_CONTENT_INFORMATION \n");

//    LOG_HIGH<<"m_pIPodAppController->Send(data)" << LOG_ENDL;
//    TAN_PRINTF("m_pIPodAppController->Send(data) \n");

    m_ReplyWaitTimer->start(TIME_REPLYWAIT);

    switch(err)
    {
    case IPOD_APPSUCCESS:
        LOG_HIGH <<"IPOD_APPSUCCESS \n" << LOG_ENDL;
        break;
    case IPOD_APPERROR_TIMEOUT:
        LOG_HIGH <<"IPOD_APPERROR_TIMEOUT \n" << LOG_ENDL;
        break;
    case IPOD_APPERROR_NOTINITIALIZED:
        LOG_HIGH <<"IPOD_APPERROR_NOTINITIALIZED\n" << LOG_ENDL;
        break;
    case IPOD_APPERROR_NOTRESPONDING:
        LOG_HIGH <<"IPOD_APPERROR_NOTRESPONDING\n" << LOG_ENDL;
        break;
    case IPOD_APPERROR_NOTCONNECTED:
        LOG_HIGH <<"IPOD_APPERROR_NOTCONNECTED\n" << LOG_ENDL;
        break;
    case IPOD_APPERROR_APPNOTOPENED:
        LOG_HIGH <<"IPOD_APPERROR_APPNOTOPENED\n" << LOG_ENDL;
        resetReplyWaitTimer(generateUniqueQueryID(false));
        emit ahaAppNotOpened();
        break;
    case IPOD_APPERROR_BUSY:
        LOG_HIGH <<"IPOD_APPERROR_BUSY\n" << LOG_ENDL;
        break;
    case IPOD_APPERROR_SESSIONCLOSED:
        LOG_HIGH <<"IPOD_APPERROR_SESSIONCLOSED\n" << LOG_ENDL;
        break;
    default:
        LOG_HIGH <<"default\n" << LOG_ENDL;
        break;
    }
}

int CAhaUSBFrameTxRx::onIPodAppControllerInitCompletion()
{
    TAN_PRINTF("CAhaUSBFrameTxRx::onIPodAppControllerInitCompletion() \n");

    LOG_HIGH <<"Function onIPodAppControllerInitCompletion\n" << LOG_ENDL;
    //temporary source
    if(!m_bInitCompletionReceived)
    {
        m_bInitCompletionReceived = true;
        emit ahaAppInitCompleted();
    }
    return 0;
}

    //LOG_HIGH<<"CAhaUSBFrameTxRx::play() [Time = "<< (QTime::currentTime()).toString("hh.mm.ss.zzz")<<"]"<<LOG_ENDL;
    //LOG_HIGH<<"CAhaUSBFrameTxRx::stop() [Time = "<< (QTime::currentTime()).toString("hh.mm.ss.zzz")<<"]"<<LOG_ENDL;
    //LOG_HIGH<<"CAhaUSBFrameTxRx::pause() [Time = "<< (QTime::currentTime()).toString("hh.mm.ss.zzz")<<"]"<<LOG_ENDL;
int CAhaUSBFrameTxRx::onDeviceDisconnected(int ipodNumber)
{
    LOG_HIGH <<"onDeviceDisconnected, ipodNumber: " << ipodNumber << LOG_ENDL;
	return 0;
}

int CAhaUSBFrameTxRx::onNotifySessionClosed()
{
    LOG_HIGH <<"onNotifySessionClosed\n" << LOG_ENDL;
    emit ahaSessionClosed();
    return 0;
}

int CAhaUSBFrameTxRx::onAudioSampleRateChanged(int nSampleRate)
{
    LOG_HIGH <<"Sample rate changed: " << nSampleRate << LOG_ENDL;
    TAN_PRINTF("CAhaUSBFrameTxRx::onAudioSampleRateChanged %d\n ", nSampleRate);

    emit ahaSampleRateChanged(nSampleRate);
    return 0;
}

int CAhaUSBFrameTxRx::onSessionClosed(EIPodAppErrors err)
{
    LOG_HIGH <<"onSessionClosed, err," << err << LOG_ENDL;
    emit ahaSessionClosed();
    return 0;
}

int CAhaUSBFrameTxRx::onSessionOpened(int protocolIndex)
{
    LOG_HIGH <<"Session opened with protocol index :" << protocolIndex << LOG_ENDL;
    //hard coded this, will have to talk to IPodController owner to ask them to make
    //a define enum for this. Otherwise, disaster.
    if(0x02 == protocolIndex)
    {
        emit ahaSessionOpened();
    }
    return 0;
}

int CAhaUSBFrameTxRx::onAcknowledgement(EIPodAppErrors status)
{
    if(IPOD_APPSUCCESS == status)
    {
        LOG_HIGH <<"onAcknowledgement IPOD_APPSUCCESS\n" << LOG_ENDL;
    }
    else
    {
        LOG_HIGH <<"onAcknowledgement Not Success\n" << LOG_ENDL;
    }
    return 0;
}
