#ifndef FMGR_ENGINE_H
#define FMGR_ENGINE_H

#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include <QApplication>
#include <QTranslator>
#include <DHAVN_AppFileManager_CommonDef.h>
#include <uistate_manager_def.h>
#include <DHAVN_AppFramework_AppVextEngineQML_Def.h>
#include "DHAVN_AppFileManager_CStateManager.h"
#include "DHAVN_AppFileManager_Logger.h"
#include "DHAVN_AppMediaPlayer_Shared.h" // modified by eugene 2012.08.27 for static analysis defect #6893
#include <DHAVN_TrackerAbstractor.h>
#include <DHAVN_AppFileManager_CTrackerExtractorClient.h> // added by Dmitry 14.08.13 for ITS0183845
#include <DHAVN_SharedDataProvider.h> // added by suilyou 20130925 for key block
#define USE_AFW_SWAP_DC     1

class UI_Main; // added by Sergey for CR#15468

const QString KLanguageId = QString("-l:");

// { added by eugene.seo 2013.03.09
struct ImageInfo
{
    // General
    QString   q_GeneralFormat;

    // Image
    int     i_ImageStreamCount;
    QString q_ImageFormat;
    int     i_ImageWidth;
    int     i_ImageHeight;
};
// } added by eugene.seo 2013.03.09

// { added by Sergey 15.09.2013 for ITS#185233
struct DRM_FILE_T
{
    bool isDRM;
    int type;
    int limitCount;
    int usedCount;
    QString path;
};
// } added by Sergey 15.09.2013 for ITS#185233

class FileManagerEngine: public AppVextEngineQML
{
   Q_OBJECT
   // USE_FM_LOG_UTIL

   Q_PROPERTY(bool scrollingTicker READ getScrollingTicker NOTIFY scrollingTickerChanged)//Added by Alexey Edelev 2012.10.06 fix text scrolling
   Q_PROPERTY(bool middleEast READ middleEast WRITE setMiddleEast NOTIFY middleEastChanged) // added by Dmitry 03.05.13
   Q_PROPERTY(bool videoPlaying READ videoPlaying WRITE setVideoPlaying NOTIFY videoPlayingChanged) // added by Dmitry 12.07.13 for ISV85006

public:

   FileManagerEngine( int &argc, char **argv, LANGUAGE_T nLanguage,
                     ECountryVariant cv, QObject *pParent = NULL );

   ~FileManagerEngine();

   // Watch Dog
   static FileManagerEngine *m_pInstance;
   static void signalHandelr(int signum);
   static void setSignalHandlerObject(FileManagerEngine *m);
   void signalConnection();
   void restart(int signum);

   Q_INVOKABLE void HandleBackKey(bool isFront);
   Q_INVOKABLE void HandleHomeKey(bool isFront);

   Q_INVOKABLE void OnUSBTest();

   Q_INVOKABLE void PlayMusicFile(const QString& filePath, bool isFront);
   Q_INVOKABLE void PlayVideoFile(const QString& filePath, bool isFront);
   Q_INVOKABLE void ShowPictureFile( const QString & filePath, bool isFront , bool isSupported);
   Q_INVOKABLE void LaunchSettingsPhotoFrame(QString path, bool isFront);
   Q_INVOKABLE void FileSelected(const QString &filePath, int fileType, bool isFront, bool isSupported = false);
   Q_INVOKABLE void LaunchMusicList(bool isFront);
   Q_INVOKABLE void QmlKeyHandler( int nKeyCode, int nStatus, bool isFront );
   Q_INVOKABLE void LaunchSettingsKeypad(bool isFront);
   Q_INVOKABLE int getCloneMode() { return m_eCloneStatus; } // added by Dmitry 23.04.13
// modified by Dmitry 17.08.13
   Q_INVOKABLE void notifyFormatJukeboxBegin();
   Q_INVOKABLE void notifyFormatJukeboxEnd();
   Q_INVOKABLE bool isFrontDisplayFG(){ return m_IsFrontVisible; }
   Q_INVOKABLE bool isRearDisplayFG(){ return m_IsRearVisible; }
   Q_INVOKABLE void MBeep();

   // } added by lssanh 2012.09.12 for CR13482
   Q_PROPERTY ( bool usbAttached READ getUsbAttached NOTIFY updateUsbAttached )
   Q_PROPERTY (int variantCountry READ variantCountry NOTIFY variantCountryChanged)

   void RetranslateUi();

   bool getUsbAttached() { return m_isUsbAttached; }
   //added by edo.lee 2013.05.27
   //APP_ID_T getAppId() { return appId; }
   APP_ID_T getFrontAppId() { return appId_front; }
   APP_ID_T getRearAppId() { return appId_rear; }
   //added by edo.lee 2013.05.27
   
   void notifyDeleteVideoPlayer(bool isJukeBox, const QString& path, bool isFront);
   void setPhotoFrame(const QString path, bool isFront);

   bool isBgState(bool isFront);
   void launchVideoPlayer(bool isFront);
   void launchPhotoGallery(bool isFront);
   void renewFocus(bool isFront);
   bool sendEventToUISH(Event& aEvent, bool isFront);
   bool isCommonTypeEvent(EVT_ID_T aEventID);
   Q_INVOKABLE void checkFocusVisibility(bool isFront, bool visible);
   void notifyFilesChanged();
   void setCurrentState(EAppFileManagerState value) { m_currentStateTraking = value; }
  static QString GetValidFilePath( QString originalFileName );
   bool checkInUsbState();
   //modified by edo.lee 2013.06.11
   void setJukeboxDevice(bool value, bool isFront); 
// modified by Dmitry 17.08.13
   //modified by edo.lee 2013.06.11
   
   void setMoveStatus(bool status);// add by eunhye 2012.09.21 for CR 9243

   // { added by junggil 2012.10.04 for CR13505 Media Apps - delta UIS 2.15 to 2.17 
   void handleSettingsCommand( EVENT_SETTINGS_CHANGED_DATA_T *eventData );
   bool getScrollingTicker() const {return m_bScrollingTicker;}//Changed by Alexey Edelev 2012.10.06 fix text scrolling
   // } added by junggil
   DISPLAY_T getDisplayVisible(); // added by lssanh 2013.02.28 ISV72537 USB remove

   // { added by Sergey 15.09.2013 for ITS#185233
   Q_INVOKABLE void handlePlayVideo(QString filePath, bool isFront);
   Q_INVOKABLE void drmConfirm(bool isFront);
   Q_INVOKABLE void drmReset();
   Q_INVOKABLE int drmLimitCount() { return m_drmFile.limitCount; }
   Q_INVOKABLE int drmUsedCount() { return m_drmFile.usedCount; }
// } added by Sergey 15.09.2013 for ITS#185233

   Q_INVOKABLE void cancelJogDirectionKey();

   void setFilePath(const QString& filepath, bool isFront); // modified by Dmitry 22.05.13
   Q_INVOKABLE void  isPopupID(bool popupType){ m_popupType = popupType; }; // add by eunhye 2013.02.06 for ISV70222/70319, ITS150138/149862
   
   bool isTAReadComplete(ETrackerAbstarctor_VolumeType volumeType);
   AppEngineQMLConstants::EAPP_SCREEN_GEOMETRY_T GetViewR(DISPLAY_T display); //added by edo.lee 2013.03.19
   CTrackerAbstractor* getTrackerInstance(); // added by Dmitry Bykov 04.04.2013
   QString GetSWVersion();
   // { modified by ravikanth 16-04-13
   void notifyCopyToJukeboxBegin(bool isFront, bool isVideo); // modified by eugene.seo 2013.05.29
   void notifyCopyToJukeboxEnd(bool isVideo); // modified by ravikanth 12-07-13 for ITS 0174301
   void CancelCopyJukebox(); 
   bool GetIsfileCopyOperationActive(){ return m_bfileCopyOperationStatus; }
   void setFileCopyOperationStatus(bool isInProgress); // modified by kihyung 2013.07.15 for STATIC 20893
   void cancelFileCopyOperation();
   // } modified by ravikanth 16-04-13
// added by Dmitry 03.05.13
   bool middleEast() { return m_bMiddleEast; }
   void setMiddleEast(bool east); // modified by Dmitry 11.05.13
// added by Dmitry 03.05.13
	void setOSDDisplay(bool isFront);//added by edo.lee 2013.05.17	
	void SetTranslator(QTranslator* translator){m_pTranslator = translator;}
   void cancelCopyOnInstance(bool screenInstance); // modified by ravikanth 29-06-13 for ITS 0176909

// added by Dmitry 12.07.13 for ISV85006
   bool videoPlaying() { return m_bVideoPlaying; }
   void setVideoPlaying(bool playing) { m_bVideoPlaying = playing; emit videoPlayingChanged(); }
// added by Dmitry 12.07.13 for ISV85006
	//added by edo.lee 2013.07.14
	bool m_isFrontVideoType;
	bool m_isRearVideoType;
	//added by edo.lee 2013.07.14
   CTrackerExtractorClient* GetTrackerExtractorClient() { return m_pExtractor; } // added by Dmitry 14.08.13 for ITS0183845
   void connectReadComplete(bool bConnect); // added by Dmitry 21.08.13
// removed by Dmitry 03.10.13 for ITS0187937
    Q_INVOKABLE void sendTouchCleanUpForApps(); //added by suilyou 20130930 for ITS 186169
   void storeSM(CStateManager *front, CStateManager *rear) { m_pSMFront = front; m_pSMRear = rear; } // added by Dmitry 09.10.13
   Q_INVOKABLE bool getCloneState4QML(){return m_eCloneStatus != CLONE_OFF?true:false;} //added by suilyou ITS 0194641
//   Q_INVOKABLE void clearCCPJogKey();
//   Q_INVOKABLE void clearRRCJogKey();
   Q_INVOKABLE void clearALLJogKey();

    // added by kihyung 2013.12.09 for ITS 0213549 
    QString GetFrontUSBPath();
    QString GetRearUSBPath();
    void EventHandlerPostEvent(Event & aEvent, bool isFront); // added by Michael.Kim 2014.06.27 for Copy List Issue in smoke test
    Q_INVOKABLE void qmlCritical(QString source, QString message) { MP_HIGH << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " [MP_0][QML]" << source << "::" << message << LOG_ENDL; } // added by Sergey 28.112013 for ITS#211490
    Q_INVOKABLE void qmlLog( QString message) { MP_LOG << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " [MP_0][QML]" << "::" << message << LOG_ENDL; } // added by Sergey 28.112013 for ITS#211490

signals:

   // { modified by kihyung 2012.08.02 for CR 12191
   /*
   void signalUSBPluggedIn( const QString& path );
   void signalUSBPluggedOut();
   */
   void signalUSBPluggedIn(const QString& path, const int nPort);
   void signalUSBPluggedOut(const int nPort);
   // } modified by kihyung
  
   void fileSelectedReset(); // added by Michael.Kim 2014.11.14 for ITS 252495
   void signalCDInserted();
   void startView();
   void postEventToStart(int state, QString currentPath, QString fullFileName,
                         int screen);
   void postEventPlayFileChanged(QString fullFileName, int type,
                                 int screen);
   void jogEvent( int event );
   void retranslateUi(int langId);
   void updateUsbAttached();
   void keyEvent( int nEventKey, int nStatus, int screen );
   void variantCountryChanged(int value);
   void ClearSharedData(); // added by suilyou 20131016
   Q_INVOKABLE void defaultFocus();
   void hideOnBg();
   // modified by ravikanth 10-07-13 for ITS 0179181
   void hideOnFg(int display); // added by wspark 2013.02.03
   void signalSetDisplay( int disp );
   void focusReceived( bool aValue );
   void scrollingTickerChanged();//Added by Alexey Edelev 2012.10.06 fix text scrolling
   void showProgressingPopup(); // added by junggil 2012.10.09 for CR14142 & CR14228 & CR14275 
   void showPopup( int type, bool isFront); // added by Sergey for CR#15468
   void signalJogNavigation( uint arrow, uint status ); // add by eunhye 2013.02.06 for ISV70222/70319, ITS150138/149862 
   // { added by lssanh 2013.02.07 2USB
   void cancelFileOperation();
   // } added by lssanh 2013.02.07 2USB
   void playingDirectoryChanged(QString playingPath); // added by wspark 2013.02.08 for ISV 70322
   void showCopy(bool m_iscopy, bool unsupportedFile); // addd by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list
   void loadMainQml(int screen); // modified by Sergey 25.04.2013
   void showBottomBarOnCopy(); // modified by ravikanth 06-04-13
   void middleEastChanged(); // added by Dmitry 03.05.13
   void tickerChanged(bool ticker); //added by aettie for ticker stop when DRS on
   void hideMenu(int screen); // modified by Dmitry 20.07.13
   void cancelFileOperationOnInstance(bool screenInstance); // modified by ravikanth 29-06-13 for ITS 0176909
   void videoPlayingChanged(); // added by Dmitry 12.07.13 for ISV85006
// removed by Dmitry 03.10.13 for ITS0187937
// removed by Dmitry 16.08.13 for ITS0184683
   void minerStatusFinished(); // added by Dmitry 14.08.13 for ITS0183845
   void resetFMLaunchBrowseState(int screen); // modified by ravikanth for ITS 0184642
   void readCompleted(ETrackerAbstarctor_VolumeType, int); // added by Dmitry 21.08.13
   void hideVideoMenu(); // mofdified by ravikanth for ISV 89536
   void showContent(int screen, bool show); // added by Dmitry 08.10.13 for ITS0194416
   void cancelJukeboxFormat();
   void formatting(bool started); // added by Dmitry 06.11.13 for ISV94041
   void setFileOperationStopMedia(int startState, bool isFront); // modified for ITS 0214738
   void closeCopyCancelConfirmPopup(); // modified for ITS 0217509
   void closePopupOnFG();//added for ITS 231147 2014.03.26

private:
   void EventFromUIStateManager(Event & aEvent, bool isFront);
   void processJogDirectionKey(Event & aEvent);

public slots:
   // { modified by kihyung 2012.12.23 for SwapDC
   virtual void NotifyFromUIStateManager( Event aEvent );
   virtual void NotifyFromUIStateManagerRear( Event aEvent );
   virtual void NotifyFromUIStateManagerCommon ( Event aEvent );
   // } modified by kihyung 2012.12.23 for SwapDC

   void TAReadCompleted(ETrackerAbstarctor_VolumeType, int); // added by kihyung 2013.2.17    

private:

   struct LocalScreenData
   {
       bool m_isBgState;
       bool m_showFocusOnFg;
       bool m_isListFromMP;
   };

   virtual APP_ID_T GetThisAppId() const { return APP_FMGR; }
   EVT_ID_T GetEvent( const int nKeyCode );
   int variantCountry() { return m_variantCounry; };
   void setCountryChanged(ECountryVariant value);

   void KeyEventHandler( CommonDef::FILEMANAGER_EVT_KEY_T nEventKey, Event aEvent,
                        int screen);
   void JogEventHandle( CommonDef::JOG_DIRECTIONS direction, Event &event );

   //DISPLAY_T getDisplayVisible(); // deleted by lssanh 2013.02.28 ISV72537 USB remove
   void setDisplayVisible( DISPLAY_T display, bool value );
   bool isFileSupported(QString file_path); // added by eugene.seo 2013.03.09

   USBController* GetUSBController(); // added by kihyung 2013.07.08 for ITS 0178816

   //added by edo.lee 2013.05.27
   //APP_ID_T appId;
   APP_ID_T appId_front;
   APP_ID_T appId_rear;
   
   QTranslator *m_pTranslator;
   LANGUAGE_T m_nLanguage;

   bool m_isUsbAttached;
   CommonDef::FILEMANAGER_DISPLAY_T m_nDisplay;
   ECountryVariant m_variantCounry;

   LocalScreenData frontScreenData;
   LocalScreenData rearScreenData;
   EAppFileManagerState m_currentStateTraking;

   bool m_IsFrontVisible;
   bool m_IsRearVisible;
// modified by Dmitry 17.08.13
   bool currendDeviceIsJuke_front;
   bool currendDeviceIsJuke_rear;
   bool m_jogDirectionPressedStatus;
   uint m_jogDirectionKeyPressed;

   DeletedFiles *m_pDeletedFiles; // modified by eugene 2012.08.27 for static analysis defect #6893
   bool m_isMove;// add by eunhye 2012.09.21 for CR 9243
   bool m_bScrollingTicker; // added by junggil 2012.10.04 for CR13505 Media Apps - delta UIS 2.15 to 2.17 
   USBController *m_pUSBController; // added by Sergey for CR#15468
   QHash<CommonDef::FILEMANAGER_DISPLAY_T, QString> m_filePath; // modified by Dmitry 22.05.13
   bool m_popupType; // add by eunhye 2013.02.06 for ISV70222/70319, ITS150138/149862

   // { added by kihyung 2013.2.17
   bool m_bUSB1TAReadComplete;
   bool m_bUSB2TAReadComplete;   
   bool m_bJukeboxTAReadComplete;   
   CTrackerAbstractor *m_pTrackerAbs; 
   CTrackerExtractorClient *m_pExtractor; // added by Dmitry 14.08.13 for ITS0183845
   // } added by kihyung 2013.2.17   
	// { modified - ravikanth 05-03-13
   bool m_bUSB1Attached;
   bool m_bUSB2Attached;
   // {  modified - ravikanth 05-03-13
   ImageInfo m_imageInfoStruct; // added by eugene.seo 2013.03.09

   CLONE_STATUS_T m_eCloneStatus; //added by edo.lee 2013.03.19
   bool m_bfileCopyOperationStatus; // modified by ravikanth 16-04-13 
   bool m_bMiddleEast; // added by Dmitry 03.05.13

   QString m_strSWVersion;
   bool m_bVideoPlaying; // added by Dmitry 12.07.13 for ISV85006
   bool copyFromMainPlayer; //[KOR][ITS][179014][comment](aettie.ji)
   bool m_isFrontLCD; // added by Sergey 02.08.2103 for ITS#181512
   bool m_bisPowerOff; // added by Dmitry 10.10.13 for ITS0194726
// removed by Dmitry 03.10.13 for ITS0187937
   DRM_FILE_T m_drmFile; // added by Sergey 15.09.2013 for ITS#185233
   SharedDataProvider *m_pSharedFMData; //added by suilyou 20130925 for key block
   SharedDataProvider *m_pSharedVideoData; // added by Dmitry 03.10.13 for ITS0187937
   SharedDataProvider *m_pSharedPictureData; // added by Dmitry 03.10.13 for ITS0187937
   CStateManager *m_pSMFront; // modified by Dmitry 09.10.13
   CStateManager *m_pSMRear; // modified by Dmitry 09.10.13
   QString m_strUSB1Path; // added by kihyung 2013.12.09 for ITS 0213549 
   QString m_strUSB2Path; // added by kihyung 2013.12.09 for ITS 0213549 
};

class LocalizeTrigger : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString empty READ empty NOTIFY retrigger);

public:
    QString empty() { return QString(); }

signals:
    void retrigger();
};

#endif // MUSIC_APP_ENGINE_H

