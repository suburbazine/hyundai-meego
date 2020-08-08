#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENPOPPROGRESS_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENPOPPROGRESS_H

#include <QDeclarativeItem>
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"


class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_EngineVideo;
class AppMediaPlayer_Video_SModel_PopProgress;
class AppMediaPlayer_Video_ControllerFS;




class AppMediaPlayer_Video_ScreenPopProgress : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT

public:
   AppMediaPlayer_Video_ScreenPopProgress ( AppMediaPlayer_Video_Controller *pController,
                                            AppMediaPlayer_EngineVideo *pEngineVideo,
                                            QObject *parent = NULL );

   virtual ~AppMediaPlayer_Video_ScreenPopProgress();

   virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_POP_PROGRESS; }

public slots:
   virtual void onHide();
   virtual void onShow( QVariant arg );
   virtual void onBackKeyPressed();

   Q_INVOKABLE void onCancelClicked();

private slots:
   void onOpertaionCopyComplete();
   void onOpertaionCopyError( int type );

private:
   AppMediaPlayer_Video_Controller *m_pController;
   AppMediaPlayer_Video_SModel_PopProgress *m_pModel;
   AppMediaPlayer_Video_ControllerFS *m_pControllerFS;
   AppMediaPlayer_Video_CModel_FS *m_pControllerModel;
   bool m_isConnect;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENPOPPROGRESS_H
