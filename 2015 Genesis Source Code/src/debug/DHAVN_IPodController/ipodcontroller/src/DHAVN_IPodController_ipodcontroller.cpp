
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                 Main Controller interface class which will be  *
 *                 used by the HMI, This class is the main        *
 *                 controller class and will create and handle    *
 *                 the functionality of all task handlers         *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_Def.h>
#include <DHAVN_IPodController_deviceDetector.h>
#include <DHAVN_IPodController_audioDeviceDetector.h>
#include <DHAVN_IPodController_Identifier.h>
#include <DHAVN_IPodController_Identifier_IDPS.h>
#include <DHAVN_IPodController_Identifier_NonIDPS.h>
#include <DHAVN_IPodController_usbconfigurationsetter.h>
#include <DHAVN_IPodController_authorisor.h>
#include <DHAVN_IPodController_IAPPacketReader.h>
#include <DHAVN_IPodController_IAPPacketWriter.h>
#include <DHAVN_IPodController_coprocessorDataProvider.h>
#include <DHAVN_IPodController_playEngineTaskHandler.h>
#include <DHAVN_IPodController_iPodDBFetchTaskHandler.h>
#include <DHAVN_IPodController_remoteUiModeTaskHandler.h>
#include <DHAVN_IPodController_trackerConnector.h>
#include <DHAVN_IPodController_artworkFetchTaskHandler.h>
#include <DHAVN_IPodController_iTunesTagTaskHandler.h>
#include <DHAVN_IPodController_iAPThread.h>
#include <DHAVN_IPodController_PrivateData.h>
#include <DHAVN_IPodController_iPodDBSyncThread.h>
#include <DHAVN_IPodController_Performance.h>
#include <DHAVN_IPodController_DBconnection.h>
#include <DHAVN_IPodController_PerformanceLog.h>
#include <DHAVN_IPodController_ucsControl.h>    // added by sungha.choi 2013.03.07 to reset UCS > deleted at 2013.03.07

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <DHAVN_TrackerAbstractor.h>
#include <DHAVN_EventLoop.h>

#ifdef DBUSBASEDAPPHANDLER
#include <DHAVN_IPodController_DBusCore.h>
#include <QDBusError>
#endif

#include <DHAVN_IPodController_iOSAppTaskHandler.h>
#include <QTimer> // added by jungae, 2013.01.26 for ISV70478 side effect

#define PLAYING          1
#define PAUSED           2
#define SEEKING_FF       3
#define SEEKING_REWIND   4
#define STOPPED          5
#define TO_BE_RESET      -34

#define ONESECOND        1000
#define TENSECONDS       10000
#define SCAN_DURATION    10000

CIpodController* CIpodController::m_pInstance = 0;
int CIpodController::m_instanceCount = 0; // added by jungae, 2013.01.26 for ISV70478 side effect
int CIpodController::m_ConnectionDeviceNum = 0;

QString iPodUsbPort1;

CIpodController::CIpodController(QObject *parent, EIPodPorts portNum, int nVehicleType ) :
    QObject(parent),
    m_IsIDPS(true),
    m_isLogoSupported(false) ,
    m_ControllerState(eStateIdle),
    m_IsPrev(false)
{
    //do not try to print log before this
    switch(portNum)
    {
        case eIPOD_PORT_1:
        {
            LOG_INIT_V2(giPodControllerLogName_1);

            QString directory;
            directory.append(TEMP_ARTWORK_LOCATION);
            cleanDirectory(directory);
            m_eCurrentAudioSourceType = EIpodFront;
        }
            break;
        case eIPOD_PORT_2:
            LOG_INIT_V2(giPodControllerLogName_2);
            m_eCurrentAudioSourceType = EIpodRear;
            break;
        default:
            LOG_INIT_V2(giPodControllerLogName);
            break;
    }

    //LOG_TRACE << "CIpodController::CIpodController() IN" << LOG_ENDL;
    if(nVehicleType == 0)
    {
        //-evv:[value]
        //value 값 ( 0: DH, 1: DH15MY, 2:DH PE )
        //DH: "2-2", KH/VI: "1-1"
        iPodUsbPort1.clear();
        iPodUsbPort1.append("2-2");
    }
    else
    {
        iPodUsbPort1.clear();
        iPodUsbPort1.append("1-1");
    }

    m_bStartiPodInitializationFlag = false;
    m_bReaderThreadStartedFlag = false;
    m_bWriterThreadStartedFlag = false;

    m_bDelayNextPrev = false;
    m_delayNextPrevTimer = new QTimer(this);
    if(m_delayNextPrevTimer)
    {
        m_delayNextPrevTimer->setSingleShot(true);
        connect(m_delayNextPrevTimer, SIGNAL(timeout()), this, SLOT(delayNextPrevExecutionSlot()));
    }

    m_resetOSDKeyTimer = new QTimer(this);
    if(m_resetOSDKeyTimer)
    {
        m_resetOSDKeyTimer->setSingleShot(true);
        connect(m_resetOSDKeyTimer, SIGNAL(timeout()), this, SLOT(ResetOSDKeyTimeOutSlot()));
    }

    m_disableTuneOnSetIndexTimer = new QTimer(this);
    if(m_disableTuneOnSetIndexTimer)
    {
        m_disableTuneOnSetIndexTimer->setSingleShot(true);
    }

#ifdef ENABLE_TEST_BUG_PORT_RESET
    m_pPortResetTimer = new QTimer (this);
    if(m_pPortResetTimer)
    {
        connect(m_pPortResetTimer, SIGNAL(timeout()), this, SLOT(portResetTimerTimedOutSlot()));
    }
#endif

    // Objects
    m_Identifier           = NULL;
    m_Authorisor           = NULL;
    m_Detector             = NULL;

    // Reader/Writer
    m_iAPReaderThread      = NULL;
    m_iAPWriterThread      = NULL;
    m_Writer               = NULL;
    m_Reader               = NULL;

    // States
    m_PlayState            = STOPPED;
    m_PrevPlayState        = STOPPED;

    // Task Handler
    m_PlayEngineTaskHandler    = NULL;
    m_ITunesTaskHandler        = NULL;
    m_RemoteUiTaskHandler      = NULL;
    m_iTunesTagTaskHandler     = NULL;
    m_artworkFetchTaskHandler  = NULL;
    m_iOSAppTaskHandler        = NULL; //added for CR 10930

    // DB Thread
    m_TrackerConnector  = NULL;
    m_iPodDBSyncThread  = NULL;

    // Pointers
    m_TrackerAbs        = NULL;   // Added by jonghwan.cho@lge.com   2013.04.29.

    // Variables
    m_vendorId          = 0;
    m_productId         = 0;
    m_DeviceState       = eDisconnected;
    m_DBDeleteSyncFlag  = false;
    m_scanMode          = SCANOFF;
    m_nScanStartPosition = -1;

    m_startNotifierFlag = false; // added by jungae 2013.01.23, for ISV70478

    m_FirstTimeiPodConnectedCount = 0;

    m_pGSTAudioRouterWrapperProcess = NULL;
    m_AudioSource = "plughw:2,0";
    m_SampleRate  = 44100;

    m_IsSendedFirstConnectionSignal = false;
    m_bWaitForFirstConnectionSignal = false;

    // Private allocation and set init values.
    m_PrivateData = new iPodControllerPrivateData(this);

    m_PrivateData->setiPodPortNum(portNum);
    m_PrivateData->m_isAutoPlay_PlaylistReady = false;
    m_PrivateData->m_FirstPlayListSet = false;             // added by pawan 2013.05.01.

    // Restart values
    m_restartUcsCount   = 0;        // added by sungha.choi 2013.03.07
    m_PrivateData->m_restartUcsFlag = false;
    m_PrivateData->m_portresetFlag = false;

    InitializeController();
    m_startNotifierFlag = true;

    CIpodController::m_instanceCount++;

    //LOG_TRACE << "CIpodController::CIpodController() OUT " << LOG_ENDL;
}

void CIpodController::OnCurrentPlayingTrackCountSlot(int count)
{
    //LOG_TRACE << "CIpodController::OnCurrentPlayingTrackCountSlot() IN" << LOG_ENDL;
    //LOG_TRACE << "   in count basic player: " << count << "isplaylist set: " << m_PrivateData->m_FirstPlayListSet << LOG_ENDL;

    if(!m_PrivateData->m_FirstPlayListSet)
    {
        //LOG_TRACE << "   emit OnCurrentPlayingTrackCount(count =" << count <<")"<<LOG_ENDL;
        emit OnCurrentPlayingTrackCount(count);
    }
    //LOG_TRACE << "CIpodController::OnCurrentPlayingTrackCountSlot() OUT" << LOG_ENDL;
}

CIpodController::~CIpodController()
{
    //LOG_TRACE <<"CIpodController::~CIpodController() IN"<< LOG_ENDL;

    if(m_disableTuneOnSetIndexTimer)
    {
        m_disableTuneOnSetIndexTimer->stop();
        delete m_disableTuneOnSetIndexTimer;
        m_disableTuneOnSetIndexTimer = NULL;
    }

    if(m_delayNextPrevTimer)
    {
        m_delayNextPrevTimer->stop();
        delete m_delayNextPrevTimer;
        m_delayNextPrevTimer = NULL;
    }

    if(m_resetOSDKeyTimer)
    {
        m_resetOSDKeyTimer->stop();
        delete m_resetOSDKeyTimer;
        m_resetOSDKeyTimer = NULL;
    }

    if( m_DataProvider )
    {
        delete m_DataProvider;
        m_DataProvider = NULL;
    }

    if( m_Detector )
    {
        delete m_Detector;
        m_Detector = NULL;
    }

    cleariAPChannel();
    stopTaskHandlers();
    destroyTaskHandler();

    if( m_PrivateData )
    {
        delete m_PrivateData;
        m_PrivateData = NULL;   // added by sungha.choi 2013.03.28
    }

#ifdef DBUSBASEDAPPHANDLER
    if(m_DBusInitializer)
    {
        delete m_DBusInitializer;
        m_DBusInitializer = NULL;   // added by sungha.choi 2013.03.28
    }
    //LOG_TRACE << "CIpodController::~CIpodController Deleted m_DBusInitializer" << LOG_ENDL;
#endif
    //LOG_TRACE << "CIpodController::~CIpodController() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::InitializeController                    int (OUT), (void) (IN)
//
// Explanation
// This method does the basic initialization of the controller
// it initializes the raw packet handler, coprocessor, reader, writer and detector
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIpodController::InitializeController()
{
    //LOG_TRACE << "CIpodController::InitializeController() IN" << LOG_ENDL;

    m_DataProvider = new CCoProcessorDataProvider(this);
    m_Detector = new CDeviceDetector(this, m_PrivateData->getiPodPortNum());

    if( m_Detector )
    {
        LOG_HIGH << "CIpodController::InitializeController() m_Detector is connected signal/slot()"<< LOG_ENDL;
        connect(m_Detector, SIGNAL(iPodDeviceDetetcted(int,int)),  this, SLOT(iPodConnectedSlot(int,int)));
        connect(m_Detector, SIGNAL(iPodHidDetected(bool,QString)), this, SLOT(detectionProcessFinished(bool,QString)));
        connect(m_Detector, SIGNAL(iPodDeviceDisconnected()),      this, SLOT(disconnectSignalCaught()) );
        connect(m_Detector, SIGNAL(onIPodDeviceConnectedSignal( QString, QString, EIPodPorts )), this, SLOT(onIPodDeviceConnectedSLOT( QString, QString, EIPodPorts)));
    }

#ifdef DBUSBASEDAPPHANDLER
    m_DBusInitializer = new CIPodControllerDBus_Initializer(this);
#endif

    if( m_PrivateData->getiPodPortNum() == eIPOD_PORT_1 )
    {
        if( ExcuteGSTAudioRouterWrapperProcess() == false )
        {
            LOG_HIGH << "CIpodController::InitializeController(), Cannot run GSTAudioRouterWrapper Process, No sound...!!!" << LOG_ENDL;
        }
        else
        {
            LOG_HIGH << "CIpodController::InitializeController(), run GSTAudioRouterWrapper Process..." << LOG_ENDL;
        }
    }

    //LOG_TRACE <<"CIpodController::InitializeController() OUT"<< LOG_ENDL;
    return 0;
}

bool CIpodController::ExcuteGSTAudioRouterWrapperProcess()
{
    //LOG_TRACE << "CIpodController::ExcuteGSTAudioRouterWrapperProcess() IN " << LOG_ENDL;

    // Wrong process is kiiled.
    int   pid = 0;
    FILE* fp = popen("pgrep -l GSTAudioRouterW","r");
    char readbuf[80];
    fgets(readbuf, 80, fp);
    pid = atoi(readbuf);
    pclose( fp );

    if( pid > 0  && m_pGSTAudioRouterWrapperProcess == NULL )
    {
        QString command;
        command.append("kill -9 ");
        QString strPid;
        strPid.setNum(pid);
        command.append(strPid);
        //LOG_TRACE <<"GST    killing GSTAudioRouterWrapper process, command = " << command <<LOG_ENDL;
        system(command.toStdString().c_str());
    }

    if( m_pGSTAudioRouterWrapperProcess == NULL )
    {
        QStringList list = GetArguments();

        if(list.count() < 2)    // added by sungha.choi 2013.04.23
        {
            return false;
        }

        //LOG_TRACE << "GST::   list items are : " << list[0] << "        " << list[1] << LOG_ENDL;

        if( list[0].contains( "plughw", Qt::CaseInsensitive ) != true ) {
            ////LOG_TRACE << "GST:: we cannot get argument list value, so return" << LOG_ENDL;
            return false;
        }

        int fdlimit = getdtablesize();
        for( int i = STDERR_FILENO + 1 ; i < fdlimit ; i++ )
        {
            if(fcntl(i, F_SETFD, FD_CLOEXEC )<0)
            {
                //LOG_INFO << "GST::   error in fcntl, ExcuteGSTAudioRouterWrapperProcess()"<<LOG_ENDL;
            }
        }

        m_pGSTAudioRouterWrapperProcess = new QProcess();
        m_pGSTAudioRouterWrapperProcess->start("/app/bin/GSTAudioRouterWrapper" , list);
        //LOG_TRACE << "GST::   waiting to start process" << LOG_ENDL;

        if(!m_pGSTAudioRouterWrapperProcess->waitForStarted(3000))
        {
            //LOG_TRACE << "GST::   Process wait for start failed"<<LOG_ENDL;
            if(m_pGSTAudioRouterWrapperProcess->pid() > 0 )//Its already running
            {
                //LOG_TRACE << "GST::   It's running" << LOG_ENDL;
                return true;
            }
            return false;
        }
        else
        {
            //LOG_TRACE << "GST::   Connect to know process status , Only once" << LOG_ENDL;
            connect( m_pGSTAudioRouterWrapperProcess, SIGNAL( finished( int, QProcess::ExitStatus )), this, SLOT( stoppedSlot( int, QProcess::ExitStatus )));
            return true;
        }
    }

    //LOG_TRACE << "CIpodController::ExcuteGSTAudioRouterWrapperProcess() OUT" << LOG_ENDL;
    return true;
}

void CIpodController::stoppedSlot( int exitCode, QProcess::ExitStatus exitStatus )
{
    //LOG_TRACE << "GST::   stoppedSlot() IN, exitCode = " << exitCode << ", ExitStatus = " << exitStatus << LOG_ENDL;

    if( exitStatus == QProcess::NormalExit )
    {
        //LOG_TRACE << "GST::   stoppedSlot(), GSTAudioRouterWrapper is terminated in normail case" << LOG_ENDL;
    }
    else if( exitStatus == QProcess::CrashExit )
    {
        LOG_HIGH << "GST::   stoppedSlot() GSTAudioRouterWrapper Crashed!!!!!!!! retry" << LOG_ENDL;
    }

    //LOG_TRACE << "GST::   stoppedSlot() OUT"<< LOG_ENDL;
}

QStringList CIpodController::GetArguments()
{
    //LOG_TRACE << "GST::CIpodController::GetArguments() IN" << LOG_ENDL;

    QStringList list;
    QString     src;

//    m_AudioSource = m_Detector->getiPodAudioDevPath();

//    if( m_AudioSource.contains( "plughw:1", Qt::CaseInsensitive ) != true &&
//        m_AudioSource.contains( "plughw:2", Qt::CaseInsensitive ) != true &&
//        m_AudioSource.contains( "plughw:3", Qt::CaseInsensitive ) != true )
//    {
//        if( setManuallyAudioDevicePath(1) == false )  // added by sungha.choi 2013.04.23
//        {   // setSrc fail
//            //LOG_TRACE << "GST::   Source port is not here, return NULL" << LOG_ENDL;
//            return list;
//        }
//    }

    src.append("src=");
    src.append(m_AudioSource);

    QString sampleRate;
    sampleRate.append("rate=");
    sampleRate.append(m_SampleRate);

    QString srcType;
    srcType = srcType.sprintf("srcType=%d", m_eCurrentAudioSourceType );

    list.append(src);
    list.append(sampleRate);
    list.append(srcType);

    //LOG_TRACE << "GST::list items are : " << list[0] << "        " << list[1] << LOG_ENDL;
    //LOG_TRACE << "GST::CIpodController::GetArguments() OUT" << LOG_ENDL;
    return list;
}

bool CIpodController::setManuallyAudioDevicePath(int cardNum)
{
    //LOG_TRACE << "GST::CIpodController::setManuallyAudioDevicePath() IN, cardNum = " << cardNum << LOG_ENDL;

    if(cardNum > 3)
        return false;

    char linebuffer[128 + 4] = {0,};
    QString path("plughw:");
    QString device_num(",0");
    QString cardNumStr = QString::number(cardNum);
    char filePath[128] = {0,};
    FILE *pFile = NULL;
    EIPodPorts portNum = m_PrivateData->getiPodPortNum();

    path.append(cardNumStr);
    path.append(device_num);
    snprintf(filePath, 128, "/proc/asound/card%d/usbmixer", cardNum);
    pFile = fopen(filePath, "r");
    //LOG_TRACE <<"   filePath = " << filePath << " portPath = " << path << LOG_ENDL;

    if(pFile == NULL)
    {
        //LOG_TRACE <<"   /proc/asound/cardX/usbmixer is not existed" <<LOG_ENDL;
        return setManuallyAudioDevicePath(cardNum + 1);
    }

    while(fgets(linebuffer, 128, pFile) != NULL)
    {
        if(strncmp(linebuffer, "Card:", 5) == 0)
        {
            //LOG_TRACE <<"   linebuffer = " << linebuffer << LOG_ENDL;
            if(strstr(linebuffer, "3-2") != NULL) // front
            {
                //LOG_TRACE << "   front port" << LOG_ENDL;
                if(portNum == eIPOD_PORT_1)
                {
                    m_AudioSource = path;
                    fclose(pFile);
                    return true;
                }
                else
                {
                    fclose(pFile);
                    return setManuallyAudioDevicePath(cardNum + 1);
                }
            }
            else if(strstr(linebuffer, "3-1") != NULL) // rear
            {
                //LOG_TRACE << "   rear port" << LOG_ENDL;
                if(portNum == eIPOD_PORT_2)
                {
                    m_AudioSource = path;
                    fclose(pFile);
                    return true;
                }
                else
                {
                    fclose(pFile);
                    return setManuallyAudioDevicePath(cardNum + 1);
                }
            }
            break;
        }
    }
    fclose(pFile);
    //LOG_TRACE << "GST::CIpodController::setManuallyAudioDevicePath() OUT" << LOG_ENDL;
    return false;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::start                    void (OUT), (void) (IN)
//
// Explanation
// This method starts the controller and set the ipod for any further notification
// for the device
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::start(void)
{
    //LOGH LOG_ENDL;

    if(CIpodController::m_instanceCount <= 0) return;

    if( m_startNotifierFlag )
    {
        m_Detector->registerForTheNotification();
        LOG_HIGH << "CIpodController::start() m_startNotifierFlag is true, m_Detector->registerForTheNotification()" << LOG_ENDL;
        m_startNotifierFlag = false;
    }
    else
        LOG_HIGH << "CIpodController::start() m_startNotifierFlag is false, so we don't use detector" << LOG_ENDL;

    //LOG_TRACE << "CIpodController::start OUT"<<LOG_ENDL;
}

void CIpodController::disconnectSignalCaught()
{
    LOG_HIGH << "CIpodController::disconnectSignalCaught()" << LOG_ENDL;
    //Added by Tan for Aha.
    m_scanMode = SCAN_OFF;
    m_nScanStartPosition = -1;
    m_PrivateData->removeSampleRateFile();

    m_delayNextPrevTimer->stop();
    m_resetOSDKeyTimer->stop();

    m_bDelayNextPrev = false;

    if(m_PrivateData->m_restartUcsFlag == false)
    {
        m_restartUcsCount = 0;
    }

    m_ControllerState = eStateIdle;
#ifdef CONNECT_BOOTUP
    m_DeviceState     = eDisconnected;
#endif
    m_PlayState     = STOPPED;
    m_PrevPlayState = STOPPED;
    m_IsIDPS        = true;
    m_FirstTimeiPodConnectedCount = 0;
    m_IsSendedFirstConnectionSignal = false;
    m_bWaitForFirstConnectionSignal = false;
    //LOG_TRACE << "   emit disconnected() " << LOG_ENDL;
    emit disconnected();

    m_PrivateData->clearCurrentiPodData();
    m_PrivateData->m_CurrentInProgressEvent = EIdle;
    m_PrivateData->m_QueueData.clearQueueData();
    m_PrivateData->m_EventCommandQueue.clear();

    if( m_TrackerAbs )
    {
        disconnect(m_TrackerAbs, SIGNAL(getCoverArt(QStringList, QStringList, QString)), this, SLOT(fetchCoverArtFromIPodSlot(QStringList, QStringList, QString)));
    }

    m_Detector->piPodLog->WriteLog("Here 1 disconnectSignalCaught");
    stopTaskHandlers();
    m_Detector->piPodLog->WriteLog("Here 2 stopTaskHandlers after");
    cleariAPChannel();    // added by sungha.choi 2013.04.02, this function should be called before DestroyTaskHandler
    m_Detector->piPodLog->WriteLog("Here 3 cleariAPChannel after");

    emit OnStateChanged(PLAYER_STOP);
    emit OnModeChanged( 0 );

    m_Detector->requestDestroyTask();
    m_Detector->piPodLog->WriteLog("Here 4 requestDestroyTask after");

    CIpodController::m_ConnectionDeviceNum -= 1;
}

void CIpodController::cleanDirectory(QString &directoryPath)
{
    QDir albumArtDir(directoryPath);
    //Check if any Album Art file is present and delete them
    if(albumArtDir.exists())
    {
        for(uint i= 0; i < albumArtDir.count();i++)
        {
            albumArtDir.remove(albumArtDir.entryList().at(i));
        }
    }
}

void CIpodController::OnCurrentMediaInfoSent(MediaInfo* mediaInfo)
{
    if(m_PlayEngineTaskHandler == NULL)
        return;

    MediaInfo myMediaInfo;
    myMediaInfo.Title = mediaInfo->Title;
    myMediaInfo.Album = mediaInfo->Album ;
    myMediaInfo.Artist = mediaInfo->Artist;
    myMediaInfo.Genre = mediaInfo->Genre;
    myMediaInfo.Composer = mediaInfo->Composer;
    myMediaInfo.fileName = mediaInfo->fileName;
    myMediaInfo.coverarturl = mediaInfo->coverarturl;

    SetOSDCondition();
    if(mediaInfo->Title == " ")
    {
        TAN_PRINTF("break here, find out why\n");
    }

    //LOG_TRACE << "myMediaInfo.coverarturl: "<< myMediaInfo.coverarturl <<LOG_ENDL;
    //LOG_TRACE << "TanDebug OnCurrentMediaInfoSent Artist: " << myMediaInfo.Artist <<LOG_ENDL;
    //LOG_TRACE << "TanDebug OnCurrentMediaInfoSent Title: " << myMediaInfo.Title <<LOG_ENDL;
    //LOG_TRACE << "TanDebug OnCurrentMediaInfoSent Album: " << myMediaInfo.Album <<LOG_ENDL;

    if(m_PrivateData->m_OSDKey == OSDKey_Next || m_PrivateData->m_OSDKey == OSDKey_Prev) //next or prev was pressed.
    {
        emit OnAutoTrackChanged(-1);
    }
    else
    {
        emit OnAutoTrackChanged(m_PlayEngineTaskHandler->m_CurrentIndex);
    }
    emit OnCurrentMediaInfo(&myMediaInfo);
}

void CIpodController::iPodConnectedSlot(int vendorId, int productId)
{
    //LOGH " m_ControllerState = " << GetControllerStateString() << LOG_ENDL;
    m_PrivateData->m_TempPerformanceTimer.start();

    if(eStateIdle == m_ControllerState)
    {


        m_vendorId   = vendorId;
        m_productId  = productId;

        m_PrivateData->m_iPodProductID = productId;// Saving the Product ID in variable, and this will accessed in iPodDBFetchTaskHandler
        //LOG_TRACE<< "   m_FirstTimeiPodConnectedCount in  iPodConnectedSlot " << m_FirstTimeiPodConnectedCount << LOG_ENDL;

        //LOG_HIGH << "restartUcs, m_PrivateData->m_portresetFlag = false!!!!" << LOG_ENDL;
        m_PrivateData->m_portresetFlag = false;

        if( ++m_FirstTimeiPodConnectedCount == 1 )
        {
            //LOG_TRACE<< "m_FirstTimeiPodConnectedCount " << m_FirstTimeiPodConnectedCount << LOG_ENDL;
            PERFORMANCELOG ( "----------------------New Device Connected-------------------------------\n"  );
            PERFORMANCELOG ("Vendor Id is %d ", m_vendorId );
            PERFORMANCELOG ("Product id is % d ", m_productId );
            if(m_PrivateData->m_CurrentTime.isNull())
            {
                //LOG_TRACE<< "Current Time start" << LOG_ENDL;
                m_PrivateData->m_CurrentTime.start();
            }
            else
            {
                //LOG_TRACE<< "Current Time Re-start" << LOG_ENDL;
                m_PrivateData->m_CurrentTime.restart();
            }

            if(m_PrivateData->m_AutoPlayTime.isNull())
            {
                //LOG_TRACE<< "Autoplay Time start" << LOG_ENDL;
                m_PrivateData->m_AutoPlayTime.start();
            }
            else
            {
                //LOG_TRACE<< "Autoplay Time Re-start" << LOG_ENDL;
                m_PrivateData->m_AutoPlayTime.restart();
            }

        }
        CIpodController::m_ConnectionDeviceNum += 1;
    }

    //LOG_TRACE<< "CIpodController::iPodConnectedSlot() OUT " << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::detectionProcessFinished                    void (OUT), (void) (IN)
//
// Explanation
// This is a slot and will be called when the detection process is finished
// it creates the task handlersand starts the identification process
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::detectionProcessFinished(bool status, QString hidDevPath)
{
    //LOG_TRACE << "CIpodController::detectionProcessFinished() IN"<<LOG_ENDL;

    if( !status )
    {
        /* Emit detection error to Application*///TODO
        LOG_HIGH << "CIpodController::detectionProcessFinished() failed : status = " << status <<LOG_ENDL;
        //emit  IPodPopUp(IPOD_POPUP_UNSUPPORTED);
        m_Detector->piPodLog->WriteLog("Here 0 detectionProcessFinished OUT");  // added by sungha.choi 2013.04.12
        return;
    }

    if(eStateIdle == m_ControllerState)
    {
        //LOG_HIGH << "CIPodController::detectionProcessFinished(), emit connected()"  << LOG_ENDL;
        emit connected();
        m_ControllerState = eStateiPodDetetcted;

        /* Start the iAP Channel */
        m_Detector->piPodLog->WriteLog("Here 0 detectionProcessFinished startiAPChannel before");  // added by sungha.choi 2013.04.12
        if(IPOD_SUCCESS != startiAPChannel(hidDevPath))
        {
            LOG_HIGH << "CIpodController::detectionProcessFinished() Failed to Start the iAP Channel : startiAPChannel(hidDevPath) = " << startiAPChannel(hidDevPath) <<LOG_ENDL;
            //emit  IPodPopUp(IPOD_POPUP_UNSUPPORTED);
        }
        else
        {
            LOG_HIGH << "detectionProcessFinished(), CurrentState = "<< GetControllerStateString() << ", hidDevPath = "<< hidDevPath << LOG_ENDL;
        }
    }
    else
    {
        LOG_HIGH << "CIpodController::detectionProcessFinished() Wrong event in Wrong state, CurrentState =" << GetControllerStateString() << LOG_ENDL;
    }

    PT_END_TIME(PT_OBJ_DEVICE_DETECT, 0);
    //LOG_TRACE << "CIpodController::detectionProcessFinished() OUT"<<LOG_ENDL;
    m_Detector->piPodLog->WriteLog("Here 0 detectionProcessFinished OUT");  // added by sungha.choi 2013.04.12
}

void CIpodController::iAPReaderReady(bool status)
{
    //LOG_TRACE << "CIpodController::iAPReaderReady() IN"<<LOG_ENDL;
    //LOG_TRACE << "   m_ControllerState = " << GetControllerStateString() << ", status = " << status << LOG_ENDL;
    if( !status )
    {
        /* Emit detection error to Application*///TODO
        return;
    }

    switch(m_ControllerState)
    {
        case eStateiPodDetetcted:
            m_ControllerState = eStateiAPReaderReady;
            //LOG_TRACE << "CIpodController::iAPReaderReady() StateChange: eStateiPodDetetcted -> eStateiAPReaderReady"<<LOG_ENDL;
            break;
        case eStateiAPWriterReady:
            m_ControllerState = eStateiAPReady;
            LOG_HIGH << "iAPReaderReady(), eStateiAPWriterReady -> eStateiAPReady" << LOG_ENDL;
            //startiPodInitialization();
            break;
        case eStateIdle:
        case eStateiAPReaderReady:
        case eStateiAPReady:
        case eStateIdentified:
        case eStateAuthorized:
        case eStateiPodInitialized:
        case eStateEnteredInUIMode:
        case eStateiPodReady:
        case eStateAutoPlayReady:
        default:
            break;
    }
    //LOG_TRACE << "CIpodController::iAPReaderReady() OUT"<<LOG_ENDL;
}

void CIpodController::iAPWriterReady(bool status)
{
    //LOG_TRACE << "CIpodController::iAPWriterReady() IN"<<LOG_ENDL;
    //LOG_TRACE << "   m_ControllerState = " << GetControllerStateString() << ", status = " << status << LOG_ENDL;
    if( !status )
    {
        /* Emit detection error to Application*///TODO
        return;
    }

    switch( m_ControllerState )
    {
        case eStateiPodDetetcted:
            m_ControllerState = eStateiAPWriterReady;
            LOG_HIGH << "iAPWriterReady(), eStateiPodDetetcted -> eStateiAPWriterReady"<<LOG_ENDL;
            break;

        case eStateiAPReaderReady:
            m_ControllerState = eStateiAPReady;
            LOG_HIGH << "iAPWriterReady(), eStateiAPReaderReady -> eStateiAPReady" << LOG_ENDL;
            //startiPodInitialization();
            break;
        case eStateIdle:
        case eStateiAPWriterReady:
        case eStateiAPReady:
        case eStateIdentified:
        case eStateAuthorized:
        case eStateiPodInitialized:
        case eStateEnteredInUIMode:
        case eStateiPodReady:
        case eStateAutoPlayReady:
        default:
            break;
    }
    //LOG_TRACE << "CIpodController::iAPWriterReady() OUT"<<LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::IdentificationProcessFinished                    void (OUT), (bool) (IN)
//
// Explanation
// This is a slot and will be called when the identification process is finished
// it emits a signal for the same
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::IdentificationProcessFinished(bool finished)
{
    //LOG_TRACE << "CIpodController::IdentificationProcessFinished() IN"<<LOG_ENDL;
    int milliseconds = m_PrivateData->m_CurrentTime.elapsed();
    QString performanceTime = m_PrivateData->getPerformanceTime( milliseconds );
    PERFORMANCELOG ( "IPod Identification time is %s " , performanceTime.toAscii().data()  );
    m_PrivateData->m_CurrentTime.restart();
    //LOG_TRACE << "   m_ControllerState = " << GetControllerStateString() << ", finished = " << finished << LOG_ENDL;

    if( !finished )
    {
        LOG_HIGH << "CIpodController::IdentificationProcessFinished(), Identification Failed!! : finished = "<< finished << LOG_ENDL;
        //LOG_HIGH << "   emit Identified(finished = " << finished << ")"  << LOG_ENDL;
        emit IPodPopUp(IPOD_POPUP_CONNECTION_ERROR);
        emit Identified(finished);
        return;
    }

    switch(m_ControllerState)
    {
        case eStateiAPReady:
            m_ControllerState = eStateIdentified;
            LOG_HIGH << "IdentificationProcessFinished(), eStateiAPReady -> eStateIdentified" << LOG_ENDL;
            emit Identified(finished);
            break;
        case eStateAuthorized:
            m_restartUcsCount = 0;    // added by sungha.choi 2013.03.07
            m_ControllerState = eStateiPodInitialized;
            LOG_HIGH << "IdentificationProcessFinished(), eStateAuthorized -> eStateiPodInitialized" << LOG_ENDL;
            emit Identified(finished);
            break;
        case eStateIdle:
        case eStateiPodDetetcted:
        case eStateiAPReaderReady:
        case eStateiAPWriterReady:
        case eStateiPodInitialized:
        case eStateIdentified:
        case eStateEnteredInUIMode:
        case eStateiPodReady:
        case eStateAutoPlayReady:
        default:
            break;
    }
    //LOG_TRACE << "CIpodController::IdentificationProcessFinished OUT"<<LOG_ENDL;
    PT_END_TIME(PT_OBJ_IDENTIFICATION, 0);
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::authorisationProcessFinished                    void (OUT), (bool) (IN)
//
// Explanation
// This is a slot and will be called when the authentication process is finished
// it emits a signal for the same
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::authorisationProcessFinished(bool finished)
{
    //LOG_TRACE << "CIpodController::authorisationProcessFinished IN"<<LOG_ENDL;

    int milliseconds = m_PrivateData->m_CurrentTime.elapsed();
    QString performanceTime = m_PrivateData->getPerformanceTime( milliseconds );
    PERFORMANCELOG ( "IPod Authorisation time is %s " , performanceTime.toAscii().data()  );
    m_PrivateData->m_CurrentTime.restart();

    if(!finished)
    {
        LOG_HIGH << "   Authorization Failed!! : finished = "<< finished << ", emit authorised(false)"<<LOG_ENDL;
        emit IPodPopUp(IPOD_POPUP_CONNECTION_ERROR);
        emit authorised(finished);
        return;
    }

    //LOG_TRACE << "   m_ControllerState = " << GetControllerStateString() << LOG_ENDL;

    switch(m_ControllerState)
    {
        case eStateiAPReady:
            m_ControllerState = eStateAuthorized;
            LOG_HIGH << "authorisationProcessFinished(), eStateiAPReady -> eStateAuthorized" << LOG_ENDL;
            emit authorised(finished);
            break;
        case eStateIdentified :
            m_restartUcsCount = 0;    // added by sungha.choi 2013.03.07
            m_ControllerState = eStateiPodInitialized;
            LOG_HIGH << "authorisationProcessFinished(), eStateIdentified -> eStateiPodInitialized" << LOG_ENDL;
            emit authorised(finished);
            break;

        case eStateIdle:
        case eStateiPodDetetcted:
        case eStateiAPReaderReady:
        case eStateiAPWriterReady:
        case eStateAuthorized:
        case eStateiPodInitialized:
        case eStateEnteredInUIMode:
        case eStateiPodReady:
        case eStateAutoPlayReady:
        default:
            break;
    }

    //LOG_TRACE << "CIpodController::authorisationProcessFinished OUT"<<LOG_ENDL;

    PT_END_TIME(PT_OBJ_AUTHENTICATION, 0);
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::extendedModeSwitched                    void (OUT), (bool) (IN)
//
// Explanation
// This is a slot and will be called when all the initialization of connected process is finished
// it emits a signal for the same
// History
// 07-Nov-2011     Venkataramana K                        Added initial implementation
*****************************************************************************/
void CIpodController::extendedModeSwitched(bool status)
{
    //LOGH " m_ControllerState = " << GetControllerStateString() << LOG_ENDL;

    int milliseconds = m_PrivateData->m_CurrentTime.elapsed();
    QString performanceTime = m_PrivateData->getPerformanceTime( milliseconds );
    PERFORMANCELOG ( "IPod ExtendedMode time is %s " , performanceTime.toAscii().data()  );
    //LOG_TRACE << "   m_ControllerState = " << GetControllerStateString() << ", status = " << status << LOG_ENDL;

    if(!status)
    {
        LOG_HIGH << "   Extended mode switchFailed : status = "<< status << ", emit iPodReady(false)_"  <<  LOG_ENDL;
        emit  IPodPopUp(IPOD_POPUP_CONNECTION_ERROR);
        emit  iPodReady(false);
        return;
    }

    switch(m_ControllerState)
    {
        case eStateiPodInitialized:{
            m_ControllerState = eStateEnteredInUIMode;
            LOG_HIGH << "extendedModeSwitched(), eStateiPodInitialized -> eStateEnteredInUIMode"<<LOG_ENDL;
            int ret = -1;
            ret = startPostExtModeInitEC();
            if(IPOD_SUCCESS != ret)
            {
                LOG_HIGH << "   startPortExtModeInitEC() FAILED : ret = "<< ret << ", emit iPodReady(false)_"  << LOG_ENDL;
                //emit  IPodPopUp(IPOD_POPUP_CONNECTION_ERROR);
                emit  iPodReady(false);
                break;
            }
        }
            break;
        case eStateIdle:
        case eStateiPodDetetcted:
        case eStateiAPReaderReady:
        case eStateiAPWriterReady:
        case eStateiAPReady:
        case eStateIdentified:
        case eStateAuthorized:
        case eStateEnteredInUIMode:
        case eStateiPodReady:
        case eStateAutoPlayReady:
        default:
            break;
    }
    //LOG_TRACE << "CIpodController::extendedModeSwitched() OUT"<<LOG_ENDL;
}

bool CIpodController::isAcceptingTuneDial()
{
    if(m_PlayEngineTaskHandler)
    {
        return (m_PlayEngineTaskHandler->isAcceptingTuneDial() &&
                m_PlayEngineTaskHandler->m_bCoverArtUpdated &&
                m_disableTuneOnSetIndexTimer->isActive() == false);
    }
    return false;
}

void CIpodController::postExtendedModeInitDoneEC(bool status)
{
    //LOGH " m_ControllerState = " << GetControllerStateString() << ", status = " << status << LOG_ENDL;
    try
    {
        if(!status)
        {
            LOG_HIGH << "   postExtendedModeInit Failed, status = " << status << ", emit iPodReady(false)" << LOG_ENDL;
            emit  IPodPopUp(IPOD_POPUP_CONNECTION_ERROR);
            emit  iPodReady(false);
            return;
        }

        switch(m_ControllerState)
        {
            case eStateEnteredInUIMode:
            case eStateAutoPlayReady:
            case eStateReadyToAutoPlayReady:
                if(m_ControllerState != eStateReadyToAutoPlayReady)
                {
                    m_ControllerState = eStateiPodReady;
                }
                m_PrivateData->m_IsDBIndexingFinished = true;
                //LOG_TRACE << "CIpodController::postExtendedModeInitDone: StateChange: eStateEnteredInUIMode -> eStateiPodReady"<<LOG_ENDL;

                if(m_PrivateData->m_CoverArtData.m_RequestedCoverAlbumName.length() > 0)
                {
                    fetchCoversForBasicPlayer(m_PrivateData->m_CoverArtData.m_RequestedCoverAlbumName, m_PrivateData->m_CoverArtData.m_RequestedCoverArtistName, false);
                }

                if(m_PlayEngineTaskHandler)
                {
                    m_PlayEngineTaskHandler->setm_IsSyncInProgress(false, 0);
                    //LOG_HIGH << "CIpodController::postExtendedModeInitDone() emit iPodReady(true) for Index based devices" << LOG_ENDL;
                    emit iPodReady(true);
                }

                // when the media indexing was completed on eStateiPodReady
                // if state is 'true', send the iPodReady signal to AppMediaPlayer
                if( status == true )
                {
                    LOG_HIGH<<"CIpodController::postExtendedModeInitDone() status = true, emit iPodReady(true)"<<LOG_ENDL;
                    emit iPodReady(true);
                }
                break;
            case eStateIdle:
            case eStateiPodDetetcted:
            case eStateiAPReaderReady:
            case eStateiAPWriterReady:
            case eStateiAPReady:
            case eStateIdentified:
            case eStateAuthorized:
            case eStateiPodInitialized:
            case eStateiPodReady:
            default:
                break;
        }
    }
    catch(EIPodErrors ex){
        //LOG_TRACE << "CIpodController::postExtendedModeInitDone: DisConnect Exeption caught!!!"<<LOG_ENDL;
        return;
    }

    //LOG_TRACE << "CIpodController::postExtendedModeInitDone() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::setTrackerAbsObject                    void (OUT), (CTrackerAbstractor*) (IN)
//
// Explanation
// this function is called to set the tracker abtwork to Tracker ABstractor object into
// IPodController.
// This object must be set before raising a request for arstractor in case of IPodController.
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::setTrackerAbsObject(CTrackerAbstractor* abs)
{
    //LOG_TRACE << "CIpodController::setTrackerAbsObject() IN" << LOG_ENDL;
    if(!abs) return;
    m_TrackerAbs = abs;
    connect(abs, SIGNAL(getCoverArt(QStringList, QStringList, QString)), this, SLOT(fetchCoverArtFromIPodSlot(QStringList, QStringList, QString)));
    //LOG_TRACE << "CIpodController::setTrackerAbsObject() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::fetchCoverArtFromIPodSlot               QStringList, QStringList(IN)
//
// Explanation
// This is a slot which will be called on the signal of Tracker abstractor
// This will start the processing of the artwork fetch
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::fetchCoverArtFromIPodSlot(QStringList albumlist, QStringList artistlist, QString iPodSerialNumber)
{
    try
    {
        //LOGH " m_ControllerState = " << GetControllerStateString() << ", albumlist.count() = " << albumlist.count() << LOG_ENDL;

        if( m_ITunesTaskHandler && (!m_ITunesTaskHandler->getCurrentSerialNumberE().compare(iPodSerialNumber)))
        {
            if(((m_ControllerState != eStateAutoPlayReady) &&
                (m_ControllerState != eStateiPodReady)) ||
                    (m_ControllerState == eStateIdle))
            {
                m_PrivateData->m_CoverArtData.m_AlbumList.clear();
                m_PrivateData->m_CoverArtData.m_ArtistList.clear();
                QStringList emptylist;
                connect(this, SIGNAL(coverArtFetched(QStringList)), m_TrackerAbs, SLOT(onCoverArtsFromIPOD(QStringList)));
                emit coverArtFetched(emptylist);
                disconnect(this, SIGNAL(coverArtFetched(QStringList)), m_TrackerAbs, SLOT(onCoverArtsFromIPOD(QStringList)));
                return;
            }
        }
        else
        {
            m_PrivateData->m_CoverArtData.m_AlbumList.clear();
            m_PrivateData->m_CoverArtData.m_ArtistList.clear();
            QStringList emptylist;
            connect(this, SIGNAL(coverArtFetched(QStringList)), m_TrackerAbs, SLOT(onCoverArtsFromIPOD(QStringList)));
            emit coverArtFetched(emptylist);
            disconnect(this, SIGNAL(coverArtFetched(QStringList)), m_TrackerAbs, SLOT(onCoverArtsFromIPOD(QStringList)));
            return;
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE <<"CIpodController::fetchCoverArtFromIPodSlot Execption Caught ******" << LOG_ENDL;
        m_PrivateData->m_CoverArtData.m_AlbumList.clear();
        m_PrivateData->m_CoverArtData.m_ArtistList.clear();
        QStringList emptylist;
        if(m_TrackerAbs != NULL)
        {
            connect(this, SIGNAL(coverArtFetched(QStringList)), m_TrackerAbs, SLOT(onCoverArtsFromIPOD(QStringList)));
            emit coverArtFetched(emptylist);
            disconnect(this, SIGNAL(coverArtFetched(QStringList)), m_TrackerAbs, SLOT(onCoverArtsFromIPOD(QStringList)));
        }
        return;
    }

    for (int i=0;i<albumlist.count(); i++)
    {
        //LOG_TRACE << "   album name :  " << albumlist[i] <<  "   artist name : " << artistlist[i] <<LOG_ENDL;
    }

    if(m_PrivateData->m_CoverArtData.m_IsBasicPlayerCoverArtRequestInProgress == true)
    {
        //LOG_TRACE << "   basicp cover art fetching already in progress" <<LOG_ENDL;
        m_PrivateData->m_CoverArtData.m_AlbumList = albumlist;
        m_PrivateData->m_CoverArtData.m_ArtistList = artistlist;
    }
    else
    {
        m_PrivateData->m_CoverArtData.m_IsOnDemandCoverArtRequestInProgress = true;
        m_iPodDBSyncThread->fetchArtwork(albumlist, artistlist, false);
    }

    //LOG_TRACE << " CIpodController::fetchCoverArtFromIPodSlot() OUT" << LOG_ENDL;
}

int CIpodController::convertPlayerEnginePlayState(int nPlayerEnginePlayState)
{
    int nPlayState = -1;
    switch(nPlayerEnginePlayState)
    {
    case ePlayState_STOP:
        nPlayState = STOPPED;
        break;
    case ePlayState_PLAY:
        nPlayState = PLAYING;
        break;
    case ePlayState_PAUSE:
        nPlayState = PAUSED;
        break;
    case ePlayState_FF:
        nPlayState = SEEKING_FF;
        break;
    case ePlayState_REW:
        nPlayState = SEEKING_REWIND;
        break;
    default:
        break;
    }
    return nPlayState;
}

void CIpodController::startAutoPlay(bool isSyncInProgress, int syncType)
{
    //LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState = " << GetPlayStateString(m_PlayState) << ", isSyncInProgress =" << isSyncInProgress << ", syncType = " << syncType <<LOG_ENDL;

    if(m_ControllerState == eStateIdle)
    {
        //LOG_TRACE << "CIpodController:: AUTOPLAY : startAutoPlay() : after disconnect, start autoplay command shuld be ignored" << LOG_ENDL;
        return;
    }

    if(m_Detector == NULL || m_PlayEngineTaskHandler == NULL)
    {
        return;
    }

    try //Crash Fix
    {
        //LOG_TRACE << "   DeviceResetValue = " << m_Detector->getDeviceResetValue() << LOG_ENDL;
        if( m_Detector->getDeviceResetValue() )
        {
            LOG_HIGH << "CIpodController::startAutoPlay(), emit SystemStateStatus( SYSTEM_IPODDEVICE_RESET_OFF )" << LOG_ENDL;
            emit SystemStateStatus(SYSTEM_IPODDEVICE_RESET_OFF);
            m_Detector->setDeviceResetValue(false);
        }

        if(/*!m_iPodAVModeReady ||*/ m_PrivateData->m_IsTabOff)
        {
            //LOGH ", waiting for the start at: " << m_PrivateData->m_TempPerformanceTimer.elapsed() << LOG_ENDL;
            LOG_HIGH << "m_PrivateData->m_IsTabOff = " << m_PrivateData->m_IsTabOff << LOG_ENDL;
            m_ControllerState = eStateReadyToAutoPlayReady;
        }
        else
        {
            //LOGH ", play requested at: " << m_PrivateData->m_TempPerformanceTimer.elapsed() << LOG_ENDL;
            m_ControllerState = eStateAutoPlayReady;
            m_PlayEngineTaskHandler->setm_IsSyncInProgress(isSyncInProgress, syncType);
            //Tan, this is silly code, I know!
            //{Previously it was m_PlayState = m_PlayEngineTaskHandler->startAutoplay(m_iOSAppTaskHandler);
            //I am fixing this bug with minimum change.
            int nPlayerEnginePlayState = m_PlayEngineTaskHandler->startAutoplay(m_iOSAppTaskHandler);
            m_PlayState = convertPlayerEnginePlayState(nPlayerEnginePlayState);
            //}
            LOG_HIGH << "start autoplay" << LOG_ENDL;
            //LOG_TRACE << "AUTOPLAY: play finished at: " << m_PrivateData->m_TempPerformanceTimer.elapsed() << LOG_ENDL;
            Play();
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE <<"AUTOPLAY :CIpodController::startAutoPlay - Exception Caught" << LOG_ENDL;
        return;
    }
    emit autoPlayReady(true);

    //LOG_TRACE << "IOSAPP::AUTOPLAY:CIpodController::startAutoPlay() OUT"<<LOG_ENDL;
}

void CIpodController::onDBSyncThreadFinish()
{
    LOGH LOG_ENDL;

    if( eStateIdle == m_ControllerState )
    {
        if( m_DBDeleteSyncFlag == true )
        {
            //LOG_TRACE << "CIpodController::onDBSyncThreadFinish() Already delete!!!"<<LOG_ENDL;
            return;
        }

        m_DBDeleteSyncFlag = true;
        /* Clean up only if the ipod disconnected, else thread will be cleaned up at ipod disconnet */
        if(m_TrackerConnector)
        {
            m_TrackerConnector->cleanUpOnDisconnect();
        }

        if( m_iPodDBSyncThread ) delete m_iPodDBSyncThread;
        if( m_TrackerConnector ) delete m_TrackerConnector;

        m_iPodDBSyncThread = NULL;
        m_TrackerConnector = NULL;
    }

    //LOG_TRACE << "CIpodController::onDBSyncThreadFinish() OUT"<<LOG_ENDL;
}

/*********************************** private helper methods ************************************/

int CIpodController::startiAPChannel(QString hidDevPath)
{
    LOG_HIGH << "startiAPChannel(), hidDevPath = " << hidDevPath << LOG_ENDL;
    m_PrivateData->m_hidDevPath = hidDevPath;

    m_Writer = new CIAPPacketWriter(m_PrivateData->getiPodPortNum(), m_vendorId, m_productId, hidDevPath);
    if(NULL == m_Writer)
    {
        //LOG_TRACE << "CIpodController::startiAPChannel() Malloc for m_Writer Failed!!"<<LOG_ENDL;
        goto startiAPChannel_fail;
    }
    connect(m_Writer, SIGNAL(iAPWriterStatus(bool)),
            this, SLOT(iAPWriterReady(bool)));

    m_Reader = new CIAPPacketReader(m_PrivateData->getiPodPortNum(), m_vendorId, m_productId, hidDevPath);
    if(NULL == m_Reader)
    {
        LOG_HIGH << "   Malloc for m_Reader Failed!!"<<LOG_ENDL;
        delete m_Writer;
        m_Writer = NULL;    // added by sungha.choi 2013.03.28
        goto startiAPChannel_fail;
    }
    connect(m_Reader, SIGNAL(iAPReaderStatus(bool)),
            this, SLOT(iAPReaderReady(bool)));

    connect(this, SIGNAL(closeFileDescriptorSignal()), m_Reader, SLOT(closeFileDescriptorSlot()));
    connect(this, SIGNAL(closeFileDescriptorSignal()), m_Writer, SLOT(closeFileDescriptorSlot()));

    m_iAPReaderThread = new iAPThread();
    if(NULL == m_iAPReaderThread)
    {
        LOG_HIGH << "   Malloc for m_iAPReaderThread Failed!!"<<LOG_ENDL;
        if( m_Writer ) delete m_Writer;
        if( m_Reader ) delete m_Reader;
        m_Writer = NULL;    // added by sungha.choi 2013.03.28
        m_Reader = NULL;    // added by sungha.choi 2013.03.28
        goto startiAPChannel_fail;
    }
    m_Reader->moveToThread(m_iAPReaderThread);

    m_iAPWriterThread = new iAPThread();
    if(NULL == m_iAPWriterThread)
    {
        LOG_HIGH << "   Malloc for m_iAPWriterThread Failed!!"<<LOG_ENDL;
        if( m_Writer ) delete m_Writer;
        if( m_Reader ) delete m_Reader;
        m_Writer = NULL;    // added by sungha.choi 2013.03.28
        m_Reader = NULL;    // added by sungha.choi 2013.03.28
        goto startiAPChannel_fail;
    }
    m_Writer->moveToThread(m_iAPWriterThread);

    connect( m_iAPWriterThread, SIGNAL(finished()), m_Writer, SLOT(deleteLater()));
    connect( m_iAPReaderThread, SIGNAL(finished()), m_Reader, SLOT(deleteLater()));
    connect( m_iAPWriterThread, SIGNAL(finished()), m_iAPWriterThread, SLOT(deleteLater()));    // added by sungha.choi 2013.04.02
    connect( m_iAPReaderThread, SIGNAL(finished()), m_iAPReaderThread, SLOT(deleteLater()));    // added by sungha.choi 2013.04.02

    connect( m_iAPReaderThread, SIGNAL(started()), this, SLOT(readerThreadStarted()));
    connect( m_iAPWriterThread, SIGNAL(started()), this, SLOT(writerThreadStarted()));

    m_Reader->setDevice(m_vendorId, m_productId, hidDevPath);
    m_Writer->setDevice(m_vendorId, m_productId, hidDevPath);

    //USB Device Vendor Request to set available current for iPod charging, after '/dev/hidrawX' file open
    m_Writer->HIDVendorSpecificRequest(VENDOR_DEFINED_POWER_REQUEST, EXTRA_CURRENT_500mA_IN_SUSPEND_STATE, EXTRA_CURRENT_1600mA_IN_NO_SUSPEND_STATE);

    m_bStartiPodInitializationFlag = false;
    m_bReaderThreadStartedFlag = false;
    m_bWriterThreadStartedFlag = false;
    m_iAPReaderThread->start();
    m_iAPWriterThread->start();

    //LOG_TRACE << "CIpodController::startiAPChannel() OUT" << LOG_ENDL;
    return IPOD_SUCCESS;

startiAPChannel_fail:
    return IPOD_ERROR_GENERIC;
}


void CIpodController::cleariAPChannel()
{
    //LOG_TRACE << "CIpodController::cleariAPChannel() IN" << LOG_ENDL;

    if(m_iAPReaderThread && m_Reader) // { modified by sungha.choi 2013.04.02, thread gracefully finish
    {
        m_Reader->onDeviceDisconnect();
        m_iAPReaderThread->quit();
    }

    if(m_iAPWriterThread && m_Writer) // { modified by sungha.choi 2013.04.02, thread gracefully finish
    {
        m_Writer->onDeviceDisconnect();
        m_iAPWriterThread->quit();
    }

    //LOG_TRACE << "CIpodController::cleariAPChannel() OUT" << LOG_ENDL;
}

void CIpodController::readerThreadStarted()
{
    LOG_HIGH << "readerThreadStarted" << LOG_ENDL;
    //we can start ipod initialization if
    m_bReaderThreadStartedFlag = true;
    if(m_ControllerState == eStateiAPReady)
    {
        if(m_bWriterThreadStartedFlag == true && m_bStartiPodInitializationFlag == false)
        {
            m_bStartiPodInitializationFlag = true;
            startiPodInitialization();
        }
    }
    else
    {
        LOG_HIGH << "CRITICAL readerThreadStarted but m_ControllerState != eStateiAPReady" << LOG_ENDL;
    }
}

void CIpodController::writerThreadStarted()
{
    LOG_HIGH << "writerThreadStarted" << LOG_ENDL;
    m_bWriterThreadStartedFlag = true;
    if(m_ControllerState == eStateiAPReady)
    {
        if(m_bReaderThreadStartedFlag == true && m_bStartiPodInitializationFlag == false)
        {
            m_bStartiPodInitializationFlag = true;
            startiPodInitialization();
        }
    }
    else
    {
        LOG_HIGH << "CRITICAL writerThreadStarted but m_ControllerState != eStateiAPReady" << LOG_ENDL;
    }
}

/* Method will be called after iAP channel is ready
* This will create the identifier,authorisor and remoteUI taskhandlers
*/
void CIpodController::startiPodInitialization()
{
    //LOG_HIGH << "CIpodController::startiPodInitialization() IN" << LOG_ENDL;

    qint16 localTransId = 1;

    m_PrivateData->setTransactionId(localTransId);
    m_PrivateData->m_ExtendedLingoVersion = 0.0;
    m_PrivateData->m_IsiOSAppCommunicationSupported = false;
    /*
    *  Note: The initialization of task handler should be in the same flow as below
    *  Identifier  - Authorizer - RemoteUiMode - PlayEngine - ITune
    */
    m_Identifier = new CIdentifierTaskHandler(this);

    connect(m_Identifier, SIGNAL(identificationType(bool)), this, SLOT(startIdentification(bool)), Qt::QueuedConnection);
    connect(m_Identifier, SIGNAL(reconnection()), this, SLOT(restartUcs()));   // added by sungha.choi 2013.03.07

    LOG_HIGH  << "CIpodController::startiPodInitialization(), call m_Identifier->StartIDPS()" << LOG_ENDL;
    m_Identifier->StartIDPS();
    //LOG_HIGH << "CIpodController::startiPodInitialization OUT"<<LOG_ENDL;

#ifdef ENABLE_TEST_BUG_PORT_RESET
    int nRandomSeconds = rand()% 9;
    m_pPortResetTimer->setInterval(nRandomSeconds*100);
    m_pPortResetTimer->setSingleShot(true);
    m_pPortResetTimer->start();
    //LOG_HIGH << "TanDebug Will randomly reset port after " << nRandomSeconds*100 << " ms"<<LOG_ENDL;
#endif
}


#ifdef ENABLE_TEST_BUG_PORT_RESET
void CIpodController::portResetTimerTimedOutSlot()
{
    //LOG_HIGH << "TanDebug portResetTimerTimedOutSlot restartUcs"<< LOG_ENDL;
    restartUcs();
}
#endif

void CIpodController::startIdentification(bool type)
{
    LOG_HIGH << "CIpodController::startIdentification()" << LOG_ENDL;

    if(m_ControllerState == eStateIdle)    // added by sungha.choi 2013.04.12
        return;

    if(m_Identifier)
    {
        disconnect(m_Identifier, SIGNAL(reconnection()), this, SLOT(restartUcs()));   // added by sungha.choi 2013.03.07
        m_Identifier->stopCurrentTask();
        m_Identifier->stopTaskHandler();
        m_Identifier->deleteLater();    // modified by sungha.choi 2013.04.02, mainloopquit() -> deletelater(), eventqueue
        m_Identifier = NULL;
    }
    else
    {
        //LOG_TRACE << "CIpodController::startIdentification(), m_Identifier is NULL"<<LOG_ENDL;
    }
    
    if( type )
    {
        CIdentifierIDPSTaskHandler* Identifier = new CIdentifierIDPSTaskHandler(this);
        m_Identifier = Identifier;
        connect(m_Identifier, SIGNAL(reconnection()), this, SLOT(restartUcs()));   // added by sungha.choi 2013.03.07
    }
    else
    {
        CIdentifierNonIDPSTaskHandler* Identifier = new CIdentifierNonIDPSTaskHandler(this);
        m_Identifier = Identifier;
    }

    if(m_Identifier)
    {
        connect(m_Identifier, SIGNAL(identified(bool)), this, SLOT(IdentificationProcessFinished(bool)), Qt::QueuedConnection);
    }

    m_Authorisor = new CAuthorisorTaskHandler(this);
    if(m_Authorisor)
    {
        connect(m_Authorisor, SIGNAL(authorised(bool)), this, SLOT(authorisationProcessFinished(bool)), Qt::QueuedConnection);
    }

    m_RemoteUiTaskHandler = new CRemoteUIModeTaskHandler(this);
    if(m_RemoteUiTaskHandler)
    {
        m_RemoteUiTaskHandler->setRemoteUIProductID( m_productId ); // added by jungae 2012.08.17 to get the device product id

        connect(m_RemoteUiTaskHandler, SIGNAL(EnteredInUiMode(bool)),    this, SLOT(extendedModeSwitched(bool)),Qt::QueuedConnection);
        connect(m_RemoteUiTaskHandler, SIGNAL(GetDeviceName(QString)),   this, SIGNAL(OnGetDeviceName(QString))); // added by jungae 2012.08.16 to get the iPod device name

        connect(m_Authorisor,          SIGNAL(authorised(bool)),         m_RemoteUiTaskHandler, SLOT(authorisationFinished(bool)));
        connect(m_Identifier,          SIGNAL(identified(bool)),         m_RemoteUiTaskHandler, SLOT(IdentificationProcessFinished(bool)));

        connect(m_RemoteUiTaskHandler, SIGNAL(identify_restart_signal()), m_Identifier, SLOT(identify_restart_slot()));
        connect(m_RemoteUiTaskHandler, SIGNAL(identify_restart_signal()), m_Authorisor, SLOT(Authorisor_restart_slot()));
        connect(m_RemoteUiTaskHandler, SIGNAL(reconnection()),            this,         SLOT(restartUcs()));   // added by sungha.choi 2013.04.03
    }
    //Set the lingo and the FID token values to be
    //set to the device in the array which will be passed and
    //used by the General lingo commands later

    if(!m_ITunesTaskHandler)
    {
        m_ITunesTaskHandler = new CIPodDBFetchTaskHandler(this, m_PrivateData, m_Writer, m_Reader);
    }

    QBitArray lingoArray;
    QList <FIDTokenValue> fidTokenArray;

    lingoArray.resize(15);
    lingoArray.fill(0);
    lingoArray.setBit(0x00);
    lingoArray.setBit(0x03);        // sungha.choi added for display remote lingo support
    lingoArray.setBit(0x04);
    lingoArray.setBit(0x0A);

    fidTokenArray.clear();
    fidTokenArray.append(FIDTokenValue(gIdentifyToken));
    fidTokenArray.append(FIDTokenValue(gAccCapsToken));
    fidTokenArray.append(FIDTokenValue(gAccInfoToken));

    m_Identifier->identify(lingoArray, fidTokenArray);

    //LOG_HIGH << "CIpodController::startIdentification() OUT" << LOG_ENDL;
}

void CIpodController::CheckIpodConnectionStatus()
{
    //LOG_HIGH<<"Request Identify Ipod should have just been disconnected"<<m_Detector->GetDetectorState()<<LOG_ENDL;
    QTimer::singleShot(5000, this, SLOT(CheckIpodConnectionStatusSlot5()));
}

void CIpodController::CheckIpodConnectionStatusSlot5()
{
    //LOG_HIGH<<"Request Identify 5 Checking if ipod is still connected"<<m_Detector->GetDetectorState()<<LOG_ENDL;
    if(m_Detector->GetDetectorState() == eiPodDeviceDisconnected)
    {
        LOG_HIGH<<"Request Identify Sending disconnect signal to AVP"<<LOG_ENDL;
        m_PrivateData->m_restartUcsFlag = false;
        disconnectSignalCaught();
    }
}

void CIpodController::resetCurrentIPodPort()
{
    QFile m_PortResetFile("/proc/dh_usb_port_reset");

    if( m_PortResetFile.open(QIODevice::WriteOnly|QIODevice::Text) == false )
    {
        LOG_HIGH << "CIpodController::restartUcs(), We try to reset USB port, but cannot open port reset file ( /proc/dh_usb_port_reset )" << LOG_ENDL;
        return;
    }
    if(m_PrivateData->m_currentPortNum == eIPOD_PORT_1)
    {
        m_PortResetFile.write("1");
        LOG_HIGH << "CIPodController::restartUcs(), Front USB port will be resetted" << LOG_ENDL;
    }
    else if(m_PrivateData->m_currentPortNum == eIPOD_PORT_2)    // added by sungha.choi 2013.03.29 for rear usb port reset
    {
        m_PortResetFile.write("2");
        LOG_HIGH << "CIPodController::restartUcs(), Rear  USB port will be resetted" << LOG_ENDL;
    }

    if( m_PortResetFile.isOpen() == true )
        m_PortResetFile.close();
}

void CIpodController::restartUcs()  // added by sungha.choi 2013.03.07
{
    //LOGH LOG_ENDL;

    //close the file descriptor.
    emit closeFileDescriptorSignal();

    if(m_restartUcsCount < RESTART_UCS_MAX_COUNT)
    {
        m_PrivateData->m_restartUcsFlag = true;    // added by sungha.choi 130322
        m_restartUcsCount++;

        resetCurrentIPodPort();

        //LOG_TRACE << "CIpodController::restartUcs() m_restartUcsCount = " << m_restartUcsCount << LOG_ENDL;
    }
    else
    {
        m_restartUcsCount = 0;
        LOG_HIGH << "CIpodController::restartUcs() : limit of RestartUCS count" <<LOG_ENDL;
        //resetting m_restartUcsCount to allow retry after iphone connect - disconnect.
        //emit  IPodPopUp(IPOD_POPUP_UNSUPPORTED);
    }
    //LOG_TRACE << "CIpodController::restartUcs() OUT" << LOG_ENDL;
}

void CIpodController::startUcs()  // added by sungha.choi 2013.04.12
{
    //LOGH LOG_ENDL;
    if(m_PrivateData->m_restartUcsFlag == true ) // modified by sungha.choi 2013.03.22
    {
        if(m_PrivateData->m_currentPortNum == eIPOD_PORT_1)
        {
            if(CUcsControl::startUcs())
            {
                //LOG_TRACE << "   CUcsControl::startUcs() true : restart Ucs" << LOG_ENDL;
            }
            else
            {
                //LOG_TRACE << "   CUcsControl::startUcs() false : already Ucs turn on status" << LOG_ENDL;
            }
        }
    }
    //LOG_TRACE << "CIpodController::startUcs() OUT" << LOG_ENDL;
}

void CIpodController::trackerConnectorDestroyedSlot()
{
    //LOG_HIGH << "TanLog: tracker connector destroyed slot" << LOG_ENDL;
    m_TrackerConnector = NULL; //the actual deletion is being done by delete later.
}
void CIpodController::dbThreadDestroyedSlot()
{
    //LOG_HIGH << "TanLog: db thread destroyed slot" << LOG_ENDL;
    m_iPodDBSyncThread = NULL; //the actual deletion is being done by delete later.
}

int CIpodController::ondbChangedAfterDeletion()
{
    //LOG_TRACE << "CIpodController::ondbChangedAfterDeletion() IN" << LOG_ENDL;
    //LOG_TRACE << "CIpodController::ondbChangedAfterDeletion()  m_ControllerState  = " << GetControllerStateString() << LOG_ENDL;

    try
    {
        if((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady))
        {
            m_ControllerState = eStateAutoPlayReady;
            m_PrivateData->m_FirstPlayListSet = false;

            if(m_iPodDBSyncThread)
            {
                LOG_HIGH << "   ondbChangedAfterDeletion Previous DBSync thread is still running, wait for finish" << LOG_ENDL;
                //LOG_TRACE<< "TanLog calling stopDBSync" << LOG_ENDL;
                m_iPodDBSyncThread->stopDBSync();

                // m_iPodDBSyncThread->quit();  // Modified by sungha.choi 2013.05.16. for crash issue
                // 1. quit() is called internally by DBSyncThread, So main thread don’t call thread quit().

                //m_iPodDBSyncThread->wait(5000);
            }

            int count = 0;
            while(m_PlayEngineTaskHandler && (m_TrackerConnector || m_iPodDBSyncThread))
            {
                count++;
                if(count > 20)
                {
                    LOG_HIGH<< "TanLog: Maximum wait for deleteLater reached!"<<LOG_ENDL;
                    break;
                }
                //LOG_TRACE << "TanLog: waitForDeletion: " << count <<LOG_ENDL;
                m_PlayEngineTaskHandler->waitForResponceFromExitCode(50, ExitCode_AllExit);
            }
            m_TrackerConnector = NULL;
            m_iPodDBSyncThread = NULL;

            //LOG_TRACE << "TanLog: creating new thread" <<LOG_ENDL;

            //LOG_TRACE << "   emit OnCurrentTrackCountChanged(0)_"  << LOG_ENDL;
            emit OnCurrentTrackCountChanged(0);

            if( m_TrackerConnector == NULL ) // Added by sungha.choi 2013.05.16. for crash issue.
            {
                m_TrackerConnector = new CTrackerConnector(this);
                if( !m_TrackerConnector )
                {
                    LOG_HIGH << "CIpodController::ondbChangedAfterDeletion(), we cannot make TrackerConnector" << LOG_ENDL;
                    return IPOD_ERROR_GENERIC;
                }
            }

            connect(m_TrackerConnector, SIGNAL(initDBSyncComplete(bool)),               this, SLOT(postExtendedModeInitDoneEC(bool)));

            //connect(m_TrackerConnector, SIGNAL(initAutoPlayReady(bool,int)),            this, SLOT(startAutoPlay(bool,int)));

            connect(m_TrackerConnector, SIGNAL(OnCurrentPlayingTrackCount(int)),        this, SLOT(OnCurrentPlayingTrackCountSlot(int))); //added by junam 2013.04.23 for index DB
            connect(m_TrackerConnector, SIGNAL(IPODConnected(QString,int,int,int,int)), this, SIGNAL(IPODConnected(QString,int,int,int,int)));
            connect(m_TrackerConnector, SIGNAL(IpodTracksAfterSync(int)),               this, SIGNAL(IPodTotalNumTracks(int)));
            connect(m_TrackerConnector, SIGNAL(IPodPlayableSongCount(int,int)),         this, SIGNAL(IPodPlayableSongCount(int,int)));
            connect(m_TrackerConnector, SIGNAL(OnCurrentTrackCountChanged(int)),        this, SIGNAL(OnCurrentTrackCountChanged(int))); //added by junam 2013.04.23 for index DB

            if(m_PlayEngineTaskHandler != NULL)
            {
                connect( m_TrackerConnector, SIGNAL(OnCurrentTrackCountChanged(int)), m_PlayEngineTaskHandler, SLOT(onCurrentTrackCountChangedSlot(int)));
            }

            //connect the signal on the tracker abs signal for the cover art fetching complete
            bool retpp = connect(m_TrackerConnector, SIGNAL(artworkFetched(QStringList)), this, SLOT(onCoverArtFetchSlot(QStringList)));
            //LOG_TRACE << "   connect tracker signal to ipod slot: " << retpp <<LOG_ENDL;

            //LOG_TRACE << "   creating SyncThread" << LOG_ENDL;
            m_iPodDBSyncThread = new CiPodDBSyncThread(this, m_TrackerConnector);

            if( !m_iPodDBSyncThread )
            {
                return IPOD_ERROR_GENERIC;
            }

            m_DBDeleteSyncFlag = false;

            //{Added by Tan 2014.07.23 Crash Issue
            //Use the started signal to trigger the request db sync!!
            //directly call the ->requestDBSync before the thread actually started will cause sigabrt
            connect(m_iPodDBSyncThread, SIGNAL(started()), m_iPodDBSyncThread, SLOT(requestDBSync()));
            //} Tan 2014.07.23 Crash Issue

            connect(m_iPodDBSyncThread, SIGNAL(finished()), m_TrackerConnector, SLOT(deleteLater()));  // deleted by sungha.choi 2013.04.02
            connect(m_iPodDBSyncThread, SIGNAL(finished()), m_iPodDBSyncThread, SLOT(deleteLater()));  // deleted by sungha.choi 2013.04.02
            connect(m_TrackerConnector, SIGNAL(destroyed()), this, SLOT(trackerConnectorDestroyedSlot()));
            connect(m_iPodDBSyncThread, SIGNAL(destroyed()), this, SLOT(dbThreadDestroyedSlot()));

            m_TrackerConnector->moveToThread(m_iPodDBSyncThread);
            //LOG_TRACE<< "   Tracker Connector moved to separate thread" << LOG_ENDL;

            //LOG_HIGH << "CIpodController::ondbChangedAfterDeletion(), call m_iPodDBSyncThread->start()" << LOG_ENDL;
            m_iPodDBSyncThread->start();


            //{Commented out by Tan 2014.07.23 Crash Issue
            //Use the started signal to trigger the request db sync!!
            //directly call the ->requestDBSync before the thread actually started will cause sigabrt
            //m_iPodDBSyncThread->requestDBSync();    // added by sungha.choi 2013.04.16
            //}Commented out by Tan 2014.07.23

            LOG_HIGH << "CIpodController::ondbChangedAfterDeletion(), emit onIPodConnectedSignal() for EVT_IPOD_ENABLED to UISH" << LOG_ENDL;
            emit onIPodConnectedSignal( "", "", m_PrivateData->m_currentPortNum, false );
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::ondbChangedAfterDeletion(), DisConnect Exeption caught!!!"<<LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }

    //LOG_TRACE << "CIpodController::ondbChangedAfterDeletion() OUT" << LOG_ENDL;
    return IPOD_SUCCESS;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::startPostExtModeInit                    int (OUT), (void) (IN)
//
// Explanation
// This method does the initialization of all the task handlers present in the
// controller
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIpodController::startPostExtModeInitEC()
{
    //LOGH LOG_ENDL;

    if(m_Authorisor)
    {
        m_Authorisor->stopTaskHandler();
    }

    try
    {
        m_PrivateData->m_FirstPlayListSet          = false;
        m_PrivateData->m_IsConvertedFromUIDToIndex = false;

        m_iOSAppTaskHandler = new CiOSAppTaskHandler(this, m_Detector->piPodLog);    // modified by sungha.choi 2013.04.12
        connect(m_iOSAppTaskHandler, SIGNAL(OnBasicControlEnableStatusChange(bool, QString)), this, SIGNAL(OnBasicControlEnableStatusChange(bool, QString)));
        connect(m_iOSAppTaskHandler, SIGNAL(OnTrackChanged(int)), this, SIGNAL(OnTrackChanged(int)));
#ifdef SIRI_TIMEOUT_TEST
        connect(m_iOSAppTaskHandler, SIGNAL(LaunchedDefaultMusicApp(bool,QString)),           this, SIGNAL(LaunchedDefaultMusicApp(bool,QString)));
#endif

        double dVersion  = m_ITunesTaskHandler->lingoProtocolVersion(4);
        int    productID = 0;
        if( m_Writer ) productID = m_Writer->getProductID();

        bool isIPAD23 = ((productID == IPAD_2G_WIFI_ID)
                         || (productID == IPAD_2G_GSM_ID)
                         || (productID == IPAD_2G_CDMA_ID)
                         || (productID == IPAD_2G_WIFI_RA_ID)
                         || (productID == IPAD_3G_WIFI_ID )
                         || (productID == IPAD_3G_CDMA_ID)
                         || (productID == IPAD_3G_GLOBAL_ID));
        bool isiOSbelow52 = ((m_PrivateData->m_MajorVersion < 5) ||
                             ((m_PrivateData->m_MajorVersion == 5) &&
                              (m_PrivateData->m_MinorVersion < 2)));
        if((productID == IPAD_1G_PRODUCT_ID) || (isIPAD23 && isiOSbelow52))
        {
            m_PrivateData->m_ExtendedLingoVersion = 1.13;
            dVersion = m_PrivateData->m_ExtendedLingoVersion;
        }

        if( dVersion >= (double)1.14 && m_PrivateData->m_isIDPS && (productID != NANO_5G_PRODUCT_ID) && (productID != NANO_6G_PRODUCT_ID))
        {

            //LOG_TRACE << "UID_Index_Merge:StartPostExtendedModeInit() making it a converted device" << LOG_ENDL;
            m_PrivateData->m_ExtendedLingoVersion = 1.13;
            m_PrivateData->m_IsConvertedFromUIDToIndex = true;
        }

        disconnect(this, SIGNAL(OnStateChanged(int)), this, SLOT(catchOnStateChangeSignal(int)));
        connect(this, SIGNAL(OnStateChanged(int)), this, SLOT(catchOnStateChangeSignal(int)));

        //LOG_HIGH << "CIpodController::startPostExtModeInitEC() create PlayEngineTaskHandler() " << LOG_ENDL;
        m_PlayEngineTaskHandler = new CPlayEngineTaskHandler(this, m_Detector->piPodLog);    // modified by sungha.choi 2013.04.12

        if(m_PlayEngineTaskHandler)
        {
            m_PlayEngineTaskHandler->InitializePlayEngineTaskHandler();
            // SIGNAL ---> SLOT
            connect(m_PlayEngineTaskHandler, SIGNAL(fetchCoversForBasicPlayer(QString, QString, bool)),    this, SLOT(fetchCoversForBasicPlayer(QString,QString, bool)));
            connect(m_PlayEngineTaskHandler, SIGNAL(OnPositionChanged(int)),                         this, SLOT(OnPositionChangedSlot(int)));
            connect(m_PlayEngineTaskHandler, SIGNAL(OnStateChanged(int)),                            this, SLOT(OnStateChangedSlot(int)));
            connect(m_PlayEngineTaskHandler, SIGNAL(OnCurrentMediaInfo(MediaInfo*)),                 this, SLOT(OnCurrentMediaInfoSent(MediaInfo*)));
            connect(m_PlayEngineTaskHandler, SIGNAL(OnScanCompleted()),                              this, SLOT(EmitScanComplete()), Qt::QueuedConnection);
            connect(m_PlayEngineTaskHandler, SIGNAL(OnCurrentPlayingTrackCount(int)),                this, SLOT(OnCurrentPlayingTrackCountSlot(int))); // Added by pawan 2013.05.01.
            connect(m_PlayEngineTaskHandler, SIGNAL(OnChangeDBIndexingStatus(bool)),                 this, SLOT(changeDBIndexingStatus(bool)));
            connect(m_PlayEngineTaskHandler, SIGNAL(dbChangedAfterDeletion()),                       this, SLOT(ondbChangedAfterDeletion()));

            // SIGNAL ---> SIGNAL
            connect(m_PlayEngineTaskHandler, SIGNAL(OnTrackChanged(int)),                            this, SIGNAL(OnTrackChanged(int)));
            connect(m_PlayEngineTaskHandler, SIGNAL(OnDurationChanged(int)),                         this, SIGNAL(OnDurationChanged(int)));
            connect(m_PlayEngineTaskHandler, SIGNAL(OnModeChanged(int)),                             this, SIGNAL(OnModeChanged(int)));
            connect(m_PlayEngineTaskHandler, SIGNAL(RequestComplete( QVector<QStringList>, ETrackerAbstarctor_Query_Ids, int, uint)),
                    this, SIGNAL(RequestComplete( QVector<QStringList>, ETrackerAbstarctor_Query_Ids, int, uint)));
        }
        else
        {
            LOG_HIGH << "CIpodController::startPostExtModeInitEC() Fail to create PlayEngineTaskHandler() !!" << LOG_ENDL;
            return IPOD_ERROR_GENERIC;
        }

        //send total umber of tracker to AVP to show the ipod tab in DH
        m_PrivateData->m_TotalIpodTrackCount = m_ITunesTaskHandler->getTotalCountOfiPodTracksE();
        emit IPodTotalNumTracks(m_PrivateData->m_TotalIpodTrackCount);

        LOG_HIGH << "EL ver = " << dVersion << ", ProductID = " << productID << ", TotTrackNum = " << m_PrivateData->m_TotalIpodTrackCount<< LOG_ENDL;
        //LOG_HIGH << "CIpodController::startPostExtModeInitEC(), m_TotalIpodTrackCount =  " << m_PrivateData->m_TotalIpodTrackCount << LOG_ENDL;

        //Becuase of continuous packets, sometimes the serial Number packet getting missed, becoz of which it may return NULL
        //so it is better to get the serial Number before the flow starts.
        QString  serialNumber = m_ITunesTaskHandler->getCurrentSerialNumberE();
        //LOG_TRACE << "   serialNumber = " << serialNumber << LOG_ENDL;

        // { Added by jonghwan.cho@lge.com 2013.07.16.
        //LOG_HIGH << "CIpodController::startPostExtModeInitEC() create CiTunesTagTaskHandler() " << LOG_ENDL;
        m_iTunesTagTaskHandler = new CiTunesTagTaskHandler(this, m_Detector->piPodLog);

        if( !m_iTunesTagTaskHandler ) 
        {
            LOG_HIGH << "CIpodController::startPostExtModeInitEC() Fail to create CiTunesTagTaskHandler() !!" << LOG_ENDL;
            return IPOD_ERROR_GENERIC;
        }
        connect( m_iTunesTagTaskHandler,   SIGNAL(IPodPopUp(EIPodPopUp)),   this,   SIGNAL(IPodPopUp(EIPodPopUp)));
        m_iTunesTagTaskHandler->startiTunesTagTaskHandlerE();
        // } Added by jonghwan.cho@lge.com 2013.07.16.

        /* DB Sync related initialization Start */
        /* Check whether previous BDSync thread is cleared, if not clean it*/
        if(m_iPodDBSyncThread)
        {
            LOG_HIGH << "   Previous DBSync thread is still running, wait for finish" << LOG_ENDL;
            m_iPodDBSyncThread->stopDBSync();
            m_iPodDBSyncThread->wait(5000);
            m_TrackerConnector = NULL;
            m_iPodDBSyncThread = NULL;
        }

        //LOG_HIGH << "CIpodController::startPostExtModeInitEC() create CTrackerConnector() " << LOG_ENDL;
        m_TrackerConnector = new CTrackerConnector(this);
        if(!m_TrackerConnector)
        {
            LOG_HIGH << "CIpodController::startPostExtModeInitEC() Fail to create CTrackerConnector() !!" << LOG_ENDL;
            return IPOD_ERROR_GENERIC;
        }

        // connect SIGNAL - SLOT of TrackerConnector
        connect( m_TrackerConnector, SIGNAL(initDBSyncComplete(bool)),              this, SLOT(postExtendedModeInitDoneEC(bool)));
        connect( m_TrackerConnector, SIGNAL(initAutoPlayReady(bool,int)),           this, SLOT(startAutoPlay(bool,int)));
        connect( m_TrackerConnector, SIGNAL(OnCurrentPlayingTrackCount(int)),       this, SLOT(OnCurrentPlayingTrackCountSlot(int))); //added by junam 2013.04.23 for index DB
        connect( m_TrackerConnector, SIGNAL(artworkFetched(QStringList)),           this, SLOT(onCoverArtFetchSlot(QStringList)));

        // connect SIGNAL - SLOT of TrackerConnector
        connect( m_TrackerConnector, SIGNAL(IPODConnected(QString,int,int,int,int)),this, SIGNAL(IPODConnected(QString,int,int,int,int)));
        connect( m_TrackerConnector, SIGNAL(IpodTracksAfterSync(int)),              this, SIGNAL(IPodTotalNumTracks(int)));
        connect( m_TrackerConnector, SIGNAL(IPodPlayableSongCount(int,int)),        this, SIGNAL(IPodPlayableSongCount(int,int)));
        connect( m_TrackerConnector, SIGNAL(OnCurrentTrackCountChanged(int)),       this, SIGNAL(OnCurrentTrackCountChanged(int))); //added by junam 2013.04.23 for index DB

        //{added by junam 2013.04.24 for Prev() in sync state
        if(m_PlayEngineTaskHandler != NULL)
        {
            connect( m_TrackerConnector, SIGNAL(OnCurrentTrackCountChanged(int)), m_PlayEngineTaskHandler, SLOT(onCurrentTrackCountChangedSlot(int)));
        }
        //}added by junam

        //LOG_HIGH<< "CIpodController::startPostExtModeInit(), creating iPodDBSyncThread()" << LOG_ENDL;
        m_iPodDBSyncThread = new CiPodDBSyncThread(this, m_TrackerConnector);
        if( !m_iPodDBSyncThread )
        {
            LOG_HIGH << "CIpodController::startPostExtModeInitEC() Fail to create m_iPodDBSyncThread() !!" << LOG_ENDL;
            return IPOD_ERROR_GENERIC;
        }

        m_DBDeleteSyncFlag = false;
        connect(m_iPodDBSyncThread, SIGNAL(finished()), m_TrackerConnector, SLOT(deleteLater()));  // deleted by sungha.choi 2013.04.02
        connect(m_iPodDBSyncThread, SIGNAL(finished()), m_iPodDBSyncThread, SLOT(deleteLater()));  // deleted by sungha.choi 2013.04.02

        //{Added by Tan 2014.07.23 Crash Issue
        //Use the started signal to trigger the request db sync!!
        //directly call the ->requestDBSync before the thread actually started will cause sigabrt
        connect(m_iPodDBSyncThread, SIGNAL(started()), m_iPodDBSyncThread, SLOT(requestDBSync()));
        //}Tan 2014.07.23 Crash Issue

        connect(m_TrackerConnector, SIGNAL(destroyed()), this, SLOT(trackerConnectorDestroyedSlot()));
        connect(m_iPodDBSyncThread, SIGNAL(destroyed()), this, SLOT(dbThreadDestroyedSlot()));

        m_TrackerConnector->moveToThread(m_iPodDBSyncThread);

        //LOG_HIGH << "   call m_iPodDBSyncThread->start()" << LOG_ENDL;
        m_iPodDBSyncThread->start();
        //LOG_HIGH << "   call m_iPodDBSyncThread->requestDBSync()" << LOG_ENDL;


        //{Commented out by Tan 2014.07.23 Crash Issue
        //Use the started signal to trigger the request db sync!!
        //directly call the ->requestDBSync before the thread actually started will cause sigabrt
        //m_iPodDBSyncThread->requestDBSync();    // added by sungha.choi 2013.04.16
        //}Commented out by Tan 2014.07.23

        if( m_IsSendedFirstConnectionSignal == true )
        {
            LOG_HIGH << "emit onIPodConnectedSignal() for EVT_IPOD_ENABLED to UISH" << LOG_ENDL;
            emit onIPodConnectedSignal( "", "", m_PrivateData->m_currentPortNum, false );
            m_bWaitForFirstConnectionSignal = false;
        }
        else
        {
            LOG_HIGH << "CIpodController::startPostExtModeInitEC(), First Connection Signal was not sent so we will waiting until first Connection Signal" << LOG_ENDL;
            m_bWaitForFirstConnectionSignal = true;
        }

        if(m_PrivateData->m_TotalIpodTrackCount == 0)
        {
            LOG_HIGH << "CIpodController::startPostExtModeInitEC(), OSD IPodPopUp(iPodiTunes_NONE)" << LOG_ENDL;
            emit IPodPopUp(iPodiTunes_NONE);
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::startPostExtModeInit() DisConnect Exeption caught!!!"<<LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }
    PT_PRINT_TIME(PT_LOG_FILE_NAME);

    //LOG_TRACE << "CIpodController::startPostExtModeInit() OUT"<<LOG_ENDL;
    return IPOD_SUCCESS;
}

void CIpodController::fetchCoversForBasicPlayer(QString albumName, QString artistName, bool forceFlag)
{

    QString albumNameForTrackerAbstractor = albumName;
    if(albumNameForTrackerAbstractor.isEmpty())
    {
        albumNameForTrackerAbstractor = QString(" ");
    }


    //LOG_TRACE << "basic fetchCoversForBasicPlayer() IN, " << "artistname: " << artistName << LOG_ENDL;
    if(!artistName.contains("file://"))
        artistName.prepend("file://");

    if(!m_PrivateData->m_TrackerObj || m_ControllerState == eStateIdle)
        return;

    bool isCoverartInCache = m_PrivateData->m_TrackerObj->RequestData(albumNameForTrackerAbstractor, artistName, eIsCoverArtExists_Id);
    //bool isCoverartInCache = false;

    QStringList albumList;
    QStringList artistList;

    if(isCoverartInCache && m_PrivateData->m_IsCurrentPlayingMP && !forceFlag)
    {
        m_PrivateData->updateBasicCoverArtVars(albumName, artistName);
        albumList.append(albumNameForTrackerAbstractor);
        artistList.append(artistName);
        bool ret = connect(m_PrivateData->m_TrackerObj, SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)),
                           this, SLOT(gotCoversForBasicPlayerFromTracker(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)));
        //LOG_TRACE << "basicp cover art present in cache" << ret << LOG_ENDL;
        m_PrivateData->m_TrackerObj->RequestData(albumList, artistList, eGetMusicAlbumCoverArt_Id);
    }
    else if(/*m_ControllerState == eStateiPodReady*/ 1)  // Modified by pawan 2013.05.15.
    {
        //LOG_TRACE << "basicp cover art fetching from ipod" << LOG_ENDL;
        if(m_PrivateData->m_CoverArtData.m_IsOnDemandCoverArtRequestInProgress || m_PrivateData->m_CoverArtData.m_IsBasicPlayerCoverArtRequestInProgress)
        {
            //LOG_TRACE << "basicp basic player or on demand cover art fetching from ipod already in progress" << LOG_ENDL;
        }
        else
        {
            m_PrivateData->updateBasicCoverArtVars(albumName, artistName);
            albumList.append(albumName);
            artistList.append(artistName);
            if( m_iPodDBSyncThread ) // Added by jonghwan.cho@lge.com 2013.05.17 for crash issue.
            {
                m_iPodDBSyncThread->fetchArtwork(albumList, artistList, forceFlag);
                //Tan. 2013.05.31: cover art not fetched when there is no playlist.
                //The flag should be set to true only if we actually fetching artwork.
                m_PrivateData->m_CoverArtData.m_IsBasicPlayerCoverArtRequestInProgress = true;
            }
            //Tan. Moved up there.
            //m_PrivateData->m_CoverArtData.m_IsBasicPlayerCoverArtRequestInProgress = true;
        }
    }
    else
    {
        m_PrivateData->updateBasicCoverArtVars(albumName, artistName);
        //LOG_TRACE << "basicp not in cache nor ipodready" << LOG_ENDL;
    }

    //LOG_TRACE << "basicp fetchCoversForBasicPlayer() OUT" << LOG_ENDL;
}

void CIpodController::gotCoversForBasicPlayerFromTracker(QVector<QStringList> list,ETrackerAbstarctor_Query_Ids /*id*/)
{
    //LOG_TRACE << "basicp gotCoversForBasicPlayerFromTracker() IN" << LOG_ENDL;
    if(!m_PrivateData->m_TrackerObj)
        return;

    bool ret = disconnect(m_PrivateData->m_TrackerObj, SIGNAL(RequesteComplete(QVector<QStringList> , ETrackerAbstarctor_Query_Ids )),
                          this, SLOT(gotCoversForBasicPlayerFromTracker(QVector<QStringList>,ETrackerAbstarctor_Query_Ids)));
    //LOG_TRACE << "basicp ret val for disconnect = " << ret << LOG_ENDL;

    if(!(list.count() > 0 && list[0].count() >0))
    {
        //LOG_TRACE << "basicp no cover arts" << LOG_ENDL;
        m_PrivateData->m_CoverArtData.m_RequestedCoverAlbumName.clear();
        m_PrivateData->m_CoverArtData.m_RequestedCoverArtistName.clear();
        return;
    }

    updateHMIForBasicPlayerCoverArt(list[0][0]);
    //LOG_TRACE << "basicp gotCoversForBasicPlayerFromTracker() OUT" << LOG_ENDL;
}

void CIpodController::gotCoversForBasicPlayerFromIPod(QStringList list)
{
    //LOG_TRACE << "basicp gotCoversForBasicPlayerFromIPod() IN" << LOG_ENDL;
    if(!m_PrivateData->m_TrackerObj)
        return;

    if(list.count() <= 0)
    {
        //LOG_TRACE << "basicp no cover arts" << LOG_ENDL;
        m_PrivateData->m_CoverArtData.m_RequestedCoverAlbumName.clear();
        m_PrivateData->m_CoverArtData.m_RequestedCoverArtistName.clear();
        return;
    }

    MediaInfo info = m_PrivateData->getCurrentMediaInfo();
    QString albumName(info.Album);
    QString artistName(info.fileName);
    //save covers into cache only for IPod music player

    if(m_PrivateData->m_IsCurrentPlayingMP && !(m_PrivateData->m_CoverArtData.m_RequestedCoverAlbumName.compare(info.Album) && m_PrivateData->m_CoverArtData.m_RequestedCoverArtistName.compare(info.fileName)))
    {
        SCoverartMusicData data;
        data.album.append(m_PrivateData->m_CoverArtData.m_RequestedCoverAlbumName);
        data.track_artist.append(m_PrivateData->m_CoverArtData.m_RequestedCoverArtistName);
        data.imagePath.append(list[0]);

        m_PrivateData->m_TrackerObj->UpdateData(&data, eInsertAlbumCoverArtDataQuery_Id);
    }

    QString urlcover(list[0]);
    //LOG_TRACE <<"basicp cover to update the HMI is = " << urlcover << LOG_ENDL;
    updateHMIForBasicPlayerCoverArt(urlcover);
    /*
    if(!m_PrivateData->m_IsCurrentPlayingMP && (m_PrivateData->m_CurrentArtworkRetyCountForOtherMP++) < 3)
    {
        bool retval = connect(this, SIGNAL(OnRetryCoverForOtherMP(QString,QString)), this, SLOT(RetryCoverForOtherMP(QString,QString)), Qt::QueuedConnection);
        //LOG_TRACE << "basicp emit OnRetryCoverForOtherMP()"  << LOG_ENDL;
        emit OnRetryCoverForOtherMP(albumName, artistName);
        //LOG_TRACE << "basicp retrying th covers : artistname: " << artistName <<" and album name: " << albumName << "and ret val: " << retval <<LOG_ENDL;
    }
    */
    //LOG_TRACE << "basicp gotCoversForBasicPlayerFromIPod() OUT" << LOG_ENDL;
}

void CIpodController::RetryCoverForOtherMP(QString albumName, QString artistName)
{
    //LOG_TRACE << "basicp RetryCoverForOtherMP() IN, retrying the cover for [ " << m_PrivateData->m_CurrentArtworkRetyCountForOtherMP << " ] time" <<LOG_ENDL;
    disconnect(this, SIGNAL(OnRetryCoverForOtherMP(QString,QString)), this, SLOT(RetryCoverForOtherMP(QString,QString)));
    try
    {
        if(m_PlayEngineTaskHandler)
            m_PlayEngineTaskHandler->waitForResponceFromExitCode(2000, 0x00);
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "basicp got the throw error while retrying the covers" << LOG_ENDL;
        return;
    }

    fetchCoversForBasicPlayer(albumName, artistName, true);
    //LOG_TRACE << "basicp RetryCoverForOtherMP() OUT" << LOG_ENDL;
}

void CIpodController::SetOSDCondition()
{
    if(m_PrivateData->m_OSDKey != OSDKey_None)
    {
        setTrackBufferForOSD(1);
    }
}


void CIpodController::ResetOSDConditionAtSongEnd(int msec)
{
    ////LOG_TRACE << "CIpodController::ResetOSDConditionAtSongEnd() IN, msec = " << msec << LOG_ENDL;
    int duration = 0;
    int timeLeft;
    if(m_PlayEngineTaskHandler)
    {
        duration = m_PlayEngineTaskHandler->getCurrentMediaDuration();
    }
    ////LOG_TRACE << "   duration = " << duration << LOG_ENDL;

    if(duration != 0)
    {
        timeLeft = duration - msec;
        if(timeLeft< 2000 && timeLeft>0)
        {
            m_PrivateData->m_OSDKey = OSDKey_None;
        }
    }
    ////LOG_TRACE << "CIpodController::ResetOSDConditionAtSongEnd() OUT" << LOG_ENDL;
}

void CIpodController::updateHMIForBasicPlayerCoverArt(QString coverURL)
{
    //LOGH LOG_ENDL;
    MediaInfo info = m_PrivateData->getCurrentMediaInfo();

    //LOG_TRACE << "basicp: CIpodController::updateHMIForBasicPlayerCoverArt " <<  m_PrivateData->m_CoverArtData.m_RequestedCoverAlbumName << LOG_ENDL;
    //LOG_TRACE << "basicp: CIpodController::updateHMIForBasicPlayerCoverArt " <<  info.Album << LOG_ENDL;
    //LOG_TRACE << "basicp: CIpodController::updateHMIForBasicPlayerCoverArt " <<  m_PrivateData->m_CoverArtData.m_RequestedCoverArtistName << LOG_ENDL;
    //LOG_TRACE << "basicp: CIpodController::updateHMIForBasicPlayerCoverArt " <<  info.fileName << LOG_ENDL;
    //LOG_TRACE << "basicp: CIpodController::updateHMIForBasicPlayerCoverArt coverURL: " << coverURL << LOG_ENDL;

    if(!m_PrivateData->m_CoverArtData.m_RequestedCoverAlbumName.compare(info.Album) &&
        !m_PrivateData->m_CoverArtData.m_RequestedCoverArtistName.compare(info.fileName))  // Modified by pawan 2013.05.16.
    {
        //LOG_TRACE << "basicp CIpodController::updateHMIForBasicPlayerCoverArt match found emitting signal" << LOG_ENDL;
        m_PrivateData->m_CoverArtData.m_RequestedCoverAlbumName.clear();
        m_PrivateData->m_CoverArtData.m_RequestedCoverArtistName.clear();
        info.coverarturl.clear();
        info.coverarturl.append(coverURL);
        m_PrivateData->setCurrentMediaInfo(info, true);

        OnCurrentMediaInfoSent(&info);

        if(m_PlayEngineTaskHandler)
            m_PlayEngineTaskHandler->m_bCoverArtUpdated = true;
    }
    else
    {
        //LOG_TRACE << "basicp CIpodController::updateHMIForBasicPlayerCoverArt No match found not emitting signal" << LOG_ENDL;
        m_PrivateData->updateBasicCoverArtVars(info.Album, info.fileName);  // Modified by pawan 2013.05.16.
        fetchCoversForBasicPlayer(info.Album, info.fileName, true);  // Modified by pawan 2013.05.16.
    }
    //LOG_TRACE << "basicp updateHMIForBasicPlayerCoverArt() OUT" << LOG_ENDL;
}

bool CIpodController::QueuingEvent(EHMIEvent newEvent, CIpodControllerQueueData& queueData)
{
    //LOG_TRACE << "CIpodController::QueuingEvent() IN, newEvent = " << newEvent << LOG_ENDL;
    bool ret = false;

    //LOG_TRACE << "   m_PrivateData->m_CurrentInProgressEvent = " << m_PrivateData->m_CurrentInProgressEvent << LOG_ENDL;

    for (int i=0; i< m_PrivateData->m_EventCommandQueue.count(); i++)
    {
        //LOG_TRACE << "   already Queued Command " << i << " : " << m_PrivateData->m_EventCommandQueue[i] << LOG_ENDL;
    }

    ret = true;
    switch (newEvent)
    {
    case EPlayEvent:
    {
        m_PrivateData->m_EventCommandQueue.removeAll(ENextEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EFFwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(ERwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPrevEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPlayEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPauseEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EStopEvent);
        m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case EPauseEvent:
    {
        m_PrivateData->m_EventCommandQueue.removeAll(ENextEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EFFwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(ERwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPrevEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EStopEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPauseEvent);
        m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case EStopEvent:
    {
        m_PrivateData->m_EventCommandQueue.removeAll(ENextEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EFFwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(ERwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPrevEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EStopEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPauseEvent);
        m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case ENextEvent:
    {
        m_PrivateData->m_EventCommandQueue.removeAll(ENextEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EFFwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(ERwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPrevEvent);
        m_PrivateData->m_QueueData.m_NextLevel++;
        if(m_PrivateData->m_QueueData.m_NextLevel < 0)
            m_PrivateData->m_EventCommandQueue.append(EPrevEvent);
        else if(m_PrivateData->m_QueueData.m_NextLevel > 0)
            m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case EPrevEvent:
    {
        m_PrivateData->m_EventCommandQueue.removeAll(ENextEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EFFwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(ERwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPrevEvent);
        m_PrivateData->m_QueueData.m_PrevForced = queueData.m_PrevForced;
        m_PrivateData->m_QueueData.m_NextLevel--;
        if(m_PrivateData->m_QueueData.m_NextLevel < 0)
            m_PrivateData->m_EventCommandQueue.append(newEvent);
        else if(m_PrivateData->m_QueueData.m_NextLevel > 0)
            m_PrivateData->m_EventCommandQueue.append(ENextEvent);
        break;
    }
    case EFFwdEvent:
    case ERwdEvent:
    {
        m_PrivateData->m_EventCommandQueue.removeAll(EFFwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(ERwdEvent);
        m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case ERandomEvent:
    {
        m_PrivateData->m_EventCommandQueue.removeAll(ERandomEvent);
        m_PrivateData->m_QueueData.m_RandomMode = queueData.m_RandomMode;
        m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case ERepeatEvent:
    {
        m_PrivateData->m_EventCommandQueue.removeAll(ERepeatEvent);
        m_PrivateData->m_QueueData.m_RepeatMode = queueData.m_RepeatMode;
        m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case ESetPlaylistEvent:
    {
        m_PrivateData->m_EventCommandQueue.removeAll(ENextEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EFFwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(ERwdEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPrevEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPlayEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EPauseEvent);
        m_PrivateData->m_EventCommandQueue.removeAll(EStopEvent);
        QList<QString> uris = *queueData.m_List->list;
        m_PrivateData->m_QueueData.m_List = queueData.m_List;
        m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case ESetCurrentIndex:
    {
        m_PrivateData->m_QueueData.m_CurrentIndex = queueData.m_CurrentIndex;
        m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case ESetMediaPosEvent: // added by sungha.choi 2013.04.19
    {
        m_PrivateData->m_EventCommandQueue.removeAll(ESetMediaPosEvent);
        m_PrivateData->m_QueueData.m_position = queueData.m_position;
        m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case ERequestDataEvent:
    {
        m_PrivateData->m_EventCommandQueue.removeAll(ERequestDataEvent);
        m_PrivateData->m_QueueData.m_RequestData = queueData.m_RequestData;
        m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case EPlayiPodFilesEvent:
    {
        m_PrivateData->m_EventCommandQueue.removeAll(EPlayiPodFilesEvent);
        m_PrivateData->m_EventCommandQueue.append(newEvent);
        break;
    }
    case EIdle:
    default:
        break;
    }

    for (int j=0; j< m_PrivateData->m_EventCommandQueue.count(); j++)
    {
        //LOG_TRACE << "   Post Queued Command " << j << " : " << m_PrivateData->m_EventCommandQueue[j] << LOG_ENDL;
    }
    
    //LOG_TRACE << "CIpodController::QueuingEvent() OUT" << LOG_ENDL;
    return ret;
}

void CIpodController::ExecuteAlreadyQueuedEvent()
{
    //LOG_TRACE << "CIpodController::ExecuteAlreadyQueuedEvent() IN" << LOG_ENDL;
    //Timer is removed due to the issue we are getting in multiple next use case,
    //where 10 time next does not work fine
    if( (m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady)
            || (m_ControllerState == eStateReadyToAutoPlayReady) )
        QTimer::singleShot(5, this, SLOT(ExecuteAlreadyQueuedEventSlot()));

    //LOG_TRACE << "CIpodController::ExecuteAlreadyQueuedEvent() OUT" << LOG_ENDL;
}

void CIpodController::ExecuteAlreadyQueuedEventSlot()
{
    //LOG_TRACE << "CIpodController::ExecuteAlreadyQueuedEventSlot() IN" << LOG_ENDL;
    if(m_ControllerState == eStateIdle)
    {
        m_PrivateData->m_CurrentInProgressEvent = EIdle;
        m_PrivateData->m_QueueData.clearQueueData();
        m_PrivateData->m_EventCommandQueue.clear();
    }
    if(m_PrivateData->m_CurrentInProgressEvent != EIdle)
    {
        return;
    }

    // Commented by jonghwan.cho@lge.com 2013.10.16.
    //EHMIEvent lastProcessedEvent = m_PrivateData->m_CurrentInProgressEvent;

    bool processNextEvent = false;
    if(m_PrivateData->m_EventCommandQueue.count() > 0)
    {
        if(m_PrivateData->m_EventCommandQueue.count() > 0)
            processNextEvent = true;
    }

    if(!processNextEvent)
    {
        if(m_PrivateData->m_QueueData.m_NextLevel < 0)
        {
            m_PrivateData->m_EventCommandQueue.append(EPrevEvent);
            ExecuteAlreadyQueuedEvent();
        }
        else if(m_PrivateData->m_QueueData.m_NextLevel > 0)
        {
            m_PrivateData->m_EventCommandQueue.append(ENextEvent);
            ExecuteAlreadyQueuedEvent();
        }
        return;
    }

    //LOG_TRACE << "CIpodController::ExecuteAlreadyQueuedEventSlot command to execute is: " << m_PrivateData->m_EventCommandQueue[0] << LOG_ENDL;

    EHMIEvent event = m_PrivateData->m_EventCommandQueue[0];

    if((event == ENextEvent || event == EPrevEvent) && m_bDelayNextPrev == true)
    {
        //LOG_TRACE << "Delaying execution of next" << LOG_ENDL;
        return;
    }

    m_PrivateData->m_EventCommandQueue.removeAt(0);
    switch(event)
    {
        case EPlayEvent:
            m_PrivateData->m_CurrentInProgressEvent = EPlayEvent;
            PlayFromQueue();
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
        case EPauseEvent:
            m_PrivateData->m_CurrentInProgressEvent = EPauseEvent;
            PauseFromQueue();
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
        case EStopEvent:
            m_PrivateData->m_CurrentInProgressEvent = EStopEvent;
            StopFromQueue();
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
        case ENextEvent:
            m_PrivateData->m_CurrentInProgressEvent = ENextEvent;
            NextFromQueue(0);
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            delayResetOSDKey(5000);
            break;
        case EPrevEvent:
            m_PrivateData->m_CurrentInProgressEvent = EPrevEvent;
            PrevFromQueue(0, m_PrivateData->m_QueueData.m_PrevForced);
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            delayResetOSDKey(5000);
            break;
        case ERandomEvent:
            m_PrivateData->m_CurrentInProgressEvent = ERandomEvent;
            RandomPlayFromQueue(m_PrivateData->m_QueueData.m_RandomMode, 0);
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
        case ERepeatEvent:
            m_PrivateData->m_CurrentInProgressEvent = ERepeatEvent;
            RepeatPlayFromQueue(m_PrivateData->m_QueueData.m_RepeatMode, 0);
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
        case EFFwdEvent:
            m_PrivateData->m_CurrentInProgressEvent = EFFwdEvent;
            FastForwardFromQueue(0);
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
        case ERwdEvent:
            m_PrivateData->m_CurrentInProgressEvent = ERwdEvent;
            RewindFromQueue(0);
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
        case ESetPlaylistEvent:
            m_PrivateData->m_CurrentInProgressEvent = ESetPlaylistEvent;
            SetPlayListFromQueue(m_PrivateData->m_QueueData.m_List);
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
        case ESetCurrentIndex:
            m_PrivateData->m_CurrentInProgressEvent = ESetCurrentIndex;
            SetCurrentIndexFromQueue(m_PrivateData->m_QueueData.m_CurrentIndex);
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            delayResetOSDKey(5000);
            break;
        case ESetMediaPosEvent: // added by sungha.choi 2013.04.19
            m_PrivateData->m_CurrentInProgressEvent = ESetMediaPosEvent;
            SetCurrentMediaPositionFromQueue(m_PrivateData->m_QueueData.m_position);
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
        case ERequestDataEvent:
            m_PrivateData->m_CurrentInProgressEvent = ERequestDataEvent;
            requestDataFromQueue((ETrackerAbstarctor_Query_Ids) m_PrivateData->m_QueueData.m_RequestData.m_queryId,
                        m_PrivateData->m_QueueData.m_RequestData.m_reqUID,
                        m_PrivateData->m_QueueData.m_RequestData.m_playlistIndex,
                        m_PrivateData->m_QueueData.m_RequestData.m_startIndex,
                        m_PrivateData->m_QueueData.m_RequestData.m_nCount);
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
        case EPlayiPodFilesEvent:
            m_PrivateData->m_CurrentInProgressEvent = EPlayiPodFilesEvent;
            PlayiPodFilesFromQueue();
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
        case EIdle:
        default:
            m_PrivateData->m_CurrentInProgressEvent = EIdle;
            break;
    }
    ExecuteAlreadyQueuedEvent();
    //LOG_TRACE << "CIpodController::ExecuteAlreadyQueuedEventSlot() OUT" << LOG_ENDL;
}

void CIpodController::catchOnStateChangeSignal(int state)
{
    //LOG_TRACE << "CIpodController::catchOnStateChangeSignal() IN, state = " << state << LOG_ENDL;
    if(m_PlayEngineTaskHandler)
    {
//        m_PlayEngineTaskHandler->m_playState = state;
    }
    //LOG_TRACE << "CIpodController::catchOnStateChangeSignal() OUT" << LOG_ENDL;
}

void CIpodController::onCoverArtFetchSlot(QStringList listt)
{
    //LOG_TRACE << "CIpodController::onCoverArtFetchSlot() IN" << LOG_ENDL;

    if(m_PrivateData->m_CoverArtData.m_IsOnDemandCoverArtRequestInProgress && m_PrivateData->m_CoverArtData.m_RequestedCoverAlbumName.length() > 0)
    {
        //LOG_TRACE << "basicp ondemand finishes now going for the basic player cover art fetch" << LOG_ENDL;
        m_PrivateData->m_CoverArtData.m_IsOnDemandCoverArtRequestInProgress = false;
        m_PrivateData->m_CoverArtData.m_IsBasicPlayerCoverArtRequestInProgress = true;

        QStringList albumList;
        albumList.append(m_PrivateData->m_CoverArtData.m_RequestedCoverAlbumName);
        QStringList artistList;
        artistList.append(m_PrivateData->m_CoverArtData.m_RequestedCoverArtistName);

        m_iPodDBSyncThread->fetchArtwork(albumList, artistList, true);  // Modified by pawan 2013.05.15.
    }
    else if(m_PrivateData->m_CoverArtData.m_IsBasicPlayerCoverArtRequestInProgress)
    {
        //LOG_TRACE << "basicp basic player cover art fetch finishes " << LOG_ENDL;
        m_PrivateData->m_CoverArtData.m_IsBasicPlayerCoverArtRequestInProgress = false;
        m_PrivateData->m_CoverArtData.m_IsOnDemandCoverArtRequestInProgress = false;
        gotCoversForBasicPlayerFromIPod(listt);
        if(m_PrivateData->m_CoverArtData.m_IsBasicPlayerCoverArtRequestInProgress)
        {
            //LOG_TRACE << "basicp again going for the basic player cover art as it has been changed " << LOG_ENDL;
        }
        else if(m_PrivateData->m_CoverArtData.m_AlbumList.count() > 0)
        {
            //LOG_TRACE << "basicp going for the on demand cover art request " << LOG_ENDL;
            m_PrivateData->m_CoverArtData.m_IsOnDemandCoverArtRequestInProgress = true;
            m_iPodDBSyncThread->fetchArtwork(m_PrivateData->m_CoverArtData.m_AlbumList, m_PrivateData->m_CoverArtData.m_ArtistList, false);
            m_PrivateData->m_CoverArtData.m_AlbumList.clear();
            m_PrivateData->m_CoverArtData.m_ArtistList.clear();
        }
        return;
    }
    m_PrivateData->m_CoverArtData.m_IsOnDemandCoverArtRequestInProgress = false;

    bool ret2 = connect(this, SIGNAL(coverArtFetched(QStringList)), m_TrackerAbs, SLOT(onCoverArtsFromIPOD(QStringList)));
    emit coverArtFetched(listt);
    bool ret3 = disconnect(this, SIGNAL(coverArtFetched(QStringList)), m_TrackerAbs, SLOT(onCoverArtsFromIPOD(QStringList)));
    //LOG_TRACE << "CIpodController::onCoverArtFetchSlot() OUT, ret2 = " << ret2 << ", ret3 = " << ret3 << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::stopTaskHandlers                    int (OUT), (void) (IN)
//
// Explanation
// This methodis responsible to stop the cuurent task of the taskhandler when the iPod
// is disconneted from head unit.
// History
// 29-Nov-2011      Venkataramana K                 Added initial implementation
*****************************************************************************/
int CIpodController::stopTaskHandlers()
{
    //LOG_TRACE << "CIpodController::stopTaskHandlers() IN" << LOG_ENDL;
    if(m_iPodDBSyncThread)
    {
        m_iPodDBSyncThread->stopDBSync();
    }

    if(m_Identifier)
    {
        m_Identifier->stopCurrentTask();
        m_Identifier->stopTaskHandler();
    }

    if(m_ITunesTaskHandler)
    {
        m_ITunesTaskHandler->stopCurrentTask();
        m_ITunesTaskHandler->stopTaskHandler();
    }

    if(m_iOSAppTaskHandler)
    {
        m_iOSAppTaskHandler->stopCurrentTask();
        m_iOSAppTaskHandler->stopTaskHandler();
    }

    if(m_PlayEngineTaskHandler)
    {
        m_PlayEngineTaskHandler->stopCurrentTask();
        m_PlayEngineTaskHandler->stopTaskHandler();
    }

    if(m_iTunesTagTaskHandler)
    {
        m_iTunesTagTaskHandler->stopCurrentTask();
        m_iTunesTagTaskHandler->stopTaskHandler();
    }

    //LOG_TRACE << "CIpodController::stopTaskHandlers() OUT" << LOG_ENDL;
    return IPOD_SUCCESS;
}

void CIpodController::destroyTaskHandlerSlot()
{
    if(1 == destroyTaskHandler())
    {
        m_Detector->iPodDeviceDisconnectionComplete();
    }
    else
    {
        m_Detector->requestDestroyTask();  // re-emit destroyTaskHandlerSlot by Event Queue
    }
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::destroyTaskHandler                    int (OUT), (void) (IN)
//
// Explanation
// This method does the destruction of all the task handlers present in the
// controller
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIpodController::destroyTaskHandler()
{
    //LOG_HIGH << "CIpodController::destroyTaskHandler()" << LOG_ENDL;

    int bisAllDeleted = 1;

    if( m_iPodDBSyncThread )
    {
        m_iPodDBSyncThread->wait(3000);
        if(!m_iPodDBSyncThread->isFinished())
        {
            //LOG_TRACE << "   m_iPodDBSyncThread terminate Critical Issue !!!!!!!!!!!!!!" << LOG_ENDL;
        }
        m_TrackerConnector = NULL;
        m_iPodDBSyncThread = NULL;
    }

    if(m_iAPReaderThread)
    {
        m_iAPReaderThread->wait(3000);  // added by sungha.choi 2013.04.01
        if(!m_iAPReaderThread->isFinished())
        {
            m_Detector->piPodLog->WriteLog("m_iAPReaderThread is not finished !!!!!!!!!!!!!!");
            //LOG_TRACE << "   m_iAPReaderThread terminate Critical Issue !!!!!!!!!!!!!!!" << LOG_ENDL;
        }
        m_Reader = NULL;
        m_iAPReaderThread = NULL;
    }

    if(m_iAPWriterThread)
    {
        m_iAPWriterThread->wait(3000);  // added by sungha.choi 2013.04.01
        if(!m_iAPWriterThread->isFinished())
        {
            m_Detector->piPodLog->WriteLog("m_iAPWriterThread is not finished !!!!!!!!!!!!!!");
            //LOG_TRACE << "   m_iAPWriterThread terminate Critical Issue !!!!!!!!!!!!!!!!" << LOG_ENDL;
        }
        m_Writer = NULL;
        m_iAPWriterThread = NULL;
    }

    if(m_Identifier)
    {
        if(m_Identifier->isReadyToDelete() == true)   // added by sungha.choi 2013.04.12
        {
            delete m_Identifier;
            m_Identifier = NULL;
        }
        else
        {
            //LOG_HIGH << "CIpodController::destroyTaskHandler1 identifier" << LOG_ENDL;
            bisAllDeleted = 0;
        }
    }

    if(m_Authorisor)
    {
        delete m_Authorisor;
        m_Authorisor = NULL;
    }

    if(m_PlayEngineTaskHandler)
    {
        if(m_PlayEngineTaskHandler->isReadyToDelete() == true)   // added by sungha.choi 2013.04.12
        {
            m_Detector->piPodLog->WriteLog("CIpodController::m_PlayEngineTaskHandler() delete before");
            delete m_PlayEngineTaskHandler;
            m_PlayEngineTaskHandler = NULL;
        }
        else
        {
            //LOG_HIGH << "CIpodController::destroyTaskHandler1 playengine" << LOG_ENDL;
            bisAllDeleted = 0;
        }
    }

    if(m_iOSAppTaskHandler)
    {
        if(m_iOSAppTaskHandler->isReadyToDelete() == true)   // added by sungha.choi 2013.04.12
        {
            delete m_iOSAppTaskHandler;
            m_iOSAppTaskHandler = NULL;
        }
        else
        {
            //LOG_HIGH << "CIpodController::destroyTaskHandler1 iosapp" << LOG_ENDL;
            bisAllDeleted = 0;
        }
    }

    if(m_RemoteUiTaskHandler)
    {
        delete m_RemoteUiTaskHandler;
        m_RemoteUiTaskHandler = NULL;
    }

    if(m_ITunesTaskHandler)
    {
        if(m_ITunesTaskHandler->isReadyToDelete() == true)   // added by sungha.choi 2013.04.12
        {
            delete m_ITunesTaskHandler;
            m_ITunesTaskHandler = NULL;
        }
        else
        {
            //LOG_HIGH << "CIpodController::destroyTaskHandler1 itunes" << LOG_ENDL;
            bisAllDeleted = 0;
        }
    }

    if(m_iTunesTagTaskHandler)
    {
        if(m_iTunesTagTaskHandler->isReadyToDelete() == true)   // added by sungha.choi 2013.04.12
        {
            delete m_iTunesTagTaskHandler;
            m_iTunesTagTaskHandler = NULL;
        }
        else
        {
            //LOG_HIGH << "CIpodController::destroyTaskHandler1 itunetag" << LOG_ENDL;
            bisAllDeleted = 0;
        }
    }

    //LOG_HIGH << "CIpodController::destroyTaskHandler1 bisAllDeleted: " << bisAllDeleted << LOG_ENDL;
    if(bisAllDeleted == 1)
    {
        //LOG_HIGH << "CIpodController::destroyTaskHandler1 it is one" << LOG_ENDL;
        if( m_PrivateData )
        {
            if(!m_PrivateData->m_restartUcsFlag)
            {
                LOG_HIGH << "destroyTaskHandler() bisAllDeleted = 1, emit IPODDisConnected()" << LOG_ENDL;
                emit IPODDisConnected( m_PrivateData->getCurrentSerialNumber(),m_PrivateData->getiPodPortNum());
            }
            else
            {
                LOG_HIGH << "destroyTaskHandler() bisAllDeleted = 1, Don't emit IPODDisConnected()" << LOG_ENDL;
                //LOG_HIGH << "CIpodController::destroyTaskHandler() m_restartUcsFlag == true, m_PrivateData : IPODDisConnected signal don't sent " << LOG_ENDL;
            }
        }
    }
    //LOG_TRACE << "CIpodController::destroyTaskHandler() OUT" << LOG_ENDL;
    return bisAllDeleted;
}

#ifdef CONNECT_BOOTUP
void CIpodController::IPodConnectStateForBootupSlot( bool bConnectState )
{
    //LOG_TRACE << "CIpodController::IPodConnectStateForBootupSlot() IN, bConnectState = " << bConnectState << LOG_ENDL;

    switch ( bConnectState )
    {
        case true :
            m_DeviceState = eConnected;
            m_IsSendedFirstConnectionSignal = true;
            break;
        case false :
            m_DeviceState = eDisconnected;
            break;
        default :
            break;
    }

    LOG_HIGH << "CIpodController::IPodConnectStateForBootupSlot() Connect State (0:Disconnect,1:Connect) = " << m_DeviceState << LOG_ENDL;
    //LOG_TRACE << "CIpodController::IPodConnectStateForBootupSlot() OUT" << LOG_ENDL;
}
#endif

void CIpodController::OnDBIndexingStatusChanged(bool pause)
{
    //LOG_TRACE << "PLAYLIST: CIpodController::OnDBIndexingStatusChanged() IN, pause = " << pause << LOG_ENDL;
    int waitingCode = -1;
    if(!pause)
        waitingCode = ExitCode_ResumeIndexingStatus;
    else
        waitingCode = ExitCode_PauseIndexingStatus;

    //LOG_HIGH << "PLAYLIST:  CIpodController::OnDBIndexingStatusChanged() emit mainLoopQuit(), waitingCode = " << waitingCode << LOG_ENDL;
    emit mainLoopQuit(waitingCode);

    //LOG_TRACE << "PLAYLIST: CIpodController::OnDBIndexingStatusChanged() OUT" << LOG_ENDL;
}

void CIpodController::changeDBIndexingStatus(bool pause)
{
    //LOG_TRACE << "PLAYLIST: CIpodController::changeDBIndexingStatus() IN, pause = " << pause << LOG_ENDL;
    try
    {
        if( m_ControllerState == eStateiPodReady || m_PrivateData->m_IsDBIndexingPaused == pause ||
                m_PrivateData->m_IsDBIndexingFinished)
            return;

        bool ret =  connect(m_iPodDBSyncThread, SIGNAL(OnDBIndexingStatusChanged(bool)),  this,               SLOT(OnDBIndexingStatusChanged(bool)));
        bool ret1 = connect(this,               SIGNAL(emitDBIndexingChangeStatus(bool)), m_iPodDBSyncThread, SIGNAL(OnChangeDBIndexingStatus(bool)));

        //LOG_TRACE << "PLAYLIST:   connect ret = " << ret << ",  ret1 = " << ret1 << LOG_ENDL;

        //LOG_TRACE << "PLAYLIST:   emit emitDBIndexingChangeStatus(pause = " << pause << ")" << LOG_ENDL;
        emit emitDBIndexingChangeStatus(pause);

        int waitingCode = -1;
        if(!pause)
            waitingCode = ExitCode_ResumeIndexingStatus;
        else
            waitingCode = ExitCode_PauseIndexingStatus;

        if( m_PlayEngineTaskHandler )
        {
            //LOG_TRACE<< "PLAYLIST:   start waiting for playengine TH " << LOG_ENDL;
            connect(this, SIGNAL(mainLoopQuit(int)), m_PlayEngineTaskHandler, SIGNAL(mainLoopQuit(int)));
            if(m_ControllerState != eStateiPodReady)
                m_PlayEngineTaskHandler->waitForResponceFromExitCode(5000, waitingCode);
            disconnect(this, SIGNAL(mainLoopQuit(int)), m_PlayEngineTaskHandler, SIGNAL(mainLoopQuit(int)));
            //LOG_TRACE<< "PLAYLIST:   end waiting for playengine TH " << LOG_ENDL;
        }

        //LOG_TRACE<< "PLAYLIST:   changeDBIndexingStatus()  returning" << LOG_ENDL;
        disconnect(m_iPodDBSyncThread, SIGNAL(OnDBIndexingStatusChanged(bool)), this, SLOT(OnDBIndexingStatusChanged(bool)));
        disconnect(this, SIGNAL(emitDBIndexingChangeStatus(bool)), m_iPodDBSyncThread, SIGNAL(OnChangeDBIndexingStatus(bool)));
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE<<"PLAYLIST:   got exception while pausing the indexing" << LOG_ENDL;
        return;
    }
    //LOG_TRACE << "PLAYLIST: CIpodController::changeDBIndexingStatus() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::GetInstance                    CIpodController* (OUT), (void) (IN)
//
// Explanation
// Funtion to get the instance of the class
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CIpodController* CIpodController::GetInstance(void)
{
    if(m_pInstance == NULL)
        m_pInstance = new CIpodController();
    return m_pInstance;;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::GetIpodDBTracksInfo                    
//
// Explanation
// TEMPORARY Funtion to get the list of tracks. WILL BE REMOVED ONCE TRACKER FIX IS AVAILABLE
// History
// 15-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
SAppleInfoList CIpodController::GetIpodDBTracksInfo()
{
    //LOG_TRACE << "CIpodController::GetIpodDBTracksInfo() IN" << LOG_ENDL;

    QList<SAppleTrackInfo> tracksInfo;
    SAppleInfoList listInfo = { 0 , "" , tracksInfo };
    //LOG_TRACE << "CIpodController::GetIpodDBTracksInfo() OUT" << LOG_ENDL;
    return listInfo;
}

int CIpodController::GetTotalNumberOfTracksOnAutoPlay()
{
    //LOG_TRACE << "CIpodController::GetTotalNumberOfTracksOnAutoPlay() IN, m_ControllerState = " << GetControllerStateString() << LOG_ENDL;

    int dTotal = 0;

    if ( (m_ControllerState == eStateAutoPlayReady) || (m_ControllerState == eStateiPodReady) )
    {
        if (m_PlayEngineTaskHandler)
        {
            dTotal = m_PlayEngineTaskHandler->getTotalNumberOfTracks();
        }
    }
    LOG_HIGH << "CIpodController::GetTotalNumberOfTracksOnAutoPlay(), Total Song Number = " << dTotal << LOG_ENDL;
    //LOG_TRACE << "CIpodController::GetTotalNumberOfTracksOnAutoPlay() Exit " << LOG_ENDL;

    return dTotal;
}

void CIpodController::SetIsAhaPandoraMode(bool isAhaPandoraMode)
{
    m_PrivateData->m_bIsPreviousAhaPandoraMode = m_PrivateData->m_bIsAhaPandoraMode;
    if(m_PrivateData->m_bIsAhaPandoraMode != isAhaPandoraMode)
    {
        LOG_HIGH << "m_bIsAhaPandoraMode: " << isAhaPandoraMode << LOG_ENDL;
        m_PrivateData->m_bIsAhaPandoraMode = isAhaPandoraMode;
    }
}

void CIpodController::PlayiPodFiles()
{
    LOG_HIGH<< "PlayiPodFiles "<<LOG_ENDL;
    m_PrivateData->m_bBreakNextLoop = true;

    CIpodControllerQueueData tempData;
    QueuingEvent(EPlayiPodFilesEvent, tempData);
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::PlayiPodFilesFromQueue()
{
    try
    {
        LOG_HIGH<< "PlayiPodFilesFromQueue"<<LOG_ENDL;
        m_PrivateData->m_bBreakNextLoop = false;
        if( m_PlayEngineTaskHandler )
        {
            m_PlayEngineTaskHandler->PlayiPodFiles();
        }
    }
    catch(EIPodErrors ex)
    {
        return;
    }
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::Play                    void (OUT), (void) (IN)
//
// Explanation
// Funtion to start the playback from any state
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
// 03-Dec-2011      Sowmini Philip                     Added incorrect state API call. Play() call when controller is in incorrect PLAYING state
*****************************************************************************/
void CIpodController::Play()
{
    LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState =" << GetPlayStateString(m_PlayState) << ", m_IsTabOff = " << m_PrivateData->m_IsTabOff << LOG_ENDL;

    if(m_PrivateData->m_IsTabOff == true)
    {
        m_bDelayNextPrev = false;

        //LOG_TRACE << "IOSAPP::CIpodController::Play(), m_IsTabOff == true, m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;

        MediaInfo info = m_PrivateData->getCurrentMediaInfo();
        if(info.Title!= " " || info.Artist!= " " || info.Album != " ")
        {
            OnCurrentMediaInfoSent(&info);
        }

        if(m_iOSAppTaskHandler)
        {
            m_iOSAppTaskHandler->resetCurrentBasicControlStateVariable();
        }


        m_PrivateData->m_IsTabOff = false;

        int ipodActualMode;
        if(m_PlayEngineTaskHandler)
        {
            switch(m_PlayEngineTaskHandler->m_RandomMode)
            {
            case RANDOM_FILE:
            case RANDOM_FOLDER:
                ipodActualMode = RANDOMFILE;
                break;
            case RANDOM_OFF:
                ipodActualMode = RANDOMOFF;
                break;
            }
            emit OnModeChanged(ipodActualMode);
        }
    }

    CIpodControllerQueueData tempData;
    QueuingEvent(EPlayEvent, tempData);
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::PlayFromQueue()
{

    bool bStartAutoPlayWasCalled = false;
    if( m_PrivateData == NULL ) {
        LOG_HIGH << "   m_PrivateData is NULL, so return!!!!!!!!!!!!" << LOG_ENDL;
        return;
    }

	m_PrivateData->m_bBreakNextLoop = false;
    

    bool result         = false;

    try
    {
        if(m_PrivateData->m_bIsPreviousAhaPandoraMode)
        {
            LOG_HIGH<<"m_bIsPreviousAhaPandoraMode delay"<<LOG_ENDL;
            //delay the play safely!!!
            usleep(500000);
        }
        if(m_ControllerState == eStateReadyToAutoPlayReady)
        {
            m_ControllerState = eStateAutoPlayReady;
            //Tan, this is silly code, I know!
            //Previously it was m_PlayState = m_PlayEngineTaskHandler->startAutoplay(m_iOSAppTaskHandler);
            //I am fixing this bug with minimum change.
            bStartAutoPlayWasCalled = true;
            int nPlayerEnginePlayState = m_PlayEngineTaskHandler->startAutoplay(m_iOSAppTaskHandler);
            m_PlayState = convertPlayerEnginePlayState(nPlayerEnginePlayState);
        }
        if( ((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady)) )
        {
            if(m_PlayEngineTaskHandler)
            {
                bool isNowPlayingMP = m_iOSAppTaskHandler->isNowPlayingApplicationIsMPE();
                LOGH ", m_PrivateData->m_IsTabOff = " << m_PrivateData->m_IsTabOff << ", isNowPlayingMP = " << isNowPlayingMP << LOG_ENDL;

                if( (m_PlayState == SEEKING_FF) || (m_PlayState == SEEKING_REWIND) )
                {
                    result = m_PlayEngineTaskHandler->playControlE("StopSeek");
                }
                else if((m_PlayState == PAUSED) )
                {
                    if (!isNowPlayingMP)
                    {
                        if(bStartAutoPlayWasCalled == false)
                        {
                            if( m_iOSAppTaskHandler )
                                result = m_iOSAppTaskHandler->RestartPlay();
                        }
                        else
                        {
                            result = true;
                        }
                    }
                    else
                    {
                        int nState =  m_PlayEngineTaskHandler->m_playState; //this is the latest state of IPod
                        if(m_PlayEngineTaskHandler->IsPlaylistSpecificCategoryIsSet())
                        {
                            result = m_PlayEngineTaskHandler->playE();
                        }
                        else if(0x02 == nState)
                        {
                            result = m_PlayEngineTaskHandler->playControlE("Play");
                        }
                        else
                        {
                            result = true;
                        }
                        if(m_PrivateData->m_IsConvertedFromUIDToIndex == false)
                        {
                            MediaInfo mediaInfo;
                            m_PlayEngineTaskHandler->emitCurrentMediaInfoE(&mediaInfo);
                        }
                    }
                }
                else  if( (m_PlayState == STOPPED) )
                {
                    int nState =  m_PlayEngineTaskHandler->m_playState; //this is the latest state of IPod
                    if (nState == 0x00 && !isNowPlayingMP)
                    {
                        if(bStartAutoPlayWasCalled == false)
                        {
                            if( m_iOSAppTaskHandler )
                                result = m_iOSAppTaskHandler->RestartPlay();
                        }
                        else
                        {
                            result = true;
                        }
                    }
                    else
                    {
                        result = m_PlayEngineTaskHandler->playE();
                    }
                }
                else if(m_PlayState == PLAYING)
                {
                    int nExpectedState = 2; //PAUSeD state value from devices
                    int nState =  m_PlayEngineTaskHandler->m_playState; //this is the latest state of IPod

                    if (!isNowPlayingMP)
                    {
                        if(bStartAutoPlayWasCalled == false)
                        {
                            if( m_iOSAppTaskHandler )
                                result = m_iOSAppTaskHandler->RestartPlay();
                        }
                        else
                        {
                            result = true;
                        }
                    }
                    else
                    {
                        if(nState != PLAYING)
                        {
                            result = m_PlayEngineTaskHandler->playControlE("Play");
                            if(m_PrivateData->m_IsConvertedFromUIDToIndex == false)
                            {
                                MediaInfo mediaInfo;
                                m_PlayEngineTaskHandler->emitCurrentMediaInfoE(&mediaInfo);
                            }
                        }
                        else
                        {
                            result = true;
                        }
                    }
                }
            }

            if(result)
            {
                m_PrevPlayState = m_PlayState;
                m_PlayState = PLAYING;
                emit OnStateChanged(PLAYER_PLAY);
                if(m_IsIDPS == false )
                {
                    m_PlayEngineTaskHandler->emitStateChangeForNonIDPS();
                }
            }
            updateDigitalAudioPipeline(PLAYER_PLAY);
        }
    }
    catch(EIPodErrors ex)
    {
        return;
    }
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::Pause                    void (OUT), (void) (IN)
//
// Explanation
// Funtion to pause the playback from play state
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
// 03-Dec-2011      Sowmini Philip                     Added incorrect state API call. Pause() call when controller is in incorrect PAUSED state
*****************************************************************************/
void CIpodController::Pause()
{
    //LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState =" << GetPlayStateString(m_PlayState) << LOG_ENDL;

    /*
    if(m_PrivateData->m_bIsAhaPandoraMode)
    {
        return;
    }
    */

    CIpodControllerQueueData tempData;
    QueuingEvent(EPauseEvent, tempData);
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::PauseFromQueue()
{
    LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;

    bool result       = false;
    try
    {
        if( ((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady)) )
        {
            //LOG_TRACE << "   m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
 
            if( m_PlayEngineTaskHandler )
            {
                if( (m_PlayState == SEEKING_FF) || (m_PlayState == SEEKING_REWIND) )
                {
                    result = m_PlayEngineTaskHandler->playControlE("StopSeek");
                    result = false; //resetting
                    result = m_PlayEngineTaskHandler->playControlE("Pause");
                }
                else if(m_PlayState == PLAYING)
                    result = m_PlayEngineTaskHandler->playControlE("Pause");
                else if(m_PlayState == PAUSED || m_PlayState == STOPPED)
                {
                    int nExpectedState = 1;
                    int nState = m_PlayEngineTaskHandler->m_playState; //this is the latest state of IPod
                    //LOG_TRACE << "   Index base expected state = " << nExpectedState << ", Current state = " <<  nState << LOG_ENDL;
                    if(nState == nExpectedState) //PLAYING STATE = 1 from  device
                        result = m_PlayEngineTaskHandler->playControlE("Pause");
                    else
                        result = true;
                }
            }

            if(result == true && m_PrivateData->m_IsTabOff == false )
            {
                m_PrevPlayState = m_PlayState;
                m_PlayState     = PAUSED;
                //LOG_TRACE << "   m_IsTabOff = false, m_PrevPlayState = " << GetPlayStateString(m_PrevPlayState)  << ", m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;

                if(m_IsIDPS == false )
                {
                    m_PlayEngineTaskHandler->emitStateChangeForNonIDPS();
                }
                OnStateChangedSlot(0x0B);
            }
            else if( m_PrivateData->m_IsTabOff == true )
            {
                m_PrevPlayState = m_PlayState;
                m_PlayState     = PAUSED;
                //LOG_TRACE << "   m_IsTabOff = true, m_PrevPlayState = " << GetPlayStateString(m_PrevPlayState)  << ", m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
                OnStateChangedSlot(0x0B);
            }
            else
            {
                //LOG_TRACE << "   Pause failed" << LOG_ENDL;
            }
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::Pause() ... Execption" << LOG_ENDL;
        return;
    }
    //LOG_TRACE << "CIpodController::Pause() OUT" << LOG_ENDL;
}

void CIpodController::FunctionStop()                          
{
    //LOG_TRACE << "CIpodController::FunctionStop is not applicable " << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::Stop                    void (OUT), (void) (IN)
//
// Explanation
// Funtion to stop the playback from play or pause state
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::Stop()
{
    //LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState =" << GetPlayStateString(m_PlayState) << LOG_ENDL;
    CIpodControllerQueueData tempData;
    QueuingEvent(EStopEvent, tempData);
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::StopFromQueue()
{
    LOGH " m_ControllerState = " << GetControllerStateString()<< ", m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;

    bool result       = false;
    try
    {
        if( ((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady)))
        {
            if(m_PlayEngineTaskHandler)
            {
                if( (m_PlayState == SEEKING_FF) || (m_PlayState == SEEKING_REWIND) )
                {
                    result = m_PlayEngineTaskHandler->playControlE("StopSeek");
                    if(result)
                    {
                        m_PlayState = PLAYING;
                        //LOG_TRACE << "   Play current selection succeeded, m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
                    }
                    //INFO : Stop Seek will cause normal play
                }
            }
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::Stop() ... Execption " << LOG_ENDL;
        return;
    }
    //LOG_TRACE << "CIpodController::Stop() OUT" << LOG_ENDL;
}

void CIpodController::EmitScanComplete()
{
    //LOG_TRACE << "CIpodController::EmitScanComplete() IN" << LOG_ENDL;
    try{
        if((m_scanMode == SCAN_FILE) || (m_scanMode == SCAN_FOLDER))
        {
            if( m_PlayEngineTaskHandler )
                m_PlayEngineTaskHandler->setScanModeE(SCAN_OFF);

            m_scanMode = SCAN_OFF;
            m_nScanStartPosition = -1;
            emit OnScanCompleted();

            LOG_HIGH << "CIpodController::EmitScanComplete(), m_scanMode = SCAN_OFF" << "emit OnScanCompleted()__"  << LOG_ENDL;
        }
    }
    catch(EIPodErrors ex)
    {
        return;
    }
    //LOG_TRACE << "CIpodController::EmitScanComplete() OUT" << LOG_ENDL;
}

void CIpodController::setTrackBufferForOSD( int isPrev ) //prev is 1
{
    if( m_PlayEngineTaskHandler && m_PrivateData->m_IsTabOff == false)
    {
        emit OnTrackBuffered( isPrev,//Next
                              m_PlayEngineTaskHandler->m_CurrentIndex,
                              m_PlayEngineTaskHandler->m_CurrentIndex,
                              m_PlayEngineTaskHandler->m_NumberOfPlayingTracks
                              );
        //LOG_TRACE << "   emit OnTrackBuffered(0) Signal send Next " << m_PlayEngineTaskHandler->m_CurrentIndex << ", "
                  //<< m_PlayEngineTaskHandler->m_CurrentIndex << ", " <<
                  //   m_PlayEngineTaskHandler->m_NumberOfPlayingTracks << LOG_ENDL;
    }
    else
    {
        //LOG_TRACE << "Tab off do not send track buffer" << LOG_ENDL;
    }
}

void CIpodController::disableTuneForFewMiliSecond(int nMiliSecond)
{
    m_disableTuneOnSetIndexTimer->start(nMiliSecond);
}

void CIpodController::delayNextPrevExecution(int nDelay)
{
    m_bDelayNextPrev = true;
    m_delayNextPrevTimer->start(nDelay);//single shot, with interval
}

void CIpodController::delayNextPrevExecutionSlot()
{
    m_bDelayNextPrev = false;
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::delayResetOSDKey(int nDelay)
{
    m_resetOSDKeyTimer->start(nDelay);
}

void CIpodController::ResetOSDKeyTimeOutSlot()
{
    //LOG_TRACE<<"TanDebug ResetOSDKeyTimeOutSlot"<<LOG_ENDL;
    m_PrivateData->m_OSDKey = OSDKey_None;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::Next                    void (OUT), (int) (IN)
//
// Explanation
// Funtion to play the next song
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::Next(int /*nSkipLevel*/)
{
    //when next is received, start a timer to delay Next event execution.
    if(m_PlayEngineTaskHandler == NULL)
        return;

    LOGH ", m_CurrentIndex = " << m_PlayEngineTaskHandler->m_CurrentIndex << ", m_CurrentIndexToPlay = " << m_PlayEngineTaskHandler->m_CurrentIndexToPlay << ", m_NextLevel = " << m_PrivateData->m_QueueData.m_NextLevel<< LOG_ENDL;

    if(m_PrivateData->m_QueueData.m_NextLevel != 0 || m_PlayEngineTaskHandler->isCurrentTrackHasChapter() == false)
    {
        if(m_PrivateData->m_QueueData.m_NextLevel <= 0)
            delayNextPrevExecution(500);
        else
            delayNextPrevExecution(1000);
    }

    CIpodControllerQueueData tempData;
    QueuingEvent(ENextEvent, tempData);
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::NextFromQueue(int /*nSkipLevel*/)
{
    //LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
    LOGH ", m_CurrentIndex = " << m_PlayEngineTaskHandler->m_CurrentIndex << ", m_CurrentIndexToPlay = " << m_PlayEngineTaskHandler->m_CurrentIndexToPlay << ", m_NextLevel = " << m_PrivateData->m_QueueData.m_NextLevel<< LOG_ENDL;
    EmitScanComplete();

    bool result    = false;
    // isGetPlayStatusEmitted is added to keep check that Accessory sends GetPlayStatus command only once.
    bool isGetPlayStatusEmitted = false;

    try
    {
        if( ((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady)) )
        {
            //LOG_TRACE << "CIpodController::Next() IN level = " << m_PrivateData->m_QueueData.m_NextLevel << LOG_ENDL;
            int nextLevel = 0;
            nextLevel = m_PrivateData->m_QueueData.m_NextLevel;
            if(nextLevel <= 0)//when next, next level must not negative
            {
                nextLevel = 1;
            }

            m_resetOSDKeyTimer->stop();
            m_PrivateData->m_OSDKey = OSDKey_Next;

            m_PrivateData->m_QueueData.m_NextLevel = 0;

            if(m_PlayEngineTaskHandler)
            {
                if( (m_PlayState == SEEKING_FF) || (m_PlayState == SEEKING_REWIND) )
                {
                    result = m_PlayEngineTaskHandler->playControlE("StopSeek");
                    if(result)
                    {
                        m_PlayState = PLAYING;
                        //LOG_TRACE << "   m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
                    }
                }

                if(m_PlayState != STOPPED)
                {
                    if(m_PlayEngineTaskHandler->isPlayListSet())
                    {
                        m_PlayEngineTaskHandler->m_JumpInProgress = true;
                        int  dNext = 0;
                        bool executePlay = false;

                        while(nextLevel)
                        {
                            nextLevel--;
                            dNext = m_PlayEngineTaskHandler->getNextIndex();
                            executePlay = m_PlayEngineTaskHandler->setCurrentIndexE(dNext);
                        }

                        if(executePlay)
                        {
                            //code change to solve the meta data mismatch
                            // for multiple next case, the Play() is removed as i goes into queue
                            // and due to that the playlist index may change and we may
                            //not find the correct song to play
                            int count = 0;
                            //LOG_TRACE << "   m_PlayEngineTaskHandler->playE()" << LOG_ENDL;
                            while(count < 5 && !result)
                            {
                                result = m_PlayEngineTaskHandler->playE();
                                count++;
                            }
                            isGetPlayStatusEmitted = true;
                            //LOGH ", Play m_CurrentIndex = " << m_PlayEngineTaskHandler->m_CurrentIndex << ", m_CurrentIndexToPlay = " << m_PlayEngineTaskHandler->m_CurrentIndexToPlay <<LOG_ENDL;
                        }
                    } // if(m_PlayEngineTaskHandler->isPlayListSet()) {
                    else
                    {
                        int  dNext = 0;
                        bool executePlay = false;
                        m_PlayEngineTaskHandler->m_CurrentIndexToPlay =  m_PlayEngineTaskHandler->m_CurrentIndex;

                        //Next/Prev for iOS App
                        if(m_iOSAppTaskHandler)
                        {
                            LOGH " m_ApplicationBundleName1 = " << m_iOSAppTaskHandler->m_ApplicationBundleName1 << LOG_ENDL;
                        }
                        else
                        {
                            LOGH " m_iOSAppTaskHandler is NULL!!" << LOG_ENDL;
                            return;
                        }

//                        if( m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_APP_ID, Qt::CaseInsensitive) &&
//                                !(m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MUSICAPP_APP_ID, Qt::CaseInsensitive) ||
//                                  m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MOBILEAPP_APP_ID, Qt::CaseInsensitive)))
                        if((m_PlayEngineTaskHandler->isCurrentTrackHasChapter() == true) ||
                                !(m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MUSICAPP_APP_ID, Qt::CaseInsensitive) ||
                                  m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MOBILEAPP_APP_ID, Qt::CaseInsensitive)))
                        {
                            while(nextLevel)
                            {
                                if(m_productId == NANO_1G_PRODUCT_ID || m_productId == NANO_2G_PRODUCT_ID)
                                {
                                    result = m_PlayEngineTaskHandler->playControlE("Next", true);
                                }
                                else
                                {
                                    LOGH " playControlE(Next) nextLevel = "<< nextLevel << LOG_ENDL;
                                    result = m_PlayEngineTaskHandler->playControlE("Next");
                                }
                                m_PlayEngineTaskHandler->m_CurrentIndexToPlay++;
                                nextLevel--;
                                if(m_PrivateData->m_bBreakNextLoop)
                                {
                                    m_PrivateData->m_bBreakNextLoop = false;
                                    break;
                                }
                                usleep(100000);
                            }
                        }
                        else
                        {
                            if(nextLevel == 1)
                            {
                                if(m_productId == NANO_1G_PRODUCT_ID || m_productId == NANO_2G_PRODUCT_ID)
                                {
                                    result = m_PlayEngineTaskHandler->playControlE("Next", true);
                                }
                                else
                                {
                                    result = m_PlayEngineTaskHandler->playControlE("Next");
                                }
                                m_PlayEngineTaskHandler->m_CurrentIndexToPlay++;
                                nextLevel--;
                            }
                            while(nextLevel)
                            {
                                nextLevel--;
                                dNext = m_PlayEngineTaskHandler->getNextIndexForConvertedDevice(m_PlayEngineTaskHandler->m_CurrentIndexToPlay);
                                m_PlayEngineTaskHandler->m_CurrentIndexToPlay ++;
                                executePlay = m_PlayEngineTaskHandler->setCurrentIndexE(dNext);
                            }

                            if(executePlay)
                            {
                            //LOG_TRACE << "   m_PlayEngineTaskHandler->playE()" << LOG_ENDL;
                                result = m_PlayEngineTaskHandler->playE();
                                isGetPlayStatusEmitted = true;
                                m_PlayEngineTaskHandler->m_CurrentIndex =  m_PlayEngineTaskHandler->m_CurrentIndexToPlay;
                            //LOGH ", Play m_CurrentIndex = " << m_PlayEngineTaskHandler->m_CurrentIndex << ", m_CurrentIndexToPlay = " << m_PlayEngineTaskHandler->m_CurrentIndexToPlay <<LOG_ENDL;
                            }
                        }
                        if(m_PlayState == PAUSED)
                        {
                            //LOG_TRACE << "   m_PlayState = " << GetPlayStateString(m_PlayState) << ", call Play()"  << LOG_ENDL;
                            Play();
                        }
                    }
                } // if(m_PlayState != STOPPED) {
            }

            if(result && !isGetPlayStatusEmitted)
            {
                if(m_IsIDPS == false){
                        m_PlayEngineTaskHandler->emitStateChangeForNonIDPS();
                }
            }
            else if(!result)
            {
                //LOG_TRACE << "   Play Next() failed_"  << LOG_ENDL;
            }
            delayNextPrevExecution(200);
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::Next() ... Execption" << LOG_ENDL;
        return;
    }

    //LOG_TRACE << "CIpodController::Next() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::Next                    void (OUT), (int) (IN)
//
// Explanation
// Funtion to play the previous song
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::Prev(int /*nSkipLevel*/,int forceFlag)
{
    //when next is received, start a timer to delay Next event execution.

    if(m_PlayEngineTaskHandler == NULL)
        return;


    LOGH ", m_CurrentIndex = " << m_PlayEngineTaskHandler->m_CurrentIndex << ", m_CurrentIndexToPlay = " << m_PlayEngineTaskHandler->m_CurrentIndexToPlay << ", m_NextLevel = " << m_PrivateData->m_QueueData.m_NextLevel << ", forceFlag = " << forceFlag << LOG_ENDL;

    if(m_PrivateData->m_QueueData.m_NextLevel != 0 || m_PlayEngineTaskHandler->isCurrentTrackHasChapter() == false)
    {
        if(m_PrivateData->m_QueueData.m_NextLevel >= 0)
            delayNextPrevExecution(500);
        else
            delayNextPrevExecution(1000);
    }

    CIpodControllerQueueData tempData(forceFlag);
    QueuingEvent(EPrevEvent, tempData);
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::PrevFromQueue(int /*nSkipLevel*/,int forceFlag)
{
    //LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState =" << GetPlayStateString(m_PlayState) << LOG_ENDL;
    //LOGH ", m_NextLevel = " << m_PrivateData->m_QueueData.m_NextLevel << ", forceFlag = " << forceFlag << LOG_ENDL;

    LOGH ", m_CurrentIndex = " << m_PlayEngineTaskHandler->m_CurrentIndex << ", m_CurrentIndexToPlay = " << m_PlayEngineTaskHandler->m_CurrentIndexToPlay << ", m_NextLevel = " << m_PrivateData->m_QueueData.m_NextLevel << ", forceFlag = " << forceFlag << LOG_ENDL;

    EmitScanComplete();
    bool result     = false;

    // isGetPlayStatusEmitted is added to keep check that Accessory sends GetPlayStatus command only once.
    bool isGetPlayStatusEmitted = false;
    try
    {
        if( ((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady)) )
        {
            //LOG_TRACE << "CIpodController::Prev() IN execution : level = " << m_PrivateData->m_QueueData.m_NextLevel << LOG_ENDL;
            int nextLevel = 0;
            nextLevel = m_PrivateData->m_QueueData.m_NextLevel;
            if(nextLevel >= 0) //when previous, next level must not be positive.
            {
                nextLevel = -1;
            }

            m_resetOSDKeyTimer->stop();
            m_PrivateData->m_OSDKey = OSDKey_Prev;

            m_PrivateData->m_QueueData.m_NextLevel = 0;

            if(m_PlayEngineTaskHandler )
            {
                if( (m_PlayState == SEEKING_FF) || (m_PlayState == SEEKING_REWIND) )
                {
                    result = m_PlayEngineTaskHandler->playControlE("StopSeek");
                    if( result )
                    {
                        m_PlayState = PLAYING;
                        //LOG_TRACE << "   Play current selection succeeded, m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
                    }
                }
                //LOG_TRACE << "   m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
                if( m_PlayState != STOPPED )
                {
                    if(m_PlayEngineTaskHandler->isPlayListSet() == true )
                    {
                        m_IsPrev = true;
                        int dCurrent = m_PlayEngineTaskHandler->getCurrentIndex();
                        int position = m_PlayEngineTaskHandler->getCurrentMediaPosition()/1000;
                        //LOG_TRACE << "   position = " << position << LOG_ENDL;
                        bool needJump = (nextLevel < -1) ? true : false;

                        if( position <= 3 || needJump || forceFlag)
                        {
                            m_PlayEngineTaskHandler->m_JumpInProgress = true;
                            //LOG_TRACE << "   prev() case: jumping for < 4" << LOG_ENDL;
                            m_PlayEngineTaskHandler->resetCurrentMediaPositionVariable();
                            bool needplay = false;

                            while(nextLevel)
                            {
                                nextLevel++;
                                int dPrev = m_PlayEngineTaskHandler->getPrevIndex();
                                needplay = m_PlayEngineTaskHandler->setCurrentIndexE(dPrev);
                                //LOG_TRACE << "   prev setting index = " << dPrev << LOG_ENDL;
                            }

                            if(needplay)
                            {
                                //LOG_TRACE << "   prev start play by needplay flag, needplay = " << needplay << LOG_ENDL;
                                m_PlayState = STOPPED;
                                Play();
                                isGetPlayStatusEmitted = true;
                                result = true;
                            }
                            m_PlayEngineTaskHandler->m_JumpInProgress = false;
                        }
                        else
                        {
                            /*On pressing prev in Podcast, it should start playing from
                            begining when the pos > 3 */
                            QString fileName;
                            fileName.append(m_PlayEngineTaskHandler->getCurrentFileName());
                            ////LOG_TRACE << "File name:" << fileName << LOG_ENDL;
                            if(fileName.contains(gIPODPodcastType) || fileName.contains(gIPODAudioBookType) || fileName.contains(gIPODiTunesUType))
                            {
                                //LOG_TRACE << "   prev() case: Podcast/AudioBook/iTunes" << LOG_ENDL;
                                //Playcontrol command 09 for previous is not working for nano1G.
                                //So sending 04 deprecated command to fix the issue.
                                m_PlayEngineTaskHandler->resetCurrentMediaPositionVariable();
                                if(m_productId == NANO_1G_PRODUCT_ID)
                                {
                                    result = m_PlayEngineTaskHandler->playControlE("Previous", true);
                                }
                                else
                                {
                                    result = m_PlayEngineTaskHandler->playControlE("Previous");
                                }
                            }
                            else
                            {
                                //LOG_TRACE << "   prev() case: normal" << LOG_ENDL;
                                SetCurrentIndex(dCurrent);
                                /*
                                IPod Nano 2G, if the position is greater than 3, the track time offset
                                is not reset to 0. PlayStatusChangeNotiifcation command return the track
                                time offset as the current position.
                                So, Stop command is issued for IPod Nano 2G and start play the same song
                                from the beginning.
                                */
                                /* Added same procedure as Nano 2G because IPod Nano 1G also shows same working as Nano 2G when select current track again */
                                if((m_productId == NANO_2G_PRODUCT_ID) || (m_productId == NANO_1G_PRODUCT_ID)) {
                                    ////LOG_TRACE << "Stop and start play for nano 2G/1G is needed when we do prev more than 3 sec so do setcurrentposition to zero instead" << LOG_ENDL;
                                    SetCurrentMediaPosition(0);
                                }
                                else
                                {
                                    m_PlayEngineTaskHandler->resetCurrentMediaPositionVariable();
                                    result = m_PlayEngineTaskHandler->playControlE("Previous");
                                }

                                isGetPlayStatusEmitted = true;
                                result = true;
                            }
                        }
                        result = true;
                        m_IsPrev = false;
                    }
                    else
                    {
                        //LOG_TRACE << "CIpodController::Prev : in no playlist case : forceFlag : " << forceFlag << "  nextLevel : " << nextLevel << LOG_ENDL;
                        if(forceFlag == 0 )
                        {
                            int position = m_PlayEngineTaskHandler->getCurrentMediaPosition()/1000;
                            //LOG_TRACE << "   position = " << position << LOG_ENDL;

                            //Next/Prev for iOS App
                            if(m_iOSAppTaskHandler)
                            {
                                LOGH " m_ApplicationBundleName1 = " << m_iOSAppTaskHandler->m_ApplicationBundleName1 << LOG_ENDL;
                            }
                            else
                            {
                                LOGH " m_iOSAppTaskHandler is NULL!!" << LOG_ENDL;
                                return;
                            }

//                            if( m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_APP_ID, Qt::CaseInsensitive) &&
//                                    !(m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MUSICAPP_APP_ID, Qt::CaseInsensitive) ||
//                                      m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MOBILEAPP_APP_ID, Qt::CaseInsensitive)))
                            if((m_PlayEngineTaskHandler->isCurrentTrackHasChapter() == true) ||
                                    !(m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MUSICAPP_APP_ID, Qt::CaseInsensitive) ||
                                      m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MOBILEAPP_APP_ID, Qt::CaseInsensitive)))
                            {
                                while(nextLevel)
                                {
                                    LOGH " playControlE(Previous) nextLevel1 = "<< nextLevel << LOG_ENDL;
                                    if(m_productId == NANO_1G_PRODUCT_ID || m_productId == NANO_2G_PRODUCT_ID)
                                    {
                                        result = m_PlayEngineTaskHandler->playControlE("Previous", true);
                                    }
                                    else
                                    {
                                        result = m_PlayEngineTaskHandler->playControlE("Previous");
                                    }
                                    m_PlayEngineTaskHandler->m_CurrentIndexToPlay --;
                                    nextLevel++;
                                    if(m_PrivateData->m_bBreakNextLoop)
                                    {
                                        m_PrivateData->m_bBreakNextLoop = false;
                                        break;
                                    }
                                    usleep(100000);
                                }
                            }
                            else
                            {
                                if(m_PlayEngineTaskHandler->isCurrentTrackHasChapter())
                                {
                                    if(nextLevel == -1)
                                    {
                                    //LOG_TRACE<<"PrevFromQueue playControl Previous"<<LOG_ENDL;
                                        if(m_productId == NANO_1G_PRODUCT_ID || m_productId == NANO_2G_PRODUCT_ID)
                                        {
                                            result = m_PlayEngineTaskHandler->playControlE("Previous", true);
                                        }
                                        else
                                        {
                                            result = m_PlayEngineTaskHandler->playControlE("Previous");
                                        }
                                        nextLevel++;
                                    }
                                }
                                else if( position > 3 )
                                {
                                //LOG_TRACE<<"PrevFromQueue position > 3"<<LOG_ENDL;
                                    m_PlayEngineTaskHandler->resetCurrentMediaPositionVariable();
                                    if(nextLevel == -1)
                                    {
                                    //LOG_TRACE<<"PrevFromQueue playControl Previous"<<LOG_ENDL;
                                        if(m_productId == NANO_1G_PRODUCT_ID || m_productId == NANO_2G_PRODUCT_ID)
                                        {
                                            result = m_PlayEngineTaskHandler->playControlE("Previous", true);
                                        }
                                        else
                                        {
                                            result = m_PlayEngineTaskHandler->playControlE("Previous");
                                        }
                                    }
                                    nextLevel++;
                                }

                                //after that... do jump for the rest of nextLevel
                                bool needplay = false;
                                m_PlayEngineTaskHandler->m_CurrentIndexToPlay =  m_PlayEngineTaskHandler->m_CurrentIndex;
                                while(nextLevel)
                                {
                                //LOG_TRACE<<"PrevFromQueue setCurrentIndex"<<LOG_ENDL;
                                    nextLevel++;
                                    int dPrev = m_PlayEngineTaskHandler->getPrevIndexForConvertedDevice(m_PlayEngineTaskHandler->m_CurrentIndexToPlay);
                                    m_PlayEngineTaskHandler->m_CurrentIndexToPlay --;
                                    needplay = m_PlayEngineTaskHandler->setCurrentIndexE(dPrev);
                                //LOG_TRACE << "   prev setting index = " << dPrev << LOG_ENDL;
                                }
                                if(needplay)
                                {
                                //LOG_TRACE << "   prev start play by needplay flag, needplay = " << needplay << LOG_ENDL;
                                    m_PlayState = STOPPED;
                                    Play();
                                    isGetPlayStatusEmitted = true;
                                    result = true;
                                    m_PlayEngineTaskHandler->m_CurrentIndex =  m_PlayEngineTaskHandler->m_CurrentIndexToPlay;
                                //LOGH ", Play m_CurrentIndex = " << m_PlayEngineTaskHandler->m_CurrentIndex << ", m_CurrentIndexToPlay = " << m_PlayEngineTaskHandler->m_CurrentIndexToPlay <<LOG_ENDL;
                                }
                            }
                        }
                        else
                        {
                            bool needplay = false;
                            m_PlayEngineTaskHandler->m_CurrentIndexToPlay =  m_PlayEngineTaskHandler->m_CurrentIndex;

                            //Next/Prev for iOS App
                            if(m_iOSAppTaskHandler)
                            {
                                LOGH " m_ApplicationBundleName1 = " << m_iOSAppTaskHandler->m_ApplicationBundleName1 << LOG_ENDL;
                            }
                            else
                            {
                                LOGH " m_iOSAppTaskHandler is NULL!!" << LOG_ENDL;
                                return;
                            }

//                            if( m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_APP_ID, Qt::CaseInsensitive) &&
//                                    !(m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MUSICAPP_APP_ID, Qt::CaseInsensitive) ||
//                                      m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MOBILEAPP_APP_ID, Qt::CaseInsensitive)))
                            if((m_PlayEngineTaskHandler->isCurrentTrackHasChapter() == true) ||
                                    !(m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MUSICAPP_APP_ID, Qt::CaseInsensitive) ||
                                      m_iOSAppTaskHandler->m_ApplicationBundleName1.contains(IPOD_MOBILEAPP_APP_ID, Qt::CaseInsensitive)))
                            {
                                while(nextLevel)
                                {
                                    LOGH " playControlE(Previous) nextLevel1 = "<< nextLevel << LOG_ENDL;
                                    if(m_productId == NANO_1G_PRODUCT_ID || m_productId == NANO_2G_PRODUCT_ID)
                                    {
                                        result = m_PlayEngineTaskHandler->playControlE("Previous", true);
                                    }
                                    else
                                    {
                                        result = m_PlayEngineTaskHandler->playControlE("Previous");
                                    }
                                    m_PlayEngineTaskHandler->m_CurrentIndexToPlay --;
                                    nextLevel++;
                                    if(m_PrivateData->m_bBreakNextLoop)
                                    {
                                        m_PrivateData->m_bBreakNextLoop = false;
                                        break;
                                    }
                                    usleep(100000);
                                }
                            }
                            else
                            {
                                while(nextLevel)
                                {
                                    nextLevel++;
                                    int dPrev = m_PlayEngineTaskHandler->getPrevIndexForConvertedDevice(m_PlayEngineTaskHandler->m_CurrentIndexToPlay);
                                    m_PlayEngineTaskHandler->m_CurrentIndexToPlay --;
                                    needplay = m_PlayEngineTaskHandler->setCurrentIndexE(dPrev);
                                //LOG_TRACE << "   prev setting index = " << dPrev << LOG_ENDL;
                                }
                                if(needplay)
                                {
                                //LOG_TRACE << "   prev start play by needplay flag, needplay = " << needplay << LOG_ENDL;
                                    m_PlayState = STOPPED;
                                    Play();
                                    isGetPlayStatusEmitted = true;
                                    result = true;
                                    m_PlayEngineTaskHandler->m_CurrentIndex =  m_PlayEngineTaskHandler->m_CurrentIndexToPlay;
                                //LOGH ", Play m_CurrentIndex = " << m_PlayEngineTaskHandler->m_CurrentIndex << ", m_CurrentIndexToPlay = " << m_PlayEngineTaskHandler->m_CurrentIndexToPlay <<LOG_ENDL;
                                }
                            }
                        }
                    }
                    if(m_PlayState == PAUSED)
                    {
                        //LOG_TRACE << "   m_PlayState = " << GetPlayStateString(m_PlayState) << ", call Play()" << LOG_ENDL;
                        Play();
                    }
                }
            }

            if(result && !isGetPlayStatusEmitted)
            {
                //LOG_TRACE << "   Play current selection succeeded" << LOG_ENDL;
                if(m_IsIDPS == false)
                {
                    if(m_PlayEngineTaskHandler)
                        m_PlayEngineTaskHandler->emitStateChangeForNonIDPS();
                }
            }
            else
            {
                //LOG_TRACE << "   Play Previous fail" << LOG_ENDL;
            }
            delayNextPrevExecution(200);
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "IpodController::Prev() ... Execption" << LOG_ENDL;
        return;
    }
    //LOG_TRACE << "CIpodController::Prev() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::Scan                    void (OUT), (int) (IN)
//
// Explanation
// Funtion to scan the device
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::Scan(int mode)
{
    LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState = " << GetPlayStateString(m_PlayState) << ", mode = " << mode << LOG_ENDL;

    bool result = false;

    m_nScanStartPosition = -1;

    try
    {
        if((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady))
        {
            bool isUIDPlay = false;
            int scanMode = -1;
            m_scanMode = -1;

            switch( mode )
            {
                case SCANFILE:
                {
                    scanMode = SCAN_FILE;
                }
                    break;
                case SCANALL:
                case SCANFOLDER:
                {
                    scanMode = SCAN_FOLDER;
                }
                    break;
                case SCANOFF:
                {
                    scanMode = SCAN_OFF;
                }
                    break;
                default:
                    break;
            }

            m_scanMode = scanMode;
            //LOG_TRACE << "   scanMode = " << scanMode << ", m_scanMode = " << m_scanMode << LOG_ENDL;

            if(mode != SCAN_OFF)
            { //Make random off if it is on
                if( (m_PlayEngineTaskHandler != NULL) && (m_PlayEngineTaskHandler->m_RandomMode != RANDOM_OFF) )
                {
                    result = m_PlayEngineTaskHandler->setShuffleE(RANDOM_OFF);
                    result = m_PlayEngineTaskHandler->setRandomMode(RANDOM_OFF);
                    if(m_PrivateData->m_IsConvertedFromUIDToIndex == false)
                    {   //Tell AVP that we are RANDOM_OFF
                        //This is for index device only.
                        //For UID device, we do this from packet available
                        emit OnModeChanged(RANDOMOFF);
                    }
                }
                if( (m_PlayEngineTaskHandler != NULL) && (m_PlayEngineTaskHandler->m_RepeatMode != REPEAT_FOLDER) )
                {
                    result = m_PlayEngineTaskHandler->setRepeatE(REPEAT_FOLDER);
                    result = m_PlayEngineTaskHandler->setRepeatMode(REPEAT_FOLDER);
                }
                //LOG_TRACE << "   Random(Shuffle) Mode is off_"  << LOG_ENDL;
            }

            if(isUIDPlay == false)
            {
                if( m_PlayEngineTaskHandler != NULL )
                {
                    result = m_PlayEngineTaskHandler->setScanModeE(scanMode);
                }

                if(m_PlayState == PAUSED && mode != SCAN_OFF)
                {
                    Play();
                }
            }
            else
            {
                if(m_PlayState == PAUSED && mode != SCAN_OFF)
                {
                    Play();
                }
            }

            //LOG_TRACE << "   setScanMode result = " << result << LOG_ENDL;
            if( result == true )
            {
                //LOG_TRACE << "   emit OnModeChanged( mode = " << mode << ")"  << LOG_ENDL;
                emit OnModeChanged( mode );
            }
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::Scan() ... Execption" << LOG_ENDL;
        return;
    }
    m_nScanStartPosition = -1;
    //LOG_TRACE << "CIpodController::Scan() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::RandomPlay                    void (OUT), (int) (IN)
//
// Explanation
// Funtion to support the suffle playing of the songs
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::RandomPlay(int mode, int /*AutoPlayFlag */ )
{
    //LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState =" << GetPlayStateString(m_PlayState) << ", mode = " << mode << LOG_ENDL;
    if(m_PrivateData->m_IsTabOff == true)
    {
        LOGH " m_PrivateData->m_IsTabOff == true so RETURN" << LOG_ENDL;
        return;
    }

    CIpodControllerQueueData tempData( false, -1, mode );
    QueuingEvent( ERandomEvent, tempData );
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::RandomPlayFromQueue(int mode, int /*AutoPlayFlag */ )
{
    LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState = " << GetPlayStateString(m_PlayState) << ", mode = " << mode << LOG_ENDL;

    bool result = false;

    try
    {
        if( (( m_ControllerState == eStateiPodReady ) || 
             ( m_ControllerState == eStateAutoPlayReady )))
        {
            int randomMode = -1;
            switch(mode) {
                case RANDOMFILE:    { randomMode = RANDOM_FILE;    break; }
                case RANDOMFOLDER:  { randomMode = RANDOM_FOLDER;  break; }
                case RANDOMOFF:     { randomMode = RANDOM_OFF;     break; }
                default: break;
            }

            //LOG_TRACE << "   randomMode = " << randomMode << LOG_ENDL;

            if( m_PlayEngineTaskHandler != NULL )
            {
                if( m_PlayEngineTaskHandler->isPlayListSet() == false )
                {
                    result = m_PlayEngineTaskHandler->setShuffleE(randomMode);
                }
                else
                {
                    result = m_PlayEngineTaskHandler->setShuffleE(0);
                }
                result = m_PlayEngineTaskHandler->setRandomMode(randomMode);
            }
            //LOG_TRACE << "   setShuffle() result = " << result << LOG_ENDL;

            if( result == true )
            {
                //LOG_TRACE << "   emit OnModeChanged( mode = " << mode << ")"  << LOG_ENDL;
                emit OnModeChanged(mode);
            }
            //When Pause state and shuffle/repeat change, play music for consistence of USB/JukeBOX
            if( m_PlayState == PAUSED && m_PlayEngineTaskHandler != NULL )
                m_PlayEngineTaskHandler->playControlE("Play");
        }

    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::RandomPlay() ... Execption" << LOG_ENDL;
        return;
    }

    //LOG_TRACE << "CIpodController::RandomPlay() OUT" << LOG_ENDL;
}

void CIpodController::RepeatPlay(int repeatMode, int /*AutoPlayFlag */ )
{
    //LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState =" << GetPlayStateString(m_PlayState) << ", mode = " << repeatMode << LOG_ENDL;
    CIpodControllerQueueData tempData(false, repeatMode, -1);
    QueuingEvent(ERepeatEvent, tempData);
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::RepeatPlayFromQueue(int repeatMode, int /*AutoPlayFlag */ )
{
    LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState = " << GetPlayStateString(m_PlayState) << ", mode = " << repeatMode << LOG_ENDL;

    bool result = false;

    try
    {
        if( ((m_ControllerState == eStateiPodReady) ||
             ( m_ControllerState == eStateAutoPlayReady)) )
        {
            int mode = -1;
            switch( repeatMode )
            {
                case REPEATFILE:
                {
                    mode = 0x01;   // Modified by pawan 2013.05.28.
                    if( m_PlayEngineTaskHandler != NULL )
                        result = m_PlayEngineTaskHandler->setRepeatE(mode);
                }
                    break;
                /*REPEAT ALL and FOLDER are handled in same way for ipod */
                case REPEATALL:
                case REPEATFOLDER:
                case REPEATOFF:
                {
                    mode = 0x02;
                    if( m_PlayEngineTaskHandler != NULL )
                    {
                        result = m_PlayEngineTaskHandler->setRepeatE(mode);
                        result = m_PlayEngineTaskHandler->setRepeatMode(mode);
                    }
                }
                    break;
                default:
                    break;
            }

            //LOG_TRACE << "   setRepeat() result = " << result << LOG_ENDL;

            if( result == true )
            {
                //LOG_TRACE << "   emit OnModeChanged( repeatMode = " << repeatMode << ")"  << LOG_ENDL;
                emit OnModeChanged(repeatMode);
            }

            //When Pause state and shuffle/repeat change, play music for consistence of USB/JukeBOX
            if(m_PlayState == PAUSED  && m_PlayEngineTaskHandler != NULL)
               m_PlayEngineTaskHandler->playControlE("Play");           
        }

    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::RepeatPlay() .... Execption" << LOG_ENDL;
        return;
    }
    //LOG_TRACE << "CIpodController::RepeatPlay() OUT" << LOG_ENDL;
}

void CIpodController::FastForward(int /*rate*/)
{
    CIpodControllerQueueData tempData;
    QueuingEvent(EFFwdEvent, tempData);
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::FastForwardFromQueue(int /*rate*/)
{
    LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState =" << GetPlayStateString(m_PlayState) << LOG_ENDL;

    EmitScanComplete();
    bool result = false;

    try
    {
        if((((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady))))
        {
            if(m_PlayEngineTaskHandler)
            {
                if( (m_PlayState == PLAYING) || (m_PlayState == PAUSED) )
                {
                    //After pause for nano1g, Setplaystatus change notification is not sent.
                    //so change the state to play and then do seek in order to reflect the position change during seek.
                    // After pause for all NonUID devices, Setplaystatus change notification is not sent.
                    // So, it should deleted this condition
                    if(m_PlayState == PAUSED)
                    {
                        m_PlayEngineTaskHandler->playControlE("play");
                    }
                    result = m_PlayEngineTaskHandler->playControlE("FastForward");
                    //if( m_PrivateData ) m_PrivateData->SendAudioRouterFadeOut();
                }
                else if (m_PlayState == SEEKING_FF)
                    result = true;
            }
            if(result)
            {
                m_PlayState = SEEKING_FF;
                //LOG_TRACE << "   FastForward result = " << result << ", m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
            }
            else
            {
                //LOG_TRACE << "   FastForward failed" << LOG_ENDL;
            }
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::FastForward() ... Execption" << LOG_ENDL;
        return;
    }
    //LOG_TRACE << "CIpodController::FastForward() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::Rewind                    void (OUT), (int) (IN)
//
// Explanation
// Funtion to support the rewind of the current playing song
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::Rewind(int /*rate*/)
{
    CIpodControllerQueueData tempData;
    QueuingEvent(ERwdEvent, tempData);
    ExecuteAlreadyQueuedEvent();
}
void CIpodController::RewindFromQueue(int /*rate*/)
{
    LOGH " m_ControllerState = " << GetControllerStateString() << ", m_PlayState =" << GetPlayStateString(m_PlayState) << LOG_ENDL;

    EmitScanComplete();
    bool result = false;

    try
    {
        if(( ((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady))))
        {
            if(m_PlayEngineTaskHandler)
            {
                if( (m_PlayState == PLAYING) || (m_PlayState == PAUSED) )
                {
                    //After pause for nano1g, Setplaystatus change notification is not sent.
                    //so change the state to play and then do rewind in order to reflect the position change during rewind.
                    // After pause for all NonUID devices, Setplaystatus change notification is not sent.
                    // So, it should changed this condition
                    if(m_PlayState == PAUSED)
                    {
                        m_PlayEngineTaskHandler->playControlE("play");
                    }
                    result = m_PlayEngineTaskHandler->playControlE("Rewind");
                    //if( m_PrivateData ) m_PrivateData->SendAudioRouterFadeOut();
                }
                else if (m_PlayState == SEEKING_REWIND)
                    result = true;
            }

            if(result)
            {
                m_PlayState = SEEKING_REWIND;
                //LOG_TRACE << "   Rewind result = " << result << ", m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
            }
            else
            {
                //LOG_TRACE << "   Rewind failed" << LOG_ENDL;
            }
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::Rewind() ... Execption" << LOG_ENDL;
        return;
    }
    //LOG_TRACE << "CIpodController::Rewind() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::NextFolder                    void (OUT), (struct PlayList*) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::NextFolder(struct PlayList*)
{
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::PrevFolder                    void (OUT), (struct PlayList*) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::PrevFolder(struct PlayList*)
{
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::GetCurrentFileName     void (OUT), void (IN)
//
// Explanation
// This method returns the title of currently playing track
// Receive notifications for state, track index and play position changes
//
// History
// 19-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
QString CIpodController::GetCurrentFileName()
{
    ////LOG_TRACE << "CIpodController::GetCurrentFileName() IN" << LOG_ENDL;
    QString title("");
    try
    {
        if((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady))
        {
            if(m_PlayState == STOPPED)
                title = "No Tracks Playing";
            else
            {
                if(m_PlayEngineTaskHandler)
                    title = m_PlayEngineTaskHandler->getCurrentFileName();
            }
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::GetCurrentFileName ... Execption" << LOG_ENDL;
        return title;
    }
    ////LOG_TRACE << "CIpodController::GetCurrentFileName OUT" << LOG_ENDL;
    return title;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::GetCurrentIndex                    int (OUT), (void) (IN)
//
// Explanation
// return the current index of the playing song
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIpodController::GetCurrentIndex()
{
    ////LOG_TRACE << "CIpodController::GetCurrentIndex() IN" << LOG_ENDL;
    int dCurrentIndex = -1;
    try
    {
        if((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady))
        {
            if(m_PlayEngineTaskHandler)
                dCurrentIndex =  m_PlayEngineTaskHandler->getCurrentIndex();
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::GetCurrentIndex() ... Execption" << LOG_ENDL;
        return 0;
    }

    ////LOG_TRACE << "CIpodController::GetCurrentIndex() OUT" << LOG_ENDL;
    return dCurrentIndex;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::SetCurrentIndex                    void (OUT), (int) (IN)
//
// Explanation
// set the now playing song with the current index
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::SetCurrentIndex(int nIndex)
{
    //LOGH ", index = " << nIndex << LOG_ENDL;
    if(m_PrivateData->m_IsTabOff == true)
    {
        LOGH " m_PrivateData->m_IsTabOff == true so RETURN" << LOG_ENDL;
        return;
    }
    disableTuneForFewMiliSecond(500);
    CIpodControllerQueueData tmpData(false, -1, -1, NULL, nIndex);
    QueuingEvent(ESetCurrentIndex, tmpData);
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::SetCurrentIndexFromQueue(int nIndex)
{
    LOGH ", index = " << nIndex << LOG_ENDL;
    {
        try
        {
            if(((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady)))
            {
                if(m_PlayEngineTaskHandler)
                {
                    //LOG_TRACE << " CIpodController::SetCurrentIndex  Parameter index = " << nIndex << ", current index = " << m_PlayEngineTaskHandler->getCurrentIndex() << LOG_ENDL;

                    m_resetOSDKeyTimer->stop();
                    m_PrivateData->m_OSDKey = OSDKey_SetCurrentIndex;

                    if(m_PlayEngineTaskHandler->getCurrentIndex() != nIndex)
                    {
                        m_PlayState = STOPPED;
                        emit OnStateChanged(PLAYER_PAUSE);
                    }
                    if(m_PlayEngineTaskHandler->setCurrentIndexE(nIndex))
                    {
                        //LOG_TRACE << " CIpodController::SetCurrentIndex  m_PlayState = STOPPED, emit OnStateChnaged(PLAYER_PAUSED)"<<LOG_ENDL;
                    }
                    else
                    {
                        m_PlayState = TO_BE_RESET;
                        emit OnStateChanged(PLAYER_PAUSE);
                        //LOG_TRACE << " CIpodController::SetCurrentIndex  m_PlayState = TO_BE_RESET, emit OnStateChnaged(PLAYER_PAUSE)"<<LOG_ENDL;
                    }
                }
            }
        }
        catch(EIPodErrors ex)
        {
            //LOG_TRACE << "CIpodController::SetCurrentIndex()... Execption" << LOG_ENDL;
            return;
        }
    }
    //LOG_TRACE << "CIpodController::SetCurrentIndex() Exit" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::GetCurrentMediaState                    int (OUT), (void) (IN)
//
// Explanation
// returns the current state of the media
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIpodController:: GetCurrentMediaState()
{
    ////LOG_TRACE << "CIpodController::GetCurrentMediaState() IN" << LOG_ENDL;
    if((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady))
    {
        int nPlayState = -1;

        switch(m_PlayState)
        {
            case PLAYING       : { nPlayState = PLAYER_PLAY;        break; }
            case PAUSED        : { nPlayState = PLAYER_PAUSE;       break; }
            case SEEKING_FF    : { nPlayState = PLAYER_FASTFORWARD; break; }
            case SEEKING_REWIND: { nPlayState = PLAYER_REWIND;      break; }
//          case STOPPED       : { nPlayState = PLAYER_STOP;        break; }
            case STOPPED       : { nPlayState = PLAYER_PAUSE;        break; }
            default: break;
        }
        return nPlayState;
    }
    else
    {
        return PLAYER_STOP;
    }
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::GetCurrentMediaDuration                    int (OUT), (void) (IN)
//
// Explanation
// get the time duration of the current playback
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIpodController::GetCurrentMediaDuration()
{
    ////LOG_TRACE << "CIpodController::GetCurrentMediaDuration() Entry" << LOG_ENDL;
    int dCurrentDuration = 0;
    try
    {
        if((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady))
        {
            if(m_PlayEngineTaskHandler)
                dCurrentDuration = m_PlayEngineTaskHandler->getCurrentMediaDuration();
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::GetCurrentMediaDuration()... Execption" << LOG_ENDL;
        return 0;
    }
    return dCurrentDuration;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::GetCurrentMediaPosition                    int (OUT), (void) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIpodController::GetCurrentMediaPosition()
{
    ////LOG_TRACE << "CIpodController::GetCurrentMediaPosition() Entry" << LOG_ENDL;
    int dCurrentPosition = 0;
    try
    {
        if((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady))
        {
            if( m_PlayState != STOPPED )
            {
                if(m_PlayEngineTaskHandler)
                    dCurrentPosition = m_PlayEngineTaskHandler->getCurrentMediaPosition();
            }
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::GetCurrentMediaPosition() ... Execption" << LOG_ENDL;
        return 0;
    }
    return dCurrentPosition;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::SetCurrentMediaPosition                    void (OUT), (int) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::SetCurrentMediaPosition(int position)
{
    CIpodControllerQueueData tmpData(false, -1, -1, NULL, -1, position);
    QueuingEvent(ESetMediaPosEvent, tmpData);
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::SetCurrentMediaPositionFromQueue(int position)    // modified by sungha.choi 2013.04.19
{
    //LOG_TRACE << "CIpodController::SetCurrentMediaPosition() IN, position = " << position << LOG_ENDL;

    if( position < 0 )
    {
        //LOG_TRACE << "   position less than 0, so return" << LOG_ENDL;
        return;
    }
    if( m_PrivateData )
    {
        try
        {
            if(((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady)))
            {
                m_PlayEngineTaskHandler->setPosition(position);
            }
        }
        catch(EIPodErrors ex)
        {
            //LOG_TRACE << "CIpodController::SetCurrentMediaPosition() ... Execption" << LOG_ENDL;
            return;
        }
    }
    //LOG_TRACE << "CIpodController::SetCurrentMediaPosition() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::GetDeviceState                    int (OUT), (void) (IN)
//
// Explanation
// returns the state of the device
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
// 10-Feb-2012      Junghwan Jang                      Function implementation
*****************************************************************************/
int CIpodController:: GetDeviceState()
{
    ////LOG_TRACE << "GetDeviceState Entry = " << m_DeviceState << LOG_ENDL;
    return m_DeviceState;
}

int
CIpodController::GetControllerState()
{
    ////LOG_TRACE << "GetControllerState Entry = " << GetControllerStateString() << LOG_ENDL;
    return m_ControllerState;
}


void CIpodController::ReplaceEmptyMediaInfo(MediaInfo* pMediaInfo)
{
    if(pMediaInfo->Title.isEmpty())
    {
        pMediaInfo->Title = gUnknownTitle;
    }

    if(pMediaInfo->Artist.isEmpty())
    {
        pMediaInfo->Artist = gUnknownArtist;
    }

    if(pMediaInfo->Album.isEmpty())
    {
        pMediaInfo->Album = gUnknownAlbum;
    }

    if(pMediaInfo->Genre.isEmpty())
    {
        pMediaInfo->Genre = gUnknownGenre;
    }

    if(pMediaInfo->Composer.isEmpty())
    {
        pMediaInfo->Composer = gUnknownComposer;
    }
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::GetCurrentMediaInfo     void (OUT), void (IN)
//
// Explanation
// This method returns media info of currently playing track
//
// History
// 19-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
void CIpodController::GetCurrentMediaInfo(MediaInfo* mediaInfo)
{
    if((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady))
    {
        MediaInfo* pMediaInfo = (MediaInfo*) mediaInfo;
        if(m_PlayState != STOPPED)
        {
            if(pMediaInfo)
            {
                if(m_PlayEngineTaskHandler)
                    m_PlayEngineTaskHandler->getCurrentMediaInfo(pMediaInfo);

                ReplaceEmptyMediaInfo(pMediaInfo);

                //LOG_TRACE << "TanDebug GetCurrentMediaInfo Artist: " << pMediaInfo->Artist <<LOG_ENDL;
                //LOG_TRACE << "TanDebug GetCurrentMediaInfo Title: " << pMediaInfo->Title <<LOG_ENDL;
                //LOG_TRACE << "TanDebug GetCurrentMediaInfo Album: " << pMediaInfo->Album <<LOG_ENDL;
            }
        }
    }
}


/******************************************************************************
// Function Name                                            Parameters
// CIpodController::GetDeviceInfo                    DeviceInfo* (OUT), (void) (IN)
//
// Explanation
// Fill the information of the device information in the passed object
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::GetDeviceInfo(DeviceInfo* /*pDeviceInfo*/)
{
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::GetPlaylist                    PlayList* (OUT), (void) (IN)
//
// Explanation
// Fill the information of the playlist in the passed object
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::GetPlaylist(PlayList* pPlaylist)
{
    //LOG_TRACE << "CIpodController::GetPlaylist() IN" << LOG_ENDL;
    try
    {
        if( pPlaylist != NULL )
        {
            if(m_PlayEngineTaskHandler)
            {
                if ( pPlaylist->list != NULL)
                {
                    *(pPlaylist->list) = m_PlayEngineTaskHandler->m_PlaylistURIs;
                }
                pPlaylist->index = m_PlayEngineTaskHandler->getCurrentIndex() ;
                pPlaylist->mode =  REPEATOFF;
                //LOG_TRACE << "   getCurrentIndex         = " << m_PlayEngineTaskHandler->getCurrentIndex() << LOG_ENDL;
                //LOG_TRACE << "   getCurrentMediaPosition = " << m_PlayEngineTaskHandler->getCurrentMediaPosition() << LOG_ENDL;
                pPlaylist->position = m_PlayEngineTaskHandler->getCurrentMediaPosition();
            }
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::GetPlaylist() Execption" << LOG_ENDL;
        return;
    }
    //LOG_TRACE <<"CIpodController::GetPlaylist() OUT"<< LOG_ENDL;
}

QString CIpodController::getCategoryType(int category)
{
    QString CategoryStr;
    switch(category)
    {
        case 0:
            CategoryStr = DHTrackCategory;
            break;
        case 3:
            CategoryStr = PLAYLISTCATEGORY;
            break;
        case 5:
            CategoryStr = ARTISTCATEGORY;
            break;
        case 6:
            CategoryStr = ALBUMCATEGORY;
            break;
        case 7:
            CategoryStr = TRACKCATEGORY;
            break;
        case 8:
            CategoryStr = GENRECATEGORY;
            break;
        case 9:
            CategoryStr = COMPOSERCATEGORY;
            break;
        case 10:
            CategoryStr = AUDIOBOOKCATEGORY;
            break;
        case 11:
            CategoryStr = PODCASTCATEGORY;
            break;
        case 12:
            CategoryStr = ITUNEUCATEGORY;
            break;
        default:
            CategoryStr = DHTrackCategory;
            break;
    }
    return CategoryStr;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::SetPlayList                    void (OUT), (struct PlayList*) (IN)
//
// Explanation
// Sets the playlist to the device
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/

void CIpodController::SetPlayList(struct PlayList* playlist)
{
    CIpodControllerQueueData tempData(false, -1, -1, playlist);
    QueuingEvent(ESetPlaylistEvent, tempData);
    ExecuteAlreadyQueuedEvent();
}

void CIpodController::SetPlayListFromQueue(struct PlayList* playlist)
{
    LOGH LOG_ENDL;

    EmitScanComplete();

    if( playlist == NULL || playlist->list == NULL )
    {
        //LOG_TRACE << "   Returning as one of the playlist or playlist->list is null" << LOG_ENDL;
        return;
    }

    //Changed to emit the count when the playlist is empty i.e we are playing the all songs category
    if( playlist->list->count() > 0 )
    {
        //LOG_TRACE <<"   count of playlist is greater than 0, count = " << playlist->list->count() << LOG_ENDL;
        m_PrivateData->m_FirstPlayListSet = true;   // Added by pawan 2013.05.01.
    }
    else
    {
        //LOG_TRACE <<"   count of playlist is lesser than 0, count = " << playlist->list->count() << LOG_ENDL;
        m_PrivateData->m_FirstPlayListSet = false;
    }

    //LOG_TRACE << "PLAYLIST : playlist->category: " << playlist->category <<LOG_ENDL;

    QString Category = getCategoryType(playlist->category);

#ifdef ONDEMANDCATEGORY
    Category = CurrentCategory;
#endif
    if(Category.compare("DHTrack", Qt::CaseInsensitive))
    {
        //LOG_TRACE << "PLAYLIST : converting device" <<LOG_ENDL;

        if(m_PlayEngineTaskHandler)
        {
            m_PlayState = STOPPED;
            //LOG_TRACE << "PLAYLIST : calling setPlayListCategoryItems with : playlist->mode: " << playlist->mode <<"  and playlist->index: " << playlist->index <<LOG_ENDL;
            m_PlayEngineTaskHandler->setPlayListCategoryItems(Category, playlist->mode, playlist->index);
            return;
        }
    }
    try
    {
        if((((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady))))
        {
            if(playlist != NULL)
            {
                if(playlist->list != NULL)
                {
                    QList<QString> *uris = playlist->list;
                    if(m_PlayEngineTaskHandler)
                    {
                        m_PlayEngineTaskHandler->setPlayListE(uris);
                    }
                    m_PlayState = STOPPED;
                    bool result = false;
                    int randomMode = -1;
                    int mode = -1;

                    switch(playlist->mode)
                    {
                        case RANDOMFILE:   { randomMode = 1; break; }
                        case RANDOMFOLDER: { randomMode = 2;  break; }
                        case RANDOMOFF:    { randomMode = 0;  break; }
                        default: break;
                    }
                    //randomMode = 9;
                    //LOG_TRACE << "   playlist->mode is " << playlist->mode
                    //          << ", RandomMode is  = " << randomMode << LOG_ENDL;
                    if( (randomMode == 0) ||  (randomMode == 1) ||  (randomMode == 2) )
                    {
                        //Coverity fix -Forward Null => Null  pointer  dereference
                        if(m_PlayEngineTaskHandler)
                        {
                            result = m_PlayEngineTaskHandler->setRandomMode(randomMode);
                        }
                    }

                    switch(playlist->mode)
                    {
                        case REPEATFILE:   { mode = 1; break; }
                        case REPEATFOLDER: { mode = 2; break; }
                        case REPEATOFF:    { mode = 0; break; }
                        default: break;
                    }
                    //LOG_TRACE << "   playlist->mode = " << playlist->mode
                    //          << ", repeatMode = " << mode << LOG_ENDL;
                    //mode = 9;
                    if( (mode == 0) ||  ( mode == 1) ||  (mode == 2) )
                    {
                        //Coverity fix -Forward Null => Null  pointer  dereference
                        if(m_PlayEngineTaskHandler)
                        {
                            result = m_PlayEngineTaskHandler->setRepeatMode(mode);
                        }
                    }
                }
            }
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CIpodController::SetPlayList() ... Execption" << LOG_ENDL;
        return;
    }

    //LOG_TRACE << "CIpodController::SetPlayList() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::SeekableMedia                    bool (OUT), (void) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
bool CIpodController::SeekableMedia()
{
    return true;
}

/******************************************************************************
// Function Name                                            Parameters
// CIpodController::SetBrightness                    void (OUT), (int) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
// 29-Sep-2011      Sowmini Philip                     Modified param int to double
*****************************************************************************/
void CIpodController::SetBrightness(double)
{
}

void CIpodController::SetBrightness(MEDIA_DISPLAY /*displayPlane */,double)
{
}

/******************************************************************************
// Function                                            Parameters
// CIpodController::SetContrast                    void (OUT), (int) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
// 29-Sep-2011      Sowmini Philip                     Modified param int to double
*****************************************************************************/
void CIpodController::SetContrast(double)
{
}

void CIpodController::SetContrast(MEDIA_DISPLAY /* displayPlane */,double)
{
}

/******************************************************************************
// Function                                             Parameters
// CIpodController::SetTint                    void (OUT), (int) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
// 29-Sep-2011      Sowmini Philip                     Modified param int to double
*****************************************************************************/
void CIpodController::SetTint(double)
{
}

void CIpodController::SetTint(MEDIA_DISPLAY /* displayPlane */,double)
{
}

/******************************************************************************
// Function                                            Parameters
// CIpodController::GetBrightness                    int (OUT), (void) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
// 29-Sep-2011      Sowmini Philip                     Modified param int to double
*****************************************************************************/
double CIpodController:: GetBrightness()
{
    ////LOG_TRACE << "CIpodController::SetPlayList Exit" << LOG_ENDL;
    return IPOD_ERROR_UNSUPPORTED;
}

/******************************************************************************
// Function                                            Parameters
// CIpodController::GetContrast                    int (OUT), (void) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
// 29-Sep-2011      Sowmini Philip                     Modified param int to double
*****************************************************************************/
double CIpodController:: GetContrast()
{
    ////LOG_TRACE << "CIpodController::Contrast is not applicable" << LOG_ENDL;
    return IPOD_ERROR_UNSUPPORTED;
}

/******************************************************************************
// Function                                            Parameters
// CIpodController::GetTint                    int (OUT), (void) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
// 29-Sep-2011      Sowmini Philip                     Modified param int to double
*****************************************************************************/
double CIpodController:: GetTint()
{
    //LOG_TRACE << "CIpodController::GetTint is not applicable" << LOG_ENDL;
    return IPOD_ERROR_UNSUPPORTED;
}

/******************************************************************************
// Function                                            Parameters
// CIpodController::SetSystemState                    void (OUT), (int) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::SetSystemState(int state)
{
    //LOGH " state = " << state << LOG_ENDL;

    switch( state )
    {
        case SYSTEM_IPODDEVICE_RESET_ON :
        {
            LOG_HIGH << "CIpodController::SetSystemState() recieved  SYSTEM_IPODDEVICE_RESET_ON" << LOG_ENDL;
            if( state == SYSTEM_IPODDEVICE_RESET_ON )
                m_Detector->resetIPod();
        }
            break;
        case SYSTEM_IPODDEVICES_TABOFF  :
        {
            LOG_HIGH << "CIpodController::SetSystemState() recieved  SYSTEM_IPODDEVICE_TABOFF" << LOG_ENDL;
            //LOG_TRACE << "   m_PrivateData->m_IsTabOff = true" << LOG_ENDL;
            m_PrivateData->m_IsTabOff = true;
            m_PrivateData->m_bBreakNextLoop = true;
            if(m_TrackerAbs)
                disconnect(m_TrackerAbs, SIGNAL(getCoverArt(QStringList, QStringList, QString)), this, SLOT(fetchCoverArtFromIPodSlot(QStringList, QStringList, QString)));

            m_bDelayNextPrev = false;

            if(m_PlayEngineTaskHandler)
            {
                m_PlayEngineTaskHandler->setPauseState();
            }

            if(m_scanMode != SCANOFF)
                Scan(SCANOFF);

            //emit OnModeChanged(SCANOFF);

            m_PrivateData->m_QueueData.clearQueueData();
            m_PrivateData->m_EventCommandQueue.clear();
        }
            break;
        case SYSTEM_ACC_ON:
        {
            LOG_HIGH << "CIpodController::SetSystemState() recieved  SYSTEM_ACC_ON" << LOG_ENDL;
            //LOG_TRACE << "   m_startNotifierFlag = true, call start()" << LOG_ENDL;
            m_startNotifierFlag = true;
            start();
        }
            break;
        case SYSTEM_ACC_OFF:
        {
            LOG_HIGH << "CIpodController::SetSystemState() recieved  SYSTEM_ACC_OFF" << LOG_ENDL;
            m_startNotifierFlag = false;
        }
            break;
        // { Added by jonghwan.cho@lge.com 2013.11.05.
        case SYSTEM_BT_CALL_START :
            LOG_HIGH << "CIpodController::SetSystemState() recieved  SYSTEM_BT_CALL_START" << LOG_ENDL;
			m_PrivateData->m_IsBTCalling = true;
            break;
        case SYSTEM_BT_CALL_END :
            LOG_HIGH << "CIpodController::SetSystemState() recieved  SYSTEM_BT_CALL_END" << LOG_ENDL;
			m_PrivateData->m_IsBTCalling = false;
            break;
        // } Added by jonghwan.cho@lge.com
        default :
            break;
    }
    //LOG_TRACE << "CIpodController::SetSystemState() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// CIpodController::GetVideoWidget                    QWidget* (OUT), (void) (IN)
//
// Explanation
// Future implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QWidget*    CIpodController::GetVideoWidget()
{
    return NULL;
}

/******************************************************************************
// Function                                            Parameters
// CIpodController::OnStateChangedSlot                    void (OUT), (int) (IN)
//
// Explanation
// sets the media state
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::OnStateChangedSlot(int state)
{
    //LOG_TRACE << "CIpodController::OnStateChangedSlot() IN, state = " << state << LOG_ENDL;

    int nPlayState = -1;

    try
    {
        if( (m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady) )
        {
            if(state == ePlaybackState_Playing_0x0A && m_PrivateData->m_IsTabOff)
            {
                LOG_HIGH << "   its tab off and got play status , so pause it please" << LOG_ENDL;

 #ifdef SIRI_TIMEOUT_TEST
                //LOG_TRACE << "   IsTabOff = " << m_PrivateData->m_IsTabOff << ", but sending Signal for SIRI" << LOG_ENDL;
                emit NotifyPlayStateForSiri();
 #endif
                state = ePlaybackState_Paused_0x0B;
                m_PlayState = PAUSED;
                nPlayState = PLAYER_PAUSE;
                Pause();
            }
            switch(state)
            {
                case ePlaybackState_FFW_REW_SeekStopped_0x07 : {
                    //LOG_TRACE << "   case STOP SEEK 0x07" << LOG_ENDL;
                    if( (m_PlayState == SEEKING_FF) || (m_PlayState == SEEKING_REWIND) )
                    {
                        m_PlayEngineTaskHandler->playControlE("StopSeek");
                    }
                }
                    break;
                case ePlaybackState_Playing_0x0A :
                {
                    m_PlayState = PLAYING;
                    nPlayState  = PLAYER_PLAY;
                    //LOG_TRACE << "   case PLAYING 0x0A, m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
                }
                    break;
                case ePlaybackState_Paused_0x0B :
                {
                    // condition is added, as while setting playlist for
                    // devices sometime the pause signal comes and state changed
                    // to pause and the play functionlity takes a different path
                    // and keep play the same song again and again.
                    if(m_PlayState != STOPPED)
                    {
                        m_PlayState = PAUSED;
                        nPlayState = PLAYER_PAUSE;
                        //LOG_TRACE << "   case PAUSED 0x0B, m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
                    }
                }
                    break;
                case ePlaybackState_FFWSeekStarted_0x05 :
                {
                    m_PlayState = SEEKING_FF;
                    nPlayState = PLAYER_FASTFORWARD;
                    //LOG_TRACE << "   case SEEKING_FF 0x05, m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
                }
                    break;
                case ePlaybackState_REWSeekStarted_0x06 :
                {
                    m_PlayState = SEEKING_REWIND;
                    nPlayState = PLAYER_REWIND;
                    //LOG_TRACE << "   case SEEKING_REW 0x06, m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
                    break;
                }
                case ePlaybackState_Stoppped_0x02 :
                {
                    m_PlayState = STOPPED;
                    //dont send the stop state to IPod ever, as it can cause issue
                    //nPlayState = PLAYER_STOP;
                    nPlayState = PLAYER_PAUSE;
                    //LOG_TRACE << "   case STOP 0x02, m_PlayState = " << GetPlayStateString(m_PlayState) << LOG_ENDL;
                    break;
                }
                case ePlaybackState_Reserved_0x01 :
                {
                    m_PlayState = PLAYING;
                    nPlayState = PLAYER_PLAY;
                    //LOG_TRACE << "   case PLAYING 0x01, m_PlayState = " << GetPlayStateString(m_PlayState) << ", non-IDPS" << LOG_ENDL;
                }
                    break;
                case ePlaybackState_Reserved_0x00 :
                {
                    if( (m_PlayState == SEEKING_FF) || (m_PlayState == SEEKING_REWIND) )
                    {
                        m_PlayEngineTaskHandler->playControlE("StopSeek");
                    }
                    m_PlayState = STOPPED;
                    //dont send the stop state to IPod ever, as it can cause issue
                    //nPlayState = PLAYER_STOP;
                    nPlayState = PLAYER_PAUSE;
                    //LOG_TRACE << "   case STOP 0x00, m_PlayState = " << GetPlayStateString(m_PlayState) << ", non-IDPS" << LOG_ENDL;
                }
                    break;
                default:
                    break;
            } //switch(state) {
        } // if
    } // try
    catch(EIPodErrors ex)
    {
        //LOG_TRACE <<"CIpodController::OnStateChangedSlot Execption Caught ******" << LOG_ENDL;
        return;
    }

    //LOG_HIGH << "CIpodController::OnStateChangedSlot(), call updateDigitalAudioPipeline( nPlayState = " << GetnPlayStateString(nPlayState) << " )" << LOG_ENDL;
    updateDigitalAudioPipeline(nPlayState);

    LOG_HIGH << "CIpodController::OnStateChangedSlot(), nPlayState = " << GetnPlayStateString(nPlayState) << ", m_PlayState = " << m_PlayState << LOG_ENDL;

    if(nPlayState != -1)
    {
        emit OnStateChanged(nPlayState);
    }

    //LOG_TRACE << "   m_PrevPlayState = " << GetPlayStateString(m_PrevPlayState) << ", m_PlayState = " << GetPlayStateString(m_PlayState) << ", nPlayState = " << GetnPlayStateString(nPlayState) << LOG_ENDL;
    if( m_PlayState == PLAYING )
    {
        //LOG_HIGH << "CIpodController::OnStateChangedSlot(), emit OnNormalPlay()"  << LOG_ENDL;
        emit OnNormalPlay();
    }
    //LOG_TRACE << "CIpodController::OnStateChangedSlot() OUT" << LOG_ENDL;
}

void CIpodController::updateDigitalAudioPipeline(int state)
{
    //LOG_TRACE << "CIpodController::updateDigitalAudioPipeline() IN, state = " << state << LOG_ENDL;

    switch(state)
    {
        case PLAYER_PLAY:
        {
            if(m_PlayEngineTaskHandler)
            {
                m_PlayEngineTaskHandler->setPlayState();
            }
        }
            break;
        case PLAYER_PAUSE:
        case PLAYER_STOP:
            break;
    }

    //LOG_TRACE << "CIpodController::updateDigitalAudioPipeline() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// CIpodController::OnErrorSlot                    void (OUT), (int) (IN)
//
// Explanation
// Slot will handle the errors generated in the system
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::OnErrorSlot(int)
{
}

/******************************************************************************
// Function                                            Parameters
// CIpodController::OnModeChangedSlot                    void (OUT), (int) (IN)
//
// Explanation
// Future Implementation
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIpodController::OnModeChangedSlot(int /*index*/)
{
}

/******************************************************************************
// Function                                            Parameters
// CIpodController::OnCurrentMediaStatusChangedSlot                    void (OUT) , void (IN)
//
// Explanation
// Emit OnCurrentMediaStatusChanged signal at the end of track
// History
// 15-Feb-2012      Sowmini Philip                        Added initial implementation
*****************************************************************************/
void CIpodController::OnCurrentMediaStatusChangedSlot()
{
    //LOG_TRACE << "CIpodController::OnCurrentMediaStatusChangedSlot() IN" << LOG_ENDL;
    //LOG_HIGH << "CIpodController::OnCurrentMediaStatusChangedSlot(), emit OnCurrentMediaStatusChanged(PLAYER_EndOfMedia)_"  << LOG_ENDL;
    emit OnCurrentMediaStatusChanged(PLAYER_EndOfMedia);
    //LOG_TRACE << "CIpodController::OnCurrentMediaStatusChangedSlot() OUT" << LOG_ENDL;
}

void CIpodController::EnableSubTitle(bool /* bEnable*/)
{
}

void CIpodController::SetPlayerEngine(PLAYER_ENGINE_TYPE)
{
}

void  CIpodController::SetFontSize(const int /*fontSize*/)
{
}

int CIpodController::ChangeMediaMode(MEDIA_PLAYER_MODE /*eMode*/)
{
    return IPOD_ERROR_UNSUPPORTED;
}

QString CIpodController::getiPodSerialNumber()
{
    QString serialNum;
    serialNum.clear();
    if(m_ControllerState >= eStateEnteredInUIMode)
    {
        if(m_ITunesTaskHandler != NULL)
        {
            try
            {
                serialNum = m_ITunesTaskHandler->getCurrentSerialNumberE();
            }
            catch(EIPodErrors ex)
            {
                //LOG_TRACE <<"CIpodController::getiPodSerialNumber Execption Caught ******" << LOG_ENDL;
                return NULL;
            }
        }
    }
    return serialNum;
}

void  CIpodController::SetHue(double /*value*/)
{
}

void  CIpodController::SetHue(MEDIA_DISPLAY /*displayPlane*/, double /*value*/)
{
}

double  CIpodController::GetHue()
{
    return IPOD_ERROR_UNSUPPORTED;
}

bool CIpodController::ClearDisplay(bool /*isBackground*/)
{
    return IPOD_ERROR_UNSUPPORTED;
}

// For some devices/songs the scantimer is expiring between 7-11 secs
// So LG wanted the implementation based on the position change.
void CIpodController::OnPositionChangedSlot(int msec)
{
    // For some devices the notification for the state change, track index
    // change and track UID change is not coming in some specific cases
    // So this condition is to make sure that UI is always in right state.
    if((m_PlayState == PAUSED) && (m_PlayEngineTaskHandler->m_playState == ePlayState_PLAY))
    {
        m_PlayState = PLAYING;
        OnStateChangedSlot(0x0A);
    }

    if( m_PlayEngineTaskHandler)
    {
        if(!m_PrivateData->m_IsConvertedFromUIDToIndex)
        {
            MediaInfo mediaInfo;
            m_PlayEngineTaskHandler->emitCurrentMediaInfoE(&mediaInfo);
        }
    }

    emit OnPositionChanged(msec);
    //ResetOSDConditionAtSongEnd(msec);
    PerformScanCheck(msec);
}

void CIpodController::PerformScanCheck(int msec)
{
    try
    {
        if((m_scanMode == SCANFILE)|| (m_scanMode == SCANFOLDER) || (m_scanMode == SCANALL))
        {

            m_PrivateData->m_OSDKey = OSDKey_Scan;
            //LOG_TRACE <<"PerformScanCheck m_nScanStartPosition = " << m_nScanStartPosition << ", msec = " << msec << LOG_ENDL;

            //Tan, scan 10 sec is measured as difference between ScanStartPosition and current time.
            //Normal song: 10-1>=9
            //Podcast: 59-50>=9
            int elapsed = (msec - m_nScanStartPosition);
            if(m_nScanStartPosition <= 0 || elapsed < 0)
            {
                elapsed = -1;
                m_nScanStartPosition = msec;
            }

            if(elapsed >= SCAN_DURATION - 1000)
            {
                m_nScanStartPosition = -1;
                if(m_PlayEngineTaskHandler)
                {
                    m_PlayEngineTaskHandler->scanPlaylist();
                }
            }
        }
        else
        {
            if(m_PrivateData->m_OSDKey == OSDKey_Scan)
                m_PrivateData->m_OSDKey = OSDKey_None;
        }
    }
    catch(EIPodErrors er)
    {
        return;
    }
}

/******************************************************************************
// Function                                      Parameters
// CIpodController::GetCurrentFileFolderIndex    void (OUT) , int* (IN), int* (IN)
//
// Explanation
// Added this dummy API to avoid build break due to change in IMediaInterface 
   in media controller library.
// History
// 13-Sept-2012      Preetha P V                 Added dummy implementation
*****************************************************************************/
void CIpodController::GetCurrentFileFolderIndex(int */*index*/, int */*count*/)
{
}

void CIpodController::iPodReadySlot()
{
    //LOG_TRACE << "CIpodController::iPodReadySlot() Entry " << LOG_ENDL;
    LOG_HIGH << "CIpodController::iPodReadySlot(), emit iPodReady(true)_"  << LOG_ENDL;
    emit iPodReady(true);
    //LOG_TRACE << "CIpodController::iPodReadySlot() Exit " << LOG_ENDL;
}

void CIpodController::onIPodDeviceConnectedSLOT( QString node, QString serialNum, EIPodPorts ePortNum )
{
    //LOG_TRACE << "CIpodController::onIPodDeviceConnectedSLOT() IN " << LOG_ENDL;
    //LOG_HIGH  << "CIpodController::onIPodDeviceConnectedSLOT() node = " << node << ", seri = " << serialNum << ", port = " << ePortNum << ", m_IsSendedFirstConnectionSignal = true"<<LOG_ENDL;
    //LOG_HIGH  << "CIpodController::onIPodDeviceConnectedSLOT() m_IsSendedFirstConnectionSignal = true;_"  << LOG_ENDL;
    m_IsSendedFirstConnectionSignal = true;

    if(!m_PrivateData->m_restartUcsFlag)
    {
        //LOG_HIGH << "m_restartUcsFlag = false, emit onIPodConnectedSignal()"  << LOG_ENDL;
        emit onIPodConnectedSignal( node, serialNum, ePortNum, true );

        if( m_bWaitForFirstConnectionSignal == true )
        {
            node.clear();
            serialNum.clear();

            LOG_HIGH << "emit onIPodConnectedSignal( node = "
                     << node << ", serialNum = " << serialNum << ", ePortNum = " << ePortNum << ", false )_"  << LOG_ENDL;
            emit onIPodConnectedSignal( node, serialNum, ePortNum, false );
            m_bWaitForFirstConnectionSignal = false;
        }
    }
    else
    {
        LOG_HIGH << "   restartUcs, m_portresetFlag" << m_PrivateData->m_portresetFlag<< LOG_ENDL;
        //LOG_HIGH << "   m_restartUcsFlag = true, don't emit signal" << LOG_ENDL;
        if( m_bWaitForFirstConnectionSignal == true )
        {
            node.clear();
            serialNum.clear();

            LOG_HIGH << "emit onIPodConnectedSignal( node = "
                     << node << ", serialNum = " << serialNum << ", ePortNum = " << ePortNum << ", false )_"  << LOG_ENDL;
            emit onIPodConnectedSignal( node, serialNum, ePortNum, false );
            m_bWaitForFirstConnectionSignal = false;
        }

        if(m_PrivateData->m_portresetFlag == false)
        {
            LOG_HIGH << "restartUcs, m_restartUcsFlag" << m_PrivateData->m_restartUcsFlag<< LOG_ENDL;
            m_PrivateData->m_restartUcsFlag = false;
        }
    }

    //LOG_TRACE << "CIpodController::onIPodDeviceConnectedSLOT() OUT " << LOG_ENDL;
}

bool CIpodController::requestData(ETrackerAbstarctor_Query_Ids queryId, uint reqUID, int playlistIndex, int startIndex, int nCount)
{
    RequestDataElement element(queryId, reqUID, playlistIndex, startIndex, nCount);
    CIpodControllerQueueData tempData(element);
    QueuingEvent(ERequestDataEvent, tempData);
    ExecuteAlreadyQueuedEvent();

    return true; // Added by jonghwan.cho@lge.com 2013.10.16.
}

bool CIpodController::requestDataFromQueue( ETrackerAbstarctor_Query_Ids queryId, uint reqUID, int playlistIndex, int startIndex, int nCount )
{
    //LOG_TRACE << "CIpodController::requestData IN : playlistIndex: "<<playlistIndex <<" startIndex : "  <<startIndex << "  nCount: " << nCount << LOG_ENDL;
    try
    {
        bool retval = false;
        if( ((m_ControllerState == eStateiPodReady) || (m_ControllerState == eStateAutoPlayReady)))
        {
            //LOG_TRACE << "CIpodController::requestData processing : startIndex : " <<startIndex << "  nCount: " << nCount <<LOG_ENDL;
            retval = m_PlayEngineTaskHandler->requestData(queryId, reqUID, playlistIndex, startIndex, nCount);
        }
        return retval;
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE<<"Throw from requestData function"<<LOG_ENDL;
        return false;
    }
}

int CIpodController::getSortingOrder()
{
    int nSortingOrder = 0;
    if(m_TrackerConnector)
    {
        nSortingOrder = m_TrackerConnector->getSortingOrder();
    }
    //LOG_TRACE << "getSortingOrder: " << nSortingOrder << LOG_ENDL;
    return nSortingOrder;
}

// { Added by jonghwan.cho@lge.com 2013.06.10. for Progress bar sound is broken.
void CIpodController::AudioDragStart()
{
    LOGH LOG_ENDL;
    //LOG_TRACE << "CIpodController::AudioDragStart() IN" << LOG_ENDL;
    if( m_PrivateData ) m_PrivateData->SendAudioRouterFadeOut();
    //LOG_TRACE << "CIpodController::AudioDragStart() OUT" << LOG_ENDL;
}

void CIpodController::AudioDragEnd()
{
    LOGH LOG_ENDL;
    //LOG_TRACE << "CIpodController::AudioDragEnd() IN" << LOG_ENDL;
    if( m_PrivateData ) m_PrivateData->SendAudioRouterFadeIn();
    //LOG_TRACE << "CIpodController::AudioDragEnd() OUT" << LOG_ENDL;
}
// } Added by jonghwan.cho@lge.com 2013.06.10. for Progress bar sound is broken.

QString CIpodController::GetControllerStateString()
{
    QString strReturnState;
    switch( m_ControllerState )
    {
        case eStateIdle           : strReturnState = "eStateIdle";               break;
        case eStateiPodDetetcted  : strReturnState = "eStateiPodDetetcted";      break;
        case eStateiAPReaderReady : strReturnState = "eStateiAPReaderReady";     break;
        case eStateiAPWriterReady : strReturnState = "eStateiAPWriterReady";     break;
        case eStateiAPReady       : strReturnState = "eStateiAPReady";           break;
        case eStateIdentified     : strReturnState = "eStateIdentified";         break;
        case eStateAuthorized     : strReturnState = "eStateAuthorized";         break;
        case eStateiPodInitialized: strReturnState = "eStateiPodInitialized";    break;
        case eStateEnteredInUIMode: strReturnState = "eStateEnteredInUIMode";    break;
        case eStateiPodReady      : strReturnState = "eStateiPodReady";          break;
        case eStateAutoPlayReady  : strReturnState = "eStateAutoPlayReady";      break;
        case eStateReadyToAutoPlayReady : strReturnState = "eStateReadyToAutoPlayReady"; break;
        default :                   strReturnState = "Abnormal State";           break;
    }
    return strReturnState;
}

QString CIpodController::GetPlayStateString(int nPlayState)
{
    QString strReturnState;
    switch( nPlayState )
    {
        case PLAYING              : strReturnState = "PLAYING";       break;
        case PAUSED               : strReturnState = "PAUSED";        break;
        case SEEKING_FF           : strReturnState = "SEEKING_FF";    break;
        case SEEKING_REWIND       : strReturnState = "SEEKING_REWIND";break;
        case STOPPED              : strReturnState = "STOPPED";       break;
        default                   : strReturnState = "Abnormal State";break;
    }
    return strReturnState;
}

QString CIpodController::GetnPlayStateString(int nPlayState)
{
    QString strReturnState;
    switch( nPlayState )
    {
        case PLAYER_PLAY              : strReturnState = "PLAYER_PLAY";       break;
        case PLAYER_PAUSE             : strReturnState = "PLAYER_PAUSE";      break;
        case PLAYER_STOP              : strReturnState = "PLAYER_PAUSE";      break;
        case PLAYER_REPEATPLAY        : strReturnState = "PLAYER_REPEATPLAY"; break;
        case PLAYER_REPEATPAUSE       : strReturnState = "PLAYER_REPEATPAUSE";break;
        case PLAYER_RANDOMPLAY        : strReturnState = "PLAYER_RANDOMPLAY"; break;
        case PLAYER_RANDOMPAUSE       : strReturnState = "PLAYER_RANDOMPAUSE";break;
        case PLAYER_FASTFORWARD       : strReturnState = "PLAYER_FASTFORWARD";break;
        case PLAYER_REWIND            : strReturnState = "PLAYER_REWIND";     break;
        case PLAYER_NEXTTRACK         : strReturnState = "PLAYER_NEXTTRACK";  break;
        case PLAYER_PREVTRACK         : strReturnState = "PLAYER_PREVTRACK";  break;
        case PLAYER_SCAN              : strReturnState = "PLAYER_SCAN";       break;
        case PLAYER_SEARCH            : strReturnState = "PLAYER_SEARCH";     break;
        case PLAYER_FASTFORWARD_X4    : strReturnState = "PLAYER_FASTFORWARD_X4"; break;
        case PLAYER_FASTFORWARD_X16   : strReturnState = "PLAYER_FASTFORWARD_X16";break;
        case PLAYER_FASTFORWARD_X20   : strReturnState = "PLAYER_FASTFORWARD_X20";break;
        case PLAYER_REWIND_X4         : strReturnState = "PLAYER_REWIND_X4";  break;
        case PLAYER_REWIND_X16        : strReturnState = "PLAYER_REWIND_X16"; break;
        case PLAYER_REWIND_X20        : strReturnState = "PLAYER_REWIND_X20"; break;
        default                       : strReturnState = "Abnormal State";    break;
    }
    return strReturnState;
}

