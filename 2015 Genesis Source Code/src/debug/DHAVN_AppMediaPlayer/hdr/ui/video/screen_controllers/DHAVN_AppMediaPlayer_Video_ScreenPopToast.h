// { modified by Sergey for CR#11607
#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENPOPTOAST_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENPOPTOAST_H

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_PopToast.h"
#include "QTimer"

class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_EngineVideo;

const int TIMER_PERIOD = 3000;

class AppMediaPlayer_Video_ScreenPopToast : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_ScreenPopToast( AppMediaPlayer_Video_Controller *controller,
                                        AppMediaPlayer_EngineVideo *pEngineVideo,
                                        QObject *parent = NULL );

    virtual ~AppMediaPlayer_Video_ScreenPopToast();

    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_POP_TOAST; }

public slots:
    virtual void onHide();
    virtual void onShow( QVariant arg );

private slots:
    void closeScreen();

private:
    void setupPopUp(POPUP_TOAST_TYPE_T typePopUp);
    void initMessage();

    AppMediaPlayer_Video_SModel_PopToast *m_pModel;
    QTimer *m_pTimer;
    QHash <POPUP_TOAST_TYPE_T,QString> m_messagePopUp;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENPOPTOAST_H
// } modified by Sergey for CR#11607
