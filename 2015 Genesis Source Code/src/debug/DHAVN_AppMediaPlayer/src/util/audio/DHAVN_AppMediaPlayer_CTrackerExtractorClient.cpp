#include "util/audio/DHAVN_AppMediaPlayer_CTrackerExtractorClient.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h" // added by eugeny.novikov 2012.12.07 for "AVP hangs in pause/resume Miner during Copy" issue

// { modified by eugeny.novikov 2012.12.07 for "AVP hangs in pause/resume Miner during Copy" issue
CTrackerExtractorClient::CTrackerExtractorClient(QObject *parent) :
    QObject(parent),
    connection(QDBusConnection::connectToBus(QDBusConnection::SessionBus,
                                             "org.lge.TrackerExtractor")),
    m_ExtractorAsync(new TrackerExtractorThread()) // modified by ravikanth 16-07-13 for ISV 87299, 86965
{
    if (!connection.isConnected())
    {
        MP_LOG << "Could not connect to TrackerExtractor!" << LOG_ENDL;
    }
    else
    {
        QStringList serviceNames = connection.interface()->registeredServiceNames();
        MP_LOG << "Registered DBUS services:" << serviceNames << LOG_ENDL;

        bool result;

        // { modified by eugene 2012.11.09 for emitting miner start signal with usb path
        //result = connection.connect("org.lge.TrackerExtractor", "/", "org.lge.TrackerExtractor", "MiningStatus", this,
        //                            SLOT(onMinerStatus(QString,QString)));
        result = connection.connect("org.lge.TrackerExtractor", "/", "org.lge.TrackerExtractor", "MiningStatus", this,
                                    SLOT(onMinerStatus(QString,QString,QString)));
        // } modified by eugene 2012.11.09 for emitting miner start signal with usb path
        if (!result)
            MP_LOG << "Unable to connect signal MinerProgress!" << LOG_ENDL;

        result = connection.connect("org.lge.TrackerExtractor", "/", "org.lge.TrackerExtractor", "MiningStatusInDetail", this,
                                    SLOT(onMinerStatusInDetail(QString,QString,int)));
        if (!result)
           MP_LOG << "Unable to connect signal MiningStatusInDetail!" << LOG_ENDL;

        result = connection.connect("org.lge.TrackerExtractor", "/", "org.lge.TrackerExtractor", "UseQDir", this,
                                    SIGNAL(needToUseQDir(QString)));
        if (!result)
           MP_LOG << "Unable to connect signal UseQDir!" << LOG_ENDL;

        // { added by honggi.shin 2014.02.13 for testing Dbus
        result = connection.connect("org.lge.TrackerExtractor", "/", "org.lge.TrackerExtractor", "DBusSendTest", this,
                                    SIGNAL(onDBusSendTest()));
        if (!result)
           MP_LOG << "Unable to connect signal DBusSendTest!" << LOG_ENDL;
        // } added by honggi.shin 2014.02.13 for testing Dbus. END
    }
    m_bIsMinerStarted = false;
    m_bIsMinerReady = false;
    m_bIsJukeboxMiningFinished = false; // added by eugene 2012.11.14 for jukebox miner status
    m_isMinerStatePaused = false;

	// modified by ravikanth 16-07-13 for ISV 87299, 86965
    m_ExtractorAsync->moveToThread(&m_ExtractorThread);
    m_ExtractorThread.setObjectName("TrackerExtractorThread");   // added by sungha.choi 2013.07.19
    m_ExtractorThread.start(QThread::IdlePriority);
    connect(this, SIGNAL(pauseMinerAsync()), m_ExtractorAsync, SLOT(pauseMinerAsync()), Qt::QueuedConnection);
    connect(this, SIGNAL(resumeMinerAsync()), m_ExtractorAsync, SLOT(resumeMinerAsync()), Qt::QueuedConnection);
    connect(this, SIGNAL(dbusTestAckAsync()), m_ExtractorAsync, SLOT(dbusTestAckAsync()), Qt::QueuedConnection); // added by honggi.shin 2014.02.13 for testing Dbus
}

// { modified by eugene 2012.11.09 for emitting miner start signal with usb path
//void CTrackerExtractorClient::onMinerStatus(QString minerType, QString minerStatus)
void CTrackerExtractorClient::onMinerStatus(QString minerType, QString minerStatus, QString usbMiningPath)
{
    MP_LOG << "Mining status for" << minerType << "is" << minerStatus << LOG_ENDL;
    MP_LOG << "usbMiningPath" << usbMiningPath << LOG_ENDL;
    // } modified by eugene 2012.11.09 for emitting miner start signal with usb path

    if (minerStatus.contains("Finished"))
    {
        if (false == m_bIsMinerStarted)
        {
           // { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
           //emit minerFinished(false);
           emit minerFinished(false , usbMiningPath);
           // { modified by  yongkyun.lee 
        }
        // { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
        //emit minerFinished(true);
        emit minerFinished(true , usbMiningPath);
        // { modified by  yongkyun.lee 
        m_bIsMinerStarted = false;
    }
    else if ((minerStatus.contains("inProgress") && false == m_bIsMinerStarted) ||
              minerStatus.contains("Started"))
    {
       // { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
       //emit minerFinished(false);
       emit minerFinished(false , usbMiningPath);
       // { modified by  yongkyun.lee 
       m_bIsMinerStarted = true;
    }
}

void CTrackerExtractorClient::onMinerStatusInDetail(QString minerType, QString minerStatus, int percentage)
{
   MP_LOG << "Mining status for" << minerType << "is" << minerStatus << LOG_ENDL;
   MP_LOG << "Percentage" << percentage << LOG_ENDL;

   if (minerStatus == "inProgress")
   {
       emit minerInProgress(percentage);
       if ( false == m_bIsMinerStarted )
       {
       // { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
       // emit minerFinished(false);
          emit minerFinished(false,"Detail");
       // { modified by  yongkyun.lee 
          m_bIsMinerStarted = true;
       }
   }
}

// { added by honggi.shin 2014.02.13 for testing Dbus
void CTrackerExtractorClient::onDBusSendTest() {
    MP_HIGH << "---------------- SHG DBus Test OK!" << LOG_ENDL;
    //DBusTestAck();
}
// } added by honggi.shin 2014.02.13 for testing Dbus. END

bool CTrackerExtractorClient::isMinerReady()
{
    MP_LOG << LOG_ENDL;
    QDBusInterface interface( "org.lge.TrackerExtractor", "/" );

    QVariant arg1 = "Miner.Files";
    QDBusReply<QString> reply = interface.call( "GetMinerStatus", arg1 );

    if ( reply.isValid() )
    {
        QString minerState = reply.value();
        MP_LOG << "Reply is received, miner status = " << minerState << LOG_ENDL;
        m_bIsMinerReady = ( minerState == "Finished" );
    }
    else
    {
        MP_LOG << "Reply is invalid. Error type =" << reply.error().type() << LOG_ENDL;
        MP_LOG << "Reply is invalid. Error name =" << reply.error().name() << LOG_ENDL;
        MP_LOG << "Reply is invalid. Error message =" << reply.error().message() << LOG_ENDL;
    }

    return m_bIsMinerReady;
}

void
CTrackerExtractorClient::pauseMiner()
{
    MP_LOG << LOG_ENDL;
    m_isMinerStatePaused = true;
	// modified by ravikanth 16-07-13 for ISV 87299, 86965
    //QDBusInterface("org.lge.TrackerExtractor", "/").call(QDBus::NoBlock, "PauseMiner");
    emit pauseMinerAsync();
}

void
CTrackerExtractorClient::resumeMiner()
{
    MP_LOG << m_isMinerStatePaused << LOG_ENDL;
    // modified by ravikanth 16-07-13 for ISV 87299, 86965
    //QDBusInterface("org.lge.TrackerExtractor", "/").call(QDBus::NoBlock, "ResumeMiner");
    if(m_isMinerStatePaused)
    {
        emit resumeMinerAsync();
        m_isMinerStatePaused = false;
    }
}

// { added by honggi.shin 2014.02.13 for testing Dbus
void CTrackerExtractorClient::DBusTestAck() {
    emit dbusTestAckAsync();
}
// } added by honggi.shin 2014.02.13 for testing Dbus. END

// { added by eugene 2012.11.14 for jukebox miner status
bool
CTrackerExtractorClient::getJukeboxStatus()
{
    MP_LOG << LOG_ENDL;
    QDBusInterface interface( "org.lge.TrackerExtractor", "/" );

    QDBusReply<int> reply = interface.call("GetJukeboxStatus");

    if (reply.isValid())
    {
        int jukebox_status = reply.value();
        MP_LOG << "Reply is received, jukebox_status = " << jukebox_status << LOG_ENDL;
        m_bIsJukeboxMiningFinished = ( jukebox_status == 4 );
    }
    else
    {
        MP_LOG << "Reply is invalid. Error type =" << reply.error().type() << LOG_ENDL;
        MP_LOG << "Reply is invalid. Error name =" << reply.error().name() << LOG_ENDL;
        MP_LOG << "Reply is invalid. Error message =" << reply.error().message() << LOG_ENDL;
    }

    return m_bIsJukeboxMiningFinished;
}
// } added by eugene 2012.11.14 for jukebox miner status
// } modified by eugeny.novikov 2012.12.07 for "AVP hangs in pause/resume Miner during Copy" issue

// modified by ravikanth 16-07-13 for ISV 87299, 86965
TrackerExtractorThread::TrackerExtractorThread(QObject* parent) : QObject(parent)
{
    pauseMinerState = false; // unused variable
}

TrackerExtractorThread::~TrackerExtractorThread()
{
}

void TrackerExtractorThread::pauseMinerAsync()
{
    QDBusInterface("org.lge.TrackerExtractor", "/").asyncCall("PauseMiner");
}

void TrackerExtractorThread::resumeMinerAsync()
{
    QDBusInterface("org.lge.TrackerExtractor", "/").asyncCall("ResumeMiner");
}

// { added by honggi.shin 2014.02.13 for testing Dbus
void TrackerExtractorThread::dbusTestAckAsync()
{
    QDBusInterface("org.lge.TrackerExtractor", "/").asyncCall("DBusTestAck");
}
// } added by honggi.shin 2014.02.13 for testing Dbus. END
