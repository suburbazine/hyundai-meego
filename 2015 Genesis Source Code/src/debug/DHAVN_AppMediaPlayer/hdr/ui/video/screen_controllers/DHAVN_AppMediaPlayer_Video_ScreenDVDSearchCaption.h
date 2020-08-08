#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENSEARCHLANGUAGE_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENSEARCHLANGUAGE_H

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_DVDSearchCaption.h"

class AppMediaPlayer_Video_CModel_DVD;
class AppMediaPlayer_Video_SModel_DVDSearchCaption;
class VListModel;

class ScreenDVDSearchCaption : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT

public:
    ScreenDVDSearchCaption(AppMediaPlayer_Video_Controller *controller,
                           AppMediaPlayer_EngineVideo *pEngineVideo,
                           int disp, // added by lssanh 2013.04.27 ITS165097
                           QObject *parent = NULL);
    virtual ~ScreenDVDSearchCaption();

    virtual QString viewName();


public slots:
    virtual void onHide();
    virtual void onShow(QVariant arg);

    Q_INVOKABLE void onKeyEntered(QString key);
    Q_INVOKABLE void onCodeEntered(int languageCode); //added by Michael.Kim 2013.07.23 for New UX
    Q_INVOKABLE QString getCurrentCode(); // added by lssanh 2013.04.19 ITS158176
    Q_INVOKABLE void refreshCaptionList(); // moved by lssanh 2013.04.19 ITS158176
//    Q_INVOKABLE void closeSerachLanguageScreen();
    Q_INVOKABLE void onSoftkeyBack();
    Q_INVOKABLE void setDefaultInputText(); // added by sjhyun 2013.12.28 for ITS 198332
    //Q_INVOKABLE int getCurrentInputTextCount(); // added by cychoi 2014.01.23 for ITS 222055
    Q_INVOKABLE void deleteAllInputText(); // added by wspark 2014.01.24 for ITS 222575
    void onBackKeyPressed();

    virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                 const int eventId, const QVariant arg); // added by sjhyun 2013.09.11 for ITS 189090

private:
    //void refreshCaptionList(); // moved by lssanh 2013.04.19 ITS158176
    void handleItemSelected(int code);
    bool isDefaultLangCode(int langCode); // added by cychoi 2013.06.30 for Nex UX
    bool isNotLangCode(int langCode); // added by cychoi 2013.11.27 for ITS 211510
    void setCurrentUserLangCode(int langCode); // added by cychoi 2013.09.22 for ITS 158176
    QString langCodeToString(int langCode); // added by cychoi 2013.12.05 for ITS 211510
    void setupModeArea(); // added by cychoi 2013.10.25 for SearchCaption title

    AppMediaPlayer_Video_Controller *m_pController;
    AppMediaPlayer_Video_ControllerDVD *m_pControllerDVD;
    AppMediaPlayer_Video_CModel_DVD *m_pControllerModel;
    AppMediaPlayer_EngineVideo *m_pEngineVideo;
    QString m_viewName;
    AppMediaPlayer_Video_SModel_DVDSearchCaption *m_pModel;
    VListModel *m_pListModel;
    int m_launchMode;
    int m_nUserSubTitleLangCode; // modified by cychoi 2013.09.22 for ITS 158176 // added by sjhyun 2013.09.11 for ITS 189090
    // { added by cychoi 2013.09.22 for ITS 158176
    int m_nUserAudioLangCode; 
    int m_nUserMenuLangCode;
    // } added by cychoi 2013.09.22
    int m_disp; // added by lssanh 2013.04.27 ITS165097
};


#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENSEARCHLANGUAGE_H
