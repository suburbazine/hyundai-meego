#include <DHAVN_IPodController_iPodDBSyncThread.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_Def.h>
#include <DHAVN_IPodController_PrivateData.h>

CiPodDBSyncThread::CiPodDBSyncThread(QObject* controller,CTrackerConnector *trackerConnector):QThread(controller)
{
    iPodControllerPrivateData *iPodPrivateData = controller->findChild<iPodControllerPrivateData*>(gControllerPrivateData);
    EIPodPorts portNum;
    portNum = iPodPrivateData->getiPodPortNum();
    switch(portNum){
    case eIPOD_PORT_1:
        LOG_INIT_V2(giPodControllerLogName_1);
        break;
    case eIPOD_PORT_2:
        LOG_INIT_V2(giPodControllerLogName_2);
        break;
    default:
         LOG_INIT_V2(giPodControllerLogName);
    }
    //LOG_INIT_V2(gControllerLogName);
    m_isDisconnected = false;   // added by sungha.choi 2013.04.15
    m_trackerConnector = trackerConnector;

    m_iAPWriter = ((CIpodController*)controller)->m_Writer;
    m_iAPReader = ((CIpodController*)controller)->m_Reader;

    connect( this,                        SIGNAL(startDBSync()),   trackerConnector,    SLOT(startDBSyncEC()) );
    connect( m_trackerConnector,          SIGNAL(dbThreadQuit()),  this,                SLOT(quit()) );
    connect( this,                        SIGNAL(getArtWork(QStringList,QStringList, bool)), m_trackerConnector, SLOT(getArtworkEC(QStringList, QStringList, bool)));
    connect( this,                        SIGNAL(requestQuit()),   trackerConnector,    SLOT(requestQuitSlot()));  // added by sungha.choi 2013.04.16
    connect( trackerConnector,            SIGNAL(requestQuit()),   trackerConnector,    SLOT(requestQuitSlot()), Qt::QueuedConnection);  // added by sungha.choi 2013.04.16 this connection must be  Qt::QueuedConnection 2013.04.16
    bool ret1 = connect(this, SIGNAL(OnChangeDBIndexingStatus(bool)), m_trackerConnector, SLOT(OnChangeDBIndexingStatus(bool)));
    bool ret2 = connect(m_trackerConnector, SIGNAL(OnDBIndexingStatusChanged(bool)), this, SIGNAL(OnDBIndexingStatusChanged(bool)));
    //LOG_TRACE << "PLAYLIST: conneting the signal slots: ret1: " << ret1 << "  ret2: " << ret2 << LOG_ENDL;
}

CiPodDBSyncThread::~CiPodDBSyncThread()
{
    ////LOG_TRACE <<"[SHCHOI5] CiPodDBSyncThread::~CiPodDBSyncThread Thread ID = " << QThread::currentThreadId() << LOG_ENDL;
}

void CiPodDBSyncThread::run()
{
    try
    {
        //emit startDBSync(); // deleted by sungha.choi 2013.04.15
        this->exec();
        m_trackerConnector->cleanUPOnSyncComplete();
        ////LOG_TRACE <<"[SHCHOI5] CiPodDBSyncThread::run == finished  Thread ID = " << QThread::currentThreadId() << LOG_ENDL;
    }
    catch(EIPodErrors ex){
    }
}

void CiPodDBSyncThread::stopDBSync()
{
    m_isDisconnected = true;    // added by sungha.choi 2013.04.15
    m_trackerConnector->stopCurrentTask();
    ////LOG_TRACE <<"[SHCHOI5] CiPodDBSyncThread::stopDBSync Thread ID = " << QThread::currentThreadId() << LOG_ENDL;
    emit requestQuit();
    //quit(); // deleted by sungha.choi 2013.04.02, quit() is called by iPodcontroller object
    ////LOG_TRACE <<"CiPodDBSyncThread::stopDBSync Exit"<<LOG_ENDL;
}

void CiPodDBSyncThread::requestDBSync() // added by sungha.choi 2013.04.16
{
    //LOG_HIGH << "requestDBSync" << LOG_ENDL;
    ////LOG_TRACE <<"[SHCHOI5] CiPodDBSyncThread::requestDBSync Thread ID = " << QThread::currentThreadId() << LOG_ENDL;
    emit startDBSync();
}

void CiPodDBSyncThread::fetchArtwork(QStringList albumList, QStringList artistList, bool forceFlage)
{
    //LOG_TRACE << "TanLog CiPodDBSyncThread::fetchArtwork" << LOG_ENDL;
    emit getArtWork(albumList, artistList, forceFlage);
}

