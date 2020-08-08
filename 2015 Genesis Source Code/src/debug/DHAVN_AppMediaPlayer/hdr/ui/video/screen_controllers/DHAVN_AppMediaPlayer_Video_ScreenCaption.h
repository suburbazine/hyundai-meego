// { modified by Sergey 20.08.2013 for ITS#184640 
#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENCAPTION_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENCAPTION_H

#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "util/DHAVN_AppMediaPlayer_Notifier.h"


class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_EngineVideo;
class AppMediaPlayer_Video_ControllerFS;
class AppMediaPlayer_Video_SModel_Caption;
class AppMediaPlayer_Video_CModel_FS;
class RadioListModel;


class AppMediaPlayer_Video_ScreenCaption
    : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT

public:
    AppMediaPlayer_Video_ScreenCaption( AppMediaPlayer_Video_Controller *pController,
                                        AppMediaPlayer_EngineVideo *pEngineVideo,
                                       VIDEO_DISPLAY_T disp,
                                        QObject *parent = NULL );

    virtual ~AppMediaPlayer_Video_ScreenCaption();

    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_FS_CAPTIONS; }

    Q_INVOKABLE void onSetEnabledCaption ( bool enabledCaption );
    Q_INVOKABLE void onRadioItem(int item);
    Q_INVOKABLE void onSoftkeyBack();
    Q_INVOKABLE void onMenuItem(int item);

    void setupModeArea();


public slots:
    virtual void onHide();
    virtual void onShow(QVariant arg);
    virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                 const int eventId, const QVariant arg); // added by Sergey 23.10.2013 for ITS#196877

    void onDurationChanged(int);
    void onCaptionCurIdChanged(int); // added by Sergey 09.10.2013 for ITS#187939

protected:
    void setIsModeAreaFocused(bool isFocused) { m_isModeAreaFocused = isFocused; } // added by Sergey 10.01.2014 for ITS#218955


signals:
    void initScreen(); // added by Sergey 09.10.2013 for ITS#187939
    // moved by Sergey 25.10.2013 for new DivX


private: // modified by Sergey 23.10.2013 for ITS#196877
    AppMediaPlayer_Video_Controller *m_pController;
    AppMediaPlayer_EngineVideo *m_pEngineVideo;
    AppMediaPlayer_Video_ControllerFS *m_pControllerFS;
    AppMediaPlayer_Video_CModel_FS *m_pControllerModel;
    AppMediaPlayer_Video_SModel_Caption *m_pModel;
    VIDEO_DISPLAY_T m_disp;
    RadioListModel *m_pRadioListModel;
    QStringList m_FontSizeList;
    bool m_bInitScreen; // added by Sergey 23.10.2013 for ITS#196877

};


#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENCAPTION_H
// } modified by Sergey 20.08.2013 for ITS#184640 
