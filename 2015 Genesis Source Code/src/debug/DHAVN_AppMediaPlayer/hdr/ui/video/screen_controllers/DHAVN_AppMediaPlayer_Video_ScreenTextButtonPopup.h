// { modified by Sergey for CR#15744
#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENTEXTBUTTONPOPUP_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENTEXTBUTTONPOPUP_H

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_TextButtonPopup.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h" //added by Sergey for CR#16015

const int TIMER_PERIOD_POPUP = 5000;

class AppMediaPlayer_Video_ScreenTextButtonPopup : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT

public:
    AppMediaPlayer_Video_ScreenTextButtonPopup(AppMediaPlayer_Video_Controller *controller,
                                               AppMediaPlayer_EngineVideo *pEngineVideo,
                                               int disp, // added by wspark 2013.03.14 for ITS 136262
                                               QObject *parent = NULL);

    virtual ~AppMediaPlayer_Video_ScreenTextButtonPopup();
    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_POP_TEXTBUTTON; }

    POPUP_TXT_BTN_TYPE_T popupType() { return m_popUpType; }  // added by Sergey 01.09.2013 for ITS#186834 
    void closePopupBySystemPopup(); //added by edo.lee 2013.12.13 close popup by popup
    bool checkIsSamePopup(); // added by cychoi 2014.04.01 for ITS 233083

signals:
    void updateButtonModel(QString btn1, QString btn2);
    void popUpTimerOut();// added by yongkyun.lee 20130324 for : ISV 77052
    void showLockout(bool onShow);//added by edo.lee 2013.09.13

public slots:
    virtual void onHide();
    virtual void onShow(QVariant arg);
    void closeScreen();
    Q_INVOKABLE void onSoftkeyBtn(QString button);
    void onBackKeyPressed(); //added by lssanh 2013.02.05 ISV72351
    void onLockoutMode(bool on);//added by edo.lee 2013.09.13

private slots:
    void onRetranslateUI(); // added by Sergey for DRM 30.07.2013

private:
    void setupPopUp( POPUP_TXT_BTN_TYPE_T typePopUp );
    void initMessage();

private:
    AppMediaPlayer_Video_SModel_TextButtonPopup *m_pModel;
    AppMediaPlayer_Video_CModel_FS *m_pCModel; //added by Sergey for CR#16015
    AppMediaPlayer_EngineVideo *m_pEngineVideo;
    AppMediaPlayer_Video_ControllerFS *m_pControllerFS;
    AppMediaPlayer_Video_Controller *m_pController;
    QHash <POPUP_TXT_BTN_TYPE_T,QString> m_messagePopUp;
    POPUP_TXT_BTN_TYPE_T m_popUpType;
    QTimer *m_pTimer;
    int m_disp; // added by wspark 2013.03.14
    bool m_isLockout;//added by edo.lee 2013.09.13
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENTEXTBUTTONPOPUP_H
// } modified by Sergey for CR#15744.
