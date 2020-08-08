#ifndef HOME_MEDIAPLAYER_NOTIFIER_H
#define HOME_MEDIAPLAYER_NOTIFIER_H

#include <QObject>
#include <QSharedMemory> // added by wspark 2013.04.23 for cluster update
#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include "engine/DHAVN_AppMediaPlayer_EngineBase.h"
#include <DHAVN_QCANController_ApplnTxRx.h>
#include <DHAVN_AppHomeScreen_Def.h>
#include <DHAVN_SharedDataProvider.h>
#include <DHAVN_DualDisplayHelper.h> // added by ravikanth 27-12-12
#include "util/video/DHAVN_AppMediaPlayer_MainUtils.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"


#define STATUSBAR_NEW 1 //added  by edo.lee 2012.12.12 for statusbar
#define AVP_USE_NEW_BLEND_AFW   1 // youngsim.jo 2013.08.05 blanding offmodified by kihyung 2013.08.02 for new afw

// deleted by mozart, 2013.05.26 for removing redundancy

// deleted by mozart, 2013.05.26 for removing redundancy

class AppMediaPlayer_EngineMain;
class AppMediaPlayer_VideoSettings; // added by ravikanth 27-12-12
class GSTAudioRouterDBus;

//added by edo.lee 2013.02.14
enum MEDIA_ENABLE_T
{
        MEDIA_DISABLE = 0,
        MEDIA_ENABLE,
        MEDIA_INIT
};
// added by edo.lee 2013.02.01
enum VIDEO_DISPLAY_T
{
   VIDEO_DISPLAY_FRONT = 0,
   VIDEO_DISPLAY_REAR,
   VIDEO_DISPLAY_MAX
};
// added by edo.lee
//added by edo.lee 2013.10.25
enum KEY_PRESSED_T
{
    PRESSED_LEFT = 0,
	PRESSED_RIGHT,
	PRESSED_MAX
};
//added by edo.lee 2013.10.25

class ExternalNotifier: public QObject
{
    Q_OBJECT

public:

    ExternalNotifier( AppMediaPlayer_EngineMain *pEngineMain );
    ~ExternalNotifier(); 
    
    void SetActiveMode(APP_MEDIA_MODE_T mode);
    // { added by changjin 2012.10.04 for SANITY_CM_AJ338
    APP_MEDIA_MODE_T GetActiveMode(void); 
    void SetAVOffMode(bool mode);
    bool GetAVOffMode(void);
    // } added by changjin 2012.10.04 for SANITY_CM_AJ338
    void UpdateMainMenu(MEDIA_SOURCE_T source);
    void UpdateMainMenuAfterBoot(); // added by kihyung 2013.2.4 for ISV 71353
	//{ modified by eunhye 20120831 for CR12993
    void updateErrorOSD(QString text);
    void updateErrorOSD(int param); 
	//} modified by eunhye 20120831

    // Technical method to check SDP update functionality.
    void readSDPVideoData();

    // { removed by junam 2012.11.28 for CR14466
    // { added by changjin 2012.10.15 for SANITY_CM_AG056
    //    void SetDelayOSDUpdate(bool mode);
    //    bool GetDelayOSDUpdate(void);
    // } added by changjin 2012.10.15
    // } remoeve by junam

    //modified by edo.lee 2013.01.03
    //void DisplayVideoTrackOSD(); // added by yungi 2012.10.30 for Sanity_DV2.05.20 -1625 
    //void DisplayAudioTrackOSD(); //moved from by hyochang.ryu 20130531
    //void DisplayCustomOSD(QString sTitle);		//added by hyochang.ryu 20130715 //ys-20131007 is not used
    //void DisplayVideoTrackOSD(); //added by edo.lee 2013.03.04 //ys-20131007 is not used
    //void DisplayDVDMenuOSD(); // added by yongkyun.lee 20130705 for : ITS 178308 //ys-20131007 is not used
    //void DisplayVideoOSDonDevice(int device);//added by edo.lee 2013.04.20 //ys-20131007 is not used
    //modified by edo.lee

    // { removed by junam 2012.11.28 for CR14466
    //bool getCancelUpdateOSD(); //added by changjin 2012.10.22 for CR 14445
    // } removed by junam

    // { added by kihyung 2013.1.14
    void setOverlayPlanes(DISPLAY_PLANES_T eDispPlane, bool bForce = false);
    // } added by kihyung 2013.1.14
   	void resetMediaEnabled(MEDIA_SOURCE_T source); //added by edo.lee 2013.01.23
	void setEnableMedia(MEDIA_SOURCE_T source, MEDIA_TYPE_T playMediaType); //added by edo.lee 2013.01.23
	// { removed by kihyung 2013.2.17
	// void setMusicEnableMedia(MEDIA_SOURCE_T source, MEDIA_ENABLE_T isEnable); // added by eugene.seo 2013.01.29 for all unsupported video
	// void setVideoEnableMedia(MEDIA_SOURCE_T source, MEDIA_ENABLE_T isEnable); // added by eugene.seo 2013.01.29 for all unsupported video
	// void setPhotoEnableMedia(MEDIA_SOURCE_T source, MEDIA_ENABLE_T isEnable); // added by eugene.seo 2013.01.29 for all unsupported video
	// } removed by kihyung 2013.2.17	
    void cancelFileOperation();// added by lssanh 2013.02.07 2USB
    QString GetEjectDisplay(); //added by edo.lee 2013.03.11
    void setIsScanMode(bool isScan){m_isScanMode = isScan;} //added by edo.lee 2013.04.05
    void setClusterUpdate(bool value); // added by wspark 2013.04.10 for cluster update   
    void setHistoryInfo(MEDIA_SOURCE_T device,QString filename, QString foldername, int position);
	void setHistoryTuneInfo();//added by edo.lee 2013.05.08
	void setHistoryPlayMode(int random, int repeat);//added by edo.lee 2013.07.05
	
    void UpdateSDPMediaCount(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, int count); // removed by kihyung 2013.5.6

	void resetScanMode(MEDIA_SOURCE_T device);//added by edo.lee 2013.05.16
	void ClearOSD(bool isFront);//added by edo.lee 2013.06.25
	MODE_STATE_T GetCurrentFromSHM();//added by edo.lee 2013.06.27
        //bool continueDisplayOSD(bool isAudio);//added by edo.lee 2013.07.06
        void setAutoChangeFileIndex(int index);

        // { added by kihyung 2014.01.14 for ITS 0218420
        void setGstPipelineDisplay(DISPLAY_T display);
        DISPLAY_T getGstPipelineDisplay() { return m_gstPipelineDisplay; }
        // } added by kihyung 2014.01.14 for ITS 0218420

signals:
    void Can_data_send(const SQCanFrame &canframe);

public slots:

    // Controllers' signals:
    void onStateChanged(int state);
    void onPositionChanged(int seconds);
    // { modified by wspark 2013.04.09
    //void onPositionCleared(); // added by wspark 2013.03.30 for ISV 77359
    void onPositionCleared(bool value);
    // } modified by wspark
    void onDurationChanged(int seconds);
    void onTrackChanged(int index, int quantity);
    void onTrackClusterChanged(int index, int quantity, bool bResetPosition); // added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    void onMediaInfoChanged( int     deviceID, // added by kihyung 2013.08.08 for ITS 0183028
                             QString title,
                             QString album,
                             QString artist,
                             QString genre,
                             QString cover,
                             QString composer, // add by junggil 2012.06.29 for CR08958 : Composer info is not displayed on activating more info in Basic play back screen
                             QString folder, // added by junggil 2012.08.02 for CR12564
                             QString filename);
    void onMediaInfoClusterChanged(QString title); // added by cychoi 2013.08.17 for HMC request - Cluster display = OSD display
    void onDirTreeFetched( bool bFetched); // added by cychoi 2013.11.18 for ITS 208698 MP3 Play Info on Cluster 

	void onBTMusicInfoChanged( QString title, QString album, QString artist ,QString count); //added by eunkoo 2012.07.26 for CR11898
    void onDeviceChanged(int device);
    void onTuneSameFile(bool val);   //  added by yongkyun.lee  2012.10.17  for Current file is white color
    void onContentSize(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, int count);
    // void onSourceAvailableChanged(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, bool status); // added by Sergey for #16250 // removed by kihyung 2013.2.17
    // { modified by yongkyun.lee@lge.com 2012.07.14  CR 11922
    //void onPlayingModeChanged( int nRandom, int nRepeat, int nScan );
    void onPlayingModeChanged( int nRandom, int nRepeat, int nScan, bool nNoOSD);
    // } modified by yongkyun.lee@lge.com

    //{changed by junam 2013.03.26 for ISV72425
    //void onTuneSearchChanged( QString m_coverart, QString m_artist, QString m_song, QString m_album, QString m_folder, QString m_genre, QString m_composer, QString m_countInfo , bool m_samefile);
    void onTuneSearchChanged( QString m_coverart, QString m_artist, QString m_song, QString m_album, QString m_folder, QString m_countInfo , bool m_samefile);
    //}changed by junam

    void onTuneCounterChanged(QString count);

    //{changed by junam 2013.03.26 for ISV72425
    //void onTuneMetaDataChanged( QString artist, QString song, QString album, QString folder, QString genre, QString composer, bool isColor );
    void onTuneMetaDataChanged( QString artist, QString song, QString album, QString folder, bool isColor );
    //}changed by junam

    //void displayOSD(unsigned int nEventType);
    //void displayOSD(unsigned int nEventType, DISPLAY_T display); // modified by ravikanth for ITS 0178087
    void onSetDisplayOSD(bool vOSD);// added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD  
    //bool GetDisplayOSD() { return m_OSDDisplay; }	    //added by hyochang.ryu 20130817 for ITS184947

    // { removed by junam 2012.11.28 for CR14466
    //void setCancelUpdateOSD(bool mode);//added by changjin 2012.10.15 for SANITY_CM_AG056
    // } removed by junam
    void onUpdateStateTrackOSD(int speedCheck); //added by yungi 2012.11.14 for ISV 62667,ISV 62596
    void onPlayingModeChanged(MEDIA_MODE mode);
    void onMoreLikeThisCluster();

    // Video player model:
    void onVideoFilenameChanged(QString filename);
    void onFoldernameChanged(QString name);
    void onVideoFullFolderNameChanged(QString fullfoldername);
    void onPlaylistLenghtChanged(int count);
    void onPlaylistPositionChanged(int position);
    // { added by Sergey Vetugov for CR#13601
    // void onDVDScreenChanged(int state); // remvoed by kihyung 2012.11.26. removed spec.
    // } added by Sergey Vetugov for CR#13601
    // removed by Sergey for CR#15771

    // Media Manager:
    void onDeviceAttached(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, bool autoplay);
    void onDeviceRemoved(MEDIA_SOURCE_T source, MEDIA_TYPE_T type);

    // Engines
    void setTuneMode(bool on) { m_bTuneMode = on; }
    void setFullScreenMode(bool on) { m_bFullScreenMode = on; }
    void onSetPassPlayState(bool on) { m_bPassPlayState = on; } // added by sjhyun 2013.09.22 for ITS 190471

    void onSetInitialDisplayValue(); //added by shkim for ITS 189354 Display Setting value Issue

    void onDiscLoading();
    void onNotifyIPodReady(MEDIA_SOURCE_T source);
    void onAvModeChanged(MEDIA_SOURCE_T source,bool);
    // { modified by wspark 2013.03.27 for ISV 77371
    //void onUpdateMP3CDTrackInfo(int track); // added by wspark 2012.11.07 for updating mp3cd track info correctly.
    void onUpdateTrackInfo(int track);
    // } modified by wspark
    // void onAuxAudioActivated(); // deleted by wspark 2012.12.10 for sending bluetooth opstate
	// { modified by ravikanth 27-12-12
    void onBrightnessChanged(int brightness);
    void onTintChanged(int tint);
    void onHueChanged(int hue);
    void onContrastChanged(int contrast);
	// } modified by ravikanth 27-12-12

    void onUpdateMainMenuAfterBoot(); // added by kihyung 2013.2.4 for ISV 71353
    void displayNoMediaFileOSD(MEDIA_SOURCE_T source );//added by edo.lee 2013.02.18
    void onNotifyBtCallProgressing(bool value); // added by wspark 2013.03.16
    void onClearTpMessage(); // added by wspark 2013.04.09 for cluster IOT #37
    // { modified by wspark 2013.04.30 for cluster IOT
    //void onSendTpMessage(QString); // added by wspark 2013.04.09 for cluster IOT #34
    void onSendTpMessage(QString, int);
    void onChangeBasicControlEnable(bool isBasicControlEnable);
    // } modified by wspark
    void onRetranslateUI(); // added by cychoi 2013.09.03 for ITS 187585
    void onPlayListChanged(); // added by wspark 2013.04.21 for cluster IOT
	 void StopOSDForClone();// added by edo.lee 2013.04.10
    void onSendTpMessage(); // added by wspark 2013.04.15 for EVT_UPDATE_CLUSTER
    //{ added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
    bool GetCDDAMataInfo( ){return m_CDDAMataInfo; }
    void SetCDDAMataInfo(bool val ){ m_CDDAMataInfo = val; }
    //} added by yongkyun.lee 20130416 

    GSTAudioRouterDBus* getGSTAudioRouterDBus();

    // { moved by kihyung 2013.07.05 for ISV 84474
    bool GetSharedMemoryInfo(int offset, unsigned char * pBuf, int size);
    bool SetSharedMemoryInfo(int offset, unsigned char * pBuf, int size);    
    // } moved by kihyung 2013.07.04

    void updateAFWScreenSetting(); // added by kihyung 2013.07.17 for NEW AFW 
    
private:
    void UpdateSDP_DeviceStatus(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, bool isConnected);

    void ResetModeData();
    void ResetDeviceData(MEDIA_SOURCE_T device);
    void ResetTrackData(MEDIA_SOURCE_T device);

    int GetClusterState();
    void SendClusterData(quint64 data);
    bool sendCanTpMessage(quint16 id, QString data); // added by wspark 2012.09.18 CAN TP Message
    void SendClusterData_OpStateOnly(quint64 opState); // added by wspark 2012.09.14 CAN unused signals.
    void SendClusterData_Progress(int position);
    // void SendMostData_Progress(int position); // Removed by Radhakrushna 2012.25.12 CR 13924
    void SendMostData_State();
    // { remvoed by kihyung 2012.11.26. removed spec.
    /*
    void updateVideoAVWidget();
    // { added by Sergey Vetugov for CR#13601
    void hideVideoAVWidget();
    // } added by Sergey Vetugov for CR#13601
    */
    // } remvoed by kihyung 2012.11.26. removed spec.
    //void updateVideoOSD();
    void updateVideoFileManager();
    void updateAudioFileManager();
    // { remvoed by kihyung 2012.11.26. removed spec.
    /*
    void StopAVWidgets();
    void updateAVWidget();
    */
    // } remvoed by kihyung 2012.11.26. removed spec.
    //move public	void DisplayAudioTrackOSD(); //added by edo.lee 2012.11.30
    //void DisplayAudioInfo_Tune(QString title, QString number); //added by edo.lee 2012.11.30
	//{modified by edo.lee 2013.01.03
	//void DisplayVideoInfo(); //added by edo.lee 2012.11.30
        //void DisplayVideoInfo(); //modified by edo.lee 2013.03.04
	//}modified by edo.lee
        //void DisplyChangeTime(int nSeconds,  APP_MEDIA_MODE_T media);  //added by edo.lee 2012.12.06

	
    HomeScreenDef::MEDIA_SOURCE_T TranslateToHomeSource(MEDIA_SOURCE_T source);

    DISPLAY_T GetOSDDisplay();
	QString GetOSDDispString(); //added by edo.lee 2012.12.07 for statusbar
    void resetDisplayParamWrtState(APP_STATE_T mediaState); // added by ravikanth 27-12-12
	
    void notifySDPByVideoSource(MEDIA_SOURCE_T source);
    void notifySDPByVideoPlayingState(PLAYER_STATE state);
    void notifyFMVideoPlaybackChanged(PLAYER_STATE state); //added by Dmitry 12.07.13 for ISV85006
    void setOSDFileFolder(QString &fileName, QString &folderName);
    // void onUpdateDvdTrackInfo(int state); // modified by ravikanth - 12-09-25 - cr 13789 // remvoed by kihyung 2012.11.26. removed spec.
	
	// { modified by ravikanth 27-12-12
    void setBrightness(int brightness);
    void setTint(int tint);
    void setHue(int hue);
    void setContrast(int contrast);
    void InitilizeDisplayParam();
	// } modified by ravikanth 27-12-12

    // { moved by kihyung 2013.07.05 for ISV 84474
    // { added by wspark 2013.04.23 for cluster IOT
    // bool GetSharedMemoryInfo(int offset, unsigned char * pBuf, int size);
    // bool SetSharedMemoryInfo(int offset, unsigned char * pBuf, int size);
    // } moved by kihyung 2013.07.04    
    bool IsAvModeFromSharedMemory();
    // } added by wspark
    
    // { added by junam 2012.11.26 for CR15923
    QDBusInterface * getMostJukeboxAdaptor();
    QDBusInterface * getMostDiscAdaptor();
    QDBusInterface * getMostAuxInAdaptor();
    // } added by junam

	MEDIA_TYPE_T GetMediaType(APP_MEDIA_MODE_T mode); // added by eugene.seo 2013.02.22
	
    // Application environment:
    AppMediaPlayer_EngineBase* m_pEngines[APP_MEDIA_MODE_MAX];
    AppMediaPlayer_Video_Controller* m_pVideoCtrl;
    APP_MEDIA_MODE_T m_mode;
    APP_MEDIA_MODE_T m_activeMode; // added by cychoi 2013.09.03 for cluster update on device changed
    APP_MEDIA_MODE_T m_modeforPhoto; // added by wspark 2013.04.25 for cluster IOT
    MEDIA_SOURCE_T m_device;
	MEDIA_SOURCE_T m_activeDevice; // added by cychoi 2013.09.03 for cluster update on device changed
    MEDIA_SOURCE_T m_BackupOSDDevice;// added by yongkyun.lee 20130626 for : ITS 176114

	QString m_strOSDChangeTime; //added by edo.lee 2012.12.06 for statusbar

        //bool m_bPlayNextTrack;//added by edo.lee 2013.03.25//delete for Static Test 2014.07.07

    // Cluster interface:
    CQCANController_ApplnTxRx* m_pQCanController;
    QDBusInterface* m_pCanTPInterface; // added by wspark 2012.09.18 CAN TP Message

    // MOST Manager interfaces:
    QDBusInterface *m_pMostJukeboxAdaptor;
    QDBusInterface *m_pMostDiscAdaptor;
    QDBusInterface *m_pMostAuxInAdaptor;

    // EngineMain links:
    AppMediaPlayer_EngineMain *m_pEngineMain;
    AppMediaPlayer_MediaManager *m_pMediaManager;

//    // Shared data provider:
//    SharedDataProvider *m_pSharedAudioData;
//    SharedDataProvider *m_pSharedPhotoData;
//    SharedDataProvider *m_pSharedVideoData;
    // AppMediaPlayer_MainUtils *m_pMainUtil; //ys-20131007 is not used
	// { modified by ravikanth 27-12-12
    DualDisplayHelper* m_pDDHelper;

    typedef struct
    {
        int m_brightness;
        int m_tint;
        int m_hue;
        int m_contrast;
    } MEDIA_DISPLAY_PLANE_DATA;
	// } modified by ravikanth 27-12-12

    // Controllers' data storage:
    typedef struct
    {
        PLAYER_STATE m_state;
        int m_position;
        int m_duration;
        int m_number;
        int m_quantity;
        //{ modified by yongkyun.lee 2013-07-15 for : ITS 179776
        int m_playingScanMode;
        int m_playingRandomMode;
        int m_playingRepeatMode;
        //} modified by yongkyun.lee 2013-07-15 
        QString m_title;
        QString m_album;
        QString m_artist;
        QString m_fullFolderName;
        QString m_folderName;
        QString m_videoTpMessage; // added by wspark 2013.03.28 for ISV 77376
    } DEVICE_DATA;

    DEVICE_DATA m_devices[MEDIA_SOURCE_MAX];
    // removed by Sergey for CR#15771

	// { added by edo.lee 2013.01.23
	MEDIA_ENABLE_T m_MediaEnabled[MEDIA_TYPE_MAX][MEDIA_SOURCE_MAX];	//modified by edo.lee 2013.02.14
	// } added by edo.lee

    bool m_bTuneSameFile;   // added by yongkyun.lee  2012.10.17  for Current file is white color
    bool m_bTuneMode;
    bool m_bFullScreenMode;
    bool m_bPassPlayState; // added by sjhyun 2013.09.22 for ITS 190471
    //QTimer m_updatePositionTimer;
    bool m_bAVOffMode; // added by changjin 2012.10.04 for SANITY_CM_AJ338
    // { removed by junam 2012.11.28 for CR14466
    //bool m_bDelayOSDUpdate; //added by changjin 2012.10.15 for SANITY_CM_AG056
    //bool m_bCancelUpdateOSD; //added by changjin 2012.10.22 for CR 14445
    // } removed by junam
    //bool m_OSDDisplay; // added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD      
    //QString m_tuneCounter; // added by junam 2012.11.14 for tune update osd
    //bool m_OSDTreckSpeed; // added by yungi 2012.11.14 for ISV 62667,ISV 62596
    //bool m_bAuxForBoot; // added by lssanh 2013.01.26 for ISV68519

        //QDeclarativeView* m_pOSDView; //delete by ys 2013.09.30 //added by edo.lee 2012.12.04 for new statusbar
	// { modified by ravikanth 27-12-12
    int m_currentBrightness;
    int m_currentTint;
    int m_currentHue;
    int m_currentContrast;
    APP_STATE_T m_currentVideoMediaState;
    MEDIA_DISPLAY_PLANE_DATA m_displayPlaneData[APP_STATE_MAX];
    AppMediaPlayer_VideoSettings* m_pSettingsStorage;
	// } modified by ravikanth 27-12-12
	DISPLAY_PLANES_T m_displayPlane;
    bool m_bSendingCanProgressMessage; // added by wspark 2013.02.19
    bool m_bBtCallProgressing; // added by wspark 2013.03.16
    bool m_bDirTreeFetched; // added by cychoi 2013.11.18 for ITS 208698 MP3 Play Info on Cluster 
    bool m_isScanMode;//added by edo.lee 2013.04.05
	bool m_bClusterUpdate; // added by wspark 2013.04.10 for cluster update
    bool m_bFirstUpdateTrack; // added by wspark 2013.04.14 for cluster IOT #40
    bool m_CDDAMataInfo;// added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
    bool m_bTpNeedToSend; // added by wspark 2013.04.17
    bool m_bBasicControlEnable;
    QSharedMemory mShmCurrentAVInfo; // added by wspark 2013.04.23 for cluster IOT

	QString m_SearchTitle; //added by edo.lee 2013.05.08
	QString m_DeckOSDStatus;// added by yongkyun.lee 20130612 for : ITS 173417-70953

    GSTAudioRouterDBus *m_pIPODAudioRouter;
    int m_autoTrackChangeIndex;

    DISPLAY_T m_gstPipelineDisplay; // added by kihyung 2014.01.14 for ITS 0218420
};

#endif
