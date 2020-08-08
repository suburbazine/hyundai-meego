#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENOPTIONS_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENOPTIONS_H

#include <QDeclarativeItem>

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Options.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerVCD.h"

class AppMediaPlayer_EngineVideo;
class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_Video_SModel_Options;
class AppMediaPlayer_Video_CModel_DVD; // modified by ravikanth CR 15994 12-11-22

class AppMediaPlayer_Video_ScreenOptions : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_ScreenOptions( AppMediaPlayer_Video_Controller *controller,
                                        AppMediaPlayer_EngineVideo *pEngineVideo,
                                        int disp,
                                        QObject *parent = NULL );
    virtual ~AppMediaPlayer_Video_ScreenOptions();

    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_OPTIONS; }
    // removed by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen //{ added by yongkyun.lee 2013-11-05 for ITS 206356

public slots:
    virtual void onHide();
    virtual void onShow( QVariant arg );
    virtual void onDeactivate(); // added by cychoi 2013.08.05 for ScreenOptions is opened when returned from DRS
    virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                 const int eventId, const QVariant arg);
    Q_INVOKABLE virtual void onMenuKeyPressed();

    Q_INVOKABLE void onMouseReleased();
    Q_INVOKABLE void onItemPressed(int item);
    Q_INVOKABLE void onCheckBoxSelect ( int item, bool flag );
    Q_INVOKABLE void onHidden(); // added by Sergey 06.08.2013 to close menu in good time
    Q_INVOKABLE void invokeSelectItemMethod(QObject* pObj, QString method, int arg ); // added by edo.lee 2013.11.20 ITS 0207978  

    void closeOptionsIfAudioLangModel();
    void onBackKeyPressed(); // added by wspark 2012.11.12 for processing back h/w key if option menu is in second depth.
    // { modified by ravikanth CR 15994 12-11-22
    // { modified by dongjin 2013.02.27 for ISV 71500
    //void onActiveAudioLangChanged(QString text);
    void onActiveAudioLangChanged(QString text, QString text2);
    // } modified by dongjin
    void onActiveCaptionChanged(QString text);
    void onActiveAngleChanged(int text);
    // } modified by ravikanth CR 15994 12-11-22
    // removed by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen // added by yongkyun.lee 2013-11-05 for ITS 206356
    void onSubtitleSettingChanged(int mediaSource); // added by kihyung 2013.08.22 for ITS 0185761
    void onVoiceLangEnabled(bool on); // added by Sergey 09.10.2013 for ITS#192050
    void onLockoutMode(bool on); //added by edo.lee 2013.09.14

private slots:
    void onFgBg(bool isFG); 	// modified by Sergey 06.08.2013 to close menu in good time

private:
    AppMediaPlayer_Video_SModel_Options *m_pModel;
    AppMediaPlayer_EngineVideo *m_pEngineVideo;
    AppMediaPlayer_Video_Controller* m_pController;

    AppMediaPlayer_Video_CModel_FS *jukeboxData;
    AppMediaPlayer_Video_CModel_FS *usbData;
    AppMediaPlayer_Video_CModel_FS *m_pControllerModel;
    AppMediaPlayer_Video_CModel_FS *fsData;
    AppMediaPlayer_Video_CModel_VCD *vcdData;
    AppMediaPlayer_Video_CModel_DVD *dvdData; // modified by ravikanth CR 15994 12-11-22
    OPTION_MENU_T* optionsMenuData;

	int m_disp;
	bool m_bCloseInBG; 	// modified by Sergey 06.08.2013 to close menu in good time

    void setRepeatRandomeModel(int repeatMode, int randomMode);
    void setVCDRepeatModel(int repeatMode); // added by cychoi 2013.07.06 for VCD repeat menu
    void setLanguageSettings(QStringList langCodeList,int nCurrStreamId);

private:
    OPTION_MENU_T* m_pVideoLangugeSettingsMenu;
	bool m_isLockoutView;//added by edo.lee 2013.07.05
	bool m_isLockout;//added by edo.lee 2013.09.14
	QString m_menuType; // added by Sergey 16.09.2013 for ITS#185236

    // { added by wspark 2012.11.30 for ISV59938
signals:
    void backPressed();
    // } added by wspark
    void closeOptions(bool animated); // modified by Dmitry 26.04.13
    void showOptions(); // added by Sergey 02.08.2103 for ITS#181512
    void showLockout(bool);//added by edo.lee 2013.09.14
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENOPTIONS_H
