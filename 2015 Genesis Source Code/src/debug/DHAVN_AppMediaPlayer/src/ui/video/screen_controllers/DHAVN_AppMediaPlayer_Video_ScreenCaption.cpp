// { modified by Sergey 20.08.2013 for ITS#184640 
#include "hdr/ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenCaption.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Caption.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/video/DHAVN_AppMediaPlayer_RadioListModel.h"
#include <QDeclarativeContext>
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h" // added by Sergey 23.10.2013 for ITS#196877

AppMediaPlayer_Video_ScreenCaption::
AppMediaPlayer_Video_ScreenCaption( AppMediaPlayer_Video_Controller *pController,
                                    AppMediaPlayer_EngineVideo *pEngineVideo,
                                   VIDEO_DISPLAY_T disp,
                                    QObject *parent )
   : AppMediaPlayer_Video_Screen( parent ),
     m_pController( pController ),
     m_pEngineVideo( pEngineVideo ),
     m_pControllerFS( NULL ),
     m_pControllerModel( NULL ),
     m_pModel( new AppMediaPlayer_Video_SModel_Caption(this)),
     m_disp(disp),
     m_bInitScreen(true) // added by Sergey 23.10.2013 for ITS#196877
{
    setModel( m_pModel );
    m_overlays << AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA; //added by aettie 20130520 Caption setting
    m_myName = AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS; // added by Sergey 23.10.2013 for ITS#196877

    m_pRadioListModel = new RadioListModel(this);
    m_FontSizeList.append(QT_TR_NOOP("STR_MEDIA_CAPTION_FONT_SIZE_SMALL"));
    m_FontSizeList.append(QT_TR_NOOP("STR_MEDIA_CAPTION_FONT_SIZE_NORMAL"));
    m_FontSizeList.append(QT_TR_NOOP("STR_MEDIA_CAPTION_FONT_SIZE_LARGE"));

    AppEngineQMLConstants::EAPP_SCREEN_USAGE_T screen = (m_disp == VIDEO_DISPLAY_FRONT) ? AppEngineQMLConstants::APP_FRONT_SCREEN : AppEngineQMLConstants::APP_REAR_SCREEN;
    QDeclarativeContext *context = m_pEngineVideo->getEngineMain().GetScreenContext(screen);
    context->setContextProperty("FSCaptionRadioListModel", m_pRadioListModel);
}


AppMediaPlayer_Video_ScreenCaption::~AppMediaPlayer_Video_ScreenCaption()
{
}


void AppMediaPlayer_Video_ScreenCaption::onSetEnabledCaption( bool enabledCaption )
{
   m_pControllerFS->enableSubTitle(enabledCaption);
}


void AppMediaPlayer_Video_ScreenCaption::onSoftkeyBack()
{
   emit popScreen();
}

void AppMediaPlayer_Video_ScreenCaption::onHide()
{
   MP_LOG << LOG_ENDL;

   disconnect(m_pControllerFS, 0, this, 0);
   disconnect(m_pControllerModel, 0, this, 0); // added by Sergey 09.10.2013 for ITS#187939

   m_pControllerFS = NULL;
   m_pControllerModel = NULL;
    //added by edo.lee 2013.05.20
    //m_pEngineVideo->getEngineMain().setSubCreenOn(false , AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS);//added by edo.lee 2013.07.30 subscreen
	notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_NORMAL, QVariant());
}


// { modified by Sergey 09.10.2013 for ITS#187939
void AppMediaPlayer_Video_ScreenCaption::onShow( QVariant arg )
{
	Q_UNUSED(arg);

    m_pControllerFS =  dynamic_cast<AppMediaPlayer_Video_ControllerFS*>(m_pController->getCurrentController());

    if(!m_pControllerFS)
    {
        popScreen(); // if controller is NULL then close this screen
        return;
    }

    m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pControllerFS->getModel());
    m_pModel->bindWith(m_pControllerModel);

    connect(m_pControllerFS, SIGNAL(durationChanged(int)), this, SLOT(onDurationChanged(int)));
    connect(m_pControllerModel, SIGNAL(captionCurIdChanged(int)), this, SLOT(onCaptionCurIdChanged(int)));


    if(m_pControllerModel->captionLangList().isEmpty())
    {
        QStringList list;
        list.append("Caption 1");
        m_pControllerModel->setCaptionLangList(list);
        m_pControllerModel->setCaptionCurId(0);
    }

	setupModeArea();


	// { added by Sergey 23.10.2013 for ITS#196877
	if(m_bInitScreen)
	{
		m_isModeAreaFocused = false; // added by Sergey 10.01.2014 for ITS#218955
		emit initScreen();
		emit setDefaultFocus();
		m_pEngineVideo->notifyBothScreens(m_myName, AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS, EVENTS_INIT_SCREEN, QVariant(false));
	}
	// } added by Sergey 23.10.2013 for ITS#196877
    m_pEngineVideo->GetVideoController()->clearDisplay(true);//added by taihyun.ahn for ITS 216590sss
}
// } modified by Sergey 09.10.2013 for ITS#187939


void AppMediaPlayer_Video_ScreenCaption::setupModeArea()
{
    QString title = QT_TR_NOOP("STR_MEDIA_CAPTION_SETTING"); // modified by Sergey 25.08.2013 for ITS#186435

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_TEXT, QVariant(title));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_SETTING, QVariant());
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_RTEXT_VISIBLE, QVariant(false));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_ICON_VISIBLE, QVariant(false));
} 


void AppMediaPlayer_Video_ScreenCaption::onMenuItem(int item)
{
    switch(item)
    {
    case 0: // View subtitles
    {
        m_pModel->setUiState("CAPTION_VIEW");

        break;
    }
    case 1: // Languages
    {
        m_pModel->setUiState("CAPTION_LANG");
        m_pRadioListModel->updateModel(m_pControllerModel->captionLangList(), (int)m_pControllerModel->captionCurId());

        break;
    }
    case 2: // Fonts
    {
        m_pModel->setUiState("CAPTION_FONT_SIZE");
        m_pRadioListModel->updateModel(m_FontSizeList, (int)(m_pControllerModel->captionSize()));

        break;
    }
    default:
    {
        break;
    }
    }
}

void AppMediaPlayer_Video_ScreenCaption::onRadioItem(int item)
{
    QString state = m_pModel->uiState();

    if(state == "CAPTION_LANG")
    {
        m_pControllerFS->setSubtitleLanguage(item); // added by Sergey 09.10.2013 for ITS#187939
        m_pControllerModel->setCaptionCurId(item);
        AppMediaPlayer_SettingsStorage::GetInstance()->SaveVideoSettings(m_pEngineVideo->GetVideoController()); // added by Sergey 08.01.2014 for ITS#211412
    }
    else if(state == "CAPTION_FONT_SIZE")
    {
        if ( item >= AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_SMALL &&
             item <= AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_LARGE )
        {
           m_pControllerFS->setFontSize( (AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_T)item);
        }
    }
}
// } modified by Sergey 20.08.2013 for ITS#184640 


// { modified by Sergey 23.10.2013 for ITS#196877
void AppMediaPlayer_Video_ScreenCaption::onDurationChanged(int duration)
{
    Q_UNUSED(duration);

    if(m_myName == m_pEngineVideo->getScreenManager(m_disp)->topScreenName())
    {
        if(m_pEngineVideo->isVideoInFG())
        {
            emit initScreen();
            emit setDefaultFocus();
        }
        else
        {
            m_pEngineVideo->notifyBothScreens(m_myName, AppMediaPlayer_Video_Screens::SCREEN_FS_CAPTIONS, EVENTS_INIT_SCREEN, QVariant(false));
        }
    }
}
// } modified by Sergey 23.10.2013 for ITS#196877


void AppMediaPlayer_Video_ScreenCaption::onCaptionCurIdChanged(int item)
{
    if(m_pModel->uiState() == "CAPTION_LANG")
    {
        m_pRadioListModel->setSelectedIndex(item);
    }
} // added by Sergey 09.10.2013 for ITS#187939


// { added by Sergey 23.10.2013 for ITS#196877
void  AppMediaPlayer_Video_ScreenCaption::onEventReceived(const QString &srcScreenName,
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
                m_bInitScreen = arg.toBool(); // added by Sergey 25.10.2013 for new DivX
                break;
            }

            default:
                break;
        }
    }
}
// } added by Sergey 23.10.2013 for ITS#196877
