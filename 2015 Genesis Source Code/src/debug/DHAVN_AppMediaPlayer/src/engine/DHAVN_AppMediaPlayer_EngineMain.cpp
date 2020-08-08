#include <QDebug>
#include <QTranslator>
#include <QDeclarativeContext>

#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "engine/DHAVN_AppMediaPlayer_EngineAudio.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include "engine/DHAVN_AppMediaPlayer_EnginePhoto.h"
#include "controller/DHAVN_AppMediaPlayer_Controllers.h"
#include "util/photo/DHAVN_AppMediaPlayer_Thumbnailer.h" //2014.05.23
#include "util/DHAVN_AppMediaPlayer_GraphicsOutput.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/DHAVN_AppMediaPlayer_Bootstrap.h"
#include "util/DHAVN_AppMediaPlayer_SettingsLoader.h"//Added by Alexey Edelev 2012.10.08
//#include "util/audio/DHAVN_AppMediaPlayer_AppGLWidget.h"//added by junam 2013.04.03 for ISV77703
#include "util/audio/DHAVN_AppMediaPlayer_Utils.h"
#include "util/osd/OSD_Manager.h"

#define AVP_USE_USB_2SECOND_CONNECT     1
// added by kihyung 2013.08.09 for ITS 0183077 
#define AVP_SETTING_DEBUG_FILE_FLIP     "/app/data/AppMedia/flip" 
#define AVP_SETTING_DEBUG_FILE_FLOP     "/app/data/AppMedia/flop"
#ifdef AVP_BACKTRACE
#include <execinfo.h>
#include <time.h> // added by Michael.Kim 2014. 07.17 for timer log
#endif
// Watch Dog
AppMediaPlayer_EngineMain* AppMediaPlayer_EngineMain::m_pInstance = NULL;

// { added by kihyung 2013.1.9 for ISV 69132
extern SharedDataProvider *m_pSharedAudioData;
extern SharedDataProvider *m_pSharedPhotoData;
extern SharedDataProvider *m_pSharedVideoData;
// } added by kihyung 2013.1.9 for ISV 69132



AppMediaPlayer_EngineMain::AppMediaPlayer_EngineMain( LANGUAGE_T aLanguage, QTranslator &translator, int nVehicleType, QObject *pParent )
: AppVextEngineQML( false, "AppMediaPlayer", pParent ),
  m_AppFrontMode( APP_MEDIA_MODE_MAX ),
  m_AppRearMode( APP_MEDIA_MODE_MAX ),
  m_LastPlayerMode( APP_MEDIA_MODE_MAX ),
  m_AppFrontView( APP_MEDIA_MODE_MAX ),
  m_AppRearView( APP_MEDIA_MODE_MAX ),
  //m_AppFrontViewR( APP_MEDIA_MODE_MAX ), // remove by kihyung 2013.2.27
  //m_AppRearViewR( APP_MEDIA_MODE_MAX ), // remove by kihyung 2013.2.27
  m_Translator( translator ),
//  m_scrollingTicker(SettingsLoader::instance()->LoadSetting(68)), //Added by Alexey Edelev 2012.10.08
  m_scrollingTicker(true), 		//modified by aettie for ticker stop when DRS
  m_isFromOtherAV(false), //added by edo.lee 2013.01.08
  m_bCamInFG(false), // added by kihyung 2013.1.8
  m_bMediaOnFromCamOff(false),
  m_currentScreen(DISPLAY_NONE), // added by kihyung 2013.1.12
  //m_isRestored(false), //remove by edo.lee 2013.02.27
  m_bSwapEnabled(false), // added by kihyung 2013.1.24 for ISV 70304

  m_bIsFrontBlendOn(false), // added by shkim for DVD(Title)->Camera On Flickering Issue
  m_bIsRearBlendOn(false),  // added by shkim for DVD(Title)->Camera On Flickering Issue
  m_bFrontAVPIsForeground(false), //added by suryanto.tan 2013.3.1 for SANITY_CM_AI001
  m_eCloneStatus(CLONE_OFF), // added by kihyung 2013.1.24 for ISV 70304
  m_pTrackerAbs(NULL), // Added by kihyung 2013.2.17
  m_onBoot(true),//added by edo.lee 2013.02.28
  m_isNoneState(true),//added by edo.lee 2013.03.11
  m_isDiscEject(false),//added by edo.lee 2013.03.11
  m_isBgReceived(false),//added by edo.lee 2013.03.26
  m_isMuteState(false),//added by edo.lee 2013.04.01
  m_isBTCall(false), //added by edo.lee 2013.04.03
  m_isBLCall(false), //added by hyochang.ryu 20131017 for BL call
  //m_bFrontFGResponsed(true), // added by Sergey 16.04.2013 //youngsim.jo 20131014 is not used
  //m_bRearFGResponsed(true), // added by Sergey 16.04.2013 //youngsim.jo 20131014 is not used
  m_jukeboxCopyInProgress(false), // modified by ravikanth 16-04-13
  m_usbType(0), // added by eugene.seo 2013.05.29
  m_isMuteForSeek(false),//added by edo.lee 2013.04.21 for Mute
  m_isCloneTemp(false),//added by edo.lee 2013.04.21 for Mute
  m_bConnectAVP2UISH(false),
  m_isCompleteModeChange(true),// modified by Dmitry 05.05.13
  m_bMiddleEast(false), // added by Dmitry 05.05.13
  m_receivedTrackInfo(0),//added by edo.lee 2013.05.09
  m_nLastAVEntryPoint(eAPP_NONE),
  // { added by cychoi 2014.03.19 for ITS 230153
  m_nFrontAVPEntryPoint(eAPP_NONE),
  m_nRearAVPEntryPoint(eAPP_NONE),
  // } added by cychoi 2014.03.19
  m_bDisplayOff(false), // added by kihyung 2013.07.04 for ISV 86619
  m_bPowerOff(false),
  m_bAVOff(false),
  m_bFrontLCDOff(false),
  m_bMediaOnFromOff(false),  
  m_bACCOff(false),
  m_bDRSShow(false),
  m_bShowLoadingFlag(false),
  m_bTempMode(false),//added by edo.lee 2013.06.22
  m_bReleaseGstFromVideo(false),
  m_subscreenOn(false),//added by edo.lee 2013.07.30 subscreen
  m_lockoutOnDVDSetting(false),//added by edo.lee 2013.06.25
  m_unLockoutOnDVDSetting(false),//added by edo.lee 2013.06.25
  m_playOnBTCall(false), //added by edo.lee 2013.06.27 for BT CALL

  m_DiscInOutTestMode(false), //added by wonseok.heo 2013.07.04 disc in out test

  m_FRDRSViewLoading(false), //added by edo.lee 2013.07.03
  m_releasingClone(false), //added by edo.lee 2013.07.07
  m_PowerOffBTCall(false), //added by wonseok.heo for  ITS 178767 2013.07.10
  m_isTAOn(false), //added by sh.kim for ITS 179104
  m_isAuxDisconn(false),		//added by hyochang.ryu 20130719
  m_isTermOnTrackChange(false),
  m_bKeepPlayState(false),//added by edo.lee 2013.07.30
  m_launchSubscreen(false),
  m_bPlayAfterBTCallState(false),//Added by taihyun.ahn 2013.10.02  for ITS 188100
  m_accOffFromOn(false),//Added by taihyun.ahn 2013.10.07  for ITS 0180008
  m_bFgEventStart(false),//modified by edo.lee 2013.10.16 ITS 0196016
  m_btCallPowerOn(false),//Added by taihyun.ahn 2013.10.22
  m_nAcount(0),//added by sangmin.seol 2013.12.10 for ITS 0214221
  m_bBlockMouse(false) // added by Sergey 26.10.2013 for ITS#198513
 ,m_bVRStatus(false) // added by suilyou 20140106 ITS 0210488
#ifndef DUAL_KEY
  ,m_isHUPrePressed(false)
  ,m_isHUNextPressed(false)
#endif
#ifdef DUAL_KEY
  ,frontLCDMode(APP_MEDIA_MODE_MAX)
  ,rearLCDMode(APP_MEDIA_MODE_MAX)
   // { added by sangmin.seol 2014.02.26 DUAL_KEY Flag moved from appfreamework
  ,nCCPStatus(CCP_NONE)
  ,nRRCStatus(RRC_NONE)
  ,nETCStatus(ETC_NONE)
   // } added by sangmin.seol 2014.02.26 DUAL_KEY Flag moved from appfreamework
#endif
    //{ added by jaehwan 2014.04.03 to fix the issue that ipod icon is not activated after reboot.
   ,m_pendingIpodEnabled(false)
   ,m_iPodStrNodePath("")
   ,m_iPodStrSerial("")
   ,m_iPodEPortNum(eIPOD_PORT_NONE)
   ,m_iPodIsFirst(false)
    //} added by jaehwan
{
    Q_UNUSED (aLanguage);
    setMiddleEast(GetLanguage() == LANGUAGE_AR); // modified by Dmitry 11.05.13
    for(int i = 0; i < MEDIA_SOURCE_MAX; i++) {
        m_pIsSendEnableMedia2UISH[i] = false;
    }
    m_pIsSendIPODMounted2UISH[0] = false;
    m_pIsSendIPODMounted2UISH[1] = false;
    m_pIsSendIPODRemoved2UISH[0] = 0;
    m_pIsSendIPODRemoved2UISH[1] = 0;
    
    // { added by kihyung 2013.2.17
    m_bUSB1TAScanComplete = false;
    m_bUSB2TAScanComplete = false;   
    m_bJukeboxTAScanComplete = false; 
    m_bUSB1TAReadComplete = false;
    m_bUSB2TAReadComplete = false;   
    m_bJukeboxTAReadComplete = false; 
    m_bRequestedJukeboxRescan = false;
    // } added by kihyung 2012.2.17
    m_LastFrontEntryPoint = eAPP_NONE;
	m_LastRearEntryPoint = eAPP_NONE;

	m_isChangeToParking = false; //added by edo.lee 2013.09.04 ITS 0188044
	m_isChangeToDriving = false;//added by edo.lee 2013.09.04 ITS 0188044

    //{ added by yongkyun.lee 20130702 for : NO CR - None AV OSD
    m_isNotifyAVMode = MODE_STATE_NONE;
    //} added by yongkyun.lee 20130702 
    m_isKeepSystemMute = false;//added by edo.lee 2013.09.05 0188286
    m_KeyPressed = PRESSED_MAX;

    memset((void*)m_pEnableData, 0x00, sizeof(EVT_DEVICE_ENABLE_DATA_T) * MEDIA_SOURCE_MAX); // added by kihyung 2013.05.31
    #ifdef BOOT_LOADING_SCREEN
    memset(&m_tBootInfo,0,sizeof(m_tBootInfo)); //ys-20131010 boot isnotlast
    #endif
    m_pSharedAudioData = new SharedDataProvider(MEDIAPLAYER_SHARED_MEMORY_KEY);
    m_pSharedPhotoData = new SharedDataProvider(PHOTOVIEWER_SHARED_MEMORY_KEY);
    m_pSharedVideoData = new SharedDataProvider(VIDEOPLAYER_SHARED_MEMORY_KEY);
    m_pSharedFMData = new SharedDataProvider(FILEMANAGER_SHARED_MEMORY_KEY); // modified by ravikanth for ITS 0186965

    m_pSharedAudioData->SetStatusDateByIndex (DATA_TYPE_RANDOM, STATUS_OFF);
    m_pSharedAudioData->SetStatusDateByIndex (DATA_TYPE_REPEAT, STATUS_OFF);
    m_pSharedVideoData->SetStatusDateByIndex (DATA_TYPE_RANDOM, STATUS_OFF);
    m_pSharedVideoData->SetStatusDateByIndex (DATA_TYPE_REPEAT, STATUS_OFF);
    m_pSharedAudioData->SetStatusDateByIndex (DATA_TYPE_RANDOM_ENABLE, ENABLE); //added by edo.lee 2013.05.06 for VR random disable

    m_pMainUtil     = AppMediaPlayer_MainUtils::GetInstance(); // added by Sergey for SwapDC
    m_Controllers   = new AppMediaPlayer_Controllers(nVehicleType);
    m_pMediaManager = new AppMediaPlayer_MediaManager(*this, pParent); // modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot

    qRegisterMetaType<APP_MEDIA_MODE_T>("APP_MEDIA_MODE_T");
    m_pBootstrap = new AppMediaPlayer_Bootstrap( this, this );
    connect(m_pBootstrap, SIGNAL(componentLoaded(int,int)),  this, SLOT(componentLoaded(int,int)));
    m_Engines[ APP_MEDIA_MODE_AUDIO ]       = new AppMediaPlayer_EngineAudio( *this );
    m_Engines[ APP_MEDIA_MODE_VIDEO ]       = new AppMediaPlayer_EngineVideo( *this );
    m_Engines[ APP_MEDIA_MODE_PHOTO_FRONT ] = new AppMediaPlayer_EnginePhoto( *this, true );
    m_Engines[ APP_MEDIA_MODE_PHOTO_REAR ]  = new AppMediaPlayer_EnginePhoto( *this, false );
    // A little magic to make it possible to display graphics on qml...
    qmlRegisterType<GraphicsOutput>("GraphicsOutput", 1, 0, "GraphicsOutput");

    m_pNotifier        = new ExternalNotifier( this );
    m_pEngModeNotifier = new EngModeNotifier();
    // ((AppMediaPlayer_Controller_Audio*)m_Engines[ APP_MEDIA_MODE_AUDIO ]->GetController())->requestContentSize(MP::JUKEBOX); // removed by kihyung 2013.2.17
    // } modified by kihyung 2013.1.9

    //((AppMediaPlayer_Controller_Audio*)m_Engines[APP_MEDIA_MODE_AUDIO]->GetController())->pauseMiner(); // removed by eugene.seo 2013.03.16

    // { added by kihyung 2013.3.18
    // Watch Dog
#ifdef WATCHDOG
    signalConnection();
    setSignalHandlerObject(this);
#endif
    // } added by kihyung 2013.3.18    

	connectImageEventToUISH();//added by edo.lee 2013.04.09
    // added by kihyung 2013.3.26
    // connect(&m_tUSB1OutTimer, SIGNAL(timeout()), this, SLOT(handleUSB1OutTimer()));
    // connect(&m_tUSB2OutTimer, SIGNAL(timeout()), this, SLOT(handleUSB2OutTimer()));
    // added by kihyung 2013.3.26    
    // { added by wspark 2013.04.10 for cluster update
    m_ClusterUpdateTimer.setSingleShot( true );
    m_ClusterUpdateTimer.setInterval( 1000 );
    connect( &m_ClusterUpdateTimer, SIGNAL(timeout()), this, SLOT(onClusterUpdateTimer()) );
    // } added by wspark

	//remove by edo.lee 2013.11.08
	

	// removed by Sergey 07.09.2013 for PRE_FG supoort
    
	//added by edo.lee 2013.07.07
    m_releaseCloneTimer.setSingleShot(true);
    m_releaseCloneTimer.setInterval(200);
    connect(&m_releaseCloneTimer, SIGNAL(timeout()), this, SLOT(onShowOSDReleaseClone()));

    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    m_tDeckErrorTimer.setSingleShot(true);
    m_tDeckErrorTimer.setInterval(5000);
    connect(&m_tDeckErrorTimer, SIGNAL(timeout()), this, SLOT(onSendDeckErrorToUISH()));
    m_nDeckLastError = PLAYER_NO_ERROR;
    // } added by cychoi 2014.04.09

#if (USE_AVP_EVENT_FILTER == 1)
    connect(this, SIGNAL(sendToEventFilter(EVENT_DISPLAY)), &m_EventFilter, SLOT(onEnqueue(EVENT_DISPLAY)));   // { added by sungha.choi 2013.08.07 for ITS 0181470
    connect(&m_EventFilter, SIGNAL(sendEventForExecute(EVENT_DISPLAY)), this, SLOT(onRecvEventFromEventFilter(EVENT_DISPLAY)));   // } added by sungha.choi 2013.08.07 for ITS 0181470
#endif

    //AUXInit();		//added by hyochang.ryu 20130627

    // { added by kihyung 2013.08.09 for ITS 0183077 
    int nFlipFlop = 0;
    QFileInfo tFlip(AVP_SETTING_DEBUG_FILE_FLIP);
    if(tFlip.exists() == true) {
        QFile::remove(AVP_SETTING_DEBUG_FILE_FLIP);
        MP_LOG << "FLIP OK" << LOG_ENDL;
    }
    else {
        MP_LOG << "FLIP NG" << LOG_ENDL;
    }

    QFileInfo tFlop(AVP_SETTING_DEBUG_FILE_FLOP);
    if(tFlop.exists() == true) {
        QFile::remove(AVP_SETTING_DEBUG_FILE_FLOP);
        MP_LOG << "FLOP OK" << LOG_ENDL;
    }
    else {
        MP_LOG << "FLOP NG" << LOG_ENDL;
    }
    // } added by kihyung 2013.08.09 for ITS 0183077 

    //{removed by junam 2013.10.02
    //ConnectToUISH(); //2013.09.26 add by youngsim.jo
    //m_bConnectAVP2UISH = true; //2013.09.26 add by youngsim.jo
    //}removed by junam
#ifdef BOOT_LOADING_SCREEN
    connect(this,SIGNAL(isReadytoPlay()),this,SLOT(ConnectAVP2UISHReadyToPlay())); //ys-20131008 boot
    connect(m_pMediaManager,SIGNAL(isReadytoPlay()),this,SLOT(ConnectAVP2UISHReadyToPlay())); //ys-20131011 disc boot
    connect(m_Engines[APP_MEDIA_MODE_VIDEO],SIGNAL(isReadytoPlay()),this,SLOT(ConnectAVP2UISHReadyToPlay()));
    connect(m_Engines[APP_MEDIA_MODE_AUDIO],SIGNAL(isReadytoPlay()),this,SLOT(ConnectAVP2UISHReadyToPlay()));

    m_qBootTimer = new QTimer(this);
    connect(m_qBootTimer,SIGNAL(timeout()),this,SLOT(ConnectAVP2UISHTimeOut()));
    m_qBootTimer->setSingleShot(true);
    m_qBootTimer->setInterval(4000);
#endif
    
    //{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
    m_BTPlayUnmuteTimer = new QTimer(this);
    connect(m_BTPlayUnmuteTimer,SIGNAL(timeout()),this,SLOT(onBTPlayUnmuteTimer()));
    m_BTPlayUnmuteTimer->setSingleShot(true);
    m_BTPlayUnmuteTimer->setInterval(500);
    //{ modified by yongkyun.lee 2014-03-13
}

//{ added by hyochang.ryu 20130627
//bool isFirst = true;
void AppMediaPlayer_EngineMain::AUXInit(MODE_STATE_T state)
{
    //if(!isFirst) return;
    //isFirst = false;
    MP_LOG << LOG_ENDL;
    //if (m_device==MEDIA_SOURCE_AUX1||m_device==MEDIA_SOURCE_AUX2)
    {
        switch(state)
        {
            case MODE_STATE_AUX1_AUDIO:
                if(m_pMediaManager->IsSourceAvailable(MEDIA_SOURCE_AUX1, MEDIA_TYPE_MUSIC) == false)
                    m_pMediaManager->onAUXConnected(MEDIA_TYPE_MUSIC);
                break;
            case MODE_STATE_AUX2_AUDIO:
                if(m_pMediaManager->IsSourceAvailable(MEDIA_SOURCE_AUX2, MEDIA_TYPE_MUSIC) == false)
                    m_pMediaManager->onAUX2Connected(MEDIA_TYPE_MUSIC);
                break;
            case MODE_STATE_AUX1_VIDEO:
                if(m_pMediaManager->IsSourceAvailable(MEDIA_SOURCE_AUX1, MEDIA_TYPE_VIDEO) == false)
                    m_pMediaManager->onAUXConnected(MEDIA_TYPE_VIDEO);
                break;
            case MODE_STATE_AUX2_VIDEO:
                if(m_pMediaManager->IsSourceAvailable(MEDIA_SOURCE_AUX2, MEDIA_TYPE_VIDEO) == false)
                    m_pMediaManager->onAUX2Connected(MEDIA_TYPE_VIDEO);
                break;
            // { added by cychoi 2013.08.20 for ITS 178895 Make compiler happy
            default: // make compiler happy
                break;
            // } added by cychoi 2013.08.20
        }
    }
}
//} added by hyochang.ryu 20130627

AppMediaPlayer_EngineMain::~AppMediaPlayer_EngineMain()
{
    delete m_pNotifier;

    for ( unsigned index = 0; index < APP_MEDIA_MODE_MAX; index++ )
    {
       delete m_Engines[ index ];
    }

    delete m_pSharedFMData;
    delete m_pMediaManager;
    delete m_pEngModeNotifier;
    delete m_Controllers;     // Modified by Radhakrushna 20121012 NO_CR

}
#ifdef WATCHDOG
void AppMediaPlayer_EngineMain::signalHandelr(int signum)
{
    MP_LOG << "Signum = " << signum << LOG_ENDL;
#ifdef AVP_LAST_AUDIOFILE_BACKUP
    m_pInstance->backupLastAudioFile();
#endif
#ifdef AVP_BACKTRACE
    m_pInstance->avpBackTrace(signum);
#endif
    m_pInstance->restart(signum);
    raise(signum);
}

void AppMediaPlayer_EngineMain::setSignalHandlerObject(AppMediaPlayer_EngineMain *m)
{
    m_pInstance = m;
}

// Watch Dog
void AppMediaPlayer_EngineMain::signalConnection()
{
    struct sigaction act;
    memset(&act,0x00,sizeof(act)); //added by edo.lee 2013.07.15 STATIC TEST
	
    act.sa_flags    = 0;
    act.sa_flags   |= SA_RESETHAND;//SA_RESTART;
    act.sa_handler  = AppMediaPlayer_EngineMain::signalHandelr;
    sigemptyset(&act.sa_mask);  //added by aettie 2013.04.03 for static defect 20375

    for( int signumber = SIGHUP ; signumber<SIGUNUSED ; signumber++)
    {
        if ( signumber == SIGKILL || signumber == SIGSTOP || signumber == SIGCHLD) continue; //modify by youngsim.jo 2013-11-18 ITS 0209718,0209677,0209164
        if(sigaction(signumber, &act, (struct sigaction *)NULL)<0)
        {
            MP_LOG << "Sigaction failnum =" << signumber << LOG_ENDL;
            //exit(1); //modify by youngsim.jo 2013-11-18 ITS 0209718,0209677,0209164
        }
    }
}

void AppMediaPlayer_EngineMain::restart(int signum)
{
    Event event( EVT_REQUEST_ABNORMAL_TERMINATION, GetThisAppId(), APP_UI_MANAGER, (EVT_COMMAND_T)signum);//DISPLAY_FRONT);
    if(!NotifyUISH(event, DISPLAY_FRONT))
    {
        MP_LOG << "AppMediaPlayer_EngineMain::restart: Event notification failed" << LOG_ENDL;
    }
    //exit(0);
}
#endif


// { modified by Sergey 07.09.2013 for PRE_FG supoort
void AppMediaPlayer_EngineMain::HandleGstPipelinesAvailable(int nGpType)
{
    AppMediaPlayer_EngineVideo *pVideoEngine = NULL;
    pVideoEngine =  dynamic_cast<AppMediaPlayer_EngineVideo*>(GetEngine(APP_MEDIA_MODE_VIDEO));
    if(pVideoEngine != NULL)
        pVideoEngine->handleGstPipelinesAvailable((DISPLAY_T)nGpType);
        //dynamic_cast<AppMediaPlayer_EngineVideo*>(GetEngine(APP_MEDIA_MODE_VIDEO))->handleGstPipelinesAvailable((DISPLAY_T)nGpType);
}
// } modified by Sergey 07.09.2013 for PRE_FG supoort


#if (USE_AVP_EVENT_FILTER == 1)   // added by sungha.choi 2013.08.07 for ITS 0181470
void AppMediaPlayer_EngineMain::onRecvEventFromEventFilter(EVENT_DISPLAY evt)   // { added by sungha.choi 2013.08.07 for ITS 0181470
{
    Event aEvent = evt.m_event;
    DISPLAY_T display = evt.m_display;

    if(display == DISPLAY_COMMON)
    {
        OSDManager::instance()->preHandleUishEvent(aEvent);
        NotifyFromUISHCommon(aEvent);
        OSDManager::instance()->postHandleUishEvent(aEvent);
    }
    else if(display == DISPLAY_FRONT)
    {
        OSDManager::instance()->preHandleUishEvent(aEvent);
#if USE_AFW_SWAP_DC
        NotifyFromUISHDisplay(aEvent, DISPLAY_FRONT);
#else
        NotifyFromUISHCommon(aEvent);
#endif
        OSDManager::instance()->postHandleUishEvent(aEvent);
    }
    else if(display == DISPLAY_REAR)
    {
        OSDManager::instance()->preHandleUishEvent(aEvent, DISPLAY_REAR);
        NotifyFromUISHDisplay(aEvent, DISPLAY_REAR);
        OSDManager::instance()->postHandleUishEvent(aEvent, DISPLAY_REAR);
    }
}   // } added by sungha.choi 2013.08.07 for ITS 0181470
#endif

// { modified by Sergey for SwapDC (customization)
void AppMediaPlayer_EngineMain::NotifyFromUIStateManagerCommon(Event aEvent)
{
	MP_LOG << LOG_ENDL;
#ifdef DUAL_KEY
    if(aEvent.type() == QEvent::MouseButtonPress)
    {
        MP_LOG << "DUAL_KEY Touch Pressed !!!!!!!!!!!!!!!!1" << LOG_ENDL;
    }
#endif

#if (USE_AVP_EVENT_FILTER == 1)   // added by sungha.choi 2013.08.07 for ITS 0181470
        EVENT_DISPLAY new_event(aEvent, DISPLAY_COMMON);
        emit sendToEventFilter(new_event);
#else   // deleted by sungha.choi 2013.08.07 for ITS 0181470
	OSDManager::instance()->preHandleUishEvent(aEvent);
    NotifyFromUISHCommon(aEvent);
	OSDManager::instance()->postHandleUishEvent(aEvent);
#endif
}

void AppMediaPlayer_EngineMain::NotifyFromUIStateManager(Event aEvent)
{
	MP_LOG << LOG_ENDL;

#if (USE_AVP_EVENT_FILTER == 1)   // added by sungha.choi 2013.08.07 for ITS 0181470
        EVENT_DISPLAY new_event(aEvent, DISPLAY_FRONT);
        emit sendToEventFilter(new_event);
#else   // deleted by sungha.choi 2013.08.07 for ITS 0181470
	OSDManager::instance()->preHandleUishEvent(aEvent);
#if USE_AFW_SWAP_DC
    NotifyFromUISHDisplay(aEvent, DISPLAY_FRONT);
#else
    NotifyFromUISHCommon(aEvent);
#endif
	OSDManager::instance()->postHandleUishEvent(aEvent);
#endif
}

void AppMediaPlayer_EngineMain::NotifyFromUIStateManagerRear( Event aEvent )
{
	MP_LOG << LOG_ENDL;

#if (USE_AVP_EVENT_FILTER == 1)   // added by sungha.choi 2013.08.07 for ITS 0181470
        EVENT_DISPLAY new_event(aEvent, DISPLAY_REAR);
        emit sendToEventFilter(new_event);
#else   // deleted by sungha.choi 2013.08.07 for ITS 0181470
	OSDManager::instance()->preHandleUishEvent(aEvent, DISPLAY_REAR);
    NotifyFromUISHDisplay(aEvent, DISPLAY_REAR);
	OSDManager::instance()->postHandleUishEvent(aEvent, DISPLAY_REAR);
#endif
}
// } modified by Sergey for SwapDC (customization)

void AppMediaPlayer_EngineMain::NotifyFromUISHCommon(Event aEvent) // modified by Sergey for SwapDC (customization)
{
#ifdef DUAL_KEY
    cancelPressedKey(aEvent);
    KEY_EVENT_DATA_T *dd = (KEY_EVENT_DATA_T*)aEvent.Data().data();
    MP_LOG << "DUAL_KEY KeyID = " << aEvent.GetEventId()  << " key status = " << dd->keyStatus << LOG_ENDL;
    switch(dd->keyStatus)
    {
        case KEY_STATUS_PRESSED:
        {
            switch(aEvent.GetEventId())
            {
                case EVT_KEY_HU_SEEK_PREV:
                    setETCPressed(ETC_FACE_REW);
                    break;
                case EVT_KEY_SWRC_SEEK_PREV:
                    setETCPressed(ETC_SWRC_REW);
                    break;
                case EVT_KEY_HU_SEEK_NEXT:
                    setETCPressed(ETC_FACE_FF);
                    break;
                case EVT_KEY_SWRC_SEEK_NEXT:
                    setETCPressed(ETC_SWRC_FF);
                    break;
                case EVT_KEY_HU_TUNE_PRESSED:
                    setETCPressed(ETC_FACE_TUNE_CENTER);
                    break;
                // { Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
                case EVT_KEY_CCP_SEEK_PREV:
                    setETCPressed(ETC_CCP_SEEK_REW);
                    break;
                case EVT_KEY_CCP_SEEK_NEXT:
                    setETCPressed(ETC_CCP_SEEK_FF);
                    break;
                // } Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
                // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                //case EVT_KEY_RRC_CH_BK:
                //    setETCPressed(ETC_RRC_CH_BK);
                //    break;
                //case EVT_KEY_RRC_CH_FW:
                //    setETCPressed(ETC_RRC_CH_FW);
                //   break;
                // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                case EVT_KEY_RRC_SEEK_PREV:
                    setETCPressed(ETC_RRC_SEEK_REW);
                    break;
                case EVT_KEY_RRC_SEEK_NEXT:
                    setETCPressed(ETC_RRC_SEEK_FF);
                    break;
                case EVT_KEY_CCP_BACK:
                    setCCPPressed(CCP_BACK);
                    break;
                case EVT_KEY_CCP_MENU:
                    setCCPPressed(CCP_MENU);
                    break;
                case EVT_KEY_CCP_JOG_DIAL_CENTER:
                    setCCPPressed(CCP_CENTER);
                    break;
                case EVT_KEY_RRC_BACK:
                    setRRCPressed(RRC_BACK);
                    break;
                case EVT_KEY_RRC_MENU:
                    setRRCPressed(RRC_MENU);
                    break;
                case EVT_KEY_RRC_JOG_DIAL_CENTER:
                    setRRCPressed(RRC_CENTER);
                    break;
                case EVT_KEY_CCP_JOG_DIAL_LEFT:
                    setCCPPressed(CCP_LEFT);
                    break;
                case EVT_KEY_CCP_JOG_DIAL_RIGHT:
                    setCCPPressed(CCP_RIGHT);
                    break;
                case EVT_KEY_CCP_JOG_DIAL_UP:
                    setCCPPressed(CCP_UP);
                    break;
                case EVT_KEY_CCP_JOG_DIAL_DOWN:
                    setCCPPressed(CCP_DOWN);
                    break;
                case EVT_KEY_RRC_JOG_DIAL_LEFT:
                    setRRCPressed(RRC_LEFT);
                    break;
                case EVT_KEY_RRC_JOG_DIAL_RIGHT:
                    setRRCPressed(RRC_RIGHT);
                    break;
                case EVT_KEY_RRC_JOG_DIAL_UP:
                    setRRCPressed(RRC_UP);
                    break;
                case EVT_KEY_RRC_JOG_DIAL_DOWN:
                    setRRCPressed(RRC_DOWN);
                    break;
                }
            break;
        }
        case KEY_STATUS_RELEASED:
        {
            switch(aEvent.GetEventId())
            {
                case EVT_KEY_HU_SEEK_PREV:
                    if(checkValidKey(aEvent.GetEventId()))
                        setETCReleased(ETC_FACE_REW);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_SWRC_SEEK_PREV:
                    if(checkValidKey(aEvent.GetEventId()))
                        setETCReleased(ETC_SWRC_REW);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_HU_SEEK_NEXT:
                    if(checkValidKey(aEvent.GetEventId()))
                        setETCReleased(ETC_FACE_FF);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_SWRC_SEEK_NEXT:
                    if(checkValidKey(aEvent.GetEventId()))
                        setETCReleased(ETC_SWRC_FF);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_HU_TUNE_PRESSED:
                    if(checkValidKey(aEvent.GetEventId()))
                        setETCReleased(ETC_FACE_TUNE_CENTER);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                // { Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
                case EVT_KEY_CCP_SEEK_PREV:
                    if(checkValidKey(aEvent.GetEventId()))
                        setETCReleased(ETC_CCP_SEEK_REW);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_CCP_SEEK_NEXT:
                    if(checkValidKey(aEvent.GetEventId()))
                        setETCReleased(ETC_CCP_SEEK_FF);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                // } Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
                // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                //case EVT_KEY_RRC_CH_BK:
                //    if(checkValidKey(aEvent.GetEventId()))
                //        setETCReleased(ETC_RRC_CH_BK);
                //    else
                //    {
                //        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                //        return;
                //    }
                //    break;
                //case EVT_KEY_RRC_CH_FW:
                //    if(checkValidKey(aEvent.GetEventId()))
                //        setETCReleased(ETC_RRC_CH_FW);
                //    else
                //    {
                //        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                //        return;
                //    }
                //    break;
                // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                case EVT_KEY_RRC_SEEK_PREV:
                    if(checkValidKey(aEvent.GetEventId()))
                        setETCReleased(ETC_RRC_SEEK_REW);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_RRC_SEEK_NEXT:
                    if(checkValidKey(aEvent.GetEventId()))
                        setETCReleased(ETC_RRC_SEEK_FF);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_CCP_BACK:
                    if(checkValidKey(aEvent.GetEventId()))
                        setCCPReleased(CCP_BACK);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_CCP_MENU:
                    if(checkValidKey(aEvent.GetEventId()))
                        setCCPReleased(CCP_MENU);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_CCP_JOG_DIAL_CENTER:
                    if(checkValidKey(aEvent.GetEventId()))
                        setCCPReleased(CCP_CENTER);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_RRC_BACK:
                    if(checkValidKey(aEvent.GetEventId()))
                        setRRCReleased(RRC_BACK);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_RRC_MENU:
                    if(checkValidKey(aEvent.GetEventId()))
                        setRRCReleased(RRC_MENU);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_RRC_JOG_DIAL_CENTER:
                    if(checkValidKey(aEvent.GetEventId()))
                        setRRCReleased(RRC_CENTER);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_CCP_JOG_DIAL_LEFT:
                    if(checkValidKey(aEvent.GetEventId()))
                        setCCPReleased(CCP_LEFT);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_CCP_JOG_DIAL_RIGHT:
                    if(checkValidKey(aEvent.GetEventId()))
                        setCCPReleased(CCP_RIGHT);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_CCP_JOG_DIAL_UP:
                    if(checkValidKey(aEvent.GetEventId()))
                        setCCPReleased(CCP_UP);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_CCP_JOG_DIAL_DOWN:
                    if(checkValidKey(aEvent.GetEventId()))
                        setCCPReleased(CCP_DOWN);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_RRC_JOG_DIAL_LEFT:
                    if(checkValidKey(aEvent.GetEventId()))
                        setRRCReleased(RRC_LEFT);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_RRC_JOG_DIAL_RIGHT:
                    if(checkValidKey(aEvent.GetEventId()))
                        setRRCReleased(RRC_RIGHT);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_RRC_JOG_DIAL_UP:
                    if(checkValidKey(aEvent.GetEventId()))
                        setRRCReleased(RRC_UP);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                case EVT_KEY_RRC_JOG_DIAL_DOWN:
                    if(checkValidKey(aEvent.GetEventId()))
                        setRRCReleased(RRC_DOWN);
                    else
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Release" << LOG_ENDL;
                        return;
                    }
                    break;
                default:
                    break;
            }
            break;
        }
        case KEY_STATUS_PRESSED_LONG:
        {
            switch(aEvent.GetEventId())
            {
                case EVT_KEY_HU_SEEK_PREV:
                case EVT_KEY_SWRC_SEEK_PREV:
                case EVT_KEY_HU_SEEK_NEXT:
                case EVT_KEY_SWRC_SEEK_NEXT:
                case EVT_KEY_HU_TUNE_PRESSED:
                case EVT_KEY_CCP_SEEK_PREV:         // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
                case EVT_KEY_CCP_SEEK_NEXT:         // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
                //case EVT_KEY_RRC_CH_BK:             // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                //case EVT_KEY_RRC_CH_FW:             // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                case EVT_KEY_RRC_SEEK_PREV:
                case EVT_KEY_RRC_SEEK_NEXT:
                case EVT_KEY_CCP_BACK:
                case EVT_KEY_CCP_MENU:
                case EVT_KEY_CCP_JOG_DIAL_CENTER:
                case EVT_KEY_RRC_BACK:
                case EVT_KEY_RRC_MENU:
                case EVT_KEY_RRC_JOG_DIAL_CENTER:
                case EVT_KEY_CCP_JOG_DIAL_LEFT:
                case EVT_KEY_CCP_JOG_DIAL_RIGHT:
                case EVT_KEY_CCP_JOG_DIAL_UP:
                case EVT_KEY_CCP_JOG_DIAL_DOWN:
                case EVT_KEY_RRC_JOG_DIAL_LEFT:
                case EVT_KEY_RRC_JOG_DIAL_RIGHT:
                case EVT_KEY_RRC_JOG_DIAL_UP:
                case EVT_KEY_RRC_JOG_DIAL_DOWN:

                    if(!checkValidKey(aEvent.GetEventId()))
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Long Press" << LOG_ENDL;
                        return;
                    }
                    break;
                default:
                    break;
            }
            break;
        }
        case KEY_STATUS_PRESSED_CRITICAL:
        {
            switch(aEvent.GetEventId())
            {
                case EVT_KEY_HU_SEEK_PREV:
                case EVT_KEY_SWRC_SEEK_PREV:
                case EVT_KEY_HU_SEEK_NEXT:
                case EVT_KEY_SWRC_SEEK_NEXT:
                case EVT_KEY_HU_TUNE_PRESSED:
                case EVT_KEY_CCP_SEEK_PREV:         // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
                case EVT_KEY_CCP_SEEK_NEXT:         // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
                //case EVT_KEY_RRC_CH_BK:             // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                //case EVT_KEY_RRC_CH_FW:             // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                case EVT_KEY_RRC_SEEK_PREV:
                case EVT_KEY_RRC_SEEK_NEXT:
                case EVT_KEY_CCP_BACK:
                case EVT_KEY_CCP_MENU:
                case EVT_KEY_CCP_JOG_DIAL_CENTER:
                case EVT_KEY_RRC_BACK:
                case EVT_KEY_RRC_MENU:
                case EVT_KEY_RRC_JOG_DIAL_CENTER:
                case EVT_KEY_CCP_JOG_DIAL_LEFT:
                case EVT_KEY_CCP_JOG_DIAL_RIGHT:
                case EVT_KEY_CCP_JOG_DIAL_UP:
                case EVT_KEY_CCP_JOG_DIAL_DOWN:
                case EVT_KEY_RRC_JOG_DIAL_LEFT:
                case EVT_KEY_RRC_JOG_DIAL_RIGHT:
                case EVT_KEY_RRC_JOG_DIAL_UP:
                case EVT_KEY_RRC_JOG_DIAL_DOWN:
                    if(!checkValidKey(aEvent.GetEventId()))
                    {
                        MP_LOG << "DUAL_KEY Ignored Invalid Critical Press" << LOG_ENDL;
                        return;
                    }
                    break;
                default:
                    break;
            }
            break;
        }
        case KEY_STATUS_CANCEL:
        {
            MP_LOG << "DUAL_KEY " << aEvent.GetEventId() << "Canceled" << LOG_ENDL;
            switch(aEvent.GetEventId())
            {
                case EVT_KEY_HU_SEEK_PREV:
                    setETCReleased(ETC_FACE_REW);
                    break;
                case EVT_KEY_SWRC_SEEK_PREV:
                    setETCReleased(ETC_SWRC_REW);
                    break;
                case EVT_KEY_HU_SEEK_NEXT:
                    setETCReleased(ETC_FACE_FF);
                    break;
                case EVT_KEY_SWRC_SEEK_NEXT:
                    setETCReleased(ETC_SWRC_FF);
                    break;
                case EVT_KEY_HU_TUNE_PRESSED:
                    setETCReleased(ETC_FACE_TUNE_CENTER);
                    break;
                // { Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
                case EVT_KEY_CCP_SEEK_PREV:
                    setETCReleased(ETC_CCP_SEEK_REW);
                    break;
                case EVT_KEY_CCP_SEEK_NEXT:
                    setETCReleased(ETC_CCP_SEEK_FF);
                    break;
                // } Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
                // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                //case EVT_KEY_RRC_CH_BK:
                //    setETCReleased(ETC_RRC_CH_BK);
                //    break;
                //case EVT_KEY_RRC_CH_FW:
                //    setETCReleased(ETC_RRC_CH_FW);
                //    break;
                // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                case EVT_KEY_RRC_SEEK_PREV:
                    setETCReleased(ETC_RRC_SEEK_REW);
                    break;
                case EVT_KEY_RRC_SEEK_NEXT:
                    setETCReleased(ETC_RRC_SEEK_FF);
                    break;
                case EVT_KEY_CCP_BACK:
                    setCCPReleased(CCP_BACK);
                    break;
                case EVT_KEY_CCP_MENU:
                    setCCPReleased(CCP_MENU);
                    break;
                case EVT_KEY_CCP_JOG_DIAL_CENTER:
                    setCCPReleased(CCP_CENTER);
                    break;
                case EVT_KEY_RRC_BACK:
                    setRRCReleased(RRC_BACK);
                    break;
                case EVT_KEY_RRC_MENU:
                    setRRCReleased(RRC_MENU);
                    break;
                case EVT_KEY_RRC_JOG_DIAL_CENTER:
                    setRRCReleased(RRC_CENTER);
                    break;
                case EVT_KEY_CCP_JOG_DIAL_LEFT:
                    setCCPReleased(CCP_LEFT);
                    break;
                case EVT_KEY_CCP_JOG_DIAL_RIGHT:
                    setCCPReleased(CCP_RIGHT);
                    break;
                case EVT_KEY_CCP_JOG_DIAL_UP:
                    setCCPReleased(CCP_UP);
                    break;
                case EVT_KEY_CCP_JOG_DIAL_DOWN:
                    setCCPReleased(CCP_DOWN);
                    break;
                case EVT_KEY_RRC_JOG_DIAL_LEFT:
                    setRRCReleased(RRC_LEFT);
                    break;
                case EVT_KEY_RRC_JOG_DIAL_RIGHT:
                    setRRCReleased(RRC_RIGHT);
                    break;
                case EVT_KEY_RRC_JOG_DIAL_UP:
                    setRRCReleased(RRC_UP);
                    break;
                case EVT_KEY_RRC_JOG_DIAL_DOWN:
                    setRRCReleased(RRC_DOWN);
                    break;
                default:
                    break;
            }
            m_KeyPressed = PRESSED_MAX;
            break;
        }
    }
#endif
    /* Events which don't depend on Display ID are handled here. */

    switch( aEvent.GetEventId() )
    {
        case EVT_USB_MOUNTED:
        {
            MP_HIGH << "[EVT_USB_MOUNTED]" << LOG_ENDL;

            if ( sizeof(APP_EVENT_USB_DATA_T) == aEvent.Data().size() )
            {
                // { modified by kihyung 2013.07.31 for ISV 88437
                QString               strBootUSBUUID = AppMediaPlayer_SettingsStorage::GetInstance()->GetBootUSBUUID();
                USB_PORT_T            nBootUSBPort   = (USB_PORT_T)AppMediaPlayer_SettingsStorage::GetInstance()->GetBootUSBPort();
                APP_EVENT_USB_DATA_T *pEventData     = (APP_EVENT_USB_DATA_T *)aEvent.Data().data();
                
                if(pEventData->usbPort != eUSB_PORT_INVALID && nBootUSBPort == pEventData->usbPort && strBootUSBUUID == pEventData->UUID) 
                {
                    MP_HIGH << "AFTER BOOT, SAME BOOT USB... RETURN..." << LOG_ENDL;
                    AppMediaPlayer_SettingsStorage::GetInstance()->SetBootUSBPort((int)eUSB_PORT_INVALID);
                    AppMediaPlayer_SettingsStorage::GetInstance()->SetBootUSBUUID("");
                }
                else {
                    m_pMediaManager->onUSBDevConnected( pEventData->usbPort, pEventData->UUID, pEventData->mountPath);
                }
                // } modified by kihyung 2013.07.31 for ISV 88437
            }
            break;
        }

        case EVT_USB_REMOVED:
        {
            MP_HIGH << "[EVT_USB_REMOVED]" << LOG_ENDL;

            if ( sizeof(APP_EVENT_USB_DATA_T) == aEvent.Data().size() )
            {
                APP_EVENT_USB_DATA_T *pEventData = (APP_EVENT_USB_DATA_T *)aEvent.Data().data();
				//added by edo.lee 2013.05.18                
				if(pEventData->usbPort == m_usbType) // added by eugene.seo 2013.05.29
				{
					AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_Engines[APP_MEDIA_MODE_AUDIO]);
                    // { modified by cychoi 2013-07-15 for Prevent fix
                    if(pAudioEngine != NULL)
                        pAudioEngine->CancelAudioCopy(true);
                    // } modified by cychoi 2013-07-15
				}       

                m_pMediaManager->onUSBDevDisconnected( pEventData->usbPort );
            }

            break;
        }

        case EVT_IPOD_MOUNTED:
        {
            //{ modified by yongkyun.lee 2013-12-13 for : ITS 213657
            AppMediaPlayer_EngineAudio      *pEngineAudio = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
            AppMediaPlayer_Controller_Audio *pControler   = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();

            if(pControler != NULL)
                pControler->iPodMountedTimer();
            //} modified by yongkyun.lee 2013-12-13 

            /*
            MP_LOG << "[EVT_IPOD_MOUNTED]" << LOG_ENDL;

            //emit hidePopup(); // remove by eugene.seo 2012.10.16

            if ( sizeof(APP_EVENT_IPOD_DATA_T) == aEvent.Data().size() )
            {
                // { modified by kihyung 2012.07.09 for CR 10343
                // APP_EVENT_IPOD_DATA_T *pEventData = (APP_EVENT_IPOD_DATA_T *)aEvent.Data().data();
                 m_pNotifier->displayOSD(OSD_EVENT_IPOD_READING); //modified by edo.lee 2013.04.09
                // m_pMediaManager->onIPODConnected( pEventData->usbPort, pEventData->SerialNum, pEventData->mountPath, pEventData->contentmask );
                AppMediaPlayer_EngineAudio      *pEngineAudio = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
                AppMediaPlayer_Controller_Audio *pControler   = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();
                if(pControler->IsAppleConnectError() == false) {
                    // { modified by jungae for ISV63667
                    MP_LOG << "EVT_IPOD_MOUNTED: pControler()->IsAppleConnectError() is false" << LOG_ENDL;
                    //modified by suryanto.tan 2013.3.1 for SANITY_CM_AI001
                    if(!m_bFrontAVPIsForeground)
                    {
                    //if(GetFrontMode() != APP_MEDIA_MODE_AUDIO) {
                    //modified by suryanto.tan 2013.3.1 for SANITY_CM_AI001
                        //m_pNotifier->displayOSD(OSD_EVENT_IPOD_READING); //modified by edo.lee 2013.04.09
                        MP_LOG << "EVT_IPOD_MOUNTED: GetFrontMode is not APP_MEDIA_MODE_AUDIO" << LOG_ENDL;
                    }
                    else {
                        MP_LOG << "EVT_IPOD_MOUNTED: GetFrontMode is APP_MEDIA_MODE_AUDIO" << LOG_ENDL;
                    }// } modified by jungae
                }
                else {
                    MP_LOG << "EVT_IPOD_MOUNTED: pControler()->IsAppleConnectError() is true" << LOG_ENDL;
                }
                APP_EVENT_IPOD_DATA_T *pEventData = (APP_EVENT_IPOD_DATA_T *)aEvent.Data().data();
                m_pMediaManager->onIPODConnected( pEventData->usbPort, pEventData->SerialNum, pEventData->mountPath, pEventData->contentmask );
                // } modified by kihyung
            }
            */
            break;
        }

        case EVT_IPOD_REMOVED:
        {
            /*
            MP_LOG << "[EVT_IPOD_REMOVED]" << LOG_ENDL;

            if ( sizeof(APP_EVENT_IPOD_DATA_T) == aEvent.Data().size() )
            {
                APP_EVENT_IPOD_DATA_T *pEventData = (APP_EVENT_IPOD_DATA_T *)aEvent.Data().data();
                //{ Removed by Radhakrushna , Its handled in UISH . CR#13435 2012007
                // m_pNotifier->displayOSD(OSD_EVENT_IPOD_DISCONNECTED);
                //{ Removed by Radhakrushna , Its handled in UISH . CR#13435 2012007

                m_pMediaManager->onIPODDisconnected( pEventData->usbPort );
            }
            */
            break;
        }

        case EVT_AUX_PLUGEDIN:
        {
            MP_HIGH << "[EVT_AUX_PLUGEDIN]" << LOG_ENDL;
            if(sizeof(EVENT_APP_AUX_DATA) == aEvent.Data().size())
            {
                EVENT_APP_AUX_DATA eventData;
                memcpy(&eventData, aEvent.Data().data(), sizeof(EVENT_APP_AUX_DATA));

                if (eventData.contentType == eContent_AUDIO)
                {
                    // { modified by lssanh 2013.05.19 NoCR 
                    //m_pMediaManager->onAUXConnected(MEDIA_TYPE_MUSIC);
                    //if(eventData.Connected_Boot == false) GetNotifier()->displayOSD(OSD_EVENT_AUX_CONNECTED);
                    if(eventData.Connected_Boot == false && !m_isAuxDisconn)	// && m_pMediaManager->IsSourceAvailable(MEDIA_SOURCE_AUX1, MEDIA_TYPE_MUSIC) == false)
                    {
                        OSDManager::instance()->displayOSD(OSD_EVENT_AUX_CONNECTED);
                    }
                    OSDManager::instance()->setAUXDisconnect(false);		//added by hyochang.ryu 20131004 for ITS193801 //{ removed by shkim for ITS 205553 Aux OSD Issue
                    //OSDManager::instance()->checkAuxMode();		//added by hyochang.ryu 20131105 ITS 206819 //{ removed by shkim for ITS 0208482 Aux OSD Issue 2013.11.12
                    //{ modified by hyochang.ryu 20131004 
                    AUXInit(MODE_STATE_AUX1_AUDIO);
                    AUXInit(MODE_STATE_AUX2_AUDIO);
                    //} modified by hyochang.ryu 20131004 
                    // } modified by lssanh 2013.05.19 NoCR 					
					//MP_LOG << "EVT_AUX_PLUGEDIN" << eventData.contentType << "MUSIC AUX"  << LOG_ENDL;
                }
                if (eventData.contentType == eContent_VIDEO)	//modified by hyochang.ryu 20131004
                {
                    //{ modified by hyochang.ryu 20131004 
                    AUXInit(MODE_STATE_AUX1_VIDEO);
                    AUXInit(MODE_STATE_AUX2_VIDEO);
                    //} modified by hyochang.ryu 20131004 
					//GetNotifier()->displayOSD(OSD_EVENT_AUX_CONNECTED);
					//MP_LOG << "EVT_AUX_PLUGEDIN" << eventData.contentType << "VIDEO AUX"  << LOG_ENDL;
                }
                m_isAuxDisconn = false;
            }
            break;
        }

        case EVT_AUX_REMOVED:
        {
            MP_HIGH << "[EVT_AUX_REMOVED]" << LOG_ENDL;

            m_pMediaManager->onAUXDisconnected( );
            //TODO - onAUX2Disconnected( ) must be called on apropriate
            // event which actually does not exist no
            //m_pMediaManager->onAUX2Disconnected( );
            m_isAuxDisconn = true;
            break;
        }

        case EVT_LANGUAGE_CHANGED:
        {
            MP_LOG << "AppMediaPlayer_EngineMain::NotifyFromUIStateManager: EVT_LANGUAGE_CHANGED" << LOG_ENDL;

            EVENT_LANGUAGE_CHANGED_DATA_T *pLanguageData = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();

            if ( pLanguageData )
            {
                setMiddleEast(false); // added by Dmitry 11.05.13
				//aettie added counrty variant LANGUAGE_ES_NA / LANGUAGE_FR_NA / LANGUAGE_EN_US
                switch ( pLanguageData->languageId )
                {
                case LANGUAGE_KO:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_ko" );
                    break;
                case LANGUAGE_EN_US:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_en" );
                    break;
                case LANGUAGE_ES:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_es-eu" );
                    break;
                case LANGUAGE_FR:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_fr-eu" );
                    break;
                case LANGUAGE_ES_NA:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_es-na" );
                    break;
                case LANGUAGE_FR_NA:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_fr-na" );
                    break;
                case LANGUAGE_ZH_CN:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_zh-cmn" );
                    break;
                case LANGUAGE_ZH_YUE:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_zh-yue" );
                    break;
                case LANGUAGE_AR:
                    setMiddleEast(true); // added by Dmitry 11.05.13
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_ar" );
                    break;
                case LANGUAGE_IT:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_it" );
                    break;
                case LANGUAGE_RU:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_ru" );
                    break;
                case LANGUAGE_DE:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_ge" );
                    break;
                case LANGUAGE_PT:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_pt" );
                    break;
                case LANGUAGE_EN_GB:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_en-gb" );
                    break;
                case LANGUAGE_PL:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_pl" );
                    break;
                case LANGUAGE_CS:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_cs" );
                    break;
                case LANGUAGE_DA:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_da" );
                    break;
                case LANGUAGE_NL:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_nl" );
                    break;
                case LANGUAGE_SK:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_sk" );
                    break;
                case LANGUAGE_SV:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_sv" );
                    break;
                case LANGUAGE_TR:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_tr" );
                    break;
                default:
                    m_Translator.load( "/app/share/AppMediaPlayer/localization/MediaPlayer_en" );
                    break;
                }

                qApp->installTranslator( &m_Translator );
                // { added by jaehwan.lee for send languge settting to trackeabstractor
                MP_HIGH << "EVT_LANGUAGE_CHANGED  : languageId = "<<  pLanguageData->languageId << LOG_ENDL;
                if(m_pTrackerAbs) m_pTrackerAbs->SetLanguage( pLanguageData->languageId );

                AppMediaPlayer_EngineAudio      *pEngineAudio = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
                AppMediaPlayer_Controller_Audio *pController  = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();
                pController->GetTrackerAbstractor()->SetLanguage(pLanguageData->languageId ); 

                //{ added by jaehwan.lee 2014.2.21 to load pinyin table only for chiness.
                if (pLanguageData->languageId == LANGUAGE_ZH_CMN)
                {
                    MP_MEDIUM << "Language Changed, Load Pinyin Table!" << LOG_ENDL;
                    CTrackerAbstractor::LoadPinyinTable();
                }
                //} added by jaehwan.lee

                pController->GetListViewModel()->getTrackerAbstactor()->SetLanguage(pLanguageData->languageId );
                //refresh sort order
                pController->requestCovers();//true); //changed by junam 2013.11.06 for coverflow update
               // pController->updateCoverFlowMediaInfo(pController->getDevice(), true, true); 
                //}

                // FIXME: We need to emit this to all views

                // added by richard.dale 2012.11.03 for CR 14076
                LocalizeTrigger::self()->SetLanguage( pLanguageData->languageId );
                emit retranslateUi( pLanguageData->languageId );
            }
            break;
        }

        case EVT_KEY_HU_EJECT:
        {
            if ( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

                MP_LOG << "EVT_KEY_HU_EJECT: key status" << eventData->keyStatus << LOG_ENDL;

                emit scrollAccStop(); // added by lssanh 2013.02.26 ISV73837

                // { modified by cychoi 2013.05.13 - Please handle HK on Press if it is not handled on Long Press.
                if ( eventData->keyStatus == KEY_STATUS_PRESSED )
                //if ( eventData->keyStatus == KEY_STATUS_RELEASED )
                // } modified by cychoi 2013.05.13
                {
                    // { modified by kihyung 2012.07.27 for ITS 0136589
                    if(m_pMediaManager->IsDiscEjected() == true) {
                        // { modified by changjin 2012.12.14 for Cr 16130
                        if(m_pMediaManager->getDiscState() == PLAYER_DISCEJECTED)
                        {
                            m_pMediaManager->loadDisc();
                        }
                        else // PLAYER_DISCREMOVED
                        {
							OSDManager::instance()->displayOSD(OSD_EVENT_NO_INSERTED_DISC);
                        }
                        // } modified by changjin 2012.12.14
                    }
                    else{
						m_isDiscEject = true;//added by edo.lee 2013.03.11
                        m_pMediaManager->ejectDisc( false );
                    }
                }
                else if ( eventData->keyStatus == KEY_STATUS_PRESSED_LONG )
                {
                    // { commented by kihyung 2012.08.05
                    // HMI should not use the forced eject. The forced eject is perfomed by MICOM.
                    // Please use always normal eject.
                    // m_pMediaManager->ejectDisc( true );
                    // } commented by kihyung
                }
            }
            break;
        }

        case EVT_RESTORE_LAST_AV_MODE:
        {
            MP_HIGH << "EVT_RESTORE_LAST_AV_MODE" << LOG_ENDL;
            handleRestoreAVEvent(aEvent);
            break;
        }

        case EVT_REQUEST_SAVE_UNSAVED_DATA:
        {
            MP_HIGH << "EVT_REQUEST_SAVE_UNSAVED_DATA" << LOG_ENDL;

            // { added by honggi.shin, 2014.05.23, for finishing thumbnail operation.
            // 1. stop tracker mining
            AppMediaPlayer_EngineAudio      *pEngineAudio = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
            AppMediaPlayer_Controller_Audio *pControler   = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();
            pControler->pauseMiner();
            //2. stop photo thumbnail operation
            PhotoThumbnailer::setReadyToPWOFF(true);
            AppMediaPlayer_Controller_Photo::setReadyToPWOFF(true);
            MP_MEDIUM << "setReadyToPWOFF is true. photo thumbnail operation stoped." << LOG_ENDL;
            // } added by honggi.shin, 2014.05.23, END

            SaveSettings();

            //usleep(500 * 1000); // added by kihyung 2013.08.09 for ITS 0183077
            usleep(1000 * 1000);  //modified by honggi.shin, 2014.05.23, for finishing thumbnail operation.

            Event event( EVT_RESPONSE_SAVE_UNSAVED_DATA, GetThisAppId(), APP_UI_MANAGER );
            notifyUISH( event, DISPLAY_FRONT_REAR ); // added by Sergey for SwapDC
            break;
        }

        case EVT_FORMAT_JUKEBOX_START:
        {
            MP_HIGH << "EVT_FORMAT_JUKEBOX_START" << LOG_ENDL;

            // { added by kihyung 2013.10.01 for ITS 0189653
            m_pNotifier->UpdateSDPMediaCount(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_MUSIC, 0);
            m_pNotifier->UpdateSDPMediaCount(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_VIDEO, 0);
            m_pNotifier->UpdateSDPMediaCount(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_PHOTO, 0);
            // } added by kihyung 2013.10.01 for ITS 0189653
            
            emit formatting(true); // added by Dmitry 06.11.13 for ITS94041
            EventHandler(aEvent, DISPLAY_FRONT); // modified by kihyung 2013.1.29
            break;
        }

        case EVT_FORMAT_JUKEBOX_END:
        {
            MP_HIGH << "EVT_FORMAT_JUKEBOX_END" << LOG_ENDL;

            m_pMediaManager->SetSourceAvailable(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_MUSIC, false);
            m_pMediaManager->SetSourceAvailable(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_VIDEO, false);
            m_pMediaManager->SetSourceAvailable(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_PHOTO, false);
            //requestTrackerAbsRescan(eJukeBox,false);
            requestTrackerAbsRescan(eJukeBox,true); //modified by Michaele.Kim 2013.09.12 for ITS 189482
            emit formatting(false); // added by Dmitry 06.11.13 for ITS94041
            EventHandler(aEvent, DISPLAY_FRONT); // modified by kihyung 2013.1.29
            break;
        }
        case EVT_KEY_HU_JOG_DIAL_ENCODER:
        case EVT_KEY_HU_SEEK_PREV:
        case EVT_KEY_HU_SEEK_NEXT:
        case EVT_KEY_SWRC_SEEK_PREV:
        case EVT_KEY_SWRC_SEEK_NEXT:
        case EVT_KEY_HU_TUNE_PRESSED:
        {
            MP_LOG << "[Hard Key Event]:" << aEvent.GetEventId() << LOG_ENDL;
#ifndef DUAL_KEY
            //added by taihyun.ahn 20131104
            switch(aEvent.GetEventId())
            {
                case EVT_KEY_HU_SEEK_PREV:
                {
                    KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                    if(eventData->keyStatus == KEY_STATUS_PRESSED)
                        m_isHUPrePressed = true;
                    else if(eventData->keyStatus == KEY_STATUS_RELEASED)
                        m_isHUPrePressed = false;
                    break;
                }
                case EVT_KEY_HU_SEEK_NEXT:
                {
                    KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                    if(eventData->keyStatus == KEY_STATUS_PRESSED)
                        m_isHUNextPressed = true;
                    else if(eventData->keyStatus == KEY_STATUS_RELEASED)
                        m_isHUNextPressed = false;
                }
            }
#endif
            //added by suilyou 20130923 START
            switch(aEvent.GetEventId())
            {
                case EVT_KEY_HU_JOG_DIAL_ENCODER:
                case EVT_KEY_HU_SEEK_PREV:
                case EVT_KEY_SWRC_SEEK_PREV:
                case EVT_KEY_HU_SEEK_NEXT:
                case EVT_KEY_SWRC_SEEK_NEXT:
                {
                    //sendTouchCleanUpForApps(); //removed by suilyou 20131127 DUAL_KEY
                    switch(m_eCloneStatus)
                    {
                        case CLONE_OFF:
                        {
                            switch(m_bSwapEnabled)
                            {
                                case true:
                                {
                                    if(getJogKeyPressed()!=0||getSharedMemData()!=0)
                                    {
                                        MP_LOG << "EVT_KEY_HU_SEEK: JogKeyPressed key status" << getJogKeyPressed() << LOG_ENDL;
                                        return;
                                    }
                                    break;
                                }
                                case false:
                                {
                                    //{ modified by yongkyun.lee 2013-10-16 for : ITS 194947
                                    bool hwkey = true;
                                    if( (m_LastPlayerMode == APP_MEDIA_MODE_AUDIO /*|| m_LastPlayerMode ==  APP_MEDIA_MODE_VIDEO*/) &&
                                         aEvent.GetEventId() != EVT_KEY_HU_JOG_DIAL_ENCODER)
                                    {
                                        bool hwseek;
                                        AppMediaPlayer_EngineAudio * aEngine;
                                        aEngine =  (AppMediaPlayer_EngineAudio*) m_Engines[APP_MEDIA_MODE_AUDIO];
                                        hwseek = aEngine->getNextHardKeyPressed() ||aEngine->getPrevHardKeyPressed();
                                        KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                                        if((eventData->keyStatus == KEY_STATUS_PRESSED_LONG || eventData->keyStatus == KEY_STATUS_PRESSED_CRITICAL) &&
                                            hwseek )
                                            hwkey = false;
                                    }
                                    if((getJogKeyPressed()!=0 && hwkey)||getSharedMemData()!=0)
                                    //} modified by yongkyun.lee 2013-10-16 
                                    {
                                        MP_LOG << "EVT_KEY_HU_SEEK: JogKeyPressed key status" << getJogKeyPressed() << LOG_ENDL;
                                        return;
                                    }
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                            }
                            break;
                        }
                        default:
                        {
                            //{ modified by yongkyun.lee 2013-10-16 for :  ITS 194947
                            bool hwkey = true;
                            if( (m_LastPlayerMode == APP_MEDIA_MODE_AUDIO /*|| m_LastPlayerMode ==  APP_MEDIA_MODE_VIDEO*/) &&
                                 aEvent.GetEventId() != EVT_KEY_HU_JOG_DIAL_ENCODER)
                            {
                                bool hwseek;
                                AppMediaPlayer_EngineAudio * aEngine;
                                aEngine =  (AppMediaPlayer_EngineAudio*) m_Engines[APP_MEDIA_MODE_AUDIO];
                                hwseek = aEngine->getNextHardKeyPressed() ||aEngine->getPrevHardKeyPressed();            
                                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                                if((eventData->keyStatus == KEY_STATUS_PRESSED_LONG || eventData->keyStatus == KEY_STATUS_PRESSED_CRITICAL) &&
                                    hwseek )
                                    hwkey = false;
                            }
                            if((getJogKeyPressed()!=0 && hwkey)||getSharedMemData()!=0)
                            //} modified by yongkyun.lee 2013-10-16 
                            {
                                MP_LOG << "EVT_KEY_HU_SEEK: JogKeyPressed key status" << getJogKeyPressed() << LOG_ENDL;
                                return;
                            }
                            break;
                        }
                    }

                //deprecated
//                if((!m_bSwapEnabled&&(getJogKeyPressed()&CCP_JOG_CENTER_PRESSED))//normal
//                        ||(m_bSwapEnabled&&getJogKeyPressed()&RRC_JOG_CENTER_PRESSED)//swap

//                        )
//                    {
//                        MP_LOG << "EVT_KEY_HU_JOG_DIAL_ENCODER: JogKeyPressed key status" << getJogKeyPressed() << LOG_ENDL;
//                        return;
//                    }// deprecated
                    break;
                }
                default:
                {
                    break;
                }
            }
            //added by suilyou 20130923 END
            if(getInOutTestMode() == true) break; //modified by wonseok.heo 2013.07.04 disc in out test

            if (m_LastPlayerMode == APP_MEDIA_MODE_AUDIO || m_LastPlayerMode == APP_MEDIA_MODE_VIDEO)
            {
                if(m_Engines[m_LastPlayerMode])
                    m_Engines[m_LastPlayerMode]->EventHandler(aEvent, DISPLAY_FRONT);
            }
            else
            {
                APP_MEDIA_MODE_T activeMode = m_pNotifier->GetActiveMode();

                if (activeMode == APP_MEDIA_MODE_AUDIO || activeMode == APP_MEDIA_MODE_VIDEO)
                {
                    if(m_Engines[activeMode])
                        m_Engines[activeMode]->EventHandler(aEvent, DISPLAY_FRONT);
                }
            }
            break;
        }
        case EVT_KEY_CCP_SEEK_PREV:     // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
        case EVT_KEY_CCP_SEEK_NEXT:     // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
        //case EVT_KEY_RRC_CH_BK:         // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
        //case EVT_KEY_RRC_CH_FW:         // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
        case EVT_KEY_RRC_SEEK_PREV:
        case EVT_KEY_RRC_SEEK_NEXT:
        {
            //{ modified by yongkyun.lee 2013-10-16 for :  ITS 194947
            bool hwkey = true;
            if( (m_LastPlayerMode == APP_MEDIA_MODE_AUDIO /*|| m_LastPlayerMode ==  APP_MEDIA_MODE_VIDEO*/) &&
                 aEvent.GetEventId() != EVT_KEY_HU_JOG_DIAL_ENCODER)
            {
                bool hwseek;
                AppMediaPlayer_EngineAudio * aEngine;
                aEngine =  (AppMediaPlayer_EngineAudio*) m_Engines[APP_MEDIA_MODE_AUDIO];
                hwseek = aEngine->getNextHardKeyPressed() ||aEngine->getPrevHardKeyPressed();  
                MP_LOG << "[leeyk1]" << hwseek <<  aEngine->getNextHardKeyPressed() << aEngine->getPrevHardKeyPressed() << LOG_ENDL;  
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                if((eventData->keyStatus == KEY_STATUS_PRESSED_LONG || eventData->keyStatus == KEY_STATUS_PRESSED_CRITICAL) &&
                    hwseek)
                    hwkey = false;
            }
            //} modified by yongkyun.lee 2013-10-16 
            //added by suilyou 20130925 START
            if(((getJogKeyPressed()& CCP_JOG_CENTER_PRESSED) && hwkey) ||(getSharedMemData()!=0))//{ modified by yongkyun.lee 2013-10-16 for :  ITS 194947
            {
                MP_LOG << "CCP OR RRC SEEK Key: ignore JogKeyPressed key status" << getJogKeyPressed() << LOG_ENDL;
                return;
            }
            //added by suilyou 20130925 END
            MP_LOG << "[Hard Key Event]:" << aEvent.GetEventId() << LOG_ENDL;
            if(getInOutTestMode() == true) break; //modified by wonseok.heo 2013.07.04 disc in out test

            if (m_LastPlayerMode == APP_MEDIA_MODE_AUDIO || m_LastPlayerMode == APP_MEDIA_MODE_VIDEO)
            {
                if(m_Engines[m_LastPlayerMode])
                    m_Engines[m_LastPlayerMode]->EventHandler(aEvent, DISPLAY_REAR);
            }
            else
            {
                APP_MEDIA_MODE_T activeMode = m_pNotifier->GetActiveMode();

                if (activeMode == APP_MEDIA_MODE_AUDIO || activeMode == APP_MEDIA_MODE_VIDEO)
                {
                    if(m_Engines[activeMode])
                        m_Engines[activeMode]->EventHandler(aEvent, DISPLAY_REAR);
                }
            }
            break;
        }

        // { added by kihyung 2013.1.24 for ISV 70304
        case EVT_KEY_HU_BACK:		
        case EVT_KEY_CCP_BACK:
			m_isFrontBack = true; //added by edo.lee 2013.03.21
        case EVT_KEY_HU_MENU:
        case EVT_KEY_SOFT_MENU:
        case EVT_KEY_CCP_MENU:
        case EVT_KEY_CCP_JOG_DIAL_ENCODER:
        case EVT_KEY_CCP_JOG_DIAL_CENTER:
#ifdef DUAL_KEY
        case EVT_KEY_CCP_JOG_DIAL_LEFT:
        case EVT_KEY_CCP_JOG_DIAL_RIGHT:
        case EVT_KEY_CCP_JOG_DIAL_UP:
        case EVT_KEY_CCP_JOG_DIAL_DOWN:
#endif
        {
#ifndef DUAL_KEY
        //added by suilyou 20130913 START
        KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
        if(aEvent.GetEventId() == EVT_KEY_CCP_JOG_DIAL_CENTER)
        {
            switch(eventData->keyStatus)
            {
                case KEY_STATUS_PRESSED:
                {
                    setJogKeyPressed(CCP_JOG_CENTER_PRESSED );
                    MP_LOG << "EVT_KEY_CCP_JOG_DIAL_CENTER:setJogKeyPressed key status" << getJogKeyPressed() << LOG_ENDL;
                    break;
                }
                case KEY_STATUS_RELEASED:
                {
                    setJogKeyReleased(CCP_JOG_CENTER_PRESSED);
                     MP_LOG << "EVT_KEY_CCP_JOG_DIAL_CENTER:setJogKeyReleased key status" << getJogKeyPressed() << LOG_ENDL;
                    break;
                }
                default:
                {
                    break; // make compiler happy
                }
            }
        }
        else if(aEvent.GetEventId() ==EVT_KEY_CCP_JOG_DIAL_ENCODER)
        {
            //sendTouchCleanUpForApps();//added by suilyou 20131001 ITS 0192529 //removed by sangmin.seol DUAL_KEY-Fix Not need here.
            if(getJogKeyPressed()&RRC_JOG_CENTER_PRESSED)
            {
                MP_LOG << "EVT_KEY_CCP_JOG_DIAL_ENCODER:getJogKeyReleased key status" << getJogKeyPressed() << " wheel ignored" << LOG_ENDL;
                return;
            }
        }
        // @todo - UISH to fix menu press on clone, remove below code once fixed in UISH
        else if(aEvent.GetEventId() == EVT_KEY_HU_MENU ||
            aEvent.GetEventId() == EVT_KEY_SOFT_MENU ||
            aEvent.GetEventId() == EVT_KEY_CCP_MENU)
        {
            if((m_eCloneStatus != CLONE_OFF) && ( getJogKeyPressed() != 0))
            {
                MP_HIGH << "EVT_KEY_HU_MENU Menu not processed in clone mode and jog already pressed" << LOG_ENDL;
                break;
            }
        }
#endif
        //added by suilyou 20130913 END
            if(getInOutTestMode() == true) break; //modified by wonseok.heo 2013.07.04 disc in out test
            MP_LOG << "[Front Hard Key Event]:" << aEvent.GetEventId() << LOG_ENDL;
            TopEventHandler(aEvent, DISPLAY_FRONT);                
            break;
        }

        case EVT_KEY_RRC_BACK:
			m_isFrontBack = false; //added by edo.lee 2013.03.21
        case EVT_KEY_RRC_MENU:
        case EVT_KEY_RRC_JOG_DIAL_ENCODER:
        case EVT_KEY_RRC_JOG_DIAL_CENTER:
#ifdef DUAL_KEY
        case EVT_KEY_RRC_JOG_DIAL_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_UP:
        case EVT_KEY_RRC_JOG_DIAL_DOWN:
#endif
        {
#ifndef DUAL_KEY
        //added by suilyou 20130913 START
        KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
        if(aEvent.GetEventId() == EVT_KEY_RRC_JOG_DIAL_CENTER)
        {
            switch(eventData->keyStatus)
            {
                case KEY_STATUS_PRESSED:
                {
                    setJogKeyPressed(RRC_JOG_CENTER_PRESSED );
                    MP_LOG << "EVT_KEY_RRC_JOG_DIAL_CENTER:setJogKeyPressed key status" << eventData->keyStatus << LOG_ENDL;
                    break;
                }
                case KEY_STATUS_RELEASED:
                {
                    setJogKeyReleased(RRC_JOG_CENTER_PRESSED );
                     MP_LOG << "EVT_KEY_RRC_JOG_DIAL_CENTER:setJogKeyReleased key status" << eventData->keyStatus << LOG_ENDL;
                    break;
                }
                default:
                {
                    break; // make compiler happy
                }
            }
        }
        else if(aEvent.GetEventId() ==EVT_KEY_RRC_JOG_DIAL_ENCODER)
        {
            if(getJogKeyPressed()&CCP_JOG_CENTER_PRESSED)
            {
                MP_LOG << "EVT_KEY_RRC_JOG_DIAL_ENCODER:getJogKeyReleased key status" << getJogKeyPressed() << " wheel ignored" << LOG_ENDL;
                return;
            }
        }
        else if(aEvent.GetEventId() == EVT_KEY_RRC_MENU)
        {
            if((m_eCloneStatus != CLONE_OFF) && ( getJogKeyPressed() != 0))
            {
                MP_HIGH << "EVT_KEY_HU_MENU Menu not processed in clone mode and jog already pressed" << LOG_ENDL;
                break;
            }
        }
        //added by suilyou 20130913 END
#endif
            MP_LOG << "[Rear Hard Key Event]:" << aEvent.GetEventId() << LOG_ENDL;			
            if(getInOutTestMode() == true) break; //modified by wonseok.heo 2013.07.04 disc in out test
            //sendTouchCleanUpForApps();//added by suilyou 20131001 ITS 0192529 // DUAL_KEY removed by sangmin.seol
            TopEventHandler(aEvent, DISPLAY_REAR);
            break;
        }
        // } added by kihyung 2013.1.24

        case EVT_SETTINGS_CHANGED:
        {
            MP_LOG << "EVT_SETTINGS_CHANGED" << LOG_ENDL;

            if ( aEvent.Data().isEmpty() )
            {
                MP_LOG << "Empty data. Return." << LOG_ENDL;
                break;
            }

            EVENT_SETTINGS_CHANGED_DATA_T *eventData = (EVENT_SETTINGS_CHANGED_DATA_T*)aEvent.Data().data();
            if( eventData->sKey == AppSettingsDef::DB_KEY_SCROLL_TICKER )
            {
                m_scrollingTicker = (eventData->sValue == 1);
                emit scrollingTickerChanged();
            }
            
            // { added by kihyung 2013.1.12
            //TopEventHandler(aEvent, m_currentScreen);  // deleted by eugene.seo 2013.02.26
			EventHandler(aEvent, m_currentScreen);  // added by eugene.seo 2013.02.26
            // } added by kihyung 2013.1.12                
            
            break;
        }

        // { added by kihyung 2012.12.26 for USB Diag
        case EVT_DIAGNOSTIC_TEST_START:
        {
            USBEngineerMode *pUSBEng = GetEngModeNotifier()->GetUSBEngineerMode();

            APP_EVENT_DIAGNOSTIC_TEST_T *eventData = (APP_EVENT_DIAGNOSTIC_TEST_T*)aEvent.Data().data();
            if( eventData->testStatus == true )
            {
                bool bReadingFail = pUSBEng->USBFileReadingFail(AudioUtils::GetFrontUSBPath());
                GetEngModeNotifier()->SendUSBDiagResult(true, bReadingFail);

                // { removed by kihyung 2013.07.12. Removed spec.
                /*
                bReadingFail = pUSBEng->USBFileReadingFail(AudioUtils::GetRearUSBPath());
                GetEngModeNotifier()->SendUSBDiagResult(false, bReadingFail);
                */
                // } removed by kihyung 2013.07.12. Removed spec.

                // added by kihyung 2013.10.03
                if(bReadingFail == true) 
                {
                    MP_LOG << bReadingFail << "beep" << LOG_ENDL;
                    beep(eBeepType1);    
                }
                
                Event event( EVT_DIAGNOSTIC_TEST_END, GetThisAppId(), APP_UI_MANAGER );
                notifyUISH( event, DISPLAY_FRONT_REAR );
            }
            
            break;
        }        
        // } added by kihyung 2012.12.26 for USB Diag

        // { removed by kihyung 2014.01.14 for ITS 0218420

        // { modified by kihyung 2013.7.29 for ITS 0181531
        // { modified by Sergey 15.10.2013 for ITS#194932
        //#if (AVP_USE_NEW_BLEND_AFW == 0)
        case EVT_CHANGE_OVERLAYPLANE:
        {
            MP_HIGH << "[EVT_CHANGE_OVERLAYPLANE]" << LOG_ENDL;
            //DISPLAY_PLANES_T eDispPlane = (DISPLAY_PLANES_T)aEvent.GetCommand();
            //m_pNotifier->setOverlayPlanes(eDispPlane, true);
            emit overplaneChanged();

            break;
        }
        //#endif
        // } modified by Sergey 15.10.2013 for ITS#194932
        // { modified by kihyung 2013.7.29 for ITS 0181531

        // } removed by kihyung 2014.01.14 for ITS 0218420

        // { added by kihyung 2013.1.24 for ISV 70304
        case EVT_SWAP_DC_ENABLED:
        {
            MP_LOG << "[EVT_SWAP_DC_ENABLED]" << LOG_ENDL;
            m_bSwapEnabled = true;
            break;
        }

        case EVT_SWAP_DC_DISABLED:
        {
            MP_LOG << "[EVT_SWAP_DC_DISABLED]" << LOG_ENDL;            
            m_bSwapEnabled = false;
            break;
        }
        // } added by kihyung 2013.1.24

        //{ added by hyochang.ryu 20131017 for BL call
        case EVT_BLUELINK_CALL_ONGOING:	//188
        {
            MP_LOG << "EVT_BLUELINK_CALL_ONGOING" << LOG_ENDL;
            setisBLCall(true);
            break;
        }
        case EVT_BLUELINK_CALL_IDLE:	//189
        {
            MP_LOG << "EVT_BLUELINK_CALL_IDLE" << LOG_ENDL;
            //setisBLCall(false);
            break;
        }
        //} added by hyochang.ryu 20131017 for BL call
    
        //added by edo.lee 2013.04.03
		case EVT_TA_DAB_START: // added by kihyung 2013.08.01 for ITS 0174593
        case EVT_TA_RDS_START: // added by kihyung 2013.08.01 for ITS 0174593
        case EVT_CALL_START:
        {
            // { added by sh.kim for ITS 179104
            if(aEvent.GetEventId() == EVT_TA_RDS_START) {
                if(m_bFrontAVPIsForeground || !m_isNoneState){
                    MP_LOG << "Set m_isTAOn at Only FG State" << LOG_ENDL;
                    m_isTAOn = true;
                }
            }
            // } added by sh.kim for ITS 179104
            
            // ITS 232821 setisBTCall first and then notifyBtCallProgressing
            if(m_isBTCall) return;// added by lssanh 2013.06.26 bt call
            if(aEvent.GetEventId() == EVT_CALL_START)//added by yongkyun.lee 2013-10-11 for : ITS 191526
                setisBTCall(true); // modified by Dmitry 28.04.13
            emit notifyBtCallProgressing(true); // added by wspark 2013.04.10 for cluster update
            m_playOnBTCall = true; //added by edo.lee 2013.06.27 for BT CALL

            NotifyFromUISHDisplay(aEvent, DISPLAY_FRONT); // added by wspark 2013.04.12
            //{ added by wonseok.heo for  ITS 178767 2013.07.10
            if(isPowerOff() == true){
                m_PowerOffBTCall = true;
            } // } added by wonseok.heo for  ITS 178767 2013.07.10

            break;
        }
        
        case EVT_TA_DAB_END: // added by kihyung 2013.08.01 for ITS 0174593
        case EVT_TA_RDS_STOP: // added by kihyung 2013.08.01 for ITS 0174593
        case EVT_CALL_END:
        {
            // { added by sh.kim for ITS 179104
            if(aEvent.GetEventId() == EVT_TA_RDS_STOP) {
                m_isTAOn = false;
            }
            // } added by sh.kim for ITS 179104
            
            emit notifyBtCallProgressing(false);
            // { modified by cychoi 2013.10.30 for ISV 93613
            // { modified by yungi 2013.08.16 for ITS 184328 Opstate update after BT call
            emit sendTpMessage(); // rollback by yungi 2013.11.04 Cluster BT-Call END // added by yungi 2013.11.03 for Cluster BT-Call end Not State Changed When DVD TItle Disc Menu
            // } modified by yungi 2013.08.16
            // } modified by cychoi 2013.10.30
            if(aEvent.GetEventId() == EVT_CALL_END)//added by yongkyun.lee 2013-10-11 for : ITS 191526
                setisBTCall(false); // modified by Dmitry 28.04.13
            NotifyFromUISHDisplay(aEvent, DISPLAY_FRONT); // added by wspark 2013.04.12
            setPlayOnBTCall(); //Added by Puneet on 2013.07.11 for ITS #0179376 , Moved from EngineAudio and EngineVideo
            m_PowerOffBTCall = false;//added by edo.lee 2013.07.31
            break;
        }
        //added by edo.lee 2013.04.03

        // { added by kihyung 2013.2.14
        case EVT_CAMERA_ENABLE:
        {
            MP_HIGH << "CAMS [EVT_CAMERA_ENABLE]" << LOG_ENDL;

            // { modified by Sergey 26.05.2013
            AppMediaPlayer_EngineVideo *pVEngine = (AppMediaPlayer_EngineVideo*)GetEngine(APP_MEDIA_MODE_VIDEO);
            if(pVEngine)
            {
                DISPLAY_T cameraOpposite = IsSwapDCEnabled() ? DISPLAY_FRONT : DISPLAY_REAR;
                pVEngine->GetVideoController()->changeWidgetDisplay(cameraOpposite);

                if(pVEngine->GetVideoController()->getSource() != MEDIA_SOURCE_MAX)
                {
                    if(IsSwapDCEnabled())
                        SetGstPipelinesStatus(GP_DISPLAY_REAR, false);
                    else
                        SetGstPipelinesStatus(GP_DISPLAY_FRONT, false);
                }
            }
			setCamInFG(true);
            emit camEnable(true);
            // } modified by Sergey 26.05.2013

            break;
        }

        case EVT_CAMERA_DISABLE:
        {
            // { modified by kihyung 2013.3.16 // for ISV 76163
            MP_HIGH << "CAMS [EVT_CAMERA_DISABLE]" << LOG_ENDL;
            setCamInFG(false);
            emit camEnable(false); // added by wspark 2013.02.21 for ISV 73305
            
            AppMediaPlayer_EngineVideo *pVEngine = (AppMediaPlayer_EngineVideo*)GetEngine(APP_MEDIA_MODE_VIDEO);
            //{ modified by yongkyun.lee 2013-08-16 for : ITS 184636,184637
            //MP_HIGH << "[leeyk1]CAMS [EVT_CAMERA_DISABLE] : m_eCloneStatus=" << IsSwapDCEnabled() << pVEngine->GetVideoController()->isSeekMode()   << LOG_ENDL;
            if( (IsSwapDCEnabled()) && (pVEngine->GetVideoController()->isSeekMode()))
            {
                m_bMediaOnFromCamOff = true;
                break;
            }
            //} modified by yongkyun.lee 2013-08-16 
            if(pVEngine) {
                if(GetActiveMode() == APP_MEDIA_MODE_VIDEO
                        && !(pVEngine->actualDisp(pVEngine->getCurrentScreen()) == DISPLAY_FRONT && isFrontLCDOff()) // added by Sergey 05.11.2013 for ITS#212560
                        && (pVEngine->isLaunchingScreenSettings() == true || pVEngine->isVideoInFG())) { // modified by Sergey for 158028
                    MP_MEDIUM << "CAMS [EVT_CAMERA_DISABLE] isLaunchingScreenSettings is true" << LOG_ENDL;
                    // usleep(500000); // removed by Sergey 02.05.2013 for ISV_NA#71667
                    MP_MEDIUM << "NotifyWhenGstPipelinesAvailable" << pVEngine->getCurrentScreen() << LOG_ENDL;
                    m_pNotifier->setGstPipelineDisplay(pVEngine->getCurrentScreen()); // added by kihyung 2014.01.14 for ITS 0218420
                    NotifyWhenGstPipelinesAvailable(pVEngine->getCurrentScreen()); // added by kihyung 2013.4.4
                    pVEngine->GetVideoController()->clearDisplay(false);
					pVEngine->GetVideoController()->showVideo(pVEngine->getCurrentScreen()); // added by Sergey 12.04.2013 for DV2-3#31
                    pVEngine->SetVideoSize(pVEngine->getCurrentScreen()); // modified by cychoi 2013.07.03 for ITS 177711 // { added by cychoi 2013.05.16 for ISV 78583 & ITS 167437
                }
            }
            // } modified by kihyung 2013.3.16 // for ISV 76163
            
            m_bMediaOnFromCamOff = true; // added by kihyung 2013.06.26
            break;
        }
        // } added by kihyung 2013.2.14
        
		//{modified by edo.lee 2013.09.05
		case EVT_REQEUST_MUTE:
		{
			MP_HIGH << "EVT_REQUEST_MUTE" << LOG_ENDL;
			m_isMuteState = true;
			bool isFFREW = false;
			//added by edo.lee 2013.06.22
			if(m_isNoneState == false){
				if(GetFrontMode()==APP_MEDIA_MODE_AUDIO || GetRearMode()==APP_MEDIA_MODE_AUDIO)
				{
					AppMediaPlayer_EngineAudio*pAEngine = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
					pAEngine->SetSystemMute();
					isFFREW = pAEngine->getFFREWState();
				}else if(GetFrontMode()==APP_MEDIA_MODE_VIDEO || GetRearMode()==APP_MEDIA_MODE_VIDEO)
				{
					AppMediaPlayer_EngineVideo*pVEngine = (AppMediaPlayer_EngineVideo*)GetEngine(APP_MEDIA_MODE_VIDEO);
					pVEngine->SetSystemMute();
					isFFREW = pVEngine->getFFREWState();
					
				}
				//added by edo.lee 2013.09.05 ITS 0188286
				if(getIsTermOnTrackChange() == true)// || isFFREW)  modified by edo.lee 2013.11.20
				{
				    setKeepSystemMute(true);
				}
			}
			//modified by edo.lee 2013.09.05
            break;
		}
		//added by edo.lee 2013.09.24
		case EVT_REQEUST_UNMUTE:
		{
			MP_HIGH << "EVT_REQEUST_UNMUTE" << LOG_ENDL;
			m_isMuteState = false;
			break;
		}
		//added by edo.lee 2013.09.24
		// { modified by Sergey 13.09.2013 for ITS#188762
        case EVT_INITIALIZATION_REQUEST:
        {
            MP_HIGH << "EVT_INITIALIZATION_REQUEST" << LOG_ENDL;

            for (int index = 0; index < APP_MEDIA_MODE_MAX; index++)
            {
                if (m_Engines[index])
                {
                    m_Engines[index]->resetAll();
                }
            }


            AppMediaPlayer_SettingsStorage::GetInstance()->ResetData(this);
            m_pEngModeNotifier->ResetData((EMPCountryVariant)AppEngineBase::GetCountryVariant()); // added by cychoi 2013.07.26 for ITS 181382 & 158365 Initialize DVD Settings

            break;
        }


        // { added by kihyung lee 2013.05.08
        case EVT_FACTORY_RESET_REQUEST:
        {
            MP_HIGH << "EVT_FACTORY_RESET_REQUEST" << LOG_ENDL;

            for (int index = 0; index < APP_MEDIA_MODE_MAX; index++)
            {
                if (m_Engines[index])
                {
                    m_Engines[index]->factoryReset();
                }
            }

            AppMediaPlayer_SettingsStorage::GetInstance()->FactoryReset(this); // modified by Sergey 19.05.2013
            m_pEngModeNotifier->FactoryReset((EMPCountryVariant)AppEngineBase::GetCountryVariant()); // added by cychoi 2013.07.26 for ITS 181382 & 158365 Initialize DVD Settings
            ((AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO))->deleteUserData();

            // added by kihyung 2013.10.03 for factory reset
            CTrackerAbstractor *pTrackerAbs = GetTrackerAbs();
            if(pTrackerAbs) {
                pTrackerAbs->FactoryReset();
            }

            Event tResEvent(EVT_FACTORY_RESET_RESPONSE, GetThisAppId(), APP_UI_MANAGER);
            NotifyUISHCommon(tResEvent);

            break;
        }
        // } modified by Sergey 13.09.2013 for ITS#188762    

        
        case EVT_POWER_STATE_NORMAL_OFF:
        {
            MP_HIGH << "EVT_POWER_STATE_NORMAL_OFF" << LOG_ENDL;
            m_bPowerOff = true;
            //emit signalPowerNormalOff(); // modified for ITS 0210855 //removed by Michael.Kim for ITS 212424
            break;
        }

        case EVT_POWER_STATE_NORMAL_ON:
        {
            MP_HIGH << "EVT_POWER_STATE_NORMAL_ON" << LOG_ENDL;
            m_bPowerOff = false;
            //{added by junam 2013.11.26 for ITS_NA_210792
            if(AppEngineBase::GetCountryVariant() == eCVNorthAmerica)
                MP_LOG<<"NA do not turn off" << LOG_ENDL;
            else //}added by junam
                m_bMediaOnFromOff = true;
            // { removed by sangmin.seol For ITS 0232871 maintain pause state when BT Call started in power off state
            //AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_Engines[APP_MEDIA_MODE_AUDIO]);
            //pAudioEngine->setMaintainPause(false);
            // } removed by sangmin.seol For ITS 0232871 maintain pause state when BT Call started in power off state
            break;
        }

        // { added by kihyung 2013.08.20 for ITS 0183782 
        case EVT_ACC_CHANGED:
        {
            EVENT_EVT_CAR_IF_STATUS_T *pACCStatus = (EVENT_EVT_CAR_IF_STATUS_T *)aEvent.Data().data();
            if(pACCStatus->status == 0)
            {
                MP_HIGH << "EVT_ACC_CHANGED -> ACC OFF" << LOG_ENDL;
                m_bACCOff = true;
                emit signalACCchanged(true); // added by suilyou 20131002 ITS 0192903
            }
            else
            {
                MP_HIGH << "EVT_ACC_CHANGED -> ACC ON" << LOG_ENDL;
                m_bACCOff = false;
                m_accOffFromOn = true;
                emit signalACCchanged(false); // added by suilyou 20131002 ITS 0192903
            }
            break;
        }
		
        // modified for ITS 0212002, replaced EVT_ACC_CHANGED with EVT_POWER_STATE_LOGIC_OFF for cancel operations
        case EVT_POWER_STATE_LOGIC_OFF:
        {
            MP_HIGH << "EVT_POWER_STATE_LOGIC_OFF -> ACC OFF" << LOG_ENDL;
            emit signalPowerStateLogicOff();
            break;
        }
		
        case EVT_AGREE_SHOW_OSD:
        {
            MP_HIGH <<"EVT_AGREE_SHOW_OSD" << LOG_ENDL;
            emit signalAgreeOSD();
            //OSDManager::instance()->connectOSDsignalToSlot();  //remove by edo.lee 2013.12.14
            break;
        }
        // } added by kihyung 2013.08.20 for ITS 0183782 
        
        // { added by kihyung 2013.07.04 for ISV 86619
        case EVT_DISPLAY_ON:
        {
            m_bDisplayOff = false;
            break;
        }

        case EVT_DISPLAY_OFF:
        {
            m_bDisplayOff = true;
            break;
        }        
        // } added by kihyung 2013.07.04 for ISV 86619

        // { moved by kihyung 2013.08.01
        /*
        //added by sh.kim for ITS 179104
        case EVT_TA_RDS_START:
        {
            MP_LOG<< "Current AVP FG State : "<< m_bFrontAVPIsForeground <<"Current AV Mode None State: " << m_isNoneState << LOG_ENDL;
            MP_LOG<<"EVT_TA_START message::From UISH Common" << LOG_ENDL;
            if(m_bFrontAVPIsForeground || !m_isNoneState){
                MP_LOG << "Set m_isTAOn at Only FG State" << LOG_ENDL;
                m_isTAOn = true;
            }
            break;
        }
        case EVT_TA_RDS_STOP:
        {
            MP_LOG<< "Current AVP FG State : "<< m_bFrontAVPIsForeground << LOG_ENDL;
            MP_LOG<< "EVT_TA_STOP message::From UISH Common" << LOG_ENDL;
            m_isTAOn = false;
            break;
        }
        //added by sh.kim for ITS 179104
        */
        // } moved by kihyung 2013.08.01
        
        // { modified by Sergey for SwapDC (customization)
        case EVT_NOTIFY_VR_STATUS:
        {
            MP_LOG<<"EVT_NOTIFY_VR_STATUS : "<<  aEvent.GetCommand() << LOG_ENDL;
#ifndef DUAL_KEY
            if(aEvent.GetCommand() == 1)
            {
                clearCCPJogKey();
                clearRRCJogKey();
            }
#endif
            m_bVRStatus = aEvent.GetCommand();
            EventHandler(aEvent, DISPLAY_FRONT); // added by kihyung 2013.10.24 for ITS 0197481 
            break;
        }
        default:
        {
            /* If event is not handled here it means that it may be handled
             * in one if engines or it is a display dependent event.
             * Give a chance to handle it.
             */
            NotifyFromUISHDisplay(aEvent, DISPLAY_FRONT);
            break;
        }
        // } modified by Sergey for SwapDC (customization)
    }
}

void AppMediaPlayer_EngineMain::NotifyFromUISHDisplay( Event aEvent, DISPLAY_T display ) // { modified by Sergey for SwapDC (customization)
{
    switch ( aEvent.GetEventId() )
    {
		case EVT_FB_OVERLAY_BLEND_ON:
		{
			MP_LOG << ": FB_BLEND_ON " << LOG_ENDL;
			if( m_bSwapEnabled == true)
			{
			    if(display == DISPLAY_FRONT)
			    {
			        m_bIsRearBlendOn = true;
			    }
			    if(display == DISPLAY_REAR)
			    {
			        m_bIsFrontBlendOn = true;
			    }
			}
			else
			{
			    if(display == DISPLAY_FRONT)
			    {
			        m_bIsFrontBlendOn = true;
			    }
			    if(display == DISPLAY_REAR)
			    {
			        m_bIsRearBlendOn = true;
			    }
			}
			break;
		}

		case EVT_FB_OVERLAY_BLEND_OFF:
		{
			MP_LOG << ": FB_BLEND_OFF " << LOG_ENDL;

			if( m_bSwapEnabled == true)
			{
			    if(display == DISPLAY_FRONT)
			    {
			        m_bIsRearBlendOn = false;
			    }
			    if(display == DISPLAY_REAR)
			    {
			        m_bIsFrontBlendOn = false;
			    }
			}
			else
			{
			    if(display == DISPLAY_FRONT)
			    {
			        m_bIsFrontBlendOn = false;
			    }
			    if(display == DISPLAY_REAR)
			    {
			        m_bIsRearBlendOn = false;
			    }
			}

			break;
		}

        // { added by wspark 2013.04.15 for EVT_UPDATE_CLUSTER
        case EVT_UPDATE_CLUSTER:
        {
            MP_LOG << "clusterupdate on!!" << LOG_ENDL;
            m_pNotifier->setClusterUpdate(true);
            emit sendTpMessage();
            break;
        }
        // } added by wspark

        // removed by kihyung 2013.07.10 for ITS 0173538 


        // { added by Sergey 07.09.2013 for PRE_FG supoort
        case EVT_REQUEST_FG:
        {
            MP_HIGH << "START EVT_REQUEST_FG" << display << LOG_ENDL;
            TopEventHandler(aEvent, display);

#if 0   // deleted by sungha.choi 2013.09.26 for using auto response fg of AFW
            Event evt(EVT_RESPONSE_FG, APP_AVP, APP_UI_MANAGER);
            NotifyUISH(evt, display);
#endif
            m_bShowLoadingFlag = false; // added by cychoi 2013.09.10 for SmokeTest DVD Title Menu Screen
            m_bFgEventStart = false;//modified by edo.lee 2013.10.16 ITS 0196016 

            MP_LOG << "END EVT_REQUEST_FG" << display << LOG_ENDL;
            break;
        }
        // } added by Sergey 07.09.2013 for PRE_FG supoort

        case EVT_REQUEST_PRE_FG:		// modified by Sergey 07.09.2013 for PRE_FG supoort
        {        
            AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_Engines[APP_MEDIA_MODE_AUDIO]);
            //{removed by junam 2013.11.29 for tune
            // { modified by cychoi 2013.11.16 for Prevent Fix
            //APP_ENTRY_POINT_DATA_T *pD = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
            //if(pAudioEngine&&pD&&pD->sHeader.bTemporalMode==false) // ITS 0210307
            //{
            //    pAudioEngine->clearTuneDelayTimer(); // added by suilyou 20131112 ITS 0208066
            //}
            // } modified by cychoi 2013.11.16
            //}removed by junam
            if ( !aEvent.Data().isEmpty() )
            {
                APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();

                if ( pEventData )
                {                    
                    
                    if(isMediaOnFromOff() == true && getisBTCall())//Added by taihyun.ahn 2013.10.22
                    {
                        setBTCallPowerOn(true);
                    }
					// { modified by Sergey 16.04.2013
                    MP_HIGH << "START EVT_REQUEST_PRE_FG for display =" << display << "entry =" << pEventData->sAppEntryPoint << ", bTemporalMode =" << pEventData->sHeader.bTemporalMode << LOG_ENDL; // modified by Sergey 07.09.2013 for PRE_FG supoort
                    m_bFgEventStart = true;//modified by edo.lee 2013.10.16 ITS 0196016 
	 			// { modified by edo.lee 2013.08.16 for subscreen in AVP
					if( (pEventData->sAppEntryPoint == eAPP_AVP_VIDEO_CAPTION_SETTING 
					  ||pEventData->sAppEntryPoint == eAPP_AVP_VIDEO_DIVX_SETTING  
					  ||pEventData->sAppEntryPoint == eAPP_AVP_DVD_SETTING  
					  ||pEventData->sAppEntryPoint == eAPP_AVP_VCD_PBC_MENU_SETTING  ))
					{
						MP_LOG<<" subscreen FG" << LOG_ENDL;
						setLaunchSubscreen(true);
					}else {
						setLaunchSubscreen(false);
						//remove by edo.lee 2013.09.02 ITS 0187851
						//if( GetLastAVEntryPoint(false) != pEventData->sAppEntryPoint )
						//{
							//setSubCreenOn(false , getSubCreenName()); //remove by edo.lee 2013.10.10 
						//}
					}
					setTempMode( checkIsSameEntryPoint( display, pEventData->sAppEntryPoint) && pEventData->sHeader.bTemporalMode);//added by edo.lee 2013.08.20
					// { modified by edo.lee 2013.10.10 
					if(getLaunchSubscreen() == false){
						if(getSubCreenOn()){ // from subscreen
						    setSubCreenOn(false , getSubCreenName());
							m_isChangeToDriving = false;
							m_isChangeToParking = false;
						}else // normal case 
						{
						    setIsChangeToDriving(pEventData->sAppEntryPoint,display); //added by edo.lee 2013.09.04  ITS 0188044
						    setIsChangeToParking(pEventData->sAppEntryPoint,display);//added by edo.lee 2013.09.04 ITS 0188044
						}
                    	SetLastAVEntryPoint(pEventData->sAppEntryPoint); // modified by Sergey 28.04.2013
	                    SetAVPEntryPoint(pEventData->sAppEntryPoint, display); // added by kihyung 2013.07.20 for ITS 0180419
					}
					// } modified by edo.lee 2013.10.10 

                    //{ modified by yongkyun.lee 2013-08-27 for : ITS 186453-186494
                    if(m_bAVOff == true)
                    {
                        m_bAVOff = false;
                        m_bMediaOnFromOff = true;
                    }
                    //} modified by yongkyun.lee 2013-08-27 
                    
                    m_isCompleteModeChange = true;//added by edo.lee 2013.05.01
                    if(m_isNoneState)
                        m_isMuteState = false;//added by edo.lee 2013.10.07 ITS 186267
                    /* //youngsim.jo 20131014 is not used
                    if(display == DISPLAY_FRONT)
                        m_bFrontFGResponsed = false;
                    else if(display == DISPLAY_REAR)
                        m_bRearFGResponsed = false;
                    */
                    // } modified by Sergey 16.04.2013
                    //added by edo.lee 2013.04.20
                    m_FRDRSViewLoading =  pEventData->sHeader.bReserved07;//added by edo.lee 2013.07.03
                    //remove by edo.lee 2013.07.09
                    bool isClone = pEventData->sHeader.bReserved02 ;//|| pEventData->sHeader.bReserved07;//modified by edo.lee 2013.07.03
                    /* if((GetLastAVEntryPoint(false) != pEventData->sAppEntryPoint) || getIsFromOtherAV())//added by edo.lee 2013.07.08
                    {
                     m_releasingClone = pEventData->sHeader.bReserved05 || pEventData->sHeader.bReserved06; //added by edo.lee 2013.07.07
                    }*/
					//remove by edo.lee 2013.07.09
					m_bKeepPlayState =  pEventData->sHeader.sIsJogEnabled ;// //added by edo.lee 2013.07.30 stay previous play state
					MP_LOG<<"m_bKeepPlayState "<<m_bKeepPlayState << LOG_ENDL;
                    //{modified by edo.lee 2013.04.26
					if(pEventData->sAppEntryPoint != eAPP_AVP_PHOTO_MODE_JUKEBOX &&
					   pEventData->sAppEntryPoint != eAPP_AVP_PHOTO_MODE_USB1 &&
					   pEventData->sAppEntryPoint != eAPP_AVP_PHOTO_MODE_USB2 &&
					   //added by edo.lee 2013.05.09 ITS_167973
					   pEventData->sAppEntryPoint != eAPP_AVP_PHOTO_MODE_JUKEBOX_DRS && 
					   pEventData->sAppEntryPoint != eAPP_AVP_PHOTO_MODE_USB1_DRS &&
					   pEventData->sAppEntryPoint != eAPP_APP_PHOTO_MODE_USB2_DRS)
					   //added by edo.lee 2013.05.09
					{
					//	m_playInfoDisplay = DISPLAY_FRONT_REAR;
					//}
					//else
					//added by edo.lee 2013.04.21	
					//{
	                    if( isClone)
							m_isCloneTemp = true;
						//else remove by edo.lee 2013.04.22 for OSD
						//{
						if(display == DISPLAY_FRONT)
						{
							m_playInfoDisplay = DISPLAY_REAR;
							GetNotifier()->ClearOSD(true); //added by edo.lee 2013.06.26
						}
						else
						{
							m_playInfoDisplay = DISPLAY_FRONT;
							GetNotifier()->ClearOSD(false);//added by edo.lee 2013.06.26
						}
						//}	
						//added by edo.lee 2013.06.18						
						AppMediaPlayer_EnginePhoto* pPhotoengine=NULL;
						if(display == DISPLAY_FRONT)
							 pPhotoengine = dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_FRONT]);
						else
							 pPhotoengine = dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_REAR]);
						if(pPhotoengine)
							pPhotoengine->setIsSlideShow(false);
						m_isVideoFrontEntered = false;  //added by edo.lee display OSD while video & image mode 
						m_isVideoRearEntered = false; 
						//added by edo.lee 2013.06.18
						//added by edo.lee 2013.06.21
						setIsFromOtherAV(m_isNoneState);
                		if(m_isNoneState) 
                        	GetNotifier()->onSetDisplayOSD(true);
						m_isNoneState = false;//added by edo.lee 2013.03.11
					}
					//}modified by edo.lee 2013.04.26
					
                	m_onBoot = false;//added by edo.lee 2013.02.28
                	//setIsFromOtherAV(m_isNoneState);//remove by edo.lee 2013.06.22
                	//if(m_isNoneState) 
                    //    GetNotifier()->onSetDisplayOSD(true);
                	//else	
					//	GetNotifier()->ClearOSD();//remove by edo.lee 2013.06.13
                	//m_isNoneState = false;//remove by edo.lee 2013.03.22
					//added by edo.lee 2013.03.29
					//{added by edo.lee 2013.05.31
                    // { modified by sungha.choi 2013.07.15 for static test
                    AppMediaPlayer_EngineVideo* pVideoEngine = NULL;
                    pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO]);
                    if(pVideoEngine == NULL) {
                        MP_HIGH << "pVideoEngine is NULL" << LOG_ENDL;
                        break;
                    }
                    // } modified by sungha.choi 2013.07.15 for static test

                    // { added by cychoi 2014.02.14 for DVD SearchCaption local Temporal Mode
                    if(pEventData->sAppEntryPoint == eAPP_AVP_DVD_SETTING)
                    {
                        if(pVideoEngine->isTempMode() == false)
                        {
                            setTempMode(false);
                        }
                    }
                    // } added by cychoi 2014.02.14

					if(GetActiveMode() == APP_MEDIA_MODE_PHOTO_FRONT || GetActiveMode() == APP_MEDIA_MODE_PHOTO_REAR)
					{
						m_fromPhoto = true;
					}
                    else
					{
						m_fromPhoto = false;
					}				
					//}added by edo.lee 2013.05.31
					bool bLockout = true;
					//{ added by edo.lee 2013.02.04
                    // removed by cychoi 2013.08.13 for unused code
                    // { added by cychoi 2013.07.22 for ITS 180543
                    m_bShowLoadingFlag = true;
                    if(isTempMode()) m_bShowLoadingFlag = false; //modified by edo.lee 2013.08.21
					
                    m_lockoutOnDVDSetting = false;
                    m_unLockoutOnDVDSetting = false;
                    // } added by cychoi 2013.07.22
                    //added by sh.kim for ITS 183716
                    switch(pEventData->sAppEntryPoint)
                    {
                        case eAPP_AVP_AUDIO_MODE_AUX:
                        {
                            MP_LOG<< "[EVT_AUX_AUDIO_PLUGEDIN] at FG " << LOG_ENDL;

                            //{ modified by cychoi 2013.08.20 for AUX Initial Detect
                            AUXInit(MODE_STATE_AUX1_AUDIO);
                            AUXInit(MODE_STATE_AUX2_AUDIO);
                            //} modified by cychoi 2013.08.20

                            m_isAuxDisconn = false;

                            //{ modified by shkim for ITS 0208482 Aux OSD Issue 2013.11.12
                            if(!pEventData->sHeader.bReserved07 &&  !OSDManager::instance()->getAUXDisconnect())
                                OSDManager::instance()->checkAuxMode();		//added by hyochang.ryu 20131105 for ITS 206819
                            //OSDManager::instance()->setAUXDisconnect(false); //{ added by shkim for ITS 205553 Aux OSD Issue
                            //} modified by shkim for ITS 0208482 Aux OSD Issue 2013.11.12
                        }
                        break;
                        case eAPP_AVP_VIDEO_MODE_AUX:
                        case eAPP_AVP_VIDEO_MODE_AUX_DRS:
                        {
                            MP_LOG<< "[EVT_AUX_VIDEO_PLUGEDIN] at FG " << LOG_ENDL;

                            //{ modified by cychoi 2013.08.20 for AUX Initial Detect
                            AUXInit(MODE_STATE_AUX1_VIDEO);
                            AUXInit(MODE_STATE_AUX2_VIDEO);
                            //} modified by cychoi 2013.08.20

                            m_isAuxDisconn = false;
                        }
                        break;
                        // { added by cychoi 2013.08.20 for ITS 178895 Make compiler happy
                        default: // make compiler happy
                        {
                            break;
                        }
                        // } added by cychoi 2013.08.20
                    }
                    //added by sh.kim for ITS 183716
                    switch(pEventData->sAppEntryPoint)
                    {

                    case eAPP_AVP_VIDEO_MODE_DISC:
                    case eAPP_AVP_VIDEO_MODE_JUKEBOX:
                    case eAPP_AVP_VIDEO_MODE_USB1:
                    case eAPP_AVP_VIDEO_MODE_USB2:
                    case eAPP_AVP_VIDEO_MODE_AUX:
                        //move by edo.lee 2013.08.27
                    case eAPP_AVP_VIDEO_CAPTION_SETTING :
                    case eAPP_AVP_VIDEO_DIVX_SETTING  :
                    case eAPP_AVP_DVD_SETTING  :
                    case eAPP_AVP_VCD_PBC_MENU_SETTING :

                        //added by edo.lee 2013.03.29
                        pVideoEngine->setIsShowLockoutView(display,false);
                        bLockout = false;
                        //added by edo.lee 2013.02.05
                    case eAPP_AVP_VIDEO_MODE_DISC_DRS:
                    case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
                    case eAPP_AVP_VIDEO_MODE_USB1_DRS:
                    case eAPP_AVP_VIDEO_MODE_USB2_DRS:
                    case eAPP_AVP_VIDEO_MODE_AUX_DRS:
                        //remove by edo.lee 2013.08.27
                        //added by edo.lee
                    {
                        m_isBgReceived = false;//added by edo.lee 2013.03.26
                        pEventData->sAppEntryPoint = GetLastAVEntryPoint(false);
                        //added by edo.lee 2013.03.29
                        //added by edo.lee 2013.06.26
                        if(bLockout)// normal -> lockout
                        {
                            pVideoEngine->setIsShowLockoutView(display,true);
                            if( getSubCreenOn() )//&&  isLockOnDVDSetting(pEventData->sAppEntryPoint))
                            {
                                MP_LOG<<"lock on DVD Setting" << LOG_ENDL;
                                m_lockoutOnDVDSetting = true;
                            }
                        }
                        else// lockout -> normal
                        {
                            if(getSubCreenOn() )// && isUnLockOnDVDSetting(pEventData->sAppEntryPoint) )

                            {
                                MP_LOG<<"unlock on DVD Setting" << LOG_ENDL;
                                m_unLockoutOnDVDSetting	= true;
                            }
                        }
                        //added by edo.lee 2013.06.26

                        if(m_lockoutOnDVDSetting || m_unLockoutOnDVDSetting)
                            m_bShowLoadingFlag = false;
                        m_lockoutOnDVDSetting = false;
                        m_unLockoutOnDVDSetting = false;

                        if(display == DISPLAY_REAR){
                            MP_LOG<<"rear true" << LOG_ENDL;
                            m_isVideoRearEntered = true;
                            m_currentVideoMode = VIDEO_DISPLAY_REAR;
                        }
                        else{
                            m_currentVideoMode = VIDEO_DISPLAY_FRONT;
                            m_isVideoFrontEntered = true;
                        }

                        break;
                    }
                        // { modified by cychoi 2013.08.20 for ITS 178895 Make compiler happy
                    default: // make compiler happy
                    {
                        //modified by edo.lee 2013.04.04
                        //pVideoEngine->setIsShowLockoutView(display, false);
                        //pVideoEngine->HideScreen(display); // added by kihyung 2013.06.1
                        break;
                    }
                    // } modified by cychoi 2013.08.20
                    }
                    m_bFrontAVPIsForeground = true; //added by suryanto.tan 2013.3.1 for SANITY_CM_AI001
                    //setTempMode(pEventData->sHeader.bTemporalMode);//added by edo.lee 2013.06.22
                    //ys -20130911 ITS 0186726
                    APP_MEDIA_MODE_T nMediaMode = GetAVMediaMode(pEventData->sAppEntryPoint);
                    if(nMediaMode == APP_MEDIA_MODE_PHOTO_FRONT || nMediaMode == APP_MEDIA_MODE_PHOTO_REAR )
                        emit signalPhotoFgReceived(isTempMode());
                    else if (nMediaMode == APP_MEDIA_MODE_AUDIO)
                        emit signalAudioFgReceived(isTempMode());
                    else if (nMediaMode == APP_MEDIA_MODE_VIDEO)
                        emit signalVideoFgReceived(isTempMode());
                    //ys -20130911 ITS 0186726


                    // } modified by wspark
                    // } modified by wspark
					// } modified by edo.lee 2013.07.30 subscreen
                    
                    SetActiveMode( pEventData->sAppEntryPoint, display, m_bShowLoadingFlag);  // modified by kihyung 2013.4.9 for ISV 78237
                    // Let's allow engines to handle REQUEST_FG as well
                    TopEventHandler( aEvent, display );   
                    //m_bShowLoadingFlag = false;  // commented by cychoi 2013.09.10 for SmokeTest DVD Title Menu Screen
                    MP_LOG << "END EVT_REQUEST_PRE_FG" << LOG_ENDL;
                }
                else {
                    MP_HIGH << "EVT_REQUEST_FG pEventData is NULL" << LOG_ENDL;
                }
				
				//remove by edo.lee 2013.11.08

                resetMediaOnFromOff();  // added by kihyung 2013.06.10 for ITS 0172955
                resetMediaOnFromCamOff();

				// removed by Sergey 07.09.2013 for PRE_FG supoort
                
            }
            else
            {
                MP_HIGH << "EVT_REQUEST_PRE_FG aEvent.Data().isEmpty()" << LOG_ENDL; // modified by Sergey 07.09.2013 for PRE_FG supoort
            }

            break;
        }
        case EVT_REQUEST_BG:
        {
            MP_HIGH << "START EVT_REQUEST_BG START" << display << LOG_ENDL;
#ifndef DUAL_KEY
            MP_LOG << "clear HardKeyStatus" << LOG_ENDL;
            clearCCPJogKey();
            clearRRCJogKey();
#endif
#ifdef DUAL_KEY
            switch(display)
            {
                case DISPLAY_FRONT:
                    MP_LOG << frontLCDMode << " is unlaunched front LCD" << LOG_ENDL;
                    frontLCDMode = APP_MEDIA_MODE_MAX;
                    break;
                case DISPLAY_REAR:
                    MP_LOG << rearLCDMode << " is unlaunched rear LCD" << LOG_ENDL;
                    rearLCDMode = APP_MEDIA_MODE_MAX;
                    break;
            }
#endif
            SetAVPEntryPoint(eAPP_NONE, display); // added by kihyung 2013.07.20 for ITS 0180419

            // { added by wonseok.heo 2013.07.04 disc in out test
            if(m_DiscInOutTestMode == true){

                emit discStatusData(99);
                m_DiscInOutTestMode = false;
                InOutTestEject();
            }
            // } added by wonseok.heo 2013.07.04 disc in out test

           // SetStyleSheet( AppEngineQMLConstants::APP_DUPLICATE_SCEENS, "background:transparent;"); // remove by edo.lee	2013.06.10

			// removed by Sergey 07.09.2013 for PRE_FG supoort
           
            bool bCalledHideUI = false;
			m_FRDRSViewLoading = false; //added by edo.lee 2013.07.03

            m_isBgReceived = true;
            emit signalBgReceived();

            if ( display == DISPLAY_FRONT )
            {
                if ( m_AppFrontMode != APP_MEDIA_MODE_MAX &&
                     m_Engines[ m_AppFrontMode ] != NULL )
                {
//                   m_Engines[ m_AppFrontMode ]->HideUI(display); // removed by Sergey 24.08.2013 for ITS#185556 
                   bCalledHideUI = true;
                }
            }
            else
            {
                if ( m_AppRearMode != APP_MEDIA_MODE_MAX &&
                     m_Engines[ m_AppRearMode ] != NULL )
                {
//                    m_Engines[ m_AppRearMode ]->HideUI(display); // removed by Sergey 24.08.2013 for ITS#185556 
                    bCalledHideUI = true;
                }
            }

            AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO]);
            // { modified by cychoi 2013-07-16 for Prevent fix
            if(pVideoEngine) {
                pVideoEngine->HideScreen(display);
               // remove by edo.lee 2013.08.20
            }
            // } modified by cychoi 2013-07-16
			
            UpdateLayout( APP_MEDIA_MODE_MAX, display );
			m_bFrontAVPIsForeground = false;

            MP_HIGH << "END EVT_REQUEST_BG" << LOG_ENDL;
            
            break;
        }

        // { added by kihyung 2013.1.24 for ISV 70304
        case EVT_CLONE_NOTIFICATION:
        {
            if(!aEvent.Data().isEmpty())
            {
                EVENT_CLONE_NOTIFICATION_DATA_T *pEventData = (EVENT_CLONE_NOTIFICATION_DATA_T *)aEvent.Data().data();
                // { modified by cychoi 2013.08.25 for Prevent Fix
                #if (AVP_USE_NEW_BLEND_AFW == 0)
				AppMediaPlayer_EngineVideo *pVEngine = (AppMediaPlayer_EngineVideo*)GetEngine(APP_MEDIA_MODE_VIDEO); // added by eugene.seo 2013.04.16
				#endif
                // } modified by cychoi 2013.08.25

                if(pEventData)
                {
#ifndef DUAL_KEY
                    //added by suilyou 20131011 P2 no.25 START
                    if(m_eCloneStatus!=pEventData->sCloneStatus)
                    {
                        clearCCPJogKey();
                        clearRRCJogKey();

                        AppMediaPlayer_Controller_Audio *paController   = (AppMediaPlayer_Controller_Audio*)m_Engines[APP_MEDIA_MODE_AUDIO]->GetController();
                        if(paController->GetFfRewState())
                            paController->cancelFFRW();

                        AppMediaPlayer_Video_Controller *pvController   = (AppMediaPlayer_Video_Controller*)m_Engines[APP_MEDIA_MODE_VIDEO]->GetController();
                        if(pvController->isSeekMode())
                            pvController->cancel_ff_rew();

                    }
                    //added by suilyou 20131011 P2 no.25 END
#endif
#ifdef DUAL_KEY
                    if(m_eCloneStatus == CLONE_ON_FRONT && IsVideoModeEnterd(DISPLAY_REAR)&&IsSwapDCEnabled()) //modify by edo 20140217 its-224531
                    {
                        emit overplaneChanged();
                    }

                    if(m_eCloneStatus!=pEventData->sCloneStatus)
                    {
                        switch(m_eCloneStatus)
                        {
                            case CLONE_ON_FRONT:
                            {
                                switch(pEventData->sCloneStatus)
                                {
                                    case CLONE_OFF:
                                        MP_LOG << rearLCDMode << "is unlaunched rear LCD CLONE_ON_FRONT -> CLONE_OFF" << LOG_ENDL;
                                        rearLCDMode = APP_MEDIA_MODE_MAX;
                                        break;
                                }
                                break;
                            }
                            case CLONE_ON_REAR:
                            {
                                switch(pEventData->sCloneStatus)
                                {
                                    case CLONE_OFF:
                                        MP_LOG << frontLCDMode << "is unlaunched front LCD CLONE_ON_REAR -> CLONE_OFF" << LOG_ENDL;
                                        frontLCDMode = APP_MEDIA_MODE_MAX;
                                        break;
                                }
                                break;
                            }
                            case CLONE_OFF:
                            {
                                // { modified by wspark 2014.02.19 for ITS 225827
                                AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO]);
                                switch(pEventData->sCloneStatus)
                                {
                                    case CLONE_ON_FRONT:
                                        rearLCDMode=frontLCDMode ;
                                        MP_LOG << rearLCDMode << "is launched F/R LCD CLONE_OFF -> CLONE_ON_FRONT" << LOG_ENDL;
                                        pVideoEngine->initDirectAccessDisplayed (VIDEO_DISPLAY_REAR, pVideoEngine->isDirectAccessDisplayed(VIDEO_DISPLAY_FRONT));
                                        break;
                                    case CLONE_ON_REAR:
                                        frontLCDMode=rearLCDMode ;
                                        MP_LOG << rearLCDMode << "is launched F/R LCD CLONE_OFF -> CLONE_ON_REAR" << LOG_ENDL;
                                        pVideoEngine->initDirectAccessDisplayed (VIDEO_DISPLAY_FRONT, pVideoEngine->isDirectAccessDisplayed(VIDEO_DISPLAY_REAR));
                                        break;
                                }
                                // } modified by wspark
                                break;
                            }
                        }
                    }
#endif
                    m_eCloneStatus = pEventData->sCloneStatus;
                    if(m_eCloneStatus == CLONE_ON_FRONT && GetFrontView()!= APP_MEDIA_MODE_MAX ) //added by edo.lee 2013.07.06
					{
						//m_AppRearViewR = m_AppFrontViewR;                        
                        GetNotifier()->StopOSDForClone();// added by edo.lee 2013.04.10
                    }
                    else if(m_eCloneStatus == CLONE_ON_REAR && GetRearView()!= APP_MEDIA_MODE_MAX) //added by edo.lee 2013.07.06
					{
                        //m_AppFrontViewR = m_AppRearViewR;
                         GetNotifier()->StopOSDForClone();// added by edo.lee 2013.04.10
                    }
                    else {
                        m_isCloneTemp = false;//added by edo.lee 2013.04.21 for OSD
						//if(GetLastPlayMode()== APP_MEDIA_MODE_AUDIO)//remove by edo.lee 2013.07.08
	                    //    m_releaseCloneTimer.start();
						//else
						//	m_releasingClone = false;
                       // QTimer::singleShot(200, this, SLOT(onShowOSDReleaseClone()));
                    }
                        
                    MP_MEDIUM << "CAMS [EVT_CLONE_NOTIFICATION]:" << m_eCloneStatus << LOG_ENDL;//<< "F:" << m_AppFrontViewR << "R:" << m_AppRearViewR << LOG_ENDL;

                    // { modified by kihyung 2013.08.13 for ITS 0183749 
                    #if (AVP_USE_NEW_BLEND_AFW == 0)
					// { added by eugene.seo 2013.04.16
					if(pVEngine && pVEngine->isLaunchingScreenSettings() == true && (m_eCloneStatus == CLONE_ON_FRONT || m_eCloneStatus == CLONE_ON_REAR)) 						
					    m_pNotifier->setOverlayPlanes(OVERLAY_SPRITEC);
					// } added by eugene.seo 2013.04.16
					#endif
                    // } modified by kihyung 2013.08.13 for ITS 0183749 
                }
            }
            break;
        }

        case EVT_AV_ON:
        {
            MP_HIGH << "EVT_AV_ON" << LOG_ENDL;
            m_bAVOff = false;
            m_bMediaOnFromOff = true;
            break;
        }
        
        case EVT_AV_OFF:
        {
            MP_HIGH << "EVT_AV_OFF" << LOG_ENDL;
            m_bAVOff = true;
            cancelAllPressedKey(0);        // added by sangmin.seol 2014.02.15 for ITS 0225190 Key Cancel in the event of AV_OFF
            break;
        }

        // { added by kihyung 2013.10.31 for ITS 0205241 
        case EVT_FRONT_LCD_ON:
        {
            MP_HIGH << "EVT_FRONT_LCD_ON" << LOG_ENDL;
            m_bFrontLCDOff = false;
            EventHandler(aEvent, display); // added by kihyung 2013.11.06 for ITS 0206207 
            break;
        }

        case EVT_FRONT_LCD_OFF:
        {
            MP_HIGH << "EVT_FRONT_LCD_OFF" << LOG_ENDL;
            m_bFrontLCDOff = true;
            EventHandler(aEvent, display); // added by kihyung 2013.11.06 for ITS 0206207 
            break;
        }
        // } added by kihyung 2013.10.31 for ITS 0205241 

		//{added by aettie for ticker stop when DRS
        case EVT_DRS_SHOW:
        {
            MP_HIGH << "EVT_DRS_SHOW" << LOG_ENDL;
            m_bDRSShow = true;
            m_scrollingTicker = false;
            emit tickerChanged(m_scrollingTicker);

            break;
        }
        case EVT_DRS_HIDE:
        {
            MP_HIGH << "EVT_DRS_HIDE" << LOG_ENDL;
            m_bDRSShow = false;
	
            AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO]);
            // { added by wspark 2014.02.10 for ITS 224249
            if(pVideoEngine)
            {
                if(pVideoEngine->getIsShowLockoutView() == VIDEO_DISPLAY_FRONT)
                {
                    if(pVideoEngine->isVideoInFG(DISPLAY_REAR))
                    {
                        emit clearInputText(VIDEO_DISPLAY_FRONT);
                        pVideoEngine->initDirectAccessDisplayed (VIDEO_DISPLAY_FRONT, pVideoEngine->isDirectAccessDisplayed(VIDEO_DISPLAY_REAR)); // modified by wspark 2014.02.19 for ITS 225827 // added by cychoi 2014.02.14 for ITS 225321
                    }
                }
                else if(pVideoEngine->getIsShowLockoutView() == VIDEO_DISPLAY_REAR)
                {
                    if(pVideoEngine->isVideoInFG(DISPLAY_FRONT))
                    {
                        emit clearInputText(VIDEO_DISPLAY_REAR);
                        pVideoEngine->initDirectAccessDisplayed (VIDEO_DISPLAY_REAR, pVideoEngine->isDirectAccessDisplayed(VIDEO_DISPLAY_FRONT)); // modified by wspark 2014.02.19 for ITS 225827 // added by cychoi 2014.02.14 for ITS 225321
                    }
                }
            }
            // } added by wspark
            if(pVideoEngine) pVideoEngine->setIsShowLockoutView(DISPLAY_FRONT , false);
            if(pVideoEngine) pVideoEngine->setIsShowLockoutView(DISPLAY_REAR , false);

            m_scrollingTicker = true;
            emit tickerChanged(m_scrollingTicker);
            break;
        }
		//}added by aettie for ticker stop when DRS
		//added by edo.lee 2013.02.06
	    /*
	    case EVT_DRS_CHANGE:
		{			
			APP_MEDIA_MODE_T notifier_mode = APP_MEDIA_MODE_MAX;
			 if ( !aEvent.Data().isEmpty() )
            {
                APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();

                if ( pEventData )
                {
					//{ added by edo.lee 2013.02.04
                    switch(pEventData->sAppEntryPoint)
                    {
                    	case eAPP_AVP_VIDEO_MODE_DISC_DRS:
						case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
						case eAPP_AVP_VIDEO_MODE_USB1_DRS:
						case eAPP_AVP_VIDEO_MODE_USB2_DRS:
						case eAPP_AVP_VIDEO_MODE_AUX_DRS:
						{
							m_isLockoutMode = true;
							break;
						}
						default:
							m_isLockoutMode = false;
							break;
                    }
                }
			 }
			 EventHandler( aEvent, display );
			 break;
		}*/
        // } added by kihyung 2013.1.24

        default:
        {
#ifndef DUAL_KEY
        //added by suilyou 20130913 START
        KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
        switch(aEvent.GetEventId())// == EVT_KEY_CCP_JOG_DIAL_CENTER)
        {
            case EVT_KEY_CCP_JOG_DIAL_LEFT:
            {
                switch(eventData->keyStatus)
                {
                    case KEY_STATUS_PRESSED:
                    {
                        setJogKeyPressed(CCP_JOG_LEFT_PRESSED );
                        MP_LOG << "EVT_KEY_CCP_JOG_DIAL_LEFT:setJogKeyPressed key status" << getJogKeyPressed() << LOG_ENDL;
                        break;
                    }
                    case KEY_STATUS_RELEASED:
                    {
                        setJogKeyReleased(CCP_JOG_LEFT_PRESSED );
                         MP_LOG << "EVT_KEY_CCP_JOG_DIAL_LEFT:setJogKeyReleased key status" << getJogKeyPressed() << LOG_ENDL;
                        break;
                    }
                    default:
                    {
                        break; // make compiler happy
                    }
                }
                break;
            }
            case EVT_KEY_CCP_JOG_DIAL_RIGHT:
            {
                switch(eventData->keyStatus)
                {
                    case KEY_STATUS_PRESSED:
                    {
                        setJogKeyPressed(CCP_JOG_RIGHT_PRESSED );
                        MP_LOG << "EVT_KEY_CCP_JOG_DIAL_RIGHT:setJogKeyPressed key status" << getJogKeyPressed() << LOG_ENDL;
                        break;
                    }
                    case KEY_STATUS_RELEASED:
                    {
                        setJogKeyReleased(CCP_JOG_RIGHT_PRESSED );
                         MP_LOG << "EVT_KEY_CCP_JOG_DIAL_RIGHT:setJogKeyReleased key status" << getJogKeyPressed() << LOG_ENDL;
                        break;
                    }
                    default:
                    {
                        break; // make compiler happy
                    }
                }
                break;
            }
            case EVT_KEY_RRC_JOG_DIAL_RIGHT:
            {
                switch(eventData->keyStatus)
                {
                    case KEY_STATUS_PRESSED:
                    {
                        setJogKeyPressed(RRC_JOG_RIGHT_PRESSED );
                        MP_LOG << "EVT_KEY_RRC_JOG_DIAL_RIGHT:setJogKeyPressed key status" << getJogKeyPressed() << LOG_ENDL;
                        break;
                    }
                    case KEY_STATUS_RELEASED:
                    {
                        setJogKeyReleased(RRC_JOG_RIGHT_PRESSED );
                         MP_LOG << "EVT_KEY_RRC_JOG_DIAL_RIGHT:setJogKeyReleased key status" <<getJogKeyPressed() << LOG_ENDL;
                        break;
                    }
                    default:
                    {
                        break; // make compiler happy
                    }
                }
                break;
            }
            case EVT_KEY_RRC_JOG_DIAL_LEFT:
            {
                switch(eventData->keyStatus)
                {
                    case KEY_STATUS_PRESSED:
                    {
                        setJogKeyPressed(RRC_JOG_LEFT_PRESSED);
                        MP_LOG << "EVT_KEY_RRC_JOG_DIAL_LEFT:setJogKeyPressed key status" << getJogKeyPressed() << LOG_ENDL;
                        break;
                    }
                    case KEY_STATUS_RELEASED:
                    {
                        setJogKeyReleased(RRC_JOG_LEFT_PRESSED);
                         MP_LOG << "EVT_KEY_RRC_JOG_DIAL_LEFT:setJogKeyReleased key status" << getJogKeyPressed() << LOG_ENDL;
                        break;
                    }
                    default:
                    {
                        break; // make compiler happy
                    }
                }
                break;
            }
            case EVT_POST_EVENT:
            {
                if(aEvent.GetCommand() ==  EVT_JUKEBOX_COPY_END)
                {
                    MP_HIGH << "EVT_JUKEBOX_COPY_END" << LOG_ENDL;
                    emit closeCopyCancelConfirmPopup(); // modified for ITS 0217509
                }
                break;
            }
            default:
            {
                break; // make compiler happy
            }
        }
#endif
        //added by suilyou 20130913 END
            MP_LOG << "Transit to Engines event:" << aEvent.GetEventId() << LOG_ENDL;
            EventHandler( aEvent, display );
            break;
        }
    }
}
// } modified by Sergey for SwapDC

void
AppMediaPlayer_EngineMain::TopEventHandler( Event &aEvent, DISPLAY_T display )
{
    // { added by kihyung 2013.1.24 for ISV 70304
    //APP_MEDIA_MODE_T eCurrMode = GetActiveMode(); // removed by edo.lee 2013.03.05
	//MP_LOG<<"m_AppFrontViewR"<<m_AppFrontViewR<<"m_AppRearViewR"<<m_AppRearViewR << LOG_ENDL;
	
	//{added by edo.lee 2013.07.15 STATIC TEST
	APP_MEDIA_MODE_T engineType = GetViewR(display);
    if(engineType != APP_MEDIA_MODE_MAX)//(display == DISPLAY_FRONT && m_AppFrontViewR != APP_MEDIA_MODE_MAX)
    {
        //m_Engines[m_AppFrontViewR]->EventHandler(aEvent, display);
        //{ added by edo.lee 2013.03.08 when photo clone mode , do jog event
        DISPLAY_T cDisp = display;
		 // { modified by edo.lee 2013.03.12
		if(aEvent.GetEventId() != EVT_REQUEST_PRE_FG && aEvent.GetEventId() != EVT_REQUEST_FG) // modified by Sergey 26.09.2013 for ITS#191661
        {
	        if(engineType == APP_MEDIA_MODE_PHOTO_FRONT)
				cDisp = DISPLAY_FRONT;
			else if(engineType == APP_MEDIA_MODE_PHOTO_REAR)
				cDisp = DISPLAY_REAR;
        }else
    	{
#ifndef DUAL_KEY
            //added by suilyou 20131002 ITS 0192624 START
            if(display==DISPLAY_FRONT)
                clearCCPJogKey();
            else
                clearRRCJogKey();
            //added by suilyou 20131002 ITS 0192624 END
#endif
            //added by suilyou 20130927 START ITS 0186938
            APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
            APP_MEDIA_MODE_T nMediaMode = GetAVMediaMode(pEventData->sAppEntryPoint);

            //added by suilyou 20131011 END
            switch(nMediaMode)
            {
            case APP_MEDIA_MODE_AUDIO:
            {
                AppMediaPlayer_Controller_Audio *pController   = (AppMediaPlayer_Controller_Audio*)m_Engines[nMediaMode]->GetController();
                if(pController->GetFfRewState())
                    pController->cancelFFRW();
                break;
            }
            case APP_MEDIA_MODE_VIDEO:
            {
                AppMediaPlayer_Video_Controller *pController   = (AppMediaPlayer_Video_Controller*)m_Engines[nMediaMode]->GetController();
                if(pController->isSeekMode())
                    pController->cancel_ff_rew();
                break;
            }
            default:
            {
                break;
            }
            }

                    //added by suilyou 20130927 END
    		if(display == DISPLAY_FRONT && m_AppFrontView != APP_MEDIA_MODE_MAX)
                        m_Engines[m_AppFrontView]->EventHandler(aEvent, cDisp);
			else if(display == DISPLAY_REAR && m_AppRearView != APP_MEDIA_MODE_MAX)
				m_Engines[m_AppRearView]->EventHandler(aEvent, cDisp);

			return;
    	}
		// } modified by edo.lee 2013.03.12
		//{ added by edo.lee
		
		if(engineType != APP_MEDIA_MODE_MAX)
			m_Engines[engineType]->EventHandler(aEvent, cDisp); //modified by edo.lee 2013.03.08
		//}added by edo.lee 2013.07.15 STATIC TEST
    }
	//added by edo.lee 2013.03.18
	else //enter the media from other module
	{

		if(display == DISPLAY_FRONT && m_AppFrontView != APP_MEDIA_MODE_MAX)
			m_Engines[m_AppFrontView]->EventHandler(aEvent, display);
		else if(display == DISPLAY_REAR && m_AppRearView != APP_MEDIA_MODE_MAX)
			m_Engines[m_AppRearView]->EventHandler(aEvent, display);
	}
	//added by edo.lee 2013.03.18
    //else if(display == DISPLAY_REAR && m_AppRearViewR != APP_MEDIA_MODE_MAX)
    //{
    //   m_Engines[m_AppRearViewR]->EventHandler(aEvent, display);
    //}
    // } added by kihyung 2013.1.24 for ISV 70304
}

//{ added by yongkyun.lee 20130702 for : NO CR - None AV OSD
bool
AppMediaPlayer_EngineMain::GetNotifyAVModeState()
{
    switch(m_isNotifyAVMode)
    {
        case        MODE_STATE_DISC_AUDIO  :
        case        MODE_STATE_DISC_VIDEO  :
        case        MODE_STATE_USB1_AUDIO  :
        case        MODE_STATE_USB1_VIDEO  :
        case        MODE_STATE_USB2_AUDIO  :
        case        MODE_STATE_USB2_VIDEO  :
        case        MODE_STATE_IPOD1_AUDIO :
        case        MODE_STATE_IPOD1_PANDORA   :
        case        MODE_STATE_AHA_RADIO_IPOD1 :
        case        MODE_STATE_IPOD2_AUDIO :
        case        MODE_STATE_IPOD2_PANDORA   :
        case        MODE_STATE_AHA_RADIO_IPOD2 :
        case        MODE_STATE_VCDC        :
        case        MODE_STATE_JUKEBOX_AUDIO   :
        case        MODE_STATE_JUKEBOX_VIDEO   :
        case        MODE_STATE_BT_AUDIO    :
        case        MODE_STATE_BT_PANDORA  :
        case        MODE_STATE_AHA_RADIO_BT:
        case        MODE_STATE_AUX1_AUDIO  :
        case        MODE_STATE_AUX1_VIDEO  :
        case        MODE_STATE_AUX2_AUDIO  :
        case        MODE_STATE_AUX2_VIDEO  :
        {
			MP_LOG<<"av mode is correct.." << LOG_ENDL;
            return true;
        }        
        default:
            return false;
    }

    return false;
    
}


MODE_STATE_T
AppMediaPlayer_EngineMain::GetNotifyAVMode()
{
     return m_isNotifyAVMode;
}
//} added by yongkyun.lee 20130702 


void
AppMediaPlayer_EngineMain::EventHandler( Event &aEvent, DISPLAY_T display )
{
    APP_MEDIA_MODE_T mode          = APP_MEDIA_MODE_MAX;
    APP_MEDIA_MODE_T notifier_mode = APP_MEDIA_MODE_MAX;

    //{removed by junam 2013.11.29 for tune
    //if(aEvent.GetEventId() == EVT_AV_MODE_CHANGE)
    //{
    //MP_HIGH << "START EVT_AV_MODE_CHANGE" << LOG_ENDL;
    //AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_Engines[APP_MEDIA_MODE_AUDIO]);
    //// { modified by cychoi 2013.11.16 for Prevent Fix
    //if(pAudioEngine)
    //{
    //    pAudioEngine->clearTuneDelayTimer(); // added by suilyou 20131112 ITS 0208066
    //}
    // } modified by cychoi 2013.11.16
    //}
    //}removed by junam

    //{ added by yongkyun.lee 20130702 for :  NO CR - None AV OSD
    if ( aEvent.GetEventId() == EVT_NOTIFY_AV_MODE)
    {
        if(!aEvent.Data().isEmpty())
        {
            EVT_COMMON_BASE_EVENT_DATA_T* pData = (EVT_COMMON_BASE_EVENT_DATA_T*)aEvent.Data().data();
            if(m_isNotifyAVMode != (MODE_STATE_T)pData->iEventData01)
            {
                emit signalModeChange();
                m_isNotifyAVMode = (MODE_STATE_T)pData->iEventData01;
            }
            // MP_HIGH << "EVT_NOTIFY_AV_MODE. AVP Audio Mode...=" << pData->iEventData01 << m_isNotifyAVMode << LOG_ENDL;
        }
    }
    //} added by yongkyun.lee 20130702 

    if ( (aEvent.GetEventId() == EVT_AV_MODE_CHANGE) || (aEvent.GetEventId() == EVT_AUTO_PLAY_IN_BG) )
    {
        if ( !aEvent.Data().isEmpty() )
        {

			m_isCompleteModeChange = false;//added by edo.lee 2013.05.01
			
            EVENT_MODE_CHANGED_DATA_T* pData = (EVENT_MODE_CHANGED_DATA_T*) aEvent.Data().data();

            // { added by cychoi 2013-07-15 for Prevent fix
            if (pData == NULL)
            {
                return;
            }
            // } added by cychoi 2013-07-15

            MP_HIGH << "EVT_AV_MODE_CHANGE" << pData->sMode << LOG_ENDL;


            AUXInit(pData->sMode); // added by hyochang.ryu 20130703
			//added by edo.lee 2013.06.02
            //if(!m_bFrontAVPIsForeground && pData->sMode != MODE_STATE_NONE && (m_AppFrontView < APP_MEDIA_MODE_PHOTO_FRONT|| m_AppRearView < APP_MEDIA_MODE_PHOTO_FRONT))) //modified by wonseok.heo 2013.06.13
            if( m_bFrontAVPIsForeground)
			{
				if((m_AppFrontView < APP_MEDIA_MODE_PHOTO_FRONT || m_AppRearView < APP_MEDIA_MODE_PHOTO_FRONT)  // already shown
					|| m_bFgEventStart)// same time with FG event  //modified by edo.lee 2013.10.16 ITS 0196016 
				{
					if (pData->sMode != MODE_STATE_NONE && pData->sMode != MODE_STATE_AVOFF)//modified by edo.lee 2013.07.07
					{
						MP_LOG<<"current AVP state is FG .... so MODE_CHANGE is ignored" << LOG_ENDL;
						return;
					}
				}				
			}
			//added by edo.lee 2013.06.02

			if(GetFrontMode()==APP_MEDIA_MODE_VIDEO || GetFrontMode()==APP_MEDIA_MODE_AUDIO)	
			{
				MP_LOG << "EVT_AV_MODE_CHANGE video front" << LOG_ENDL;
				display = DISPLAY_FRONT;
			}
			else if(GetRearMode()==APP_MEDIA_MODE_VIDEO || GetRearMode()==APP_MEDIA_MODE_AUDIO)	
			{
				MP_LOG << "EVT_AV_MODE_CHANGE video rear" << LOG_ENDL;
				display = DISPLAY_REAR;
			}    

			//m_isVideoRearEntered  = false; //remove by edo.lee 2013.07.02			
			//m_isVideoFrontEntered = false;
			if(m_isNoneState)
				m_isMuteState = false;//added by edo.lee 2013.10.07 ITS 186267
				
			setIsFromOtherAV(m_isNoneState);//added by edo.lee 2013.03.11
			m_isNoneState = false; //added by edo.lee 2013.03.11

                        /*if(getPowerOffBTCall() == false)//moved by taihyun.ahn 2013.10.28 for its 198808,  0198848 
			{
			    setPlayOnBTCall();//added by edo.lee 2013.09.15 ITS 0190407 
			}else
                setPowerOffBTCall(false); */

            //{ modified by yongkyun.lee 2013-08-27 for :  ITS 186453-186494
            if((m_bAVOff == true) && (pData->sMode != MODE_STATE_AVOFF) )//{ modified by yongkyun.lee 2013-09-08 for :  NO CR
            {
                m_bAVOff = false;
                m_bMediaOnFromOff = true;
            }
            //} modified by yongkyun.lee 2013-08-27 
            
            //added by edo.lee 2013.09.21 ITS 0188286 
            if(pData->sMode != MODE_STATE_AVOFF && pData->sMode != MODE_STATE_NONE)
            {
                // { modified by oseong.kwon 2014.09.18 for ITS 248589 fail to unmute BT Audio after ModeChanged
                // added by sangmin.seol 2014.06.18 ITS 0240618
                if( pData->sMode == MODE_STATE_BT_AUDIO ) //{ modified by yongkyun.lee 2013-12-19 for : ITS 216290
                {
                    setIsTermOnTrackChange(false);
                }
                else
                {
                    AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_Engines[APP_MEDIA_MODE_AUDIO]);
                    if(pAudioEngine && pAudioEngine->getLastAVMode() != pData->sMode) // added by sangmin.seol 2014.06.18 ITS 0240618
                    {
                        setIsTermOnTrackChange(true);
                    }
                }
                // } modified by oseong.kwon 2014.09.18
            }
            //added by edo.lee 2013.09.21 ITS 0188286 

            if( pData->sMode != MODE_STATE_DISC_AUDIO )// added by yongkyun.lee 20130524 for : ITS 169767
     		    m_playInfoDisplay = DISPLAY_FRONT_REAR;//added by edo.lee 2013.04.20
     		
            //if (pData != NULL) // commented by cychoi 2013-07-15 for Prevent fix
            {
                SetLastAVEntryPoint(mapToEntryState(pData->sMode)); // modified by Sergey 28.04.2013

                switch(pData->sMode)
                {
                case MODE_STATE_AUX1_AUDIO: // fall through
                case MODE_STATE_AUX2_AUDIO: // fall through
                case MODE_STATE_JUKEBOX_AUDIO:
                case MODE_STATE_USB1_AUDIO:
                case MODE_STATE_USB2_AUDIO:
                case MODE_STATE_IPOD1_AUDIO:
                case MODE_STATE_IPOD2_AUDIO:
                case MODE_STATE_BT_AUDIO:
                case MODE_STATE_DISC_AUDIO:
                {
                    notifier_mode = APP_MEDIA_MODE_AUDIO; // added by wspark 2013.04.20
                    
                    // { modified by kihyung 2013.08.23 for ITS 0186192
                    AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_Engines[APP_MEDIA_MODE_AUDIO]);
                    if(!pAudioEngine || (pAudioEngine->isStopped() == false && pAudioEngine->getLastAVMode() == pData->sMode))
					{
                        MP_LOG << "AUDIO EVT_AV_MODE_CHANGE return. Curr:" << pData->sMode << LOG_ENDL;
					//{added by edo.lee 2013.09.09 ITS 0188604 
					if(pAudioEngine->getLastAVMode() == pData->sMode)
						pAudioEngine->showCurrentOSD();
				    //}added by edo.lee 2013.09.09 ITS 0188604 
                        break;
                    }
                    // } modified by kihyung 2013.08.23 for ITS 0186192
                    
                    //added by edo.lee 2013.07.07
                    m_isVideoRearEntered  = false;			
         			m_isVideoFrontEntered = false;
					//added by edo.lee 2013.07.07
                    //notifier_mode = APP_MEDIA_MODE_AUDIO; // deleted by wspark 2013.04.20
					//GetNotifier()->onSetDisplayOSD(true);//remove by edo.lee 2013.06.13
                    m_pBootstrap->loadComponents(notifier_mode, display);
                    emit clearPlaybackJog(); //added by suilyou 20131007
					//if(!getIsFromOtherAV())  //remove by edo.lee 2013.04.05
					if(MODE_STATE_BT_AUDIO == pData->sMode)//added by edo.lee 2013.04.05
						ConnectSignalToUISH(display, notifier_mode);//added by edo.lee 2013.03.19
                    break;
                }
                case MODE_STATE_AUX1_VIDEO: // fall through
                case MODE_STATE_AUX2_VIDEO: // fall through
                case MODE_STATE_DISC_VIDEO:
                case MODE_STATE_USB1_VIDEO:
                case MODE_STATE_USB2_VIDEO:
                case MODE_STATE_JUKEBOX_VIDEO:
                {
                    notifier_mode = APP_MEDIA_MODE_VIDEO; // added by wspark 2013.04.20

                    // { modified by kihyung 2013.08.23 for ITS 0186192
                    AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO]);
                    if(!pVideoEngine || (pVideoEngine->IsStopped() == false && pVideoEngine->getLastAVMode() == pData->sMode)) 
                    {
                        MP_LOG << "VIDEO EVT_AV_MODE_CHANGE return.    Curr:" << pData->sMode << LOG_ENDL;
						//{added by edo.lee 2013.09.09 ITS 0188604 
					    if(pVideoEngine->getLastAVMode() == pData->sMode)
						    pVideoEngine->showCurrentOSD();
				        //}added by edo.lee 2013.09.09 ITS 0188604 
                        break;
                    }
                    // } modified by kihyung 2013.08.23 for ITS 0186192
                    
                    //added by edo.lee 2013.07.02
                    m_isVideoRearEntered  = false;			
         			m_isVideoFrontEntered = false;
					//added by edo.lee 2013.07.02
					if(display == DISPLAY_REAR)
					{
						MP_LOG<<"rear true" << LOG_ENDL;
						m_isVideoRearEntered = true;
						m_currentVideoMode = VIDEO_DISPLAY_REAR;						
					}
					else
					{
						m_isVideoFrontEntered = true;
						m_currentVideoMode = VIDEO_DISPLAY_FRONT;
					}
					//GetNotifier()->onSetDisplayOSD(true);//remove by edo.lee 2013.06.13
					
					// removed by Sergey 18.05.2013
                    if(pVideoEngine->getIsShowLockoutView() == VIDEO_DISPLAY_FRONT) {
                        // commented by Sergey 25.09.2013. Should not show UI when in BG
                        // SetActiveMode(mapToEntryState(pData->sMode), DISPLAY_REAR, true); // modified by kihyung 2013.4.9 for ISV 78237
                    }
					else if(pVideoEngine->getIsShowLockoutView() == VIDEO_DISPLAY_REAR) {
						// commented by Sergey 25.09.2013. Should not show UI when in BG
						// SetActiveMode(mapToEntryState(pData->sMode), DISPLAY_FRONT, true); // modified by kihyung 2013.4.9 for ISV 78237
					}
                    else  
                    {
                        m_pBootstrap->loadComponents(notifier_mode, display);
                        emit clearPlaybackJog(); //added by suilyou 20131007
						//if(!getIsFromOtherAV())  //remove by edo.lee 2013.04.05
						//	ConnectSignalToUISH(display, notifier_mode); //remove by edo.lee 2013.04.05
                    }
                    break;
                }
                case MODE_STATE_AVOFF:
				case MODE_STATE_NONE:
                    m_pNotifier->setClusterUpdate(false); // added by wspark 2013.04.10 for cluster update
                    MP_LOG << "wspark clusterupdate off!!" << LOG_ENDL;
                    notifier_mode = m_pNotifier->GetActiveMode();					
					m_isNoneState = true;//added by edo.lee 2013.03.11
					setIsFromOtherAV(true);//added by edo.lee 2013.03.11
					emit clearPlaybackJog(); //added by edo.lee 2013.06.07
                    break;
                default:
                    // Should use current active mode which is already set.
                    notifier_mode = (display == DISPLAY_FRONT) ? m_AppFrontMode : m_AppRearMode;
                    break;
                }
            }
        }

        if ( notifier_mode == APP_MEDIA_MODE_AUDIO || notifier_mode == APP_MEDIA_MODE_VIDEO )
        {
            m_LastPlayerMode = notifier_mode;
        }
        
        m_pNotifier->SetActiveMode( notifier_mode );
    }

    if ( display == DISPLAY_FRONT &&
        m_AppFrontMode < APP_MEDIA_MODE_MAX &&
        m_Engines[ m_AppFrontMode ])
    {
        m_Engines[ m_AppFrontMode ]->EventHandler( aEvent, display );
        mode = m_AppFrontMode;
    }

    if ( display == DISPLAY_REAR &&
        m_AppRearMode < APP_MEDIA_MODE_MAX &&
        m_Engines[ m_AppRearMode ])
    {
        m_Engines[ m_AppRearMode ]->EventHandler( aEvent, display );
        mode = m_AppRearMode;
    }

    // Lets chance to handle events to engines which active in BG state
    for ( unsigned int index = 0; index < APP_MEDIA_MODE_MAX; index++ )
    {
        if ( index != mode )
        {
            if ( m_Engines[ index ] )
            {
                m_Engines[ index ]->EventHandler( aEvent, display );
            }
        }
    }

    if(aEvent.GetEventId() == EVT_AV_MODE_CHANGE) 
    {
     	m_isCompleteModeChange = true;//added by edo.lee 2013.05.01
     	resetMediaOnFromOff(); // added by kihyung 2013.06.10 for ITS 0172955
     	resetMediaOnFromCamOff();
    }
}

APP_ENTRY_POINT_T AppMediaPlayer_EngineMain::mapToEntryState(MODE_STATE_T pData)
{
    APP_ENTRY_POINT_T result = eAPP_NONE;

    switch(pData)
    {
        // case eAPP_MODE_DYNAMICS_DISC: // added by wonseok.heo for Disc IN OUT TEST 2013.07.20 // removed by kihyung 2013.08.01 for static test
     	case MODE_STATE_JUKEBOX_AUDIO:
            result = eAPP_AVP_AUDIO_MODE_JUKEBOX;
            break;
        case MODE_STATE_USB1_AUDIO:
            result = eAPP_AVP_AUDIO_MODE_USB1;
            break;
        case MODE_STATE_USB2_AUDIO:
            result = eAPP_AVP_AUDIO_MODE_USB2;
            break;
        case MODE_STATE_DISC_AUDIO:
            result = eAPP_AVP_AUDIO_MODE_DISC;
            break;
        case MODE_STATE_IPOD1_AUDIO:
            result = eAPP_AVP_AUDIO_MODE_IPOD1;
            break;
		case MODE_STATE_IPOD2_AUDIO:
            result = eAPP_AVP_AUDIO_MODE_IPOD2;
            break;
		case MODE_STATE_AUX1_AUDIO:
            result = eAPP_AVP_AUDIO_MODE_AUX;
            break;
		case MODE_STATE_AUX2_AUDIO:
            result = eAPP_AVP_AUDIO_MODE_AUX;
            break;
		case MODE_STATE_BT_AUDIO:
            result = eAPP_AVP_MODE_BLUETOOTH;
            break;
			
        case MODE_STATE_JUKEBOX_VIDEO:
            result = eAPP_AVP_VIDEO_MODE_JUKEBOX;
            break;
        case MODE_STATE_USB1_VIDEO:
            result = eAPP_AVP_VIDEO_MODE_USB1;
            break;
        case MODE_STATE_USB2_VIDEO:
            result = eAPP_AVP_VIDEO_MODE_USB2;
            break;
        case MODE_STATE_DISC_VIDEO:
            result = eAPP_AVP_VIDEO_MODE_DISC;
            break;
        case MODE_STATE_AUX1_VIDEO:
            result = eAPP_AVP_VIDEO_MODE_AUX;
            break;
		case MODE_STATE_AUX2_VIDEO:
            result = eAPP_AVP_VIDEO_MODE_AUX;
            break;
        default:
            MP_LOG << "Unsupported mode state:" << pData << LOG_ENDL;
            break;
    }

    return result;
}
//modified by edo.lee 2013.09.04 ITS 0188044
void AppMediaPlayer_EngineMain::setIsChangeToDriving(APP_ENTRY_POINT_T entryPoint, DISPLAY_T display)
{
    bool bResult = false;	
    AppMediaPlayer_EngineVideo* pVengine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO]);

    APP_ENTRY_POINT_T last_entry = display == DISPLAY_FRONT ?  m_LastFrontEntryPoint : m_LastRearEntryPoint;//GetLastAVEntryPoint(false);
	MP_LOG<<entryPoint <<" "<< last_entry << LOG_ENDL;
	switch(entryPoint)
	{
	case eAPP_AVP_VIDEO_MODE_DISC_DRS:
		bResult = last_entry == eAPP_AVP_VIDEO_MODE_DISC ? true : false;
		break;
	case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
		bResult =  last_entry == eAPP_AVP_VIDEO_MODE_JUKEBOX ? true : false;
		break;
	case eAPP_AVP_VIDEO_MODE_USB1_DRS:
		bResult = last_entry == eAPP_AVP_VIDEO_MODE_USB1 ? true : false;
		break;
	case eAPP_AVP_VIDEO_MODE_USB2_DRS:
		bResult = last_entry == eAPP_AVP_VIDEO_MODE_USB2 ? true : false;
		break;
    // { added by shkim for ITS 210827 Enter DRS Mode -> AUX Modearea shown issue
    case eAPP_AVP_VIDEO_MODE_AUX_DRS:
            bResult = last_entry == eAPP_AVP_VIDEO_MODE_AUX ? true : false;
            break;
    // } added by shkim for ITS 210827 Enter DRS Mode -> AUX Modearea shown issue
	default:
		MP_LOG<<"default " << LOG_ENDL;
		break;
	}
	m_isChangeToDriving = bResult && pVengine->isVideoInFG(display);
}
void AppMediaPlayer_EngineMain::setIsChangeToParking(APP_ENTRY_POINT_T entryPoint, DISPLAY_T display)
{
    bool bResult = false;		
    AppMediaPlayer_EngineVideo* pVengine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO]);
    APP_ENTRY_POINT_T last_entry = display == DISPLAY_FRONT ?  m_LastFrontEntryPoint : m_LastRearEntryPoint;//GetLastAVEntryPoint(false);
	MP_LOG<<entryPoint <<" "<< last_entry << LOG_ENDL;
	switch(entryPoint)
	{
	case eAPP_AVP_VIDEO_MODE_DISC:
		bResult = last_entry == eAPP_AVP_VIDEO_MODE_DISC_DRS ? true : false;
		break;
	case eAPP_AVP_VIDEO_MODE_JUKEBOX:
		bResult =  last_entry == eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS ? true : false;
		break;
	case eAPP_AVP_VIDEO_MODE_USB1:
		bResult = last_entry == eAPP_AVP_VIDEO_MODE_USB1_DRS ? true : false;
		break;
	case eAPP_AVP_VIDEO_MODE_USB2:
		bResult = last_entry == eAPP_AVP_VIDEO_MODE_USB2_DRS ? true : false;
		break;
    // { added by shkim for ITS 210827 Enter DRS Mode -> AUX Modearea shown issue
    case eAPP_AVP_VIDEO_MODE_AUX:
            bResult = last_entry == eAPP_AVP_VIDEO_MODE_AUX_DRS ? true : false;
            break;
    // } added by shkim for ITS 210827 Enter DRS Mode -> AUX Modearea shown issue
	default:
		MP_LOG<<"default " << LOG_ENDL;
		break;

	}
	m_isChangeToParking = bResult &&  pVengine->isVideoInFG(display);
}
//}modified by edo.lee 2013.09.04 ITS 0188044
AppMediaPlayer_EngineBase *
AppMediaPlayer_EngineMain::GetEngine( APP_MEDIA_MODE_T mode )
{
   if ( mode == APP_MEDIA_MODE_MAX )
      return NULL;

   return m_Engines[mode];
}

AppMediaPlayer_MediaManager *
AppMediaPlayer_EngineMain::GetMediaManager()
{
   return m_pMediaManager;
}
//modified by edo.lee 2013.04.09
void
AppMediaPlayer_EngineMain::connectImageEventToUISH()
{
		connect( m_Engines[APP_MEDIA_MODE_PHOTO_FRONT], SIGNAL(NotifyUISH(Event)), this, SLOT(doNotifyUISHFront(Event)));
		connect( m_Engines[APP_MEDIA_MODE_PHOTO_FRONT], SIGNAL(NotifyUISH(Event,DISPLAY_T)), this, SLOT(doNotifyUISH(Event,DISPLAY_T)));
		connect( m_Engines[APP_MEDIA_MODE_PHOTO_REAR], SIGNAL(NotifyUISH(Event)), this, SLOT(doNotifyUISHFront(Event)));
		connect( m_Engines[APP_MEDIA_MODE_PHOTO_REAR], SIGNAL(NotifyUISH(Event,DISPLAY_T)), this, SLOT(doNotifyUISH(Event,DISPLAY_T)));
}
//added by edo.lee

CTrackerAbstractor* AppMediaPlayer_EngineMain::GetTrackerAbs()
{
    if(m_pTrackerAbs == NULL) 
    {
        m_pTrackerAbs = new CTrackerAbstractor();
        if(m_pTrackerAbs) {
            connect(m_pTrackerAbs, SIGNAL(ScanCompleted(ETrackerAbstarctor_VolumeType,int,int)), this, SLOT(TAScanCompleted(ETrackerAbstarctor_VolumeType,int,int)));
            // Removed by kihyung 2013.07.31 for ISV 88268
            // connect(m_pTrackerAbs, SIGNAL(MediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)), this, SLOT(TAMediaFileFound(ETrackerAbstarctor_VolumeType,int,int,int,int)));
            m_pTrackerAbs->SetCountryVariant(AppEngineBase::GetCountryVariant()); // added by Dmitry 28.05.13
            m_pTrackerAbs->SetAVNSWVersion(GetSWVersion());
            // added by jaehwan.lee 2013.10.03
            m_pTrackerAbs->SetLanguage(GetLanguage());
        }
    }
    
    return m_pTrackerAbs;
}

bool AppMediaPlayer_EngineMain::isTAScanComplete(ETrackerAbstarctor_VolumeType volumeType)
{
    MP_LOG << volumeType << LOG_ENDL;
    
    if(volumeType == eJukeBox) 
    {
        return m_bJukeboxTAScanComplete;
    }
    else if(volumeType == eUSB_FRONT)
    {
        return m_bUSB1TAScanComplete;
    }
    else if(volumeType == eUSB_REAR)
    {
        return m_bUSB2TAScanComplete;
    }
    else 
    {
        return false;
    }
}

bool AppMediaPlayer_EngineMain::isTAReadComplete(ETrackerAbstarctor_VolumeType volumeType)
{
    MP_LOG << volumeType << LOG_ENDL;
    
    if(volumeType == eJukeBox) 
    {
        return m_bJukeboxTAReadComplete;
    }
    else if(volumeType == eUSB_FRONT)
    {
        return m_bUSB1TAReadComplete;
    }
    else if(volumeType == eUSB_REAR)
    {
        return m_bUSB2TAReadComplete;
    }
    else 
    {
        return false;
    }
}

void AppMediaPlayer_EngineMain::requestFMReadTAScan(ETrackerAbstarctor_VolumeType volumeType)
{
    MP_LOG << volumeType << LOG_ENDL;
    
    if(volumeType == eJukeBox) 
    {
        if(m_bJukeboxTAScanComplete == true && m_bJukeboxTAReadComplete == false) {
            Event tEvent(EVT_POST_EVENT, GetThisAppId(), APP_FMGR, EVT_FM_MEDIA_SCAN_READ_START, QByteArray((char*)&volumeType, sizeof(volumeType)));
            emit NotifyUISH(tEvent);
            usleep(500000);
        }
    }
    else if(volumeType == eUSB_FRONT)
    {
        if(m_bUSB1TAScanComplete == true && m_bUSB1TAReadComplete == false) {
            Event tEvent(EVT_POST_EVENT, GetThisAppId(), APP_FMGR, EVT_FM_MEDIA_SCAN_READ_START, QByteArray((char*)&volumeType, sizeof(volumeType)));
            emit NotifyUISH(tEvent);
            usleep(500000);
        }
    }
    else if(volumeType == eUSB_REAR)
    {
        if(m_bUSB2TAScanComplete == true && m_bUSB2TAReadComplete == false) {
            Event tEvent(EVT_POST_EVENT, GetThisAppId(), APP_FMGR, EVT_FM_MEDIA_SCAN_READ_START, QByteArray((char*)&volumeType, sizeof(volumeType)));   // modified by sungha.choi 2013.08.25
            emit NotifyUISH(tEvent);
        }
    }
}

bool AppMediaPlayer_EngineMain::requestTrackerAbsRescan(ETrackerAbstarctor_VolumeType volumeType, bool bSendCapa)
{
    MP_LOG << volumeType << LOG_ENDL;

    bool bResult = false;
    
    if(volumeType == eJukeBox && m_bRequestedJukeboxRescan == false) 
    {
        if(bSendCapa == true) 
        {
            MP_LOG << "EVT_JUKEBOX_CAPACITY_CHANGE" << LOG_ENDL;
            Event tEvent(EVT_POST_EVENT, GetThisAppId(), APP_SETTINGS, EVT_JUKEBOX_CAPACITY_CHANGE);
            emit NotifyUISH(tEvent);
        }
        
        CTrackerAbstractor *pTrackerAbs = GetTrackerAbs();
        if(pTrackerAbs) {
            m_bRequestedJukeboxRescan = true;
            bResult = pTrackerAbs->StartScanFileSystem(volumeType, true);
        }
    }

    return bResult;
}

// { added by kihyung 2013.07.05 for ISV 84474
bool AppMediaPlayer_EngineMain::startTrackerAbsScan(ETrackerAbstarctor_VolumeType volumeType)
{
    CTrackerAbstractor *pTrackerAbs = GetTrackerAbs();
    if(pTrackerAbs) {
        MP_HIGH << volumeType << LOG_ENDL;
        if(volumeType == eJukeBox) m_bRequestedJukeboxRescan = true;
        if(pTrackerAbs->StartScanFileSystem(volumeType) == true) {
            return true;
        }
    }

    return false;
}
// } added by kihyung 2013.07.05 for ISV 84474

// { modified by kihyung 2013.07.25 for ITS 0180896 
void AppMediaPlayer_EngineMain::stopTrackerAbsScan(bool bJukebox, USB_PORT_T portNumber)
{
    CTrackerAbstractor *pTrackerAbs = GetTrackerAbs();
    if(pTrackerAbs) 
    {
        MP_HIGH << bJukebox << portNumber << LOG_ENDL;
        
        if(bJukebox == false) {
            if(portNumber == eUSB_PORT1) {
                m_bUSB1TAScanComplete = false;
                m_bUSB1TAReadComplete = false;
                pTrackerAbs->StopScanFileSystem(eUSB_FRONT);
            }
            else if(portNumber == eUSB_PORT2) {
                m_bUSB2TAScanComplete = false;
                m_bUSB2TAReadComplete = false;                    
                pTrackerAbs->StopScanFileSystem(eUSB_REAR);
            }
        }
        else {
            m_bJukeboxTAScanComplete = false;
            m_bJukeboxTAReadComplete = false;
            pTrackerAbs->StopScanFileSystem(eJukeBox);
        }
    }
    else {
        MP_HIGH << "pTrackerAbs is NULL" << LOG_ENDL;
    }
}
// } modified by kihyung 2013.07.25 for ITS 0180896 

void AppMediaPlayer_EngineMain::completedTARead(ETrackerAbstarctor_VolumeType volumeType, int bSuccess)
{   
    MP_LOG << volumeType << bSuccess << LOG_ENDL;
    
    if(volumeType == eJukeBox) 
    {
        m_bJukeboxTAReadComplete = true;
    }
    else if(volumeType == eUSB_FRONT) 
    {
        m_bUSB1TAReadComplete = true;
    }
    else if(volumeType == eUSB_REAR) 
    {
        m_bUSB2TAReadComplete = true;
    }

    // { added by kihyung 2013.3.16 for ISV 76113
    //((AppMediaPlayer_Controller_Audio*)m_Engines[APP_MEDIA_MODE_AUDIO]->GetController())->resumeMiner();  // removed by eugene.seo 2013.03.16
    // } added by kihyung
}
//modified by edo.lee 2013.04.20
void
AppMediaPlayer_EngineMain::setUnMuteState(bool isAudio)
{
    MP_LOG <<"m_isMuteState: " << m_isMuteState << "m_isNoneState: " << m_isNoneState
         << "m_isKeepSystemMute: " <<m_isKeepSystemMute << LOG_ENDL;
	if(m_isMuteForSeek)
		setUnMuteForSeek(isAudio);

	if(m_isMuteState && !m_isNoneState && !getKeepSystemMute())//added by edo.lee 2013.09.05 ITS 0188328
	{
		EVENT_AUDIO_PATH_REQUEST_DATA requestData;
		
		//DISPLAY_T cDisp = DISPLAY_FRONT;
		//if(GetActiveMode() == APP_MEDIA_MODE_AUDIO && GetRearMode() != APP_MEDIA_MODE_AUDIO)
		if(isAudio)
		{
			AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_Engines[APP_MEDIA_MODE_AUDIO]);
			if(pAudioEngine)// added by edo.lee 2013.07.15 STATIC_TEST
				requestData.avState = pAudioEngine->getLastAVMode();	
			//cDisp = DISPLAY_FRONT;
		}
		else //if(GetActiveMode() == APP_MEDIA_MODE_VIDEO && GetRearMode() != APP_MEDIA_MODE_VIDEO)
		{
			AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO]);
			if(pVideoEngine)// added by edo.lee 2013.07.15 STATIC_TEST
				requestData.avState = pVideoEngine->getLastAVMode();	
			//cDisp=DISPLAY_REAR;
		}		
		
		//added by edo.lee 2013.06.08
		MP_LOG<<"EDO last mode :" << requestData.avState << LOG_ENDL;
		if(isAudio)
		{
			if(requestData.avState != MODE_STATE_DISC_AUDIO
			&&requestData.avState != MODE_STATE_USB1_AUDIO
			&&requestData.avState != MODE_STATE_USB2_AUDIO
			&&requestData.avState != MODE_STATE_IPOD1_AUDIO
			&&requestData.avState != MODE_STATE_IPOD2_AUDIO
			&&requestData.avState != MODE_STATE_JUKEBOX_AUDIO
			&&requestData.avState != MODE_STATE_AUX1_AUDIO
			&&requestData.avState != MODE_STATE_AUX2_AUDIO
			&&requestData.avState != MODE_STATE_BT_AUDIO			//added by hyochang.ryu 20130710
			)
			return;
		}else
		{
			if(requestData.avState != MODE_STATE_USB1_VIDEO
			&&requestData.avState != MODE_STATE_USB2_VIDEO
			&&requestData.avState != MODE_STATE_DISC_VIDEO
			&&requestData.avState != MODE_STATE_JUKEBOX_VIDEO
			&&requestData.avState != MODE_STATE_AUX1_VIDEO
			&&requestData.avState != MODE_STATE_AUX2_VIDEO)
			return;
		}
			
		Event tEvent(EVT_MUTE_CANCEL/*EVT_REQUEST_AUDIO_PATH*/, APP_AVP, APP_UI_MANAGER, QByteArray((char*)&requestData, sizeof(requestData)));//modified by edo.lee 2013.10.03
		//notifyUISH(tEvent,cDisp );
		notifyUISH(tEvent );
		m_isMuteState = false;
        MP_LOG<<"send UnMute State Signal isAudio :"<<isAudio << LOG_ENDL;//<<" "<<cDisp << LOG_ENDL;
	}
	
	setKeepSystemMute(false);//added by edo.lee 2013.09.05
}
//added by edo.lee 2013.04.01
//added by edo.lee 2013.04.21 for Mute

//{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
void
AppMediaPlayer_EngineMain::setMuteForBTStream(MODE_STATE_T state)
{
    if(m_isTAOn == false)
    {
    	if(m_isMuteForSeek==false && !m_isNoneState)
    	{
    		EVENT_AUDIO_PATH_REQUEST_DATA requestData;
    		
            requestData.avState = state;				
    
    		MP_LOG<<"EDO last mode :" << requestData.avState << LOG_ENDL;
            if(requestData.avState != MODE_STATE_DISC_AUDIO
    			&&requestData.avState != MODE_STATE_USB1_AUDIO
    			&&requestData.avState != MODE_STATE_USB2_AUDIO
    			&&requestData.avState != MODE_STATE_IPOD1_AUDIO
    			&&requestData.avState != MODE_STATE_IPOD2_AUDIO
    			&&requestData.avState != MODE_STATE_JUKEBOX_AUDIO
    			&&requestData.avState != MODE_STATE_AUX1_AUDIO
    			&&requestData.avState != MODE_STATE_AUX2_AUDIO
    			&&requestData.avState != MODE_STATE_BT_AUDIO
                //{ modified by yongkyun.lee 2014-03-03 for : 
                &&requestData.avState != MODE_STATE_BT_PANDORA  
                &&requestData.avState != MODE_STATE_AHA_RADIO_BT
                //} modified by yongkyun.lee 2014-03-03 
    			)
    			return;
            if(state == MODE_STATE_BT_PANDORA) 
            {
                Event pEvent(EVT_AV_TEMPORAL_MUTE , APP_PANDORA, APP_UI_MANAGER, QByteArray((char*)&requestData, sizeof(requestData)));
    		    NotifyUISHCommon(pEvent );
                MP_HIGH<<"setMuteForSeek Signal APP_PANDORA: " << LOG_ENDL;//<<" "<<cDisp << LOG_ENDL;
            }
            else if(state == MODE_STATE_AHA_RADIO_BT) 
            {
                Event aEvent(EVT_AV_TEMPORAL_MUTE , APP_AHA_RADIO, APP_UI_MANAGER, QByteArray((char*)&requestData, sizeof(requestData)));
    		    NotifyUISHCommon(aEvent );
                MP_HIGH<<"setMuteForSeek Signal APP_AHA_RADIO: " << LOG_ENDL;//<<" "<<cDisp << LOG_ENDL;
            }
            else
            {
                MP_HIGH<<"setMuteForSeek NO SEND... : ";//<<" "<<cDisp << LOG_ENDL;
                return;
            }

    		m_isMuteForSeek = true;
    	}
    }
}
//} modified by yongkyun.lee 2014-03-13

void
AppMediaPlayer_EngineMain::setMuteForSeek(bool isAudio)
{
    //added by sh.kim for ITS 179104
    if(m_isTAOn == false)
    {
	if(m_isMuteForSeek==false && !m_isNoneState)
	{
		EVENT_AUDIO_PATH_REQUEST_DATA requestData;
		
		//DISPLAY_T cDisp = DISPLAY_FRONT;
		//if(GetActiveMode() == APP_MEDIA_MODE_AUDIO && GetRearMode() != APP_MEDIA_MODE_AUDIO)
		if(isAudio)
		{
			AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_Engines[APP_MEDIA_MODE_AUDIO]);
			if(pAudioEngine)// added by edo.lee 2013.07.15 STATIC_TEST
				requestData.avState = pAudioEngine->getLastAVMode();				
			//cDisp = DISPLAY_FRONT;
		}
		else //if(GetActiveMode() == APP_MEDIA_MODE_VIDEO && GetRearMode() != APP_MEDIA_MODE_VIDEO)
		{
			AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO]);
			if(pVideoEngine)// added by edo.lee 2013.07.15 STATIC_TEST
				requestData.avState = pVideoEngine->getLastAVMode();	
			//cDisp=DISPLAY_REAR;
		}
		//setIsTermOnTrackChange(true);
		//added by edo.lee 2013.06.08
		MP_LOG<<"EDO last mode :" << requestData.avState << LOG_ENDL;
		if(isAudio)
		{
			if(requestData.avState != MODE_STATE_DISC_AUDIO
			&&requestData.avState != MODE_STATE_USB1_AUDIO
			&&requestData.avState != MODE_STATE_USB2_AUDIO
			&&requestData.avState != MODE_STATE_IPOD1_AUDIO
			&&requestData.avState != MODE_STATE_IPOD2_AUDIO
			&&requestData.avState != MODE_STATE_JUKEBOX_AUDIO
			&&requestData.avState != MODE_STATE_AUX1_AUDIO
			&&requestData.avState != MODE_STATE_AUX2_AUDIO
			&&requestData.avState != MODE_STATE_BT_AUDIO		//added by hyochang.ryu 20130710
			)
			return;
		}else
		{
			if(requestData.avState != MODE_STATE_USB1_VIDEO
			&&requestData.avState != MODE_STATE_USB2_VIDEO
			&&requestData.avState != MODE_STATE_DISC_VIDEO
			&&requestData.avState != MODE_STATE_JUKEBOX_VIDEO
			&&requestData.avState != MODE_STATE_AUX1_VIDEO
			&&requestData.avState != MODE_STATE_AUX2_VIDEO)
			return;
		}
        Event tEvent(EVT_AV_TEMPORAL_MUTE , APP_AVP, APP_UI_MANAGER, QByteArray((char*)&requestData, sizeof(requestData)));
		//notifyUISH(tEvent,cDisp );
		NotifyUISHCommon(tEvent );
		m_isMuteForSeek = true;
                MP_LOG<<"setMuteForSeek Signal isAudio: "<<isAudio << LOG_ENDL;//<<" "<<cDisp << LOG_ENDL;
	}
    }
}
void
AppMediaPlayer_EngineMain::setUnMuteForSeek(bool isAudio)
{
	if(m_isMuteForSeek && !m_isNoneState)
	{
		EVENT_AUDIO_PATH_REQUEST_DATA requestData;
		
		//DISPLAY_T cDisp = DISPLAY_FRONT;
		//if(GetActiveMode() == APP_MEDIA_MODE_AUDIO && GetRearMode() != APP_MEDIA_MODE_AUDIO)
		if(isAudio)
		{
			AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_Engines[APP_MEDIA_MODE_AUDIO]);
			if(pAudioEngine)// added by edo.lee 2013.07.15 STATIC_TEST
				requestData.avState = pAudioEngine->getLastAVMode();	
			//cDisp = DISPLAY_FRONT;
		}
		else //if(GetActiveMode() == APP_MEDIA_MODE_VIDEO && GetRearMode() != APP_MEDIA_MODE_VIDEO)
		{
			AppMediaPlayer_EngineVideo* pVideoEngine = dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO]);
			if(pVideoEngine)// added by edo.lee 2013.07.15 STATIC_TEST
				requestData.avState = pVideoEngine->getLastAVMode();	
			//cDisp=DISPLAY_REAR;
		}		

		//setIsTermOnTrackChange(false); //remove by edo.lee 2013.09.26 ITS 0183932
		MP_LOG<<"EDO last mode :" << requestData.avState << LOG_ENDL;
		if(isAudio)
		{
			if(requestData.avState != MODE_STATE_DISC_AUDIO
			&&requestData.avState != MODE_STATE_USB1_AUDIO
			&&requestData.avState != MODE_STATE_USB2_AUDIO
			&&requestData.avState != MODE_STATE_IPOD1_AUDIO
			&&requestData.avState != MODE_STATE_IPOD2_AUDIO
			&&requestData.avState != MODE_STATE_JUKEBOX_AUDIO
			&&requestData.avState != MODE_STATE_AUX1_AUDIO
			&&requestData.avState != MODE_STATE_AUX2_AUDIO
			&&requestData.avState != MODE_STATE_BT_AUDIO		//added by hyochang.ryu 20130710
			)
			return;
		}else
		{
			if(requestData.avState != MODE_STATE_USB1_VIDEO
			&&requestData.avState != MODE_STATE_USB2_VIDEO
			&&requestData.avState != MODE_STATE_DISC_VIDEO
			&&requestData.avState != MODE_STATE_JUKEBOX_VIDEO
			&&requestData.avState != MODE_STATE_AUX1_VIDEO
			&&requestData.avState != MODE_STATE_AUX2_VIDEO)
			return;
		}
         Event tEvent(EVT_AV_TEMPORAL_UNMUTE, APP_AVP, APP_UI_MANAGER, QByteArray((char*)&requestData, sizeof(requestData)));
		//notifyUISH(tEvent,cDisp );
		NotifyUISHCommon(tEvent );
		m_isMuteForSeek = false;
        MP_LOG<<"setUnMuteForSeek Signal isAudio"<<isAudio << LOG_ENDL;//<<" "<<cDisp << LOG_ENDL;
	}
}
//added by edo.lee 2013.04.21 for Mute
//added by edo.lee 2013.07.03
void
AppMediaPlayer_EngineMain::syncIndexFRPhotoEngine(bool fromFront, QString filepath, bool fromList, PHOTO_SOURCES_LIST source)
{
	MP_LOG<<filepath << LOG_ENDL;
	// modified by ravikanth 27-07-13 for ITS 181678
        //if(m_FRDRSViewLoading == false) return; //working when both screens are photo
	AppMediaPlayer_EnginePhoto *pPhotoengine = NULL;
	if(fromFront == false)
	{
		pPhotoengine =  dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_FRONT]);
	}
	else
	{
		pPhotoengine =  dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_REAR]);
	}
	if(pPhotoengine)
	{	
//                if(fromList)
//                {
//                        if(filepath != "")
//                                pPhotoengine->syncImageFRonDRS(filepath);
//                }
//                else
//			pPhotoengine->PlayTrackFromFileManager(filepath);
            pPhotoengine->syncImageFRonDRS(filepath, source);
	}
}
QString 
AppMediaPlayer_EngineMain::getCurrnetFilePath(bool fromFront)
{
	MP_LOG << LOG_ENDL;
	QString rPath ="";
	if(m_FRDRSViewLoading == false) return rPath; //working when both screens are photo
	AppMediaPlayer_EnginePhoto *pPhotoengine = NULL;
	if(fromFront == false)
	{
		pPhotoengine =  dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_FRONT]);		
	}
	else
	{
		pPhotoengine =  dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_REAR]);
	}
	if(pPhotoengine)
		rPath = pPhotoengine->GetCurrentFilePath();
	return rPath;	
}
bool 
AppMediaPlayer_EngineMain::getIsSameDRSPhoto(bool fromFront , int sourceid)
{
// {modified by edo.lee 2013.08.17 hotfix empty list on photo
    MP_LOG<<fromFront<<sourceid << LOG_ENDL;
    if(m_FRDRSViewLoading == false)
    {
        return false; //working when both screens are photo
    }
    AppMediaPlayer_EnginePhoto *pPhotoengine = NULL;
    if(fromFront == false)
    {
        pPhotoengine =  dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_FRONT]);
    }
    else
    {
        pPhotoengine =  dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_REAR]);
    }
    if(pPhotoengine)
    {
        MP_LOG<<pPhotoengine->getCurrentSourceID()<<" "<<sourceid<< " "<<pPhotoengine->isDRSShow() << LOG_ENDL;
        if(pPhotoengine->getCurrentSourceID() == sourceid)
            //&& pPhotoengine->isDRSShow())
        {
            return true;
        }
    }
    return false;
// } modified by edo.lee 2013.08.17 hotfix empty list on photo
}

//added by edo.lee 2013.07.03
void
AppMediaPlayer_EngineMain::SetActiveMode( APP_ENTRY_POINT_T pEventData, DISPLAY_T display, bool bShowLoading )
{
    bool             bShowVideoLoading = false;
    APP_MEDIA_MODE_T mode = APP_MEDIA_MODE_MAX;

    if ( pEventData )
    {
        MP_LOG << "sAppEntryPoint =" <<pEventData << "display =" << display << "bShowLoading =" << bShowLoading << LOG_ENDL;

        switch ( pEventData )
        {
        //{added by junam 2013.12.19 for LIST_ENTRY_POINT
        case eAPP_FILEMANAGER_JUKEBOX_AUDIO:
        case eAPP_FILEMANAGER_USB1_AUDIO:
        case eAPP_FILEMANAGER_USB2_AUDIO:
        case eAPP_FILEMANAGER_IPOD1:
        case eAPP_FILEMANAGER_IPOD2:
        //}added by junam
        case eAPP_FILEMANAGER_DISC_AUDIO: // added by oseong.kwon 2014.02.10 for ITS 223602
        case eAPP_MODE_DYNAMICS_DISC: // added by wonseok.heo for Disc IN OUT TEST 2013.07.20
        case eAPP_AVP_AUDIO:
        case eAPP_AVP_AUDIO_MODE_JUKEBOX:
        case eAPP_AVP_AUDIO_MODE_USB1:
        case eAPP_AVP_AUDIO_MODE_USB2:
        case eAPP_AVP_AUDIO_MODE_IPOD1:
        case eAPP_AVP_AUDIO_MODE_IPOD2:
        case eAPP_AVP_AUDIO_MODE_DISC:
        {
            if(pEventData == eAPP_AVP_AUDIO_MODE_IPOD1 && m_pMediaManager->GetMountPath(MEDIA_SOURCE_IPOD1).isEmpty()) {
                MP_LOG << "------> ERROR NEWIPOD IPOD1 already disconnected" << LOG_ENDL;
                //return;
            }
            else if(pEventData == eAPP_AVP_AUDIO_MODE_IPOD2 && m_pMediaManager->GetMountPath(MEDIA_SOURCE_IPOD2).isEmpty()) {
                MP_LOG << "------> ERROR NEWIPOD IPOD2 already disconnected" << LOG_ENDL;
                //return;
            }
        }
        case eAPP_AVP_MODE_BLUETOOTH:
        case eAPP_AVP_AUDIO_MODE_AUX:
        case eAPP_AVP_AUDIO_MODE_BT:
        {
            mode = APP_MEDIA_MODE_AUDIO;
            break;
        }
        case eAPP_AVP_VIDEO:
        case eAPP_AVP_VIDEO_MODE_JUKEBOX:
        case eAPP_AVP_VIDEO_MODE_USB1:
        case eAPP_AVP_VIDEO_MODE_USB2:
        case eAPP_AVP_VIDEO_MODE_DISC:
        {
            bShowVideoLoading = true;
            // { added by cychoi 2013-07-15 for Prevent fix
            mode = APP_MEDIA_MODE_VIDEO;
            break;
            // } added by cychoi 2013-07-15
        }
        case eAPP_AVP_VIDEO_MODE_AUX:
        case eAPP_AVP_VIDEO_MODE_DISC_DRS:
        case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
        case eAPP_AVP_VIDEO_MODE_USB1_DRS:
        case eAPP_AVP_VIDEO_MODE_USB2_DRS:
        case eAPP_AVP_VIDEO_MODE_AUX_DRS:
        {
            mode = APP_MEDIA_MODE_VIDEO;
            break;
        }
        case eAPP_AVP_PHOTO:
        case eAPP_AVP_PHOTO_MODE_JUKEBOX:
        case eAPP_AVP_PHOTO_MODE_USB1:
        case eAPP_AVP_PHOTO_MODE_USB2:
        case eAPP_AVP_PHOTO_MODE_JUKEBOX_DRS:
        case eAPP_AVP_PHOTO_MODE_USB1_DRS:
        case eAPP_APP_PHOTO_MODE_USB2_DRS:
        {
            mode = display == DISPLAY_FRONT ? APP_MEDIA_MODE_PHOTO_FRONT : APP_MEDIA_MODE_PHOTO_REAR;
            break;
        }
        case eAPP_MODE_LAST_STATE:
        default:
        {
            // Should use current active mode which is already set.
            mode = (display == DISPLAY_FRONT) ? m_AppFrontMode : m_AppRearMode;
            break;
        }
        }

        MP_LOG << "mode =" << mode << LOG_ENDL;

        if(mode == APP_MEDIA_MODE_MAX)
        {
            MP_LOG << "Unsupported mode" << LOG_ENDL;
            return;
        }

        //{changed by junam 2013.09.16 ITS_KOR_189473
//        QDeclarativeView *pView = GetDeclarativeView( (display == DISPLAY_FRONT) ?
//                                                         AppEngineQMLConstants::APP_FRONT_SCREEN : AppEngineQMLConstants::APP_REAR_SCREEN);
//        if(pView && pView->viewport() != AppGLWidget::instance())
//        {
//            AppGLWidget::setCurrentWidget(pView->viewport());
//            emit swapScreen();
//        }
        //if(display == DISPLAY_FRONT)
        //{
        //    if(AppGLWidget::instance() != AppGLWidget::instanceFront() && mode == APP_MEDIA_MODE_AUDIO)
        //    {
        //        AppGLWidget::selectInstance(true);
        //        emit swapScreen();
        //    }
        //}
        //else if(display == DISPLAY_REAR)
        //{
        //    if(AppGLWidget::instance() != AppGLWidget::instanceRear() && mode == APP_MEDIA_MODE_AUDIO)
        //    {
        //        AppGLWidget::selectInstance(false);
        //        emit swapScreen();
        //    }
        //}
        //}changed by junam
        
        m_pBootstrap->loadComponents(mode, display); // modified by edo.lee 2013.02.04

        if(mode >= 0 && mode < APP_MEDIA_MODE_MAX) {
            m_Engines[mode]->SetEntryPoint(pEventData);
        }
    }
    else
    {
        MP_LOG << "no pEventData" << LOG_ENDL;
        return;
    }

    /* Connect UIListener stuff
     * 1 step: We are trying to disconnect old engine from received 'display', if it has connection.
     * 2 step: If "new" engine is already connected to  other 'display' - we try to disconnect it
     *         from that 'display'. This could happen when the one application run cross ways.
     *         For example: Launch app on front display -> close the app -> run it app on rear display.
     * 3 step: Engine is connecting to the actual 'display'.
     */

    ConnectSignalToUISH(display, mode); //added by edo.lee 2013.03.19

    m_pNotifier->SetActiveMode( mode );
    UpdateLayout( mode, display );

    //modified by edo.lee 2013.03.07
    if ( display == DISPLAY_FRONT )
    {
        m_AppFrontMode = mode;
        if(mode == APP_MEDIA_MODE_AUDIO)
            m_AppRearMode = APP_MEDIA_MODE_MAX;
    }
    else
    {
        m_AppRearMode = mode;
        if(mode == APP_MEDIA_MODE_AUDIO)
            m_AppFrontMode = APP_MEDIA_MODE_MAX;
    }

    m_currentScreen = display;

    // Set last player mode & clearOSDInfo
    if ( mode == APP_MEDIA_MODE_AUDIO || mode == APP_MEDIA_MODE_VIDEO )
    {
        m_LastPlayerMode = mode;
    }
}

//added by edo.lee 2013.03.19 connect signal before FG
void AppMediaPlayer_EngineMain::ConnectSignalToUISH(DISPLAY_T display, APP_MEDIA_MODE_T mode)
{
    if(mode == APP_MEDIA_MODE_PHOTO_FRONT ||mode == APP_MEDIA_MODE_PHOTO_REAR) return; //added by edo.lee 2013.04.09

    if ( display == DISPLAY_FRONT )
    {
        MP_LOG << "1 front mode =" << m_AppFrontMode << LOG_ENDL;
        MP_LOG << "1 rear mode =" << m_AppRearMode << LOG_ENDL;

        // 1 step
        if ( m_AppFrontMode != APP_MEDIA_MODE_MAX && m_AppFrontMode != APP_MEDIA_MODE_PHOTO_FRONT)// modified by edo.lee 2013.04.09
        {
            disconnect( m_Engines[m_AppFrontMode], NULL, this, SLOT(doNotifyUISHFront(Event)));
            disconnect( m_Engines[m_AppFrontMode], NULL, this, SLOT(doNotifyUISH(Event,DISPLAY_T)));
        }

        // 2 step
        if ( mode == m_AppRearMode && mode != APP_MEDIA_MODE_PHOTO_FRONT)// modified by edo.lee 2013.04.09
        {
            disconnect( m_Engines[m_AppRearMode], NULL, this, SLOT(doNotifyUISHRear(Event)));
            disconnect( m_Engines[m_AppRearMode], NULL, this, SLOT(doNotifyUISH(Event,DISPLAY_T)));
        }

        // 3 step
        if( mode != APP_MEDIA_MODE_PHOTO_FRONT) // modified by edo.lee 2013.04.09
        {
            // { modified by sangmin.seol 2014.02.07 ITS_0223824 prevent duplicate connection
            connect( m_Engines[mode], SIGNAL(NotifyUISH(Event)), this, SLOT(doNotifyUISHFront(Event)), Qt::UniqueConnection);
            connect( m_Engines[mode], SIGNAL(NotifyUISH(Event,DISPLAY_T)), this, SLOT(doNotifyUISH(Event,DISPLAY_T)), Qt::UniqueConnection);
            // } modified by sangmin.seol 2014.02.07 ITS_0223824 prevent duplicate connection
        }
    }
    else if ( display == DISPLAY_REAR )
    {
        MP_LOG << "2 front mode =" << m_AppFrontMode << LOG_ENDL;
        MP_LOG << "2 rear mode =" << m_AppRearMode << LOG_ENDL;

        // 1 step
        if ( m_AppRearMode != APP_MEDIA_MODE_MAX && m_AppRearMode != APP_MEDIA_MODE_PHOTO_REAR)// modified by edo.lee 2013.04.09
        {
            disconnect( m_Engines[m_AppRearMode], NULL, this, SLOT(doNotifyUISHRear(Event)));
            disconnect( m_Engines[m_AppRearMode], NULL, this, SLOT(doNotifyUISH(Event,DISPLAY_T)));
        }

        // 2 step
        if ( mode == m_AppFrontMode && mode != APP_MEDIA_MODE_PHOTO_REAR )// modified by edo.lee 2013.04.09
        {
            disconnect( m_Engines[m_AppFrontMode], NULL, this, SLOT(doNotifyUISHFront(Event)));
            disconnect( m_Engines[m_AppFrontMode], NULL, this, SLOT(doNotifyUISH(Event,DISPLAY_T)));
        }

        // 3 step
        if(mode != APP_MEDIA_MODE_PHOTO_REAR)   // modified by edo.lee 2013.04.09
        {
            // { modified by sangmin.seol 2014.02.07 ITS_0223824 prevent duplicate connection
            connect( m_Engines[mode], SIGNAL(NotifyUISH(Event)), this, SLOT(doNotifyUISHRear(Event)), Qt::UniqueConnection);
            connect( m_Engines[mode], SIGNAL(NotifyUISH(Event,DISPLAY_T)), this, SLOT(doNotifyUISH(Event,DISPLAY_T)), Qt::UniqueConnection);
            // } modified by sangmin.seol 2014.02.07 ITS_0223824 prevent duplicate connection
        }
    }

    connect( m_Engines[mode], SIGNAL(statusBarPostEventSend(int,int)), this, SLOT(StatusBarPostEventSend(int,int)));
}
//added by edo.lee 2013.03.19

void AppMediaPlayer_EngineMain::handleUSB1OutTimer()
{
    /*
    m_tUSB1OutTimer.stop();
    
    if(m_pMediaManager->GetMountPath(MEDIA_SOURCE_USB1).isEmpty() == false) {
        MP_LOG << "[EVT_USB_REMOVED] handleUSB1OutTimer. START TA" << LOG_ENDL;
        CTrackerAbstractor *pTrackerAbs = GetTrackerAbs();
        if(pTrackerAbs) {
            bool bResult = pTrackerAbs->StartScanFileSystem(eUSB_FRONT);
            if(bResult == true) {
                // removed by eugene.seo 2013.03.16
                // ((AppMediaPlayer_Controller_Audio*)m_Engines[APP_MEDIA_MODE_AUDIO]->GetController())->pauseMiner();
            }
        }
    }
    else {
        MP_LOG << "[EVT_USB_REMOVED] handleUSB1OutTimer" << LOG_ENDL;
    }
    */
}

void AppMediaPlayer_EngineMain::handleUSB2OutTimer()
{
    /*
    m_tUSB2OutTimer.stop();
    
    if(m_pMediaManager->GetMountPath(MEDIA_SOURCE_USB2).isEmpty() == false) {
        MP_LOG << "[EVT_USB_REMOVED] handleUSB2OutTimer. START TA" << LOG_ENDL;
        CTrackerAbstractor *pTrackerAbs = GetTrackerAbs();
        if(pTrackerAbs) {
            bool bResult = pTrackerAbs->StartScanFileSystem(eUSB_REAR);
            if(bResult == true) {
                // removed by eugene.seo 2013.03.16
                // ((AppMediaPlayer_Controller_Audio*)m_Engines[APP_MEDIA_MODE_AUDIO]->GetController())->pauseMiner();
            }
        }
    }
    else {
        MP_LOG << "[EVT_USB_REMOVED] handleUSB2OutTimer" << LOG_ENDL;
    }
    */
}

APP_MEDIA_MODE_T AppMediaPlayer_EngineMain::GetViewR(DISPLAY_T display) 
{
	MP_LOG << LOG_ENDL;
	APP_MEDIA_MODE_T ret = APP_MEDIA_MODE_MAX;
	if(m_eCloneStatus == CLONE_ON_FRONT)
	{
		ret = m_AppFrontView;
	}
	else if(m_eCloneStatus == CLONE_ON_REAR)
	{
		ret = m_AppRearView;
	}
	else if(m_eCloneStatus == CLONE_OFF)
	{
		if(display == DISPLAY_FRONT)
			 ret = m_AppFrontView;
		else if(display == DISPLAY_REAR)
			ret = m_AppRearView;
	}	
	return ret;
}

//{ added by edo.lee 2013.02.04
VIDEO_DISPLAY_T AppMediaPlayer_EngineMain::getVideoDisplay()
{
	return m_currentVideoMode;
}

bool AppMediaPlayer_EngineMain::IsVideoModeEnterd(DISPLAY_T display)
{		
	if(display == DISPLAY_REAR)
	{
		//MP_LOG<<"rear" << LOG_ENDL;
		return m_isVideoRearEntered;
	}
	//MP_LOG<<"front" << LOG_ENDL;
	return m_isVideoFrontEntered;
}
//} added by edo.lee

// { modified by kihyung 2013.08.09 for ITS 0183077 
void AppMediaPlayer_EngineMain::SaveSettings()
{
    QFile tFlip(AVP_SETTING_DEBUG_FILE_FLIP);
    if(tFlip.open(QFile::ReadWrite) == true) {
        MP_HIGH << "FLIP OK" << LOG_ENDL;
        tFlip.flush();
        tFlip.close();
    }
    
    for (int index = 0; index < APP_MEDIA_MODE_MAX; index++)
    {
        if (m_Engines[index])
        {
            m_Engines[index]->SaveSettings(true);
        }
    }
    
    QFile tFlop(AVP_SETTING_DEBUG_FILE_FLOP);
    if(tFlop.open(QFile::ReadWrite) == true) {
        MP_HIGH << "FLOP OK" << LOG_ENDL;
        tFlop.flush();
        tFlop.close();
    }
}
// } modified by kihyung 2013.08.09 for ITS 0183077 

void
AppMediaPlayer_EngineMain::LoadAppState()
{

}

void
AppMediaPlayer_EngineMain::SaveAppState()
{

}

// DEAD CODE:
void
AppMediaPlayer_EngineMain::HandleSoftKeyHome()
{
    Event event( EVT_KEY_SOFT_HOME, GetThisAppId(), APP_NONE );
    NotifyUISH( event );
}

// { modified by Sergey for CR#16565
void AppMediaPlayer_EngineMain::deckErrorHandler(int errorState)
{
    // { modified by cychoi 2013.07.30 for Display disc error OSD by new OSD manager
    // { added by wonseok.heo 2013.07.04 disc in out test
    if(m_DiscInOutTestMode == true){
        emit deckTestError(errorState);
    }
    // } added by wonseok.heo 2013.07.04 disc in out test

    MP_HIGH << "ErrorState = " << errorState << LOG_ENDL;

    switch(errorState)
    {
        case PLAYER_DISC_READ_ERROR:
        case PLAYER_DISCCORRUPTED:
        case PLAYER_NOMEDIA:
        case PLAYER_DISC_REGION_ERROR:
        {
            OSDManager::instance()->displayDiscErrorOSD(errorState);
            m_pNotifier->updateErrorOSD(errorState);
            m_nDeckLastError = errorState; // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
            break;
        }
        case PLAYER_HIGHTEMPERATURE:
        {
            // { commented by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
            // { added by kihyung 2012.12.28
            //EVENT_DISC_STATUS_T tDiscStatus;
            //tDiscStatus.sDiscStatus = eDISC_STATE_DECK_ERROR;
            //tDiscStatus.sDiscType   = eDISC_TYPE_INVALID;
            //Event tEvent(EVT_DISC_STATUS, APP_AVP, APP_UI_MANAGER, QByteArray((char*)&tDiscStatus, sizeof(tDiscStatus))); // modified by lssanh 2013.02.21 IPC payload
            //notifyUISH(tEvent);
            // } added by kihyung 2012.12.28
            // } commented by cychoi 2014.04.09
            // { modified by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
            APP_ENTRY_POINT_T nEntryPoint = GetLastAVEntryPoint(false);
            DISPLAY_T display = DISPLAY_FRONT_REAR;
            if(nEntryPoint == eAPP_AVP_VIDEO_MODE_DISC ||
               nEntryPoint == eAPP_AVP_VIDEO_MODE_DISC_DRS)
            {
                if(GetFrontView()==APP_MEDIA_MODE_VIDEO && GetRearView()==APP_MEDIA_MODE_VIDEO)
                {
                    display = DISPLAY_NONE;
                }
                else if(GetFrontView()==APP_MEDIA_MODE_VIDEO)
                {
                    display = DISPLAY_REAR;
                }
                else if(GetRearView()==APP_MEDIA_MODE_VIDEO)
                {
                    display = DISPLAY_FRONT;
                }
                AppMediaPlayer_EngineVideo*pVEngine = (AppMediaPlayer_EngineVideo*)GetEngine(APP_MEDIA_MODE_VIDEO);
                pVEngine->showDeckHighTempPopup();
                m_tDeckErrorTimer.start();
            }
            else if(nEntryPoint == eAPP_AVP_AUDIO_MODE_DISC)
            {
                if(GetFrontView()==APP_MEDIA_MODE_AUDIO && GetRearView()==APP_MEDIA_MODE_AUDIO)
                {
                    display = DISPLAY_NONE;
                }
                else if(GetFrontView()==APP_MEDIA_MODE_AUDIO)
                {
                    display = DISPLAY_REAR;
                }
                else if(GetRearView()==APP_MEDIA_MODE_AUDIO)
                {
                    display = DISPLAY_FRONT;
                }
                AppMediaPlayer_EngineAudio*pAEngine = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
                pAEngine->showDeckHighTempPopup();
                m_tDeckErrorTimer.start();
            }
            OSDManager::instance()->displayDiscErrorOSD(errorState, display);
            m_pNotifier->updateErrorOSD(errorState);
            m_nDeckLastError = errorState;
            break;
            // } modified by cychoi 2014.04.09
        }
        // { added by cychoi 2014.05.22 for Deck is not connected
        case PLAYER_FILENOTFOUND:
        {
            //OSDManager::instance()->displayDiscErrorOSD(errorState);
            //m_pNotifier->updateErrorOSD(errorState);
            m_nDeckLastError = errorState; // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling

            EVENT_DISC_STATUS_T tDiscStatus;
            tDiscStatus.sDiscStatus = eDISC_STATE_DECK_ERROR;
            tDiscStatus.sDiscType = eDISC_TYPE_INVALID;
            Event tEvent(EVT_DISC_STATUS, APP_AVP, APP_UI_MANAGER, QByteArray((char*)&tDiscStatus, sizeof(tDiscStatus)));
            notifyUISH(tEvent);
        }
        // } added by cychoi 2014.05.22
        // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
        case PLAYER_NO_ERROR:
        case PLAYER_DISC_MECHANIC_ERROR:
        case PLAYER_DISC_LOW_CONSUMPTION:
            m_nDeckLastError = errorState;
            break;
        // } added by cychoi 2014.04.09
        default: //make compiler happy
            break;
    }
    // } modified by cychoi 2013.07.30
}
// } modified by Sergey for CR#16565

// { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
void AppMediaPlayer_EngineMain::sendDeckErrorToUISH_Impl(bool bPopupClose)
{
    int discState = m_pMediaManager->getDiscState();
    MP_HIGH << "DiscState = " << discState << LOG_ENDL;
    EVENT_DISC_STATUS_T tDiscStatus;
    tDiscStatus.sDiscStatus = eDISC_STATE_DECK_ERROR;
    switch(discState)
    {
        case PLAYER_DVDVIDEOINSERTED:
            tDiscStatus.sDiscType = eDISC_TYPE_VIDEO_DVD;
            break;
        case PLAYER_VCDINSERTED:
            tDiscStatus.sDiscType = eDISC_TYPE_VIDEO_CD;
            break;
        case PLAYER_CAINSERTED:
            tDiscStatus.sDiscType = eDISC_TYPE_MP3_CD;
            break;
        case PLAYER_CDDAINSERTED:
            tDiscStatus.sDiscType = eDISC_TYPE_AUDIO_CD;
            break;
        case PLAYER_DVDAUDIOINSERTED:
            tDiscStatus.sDiscType = eDISC_TYPE_AUDIO_DVD;
            break;
        case PLAYER_DISCLOADED:
        case PLAYER_DISCLOADING:
        case PLAYER_DISCRELOADING:
        case PLAYER_DISCRELOADED:
            tDiscStatus.sDiscType = eDISC_TYPE_INVALID;
            break;
        case PLAYER_DISCEJECTING:
        case PLAYER_DISCEJECTED:
        case PLAYER_DISCREMOVED:
        default:
            break;
    }
    Event tEvent(EVT_DISC_STATUS, APP_AVP, APP_UI_MANAGER, QByteArray((char*)&tDiscStatus, sizeof(tDiscStatus)));
    notifyUISH(tEvent);

    if(bPopupClose == true)
    {
        APP_ENTRY_POINT_T nEntryPoint = GetLastAVEntryPoint(false);
        if(nEntryPoint == eAPP_AVP_VIDEO_MODE_DISC ||
           nEntryPoint == eAPP_AVP_VIDEO_MODE_DISC_DRS)
        {
            AppMediaPlayer_EngineVideo*pVEngine = (AppMediaPlayer_EngineVideo*)GetEngine(APP_MEDIA_MODE_VIDEO);
            pVEngine->closeDeckHighTempPopup();
        }
        else if(nEntryPoint == eAPP_AVP_AUDIO_MODE_DISC)
        {
            AppMediaPlayer_EngineAudio*pAEngine = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
            pAEngine->closeDeckHighTempPopup();
        }
    }
}

void AppMediaPlayer_EngineMain::sendDeckErrorToUISH()
{
    if(m_tDeckErrorTimer.isActive())
    {
        m_tDeckErrorTimer.stop();
        // Popup was alrwady closed, only send Deck error to UISH
        sendDeckErrorToUISH_Impl(false);
    }
}

bool AppMediaPlayer_EngineMain::IsDeckBlocked()
{
    if(m_tDeckErrorTimer.isActive())
    {
        // should block deck operation although error is cleared  during High Temperature Notification
        return true;
    }

    return (m_nDeckLastError == PLAYER_HIGHTEMPERATURE);
}

void AppMediaPlayer_EngineMain::handleDeckErrorOnDeviceChanged()
{
    APP_ENTRY_POINT_T nEntryPoint = GetLastAVEntryPoint(false);
	MP_LOG << "Entry = " << nEntryPoint << LOG_ENDL;
    if(nEntryPoint == eAPP_AVP_VIDEO_MODE_DISC ||
       nEntryPoint == eAPP_AVP_VIDEO_MODE_DISC_DRS ||
       nEntryPoint == eAPP_AVP_AUDIO_MODE_DISC)
    {
        DeckController *pDeckController = GetControllers().GetDeckController();
        if(pDeckController &&
           pDeckController->GetMediaInstance()->GetDeviceState() == DiscError &&
           m_nDeckLastError == PLAYER_HIGHTEMPERATURE)
        {
            MP_HIGH << "ErrorState = " << m_nDeckLastError << LOG_ENDL;
            deckErrorHandler(m_nDeckLastError);
        }
    }
}
// } added by cychoi 2014.04.09

//{added by edo.lee 2012.01.09
void AppMediaPlayer_EngineMain::notifyEnableMedia( MEDIA_SOURCE_T deviceid, MODE_STATE_T modeState, bool isAudioEnable, bool isVideoEnable, bool isPhotoEnable)
{
    EVT_ID_T type;

    switch(deviceid)
    {
    case MEDIA_SOURCE_JUKEBOX: 
    {
        type = EVT_JUKEBOX_ENABLED;
        break;
    }
    case MEDIA_SOURCE_USB1 : 
    {
        type = EVT_USB1_ENABLED;
        break;
    }
    case MEDIA_SOURCE_USB2: 
    {
        type = EVT_USB2_ENABLED;
        break;
    }
    case MEDIA_SOURCE_IPOD1: 
    {
        AppMediaPlayer_EngineAudio      *pEngineAudio = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
        AppMediaPlayer_Controller_Audio *pController  = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();
        // { modified by cychoi 2013-07-15 for Prevent fix
        //suryanto.tan 2015.01.05 Add logs and use %s when using snprintf
        QString strDeviceName1 = pController->getIPODDeviceName(MEDIA_SOURCE_IPOD1);
        QByteArray strByteArray1 = strDeviceName1.toUtf8();
        MP_HIGH<<"strByteArray1.data(): " << strByteArray1.data() <<LOG_ENDL;
        snprintf(m_pEnableData[deviceid].cstrDeviceName, sizeof(m_pEnableData[deviceid].cstrDeviceName), "%s", strByteArray1.data());
        MP_HIGH<<"m_pEnableData[deviceid].cstrDeviceName: " << m_pEnableData[deviceid].cstrDeviceName <<LOG_ENDL;
        //sprintf(m_pEnableData[deviceid].cstrDeviceName, pController->getIPODDeviceName(deviceid));
        // } modified by cychoi 2013-07-15
        type = EVT_IPOD1_ENABLED;
        break;
    }
    case MEDIA_SOURCE_IPOD2: 
    {
        AppMediaPlayer_EngineAudio      *pEngineAudio = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
        AppMediaPlayer_Controller_Audio *pController  = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();
        // { modified by cychoi 2013-07-15 for Prevent fix
        //suryanto.tan 2015.01.05 Add logs and use %s when using snprintf
        QString strDeviceName2 = pController->getIPODDeviceName(MEDIA_SOURCE_IPOD2);
        QByteArray strByteArray2 = strDeviceName2.toUtf8();
        MP_HIGH<<"strByteArray1.data(): " << strByteArray2.data() <<LOG_ENDL;
        snprintf(m_pEnableData[deviceid].cstrDeviceName, sizeof(m_pEnableData[deviceid].cstrDeviceName),"%s", strByteArray2.data());
        MP_HIGH<<"m_pEnableData[deviceid].cstrDeviceName: " << m_pEnableData[deviceid].cstrDeviceName <<LOG_ENDL;
        //sprintf(m_pEnableData[deviceid].cstrDeviceName, pController->getIPODDeviceName(deviceid));
        // } modified by cychoi 2013-07-15
        type = EVT_IPOD2_ENABLED;
        break;
    }
    default:
        return;
    }    

	// { modified by eugene.seo 2013.04.16
	if(!isAudioEnable && !isVideoEnable && isPhotoEnable)
		m_pEnableData[deviceid].ucAvModeState = MODE_STATE_AVOFF;
	else		
	    m_pEnableData[deviceid].ucAvModeState = modeState;

	m_pEnableData[deviceid].bMediaEnable0 = isAudioEnable;
    m_pEnableData[deviceid].bMediaEnable1 = isVideoEnable;
    m_pEnableData[deviceid].bMediaEnable2 = isPhotoEnable;	
	
    MP_HIGH << "TASCAN -> " << deviceid << modeState << isAudioEnable << isVideoEnable << isPhotoEnable << LOG_ENDL; // modified by kihyung 2013.07.08
    // } modified by eugene.seo 2013.04.16
	
    Event event(type, GetThisAppId(), APP_UI_MANAGER, QByteArray( (char *)&m_pEnableData[deviceid], sizeof( EVT_DEVICE_ENABLE_DATA_T ))); // modified by lssanh 2013.02.21 IPC payload
    emit NotifyUISH(event,DISPLAY_COMMON); //modified by edo.lee 2013.01.23
}
//}added by edo.lee 



void
AppMediaPlayer_EngineMain::beep(EAudioBeepType beepType)
{
    // { modified by kihyung 2013.07.11 for P1 356
    /*
    MP_LOG << "Beep is played from media player" << LOG_ENDL;
    EVENT_BEEP_TYPE_DATA_T eventData;
    eventData.beepType = beepType;
    Event postEvent( EVT_BEEP_PLAY, GetThisAppId(), APP_UI_MANAGER,
                                QByteArray( (char*)&eventData, sizeof( eventData ) ) ); // modified by lssanh 2013.02.21 IPC payload
    NotifyUISH ( postEvent );
    */
    MP_LOG << LOG_ENDL;
    Q_UNUSED(beepType);
    ManualBeep();
    // } modified by kihyung 2013.07.11 for P1 356    
}

//{ modified by edo.lee 2013.02.04
void
AppMediaPlayer_EngineMain::componentLoaded(int imode, int display)
{
   MP_LOG << LOG_ENDL;
   APP_MEDIA_MODE_T mode = (APP_MEDIA_MODE_T)imode;
   QDeclarativeContext *context = m_pBootstrap->componentContext(mode,(DISPLAY_T)display);

   MP_LOG << (void *)context << LOG_ENDL;
	if( display == DISPLAY_REAR && mode == APP_MEDIA_MODE_VIDEO)
   		m_Engines[mode]->SetRearContext( context );
	else
		m_Engines[mode]->SetFrontContext( context );
	//	m_Engines[mode]->SetContext( context );
	context->setContextProperty("EngineListener", m_Engines[mode] );

	context->setContextProperty("isFrontView", display == DISPLAY_FRONT ? true : false ); //added by edo.lee 2013.03.04

   /* As View is not yet determined during component loading process -
    * use FRONT_SCREEN one as default value. The UIListenter will be set to appropriate value
    * once View changed */

	//if(mode == APP_MEDIA_MODE_PHOTO_REAR) //01.26
	if( display == DISPLAY_REAR ) 
	{
		context->setContextProperty("UIListener",
	                       GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN));
	}
	else
	{
		context->setContextProperty("UIListener",
	                       GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN));
	}

   m_pBootstrap->finishComponentCreation( mode ,(DISPLAY_T)display );

   if ( m_pBootstrap->componentItem(mode,(DISPLAY_T)display) != NULL )
   {
      m_pBootstrap->componentItem(mode,(DISPLAY_T)display)->setProperty("visible", false);
   }
   else
   {
      MP_LOG << "Failed to create item for component " << mode << LOG_ENDL;
   }
   if(mode == APP_MEDIA_MODE_AUDIO)//added by edo.lee 2013.03.23
	emit isCurrentFrontViewChanged(display == DISPLAY_FRONT);//added by edo.lee 2013.03.07
   //emit loadComponent(imode, display); //2013.09.26 delete by youngsim.jo added by junam 2013.09.24 for fast agree
#ifdef DUAL_KEY
   switch(display)
   {
       case DISPLAY_FRONT:
           frontLCDMode = mode;
           MP_LOG << mode << "is launched on a front LCD" << LOG_ENDL;
           break;
       case DISPLAY_REAR:
           rearLCDMode = mode;
           MP_LOG << mode << "is launched on a rear LCD" << LOG_ENDL;
           break;
   }
#endif
}
//} modified by edo.lee 
// { modified by bluepark 2012.08.09
// for booting time improvement
/*
void
AppMediaPlayer_EngineMain::loadComponents()
{
   MP_LOG << LOG_ENDL;
   m_pBootstrap->loadComponents();
}
*/
//{ modified by edo.lee 2013.02.04
void
AppMediaPlayer_EngineMain::loadComponents(APP_MEDIA_MODE_T mode, DISPLAY_T display)
{
   MP_LOG << LOG_ENDL;
   m_pBootstrap->loadComponents(mode,display);
}
//} modified by edo.lee 
// } modified by bluepark

// { modified by sjhyun 2013.10.18 for ITS_195165
// { added by yungi 2013.09.27 for ITS_NA 191163
void
AppMediaPlayer_EngineMain::SetModeAreaDim(bool bDimOn, int m_disp)
{
    // { modified by yungi 2013.10.10 for ITS 194869
    MP_LOG<<"Dim : "<<bDimOn<<" m_disp:"<<m_disp << LOG_ENDL;
    int lockoutView  = VIDEO_DISPLAY_MAX;
    bool sendOK      = true;                  // used for sending emit or not
    AppMediaPlayer_EngineBase * engineBase = GetEngine(APP_MEDIA_MODE_VIDEO);
    // (Physical)Front screen should not be set as dim on DRS.
    if (engineBase != NULL && bDimOn == true) // modified by sjhyun 2013.11.07 for ITS 207316
    {
        AppMediaPlayer_EngineVideo *pEngineVideo = (AppMediaPlayer_EngineVideo*)engineBase;
        lockoutView = pEngineVideo->getIsShowLockoutView();
        if (m_disp == lockoutView)
        {
            // condition: (DRS) && (m_disp == lockoutView)
            // We don't need to draw modeareaDim on DRS screen.
            sendOK = false;
        }
        // { added by sjhyun 2013.10.25 for ITS 198704
        // check TopScreen
        if ( !pEngineVideo->isTopScreenDVDTitleDiscMenu(m_disp) ) // modified by sjhyun 2013.11.07 for ITS 207316
            sendOK = false;
        // } added by sjhyun
    }
    //MP_LOG << "DRS("<< (lockoutView!= VIDEO_DISPLAY_MAX) <<") m_disp("<< m_disp<<") lockoutView("<<lockoutView<<")" << LOG_ENDL;

    if(sendOK)
    {
        // { modified by sjhyun 2013.11.01 for ITS 205242
        if(bDimOn)
        {
            if(m_disp ==0)
                emit setModeAreaOnlyBgFront(bDimOn);
            else if(m_disp ==1)
                emit setModeAreaOnlyBgRear(bDimOn);
        }
        else
        {
            emit setModeAreaOnlyBgFront(bDimOn);
            emit setModeAreaOnlyBgRear(bDimOn);
        }
        // } modified by sjhyun
    }
    // } modified by yungi 2013.10.10 for ITS 194869
}
// } added by yungi
// } modified by sjhyun

void
AppMediaPlayer_EngineMain::SetRoot( QObject *root )
{
   m_pBootstrap->reparent(root);
}

// { added by  yongkyun.lee 2012.11.21 for  CR 14584 When say "Music search" in VR App, "There is no connection for media" comment is coming
void
AppMediaPlayer_EngineMain::sendVRMining( Event aEvent)
{
    NotifyUISH( aEvent, DISPLAY_FRONT );
}
// { added by  yongkyun.lee 2012.11.21

// { added by kihyung 2013.1.8
void AppMediaPlayer_EngineMain::setCamInFG(bool bCamInFG)
{
    m_bCamInFG = bCamInFG;
	// removed by Sergey 10.05.2013
}

bool AppMediaPlayer_EngineMain::getCamInFG()
{
    return m_bCamInFG;
}
// } added by kihyung 2013.1.8

// { added by kihyung 2013.1.29
void AppMediaPlayer_EngineMain::setGstPipelineStatus(bool bStop)
{
    if(m_pNotifier->getGstPipelineDisplay() != DISPLAY_NONE) // added by kihyung 2014.01.14 for ITS 0218420
    {
        if(bStop == true) 
        {
            MP_HIGH << m_isBgReceived << getCamInFG() << m_pNotifier->getGstPipelineDisplay() << bStop << LOG_ENDL;
            
            // m_isBgReceived = false;
            // { modified by kihyung 2014.01.14 for ITS 0218420 
            // if(m_currentScreen == DISPLAY_FRONT) // modified by Sergey 16.04.2013
            if(m_pNotifier->getGstPipelineDisplay() == DISPLAY_FRONT)
            // } modified by kihyung 2014.01.14 for ITS 0218420 
                SetGstPipelinesStatus(GP_DISPLAY_FRONT, false);
            else
                SetGstPipelinesStatus(GP_DISPLAY_REAR, false);
        }
    }
}
// } added by kihyung 2013.1.29

//{ modified by yongkyun.lee 2013-07-16 for : ITS 175107
void
AppMediaPlayer_EngineMain::NotifyUISHMute( Event aEvent )
{
    MP_LOG  << LOG_ENDL;
    notifyUISH( aEvent, DISPLAY_FRONT_REAR );
}
//} modified by yongkyun.lee 2013-07-17 


// { modified by Sergey for SwapDC
void
AppMediaPlayer_EngineMain::doNotifyUISHFront( Event aEvent )
{
   notifyUISH( aEvent, DISPLAY_FRONT ); 
}

void
AppMediaPlayer_EngineMain::doNotifyUISHRear( Event aEvent )
{
    notifyUISH( aEvent, DISPLAY_REAR );
}

void AppMediaPlayer_EngineMain::doNotifyUISH( Event aEvent, DISPLAY_T aDisplay )
{
    MP_LOG << aDisplay << LOG_ENDL;
    notifyUISH( aEvent, aDisplay );
}

void AppMediaPlayer_EngineMain::TAScanCompleted(ETrackerAbstarctor_VolumeType volumeType, int bRescanMode, int bSuccess)
{
    MP_MEDIUM << volumeType << bRescanMode << bSuccess << LOG_ENDL;

    MEDIA_SOURCE_T nSource;
    int            nAcount;
    int            nVcount;        
    int            nPcount;   
        
    if(volumeType == eJukeBox) 
    {
        m_bJukeboxTAScanComplete  = true; 
        m_bRequestedJukeboxRescan = false;
        nSource = MEDIA_SOURCE_JUKEBOX;

        // added by kihyung 2013.11.16 to remove sync error after copy/delete.
        AppMediaPlayer_SettingsStorage::GetInstance()->SaveCommonSettings(this); //modified by sangmin.seol 2014.09.24 to fix "data load fail"
    }
    else if(volumeType == eUSB_FRONT) 
    {
        nSource = MEDIA_SOURCE_USB1;  
        if(m_pMediaManager->GetMountPath(nSource).isEmpty() == true) {
            MP_HIGH << "removed USB1. return..." << LOG_ENDL;
            return;
        }
        m_bUSB1TAScanComplete = true;      
    }
    else if(volumeType == eUSB_REAR) 
    {
        nSource = MEDIA_SOURCE_USB2;                
        if(m_pMediaManager->GetMountPath(nSource).isEmpty() == true) {
            MP_HIGH << "removed USB2. return..." << LOG_ENDL;
            return;
        }
        m_bUSB2TAScanComplete = true;   
    }
    else 
    {
        MP_HIGH << "return..." << LOG_ENDL;
        return;
    }

    CTrackerAbstractor *pTrackerAbs = GetTrackerAbs();
    if(pTrackerAbs == NULL) return;
    nAcount = pTrackerAbs->GetAudioFileNum(volumeType);
    nVcount = pTrackerAbs->GetVideoFileNum(volumeType);
    nPcount = pTrackerAbs->GetPhotoFileNum(volumeType);

    MP_HIGH << volumeType << ", " << bRescanMode  << ", " << bSuccess  << ", " << nAcount  << ", " << nVcount  << ", " << nPcount << LOG_ENDL;

    // { added by kihyung 2013.05.31
    m_pEnableData[nSource].bMediaEnable4 = nAcount > 0 ? true : false;
    m_pEnableData[nSource].bMediaEnable5 = nVcount > 0 ? true : false;
    m_pEnableData[nSource].bMediaEnable6 = nPcount > 0 ? true : false;    
    // } added by kihyung 2013.05.31
    
        //m_nAcount = nAcount; // removed by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB

    // { modified by kihyung 2013.07.05 for ITS 0178512 
    if(bRescanMode == false && (volumeType == eUSB_FRONT || volumeType == eUSB_REAR)) {
        // modified by kihyung 2013.09.09 for ITS 0188809 // modified by Michael.Kim 2014.02.21 for ITS 226485
        //if(nAcount == 0 && nVcount == 0) {
        if(nAcount == 0 && nVcount == 0 && nPcount == 0) {
            OSDManager::instance()->displayNoMediaFileOSD(nSource);
        }
    }
    // } modified by kihyung

    m_pMediaManager->SetSourceAvailable(nSource, MEDIA_TYPE_MUSIC, (nAcount > 0));
    m_pMediaManager->SetSourceAvailable(nSource, MEDIA_TYPE_VIDEO, (nVcount > 0));
    m_pMediaManager->SetSourceAvailable(nSource, MEDIA_TYPE_PHOTO, (nPcount > 0));

    if(bRescanMode == false) 
    {
        m_pMediaManager->EmitDeviceAttached(nSource, MEDIA_TYPE_MUSIC, false);
        m_pMediaManager->EmitDeviceAttached(nSource, MEDIA_TYPE_VIDEO, false);
        m_pMediaManager->EmitDeviceAttached(nSource, MEDIA_TYPE_PHOTO, false);

        // Added by kihyung 2013.07.31 for ISV 88268
        TAMediaFileFound(volumeType, bRescanMode, (nAcount > 0), (nVcount > 0), (nPcount > 0));

        bool bStartAudio = true;
        if(volumeType == eUSB_FRONT || volumeType == eUSB_REAR)
        {
            QString strUSBID      = m_pMediaManager->GetID(nSource);
            QString strOtherUSBID = m_pMediaManager->GetID(nSource == MEDIA_SOURCE_USB1 ? MEDIA_SOURCE_USB2 : MEDIA_SOURCE_USB1); // added by kihyung 2013.09.30 for ITS 0191538
            QString strLastUSB1ID = AppMediaPlayer_SettingsStorage::GetInstance()->GetLastUSB1ID();
            QString strLastUSB2ID = AppMediaPlayer_SettingsStorage::GetInstance()->GetLastUSB2ID();
            int     nLastUSB1Mode = AppMediaPlayer_SettingsStorage::GetInstance()->GetLastUSB1AVMode();
            int     nLastUSB2Mode = AppMediaPlayer_SettingsStorage::GetInstance()->GetLastUSB2AVMode();

            if(strLastUSB1ID.isEmpty() == false && strUSBID == strLastUSB1ID) {
                // modified by kihyung 2013.09.30 for ITS 0191538
                if((APP_MEDIA_MODE_T)nLastUSB1Mode == APP_MEDIA_MODE_VIDEO && strOtherUSBID.isEmpty() == true) bStartAudio = false;
            }
            else if(strLastUSB2ID.isEmpty() == false && strUSBID == strLastUSB2ID) {
                // modified by kihyung 2013.09.30 for ITS 0191538
                if((APP_MEDIA_MODE_T)nLastUSB2Mode == APP_MEDIA_MODE_VIDEO && strOtherUSBID.isEmpty() == true) bStartAudio = false;
            }

            if(bStartAudio == false && nVcount <= 0) {
                bStartAudio = true;
            }

            MP_HIGH << strUSBID << strOtherUSBID << strLastUSB1ID << strLastUSB2ID << nLastUSB1Mode << nLastUSB2Mode << bStartAudio << LOG_ENDL;
        }
        
        if(nAcount > 0 && bStartAudio == true) {
            if(IsConnectAVP2UISH() == true) {
                m_pIsSendEnableMedia2UISH[nSource] = true;
                m_pNotifier->setEnableMedia(nSource, MEDIA_TYPE_MUSIC);
            }
        }
        else if(nVcount > 0) {
            if(IsConnectAVP2UISH() == true) {
                m_pIsSendEnableMedia2UISH[nSource] = true;
                m_pNotifier->setEnableMedia(nSource, MEDIA_TYPE_VIDEO);
            }
        }
        else {
            if(IsConnectAVP2UISH() == true) {
                m_pIsSendEnableMedia2UISH[nSource] = true;
                m_pNotifier->setEnableMedia(nSource, MEDIA_TYPE_MAX);
            }
        }        
    }
    else if(nSource == MEDIA_SOURCE_JUKEBOX)
    {
        if(IsConnectAVP2UISH() == true) {
            m_pIsSendEnableMedia2UISH[nSource] = true;
            m_pNotifier->setEnableMedia(nSource, MEDIA_TYPE_MAX);
        }

        // { modified by kihyung 2013.07.09 for ITS 0177427
        AppMediaPlayer_Controller_Audio *pAEngine = (AppMediaPlayer_Controller_Audio*)(GetEngine(APP_MEDIA_MODE_AUDIO)->GetController());
        if(pAEngine) {
            // { modified by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB
            if(m_nAcount != 0 && m_nAcount < nAcount)  //modified by sangmin.seol 2013.12.10 for ITS 0214221
                pAEngine->requestAllPlaylist(MP::JUKEBOX, true, true);
            else
                pAEngine->requestAllPlaylist(MP::JUKEBOX, true);

            m_nAcount = nAcount;
            // } modified by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB
        }
        // } modified by kihyung 2013.07.09 for ITS 0177427
    	
        AppMediaPlayer_EngineVideo *pVEngine = (AppMediaPlayer_EngineVideo*)GetEngine(APP_MEDIA_MODE_VIDEO);
        if(pVEngine) {
            pVEngine->createPlaylistFromTA(nSource);
        }

        AppMediaPlayer_EnginePhoto *pP1Engine = (AppMediaPlayer_EnginePhoto*)GetEngine(APP_MEDIA_MODE_PHOTO_FRONT);
        if(pP1Engine) {
            pP1Engine->trytoRestoreDevice(PHOTO_SOURCE_JUKEBOX);
        }

        AppMediaPlayer_EnginePhoto *pP2Engine = (AppMediaPlayer_EnginePhoto*)GetEngine(APP_MEDIA_MODE_PHOTO_REAR);
        if(pP2Engine) {
            pP2Engine->trytoRestoreDevice(PHOTO_SOURCE_JUKEBOX);
        }
    }

    m_pNotifier->UpdateSDPMediaCount(nSource, MEDIA_TYPE_MUSIC, nAcount);
    m_pNotifier->UpdateSDPMediaCount(nSource, MEDIA_TYPE_VIDEO, nVcount);
    m_pNotifier->UpdateSDPMediaCount(nSource, MEDIA_TYPE_PHOTO, nPcount);

    Event tEvent(EVT_POST_EVENT, GetThisAppId(), APP_FMGR, EVT_FM_MEDIA_SCAN_READ_START, QByteArray((char*)&volumeType, sizeof(volumeType)));
    emit NotifyUISH(tEvent);

    if(m_bJukeboxTAScanComplete == false && m_bRequestedJukeboxRescan == false) {
        startTrackerAbsScan(eJukeBox);
    }

#ifdef BOOT_LOADING_SCREEN
    if(m_tBootInfo.bisReadyToPlay == false && m_tBootInfo.bisTAFinish) //ys-20131008 boot
    {
        MP_HIGH<<"ConnectAVP2UISHReadyToPlay - isReadytoPlay" << LOG_ENDL;
        emit isReadytoPlay();
    }
#endif

}

// { modified by kihyung 2013.07.20 for ITS 0179774
void AppMediaPlayer_EngineMain::TAMediaFileFound(ETrackerAbstarctor_VolumeType volumeType, int bRescanMode, int bAudio, int bVideo, int bPhoto)
{
    APP_ENTRY_POINT_T nLastAVEntry = (APP_ENTRY_POINT_T)AppMediaPlayer_SettingsStorage::GetInstance()->GetLoadLastAVEntryPoint();
    if(nLastAVEntry == eAPP_NONE) {
        return;
    }

    MP_HIGH << volumeType << ", " << bRescanMode  << ", " << bAudio  << ", " << bVideo  << ", " << bPhoto  << ", " << nLastAVEntry << LOG_ENDL;

    int                         bFinish      = false;
    int                         bModeFail    = true;
    int                         nOffset      = 0;
    int                         nSize        = 0;
    char                        pRecord[SHM_AVP_RECORD_AVAILABLE_DEV_SIZE];
    MODE_STATE_T                nAVModeState = MODE_STATE_AVOFF;
    EVT_DEVICE_ENABLE_DATA_T    tEnableData;

    memset((void*)&tEnableData, 0x00, sizeof(EVT_DEVICE_ENABLE_DATA_T));

    m_pNotifier->GetSharedMemoryInfo(SHM_OFFSET_RECORD_AVAILABLE_DEV_INFO, (unsigned char*)pRecord, SHM_AVP_RECORD_AVAILABLE_DEV_SIZE);
    
    // JUKEBOX
    if(volumeType == eJukeBox && (nLastAVEntry == eAPP_AVP_AUDIO_MODE_JUKEBOX || nLastAVEntry == eAPP_AVP_VIDEO_MODE_JUKEBOX))
    {
        MP_MEDIUM << "JUKEBOX" << LOG_ENDL;
        if(nLastAVEntry == eAPP_AVP_AUDIO_MODE_JUKEBOX && bAudio == true) {
            nAVModeState = MODE_STATE_JUKEBOX_AUDIO;
            bModeFail    = false;
        }
        else if(nLastAVEntry == eAPP_AVP_VIDEO_MODE_JUKEBOX && bVideo == true) {
            nAVModeState = MODE_STATE_JUKEBOX_VIDEO;
            bModeFail    = false;
        }
        
        nSize   = SHM_JUKEBOX_MEDIA_INFO_SIZE;
        nOffset = SHM_OFFSET_JUKEBOX_MEDIA_INFO;
        bFinish = true;
        pRecord[0] = 1;
    }
    // USB1
    else if(volumeType == eUSB_FRONT && (nLastAVEntry == eAPP_AVP_AUDIO_MODE_USB1 || nLastAVEntry == eAPP_AVP_VIDEO_MODE_USB1))
    {
        MP_MEDIUM << "USB1" << LOG_ENDL;
        if(nLastAVEntry == eAPP_AVP_AUDIO_MODE_USB1 && bAudio == true) {
            nAVModeState = MODE_STATE_USB1_AUDIO;
            bModeFail    = false;
        }
        else if(nLastAVEntry == eAPP_AVP_VIDEO_MODE_USB1 && bVideo == true) {
            nAVModeState = MODE_STATE_USB1_VIDEO;
            bModeFail    = false;
        }
        
        nSize   = SHM_USB1_MEDIA_INFO_SIZE;
        nOffset = SHM_OFFSET_USB1_MEDIA_INFO;
        bFinish = true;
        pRecord[1] = 1;
    }
    // USB2
    else if(volumeType == eUSB_REAR && (nLastAVEntry == eAPP_AVP_AUDIO_MODE_USB2 || nLastAVEntry == eAPP_AVP_VIDEO_MODE_USB2))
    {
        MP_MEDIUM << "USB2" << LOG_ENDL;
        if(nLastAVEntry == eAPP_AVP_AUDIO_MODE_USB2 && bAudio == true) {
            nAVModeState = MODE_STATE_USB2_AUDIO;
            bModeFail    = false;
        }
        else if(nLastAVEntry == eAPP_AVP_VIDEO_MODE_USB2 && bVideo == true) {
            nAVModeState = MODE_STATE_USB2_VIDEO;
            bModeFail    = false;
        }
        
        nSize   = SHM_USB2_MEDIA_INFO_SIZE;
        nOffset = SHM_OFFSET_USB2_MEDIA_INFO;
        bFinish = true;
        pRecord[2] = 1;
    }

    if(bFinish == true) 
    {
        AppMediaPlayer_SettingsStorage::GetInstance()->SetLoadLastAVEntryPoint(eAPP_NONE);

        tEnableData.ucAvModeState = nAVModeState;
        tEnableData.bMediaEnable0 = bAudio;
        tEnableData.bMediaEnable1 = bVideo;
        tEnableData.bMediaEnable2 = bPhoto;

        MP_HIGH <<nAVModeState << (int)pRecord[0] << (int)pRecord[1] << (int)pRecord[2] << LOG_ENDL;
        
        m_pNotifier->SetSharedMemoryInfo(nOffset, (unsigned char*)&tEnableData, nSize);
        m_pNotifier->SetSharedMemoryInfo(SHM_OFFSET_RECORD_AVAILABLE_DEV_INFO, (unsigned char*)pRecord, SHM_AVP_RECORD_AVAILABLE_DEV_SIZE);

#ifdef BOOT_LOADING_SCREEN
        m_tBootInfo.bisTAFinish = true;
#else
        ConnectAVP2UISH();
#endif
    }
    else
    {
        MP_HIGH << "bFinish=" << bFinish << LOG_ENDL;
    }
}
// } modified by kihyung 2013.07.20 for ITS 0179774

void
AppMediaPlayer_EngineMain::notifyUISH( Event aEvent, DISPLAY_T aDisplay )
{
#if USE_AFW_SWAP_DC
    if(m_pMainUtil->isCommonEvent(aEvent))
        NotifyUISHCommon(aEvent);
    else
        NotifyUISH(aEvent, aDisplay);
#else
    NotifyUISH(aEvent, aDisplay);
#endif
}
// } modified by Sergey for SwapDC

// modified by edo.lee 2013.02.04
void
AppMediaPlayer_EngineMain::UpdateLayout( APP_MEDIA_MODE_T mode, DISPLAY_T display )
{
    MP_LOG <<"mode= "<< mode <<"display= "<< display << LOG_ENDL;
    
    if ( mode != APP_MEDIA_MODE_MAX )
    {
        QDeclarativeItem    *view    = m_pBootstrap->componentItem(mode, display);
        QDeclarativeContext *context = m_pBootstrap->componentContext(mode, display);

        if ( view == NULL || context == NULL )
        {
            MP_LOG << "UI for mode" << mode << "is not available." << LOG_ENDL;
            return;
        }

        //MP_LOG << "Show view: mode:" << mode << "display:" << display << LOG_ENDL;
        // modified by Dmitry 11.10.13 for ITS0194991
        // need to hide previous screen first...
        if ( display == DISPLAY_FRONT &&
             m_AppFrontView != mode &&
             m_AppFrontView != APP_MEDIA_MODE_MAX &&
             m_pBootstrap->componentItem( m_AppFrontView, display ))
        {
            MP_LOG << "Hide front view" << LOG_ENDL;

            m_Engines[m_AppFrontView]->HideUI(display); // modified by Sergey 15.08.2013 for ITS#184382
            m_pBootstrap->componentItem( m_AppFrontView, display )->setProperty("visible", QVariant(false));
        }

        if ( display == DISPLAY_REAR &&
             m_AppRearView != mode &&
             m_AppRearView != APP_MEDIA_MODE_MAX &&
             m_pBootstrap->componentItem( m_AppRearView, display ))
        {
            MP_LOG << "Hide rear view" << LOG_ENDL;
            m_Engines[m_AppRearView]->HideUI(display); // modified by Sergey 15.08.2013 for ITS#184382
            m_pBootstrap->componentItem( m_AppRearView, display )->setProperty("visible", QVariant(false));
        }

        view->setProperty("visible", true);
        m_Engines[mode]->ShowUI( display );

    }
    else // mode == APP_MEDIA_MODE_MAX -- hiding stuff on one of the specific display
    {
        if ( display == DISPLAY_FRONT )
        {
            if ( m_AppFrontView != APP_MEDIA_MODE_MAX && m_pBootstrap->componentItem( m_AppFrontView, display ))
            {
                MP_LOG << "Hide front view" << LOG_ENDL;
                m_Engines[m_AppFrontView]->HideUI(display); // modified by Sergey 15.08.2013 for ITS#184382
                m_pBootstrap->componentItem( m_AppFrontView, display )->setProperty("visible", QVariant(false));
            }
        }
        else
        {
            if ( m_AppRearView != APP_MEDIA_MODE_MAX && m_pBootstrap->componentItem( m_AppRearView, display ))
            {
                MP_LOG << "Hide rear view" << LOG_ENDL;
                m_Engines[m_AppRearView]->HideUI(display); // modified by Sergey 15.08.2013 for ITS#184382
                m_pBootstrap->componentItem( m_AppRearView, display )->setProperty("visible", QVariant(false));
            }
        }
    }	
// modified by Dmitry 11.10.13 for ITS0194991

	if(m_bFrontAVPIsForeground) //added by edo.lee 2013.03.05
	{
	    if ( display == DISPLAY_FRONT )
	    {
	        SetFrontView(mode);
	    }
	    else
	    {
	        SetRearView(mode);
	    }
	}
}

// modified by edo.lee 
QTranslator& AppMediaPlayer_EngineMain::GetTranslator()
{
    return this->m_Translator;
}

// { removed by ravikanth for changes - CR 12929, 12935 (alternative for CR 12182)
// { added by kihyung 2012.07.27 for CR 12182
//MODE_STATE_T AppMediaPlayer_EngineMain::GetLastAVModeEvent()
//{
//    return m_nLastAVModeEvent;
//}

//void AppMediaPlayer_EngineMain::SetLastAVModeEvent(MODE_STATE_T nAVMode)
//{
//    m_nLastAVModeEvent = nAVMode;
//}
// } added by kihyung
// } removed by ravikanth

// { removed by eugeny.novikov 2012.12.10 for CR 16564 fastboot
// { Added by Vishal for CR 13016
//void AppMediaPlayer_EngineMain::StartIPODControllers()
//{
//    // This function will be called only once from Main.cpp, so iPod controllers can be
//    // initialized here

//    m_pMediaManager->StartIPODControllers(*m_Controllers);     // Modified by Radhakrushna 20121012 NO_CR
//}
// } Added by Vishal
// } removed by eugeny.novikov 2012.12.10

// { added by changjin 2012.10.04 for SANITY_CM_AJ338
APP_MEDIA_MODE_T AppMediaPlayer_EngineMain::GetActiveMode(void)
{
    return m_pNotifier->GetActiveMode();
}

void AppMediaPlayer_EngineMain::SetAVOffMode(bool mode)
{
    MP_LOG << mode << LOG_ENDL;
    m_pNotifier->SetAVOffMode(mode);
}

bool AppMediaPlayer_EngineMain::GetAVOffMode(void)
{
    return m_pNotifier->GetAVOffMode();
}
// } added by changjin 2012.10.04

// { removed by junam 2012.11.28 for CR14466
// { added by changjin 2012.10.15 for SANITY_CM_AG056
//void AppMediaPlayer_EngineMain::SetDelayOSDUpdate(bool mode)
//{
//    m_pNotifier->SetDelayOSDUpdate(mode);
//}

//bool AppMediaPlayer_EngineMain::GetDelayOSDUpdate(void)
//{
//    return m_pNotifier->GetDelayOSDUpdate();
//}
// } added by changjin 2012.10.15
// } removed by junam

// { added by richard.dale 2012.11.03 for CR 14076
LocalizeTrigger::LocalizeTrigger()
                : m_pTranslator( new QTranslator )
 {
    m_nLanguage = AppEngineBase::GetLanguage();
    InitTranslator();
    m_languageItem = new MGConfItem("/meegotouch/i18n/language");
    m_timeItem = new MGConfItem("/meegotouch/i18n/lc_time");
    m_collateItem = new MGConfItem("/meegotouch/i18n/lc_collate");
    m_numericItem = new MGConfItem("/meegotouch/i18n/lc_numeric");
    m_monetaryItem = new MGConfItem("/meegotouch/i18n/lc_monetary");
    SetTrackerLocale();
 }
 
 void LocalizeTrigger::SetTrackerLocale()
 {
     QString locale = GetLocaleName();
 
     /* For the Korean variant of the DHAVN system, always use
       * a locale of 'ko_KR' with tracker even if the UI is in
       * in English. Then Korean MP3s with EUC-KR encoded metadata
       * will handled correctly
       */
     if (AppEngineBase::GetCountryVariant() == eCVKorea || AppEngineBase::GetCountryVariant() == eCVNorthAmerica) { // modified by eugene.seo 2013.08.08 for P1_378 (NA-ID3tag)
         locale = QString::fromLatin1("ko_KR");
     }
 
     QVariant v(locale);
     m_languageItem->set(v);
     m_timeItem->set(v);
     m_collateItem->set(v);
     m_numericItem->set(v);
     m_monetaryItem->set(v);
 }
 
 void LocalizeTrigger::SetLanguage( LANGUAGE_T nLanguage )
 {
    m_nLanguage = nLanguage;
    InitTranslator();
    SetTrackerLocale();
 
    emit retrigger();
 }
 
 QString LocalizeTrigger::Translate( QString sStrId )
 {
    return m_pTranslator->translate( "main", sStrId.toUtf8().data() );
 }
 
 void LocalizeTrigger::InitTranslator()
 {
    m_pTranslator->load( GetTransRes() );
    ( (QApplication *) qApp)->installTranslator( m_pTranslator );
 }
 
 QString LocalizeTrigger::GetTransRes()
 {
    switch( m_nLanguage )
    {
       case LANGUAGE_ZH_CMN: return TransRes_ZH_CMN;
       case LANGUAGE_ZH_YUE: return TransRes_ZH_YUE;
       case LANGUAGE_EN_GB: return TransRes_EN_GB;
       case LANGUAGE_EN: return TransRes_EN;
       case LANGUAGE_KO: return TransRes_KO;
       case LANGUAGE_PT: return TransRes_PT;
       case LANGUAGE_FR: return TransRes_FR;
       case LANGUAGE_IT: return TransRes_IT;
       case LANGUAGE_DE: return TransRes_DE;
       case LANGUAGE_ES: return TransRes_ES;
       case LANGUAGE_RU: return TransRes_RU;
       case LANGUAGE_NL: return TransRes_NL;
       case LANGUAGE_SV: return TransRes_SV;
       case LANGUAGE_PL: return TransRes_PL;
       case LANGUAGE_TR: return TransRes_TR;
       case LANGUAGE_CS: return TransRes_CS;
       case LANGUAGE_DA: return TransRes_DA;
       case LANGUAGE_SK: return TransRes_SK;
       case LANGUAGE_AR: return TransRes_AR;
       case LANGUAGE_ES_NA: return TransRes_ES_NA;
       case LANGUAGE_FR_NA: return TransRes_FR_NA;
       default: return TransRes_EN;
    }
 }
 
 QString LocalizeTrigger::GetLocaleName()
 {
     switch( m_nLanguage )
     {
         case LANGUAGE_ZH_CMN: return QString::fromLatin1("zh_CMN");
         case LANGUAGE_ZH_YUE: return QString::fromLatin1("zh_YUE");
         case LANGUAGE_EN_GB: return QString::fromLatin1("en_GB");
         case LANGUAGE_EN: return QString::fromLatin1("en");
         case LANGUAGE_KO: return QString::fromLatin1("ko_KR");
         case LANGUAGE_PT: return QString::fromLatin1("pt");
         case LANGUAGE_FR: return QString::fromLatin1("fr_FR");
         case LANGUAGE_IT: return QString::fromLatin1("it");
         case LANGUAGE_DE: return QString::fromLatin1("de");
         case LANGUAGE_ES: return QString::fromLatin1("es_ES");
         case LANGUAGE_RU: return QString::fromLatin1("ru");
         case LANGUAGE_NL: return QString::fromLatin1("nl");
         case LANGUAGE_SV: return QString::fromLatin1("sv");
         case LANGUAGE_PL: return QString::fromLatin1("pl");
         case LANGUAGE_TR: return QString::fromLatin1("tr");
         case LANGUAGE_CS: return QString::fromLatin1("cs");
         case LANGUAGE_DA: return QString::fromLatin1("da");
         case LANGUAGE_SK: return QString::fromLatin1("sk");
         case LANGUAGE_AR: return QString::fromLatin1("ar");
         case LANGUAGE_FR_NA: return QString::fromLatin1("fr_NA");
         case LANGUAGE_ES_NA: return QString::fromLatin1("es_NA");
         default: return QString::fromLatin1("en");
     }
 }
 
 LocalizeTrigger *LocalizeTrigger::s_self = 0;
 
 LocalizeTrigger *LocalizeTrigger::self()
 {
   if ( s_self == 0 ) s_self = new LocalizeTrigger();
   return s_self;
 }
 // } added by richard.dale 2012.11.03 for CR 14076

// { removed by eugeny.novikov 2012.11.10 for CR 15466
// { added by changjin 2012.11.08 for Mode key handle the same mode change
//void AppMediaPlayer_EngineMain::SetEntryPoint(APP_ENTRY_POINT_T mode)
//{
//    m_PrevEntryPoint = mode;
//}

//APP_ENTRY_POINT_T AppMediaPlayer_EngineMain::GetEntryPoint()
//{
//    return m_PrevEntryPoint;
//}
// } added by changjin 2012.11.08 
// } removed by eugeny.novikov

// { added by Sergey for SwapDC
 void AppMediaPlayer_EngineMain::handleRestoreAVEvent( Event& aEvent )
 {
     if(aEvent.Data().isEmpty())
     {
         MP_LOG << "Empty data. Return." << LOG_ENDL;
         return;
     }

     EVENT_RESTORE_LAST_MODE_DATA_T* eventData = (EVENT_RESTORE_LAST_MODE_DATA_T*)aEvent.Data().data();
     if (eventData)
     {
         // { added by wspark 2012.10.15 for CAN message in BG mode.
         APP_MEDIA_MODE_T notifier_mode = APP_MEDIA_MODE_MAX;
         switch(eventData->sMode)
         {
         case MODE_STATE_JUKEBOX_AUDIO:
         case MODE_STATE_USB1_AUDIO:
         case MODE_STATE_USB2_AUDIO:
         case MODE_STATE_IPOD1_AUDIO:
         case MODE_STATE_IPOD2_AUDIO:
         case MODE_STATE_AUX1_AUDIO:
         case MODE_STATE_AUX2_AUDIO:
         case MODE_STATE_BT_AUDIO:
         case MODE_STATE_DISC_AUDIO:
             m_pNotifier->onSetDisplayOSD(true); //added by yungi 2012.11.30 for ISV 63920
             notifier_mode = APP_MEDIA_MODE_AUDIO;
             break;
         case MODE_STATE_DISC_VIDEO:
         case MODE_STATE_USB1_VIDEO:
         case MODE_STATE_USB2_VIDEO:
         case MODE_STATE_JUKEBOX_VIDEO:
         case MODE_STATE_AUX1_VIDEO:
         case MODE_STATE_AUX2_VIDEO:
             m_pNotifier->onSetDisplayOSD(true); //added by yungi 2012.11.30 for ISV 63920
             notifier_mode = APP_MEDIA_MODE_VIDEO;
             break;
         default:
             notifier_mode = APP_MEDIA_MODE_MAX;
             break;
         }

         loadComponents(notifier_mode, DISPLAY_FRONT); // modified by edo.lee 2013.02.04
         m_pNotifier->SetActiveMode( notifier_mode );
         // } added by wspark
         GetEngine( APP_MEDIA_MODE_AUDIO )->Restore( eventData->sMode );
         GetEngine( APP_MEDIA_MODE_VIDEO )->Restore( eventData->sMode );
         SetAVOffMode(false); //added by changjin 2012.10.04 for SANITY_CM_AJ338
     }
 }
// } added by Sergey for SwapDC

// removed by Sergey 07.09.2013 for PRE_FG supoort
//remove by edo.lee 2013.11.08
 // { added by wspark 2013.04.10 for cluster update
 void AppMediaPlayer_EngineMain::onClusterUpdateTimer()
 {
     emit notifyBtCallProgressing(false);
     // { added by wspark 2013.04.17
     emit sendTpMessage();
     // } added by wspark
 }
 // } added by wspark

void AppMediaPlayer_EngineMain::IPodDeviceConnected(QString strNodePath, QString strSerial, EIPodPorts ePortNum, bool isFirst)
{
    MP_HIGH << "-----> START" << strNodePath << strSerial << ePortNum << isFirst << LOG_ENDL;

    MEDIA_SOURCE_T          nSource;
    USB_PORT_T              eUsbPortNum;
    int                     nIndex = 0;
    APP_EVENT_IPOD_DATA_T   stMessage;

    //{ added by jaehwan 2014.04.03 to fix the issue that ipod icon is not activated after reboot.
    if (IsConnectAVP2UISH() == false)
    {
        MP_HIGH << "AVP is not Connected with UISH . Save Ipod Connection Infomation." << LOG_ENDL;
        m_pendingIpodEnabled = true;
        m_iPodStrNodePath = strNodePath;
        m_iPodStrSerial = strSerial;
        m_iPodEPortNum = ePortNum;
        m_iPodIsFirst = isFirst;
    }
    //} added by jaehwan

    if(isFirst == true)
    {
        OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_READING);

        if(ePortNum == eIPOD_PORT_1)
        {
            eUsbPortNum = eUSB_PORT1;
        }
        else if(ePortNum == eIPOD_PORT_2)
        {
            eUsbPortNum = eUSB_PORT2;
        }
        else
        {
            eUsbPortNum = eUSB_PORT_INVALID;
        }

        m_pMediaManager->onIPODConnected( eUsbPortNum, strSerial, strNodePath, USB_DATA_CONTENT_NO );

    	stMessage.header.reasonEvent    = EVT_IPOD_MOUNTED;
    	stMessage.header.dataSize       = sizeof( stMessage.mountPath );
    	stMessage.header.sIsJogEnabled  = false;
    	stMessage.header.appIdReciever  = APP_UI_MANAGER;

    	strncpy( stMessage.SerialNum, strSerial.toLocal8Bit().data(),   PATH_MAX_LENGTH );
    	strncpy( stMessage.mountPath, strNodePath.toLocal8Bit().data(), PATH_MAX_LENGTH );

    	stMessage.contentmask           = USB_DATA_CONTENT_NO;
    	stMessage.usbPort               = eUsbPortNum;

        if(IsConnectAVP2UISH() == true) 
        {
            MP_HIGH << "NotifyUISHCommon EVT_IPOD_MOUNTED" << LOG_ENDL;
            if(ePortNum == eIPOD_PORT_1)
                m_pIsSendIPODMounted2UISH[0] = true;
            else if(ePortNum == eIPOD_PORT_2)
                m_pIsSendIPODMounted2UISH[1] = true;
            Event tEvent(EVT_IPOD_MOUNTED, GetThisAppId(), APP_UI_MANAGER, QByteArray((char*)&stMessage, sizeof(stMessage)));
            emit NotifyUISHCommon(tEvent);
        }
    }
    else
    {
        if(ePortNum == eIPOD_PORT_1)
        {
            eUsbPortNum = eUSB_PORT1;
            nSource     = MEDIA_SOURCE_IPOD1;
            nIndex      = 0;
        }
        else if(ePortNum == eIPOD_PORT_2)
        {
            eUsbPortNum = eUSB_PORT2;
            nSource     = MEDIA_SOURCE_IPOD2;
            nIndex      = 1;
        }
        else
        {
            return;
        }

        if(IsConnectAVP2UISH() == true) 
        {
            if(m_pIsSendIPODMounted2UISH[nIndex] == false) 
            {
                QString               sMountPath = m_pMediaManager->GetMountPath(nSource);
                QString               sUUID      = m_pMediaManager->GetID(nSource);

            	stMessage.header.reasonEvent    = EVT_IPOD_MOUNTED;
            	stMessage.header.dataSize       = sizeof( stMessage.mountPath );
            	stMessage.header.sIsJogEnabled  = false;
            	stMessage.header.appIdReciever  = APP_UI_MANAGER;

            	strncpy( stMessage.SerialNum, sUUID.toLocal8Bit().data(),      PATH_MAX_LENGTH );
            	strncpy( stMessage.mountPath, sMountPath.toLocal8Bit().data(), PATH_MAX_LENGTH );

            	stMessage.contentmask           = USB_DATA_CONTENT_NO;
            	stMessage.usbPort               = eUsbPortNum;

                MP_HIGH << "NotifyUISHCommon EVT_IPOD_MOUNTED for IPOD PORT" << ePortNum << LOG_ENDL;
                
                Event tEvent(EVT_IPOD_MOUNTED, GetThisAppId(), APP_UI_MANAGER, QByteArray((char*)&stMessage, sizeof(stMessage)));
                emit NotifyUISHCommon(tEvent);

                m_pIsSendIPODMounted2UISH[nIndex] = true;

            }
            
            m_pIsSendEnableMedia2UISH[nSource] = true;
            m_pNotifier->setEnableMedia(nSource, MEDIA_TYPE_MAX);
        }
    }

    MP_LOG << "-----> END" << LOG_ENDL;
}
 
void AppMediaPlayer_EngineMain::IPodDeviceDisconnected(QString strDeviceName, EIPodPorts ePortNum)
{
    MP_HIGH << "-----> START" << strDeviceName << ePortNum << LOG_ENDL;

    int        nIndex = 0;
    USB_PORT_T eUsbPortNum;

    if(ePortNum == eIPOD_PORT_1)
    {
        eUsbPortNum = eUSB_PORT1;
        nIndex      = 0;
    }
    else if(ePortNum == eIPOD_PORT_2)
    {
        eUsbPortNum = eUSB_PORT2;
        nIndex      = 1;
    }
    else
    {
        eUsbPortNum = eUSB_PORT_INVALID;
        nIndex      = -1;
    }
    /*
    if(strDeviceName.isEmpty() == false) {
        m_pNotifier->displayOSD(OSD_EVENT_IPOD_DISCONNECTED);
    }
    */
    m_pMediaManager->onIPODDisconnected( eUsbPortNum );

    APP_EVENT_IPOD_DATA_T 	stMessage;

    memset(&stMessage, 0x00, sizeof(stMessage));

    stMessage.header.reasonEvent    = EVT_IPOD_REMOVED;
    stMessage.header.dataSize       = sizeof( stMessage.mountPath );
    stMessage.header.sIsJogEnabled  = false;
    stMessage.header.appIdReciever  = APP_UI_MANAGER;
    stMessage.contentmask           = USB_DATA_CONTENT_NO;
    stMessage.usbPort               = eUsbPortNum;
    
    if(strDeviceName.isEmpty() == false) {
        strncpy(stMessage.SerialNum, strDeviceName.toLocal8Bit().data(), PATH_MAX_LENGTH);
    }

	// removed by Sergey 07.09.2013 for PRE_FG supoort
    
    if(IsConnectAVP2UISH() == true) {
        if(nIndex == 0 || nIndex == 1) {
            m_pIsSendIPODRemoved2UISH[nIndex] = 1;
        }
        Event tEvent(EVT_IPOD_REMOVED, GetThisAppId(), APP_UI_MANAGER, QByteArray((char *)&stMessage, sizeof(stMessage)));
        emit NotifyUISHCommon(tEvent);
    }
    else {
        if(nIndex == 0 || nIndex == 1) {
            m_pIsSendIPODRemoved2UISH[nIndex] = 2;
        }
    }

    MP_LOG << "-----> END" << LOG_ENDL;
}

// { added by Sergey 16.04.2013
//added by edo.lee 2013.07.07
void AppMediaPlayer_EngineMain::onShowOSDReleaseClone()
{
    MP_LOG<<m_releasingClone << LOG_ENDL;
	/* remove by edo.lee 2013.07.09
    if(m_releasingClone)
    {
        m_releasingClone = false;
        GetNotifier()->onSetDisplayOSD(true);
		if(GetLastPlayMode() == APP_MEDIA_MODE_AUDIO)
	        GetNotifier()->displayOSD(OSD_EVENT_AUDIO_TRACK_INFO);
        else if(GetLastPlayMode() == APP_MEDIA_MODE_VIDEO)// video loading is slow
            GetNotifier()->DisplayVideoTrackOSD();
    }
	m_releaseCloneTimer.stop();
	*/
}
//added by edo.lee 2013.07.07

// { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
void AppMediaPlayer_EngineMain::onSendDeckErrorToUISH()
{
    // send Deck error to UISH then close Popup
    sendDeckErrorToUISH_Impl(true);
}
// } added by cychoi 2014.04.09

void AppMediaPlayer_EngineMain::setSubCreenOn(bool on, QString screenName )
{
	MP_LOG<< on << screenName << LOG_ENDL;
	m_subscreenOn = on;
	m_subScreenName = screenName;
    // { commented by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
	   // { modified by edo.lee 2013.07.30 subscreen OSD
	//if(on)
	//	emit showSubScreen();
	//else
	//	emit hideSubScreen();
	   // } modified by edo.lee 2013.07.30 
    // } commented by cychoi 2013.09.22
}
void AppMediaPlayer_EngineMain::setFullScreen(bool isFront)
{
	MP_LOG<<isFront << LOG_ENDL;
	emit showFullScreen(isFront);
}
  //  added by edo.lee 2013.07.30 fullscreen OSD
void AppMediaPlayer_EngineMain::setPlaybackScreen(bool isFront)
{
	MP_LOG<<isFront << LOG_ENDL;
	emit hideFullScreen(isFront);
}

// { added by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
void AppMediaPlayer_EngineMain::setVideoSubScreen(bool isFront, bool isOn)
{
    MP_LOG<<"isFront: "<<isFront<<"isOn: "<<isOn << LOG_ENDL;
    if(isOn)
        emit showVideoSubScreen(isFront);
    else
        emit hideVideoSubScreen(isFront);
}
// } added by cychoi 2013.09.22

QString AppMediaPlayer_EngineMain::getSubCreenName()
{
	MP_LOG << LOG_ENDL;
	if(m_subscreenOn)
		return m_subScreenName;
	else
		return "";		
}
/*
 *  In case of SWAP DC front and rear screen IDs are switched.
 *  This function returns current display after checking if SWAP DC mode is enabled.
 *  Use this function if required to get actual screen where UI is drawing.
 */
DISPLAY_T AppMediaPlayer_EngineMain::getSwapScreen(DISPLAY_T disp)
{
    DISPLAY_T result = DISPLAY_NONE;

    MP_LOG << "MMM disp =" << disp << "bSwap = " << getIsSwapEnabled() << LOG_ENDL;

    if(disp != DISPLAY_NONE)
    {
        bool bSwap = getIsSwapEnabled();

        if(bSwap == false)
            result = disp;
        else
            result = (disp == DISPLAY_FRONT) ? DISPLAY_REAR : DISPLAY_FRONT;
    }

    MP_LOG << "MMM result =" << result << LOG_ENDL;
    return result;
}
// } added by Sergey 16.04.2013

// { modified by ravikanth 16-04-13
 void AppMediaPlayer_EngineMain::cancelAudioFileCopyOperation()
 {
     AppMediaPlayer_EngineAudio* pEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_Engines[APP_MEDIA_MODE_AUDIO]);
     if(pEngine)
         pEngine->CancelAudioCopy();
 }
// } modified by ravikanth 16-04-13 

// { modified by ravikanth 18-04-13
void AppMediaPlayer_EngineMain::setCopyInProgress(bool isCopying, int usbType) // modified by eugene.seo 2013.05.29
{
    MP_HIGH << isCopying << usbType << LOG_ENDL;
    
    m_jukeboxCopyInProgress = isCopying;
    if(!isCopying)
        m_pSharedFMData->SetStatusDateByIndex(DATA_TYPE_FILEOPERATION_STATUS,FILEOPERATION_STATUS_NONE);
	if(usbType != 0) // added by eugene.seo 2013.05.29
		m_usbType = usbType; // added by eugene.seo 2013.05.29
    emit jukeboxCopyInProgressChanged(m_jukeboxCopyInProgress);

    if(isCopying == true) // added by kihyung 2013.11.16 to remove sync error after copy/delete.
    {
        AppMediaPlayer_SettingsStorage::GetInstance()->SaveCommonSettings(this); //modified by sangmin.seol 2014.09.24 to fix "data load fail" // added by kihyung 2013.07.24 for ITS 0180896
    }
    if(!m_jukeboxCopyInProgress)
        emit closeCopyCancelConfirmPopup(); // modified for ITS 0217509
}
// } modified by ravikanth 18-04-13

// added by Dmitry 28.04.13
void AppMediaPlayer_EngineMain::setisBTCall(bool call)
{
   m_isBTCall = call;
   emit isBTCallChanged();
}
// added by Dmitry 28.04.13

// { added by kihyung 2013.07.05 for ISV 84474
void AppMediaPlayer_EngineMain::CheckMediaLastMode()
{
    bool                        bFSModeSuccess   = false;
    bool                        bIPODModeSuccess = false;
    MODE_STATE_T                nIPODModeState   = MODE_STATE_AVOFF;
    int                         nIPODOffset      = 0;
    int                         nIPODSize        = 0;
    int                         nUSBMount        = 0;
    int                         nIPODMount       = 0;
    char                        pRecord[SHM_AVP_RECORD_AVAILABLE_DEV_SIZE];
    APP_ENTRY_POINT_T           nLastAVEntry     = GetLastAVEntryPoint(true);
    EVT_DEVICE_ENABLE_DATA_T    tEnableData;

    MP_HIGH << "nLastAVEntry = " << nLastAVEntry << LOG_ENDL;

    memset((void*)&tEnableData, 0x00, sizeof(EVT_DEVICE_ENABLE_DATA_T));

    m_pNotifier->GetSharedMemoryInfo(SHM_OFFSET_RECORD_AVAILABLE_DEV_INFO, (unsigned char*)pRecord, SHM_AVP_RECORD_AVAILABLE_DEV_SIZE);

    ////////////////////////////////////////////////////////////////////////
    // USB1
    if(nLastAVEntry == eAPP_AVP_AUDIO_MODE_USB1 || nLastAVEntry == eAPP_AVP_VIDEO_MODE_USB1) 
    {
        m_pNotifier->GetSharedMemoryInfo(SHM_OFFSET_USB1_MOUNT_INFO, (unsigned char *)&nUSBMount, SHM_USB1_MOUNT_INFO_SIZE); // modified by kihyung 2013.07.16 for STATIC 23538
        MP_HIGH << "USB 1 = " << nUSBMount << LOG_ENDL;
        if(nUSBMount == EVT_USB_MOUNTED) 
        {
            char    pUSBPath[SHM_USB1_MOUNTPATH_SIZE + 1] = {0,};
            char    pUSBUUID[SHM_USB1_UUID_INFO_SIZE + 1] = {0,};
            
            m_pNotifier->GetSharedMemoryInfo(SHM_OFFSET_USB1_MOUNTPATH_INFO, (unsigned char *)pUSBPath, SHM_USB1_MOUNTPATH_SIZE); // modified by kihyung 2013.07.16 for STATIC 23538
            m_pNotifier->GetSharedMemoryInfo(SHM_OFFSET_USB1_UUID_INFO, (unsigned char *)pUSBUUID, SHM_USB1_UUID_INFO_SIZE); // modified by kihyung 2013.07.16 for STATIC 23538

            QString strUSBPath = QString::fromUtf8(pUSBPath);
            QString strUSBUUID = QString::fromUtf8(pUSBUUID);    

            // { modified by kihyung 2013.07.31 for ISV 88437
            AppMediaPlayer_SettingsStorage::GetInstance()->SetBootUSBPort(eUSB_PORT1);
            AppMediaPlayer_SettingsStorage::GetInstance()->SetBootUSBUUID(strUSBUUID);
            // } modified by kihyung 2013.07.31 for ISV 88437
            AppMediaPlayer_SettingsStorage::GetInstance()->SetLoadLastAVEntryPoint(nLastAVEntry);
            bFSModeSuccess = m_pMediaManager->onUSBDevConnected(eUSB_PORT1, strUSBUUID, strUSBPath);
        }
    }
    ////////////////////////////////////////////////////////////////////////
    // USB2
    else if(nLastAVEntry == eAPP_AVP_AUDIO_MODE_USB2 || nLastAVEntry == eAPP_AVP_VIDEO_MODE_USB2) 
    {
        m_pNotifier->GetSharedMemoryInfo(SHM_OFFSET_USB2_MOUNT_INFO, (unsigned char *)&nUSBMount, SHM_USB2_MOUNT_INFO_SIZE); // modified by kihyung 2013.07.16 for STATIC 23538
        MP_HIGH << "USB 2 = " << nUSBMount << LOG_ENDL;
        if(nUSBMount == EVT_USB_MOUNTED) 
        {
            char    pUSBPath[SHM_USB2_MOUNTPATH_SIZE + 1] = {0,};
            char    pUSBUUID[SHM_USB2_UUID_INFO_SIZE + 1] = {0,};
            
            m_pNotifier->GetSharedMemoryInfo(SHM_OFFSET_USB2_MOUNTPATH_INFO, (unsigned char *)pUSBPath, SHM_USB2_MOUNTPATH_SIZE); // modified by kihyung 2013.07.16 for STATIC 23538
            m_pNotifier->GetSharedMemoryInfo(SHM_OFFSET_USB2_UUID_INFO, (unsigned char *)pUSBUUID, SHM_USB2_UUID_INFO_SIZE); // modified by kihyung 2013.07.16 for STATIC 23538

            QString strUSBPath = QString::fromUtf8(pUSBPath);
            QString strUSBUUID = QString::fromUtf8(pUSBUUID);    

            // { modified by kihyung 2013.07.31 for ISV 88437
            AppMediaPlayer_SettingsStorage::GetInstance()->SetBootUSBPort(eUSB_PORT2);
            AppMediaPlayer_SettingsStorage::GetInstance()->SetBootUSBUUID(strUSBUUID);
            // } modified by kihyung 2013.07.31 for ISV 88437
            AppMediaPlayer_SettingsStorage::GetInstance()->SetLoadLastAVEntryPoint(nLastAVEntry);
            bFSModeSuccess = m_pMediaManager->onUSBDevConnected(eUSB_PORT2, strUSBUUID, strUSBPath);           
        }
    }

    ////////////////////////////////////////////////////////////////////////
    // JUKEBOX
    if(bFSModeSuccess == false)
    {
        AppMediaPlayer_SettingsStorage::GetInstance()->SetLoadLastAVEntryPoint(nLastAVEntry);
        if(startTrackerAbsScan(eJukeBox) == true)
        {
            if(nLastAVEntry == eAPP_AVP_AUDIO_MODE_JUKEBOX || nLastAVEntry == eAPP_AVP_VIDEO_MODE_JUKEBOX)
                bFSModeSuccess = true;
        }
    }

    ////////////////////////////////////////////////////////////////////////    
    // IPOD1
    if(m_pIsSendIPODRemoved2UISH[0] != 1 && GetControllers().GetIPod1Controller()->GetDeviceState() == 0)
    {
        nIPODMount = EVT_IPOD_REMOVED;
        m_pNotifier->SetSharedMemoryInfo(SHM_OFFSET_IPOD1_MOUNT_INFO, (unsigned char*)&nIPODMount, SHM_IPOD1_MOUNT_INFO_SIZE);
        m_pIsSendIPODRemoved2UISH[0] = 1;

        MP_HIGH << "IPOD 1--->EVT_IPOD_REMOVED" << LOG_ENDL;
    }
    else 
    {
        MP_HIGH << "IPOD 1--->CONNECTING..." << LOG_ENDL;
    }
    GetControllers().GetIPod1Controller()->SetSystemState(SYSTEM_ACC_ON);
    
    if(m_pMediaManager->GetMountPath(MEDIA_SOURCE_IPOD1).isEmpty() == false) 
    {
        nIPODSize      = SHM_IPOD1_MEDIA_INFO_SIZE;            
        nIPODOffset    = SHM_OFFSET_IPOD1_MEDIA_INFO;
        nIPODModeState = MODE_STATE_IPOD1_AUDIO;

        AppMediaPlayer_EngineAudio      *pEngineAudio = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
        AppMediaPlayer_Controller_Audio *pController  = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();
        //{modified by kihyung 2013.07.16 for STATIC 23542
        //sprintf(tEnableData.cstrDeviceName, pController->getIPODDeviceName(MEDIA_SOURCE_IPOD1));
        //suryanto.tan 2015.01.05 Add logs and use %s when using snprintf

        QString strDeviceName1 = pController->getIPODDeviceName(MEDIA_SOURCE_IPOD1);
        QByteArray strByteArray1 = strDeviceName1.toUtf8();

        snprintf(tEnableData.cstrDeviceName, sizeof(tEnableData.cstrDeviceName), "%s", strByteArray1.data());
        //}modified by kihyung 2013.07.16 for STATIC 23542        

        tEnableData.ucAvModeState = nIPODModeState;
        tEnableData.bMediaEnable0 = true;
        tEnableData.bMediaEnable1 = false;
        tEnableData.bMediaEnable2 = false;
        m_pNotifier->SetSharedMemoryInfo(nIPODOffset, (unsigned char*)&tEnableData, nIPODSize);

        pRecord[4] = 0;
        m_pNotifier->SetSharedMemoryInfo(SHM_OFFSET_RECORD_AVAILABLE_DEV_INFO, (unsigned char*)pRecord, SHM_AVP_RECORD_AVAILABLE_DEV_SIZE);

        if(nLastAVEntry == eAPP_AVP_AUDIO_MODE_IPOD1) {
            bIPODModeSuccess = true;
        }

        nIPODMount = EVT_IPOD_MOUNTED;
        m_pNotifier->SetSharedMemoryInfo(SHM_OFFSET_IPOD1_MOUNT_INFO, (unsigned char*)&nIPODMount, SHM_IPOD1_MOUNT_INFO_SIZE);
        MP_HIGH << "IPOD 1--->EVT_IPOD_MOUNTED" << LOG_ENDL;
    }

    ////////////////////////////////////////////////////////////////////////
    // IPOD2
    if(m_pIsSendIPODRemoved2UISH[1] != 1 && GetControllers().GetIPod2Controller()->GetDeviceState() == 0)
    {
        nIPODMount = EVT_IPOD_REMOVED;
        m_pNotifier->SetSharedMemoryInfo(SHM_OFFSET_IPOD2_MOUNT_INFO, (unsigned char*)&nIPODMount, SHM_IPOD2_MOUNT_INFO_SIZE);
        m_pIsSendIPODRemoved2UISH[1] = 1;
        
        MP_HIGH << "IPOD 2--->EVT_IPOD_REMOVED" << LOG_ENDL;
    }
    else 
    {
        MP_HIGH << "IPOD 2--->CONNECTING..." << LOG_ENDL;
    }

    if(GetVariantRearUSB())//added by junam 2013.07.24 for disable rear iPod port
    {
        MP_HIGH << "IPOD 2--->SUPPORTED" << LOG_ENDL;
        GetControllers().GetIPod2Controller()->SetSystemState(SYSTEM_ACC_ON);
    }
    else 
    {
        MP_HIGH << "IPOD 2--->NOT SUPPORTED" << LOG_ENDL;        
    }
    
    if(m_pMediaManager->GetMountPath(MEDIA_SOURCE_IPOD2).isEmpty() == false) 
    {
        nIPODSize      = SHM_IPOD2_MEDIA_INFO_SIZE;            
        nIPODOffset    = SHM_OFFSET_IPOD2_MEDIA_INFO;
        nIPODModeState = MODE_STATE_IPOD2_AUDIO;

        AppMediaPlayer_EngineAudio      *pEngineAudio = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
        AppMediaPlayer_Controller_Audio *pController  = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();
        //{modified by kihyung 2013.07.16 for STATIC 23542
        //sprintf(tEnableData.cstrDeviceName, pController->getIPODDeviceName(MEDIA_SOURCE_IPOD2));
        //suryanto.tan 2015.01.05 Add logs and use %s when using snprintf
        QString strDeviceName2 = pController->getIPODDeviceName(MEDIA_SOURCE_IPOD2);
        QByteArray strByteArray2 = strDeviceName2.toUtf8();
        snprintf(tEnableData.cstrDeviceName, sizeof(tEnableData.cstrDeviceName), "%s", strByteArray2.data());
        //}modified by kihyung 2013.07.16 for STATIC 23542

        bIPODModeSuccess = true;            

        tEnableData.ucAvModeState = nIPODModeState;
        tEnableData.bMediaEnable0 = true;
        tEnableData.bMediaEnable1 = false;
        tEnableData.bMediaEnable2 = false;
        m_pNotifier->SetSharedMemoryInfo(nIPODOffset, (unsigned char*)&tEnableData, nIPODSize);

        pRecord[5] = 0;
        m_pNotifier->SetSharedMemoryInfo(SHM_OFFSET_RECORD_AVAILABLE_DEV_INFO, (unsigned char*)pRecord, SHM_AVP_RECORD_AVAILABLE_DEV_SIZE);

        if(nLastAVEntry == eAPP_AVP_AUDIO_MODE_IPOD2) {
            bIPODModeSuccess = true;
        }

        nIPODMount = EVT_IPOD_MOUNTED;
        m_pNotifier->SetSharedMemoryInfo(SHM_OFFSET_IPOD2_MOUNT_INFO, (unsigned char*)&nIPODMount, SHM_IPOD2_MOUNT_INFO_SIZE);
        MP_HIGH << "IPOD 2--->EVT_IPOD_MOUNTED" << LOG_ENDL;
    }


    ////////////////////////////////////////////////////////////////////////
    // For TAMediaFileFound()
    if(bFSModeSuccess == false || bIPODModeSuccess == true) 
    {
        nLastAVEntry = eAPP_NONE;
        AppMediaPlayer_SettingsStorage::GetInstance()->SetLoadLastAVEntryPoint(nLastAVEntry);
    }

#ifdef BOOT_LOADING_SCREEN
    // ys-20131011 is AVP empty
    if(m_tBootInfo.bisReadyToPlay == false)
    {
        MP_HIGH<<"ConnectAVP2UISHReadyToPlay - isReadytoPlay" << LOG_ENDL;
        m_tBootInfo.bisFS = bFSModeSuccess;
        m_tBootInfo.bisIpod = bIPODModeSuccess;
        emit isReadytoPlay();
    }
#else
    ////////////////////////////////////////////////////////////////////////
    // CONNECT TO UISH    
    MP_HIGH << "bFSModeSuccess = " << bFSModeSuccess << " bIPODModeSuccess = " << bIPODModeSuccess << LOG_ENDL;
    if(bFSModeSuccess == false || bIPODModeSuccess == true)
    {
        ConnectAVP2UISH();
    }
#endif
}
// } added by kihyung 2013.07.05 for ISV 84474

//{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
void AppMediaPlayer_EngineMain::onBTPlayUnmuteTimerStart()
{
    MP_HIGH << LOG_ENDL;
    m_BTPlayUnmuteTimer->start();
}

void AppMediaPlayer_EngineMain::onBTPlayUnmuteTimer()
{
    MP_HIGH << LOG_ENDL;

    m_BTPlayUnmuteTimer->stop();

    MODE_STATE_T avState = MODE_STATE_NONE;
    avState = MODE_STATE_BT_AUDIO;

    EVENT_AUDIO_PATH_REQUEST_DATA message = { { APP_ALL, EVT_REQUEST_AUDIO_PATH, 0, 0 }, avState };
    
    message.header.dataSize = sizeof( message );
    Event event( EVT_REQUEST_AUDIO_PATH, GetThisAppId(),
                 APP_UI_MANAGER, QByteArray( (char*)&message, sizeof( message ) ) ); // modified by lssanh 2013.02.21 IPC payload

    NotifyUISHCommon( event );

}
//} modified by yongkyun.lee 2014-03-13

#ifdef BOOT_LOADING_SCREEN
void AppMediaPlayer_EngineMain::setAVPBootMode(QString param) //ys-20131010 boot isnotlast
{

    if(param.endsWith("1") == true)
        m_tBootInfo.bisAVPLastMode = true;
    else
        m_tBootInfo.bisAVPLastMode = false;

    MP_HIGH<<"================ EVT m_tBootInfo.bisAVPLastMode" <<param<<m_tBootInfo.bisAVPLastMode << LOG_ENDL;
}

void AppMediaPlayer_EngineMain::setAVModeChange(bool bAVMode)
{
    m_tBootInfo.bisAVModeChange = bAVMode;
}

void AppMediaPlayer_EngineMain::ConnectAVP2UISH()
{
    MP_HIGH<<"================ ConnectToUISH ===============" << LOG_ENDL;
    ConnectToUISH();
    m_bConnectAVP2UISH = true;

}
void AppMediaPlayer_EngineMain::ConnectAVP2UISHTimeOut()
{
    MP_HIGH<<"================ Timer" << LOG_ENDL;
    if(m_tBootInfo.bisReadyToPlay == false)
    {
        MP_HIGH<<"================ EVT_APP_IS_READY_TO_PLAY ===============" << LOG_ENDL;
        Event tResEvent(EVT_APP_IS_READY_TO_PLAY, GetThisAppId(), APP_UI_MANAGER);
        NotifyUISHCommon(tResEvent);
        m_tBootInfo.bisReadyToPlay = true;
    }

}

void AppMediaPlayer_EngineMain::ConnectAVP2UISHReadyToPlay() //ys-20131008 boot
{
    APP_ENTRY_POINT_T tEntry =GetLastAVEntryPoint();

    int discState =  m_pMediaManager->isDiscState();
    MP_HIGH<<"Lastmode:"<<m_tBootInfo.bisAVPLastMode<<"TA:"<<m_tBootInfo.bisTAFinish<<"AVmode:"<<m_tBootInfo.bisAVModeChange << LOG_ENDL;
    MP_HIGH<<"AVP is empty FS:"<<m_tBootInfo.bisFS<<"ipod:"<<m_tBootInfo.bisIpod<<"disc:"<<discState << LOG_ENDL;

    if(m_qBootTimer->isActive())
        m_qBootTimer->stop();

    // AVP is empty
    if(m_tBootInfo.bisFS == false && m_tBootInfo.bisIpod == false &&
       (discState == PLAYER_DISCEJECTED || discState == PLAYER_DISCREMOVED))
    {
        if(m_tBootInfo.bisReadyToPlay == false)
        {
            MP_HIGH<<"================ EVT_APP_IS_READY_TO_PLAY ===============" << LOG_ENDL;
            Event tResEvent(EVT_APP_IS_READY_TO_PLAY, GetThisAppId(), APP_UI_MANAGER);
            NotifyUISHCommon(tResEvent);
            m_tBootInfo.bisReadyToPlay = true;

        }
    }
    else
    {
        if(tEntry == eAPP_AVP_AUDIO_MODE_JUKEBOX || tEntry == eAPP_AVP_VIDEO_MODE_JUKEBOX ||
           tEntry == eAPP_AVP_AUDIO_MODE_USB1 || tEntry == eAPP_AVP_VIDEO_MODE_USB1 ||
           tEntry == eAPP_AVP_AUDIO_MODE_USB2 || tEntry == eAPP_AVP_VIDEO_MODE_USB2)
        {
            if(!m_tBootInfo.bisTAFinish )
            {
                MP_HIGH<<"================ !m_tBootInfo.bisTAFinish" << LOG_ENDL;
                m_qBootTimer->start();
                return;
            }
        }
        else if(tEntry == eAPP_AVP_AUDIO_MODE_DISC || tEntry == eAPP_AVP_VIDEO_MODE_DISC)
        {
            if(discState == -1 || discState == PLAYER_DISCEJECTED || discState == PLAYER_DISCREMOVED ||
               discState == PLAYER_DISCRELOADED || discState == PLAYER_DISCRELOADING || discState == PLAYER_DISCRELOADING ||
               discState == PLAYER_DISCLOADING )
            {

                MP_HIGH<<"================ Disc" << LOG_ENDL;
                m_qBootTimer->start();
                return;
            }
        }


        // If AVP is Last Mode, check the AV ModeChange, other case do not check the av mode change.
        if(!m_tBootInfo.bisAVModeChange && m_tBootInfo.bisAVPLastMode == true) //ys-20131010 boot isnotlast
        {

            MP_HIGH<<"================ AV Mode" << LOG_ENDL;
            m_qBootTimer->start();
            return;
        }


        if(m_tBootInfo.bisReadyToPlay == false)
        {


            APP_MEDIA_MODE_T nLastMediaMode = GetAVMediaMode(tEntry);

            if(nLastMediaMode == APP_MEDIA_MODE_AUDIO || nLastMediaMode == APP_MEDIA_MODE_VIDEO)
            {
                m_pBootstrap->loadComponents(nLastMediaMode, DISPLAY_FRONT);

            //    if(nLastMediaMode == APP_MEDIA_MODE_VIDEO) {
            //        dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO])->loadUI(GetLastAVEntryPoint(), DISPLAY_FRONT);
            //   }
            }
            else
            {
                MP_HIGH << "not preloading" << LOG_ENDL;
            }


            MP_HIGH <<"nLastMediaMode"<< nLastMediaMode << LOG_ENDL;


            //ConnectToUISH();
            //m_bConnectAVP2UISH = true;

            GetMediaManager()->StartDiscControllers();
            GetNotifier()->UpdateMainMenuAfterBoot();


            MP_HIGH<<"================ EVT_APP_IS_READY_TO_PLAY ===============" << LOG_ENDL;
            Event tResEvent(EVT_APP_IS_READY_TO_PLAY, GetThisAppId(), APP_UI_MANAGER);
            NotifyUISHCommon(tResEvent);
            m_tBootInfo.bisReadyToPlay = true;
        }
    }

}
#else
void AppMediaPlayer_EngineMain::ConnectAVP2UISH()
{
    MP_HIGH << LOG_ENDL;
    APP_ENTRY_POINT_T tEntry = GetLastAVEntryPoint();

    if(m_bConnectAVP2UISH == false)
    {


        APP_MEDIA_MODE_T nLastMediaMode = GetAVMediaMode(tEntry);

        if(nLastMediaMode == APP_MEDIA_MODE_AUDIO || nLastMediaMode == APP_MEDIA_MODE_VIDEO)
        {
            m_pBootstrap->loadComponents(nLastMediaMode, DISPLAY_FRONT);

        //    if(nLastMediaMode == APP_MEDIA_MODE_VIDEO) {
        //        dynamic_cast<AppMediaPlayer_EngineVideo*>(m_Engines[APP_MEDIA_MODE_VIDEO])->loadUI(GetLastAVEntryPoint(), DISPLAY_FRONT);
        //   }
        }
        else
        {
            MP_LOG << "not preloading" << LOG_ENDL;
        }


        MP_HIGH <<"nLastMediaMode"<< nLastMediaMode << LOG_ENDL;


        ConnectToUISH();
        m_bConnectAVP2UISH = true;

        //{ added by jaehwan 2014.04.03 to fix the issue that ipod icon is not activated after reboot.
        if (m_pendingIpodEnabled)
        {
            MP_HIGH << "Delayed Call IPodDeviceConnected" << LOG_ENDL;
            IPodDeviceConnected(m_iPodStrNodePath, m_iPodStrSerial, m_iPodEPortNum, m_iPodIsFirst);
            m_pendingIpodEnabled = false;
        }
        //} added by jaehwan

        GetMediaManager()->StartDiscControllers();
        GetNotifier()->UpdateMainMenuAfterBoot();

    }


}
#endif

bool AppMediaPlayer_EngineMain::IsConnectAVP2UISH()
{
    return m_bConnectAVP2UISH;
}

// { modified by kihyung 2013.07.20 for ITS 0180419
void AppMediaPlayer_EngineMain::SetLastAVEntryPoint(APP_ENTRY_POINT_T ep)
{
    switch(ep)
    {
        case eAPP_MODE_DYNAMICS_DISC: // added by wonseok.heo for Disc IN OUT TEST 2013.07.20
        case eAPP_AVP_AUDIO_MODE_JUKEBOX:
        case eAPP_AVP_AUDIO_MODE_USB1:
        case eAPP_AVP_AUDIO_MODE_USB2:
        case eAPP_AVP_AUDIO_MODE_IPOD1:
        case eAPP_AVP_AUDIO_MODE_IPOD2:
        case eAPP_AVP_AUDIO_MODE_DISC:
        case eAPP_AVP_AUDIO_MODE_AUX:
        case eAPP_AVP_AUDIO_MODE_BT:
		case eAPP_AVP_MODE_BLUETOOTH:
        case eAPP_AVP_VIDEO_MODE_JUKEBOX:
        case eAPP_AVP_VIDEO_MODE_USB1:
        case eAPP_AVP_VIDEO_MODE_USB2:
        case eAPP_AVP_VIDEO_MODE_DISC:
        case eAPP_AVP_VIDEO_MODE_AUX:
        case eAPP_AVP_VIDEO_MODE_DISC_DRS:
        case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
        case eAPP_AVP_VIDEO_MODE_USB1_DRS:
        case eAPP_AVP_VIDEO_MODE_USB2_DRS:
        case eAPP_AVP_VIDEO_MODE_AUX_DRS:
        {
            // { modified by kihyung 2013.09.10 for ITS 0188968 
            if(ep == eAPP_AVP_AUDIO_MODE_USB1) 
            {
                AppMediaPlayer_SettingsStorage::GetInstance()->SetLastUSB1ID(m_pMediaManager->GetID(MEDIA_SOURCE_USB1), (int)APP_MEDIA_MODE_AUDIO);

                // added by kihyung 2013.09.30 for ITS 0191538 
                if(m_pMediaManager->GetID(MEDIA_SOURCE_USB1) == AppMediaPlayer_SettingsStorage::GetInstance()->GetLastUSB2ID()) 
                {
                    AppMediaPlayer_SettingsStorage::GetInstance()->SetLastUSB2ID("", APP_MEDIA_MODE_MAX);
                }
            }
            else if(ep == eAPP_AVP_VIDEO_MODE_USB1 || ep == eAPP_AVP_VIDEO_MODE_USB1_DRS) 
            {
                AppMediaPlayer_SettingsStorage::GetInstance()->SetLastUSB1ID(m_pMediaManager->GetID(MEDIA_SOURCE_USB1), (int)APP_MEDIA_MODE_VIDEO);

                // added by kihyung 2013.09.30 for ITS 0191538                 
                if(m_pMediaManager->GetID(MEDIA_SOURCE_USB1) == AppMediaPlayer_SettingsStorage::GetInstance()->GetLastUSB2ID()) 
                {
                    AppMediaPlayer_SettingsStorage::GetInstance()->SetLastUSB2ID("", APP_MEDIA_MODE_MAX);
                }
            }
            else if(ep == eAPP_AVP_AUDIO_MODE_USB2) 
            {
                AppMediaPlayer_SettingsStorage::GetInstance()->SetLastUSB2ID(m_pMediaManager->GetID(MEDIA_SOURCE_USB2), (int)APP_MEDIA_MODE_AUDIO);

                // added by kihyung 2013.09.30 for ITS 0191538 
                if(m_pMediaManager->GetID(MEDIA_SOURCE_USB2) == AppMediaPlayer_SettingsStorage::GetInstance()->GetLastUSB1ID()) 
                {
                    AppMediaPlayer_SettingsStorage::GetInstance()->SetLastUSB1ID("", APP_MEDIA_MODE_MAX);
                }
            }
            else if(ep == eAPP_AVP_VIDEO_MODE_USB2 || ep == eAPP_AVP_VIDEO_MODE_USB2_DRS) 
            {
                AppMediaPlayer_SettingsStorage::GetInstance()->SetLastUSB2ID(m_pMediaManager->GetID(MEDIA_SOURCE_USB2), (int)APP_MEDIA_MODE_VIDEO);
                
                // added by kihyung 2013.09.30 for ITS 0191538 
                if(m_pMediaManager->GetID(MEDIA_SOURCE_USB2) == AppMediaPlayer_SettingsStorage::GetInstance()->GetLastUSB1ID()) 
                {
                    AppMediaPlayer_SettingsStorage::GetInstance()->SetLastUSB1ID("", APP_MEDIA_MODE_MAX);
                }
            }
            // } modified by kihyung 2013.09.10 for ITS 0188968

            m_nLastAVEntryPoint = ep;
            AppMediaPlayer_SettingsStorage::GetInstance()->SaveCommonSettings(this); //modified by sangmin.seol 2014.09.24 to fix "data load fail" // modified by Sergey 19.05.2013
            break;
        }

        default:
            break;
    }
}

MEDIA_SOURCE_T AppMediaPlayer_EngineMain::mapToSource(APP_ENTRY_POINT_T ep)
{
    switch(ep)
    {
        case eAPP_MODE_DYNAMICS_DISC: // added by wonseok.heo for Disc IN OUT TEST 2013.07.20
        case eAPP_AVP_AUDIO_MODE_JUKEBOX:       return MEDIA_SOURCE_JUKEBOX;
        case eAPP_AVP_AUDIO_MODE_USB1:          return MEDIA_SOURCE_USB1;
        case eAPP_AVP_AUDIO_MODE_USB2:          return MEDIA_SOURCE_USB2;
        case eAPP_AVP_AUDIO_MODE_IPOD1:         return MEDIA_SOURCE_IPOD1;
        case eAPP_AVP_AUDIO_MODE_IPOD2:         return MEDIA_SOURCE_IPOD2;
        case eAPP_AVP_AUDIO_MODE_DISC:	        return MEDIA_SOURCE_MP3;
        case eAPP_AVP_AUDIO_MODE_AUX:           return MEDIA_SOURCE_AUX1;
        case eAPP_AVP_AUDIO_MODE_BT:            return MEDIA_SOURCE_BLUETOOTH;
		case eAPP_AVP_MODE_BLUETOOTH:           return MEDIA_SOURCE_BLUETOOTH;
        case eAPP_AVP_VIDEO_MODE_JUKEBOX:       return MEDIA_SOURCE_JUKEBOX;
        case eAPP_AVP_VIDEO_MODE_USB1:          return MEDIA_SOURCE_USB1;
        case eAPP_AVP_VIDEO_MODE_USB2:          return MEDIA_SOURCE_USB2;
        case eAPP_AVP_VIDEO_MODE_DISC:          return MEDIA_SOURCE_DVDVIDEO;
        case eAPP_AVP_VIDEO_MODE_AUX:           return MEDIA_SOURCE_AUX1;
        case eAPP_AVP_VIDEO_MODE_DISC_DRS:      return MEDIA_SOURCE_DVDVIDEO;
        case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:   return MEDIA_SOURCE_JUKEBOX;
        case eAPP_AVP_VIDEO_MODE_USB1_DRS:      return MEDIA_SOURCE_USB1;
        case eAPP_AVP_VIDEO_MODE_USB2_DRS:      return MEDIA_SOURCE_USB2;
        case eAPP_AVP_VIDEO_MODE_AUX_DRS:       return MEDIA_SOURCE_AUX1;
        default:                                return MEDIA_SOURCE_MAX;
    }
}

APP_ENTRY_POINT_T AppMediaPlayer_EngineMain::GetLastAVEntryPoint(bool bFromSetting)
{
    APP_ENTRY_POINT_T nResult;
    if(bFromSetting == true) {
        nResult = (APP_ENTRY_POINT_T)AppMediaPlayer_SettingsStorage::GetInstance()->GetLastAVEntryPoint(); // modified by Sergey 19.05.2013
    }
    else {
        nResult = m_nLastAVEntryPoint;
    }
    MP_HIGH << bFromSetting << nResult << LOG_ENDL;
    return nResult;
}


void AppMediaPlayer_EngineMain::SetAVPEntryPoint(APP_ENTRY_POINT_T nEntryPoint, DISPLAY_T display)
{
    MP_LOG << nEntryPoint << display << m_LastFrontEntryPoint << LOG_ENDL;

#ifdef DUAL_KEY
    // modified by sangmin.seol 2014.01.20 ITS_0220884 Skip KeyCancel if new EntryPoint is same with LastEntryPoint
    if(!((nEntryPoint>=eAPP_AVP_VIDEO_MODE_DISC_DRS&&nEntryPoint<=eAPP_APP_PHOTO_MODE_USB2_DRS)
            ||(nEntryPoint==eAPP_AVP_PHOTO_MODE_JUKEBOX
               ||nEntryPoint==eAPP_AVP_PHOTO_MODE_USB1
               ||nEntryPoint==eAPP_AVP_PHOTO_MODE_USB2))) // for suilyou ITS 0228847
    {
        if(nEntryPoint==eAPP_NONE
                ||(display == DISPLAY_FRONT && m_LastFrontEntryPoint != nEntryPoint)
                || (display == DISPLAY_REAR && m_LastRearEntryPoint != nEntryPoint))
            cancelAllPressedKey(0);
    }
#endif
	// { modified by edo.lee 2013.08.20
    if(display == DISPLAY_FRONT) {
        m_nFrontAVPEntryPoint = nEntryPoint;
        if(nEntryPoint != 0)
	       m_LastFrontEntryPoint = nEntryPoint;
    }
    else{
        m_nRearAVPEntryPoint = nEntryPoint;
        if(nEntryPoint != 0)
           m_LastRearEntryPoint = nEntryPoint;
    }
	// } modified by edo.lee 2013.08.20
}

APP_ENTRY_POINT_T AppMediaPlayer_EngineMain::GetAVPEntryPoint(DISPLAY_T display)
{
    APP_ENTRY_POINT_T nResult;
    
    if(display == DISPLAY_FRONT) 
        nResult = m_nFrontAVPEntryPoint;
    else
        nResult = m_nRearAVPEntryPoint;

    MP_LOG << display << nResult << LOG_ENDL;

    return nResult;
}

APP_MEDIA_MODE_T AppMediaPlayer_EngineMain::GetAVMediaMode(APP_ENTRY_POINT_T nEntryPnt, DISPLAY_T display)
{
    APP_MEDIA_MODE_T result = APP_MEDIA_MODE_MAX;

    switch(nEntryPnt)
    {
        //{added by junam 2013.12.19 for LIST_ENTRY_POINT
        case eAPP_FILEMANAGER_JUKEBOX_AUDIO:
        case eAPP_FILEMANAGER_USB1_AUDIO:
        case eAPP_FILEMANAGER_USB2_AUDIO:
        case eAPP_FILEMANAGER_IPOD1:
        case eAPP_FILEMANAGER_IPOD2:
        //}added by junam
        case eAPP_FILEMANAGER_DISC_AUDIO: // added by oseong.kwon 2014.02.10 for ITS 223602

        case eAPP_MODE_DYNAMICS_DISC: // added by wonseok.heo for Disc IN OUT TEST 2013.07.20
        case eAPP_AVP_AUDIO_MODE_JUKEBOX:
        case eAPP_AVP_AUDIO_MODE_USB1:
        case eAPP_AVP_AUDIO_MODE_USB2:
        case eAPP_AVP_AUDIO_MODE_IPOD1:
        case eAPP_AVP_AUDIO_MODE_IPOD2:
        case eAPP_AVP_AUDIO_MODE_DISC:
        case eAPP_AVP_AUDIO_MODE_AUX:
        case eAPP_AVP_AUDIO_MODE_BT:
        case eAPP_AVP_MODE_BLUETOOTH:
        {
            result = APP_MEDIA_MODE_AUDIO;
            break;
        }

        case eAPP_AVP_VIDEO_MODE_JUKEBOX:
        case eAPP_AVP_VIDEO_MODE_USB1:
        case eAPP_AVP_VIDEO_MODE_USB2:
        case eAPP_AVP_VIDEO_MODE_DISC:
        case eAPP_AVP_VIDEO_MODE_AUX:
        case eAPP_AVP_VIDEO_MODE_DISC_DRS:
        case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
        case eAPP_AVP_VIDEO_MODE_USB1_DRS:
        case eAPP_AVP_VIDEO_MODE_USB2_DRS:
        case eAPP_AVP_VIDEO_MODE_AUX_DRS:
        {
            result = APP_MEDIA_MODE_VIDEO;
            break;
        }

        case eAPP_AVP_PHOTO_MODE_JUKEBOX:
        case eAPP_AVP_PHOTO_MODE_USB1:
        case eAPP_AVP_PHOTO_MODE_USB2:            
        case eAPP_AVP_PHOTO_MODE_JUKEBOX_DRS:
        case eAPP_AVP_PHOTO_MODE_USB1_DRS:
        case eAPP_APP_PHOTO_MODE_USB2_DRS:
        {
            if(display == DISPLAY_FRONT) 
                result = APP_MEDIA_MODE_PHOTO_FRONT;
            else
                result = APP_MEDIA_MODE_PHOTO_REAR;
            break;
        }
        default:
        {
            result = APP_MEDIA_MODE_MAX;
            break;
        }
    }

    MP_HIGH << nEntryPnt << result << LOG_ENDL;

    return result;
}
// } modified by kihyung 2013.07.20 for ITS 0180419

void AppMediaPlayer_EngineMain::SendJukeboxEnableAtBoot()
{
    MEDIA_SOURCE_T source = MEDIA_SOURCE_JUKEBOX;
    if(m_pIsSendEnableMedia2UISH[source] == false && m_bJukeboxTAScanComplete == true) 
    {
        MP_HIGH << LOG_ENDL;
        m_pIsSendEnableMedia2UISH[source] = true;
        m_pNotifier->setEnableMedia(source, MEDIA_TYPE_MAX);
    }
}

void AppMediaPlayer_EngineMain::SendIPODEnableAtBoot()
{
    MEDIA_SOURCE_T source = MEDIA_SOURCE_IPOD1;            

    if(m_pIsSendEnableMedia2UISH[source] == false && m_pMediaManager->GetMountPath(source).isEmpty() == false) 
    {
        if(m_pIsSendIPODMounted2UISH[0] == false) 
        {
            MP_HIGH << "SEND IPOD1 MOUNT" << LOG_ENDL;
            
            APP_EVENT_IPOD_DATA_T stMessage;
            QString               sMountPath = m_pMediaManager->GetMountPath(source);
            QString               sUUID      = m_pMediaManager->GetID(source);

        	stMessage.header.reasonEvent    = EVT_IPOD_MOUNTED;
        	stMessage.header.dataSize       = sizeof( stMessage.mountPath );
        	stMessage.header.sIsJogEnabled  = false;
        	stMessage.header.appIdReciever  = APP_UI_MANAGER;

        	strncpy( stMessage.SerialNum, sUUID.toLocal8Bit().data(),      PATH_MAX_LENGTH );
        	strncpy( stMessage.mountPath, sMountPath.toLocal8Bit().data(), PATH_MAX_LENGTH );

        	stMessage.contentmask           = USB_DATA_CONTENT_NO;
        	stMessage.usbPort               = eUSB_PORT1;
            
            Event tEvent(EVT_IPOD_MOUNTED, GetThisAppId(), APP_UI_MANAGER, QByteArray((char*)&stMessage, sizeof(stMessage)));
            emit NotifyUISHCommon(tEvent);

            m_pIsSendIPODMounted2UISH[0] = true;
        }
        
        MP_HIGH << "SEND IPOD1 MEDIA ENABLE" << LOG_ENDL;
        
        m_pIsSendEnableMedia2UISH[source] = true;
        m_pNotifier->setEnableMedia(source, MEDIA_TYPE_MAX);
    }
    else if(m_pIsSendIPODRemoved2UISH[0] != 1 && GetControllers().GetIPod1Controller()->GetDeviceState() == 0)
    {
        APP_EVENT_IPOD_DATA_T 	stMessage;

        memset(&stMessage, 0x00, sizeof(stMessage));

        stMessage.header.reasonEvent    = EVT_IPOD_REMOVED;
        stMessage.header.dataSize       = sizeof( stMessage.mountPath );
        stMessage.header.sIsJogEnabled  = false;
        stMessage.header.appIdReciever  = APP_UI_MANAGER;
        stMessage.contentmask           = USB_DATA_CONTENT_NO;
        stMessage.usbPort               = eUSB_PORT1;
    
        Event tEvent(EVT_IPOD_REMOVED, GetThisAppId(), APP_UI_MANAGER, QByteArray((char *)&stMessage, sizeof(stMessage)));
        emit NotifyUISHCommon(tEvent);

        m_pIsSendIPODRemoved2UISH[0] = 1;
    }

    source = MEDIA_SOURCE_IPOD2;
    if(m_pIsSendEnableMedia2UISH[source] == false && m_pMediaManager->GetMountPath(source).isEmpty() == false) 
    {
        if(m_pIsSendIPODMounted2UISH[1] == false) 
        {
            MP_HIGH << "SEND IPOD2 MOUNT" << LOG_ENDL;
            
            APP_EVENT_IPOD_DATA_T stMessage;
            QString               sMountPath = m_pMediaManager->GetMountPath(source);
            QString               sUUID      = m_pMediaManager->GetID(source);

        	stMessage.header.reasonEvent    = EVT_IPOD_MOUNTED;
        	stMessage.header.dataSize       = sizeof( stMessage.mountPath );
        	stMessage.header.sIsJogEnabled  = false;
        	stMessage.header.appIdReciever  = APP_UI_MANAGER;

        	strncpy( stMessage.SerialNum, sUUID.toLocal8Bit().data(),      PATH_MAX_LENGTH );
        	strncpy( stMessage.mountPath, sMountPath.toLocal8Bit().data(), PATH_MAX_LENGTH );

        	stMessage.contentmask           = USB_DATA_CONTENT_NO;
        	stMessage.usbPort               = eUSB_PORT2;
            
            Event tEvent(EVT_IPOD_MOUNTED, GetThisAppId(), APP_UI_MANAGER, QByteArray((char*)&stMessage, sizeof(stMessage)));
            emit NotifyUISHCommon(tEvent);

            m_pIsSendIPODMounted2UISH[1] = true;
        }
        
        MP_HIGH << "SEND IPOD2 MEDIA ENABLE" << LOG_ENDL;
        
        m_pIsSendEnableMedia2UISH[source] = true;
        m_pNotifier->setEnableMedia(source, MEDIA_TYPE_MAX);
    }
    else if(m_pIsSendIPODRemoved2UISH[1] != 1 && GetControllers().GetIPod2Controller()->GetDeviceState() == 0)
    {
        APP_EVENT_IPOD_DATA_T 	stMessage;

        memset(&stMessage, 0x00, sizeof(stMessage));

        stMessage.header.reasonEvent    = EVT_IPOD_REMOVED;
        stMessage.header.dataSize       = sizeof( stMessage.mountPath );
        stMessage.header.sIsJogEnabled  = false;
        stMessage.header.appIdReciever  = APP_UI_MANAGER;
        stMessage.contentmask           = USB_DATA_CONTENT_NO;
        stMessage.usbPort               = eUSB_PORT2;
    
        Event tEvent(EVT_IPOD_REMOVED, GetThisAppId(), APP_UI_MANAGER, QByteArray((char *)&stMessage, sizeof(stMessage)));
        emit NotifyUISHCommon(tEvent);

        m_pIsSendIPODRemoved2UISH[1] = 1;
    }
}

void AppMediaPlayer_EngineMain::SendUSBEnableAtBoot()
{
    MEDIA_SOURCE_T source = MEDIA_SOURCE_USB1;
    if(m_pIsSendEnableMedia2UISH[source] == false && m_bUSB1TAReadComplete == true) 
    {
        MP_HIGH << "USB1" << LOG_ENDL;
        m_pIsSendEnableMedia2UISH[source] = true;
        m_pNotifier->setEnableMedia(source, MEDIA_TYPE_MAX);
    }

    source = MEDIA_SOURCE_USB2;
    if(m_pIsSendEnableMedia2UISH[source] == false && m_bUSB2TAReadComplete == true) 
    {
        MP_HIGH << "USB2" << LOG_ENDL;
        m_pIsSendEnableMedia2UISH[source] = true;
        m_pNotifier->setEnableMedia(source, MEDIA_TYPE_MAX);
    }
}

// modified by Dmitry 11.05.13
void AppMediaPlayer_EngineMain::setMiddleEast(bool east)
{
   if (m_bMiddleEast != east)
   {
      m_bMiddleEast = east;
      emit middleEastChanged();
   }
}
// modified by Dmitry 11.05.13

//{changed by junam 2013.06.06 for factory test additional fix
void AppMediaPlayer_EngineMain::notifyCurrentTrackName(QString trackName)
{
    MP_LOG << "trackName "<<trackName << LOG_ENDL;

    EVT_COMMON_MPAUTOTEST_EVENT_DATA_T  tComEvtData;
    memset(&tComEvtData,0,sizeof(tComEvtData));
    tComEvtData.tHeader.appIdReciever = APP_MPAUTOTEST;
    tComEvtData.tHeader.dataSize = sizeof( tComEvtData );
    tComEvtData.tHeader.reasonEvent = EVT_MP_IPOD_TRACK_NAME_RES;

    if(trackName.isNull()) 
    {
        tComEvtData.tPayload.iLength = 1;
        tComEvtData.tPayload.data[0] = 0;
    }
    else
    {
        tComEvtData.tPayload.iLength = qMin( trackName.length(), COMMON_MAX_MPAUTOTEST_PAYLOAD_DATA_SIZE);
        memcpy(tComEvtData.tPayload.data, trackName.toLatin1().constData(), tComEvtData.tPayload.iLength);
    }

    Event tEvent(EVT_POST_EVENT, GetThisAppId(),APP_MPAUTOTEST, EVT_MP_IPOD_TRACK_NAME_RES, QByteArray( (char*)&tComEvtData, sizeof( tComEvtData)));
    emit NotifyUISHCommon(tEvent);
}
//}changed by junam

//{ added by wonseok.heo 2013.06.12 for factory test
void AppMediaPlayer_EngineMain::notifyDiscCurrentTrackName(Event aEvent)
{
    AppMediaPlayer_EngineAudio      *pEngineAudio = (AppMediaPlayer_EngineAudio*)GetEngine(APP_MEDIA_MODE_AUDIO);
    AppMediaPlayer_Controller_Audio *pController  = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();

    EVT_COMMON_MPAUTOTEST_EVENT_DATA_T *tComEvtData = (EVT_COMMON_MPAUTOTEST_EVENT_DATA_T*)aEvent.Data().data();

    COMMON_DATA_MPAUTOTEST_PAYLOAD_T receive_Payload;

    memset(&receive_Payload, 0x00, sizeof(COMMON_DATA_MPAUTOTEST_PAYLOAD_T));

    memcpy(&receive_Payload, &(tComEvtData->tPayload), sizeof(COMMON_DATA_MPAUTOTEST_PAYLOAD_T));

    char str[COMMON_MAX_MPAUTOTEST_PAYLOAD_DATA_SIZE+1] = { 0, };
    memcpy(str, receive_Payload.data, receive_Payload.iLength + 1);

    QString trackNumber = QString(str);

    int trackNum = trackNumber.toInt();


//    EVT_COMMON_MPAUTOTEST_EVENT_DATA_T  tComEvtData;
//    memset(&tComEvtData,0,sizeof(tComEvtData));
//    tComEvtData.tHeader.appIdReciever = APP_MPAUTOTEST;
//    tComEvtData.tHeader.dataSize = sizeof( tComEvtData );
//    QString str = tComEvtData.tPayload.data;
//    int trackNum = str.toInt();

    //setTrackChange
    pController->setTrackChange(trackNum);

}
//}added by wonseok.heo 2013.06.12 for factory test

// { added by wonseok.heo 2013.07.04 disc in out test
void AppMediaPlayer_EngineMain::InOutTestEject(){
    emit scrollAccStop();
    m_isDiscEject = true;
    m_pMediaManager->ejectDisc(false);
}

bool AppMediaPlayer_EngineMain::getInOutTestMode(){

//    MP_LOG << "m_DiscInOutTestMode " <<QString::number(m_DiscInOutTestMode) << LOG_ENDL;
//    MP_LOG << "m_DiscInOutTestMode = %d", m_DiscInOutTestMode) << LOG_ENDL;
    return m_DiscInOutTestMode;
}

void AppMediaPlayer_EngineMain::setInOutTestMode(bool isInMode){

    MP_LOG << LOG_ENDL;
    m_DiscInOutTestMode = isInMode;
}

void AppMediaPlayer_EngineMain::sendDiscStatusChk(int data){

    MP_LOG << "Value : " << data << LOG_ENDL;
    emit discStatusData(data);
}

void AppMediaPlayer_EngineMain::InOutTestLoad(){ // added by wonseok.heo DISC IN OUT TEST 

    m_pMediaManager->loadDisc();
}

// } added by wonseok.heo 2013.07.04 disc in out test

QString AppMediaPlayer_EngineMain::GetSWVersion()
{
    if(m_strSWVersion.isEmpty() == true) 
    {
        QFile inFile("/etc/version.txt");
        if(inFile.open(QIODevice::ReadOnly) == false) {
            MP_LOG << "file not open" << LOG_ENDL;
            return false;
        }

        QTextStream inFileStream(&inFile);
        QString     strVersion;
        
        inFileStream.skipWhiteSpace();
        inFileStream >> strVersion;

        inFile.close();

        if(strVersion.isEmpty() == true) {
            m_strSWVersion = "unknown";
        }
        else {
            m_strSWVersion = strVersion;
        }
    }

    MP_HIGH << m_strSWVersion << LOG_ENDL;
    
    return m_strSWVersion;
}
//{added by junam 2013.07.18 for ITS_NA_180278
bool AppMediaPlayer_EngineMain::GetVariantRearUSB()
{
     return ( AppEngineBase::GetSystemVariant() & SYSTEM_VARIANT_REAR_USB );
}
//}added by junam

// modified by ravikanth 27-07-13 for ITS 181678
void AppMediaPlayer_EngineMain::SetPhotoIndexOnOtherEngine(PHOTO_SOURCES_LIST source, int index, bool isFront)
{
    AppMediaPlayer_EnginePhoto *pPhotoengine = NULL;
    if(!isFront)
    {
        pPhotoengine =  dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_FRONT]);
    }
    else
    {
        pPhotoengine =  dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_REAR]);
    }
    if(pPhotoengine != NULL)
    {
        pPhotoengine->SetPhotoIndex(source, index);
    }
}

void AppMediaPlayer_EngineMain::setSlideShowDelay(int delay)
{
    AppMediaPlayer_EnginePhoto *pFrontPhotoengine = NULL;
    pFrontPhotoengine =  dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_FRONT]);
    //MP_LOG << "Fr" << (pFrontPhotoengine == NULL) << LOG_ENDL;
    if(pFrontPhotoengine != NULL)
        pFrontPhotoengine->setCurrentDisplaySlideShowDelay(delay);

    AppMediaPlayer_EnginePhoto *pRearPhotoengine = NULL;
    pRearPhotoengine =  dynamic_cast<AppMediaPlayer_EnginePhoto*>(m_Engines[APP_MEDIA_MODE_PHOTO_REAR]);
    //MP_LOG << "Re" << (pRearPhotoengine == NULL) << LOG_ENDL;
    if(pRearPhotoengine != NULL)
            pRearPhotoengine->setCurrentDisplaySlideShowDelay(delay);
}
bool AppMediaPlayer_EngineMain::checkIsSameEntryPoint(DISPLAY_T display,APP_ENTRY_POINT_T entry )
{
	MP_LOG<<m_LastFrontEntryPoint<<m_LastRearEntryPoint << LOG_ENDL;
    //added by shkim for ITS 185028
    if(getSubCreenOn())
    {
        MP_LOG << "SubScreen is On : " <<getSubCreenOn()  << LOG_ENDL;
        return true;
    }
    //added by shkim for ITS 185028
    if(m_LastFrontEntryPoint != eAPP_NONE && display == DISPLAY_FRONT)
        return  m_LastFrontEntryPoint == entry ;
    else if (m_LastRearEntryPoint != eAPP_NONE && display == DISPLAY_REAR)
        return m_LastRearEntryPoint == entry;

    return false;
}

// modified by ravikanth for ITS 0186965
bool AppMediaPlayer_EngineMain::isCopyInProgress()
{
    return ( m_jukeboxCopyInProgress ||
            ( m_pSharedFMData->GetStatusDateByIndex(DATA_TYPE_FILEOPERATION_STATUS) == FILEOPERATION_STATUS_COPY_PROGRESS ) );
}



//added by suilyou 20130925 START
void AppMediaPlayer_EngineMain::setSharedMemData(int data)
{

    m_pSharedFMData->SetStatusDateByIndex(DATA_TYPE_JOG_KEY_STATUS, data);

}
int AppMediaPlayer_EngineMain::getSharedMemData()
{
    MP_LOG << "getSharedMemData = " << m_pSharedFMData->GetStatusDateByIndex(DATA_TYPE_JOG_KEY_STATUS)  << LOG_ENDL;
    return m_pSharedFMData->GetStatusDateByIndex(DATA_TYPE_JOG_KEY_STATUS);
}
//added by suilyou 20130925 END


// { added by Sergey 2013.09.26 for ITS#191449
void AppMediaPlayer_EngineMain::unblockUI()
{
    QEventLoop loop;
    loop.processEvents(QEventLoop::ExcludeUserInputEvents, 100);
}
// } added by Sergey 2013.09.26 for ITS#191449


// { added by Sergey 18.10.2013 for safe ui update
void AppMediaPlayer_EngineMain::repaintUI(DISPLAY_T display)
{
    QRect rect = QRect(0, 0, 1280, 720);

    if(display == DISPLAY_NONE)
    {
        GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN)->repaint(rect);
        GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN)->repaint(rect);
    }
    else
    {
        AppEngineQMLConstants::EAPP_SCREEN_USAGE_T screen = (display == DISPLAY_FRONT) ? AppEngineQMLConstants::APP_FRONT_SCREEN : AppEngineQMLConstants::APP_REAR_SCREEN;
        GetDeclarativeView(screen)->repaint(rect);
    }
}
// } added by Sergey 18.10.2013 for safe ui update


//added by suilyou 20130930 for 0186169
void AppMediaPlayer_EngineMain::sendTouchCleanUpForApps()
{
    MP_LOG << " sendTouchCleanUpForApps()" << LOG_ENDL;
    SendTouchCleanUpForApps ();
}

//added by suilyou 20131002 ITS 0192624 START
void AppMediaPlayer_EngineMain::clearCCPJogKey()
{
    setJogKeyReleased(CCP_JOG_LEFT_PRESSED|CCP_JOG_CENTER_PRESSED|CCP_JOG_RIGHT_PRESSED);
    MP_LOG << "clearCCPJogKey() key status" << getJogKeyPressed() << LOG_ENDL;
}
void AppMediaPlayer_EngineMain::clearRRCJogKey()
{
    setJogKeyReleased(RRC_JOG_LEFT_PRESSED|RRC_JOG_CENTER_PRESSED|RRC_JOG_RIGHT_PRESSED);
    MP_LOG << "clearCCPJogKey() key status" << getJogKeyPressed() << LOG_ENDL;
}

//added by suilyou 20131002 ITS 0192624 END
void
AppMediaPlayer_EngineMain::NofityAudioPathChanged (int mode)
{
//     MP_LOG << "NofityAudioPathChanged" << mode << LOG_ENDL;
//        EVENT_AUDIO_PATH_REQUEST_DATA message = { { APP_ALL, EVT_REQUEST_AUDIO_PATH, 0, 0 }, mode };
//        message.header.dataSize = sizeof( message );
//        Event event( EVT_REQUEST_AUDIO_PATH, APP_AVP,
//                         APP_UI_MANAGER, QByteArray( (char*)&message, sizeof( message ) ) ); // modified by lssanh 2013.02.21 IPC payload
//        NotifyUISHCommon(event);
        {
            MODE_STATE_T avState = MODE_STATE_NONE;
            MP_LOG <<  "NofityAudioPathChanged Send Sound path:device=" << mode << LOG_ENDL;//Added by Alexey Edelev 2012.10.19 CR14799
            switch (mode)
            {
            case MP::JUKEBOX:
                avState = MODE_STATE_JUKEBOX_AUDIO;
                break;

            case MP::USB1:
                avState = MODE_STATE_USB1_AUDIO;
                break;

            case MP::USB2:
                avState = MODE_STATE_USB2_AUDIO;
                break;

        //    case MP::DISC:
        //        avState = MODE_STATE_DISC_AUDIO;
        //        break;

            case MP::IPOD1:
                avState = MODE_STATE_IPOD1_AUDIO;
                break;

            case MP::IPOD2:
                avState = MODE_STATE_IPOD2_AUDIO;
                break;

        //    case MP::AUX:
        //        avState = MODE_STATE_AUX1_AUDIO;
        //        break;
                //added by edo.lee 2012.09.04 for cr 6401
        //    case MP::BLUETOOTH:
        //        avState = MODE_STATE_BT_AUDIO;
        //        break;
                //added by edo.lee

            default:
                MP_LOG << "Warning! Unknown id" << LOG_ENDL;
                break;
            }


            EVENT_AUDIO_PATH_REQUEST_DATA message = { { APP_ALL, EVT_REQUEST_AUDIO_PATH, 0, 0 }, avState };

            message.header.dataSize = sizeof( message );
            Event event( EVT_REQUEST_AUDIO_PATH, GetThisAppId(),
                         APP_UI_MANAGER, QByteArray( (char*)&message, sizeof( message ) ) ); // modified by lssanh 2013.02.21 IPC payload

            NotifyUISHCommon( event );
        }
}
MODE_STATE_T  AppMediaPlayer_EngineMain::mapToMode(APP_ENTRY_POINT_T ep)
{
    MODE_STATE_T result = MODE_STATE_MAX;

    switch(ep)
    {
        case eAPP_AVP_AUDIO_MODE_JUKEBOX:
            result = MODE_STATE_JUKEBOX_AUDIO;
            break;
        case eAPP_AVP_AUDIO_MODE_USB1:
            result = MODE_STATE_USB1_AUDIO;
            break;
        case eAPP_AVP_AUDIO_MODE_USB2:
            result = MODE_STATE_USB2_AUDIO;
            break;
        case eAPP_AVP_AUDIO_MODE_DISC:
            result = MODE_STATE_DISC_AUDIO;
            break;
        case eAPP_AVP_AUDIO_MODE_IPOD1:
            result = MODE_STATE_IPOD1_AUDIO;
            break;
        case eAPP_AVP_AUDIO_MODE_IPOD2:
            result = MODE_STATE_IPOD2_AUDIO;
            break;
        case eAPP_AVP_AUDIO_MODE_AUX:
            result = MODE_STATE_AUX1_AUDIO;
            break;
        case eAPP_AVP_MODE_BLUETOOTH:
            result = MODE_STATE_BT_AUDIO;
            break;

        case eAPP_AVP_VIDEO_MODE_JUKEBOX:
            result = MODE_STATE_JUKEBOX_VIDEO;
            break;
        case eAPP_AVP_VIDEO_MODE_USB1:
            result =MODE_STATE_USB1_VIDEO ;
            break;
        case eAPP_AVP_VIDEO_MODE_USB2:
            result =MODE_STATE_USB2_VIDEO ;
            break;
        case eAPP_AVP_VIDEO_MODE_DISC:
            result = MODE_STATE_DISC_VIDEO;
            break;
        case eAPP_AVP_VIDEO_MODE_AUX:
            result = MODE_STATE_AUX1_VIDEO;
            break;
        default:
            MP_LOG << "Unsupported mode state:" << ep << LOG_ENDL;
            break;
    }
    return result;
}

DISPLAY_T AppMediaPlayer_EngineMain::getFrontScreenView()
{
    DISPLAY_T result = DISPLAY_NONE;

    CLONE_STATUS_T bClone = getCloneState();

    if(bClone == CLONE_OFF)
    {
        if(m_bSwapEnabled)
            result = DISPLAY_REAR;
        else
            result = DISPLAY_FRONT;
    }
    else if(bClone == CLONE_ON_FRONT)
        result = DISPLAY_FRONT;
    else
        result = DISPLAY_REAR;

    MP_LOG << "MMM result =" << result << LOG_ENDL;
    return result;
}

#ifdef DUAL_KEY
    void AppMediaPlayer_EngineMain::cancelAllPressedKey(int mode)
    {
        //MP_LOG << "DUAL_KEY cancelAllPressedKey() mode = " << mode << LOG_ENDL; // removed by sangmin.seol 2014.06.27 reduce log for performance
        KEY_EVENT_DATA_T eventData;
        // mode = 0 // all clear CCP+RRC+ETC
        // mode =1  // clear only CCP
        // mode = 2 // clear only RRC
        // mode =3 // clear only ETC
        eventData.header.appIdReciever = APP_ALL;
        eventData.header.dataSize = sizeof( KEY_EVENT_DATA_T );
        eventData.keyStatus = KEY_STATUS_CANCEL;
        //cancel CCPf
        //MP_LOG << "DUAL_KEY getCCPStatus = " << getCCPStatus() << LOG_ENDL;  // removed by sangmin.seol 2014.06.27 reduce log for performance
        if(getCCPStatus()!=0 && (mode == 0 || mode ==1))
        {
            Event aEvent(getEVTfromCCPStatus(getCCPStatus()), GetThisAppId(), GetThisAppId(), QByteArray( (char *)&eventData, sizeof( eventData ))/**/ );
            MP_LOG << "DUAL_KEY CCP key = " << getEVTfromCCPStatus(getCCPStatus()) << "keystatue = " << eventData.keyStatus << LOG_ENDL;
            NotifyFromUISHCommon(aEvent);
        }
        //MP_LOG << "DUAL_KEY getRRCStatus = " << getRRCStatus() << LOG_ENDL;  // removed by sangmin.seol 2014.06.27 reduce log for performance
        if(getRRCStatus()!=0 && (mode == 0 || mode ==2))
        {
           Event aEvent(getEVTfromRRCStatus(getRRCStatus()), GetThisAppId(), GetThisAppId(), QByteArray( (char *)&eventData, sizeof( eventData ))/*, sizeof( eventData )*/ );
            MP_LOG << "DUAL_KEY key = " << getEVTfromRRCStatus(getRRCStatus()) << "keystatue = " << eventData.keyStatus << LOG_ENDL;
            NotifyFromUISHCommon(aEvent);
        }
        //MP_LOG << "DUAL_KEY ETC getETCStatus = " << getETCStatus() << LOG_ENDL; // removed by sangmin.seol 2014.06.27 reduce log for performance
        if(getETCStatus()!=0&& (mode == 0 || mode ==3))
        {
            Event aEvent(getEVTfromETCStatus(getETCStatus()), GetThisAppId(), GetThisAppId(), QByteArray( (char *)&eventData, sizeof( eventData ))/*, sizeof( eventData )*/ );
            MP_LOG << "DUAL_KEY ETC key = " << getEVTfromETCStatus(getETCStatus()) << "keystatue = " << eventData.keyStatus << LOG_ENDL;
           NotifyFromUISHCommon(aEvent);
        }
    }
EVT_ID_T AppMediaPlayer_EngineMain::getEVTfromCCPStatus(unsigned int key)
{
    int ret=EVT_MAX;
    switch(key)
    {
        case CCP_BACK:
        {
            ret = EVT_KEY_CCP_BACK;
            break;
        }
        case CCP_CENTER:
        {
            ret = EVT_KEY_CCP_JOG_DIAL_CENTER;
            break;
        }
        case CCP_DOWN:
        {
            ret = EVT_KEY_CCP_JOG_DIAL_DOWN;
            break;
        }
        case CCP_UP:
        {
            ret = EVT_KEY_CCP_JOG_DIAL_UP;
            break;
        }
        case CCP_LEFT:
        {
            ret = EVT_KEY_CCP_JOG_DIAL_LEFT;
            break;
        }
        case CCP_RIGHT:
        {
            ret = EVT_KEY_CCP_JOG_DIAL_RIGHT;
            break;
        }
        case CCP_MENU:
        {
            ret = EVT_KEY_CCP_MENU;
            break;
        }
    default:
        //MP_LOG << "Unsupported CCP KEY" << LOG_ENDL;   // removed by sangmin.seol 2014.06.27 reduce log for performance
        break;
    }
    return ret;
}
EVT_ID_T AppMediaPlayer_EngineMain::getEVTfromRRCStatus(unsigned int key)
{
    int ret=EVT_MAX;
    switch(key)
    {
        case RRC_BACK:
        {
            ret = EVT_KEY_RRC_BACK;
            break;
        }
        case RRC_CENTER:
        {
            ret = EVT_KEY_RRC_JOG_DIAL_CENTER;
            break;
        }
        case RRC_DOWN:
        {
            ret = EVT_KEY_RRC_JOG_DIAL_DOWN;
            break;
        }
        case RRC_UP:
        {
            ret = EVT_KEY_RRC_JOG_DIAL_UP;
            break;
        }
        case RRC_LEFT:
        {
            ret = EVT_KEY_RRC_JOG_DIAL_LEFT;
            break;
        }
        case RRC_RIGHT:
        {
            ret = EVT_KEY_RRC_JOG_DIAL_RIGHT;
            break;
        }
        case RRC_MENU:
        {
            ret = EVT_KEY_RRC_MENU;
            break;
        }

        default:
            //MP_LOG << "Unsupported RRC KEY" << LOG_ENDL; // removed by sangmin.seol 2014.06.27 reduce log for performance
            break;
    }
    return ret;
}
EVT_ID_T AppMediaPlayer_EngineMain::getEVTfromETCStatus(unsigned int key)
{
    int ret=EVT_MAX;
    switch(key)
    {
        case ETC_FACE_REW:
        {
            ret = EVT_KEY_HU_SEEK_PREV;
            break;
        }
        case ETC_FACE_FF:
        {
            ret = EVT_KEY_HU_SEEK_NEXT;
            break;
        }
        case ETC_FACE_TUNE_CENTER:
        {
            ret = EVT_KEY_HU_TUNE_PRESSED;
            break;
        }
        case ETC_SWRC_REW:
        {
            ret = EVT_KEY_SWRC_SEEK_PREV;
            break;
        }
        case ETC_SWRC_FF:
        {
            ret = EVT_KEY_SWRC_SEEK_NEXT;
            break;
        }
        // { Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
        case ETC_CCP_SEEK_REW:
        {
            ret = EVT_KEY_CCP_SEEK_PREV;
            break;
        }
        case ETC_CCP_SEEK_FF:
        {
            ret = EVT_KEY_CCP_SEEK_NEXT;
            break;
        }
        // } Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
        // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
        //case ETC_RRC_CH_BK:
        //{
        //    ret = EVT_KEY_RRC_CH_BK;
        //    break;
        //}
        //case ETC_RRC_CH_FW:
        //{
        //    ret = EVT_KEY_RRC_CH_FW;
        //    break;
        //}
        // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
        case ETC_RRC_SEEK_REW:
        {
            ret = EVT_KEY_RRC_SEEK_PREV;
            break;
        }
        case ETC_RRC_SEEK_FF:
        {
            ret = EVT_KEY_RRC_SEEK_NEXT;
            break;
        }
        default:
            //MP_LOG << "Unsupported ETC KEY" << LOG_ENDL; // removed by sangmin.seol 2014.06.27 reduce log for performance
            break;
    }
    return ret;
}
bool AppMediaPlayer_EngineMain::checkValidKey(EVT_ID_T key)
{
    if(key == EVT_MAX )
        return false;
    switch(key)
    {
        // CCP START
        case EVT_KEY_CCP_BACK:
            return getCCPStatus()&CCP_BACK;
        case EVT_KEY_CCP_JOG_DIAL_CENTER:
            return getCCPStatus()&CCP_CENTER;
        case EVT_KEY_CCP_JOG_DIAL_DOWN:
            return getCCPStatus()&CCP_DOWN;
        case EVT_KEY_CCP_JOG_DIAL_UP:
                return getCCPStatus()&CCP_UP;
        case EVT_KEY_CCP_JOG_DIAL_LEFT:
                return getCCPStatus()&CCP_LEFT;
        case EVT_KEY_CCP_JOG_DIAL_RIGHT:
                return getCCPStatus()&CCP_RIGHT;
        case EVT_KEY_CCP_MENU:
                return getCCPStatus()&CCP_MENU;
        // CCP END RRC START
        case EVT_KEY_RRC_BACK:
            return getRRCStatus()&RRC_BACK;
        case EVT_KEY_RRC_JOG_DIAL_CENTER:
            return getRRCStatus()&RRC_CENTER;
        case EVT_KEY_RRC_JOG_DIAL_DOWN:
            return getRRCStatus()&RRC_DOWN;
        case EVT_KEY_RRC_JOG_DIAL_UP:
            return getRRCStatus()&RRC_UP;
        case EVT_KEY_RRC_JOG_DIAL_LEFT:
            return getRRCStatus()&RRC_LEFT;
        case EVT_KEY_RRC_JOG_DIAL_RIGHT:
            return getRRCStatus()&RRC_RIGHT;
        case EVT_KEY_RRC_MENU:
            return getRRCStatus()&RRC_MENU;
        // { Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
        case EVT_KEY_CCP_SEEK_PREV:
            return getETCStatus()&ETC_CCP_SEEK_REW;
        case EVT_KEY_CCP_SEEK_NEXT:
            return getETCStatus()&ETC_CCP_SEEK_FF;
        // } Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
        // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
        //case EVT_KEY_RRC_CH_BK:
        //    return getETCStatus()&ETC_RRC_CH_BK;
        //case EVT_KEY_RRC_CH_FW:
        //    return getETCStatus()&ETC_RRC_CH_FW;
        // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
        case EVT_KEY_RRC_SEEK_PREV:
            return getETCStatus()&ETC_RRC_SEEK_REW;
        case EVT_KEY_RRC_SEEK_NEXT:
            return getETCStatus()&ETC_RRC_SEEK_FF;
        // RRC END ETC START
        case EVT_KEY_HU_SEEK_PREV:
            return getETCStatus()&ETC_FACE_REW;
        case EVT_KEY_HU_SEEK_NEXT:
            return getETCStatus()&ETC_FACE_FF;
        case EVT_KEY_HU_TUNE_PRESSED:
            return getETCStatus()&ETC_FACE_TUNE_CENTER;
        case EVT_KEY_SWRC_SEEK_PREV:
            return getETCStatus()&ETC_SWRC_REW;
        case EVT_KEY_SWRC_SEEK_NEXT:
            return getETCStatus()&ETC_SWRC_FF;
        //ETC END
    default:
        return false;
    }
}
uint AppMediaPlayer_EngineMain::transformEVT2QML_KEY(EVT_ID_T key)
{
    switch(key)
    {
        case EVT_KEY_CCP_JOG_DIAL_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_LEFT:
            return AppEngineQMLConstants::JOG_LEFT;
        case EVT_KEY_CCP_JOG_DIAL_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_RIGHT:
            return AppEngineQMLConstants::JOG_RIGHT;
        case EVT_KEY_CCP_JOG_DIAL_CENTER:
        case EVT_KEY_RRC_JOG_DIAL_CENTER:
            return AppEngineQMLConstants::JOG_CENTER;
        case EVT_KEY_CCP_JOG_DIAL_UP:
        case EVT_KEY_RRC_JOG_DIAL_UP:
            return AppEngineQMLConstants::JOG_UP;
        case EVT_KEY_CCP_JOG_DIAL_DOWN:
        case EVT_KEY_RRC_JOG_DIAL_DOWN:
            return AppEngineQMLConstants::JOG_DOWN;
        // { added by yungi 2014.02.14 for ITS 225174
        case EVT_KEY_CCP_JOG_DIAL_TOP_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_TOP_LEFT:
            return AppEngineQMLConstants::JOG_TOP_LEFT;
        case EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT:
            return AppEngineQMLConstants::JOG_BOTTOM_LEFT;
        case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT:
            return AppEngineQMLConstants::JOG_TOP_RIGHT;
        case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT:
            return AppEngineQMLConstants::JOG_BOTTOM_RIGHT;
        // } added by yungi 2014.02.14
    }
    return AppEngineQMLConstants::JOG_NONE;
}


//AppEngineQMLConstants::JOG_WHEEL_RIGHT,
//                                  AppEngineQMLConstants::KEY_STATUS_PRESSED);
bool AppMediaPlayer_EngineMain::eventFilter(QObject *pFromObj, QEvent *pEvent)
{
    if ( pEvent->type() == QEvent::MouseButtonPress )
    {
        if(IsSwapDCEnabled())
        {
            if(rearLCDMode == APP_MEDIA_MODE_AUDIO || rearLCDMode == APP_MEDIA_MODE_VIDEO)
            {
                MP_LOG << "DUAL_KEY MouseButtonPressed ==> cancelAppRessedKey() " << LOG_ENDL;
                MP_LOG << rearLCDMode << " is launched front screen" << LOG_ENDL;

                if(frontLCDMode == rearLCDMode)
                    cancelAllPressedKey(0);
                else
                {
                    cancelAllPressedKey(2);
                    cancelAllPressedKey(3);
                }
            }
        }
        else
        {
            if(frontLCDMode == APP_MEDIA_MODE_AUDIO || frontLCDMode == APP_MEDIA_MODE_VIDEO)
            {
                MP_LOG << "DUAL_KEY MouseButtonPressed ==> cancelAppRessedKey() " << LOG_ENDL;
                MP_LOG << frontLCDMode << " is launched front screen" << LOG_ENDL;

                if(frontLCDMode == rearLCDMode)
                    cancelAllPressedKey(0);
                else
                {
                    cancelAllPressedKey(1);
                    cancelAllPressedKey(3);
                }
            }
        }
    }
    return AppVextEngineQML::eventFilter( pFromObj, pEvent );

}
void AppMediaPlayer_EngineMain::cancelPressedKey(Event aEvent)
{
    KEY_EVENT_DATA_T *dd = (KEY_EVENT_DATA_T*)aEvent.Data().data();

    // removed by sangmin.seol 2014.06.27 reduce log for performance
    //MP_LOG << "DUAL_KEY KeyID = " << aEvent.GetEventId()  << " key status = " << dd->keyStatus << LOG_ENDL;
    //MP_LOG << "frontLCDMode = " << frontLCDMode  << " rearLCDMode = " << rearLCDMode << LOG_ENDL;

    switch(aEvent.GetEventId())
    {
        case EVT_KEY_HU_JOG_DIAL_ENCODER:
            if(IsNeedTouchCleanUp(aEvent.GetEventId()))
            {
                sendTouchCleanUpForApps();
            }

            if(frontLCDMode == APP_MEDIA_MODE_AUDIO || frontLCDMode == APP_MEDIA_MODE_VIDEO)
                cancelAllPressedKey(1);
            if(rearLCDMode == APP_MEDIA_MODE_AUDIO || rearLCDMode == APP_MEDIA_MODE_VIDEO)
                cancelAllPressedKey(2);

            cancelAllPressedKey(3);
            break;
        case EVT_KEY_CCP_JOG_DIAL_ENCODER:
            if(IsNeedTouchCleanUp(aEvent.GetEventId()))
            {
                sendTouchCleanUpForApps();
            }

            if(frontLCDMode == APP_MEDIA_MODE_AUDIO || frontLCDMode == APP_MEDIA_MODE_VIDEO)
            {
                if(frontLCDMode == rearLCDMode)
                    cancelAllPressedKey(0);
                else
                {
                    cancelAllPressedKey(1);
                    cancelAllPressedKey(3);
                }
            }
            break;
        case EVT_KEY_RRC_JOG_DIAL_ENCODER:
            if(IsNeedTouchCleanUp(aEvent.GetEventId()))
            {
                sendTouchCleanUpForApps();
            }

            if(rearLCDMode == APP_MEDIA_MODE_AUDIO || rearLCDMode == APP_MEDIA_MODE_VIDEO)
            {
                if(frontLCDMode == rearLCDMode)
                    cancelAllPressedKey(0);
                else
                {
                    cancelAllPressedKey(2);
                    cancelAllPressedKey(3);
                }
            }
            break;
    }

    switch(dd->keyStatus)
    {
        case KEY_STATUS_PRESSED:
        {
            switch(aEvent.GetEventId())
            {
            case EVT_KEY_CCP_JOG_DIAL_CENTER:
            case EVT_KEY_CCP_JOG_DIAL_UP:
            case EVT_KEY_CCP_JOG_DIAL_RIGHT:
            case EVT_KEY_CCP_JOG_DIAL_LEFT:
            case EVT_KEY_CCP_JOG_DIAL_DOWN:
            case EVT_KEY_CCP_MENU:
                if(IsNeedTouchCleanUp(aEvent.GetEventId()))
                {
                    sendTouchCleanUpForApps();
                }

                if(frontLCDMode == APP_MEDIA_MODE_AUDIO || frontLCDMode == APP_MEDIA_MODE_VIDEO)
                {
                    if(frontLCDMode == rearLCDMode)
                        cancelAllPressedKey(0);
                    else
                    {
                        cancelAllPressedKey(1);
                        cancelAllPressedKey(3);
                    }
                }
                break;
            case EVT_KEY_RRC_JOG_DIAL_CENTER:
            case EVT_KEY_RRC_JOG_DIAL_UP:
            case EVT_KEY_RRC_JOG_DIAL_RIGHT:
            case EVT_KEY_RRC_JOG_DIAL_LEFT:
            case EVT_KEY_RRC_JOG_DIAL_DOWN:
            case EVT_KEY_RRC_MENU:
                if(IsNeedTouchCleanUp(aEvent.GetEventId()))
                {
                    sendTouchCleanUpForApps();
                }

                if(rearLCDMode == APP_MEDIA_MODE_AUDIO || rearLCDMode == APP_MEDIA_MODE_VIDEO)
                {
                    if(frontLCDMode == rearLCDMode)
                        cancelAllPressedKey(0);
                    else
                    {
                        cancelAllPressedKey(2);
                        cancelAllPressedKey(3);
                    }
                }
                break;
            case EVT_KEY_SWRC_SEEK_NEXT:
            case EVT_KEY_SWRC_SEEK_PREV:
            case EVT_KEY_HU_SEEK_NEXT:
            case EVT_KEY_HU_SEEK_PREV:
            case EVT_KEY_CCP_SEEK_NEXT:         // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
            case EVT_KEY_CCP_SEEK_PREV:         // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
            //case EVT_KEY_RRC_CH_FW:             // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
            //case EVT_KEY_RRC_CH_BK:             // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
            case EVT_KEY_RRC_SEEK_NEXT:
            case EVT_KEY_RRC_SEEK_PREV:
                if(IsNeedTouchCleanUp(aEvent.GetEventId()))
                {
                    sendTouchCleanUpForApps();
                }

                if(frontLCDMode == APP_MEDIA_MODE_AUDIO || frontLCDMode == APP_MEDIA_MODE_VIDEO)
                    cancelAllPressedKey(1);
                if(rearLCDMode == APP_MEDIA_MODE_AUDIO || rearLCDMode == APP_MEDIA_MODE_VIDEO)
                    cancelAllPressedKey(2);

                cancelAllPressedKey(3);
                break;
            }
            break;
        }
    }
}

bool AppMediaPlayer_EngineMain::IsNeedTouchCleanUp(EVT_ID_T key)
{
    if(key == EVT_MAX )
        return false;

    //MP_LOG << "key " << key << LOG_ENDL;   // removed by sangmin.seol 2014.06.27 reduce log for performance

    switch(key)
    {
        case EVT_KEY_HU_JOG_DIAL_ENCODER:
        case EVT_KEY_SWRC_SEEK_NEXT:
        case EVT_KEY_SWRC_SEEK_PREV:
        case EVT_KEY_HU_SEEK_NEXT:
        case EVT_KEY_HU_SEEK_PREV:
        case EVT_KEY_CCP_SEEK_NEXT:         // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
        case EVT_KEY_CCP_SEEK_PREV:         // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
        //case EVT_KEY_RRC_CH_FW:             // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
        //case EVT_KEY_RRC_CH_BK:             // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
        case EVT_KEY_RRC_SEEK_NEXT:
        case EVT_KEY_RRC_SEEK_PREV:
            if(IsSwapDCEnabled() && (rearLCDMode == APP_MEDIA_MODE_AUDIO || rearLCDMode == APP_MEDIA_MODE_VIDEO))
                    return true;

            if(!IsSwapDCEnabled() && (frontLCDMode == APP_MEDIA_MODE_AUDIO || frontLCDMode == APP_MEDIA_MODE_VIDEO))
                    return true;
            break;
        case EVT_KEY_CCP_JOG_DIAL_CENTER:
        case EVT_KEY_CCP_JOG_DIAL_UP:
        case EVT_KEY_CCP_JOG_DIAL_RIGHT:
        case EVT_KEY_CCP_JOG_DIAL_LEFT:
        case EVT_KEY_CCP_JOG_DIAL_DOWN:
        case EVT_KEY_CCP_MENU:
            if(!IsSwapDCEnabled() && (frontLCDMode == APP_MEDIA_MODE_AUDIO || frontLCDMode == APP_MEDIA_MODE_VIDEO))
                return true;
            break;
        case EVT_KEY_RRC_JOG_DIAL_CENTER:
        case EVT_KEY_RRC_JOG_DIAL_UP:
        case EVT_KEY_RRC_JOG_DIAL_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_DOWN:
        case EVT_KEY_RRC_MENU:
            if(IsSwapDCEnabled() && (rearLCDMode == APP_MEDIA_MODE_AUDIO || rearLCDMode == APP_MEDIA_MODE_VIDEO))
                return true;
            break;
        default: // modified by cychoi 2014.02.04 for Prevent Fix
            break;
    }

    MP_LOG << "return false" << LOG_ENDL;
    return false;
}

// { added by sangmin.seol 2014.02.26 DUAL_KEY Flag moved from appfreamework
unsigned int AppMediaPlayer_EngineMain::getCCPStatus()
{
    return nCCPStatus;
}

unsigned int AppMediaPlayer_EngineMain::getRRCStatus()
{
    return nRRCStatus;
}

unsigned int AppMediaPlayer_EngineMain::getETCStatus()
{
    return nETCStatus;
}

void AppMediaPlayer_EngineMain::setCCPPressed(KEY_CCP_STATUS_T key)
{
    nCCPStatus |= key;
}

void AppMediaPlayer_EngineMain::setCCPReleased(KEY_CCP_STATUS_T key)
{
    nCCPStatus &= !key;
}

void AppMediaPlayer_EngineMain::setCCPClear()
{
    nCCPStatus = CCP_NONE;
}

void AppMediaPlayer_EngineMain::setRRCPressed(KEY_RRC_STATUS_T key)
{
    nRRCStatus |= key;
}

void AppMediaPlayer_EngineMain::setRRCReleased(KEY_RRC_STATUS_T key)
{
    nRRCStatus &= !key;
}

void AppMediaPlayer_EngineMain::setRRCClear()
{
    nRRCStatus = RRC_NONE;
}

void AppMediaPlayer_EngineMain::setETCPressed(KEY_ETC_STATUS_T key)
{
    nETCStatus |= key;
}

void AppMediaPlayer_EngineMain::setETCReleased(KEY_ETC_STATUS_T key)
{
    nETCStatus &= !key;
}

void AppMediaPlayer_EngineMain::setETCClear()
{
    nETCStatus = ETC_NONE;
}
// } added by sangmin.seol 2014.02.26 DUAL_KEY Flag moved from appfreamework
#endif

void AppMediaPlayer_EngineMain::sendVRCancelSignal()
{
    MP_LOG << LOG_ENDL;
    if(m_bVRStatus){
        MP_LOG << "send VR Cancel signal to UISH" << LOG_ENDL;
        Event event(EVT_VR_CANCEL_APP, GetThisAppId(), APP_UI_MANAGER);
        NotifyUISHCommon(event);
    }
}



#ifdef AVP_BACKTRACE
void AppMediaPlayer_EngineMain::avpBackTrace(int num)
{
    void *array[50];
    size_t size;
    char **strings;
    size_t i;
    QString fileName = AVP_BACKTRACE_LOG_FILE;
    QFileInfo tFileInfo(fileName);
    FILE *fp;

    if(tFileInfo.size() < 1048576)  // append during filesize below 1MB
        fp = fopen (AVP_BACKTRACE_LOG_FILE, "a+");
    else
        fp = fopen (AVP_BACKTRACE_LOG_FILE, "w+");

    fprintf(fp,"AVP Backtrace Log Signal %d\n", num);
    size = backtrace(array, 50);
    strings = backtrace_symbols(array, size);

    // {added by Michael.Kim 2014. 07.17 for timer log
    struct tm *t;
    time_t timer;
    timer = time(NULL);
    t = localtime(&timer);
    // }added by Michael.Kim 2014. 07.17 for timer log

    for(i = 2; i < size; i++){
        fprintf(fp, "%d: %04d-%02d-%02d %02d:%02d:%02d %s\n", i - 2,  t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, strings[i]);
        char buffer[256];
        char strBuffer[256];
        memset(buffer, 0x0, 256);
        memset(strBuffer, 0x0, 256);
        char *start = strstr(strings[i],"[");
        char *end = strstr(strings[i],"]");
        memcpy(strBuffer,start+1,end - start-1);
        fprintf(fp,"%s\n", strBuffer);
        sprintf(buffer,"addr2line -fe /app/bin/AppMediaPlayer %s\n", strBuffer);
        FILE *addr2line = popen(buffer, "r");
        memset(strBuffer, 0x0, 256);
        if(addr2line == NULL)
        {
            perror("error : ");
        }
        else{
            while(fgets(strBuffer, 256, addr2line)!=NULL)
                fprintf(fp,"%s", strBuffer);
            pclose(addr2line);
        }
    }

    free(strings);
    fclose(fp);
}

#endif

#ifdef AVP_LAST_AUDIOFILE_BACKUP
void AppMediaPlayer_EngineMain::backupLastAudioFile()
{
    if(m_LastPlayerMode == APP_MEDIA_MODE_AUDIO)
    {
        QString lastAudioFilePath;
        AppMediaPlayer_EngineAudio* pAudioEngine = dynamic_cast<AppMediaPlayer_EngineAudio*>(m_Engines[APP_MEDIA_MODE_AUDIO]);

        if(pAudioEngine)
        {
            MP::DEVICE_T device = ((AppMediaPlayer_Controller_Audio*)pAudioEngine->GetController())->getDevice();

            if(device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2)
            {
                lastAudioFilePath = pAudioEngine->GetCurrentFilePath();

                if(lastAudioFilePath != "")
                {
                    AudioUtils::backupFile(lastAudioFilePath, 31457280); // Limted Size -> 30MB (30*1024*1024)
                }
            }
        }
    }
}
#endif
