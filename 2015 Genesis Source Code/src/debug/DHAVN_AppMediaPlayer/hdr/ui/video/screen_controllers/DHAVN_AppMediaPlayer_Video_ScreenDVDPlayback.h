#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENDVDPLAYBACK_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENDVDPLAYBACK_H

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"

class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_Video_ControllerDVD;
class AppMediaPlayer_Video_CModel_DVD;
class AppMediaPlayer_Video_SModel_Playback;

class AppMediaPlayer_Video_ScreenDVDPlayback : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_ScreenDVDPlayback( AppMediaPlayer_Video_Controller *controller,
                                           AppMediaPlayer_EngineVideo *pEngineVideo,
                                           int disp,
                                           QObject *parent = NULL );
    virtual ~AppMediaPlayer_Video_ScreenDVDPlayback();

    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_PLAYBACK; }

public slots:
    virtual void onHide();
    virtual void onShow( QVariant arg );
    virtual void onDeactivate();
    virtual void onMenuKeyPressed();
    virtual void onOptionsItemClicked(int item);
    virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                 const int eventId, const QVariant arg);

    Q_INVOKABLE void onMousePressed();
    Q_INVOKABLE void onMouseReleased();
    Q_INVOKABLE void onMouseClicked(bool pbArea);
    Q_INVOKABLE void onModeAreaTapped();
    Q_INVOKABLE void onButtonAreaTapped();
    Q_INVOKABLE void onSoftkeyBack();
    Q_INVOKABLE void onSoftkeyBtnHold( QString button );
    Q_INVOKABLE void onSoftkeyBtnHoldCritical( QString button );
    Q_INVOKABLE void onSoftkeyBtnReleased( QString button );
    Q_INVOKABLE void onTogglePlayBtn();
    Q_INVOKABLE void onLongPressPlayBtn();
    Q_INVOKABLE void listButtonHandler();
    Q_INVOKABLE void onPrevBtn();
    Q_INVOKABLE void onNextBtn();
    Q_INVOKABLE void onFlick_next();
    Q_INVOKABLE void onFlick_prev();
    Q_INVOKABLE void onNextBtnHold();
    Q_INVOKABLE void onNextBtnHoldCritical();
    Q_INVOKABLE void onPrevBtnHold();
    Q_INVOKABLE void onPrevBtnHoldCritical();
    Q_INVOKABLE void onCancel_ff_rew();
	Q_INVOKABLE void onSelectTuned(); // added by cychoi 2013.07.07 for Smoke Test - Cue is not responsed on tune mode

    // Stuff that will come from controller:
    void onPlaybackStatusChanged( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T );

    //Stuff from Video engine
    void onLockoutMode(bool on);
    void onSetTuneMode(bool on);
    void onTuneSameFile(bool on); //added by yongkyun.lee  2012.10.17  for Current file is white color
	// removed by Sergey 01.05.2013 for Tune Functionality    
	//Stuff from screen manager
    void onFocusVisibilityChanged( bool value );
    // removed by Sergey 24.09.2013 for ITS#188938

    // { commented by cychoi 2014.06.27 for removed too many logs... // { added by kihyung 2012.07.13 for CR 9867
    // { modified by eugene 2012.08.23 for static analysis defect #7578 
    //bool setSelectByTitleMenu(bool bShow);
	//void setSelectByTitleMenu(bool bShow);
	// } modified by eugene 2012.08.23 for static analysis defect #7578 
    //bool getSelectByTitleMenu();
    // } commented by cychoi 2014.06.27 // } added by kihyung
    void onSetCamMode(bool on); // added by wspark 2013.02.21 for ISV 73305
    void onRetranslateUI(); // added by cychoi 2013.09.03 for ITS 187585

    // { added by kihyung 2012.06.20
    // for CR 9855
Q_SIGNALS:    
    void resetSpeedIcon();
    void setSpeedIcon(int rate);    
    // } added by kihyung
    // removed by Sergey 23.09.2013 for ITS#188498
    void showPlaybackControls(bool bShow); // added by kihyung 2012.11.09 for Video(LGE) #12
    void releaseLongPress();	// added by junggil 2012.09.06 for keep the FF or REW mode when change to another screen.	
    // removed by sjhyun 2013.11.05 for ITS 206357
    void cancelFfRew();   // added by yongkyun 2013.01.19 for ITS 150703
    void showLockout(bool onShow);//added by edo.lee 2013.02.26
private slots:
    void onScreenStateChanged( int nScreenState );
    void showFullScreen();
    void onFgLoading(bool on); // added by Sergey 15.09.2014 for ITS#189390

private:
    void setFullScreen();
    void setupModeArea(); // added by Sergey 16.05.2013

    AppMediaPlayer_Video_Controller *m_pController;
    AppMediaPlayer_Video_ControllerDVD *m_pControllerDVD;
    AppMediaPlayer_Video_CModel_DVD *m_pControllerModel;
    AppMediaPlayer_Video_SModel_Playback *m_pModel;
    AppMediaPlayer_EngineVideo *m_pEngineVideo;
    QTimer m_fullScreenTimer;
	bool m_isLockout;//added by edo.lee 2013.02.27
    int m_nScreenState; // added by cychoi 2013.06.23 for DVD VCD screen management

	int m_disp;
};


#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENDVDPLAYBACK_H
