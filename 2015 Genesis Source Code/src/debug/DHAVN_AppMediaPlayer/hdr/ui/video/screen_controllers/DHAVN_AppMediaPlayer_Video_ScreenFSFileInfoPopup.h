#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENFSFILEINFO_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENFSFILEINFO_H

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"

class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_Video_ControllerFS;
class AppMediaPlayer_Video_CModel_FS;
class AppMediaPlayer_Video_SModel_FS_FileInfoPopup;

class AppMediaPlayer_Video_ScreenFSFileInfo : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_ScreenFSFileInfo( AppMediaPlayer_Video_Controller *controller,
                                           AppMediaPlayer_EngineVideo *pEngineVideo,
                                           QObject *parent = NULL );
    virtual ~AppMediaPlayer_Video_ScreenFSFileInfo();

    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_FS_FILEINFO_POPUP; }

    Q_INVOKABLE void closeScreen();

    Q_INVOKABLE void updateDataModel();

signals:

public slots:
    virtual void onHide();
    virtual void onShow( QVariant arg );


private slots:
    void onDurationChanged(int duration);
    
private:

    AppMediaPlayer_Video_Controller *m_pController;
    AppMediaPlayer_Video_ControllerFS *m_pControllerFS;
    AppMediaPlayer_Video_CModel_FS *m_pControllerModel;
    AppMediaPlayer_Video_SModel_FS_FileInfoPopup *m_pModel;
    AppMediaPlayer_EngineVideo *m_pEngineVideo;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENFSFILEINFO_H
