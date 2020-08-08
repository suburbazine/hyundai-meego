#include <DHAVN_AppFileManager_UI_main.h>
//#include <DHAVN_FileOperationsHelper.h> // commented by Sergey for CR#14494
#include "DHAVN_AppFileManager_BaseListModel.h"
#include "DHAVN_AppFileManager_PictureListModel.h"
#include <DHAVN_AppFileManager_CTrackerExtractorClient.h>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
//#include <usbcontroller.h> // commented by Sergey for CR#14494
#include <DHAVN_SharedDataProvider.h> // modified by ravikanth 27-04-13
#include <QmlStatusBar/osdwriter.h>

#define GB_SIZE             (1024LL * 1024LL * 1024LL)
#define JUKEBOX_VOLUME      "/usr_data"

#define MB_SIZE         (1024 * 1024)

const QString jukeboxVideoName = "/usr_data/media/Video"; // modified by Dmitry 22.05.13
const QString jukeboxPictureName = "/usr_data/media/Picture"; // modified by Dmitry 22.05.13

// removed by kihyung 2013.12.09 for ITS 0213549 
//const QString USBFront = QString("/media/front_usb"); // added by eugene.seo 2013.05.29
//const QString USBRear = QString("/media/rear_usb"); // added by eugene.seo 2013.05.29

UI_Main::UI_Main( FileManagerEngine *pEngine, CStateManager *pStateManager )
   : m_pEngine( pEngine )
   , m_pStateManager( pStateManager )
   , m_BottomBarMode( UIMainDef::BOTTOM_BAR_INVISIBLE )
   , m_Device( UIMainDef::DEVICE_USB )
   , m_FileType( UIMainDef::FILE_TYPE_AUDIO )
   , m_ModeAreaTitle( "" )
   , m_editButtonIsVisible( true )
   , m_KeyPadIsDisplayed( false )
   , m_ContentMode( UIMainDef::CONTENT_MODE_UNDEFINED )
   , m_MusicFilter ( UIMainDef::MUSIC_FILTER_ARTIST ) // modified by eugene 2012.08.26 for static analysis defect #142
   , m_BrowseOnly( false )
   , m_MusicIsChecked( false )
   , m_VideoIsChecked( false )
   , m_PictureIsChecked( false )
   , m_CurrentSelectedFolder("") // modified by eugene 2012.08.26 for static analysis defect #142
   , m_currentFileForContextMenu("") // modified by eugene 2012.08.26 for static analysis defect #142
   , m_previousBottomBarMode(UIMainDef::BOTTOM_BAR_INVISIBLE)
   , m_selectAllEnabled(false)
   , m_deselectAllEnabled(false)
   , m_IsIndexing( false )
   , currentProcessedFile("") // modified by eugene 2012.08.26 for static analysis defect #142
   , jukeBoxSize(0)
   , usedJukeBoxSize(0)
   , m_filesCount(0)
   , m_totalFilesSize(0)
   , fileInterface(NULL)
   , isFileRenamed(false)
   , m_isDeleteAllState(false)
   , m_currentPopupEvent(0) // modified by eugene 2012.08.26 for static analysis defect #142
   , m_isCopyAll(false) // modified by eugene 2012.08.26 for static analysis defect #142
   , m_pCurrentOperation(UIMainDef::CURRENT_OPERATION_INVALID)
   , m_eOperationStatus(eOperationNone) // added by Sergey for CR#14494
   , m_currentFileType( UIMainDef::FILE_TYPE_AUDIO ) //added by eunhye 2012.12.20 for No CR
   , m_fmLaunchInBrowseMode(true) // added by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list
   , m_pSharedVideoData(new SharedDataProvider(VIDEOPLAYER_SHARED_MEMORY_KEY)) // added by kihyung 2013.07.15 for STATIC 13126
   , m_pSharedFMData(new SharedDataProvider(FILEMANAGER_SHARED_MEMORY_KEY))
   , m_currentReplaceCopyCount(0) // modified by ravikanth 07-07-13 for ITS 0178184
{
    // FM_LOG_INIT_V2("AppFileManager");
    this->optionMenu.reset(new FileManagerMenu(this));

    // { deleted by wspark 2012.10.11 for Function_Jukebox_1012
    /*
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(3000);
    */
    // } deleted by wspark

    m_formatting_timer = new QTimer(this);
    m_formatting_timer->setSingleShot(true);
    // { modified by lssanh 2012.09.17 for CR13482 
    //m_formatting_timer->setInterval(500);
    m_formatting_timer->setInterval(2000);
    // } modified by lssanh 2012.09.17 for CR13482 

// #if USE_USB_SINGLE_INSTANCE
//       fileInterface = USBController::GetInstance();
// #else
//       fileInterface = USBController::GetFirstInstance();
// #endif



    // connect( m_timer, SIGNAL( timeout()), this, SLOT( slotShowPopupDeleting()) ); // deleted by wspark 2012.10.11 for Function_Jukebox_1012
    connect( m_formatting_timer, SIGNAL( timeout()), this, SLOT( slotShowPopupAllDeleting()) );

   connect( m_pEngine, SIGNAL( startView() ), this, SLOT( startViewHandler() ) );
   connect( m_pEngine, SIGNAL( updateUsbAttached() ), this, SLOT( usbStateHandler() ) );

   connect( m_pStateManager,
            SIGNAL( initState( int, int, int, int ) ),
            this,
            SLOT( initStateHandler( int, int, int, int ) ) );
   connect( m_pStateManager, SIGNAL( initializeCompleted() ),
            this, SLOT(initializeCompletedHandler()));


    connect( m_pStateManager,
             SIGNAL( updateModeArea() ),
             this,
             SLOT( slotUpdateModeArea() ) );

    connect( m_pEngine,
             SIGNAL( hideOnBg() ),
             this,
             SLOT( slotOnBg() ) );
    // { added by wspark 2013.02.03
    connect( m_pEngine,
             SIGNAL( hideOnFg(int) ),
             this,
             SLOT( slotOnFg(int) ) ); // modified by ravikanth 10-07-13 for ITS 0179181
    // } added by wspark
    // { added by lssanh 2013.02.07 2USB
    connect( m_pEngine,
             SIGNAL( cancelFileOperation() ),
             this,
             SLOT( slotCancelFileOperation() ) );	
    // } added by lssanh 2013.02.07 2USB

    connect( m_pStateManager,
             SIGNAL( currentFolderChanged()),
             this,
             SLOT( slotCurrentFolderChanged()) );

    connect( m_pStateManager,
             SIGNAL( usbRemoved(int)), // modified by eugene.seo 2013.05.29
             this,
             SLOT( slotOnUsbRemoved(int)) ); // modified by eugene.seo 2013.05.29

    connect( m_pEngine,
             SIGNAL( cancelFileOperationOnInstance(bool) ),
             this,
             SLOT( slotcancelFileOperationOnInstance(bool) ) ); // modified by ravikanth 29-06-13 for ITS 0176909

    connect( m_pEngine,
             SIGNAL(cancelJukeboxFormat()),
             this,
             SLOT(onCancelJukeboxFormat()) );
	
    // { added by lssanh 2012.09.17 for CR13482
    //connect(fileInterface, SIGNAL(OnFormatFinished(int)), this, SLOT(FormattingComleteHandler(int)));
    // } added by lssanh 2012.09.17 for CR13482

    connect(m_pEngine, SIGNAL(showProgressingPopup()), this, SLOT(slotOnShowProgressingPopup())); // added by junggil 2012.10.09 for CR14142 & CR14228 & CR14275 
	connect(m_pEngine, SIGNAL(showPopup(int, bool)), SLOT(slotEngineShowPopup(int, bool)));
    connect(m_pEngine, SIGNAL(showCopy(bool,bool)), this, SLOT( slotShowCopy(bool,bool) ));  // added by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list
    connect(m_pEngine, SIGNAL(showBottomBarOnCopy()),this, SLOT( slotShowBottomBarOnCopy())); // modified by ravikanth 06-04-13

    connect(m_pEngine, SIGNAL(resetFMLaunchBrowseState(int)),
            this, SLOT(onResetFMLaunchBrowseState(int))); // modified by ravikanth for ITS 0184642
    connect( m_pEngine,
             SIGNAL(ClearSharedData()),
             this,
             SLOT(clearSharedData()));
    setSharedMemData(0); //added by suilyou ITS 0192071
    // m_pSharedVideoData = new SharedDataProvider(VIDEOPLAYER_SHARED_MEMORY_KEY); // modified by ravikanth 27-04-13 // removed by kihyung 2013.07.15 for STATIC 13126
} 

UI_Main::~UI_Main()
{
    delete m_pSharedVideoData;
    delete m_pSharedFMData;
    delete m_formatting_timer;
}

// added by Dmitry Bykov 04.04.2013
USBController* UI_Main::getFileInterface()
{

   if (!fileInterface)
   {
#if USE_USB_SINGLE_INSTANCE
      fileInterface = USBController::GetInstance();
#else
      fileInterface = USBController::GetFirstInstance();
#endif
      connect(fileInterface, SIGNAL(OnFormatFinished(int)), this, SLOT(FormattingComleteHandler(int)));
   }
    return fileInterface;
}
// added by Dmitry Bykov 04.04.2013

TitleProvider * UI_Main::getTitleProvider()
{
   return &m_TitleProvider;
}

int UI_Main::getBottomBarMode()
{
   // FM_LOG << "UI_Main::getBottomBarMode = " << m_BottomBarMode << LOG_ENDL;
   return m_BottomBarMode;
}

void UI_Main::setBottomBarMode( int value )
{
    if(value != UIMainDef::BOTTOM_BAR_INVISIBLE)
        emit loadBottomBar(); // modified by Sergey for boot time

   m_BottomBarMode = (UIMainDef::BOTTOM_BAR_MODES)value;
   emit bottomBarModeChanged();

   switch( m_BottomBarMode )
   {
      case UIMainDef::BOTTOM_BAR_COPY_TO:
      {
         setBrowseOnly( true );
         /*requirement should be clarifyed*/
      }
      break;

      case UIMainDef::BOTTOM_BAR_MOVE_TO:
      {
         setBrowseOnly( true );
      }
      break;

      default:
      {
         setBrowseOnly( false );
      }
   }
}

int UI_Main::getCurrentDevice()
{
   return m_Device;
}

void UI_Main::setCurrentDevice( int value , bool switchToRoot) // modified by ravikanth 25.06.13 for ITS 0175213, 0175370
{
   Q_UNUSED(switchToRoot)
   m_Device = (UIMainDef::DEVICES)value;
   switch ( value )
   {
      case UIMainDef::DEVICE_USB:
      {
// removed by Dmitry 09.08.13
         m_pEngine->setJukeboxDevice(false ,m_pStateManager->isFrontInstance());//modified by edo.lee 2013.06.11
      }
      break;

      case UIMainDef::DEVICE_JUKEBOX:
      {
// removed by Dmitry 09.08.13
         m_pEngine->setJukeboxDevice(true ,m_pStateManager->isFrontInstance());//modified by edo.lee 2013.06.11
      }
      break;
   }
   emit currentDeviceChanged();
}

int UI_Main::getCurrentFileType()
{
   return m_FileType;
}

void UI_Main::setCurrentFileType( int value )
{
   m_FileType = (UIMainDef::FILE_TYPES)value;
   emit currentFileTypeChanged();
}

bool UI_Main::getEditButton()
{
   return m_editButtonIsVisible;
}

void UI_Main::setEditButton( bool value )
{
   m_editButtonIsVisible = value;
   switchBottomBarToInvisibleMode();

   if (value)
   {
       if(!m_pStateManager->getEmptyState()) // added by wspark 2012.12.26 for ITS150949
            this->m_pStateManager->showCountInfo();
   }
   else
   {
       this->m_pStateManager->hideCountInfo();
   }


   updateModeAreaType();

   emit editButtonChanged(value); //modified for [KOR][ISV][79666][B](aettie.ji)
}

QString UI_Main::getModeAreaTitle()
{
   return m_ModeAreaTitle;
}

// { added by yungi 2013.03.08 for New UX FileCount
QString UI_Main::getModeAreaFileCount()
{
   return m_ModeAreaFileCount;
}

void UI_Main::setModeAreaTitle( const QString &value )
{
   m_ModeAreaTitle = value;
   emit modeAreaTitleChanged();
}

void UI_Main::setModeAreaFileCount( const QString &value )
{
   m_ModeAreaFileCount = value;
   emit modeAreaFileCountChanged();
}
// } added by yungi 2013.03.08 for New UX FileCount

int UI_Main::getContentMode()
{
   return m_ContentMode;
}

void UI_Main::setContentMode( int value )
{
   m_ContentMode = (UIMainDef::CONTENT_MODES)value;

   if (m_ContentMode == UIMainDef::CONTENT_MODE_LIST)
   {
      setModeAreaMode(UIMainDef::MODE_AREA_TEXT_WAITING_DISPLAY);
   }
   emit contentModeChanged();
}

int UI_Main::getMusicFilter()
{
   return m_MusicFilter;
}

void UI_Main::setMusicFilter( int value )
{
   m_MusicFilter = (UIMainDef::MUSIC_FILTERS)value;
   emit musicFilterChanged();
}

bool UI_Main::getBrowseOnly()
{
   return m_BrowseOnly;
}

void UI_Main::setBrowseOnly( bool value )
{
   m_BrowseOnly = value;

   if (value && (m_ContentMode == UIMainDef::CONTENT_MODE_FILEMANAGER))
   {
      this->m_pStateManager->browseMode(true);
   }
   else this->m_pStateManager->browseMode(false);

   if (value)
   {
       this->m_pStateManager->hideCountInfo();
   }
   else if (getEditButton())
   {
       if(!m_pStateManager->getEmptyState()) // added by wspark 2012.12.26 for ITS150949
            this->m_pStateManager->showCountInfo();
   }
   
   // modified by ravikanth for ITS 0184642
   // done reset checked values when moving to browse state
   // instead of resetting on operations complete, cancel state 
   if(m_fmLaunchInBrowseMode)
       setModelCheckValue(m_BrowseOnly); // modified by ravikanth 06-04-13

   emit browseOnlyChanged();
}

QString UI_Main::getSelectedFolder()
{
   QFileInfo info( FileManagerEngine::GetValidFilePath(m_CurrentSelectedFolder) );

   // { modified by junggil 2012.09.19 for CR13643 Long image file is displayed outside of change name popup screen. 
   //return info.fileName();
   QFont mFont;
//modified by aettie.ji for ME font 20131015
   mFont.setFamily("DH_HDR");
   // { modified by edo.lee 2012.11.29 New UX
   //mFont.setPixelSize(44);
   mFont.setPointSize(44);
   // } modified by edo.lee

   QFontMetrics fontMetrics(mFont);
   return fontMetrics.elidedText(info.fileName(), Qt::ElideRight, 996);
   // } modified by junggil 
}

QString UI_Main::getKeyPadText()
{
   if ( m_CurrentSelectedFolder.isEmpty() )
   {
      return "";
   }
   else
   {
      return QFileInfo(FileManagerEngine::GetValidFilePath(m_CurrentSelectedFolder)).fileName();
   }
}

bool UI_Main::getIndexingPopup()
{
   return m_IsIndexing;
}


void UI_Main::setMusicChecked( bool value )
{
   m_MusicIsChecked = value;
   checkHandler( value, UIMainDef::FILE_TYPE_AUDIO );
}

void UI_Main::setVideoChecked( bool value )
{
   m_VideoIsChecked = value;
   checkHandler( value, UIMainDef::FILE_TYPE_VIDEO );
}

void UI_Main::setPictureChecked( bool value )
{
   m_PictureIsChecked = value;
   checkHandler( value, UIMainDef::FILE_TYPE_PICTURE );
}

void UI_Main::invertUsbJukeSwitcher()
{
   switch ( m_Device )
   {
      case UIMainDef::DEVICE_USB:
      {
         setCurrentDevice( (int)UIMainDef::DEVICE_JUKEBOX , true); // modified by ravikanth 25.06.13 for ITS 0175213, 0175370
      }
      break;

      case UIMainDef::DEVICE_JUKEBOX:
      {
         setCurrentDevice( (int)UIMainDef::DEVICE_USB , true); // modified by ravikanth 25.06.13 for ITS 0175213, 0175370
      }
      break;
   }
}


void UI_Main::checkHandler( bool value, UIMainDef::FILE_TYPES type )
{
   if ( m_ContentMode == UIMainDef::CONTENT_MODE_FILEMANAGER &&
        m_FileType == type )
   {
      // FM_LOG << "UI_Main::checkHandler  value = " << value << LOG_ENDL;

      if (this->m_BottomBarMode != UIMainDef::BOTTOM_BAR_SELECT_DESELECT)
      {
/*          if ( value )
          {
             if (DEVICE_USB)

             setBottomBarMode( (int)BOTTOM_BAR_COPY_FROM );

             else setBottomBarMode( (int)BOTTOM_BAR_DELETE );
          }
          else*/
          if (this->m_editButtonIsVisible)
          {
             setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
          }

          if ((this->m_BottomBarMode == UIMainDef::BOTTOM_BAR_COPY_FROM) ||
                  (this->m_BottomBarMode == UIMainDef::BOTTOM_BAR_DELETE) ||
                  (this->m_BottomBarMode == UIMainDef::BOTTOM_BAR_MOVE))
          {
              setDeselectAllEnabled(value);
          }
      }
      else
      {
          //setSelectAllEnabled(!value);    // fix coverity
          setDeselectAllEnabled(value);
      }
   }
}

void UI_Main::switchBottomBarToInvisibleMode()
{
   bool isChecked = false;

   switch ( m_FileType )
   {
      case UIMainDef::FILE_TYPE_AUDIO:
      {
         isChecked = m_MusicIsChecked;
      }
      break;

      case UIMainDef::FILE_TYPE_VIDEO:
      {
         isChecked = m_VideoIsChecked;
      }
      break;

      case UIMainDef::FILE_TYPE_PICTURE:
      {
         isChecked = m_PictureIsChecked;
      }
      break;
   }

   if ( isChecked || !this->m_editButtonIsVisible )
   {
      if (getCurrentDevice() == UIMainDef::DEVICE_USB)
      {
         setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_COPY_FROM );
      }
      else
      {
	  // modified by ravikanth 07-08-13 for ITS 0183044
         //setBottomBarMode( (int)BOTTOM_BAR_MOVE );
         setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
      }
   }
   else
   {
      setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
   }
}

void UI_Main::initializeCompletedHandler()
{
   // FM_LOG << "UI_Main::initializeCompletedHandler" << LOG_ENDL;
   connect( m_pEngine->GetTrackerExtractorClient(), // modified by Dmitry 14.08.13 for ITS0183845
            SIGNAL( hideNotifyPopup() ),
            this, SLOT( hidePopupHandler() ));

   connect( m_pEngine->GetTrackerExtractorClient(), // modified by Dmitry 14.08.13 for ITS0183845
            SIGNAL( showNotifyPopup() ),
            this, SLOT( showPopupHandler() ));

   connect( m_pStateManager->GetFileManager(),
           SIGNAL( signalDeleteCompleted() ),
            this, SLOT(slotDeleteComplete()));
   connect( m_pStateManager->GetFileManager(),
           SIGNAL(copyRequest(QString,int)),
           SLOT(onReplaceFileRequest(QString,int))); // modified by ravikanth 07-07-13 for ITS 0178184
   connect( m_pStateManager->GetFileManager(),
           SIGNAL(capacityError(int,qint64)),
           SLOT(onCapacityError(int,qint64)));

   connect( m_pStateManager->GetFileManager(),
              SIGNAL(copyError(int)),
              SLOT(onCopyError(int)));

   connect( m_pStateManager->GetFileManager(),
              SIGNAL(operationStatusChanged(EOperationStatus)),
              SLOT(onOperationStatusChanged(EOperationStatus))); // added by Sergey for CR#14494

   connect( m_pStateManager->GetFileManager(),
              SIGNAL(copyOpertaionExited()),
              SLOT(onCopyOpertaionExit())); // modified by ravikanth 19-04-13

   connect(m_pStateManager->GetFileManager(),
           SIGNAL(operationCopyComplete(int,int)),
           SLOT(onCopyOperationComplete(int,int))); // modified by ravikanth 05-07-13 for ITS 0174301

}

void UI_Main::hidePopupHandler()
{
   // FM_LOG << "UI_Main::hidePopupHandler" << LOG_ENDL;
   m_IsIndexing = false;
   emit notifyIndexingPopup();
}

void UI_Main::showPopupHandler()
{
   // FM_LOG << "UI_Main::showPopupHandler" << LOG_ENDL;
   m_IsIndexing = true;
   emit notifyIndexingPopup();
}


void UI_Main::bottomBarEventHandler( int event )
{
   // FM_LOG << "UI_Main::bottomBarEventHandler  event = " << event << LOG_ENDL;
   setCopyFromMainPlayer(false); // change to normal mode
   setCurrentPopupEvent(-1);
   switch ( event )
   {
      case UIMainDef::BOTTOM_BAR_EVENT_SELECT_AUDIO_FILES:
      {
         // FM_LOG << "bottomBarEventHandler  -->  BOTTOM_BAR_EVENT_SELECT_AUDIO_FILES" << LOG_ENDL;

         setCurrentFileType( (int)UIMainDef::FILE_TYPE_AUDIO );
         if ( m_ContentMode == UIMainDef::CONTENT_MODE_FILEMANAGER )
         {
            m_pStateManager->setFileType(0);
         }
         else if ( m_ContentMode == UIMainDef::CONTENT_MODE_LIST )
         {
            m_pEngine->LaunchMusicList(this->m_pStateManager->isFrontInstance());
         }
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_SELECT_VIDEO_FILES:
      {
         // FM_LOG << "bottomBarEventHandler  -->  BOTTOM_BAR_EVENT_SELECT_VIDEO_FILES" << LOG_ENDL;

         setCurrentFileType( (int)UIMainDef::FILE_TYPE_VIDEO );
         m_pStateManager->setFileType(1);
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_SELECT_PICTURE_FILES:
      {
         // FM_LOG << "bottomBarEventHandler  -->  BOTTOM_BAR_EVENT_SELECT_PICTURE_FILES" << LOG_ENDL;

         setCurrentFileType( (int)UIMainDef::FILE_TYPE_PICTURE );
         m_pStateManager->setFileType(2);
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_COPY:
      {
         m_currentFileType = m_FileType; //added by eunhye 2012.12.20 for No CR
         switch ( m_FileType )
         {
            case UIMainDef::FILE_TYPE_AUDIO:
            {
               showPopup( (int)UIMainDef::POPUP_TYPE_COPYING );
               m_pStateManager->CopySelectedItems();
            }
            break;

            case UIMainDef::FILE_TYPE_VIDEO:
            case UIMainDef::FILE_TYPE_PICTURE:
            {
               setCopyAllStatus(false); // add by wspark 2012.07.25 for CR12226.
               showPopup( (int)UIMainDef::POPUP_TYPE_COPY );
               //showPopup( (int)POPUP_TYPE_COPY_COMPLETE );
            }
            break;
         }
      }
      break;

       case UIMainDef::BOTTOM_BAR_EVENT_COPY_ALL:
       {
	   	// { modified by ravikanth 16-04-13
          if(getCurrentAudioFileOperation())
          {
               // modified by ravikanth 29-06-13 for ITS 0176909, ITS 0184119 
               //setModelCheckValue(true); 
               m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_COPY_ALL;
               emit showPopup(UIMainDef::POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM);
          }
          else
          {
              m_currentFileType = m_FileType; //added by eunhye 2012.12.20 for No CR
              switch ( m_FileType )
              {
              case UIMainDef::FILE_TYPE_VIDEO:
              case UIMainDef::FILE_TYPE_PICTURE:
              {
                  //{ added by yongkyun.lee 20130317 for : ISV 76094
                  //setModelCheckValue(true);
                  //} added by yongkyun.lee 20130317
                  setSelectAllEnabled(true);
                  showPopup( (int)UIMainDef::POPUP_TYPE_COPY_ALL_CONFIRMATION ); // modified by ravikanth for 19.06.13 SMOKE_TC 7 & SANITY_CM_AK347
                  setCopyAllStatus(true); // added by Sergey 28.05.2013
                  m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_COPY_ALL; // added by junggil 2012.10.18 for CR14336 [3PDMS][141287] No display of pop-up "Copy to Jukebox" when rejecting a call
                  //popupEventHandler(POPUP_EVENT_COPY_ALL_CONFIRMATION); // modified by ravikanth for 19.06.13 SMOKE_TC 7 & SANITY_CM_AK347
              }
                  break;
              default:
                  break;
              }
			  // } modified by ravikanth 16-04-13
          }
       }
       break;

      case UIMainDef::BOTTOM_BAR_EVENT_DELETE:
      {
         // modified by ravikanth 26-06-13 for ITS 0175972
         bool isCurrentItemPlaying = false;
         BaseListModel *model = this->m_pStateManager->GetVideoModel();
         if ((model != NULL) && (m_FileType == UIMainDef::FILE_TYPE_VIDEO)) // modified by ravikanth 14-07-13 for ITS 179913
         {
            isCurrentItemPlaying = IsCurrentVideoPlaying() ?
                        model->isElementChecked(model->getPlayedItemIndex()) : false;
         }
         if(!isCurrentItemPlaying)
         {
             // modified by ravikanth 04-07-13 for ISV 86272
             // showPopup( (int)POPUP_TYPE_DELETE );
             // modified by ravikanth 21-07-13 for copy cancel confirm on delete
             m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_DELETE;// added by eunhye 2012.10.18 for Function_Jukebox_1655
             if(getCurrentAudioFileOperation())
             {
                 emit showPopup(UIMainDef::POPUP_TYPE_CANCEL_COPY_FOR_DELETE_CONFIRM);
             }
             else
             {
                 popupEventHandler( UIMainDef::POPUP_EVENT_DELETE_CONFIRMATION );
                 setEditButton(true); // added by Michael.Kim 2014.02.26 for ITS 226811
             }
         }
         else
         {
            emit showPopup( UIMainDef::POPUP_TYPE_FILE_CANNOT_DELETE );
         }
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_DELETE_ALL:
      {
         switch ( m_FileType )
         {
         case UIMainDef::FILE_TYPE_VIDEO:
         case UIMainDef::FILE_TYPE_PICTURE:
         {
            // modified by ravikanth 25-08-13 for ITS 0184119 
            //setModelCheckValue(true);
            // modified by ravikanth 21-07-13 for copy cancel confirm on delete
            m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_DELETE_ALL;// added by eunhye 2012.10.18 for Function_Jukebox_1655
            if(getCurrentAudioFileOperation())
            {
                emit showPopup(UIMainDef::POPUP_TYPE_CANCEL_COPY_FOR_DELETE_CONFIRM);
            }
            else
            {
                setSelectAllEnabled(true);
                showPopup( (int)UIMainDef::POPUP_TYPE_DELETE_ALL );
                //setEditButton(true); removed by Hyejin.Noh 2014.03.19 for ITS 230048 //added by Michael.Kim 2014.02.26 for ITS 226811
            }
         }
         break;
         default:
          break;
         }
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_COPY_HERE:
      {
       m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_COPY;
       if(!getCurrentAudioFileOperation())
       {
           popupEventHandler(UIMainDef::POPUP_EVENT_COPY_ALL_CONFIRMATION);
       }
       else
       {
           emit showPopup(UIMainDef::POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM);
       }
	   // } modified by ravikanth 16-04-13
         // } modified by wspark
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_CREATE_FOLDER:
      {
         m_KeyPadIsDisplayed = true;
         emit showKeyPad();

         setModeAreaMode(UIMainDef::MODE_AREA_TEXT_ENTER_FOLDER_NAME);

         m_TitleProvider.setTitleType( TitleProvider::TITLE_TYPE_KEYPAD );
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_CANCEL:
      {
         m_pStateManager->breakCopyMode();
         switchBottomBarToInvisibleMode();
         //invertUsbJukeSwitcher(); //deleted by yungi 2013.2.18 for UX Scenario 5. File Copy step reduction
         setModelCheckValue(false);
         m_pStateManager->StateChangeUpdate();
         setEditButton(true);
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_SELECT_ALL:
      {
         setModelCheckValue(true);
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_DESELECT_ALL:
      {
         setModelCheckValue(false);
         fileSelectCountReset(); //added by yungi 2013.03.08 for New UX FileCount
         setCopyAllStatus(false); // modified by ravikanth 08-07-13 for reset copy all status
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_MOVE_FILE:
      {
         // m_pStateManager->setMoveMode( true ); // deleted by wspark 2012.10.15 for CR13822
         setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_MOVE_TO );
         m_pStateManager->setMoveMode( true ); // added by wspark 2012.10.15 for CR13822
         m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID; // added by junggil 2012.10.18 for CR14336 [3PDMS][141287] No display of pop-up "Copy to Jukebox" when rejecting a call
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_MOVE_ALL:
      {
         setModelCheckValue(true);
         showPopup( (int)UIMainDef::POPUP_TYPE_MOVE_ALL );
         m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_MOVE_ALL; // added by junggil 2012.10.18 for CR14336 [3PDMS][141287] No display of pop-up "Copy to Jukebox" when rejecting a call
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_MOVE:
      {
         showPopup( (int)UIMainDef::POPUP_TYPE_MOVE );
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_MOVE_HERE:
      {
         m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_MOVE;
         showPopup( (int)UIMainDef::POPUP_TYPE_MOVING );
         m_pStateManager->MoveSelectedItems();
         m_pStateManager->setMoveMode( false );
         setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
         setModelCheckValue(false);
         setEditButton(true);
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_MOVE_CANCEL:
      {
         m_pStateManager->breakMoveMode();
         switchBottomBarToInvisibleMode();
         setModelCheckValue(false);
		 setEditButton(true); // added by wspark 2012.10.05 for sanity AJ447
         m_pStateManager->StateChangeUpdate();
         m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
      }
      break;

      case UIMainDef::BOTTOM_BAR_EVENT_CANCEL_ON_FIRST_LEVEL:
      {
         setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
         setModelCheckValue(false);
         setEditButton(true);
		 // modified by ravikanth for ITS 0184642
         this->m_pStateManager->GetPictureModel()->setFileURLCount(0);
         this->m_pStateManager->GetVideoModel()->setFileURLCount(0);
      }
      break;
   }
}

void UI_Main::popupEventHandler( int event )
{
   switch ( (UIMainDef::POPUP_EVENTS)event )
   {
      case UIMainDef::POPUP_EVENT_COPY_CONFIRMATION:
      {
         //m_pStateManager->setCopyView( true ); // deleted by wspark 2012.10.09 for CR14210
         setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_COPY_TO );
         m_pStateManager->setCopyView( true ); // added by wspark 2012.10.09 for CR14210
         //invertUsbJukeSwitcher(); //deleted by yungi 2013.2.18 for UX Scenario 5. File Copy step reduction
      }
      break;

      case UIMainDef::POPUP_EVENT_COPY_COMPLETED:
      {
         showPopup( (int)UIMainDef::POPUP_TYPE_COPY_COMPLETE );
         m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
         setCopyAllStatus(false); // added by Sergey 28.05.2013
         //m_pEngine->notifyCopyToJukeboxEnd(); // modified by ravikanth 19-04-13

      }
      break;

      case UIMainDef::POPUP_EVENT_DELETE_COMPLETED:
      {
// removed by Dmitry 27.07.13
         showPopup( (int)UIMainDef::POPUP_TYPE_DELETE_COMPLETE );
         m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;// added by eunhye 2012.10.18 for Function_Jukebox_1655
      }
      break;

      case UIMainDef::POPUP_EVENT_COPY_CANCEL:
      {
         this->m_pStateManager->GetFileManager()->pauseOperation();
         emit showPopup(UIMainDef::POPUP_TYPE_COPY_CANCEL);
      }
      break;

      case UIMainDef::POPUP_EVENT_COPY_COMPLETE_POPUP_CLOSED:
      {
         m_pStateManager->setCopyView( false );
         // commented for ITS 213087
         //setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
         //setEditButton(true);
         setCopyAllStatus(false); // added by Sergey 28.05.2013
      }
      break;

      case UIMainDef::POPUP_EVENT_DELETE_CONFIRMATION:
      {
	  		// modified by ravikanth 17-08-13 for ITS 0184747
           if(getCurrentAudioFileOperation())
           {
               emit showPopup(UIMainDef::POPUP_TYPE_CANCEL_COPY_FOR_DELETE_CONFIRM); // modified by Dmitry 17.08.13
           }
           else
           {
               setSelectAllEnabled(false);
		   		// modified by ravikanth 25-08-13 for ITS 0184119 
               if(m_pCurrentOperation == UIMainDef::CURRENT_OPERATION_DELETE_ALL) // added by wspark 2013.03.12 for ISV 73833
                   setModelCheckValue(true);
               slotShowPopupDeleting(); // added by wspark 2012.10.11 for Function_Jukebox_1012
               // m_timer->start(); // deleted by wspark 2012.10.11 for Function_Jukebox_1012
               if ( m_CurrentSelectedFolder.isEmpty() )
               {
                   m_pEngine->setOSDDisplay(m_pStateManager->isFrontInstance()); //modified by edo.lee 2013.05.17
                   m_pStateManager->DeleteSelectedItems();
                   //switchBottomBarToInvisibleMode(); // removed by Sergey 11.04.2013 for ISV#162476
                   setModelCheckValue(false);
               }
               else
               {
                   m_pStateManager->deleteFolder( m_CurrentSelectedFolder );
                   m_CurrentSelectedFolder = "";
               }
           }
      }
      break;

      case UIMainDef::POPUP_EVENT_FORMATTING_JUKEBOX_CONFIRMATION:
      {
	  	 // { added by eugene.seo 2013.05.19
         if(getCurrentAudioFileOperation()) 
         {
		 	m_pEngine->CancelCopyJukebox();
	        m_pEngine->cancelFileCopyOperation();
         }
	  	 // } added by eugene.seo 2013.05.19
         m_formatting_timer->start();
         // { modified by lssanh 2012.09.17 for CR13482
         //StartJukeBoxFormat();
         m_pEngine->notifyFormatJukeboxBegin();
         emit showPopup(UIMainDef::POPUP_TYPE_FORMATTING);
         // } modified by lssanh 2012.09.17 for CR13482		 
      }
      break;

      case UIMainDef::POPUP_EVENT_DELETE_COMPLETE_POPUP_CLOSED:
      {
        if(!getEditButton()) // added by Michael.Kim 2014.02.26 for ITS 226811
            setEditButton(true);
		 // modified for ISV 91108
         BaseListModel* model = NULL;
         switch (this->m_FileType)
         {
         case UIMainDef::FILE_TYPE_VIDEO:
             model = this->m_pStateManager->GetVideoModel();
             break;
         case UIMainDef::FILE_TYPE_PICTURE:
             model = this->m_pStateManager->GetPictureModel();
         }
         if(model != NULL)
         {
         	if(model->getListCount() <= 0)
         	{
             	emit resetFocusOnEmtyModel();
         	}
         }
      }
      break;

      case UIMainDef::POPUP_EVENT_DELETE_CANCEL:
      {
         m_CurrentSelectedFolder = "";
         m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;// added by eunhye 2012.10.18 for Function_Jukebox_1655
      }
      break;

      case UIMainDef::POPUP_EVENT_DELETE_ALL_CANCEL:
      {
         //setModelCheckValue(false); //deleted by yungi 2013.03.29 for ISV 77691
         setSelectAllEnabled(false);
         m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;// added by eunhye 2012.10.18 for Function_Jukebox_1655
      }
      break;
	  
	  // modified by ravikanth 19.06.13 for ITS 0174577
      case UIMainDef::POPUP_EVENT_DELETE_PROGRESS_CANCEL:
      {
       m_CurrentSelectedFolder = "";
       m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
       m_pStateManager->GetFileManager()->cancelOperation();
      }
      break;

     case UIMainDef::POPUP_EVENT_MOVE_ALL_CANCEL:
     {
        setModelCheckValue(false);
        m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
     }
     break;

      case UIMainDef::POPUP_EVENT_NOUSB_OK:
      {
         m_pStateManager->setDevice( 1 );
         setCurrentDevice( (int)UIMainDef::DEVICE_JUKEBOX );
      }
      break;

      case UIMainDef::POPUP_EVENT_FILE_OPERATION_RENAME:
      {
          // FM_LOG << "m_currentFileForContextMenu " << m_currentFileForContextMenu << LOG_ENDL;
          // FM_LOG << "m_pStateManager->GetCurrentPlayName() " << m_pStateManager->GetCurrentPlayName() << LOG_ENDL;
          QString playpath;
          playpath.append(m_pStateManager->GetCurrentPlayFolder()).append("/").append(m_pStateManager->GetCurrentPlayName());
          // FM_LOG << "playpath " << playpath << LOG_ENDL;
          if (m_currentFileForContextMenu != playpath)
          {
             this->isFileRenamed = true;
             m_KeyPadIsDisplayed = true;
             emit showKeyPad();
             setModeAreaMode(UIMainDef::MODE_AREA_TEXT_RENAME);
          }
          else
          {
             // FM_LOG << "File is in use, can't rename" << LOG_ENDL;
             emit showPopup((int)UIMainDef::POPUP_TYPE_FILE_IS_USED);
          }
      }
      break;

      case UIMainDef::POPUP_EVENT_FOLDER_OPERATION_RENAME:
      {
          // FM_LOG << "m_CurrentSelectedFolder " << m_CurrentSelectedFolder << LOG_ENDL;
          // FM_LOG << "m_pStateManager->GetCurrentPlayFolder() " << m_pStateManager->GetCurrentPlayFolder() << LOG_ENDL;
          if (m_CurrentSelectedFolder != m_pStateManager->GetCurrentPlayFolder())
          {
             this->isFileRenamed = false;
             m_KeyPadIsDisplayed = true;
             setModeAreaMode(UIMainDef::MODE_AREA_TEXT_RENAME);
             emit showKeyPad();
          }
          else
          {
             // FM_LOG << "Folder is in use, can't rename" << LOG_ENDL;
             emit showPopup((int)UIMainDef::POPUP_TYPE_FOLDER_IS_USED);
          }
      }
      break;

      case UIMainDef::POPUP_EVENT_MOVE_CANCEL:
      {
         this->m_pStateManager->GetFileManager()->pauseOperation();
         emit showPopup(UIMainDef::POPUP_TYPE_MOVE_CANCEL);
         // modified by minho 20121016
         // for CR14271: [3PDMS][142453] When Power Off/On from the pop-up of Cancel Copying files on Jukebox while playing USB Video, the pop-up disappeared
         //m_pCurrentOperation = CURRENT_OPERATION_INVALID;
         m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_MOVE_CANCEL;
         // modified by minho
      }
      break;

      case UIMainDef::POPUP_EVENT_MOVE_COMPLETED:
      {         
         showPopup( (int)UIMainDef::POPUP_TYPE_MOVE_COMPLETE );
         m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
      }
      break;

      case UIMainDef::POPUP_EVENT_FILE_OPERATION_DELETE:
      {
         emit showPopup( UIMainDef::POPUP_TYPE_DELETE );
      }
      break;

      case UIMainDef::POPUP_EVENT_FOLDER_OPERATION_DELETE:
      {
         emit showPopup( UIMainDef::POPUP_TYPE_DELETE_FOLDER );
      }
      break;

      case UIMainDef::POPUP_EVENT_FILE_OPERATION_CANCEL:
      {
         m_CurrentSelectedFolder = "";
      }
      break;

      case UIMainDef::POPUP_EVENT_SAVE_FRAME_CANCEL:
      {
          setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
          setEditButton(true);

          BaseListModel *model = this->m_pStateManager->GetPictureModel();
          if (model != NULL)
          {
              model->clearCheckedElements();
          }

          this->m_CurrentSelectedFolder = "";
      }
      break;

      case UIMainDef::POPUP_EVENT_SAVE_FRAME_JUKEBOX:
      {
         m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_SAVE_FRAME; //added by aettie 201300613 for ITS 173605
         setPhotoFrame();
      }
      break;

      case UIMainDef::POPUP_EVENT_SAVE_FRAME_COMPLETED:
      {
          m_pStateManager->updateState();
          setEditButton(true);
          m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID; // modified by ravikanth 05-08-13 for ITS 0179420
          emit showPopup((int) UIMainDef::POPUP_TYPE_FRAME_SAVED);
      }
      break;

      case UIMainDef::POPUP_EVENT_EDIT_MODE_OK:
      {
         setEditButton(false);
      }
      break;

      case UIMainDef::POPUP_EVENT_SAVE_FRAME_REQUEST:
      {
         emit showPopup( (int)UIMainDef::POPUP_TYPE_SET_AS_FRAME );
      }
      break;

      case UIMainDef::POPUP_EVENT_CREATE_FOLDER_EMPTY_NAME:
      {
       if( isFileRenamed )
       {
           emit showPopup((int)UIMainDef::POPUP_TYPE_EMPTY_FILE_NAME);
       }
       else
       {
           emit showPopup((int)UIMainDef::POPUP_TYPE_EMPTY_FOLDER_NAME);
       }
      }
      break;

      case UIMainDef::POPUP_EVENT_CREATE_FOLDER_NAME_IS_TOO_LONG:
      {
         if( isFileRenamed )
         {
             emit showPopup((int)UIMainDef::POPUP_TYPE_FILE_NAME_IS_TOO_LONG);
         }
         else
         {
             emit showPopup((int)UIMainDef::POPUP_TYPE_FOLDER_NAME_IS_TOO_LONG);
         }
      }
      break;

      case UIMainDef::POPUP_EVENT_CREATE_FOLDER_INCORRECT_CHARACTER:
      {
         emit showPopup((int)UIMainDef::POPUP_TYPE_INCORRECT_CHARACTER);
      }
      break;

      case UIMainDef::POPUP_EVENT_CREATE_FOLDER_FOLDER_ALREADY_EXIST:
      {
         if( isFileRenamed )
         {
            emit showPopup((int)UIMainDef::POPUP_TYPE_FILE_ALREADY_EXIST);
         }
         else
         {
             emit showPopup((int)UIMainDef::POPUP_TYPE_FOLDER_ALREADY_EXIST);
         }
      }
      break;

      case UIMainDef::POPUP_EVENT_REPLACE:
      {
         switch (m_pCurrentOperation)
         {
            case UIMainDef::CURRENT_OPERATION_COPY: emit showPopup( (int)UIMainDef::POPUP_TYPE_COPYING );
               break;

            case UIMainDef::CURRENT_OPERATION_MOVE: emit showPopup( (int)UIMainDef::POPUP_TYPE_MOVING );
               break;

            case UIMainDef::CURRENT_OPERATION_SAVE_FRAME: emit showPopup( (int)UIMainDef::POPUP_TYPE_FRAME_SAVED ); //added by aettie 201300613 for ITS 173605
               break;
               
            default:
               // FM_LOG<<"Invalid operation type"  << LOG_ENDL;
               break;
         }
		 
		 // modified by ravikanth 10-07-13 for ITS 179182	
         if(m_pCurrentOperation == UIMainDef::CURRENT_OPERATION_SAVE_FRAME)
             this->m_pStateManager->CopyFileToMyFrame(m_currentFileForContextMenu,true);
         else
             this->m_pStateManager->GetFileManager()->setOverwriteMode(
                     OVERWRITE_ONE_EXIST_FILE_MODE);
      }
      break;

       case UIMainDef::POPUP_EVENT_REPLACE_ALL:
       {
         switch (m_pCurrentOperation)
         {
            case UIMainDef::CURRENT_OPERATION_COPY: emit showPopup( (int)UIMainDef::POPUP_TYPE_COPYING );
               break;

            case UIMainDef::CURRENT_OPERATION_MOVE: emit showPopup( (int)UIMainDef::POPUP_TYPE_MOVING );
               break;

            default:
               // FM_LOG<<"Invalid operation type"  << LOG_ENDL;
               break;
         }
          this->m_pStateManager->GetFileManager()->setOverwriteMode(
                    OVERWRITE_ALL_EXIST_FILE_MODE);
       }
       break;

       case UIMainDef::POPUP_EVENT_NO_REPLACE:
       {
       if(this->m_pStateManager->getCurrentCopyCount() == 1)
       {
           m_pStateManager->GetFileManager()->cancelOperation();

           switch (m_pCurrentOperation)
           {
              case UIMainDef::CURRENT_OPERATION_COPY: emit showPopup( (int)UIMainDef::POPUP_TYPE_COPY_CANCELLED );
                 break;

              case UIMainDef::CURRENT_OPERATION_MOVE: emit showPopup( (int)UIMainDef::POPUP_TYPE_MOVE_CANCELLED );
                 break;

              default:
                 // FM_LOG<<"Invalid operation type"  << LOG_ENDL;
                 break;
           }

           this->m_pStateManager->GetFileManager()->setOverwriteMode(
                     CANCEL_COPY_OPERATION_MODE);

           m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
       }
       else
       {
           switch (m_pCurrentOperation)
           {
              case UIMainDef::CURRENT_OPERATION_COPY: emit showPopup( (int)UIMainDef::POPUP_TYPE_COPYING );
                 break;

              case UIMainDef::CURRENT_OPERATION_MOVE: emit showPopup( (int)UIMainDef::POPUP_TYPE_MOVING );
                 break;

              default:
                 // FM_LOG<<"Invalid operation type"  << LOG_ENDL;
                 break;
           }

          this->m_pStateManager->GetFileManager()->setOverwriteMode(
                    NOT_OVERWRITE_EXIST_FILE_MODE);
       }
                  // } modified by wspark
       }
       break;

       case UIMainDef::POPUP_EVENT_REPLACE_CANCEL:
       {
          // modified by ravikanth 18.06.13 for ITS 0174610

          // {modified Michael.Kim 2013.11.22 for ITS 210478
          //this->m_pStateManager->GetFileManager()->pauseOperation();
          //    // modified by ravikanth 07-07-13 for ITS 0178185
          //m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_COPY_REPLACE_CANCEL;
          //emit showPopup(UIMainDef::POPUP_TYPE_COPY_CANCEL);
		  // {Modified Michael.Kim 2013.11.22 for ITS 210478
       }
       break;

       case UIMainDef::POPUP_EVENT_CAPACITY_ERROR_CONFIRM:
       {
           // modified for ITS 0212102
           // m_pStateManager->setDevice(eUsb); // modified by Dmitry 21.08.13
           setCurrentDevice(UIMainDef::DEVICE_USB);
           // { added by wspark 2012.10.04 for sanity AK349
           m_pStateManager->setCopyView( false );
           m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;

       }
	   break; // modified by eugene 2012.08.23 for static analysis defect #7086  

       case UIMainDef::POPUP_EVENT_JUKEBOX:
       {
           m_pStateManager->GetFileManager()->cancelOperation();// added by yongkyun.lee 20130314 for : ITS 158346
           m_pStateManager->setCopyView( false );
           setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
           setEditButton(true);
           invertUsbJukeSwitcher();// added by yongkyun.lee 20130314 for : ITS 158346
       }
       break;

       case UIMainDef::POPUP_EVENT_COPY_CONTINUE:
       {
           // modified by ravikanth 07-07-13 for ITS 0178185
           if(m_pCurrentOperation == UIMainDef::CURRENT_OPERATION_COPY_REPLACE_CANCEL)
           {
               m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_COPY;
               emit showPopup( UIMainDef::POPUP_TYPE_REPLACE_FILE );
           }
           else
           {
               if(isFileOperationInProgress()) // modified for ITS 210799
               {
                   m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_COPY;
                   emit showPopup(UIMainDef::POPUP_TYPE_COPYING);
                   m_pStateManager->GetFileManager()->continueOperation();
               }
               else
               {
                   emit showPopup(UIMainDef::POPUP_TYPE_COPY_CANCELLED);
               }
           }
       }
       break;

       case UIMainDef::POPUP_EVENT_COPY_CANCEL_CONFIRM:
       {
           m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_COPY_CANCEL;
           m_pEngine->cancelFileCopyOperation(); // modified by ravikanth 16-04-13
           setCopyAllStatus(false); // added by Sergey 28.05.2013
       }
       break;

       case UIMainDef::POPUP_EVENT_MOVE_CANCEL_CONFIRM:
       {
          m_pStateManager->GetFileManager()->cancelOperation();
          emit showPopup((int) UIMainDef::POPUP_TYPE_MOVE_CANCELLED);
       }
       break;

       case UIMainDef::POPUP_EVENT_MOVE_CANCELED_POPUP_CLOSED:
       {
          m_pStateManager->breakMoveMode();
          m_pStateManager->StateChangeUpdate();
          switchBottomBarToInvisibleMode();
          setModelCheckValue(false);
          setEditButton(true);
          m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
       }
       break;

       case UIMainDef::POPUP_EVENT_MOVE_CONTINUE:
       {
          m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_MOVE;
          showPopup( (int)UIMainDef::POPUP_TYPE_MOVING );
          m_pStateManager->GetFileManager()->continueOperation();
       }
       break;

       case UIMainDef::POPUP_EVENT_COPY_CANCELED_POPUP_CLOSED:
       {
           if(m_fmLaunchInBrowseMode || m_pCurrentOperation == UIMainDef::CURRENT_OPERATION_COPY_CANCEL)
           {
               m_pStateManager->setCopyView( false );
               if(m_pCurrentOperation != UIMainDef::CURRENT_OPERATION_COPY_CANCEL_FOR_COPY)
               {
                   setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
                   setEditButton(true);
               }
           }
           m_pStateManager->breakCopyMode(); // modified by ravikanth 29-08-13 for ITS 0187314
           m_fmLaunchInBrowseMode = true;
           m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
       }
       break;

       case UIMainDef::POPUP_EVENT_COPY_ALL_CONFIRMATION:
       {
	   // modified by ravikanth 04-07-13 for copy confirm when popup is already launched for copy all
           if(!getCurrentAudioFileOperation())
           {
               switch ( m_FileType )
               {
               case UIMainDef::FILE_TYPE_VIDEO:
               case UIMainDef::FILE_TYPE_PICTURE:
               {
                   m_pSharedFMData->SetStatusDateByIndex(DATA_TYPE_FILEOPERATION_STATUS, FILEOPERATION_STATUS_COPY_PROGRESS);
                   setSelectAllEnabled(false);
                   if(m_pCurrentOperation == UIMainDef::CURRENT_OPERATION_COPY_ALL) // added by wspark 2013.03.12 for ISV 73833
                       setModelCheckValue(true);
                   //{modified by yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
                   //showPopup( (int)POPUP_TYPE_COPY );
                   showPopup( (int)UIMainDef::POPUP_TYPE_COPYING );
                   //m_pCurrentOperation = CURRENT_OPERATION_INVALID; // added by junggil 2012.10.18 for CR14336 [3PDMS][141287] No display of pop-up "Copy to Jukebox" when rejecting a call
                   m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_COPY;
                   m_pStateManager->setCopyView( true );
                   m_pStateManager->CopySelectedItems();

                   setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
                   //setModelCheckValue(false);// deleted by yongkyun.lee 20130317 for : ISV 76094
                   setEditButton(true);
                   //}modified by yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
                   // { modified by eugene.seo 2013.05.29
                   if(m_FileType == UIMainDef::FILE_TYPE_VIDEO)
                   {
                       m_pEngine->notifyCopyToJukeboxBegin(m_pStateManager->isFrontInstance(), true);
                   }
                   else
                   {
                       m_pEngine->notifyCopyToJukeboxBegin(m_pStateManager->isFrontInstance(), false);
                   }
                   // } modified by eugene.seo 2013.05.29
               }
               break;

               default:
                   break;
               }
           }
           else
           {
               emit showPopup(UIMainDef::POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM);
           }
       }
       break;

	   // Added by sangmin.seol 2013.10.22 ITS 0195517 for reset sellect-all when copy confirm popup canceled
       case UIMainDef::POPUP_EVENT_CANCEL_COPY_ALL_FOR_COPY_CONFIRM:
       {
	       // Do Noting.. not copy all state...
           if(m_isCopyAll)
               setCopyAllStatus(false);
           if(m_selectAllEnabled)
               setSelectAllEnabled(false);
       }
       break;

       case UIMainDef::POPUP_EVENT_COPY_ALL_CANCEL_CONFIRMATION:
       {
          //add here something if needed
          setCopyAllStatus(false);
          setSelectAllEnabled(false);
          m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID; // added by junggil 2012.10.18 for CR14336 [3PDMS][141287] No display of pop-up "Copy to Jukebox" when rejecting a call
       }
       break;
		// { modified by ravikanth 16-04-13
       case UIMainDef::POPUP_EVENT_COPY_TO_JUKEBOX_CONFIRM:
       {
            emit showPopup( (int)UIMainDef::POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM );
       }
       break;
       case UIMainDef::POPUP_EVENT_JUKEBOX_COPY_CANCEL_CONFIRM:
       {
	   // modified by ravikanth 29-06-13 for ITS 0176909
           if(!getEditButton())
           {
                //setModelCheckValue(false);
                m_pEngine->CancelCopyJukebox();
                // m_pStateManager->GetFileManager()->cancelOperation();
                m_pEngine->cancelCopyOnInstance(m_pStateManager->isFrontInstance());
                // until copy is cancelled properly, immediate copy is not processed by controller. Wait till copy cancel is processed
                QTimer::singleShot(200, this, SLOT(startCopy()));
           }
           else
           {
               m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_COPY_CANCEL_FOR_COPY; // modified by ravikanth 16-07-13 for ISV 87550
               m_pEngine->CancelCopyJukebox();
               m_pEngine->cancelFileCopyOperation();
               setEditButton(false);
           }
       }
       break;
       // } modified by ravikanth 16-04-13
       // { added by eugene.seo 2013.06.10
       case UIMainDef::POPUP_EVENT_COPY_PROGRESS_CLOSE:
       {
           m_pStateManager->setCopyView( false );
       }
       break;
       // } added by eugene.seo 2013.06.10
       // modified by ravikanth 21.06.13 for ITS 0174571
       case UIMainDef::POPUP_EVENT_DELETE_INCOMPLETE:
       {
          showPopup( (int)UIMainDef::POPUP_TYPE_DELETE_INCOMPLETE );
          m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
       }
       break;
	   // modified by ravikanth 05-07-13 for ITS 0178514
       case UIMainDef::POPUP_EVENT_FRAME_REPLACE_CANCEL:
       {
          m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
          m_pStateManager->GetFileManager()->cancelOperation();
       }
       break;
	   // modified by ravikanth 21-07-13 for copy cancel confirm on delete
       case UIMainDef::POPUP_EVENT_CANCEL_COPY_FOR_DELETE_CONFIRM:
       {
           m_pEngine->CancelCopyJukebox();
           m_pEngine->cancelFileCopyOperation();
           // until copy is cancelled properly, immediate fileoperation goes to wait state in controller. Wait till copy cancel is processed
           if ( m_pCurrentOperation != UIMainDef::CURRENT_OPERATION_FORMAT_JUKEBOX)
           {
		   		// modified for ITS 0189254
                //setModelCheckValue(false);
                startDelete();
                //QTimer::singleShot(200, this, SLOT(startDelete()));
           }
           else
           {
                contextDeleteAllId();
           }
       }
       break;
       case UIMainDef::POPUP_EVENT_CLEAR_STATE_TO_NONE:
       {
           m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
       }
       break;
       default:
       {
          // FM_LOG << "AppFileManager::popupEventHandler:  incorrect event" << LOG_ENDL;
       }
       break;
   }
}

void UI_Main::modeAreaEventHandler( int event )
{
   switch ( (UIMainDef::MODE_AREA_EVENTS)event )
   {
      case UIMainDef::MODE_AREA_EVENT_BACK_PRESSED:
      {
         setCopyFromMainPlayer(false); // modified on 05-09-13 for ITS 0188175
         if ( m_KeyPadIsDisplayed )
         {
            m_KeyPadIsDisplayed = false;
            hideKeyPad();
            m_CurrentSelectedFolder = "";
            m_TitleProvider.setTitleType( TitleProvider::TITLE_TYPE_CURRENT_PATH );
         }
         else if ( m_pStateManager->ShouldChangePath() && m_pStateManager->FolderUp() )
         {
            //for some operations
         }
         else if ( m_pStateManager->IsMoveMode() )
         {
            m_pStateManager->breakMoveMode();
            switchBottomBarToInvisibleMode();
         }
         else if ( getBottomBarMode() == UIMainDef::BOTTOM_BAR_COPY_TO )
         {
            //invertUsbJukeSwitcher(); //deleted by yungi 2013.2.18 for UX Scenario 5. File Copy step reduction
            m_pStateManager->StateChangeUpdate();
            setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_COPY_FROM );
            m_pStateManager->changeState();  // m_pStateManager->setCopyView( false );
         }
         else if ( !getEditButton() )
         {
            m_pStateManager->UncheckAllItems();
            setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
            setEditButton(true);
         }
         else
         {
            if ( !m_pStateManager->FolderUp() )
            {
               m_pStateManager->breakMoveMode();
               // { modified by wspark 2012.12.13 for CR16396
               //m_pEngine->HandleBackKey(this->m_pStateManager->isFrontInstance());
               if(m_pStateManager->getEmptyState() == true)
               {
                   m_pEngine->HandleHomeKey(this->m_pStateManager->isFrontInstance());
               }
               else
               {
                   m_pEngine->HandleBackKey(this->m_pStateManager->isFrontInstance());
               }
               // modified by wspark
               setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
            }
         }
      }
      break;

      case UIMainDef::MODE_AREA_EVENT_EDIT_PRESSED:
      {
         emit showPopup( (int)UIMainDef::POPUP_TYPE_EDIT_MODE );
      }
      break;

      default:
      {
         // FM_LOG << "AppFileManager::modeAreaEventHandler:  incorrect event" << LOG_ENDL;
      }
   }

   updateModeAreaType();
   m_pStateManager->NavigatorUpdate(m_pStateManager->GetCurrentPresentPath());
}

// { modified by ravikanth 03-04-13 ITS 159192
void UI_Main::eventHandler( int event )
{
    switch ( (UIMainDef::MODE_AREA_EVENTS)event )
    {
        case UIMainDef::MODE_AREA_EVENT_BACK_PRESSED:
        {
            m_pStateManager->breakMoveMode();
            m_pEngine->HandleBackKey(this->m_pStateManager->isFrontInstance());
            setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
        }
            break;
        default:
            break;
    }
}
// } modified by ravikanth 03-04-13 ITS 159192

void UI_Main::keyPadEventHandler( int event, const QVariant & data )
{
   switch ( (UIMainDef::KEYPAD_EVENTS)event )
   {
      case UIMainDef::KEYPAD_EVENT_TEXT_ENERED:
      {
         if ( m_CurrentSelectedFolder.isEmpty() )
         {
            m_pStateManager->CreateFolder( data.toString() );
            m_TitleProvider.setTitleType( TitleProvider::TITLE_TYPE_CURRENT_PATH );
         }
         else
         {
            QFileInfo fileInfo(FileManagerEngine::GetValidFilePath(m_CurrentSelectedFolder));
            QFileInfo dst( QDir( fileInfo.absolutePath() ), data.toString() );

            if (this->isFileRenamed)
            {
                m_pStateManager->renameFile( FileManagerEngine::GetValidFilePath( m_CurrentSelectedFolder ),
                                               dst.absoluteFilePath() );
            }
            else
            {
                m_pStateManager->renameFolder( FileManagerEngine::GetValidFilePath( m_CurrentSelectedFolder ),
                                               dst.absoluteFilePath() );
            }
            m_CurrentSelectedFolder = "";
         }

         m_KeyPadIsDisplayed = false;
         updateModeAreaType();
      }
      break;

      default:
      {
         // FM_LOG << "AppFileManager::keyPadEventHandler: incorrect event" << LOG_ENDL;
      }
   }
}

void UI_Main::usbjukeEventHandler( int event )
{
   switch ( (UIMainDef::DEVICES)event )
   {
      case UIMainDef::DEVICE_USB:
      {
         if ( !m_pEngine->getUsbAttached() )
         {
            showPopup( (int)UIMainDef::POPUP_TYPE_NOUSB );
         }

         m_pStateManager->setDevice( 0 );
         setCurrentDevice( (int)UIMainDef::DEVICE_USB );
      }
      break;

      case UIMainDef::DEVICE_JUKEBOX:
      {
         m_pStateManager->setDevice( 1 );
         setCurrentDevice( (int)UIMainDef::DEVICE_JUKEBOX );
      }
      break;

      default:
      {
         // FM_LOG << "AppFileManager::usbjukeEventHandler: incorrect device" << LOG_ENDL;
      }
   }

   m_pStateManager->setCopyView( false );
   setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
}

void UI_Main::contentEventHandler( int event, const QVariant & data )
{
   switch ( event )
   {
   // modified by ravikanth 25-07-13 for copy spec changes
      case UIMainDef::CONTENT_EVENT_FOLDER_OPERATION:
      {
         m_CurrentSelectedFolder = data.toString();
         if(getCurrentDevice() != UIMainDef::DEVICE_USB)
             popupEventHandler( UIMainDef::POPUP_EVENT_FOLDER_OPERATION_DELETE );
         //emit showPopup( POPUP_TYPE_FOLDER_OPERATION );
      }
      break;

      case UIMainDef::CONTENT_EVENT_FILE_OPERATION:
      {
         m_CurrentSelectedFolder = data.toString();
         m_currentFileForContextMenu = data.toString();
         // { modified by ravikanth 20-08-13
         if(getCurrentDevice() != UIMainDef::DEVICE_USB)
         {
             if (getCurrentFileType() == UIMainDef::FILE_TYPE_VIDEO
                     || getCurrentFileType() == UIMainDef::FILE_TYPE_PICTURE)
             {
                 popupEventHandler( UIMainDef::POPUP_EVENT_FILE_OPERATION_DELETE );
                 //emit showPopup( UIMainDef::POPUP_TYPE_FILE_VIDEO_JUKEBOX_OPERATION );
             }
         }
		 // } modified by ravikanth 08-04-13
      }
      break;
	  // { modified by ravikanth 27-04-13
       case UIMainDef::CONTENT_EVENT_FILE_BLOCK_DELETE_OPERATION:
       {
           emit showPopup( UIMainDef::POPUP_TYPE_FILE_CANNOT_DELETE );
       }
       break;
	   // } modified by ravikanth 27-04-13
      default:
      {
         // FM_LOG << "AppFilemanager::contentEventHandler: incorrect event" << LOG_ENDL;
      }
   }
}

void UI_Main::usbStateHandler()
{

}

void UI_Main::startViewHandler()
{
   // FM_LOG << "UI_Main::startViewHandler" << LOG_ENDL;
   showPopup( (int)UIMainDef::POPUP_TYPE_START );
}

void UI_Main::initStateHandler( int device, int fileType, int musicFilter, int typeMode )
{
   // FM_LOG << "UI_Main::initStateHandler device [ " << device << " ] fileType [ " << fileType << " ] musicFilter [ " << musicFilter << " ] typeMode [ " << typeMode << " ]" << LOG_ENDL;

   switch ( device )
   {
      case 0: setCurrentDevice( (int)UIMainDef::DEVICE_USB ); break;
      case 1: setCurrentDevice( (int)UIMainDef::DEVICE_JUKEBOX ); break;
      default:
      {
         // FM_LOG << "UI_Main::initStateHandler  incorrect device" << LOG_ENDL;
      }
   }

   switch ( fileType )
   {
      case 0: setCurrentFileType( (int)UIMainDef::FILE_TYPE_AUDIO ); break;
      case 1: setCurrentFileType( (int)UIMainDef::FILE_TYPE_VIDEO ); break;
      case 2: setCurrentFileType( (int)UIMainDef::FILE_TYPE_PICTURE ); break;
      default:
      {
         // FM_LOG << "UI_Main::initStateHandler  incorrect fileType" << LOG_ENDL;
      }
   }

   switch ( musicFilter )
   {
      case 0: setMusicFilter( (int)UIMainDef::MUSIC_FILTER_ARTIST ); break;
      case 1: setMusicFilter( (int)UIMainDef::MUSIC_FILTER_ALBUM ); break;
      case 2: setMusicFilter( (int)UIMainDef::MUSIC_FILTER_SONG ); break;
      default:
      {
         // FM_LOG << "UI_Main::initStateHandler  incrrect musicFilter" << LOG_ENDL;
      }
   }

   switch( typeMode )
   {
      case 0: setContentMode( (int)UIMainDef::CONTENT_MODE_FILEMANAGER ); break;
      case 1: setContentMode( (int)UIMainDef::CONTENT_MODE_LIST ); break;

      default:
      {
         // FM_LOG << "UI_Main::initStateHandler  incorrect typeMode" << LOG_ENDL;
      }
   }

   m_TitleProvider.setCurrentPlayedFile( m_pStateManager->GetCurrentPlayName() );

   m_CurrentSelectedFolder = "";
   // modified by ravikanth for ITS 0184642, ITS 0183044
   if(m_fmLaunchInBrowseMode)
    setModelCheckValue(false);
   setEditButton(m_fmLaunchInBrowseMode); // modified by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list
   //setEditButton(true);

   emit notifyHideBottomBar();
}

void UI_Main::musicFileterEventHandler( int event )
{
   switch ( event )
   {
      case UIMainDef::MUSIC_FILTER_ARTIST:
      {
         m_pStateManager->setMusicFilter( 0 );
      }
      break;

      case UIMainDef::MUSIC_FILTER_ALBUM:
      {
         m_pStateManager->setMusicFilter( 1 );
      }
      break;

      case UIMainDef::MUSIC_FILTER_SONG:
      {
         m_pStateManager->setMusicFilter( 2 );
      }
      break;

      default:
      {
         // FM_LOG << "UI_Main::musicFileterEventHandler  incorrect event" << LOG_ENDL;
      }
   }
}

void UI_Main::handleMenuKey()
{
    // FM_LOG << "UI_Main::handleMenuKey" << LOG_ENDL;

    EAppFileManagerState state = this->m_pStateManager->GetCurrentState();
    switch (state)
    {
      case   eJukeVideoMove:
      case   eJukePictureMove:
       this->optionMenu->setMenuContext(FileManagerMenu::moveContext);
       break;
    case eJukeVideo:
    case eJukePictureShow:
       switch(m_BottomBarMode)
       {
          case UIMainDef::BOTTOM_BAR_MOVE:
          this->optionMenu->setMenuContext(FileManagerMenu::moveContext);
          break;

          case UIMainDef::BOTTOM_BAR_DELETE:
          this->optionMenu->setMenuContext(FileManagerMenu::deleteContext);
          break;

          case UIMainDef::BOTTOM_BAR_MOVE_TO:
          this->optionMenu->setMenuContext(FileManagerMenu::invalidContext);
          break;

          default:
          this->optionMenu->setMenuContext(FileManagerMenu::jukeboxContext);
          break;
       }
       break;
    case eUsbVideo:
    case eUsbPictureShow:
        if (getEditButton())
        {
            this->optionMenu->setMenuContext(FileManagerMenu::usbContext);
        }
        else
        {
            this->optionMenu->setMenuContext(FileManagerMenu::copyContext);
        }
        break;
//    case eJukeVideoMove:
//    case eJukePictureMove:
//       // this->optionMenu->setMenuContext(FileManagerMenu::moveContext);
//        break;
    default:
        // FM_LOG << "UI_Main::handleMenuKey no context menu for state" <<
        //            state << LOG_ENDL;
        return;
    }

    emit menuPressed();
}

void UI_Main::setModelCheckValue(bool value)
{
    // FM_LOG << "UI_Main::setModelCheckValue set items to" << value << LOG_ENDL;
    BaseListModel* model = NULL;

    switch (this->m_FileType)
    {
    case UIMainDef::FILE_TYPE_AUDIO:
        model = this->m_pStateManager->GetAudioModel();
        break;
    case UIMainDef::FILE_TYPE_VIDEO:
        model = this->m_pStateManager->GetVideoModel();
        break;
    case UIMainDef::FILE_TYPE_PICTURE:
        model = this->m_pStateManager->GetPictureModel();
        break;
    }

    if (model != NULL)
    {
        switch (this->m_FileType)
        {
            case UIMainDef::FILE_TYPE_AUDIO:
            {
                m_MusicIsChecked = value;
            }
            break;

            case UIMainDef::FILE_TYPE_VIDEO:
            {
                m_VideoIsChecked = value;
            }
            break;

            case UIMainDef::FILE_TYPE_PICTURE:
            {
                m_PictureIsChecked = value;
            }
            break;
        }

        for (int i = 0; i < model->getListCount(); i++)
        {
            QModelIndex index = model->index(i);
            model->setData(index, value, BaseListModel::BaseCheckedRole);
        }
		
        // { added by lssanh 2013.03.23 NoCR copyall cancel
        if (value == true)
        {
            model->setFileURLCount(model->getListCount());
            QString listCount;
            listCount.append("(").append(QString::number(model->getListCount())).append(")");
            setModeAreaCount(listCount);
        }
        // } added by lssanh 2013.03.23 NoCR copyall cancel

        //setSelectAllEnabled(!value);    // fix coverity
        setDeselectAllEnabled(value);
    }
}

bool UI_Main::getSelectAllEnabled()
{
    return this->m_selectAllEnabled;
}

bool UI_Main::getDeselectAllEnabled()
{
    return this->m_deselectAllEnabled;
}

// modified by ravikanth for ITS 0188110
void UI_Main::setSelectAllEnabled(bool value)
{
    this->m_selectAllEnabled = value;
    emit selectAllEnabledChanged();
    if(value)
    {
        if(this->m_FileType == UIMainDef::FILE_TYPE_VIDEO)
        {
            setModeAreaCount(QString("(")+ QString::number(this->m_pStateManager->GetVideoModel()->getListCount()) +QString(")"));
        }
        else if(this->m_FileType == UIMainDef::FILE_TYPE_PICTURE)
        {
            setModeAreaCount(QString("(")+ QString::number(this->m_pStateManager->GetPictureModel()->getListCount()) +QString(")"));
        }
    }
    else
    {
        if(this->m_FileType == UIMainDef::FILE_TYPE_VIDEO)
        {
            setModeAreaCount(QString("(")+ QString::number(this->m_pStateManager->GetVideoModel()->getFileURLCount()) +QString(")"));
        }
        else if(this->m_FileType == UIMainDef::FILE_TYPE_PICTURE)
        {
            setModeAreaCount(QString("(")+ QString::number(this->m_pStateManager->GetPictureModel()->getFileURLCount()) +QString(")"));
        }
    }
}

void UI_Main::setDeselectAllEnabled(bool value)
{
    this->m_deselectAllEnabled = value;
    emit deselectAllEnabledChanged(value);
}

bool UI_Main::getHideBottomBar()
{
   return ( GetStateManager()->isFrontInstance() ? m_pEngine->getFrontAppId() == APP_SETTINGS
   												  :m_pEngine->getRearAppId() == APP_SETTINGS );
}

void UI_Main::setPhotoFrame()
{
    // FM_LOG << "UI_Main::setPhotoFrame() set photo frame to" <<
    //            m_currentFileForContextMenu << LOG_ENDL;
    this->m_pEngine->setPhotoFrame(m_currentFileForContextMenu,
                                   this->m_pStateManager->isFrontInstance());
								   
	// modified by ravikanth 10-07-13 for ITS 179182
    // modified by ravikanth 19-07-13 for remove replace popup, replace true with false for file overwrite feature
    if( this->m_pStateManager->CopyFileToMyFrame(m_currentFileForContextMenu, true) == 0)
    {
        onReplaceFileRequest(m_currentFileForContextMenu, -1);
    }
    else
    {
        m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID; // modified by ravikanth 05-08-13 for ITS 0179420
        emit showPopup( (int)UIMainDef::POPUP_TYPE_FRAME_SAVED );
    }
}

QString UI_Main::getSelectedPictureName()
{
    QFileInfo info(FileManagerEngine::GetValidFilePath(this->m_currentFileForContextMenu));
    return info.fileName();
}

void UI_Main::slotShowPopupDeleting()
{
   showPopup((int)UIMainDef::POPUP_TYPE_DELETING);
}

void UI_Main::slotShowPopupAllDeleting()
{
   // { modified by lssanh 2012.09.17 for CR13482
   //showPopup((int)POPUP_TYPE_DELETING_ALL);
   StartJukeBoxFormat();
   // } modified by lssanh 2012.09.17 for CR13482
}

void UI_Main::slotDeleteComplete()
{
    if (!m_isDeleteAllState)
    {
        // m_timer->stop(); // deleted by wspark 2012.10.11 for Function_Jukebox_1012
        setEditButton(true);
        //emit showPopup((int) UIMainDef::POPUP_TYPE_DELETE_COMPLETE);
    }
}

void UI_Main::onReplaceFileRequest(QString fileName,int totalFilesForCopy) // modified by ravikanth 07-07-13 for ITS 0178184
{
    QFileInfo file( fileName );

    // FM_LOG << "UI_Main::onReplaceFileRequest request for file" << fileName << LOG_ENDL;
    this->currentProcessedFile = file.fileName();
    this->m_currentReplaceCopyCount = totalFilesForCopy; // modified by ravikanth 07-07-13 for ITS 0178184
    //modified by aettie 201300613 for ITS 173605
    if(m_pCurrentOperation == UIMainDef::CURRENT_OPERATION_SAVE_FRAME)
    {
        emit showPopup( UIMainDef::POPUP_TYPE_REPLACE_FRAME_FILE );
    }
    else
    {
		// modified by ravikanth 23-07-13 for remove copy duplicate popup and duplicate files
        // new spec request, replace all files
        this->m_pStateManager->GetFileManager()->setOverwriteMode(
                  OVERWRITE_ALL_EXIST_FILE_MODE);
        //emit showPopup( POPUP_TYPE_REPLACE_FILE );
    }
}

QString UI_Main::getProcessedFile()
{
    // { modified by lssanh 2013.02.19 ISV72922
    //return this->currentProcessedFile;
	QFont mFont;
//modified by aettie.ji for ME font 20131015
	mFont.setFamily("DH_HDR");
	mFont.setPointSize(44);
	QFontMetrics fontMetrics(mFont);
	return fontMetrics.elidedText(this->currentProcessedFile, Qt::ElideRight, 900);
	// } modified by lssanh 2013.02.19 ISV72922
}

void UI_Main::onCapacityError(int filesCount, qint64 filesSize)
{
    // FM_LOG << "UI_Main::onCapacityError filesCount=" << filesCount <<
    //           "filesSize=" << filesSize << LOG_ENDL;

    this->m_filesCount = filesCount;
    this->m_totalFilesSize = filesSize / MB_SIZE;

    setCurrentPopupEvent((int) UIMainDef::POPUP_TYPE_CAPACITY_ERROR); // added by eugene.seo 2013.02.05 for no capacity error popup
    emit showPopup((int) UIMainDef::POPUP_TYPE_CAPACITY_ERROR);
    onCopyOpertaionExit(); // modified by ravikanth 19-04-13
}

double UI_Main::getJukeboxCapacity() // modified by ruindmby 2012.08.22 for CR 12454
{
    return this->jukeBoxSize;
}

double UI_Main::getJukeboxUsedSize() // modified by ruindmby 2012.08.22 for CR 12454
{
    return this->usedJukeBoxSize;
}

FileManagerMenu* UI_Main::getOptionMenu()
{
    return this->optionMenu.data();
}

void UI_Main::showCapacityPopup()
{
// modified by Dmitry Bykov 04.04.2013
    if (getFileInterface() != NULL)
    {
        this->jukeBoxSize = (double(getFileInterface()->GetTotalDiscSpace(JUKEBOX_VOLUME)) / MB_SIZE);
        this->usedJukeBoxSize = (double(getFileInterface()->GetTotalUsedDiscSpace(JUKEBOX_VOLUME)) /
                                     MB_SIZE);
    }
 // modified by Dmitry Bykov 04.04.2013
    showPopup(UIMainDef::POPUP_TYPE_JUKEBOX_CAPACITY);
}

int UI_Main::getFilesCount()
{
    return this->m_filesCount;
}

int UI_Main::getFilesSize()
{
    return this->m_totalFilesSize;
}

void UI_Main::contextDeleteId()
{
   setEditButton(false);
   setBottomBarMode( (int) UIMainDef::BOTTOM_BAR_DELETE);
   updateModeAreaType();
}

void UI_Main::contextDeleteAllId()
{
// modified by ravikanth 24-07-13 for copy cancel confirm on delete
    if(getCurrentAudioFileOperation())
    {
        m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_FORMAT_JUKEBOX;
        emit showPopup(UIMainDef::POPUP_TYPE_CANCEL_COPY_FOR_CLEAR_JUKEBOX_CONFIRM);
    }
    else
    {
        setEditButton(true);
        setBottomBarMode( (int) UIMainDef::BOTTOM_BAR_INVISIBLE);
        showPopup( (int)UIMainDef::POPUP_TYPE_OPTION_MENU_DELETE_ALL );
    }
}

void UI_Main::contextMoveId()
{
   setEditButton(false);
   setBottomBarMode( (int) UIMainDef::BOTTOM_BAR_MOVE);
}

void UI_Main::contextInteractiveId()
{
   setEditButton(true);
   setModelCheckValue(false);
   setBottomBarMode( (int) UIMainDef::BOTTOM_BAR_INVISIBLE);
   fileSelectCountReset(); //added by yungi 2013.03.21 for ISV 76723
   m_pStateManager->StateChangeUpdate(); //added by yungi 2013.03.19 ISV 75920
   updateModeAreaType(); // modified by ravikanth 25-06-13 for ITS 0175970
}

void UI_Main::handleNowPlaying()
{
    int state = this->m_pStateManager->GetCurrentState();
    switch (state)
    {
    case eJukeVideo:
    case eUsbVideo:
        this->m_pEngine->launchVideoPlayer(this->m_pStateManager->isFrontInstance());
        break;
    case eJukePictureShow:
    case eUsbPictureShow:
        //{ added by yongkyun.lee 20130322 for : ISV 76023
        //this->m_pEngine->launchPhotoGallery(this->m_pStateManager->isFrontInstance());
        m_pEngine->HandleBackKey(this->m_pStateManager->isFrontInstance());
        //} added by yongkyun.lee 20130322 
        break;
    default:
        // FM_LOG << "UI_Main::handleNowPlaying can't show player for state" <<
        //            state << LOG_ENDL;
        break;
    }

}

// { modified by lssanh 2012.09.17 for CR13482
void UI_Main::StartJukeBoxFormat()
{
    m_isDeleteAllState = true;

    if (getFileInterface() != NULL) // modified by Dmitry Bykov 04.04.2013
    {
        QString volumeName = QUrl("/usr_data").path();
        getFileInterface()->StartDiscFormat(volumeName); // modified by Dmitry Bykov 04.04.2013
    }
}

void UI_Main::FormattingComleteHandler(int status)
{
    // FM_LOG << "UI_Main::FormattingComleteHandler"  << LOG_ENDL;
    Q_UNUSED(status)

    m_formatting_timer->stop();
    showPopup((int)UIMainDef::POPUP_TYPE_FORMAT_COMPLETE);
    m_pEngine->notifyFormatJukeboxEnd();

    m_isDeleteAllState = false;
}
// } modified by lssanh 2012.09.17 for CR13482

void UI_Main::cancelMoveMode()
{
   contextInteractiveId();
   m_pStateManager->setMoveMode(false);
}

void UI_Main::onCopyError(int typeError)
{
    // FM_LOG << "UI_Main::onCopyError typeError=" << typeError << LOG_ENDL;

    switch(typeError)
    {
       case eMP_CORRUPT_FILE_COPY_ERROR:
       case eMP_UNSUPPORTED_FORMAT_COPY_ERROR:
       {
          // FM_LOG<<"eMP_UNSUPPORTED_FORMAT_COPY_ERROR" << LOG_ENDL;
          if( getCurrentPopupEvent() != UIMainDef::POPUP_TYPE_CAPACITY_ERROR ) // added by eugene.seo 2013.02.05 for no capacity error popup
          {
                  setCurrentPopupEvent((int) UIMainDef::POPUP_TYPE_CORROPTED_ERROR); // add by wspark 2012.07.12 for CR9616.
                  emit showPopup((int) UIMainDef::POPUP_TYPE_CORROPTED_ERROR);
          }
          break;
       }

       case eMP_JUKEBOX_ERROR:
       {
          // FM_LOG<<"eMP_JUKEBOX_ERROR " << LOG_ENDL;
          emit showPopup((int) UIMainDef::POPUP_TYPE_JUKEBOX_ERROR);
          break;
       }
	   // modified by ravikanth 21.06.13 for ITS 0174790
       case eMP_USER_CANCEL_COPY_ERROR:
       {
          emit showPopup( (int)UIMainDef::POPUP_TYPE_COPY_CANCELLED );
          break;
       }

       default:
          // FM_LOG << "UI_Main::onCopyError : undefined error" << LOG_ENDL;
          break;
    }
}

// { added by Sergey for CR#14494
void UI_Main::onOperationStatusChanged(EOperationStatus status)
{
    // MP_LOG << "UI_Main::onOperationStatusChanged status =" << status << LOG_ENDL;

    m_eOperationStatus = status;
}
// } added by Sergey for CR#14494.

void UI_Main::slotUpdateModeArea()
{
    updateModeAreaType();
}


void UI_Main::setModeAreaMode(UIMainDef::MODE_AREA_TEXT type)
{
    // FM_LOG << "UI_Main::setModeAreaMode type ="<<type << LOG_ENDL;

    switch(type)
    {
       case UIMainDef::MODE_AREA_TEXT_DEFAULT: setModeAreaTitle(QT_TR_NOOP("STR_MEDIA_LIST"));
                                    setModeAreaFileCount(""); //added by yungi 2013.03.08 for New UX FileCount
        break;

       case UIMainDef::MODE_AREA_TEXT_COPY_TO: setModeAreaTitle(QT_TR_NOOP("STR_MEDIA_MNG_COPY_LOCATION"));
        break;

       case UIMainDef::MODE_AREA_TEXT_COPY_FROM:
            setModeAreaTitle(QT_TR_NOOP("STR_MEDIA_MNG_COPY_TO_JUKEBOX"));
            // commented by ravikanth 08-07-13 for Smoke Test Copy To Jukebox count mismatch after source change
            //if(m_VideoIsChecked == false && m_PictureIsChecked == false)  //added by yungi 2013.03.21 for ISV 76723
                fileSelectCountReset(); //added by yungi 2013.03.08 for New UX FileCount
        break;

       case UIMainDef::MODE_AREA_TEXT_MOVE_TO: setModeAreaTitle(QT_TR_NOOP("STR_MEDIA_MNG_LOCATION_TO_BE_MOVED"));
        break;

       case UIMainDef::MODE_AREA_TEXT_DELETE:
            setModeAreaTitle(QT_TR_NOOP("STR_MEDIA_MNG_DELETE"));
            fileSelectCountReset(); //added by yungi 2013.03.08 for New UX FileCount
        break;

       case UIMainDef::MODE_AREA_TEXT_MOVE_FROM: setModeAreaTitle(QT_TR_NOOP("STR_MEDIA_MNG_MOVE"));
        break;

       case UIMainDef::MODE_AREA_TEXT_ENTER_FOLDER_NAME: setModeAreaTitle(QT_TR_NOOP("STR_MEDIA_MNG_ENTER_FOLDERNAME"));
        break;

       case UIMainDef::MODE_AREA_TEXT_RENAME: setModeAreaTitle(QT_TR_NOOP("STR_MEDIA_MNG_INPUT_NEW_NAME"));
        break;

       case UIMainDef::MODE_AREA_TEXT_WAITING_DISPLAY: setModeAreaTitle(QT_TR_NOOP("STR_MEDIA_MNG_WAITING_DISPLAY"));
       break;


       default:
       {
          setModeAreaTitle(QT_TR_NOOP("STR_MEDIA_LIST"));
          setModeAreaFileCount(""); //added by yungi 2013.03.08 for New UX FileCount
       }
    }
    // } modified by wspark
}


void UI_Main::updateModeAreaType()
{
   // FM_LOG<<" UI_Main::updateModeAreaType()" << LOG_ENDL;
   if (!m_KeyPadIsDisplayed)
   {
       switch(getBottomBarMode())
       {
        case UIMainDef::BOTTOM_BAR_INVISIBLE:
       {
           if (m_ContentMode != UIMainDef::CONTENT_MODE_LIST)
           {
              setModeAreaMode(UIMainDef::MODE_AREA_TEXT_DEFAULT);
           }
           break;
       }

       case UIMainDef::BOTTOM_BAR_COPY_TO: setModeAreaMode(UIMainDef::MODE_AREA_TEXT_COPY_TO);
           break;

       case UIMainDef::BOTTOM_BAR_COPY_FROM: setModeAreaMode(UIMainDef::MODE_AREA_TEXT_COPY_FROM);
           break;

       case UIMainDef::BOTTOM_BAR_MOVE_TO: setModeAreaMode(UIMainDef::MODE_AREA_TEXT_MOVE_TO);
           break;

       case UIMainDef::BOTTOM_BAR_DELETE: setModeAreaMode(UIMainDef::MODE_AREA_TEXT_DELETE);
           break;

       case UIMainDef::BOTTOM_BAR_MOVE: setModeAreaMode(UIMainDef::MODE_AREA_TEXT_MOVE_FROM);
           break;

       default:
       {
           setModeAreaMode(UIMainDef::MODE_AREA_TEXT_DEFAULT);
       }
       }       
   }
   else
   {
    // FM_LOG<<"UI_Main::updateModeAreaType() m_KeyPadIsDisplayed" << LOG_ENDL;
   }
}

void UI_Main::slotOnBg()
{
    if(m_eOperationStatus == eOperationPaused)
         m_pStateManager->GetFileManager()->cancelOperation();
}

// { added by wspark 2013.02.03
void UI_Main::slotOnFg(int display)
{
    emit hidePopupOnFG(display); // modified by ravikanth 10-07-13 for ITS 0179181
}
// } added by wspark

// { added by lssanh 2013.02.07 2USB
void UI_Main::slotCancelFileOperation()
{
    MP_LOG << "slotCancelFileOperation" << LOG_ENDL;

    if (m_pCurrentOperation != UIMainDef::CURRENT_OPERATION_INVALID)
    {
        if(isFileOperationInProgress()) // modified for ISV, no cancel popup if copy is not in progress
        {
            m_pStateManager->GetFileManager()->cancelOperation();
            this->m_pStateManager->GetFileManager()->setOverwriteMode(CANCEL_COPY_OPERATION_MODE);
            // modified by ravikanth 16-07-13 for ISV 87550, ITS 0178981, ITS 0183707
            if(((m_pStateManager->currentCopyMode() == VIDEO_FILE && getCurrentFileType() == UIMainDef::FILE_TYPE_VIDEO)
                || (m_pStateManager->currentCopyMode() == IMAGE_FILE && getCurrentFileType() == UIMainDef::FILE_TYPE_PICTURE)) && (getCurrentDevice() == UIMainDef::DEVICE_USB))
            {
				// modified for ITS 0214738
                if((m_pStateManager->FileOperationStopMediaType() == eStartInvalidSState)
                        && !m_pEngine->isBgState(m_pStateManager->isFrontInstance()))
                {
                    emit showPopup(UIMainDef::POPUP_TYPE_COPY_CANCELLED);
                }
                else
                {
                    if((m_pStateManager->currentCopyMode() == IMAGE_FILE) && (m_pStateManager->FileOperationStopMediaType() != eStartUsbPicture)
                            || (m_pStateManager->currentCopyMode() == VIDEO_FILE) && (m_pStateManager->FileOperationStopMediaType() != eStartUsbVideo))
                    {
                        DisplayOsdOnFR(QCoreApplication::translate("main", "STR_MEDIA_MNG_CANCEL_FILE_COPY_CANCELLED"));
                        popupEventHandler(UIMainDef::POPUP_EVENT_COPY_CANCELED_POPUP_CLOSED);
                    }
                    else
                    {
                        emit showPopup(UIMainDef::POPUP_TYPE_COPY_CANCELLED);
                    }
                }
            }
        }
        else
        {
            if(((m_pStateManager->currentCopyMode() == VIDEO_FILE && getCurrentFileType() == UIMainDef::FILE_TYPE_VIDEO)
                || (m_pStateManager->currentCopyMode() == IMAGE_FILE && getCurrentFileType() == UIMainDef::FILE_TYPE_PICTURE)) && (getCurrentDevice() == UIMainDef::DEVICE_USB)) // modified for ITS 0208621
            popupEventHandler(UIMainDef::POPUP_EVENT_COPY_CANCELED_POPUP_CLOSED);
        }
    }
}
// } added by lssanh 2013.02.07 2USB

// { added by junggil 2012.10.09 for CR14142 & CR14228 & CR14275 
void UI_Main::slotOnShowProgressingPopup()
{
    if(m_currentFileType == getCurrentFileType()) //added by eunhye 2012.12.20 for No CR
    switch (m_pCurrentOperation)
    {
        case UIMainDef::CURRENT_OPERATION_COPY:
           if(m_Device == UIMainDef::DEVICE_USB) //added by eunhye 2012.12.20 for No CR
              emit showPopup( (int)UIMainDef::POPUP_TYPE_COPYING );
           break;

        case UIMainDef::CURRENT_OPERATION_MOVE: emit showPopup( (int)UIMainDef::POPUP_TYPE_MOVING );
           break;

        // added by minho 20121016
        // for CR14271: [3PDMS][142453] When Power Off/On from the pop-up of Cancel Copying files on Jukebox while playing USB Video, the pop-up disappeared
        case UIMainDef::CURRENT_OPERATION_COPY_CANCEL: emit showPopup( (int)UIMainDef::POPUP_TYPE_COPY_CANCEL );
           break;

        case UIMainDef::CURRENT_OPERATION_MOVE_CANCEL: emit showPopup( (int)UIMainDef::POPUP_TYPE_MOVE_CANCEL );
           break;
        // added by minho

        // { added by junggil 2012.10.18 for CR14336 [3PDMS][141287] No display of pop-up "Copy to Jukebox" when rejecting a call 
        case UIMainDef::CURRENT_OPERATION_MOVE_ALL: emit showPopup( (int)UIMainDef::POPUP_TYPE_MOVE_ALL );
           break;

        case UIMainDef::CURRENT_OPERATION_COPY_ALL: popupEventHandler(UIMainDef::POPUP_EVENT_COPY_ALL_CONFIRMATION);  // modified by ravikanth 16-04-13
		//emit showPopup( (int)POPUP_TYPE_COPY_ALL_CONFIRMATION );
           break;
        // } added by junggil 
        // { added by eunhye 2012.10.18 for Function_Jukebox_1655
        case UIMainDef::CURRENT_OPERATION_DELETE: emit showPopup( (int)UIMainDef::POPUP_TYPE_DELETE );
           break;
        case UIMainDef::CURRENT_OPERATION_DELETE_ALL: emit showPopup( (int)UIMainDef::POPUP_TYPE_DELETE_ALL );
           break;
        // } added by eunhye 2012.10.18 
        default:
           // FM_LOG<<"Invalid operation type"  << LOG_ENDL;
           break;
    }
}
// } added by junggil 

void UI_Main::slotCurrentFolderChanged()
{
    m_TitleProvider.setCurrentPath( m_pStateManager->getCurrentFolder() );
}

void UI_Main::slotOnUsbRemoved(int nPort) // modified by eugene.seo 2013.05.29
{
    // removed by sangmin.seol 2013.10.15 ITS-0195512 Remain copyToJukebox View until USB removing complete..
    //m_editButtonIsVisible = true;
    //setBottomBarMode( (int)UIMainDef::BOTTOM_BAR_INVISIBLE );
    // removed by sangmin.seol 2013.10.15 ITS-0195512 Remain copyToJukebox View until USB removing complete..
    // { added by lssanh 2013.02.28 ISV72537 USB remove
	// { removed by eugene.seo 2013.06.05 
    //if (m_pEngine->getDisplayVisible() != DISPLAY_NONE)
    //{
    //    m_pEngine->HandleBackKey(this->m_pStateManager->isFrontInstance());
    //}
	// } removed by eugene.seo 2013.06.05 
	QString currentPath = m_pStateManager->getCurrentPath();
    if (m_pCurrentOperation != UIMainDef::CURRENT_OPERATION_INVALID)
    {
        // { modified by eugene.seo 2013.05.29
        // if( (nPort == 1 && currentPath.contains(USBFront)) || (nPort == 2 && currentPath.contains(USBRear)) )
        if( (nPort == 1 && currentPath.indexOf(m_pEngine->GetFrontUSBPath()) == 0) || (nPort == 2 && currentPath.indexOf(m_pEngine->GetRearUSBPath()) == 0) ) // modified by kihyung 2013.12.09 for ITS 0213549 
        {
            m_pStateManager->GetFileManager()->cancelOperation(true);
            this->m_pStateManager->GetFileManager()->setOverwriteMode(CANCEL_COPY_OPERATION_MODE);
            // commented by ravikanth 06-04-13 (moved this change to setBrowseOnly())
            // setModelCheckValue(false);  
            emit hidePopup();
            // show copy cancelled popup when usb disconnected if copy progress ITS 0186967
            if(m_pCurrentOperation == UIMainDef::CURRENT_OPERATION_COPY ||
                    UIMainDef::CURRENT_OPERATION_COPY_ALL ||
                    UIMainDef::CURRENT_OPERATION_COPY_CANCEL_FOR_COPY)
            {
                emit showPopup(UIMainDef::POPUP_TYPE_COPY_CANCELLED);

                // added by sangmin.seol for ITS-0207841 reset select-all if copy canceled by usb remove
                if(m_pCurrentOperation != UIMainDef::CURRENT_OPERATION_COPY) // case for CURRENT_OPERATION_COPY_ALL & CURRENT_OPERATION_COPY_CANCEL_FOR_COPY
                {
                    setCopyAllStatus(false);
                    setSelectAllEnabled(false);
                }
                // added by sangmin.seol for ITS-0207841 reset select-all if copy canceled by usb remove
            }

            // moved for ITS 0190421
            m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID; //added by edo.lee 2012.11.09 USB remove while copying pictures
        }
        // } modified by eugene.seo 2013.05.29
    }
    // } added by lssanh 2013.02.28 ISV72537 USB remove
    
	// { added by yongwoo.park 2013.10.03 ISV 
	if( (nPort == 1 && currentPath.indexOf(m_pEngine->GetFrontUSBPath()) == 0) || (nPort == 2 && currentPath.indexOf(m_pEngine->GetRearUSBPath()) == 0) ) // modified by kihyung 2013.12.09 for ITS 0213549 
		this->m_pStateManager->hideCountInfo();
	// } added by yongwoo.park 2013.10.03 ISV 

    updateModeAreaType();
    emit editButtonChanged(m_editButtonIsVisible); //modified for [KOR][ISV][79666][B](aettie.ji)
}

// { add by wspark 2012.07.12 for CR9616
// ignore CopyComplete signal when there is CORRUPTED ERROR popup.
int UI_Main::getCurrentPopupEvent()
{
    return this->m_currentPopupEvent;
}

void UI_Main::setCurrentPopupEvent(int value)
{
    this->m_currentPopupEvent = value;
}
// } add by wspark

// { add by wspark 2012.07.25 for CR12226
// showing popup differently according to copyAll Status.
bool UI_Main::getCopyAllStatus()
{
    return this->m_isCopyAll;
}

void UI_Main::setCopyAllStatus(bool value)
{
    m_isCopyAll = value;
}
// } add by wspark

// { added by yungi 2013.03.08 for New UX FileCount
void UI_Main::setModeAreaCount(QString value)
{
    m_ModeAreaFileCount = value;
    emit modeAreaFileCountChanged();
}

void UI_Main::fileSelectCountReset()
{
	// modified by ravikanth for ITS 0184642
    if(m_fmLaunchInBrowseMode)
    {
        this->m_pStateManager->GetPictureModel()->setFileURLCount(0);
        this->m_pStateManager->GetVideoModel()->setFileURLCount(0);
        setModeAreaFileCount("(0)");
    }
    else
    {
        if(m_FileType == UIMainDef::FILE_TYPE_VIDEO) {
            setModeAreaFileCount("("+QString::number(this->m_pStateManager->GetVideoModel()->getFileURLCount())+")");
        }
        else if(m_FileType == UIMainDef::FILE_TYPE_PICTURE) {
            setModeAreaFileCount("("+QString::number(this->m_pStateManager->GetPictureModel()->getFileURLCount())+")");
        }
    }
}
// } added by yungi 2013.03.08 for New UX FileCount

 // added by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list 
void UI_Main::slotShowCopy(bool m_isCopy, bool unsupportedFile)
{
	// modified by ravikanth for ITS 0184642
    m_fmLaunchInBrowseMode = m_isCopy;
    m_pStateManager->setEditModeLaunchFromAVP(!m_isCopy, unsupportedFile);
}
 //added by eunhye 2013.02.26

// { modified by ravikanth 06-04-13
void UI_Main::slotShowBottomBarOnCopy()
{
    if(!m_fmLaunchInBrowseMode)
    {
        emit loadBottomBar();
        emit bottomBarModeChanged();
    }
}
// } modified by ravikanth 06-04-13

// { modified by ravikanth 19-04-13
void UI_Main::onCopyOpertaionExit()
{
    MP_LOG << "copy exited m_FileType= " << m_FileType << LOG_ENDL;
	// modified by ravikanth 12-07-13 for ITS 0174301
    if(m_FileType == UIMainDef::FILE_TYPE_VIDEO)
    {
        m_pEngine->notifyCopyToJukeboxEnd(true);
    }
    else
    {
        m_pEngine->notifyCopyToJukeboxEnd(false);
    }
    m_pSharedFMData->SetStatusDateByIndex(DATA_TYPE_FILEOPERATION_STATUS, FILEOPERATION_STATUS_NONE);
}
// } modified by ravikanth 19-04-13

// { modified by ravikanth 22-04-13
int UI_Main::currentCopySkipCount()
{
    return m_currentSkipCount;
}

void UI_Main::setCopySkipCount(int count)
{
    m_currentSkipCount = count;
    emit copySkipCountChanged();
}

int UI_Main::currentCopyTotalCount()
{
    return m_currentCopyTotalCount;
}

void UI_Main::setCopyTotalCount(int count)
{
    m_currentCopyTotalCount = count;
}
// } modified by ravikanth 22-04-13

// { modified by ravikanth 27-04-13
bool UI_Main::IsCurrentVideoPlaying()
{
    return m_pSharedVideoData->GetStatusDateByIndex(DATA_TYPE_PLAY_STATUS); // modified by kihyung 2013.07.15 for STATIC 20894
}
// } modified by ravikanth 27-04-13

// modified by ravikanth 29-06-13 for ITS 0176909
void UI_Main::slotcancelFileOperationOnInstance(bool isFront)
{
     MP_LOG << "slotcancelFileOperationOnInstance isFront" << isFront << LOG_ENDL;
    if(isFront != m_pStateManager->isFrontInstance())
    {
        MP_LOG << "slotcancelFileOperationOnInstance " << m_pStateManager->isFrontInstance() << LOG_ENDL;
        if (m_pCurrentOperation != UIMainDef::CURRENT_OPERATION_INVALID)
        {
            m_pStateManager->GetFileManager()->cancelOperation();
            m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_INVALID;
            emit showPopup(UIMainDef::POPUP_TYPE_COPY_CANCELLED);
        }
    }
}

void UI_Main::slotEngineShowPopup(int type, bool isFront){
    MP_LOG << "slotEngineShowPopup isFront" << isFront << "State : " << m_pStateManager->isFrontInstance() << LOG_ENDL;
	if ( !(isFront ^ m_pStateManager->isFrontInstance()))
	{
		emit showPopup(type);
	}
}

void UI_Main::startCopy()
{
    popupEventHandler(UIMainDef::POPUP_EVENT_COPY_ALL_CONFIRMATION);
}

// modified by ravikanth 05-07-13 for ITS 0174301
void UI_Main::onCopyOperationComplete(int skipCount, int copyFilesCount)
{
    MP_LOG << "onCopyOperationComplete " << m_pCurrentOperation << LOG_ENDL;
    if(m_pCurrentOperation == UIMainDef::CURRENT_OPERATION_COPY)
    {
        setCopyTotalCount(copyFilesCount);
        setCopySkipCount(skipCount);
        if( getCurrentPopupEvent() != UIMainDef::POPUP_TYPE_CORROPTED_ERROR)
        {
            if((skipCount == 0) && (copyFilesCount == 0)) // modified for ITS 0208078
            {
                emit closeCopyProgressPopup(); // modified Michael.Kim 2013.11.22 for ITS 210478
                popupEventHandler( UIMainDef::POPUP_TYPE_COPY_CANCELLED );
            }
            else
            {
                // modified for ITS 0208724
                QTimer::singleShot(100, this, SLOT(showCopyCompletePopup()));
                //popupEventHandler( UIMainDef::POPUP_EVENT_COPY_COMPLETED );
            }
        }
    }
}

//[KOR][ITS][179014][comment](aettie.ji)
void UI_Main::setCopyFromMainPlayer(bool val)
{ 
	// modified by ravikanth for ITS 0184642
    m_fmLaunchInBrowseMode = true;
    m_pStateManager->setEditModeLaunchFromAVP(val, false);
	//m_pEngine->setCopyFromMainPlayer(val);
}

// modified by ravikanth 21-07-13 for copy cancel confirm on delete
void UI_Main::startDelete()
{
    //m_pCurrentOperation = UIMainDef::CURRENT_OPERATION_DELETE; // modified by ravikanth 02-09-13
    popupEventHandler( UIMainDef::POPUP_EVENT_DELETE_CONFIRMATION );
}

// modified by ravikanth 14-08-13 for ISV 88691
// modified in sync with music as requested in ISV
int UI_Main::RemainedCapacity()
{
   QString volumeName = QUrl("/usr_data").path();

   double fullSize = 0;
   double usedSize = 0;

   fullSize = double(getFileInterface()->GetTotalDiscSpace(volumeName)) / MB_SIZE ;
   usedSize = double(getFileInterface()->GetTotalUsedDiscSpace(volumeName)) / MB_SIZE ;

   return fullSize - usedSize;
}

// modified by ravikanth for ITS 0184642
void UI_Main::onResetFMLaunchBrowseState(int screen)
{
    if (m_pStateManager->isFrontInstance() && screen != AppEngineQMLConstants::SCREEN_FRONT)
    {
        return;
    }
    setCopyFromMainPlayer(false);
}

//added by suilyou 20130925 START
void UI_Main::setSharedMemData(int data)
{
    m_pSharedFMData->SetStatusDateByIndex(DATA_TYPE_JOG_KEY_STATUS, data);
}

int UI_Main::getSharedMemData()
{
    return m_pSharedFMData->GetStatusDateByIndex(DATA_TYPE_JOG_KEY_STATUS);
}
//added by suilyou 20130925 END
void UI_Main::clearSharedData()
{
    setSharedMemData(0);
    MP_LOG << "clearSharedData() " << getSharedMemData() << LOG_ENDL;
}

// modified by ravikanth 08-10-13 for format cancel
void UI_Main::onCancelJukeboxFormat()
{
    MP_LOG << m_isDeleteAllState << LOG_ENDL;
    if ((getFileInterface() != NULL) && m_isDeleteAllState)
    {
        getFileInterface()->CancelFormatOperation();
        m_isDeleteAllState = false;
    }
}

// modified on 31-10-13 changes to fix altered metadata manually by user for an image
void UI_Main::showPopupType(int type, bool isFront)
{
    slotEngineShowPopup(type,isFront);
}

bool UI_Main::isFileOperationInProgress()
{
    if(m_pStateManager->GetFileManager())
        return (FileOperationsHelper::operationUnknown != m_pStateManager->GetFileManager()->currentOperation());
    else
        return false;
}

// modified for ITS 0208724
void UI_Main::showCopyCompletePopup()
{
    MP_CRITICAL << LOG_ENDL;
    emit closeCopyProgressPopup();
    popupEventHandler( UIMainDef::POPUP_EVENT_COPY_COMPLETED );
}

// modified for ITS 0214738
void UI_Main::DisplayOsdOnFR(QString msg)
{
    MP_MEDIUM << msg << LOG_ENDL;
    QString info = QString("@OSD:FR^MEDIA_INFO^WARN^JUKEBOX^");
    info += "JUKEBOX^";
    info += msg;
    OSDWriter::SendOSD(info);
}
