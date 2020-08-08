#include <DHAVN_IPodController_PrivateData.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QFile>
#include <DHAVN_TrackerAbstractor.h>

const char * const AUDIOROUTER_DBUS_NAME           = "com.lg.qt.audiorouter";  
const char * const AUDIOROUTER_DBUS_PATH           = "/audiorouter";       
const char * const AUDIOROUTER_DBUS_INTERFACE_NAME = "com.lg.qt.interface.audiorouter";   
const char * const AUDIOROUTER_DBUS_METHOD         = "control";  

iPodControllerPrivateData::iPodControllerPrivateData(QObject* parent): QObject(parent), m_Mutex(QMutex::Recursive)  // modified by sungha.choi 2013.04.22
{
	LOG_INIT_V2("Media_IPodController");

    m_nQueueExitCode = getUniqueExitCode();
    m_nRequestType = RequestType_None;

    m_OSDKey = OSDKey_None;
    m_bBreakNextLoop = false;
    m_bIsAhaPandoraMode = false;
    m_bIsPreviousAhaPandoraMode = false;
    m_portresetFlag = false;
    m_restartUcsFlag = false;

    m_MediaInfo.Title = " ";
	m_MediaInfo.Album = " ";
	m_MediaInfo.Artist = " ";
	m_MediaInfo.Genre = " ";
	m_MediaInfo.Composer = " ";
	m_MediaInfo.fileName = " ";
	m_MaxPayload = 255 ;
	isUIDSupported = false;
	clearCurrentiPodData();
	m_currentPortNum = eIPOD_PORT_NONE; // added by jungae, 2013.01.15 first time initialization
	setObjectName(gControllerPrivateData);
	m_serialNumber = " ";
	m_CurrentTimeString = " ";
	m_TrackerObj = new CTrackerAbstractor();
	m_CoverArtData.m_IsOnDemandCoverArtRequestInProgress = false;
	m_CoverArtData.m_IsBasicPlayerCoverArtRequestInProgress = false;
	m_TransactionId = 1;
	m_IsiOSAppCommunicationSupported = false;
	m_iPodProductID=0;
    //m_IsTabOff = false;
    m_IsTabOff = true; // Added by jonghwan.cho@lge.com 2013.05.19.
    m_SLeepForPlayWhileFetching = 0;  // Added by pawan 2013.05.03.
    m_isWaitingMutexUnlock = 0; // added by sungha.choi 2013.04.22
    m_TotalIpodTrackCount = 0;    // Added by pawan 2013.05.06.
    m_IsDBIndexingPaused = false;
    m_IsDBIndexingFinished = false;
	// CR 13423
	//HMI event queue clear
	//{
	m_CurrentInProgressEvent = EIdle;
	m_QueueData.clearQueueData();
	m_EventCommandQueue.clear();
	//}
	m_SessionId = -1;
	m_AppId = QString();
	// Added by jonghwan.cho@lge.com	2013.03.29.
	// for static test CR13115
    m_isAutoPlay_PlaylistReady = false;
    m_IsConvertedFromUIDToIndex = false;    // Added by pawan 2013.05.04.
    m_FirstPlayListSet = false;             // Added by pawan 2013.05.04.
//    m_UIDForCoverArt = 0;                   // Added by pawan 2013.05.15. // Removed by pawan 2013.05.15.
    m_IsBTCalling = false; // Added by jonghwan.cho@lge.com 2013.11.05.
    m_IsCurrentPlayingMP = true;
    m_CurrentArtworkRetyCountForOtherMP = 3;
    m_IsRestartOfMPInProgress = false;
    m_MajorVersion = 0;
    m_MinorVersion = 0;
    m_RevisionVersion = 0;
    m_DisplayRemoteLingoVersion = 0.0;

    m_pSendingFadeInTimer = new QTimer();
    m_pSendingFadeInTimer->setInterval(500);
    m_pSendingFadeInTimer->setSingleShot(true);
    connect( m_pSendingFadeInTimer, SIGNAL(timeout()), this, SLOT(SendAudioRouterFadeInSlot()));

	//LOG_TRACE<<"iPodControllerPrivateData::iPodControllerPrivateData:Exit"<<LOG_ENDL;
}

iPodControllerPrivateData::~iPodControllerPrivateData()
{
    m_nRequestType = RequestType_None;

    //LOG_TRACE<<"iPodControllerPrivateData::~iPodControllerPrivateData:Exit"<<LOG_ENDL;
    // Added by jonghwan.cho@lge.com	2013.03.27.
	if( m_TrackerObj ) delete m_TrackerObj;

    if(m_pSendingFadeInTimer)
    {
        disconnect( m_pSendingFadeInTimer, SIGNAL(timeout()), this, SLOT(SendAudioRouterFadeInSlot()));
        if( m_pSendingFadeInTimer->isActive() == true )
        {
            m_pSendingFadeInTimer->stop();
        }
        delete m_pSendingFadeInTimer;
        m_pSendingFadeInTimer = NULL;
    }
}

MediaInfo iPodControllerPrivateData::getCurrentMediaInfo()
{
	return m_MediaInfo;	
}


void iPodControllerPrivateData::updateFileNameWithTrackIndexSlot(int nTrackIndex, quint64 uid)
{
    //this function replace the index -1 with the correct one.
    QString strTemp;
    bool ok;
    QString strUID;
    quint64 currentUID;
    int i, length;
    strTemp.clear();
    strTemp.append(getCurrentMediaInfo().fileName);

    i = strTemp.lastIndexOf("_");
    i = i+1;
    strUID = strTemp.mid(i);

    currentUID = strUID.toULongLong(&ok, 10);

    if(currentUID == uid)
    {
        length = strTemp.length();
        i = strTemp.lastIndexOf("/");
        i = i+1;
        strTemp.remove(i, length - i);
        strTemp.append(QString::number(nTrackIndex));
        strTemp.append("_");
        strTemp.append(strUID);

        m_MediaInfo.fileName.clear();
        m_MediaInfo.fileName.append(strTemp);
    }
}

void iPodControllerPrivateData::setCurrentMediaInfo(MediaInfo &info, bool setCoverArt)
{
    if(setCoverArt)
    {
        m_MediaInfo.coverarturl = info.coverarturl;
    }
    else
    {
        m_MediaInfo.Title = info.Title;
        m_MediaInfo.Album = info.Album ;
        m_MediaInfo.Artist = info.Artist;
        m_MediaInfo.Genre = info.Genre;
        m_MediaInfo.Composer = info.Composer;
        m_MediaInfo.fileName = info.fileName;
    }
}

void iPodControllerPrivateData::resetCurrentMediaInfo()
{
	m_MediaInfo.Title = " ";
	m_MediaInfo.Album = " ";
	m_MediaInfo.Artist = " ";
	m_MediaInfo.Genre = " ";
    m_MediaInfo.Composer = " ";
    m_MediaInfo.fileName = " ";
	m_MediaInfo.coverarturl = " ";
}

void iPodControllerPrivateData::removeSampleRateFile()
{
    QString strPath;
    switch(getiPodPortNum())
    {
    case eIPOD_PORT_1:
        strPath.append(SAMPLE_RATE_FILEPATH_1);
        break;
    case eIPOD_PORT_2:
        strPath.append(SAMPLE_RATE_FILEPATH_2);
        break;
    default:
        break;
    }
    QFile qFile(strPath);

    if(qFile.exists())
    {
        qFile.remove();
    }
}

void iPodControllerPrivateData::updateSampleRateFile(int nSampleRate)
{
    QString strPath;
    switch(getiPodPortNum())
    {
    case eIPOD_PORT_1:
        strPath.append(SAMPLE_RATE_FILEPATH_1);
        break;
    case eIPOD_PORT_2:
        strPath.append(SAMPLE_RATE_FILEPATH_2);
        break;
    default:
        break;
    }

    QFile qFile(strPath);
    bool ret = qFile.open(QIODevice::WriteOnly);
    if(ret)
    {

        //LOG_INFO << "m_IPodPandoraAppSessionID open file is ret = "<< ret << LOG_ENDL;
        QTextStream stream(&qFile);
        stream << nSampleRate <<endl<<flush;
        qFile.close();
    }
}



void iPodControllerPrivateData::clearCurrentiPodData()
{
    resetCurrentMediaInfo();

    m_OSDKey = OSDKey_None;
    m_bBreakNextLoop = false;
    m_bIsAhaPandoraMode = false;
    m_bIsPreviousAhaPandoraMode = false;

    m_IsConvertedFromUIDToIndex = false;  // Added by pawan 2013.05.04.
    m_SLeepForPlayWhileFetching = 0;  // Added by pawan 2013.05.03.
    m_fwVersion = -1;
    m_iOSVersion = -1;
    m_isLogoSupported = false;
    m_isIDPS = true;
    m_generalLingoVersion = -1;
    m_ExtendedLingoVersion = -1;
    m_DigitalLingoVersion = -1;
    m_MaxPayload = 255 ;

//    m_UIDForCoverArt = 0;                     // Added by pawan 2013.05.15.  // Removed by pawan 2013.05.16.
    m_audioDeviceName.clear();
    m_hidDevPath.clear();
    m_TransactionId = 1;
    m_CoverArtData.m_AlbumList.clear();
    m_CoverArtData.m_ArtistList.clear();
    m_CoverArtData.m_IsBasicPlayerCoverArtRequestInProgress = false;
    m_CoverArtData.m_IsOnDemandCoverArtRequestInProgress = false;
    m_CoverArtData.m_RequestedCoverAlbumName.clear();
    m_CoverArtData.m_RequestedCoverArtistName.clear();
    m_FirstPlayListSet = false;   // Added by pawan 2013.05.04.
    m_IsDBIndexingPaused = false;
    m_IsDBIndexingFinished = false;
    m_TotalIpodTrackCount = 0;    // Added by pawan 2013.05.06.
    m_IsBTCalling = false;        // Added by jonghwan.cho@lge.com 2013.11.05.
    // { deleted by jungae, 2013.01.15, it is not need to init.
    //m_currentPortNum = eIPOD_PORT_NONE;
    // } deleted by jungae

    m_IsCurrentPlayingMP = true;
    m_IsRestartOfMPInProgress = false;
    m_CurrentArtworkRetyCountForOtherMP = 3;
    m_MajorVersion = 0;
    m_MinorVersion = 0;
    m_RevisionVersion = 0;
    m_DisplayRemoteLingoVersion = 0.0;
}

QString iPodControllerPrivateData::getCurrentSerialNumber()
{
    return m_serialNumber;
}

void iPodControllerPrivateData::setCurrentSerialNumber(QString serialNumber)
{
    m_serialNumber = serialNumber;
}

void iPodControllerPrivateData::setiPodPortNum( EIPodPorts portNum)
{
    m_currentPortNum = portNum;

//CR15593
    //    initLogger();
    switch(portNum){
    case eIPOD_PORT_1:
        m_CurrentIPodPrt.append(giPodControllerLogName_1);
        break;
    case eIPOD_PORT_2:
        m_CurrentIPodPrt.append(giPodControllerLogName_2);
        break;
    default:
         m_CurrentIPodPrt.append(giPodControllerLogName);
    }
    m_CurrentIPodPrt.append("  ");
    return;
}

EIPodPorts iPodControllerPrivateData::getiPodPortNum()
{
    return m_currentPortNum;
}

QString iPodControllerPrivateData::getPerformanceTime(int milliSecs)
{
    //LOG_TRACE<<"iPodControllerPrivateData::getPerformanceTime:Entry milliSecs = " << milliSecs <<LOG_ENDL;
    m_CurrentTimeString.clear();
    int secs = milliSecs / 1000; milliSecs %= 1000;
    int minutes = secs / 60; secs %= 60;
    int hours = minutes / 60; minutes %= 60;
    //LOG_TRACE<<"iPodControllerPrivateData::getPerformanceTime Exit "<<LOG_ENDL;
    return m_CurrentTimeString.append (QString("%1:%2:%3:%4").arg(hours).arg(minutes).arg(secs).arg(milliSecs) );
}

int iPodControllerPrivateData::getUniqueExitCode()
{
    static int nExitCode = 1000000;
    nExitCode++;
    if(nExitCode > 2000000) //just to prevent overflow
    {
        nExitCode = 1000000;
    }
    return nExitCode;
}

int iPodControllerPrivateData::getTransactionId(bool increment)
{
    if(increment)
        m_TransactionId++;
//    //LOG_TRACE << "transaction ID is: " << m_TransactionId << LOG_ENDL;
    return m_TransactionId;
}
void iPodControllerPrivateData::setTransactionId(qint16 transId)
{
    //LOG_TRACE << "setting transaction ID is: " << transId << LOG_ENDL;
    m_TransactionId = transId;
}

void iPodControllerPrivateData::setAppSessionId(QString appId, qint16 sessionId)
{
    //LOG_TRACE<<"Neelima: iPodControllerPrivateData::setNowPlayingApplicationBundleName :Entry appId = " <<
    //           appId << ", sessionId = " << sessionId <<LOG_ENDL;
    m_AppId = appId;
    m_SessionId =  sessionId;
    //LOG_TRACE<<"iPodControllerPrivateData::setNowPlayingApplicationBundleName :Exit " <<LOG_ENDL;
}

QString iPodControllerPrivateData::getAppId()
{
   return m_AppId;
}

qint16 iPodControllerPrivateData::getSessionId()
{
   return m_SessionId;
}

void iPodControllerPrivateData::setiPodSoftwareVerion(int major, int minor, int revision, QString sortwareVerion)
{
    m_MajorVersion = major;
    m_MinorVersion = minor;
    m_RevisionVersion = revision;
    m_SoftwareVersion = sortwareVerion;
}

QString iPodControllerPrivateData::getiPodSortwareVersion()
{
    return m_SoftwareVersion;
}

void
iPodControllerPrivateData::SendAudioRouterFadeOut()
{
	//LOG_TRACE << "iPodControllerPrivateData::SendAudioRouterFadeOut() IN" << LOG_ENDL;
    LOG_HIGH << "iPodControllerPrivate::SendAudioRouterFadeOut()" << LOG_ENDL;

	QDBusMessage msg = QDBusMessage::createSignal( AUDIOROUTER_DBUS_PATH,
												   AUDIOROUTER_DBUS_INTERFACE_NAME,
												   "AudioFadeOutSignal" );
	QDBusConnection::sessionBus().send(msg);
	//LOG_TRACE << "iPodControllerPrivateData::SendAudioRouterFadeOut() OUT" << LOG_ENDL;
}

void
iPodControllerPrivateData::SendAudioRouterFadeIn()
{
    //LOG_TRACE << "iPodControllerPrivateData::SendAudioRouterFadeIn() IN" << LOG_ENDL;


    if( m_pSendingFadeInTimer )
    {
        if( m_pSendingFadeInTimer->isActive() == false )
        {
            //LOG_TRACE << "   SendingFadeInTimer(500ms) start!" << LOG_ENDL;
            m_pSendingFadeInTimer->start();
        }
        else
        {
            //LOG_TRACE << "   SendingFadeInTimer(500ms) stop!" << LOG_ENDL;
            m_pSendingFadeInTimer->stop();
            //LOG_TRACE << "   SendingFadeInTimer(500ms) start!" << LOG_ENDL;
            m_pSendingFadeInTimer->start();
        }
    }

//    QDBusMessage msg = QDBusMessage::createSignal( AUDIOROUTER_DBUS_PATH,
//                                                   AUDIOROUTER_DBUS_INTERFACE_NAME,
//                                                   "AudioFadeInSignal" );
//    QDBusConnection::sessionBus().send(msg);
    //LOG_TRACE << "iPodControllerPrivateData::SendAudioRouterFadeIn() OUT" << LOG_ENDL;
}

void
iPodControllerPrivateData::SendAudioRouterFadeInSlot()
{
    //LOG_TRACE << "iPodControllerPrivateData::SendAudioRouterFadeInSlot() IN" << LOG_ENDL;
    LOG_HIGH << "iPodControllerPrivate::SendAudioRouterFadeIn()" << LOG_ENDL;

    QDBusMessage msg = QDBusMessage::createSignal( AUDIOROUTER_DBUS_PATH,
                                                   AUDIOROUTER_DBUS_INTERFACE_NAME,
                                                   "AudioFadeInSignal" );
    QDBusConnection::sessionBus().send(msg);
    //LOG_TRACE << "iPodControllerPrivateData::SendAudioRouterFadeInSlot() OUT" << LOG_ENDL;
}
