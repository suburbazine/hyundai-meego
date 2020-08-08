/**
 * AppMediaPlayer_EngineMain.h
 *
 * Implementation of the Class AppMediaPlayer_EngineMain
 *
 * @author  Golyshkin A.
 * @CreatedDate 29-Nov-2011 1:49:55 PM
 */

#if !defined(EA_45D0BFC8_508C_4146_B758_AC5E2C9790A2__INCLUDED_)
#define EA_45D0BFC8_508C_4146_B758_AC5E2C9790A2__INCLUDED_

// { added by richard.dale 2012.11.03 for CR 14076
#include <QTranslator> 
#include <MGConfItem>
// } added by richard.dale 2012.11.03 for CR 14076

#include <uistate_manager_def.h>
#include <DHAVN_AppFramework_AppVextEngineQML_Def.h>
#include "util/DHAVN_AppMediaPlayer_Settings.h"
#include "engine/DHAVN_AppMediaPlayer_EngineBase.h"
#include <DHAVN_QRPMClient.h>
#include "util/DHAVN_AppMediaPlayer_Notifier.h"
#include "util/DHAVN_AppMediaPlayer_EngModeNotifier.h"
#include "engine/DHAVN_AppMediaPlayer_EventFilter.h"   // added by sungha.choi 2013.08.07 for ITS 0181470
//#define STATUSBAR_NEW         1 // removed by edo.lee 2012.12.12 for statusbar
#define USE_AFW_SWAP_DC         1 // added by Sergey for SwapDC
#define USE_AFW_GST_AVAILABLE   1 // added by kihyung 2013.1.29
#define USE_AVP_EVENT_FILTER    0   // added by sungha.choi 2013.09.26

#ifdef AVP_BACKTRACE
#define AVP_BACKTRACE_LOG_FILE "/log_data/log/avp_backtrace.txt"
#endif

class AppMediaPlayer_EngineAudio;
class AppMediaPlayer_EngineVideo;
class AppMediaPlayer_EnginePhoto;
class AppMediaPlayer_Controllers;
class AppMediaPlayer_Bootstrap;

class QTranslator;
class QDeclarativeContext;
class APP_ENTRY_POINT_DATA_T;
class SharedDataProvider;

#ifdef BOOT_LOADING_SCREEN
typedef struct
{
    bool bisFS;     //Jukebox or USB1 or USB2 exit
    bool bisIpod;   //Ipod exit

    bool bisTAFinish;
    bool bisAVModeChange;
    bool bisReadyToPlay;
    bool bisAVPLastMode;


} BOOT_INFO_T;
#endif
class AppMediaPlayer_EngineMain : public AppVextEngineQML
{
   Q_OBJECT
   Q_PROPERTY(bool scrollingTicker READ getScrollingTicker NOTIFY scrollingTickerChanged)//Added by Alexey Edelev 2012.10.08
   Q_PROPERTY(bool isBTCall READ getisBTCall WRITE setisBTCall NOTIFY isBTCallChanged) // added by Dmitry 28.04.13
   Q_PROPERTY(bool middleEast READ middleEast WRITE setMiddleEast NOTIFY middleEastChanged) // added by Dmitry 05.05.13
   Q_PROPERTY(bool blockMouse READ blockMouse WRITE setBlockMouse NOTIFY blockMouseChanged) // added by Sergey 26.10.2013 for ITS#198513

#ifdef DUAL_KEY
protected:
   bool eventFilter( QObject *pFromObj, QEvent *pEvent );
#endif
public:
   AppMediaPlayer_EngineMain( LANGUAGE_T aLanguage, QTranslator &translator, int nVehicleType,
                              QObject *pParent = NULL );

   virtual ~AppMediaPlayer_EngineMain();

#ifdef WATCHDOG // Watch Dog
   static AppMediaPlayer_EngineMain *m_pInstance;
   static void signalHandelr(int signum);
   static void setSignalHandlerObject(AppMediaPlayer_EngineMain *m);
   void signalConnection();
   void restart(int signum);
#endif
   void onBTPlayUnmuteTimerStart();//{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control

   void HandleGstPipelinesAvailable(int nGpType); // added by kihyung 2013.1.29

   void LoadAppState();

   void SaveAppState();

   void TopEventHandler( Event &aEvent, DISPLAY_T display );

   void EventHandler( Event &aEvent, DISPLAY_T display = DISPLAY_FRONT ); // modified by Sergey for SwapDC

   virtual void HideUI() {}

   virtual void SaveSettings(); // modified by Sergey for SwapDC

   virtual void LoadSettings() {}

   AppMediaPlayer_EngineBase *GetEngine( APP_MEDIA_MODE_T mode );

   AppMediaPlayer_MediaManager * GetMediaManager();

   // { added by kihyung 2012.07.12 for 9874
   EngModeNotifier* GetEngModeNotifier() { return m_pEngModeNotifier; } 
   // } added by kihyung

   virtual APP_ID_T GetThisAppId() const { return APP_AVP; }

   APP_MEDIA_MODE_T GetFrontMode() const { return m_AppFrontMode; }
   APP_MEDIA_MODE_T GetRearMode() const { return m_AppRearMode; }
   APP_MEDIA_MODE_T GetFrontView() const { return m_AppFrontView; }
   APP_MEDIA_MODE_T GetRearView() const { return m_AppRearView; }
   APP_MEDIA_MODE_T GetLastPlayerMode() const { return m_LastPlayerMode;} // added by yungi 2012.11.14 for ISV 62667,ISV 62596

   //APP_MEDIA_MODE_T GetFrontViewR() const { return m_AppFrontViewR; }//remove by edo.lee 2013.02.27
   //APP_MEDIA_MODE_T GetRearViewR() const { return m_AppRearViewR; }//remove by edo.lee 2013.02.27

   bool IsVideoModeEnterd(DISPLAY_T display); // added by edo.lee 2013.02.04

//{Added by Alexey Edelev 2012.10.23 CR14638
   void SetFrontView(APP_MEDIA_MODE_T mode) { if ( m_AppFrontView != mode ) { m_AppFrontView = mode; /*m_AppFrontViewR = mode;*/ emit frontViewChanged(); } }
   void SetRearView(APP_MEDIA_MODE_T mode) { if ( m_AppRearView != mode ) { m_AppRearView = mode; /*m_AppRearViewR = mode;*/ emit rearViewChanged(); } }
//}Added by Alexey Edelev 2012.10.23 CR14638

   AppMediaPlayer_Controllers & GetControllers() { return *m_Controllers; } // Modified by Radhakrushna 20121012 NO_CR 

   Q_INVOKABLE void HandleSoftKeyHome();
   Q_INVOKABLE void SetRoot( QObject * );   
   Q_INVOKABLE bool getisBTCall(){return m_isBTCall;} //added by edo.lee 2013.04.03
   Q_INVOKABLE void SetModeAreaDim(bool on , int m_disp); // modified by yungi 2013.10.10 for ITS 194869 //added by yungi 2013.09.27 for ITS_NA 191163
   Q_INVOKABLE void invokeMethod(QObject *pObj,QString member)
                            { QMetaObject::invokeMethod(pObj, member.toLocal8Bit(), Qt::QueuedConnection); } //modified by edo.lee 2013.11.22 hideoption is changed to use invoke method
   Q_INVOKABLE void invokeMethod(QObject *pObj,QString member, int arg1, bool arg2)
                            { QMetaObject::invokeMethod(pObj, member.toLocal8Bit(), Qt::QueuedConnection, Q_ARG(int, arg1), Q_ARG(bool, arg2)); } //modified by edo.lee 2013.11.22 hideoption is changed to use invoke method
   Q_INVOKABLE void qmlCritical(QString source, QString message) { MP_HIGH << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " [MP_0][QML]" << source << "::" << message << LOG_ENDL; } // added by Sergey 28.112013 for ITS#211490
   Q_INVOKABLE void qmlLog( QString message) { MP_LOG << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " [MP_0][QML]" << "::" << message << LOG_ENDL; } // added by Sergey 28.112013 for ITS#211490

   void setisBTCall(bool call); // added by Dmitry 28.04.13
   void setisBLCall(bool call) { m_isBLCall = call; } // added by hyochang.ryu 20131017 for BL call
   bool getisBLCall() { return m_isBLCall; } // added by hyochang.ryu 20131017 for BL call

   void deckErrorHandler(int errorState);
   // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
   void sendDeckErrorToUISH_Impl(bool bPopupClose);
   void sendDeckErrorToUISH();
   bool IsDeckBlocked();
   void handleDeckErrorOnDeviceChanged();
   int getDeckLastError() { return m_nDeckLastError; } // added by cychoi 2014.04.25
   // } added by cychoi 2014.04.09

   void beep(EAudioBeepType beepType);

   // { modified by bluepark 2012.08.09
   // for booting time improvement
   // void loadComponents();
   void loadComponents(APP_MEDIA_MODE_T mode, DISPLAY_T display);// added by edo.lee 2013.02.04
   // } modified by bluepark

   ExternalNotifier *GetNotifier() { return m_pNotifier; }
   QTranslator& GetTranslator();

   // { removed by ravikanth for changes - CR 12929, 12935 (alternative for CR 12182)
   // { added by kihyung 2012.07.27 for CR 12182
   // MODE_STATE_T GetLastAVModeEvent();
   // void   SetLastAVModeEvent(MODE_STATE_T nAVMode);
   // } added by kihyung
   // } by ravikanth

//   void StartIPODControllers();//Added by Vishal for CR 13016 // removed by eugeny.novikov 2012.12.10 for CR 16564 fastboot
   bool getScrollingTicker() { return m_scrollingTicker; }//{Added by Alexey Edelev 2012.10.08

   // { added by changjin 2012.10.04 for SANITY_CM_AJ338
   APP_MEDIA_MODE_T GetActiveMode(void); 
   void SetAVOffMode(bool mode);
   bool GetAVOffMode(void);
   // } added by changjin 2012.10.04

   // { removed by junam 2012.11.28 for CR14466
   // { added by changjin 2012.10.15 for SANITY_CM_AG056
   //void SetDelayOSDUpdate(bool mode);
   //bool GetDelayOSDUpdate(void);
   // } added by changjin 2012.10.15
   // } removed by junam
   VIDEO_DISPLAY_T getVideoDisplay();//added by edo.lee 2013.02.20

   APP_MEDIA_MODE_T GetLastPlayMode(){return m_LastPlayerMode;} //added by edo.lee 2012.11.06

   void notifyUISH( Event aEvent, DISPLAY_T aDisplay = DISPLAY_FRONT ); // added by Sergey for SwapDC

   // { removed by eugeny.novikov 2012.11.10 for CR 15466
   // { added by changjin 2012.11.08 for Mode key handle the same mode change 
   // void SetEntryPoint(APP_ENTRY_POINT_T mode);
   // APP_ENTRY_POINT_T GetEntryPoint();
   // } added by changjin 2012.11.08
   // } removed by eugeny.novikov
   void sendVRMining( Event aEvent);// { added by  yongkyun.lee 2012.11.21 for  CR 14584 When say "Music search" in VR App, "There is no connection for media" comment is coming
   bool getIsFROnTune(){return m_isFROnTune; } // modified by lssanh 2013.03.24 ISV77034 //added by edo.lee 2013.01.03
   void setIsFROnTune(bool isFR){m_isFROnTune = isFR ;} // modified by lssanh 2013.03.24 ISV77034 //added by edo.lee 2013.01.03

   // { added by kihyung 2013.1.8
   void setCamInFG(bool bCamInFG);
   Q_INVOKABLE bool getCamInFG(); // modified by wspark 2014.02.05 for ITS 223741
   // } added by kihyung 2013.1.8

   Q_INVOKABLE bool getIsFromOtherAV(){return m_isFromOtherAV;} //added by edo.lee 2013.01.08
   void setIsFromOtherAV(bool isFromOtherAV){m_isFromOtherAV = isFromOtherAV;} //added by edo.lee 2013.01.08   
   void notifyEnableMedia( MEDIA_SOURCE_T deviceid, MODE_STATE_T modeState, bool isAudioEnable, bool isVideoEnable, bool isPhotoEnable); // Modified by edo.lee 2013.01.23
   bool getIsRestored(){return m_isRestored;} //added by edo.lee 2013.03.16
   void setIsRestored(bool isRestored){ m_isRestored = isRestored;} //added by edo.lee 2013.03.16
   bool getIsSwapEnabled(){return m_bSwapEnabled;} //added by edo.lee 2013.02.04
   APP_MEDIA_MODE_T GetViewR(DISPLAY_T display) ;//added by edo.lee 2013.02.27

   DISPLAY_T getCurrScreen() { return m_currentScreen; } // modified by kihyung 2013.1.26

   void setGstPipelineStatus(bool bStop); // added by kihyung 2013.1.29

   void connectImageEventToUISH(); //modified by edo.lee 2013.04.09
  	APP_ENTRY_POINT_T mapToEntryState(MODE_STATE_T pData);

   CTrackerAbstractor* GetTrackerAbs();
   bool isTAScanComplete(ETrackerAbstarctor_VolumeType volumeType);
   bool isTAReadComplete(ETrackerAbstarctor_VolumeType volumeType);
   bool isRequestedJukeboxRescan() { return m_bRequestedJukeboxRescan; } // added by kihyung 2013.11.16 to remove sync error after copy/delete.
   void requestFMReadTAScan(ETrackerAbstarctor_VolumeType volumeType);
   bool requestTrackerAbsRescan(ETrackerAbstarctor_VolumeType volumeType, bool bSendCapa = true);
   bool startTrackerAbsScan(ETrackerAbstarctor_VolumeType volumeType); // added by kihyung 2013.07.05 for ISV 84474
   void stopTrackerAbsScan(bool bJukebox, USB_PORT_T portNumber = eUSB_PORT_INVALID); // modified by kihyung 2013.07.25 for ITS 0180896 
   void completedTARead(ETrackerAbstarctor_VolumeType volumeType, int bSuccess);    
   bool getOnBoot(){return m_onBoot;}//added by edo.lee 2013.02.28
   bool getIsNoneState(){return m_isNoneState;} //added by edo.lee 2013.03.11
   bool getIsDiscEject(){return m_isDiscEject;}//added by edo.lee 2013.03.11
   void setIsDiscEject(bool isEject){m_isDiscEject=isEject;}//added by edo.lee 2013.03.11
   CLONE_STATUS_T getCloneState(){return m_eCloneStatus;} //added by edo.lee 2013.03.13
   bool getIsFrontBack(){return m_isFrontBack;} //added by edo.lee 2013.03.21
   int getnAcount(){return m_nAcount;} // added by eugene.seo 2013.04.15
   void setUnMuteState(bool isAudio); //added by edo.lee 2013.04.20   
   
   void setMuteForBTStream(MODE_STATE_T state);//{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
   Q_INVOKABLE void setMuteForSeek(bool isAudio); //added by edo.lee 2013.04.21 for Mute
   Q_INVOKABLE void setUnMuteForSeek(bool isAudio);//added by edo.lee 2013.04.21 for Mute
   void sendAppFGResponse(DISPLAY_T disp); // modified by Sergey 07.04.2013

   void IPodDeviceConnected(QString strNodePath, QString strSerial, EIPodPorts ePortNum, bool isFirst);
   void IPodDeviceDisconnected(QString strDeviceName, EIPodPorts ePortNum); 

   DISPLAY_T getSwapScreen(DISPLAY_T disp); // added by Sergey 16.04.2013
   DISPLAY_T getFrontScreenView(); // added by suilyou 20131101 ITS 0205229
	// { modified by ravikanth 16-04-13	
   bool isCopyInProgress();
   void setCopyInProgress(bool isCopying, int usbType); // modified by eugene.seo 2013.05.29
   void cancelAudioFileCopyOperation();
   // } modified by ravikanth 16-04-13
   
   DISPLAY_T getPlayInfoDisplay(){return m_playInfoDisplay;} //added by edo.lee 2013.04.20
   void setPlayInfoDisplay(DISPLAY_T display){ m_playInfoDisplay = display;} //added by edo.lee 2013.04.26
   bool getIsCloneTemp(){return m_isCloneTemp;}//added by edo.lee 2013.04.21 for Mute
   void setIsCloneTemp(bool isClone){m_isCloneTemp = isClone;}//added by edo.lee 2013.06.07

   void CheckMediaLastMode(); // added by kihyung 2013.07.05 for ISV 84474

   #ifdef BOOT_LOADING_SCREEN
   BOOT_INFO_T m_tBootInfo;  //ys-20131009 boot info
   QTimer      *m_qBootTimer; //
   void setAVPBootMode(QString param); //ys-20131010 boot isnotlast
   void setAVModeChange(bool bAVMode);
   #endif
   QTimer      *m_BTPlayUnmuteTimer;//{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
   void ConnectAVP2UISH();   
   bool IsConnectAVP2UISH();
   // { modified by kihyung 2013.07.20 for ITS 0180419
   void SetLastAVEntryPoint(APP_ENTRY_POINT_T ep); 
   MEDIA_SOURCE_T mapToSource(APP_ENTRY_POINT_T ep);
   APP_ENTRY_POINT_T GetLastAVEntryPoint(bool bFromSetting = true);
   void SetAVPEntryPoint(APP_ENTRY_POINT_T nEntryPoint, DISPLAY_T display);
   APP_ENTRY_POINT_T GetAVPEntryPoint(DISPLAY_T display);
   APP_MEDIA_MODE_T GetAVMediaMode(APP_ENTRY_POINT_T nEntryPnt, DISPLAY_T display = DISPLAY_FRONT);
   // } modified by kihyung 2013.07.20 for ITS 0180419

   bool GetMuteForSeek(){return m_isMuteForSeek;} //added by edo.lee 2013.04.29
   bool GetMuteState(){return m_isMuteState;}//added by edo.lee 2013.04.29

   bool getIsCompleteModeChange(){return m_isCompleteModeChange;}//added by edo.lee 2013.05.01
   void SendJukeboxEnableAtBoot();
   void SendIPODEnableAtBoot();
   void SendUSBEnableAtBoot();
   // added by Dmitry 05.05.13
   bool middleEast() { return m_bMiddleEast; }
   void setMiddleEast(bool east);
   // added by Dmitry 05.05.13
   void setReceivedTrackInfo(int isReceived){m_receivedTrackInfo += isReceived;} //added by edo.lee 2013.05.25
   int getReceivedTrackInfo(){return m_receivedTrackInfo;}//added by edo.lee 2013.05.25
   bool getIsFromPhoto(){return m_fromPhoto;}//added by edo.lee 2013.05.31

   Q_INVOKABLE bool isDisplayOff() { return m_bDisplayOff; } // added by kihyung 2013.07.04 for ISV 86619
   Q_INVOKABLE bool isPowerOff() { return m_bPowerOff; }
   Q_INVOKABLE bool isAVOff() { return m_bAVOff; } //modifieid by edo.lee 2013.06.02
   Q_INVOKABLE bool isFrontLCDOff() { return m_bFrontLCDOff; } // added by kihyung 2013.10.31 for ITS 0205241 
   Q_INVOKABLE bool isMediaOnFromOff() { return m_bMediaOnFromOff; }  // added by kihyung 2013.06.10 for ITS 0172955
   Q_INVOKABLE bool isDRSShow() { return m_bDRSShow; }
   Q_INVOKABLE bool isMediaOnFromCamOff() { return m_bMediaOnFromCamOff; }  // added by kihyung 2013.06.10 for ITS 0172955
   Q_INVOKABLE bool isACCOff() { return m_bACCOff; } // added by kihyung 2013.08.20 for ITS 0183782 
   void setReleaseGstFromVideo(bool bRelease) { m_bReleaseGstFromVideo = bRelease; }  // added by kihyung 2013.06.27
   bool getReleaseGstFromVideo() { return m_bReleaseGstFromVideo; }  // added by kihyung 2013.06.27

   bool IsShowLoadingFlag() { return m_bShowLoadingFlag; }
   void notifyCurrentTrackName(QString trackName); //added by junam 2013.06.04 for factory test

   void notifyDiscCurrentTrackName(Event aEvent); //added by wonseok.heo 2013.06.12 for factory test

   QString GetSWVersion();

   Q_INVOKABLE bool isTempMode(){return m_bTempMode;}//added by edo.lee 2013.06.22
   void setTempMode(bool temp){m_bTempMode = temp;}//added by edo.lee 2013.06.22
//added by edo.lee 2013.07.30 subscreen
   bool getSubCreenOn(){return m_subscreenOn;} 
   void setSubCreenOn(bool on, QString screenName );
   void setFullScreen(bool isFront);
   void setPlaybackScreen(bool isFront);//added by edo.lee 2013.07.30 fullscreen OSD
   void setVideoSubScreen(bool isFront, bool isOn); // added by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
   QString getSubCreenName();
   bool getLockOnDVDSetting(){return m_lockoutOnDVDSetting;}//added by edo.lee 2013.06.25
   void setLockOnDVDSetting(bool onLock){ m_lockoutOnDVDSetting = onLock;}//added by edo.lee 2013.06.25
   bool getUnLockOnDVDSetting(){return m_unLockoutOnDVDSetting;}//added by edo.lee 2013.06.25
   void setUnLockOnDVDSetting(bool onLock){ m_unLockoutOnDVDSetting = onLock;}//added by edo.lee 2013.06.25   
   void setIsChangeToDriving(APP_ENTRY_POINT_T entryPoint ,  DISPLAY_T display);//modified by edo.lee 2013.09.04 ITS 0188044
   void setIsChangeToParking(APP_ENTRY_POINT_T entryPoint,  DISPLAY_T display);//modified by edo.lee 2013.09.04 ITS 0188044
   // { modified by cychoi 2014.01.08 for ITS 218624 keep input box & grid focus on DRS ON <-> OFF
   Q_INVOKABLE bool IsChangeToDriving(){return m_isChangeToDriving;}//modified by edo.lee 2013.09.04 ITS 0188044
   Q_INVOKABLE bool IsChangeToParking(){return m_isChangeToParking;}//modified by edo.lee 2013.09.04 ITS 0188044
   // } modified by cychoi 2014.01.08

//added by edo.lee 2013.07.30 subscreen
   bool getFrontBlendOnOff() { return m_bIsFrontBlendOn;} //added by shkim for DVD->Cam Flickering Issue
   bool getRearBlendOnOff() { return m_bIsRearBlendOn;}    //added by shkim for DVD->Cam Flickering Issue
   void setPlayOnBTCall(){ m_playOnBTCall = false;} //added by edo.lee 2013.06.27 for BT CALL
   bool getPlayOnBTCall(){return m_playOnBTCall;}//added by edo.lee 2013.06.27 for BT CALL
   // modified by ravikanth 27-07-13 for ITS 181678
   void syncIndexFRPhotoEngine(bool fromFront, QString filepath, bool fromList, PHOTO_SOURCES_LIST source);//added by edo.lee 2013.07.03
	QString getCurrnetFilePath(bool fromFront);//added by edo.lee 2013.07.03   
	bool getIsSameDRSPhoto(bool fromFront , int sourceid);//added by edo.lee 2013.07.03
    //{ added by yongkyun.lee 20130702 for :  NO CR - None AV OSD
    bool GetNotifyAVModeState();
    MODE_STATE_T GetNotifyAVMode();
    //} added by yongkyun.lee 20130702 
   bool GetAuxDisconn() {	return m_isAuxDisconn; }	//added by hyochang.ryu 20130812 for ITS183306
   void InOutTestEject(); //added by wonseok.heo 2013.07.04 disc in out test
   bool getInOutTestMode(); //added by wonseok.heo 2013.07.04 disc in out test
   void setInOutTestMode(bool isInMode); //added by wonseok.heo 2013.07.04 disc in out test
   void sendDiscStatusChk(int data); //added by wonseok.heo 2013.07.04 disc in out test
   void InOutTestLoad(); //added by wonseok.heo 2013.07.04 disc in out test
   bool getReleasingClone(){return m_releasingClone;}//added by edo.lee 2013.07.07
   

   void setPowerOffBTCall(bool btmode){m_PowerOffBTCall = btmode; } //added by wonseok.heo for  ITS 178767 2013.07.10
   Q_INVOKABLE bool getPowerOffBTCall(){return m_PowerOffBTCall;} //modified by edo.lee 2013.08.01 for bt call pwr off
   void NotifyUISHMute( Event aEvent );  //modified by yongkyun.lee 2013-07-16 for : ITS 175107
   Q_INVOKABLE bool GetVariantRearUSB(); //added by junam 2013.07.18 for ITS_NA_180278
   bool getIsTermOnTrackChange(){return m_isTermOnTrackChange;}
   void setIsTermOnTrackChange(bool isPause){ m_isTermOnTrackChange = isPause;}
   bool getKeepSystemMute(){return m_isKeepSystemMute;}//added by edo.lee 2013.09.05 ITs 0188286
   void setKeepSystemMute(bool onMute){m_isKeepSystemMute = onMute;}//added by edo.lee 2013.09.05 ITS 0188286
   // modified by ravikanth 27-07-13 for ITS 181678
   void SetPhotoIndexOnOtherEngine(PHOTO_SOURCES_LIST source, int index, bool isFront);
   void setSlideShowDelay(int delay);
   bool getKeepPlayState(){return m_bKeepPlayState;} //added by edo.lee 2013.07.30 bt call play state
   void resetKeepPlayState(){m_bKeepPlayState = false;}//added by edo.lee 2013.07.30 bt call play state
   
// { modified by edo.lee 2013.08.16 for subscreen in AVP
   void setLaunchSubscreen(bool isLaunched){m_launchSubscreen = isLaunched;}
   bool getLaunchSubscreen(){return m_launchSubscreen;}
// } modified by edo.lee 2013.08.16 for subscreen in AVP
   bool checkIsSameEntryPoint(DISPLAY_T display,APP_ENTRY_POINT_T entry );//added by edo.lee 2013.08.21

// { Added by taihyun.ahn 2013.10.02  for ITS 188100
   bool getPlayAfterBTCallState() {return m_bPlayAfterBTCallState;}
   void setPlayAfterBTCallState(bool isFirst) {m_bPlayAfterBTCallState=isFirst;}

   bool getSelectTuned(){return m_selectTuned;};
   void setSelectTuned(bool isplayed){m_selectTuned = isplayed;}
// }

// { Added by taihyun.ahn 2013.10.07  for ITS 0180008
   bool getAccOffFromOn(){return m_accOffFromOn;};
   void setAccOffFromOn(bool accOn){m_accOffFromOn = accOn;}
// }

   void setBTCallPowerOn(bool btCallPower){m_btCallPowerOn = btCallPower;};//Added by taihyun.ahn 2013.10.22
   bool getBTCallPowerOn(){return m_btCallPowerOn;};//Added by taihyun.ahn 2013.10.22

   //added by suilyou 20130913 for blocking touch/jog event START
   Q_INVOKABLE bool getCloneState4QML(){return getCloneState()!=CLONE_OFF?true:false;}
   Q_INVOKABLE void setSharedMemData(int data);
   Q_INVOKABLE int getSharedMemData();
   //added by suilyou 20130913 for blocking touch/jog event END
   Q_INVOKABLE void unblockUI(); // added by Sergey 2013.09.26 for ITS#191449
   Q_INVOKABLE void repaintUI(DISPLAY_T display = DISPLAY_NONE); // modified by Sergey 01.11.2013 for safe ui update
   Q_INVOKABLE void sendTouchCleanUpForApps(); //added by suilyou 20130930 for ITS 186169
   Q_INVOKABLE void clearCCPJogKey();  // added by suilyou 20130930 ITS 0192624
   Q_INVOKABLE void clearRRCJogKey();  // added by suilyou 20130930 ITS 0192624
   MODE_STATE_T  mapToMode(APP_ENTRY_POINT_T ep);
   Q_INVOKABLE void NofityAudioPathChanged (int mode); // added by suilyou 20131011
   Q_INVOKABLE bool blockMouse() { return m_bBlockMouse; } // added by Sergey 26.10.2013 for ITS#198513
   Q_INVOKABLE void setBlockMouse(bool value) { m_bBlockMouse = value; emit blockMouseChanged(value); } // added by Sergey 26.10.2013 for ITS#198513

#ifdef DUAL_KEY
   void cancelAllPressedKey(int mode);
   EVT_ID_T getEVTfromCCPStatus(unsigned int key);
   EVT_ID_T getEVTfromRRCStatus(unsigned int key);
   EVT_ID_T getEVTfromETCStatus(unsigned int key);
   bool checkValidKey(EVT_ID_T key);

   // { added by sangmin.seol 2014.02.26 DUAL_KEY Flag moved from appfreamework
   Q_INVOKABLE unsigned int getCCPStatus();
   Q_INVOKABLE unsigned int getRRCStatus();
   Q_INVOKABLE unsigned int getETCStatus();
   Q_INVOKABLE void setCCPPressed(KEY_CCP_STATUS_T key);
   Q_INVOKABLE void setCCPReleased(KEY_CCP_STATUS_T key);
   Q_INVOKABLE void setCCPClear();
   Q_INVOKABLE void setRRCPressed(KEY_RRC_STATUS_T key);
   Q_INVOKABLE void setRRCReleased(KEY_RRC_STATUS_T key);
   Q_INVOKABLE void setRRCClear();
   Q_INVOKABLE void setETCPressed(KEY_ETC_STATUS_T key);
   Q_INVOKABLE void setETCReleased(KEY_ETC_STATUS_T key);
   Q_INVOKABLE void setETCClear();
       
   unsigned int nCCPStatus;
   unsigned int nRRCStatus;
   unsigned int nETCStatus;
   // } added by sangmin.seol 2014.02.26 DUAL_KEY Flag moved from appfreamework

   APP_MEDIA_MODE_T getFrontLCDMode()   { return frontLCDMode; }      // added by sangmin.seol 2014.04.07
   APP_MEDIA_MODE_T getRearLCDMode()    { return rearLCDMode; }       // added by sangmin.seol 2014.04.07

#endif
   Q_INVOKABLE void sendVRCancelSignal();
signals:

   void retranslateUi( QVariant aLanguageId );
   void showDeckErrPopup(QString text, QString btn1, QString btn2, int msec); // modified by Sergey for CR#16565
   void scrollingTickerChanged();//{Added by Alexey Edelev 2012.10.08
   void jukeboxCopyInProgressChanged(bool inProgress); // modified by ravikanth 18-04-13

//{Added by Alexey Edelev 2012.10.23 CR14638
   void frontViewChanged();
   void rearViewChanged();
//}Added by Alexey Edelev 2012.10.23 CR14638
   // { modified by wspark 2013.01.23 for ISV 70997, 70475
   //void signalFgReceived(); // added by wspark 2012.12.20 for ITS150787
   Q_SIGNAL void signalPhotoFgReceived(bool fgState);//ys -20130911 ITS 0186726
   Q_SIGNAL void signalAudioFgReceived(bool fgState);//ys -20130911 ITS 0186726

   void setModeAreaOnlyBgFront(bool bDimOn); // modified by yungi 2013.09.27 for ITS_NA 191163
   void setModeAreaOnlyBgRear(bool bDimOn); // added by yungi 2013.09.27 for ITS_NA 191163
   void signalVideoFgReceived(bool fgState);//ys -20130911 ITS 0186726
   void signalBgReceived(); // modified by kihyung 2013.1.26
   // } modified by wspark
   void camEnable(bool on); // added by wspark 2013.02.21 for ISV 73305
   void scrollAccStop(); // added by lssanh 2013.02.26 ISV73837
   void clearPlaybackJog();//added by edo.lee 2013.06.07 for reset Jog UI when exit AVP

   //void swapScreen(); //removed by junam 2013.09.30 for change raster
   Q_SIGNAL void isCurrentFrontViewChanged(bool isFront); //added by edo.lee 2013.03.07

   void notifyBtCallProgressing(bool value); // added by wspark 2013.04.10 for cluster update
   void sendTpMessage(); // added by wspark 2013.04.15 for EVT_UPDATE_CLUSTER
   void isBTCallChanged(); // added by Dmitry 28.04.13
   void middleEastChanged(); // added by Dmitry 05.05.13
   void tickerChanged(bool ticker); 	    //added by aettie for ticker stop when DRS on	

   void deckTestError(int errorValue); //added by wonseok.heo 2013.07.04 disc in out test
   void discStatusData(int statuschk); //added by wonseok.heo 2013.07.04 disc in out test
 //added by edo.lee 2013.07.30 subscreen
   void showSubScreen();
   void showVideoSubScreen(bool isFront); // added by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
   void showFullScreen(bool isFront);
   void hideSubScreen();//added by edo.lee 2013.07.30 subscreen OSD
   void hideVideoSubScreen(bool isFront); // added by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
   void hideFullScreen(bool isFront);//added by edo.lee 2013.07.30 fullscreen OSD
   //added by edo.lee 2013.07.30 subscreen
#if (USE_AVP_EVENT_FILTER == 1)
   void sendToEventFilter(EVENT_DISPLAY evt);   // added by sungha.choi 2013.08.07 for ITS 0181470
#endif
   //void loadComponent( int mode ,int display ); //2013.09.26 delete by youngsim.jo added by junam 2013.09.24 for fast agree
    void signalACCchanged(bool state); // added by suilyou 20131002 ITS 0192903
    void signalModeChange(); //added by suilyou ITS 0193848
    //void signalPowerNormalOff(); // modified for ITS 0210855 //removed by Michael.Kim for ITS 212424
    void signalPowerStateLogicOff();

#ifdef BOOT_LOADING_SCREEN
    void isReadytoPlay(); //ys-20131008 boot
#endif
    void overplaneChanged(); // added by Sergey 15.10.2013 for ITS#194932
    void blockMouseChanged(bool value); // added by Sergey 26.10.2013 for ITS#198513
#ifdef DUAL_KEY
   void UIStateManagerNotifyAppCommon( const Event aEvent, uint aScreenId );
#endif
    void formatting(bool started); // added by Dmitry 06.11.13 for ITS94041
    void signalAgreeOSD(); // added by suilyou 20131125 ITS 0210790
    void closeCopyCancelConfirmPopup(); // modified for ITS 0217509
    void clearInputText(int disp); // added by wspark 2014.02.10 for ITS 224249

public slots:

   virtual void NotifyFromUIStateManager( Event aEvent );
   virtual void NotifyFromUIStateManagerRear( Event aEvent );
   virtual void NotifyFromUIStateManagerCommon( Event aEvent ); // added by Sergey for SwapDC
#ifdef DUAL_KEY
   uint transformEVT2QML_KEY(EVT_ID_T key);
#endif
private slots:
   void componentLoaded( int mode ,int display );// added by edo.lee 2013.02.04
   void doNotifyUISHFront( Event aEvent );
   void doNotifyUISHRear( Event aEvent );
   void doNotifyUISH( Event aEvent, DISPLAY_T aDisplay ); // added by kihyung

   // { added by kihyung 2013.2.17
   void TAScanCompleted(ETrackerAbstarctor_VolumeType, int, int);
   void TAMediaFileFound(ETrackerAbstarctor_VolumeType, int, int, int,int);
   // } added by kihyung 2013.2.17

   void ConnectSignalToUISH(DISPLAY_T display, APP_MEDIA_MODE_T mode); //added by edo.lee 2013.03.19
   #ifdef BOOT_LOADING_SCREEN
   void ConnectAVP2UISHReadyToPlay(); //ys-20131008 boot
   void ConnectAVP2UISHTimeOut();
   #endif

   void onBTPlayUnmuteTimer();//{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
   // added by kihyung 2013.3.26
   void handleUSB1OutTimer();
   void handleUSB2OutTimer();
   // added by kihyung 2013.3.26

   void onClusterUpdateTimer(); // added by wspark 2013.04.10 for cluster update
   //remove by edo.lee 2013.11.08
   // removed by Sergey 07.09.2013 for PRE_FG supoort
   void onShowOSDReleaseClone(); //added by edo.lee 2013.07.07
   void onSendDeckErrorToUISH(); // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
	
#if (USE_AVP_EVENT_FILTER == 1)
   void onRecvEventFromEventFilter(EVENT_DISPLAY evt);   // added by sungha.choi 2013.08.07 for ITS 0181470
#endif
	
private:

   void NotifyFromUISHCommon( Event aEvent ); // modified by Sergey for SwapDC
   void NotifyFromUISHDisplay( Event aEvent, DISPLAY_T display ); // added by Sergey for SwapDC
   void SetActiveMode( APP_ENTRY_POINT_T pEventData, DISPLAY_T display, bool bShowLoading ); // modified by kihyung 2013.4.9 for ISV 78237
   void UpdateLayout( APP_MEDIA_MODE_T mode, DISPLAY_T display );
   void AUXInit(MODE_STATE_T state);	//added by hyochang.ryu 20130703

   void handleRestoreAVEvent( Event& aEvent ); // added by Sergey for SwapDC
   void resetMediaOnFromOff() { m_bMediaOnFromOff = false; }  // added by kihyung 2013.06.10 for ITS 0172955
   void resetMediaOnFromCamOff() { m_bMediaOnFromCamOff = false; } 
#ifdef DUAL_KEY
   void cancelPressedKey(Event aEvent);
   bool IsNeedTouchCleanUp(EVT_ID_T key);
#endif
#ifdef AVP_BACKTRACE
   void avpBackTrace(int num);
#endif
#ifdef AVP_LAST_AUDIOFILE_BACKUP
   void backupLastAudioFile();
#endif

private:

   /* Shows current active mode (MUSIC, VIDEO or PHOTO).
    * Its is used e.g. to track which mode should handle external events.
    * If application is in BG it is still valid.
    */
   APP_MEDIA_MODE_T m_AppFrontMode;
   APP_MEDIA_MODE_T m_AppRearMode;

   /* Applicable for Video and Music only.
    * It's like m_AppFrontMode but doesn't include PHOTO.
    * Used to handle events which valid for Music&Video only (e.g. SEEK).
    * If application is in BG it is still valid.
    * If PHOTO is on top it is still equaled to Video or Music.
    */
   APP_MEDIA_MODE_T m_LastPlayerMode;   //can be equaled to Video or Music only.

   /* Shows what view is currently shown (MUSIC, VIDEO or PHOTO).
    * Using it every mode can check if it is in FG or BG.
    * Also it is used to manage what view should be hidden in case of
    * application goes to BG or AVP mode is changed (e.g. from Video to Music).
    * If AVP application is in BG it is equaled to APP_MEDIA_MODE_MAX.
    */
   APP_MEDIA_MODE_T m_AppFrontView;
   APP_MEDIA_MODE_T m_AppRearView;

   // { added by kihyung 2013.2.4
   //APP_MEDIA_MODE_T m_AppFrontViewR;
   //APP_MEDIA_MODE_T m_AppRearViewR;
   // } added by kihyung 2013.2.4

   bool m_isVideoFrontEntered;// added by edo.lee 2013.02.04
   bool m_isVideoRearEntered;// added by edo.lee 2013.02.04

   bool m_isAudioFrontEntered;
   bool m_isAudioRearEntered;
   
   VIDEO_DISPLAY_T	m_currentVideoMode;//added by edo.lee 2013.02.20
   bool 	m_isNoneState;//added by edo.lee 2013.03.11
   bool		m_isDiscEject;//added by edo.lee 2013.03.11
   bool		m_isMuteState;//added by edo.lee 2013.04.01
   bool		m_isMuteForSeek;//added by edo.lee 2013.04.21  for Mute
   bool     m_isKeepSystemMute;//added by edo.lee 2013.09.05 ITS 0188286

   bool 	m_isCompleteModeChange;//added by edo.lee 2013.05.01
   
   QTranslator &m_Translator;

   AppMediaPlayer_Controllers *m_Controllers; // Modified by Radhakrushna 20121012 NO_CR 

   AppMediaPlayer_EngineBase *m_Engines[ APP_MEDIA_MODE_MAX ];

   AppMediaPlayer_MediaManager *m_pMediaManager;

   ExternalNotifier *m_pNotifier;

   EngModeNotifier *m_pEngModeNotifier;

   AppMediaPlayer_Bootstrap *m_pBootstrap;

   bool m_scrollingTicker;//{Added by Alexey Edelev 2012.10.08

   AppMediaPlayer_MainUtils *m_pMainUtil; // added by Sergey for SwapDC

   bool m_isFROnTune; // modified by lssanh 2013.03.24 ISV77034 //added by edo.lee 2013.01.03
   bool m_isFromOtherAV; //added by edo.lee 2013.01.08

   bool m_bCamInFG; // added by kihyung 2013.1.8
   bool m_bMediaOnFromCamOff;
   bool m_bACCOff; // added by kihyung 2013.08.20 for ITS 0183782 

   bool m_onBoot;//added by edo.lee 2013.02.28

   bool m_isRestored;//added by edo.lee 2013.03.16
   DISPLAY_T m_currentScreen; // added by kihyung 2013.1.12
   EVT_DEVICE_ENABLE_DATA_T m_pEnableData[MEDIA_SOURCE_MAX]; //aded by edo.lee 2013.01.23 // modified by kihyung 2013.05.31

   // removed by ravikanth for changes - CR 12929, 12935 (alternative for CR 12182)
   // MODE_STATE_T m_nLastAVModeEvent; // added by kihyung 2012.07.27 for CR 12182
   // { removed by eugeny.novikov 2012.11.10 for CR 15466
   // APP_ENTRY_POINT_T m_PrevEntryPoint; // added by changjin 2012.11.08 for Mode key handle the same mode change
   // } removed by eugeny.novikov

   // { added by kihyung 2013.1.24 for ISV 70304
   bool m_bSwapEnabled;

   bool m_bIsFrontBlendOn; //added by shkim for DVD->Cam Flickering Issue
   bool m_bIsRearBlendOn; //added by shkim for DVD->Cam Flickering Issue
   CLONE_STATUS_T m_eCloneStatus;
   // } added by kihyung 2013.1.24   

   bool m_isBgReceived; // added by kihyung 2013.2.7 for ISV 72708

   bool m_bFrontAVPIsForeground; //added by suryanto.tan 2013.3.1 for SANITY_CM_AI001

   bool m_isFrontBack; //added by edo.lee 2013.03.21
   
   bool m_isBTCall; //added by edo.lee 2013.04.03 
   bool m_isBLCall; //added by hyochang.ryu 20131017 for BL call 

   bool m_isCloneTemp;//added by edo.lee 2013.04.21  for Mute
   // { added by kihyung 2013.2.17
   bool m_bUSB1TAScanComplete;
   bool m_bUSB2TAScanComplete;   
   bool m_bJukeboxTAScanComplete;   
   bool m_bUSB1TAReadComplete;
   bool m_bUSB2TAReadComplete;   
   bool m_bJukeboxTAReadComplete;   
   bool m_bRequestedJukeboxRescan;
   CTrackerAbstractor *m_pTrackerAbs;
   // { added by kihyung 2013.2.17   

   // added by kihyung 2013.3.26
   //QTimer m_tUSB1OutTimer; //20131014 isnotused
   //QTimer m_tUSB2OutTimer; //20131014 isnotused
   // added by kihyung 2013.3.26

   // { added by Sergey 17.04.2013
   /* //youngsim.jo 20131014 is not used
   bool m_bFrontFGResponsed;
   bool m_bRearFGResponsed;
   QTimer m_FrontRespFGTimer;
   QTimer m_RearRespFGTimer;
   */
   // } added by Sergey 17.04.2013

   QTimer m_ClusterUpdateTimer; // added by wspark 2013.04.10 for cluster update
   int m_nAcount; // added by eugene.seo 2013.04.15
    DISPLAY_T m_playInfoDisplay; //added by edo.lee 2013.04.20
   // This is moved to main header as it is set by music and used by video and photo
   bool m_jukeboxCopyInProgress; // modified by ravikanth 16-04-13
   int m_usbType; // added by eugene.seo 2013.05.29



   bool m_bConnectAVP2UISH;

   bool m_pIsSendEnableMedia2UISH[MEDIA_SOURCE_MAX];
   bool m_pIsSendIPODMounted2UISH[2];
   int  m_pIsSendIPODRemoved2UISH[2];
   // removed by Sergey 28.04.2013
   bool m_bMiddleEast; // added by Dmitry 05.05.13   
   int m_receivedTrackInfo; //added by edo.lee 2013.05.09
   bool m_fromPhoto; //added by edo.lee 2013.05.31
   
   APP_ENTRY_POINT_T m_nLastAVEntryPoint;
   APP_ENTRY_POINT_T m_nFrontAVPEntryPoint; // added by kihyung 2013.07.20 for ITS 0180419
   APP_ENTRY_POINT_T m_nRearAVPEntryPoint; // added by kihyung 2013.07.20 for ITS 0180419
   
   bool m_bDisplayOff; // added by kihyung 2013.07.04 for ISV 86619
   bool m_bPowerOff;
   bool m_bAVOff;
   bool m_bFrontLCDOff; // added by kihyung 2013.10.31 for ITS 0205241
   bool m_bMediaOnFromOff;
   bool m_bDRSShow;
   bool m_bShowLoadingFlag;
   bool m_bTempMode;//added by edo.lee 2013.06.22 for keeping last UI
   bool m_bReleaseGstFromVideo;  // added by kihyung 2013.06.27
   bool m_subscreenOn;//added by edo.lee 2013.07.30 subscreen
   bool m_lockoutOnDVDSetting;//added by edo.lee 2013.06.25
   bool m_unLockoutOnDVDSetting;//added by edo.lee 2013.06.25
   bool m_playOnBTCall;//added by edo.lee 2013.0627
   bool m_FRDRSViewLoading;//added by edo.lee 2013.07.03
   MODE_STATE_T m_isNotifyAVMode; // added by yongkyun.lee 20130702 for :  NO CR - None AV OSD

   QString m_strSWVersion;

   bool m_DiscInOutTestMode; //added by wonseok.heo 2013.07.04 disc in out test
   bool m_releasingClone;//added by edo.lee 2013.07.07
   QTimer m_releaseCloneTimer;//added by edo.lee 2013.07.07
   // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
   QTimer m_tDeckErrorTimer;
   int    m_nDeckLastError;
   // } added by cychoi 2014.04.09
 
  //remove by edo.lee 2013.11.08 

   bool m_PowerOffBTCall; //added by wonseok.heo for  ITS 178767 2013.07.10

   bool m_isTAOn; //added by sh.kim for ITS 179104
   bool m_isAuxDisconn;	//added by hyochang.ryu 20130719
   
   bool m_isTermOnTrackChange;
   QString m_subScreenName;//added by edo.lee 2013.07.30 subscreen
   bool m_bKeepPlayState; //added by edo.lee 2013.07.30 stay previous play state
   bool m_launchSubscreen; //  modified by edo.lee 2013.08.16 for subscreen in AVP
   APP_ENTRY_POINT_T m_LastFrontEntryPoint;//added by edo.lee 2013.08.21
   APP_ENTRY_POINT_T m_LastRearEntryPoint;//added by edo.lee 2013.08.21

   bool m_isChangeToDriving; //added by edo.lee 2013.09.04 ITS 0188044
   bool m_isChangeToParking; //added by edo.lee 2013.09.04 ITS 0188044

// { Added by taihyun.ahn 2013.10.02  for ITS 188100
   bool m_bPlayAfterBTCallState;
   bool m_selectTuned;
// }

   bool m_accOffFromOn; //Added by taihyun.ahn 2013.10.07  for ITS 0180008
   bool m_bFgEventStart ;//added by edo.lee 2013.10.16 ITS 0196016 

   bool m_btCallPowerOn; //added by taihyun.ahn 2013.10.22 for ITS 0197257
   KEY_PRESSED_T m_KeyPressed;//added by edo.lee 2013.10.25
#ifndef DUAL_KEY
   bool m_isHUPrePressed;
   bool m_isHUNextPressed;
#endif
#if (USE_AVP_EVENT_FILTER == 1)
   EventFilter m_EventFilter;   // added by sungha.choi 2013.08.07 for ITS 0181470, // deleted by sungha.choi 2013.09.25
#endif

   SharedDataProvider *m_pSharedFMData;
   bool m_bBlockMouse; // added by Sergey 26.10.2013 for ITS#198513
   bool m_bVRStatus;
#ifdef DUAL_KEY
   APP_MEDIA_MODE_T frontLCDMode;
   APP_MEDIA_MODE_T rearLCDMode;
#endif
    //{ added by jaehwan 2014.04.03 to fix the issue that ipod icon is not activated after reboot.
   bool m_pendingIpodEnabled;
   QString m_iPodStrNodePath;
   QString m_iPodStrSerial;
   EIPodPorts m_iPodEPortNum;
   bool m_iPodIsFirst;
   //} added by jaehwan.

};

// { added by richard.dale 2012.11.03 for CR 14076
const QString TransRes_EN = "/app/share/AppMusicPlayer/localization/MusicPlayer_en";
const QString TransRes_KO = "/app/share/AppMusicPlayer/localization/MusicPlayer_ko";
const QString TransRes_ZH_CMN = "/app/share/AppMusicPlayer/localization/MusicPlayer_zh-cmn";
const QString TransRes_ZH_YUE = "/app/share/AppMusicPlayer/localization/MusicPlayer_zh-yue";
const QString TransRes_PT = "/app/share/AppMusicPlayer/localization/MusicPlayer_pt";
const QString TransRes_EN_GB = "/app/share/AppMusicPlayer/localization/MusicPlayer_en-gb";
const QString TransRes_FR = "/app/share/AppMusicPlayer/localization/MusicPlayer_fr-eu";
const QString TransRes_IT = "/app/share/AppMusicPlayer/localization/MusicPlayer_it";
const QString TransRes_DE = "/app/share/AppMusicPlayer/localization/MusicPlayer_ge";
const QString TransRes_ES = "/app/share/AppMusicPlayer/localization/MusicPlayer_es-eu";
const QString TransRes_RU = "/app/share/AppMusicPlayer/localization/MusicPlayer_ru";
const QString TransRes_NL = "/app/share/AppMusicPlayer/localization/MusicPlayer_nl";
const QString TransRes_SV = "/app/share/AppMusicPlayer/localization/MusicPlayer_sv";
const QString TransRes_PL = "/app/share/AppMusicPlayer/localization/MusicPlayer_pl";
const QString TransRes_TR = "/app/share/AppMusicPlayer/localization/MusicPlayer_tr";
const QString TransRes_CS = "/app/share/AppMusicPlayer/localization/MusicPlayer_cs";
const QString TransRes_DA = "/app/share/AppMusicPlayer/localization/MusicPlayer_da";
const QString TransRes_SK = "/app/share/AppMusicPlayer/localization/MusicPlayer_sk";
const QString TransRes_AR = "/app/share/AppMusicPlayer/localization/MusicPlayer_ar";
const QString TransRes_ES_NA = "/app/share/AppMusicPlayer/localization/MusicPlayer_es-na";
const QString TransRes_FR_NA = "/app/share/AppMusicPlayer/localization/MusicPlayer_fr-na";
// } added by richard.dale 2012.11.03 for CR 14076

// { modified by richard.dale 2012.11.03 for CR 14076
class LocalizeTrigger : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString empty READ empty NOTIFY retrigger);

public:
   // QString empty() { return QString(); }
   static LocalizeTrigger *self();
   void SetTrackerLocale();
   void SetLanguage( LANGUAGE_T nLanguage );
   LANGUAGE_T GetLanguage(){ return m_nLanguage; }
   QString Translate( QString sStrId );

   LocalizeTrigger();
   ~LocalizeTrigger(){}

signals:
    void retrigger();

private:
   QString empty(){ return QString(); }
   void InitTranslator();
   QString GetTransRes();
   QString GetLocaleName();

private:
   static LocalizeTrigger *s_self;
   QTranslator *m_pTranslator;
   LANGUAGE_T m_nLanguage;
   MGConfItem *m_languageItem;
   MGConfItem *m_timeItem;
   MGConfItem *m_collateItem;
   MGConfItem *m_numericItem;
   MGConfItem *m_monetaryItem;
};
// } modified by richard.dale 2012.11.03 for CR 14076

#endif // !defined(EA_45D0BFC8_508C_4146_B758_AC5E2C9790A2__INCLUDED_)
