#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenFull.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"
// { added by wspark 2012.11.14 for ISV 62589
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerVCD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_VCD.h"
// } added by wspark

AppMediaPlayer_Video_ScreenFull::
AppMediaPlayer_Video_ScreenFull( AppMediaPlayer_Video_Controller *controller,
                                 AppMediaPlayer_EngineVideo *pEngineVideo,
                                 int disp,
                                 QObject *parent)
   : AppMediaPlayer_Video_Screen ( parent ),
     m_pEngineVideo( pEngineVideo ),
     m_bTuneMode(false),
     m_pControllerVideo( controller ),
     m_fullScreenState( false ), // added by cychoi 2013.06.10 for ITS 172874 popup dimming
     m_disp(disp),
     m_isLockout(false)
{

   m_overlays << AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA;

// { removed by Sergey for CR#16015

// } removed by Sergey for CR#16015


   connect(m_pEngineVideo, SIGNAL(setTuneMode(bool)), this, SLOT(onSetTuneMode(bool)));
   connect(m_pEngineVideo, SIGNAL(copyCompleted()), this, SLOT(onFMCopyCompleted())); // modified by ravikanth 12-07-13 for ITS 0174301

   // { added by wspark 2012.11.14 for ISV 62589
   m_pControllerVCD = static_cast<AppMediaPlayer_Video_ControllerVCD*>(m_pControllerVideo->getController(MEDIA_SOURCE_VCD));
   m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_VCD*>(m_pControllerVCD->getModel());
   // } added by wspark
}

AppMediaPlayer_Video_ScreenFull::~AppMediaPlayer_Video_ScreenFull()
{
}

void AppMediaPlayer_Video_ScreenFull::onShow(QVariant arg)
{
    MP_LOG << arg << LOG_ENDL;
    
    // Don't hide staus bar if in tune mode.
    //m_pEngineVideo->setStatusBarOnScreen(m_bTuneMode); //removed by ruindmby 2012.08.13
    m_pEngineVideo->fullScreenModeStatusChanged(true);
    m_fullScreenState = true; // added by cychoi 2013.06.10 for ITS 172874 popup dimming
    m_pEngineVideo->getEngineMain().setFullScreen(m_disp == 0); //added by edo.lee full screen OSd
    onLockoutMode(m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode()); //added by edo.lee 2013.09.14

    //emit hideAllFocus(); // removed by Dmitry 26.05.13
    emit setItemProperty(QString("statusBar"), "onScreen", QVariant(false)); //modified by edo.lee 2013.04.04
    emit setItemProperty(QString("playbackControlls"), "onScreen", QVariant(false));
    emit setItemProperty(QString("playbackControlls"), "isCommonJogEnabled", QVariant(true)); // added by Dmitry Bykov 08.04.13 for touch rule
    emit setItemProperty(QString("progressBar"), "onScreen", QVariant(false));
    emit setItemProperty(QString("modeArea"), "onScreen", QVariant(false));
    if(!m_pEngineVideo->isSystemPopupVisible())
        emit setItemProperty(QString("playbackGestureArea"), "enabled", QVariant(true));
    connect( m_pControllerModel, SIGNAL(isPbcMenuDisplayedChanged(bool)), this, SLOT(onIsPbcMenuDisplayedChanged(bool)) );// added by wspark 2012.11.14 for ISV 62589
}

void AppMediaPlayer_Video_ScreenFull::onHide()
{
   MP_LOG << LOG_ENDL;
   m_pEngineVideo->setStatusBarOnScreen( true , (m_disp== VIDEO_DISPLAY_FRONT)?true:false);
   m_pEngineVideo->fullScreenModeStatusChanged(false);
   m_fullScreenState = false; // added by cychoi 2013.06.10 for ITS 172874 popup dimming
   m_pEngineVideo->getEngineMain().setPlaybackScreen(m_disp == 0); //added by edo.lee full screen OSd 2013.08.23
   
   emit setItemProperty(QString("statusBar"), "onScreen", QVariant(true)); //modified by edo.lee 2013.04.04
   emit setItemProperty(QString("playbackControlls"), "onScreen", QVariant(true));
   emit setItemProperty(QString("progressBar"), "onScreen", QVariant(true));
   emit setItemProperty(QString("modeArea"), "onScreen", QVariant(true));
   disconnect( m_pControllerModel, SIGNAL(isPbcMenuDisplayedChanged(bool)), // added by wspark 2012.11.14 for ISV 62589
              this, SLOT(onIsPbcMenuDisplayedChanged(bool)) );
}

void AppMediaPlayer_Video_ScreenFull::onDeactivate()
{
    emit setItemProperty(QString("playbackGestureArea"), "enabled", QVariant(false));
//    emit setItemProperty(QString("playbackControlls"), "isCommonJogEnabled", QVariant(false)); // commented by Sergey 17.10.2013 for ITS#196105
}

void AppMediaPlayer_Video_ScreenFull::onBackKeyPressed()
{
   emit setItemProperty(QString("playbackControlls"), "onScreen", QVariant(true));
   emit setItemProperty(QString("progressBar"), "onScreen", QVariant(true));
   emit setItemProperty(QString("modeArea"), "onScreen", QVariant(true));
   emit hideUI();
}

void AppMediaPlayer_Video_ScreenFull::onMenuKeyPressed()
{
   popScreen(QVariant("Options"));
}
//added by edo.lee 2013.10.10
void AppMediaPlayer_Video_ScreenFull::onMouseReleased()
{
}

void AppMediaPlayer_Video_ScreenFull::onMouseClicked()
{
   MP_LOG << LOG_ENDL;
/* remove by edo.lee 2013.07.31
if(m_pEngineVideo->getIsShowLockoutView() != VIDEO_DISPLAY_MAX)
	{
		m_pEngineVideo->setDefaultScreen(VIDEO_DISPLAY_FRONT);
		m_pEngineVideo->setDefaultScreen(VIDEO_DISPLAY_REAR);	
	}
	else*/
 	popScreen();   
/* remove by edo.lee 2013.07.30
	if(m_disp == VIDEO_DISPLAY_FRONT)
		m_pEngineVideo->setFrontFull(false);
	else if(m_disp == VIDEO_DISPLAY_REAR)
		m_pEngineVideo->setRearFull(false);
*/
}

void AppMediaPlayer_Video_ScreenFull::onSetTuneMode(bool on)
{
    m_bTuneMode = on;
	// removed by Sergey 01.05.2013 for Tune Functionality
}

void AppMediaPlayer_Video_ScreenFull::onNextBtn()
{
   MP_LOG << LOG_ENDL;
  m_pEngineVideo->DisplayOSD(true); //  added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD   
   m_pControllerVideo->next();
}

void AppMediaPlayer_Video_ScreenFull::onPrevBtn()
{
   MP_LOG << LOG_ENDL;
   m_pEngineVideo->DisplayOSD(true); //  added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD   
   m_pControllerVideo->prev();
}

void AppMediaPlayer_Video_ScreenFull::onFlick_next()
{
    m_pControllerVideo->flickNext();
}

void AppMediaPlayer_Video_ScreenFull::onFlick_prev()
{
    m_pControllerVideo->flickPrev();
}

// { added by Sergey 20.04.2013
void AppMediaPlayer_Video_ScreenFull::onNextBtnHold()
{
    m_pControllerVideo->fastForward();
}

void AppMediaPlayer_Video_ScreenFull::onNextBtnHoldCritical()
{
    m_pControllerVideo->fastForwardCritical();
}

void AppMediaPlayer_Video_ScreenFull::onPrevBtnHold()
{
    m_pControllerVideo->rewind();
}

void AppMediaPlayer_Video_ScreenFull::onPrevBtnHoldCritical()
{
    m_pControllerVideo->rewindCritical();
}
// { modified by edo.lee 2013.08.10 ISV 88688
void AppMediaPlayer_Video_ScreenFull::onLockoutMode(bool on)
{
   MP_LOG << "on =" << on << LOG_ENDL;

   m_isLockout = on;
   emit showLockout(on);
}
// { modified by edo.lee 2013.08.10 ISV 88688
void AppMediaPlayer_Video_ScreenFull::onCancel_ff_rew()
{
   // m_pEngineVideo->getEngineMain().setUnMuteState(false); //remove by edo.lee 2013.04.22 for Mute
    m_pControllerVideo->cancel_ff_rew();
}
// } added by Sergey 20.04.2013

// { removed by Sergey for CR#16015

// } removed by Sergey for CR#16015

// { added by wspark 2012.11.14 for ISV 62589
void AppMediaPlayer_Video_ScreenFull::onIsPbcMenuDisplayedChanged(bool value)
{
   MP_LOG << " value = " << value << LOG_ENDL;
   if(AppMediaPlayer_Video_Screens::SCREEN_FULL == m_pEngineVideo->getScreenManager(m_disp)->topScreenName())
   {
       if ( value == true  && m_pControllerModel->isPbcMode())
       {
          // { modified by cychoi 2014.03.13 for ITS 229176          
          //if(m_isLockout == false)
          //{
          //    onHide(); //added by eunhye 2013.05.09 ITS 163782 
          //}
          // } modified by cychoi 2014.03.13
          m_pEngineVideo->setStatusBarOnScreen( true, (m_disp== VIDEO_DISPLAY_FRONT)?true:false );
          m_pEngineVideo->popScreenFromBoth(AppMediaPlayer_Video_Screens::SCREEN_OPTIONS); // added by cychoi 2013.12.30 for ITS 217646
          emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU);
       }

   }
}

// added by Dmitry 26.05.13
void AppMediaPlayer_Video_ScreenFull::onLongPressPlayBtn()
{
//   emit popScreen(); // commented by Sergey 24.07.2013 ITS#181467
}

void AppMediaPlayer_Video_ScreenFull::onTogglePlayBtn()
{
   emit popScreen();
   m_pEngineVideo->getScreenManager(m_disp)->SendJogEvent(AppEngineQMLConstants::JOG_CENTER, AppEngineQMLConstants::KEY_STATUS_RELEASED);
}
// added by Dmitry 26.05.13

// modified by ravikanth 12-07-13 for ITS 0174301
void AppMediaPlayer_Video_ScreenFull::onFMCopyCompleted()
{
    MP_LOG << m_pControllerVideo->getSource() << LOG_ENDL;
    if(m_pControllerVideo->getSource() != MEDIA_SOURCE_JUKEBOX)
    {
        pushToastPopUp(POPUP_TOAST_COPY_COMPLETED);
    }
}

// { added by Sergey 03.08.2013 for ITS#0180899
void AppMediaPlayer_Video_ScreenFull::onEventReceived( const QString &srcScreenName,
                                                              const QString &resScreenName,
                                                              const int eventId,
                                                              const QVariant arg )
{
    MP_LOG << " srcScreenName = " << srcScreenName << " resScreenName = " << resScreenName << \
              " eventId = " << eventId << " arg = " << arg << LOG_ENDL;

    Q_UNUSED(srcScreenName);
    Q_UNUSED(arg);

    if(resScreenName == AppMediaPlayer_Video_Screens::SCREEN_FULL)
    {
       switch( eventId )
       {
           case EVENTS_NO_FULLSCREEN_ANIM:
           {
               emit setItemProperty(QString("modeArea"),            "bFullScreenAnimation", QVariant(false));
               emit setItemProperty(QString("playbackControlls"),   "bFullScreenAnimation", QVariant(false));
               emit setItemProperty(QString("progressBar"),         "bFullScreenAnimation", QVariant(false));

               break;
           }

           default:
           {
               MP_LOG << "Unsupported event." << LOG_ENDL;
               break;
           }
       }
    }
}
// } added by Sergey 03.08.2013 for ITS#0180899

void AppMediaPlayer_Video_ScreenFull::onSelectTuned()
{
        m_pControllerVideo->selectTuned();
        onSetTuneMode(false);
        m_pControllerVideo->cancelTune();
}
