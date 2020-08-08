#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerVCD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_VCD.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenVCDPlayback.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Playback.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"
#include "deckengineermode.h" // added by kihyung 2012.07.12

AppMediaPlayer_Video_ScreenVCDPlayback::
AppMediaPlayer_Video_ScreenVCDPlayback( AppMediaPlayer_Video_Controller *pController,
                                        AppMediaPlayer_EngineVideo *pEngineVideo,
                                        int disp,
                                        QObject *parent )
    : AppMediaPlayer_Video_Screen( parent ),
      m_pController( pController ),
      m_pEngineVideo( pEngineVideo ),
      m_isLockout(false),
      m_disp(disp)
{
    m_pControllerVCD = static_cast<AppMediaPlayer_Video_ControllerVCD*>(m_pController->getController(MEDIA_SOURCE_VCD));
    m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_VCD*>(m_pControllerVCD->getModel());

    m_pModel = new AppMediaPlayer_Video_SModel_Playback(this);
    setModel( m_pModel );

    m_pModel->setProgressBarMode("VCD");

    m_overlays << AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA;
    m_pModel->bindWith( m_pControllerModel );

    m_fullScreenTimer.setInterval( FULL_SCREEN_INTERVAL );

    //connect(m_pEngineVideo, SIGNAL(lockoutMode(bool)), this, SLOT(onLockoutMode(bool)));
    connect(m_pEngineVideo, SIGNAL(setTuneMode(bool)), this, SLOT(onSetTuneMode(bool)));
    connect(m_pEngineVideo, SIGNAL(tuneSameFile(bool)), this, SLOT(onTuneSameFile(bool))); // added by yongkyun.lee  2012.10.17  for Current file is white color
	// removed by Sergey 01.05.2013 for Tune Functionality
    connect(&m_fullScreenTimer, SIGNAL(timeout()), this, SLOT(showFullScreen()));
    connect(m_pControllerModel,
            SIGNAL(playbackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)),
            this,
            SLOT(onPlaybackStatusChanged(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)));

    connect( m_pEngineVideo->getScreenManager(m_disp), SIGNAL(isFocusVisibleChanged(bool)),
            this, SLOT(onFocusVisibilityChanged(bool)));

    // removed by Sergey 24.09.2013 for ITS#188938

    connect(m_pControllerVCD, SIGNAL(cancelFfRew()),   this, SIGNAL(cancelFfRew()));  // added by yongkyun 2013.01.19 for ITS 150703
    // removed by Sergey 23.09.2013 for ITS#188498

    connect(m_pControllerVCD, SIGNAL(showPlaybackControls(bool)), this, SIGNAL(showPlaybackControls(bool))); // added by kihyung 2012.08.27 for Video(LGE) #12
    // removed by sjhyun 2013.11.05 for ITS 206357
    connect(m_pEngineVideo, SIGNAL(setCamMode(bool)), this, SLOT(onSetCamMode(bool))); // added by wspark 2013.02.21 for ISV 73305

    connect(&m_pEngineVideo->getEngineMain(), SIGNAL(retranslateUi(QVariant)),
            this, SLOT(onRetranslateUI())); // added by cychoi 2013.09.03 for ITS 187585

    connect(m_pEngineVideo, SIGNAL(fgLoading(bool)), this, SLOT(onFgLoading(bool))); // added by Sergey 15.09.2014 for ITS#189390
}


AppMediaPlayer_Video_ScreenVCDPlayback::~AppMediaPlayer_Video_ScreenVCDPlayback()
{
}

// { added by kihyung 2012.07.12 for PBC Menu On/Off
void AppMediaPlayer_Video_ScreenVCDPlayback::initPBCMenu(void) // modified by shkim for 2013.08.27 ITS 186402 // modified by cychoi 2013.08.20 for ITS 185304 PBC Off on Back HK
{
    DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
    EDeckInit1Setting  tSetting;
    pDeckEngine->GetInitSetting1(tSetting);

    // removed by shkim 2013.08.27 for ITS 186402

    // Current PBC status of deck
    if(tSetting.m_nPBC) { // PBC off
        MP_LOG << "CURRENT DECK PBC STATUS is OFF" << LOG_ENDL;
        // { modified by cychoi 2013.07.03 for ITS 178068
        if(!m_isLockout)
        {
            m_pControllerModel->setIsPbcMode(false); // added by Sergey for CR#14692
        }
        // } modified by cychoi 2013.07.03
        m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                  AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                  EVENTS_SELECT_PBC, QVariant(false));
// added by ruindmby 2012.08.29 for CR 11030
        if (m_pControllerModel->vcdVersion() == VCD_VERSION_2_0)
        {
           m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                     AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                     EVENTS_PBC_MENU,
                                                                     QVariant(false));
        }
// added by ruindmby 2012.08.29 for CR 11030
    }
    else {
        MP_LOG << "CURRENT DECK PBC STATUS is ON" << LOG_ENDL;
        m_pControllerModel->setIsPbcMode(true); // added by Sergey for CR#14692
        m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                  AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                  EVENTS_SELECT_PBC, QVariant(true));
// added by ruindmby 2012.08.29 for CR 11030
        if (m_pControllerModel->vcdVersion() == VCD_VERSION_2_0)
        {
           m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                     AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                     EVENTS_PBC_MENU,
                                                                     QVariant(true));
        }
// added by ruindmby 2012.08.29 for CR 11030
    }
}
// } added by kihyung

void AppMediaPlayer_Video_ScreenVCDPlayback::onHide()
{
    MP_LOG << LOG_ENDL;

    emit releaseLongPress(); // added by junggil 2012.09.06 for keep the FF or REW mode when change to another screen. 
    // { modified by sjhyun 2013.11.05 for ITS 206357
    m_pEngineVideo->inhibitionIcon(false);
    //emit showInhibitionIcon(false); // added by kihyung 2012.12.01 for ISV 62687
    // } modified by sjhyun
    m_fullScreenTimer.stop();

    disconnect( m_pControllerModel, SIGNAL(isPbcMenuDisplayedChanged(bool)),
               this, SLOT(onIsPbcMenuDisplayedChanged(bool)) );
}


void AppMediaPlayer_Video_ScreenVCDPlayback::onShow( QVariant arg )
{
    // { modified by kihyung 2013.2.6 for ISV 70642
    MP_LOG << LOG_ENDL;

    // removed by Sergey 26.07.2013 for ITS#181096

    emit setItemProperty(QString("playbackGestureArea"), "enabled", QVariant(true));
    emit setItemProperty(QString("playbackControlls"), "isCommonJogEnabled", QVariant(true)); // added by Dmitry Bykov 08.04.13 for touch rule
	
	onLockoutMode(m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode()); //added by edo.lee 2013.03.21

    if( arg.toString() == "Options" /*&& !m_isLockout*/ && !m_pModel->loadingMode())//modified by edo.lee 2013.05.02
    {
        pushScreen(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS, QVariant("VCD"));
    }
    else if ( !arg.isNull() && arg.toBool() )
    {
        m_pControllerVCD->play();
    }
    
    //Should set full screen in a 5 seconds if video is playing.
    setFullScreen();

    // { deleted by wspark 2013.03.31 for ISV 77641
    // { added by kihyung 2012.07.12
    //initPBCMenu();
    // } added by kihyung
    // } deleted by wspark
    
    // { added by wspark 2012.11.14 for ISV 62589
    emit setItemProperty(QString("playbackControlls"), "onScreen", QVariant(true));
    emit setItemProperty(QString("progressBar"), "onScreen", QVariant(true));
    // } added by wspark
    emit setItemProperty(QString("modeArea"), "onScreen", QVariant(true)); // added by cychoi 2014.03.13 for ITS 229176

    connect(m_pControllerModel, SIGNAL(isPbcMenuDisplayedChanged(bool)), this, SLOT(onIsPbcMenuDisplayedChanged(bool)));
    // } modified by kihyung 2013.2.6 for ISV 70642

    // { added by cychoi 2013.12.04 for ITS 212759
    if((m_pEngineVideo->getFullScreenState() || 
        m_pEngineVideo->getScreenManager(m_disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_FULL ||
        m_pEngineVideo->getScreenManager(m_disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK) &&
        !m_isLockout)
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
    // } added by cychoi 2013.12.04

    setupModeArea(); // added by cychoi 2013.06.23 for DVD VCD screen management
    // removed by cychoi 2013.06.23 for DVD VCD screen management
    // } added by lssanh 2013.06.05 Lockout mode
    
//	m_pEngineVideo->getEngineMain().setPlaybackScreen(m_disp == 0); //remove edo.lee 2013.08.23
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onDeactivate()
{
    emit setItemProperty(QString("playbackGestureArea"), "enabled", QVariant(false));
    emit setItemProperty(QString("playbackControlls"), "isCommonJogEnabled", QVariant(false)); // added by Dmitry Bykov 08.04.13 for touch rule

    // commented by cychoi 2013.10.13 for ITS 195278 Should not handle DRS on Deactivate
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onMousePressed()
{
    //Reset full screen timer by any touch.
    m_fullScreenTimer.stop();
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onMouseReleased()
{
    if(!m_pController->isSeekMode()) // added by Sergey 13.09.2013 for ITS#188904
        m_fullScreenTimer.start(); // modified by ravikanth 06-07-13 for SANITY fill screen transition on drag finish
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onMouseClicked(bool pbArea)
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
        showFullScreen();
        // removed by Sergey 10.08.2013 for 182892
    }
}


void AppMediaPlayer_Video_ScreenVCDPlayback::onModeAreaTapped()
{
}


void AppMediaPlayer_Video_ScreenVCDPlayback::onButtonAreaTapped()
{
}


void AppMediaPlayer_Video_ScreenVCDPlayback::onSoftkeyBack()
{
    emit popScreen();
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onSoftkeyBtnHold( QString button )
{
    Q_UNUSED(button);
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onSoftkeyBtnHoldCritical( QString button )
{
    Q_UNUSED(button);
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onSoftkeyBtnReleased( QString button )
{
   MP_LOG << "button:" << button << LOG_ENDL;

   // { added by kihyung 2012.07.12
   if(m_pControllerModel->isPbcMode() == true) // added by Sergey for CR#14692
   {
      MP_LOG << "not support random and repeat on PBC mode" << LOG_ENDL;
      return;
   }
   else {
      MP_LOG << "Not PBC mode" << LOG_ENDL;
   }
   // } added by kihyung
   
   if ( button == "Repeat" )
   {
      m_pControllerVCD->toggleRepeat();
   }
   else if ( button == "Random" )
   {
      m_pControllerVCD->toggleRandom();
   }

   m_pEngineVideo->updatePlayingMode(DISC_VCD_VIDEO,m_pControllerVCD->getModel()->randomMode(),m_pControllerVCD->getModel()->repeatMode(),0); // modified by cychoi 2013.11.12 for OSD Title Translation
   m_fullScreenTimer.start(); // added by changjin 2012.10.05 for CR 14237   
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onPlaybackStatusChanged( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T status)
{
    // { add by kihyung 2012.07.05 for CR 9855
    // Q_UNUSED(status);
    switch(status) 
    {
    case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING:
    // case AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_STOPPED: // removed by Sergey 03.05.2013 for ISV_KR#76907
        emit resetSpeedIcon();
        m_pController->PrevNextOSD(false); // added by yongkyun.lee 20130320 for : NO CR ff/rew OSD
        // { added by shkim 2013.08.27 for ITS 186402
        if(m_pControllerVCD->isMediaSearchState() == false)
        {
            DeckEngineerMode  *pDeckEngine = m_pEngineVideo->getEngineMain().GetEngModeNotifier()->GetDeckEngineerMode();
            EDeckInit1Setting  tSetting;
            pDeckEngine->GetInitSetting1(tSetting);
            if(tSetting.m_nPBC == 0) { // Enable "PBC Menu" option when PBC is on in Deck
                m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                          AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                          EVENTS_SELECT_PBC,
                                                                          QVariant(true));

                if(m_pControllerModel->vcdVersion() == VCD_VERSION_2_0) {
                    m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                              AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                              EVENTS_PBC_MENU,
                                                                              QVariant(true));
                }
            }
            else { // Disable "PBC Menu" option when PBC is on in Deck
                m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                          AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                          EVENTS_SELECT_PBC,
                                                                          QVariant(false));

                if(m_pControllerModel->vcdVersion() == VCD_VERSION_2_0) {
                    m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                              AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                              EVENTS_PBC_MENU,
                                                                              QVariant(false));
                }
            }
        }
        // } added by shkim 2013.08.27
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

void AppMediaPlayer_Video_ScreenVCDPlayback::onOptionsItemClicked(int item)
{
   MP_LOG << "item = " << item << LOG_ENDL;
	if(/*m_isLockout ||*/ m_pModel->loadingMode()) return;//modified by edo.lee 2013.05.02
   switch(item)
   {
   // { commented by cychoi 2013.07.13 for List View spec out
   //case OPTIONS_LIST:
   //{
   //   MP_LOG <<"Search_VCD pressed" << LOG_ENDL;
   //   listButtonHandler();
   //   break;
   //}
   // } commented by cychoi 2013.07.13
   case OPTIONS_PBC_MENU:
   {
      MP_LOG <<"PBC menu pressed" << LOG_ENDL;
      // { modified by shkim 2013.08.27 for ITS 186402
      if (!m_pControllerModel->isPbcMenuDisplayed())
      {
         m_pControllerVCD->pbcReturnKey(); // modified by cychoi 2013.11.08 for ITS 207747
         //emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU); // commented by cychoi 2013.08.30 for ITS 184879
         //m_pEngineVideo->LaunchSubScreenOnVideo(VIDEO_VCD_PBC, m_disp);
      }
      // } modified by shkim 2013.08.27
      else
      {
         MP_LOG << "call pbcReturnKey" << LOG_ENDL;
         m_pControllerVCD->pbcReturnKey();
      }

      break;
   }
   case OPTIONS_DIRECT_ACCESS:
   {
      MP_LOG <<"Direct Access pressed" << LOG_ENDL;
      emit showPlaybackControls(false); // added by kihyung 2012.08.27 for Video(LGE) #12
      emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS);
      break;
   }

    case OPTIONS_SOUND_SET:
    {
       // { added by shkim 2013.08.27 for ITS 186402
       if (m_pControllerModel->isPbcMenuDisplayed())
       {
           m_pControllerVCD->setPBC(false);
       }
       // } added by shkim 2013.08.27

       m_pEngineVideo->launchSettings(AppSettingsDef::eStart_Mode_Sound, MP::DISC, false,m_disp); // modified by edo.lee 2013.05.02
       break;
    }

    case OPTIONS_DISPLAY_SET:
    {
        MP_LOG << "should launch VideoSettings" << LOG_ENDL;

        m_pEngineVideo->launchSettings(AppSettingsDef::eStart_Mode_Screen, MP::DISC, false,m_disp); // modified by edo.lee 2013.05.02
        break;
    }

    case OPTIONS_REPEAT_ONE:
    {
        m_pControllerVCD->repeatPlayFile();
        break;
    }
	
	// { modified by ravikanth - 12-09-19
    case OPTIONS_REPEAT_ALL:
    {
        m_pControllerVCD->repeatPlayAll();
        break;
    }
	// } modified by ravikanth - 12-09-19
	
    case OPTIONS_REPEAT_OFF:
    {
        m_pControllerVCD->repeatPlayOff();
        break;
    }
	
	// { added by ravikanth - 12-09-19
    case OPTIONS_REPEAT_FOLDER:
    {
        m_pControllerVCD->repeatPlayFolder();
        break;
    }
	// } added by ravikanth - 12-09-19
	
    case OPTIONS_RANDOM_ON:
    {
        m_pControllerVCD->randomPlayFile();
        break;
    }

    case OPTIONS_RANDOM_OFF:
    {
        m_pControllerVCD->randomPlayOff();
        break;
    }

  case OPTIONS_FULL_SCREEN:
  {
     showFullScreen();
     break;
  }

   default: //make compiler happy
      break;
   }
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onOptionsCheckBoxSelect ( int item, bool flag )
{
   MP_LOG << "item = " << item << " flag = " << flag << LOG_ENDL;

   switch(item)
   {
   case OPTIONS_PBC_ONOFF:
   {
       MP_LOG <<"PBC on/off pressed" << LOG_ENDL;
       // { added by cychoi 2014.02.20 for (HMC Request)
       if(m_isLockout)
       {
           m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                     AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                     EVENTS_UNDO_SELECT_PBC,
                                                                     QVariant(!flag));
           // show DRS popup, just return
           // { modified by cychoi 2014.03.13 for ITS 229323, 229324, 229325, 229327
           m_pEngineVideo->showTxtBtnPopup(POPUP_TXT_BTN_DRIVING_REGULATION, (VIDEO_DISPLAY_T)m_disp);
           //showTxtBtnPopUp(POPUP_TXT_BTN_DRIVING_REGULATION);
           // } modified by cychoi 2014.03.13
           break;
       }
       // } added by cychoi 2014.02.20
       // removed by shkim 2013.08.27 for ITS 186402
       m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                 AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                 EVENTS_SELECT_PBC,
                                                                 QVariant(flag));
       // { added by wspark 2014.02.14 for ITS 225340
       m_pEngineVideo->getScreenManager(m_disp == VPEnum::VDISP_FRONT ? VPEnum::VDISP_REAR : VPEnum::VDISP_FRONT)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                 AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS,
                                                                 EVENTS_SELECT_PBC,
                                                                 QVariant(flag));
       // } added by wspark
       
       // { modified by shkim 2013.08.27 for ITS 186402
       if(flag == false) { // Disable "PBC Menu" option if PBC is off
       //if((m_pControllerModel->vcdVersion() == VCD_VERSION_2_0 && flag == true) || flag == false) {
       // } modified by shkim 2013.08.27
           m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                                                                     AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                                                                     EVENTS_PBC_MENU,
                                                                     QVariant(flag));
       }
       showFullScreen();
       m_pControllerVCD->setPBC(flag);
       // } modified by kihyung
       m_pControllerModel->setLoadingMode(true); // modified by cychoi 2013.08.28 // added by cychoi 2013.08.27 for ITS 184304 Loading screen close
       // { added by cychoi 2013.11.09 for ITS 207971 It is temporary solution. We should fix it correctly later
       int vcdOpposite = m_disp == 0 ? 1 : 0;
       if(m_pEngineVideo->getIsShowLockoutView() == vcdOpposite &&
          m_pEngineVideo->getScreenManager(vcdOpposite)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
       {
           m_pEngineVideo->getScreenManager(vcdOpposite)->popScreen();
       }
       // } added by cychoi 2013.11.09
       break;
   }

   default: //make compiler happy
      break;
   }
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onMenuKeyPressed()
{
   MP_LOG << LOG_ENDL;
	if(/*m_isLockout || */ m_pModel->loadingMode()) return; //modified by edo.lee 2013.05.02
   pushScreen(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS, QVariant("VCD"));
}

void  AppMediaPlayer_Video_ScreenVCDPlayback::onEventReceived(const QString &srcScreenName,
                                                              const QString &resScreenName,
                                                              const int eventId,
                                                              const QVariant arg)
{
   MP_LOG << " srcScreenName = " << srcScreenName << " resScreenName = " << resScreenName << \
             " eventId = " << eventId << " arg = " << arg << LOG_ENDL;

   if (resScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK)
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

void AppMediaPlayer_Video_ScreenVCDPlayback::onTogglePlayBtn()
{	
    //{ added by yongkyun.lee 20130314 for : Video Play/Pause    
    if(m_pEngineVideo->IsPlaying())
        m_pEngineVideo->setPassPlay(true);

    // { modified by kihyung 2013.07.30 for ITS 0174622
    if(m_pControllerVCD->getModel()->playbackStatus() != AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING) {
        //{ modified by cychoi 20103.08.13 for ITS 183912 Sound mute on toggle play
        //added by edo.lee 2013.04.20
        //m_pEngineVideo->getEngineMain().setUnMuteState(false);
        m_pEngineVideo->getEngineMain().setMuteForSeek(false);
        //} modified by cychoi 20103.08.13
    }
    // } modified by kihyung 2013.07.30 for ITS 0174622  
    
    //} added by yongkyun.lee 20130314 
    m_pControllerVCD->togglePlay();
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onLongPressPlayBtn()
{
//    showFullScreen(); // commented by Sergey 24.07.2013 ITS#181467
}

void AppMediaPlayer_Video_ScreenVCDPlayback::listButtonHandler()
{
    // { commented by cychoi 2013.07.13 for List View spec out
    //MP_LOG << LOG_ENDL;
    //if(m_isLockout) return; // added by lssanh 2013.06.05 Lockout mode

    //if( m_pControllerModel->isPbcMode() == false ) // added by Sergey for CR#14692
    //{
    //    emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_VCD_CHAPTERS);
    //}
    //m_pEngineVideo->setVideoListView((VIDEO_DISPLAY_T)m_disp, true);// modified by edo.lee 2013.05.29
    // } commented by cychoi 2013.07.13
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onLockoutMode(bool on)
{
   MP_LOG << "on =" << on << LOG_ENDL;

   //m_pModel->setLockoutMode(on);
   m_isLockout = on;
   // { commented by cychoi 2013.07.13 for List View spec out
   // { added by lssanh 2013.06.05 Lockout mode
   //notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
   // { modified by cychoi 2013.06.06 for PBC on 
   // 				EVENTS_SET_LIST_BTN_VISIBLE, QVariant(!m_isLockout && !m_pControllerModel->isPbcMode()));	   
   //				EVENTS_SET_LIST_BTN_VISIBLE, QVariant(!m_isLockout));	   
   // } modified by cychoi 2013.06.06
   // } added by lssanh 2013.06.05 Lockout mode
   // } commented by cychoi 2013.07.13

   //emit showLockout(on); //deleted by yungi 2013.11.03 for ITS 198971
   invokeQmlMethod("onShowLockout", QVariant(on)); //added by yungi 2013.11.03 for ITS 198971
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onIsPbcMenuDisplayedChanged(bool value)
{
   MP_LOG << " value = " << value << LOG_ENDL;

   // { commented by cychoi 2013.07.13 for List View spec out
   // { modified by cychoi 2013.06.06 for PBC on 
   //notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
   //                      AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
   //                      EVENTS_SET_LIST_BTN_VISIBLE, QVariant(!m_isLockout && !value));
   // } modified by cychoi 2013.06.06
   // } commented by cychoi 2013.07.13
// modified by ruindmby 2012.08.29 for CR 11030
   if ( value == true )
   {
      // { modified by cychoi 2014.03.13 for ITS 229176
      if(m_isLockout == false)
      {
         m_fullScreenTimer.stop();// added by eunhye 2013.05.09 ITS 163782 
      }
      // } modified by cychoi 2014.03.13
      m_pEngineVideo->setStatusBarOnScreen( true,(m_disp== VIDEO_DISPLAY_FRONT)?true:false); // added by wspark 2012.11.14 for ISV 62589
      m_pEngineVideo->popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS); // added by cychoi 2013.12.30 for ITS 217646
      emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU);
   }
// modified by ruindmby 2012.08.29 for CR 11030
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onFocusVisibilityChanged( bool value )
{
   m_pModel->setFocusVisible( value );
}

// removed by Sergey 24.09.2013 for ITS#188938

void AppMediaPlayer_Video_ScreenVCDPlayback::onPrevBtn()
{
    //m_pEngineVideo->getEngineMain().setUnMuteState(false); // commented by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute //added by edo.lee 2013.04.20
    m_pControllerVCD->prev();
    m_pEngineVideo->DisplayOSD(true); //added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD  
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onNextBtn()
{
    //m_pEngineVideo->getEngineMain().setUnMuteState(false); // commented by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute //added by edo.lee 2013.04.20
    m_pControllerVCD->next();
    m_pEngineVideo->DisplayOSD(true); //added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD  
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onNextBtnHold()
{
    m_pControllerVCD->fastForward();
    // emit setSpeedIcon(FAST_SPEED_X4); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onNextBtnHoldCritical()
{
    m_pControllerVCD->fastForwardCritical();
    // emit setSpeedIcon(FAST_SPEED_X16); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onPrevBtnHold()
{
    m_pControllerVCD->rewind();
    // emit setSpeedIcon(FAST_SPEED_X4); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onPrevBtnHoldCritical()
{
    m_pControllerVCD->rewindCritical();
    // emit setSpeedIcon(FAST_SPEED_X16); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onFlick_next()
{
    m_pControllerVCD->flickNext();
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onFlick_prev()
{
    m_pControllerVCD->flickPrev();
}

// { added by cychoi 2013.07.07 for Smoke Test - Cue is not responsed on tune mode
void AppMediaPlayer_Video_ScreenVCDPlayback::onSelectTuned()
{ 
    m_pControllerVCD->selectTuned();	
    onSetTuneMode(false);	
    m_pControllerVCD->cancelTune();
}
// } added by cychoi 2013.07.07 


// { modified by Sergey 10.08.2013 for 182892
void AppMediaPlayer_Video_ScreenVCDPlayback::showFullScreen()
{
    pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FULL);
}

void AppMediaPlayer_Video_ScreenVCDPlayback::setFullScreen()
{
    //if(this != dynamic_cast<AppMediaPlayer_Video_ScreenVCDPlayback*>(m_pEngineVideo->getScreenManager(m_disp)->topScreen()))
    if(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK != m_pEngineVideo->getScreenManager(m_disp)->topScreenName())
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


// { added by cychoi 2013.06.23 for DVD VCD screen management
void AppMediaPlayer_Video_ScreenVCDPlayback::setupModeArea()
{
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_TEXT, QVariant(QT_TR_NOOP("STR_MEDIA_MNG_VCD"))); // modified by Sergey 25.08.2013 for ITS#186435
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                        AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_COUNTER_VISIBLE,
                        QVariant(false)); // added by Sergey 08.05.2013
    // { added by lssanh 2013.06.05 Lockout mode
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                        AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        // { modified by cychoi 2013.07.13 for List View spec out
                        // { modified by cychoi 2013.06.06 for PBC on 
                        EVENTS_SET_LIST_BTN_VISIBLE, QVariant(false));
                        //EVENTS_SET_LIST_BTN_VISIBLE, QVariant(!m_isLockout && !m_pControllerModel->isPbcMode()));
                        //EVENTS_SET_LIST_BTN_VISIBLE, QVariant(!m_isLockout));
                        // } modified by cychoi 2013.06.06
                        // } modified by cychoi 2013.07.13
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                        AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_MENU_BTN_VISIBLE, QVariant(true));
    //{ added by cychoi 2013.08.14 for ITS 184421
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                        AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_RTEXT_VISIBLE, QVariant(false));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                        AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_ICON_VISIBLE, QVariant(false));
    //} added by cychoi 2013.08.14
}
// } added by cychoi 2013.06.23

void AppMediaPlayer_Video_ScreenVCDPlayback::onCancel_ff_rew()
{
    MP_LOG << LOG_ENDL;
	m_pEngineVideo->getEngineMain().setUnMuteState(false);//added by edo.lee 2013.04.20
	m_pControllerVCD->cancel_ff_rew();	
	//if(m_pEngineVideo->getEngineMain().getisBTCall()) // deleted by lssanh 2013.06.26 bt call // modifieded by cychoi 2013.06.23 //added by edo.lee 2013.06.19
	//	 m_pControllerVCD->pause();
    // emit resetSpeedIcon(); // add by kihyung 2012.06.20 for CR 9855
}

void AppMediaPlayer_Video_ScreenVCDPlayback::onSetTuneMode(bool on)
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

// { added by yongkyun.lee  2012.10.17  for Current file is white color
void AppMediaPlayer_Video_ScreenVCDPlayback::onTuneSameFile(bool on)
{
    m_pModel->setTuneSameFile(on);
}
// } added by  yongkyun.lee 

// removed by Sergey 01.05.2013 for Tune Functionality

// { added by wspark 2013.02.21 for ISV 73305
void AppMediaPlayer_Video_ScreenVCDPlayback::onSetCamMode(bool on)
{
    m_pModel->setCamMode(on);
}
// } added by wspark

// { added by cychoi 2013.09.03 for ITS 187585
void AppMediaPlayer_Video_ScreenVCDPlayback::onRetranslateUI()
{
    // { modified by cychoi 2013.12.05 for ITS 212884
    if(m_pEngineVideo->GetVideoController()->getSource() != MEDIA_SOURCE_VCD ||
       m_pEngineVideo->getEngineMain().GetLastPlayerMode() != APP_MEDIA_MODE_VIDEO)
    {
        MP_LOG<<"mode is not matched" << LOG_ENDL;
    }
    else
    {
        m_pModel->retranslateUI(VIDEO_SOURCE_VCD);
    }

    m_pControllerVCD->retranslateUI(); // added by cychoi 2013.10.27 for ITS 197433
    // } modified by cychoi 2013.12.05
}
// } added by cychoi 2013.09.03


// { added by Sergey 15.09.2014 for ITS#189390
void AppMediaPlayer_Video_ScreenVCDPlayback::onFgLoading(bool on)
{
    m_pModel->setFgLoadingMode(on);
}
// } added by Sergey 15.09.2014 for ITS#189390
