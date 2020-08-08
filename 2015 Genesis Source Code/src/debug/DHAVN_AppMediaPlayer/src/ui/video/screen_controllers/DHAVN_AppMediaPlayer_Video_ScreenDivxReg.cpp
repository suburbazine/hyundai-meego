// { modified by Sergey 25.10.2013 for new DivX

#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDivxReg.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Divx.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"


ScreenDivxReg::
ScreenDivxReg(AppMediaPlayer_EngineVideo *pEngineVideo, int disp, QObject *parent)
    : AppMediaPlayer_Video_Screen( parent ),
      m_pEngineVideo(pEngineVideo),
      m_disp(disp),//added by edo.lee 2013.07.30 subscreen
      m_bInitScreen(false)
{
    m_myName = AppMediaPlayer_Video_Screens::SCREEN_DIVXREG;
    m_pModel = new AppMediaPlayer_Video_SModel_Divx(this);
    setModel(m_pModel);
    m_overlays << AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA;
    m_pController = m_pEngineVideo->GetVideoController();

}

ScreenDivxReg::~ScreenDivxReg()
{
    delete m_pModel;
}


void ScreenDivxReg::onHide()
{
	//m_pEngineVideo->HideSubScreenOnVideo(m_disp);//added by edo.lee 2013.08.01 new entry for subscreen 
	//m_pEngineVideo->getEngineMain().setSubCreenOn(false , AppMediaPlayer_Video_Screens::SCREEN_DIVXREG);//added by edo.lee 2013.07.30 subscreen
}

void ScreenDivxReg::onShow(QVariant arg)
{
    Q_UNUSED(arg);


	//m_pEngineVideo->getEngineMain().setSubCreenOn(true ,  AppMediaPlayer_Video_Screens::SCREEN_DIVXREG);//added by edo.lee 2013.07.30 subscreen
    setupModeArea();
    setupDivxMode();


	if(m_bInitScreen)
	{
		m_isModeAreaFocused = false; // added by Sergey 10.01.2014 for ITS#218955
		emit setDefaultFocus();
		m_pEngineVideo->notifyBothScreens(m_myName, AppMediaPlayer_Video_Screens::SCREEN_DIVXREG, EVENTS_INIT_SCREEN, QVariant(false));
	}
    m_pEngineVideo->GetVideoController()->clearDisplay(true);//added by taihyun.ahn for ITS 216590
}

void ScreenDivxReg::setupDivxMode()
{
    registrationState state = m_pController->drmRegistrationStatus();

    switch(state)
    {
        case SETTINGS_DRM_NOT_REGISTERED:
        case SETTINGS_DRM_NEVER_REGISTERED:
        case SETTINGS_DRM_GENERAL_ERROR:
        {
        	// { modified by Sergey 15.11.2013 for new DivX spec
            //            if(m_pController->divxScreenMode() == DIVX_STATE_REGISTRATION_CODE)
            //            {
            //                m_pController->recieveRegistrationCode();
            //                m_pModel->setRegCode(m_pController->drmRegistrationCode());
            //                m_pModel->setScrState("RegistrationCode");
            //            }
            //            else
			//            if(m_pController->divxScreenMode() == DIVX_STATE_DEREGISTRATION_CODE)
        if(m_pModel->scrState() == "RegistrationCode" && m_pEngineVideo->getEngineMain().isDisplayOff())
        {
            m_pController->recieveRegistrationCode();
            m_pModel->setRegCode(m_pController->drmRegistrationCode());
            m_pController->setDivxScreenMode(DIVX_STATE_REGISTRATION_CODE);
            m_pModel->setScrState("RegistrationCode");
        }
        else
        {
            m_pController->recieveDeregistrationCode();
            m_pController->setDivxScreenMode(DIVX_STATE_DEREGISTRATION_CODE);
            m_pModel->setDeregCode(m_pController->drmDeregistrationCode());
            m_pModel->setScrState("DeregistrationCode");
        }
            // } modified by Sergey 15.11.2013 for new DivX spec

            break;
        }
        case SETTINGS_DRM_SUCCESS:
        {
            m_pController->setDivxScreenMode(DIVX_STATE_ALREADY_REGISTERED);
            m_pModel->setScrState("AlreadyRegistered");

            break;
        }
        default:
        {
            MP_LOG << "not supported state =" << state << LOG_ENDL;
            break;
        }
    }
}


void ScreenDivxReg::onButtonClicked(QString btn)
{
    MP_LOG << "mode = " << m_pController->divxScreenMode() << LOG_ENDL;

    switch(m_pController->divxScreenMode())
    {
        case DIVX_STATE_REGISTRATION_CODE:
        {
            //only OK button in this screen
			closeScreen();
			
            break;
        }
        case DIVX_STATE_ALREADY_REGISTERED:
        {
            if(btn == "OK")
            {
                m_pController->recieveDeregistrationCode();
                m_pModel->setDeregCode(m_pController->drmDeregistrationCode());
                m_pController->setDivxScreenMode(DIVX_STATE_DEREGISTRATION_CODE);
                m_pModel->setScrState("DeregistrationCode");
            }
            else if(btn == "CANCEL")
            {
                closeScreen();
            }
			
            break;
        }
        case DIVX_STATE_DEREGISTRATION_CODE:
        {
            if(btn == "OK")
            {
                m_pController->recieveRegistrationCode();
                m_pModel->setRegCode(m_pController->drmRegistrationCode());
                m_pController->setDivxScreenMode(DIVX_STATE_REGISTRATION_CODE);
                m_pModel->setScrState("RegistrationCode");
            }
            else if(btn == "CANCEL")
            {
                closeScreen();
            }
			
            break;
        }
        default:
        {
            MP_LOG << "unsupported case" << LOG_ENDL;
            break;
        }
    }
}

void ScreenDivxReg::closeScreen()
{
    //emit popScreen();//remove by edo.lee 2013.07.30 subscreen
    m_pEngineVideo->backHandle(m_disp==0,false);//added by edo.lee 2013.07.31 back event on DRS
}

void ScreenDivxReg::setupModeArea()
{
    QString title = QT_TR_NOOP("STR_MEDIA_DIVX_SETTINGS"); // modified by Sergey 25.08.2013 for ITS#186435


    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DIVXREG, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_TEXT, QVariant(title));

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DIVXREG, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_LIST_BTN_VISIBLE, QVariant(false));

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DIVXREG, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_MENU_BTN_VISIBLE, QVariant(false));

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DIVXREG, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_COUNTER_VISIBLE, QVariant(false));
    //{ added by cychoi 2013.08.14 for ITS 184421
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DIVXREG, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_RTEXT_VISIBLE, QVariant(false));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DIVXREG, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_ICON_VISIBLE, QVariant(false));
    //} added by cychoi 2013.08.14
}

// } modified by Sergey 30.07.2013


void  ScreenDivxReg::onEventReceived(const QString &srcScreenName,
                                     const QString &resScreenName,
                                     const int eventId,
                                     const QVariant arg)
{
    Q_UNUSED(srcScreenName);
    Q_UNUSED(arg);


    MP_HIGH << eventId << "to =" << resScreenName << LOG_ENDL;

    if(resScreenName == m_myName)
    {
        switch( eventId )
        {
            case EVENTS_INIT_SCREEN:
            {
                m_bInitScreen = arg.toBool();
                break;
            }

            default:
                break;
        }
    }
}

// } modified by Sergey 25.10.2013 for new DivX
