#ifndef DHAVN_APPFILEMANAGER_UI_MAIN_H
#define DHAVN_APPFILEMANAGER_UI_MAIN_H
// modified by Dmitry 17.08.13
#include <QObject>
#include <DHAVN_AppFileManager_Engine.h>
#include <DHAVN_AppFileManager_CStateManager.h>
#include <DHAVN_AppFileManager_TitleProvider.h>
#include <QString>
#include <QVariant>
#include "DHAVN_AppFileManager_FileManagerMenu.h"
#include <usbcontroller.h>
#include "DHAVN_AppFileManager_Logger.h"
#include <DHAVN_FileOperationsHelper.h> // added by Sergey for CR#14494

class SharedDataProvider; // modified by ravikanth 27-04-13

class UIMainDef : public QObject
{
   Q_OBJECT

   Q_ENUMS( BOTTOM_BAR_MODES )
   Q_ENUMS( DEVICES )
   Q_ENUMS( FILE_TYPES )
   Q_ENUMS( POPUP_TYPE )
   Q_ENUMS( BOTTOM_BAR_EVENTS )
   Q_ENUMS( POPUP_EVENTS )
   Q_ENUMS( MODE_AREA_EVENTS )
   Q_ENUMS( KEYPAD_EVENTS )
   Q_ENUMS( CONTENT_MODES )
   Q_ENUMS( MUSIC_FILTERS )
   Q_ENUMS( FOLDER_POPUP_MODES )
   Q_ENUMS( CONTENT_EVENTS )
   Q_ENUMS( MODE_AREA_TEXT )
   Q_ENUMS(CURRENT_OPERATION)

public:

   enum MODE_AREA_TEXT
   {
      MODE_AREA_TEXT_DEFAULT = 0,                       // "List"
      MODE_AREA_TEXT_COPY_FROM,                     // "Copy to Jukebox"
      MODE_AREA_TEXT_COPY_TO,                       // "Select a location to be copied"
      MODE_AREA_TEXT_ENTER_FOLDER_NAME,             // "Enter the folder name"
      MODE_AREA_TEXT_RENAME,                        // "Input New Name"
      MODE_AREA_TEXT_MOVE_FROM,                     // "Move"
      MODE_AREA_TEXT_MOVE_TO,                       // "Select where to move"
      MODE_AREA_TEXT_DELETE,                        // "Delete"
      MODE_AREA_TEXT_WAITING_DISPLAY,               //"Waiting Display Image Setting"
      MODE_AREA_TEXT_INVALID
   };

   enum BOTTOM_BAR_MODES
   {
      BOTTOM_BAR_INVISIBLE,
      BOTTOM_BAR_COPY_TO,
      BOTTOM_BAR_COPY_FROM,
      BOTTOM_BAR_SELECT_DESELECT,
      BOTTOM_BAR_MOVE_TO,
      BOTTOM_BAR_DELETE,
      BOTTOM_BAR_MOVE,
      BOTTOM_BAR_DELETE_ALL
   };

   enum DEVICES
   {
      DEVICE_USB,
      DEVICE_JUKEBOX
   };

   enum FILE_TYPES
   {
      FILE_TYPE_AUDIO,
      FILE_TYPE_VIDEO,
      FILE_TYPE_PICTURE
   };

   enum POPUP_TYPE
   {
      POPUP_TYPE_START,
      POPUP_TYPE_COPY,
      POPUP_TYPE_COPYING,
      POPUP_TYPE_COPY_COMPLETE,
      POPUP_TYPE_DELETE,
      POPUP_TYPE_DELETE_FOLDER,
      POPUP_TYPE_DELETE_ALL,
      POPUP_TYPE_DELETING,
      POPUP_TYPE_DELETE_COMPLETE,
      POPUP_TYPE_NOUSB,
      POPUP_TYPE_FOLDER_OPERATION,
      POPUP_TYPE_FILE_VIDEO_JUKEBOX_OPERATION,
      POPUP_TYPE_FILE_VIDEO_USB_OPERATION,
      POPUP_TYPE_FILE_PICTURE_JUKEBOX_OPERATION,
      POPUP_TYPE_FILE_PICTURE_USB_OPERATION,
      POPUP_TYPE_SET_AS_FRAME,
      POPUP_TYPE_FRAME_SAVED,
      POPUP_TYPE_EDIT_MODE,
      POPUP_TYPE_FOLDER_NAME_IS_TOO_LONG,
      POPUP_TYPE_FOLDER_ALREADY_EXIST,
      POPUP_TYPE_FILE_ALREADY_EXIST,
      POPUP_TYPE_EMPTY_FOLDER_NAME,
      POPUP_TYPE_EMPTY_FILE_NAME,
      POPUP_TYPE_INCORRECT_CHARACTER,
      POPUP_TYPE_REPLACE_FILE,
      POPUP_TYPE_COPY_CANCELLED,
      POPUP_TYPE_CAPACITY_ERROR,
      POPUP_TYPE_COPY_CANCEL,
      POPUP_TYPE_MOVE,
      POPUP_TYPE_MOVING,
      POPUP_TYPE_MOVE_ALL,  //30
      POPUP_TYPE_MOVE_CANCEL,
      POPUP_TYPE_MOVE_CANCELLED,
      POPUP_TYPE_MOVE_COMPLETE,
      POPUP_TYPE_JUKEBOX_CAPACITY,
      POPUP_TYPE_OPTION_MENU_DELETE_ALL,
      POPUP_TYPE_DELETING_ALL,
      POPUP_TYPE_CORROPTED_ERROR,
      POPUP_TYPE_JUKEBOX_ERROR,
      POPUP_TYPE_FILE_NAME_IS_TOO_LONG,
      POPUP_TYPE_FOLDER_IS_USED,
      POPUP_TYPE_FILE_IS_USED,
      POPUP_TYPE_COPY_ALL_CONFIRMATION
      // { added by lssanh 2012.09.17 for CR13482
      ,POPUP_TYPE_FORMATTING
      ,POPUP_TYPE_FORMAT_COMPLETE
      // } added by lssanh 2012.09.17 for CR13482
      ,POPUP_TYPE_UNSUPPORTED_FILE  // added by Sergey for CR#15468
      ,POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM // modified by ravikanth 16-04-13 
      ,POPUP_TYPE_FILE_CANNOT_DELETE // modified by ravikanth 27-04-13
      ,POPUP_TYPE_REPLACE_FRAME_FILE //added by aettie 201300613 for ITS 173605
      ,POPUP_TYPE_DELETE_INCOMPLETE // modified by ravikanth 21.06.13 for ITS 0174571
      ,POPUP_TYPE_CANCEL_COPY_FOR_DELETE_CONFIRM //50 // modified by ravikanth 21-07-13 for copy cancel confirm on delete
      ,POPUP_TYPE_CANCEL_COPY_FOR_CLEAR_JUKEBOX_CONFIRM // modified by ravikanth 24-07-13 for copy spec changes
      ,POPUP_TYPE_DRM_CONFIRM	// added by Sergey 15.09.2013 for ITS#185233
      ,POPUP_TYPE_DRM_EXPIRED		// added by Sergey 15.09.2013 for ITS#185233
      ,POPUP_TYPE_DRM_NOT_AUTH	// added by Sergey 15.09.2013 for ITS#185233
      ,POPUP_TYPE_AUDIO_FORMAT_UNSUPPORTED
      ,POPUP_TYPE_RESOLUTION_UNSUPPORTED // added by Sergey 10.11.2013 new DivX popups

      ,POPUP_TYPE_MAX
   };

   enum BOTTOM_BAR_EVENTS
   {
      BOTTOM_BAR_EVENT_SELECT_AUDIO_FILES,
      BOTTOM_BAR_EVENT_SELECT_VIDEO_FILES = 1,
      BOTTOM_BAR_EVENT_SELECT_PICTURE_FILES,
      BOTTOM_BAR_EVENT_COPY,
      BOTTOM_BAR_EVENT_DELETE,
      BOTTOM_BAR_EVENT_COPY_HERE,
      BOTTOM_BAR_EVENT_CREATE_FOLDER,
      BOTTOM_BAR_EVENT_CANCEL,
      BOTTOM_BAR_EVENT_SELECT_ALL,
      BOTTOM_BAR_EVENT_DESELECT_ALL,
      BOTTOM_BAR_EVENT_MOVE,
      BOTTOM_BAR_EVENT_MOVE_ALL,
      BOTTOM_BAR_EVENT_MOVE_FILE,
      BOTTOM_BAR_EVENT_MOVE_HERE,
      BOTTOM_BAR_EVENT_MOVE_CANCEL,
      BOTTOM_BAR_EVENT_COPY_ALL,
      BOTTOM_BAR_EVENT_DELETE_ALL,
      BOTTOM_BAR_EVENT_CANCEL_ON_FIRST_LEVEL
   };

   enum POPUP_EVENTS
   {
      POPUP_EVENT_COPY_CONFIRMATION,
      POPUP_EVENT_COPY_COMPLETED,
      POPUP_EVENT_COPY_CANCEL,
      POPUP_EVENT_COPY_COMPLETE_POPUP_CLOSED,
      POPUP_EVENT_DELETE_CONFIRMATION,
      POPUP_EVENT_DELETE_CANCEL,
      POPUP_EVENT_DELETE_ALL_CANCEL,
      POPUP_EVENT_DELETE_COMPLETED,
      POPUP_EVENT_DELETE_COMPLETE_POPUP_CLOSED,
      POPUP_EVENT_NOUSB_OK,
      POPUP_EVENT_FILE_OPERATION_RENAME,
      POPUP_EVENT_FILE_OPERATION_DELETE,
      POPUP_EVENT_FOLDER_OPERATION_DELETE,
      POPUP_EVENT_FILE_OPERATION_CANCEL,
      POPUP_EVENT_SAVE_FRAME_CANCEL,
      POPUP_EVENT_SAVE_FRAME_JUKEBOX,
      POPUP_EVENT_EDIT_MODE_OK,
      POPUP_EVENT_SAVE_FRAME_REQUEST,
      POPUP_EVENT_CREATE_FOLDER_EMPTY_NAME,
      POPUP_EVENT_CREATE_FOLDER_NAME_IS_TOO_LONG,
      POPUP_EVENT_CREATE_FOLDER_INCORRECT_CHARACTER,
      POPUP_EVENT_CREATE_FOLDER_FOLDER_ALREADY_EXIST,
      POPUP_EVENT_REPLACE,
      POPUP_EVENT_REPLACE_ALL,
      POPUP_EVENT_NO_REPLACE,
      POPUP_EVENT_REPLACE_CANCEL,
      POPUP_EVENT_CAPACITY_ERROR_CONFIRM,
      POPUP_EVENT_JUKEBOX,
      POPUP_EVENT_COPY_CONTINUE,
      POPUP_EVENT_COPY_CANCEL_CONFIRM,
      POPUP_EVENT_COPY_CANCELED_POPUP_CLOSED,
      POPUP_EVENT_MOVE_ALL_CANCEL,
      POPUP_EVENT_MOVE_CANCEL,
      POPUP_EVENT_MOVE_CANCEL_CONFIRM,
      POPUP_EVENT_MOVE_CANCELED_POPUP_CLOSED,
      POPUP_EVENT_MOVE_CONTINUE,
      POPUP_EVENT_MOVE_COMPLETED,
      POPUP_EVENT_JUKEBOX_CAPACITY_OK,
      POPUP_EVENT_FORMATTING_JUKEBOX_CONFIRMATION,
      POPUP_EVENT_FOLDER_OPERATION_RENAME,
      POPUP_EVENT_SAVE_FRAME_COMPLETED,
      POPUP_EVENT_COPY_ALL_CONFIRMATION,
      POPUP_EVENT_COPY_ALL_CANCEL_CONFIRMATION,
      POPUP_TYPE_COPY_CANCEL_INFO, //added by yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
      POPUP_EVENT_COPY_TO_JUKEBOX_CONFIRM, // modified by ravikanth 16-04-13 
      POPUP_EVENT_JUKEBOX_COPY_CANCEL_CONFIRM, // modified by ravikanth 16-04-13 
      POPUP_EVENT_COPY_PROGRESS_CLOSE, // added by eugene.seo 2013.06.10
      POPUP_EVENT_DELETE_PROGRESS_CANCEL, // modified by ravikanth 19.06.13 for ITS 0174577
      POPUP_EVENT_DELETE_INCOMPLETE, // modified by ravikanth 21.06.13 for ITS 0174571
      POPUP_EVENT_FRAME_REPLACE_CANCEL, // modified by ravikanth 05-07-13 for ITS 0178514
      POPUP_EVENT_CANCEL_COPY_FOR_DELETE_CONFIRM, // modified by ravikanth 21-07-13 for copy cancel confirm on delete
      POPUP_EVENT_CANCEL_COPY_ALL_FOR_COPY_CONFIRM,	// Added by sangmin.seol 2013.10.22 ITS 0195517 for reset sellect-all when copy confirm popup canceled
      POPUP_EVENT_CLEAR_STATE_TO_NONE 
   };

   enum MODE_AREA_EVENTS
   {
      MODE_AREA_EVENT_BACK_PRESSED,
      MODE_AREA_EVENT_EDIT_PRESSED
   };

   enum KEYPAD_EVENTS
   {
      KEYPAD_EVENT_TEXT_ENERED
   };

   enum CONTENT_EVENTS
   {
      CONTENT_EVENT_FOLDER_OPERATION,
      CONTENT_EVENT_FILE_OPERATION,
      CONTENT_EVENT_FILE_BLOCK_DELETE_OPERATION // modified by ravikanth 27-04-13
   };

   enum CONTENT_MODES
   {
      CONTENT_MODE_UNDEFINED,
      CONTENT_MODE_LIST,
      CONTENT_MODE_FILEMANAGER
   };

   enum MUSIC_FILTERS
   {
      MUSIC_FILTER_ARTIST,
      MUSIC_FILTER_ALBUM,
      MUSIC_FILTER_SONG
   };

   enum FOLDER_POPUP_MODES
   {
       FOLDER_MODE_FOLDER,
       FOLDER_MODE_FILE_PICTURE_USB,
       FOLDER_MODE_FILE_PICTURE_JUKEBOX,
       FOLDER_MODE_FILE_VIDEO_USB,
       FOLDER_MODE_FILE_VIDEO_JUKEBOX
   };

   enum CURRENT_OPERATION
   {
      CURRENT_OPERATION_MOVE,
      CURRENT_OPERATION_COPY,
      // added by minho 20121016
      // for CR14271: [3PDMS][142453] When Power Off/On from the pop-up of Cancel Copying files on Jukebox while playing USB Video, the pop-up disappeared
      CURRENT_OPERATION_MOVE_CANCEL,
      CURRENT_OPERATION_COPY_CANCEL,
      // added by minho
      // { added by junggil 2012.10.18 for CR14336 [3PDMS][141287] No display of pop-up "Copy to Jukebox" when rejecting a call  
      CURRENT_OPERATION_MOVE_ALL,
      CURRENT_OPERATION_COPY_ALL,
      // } added by junggil 
	  // { added by eunhye 2012.10.18 for Function_Jukebox_1655
      CURRENT_OPERATION_DELETE,
      CURRENT_OPERATION_DELETE_ALL,
	  // } added by eunhye 2012.10.18
      CURRENT_OPERATION_INVALID,
      CURRENT_OPERATION_SAVE_FRAME, //added by aettie 201300613 for ITS 173605
      CURRENT_OPERATION_COPY_REPLACE_CANCEL, // modified by ravikanth 16-07-13 for ISV 87550 ,ITS 0178185
      CURRENT_OPERATION_COPY_CANCEL_FOR_COPY,
      CURRENT_OPERATION_FORMAT_JUKEBOX // modified by ravikanth 21-07-13 for copy cancel confirm on delete
   };
};

class UI_Main : public QObject
{
   Q_OBJECT

public:
   UI_Main( FileManagerEngine *pEngile, CStateManager *pStateManager );
   ~UI_Main();

   Q_PROPERTY( int bottomBarMode READ getBottomBarMode NOTIFY bottomBarModeChanged )
   Q_PROPERTY( int currentDevice READ getCurrentDevice NOTIFY currentDeviceChanged )
   Q_PROPERTY( int currentFileType READ getCurrentFileType NOTIFY currentFileTypeChanged )

   //ModeArea
   Q_PROPERTY( bool editButton READ getEditButton NOTIFY editButtonChanged )
   Q_PROPERTY( QString modeAreaTitle READ getModeAreaTitle NOTIFY modeAreaTitleChanged )
   Q_PROPERTY( QString modeAreaFileCount READ getModeAreaFileCount NOTIFY modeAreaFileCountChanged ) //added by yungi 2013.03.08 for New UX FileCount

   //Content
   Q_PROPERTY( int contentMode READ getContentMode NOTIFY contentModeChanged )
   Q_PROPERTY( int musicFileter READ getMusicFilter NOTIFY musicFilterChanged )
   Q_PROPERTY( bool browseOnly READ getBrowseOnly NOTIFY browseOnlyChanged )

   Q_PROPERTY( bool musicIsChecked READ emptyBoolGetter WRITE setMusicChecked )
   Q_PROPERTY( bool videoIsChecked READ emptyBoolGetter WRITE setVideoChecked )
   Q_PROPERTY( bool pictureIsChecked READ emptyBoolGetter WRITE setPictureChecked )

   Q_PROPERTY( bool selectAllEnabled READ getSelectAllEnabled NOTIFY selectAllEnabledChanged);
   Q_PROPERTY( bool deselectAllEnabled READ getDeselectAllEnabled NOTIFY deselectAllEnabledChanged);

   Q_PROPERTY( bool hideBottomBar READ getHideBottomBar NOTIFY notifyHideBottomBar )

   //PopUp
   Q_PROPERTY( QString selectedFodler READ getSelectedFolder )
   Q_PROPERTY( int currentPopupEvent READ getCurrentPopupEvent WRITE setCurrentPopupEvent ) // add by wspark 2012.07.12 for CR9616.
   Q_PROPERTY( bool is_CopyAll READ getCopyAllStatus WRITE setCopyAllStatus ) // add by wspark 2012.07.25 for CR12226.

   //Keypad
   Q_PROPERTY( QString keyPadText READ getKeyPadText )

   //Indexing Popup
   Q_PROPERTY( bool showIndexingPopup READ getIndexingPopup NOTIFY notifyIndexingPopup)

   Q_PROPERTY( QString selectedPictureName READ getSelectedPictureName)

   Q_PROPERTY( int currentCopySkipCount READ currentCopySkipCount NOTIFY copySkipCountChanged ) // modified by ravikanth 22-04-13
   Q_PROPERTY( int currentCopyTotalCount READ currentCopyTotalCount NOTIFY copySkipCountChanged ) // modified by ravikanth 22-04-13

public:
   TitleProvider * getTitleProvider();
   FileManagerMenu* getOptionMenu();

   Q_INVOKABLE void handleMenuKey();
   Q_INVOKABLE QString getProcessedFile();

   void setEditButton( bool value );
   void setBottomBarMode( int value );
   void contextDeleteId();
   void contextDeleteAllId();
   void contextMoveId();
   void contextInteractiveId();
   Q_INVOKABLE double getJukeboxCapacity(); // modified by ruindmby 2012.08.22 for CR12454
   Q_INVOKABLE double getJukeboxUsedSize(); // modified by ruindmby 2012.08.22 for CR12454
   void showCapacityPopup();
   Q_INVOKABLE int getFilesCount();
   Q_INVOKABLE int getFilesSize();
   void handleNowPlaying();
   void StartJukeBoxFormat();

   void cancelMoveMode();

   void setModeAreaMode(UIMainDef::MODE_AREA_TEXT type);

   bool getCopyAllStatus();
   Q_INVOKABLE void setCopyAllStatus(bool value);
   Q_INVOKABLE void setModeAreaCount(QString count); //added by yungi 2013.03.08 for New UX FileCount
   // } add by wspark
   UIMainDef::CURRENT_OPERATION getCurrentFileOperation() { return m_pCurrentOperation; } ; // added by wspark 2013.02.03
   bool getCurrentAudioFileOperation() { return m_pEngine->GetIsfileCopyOperationActive();} // modified by ravikanth 16-04-13

   CStateManager* GetStateManager() { return m_pStateManager; } // added by wspark 2012.12.13 for CR16396

   Q_INVOKABLE void setCopySkipCount(int count); // modified by ravikanth 22-04-13
   Q_INVOKABLE void setCopyTotalCount(int count); // modified by ravikanth 22-04-13
   Q_INVOKABLE bool IsCurrentVideoPlaying(); // modified by ravikanth 27-04-13
   Q_INVOKABLE int currentCopyReplaceCount(){ return m_currentReplaceCopyCount; } // modified by ravikanth 07-07-13 for ITS 0178184
   //[KOR][ITS][179014][comment](aettie.ji)
   Q_INVOKABLE void setCopyFromMainPlayer(bool val);
   Q_INVOKABLE int RemainedCapacity(); // modified by ravikanth 14-08-13 for ISV 88691
   //added by suilyou 20130925 START
   Q_INVOKABLE void setSharedMemData(int data);
   Q_INVOKABLE int getSharedMemData();
   //added by suilyou 20130925 END
   Q_INVOKABLE void showPopupType(int type, bool isFront);

signals:
   //BottomBar
   void bottomBarModeChanged();
   void currentDeviceChanged();
   void currentFileTypeChanged();
   void loadBottomBar(); // modified by Sergey for boot time

   //PopUp
   void showPopup( int type );
   void hidePopup();
   void hidePopupOnFG(int display); // modified by ravikanth 10-07-13 for ITS 0179181
   void closeCopyProgressPopup(); // modified for ITS 0208724

   //ModeArea
   void editButtonChanged(bool value); //modified for [KOR][ISV][79666][B](aettie.ji)
   void modeAreaTitleChanged();
   void modeAreaFileCountChanged(); //added by yungi 2013.03.08 for New UX FileCount

   //KeyPad
   void showKeyPad();
   void hideKeyPad();

   //Content
   void contentModeChanged();
   void musicFilterChanged();
   void browseOnlyChanged();

   //UsbJukeSwitcher
   void setDevice( int value );

   void selectAllEnabledChanged();
   void deselectAllEnabledChanged(bool value);

   void notifyHideBottomBar();

   void notifyIndexingPopup();
   void menuPressed();

   void copySkipCountChanged(); // modified by ravikanth 22-04-13
   void resetFocusOnEmtyModel();

private slots:
   void initializeCompletedHandler();
   void hidePopupHandler();
   void showPopupHandler();
   void slotShowPopupDeleting();
   void slotDeleteComplete();
   void slotShowPopupAllDeleting();		
	void slotEngineShowPopup(int type, bool isFront);
   void slotOnBg();
   void slotOnFg(int display); // added by wspark 2013.02.03
   void slotOnShowProgressingPopup(); // added by junggil 2012.10.09 for CR14142 & CR14228 & CR14275
   void slotCancelFileOperation(); // added by lssanh 2013.02.07 2USB
   void slotShowCopy(bool m_isCopy, bool unsupportedFile); // addd by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list
   void slotShowBottomBarOnCopy(); // modified by ravikanth 06-04-13
   void startCopy(); // modified by ravikanth 29-06-13 for ITS 0176909
   void startDelete(); // modified by ravikanth 21-07-13 for copy cancel confirm on delete
   void onResetFMLaunchBrowseState(int screen); // modified by ravikanth for ITS 0184642
   void onCancelJukeboxFormat();
   void showCopyCompletePopup(); // modified for ITS 0208724

public slots:
   void bottomBarEventHandler( int event );
   void popupEventHandler( int event );
   void modeAreaEventHandler( int event );
   void eventHandler( int event ); // modified by ravikanth 03-04-13 ITS 159192
   void keyPadEventHandler( int event, const QVariant & data );
   void usbjukeEventHandler( int event );
   void contentEventHandler( int event, const QVariant & data );
   void usbStateHandler();
   void startViewHandler();
   void initStateHandler( int device, int fileType, int musicFilter, int typeMode );
   void musicFileterEventHandler( int event );
   void onReplaceFileRequest(QString fileName, int totalFilesForCopy); // modified by ravikanth 07-07-13 for ITS 0178184
   void onCapacityError(int filesCount, qint64 filesSize);

   void onCopyError(int typeError);
   void onOperationStatusChanged(EOperationStatus status); // commented by Sergey for CR#14494
   void slotUpdateModeArea();
   void slotCurrentFolderChanged();
   void slotOnUsbRemoved(int nPort); // modified by eugene.seo 2013.05.29

   // { added by lssanh 2012.09.17 for CR13482
   void FormattingComleteHandler(int status);
   // } added by lssanh 2012.09.17 for CR13482

   void onCopyOpertaionExit(); // modified by ravikanth 19-04-13
   void slotcancelFileOperationOnInstance(bool isFront); // modified by ravikanth 29-06-13 for ITS 0176909
   void onCopyOperationComplete(int skipCount, int copyFilesCount); // modified by ravikanth 05-07-13 for ITS 0174301
   void clearSharedData();
private:
   int getBottomBarMode();

   int getCurrentDevice();
   void setCurrentDevice( int value , bool switchToRoot = false); // modified by ravikanth 25.06.13 for ITS 0175213, 0175370
   int getCurrentFileType();
   void setCurrentFileType( int value );
   int getContentMode();
   void setContentMode( int value );
   int getMusicFilter();
   void setMusicFilter( int value );
   bool getBrowseOnly();
   void setBrowseOnly( bool value );

   bool getSelectAllEnabled();
   bool getDeselectAllEnabled();
   void setSelectAllEnabled(bool value);
   void setDeselectAllEnabled(bool value);

   bool getHideBottomBar();

   //PopUp
   QString getSelectedFolder();
  
   // { add by wspark 2012.07.12 for CR9616
   // ignore CopyComplete signal when there is CORRUPTED ERROR popup.
   int getCurrentPopupEvent();
   void setCurrentPopupEvent(int value);
   // } add by wspark
   void fileSelectCountReset(); //added by yungi 2013.03.08 for New UX FileCount
   int currentCopySkipCount(); // modified by ravikanth 22-04-13
   int currentCopyTotalCount(); // modified by ravikanth 22-04-13


   //KeyPad
   QString getKeyPadText();

   bool getIndexingPopup();

   bool emptyBoolGetter() { return false; }

   void setMusicChecked( bool value );
   void setVideoChecked( bool value );
   void setPictureChecked( bool value );

   void checkHandler( bool value, UIMainDef::FILE_TYPES type );
   void invertUsbJukeSwitcher();

   void switchBottomBarToInvisibleMode();
   void setPhotoFrame();

   //ModeArea
   bool getEditButton();
   QString getModeAreaTitle();
   void setModeAreaTitle( const QString & value );
   // { added by yungi 2013.03.08 for New UX FileCount
   QString getModeAreaFileCount();
   void setModeAreaFileCount( const QString & value );
   // } added by yungi 2013.03.08 for New UX FileCount
   USBController* getFileInterface(); // added by Dmitry Bykov 04.04.2013
   bool isFileOperationInProgress();
   void DisplayOsdOnFR(QString msg); // modified for ITS 0214738

   FileManagerEngine          *m_pEngine;
   CStateManager              *m_pStateManager;
   UIMainDef::BOTTOM_BAR_MODES m_BottomBarMode;
   UIMainDef::DEVICES          m_Device;
   UIMainDef::FILE_TYPES       m_FileType;

   //ModeArea
   QString          m_ModeAreaTitle;
   QString          m_ModeAreaFileCount; ///added by yungi 2013.03.08 for New UX FileCount
   bool             m_editButtonIsVisible;

   //KeyPad
   bool             m_KeyPadIsDisplayed;

   //Content
   UIMainDef::CONTENT_MODES    m_ContentMode;
   UIMainDef::MUSIC_FILTERS    m_MusicFilter;
   bool             m_BrowseOnly;

   bool             m_MusicIsChecked;
   bool             m_VideoIsChecked;
   bool             m_PictureIsChecked;

   TitleProvider     m_TitleProvider;

   QString           m_CurrentSelectedFolder;
   QString           m_currentFileForContextMenu;
   UIMainDef::BOTTOM_BAR_MODES m_previousBottomBarMode;
   bool m_selectAllEnabled;
   bool m_deselectAllEnabled;

   bool m_IsIndexing;

   // QTimer * m_timer; // deleted by wspark 2012.10.11 for Function_Jukebox_1012
   QTimer * m_formatting_timer;
   QString currentProcessedFile;
   double jukeBoxSize; // modified by ruindmby 2012.08.22 for CR12454
   double usedJukeBoxSize; // modified by ruindmby 2012.08.22 for CR12454
   QScopedPointer<FileManagerMenu> optionMenu;
   int m_filesCount;
   int m_totalFilesSize;
   USBController *fileInterface;
   bool isFileRenamed;

   bool m_isDeleteAllState;
   int m_currentPopupEvent; // add by wspark 2012.07.12 for CR9616.
   bool m_isCopyAll; // add by wspark 2012.07.25 for CR12226.
   UIMainDef::CURRENT_OPERATION m_pCurrentOperation;
   EOperationStatus m_eOperationStatus;
   UIMainDef::FILE_TYPES       m_currentFileType; //added by eunhye 2012.12.20 for No CR
   bool m_fmLaunchInBrowseMode; // addd by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list
   int m_currentSkipCount; // modified by ravikanth 22-04-13
   int m_currentCopyTotalCount; // modified by ravikanth 22-04-13
   SharedDataProvider *m_pSharedVideoData; // modified by ravikanth 27-04-13
   SharedDataProvider *m_pSharedFMData; // modified by ravikanth for ITS 0186965 
   int m_currentReplaceCopyCount; // modified by ravikanth 07-07-13 for ITS 0178184

private:
   void setModelCheckValue(bool value);
   QString getSelectedPictureName();
   void updateModeAreaType();
};
// modified by Dmitry 17.08.13
#endif // DHAVN_APPFILEMANAGER_UI_MAIN_H
