#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenAUXPlayback.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h" // added by kihyung 2012.07.31 for CR 12259

AppMediaPlayer_Video_ScreenAUXPlayback::
AppMediaPlayer_Video_ScreenAUXPlayback( AppMediaPlayer_Video_Controller *pController,
                                        AppMediaPlayer_EngineVideo *pEngineVideo,
                                        bool isAux1,
                                        int disp,
                                        QObject *parent )
    : AppMediaPlayer_Video_Screen( parent ),
      m_pController( pController ),
      m_pEngineVideo( pEngineVideo ),
      m_bIsAux1( isAux1 ),
      m_isLockout(false),
      m_disp(disp)
{
    m_pModel = new AppMediaPlayer_Video_SModel_Playback(this);
    setModel( m_pModel );

    m_pModel->setProgressBarMode("AUX");

    m_overlays << AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA;

    m_fullScreenTimer.setInterval( FULL_SCREEN_INTERVAL );

    // { modified by kihyung 2012.07.31 for CR 12259
    /*
    connect(&m_fullScreenTimer, SIGNAL(timeout()), this, SLOT(setFullScreen()));
    */
    connect(&m_fullScreenTimer, SIGNAL(timeout()), this, SLOT(showFullScreen()));
    // } modified by kihyung
    
    //connect(m_pEngineVideo, SIGNAL(lockoutMode(bool)), this, SLOT(onLockoutMode(bool)));

    // removed by Sergey 24.09.2013 for ITS#188938

    //{ added by radhakrushna 2013.03.11 for ISV 74997
    bool ret = connect(pController , SIGNAL(playAuxOnFG()) ,this , SLOT(onPlayAuxOnFG()));
    //} added by radhakrushna

    connect(m_pEngineVideo, SIGNAL(fgLoading(bool)), this, SLOT(onFgLoading(bool))); // added by Sergey 15.09.2014 for ITS#189390
}


AppMediaPlayer_Video_ScreenAUXPlayback::~AppMediaPlayer_Video_ScreenAUXPlayback()
{
}


void AppMediaPlayer_Video_ScreenAUXPlayback::onHide()
{
    MP_LOG << LOG_ENDL; // added by kihyung 2012.07.31 for CR 12259
    m_fullScreenTimer.stop();
}


void AppMediaPlayer_Video_ScreenAUXPlayback::onShow(QVariant arg)
{
    // Q_UNUSED(arg); // commentted by kihyung 2012.07.31 for CR 12259
    MP_LOG << LOG_ENDL;

     // removed by Sergey 26.07.2013 for ITS#181096

    //{ added by hyochang.ryu 20130810 for ITS183813
    setupModeArea();
    emit lostFocus(AppEngineQMLConstants::JOG_UP);
    //} added by hyochang.ryu 20130810
    emit setItemProperty(QString("playbackGestureArea"), "enabled", QVariant(true)); // added by lssanh 2013.05.24 ITS164925
    onLockoutMode(m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode()); // added by cychoi 2014.07.28 for ITS 244176
    setFullScreen();
    // } modified by kihyung

    if( arg.toString() == "Options" /*&& !m_isLockout*/ && ! m_pModel->loadingMode())//modified by edo.lee 2013.05.02
    {
        pushScreen(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS, QVariant("AUX"));
    }
	//m_pEngineVideo->getEngineMain().setPlaybackScreen(m_disp == 0); //added by edo.lee 2013.08.23

    // { added by cychoi 2013.12.04 for ITS 212759
    if((m_pEngineVideo->getFullScreenState() || 
        m_pEngineVideo->getScreenManager(m_disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_FULL ||
        m_pEngineVideo->getScreenManager(m_disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_AUX1_PLAYBACK ||
        m_pEngineVideo->getScreenManager(m_disp)->topScreenName()== AppMediaPlayer_Video_Screens::SCREEN_AUX2_PLAYBACK) &&
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
}

// { added by lssanh 2013.05.24 ITS164925
void AppMediaPlayer_Video_ScreenAUXPlayback::onDeactivate()
{
    emit setItemProperty(QString("playbackGestureArea"), "enabled", QVariant(false));
}
// } added by lssanh 2013.05.24 ITS164925

void AppMediaPlayer_Video_ScreenAUXPlayback::onMenuKeyPressed()
{
	if(/*m_isLockout ||*/ m_pModel->loadingMode()) return;//modified by edo.lee 2013.05.02
    pushScreen(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS, QVariant("AUX"));
}

void AppMediaPlayer_Video_ScreenAUXPlayback::onMousePressed()
{
    //Reset full screen timer by any touch.
    m_fullScreenTimer.stop();
}

void AppMediaPlayer_Video_ScreenAUXPlayback::onMouseReleased()
{
    // { modified by cychoi 2014.03.17 for HW Event DV2-1 1403-00059
    if(!m_pController->isSeekMode())
        m_fullScreenTimer.start();
    //Reset full screen timer by any touch.
    //m_fullScreenTimer.stop(); // added by Sergey 04.08.2013 for ITS#179312
    // } modified by cychoi 2014.03.17
}

void AppMediaPlayer_Video_ScreenAUXPlayback::onMouseClicked(bool pbArea)
{
    MP_LOG << LOG_ENDL;
    //{ added by yongkyun.lee 20130612 for : ITS 172821
    if( m_pController->isSeekMode())
       return;
    //} added by yongkyun.lee 20130612 
    
    // { modified by kihyung 2012.07.31 for CR 12259    
    /*
    m_fullScreenTimer.start();

    if(pbArea)
        setFullScreen();
    */

    if(m_pModel->focusVisible())
    {
        emit hideAllFocus();
        //return; // modified by Sergey for #78301
    }

    if(pbArea)
    {
		showFullScreen();
		// removed by Sergey 10.08.2013 for 182892
    }
    // } modified by kihyung    
}

void AppMediaPlayer_Video_ScreenAUXPlayback::onOptionsItemClicked(int item)
{
    MP_LOG << "item = " << item << LOG_ENDL;
	//if(m_isLockout) return; //remove by edo.lee 2013.05.02
    switch(item)
    {
    case OPTIONS_SOUND_SET:
    {
        m_pEngineVideo->launchSettings(AppSettingsDef::eStart_Mode_Sound, MP::AUX, false, m_disp); // modified by edo.lee 2013.05.02
        break;
    }

    case OPTIONS_DISPLAY_SET:
    {
        MP_LOG << "should launch VideoSettings" << LOG_ENDL;

        m_pEngineVideo->launchSettings(AppSettingsDef::eStart_Mode_Screen, MP::AUX,false,m_disp); // modified by edo.lee 2013.05.02
        break;
    }

    case OPTIONS_FULL_SCREEN:
    {
        // { modified by kihyung 2012.07.31 for CR 12259
        /*
        pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FULL);
        */
        showFullScreen();
        // } modified by kihyung        
        break;
    }

    default: //make compiler happy
        break;
    }
}

void AppMediaPlayer_Video_ScreenAUXPlayback::onLockoutMode(bool on)
{
   MP_LOG << "on =" << on << LOG_ENDL;

   //m_pModel->setLockoutMode(on);
   m_isLockout = on;
   //emit showLockout(on); //deleted by yungi 2013.11.03 for ITS 198971
   invokeQmlMethod("onShowLockout", QVariant(on)); //added by yungi 2013.11.03 for ITS 198971
}

// removed by Sergey 24.09.2013 for ITS#188938

// { added by radhakrushna 2013.03.11 for ISV 74997
void AppMediaPlayer_Video_ScreenAUXPlayback::onPlayAuxOnFG()
{
    MP_LOG << LOG_ENDL;
    setFullScreen();
    //m_pEngineVideo->hideLoadingScreenForAux(m_pModel); //added by shkim ITS_177764 // removed by Sergey 26.07.2013 for ITS#181096
}
// } added by radhakrushna

// { modified by Sergey 10.08.2013 for 182892
void AppMediaPlayer_Video_ScreenAUXPlayback::setFullScreen()
{
    // { moved by cychoi 2014.03.17 for HW Event DV2-1 1403-00059
    //if(this != dynamic_cast<AppMediaPlayer_Video_ScreenAUXPlayback*>(m_pEngineVideo->getScreenManager(m_disp)->topScreen()))
    QString topScreenName = m_pEngineVideo->getScreenManager(m_disp)->topScreenName();
    if(AppMediaPlayer_Video_Screens::SCREEN_AUX1_PLAYBACK != topScreenName &&
       AppMediaPlayer_Video_Screens::SCREEN_AUX2_PLAYBACK != topScreenName)
    {
        return;
    }
    // } moved by cychoi 2014.03.17

    // { modified by kihyung 2013.3.16 for ISV 73125
    int  nPlaybackStatus = m_pModel->playbackStatus();
    // removed by Sergey 24.09.2013 for ITS#188938

    if((nPlaybackStatus == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PLAYING || 
        nPlaybackStatus == AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_PAUSED) &&
       (m_pEngineVideo->getEngineMain().GetFrontView() == APP_MEDIA_MODE_VIDEO || 
        m_pEngineVideo->getEngineMain().GetRearView() == APP_MEDIA_MODE_VIDEO)) // removed by Sergey 24.09.2013 for ITS#188938
    {
        //Should set full screen in a 5 seconds if on top.
        m_fullScreenTimer.start();
    }
    else
    {
        m_fullScreenTimer.stop();
    }  
    // } modified by kihyung    
}


void AppMediaPlayer_Video_ScreenAUXPlayback::showFullScreen()
{
    pushScreen(AppMediaPlayer_Video_Screens::SCREEN_FULL);
}
// } modified by Sergey 10.08.2013 for 182892

// { added by Sergey 04.08.2013 for ITS#179312
void  AppMediaPlayer_Video_ScreenAUXPlayback::onEventReceived(const QString &srcScreenName,
                                                              const QString &resScreenName,
                                                              const int eventId,
                                                              const QVariant arg)
{
    Q_UNUSED(srcScreenName);
    Q_UNUSED(arg);

    if (resScreenName == AppMediaPlayer_Video_Screens::SCREEN_AUX1_PLAYBACK
            || resScreenName == AppMediaPlayer_Video_Screens::SCREEN_AUX2_PLAYBACK)
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
            // { added by cychoi 2013.12.09 for ITS 214030 DRS on Popup
            case EVENTS_DRS_MODE:
            {
                onLockoutMode(arg.toBool());
                break;
            }
            // } added by cychoi 2013.12.09
            default:
                break;
        }
    }
}
// } added by Sergey 04.08.2013 for ITS#179312

//{ added by cychoi 2013.08.14 for ITS 184421
void AppMediaPlayer_Video_ScreenAUXPlayback::setupModeArea()
{
    QString thisScreen, title, modearea;

    if(m_bIsAux1==true) //MEDIA_SOURCE_AUX1:
        thisScreen = AppMediaPlayer_Video_Screens::SCREEN_AUX1_PLAYBACK;
    else //MEDIA_SOURCE_AUX2:
        thisScreen = AppMediaPlayer_Video_Screens::SCREEN_AUX2_PLAYBACK;
    title = QT_TR_NOOP("STR_MEDIA_AUX"); // modified by Sergey 25.08.2013 for ITS#186435
    modearea =  AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA;
    notifyScreenManager(thisScreen, modearea, EVENTS_SET_TEXT, QVariant(title));
    notifyScreenManager(thisScreen, modearea, EVENTS_SET_COUNTER_VISIBLE, QVariant(false));
    notifyScreenManager(thisScreen, modearea, EVENTS_SET_LIST_BTN_VISIBLE, QVariant(false)); 
    notifyScreenManager(thisScreen, modearea, EVENTS_SET_MENU_BTN_VISIBLE, QVariant(true));
    notifyScreenManager(thisScreen, modearea, EVENTS_SET_RTEXT_VISIBLE, QVariant(false));
    notifyScreenManager(thisScreen, modearea, EVENTS_SET_ICON_VISIBLE, QVariant(false));
}
//} added by cychoi 2013.08.14


// { added by Sergey 15.09.2014 for ITS#189390
void AppMediaPlayer_Video_ScreenAUXPlayback::onFgLoading(bool on)
{
    m_pModel->setFgLoadingMode(on);
}
// } added by Sergey 15.09.2014 for ITS#189390
