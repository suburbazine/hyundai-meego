// { modified by Sergey 30.07.2013
#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENDIVXREG_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENDIVXREG_H

#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"//added by edo.lee 2013.07.30 subscreen

class AppMediaPlayer_Video_Screen;
//class AppMediaPlayer_EngineVideo;//added by edo.lee 2013.07.30 subscreen
class AppMediaPlayer_Video_SModel_Divx;

class ScreenDivxReg : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT

public:
    ScreenDivxReg(AppMediaPlayer_EngineVideo *pEngineVideo,int disp,//added by edo.lee 2013.07.30 subscreen
                  QObject *parent = NULL );
    virtual ~ScreenDivxReg();

    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_DIVXREG; }

	// moved by Sergey 25.10.2013 for new DivX

    Q_INVOKABLE void onButtonClicked(QString btn); // added by Sergey 25.10.2013 for new DivX


public slots:
    virtual void onHide();
    virtual void onShow(QVariant arg);
    virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                 const int eventId, const QVariant arg); // added by Sergey 25.10.2013 for new DivX

protected:
    void setIsModeAreaFocused(bool isFocused) { m_isModeAreaFocused = isFocused; } // added by Sergey 10.01.2014 for ITS#218955

private:
    void setupDivxMode();
    void closeScreen();
    void setupModeArea();


private:
    AppMediaPlayer_EngineVideo *m_pEngineVideo;
    AppMediaPlayer_Video_Controller *m_pController;
    AppMediaPlayer_Video_SModel_Divx* m_pModel;
	int m_disp;//added by edo.lee 2013.07.30 subscreen
	bool m_bInitScreen; // added by Sergey 25.10.2013 for new DivX

};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENDIVXREG_H
// } modified by Sergey 30.07.2013
