// { modified by Sergey. Removed Divx part.
#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENDVDSETTIGS_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENDVDSETTIGS_H

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include <DHAVN_AppSettings_Shared.h>

class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_Video_ControllerDVD;
class AppMediaPlayer_Video_ControllerFS; // modified by ravikanth CR 15994 12-11-22
class AppMediaPlayer_Video_CModel_DVD;
class AppMediaPlayer_Video_SModel_Playback;
class AppMediaPlayer_Video_SModel_DVDPlayback;
class AppMediaPlayer_VideoSettings;
class DVDSettingsSModel; // modified by ravikanth 13-03-13

class AppMediaPlayer_Video_ScreenDVDSettings : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT

public:
    AppMediaPlayer_Video_ScreenDVDSettings( AppMediaPlayer_Video_Controller *controller,
                                            AppMediaPlayer_EngineVideo *pEngineVideo,
                                            int disp, // added by cychoi 2013.08.20 for ITS 178895 SerachCaption subscreen
                                            QObject *parent = NULL,
                                            QString viewName = AppMediaPlayer_Video_Screens::VIEW_DVD_SETTINGS);
    virtual ~AppMediaPlayer_Video_ScreenDVDSettings();

    virtual QString viewName();

    Q_INVOKABLE void onSoftkeyBack(); // modified by ravikanth 13-03-13


private:
    void captionSelected(int index);
    void voiceLangSelected(int index);
    void menuLangSelected(int index);
    void setupModeArea(QString source); // added by Sergey 16.05.2013

signals:
    void updateSelectedIndex( int index );
    void setDVDSettingUI();
	// } modified by ravikanth 13-03-13
	
public slots:
    virtual void onHide();
    virtual void onShow( QVariant arg );

	// { modified by ravikanth 13-03-13
    Q_INVOKABLE void radioItemSelected(int index);
    Q_INVOKABLE void onTabSelected(int index);
	// } modified by ravikanth 13-03-13

private slots:
    // removed by Sergey for CR#15775
    void handleVCItem(VCItem *item); // added by ravikanth	- CR 11552, 11938
    void onRetranslateUI(); // added by cychoi 2013.12.04 for ITS 212522

private:
    AppMediaPlayer_Video_Controller *m_pController;
    AppMediaPlayer_Video_ControllerDVD *m_pControllerDVD;
	// { modified by ravikanth CR 15994 12-11-22
    AppMediaPlayer_Video_ControllerFS *m_pControllerJukeBox;
    AppMediaPlayer_Video_ControllerFS *m_pControllerUSB1;
    AppMediaPlayer_Video_ControllerFS *m_pControllerUSB2;
	// } modified by ravikanth CR 15994 12-11-22
	
	// { modified by ravikanth 13-03-13
    AppMediaPlayer_Video_CModel_DVD *m_pControllerModel;
    DVDSettingsSModel *m_pModel;
	// } modified by ravikanth 13-03-13

    AppMediaPlayer_EngineVideo *m_pEngineVideo;
    AppMediaPlayer_VideoSettings* m_pSettingsStorage;

    QString m_viewName;

	DISPLAY_T m_lastOSDDisplay;//added by edo.lee 2013.04.26
	bool m_lastCloneTemp;//added by edo.lee 2013.06.07

    int m_disp; // added by cychoi 2013.08.20 for ITS 178895 SerachCaption subscreen
};


#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENDVDSETTIGS_H
// } modified by Sergey. Removed Divx part.
