// { modified by Sergey 07.09.2013 for PRE_FG supoort

#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_OverlaySourceModeArea.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_SourceModeAreaOverlay.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_DVD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_VCD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerBase.h"



AppMediaPlayer_Video_OverlaySourceModeArea::
AppMediaPlayer_Video_OverlaySourceModeArea( AppMediaPlayer_Video_Controller *controller,
                                           AppMediaPlayer_EngineVideo *engine,
                                           int disp,
                                           QObject *parent )
    : AppMediaPlayer_Video_Screen( parent ),
      m_pEngine(engine),
      m_pController(controller),
      m_disp(disp),
      m_display((m_disp == VIDEO_DISPLAY_FRONT) ? DISPLAY_FRONT : DISPLAY_REAR) // added by Sergey 10.05.2013
{
    m_myName = AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA; // added by Sergey 31.08.2013 for ITS#186799

    m_pModel = new AppMediaPlayer_Video_SModel_SourceModeAreaOverlay(this);
    setModel(m_pModel);

    m_pControllerModelDVD = dynamic_cast<AppMediaPlayer_Video_CModel_DVD*>(m_pController->getController(MEDIA_SOURCE_DVDVIDEO)->getModel());
    m_pControllerModelVCD = dynamic_cast<AppMediaPlayer_Video_CModel_VCD*>(m_pController->getController(MEDIA_SOURCE_VCD)->getModel());
    m_pControllerModelJukebox = dynamic_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_JUKEBOX)->getModel());
    m_pControllerModelUSB = dynamic_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_USB1)->getModel());
    m_pControllerModelUSB2 = dynamic_cast<AppMediaPlayer_Video_CModel_FS*>(m_pController->getController(MEDIA_SOURCE_USB2)->getModel());

    connect( m_pControllerModelDVD, SIGNAL(titleChanged( int )),                this, SLOT(onSetFileCountInfo(int)));
    connect( m_pControllerModelDVD, SIGNAL(focusTitleChanged( int )),           this, SLOT(onSetFileCountInfo(int))); //added by Michael.Kim 2013.06.07 for current focus index change
    connect( m_pControllerModelDVD, SIGNAL(titleCountChanged( int )),           this, SLOT(onSetFileCountInfo(int)));
    connect( m_pControllerModelDVD, SIGNAL(chapterChanged( int )),              this, SLOT(onSetFileCountInfo(int)));
    connect( m_pControllerModelDVD, SIGNAL(focusChapterChanged( int )),         this, SLOT(onSetFileCountInfo(int))); //added by Michael.Kim 2013.06.07 for current focus index change
    connect( m_pControllerModelVCD, SIGNAL(currentTrackChanged( int )),         this, SLOT(onSetFileCountInfo(int)));
    connect( m_pControllerModelVCD, SIGNAL(tuneWheelTrackChanged( int )),       this, SLOT(onSetFileCountInfo(int)));
    connect( m_pControllerModelJukebox, SIGNAL(playlistPositionChanged( int )), this, SLOT(onSetFileCountInfo(int)));
    connect( m_pControllerModelUSB, SIGNAL(playlistPositionChanged( int )),     this, SLOT(onSetFileCountInfo(int)));
    connect( m_pControllerModelUSB2, SIGNAL(playlistPositionChanged( int )),    this, SLOT(onSetFileCountInfo(int)));
}

AppMediaPlayer_Video_OverlaySourceModeArea::~AppMediaPlayer_Video_OverlaySourceModeArea()
{
}



void AppMediaPlayer_Video_OverlaySourceModeArea::onHide()
{
    m_pModel->setIsShown(false); // added by Sergey for 24.04.2013
}



void AppMediaPlayer_Video_OverlaySourceModeArea::onShow( QVariant arg )
{
    Q_UNUSED(arg);
    m_pModel->setIsShown(true); // added by Sergey for 24.04.2013
}



void AppMediaPlayer_Video_OverlaySourceModeArea::onBackKeyPressed()
{
}


void AppMediaPlayer_Video_OverlaySourceModeArea::onSoftkeyBack()
{
	m_pEngine->backHandle(m_disp == VIDEO_DISPLAY_FRONT ? true:false, true);
}



void AppMediaPlayer_Video_OverlaySourceModeArea::onMousePressed()
{
	notifyScreenManager(m_myName, m_pEngine->getScreenManager(m_disp)->topScreenName(), EVENTS_MOUSE_PRESSED); // modified by Sergey 31.08.2013 for ITS#186799
}



void AppMediaPlayer_Video_OverlaySourceModeArea::onMouseReleased()
{
	notifyScreenManager(m_myName, m_pEngine->getScreenManager(m_disp)->topScreenName(),	EVENTS_MOUSE_RELEASED); // modified by Sergey 31.08.2013 for ITS#186799
}



void AppMediaPlayer_Video_OverlaySourceModeArea::menuButtonHandler()
{
    m_pEngine->releaseTuneModeByMenu();
    m_pEngine->getScreenManager(m_disp)->onMenuKeyPressed(EVT_KEY_SOFT_MENU);
}



void AppMediaPlayer_Video_OverlaySourceModeArea::listButtonHandler()
{
    m_pEngine->getScreenManager(m_disp)->topScreen()->onOptionsItemClicked(OPTIONS_LIST);
}



void AppMediaPlayer_Video_OverlaySourceModeArea::onSetFileCountInfo(int nNumber)
{
    // { added by kihyung 2013.08.25 for ITS 0185459 
    AppMediaPlayer_Video_ControllerBase* ctrl = m_pController->getCurrentController();
    if(ctrl == NULL) return;

    if(QObject::sender() != ctrl->getModel()) 
    {
        MP_LOG << "Sender controller is not active. Return." << LOG_ENDL;
        return;
    }
    // } added by kihyung 2013.08.25 for ITS 0185459     

	int currentFile = 0, filesNumber = 0;

	// { modified by kihyung 2013.09.11 for ITS 0189474
    switch( m_pController->getSource() )
    {
        case MEDIA_SOURCE_JUKEBOX:
            if(nNumber < 0) {
                currentFile = m_pControllerModelJukebox->displayPlaylistPos() + 1;
            }
            else{
                currentFile = nNumber + 1;
            }
            filesNumber = m_pControllerModelJukebox->displayPlaylistLength();
            break;

        case MEDIA_SOURCE_USB1:
            if(nNumber < 0) {
                currentFile = m_pControllerModelUSB->displayPlaylistPos() + 1;
            }
            else {
                currentFile = nNumber + 1;
            }
            filesNumber =  m_pControllerModelUSB->displayPlaylistLength();
            break;

        case MEDIA_SOURCE_USB2:
            if(nNumber < 0) {
                currentFile = m_pControllerModelUSB2->displayPlaylistPos() + 1;
            }
            else {
                currentFile = nNumber + 1;
            }
            filesNumber = m_pControllerModelUSB2->displayPlaylistLength();
            break;

        default: // make compiler happy
            break;
    }
    // } modified by kihyung 2013.09.11 for ITS 0189474

    if( m_pModel->currentFile() != currentFile )
        m_pModel->setCurrentFile(currentFile);

    if( m_pModel->filesNumber() != filesNumber )
        m_pModel->setFilesNumber(filesNumber);
}



//{modified by edo.lee 2013.08.16 for subscreen in AVP
void AppMediaPlayer_Video_OverlaySourceModeArea::onEventReceived( const QString &srcScreenName,
                                                                 const QString &resScreenName,
                                                                 const int eventId,
                                                                 const QVariant arg )
{
    MP_LOG << " srcScreenName = " << srcScreenName << " resScreenName = " << resScreenName << \
             " eventId = " << eventId << " arg = " << arg << LOG_ENDL;

    Q_UNUSED(srcScreenName);
    Q_UNUSED(arg);

    if ( resScreenName == m_myName ) // modified by Sergey 31.08.2013 for ITS#186799
    {
        switch( eventId )
        {
        case EVENTS_SET_COUNTER_VISIBLE:
        {
            m_pModel->setCounterVisible(arg.toBool());
            break;
        }
        case EVENTS_SET_CUR_NUMBER:
        {
            m_pModel->setCurrentFile(arg.toInt());
            break;
        }
        case EVENTS_SET_TOTAL_NUMBER:
        {
            m_pModel->setFilesNumber(arg.toInt());
            break;
        }
        case EVENTS_SET_ICON_VISIBLE:
        {
            m_pModel->setIconVisible(arg.toBool());
            break;
        }
        case EVENTS_SET_ICON:
        {
            m_pModel->setIcon(arg.toString());
            break;
        }
        case EVENTS_SET_RTEXT_VISIBLE:
        {
            m_pModel->setRTextVisible(arg.toBool());
            break;
        }
        case EVENTS_SET_RTEXT:
        {
            m_pModel->setRText(arg.toString());
            break;
        }
        case EVENTS_SET_LIST_BTN_VISIBLE:
        {
            m_pModel->setIsListBtnVisible(arg.toBool());
            break;
        }
        case EVENTS_SET_MENU_BTN_VISIBLE:
        {
            m_pModel->setMenuBtnVisible(arg.toBool());
            break;
        }
        case EVENTS_SET_TEXT:
        {
            m_pModel->setText(arg.toString());
            break;
        }
        case EVENTS_SET_NORMAL:
        {
            // { modified by cychoi 2013.07.13 for List View spec out
            m_pModel->setIsListBtnVisible(false);
            //m_pModel->setIsListBtnVisible(true);
            // } modified by cychoi 2013.07.13
            m_pModel->setMenuBtnVisible(true);
            m_pModel->setCounterVisible(true);
            m_pModel->setIconVisible(true);
            break;
        }
        case EVENTS_SET_SETTING:
        {
            m_pModel->setIsListBtnVisible(false);
            m_pModel->setMenuBtnVisible(false);
            m_pModel->setCounterVisible(false);
            m_pModel->setIconVisible(false);
            break;
        }

        default:
            break; // make compiler happy
        }
    }
}
// } modified by Sergey 07.09.2013 for PRE_FG supoort


// { added by Sergey 03.10.2013 for ITS#193201
void AppMediaPlayer_Video_OverlaySourceModeArea::onJogEvent(int arrow, int status)
{
    MP_LOG << "status =" << status << "arrow =" << arrow << "top screen =" << m_pEngine->getScreenManager(m_disp)->topScreenName() << LOG_ENDL;

	 //modified by taihyun.ahn for ITS 218397 2014.01.08
    if(m_pEngine->getEngineMain().getCloneState() == CLONE_OFF &&
        (m_disp == VPEnum::VDISP_FRONT && m_pEngine->getScreenManager(m_disp)->getIsRRC()) ||
            (m_disp == VPEnum::VDISP_REAR && !m_pEngine->getScreenManager(m_disp)->getIsRRC()))
    {
        return;
    }
     //modified by taihyun.ahn for ITS 218397 2014.01.08

    if(arrow == AppEngineQMLConstants::JOG_CENTER)
    {
        if(status == AppEngineQMLConstants::KEY_STATUS_PRESSED)
        {
            notifyScreenManager(m_myName, m_pEngine->getScreenManager(m_disp)->topScreenName(), EVENTS_MOUSE_PRESSED);
        }
        else if (status == AppEngineQMLConstants::KEY_STATUS_RELEASED
                 || status == AppEngineQMLConstants::KEY_STATUS_CANCELED)
        {
            notifyScreenManager(m_myName, m_pEngine->getScreenManager(m_disp)->topScreenName(), EVENTS_MOUSE_RELEASED);
        }
    }
}
// } added by Sergey 03.10.2013 for ITS#193201
