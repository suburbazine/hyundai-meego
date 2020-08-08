#ifndef DHAVN_AppMediaPlayer_Video_ScreenTitleDiscMenu_H
#define DHAVN_AppMediaPlayer_Video_ScreenTitleDiscMenu_H

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"

class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_Video_ControllerDVD;
class AppMediaPlayer_Video_CModel_DVD;
class AppMediaPlayer_Video_SModel_Playback;

class AppMediaPlayer_Video_ScreenTitleDiscMenu : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_ScreenTitleDiscMenu( AppMediaPlayer_Video_Controller *controller,
                                              AppMediaPlayer_EngineVideo * pEngineVideo,
                                              int disp,
                                              QObject *parent = NULL );
    virtual ~AppMediaPlayer_Video_ScreenTitleDiscMenu();

    virtual QString viewName()
    {
       return AppMediaPlayer_Video_Screens::VIEW_DVD_TITLE_DISC_MENU;
    }
Q_SIGNALS:
    void showLockout(bool onShow);
    void hideTitleDiscMenu(bool onHide);//added by eunhye 2013.03.22
    void loadingModeChanged( bool onLoading); // added by lssanh 2013.05.18 ISV81619
    void fgLoadingModeChanged( bool onLoading); // added by cychoi 2013.09.13 for ITS 190207 Infinite DVD Loading Screen
    void titleDiscLoadingDisplayCheck(int m_disp); // added by yungi 2013.10.10 for ITS 194869
    //void hideDelayScreenBG(bool fromHome); // deleted by lssanh 2013.04.07 rollback NoCR hide previous screen

public slots:
    virtual void onHide();
    virtual void onShow( QVariant arg );
    // commented by cychoi 2013.10.13 for ITS 195278 Should not handle DRS on Deactivate
    virtual void onMenuKeyPressed();
    virtual void onOptionsItemClicked( int item );
    virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                 const int eventId, const QVariant arg); // added by cychoi 2013.09.13 for ITS 190207 Infinite DVD Loading Screen
    void onLockoutMode(bool on);
    void onRetranslateUI(); // added by yungi 2013.11.29 for NoCr MiddleEast Changed Not Move Screen TitleDiscMenu

    Q_INVOKABLE void listButtonHandler(); // modified by minho 20120828 for able list menu on DVD 
    Q_INVOKABLE void cursorKey( int direction );
    Q_INVOKABLE void titleMenu();
    Q_INVOKABLE void select();

private slots:
    void onScreenStateChanged( int nState );


private:
    void setupModeArea(); // added by cychoi 2013.08.14 for ITS 184421

    AppMediaPlayer_Video_Controller *m_pController;
    AppMediaPlayer_Video_ControllerDVD *m_pControllerDVD;
    AppMediaPlayer_Video_CModel_DVD *m_pControllerModel;
    AppMediaPlayer_EngineVideo *m_pEngineVideo;
	bool m_isLockout;//added by edo.lee 2013.02.27
	int m_disp;
};



#endif // DHAVN_AppMediaPlayer_Video_ScreenTitleDiscMenu_H
