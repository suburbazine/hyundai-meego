// { modified by Sergey for CR#15744, 16015, 16777
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenTextButtonPopup.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenVCDDirectAccess.h" // added by wspark 2013.03.14 for ITS 136262


AppMediaPlayer_Video_ScreenTextButtonPopup::
AppMediaPlayer_Video_ScreenTextButtonPopup(AppMediaPlayer_Video_Controller *pController,
                                           AppMediaPlayer_EngineVideo *pEngineVideo,
                                           int disp, // added by wspark 2013.03.14 for ITS 136262
                                           QObject *parent)
    : AppMediaPlayer_Video_Screen(parent),
      m_pEngineVideo(pEngineVideo),
      m_pControllerFS(NULL),
      m_pController(pController),
      m_disp(disp), // added by wspark 2013.03.14 for ITS 136262
      m_popUpType(POPUP_TXT_BTN_MAX),
      m_isLockout(false)
{
    m_myName = AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON; // added by Sergey 22.09.2013 for ITS#186731 
    m_pModel = new AppMediaPlayer_Video_SModel_TextButtonPopup(this);
    setModel(m_pModel);
	m_pCModel = NULL;//added by edo.lee 2013.07.15 STATIC TEST
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()),this, SLOT(closeScreen()));

    initMessage();

    connect(&m_pEngineVideo->getEngineMain(), SIGNAL(retranslateUi(QVariant)), this, SLOT(onRetranslateUI())); // added by Sergey for DRM 30.07.2013
}


AppMediaPlayer_Video_ScreenTextButtonPopup::~AppMediaPlayer_Video_ScreenTextButtonPopup()
{
}


void AppMediaPlayer_Video_ScreenTextButtonPopup::onHide()
{
    if ( m_pTimer->isActive() )
       m_pTimer->stop();

	if(m_popUpType == POPUP_TXT_BTN_UNSUPPORTED_ALL)
	{
		m_pEngineVideo->getEngineMain().GetMediaManager()->SetSourceAvailable(m_pController->getSource(), MEDIA_TYPE_VIDEO, false);
		m_pEngineVideo->getEngineMain().GetNotifier()->UpdateSDPMediaCount(m_pController->getSource(), MEDIA_TYPE_VIDEO, 0);
		m_pEngineVideo->getEngineMain().GetNotifier()->setEnableMedia(m_pController->getSource(), MEDIA_TYPE_MAX);
	} // added by Sergey 25.05.2013

    m_popUpType = POPUP_TXT_BTN_MAX; // added by Sergey for DRM 30.07.2013
    m_pEngineVideo->setPopupType(m_popUpType);//added by taihyun.ahn for ITS 221592 2014.01.23
    m_pEngineVideo->setLocalPopupShow(false, m_disp); // modified by yungi 2014.01.23 for ITS 221843 // added by cychoi 2013.12.05 for ITS 213053
}


void AppMediaPlayer_Video_ScreenTextButtonPopup::onShow(QVariant arg)
{
    m_pControllerFS = static_cast<AppMediaPlayer_Video_ControllerFS*>
            (m_pController->getCurrentController());
    if(m_pControllerFS)
        m_pCModel = m_pControllerFS->getFSModel();

    if(arg.canConvert<POPUP_TXT_BTN_TYPE_T>())
        setupPopUp(arg.value<POPUP_TXT_BTN_TYPE_T>());
    else
        MP_LOG << "Can not convert input arg to popUp type!" << LOG_ENDL;
    
    //{ remove by edo.lee 2013.2013.05.07 modified requirement
    //if(m_popUpType == POPUP_TXT_BTN_UNSUPPORTED_FILE)
    //{
    //    emit popUpTimerOut();
    //}
    //{ added by yongkyun.lee 20130324     

    notifyScreenManager(m_myName, m_pEngineVideo->getScreenManager((VIDEO_DISPLAY_T)m_disp)->firstScreen(), EVENTS_RESET_PENDING_POPUP); // added by Sergey 22.09.2013 for ITS#186731
    onLockoutMode(m_pEngineVideo->getScreenManager(m_disp)->getIsLockoutMode()); // modified for ITS 0195160

    m_pEngineVideo->setLocalPopupShow(true, m_disp); // modified by yungi 2014.01.23 for ITS 221843 // added by cychoi 2013.12.05 for ITS 213053
}


// { modified by Sergey for DRM 30.07.2013
void AppMediaPlayer_Video_ScreenTextButtonPopup::setupPopUp(POPUP_TXT_BTN_TYPE_T typePopUp)
{
    MP_LOG << "typePopUp = " << typePopUp << LOG_ENDL;

    QString text = m_messagePopUp.value(typePopUp,""); //added by Michael.Kim 2013.06.25 for ISV Issue 85785
    QString button1 = QT_TR_NOOP("STR_POPUP_OK"); //modified by Michael.Kim 2013.06.25 for ISV Issue 85785
    QString button2 = "";
    switch(typePopUp)
    {
        case POPUP_TXT_BTN_COPY_EXISTED_FILE:
        {
            if(m_pControllerFS)
            {
                text = QApplication::translate("main", text.toAscii());
                text = text.arg(m_pControllerFS->getCopyFileName()); // add copying file name to the message
            }
            button1 = QT_TR_NOOP("STR_MEDIA_MNG_YES"); //modified by Michael.Kim 2013.06.25 for ISV Issue 85785
            button2 = QT_TR_NOOP("STR_MEDIA_CANCEL_BUTTON"); //modified by Michael.Kim 2013.06.25 for ISV Issue 85785
            break;
        }
        case POPUP_TXT_BTN_DRM_CONFIRM:
        {
            if(m_pCModel)
            {
                text = QApplication::translate("main", text.toAscii());
                text = text.arg(m_pCModel->drmUseCount()).arg(m_pCModel->drmUseLimit());
            }
            button2 = QT_TR_NOOP("STR_MEDIA_MNG_CANCEL"); //modified by Michael.Kim 2013.06.25 for ISV Issue 85785
            break;
        }
        case POPUP_TXT_BTN_DRM_EXPIRED:
        {
            if(m_pCModel)
            {
                text = QApplication::translate("main", text.toAscii());
                text = text.arg(m_pCModel->drmUseLimit());
            }
            break;
        }
        // { added by wspark 2012.11.20 for CR15380
        case POPUP_TXT_BTN_CAPACITY_ERROR:
        {
            if(m_pControllerFS)
            {
                text = QApplication::translate("main", text.toAscii());
                text = text.arg(m_pControllerFS->getFilesCount()).arg(m_pControllerFS->getFilesSize());
            }
            button2 = QT_TR_NOOP("STR_MEDIA_MNG_JUKEBOX_MANAGER"); //modified by Michael.Kim 2013.06.25 for ISV Issue 85785
            break;
        }
        // } added by wspark
		// { modified by ravikanth 16-04-13
    case POPUP_TXT_BTN_COPY_INPROGRESS:
    {
        button1 = QT_TR_NOOP("STR_MEDIA_MNG_YES"); //modified by Michael.Kim 2013.06.25 for ISV Issue 85785
        button2 = QT_TR_NOOP("STR_MEDIA_MNG_NO"); //modified by Michael.Kim 2013.06.25 for ISV Issue 85785
        break;
    }
	// } modified by ravikanth 16-04-13
        default: // make compiler happy
            break;
    }

    m_popUpType = typePopUp;
    m_pModel->setSText(text);
    if(typePopUp != POPUP_TXT_BTN_COPY_INPROGRESS) // modified by ravikanth 22.06.13 for ITS 0175873
    {
        m_pModel->setIsWarningIcon(true);
    }
    emit updateButtonModel(button1, button2);
    //m_pEngineVideo->getEngineMain().beep(eBeepType1); // modify by youngsim.jo ISV 99507
}
// } modified by Sergey for DRM 30.07.2013

//added by edo.lee 2013.12.13 
void AppMediaPlayer_Video_ScreenTextButtonPopup::closePopupBySystemPopup()
{
   MP_LOG << LOG_ENDL;
   onSoftkeyBtn("btn2");
}
//added by edo.lee 2013.12.13 

// { added by cychoi 2014.04.01 for ITS 233083
bool AppMediaPlayer_Video_ScreenTextButtonPopup::checkIsSamePopup()
{
    if(m_popUpType == POPUP_TXT_BTN_UNAVAILABLE_TRACK || m_popUpType == POPUP_TXT_BTN_DRIVING_REGULATION)
    {
        AppMediaPlayer_Video_ScreenManager* oppositeScreenManager = m_pEngineVideo->getScreenManager((VIDEO_DISPLAY_T)m_disp == 0 ? 1 : 0);
        if(oppositeScreenManager->topScreenName() == AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON)
        {
            AppMediaPlayer_Video_ScreenTextButtonPopup* popup = dynamic_cast<AppMediaPlayer_Video_ScreenTextButtonPopup*>(oppositeScreenManager->topScreen());
            if(popup && popup->popupType() == m_popUpType)
            {
                return true;
            }
        }

        return false;
    }
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    else if(m_popUpType == POPUP_TXT_BTN_HIGH_TEMPERATURE)
    {
        // Disc High Temperature popup: should be opened or closed together on both display, return TRUE
        return true;
    }
    // } added by cychoi 2014.04.09
    else
    {
        // Jukebox/USB error popup: should be opened or closed together on both display, return TRUE
        return true;
    }
}
// } added by cychoi 2014.04.01

void AppMediaPlayer_Video_ScreenTextButtonPopup::onSoftkeyBtn(QString button)
{
    MP_LOG << "button =" << button << LOG_ENDL;

    switch (m_popUpType)
    {
        case POPUP_TXT_BTN_COPY_EXISTED_FILE:
        {
            if(button == "btn1")
            {
                if(m_pControllerFS)
                    m_pControllerFS->overwriteCopyOperation();
                replaceScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_PROGRESS,
                              QVariant(m_pController->getSource()));
            }
            else if (button == "btn2")
            {
                m_pController->setCopyingStatus(false);
                if(m_pControllerFS)
                    m_pControllerFS->cancelCopyOperation();

                // { modified by Sergey for CR#11607
                QVariant msgPopUp;
                msgPopUp.setValue( POPUP_TOAST_COPY_CANCELED );
                replaceScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_TOAST, msgPopUp);
                // } modified by Sergey for CR#11607
            }
            break;
        }
        // { added by wspark 2012.11.20 for CR15380
        case POPUP_TXT_BTN_CAPACITY_ERROR:
        {
            m_pController->setCopyingStatus(false);
            if(m_pControllerFS)
                m_pControllerFS->cancelCopyOperation();
            closeScreen();
            if(button == "btn1")
            {
                // nothing to do.
            }
            else if(button == "btn2") // going to jukebox file manager.
            {
                if(m_pCModel)
                    m_pEngineVideo->launchFileManager(MEDIA_SOURCE_JUKEBOX,
                                                      m_pCModel->filename(),
                                                      m_pCModel->fullfoldername() , m_disp); //modified by edo.lee 2013.05.02
            }
            break;
        }
        // } added by wspark
        case POPUP_TXT_BTN_DRM_CONFIRM:
        {
            closeScreen();
            if(button == "btn1") // ok
            {
                if(m_pControllerFS)
                    m_pControllerFS->confirmDRMPlay(); // added by Sergey for DRM 29.07.2013

            } 
            else if(button == "btn2") // cancel play
            {
                if(m_pEngineVideo->playRequester() == PLAY_REQUSTER_FM)
                {
                    if(m_pCModel)
                        m_pEngineVideo->launchFileManager(m_pController->getSource(),
                                                          m_pCModel->filename(),
                                                          m_pCModel->fullfoldername() , m_disp); //modified by edo.lee 2013.05.02
                }
                if(m_pControllerFS)
                    m_pControllerFS->restoreLastPlay();
            }
            break;
        }
        case POPUP_TXT_BTN_UNSUPPORTED_FILE:
        case POPUP_TXT_BTN_AUDIO_FORMAT_UNSUPPORTED:
        case POPUP_TXT_BTN_RESOLUTION_UNSUPPORTED: // added by Sergey 10.11.2013 new DivX popups
        case POPUP_TXT_BTN_DRM_EXPIRED:
        case POPUP_TXT_BTN_DRM_NOT_AUTH:
        {
            closeScreen();
            if(m_pEngineVideo->playRequester() == PLAY_REQUSTER_FM)
            {
                if(m_pCModel)
                {
                    m_pEngineVideo->launchFileManager(m_pController->getSource(),
                                                      m_pCModel->filename(),
                                                      m_pCModel->fullfoldername() , m_disp); //modified by edo.lee 2013.05.02
                    m_pEngineVideo->setVideoListView((VIDEO_DISPLAY_T)m_disp, true); // modified for ISV 91102
                }
            } 
            if(m_pControllerFS)
                m_pControllerFS->restoreLastPlay();
            break;
        }
        // { added by wspark 2012.12.17 for ISV64433
        case POPUP_TXT_BTN_NO_VIDEO_ON_JUKEBOX:
        {
            closeScreen();
            m_pEngineVideo->HandleBackKey();
            break;
        }
        // } added by wspark
        // { added by wspark 2013.03.14 for ITS 136262
        case POPUP_TXT_BTN_UNAVAILABLE_TRACK:
        {
            closeScreen();
            // commented by cychoi 2013.06.24 for UX check result by UI Team
            //if(AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS == m_pEngineVideo->getScreenManager(m_disp)->topScreenName())
            //{
            //    AppMediaPlayer_Video_ScreenVCDDirectAccess *pVcdDirectAccess;
            //    pVcdDirectAccess = dynamic_cast<AppMediaPlayer_Video_ScreenVCDDirectAccess*>(m_pEngineVideo->getScreenManager(m_disp)->topScreen());
            //    if(pVcdDirectAccess)
            //        pVcdDirectAccess->onSoftkeyBack();
            //}
            // } commented by cychoi 2013.06.24
            break; // modified by ravikanth 16-04-13
        }
        // } added by wspark
		// { modified by ravikanth 16-04-13
        case POPUP_TXT_BTN_COPY_INPROGRESS:
        {
            closeScreen();
            if(button == "btn1")
            {
                m_pEngineVideo->CancelCopyJukebox();
                m_pEngineVideo->setCopy(true);
                m_pEngineVideo->launchFileManager( m_pController->getSource(),
                                                  m_pCModel->filename(),
                                                  m_pCModel->fullfoldername()  , m_disp); //modified by edo.lee 2013.05.02
                m_pEngineVideo->setVideoListView((VIDEO_DISPLAY_T)m_disp,true); //modified by edo.lee 2013.05.29
            }
            else if(button == "btn2")
            {
                // nothing to do.
            }
            break;
        }
		// } modified by ravikanth 16-04-13
        // { added by lssanh 2013.04.19 ITS158191
        case POPUP_TXT_BTN_UNAVAILABLE_LANG_CODE:
        case POPUP_TXT_BTN_DRIVING_REGULATION: // added by cychoi 2014.02.20 for (HMC Request)
        {
            //if (button == "btn1")  //comment by edo.lee 2013.12.13
                closeScreen();
			//go to pre-previouse screen
            //m_pEngineVideo->getScreenManager(m_disp)->changeScreen(m_pEngineVideo->getScreenManager(m_disp)->getPreviousScreenName(2)); // commented by cychoi 2013.06.24 for new DVD Settings UX
            break;            
        }
        // } added by lssanh 2013.04.19 ITS158191
        // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
        case POPUP_TXT_BTN_HIGH_TEMPERATURE:
        {
            closeScreen();
            m_pEngineVideo->getEngineMain().sendDeckErrorToUISH();
            break;            
        }
        // } added by cychoi 2014.04.09
        default:
        {
            //if (button == "btn1") //comment by edo.lee 2013.12.13
                closeScreen();
            break;
        }
    }
}


// { modified by Sergey 26.10.2013 for ITS#198721
void AppMediaPlayer_Video_ScreenTextButtonPopup::closeScreen()
{
    if ( m_pTimer->isActive() )
        m_pTimer->stop();

//	//modified by edo.lee 2013.08.29
//    if(m_pEngineVideo->getIsShowLockoutView() == VIDEO_DISPLAY_MAX)
//        emit popScreen();
//	else
//		m_pEngineVideo->onPopScreen();
//	//modified by edo.lee 2013.08.29

    // { modified by cychoi 2014.04.01 for ITS 233083
    if(checkIsSamePopup() == false)
    {
        emit popScreen();
    }
    else
    {
        m_pEngineVideo->popScreenFromBoth(m_myName);
    }
    // } modified by cychoi 2014.04.01
}
// } modified by Sergey 26.10.2013 for ITS#198721


// { added by lssanh 2013.02.05 ISV72351
void AppMediaPlayer_Video_ScreenTextButtonPopup::onBackKeyPressed()
{
    MP_LOG << LOG_ENDL;
    if (m_popUpType == POPUP_TXT_BTN_COPY_EXISTED_FILE)
    {
        m_pController->setCopyingStatus(false);
        if(m_pControllerFS)
            m_pControllerFS->cancelCopyOperation();

        QVariant msgPopUp;
        msgPopUp.setValue( POPUP_TOAST_COPY_CANCELED );
        replaceScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_TOAST, msgPopUp);
    }
    else if(m_popUpType == POPUP_TXT_BTN_COPY_INPROGRESS ||
            m_popUpType == POPUP_TXT_BTN_UNAVAILABLE_LANG_CODE || // added by cychoi 2013.12.28 for popup close on Back HK
            m_popUpType == POPUP_TXT_BTN_DRIVING_REGULATION || // added by cychoi 2014.02.20 for (HMC Request)
            m_popUpType == POPUP_TXT_BTN_UNAVAILABLE_TRACK) // modified by cychoi 2013.08.30 for ISV 90247 // modified by ravikanth 04-07-13 for ISV 86338
    {
        closeScreen();
    }
    else if(m_popUpType == POPUP_TXT_BTN_UNSUPPORTED_FILE ||
            m_popUpType == POPUP_TXT_BTN_DRM_CONFIRM ||//added by taihyun.ahn for ITS 217522 2012.12.27
            m_popUpType == POPUP_TXT_BTN_DRM_EXPIRED ||
            m_popUpType == POPUP_TXT_BTN_DRM_NOT_AUTH)
    {
        closeScreen();
        m_pControllerFS->skip();
    }
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    else if(m_popUpType == POPUP_TXT_BTN_HIGH_TEMPERATURE)
    {
        closeScreen();
        m_pEngineVideo->getEngineMain().sendDeckErrorToUISH();
    }
    // } added by cychoi 2014.04.09
}
// } added by lssanh 2013.02.05 ISV72351

void AppMediaPlayer_Video_ScreenTextButtonPopup::initMessage()
{
    if (false == m_messagePopUp.isEmpty())
        m_messagePopUp.clear();

	//{modified by Michael.Kim 2013.06.25 for ISV Issue 85785
    m_messagePopUp.insert(POPUP_TXT_BTN_UNSUPPORTED_FILE, QT_TR_NOOP("STR_MEDIA_UNAVAILABLE_FILE")); // modified by kihyung 2013.11.01 for ITS 0199072 
    m_messagePopUp.insert(POPUP_TXT_BTN_NO_VIDEO_ON_USB, QT_TR_NOOP("STR_MEDIA_USB_NO_VIDEO"));
    m_messagePopUp.insert(POPUP_TXT_BTN_NO_VIDEO_ON_JUKEBOX, QT_TR_NOOP("STR_MEDIA_JUKEBOX_NO_VIDEO"));
    m_messagePopUp.insert(POPUP_TXT_BTN_UNAVAILABLE_TRACK, QT_TR_NOOP("STR_MEDIA_UNAVAILABLE_TRACK"));
    m_messagePopUp.insert(POPUP_TXT_BTN_COPY_EXISTED_FILE, QT_TR_NOOP("STR_MEDIA_MNG_FILE_COPY_REPLACE"));
    m_messagePopUp.insert(POPUP_TXT_BTN_DRM_CONFIRM, QT_TR_NOOP("STR_MEDIA_RENTAL_CONFIRM"));
    m_messagePopUp.insert(POPUP_TXT_BTN_DRM_EXPIRED, QT_TR_NOOP("STR_MEDIA_RENTAL_INFO"));
    m_messagePopUp.insert(POPUP_TXT_BTN_DRM_NOT_AUTH, QT_TR_NOOP("STR_SETTING_DIVX_AUTHORIZATION")); // modified by jeeeun 13-08-14 
    m_messagePopUp.insert(POPUP_TXT_BTN_CAPACITY_ERROR, QT_TR_NOOP("STR_MEDIA_MNG_FILE_COPY_NO_FREE_SPACE")); // added by wspark 2012.11.20 for CR15380
    m_messagePopUp.insert(POPUP_TXT_BTN_UNAVAILABLE_LANG_CODE, QT_TR_NOOP("STR_MEDIA_UNAVAILABLE_LANG_CODE")); // modified by ravikanth 13-03-13
    m_messagePopUp.insert(POPUP_TXT_BTN_COPY_INPROGRESS,QT_TR_NOOP("STR_MEDIA_CANCEL_COPY_TO_JUKEBOX")); // modified by ravikanth 16-04-13
    m_messagePopUp.insert(POPUP_TXT_BTN_UNSUPPORTED_ALL,QT_TR_NOOP("STR_MEDIA_UNAVAILABLE_FILE_ALL")); // modified by kihyung 2013.09.12 for ISV 90605
    m_messagePopUp.insert(POPUP_TXT_BTN_AUDIO_FORMAT_UNSUPPORTED,QT_TR_NOOP("STR_MEDIA_UNAVAILABLE_AUDIO_FORMAT"));
    m_messagePopUp.insert(POPUP_TXT_BTN_RESOLUTION_UNSUPPORTED,QT_TR_NOOP("STR_MEDIA_UNAVAILABLE_RESOLUTION")); // added by Sergey 10.11.2013 new DivX popups
    m_messagePopUp.insert(POPUP_TXT_BTN_DRIVING_REGULATION, QT_TR_NOOP("STR_MEDIA_DISC_VCD_DRIVING_REGULATION")); // added by cychoi 2014.02.20 for (HMC Request)
    m_messagePopUp.insert(POPUP_TXT_BTN_HIGH_TEMPERATURE, QT_TR_NOOP("STR_MEDIA_STOP_FUNCTION")); // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    m_messagePopUp.insert(POPUP_TXT_BTN_MAX, "");
	//modified by Michael.Kim 2013.06.25 for ISV Issue 85785
}
// } modified by Sergey for CR#15744, 16015, 16777.
void AppMediaPlayer_Video_ScreenTextButtonPopup::onLockoutMode(bool on)
{
   MP_LOG << "on =" << on << LOG_ENDL;

   //m_pModel->setLockoutMode(on);
   m_isLockout = on;
//   emit showLockout(on); // commented by Sergey 27.10.2013 for ITS#198669
   // { modified by cychoi 2014.01.09 for ITS 218954 DRS Off on Popup
   if(m_popUpType == POPUP_TXT_BTN_UNAVAILABLE_TRACK)
   {
       notifyScreenManager(m_myName, AppMediaPlayer_Video_Screens::SCREEN_VCD_DIRECTACCESS, EVENTS_DRS_MODE, QVariant(on)); // added by Sergey 27.10.2013 for ITS#198669
   }
   else
   {
       notifyScreenManager(m_myName, m_pEngineVideo->getScreenManager((VIDEO_DISPLAY_T)m_disp)->firstScreen(), EVENTS_DRS_MODE, QVariant(on)); // added by Sergey 27.10.2013 for ITS#198669
   }
   // } modified by cychoi 2014.01.09
}


// { added by Sergey for DRM 30.07.2013
// Strings with additional runtime arguments should be translated here.
void AppMediaPlayer_Video_ScreenTextButtonPopup::onRetranslateUI()
{
    if(this != dynamic_cast<AppMediaPlayer_Video_ScreenTextButtonPopup*>(m_pEngineVideo->getScreenManager(m_disp)->topScreen()))
        return;

    QString text = QApplication::translate("main", m_messagePopUp.value(m_popUpType,"").toAscii());

    switch(m_popUpType)
    {
        case POPUP_TXT_BTN_COPY_EXISTED_FILE:
        {
            if(m_pControllerFS)
            {
                text = text.arg(m_pControllerFS->getCopyFileName());
            }

            break;
        }
        case POPUP_TXT_BTN_DRM_CONFIRM:
        {
            if(m_pCModel)
                text = text.arg(m_pCModel->drmUseCount()).arg(m_pCModel->drmUseLimit());

            break;
        }
        case POPUP_TXT_BTN_DRM_EXPIRED:
        {
            if(m_pCModel)
                text = text.arg(m_pCModel->drmUseLimit());

            break;
        }
        case POPUP_TXT_BTN_CAPACITY_ERROR:
        {
            if(m_pControllerFS)
                text = text.arg(m_pControllerFS->getFilesCount()).arg(m_pControllerFS->getFilesSize());

            break;
        }

        default: // make compiler happy
            return;
            break;
    }

    m_pModel->setSText(text);
}
// } added by Sergey for DRM 30.07.2013
