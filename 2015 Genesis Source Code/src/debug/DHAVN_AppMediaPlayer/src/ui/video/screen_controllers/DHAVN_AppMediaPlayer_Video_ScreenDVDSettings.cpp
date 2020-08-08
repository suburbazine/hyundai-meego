#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerDVD.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h" // modified by ravikanth CR 15994 12-11-22
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_DVD.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDVDSettings.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_TitlesChapters.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/video/DHAVN_AppMediaPlayer_VideoSettings.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_DVDSettings.h" // modified by ravikanth 13-03-13
#include "util/video/DHAVN_AppMediaPlayer_RadioListModel.h"

AppMediaPlayer_Video_ScreenDVDSettings::
AppMediaPlayer_Video_ScreenDVDSettings( AppMediaPlayer_Video_Controller *pController,
                                       AppMediaPlayer_EngineVideo *pEngineVideo,
                                       int disp, // added by cychoi 2013.08.20 for ITS 178895 SerachCaption subscreen
                                       QObject *parent,
                                       QString viewName )
    : AppMediaPlayer_Video_Screen( parent ),
      m_pController( pController ),
      m_pEngineVideo( pEngineVideo ),
      m_disp(disp) // added by cychoi 2013.08.20 for ITS 178895 SerachCaption subscreen
{
    m_pControllerDVD = static_cast<AppMediaPlayer_Video_ControllerDVD*>(m_pController->getController(MEDIA_SOURCE_DVDVIDEO));
    // { modified by ravikanth CR 15994 12-11-22
    m_pControllerJukeBox = static_cast<AppMediaPlayer_Video_ControllerFS*>(m_pController->getController(MEDIA_SOURCE_JUKEBOX));
    m_pControllerUSB1 = static_cast<AppMediaPlayer_Video_ControllerFS*>(m_pController->getController(MEDIA_SOURCE_USB1));
    m_pControllerUSB2 = static_cast<AppMediaPlayer_Video_ControllerFS*>(m_pController->getController(MEDIA_SOURCE_USB2));
	// } modified by ravikanth CR 15994 12-11-22

    m_viewName = viewName;
	
	// { modified by ravikanth 13-03-13
    m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(m_pControllerDVD->getModel());
	
    m_pModel = new DVDSettingsSModel(m_pControllerModel, this, m_pEngineVideo); // modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    m_pModel->bindWith(m_pControllerModel);
    setModel(m_pModel);
	// } modified by ravikanth 13-03-13

	 m_overlays << AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA; // added by Sergey 16.05.2013

    m_pSettingsStorage = AppMediaPlayer_VideoSettings::GetInstance();

	// { modified by Sergey for CR#15775
    connect(m_pEngineVideo, SIGNAL(vcItemSelected(VCItem*)),
            this, SLOT(handleVCItem(VCItem*)));
	// } modified by Sergey for CR#15775
    // { added by cychoi 2013.12.04 for ITS 212522
    connect(&m_pEngineVideo->getEngineMain(), SIGNAL(retranslateUi(QVariant)),
            this, SLOT(onRetranslateUI()));
    // } added by cychoi 2013.12.04

	//added by edo.lee 2013.07.15 STATIC TEST
	m_lastOSDDisplay = DISPLAY_FRONT_REAR;
	m_lastCloneTemp = false;
	//added by edo.lee 2013.07.15 STATIC TEST	
}


AppMediaPlayer_Video_ScreenDVDSettings::~AppMediaPlayer_Video_ScreenDVDSettings()
{
    // { added by cychoi 2013.06.24 for new DVD Settings UX
    if(m_viewName == AppMediaPlayer_Video_Screens::VIEW_DVD_SETTINGS)
        m_pControllerDVD->setDVDSettingsLanguages();
    // } added by cychoi 2013.06.24 for new DVD Settings UX

    delete m_pModel; // modified by ravikanth 13-03-13

    AppMediaPlayer_VideoSettings::FreeInstance();
}


QString AppMediaPlayer_Video_ScreenDVDSettings::viewName()
{
    return  m_viewName;
}


void AppMediaPlayer_Video_ScreenDVDSettings::onHide()
{
    MP_LOG << LOG_ENDL;
	//added by edo.lee 2013.05.20
	notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_NORMAL, QVariant());
	//m_pEngineVideo->getEngineMain().setSubCreenOn(false ,  AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS);//added by edo.lee 2013.07.30 subscreen
	m_pEngineVideo->getEngineMain().setPlayInfoDisplay(m_lastOSDDisplay);//added by edo.lee 2013.04.26
	m_pEngineVideo->getEngineMain().setIsCloneTemp(m_lastCloneTemp);//added by edo.lee 2013.06.07
    // { modified by sjhyun 2013.09.11 for ITS 189090
    // { added by cychoi 2013.06.24 for new DVD Settings UX
    QString subScreenName = m_pEngineVideo->getEngineMain().getSubCreenName();
    if(m_viewName == AppMediaPlayer_Video_Screens::VIEW_DVD_SETTINGS &&
       subScreenName != AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS && // modified by cychoi 2014.01.10 for ITS 219020
       subScreenName != AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION)
    {
        m_pControllerDVD->setDVDSettingsLanguages();
    }
    // } added by cychoi 2013.06.24 for new DVD Settings UX
    // } modified by sjhyun 2013.09.11

    m_pEngineVideo->getEngineMain().setVideoSubScreen(m_disp == 0, false); // added by wspark 2014.02.15 for ITS 225374
}

// { added by Sergey 16.05.2013
void AppMediaPlayer_Video_ScreenDVDSettings::onShow(QVariant arg)
{
    QString source = arg.toString();
    if(source == "DISC")
    {
        m_pControllerDVD->requestDefaultLanguagesSet();
        onTabSelected(m_pModel->activeTab());
        emit setDVDSettingUI();
    }

	//m_pEngineVideo->getEngineMain().setSubCreenOn(true ,  AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS);//added by edo.lee 2013.07.30 subscreen
	m_lastOSDDisplay = m_pEngineVideo->getEngineMain().getPlayInfoDisplay();//added by edo.lee 2013.04.26
	m_pEngineVideo->getEngineMain().setPlayInfoDisplay(DISPLAY_FRONT_REAR);//added by edo.lee 2013.04.26
	m_lastCloneTemp = m_pEngineVideo->getEngineMain().getIsCloneTemp();//added by edo.lee 2013.06.07
	m_pEngineVideo->getEngineMain().setIsCloneTemp(false);//added by edo.lee 2013.06.07
	setupModeArea(source);
	
    m_pEngineVideo->getEngineMain().setVideoSubScreen(m_disp == 0, true); // added by wspark 2014.02.15 for ITS 225374
}
// } added by Sergey 16.05.2013


// { added by ravikanth	- CR 11552, 11938
void AppMediaPlayer_Video_ScreenDVDSettings::handleVCItem(VCItem *item)
{
    MP_LOG << "item type =" << item->infoType << LOG_ENDL;
    MP_LOG << "item code =" << item->code << LOG_ENDL; // modified by ravikanth CR 15994 12-11-22

    switch(item->infoType)
    {
        case VC_DATA_CAPTIONS:
        {
		// { modified by ravikanth CR 15994 12-11-22
            m_pControllerDVD->setSubtitleLanguage(item->code);
		// } modified by ravikanth CR 15994 12-11-22
            break;
        }
        case VC_DATA_VOICELANG:
        {
            m_pControllerDVD->setAudioLanguage(item->code); // modified by ravikanth CR 15994 12-11-22
            break;
        }
		// { modified by ravikanth CR 15994 12-11-22
        case VC_DATA_MENULANG:
        {
            m_pControllerDVD->setMenuLanguage(item->code);
            break;
        }
		// } modified by ravikanth CR 15994 12-11-22
        default:
        {
           break;
        }
    }
}
// } added by ravikanth

// { modified by ravikanth 13-03-13
void AppMediaPlayer_Video_ScreenDVDSettings::onSoftkeyBack()
{
    MP_LOG << LOG_ENDL;
    emit popScreen();
}

void AppMediaPlayer_Video_ScreenDVDSettings::radioItemSelected(int index)
{
//    MP_LOG << "index =" << index << "active tab =" << m_pModel->activeTab() << LOG_ENDL;

    switch(m_pModel->activeTab())
    {
    case TAB_CAPTION:
        m_pModel->setActiveCaption(index);
        captionSelected(index);
        break;
    case TAB_AUDIO:
        m_pModel->setActiveAudioLang(index);
        voiceLangSelected(index);
        break;
    case TAB_MENUL:
        m_pModel->setActiveMenuLang(index);
        menuLangSelected(index);
        break;
    default: // make compiler happy
        break;
    }
        //emit popScreen();// added by lssanh 2013.04.27 ITS165097 // deleted by wspark 2013.05.01 for rollback ITS 165097
}

void AppMediaPlayer_Video_ScreenDVDSettings::onTabSelected(int index)
{
    MP_LOG << "index =" << index << LOG_ENDL;
    m_pModel->setActiveTab(index);
}

void AppMediaPlayer_Video_ScreenDVDSettings::captionSelected(int index)
{

    if(m_pModel->getCaptionList()->at(index) == tr("STR_MEDIA_OTHERS"))
    {
        //m_pEngine->LaunchSearchScreen(m_infoType);
//        QVariant activeTab;
//        activeTab.setValue( m_pModel->activeTab());
        // { modified by cychoi 2013.08.20 for ITS 178895 SerachCaption subscreen
        // { added by sjhyun 2013.09.11 for ITS 189090
        m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS,
                            AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION,
                            EVENTS_SET_DVD_ACTIVE_TAP,
                            QVariant(m_pModel->activeTab()));
        // } added by sjhyun 2013.09.11
        m_pEngineVideo->onShowSettings("DVD_SearchCaption", QVariant(m_pModel->activeTab()));
        m_pEngineVideo->LaunchSubScreenOnVideo(VIDEO_DVD_SETTING, m_disp);
        //emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION, QVariant(m_pModel->activeTab()));
        // } modified by cychoi 2013.08.20
    }
    else
    {
//        m_pEngine->SendDvdDataToAVP( m_infoType, m_defaultLangCodes.at(index));

        // { modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add // modified by yungi 2013.10.19 for ITS 196375
        switch(m_pEngineVideo->getEngineMain().GetLanguage())
        {
            case LANGUAGE_KO:
            {
                m_pControllerDVD->setSubtitleLanguage(m_pControllerModel->domesticLanguageCodes().at(index));
                break;
            }

            case LANGUAGE_ZH_CN:
            case LANGUAGE_ZH_YUE:
            {
                m_pControllerDVD->setSubtitleLanguage(m_pControllerModel->chineseLanguageCodes().at(index));
                break;
            }

            case LANGUAGE_AR:
            {
                m_pControllerDVD->setSubtitleLanguage(m_pControllerModel->arabLanguageCodes().at(index));
                break;
            }

            default:
            {
                m_pControllerDVD->setSubtitleLanguage(m_pControllerModel->defaulLanguageCodes().at(index));
                break;
            }
        }

        // } modified by yungi
    }
}

void AppMediaPlayer_Video_ScreenDVDSettings::voiceLangSelected(int index)
{
    if(m_pModel->getAudioLangList()->at(index) == tr("STR_MEDIA_OTHERS"))
    {
        //m_pEngine->LaunchSearchScreen(m_infoType);
//        QVariant activeTab;
//        activeTab.setValue( m_pModel->activeTab());
        // { modified by cychoi 2013.08.20 for ITS 178895 SerachCaption subscreen
        // { added by sjhyun 2013.09.11 for ITS 189090
        m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS,
                            AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION,
                            EVENTS_SET_DVD_ACTIVE_TAP,
                            QVariant(m_pModel->activeTab()));
        // } added by sjhyun 2013.09.11
        m_pEngineVideo->onShowSettings("DVD_SearchCaption", QVariant(m_pModel->activeTab()));
        m_pEngineVideo->LaunchSubScreenOnVideo(VIDEO_DVD_SETTING, m_disp);
        //emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION, QVariant(m_pModel->activeTab()));
        // } modified by cychoi 2013.08.20
    }
    else
    {
//        m_pEngine->SendDvdDataToAVP( m_infoType, m_defaultLangCodes.at(index + 1));

        // { modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add  // modified by yungi 2013.10.19 for ITS 196375
        switch(m_pEngineVideo->getEngineMain().GetLanguage())
        {
            case LANGUAGE_KO:
            {
                m_pControllerDVD->setAudioLanguage(m_pControllerModel->domesticLanguageCodes().at(index + 1));
                break;
            }

            case LANGUAGE_ZH_CN:
            case LANGUAGE_ZH_YUE:
            {
                m_pControllerDVD->setAudioLanguage(m_pControllerModel->chineseLanguageCodes().at(index + 1));
                break;
            }

            case LANGUAGE_AR:
            {
                m_pControllerDVD->setAudioLanguage(m_pControllerModel->arabLanguageCodes().at(index + 1));
                break;
            }

            default:
            {
                m_pControllerDVD->setAudioLanguage(m_pControllerModel->defaulLanguageCodes().at(index + 1));
                break;
            }
        }
        // } modified by yungi
    }
}

void AppMediaPlayer_Video_ScreenDVDSettings::menuLangSelected(int index)
{
    if(m_pModel->getMenuLangList()->at(index) == tr("STR_MEDIA_OTHERS"))
    {
        //m_pEngine->LaunchSearchScreen(m_infoType);
//        QVariant activeTab;
//        activeTab.setValue( m_pModel->activeTab());
        // { modified by cychoi 2013.08.20 for ITS 178895 SerachCaption subscreen
        // { added by sjhyun 2013.09.11 for ITS 189090
        m_pEngineVideo->getScreenManager(m_disp)->onNotifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS,
                            AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION,
                            EVENTS_SET_DVD_ACTIVE_TAP,
                            QVariant(m_pModel->activeTab()));
        // } added by sjhyun 2013.09.11
        m_pEngineVideo->onShowSettings("DVD_SearchCaption", QVariant(m_pModel->activeTab()));
        m_pEngineVideo->LaunchSubScreenOnVideo(VIDEO_DVD_SETTING, m_disp);
        //emit pushScreen(AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION, QVariant(m_pModel->activeTab()));
        // } modified by cychoi 2013.08.20
    }
    else
    {
//        m_pEngine->SendDvdDataToAVP( m_infoType, m_defaultLangCodes.at(index) );

        // { modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add  // modified by yungi 2013.10.19 for ITS 196375
        switch(m_pEngineVideo->getEngineMain().GetLanguage())
        {
            case LANGUAGE_KO:
            {
                m_pControllerDVD->setMenuLanguage(m_pControllerModel->domesticLanguageCodes().at(index + 1));
                break;
            }

            case LANGUAGE_ZH_CN:
            case LANGUAGE_ZH_YUE:
            {
                m_pControllerDVD->setMenuLanguage(m_pControllerModel->chineseLanguageCodes().at(index + 1));
                break;
            }

            case LANGUAGE_AR:
            {
                m_pControllerDVD->setMenuLanguage(m_pControllerModel->arabLanguageCodes().at(index + 1));
                break;
            }

            default:
            {
                m_pControllerDVD->setMenuLanguage(m_pControllerModel->defaulLanguageCodes().at(index + 1));
                break;
            }
        }
        // } modified by yungi
    }
}

void AppMediaPlayer_Video_ScreenDVDSettings::setupModeArea(QString source)
{
    QString title = "";

    if(source == "DISC")
        title = QT_TR_NOOP("STR_SETTING_DVD_SETTING"); // modified by Sergey 25.08.2013 for ITS#186435


    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_TEXT, QVariant(title));

	//added by edo.lee 2013.05.20
	notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_SETTING, QVariant());
    //{ added by cychoi 2013.08.14 for ITS 184421
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_RTEXT_VISIBLE, QVariant(false));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SETTINGS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_ICON_VISIBLE, QVariant(false));
    //} added by cychoi 2013.08.14
} // added by Sergey 16.05.2013

// { added by cychoi 2013.12.04 for ITS 212522
void AppMediaPlayer_Video_ScreenDVDSettings::onRetranslateUI()
{
    MP_LOG << LOG_ENDL;
    m_pModel->retranslateUI();
    m_pModel->activeLangCodeChanged(m_pControllerDVD->getSubtitleLanguage(),
                                    m_pControllerDVD->getAudioLanguage(),
                                    m_pControllerDVD->getMenuLanguage());
}
// } added by cychoi 2013.12.04

