#include <qfileinfo.h>


#include "DHAVN_AppFileManager_CStateManager.h"
#include "DHAVN_AppFileManager_AudioListModel.h"
#include "DHAVN_AppFileManager_VideoListModel.h"
#include "DHAVN_AppFileManager_PictureListModel.h"
#include "DHAVN_AppFileManager_BaseListModel.h"
#include <DHAVN_FileOperationsHelper.h>
#include <DHAVN_AppFileManager_Engine.h>
#include <DHAVN_AppFileManager_Shared.h>
// removed by Dmitry 14.08.13 for ITS0183845
#include "DHAVN_AppFileManager_UI_main.h"
#include <DHAVN_AppFramework_QML_Def.h>
#include <DHAVN_AppFramework_AppEngineQML_Def.h> // added by edo.lee 2012.12.06 for statusbar
#include <DHAVN_TrackerAbstractor.h> // added by kihyung 2013.2.17



#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QTimer>
#include <QFileInfo>

//const QString AppFileManagerUsbPicturePath  = "/app/share/images/";
//const QString AppFileManagerJukePicturePath =  "/app/share/images/";

// modified by Dmitry 22.05.13
QString AppFileManagerUsbMountPath = "/media";
const QString AppFileManagerJukePicturePath = "/usr_data/media/Picture";
const QString AppFileManagerSaveFramePath = "/home/meego";   //added by yungi 2013.2.8 for UX Scenario 11. Saveframe path changed
const QString AppFileManagerJukeVideoPath = "/usr_data/media/Video";
const QString AppFileManagerJukeAudioPath = "/usr_data/media";
const QString AppFileManagerNoUsb = "/home/meego/no_usb";

//const QString AppFileManagerMyFramePicturePath = "file:///usr_data/media/Picture/My Frame"; //deleted by yungi 2013.2.8 for UX Scenario 11. Saveframe path changed
const QString AppFileManagerMyFramePicturePath = "/home/meego/saveframe"; //added by yungi 2013.2.8 for UX Scenario 11. Saveframe path changed

const int AppFileManager_Count_Item_On_VideoFolder = 2;
const int AppFileManager_Count_Item_On_PictureFolder = 2;

// removed by kihyung 2013.12.09 for ITS 0213549 
// modified by ravikanth 02-09-13
// const QString USBFront = QString("/media/front_usb");
// const QString USBRear = QString("/media/rear_usb");

// modified by Dmitry 21.08.13
const int CStateManager::stateTable[18][4] =
{
    { eUSB, eAudio, 2, eFileManager},         // eUsbMusicTitle
    { eUSB, eAudio, 0, eFileManager},         // eUsbMusicArtist
    { eUSB, eAudio, 1, eFileManager},         // eUsbMusicAlbom
    { eUSB, eVideo, 0, eFileManager},         // eUsbVideo
    { eUSB, eVideo, 0, eFileManager},         // eUsbVideoCopy
    { eUSB, ePicture, 0, eFileManager},       // eUsbPictureShow
    { eUSB, ePicture, 0, eFileManager},       // eUsbPictureCopy
    { eJukeBox, eAudio, 2, eFileManager},     // eJukeMusicTitle
    { eJukeBox, eAudio, 0, eFileManager},     // eJukeMusicArtist
    { eJukeBox, eAudio, 1, eFileManager},     // eJukeMusicAlbom
    { eJukeBox, eVideo, 0, eFileManager},     // eJukeVideo
    { eJukeBox, eVideo, 0, eFileManager},     // eJukeVideoCopy
    { eJukeBox, ePicture, 0, eFileManager},   // eJukePictureShow
    { eJukeBox, ePicture, 0, eFileManager},    // eJukePictureCopy
    { eJukeBox, eVideo, 0, eFileList},     // eJukeListVideo
    { eUSB, eVideo, 0, eFileList},     // eUsbListVideo
    { eJukeBox, ePicture, 0, eFileList},   // eJukeListPicture
    { eUSB, ePicture, 0, eFileList}    // eUsbListPicture
};

QString getStringState(EAppFileManagerState state)
{
    switch (state)
    {
    case eUsbMusicTitle:
        return "eUsbMusicTitle";
    case eUsbMusicArtist:
        return "eUsbMusicArtist";
    case eUsbMusicAlbom:
        return "eUsbMusicAlbom";
    case eUsbVideo:
        return "eUsbVideo";
    case eUsbVideoCopy:
        return "eUsbVideoCopy";
    case eUsbPictureShow:
        return "eUsbPictureShow";
    case eUsbPictureCopy:
        return "eUsbPictureCopy";
    case eJukeMusicTitle:
        return "eJukeMusicTitle";
    case eJukeMusicArtist:
        return "eJukeMusicArtist";
    case eJukeMusicAlbom:
        return "eJukeMusicAlbom";
    case eJukeVideo:
        return "eJukeVideo";
    case eJukeVideoCopy:
        return "eJukeVideoCopy";
    case eJukePictureShow:
        return "eJukePictureShow";
    case eJukePictureCopy:
        return "eJukePictureCopy";
    case eUsbListVideo:
        return "eUsbListVideo";
    case eJukeListVideo:
        return "eJukeListVideo";
    case eUsbListPicture:
        return "eUsbListPicture";
    case eJukeListPicture:
        return "eJukeListPicture";
    case eJukeVideoMove:
        return "eJukeVideoMove";
    case eJukePictureMove:
        return "eJukePictureMove";
    default:
        return "Unknown";
    }
}

CStateManager::CStateManager(QObject *parent, bool front, int state, /*USBController* controller,*/
                             FileManagerEngine* engine, QDeclarativeContext *ctxt, QTranslator *translator) :
    QObject(parent),
    appEngine(engine),
    numberOfItemInCurrentFolder(0),
    lastIndexOfFolderInCurrentFolder(-1),
    indexOfCurrentRequestFolder(0),
    currentMusicFilter(eAlbum),
    currentPlayAudioAlbum(""),
    currentPlayAudioArtist(""),
    m_pCtxt( ctxt ),
    isInitialized( false ),
    m_rootState(false), //modified for list focus at bottom index 20131020
    m_pCurrentFolder(""),
// removed by Dmitry 27.07.13
    m_showCountState(eShowNone),
// removed by Dmitry 27.07.13
    //fileController(controller),
    frontInstance(front),
    m_pBrowseMode(false),
    m_pTranslator(translator),
    m_entryState(eInvalidState), // added by junam 2012.12.01 for CR16077
// modified by Dmitry 15.04.13 for ISV79935
    m_emptyState(false), // added by wspark 2012.12.13 for CR16396
    m_bIsMenuVisible(true),
    m_copyModeType(0),
    m_editModeFromAVP(false), // modified by ravikanth for ITS 0184642
    m_fmLaunchUnsupportedFileCopy(false), // modified for ITS 0209355
    m_currentCopyStopState(eStartInvalidSState) // modified for ITS 0214738
// modified by Dmitry 15.04.13 for ISV79935
{
    // FM_LOG_INIT_V2("AppFileManager");
    this->uiControl.reset(new UI_Main(engine, this));
    setCurrentState(EAppFileManagerState(state));
    // MP_LOG << "CStateManager ctor.: Set initial state to " << getStringState(currentState) << LOG_ENDL;

    connect(appEngine, SIGNAL(postEventToStart(int, QString, QString, int)), this,
            SLOT(AppPostEventHandler(int, QString, QString, int)));
    // { modified by kihyung 2012.08.02 for CR 12191
	// { added by edo.lee 2012.12.08 for statusbar
	m_pOSDview = new QDeclarativeView();
	// } added by edo.lee
    /*
    connect(appEngine, SIGNAL(signalUSBPluggedOut()), this, SLOT(onUSBRemoved()));
    connect(appEngine, SIGNAL(signalUSBPluggedIn(const QString&)), this, SLOT(onUSBMounted(const QString&)));
    */
    connect(appEngine, SIGNAL(signalUSBPluggedOut(const int)), this, SLOT(onUSBRemoved(const int)));
    connect(appEngine, SIGNAL(signalUSBPluggedIn(const QString&,const int)), this, SLOT(onUSBMounted(const QString&, const int)));
    // } modified by kihyung
    connect(appEngine, SIGNAL(postEventPlayFileChanged(const QString&, int, int)),
            this, SLOT(AppPostEventCurrentPlayChanged(const QString&, int, int)));
// removed by Dmitry 16.10.13
	m_isFolder = false; // added by eugene 2012.12.12 for NewUX - Photo #5-2

    connect (appEngine, SIGNAL(playingDirectoryChanged(const QString&)), this, SLOT(onPlayingDirectoryChanged(const QString&))); // added by wspark 2013.02.08 for ISV 70322
// removed by Dmitry 03.10.13 for ITS0187937
}

CStateManager::~CStateManager()
{
// removed by Dmitry 22.08.13
    if (pictureListModel)
    {
        pictureListModel->clear();
    }
    if (videoListModel)
    {
        videoListModel->clear();
    }
    if (musicListModel)
    {
        musicListModel->clear();
    }
}

void CStateManager::InitialPath()
{
    currentPath = "";
    currentCopyPath = "";
    previousPath = "";
    newPath = "";

    switch ( currentState )
    {
       case eUsbMusicTitle:
       case eJukeMusicTitle:
       {
          this->currentMusicFilter = eSong;
       }
       break;

       case eUsbMusicArtist:
       case eJukeMusicArtist:
       {
          this->currentMusicFilter = eArtist;
       }
       break;

       case eUsbMusicAlbom:
       case eJukeMusicAlbom:
       {
          this->currentMusicFilter = eAlbum;
       }
       break;

       default:
         break;
    }
}

void CStateManager::initListModels()
{
   if ( !isInitialized )
   {
      // MP_LOG << "CStateManager::initListModels()  start" << LOG_ENDL;
      //musicListModel.reset(new AudioListModel()); // removed by Dmitry 23.10.13
      videoListModel.reset(new VideoListModel(this, frontInstance));
      pictureListModel.reset(new PictureListModel(this, frontInstance)); // modified by Dmitry 08.08.13 for ITS0183013
// removed by Dmitry 22.08.13

    // { added by edo.lee 2012.12.06 for statusbar
#if STATUSBAR_NEW
    copyFileManager.reset(new FileOperationsHelper(m_pOSDview,
                                         frontInstance ? USBController::GetFirstInstance() : USBController::GetSecondInstance()));
#else
    copyFileManager.reset(new FileOperationsHelper(appEngine->GetStatusAdapter(),
                                         frontInstance ? USBController::GetFirstInstance() : USBController::GetSecondInstance()));
#endif
// modified by Dmitry 22.08.13
      appEngine->SetTranslator(m_pTranslator);//added by edo.lee 2013.06.22
	  copyFileManager.data()->setTranslator(m_pTranslator);

      connect(this->copyFileManager.data(), SIGNAL(operationDeleteComplete(QString)),
              SLOT(onDeleteComplete(QString)));

      connect(this->copyFileManager.data(), SIGNAL(operationCopyComplete(int,int)), // modified by ravikanth 22-04-13
              SLOT(onFilesChanged()));
      connect(this->copyFileManager.data(), SIGNAL(operationMoveComplete()),
              SLOT(onFilesChanged()));
      connect(this->copyFileManager.data(), SIGNAL(fileSystemChanged()), // added by kihyung 2013.2.17
              SLOT(onFileSystemChanged()));

      connect( appEngine, SIGNAL( signalSetDisplay(int) ),
               this->copyFileManager.data(), SLOT( setDisplay(int) ) );

      connect(this->copyFileManager.data(), SIGNAL(setMoveStatus(bool)),SLOT(onSetMoveStatus(bool)));// add by eunhye 2012.09.21 for CR 9243
      connect(appEngine->GetTrackerExtractorClient(), SIGNAL(MinerStatusFinished()), this, SLOT(onMinerStatusFinished())); // removed by Dmitry 14.08.13 for ITS0183845
      connect(appEngine->GetTrackerExtractorClient(), SIGNAL(updateUIData(QVector<QStringList>*)), this, SLOT(onUpdateUIData(QVector<QStringList>*))); // removed by Dmitry 14.08.13 for ITS0183845
      connect(appEngine->GetTrackerExtractorClient(), SIGNAL(ThumbNailGenerated()), this, SLOT(onThumbNailGenerated())); // added by Dmitry 17.08.13 for ITS0183917
      connect(appEngine, SIGNAL(readCompleted(ETrackerAbstarctor_VolumeType,int)), this, SLOT(onReadCompleted(ETrackerAbstarctor_VolumeType,int))); // added by Dmitry 21.08.13

      connect( appEngine, SIGNAL( setFileOperationStopMedia(int,bool) ),
               this,SLOT( onSetFileOperationStopMedia(int,bool) ) ); // modified for ITS 0214738

      m_pCtxt->setContextProperty( "AudioListModel", GetAudioModel() );
      m_pCtxt->setContextProperty( "VideoListModel", GetVideoModel() );
      m_pCtxt->setContextProperty( "PictureListModel", GetPictureModel() );
      m_pCtxt->setContextProperty( "FileManager", GetFileManager() );


      isInitialized = true;

      emit initializeCompleted();
// removed by Dmitry 14.08.13 for ITS0183845

      // { modified by ruinivbe 2012.08.14  CR 11825
      InitialPath();
      // } modified by ruinivbe
   }
}

BaseListModel* CStateManager::GetAudioModel()
{
    return musicListModel.data();
}

BaseListModel* CStateManager::GetVideoModel()
{
    return videoListModel.data();
}

BaseListModel* CStateManager::GetPictureModel()
{
    return pictureListModel.data();
}

// removed by Dmitry 22.08.13

QString CStateManager::GetCurrentPresentPath()
{
    // MP_LOG << LOG_ENDL;
    QString path;

    switch(currentState)
    {
       case eUsbVideo:
       case eUsbListVideo:
       case eJukeVideo:
       case eJukeListVideo:
       case eUsbPictureShow:
       case eUsbListPicture:
       case eJukePictureShow:
       case eJukeListPicture:
       case eUsbMusicTitle:
       case eUsbMusicArtist:
       case eUsbMusicAlbom:
       case eJukeMusicTitle:
       case eJukeMusicArtist:
       case eJukeMusicAlbom:
            path = currentPath;
            break;

       case eJukePictureCopy:
       case eUsbPictureCopy:
       case eUsbVideoCopy:
       case eJukeVideoCopy:
            path = currentCopyPath;
            break;
       default:
            break;
    };

    // MP_LOG << "CStateManager::GetCurrentPresentPath returned " << path << LOG_ENDL;
    return path;

}

// modified by ravikanth 25.06.13 for ITS 0175213, 0175370
void CStateManager::StateChangeHandler(int index)
{
    StateChangeHandler(index, false);
}

// modified by Dmitry 09.08.13
void CStateManager::StateChangeHandler(int index, bool switchToRoot) // modified by ravikanth 25.06.13 for ITS 0175213, 0175370
{
    MP_LOG << isFrontInstance() << "current state = " << getStringState(currentState) << "; new state = " << getStringState(EAppFileManagerState(index)) << LOG_ENDL; // modified by Dmitry 29.07.13

    if ((EAppFileManagerState)index != currentState)
       newPath.clear();
// added by Dmitry 03.10.13 for ITS0187937
    else
    {
       UncheckAllItems();
       SetCurrentPlayItem();
       emit showContent(true);
    }
    setCurrentState(EAppFileManagerState(index));

    setIsMenuVisible(true);

    switch(index)
    {
       case eUsbVideo:
             StartVideoInfoModelCreating();
             break;

       case eJukeVideo:
             if(switchToRoot) // modified by ravikanth 25.06.13 for ITS 0175213, 0175370
                currentPath = AppFileManagerJukeVideoPath;
             StartVideoInfoModelCreating();
             break;

       case eUsbListVideo:
       case eJukeListVideo:
             StartVideoInfoModelCreating();
             break;

       case eUsbPictureShow:
             StartPictureInfoModelCreating();
             break;

       case eJukePictureShow:
             if(switchToRoot) // modified by ravikanth 25.06.13 for ITS 0175213, 0175370
                currentPath = AppFileManagerJukePicturePath;
             StartPictureInfoModelCreating();
             break;
	   
       case eUsbListPicture:
       case eJukeListPicture:
             StartPictureInfoModelCreating();
             setIsMenuVisible(false); // modified by Dmitry 15.04.13 for ISV79935
             break;

       default:
            break;
    };
	//added for list focus at bottom index 20131020
    emit currentPlayFileNameChanged(currentPlayName);
}
// modified by Dmitry 09.08.13

void CStateManager::StateChangeHandler(QString string)
{
   Q_UNUSED( string )
}

void CStateManager::StateChangeUpdate()
{
   switch ( currentState )
   {
   // { added by wspark 2012.10.15 for CR13822
      case eJukePictureShow:
        StartPictureInfoModelCreating();
        break;
        // added by wspark
      case eUsbPictureShow:
      case eUsbPictureCopy:
      //case eJukePictureShow: // deleted by wspark 2012.10.15 for CR13822
      case eJukePictureCopy:
      case eJukePictureMove:
	  case eJukeListPicture: //added by eugene.seo 2013.03.05
      {
         StartPictureInfoModelCreating();
      }
      break;

       // { added by wspark 2012.10.15 for CR13822
      case eJukeVideo:
        StartVideoInfoModelCreating();
        break;
        // } added by wspark
      case eUsbVideo:
      case eUsbVideoCopy:
      //case eJukeVideo: // deleted by wspark 2012.10.15 for CR13822
      case eJukeVideoCopy:
      case eJukeVideoMove:
      {
         StartVideoInfoModelCreating();
      }
      break;

      default:
         break;
   }
}

bool CStateManager::ShouldChangePath()
{
   bool result = false;

   switch ( currentState )
   {
      case eJukePictureMove:
      case eJukeVideoMove:
      case eJukePictureCopy:
      case eJukeVideoCopy:           
      {
         result = true;
      }
      break;

      default:
         break;
   }

   return result;
}

bool CStateManager::IsMoveMode()
{
   bool result = false;

   switch ( currentState )
   {
      case eJukePictureMove:
      case eJukeVideoMove:
      {
         result = true;
      }
      break;

      default:
         break;
   }

   return result;
}

/*
 * Return value is valid when up is equal "true".
 * Return: true - path changed
 *         false - path not changed
 */
bool CStateManager::PictureFolderChangedHandler(int indexItem, bool up)
{
    //TBD
    if (up)
    {
        //stop UP
        QString rootPath = "";
        if(currentState == eUsbPictureShow || currentState == eUsbListPicture)
        {
            rootPath = AppFileManagerUsbMountPath;
        }
        else if(currentState == eJukePictureShow || currentState == eJukeListPicture)
        {
            rootPath = AppFileManagerJukePicturePath;
        }
        if (!currentPath.compare(rootPath))
            return false;

        //cut last dir
        QFileInfo a(currentPath);
        currentPath = a.path();
    }
    else
    {
        currentPath = currentPath + "/" + pictureListModel->getPictureItem(indexItem)->getItemName();
    }
    StartPictureInfoModelCreating();
    return true;
}

/*
 * Return value is valid when up is equal "true".
 * Return: true - path changed
 *         false - path not changed
 */
bool CStateManager::PictureCopyFolderChangedHandler(int indexItem, bool up)
{
    //TBD
    if (up)
    {
        //stop UP
        QString rootPath = "";
        if(currentState == eUsbPictureCopy)
        {
            rootPath = AppFileManagerUsbMountPath;
        }
        else if(currentState == eJukePictureCopy)
        {
            rootPath = AppFileManagerJukePicturePath;
        }
        if (!currentCopyPath.compare(rootPath))
            return false;

        //cut last dir
        QFileInfo a(currentCopyPath);
        currentCopyPath = a.path();
    }
    else
    {
// modified by Dmitry 22.08.13
        currentCopyPath = currentCopyPath + "/" + pictureListModel->getPictureItem(indexItem)->getItemName();
    }
    StartPictureInfoModelCreating();
    return true;
}

/*
 * Return value is valid when up is equal "true".
 * Return: true - path changed
 *         false - path not changed
 */
bool CStateManager::VideoFolderChangedHandler(int indexItem, bool up)
{
    MP_LOG << "index = " << indexItem << " up = " << up << LOG_ENDL;
    
    if (up)
    {
        //stop UP
        QString rootPath = "";
        if(currentState == eUsbVideo || currentState == eUsbListVideo)
        {
            rootPath = AppFileManagerUsbMountPath;
        }
        else if(currentState == eJukeVideo || currentState == eJukeListVideo)
        {
            rootPath = AppFileManagerJukeVideoPath;
        }
        if (!currentPath.compare(rootPath))
            return false;

        MP_LOG << "currentPath before " << currentPath << LOG_ENDL;

        //cut last dir
        QFileInfo a(currentPath);
        currentPath = a.path();

        MP_LOG << "currentPath after " << currentPath << LOG_ENDL;
    }
    else
    {
        currentPath = currentPath + "/" + videoListModel->getVideoItem(indexItem)->getItemName();
    }
    // FM_LOG << currentVideoPath << LOG_ENDL;
    StartVideoInfoModelCreating();
    return true;
}

/*
 * Return value is valid when up is equal "true".
 * Return: true - path changed
 *         false - path not changed
 */
bool CStateManager::VideoCopyFolderChangedHandler(int indexItem, bool up)
{
    // FM_LOG << "CStateManager:VideoCopyFolderChangedHandler : index = " << indexItem <<
    //           " up = " << up << LOG_ENDL;

    //TBD
    if (up)
    {
        //stop UP
        QString rootPath = "";
        if(currentState == eUsbVideoCopy)
        {
            rootPath = AppFileManagerUsbMountPath;
        }
        else if(currentState == eJukeVideoCopy)
        {
            rootPath = AppFileManagerJukePicturePath;
        }
        if (!currentCopyPath.compare(rootPath))
            return false;

        //cut last dir
        QFileInfo a(currentCopyPath);
        currentCopyPath = a.path();
    }
    else
    {
        currentCopyPath = currentCopyPath + "/" + videoListModel->getVideoItem(indexItem)->getItemName(); // modified by Dmitry 22.08.13
    }
    StartVideoInfoModelCreating();
    return true;
}

bool CStateManager::FolderChangedHandler( int indexItem, bool up )
{
   QString rootPath = "";
   QString * pCurrentPath = NULL;
   BaseListModel * pCurrentModel = NULL;

   // FM_LOG << "CStateManager::FolderChangedHandler() indexItem = " << indexItem << " up = " << up << LOG_ENDL;
   // FM_LOG << "CStateManager::FolderChangedHandler() currentState = " << getStringState( currentState ) << LOG_ENDL;

   switch ( currentState )
   {
      case eUsbPictureShow:
      case eUsbListPicture:
      {
         rootPath = AppFileManagerUsbMountPath;
         pCurrentPath = &currentPath;
         pCurrentModel = pictureListModel.data();
      }
      break;

      case eUsbPictureCopy:
      {
         rootPath = AppFileManagerUsbMountPath;
         pCurrentPath = &currentCopyPath;
         pCurrentModel = pictureListModel.data(); // modified by Dmitry 22.08.13
      }
      break;

      case eJukePictureShow:
      case eJukeListPicture:
      {
         rootPath = AppFileManagerJukePicturePath;
         pCurrentPath = &currentPath;
         pCurrentModel = pictureListModel.data();
      }
      break;

      case eJukePictureCopy:
      case eJukePictureMove:
      {
         rootPath = AppFileManagerJukePicturePath;
         pCurrentPath = &currentCopyPath;
         pCurrentModel = pictureListModel.data(); // modified by Dmitry 22.08.13
      }
      break;

      case eUsbVideo:
      case eUsbListVideo:
      {
         rootPath = AppFileManagerUsbMountPath;
         pCurrentPath = &currentPath;
         pCurrentModel = videoListModel.data();
      }
      break;

      case eUsbVideoCopy:
      {
         rootPath = AppFileManagerUsbMountPath;
         pCurrentPath = &currentCopyPath;
         pCurrentModel = videoListModel.data(); // modified by Dmitry 22.08.13
      }
      break;

      case eJukeVideo:
      case eJukeListVideo:
      {
         rootPath = AppFileManagerJukeVideoPath;
         pCurrentPath = &currentPath;
         pCurrentModel = videoListModel.data();
      }
      break;

      case eJukeVideoCopy:
      case eJukeVideoMove:
      {
         rootPath = AppFileManagerJukeVideoPath;
         pCurrentPath = &currentCopyPath;
         pCurrentModel = videoListModel.data(); // modified by Dmitry 22.08.13
      }
      break;

      default:
      {
         // FM_LOG << "CStateManager::FolderChangedHandler() - incorrect state" << LOG_ENDL;
         return false;
      }
   }

   MP_LOG << "ZZZ rootPath = " << rootPath << LOG_ENDL;
   MP_LOG << "ZZZ pCurrentPath = " << *pCurrentPath << LOG_ENDL;

   if ( pCurrentPath == NULL )
   {
      return false;
   }

   if ( up )
   {
      if ( !pCurrentPath->compare( rootPath ) || !pCurrentPath->contains(rootPath)) // modified - ravikanth 05-03-13
      {
         return false;
      }
      else
      {
         *pCurrentPath = QFileInfo( *pCurrentPath ).path();
      }
   }
   else
   {
      *pCurrentPath = QFileInfo(QDir( *pCurrentPath ), pCurrentModel->getListItem( indexItem )->getItemName() ).filePath();
   }

   // FM_LOG << "FolderChangedHandler pCurrentPath = " << *pCurrentPath << LOG_ENDL;

// modified by Dmitry 11.09.13 for ITS0183775
   NavigatorUpdate(*pCurrentPath);
   StateChangeUpdate();
// modified by Dmitry 11.09.13 for ITS0183775
   return true;
}

EAppFileManagerState CStateManager::GetCurrentState()
{
    return currentState;
}
// { added by edo.lee 2012.12.10 for statusbar 
void CStateManager::setDisplayOSDView()
{
	if(frontInstance)
    	m_pOSDview = appEngine->GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
	else
		m_pOSDview = appEngine->GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
	
    MP_LOG << "m_pOSDview : "<< m_pOSDview << LOG_ENDL;
    copyFileManager->setDeclarativeView(m_pOSDview);
}
// } added by edo.lee

void CStateManager::CopySelectedItems()
{
    BaseListModel* modelCopy;
    QString path;
    QList<QString> list;
    setDisplayOSDView(); // added by edo.lee 2012.12.10 by edo.lee for statusbar
	// modified by ravikanth 10-08-13 for ITS 0183707
    switch (currentState)
    {
        case eJukeVideoCopy:
        case eUsbVideoCopy:
          modelCopy = videoListModel.data();
          path = currentCopyPath;
          m_copyModeType = VIDEO_FILE; // added by wspark 2012.12.14 for copying according to the media type.
          break;
        case eJukePictureCopy:
        case eUsbPictureCopy:
          modelCopy = pictureListModel.data();
          path = currentCopyPath;
          m_copyModeType = IMAGE_FILE; // added by wspark 2012.12.14 for copying according to the media type.
          break;
        case eUsbMusicTitle:
        case eUsbMusicAlbom:
        case eUsbMusicArtist:
          currentCopyPath = AppFileManagerJukeAudioPath;
          CopyOrDeleteMusicSelectedItems(true);
          return;
        case eJukeMusicTitle:
        case eJukeMusicAlbom:
        case eJukeMusicArtist:
          currentCopyPath = AppFileManagerUsbMountPath;
          CopyOrDeleteMusicSelectedItems(true);
          return;
        default:
          return;
    }

    // { added by wspark 2012.09.21 for CR6084
    m_countofCopyingFiles = 0;
    bool folderExist = false;
    // } added by wspark

    for (int i = 0; i < modelCopy->getListCount(); i++)
    {
        if ((modelCopy->getListItem(i))->getItemChecked())
        {
            // { added by wspark 2012.09.21 for CR6084
            if((modelCopy->getListItem(i))->isFolder())
            {
                folderExist = true;
            }
            // } added by wspark
			// { modified by eugene 2012.12.20 for ITS 0149957
			//list.append((modelCopy->getListItem(i))->getItemPath());
            QString copyItemPath = (modelCopy->getListItem(i))->getItemPath();   					
            list.append(copyItemPath);

// modified by Dmitry 02.08.2013 for ITS0181482
            if(eUsbVideoCopy == currentState)
            {
               int uriLength = copyItemPath.lastIndexOf('.', -1);
               copyItemPath = copyItemPath.left(uriLength);
               copyItemPath = copyItemPath.append(".smi");
               QString subUri = copyItemPath;
               if(QFile::exists(copyItemPath.remove( "file://" )))
               {
                   list.append(subUri);
               }
            }
// modified by Dmitry 02.08.2013 for ITS0181482
        }
    }
    // { added by wspark 2012.09.21 for CR6084
    if(folderExist)
        m_countofCopyingFiles = 0;
    else
        m_countofCopyingFiles = list.count();
    // } added by wspark

    // { modified by wspark 2012.12.14 for copying according to the media type.
    //copyFileManager->copy(list, path);
    MP_LOG << "ZZZ previousPath in COPY = " << currentPath << LOG_ENDL;
    previousPath = currentPath;
    copyFileManager->copy(list, path, m_copyModeType, volumeType(currentPath)); // modified by ravikanth 02-09-13
    // } modified by wspark
}

void CStateManager::MoveSelectedItems()
{
   BaseListModel * modelMove = NULL;
   QList<QString> list;
   QString path = "";
   setDisplayOSDView(); // added by edo.lee 2012.12.10 by edo.lee for statusbar

   switch ( currentState )
   {
      case eJukeVideoMove:
      {
         modelMove = videoListModel.data();
         path = currentCopyPath;
      }
      break;

      case eJukePictureMove:
      {
         modelMove = pictureListModel.data();
         path = currentCopyPath;
      }
      break;

      default:
	  	 return;  // modified by eugene 2012.08.26 for static analysis defect #4180
         //break;  // modified by eugene 2012.08.26 for static analysis defect #4180
   }

   // { added by wspark 2012.09.21 for CR6084
   m_countofCopyingFiles = 0;
   bool folderExist = false;
   // } added by wspark

   for (int i = 0; i < modelMove->getListCount(); i++)
   {
       if ((modelMove->getListItem(i))->getItemChecked())
       {
           // { added by wspark 2012.09.21 for CR6084
           if((modelMove->getListItem(i))->isFolder())
           {
               folderExist = true;
           }
           // } added by wspark

           list.append((modelMove->getListItem(i))->getItemPath());
       }
   }

   // { added by wspark 2012.098.21 for CR6084
   if(folderExist)
       m_countofCopyingFiles = 0;
   else
       m_countofCopyingFiles = list.count();
   // } added by wspark

   MP_LOG << "ZZZ previousPath in COPY = " << currentPath << LOG_ENDL;
   previousPath = currentPath;
   copyFileManager->move( list, path );
}

void CStateManager::DeleteSelectedItems()
{
   BaseListModel* modelDelete;
   int modeType = UNSUPPORTED_FILE;
   setDisplayOSDView(); // added by edo.lee 2012.12.10 by edo.lee for statusbar
   switch (currentState)
   {
      case eJukeVideo:
      case eUsbVideo:
      {
         modelDelete = videoListModel.data();
         modeType = VIDEO_FILE;
      }
      break;
      case eJukePictureShow:
      case eUsbPictureShow:
      modelDelete = pictureListModel.data();
      modeType = IMAGE_FILE;
      break;
   case eUsbMusicTitle:
   case eUsbMusicAlbom:
   case eUsbMusicArtist:
     CopyOrDeleteMusicSelectedItems(false);
     return;
   case eJukeMusicTitle:
   case eJukeMusicAlbom:
   case eJukeMusicArtist:
     CopyOrDeleteMusicSelectedItems(false);
     return;
      default:return;
   }

   QStringList deleteList;
	// modified by ravikanth 21.06.13 for ITS 0174571
   for (int i = 0; i < modelDelete->getListCount(); i++)
   {
      if ((modelDelete->getListItem(i))->getItemChecked())
      {
          deleteList.append((modelDelete->getListItem(i))->getItemPath());
      }
   }

   int ret = copyFileManager->deleteFiles(deleteList, modeType); // modified by ravikanth 02-09-13
   
   // { modified by ravikanth 30.06.13 for ITS 0177250
   if(ret != 0) // on success operation
   {
       // @todo - make more intellegent model for performance fixes
       if(deleteList.count() == modelDelete->getListCount())
       {

           deleteAllModelItems();
           //QTimer::singleShot(1,this,SLOT(deleteSelectedModelItems()));
       }
       else
       {
           deleteSelectedModelItems();
       }
   }

//   for (int i = 0; i < modelDelete->getListCount();)
//   {
//      if ((modelDelete->getListItem(i))->getItemChecked())
//      {
//          // deleteList.append((modelDelete->getListItem(i))->getItemPath());
//          /*
//         bool isDeleted = false;

//         FM_LOG << "CStateManager::DeleteSelectedItems itemPath --> " << (modelDelete->getListItem(i))->getItemPath() << LOG_ENDL;
//         FM_LOG << "CStateManager::DeleteSelectedItems itemName --> " << (modelDelete->getListItem(i))->getItemName() << LOG_ENDL;
//         if ( (modelDelete->getListItem(i))->isFolder() == false )
//         {
//            isDeleted = copyFileManager->deleteFile((modelDelete->getListItem(i))->getItemPath());
//         }
//         else
//         {
//            isDeleted = copyFileManager->deleteFolder((modelDelete->getListItem(i))->getItemPath());
//         }

//         modelDelete->checkElement(i, false);

//         if ( isDeleted )
//         {
//            modelDelete->removeDataItem(i);
//         }
//         */
//          modelDelete->checkElement(i, false);
//          modelDelete->removeDataItem(i);
//      }
//      else
//      {
//         i++;
//      }
//   }
	// } modified by ravikanth 30.06.13 for ITS 0177250
	// modified by ravikanth 21.06.13 for ITS 0174571
}

void CStateManager::CopyOrDeleteMusicSelectedItems(bool mode)
{
    QStringList list;
    BaseListModel* model;
    model = musicListModel.data();
    switch (currentState)
    {
       case eUsbMusicTitle:
       case eJukeMusicTitle:
        for (int i = 0; i < model->getListCount();i++)
        {
            if ((model->getListItem(i))->getItemChecked())
            {
                list.append((model->getListItem(i))->getItemPath());
            }
        }
        if ( mode == false)
        {
            DeleteMusicFilesByPathList(&list);
        }
        else
        {
            CopyMusicFilesByPathList(&list);
        }
       break;

       case eUsbMusicArtist:
       case eJukeMusicArtist:
       case eUsbMusicAlbom:
       case eJukeMusicAlbom:
       {

       }
       break;

       default:
          break;
    }

    if ( mode == false)
    {
        for (int i = 0; i < model->getListCount();)
        {
            if (model->getListItem(i)->getItemChecked())
            {
                model->checkElement(i, false);
                model->removeDataItem(i);
            }
            else
            {
                i++;
            }
        }
    }
}

void CStateManager::DeleteMusicFilesByPathList(QList<QString>* list)
{
    copyFileManager->deleteFiles(*list);
}

void CStateManager::CopyMusicFilesByPathList(QList<QString>* list)
{
    QString path;
    switch (currentState)
    {
        case eUsbMusicTitle:
        case eUsbMusicArtist:
        case eUsbMusicAlbom:
            path = AppFileManagerJukeAudioPath;
        break;

        case eJukeMusicTitle:
        case eJukeMusicArtist:
        case eJukeMusicAlbom:
            path = AppFileManagerUsbMountPath;
        break;

        default:
        break;

    }
    copyFileManager->copy(*list, path);
}

void CStateManager::AudioRequestBySetParamCompleted(QList<QString>* info, int requesType, int codeError, QVariant userData)
{
   Q_UNUSED( requesType )

    if (!codeError && info != NULL)
    {
        if (userData.toBool()) //copy TAG
        {


        }
        else //delete
        {
            if (info)
            {
                DeleteMusicFilesByPathList(info);
            }
        }
    }

}

void CStateManager::deleteFolder( const QString & value )
{
   QStringList deleteList;

   deleteList.append( value );
   // modified by ravikanth 02-09-13
   int modeType = UNSUPPORTED_FILE;
   switch ( currentState )
   {
       case eJukeVideo:
       case eUsbVideo:
       {
           modeType = VIDEO_FILE;
       }
           break;
       case eJukePictureShow:
       case eUsbPictureShow:
       {
           modeType = IMAGE_FILE;
       }
           break;
       default:
           break;
   }

   int ret = copyFileManager->deleteFiles(deleteList, modeType); // modified by ravikanth 21.06.13 for ITS 0174571

   // modified by ravikanth 21.06.13 for ITS 0174571
   if(ret != 0) // on success operation 
   {
      BaseListModel *model = NULL;

      switch ( currentState )
      {
         case eJukeVideo:
         case eUsbVideo:
         {
            model = videoListModel.data();
         }
         break;
         case eJukePictureShow:
         case eUsbPictureShow:
         {
            model = pictureListModel.data();
         }
         break;

         default:
            break;
      }
      
      if ( model != NULL ) //fix coverity issue
      {
          for ( int i = 0; i < model->getListCount(); i++ )
          {
              if ( !model->getListItem(i)->getItemPath().compare( value ) )
              {
                  model->removeDataItem(i);
              }
          }
      }
   }
}

void CStateManager::renameFolder( const QString & src, const QString & dst )
{
   copyFileManager->renameFolder( src, dst );
   StateChangeHandler( currentState );
}

void CStateManager::renameFile( const QString & src, const QString & dst )
{
   copyFileManager->renameFile( src, dst );
   StateChangeHandler( currentState );
}

void CStateManager::setCurrentPath(const QString & path)
{
    appEngine->setFilePath(path, isFrontInstance());
}

bool CStateManager::FolderUp()
{
   // FM_LOG << "CStateManager::FolderUp" << LOG_ENDL;

   return FolderChangedHandler( 0, true );
}

// modified by Dmitry 22.08.13
void CStateManager::CreateFolder( QString path )
{
   switch (currentState)
   {
      case eUsbVideoCopy:
      case eJukeVideoCopy:
      case eJukeVideoMove:
        {
          QDir dir( FileManagerEngine::GetValidFilePath(currentCopyPath) );
          dir.mkdir( path ); // modified by Dmitry 17.08.13
          VideoModelItem item;
          item.setItemId(videoListModel->getListCount());
          item.setFolder(QBool(true));
          item.setItemPath(currentCopyPath + "/"+ path);
          item.setItemChecked(QBool(false));
          item.setItemName(path);
          videoListModel->addDataItem(item);

          emit updateModelFinish();
          break;
        }
      case eUsbPictureCopy:
      case eJukePictureCopy:
      case eJukePictureMove:
        {
          QDir dir( FileManagerEngine::GetValidFilePath(currentCopyPath) );
          dir.mkdir( path ); // modified by Dmitry 17.08.13
          PictureModelItem item;
          item.setItemId(pictureListModel->getListCount());
          item.setFolder(QBool(true));
          item.setItemPath(currentCopyPath + "/"+ path);
          item.setItemChecked(QBool(false));
          item.setItemName(path);
          pictureListModel->addDataItem(item);

          emit updateModelFinish();
          break;
        }
      default:break;
   }
}
// modified by Dmitry 22.08.13

// modified by ravikanth 25.06.13 for ITS 0175213, 0175370
void CStateManager::setDevice( int dev )
{
    setDevice( dev, false);
}

void CStateManager::setDevice( int dev, bool switchToRoot ) // modified by ravikanth 25.06.13 for ITS 0175213, 0175370
{
   static EAppFileManagerState states[][2] = {
      { eUsbMusicTitle,  eJukeMusicTitle },  // 0  eUsbMusicTitle
      { eUsbMusicArtist, eJukeMusicArtist }, // 1  eUsbMusicArtist
      { eUsbMusicAlbom,  eJukeMusicAlbom },  // 2  eUsbMusicAlbom
      { eUsbVideo,       eJukeVideo },       // 3  eUsbVideo
      { eUsbVideoCopy,   eJukeVideoCopy },   // 4  eUsbVideoCopy
      { eUsbPictureShow, eJukePictureShow }, // 5  eUsbPictureShow
      { eUsbPictureCopy, eJukePictureCopy }, // 6  eUsbPictureCopy
      { eUsbMusicTitle,  eJukeMusicTitle },  // 7  eJukeMusicTitle
      { eUsbMusicArtist, eJukeMusicArtist }, // 8  eJukeMusicArtist
      { eUsbMusicAlbom,  eJukeMusicAlbom },  // 9  eJukeMusicAlbom
      { eUsbVideo,       eJukeVideo },       // 10 eJukeVideo
      { eUsbVideoCopy,   eJukeVideoCopy },   // 11 eJukeVideoCopy
      { eUsbPictureShow, eJukePictureShow }, // 12 eJukePictureShow
      { eUsbPictureCopy, eJukePictureCopy }, // 13 eJukePictureCopy
      { eUsbListVideo,   eJukeListVideo },   // 14 eJukeListVideo
      { eUsbListVideo,   eJukeListVideo },   // 15 eUsbListVideo
      { eUsbListPicture, eJukeListPicture }, // 16 eJukeListPicture
      { eUsbListPicture, eJukeListPicture }, // 17 eUsbListPicture
   };

   if ( states[currentState][dev] > eUsbListPicture )
   {
      // FM_LOG << "invalid state : " << states[currentState][dev] << LOG_ENDL;
      return;
   }

   // FM_LOG << "setDevice : " << getStringState(currentState) << " --> " <<
   //            getStringState(states[currentState][dev]) << LOG_ENDL;
   int new_state = states[currentState][dev];
   if (new_state != currentState)
        StateChangeHandler(new_state, switchToRoot); // modified by ravikanth 25.06.13 for ITS 0175213, 0175370
}

void CStateManager::setFileType( int type )
{
   static EAppFileManagerState states[][3] = {
      { eUsbMusicAlbom,  eUsbVideo,  eUsbPictureShow },  // 0  eUsbMusicTitle
      { eUsbMusicAlbom,  eUsbVideo,  eUsbPictureShow },  // 1  eUsbMusicArtist
      { eUsbMusicAlbom,  eUsbVideo,  eUsbPictureShow },  // 2  eUsbMusicAlbom
      { eUsbMusicAlbom,  eUsbVideo,  eUsbPictureShow },  // 3  eUsbVideo
      { eUsbMusicAlbom,  eUsbVideo,  eUsbPictureShow },  // 4  eUsbVideoCopy
      { eUsbMusicAlbom,  eUsbVideo,  eUsbPictureShow },  // 5  eUsbPictureShow
      { eUsbMusicAlbom,  eUsbVideo,  eUsbPictureShow },  // 6  eUsbPictureCopy
      { eJukeMusicAlbom, eJukeVideo, eJukePictureShow }, // 7  eJukeMusicTitle
      { eJukeMusicAlbom, eJukeVideo, eJukePictureShow }, // 8  eJukeMusicArtist
      { eJukeMusicAlbom, eJukeVideo, eJukePictureShow }, // 9  eJukeMusicAlbom
      { eJukeMusicAlbom, eJukeVideo, eJukePictureShow }, // 10 eJukeVideo
      { eJukeMusicAlbom, eJukeVideo, eJukePictureShow }, // 11 eJukeVideoCopy
      { eJukeMusicAlbom, eJukeVideo, eJukePictureShow }, // 12 eJukePictureShow
      { eJukeMusicAlbom, eJukeVideo, eJukePictureShow },  // 13 eJukePictureCopy
      { eJukeMusicList,   eJukeListVideo,  eJukeListPicture}, // 14 eJukeListVideo
      { eUsbMusicList,    eUsbListVideo,   eUsbListPicture},  // 15 eUsbListVideo
      { eJukeMusicList,   eJukeListVideo,  eJukeListPicture}, // 16 eJukeListPicture
      { eUsbMusicList,    eUsbListVideo,   eUsbListPicture},   // 17 eUsbListPicture
      { eJukeMusicList,   eJukeListVideo,  eJukeListPicture}, // 16 eUsbMusicList
      { eUsbMusicList,    eUsbListVideo,   eUsbListPicture}   // 17 eJukeMusicList
   };

   if ( currentState == eInvalidState )
   {
      // FM_LOG << "CStateManager::setFileType : invalid state" << LOG_ENDL;
      return;
   }

   EAppFileManagerState new_state = states[currentState][type];

   if ( new_state > eUsbListPicture ) //TBD should use MAX_STATE
   {
      // FM_LOG << "CStateManager::setFileType : invalid state : " << new_state << LOG_ENDL;
      return;
   }

   if ( new_state == eUsbMusicTitle ||
        new_state == eUsbMusicArtist ||
        new_state == eUsbMusicAlbom )
   {
      switch ( this->currentMusicFilter )
      {
         case eArtist:
         {
            new_state = eUsbMusicArtist;
         }
         break;

         case eAlbum:
         {
            new_state = eUsbMusicAlbom;
         }
         break;

         case eSong:
         {
            new_state = eUsbMusicTitle;
         }
         break;

         default:
         {
            // FM_LOG << "CStateManager::setFileType  incorrect this->filter" << LOG_ENDL;
         }
      }
   }

   if ( new_state == eJukeMusicTitle ||
        new_state == eJukeMusicArtist ||
        new_state == eJukeMusicAlbom )
   {
      switch ( this->currentMusicFilter )
      {
         case eArtist:
         {
            new_state = eJukeMusicArtist;
         }
         break;

         case eAlbum:
         {
            new_state = eJukeMusicAlbom;
         }
         break;

         case eSong:
         {
            new_state = eJukeMusicTitle;
         }
         break;

         default:
         {
            // FM_LOG << "CStateManager::setFileType  incorrect this->filter" << LOG_ENDL;
         }
      }
   }

   // FM_LOG << "setFileType : " << getStringState(currentState) << " --> " <<
   //            getStringState(new_state) << LOG_ENDL;

   StateChangeHandler(new_state);
}

void CStateManager::setMusicFilter( int filter )
{
   static EAppFileManagerState states[][3] = {
      { eUsbMusicArtist,  eUsbMusicAlbom,  eUsbMusicTitle },  // 0  eUsbMusicTitle
      { eUsbMusicArtist,  eUsbMusicAlbom,  eUsbMusicTitle },  // 1  eUsbMusicArtist
      { eUsbMusicArtist,  eUsbMusicAlbom,  eUsbMusicTitle },  // 2  eUsbMusicAlbom
      { eUsbMusicArtist,  eUsbMusicAlbom,  eUsbMusicTitle },  // 3  eUsbVideo
      { eUsbMusicArtist,  eUsbMusicAlbom,  eUsbMusicTitle },  // 4  eUsbVideoCopy
      { eUsbMusicArtist,  eUsbMusicAlbom,  eUsbMusicTitle },  // 5  eUsbPictureShow
      { eUsbMusicArtist,  eUsbMusicAlbom,  eUsbMusicTitle },  // 6  eUsbPictureCopy
      { eJukeMusicArtist, eJukeMusicAlbom, eJukeMusicTitle }, // 7  eJukeMusicTitle
      { eJukeMusicArtist, eJukeMusicAlbom, eJukeMusicTitle }, // 8  eJukeMusicArtist
      { eJukeMusicArtist, eJukeMusicAlbom, eJukeMusicTitle }, // 9  eJukeMusicAlbom
      { eJukeMusicArtist, eJukeMusicAlbom, eJukeMusicTitle }, // 10 eJukeVideo
      { eJukeMusicArtist, eJukeMusicAlbom, eJukeMusicTitle }, // 11 eJukeVideoCopy
      { eJukeMusicArtist, eJukeMusicAlbom, eJukeMusicTitle }, // 12 eJukePictureShow
      { eJukeMusicArtist, eJukeMusicAlbom, eJukeMusicTitle }, // 13 eJukePictureCopy
      { eJukeMusicList,   eJukeMusicList,  eJukeMusicList},   // 14 eJukeListVideo
      { eUsbMusicList,    eUsbMusicList,   eUsbMusicList},    // 15 eUsbListVideo
      { eJukeMusicList,   eJukeMusicList,  eJukeMusicList},   // 16 eJukeListPicture
      { eUsbMusicList,    eUsbMusicList,   eUsbMusicList}     // 17 eUsbListPicture
      // eUsbListPicture
   };

   this->currentMusicFilter = filter;

   if ( states[currentState][filter] > eUsbListPicture )
   {
      // FM_LOG << "CStateManager::setMusicFilter : invalid state : " << states[currentState][filter] << LOG_ENDL;
      return;
   }

   // FM_LOG << "setMusicFilter : " << getStringState(currentState) << " --> " <<
   //            getStringState(states[currentState][filter]) << LOG_ENDL;

   StateChangeHandler(states[currentState][filter]);
}

void CStateManager::setCopyView( bool isCopyView )
{
   int index = isCopyView ? 1 : 0;
   static EAppFileManagerState states[][2] = {
      { eInvalidState,    eInvalidState },    // 0  eUsbMusicTitle
      { eInvalidState,    eInvalidState },    // 1  eUsbMusicArtist
      { eInvalidState,    eInvalidState },    // 2  eUsbMusicAlbom
      { eUsbVideo,        eJukeVideoCopy },   // 3  eUsbVideo
      { eUsbVideo,        eJukeVideoCopy },   // 4  eUsbVideoCopy
      { eUsbPictureShow,  eJukePictureCopy }, // 5  eUsbPictureShow
      { eUsbPictureShow,  eUsbPictureCopy },  // 6  eUsbPictureCopy
      { eInvalidState,    eInvalidState },    // 7  eJukeMusicTitle
      { eInvalidState,    eInvalidState },    // 8  eJukeMusicArtist
      { eInvalidState,    eInvalidState },    // 9  eJukeMusicAlbom
      { eJukeVideo,       eUsbVideoCopy },    // 10 eJukeVideo
    //{ modified by yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
    //{ eJukeVideo,       eUsbVideoCopy },
      { eUsbVideo,       eUsbVideoCopy },     // 11 eJukeVideoCopy
      { eJukePictureShow, eUsbPictureCopy },  // 12 eJukePictureShow
    //{ eJukePictureShow, eUsbPictureCopy }
      { eUsbPictureShow, eUsbPictureCopy }    // 13 eJukePictureCopy
    //} modified by yungi 2013.2.7 for UX Scenario 5. File Copy step reduction

   };

   if ( states[currentState][index] == eInvalidState ||
        states[currentState][index] > eJukePictureCopy )
   {
      // FM_LOG << "invalid state : " << states[currentState][isCopyView] << LOG_ENDL;
      return;
   }

   // FM_LOG << "setCopyView : " << getStringState(currentState) << " --> " <<
   //            getStringState(states[currentState][index]) << LOG_ENDL;
// modified by Dmitry 09.08.13
   int new_state = states[currentState][index];
   if (new_state != currentState)
   {
      switch (new_state)
      {
      case eJukePictureCopy:
      case eUsbPictureCopy:
      case eJukePictureMove:
           currentCopyPath = AppFileManagerJukePicturePath;
           break;
      case eUsbVideoCopy:
      case eJukeVideoCopy:
      case eJukeVideoMove:
           currentCopyPath = AppFileManagerJukeVideoPath;
           break;
      default:
         break;
      }
      setCurrentState((EAppFileManagerState)new_state);
   }
// modified by Dmitry 09.08.13
}

void CStateManager::breakCopyMode()
{
    switch(currentState)
    {
        case eUsbVideoCopy:
           setCurrentState(eJukeVideo);
           break;
        case eJukeVideoCopy:
           setCurrentState(eUsbVideo);
           break;
        case eUsbPictureCopy:
           setCurrentState(eJukePictureShow);
           break;
        case eJukePictureCopy:
           setCurrentState(eUsbPictureShow);
           break;
        default:
           break;
    }
    // FM_LOG << "CStateManager::breakCopyMode " << LOG_ENDL;
    //filesAreChecked
}

// { added by wspark 2012.10.04 for sanity AK339
// { changed by junam 2012.12.01 for CR16077
//void CStateManager::cancelCopyMode()
bool CStateManager::cancelCopyMode()
{
    if(m_entryState == currentState)
        return false;
// } changed by junam
    switch(currentState)
    {
        case eUsbVideoCopy:
           setCurrentState(eJukeVideo);
           break;
        case eJukeVideoCopy:
           setCurrentState(eUsbVideoCopy);
           break;
        case eUsbPictureCopy:
           setCurrentState(eJukePictureShow);
           break;
        case eJukePictureCopy:
           setCurrentState(eUsbPictureCopy);
           break;
        default:
           break;
    }

    return true; // added by junam 2012.12.01 for CR16077
}
// } added by wspark

void CStateManager::setMoveMode( bool isMoveMode )
{
   EAppFileManagerState new_state = eInvalidState;

   // FM_LOG << "CStateManager::setMoveMode() isMoveMode = " << isMoveMode << LOG_ENDL;

   if ( isMoveMode )
   {
      switch( currentState )
      {
         case eJukeVideo:
         {
            new_state = eJukeVideoMove;
            checkedPath = currentPath;
            PrepareCheckedList( videoListModel.data() );
         }
         break;

         case eJukePictureShow:
         {
            new_state = eJukePictureMove;
            checkedPath = currentPath;
            PrepareCheckedList( pictureListModel.data() );
         }
         break;

         default:
         {
            // FM_LOG << "CStateManager::setMoveMode() invalid state" << LOG_ENDL;
            return;
         }
      }
   }
   else
   {
      checkedList.clear();
      switch ( currentState )
      {
         case eJukeVideoMove:
         {
            new_state = eJukeVideo;
         }
         break;

         case eJukePictureMove:
         {
            new_state = eJukePictureShow;
         }
         break;

         default:
         {
            // FM_LOG << "CStateManager::setMoveMode() invalid state" << LOG_ENDL;
            return;
         }
      }
   }

   if ( new_state != eInvalidState )
   {
      setCurrentState(new_state);
      StateChangeUpdate();
   }
}

void CStateManager::breakMoveMode()
{
   checkedList.clear();
   switch ( currentState )
   {
      case eJukeVideoMove:
      case eJukeVideo:
      {
         setCurrentState(eJukeVideo);
      }
      break;

      case eJukePictureMove:
      case eJukePictureShow:
      {
         currentPath = AppFileManagerJukePicturePath; // added by wspark 2012.12.05 for SANITY_CM_AJ533
         setCurrentState(eJukePictureShow);
      }
      break;

      default:
         return;
   }

   emit currentPathChanged();
   //StateChangeUpdate();
}

/////////////////////////////////////////////////////////////////////////////////////
////////////////////////// VIDEO ////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// modified by Dmitry 22.08.13
void CStateManager::StartVideoInfoModelCreating()
{
   MP_CRITICAL << "ZZZ newPath = " << newPath << LOG_ENDL;
   MP_CRITICAL << "ZZZ currentPath = " << currentPath << LOG_ENDL;
   if (newPath != currentPath)
   {
      newPath = currentPath;
      setCurrentPath(currentPath);

      videoListModel->cancel();
      videoListModel->clear();
      folderQueue.clear();

      if ( currentState == eUsbVideo ||
           currentState == eJukeVideo ||
           currentState == eUsbListVideo ||
           currentState == eJukeListVideo )
      {

		 // modified by ravikanth for ITS 0184642
         if(editModeLaunchFromAVP())
         {
             videoListModel->setCurrentReference(true);
         }
      }
      else if( currentState == eUsbVideoCopy ||
               currentState == eJukeVideoCopy ||
               currentState == eJukeVideoMove )
      {
         MP_LOG << "ZZZ previousPath = " << previousPath << LOG_ENDL;
         currentPath = previousPath;
      }
      else
      {
         return;
      }

      folderQueue.enqueue( currentPath );
      checkQueue();

      SetCurrentPlayItem();
      NavigatorUpdate(currentPath);
   }
   else
   {
   		// modified by ravikanth for ITS 0184642
       if ( currentState == eUsbVideo ||
            currentState == eJukeVideo ||
            currentState == eUsbListVideo ||
            currentState == eJukeListVideo )
       {
          if(editModeLaunchFromAVP())
          {
              videoListModel->setCurrentReference(false);
          }
       }
   }
}

/////////////////////////////////////////////////////////////////////////////////////
////////////////////////// PICTURE //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void CStateManager::StartPictureInfoModelCreating()
{
    MP_CRITICAL << "newPath = " << newPath << "currentPath = " << currentPath << LOG_ENDL;

   if (newPath != currentPath)
   {
      newPath = currentPath;
      setCurrentPath(currentPath);

      pictureListModel->cancel();
      pictureListModel->clear();
      folderQueue.clear();

      if ( currentState == eUsbPictureShow ||
           currentState == eJukePictureShow ||
           currentState == eUsbListPicture ||
           currentState == eJukeListPicture )
      {
         if(editModeLaunchFromAVP())
         {
             pictureListModel->setCurrentReference(true);
         }
      }
      else if( currentState == eUsbPictureCopy ||
               currentState == eJukePictureCopy ||
               currentState == eJukePictureMove )
      {
         currentPath = previousPath;
      }
      else
      {
         return;
      }

      folderQueue.enqueue( currentPath );

      checkQueue();
      NavigatorUpdate(currentPath);
   }
   else
   {
   		// modified by ravikanth for ITS 0184642
       if ( currentState == eUsbPictureShow ||
            currentState == eJukePictureShow ||
            currentState == eUsbListPicture ||
            currentState == eJukeListPicture )
       {
          if(editModeLaunchFromAVP())
          {
              pictureListModel->setCurrentReference(false);
          }
       }
   }
}
// modified by Dmitry 22.08.13

///////////////////////////////////////// AUDIO ////////////////////////////////
void CStateManager::StartAudioInfoModelCreatingWithoutNavigation()
{
    musicListModel->clear();
    bool isUsb = false;
    uint type;
    //TBD Navigation Logic

    switch (currentState)
    {
      case eUsbMusicTitle:
        isUsb = true;
        type = eMusicAllTitleRequest;
        break;
      case eUsbMusicArtist:
        isUsb = true;
        type = eMusicAllArtistRequest;
        break;
      case eUsbMusicAlbom:
        isUsb = true;
        type = eMusicAllAlbomRequest;
        break;
      case eJukeMusicTitle:
        type = eMusicAllTitleRequest;
        break;
      case eJukeMusicArtist:
        type = eMusicAllArtistRequest;
        break;
      case eJukeMusicAlbom:
        type = eMusicAllAlbomRequest;
        break;
      default:
        return;
    }
    QVariant temp(isUsb);
    //mediaDataProvider->loadAudioData(EMediaDataRequestType(type), currentPath, temp);
}

void CStateManager::AudioRequestCompleted(QList<AudioModelItem>* info, int requesType, int codeError, QVariant userData)
{
    if (!codeError)
    {
        if (requesType != eMusicAlbumsFileListRequest &&
            requesType != eMusicArtistsFileListRequest)
        {
            //TBD add verify
            if (info)
            {
                int count = info->count();
                for (int i = 0; i < count; i++)
                {
                   musicListModel->addDataItem(info->at(i));
                }
                SetCurrentPlayItem();
            }
        }
        else if (info)
        {
            if (userData == true)
            {
                QStringList items;
                for (int i = 0; i < info->size(); i++)
                {
                    items.append(info->at(i).getItemPath());
                }
                copyFileManager->copy(items, currentCopyPath);

                musicListModel->clearCheckedElements();

                int newDevice = 1 - device();
                emit initState(newDevice,
                               this->stateTable[this->currentState][1],
                               this->stateTable[this->currentState][2],
                               this->stateTable[this->currentState][3]);
                setDevice(newDevice);
            }
            else
            {
                QStringList deleteList;
                for (int i = 0; i < info->size(); i++)
                {
                    deleteList.append(info->at(i).getItemPath());
                }

                copyFileManager->deleteFiles(deleteList);
            }
        }
    }

}

// modified by Dmitry 09.08.13
/////////////////////////////// START APP//////////
void CStateManager::AppPostEventHandler(int state, QString currentPath, QString fullFileName,
                                        int screen)
{
    // modified by Dmitry 17.08.13
    if ((isFrontInstance() && screen != AppEngineQMLConstants::SCREEN_FRONT) ||
        (!isFrontInstance() && screen != AppEngineQMLConstants::SCREEN_REAR))
    {
        return;
    }
    // { modified by ruinivbe

    m_entryState = EAppFileManagerState(state); // added by junam 2012.12.01 for CR16077
    
    currentPath = currentPath.remove("file://"); // modified by Dmitry 25.07.13
    QString fullPathName = "";

    fullPathName.append(currentPath).append("/").append(fullFileName);

    MP_LOG << "ZZZ fullPathName= " << fullPathName << "; currentPath = " << currentPath << LOG_ENDL;

// removed by Dmitry 29.07.13
    initListModels();
    SetFileOperationModeType(); // modified for ITS 0212431

    this->currentPath = currentPath;
    currentPlayPath = fullPathName; // added by Dmitry 23.07.13 for ITS0181253

    setCurrentPath(currentPath);
	
	//modified by ravikanth 27.06.13 for ITS 0176629
    if(m_entryState == eUsbVideo || m_entryState == eUsbPictureShow)
    {
        //FM_LOG<<"ZZZ CStateManager::AppPostEventHandler USBPath"<<AppFileManagerUsbMountPath;
        resetUSBMountPath(this->currentPath);
    }

    if(fullFileName.count() > 0)
    {
        SaveCurrentPlayDataByPathWithoutUpdateUI(fullPathName,EAppFileManagerState(state)); // modified by Dmitry 29.07.13
    }

    setupInitialQmlState(state);
    StateChangeHandler(EAppFileManagerState(state));
}

// removed by Dmitry 03.10.13 for ITS0187937

void CStateManager::setupInitialQmlState(int state)
{
    emit initState(stateTable[state][0],
                   stateTable[state][1],
                   stateTable[state][2],
                   stateTable[state][3]);
}
// modified by Dmitry 09.08.13

int CStateManager::device()
{
    return stateTable[this->currentState][0];
}

int CStateManager::fileType()
{
    return stateTable[this->currentState][1];
}

int CStateManager::musicFilter()
{
    return stateTable[this->currentState][2];
}

int CStateManager::appMode()
{
   return stateTable[this->currentState][3];
}

void CStateManager::onUSBMounted(const QString& path, const int nPort)
{
    int nPathIdx = m_tUSBMountPath.lastIndexOf(path);

    if(nPathIdx < 0 && nPathIdx != nPort) {
        m_tUSBMountPath += path;
        m_tUSBPortList  += nPort;
    }

    AppFileManagerUsbMountPath = path;

// removed by Dmitry 16.10.13
}

void CStateManager::onUSBRemoved(const int nPort)
{
    // FM_LOG << " mount port : " << nPort << LOG_ENDL;

    if(m_tUSBPortList.count() > 0) 
    {
        int nIndex = m_tUSBPortList.lastIndexOf(nPort);
        if(nIndex >= 0) {
            m_tUSBMountPath.removeAt(nIndex);
            m_tUSBPortList.removeAt(nIndex);
        }
        else {
            // FM_LOG << " can not remove port " << nPort;
        }
    }
    
    switch(this->currentState)
    {
       case eJukeVideoCopy:
	   // modified by ravikanth 29-08-13 for ITS 0183566
        //AppFileManagerUsbMountPath = AppFileManagerNoUsb;
        resetMountPath();
        setCurrentState(eUsbVideo);
        break;

       case eJukePictureCopy:
        //AppFileManagerUsbMountPath = AppFileManagerNoUsb;
        resetMountPath();
        setCurrentState(eUsbPictureShow);
        break;

       case eUsbVideo:
       case eUsbVideoCopy:
       case eUsbListVideo:
	   // modified by ravikanth 27.06.13 for ITS 0176629
        resetMountPath();
        newPath.clear(); // added by Dmitry 07.08.13
        break;

       case eUsbPictureShow:
       case eUsbPictureCopy:       
       case eUsbListPicture:
        newPath.clear(); // added by Dmitry 07.08.13
        break;

       default:
        return;
    }

// removed by Dmitry 07.08.13

    emit usbRemoved(nPort); // modified by eugene.seo 2013.05.29
}
// } modified by kihyung

void CStateManager::resetMountPath()
{
    if(m_tUSBMountPath.count() == 1)
    {
        // if second usb is still avaliable copy its path instead of copying null path
        AppFileManagerUsbMountPath = m_tUSBMountPath.at(0);
    }
    else
    {
        AppFileManagerUsbMountPath = AppFileManagerNoUsb;
    }
}

void CStateManager::updateState()
{
   // FM_LOG << "CStateManager::updateState  init = " << isInitialized << LOG_ENDL;

   if ( isInitialized )
   {
      StateChangeHandler(this->currentState);
   }
}

void CStateManager::SetCurrentPlayItem(bool avoidFocusUpdate)
{
    QString name;
    BaseListModel* currentModel;

    if ((currentState == eUsbMusicTitle) || (currentState == eUsbMusicAlbom) || (currentState == eUsbMusicArtist))
    {
        if (!(this->currentPlayPath.contains(AppFileManagerUsbMountPath)))
            return;
    }
    else if (((currentState == eJukeMusicTitle) || (currentState == eJukeMusicAlbom) || (currentState == eJukeMusicArtist))
             && !(this->currentPlayPath.contains(AppFileManagerJukeAudioPath)))
    {
        if (!(this->currentPlayPath.contains(AppFileManagerJukeAudioPath)))
            return;
    }


    switch(currentState)
    {
        case eUsbMusicTitle:
        case eJukeMusicTitle:
            name = currentPlayName;
            currentModel = musicListModel.data();
        break;
        case eUsbMusicArtist:
        case eJukeMusicArtist:
            name = currentPlayAudioArtist;
            currentModel = musicListModel.data();
            break;
        case eUsbMusicAlbom:
        case eJukeMusicAlbom:
            name = currentPlayAudioAlbum;
            currentModel = musicListModel.data();
            break;
        case eUsbVideo:
        case eJukeVideo:
        case eUsbListVideo:
        case eJukeListVideo:
        case eJukeVideoCopy: // modified by ravikanth 06-04-13 ISV 78225
            name = currentPlayName;
            currentModel = videoListModel.data();
            break;
// added by Dmitry 08.08.13 for ITS0183013
        case eJukePictureShow:
        case eUsbPictureShow:
            name = currentPlayName;
            currentModel = pictureListModel.data();
            break;
// added by Dmitry 08.08.13 for ITS0183013
        default: return;

    }

	// modified by ravikanth 22.06.13 for ITS 175671, 175684, 175498
    QString filePath = "";
    if(currentPlayPath.startsWith("file://"))
    {
        filePath = currentPlayPath.mid(7) + QString("/") + name;
    }
    else
    {
        filePath = currentPlayPath + QString("/") + name;
    }

    //update model
    int count = currentModel->getListCount();
    for (int i = 0; i < count; i ++)
    {
       QModelIndex index = currentModel->index(i);

        if ( currentModel->getListItem(i)->getItemPlay() == true)
        {
           currentModel->setData(index, false, BaseListModel::BasePlayed);
        }
//{ added by yongkyun.lee 20130204 for : ISV 70321
//          if ( !( currentModel->getListItem(i)->getItemName().compare(name)) )

        // modified by ravikanth 22.06.13 for ITS 175671, 175684, 175498
        QString modelPath = "";
        if(currentModel->getListItem(i)->getItemPath().startsWith("file://"))
        {
            modelPath = currentModel->getListItem(i)->getItemPath().mid(7);
        }
        else
        {
            modelPath = currentModel->getListItem(i)->getItemPath();
        }

        if( ( !( currentModel->getListItem(i)->getItemName().compare(name)) &&
                !( modelPath.compare(filePath) ) ) || ( filePath.startsWith(modelPath) && ( filePath.at(modelPath.count()) == '/' ) ) ) // modified by ravikanth 23.06.13
            //( currentPlayPath.contains(currentModel->getListItem(i)->getItemPath())))
//} added by yongkyun.lee 20130204 
        {
            currentModel->setData(index, true, BaseListModel::BasePlayed);
			// modified by ravikanth for ITS 0184642
            if(editModeLaunchFromAVP())
            {
                // modified by Dmitry 04.10.13 for ITS0187937
                currentModel->setFileURLCount(1);
                currentModel->setData(index, true, BaseListModel::BaseCheckedRole);
            }
        }
    }
    appEngine->sendTouchCleanUpForApps();// added by suilyou 20130930 ITS 0186169

	// modified by ravikanth 03-10-13 new spec change, don't update highlight when current play item changed while in list
    if(!avoidFocusUpdate)
        emit currentPlayFileNameChanged(currentPlayName);
}

void CStateManager::AppPostEventCurrentPlayChanged(QString param, int type,
                                                   int screen)
{
    if (isFrontInstance() && screen != AppEngineQMLConstants::SCREEN_FRONT)
    {
        return;
    }
    // FM_LOG<<"CStateManager::AppPostEventCurrentPlayChanged" << LOG_ENDL;
    SaveCurrentPlayData(param, type);
}

void CStateManager::SaveCurrentPlayData(QString param, int type)
{

    QFileInfo temp(param);

    if (type == eAudioFileChanged)
    {
        QString previusArtist = this->currentPlayAudioArtist;
        this->currentPlayName = temp.fileName();
        this->currentPlayPath = temp.filePath();
        AppFileManagerMusicData data; //= loadAudioDataByPathFile(param);
        this->currentPlayAudioArtist = data.artist;
        this->currentPlayAudioAlbum = data.album;
//modified by aettie 2013.03.28 for static defect 13125
//        if ((currentState == eUsbMusicTitle || currentState == eUsbMusicTitle) ||
//            (( currentState == eUsbMusicArtist || currentState == eUsbMusicArtist ) && (previusArtist.compare(this->currentPlayAudioArtist))) ||
//            (( currentState == eUsbMusicAlbom || currentState == eUsbMusicAlbom ) && (previusArtist.compare(this->currentPlayAudioAlbum)))
//                )
        if (currentState == eUsbMusicTitle||
            (( currentState == eUsbMusicArtist ) && (previusArtist.compare(this->currentPlayAudioArtist))) ||
            (( currentState == eUsbMusicAlbom ) && (previusArtist.compare(this->currentPlayAudioAlbum))))
//modified by aettie 2013.03.28 for static defect 13125
       
        {
            SetCurrentPlayItem();
        }

    }
    else if (type == eVideoFileChanged || type == eVideoFileAutoChanged)
    {
        this->currentPlayName = temp.fileName();
        this->currentPlayPath = temp.path();
        //if(currentPlayVideoFilePath.contains(currentVideoPath))  //{ deleted by yongkyun.lee 20130204 for : ISV 70321
        if((type == eVideoFileAutoChanged)
                || ((type == eVideoFileChanged) && (this->m_showCountState == eShowNone))) // modified for ISV 93463
        {
           SetCurrentPlayItem(true);
        }
        else
        {
            SetCurrentPlayItem();
        }
    }
}

void CStateManager::SaveCurrentPlayDataByPathWithoutUpdateUI(QString fullFileName,int state)
{
    if (fullFileName.count() == 0)
    {
        return;
    }

    switch(state)
    {
        case eUsbMusicTitle:
        case eUsbMusicArtist:
        case eUsbMusicAlbom:
        case eJukeMusicTitle:
        case eJukeMusicArtist:
        case eJukeMusicAlbom:
        {
            AppFileManagerMusicData param; //= loadAudioDataByPathFile(fullFileName);
            QFileInfo temp(fullFileName);
            this->currentPlayName = temp.fileName();
            this->currentPlayPath = temp.filePath();
            this->currentPlayAudioArtist = param.artist;
            this->currentPlayAudioAlbum = param.album;
            break;
        }
        case eUsbVideo:
        case eJukeVideo:
        case eUsbListVideo:
        case eJukeListVideo:
        {
            QFileInfo temp(fullFileName);
            this->currentPlayName = temp.fileName();
            this->currentPlayPath = temp.path();
            break;
        }
        case eJukeListPicture:
        case eUsbListPicture:
        case eUsbPictureShow:
        case eJukePictureShow:
        {
            QFileInfo temp(fullFileName);
            this->currentPlayName = temp.fileName();
            this->currentPlayPath = temp.path();
        }
            break;
        default:break;
    }
}

QString CStateManager::GetCurrentPlayName()
{
    switch(currentState)
    {
        case eUsbMusicTitle:
        case eUsbMusicArtist:
        case eUsbMusicAlbom:
        case eJukeMusicTitle:
        case eJukeMusicArtist:
        case eJukeMusicAlbom:
        case eUsbVideo:
        case eJukeVideo:
        case eUsbListVideo:
        case eJukeListVideo:
        case eJukeListPicture:
        case eUsbListPicture:
        case eUsbPictureShow:
        case eJukePictureShow:
        {
            return this->currentPlayName;
        }
        default:break;
    }
    return QString("");
}

QString CStateManager::GetCurrentPlayFolder()
{
    switch(currentState)
    {
        case eUsbMusicTitle:
        case eUsbMusicArtist:
        case eUsbMusicAlbom:
        case eJukeMusicTitle:
        case eJukeMusicArtist:
        case eJukeMusicAlbom:
        case eUsbVideo:
        case eJukeVideo:
        case eUsbListVideo:
        case eJukeListVideo:
        case eJukeListPicture:
        case eUsbListPicture:
        case eUsbPictureShow:
        case eJukePictureShow:
        {
            return this->currentPlayPath;
        }
        default:break;
    }
    return QString("");
}

bool CStateManager::CheckFolder( QString folderName )
{
   return QFileInfo( QDir( FileManagerEngine::GetValidFilePath( currentCopyPath ) ), folderName ).exists();
}

FileOperationsHelper* CStateManager::GetFileManager()
{
    return this->copyFileManager.data();
}

// removed by Dmitry 14.08.13 for ITS0183845
// removed by Dmitry 09.08.13

void CStateManager::checkQueue()
{
   if ( folderQueue.isEmpty() )
   {
      // FM_LOG << "new_log: checkQueue:  empty queue" << LOG_ENDL;

      //emit updateModelFinish(); // modified by Dmitry 27.07.13
      return;
   }

   PrepareFolderInfo( folderQueue.head() );
}

void CStateManager::PrepareFolderInfo( const QString & path )
{
   MP_LOG << "path = " << path << LOG_ENDL;

   switch ( currentState )
   {
      case eUsbVideo:
      case eUsbListVideo:
      case eUsbVideoCopy:
      case eJukeVideo:
      case eJukeListVideo:
      case eJukeVideoCopy:
      case eJukeVideoMove:
      {
         videoListModel->exec( eVideoFilesInFolderRequest, path );
      }
      break;

      case eUsbPictureShow:
      case eUsbListPicture:
      case eUsbPictureCopy:
      case eJukePictureShow:
      case eJukeListPicture:
      case eJukePictureCopy:
      case eJukePictureMove:
      {
         pictureListModel->exec( ePictureFilesInFolderRequest, path );
      }
      break;

      default:
      {
         return;
      }
   }
}

void CStateManager::onDeleteComplete(QString path)
{
//removed by Dmitry 09.08.13
    if (this->appEngine)
    {
        bool isJukebox = path.startsWith(FileManagerEngine::GetValidFilePath(AppFileManagerJukeVideoPath));
        this->appEngine->notifyDeleteVideoPlayer(isJukebox, path, this->frontInstance);
    }
}

QString CStateManager::getJukeboxPath()
{
    return AppFileManagerJukePicturePath;
}

void CStateManager::UncheckAllItems()
{
   switch ( currentState )
   {
      case eUsbPictureShow:
      case eUsbListPicture:
      case eJukePictureShow:
      case eJukeListPicture:
      {
         GetPictureModel()->clearCheckedElements();
// removed by Dmitry 22.08.13
      }
      break;

      case eUsbVideo:
      case eUsbListVideo:
      case eJukeVideo:
      case eJukeListVideo:
      {
         GetVideoModel()->clearCheckedElements();
// removed by Dmitry 22.08.13
      }
      break;

      default:
         break;
   }
}

void CStateManager::PrepareCheckedList( BaseListModel * model )
{
   // FM_LOG << "CStateManager::PrepareCheckedList() checkedPath = " << checkedPath << LOG_ENDL;
   checkedList.clear();

   for ( int i = 0; i < model->getListCount(); i++ )
   {
      if ( model->getListItem(i)->getItemChecked() )
      {
         checkedList.append(model->getListItem(i)->getItemName());
      }
   }
}

// removed by Dmitry 21.08.13

// modified by Dmitry 17.08.13 for ITS0183917
void CStateManager::onThumbNailGenerated()
{
    switch (currentState)
    {
       case eUsbVideo:
       case eJukeVideo:
          //videoListModel->updateThumbnails(0, videoListModel->rowCount()); //commented by honggi.shin, 2014.03.06, Updating all rows makes AFM slow.
          emit updateThumbnailsByQML(); //added by honggi.shin, 2014.03.06, for avoid hanging on to AFM when updating thumbnail
          break;

       default:
          break;
    }
}
// modified by Dmitry 17.08.13 for ITS0183917

void CStateManager::setRootState(bool value)
{
   m_rootState = value;
   emit rootStateChanged();
}

bool CStateManager::getRootState()
{
   return m_rootState;
}

QString CStateManager::getCurrentFolder()
{
   return m_pCurrentFolder;
}

void CStateManager::NavigatorUpdate(QString path)
{
   if ((path == AppFileManagerJukePicturePath )
         || (path == AppFileManagerJukeVideoPath )
         ||(path == AppFileManagerUsbMountPath ))
   {
      setRootState(true);
   }
   else
   {
      setRootState(false);
   }

   m_pCurrentFolder = QFileInfo(FileManagerEngine::GetValidFilePath(path)).fileName();
   currentFolderChanged();

   // FM_LOG << "CStateManager::NavigatorUpdate(QString path) m_pCurrentFolder= " << m_pCurrentFolder << LOG_ENDL;

   emit updateModeArea();
}

void CStateManager::changeState()
{
   // FM_LOG << "CStateManager::changeState()" << LOG_ENDL;

   EAppFileManagerState state = GetCurrentState();
   switch (state)
   {
      case eJukeVideoCopy:
      case eUsbVideoCopy:
         setCurrentState(eUsbVideo);
         break;

      case eJukePictureCopy:
      case eUsbPictureCopy:
         setCurrentState(eUsbPictureShow);
         break;

      default:
         // FM_LOG<<"CStateManager::changeState() undefined state = "<<state << LOG_ENDL;
         break;
   }
}

// modified by Dmitry 27.07.13
void CStateManager::setCurrentItemNumber(int index, bool is_folder)
{
    //MP_LOG << "ZZZ CStateManager::setCurrentItemNumber index="<< index << LOG_ENDL;
    //MP_LOG << "ZZZ CStateManager::setCurrentItemNumber is_folder="<< is_folder << LOG_ENDL;

    int folders = 0;
    int files = 0;
    switch (currentState)
    {
       case eJukeVideo:
       case eUsbVideo:
       case eUsbListVideo:
       case eJukeMusicList:
          files = videoListModel->totalFiles();
          folders = videoListModel->totalFolders();
          break;

       case eJukePictureShow:
       case eUsbPictureShow:
       case eJukeListPicture:
       case eUsbListPicture:
          files = pictureListModel->totalFiles();
          folders = pictureListModel->totalFolders();
          break;

       default:
          return;
    }

    if (index >= 0)
    {
        QString formatString;

        if (folders > 0 && is_folder)
        {
            if(getEngine()->middleEast())
                formatString = QString("%1/%2").arg(folders).arg(index+1);
            else
                formatString = QString("%1/%2").arg(index+1).arg(folders);
            m_isFolder = true;
        }
        else if (files > 0)
        {
            if(appEngine->middleEast())
                formatString = QString("%1/%2").arg(files).arg(index+1 - folders);
            else
                formatString = QString("%1/%2").arg(index+1 - folders).arg(files);
            m_isFolder = false;
        }
		// modified for ITS 0218316
        emit showTotalItems(formatString, m_isFolder,QString::number(folders), QString::number(files)); // modified by eugene 2012.12.12 for NewUX - Photo #5-2
    }
}

// modified by ravikanth for ITS 0187368
void CStateManager::resetCurrentItemNumber()
{
    emit showTotalItems(QString(""), false,"",""); // modified for ITS 0218316
}

int CStateManager::getShowCountState()
{
    return this->m_showCountState;
}

void CStateManager::showCountInfo()
{
   this->m_showCountState = eShowTotal;
   emit showCountStateChanged();
}
// modified by Dmitry 27.07.13

void CStateManager::hideCountInfo()
{
    this->m_showCountState = eShowNone;
    emit showCountStateChanged();
}

UI_Main* CStateManager::getUIControl()
{
    return this->uiControl.data();
}

QString CStateManager::getCurrentPath() // added by eugene.seo 2013.05.29
{
    return this->currentPath;
}

bool CStateManager::isFrontInstance()
{
    return this->frontInstance;
}

void CStateManager::onFilesChanged()
{
   if (this->appEngine)
   {
       if(GetCurrentState() != eJukePictureShow  && GetCurrentState() != eUsbPictureShow ) //added by yungi 2012.10.17 for CR13637
            this->appEngine->notifyFilesChanged();
   }
}

void CStateManager::browseMode(bool value)
{
    m_pBrowseMode = value;
}

int CStateManager::CopyFileToMyFrame(const QString &path, bool overwrite)
{
   // FM_LOG<<"CStateManager::CopyFileToMyFrame  path="<<path << LOG_ENDL;
   //QList<QString> list;
   //list.append(path);

 //QDir dir( FileManagerEngine::GetValidFilePath(AppFileManagerJukePicturePath) ); //deleted by yungi 2013.2.8 for UX Scenario 11. Saveframe path changed
 //QDir dir( FileManagerEngine::GetValidFilePath(AppFileManagerSaveFramePath) );  //added by yungi 2013.2.8 for UX Scenario 11. Saveframe path changed
   QFileInfo fileInfo(FileManagerEngine::GetValidFilePath(AppFileManagerMyFramePicturePath));
   EOperationReturnValue ret;
   if(!fileInfo.exists())
   {
       // { modified by wspark 2012.11.08 for SANITY_CM_AK353
       // do not use below log code, it contains source code(dir.mkdir(path))
       // FM_LOG << "CStateManager::CreateFolder My Frame path= " << dir.mkdir( "My Frame" ) << LOG_ENDL;
       //bool ret = dir.mkdir( "My Frame" );
       // FM_LOG << "CStateManager::CreateFolder My Frame path= " << ret << LOG_ENDL;
       // } modified by wspark
       ret = copyFileManager->saveFileSynchronously(path,AppFileManagerMyFramePicturePath, overwrite); // modified by ravikanth 10-07-13 for ITS 179182
       if(ret == eOperationReturnFileAlreadyExists)
       {
           return 0;
       }
       // copyFileManager->copy(list, AppFileManagerMyFramePicturePath);
   }
   else if(!fileInfo.isDir())
   {
       // FM_LOG<<"CStateManager::CopyFileToMyFrame : file = My Frame exists " << LOG_ENDL;
   }
   else
   {
       ret = copyFileManager->saveFileSynchronously(path,AppFileManagerMyFramePicturePath, overwrite); // modified by ravikanth 10-07-13 for ITS 179182
       if(ret == eOperationReturnFileAlreadyExists)
       {
           return 0;
       }
       // copyFileManager->copy(list, AppFileManagerMyFramePicturePath);
   }
   return 1;
}

void CStateManager::setCurrentState(EAppFileManagerState state)
{
    currentState = state;
    appEngine->setCurrentState(currentState);
}
//{ add by eunhye 2012.09.21 for CR 9243
void CStateManager::onSetMoveStatus(bool status)//EUN
{
    this->appEngine->setMoveStatus(status);
}
//} add by eunhye 2012.09.21

// { added by wspark 2012.12.13 for CR16396
int CStateManager::GetCurrentMediaState()
{
    int currentState = undefined; //modified by aettie 2013.03.28 for Static defect 17545
    switch(GetCurrentState())
    {
        case eJukePictureMove:
        case eUsbListPicture:
        case eJukeListPicture:
        case eJukePictureCopy:
        case eJukePictureShow:
        case eUsbPictureCopy:
        case eUsbPictureShow:
            currentState = ePicture;
            break;
        case eUsbVideo:
        case eUsbVideoCopy:
        case eJukeVideo:
        case eJukeVideoCopy:
        case eJukeListVideo:
        case eUsbListVideo:
        case eJukeVideoMove:
            currentState = eVideo;
            break;
// added by Dmitry 03.05.13
        default:
            break;
// added by Dmitry 03.05.13
    }
    return currentState;
}

void CStateManager::onPlayingDirectoryChanged(QString /*playingDirectory*/)
{
   // don't know what to do here
}

// modified by Dmitry 14.08.13 for ITS0183845
void CStateManager::onFileSystemChanged()
{
// modified by Dmitry 21.08.13
    MP_LOG << LOG_ENDL;
    appEngine->connectReadComplete(true);

    ETrackerAbstarctor_VolumeType volumeType = ETrackerAbstarctor_VolumeType(eJukeBox);
    Event tPostevent(EVT_POST_EVENT, APP_FMGR, APP_AVP, EVT_MAX + 1002, QByteArray((char*)&volumeType, sizeof(volumeType)));
    appEngine->sendEventToUISH(tPostevent, true);    
}
// modified by Dmitry 14.08.13 for ITS0183845

int CStateManager::getStringWidth( const QString &fullString,
                                                      const QString &fontFamily,
                                                      int pixelSize )
{
    QFont mFont;
    mFont.setFamily(fontFamily);
    //mFont.setPixelSize(pixelSize);
    mFont.setPointSize(pixelSize);	//modified by aettie.ji 2012.11.28 for uxlaunch update

    QFontMetrics fontMetrics(mFont);

    return fontMetrics.width(fullString, -1);
}

// modified by Dmitry 22.05.13

//modified by ravikanth 27.06.13 for ITS 0176629
void CStateManager::resetUSBMountPath(QString currentPath)
{
    // @todo - modify to support generic amount of USB devices at one instance
    if(!currentPath.contains(AppFileManagerUsbMountPath))
    {
        for(int i = 0 ; i < m_tUSBMountPath.count(); i++)
        {
            if(currentPath.contains(m_tUSBMountPath.at(i)))
            {
                AppFileManagerUsbMountPath = m_tUSBMountPath.at(i);
                break;
            }
        }
    }
}

// modified by ravikanth 30.06.13 for ITS 0177250
void CStateManager::deleteAllModelItems()
{
    //FM_LOG << "CStateManager::deleteAllModelItems START" << LOG_ENDL;
    switch (currentState)
    {
    case eJukeVideo:
    case eUsbVideo:
    {
        videoListModel->clear();
    }
        break;
    case eJukePictureShow:
    case eUsbPictureShow:
        pictureListModel->clear();
        break;
    default:
        break;
    }
    //FM_LOG << "CStateManager::deleteAllModelItems END" << LOG_ENDL;
}

void CStateManager::deleteSelectedModelItems()
{
    //FM_LOG << "CStateManager::deleteSelectedModelItems START" << LOG_ENDL;

    BaseListModel* modelDelete;
    switch (currentState)
    {
       case eJukeVideo:
       case eUsbVideo:
       {
          modelDelete = videoListModel.data();
       }
       break;
       case eJukePictureShow:
       case eUsbPictureShow:
       modelDelete = pictureListModel.data();
       break;
       default:return;
    }
    for (int i = 0; i < modelDelete->getListCount();)
    {
       if ((modelDelete->getListItem(i))->getItemChecked())
       {
           modelDelete->checkElement(i, false);
           modelDelete->removeDataItem(i);
       }
       else
       {
          i++;
       }
    }
    //FM_LOG << "CStateManager::deleteSelectedModelItems END" << LOG_ENDL;
}

// modified by ravikanth 14-07-13 for ITS 0179049
QString CStateManager::GetCurrentPlayCompleteItemPath()
{
    if(GetCurrentPlayName() == "" || GetCurrentPlayFolder() == "")
    {
        return QString("");
    }
    else
    {
        return ("file://" + GetCurrentPlayFolder() + "/" +GetCurrentPlayName());
    }
}
// added by Dmitry 09.08.13
bool CStateManager::isVideoState()
{
   switch (currentState)
   {
      case eUsbVideo:
      case eJukeVideo:
      case eJukeListVideo:
      case eUsbListVideo:
         return true;
      default:
         break;
   }
   return false;
}
// added by Dmitry 09.08.13

// added by Dmitry 14.08.13 for ITS0183845
void CStateManager::onUpdateUIData(QVector<QStringList> *list)
{
   Q_UNUSED(list) // modified by Dmitry 17.08.13
// for future use
}
// added by Dmitry 14.08.13 for ITS0183845

// modified by ravikanth for ITS 0184642
void CStateManager::setEditModeLaunchFromAVP(bool value, bool unsupportedFile)
{
    m_editModeFromAVP = value;
    m_fmLaunchUnsupportedFileCopy = unsupportedFile; // modified for ITS 0209355
}

// added by Dmitry 21.08.13
void CStateManager::onReadCompleted(ETrackerAbstarctor_VolumeType volume, int status)
{
   MP_LOG << "status = " << status << LOG_ENDL;

   appEngine->connectReadComplete(false); // added by Dmitry 22.08.13

   if(volume == eJukeBox)
   {
      switch (currentState)
      {
         case eJukePictureShow:
         case eJukeListPicture:
         case eJukeVideo:
         case eJukeListVideo:
            newPath.clear();
            break;
         default: break;
      }

     StateChangeUpdate();
   }
}
// added by Dmitry 21.08.13

// modified by ravikanth 02-09-13
ETrackerAbstarctor_VolumeType CStateManager::volumeType(QString currentPath)
{
    // if( currentPath.contains(USBFront))
    if( currentPath.indexOf(appEngine->GetFrontUSBPath()) == 0) // modified by kihyung 2013.12.09 for ITS 0213549 
    {
        return eUSB_FRONT;
    }
    // else if (currentPath.contains(USBRear))
    else if (currentPath.indexOf(appEngine->GetRearUSBPath()) == 0) // modified by kihyung 2013.12.09 for ITS 0213549 
    {
        return eUSB_REAR;
    }
    else
    {
        return eJukeBox;
    }
}

// modified for ITS 0212431
void CStateManager::SetFileOperationModeType()
{
    if(this->copyFileManager)
    {
        EOperationMediaType mediaType = eOperationMediaNone;
        if((m_entryState == eJukePictureShow) || (m_entryState == eJukeVideo))
        {
            mediaType = eOperationMediaJukeBox;
        }
        else if((m_entryState == eUsbPictureShow) || (m_entryState == eUsbVideo))
        {
            mediaType = eOperationMediaUSB;
        }
        this->copyFileManager->setMediaType(mediaType);
    }
}

// modified for ITS 0214738
void CStateManager::onSetFileOperationStopMedia(int startState, bool isFront)
{
    MP_LOG << startState <<" "<<m_entryState << LOG_ENDL;
    if((startState == eStartInvalidSState) || (isFront == isFrontInstance()))
        this->m_currentCopyStopState = (EAppFileManagerStartState)(startState);
    else
        this->m_currentCopyStopState = eStartInvalidSState;
}

