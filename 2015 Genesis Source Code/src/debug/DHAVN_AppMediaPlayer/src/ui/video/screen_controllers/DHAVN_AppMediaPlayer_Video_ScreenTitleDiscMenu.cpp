#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerDVD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_DVD.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenTitleDiscMenu.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Playback.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"     // added by kihyung 2013.1.28 for ISV 71502
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"

AppMediaPlayer_Video_ScreenTitleDiscMenu::
AppMediaPlayer_Video_ScreenTitleDiscMenu( AppMediaPlayer_Video_Controller *pController,
                                          AppMediaPlayer_EngineVideo * pEngineVideo,
                                          int disp,
                                          QObject *parent )
    : AppMediaPlayer_Video_Screen( parent ),
      m_pController( pController ),
      m_pEngineVideo(pEngineVideo),
      m_isLockout(false),
      m_disp(disp)
{
    m_myName = AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU; // added by sjhyun 2013.11.08 for ITS 207579
    m_pControllerDVD = static_cast<AppMediaPlayer_Video_ControllerDVD*>(m_pController->getController(MEDIA_SOURCE_DVDVIDEO));
    m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(m_pControllerDVD->getModel());
    m_overlays << AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA;

    connect(m_pControllerModel, SIGNAL(loadingModeChanged(bool)), this, SIGNAL(loadingModeChanged(bool))); // modified by cychoi 2013.08.30 for ITS 187026 DVD Title/Disc Menu Loading Screen // added by lssanh 2013.05.18 ISV81619
    connect(m_pEngineVideo, SIGNAL(fgLoading(bool)), this, SIGNAL(fgLoadingModeChanged(bool))); // modified by Sergey 15.09.2014 for ITS#189390
    connect( m_pControllerDVD, SIGNAL(screenStateChanged(int)), this, SLOT(onScreenStateChanged(int)) ); // added by lssanh 2013.05.18 ISV81619
    connect(&m_pEngineVideo->getEngineMain(), SIGNAL(retranslateUi(QVariant)), this, SLOT(onRetranslateUI())); // added by yungi 2013.11.29 for NoCr MiddleEast Changed Not Move Screen TitleDiscMenu
}


AppMediaPlayer_Video_ScreenTitleDiscMenu::~AppMediaPlayer_Video_ScreenTitleDiscMenu()
{
}
//added by edo.lee 2013.02.27
void AppMediaPlayer_Video_ScreenTitleDiscMenu::onLockoutMode(bool on)
{
   MP_LOG << "on =" << on << LOG_ENDL;
   m_isLockout = on;
   emit showLockout(on);

   // { added by sjhyun 2013.11.08 for ITS 207579
   if(on)
       m_pEngineVideo->getScreenManager(m_disp)->setupFocusSequence(FOCUS_MODE_MODEAREA, m_myName);
   else
       m_pEngineVideo->getScreenManager(m_disp)->setupFocusSequence(FOCUS_MODE_ALL, m_myName);
   // } added by sjhyun
}


void AppMediaPlayer_Video_ScreenTitleDiscMenu::onHide()
{

    //disconnect( m_pControllerDVD, SIGNAL(screenStateChanged(int)), // deleted by lssanh 2013.05.18 ISV81619
    //            this, SLOT(onScreenStateChanged(int)) ); // deleted by lssanh 2013.05.18 ISV81619
    // { modified by cychoi 2013.06.26 for ITS 176110 Lockout mode
    if(!m_isLockout)
    {
        m_pEngineVideo->setFullScreenState(true); // added for eunhye 2013.03.19
    }
    // } modified by cychoi 2013.06.26
    emit hideTitleDiscMenu(false); //added by eunhye 2013.03.22
    // { modified by sjhyun 2013.11.05 for ITS 206357
    //emit showInhibitionIcon(false);
    m_pEngineVideo->inhibitionIcon(false);
    // } modified by sjhyun
}


void AppMediaPlayer_Video_ScreenTitleDiscMenu::onShow( QVariant arg )
{
    Q_UNUSED(arg);
    //connect( m_pControllerDVD, SIGNAL(screenStateChanged(int)), // deleted by lssanh 2013.05.18 ISV81619
    //         this, SLOT(onScreenStateChanged(int)) ); // deleted by lssanh 2013.05.18 ISV81619
    setupModeArea();

	//modified by edo.lee 2013.03.18
	//m_isLockout = m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode();
	onLockoutMode(m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode());
	//modified by edo.lee 2013.03.18
    //{ added for eunhye 2013.03.19
    // { modified by lssanh 2013.05.17 ISV81959
    // { modified by cychoi 2013.06.26 for ITS 176110 Lockout mode
    if((m_pEngineVideo->getFullScreenState()||
       m_pEngineVideo->getScreenManager(m_disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU) &&
        !m_isLockout)
    // } modified by cychoi 2013.06.26
    // } modified by lssanh 2013.05.17 ISV81959
    {
        // { deleted by lssanh 2013.04.07 rollback NoCR hide previous screen
        /*
        if (m_pEngineVideo->getScreenManager(m_disp)->getScreensCount() < 2)
            emit hideDelayScreenBG(true); // from home
        else
            emit hideDelayScreenBG(false); // from playback
        */
        // } deleted by lssanh 2013.04.07 rollback NoCR hide previous screen

        // { modified by Sergey 11.05.2013
        if(m_pEngineVideo->getEngineMain().middleEast())
            m_pController->SetVideoSizeWrtDisplay( 401 , 210 , 780, 435 );
        else
            m_pController->SetVideoSizeWrtDisplay( 94 , 210 , 780, 435 );
        // } modified by Sergey 11.05.2013


        m_pEngineVideo->setFullScreenState(false);
    }
    //} added for eunhye 2013.03.19
}

// commented by cychoi 2013.10.13 for ITS 195278 Should not handle DRS on Deactivate

void AppMediaPlayer_Video_ScreenTitleDiscMenu::onMenuKeyPressed()
{
    MP_LOG << LOG_ENDL;
    // { commented by cychoi 2014.03.04 for ITS 228080 (HMC Request)
    //if(/*m_isLockout ||*/ m_pControllerDVD->getModel()->loadingMode()) return; // modified by yungi.lee 2013.10.09 for ITS 194557 // remove by edo.lee 2013.05.02
    //pushScreen(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS, QVariant("DVD_TITLE_MENU"));
    // } commented by cychoi 2014.03.04
}

void AppMediaPlayer_Video_ScreenTitleDiscMenu::onOptionsItemClicked( int item )
{
    MP_LOG << " item " << item << LOG_ENDL;
	//if(m_isLockout) return; remove by edo.lee 2013.05.02
    switch(item)
    {
    case OPTIONS_LIST:
    {
        MP_LOG <<"Search_DVD pressed " << LOG_ENDL;
        listButtonHandler();
        break;
    }

    case OPTIONS_SOUND_SET:
    {
        if(m_pEngineVideo)
        {
            m_pEngineVideo->launchSettings(AppSettingsDef::eStart_Mode_Sound, MP::DISC, false,m_disp); // modified by edo.lee 2013.05.02
        }
        break;
    }

    case OPTIONS_DISPLAY_SET:
    {
        MP_LOG << "should launch VideoSettings" << LOG_ENDL;
        if(m_pEngineVideo)
        {
            m_pEngineVideo->launchSettings(AppSettingsDef::eStart_Mode_Screen, MP::DISC, false,m_disp); // modified by edo.lee 2013.05.02
        }
        break;
    }

    case OPTIONS_SETTINGS:
    {
        MP_LOG << "Settings pressed" << LOG_ENDL;
        if(m_pEngineVideo)
        {
            m_pControllerDVD->InitDeckLang();  // added by cychoi 2013.09.08 for ITS 181382 EVT_INITIALIZATION_REQUEST
            // { modified by cychoi 2013.08.20 for ITS 178895 SerachCaption subscreen
        	m_pEngineVideo->onShowSettings("DVD_Settings", QVariant("DISC"));
 			m_pEngineVideo->LaunchSubScreenOnVideo(VIDEO_DVD_SETTING, m_disp);
            // } modified by cychoi 2013.08.20
			// modified by ravikanth 13-03-13
            //emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS, QVariant("DISC"));
            //m_pEngineVideo->launchSettings(AppSettingsDef::eStart_Mode_General_Video_Control, MP::DISC, true);
        }
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
        pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FULL);
        break;
    }
    // { added by cychoi 2013.10.05 for ITS 193836 Active Cation, Voice, Angle
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
    // } added by cychoi 2013.10.05
    default: //make compiler happy
        break;
    }
}

// { added by cychoi 2013.09.13 for ITS 190207 Infinite DVD Loading Screen
void AppMediaPlayer_Video_ScreenTitleDiscMenu::onEventReceived( const QString &srcScreenName,
                                                              const QString &resScreenName,
                                                              const int eventId,
                                                              const QVariant arg )
{
   MP_LOG << " srcScreenName = " << srcScreenName << " resScreenName = " << resScreenName << \
             " eventId = " << eventId << " arg = " << arg << LOG_ENDL;

   Q_UNUSED(srcScreenName);
   Q_UNUSED(arg);

   if ( resScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU )
   {
      switch( eventId )
      {
          case EVENTS_SHOW_PLAYBACK_ONLY:
             hide();
             break;
          case EVENTS_SET_FG_LOADING_MODE:
          {
              emit fgLoadingModeChanged(arg.toBool());
              // { added by sjhyun 2013.10.09 for ITS 194622
              // TODO
              // When loading popup is appeared, visible flag of the titleDiscQML's mainFrame is set as false not true.
              // We need root solution for this later.
              if(arg.toBool() == true)
              {
                  emit hideTitleDiscMenu(true);
              }
              // } added by sjhyun 2013.10.09
              emit titleDiscLoadingDisplayCheck(m_disp); // added by yungi 2013.10.10 for ITS 194869		
              break;
          } // added by Sergey 07.09.2013 for PRE_FG supoort
          // { added by cychoi 2014.03.12 for ITS 229158
          case EVENTS_DRS_MODE:
          {
              onLockoutMode(arg.toBool());
              break;
          }
          // } added by cychoi 2014.03.12
          default:
             break; // make compiler happy
      }
   }
}
// } added by cychoi 2013.09.13

void AppMediaPlayer_Video_ScreenTitleDiscMenu::listButtonHandler()
{
   // modified by minho 20120828
   // { for able list menu on DVD
   //if(m_pControllerModel->chapterCount() > 0)
   if(m_pControllerDVD->getChapterCount() > 0)
   // } modified by minho
   {
      onHide();// added by eunhye 2013.03.22
      m_pControllerDVD->storeDiscState();
      emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLES);
   }
}


void AppMediaPlayer_Video_ScreenTitleDiscMenu::cursorKey( int direction )
{
      m_pControllerDVD->cursorKey( direction );
}

void AppMediaPlayer_Video_ScreenTitleDiscMenu::titleMenu()
{
      m_pControllerDVD->titleMenu();
}

void AppMediaPlayer_Video_ScreenTitleDiscMenu::select()
{
    // moved by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
    m_pControllerDVD->select();
}

void AppMediaPlayer_Video_ScreenTitleDiscMenu::onScreenStateChanged( int nState )
{
    MP_LOG << "nState = " << nState << LOG_ENDL;

    // removed by cychoi 2013.08.05 for unnecessary code. Popup was already closed in DVD Playback. added by lssanh 2013.06.04 ISV81959

    // removed by cychoi 2013.08.30 for ITS 187026 DVD Title/Disc Menu Loading Screen
    // Screen was already changed in DVD Playback. Do nothing here.
}

//{ added by cychoi 2013.08.14 for ITS 184421
void AppMediaPlayer_Video_ScreenTitleDiscMenu::setupModeArea()
{
    notifyScreenManager( AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU,
                         AppMediaPlayer_Video_Screens::SCREEN_DVD_PLAYBACK,
                         EVENTS_SHOW_PLAYBACK_ONLY );
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU,
                        AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_COUNTER_VISIBLE,
                        QVariant(false)); // added by Sergey 08.05.2013
    // { modified by lssanh 2013.06.05 Lockout mode 
    // { added by cychoi 2013.06.23 for DVD VCD screen management
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU,
                        AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_TEXT, QVariant(QT_TR_NOOP("STR_MEDIA_MNG_DVD"))); // modified by Sergey 25.08.2013 for ITS#186435
    // { added by cychoi 2013.06.23
    //m_pControllerModel->setListBtnVisible(false); // added by wspark 2013.03.29 for ISV 77716
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU,
                        AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_LIST_BTN_VISIBLE,
                        QVariant(false));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU,
                        AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_MENU_BTN_VISIBLE,
                        QVariant(false)); // modified by cychoi 2014.02.19 for (HMC Request)
    // } modified by lssanh 2013.06.05 Lockout mode
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU,
                        AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_RTEXT_VISIBLE, QVariant(false));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU,
                        AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_ICON_VISIBLE, QVariant(false));
}
//} added by cychoi 2013.08.14

// added by yungi 2013.11.29 for NoCr MiddleEast Changed Not Move Screen TitleDiscMenu
void AppMediaPlayer_Video_ScreenTitleDiscMenu::onRetranslateUI()
{
    // { added by cychoi 2014.02.10 for ITS 224569
    if(m_pEngineVideo->GetVideoController()->getSource() != MEDIA_SOURCE_DVDVIDEO ||
       m_pEngineVideo->getEngineMain().GetLastPlayerMode() != APP_MEDIA_MODE_VIDEO)
    {
        MP_LOG<<"mode is not matched" << LOG_ENDL;
    }
    else
    // } added by cychoi 2014.02.10
    {
        if (!m_isLockout && m_pEngineVideo->getScreenManager(m_disp)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_DVD_TITLE_DISC_MENU )
        {
            if(m_pEngineVideo->getEngineMain().middleEast())
                m_pController->SetVideoSizeWrtDisplay( 401 , 210 , 780, 435 );
            else
                m_pController->SetVideoSizeWrtDisplay( 94 , 210 , 780, 435 );
        }
    }
}
// } added by yungi

