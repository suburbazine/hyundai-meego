/**
 * AppMediaPlayer_EngineVideo.h
 *
 * Implementation of the Class AppMediaPlayer_EngineVideo
 *
 * @author  Golyshkin A.
 * @CreatedDate 29-Nov-2011 1:55:13 PM
 */
#if !defined(EA_53E3F1CF_656C_4dec_A8EC_11324F4C16E8__INCLUDED_)
#define EA_53E3F1CF_656C_4dec_A8EC_11324F4C16E8__INCLUDED_

#include <QObject>
#include <QTimer>

#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "engine/DHAVN_AppMediaPlayer_EngineBase.h"

#include "util/DHAVN_AppMediaPlayer_MediaManager.h"
#include <DHAVN_AppSettings_Shared.h>
#include "util/video/DHAVN_AppMediaPlayer_MainUtils.h"
#include "util/video/DHAVN_AppMediaPlayer_VP_Enums.h"	// added by Sergey 28.08.2013 for ITS#186507 
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"  // added by Sergey 01.09.2013 for ITS#186834 
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h" // added by Sergey 23.10.2013 for ITS#196877


#define TUNE_INTERVAL 5000

class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_Video_ScreenManager;
class AppMediaPlayer_VideoCaptionProvider; // modified by ravikanth 04-04-13
class AppMediaPlayer_Video_SModel_Playback;  // added by kihyung 2013.06.27

// { added by Sergey for CR#16015
typedef enum
{
    PLAY_REQUSTER_FM,
    PLAY_REQUSTER_TUNE,
    PLAY_REQUSTER_MAX
} PLAY_REQUESTER_T;
// } added by Sergey for CR#16015.

typedef enum
{
	VIDEO_FS_CAPTIONS,
	VIDEO_FS_DIVX,
	VIDEO_DVD_SETTING,
	VIDEO_VCD_PBC,
	VIDEO_VCD_SHORTCUT
}SUBSCREEN_T;

// { added by sjhyun 2013.11.01 for ITS 204921
typedef enum
{
    VCD_DIRECTACCESS_MODE_READY,
    VCD_DIRECTACCESS_MODE_END,
} VCD_DIRECTACCESS_MODE_T;
// } added by sjhyun

/** @brief The UI background for the video mode.
 * This class implements all UI logic of the video mode of the MediaPlayer.
 * The idea is that the UI is built around the state machine that reflects
 * the UI completely. Each UI state has a number of actions it can react on
 * without state transitions (stuff like play/pause) and the number of
 * actions that could lead to state change.
 * */
class AppMediaPlayer_EngineVideo : public AppMediaPlayer_EngineBase
{
    Q_OBJECT

    // { modified by Sergey 28.08.2013 for ITS#186507 
public:
    Q_PROPERTY(bool rewReachedFirst READ rewReachedFirst WRITE setRewReachedFirst NOTIFY rewReachedFirstChanged); // added by Sergey 28.05.2013

    Q_PROPERTY(VPEnum::VDISP_T pbOwner READ pbOwner WRITE setPBOwner);
    Q_INVOKABLE VPEnum::VDISP_T pbOwner() { return m_pbOwner; }
    Q_INVOKABLE void setPBOwner(VPEnum::VDISP_T value) { MP_MEDIUM << value << LOG_ENDL; m_pbOwner = value; } // modified by Sergey 24.10.2013 for ITS#197778
	// } modified by Sergey 28.08.2013 for ITS#186507 

    Q_PROPERTY(bool isSystemPopVisible READ isSystemPopupVisible WRITE setSystemPopupShow NOTIFY systemPopupShow); // added by Sergey 15.01.2013 for ITS#219649
    
    AppMediaPlayer_EngineVideo( AppMediaPlayer_EngineMain &aEngineMain );

    ~AppMediaPlayer_EngineVideo();

	// for OSD use
	void setOSDMetaData(OSD_DEVICE device, QString title, int shuffle , int repeat, int scan, bool isTune);
	void updatePosition(OSD_DEVICE device , int position, bool isLong = false);
	void updatePlayingMode(OSD_DEVICE device ,int shuffle , int repeat, int scan);
	void showCurrentOSD();
	void updateOSDMetaData(OSD_DEVICE device , QString title);
	int getOSDRandom(int random);
	int getOSDRepeat(int repeat);

    void * GetUIState();

    QString GetUSBPath( MEDIA_SOURCE_T source );

    virtual void ShowUI(DISPLAY_T display);

    virtual void HideUI(DISPLAY_T display);

    virtual void DeactivateUI(DISPLAY_T display) {};  // modified by Sergey 15.08.2013 for ITS#185556 

    virtual void SaveSettings(bool bSync = false); // modified by kihyung 2013.08.09 for ITS 0183077

    virtual void LoadSettings();

    virtual void Restore( MODE_STATE_T mode );

    virtual void factoryReset();	// added by Sergey 13.09.2013 for ITS#188762

    virtual void resetAll();	// added by Sergey 13.09.2013 for ITS#188762

    bool EventHandler( Event& aEvent, DISPLAY_T display );

    void notifyUISH(Event evt); // added by ravikanth	- CR 11552, 11938

    AppMediaPlayer_Video_ScreenManager*  getScreenManager(int disp);

    AppMediaPlayer_Video_ScreenManager*  getScrMgr(DISPLAY_T display); // added by Sergey 07.09.2013 for PRE_FG supoort

    AppMediaPlayer_Controller_Base* GetController() { return m_pController; };

    AppMediaPlayer_Video_Controller* GetVideoController() { return m_pController; }

    AppMediaPlayer_MainUtils* GetMainUtils() { return m_pMainUtil; }

    bool GetVariantRearUSB(); //[KOR][ITS][180278](aettie.ji)


    // Wrapper around AppMediaPlayer_MediaManger::IsSourceAvailable
    // for more clean access from UI & controllers code.
    bool IsSourceAvailable( MEDIA_SOURCE_T source );

    void launchFileManager( MEDIA_SOURCE_T mediaSource, QString fileName, QString folderPath , int display);//modified by edo.lee 2013.05.02
    Q_INVOKABLE void NofityAudioPathChanged (MODE_STATE_T id_device); // added by lssanh 2012.08.24 for audio path
//{ add by lssanh 2012.09.04 for ITS 136354 
    Q_INVOKABLE QString makeElidedString( const QString &fullString,
						  const QString &fontFamily,
						  int pixelSize, int maxWidth ,
						  Qt::TextElideMode elideMode = Qt::ElideRight );
//} add by lssanh 2012.09.04 for ITS 136354
    Q_INVOKABLE  int getStringWidth( const QString &fullString, const QString &fontFamily, int pixelSize ); // added by junggil 2012.10.05 for SANITY_CM_AJ497

    void PlayTrackFromFileManager( QString filePath, DISPLAY_T display ); // modified by Sergey 03.09.2013 for ITS#186054
    // removed by Sergey for CR#15775
    void launchSettings( AppSettingsDef::EAppSettingsStartState nState, int device, bool isDvdTabDimmed /*= false*/, int display); // modified by edo.lee 2013.05.02
    bool isLaunchingScreenSettings() { return m_bIsLaunchingScreenSettings != VIDEO_DISPLAY_MAX; } //modified by edo.lee 2013.12.13 ITS 0211957
    void setStatusBarOnScreen ( bool onScreen , bool isFront);

    AppMediaPlayer_EngineMain& getEngineMain() const {return m_EngineMain;}

    Q_INVOKABLE void notifyComplete( QObject *root, bool isFront );//modified by edo.lee 2013.02.19
//{Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side
    Q_INVOKABLE void modeTabClicked(bool tabtouch ){m_TabClicked = tabtouch; }//{ added by yongkyun.lee 2012.10.30 : CR 13733  : Sound Path
//    Q_INVOKABLE void modeTabClicked(bool tabtouch ){m_TabClicked = tabtouch; }//{ added by yongkyun.lee 2012.09.25 : CR 13733  : Sound Path
//}Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side

    Q_INVOKABLE bool IsPlaying() const;     // added by aettie.ji 2012.11.7 for New UX
    Q_INVOKABLE bool IsStopped() const;     // added by aettie.ji 2012.11.7 for New UX
    Q_INVOKABLE bool setPlaybackControlQMLIsLoaded(bool loaded){ return m_bIsPlaybackControlQMLIsLoaded = loaded; } // modified by sjhyun 2013.11.11 for ITS 206822
    Q_INVOKABLE bool setVideoPlaybackQMLlsLoaded(bool loaded){ return m_bIsVideoPlaybackQMLIsLoaded = loaded; } // added by sjhyun 2013.11.11 for ITS 206822

    void fullScreenModeStatusChanged(bool on);
    void backHandle(bool isFrontBack, bool isSoftKey = false); //modified by aettie 20130620 for back key event
    

    bool getVPWheelDisable(bool isFront); //Add by changjin 2012.07.17 : for CR 12032 : Additional fix for CCP/RRC on Video playback screen.

    bool isFMFile(DISPLAY_T disp) { return (m_isFMFile[disp] != FM_FILE_MAX); } // modified by Sergey 09.09.2013 for ITS#188283

    int getVideoControllerFoucusIndex(DISPLAY_T display); // added by changjin 2012.08.16 : for CR 12449

    // added by ruindmby 2012.09.11 for CR#13584
    void setLastAVMode(MODE_STATE_T mode) { m_nLastAVModeEvent = mode; }
    MODE_STATE_T getLastAVMode() { return m_nLastAVModeEvent; } // added by kihyung 2013.3.26
    // added by ruindmby 2012.09.11 for CR#13584
    void SetTuneSameFile(bool val); //added by yongkyun.lee  2012.10.17  for Current file is white color
    void setTuneModeOnStateChanged(bool val); // added by cychoi 2013.11.10 for ITS 207831 Control Cue Icon on Tune Mode

    Q_INVOKABLE  bool getScrollingTicker() const {return m_bScrollingTicker;} // added by junggil 2012.10.05 for SANITY_CM_AJ497
    // removed by Sergey for CR#15775
    Q_INVOKABLE bool isVideoInFG(); // added by kihyung 2012.11.08 for Camera & Video Overlap problem.
    bool isVideoInFG(DISPLAY_T disp); // added by Sergey 10.05.2013
    DISPLAY_T getCurrentScreen() { return m_currentScreen; } // modified by eugene.seo 2012.10.31 for Function_USB_0863
    bool DisplayOSD(bool vOSD);// added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD    
    // removed by Sergey 31.07.2013
    // { added by Sergey for CR#16015
    PLAY_REQUESTER_T playRequester() { return m_playRequester; }
    void setPlayRequester(PLAY_REQUESTER_T value) { m_playRequester = value; }
    // } added by Sergey for CR#16015.
    void HandleBackKey(); // added by wspark 2012.12.17 for ISV64433
    void ClearOSD();//added by edo.lee 2013.01.10
    //bool GetIsShowOSDByJog(){return m_isShowOSDByJog;}//remove by edo.lee 2013.02.28
    //void SetIsShowOSDByJog(bool isShowOSDByJog){ m_isShowOSDByJog=isShowOSDByJog;}//remove by edo.lee 2013.02.28
    // removed by Sergey 18.05.2013
    bool getIsFgReceived() { return m_isFgReceived; }; // modified by kihyung 2013.1.26
    // void setGstPipelineStatus(bool bStop); // removed by kihyung 2013.6.27    
    // removed by cychoi 2013.08.30 for ITS 187026 DVD Title/Disc Menu Loading Screen // added by lssanh 2013.05.18 ISV81619
    void DelayedPlay(bool bFromAFW = false); // added by kihyung 2013.1.29
    void setPBCOff(); // added by kihyung 2013.1.30
    bool isPBCOff(); // added by cychoi 2013.12.19 for ITS 216591 close VCD loading screen when changed play position
    Q_INVOKABLE void releaseTuneMode(); // modified by Sergey 19.05.2013
    void SetPlayBackControlbyTune();
    void releaseTuneModeByMenu(); //added by edo.lee 2013.04.05 78867
    //remove by edo.lee 2013.11.08
	void HideScreen(DISPLAY_T display);// added by edo.lee 2013.02.04
    //Q_INVOKABLE bool getIsLockoutMode(){return m_isLockoutMode;} //remove by edo.lee 2013.02.26

	// { modified by edo.lee 2013.05.29
    // { added by eunhye 2013.02.07 for ISV70720
    void setVideoListView(VIDEO_DISPLAY_T disp, bool isListView) { m_isListView = isListView; m_videoListScreen = disp; }
    VIDEO_DISPLAY_T isVideoListView(){ return  m_isListView ? m_videoListScreen : VIDEO_DISPLAY_MAX; };
    // } added by eunhye 2013.02.07
    // } modified by edo.lee 2013.05.29
	//void onShowLockoutView(VIDEO_DISPLAY_T show); //remove by edo.lee 2013.03.13

    VIDEO_DISPLAY_T getIsShowLockoutView();//{return m_isShowLockoutView;}	modified by edo.lee 2013.03.13
    void createPlaylistFromTA(MEDIA_SOURCE_T source);
    void setCopy(bool isCopy) { m_isCopy = isCopy; } // by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list
	// removed by Sergey 01.05.2013 for Tune Functionality
//{ added by yongkyun.lee 20130310 for : NO CR Play and Puase
    bool getPassPlay(){ return m_PassPlay;}
    void setPassPlay(bool val){  m_PassPlay = val;}
//} added by yongkyun.lee 20130310
	// removed by Sergey 10.05.2013 
    //{ added by eunhye 2013.03.19
    void setFullScreenState( bool fullscreen ) { is_FullScreen = fullscreen; }
    bool getFullScreenState() { return is_FullScreen; }
    //} added by eunhye 2013.03.19
	// removed by Sergey 01.05.2013 for Tune Functionality
    void setIsShowLockoutView(DISPLAY_T disp, bool lockout);//added by edo.lee 2013.03.26
    void setDefaultScreen(VIDEO_DISPLAY_T disp);//added by edo.lee 2013.03.28
    void onShowFullScreen();//added by edo.lee 2013.05.17
    void onShowSettings(QString type, QVariant mode); 			//added by aettie 20130612 for 172347
    void onShowPopup(POPUP_TXT_BTN_TYPE_T param); // added by Sergey 16.09.2013 for ITS#185236

    AppMediaPlayer_VideoCaptionProvider*  videoCaptionProvider(){ return m_imageProvider;} // modified by ravikanth 04-04-13
    bool getFgState() { return m_bFgState; } // added by wspark 2013.04.06 for ISV 78044
    void unblockUI(); // added by Sergey 2013.04.07 for ISV#143958
    void repaintUI(DISPLAY_T display = DISPLAY_NONE); // added by Sergey 18.10.2013 for safe ui update
    Q_INVOKABLE void repaintUIQML(QVariant disp); // added by Sergey 26.10.2013 for ITS#198719

    Q_PROPERTY(bool loadingOnCopy READ  loadingOnCopy NOTIFY jukeboxCopyInProgress); // modified by ravikanth 18-04-13
    void CancelCopyJukebox(); // modified by ravikanth 16-04-13
    bool isCopyInProgress() { return m_EngineMain.isCopyInProgress();} // modified by ravikanth 16-04-13
    bool loadingOnCopy(); // modified by ravikanth 18-04-13
    void setRescanProgressLoading(); // modified by ravikanth 20-04-13
    void ResetProgressLoading(); // modified by ravikanth 20-04-13
    void loadUI(APP_ENTRY_POINT_T ep, DISPLAY_T disp); // added by Sergey 28.04.2013
    // removed by Sergey 10.08.2013 for 182892
    void SetVideoSize(DISPLAY_T aDisplay); // modified by cychoi 2013.07.03 for ITS 177711 // added by cychoi 2013.05.16 for ISV 78583 & ITS 167437
    // removed by Sergey 28.05.2013
	bool rewReachedFirst() { return m_bRewReachedFirst; } // added by Sergey 28.05.2013

    bool showSubTitle(bool bShow); // modified by kihyung 2013.07.10 for ITS 0173538 

    // removed by Sergey 26.07.2013 for ITS#181096
    void SetSystemMute();//added by edo.lee 2013.06.23
    void setShowOSDFromBUfferedTrack(bool val); //added by Puneet 2013.07.03 ITS# 0177010
    bool getShowOSDFromBUfferedTrack();//added by Puneet 2013.07.03 ITS# 0177010

    bool getScreenDVDMenuTitle(VIDEO_DISPLAY_T disp = VIDEO_DISPLAY_MAX); // modified by cychoi 2013.08.06 for Position OSD is displayed on DVD Setting from Title/Disc Menu // modified by yongkyun.lee 2013-07-22 for : NO CR DVD title OSD
    bool isDVDTrackPlaying(); // added by cychoi 2013.09.04 for ITS 186733
    bool getScreenDVDModeChanged(){ return m_DVDModeChanged; };// modified by yongkyun.lee 2013-07-22 for : NO CR DVD title OSD
    void setScreenDVDModeChanged(bool val ){ m_DVDModeChanged = val; };// modified by yongkyun.lee 2013-07-22 for : NO CR DVD title OSD // modified by kihyung 2013.08.01 for static test
    bool isVideoSubScreen(QString screenName);//added by edo.lee 2013.07.30 subscreen
    DISPLAY_T actualDisp(DISPLAY_T disp); // added by Sergey for ITS#181872 30.07.2013
    void LaunchSubScreenOnVideo(SUBSCREEN_T screen, int display);
    void HideSubScreenOnVideo(int display);
    POPUP_TXT_BTN_TYPE_T popupType(DISPLAY_T disp);  // added by Sergey 01.09.2013 for ITS#186834
    bool isUnsupportedPopup(DISPLAY_T disp);	 // added by Sergey 01.09.2013 for ITS#186834
    bool getFFREWState(); //added by edo.lee 2013.09.05 ITS 0188286
    void handleGstPipelinesAvailable(DISPLAY_T display); // added by Sergey 07.09.2013 for PRE_FG supoort
    void showTxtBtnPopup(POPUP_TXT_BTN_TYPE_T typePopUp, VIDEO_DISPLAY_T disp); // added by Sergey 16.09.2013 for ITS#185236
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    void showDeckHighTempPopup();
    void closeDeckHighTempPopup();
    // } added by cychoi 2014.04.09
    bool isDiscPopup(POPUP_TXT_BTN_TYPE_T typePopUp); // added by cychoi 2014.03.24 for ITS 230874
    bool isTuneMode() { return m_pTuneTimer->isActive(); } //added by Sergey 27.09.2013 for ITS#189958
    bool isPlaybackScreen(QString screen); //added by Sergey 27.09.2013 for ITS#189958
    void popScreenFromBoth(QString screen); // added by Sergey 28.09.2013 for ITS#187711
    Q_INVOKABLE bool isSystemPopupVisible() { return m_systemPopVisible; } // modified by ravikanth 02-10-13 for ITS 0190988 // added by Sergey 15.01.2013 for ITS#219649
    void setLocalPopupShow( bool visible , int m_disp); // modified by yungi 2014.01.23 for ITS 221843// added by cychoi 2013.12.05 for ITS 213053
    Q_INVOKABLE void lightPlaybackControls(int item, bool isOn, QVariant disp); // added by Sergey 10.10.2013 for ITS#194643, 195505
    bool isMaintainPause() { return m_bMaintainPause; } // added by Sergey 11.10.2013 for ITS#194942
    void setMaintainPause(bool value) { m_bMaintainPause = value; } // added by Sergey 11.10.2013 for ITS#194942
    Q_INVOKABLE void stopFFRewTimer(); // added by Sergey13.10.13
    void notifyBothScreens(QString srcScreen, QString resScreen, EVENTS_T evt, QVariant arg = QVariant()); // added by Sergey 23.10.2013 for ITS#196877
    Q_INVOKABLE bool isFrontLoaded() { return m_bFrontLoaded; } // added by Sergey 24.10.2013 for ITS#197778
    bool isTopScreenDVDTitleDiscMenu(int disp); //added by sjhyun 2013.10.25 for ITS 198704
    // { modified by wspark 2014.02.19 for ITS 225827
    void initDirectAccessDisplayed (int disp, bool bShow); // added by cychoi 2014.02.14 for ITS 225321
    bool isDirectAccessDisplayed(int disp);
    // } modified by wspark
    Q_INVOKABLE bool seekHKPressed() { return m_bSeekHKPressed; } // added by Sergey 26.10.2013 to fix SEEK after ModeArea focus
    Q_INVOKABLE void setSeekHKPressed(bool state) { m_bSeekHKPressed = state; }//added by taihyun.ahn for ITS 221363 2014.01.21

    // { added by sjhyun 2013.11.01 for ITS 204921
    VCD_DIRECTACCESS_MODE_T getModeChangeOnDirectAccess(){ return m_eModeChange; }
    void setModeChangeOnDirectAccess(VCD_DIRECTACCESS_MODE_T newState){ m_eModeChange = newState; }
    // } added by sjhyun
    bool inhibitionIcon(bool val); // added by sjhyun 2013.11.05 for ITS 206357
    void closeCaptionScreen();//added by edo.lee 2013.1.8 ITS 218077
    void CancelAllJogKey(); //added by edo.lee 2014.1.8 ITS 0217055 

    Q_INVOKABLE void next();
    Q_INVOKABLE void prev();
    Q_INVOKABLE void fastForward();
    Q_INVOKABLE void fastForwardCritical();
    Q_INVOKABLE void rewind();
    Q_INVOKABLE void rewindCritical();
    Q_INVOKABLE void cancel_ff_rew();

    bool isDRSEntryPoint(APP_ENTRY_POINT_T ep); // added by Sergey 13.11.2013 for ITS#208891

    void setPopupType(POPUP_TXT_BTN_TYPE_T popupType){m_popupType = popupType;}//added by taihyun.ahn for ITS 221592 2014.01.23
    POPUP_TXT_BTN_TYPE_T getPopupType(){return m_popupType;}//added by taihyun.ahn for ITS 221592 2014.01.23
    bool isTempMode(){return m_bTempMode;} // added by cychoi 2014.02.14 for DVD SearchCaption local Temporal Mode
    void setTempMode(bool temp){m_bTempMode = temp;} // added by cychoi 2014.02.14 for DVD SearchCaption local Temporal Mode

    void setFSAnimation(bool bOn); // added by cychoi 2014.03.13 for ITS 229176

    void setVideoInterface(IVideoInterface *pIVideo) { m_pIVideo = pIVideo; } // added by cychoi 2014.06.27 for removed too many logs...

public slots:
	void onPopScreen();//added by edo.lee 2013.03.27	
	Q_INVOKABLE void setRewReachedFirst(bool); // added by Sergey 28.05.2013
	//added by aettie 20130620 for back key event
    Q_INVOKABLE void setIsBackPressByJog(bool state) {m_isBackPressByJog = state;}  
    Q_INVOKABLE void setIsBackRRC(bool bRRC) {m_bRRC = bRRC;} 
    void onStateChangedForGstPipe(int nState);
    void onAuxStateChangedForGstPipe(int nState);

signals:
    void signalJogNavigation( uint arrow, int status, bool bRRC, int nRepeat);//{ added by yongkyun.lee 20130225 for :  ISV 73823
#ifdef DUAL_KEY
    uint signalTransformEVT2QML_KEY(EVT_ID_T key);
#endif
    void deviceAttached( MEDIA_SOURCE_T, bool );
    void deviceRemoved( MEDIA_SOURCE_T );
    void avModeChanged( MEDIA_SOURCE_T, bool );
    void setInitDispValue(); //added by shkim for ITS 189354 Display Setting value Issue
    void lockoutMode( bool );
   // { added by yongkyun.lee  2012.10.17  for Current file is white color
    void tuneSameFileNoti( bool );   
    void tuneSameFile( bool );
   // } added by yongkyun.lee
    void setTuneMode(bool);
	// removed by Sergey 01.05.2013 for Tune Functionality
	void setFullScreenMode(bool bOn); // modified by Sergey 26.07.2013 for ITS#181444
    void setPassPlayState(bool bOn); // added by sjhyun 2013.09.22 for ITS 190471
    void showTitleMenu(int); // added by kihyung 2012.07.13
    void vcItemSelected(VCItem*); // added by ravikanth	- CR 11552, 11938

    // Send when Video player goes to FG/BG
    void fgBgStateChanged(bool isFG);
#ifdef BOOT_LOADING_SCREEN
    void isReadytoPlay();
#endif

    void NotifyUISH(Event aEvent);
    void NotifyUISH(Event aEvent, DISPLAY_T aDisplayType); // modified by kihyung 2013.1.26
    // removed by Sergey for CR16250
    void setDisplayOSD( bool vOSD );  // added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD      
    //void blackVideo(bool bHide); //added by Sergey for CR#15575
    void blackVideoOnRear(bool bHide);
	void blackVideoOnFront(bool bHide);

    void notifyFormatEnded(); // added by wspark 2012.12.17 for ISV64433
    void setCamMode(bool); // added by wspark 2013.02.21 for ISV 73305
    void jukeboxCopyInProgress(); // modified by ravikanth 18-04-13
    // removed by cychoi 2013.08.30 for ITS 187026 DVD Title/Disc Menu Loading Screen // added by lssanh 2013.05.18 ISV81619
    void rewReachedFirstChanged(bool); // added by Sergey 28.05.2013
    // removed by Sergey 08.08.2013 to fix double OptionsMenu
    void seekHandlejogevent(int arrow, int status); // modified by ravikanth 05-07-13 for ITS 0178426
    void copyCompleted(); // modified by ravikanth 12-07-13 for ITS 0174301
    void fgLoading(bool); // added by Sergey 15.09.2014 for ITS#189390
    void fsAnimation(bool bOn); // added by Sergey 26.09.2013 for ITS#191542
    void systemPopupShow(bool bShown, QVariant disp); // modified by yungi 2014.01.23 for ITS 221843 // modified by ravikanth 02-10-13 for ITS 0190988
    void localPopupShow(bool bShown, QVariant disp ); // modified by yungi 2014.01.23 for ITS 221843 // added by cychoi 2013.12.05 for ITS 213053
    void lightControls(int item, bool isOn, QVariant disp); // added by Sergey 10.10.2013 for ITS#194643, 195505
    void stopFFRewTimerSignal(); // added by Sergey13.10.13
    void showCaptions(QVariant disp, bool bOn); // added by Sergey 12.10.2013 for ITS#195158
    // removed by sjhyun 2013.11.07 for ITS 207179
    void showInhibitionIcon(bool bShow); // added by sjhyun 2013.11.05 for ITS 206357

    void setLoadingPBCMenu(bool isOn); //add by shkim 2013.11.13 for ITS 209040 Camera -> VCD PBC Menu Issue

private slots:
    void onDeviceAttached( MEDIA_SOURCE_T source, MEDIA_TYPE_T type, bool autoplay);
    void onDeviceRemoved( MEDIA_SOURCE_T source, MEDIA_TYPE_T type );
    void onDeviceChanged(int aSource); // added by Sergey for CR#15775
    void onHideUI();
    void onContextChanged(bool isFront);
    void onTuneTimerExpired();
	// removed by Sergey 01.05.2013 for Tune Functionality
    void onVideoTimerExpired(); // added by wspark 2013.01.16
    void onSignalVideoFgReceived(bool fgState); // modified by kihyung 2013.1.26
    void onSignalBgReceived(); // modified by kihyung 2013.1.26
    void onCamEnableReceived(bool); // added by wspark 2013.02.21 for ISV 73305
    void onSourceReleased(); // added by Sergey 14.08.2013 for ITS#184081
    void onFgLoadingTimerExpired(); // added by Sergey 15.09.2014 for ITS#189390
    void onCloseCopyCancelConfirmPopup(); // modified for ITS 0217509
	
private:
    void handleVRCommand(EVENT_EVT_VR_COMMAND_DATA_T *eventData, DISPLAY_T display);
    void handleAVModeChange(MODE_STATE_T mode, DISPLAY_T display);
    bool handleSettingsCommand( EVENT_SETTINGS_CHANGED_DATA_T *eventData );
    void prevSEEKButtonHandler(KEY_STATUS_T keyStatus, DISPLAY_T display);
    void nextSEEKButtonHandler(KEY_STATUS_T keyStatus, DISPLAY_T display);
    void tuneKnobHandler(Event& aEvent,DISPLAY_T display);
    bool processedInAUXMode(Event &aEvent);
    bool processedInDiscMode(); // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    void handleDefaultEntryPoint();
    void handleAspectRatioEvent(Event& aEvent); //added by Sergey for CR#16002
    // { modified by Sergey for CR#15775
    void handleVideoPlayEvent(Event& aEvent);
    // void updateDVDSettings(MEDIA_SOURCE_T aSource); // deleted by wspark 2013.02.28 for ISV 73328
    MODE_STATE_T mapToModeState(APP_ENTRY_POINT_T aEntryPoint);
	MEDIA_SOURCE_T getMediaSourceFromDRS(MODE_STATE_T mode);//added by edo.lee 2013.02.20
	bool shouldHandleJogTuneEvent(DISPLAY_T display); // added by Sergey 01.05.2013 for Tune Functionality
    
    // { added by Sergey 15.08.2013 for ITS#185556 
    /** @brief When call this function video shows focus and starts to catch jog events from UIListener.
     */
    void activateFocus(DISPLAY_T disp);

    /** @brief When call this function video hides focus and stops to catch jog events from UIListener.
     */
    void deactivateFocus(DISPLAY_T disp);
     // } added by Sergey 15.08.2013 for ITS#185556

    void registerScreens(); // modified by Sergey 03.09.2013 for ITS#186054

    // { added by Sergey 07.09.2013 for PRE_FG supoort
    MEDIA_SOURCE_T mapModeStateToSource(MODE_STATE_T mode);
    MEDIA_SOURCE_T mapEPToSource(APP_ENTRY_POINT_T ep);
    MODE_STATE_T mapSourceToMode(MEDIA_SOURCE_T source);
    QString mapSourceToPlaybackScreen(MEDIA_SOURCE_T source);
    void switchSource(MEDIA_SOURCE_T source);
    void switchScreenOnFG(MEDIA_SOURCE_T source, DISPLAY_T display);
    void preSourceSwitchProcess(MEDIA_SOURCE_T source, DISPLAY_T display);
    void lateScreenChange(MEDIA_SOURCE_T source, DISPLAY_T display);
    void turnOffFullScreenAnimation(DISPLAY_T display);
    void showEmptySourcePopup(MEDIA_SOURCE_T source, DISPLAY_T display);
    // } added by Sergey 07.09.2013 for PRE_FG supoort
    bool checkScreenToFullonDRS(DISPLAY_T display);//added by edo.lee 2013.09.09
    void sendSeekEventToFM( Event &aEvent ); // mofdified for ISV 89536
    Q_INVOKABLE void setSystemPopupShow( bool visible); // modified by ravikanth 02-10-13 for ITS 0190988 // added by Sergey 15.01.2013 for ITS#219649
    bool isPbcMode(MEDIA_SOURCE_T source, QString screen); // modified by sjhyun for ITS 195523 // added by sjhyun 2013.10.08 for ITS 193794
    // removed by sjhyun 2013.11.07 for ITS 207179
    bool isDVDScreenStatusKeep(MEDIA_SOURCE_T source, QString screen); // added by sjhyun 2013.10.29 for ITS 198332

    AppMediaPlayer_EngineMain            &m_EngineMain;
    AppMediaPlayer_Video_Controller      *m_pController; 
    AppMediaPlayer_Video_ControllerFS    *m_pJBCtrl;	// added by Sergey 13.09.2013 for ITS#188762
    AppMediaPlayer_Video_ControllerFS    *m_pUSB1Ctrl;	// added by Sergey 13.09.2013 for ITS#188762
    AppMediaPlayer_Video_ControllerFS    *m_pUSB2Ctrl;	// added by Sergey 13.09.2013 for ITS#188762
    AppMediaPlayer_Video_ScreenManager   *m_pScreenManager[2];// added by edo.lee 2013.02.04
    AppMediaPlayer_MainUtils             *m_pMainUtil;
    DISPLAY_T                             m_currentScreen;
    QTimer                               *m_pTuneTimer;
    QTimer                               *m_pVideoTimer; // added by wspark 2013.01.16
    QTimer                               *m_pFgLoadingTimer; // added by Sergey 15.09.2014 for ITS#189390
    QHash<DISPLAY_T, FM_FILE_T>   m_isFMFile; // modified by Sergey 09.09.2013 for ITS#188283
    // } modified by Sergey for CR#15775

	bool                                  m_PassPlay;//{ added by yongkyun.lee 20130310 for : NO CR Play and Puase

    MODE_STATE_T m_nLastAVModeEvent; // added by ravikanth for changes - CR 12929, 12935 (alternative for CR 12182)
   
    // added by ravikanth 2012-09-06
    // todo - fix it in generic way as it supports only settings external application
    QHash<DISPLAY_T, bool> m_bVideoInFG; // modified by Sergey 10.05.2013
    VIDEO_DISPLAY_T m_bIsLaunchingScreenSettings; //modified by edo.lee 2013.12.13 ITS 0211957
    bool m_bIsLaunchingSoundSettings; // added by cychoi 2013.08.05 for ScreenOptions is opened when returned from SoundSetting
    bool m_bTempMode; // added by cychoi 2014.02.14 for DVD SearchCaption local Temporal Mode
//{Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side
    bool m_TabClicked;//{ added by yongkyun.lee 2012.10.30 : CR 13733  : Sound Path
//    bool m_TabClicked;//{ added by yongkyun.lee 2012.09.25 : CR 13733  : Sound Path
//}Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side
    bool m_bScrollingTicker; // added by junggil 2012.10.05 for SANITY_CM_AJ497
    // bool m_bCamInFG; // modified by ravikanth 12-10-24
    PLAY_REQUESTER_T m_playRequester; // added by Sergey for CR#16015
        //QDeclarativeView* OSDview; //delete by ys 2013.09.30 //added by edo.lee 2012.12.08 for statusbar
	bool m_formatStatus; // added by wspark 2012.12.17 for ISV64433
	// removed by Sergey 07.09.2013 for PRE_FG supoort
    bool m_isFgReceived; // modified by kihyung 2013.1.26
    int  m_isLongPressed; // added by kihyung 2013.2.5 for ISV 72327
    bool m_isNotifyGstAvailable; // added by kihyung 2013.2.7 for ISV 72708
	//bool m_isLockoutMode;//remove by edo.lee 2013.02.25
	bool m_isListView; // added by eunhye 2013.02.07 for ISV70720
	VIDEO_DISPLAY_T m_videoListScreen; //added by edo.lee 2013.05.29
	//VIDEO_DISPLAY_T m_isShowLockoutView;//remove by edo.lee 2013.03.13
	//bool m_onBoot;//remove by edo.lee 2013.02.28
    bool m_bJukeboxDelayedPlay;
    bool m_bUSB1DelayedPlay;
    bool m_bUSB2DelayedPlay;
    bool m_isCopy; // added by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list 
    bool m_bJogCenterClickedInFront; // added by wspark 2013.03.01 for ISV 73547
    bool is_FullScreen;// added for eunhye 2013.03.19
    AppMediaPlayer_VideoCaptionProvider *m_imageProvider; // modified by ravikanth 04-04-13	
    bool m_bFgState; // added by wspark 2013.04.06 for ISV 78044
    bool m_trackerUpdateInProgress; // modified by ravikanth 20-04-13
    // removed by Sergey 10.08.2013 for 182892
	bool m_bRewReachedFirst; // added by Sergey 28.05.2013
	//added by aettie 20130620 for back key event
    bool m_isBackPressByJog;   
    bool m_bRRC;
	bool m_bPlayAfterBTCall;//added by edo.lee 2013.06.20
	int  m_nAuxPlaybackState;
    bool m_ShowOSDFromBUfferedTrack; //added by Puneet 2013.07.03 ITS# 0177010
    bool m_DVDModeChanged;// modified by yongkyun.lee 2013-07-22 for : NO CR DVD title OSD
    VPEnum::VDISP_T m_pbOwner;	// added by Sergey 28.08.2013 for ITS#186507 
	//bool m_bKeepFFREWOnBTCall;//remove by edo.lee 2013.09.25
    bool m_systemPopVisible; // modified by ravikanth 02-10-13 for ITS 0190988
    bool m_localPopVisible; // added by cychoi 2013.12.05 for ITS 213053
    VIDEO_DISPLAY_T m_systemPopupDisplay; // added by yungi 2014.01.23 for ITS 221843
    bool m_bMaintainPause; // added by Sergey 11.10.2013 for ITS#194942
    bool m_bFrontLoaded; // added by Sergey 24.10.2013 for ITS#197778
    bool m_bSeekHKPressed; // added by Sergey 26.10.2013 to fix SEEK after ModeArea focus
    VCD_DIRECTACCESS_MODE_T m_eModeChange; // added by sjhyun 2013.11.01 for ITS 204921
    bool m_bIsPlaybackControlQMLIsLoaded; // modified by sjhyun 2013.11.11 for ITS 206822
    bool m_bIsVideoPlaybackQMLIsLoaded; // added by sjhyun 2013.11.06 for ITS 206822
    //POPUP_TXT_BTN_TYPE_T m_popupTypeOnSubScreen; // added by cychoi 2014.03.19 for ITS 230153
    POPUP_TXT_BTN_TYPE_T m_popupType;//added by taihyun.ahn for ITS 221592 2014.01.23
    IVideoInterface *m_pIVideo; // added by cychoi 2014.06.27 for removed too many logs...
};

#endif // !defined(EA_53E3F1CF_656C_4dec_A8EC_11324F4C16E8__INCLUDED_)
