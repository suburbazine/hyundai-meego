/**
 * AppMediaPlayer_EnginePhoto.h
 *
 * Implementation of the Class AppMediaPlayer_EnginePhoto 
 *
 * @author  Golyshkin A.
 * @CreatedDate 29-Nov-2011 1:55:18 PM
 */
#if !defined(EA_B93231A3_C976_4680_9636_D2B624AB14BB__INCLUDED_)
#define EA_B93231A3_C976_4680_9636_D2B624AB14BB__INCLUDED_

#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "engine/DHAVN_AppMediaPlayer_EngineBase.h"
#include "controller/DHAVN_AppMediaPlayer_ControllerPhoto.h"
#include "DHAVN_OptionMenuModel.h"
//#include "util/video/DHAVN_AppMediaPlayer_FSMonitor.h" // removed by kihyuung 2013.2.17

#define DEFAULT_JUKEBOX_PATH "/usr_data/media/Picture"
#define DEFAULT_SAVEFRAME_PATH "/home/meego" //added by yungi 2013.2.8 for UX Scenario 11. Saveframe path changed
#define DEFAULT_SAVEFRAME_FILE1 "saveframe1" //added by kihyung 2013.08.14 for ITS 0184288  
#define DEFAULT_SAVEFRAME_FILE2 "saveframe2" //added by kihyung 2013.08.14 for ITS 0184288  

enum PHOTO_EVT_KEY_T
{
   PHOTO_EVT_KEY_MENU =0,
   PHOTO_EVT_KEY_BACK
};

class AppMediaPlayer_EnginePhoto : public AppMediaPlayer_EngineBase
{
   Q_OBJECT

   Q_PROPERTY(QUrl currentSource READ currentSource NOTIFY currentSourceChanged) // modified by eugene.seo 2013.02.18
   Q_PROPERTY(bool isSlideShow READ isSlideShow WRITE setIsSlideShow NOTIFY isSlideShowChanged)
   Q_PROPERTY(int slideShowDelay READ slideShowDelay WRITE setSlideShowDelay NOTIFY slideShowDelayChanged)
   Q_PROPERTY(bool uiHidden READ isUIHidden NOTIFY photoUIHidden)
   Q_PROPERTY(OptionMenuModel *optMenuModel READ GetOptMenuModel WRITE SetOptMenuModel NOTIFY optMenuModelChanged)
   Q_PROPERTY(bool isRunFromFileManager READ isRunFromFileManager WRITE setIsRunFromFileManager)
   Q_PROPERTY(bool isActive READ getIsActive NOTIFY isActiveChanged)
   Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
   Q_PROPERTY(int playlistCount READ playlistCount NOTIFY playlistCountChanged)
   Q_PROPERTY(bool isUnsupportedFile READ isUnsupportedFile NOTIFY currentSourceChanged) // modified by ravikanth 15-05-13
   Q_PROPERTY(bool m_isDRSShow READ isDRSShow NOTIFY isDRSChanging) //added by edo.lee 2013.03.04
   Q_PROPERTY(bool loadingOnCopy READ  loadingOnCopy NOTIFY jukeboxCopyInProgress) // modified by ravikanth 18-04-13
   Q_PROPERTY(bool mirrorLayout READ mirrorLayout WRITE setMirrorLayout NOTIFY mirrorLayoutChanged) // modified by ravikanth 04-05-13
   Q_PROPERTY(QString folderName READ folderName NOTIFY folderNameChanged) // modified for ITS 0215804

public:

   AppMediaPlayer_EnginePhoto( AppMediaPlayer_EngineMain &aEngineMain, bool isFrontDisplay );
   ~AppMediaPlayer_EnginePhoto();

   Q_INVOKABLE void next();
   Q_INVOKABLE void previous();
   Q_INVOKABLE void save(qreal angle);


   //GETTERS
   // QUrl currentSource() { return QUrl::fromLocalFile(m_source); } // modified by eugene.seo 2013.02.18
    QUrl currentSource(); // modified by ravikanth 02-04-13
   bool isSlideShow() { return m_slideShowTimer.isActive(); }
   int slideShowDelay() { return m_slideShowDelay; }
   bool statusBarVisible() { return m_statusBarVisible; }
   int currentIndex() { return GetCurrentIndex(); } // modified by Sergey for ITS#159209 for 27.04.2013
   int playlistCount() { return GetCurrentPlayListCount(); } // modified by Sergey for ITS#159209 for 27.04.2013
   bool isUnsupportedFile() { return m_unsupportedFile; } // modified by ravikanth 15-05-13
   bool isDRSShow() {return m_isDRSShow;}//added by edo.lee 2013.03.04
   QString folderName();

   //SETTERS
   void setIsSlideShow(bool value);
   // modified by ruindmby 2012.11.30 for CR#15766
   void setCurrentSource(QString source, bool onPlaylistSet = false); // modified by ravikanth 12-07-13
   void setSlideShowDelay(int delay);
   // modified by ruindmby 2012.11.30 for CR#15766
   Q_INVOKABLE void setStatusBarVisible( bool visible );
   void setCurrentDisplaySlideShowDelay(int delay); // modified by ravikanth 27-07-13 for ITS 181678
   void stopSlideShowForSource(const PHOTO_SOURCES_LIST source);

signals:
   void currentSourceChanged();
   void slideShowNext();
   void isSlideShowChanged();
   void slideShowDelayChanged();
   void statusBarVisibleChanged();
   void currentIndexChanged();
   void playlistCountChanged();
   // void showImageLoading(); // commented by ravikanth 15-05-13
   void isDRSChanging();//added by edo.lee 2013.03.04
   void jukeboxCopyInProgress(); // modified by ravikanth 18-04-13
   void mirrorLayoutChanged(); // modified by ravikanth 04-05-13
// removed by sangmin.seol ITS_0218558 Remove signalJogNavigation for Engine Photo.
//#ifdef DUAL_KEY
//   uint signalTransformEVT2QML_KEY(EVT_ID_T key);
//   void signalJogNavigation( uint arrow, int status, bool bRRC, int nRepeat );
//#endif
// removed by sangmin.seol ITS_0218558 Remove signalJogNavigation for Engine Photo.
   void folderNameChanged(); // modified for ITS 0215804

private:
   void setStatusBarVisiblePrivate(bool visible);

private:
   QTimer m_slideShowTimer;
   QString m_source;
   int m_slideShowDelay;
   bool m_statusBarVisible;
   bool m_isDRSShow;//added by yongkyun.lee 2012.12.03 for photo DRS from VR.
   bool m_unsupportedFile; // modified by ravikanth 15-05-13
   bool m_isHideScreen; //added by edo.lee 2013.01.10

public:
   virtual void ShowUI(DISPLAY_T display);
   virtual void HideUI(DISPLAY_T display);
   void * GetUIState();

   Q_INVOKABLE QString getCurrentFileNameByPath(const QString& fileFullPath);
   Q_INVOKABLE QString getCurrentFileExtByPath(const QString& fileFullPath);
   Q_INVOKABLE QString getCurrentFolderNameByPath(const QString& fileFullPath);
   Q_INVOKABLE QString getCurrentFilePath(const QString& fileFullPath);
   Q_INVOKABLE QString getCurrentFileCreatedDateByPath(const QString& fileFullPath); // added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
   Q_INVOKABLE int GetCurrentPlayListCount();
   Q_INVOKABLE void SetNotifyAllUnsupported(int sourceID); // added by kihyung 2013.4.14
   Q_INVOKABLE int  GetCurrentIndex();
   Q_INVOKABLE void SetCurModePlaylist();
   Q_INVOKABLE void HandleBackKey(bool onUserBack = false);
   Q_INVOKABLE void HandleSoftBackKey(bool isFront, bool bRRC); //modified by aettie 20130620 for back key event
   Q_INVOKABLE bool SaveAsFrame( const bool replace );
   Q_INVOKABLE void LaunchScreenSettings();
   // { modified by wspark 2012.11.20 for CR15521
   //Q_INVOKABLE bool CopyImageToJukebox();
   Q_INVOKABLE bool CopyImageToJukebox(bool replace);
   // } modified by wspark
   Q_INVOKABLE void HandleNoImageInJukebox(); // added by wspark 2012.11.01 for SANITY_CM_AJ559
   Q_INVOKABLE void setUnsupportedOnImageLoadingError(); // modified on 31-12-13 for HKMC image load error fix

   virtual bool EventHandler( Event& aEvent, DISPLAY_T display );
   Q_INVOKABLE void LaunchFileManager( QString folderPath );

   virtual void PlayTrackFromFileManager( QString strNewFilePath );
   Q_INVOKABLE bool restorePreviouslyPlayedImage();//Added by alexey edelev 2012.09.08. Fix bug 13059.
   PHOTO_SOURCES_LIST getSourceByPath(const QString& filePath); //Added by Alexey Edelev 2012.09.07. CR13196

   // void trytoRestoreJukebox(); // removed by kihyung 2013.2.17
   void trytoRestoreDevice(const PHOTO_SOURCES_LIST restored_source); // modified by kihyung 2013.2.17

   virtual void LoadSettings();
   Q_INVOKABLE virtual void SaveSettings(bool bSync = false); // modified by kihyung 2013.08.09 for ITS 0183077
   virtual void Restore(MODE_STATE_T mode);

   AppMediaPlayer_Controller_Base* GetController() { return &m_PhotoController; };

   OptionMenuModel* GetOptMenuModel() { return m_pOptMenuModel; }
   void SetOptMenuModel( OptionMenuModel *optMenuModel );

   //void createFSMonitorForSource (PHOTO_SOURCES_LIST source,const QString monitor_path); // removed by kihyung 2013.2.17
   //{modified by edo.lee 2012.08.22 for new UX Photo(LGE) #5
   //void updateSource( int source_id );
   Q_INVOKABLE void updateSource( int source_id );
   //}modified by edo.lee
   Q_INVOKABLE void SwitchMode(const int source_id);
   Q_INVOKABLE bool isSwitchFromRestore(); // added by kihyung 2013.3.19 for ISV 67749
   Q_INVOKABLE void setIsSwitchFromRestore(bool bSwitch) { m_isSwitchFromRestore = bSwitch; }  // added by kihyung 2013.3.19 for ISV 67749
   void OptionMenuUpdate( PHOTO_SOURCES_LIST source ); 
   Q_INVOKABLE void photoMenuUpdate( int source );//modified by edo.lee 2013.06.08

   bool isUIHidden(){ return m_nHideUI; }; // added by kihyung 2012.07.06 for CR 10841

   //changed by Alexey Edelev. Fix bug 13059. 2012.08.24
   //{
   bool isRunFromFileManager() { return m_isRunFromFileManager; }
   void setIsRunFromFileManager( bool isRunFromFileManager ) { m_isRunFromFileManager = isRunFromFileManager; }
   //}
   //changed by Alexey Edelev. Fix bug 13059. 2012.08.24

   Q_INVOKABLE QString makeElidedString( const QString &fullString, const QString &fontFamily, int pixelSize, int maxWidth ); //added by changjin 2012.09.03 : for CR 13367

   // { added by junggil 2012.10.05 for SANITY_CM_AJ497 
   Q_INVOKABLE  int getStringWidth( const QString &fullString, const QString &fontFamily, int pixelSize );
   // } added by junggil

   // removed by ruindmby 2012.11.30 for CR# 16072
   //Q_INVOKABLE bool getIsNewSource() const {return m_isNewSource;} //added by edo.lee 2012.10.09 for CR 13745
   // removed by ruindmby 2012.11.30 for CR# 16072
   void NotifyUSBPhotoStatus( bool USBPhotoStatus );  // added by dongjin 2012.10.17 for CR13581
   bool getIsActive();//Added by Alexey Edelev 2012.10.23 CR14638
   Q_INVOKABLE void setCopy(bool isCopy) { m_isCopy = isCopy; } // added by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list
   Q_INVOKABLE void setIsBackPressByJog(bool state) {m_isBackPressByJog = state;} //added by edo.lee 2013.03.30(bool PlaybackControl) {m_focusPlaybackControl = PlaybackControl;};

   Q_INVOKABLE QString getThumbnailName(const QString &image); // modified by ravikanth 08-04-13

	// { modified by ravikanth 16-04-13
   Q_INVOKABLE void CancelCopyJukebox();
   Q_INVOKABLE bool isCopyInProgress() { return m_EngineMain.isCopyInProgress();}
   bool loadingOnCopy(); // modified by ravikanth 18-04-13
   // } modified by ravikanth 16-04-13
   void setRescanProgressLoading(); // modified by ravikanth 20-04-13
   void ResetProgressLoading(); // modified by ravikanth 20-04-13
   bool mirrorLayout() { return m_bMirrorLayout; } // modified by ravikanth 04-05-13
   void setMirrorLayout(bool mirrored) { m_bMirrorLayout = mirrored; emit mirrorLayoutChanged(); } // modified by ravikanth 04-05-13
   Q_INVOKABLE int GetAllPlaylistCurrentIndex(); // modified by ravikanth 15-05-13
   Q_INVOKABLE int GetAllPlaylistTotalCount(); // modified by ravikanth 15-05-13
   Q_INVOKABLE void stopOtherViewSlideShow();//added by edo.lee 2013.06.09
   Q_INVOKABLE bool isHighResolutionImage(); // modified by ravikanth 13.06.2013 for ITS 0172957
   void syncImageFRonDRS(QString path, PHOTO_SOURCES_LIST source = PHOTO_SOURCE_MAX);//added by edo.lee 2013.07.03
   QString GetCurrentFilePath();//added by edo.lee 2013.07.03
   int getCurrentSourceID(){return m_PhotoController.getCurSourceID();}//added by edo.lee 2013.07.03

   PHOTO_LAST_STATE_T* getLastState() { return m_LastState; } // added by kihyung 2013.07.25 for ITS 0181599 
	// modified by ravikanth 27-07-13 for ITS 181678
    void SetPhotoIndex(PHOTO_SOURCES_LIST source, int index);
    void SetPhotoLastState(PHOTO_SOURCES_LIST source);
    void setSlideShowDelayToOptions();

signals:

   void photoIsFront(bool isFront);//added by edo.lee 2013.07.03
   void photoRequestFG(bool retainPreviousState); // modified by ravikanth 11-07-13 for ITS 0179160
   void photoUpdateSource(int sourceId, int sourceMask);
   void photoDRSModeON();
   void photoDRSModeOFF();
   void retranslateUI(int langId);
   void photoKeyMenuReleased(bool nKeyType); // modified by dongjin 2012.11.23
   void photoKeyBackPressed();
   void photoUIHidden();
   void backtoPrev(int sourceMask); //added by edo.lee 2012.08.24
   	
   void noImageFile(int sourceId);
   void closeMenu();

   //{ added by yongkyun.lee 20130401 for : : ISV 78014
   void usbPluggedOut(int newSourceID);
   //void usbPluggedOut();
   //} added by yongkyun.lee 20130401 
   void showErrorPopup();
   void onMenuClosed(); //added by edo.lee 2013.05.31
   void closeCopyCancelConfirmPopup(); // modified for ITS 0217509

   //Q_PROPERTY NOTIFY signals
   // removed by ruindmby 2012.11.30 for CR#15766
   // void zoomChanged(int level);
   // void setZoomWithPinchMode(int level); //modified by edo.lee 2012.09.18  after zoom with pinch , push the 4 direction
   // removed by ruindmby 2012.11.30 for CR#15766
   void NotifyUISH(Event aEvent);
   void NotifyUISH(Event aEvent, DISPLAY_T aDisplayType); // modified by kihyung 2013.1.26
   void isActiveChanged();//Added by Alexey Edelev 2012.10.23 CR14638
   void handleAllUnsupportedFiles(int mediaSource); // modified by ravikanth 28-04-13
   void copyCompleted(); // modified by ravikanth 12-07-13 for ITS 0174301
   void defaulControlCue();

public slots:
   void onDeviceAttached(MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType, bool autoplay);
   void onDeviceRemoved(MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType );
   // void onContentChanged(); // removed by kihyung 2013.2.17
   // modified by ravikanth 28-04-13
   void updateFilesInCurFolder(QStringList fileList, int mediaSource); // added by Sergey for ITS#159209 for 27.04.2013
   void skip(); // added by Sergey 22.05.2013
   void onHandleAllUnsupportedFiles(int source); // modified for icon disable on all unsupported files

private:
   void KeyEventHandler(PHOTO_EVT_KEY_T nEventKey, Event aEvent);
   // bool isUIHidden(){ return m_nHideUI; }; // commentted by kihyung 2012.07.06 for CR 10841
   void setUIHidden( bool value );
   void SetPhotoEntryPoint ( APP_ENTRY_POINT_T entryPoint );

private:
   AppMediaPlayer_EngineMain &m_EngineMain;
   QTranslator *m_pTranslator;
   AppMediaPlayer_Controller_Photo m_PhotoController;

   bool m_nHideUI;

   OptionMenuModel *m_pOptMenuModel;
   // AppMediaPlayer_FSMonitor *m_pFSMonitor[PHOTO_SOURCE_MAX]; // removed by kihyung 2013.2.17
   // bool m_nZoomWithPinchMode; // removed by ruindmby 2012.11.30 for CR#15766
   //changed by Alexey Edelev. Fix bug 13059. 2012.08.24
   //{
   bool m_isRunFromFileManager;
   //}
   //changed by Alexey Edelev. Fix bug 13059. 2012.08.24
   // removed by ruindmby 2012.11.30 for CR# 16072
   //bool m_isNewSource;  //added by edo.lee 2012.10.09 for CR 13745
   // modified by ruindmby 2012.11.30 for CR# 16072

   QString m_FMPreviouslyPlayedImage;//Added by alexey edelev 2012.09.08. Fix bug 13059.
   PHOTO_LAST_STATE_T m_LastState[PHOTO_DISPLAY_NONE]; // added by ruindmby 2012.11.30 for CR#15766
   PHOTO_DISPLAY m_CurDisplay; // added by ruindmby 2012.11.30 for CR#15766
   //QDeclarativeView* OSDview; //delete by ys 2013.09.30 //added by edo.lee 2012.12.08 for statusbar
   bool m_isCopy; // added by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list 
   bool m_isSwitchFromRestore;  // added by kihyung 2013.3.19 for ISV 67749
   bool m_isFrontSoftBackPressed; //added by edo.lee 2013.03.30
   bool m_isRearSoftBackPressed;//added by edo.lee 2013.03.30
   bool m_isBackPressByJog;//added by edo.lee 2013.03.30
   bool m_trackerUpdateInProgress; // modified by ravikanth 20-04-13
   bool m_bMirrorLayout; // modified by ravikanth 04-05-13
   bool m_bSeekForward; // added by Sergey 22.05.2013
   bool m_bPreviousDRSState; // modified by ravikanth 11-07-13 for ITS 0179160
   bool m_bPhotolistlunch;
   bool m_bBackOnUserRequest; //added by Michael.Kim 2013.10.11 for ITS 194857

Q_SIGNALS:
   void optMenuModelChanged();
};

#endif // !defined(EA_B93231A3_C976_4680_9636_D2B624AB14BB__INCLUDED_)
