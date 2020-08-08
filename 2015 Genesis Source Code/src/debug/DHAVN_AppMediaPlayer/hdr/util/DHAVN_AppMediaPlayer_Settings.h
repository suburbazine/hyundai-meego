
#ifndef APP_MEDIA_PLAYER_SETTINGS
#define APP_MEDIA_PLAYER_SETTINGS

#include <QObject>
#include <QString>
#include <QSettings>
#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/DHAVN_AppMediaPlayer_ControllerAudio.h"
#include "controller/DHAVN_AppMediaPlayer_ControllerPhoto.h"
#include "util/DHAVN_AppMediaPlayer_Settings_Def.h"

// { added by kihyung 2013.08.09 for ITS 0183077
class AppMediaPlayer_SettingsStorage : public QObject
{
    Q_OBJECT
public:
    static AppMediaPlayer_SettingsStorage* GetInstance();
    
    ~AppMediaPlayer_SettingsStorage() {}
 
    void LoadAudioData(AppMediaPlayer_Controller_Audio *controllerAudio);
    void LoadPhotoData(PHOTO_LAST_STATE_T* lastState, PHOTO_DISPLAY display); // modified by kihyung 2013.07.26 for ITS 0181599 
    void LoadVideoData(AppMediaPlayer_Video_Controller *controllerVideo);
    void LoadCommon();
    void LoadUSBAudioExtended(AppMediaPlayer_Controller_Audio *controllerAudio);
    void LoadUSBVideoExtended(AppMediaPlayer_Video_Controller *controllerVideo);    
    
    void SaveAudioData(AppMediaPlayer_Controller_Audio *controllerAudio, bool bBackup = false); // modified by jaehwan.lee 2014.05.25
    void SavePlaylist(AppMediaPlayer_Controller_Audio *controllerAudio, bool bSync = false);    
    void SavePhotoData(PHOTO_LAST_STATE_T* lastState, PHOTO_DISPLAY display, bool bBackup = false);// modified by jaehwan.lee 2014.05.25
    void SaveVideoSettings(AppMediaPlayer_Video_Controller *controllerVideo); // modified by jaehwan.lee 2014.05.25
    void SaveAudioSettings(AppMediaPlayer_Controller_Audio *controllerAudio); // modified by jaehwan.lee 2014.05.25
    void SavePhotoSettings(PHOTO_LAST_STATE_T* lastState, PHOTO_DISPLAY display); // modified by jaehwan.lee 2014.05.25
    void SaveVideoData(AppMediaPlayer_Video_Controller *controllerVideo, bool bBackup = false); // modified by jaehwan.lee 2014.05.25
    void SaveCommon(AppMediaPlayer_EngineMain *pEngineMain, bool bBackup = false); //modified by sangmin.seol 2014.09.24 to fix "data load fail"
    void SaveUSBAudioExtended(AppMediaPlayer_Controller_Audio *controllerAudio, bool bSync = false);
    void SaveUSBVideoExtended(AppMediaPlayer_Video_Controller *controllerVideo, bool bSync = false);
    void SaveCommonSettings(AppMediaPlayer_EngineMain *pEngineMain); //added by sangmin.seol 2014.09.24 to fix "data load fail"

    // { added by kihyung 2013.07.30 for ITS 0181638 
    void SaveAudioDataForcedSpec(AppMediaPlayer_Controller_Audio *controllerAudio);
    void SaveVideoDataForcedSpec(AppMediaPlayer_Video_Controller *controllerVideo);
    // } added by kihyung 2013.07.30 for ITS 0181638     
    
    int  GetLastAVEntryPoint();
    bool GetJukeboxIndexingFinished();
    bool GetJukebxoScanningFinished(); // added by kihyung 2013.07.25 for ITS 0180896 

    int  GetLoadLastAVEntryPoint();
    void SetLoadLastAVEntryPoint(int nAVEntry);

    // { added by kihyung 2013.07.31 for ISV 88437
    int     GetBootUSBPort();
    QString GetBootUSBUUID();
    void    SetBootUSBPort(int nPort);
    void    SetBootUSBUUID(QString strUUID);
    // } added by kihyung 2013.07.31 for ISV 88437
 
    int  GetLastAVEntry();

    void    SetLastUSB1ID(QString strID, int nAVMode);
    void    SetLastUSB2ID(QString strID, int nAVMode);
    int     GetLastUSB1AVMode();
    int     GetLastUSB2AVMode();    
    QString GetLastUSB1ID();
    QString GetLastUSB2ID();
 
    void SyncAll();

    // { modified by kihyung 2013.07.26 for ITS 0181599 
    void ResetData(AppMediaPlayer_EngineMain *pEngineMain);
    void FactoryReset(AppMediaPlayer_EngineMain *pEngineMain);
 	// } modified by kihyung 2013.07.26 for ITS 0181599
 
private:
    AppMediaPlayer_SettingsStorage( QObject* parent = NULL );
    
    static AppMediaPlayer_SettingsStorage *m_pInstance;

    // { modified by kihyung 2013.07.26 for ITS 0181599 
    void ResetAudioData(AppMediaPlayer_Controller_Audio *controllerAudio);
    void ResetVideoData(AppMediaPlayer_Video_Controller *controllerVideo);
    void ResetPhotoData(PHOTO_LAST_STATE_T* lastState, PHOTO_DISPLAY display);
    void ResetCommon(AppMediaPlayer_EngineMain *pEngineMain);
    void ResetUSBAudioExtended(AppMediaPlayer_Controller_Audio *controllerAudio);
    void ResetUSBVideoExtended(AppMediaPlayer_Video_Controller *controllerVideo);
    // } modified by kihyung 2013.07.26 for ITS 0181599

    void LoadPlaylist(AppMediaPlayer_Controller_Audio *controllerAudio);
    void LoadUSBPlaylist(AppMediaPlayer_Controller_Audio *controllerAudio);    
    void SaveUSBPlaylist(AppMediaPlayer_Controller_Audio *controllerAudio, bool bSync = false);
    
    QSettings   photo;
    QSettings   audio;
    QSettings   video;
    //{ added by jaehwan.lee 2014.05.23 to fix "data load fail"
    QSettings   photoBackup;
    QSettings   audioBackup;
    QSettings   videoBackup;
    //} added by jaehwan.lee 2014.05.23 to fix "data load fail"
    QSettings   common;
    QSettings   commonBackup;   //added by sangmin.seol 2014.09.24 to fix "data load fail"
    QSettings   playlist;
    
    QSettings   usbextended;
    QSettings   usbplaylist;
    QSettings   usbvideoextended;
 
    int         m_nLastAVEntry;
    int         m_nLoadLastAVEntry;
    bool        m_bJukeboxIndexing;
    bool        m_bJukeboxScanning; // added by kihyung 2013.07.25 for ITS 0180896 
    int         m_nLastUSB1AVMode; 
    int         m_nLastUSB2AVMode;     
    QString     m_strLastUSB1ID;
    QString     m_strLastUSB2ID;

    // { added by kihyung 2013.07.30 for ITS 0181638
    int         m_nLastAudioPos;
    int         m_nLastVideoPos;
    // } added by kihyung 2013.07.30 for ITS 0181638

    // { added by kihyung 2013.07.31 for ISV 88437
    int         m_nBootUSBPort;
    QString     m_strBootUSBUUID;
    // } added by kihyung 2013.07.31 for ISV 88437
    //{ added by jaehwan.lee 2014.05.23 to fix "data load fail"
    QTimer      m_videoBackupTimer;
    QTimer      m_audioBackupTimer;
    QTimer      m_photoBackupTimer;
    QTimer      m_commonBackupTimer;        //added by sangmin.seol 2014.09.24 to fix "data load fail"
    AppMediaPlayer_Video_Controller *m_controllerVideo;
    AppMediaPlayer_Controller_Audio *m_controllerAudio;
    PHOTO_LAST_STATE_T m_photoLastState[PHOTO_DISPLAY_NONE];
    PHOTO_DISPLAY m_photoDisplay;
    AppMediaPlayer_EngineMain *m_EngineMain;      //added by sangmin.seol 2014.09.24 to fix "data load fail"

private slots:
    void OnVideoBackupTimerExpired();
    void OnAudioBackupTimerExpired();
    void OnPhotoBackupTimerExpired();
    void OnCommonBackupTimerExpired();  //added by sangmin.seol 2014.09.24 to fix "data load fail"
    //} added by jaehwan.lee 2014.05.23 to fix "data load fail"

};
// } added by kihyung 2013.08.09 for ITS 0183077

#endif // SETTINGS_APP_STORAGE
