// { modified by Sergey 07.09.2013 for PRE_FG supoort
#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_OVERLAYSOURCEMODEAREA_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_OVERLAYSOURCEMODEAREA_H

#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"


class AppMediaPlayer_Video_SModel_SourceModeAreaOverlay;
class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_Video_CModel_FS;
class AppMediaPlayer_Video_CModel_VCD;
class AppMediaPlayer_Video_CModel_DVD;



class AppMediaPlayer_Video_OverlaySourceModeArea
        : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT

public:
    AppMediaPlayer_Video_OverlaySourceModeArea(AppMediaPlayer_Video_Controller *controller,
                                               AppMediaPlayer_EngineVideo * engine,
                                               int disp,
                                               QObject *parent = NULL);

    virtual ~AppMediaPlayer_Video_OverlaySourceModeArea();

    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_SOURCE_MODEAREA_OVERLAY;}



public slots:
    virtual void onHide();
    virtual void onShow( QVariant arg );
    virtual void onBackKeyPressed();
    virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                 const int eventId, const QVariant arg); // added by Sergey 08.05.2013


    Q_INVOKABLE void menuButtonHandler();
    Q_INVOKABLE void onMousePressed();
    Q_INVOKABLE void onMouseReleased();
    Q_INVOKABLE void onSoftkeyBack();
    Q_INVOKABLE void listButtonHandler();
    Q_INVOKABLE void onJogEvent(int arrow, int status); // added by Sergey 03.10.2013 for ITS#193201

private slots:
    void onSetFileCountInfo(int nNumber);
	

private:

    AppMediaPlayer_Video_SModel_SourceModeAreaOverlay *m_pModel;
    AppMediaPlayer_EngineVideo *m_pEngine;

    AppMediaPlayer_Video_Controller  *m_pController;

    AppMediaPlayer_Video_CModel_DVD *m_pControllerModelDVD;
    AppMediaPlayer_Video_CModel_VCD *m_pControllerModelVCD;
    AppMediaPlayer_Video_CModel_FS *m_pControllerModelJukebox;
    AppMediaPlayer_Video_CModel_FS *m_pControllerModelUSB;
    AppMediaPlayer_Video_CModel_FS *m_pControllerModelUSB2;

	int m_disp;
	DISPLAY_T m_display;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_OVERLAYSOURCEMODEAREA_H
// } modified by Sergey 07.09.2013 for PRE_FG supoort
