#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerDVD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_DVD.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDVDPlayback.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Playback.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"
#include "util/osd/OSD_Manager.h"// modified by yongkyun.lee 2013-07-22 for : NO CR DVD title OSD

AppMediaPlayer_Video_ScreenDVDPlayback::
AppMediaPlayer_Video_ScreenDVDPlayback( AppMediaPlayer_Video_Controller *pController,
                                        AppMediaPlayer_EngineVideo *pEngineVideo,
                                        int disp,
                                        QObject *parent )
    : AppMediaPlayer_Video_Screen( parent ),
      m_pController( pController ),
      m_pEngineVideo( pEngineVideo ),
      m_isLockout(false),
      m_nScreenState(PLAYER_PLAY_DISPLAY), // added by cychoi 2013.06.23 for DVD VCD screen management
      m_disp(disp)
{
    m_pControllerDVD = static_cast<AppMediaPlayer_Video_ControllerDVD*>(m_pController->getController(MEDIA_SOURCE_DVDVIDEO));
    m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(m_pControllerDVD->getModel());

    m_pModel = new AppMediaPlayer_Video_SModel_Playback(this);
    setModel(m_pModel);

    m_pModel->setProgressBarMode("DVD");
    
    m_overlays << AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA;
    m_pModel->bindWith( m_pControllerModel );

    m_fullScreenTimer.setInterval( FULL_SCREEN_INTERVAL );

    //connect(m_pEngineVideo, SIGNAL(lockoutMode(bool)), this, SLOT(onLockoutMode(bool)));
    connect(m_pEngineVideo, SIGNAL(setTuneMode(bool)), this, SLOT(onSetTuneMode(bool)));  
	// removed by Sergey 01.05.2013 for Tune Functionality
    connect(m_pEngineVideo, SIGNAL(tuneSameFile(bool)), this, SLOT(onTuneSameFile(bool)));  // added by  yongkyun.lee  2012.10.17  for Current file is white color
    connect(m_pEngineVideo, SIGNAL(showTitleMenu(int)), this, SLOT(onScreenStateChanged(int))); // added by kihyung 2012.07.13 for CR 9867
    connect(&m_fullScreenTimer, SIGNAL(timeout()), this, SLOT(showFullScreen()));
    connect(m_pControllerModel,
            SIGNAL(playbackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)),
            this,
            SLOT(onPlaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)));

    connect(m_pEngineVideo->getScreenManager(m_disp), SIGNAL(isFocusVisibleChanged(bool)),
            this, SLOT(onFocusVisibilityChanged(bool)));

    // removed by Sergey 24.09.2013 for ITS#188938

    // { added by kihyung 2012.07.28 for CR 11057
    connect( m_pControllerDVD, SIGNAL(screenStateChanged(int)),
             this, SLOT(onScreenStateChanged(int)) );
    // } added by kihyung
    connect( m_pControllerDVD, SIGNAL(cancelFfRew()),   this, SIGNAL(cancelFfRew()));  // added by yongkyun 2013.01.19 for ITS 150703
    // removed by Sergey 23.09.2013 for ITS#188498
    // removed by sjhyun 2013.11.05 for ITS 206357
    connect(m_pEngineVideo, SIGNAL(setCamMode(bool)), this, SLOT(onSetCamMode(bool))); // added by wspark 2013.02.21 for ISV 73305

    connect(&m_pEngineVideo->getEngineMain(), SIGNAL(retranslateUi(QVariant)),
            this, SLOT(onRetranslateUI())); // added by cychoi 2013.09.03 for ITS 187585

    connect(m_pEngineVideo, SIGNAL(fgLoading(bool)), this, SLOT(onFgLoading(bool))); // added by Sergey 15.09.2014 for ITS#189390
}

AppMediaPlayer_Video_ScreenDVDPlayback::~AppMediaPlayer_Video_ScreenDVDPlayback()
{
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onHide()
{
    MP_LOG << LOG_ENDL;

    emit releaseLongPress(); // added by junggil 2012.09.06 for keep the FF or REW mode when change to another screen. 
    // { added by sjhyun 2013.11.05 for ITS 206357
    //emit showInhibitionIcon(false); // added by kihyung 2012.12.01 for ISV 62687
    m_pEngineVideo->inhibitionIcon(false);
    // } added by sjhyun
    m_fullScreenTimer.stop();

    // { commentted by kihyung 2012.07.28 for CR 11057
    /*
    disconnect( m_pControllerDVD, SIGNAL(screenStateChanged(int)),
                this, SLOT(onScreenStateChanged(int)) );
    */
    // } commentted by kihyung
}


void AppMediaPlayer_Video_ScreenDVDPlayback::onShow( QVariant arg )
{
    MP_LOG << LOG_ENDL;

    // removed by Sergey 26.07.2013 for ITS#181096

    emit setItemProperty(QString("playbackGestureArea"), "enabled", QVariant(true));
    emit setItemProperty(QString("playbackControlls"), "isCommonJogEnabled", QVariant(true)); // added by Dmitry Bykov 08.04.13 for touch rule
	//m_isLockout = m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode();
	onLockoutMode(m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode()); //modified by edo.lee 2013.03.20
	
    if ( arg.toString() == "Options" /*&& !m_isLockout*/ && !m_pModel->loadingMode())//modified by edo.lee 2013.05.02
    {
        pushScreen(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS, QVariant("DVD"));
    }

    //m_pControllerModel->setListBtnVisible(true); // deleted by lssanh 2013.06.05 Lockout mode // added by wspark 2013.03.29 for ISV 77716

    //{ added for eunhye 2013.03.19
    // { modified by lssanh 2013.06.05 Lockout mode
    if((m_pEngineVideo->getFullScreenState() || 
        m_pEngineVideo->getScreenManager(m_disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_FULL ||
        m_pEngineVideo->getScreenManager(m_disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK) &&
        !m_isLockout)
    // { modified by lssanh 2013.06.05 Lockout mode
        //{ modified for eunhye 2013.04.09
    {
        ASPECT_RATIO_T currAspectRatio;
        currAspectRatio = m_pEngineVideo->GetVideoController()->getAspectRatio();
        switch(currAspectRatio)
        {
        case ASPECT_RATIO_FULL:
        case ASPECT_RATIO_16_9:
            m_pController->SetVideoSizeWrtDisplay( 0, 0, 1280, 720 );
            break;
        case ASPECT_RATIO_4_3:
            m_pController->SetVideoSizeWrtDisplay( 160, 0, 960, 720 );
            break;
        default:
            break;
        }

    }

    setupModeArea(); // added by Sergey 16.05.2013
    setFullScreen();

//	m_pEngineVideo->getEngineMain().setPlaybackScreen(m_disp == 0); // remove by edo.lee 2013.08.23
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onDeactivate()
{
    emit setItemProperty(QString("playbackGestureArea"), "enabled", QVariant(false));
    emit setItemProperty(QString("playbackControlls"), "isCommonJogEnabled", QVariant(false)); // added by Dmitry Bykov 08.04.13 for touch rule

    // commented by cychoi 2013.10.13 for ITS 195278 Should not handle DRS on Deactivate
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onMousePressed()
{
    //Reset full screen timer by any touch.
    m_fullScreenTimer.stop();
}


void AppMediaPlayer_Video_ScreenDVDPlayback::onMouseReleased()
{
    if(!m_pController->isSeekMode()) // added by Sergey 13.09.2013 for ITS#188904
        m_fullScreenTimer.start(); // modified by ravikanth 06-07-13 for SANITY fill screen transition on drag finish
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onMouseClicked(bool pbArea)
{
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
     	// removed by Sergey 10.08.2013 for 182892
        showFullScreen();
    }
}


void AppMediaPlayer_Video_ScreenDVDPlayback::onModeAreaTapped()
{
}


void AppMediaPlayer_Video_ScreenDVDPlayback::onButtonAreaTapped()
{
}


void AppMediaPlayer_Video_ScreenDVDPlayback::onSoftkeyBack()
{
    emit popScreen();
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onSoftkeyBtnHold( QString button )
{
    Q_UNUSED(button);
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onSoftkeyBtnHoldCritical( QString button )
{
    Q_UNUSED(button);
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onSoftkeyBtnReleased( QString button )
{
    Q_UNUSED(button);
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onPlaybackStatusChanged( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T status)
{
    // { add by kihyung 2012.07.05 for CR 9855
    // Q_UNUSED(status);
    switch(status) 
    {
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING:
    // case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_STOPPED: // removed by Sergey 03.05.2013 for ISV_KR#76907
        emit resetSpeedIcon();
        m_pController->PrevNextOSD(false);// added by yongkyun.lee 20130320 for : NO CR ff/rew OSD
        break;
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_FASTFORWARDING_4X:
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_REWINDING_4X:
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
        emit setSpeedIcon(FAST_SPEED_X20);
        m_pController->PrevNextOSD(true); // added by yongkyun.lee 20130320 for : NO CR ff/rew OSD
        break;
    default:
        break;
    }
    // } add by kihyung
    setFullScreen();
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onMenuKeyPressed()
{
   MP_LOG << LOG_ENDL;
	if(/*m_isLockout ||*/ m_pModel->loadingMode()) return; //modified by edo.lee 2013.05.02
   pushScreen(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS, QVariant("DVD"));
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onOptionsItemClicked( int item )
{
    MP_LOG << " item " << item << LOG_ENDL;
	if(/*m_isLockout ||*/ m_pModel->loadingMode()) return; //modified by edo.lee 2013.02.28
    switch(item)
    {
       // { commented by cychoi 2013.07.13 for List View spec out
       //case OPTIONS_LIST:
       //{
       //   MP_LOG <<"Search_DVD pressed " << LOG_ENDL;
       //   listButtonHandler();
       //   break;
       //}
       // } commented by cychoi 2013.07.13

    case OPTIONS_SETTINGS:
    {
         MP_LOG <<"Settings pressed" << LOG_ENDL;
//added by aettie 20130612 for 172347
         //if(m_pEngineVideo->getIsShowLockoutView() != VIDEO_DISPLAY_MAX)
    	   {
                m_pControllerDVD->InitDeckLang(); // added by cychoi 2013.09.08 for ITS 181382 EVT_INITIALIZATION_REQUEST
    	        m_pEngineVideo->onShowSettings("DVD_Settings", QVariant("DISC"));
				m_pEngineVideo->LaunchSubScreenOnVideo(VIDEO_DVD_SETTING, m_disp);
    	   }
    	   //else
      	//	  emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS, QVariant("DISC"));

         break;
    }

    case OPTIONS_SOUND_SET:
    {
          m_pEngineVideo->launchSettings(AppSettingsDef::eStart_Mode_Sound, MP::DISC, false,m_disp); // modified by edo.lee 2013.05.02
          break;
    }

    case OPTIONS_DISPLAY_SET:
    {
           MP_LOG << "should launch VideoSettings" << LOG_ENDL;

           m_pEngineVideo->launchSettings(AppSettingsDef::eStart_Mode_Screen, MP::DISC, false,m_disp); // modified by edo.lee 2013.05.02
           break;
     }

     case OPTIONS_TITLE_MENU:
     {
          MP_LOG<<"Title Menu pressed" << LOG_ENDL;
          // { added by cychoi 2014.02.20 for (HMC Request)
          if(m_isLockout)
          {
              // show DRS popup, just return
              // { modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327
              m_pEngineVideo->showTxtBtnPopup(POPUP_TXT_BTN_DRIVING_REGULATION, (VIDEO_DISPLAY_T)m_disp);
              //showTxtBtnPopUp(POPUP_TXT_BTN_DRIVING_REGULATION);
              // } modified by cychoi 2014.03.13
              break;
          }
          // } added by cychoi 2014.02.20
          m_pControllerDVD->titleMenu();
          break;
     }

     case OPTIONS_DISC_MENU:
     {
           MP_LOG<<"Disc Menu pressed" << LOG_ENDL;
           // { added by cychoi 2014.02.20 for (HMC Request)
           if(m_isLockout)
           {
               // show DRS popup, just return
               // { modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327
               m_pEngineVideo->showTxtBtnPopup(POPUP_TXT_BTN_DRIVING_REGULATION, (VIDEO_DISPLAY_T)m_disp);
               //showTxtBtnPopUp(POPUP_TXT_BTN_DRIVING_REGULATION);
               // } modified by cychoi 2014.03.13
               break;
           }
           // } added by cychoi 2014.02.20
           m_pControllerDVD->topMenu();
           break;
     }

     case OPTIONS_FULL_SCREEN:
     {
          showFullScreen();
          break;
     }
	 // { modified by ravikanth CR 15994 12-11-22
     case OPTIONS_VIDEO_SWITCH_CAPTION:
     {
        m_pControllerDVD->switchCaption();
        break;
     }
     case OPTIONS_VIDEO_SWITCH_VOICE:
     {
        m_pControllerDVD->switchVoice();
        break;
     }
     case OPTIONS_VIDEO_SWITCH_ANGLE:
     {
        m_pControllerDVD->switchAngle();
        break;
     }
	 // } modified by ravikanth CR 15994 12-11-22
       default: //make compiler happy
          break;
    }
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onEventReceived( const QString &srcScreenName,
                                                              const QString &resScreenName,
                                                              const int eventId,
                                                              const QVariant arg )
{
   MP_LOG << " srcScreenName = " << srcScreenName << " resScreenName = " << resScreenName << \
             " eventId = " << eventId << " arg = " << arg << LOG_ENDL;

   Q_UNUSED(srcScreenName);
   Q_UNUSED(arg);

   if ( resScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK )
   {
      switch( eventId )
      {
          case EVENTS_SHOW_PLAYBACK_ONLY:
             hide();
             break;
          case EVENTS_MOUSE_PRESSED: // added by Sergey 04.08.2013 for ITS#179312
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
          // { added by cychoi 2013.12.09 for ITS 214030 DRS on Popup
          case EVENTS_DRS_MODE:
          {
              onLockoutMode(arg.toBool());
              break;
          }
          // } added by cychoi 2013.12.09
          default:
             break; // make compiler happy
      }
   }
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onTogglePlayBtn()
{
    //{ added by yongkyun.lee 20130314 for : Video Play/Pause 
    if(m_pEngineVideo->IsPlaying())
        m_pEngineVideo->setPassPlay(true);
    
    // { modified by kihyung 2013.07.30 for ITS 0174622
    if(m_pControllerDVD->getModel()->playbackStatus() != AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING) {
        //{ modified by cychoi 20103.08.13 for ITS 183912 Sound mute on toggle play
        //added by edo.lee 2013.04.20
        //m_pEngineVideo->getEngineMain().setUnMuteState(false);
        m_pEngineVideo->getEngineMain().setMuteForSeek(false);
        //} modified by cychoi 20103.08.13
    }
    // } modified by kihyung 2013.07.30 for ITS 0174622    
    
    //} added by yongkyun.lee 20130314 
    m_pControllerDVD->togglePlay();
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onLongPressPlayBtn()
{
//    showFullScreen(); // commented by Sergey 24.07.2013 ITS#181467
}

void AppMediaPlayer_Video_ScreenDVDPlayback::listButtonHandler()
{
   // { commented by cychoi 2013.07.13 for List View spec out
   //if(m_isLockout) return; // added by lssanh 2013.06.05 Lockout mode
   // modified by minho 20120828
   // { for able list menu on DVD
   //if(m_pControllerModel->chapterCount() > 0)
   //if(m_pControllerDVD->getChapterCount() > 0)
   // } modified by minho
   //{
   //   m_pControllerDVD->storeDiscState();
   //   emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLES);
   //}
   // } commented by cychoi 2013.07.13
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onLockoutMode( bool on )
{
   MP_LOG << "on =" << on << LOG_ENDL;

   //m_pModel->setLockoutMode(on);
   m_isLockout = on;
   // { commented by cychoi 2013.07.13 for List View spec out
   // { added by lssanh 2013.06.05 Lockout mode
   //notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
   //					EVENTS_SET_LIST_BTN_VISIBLE, QVariant(!m_isLockout));	   
   // } added by lssanh 2013.06.05 Lockout mode
   // } commented by cychoi 2013.07.13

   //emit showLockout(on); //deleted by yungi 2013.11.03 for ITS 198971
   invokeQmlMethod("onShowLockout", QVariant(on)); //added by yungi 2013.11.03 for ITS 198971
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onFocusVisibilityChanged( bool value )
{
    m_pModel->setFocusVisible( value );
}

// removed by Sergey 24.09.2013 for ITS#188938

// { commented by cychoi 2014.06.27 for removed too many logs... // { added by kihyung 2012.07.13 for CR 9867
// { modified by eugene 2012.08.23 for static analysis defect #7578 
//bool AppMediaPlayer_Video_ScreenDVDPlayback::setSelectByTitleMenu(bool bShow)
//void AppMediaPlayer_Video_ScreenDVDPlayback::setSelectByTitleMenu(bool bShow)
// } modified by eugene 2012.08.23 for static analysis defect #7578 
//{
//    m_pControllerDVD->setSelectByTitleMenu(bShow);
//}

//bool AppMediaPlayer_Video_ScreenDVDPlayback::getSelectByTitleMenu()
//{
//    return m_pControllerDVD->getSelectByTitleMenu();
//}
// } commented by cychoi 2014.06.27 // } added by kihyung

void AppMediaPlayer_Video_ScreenDVDPlayback::onPrevBtn()
{
    //m_pEngineVideo->getEngineMain().setUnMuteState(false); // commented by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute //added by edo.lee 2013.04.20
    m_pControllerDVD->prev();
    m_pEngineVideo->DisplayOSD(true); // added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD  
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onNextBtn()
{
    //m_pEngineVideo->getEngineMain().setUnMuteState(false); // commented by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute //added by edo.lee 2013.04.20
    m_pControllerDVD->next();
    m_pEngineVideo->DisplayOSD(true); // added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD  
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onFlick_next()
{
    m_pControllerDVD->flickNext();
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onFlick_prev()
{
    m_pControllerDVD->flickPrev();
}

// { added by cychoi 2013.07.07 for Smoke Test - Cue is not responsed on tune mode
void AppMediaPlayer_Video_ScreenDVDPlayback::onSelectTuned()
{ 
    m_pControllerDVD->selectTuned();	
    onSetTuneMode(false);	
    m_pControllerDVD->cancelTune();
}
// } added by cychoi 2013.07.07 

void AppMediaPlayer_Video_ScreenDVDPlayback::onNextBtnHold()
{
    m_pControllerDVD->fastForward();
    // emit setSpeedIcon(FAST_SPEED_X4); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onNextBtnHoldCritical()
{
    m_pControllerDVD->fastForwardCritical();
    // emit setSpeedIcon(FAST_SPEED_X16); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onPrevBtnHold()
{
    m_pControllerDVD->rewind();
    // emit setSpeedIcon(FAST_SPEED_X4); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onPrevBtnHoldCritical()
{
    m_pControllerDVD->rewindCritical();
    // emit setSpeedIcon(FAST_SPEED_X16); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenDVDPlayback::onCancel_ff_rew()
{
	m_pEngineVideo->getEngineMain().setUnMuteState(false);//added by edo.lee 2013.04.20
    m_pControllerDVD->cancel_ff_rew();
	//if(m_pEngineVideo->getEngineMain().getisBTCall()) // deleted by lssanh 2013.06.26 bt call //added by edo.lee 2013.06.19
	//	 m_pControllerDVD->pause(); // deleted by lssanh 2013.06.26 bt call
	
    // emit resetSpeedIcon(); // add by kihyung 2012.06.20 for CR 9855
}


// { modified by Sergey 10.08.2013 for 182892
void AppMediaPlayer_Video_ScreenDVDPlayback::showFullScreen()
{
    pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FULL);
}


void AppMediaPlayer_Video_ScreenDVDPlayback::setFullScreen()
{
    //if(this != dynamic_cast<AppMediaPlayer_Video_ScreenDVDPlayback*>(m_pEngineVideo->getScreenManager(m_disp)->topScreen()))
    if(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK != m_pEngineVideo->getScreenManager(m_disp)->topScreenName())
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
// } modified by Sergey 10.08.2013 for 182892

void AppMediaPlayer_Video_ScreenDVDPlayback::onScreenStateChanged( int nScreenState )
{
    // removed by cychoi 2013.08.05 for unused code
    MP_LOG << "nScreenState = " << nScreenState << LOG_ENDL;
    m_nScreenState = nScreenState; // added by cychoi 2013.06.23 for DVD VCD screen management

    switch (nScreenState)
    {
    // { added by cychoi 2013.06.16 ITS 167653 DVD Setting screen error
    case PLAYER_PLAY_DISPLAY:
    {
        // { modified by cychoi 2013.06.23 for DVD VCD screen management
        QString topScreenName = m_pEngineVideo->getScreenManager(m_disp)->topScreenName();
        // { modified by cychoi 2013.08.05 for ScreenOption should be kept when screen is changed to SCREEN_DVD_PLAYBACK by onScreenStateChanged
        if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL)
        {
            m_pEngineVideo->getScreenManager(m_disp)->popScreen(QVariant());
            /* Return the Playback screen. */
            m_pEngineVideo->getScreenManager(m_disp)->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK);
        }
        else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
        {
            /* Return the Playback screen. */
            // { modified by yungi 2013.09.08 for ITS 186704
            if(m_isLockout)
                m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, 1, QVariant(), false);
            else
            {
                m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, 1, QVariant());
            }
            // { modified by cychoi 2013.11.06 for ITS 206356
            ASPECT_RATIO_T currAspectRatio;
            currAspectRatio = m_pEngineVideo->GetVideoController()->getAspectRatio();
            switch(currAspectRatio)
            {
            case ASPECT_RATIO_FULL:
            case ASPECT_RATIO_16_9:
                m_pController->SetVideoSizeWrtDisplay( 0, 0, 1280, 720 );
                break;
            case ASPECT_RATIO_4_3:
                m_pController->SetVideoSizeWrtDisplay( 160, 0, 960, 720 );
               break;
            default:
                break;
            }
            //m_pController->SetVideoSizeWrtDisplay( 0, 0, 1280, 720 ); // modified by yongkyun.lee 2013-11-05 for ITS 206356
            // } modified by cychoi 2013.11.06
            // } modified by yungi 2013.09.08
        }
        // } modified by cychoi 2013.08.05
        // { commented by cychoi 2013.08.05 for List View spec out
        //else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLES ||
        //        topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_CHAPTERS)
        //{
            /* Return the Playback screen. */
        //    if (m_isLockout || nScreenState == PLAYER_ROOTMENU_DISPLAY)
        //    {
        //        m_pEngineVideo->getScreenManager(m_disp)->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK);
        //    }
        //    else
        //    {
                /* Kepp the current screen and change main screen to Playback  */
        //        if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLES)
        //            m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, 1, QVariant());
        //        else
        //            m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, 2, QVariant());
        //    }
        //}
        // } commented by cychoi 2013.08.05
        else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS ||
                topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION)
        {
            /* Kepp the current screen and change main screen to Playback  */
            if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS)
                m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, 1, QVariant());
            else
                m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, 2, QVariant());
        }
        else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON)
        {
            /* Kepp the current screen and change main screen to Playback  */
            QString prevScreenName = m_pEngineVideo->getScreenManager(m_disp)->getPreviousScreenName(1);
            if(prevScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS)
                m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, 2, QVariant());
            else if(prevScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION)
                m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, 3, QVariant());
            else
            {
                // { modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327
                m_pEngineVideo->getScreenManager(m_disp)->popScreen(QVariant());
                m_pEngineVideo->getScreenManager(m_disp)->replaceScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, QVariant());
                m_pEngineVideo->showTxtBtnPopup(POPUP_TXT_BTN_DRIVING_REGULATION, (VIDEO_DISPLAY_T)m_disp);
                //m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, 1, QVariant());
                // } modified by cychoi 2014.03.13
            }
        }
        else if(topScreenName != AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK)
        {
            /* Change the Playback screen. */
            // { added by cychoi 2013.11.09 for ITS 207825 DVD Screen on DisplaySetting
            // { commented by cychoi 2014.06.27 for removed too many logs... // { moved from Title/Disc Menu by cychoi 2013.08.05. added by kihyung 2012.07.13 for CR 9867
            //if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU)
            //    m_pControllerDVD->setSelectByTitleMenu(true);
            // } commented by cychoi 2014.06.27 // } moved  from Title/Disc Menu by cychoi 2013.08.05
            m_pEngineVideo->getScreenManager(m_disp)->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK);
            if(m_pEngineVideo->isVideoInFG() == false) // modified by yongkyun.lee 2013-07-11 for : ITS 179418
            {
                m_fullScreenTimer.stop();
            }
            // } added by cychoi 2013.11.09
        }
        // } modified by cychoi 2013.06.23
        MP_LOG << "topScreenName = " << m_pEngineVideo->getScreenManager(m_disp)->topScreenName() << LOG_ENDL;
        break;
    }
    case PLAYER_TOPMENU_DISPLAY:
    case PLAYER_ROOTMENU_DISPLAY:
    case PLAYER_TITLEMENU_DISPLAY:
    {
        // { modified by cychoi 2013.06.23 for DVD VCD screen management
        QString topScreenName = m_pEngineVideo->getScreenManager(m_disp)->topScreenName();
        // { modified by cychoi 2013.08.05 for ScreenOption should be kept when screen is changed to SCREEN_DVD_TITLE_DISC_MENU by onScreenStateChanged
        if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL)
        {
            m_pEngineVideo->getScreenManager(m_disp)->popScreen(QVariant());
            /* Return the Title/Disc Menureen. */
            m_pEngineVideo->getScreenManager(m_disp)->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU);
        }
        else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
        {
            /* Return the Title/Disc Menu screen. */

            // { modified by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen
            m_pEngineVideo->getScreenManager(m_disp)->popScreen(QVariant());
            m_pEngineVideo->getScreenManager(m_disp)->replaceScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, QVariant());
            // { modified by yungi 2013.09.08 for ITS 186704
            //if(m_isLockout)
            //    m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, 1, QVariant(), false);
            //else
            //    m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, 1, QVariant());
            // } modified by yungi 2013.09.08
            // { modified by cychoi 2013.11.06 for ITS 206356
            //if(m_pEngineVideo->getEngineMain().middleEast())
            //    m_pController->SetVideoSizeWrtDisplay( 401 , 210 , 780, 435 );
            //else
            //    m_pController->SetVideoSizeWrtDisplay( 94 , 210 , 780, 435 );
            // } modified by cychoi 2013.11.06 for ITS 206356
            // } modified by cychoi 2014.03.18
        }
        // } modified by cychoi 2013.08.05
        // { commented by cychoi 2013.08.05 for List View spec out
        //else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLES ||
        //        topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_CHAPTERS)
        //{
            /* Return the Title/Disc Menu screen. */
        //    if (m_isLockout || nScreenState == PLAYER_ROOTMENU_DISPLAY)
        //    {
        //        m_pEngineVideo->getScreenManager(m_disp)->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU);
        //    }
        //    else
        //    {
                /* Kepp the current screen and change main screen to Title/Disc Menu  */
        //        if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLES)
        //            m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, 1, QVariant());
        //        else
        //            m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, 2, QVariant());
        //    }
        //}
        // } commented by cychoi 2013.08.05
        else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS ||
                topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION)
        {
            /* Kepp the current screen and change main screen to Title/Disc Menu */
            if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS)
                m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, 1, QVariant());
            else
                m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, 2, QVariant());
        }
        // { added by cychoi 2014.02.20 for (HMC Request)
        else if(topScreenName == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON)
        {
            /* Kepp the current screen and change main screen to Playback  */
            QString prevScreenName = m_pEngineVideo->getScreenManager(m_disp)->getPreviousScreenName(1);
            if(prevScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS)
                m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, 2, QVariant());
            else if(prevScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION)
                m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, 3, QVariant());
            else
            {
                // { modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327
                m_pEngineVideo->getScreenManager(m_disp)->popScreen(QVariant());
                m_pEngineVideo->getScreenManager(m_disp)->replaceScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, QVariant());
                m_pEngineVideo->showTxtBtnPopup(POPUP_TXT_BTN_DRIVING_REGULATION, (VIDEO_DISPLAY_T)m_disp);
                //m_pEngineVideo->getScreenManager(m_disp)->replacePrevScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU, 1, QVariant());
                // } modified by cychoi 2014.03.13
            }
        }
        // } added by cychoi 2014.02.20
        else if(topScreenName != AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU)
        {
            // { added by cychoi 2014.03.18 for ITS 229882
            if(m_pModel->tuneMode() == true)
            {
                m_pModel->setTuneMode(false);   
            }
            // } added by cychoi 2014.03.18
            /* Change the Title/Disc Menu screen. */
            m_pEngineVideo->getScreenManager(m_disp)->changeScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU);
        }
        // } modified by cychoi 2013.06.23
        //m_pEngineVideo->getEngineMain().GetNotifier()->DisplayDVDMenuOSD(); // commented by cychoi 2013.07.13 for Smoke Test DVD menu OSD wrong display // modified by yongkyun.lee 2013-07-06 for : ITS 178575
        //{ modified by yongkyun.lee 2013-07-22 for : NO CR DVD title OSD
        if(m_pEngineVideo->getScreenDVDModeChanged() && !m_pEngineVideo->isDVDTrackPlaying()) // added by cychoi 2013.09.04 for ITS 186733
        {
            OSDManager::instance()->displayDVDMenuMode();
        }
        //} modified by yongkyun.lee 2013-07-22 
        MP_LOG << "topScreenName = " << m_pEngineVideo->getScreenManager(m_disp)->topScreenName() << LOG_ENDL;
        break;
    }
    // } added by cychoi 2013.06.16
    // removed by cychoi 2013.06.16 code arrangement
    default:
    {
        //ignore
        break;
    }
    }
    
    m_pEngineVideo->setScreenDVDModeChanged(false);// modified by yongkyun.lee 2013-07-22 for : NO CR DVD title OSD
}
// { added by  yongkyun.lee 2012.10.17  for Current file is white color
void AppMediaPlayer_Video_ScreenDVDPlayback::onTuneSameFile(bool on)
{
    m_pModel->setTuneSameFile(on);
}
// } added by  yongkyun.lee 

void AppMediaPlayer_Video_ScreenDVDPlayback::onSetTuneMode(bool on)
{
    m_pModel->setTuneMode(on);
    setFullScreen(); //{ added by Sergey for CR13772

    //{ modified by yongkyun.lee 2013-07-05 for : ITS 139453
    // if (!on && !m_pModel->tuneSameFile())
    // {
    //     m_fullScreenTimer.setInterval( 1000 );
    //     m_fullScreenTimer.start();
    // }
    //} modified by yongkyun.lee 2013-07-05 
}

// removed by Sergey 01.05.2013 for Tune Functionality

// { added by wspark 2013.02.21 for ISV 73305
void AppMediaPlayer_Video_ScreenDVDPlayback::onSetCamMode(bool on)
{
    m_pModel->setCamMode(on);
}
// } added by wspark

// { added by cychoi 2013.09.03 for ITS 187585
void AppMediaPlayer_Video_ScreenDVDPlayback::onRetranslateUI()
{
    // { modified by cychoi 2013.12.05 for ITS 212884
    if(m_pEngineVideo->GetVideoController()->getSource() != MEDIA_SOURCE_DVDVIDEO ||
       m_pEngineVideo->getEngineMain().GetLastPlayerMode() != APP_MEDIA_MODE_VIDEO)
    {
        MP_LOG<<"mode is not matched" << LOG_ENDL;
    }
    else
    {
        m_pModel->retranslateUI(VIDEO_SOURCE_DVD);
    }

    m_pControllerDVD->retranslateUI(); // added by cychoi 2013.10.22 for ITS 197384
    // } modified by cychoi 2013.12.05
}
// } added by cychoi 2013.09.03

void AppMediaPlayer_Video_ScreenDVDPlayback::setupModeArea()
{
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_COUNTER_VISIBLE, QVariant(false));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_TEXT, QVariant(QT_TR_NOOP("STR_MEDIA_MNG_DVD"))); // modified by Sergey 25.08.2013 for ITS#186435
    // { modified by cychoi 2013.07.13 for List View spec out
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_LIST_BTN_VISIBLE, QVariant(false)); 
    // { modified by lssanh 2013.06.05 Lockout mode
    // { modified by cychoi 2013.06.23 for DVD VCD screen management
    //if(m_nScreenState == PLAYER_PLAY_DISPLAY)
    //{
    // } modified by cychoi 2013.06.23
    //    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
    //                        EVENTS_SET_LIST_BTN_VISIBLE, QVariant(false)); 
    //                        //EVENTS_SET_LIST_BTN_VISIBLE, QVariant(!m_isLockout)); 
    // { modified by cychoi 2013.06.23 for DVD VCD screen management
    //}
    //else
    //{
    //    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
    //                        EVENTS_SET_LIST_BTN_VISIBLE, QVariant(false)); 
    //}
    // } modified by cychoi 2013.06.23
    // } modified by cychoi 2013.07.13
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_MENU_BTN_VISIBLE, QVariant(true)); 
    // } modified  by lssanh 2013.06.05 Lockout mode
    //{ added by cychoi 2013.08.14 for ITS 184421
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_RTEXT_VISIBLE, QVariant(false));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_ICON_VISIBLE, QVariant(false));
    //} added by cychoi 2013.08.14
} // added by Sergey 16.05.2013


// { added by Sergey 15.09.2014 for ITS#189390
void AppMediaPlayer_Video_ScreenDVDPlayback::onFgLoading(bool on)
{
    m_pModel->setFgLoadingMode(on);
}
// } added by Sergey 15.09.2014 for ITS#189390
