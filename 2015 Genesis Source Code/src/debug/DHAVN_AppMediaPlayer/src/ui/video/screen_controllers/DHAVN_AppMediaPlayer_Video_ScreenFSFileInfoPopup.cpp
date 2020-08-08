#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenFSFileInfoPopup.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_FS_FileInfoPopup.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "QVariant"

AppMediaPlayer_Video_ScreenFSFileInfo::AppMediaPlayer_Video_ScreenFSFileInfo( AppMediaPlayer_Video_Controller *pController,
                                                                              AppMediaPlayer_EngineVideo *pEngineVideo,
                                                                              QObject *parent )
    : AppMediaPlayer_Video_Screen( parent ),
      m_pController( pController ),
      m_pEngineVideo( pEngineVideo )
{
    m_pModel = new AppMediaPlayer_Video_SModel_FS_FileInfoPopup(this);
    setModel( m_pModel );
    m_pControllerFS = NULL; // added by sungha.choi 2013.07.15 for static test
    m_pControllerModel = NULL; // added by sungha.choi 2013.07.15 for static test
}


AppMediaPlayer_Video_ScreenFSFileInfo::~AppMediaPlayer_Video_ScreenFSFileInfo()
{
}

void AppMediaPlayer_Video_ScreenFSFileInfo::onHide()
{
   disconnect( m_pController, SIGNAL(durationChanged(int)),
           this, SLOT(onDurationChanged(int)));

   m_pControllerFS = NULL;
   m_pControllerModel = NULL;
}

/** @copydoc AppMediaPlayer_Video_Screen::onShow() */
void AppMediaPlayer_Video_ScreenFSFileInfo::onShow( QVariant arg )
{
   m_pControllerFS = static_cast<AppMediaPlayer_Video_ControllerFS*>
                   ( m_pController->getController(arg.value<MEDIA_SOURCE_T>()) );

   m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pControllerFS->getModel());
   m_pModel->bindWith( m_pControllerModel );

   connect( m_pController, SIGNAL(durationChanged(int)),
                this, SLOT(onDurationChanged(int)));

   updateDataModel();
}

void AppMediaPlayer_Video_ScreenFSFileInfo::closeScreen()
{
   emit popScreen();
}

// { modified by Sergey for CR#15303
void AppMediaPlayer_Video_ScreenFSFileInfo::updateDataModel()
{
   onDurationChanged( m_pControllerModel->duration() );
}

void AppMediaPlayer_Video_ScreenFSFileInfo::onDurationChanged(int duration)
{
   m_pModel->setDuration(m_pEngineVideo->GetMainUtils()->convertDuration(duration));
}
// } modified by Sergey for CR#15303.
