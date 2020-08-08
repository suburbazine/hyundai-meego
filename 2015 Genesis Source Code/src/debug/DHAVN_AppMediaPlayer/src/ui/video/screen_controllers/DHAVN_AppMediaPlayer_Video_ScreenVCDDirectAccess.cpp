// { modified by Sergey 09.05.2013
#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerVCD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_VCD.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenVCDDirectAccess.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_DirectAccess.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h" // added by yungi 2013.09.07 for ITS 187311
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"

static const QString HYPHEN = "-";

AppMediaPlayer_Video_ScreenVCDDirectAccess::AppMediaPlayer_Video_ScreenVCDDirectAccess( AppMediaPlayer_Video_Controller *pController,
                                                                                       AppMediaPlayer_EngineVideo *pEngineVideo,
                                                                                       int disp, // added by yungi 2013.09.07 for ITS 187311
                                                                                       QObject *parent )
   : AppMediaPlayer_Video_Screen( parent ),
     m_pController( pController ),
     m_pEngineVideo( pEngineVideo ),
     //{ added by yungi 2013.09.07 for ITS 187311
     m_isLockout(false),
     m_disp(disp)
     //} added by yungi 2013.09.07
{
  m_pControllerVCD = static_cast<AppMediaPlayer_Video_ControllerVCD*>(m_pController->getController(MEDIA_SOURCE_VCD));
   m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_VCD*>(m_pControllerVCD->getModel());

   m_pModel = new AppMediaPlayer_Video_SModel_DirectAccess(this);
   setModel( m_pModel );
   m_pModel->setIndex1(HYPHEN);
   m_pModel->setIndex2(HYPHEN);
   m_overlays << AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA;
   //removed by sjhyun 2013.11.07 for ITS 207179
}


AppMediaPlayer_Video_ScreenVCDDirectAccess::~AppMediaPlayer_Video_ScreenVCDDirectAccess()
{
}

// { added by cychoi 2013.06.23 for DVD VCD screen management
void AppMediaPlayer_Video_ScreenVCDDirectAccess::onDeactivate()
{
    //onLockoutMode(m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode()); // modified by cychoi 2014.01.09 for ITS 218948 // added by yungi 2013.09.07 for ITS 187311
    //removed by cychoi 2013.08.14 for ITS 184421
}
// } added by cychoi 2013.06.23

void AppMediaPlayer_Video_ScreenVCDDirectAccess::onHide()
{
    MP_LOG << LOG_ENDL;
    //removed by cychoi 2013.08.14 for ITS 184421
    m_pControllerVCD->setPlaybackControl(true);
    //m_pControllerVCD->setDirectAccessDisplayed(false); //deleted by yungi 2013.09.12 for SmokeTest DirectAccess During DRS On //added by yungi 2013.09.05 for ITS 187311

    m_pEngineVideo->getEngineMain().setVideoSubScreen(m_disp == 0, false); // added by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
    // removed by sjhyun 2013.11.07 for ITS 207179
}

// { modified by Sergey 09.05.2013
void AppMediaPlayer_Video_ScreenVCDDirectAccess::onShow(QVariant arg)
{
    Q_UNUSED(arg);

    onLockoutMode(m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode()); // added by yungi 2013.09.07 for ITS 187311

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS,
                        AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                        EVENTS_SHOW_PLAYBACK_ONLY);

    setupModeArea();

    // { added by cychoi 2013.12.19 for ITS 215825 Default focus
    // { modified by wspark 2014.02.19 for ITS 225827
    if(m_pControllerVCD->isDirectAccessDisplayed(m_disp) == false)
    {
        setDirectAccessUI(false);
        //m_pModel->setIndex1(HYPHEN);
        //m_pModel->setIndex2(HYPHEN);
        if(m_pEngineVideo->getEngineMain().getCloneState() == CLONE_OFF)
        {
            m_pControllerVCD->setDirectAccessDisplayed(m_disp, true); // moved by cychoi 2013.12.23 for ITS 217058 //added by yungi 2013.09.05 for ITS 187311
        }
        else
        {
            m_pControllerVCD->setDirectAccessDisplayed(VIDEO_DISPLAY_FRONT, true);
            m_pControllerVCD->setDirectAccessDisplayed(VIDEO_DISPLAY_REAR, true);
        }
    }
    // } modified by wspark
    else
    {
        setDirectAccessUI(true);
    }
    // } added by cychoi 2013.12.19

    m_pEngineVideo->getEngineMain().setVideoSubScreen(m_disp == 0, true); // added by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
    // { added by sjhyun 2013.11.01 for ITS 204921
    if (m_pEngineVideo->getModeChangeOnDirectAccess() == VCD_DIRECTACCESS_MODE_END)
    {
        MP_LOG << "[VCD_DIRECTACCESS_MODE] END->READY" << LOG_ENDL;
        m_pEngineVideo->setModeChangeOnDirectAccess(VCD_DIRECTACCESS_MODE_READY);
    }
    // } added by sjhyun
}

// { added by cychoi 2013.12.17 for ITS 214030 DRS on Popup
void  AppMediaPlayer_Video_ScreenVCDDirectAccess::onEventReceived(const QString &srcScreenName,
                                                              const QString &resScreenName,
                                                              const int eventId,
                                                              const QVariant arg)
{
   MP_LOG << " srcScreenName = " << srcScreenName << " resScreenName = " << resScreenName << \
             " eventId = " << eventId << " arg = " << arg << LOG_ENDL;

   if (resScreenName == AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS)
   {
      switch( eventId )
      {
          case EVENTS_DRS_MODE:
          {
              onLockoutMode(arg.toBool());
              break;
          }
          // { added by wspark 2014.02.14 for ITS 225340
          case EVENTS_SELECT_PBC:
          {
              QString topScreen = m_pEngineVideo->getScreenManager(m_disp)->topScreenName();
              if(topScreen == AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS)
              {
                  emit popScreen();
                  m_pControllerVCD->setDirectAccessDisplayed(m_disp, false); // modified by wspark 2014.02.19 for ITS 225827
              }
              break;
          }
          // } added by wspark
          default:
             break; // make compiler happy
      }
   }
}
// } added by cychoi 2013.12.17

void AppMediaPlayer_Video_ScreenVCDDirectAccess::onSoftkeyBack()
{
    // { added by cychoi 2014.03.18 for ITS 229926
    if(m_pEngineVideo->getEngineMain().getCloneState() == CLONE_OFF)
    {
       m_pControllerVCD->setDirectAccessDisplayed(m_disp, false);
    }
    else
    {
       m_pControllerVCD->setDirectAccessDisplayed(VIDEO_DISPLAY_FRONT, false);
       m_pControllerVCD->setDirectAccessDisplayed(VIDEO_DISPLAY_REAR, false);
    }
    // } added by cychoi 2014.03.18
   emit popScreen();
}

void AppMediaPlayer_Video_ScreenVCDDirectAccess::onVCDMenuItemPressed(QString str)
{
   const static QRegExp rx = QRegExp("^[0123456789]$");
   if (str.contains(rx))
   {
      if(m_pModel->index1() == HYPHEN)
      {
         m_pModel->setIndex1(str);
      }
      else if(m_pModel->index2() == HYPHEN)
      {
         m_pModel->setIndex2(str);
      }
   }
   else if(str == "OK")
   {
      int newTrack = convertToInt();
      if(newTrack > m_pControllerModel->trackCount() || newTrack <= 0)
      {
         // { modified by cychoi 2014.03.19 for ITS 230153
         //m_pEngineVideo->showTxtBtnPopup(POPUP_TXT_BTN_UNAVAILABLE_TRACK, (VIDEO_DISPLAY_T)m_disp);
         showTxtBtnPopUp( POPUP_TXT_BTN_UNAVAILABLE_TRACK );
         // } modified by cychoi 2014.03.19
      }
      else
      {
         m_pEngineVideo->DisplayOSD(true);//  modified by yongkyun.lee 2013-07-09 for : ITS 178789
         m_pControllerVCD->switchTrack(convertToInt());
         // { modified by wspark 2014.02.19 for ITS 225827
         if(m_pEngineVideo->getEngineMain().getCloneState() == CLONE_OFF)
         {
            m_pControllerVCD->setDirectAccessDisplayed(m_disp, false);  // added by yungi 2013.09.16 for SmokeTest DirectAccess During DRS On 2nd
         }
         else
         {
            m_pControllerVCD->setDirectAccessDisplayed(VIDEO_DISPLAY_FRONT, false);
            m_pControllerVCD->setDirectAccessDisplayed(VIDEO_DISPLAY_REAR, false);
         }
         // } modified by wspark
         m_pModel->setIndex1(HYPHEN);
         m_pModel->setIndex2(HYPHEN);
         emit popScreen();
      }
   }
   else if(str == "Delete")
   {
      if(m_pModel->index2() != HYPHEN)
      {
         m_pModel->setIndex2(HYPHEN);
      }
      else if(m_pModel->index1() != HYPHEN)
      {
         m_pModel->setIndex1(HYPHEN);
      }
   }
}

// { added by cychoi 2013.12.19 ITS 215825
void AppMediaPlayer_Video_ScreenVCDDirectAccess::setDefaultInputText()
{
    m_pModel->setIndex1(HYPHEN);
    m_pModel->setIndex2(HYPHEN);
}
// } added by cychoi 2013.12.19

int AppMediaPlayer_Video_ScreenVCDDirectAccess::convertToInt()
{
   QString convStr = "" + (m_pModel->index1() != HYPHEN ? m_pModel->index1() :"") +
                          (m_pModel->index2() != HYPHEN ? m_pModel->index2() :"");
   if (convStr != "")
   {
      return convStr.toInt();
   }
   return -1;
}

void AppMediaPlayer_Video_ScreenVCDDirectAccess::setupModeArea()
{
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_LIST_BTN_VISIBLE, QVariant(false));

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_MENU_BTN_VISIBLE, QVariant(false));

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_COUNTER_VISIBLE, QVariant(false));

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_RTEXT_VISIBLE, QVariant(true));

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_RTEXT, QVariant(QString::number(m_pControllerModel->trackCount())));

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_ICON_VISIBLE, QVariant(true));

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_ICON, QVariant("/app/share/images/video/ico_chapter_s.png"));
}

//removed by cychoi 2013.08.14 for ITS 184421
// } modified by Sergey 09.05.2013

// { added by yungi 2013.09.05 for ITS 187311
void AppMediaPlayer_Video_ScreenVCDDirectAccess::onLockoutMode(bool onShow)
{
   MP_LOG<<onShow << LOG_ENDL;
   m_isLockout = onShow;
   emit showLockout(onShow);
}
// } added by yungi 2013.09.05 for ITS 187311

// { added by cychoi 2014.07.01 for focus handling on LCD ON/OFF (ITS 241667)
void AppMediaPlayer_Video_ScreenVCDDirectAccess::onBackKeyPressed()
{
    onSoftkeyBack();
}
// } added by cychoi 2014.07.01

// removed by sjhyun 2013.11.07 for ITS 207179
