#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENAUXPLAYBACK_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENAUXPLAYBACK_H

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Playback.h"

class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_Video_SModel_Playback;

class AppMediaPlayer_Video_ScreenAUXPlayback : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_ScreenAUXPlayback( AppMediaPlayer_Video_Controller *controller,
                                           AppMediaPlayer_EngineVideo *pEngineVideo,
                                           bool isAux1,
                                           int disp,
                                           QObject *parent = NULL );
    virtual ~AppMediaPlayer_Video_ScreenAUXPlayback();

    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_PLAYBACK; }

public slots:
    virtual void onHide();
    virtual void onShow(QVariant arg);
    virtual void onDeactivate(); // added by lssanh 2013.05.24 ITS164925
    virtual void onMenuKeyPressed();
    virtual void onOptionsItemClicked(int item);
    virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                 const int eventId, const QVariant arg); // added by Sergey 04.08.2013 for ITS#179312

    Q_INVOKABLE void onMousePressed();
    Q_INVOKABLE void onMouseReleased(); // added by Sergey 04.08.2013 for ITS#179312
    Q_INVOKABLE void onMouseClicked(bool pbArea);

    //Stuff from Video engine
    void onLockoutMode(bool on);
    // removed by Sergey 24.09.2013 for ITS#188938

    //stuffs from video controller
    void onPlayAuxOnFG(); // added by radhakrushna 2013.03.11 for ISV 74997

private slots:
    // { modified by kihyung 2012.07.31 for CR 12259
    /*
    void setFullScreen();
    */
    void showFullScreen();
    // } modified by kihyung
    void onFgLoading(bool on); // added by Sergey 15.09.2014 for ITS#189390

Q_SIGNALS:
	void showLockout(bool onShow);//added by edo.lee 2013.02.26
signals:
	void lostFocus(int direction);	//added by hcryu

private:
    // { modified by kihyung 2012.07.31 for CR 12259
    void setFullScreen();
    // } modified by kihyung
    
    void setupModeArea(); // added by cychoi 2013.08.14 for ITS 184421
    
    AppMediaPlayer_Video_Controller *m_pController;
    AppMediaPlayer_Video_SModel_Playback *m_pModel;
    AppMediaPlayer_EngineVideo *m_pEngineVideo;
    bool m_bIsAux1;
    QTimer m_fullScreenTimer;
	bool m_isLockout;//added by edo.lee 2013.02.27
	int m_disp;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENAUXPLAYBACK_H

