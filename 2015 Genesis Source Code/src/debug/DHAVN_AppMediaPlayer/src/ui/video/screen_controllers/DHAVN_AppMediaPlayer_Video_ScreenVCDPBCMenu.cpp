#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerVCD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_VCD.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenVCDPBCMenu.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_PbcMenu.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"

AppMediaPlayer_Video_ScreenVCDPBCMenu::AppMediaPlayer_Video_ScreenVCDPBCMenu( AppMediaPlayer_Video_Controller *pController,
                                                                             AppMediaPlayer_EngineVideo *pEngineVideo,
                                                                             int disp,
                                                                             QObject *parent )
   : AppMediaPlayer_Video_Screen( parent ),
     m_pController( pController ),
     m_pEngineVideo( pEngineVideo ),
     m_isLockout(false), // added by cychoi 2013.06.23 for DVD VCD screen management
     m_bInitText(false), // added by cychoi 2013.07.07 for New UX
     m_disp(disp)
{
   m_pControllerVCD = static_cast<AppMediaPlayer_Video_ControllerVCD*>(m_pController->getController(MEDIA_SOURCE_VCD));
   m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_VCD*>(m_pControllerVCD->getModel());

   m_pModel = new AppMediaPlayer_Video_SModel_PbcMenu(this);
   setModel( m_pModel );
   m_pModel->setInputtext("  "); // added by cychoi 2013.12.16 ITS 215825

   connect(&m_pEngineVideo->getEngineMain(), SIGNAL(retranslateUi(QVariant)), this, SLOT(onRetranslateUI())); // added by yungi 2013.11.22 for ITS 210298
}


AppMediaPlayer_Video_ScreenVCDPBCMenu::~AppMediaPlayer_Video_ScreenVCDPBCMenu()
{
}

void AppMediaPlayer_Video_ScreenVCDPBCMenu::onHide()
{
   MP_LOG << LOG_ENDL;
   // { added for eunhye 2013.04.04 for ISV_KR 78583
   if(!m_pEngineVideo->getFullScreenState())
       m_pEngineVideo->setFullScreenState(true);
   // } added for eunhye 2013.04.04 for ISV_KR 78583
   // removed by cychoi 2013.12.04 for ITS 212759
   disconnect( m_pControllerModel, SIGNAL(isPbcMenuDisplayedChanged(bool)),
           this, SLOT(onIsPbcMenuDisplayedChanged(bool)) );
}


void AppMediaPlayer_Video_ScreenVCDPBCMenu::onShow( QVariant arg )
{
   Q_UNUSED(arg);

   MP_LOG << LOG_ENDL;
   // { added for eunhye 2013.04.04 for ISV_KR 78583
   if(m_pEngineVideo->getFullScreenState())
       m_pEngineVideo->setFullScreenState(false);
   // } added for eunhye 2013.04.04 for ISV_KR 78583
   onLockoutMode(m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode()); //added by edo.lee 2013.03.21

   // { added by cychoi 2013.06.23 for DVD VCD screen management
   if (!m_isLockout)
   {
   // } added by cychoi 2013.06.23
       if(m_pEngineVideo->getEngineMain().middleEast())
           m_pController->SetVideoSizeWrtDisplay( 688, 253, 565, 380 ); //added by Michael.Kim 2013.08.07 for Middle East UI
       else
           m_pController->SetVideoSizeWrtDisplay( 27, 253, 565, 380 );
   // { added by cychoi 2013.06.23 for DVD VCD screen management
   }
   // } added by cychoi 2013.06.23

   // { added by cychoi 2013.12.19 for ITS 215825 Default focus
   if(m_pControllerVCD->isPbcMenuShown() == false)
   {
       setPBCMenuUI(false);
       //m_pModel->setInputtext("  ");  // commented by cychoi 2013.12.16 ITS 215825 // modified by cychoi 2013.07.07 for New UX
   }
   else
   {
       setPBCMenuUI(true);
   }
   // } added by cychoi 2013.12.19

   connect( m_pControllerModel, SIGNAL(isPbcMenuDisplayedChanged(bool)),
           this, SLOT(onIsPbcMenuDisplayedChanged(bool)) );

   setupModeArea(); // modified by cychoi 2013.07.07 for New UX
}


// { added by cychoi 2013.07.03 for ITS 177711 
void AppMediaPlayer_Video_ScreenVCDPBCMenu::onDeactivate()
{
    onLockoutMode(m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode());
}
// } added by cychoi 2013.07.03


void AppMediaPlayer_Video_ScreenVCDPBCMenu::onKeyEntered( int index )
{
   MP_LOG << " index = " << index << LOG_ENDL;
   QString value;
   value = m_pModel->inputtext();
   MP_LOG << " value = " << value << LOG_ENDL;
   if( index == KEY_NEXT )
   {
   	  //m_pEngineVideo->getEngineMain().setUnMuteState(false); // commented by cychoi 2013.08.30 for ITS 184879 //added by edo.lee 2013.04.20
      m_pControllerVCD->next();
   }
   else if ( index == KEY_PREV )
   {
   	  //m_pEngineVideo->getEngineMain().setUnMuteState(false); // commented by cychoi 2013.08.30 for ITS 184879 //added by edo.lee 2013.04.20
      m_pControllerVCD->prev();
   }
   // { added by cychoi 2013.07.07 for New UX
   else if( index == KEY_OK )
   {
      onEnterKey();
   }
   else if( index == KEY_DEL )
   {
       if ((value.data()[0]  != ' ' && value.data()[1] != ' '))
       {
          m_pControllerVCD->pbcClearKey(); // modified by cychoi 2013.07.06 for ITS 163782

           index = (value.data()[0]).digitValue();

           value = QString::number(index)+" ";
           m_pModel->setInputtext(value);
           m_pControllerVCD->pbcTenKey( index );
       }
       else if (value.data()[0]  != ' ' && value.data()[1] == ' ')
       {
           m_pControllerVCD->pbcClearKey();

           m_pModel->setInputtext("  "); 
       }
   }
   else if( index == KEY_RETURN )
   {
      onReturnKey();
   }
   // } added by cychoi 2013.07.07
   // { modified by cychoi 2013.07.07 for New UX
   else
   {
      if (m_bInitText == true || (value.data()[0]  != ' ' && value.data()[1] != ' '))
      {
          m_pControllerVCD->pbcClearKey();
          m_bInitText = false;

          index = (index == KEY_0)? 0: index + 1;
          value = QString::number(index)+" ";
          m_pModel->setInputtext(value);
      }
      else if (value.data()[0]  != ' ' && value.data()[1] == ' ')
      {
         index = (index == KEY_0)? 0: index + 1;
         m_pModel->setInputtext( (QString::number( (value.data()[0]).digitValue() )).append(QString::number(index) ));
      }
      else
      {
         index = (index == KEY_0)? 0: index + 1;
         value = QString::number(index)+" ";
         m_pModel->setInputtext(value);
      }

      m_pControllerVCD->pbcTenKey( index );

      MP_LOG << " m_pModel->inputtext() = " << m_pModel->inputtext() << LOG_ENDL;
   }
   // } modified by cychoi 2013.07.07
}


void AppMediaPlayer_Video_ScreenVCDPBCMenu::onEnterKey()
{
   MP_LOG << LOG_ENDL;
   // { modified by cychoi 2013.07.07 for New UX
   m_bInitText = true;
   m_pModel->setInputtext("  "); 
   // } modified by cychoi 2013.07.07
   m_pControllerVCD->pbcEnterKey();
}

// { modified by cychoi 2013.07.06 for New UX
void AppMediaPlayer_Video_ScreenVCDPBCMenu::onReturnKey()
{
    MP_LOG << LOG_ENDL;

    m_pControllerVCD->pbcReturnKey();
}

void AppMediaPlayer_Video_ScreenVCDPBCMenu::onSoftkeyBack()
{
    MP_LOG << LOG_ENDL;

    onPbcOff();
}
// } modified by cychoi 2013.07.07

void AppMediaPlayer_Video_ScreenVCDPBCMenu::onPbcOff()
{
   MP_LOG << LOG_ENDL;
   m_pModel->setInputtext("  "); // added by cychoi 2013.12.16 ITS 215825
   m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU,
                         AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                         EVENTS_SELECT_PBC,
                         QVariant(false));

   // { added by kihyung 2012.07.12
   m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU,
                         AppMediaPlayer_Video_Screens::SCREEN_OPTIONS,
                         EVENTS_PBC_MENU,
                         QVariant(false));
   // } added by kihyung
   
   m_pControllerVCD->setPBC(false);

   m_pEngineVideo->setFSAnimation(false); // added by cychoi 2014.03.13 for ITS 229176
   emit popScreen();
   // { added by wspark 2012.11.14 for ISV 62589
   if(AppMediaPlayer_Video_Screens::SCREEN_FULL == m_pEngineVideo->getScreenManager(m_disp)->topScreenName())
   {
       m_pEngineVideo->setStatusBarOnScreen( false, (m_disp== VIDEO_DISPLAY_FRONT)?true:false );
   }
   // } added by wspark
   m_pControllerModel->setLoadingMode(true); // added by cychoi 2013.08.27 for ITS 184304 Loading screen close

   // { added by wspark 2014.01.22 for ITS 221857
   int vcdOpposite = m_disp == 0 ? 1 : 0;
   if(m_pEngineVideo->getIsShowLockoutView() == vcdOpposite &&
      m_pEngineVideo->getScreenManager(vcdOpposite)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_OPTIONS)
   {
       m_pEngineVideo->getScreenManager(vcdOpposite)->popScreen();
   }
   // } added by wspark
   m_pEngineVideo->setFSAnimation(true); // added by cychoi 2014.03.13 for ITS 229176
}

// { added by cychoi 2013.12.16 ITS 215825
void AppMediaPlayer_Video_ScreenVCDPBCMenu::setDefaultInputText()
{
    m_pModel->setInputtext("  "); 
}
// } added by cychoi 2013.12.16


// { added by cychoi 2014.01.23 for ITS 222055
//int AppMediaPlayer_Video_ScreenVCDPBCMenu::getCurrentInputTextCount()
//{
//    return m_pModel->inputtext().count();
//}

//bool AppMediaPlayer_Video_ScreenVCDPBCMenu::isInputAvailable()
//{
//    QString value;
//    value = m_pModel->inputtext();
//    return (value.data()[0] == ' ' || value.data()[1] == ' ');
//}
// } added by cychoi 2014.01.23

void AppMediaPlayer_Video_ScreenVCDPBCMenu::onIsPbcMenuDisplayedChanged( bool value )
{
   MP_LOG << "value = " << value << LOG_ENDL;
// modified by ruindmby 2012.08.29 for CR 11030
   if (value == false)
   {
      m_pEngineVideo->setFSAnimation(false); // added by cychoi 2014.03.13 for ITS 229176
      emit popScreen();
      // { added by wspark 2012.11.14 for ISV 62589
      if(AppMediaPlayer_Video_Screens::SCREEN_FULL == m_pEngineVideo->getScreenManager(m_disp)->topScreenName())
      {
          m_pEngineVideo->setStatusBarOnScreen( false,(m_disp== VIDEO_DISPLAY_FRONT)?true:false );
      }
      // } added by wspark
      m_pEngineVideo->setFSAnimation(true); // added by cychoi 2014.03.13 for ITS 229176
   }
// modified by ruindmby 2012.08.29 for CR 11030
}

void AppMediaPlayer_Video_ScreenVCDPBCMenu::onLockoutMode(bool on)
{
	MP_LOG << LOG_ENDL;
	m_isLockout = on;
	emit showLockout(on);
}

// { added by wspark 2013.04.18 for ISV 70606
void AppMediaPlayer_Video_ScreenVCDPBCMenu::onBackKeyPressed()
{
    MP_LOG << LOG_ENDL;
    onPbcOff();
}
// } added by wspark

// { modified by cychoi 2013.07.07 for New UX
void AppMediaPlayer_Video_ScreenVCDPBCMenu::setupModeArea()
{
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU, AppMediaPlayer_Video_Screens::SCREEN_VCD_PLAYBACK,
                        EVENTS_SHOW_PLAYBACK_ONLY);
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_COUNTER_VISIBLE, QVariant(false));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_TEXT, QVariant(QT_TR_NOOP("STR_MEDIA_PBC_MENU"))); // modified by Sergey 25.08.2013 for ITS#186435
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_LIST_BTN_VISIBLE, QVariant(false)); 
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_MENU_BTN_VISIBLE, QVariant(false)); 
    //{ added by cychoi 2013.08.14 for ITS 184421
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_RTEXT_VISIBLE, QVariant(false));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_ICON_VISIBLE, QVariant(false));
    //} added by cychoi 2013.08.14
}
// } modified by cychoi 2013.07.07

// { added by yungi 2013.11.22 for ITS 210298
void AppMediaPlayer_Video_ScreenVCDPBCMenu::onRetranslateUI()
{
    if (!m_isLockout && m_pEngineVideo->getScreenManager(m_disp)->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_VCD_PBCMENU )
    {
        if(m_pEngineVideo->getEngineMain().middleEast())
            m_pController->SetVideoSizeWrtDisplay( 688, 253, 565, 380 );
        else
            m_pController->SetVideoSizeWrtDisplay( 27, 253, 565, 380 );
    }
}
// } added by yungi
