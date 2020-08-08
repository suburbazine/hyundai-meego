#if !defined( APP_MEDIA_PLAYER_CONTROLLER_PHOTO_H )
#define APP_MEDIA_PLAYER_CONTROLLER_PHOTO_H

#include "controller/DHAVN_AppMediaPlayer_ControllerBase.h"
#include <QtCore>
#include <QObject>
#include <QGraphicsProxyWidget>

#include <imediainterface.h>
#include <usbcontroller.h>
#include <jukeboxcontroller.h>
#include <deckcontroller.h>
#include <iviewerinterface.h>
#include <DHAVN_SharedDataProvider.h>

enum PHOTO_SOURCES_LIST
{
   PHOTO_SOURCE_JUKEBOX = 0,
   PHOTO_SOURCE_USB_1,
   PHOTO_SOURCE_USB_2,
   PHOTO_SOURCE_MAX
};

// added by ruindmby 2012.11.30 for CR#15766
enum PHOTO_DISPLAY
{
   PHOTO_DISPLAY_FRONT = 0,
   PHOTO_DISPLAY_REAR,
   PHOTO_DISPLAY_NONE
};

typedef struct
{
   QString currentFile[PHOTO_SOURCE_MAX];     // Last played file full path
   QString curFolder[PHOTO_SOURCE_MAX];				// added by Sergey for ITS#159209 for 27.04.2013
   QStringList filesInCurFolder[PHOTO_SOURCE_MAX];  // added by Sergey for ITS#159209 for 27.04.2013
   int     slideshowDelay;
} PHOTO_LAST_STATE_T;
// added by ruindmby 2012.11.30 for CR#15766

// { added by ravikanth 25-12-12
struct ImageInfo
{
    // General
    QString   q_GeneralFormat;

    // Image
    int     i_ImageStreamCount;
    QString q_ImageFormat;
    int     i_ImageWidth;
    int     i_ImageHeight;
    int     i_FileZise; // modified by ravikanth 26-03-13
};
// } added by ravikanth 25-12-12

#define PHOTO_SOURCE_MASK_JUKEBOX   1
#define PHOTO_SOURCE_MASK_USB1      2
#define PHOTO_SOURCE_MASK_USB2      4
//{added by aettie.ji 2013.01.17 for ISV 68019
#define PHOTO_SOURCE_MASK_MAX       7
#define PHOTO_SOURCE_MASK_USB1_JUKEBOX  3
#define PHOTO_SOURCE_MASK_USB2_JUKEBOX  5
//}added by aettie.ji 2013.01.17 for ISV 68019
class AppMediaPlayer_EnginePhoto; // added by Sergey 22.05.2013

class AppMediaPlayer_Controller_Photo : public AppMediaPlayer_Controller_Base
{
   Q_OBJECT

//   friend class AppMediaPlayer_SettingsStorage; // removed by ruindmby 2012.11.30 for CR#15766

public:
   enum SCREEN_SIZE
   {
      SCREEN_HEIGHT = 720,
      SCREEN_WIDTH = 1280
   };

// removed by ruindmby 2012.11.30 for CR#15766
//   typedef struct
//   {
//      QString currentFile[PHOTO_SOURCE_MAX];     // Last played file full path
//      int     slideshowDelay;
//   } PHOTO_LAST_STATE_T;

//   enum PHOTO_DISPLAY
//   {
//      PHOTO_DISPLAY_FRONT,
//      PHOTO_DISPLAY_REAR,
//      PHOTO_DISPLAY_NONE
//   };
// removed by ruindmby 2012.11.30 for CR#15766

   AppMediaPlayer_Controller_Photo( AppMediaPlayer_EngineMain &engineMain,
                                    AppMediaPlayer_EnginePhoto* enginePhoto); // added by Sergey 22.05.2013
   ~AppMediaPlayer_Controller_Photo();

   void SwitchMode(const PHOTO_SOURCES_LIST source);
   void SetSource(const PHOTO_SOURCES_LIST source);
   void InitSources();
   void changeCurSourceID(const PHOTO_SOURCES_LIST source);
   PHOTO_SOURCES_LIST getCurSourceID();
   void NextImage();
   void PrevImage();
   // void createPlayList(PlayList &playlist,const QString folder_path, const QString file_path, const PHOTO_SOURCES_LIST source); // removed by kihyung 2013.2.17

   QString GetCurrentFileFullPathName();
   // int GetSlideShowDelay(); // removed by ruindmby 2012.11.30 for CR#15766
   // modified by ravikanth 15-05-13
    int GetCurrentPlayListCount(); // removed by Sergey for ITS#159209 for 27.04.2013
    int GetCurrentIndex();		 // removed by Sergey for ITS#159209 for 27.04.2013
   // QString GetRestoredFile(const PHOTO_SOURCES_LIST source); // removed by ruindmby 2012.11.30 for CR#15766
   // QString GetFolderPathFromSource(const PHOTO_SOURCES_LIST source); // removed by kihyung 2013.2.17

   // { removed by kihyung 2013.2.17 
   /*
   void SetFolder (const QString &folder_path, const PHOTO_SOURCES_LIST source);
   void SetCurModePlaylist();
   bool SetFolderAndFile(const QString file_path, const PHOTO_SOURCES_LIST source);
   */
   // } removed by kihyung 2013.2.17
   
   // removed by ruindmby 2012.11.30 for CR#15766
   //void SetSlideShowDelay(int slideShowDelay) { m_nSlideshowDelay = slideShowDelay;}
   // removed by ruindmby 2012.11.30 for CR#15766

   bool isSourceEmpty(PHOTO_SOURCES_LIST source); /* Check for available images in Jukebox/USB */
   int getSourceMask(){ return m_SourceMask; }
   void addSource(int mask){ m_SourceMask |= mask; }
   void removeSource(int mask){ m_SourceMask &= ~mask; }

   // removed by ruindmby 2012.11.30 for CR#15766
   // void LoadLastStateParams();
   // void SaveLastStateParams();
   // removed by ruindmby 2012.11.30 for CR#15766

   void ClearPlayList( PHOTO_SOURCES_LIST source );
   void ClearThumbnailData( PHOTO_SOURCES_LIST source ); // modified by ravikanth 08-04-13
   // removed by ruindmby 2012.11.30 for CR#15766
   //void ClearStoredFilePath( PHOTO_SOURCES_LIST source );
   // int getPhotoDisplay(){ return ( int )m_CurDisplay; }
   // modified by ruindmby 2012.11.30 for CR#15766

   // { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
   void GetPhotoMetaDataInfo(const QString& fileFullPath);
   QString GetCurrentCreateDate(); 
   // void RequestComplete( QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID); // removed by kihyung 2013.2.17
   // } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
   void ConvertTimeFormat(const QString& localtime); // added eugene.seo 2012.12.06 for convert contentCreated date format into file info

// { added by ravikanth 25-12-12
   double currentLoadingFileSize();
   bool isFileSupported(const QString file_path);
// } added by ravikanth 25-12-12   

    void requestFirstFolder(PHOTO_SOURCES_LIST photoSource);
    void requestLastFolder(PHOTO_SOURCES_LIST photoSource, QString strFolder, QString strFileName);
    void requestAllFiles(PHOTO_SOURCES_LIST photoSource); // added by Sergey for ITS#159209 for 27.04.2013
    void requestFilesInFolder(QString folder, PHOTO_SOURCES_LIST photoSource); // added by Sergey for ITS#159209 for 27.04.2013

    void updatePlaylist(QStringList &photoList, QString strFMRequestFile, PHOTO_SOURCES_LIST mediaSource);

    QString currentFolderPath(PHOTO_SOURCES_LIST photoSource) { return m_FolderPath[photoSource];} //modified by ravikanth 29-03-13
    bool setPlaylistIndexWrtFile(QString filepath); //modified by ravikanth 27-07-13
    QString relativeThumbnailPath(); // modified by ravikanth 08-04-13
    bool isHighResolutionImage(); // modified by ravikanth 13.06.2013 for ITS 0172957
    void stopUnsupportedSkipTimer(); // modified by ravikanth 29-06-13
	// modified by ravikanth 27-07-13 for ITS 181678
    void SetIndexForPhotoSource(PHOTO_SOURCES_LIST source,int index);
    QString GetFileFullPathName(PHOTO_SOURCES_LIST source);
    void setCurrentFileSupported(QString filename); // modified for ITS 0206895
    static void setReadyToPWOFF(bool value); //2014.05.23

public slots:
   void HandleError(int errorCode);
   // added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
   void HandleRequestComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID);
   void HandleRequestComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType, uint reqUID); // added by Sergey for ITS#159209 for 27.04.2013
   void updateUnsupportedInfo(const QString filepath); // modified by ravikanth 08-04-13
   void handleUnsupportedFile(const QString filepath); // added by Sergey 22.05.2013

signals:
   void showErrorPopup();
   // modified by ravikanth 28-0413
   void filesInCurFolder(QStringList, int mediaSource); // added by Sergey for ITS#159209 for 27.04.2013
   void handleAllUnsupportedFiles(int mediaSource);

private:
   //methods for internal usage only
     PlayList* getPointerToPL(const PHOTO_SOURCES_LIST source);
     QString* getPointerToFolder(const PHOTO_SOURCES_LIST source);
     void updateSupportedInfo(PHOTO_SOURCES_LIST mediaSource); // modified by ravikanth 26-03-13
     void resizeAndResetImage(QString imgPath, QImage *image); // modified by ravikanth 13.06.2013 for ITS 0172957

private:
   // { removed by kihyung 2013.2.17
   /*
   // { added by ruinseve for CR#12584
   bool isImageInFolder(const QString &folder_path);
   bool SearchImageInSubFolders(QString *folder_path);
   // } added by ruinseve for CR#12584.
   */
   // } removed by kihyung 2013.2.17   
   AppMediaPlayer_EnginePhoto* m_enginePhoto; // added by Sergey 22.05.2013
   PHOTO_SOURCES_LIST m_CurSource;

   //QString m_CurFilePath[PHOTO_SOURCE_MAX]; // modified by ruindmby 2012.11.30 for CR#15766
   QString m_FolderPath[PHOTO_SOURCE_MAX];
   PlayList m_PlayList[PHOTO_SOURCE_MAX];
   int      m_corruptedImages[PHOTO_SOURCE_MAX];
   QStringList  m_corruptedFiles[PHOTO_SOURCE_MAX]; // added by Sergey 22.05.2013

   // removed by ruindmby 2012.11.30 for CR#15766
   // int m_nSlideshowDelay;
   // PHOTO_LAST_STATE_T m_LastState[PHOTO_DISPLAY_NONE];
   // removed by ruindmby 2012.11.30 for CR#15766

   int m_SourceMask;
   // PHOTO_DISPLAY m_CurDisplay; // removed by ruindmby 2012.11.30 for CR#15766
   
   // { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
   CTrackerAbstractor   *m_pTrackerAbstractor;
   QString m_contentCreated; // } added eugene.seo 2012.12.06
   // } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
   // { added by ravikanth 25-12-12
   double m_currentLoadingFileSize; // return in MegaBytes
   ImageInfo m_imageInfoStruct;
   // } added by ravikanth 25-12-12

   QString m_strFMRequestFile;
   QTimer *unsupportedImageTimer; // modified by ravikanth 23.06.13

	// modified for ITS 0206895
   struct ImageSupportedInfo
   {
       QString   q_Filename;
       bool b_Supported;
   };
   ImageSupportedInfo m_unsupportedFileInfo;
   static bool m_readyToPWOFF; //2014.05.23
};

#endif
