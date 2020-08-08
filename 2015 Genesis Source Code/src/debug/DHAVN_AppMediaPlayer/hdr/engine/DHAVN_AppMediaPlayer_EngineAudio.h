/**
 * AppMediaPlayer_EngineAudio.h
 *
 * Implementation of the Class AppMediaPlayer_EngineAudio
 *
 * @author  Golyshkin A.
 * @CreatedDate 29-Nov-2011 1:55:11 PM
 */
#if !defined(EA_E12D33B7_E7B0_4f3c_ADC6_19CB26EEC8C5__INCLUDED_)
#define EA_E12D33B7_E7B0_4f3c_ADC6_19CB26EEC8C5__INCLUDED_

#include <QDeclarativeView>
#include <QTranslator>
#include <QApplication>
#include <QGraphicsObject>
#include <DHAVN_AppFramework_Event_Def.h>
#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include <uistate_manager_def.h>
#include <DHAVN_SharedDataProvider.h>
#include <DHAVN_GracenoteCDLookupInterface.h>
#include <DHAVN_AppMediaPlayer_Shared.h>
#include <DHAVN_QCANController_ApplnTxRx.h>
#include <DHAVN_OptionMenuModel.h>
#include <DHAVN_AppSettings_Shared.h>

#include "controller/DHAVN_AppMediaPlayer_ControllerAudio.h"
#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "engine/DHAVN_AppMediaPlayer_EngineBase.h"
#include "util/audio/DHAVN_AppMediaPlayer_CTrackerExtractorClient.h"

// { modified by yongkyun.lee@lge.com    2012.09.12 :CR13323, Change VR data struct type. 
#if 1
#define VR_MOUNT_PATH_LEN  32
typedef struct
{
    int bJukeboxData;          //each data valid/invalid decide
    int bUSBData;              //each data valid/invalid decide
    int bIpodData;             //each data valid/invalid decide
    char szUSBMountPath[VR_MOUNT_PATH_LEN];    //USB mount path // bUSBData is true case
    ETrackerAbstarctor_VolumeType eIpodConnectStatus; //This data is valid when bIpodData is true
    char szIpodSerial[VR_MOUNT_PATH_LEN];      //IPOD serial number
}MusicDBChangedStatus;
#else
typedef struct
{
    bool bJukeboxData;          //each data valid/invalid decide
    bool bUSBData;              //each data valid/invalid decide
    bool bIpodData;             //each data valid/invalid decide
    char szUSBMountPath[32];    //USB mount path // bUSBData is true case
    ETrackerAbstarctor_VolumeType eIpodConnectStatus; //This data is valid when bIpodData is true
    char szIpodSerial[32];      //IPOD serial number
}MusicDBChangedStatus;
 #endif//
// { modified by yongkyun.lee@lge.com      

// { added by jungae 2012.12.26, for send the connection information to AppBtPhone
#define MAX_IPOD_DEVICE_NAME_LENGTH 80

typedef struct
{
    int iPortNum;
    char cDeviceName[MAX_IPOD_DEVICE_NAME_LENGTH + 1];
    bool bConnectStatus;
}EVENT_IPOD_CONNECT_INFO_DATA_T;
// } added by jungae

typedef struct
{
   int iPortNo;
}EVENT_IPOD_STATE_CHG_INFO_T;

#ifdef SIRI_TIMEOUT_TEST
#define MAX_IPOD_MUSIC_APP_NAME  128
typedef struct
{
    int  iPortNum;
    bool bIsDefaultMusicApp;
    char cMusicAppName[MAX_IPOD_MUSIC_APP_NAME + 1];
}EVENT_IPOD_DEFAULT_MUSIC_APP_T;
#endif
//{ modified by yongkyun.lee 2013-09-04 for :  BT Mode key - pause
typedef struct
{
    MODE_STATE_T prevMode;
    MODE_STATE_T currMode;
    bool         firstPlayOnModeChange;//{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
}modeStateHistory;
//} modified by yongkyun.lee 2013-09-04 


class AppMediaPlayer_EngineAudio : public AppMediaPlayer_EngineBase
{
   Q_OBJECT

public:
   AppMediaPlayer_EngineAudio( AppMediaPlayer_EngineMain &aEngineMain );

   ~AppMediaPlayer_EngineAudio();

	void setOSDMetaData(OSD_DEVICE device, QString title, int shuffle , int repeat, int scan, bool isTune);
	void updatePosition(OSD_DEVICE device , int position, bool isLong = false);
	void updatePlayingMode(OSD_DEVICE device ,int shuffle , int repeat, int scan);
	void showCurrentOSD();
	void updateOSDMetaData(OSD_DEVICE device , QString title);
    void updateCDDAOSDMetaData(OSD_DEVICE device , QString title); // added by wonseok.heo for ITS 197433 2013.10.28

   void* GetUIState();
   void MoreInfoState(MP::DEVICE_T state);// added by eunhye 2012.09.10 for CR 13134

#ifdef SIRI_TIMEOUT_TEST
   void SendToBTDefaultMusicApp(MP::DEVICE_T source, bool bEnable, QString stAppName);
#endif

   virtual void ShowUI(DISPLAY_T display);
   virtual void HideUI(DISPLAY_T display);

   virtual void SaveSettings(bool bSync = false); // modified by kihyung 2013.08.09 for ITS 0183077
   virtual void LoadSettings();
   virtual void Restore(MODE_STATE_T mode);

   bool EventHandler( Event& aEvent, DISPLAY_T display );
   //virtual bool HandleSoftKeyBack(); remove by edo.lee 2013.03.30

   Q_INVOKABLE void SetMediaMode (MP::DEVICE_T eMode);

   Q_INVOKABLE QString GetCurrentFilePath();
   Q_INVOKABLE QString GetCurrentFileName();
   Q_INVOKABLE QString GetFilesCountInfo(int device = MP::UNDEFINED); //added by junam 2013.08.30 for ITS_KOR_186080
   Q_INVOKABLE bool isTuneTimer();//{ modified by yongkyun.lee 2013-11-06 for : ITS 206866
   Q_INVOKABLE int GetMediaNum();
   //Q_INVOKABLE QString GetCurrentCategoryId(); //added by aettie.ji 2012.12.18 for new ux

   Q_INVOKABLE void Play();
   Q_INVOKABLE void Pause();
   Q_INVOKABLE void Stop();
   Q_INVOKABLE void Toggle();		//added by hyochang.ryu 20130517
   Q_INVOKABLE void ResumeBT();		//added by hyochang.ryu 20130705

   //Q_INVOKABLE void NextTrack();
   Q_INVOKABLE void NextTrack(bool isUnsurpported = false); // modified by sangmin.seol 2013-12-13 for : ITS 0215489
   // { modified by lssanh 2012.09.18 for No35 car DQA#79  
   //Q_INVOKABLE void PrevTrack(bool isVRPrev);
   Q_INVOKABLE void PrevTrack(bool isDirectPrev);
   // } modified by lssanh 2012.09.18 for No35 car DQA#79 

   Q_INVOKABLE void GetDiscInfo();
//   Q_INVOKABLE void onTitleClicked(int title); // removed by eugeny.novikov 2012.11.10 for CR 14795
   Q_INVOKABLE void setPlayingPosition( int seconds );
   Q_INVOKABLE int  getPlayingPosition() const;
   Q_INVOKABLE int  getPlayingDuration() const;

// { removed by eugeny - 12-09-15
//   Q_INVOKABLE void setRandomPlayMode( bool needIncrement, int nRandomStatus = -1 );
//   Q_INVOKABLE int  getRandomMode() const;
//   Q_INVOKABLE bool isRandomOff() const;
//   Q_INVOKABLE void setScanMode();
//   Q_INVOKABLE void resetScan();
//   Q_INVOKABLE void setRepeatPlayMode( bool needIncrement, int nRepeatStatus = -1, bool byUser = true );
//   Q_INVOKABLE int  getRepeatMode() const;
// } removed by eugeny - 12-09-15
   void setBTSetInfo(bool set) ;
//{ modified by yongkyun.lee 2013-08-21 for : BT Mode key - unknown OSD
   Q_INVOKABLE bool getBTSetInfo();		//hyochang.ryu 20130821 P1#4
//} modified by yongkyun.lee 2013-08-21 

   Q_INVOKABLE bool isPaused() const;

// { modified by eugeny - 12-09-15
   Q_INVOKABLE void HandleFF4X();
   Q_INVOKABLE void HandleFF20X();
   Q_INVOKABLE void HandleRW4X();
   Q_INVOKABLE void HandleRW20X();

   Q_INVOKABLE void playIpodFiles();

   Q_INVOKABLE void normalPlay();
   //Q_INVOKABLE void cancelFFRW(); // removed by Dmitry 23.04.13
// } modified by eugeny - 12-09-15

   Q_INVOKABLE void requestMLTList();
   Q_INVOKABLE void playFromMLTList(int index);
//   Q_INVOKABLE void onSetOptionModel(int id_state); // removed by eugeny - 12-09-15
   // modified by ravikanth 17-05-13 for ISV 83180
   Q_INVOKABLE void SetOptionModel(QString UIState, bool editSate = false); // added by eugeny - 12-09-15

   Q_INVOKABLE void createPlayList( QStringList filePaths );

   //Q_INVOKABLE void updatePopupList();//removed by junam 2012.11.02 for unusing code.

   Q_INVOKABLE void setAlbumThumbnail (QUrl thumbnail);
   Q_INVOKABLE QString getLastPathOfString(QString fullString); //added by changjin 2012.12.10 for CR 16364

   Q_INVOKABLE QString makeElidedString( const QString &fullString,
                                        const QString &fontFamily,
                                        int pixelSize, int maxWidth );

   Q_INVOKABLE  int getStringWidth( const QString &fullString, const QString &fontFamily, int pixelSize ); // added by junggil 2012.10.04 for CR13505 Media Apps - delta UIS 2.15 to 2.17

   Q_INVOKABLE void LaunchHMISettings(bool isFront); //modified by edo.lee 2013.05.06
   Q_INVOKABLE void NofityAudioPathChanged(int id_device); // added by lssanh 2012.08.24 for audio path

   /* Request all covers from mount point Jukebox or USB */
   Q_INVOKABLE void requestCovers ();//bool bFirst = true); //changed by junam 2013.11.06 for coverflow update
   
   Q_INVOKABLE bool tuneWheel( bool bTuneForward );
   Q_INVOKABLE void switchToTuned();

   Q_INVOKABLE void SetNotifyAllUnsupported(); // added by kihyung 2013.09.12 for ISV 90605
   
   Q_INVOKABLE void HandleBackKey();
   Q_INVOKABLE void HandleSoftBackKey(bool isFront, bool bRRC); //modified by aettie 20130620 for back key event
   Q_INVOKABLE void HandleSoftIpod(); // ITS 247904

   Q_INVOKABLE void LaunchBTSettings();

   Q_INVOKABLE void LaunchKeypadSettings(bool isFront); //modified by edo.lee 2013.05.06

   Q_INVOKABLE bool IsPlaying() const;
   Q_INVOKABLE bool isStopped() const;

   Q_INVOKABLE int getSongNameListTempCount();

   Q_INVOKABLE bool isMinerHandleRequired ( int percentage);

   Q_INVOKABLE void tuneReset();

   Q_INVOKABLE void updateCDTuneInfo();

   Q_INVOKABLE void clearTemps();
   
   Q_INVOKABLE QString popScreen();
   Q_INVOKABLE void pushScreen(QString strScreen);
   Q_INVOKABLE QString topScreen();
   Q_INVOKABLE void setAudioPathRPM();

   Q_INVOKABLE bool SaveLastState() { return m_bSaveState; }
   
   Q_INVOKABLE void onTuneTimerStop(); // added by junggil 2012.07.26 for CR11681

   //{changed by junam 2013.06.28 for ISV86231
   //Q_INVOKABLE void onTuneTimerStopByMenu();//added by edo.lee 2013.04.05  78867
   Q_PROPERTY (bool tuneEnabled READ getTuneEnabled WRITE setTuneEnabled);
   bool getTuneEnabled(void) const {return m_bTuneEnabled;};
   void setTuneEnabled(bool bEnable);
   //}changed by junam

   Q_INVOKABLE void onChapterScreenLaunched(bool show); // added by ravikanth
//{Commented by Alexey Edelev 2012.10.09 Fix for CR14251
// { added by lssanh 2012.12.05 ISV?64581
   Q_PROPERTY (bool isModeTabClicked READ isModeTabClicked WRITE modeTabClicked);
   Q_INVOKABLE bool isModeTabClicked(void) const {return m_TabClicked;}
// { added by lssanh 2012.12.05 ISV?64581
   Q_INVOKABLE void modeTabClicked(bool tabtouch ){m_TabClicked = tabtouch; }//{ added by yongkyun.lee 2012.10.30 : CR 13733  : Sound Path
//   Q_INVOKABLE void modeTabClicked(bool tabtouch ){m_TabClicked = tabtouch; }//{ added by yongkyun.lee 2012.09.25 : CR 13733  : Sound Path
//}Commented by Alexey Edelev 2012.10.09 Fix for CR14251
   Q_PROPERTY(OptionMenuModel* optionMenuModel READ getOptionMenuModel NOTIFY changeOptionMenuModel)
   OptionMenuModel* getOptionMenuModel() { return m_OptionMenuModel; }
//{ Modified by Radhakrushna No CR 20121201 , Handling of Jog from photo to music
   Q_INVOKABLE bool isForeground(){return m_AudioController.isForeground();}
//} Modified by Radhakrushna No CR 20121201 , Handling of Jog from photo to music
   Q_INVOKABLE void DisplayOSD(bool osd);//{ added by  yongkyun.lee 2012.11.12 for  Check Video/audio OSD

   //Add by changjin 2012.07.03 : Jog and Tune event should disable on Menu screen
   Q_PROPERTY (bool isMenuVisible READ isMenuVisible WRITE setMenuVisible);
   Q_INVOKABLE void setMenuVisible(bool bMenuVisible) {m_bMenuVisible = bMenuVisible;};
   Q_INVOKABLE bool isMenuVisible(void) const {return m_bMenuVisible;};
   //Add by changjin 2012.07.03

   // { modified by junggil 2012.07.26 for CR11681
   // Not able to stop scan using CCP/RRC
   Q_PROPERTY (bool isFocusPlaybackControl READ isFocusPlaybackControl WRITE setFocusPlaybackControl);
   Q_INVOKABLE void setFocusPlaybackControl(bool PlaybackControl) {m_focusPlaybackControl = PlaybackControl;};
   Q_INVOKABLE bool isFocusPlaybackControl(void) const {return m_focusPlaybackControl;};
   // } add by junggil

   // { added by junam 2012.12.10 for CR16482
   Q_PROPERTY (QString stateUI READ getStateUI WRITE setStateUI);
   QString getStateUI(void) const {return m_stateUI;}
   void setStateUI(QString state) {m_stateUI = state;}
   // } added by junam

   
	Q_INVOKABLE void setIsBackPressByJog(bool state) {m_isBackPressByJog = state;} //added by edo.lee 2013.03.30

   /**
   * This function is resposible for stoping drawing AV widgets in home screen
   */
   // Q_INVOKABLE void stopAVWidgets(); // remvoed by kihyung 2012.11.26. removed spec.

   AppMediaPlayer_Controller_Base* GetController() { return &m_AudioController; };

   Q_INVOKABLE void notifyFormatJukeboxBegin();
   Q_INVOKABLE void notifyFormatJukeboxEnd();
   Q_INVOKABLE void ClearOSD(bool isFront);//modified by edo.lee 2013.07.07


   // { removed by junam 2013.12.16 for deprecated
    //Q_PROPERTY (bool isSearchState READ isSearchState WRITE setSearchState);
    //Q_INVOKABLE void setSearchState(bool bSearchState) {m_bSearchState = bSearchState;}
    //Q_INVOKABLE bool isSearchState(void) const {return m_bSearchState;}

    //Q_PROPERTY (bool isMltState READ isMltState WRITE setMltState);
    //Q_INVOKABLE void setMltState(bool bMltState) {m_bMltState = bMltState;}
    //Q_INVOKABLE bool isMltState(void) const {return m_bMltState;}
   // } removed by junam  

   // { addded by junam 2013.12.16 for ITS_ME_215036
   Q_PROPERTY (int focusIndex READ focusIndex WRITE setFocusIndex);
   int focusIndex() const { return m_focusIndex; }
   void setFocusIndex(int index) { m_focusIndex = index;}
   // } added by junam

   //{added by junam 2013.06.24 for ISV_KR_85467
   Q_PROPERTY (bool isCategoryEditMode READ getCategoryEditMode WRITE setCategoryEditMode);
   void setCategoryEditMode(bool bMode) {m_bCategoryEditMode = bMode;}
   bool getCategoryEditMode(void) const {return m_bCategoryEditMode;}
   //}added by junam

   // { added by eugeny.novikov 2012.11.13 for CR 15121
   Q_PROPERTY(bool isKeypadSettings READ isKeypadSettings WRITE setIsKeypadSettings);
   Q_INVOKABLE bool isKeypadSettings() const { return m_bIsKeypadSettings; }
   Q_INVOKABLE void setIsKeypadSettings(bool bFlag) { m_bIsKeypadSettings = bFlag; }
   // } removed by eugeny.novikov

   Q_PROPERTY(bool rewReachedFirst READ rewReachedFirst WRITE setRewReachedFirst NOTIFY rewReachedFirstChanged); // added by Sergey 28.05.2013
   bool rewReachedFirst() { return m_bRewReachedFirst; } // added by Sergey 28.05.2013
   Q_INVOKABLE void setRewReachedFirst(bool); // added by Sergey 28.05.2013

   Q_INVOKABLE bool IsGracenoteValid();
   // { added by edo.lee 2013.08.12 ISV 88908
   Q_INVOKABLE void setFileManager(bool isFront);
   Q_INVOKABLE void setFileManagerBG();
   // { added by edo.lee 2013.08.12
   bool             IsAudioMode(MODE_STATE_T nMode); // modified by kihyung 2013.07.20 for ITS 0179388  
   // { added by kihyung 2012.08.01 for CR 12268
   // Q_INVOKABLE bool IsPlayedbyAVModeChanged(); // removed by ruindmby 2012.08.20
   // } added by kihyung
   
   // { added by kihyung 2012.08.01 for CR 9376
   // Q_INVOKABLE void SetPlayedByAVModeChanged(bool bPlayed); // modified by ruindmby 2012.08.20
   // } added by kihyung   

   // added by ruindmby 2012.08.20
   void setLastAVMode(MODE_STATE_T mode) { m_nLastAVModeEvent = mode; }
   MODE_STATE_T getLastAVMode() { return m_nLastAVModeEvent; } // added by kihyung 2013.3.26 for ISV 77428
   // added by ruindmby 2012.08.20

// { add by yongkyun.lee@lge.com  2012.08.28  for :CR13323 ,VR Create START/END
   // { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
   //void sendVRTrackerChanged(bool isMinerFinished ,MusicDBChangedStatus *changeDB);
   //void sendVRTrackerChanged(bool isMinerFinished ,MP::DEVICE_T newDevice , bool isXsmapa);// deleted by yongkyun.lee 20130206 for : NO VR: VR is not using Mining  sig.
   int getVolumeId( QString path );
   int  DeviceConverter(int deviceType);
//{ deleted by yongkyun.lee 20130206 for : NO VR: VR is not using Mining  sig.
/*   bool DeviceMiningBitMask(MP::DEVICE_T newDevice ,vrBitShift isShift);
   bool DeviceUnminingBitMask(MP::DEVICE_T newDevice ,vrBitShift isShift);
   bool CheckMiningBitMask(MP::DEVICE_T newDevice);
   bool ClearMiningMask(MP::DEVICE_T newDevice) ; 
   bool MiningMask(MP::DEVICE_T newDevice ,vrBitShift isShift);
   // } modified by  yongkyun.lee 
   MusicDBChangedStatus *   makeDataVRTracker( MP::DEVICE_T newDevice);
// } add by yongkyun.lee@lge.com
*/
//} deleted by yongkyun.lee 20130206
   // { added by kihyung 2012.08.30 for DVD-Audio
   Q_INVOKABLE void launchSettings(bool bDVDSetting, bool isDvdTabDimmed = false, bool isFront=false); //modfied by edo.lee 2013.05.06
   // } added by kihyung

   Q_INVOKABLE bool IsSourceAvailable(const QString& tabId);//Added by Alexey Edelev 2012.09.06. CR 9413

   Q_INVOKABLE bool selectTune(void); //Added by junam 2012.09.12 for CR13632 

   void ResyncGracenote(int targetVolume); // added by ruindmby 2012.09.17 for CR11770

   // { added by ravikanth - 12-09-26
   Q_INVOKABLE void storeDVDdata();
   Q_INVOKABLE void restoreDVDdata();
   // } added by ravikanth - 12-09-26
   
   Q_INVOKABLE bool isAudioOff() { return m_bAudioOff; }
   void setAudioOff(bool bOff) { m_bAudioOff = bOff; }
 
   //void setIsLoadingScreen(bool isVisible){m_AudioController.setIsLoadingScreen( isVisible);} // removed by junam 2013.12.10 for code clean
   DISPLAY_T getCurrentScreen() { return m_currentScreen; } // added by kihyung 2013.2.5 for ITS 147127

	//added by edo.lee 2013.02.01
	Q_INVOKABLE QString getBTDeviceName(){return m_btDeviceName;}
	Q_INVOKABLE bool getIsMetaInfo(){return m_isMetaInfo;}
	Q_INVOKABLE bool getIsRemoteCtrl(){return m_isRemoteCtrl;}
	//added by edo.lee

//{modified by aettie 2013.03.19 for Gracenote issue
//    Q_INVOKABLE  bool getGracenoteIndexingStatus(){return m_bGracenoteIndexingStatus;} //added by aettie.ji 2013.02.22 for ISV 71131 etc. 
//deleted by aettie 20131010 unused code
//    Q_INVOKABLE  bool getGracenoteIndexingStatus(int devType);
	void GracenoteInvalidOnCDTocReceived(bool gracenoteState);
	QString GetCDToc();
	bool IsGracenoteInvalidOnCDTocReceived();
//}modified by aettie 2013.03.19 for Gracenote issue
//	Q_INVOKABLE  void setIsFrontView(bool isFront);	//remove by edo.lee 2013.03.21
//{Commented by Alexey Edelev 2012.10.08
//   // { added by junggil 2012.10.04 for CR13505 Media Apps - delta UIS 2.15 to 2.17
//   bool handleSettingsCommand( EVENT_SETTINGS_CHANGED_DATA_T *eventData );
//   Q_INVOKABLE  bool getScrollingTicker() const {return m_bScrollingTicker;}
//   // } added by junggil
//}Commented by Alexey Edelev 2012.10.08
   void setMP3ListReadState(bool listRead);// added by yongkyun.lee 20130413 for : NO CR MP3 List 

   //{ added by junam 2012.10.08 for CR14112
    Q_PROPERTY(bool loadingOnCopy READ  loadingOnCopy NOTIFY jukeboxCopyInProgress); // modified by ravikanth 18-04-13
	// { modified by ravikanth 16-04-13
    Q_INVOKABLE bool isCopyInProgress() { return m_EngineMain.isCopyInProgress(); }
    Q_INVOKABLE void setCopyToJukeboxStatus(bool isInProgress, int usbType); // modified by eugene.seo 2013.05.29
    Q_INVOKABLE void CancelCopyJukebox();
    Q_INVOKABLE void CancelAudioCopy(bool isRemoved = false);
    bool loadingOnCopy();
	// } modified by ravikanth 16-04-13
    void setRescanProgressLoading(); // modified by ravikanth 20-04-13
    void ResetProgressLoading(); // modified by ravikanth 20-04-13

    void hideBTMusic(); // added by kihyung 2013.5.2

	void SetSystemMute();//added by edo.lee 2013.06.22

    MODE_STATE_T mapToModeState(APP_ENTRY_POINT_T aEntryPoint); // added by kihyung 2013.06.02

    Q_INVOKABLE void testModeOff(); //added by wonseok.heo 2013.07.04 disc in out test
    Q_INVOKABLE void testModeOn(); //added by wonseok.heo 2013.07.04 disc in out test
    void InOutTestEject(); //added by wonseok.heo 2013.07.04 disc in out test    

    Q_INVOKABLE bool isBTPaused() const; //added by wonseok.heo for ISV 86609 2013.07.20

    Q_INVOKABLE bool isBtCallAfterScan() const; //added by sh.kim 2013.08.09 for ITS 183042
    //deleted by yongkyun.lee 2013-10-11 for : ITS 191526
    virtual void factoryReset(); //added by junam 2013.08.07 for factory reset
    virtual void resetAll(); // added by Sergey 13.09.2013 for ITS#188762
    void deleteUserData(); //added by shkim for Factory Reset Delete User Data
    //{ modified by yongkyun.lee 2013-09-04 for :  BT Mode key - pause
    MODE_STATE_T getModeStateHistoryPrev();
    MODE_STATE_T getModeStateHistoryCurr();
    //{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
    bool getModeStateHistoryFMode();
    void setModeStateHistoryFMode(bool val);
    //} modified by yongkyun.lee 2014-03-13
    bool getModeStateAVP();
    //} modified by yongkyun.lee 2013-09-04 
    bool getFFREWState(){return m_AudioController.GetFfRewState();}//added by edo.lee 2013.09.05 ITS 0188286
  void setMaintainPause(bool value){m_maintainPause = value;} //added by edo.lee 2013.10.16
   bool isMaintainPause(){return m_maintainPause;}//added by edo.lee 2013.10.16
	//{ modified by yongkyun.lee 2013-10-16 for :  ITS 194947
    bool getNextHardKeyPressed() const { return m_bNextHardKeyPressed;}
    bool getPrevHardKeyPressed() const { return m_bPrevHardKeyPressed;}
    //} modified by yongkyun.lee 2013-10-16 
    //void clearTuneDelayTimer(); // removed by junam 2013.11.29 for tune

    Q_INVOKABLE void showListView(bool bShow); //added by junam 2013.12.19 for LIST_ENTRY_POINT
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    void showDeckHighTempPopup();
    void closeDeckHighTempPopup();
    Q_INVOKABLE void sendDeckErrorToUISH();
    // } added by cychoi 2014.04.09
    void setPlayAfterBTCalliPod(bool value) { m_bPlayAfterBTCalliPod = value; } // ITS 240742_240746

public:
    //{changed by junam 2013.09.07 for ITS_KOR_185529
    //Q_PROPERTY (bool isSeekHardKeyPressed READ getSeekHardKeyPressed WRITE setSeekHardKeyPressed);
    Q_PROPERTY (bool isNextHardKeyPressed READ getNextHardKeyPressed WRITE setNextHardKeyPressed);
    Q_PROPERTY (bool isPrevHardKeyPressed READ getPrevHardKeyPressed WRITE setPrevHardKeyPressed);
    //}changed by junam
private:
//{modified by aettie 2013.03.19 for Gracenote issue
   //bool m_bGracenoteIndexingStatus;  //added by aettie.ji 2013.02.22 for ISV 71131 etc.
   //deleted by aettie 20131010 unused code
   //bool m_bGracenoteIndexingStatus_Jukebox;
  // bool m_bGracenoteIndexingStatus_USB1;
  // bool m_bGracenoteIndexingStatus_USB2;
  // bool m_bGracenoteIndexingStatus_DISC;
//}modified by aettie 2013.03.19 for Gracenote issue   

   //{changed by junam 2013.09.07 for ITS_KOR_185529
   //bool m_bSeekHardKeyPressed;
   //bool getSeekHardKeyPressed() const { return m_bSeekHardKeyPressed;}
   //void setSeekHardKeyPressed(const bool bSeekHardKeyPressed) {m_bSeekHardKeyPressed = bSeekHardKeyPressed; }

   void PassNotifyAvModeToiPod(MODE_STATE_T eNextMode);


   bool m_bNextHardKeyPressed;
   //removed by yongkyun.lee 2013-10-16 for :  ITS 194947
   void setNextHardKeyPressed(const bool bNextHardKeyPressed) {m_bNextHardKeyPressed = bNextHardKeyPressed; }

   bool m_bPrevHardKeyPressed;
   //removed by yongkyun.lee 2013-10-16 for :  ITS 194947
   void setPrevHardKeyPressed(const bool bPrevHardKeyPressed) {m_bPrevHardKeyPressed = bPrevHardKeyPressed; }
   // } changed by junam

   void notifyCopyToJukeboxBegin(); // modified by ravikanth 16-04-13
   void notifyCopyToJukeboxEnd(); // modified by ravikanth 16-04-13
   EVT_ID_T resetEventForListView(EVT_ID_T eventId); // modified by ravikanth 25-04-13 for ITS 0175705

signals:
   void updateBTPlayTime( int playtime );
   void updateBTTrackName( QString trackName );
   void updateBTArtist( QString artist );
   void updateBTTrackCount( QString track_count_string );
   void updateBTDeviceName( QString deviceName, bool bIsMetaInfo, bool bIsRemoteCtrl ); //modifeid by edo.lee 2012.09.18 for New UX : Music (LGE) # 43
   void updateBTCoverArt(bool status, QString imagePath);
   void commandFromBT( int command );
   void mp3ListReadState(bool listRead);// added by yongkyun.lee 20130413 for : NO CR MP3 List 

   void playCommandFromVR();
   void pauseCommandFromVR();
   void nextCommandFromVR();
   void prevCommandFromVR();  
   //void searchCommandFromVR();// add by yongkyun.lee@lge.com 2012.07.24 for CR 11593 //20131016 is not used
   //void releaseLongPress();    // removed by junam 2012.10.11 to use clearSeekMode instead.
   void seekNextHardkeyPressed();
   void resetPlaybackControl();//{ modified by yongkyun.lee 2013-10-17 for : 196623
   void seekPrevHardkeyPressed();
   void ejectHardkeyPressed(int mode);

   void jogHardkeyPressed(int jog_key);
   void jogsendKeyTovc(int jog_key);
   void jogWheel(QVariant arrow);
   void startMusicList();
   void closeListView(); 
   
   // { removed by junam 2012.12.05 for moving to activateTab() function
   // { modified by eugeny.novikov 2012.10.11 for CR 14229
   //Added by Radhakrushna 20121009 cr 14261
   //void handleDeviceRemoved(int tabId);
   // } modified by eugeny.novikov
   // } removed by junam 

   void retranslateUi( QVariant languageId);
   
   //{added by junam 2013.03.26 for ISV72425
   //void tuneTimerOff(QString art, QString tit, QString alb, QString fol, QString gen, QString com, QString cov );
   void tuneTimerOff(QString art, QString tit, QString alb, QString fol, QString cov );
   //}added by junam

   void signalPowerOff();

   void backKeyPressed();
   // { modified by wspark 2012.08.02 for CR12020
   //void menuKeyPressed();
   void menuKeyPressed(int nKeyType);
   // } modified by wspark
   void menuOptionClose(); // added by lssanh 2013.02.20 ISV72878 + option menu close
   void scrollAccStop(); // added by lssanh 2013.02.26 ISV73837
   void changeOptionMenuModel();

   void NotifyUISH(Event evt);
   void NotifyUISH(Event evt, DISPLAY_T aDisplayType); // modified by kihyung 2013.1.26

   // { added by kihyung 2012.08.22 for new UX MUSIC(LGE) #3
   //deleted by aettie 20131010 unused code
   //void startGracenoteIndex(int nDevice);
   //void endGracenoteIndex(int nDevice);
   //void killedGracenote(); //added by aettie for Gracenote killed handling	


   // } added by kihyung
   void activateBTTab( int tabId, bool isVisible, bool isSelected );// added by edo.lee 2012.08.23
   void deactivateBTTab(  );	//added by hyochang.ryu for ITS185528
   void moreInfoChanged(bool tmp); //added by eunhye 2012.09.10 for CR 13134
//   void setDefaultMusicPlayer(); // removed by eugeny.novikov 2012.11.13 for CR 15121 //added by changjin 2012.09.18 for CR 11792
   void bgReceived(); // added by ruindmby 2012.09.21 for CR 13802
// { added by jungae 2012.11.22 No CR
// It's DQA issue. Disappear indexing popup issue before the not completed media-indexing. 
   void fgReceived();
// } added by jungae
   void clearSeekMode(); // change name by junam 2012.10.11
   void clearSeekModeSoftKey(); //added by junam 2012.10.15 for CR14476 
   void setDisplayOSD( bool vOSD ); // added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD        

   void signalTunePressed(); //added by junam 2013.07.12 for ITS_177486
   void signalTuneReleased();
   void signalTuneCanceled(); // DUAL_KEY added for TunePressed Cancel.

   void signalTuneNavigation( uint arrow, int status );
   void signalJogNavigation( uint arrow, int status, bool bRRC, int nRepeat ); //changed by junam 2013.11.13 for add repeat

   void rewReachedFirstChanged(bool); // added by Sergey 28.05.2013

   //void notifyBtCallProgressing(bool value); // added by wspark 2013.03.16 // deleted by wspark 2013.04.10 for cluster update

   void jukeboxCopyInProgress(); // modified by ravikanth 18-04-13
   void hideBasicBTMusic(); // added by kihyung 2013.5.2
   void UpdateStateTrackOSD(int speedCheck);  //added by junam 2013.05.18 for ISV_KR81848   

   void sendTestModeStatus( int mode); //added by wonseok.heo 2013.07.04 disc in out test
#ifdef BOOT_LOADING_SCREEN
   void isReadytoPlay();
#endif
#ifdef DUAL_KEY
   uint signalTransformEVT2QML_KEY(EVT_ID_T key);
   //void signalJogNavigation( uint arrow, int status, bool bRRC );
#endif
   void closeUnsupportedPopup(); // added by Dmitry 11.10.13 for ITS0194940   
   void showPopup(int type); //added by junam 2013.11.28 for ITS_CHN_211039
   void setDraggable(bool isDraggable);  // added ITS 210706,7 for disable iPod UI during a BT call
   void closeCopyCancelConfirmPopup(); // modified for ITS 0217509
   void cancelPressedControlCue(); // added sangmin.seol 2014.03.11 for ITS 0225385 H/U Tune Key Cancel Fix

public slots:
   void SendCommandToBT(int);
   void onSetAppState(MP::DEVICE_T state);
   void onSetStatusDataByIndex (MP::DEVICE_T state, DATA_TYPE_T index, int aValue );
   void MusicIDCDDataReceived(bool status);
   //void slotSwitchToTune();//removed by junam 2013.11.29 for tune
   void onGracenoteIndexingStatus(bool status, int deviceType); // added by kihyung 2012.08.22 for new UX MUSIC(LGE) #3
   void onGracenoteKilled(); //added by aettie for Gracenote killed handling
private:
   void tuneHandler( int eventId, const Event &aEvent );
   void setOptionMenuModel(OptionMenuModel* optionMenuModel);

   virtual APP_ID_T GetThisAppId() const { return APP_AVP; }
//{ Added by radhakrushna Mohanty 2012.08.18 CR 11181
   bool processedInAUXMode(Event &aEvent);
//} Added by radhakrushna Mohanty 2012.08.18 CR 11181
   bool processedInBTMode(Event &aEvent);		//added by hyochang.ryu 20130704
   bool processedIpodIndexing(int nEventID); //added by eunhye 2012.8.31 for CR13047
   bool processedInDiscMode(); // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
   void cancelJukeboxFormat();

private:
//{ added by yongkyun.lee 2012.12.19 for CR 14219 ,POPUP/list close
    bool m_isAccOn;
    bool m_isFrontLCD;
//} added by yongkyun.lee     
   
   unsigned short m_vrMiningMask; // added by  yongkyun.lee 2012.11.21 for  xSampa check.
   AppMediaPlayer_EngineMain &m_EngineMain;
   AppMediaPlayer_Controller_Audio m_AudioController;

   //int m_BtTrackButton;  // commented by cychoi 2014.03.05 for unused code // added by hyochang.ryu 20130517 for BT OSD
	//added by edo.lee 2013.02.01
   QString m_btDeviceName;
   bool m_isMetaInfo;
   bool m_isRemoteCtrl;
   //added by edo.lee

   QTranslator *m_pTranslator;
   MP::DEVICE_T m_mode;
   bool m_bAudioOff;
   bool m_bSaveState;
   OrgLgeGracenoteCDLookupDBusInterface *m_pGraceNote;
   QString m_sToc;
   bool m_bToc; //added by aettie 2013.03.19 for Gracenote issue
   QTimer *m_tuneSelectTimer;
   QTimer *m_tuneDoublePressTimer; //added by WJL 2014.04.15 for ITS 228616
   QTimer *m_tunePressDelayTimer; // added by suilyou 20131025
   //QTimer m_tuneRequestTimer; //removed by junam 2013.03.10 for ISV73458

   //CQCANController_ApplnTxRx* m_pQCanController; //delete by ys-2013.09.30
   OptionMenuModel *m_OptionMenuModel;
   QStack<QString> m_screenStack;
   bool m_bMenuVisible; //Add by changjin 2012.07.03 : Jog and Tune event should disable on Menu screen
   bool m_bBasicView; // added by kihyung 2012.07.09 for CR 11165
   //bool m_bSearchState; // remvoed by junam 2013.12.16 for deprecated
   //bool m_bMltState; // removed by junam 2013.12.16 for deprecated
   int m_focusIndex; // added by junam 2013.12.16 for ITS_ME_215036
   bool m_focusPlaybackControl; // added by junggil 2012.07.18 for CR12028
   //bool m_bPlayedbyAVModeChanged; // removed by ruindmby 2012.08.20
   MODE_STATE_T m_nLastAVModeEvent; // added by ravikanth for changes - CR 12929, 12935 (alternative for CR 12182)
   MusicDBChangedStatus m_changeDB; // added by yongkyun.lee@lge.com    2012.09.12 : CR13323,Change VR data struct type.
//{Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side
////{Changed by Alexey Edelev 2012.10.09 Fix for CR14251
//   bool m_AvModeChangeMutex; //{ added by yongkyun.lee 2012.09.25 : CR 13733  : Sound Path
   bool m_TabClicked; //{ added by yongkyun.lee 2012.10.30 : CR 13733  : Sound Path
////}Changed by Alexey Edelev 2012.10.09 Fix for CR14251
//}Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side
//{Commented by Alexey Edelev 2012.10.08
//   bool m_bScrollingTicker; // added by junggil 2012.10.04 for CR13505 Media Apps - delta UIS 2.15 to 2.17
//}Commented by Alexey Edelev 2012.10.08
   bool m_bIsKeypadSettings; // added by eugeny.novikov 2012.11.13 for CR 15121
   // bool m_bCamInFG; // added by minho 20121114 for not sync status bar when turn on camera.
   QString m_stateUI; // added by junam 2012.12.10 for CR16482
   //QDeclarativeView* OSDview; //delete by ys 2013.09.30 added by edo.lee 2012.12.08 for statusbar

   DISPLAY_T m_currentScreen; // added by kihyung 2013.2.5 for ITS 147127
   //bool m_isFrontBack;//removed by edo.lee 2013.03.21
   bool m_isFrontSoftBackPressed; //added by edo.lee 2013.03.30
   bool m_isRearSoftBackPressed;//added by edo.lee 2013.03.30
   bool m_isBackPressByJog;//added by edo.lee 2013.03.30
   //QTimer *m_pClusterUpdateTimer; // added by wspark 2013.04.02 // deleted by wspark 2013.04.10 for cluster update
   bool m_MP3ListView;//{ added by yongkyun.lee 20130413 for : NO CR MP3 List 
   bool m_trackerUpdateInProgress; // modified by ravikanth 20-04-13
   bool m_bRewReachedFirst; // added by Sergey 28.05.2013
   bool m_bPlayAfterBTCall; //added by edo.lee 2013.06.20
   bool m_bCategoryEditMode; //added by junam 2013.06.24 for ISV_KR_85467
   bool m_isFrontFG; //added by edo.lee 2013.06.26 ITS 0176308 
   bool m_bTuneEnabled; //added by junam 2013.06.28 for ISV86231
   bool m_BtCallAfterScan; //added by sh.kim 2013.08.09 for ITS 183042
   bool m_bBTMusicInfoAfterBTCall; // added by cychoi 2014.03.05 for ITS 228131, ITS 228134 Request BT Music Info after BT call
   //{ modified by yongkyun.lee 2013-08-21 for : BT Mode key - unknown OSD
   bool m_BTSetInfo;	//hyochang.ryu 20130821 P1#4
   //deleted by yongkyun.lee 2013-10-11 for : ITS 191526
   //} modified by yongkyun.lee 2013-08-21 
   //bool m_bKeepFFREWOnBTCall;//remove by edo.lee 2013.09.25

   modeStateHistory m_modeStateHistory;//modified by yongkyun.lee 2013-09-04 for :  BT Mode key - pause
   bool m_maintainPause;//added by edo.lee 2013.10.16
   bool m_bPlayAfterBTCalliPod; // ITS 235835 separate from m_bPlayAfterBTCall
 private slots:
   /** @brief Slot to be called once user have not touched the tune wheel.
    * This slot is binded to the m_tuneSelectTimer and is called after a
    * 5 seconds after the last touch of the tune wheel. */
   void onTuneSelectTimer();
   void onTuneDoublePressTimer();
   void onDiscLoaded(bool);
   void onTocReceived(QString toc);

   /**
   * This function is resposible for updating drawing media progress bar in home screen
   * nSec - current playing second
   */
   void onDeviceAttached( MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType, bool autoplay);
   void onDeviceRemoved( MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType );
   void oncatreeRemoved(); //added by wonseok.heo for ITS 205774 2013.11.01
   //deleted by aettie 20131010 unused code
   //void SetGracenoteVolumeType(int type, QString id, QString path);
   void setStatusBarOnScreen(DISPLAY_T display );// add by yongkyun.lee@lge.com  2012.08.03  for CR 12118
   //deleted by aettie 20131010 unused code
   //void onSetGracenoteDeviceStateChanged(int volume, QString id, QString path,
   //                                      int startIndex, int endIndex, int syncType, bool isMounted);
   void onSetGracenoteHighlightedVolume(int volume, QString id, QString path);
   void onSetHardKeyDisable(bool isTrackChanged); //added by jeeeun for MLT play in FF/REW 2012.12.05
   void onIPodDeviceConnectInfo(int portNum, QString devName, bool connectStatus); // added by jungae 2012.12.26, for send the connection information to AppBtPhone
   void OniPodPlaying(int portNum, QString devName, bool connectStatus);
   //void onClusterUpdateTimer(); // added by wspark 2013.04.02 // deleted by wspark 2013.04.10 for cluster update
   void OnACCStatusChanged(bool accOff); // modified by ravikanth 05-10-13 for ITS 0193972
   void onSignalAgreeOSD(); //added by suilyou 20131125 ITS 0210790
   //void OnPowerNormalOff(); // modified for ITS 0210855 // removed by ITS 212424
   void OnPowerStateLogicOff(); // modified for 0212002

Q_SIGNALS:
   void retranslateUI(int langId);
// removed by Dmitry 23.04.13
};

#endif // !defined(EA_E12D33B7_E7B0_4f3c_ADC6_19CB26EEC8C5__INCLUDED_)
