#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENFSPLAYBACK_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENFSPLAYBACK_H

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"

class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_Video_ControllerFS;
class AppMediaPlayer_Video_CModel_FS;
class AppMediaPlayer_Video_SModel_Playback;

class AppMediaPlayer_Video_ScreenFSPlayback : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_ScreenFSPlayback( AppMediaPlayer_Video_Controller *controller,
                                           AppMediaPlayer_EngineVideo *pEngineVideo,
                                           MEDIA_SOURCE_T source,
                                           int disp,
                                           QObject *parent = NULL );
    virtual ~AppMediaPlayer_Video_ScreenFSPlayback();

    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_PLAYBACK; }

signals:
    void updateModeArea(int tab, bool isShow);
    void setBottomArea(int tab);


public slots:
    virtual void onHide();
    virtual void onShow( QVariant arg );
    virtual void onDeactivate();

    virtual void onMenuKeyPressed();
    virtual void onOptionsItemClicked(int item);
    virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                 const int eventId, const QVariant arg); // added by Sergey 04.08.2013 for ITS#179312

    Q_INVOKABLE void onMousePressed();
    Q_INVOKABLE void onMouseReleased();
    Q_INVOKABLE void onMouseClicked(bool pbArea);
    Q_INVOKABLE void onModeAreaTapped();
    Q_INVOKABLE void onSoftkeyBack();
    Q_INVOKABLE void onSoftkeyBtnHold( QString button );
    Q_INVOKABLE void onSoftkeyBtnHoldCritical( QString button );
    Q_INVOKABLE void onSoftkeyBtnReleased( QString button );
    Q_INVOKABLE void onTogglePlayBtn();
    Q_INVOKABLE void onLongPressPlayBtn();
    Q_INVOKABLE void onPrevBtn();
    Q_INVOKABLE void onNextBtn();
    Q_INVOKABLE void onPrevBtnHold();
    Q_INVOKABLE void onPrevBtnHoldCritical();
    Q_INVOKABLE void onNextBtnHold();
    Q_INVOKABLE void onNextBtnHoldCritical();
    Q_INVOKABLE void onProgressBarPressed();
    Q_INVOKABLE void onProgressBarReleased();
    Q_INVOKABLE void onProgressBarSetPosition(int position);
    Q_INVOKABLE void onCancel_ff_rew();
    Q_INVOKABLE void listButtonHandler();
    Q_INVOKABLE void onFlick_next();
    Q_INVOKABLE void onFlick_prev();
	Q_INVOKABLE void onSelectTuned();//added by edo.lee 2013.06.11

    // Stuff that will come from controller:
    void onPlaybackStatusChanged( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T );
    void onRandomModeChanged( AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T );
    void onRepeatModeChanged( AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T );
    void onFSPlaybackError(AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_T);
    void onReadingFile(bool inProgress); // added by Sergey for CR#11607
    void onFilenameChanged(QString filename);
    void onFoldernameChanged(QString foldername);

    // Stuff from Video engine.
    virtual void onLockoutMode(bool on); //modified by edo.lee 2013.02.05
    void onSetTuneMode(bool on);
    void onTuneSameFile(bool on);   // added by yongkyun.lee  2012.10.17  for Current file is white color
	// removed by Sergey 01.05.2013 for Tune Functionality
    //Stuff from screen manager
    void onFocusVisibilityChanged( bool value );
    // removed by Sergey 24.09.2013 for ITS#188938
	// { modified by wspark 2012.11.20 for CR15380
    //void onShowPopup(); // added by wspark 2012.08.21 for DQA #47
	void onShowPopup(POPUP_TXT_BTN_TYPE_T popupType);
    // modified by wspark
    void onSetCamMode(bool on); // added by wspark 2013.02.21 for ISV 73305
    void onFgLoading(bool on); // added by Sergey 15.09.2014 for ITS#189390
    void onRetranslateUI(); // added by Sergey 26.11.2013 for Root folder translation

    // { added by kihyung 2012.06.20
    // for CR 9855
Q_SIGNALS:    
    void resetSpeedIcon();
    void setSpeedIcon(int rate);    
    // } added by kihyung    
    void cancelFfRew();   // added by yongkyun 2013.01.19 for ITS 150703
    // removed by Sergey 23.09.2013 for ITS#188498
    void showPlaybackControls(bool bShow); // added by kihyung 2012.11.09 for Video(LGE) #12
    void releaseLongPress();	// added by junggil 2012.09.06 for keep the FF or REW mode when change to another screen.	
    // removed by sjhyun 2013.11.05 for ITS 206357
    void showLockout(bool onShow);//added by edo.lee 2013.02.26
    
private slots:
    void showFullScreen();
    void onFMCopyCompleted(); // modified by ravikanth 12-07-13 for ITS 0174301
    void onDurationChanged(int duration); // added by Sergey 22.09.2013 for ITS#186731

private:
    QString identifyMyName(MEDIA_SOURCE_T source); // added by Sergey 31.08.2013 for ITS#186799

    /** @brief This method prepare a string for displaying on the control.
      * If the fullString text is wider than maxWidth,
      * returns an elided version of the string (string with "..." in it).
      * @param fullString - it's a full string that you want to display on the control.
      * @param fontFamily - family of font [for example "HDB", "HDR", e.g.]
      * This argument is required for properly calculate width of full string in the pixels.
      * @param pixelSize - The size  of the font used ( in the pixels ).
      * This argument is required for properly calculate width of full string in the pixels.
      * @param maxWidth - The maximum width of the place on control for displaying fullString string. (in the pixels)
      * */
    QString makeElidedString( const QString &fullString,
                              const QString &fontFamily,
                              int pixelSize, int maxWidth ,
                              Qt::TextElideMode elideMode = Qt::ElideRight );

    void setFullScreen();
    void setupModeArea(); // added by Sergey 16.05.2013

    AppMediaPlayer_Video_Controller *m_pController;
    AppMediaPlayer_Video_ControllerFS *m_pControllerFS;
    AppMediaPlayer_Video_CModel_FS *m_pControllerModel;
    AppMediaPlayer_Video_SModel_Playback *m_pModel;
    AppMediaPlayer_EngineVideo *m_pEngineVideo;
    MEDIA_SOURCE_T m_mediaSource;
    QTimer m_fullScreenTimer;
	bool m_isLockout;//added by edo.lee 2013.02.27
	int m_disp;
	bool 				m_isFFREW;//added by edo.lee 2013.06.25
	POPUP_TXT_BTN_TYPE_T	m_pendingPopup; // added by Sergey 22.09.2013 for ITS#186731
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENFSPLAYBACK_H
