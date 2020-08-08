#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENVCDDIRECTACCESS_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENVCDDIRECTACCESS_H


#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"


class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_Video_ControllerVCD;
class AppMediaPlayer_Video_CModel_VCD;
class AppMediaPlayer_Video_SModel_DirectAccess;

class AppMediaPlayer_Video_ScreenVCDDirectAccess : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_ScreenVCDDirectAccess( AppMediaPlayer_Video_Controller *controller,
                                           AppMediaPlayer_EngineVideo *pEngineVideo,
                                           int disp, // added by yungi 2013.09.07 for ITS 187311
                                           QObject *parent = NULL );
    virtual ~AppMediaPlayer_Video_ScreenVCDDirectAccess();

    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_VCD_DIRECTACCESS; }

	// removed by Sergey 09.05.2013
Q_SIGNALS:
    void showLockout(bool onShow); //added by yungi 2013.09.05 for ITS 187311
    void setDirectAccessUI(bool onTempMode); // added by cychoi 2013.12.19 for ITS 215825 Default focus
public slots:
    virtual void onDeactivate(); // added by cychoi 2013.06.23 for DVD VCD screen management
    virtual void onHide();
    // { Modified by Sergey Vetugov for CR#13621
    virtual void onShow(QVariant arg);
    // } Modified by Sergey Vetugov for CR#13621.
    virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                 const int eventId, const QVariant arg);// added by cychoi 2013.12.17 for ITS 214030 DRS on Popup
    void onLockoutMode(bool on); //added by yungi 2013.09.05 for ITS 187311
    void onBackKeyPressed(); // added by cychoi 2014.07.01 for focus handling on LCD ON/OFF (ITS 241667)
    // removed by sjhyun 2013.11.07 for ITS 207179

    Q_INVOKABLE void onSoftkeyBack();
    Q_INVOKABLE void onVCDMenuItemPressed(QString str);
    Q_INVOKABLE void setDefaultInputText(); // added by cychoi 2013.12.19 ITS 215825

private:
    int convertToInt();
    void setupModeArea(); // added by Sergey 09.05.2013
    //removed by cychoi 2013.08.14 for ITS 184421

    AppMediaPlayer_Video_Controller *m_pController;
    AppMediaPlayer_Video_ControllerVCD *m_pControllerVCD;
    AppMediaPlayer_Video_CModel_VCD *m_pControllerModel;
    AppMediaPlayer_Video_SModel_DirectAccess *m_pModel;
    AppMediaPlayer_EngineVideo *m_pEngineVideo;

    //{ added by yungi 2013.09.07 for ITS 187311
    bool m_isLockout;
    int m_disp;
    //} added by yungi 2013.09.07
};


#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENVCDDIRECTACCESS_H
