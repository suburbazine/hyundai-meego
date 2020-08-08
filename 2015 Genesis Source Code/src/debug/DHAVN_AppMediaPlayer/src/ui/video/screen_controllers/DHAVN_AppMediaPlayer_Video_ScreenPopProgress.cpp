#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenPopProgress.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_PopProgress.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"

AppMediaPlayer_Video_ScreenPopProgress::
AppMediaPlayer_Video_ScreenPopProgress( AppMediaPlayer_Video_Controller *pController,
                                        AppMediaPlayer_EngineVideo *pEngineVideo,
                                        QObject *parent )
   : AppMediaPlayer_Video_Screen( parent ),
     m_pController( pController ),
     m_pModel( NULL ),
     m_pControllerFS ( NULL ),
     m_pControllerModel(NULL),
     m_isConnect(false)
{
   Q_UNUSED(pEngineVideo);

   m_pModel = new AppMediaPlayer_Video_SModel_PopProgress(this);
   setModel( m_pModel );
}

AppMediaPlayer_Video_ScreenPopProgress::~AppMediaPlayer_Video_ScreenPopProgress()
{
}

void AppMediaPlayer_Video_ScreenPopProgress::onHide()
{
   MP_LOG << LOG_ENDL;
   //modified by eugene.seo 2012.10.31 for Function_USB_0863
   //disconnect( m_pControllerFS, SIGNAL(operationCopyComplete()),
   //        this, SLOT(onOpertaionCopyComplete()) );
   //disconnect( m_pControllerFS, SIGNAL(operationCopyError(int)),
   //        this, SLOT(onOpertaionCopyError(int)) );
   m_pControllerFS = NULL;
   m_pControllerModel = NULL;
}

void AppMediaPlayer_Video_ScreenPopProgress::onShow( QVariant arg )
{
   m_pControllerFS = static_cast<AppMediaPlayer_Video_ControllerFS*>
                    (m_pController->getController(arg.value<MEDIA_SOURCE_T>()));

   m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_FS*>(m_pControllerFS->getModel());
   m_pModel->bindWith( m_pControllerModel );

   if(!m_isConnect)
   {
       connect( m_pControllerFS, SIGNAL(operationCopyComplete()),
                  this, SLOT(onOpertaionCopyComplete()) ); 
       connect( m_pControllerFS, SIGNAL(operationCopyError(int)),
                  this, SLOT(onOpertaionCopyError(int)) );
	   m_isConnect = true;  
   }

// { modified by wspark 2012.08.21 for DQA #47
   //m_pControllerFS->copyFileToJukebox();
   if(!m_pController->getCopyingStatus()) 
   {
       m_pControllerFS->copyFileToJukebox();
       m_pController->setCopyingStatus(true); 
   }
// } modified by wspark 2012.08.21 for DQA #47

}

void AppMediaPlayer_Video_ScreenPopProgress::onBackKeyPressed()
{
   MP_LOG << LOG_ENDL;
   m_pController->setCopyingStatus(false); // added by wspark 2012.08.21 for DQA #47
   m_pControllerFS->cancelCopyOperation();
   popScreen();
}

void AppMediaPlayer_Video_ScreenPopProgress::onCancelClicked()
{
   MP_LOG << LOG_ENDL;
   m_pController->setCopyingStatus(false); // added by wspark 2012.08.21 for DQA #47
   m_pControllerFS->cancelCopyOperation();
   popScreen();
}

void AppMediaPlayer_Video_ScreenPopProgress::onOpertaionCopyComplete()
{
   MP_LOG << LOG_ENDL;
   m_pController->setCopyingStatus(false); // added by wspark 2012.08.21 for DQA #47
   // { modified by Sergey for CR#11607
   QVariant msgPopUp;
   msgPopUp.setValue( POPUP_TOAST_COPY_COMPLETED );
   replaceScreen(AppMediaPlayer_Video_Screens::SCREEN_POP_TOAST, msgPopUp);
   // } modified by Sergey for CR#11607
}

void AppMediaPlayer_Video_ScreenPopProgress::onOpertaionCopyError( int type )
{
   MP_LOG << "type: " << type << LOG_ENDL;
   // { modified by wspark 2012.12.26 for ITS151423
   // popScreen();
    switch(type)
    {
        case eMP_ERR_SUBTITLE_FILE_NOTFOUND:
        case eMP_ERR_RESOURCE:
        {
            break;
        }
        default:
            popScreen();
            break;
    }
    // } modified by wspark
}
