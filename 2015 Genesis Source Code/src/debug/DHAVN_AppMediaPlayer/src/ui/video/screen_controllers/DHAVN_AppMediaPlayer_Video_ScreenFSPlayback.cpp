#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenFSPlayback.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Playback.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "QVariant"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"

AppMediaPlayer_Video_ScreenFSPlayback::
AppMediaPlayer_Video_ScreenFSPlayback( AppMediaPlayer_Video_Controller *pController,
                                       AppMediaPlayer_EngineVideo *pEngineVideo,
                                       MEDIA_SOURCE_T source,
                                       int disp,
                                       QObject *parent )
    : AppMediaPlayer_Video_Screen( parent ),
      m_pController( pController ),
      m_pEngineVideo( pEngineVideo ),
      m_mediaSource( source ),
      m_isLockout(false),
      m_disp(disp),
      m_pendingPopup(POPUP_TXT_BTN_MAX) // added by Sergey 22.09.2013 for ITS#186731
{
    m_myName = identifyMyName(m_mediaSource); // added by Sergey 31.08.2013 for ITS#186799

    m_pControllerFS = static_cast<AppMediaPlayer_Video_ControllerFS*>(m_pController->getController(source));

    m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pControllerFS->getModel());

    m_pModel = new AppMediaPlayer_Video_SModel_Playback(this);
    setModel( m_pModel );

    m_pModel->setProgressBarMode("FS");

    m_overlays << AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA;

    m_pModel->bindWith( m_pControllerModel );

    m_fullScreenTimer.setInterval( FULL_SCREEN_INTERVAL );

	m_isFFREW = false;//added by edo.lee 2013.06.25

    //connect(m_pEngineVideo, SIGNAL(lockoutMode(bool)), this, SLOT(onLockoutMode(bool))); // modified 2013.02.05   
    connect(m_pEngineVideo, SIGNAL(setTuneMode(bool)), this, SLOT(onSetTuneMode(bool)));
    connect(m_pEngineVideo, SIGNAL(tuneSameFile(bool)), this, SLOT(onTuneSameFile(bool)));  // added by yongkyun.lee  2012.10.17  for Current file is white color
	// removed by Sergey 01.05.2013 for Tune Functionality    
	connect(m_pControllerFS,
            SIGNAL(fsPlaybackError(AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_T)),
            this,
            SLOT(onFSPlaybackError(AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_T)));
    connect( m_pControllerFS, SIGNAL(durationChanged(int)),
            this, SLOT(onDurationChanged(int))); // added by Sergey 22.09.2013 for ITS#186731
    connect(&m_fullScreenTimer, SIGNAL(timeout()), this, SLOT(showFullScreen()));
    connect(m_pControllerModel,
            SIGNAL(playbackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)),
            this,
            SLOT(onPlaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)));
    connect(m_pControllerModel,
            SIGNAL(filenameChanged(QString)),
            this,
            SLOT(onFilenameChanged(QString)));
    connect(m_pControllerModel,
            SIGNAL(foldernameChanged(QString)),
            this,
            SLOT(onFoldernameChanged(QString)));
    connect(m_pControllerModel, SIGNAL(readingFileChanged(bool)), this, SLOT(onReadingFile(bool))); // added by Sergey for CR#11607

    connect( m_pEngineVideo->getScreenManager(m_disp), SIGNAL(isFocusVisibleChanged(bool)),
            this, SLOT(onFocusVisibilityChanged(bool)));

    // removed by Sergey 24.09.2013 for ITS#188938

    connect(m_pControllerFS, SIGNAL(cancelFfRew()),   this, SIGNAL(cancelFfRew()));  // added by yongkyun 2013.01.19 for ITS 150703
    // removed by Sergey 23.09.2013 for ITS#188498
    // { modified by wspark 2012.11.20 for CR15380
    //connect(m_pControllerFS, SIGNAL(showPopup()), this, SLOT(onShowPopup())); // added by wspark 2012.08.21 for DQA #47
    connect(m_pControllerFS, SIGNAL(showPopup(POPUP_TXT_BTN_TYPE_T)), this, SLOT(onShowPopup(POPUP_TXT_BTN_TYPE_T)));
    // } modified by wspark
    // removed by sjhyun 2013.11.05 for ITS 206357
    connect(m_pEngineVideo, SIGNAL(setCamMode(bool)), this, SLOT(onSetCamMode(bool))); // added by wspark 2013.02.21 for ISV 73305

    connect(m_pEngineVideo, SIGNAL(copyCompleted()), this, SLOT(onFMCopyCompleted())); // modified by ravikanth 12-07-13 for ITS 0174301

    connect(m_pEngineVideo, SIGNAL(fgLoading(bool)), this, SLOT(onFgLoading(bool))); // added by Sergey 15.09.2014 for ITS#189390

    connect(&m_pEngineVideo->getEngineMain(), SIGNAL(retranslateUi(QVariant)), this, SLOT(onRetranslateUI())); // added by Sergey 26.11.2013 for Root folder translation
}


AppMediaPlayer_Video_ScreenFSPlayback::~AppMediaPlayer_Video_ScreenFSPlayback()
{
}

void AppMediaPlayer_Video_ScreenFSPlayback::onHide()
{
    MP_LOG << LOG_ENDL;

    emit releaseLongPress(); // added by junggil 2012.09.06 for keep the FF or REW mode when change to another screen. 

    m_fullScreenTimer.stop();
}

void AppMediaPlayer_Video_ScreenFSPlayback::onDeactivate()
{   
    emit setItemProperty(QString("playbackGestureArea"), "enabled", QVariant(false));
    emit setItemProperty(QString("playbackControlls"), "isCommonJogEnabled", QVariant(false)); // added by Dmitry Bykov 08.04.13 for touch rule
    //emit setItemProperty(QString("playbackGestureArea"), "y",0);

    // commented by cychoi 2013.10.13 for ITS 195278 Should not handle DRS on Deactivate
}

/** @copydoc AppMediaPlayer_Video_Screen::onShow() */
void AppMediaPlayer_Video_ScreenFSPlayback::onShow( QVariant arg )
{
    MP_LOG << arg << LOG_ENDL;  

    // removed by Sergey 26.07.2013 for ITS#181096
    
    emit setItemProperty(QString("playbackGestureArea"), "enabled", QVariant(true));
    emit setItemProperty(QString("playbackControlls"), "isCommonJogEnabled", QVariant(true)); // added by Dmitry Bykov 08.04.13 for touch rule
    onLockoutMode(m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode());

    if ( arg.toString() == "Options" /*&& !m_isLockout*/ && !m_pModel->loadingMode()) //modified by edo.lee 2013.05.02
    {
       bool isUSB = ( m_mediaSource == MEDIA_SOURCE_USB1 || m_mediaSource == MEDIA_SOURCE_USB2 );
       pushScreen(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS, isUSB?QVariant("USB"):QVariant("JUKEBOX"));
    }

    // removed by Sergey 11.08.2013

    setupModeArea(); // added by Sergey 16.05.2013
    //setFullScreen();
    m_fullScreenTimer.start(); //modified by edo.lee 2013.08.30
    m_pEngineVideo->getEngineMain().GetNotifier()->ClearOSD(m_disp == 0); //added on 2013.07.03 for ITS# 0177010
    //m_pEngineVideo->getEngineMain().setPlaybackScreen(m_disp == 0); //remove by edo.lee 2013.08.23

    m_pEngineVideo->SetPlayBackControlbyTune(); // ys ITS 0188976

    MP_LOG << "m_pendingPopup = " << m_pendingPopup << LOG_ENDL;

    m_pendingPopup = m_pEngineVideo->getPopupType();//added by taihyun.ahn for ITS 221592 2014.01.23

    if(m_pendingPopup != POPUP_TXT_BTN_MAX)
    {
        m_pEngineVideo->showTxtBtnPopup(m_pendingPopup, (VIDEO_DISPLAY_T)m_disp); // added by Sergey 22.09.2013 for ITS#186731
    }
}

void AppMediaPlayer_Video_ScreenFSPlayback::onMousePressed()
{
    //Reset full screen timer by any touch.
    m_fullScreenTimer.stop();
}

void AppMediaPlayer_Video_ScreenFSPlayback::onMouseReleased()
{
    if(!m_pController->isSeekMode()) // added by Sergey 13.09.2013 for ITS#188904
        m_fullScreenTimer.start(); // modified by ravikanth 06-07-13 for SANITY fill screen transition on drag finish
}

void AppMediaPlayer_Video_ScreenFSPlayback::onMouseClicked(bool pbArea)
{
    MP_LOG << "source: " << m_mediaSource << LOG_ENDL;
 
    //{ added by yongkyun.lee 20130612 for : : ITS 172821
    if( m_pController->isSeekMode())
       return;
    //} added by yongkyun.lee 20130612 
 
    if ( m_pModel->focusVisible() )
    {
 	   // { modified by Sergey 17.04.2013 for ITS#160758
        if(m_pModel->playbackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING
                || m_pModel->playbackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED
                || m_pModel->playbackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_STOPPED)
        {
            emit hideAllFocus();
        }
 	   // } modified by Sergey 17.04.2013 for ITS#160758
    }
    
    if(pbArea)
   	{
   		showFullScreen();
   		// removed by Sergey 10.08.2013 for 182892
   	}
}

void AppMediaPlayer_Video_ScreenFSPlayback::onModeAreaTapped()
{
}

void AppMediaPlayer_Video_ScreenFSPlayback::onSoftkeyBack()
{
    emit popScreen();
}

void AppMediaPlayer_Video_ScreenFSPlayback::onSoftkeyBtnHold( QString button )
{
    Q_UNUSED(button);
}

void AppMediaPlayer_Video_ScreenFSPlayback::onSoftkeyBtnHoldCritical( QString button )
{
    Q_UNUSED(button);
}


void AppMediaPlayer_Video_ScreenFSPlayback::onSoftkeyBtnReleased( QString button )
{
   MP_LOG << "button:" << button << LOG_ENDL;

   if ( button == "Repeat" )
   {
      m_pControllerFS->toggleRepeat();
   }
   else if ( button == "Random" )
   {
      m_pControllerFS->toggleRandom();
   }
   
   if(m_isFFREW == false)//added by edo.lee 2013.06.25
   {
   		MP_LOG << "m_fullScreenTimer.start" << LOG_ENDL;
   		m_fullScreenTimer.start(); //added by changjin 2012.10.05 for CR 14237
   }

	if(m_mediaSource == MEDIA_SOURCE_JUKEBOX)
	   m_pEngineVideo->updatePlayingMode(JUKEBOX_VIDEO,m_pControllerFS->getModel()->randomMode(),m_pControllerFS->getModel()->repeatMode(),0);
	else
	   m_pEngineVideo->updatePlayingMode(USB_VIDEO,m_pControllerFS->getModel()->randomMode(),m_pControllerFS->getModel()->repeatMode(),0);
	   
}

void AppMediaPlayer_Video_ScreenFSPlayback::onTogglePlayBtn()
{
    MP_LOG << LOG_ENDL;
    
    //{ added by yongkyun.lee 20130314 for : Video Play/Pause 
    if(m_pEngineVideo->IsPlaying())
        m_pEngineVideo->setPassPlay(true);
    
    // { modified by kihyung 2013.07.30 for ITS 0174622
        //if(m_pControllerFS->getModel()->playbackStatus() != AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING) {
       //added by edo.lee 2013.04.20
            //m_pEngineVideo->getEngineMain().setUnMuteState(false);
        //}
    // } modified by kihyung 2013.07.30 for ITS 0174622    
    
    //} added by yongkyun.lee 20130314 
    m_pControllerFS->togglePlay();
}

void AppMediaPlayer_Video_ScreenFSPlayback::onLongPressPlayBtn()
{
//    showFullScreen(); // commented by Sergey 24.07.2013 ITS#181467
}

void AppMediaPlayer_Video_ScreenFSPlayback::onPrevBtn()
{
    m_pControllerFS->prev();
    m_pEngineVideo->DisplayOSD(true); //added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD  
    // { added by dongjin 2013.03.21 for ISV 76442
    if (!m_pModel->focusVisible())
    {
        setFullScreen();
    }
    // } added by dongjin
}

void AppMediaPlayer_Video_ScreenFSPlayback::onNextBtn()
{
    m_pControllerFS->next();
    m_pEngineVideo->DisplayOSD(true); //added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD  
    // { added by dongjin 2013.03.21 for ISV 76442
    if (!m_pModel->focusVisible())
    {
        setFullScreen();
    }
    // } added by dongjin
}

void AppMediaPlayer_Video_ScreenFSPlayback::onNextBtnHold()
{
    m_pControllerFS->fastForward();
	m_isFFREW = true; //added by edo.lee 2013.06.25
    // emit setSpeedIcon(FAST_SPEED_X4); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenFSPlayback::onNextBtnHoldCritical()
{
    m_pControllerFS->fastForwardCritical();
	m_isFFREW = true; //added by edo.lee 2013.06.25
    // emit setSpeedIcon(FAST_SPEED_X20); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenFSPlayback::onPrevBtnHold()
{
    m_pControllerFS->rewind();
    // emit setSpeedIcon(FAST_SPEED_X4); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenFSPlayback::onPrevBtnHoldCritical()
{
    m_pControllerFS->rewindCritical();
    // emit setSpeedIcon(FAST_SPEED_X20); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenFSPlayback::onFlick_next()
{
    m_pControllerFS->flickNext();
}

void AppMediaPlayer_Video_ScreenFSPlayback::onFlick_prev()
{
    m_pControllerFS->flickPrev();
}
//added by edo.lee 2013.06.11
void AppMediaPlayer_Video_ScreenFSPlayback::onSelectTuned()
{		
	m_pControllerFS->selectTuned();	
	onSetTuneMode(false);	
	m_pControllerFS->cancelTune();
}
//added by edo.lee 2013.06.11
void AppMediaPlayer_Video_ScreenFSPlayback::onPlaybackStatusChanged( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T status)
{
    // { add by kihyung 2012.07.05 for CR 9855
    // Q_UNUSED(status);
    switch(status) 
    {
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING:
    // case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_STOPPED: // removed by Sergey 03.05.2013 for ISV_KR#76907
        if(!m_pControllerFS->longBtnTimer()->isActive()) // added by Sergey 26.10.2013 for ITS#197713
        {
            emit cancelFfRew();//added by taihyun.ahn 2013.12.06
            emit resetSpeedIcon();
        }
        m_pController->PrevNextOSD(false); // added by yongkyun.lee 20130320 for : NO CR ff/rew OSD
        break;
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_4X:
        m_pEngineVideo->popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS); // added by Sergey 22.11.2013 for ITS#210500
        emit setSpeedIcon(FAST_SPEED_X4);
        m_pController->PrevNextOSD(true); // added by yongkyun.lee 20130320 for : NO CR ff/rew OSD
        break;
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_16X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_16X:
        emit setSpeedIcon(FAST_SPEED_X16);
        m_pController->PrevNextOSD(true); // added by yongkyun.lee 20130320 for : NO CR ff/rew OSD     
        break;
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_20X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_20X:
        m_pEngineVideo->popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS); // added by Sergey 22.11.2013 for ITS#210500
        emit setSpeedIcon(FAST_SPEED_X20);
        m_pController->PrevNextOSD(true); // added by yongkyun.lee 20130320 for : NO CR ff/rew OSD
        break;
    default:
        break;
    }
    // } add by kihyung
    setFullScreen();
}


void AppMediaPlayer_Video_ScreenFSPlayback::onRandomModeChanged( AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T mode)
{
   // The numerical values ?�​in the enumeration and in the model and in the qml are the same.
   MP_LOG<<" mode = " << mode << LOG_ENDL;
   m_pModel->setRandomMode(mode);
}


void AppMediaPlayer_Video_ScreenFSPlayback::onRepeatModeChanged( AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T mode )
{
   // The numerical values ?�​in the enumeration and in the model and in the qml are the same.
   MP_LOG<<" mode = " << mode << LOG_ENDL;
   m_pModel->setRepeatMode(mode);
}

void AppMediaPlayer_Video_ScreenFSPlayback::onProgressBarPressed()
{
    MP_LOG << LOG_ENDL;
    //m_pEngineVideo->getEngineMain().setMuteForSeek(false);//added by edo.lee 2013.09.02 for drag mute
    m_pEngineVideo->getEngineMain().setIsTermOnTrackChange(true);//added by edo.lee 2013.10.07 ITS 186267
    m_pControllerModel->setDragging(true);
}

void AppMediaPlayer_Video_ScreenFSPlayback::onProgressBarReleased()
{
    MP_LOG << LOG_ENDL;
    //m_pEngineVideo->getEngineMain().setUnMuteState(false);//remove by edo.lee 2013.04.20 for Mute
    m_pControllerModel->setDragging(false);
    //{ added by yongkyun 2013.01.18 for ISV 70099
    MP_LOG << "m_fullScreenTimer.start" << LOG_ENDL;
    m_fullScreenTimer.start();   
    //} added by yongkyun 2013.01.18
    
    // removed by kihyung 2013.09.10 for smoke test
    // if(m_pEngineVideo->IsPlaying() == false) // modified by lssanh 2013.06.26 bt call
    //    m_pControllerFS->togglePlay();
}

void AppMediaPlayer_Video_ScreenFSPlayback::onProgressBarSetPosition( int position )
{
    if ( position >= 0 )
    {
        m_pControllerFS->setPosition(position);

        // added by kihyung 2013.09.10 for smoke test 
        if(m_pEngineVideo->IsPlaying() == false) 
            m_pControllerFS->play();
    }
}

void AppMediaPlayer_Video_ScreenFSPlayback::onCancel_ff_rew()
{
    MP_LOG << LOG_ENDL;
	m_isFFREW = false;//added by edo.lee 2013.06.25
	//m_pEngineVideo->getEngineMain().setUnMuteState(false); //remve by edo.lee 2013.09.06
	m_pControllerFS->cancel_ff_rew();
	//if(m_pEngineVideo->getEngineMain().getisBTCall()) // deleted by lssanh 2013.06.26 bt call //added by edo.lee 2013.06.19
	//	 m_pControllerFS->pause(); // deleted by lssanh 2013.06.26 bt call
	
    // emit resetSpeedIcon(); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenFSPlayback::onMenuKeyPressed()
{
   MP_LOG << LOG_ENDL;
   if(/*m_isLockout ||*/ m_pModel->loadingMode() && m_pEngineVideo->getEngineMain().getIsTermOnTrackChange()) return; //modified by edo.lee 2013.1.09 ITS 0207835 
   bool isUSB = ( m_mediaSource == MEDIA_SOURCE_USB1 || m_mediaSource == MEDIA_SOURCE_USB2 );
   pushScreen(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS, isUSB?QVariant("USB"):QVariant("JUKEBOX"));
}



void AppMediaPlayer_Video_ScreenFSPlayback::onOptionsItemClicked(int item)
{
   MP_LOG << "item =" << item << LOG_ENDL;
   if(/*m_isLockout ||*/ m_pModel->loadingMode())return; //modified by edo.lee 2013.02.28
   m_pEngineVideo->releaseTuneMode(); // modified by ravikanth 06-07-13 for ITS 0175700
   switch(item)
   {
   case OPTIONS_LIST:
   {
      MP_LOG << "should launch FileManager" << LOG_ENDL;
      listButtonHandler();
      break;
   }
   case OPTIONS_CAPTIONS:
   {
      QVariant vSource;
      vSource.setValue(m_mediaSource);
//added by aettie 20130612 for 172347
    //if(m_pEngineVideo->getIsShowLockoutView() != VIDEO_DISPLAY_MAX)
	{
		m_pEngineVideo->onShowSettings("FS_Captions", vSource);
		m_pEngineVideo->LaunchSubScreenOnVideo(VIDEO_FS_CAPTIONS, m_disp);
	}
	//else
    //      pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS, vSource );
      m_pEngineVideo->notifyBothScreens(m_myName, AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS, EVENTS_INIT_SCREEN, QVariant(true)); // added by Sergey 23.10.2013 for ITS#196877

      break;
   }
   case OPTIONS_SOUND_SET:
   {
      m_pEngineVideo->launchSettings(AppSettingsDef::eStart_Mode_Sound, MP::JUKEBOX, false,m_disp); // modified by edo.lee 2013.05.02
      break;
   }

   case OPTIONS_DISPLAY_SET:
   {
       MP_LOG << "should launch VideoSettings" << LOG_ENDL;

       m_pEngineVideo->launchSettings(AppSettingsDef::eStart_Mode_Screen, MP::JUKEBOX, false,m_disp); // modified by edo.lee 2013.05.02
       break;
   }

   case OPTIONS_REPEAT_ONE:
   {
       m_pControllerFS->repeatPlayFile();
       break;
   }
	
   // { modified by ravikanth - 12-09-19
   case OPTIONS_REPEAT_ALL:
   {
       m_pControllerFS->repeatPlayAll();
       break;
   }
   // } modified by ravikanth - 12-09-19
   
   case OPTIONS_REPEAT_OFF:
   {
       m_pControllerFS->repeatPlayOff();
       break;
   }

   // { added by ravikanth - 12-09-19
   case OPTIONS_REPEAT_FOLDER:
   {
       m_pControllerFS->repeatPlayFolder();
       break;
   }
   // { added by ravikanth - 12-09-19

   // { modified by Sergey 16.10.2013 for ITS#195519
   case OPTIONS_RANDOM_ON:
   {
       if(m_pControllerModel->randomMode() != AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_FILE
               && m_pControllerModel->randomMode() != AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_FOLDER)
       {
           m_pControllerFS->randomPlayFolder();
       }

       break;
   }
   // } modified by Sergey 16.10.2013 for ITS#195519

   case OPTIONS_RANDOM_OFF:
   {
       m_pControllerFS->randomPlayOff();
       break;
   }
   case OPTIONS_FILE_INFO:
   {
      QVariant vSource;
      vSource.setValue(m_mediaSource);
      pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FS_FILEINFO_POPUP, vSource);
      break;
   }
   case OPTIONS_COPY:
   {
   // modified by ravikanth 16-04-13
       if(m_pEngineVideo->isCopyInProgress())
       {
           showTxtBtnPopUp(POPUP_TXT_BTN_COPY_INPROGRESS);
       }
       else
       {
           if ( m_mediaSource == MEDIA_SOURCE_USB1 || m_mediaSource == MEDIA_SOURCE_USB2 )
           {
               // { modified by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list
               m_pEngineVideo->setCopy(true);
               listButtonHandler();
               //QVariant vSource;
               //vSource.setValue(m_mediaSource);
               //pushScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_PROGRESS, vSource);
               // } modified by eunhye 2013.02.26
           }
       }
	   // modified by ravikanth 16-04-13
      break;
   }
   case OPTIONS_FULL_SCREEN:
   {
      showFullScreen();
      break;
   }
   case OPTIONS_DIVX_SETTINGS:
   {
		//added by edo.lee 2013.07.30 subscreen
		//if(m_pEngineVideo->getIsShowLockoutView() != VIDEO_DISPLAY_MAX)
		{
		     m_pEngineVideo->onShowSettings("DIVX_Settings", QVariant("FS"));
		     m_pEngineVideo->LaunchSubScreenOnVideo(VIDEO_FS_DIVX, m_disp);
		}
		//else			
	   	//	emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DIVXREG); // modified by Sergey 19.07.2013
	   m_pEngineVideo->notifyBothScreens(m_myName, AppMediaPlayer_Video_Screens::SCREEN_DIVXREG, EVENTS_INIT_SCREEN, QVariant(true)); // added by Sergey 25.10.2013 for new DivX
			
      break;
   }
   default: //make compiler happy
      break;

   }
}

void AppMediaPlayer_Video_ScreenFSPlayback::listButtonHandler()
{
   MP_LOG << "file name: " << m_pControllerModel->filename() << LOG_ENDL;
   MP_LOG << "folder name: " << m_pControllerModel->fullfoldername() << LOG_ENDL;
   m_pEngineVideo->launchFileManager( m_mediaSource, m_pControllerModel->filename(), m_pControllerModel->fullfoldername() , m_disp); //modified by edo.lee 2013.05.02
   m_pEngineVideo->setVideoListView((VIDEO_DISPLAY_T)m_disp, true); //modified by edo.lee 2013.05.29
}

void AppMediaPlayer_Video_ScreenFSPlayback::onLockoutMode(bool on)
{
    MP_HIGH << "on =" << on << LOG_ENDL;

    //m_pModel->setLockoutMode(on);
    m_isLockout = on;
    // emit showLockout(on); // commented by Sergey 27.10.2013 for ITS#198669
    invokeQmlMethod("onShowLockout", QVariant(on)); // added by Sergey 27.10.2013 for ITS#198669
}

// { modified by Sergey for CR#16015
void AppMediaPlayer_Video_ScreenFSPlayback::onFSPlaybackError(AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_T error)
{
    MP_LOG << "error =" << error<< "disp" << m_disp  << LOG_ENDL;

	// removed by Sergey 24.09.2013 for ITS#188821
    
    POPUP_TXT_BTN_TYPE_T popupType = POPUP_TXT_BTN_MAX;

    switch(error)
    {
    	// { modified by Sergey 10.11.2013 new DivX popups
        case AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_UNSUPPORTED:
        case AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_AUDIO_FORMAT_UNSUPPORTED:
        case AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_RESOLUTION_UNSUPPORTED:
        {
            if(m_pControllerModel->lastPlayedUrl() != "")
            {
                // { modified by cychoi 2013.11.16 for Prevent Fix
                if(error == AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_AUDIO_FORMAT_UNSUPPORTED)
                {
                    popupType = POPUP_TXT_BTN_AUDIO_FORMAT_UNSUPPORTED;
                }
                else if(error == AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_RESOLUTION_UNSUPPORTED)
                {
                    popupType = POPUP_TXT_BTN_RESOLUTION_UNSUPPORTED;
                }
                else //if(error == AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_UNSUPPORTED)
                {
                    popupType = POPUP_TXT_BTN_UNSUPPORTED_FILE;
                }
                // } modified by cychoi 2013.11.16
            }
            // } modified by Sergey 10.11.2013 new DivX popups
            else
            {
                // { modified by Sergey 24.09.2013 for ITS#188821
                setFullScreen(); // added by Sergey 30.04.2013 to show info of unsupported file

                MP_LOG << "current video screen =" << m_pEngineVideo->getEngineMain().getVideoDisplay() << LOG_ENDL;

                if(m_pEngineVideo->getEngineMain().getVideoDisplay() == m_disp)
                {
                    m_pControllerFS->startSkipTimer(); // modified by Sergey 08.10.2013 for ISV#92307
//                    QTimer::singleShot(2000, m_pControllerFS, SLOT(skip())); // added by Sergey 25.05.2013
                }
                // } modified by Sergey 24.09.2013 for ITS#188821
            }

            break;
        }
        case AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_UNSUPPORTED_ALL:
        {
    		popupType = POPUP_TXT_BTN_UNSUPPORTED_ALL; // modified by Sergey 25.05.2013

            break;
        } 
        case AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_DRM_STATUS:
        {
            popupType = POPUP_TXT_BTN_DRM_CONFIRM;
            break;
        }
        case AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_DRM_EXPIRED:
        {
            popupType = POPUP_TXT_BTN_DRM_EXPIRED; // modified by Sergey 07.08.2013

            break;
        }
        case AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_DRM_NOT_AUTH:
        {
            popupType = POPUP_TXT_BTN_DRM_NOT_AUTH; // modified by Sergey for DRM 30.07.2013

            break;
        }
        default: //make compiler happy
            break;
    }

    // { modified by Sergey 22.09.2013 for ITS#186731
	m_pEngineVideo->getEngineMain().GetNotifier()->onSetDisplayOSD(true);// added by edo.lee 2013.06.26 ITS 0176330 

    if(popupType != POPUP_TXT_BTN_MAX)
    {
        QVariant param;
        param.setValue(popupType);

        QString topScreen = m_pEngineVideo->getScreenManager(m_disp)->topScreenName();

        if(m_myName == topScreen || topScreen == AppMediaPlayer_Video_Screens::SCREEN_FULL)
        {
				// { modified by Sergey 24.09.2013 for ITS#188821, 185236
                m_pEngineVideo->showTxtBtnPopup(popupType, (VIDEO_DISPLAY_T)m_disp);
				// } modified by Sergey 24.09.2013 for ITS#188821, 185236
        }
        else
        {
            m_pendingPopup = popupType;
            m_pEngineVideo->setPopupType(m_pendingPopup);//added by taihyun.ahn for ITS 221592 2014.01.23

            MP_LOG << "We have a pending popup =" << m_pendingPopup << LOG_ENDL;
        }
    }
    // } modified by Sergey 22.09.2013 for ITS#186731
}
// } modified by Sergey for CR#16015.

// { added by Sergey for CR#11607
void AppMediaPlayer_Video_ScreenFSPlayback::onReadingFile(bool inProgress)
{
    MP_LOG << "Is in progress =" << inProgress << LOG_ENDL;

    if(inProgress)
    {
        pushToastPopUp(POPUP_TOAST_READING_FILE);
    }
    else
    {
        if(AppMediaPlayer_Video_Screens::SCREEN_POP_TOAST == m_pEngineVideo->getScreenManager(m_disp)->topScreenName())
        {
            m_pEngineVideo->getScreenManager(m_disp)->popScreen();
        }
    }
}
// } added by Sergey for CR#11607

QString AppMediaPlayer_Video_ScreenFSPlayback::makeElidedString( const QString &fullString,
                                                                 const QString &fontFamily,
                                                                 int pixelSize, int maxWidth,
                                                                 Qt::TextElideMode elideMode )
{
    //Creation font for properly calculate width of full string in the pixels.
    QFont mFont;
    mFont.setFamily( fontFamily );
  // mFont.setPixelSize( pixelSize );
    mFont.setPointSize( pixelSize );	//modified by aettie.ji 2012.11.28 for uxlaunch update

    QFontMetrics fontMetrics( mFont );
    return fontMetrics.elidedText( fullString, elideMode, maxWidth - 3 );
}


// { modified by Sergey 10.08.2013 for 182892
void AppMediaPlayer_Video_ScreenFSPlayback::setFullScreen()
{
    //if(this != dynamic_cast<AppMediaPlayer_Video_ScreenFSPlayback*>(m_pEngineVideo->getScreenManager(m_disp)->topScreen()))
    QString topScreenName = m_pEngineVideo->getScreenManager(m_disp)->topScreenName();
    if(AppMediaPlayer_Video_Screens::SCREEN_JUKEBOX_PLAYBACK != topScreenName &&
       AppMediaPlayer_Video_Screens::SCREEN_USB_PLAYBACK != topScreenName &&
       AppMediaPlayer_Video_Screens::SCREEN_USB2_PLAYBACK != topScreenName)
    {
        return;
    }

    if((m_pModel->playbackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING || 
        m_pModel->playbackStatus() == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED)
            // removed by Sergey 24.09.2013 for ITS#188938
            && !m_pModel->tuneMode()
            && !m_pControllerModel->dragging()) // added by kihyung 2013.06.21 for 0175221 
    {
        m_fullScreenTimer.start();
    }
    else
    {
        m_fullScreenTimer.stop();
    }
}


void AppMediaPlayer_Video_ScreenFSPlayback::showFullScreen()
{
    pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FULL);
}
// } modified by Sergey 10.08.2013 for 182892


void AppMediaPlayer_Video_ScreenFSPlayback::onFilenameChanged(QString filename)
{
     m_pModel->setFilename( makeElidedString(filename, "DH_HDB", 32, 1060 ,Qt::ElideNone) );
}

void AppMediaPlayer_Video_ScreenFSPlayback::onFoldernameChanged(QString foldername)
{
    m_pModel->setFilename2( makeElidedString(foldername, "DH_HDR", 28, 526));
}

void AppMediaPlayer_Video_ScreenFSPlayback::onSetTuneMode(bool on)
{
    m_pModel->setTuneMode(on);
    setFullScreen(); //{ added by Sergey for CR13772
}

// { added by yongkyun.lee  2012.10.17  for Current file is white color
void AppMediaPlayer_Video_ScreenFSPlayback::onTuneSameFile(bool on)
{
    m_pModel->setTuneSameFile(on);
}
// } added by  yongkyun.lee 

// removed by Sergey 01.05.2013 for Tune Functionality

void AppMediaPlayer_Video_ScreenFSPlayback::onFocusVisibilityChanged( bool value )
{
   m_pModel->setFocusVisible( value );
}

// removed by Sergey 24.09.2013 for ITS#188938


// { added by wspark 2012.08.21 for DQA #47
// { modified by wspark 2012.11.20 for CR15380
//void AppMediaPlayer_Video_ScreenFSPlayback::onShowPopup()
void AppMediaPlayer_Video_ScreenFSPlayback::onShowPopup(POPUP_TXT_BTN_TYPE_T popupType)
// } modified by wspark
{
    MP_LOG << LOG_ENDL;
    // { modified by wspark 2012.11.20 for CR15380
    //showTxtBtnPopUp( POPUP_TXT_BTN_COPY_EXISTED_FILE );
    showTxtBtnPopUp( popupType );
    // } modified by wspark
}
// } added by wspark 2012.08.21 for DQA #47

// { added by wspark 2013.02.21 for ISV 73305
void AppMediaPlayer_Video_ScreenFSPlayback::onSetCamMode(bool on)
{
    m_pModel->setCamMode(on);
}
// } added by wspark

// { added by Sergey 16.05.2013
void AppMediaPlayer_Video_ScreenFSPlayback::setupModeArea()
{
    QString thisScreen = "";
    QString title = "";

    switch(m_mediaSource)
    {
    case MEDIA_SOURCE_JUKEBOX:
    {
        thisScreen = AppMediaPlayer_Video_Screens::SCREEN_JUKEBOX_PLAYBACK;
        title = QT_TR_NOOP("STR_MEDIA_VIDEO_JUKEBOX"); // modified by Sergey 25.08.2013 for ITS#186435
        break;
    }
	//[KOR][ITS][181982][minor](aettie.ji)
    case MEDIA_SOURCE_USB1:
    {
	//[KOR][ITS][180278](aettie.ji)
        thisScreen = AppMediaPlayer_Video_Screens::SCREEN_USB_PLAYBACK;
        if(m_pEngineVideo->GetVariantRearUSB())
            title = QT_TR_NOOP("STR_MEDIA_VIDEO_USB_FRONT"); // modified by Sergey 25.08.2013 for ITS#186435
        else
            title = QT_TR_NOOP("STR_MEDIA_VIDEO_USB"); // modified by Sergey 25.08.2013 for ITS#186435
        break;
    }
    case MEDIA_SOURCE_USB2:
    {
        thisScreen = AppMediaPlayer_Video_Screens::SCREEN_USB2_PLAYBACK;
        if(m_pEngineVideo->GetVariantRearUSB())
            title = QT_TR_NOOP("STR_MEDIA_VIDEO_USB_REAR"); // modified by Sergey 25.08.2013 for ITS#186435
        else
            title = QT_TR_NOOP("STR_MEDIA_VIDEO_USB"); // modified by Sergey 25.08.2013 for ITS#186435
        break;
    }
    default:
        break;
    }

    notifyScreenManager(thisScreen, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_COUNTER_VISIBLE, QVariant(true));
    notifyScreenManager(thisScreen, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_TEXT, QVariant(title));
    // { added by cychoi 2013.06.20 for ITS 175384
    notifyScreenManager(thisScreen, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_LIST_BTN_VISIBLE, QVariant(true)); 
    notifyScreenManager(thisScreen, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_MENU_BTN_VISIBLE, QVariant(true)); 
    notifyScreenManager(thisScreen, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_TOTAL_NUMBER, QVariant(m_pControllerModel->displayPlaylistLength())); // modified by kihyung 2013.09.11 for ITS 0189474
    notifyScreenManager(thisScreen, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_CUR_NUMBER, QVariant(m_pControllerModel->displayPlaylistPos() + 1)); // modified by kihyung 2013.09.11 for ITS 0189474
    // } added by cychoi 2013.06.20
    //{ added by cychoi 2013.08.14 for ITS 184421
    notifyScreenManager(thisScreen, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_RTEXT_VISIBLE, QVariant(false));
    notifyScreenManager(thisScreen, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_ICON_VISIBLE, QVariant(false));
    //} added by cychoi 2013.08.14
}
// } added by Sergey 16.05.2013

// modified by ravikanth 12-07-13 for ITS 0174301
void AppMediaPlayer_Video_ScreenFSPlayback::onFMCopyCompleted()
{
    MP_LOG << LOG_ENDL;
    if(m_mediaSource != MEDIA_SOURCE_JUKEBOX)
    {
        pushToastPopUp(POPUP_TOAST_COPY_COMPLETED);
    }
}


// { added by Sergey 04.08.2013 for ITS#179312
void  AppMediaPlayer_Video_ScreenFSPlayback::onEventReceived(const QString &srcScreenName,
                                                              const QString &resScreenName,
                                                              const int eventId,
                                                              const QVariant arg)
{
    Q_UNUSED(srcScreenName);
    Q_UNUSED(arg);

    if(resScreenName == m_myName) // modified by Sergey 31.08.2013 for ITS#186799
    {
        switch( eventId )
        {
            case EVENTS_MOUSE_PRESSED:
            {
                onMousePressed();
                break;
            }
            case EVENTS_MOUSE_RELEASED:
            {
                onMouseReleased();
                break;
            }
            case EVENTS_RESTART_FULLSCREEN_TIMER:
            {
                m_fullScreenTimer.start();
                break;
            } // added by Sergey 31.08.2013 for ITS#186799
            case EVENTS_SET_FG_LOADING_MODE:
            {
                m_pModel->setFgLoadingMode(arg.toBool());
                break;
            } // added by Sergey 07.09.2013 for PRE_FG supoort
            case EVENTS_RESET_PENDING_POPUP:
            {
                if(m_pendingPopup != POPUP_TXT_BTN_MAX)
                {
                    m_pendingPopup = POPUP_TXT_BTN_MAX;
                    m_pEngineVideo->setPopupType(m_pendingPopup);//added by taihyun.ahn for ITS 221592 2014.01.23

                    MP_LOG << "No more pending popup." << LOG_ENDL;
                }

                break;
            } // added by Sergey 22.09.2013 for ITS#186731
            // { added by Sergey 27.10.2013 for ITS#198669
            case EVENTS_DRS_MODE:
            {
                onLockoutMode(arg.toBool());
                break;
            }
            // } added by Sergey 27.10.2013 for ITS#198669
            default:
                break;
        }
    }
}
// } added by Sergey 04.08.2013 for ITS#179312

// { added by Sergey 31.08.2013 for ITS#186799
QString AppMediaPlayer_Video_ScreenFSPlayback::identifyMyName(MEDIA_SOURCE_T source)
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
        default:
        {
            MP_LOG << "WARNING! Wrong source is given to this screen!" << LOG_ENDL;
            break;
        }
    }

    return result;
}
// } added by Sergey 31.08.2013 for ITS#186799


// { added by Sergey 15.09.2014 for ITS#189390
void AppMediaPlayer_Video_ScreenFSPlayback::onFgLoading(bool on)
{
    m_pModel->setFgLoadingMode(on);
}
// } added by Sergey 15.09.2014 for ITS#189390


// { added by Sergey 22.09.2013 for ITS#186731
void AppMediaPlayer_Video_ScreenFSPlayback::onDurationChanged(int duration)
{
    Q_UNUSED(duration);

    if(m_pendingPopup != POPUP_TXT_BTN_MAX)
    {
        m_pendingPopup = POPUP_TXT_BTN_MAX;
        m_pEngineVideo->setPopupType(m_pendingPopup);//added by taihyun.ahn for ITS 221592 2014.01.23

        MP_LOG << "No more pending popup." << LOG_ENDL;
    }
}
// } added by Sergey 22.09.2013 for ITS#186731


// { added by Sergey 26.11.2013 for Root folder translation
void AppMediaPlayer_Video_ScreenFSPlayback::onRetranslateUI()
{
    m_pModel->retranslateUI(VIDEO_SOURCE_FS);
}
// } added by Sergey 26.11.2013 for Root folder translation
