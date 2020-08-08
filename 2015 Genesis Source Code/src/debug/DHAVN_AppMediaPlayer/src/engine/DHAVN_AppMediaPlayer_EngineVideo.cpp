#include <QDebug>
#include <QtDeclarative>
#include <QDeclarativeContext>
#include <QGraphicsProxyWidget>
#include <QtGstQmlSink/qmlgstvideoitem.h>
#include <QtGstQmlSink/qmlpaintervideosurface.h>
#include <DHAVN_AppFileManager_Shared.h>
#include <QmlStatusBar/osdwriter.h>//modified by edo.lee 2013.04.04

#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_OverlaySourceModeArea.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenFSPlayback.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenVCDPBCMenu.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenVCDDirectAccess.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDVDPlayback.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDVDSettings.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenVCDPlayback.h"
//#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenTitlesChapters.h" // commented by cychoi 2013.07.13 for List View spec out
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenOptions.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenAUXPlayback.h"
//#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenVCDChapters.h" // commented by cychoi 2013.07.13 for List View spec out
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenCaption.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenFSFileInfoPopup.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDVDSearchCaption.h" // modified by ravikanth 13-03-13
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenPopProgress.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenPopToast.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenTitleDiscMenu.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenFull.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenTextButtonPopup.h"
// removed by Sergey 10.08.2013 for 182892
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDivxReg.h" // added by Sergey 19.07.2013
#include "util/video/DHAVN_AppMediaPlayer_Caption_ImageProvider.h" // modified by ravikanth 04-04-13

#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerDVD.h" // added by kihyung 2012.07.13
#include "util/osd/OSD_Manager.h"
#include "deckengineermode.h" // added by kihyung 2012.08.21 for 3PDMS 0137765 

#define USE_NEW_SETTING_ENTRY_POINT     1  // modified by kihyung 2013.1.26

AppMediaPlayer_EngineVideo::AppMediaPlayer_EngineVideo( AppMediaPlayer_EngineMain &aEngineMain )
    // { modified by Sergey Vetugov. CR#12671
    : m_EngineMain( aEngineMain ),
      // removed by Sergey 03.09.2013 for ITS#186054
      m_isFgReceived(false), // modified by kihyung 2013.1.26
      m_isLongPressed(0),  // added by kihyung 2013.2.5 for ISV 72327
      m_isNotifyGstAvailable(false),  // added by kihyung 2013.2.7 for ISV 72708
	  //m_isLockoutMode(false), //remove by edo.lee 2013.02.25
      m_isListView(false), // added by eunhye 2013.02.07 for ISV70720
      //m_isShowLockoutView(VIDEO_DISPLAY_MAX), //remove by edo.lee 2013.03.13
      //m_onBoot(true), //remove by edo.lee 2013.02.28
      m_bJukeboxDelayedPlay(false),
      m_bUSB1DelayedPlay(false),
      m_bUSB2DelayedPlay(false),
      m_isCopy(false), // addd by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list
	  // modified by Sergey 01.05.2013 for Tune Functionality
      is_FullScreen(true),  //added by eunhye 2013.03.19
      m_bFgState(false),  // added by wspark 2013.04.06 for ISV 78044
      m_trackerUpdateInProgress(false), // modified by ravikanth 20-04-13
      m_bPlayAfterBTCall(false), // added by kihyung 2013.08.03 for smoke test 24.18
      m_nAuxPlaybackState(PLAYER_STOP),
      m_ShowOSDFromBUfferedTrack(false), //added on 2013.07.03 ITS# 0177010
      m_DVDModeChanged(false),// modified by yongkyun.lee 2013-07-22 for : NO CR DVD title OSD
      m_pbOwner(VPEnum::VDISP_MAX),// added by Sergey 28.08.2013 for ITS#186507
      //m_bKeepFFREWOnBTCall(false)//remove by edo.lee 2013.09.25
      m_systemPopVisible(false),
      m_localPopVisible(false), // added by cychoi 2013.12.05 for ITS 213053
      m_systemPopupDisplay(VIDEO_DISPLAY_MAX), // added by yungi 2014.01.23 for ITS 221843
      m_bFrontLoaded(false), // added by Sergey 24.10.2013 for ITS#197778
      m_bSeekHKPressed(false), // added by Sergey 26.10.2013 to fix SEEK after ModeArea focus
      m_eModeChange(VCD_DIRECTACCESS_MODE_END), // added by sjhyun 2013.11.01 for ITS 204921
      m_bIsPlaybackControlQMLIsLoaded(false), // modified by sjhyun 2013.11.11 for ITS 206822
      m_bIsVideoPlaybackQMLIsLoaded(false), // added by sjhyun 2013.11.11 for ITS 206822
      //m_popupTypeOnSubScreen(POPUP_TXT_BTN_MAX) // added by cychoi 2014.03.19 for ITS 230153
      m_popupType(POPUP_TXT_BTN_MAX)//added by taihyun.ahn for ITS 221592 2014.01.23
{
	// { modified by Sergey 28.08.2013 for ITS#186507 
    qmlRegisterType<VPEnum>("VPEnum", 1, 0, "VPEnum");

    m_pController = new AppMediaPlayer_Video_Controller( aEngineMain, this );
    m_pJBCtrl = static_cast<AppMediaPlayer_Video_ControllerFS*>(m_pController->getController(MEDIA_SOURCE_JUKEBOX)); // added by Sergey 13.09.2013 for ITS#188762
    m_pUSB1Ctrl = static_cast<AppMediaPlayer_Video_ControllerFS*>(m_pController->getController(MEDIA_SOURCE_USB1)); // added by Sergey 13.09.2013 for ITS#188762
    m_pUSB2Ctrl = static_cast<AppMediaPlayer_Video_ControllerFS*>(m_pController->getController(MEDIA_SOURCE_USB2)); // added by Sergey 13.09.2013 for ITS#188762

	m_pScreenManager[VIDEO_DISPLAY_FRONT] = new AppMediaPlayer_Video_ScreenManager( aEngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN), VPEnum::VDISP_FRONT, this, this ); //modified by Sergey 27.09.2013 for ITS#189958
	m_pScreenManager[VIDEO_DISPLAY_REAR] = new AppMediaPlayer_Video_ScreenManager( aEngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN), VPEnum::VDISP_REAR, this, this ); //modified by Sergey 27.09.2013 for ITS#189958

    QDeclarativeContext *context = m_EngineMain.GetScreenContext(AppEngineQMLConstants::APP_FRONT_SCREEN);
    context->setContextProperty("VideoEngine", this);// modified by edo.lee 2013.02.04
    context->setContextProperty("SM", m_pScreenManager[VIDEO_DISPLAY_FRONT]);
    context = m_EngineMain.GetScreenContext(AppEngineQMLConstants::APP_REAR_SCREEN);// modified by edo.lee 2013.02.04
    context->setContextProperty("VideoEngine", this);
    context->setContextProperty("SM", m_pScreenManager[VIDEO_DISPLAY_REAR]);
	// } modified by Sergey 28.08.2013 for ITS#186507 


	
	m_imageProvider = new AppMediaPlayer_VideoCaptionProvider(); // modified by ravikanth 04-04-13
	aEngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN)->engine()->addImageProvider(QLatin1String("colors"), m_imageProvider); //XSubtitle added by jeeeun 2013.04.04
	aEngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN)->engine()->addImageProvider(QLatin1String("colors"), m_imageProvider); //XSubtitle added by jeeeun 2013.04.04

	// { modified by Sergey 03.09.2013 for ITS#186054
	registerScreens();
		
	m_isFMFile[DISPLAY_FRONT] = FM_FILE_MAX; // modified by Sergey 09.09.2013 for ITS#188283
	m_isFMFile[DISPLAY_REAR] = FM_FILE_MAX; // modified by Sergey 09.09.2013 for ITS#188283
	// } modified by Sergey 03.09.2013 for ITS#186054
	
    connect( m_EngineMain.GetMediaManager(),
             SIGNAL(deviceAttached(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool)),
             this,
             SLOT(onDeviceAttached(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool)) );
    connect( m_EngineMain.GetMediaManager(),
             SIGNAL(deviceRemoved(MEDIA_SOURCE_T,MEDIA_TYPE_T)),
             this,
             SLOT(onDeviceRemoved(MEDIA_SOURCE_T,MEDIA_TYPE_T)) );
    connect(m_pController, SIGNAL(deviceChanged(int)),
            this, SLOT(onDeviceChanged(int))); // added by Sergey for CR#15775

    
    connect(&m_EngineMain, SIGNAL(signalVideoFgReceived(bool)), this, SLOT(onSignalVideoFgReceived(bool))); //ys -20130911 ITS 0186726
    connect(&m_EngineMain, SIGNAL(signalBgReceived()),     this, SLOT(onSignalBgReceived())); // modified by kihyung 2013.1.26
    connect(&m_EngineMain, SIGNAL(camEnable(bool)),     this, SLOT(onCamEnableReceived(bool))); // added by wspark 2013.02.21 for ISV 73305
//    connect(&m_EngineMain, SIGNAL(jukeboxCopyInProgressChanged(bool)), this, SIGNAL(jukeboxCopyInProgress())); // modified by ravikanth 20-04-13
    connect(&m_EngineMain, SIGNAL(closeCopyCancelConfirmPopup()), this, SLOT(onCloseCopyCancelConfirmPopup())); // modified for ITS 0217509

#ifdef DUAL_KEY
     connect(this, SIGNAL(signalTransformEVT2QML_KEY(EVT_ID_T )), &m_EngineMain, SLOT(transformEVT2QML_KEY(EVT_ID_T)));
#endif

    connect(&m_EngineMain, SIGNAL(showInhibitionIcon(bool)), this, SIGNAL(showInhibitionIcon(bool))); // added by sjhyun 2013.11.05 for ITS 206357
    m_pMainUtil = AppMediaPlayer_MainUtils::GetInstance();

    m_PassPlay = false;// added by yongkyun.lee 20130310 for : NO CR Play and Puase
    // Timer to support TUNE fnctionality.
    m_pTuneTimer = new QTimer(this);
    m_pTuneTimer->setInterval(TUNE_INTERVAL);
    m_pTuneTimer->setSingleShot(true);

    m_pVideoTimer = new QTimer(this);
    m_pVideoTimer->setInterval(5000);    
    connect(m_pVideoTimer, SIGNAL(timeout()), this, SLOT(onVideoTimerExpired()));

    // { added by Sergey 15.09.2014 for ITS#189390
    m_pFgLoadingTimer = new QTimer(this); 
    m_pFgLoadingTimer->setInterval(5000);
    m_pFgLoadingTimer->setSingleShot(true); // added by Sergey 26.09.2013 for ITS#191542
    connect(m_pFgLoadingTimer, SIGNAL(timeout()), this, SLOT(onFgLoadingTimerExpired()));
    // } added by Sergey 15.09.2014 for ITS#189390

    //{Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side
    m_TabClicked=false;// added by yongkyun.lee@lge.com  2012.10.30 : : CR 13733  : Sound Path
    //    m_TabClicked=false;// added by yongkyun.lee@lge.com  2012.09.25 : : CR 13733  : Sound Path
    //}Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side

    connect( this, SIGNAL(contextChanged(bool)), this, SLOT(onContextChanged(bool)) );
    connect(m_pTuneTimer, SIGNAL(timeout()), this, SLOT(onTuneTimerExpired()));

    // { modified by Sergey for CR#15775
    m_nLastAVModeEvent = MODE_STATE_NONE;
    m_bVideoInFG[DISPLAY_FRONT] = false;  // added by Sergey 10.05.2013
    m_bVideoInFG[DISPLAY_REAR] = false;	// added by Sergey 10.05.2013
    m_bIsLaunchingScreenSettings = VIDEO_DISPLAY_MAX; //modified by edo.lee 2013.12.13 ITS 0211957
    m_bIsLaunchingSoundSettings = false; // added by cychoi 2013.08.05 for ScreenOptions is opened when returned from SoundSetting
    // } modified by Sergey for CR#15775
    m_bTempMode = false; // added by cychoi 2014.02.14 for DVD SearchCaption local Temporal Mode
    m_bScrollingTicker = true; // modified by Sergey 22.08.2013 for ITS#185742 
    m_playRequester = PLAY_REQUSTER_MAX; // added by Sergey for CR#16015

	// removed by Sergey 28.08.2013 for ITS#186507 
	
    //OSDview = new QDeclarativeView(); //delete by ys 2013.09.30 //added by edo.lee 2012.12.08 for statusbar
	m_formatStatus = false; // added by wspark 2012.12.17 for ISV64433
	// removed by Sergey 10.08.2013 for 182892
    //Load data from QSettings and fill the models
    m_bRewReachedFirst = false;
    LoadSettings();

    connect(m_pController->GetControllers().GetUSBController(),     SIGNAL(OnStateChanged(int)), this, SLOT(onStateChangedForGstPipe(int)));
    connect(m_pController->GetControllers().GetJukeBoxController(), SIGNAL(OnStateChanged(int)), this, SLOT(onStateChangedForGstPipe(int)));
    connect(m_pController->GetControllers().GetDeckController(),    SIGNAL(OnStateChanged(int)), this, SLOT(onStateChangedForGstPipe(int)));
    connect(m_pController, SIGNAL(onAuxStateChanged(int)), this, SLOT(onAuxStateChangedForGstPipe(int)));
    connect(m_pController, SIGNAL(tuneTimerExpired()), this, SLOT(onTuneTimerExpired())); // ITS 188647
    connect(GetVideoController(), SIGNAL(sourceReleased()), this, SLOT(onSourceReleased())); // added by Sergey 14.08.2013 for ITS#184081
}


//added on 2013.07.03 ITS# 0177010
void AppMediaPlayer_EngineVideo::setShowOSDFromBUfferedTrack(bool val){
    m_ShowOSDFromBUfferedTrack = val;
}

//added on 2013.07.03 ITS# 0177010
bool AppMediaPlayer_EngineVideo::getShowOSDFromBUfferedTrack(){
    return m_ShowOSDFromBUfferedTrack;
}


AppMediaPlayer_EngineVideo::~AppMediaPlayer_EngineVideo()
{
    delete m_imageProvider; // modified by ravikanth 04-04-13
    delete m_pController;
    AppMediaPlayer_MainUtils::FreeInstance();
}

void *AppMediaPlayer_EngineVideo::GetUIState()
{
    return NULL;
}

QString AppMediaPlayer_EngineVideo::GetUSBPath( MEDIA_SOURCE_T source )
{
   return m_EngineMain.GetMediaManager()->GetMountPath(source);
}

// { modified by Sergey for CR#15775
bool AppMediaPlayer_EngineVideo::EventHandler( Event& aEvent, DISPLAY_T display )
{
    bool ret = false;

    switch( aEvent.GetEventId() )
    {
        // { added by Sergey 07.09.2013 for PRE_FG supoort
        case EVT_REQUEST_FG:
        {
            MP_HIGH << "EVT_REQUEST_FG display =" << display << LOG_ENDL;

            switchSource(mapEPToSource(m_EntryPoint));

            lateScreenChange(GetVideoController()->getSource(), display);

            // { modified by Sergey 09.10.2013 for ITS#188283, 191978, 194422
            if(m_isFMFile[DISPLAY_FRONT] == FM_FILE_NEW || m_isFMFile[DISPLAY_REAR] == FM_FILE_NEW)
                GetVideoController()->playFromFileManager();

            m_isFMFile[DISPLAY_FRONT] = FM_FILE_MAX;
            m_isFMFile[DISPLAY_REAR] = FM_FILE_MAX;
            // } modified by Sergey 09.10.2013 for ITS#188283, 191978, 194422

            m_pFgLoadingTimer->start(); // added by Sergey 15.09.2014 for ITS#189390

            // { modified by Sergey 14.12.2013. NoITS. Blinking Loading.. when press BACK HK in single DISPLAY mode.
            MP_HIGH << "pipeline owner =" << m_EngineMain.GetGstPipelinesOwner(display) << LOG_ENDL;

            if(!isDRSEntryPoint(m_EntryPoint) && m_EngineMain.GetGstPipelinesOwner(display) != APP_AVP) // added by Sergey 13.11.2013 for ITS#208891
            {
                MP_HIGH << "NotifyWhenGstPipelinesAvailable" << display << LOG_ENDL;
                m_pVideoTimer->start();//added by taihyun.ahn 2013.12.18 for ITS 216003
                m_EngineMain.GetNotifier()->setGstPipelineDisplay(display); // added by kihyung 2014.01.14 for ITS 0218420
                m_EngineMain.NotifyWhenGstPipelinesAvailable(display);
            }
            else
            {
                DelayedPlay();//added by taihyun.ahn 2013.12.18 for ITS 216003
                
                if(!isDRSEntryPoint(m_EntryPoint) && GetVideoController()->getClearDisplay() == true)
                    GetVideoController()->clearDisplay(false); // added by Sergey 24.12.2013 for ITS#217101
            }
            // } modified by Sergey 14.12.2013. NoITS. Blinking Loading.. when press BACK HK in single DISPLAY mode.

            break;
        }
        // } added by Sergey 07.09.2013 for PRE_FG supoort

        
        case EVT_REQUEST_PRE_FG:
        {
            bool bLockoutMode = false; // added by kihyung 2013.06.08 for ITS 0172032

            // { modified by kihyung 2013.1.26
            // { added by edo.lee 2013.04
            //if(display == DISPLAY_FRONT)
            {
                APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();

                MP_HIGH << "EVT_REQUEST_PRE_FG entry =" << pEventData->sAppEntryPoint << LOG_ENDL;	// modified by Sergey 07.09.2013 for PRE_FG supoort

                //{ added by hyochang.ryu 20130619->//deleted 20130717 
                //if(pEventData->sAppEntryPoint==eAPP_AVP_VIDEO_MODE_AUX||pEventData->sAppEntryPoint==eAPP_AVP_VIDEO_MODE_AUX_DRS)
                //    m_EngineMain.GetNotifier()->DisplayVideoTrackOSD();
                //} added by hyochang.ryu 20130619 
                //DisplayOSD(true); //remove by edo.lee 2013.04.01
                setVideoListView(VIDEO_DISPLAY_MAX, false);// modified by edo.lee 2013.06.10
                switch (pEventData->sAppEntryPoint)
                {
                    case eAPP_AVP_VIDEO_MODE_DISC_DRS:
                    case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
                    case eAPP_AVP_VIDEO_MODE_USB1_DRS:
                    case eAPP_AVP_VIDEO_MODE_USB2_DRS:
                    case eAPP_AVP_VIDEO_MODE_AUX_DRS:
                    {
                        //if(m_isLockoutMode) break; //added by edo.lee 2013.02.25
                        // m_isLockoutMode = true;
                        //remove by edo.lee 2013.07.15 STATIC TEST
                        //QObject *uiListener = m_EngineMain.GetDeclarativeView(display == DISPLAY_FRONT ? AppEngineQMLConstants::APP_FRONT_SCREEN : AppEngineQMLConstants::APP_REAR_SCREEN);
                        MP_LOG <<"DRS ON" << LOG_ENDL;
                        //{move by edo.lee 2014.1.8 ITS 0217055 
                        //emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED); //Added by taihyun.ahn for ITS 0189456, 0192211
                        //emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED);
                        //emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_CENTER, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED);
                        //}
                        if(display == DISPLAY_FRONT)
                        {
                            MP_LOG<<"front" << LOG_ENDL;
                            //if(m_pScreenManager[VIDEO_DISPLAY_FRONT]->getScreensCount() == 0)
                            //{
                            //	m_EngineMain.loadComponents(APP_MEDIA_MODE_VIDEO,DISPLAY_FRONT);
                            //}							
                            //m_pScreenManager[VIDEO_DISPLAY_FRONT]->setJogSource(uiListener);
                            //if(m_pScreenManager[VIDEO_DISPLAY_FRONT]->getIsLockoutMode())break;
                            //m_pScreenManager[VIDEO_DISPLAY_FRONT]->setIsLockoutMode(true);//remove by edo.lee 2013.03.26
                            m_pScreenManager[VIDEO_DISPLAY_FRONT]->lockoutMode(true);//getMediaSourceFromDRS(mapToModeState(pEntryPoint)));
                            //remove by edo.lee 2013.07.30 two screen on DRS
                            //if(getRearFull())
                            //{
                            //setDefaultScreen(VIDEO_DISPLAY_FRONT);
                            //}

                            //m_pScreenManager[VIDEO_DISPLAY_REAR]->closeFullScreen();//added by edo.lee 2013.02.25
                            //m_isShowLockoutView = VIDEO_DISPLAY_FRONT; //remove by edo.lee 2013.03.13
                            //emit lockoutMode(true);
                        }
                        else
                        {
                            // removed by Sergey 10.08.2013 for 182892
                            m_pScreenManager[VIDEO_DISPLAY_REAR]->lockoutMode(true);//getMediaSourceFromDRS(mapToModeState(pEntryPoint)));
                        }
                        bLockoutMode = true;
                        break;
                    }

                    default:
                    {
                        //if(m_isLockoutMode == false) break; //added by edo.lee 2013.02.25
                        //m_isLockoutMode = false;
                        MP_LOG <<"DRS OFF" << LOG_ENDL;
                        //modified by edo.lee 2013.02.25
                        //m_isShowLockoutView = VIDEO_DISPLAY_MAX;

                        VIDEO_DISPLAY_T disp = VIDEO_DISPLAY_FRONT;
                        if(display == DISPLAY_FRONT)			
                            disp = VIDEO_DISPLAY_FRONT;
                        else
                            disp = VIDEO_DISPLAY_REAR;

                        m_pScreenManager[disp]->lockoutMode(false);//added by edo.lee 2013.02.27
                        //{move by edo.lee 2014.1.8 ITS 0217055 
                        //emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED); //Added by taihyun.ahn for ITS 0189456, 0192211
                        //emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED);
                        //emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_CENTER, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED);
                        //}
                        // removed by Sergey for ITS#181872 30.07.2013
                        break;
                    }
                }

                m_nLastAVModeEvent = mapToModeState(pEventData->sAppEntryPoint); // added by kihyung 2013.06.02
            }
            // } added by edo.lee 

            if(m_pVideoTimer->isActive()) {
                m_pVideoTimer->stop();
            }

            // removed by Sergey 31.08.2013 for ITS#187830

            m_isNotifyGstAvailable = true;

            //{ modified by yongkyun.lee 2013-07-22 for : NO CR : DVD title OSD
            if( m_nLastAVModeEvent == MODE_STATE_DISC_VIDEO )
                m_DVDModeChanged = true;
            //} modified by yongkyun.lee 2013-07-22 

            // { modified by kihyung 2013.06.08 for ITS 0172032
            if(bLockoutMode == false) {
                // MP_LOG << "m_EngineMain.NotifyWhenGstPipelinesAvailable" << LOG_ENDL;
                // { modified by cychoi 2013.12.17 NoITS. Blinking Loading.. when press BACK HK in single DISPLAY mode.
                if(!isDRSEntryPoint(m_EntryPoint) && m_EngineMain.GetGstPipelinesOwner(display) != APP_AVP) // added by Sergey 13.11.2013 for ITS#208891
                {
                    emit setLoadingPBCMenu(true); //add by shkim 2013.11.13 for ITS 209040 Camera -> VCD PBC Menu Issue
                }
                // } modified by cychoi 2013.12.17
                //{removed by taihyun.ahn 2013.12.18 for ITS 216003
                /*if(!getEngineMain().getBTCallPowerOn())//Added by taihyun.ahn 2013.10.22
                {
                    m_pVideoTimer->start();
                }*/
                //}removed by taihyun.ahn 2013.12.18 for ITS 216003
                // removed by Sergey 07.09.2013 for PRE_FG supoort
            }
            //{removed by taihyun.ahn 2013.12.18 for ITS 216003
            //else {
                //DelayedPlay();
            //}
            //}removed by taihyun.ahn 2013.12.18 for ITS 216003
            // } modified by kihyung 2013.06.08
            //m_EngineMain.setIsFromOtherAV(true); remove by edo.lee 2013.03.11
            //m_onBoot = false; //remove by edo.lee 2013.02.28
            setTempMode(true); // added by cychoi 2014.02.14 for DVD SearchCaption local Temporal Mode
            break;
        }
        // } added by edo.lee

		// { modified by Sergey 01.05.2013 for Tune Functionality

        // Jog Tune events
        case EVT_KEY_CCP_JOG_DIAL_ENCODER:
        case EVT_KEY_CCP_JOG_DIAL_CENTER:
        case EVT_KEY_CCP_JOG_DIAL_UP:
        case EVT_KEY_CCP_JOG_DIAL_RIGHT:
        case EVT_KEY_CCP_JOG_DIAL_LEFT:
        case EVT_KEY_CCP_JOG_DIAL_DOWN:
        // { added by yungi 2014.02.14 for ITS 225174
        case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:
        case EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT:
        case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:
        case EVT_KEY_CCP_JOG_DIAL_TOP_LEFT:
        // } added by yungi 2014.02.14
        case EVT_KEY_RRC_JOG_DIAL_ENCODER:
        case EVT_KEY_RRC_JOG_DIAL_CENTER:
        case EVT_KEY_RRC_JOG_DIAL_UP:
        case EVT_KEY_RRC_JOG_DIAL_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_DOWN:
        // { added by yungi 2014.02.14 for ITS 225174
        case EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT:
        case EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT:
        case EVT_KEY_RRC_JOG_DIAL_TOP_LEFT:
        // } added by yungi 2014.02.14
		{
			#ifdef DUAL_KEY
            switch(aEvent.GetEventId())
            {
                case EVT_KEY_CCP_JOG_DIAL_ENCODER:
                case EVT_KEY_CCP_JOG_DIAL_CENTER:
                case EVT_KEY_CCP_JOG_DIAL_UP:
                case EVT_KEY_CCP_JOG_DIAL_RIGHT:
                case EVT_KEY_CCP_JOG_DIAL_LEFT:
                case EVT_KEY_CCP_JOG_DIAL_DOWN:
                // { added by yungi 2014.02.14 for ITS 225174
                case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:
                case EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT:
                case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:
                case EVT_KEY_CCP_JOG_DIAL_TOP_LEFT:
                // } added by yungi 2014.02.14
                {
                    MP_LOG << "DUAL_KEY CCP JOG forwarding to QML through signalJogNavigation" << LOG_ENDL;
                    KEY_EVENT_DATA_T *ed = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                    emit signalJogNavigation(signalTransformEVT2QML_KEY(aEvent.GetEventId()),
                                              (ed->keyStatus)+1001,  false, (int)aEvent.GetCommand());
                    break;
                }
                case EVT_KEY_RRC_JOG_DIAL_ENCODER:
                case EVT_KEY_RRC_JOG_DIAL_CENTER:
                case EVT_KEY_RRC_JOG_DIAL_UP:
                case EVT_KEY_RRC_JOG_DIAL_RIGHT:
                case EVT_KEY_RRC_JOG_DIAL_LEFT:
                case EVT_KEY_RRC_JOG_DIAL_DOWN:
                // { added by yungi 2014.02.14 for ITS 225174
                case EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT:
                case EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT:
                case EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT:
                case EVT_KEY_RRC_JOG_DIAL_TOP_LEFT:
                // } added by yungi 2014.02.14
                {
                    MP_LOG << "DUAL_KEY RRC JOG forwarding to QML through signalJogNavigation" << LOG_ENDL;
                    KEY_EVENT_DATA_T *ed = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                    emit signalJogNavigation(signalTransformEVT2QML_KEY(aEvent.GetEventId()),
                                              (ed->keyStatus)+1001, true, (int)aEvent.GetCommand());
                    break;
                }
            }
            #endif

            switch(aEvent.GetEventId())
            {
                case EVT_KEY_CCP_JOG_DIAL_ENCODER:
                {
                    if( (int)aEvent.GetCommand() > 0 )
                    {
                        emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                                  AppEngineQMLConstants::KEY_STATUS_PRESSED, false, (int)aEvent.GetCommand());
                        emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                                  AppEngineQMLConstants::KEY_STATUS_RELEASED, false, (int)aEvent.GetCommand());
                    }
                    else
                    {
                        emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                                  AppEngineQMLConstants::KEY_STATUS_PRESSED, false, (int)aEvent.GetCommand());
                        emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                                  AppEngineQMLConstants::KEY_STATUS_RELEASED, false, (int)aEvent.GetCommand());
                    }
                    break;
                }
                case EVT_KEY_RRC_JOG_DIAL_ENCODER:
                {
                    if( (int)aEvent.GetCommand() > 0 )
                    {
                        emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                                  AppEngineQMLConstants::KEY_STATUS_PRESSED, true, (int)aEvent.GetCommand());
                        emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                                  AppEngineQMLConstants::KEY_STATUS_RELEASED, true, (int)aEvent.GetCommand());
                    }
                    else
                    {
                        emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                                  AppEngineQMLConstants::KEY_STATUS_PRESSED, true, (int)aEvent.GetCommand());
                        emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                                  AppEngineQMLConstants::KEY_STATUS_RELEASED, true, (int)aEvent.GetCommand());
                    }
                    break;
                }
            }

            // Should not handle if playback screen is not on top
            //{ added by yongkyun.lee 20130705 for : ITS 178308
            VIDEO_DISPLAY_T disp = (getCurrentScreen() == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;
            // removed by cychoi 2013.08.06 for Position OSD is displayed on DVD Setting from Title/Disc Menu 

			//{added by taihyun.ahn 2013.11.04
            /*KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            //if(m_pController->IsFFRewState() && eventData->keyStatus == KEY_STATUS_PRESSED)
            if(eventData->keyStatus == KEY_STATUS_PRESSED)
            {
                m_pController->cancel_ff_rew();
                m_isLongPressed = 0;
            }*/
			//}
            if( (aEvent.GetEventId() == EVT_KEY_CCP_JOG_DIAL_ENCODER || aEvent.GetEventId() == EVT_KEY_RRC_JOG_DIAL_ENCODER) &&
                getScreenDVDMenuTitle(disp) ) // modified by cychoi 2013.08.06 for Position OSD is displayed on DVD Setting from Title/Disc Menu // modified by cychoi 2013.07.27 for ISV 88393
            {
                 //emit setTuneMode(true ); //commented by yungi 2013.08.13 for ISV 88897 // modified by yongkyun.lee 2013-07-06 for : ITS 178575
                 //m_EngineMain.GetNotifier()->DisplayDVDMenuOSD(); // commented by cychoi 2013.07.13 for Smoke Test DVD menu OSD wrong display
                 //emit setTuneMode(false); //commented by yungi 2013.08.13 for ISV 88897 // modified by yongkyun.lee 2013-07-06 for : ITS 178575  
                 break;
            }            
            //} added by yongkyun.lee 20130705 
            if(shouldHandleJogTuneEvent(display) == false)
                break;
        }
        // HU Tune events are always handled
        case EVT_KEY_HU_JOG_DIAL_ENCODER:
        case EVT_KEY_HU_TUNE_PRESSED:
        {
            MP_LOG << "Tune Event =" << aEvent.GetEventId() << LOG_ENDL;

            //{ added by yongkyun.lee 20130705 for : ITS 178308
            VIDEO_DISPLAY_T disp = (getCurrentScreen() == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;
            // removed by cychoi 2013.08.06 for Position OSD is displayed on DVD Setting from Title/Disc Menu 

            if(processedInAUXMode(aEvent) || processedInDiscMode()) break; // modified by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling

            if( (aEvent.GetEventId() == EVT_KEY_HU_TUNE_PRESSED ) &&
                getScreenDVDMenuTitle(disp) ) // modified by cychoi 2013.08.06 for Position OSD is displayed on DVD Setting from Title/Disc Menu // modified by cychoi 2013.07.27 for ISV 88393
            {
                // {  modified by cychoi 2013.10.02 for ITS 192831 DVD Audio OSD
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

                if(eventData->keyStatus == KEY_STATUS_PRESSED)
                {
                    // { added by cychoi 20103.11.13 for ITS 208746 & 196052 OSD on DVD Audio Play
                    if(isDVDTrackPlaying())
                    {
                        showCurrentOSD();
                        m_EngineMain.setIsTermOnTrackChange(false);
                    }
                    else
                    {
                        OSDManager::instance()->displayDVDMenuOSD(true); // modified by cychoi 2013.10.15 for SmokeTest
                    }
                    // } added by cychoi 20103.11.13
                }
                // }  modified by cychoi 2013.10.02
                break;
            }            
            //} added by yongkyun.lee 20130705 

            if(!aEvent.Data().isEmpty())
            {
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

                MP_HIGH << "key =" << aEvent.GetEventId() << "status =" << eventData->keyStatus << "tuneTimer =" << m_pTuneTimer->isActive() << LOG_ENDL; // added by Sergey 16.10.2013 for ITS#195505

                if((aEvent.GetEventId() == EVT_KEY_CCP_JOG_DIAL_CENTER && getVideoControllerFoucusIndex(DISPLAY_FRONT) == 1) || (aEvent.GetEventId() == EVT_KEY_RRC_JOG_DIAL_CENTER && getVideoControllerFoucusIndex(DISPLAY_REAR) == 1))//modified for ISV 98058
                   // || aEvent.GetEventId() == EVT_KEY_HU_TUNE_PRESSED) // remove by edo.lee 2014.01.14 for working at press time
                   //     && eventData->keyStatus != KEY_STATUS_RELEASED)
                {
                	// { added by Sergey 16.10.2013 for ITS#195505
                    if(eventData->keyStatus == KEY_STATUS_PRESSED /*&& aEvent.GetEventId() == EVT_KEY_HU_TUNE_PRESSED */&& m_pTuneTimer->isActive())// modified by edo.lee 2014.01.14 for working at press time
                    {
                        emit lightControls((int)VLIGHT_CENTER, true, QVariant(VPEnum::VDISP_FRONT));
                        emit lightControls((int)VLIGHT_CENTER, true, QVariant(VPEnum::VDISP_REAR));
                    }
                    // } added by Sergey 16.10.2013 for ITS#195505

                    break; // handle only RELEASE for "tune select" operation
                }

                //{added for ITS 231473 2014.03.27
                if(aEvent.GetEventId() == EVT_KEY_HU_TUNE_PRESSED && eventData->keyStatus == KEY_STATUS_PRESSED && m_pTuneTimer->isActive())
                {
                    emit lightControls((int)VLIGHT_CENTER, true, QVariant(VPEnum::VDISP_FRONT));
                    m_EngineMain.repaintUI();
                    emit lightControls((int)VLIGHT_CENTER, true, QVariant(VPEnum::VDISP_REAR));
                    m_EngineMain.repaintUI();
                }
                //}added for ITS 231473 2014.03.27
            }

            if(playRequester() != PLAY_REQUSTER_TUNE) // added by Sergey 25.08.2013 for ITS#186506
                tuneKnobHandler(aEvent, display);

            break;

        } // end of tune handle

		// } modified by Sergey 01.05.2013 for Tune Functionality

        case EVT_KEY_HU_SEEK_PREV:
        case EVT_KEY_CCP_SEEK_PREV:         // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
        //case EVT_KEY_RRC_CH_BK:             // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
        case EVT_KEY_RRC_SEEK_PREV:
        case EVT_KEY_SWRC_SEEK_PREV:
        {
            MP_LOG << "EVT_KEY_HU_SEEK_PREV =" << aEvent.GetEventId() << LOG_ENDL;

            //if(processedInAUXMode(aEvent)) break;		//deleted by hyochang.ryu 20131009 for ITS194476

            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                //{ added by hyochang.ryu 20131009 for ITS194476
                if((eventData->keyStatus == KEY_STATUS_PRESSED && processedInAUXMode(aEvent)) || processedInDiscMode()) // modified by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
                    break;
                //} added by hyochang.ryu 20131009 for ITS194476
                prevSEEKButtonHandler(eventData->keyStatus,display);
                if(eventData->keyStatus == KEY_STATUS_PRESSED)
                    sendSeekEventToFM(aEvent);  // mofdified by ravikanth for ISV 89536
            }
            break;
        }
        case EVT_KEY_HU_SEEK_NEXT:
        case EVT_KEY_CCP_SEEK_NEXT:     // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
        //case EVT_KEY_RRC_CH_FW:         // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
        case EVT_KEY_RRC_SEEK_NEXT:
        case EVT_KEY_SWRC_SEEK_NEXT:
        {
            MP_LOG << "EVT_KEY_HU_SEEK_NEXT =" << aEvent.GetEventId() << LOG_ENDL;

            //if(processedInAUXMode(aEvent)) break;		//deleted by hyochang.ryu 20131009 for ITS194476

            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                //{ added by hyochang.ryu 20131009 for ITS194476
                if((eventData->keyStatus == KEY_STATUS_PRESSED && processedInAUXMode(aEvent)) || processedInDiscMode()) // modified by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
                    break;
                //} added by hyochang.ryu 20131009 for ITS194476
                nextSEEKButtonHandler(eventData->keyStatus, display);
                if(eventData->keyStatus == KEY_STATUS_PRESSED)
                    sendSeekEventToFM(aEvent); // mofdified by ravikanth for ISV 89536
            }
            break;
        }
        case EVT_KEY_HU_BACK:
        case EVT_KEY_CCP_BACK:
        {
            MP_LOG << "EVT_KEY_CCP_BACK" << LOG_ENDL;
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                if ( eventData->keyStatus == KEY_STATUS_PRESSED )
                {
                    backHandle(true);
                }
            }
            break;
        }
        case EVT_KEY_RRC_BACK:
		{
            MP_LOG << "EVT_KEY_RRC_BACK" << LOG_ENDL;
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                if ( eventData->keyStatus == KEY_STATUS_PRESSED )
                {
                    backHandle(false);
                }
            }
            break;
        }
        case EVT_KEY_CCP_MENU:
        case EVT_KEY_HU_MENU:		
        case EVT_KEY_RRC_MENU:
        {
            MP_LOG << "EVT_KEY_CCP_MENU" << LOG_ENDL;
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                if(processedInDiscMode()) break; // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
//{ added by aettie 2013.08.01 for ITS 181900
//                if ( eventData->keyStatus == KEY_STATUS_RELEASED )
                if ( eventData->keyStatus == KEY_STATUS_PRESSED )
                {
                    //releaseTuneMode();  // added by dongjin 2013.02.25 for ISV 73830
                    releaseTuneModeByMenu();//added by edo.lee 2013.04.05 78867
                    //{ modified by edo.lee 2013.03.12
                    // { modified by wspark 2012.08.02 for CR12020
                    // m_pScreenManager->onMenuKeyPressed();  

                    //{modified by edo.lee 2013.03.13
                   /*remove by edo.lee 2013.05.02 
                   if((m_pScreenManager[VIDEO_DISPLAY_FRONT]->getIsLockoutMode() && display == DISPLAY_FRONT)
						||(m_pScreenManager[VIDEO_DISPLAY_REAR]->getIsLockoutMode() && display == DISPLAY_REAR))
                    	{
                    		break;
                    	}
					*/
					MP_LOG <<"display " << display << "swap " <<m_EngineMain.getIsSwapEnabled() << LOG_ENDL;
					if( m_EngineMain.getCloneState() == CLONE_OFF)
					{
						if(display == DISPLAY_FRONT )
							m_pScreenManager[VIDEO_DISPLAY_FRONT]->onMenuKeyPressed(aEvent.GetEventId());
						else if( display == DISPLAY_REAR )
							m_pScreenManager[VIDEO_DISPLAY_REAR]->onMenuKeyPressed(aEvent.GetEventId());
					}else
					{
						if( m_EngineMain.getCloneState() == CLONE_ON_FRONT)
							m_pScreenManager[VIDEO_DISPLAY_FRONT]->onMenuKeyPressed(aEvent.GetEventId());
						else if( m_EngineMain.getCloneState() == CLONE_ON_REAR)
							m_pScreenManager[VIDEO_DISPLAY_REAR]->onMenuKeyPressed(aEvent.GetEventId());
					}
					
						
					/*
	                     if(  //m_isShowLockoutView != VIDEO_DISPLAY_FRONT && //remove by edo.lee 2013.03.13
						 	 //m_EngineMain.GetFrontView() == APP_MEDIA_MODE_VIDEO && //modified by edo.lee 2013.03.16
						 	 m_pScreenManager[VIDEO_DISPLAY_FRONT]->getScreensCount() > 0 )
						 	 //!m_pScreenManager[VIDEO_DISPLAY_FRONT]->getIsLockoutMode())//modified by edo.lee 2013.05.02

	                     	{                     		
			                	m_pScreenManager[VIDEO_DISPLAY_FRONT]->onMenuKeyPressed(aEvent.GetEventId());
	                     	}
						 else if( //m_isShowLockoutView != VIDEO_DISPLAY_REAR &&//remove by edo.lee 2013.03.13
						 	// m_EngineMain.GetRearView() == APP_MEDIA_MODE_VIDEO &&//modified by edo.lee 2013.03.16
						 	 m_pScreenManager[VIDEO_DISPLAY_REAR]->getScreensCount() > 0)
						 	 //!m_pScreenManager[VIDEO_DISPLAY_REAR]->getIsLockoutMode())//added by edo.lee 2013.05.02
						 	{					 		
								m_pScreenManager[VIDEO_DISPLAY_REAR]->onMenuKeyPressed(aEvent.GetEventId());
						 	}
						*/
					 	
					//} modified by edo.lee 2013.03.12
					 //}modified by edo.lee
                    // } modified by wspark
                }
            }
            ret = true;
            break;
        }
        case EVT_KEY_SOFT_MENU:
        {
            MP_LOG << "EVT_KEY_SOFT_MENU" << LOG_ENDL;
            releaseTuneMode();  // added by dongjin 2013.02.25 for ISV 73830
            // { modified by wspark 2012.08.02 for CR12020
            // m_pScreenManager->onMenuKeyPressed();
             if(display == DISPLAY_FRONT)
	            {	
                	m_pScreenManager[VIDEO_DISPLAY_FRONT]->onMenuKeyPressed(aEvent.GetEventId());
             	}
	         else
			 	{			 	
					m_pScreenManager[VIDEO_DISPLAY_REAR]->onMenuKeyPressed(aEvent.GetEventId());
			 	}
            // } modified by wspark
            ret = true;
            break;
        }
        
        // removed by Sergey 31.08.2013 for ITS#187830
        case EVT_NOTIFY_AV_MODE: //younsim.jo 20140220 its-225184
        {
            if( !aEvent.Data().isEmpty() ){
                EVT_COMMON_BASE_EVENT_DATA_T *pData = (EVT_COMMON_BASE_EVENT_DATA_T*)aEvent.Data().data();
                MODE_STATE_T                  eCurrMode = mapToModeState(m_EngineMain.GetLastAVEntryPoint(false));
                MODE_STATE_T eNextMode = (MODE_STATE_T)pData->iEventData01;
                MP_LOG << "EVT_NOTIFY_AV_MODE eNextMode = " <<eNextMode << LOG_ENDL;
                if(eNextMode != eCurrMode)
                {
                    if(m_EngineMain.GetAVMediaMode(m_EngineMain.GetLastAVEntryPoint(false)) == APP_MEDIA_MODE_VIDEO)
                    {
                        switchSource(MEDIA_SOURCE_MAX);
                        MP_LOG << "call switchSource func" << LOG_ENDL;
                    }
                }
            }
            break;
        }
        case EVT_AV_MODE_CHANGE:
        {
            MP_HIGH << "EVT_AV_MODE_CHANGE" << LOG_ENDL;
            if( !aEvent.Data().isEmpty() )
            {
                EVENT_MODE_CHANGED_DATA_T* pData = (EVENT_MODE_CHANGED_DATA_T*)aEvent.Data().data();
                if(pData == NULL) break;
				//added by edo.lee 2013.05.01
	        	if(m_EngineMain.getIsCompleteModeChange() == true)
	    		{
	    			MP_LOG<< " now is proceding to EVT_REQUEST_FG" << LOG_ENDL;
					break;
	    		}
				//added by edo.lee                 
                MP_LOG << "CNTHK EVT_AV_MODE_CHANGE: mode =" << pData->sMode << LOG_ENDL;
                
                //if(pData->sMode >=MODE_STATE_DISC_AUDIO && pData->sMode <= MODE_STATE_AUX2_VIDEO)
				//{
				//	DisplayOSD(true);
					//m_EngineMain.setIsFromOtherAV(true); remove by edo.lee 2013.03.11
				//}
				//else
				//{
				//	DisplayOSD(false);
					//m_EngineMain.setIsFromOtherAV(false); remove by edo.lee 2013.03.11
				//}

                //bool bVideoMode = false;

                // { removed by kihyung 2013.3.26 for ISV 77428
                /*
                if(m_nLastAVModeEvent == pData->sMode)
                {
                    MP_LOG << "EVT_AV_MODE_CHANGE return.    Prev:" << m_nLastAVModeEvent << " Curr:" << pData->sMode << LOG_ENDL;
                    break;
                }
                else
                */
                // } removed by kihyung
                {
                    MP_LOG << "CNTHK EVT_AV_MODE_CHANGE different. Prev:" << m_nLastAVModeEvent << " Curr:" << pData->sMode << LOG_ENDL;
                    releaseTuneMode(); //added by edo.lee 2013.06.18
                    m_nLastAVModeEvent = pData->sMode;

                    switch(pData->sMode)
                    {
                    //moved under disc by hyochang.ryu 20131004 for ITS193813
                    //added by shkim ITS 178085
                    case MODE_STATE_DISC_VIDEO:
                    {
                        if(!isVideoInFG()) // modified by Sergey 15.09.2013 for ITS#190306
                        {
                            handleAVModeChange(pData->sMode, display);
                        }
                                                DisplayOSD(true);//added by edo.lee 2013.06.13
                        if(m_isNotifyGstAvailable == false && m_pVideoTimer->isActive() == false) {
                            if(m_EngineMain.getOnBoot() == true) { // modified by kihyung 2013.4.4
                                DelayedPlay(false);
                            }
                            else {
                                if(m_bIsLaunchingScreenSettings == VIDEO_DISPLAY_MAX){ // modified by cychoi 2013.12.17 for ITS 211957
                                    if(m_EngineMain.getCamInFG() == true){
                                        m_pController->clearDisplay(true); // modified by Sergey for ITS#181872 30.07.2013
                                    }
                                    else
                                    {
                                        // { modified by kihyung 2014.01.17 for ITS 0218420
                                        /*
                                        // removed by Sergey 18.05.2013
                                        MP_HIGH << "NotifyWhenGstPipelinesAvailable" << display << LOG_ENDL;
                                        m_pVideoTimer->start();
                                        m_EngineMain.setGstPipelineDisplay(display); // added by kihyung 2014.01.14 for ITS 0218420 
                                        m_EngineMain.NotifyWhenGstPipelinesAvailable(display);
                                        */
                                        //m_EngineMain.GetNotifier()->setGstPipelineDisplay(display); // added by kihyung 2014.01.14 for ITS 0218420
                                        DelayedPlay(false);
                                        // } modified by kihyung 2014.01.17 for ITS 0218420
                                    }
                                }

                            }
                        }
                        m_EngineMain.SetAVOffMode(false);
                        //{ modified by yongkyun.lee 2013-07-22 for : NO CR : DVD title OSD
                        if( pData->sMode == MODE_STATE_DISC_VIDEO )
                             m_DVDModeChanged = true;
                        //} modified by yongkyun.lee 2013-07-22 
                        break;
                    }
                    //added by shkim ITS 178085
                    //{ moved under disc by hyochang.ryu 20131004 for ITS193813
                    case MODE_STATE_AUX1_VIDEO:
                    case MODE_STATE_AUX2_VIDEO:  
                    //} moved under disc by hyochang.ryu 20131004 for ITS193813
                    case MODE_STATE_USB1_VIDEO:
                    case MODE_STATE_USB2_VIDEO:
                    case MODE_STATE_JUKEBOX_VIDEO:
                        if(!isVideoInFG()) // modified by Sergey 15.09.2013 for ITS#190306
                        {
                            handleAVModeChange(pData->sMode, display);
                        }
						DisplayOSD(true);//added by edo.lee 2013.06.13
                        if(m_isNotifyGstAvailable == false && m_pVideoTimer->isActive() == false) {
                            // { modified by kihyung 2014.01.17 for ITS 0218420
                            /*
                            if(m_EngineMain.getOnBoot() == true) { // modified by kihyung 2013.4.4
                                DelayedPlay(false);
                            }
                            else {
                                // removed by Sergey 18.05.2013
                                MP_HIGH << "NotifyWhenGstPipelinesAvailable" << display << LOG_ENDL;
                                m_pVideoTimer->start();
                                m_EngineMain.setGstPipelineDisplay(display); // added by kihyung 2014.01.14 for ITS 0218420 
                                m_EngineMain.NotifyWhenGstPipelinesAvailable(display);
                            }
                            */
                            //m_EngineMain.GetNotifier()->setGstPipelineDisplay(display); // added by kihyung 2014.01.14 for ITS 0218420
                            DelayedPlay(false);
                            // } modified by kihyung 2014.01.17 for ITS 0218420
                        }
                        m_EngineMain.SetAVOffMode(false);
                        break;
                    //case MODE_STATE_NONE: // fall through
                    //    m_PassPlay = false;
                    case MODE_STATE_AVOFF: // fall through
                        m_EngineMain.SetAVOffMode(true); //modified by edo.lee 2013.04.02 ISV 77421
                        // { added by cychoi 2013.10.14 for ITS 195547 Display Title/Disc menu OSD on AV Off
                        if(m_EngineMain.GetLastPlayerMode() == APP_MEDIA_MODE_VIDEO)
                        {
                             m_DVDModeChanged = true;
                        }
                        // } added by cychoi 2013.10.14
                    case MODE_STATE_NONE: // fall through
                        m_PassPlay = false;
						m_pScreenManager[VIDEO_DISPLAY_FRONT]->setFocusActivefromEngineVideo( false); //added by edo.lee 2013.06.19
						m_pScreenManager[VIDEO_DISPLAY_REAR]->setFocusActivefromEngineVideo( false);//added by edo.lee 2013.06.19
                    default:		
                        showSubTitle(false); // added by kihyung 2013.07.10 for ITS 0173538 
                        // { added by kihyung 2013.4.1
                        if(m_pVideoTimer->isActive() == true) {
                            m_pVideoTimer->stop();
                        }
                        // } added by kihyung 2013.4.1
                        getEngineMain().setSubCreenOn(false ,getEngineMain().getSubCreenName()); //added by edo.lee 2013.08.22 ITS 0185735
                        
                        // removed by Sergey 28.05.2013
                        emit avModeChanged(MEDIA_SOURCE_MAX, true);
                        switchSource(MEDIA_SOURCE_MAX); // modified by Sergey 07.09.2013 for PRE_FG supoort
                        setPBCOff();
                        setMaintainPause(false); // added by Sergey 16.10.2013 for ITS#195185
                        break;
                    }
                }
                
                emit fgBgStateChanged(false);
#ifdef BOOT_LOADING_SCREEN
                if(m_EngineMain.GetLastPlayerMode() == APP_MEDIA_MODE_VIDEO && m_EngineMain.m_tBootInfo.bisReadyToPlay == false)
                {
                    MP_HIGH<<"ConnectAVP2UISHReadyToPlay - isReadytoPlay" << LOG_ENDL;
                    //m_EngineMain.m_tBootInfo.bisAVModeChange = true; //ys-20131010 TA scan finish after AV_Mode_change
                    m_EngineMain.setAVModeChange(true);
                    emit isReadytoPlay(); //ys-20131008 boot
                }
#endif
            }
            ret = true;
            break;
        }

        // { added by Sergey Vetugov for CR#13599
        case EVT_AUTO_PLAY_IN_BG:
        {
            MP_HIGH << "EVT_AUTO_PLAY_IN_BG" << LOG_ENDL;

            if (!aEvent.Data().isEmpty())
            {
                EVENT_AUTO_PLAY_BG_DATA_T* pData = (EVENT_AUTO_PLAY_BG_DATA_T*)aEvent.Data().data();
                if (pData)
                {
                    // { added by wspark 2012.10.15 for CAN message in BG mode.
                    APP_MEDIA_MODE_T notifier_mode = APP_MEDIA_MODE_MAX;
                    switch(pData->sMode)
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
                        notifier_mode = APP_MEDIA_MODE_AUDIO;
                        break;
                    case MODE_STATE_DISC_VIDEO:
                    case MODE_STATE_USB1_VIDEO:
                    case MODE_STATE_USB2_VIDEO:
                    case MODE_STATE_JUKEBOX_VIDEO:
                    case MODE_STATE_AUX1_VIDEO:
                    case MODE_STATE_AUX2_VIDEO:
                        notifier_mode = APP_MEDIA_MODE_VIDEO;
                        break;
                    default:
                        notifier_mode = APP_MEDIA_MODE_MAX;
                        break;
                    }
                    m_EngineMain.GetNotifier()->SetActiveMode( notifier_mode );
                    // } added by wspark

                    handleAVModeChange(pData->sMode, display);
                }
            }
            ret = true;
            break;
        }
        // } added by Sergey Vetugov for CR#13599
     
        //{ added by yongkyun.lee 20130322 for : NO CR .VR Auto Play
        case EVT_VR_NOTIFY:
        {
			//remove by edo.lee 2013.07.15 STATIC TEST
            //APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
            if(!IsPlaying())
                m_pController->play();
			//added by edo.lee 2013.04.15
			DisplayOSD(true);
			//m_EngineMain.GetNotifier()->DisplayVideoTrackOSD();
		
            break;
         }
        //} added by yongkyun.lee 20130322       
        case EVT_VR_COMMAND:
        {
            MP_LOG << "EVT_VR_COMMAND" << LOG_ENDL;
            if ( !aEvent.Data().isEmpty() )
            {
                EVENT_EVT_VR_COMMAND_DATA_T *eventData = (EVENT_EVT_VR_COMMAND_DATA_T*)aEvent.Data().data();     
                handleVRCommand(eventData, display);
            }
            ret = true;
            break;
        }

        case EVT_POST_EVENT:
        {
            switch(aEvent.GetCommand())
            {
            // { modified by Sergey 13.08.2013
            #if AVP_USE_NEW_BLEND_AFW
            case EVT_REQUEST_RENDER_ON:
            {
                MP_HIGH << "EVT_REQUEST_RENDER_ON" << "display =" << display << "actualDisp =" << actualDisp(display) << LOG_ENDL; // added by Sergey 05.11.2013 for ITS#212560
				//added by edo.lee 2013.11.15 ITS 0209530 
				if(getEngineMain().getIsNoneState()) 
				{
				    MP_LOG<<"return because non avp mode" << LOG_ENDL;
					break;
				}

                // Some APP requests to show video widget
                if(!getEngineMain().getCamInFG() || actualDisp(display) == DISPLAY_REAR)
                {
                    m_pController->showVideo(display);
                }
                m_pVideoTimer->start();
                MP_HIGH << "NotifyWhenGstPipelinesAvailable" << display << LOG_ENDL;
                m_EngineMain.GetNotifier()->setGstPipelineDisplay(display); // added by kihyung 2014.01.14 for ITS 0218420
                m_EngineMain.NotifyWhenGstPipelinesAvailable(display);

                break;
            }
            
            case EVT_REQUEST_RENDER_OFF:
            {
                MP_HIGH << "EVT_REQUEST_RENDER_OFF" << LOG_ENDL; // added by Sergey 05.11.2013 for ITS#212560
                // { modified by sjhyun 2013.11.05 for P2_JOINT_EVALUIATION
                if (!isVideoInFG())
                {
                    // Some APP requests to stop rendering
                    m_pController->clearDisplay(true);
                    m_EngineMain.setGstPipelineStatus(true);
					//added by edo.lee 2013.11.06 ITS 198890
					if(getIsShowLockoutView() != VIDEO_DISPLAY_MAX)
					{
					    m_bIsLaunchingScreenSettings = VIDEO_DISPLAY_MAX;//modified by edo.lee 2013.12.13 ITS 0211957
					}
                }
                // } modified by sjhyun
                break;
            }
            #endif
            // } modified by Sergey 13.08.2013
            
            // { added by ravikanth     - CR 11552, 11938
            // Handle DVD Settings item selected
            case EVT_VC_ITEM_SELECTED:
            {
                MP_LOG << "EVT_VC_ITEM_SELECTED" << LOG_ENDL;
                if(!aEvent.Data().isEmpty())
                {
                    emit vcItemSelected((VCItem*)aEvent.Data().data());
                }
                break;
            }
            case EVT_REQUEST_DVD_INFO:
            {
                AppMediaPlayer_Video_ControllerDVD *pDVD    = (AppMediaPlayer_Video_ControllerDVD*)m_pController->getController(MEDIA_SOURCE_DVDVIDEO);
                if(pDVD != NULL)
                {
                    pDVD->requestDefaultLanguagesSet(); // modified by ravikanth CR 15994 12-11-22
                }
                break;
            }
            // } added by ravikanth
            case EVT_VIDEOPLAY_STATE:
            {
                MP_LOG << "EVT_VIDEOPLAY_STATE" << LOG_ENDL;
                handleVideoPlayEvent(aEvent);
                break;
            }

            // { modified by Sergey 09.09.2013 for ITS#188283
            case EVT_PLAY_VIDEO:
            {
                if((APP_FMGR == aEvent.GetSource()) && (!aEvent.Data().isEmpty()))
                {
                    QString filePath = filePath.fromUtf8(aEvent.Data().data());
                    filePath.remove( "file://" );

                    MP_LOG << "[EVT_PLAY_VIDEO] filePath" << filePath << LOG_ENDL;

                    if (m_pController->getCurrentController() != NULL) // modified by Sergey 27.07.2013
                    {
                       if (filePath != ((AppMediaPlayer_Video_CModel_FS *)m_pController->getCurrentController()->getModel())->playingUrl())
                       {
                              PlayTrackFromFileManager(filePath, display); //  modified by Sergey 03.09.2013 for ITS#186054
                       }
                       else
                       {
                           m_isFMFile[display] = FM_FILE_SAME;

                           if(IsStopped() == true) // modified by kihyung 2013.08.24. Maintain a pause state
                               m_pController->play();
                       }
                    }
                    else
                    {
                       PlayTrackFromFileManager(filePath, display); //  modified by Sergey 03.09.2013 for ITS#186054
                    }
                    // modified by Dmitry 11.07.2013
                }

                break;
            }
            // } modified by Sergey 09.09.2013 for ITS#188283
            
            // { modified by Sergey for CR#16002
            case EVT_ASPECTRATIO_CHANGE:
            {
                MP_LOG << "EVT_ASPECTRATIO_CHANGE" << LOG_ENDL;
                handleAspectRatioEvent(aEvent);
                break;
            }
            // } modified by Sergey for CR#16002.
            // removed by Sergey for CR16250
            //added by edo.lee 2013.02.15
            case EVT_FORMAT_JUKEBOX_START:
			{
				MP_LOG << "from FMGR EVT_FORMAT_JUKEBOX_START" << LOG_ENDL;
				AppMediaPlayer_Video_ControllerFS* pJBCtl = static_cast<AppMediaPlayer_Video_ControllerFS *>(m_pController->getController(MEDIA_SOURCE_JUKEBOX)); // added by lssanh 2013.03.26 ISV77212
				m_formatStatus = true; 
				if (m_pController->getSource() == MEDIA_SOURCE_JUKEBOX)
				{
					m_pController->stop();
					pJBCtl->setPosition(0); // added by lssanh 2013.03.26 ISV77212
				}		
				getEngineMain().GetNotifier()->resetMediaEnabled(MEDIA_SOURCE_JUKEBOX);
				break;
            }		
			// added by edo.lee 2013.02.18
			case EVT_FORMAT_JUKEBOX_END:
		    {
		        if(m_formatStatus)
		        {
		            m_formatStatus = false;
		            emit notifyFormatEnded();
		        }
		        break;
		    }
			//added by edo.lee
                        // { modified by ravikanth 16-04-13
            case EVT_JUKEBOX_COPY_START:
             {			 
                // added by eugene.seo 2013.05.29			 	
			 	bool isVideo = *(bool *)aEvent.Data().data();
			 	MEDIA_SOURCE_T  nSource = m_pController->getSource();				

				if(isVideo && nSource == MEDIA_SOURCE_USB1)				
            		m_EngineMain.setCopyInProgress(true, 1);
				else if(isVideo && nSource == MEDIA_SOURCE_USB2)
					m_EngineMain.setCopyInProgress(true, 2);
				else
					m_EngineMain.setCopyInProgress(true, 0);
				// added by eugene.seo 2013.05.29
                break;
             }
             case EVT_JUKEBOX_COPY_END:
             {
                // modified by ravikanth 29-08-13 for ITS 0174301, 0187085
                //bool isVideo = *(bool *)aEvent.Data().data();
                //if(isVideo && isVideoInFG())
                //{
                //    emit copyCompleted();
                //}
                //m_EngineMain.setCopyInProgress(false, 0); // modified by eugene.seo 2013.05.29
                break;
             }
			 //added by edo.lee 2013.07.14
			 case EVT_FM_VIDEO_HIDE:
			 {
			    // { added by kihyung 2013.10.31 for ITS 0205241 
                MP_HIGH<<"EVT_FM_VIDEO_HIDE" << LOG_ENDL;
                
				if(isVideoInFG() == false) 
				{
					m_EngineMain.setPlayInfoDisplay(DISPLAY_FRONT_REAR);
                    if(m_EngineMain.isFrontLCDOff() == false && m_EngineMain.getisBLCall()==false && m_EngineMain.getCamInFG()==false) // added by edo.lee 2013.12.12 ITS 0214417
    				{    				   
    				    OSDManager::instance()->setFileManagerToBG();
                    }
					else if(m_EngineMain.isFrontLCDOff() && m_EngineMain.isACCOff()) 
                	{
  					   OSDManager::instance()->setFileManagerToBG();
					}
				}

                if(m_EngineMain.getCamInFG() == false && m_EngineMain.isFrontLCDOff() == false)
                {
                    MP_HIGH<<"EVT_FM_VIDEO_HIDE -> LIST VIEW FALSE" << LOG_ENDL;
                    setVideoListView(VIDEO_DISPLAY_MAX, false);
                }
                // } added by kihyung 2013.10.31 for ITS 0205241 
            }
			 //added by edo.lee 2013.07.14
              // } modified by ravikanth 16-04-13
            default: 
            {
                break; //make compiler happy
            }
        }
        break;
    }
    case EVT_SETTINGS_CHANGED:
    {
        if ( aEvent.Data().isEmpty() )
        {
            MP_LOG << "Settings interface: EVT_SETTINGS_CHANGED payload is empty"
                   << "if you're seeing this log, make sure not to waste time but"
                   << "notify AppSettings team." << LOG_ENDL;
            break;
        }

        MP_LOG << "EVT_SETTINGS_CHANGED" << LOG_ENDL;

        EVENT_SETTINGS_CHANGED_DATA_T *eventData = (EVENT_SETTINGS_CHANGED_DATA_T*)aEvent.Data().data();
        ret = handleSettingsCommand( eventData );
        break;
    }

    case EVT_FORMAT_JUKEBOX_START:
    {
        m_formatStatus = true; // added by wspark 2012.12.17 for ISV64433
        AppMediaPlayer_Video_ControllerFS* pJBCtl = static_cast<AppMediaPlayer_Video_ControllerFS *>(m_pController->getController(MEDIA_SOURCE_JUKEBOX)); // added by lssanh 2013.03.26 ISV77212
        MP_LOG<<"[EVT_FORMAT_JUKEBOX_START]" << LOG_ENDL;
        if (m_pController->getSource() == MEDIA_SOURCE_JUKEBOX)
        {
           m_pController->stop();
           pJBCtl->setPosition(0); // added by lssanh 2013.03.26 ISV77212
        }
        break;
    }
    // { added by wspark 2012.12.17 for ISV64433
    case EVT_FORMAT_JUKEBOX_END:
    {
        if(m_formatStatus)
        {
            m_formatStatus = false;
            emit notifyFormatEnded();
        }
        break;
    }
    // } added by wspark

	// removed by Sergey for ITS#181872 30.07.2013
    case EVT_TA_DAB_START: // added by kihyung 2013.08.01 for ITS 0174593
    case EVT_TA_RDS_START: // added by kihyung 2013.08.01 for ITS 0174593
    case EVT_CALL_START:
    {
        MP_HIGH << "[Video]EVT_CALL_START isPowerOff =" << getEngineMain().isPowerOff() << LOG_ENDL; // added by Sergey 11.10.2013 for ITS#194942
        //m_pController->pause(); remove by edo.lee 2013.06.27
		//added by edo.lee 2013.06.20
		//m_bPlayAfterBTCall = false;

		if(getEngineMain().isPowerOff() == true)
			setMaintainPause(true); // added by Sergey 11.10.2013 for ITS#194942
		
        //{ added by wonseok.heo for  ITS 178767 2013.07.10
        if(m_EngineMain.isPowerOff() == true || m_EngineMain.getOnBoot())//added by edo.lee 2013.09.23 ITS 0189921 
        {
            m_bPlayAfterBTCall = true;
            m_pController->pause();//added by taihyun.ahn 2013.12.12 ITS 213986
			// m_PassPlay = true;//added by edo.lee 2013.08.01 bt call on pwroff // removed by kihyung 2013.08.02 24.18 smoke test 
        }
        // } added by wonseok.heo for  ITS 178767 2013.07.10
        else if(IsPlaying() || m_pController->getLoadingStatus() || m_EngineMain.getSelectTuned()) // modified by taihyun.ahn 2013.10.02  for ITS 188100
        { 
            m_bPlayAfterBTCall = true;
            //{modified for its 229111
            VIDEO_DISPLAY_T vDisp = (display == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;

            if(!(getScreenManager(vDisp)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON))
            {
                m_pController->pause(); //added by edo.lee 2013.06.27
            }
            //{modified for its 229111
                        
            m_EngineMain.setPlayAfterBTCallState(true);//Added by taihyun.ahn 2013.10.02  for ITS 188100

            emit setPassPlayState(true); // added by sjhyun 2013.09.22 for ITS 190471
        }
        else if((m_pController->getPlaybackStatus() >= AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X) &&
                (m_pController->getPlaybackStatus() != AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_INVALID))	
		{
		    //{remove by edo.lee 2013.09.25
		    //if(m_EngineMain.GetViewR(DISPLAY_FRONT) != APP_MEDIA_MODE_MAX)
		    //{
		    //   m_bKeepFFREWOnBTCall = true;
		    //}
				
			// { added by Sergey 25.11.2013
                        //m_pController->cancel_ff_rew();// removed by taihyun.ahn 2013.12.06
                        m_pController->pause();
			// } added by Sergey 25.11.2013
			m_bPlayAfterBTCall = true;
		}
		else
		{
			m_bPlayAfterBTCall = false;
		}
        break;
    }

    case EVT_TA_DAB_END: // added by kihyung 2013.08.01 for ITS 0174593
    case EVT_TA_RDS_STOP: // added by kihyung 2013.08.01 for ITS 0174593
    case EVT_CALL_END:
    {
        MP_HIGH << "[Video]EVT_CALL_END"<< m_bPlayAfterBTCall<<m_EngineMain.getPlayOnBTCall() << LOG_ENDL;
        // { modified by wspark 2013.03.21 for ISV 75984
        //m_pController->play();
        // { modified by cychoi 2013.08.30 for ITS 187026 DVD Title/Disc Menu Loading Screen
        //m_bKeepFFREWOnBTCall = false;//remove by edo.lee 2013.09.25
        if(m_bPlayAfterBTCall && m_EngineMain.getPlayOnBTCall())//modify by edo.lee 2013.09.05
        {
            //m_EngineMain.setPlayOnBTCall();
            m_EngineMain.setIsTermOnTrackChange(true);
            // { modified by cychoi 2013.10.23 for ITS 196358 & 198062 Keep the play state on BT call if pause is forbidden.
            MEDIA_SOURCE_T source = GetVideoController()->getSource();
            if(source == MEDIA_SOURCE_DVDVIDEO ||
               source == MEDIA_SOURCE_DVDAUDIO ||
               source == MEDIA_SOURCE_VCD)
            {
                if(m_pController->getPlaybackStatus() != AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING)
                {
                    m_pController->play();
                }
            }
            else
            {
                //{added by taihyun.ahn 198515 2013.10.30
                if(m_pController->getPlaybackStatus() >= AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X &&
                   m_pController->getPlaybackStatus() < AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_4X)
                {
                    emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_RELEASED);
                    emit setDisplayOSD( true );
                }
                else if(m_pController->getPlaybackStatus() >= AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_4X)
                {
                    emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_RELEASED);
                    emit setDisplayOSD( true );
                }
                else
                {
                    //{modified for its 229111
                    VIDEO_DISPLAY_T vDisp = (display == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;

                    if(!(getScreenManager(vDisp)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON))
                    {
                        m_pController->play();
                    }
                    //}modified for its 229111
                }
                //}
            }
            // } modified by cychoi 2013.10.23
        }
		//}added by edo.lee 2013.09.13 ITS 0189021
        // } modified by cychoi 2013.08.30
        //Removed by Puneet on 2013.07.11 for ITS #0179376 , Moved to Engine Main.
        //m_EngineMain.setPlayOnBTCall(); //added by edo.lee 2013.06.27 for BT CALL
        // } modified by wspark
         m_EngineMain.setPlayAfterBTCallState(false);//Added by taihyun.ahn 2013.10.02  for ITS 188100
         setMaintainPause(false); // added by Sergey 11.10.2013 for ITS#194942
         m_EngineMain.setBTCallPowerOn(false);//Added by taihyun.ahn 2013.10.22
        break;
    }
    
	// { modified by Sergey 27.08.2013 for ITS#186787
    case EVT_SHOW_POPUP:
    {
        VIDEO_DISPLAY_T vDisp = (display == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR; // moved by cychoi 2014.02.20 for ITS 0226393 
        
        if(isVideoInFG())
        {
            if(getScreenManager(vDisp)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
                getScreenManager(vDisp)->popScreen();
			//added by edo.lee 2013.12.13 close popup
			else if( getScreenManager(vDisp)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON)
			{
			    AppMediaPlayer_Video_ScreenTextButtonPopup* popup = dynamic_cast<AppMediaPlayer_Video_ScreenTextButtonPopup*>(m_pScreenManager[vDisp]->topScreen());
				if(popup)// added by edo.lee 2014.01.09 STATE TEST
					popup->closePopupBySystemPopup();
			}
			//added by edo.lee 2013.12.13 

            deactivateFocus(display);
        }
        // { moved by cychoi 2014.02.20 for ITS 0226393 
        m_systemPopupDisplay = vDisp; // added by yungi 2014.01.23 for ITS 221843
        setSystemPopupShow(true); // modified by ravikanth 02-10-13 for ITS 0190988
        // } moved by cychoi 2014.02.20

        break;
    }

    case EVT_HIDE_POPUP:
    {
        // { modified by wspark 2014.02.15 for modifying focus disappear in playbackcontrol and modearea when pressing MEDIA HK 2times while there is system popup.
        if(isVideoInFG())
        {
            activateFocus(display);
        }
        VIDEO_DISPLAY_T vDisp = (display == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR; // added by yungi 2014.01.23 for ITS 221843
        m_systemPopupDisplay = vDisp; // added by yungi 2014.01.23 for ITS 221843
        setSystemPopupShow(false); // modified by ravikanth 02-10-13 for ITS 0190988
        // } modified by wspark

        break;
    }
    // } modified by Sergey 27.08.2013 for ITS#186787
    // { added by Ravi 30.10.2013 for ITS#194326
    case EVT_KEY_HU_TA:
    {
        popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_FULL);
        break;
    }
    // } added by Ravi 30.10.2013 for ITS#194326


    default:
        {
        MP_LOG << "Ignoring event" << aEvent.GetEventId() << LOG_ENDL;
        break;
        }
    } // end of switch( aEvent.GetEventId() )

    return ret;
}
// } modified by Sergey for CR#15775


// removed by Sergey 07.09.2013 for PRE_FG supoort


// } modified by edo.lee
bool AppMediaPlayer_EngineVideo::IsSourceAvailable( MEDIA_SOURCE_T source )
{
    return m_EngineMain.GetMediaManager()->IsSourceAvailable( source, MEDIA_TYPE_VIDEO );
}
void AppMediaPlayer_EngineVideo::ClearOSD() //added by edo.lee 2013.01.10
{
	//modified by edo.lee 2013.07.09
	m_EngineMain.GetNotifier()->ClearOSD(true);
	m_EngineMain.GetNotifier()->ClearOSD(false);
	/*
	MP_LOG << LOG_ENDL;
		//{modified by edo.lee 2013.03.11
	if(!m_EngineMain.getIsDiscEject())
	{
		//OSDview->setWindowTitle("@OP^CLEAR_AV"); // modified by edo.lee 2013.03.14
		OSDWriter::SendOSD("@OSD:FR:CLEAR_AV"); //modified by edo.lee 2013.04.10
		//QCoreApplication::flush(); //added by edo.lee 2013.01.16
	}
	else
	{
		m_EngineMain.setIsDiscEject(false);
	}
	//}modified by edo.lee 2013.03.11
	*/
}



// { modified by Sergey 07.09.2013 for PRE_FG supoort
void AppMediaPlayer_EngineVideo::ShowUI(DISPLAY_T display)
{
    MP_HIGH << "entry point =" << m_EntryPoint << " display =" << display << LOG_ENDL;


    // { added by Sergey 11.12.2013 for ITS#211252, 212879
    if(isTuneMode() && !isVideoInFG())
    {
        onTuneTimerExpired();
        showCurrentOSD();
    }
    // } added by Sergey 11.12.2013 for ITS#211252, 212879

    m_bVideoInFG[display] = true; // added by Sergey 10.05.2013

    // { modified by kihyung 2013.06.13 for ITS 0173713
    if(getIsShowLockoutView() == VIDEO_DISPLAY_FRONT && display == DISPLAY_FRONT ){//modified by edo.lee 2013.11.06 ITS 198890
        MP_LOG << "CASE1" << m_bIsLaunchingScreenSettings << LOG_ENDL;
    }
    else if(getIsShowLockoutView() == VIDEO_DISPLAY_REAR && display == DISPLAY_REAR){//modified by edo.lee 2013.11.06 ITS 198890
        MP_LOG << "CASE2" << m_bIsLaunchingScreenSettings << LOG_ENDL;        
    }
    else {
        MP_LOG << "CASE3" << m_bIsLaunchingScreenSettings << LOG_ENDL;   

        m_bIsLaunchingScreenSettings = VIDEO_DISPLAY_MAX;//modified by edo.lee 2013.12.13 ITS 0211957
        m_bIsLaunchingSoundSettings = false; // added by cychoi 2013.08.05 for ScreenOptions is opened when returned from SoundSetting
    }
    // } modified by kihyung 2013.06.13 for ITS 0173713    
   //remove by edo.lee 2013.11.02  ITS206061
    
	m_EngineMain.setIsFROnTune(false);//added by edo.lee 2013.03.25
	releaseTuneModeByMenu(); //  //added by edo.lee 2013.08.21 ITS 0185455 

	 // removed by Sergey 15.08.2013 for ITS#185556 

    if(m_EntryPoint <= eAPP_AVP_VIDEO_MODE_AUX) { //added by edo.lee 2013.03.13 except DRS Entry Point
	    m_currentScreen = display;
    }
    
	setStatusBarOnScreen(true, display == DISPLAY_FRONT ? true:false);//added by edo.lee 2013.03.14

    //added by edo.lee 2014.1.8 ITS 0217055 
	if(m_EngineMain.IsChangeToDriving() || m_EngineMain.IsChangeToParking() )
		CancelAllJogKey();
	//added by edo.lee 2014.1.8


    MEDIA_SOURCE_T source = mapEPToSource(m_EntryPoint);

    if(IsSourceAvailable(source))
    {
        switchScreenOnFG(source, display);
    }
    else
    {
        showEmptySourcePopup(source, display);
    }
    
    VPEnum::VDISP_T vDisp = (display == DISPLAY_FRONT) ? VPEnum::VDISP_FRONT : VPEnum::VDISP_REAR;
    showCaptions(QVariant(vDisp), true); // added by Sergey 12.10.2013 for ITS#195158

    activateFocus(display);  // added by Sergey 15.08.2013 for ITS#185556
    GetVideoController()->setBackground(false); // added by Sergey 24.10.2013 for ITS#87134
    if(!getFgState())
    {
        if(popupType(display) == POPUP_TXT_BTN_COPY_INPROGRESS) // modified by ravikanth for ITS 0217179 
            popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON); // modified for ITS 0216167
    }
}
// } modified by Sergey 07.09.2013 for PRE_FG supoort

// removed by Sergey 18.05.2013

//void AppMediaPlayer_EngineVideo::setGstPipelineStatus(bool bStop)
//{
//    m_EngineMain.setGstPipelineStatus(bStop);
//}

// removed by cychoi 2013.08.30 for ITS 187026 DVD Title/Disc Menu Loading Screen

// { modified by kihyung 2013.2.14
void AppMediaPlayer_EngineVideo::DelayedPlay(bool bFromAFW)
{
    MP_LOG << "bFromAFW():" << bFromAFW <<
              "getIsFgReceived():" << getIsFgReceived() <<
              "getPlaybackStatus():" << m_pController->getPlaybackStatus() <<
              "getCurrentScreen():" << getCurrentScreen() <<
              "IsSwapDCEnabled():" << m_EngineMain.IsSwapDCEnabled() <<
              "getPowerOffBTCall():" << m_EngineMain.getPowerOffBTCall() <<
              "getKeepPlayState():" << m_EngineMain.getKeepPlayState() << LOG_ENDL;
    
    MEDIA_SOURCE_T  source = GetVideoController()->getSource(); //added by sh.kim for ITS 0186812
        
    m_isNotifyGstAvailable = false;

    if(m_pVideoTimer->isActive()) {
        MP_LOG << "m_pVideoTimer->stop()" << LOG_ENDL;
        m_pVideoTimer->stop();
    }

    if(m_EngineMain.GetAVMediaMode(m_EngineMain.GetLastAVEntryPoint(false)) != APP_MEDIA_MODE_VIDEO) {
        MP_HIGH << "return... GetAVMediaMode() != APP_MEDIA_MODE_VIDEO" << LOG_ENDL;
        return;
    }
    
    if(getIsFgReceived() == true || bFromAFW == true ) // modified by Sergey 17.05.2013 for ISV#83170
    {
        if(m_EngineMain.getCamInFG() == true && isVideoInFG() == false) {
            MP_LOG << "usleep(500000)" << LOG_ENDL;
            usleep(500000);
            m_pController->requestVideoInfo();
        }

        if(getIsFgReceived() == true) {
            // } modified by eunhye 2013.03.19
            // { modified by wspark 2013.04.06 for ISV 78044
            //if(is_FullScreen)
            if(is_FullScreen && getFgState() == false)
            // } modified by wspark
            {
                m_pController->moveWidget(getCurrentScreen());
            }
            // } modified by eunhye 2013.03.19
            // { added by cychoi 2013.05.16 for ISV 78583 & ITS 167437
            else
            {
                SetVideoSize(getCurrentScreen()); // modified by cychoi 2013.07.03 for ITS 177711 
            }
            // } added by cychoi 2013.05.16

			// removed by Sergey 31.07.2013 
        }

        m_isFgReceived = false;
        
        if(m_nLastAVModeEvent == MODE_STATE_AUX1_VIDEO || m_nLastAVModeEvent == MODE_STATE_AUX2_VIDEO) 
        {
            m_pController->playAux();
        }
		//modified by edo.lee 2013.08.21 bt call on pwroff
		else if(/*m_EngineMain.getPowerOffBTCall() &&*/ m_EngineMain.getKeepPlayState() ) // modified by Sergey 28.09.2013 for ITS#192062
		{
		    m_pController->play(); 
            MP_HIGH << m_pController->getPlaybackStatus() << LOG_ENDL;
        }
		//modified by edo.lee 2013.08.21 bt call on pwroff
        // { modified by kihyung 2013.08.05 24.22 smoke test 
        /*
        else if(m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_STOPPED && !m_PassPlay|| //added by edo.lee 2013.08.01 bt call on pwroff
                (m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED && !m_PassPlay) ||
                (m_pController->getPlaybackStatus() != AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING && !m_PassPlay) || 
                m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_INVALID)
        */
        else if((m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_STOPPED && m_EngineMain.getPowerOffBTCall() == false) || 
                (m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED  && m_PassPlay == false) ||
                (m_pController->getPlaybackStatus() != AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING && m_PassPlay == false) || 
                (m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_INVALID))
        // } modified by kihyung 2013.08.05 24.22 smoke test 
        {
            //bool            bSetSubtitle = false;
            //            MEDIA_SOURCE_T  source = GetVideoController()->getSource(); //added(modify) by sh.kim for ITS 184632
            
            MP_LOG << "source" << source << LOG_ENDL;
            
            m_PassPlay = false;// added by yongkyun.lee 20130310 for : NO CR Play and Puase
            
            MP_HIGH << " keepPlay =" << m_EngineMain.getKeepPlayState() << "isPlay =" << IsPlaying() << LOG_ENDL; // added by Sergey 11.10.2013 for ITS#194942

            // { commented by Sergey 16.10.2013 for ITS#195185
//            if(source == MEDIA_SOURCE_JUKEBOX) {
//                if(m_EngineMain.isTAScanComplete(eJukeBox) == true) {
//				  // { modified by edo.lee 2013.07.30 bt call play state
//					if(m_EngineMain.getKeepPlayState()){
//						IsPlaying() ? m_pController->play() : m_pController->pause();
//					}else{
//                        m_pController->play();
//                        bSetSubtitle = true;
//					}
//                }else{
//                    m_bJukeboxDelayedPlay = true;
//                }
//            }
//            else if(source == MEDIA_SOURCE_USB1) {
//                if(m_EngineMain.isTAScanComplete(eUSB_FRONT) == true) {
//					if(m_EngineMain.getKeepPlayState()){
//						IsPlaying() ? m_pController->play() : m_pController->pause();
//					}else{
//                        m_pController->play();
//                        bSetSubtitle = true;
//					}
//                }else{
//                    m_bUSB1DelayedPlay = true;
//                }
//            }
//            else if(source == MEDIA_SOURCE_USB2) {
//                if(m_EngineMain.isTAScanComplete(eUSB_REAR) == true) {
//					if(m_EngineMain.getKeepPlayState()){
//						IsPlaying() ? m_pController->play() : m_pController->pause();
//					}else{
//                        m_pController->play();
//                        bSetSubtitle = true;
//					}
//                }else{
//                    m_bUSB2DelayedPlay = true;
//                }
//            }
// } commented by Sergey 16.10.2013 for ITS#195185
            //added(delete) by shkim for Media Menu-> DVD Insert -> Cam On flickering Issue
            //added by shkim for ITS 0186812
//            else if(source == MEDIA_SOURCE_DVDVIDEO){

//                if(bFromAFW){
//                    MP_LOG << "clear Display when HandleGstPipelinesAvailable:: DVD " << LOG_ENDL;
//                    m_pController->clearDisplay(false);
//                }
//                if(m_EngineMain.getKeepPlayState()){
//                    IsPlaying() ? m_pController->play() : m_pController->pause();
//                }else{
//                    m_pController->play();
//                }
//            }//added by shkim for ITS 0186812

			if (!GetVideoController()->isFSSource(source)) // modified by Sergey 16.10.2013 for ITS#195185
			{ 
				if(m_EngineMain.getKeepPlayState()){
					IsPlaying() ? m_pController->play() : m_pController->pause();	
				}else{
	                m_pController->play();            
				}
            }
			//getEngineMain().resetKeepPlayState();//remove by edo.lee 2013.08.01 bt call on pwroff
			// } modified by edo.lee 2013.07.30 bt call play state
			// { modified by Sergey 09.09.2013 for ITS#185543
			// { commented by Sergey 16.10.2013 for ITS#195185
//            if(bSetSubtitle == true)
//            {
//                AppMediaPlayer_Video_ControllerFS *pControllerFS = static_cast<AppMediaPlayer_Video_ControllerFS*>(m_pController->getController(source));
//                AppMediaPlayer_Video_CModel_FS    *pModelFS = qobject_cast<AppMediaPlayer_Video_CModel_FS*>(pControllerFS->getModel());

//                pControllerFS->enableSubTitle(pModelFS->captionEnable());
//                pControllerFS->setAudioLanguage(pModelFS->getCurrentStreamId());
//            }
			// } commented by Sergey 16.10.2013 for ITS#195185
            // } modified by Sergey 09.09.2013 for ITS#185543
        }
        //added(delete) by shkim for Media Menu-> DVD Insert -> Cam On flickering Issue
        //added by shkim for ITS 0186812
//        else{
//            if(source == MEDIA_SOURCE_DVDVIDEO && bFromAFW){
//                MP_LOG << "clear Display when HandleGstPipelinesAvailable:: DVD " << LOG_ENDL;
//                m_pController->clearDisplay(false);
//            }
//        }
        //added by shkim for ITS 0186812
        /* remove by edo.lee 2013.08.21 
        else {
			if(m_EngineMain.getPowerOffBTCall() && m_EngineMain.getKeepPlayState())//modified by edo.lee 2013.08.01 bt call on pwroff
				m_pController->play();  //modified by edo.lee 2013.08.01 bt call on pwroff
            MP_HIGH << m_pController->getPlaybackStatus() << LOG_ENDL;
        }*/
        // removed by Sergey 18.05.2013
    }
    // removed by Sergey 18.05.2013
}
// } modified by kihyung 2013.2.14

// { added by kihyung 2013.1.30
void AppMediaPlayer_EngineVideo::setPBCOff()  
{
    DeckEngineerMode  *pDeckEngine = getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    EDeckInit1Setting  tSetting;
    pDeckEngine->GetInitSetting1(tSetting);

    MP_LOG << tSetting.m_nPBC << LOG_ENDL;
    
    if(tSetting.m_nPBC == 0) {
        m_pController->stop(); // added by shkim 2013.08.27 for ITS 186402
        AppMediaPlayer_Video_ControllerVCD *pVCD = (AppMediaPlayer_Video_ControllerVCD*)m_pController->getController(MEDIA_SOURCE_VCD);
        pVCD->setPBC(false);
        pVCD->setPbcMenuDisplayed(false); // added by cychoi 2013.11.10 for ITS 206869 Pop PBC Menu Screen on PBCOff
    }
}
// } added by kihyung 2013.1.30

// { added by cychoi 2013.12.19 for ITS 216591 close VCD loading screen when changed play position
bool AppMediaPlayer_EngineVideo::isPBCOff()  
{
    DeckEngineerMode  *pDeckEngine = getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    EDeckInit1Setting  tSetting;
    pDeckEngine->GetInitSetting1(tSetting);

    MP_LOG << tSetting.m_nPBC << LOG_ENDL;
    
    return (tSetting.m_nPBC != 0);
}
// } added by cychoi 2013.12.19

void AppMediaPlayer_EngineVideo::handleDefaultEntryPoint()
{
    MEDIA_SOURCE_T source = MEDIA_SOURCE_MAX;

    if(m_pController->getSource() != MEDIA_SOURCE_MAX)
        source = m_pController->getSource();
    else
        source = m_pController->getLastSource();

    if ( source == MEDIA_SOURCE_MAX )
        emit avModeChanged( MEDIA_SOURCE_JUKEBOX, true );
    else
        emit avModeChanged( source, true );
}

// { modified by Sergey for ITS#181872 02.08.2013
void AppMediaPlayer_EngineVideo::HideUI(DISPLAY_T display)
{    
    MP_HIGH << display << " START" << LOG_ENDL;

    if(m_pController->isSeekMode())
    {
        m_pController->cancel_ff_rew();
    }

	deactivateFocus(display);  // added by Sergey 15.08.2013 for ITS#185556 
    
	// { added by Sergey 12.10.2013 for ITS#195158
	VPEnum::VDISP_T vDisp = (display == DISPLAY_FRONT) ? VPEnum::VDISP_FRONT : VPEnum::VDISP_REAR;
	showCaptions(QVariant(vDisp), false);
	repaintUI(display); // added by Sergey 18.10.2013 for safe ui update
    // } added by Sergey 12.10.2013 for ITS#195158

	if(isVideoListView() == VIDEO_DISPLAY_MAX)//added  by edo.lee 2013.07.14
		m_EngineMain.setPlayInfoDisplay(DISPLAY_FRONT_REAR);//added by edo.lee 2013.04.26

	VIDEO_DISPLAY_T disp = VIDEO_DISPLAY_FRONT;
    if(display == DISPLAY_FRONT)
		disp = VIDEO_DISPLAY_FRONT;
	else
		disp = VIDEO_DISPLAY_REAR;

	// removed by Sergey 15.08.2013 for ITS#185556 

	// { added by Sergey 11.12.2013 for ITS#211252, 212879
	DISPLAY_T otherDisp = (display == DISPLAY_FRONT) ? DISPLAY_REAR : DISPLAY_FRONT;

		if(isTuneMode() && !isVideoInFG(otherDisp) && m_EngineMain.getCloneState() == CLONE_OFF)
		{
			onTuneTimerExpired();
			showCurrentOSD();
		}
		// } added by Sergey 11.12.2013 for ITS#211252, 212879

	//modified by edo.lee 2013.09.23 ITS 0189923
	if(isVideoListView() != VIDEO_DISPLAY_MAX && isVideoInFG(DISPLAY_FRONT) 
		&&getIsShowLockoutView() == VIDEO_DISPLAY_FRONT&&  m_EngineMain.GetRearView() == APP_MEDIA_MODE_VIDEO)
		OSDManager::instance()->setFileManagerScreen(DISPLAY_FRONT);
	else if(isVideoListView() != VIDEO_DISPLAY_MAX && getIsShowLockoutView() == VIDEO_DISPLAY_MAX)
		OSDManager::instance()->setFileManagerScreen(display);
	//modified by edo.lee 2013.09.23 
	setStatusBarOnScreen(true, display == DISPLAY_FRONT ? true:false);//added by edo.lee 2013.03.14
	m_pScreenManager[disp]->fgBgStateChange(false);

	// moved by Sergey 27.11.2013 for ITS#211252, 212879
    if(m_bIsLaunchingScreenSettings == VIDEO_DISPLAY_MAX && !m_bVideoInFG[otherDisp])//modified by edo.lee 2013.12.13 ITS 0211957
    {
    	// { added by Sergey 24.10.2013 for ITS#87134
        GetVideoController()->setBackground(true);
        repaintUI(display);
        // } added by Sergey 24.10.2013 for ITS#87134

        m_pController->clearDisplay(true);
        // { modified by kihyung 2014.01.14 for ITS 0218420 
        // m_EngineMain.setGstPipelineStatus(true);
        if(getIsShowLockoutView() == VIDEO_DISPLAY_MAX)
        {
            m_EngineMain.setGstPipelineStatus(true);
        }
        // } modified by kihyung 2014.01.14 for ITS 0218420 
    }
    // { added by cychoi 2013.10.11 for ITS 195176 Video Size on Display Setting
    else if(m_bIsLaunchingScreenSettings == disp)//modified by edo.lee 2013.12.13 ITS 0211957
    {
        SetVideoSize(display);
    }
    // } added by cychoi 2013.10.11
    
//	m_pScreenManager[disp]->setIsLockoutMode(false);  // commented by Sergey 15.09.2013 for ITS#190306
// moved by Sergey 27.11.2013 for ITS#211252, 212879
	
	m_bVideoInFG[display] = false;  // added by Sergey 15.08.2013 for ITS#185556
	m_isFMFile[display] = FM_FILE_MAX; 	// modified by Sergey 09.09.2013 for ITS#188283
	
    MP_HIGH << "END" << LOG_ENDL;
}
// } modified by Sergey for ITS#181872 02.08.2013


 // removed by Sergey 15.08.2013 for ITS#185556 


// modified by Sergey for CR#15775
/*
void AppMediaPlayer_EngineVideo::onShowLockoutView(VIDEO_DISPLAY_T show)
{
	MP_LOG<<show << LOG_ENDL;
	m_isShowLockoutView = show; //remove by edo.lee 2013.03.13
}
*/

// added by edo.lee 2013.03.13
void AppMediaPlayer_EngineVideo::setIsShowLockoutView(DISPLAY_T disp, bool lockout)
{
	MP_LOG<<disp<<lockout << LOG_ENDL;
	m_pScreenManager[disp == DISPLAY_FRONT? VIDEO_DISPLAY_FRONT:VIDEO_DISPLAY_REAR]->setIsLockoutMode(lockout);
}
VIDEO_DISPLAY_T AppMediaPlayer_EngineVideo::getIsShowLockoutView()
{
	VIDEO_DISPLAY_T res = VIDEO_DISPLAY_MAX;

	if(m_pScreenManager[VIDEO_DISPLAY_FRONT]->getIsLockoutMode())
		res = VIDEO_DISPLAY_FRONT;
	else if(m_pScreenManager[VIDEO_DISPLAY_REAR]->getIsLockoutMode())
		res = VIDEO_DISPLAY_REAR;

        //MP_LOG<<res << LOG_ENDL; //removed - too many logs
	return res;	
}
// added by edo.lee 2013.03.13

void AppMediaPlayer_EngineVideo::createPlaylistFromTA(MEDIA_SOURCE_T source)
{
    MP_LOG << source << LOG_ENDL;
    AppMediaPlayer_Video_ControllerFS *pController = (AppMediaPlayer_Video_ControllerFS*)m_pController->getController(source);
    if(pController) {
        QString mediaRoot;
        if(source != MEDIA_SOURCE_JUKEBOX) {
            mediaRoot = GetUSBPath(source);
        }
        else {
            mediaRoot = pController->getRootPath();
        }

        pController->requestAllPlaylist(REQUID_RESCAN_ID);
    }
}

// { modified by edo.lee 2013.02.04
void AppMediaPlayer_EngineVideo::HideScreen(DISPLAY_T display)
{
	VIDEO_DISPLAY_T dispType;
	if(display == DISPLAY_FRONT )
		dispType = VIDEO_DISPLAY_FRONT;
	else
		dispType = VIDEO_DISPLAY_REAR;

	// removed by Sergey 24.07.2013

    //added by shkim ITS 178085
    if(display == DISPLAY_FRONT){
        if(m_EngineMain.GetFrontMode() == APP_MEDIA_MODE_PHOTO_FRONT && m_nLastAVModeEvent == MODE_STATE_DISC_VIDEO
            && m_EngineMain.getCamInFG() == true && m_EngineMain.GetRearMode() != APP_MEDIA_MODE_VIDEO){
            MP_LOG<<"Clear Display at Front Photo DRS and Insert DVD -> Camera On :: " << LOG_ENDL;
            m_pController->clearDisplay(true); // modified by Sergey for ITS#181872 30.07.2013
        }
    }
    //added by shkim ITS 178085

    // { added by cychoi 2013.08.05 for ScreenOptions is opened when returned from SoundSetting
    if(m_bIsLaunchingSoundSettings)
    {
        if(getScreenManager(dispType)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
        {
            getScreenManager(dispType)->popScreen(QVariant());
        }
        popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON); // added by cychoi 2013.03.19 for ITS 230152 Popup close when opened SoundSetting
    }
    // } added by cychoi 2013.08.05
}
// } modified by edo.lee 

// modified by kihyung 2013.08.09 for ITS 0183077
void AppMediaPlayer_EngineVideo::SaveSettings(bool bSync)
{
    AppMediaPlayer_SettingsStorage::GetInstance()->SaveVideoSettings(m_pController);
    AppMediaPlayer_SettingsStorage::GetInstance()->SaveUSBVideoExtended(m_pController, bSync); // modified by kihyung 2013.07.20 for ITS 0179774
}

void AppMediaPlayer_EngineVideo::LoadSettings()
{
    MP_LOG << LOG_ENDL;
    AppMediaPlayer_SettingsStorage::GetInstance()->LoadVideoData(m_pController);
    AppMediaPlayer_SettingsStorage::GetInstance()->LoadUSBVideoExtended(m_pController); // modified by kihyung 2013.07.20 for ITS 0179774
}


void AppMediaPlayer_EngineVideo::Restore(MODE_STATE_T mode)
{
    MP_LOG << LOG_ENDL;
    handleAVModeChange( mode , DISPLAY_FRONT );
    handleAVModeChange( mode , DISPLAY_REAR);
}

// { modified by Sergey for CR#15775
void AppMediaPlayer_EngineVideo::onDeviceAttached( MEDIA_SOURCE_T source,
                                                   MEDIA_TYPE_T type,
                                                   bool autoplay )
{
    if( type != MEDIA_TYPE_VIDEO )
        return;

    Q_UNUSED(autoplay);
    MP_LOG << "TASCAN -> [source] =" << source << LOG_ENDL;

    getEngineMain().setSubCreenOn(false ,getEngineMain().getSubCreenName()); //added by edo.lee 2013.09.21 ITS 0190776
    //{ modified by kihyung 2013.07.19 for ITS 0179774 
    if(source == MEDIA_SOURCE_USB1) {
        m_pController->handleUSBPluggedIn(m_EngineMain.GetMediaManager()->GetMountPath(source), false, eUSB_PORT1, 
                                          m_EngineMain.GetMediaManager()->GetID(source));
        emit deviceAttached(source, false);
    }
    else if(source == MEDIA_SOURCE_USB2) {
        m_pController->handleUSBPluggedIn(m_EngineMain.GetMediaManager()->GetMountPath(source), false, eUSB_PORT2, 
                                          m_EngineMain.GetMediaManager()->GetID(source));
        emit deviceAttached(source, false);
    }    
    //{ added by hyochang.ryu 20130709
    else if(source == MEDIA_SOURCE_AUX1 || source == MEDIA_SOURCE_AUX2) {
        emit deviceAttached(source, false);
        //m_pController->playAux();	//deleted by hyochang.ryu 20130826 for ITS186391
    }
    //} added by hyochang.ryu 20130709
    else {
        emit deviceAttached(source, false);
    }
    //} modified by kihyung 2013.07.19 for ITS 0179774
}

void AppMediaPlayer_EngineVideo::onDeviceRemoved( MEDIA_SOURCE_T source,
                                                  MEDIA_TYPE_T type )
{
	// { modified by kihyung 2013.07.10 for ITS 0173538 
    // Ignore everything we cannot want to play
    if( type != MEDIA_TYPE_VIDEO )
        return;

    // { removed by kihyung 2013.09.15 for ISV 91113
    /*
    // modified by Dmitry 18.07.13 for ITS0180717
    if (source == m_pController->getSource())
    {
       if (showSubTitle(false)) unblockUI();
    }
    // modified by Dmitry 18.07.13 for ITS0180717
    */
    // } removed by kihyung 2013.09.15 for ISV 91113
    //added by edo.lee 2013.09.21 ITS 0190776
    if(GetVideoController()->getSource() == source)
    	getEngineMain().setSubCreenOn(false ,getEngineMain().getSubCreenName()); 
	//added by edo.lee 2013.09.21
    MP_LOG << "[source] =" << source << LOG_ENDL;
    emit deviceRemoved(source);

    if(source == MEDIA_SOURCE_VCD)
    {
        AppMediaPlayer_Video_ControllerVCD *pVCD = (AppMediaPlayer_Video_ControllerVCD*)m_pController->getController(MEDIA_SOURCE_VCD);
        pVCD->setPbcMenuDisplayed(false);
    }
    // { added by kihyung 2013.07.19 for ITS 0179774 
    else if(source == MEDIA_SOURCE_USB1 || source == MEDIA_SOURCE_USB2)
    {
        m_pController->handleUSBPluggedOut(source);
    }
    // } added by kihyung 2013.07.19 for ITS 0179774     
    // } modified by kihyung 2013.07.10 for ITS 0173538 
}

void AppMediaPlayer_EngineVideo::onDeviceChanged(int aSource)
{
   Q_UNUSED(aSource);
   // removed by Sergey 31.07.2013
}


// This slot will be invoked once the screen manager gets nothing
// todo, e.g. the last screen is requested to be hidden.
void AppMediaPlayer_EngineVideo::onHideUI()
{
    MP_LOG << LOG_ENDL;

    // { removed by kihyung 2013.4.1
    /*
    // { modified by kihyung 2013.1.21 for ISV 70412
    // HideUI();
    // { modified by kihyung 2013.2.14
    // #if (USE_AFW_GST_AVAILABLE == 0)
    if(m_pVideoTimer->isActive()) {
        MP_LOG << "CNTKH m_pVideoTimer->stop()" << LOG_ENDL;
        m_pVideoTimer->stop();
    }
    // #endif
    // } modified by kihyung 2013.2.14
    */
    // } removed by kihyung 2013.4.1
    
    // removed by Sergey 18.05.2013

    if(m_pController->isSeekMode())
    {
        m_pController->cancel_ff_rew();
    }

    //getScreenManager()->clearStack(); // remove by edo.lee 2013.02.04
    //getScreenManager()->setJogSource( NULL ); //remove by edo.lee 2013.02.04
    
    // } modified by kihyung 2013.1.21 for ISV 70412

    // { removed by kihyung 2013.1.9 for ISV 69281
    // Event event( EVT_KEY_SOFT_BACK, m_EngineMain.GetThisAppId(), APP_NONE );
    // emit NotifyUISH (event);
    // } removed by kihyung 2013.1.9 for ISV 69281
}

// modified by Dmitry 25.07.13
void AppMediaPlayer_EngineVideo::launchFileManager( MEDIA_SOURCE_T mediaSource,
                                                    QString fileName,
                                                    QString folderPath,
                                                    int display) //added by edo.lee 2013.05.02
{
    AppFileManagerStartParameter fmParams;

    MP_LOG<<" filename:"<<fileName<<" folderPath"<<folderPath << "VIDEO_DISPLAY_T"<<display << LOG_ENDL;

    snprintf(fmParams.currentPlayFileName, sizeof(fmParams.currentPlayFileName), fileName.toUtf8().constData());
    snprintf(fmParams.folderPath, sizeof(fmParams.folderPath), folderPath.toUtf8().constData());

    fmParams.copyState = m_isCopy; // addd by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list 

    //added by edo.lee 2013.05.02
    DISPLAY_T disp = display == VIDEO_DISPLAY_FRONT ? DISPLAY_FRONT :DISPLAY_REAR ;
    //added by edo.lee 2013.06.09
	// added VIDEO_DISPLAY_FRONT check by ravikanth 20-08-13 for ISV 88689
    if((getIsShowLockoutView() == VIDEO_DISPLAY_REAR || getIsShowLockoutView() == VIDEO_DISPLAY_FRONT ) && m_EngineMain.GetFrontView() == APP_MEDIA_MODE_VIDEO )//modified by edo.lee 2013.06.27
       disp = DISPLAY_FRONT;

    APP_EVENT_DATA_BASE_T pEventData;
    pEventData.sIsJogEnabled = getScreenManager(VIDEO_DISPLAY_FRONT)->isFocusVisible();// { modified by edo.lee 2013.02.04

    MP_LOG << "FM (video) was launched with focus " << getScreenManager(VIDEO_DISPLAY_FRONT)->isFocusVisible() << LOG_ENDL; //  modified by edo.lee 2013.02.04

// removed by Dmitry 03.10.13 for ITS0187937

    Event fmstart( EVT_KEY_SOFT_FMGR, m_EngineMain.GetThisAppId(), APP_NONE,
                   QByteArray((char *)&pEventData, sizeof(pEventData))); // modified by lssanh 2013.02.21 IPC payload
    emit NotifyUISH( fmstart ,disp);//modified by edo.lee 2013.03.13

    fmParams.startState = (mediaSource != MEDIA_SOURCE_JUKEBOX) ? eStartUsbVideo : eStartJukeVideo;
	//added by edo.lee 2013.11.11 ITS 0208062 
	if((getIsShowLockoutView() == VIDEO_DISPLAY_REAR ) && !isVideoInFG(DISPLAY_FRONT)&& m_bIsLaunchingScreenSettings == VIDEO_DISPLAY_FRONT)//modified by edo.lee 2013.12.13 ITS 0211957
       disp = DISPLAY_FRONT;
	//added by edo.lee 2013.11.11 ITS 0208062 

    Event fmstate( EVT_POST_EVENT,
                   m_EngineMain.GetThisAppId(),
                   APP_FMGR,
                   EVT_FM_LAUNCH_STATE,
                   QByteArray((char *)&fmParams, sizeof(fmParams))); // modified by lssanh 2013.02.21 IPC payload
    emit NotifyUISH( fmstate ,disp);//modified by edo.lee 2013.03.13

    // } modified by Sergey 25.04.2013 for fast FM launch

    m_isCopy = false; // addd by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list 
}
// modified by Dmitry 25.07.13

// { modified by edo.lee 2013.02.04
void AppMediaPlayer_EngineVideo::handleVRCommand(EVENT_EVT_VR_COMMAND_DATA_T *eventData, DISPLAY_T display)
{
   switch(eventData->commandId)
   {
       case AV_MODE_CHANGE:
       {
	   	  DisplayOSD(true); //added by edo.lee 2013.01.14	
	   	  int disp = display == DISPLAY_FRONT ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;
          switch(eventData->sVRPayload.data[0])
          {
              case AV_MODE_JUKEBOX_VIDEO:
                 getScreenManager(disp)->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_JUKEBOX_PLAYBACK);
                 break;

              case AV_MODE_USB_VIDEO:
                 getScreenManager(disp)->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_USB_PLAYBACK);
                 break;

              case AV_MODE_CD_DVD:
                 getScreenManager(disp)->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK);
                 break;

              case AV_MODE_CD_VCD:
                 getScreenManager(disp)->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK);
                 break;

              case AV_MODE_AUX:
                 getScreenManager(disp)->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_AUX1_PLAYBACK);
                 break;

              default:
                  break;
          }
          break;
       } // end of AV_MODE_CHANGE

       default: // make compiler happy
           break;
   }
}

MEDIA_SOURCE_T AppMediaPlayer_EngineVideo::getMediaSourceFromDRS(MODE_STATE_T mode)
{
	MP_LOG<< "mode = " << mode << LOG_ENDL;
	MEDIA_SOURCE_T source = MEDIA_SOURCE_MAX;
	switch( mode )
    {
        case MODE_STATE_JUKEBOX_VIDEO:
        {
            source = MEDIA_SOURCE_JUKEBOX;
            break;
        }
        case MODE_STATE_USB1_VIDEO:
        {
            source = MEDIA_SOURCE_USB1;
            break;
        }
        case MODE_STATE_USB2_VIDEO:
        {
            source = MEDIA_SOURCE_USB2;
            break;
        }
        case MODE_STATE_DISC_VIDEO:
        {
            if( m_EngineMain.GetMediaManager()->IsSourceAvailable(MEDIA_SOURCE_DVDVIDEO,
                                                                  MEDIA_TYPE_VIDEO) )
            {
                source = MEDIA_SOURCE_DVDVIDEO;
            }

            if( m_EngineMain.GetMediaManager()->IsSourceAvailable(MEDIA_SOURCE_VCD,
                                                                  MEDIA_TYPE_VIDEO) )
            {
                source = MEDIA_SOURCE_VCD;
            }

            break;
        }
        case MODE_STATE_AUX1_VIDEO:
        {
            source = MEDIA_SOURCE_AUX1;
            break;
        }
        case MODE_STATE_AUX2_VIDEO:
        {
            source = MEDIA_SOURCE_AUX2;
            break;
        }
		 default:
            source = MEDIA_SOURCE_MAX;
            break;
	}
	return source;
}
//} modified by edo.lee 2013.02.04



// { modified by Sergey 07.09.2013 for PRE_FG supoort
void AppMediaPlayer_EngineVideo::handleAVModeChange(MODE_STATE_T mode, DISPLAY_T display)
{
    MEDIA_SOURCE_T source = mapModeStateToSource(mode);

    preSourceSwitchProcess(source, display);

    switchSource(source);
}
// } modified by Sergey 07.09.2013 for PRE_FG supoort



// { added by lssanh 2012.08.24 for audio path
void
AppMediaPlayer_EngineVideo::NofityAudioPathChanged (MODE_STATE_T mode)
{
     // { add by yongkyun.lee@lge.com  2012.10.30 : : CR 13733  : Sound Path  
    MP_LOG <<  "NofityAudioPathChanged Send Sound path to UISH" << mode << LOG_ENDL;
    // } add by yongkyun.lee@lge.com
//{Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side
//     // { add by yongkyun.lee@lge.com  2012.09.25 : : CR 13733  : Sound Path
//    if( !m_TabClicked)
//    {
//        return;
//    }
//    m_TabClicked=false;
//    // } add by yongkyun.lee@lge.com
//}Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side
    EVENT_AUDIO_PATH_REQUEST_DATA message = { { APP_ALL, EVT_REQUEST_AUDIO_PATH, 0, 0 }, mode };

    message.header.dataSize = sizeof( message );
    Event event( EVT_REQUEST_AUDIO_PATH, APP_AVP,
                     APP_UI_MANAGER, QByteArray( (char*)&message, sizeof( message ) ) ); // modified by lssanh 2013.02.21 IPC payload

    //{ modified by yongkyun.lee 2013-07-16 for : ITS 175107
    //emit NotifyUISH( event );
    m_EngineMain.NotifyUISHMute(event);
    //} modified by yongkyun.lee 2013-07-16 
    
}
// } added by lssanh

//{ add by lssanh 2012.09.04 for ITS 136354 
QString AppMediaPlayer_EngineVideo::makeElidedString( const QString &fullString,
                                                      const QString &fontFamily,
                                                      int pixelSize, int maxWidth,
                                                      Qt::TextElideMode elideMode)
{
    QFont mFont;
    mFont.setFamily(fontFamily);
    //mFont.setPixelSize(pixelSize);
    mFont.setPointSize(pixelSize);	//modified by aettie.ji 2012.11.28 for uxlaunch update

    QFontMetrics fontMetrics(mFont);

    return fontMetrics.elidedText(fullString, elideMode, maxWidth);
}
//} add by lssanh 2012.09.04 for ITS 136354 

void AppMediaPlayer_EngineVideo::PlayTrackFromFileManager( QString filePath, DISPLAY_T display ) 	// modified by Sergey 03.09.2013 for ITS#186054
{
   /* FileManager sends us file to play then launches us. So here we shouldn't
    * change screen as it will be done by EVT_REQUEST_FG.
    */
   MP_LOG << "filePath: " << filePath << LOG_ENDL;

   AppMediaPlayer_Video_ControllerFS* pUsb1Ctl = static_cast<AppMediaPlayer_Video_ControllerFS *>(m_pController->getController(MEDIA_SOURCE_USB1));
   AppMediaPlayer_Video_ControllerFS* pUsb2Ctl = static_cast<AppMediaPlayer_Video_ControllerFS *>(m_pController->getController(MEDIA_SOURCE_USB2));
   AppMediaPlayer_Video_ControllerFS* pJBCtl = static_cast<AppMediaPlayer_Video_ControllerFS *>(m_pController->getController(MEDIA_SOURCE_JUKEBOX));
   // { modified by Sergey Vetugov. CR#12671
   m_isFMFile[display] = FM_FILE_NEW; // modified by Sergey 09.09.2013 for ITS#188283
   //setPlayRequester(PLAY_REQUSTER_FM); // added by Sergey for CR#16015//removed for ITS 250238
   emit setDisplayOSD( true );//added by edo.lee 2013.05.29
   OSDManager::instance()->setFileManagerToBG();//added by edo.lee 2013.08.22 do not show OSD 
   // { modified by Sergey Vetugov. CR#12671
   if( NULL != pUsb1Ctl && GetUSBPath(MEDIA_SOURCE_USB1).length() > 0 && filePath.startsWith(GetUSBPath(MEDIA_SOURCE_USB1)) )
   {
      MP_LOG << "MEDIA_SOURCE_USB1 playFile " << LOG_ENDL;
      pUsb1Ctl->saveFileInfoForRestore(filePath);
      pUsb1Ctl->saveLastPlayedInfo(); // added by Sergey for CR#16015
      pUsb1Ctl->setCheckForPlayableFile(false); // added by Sergey for ScreenTransition
	  // removed by Sergey 26.10.2013 for ITS#198812 
   }
   else if ( NULL != pUsb2Ctl && GetUSBPath(MEDIA_SOURCE_USB2).length() > 0 && filePath.startsWith(GetUSBPath(MEDIA_SOURCE_USB2)) )
   {
      MP_LOG << "MEDIA_SOURCE_USB2 playFile " << LOG_ENDL;
      pUsb2Ctl->saveFileInfoForRestore(filePath);
      pUsb2Ctl->saveLastPlayedInfo(); // added by Sergey for CR#16015
      pUsb2Ctl->setCheckForPlayableFile(false); // added by Sergey for ScreenTransition
	  // removed by Sergey 26.10.2013 for ITS#198812 
   }
   else if(pJBCtl != NULL && filePath.startsWith("/usr_data/media") )
   {
      MP_LOG << "MEDIA_SOURCE_JUKEBOX playFile " << LOG_ENDL;
      pJBCtl->saveFileInfoForRestore(filePath);
      pJBCtl->saveLastPlayedInfo(); // added by Sergey for CR#16015
      pJBCtl->setCheckForPlayableFile(false); // added by Sergey for ScreenTransition
	  // removed by Sergey 26.10.2013 for ITS#198812 
   }

}

// { modified by Sergey 13.08.2013 for ITS#183831
bool AppMediaPlayer_EngineVideo::handleSettingsCommand(EVENT_SETTINGS_CHANGED_DATA_T *eventData)
{
//    if(GetVideoController()->getSource() == MEDIA_SOURCE_MAX) // commented by Sergey 25.08.2013 for ITS#186166
//        return false;

    MP_LOG << "sourceDisplay = " << eventData->sSourceDisplay << "key =" << eventData->sKey << "val =" << eventData->sValue << LOG_ENDL;

    switch(eventData->sKey)
    {
        case AppSettingsDef::DB_KEY_VIDEO_BRIGHTNESS:
            m_pController->setBrightness( eventData->sSourceDisplay, eventData->sValue );
            break;

        case AppSettingsDef::DB_KEY_VIDEO_SATURATION:
            m_pController->setSaturation( eventData->sSourceDisplay, eventData->sValue );
            break;

        case AppSettingsDef::DB_KEY_VIDEO_CONTRAST:
            m_pController->setDarkness( eventData->sSourceDisplay, eventData->sValue );
            break;

        default:
            return false; // unhandled
    }

    return true;
}
// } modified by Sergey 13.08.2013 for ITS#183831

// { modified by kihyung 2013.1.25
void AppMediaPlayer_EngineVideo::launchSettings( AppSettingsDef::EAppSettingsStartState nState,
													int device, bool isDvdTabDimmed, int display) //added by edo.lee 2013.05.02
{
    MP_LOG << nState << device << isDvdTabDimmed << "display"<<display << LOG_ENDL;

    emit setInitDispValue(); //added by shkim for ITS 189354 Display Setting value Issue
	//added by edo.lee 2013.03.13
	
	DISPLAY_T disp = display == VIDEO_DISPLAY_FRONT? DISPLAY_FRONT : DISPLAY_REAR;
	/*remove by edo.lee 2013.05.02
	if(m_EngineMain.GetFrontView() == APP_MEDIA_MODE_VIDEO)
	{
		disp = DISPLAY_FRONT;
	}else if(m_EngineMain.GetRearView() == APP_MEDIA_MODE_VIDEO)
	{
		disp = DISPLAY_REAR;
	}
	
	if(getIsShowLockoutView() == VIDEO_DISPLAY_FRONT)
	{
		disp = DISPLAY_REAR;
	}else if(getIsShowLockoutView() == VIDEO_DISPLAY_REAR)
	{
		disp = DISPLAY_FRONT;
	}
	*/
	//added by edo.lee 2013.03.13
    
    if(nState == AppSettingsDef::eStart_Mode_Screen)
    {
        m_bIsLaunchingScreenSettings = (VIDEO_DISPLAY_T)display;//modified by edo.lee 2013.12.13 ITS 0211957
    }
    // { added by cychoi 2013.08.05 for ScreenOptions is opened when returned from SoundSetting
    else if(nState == AppSettingsDef::eStart_Mode_Sound)
    {
        m_bIsLaunchingSoundSettings = true;
        // { added by wspark 2014.02.19 for ITS 225827
        initDirectAccessDisplayed(VIDEO_DISPLAY_FRONT, false);
        initDirectAccessDisplayed(VIDEO_DISPLAY_REAR, false);
        // } added by wspark
    }
    // } added by cychoi 2013.08.05 for ScreenOptions is opened when returned from SoundSetting
    
    APP_EVENT_SETTINGS_DATA_T pEventData;
    pEventData.header.sIsJogEnabled = getScreenManager(VIDEO_DISPLAY_FRONT)->isFocusVisible();
    pEventData.sStartState = nState;

#if USE_NEW_SETTING_ENTRY_POINT
    int nEntryPoint;
    if(nState == AppSettingsDef::eStart_Mode_Screen)
    {
        if(device == MP::AUX)       nEntryPoint = eAPP_SETTINGS_SCREEN_AVP_AUX;
        else if(device == MP::DISC) nEntryPoint = eAPP_SETTINGS_SCREEN_AVP_DVD;
        else                        nEntryPoint = eAPP_SETTINGS_SCREEN_AVP_DIVX;
    }
    else if(nState == AppSettingsDef::eStart_Mode_Sound)
    {
        nEntryPoint = eAPP_SETTINGS_SOUND;
    }
    else if(nState == AppSettingsDef::eStart_Mode_General_Video_Control)
    {
        if(device == MP::DISC) nEntryPoint = eAPP_SETTINGS_GENERAL_VIDEO_DVD;
        else                   nEntryPoint = eAPP_SETTINGS_GENERAL_VIDEO_DIVX; 
    }
    else
    {
        return;
    }

    MP_LOG << "nEntryPoint:" << nEntryPoint << LOG_ENDL;
    Event softEvent(EVT_KEY_SOFT_SETTINGS, m_EngineMain.GetThisAppId(), APP_UI_MANAGER, 
                    nEntryPoint, QByteArray((char*)&pEventData, sizeof(pEventData))); // modified by lssanh 2013.02.21 IPC payload
    emit NotifyUISH(softEvent, disp);//  m_EngineMain.getCurrScreen());//modified by edo.lee 2013.03.13
#else
    AppSettingsStartParameter param;
    param.startState       = nState;
	param.transparentBG    = true;
    param.divxDvdTabDimmed = isDvdTabDimmed;
    
    Event postEvent(EVT_POST_EVENT, m_EngineMain.GetThisAppId(), APP_SETTINGS, QByteArray((char*)&param, sizeof(param))); // modified by lssanh 2013.02.21 IPC payload
    emit NotifyUISH( postEvent ,disp);//modified by edo.lee 2013.03.13
    
    Event softEvent(EVT_KEY_SOFT_SETTINGS, m_EngineMain.GetThisAppId(), APP_UI_MANAGER, QByteArray((char*)&pEventData, sizeof(pEventData))); // modified by lssanh 2013.02.21 IPC payload
    emit NotifyUISH(softEvent ,disp);//modified by edo.lee 2013.03.13
#endif
}
// } modified by kihyung 2013.1.25

void AppMediaPlayer_EngineVideo::setStatusBarOnScreen( bool onScreen , bool isFront)
{
    Q_UNUSED(onScreen);
    Q_UNUSED(isFront);
	//modified by edo.lee 2013.04.04
 /*  MP_LOG << ", onScreen = " << onScreen << LOG_ENDL;
   
	#if STATUSBAR_NEW
	// { added by edo.lee 2012.12.05 for statusbar
	//modified by edo.lee 2013.03.10
	//if(isVideoInFG() == false || m_isHideScreen) return; //added by edo.lee 2013.01.10	
	if(isVideoInFG() == false || m_EngineMain.GetNotifier()->GetActiveMode() == APP_MEDIA_MODE_AUDIO) //modified by edo.lee 2013.03.14
		return;
	//modified by edo.lee
	//modified by edo.lee 2013.02.04
	//if(m_EngineMain.GetFrontView() == APP_MEDIA_MODE_VIDEO)
	//	OSDview = m_EngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
	//else if (m_EngineMain.GetRearView() == APP_MEDIA_MODE_VIDEO)
	//	OSDview = m_EngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
	if(isFront )//&& m_EngineMain.GetFrontView() == APP_MEDIA_MODE_VIDEO)//modified by edo.lee 2013.03.14
	{
		OSDview = m_EngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
	}
	else if(isFront==false )//&& m_EngineMain.GetRearView() == APP_MEDIA_MODE_VIDEO)//modified by edo.lee 2013.03.14
	{
		OSDview = m_EngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
	}
	//modified by edo.lee 2013.03.12
	else
	{
		MP_LOG<<"don't access declarative view" << LOG_ENDL;
		return;
	}
	//modified by edo.lee
	//modified by edo.lee
	// } added by edo.lee
	QString info = "@OP^";
	info += isFront ? "F^":"R^";
	info += onScreen ? "SSHOW" : "SHIDE"; //modified by edo.lee 2013.02.01
    MP_LOG<<"OSD String "<<OSDview<<info << LOG_ENDL;
	if(OSDview){
	    //OSDview->setWindowTitle(info); EDO
	    OSDWriter::SendOSD(info);
		//QCoreApplication::flush(); //added by edo.lee 2013.01.16
		}
			
	
	// } added by edo.lee
	#else
    int aDisplayId = m_currentScreen;
    EVT_PUBLIC_ID_T aEventId = ( onScreen ) ? EVT_SHOW_STATUS_BAR : EVT_HIDE_STATUS_BAR;

    Event aEvent( aEventId, m_EngineMain.GetThisAppId(), APP_STATUS_BAR,
                 QByteArray( (char*)&aDisplayId,sizeof(aDisplayId) ) ); // modified by lssanh 2013.02.21 IPC payload

    emit NotifyUISH( aEvent );
	#endif

*/
	//modified by edo.lee 2013.04.04
}

// modified by Dmitry 16.05.13
void AppMediaPlayer_EngineVideo::prevSEEKButtonHandler(KEY_STATUS_T keyStatus, DISPLAY_T display)
{
	// { added by Sergey 26.10.2013 to fix SEEK after ModeArea focus
    if(keyStatus == KEY_STATUS_PRESSED)
        m_bSeekHKPressed = true;
        //moved by taihyun.ahn for ITS 221363 2014.01.21 to SM
    //else if(keyStatus == KEY_STATUS_RELEASED || keyStatus == KEY_STATUS_CANCEL)
        //m_bSeekHKPressed = false;
        //moved by taihyun.ahn for ITS 221363 2014.01.21 to SM
     // } added by Sergey 26.10.2013 to fix SEEK after ModeArea focus

    int disp; // modified by cychoi 2013.09.22 for ITS 190635 DVD OSD

    //{ modified by cychoi 2013.09.23 for ITS 190264 DVD OSD
    if(m_EngineMain.GetFrontView() == APP_MEDIA_MODE_VIDEO )
        disp = VIDEO_DISPLAY_FRONT;
    else if(m_EngineMain.GetRearView() == APP_MEDIA_MODE_VIDEO )
        disp = VIDEO_DISPLAY_REAR;
    else
        disp = (display == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;
    //} modified by cychoi 2013.09.23

    // { modified by Sergey 17.10.2013 for ITS#196105
    MP_HIGH << "keyStatus:" << keyStatus << "disp =" << disp << "owner =" << pbOwner() << LOG_ENDL;

//    // { added by Sergey 05.10.2013 for ITS#192069
//    if((VIDEO_DISPLAY_T)pbOwner() != disp)
//    {
//        emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED);
//    }
//    // } added by Sergey 05.10.2013 for ITS#192069
    // } modified by Sergey 17.10.2013 for ITS#196105

    //{ moved by cychoi 2013.09.22 for ITS 190635 DVD OSD
    //{ modified by cychoi 2013.08.06 for Position OSD is displayed on DVD Setting from Title/Disc Menu
    // ISV 88393. Smoke Test DVD menu OSD wrong display. ITS 178308
    if(getScreenDVDMenuTitle((VIDEO_DISPLAY_T)disp) && !isDVDTrackPlaying()) // added by cychoi 2013.09.04 for ITS 186733
    {
        if(getScreenManager(disp)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
            getScreenManager(disp)->popScreen(QVariant());

        if(keyStatus == KEY_STATUS_PRESSED)
            OSDManager::instance()->displayDVDMenuOSD(true);
        return;
    }
    //} modified by cychoi 2013.08.06
    //} moved by cychoi 2013.09.22

    if (m_pTuneTimer->isActive())
    {
       ClearOSD();
       m_pTuneTimer->stop();
       onTuneTimerExpired();
    }

	// { modified by cychoi 2013.06.16 for Chapters List Seek Up/Down
	bool isSeekEnabled = false;	
    // { modified by cychoi 2013.06.25 for Search Caption & PBC Menu Seek Up/Down
    QString topScreenName = getScreenManager(disp)->topScreenName();
    if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS ||
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU ||
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION ||
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DIVXREG||  //added by edo.lee 2013.07.31 next/prev on subscreen
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS|| //added by edo.lee 2013.07.31
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS || // added by cychoi 2013.07.04 for ISV 86696 & 86858
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU || // added by cychoi 2013.09.04 for ITS 186733
       isUnsupportedPopup(display))		// added by Sergey 01.09.2013 for ITS#186834 
    {
		isSeekEnabled = true;
    }
    // } modified by cychoi 2013.06.25

	// removed by Sergey 10.09.2013 for ITS#188771
    
    switch(keyStatus)
    {
#ifndef DUAL_KEY
    case KEY_STATUS_CANCEL:   // modified by yongkyun.lee 2013-07-08 for : ITS 178517
#endif
    case KEY_STATUS_RELEASED:
    {
    	// { modified by Sergey 09.10.2013 for ITS#194195
        if(isUnsupportedPopup(display))
        {
            if(m_isLongPressed != 0)
            {
                m_isLongPressed = 0;
                return;
            }
            onPopScreen(); // added by Sergey 01.09.2013 for ITS#186834
        }
        // } modified by Sergey 09.10.2013 for ITS#194195

		MP_HIGH << "FG =" << isVideoInFG() << "seek =" << isSeekEnabled << "playback =" << m_pController->getPlaybackStatus() << "longPress =" << m_isLongPressed << LOG_ENDL; // added by Sergey 09.10.2013 for ITS#194195
        
        // { modified by cychoi 2013.06.16 for Chapters List Seek Up/Down
        if (isVideoInFG() && !isSeekEnabled)
        //if (isVideoInFG() && !isSettingScreen)//modified by edo.lee 2013.05.31
        //} modified by cychoi 2013.06.16
        {
           //deleted by aettie for 20130613 ITS 0172342
           getScreenManager(disp)->SendJogEvent(AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_RELEASED );//added by yongkyun.lee 20130225 for : ISV 73823
           if( getScreenManager(disp)->getControllerFocusIndex() == 0 )
           {
               emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_RELEASED); // modified by ravikanth 05-07-13 for ITS 0178426
           }

        }
        // { modified by Sergey 17.10.2013 for ITS#195124
        else
        {
//           if( (m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_4X)||
//               (m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_16X)||
//               (m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_20X))
//           {
//               m_pController->cancel_ff_rew();  // modified by Sergey for 78169
//           }
//           else
//           {
//               if(m_isLongPressed == 0)
//               {
//                   // { commented by cychoi 2013.07.13 for List View spec out
//                   //if ((getScreenManager(disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLES) ||
//                   //    (getScreenManager(disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_DVD_CHAPTERS) ||
//                   //    (getScreenManager(disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_VCD_CHAPTERS)//||
//                   //    /*(getScreenManager(disp)->topScreenName()== "")*/) 		//remove by edo.lee 2013.06.11
//                   //{
//                   //    m_pController->next();
//                   //}
//                   //else
//                   // } commented by cychoi 2013.07.13
//                   {
//                       setRewReachedFirst(false); // added by Sergey 28.05.2013
//                       m_pController->prev();
//                   }
//               }
//               emit setDisplayOSD( true );
//            }
            if(m_EngineMain.getOnBoot())//Added by taihyun.ahn 2013.10.23 for ITS 0197547
            {
                if(m_isLongPressed == 0){
                    m_pController->prev();
                }
                else{
                    m_pController->cancel_ff_rew();
                    m_isLongPressed = 0;
                }
            }
            // { added by sjhyun 2013.11.06 for ITS 206822
            else if(!m_bIsPlaybackControlQMLIsLoaded || !m_bIsVideoPlaybackQMLIsLoaded) // modified by sjhyun 2013.11.11 for ITS 206822
            {
                MP_LOG<<"controller->prev() PlaybackControlQML("<< m_bIsPlaybackControlQMLIsLoaded<<") PlaybackQML("<< m_bIsVideoPlaybackQMLIsLoaded<<")" << LOG_ENDL; // added by sjhyun 2013.11.11 for ITS 206822
                if(m_isLongPressed == 0){
                    m_pController->prev();
                }
                else{
                    m_pController->cancel_ff_rew();
                    m_isLongPressed = 0;
                }
            }
            // } added by sjhyun
            else
            {
                emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_RELEASED);
                emit setDisplayOSD( true );
            }

//            }
        }
        // } modified by Sergey 17.10.2013 for ITS#195124

        break;
    }

    case KEY_STATUS_PRESSED:
    {
        // { modified by Sergey 10.09.2013 for ITS#188771
        if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS) // modified by Sergey 01.09.2013 for ITS#186834
        {
            getScreenManager(disp)->popScreen(QVariant());
        }
        // } modified by Sergey 10.09.2013 for ITS#188771

		// removed by Sergey 18.10.2013 to highlight SEEK button when hold SEEK while unsupported popup  

        m_isLongPressed = 0;
        // { modified by cychoi 2013.06.16 for Chapters List Seek Up/Down
        if (isVideoInFG() && !isSeekEnabled)
        //if (isVideoInFG() && !isSettingScreen)//modified by edo.lee 2013.05.31
        //} modified by cychoi 2013.06.16
        {
           getScreenManager(disp)->SendJogEvent( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_PRESSED );
           if( getScreenManager(disp)->getControllerFocusIndex() == 0 )
           {
               //if(!isSystemPopupVisible()){//Removed by taihyun.ahn 2013.10.10
               getScreenManager(disp)->setDefaultFocus();//Added by taihyun.ahn 2013.10.09 for ITS 189091
               getScreenManager(disp)->showFocus();//Added by taihyun.ahn 2013.10.09 for ITS 189091
               //}
               emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_PRESSED); // modified by ravikanth 05-07-13 for ITS 0178426
           }
        }
        // { added by Sergey 17.10.2013 for ITS#195124
        else
        {
            if(!m_EngineMain.getOnBoot()){//Added by taihyun.ahn 2013.10.23 for ITS 0197547
                emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_PRESSED); // modified by ravikanth 05-07-13 for ITS 0178426
            }
        }
        // } added by Sergey 17.10.2013 for ITS#195124
        
        break;
    }

    case KEY_STATUS_PRESSED_LONG:
    {
        if(isUnsupportedPopup(display)) // added by Sergey 01.09.2013 for ITS#186834 
            return;

        m_isLongPressed = -1;
        //m_EngineMain.beep(eBeepType1); // modified by yongkyun.lee 2013-08-22 for : ISV 89537

        // { modified by cychoi 2013.06.16 for Chapters List Seek Up/Down
        if (isVideoInFG() && !isSeekEnabled)
        //if (isVideoInFG() && !isSettingScreen)//modified by edo.lee 2013.05.31
        //} modified by cychoi 2013.06.16
        {
           getScreenManager(disp)->SendJogEvent( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED );
           if( getScreenManager(disp)->getControllerFocusIndex() == 0 )
           {
               emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED); // modified by ravikanth 05-07-13 for ITS 0178426
           }
        }
        else
        {
           //m_pController->rewind(); // commented by Sergey 17.10.2013 for ITS#195124
           if(m_EngineMain.getOnBoot()){//Added by taihyun.ahn 2013.10.23 for ITS 0197547
               m_pController->rewind();
           }
           else{
               emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED); // added by Sergey 17.10.2013 for ITS#195124
           }
           getEngineMain().ManualBeep();
        }
        break;
    }
    case KEY_STATUS_PRESSED_CRITICAL:
    {
        if(isUnsupportedPopup(display)) // added by Sergey 01.09.2013 for ITS#186834 
            return;

        m_isLongPressed = -1;
        if (m_pController->isSeekMode() == false)
            break;
        //m_EngineMain.beep(eBeepType1); // modified by yongkyun.lee 2013-08-22 for : ISV 89537

        // { modified by cychoi 2013.06.16 for Chapters List Seek Up/Down
        if (isVideoInFG() && !isSeekEnabled)
        //if (isVideoInFG() && !isSettingScreen)//modified by edo.lee 2013.05.31
        //} modified by cychoi 2013.06.16
        {
           getScreenManager(disp)->SendJogEvent( AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED );
           if( getScreenManager(disp)->getControllerFocusIndex() == 0 )
           {
               emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED); // modified by ravikanth 05-07-13 for ITS 0178426
           }
        }
        else
        {
            if(m_EngineMain.getOnBoot()){//Added by taihyun.ahn 2013.10.23 for ITS 0197547
                m_pController->rewindCritical();
            }
            else{
                emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED); // added by Sergey 17.10.2013 for ITS#195124
            }
//           m_pController->rewindCritical(); // commented by Sergey 17.10.2013 for ITS#195124
           getEngineMain().ManualBeep();
        }
        break;
    }
#ifdef DUAL_KEY
        case KEY_STATUS_CANCEL:
        {
            if(isUnsupportedPopup(display))
            {
                if(m_isLongPressed != 0)
                {
                    m_isLongPressed = 0;
                    return;
                }
                onPopScreen(); // added by Sergey 01.09.2013 for ITS#186834
            }

            if (isVideoInFG() && !isSeekEnabled)
            {
               getScreenManager(disp)->SendJogEvent(AppEngineQMLConstants::JOG_LEFT, AppEngineQMLConstants::KEY_STATUS_CANCELED );
               if( getScreenManager(disp)->getControllerFocusIndex() == 0 )
               {
                   emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED);
               }

            }
            else
            {
                if(m_EngineMain.getOnBoot())
                {
                    if(m_isLongPressed == 0){
                        m_pController->prev();
                    }
                    else{
                        m_pController->cancel_ff_rew();
                        m_isLongPressed = 0;
                    }
                }
                else if(!m_bIsPlaybackControlQMLIsLoaded || !m_bIsVideoPlaybackQMLIsLoaded) // modified by sjhyun 2013.11.11 for ITS 206822
                {
                    MP_LOG<<"controller->prev() PlaybackControlQML("<< m_bIsPlaybackControlQMLIsLoaded<<") PlaybackQML("<< m_bIsVideoPlaybackQMLIsLoaded<<")" << LOG_ENDL; // added by sjhyun 2013.11.11 for ITS 206822
                    if(m_isLongPressed == 0){
                        m_pController->prev();
                    }
                    else{
                        m_pController->cancel_ff_rew();
                        m_isLongPressed = 0;
                    }
                }
                else
                {
                    emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED);
//                    emit setDisplayOSD( true );
                }
            }
            break;
        }
#endif
    default:
        break;
    }
}
// modified by Dmitry 16.05.13

//added by edo.lee 2013.03.28
void AppMediaPlayer_EngineVideo::setDefaultScreen(VIDEO_DISPLAY_T disp)
{
    MP_LOG<<disp << LOG_ENDL;
    if ( getScreenManager(disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_FULL 
        || getScreenManager(disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_OPTIONS
        || getScreenManager(disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON)//added by edo.lee 2013.08.26 
    {
        MP_LOG << "first check FullScreen." << LOG_ENDL;
        getScreenManager(disp)->popScreen(QVariant());
    }
}

// modified by Dmitry 16.05.13
void AppMediaPlayer_EngineVideo::nextSEEKButtonHandler(KEY_STATUS_T keyStatus, DISPLAY_T display)
{
	// { added by Sergey 26.10.2013 to fix SEEK after ModeArea focus
    if(keyStatus == KEY_STATUS_PRESSED)
        m_bSeekHKPressed = true;
        //moved by taihyun.ahn for ITS 221363 2014.01.21 to SM
    //else if(keyStatus == KEY_STATUS_RELEASED || keyStatus == KEY_STATUS_CANCEL)
        //m_bSeekHKPressed = false;
        //moved by taihyun.ahn for ITS 221363 2014.01.21 to SM
	// } added by Sergey 26.10.2013 to fix SEEK after ModeArea focus
	
    int disp; // modified by cychoi 2013.09.22 for ITS 190635 DVD OSD

    //{ modified by cychoi 2013.09.23 for ITS 190264 DVD OSD
    if(m_EngineMain.GetFrontView() == APP_MEDIA_MODE_VIDEO )
        disp = VIDEO_DISPLAY_FRONT;
    else if(m_EngineMain.GetRearView() == APP_MEDIA_MODE_VIDEO )
        disp = VIDEO_DISPLAY_REAR;
    else
        disp = (display == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;
    //} modified by cychoi 2013.09.23


    // { modified by Sergey 17.10.2013 for ITS#196105
    MP_HIGH << "keyStatus:" << keyStatus << "disp =" << disp << "owner =" << pbOwner() << LOG_ENDL;

//    // { added by Sergey 05.10.2013 for ITS#192069
//    if((VIDEO_DISPLAY_T)pbOwner() != disp)
//    {
//        emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED);
//    }
//    // } added by Sergey 05.10.2013 for ITS#192069
    // } modified by Sergey 17.10.2013 for ITS#196105

    //{ moved by cychoi 2013.09.22 for ITS 190635 DVD OSD
    //{ modified by cychoi 2013.08.06 for Position OSD is displayed on DVD Setting from Title/Disc Menu
    // ISV 88393. Smoke Test DVD menu OSD wrong display. ITS 178308
    if(getScreenDVDMenuTitle((VIDEO_DISPLAY_T)disp) && !isDVDTrackPlaying()) // added by cychoi 2013.09.04 for ITS 186733
    {
        if(getScreenManager(disp)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
            getScreenManager(disp)->popScreen(QVariant());

        if(keyStatus == KEY_STATUS_PRESSED)
            OSDManager::instance()->displayDVDMenuOSD(true);
        return;
    }
    //} modified by cychoi 2013.08.06
    //} moved by cychoi 2013.09.22

    if (m_pTuneTimer->isActive())
    {
       ClearOSD();
       m_pTuneTimer->stop();
       onTuneTimerExpired();
    }

	// { modified by cychoi 2013.06.16 for Chapters List Seek Up/Down
	bool isSeekEnabled = false;	
    // { modified by cychoi 2013.06.25 for Search Caption & PBC Menu Seek Up/Down
    QString topScreenName = getScreenManager(disp)->topScreenName();
    if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS ||
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU ||
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION ||
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DIVXREG||  //added by edo.lee 2013.07.31 next/prev on subscreen
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS|| //added by edo.lee 2013.07.31
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS || // added by cychoi 2013.07.04 for ISV 86696 & 86858
       topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU || // added by cychoi 2013.09.04 for ITS 186733
       isUnsupportedPopup(display))  // added by Sergey 01.09.2013 for ITS#186834
    {
		isSeekEnabled = true;
    }
    // } modified by cychoi 2013.06.25

	// removed by Sergey 10.09.2013 for ITS#188771

	MP_HIGH << "FG =" << isVideoInFG() << "seek =" << isSeekEnabled << "playback =" << m_pController->getPlaybackStatus() << "longPress =" << m_isLongPressed << LOG_ENDL; // added by Sergey 17.10.2013 for ITS#195124

    switch(keyStatus)
    {
#ifndef DUAL_KEY
    case KEY_STATUS_CANCEL:   // modified by yongkyun.lee 2013-07-08 for : ITS 178517
#endif
    case KEY_STATUS_RELEASED:
    {
    	// { modified by Sergey 09.10.2013 for ITS#194195
        if(isUnsupportedPopup(display)) 
        {
            if(m_isLongPressed != 0)
            {
                m_isLongPressed = 0;
                return;
            }

            onPopScreen();  // added by Sergey 01.09.2013 for ITS#186834
        }
		// } modified by Sergey 09.10.2013 for ITS#194195

		// commented by Sergey 17.10.2013 for ITS#195124

        // { modified by cychoi 2013.06.16 for Chapters List Seek Up/Down
        if (isVideoInFG() && !isSeekEnabled)
        //if (isVideoInFG() && !isSettingScreen)//modified by edo.lee 2013.05.31
        //} modified by cychoi 2013.06.16
        {
           //deleted by aettie for 20130613 ITS 0172342

           getScreenManager(disp)->SendJogEvent(AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_RELEASED );//added by yongkyun.lee 20130225 for :  ISV 73823
           if( getScreenManager(disp)->getControllerFocusIndex() == 0 )
           {
               emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_RELEASED); // modified by ravikanth 05-07-13 for ITS 0178426
           }
        }
        else
        {
        // { modified by Sergey 17.10.2013 for ITS#195124
//           if(m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X
//                   ||(m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_16X)
//                       ||(m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_20X))
//           {
//               m_pController->cancel_ff_rew();  // modified by Sergey for 78169
//           }
//           else
//           {
//               if(m_isLongPressed == 0) {
//                   // { commented by cychoi 2013.07.13 for List View spec out
//                   //if ((getScreenManager(disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLES) ||
//                   //    (getScreenManager(disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_DVD_CHAPTERS) ||
//                   //    (getScreenManager(disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_VCD_CHAPTERS)//||
//                   //    /*(getScreenManager(disp)->topScreenName()== "")*/) //remove by edo.lee 2013.06.11
//                   //{
//                   //    setRewReachedFirst(false);

//                   //    m_pController->prev();
//                   //}
//                   //else
//                   // } commented by cychoi 2013.07.13
//                   {
//                       m_pController->next();
//                   }
//               }
//               }
            if(m_EngineMain.getOnBoot())//Added by taihyun.ahn 2013.10.23 for ITS 0197547
            {
                if(m_isLongPressed == 0){
                    m_pController->next();
                }
                else{
                    m_pController->cancel_ff_rew();
                    m_isLongPressed = 0;
                }

            }
            // { added by sjhyun 2013.11.06 for ITS 206822
            else if(!m_bIsPlaybackControlQMLIsLoaded || !m_bIsVideoPlaybackQMLIsLoaded) // modified by sjhyun 2013.11.11 for ITS 206822
            {
                MP_LOG<<"controller->next() PlaybackControlQML("<< m_bIsPlaybackControlQMLIsLoaded<<") PlaybackQML("<< m_bIsVideoPlaybackQMLIsLoaded<<")" << LOG_ENDL; // added by sjhyun 2013.11.11 for ITS 206822
                if(m_isLongPressed == 0){
                    m_pController->next();
                }
                else{
                    m_pController->cancel_ff_rew();
                    m_isLongPressed = 0;
                }
            }
            // } added by sjhyun
            else
            {
               emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_RELEASED);
               emit setDisplayOSD( true );
            }
//           }
		// } modified by Sergey 17.10.2013 for ITS#195124
        }
        break;
    }

    case KEY_STATUS_PRESSED:
    {
        // { modified by Sergey 10.09.2013 for ITS#188771
        if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)  // modified by Sergey 01.09.2013 for ITS#186834
        {
            getScreenManager(disp)->popScreen(QVariant());
        }
        // } modified by Sergey 10.09.2013 for ITS#188771

		// removed by Sergey 18.10.2013 to highlight SEEK button when hold SEEK while unsupported popup

        m_isLongPressed = 0;
        // { modified by cychoi 2013.06.16 for Chapters List Seek Up/Down
        if (isVideoInFG() && !isSeekEnabled)
        //if (isVideoInFG() && !isSettingScreen)//modified by edo.lee 2013.05.31
        //} modified by cychoi 2013.06.16
        {
           getScreenManager(disp)->SendJogEvent(AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_PRESSED );
           if( getScreenManager(disp)->getControllerFocusIndex() == 0 )
           {
               //if(!isSystemPopupVisible()){//Deleted by taihyun.ahn 2013.10.10
               getScreenManager(disp)->setDefaultFocus();//Added by taihyun.ahn 2013.10.09 for ITS 189091
               getScreenManager(disp)->showFocus();//Added by taihyun.ahn 2013.10.09 for ITS 189091
               //}
               emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_PRESSED); // modified by ravikanth 05-07-13 for ITS 0178426
           }
        }
        // { added by Sergey 17.10.2013 for ITS#195124
        else
        {
            if(!m_EngineMain.getOnBoot()){   //Added by taihyun.ahn 2013.10.23 for ITS 0197547
                emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_PRESSED); // modified by ravikanth 05-07-13 for ITS 0178426
            }
        }
        // } added by Sergey 17.10.2013 for ITS#195124

        break;
    }

    case KEY_STATUS_PRESSED_LONG:
    {
        if(isUnsupportedPopup(display))  // added by Sergey 01.09.2013 for ITS#186834 
            return;

        m_isLongPressed = 1;
        //m_EngineMain.beep(eBeepType1); // modified by yongkyun.lee 2013-08-22 for : ISV 89537

        // { modified by cychoi 2013.06.16 for Chapters List Seek Up/Down
        if (isVideoInFG() && !isSeekEnabled)
        //if (isVideoInFG() && !isSettingScreen)//modified by edo.lee 2013.05.31
        //} modified by cychoi 2013.06.16
        {
           getScreenManager(disp)->SendJogEvent(AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED );
           if( getScreenManager(disp)->getControllerFocusIndex() == 0 )
           {
               emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED); // modified by ravikanth 05-07-13 for ITS 0178426
           }
        }
        else
        {
//           m_pController->fastForward(); // commented by Sergey 17.10.2013 for ITS#195124
            if(m_EngineMain.getOnBoot()){//Added by taihyun.ahn 2013.10.23 for ITS 0197547
                m_pController->fastForward();
            }
            else{
                emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED); // added by Sergey 17.10.2013 for ITS#195124
            }
           getEngineMain().ManualBeep();
        }
        break;
    }

    case KEY_STATUS_PRESSED_CRITICAL:
    {
        if(isUnsupportedPopup(display))  // added by Sergey 01.09.2013 for ITS#186834 
            return;

        m_isLongPressed = 1;

        if (m_pController->isSeekMode() == false)
            break;

        //m_EngineMain.beep(eBeepType1); // modified by yongkyun.lee 2013-08-22 for : ISV 89537

        // { modified by cychoi 2013.06.16 for Chapters List Seek Up/Down
        if (isVideoInFG() && !isSeekEnabled)
        //if (isVideoInFG() && !isSettingScreen)//modified by edo.lee 2013.05.31
        //} modified by cychoi 2013.06.16
        {
           getScreenManager(disp)->SendJogEvent(AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED );
           if( getScreenManager(disp)->getControllerFocusIndex() == 0 )
           {
               emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED); // modified by ravikanth 05-07-13 for ITS 0178426
           }
        }
        else
        {
            if(m_EngineMain.getOnBoot()){//Added by taihyun.ahn 2013.10.23 for ITS 0197547
                m_pController->fastForwardCritical();
            }
            else{
                emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED); // added by Sergey 17.10.2013 for ITS#195124
            }
                //           m_pController->fastForwardCritical(); // commented by Sergey 17.10.2013 for ITS#195124
           getEngineMain().ManualBeep();
        }

        break;
    }
#ifdef DUAL_KEY
        case KEY_STATUS_CANCEL:
        {
            if(isUnsupportedPopup(display))
            {
                if(m_isLongPressed != 0)
                {
                    m_isLongPressed = 0;
                    return;
                }
                onPopScreen();  // added by Sergey 01.09.2013 for ITS#186834
            }
            if (isVideoInFG() && !isSeekEnabled)
            {
               getScreenManager(disp)->SendJogEvent(AppEngineQMLConstants::JOG_RIGHT, AppEngineQMLConstants::KEY_STATUS_CANCELED );//added by yongkyun.lee 20130225 for :  ISV 73823
               if( getScreenManager(disp)->getControllerFocusIndex() == 0 )
               {
                   emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED); // modified by ravikanth 05-07-13 for ITS 0178426
               }
            }
            else
            {
                if(m_EngineMain.getOnBoot())//Added by taihyun.ahn 2013.10.23 for ITS 0197547
                {
                    if(m_isLongPressed == 0){
                        m_pController->next();
                    }
                    else{
                        m_pController->cancel_ff_rew();
                        m_isLongPressed = 0;
                    }

                }
                else if(!m_bIsPlaybackControlQMLIsLoaded || !m_bIsVideoPlaybackQMLIsLoaded) // modified by sjhyun 2013.11.11 for ITS 206822
                {
                    MP_LOG<<"controller->next() PlaybackControlQML("<< m_bIsPlaybackControlQMLIsLoaded<<") PlaybackQML("<< m_bIsVideoPlaybackQMLIsLoaded<<")" << LOG_ENDL; // added by sjhyun 2013.11.11 for ITS 206822
                    if(m_isLongPressed == 0){
                        m_pController->next();
                    }
                    else{
                        m_pController->cancel_ff_rew();
                        m_isLongPressed = 0;
                    }
                }
                else
                {
                   emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED);
        //           emit setDisplayOSD( true );
                }
            }
            break;
        }
#endif
    default:
        break;
    }
}
// modified by Dmitry 16.05.13

//{modified by edo.lee 2013.02.19
void AppMediaPlayer_EngineVideo::notifyComplete( QObject *root, bool isFront )
{
	if(isFront)
	   	m_pScreenManager[VIDEO_DISPLAY_FRONT]->setRootItem( root );//  modified by edo.lee 2013.02.04
	else
		m_pScreenManager[VIDEO_DISPLAY_REAR]->setRootItem( root );
}
//{modified by edo.lee

void AppMediaPlayer_EngineVideo::onContextChanged(bool isFront)
{
	MP_LOG<<isFront << LOG_ENDL;
	if(isFront)
	   	m_pScreenManager[VIDEO_DISPLAY_FRONT]->setContext( GetFrontContext() );//  modified by edo.lee 2013.02.04
	else
		m_pScreenManager[VIDEO_DISPLAY_REAR]->setContext( GetRearContext() );
	//	getScreenManager()->setContext( GetContext() );
}

// { modified by Sergey 01.05.2013 for Tune Functionality
void AppMediaPlayer_EngineVideo::tuneKnobHandler(Event &aEvent, DISPLAY_T display)
{
    // Return if in BT call or Video content is not ready or NULL pointers

//    if (m_EngineMain.getisBTCall()) return; // commented by Sergey 17.05.2013 for ISV#83170

    if(!m_pController || !m_pController->getCurrentController()) return;

    if(m_pController->getSource() == MEDIA_SOURCE_JUKEBOX && m_EngineMain.isTAScanComplete(eJukeBox) == false) {
        MP_LOG << "JUKEBOX is running TA Scan. return..." << LOG_ENDL;
        return;
    }
    else if(m_pController->getSource() == MEDIA_SOURCE_USB1 && m_EngineMain.isTAScanComplete(eUSB_FRONT) == false) {
        MP_LOG << "USB1 is running TA Scan. return..." << LOG_ENDL;
        return;
    }
    else if(m_pController->getSource() == MEDIA_SOURCE_USB2 && m_EngineMain.isTAScanComplete(eUSB_REAR) == false) {
        MP_LOG << "USB2 is running TA Scan. return..." << LOG_ENDL;
        return;
    }
    
    VIDEO_DISPLAY_T disp = (getCurrentScreen() == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;

	QString topScreenName = getScreenManager(disp)->topScreenName();

	KEY_STATUS_T keyStatus = KEY_STATUS_PRESSED; // just to initialize

	if(!aEvent.Data().isEmpty())
	{
		KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
		keyStatus =  eventData->keyStatus;
	}

    switch(aEvent.GetEventId())
    {

        // Search track by HU SPIN
        case EVT_KEY_HU_JOG_DIAL_ENCODER:
        {
#ifndef DUAL_KEY    // { modified by sangmin.seol 2014.03.11 ITS 0225385 H/U Tune Key Cancel Fix
            if(getFFREWState()==true) //if(getFFREWState()==true || m_EngineMain.isDRSShow()) // rollbacked by wspark 2014.02.24 for ITS 226456 // modified by wspark 2014.02.21 for ITS 226456
            {
                MP_LOG <<"ignore Tune knob FF/REW" << LOG_ENDL;
                return;
            }
#endif
            // { commented by cychoi 2013.07.13 for List View spec out
            // HU tune should select chapters in DVD/VCD list
            //if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLES ||
            //   topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_CHAPTERS ||
            //   topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_CHAPTERS)
            //{
            //    if((int)aEvent.GetCommand() > 0)
            //        getScreenManager(disp)->SendJogEvent( AppEngineQMLConstants::JOG_WHEEL_RIGHT, AppEngineQMLConstants::KEY_STATUS_PRESSED ); // modified by Dmitry 21.05.13
            //    else
            //        getScreenManager(disp)->SendJogEvent( AppEngineQMLConstants::JOG_WHEEL_LEFT, AppEngineQMLConstants::KEY_STATUS_PRESSED ); // modified by Dmitry 21.05.13
            //}
            //else
            // } commented by cychoi 2013.07.13
           // if(topScreenName != AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON) //remove by edo.lee 2013.10.01 ITS 0177248 
            {
                if (topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL ||
                        topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS)
                {
                    m_EngineMain.setIsFROnTune(true);
                }
                else
                    m_EngineMain.setIsFROnTune(false);

                popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS); // modified by Sergey 28.09.2013 for ITS#187711

                m_pController->tuneWheel(aEvent.GetCommand() >= 0);// modify by yongkyun.lee 20130703 for : ITS 177707
            }

            break;
        }

        // Search track by JOG SPIN
        case EVT_KEY_CCP_JOG_DIAL_ENCODER:
        case EVT_KEY_RRC_JOG_DIAL_ENCODER:
        {
            // { rollbacked by wspark 2014.02.24 for ITS 226456 { added by wspark 2014.02.21 for ITS 226456
            //if(m_EngineMain.isDRSShow())
            //{
            //    if((!m_EngineMain.IsSwapDCEnabled() && aEvent.GetEventId() == EVT_KEY_CCP_JOG_DIAL_ENCODER)
            //     || (m_EngineMain.IsSwapDCEnabled() && aEvent.GetEventId() == EVT_KEY_RRC_JOG_DIAL_ENCODER))
            //        break;
            //}
            // } added by wspark
            // removed by cychoi 2013.07.22 for ISV 87948 
            // If we are here it means that playback screen is on top
            if((getVideoControllerFoucusIndex(display) != 0))
            {
                if (topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL ||
                        topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS)
                    m_EngineMain.setIsFROnTune(true);
                else
                    m_EngineMain.setIsFROnTune(false);

                m_pController->tuneWheel(aEvent.GetCommand() >= 0);// modify by yongkyun.lee 20130703 for : ITS 177707
            }

            break;
        }

        // { modified by Sergey 27.09.2013 for ITS#189958
        // Cancel TUNE by JOG UP, LEFT, RIGHT
//        case EVT_KEY_CCP_JOG_DIAL_UP:
//        case EVT_KEY_RRC_JOG_DIAL_UP:
//        {
//            if(!m_pTuneTimer->isActive())
//                break;

//            // Switch off tune
//            if(keyStatus == KEY_STATUS_RELEASED)
//            {
//                m_pTuneTimer->stop();
//                m_pController->cancelTune();
//                emit setTuneMode(false);
//            }
//            break;
//        }
        // } modified by Sergey 27.09.2013 for ITS#189958

        // Select track by JOG/HU center
        case EVT_KEY_HU_TUNE_PRESSED:
        case EVT_KEY_CCP_JOG_DIAL_CENTER:
        case EVT_KEY_RRC_JOG_DIAL_CENTER:
        {
            // removed by cychoi 2013.07.22 for ISV 87948
            // { commented by cychoi 2013.07.13 for List View spec out
            // HU CENTER should select chapters in DVD/VCD list
            //if(keyStatus == KEY_STATUS_RELEASED
            //        && (topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLES ||
            //            topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_CHAPTERS ||
            //            topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_CHAPTERS))
            //{
            //    getScreenManager(disp)->SendJogEvent( AppEngineQMLConstants::JOG_CENTER, AppEngineQMLConstants::KEY_STATUS_RELEASED ); // modified by Dmitry 21.05.13
            //    break;
            //}
            // } commented by cychoi 2013.07.13
			//m_EngineMain.setMuteForSeek(true); //remove by edo.lee 2013.05.19
			//{added for ITS 230762
            if(aEvent.GetEventId() == EVT_KEY_CCP_JOG_DIAL_CENTER || aEvent.GetEventId() == EVT_KEY_RRC_JOG_DIAL_CENTER)
            {
                if( getScreenManager(disp)->getControllerFocusIndex() == 0 )
                {
                    return;
                }
            }
            //}added for ITS 230762

            if(m_pTuneTimer->isActive())
            {
				// { added by Sergey 16.10.2013 for ITS#195505
				if(keyStatus == KEY_STATUS_RELEASED)
				{
					emit lightControls((int)VLIGHT_CENTER, false, QVariant(VPEnum::VDISP_FRONT));
					emit lightControls((int)VLIGHT_CENTER, false, QVariant(VPEnum::VDISP_REAR));
				}
				// } added by Sergey 16.10.2013 for ITS#195505
				
				m_EngineMain.setMuteForSeek(false); //modified by edo.lee 2013.05.27
				m_EngineMain.setIsTermOnTrackChange(true);

                if(getVideoControllerFoucusIndex(display) != 0 || (aEvent.GetEventId() == EVT_KEY_HU_TUNE_PRESSED  && keyStatus == KEY_STATUS_PRESSED))// modified by edo.lee 2014.01.14 for working at press time
                {
                    if(m_pController->getCurrentController()->selectTuned())
                    {
                        //emit setTuneMode(false); //commented by yungi 2013.08.13 for ISV 88897
                        getEngineMain().GetNotifier()->setHistoryTuneInfo();
                        //emit setDisplayOSD( true );
                        //m_EngineMain.GetNotifier()->DisplayVideoTrackOSD();
                    }
                }
                m_pTuneTimer->stop();
                emit setTuneMode(false); //added by yungi 2013.08.13 for ISV 88897 
            }
			else if(aEvent.GetEventId() == EVT_KEY_HU_TUNE_PRESSED && keyStatus == KEY_STATUS_PRESSED) // modified by edo.lee 2014.01.14 for working at press time
        	{        		
        		//added by edo.lee 2013.07.12
        		MP_LOG<<"topScreenName " <<topScreenName << LOG_ENDL;
        		if (topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL ||
                    topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS)
                    m_EngineMain.setIsFROnTune(true);
                else
                    m_EngineMain.setIsFROnTune(false);
				//added by edo.lee 2013.07.12
        		//emit setDisplayOSD( true );
        		//m_EngineMain.GetNotifier()->DisplayVideoTrackOSD(); 
				showCurrentOSD();
        	}
			//added by edo.lee 2013.05.08
            break;
        }
        default:
        {
            MP_LOG << "Unsupported event" << LOG_ENDL;
            break;
        }
    }
}

// } modified by Sergey 01.05.2013 for Tune Functionality

bool AppMediaPlayer_EngineVideo::processedInAUXMode(Event &aEvent)
{
    bool ret = false;

    if ( m_pController->getSource() == MEDIA_SOURCE_AUX1 ||
         m_pController->getSource() == MEDIA_SOURCE_AUX2 )
    {
        ret = true;
        switch(aEvent.GetEventId())
        {
            case EVT_KEY_CCP_JOG_DIAL_ENCODER:
            case EVT_KEY_RRC_JOG_DIAL_ENCODER:
            case EVT_KEY_HU_JOG_DIAL_ENCODER:
            case EVT_KEY_HU_SEEK_PREV:
            case EVT_KEY_RRC_SEEK_PREV:
            case EVT_KEY_SWRC_SEEK_PREV:
            case EVT_KEY_HU_SEEK_NEXT:
            case EVT_KEY_RRC_SEEK_NEXT:
            case EVT_KEY_SWRC_SEEK_NEXT:
            case EVT_KEY_CCP_SEEK_PREV:         // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
            case EVT_KEY_CCP_SEEK_NEXT:         // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
            //case EVT_KEY_RRC_CH_BK:             // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
            //case EVT_KEY_RRC_CH_FW:             // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                m_EngineMain.setIsFROnTune(true);	//added by hyochang.ryu 20130822 for ITS184441
                OSDManager::instance()->displayOSD(OSD_EVENT_UNSUPPORTED_ACTION);
                break;

            case EVT_KEY_HU_TUNE_PRESSED:
                m_EngineMain.setIsFROnTune(false);	//added by hyochang.ryu 20130822 for ITS184441		
                // { modified by cychoi 2014.02.21 for ITS 226585 No current OSD on AUX mode
                OSDManager::instance()->displayOSD(OSD_EVENT_AUX_EXTERNAL_DEVICE_CONNECTED);
                //showCurrentOSD();
                // } modified by cychoi 2014.02.21
                break;

            default:
                MP_LOG << "Unsupported event" << LOG_ENDL;
                break;
        }
    }

    return ret;
}

// { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
bool AppMediaPlayer_EngineVideo::processedInDiscMode()
{
    if (m_pController->getSource() == MEDIA_SOURCE_DVDVIDEO ||
        m_pController->getSource() == MEDIA_SOURCE_DVDAUDIO ||
        m_pController->getSource() == MEDIA_SOURCE_VCD)
    {
        return getEngineMain().IsDeckBlocked();
    }

    return false;
}
// } added by cychoi 2014.04.09

// removed by Sergey 01.05.2013 for Tune Functionality

//added by edo.lee 2013.03.27

void AppMediaPlayer_EngineVideo::onPopScreen()
{	
	setDefaultScreen(VIDEO_DISPLAY_FRONT);
	setDefaultScreen(VIDEO_DISPLAY_REAR);	
}
//added by edo.lee 2013.05.17
void AppMediaPlayer_EngineVideo::onShowFullScreen()
{
	getScreenManager(VIDEO_DISPLAY_FRONT)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FULL);
	getScreenManager(VIDEO_DISPLAY_REAR)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FULL);
}
//added by edo.lee 2013.03.27


// { modified by Sergey 16.09.2013 for ITS#185236
void AppMediaPlayer_EngineVideo::onShowPopup(POPUP_TXT_BTN_TYPE_T param)
{
	showTxtBtnPopup(param, VIDEO_DISPLAY_FRONT);
	showTxtBtnPopup(param, VIDEO_DISPLAY_REAR);
}
// } modified by Sergey 16.09.2013 for ITS#185236


//added by aettie 20130612 for 172347
void AppMediaPlayer_EngineVideo::onShowSettings(QString type, QVariant param)
{
    MP_LOG<<"type = "<< type <<"     param = "<< param << LOG_ENDL;
   /* remove by edo.lee 2013.09.03 ITS 0187018
    if(getScreenManager(VIDEO_DISPLAY_FRONT)->topScreenName()==AppMediaPlayer_Video_Screens::SCREEN_FULL)
    {
        getScreenManager(VIDEO_DISPLAY_FRONT)->popScreen(QVariant());
        // removed by Sergey 10.08.2013 for 182892
    }
    else if(getScreenManager(VIDEO_DISPLAY_REAR)->topScreenName()==AppMediaPlayer_Video_Screens::SCREEN_FULL)
    {
        getScreenManager(VIDEO_DISPLAY_REAR)->popScreen(QVariant());
        // removed by Sergey 10.08.2013 for 182892
    }
    */
    if(type=="FS_Captions")
    {
    	//getScreenManager(VIDEO_DISPLAY_FRONT)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS, param);
	    //getScreenManager(VIDEO_DISPLAY_REAR)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS, param);	    
		getEngineMain().setSubCreenOn(true , AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS);
    }
    //added by edo.lee 2013.07.30 subscreen
	else if(type == "DIVX_Settings")
	{
		//getScreenManager(VIDEO_DISPLAY_FRONT)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DIVXREG, param);
		//getScreenManager(VIDEO_DISPLAY_REAR)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DIVXREG, param);
		getEngineMain().setSubCreenOn(true , AppMediaPlayer_Video_Screens::SCREEN_DIVXREG);
	}
	//added by edo.lee 2013.07.30 subscreen
	else if(type == "DVD_Settings")
     {
       //getScreenManager(VIDEO_DISPLAY_FRONT)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS, param);
	   //getScreenManager(VIDEO_DISPLAY_REAR)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS, param);
	   getEngineMain().setSubCreenOn(true , AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS);
    }
    // { added by cychoi 2013.08.20 for ITS 178895 SerachCaption subscreen
    else if(type == "DVD_SearchCaption")
    {
        //getScreenManager(VIDEO_DISPLAY_FRONT)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION, param);
        //getScreenManager(VIDEO_DISPLAY_REAR)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION, param);
        getEngineMain().setSubCreenOn(true , AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION);
    }
    // } added by cychoi 2013.08.20
    else
	{
	   //getScreenManager(VIDEO_DISPLAY_FRONT)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU, param);
	   //getScreenManager(VIDEO_DISPLAY_REAR)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU, param);
	   getEngineMain().setSubCreenOn(true , AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU);
	}
	
}


void AppMediaPlayer_EngineVideo::onTuneTimerExpired()
{
    MP_LOG << LOG_ENDL;

    //{ added by yongkyun.lee 20130703 for :  ITS 177707
    if (m_pTuneTimer->isActive())
        m_pTuneTimer->stop(); 
    //} added by yongkyun.lee 20130703 
    
    // Notify everyone that tune mode is off.
    emit setTuneMode(false);
    getScreenManager(VIDEO_DISPLAY_FRONT)->setFocusActivefromEngineVideo(false); //added by changjin 2012.12.06 for ISV 64510 // modified by edo.lee 2013.02.04
	getScreenManager(VIDEO_DISPLAY_REAR)->setFocusActivefromEngineVideo(false); 

	//{Modified by radhakrushna 20120812 cr 14111
	//if(m_pController->getCurrentController())
	//m_pController->getCurrentController()->cancelTune();
    m_pController->cancelTune();
	//}Modified by radhakrushna 20120812 cr 14111
    setPlayRequester(PLAY_REQUSTER_MAX); // added by sjhyun 2013.10.22 for ITS 197247
}

// { added by wspark 2013.01.16
void AppMediaPlayer_EngineVideo::onVideoTimerExpired()
{
    MP_HIGH << LOG_ENDL;
    if(!getEngineMain().getBTCallPowerOn())//Added for ITS 232476 2014.03.28
    {
        DelayedPlay();
    }
}
// } added by wspark

void AppMediaPlayer_EngineVideo::fullScreenModeStatusChanged(bool on)
{
    emit setFullScreenMode(on);
}
// { modified by edo.lee 2013.02.04
//{modified by aettie 20130620 for back key event
void AppMediaPlayer_EngineVideo::backHandle(bool isFrontBack, bool isSoftKey)
{
    // { modified by kihyung 2013.1.9
    int             bBack   = false;
    MEDIA_SOURCE_T  nSource = m_pController->getSource();

	VIDEO_DISPLAY_T disp = isFrontBack ? VIDEO_DISPLAY_FRONT: VIDEO_DISPLAY_REAR;

    // { modified by kihyung 2013.1.28 for ISV 71502
    if(nSource == MEDIA_SOURCE_DVDVIDEO || nSource == MEDIA_SOURCE_DVDAUDIO) {
        // { modified by cychoi 2013.09.24 for ITS 191333
        AppMediaPlayer_Video_ControllerDVD *pDVD = (AppMediaPlayer_Video_ControllerDVD*)m_pController->getController(MEDIA_SOURCE_DVDVIDEO);
        if(pDVD->isScreenDVDMenuTitleState() == true)
        {
            bBack = true;
        }
        // } modified by cychoi 2013.09.24

        if(m_pScreenManager[disp]->topScreenName() != AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU) {
            bBack = true; //bBack = false;//{ modified by yongkyun.lee 20130406 for : NO CR DVD Title menu back key
        }
    }
    else if(nSource == MEDIA_SOURCE_VCD) {
        // { modified by wspark 2013.04.18 for ISV 70606
        /*
        AppMediaPlayer_Video_ControllerVCD *pDVD   = (AppMediaPlayer_Video_ControllerVCD*)m_pController->getController(MEDIA_SOURCE_VCD);
        AppMediaPlayer_Video_CModel_VCD    *pModel = (AppMediaPlayer_Video_CModel_VCD*)pDVD->getModel();


        if(pModel->isPbcMenuDisplayed() == true) {
            bBack = true;
        }

        if(m_pScreenManager[disp]->topScreenName() != AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU) {
            bBack = false;
        }
        */
        //AppMediaPlayer_Video_ControllerVCD *pVCD    = (AppMediaPlayer_Video_ControllerVCD*)m_pController->getController(MEDIA_SOURCE_VCD); // commented by cychoi 2014.03.18 for ITS 229926 //added by yungi 2013.09.12 for SmokeTest DirectAccess During DRS On
        bBack = false;
        //pVCD->setDirectAccessDisplayed(disp, false); // commented by cychoi 2014.03.18 for ITS 229926  // modified by wspark 2014.02.19 for ITS 225827 //added by yungi 2013.09.05 for ITS 187311  //added by yungi 2013.09.12 for SmokeTest DirectAccess During DRS On
        // } modified by wspark
    }

	//if(m_EngineMain.GetRearViewR() == m_EngineMain.GetFrontViewR()) // clone mode
	if(m_EngineMain.GetViewR(DISPLAY_FRONT) == m_EngineMain.GetViewR(DISPLAY_REAR))
		bBack = true;

    MP_LOG << "m_pScreenManager->getScreensCount():" << m_pScreenManager[disp]->getScreensCount() << LOG_ENDL;
    MP_LOG << "m_pScreenManager->topScreenName():" << m_pScreenManager[disp]->topScreenName() << LOG_ENDL;
    MP_LOG << "bBack" << bBack << LOG_ENDL;
	
	if ( m_EngineMain.getCloneState() == CLONE_ON_FRONT)
	{
		disp = VIDEO_DISPLAY_FRONT;
	}
	else if ( m_EngineMain.getCloneState() == CLONE_ON_REAR)
	{
		disp = VIDEO_DISPLAY_REAR;
	}
	
    // { added by sjhyun 2013.09.11 for ITS 189090
    // { modified by shkim 2013.08.27 for ITS 186402
    QString topScreenName = m_pScreenManager[disp]->topScreenName();
	if ((topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS)||
		(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DIVXREG)|| 
		(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS))
    // } modified by shkim 2013.08.27
	{            	
        // removed by shkim 2013.08.27 for ITS 186402
		getEngineMain().setSubCreenOn(false ,getEngineMain().getSubCreenName()); 
		HideSubScreenOnVideo(disp);	
		return;
	}
    else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION)
    {
        getEngineMain().setSubCreenOn(true ,AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS); 
    }
    // } added by sjhyun 2013.09.11

    if(m_pScreenManager[disp]->getScreensCount() == 1 || m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_FULL 
		|| bBack /*|| m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_EMPTY*/)//added by edo.lee 2013.02.06
    {
        // { added by wspark 2013.03.19 for ISV 76590
       /* if(m_EngineMain.getCloneState() == CLONE_ON_FRONT)
        {
            disp = VIDEO_DISPLAY_FRONT;
        }
        else if(m_EngineMain.getCloneState() == CLONE_ON_REAR)
        {
            disp = VIDEO_DISPLAY_REAR;
        }*/
        // } added by wspark

        //{ modified by yongkyun.lee 20130406 for : NO CR DVD Title menu back key
        // if(bBack && m_pScreenManager[disp]->getScreensCount() > 1
        //   && m_pScreenManager[disp]->topScreenName() != AppMediaPlayer_Video_Screens::SCREEN_FULL ) //added by edo.lee 2013.03.20
        if( bBack && m_pScreenManager[disp]->getScreensCount() > 1 && 
            ( m_pScreenManager[disp]->topScreenName() != AppMediaPlayer_Video_Screens::SCREEN_FULL && 
              m_pScreenManager[disp]->topScreenName() != AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU) ) //added by edo.lee 2013.03.20
        //} modified by yongkyun.lee 20130406 
        {
//modified by Dmitry 26.04.13
            if(m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU ||
               m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
//modified by Dmitry 26.04.13
            {
                m_pScreenManager[disp]->onBackKeyPressed();
            }
			//added by aettie 20130612 for 172347
           /* else if ( ( (m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS)||
				(m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_DIVXREG)|| //added by edo.lee 2013.07.30 subscreen
               (m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS)||
               (m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU))&& 
                (getIsShowLockoutView() != VIDEO_DISPLAY_MAX) )                
            {            	
               // m_pScreenManager[VIDEO_DISPLAY_FRONT]->popScreen(QVariant());
                getEngineMain().setSubCreenOn(false ,getEngineMain().getSubCreenName()); 
            	HideSubScreenOnVideo(disp);				
                //m_pScreenManager[VIDEO_DISPLAY_REAR]->popScreen(QVariant());
            }*/
            // { added by cychoi 2013.06.16 for ISV 85091 DVD Settings title disappear
            else if(m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION)
            {
                m_pScreenManager[disp]->onBackKeyPressed();
                getEngineMain().setSubCreenOn(true ,AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS); // modified by cychoi 2013.08.20 for ITS 178895 SerachCaption subscreen
            }
            // } added by cychoi 2013.06.16
            //{modified by edo.lee 2013.08.29
			else if(m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON)
			{
				m_pScreenManager[disp]->onBackKeyPressed();  // modified by Sergey 30.08.2013 for ITS#185691 
			}
			//}modified by edo.lee 2013.08.29
            else
            {
                // { modified by cychoi 2014.03.18 for ITS 229926 // modified by sjhyun 2013.11.09 for ITS 207953
                if (nSource == MEDIA_SOURCE_VCD && 
                    m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS)
                {
                    AppMediaPlayer_Video_ControllerVCD *pVCD = (AppMediaPlayer_Video_ControllerVCD*)m_pController->getController(MEDIA_SOURCE_VCD);
                    if(m_EngineMain.getCloneState() != CLONE_OFF)
                    {
                        pVCD->setDirectAccessDisplayed(VIDEO_DISPLAY_FRONT, false);
                        pVCD->setDirectAccessDisplayed(VIDEO_DISPLAY_REAR, false);
                        popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS);
                    }
                    else
                    {
                        pVCD->setDirectAccessDisplayed(disp, false);
                        m_pScreenManager[disp]->popScreen(QVariant());
                    }
                }
                else
                    m_pScreenManager[disp]->popScreen(QVariant());
                // } modified by cychoi 2014.03.18 // modified by sjhyun
            }
            // } modified by wspark
        }
        // { modified by kihyung 2013.2.6
        // { added by wspark 2013.03.19 for ISV 76590
        else
        {
        // } added by wspark
            MP_LOG << "Back_key : return to media home m_isBackPressByJog="<<m_isBackPressByJog << LOG_ENDL;
            MP_LOG << "Back_key : isSoftKey = "<<isSoftKey << LOG_ENDL;
            MP_LOG << "Back_key : m_bRRC = "<<m_bRRC << LOG_ENDL;
            MP_LOG << "Back_key : isFrontBack= "<<isFrontBack << LOG_ENDL;

            // removed by Sergey 11.12.2013 for ITS#211252, 212879

            if(isSoftKey)
			{
                Event event( EVT_KEY_SOFT_BACK, m_EngineMain.GetThisAppId(), APP_UI_MANAGER );

                if (m_isBackPressByJog)
				{
                    if(!m_bRRC)
                    {
                        MP_LOG << "Back_key : isSoftKey DISPLAY_FRONT" << LOG_ENDL;
                        emit NotifyUISH (event, DISPLAY_FRONT);
                    }
                    else
                    {
                        MP_LOG << "Back_key :isSoftKey DISPLAY_REAR" << LOG_ENDL;
                        emit NotifyUISH (event, DISPLAY_REAR);
                    } 
                }
                else
                {
                    Event event( EVT_TOUCH_BACK_KEY, m_EngineMain.GetThisAppId(), APP_UI_MANAGER );
                    MP_LOG << "Back_key : EVT_TOUCH_BACK_KEY" << LOG_ENDL;
                    emit NotifyUISH (event, DISPLAY_FRONT);
                }
            }
            else
            {
                Event event( EVT_KEY_SOFT_BACK, m_EngineMain.GetThisAppId(), APP_UI_MANAGER );

                if(isFrontBack)
                {
                    MP_LOG << "Back_key : hardKey DISPLAY_FRONT" << LOG_ENDL;
                    emit NotifyUISH (event, DISPLAY_FRONT);
                }
                else
                {
                    MP_LOG << "Back_key : hardKey DISPLAY_REAR" << LOG_ENDL;
                    emit NotifyUISH (event, DISPLAY_REAR);
                } 
            }
        }
    }
    else 
    {
    	
		disp = isFrontBack ? VIDEO_DISPLAY_FRONT: VIDEO_DISPLAY_REAR;
        MP_LOG << m_pScreenManager[disp]->getScreensCount() << m_pScreenManager[disp]->topScreenName() << LOG_ENDL;
        m_pScreenManager[disp]->onBackKeyPressed();
    }
    // } modified by kihyung 2013.1.9    
    // } added by kihyung 2013.1.28 for ISV 71502
}
// } modified by edo.lee 
//}modified by aettie 20130620 for back key event

//Add by changjin 2012.07.18 : for CR 12032 : Additional fix for CCP/RRC on Video playback screen.
// {
//  modified by edo.lee 2013.02.04
bool AppMediaPlayer_EngineVideo:: getVPWheelDisable(bool isFront)
{
	VIDEO_DISPLAY_T disp = isFront ? VIDEO_DISPLAY_FRONT: VIDEO_DISPLAY_REAR;
	
        MP_LOG << "Current screen = " <<  m_pScreenManager[disp]->topScreenName() << LOG_ENDL;

    if(  m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_JUKEBOX_PLAYBACK
        ||  m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_USB_PLAYBACK
        ||  m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_USB2_PLAYBACK
        ||  m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK
        ||  m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK
        ||  m_pScreenManager[disp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_FULL
        // removed by Sergey 10.08.2013 for 182892
        )
        return false;
    else
        return true;
}
//  modified by edo.lee 
// }
//Add by changjin 2012.07.18

// { removed by ravikanth for changes - CR 12929, 12935 (alternative for CR 12182)
// { added by kihyung 2012.07.27 for CR 12182
//bool AppMediaPlayer_EngineVideo::IsVideoMode(int nMode)
//{
//    switch(nMode)
//    {
//    case MODE_STATE_JUKEBOX_VIDEO:
//    case MODE_STATE_USB1_VIDEO:
//    case MODE_STATE_USB2_VIDEO:
//    case MODE_STATE_DISC_VIDEO:
//    case MODE_STATE_AUX1_VIDEO:
//    case MODE_STATE_AUX2_VIDEO:
//        return true;
//    default:
//        return false;
//    }
//}
// } added by kihyung
// } removed by ravikanth

// { added by ravikanth	- CR 11552, 11938
void AppMediaPlayer_EngineVideo::notifyUISH(Event evt)
{
    emit NotifyUISH(evt);
}
// } added by ravikanth

// { added by  yongkyun.lee  2012.10.17  for Current file is white color
//modified by edo.lee 2013.03.22
void AppMediaPlayer_EngineVideo::SetTuneSameFile(bool val)
{
    MP_LOG<<val << LOG_ENDL;
    emit tuneSameFileNoti( val );  
    emit tuneSameFile( val ); 
    //{ added by yongkyun.lee 20130703 for : ITS 177707
    if(val == true)
    {
        onTuneTimerExpired();		
    }
    else
    {
        emit setTuneMode(true);
        m_pTuneTimer->start();
    }
    //} added by yongkyun.lee 20130703 

    // m_EngineMain.GetNotifier()->DisplayVideoTrackOSD();
}
//modified by edo.lee 2013.03.22
// } added by  yongkyun.lee 

// { added by cychoi 2013.11.10 for ITS 207831 Control Cue Icon on Tune Mode
void AppMediaPlayer_EngineVideo::setTuneModeOnStateChanged(bool val)
{
    MP_LOG<<val << LOG_ENDL;
    emit tuneSameFileNoti( val );  
    emit tuneSameFile( val ); 
    emit setTuneMode(val == true);
    emit setTuneMode(val == false);
}
// } added by cychoi 2013.11.10

// {  added by changjin 2012.08.17 : for CR 12449
int AppMediaPlayer_EngineVideo::getVideoControllerFoucusIndex(DISPLAY_T display)
{
	MP_LOG<<"display"<<display<<" clone"<<m_EngineMain.getCloneState() << LOG_ENDL;
	VIDEO_DISPLAY_T disp = display == DISPLAY_FRONT ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;
	//added by edo.lee 2013.06.28
	if(m_EngineMain.getCloneState() == CLONE_ON_FRONT)
	{
		disp = VIDEO_DISPLAY_FRONT;
	}
	else if(m_EngineMain.getCloneState() == CLONE_ON_REAR)
	{
		disp = VIDEO_DISPLAY_REAR;
	}
	//added by edo.lee 2013.06.28
   MP_LOG << "Focus index = " << getScreenManager(disp)->getControllerFocusIndex() << LOG_ENDL;//  modified by edo.lee 2013.02.04  
   return getScreenManager(disp)->getControllerFocusIndex();//  modified by edo.lee 2013.02.04
}
// } added by changjin

// { added by junggil 2012.10.05 for SANITY_CM_AJ497
int AppMediaPlayer_EngineVideo::getStringWidth( const QString &fullString,
                                                      const QString &fontFamily,
                                                      int pixelSize )
{
    QFont mFont;
    mFont.setFamily(fontFamily);
    //mFont.setPixelSize(pixelSize);
    mFont.setPointSize(pixelSize);	//modified by aettie.ji 2012.11.28 for uxlaunch update

    QFontMetrics fontMetrics(mFont);

    return fontMetrics.width(fullString, -1);
}
// } added by junggil 

// removed by Sergey for CR#15775

// { modified by Sergey 10.05.2013
bool AppMediaPlayer_EngineVideo::isVideoInFG()
{
    bool ret = false;

    if(m_bVideoInFG[DISPLAY_FRONT] || m_bVideoInFG[DISPLAY_REAR])
        ret = true;

    return ret;
}

bool AppMediaPlayer_EngineVideo::isVideoInFG(DISPLAY_T disp)
{
    return m_bVideoInFG[disp];
}
// } modified by Sergey 10.05.2013

//{ added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD   
bool AppMediaPlayer_EngineVideo::DisplayOSD(bool vOSD)
{
    emit setDisplayOSD( vOSD );//leeyk1       
    return true;
}
//} added by  yongkyun.lee 

// { added by aettie.ji 2012.11.7 for New UX
bool AppMediaPlayer_EngineVideo::IsPlaying() const
{
    return (m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING);
}
// } added by aettie.ji 2012.11.7 for New UX

bool AppMediaPlayer_EngineVideo::IsStopped() const
{
    return (m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_STOPPED ||
            m_pController->getPlaybackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_INVALID);
}

// removed by Sergey 31.07.2013

// { added by Sergey for CR#16002
void AppMediaPlayer_EngineVideo::handleAspectRatioEvent(Event &aEvent)
{
    if(APP_SETTINGS != aEvent.GetSource() || aEvent.Data().isEmpty())
    {
        MP_LOG << "Wrong sender or empty data. Return." << LOG_ENDL;
        return;
    }

    VideoWidgetProps *widgetProps = (VideoWidgetProps*)aEvent.Data().data();
    if ( widgetProps != NULL )
    {
        /* Settings don't know current position of video widget.
         * Hence in case of video player is on top it should reset
         * the position of video widget.
         */
        if(isVideoInFG())
            widgetProps->isFrontDisplay = (m_currentScreen == DISPLAY_FRONT);

        // Update video widget sizes based on widget properties.
        m_pController->updateAspectRatio(widgetProps);

        /* If video player on foreground then show full screen widget.
         *  Otherwise we should show small widget for aspect rotation screen.
         */
        if(isVideoInFG())
            m_pController->showVideo(getCurrentScreen()); // modified by Sergey for CR#15775
        else // video in BG
        {
            // For DisplaySettings screen should show small widget.
            if(getEngineMain().getCamInFG() == false ||
                    (getEngineMain().getCamInFG() && actualDisp(getCurrentScreen()) == DISPLAY_REAR)) // modified by Sergey 06.11.2013 for camera flickering while changing ratio
            {
                m_pController->showSmallWidget(); // modified by Sergey 30.07.2013 for ITS#181902
            }
        } 
    }
}
// } added by Sergey for CR#16002.

// { added by Sergey for CR#15775
void AppMediaPlayer_EngineVideo::handleVideoPlayEvent(Event &aEvent)
{
    if(APP_SETTINGS != aEvent.GetSource() || aEvent.Data().isEmpty())
    {
        MP_LOG << "Wrong sender or empty data. Return." << LOG_ENDL;
        return;
    }

    DISPLAY_T* display = (DISPLAY_T*)aEvent.Data().data();
    if(*display)
    {
        m_pController->showVideo((DISPLAY_T)*display);
    }
    else
    {
        m_bIsLaunchingScreenSettings = VIDEO_DISPLAY_MAX;//modified by edo.lee 2013.12.13 ITS 0211957
        m_bIsLaunchingSoundSettings = false; // added by cychoi 2013.08.05 for ScreenOptions is opened when returned from SoundSetting
		// removed by Sergey for ITS#181872 30.07.2013
    }
}

// { deleted by wspark 2013.02.28 for ISV 73328
/*
void AppMediaPlayer_EngineVideo::updateDVDSettings(MEDIA_SOURCE_T aSource)
{
    if(aSource == MEDIA_SOURCE_JUKEBOX || aSource == MEDIA_SOURCE_USB1 || aSource == MEDIA_SOURCE_USB2)
    {
        Event dvdMode(EVT_POST_EVENT, getEngineMain().GetThisAppId(), APP_SETTINGS,
                      EVT_ENABLE_DVDSETTINGS);
        notifyUISH(dvdMode);
    }
    else
    {
        Event dvdMode(EVT_POST_EVENT, getEngineMain().GetThisAppId(), APP_SETTINGS,
                      EVT_DISABLE_DVDSETTINGS);
        notifyUISH(dvdMode);
    }
}
*/
// } deleted by wspark

MODE_STATE_T AppMediaPlayer_EngineVideo::mapToModeState(APP_ENTRY_POINT_T aEntryPoint)
{
    MODE_STATE_T result = MODE_STATE_MAX;

    switch(aEntryPoint)
    {
        case eAPP_AVP_VIDEO_MODE_JUKEBOX:
		case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
            result = MODE_STATE_JUKEBOX_VIDEO;
            break;
        case eAPP_AVP_VIDEO_MODE_USB1:
		case eAPP_AVP_VIDEO_MODE_USB1_DRS:
            result = MODE_STATE_USB1_VIDEO;
            break;
        case eAPP_AVP_VIDEO_MODE_USB2:
		case eAPP_AVP_VIDEO_MODE_USB2_DRS:
            result = MODE_STATE_USB2_VIDEO;
            break;
        case eAPP_AVP_VIDEO_MODE_DISC:
		case eAPP_AVP_VIDEO_MODE_DISC_DRS:
            result = MODE_STATE_DISC_VIDEO;
            break;
        case eAPP_AVP_VIDEO_MODE_AUX:
		case eAPP_AVP_VIDEO_MODE_AUX_DRS:
            result = MODE_STATE_AUX1_VIDEO;
            break;

        default:
            MP_LOG << "Unsupported entry point:" << aEntryPoint << LOG_ENDL;
            break;
    }

    return result;
}

// } added by Sergey for CR#15775

// { added by wspark 2012.12.17 for ISV64433
void
AppMediaPlayer_EngineVideo::HandleBackKey()
{
    MP_LOG << LOG_ENDL;
    Event event( EVT_KEY_SOFT_BACK, m_EngineMain.GetThisAppId(), APP_UI_MANAGER );
    //emit NotifyUISH (event);   
    emit NotifyUISH (event ,m_EngineMain.getIsFrontBack() ? DISPLAY_FRONT:DISPLAY_REAR); //modified by edo.lee 2013.03.21
}
// } added by wspark

// { modified by kihyung 2013.1.26
void
AppMediaPlayer_EngineVideo::onSignalVideoFgReceived(bool fgState)
{
    MP_LOG << LOG_ENDL;
    m_isFgReceived = true;
    m_bFgState = fgState; // added by wspark 2013.04.06 for ISV 78044
}

void 
AppMediaPlayer_EngineVideo::onSignalBgReceived()
{
    MP_LOG << LOG_ENDL;
    
//{ added by yongkyun.lee 20130206 for : ISV 70913
    MEDIA_SOURCE_T  nSource = m_pController->getSource();
    // removed by Sergey 31.07.2013
    if(nSource == MEDIA_SOURCE_DVDVIDEO || nSource == MEDIA_SOURCE_VCD) 
    {
        // removed by sjhyun 2013.10.15 for ITS 195523
        /*
        // { modified by yungi 2013.09.16 for SmokeTest DirectAccess During DRS On 2nd
        if(nSource == MEDIA_SOURCE_VCD)
        {
            AppMediaPlayer_Video_ControllerVCD *pVCD    = (AppMediaPlayer_Video_ControllerVCD*)m_pController->getController(MEDIA_SOURCE_VCD); //moved by yungi 2013.09.16 for SmokeTest DirectAccess During DRS On 2nd
            pVCD->setDirectAccessDisplayed(false);  //added by yungi 2013.09.12 for SmokeTest DirectAccess During DRS On
        }
        // } modified by yungi
        */
        //QEventLoop* loop = new QEventLoop(this); // removed by shkim for ITS 176008
        //loop->processEvents(QEventLoop::ExcludeUserInputEvents, 100); // removed by shkim for ITS 176008
    }
//} added by yongkyun.lee 20130206 
    m_isFgReceived = false;
}
// } modified by kihyung 2013.1.26
//remove by edo.lee 2013.11.08
// { added by wspark 2013.02.21 for ISV 73305
void
AppMediaPlayer_EngineVideo::onCamEnableReceived(bool on)
{
    MP_LOG << LOG_ENDL;
    // { modified by cychoi 2013.11.06 for ITS 206977 //remove by edo.lee 2013.06.08
    if(on == true)
    {
        if(m_pController->isSeekMode())
        {
            m_pController->cancel_ff_rew();
        }		
    }
    // } modified by cychoi 2013.11.06

    // { removed by shkim for ITS 206977 DVD Black Issue 2013.11.08
    //added by shkim for DISC(Title)->Display Setting -> Camera On : flickering issue  
    /*
    if(m_bIsLaunchingScreenSettings != VIDEO_DISPLAY_MAX)  // modified by cychoi 2013.12.17 for ITS 211957
    {
        if(getIsShowLockoutView() == VIDEO_DISPLAY_MAX && (m_nLastAVModeEvent == MODE_STATE_DISC_VIDEO))
        {
            if( m_EngineMain.getIsSwapEnabled() == false && m_EngineMain.getCloneState() == CLONE_OFF
                    && (!isVideoInFG()) && m_EngineMain.getRearBlendOnOff()== false)
            {
                MP_HIGH << "CASE : Only Front DVD(Title, PlayBack) ->Setting ->  Camera ON" << LOG_ENDL;
                m_pController->clearDisplay(true); // modified by Sergey for ITS#181872 30.07.2013
            }
            else if(m_EngineMain.getIsSwapEnabled() == true && m_EngineMain.getCloneState() == CLONE_OFF
                    && (!isVideoInFG()) && m_EngineMain.getRearBlendOnOff()== false)
            {
                MP_HIGH << "CASE : Swap and Only Front DVD(Title, PlayBack) ->Setting ->  Camera ON" << LOG_ENDL;
                m_pController->clearDisplay(true); // modified by Sergey for ITS#181872 30.07.2013
            }
        }

        if(getIsShowLockoutView() == VIDEO_DISPLAY_FRONT && m_nLastAVModeEvent == MODE_STATE_DISC_VIDEO)
        {
            if( (m_EngineMain.getIsSwapEnabled() == false) && (!isVideoInFG()) //modified by shkim for [NoCR] DVD title(DRS/Video Play) -> Display setting ->Cam Flickering Issue 2013.11.06
            && (m_EngineMain.getRearBlendOnOff() == false) &&  (m_EngineMain.getCloneState()== CLONE_OFF))
            {
                MP_LOG << "Front DVD -> Setting -> DRS On-> Cam On : " << LOG_ENDL;
                m_pController->clearDisplay(true); // modified by Sergey for ITS#181872 30.07.2013
            }
        }
    }
    */
    //added by shkim for DISC(Title)->Display Setting -> Camera On : flickering issue
    // } removed by shkim for ITS 206977 DVD Black Issue 2013.11.08
    emit setCamMode(on);
}
// } added by wspark
// { added by dongjin 2013.02.25 for ISV 73830
void
AppMediaPlayer_EngineVideo::releaseTuneMode()
{
    if (m_pTuneTimer->isActive())
    {
        m_pTuneTimer->stop();
        m_pController->cancelTune();
        emit setTuneMode(false);
    }
}

void
AppMediaPlayer_EngineVideo::SetPlayBackControlbyTune()
{
    if (m_pTuneTimer->isActive())
    {
        emit setTuneMode(true);
    }
}


//added by edo.lee 2013.04.05 78867
void
AppMediaPlayer_EngineVideo::releaseTuneModeByMenu()
{
    if (m_pTuneTimer->isActive())
    {
        m_pTuneTimer->stop();
        m_pController->cancelTune();
        emit setTuneMode(false);
		//m_EngineMain.GetNotifier()->DisplayVideoTrackOSD();
		showCurrentOSD();
    }
}
//added by edo.lee 2
//{ modified by edo.lee 2013.09.05 ITS 0188286
bool AppMediaPlayer_EngineVideo::getFFREWState()
{
  if( (m_pController->getPlaybackStatus() >= AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X)
      &&(m_pController->getPlaybackStatus() != AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_INVALID))
      return true;
  else
      return false;
}
//} modified by edo.lee 2013.09.05 ITS 0188286


// { modified by Sergey 2013.04.07 for ISV#143958,191449
void AppMediaPlayer_EngineVideo::unblockUI()
{
    if(isVideoInFG())
    {
        getEngineMain().unblockUI();
    }
}
// } modified by Sergey 2013.04.07 for ISV#143958,191449


// { added by Sergey 18.10.2013 for safe ui update
void AppMediaPlayer_EngineVideo::repaintUI(DISPLAY_T display)
{
    if(display == DISPLAY_NONE)
    {
        getEngineMain().repaintUI(); // repaint both displays
    }
    else if(isVideoInFG(display))
    {
        getEngineMain().repaintUI(display);  // repaint specific display
    }
}
// } added by Sergey 18.10.2013 for safe ui update


// { added by Sergey 26.10.2013 for ITS#198719
void AppMediaPlayer_EngineVideo::repaintUIQML(QVariant disp)
{
    DISPLAY_T display = ((VPEnum::VDISP_T)disp.toInt() == VPEnum::VDISP_FRONT) ? DISPLAY_FRONT : DISPLAY_REAR;

    repaintUI(display);
}
// } added by Sergey 26.10.2013 for ITS#198719

// { modified by ravikanth 16-04-13
void AppMediaPlayer_EngineVideo::CancelCopyJukebox()
{
	// modified for ITS 0214738
    AppFileManagerFileOperationParameter param;
    param.isFront = (m_currentScreen == DISPLAY_FRONT); // modified by ravikanth for ISV 93629
    param.mediaType = (m_pController->getSource() != MEDIA_SOURCE_JUKEBOX) ? eStartUsbVideo : eStartJukeVideo;
    Event postEvent( EVT_POST_EVENT,
                     m_EngineMain.GetThisAppId(),
                     APP_FMGR,
                     EVT_FILEOPERATION_STOP, QByteArray((char *)&param, sizeof(param)));
    emit NotifyUISH (postEvent);
    m_EngineMain.cancelAudioFileCopyOperation();
}
// } modified by ravikanth 16-04-13

// { modified by ravikanth 18-04-13
bool AppMediaPlayer_EngineVideo::loadingOnCopy()
{
    // removed by kihyung 2013.09.09 for ITS 0186027
    /*
    if(m_pController->getSource() == MEDIA_SOURCE_JUKEBOX)
    {
        return m_trackerUpdateInProgress; // modified by ravikanth 20-04-13
    }
    */
    return false;
}
// } modified by ravikanth 18-04-13

// { modified by ravikanth 20-04-13
void AppMediaPlayer_EngineVideo::setRescanProgressLoading()
{
    // removed by kihyung 2013.09.09 for ITS 0186027
    /*
    MP_LOG << LOG_ENDL;
    m_trackerUpdateInProgress = true;
    emit jukeboxCopyInProgress();
    */
}

void AppMediaPlayer_EngineVideo::ResetProgressLoading()
{
    // removed by kihyung 2013.09.09 for ITS 0186027
    /*
    MP_LOG << LOG_ENDL;
    m_trackerUpdateInProgress = false;
    emit jukeboxCopyInProgress();
    */
}
// } modified by ravikanth 20-04-13


// { modified by Sergey 07.09.2013 for PRE_FG supoort
void AppMediaPlayer_EngineVideo::loadUI(APP_ENTRY_POINT_T ep, DISPLAY_T disp)
{
    Q_UNUSED(ep);
    Q_UNUSED(disp);
}
// modified by Sergey 07.09.2013 for PRE_FG supoort


// removed by Sergey 10.08.2013 for 182892

// { added by cychoi 2013.05.16 for ISV 78583 & ITS 167437
void AppMediaPlayer_EngineVideo::SetVideoSize(DISPLAY_T aDisplay) // modified by cychoi 2013.07.03 for ITS 177711 
{
    MEDIA_SOURCE_T nSource = m_pController->getSource();
    if(nSource == MEDIA_SOURCE_DVDVIDEO || nSource == MEDIA_SOURCE_VCD ||
       nSource == MEDIA_SOURCE_AUX1 || nSource == MEDIA_SOURCE_AUX2) // added by cychoi 2013.12.04 for ITS 212759
    {
        // { modified by cychoi 2013.10.28 for ITS 198668 Video Y position
        // { modified by cychoi 2013.08.05 for Video size if top screen is ScreenOptions
        // { modified by cychoi 2013.07.03 for ITS 177711 
        int disp = aDisplay == DISPLAY_FRONT ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;
        // { added by cychoi 2013.06.26 for ITS 176110
        // { modified by cychoi 2013.07.27 for ISV 88393
        QString topScreenName = m_pScreenManager[disp]->topScreenName();
        if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
        {
            topScreenName = m_pScreenManager[disp]->getPreviousScreenName(1);
        }
        // } modified by cychoi 2013.07.27
        MP_LOG << topScreenName << LOG_ENDL;
        // } modified by cychoi 2013.07.03
        // } added by cychoi 2013.06.26
        if(getIsShowLockoutView() == VIDEO_DISPLAY_FRONT)
        {
            aDisplay = DISPLAY_REAR;
        }
        else if(getIsShowLockoutView() == VIDEO_DISPLAY_REAR)
        {
            aDisplay = DISPLAY_FRONT;
        }

        switch(nSource)
        {
            case MEDIA_SOURCE_DVDVIDEO:
            {
                // { added by cychoi 2013.07.03 for ITS 177711 
                if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL || // added by cychoi 2013.12.04 for ITS 212759
                   topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK)
                {
                    int y = (aDisplay == DISPLAY_REAR) ? 720 : 0;
                    ASPECT_RATIO_T currAspectRatio;
                    currAspectRatio = m_pController->getAspectRatio();
                    switch(currAspectRatio)
                    {
                    case ASPECT_RATIO_FULL:
                    case ASPECT_RATIO_16_9:
                        m_pController->SetVideoWidgetSize( 0, y, 1280, 720 );
                        break;
                    case ASPECT_RATIO_4_3:
                        m_pController->SetVideoWidgetSize( 160, y, 960, 720 );
                        break;
                    default:
                        break;
                    }
                }
                // } added by cychoi 2013.07.03
                else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU)
                {
                    // { added by cychoi 2013.10.11 for ITS 195176 Video Size on Display Setting
                    if(m_bIsLaunchingScreenSettings != VIDEO_DISPLAY_MAX)//modified by edo.lee 2013.12.13 ITS 0211957
                    {
                        // { modified by cychoi 2013.11.06 for ITS 207042 Ratio
                        int y = (aDisplay == DISPLAY_REAR) ? 720 : 0;
                        ASPECT_RATIO_T currAspectRatio;
                        currAspectRatio = m_pController->getAspectRatio();
                        switch(currAspectRatio)
                        {
                        case ASPECT_RATIO_FULL:
                        case ASPECT_RATIO_16_9:
                            m_pController->SetVideoWidgetSize( 0, y, 1280, 720 );
                            break;
                        case ASPECT_RATIO_4_3:
                            m_pController->SetVideoWidgetSize( 160, y, 960, 720 );
                            break;
                        default:
                            break;
                        }
                        // } modified by cychoi 2013.11.06
                    }
                    else
                    {
                        int y = (aDisplay == DISPLAY_REAR) ? 210 + 720 : 210;
                        // { added by cychoi 2013.07.03 for ITS 177711 
                        if(getEngineMain().middleEast())
                            m_pController->SetVideoWidgetSize( 401 , y, 780, 435 );
                        else
                        // } added by cychoi 2013.07.03
                            m_pController->SetVideoWidgetSize( 94 , y, 780, 435 );
                        setFullScreenState(false); // modified by cychoi 2013.07.03 for ITS 177711 
                    }
                    // } added by cychoi 2013.10.11
                }
                // { commented by cychoi 2013.08.05 for List View spec out
                //else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_CHAPTERS)
                //{
                //    int y = (aDisplay == DISPLAY_REAR) ? 319 + 720 : 319;
                //    if(getEngineMain().middleEast())
                //        m_pController->SetVideoSizeWrtDisplay( 53, y, 500, 300 );
                //    else
                //        m_pController->SetVideoSizeWrtDisplay( 727, y, 500, 300 );
                //}
                // } commented by cychoi 2013.08.05
                break;
            }
            case MEDIA_SOURCE_VCD:
            {
                // { added by cychoi 2013.07.03 for ITS 177711 
                if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL || // added by cychoi 2013.12.04 for ITS 212759
                   topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK) // added by cychoi 2013.12.04 for ITS 212759
                {
                    int y = (aDisplay == DISPLAY_REAR) ? 720 : 0;
                    ASPECT_RATIO_T currAspectRatio;
                    currAspectRatio = m_pController->getAspectRatio();
                    switch(currAspectRatio)
                    {
                    case ASPECT_RATIO_FULL:
                    case ASPECT_RATIO_16_9:
                        m_pController->SetVideoWidgetSize( 0, y, 1280, 720 );
                        break;
                    case ASPECT_RATIO_4_3:
                        m_pController->SetVideoWidgetSize( 160, y, 960, 720 );
                        break;
                    default:
                        break;
                    }
                }
                // } added by cychoi 2013.07.03
                else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU)
                {
                    int y = (aDisplay == DISPLAY_REAR) ? 253 + 720 : 253;
                    // { modified by cychoi 2013.11.12 for SmokeTest Video size error when returned form DisplaySetting & SoundSetting
                    if(getEngineMain().middleEast())
                        m_pController->SetVideoWidgetSize( 688, y, 565, 380 );
                    else
                        m_pController->SetVideoWidgetSize( 27, y, 565, 380 );
                    // } modified by cychoi 2013.11.12
                }
                // { commented by cychoi 2013.08.05 for List View spec out
                //else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_CHAPTERS)
                //{
                //    int y = (aDisplay == DISPLAY_REAR) ? 319 + 720 : 319;
                //    if(getEngineMain().middleEast())
                //        m_pController->SetVideoSizeWrtDisplay( 53, y, 500, 300 );
                //    else
                //        m_pController->SetVideoSizeWrtDisplay( 727, y, 500, 300 );
                //}
                // } commented by cychoi 2013.08.05
                break;
            }
            case MEDIA_SOURCE_AUX1: // added by cychoi 2013.12.04 for ITS 212759
            case MEDIA_SOURCE_AUX2: // added by cychoi 2013.12.04 for ITS 212759
            {
                // { added by cychoi 2013.07.03 for ITS 177711 
                if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL || // added by cychoi 2013.12.04 for ITS 212759
                   topScreenName == AppMediaPlayer_Video_Screens::SCREEN_AUX1_PLAYBACK || // added by cychoi 2013.12.04 for ITS 212759
                   topScreenName == AppMediaPlayer_Video_Screens::SCREEN_AUX2_PLAYBACK) // added by cychoi 2013.12.04 for ITS 212759
                {
                    int y = (aDisplay == DISPLAY_REAR) ? 720 : 0;
                    ASPECT_RATIO_T currAspectRatio;
                    currAspectRatio = m_pController->getAspectRatio();
                    switch(currAspectRatio)
                    {
                    case ASPECT_RATIO_FULL:
                    case ASPECT_RATIO_16_9:
                        m_pController->SetVideoWidgetSize( 0, y, 1280, 720 );
                        break;
                    case ASPECT_RATIO_4_3:
                        m_pController->SetVideoWidgetSize( 160, y, 960, 720 );
                        break;
                    default:
                        break;
                    }
                }
                // } added by cychoi 2013.07.03
                break;
            }
            default: // make compiler happy
                break;		
        }
    }
    // } modified by cychoi 2013.08.05
    // } modified by cychoi 2013.10.28
}
// } added by cychoi 2013.05.16


// { modified by kihyung 2013.07.10 for ITS 0173538 
bool AppMediaPlayer_EngineVideo::showSubTitle(bool bShow)
{
    MP_LOG << bShow << LOG_ENDL;
    if(m_pController) {
        AppMediaPlayer_Video_ControllerBase *p = m_pController->getCurrentController();
        if(p) {
            return p->showSubTitle(bShow);
        }
    }

    return false;
}
// } modified by kihyung 2013.07.10 for ITS 0173538 

// removed by Sergey 26.07.2013 for ITS#181096

//added by edo.lee 2013.06.23
void AppMediaPlayer_EngineVideo::LaunchSubScreenOnVideo(SUBSCREEN_T screen, int display)
{
    MP_LOG << screen << LOG_ENDL;
    //if(getIsShowLockoutView() == VIDEO_DISPLAY_MAX) return;
    DISPLAY_T disp = display == VIDEO_DISPLAY_FRONT ? DISPLAY_FRONT :DISPLAY_REAR ;
    //m_EngineMain.setLaunchSubscreen(true);
    APP_ENTRY_POINT_T subscreen_entry;

    switch(screen)
    {
    case VIDEO_FS_CAPTIONS :
        subscreen_entry = eAPP_AVP_VIDEO_CAPTION_SETTING ;
        break;
    case VIDEO_FS_DIVX:
        subscreen_entry = eAPP_AVP_VIDEO_DIVX_SETTING  ;
        break;
    case VIDEO_DVD_SETTING:
        setTempMode(getEngineMain().getSubCreenName() != AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION); // added by cychoi 2014.02.14 for DVD SearchCaption local Temporal Mode
        subscreen_entry = eAPP_AVP_DVD_SETTING  ;
        break;
    case VIDEO_VCD_PBC:
        subscreen_entry = eAPP_AVP_VCD_PBC_MENU_SETTING  ;
        break;
    case VIDEO_VCD_SHORTCUT:
        subscreen_entry = eAPP_AVP_VCD_SHORTCUT_SETTING  ;
        break;
    default:
        // invalid screen, just return.
        return;
    }

    Event event( EVT_KEY_SOFT_AVP,m_EngineMain.GetThisAppId(), APP_UI_MANAGER, subscreen_entry );
    emit NotifyUISH(event,disp);
}

void AppMediaPlayer_EngineVideo::HideSubScreenOnVideo(int display)
{
    MP_LOG << LOG_ENDL;
	//m_EngineMain.setLaunchSubscreen(false);
    DISPLAY_T disp = display == VIDEO_DISPLAY_FRONT? DISPLAY_FRONT :DISPLAY_REAR ;	
	//modified by edo.lee 2013.08.16 fixed  back event error
	APP_ENTRY_POINT_T pre_entry = m_EngineMain.GetAVPEntryPoint(disp);
    APP_ENTRY_POINT_T last_entry = pre_entry==0?m_EngineMain.GetLastAVEntryPoint(false):pre_entry;
	//modified by edo.lee 2013.08.16 
	// { added by edo.lee 2013.08.27 for DRS  flickering
	MP_LOG<<"disp " <<disp<<"lockout "<<getIsShowLockoutView() << LOG_ENDL;
	if(getIsShowLockoutView() == VIDEO_DISPLAY_MAX ||
		(disp == DISPLAY_FRONT && getIsShowLockoutView() != VIDEO_DISPLAY_FRONT)||
		(disp == DISPLAY_REAR && getIsShowLockoutView() != VIDEO_DISPLAY_REAR))
	{
		switch(last_entry)
		{
			case eAPP_AVP_VIDEO_MODE_DISC_DRS:
				last_entry = eAPP_AVP_VIDEO_MODE_DISC;
				break;
			case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
				last_entry = eAPP_AVP_VIDEO_MODE_JUKEBOX;
				break;
			case eAPP_AVP_VIDEO_MODE_USB1_DRS:
				last_entry = eAPP_AVP_VIDEO_MODE_USB1;
				break;
			case eAPP_AVP_VIDEO_MODE_USB2_DRS:
				last_entry = eAPP_AVP_VIDEO_MODE_USB2;
				break;
			case eAPP_AVP_VIDEO_MODE_AUX_DRS:
				last_entry = eAPP_AVP_VIDEO_MODE_AUX;
				break;
            default: // make compiler happy
                break;		
		}
	}
	// } added by edo.lee 2013.08.27
	MP_LOG<<"last " << last_entry << LOG_ENDL;

        //{added for ITS 233081 2014.04.04
        if (!(last_entry == eAPP_AVP_VIDEO_MODE_DISC || last_entry == eAPP_AVP_VIDEO_MODE_DISC_DRS)  && !(m_EngineMain.getIsTermOnTrackChange()))
        {
            if(!isDRSEntryPoint(last_entry))//added for ITS 234544 2014.04.15
                GetVideoController()->showVideo(disp);//added for ITS 234544 2014.04.15

            emit fgLoading(false);
        }
        //}added for ITS 233081 2014.04.04

    Event event( EVT_KEY_SOFT_AVP,m_EngineMain.GetThisAppId(), APP_UI_MANAGER, last_entry );
    emit NotifyUISH(event,disp);

	//Event event_back( EVT_KEY_SOFT_BACK, m_EngineMain.GetThisAppId(), APP_UI_MANAGER );
    //emit NotifyUISH (event_back, DISPLAY_FRONT);
}


void AppMediaPlayer_EngineVideo::SetSystemMute()
{
	m_pController->SetSystemMute();
}
//added by edo.lee 2013.06.23
// { added by Sergey 01.05.2013 for Tune Functionality
bool AppMediaPlayer_EngineVideo::shouldHandleJogTuneEvent(DISPLAY_T display)
{
	bool ret = true;

	//{modified by edo.lee 2013.03.03
	bool isFront = false;
	if(m_EngineMain.getCloneState() == CLONE_ON_FRONT)
		isFront = true;
	else if (m_EngineMain.getCloneState() == CLONE_ON_REAR) //modified by edo.lee 2013.03.30
		isFront = false;
	else
	{
		if(display == DISPLAY_FRONT)
			isFront = true;
	}
	//}modified by edo.lee
	if(getVPWheelDisable(isFront) == true) ret = false;

	return ret;
}
// } added by Sergey 01.05.2013 for Tune Functionality

void AppMediaPlayer_EngineVideo::setRewReachedFirst(bool value)
{
	m_bRewReachedFirst = value;
	emit rewReachedFirstChanged(m_bRewReachedFirst);
} // added by Sergey 28.05.2013

void AppMediaPlayer_EngineVideo::onStateChangedForGstPipe(int nState)
{
    MP_HIGH << nState << LOG_ENDL;
    if(nState == PLAYER_STOP && m_EngineMain.getReleaseGstFromVideo() == true)
    {
        m_EngineMain.setReleaseGstFromVideo(false);
        m_EngineMain.setGstPipelineStatus(true);
    }
    // { modified by kihyung 2013.07.16 for ISV 87564
    else if(nState == PLAYER_PLAY && m_EngineMain.getReleaseGstFromVideo() == true)
    {
        m_EngineMain.setReleaseGstFromVideo(false);
    }
    // } modified by kihyung 2013.07.16 for ISV 87564
}

void AppMediaPlayer_EngineVideo::onAuxStateChangedForGstPipe(int nState)
{
    MP_HIGH << nState << LOG_ENDL;
    
    m_nAuxPlaybackState = nState;
    //modify by shkim ITS_177764
//    if(/*nState == PLAYER_STOP &&*/ m_EngineMain.getReleaseGstFromVideo() == true)
//    {
//        usleep(500000);
//        usleep(500000);
       
//        m_EngineMain.setReleaseGstFromVideo(false);
//        m_EngineMain.setGstPipelineStatus(true);
//    }
}

//{ modified by cychoi 2013.08.06 for Position OSD is displayed on DVD Setting from Title/Disc Menu
//{ modified by yongkyun.lee 2013-07-22 for : NO CR DVD title OSD
bool AppMediaPlayer_EngineVideo::getScreenDVDMenuTitle(VIDEO_DISPLAY_T disp)
{
    // { modiifed by cychoi 2013.09.24 for ITS 188786
    if(m_pController->getSource() != MEDIA_SOURCE_DVDVIDEO)
    {
        return false;
    }
    // } modiifed by cychoi 2013.09.24

    if(disp == VIDEO_DISPLAY_FRONT || disp == VIDEO_DISPLAY_MAX)
    {
        QString topScreenName = getScreenManager(VIDEO_DISPLAY_FRONT)->topScreenName();
        if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU ||
           ((topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS || topScreenName == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS) &&
            getScreenManager(VIDEO_DISPLAY_FRONT)->getPreviousScreenName(1) == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU) ||
           (topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION &&
            getScreenManager(VIDEO_DISPLAY_FRONT)->getPreviousScreenName(2) == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU))
        {
            return true;
        }
    }

    if(disp == VIDEO_DISPLAY_REAR || disp == VIDEO_DISPLAY_MAX)
    {
        QString topScreenName = getScreenManager(VIDEO_DISPLAY_REAR)->topScreenName();
        if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU ||
           ((topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS || topScreenName == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS) &&
            getScreenManager(VIDEO_DISPLAY_REAR)->getPreviousScreenName(1) == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU) ||
           (topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION &&
            getScreenManager(VIDEO_DISPLAY_REAR)->getPreviousScreenName(2) == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU))
        {
            return true;
        }
    }

    return false;
}
//} modified by yongkyun.lee 2013-07-22 
//} modified by cychoi 2013.08.06

//{ added by cychoi 2013.09.04 for ITS 186733
bool AppMediaPlayer_EngineVideo::isDVDTrackPlaying()
{
    //{ modified by cychoi 2013.09.09 for ITS 188978 & 189116
    MEDIA_SOURCE_T source = GetVideoController()->getSource();
    if(source != MEDIA_SOURCE_DVDVIDEO)
    {
        return false;
    }

    DVDStatusResponse DVDInfo;
    // { modified by cychoi 2014.06.27 for removed too many logs...
    IVideoInterface *pIVideo = m_pIVideo;
    if(pIVideo == NULL)
    {
        pIVideo = m_pController->GetControllers().GetDeckController()->GetVideoInstance();
    }
    // } modified by cychoi 2014.06.27
    if(pIVideo == NULL || pIVideo->GetDVDStatus(&DVDInfo) == -1)
    {
        return false;
    }
    //} modified by cychoi 2013.09.09

    if(DVDInfo.trackOrChapterNo == 0 || DVDInfo.trackOrChapterNo == 0xFFF)
    {
        return false;
    }

    return true;
}
//} added by cychoi 2013.09.04

//added by edo.lee 2013.07.30 subscreen
bool AppMediaPlayer_EngineVideo::isVideoSubScreen(QString screenName)
{
	MP_LOG<<screenName << LOG_ENDL;
	if( AppMediaPlayer_Video_Screens::SCREEN_DIVXREG == screenName
		||AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS == screenName
		||AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS == screenName
		||AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION== screenName //added by edo.lee 2013.08.16 for drs subscreen
		||AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU== screenName) // added by edo.lee 2013.08.12 for drs subscreen
			return true;

	return false;
}
//added by edo.lee 2013.07.30 subscreen
int AppMediaPlayer_EngineVideo::getOSDRandom( int random)
{	
	switch(random)
	{
		case 0:
			return 3;
		case 1:
			return 5;			
		case 2:
			return 4;
        default:
            return 3; // added by kihyung 2013.08.01 for static test
	}
}
int AppMediaPlayer_EngineVideo::getOSDRepeat(int repeat)
{	
	if(repeat == -1)
		return 6;
    
	switch(repeat)
	{
		case 0:
			return 9;
		case 1:
			return 8;			
		case 2:
			return 7;
        default: 
            return 6; // added by kihyung 2013.08.01 for static test
	}
}

void AppMediaPlayer_EngineVideo::setOSDMetaData(OSD_DEVICE device, QString title, int shuffle , int repeat, int scan, bool isTune)
{
	emit setOSDMetaDataChanged( device,  title,  getOSDRandom(shuffle), getOSDRepeat(repeat), scan,  isTune);
}
void AppMediaPlayer_EngineVideo::updatePosition(OSD_DEVICE device , int position, bool isLong)
{
    // { modiifed by cychoi 2013.09.24 for ITS 188786 //{ modified by yongkyun.lee 2013-07-22 for : 
    if(device == DISC_DVD_VIDEO) // modified by cychoi 2013.11.12 for OSD Title Translation
    {
        if(getScreenDVDMenuTitle() && !isDVDTrackPlaying()) // added by cychoi 2013.09.04 for ITS 186733
            return;
    }
    // } modiifed by cychoi 2013.09.24 //} modified by yongkyun.lee 2013-07-22 
	emit updatePositionChanged( device,  position, isLong);
}
void AppMediaPlayer_EngineVideo::updatePlayingMode(OSD_DEVICE device ,int shuffle , int repeat, int scan)
{
	emit updatePlayingModeChanged( device,  getOSDRandom(shuffle) ,getOSDRepeat(repeat), scan);
}
void AppMediaPlayer_EngineVideo::showCurrentOSD()
{
	emit showCurrentOSDChanged();
}
void AppMediaPlayer_EngineVideo::updateOSDMetaData(OSD_DEVICE device , QString title)
{
	emit updateOSDMetaDataChanged(device , title);
}
//[KOR][ITS][180278](aettie.ji)
bool AppMediaPlayer_EngineVideo::GetVariantRearUSB()
{
     return ( m_EngineMain.GetVariantRearUSB() );
}

// { added by Sergey for ITS#181872 30.07.2013
DISPLAY_T AppMediaPlayer_EngineVideo::actualDisp(DISPLAY_T disp)
{
    if(disp != DISPLAY_FRONT && disp != DISPLAY_REAR)
    {
        MP_LOG << "Unsupported DISPLAY id. Return DISPLAY_FRONT." << LOG_ENDL;
        return DISPLAY_FRONT;
    }

    DISPLAY_T result = disp;

    if(m_EngineMain.IsSwapDCEnabled() == true)
    {
        result = (disp == DISPLAY_FRONT) ? DISPLAY_REAR : DISPLAY_FRONT;
    }

    return result;
}
// } added by Sergey for ITS#181872 30.07.2013


// { added by Sergey 14.08.2013 for ITS#184081
void AppMediaPlayer_EngineVideo::onSourceReleased()
{
    for(int i = VIDEO_DISPLAY_FRONT; i <= VIDEO_DISPLAY_REAR; i++)
    {
        VIDEO_DISPLAY_T disp = (VIDEO_DISPLAY_T)i;
        QString topScreenName = getScreenManager(disp)->topScreenName();

        if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS
                || topScreenName == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON)
        {
            getScreenManager(disp)->popScreen();
        }
    }
}
// } added by Sergey 14.08.2013 for ITS#184081



 // { added by Sergey 15.08.2013 for ITS#185556 
void AppMediaPlayer_EngineVideo::activateFocus(DISPLAY_T disp)
{
#ifndef DUAL_KEY // modified by cychoi 2014.02.04 for Prevent Fix
    QObject *uiListener = m_EngineMain.GetDeclarativeView(disp == DISPLAY_FRONT ? AppEngineQMLConstants::APP_FRONT_SCREEN : AppEngineQMLConstants::APP_REAR_SCREEN);
#endif
    VIDEO_DISPLAY_T videoDisp =	(disp == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;

    m_pScreenManager[videoDisp]->showFocus();
#ifndef DUAL_KEY
    m_pScreenManager[videoDisp]->setJogSource(uiListener);
#elif DUAL_KEY
    /* if(getEngineMain().getIsSwapEnabled() && isVideoInFG(disp) && videoDisp == VIDEO_DISPLAY_FRONT || getEngineMain().getCloneState() == CLONE_ON_REAR)
        m_pScreenManager[VIDEO_DISPLAY_REAR]->setJogSource(m_EngineMain.GetEngine(APP_MEDIA_MODE_VIDEO));
    else if(getEngineMain().getIsSwapEnabled() && isVideoInFG(disp) && videoDisp == VIDEO_DISPLAY_REAR)
        m_pScreenManager[VIDEO_DISPLAY_FRONT]->setJogSource(m_EngineMain.GetEngine(APP_MEDIA_MODE_VIDEO));
    else*/
        m_pScreenManager[videoDisp]->setJogSource(m_EngineMain.GetEngine(APP_MEDIA_MODE_VIDEO));
#endif
}


void AppMediaPlayer_EngineVideo::deactivateFocus(DISPLAY_T disp)
{
	VIDEO_DISPLAY_T videoDisp =	(disp == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;

	m_pScreenManager[videoDisp]->setJogSource(NULL);
	m_pScreenManager[videoDisp]->hideFocus();
}
 // } added by Sergey 15.08.2013 for ITS#185556 


 // { added by Sergey 01.09.2013 for ITS#186834 
POPUP_TXT_BTN_TYPE_T AppMediaPlayer_EngineVideo::popupType(DISPLAY_T disp)
{
    POPUP_TXT_BTN_TYPE_T result = POPUP_TXT_BTN_MAX;

    VIDEO_DISPLAY_T videoDisp =	(disp == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;

    if(m_pScreenManager[videoDisp]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON)
    {
        AppMediaPlayer_Video_ScreenTextButtonPopup* popup = dynamic_cast<AppMediaPlayer_Video_ScreenTextButtonPopup*>(m_pScreenManager[videoDisp]->topScreen());
        if(popup != NULL)
        {
            result = popup->popupType();
        }
    }

    return result;
}


// { modified by Sergey 09.10.2013 for ITS#194391
bool AppMediaPlayer_EngineVideo::isUnsupportedPopup(DISPLAY_T disp)
{
    Q_UNUSED(disp);

    for(int i = DISPLAY_FRONT; i <= DISPLAY_REAR; i++)
    {
        POPUP_TXT_BTN_TYPE_T popup = popupType((DISPLAY_T)i);

        // { modified by Sergey 10.11.2013 new DivX popups
        if(popup == POPUP_TXT_BTN_UNSUPPORTED_FILE
                || popup == POPUP_TXT_BTN_AUDIO_FORMAT_UNSUPPORTED
                || popup == POPUP_TXT_BTN_RESOLUTION_UNSUPPORTED
                || popup == POPUP_TXT_BTN_DRM_CONFIRM
                || popup == POPUP_TXT_BTN_DRM_EXPIRED
                || popup == POPUP_TXT_BTN_DRM_NOT_AUTH
                || popup == POPUP_TXT_BTN_UNSUPPORTED_ALL)
        // } modified by Sergey 10.11.2013 new DivX popups
        {
            return true;
        }
    }

    return false;
}
// } modified by Sergey 09.10.2013 for ITS#194391
 // } added by Sergey 01.09.2013 for ITS#186834 


 // { added by Sergey 03.09.2013 for ITS#186054
void AppMediaPlayer_EngineVideo::registerScreens()
{
	for(int disp = VIDEO_DISPLAY_FRONT; disp < VIDEO_DISPLAY_MAX; disp++)
	{

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
											   new AppMediaPlayer_Video_OverlaySourceModeArea( m_pController,
																							  this,
																							  disp,
																							  this ));


		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_JUKEBOX_PLAYBACK,
											   new AppMediaPlayer_Video_ScreenFSPlayback( m_pController,
																						 this, // engine
																						 MEDIA_SOURCE_JUKEBOX,
																						 disp,
																						 this ));

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_USB_PLAYBACK,
											   new AppMediaPlayer_Video_ScreenFSPlayback( m_pController,
																						 this, // engine
																						 MEDIA_SOURCE_USB1,
																						 disp,
																						 this));

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_USB2_PLAYBACK,
											   new AppMediaPlayer_Video_ScreenFSPlayback( m_pController,
																						 this, // engine
																						 MEDIA_SOURCE_USB2,
																						 disp,
																						 this));

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK,
											   new AppMediaPlayer_Video_ScreenDVDPlayback( m_pController,
																						  this,
																						  disp,
																						  this ));

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
											   new AppMediaPlayer_Video_ScreenVCDPlayback( m_pController,
																						  this,
																						  disp,
																						  this ));

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU,
											   new AppMediaPlayer_Video_ScreenVCDPBCMenu( m_pController,
																						 this,
																						 disp,
																						 this ));

		// modified by ravikanth 13-03-13
		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION,
											   new ScreenDVDSearchCaption( m_pController,
																		  this,
																		  disp, // added by lssanh 2013.04.27 ITS165097
																		  this ));

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS,
											   new AppMediaPlayer_Video_ScreenVCDDirectAccess( m_pController,
																							  this,
																							  disp, // added by yungi 2013.09.07 for ITS 187311
																							  this ));


		// { commented by cychoi 2013.07.13 for List View spec out
		//m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLES,
		//                                  new AppMediaPlayer_Video_ScreenTitlesChapters( m_pController,
		//                                                                                 this,
		//                                                                                 disp, // added by lssanh 2013.06.03 ITS170473 Lockout Mode on chapter
		//                                                                                 this ));
		// } commented by cychoi 2013.07.13

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS,
											   new AppMediaPlayer_Video_ScreenDVDSettings( m_pController,
																						  this,
																						  disp, // added by cychoi 2013.08.20 for ITS 178895 SerachCaption subscreen
																						  this ));

		// { commented by cychoi 2013.07.13 for List View spec out
		//m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_DVD_CHAPTERS,
		//                                  new AppMediaPlayer_Video_ScreenTitlesChapters( m_pController,
		//                                                                                 this,
		//                                                                                 disp,// added by lssanh 2013.06.03 ITS170473 Lockout Mode on chapter
		//                                                                                 this,
		//                                                                                 AppMediaPlayer_Video_Screens::VIEW_DVD_CHAPTERS));
		// } commented by cychoi 2013.07.13

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
											   new AppMediaPlayer_Video_ScreenOptions( m_pController,
																					  this,
																					  disp,
																					  this ));


		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_AUX1_PLAYBACK,
											   new AppMediaPlayer_Video_ScreenAUXPlayback( m_pController,
																						  this,
																						  true,
																						  disp,
																						  this));
		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_AUX2_PLAYBACK,
											   new AppMediaPlayer_Video_ScreenAUXPlayback( m_pController,
																						  this,
																						  false,
																						  disp,
																						  this));

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS,
											   new AppMediaPlayer_Video_ScreenCaption( m_pController,
																					  this,  // engine
																					  (VIDEO_DISPLAY_T)disp,  // added by Sergey 20.08.2013 for ITS#184640
																					  this ));

		// { commented by cychoi 2013.07.13 for List View spec out
		//m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_VCD_CHAPTERS,
		//                                  new AppMediaPlayer_Video_ScreenVCDChapters( m_pController,
		//                                                                              this,
		//                                                                              disp,// added by lssanh 2013.06.03 ITS170473 Lockout Mode on chapter
		//                                                                              this));
		// } commented by cychoi 2013.07.13

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_FS_FILEINFO_POPUP,
											   new AppMediaPlayer_Video_ScreenFSFileInfo( m_pController,
																						 this,
																						 this ));


		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_POP_PROGRESS,
											   new AppMediaPlayer_Video_ScreenPopProgress( m_pController,
																						  this,
																						  this ));

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_POP_TOAST,
											   new AppMediaPlayer_Video_ScreenPopToast( m_pController,
																					   this,
																					   this ));

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU,
											   new AppMediaPlayer_Video_ScreenTitleDiscMenu( m_pController,
																							this,
																							disp,
																							this ));

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_FULL,
											   new AppMediaPlayer_Video_ScreenFull( m_pController,
																				   this,  //engine
																				   disp,
																				   this ));

		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON,
											   new AppMediaPlayer_Video_ScreenTextButtonPopup( m_pController,
																							  this,
																							  disp,
																							  this));


		m_pScreenManager[disp]->registerScreen( AppMediaPlayer_Video_Screens::SCREEN_DIVXREG,
											   new ScreenDivxReg(this, disp, this));

		connect( m_pScreenManager[disp], SIGNAL(hideUI()), this, SLOT(onHideUI()) );
		connect( m_pScreenManager[disp], SIGNAL(popFullScreen()), this, SLOT(onPopScreen()) );
	}
}
// } added by Sergey 03.09.2013 for ITS#186054



// { added by Sergey 07.09.2013 for PRE_FG supoort
AppMediaPlayer_Video_ScreenManager*  AppMediaPlayer_EngineVideo::getScrMgr(DISPLAY_T display)
{
    VIDEO_DISPLAY_T vDisp = (display == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;

    return m_pScreenManager[vDisp];
}


AppMediaPlayer_Video_ScreenManager*  AppMediaPlayer_EngineVideo::getScreenManager(int disp)
{	
    VIDEO_DISPLAY_T type = (VIDEO_DISPLAY_T)disp   ;
   return m_pScreenManager[type];
}



MODE_STATE_T AppMediaPlayer_EngineVideo::mapSourceToMode(MEDIA_SOURCE_T source)
{
    MODE_STATE_T result = MODE_STATE_NONE;

    switch(source)
    {
        case MEDIA_SOURCE_JUKEBOX:
        {
            result = MODE_STATE_JUKEBOX_VIDEO;
            break;
        }
        case MEDIA_SOURCE_USB1:
        {
            result = MODE_STATE_USB1_VIDEO;
            break;
        }
        case MEDIA_SOURCE_USB2:
        {
            result = MODE_STATE_USB2_VIDEO;
            break;
        }
        case MEDIA_SOURCE_DVDVIDEO:
        case MEDIA_SOURCE_VCD:
        {
            result = MODE_STATE_DISC_VIDEO;
            break;
        }
        case MEDIA_SOURCE_AUX1:
        {
            result = MODE_STATE_AUX1_VIDEO;
            break;
        }
        default:
        {
            MP_LOG << "Not video source =" << source << LOG_ENDL;
            break;
        }
    }

    return result;
}



/*
 * Maps entry point to video SOURCE.
 */
MEDIA_SOURCE_T AppMediaPlayer_EngineVideo::mapEPToSource(APP_ENTRY_POINT_T ep)
{
    MEDIA_SOURCE_T source = MEDIA_SOURCE_MAX;

    switch(ep)
    {
    case eAPP_AVP_VIDEO_MODE_JUKEBOX:
    case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS:
    {
        source = MEDIA_SOURCE_JUKEBOX;
        break;
    }

    case eAPP_AVP_VIDEO_MODE_USB1_DRS:
    case eAPP_AVP_VIDEO_MODE_USB1:
    {
        source = MEDIA_SOURCE_USB1;
        break;
    }

    case eAPP_AVP_VIDEO_MODE_USB2:
    case eAPP_AVP_VIDEO_MODE_USB2_DRS:
    {
        source = MEDIA_SOURCE_USB2;
        break;
    }

    case eAPP_AVP_VIDEO_MODE_DISC:
    case eAPP_AVP_VIDEO_MODE_DISC_DRS:
    {
        if(m_EngineMain.GetMediaManager()->IsSourceAvailable(MEDIA_SOURCE_DVDVIDEO, MEDIA_TYPE_VIDEO))
        {
            source = MEDIA_SOURCE_DVDVIDEO;
        }

        if(m_EngineMain.GetMediaManager()->IsSourceAvailable(MEDIA_SOURCE_VCD, MEDIA_TYPE_VIDEO))
        {
            source = MEDIA_SOURCE_VCD;
        }

        break;
    }

    case eAPP_AVP_VIDEO_MODE_AUX:
    case eAPP_AVP_VIDEO_MODE_AUX_DRS:
    {
        source = MEDIA_SOURCE_AUX1;
        break;
    }

    case eAPP_AVP_VIDEO_CAPTION_SETTING:
    case eAPP_AVP_VIDEO_DIVX_SETTING:
    case eAPP_AVP_DVD_SETTING:
    case eAPP_AVP_VCD_PBC_MENU_SETTING:
    case eAPP_AVP_VCD_SHORTCUT_SETTING:
    case eAPP_MODE_LAST_STATE:
    case eAPP_AVP_VIDEO:
    {
        if(m_pController->getSource() != MEDIA_SOURCE_MAX)
            source = m_pController->getSource();
        else
            source = m_pController->getLastSource();

        if(source == MEDIA_SOURCE_MAX)
            source = MEDIA_SOURCE_JUKEBOX;

        break;
    }

    default:
    {
        source = MEDIA_SOURCE_MAX;
        break;
    }
    }

    return source;
}



/*
 * Maps MODE_STATE to video SOURCE.
 */
MEDIA_SOURCE_T AppMediaPlayer_EngineVideo::mapModeStateToSource(MODE_STATE_T mode)
{
    MEDIA_SOURCE_T source = MEDIA_SOURCE_MAX;

    switch( mode )
    {
        case MODE_STATE_JUKEBOX_VIDEO:
        {
            source = MEDIA_SOURCE_JUKEBOX;
            break;
        }
        case MODE_STATE_USB1_VIDEO:
        {
            source = MEDIA_SOURCE_USB1;
            break;
        }
        case MODE_STATE_USB2_VIDEO:
        {
            source = MEDIA_SOURCE_USB2;
            break;
        }
        case MODE_STATE_DISC_VIDEO:
        {
            if( m_EngineMain.GetMediaManager()->IsSourceAvailable(MEDIA_SOURCE_DVDVIDEO,
                                                                  MEDIA_TYPE_VIDEO) )
            {
                source = MEDIA_SOURCE_DVDVIDEO;
            }

            if( m_EngineMain.GetMediaManager()->IsSourceAvailable(MEDIA_SOURCE_VCD,
                                                                  MEDIA_TYPE_VIDEO) )
            {
                source = MEDIA_SOURCE_VCD;
            }

            break;
        }
        case MODE_STATE_AUX1_VIDEO:
        {
            source = MEDIA_SOURCE_AUX1;
            break;
        }
        case MODE_STATE_AUX2_VIDEO:
        {
            source = MEDIA_SOURCE_AUX2;
            break;
        }
        default:
        {
            source = MEDIA_SOURCE_MAX;
            break;
        }
    }

    return source;
}



/*
 * Maps MEDIA_SOURCE to video screen.
 */
QString AppMediaPlayer_EngineVideo::mapSourceToPlaybackScreen(MEDIA_SOURCE_T source)
{
    QString result = "";

    switch(source)
    {
        case MEDIA_SOURCE_JUKEBOX:
        {
            result = AppMediaPlayer_Video_Screens::SCREEN_JUKEBOX_PLAYBACK;

            break;
        }
        case MEDIA_SOURCE_USB1:
        {
            result = AppMediaPlayer_Video_Screens::SCREEN_USB_PLAYBACK;

            break;
        }
        case MEDIA_SOURCE_USB2:
        {
            result = AppMediaPlayer_Video_Screens::SCREEN_USB2_PLAYBACK;

            break;
        }
        case MEDIA_SOURCE_DVDVIDEO:
        {
            // { modified by cychoi 2013.09.24 for ITS 191333
            AppMediaPlayer_Video_ControllerDVD *pDVD = (AppMediaPlayer_Video_ControllerDVD*)m_pController->getController(MEDIA_SOURCE_DVDVIDEO);
            if(pDVD->isScreenDVDMenuTitleState() == true)
            {
                result = AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU;
            }
            else
            {
                result = AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK;
            }
            // } modified by cychoi 2013.09.24

            break;
        }
        case MEDIA_SOURCE_VCD:
        {
            result = AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK;

            break;
        }
        case MEDIA_SOURCE_AUX1:
        {
            result = AppMediaPlayer_Video_Screens::SCREEN_AUX1_PLAYBACK;

            break;
        }
        default:
        {
            MP_LOG << "No video screen for source =" << source << LOG_ENDL;
        }
    }

    return result;
}



/*
 *  Switches video controller to given source. If SOURCE_MAX releases current source.
 */
void AppMediaPlayer_EngineVideo::switchSource(MEDIA_SOURCE_T source)
{
	if(source == MEDIA_SOURCE_MAX)
	{
		GetVideoController()->releaseSource();
	}
	else if(IsSourceAvailable(source))
	{
		GetVideoController()->setSource(source);
	}

	setLastAVMode(mapSourceToMode(GetVideoController()->getSource()));
}

void AppMediaPlayer_EngineVideo::switchScreenOnFG(MEDIA_SOURCE_T source, DISPLAY_T display)
{
    AppMediaPlayer_Video_ScreenManager* sm = getScrMgr(display);
    //removed yungi 2013.09.16 for SmokeTest DirectAccess During DRS On 2nd //added by yungi 2013.09.12 for SmokeTest DirectAccess During DRS On
    bool bShowPlayback =  getEngineMain().IsShowLoadingFlag();
    QString playbackScreen = mapSourceToPlaybackScreen(source);
    QString topScreenName =  sm->topScreenName();
    QString subName =  getEngineMain().getSubCreenName();
    bool bFgLoadingMode = true; // added by sjhyun 2013.11.16 for ITS 209663

    if(((topScreenName == ""|| isFMFile(display))  && subName == "") //modified by edo.lee 2013.11.13 ITS 0208900 
            || ( isPbcMode(source, topScreenName) && getEngineMain().IsChangeToParking() ) // modified by sjhyun for ITS 195523 // added by sjhyun 2013.10.08 for ITS 193794
            || (bShowPlayback
                && (!getEngineMain().IsChangeToDriving() && !getEngineMain().IsChangeToParking()) // added by edo.lee 2013.09.14 keep full on DRS
                && (source != GetVideoController()->getSource()
                    || topScreenName != AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON))) // modified by Sergey 06.08.2013 for ITS#182902
    {
        emit fsAnimation(false); // added by Sergey 26.09.2013 for ITS#191542
        // { modified by cychoi 2014.04.10 for ITS 234114, ITS 234177
        if(playbackScreen == topScreenName)
        {
            // should replace screen if new playback screen == top screen
            sm->replaceScreen(playbackScreen);
        }
        else
        {
            // should change screen if new playback screen != top screen
            sm->changeScreen(playbackScreen);
        }
        // } modified by cychoi 2014.04.10
        sm->setDefaultFocus(); // added by Sergey 24.08.2013 for ITS#185556
		// moved by Sergey 18.10.2013 for Loading screen after Camera
        if (playbackScreen != AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU && // modified by cychoi 2013.10.13 for ITS 195278 DVD Title/Disc menu screen should not be Full Screen
            checkScreenToFullonDRS(display)
            && source == GetVideoController()->getSource())//added by edo.lee 2013.11.06 ITS 0206974 
        {
            emit fsAnimation(false); // modified by Sergey 26.09.2013 for ITS#191542
            sm->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FULL);
        }
        else
            sm->onNotifyScreenManager("", topScreenName, EVENTS_RESTART_FULLSCREEN_TIMER); // added by Sergey 31.08.2013 for ITS#186799
        emit fsAnimation(true); // added by Sergey 26.09.2013 for ITS#191542

        if(display == DISPLAY_FRONT)
            m_bFrontLoaded = true; // added by Sergey 24.10.2013 for ITS#197778
    }
    else if(isVideoSubScreen(subName))
    {
        if(!m_EngineMain.isTempMode() || isDVDScreenStatusKeep(source, topScreenName) == false) // modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327 // moved by cychoi 2014.03.03 for ITS 225375 // added by sjhyun 2013.10.29 for ITS 198332
        {
            //POPUP_TXT_BTN_TYPE_T tempType = m_popupTypeOnSubScreen; // added by cychoi 2014.03.19 for ITS 230153
            // { modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327
            AppMediaPlayer_Video_ScreenManager* oppositeScreenManager = getScrMgr(display == DISPLAY_FRONT ? DISPLAY_REAR : DISPLAY_FRONT);
            if(oppositeScreenManager->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
            {
                oppositeScreenManager->popScreen();
            }
            // } modified by cychoi 2014.03.13
            if( sm->getScreensCount() > 0)
            {
                sm->clearStack();
            }
            emit fsAnimation(false); // modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327 // added by Sergey 26.09.2013 for ITS#191542
            turnOffFullScreenAnimation(display);//added by edo.lee 2013.09.11
            sm->changeScreen(playbackScreen);
            //{added by edo.lee 2013.08.16
            if(subName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS)
            {
                sm->pushScreen(subName, "DISC");
                bFgLoadingMode = false; // added by sjhyun 2013.11.16 for ITS 209663
            }
            else if(subName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION)
            {
                sm->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS, "DISC");
                sm->pushScreen(subName);
                // { added by cychoi 2014.03.19 for ITS 230153
                //if(tempType == POPUP_TXT_BTN_UNAVAILABLE_LANG_CODE)
                //{
                //    VIDEO_DISPLAY_T videoDisp = display == DISPLAY_FRONT ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;
                //    showTxtBtnPopup(tempType, videoDisp);
                //}
                // } added by cychoi 2014.03.19
                bFgLoadingMode = false; // added by sjhyun 2013.11.16 for ITS 209663
            }
            else
            {
                sm->pushScreen(subName);
            }
            //}added by edo.lee 2013.08.16
        }
    }
    else if(isVideoSubScreen(topScreenName) // modified by edo.lee 2013.08.20
            || (/*bShowPlayback &&*/ topScreenName == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON // modified by cychoi 2014.03.03 for ITS 225375
                && isVideoSubScreen(sm->getPreviousScreenName(1)))) // modified by cychoi 2014.02.16 for should not keep Popup on Sub Screen
    {
        //if(bShowPlayback == false)
        //{
        //	MP_LOG<<"do not anything..keep the screen" << LOG_ENDL;
        //}else{
        turnOffFullScreenAnimation(display);
        sm->changeScreen(playbackScreen);
		// moved by Sergey 18.10.2013 for Loading screen after Camera
        //}
    }
    //added by edo.lee 2013.11.11 ITS 0208120 
    // { modified by cychoi 2014.03.12 for ITS 229158
    else if(bShowPlayback && topScreenName == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON) //modified by edo.lee 2013.11.14 ITS 209167
    {
        VIDEO_DISPLAY_T videoDisp = display == DISPLAY_FRONT ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;
        AppMediaPlayer_Video_ScreenTextButtonPopup* popup = dynamic_cast<AppMediaPlayer_Video_ScreenTextButtonPopup*>(m_pScreenManager[videoDisp]->topScreen());
        // { modified by cychoi 2013.11.16 for Prevent Fix
        if(popup)
        {
            POPUP_TXT_BTN_TYPE_T tempType = popup->popupType();
            /*if((source != GetVideoController()->getSource()) || (isDiscPopup(tempType) == true)) // modified by cychoi 2014.03.24 for ITS 230874 // modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327
            {
                sm->clearStack();		
                emit fsAnimation(false);
                sm->changeScreen(playbackScreen);
                if(isDiscPopup(tempType) == false) // modified by cychoi 2014.03.24 for ITS 230874 // modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327
                {
                    showTxtBtnPopup(tempType,videoDisp);
                }
            }*/
                        // { modified by cychoi 2014.07.29 for ITS 244377
            if((topScreenName != playbackScreen) || (isDiscPopup(tempType) == true)) // modified by cychoi 2014.03.24 for ITS 230874 // modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327
            // } modified by cychoi 2014.07.29
            {
                sm->clearStack();		
                emit fsAnimation(false);
                sm->changeScreen(playbackScreen);
                if(isDiscPopup(tempType) == false) // modified by cychoi 2014.03.24 for ITS 230874 // modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327
                {
                    showTxtBtnPopup(tempType,videoDisp);
                }
            }
        }
        // } modified by cychoi 2013.11.16
    }
    // } modified by cychoi 2014.03.12
    //added by edo.lee 2013.11.11 ITS 0208120 
    // } modified by edo.lee 2013.08.16 for drs subscreen

    // { modified by Sergey 14.12.2013. NoITS. Blinking Loading.. when press BACK HK in single DISPLAY mode.
    if(m_EngineMain.GetGstPipelinesOwner(display) != APP_AVP)
    	sm->onNotifyScreenManager("", playbackScreen, EVENTS_SET_FG_LOADING_MODE, QVariant(bFgLoadingMode)); // modified by sjhyun 2013.11.16 for ITS 209663 // moved by Sergey 18.10.2013 for Loading screen after Camera
    // } modified by Sergey 14.12.2013. NoITS. Blinking Loading.. when press BACK HK in single DISPLAY mode.
}

//{added by edo.lee 2013.09.09
bool AppMediaPlayer_EngineVideo::checkScreenToFullonDRS(DISPLAY_T display)
{    
    bool result = false;
    VIDEO_DISPLAY_T disp = display == DISPLAY_FRONT ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;
    VIDEO_DISPLAY_T other_disp = disp == VIDEO_DISPLAY_FRONT ? VIDEO_DISPLAY_REAR : VIDEO_DISPLAY_FRONT;
    DISPLAY_T other_display = display == DISPLAY_FRONT ? DISPLAY_REAR : DISPLAY_FRONT;
    if(/*getScreenManager(disp)->getIsLockoutMode()&&*///modified by edo.lee 2013.10.10 ITS 194398
       isVideoInFG(other_display) && getScreenManager(other_disp)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_FULL)
    {
        MP_LOG<<display << LOG_ENDL;
        result = true;
    }
    return result;
}
//}added by edo.lee 2013.09.09


void AppMediaPlayer_EngineVideo::showEmptySourcePopup(MEDIA_SOURCE_T source, DISPLAY_T display)
{
    QVariant popupType;
    popupType.setValue(POPUP_TXT_BTN_MAX);

    switch(source)
    {
        case MEDIA_SOURCE_JUKEBOX:
        {
            popupType.setValue(POPUP_TXT_BTN_NO_VIDEO_ON_JUKEBOX);

            break;
        }

        case MEDIA_SOURCE_USB1:
        case MEDIA_SOURCE_USB2:
        {
            popupType.setValue(POPUP_TXT_BTN_NO_VIDEO_ON_USB);

            break;
        }

        default:
        {
            MP_LOG << "No error popup for this source." << LOG_ENDL;
            break;
        }
    }

    if(popupType.value<POPUP_TXT_BTN_TYPE_T>() != POPUP_TXT_BTN_MAX)
    {
        getScrMgr(display)->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON, popupType);
    }
}


void AppMediaPlayer_EngineVideo::turnOffFullScreenAnimation(DISPLAY_T display)
{
    getScrMgr(display)->onNotifyScreenManager("", AppMediaPlayer_Video_Screens::SCREEN_FULL, EVENTS_NO_FULLSCREEN_ANIM);
}


// { added by cychoi 2014.03.13 for ITS 229176
void AppMediaPlayer_EngineVideo::setFSAnimation(bool bOn)
{
    emit fsAnimation(bOn);
}
// } added by cychoi 2014.03.13


void AppMediaPlayer_EngineVideo::handleGstPipelinesAvailable(DISPLAY_T display)
{
    MP_MEDIUM << isLaunchingScreenSettings() << getEngineMain().GetAVPEntryPoint(display) << LOG_ENDL;

    // { modified by kihyung 2014.01.14 for ITS 0218420 
    // if(isLaunchingScreenSettings() == false)
    if(isLaunchingScreenSettings() == false && getEngineMain().GetAVPEntryPoint(display) != eAPP_NONE)
    // } modified by kihyung 2014.01.14 for ITS 0218420 
    {
        MP_HIGH << "disp =" << display << " FG =" << isVideoInFG(display) << " camera =" << getEngineMain().getCamInFG() << "actualDisp =" <<  actualDisp(display) << "settings =" << isLaunchingScreenSettings() << LOG_ENDL;
        
        if(!getEngineMain().getBTCallPowerOn())//Added by taihyun.ahn 2013.10.22
        {
            DelayedPlay(true);
        }
        //{ modified by cychoi 2013.12.18 for ITS 216269
        DISPLAY_T actualDisplay = actualDisp(display);
        //{ modified by yongkyun.lee 2013-09-11 for : ITS 189173
        //{ modified by taihyun.ahn 2013-12-21 for
        if(getIsShowLockoutView() == VIDEO_DISPLAY_FRONT)
            display = DISPLAY_REAR;
        else if(getIsShowLockoutView() == VIDEO_DISPLAY_REAR)
            display = DISPLAY_FRONT;
        //}
        //} modified by yongkyun.lee 2013-09-11

        //if(isVideoInFG(actualDisplay) && (!getEngineMain().getCamInFG() || actualDisplay == DISPLAY_REAR))
        if((isVideoInFG() && !getEngineMain().getCamInFG()) || (getEngineMain().getCamInFG() &&actualDisplay == DISPLAY_REAR))//modified by taihyun.ahn 2013-12-21 
        {
            GetVideoController()->showVideo(display);//modified by taihyun.ahn 2013-12-21 
            emit setLoadingPBCMenu(false); //add by shkim 2013.11.13 for ITS 209040 Camera -> VCD PBC Menu Issue
            m_pFgLoadingTimer->stop();
            emit fgLoading(false); // modified by Sergey 15.09.2014 for ITS#189390
        }
        //} modified by cychoi 2013.12.18
    }
}



void AppMediaPlayer_EngineVideo::preSourceSwitchProcess(MEDIA_SOURCE_T source, DISPLAY_T display)
{
	Q_UNUSED(display);

	/*
	 * Add sources to this switch if some prechange operations are required for them.
	 */
	switch(source)
	{
		case MEDIA_SOURCE_AUX1:
		{
			m_EngineMain.GetMediaManager()->SetSourceAvailable(MEDIA_SOURCE_AUX1, MEDIA_TYPE_MUSIC, true); // added by lssanh 2013.05.24 ITS168455 AUX flickering
			m_EngineMain.GetMediaManager()->SetSourceAvailable(MEDIA_SOURCE_AUX1, MEDIA_TYPE_VIDEO, true); // added by lssanh 2013.05.24 ITS168455 AUX flickering
			if(!m_EngineMain.GetAuxDisconn())	//added by hyochang.ryu 20130812 for ITS183306
			showCurrentOSD();

			break;
		}


		default:
		{
			break;
		}
	}

    if(source != MEDIA_SOURCE_MAX)
    {
        m_EngineMain.SetAVOffMode(false);
    }
}



void AppMediaPlayer_EngineVideo::lateScreenChange(MEDIA_SOURCE_T source, DISPLAY_T display)
{
    if(!isVideoInFG(display))
        return;

    AppMediaPlayer_Video_ScreenManager* sm = getScrMgr(display);
    QString scrName = ""; // added by cychoi 2013.08.15 for ITS 180543


    switch(source)
    {
        case MEDIA_SOURCE_DVDVIDEO:
        {
            // { modified by cychoi 2013.09.24 for ITS 191333
            AppMediaPlayer_Video_ControllerDVD *pDVD = (AppMediaPlayer_Video_ControllerDVD*)m_pController->getController(MEDIA_SOURCE_DVDVIDEO);
            if(pDVD->isScreenDVDMenuTitleState() == true)
            {
                sm->onNotifyScreenManager("", AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, EVENTS_SET_FG_LOADING_MODE, QVariant(false)); // added by cychoi 2013.09.13 for ITS 190207 Infinite DVD Loading Screen
                // { modified by cychoi 2013.08.15 for ITS 180543
                scrName = AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU;
                //m_pEngine->sm->changeScreen( AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU );
                // } modified by cychoi 2013.08.15
            }
            // } modified by cychoi 2013.09.24
            break;
        }

        case MEDIA_SOURCE_VCD:
        {
            // { modified by shkim 2013.08.27 for ITS 186402  // { added by cychoi 2013.08.20 for ITS 185304 PBC Off on Back HK
            AppMediaPlayer_Video_ControllerVCD *pVCD    = (AppMediaPlayer_Video_ControllerVCD*)m_pController->getController(MEDIA_SOURCE_VCD);
            AppMediaPlayer_Video_CModel_VCD    *pModel  = (AppMediaPlayer_Video_CModel_VCD*)pVCD->getModel();
            QString topScreenName =  sm->topScreenName();

            // { added by wspark 2013.03.31 for ISV 77641
            if(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK == topScreenName)
            {
                AppMediaPlayer_Video_ScreenVCDPlayback *pVcdPlayback;
                pVcdPlayback = dynamic_cast<AppMediaPlayer_Video_ScreenVCDPlayback*>( sm->topScreen());
                if(pVcdPlayback)
                {
                    pVcdPlayback->initPBCMenu();
                }
            }
            // } added by wspark

            DeckEngineerMode  *pDeckEngine =  getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
            EDeckInit1Setting  tSetting;
            pDeckEngine->GetInitSetting1(tSetting);

            // { modified by wspark 2014.02.19 for ITS 225827
            VIDEO_DISPLAY_T vDisp = (display == DISPLAY_FRONT) ? VIDEO_DISPLAY_FRONT : VIDEO_DISPLAY_REAR;
            if (!m_EngineMain.isTempMode() && (m_EngineMain.getCloneState() == CLONE_OFF) &&
                topScreenName != AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS)
            {
                if(m_EngineMain.isFrontLCDOff() == false)
                {
                    pVCD->setDirectAccessDisplayed(vDisp, false);
                }
            }
            // } modified by wspark

            if(pModel->isPbcMenuDisplayed() == true &&
               topScreenName != AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU &&
               tSetting.m_nPBC == 0)
            {
                // { commented by cychoi 2014.03.13 for ITS 229176 // { modified by cychoi 2014.02.18 for ITS 225861
                //if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL) // modified by cychoi 2013.12.30 for ITS 229176
                //{
                    //turnOffFullScreenAnimation(display);
                    // should use replacePrevScreen for avoid screen flickering
                    //sm->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK);
                    //sm->popScreen();
                    //setFullScreenState(false);
                //}
                // } commented by cychoi 2014.03.13 // } modified by cychoi 2014.02.18
                // { modified by cychoi 2013.09.13 for ITS 190207 Infinite DVD Loading Screen
                sm->onNotifyScreenManager("", AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK, EVENTS_SET_FG_LOADING_MODE, QVariant(false));
                pModel->setLoadingMode(false);
                // } modified by cychoi 2013.09.13
                // { modified by cychoi 2013.08.15 for ITS 180543
                scrName = AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU;
                // sm->pushScreen( AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU );
                // } modified by cychoi 2013.08.15
            }
            // }  modified by shkim 2013.08.27 // } added by cychoi 2013.08.20
            // { added by yungi 2013.09.05 for ITS 187311
            else if(pVCD->isDirectAccessDisplayed(vDisp) && // modified by wspark 2014.02.19 for ITS 225827
                    topScreenName != AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS)
            {
                // { added by cychoi 2014.02.14 for ITS 225321
                if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL)
                {
                    pVCD->setDirectAccessDisplayed(vDisp, false); // modified by wspark 2014.02.19 for ITS 225827
                }
                // } added by cychoi 2014.02.14 for ITS 225321
                else
                {
                    // { added by cychoi 2013.09.13 for ITS 190207 Infinite DVD Loading Screen
                    sm->onNotifyScreenManager("", AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK, EVENTS_SET_FG_LOADING_MODE, QVariant(false));
                    pModel->setLoadingMode(false);
                    // } added by cychoi 2013.09.13
                    scrName = AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS;
                }
            }
            // } added by yungi 2013.09.05 for ITS 187311
            break;
        }
        default:
        {
            MP_LOG << "Unsupported tab. Return." << LOG_ENDL;
        }
    }

    if(source != MEDIA_SOURCE_MAX)
    {
        // { modified by cychoi 2013.07.07 for IVS 85706 drawn Playback then drawn Title/Disc menu
        if(source == MEDIA_SOURCE_DVDVIDEO || source == MEDIA_SOURCE_VCD)
        {
            // { modified by cychoi 2013.08.15 for ITS 180543
            bool bShowPlayback =  getEngineMain().IsShowLoadingFlag();
            QString topScreenName =  sm->topScreenName();

            if(bShowPlayback &&
               (topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL || // modified by cychoi 2013.10.13 for ITS 195278 DVD Title/Disc menu screen should not be Full Screen
                topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK ||
                topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK)) // modified by Sergey 06.08.2013 for ITS#182902
            {
                // removed by shkim 2013.08.27 for ITS 186402
                if(scrName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU)
                {
                    // { modified by cychoi 2014.03.03 for removing unreachable code
                    turnOffFullScreenAnimation(display);
                    sm->changeScreen(scrName);
                    // { modified by cychoi 2013.12.14 NoITS. Blinking Loading.. when press BACK HK in single DISPLAY mode.
                    if(m_EngineMain.GetGstPipelinesOwner(display) != APP_AVP)
                        sm->onNotifyScreenManager("", AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, EVENTS_SET_FG_LOADING_MODE, QVariant(true)); // added by cychoi 2013.09.13 for ITS 190207 Infinite DVD Loading Screen
                    // } modified by cychoi 2013.12.14
                    // } modified by cychoi 2014.03.03
                }
                // { modified by cychoi 2013.09.06 forSmokeTest PBC Menu display
                // { added by shkim 2013.08.27 for ITS 186402
                else if((source == MEDIA_SOURCE_VCD) &&
                        (topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL ||
                         topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK)) // modified by cychoi 2014.02.14 for ITS 225321
                {
                    if(scrName == AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU ||
                       scrName == AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS) // added by yungi 2013.09.05 for ITS 187311
                    {
                        sm->pushScreen(scrName);
                    }
                }
                // } added by shkim 2013.08.27
                // } modified by cychoi 2013.09.06
            }
            // } modified by cychoi 2013.08.15
            // { added by shkim 2013.08.27 for ITS 186402
            else if((source == MEDIA_SOURCE_VCD) &&
                    (topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL ||
                     topScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK)) // modified by cychoi 2014.02.14 for ITS 225321
            {
                // { modified by cychoi 2013.09.06 forSmokeTest PBC Menu display
                if(scrName == AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU ||
                   scrName == AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS) // added by yungi 2013.09.05 for ITS 187311
                {
                    sm->pushScreen(scrName);
                }
                // } modified by cychoi 2013.09.06
            }
            // } added by shkim 2013.08.27
        }
        // } modified by cychoi 2013.07.07
    }
}
// } added by Sergey 07.09.2013 for PRE_FG supoort



// { added by Sergey 13.09.2013 for ITS#188762
void AppMediaPlayer_EngineVideo::resetAll()
{
    // 1 step. Set default values for active source.
    switch(m_pController->getSource())
    {
        case MEDIA_SOURCE_JUKEBOX:
        case MEDIA_SOURCE_USB1:
        case MEDIA_SOURCE_USB2:
        {
            AppMediaPlayer_Video_ControllerFS* pFSCtrl = static_cast<AppMediaPlayer_Video_ControllerFS*>(m_pController->getCurrentController());

            if(pFSCtrl)
            {
                pFSCtrl->enableSubTitle(false);
                pFSCtrl->setFontSize(AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_NORMAL); // added by Sergey 28.09.2013 for ITS#192050
            }

            break;
        }

        default:
            break;
    }


    // 2 step. Set default values for all sources.
    GetVideoController()->updateEnableSubtitleInModels(false); // modified by Sergey 17.10.2013 for ITS#195182
    GetVideoController()->updateCaptionSizeInModels(AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_NORMAL); // modified by Sergey 17.10.2013 for ITS#195182
}



void AppMediaPlayer_EngineVideo::factoryReset()
{
}
// } added by Sergey 13.09.2013 for ITS#188762



// { added by Sergey 15.09.2014 for ITS#189390
void AppMediaPlayer_EngineVideo::onFgLoadingTimerExpired()
{
    emit fgLoading(false);
}
// } added by Sergey 15.09.2014 for ITS#189390

// { added by Sergey 16.09.2013 for ITS#185236
void AppMediaPlayer_EngineVideo::showTxtBtnPopup(POPUP_TXT_BTN_TYPE_T typePopUp, VIDEO_DISPLAY_T disp)
{
    AppMediaPlayer_Video_ScreenManager* sm = getScreenManager(disp);

    if(sm->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
    {
        sm->popScreen();
    }

    QVariant msgPopUp;
    msgPopUp.setValue(typePopUp);
    sm->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON, msgPopUp);
    // { added by cychoi 2014.03.19 for ITS 230153
    //if(typePopUp == POPUP_TXT_BTN_UNAVAILABLE_LANG_CODE)
    //{
    //    m_popupTypeOnSubScreen = POPUP_TXT_BTN_UNAVAILABLE_LANG_CODE;
    //}
    //else
    //{
    //    m_popupTypeOnSubScreen = POPUP_TXT_BTN_MAX;
    //}
    // } added by cychoi 2014.03.19
}
// } added by Sergey 16.09.2013 for ITS#185236

// { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
void AppMediaPlayer_EngineVideo::showDeckHighTempPopup()
{
    AppMediaPlayer_Video_ScreenManager* smFront = getScreenManager(VIDEO_DISPLAY_FRONT);
    AppMediaPlayer_Video_ScreenManager* smRear = getScreenManager(VIDEO_DISPLAY_REAR);

    if(smFront->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
    {
        smFront->popScreen();
    }

    if(smRear->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
    {
        smRear->popScreen();
    }

    QVariant msgPopUp;
    msgPopUp.setValue(POPUP_TXT_BTN_HIGH_TEMPERATURE);
    smFront->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON, msgPopUp);
    smRear->pushScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON, msgPopUp);
}

void AppMediaPlayer_EngineVideo::closeDeckHighTempPopup()
{
    if(popupType(DISPLAY_FRONT) == POPUP_TXT_BTN_HIGH_TEMPERATURE)
    {
        AppMediaPlayer_Video_ScreenManager* smFront = getScreenManager(VIDEO_DISPLAY_FRONT);
        smFront->popScreen();
    }

    if(popupType(DISPLAY_REAR) == POPUP_TXT_BTN_HIGH_TEMPERATURE)
    {
        AppMediaPlayer_Video_ScreenManager* smRear = getScreenManager(VIDEO_DISPLAY_REAR);
        smRear->popScreen();
    }
}
// } added by cychoi 2014.04.09

// { added by cychoi 2014.03.24 for ITS 230874
bool AppMediaPlayer_EngineVideo::isDiscPopup(POPUP_TXT_BTN_TYPE_T typePopUp)
{
    return (typePopUp == POPUP_TXT_BTN_UNAVAILABLE_TRACK ||
            typePopUp == POPUP_TXT_BTN_DRIVING_REGULATION ||
            typePopUp == POPUP_TXT_BTN_HIGH_TEMPERATURE); // modified by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
}
// } added by cychoi 2014.03.24

// mofdified by ravikanth for ISV 89536
void AppMediaPlayer_EngineVideo::sendSeekEventToFM( Event &aEvent )
{
    Event postEvent( EVT_POST_EVENT,
                     m_EngineMain.GetThisAppId(),
                     APP_FMGR,
                     aEvent.GetEventId());
    emit NotifyUISH (postEvent);
}

// { added by Sergey 27.09.2013 for ITS#189958
bool AppMediaPlayer_EngineVideo::isPlaybackScreen(QString screen)
{
    bool result = false;

    if(screen == AppMediaPlayer_Video_Screens::SCREEN_JUKEBOX_PLAYBACK
            ||  screen == AppMediaPlayer_Video_Screens::SCREEN_USB_PLAYBACK
            ||  screen == AppMediaPlayer_Video_Screens::SCREEN_USB2_PLAYBACK
            ||  screen == AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK
            ||  screen == AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK
            ||  screen == AppMediaPlayer_Video_Screens::SCREEN_AUX1_PLAYBACK
            ||  screen == AppMediaPlayer_Video_Screens::SCREEN_AUX2_PLAYBACK
            ||  screen == AppMediaPlayer_Video_Screens::SCREEN_FULL)
    {
        result = true;
    }

    return result;
}
// } added by Sergey 27.09.2013 for ITS#189958


// { added by Sergey 28.09.2013 for ITS#187711
void AppMediaPlayer_EngineVideo::popScreenFromBoth(QString screen)
{
    for(int i = VIDEO_DISPLAY_FRONT; i <= VIDEO_DISPLAY_REAR; i++)
    {
        VIDEO_DISPLAY_T disp = (VIDEO_DISPLAY_T)i;
        AppMediaPlayer_Video_ScreenManager* sm = getScreenManager(disp);

        if(screen == sm->topScreenName())
        {
            sm->popScreen();
        }
    }
}
// } added by Sergey 28.09.2013 for ITS#187711

// { modified by yungi 2014.01.23 for ITS 221843 // modified by ravikanth 02-10-13 for ITS 0190988
void AppMediaPlayer_EngineVideo::setSystemPopupShow( bool visible )
{
    m_systemPopVisible = visible;
    emit systemPopupShow(visible, m_systemPopupDisplay);
    m_systemPopupDisplay = VIDEO_DISPLAY_MAX;
}
// } modified by yungi 2014.01.23

// { modified by yungi 2014.02.11 No CR Fixed Focus now show // { modified by yungi 2014.01.23 for ITS 221843 //{ added by cychoi 2013.12.05 for ITS 213053
void AppMediaPlayer_EngineVideo::setLocalPopupShow( bool visible , int m_disp)
{
    m_localPopVisible = visible;
    emit localPopupShow(visible, m_disp);
    // { added by cychoi 2014.03.19 for ITS 230153
    //if(visible == false)
    //{
    //    m_popupTypeOnSubScreen = POPUP_TXT_BTN_MAX;
    //}
    // } added by cychoi 2014.03.19
}
// } modified by yungi 2014.02.11 // } modified by yungi 2014.01.23

// { added by sjhyun 2013.10.08 for ITS 193794
bool AppMediaPlayer_EngineVideo::isPbcMode(MEDIA_SOURCE_T source, QString topScreenName) // modified by sjhyun for ITS 195523
{
    Q_UNUSED(topScreenName);
    if (source == MEDIA_SOURCE_VCD && m_pController != NULL)
    {
        AppMediaPlayer_Video_ControllerVCD *pVCD    = (AppMediaPlayer_Video_ControllerVCD*)m_pController->getController(MEDIA_SOURCE_VCD);
        AppMediaPlayer_Video_CModel_VCD    *pModel  = (AppMediaPlayer_Video_CModel_VCD*)pVCD->getModel();
        if (pModel != NULL)
            return pModel->isPbcMode();
        // removed by sjhyun 2013.11.07 for ITS 207179
    }
    return false;
}
// } added by sjhyun


// { added by Sergey 10.10.2013 for ITS#194643, 195505
void AppMediaPlayer_EngineVideo::lightPlaybackControls(int item, bool isOn, QVariant disp)
{
    emit lightControls(item, isOn, QVariant(disp));
}
// } added by Sergey 10.10.2013 for ITS#194643, 195505


// { added by Sergey13.10.13
void AppMediaPlayer_EngineVideo::stopFFRewTimer()
{
    MP_HIGH << LOG_ENDL;
    emit stopFFRewTimerSignal();
}
// } added by Sergey13.10.13


// removed by sjhyun 2013.11.07 for ITS 207179

// { added by Sergey 23.10.2013 for ITS#196877
void AppMediaPlayer_EngineVideo::notifyBothScreens(QString srcScreen, QString resScreen, EVENTS_T evt, QVariant arg)
{
    getScrMgr(DISPLAY_FRONT)->onNotifyScreenManager(srcScreen, resScreen, evt, arg);
    getScrMgr(DISPLAY_REAR)->onNotifyScreenManager(srcScreen, resScreen, evt, arg);
}
// } added by Sergey 23.10.2013 for ITS#196877

// { added by sjhyun 2013.10.25 for ITS 198704
bool AppMediaPlayer_EngineVideo::isTopScreenDVDTitleDiscMenu(int disp)
{
    QString str = getScreenManager(disp)->topScreenName();
    MP_LOG<< "disp("<<disp<<") tscr("<<str<<")" << LOG_ENDL;
    if ( str == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU)
        return true;
    return false;
}
// } added by sjhyun

// { added by sjhyun 2013.10.29 for ITS 198332
bool AppMediaPlayer_EngineVideo::isDVDScreenStatusKeep(MEDIA_SOURCE_T source, QString screen)
{
    if (source == MEDIA_SOURCE_DVDVIDEO)
    {
        // Exception case for DVD Setting Popup subscreen.
        // In this case, the subscreen should be kept whether it wake up by DISP H/K.
        if (screen == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON)
            return true;// modified by cychoi 2014.03.03 for ITS 225375
    }
    return false; // modified by cychoi 2014.03.03 for ITS 225375
}
// } added by sjhyun

// { modified by wspark 2014.02.19 for ITS 225827
// { added by cychoi 2014.02.14 for ITS 225321
void AppMediaPlayer_EngineVideo::initDirectAccessDisplayed (int disp, bool bShow)
{
    AppMediaPlayer_Video_ControllerVCD *pVCD = (AppMediaPlayer_Video_ControllerVCD*)m_pController->getController(MEDIA_SOURCE_VCD);

    if(pVCD)
    {
        pVCD->setDirectAccessDisplayed(disp, bShow);
    }

    if ( getScreenManager(disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON
       && getScreenManager(disp)->getPreviousScreenName(1) == AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS)
    {
        getScreenManager(disp)->popScreen();
    }
}
// } added by cychoi 2014.02.14

bool AppMediaPlayer_EngineVideo::isDirectAccessDisplayed (int disp)
{
    AppMediaPlayer_Video_ControllerVCD *pVCD = (AppMediaPlayer_Video_ControllerVCD*)m_pController->getController(MEDIA_SOURCE_VCD);

    if(pVCD)
    {
        return pVCD->isDirectAccessDisplayed(disp);
    }

    return false;
}
// } modified by wspark

void AppMediaPlayer_EngineVideo::next()
{
    GetVideoController()->next();
}

void AppMediaPlayer_EngineVideo::fastForward()
{
    GetVideoController()->fastForward();
}

void AppMediaPlayer_EngineVideo::fastForwardCritical()
{
    GetVideoController()->fastForwardCritical();
}

void AppMediaPlayer_EngineVideo::prev()
{
    GetVideoController()->prev();
}

void AppMediaPlayer_EngineVideo::rewind()
{
    GetVideoController()->rewind();
}

void AppMediaPlayer_EngineVideo::rewindCritical()
{
    GetVideoController()->rewindCritical();
}

void AppMediaPlayer_EngineVideo::cancel_ff_rew()
{
    GetVideoController()->cancel_ff_rew();
}

// { added by sjhyun 2013.11.05 for ITS 206357
bool AppMediaPlayer_EngineVideo::inhibitionIcon(bool val)
{
    emit showInhibitionIcon(val);
    return true; // added by cychoi 2013.11.11 for Prevent fix
}
// } added by sjhyun


// { added by Sergey 13.11.2013 for ITS#208891
bool AppMediaPlayer_EngineVideo::isDRSEntryPoint(APP_ENTRY_POINT_T ep)
{
    bool result = false;

    if(ep == eAPP_AVP_VIDEO_MODE_DISC_DRS
       || ep == eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS
       || ep == eAPP_AVP_VIDEO_MODE_USB1_DRS
       || ep == eAPP_AVP_VIDEO_MODE_USB2_DRS
       || ep == eAPP_AVP_VIDEO_MODE_AUX_DRS)
    {
        result = true;
    }

    return result;
}
// } added by Sergey 13.11.2013 for ITS#208891

// modified for ITS 0217509
void AppMediaPlayer_EngineVideo::onCloseCopyCancelConfirmPopup()
{
    if((popupType(DISPLAY_FRONT) == POPUP_TXT_BTN_COPY_INPROGRESS)
            || (popupType(DISPLAY_REAR) == POPUP_TXT_BTN_COPY_INPROGRESS))
        popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON);
}
//added by edo.lee 2014.1.8 ITS 0217055 
void AppMediaPlayer_EngineVideo::CancelAllJogKey()
{
    emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_RIGHT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED); //Added by taihyun.ahn for ITS 0189456, 0192211
    emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_LEFT, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED);
    emit seekHandlejogevent((int)AppEngineQMLConstants::JOG_CENTER, (int)AppEngineQMLConstants::KEY_STATUS_CANCELED);
}
//added by edo.lee 2014.1.8

// added by edo.lee 2014.1.8 ITS 218077
void AppMediaPlayer_EngineVideo::closeCaptionScreen()
{
	VIDEO_DISPLAY_T disp =  m_pScreenManager[VIDEO_DISPLAY_FRONT]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS ?
		                    VIDEO_DISPLAY_FRONT : m_pScreenManager[VIDEO_DISPLAY_REAR]->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS?
		                    VIDEO_DISPLAY_REAR :VIDEO_DISPLAY_MAX;
	
	if (disp != VIDEO_DISPLAY_MAX)
	{            	
	    
        MP_LOG<<"disp : "<<disp << LOG_ENDL;
		getEngineMain().setSubCreenOn(false ,getEngineMain().getSubCreenName()); 
		HideSubScreenOnVideo(disp);	
		return;
	}
}
// added by edo.lee  2014.1.8 ITS 218077
