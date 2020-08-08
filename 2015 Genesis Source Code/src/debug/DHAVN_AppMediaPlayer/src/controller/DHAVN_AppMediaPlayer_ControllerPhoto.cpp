#include "controller/DHAVN_AppMediaPlayer_ControllerPhoto.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "engine/DHAVN_AppMediaPlayer_EnginePhoto.h" // added by kihyung for CR 10841
#include <MediaInfo/MediaInfo.h> // added by ravikanth 25-12-12
#include "util/photo/DHAVN_AppMediaPlayer_Thumbnailer.h" // modified by ravikanth 08-04-13
#include "engine/DHAVN_AppMediaPlayer_EnginePhoto.h" // added by Sergey 22.05.2013

#define one_second 1000
// { added by ravikanth 25-12-12
#define PHOTO_MAX_SUPPORTED_SIZE 5 //mega bytes
#define PHOTO_MAX_SUPPORTED_WIDTH 4000 // modified by eugene.seo 2013.03.26
#define PHOTO_MAX_SUPPORTED_HEIGHT 3000 // modified by eugene.seo 2013.03.26
// modified by ravikanth 13.06.2013 for ITS 0172957
#define PHOTO_MAX_DISPLAY_HEIGHT 720
#define PHOTO_MAX_DISPLAY_WIDTH 1280
#define PHOTO_MAX_RESOLUTION_WIDTH 2048
#define PHOTO_MAX_RESOLUTION_HEIGHT 1152
// } added by ravikanth 25-12-12

#define TA_REQUID(X) (X + m_CurSource)
enum
{
    REQUID_PHOTO_ID = 2000,
};

bool AppMediaPlayer_Controller_Photo::m_readyToPWOFF(false);   //2014.05.23

// modified by ruindmby 2012.11.30 for CR#15766
AppMediaPlayer_Controller_Photo::AppMediaPlayer_Controller_Photo( AppMediaPlayer_EngineMain &engineMain, 
																  AppMediaPlayer_EnginePhoto* enginePhoto ) // added by Sergey 22.05.2013
: AppMediaPlayer_Controller_Base( engineMain ),
   m_enginePhoto(enginePhoto), // added by Sergey 22.05.2013
   m_CurSource(PHOTO_SOURCE_JUKEBOX),
   m_SourceMask(PHOTO_SOURCE_MASK_JUKEBOX),
   m_currentLoadingFileSize(0),    // added by ravikanth 25-12-12
   m_pTrackerAbstractor( new CTrackerAbstractor() ), // added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
   unsupportedImageTimer(NULL) // modified by ravikanth 23.06.13
{
    for (int i=0;i<PHOTO_SOURCE_MAX;i++)
    {
        m_FolderPath[i] = "";
        m_PlayList[i].list = new QList<QString>;
        m_PlayList[i].list->clear();
        m_corruptedImages[i] = 0;
    }	    
	// { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
	if ( m_pTrackerAbstractor != NULL )
    {
	    connect( m_pTrackerAbstractor, SIGNAL( RequesteComplete( QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint) ), 
                                 this,   SLOT( HandleRequestComplete( QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint) ) );
        connect(m_pTrackerAbstractor, SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint)),
                                this,   SLOT(HandleRequestComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint))); // added by Sergey for ITS#159209 for 27.04.2013
        m_pTrackerAbstractor->setDeviceId(eJukeBox, NULL, "/usr_data/media");
        m_pTrackerAbstractor->SetCountryVariant(AppEngineBase::GetCountryVariant()); // added by Dmitry 28.03.15
    }
	m_contentCreated = "";  // added eugene.seo 2012.12.06
    // } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
    connect(PhotoThumbnailer::instance(), SIGNAL(fileNotSupported(QString)), this, SLOT(updateUnsupportedInfo(QString))); // modified by ravikanth 08-04-13
    m_unsupportedFileInfo.q_Filename = "";
    m_unsupportedFileInfo.b_Supported = false;
}
// modified by ruindmby 2012.11.30 for CR#15766

AppMediaPlayer_Controller_Photo::~AppMediaPlayer_Controller_Photo()
{
    for (int i = 0;i < PHOTO_SOURCE_MAX; i++)
    {
        if(m_PlayList[i].list)
        {
            delete m_PlayList[i].list;
        }
    }
    // { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
	if ( m_pTrackerAbstractor != NULL )
    {
        disconnect(m_pTrackerAbstractor, 0, 0, 0);
	    delete m_pTrackerAbstractor;
    }
    // } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
	// modified by ravikanth 23.06.13
    if(unsupportedImageTimer != NULL) 
    {
        if(unsupportedImageTimer->isActive())
        {
            unsupportedImageTimer->stop();
        }
        delete unsupportedImageTimer;
    }
}

// 2014.05.23
void AppMediaPlayer_Controller_Photo::setReadyToPWOFF(bool value)
{
   m_readyToPWOFF = value;
}

void AppMediaPlayer_Controller_Photo::SwitchMode(const PHOTO_SOURCES_LIST source)
{
    MP_LOG << LOG_ENDL;

    m_CurSource = source;
    SetSource(m_CurSource);
    // SetCurModePlaylist(); // removed by kihyung 2013.2.17
}

void AppMediaPlayer_Controller_Photo::SetSource(const PHOTO_SOURCES_LIST source)
{
   // MP_LOG << "source =" << source << LOG_ENDL;
   // { added by lssanh 2013.02.07 2USB
   GetEngineMain().GetNotifier()->cancelFileOperation();
   // } added by lssanh 2013.02.07 2USB   
   if( source == PHOTO_SOURCE_JUKEBOX )
   {    
      //emit deviceChanged(MEDIA_SOURCE_JUKEBOX); // deleted by wspark 2013.04.25 for cluster IOT
   }
   else if ( source == PHOTO_SOURCE_USB_1 || source == PHOTO_SOURCE_USB_2 )
   {
      //MEDIA_SOURCE_T media_source = (MEDIA_SOURCE_T)source; // commented by cychoi 2013-07-16 for Prevent fix
      //emit deviceChanged( media_source ); // deleted by wspark 2013.04.25 for cluster IOT
   }
   else
   {
      // MP_LOG<<"[no source]" << LOG_ENDL;
      return;
   }
}

void AppMediaPlayer_Controller_Photo::InitSources()
{
   // MP_LOG << LOG_ENDL;
   SetSource(m_CurSource);
   // SetCurModePlaylist();
}

void AppMediaPlayer_Controller_Photo::changeCurSourceID(const PHOTO_SOURCES_LIST source)
{
   // MP_LOG << LOG_ENDL;
   m_CurSource = source;
}

PHOTO_SOURCES_LIST AppMediaPlayer_Controller_Photo::getCurSourceID()
{
   // MP_LOG << LOG_ENDL;
   return m_CurSource;
}

// { removed by kihyung 2013.2.17
/*
bool AppMediaPlayer_Controller_Photo::isImageInFolder(const QString &folder_path) // modified by ruinseve for CR#12584
{
   
   QDir dir(folder_path);

   QStringList nameFilter;
   nameFilter = GetControllers().getExtList(MEDIA_TYPE_PHOTO); 
   
   if ( dir.entryList(nameFilter, QDir::NoDotAndDotDot|QDir::Files).count() > 0 )
   {
      return true;
   }
   return false; 
}
*/
// } removed by kihyung 2013.2.17

// { removed by kihyung 2013.2.17
/*
bool AppMediaPlayer_Controller_Photo::SearchImageInSubFolders(QString *folder_path) // modified by ruinseve for CR#12584
{
   if (folder_path == NULL)
   {
      return false;
   }

   QDir dir (*folder_path);
   QStringList dir_list = dir.entryList( QDir::Dirs | QDir::NoDotAndDotDot);

   for (int i=0; i< dir_list.count(); i++)
   {
      QString dir_name = dir_list.at(i);
      folder_path->append("/");
      folder_path->append(dir_name);

      if (isImageInFolder(*folder_path) || SearchImageInSubFolders(folder_path) == true )
      {
         return true;
      }

      folder_path->truncate( folder_path->lastIndexOf('/'));
   }
   return false;
}
*/
// } removed by kihyung 2013.2.17

PlayList* AppMediaPlayer_Controller_Photo::getPointerToPL(const PHOTO_SOURCES_LIST source)
{
    MP_LOG<<"[source]:"<<source << LOG_ENDL;
   PlayList* tmpList = NULL;
   if (source >= 0 && source < PHOTO_SOURCE_MAX)
   {
     tmpList= &m_PlayList[source];
   }
   else
   {
      // MP_LOG<<"[source is wrong]" << LOG_ENDL;
   }
   return tmpList;
}

QString* AppMediaPlayer_Controller_Photo::getPointerToFolder(const PHOTO_SOURCES_LIST source)
{
   // MP_LOG<<"[source]:"<<source << LOG_ENDL;
   QString* tmpFolder = NULL;
   if (source >= 0 && source < PHOTO_SOURCE_MAX)
   {
      tmpFolder= &m_FolderPath[source];
   }
   else
   {
      // MP_LOG<<"[source is wrong]" << LOG_ENDL;
   }
   return tmpFolder;
}

// { removed by kihyung 2013.2.17
/*
QString AppMediaPlayer_Controller_Photo::GetFolderPathFromSource(const PHOTO_SOURCES_LIST source)
{
   // MP_LOG<<"[source]:"<<source << LOG_ENDL;
   QString tmpFolderPath = "";
   if (source >= 0 && source < PHOTO_SOURCE_MAX)
   {
      tmpFolderPath.append(m_FolderPath[source]);
   }
   else
   {
      // MP_LOG<<"[source is wrong]" << LOG_ENDL;
   }
   return tmpFolderPath;
}
*/
// } removed by kihyung 2013.2.17

// { removed by kihyung 2013.2.17
/*
void AppMediaPlayer_Controller_Photo::SetFolder(const QString &folder_path, const PHOTO_SOURCES_LIST source)
{ 
   // MP_LOG<<"[folder_path]"<< folder_path << LOG_ENDL;
   // MP_LOG<<"[source]"<< source << LOG_ENDL;

   PlayList* tmpList = getPointerToPL(source);
   QString* tmpFolder = getPointerToFolder(source);

   if( tmpList != NULL && tmpFolder != NULL )
   {
      tmpFolder->clear();
      tmpFolder->append(&folder_path);

      if (!isImageInFolder(folder_path) )
      {
         SearchImageInSubFolders(tmpFolder);
      }

      createPlayList(tmpList[0], *tmpFolder,"", source); // modified by wspark 2012.09.01 for CR11583
   }
   else
   {
      // MP_LOG<<"[NULL pointers to PlayList or Folder]" << LOG_ENDL;
   }
}
*/
// } removed by kihyung 2013.2.17

// { removed by kihyung 2013.2.17
/*
bool AppMediaPlayer_Controller_Photo::SetFolderAndFile(const QString file_path, const PHOTO_SOURCES_LIST source)
{
   MP_LOG << LOG_ENDL;
   MP_LOG<< "[file_path]:" << file_path << LOG_ENDL;
   MP_LOG<< "[source]: " << source << LOG_ENDL;

   bool result = false;

   PlayList* tmpList = getPointerToPL(source);
   QString* tmpFolder = getPointerToFolder(source);

   if( tmpList != NULL && tmpFolder != NULL )
   {
      QString folder_path = file_path;
      int nIndex = folder_path.lastIndexOf('/');
      folder_path   = folder_path.mid(0, nIndex);

      MP_LOG<< "[folder_path]:" << folder_path << LOG_ENDL;

      QFile file_to_set(file_path);

      if( file_to_set.exists())
      {
         tmpFolder->clear();
         tmpFolder->append(&folder_path);

         createPlayList(tmpList[0], folder_path, file_path, source); // modified by wspark 2012.09.01 for CR11583
         result = true;
      }
   }
   else
   {
      // MP_LOG<< "[NULL pointers to current PlayList or Folder]" << LOG_ENDL;
   }

   return result;
}
*/
// } removed by kihyung 2013.2.17

// { removed by kihyung 2013.2.17
/*
// modified by ruindmby 2012.11.28
void AppMediaPlayer_Controller_Photo::createPlayList(PlayList &playlist,const QString folder_path, const QString file_path, const PHOTO_SOURCES_LIST source) // modified by wspark 2012.09.01 for CR11583
{
   // MP_LOG << LOG_ENDL;

   int temp_ind=0;
   playlist.list->clear();

   QDir dir(folder_path);

   QStringList nameFilter;
   // { modified by ruinseve for CR#12584
   // { modified by yongkyun.lee@lge.com    2012.09.14 : Bug 12898 - USB Image-Play unsupported file format, popup message is not displayed
   // nameFilter << "*.jpg" << "*.png" << "*.bmp" << "*.gif"<< "*.tif"<< "*.tiff";
   // nameFilter << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.gif"<< "*.tif"<< "*.tiff";
   // } modified by  yongkyun.lee@lge.com 
   nameFilter = GetControllers().getExtList(MEDIA_TYPE_PHOTO); // modified by ruinseve for CR#12584
   // } modified by ruinseve for CR#12584.

   playlist.list->append(dir.entryList(nameFilter, QDir::NoDotAndDotDot|QDir::Files));

   temp_ind = playlist.list->indexOf(file_path.section('/', -1));

   emit contentSize( (MEDIA_SOURCE_T)source, MEDIA_TYPE_PHOTO, playlist.list->count() ); // modified by wspark 2012.09.01 for CR11583
   playlist.index = (temp_ind != -1) ? temp_ind : 0;
   playlist.position = 0;
   playlist.mode = 0;
}
// modified by ruindmby 2012.11.28
*/
// } removed by kihyung 2013.2.17

// { removed by kihyung 2013.2.17
/*
void AppMediaPlayer_Controller_Photo::SetCurModePlaylist()
{
   // MP_LOG<< "[m_CurSource]:" << m_CurSource << LOG_ENDL;
   MEDIA_SOURCE_T media_source = (MEDIA_SOURCE_T)m_CurSource;
   emit contentSize( media_source, MEDIA_TYPE_PHOTO, m_PlayList[m_CurSource].list->count() );
}
*/
// } removed by kihyung 2013.2.17


QString AppMediaPlayer_Controller_Photo::GetCurrentFileFullPathName()
{
   MP_LOG << m_CurSource << m_PlayList[m_CurSource].list->count() << m_PlayList[m_CurSource].index << LOG_ENDL;

   QString retStr = "";

   if((m_PlayList[m_CurSource].list->count() <= m_PlayList[m_CurSource].index) || (m_PlayList[m_CurSource].index < 0 ))
   {
       MP_LOG << "Fatal error index out of range! Reset to 0" << LOG_ENDL;
       m_PlayList[m_CurSource].index = 0;
   }

   if(!m_PlayList[m_CurSource].list->isEmpty())
   {
       retStr = m_PlayList[m_CurSource].list->at(m_PlayList[m_CurSource].index); // modified by Sergey for ITS#159209 for 27.04.2013
   }

   MP_LOG << retStr << LOG_ENDL;
   
   return retStr;
}

// { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
void AppMediaPlayer_Controller_Photo::GetPhotoMetaDataInfo(const QString& fileFullPath)
{
	if(m_pTrackerAbstractor)	
		m_pTrackerAbstractor->RequestData( fileFullPath, eGetPhotoMetaDataInfoFromUrlQuery_Id );
}

QString AppMediaPlayer_Controller_Photo::GetCurrentCreateDate()
{
	return m_contentCreated; // added eugene.seo 2012.12.06
}
// } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup

void AppMediaPlayer_Controller_Photo::NextImage()
{
    MP_LOG << LOG_ENDL;
    if(m_PlayList[m_CurSource].list->isEmpty()) {
        return;
    }
	// { modified by ravikanth 25-12-12
    // removed by ruindmby 2012.11.30 for CR#15766
    //    m_CurFilePath[m_CurSource].clear();
    //    m_CurFilePath[m_CurSource].append(GetCurrentFileFullPathName());
    // removed by ruindmby 2012.11.30 for CR#15766

// commented by ravikanth 14-05-13
//    bool bSupported = true;
//     m_corruptedImages[m_CurSource] = 0; // modified by ravikanth 24-04-13
//    do
//    {
        m_PlayList[m_CurSource].index++;
        if(m_PlayList[m_CurSource].index >= m_PlayList[m_CurSource].list->count()) {
            m_PlayList[m_CurSource].index = 0;
            // break;
        }
// commented by ravikanth 14-05-13
//        bSupported = isFileSupported(QString(""));
//        if(bSupported == false) {
//            m_corruptedImages[m_CurSource]++;
//            if(m_corruptedImages[m_CurSource] == m_PlayList[m_CurSource].list->count()) {
//                int source = (int)m_CurSource;
//                GetEngineMain().GetMediaManager()->SetSourceAvailable((MEDIA_SOURCE_T)source, MEDIA_TYPE_PHOTO, false);
//                GetEngineMain().GetNotifier()->UpdateSDPMediaCount((MEDIA_SOURCE_T)source, MEDIA_TYPE_PHOTO, 0);
//                GetEngineMain().GetNotifier()->setEnableMedia((MEDIA_SOURCE_T)source, MEDIA_TYPE_MAX);
//                emit handleAllUnsupportedFiles(source); // modified by ravikanth 28-04-13
//                m_PlayList[m_CurSource].list->clear(); // modified by ravikanth 04-05-13
//                break;
//            }
//        }
//    }
//    while(!bSupported);
	// } modified by ravikanth 25-12-12
}

void AppMediaPlayer_Controller_Photo::PrevImage()
{
    MP_LOG << LOG_ENDL;
    if(m_PlayList[m_CurSource].list->isEmpty()) {
        return;
    }
    // removed by ruindmby 2012.11.30 for CR#15766
    //    m_CurFilePath[m_CurSource].clear();
    //    m_CurFilePath[m_CurSource].append(GetCurrentFileFullPathName());
    // removed by ruindmby 2012.11.30 for CR#15766
	// { modified by ravikanth 25-12-12

// commented by ravikanth 14-05-13	
//	bool bSupported = true;
//	m_corruptedImages[m_CurSource] = 0;  // modified by ravikanth 24-04-13

//    do
//    {
        m_PlayList[m_CurSource].index--;
        if(m_PlayList[m_CurSource].index < 0) {
            m_PlayList[m_CurSource].index = m_PlayList[m_CurSource].list->count() - 1;
            // break; // deleve by eugene.seo 2013.03.07
        }
		
// commented by ravikanth 14-05-13
//        bSupported = isFileSupported(QString(""));
//        if(bSupported == false) {
//            m_corruptedImages[m_CurSource]++;
//            if(m_corruptedImages[m_CurSource] == m_PlayList[m_CurSource].list->count()) {
//                int source = (int)m_CurSource;
//                GetEngineMain().GetMediaManager()->SetSourceAvailable((MEDIA_SOURCE_T)source, MEDIA_TYPE_PHOTO, false);
//                GetEngineMain().GetNotifier()->UpdateSDPMediaCount((MEDIA_SOURCE_T)source, MEDIA_TYPE_PHOTO, 0);
//                GetEngineMain().GetNotifier()->setEnableMedia((MEDIA_SOURCE_T)source, MEDIA_TYPE_MAX);
//                emit handleAllUnsupportedFiles(source); // modified by ravikanth 28-04-13
//                m_PlayList[m_CurSource].list->clear(); // modified by ravikanth 04-05-13
//                break;
//            }
//        }
//    }
//    while(!bSupported);
	// } modified by ravikanth 25-12-12
}

// { removed by Sergey for ITS#159209 for 27.04.2013
// { modified by ravikanth 15-05-13 for unsuported images skip index
int AppMediaPlayer_Controller_Photo::GetCurrentIndex()
{
    MP_LOG<< m_CurSource << m_PlayList[m_CurSource].index << LOG_ENDL;
    return m_PlayList[m_CurSource].index;
}

int AppMediaPlayer_Controller_Photo::GetCurrentPlayListCount()
{
    MP_LOG << m_CurSource << m_PlayList[m_CurSource].list->count() << LOG_ENDL;
    return m_PlayList[m_CurSource].list->count();
}
// } modified by ravikanth 15-05-13
// } removed by Sergey for ITS#159209 for 27.04.2013

bool AppMediaPlayer_Controller_Photo::isSourceEmpty( PHOTO_SOURCES_LIST source )
{
   // MP_LOG << "[source]:" <<source << LOG_ENDL;
   if (source>=0 && source<PHOTO_SOURCE_MAX)
   {
      if (m_PlayList[source].list->count() == 0)
      {
          // MP_LOG << "[No images on source]" << LOG_ENDL;
          return true;
      }
   }
   return false;
}

void AppMediaPlayer_Controller_Photo::requestFirstFolder(PHOTO_SOURCES_LIST photoSource)
{
    MP_LOG << photoSource << LOG_ENDL;
    
    ETrackerAbstarctor_VolumeType volumeType;
    if(photoSource == PHOTO_SOURCE_JUKEBOX)
        volumeType = eJukeBox;
    else if(photoSource == PHOTO_SOURCE_USB_1)
        volumeType = eUSB_FRONT;
    else if(photoSource == PHOTO_SOURCE_USB_2)
        volumeType = eUSB_REAR;
    else
        return;

    if(m_pTrackerAbstractor) {
        QString strFirstFolder = m_pTrackerAbstractor->GetFirstPhotoFolder(volumeType);         

        MP_LOG << strFirstFolder << LOG_ENDL;
        
        QString *pFolder = getPointerToFolder(photoSource);
        if(pFolder != NULL) {
            pFolder->clear();
            pFolder->append(strFirstFolder);
        }

        m_strFMRequestFile.clear(); // added by kihyung 2013.3.19 for ISV 76493
        requestFilesInFolder(strFirstFolder,photoSource); // modified by ravikanth 29-06-13 for All playlist to Folder playlist changes
        //m_pTrackerAbstractor->RequestData(strFirstFolder, eGetPhotoFolderTabList_Id, REQUID_PHOTO_ID + photoSource);
    }
}

void AppMediaPlayer_Controller_Photo::requestLastFolder(PHOTO_SOURCES_LIST photoSource, QString strFolder, QString strFileName)
{
    MP_LOG << photoSource << LOG_ENDL;
    
    ETrackerAbstarctor_VolumeType volumeType;
    if(photoSource == PHOTO_SOURCE_JUKEBOX)
        volumeType = eJukeBox;
    else if(photoSource == PHOTO_SOURCE_USB_1)
        volumeType = eUSB_FRONT;
    else if(photoSource == PHOTO_SOURCE_USB_2)
        volumeType = eUSB_REAR;
    else
        return;

    if(m_pTrackerAbstractor) 
    {
        QString strModifiedPath;
        if(strFolder.startsWith("file://"))
            strModifiedPath = strFolder.mid(7);
        else
            strModifiedPath = strFolder;

        QString *pFolder = getPointerToFolder(photoSource);
        if(pFolder != NULL) {
            pFolder->clear();
            pFolder->append(strModifiedPath);
        }
        
        m_strFMRequestFile = strFileName;
        m_pTrackerAbstractor->RequestData(strModifiedPath, eGetPhotoFolderTabList_Id, REQUID_PHOTO_ID + photoSource);
    }
}

void AppMediaPlayer_Controller_Photo::updatePlaylist(QStringList &photoList, QString strFMRequestFile, PHOTO_SOURCES_LIST mediaSource)
{
    MP_LOG << "photoList:" << photoList.count() << "source:" << mediaSource << LOG_ENDL;

    PlayList *pPlayList = getPointerToPL(mediaSource);
    
    if(pPlayList != NULL)
    {
        pPlayList->list->clear();
        pPlayList->list->append(photoList);
        pPlayList->position = 0;
        pPlayList->mode     = 0;
        pPlayList->category = 0;

        int nTmpIdx = pPlayList->list->indexOf(strFMRequestFile.section('/', -1));
        pPlayList->index    = (nTmpIdx != -1) ? nTmpIdx : 0;
        
        m_corruptedImages[mediaSource] = 0;
        m_corruptedFiles[mediaSource].clear(); // added by Sergey 22.05.2013
    }
    // QFuture<void> future = QtConcurrent::run(this, &AppMediaPlayer_Controller_Photo::updateSupportedInfo, mediaSource); // modified by ravikanth 26-03-13
}

void
AppMediaPlayer_Controller_Photo::HandleError(int errorCode)
{
    // MP_LOG << "errorCode" << errorCode << LOG_ENDL;

    switch(errorCode)
    {
        case eMP_ERR_STREAM:
        case eMP_ERR_UNSUPPORTED_FILE:
        case eMP_ERR_FORMAT_ERROR:
        case eMP_ERR_RESOURCE:
        {
           emit showErrorPopup();
           break;
        }
        // { add by kihyung 2012.06.22
        // To remove an inifinite loop and CPU 100%.
        case eMP_ERR_UNSUPPORTED_ALL_FILE:
        {
            break;
        }
        // } add by kihyung
        default:
            break;
    }
}

void
AppMediaPlayer_Controller_Photo::ClearPlayList( PHOTO_SOURCES_LIST source )
{
    MP_LOG << LOG_ENDL;
    m_PlayList[source].list->clear();
    m_PlayList[source].index    = 0;
    m_PlayList[source].position = 0;
    m_PlayList[source].mode     = 0;
    m_corruptedImages[source]   = 0;
    m_corruptedFiles[source].clear(); // added by Sergey 22.05.2013
    ClearThumbnailData(source); // modified by ravikanth 08-04-13
}

// { added by Sergey for ITS#159209 for 27.04.2013
void AppMediaPlayer_Controller_Photo::requestFilesInFolder(QString folder, PHOTO_SOURCES_LIST photoSource)
{
    m_pTrackerAbstractor->RequestData(folder, eGetImagesByFolderQuery_Id, REQUID_PHOTO_ID + photoSource);
}

void AppMediaPlayer_Controller_Photo::requestAllFiles(PHOTO_SOURCES_LIST photoSource)
{
    MP_LOG << photoSource << LOG_ENDL;

    ETrackerAbstarctor_VolumeType volumeType;
    if(photoSource == PHOTO_SOURCE_JUKEBOX)
        volumeType = eJukeBox;
    else if(photoSource == PHOTO_SOURCE_USB_1)
        volumeType = eUSB_FRONT;
    else if(photoSource == PHOTO_SOURCE_USB_2)
        volumeType = eUSB_REAR;
    else
        return;

    if(m_pTrackerAbstractor)
    {
        m_strFMRequestFile.clear(); // added by kihyung 2013.3.19 for ISV 76493
        m_pTrackerAbstractor->RequestData(volumeType, eGetPhotoLimitFileListDir_Id, REQUID_PHOTO_ID + photoSource);
    }
}

void AppMediaPlayer_Controller_Photo::HandleRequestComplete(QVector<QStringList> fileList,
                                                        ETrackerAbstarctor_Query_Ids queryId,
                                                        ETrackerAbstarctor_VolumeType volumeType,
                                                        uint reqUID)
{
    MP_LOG << "queryId =" << queryId << "volumeType =" << volumeType << LOG_ENDL;
    MP_LOG << "count =" << fileList.count() << LOG_ENDL;

    if(queryId == eGetPhotoLimitFileListDir_Id)
    {
        PHOTO_SOURCES_LIST mediaSource;
        switch(reqUID)
        {
        case REQUID_PHOTO_ID + PHOTO_SOURCE_JUKEBOX: mediaSource = PHOTO_SOURCE_JUKEBOX; break;
        case REQUID_PHOTO_ID + PHOTO_SOURCE_USB_1:   mediaSource = PHOTO_SOURCE_USB_1;   break;
        case REQUID_PHOTO_ID + PHOTO_SOURCE_USB_2:   mediaSource = PHOTO_SOURCE_USB_2;   break;
        default:
            MP_LOG << "reqUID is invalid" << reqUID << LOG_ENDL;
            return;
        }

        QStringList photoList;

        PhotoThumbnailer::instance()->createDirIfAbsent(); // modified by ravikanth 08-04-13

        foreach (QStringList list, fileList)
        {
            foreach(QString file, list)
            {
                QString strFilePath = file;
                if( m_readyToPWOFF ) break; //2014.05.23
                PhotoThumbnailer::instance()->getThumbnail(QString("file://" + strFilePath),
                                                           (PHOTO_THUMBNAIL_MEDIA_LIST)mediaSource); // modified by ravikanth 08-04-13
                photoList.append(file);
            }
        }

        if(!photoList.isEmpty())
        {
            updatePlaylist(photoList, m_strFMRequestFile, mediaSource);
            m_strFMRequestFile.clear();
			// { modified by ravikanth 01-05-13
            if(mediaSource == PHOTO_SOURCE_JUKEBOX)
            {
				// jukebox data can be updated on copy and delete, rebuild folder list on every request
                QString folder = photoList.at(0).mid(0, photoList.at(0).lastIndexOf('/'));
                requestFilesInFolder(folder, mediaSource);
            }
			// } modified by ravikanth 01-05-13
        }
    }

    // { modified by ravikanth 20-04-13
    if(reqUID == (REQUID_PHOTO_ID + PHOTO_SOURCE_JUKEBOX))
    {
        ((AppMediaPlayer_EnginePhoto*)GetEngineMain().GetEngine(APP_MEDIA_MODE_PHOTO_FRONT))->ResetProgressLoading();
        ((AppMediaPlayer_EnginePhoto*)GetEngineMain().GetEngine(APP_MEDIA_MODE_PHOTO_REAR))->ResetProgressLoading();
    }
    // } modified by ravikanth 20-04-13
}

// { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
void
AppMediaPlayer_Controller_Photo::HandleRequestComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID) // add reqUID by junam 2012.10.25 for CR14451
{
    MP_LOG << "=================================================" << LOG_ENDL;
    MP_LOG << "queryId =" << queryId << LOG_ENDL;
    MP_LOG << "count =" << fileList.count() << LOG_ENDL;

    switch ( queryId )
    {
    case eGetPhotoMetaDataInfoFromUrlQuery_Id:
    {
        if(!fileList.isEmpty())
        {
            QString contentCreatedTime; // added eugene.seo 2012.12.06
            foreach (QStringList file, fileList)
            {
                contentCreatedTime = file[2];	  // modified eugene.seo 2012.12.06
            }
            ConvertTimeFormat(contentCreatedTime); // added eugene.seo 2012.12.06 for convert contentCreated date format into file info
        }
        break;
    }

    case eGetImagesByFolderQuery_Id:
    {
		// { modified by ravikanth 29-06-13 for All playlist to Folder playlist changes
		// { modified by ravikanth 28-04-13
        PHOTO_SOURCES_LIST mediaSource;
        switch(reqUID)
        {
        case REQUID_PHOTO_ID + PHOTO_SOURCE_JUKEBOX: mediaSource = PHOTO_SOURCE_JUKEBOX; break;
        case REQUID_PHOTO_ID + PHOTO_SOURCE_USB_1:   mediaSource = PHOTO_SOURCE_USB_1;   break;
        case REQUID_PHOTO_ID + PHOTO_SOURCE_USB_2:   mediaSource = PHOTO_SOURCE_USB_2;   break;
        default:
            MP_LOG << "reqUID is invalid" << reqUID << LOG_ENDL;
            return;
        }
// modified by Dmitry 06.08.13
        if (fileList.size() >= 3)
        {
           QStringList files;
           QStringList filePaths = fileList[1];
           QStringList folder = fileList[2];

           for (int i = 0; i < filePaths.count(); ++i)
           {
               if (folder.at(i) != "true")
               {
                  QString file = filePaths.at(i);
                  if(file.startsWith("file://"))
                  {
                      if( m_readyToPWOFF ) break; //2014.05.23
                      PhotoThumbnailer::instance()->getThumbnail(QString(file),
                                                                 (PHOTO_THUMBNAIL_MEDIA_LIST)mediaSource);
                      files.append(file.remove(0,7));
                  }
                  else
                  {
                      if( m_readyToPWOFF ) break; //2014.05.23
                      PhotoThumbnailer::instance()->getThumbnail(QString("file://" + file),
                                                                 (PHOTO_THUMBNAIL_MEDIA_LIST)mediaSource);
                      files.append(file);
                  }
               }
           }
           if(!files.isEmpty())
           {
               updatePlaylist(files, m_strFMRequestFile, mediaSource);
               m_strFMRequestFile.clear();
           }
                // } modified by ravikanth 29-06-13 for All playlist to Folder playlist changes

           emit filesInCurFolder(files, (int)mediaSource);
                // } modified by ravikanth 28-04-13
        }
// modified by Dmitry 06.08.13
    }

    default:
        break;
    }
    if(reqUID == (REQUID_PHOTO_ID + PHOTO_SOURCE_JUKEBOX))
    {
        ((AppMediaPlayer_EnginePhoto*)GetEngineMain().GetEngine(APP_MEDIA_MODE_PHOTO_FRONT))->ResetProgressLoading();
        ((AppMediaPlayer_EnginePhoto*)GetEngineMain().GetEngine(APP_MEDIA_MODE_PHOTO_REAR))->ResetProgressLoading();
    }
}
// } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
// } modified by Sergey for ITS#159209 for 27.04.2013

// { added eugene.seo 2012.12.06 for convert contentCreated date format into file info
void
AppMediaPlayer_Controller_Photo::ConvertTimeFormat(const QString& localtime)
{
	QString tmpStr = localtime;
	QString date = "";
	QString time = "";             	

	if(!tmpStr.isEmpty())
    {
    	if(tmpStr.contains("."))
			tmpStr.remove(tmpStr.indexOf("."),tmpStr.indexOf("Z"));
		else
			tmpStr.remove("Z");
		
		tmpStr.replace("-",".");
		date = tmpStr.left(10);
		time = tmpStr.right(8);
    }

	QString hh = time.left(2);
	QString mmss = time.right(6);

	int tmpNum = hh.toInt();	
	if(tmpNum > 12)
	{
		tmpNum = tmpNum - 12;
		if(tmpNum < 10)
			m_contentCreated = date + " PM 0" + hh.number(tmpNum) + mmss;	
		else
			m_contentCreated = date + " PM " + hh.number(tmpNum) + mmss;	
	}
	else if(tmpNum < 10)	
		m_contentCreated = date + " AM 0" + hh.number(tmpNum) + mmss;
	else
		m_contentCreated = date + " AM " + hh.number(tmpNum) + mmss;
}
// } added eugene.seo 2012.12.06 for convert contentCreated date format into file info

// { modified by ravikanth 25-12-12
// { modified by ravikanth 26-03-13
bool AppMediaPlayer_Controller_Photo::isFileSupported(const QString file_path)
{    
    QString filename = (file_path == "") ? GetCurrentFileFullPathName(): file_path;

    MP_LOG << "Filename: " << filename << "q_Filename: "<<m_unsupportedFileInfo.q_Filename << LOG_ENDL;

	// modified for ITS 0206895
    if(m_unsupportedFileInfo.q_Filename == filename)
    {
        MP_LOG << "b_Supported: " << m_unsupportedFileInfo.b_Supported << LOG_ENDL;
        return m_unsupportedFileInfo.b_Supported;
    }
    bool retVal = false;
	// { removed by eugene.seo 2013.03.29
    /*
    MediaInfoLib::MediaInfo MI;
    MI.Open(filename.toStdWString());
    m_imageInfoStruct.i_ImageWidth = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Image, 0, _T("Height"))).toInt();
    m_imageInfoStruct.i_ImageHeight = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Image, 0, _T("Width"))).toInt();
    m_imageInfoStruct.i_FileZise = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_General, 0, _T("FileSize"))).toInt();
    MI.Close();	
    m_currentLoadingFileSize = (double)m_imageInfoStruct.i_FileZise/1048576;
    MP_LOG << "Width: " << m_imageInfoStruct.i_ImageWidth << ", Height: " << m_imageInfoStruct.i_ImageHeight  << LOG_ENDL;
    MP_LOG << "Size 1: " << m_currentLoadingFileSize << LOG_ENDL;
	*/	
	// } removed by eugene.seo 2013.03.29
	
	// { modified by eugene.seo 2013.03.29
	double imageFileSize = 0;
		if(filename != NULL && ((m_pTrackerAbstractor && m_pTrackerAbstractor->FileExists(filename) != false) || QFile::exists(filename) != false))    // modified by sungha.choi 2013.07.31 for scandir performance
	{		
		QFile imageFile(filename);
		imageFileSize = (double)imageFile.size()/1048576;
		MP_LOG << "File size: " << imageFileSize << LOG_ENDL;
		m_currentLoadingFileSize = imageFileSize; // modified by ravikanth 07-04-13
	}     
	
	if( imageFileSize > 0 && imageFileSize <= PHOTO_MAX_SUPPORTED_SIZE)
	{
			// { modified by ravikanth 08-04-13
            MediaInfoLib::MediaInfo MI;
            MI.Open(filename.toStdWString());
            m_imageInfoStruct.i_ImageWidth = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Image, 0, _T("Height"))).toInt();
            m_imageInfoStruct.i_ImageHeight = QString::fromStdWString(MI.Get(MediaInfoLib::Stream_Image, 0, _T("Width"))).toInt();
            MI.Close();
			// { modified by eugene.seo 2013.04.09 
			/*
            		if(m_imageInfoStruct.i_ImageWidth > PHOTO_MAX_SUPPORTED_WIDTH
                    		|| m_imageInfoStruct.i_ImageHeight > PHOTO_MAX_SUPPORTED_HEIGHT )
                    */
            if(m_imageInfoStruct.i_ImageWidth * m_imageInfoStruct.i_ImageHeight > PHOTO_MAX_SUPPORTED_WIDTH * PHOTO_MAX_SUPPORTED_HEIGHT )
			// } modified by eugene.seo 2013.04.09 
            {
                retVal = false;
            }
			// { modified by ravikanth 13.06.2013 for ITS 0172957
            else if((m_imageInfoStruct.i_ImageWidth * m_imageInfoStruct.i_ImageHeight) > (PHOTO_MAX_RESOLUTION_WIDTH * PHOTO_MAX_RESOLUTION_HEIGHT))
            {
                resizeAndResetImage(filename, NULL);
                retVal = true;
            }
            else if(m_imageInfoStruct.i_ImageWidth == 0 || m_imageInfoStruct.i_ImageHeight == 0)
            {
                // some times for a valid image header info width and height is 0
                // use header info for performance for valid data if info is not valid
                // query data from image
		QImage image(filename);
                m_imageInfoStruct.i_ImageWidth = image.width();
                m_imageInfoStruct.i_ImageHeight = image.height();
                MP_LOG << "Width: " << m_imageInfoStruct.i_ImageWidth << ", Height: " << m_imageInfoStruct.i_ImageHeight << LOG_ENDL;
                // { modified by eugene.seo 2013.02.06 for unsupported image file
                if( m_imageInfoStruct.i_ImageWidth == 0 || m_imageInfoStruct.i_ImageHeight == 0 ||
						//imageWidth > PHOTO_MAX_SUPPORTED_WIDTH || imageHeight > PHOTO_MAX_SUPPORTED_HEIGHT ) // delete by eugene.seo 2013.04.09
                                                m_imageInfoStruct.i_ImageWidth * m_imageInfoStruct.i_ImageHeight > PHOTO_MAX_SUPPORTED_WIDTH * PHOTO_MAX_SUPPORTED_HEIGHT ) // added by eugene.seo 2013.04.09
                {
                    retVal = false;
                }
                else if((m_imageInfoStruct.i_ImageWidth * m_imageInfoStruct.i_ImageHeight) > (PHOTO_MAX_RESOLUTION_WIDTH * PHOTO_MAX_RESOLUTION_HEIGHT))
                {
                    resizeAndResetImage(filename, &image);
                    retVal = true;
                }
				// } modified by ravikanth 13.06.2013 for ITS 0172957
                else // added by ravikanth 25-04-13
                {
                    retVal = true;
                }
            }
			// } modified by ravikanth 08-04-13
        else
            retVal = true;
        // } modified by eugene.seo 2013.02.06 for unsupported image file
    }
    else
    {
        retVal = false; // unsupported value greater than 5
    }
	// } modified by eugene.seo 2013.03.29
    m_unsupportedFileInfo.q_Filename = filename;
    m_unsupportedFileInfo.b_Supported = retVal;
    return retVal; 
}

// modified by ravikanth 13.06.2013 for ITS 0172957
void AppMediaPlayer_Controller_Photo::resizeAndResetImage(QString imgPath, QImage *image)
{
    QImageReader reader(imgPath); // modified for ITS 0206170
    if(m_enginePhoto->isSlideShow() || (reader.format() == "png")) // modified by ravikanth 04-07-13 for 2/5 times show black screen during sliding show
        return;

    QString thumbnailPath = QString(QCryptographicHash::hash(QString(QString("file://") + imgPath).toUtf8(), QCryptographicHash::Md5).toHex())
            + QString(".jpg");
    thumbnailPath.prepend(relativeThumbnailPath());
    MP_LOG << "imgPath: " << imgPath << " thumbnailPath: " << thumbnailPath << LOG_ENDL;
    if(!QFile::exists(thumbnailPath))
    {
        QImage img;
        if(image == NULL)
        {
            img.load(imgPath);
        }
        else
        {
            img = *image;
        }
        img = img.scaled(QSize(PHOTO_MAX_DISPLAY_WIDTH, PHOTO_MAX_DISPLAY_HEIGHT), Qt::KeepAspectRatio, Qt::FastTransformation);
        if(img.width() == 0 || img.height() == 0 || img.isNull())
        {
            MP_LOG << "Cannot scale thumbnail image" << LOG_ENDL;
            return;
        }
        if(!m_readyToPWOFF && img.save(thumbnailPath, "JPG", -1)) //modified by honggi.shin, 2014.05.23, for finishing thumbnail operation.
        {
			// image saved successfully 
            //m_PlayList[m_CurSource].list->replace(m_PlayList[m_CurSource].index, thumbnailPath);
        }
        else
        {
            MP_LOG << "Cannot save thumbnail image: " << thumbnailPath << LOG_ENDL;
        }
    }
}

double AppMediaPlayer_Controller_Photo::currentLoadingFileSize()
{
    return m_currentLoadingFileSize;
}
// } modified by ravikanth 25-12-12
void AppMediaPlayer_Controller_Photo::updateSupportedInfo(PHOTO_SOURCES_LIST mediaSource)
{
    MP_LOG << LOG_ENDL;
    PlayList* tmpList = NULL;
    tmpList = getPointerToPL(mediaSource);
    if(tmpList->list->count() > 0)
    {
        for(int i = 0 ;  i <  tmpList->list->count(); i++)
        {
            // MP_LOG << " filename " << tmpList->list->at(i) << LOG_ENDL; // commented by ravikanth 02-04-13
            if(!isFileSupported(m_FolderPath[mediaSource] + "/" + tmpList->list->at(i)))
            {
                tmpList->list->removeAt(i);
                i = i - 1;
            }
        }
    }
}
// } modified by ravikanth 26-03-13

// { modified by Sergey for ITS#159209 for 27.04.2013
bool AppMediaPlayer_Controller_Photo::setPlaylistIndexWrtFile(QString filepath)
{
    int index = m_PlayList[getCurSourceID()].list->indexOf(filepath);
    m_PlayList[getCurSourceID()].index = (index != -1) ? index : 0;
    return ( index == -1) ? false : true; // modified by ravikanth 27-07-13 for ITS 181678
}
// } modified by Sergey for ITS#159209 for 27.04.2013

// { modified by ravikanth 08-04-13
void AppMediaPlayer_Controller_Photo::updateUnsupportedInfo(const QString filepath)
{
    MP_LOG << LOG_ENDL;
    PlayList* tmpList = NULL;
    tmpList = getPointerToPL(getCurSourceID());
    if(tmpList != NULL && tmpList->list->count() > 0)
    {
        QString filename = filepath.right(filepath.size() - filepath.lastIndexOf("/") - 1);
        int index = tmpList->list->indexOf(filename);
        if(index > 0)
        {
        tmpList->list->removeAt(index);
        }
    }
}

void AppMediaPlayer_Controller_Photo::handleUnsupportedFile(const QString filepath)
{
    PlayList* playlist = getPointerToPL(getCurSourceID());

    if(playlist != NULL && playlist->list->count() > 0)
    {
        if(!m_corruptedFiles[m_CurSource].contains(filepath))
        {
            m_corruptedFiles[m_CurSource].append(filepath);
            m_corruptedImages[m_CurSource]++;
        }

        if(m_corruptedFiles[m_CurSource].count() >= playlist->list->count())
        {
			// modified for icon disable on all unsupported files
            ETrackerAbstarctor_VolumeType volumeType;
            if(m_CurSource == PHOTO_SOURCE_JUKEBOX)
                volumeType = eJukeBox;
            else if(m_CurSource == PHOTO_SOURCE_USB_1)
                volumeType = eUSB_FRONT;
            else if(m_CurSource == PHOTO_SOURCE_USB_2)
                volumeType = eUSB_REAR;

            //MP_LOG << "Total Count: " << m_pTrackerAbstractor->GetPhotoFileNum(volumeType) <<" PlayList Count: "<<playlist->list->count() << LOG_ENDL;
            if(m_pTrackerAbstractor->GetPhotoFileNum(volumeType) == playlist->list->count())
            {
                int source = (int)m_CurSource;
                emit handleAllUnsupportedFiles(m_CurSource);
            }
        }
        else
        {
			// modified by ravikanth 23.06.13
            // created timer on first unsupported image to not to effect bootup time
            if(unsupportedImageTimer == NULL)
            {
                unsupportedImageTimer = new QTimer(this);
                unsupportedImageTimer->setInterval(2000);
                unsupportedImageTimer->setSingleShot(true);
                connect(unsupportedImageTimer, SIGNAL(timeout()), m_enginePhoto, SLOT(skip()));
            }
            if(unsupportedImageTimer->isActive())
            {
                unsupportedImageTimer->stop();
            }
            unsupportedImageTimer->start();
            //QTimer::singleShot(2000, m_enginePhoto, SLOT(skip()));
        }
    }
} // added by Sergey 22.05.2013

QString AppMediaPlayer_Controller_Photo::relativeThumbnailPath()
{
    switch(getCurSourceID())
    {
    case PHOTO_SOURCE_JUKEBOX:
        return QString(lThumbnailsDirJBox);
        //break; // commented by cychoi 2013-07-15 for Prevent fix
    case PHOTO_SOURCE_USB_1:
        return QString(lThumbnailsDirUSB1);
        //break; // commented by cychoi 2013-07-15 for Prevent fix
    case PHOTO_SOURCE_USB_2:
        return QString(lThumbnailsDirUSB2);
        //break; // commented by cychoi 2013-07-15 for Prevent fix
    default:
        break;
    }

    // added by cychoi 2013-07-15 for Prevent fix
    return QString();
    // added by cychoi 2013-07-15
}

void AppMediaPlayer_Controller_Photo::ClearThumbnailData( PHOTO_SOURCES_LIST source )
    {
        PhotoThumbnailer::instance()->ClearThumbnailData((PHOTO_THUMBNAIL_MEDIA_LIST)source);
    }
// } modified by ravikanth 08-04-13

// modified by ravikanth 13.06.2013 for ITS 0172957
bool AppMediaPlayer_Controller_Photo::isHighResolutionImage()
{
    return ((m_imageInfoStruct.i_ImageWidth * m_imageInfoStruct.i_ImageHeight)
            > (PHOTO_MAX_RESOLUTION_WIDTH * PHOTO_MAX_RESOLUTION_HEIGHT));
}
// modified by ravikanth 29-06-13
void AppMediaPlayer_Controller_Photo::stopUnsupportedSkipTimer()
{
    if(unsupportedImageTimer != NULL)
    {
        if(unsupportedImageTimer->isActive())
        {
            unsupportedImageTimer->stop();
        }
    }
}

// modified by ravikanth 27-07-13 for ITS 181678
void AppMediaPlayer_Controller_Photo::SetIndexForPhotoSource(PHOTO_SOURCES_LIST source, int index)
{
    MP_LOG << LOG_ENDL;
    if( m_PlayList[source].list->isEmpty() || ( m_PlayList[source].list->count() <= index) || index < 0) // modified by Dmitry 04.08.13 for photo crash
    {
        return;
    }
    m_PlayList[source].index = index;
}

QString AppMediaPlayer_Controller_Photo::GetFileFullPathName(PHOTO_SOURCES_LIST source)
{
    //MP_LOG << source << m_PlayList[source].list->count() << m_PlayList[source].index << LOG_ENDL;

    QString retStr = "";

    if((m_PlayList[source].list->count() <= m_PlayList[source].index) // modified by Dmitry 04.08.13 for photo crash
            || (m_PlayList[source].index < 0 ) || m_PlayList[source].list->isEmpty())
    {
        MP_LOG << "Fatal error index out of range! Reset to null" << LOG_ENDL;
        return retStr;
    }
    retStr = m_PlayList[source].list->at(m_PlayList[source].index);

    //MP_LOG << retStr << LOG_ENDL;
    return retStr;
}

// modified for ITS 0206895
void AppMediaPlayer_Controller_Photo::setCurrentFileSupported(QString filename)
{
    //MP_LOG << filename << LOG_ENDL;
    m_unsupportedFileInfo.q_Filename = filename;
    m_unsupportedFileInfo.b_Supported = true;
}
