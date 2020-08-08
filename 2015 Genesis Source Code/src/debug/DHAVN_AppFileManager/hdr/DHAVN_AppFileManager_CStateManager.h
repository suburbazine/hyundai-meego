#ifndef DHAVN_APPFILEMANAGER_STATEMANAGER_H
#define DHAVN_APPFILEMANAGER_STATEMANAGER_H

#include <QObject>
#include <QString>
#include <QAbstractListModel>
#include <QDeclarativeContext>
#include <QQueue>
#include <QTranslator>
#include "usbcontroller.h"
#include "DHAVN_AppFileManager_Logger.h"
#include "DHAVN_AppFileManager_Shared.h" // added by Dmitry 09.08.13
#include <QDeclarativeView>//added by edo.lee 2012.11.04 for statusbar


#define STATUSBAR_NEW 1 //added by edo.lee 2012.12.07 for statusbar	


class PictureModelItem;
class VideoModelItem;
class AudioModelItem;
class BaseListModel;
class AudioListModel;
class VideoListModel;
class PictureListModel;
// removed by Dmitry 14.08.13 for ITS0183845

class FileManagerEngine;
class FileOperationsHelper;
class UI_Main;

enum EAppFileManagerState
{
    eInvalidState = -1,
    eUsbMusicTitle = 0,
    eUsbMusicArtist,
    eUsbMusicAlbom,
    eUsbVideo,
    eUsbVideoCopy,
    eUsbPictureShow,
    eUsbPictureCopy,
    eJukeMusicTitle,
    eJukeMusicArtist,
    eJukeMusicAlbom,
    eJukeVideo,
    eJukeVideoCopy,
    eJukePictureShow,
    eJukePictureCopy,
    eJukeListVideo,
    eUsbListVideo,
    eJukeListPicture,
    eUsbListPicture,
    eUsbMusicList,
    eJukeMusicList,
    eJukeVideoMove,
    eJukePictureMove
};

enum EMediaDataRequestType
{
    // { modified by kihyung 2013.07.16 for ITS 0178868
    eRequestInvalid = 0,

    eMusicAllTitleRequest,
    eMusicAllArtistRequest,
    eMusicAllAlbomRequest,
    eMusicAllTitleByArtistRequest,

    eVideoFilesInFolderRequest     = 1000,
    eVideoFilesInSubFolders        = 1010,    

    ePictureFolderAllRequest,
    ePictureFilesInFolderRequest,
    ePictureFilesInSubFolders,

    eMusicArtistsFileListRequest,
    eMusicAlbumsFileListRequest
    // } modified by kihyung 2013.07.16 for ITS 0178868
};

enum EShowCountState
{
    eShowNone = 0,
    eShowTotal,
    eShowSelected
};

struct AppFileManagerMusicData
{
    AppFileManagerMusicData() : fullFilePathName(""), name(""),artist(""),album(""){};

    QString fullFilePathName;
    QString name;
    QString artist;
    QString album;
};

class CStateManager : public QObject
{
    Q_OBJECT
    //Q_ENUMS( EDevice )
    //Q_ENUMS( EFileType )
    //Q_ENUMS( EMusicFilter )
    Q_DISABLE_COPY(CStateManager)
    // USE_FM_LOG_UTIL
public:
    CStateManager(QObject *parent, bool front, int state, /*USBController* controller,*/
                  FileManagerEngine* engine, QDeclarativeContext *ctxt, QTranslator *translator);
    ~CStateManager();

Q_SIGNALS:
    void startPopupShouldShow();
    void copyStarted();
    void copyReject();
    void copyComplete();
    void copyAbort();

    //void postEventToStart(int state, QString currentPath);//SIGNAL FOR ENGINE
    void initState(int device, int fileType, int musicFilter,int typeMode);
    void currentPathChanged();
    void currentPlayFileNameChanged(QString currentName);
    void updateThumbnailsByQML(); //added by honggi.shin, 2014.03.06, for avoid hanging on to AFM when updating thumbnail
    void initializeCompleted();

    void rootStateChanged();
    void currentFolderChanged();
// removed by Dmitry 27.07.13
    //void setMenuKey(bool menuFlag); // removed by Dmitry 15.04.13 for ISV79935
	// modified for ITS 0218316
    void showTotalItems(QString infoText, bool m_isFolder, QString folderCount, QString fileCount); // modified by eugene 2012.12.12 for NewUX - Photo #5-2
    void showCountStateChanged();
    // removed by Dmitry Bykov 05.04.13 for ISV78029
	
    void updateModeArea();
    void  updateModelFinish();
    void modelChanged();

    void usbRemoved(int nPort); // modified by eugene.seo 2013.05.29
    void isMenuVisibleChanged(); // added by Dmitry 15.04.13 for ISV79935
    void showContent(bool show); // added by Dmitry 03.10.13 for ITS0187937

public: //call backs
    //on Audio
    void AudioRequestCompleted(QList<AudioModelItem>* info, int requesType, int codeError, QVariant userData);
    //on audio set
    void AudioRequestBySetParamCompleted(QList<QString>* info, int requesType, int codeError, QVariant userData);

    void deleteFolder( const QString & value );
    void renameFolder( const QString & src, const QString & dst );
    void renameFile( const QString & src, const QString & dst );

    int CopyFileToMyFrame(const QString &path, bool overwrite); // modified by ravikanth 10-07-13 for ITS 179182
    QTranslator* GetTranslator(){return m_pTranslator;}//added by edo.lee 2013.06.21

public slots:
    void AppPostEventHandler(int state, QString currentPath, QString fullFileName,
                             int screen);
    // { modified by kihyung 2012.08.02 for CR 12191
    /*
    void onUSBMounted(const QString& path);
    void onUSBRemoved();
    */
    void onUSBMounted(const QString& path, const int nPort);
    void onUSBRemoved(const int nPort);
    // } modified by kihyung
    void updateState();
    void AppPostEventCurrentPlayChanged(QString param, int type, int screen);
// removed by Dmitry 08.08.13
    void onDeleteComplete(QString path);
// removed by Dmitry 21.08.13
    void onFilesChanged();
    void onSetMoveStatus(bool status);// add by eunhye 2012.09.21 for CR 9243
    void onPlayingDirectoryChanged(QString playingDirectory); // added by wspark 2013.02.08 for ISV 70322
    void onFileSystemChanged(); // added by kihyung 2013.2.17
// removed by Dmitry 03.10.13 for ITS0187937
    void onUpdateUIData(QVector<QStringList> *list); // added by Dmitry 14.08.13 for ITS0183845
    void onReadCompleted(ETrackerAbstarctor_VolumeType volume, int status); // added by Dmitry 21.08.13
    void onSetFileOperationStopMedia(int startState, bool isFront); // modified for ITS 0214738

public:
    Q_INVOKABLE void StateChangeHandler(int index);
    Q_INVOKABLE void StateChangeHandler(QString string);
    Q_INVOKABLE void StateChangeUpdate();
    Q_INVOKABLE bool ShouldChangePath();
    Q_INVOKABLE bool IsMoveMode();
    Q_INVOKABLE bool PictureFolderChangedHandler(int index, bool up = false);
    Q_INVOKABLE bool VideoFolderChangedHandler(int index, bool up = false);
    Q_INVOKABLE bool PictureCopyFolderChangedHandler(int indexItem, bool up = false);
    Q_INVOKABLE bool VideoCopyFolderChangedHandler(int indexItem, bool up = false);
    Q_INVOKABLE bool FolderChangedHandler( int indexItem, bool up );
    Q_INVOKABLE EAppFileManagerState GetCurrentState();
    Q_INVOKABLE void CopySelectedItems();
    Q_INVOKABLE void MoveSelectedItems();
    Q_INVOKABLE void DeleteSelectedItems();
    Q_INVOKABLE void CopyOrDeleteMusicSelectedItems(bool mode = false);//false - delete  true - copy
// removed by Dmitry 22.05.13
    Q_INVOKABLE QString GetCurrentPresentPath();

    Q_INVOKABLE bool FolderUp();
    Q_INVOKABLE void CreateFolder( QString path );

    Q_INVOKABLE void setDevice( int dev );
    Q_INVOKABLE void setFileType( int type );
    Q_INVOKABLE void setMusicFilter( int filter );
    Q_INVOKABLE void setCopyView( bool isCopyView );
    Q_INVOKABLE void breakCopyMode();

    // { changed by junam 2012.12.01 for CR16077
    //Q_INVOKABLE void cancelCopyMode(); // added by wspark 2012.10.04 for sanity AK339
    Q_INVOKABLE bool cancelCopyMode();
    // } changed by junam

    Q_INVOKABLE void setMoveMode( bool isMoveMode );
    Q_INVOKABLE void breakMoveMode();

    Q_INVOKABLE QString GetCurrentPlayName();
    Q_INVOKABLE QString GetCurrentPlayFolder();

    Q_INVOKABLE bool CheckFolder( QString folderName );
    Q_INVOKABLE void setCurrentItemNumber(int index, bool is_folder);
    Q_INVOKABLE int GetCurrentMediaState(); // added by wspark 2012.12.13 for CR16396
    Q_INVOKABLE QString GetCurrentPlayCompleteItemPath(); // modified by ravikanth 14-07-13 for ITS 0179049

//{Commented by Alexey Edelev 2012.10.08
//    // { added by junggil 2012.10.04 for CR13505 Media Apps - delta UIS 2.15 to 2.17  
//    Q_INVOKABLE bool getScrollingTicker();
//    Q_INVOKABLE QString getFocusedItemName(int index);
//    Q_INVOKABLE QString makeElidedString( const QString &fullString, const QString &fontFamily, int pixelSize, int maxWidth );
//    // } added by junggil 
//}Commented by Alexey Edelev 2012.10.08
    Q_INVOKABLE int getStringWidth( const QString &fullString, const QString &fontFamily, int pixelSize ); //restored 20130621 for [KOR][ISV][83967][C] (aettie.ji)
    Q_INVOKABLE bool editModeLaunchFromAVP() { return (m_editModeFromAVP && !m_fmLaunchUnsupportedFileCopy); } // modified by ravikanth for ITS 0184642

    BaseListModel* GetAudioModel();
    BaseListModel* GetVideoModel();
    BaseListModel* GetPictureModel();
// removed by Dmitry 22.08.13
    FileOperationsHelper* GetFileManager();
// removed by Dmitry 14.08.13 for ITS0183845

    void setupInitialQmlState(int state); // modified by Dmitry 09.08.13

    Q_PROPERTY(int device READ device);
    Q_PROPERTY(int fileType READ fileType);
    Q_PROPERTY(int musicFilter READ musicFilter);
    Q_PROPERTY(int appMode READ appMode)

    Q_PROPERTY( bool rootState READ getRootState NOTIFY rootStateChanged )

    Q_PROPERTY( QString currentFolder READ getCurrentFolder NOTIFY currentFolderChanged )
    Q_PROPERTY( int showCountState READ getShowCountState
               NOTIFY showCountStateChanged)
    // { added by wspark 2012.12.13 for CR16396
    Q_PROPERTY( bool emptyState READ getEmptyState WRITE setEmptyState )
    Q_PROPERTY( bool isMenuVisible READ isMenuVisible WRITE setIsMenuVisible NOTIFY isMenuVisibleChanged) // added by Dmitry 15.04.13 for ISV79935
    bool getEmptyState() { return m_emptyState; }
    void setEmptyState(bool state) { m_emptyState = state; };
    // } added by wspark

    QString getJukeboxPath();
    void UncheckAllItems();
    UI_Main* getUIControl();
	QString getCurrentPath(); // added by eugene.seo 2013.05.29
    bool isVideoState(); // added by Dmitry 09.08.13
    FileManagerEngine* getEngine() { return appEngine; } // added by Dmitry 23.10.13

public:
    int device();
    int fileType();
    int musicFilter();
    int appMode();

    void setRootState(bool value);
    void changeState();
    int getShowCountState();
    Q_INVOKABLE void showCountInfo(); // modified by Dmitry 27.07.13
    void hideCountInfo();
    bool isFrontInstance();

    void browseMode(bool value);
    void NavigatorUpdate( QString path );
    QString getCurrentFolder();
    int getCurrentCopyCount() { return   m_countofCopyingFiles; } // added by wspark 2012.09.21 for CR6084
// removed by Dmitry 27.07.13

// added  by Dmitry 15.04.13 for ISV79935
    bool isMenuVisible() { return m_bIsMenuVisible; }
    void setIsMenuVisible(bool isVisible) { m_bIsMenuVisible = isVisible; emit isMenuVisibleChanged(); }
// added by Dmitry 15.04.13 for ISV79935

	// modified by ravikanth 25.06.13 for ITS 0175213, 0175370
    void StateChangeHandler(int index, bool switchToRoot);
    void setDevice( int dev , bool switchToRoot);
    int currentCopyMode(){ return m_copyModeType; } // modified by ravikanth 10-08-13 for ITS 0183707
    void setEditModeLaunchFromAVP(bool value, bool unsupportedFile); // modified by ravikanth for ITS 0184642
    void resetCurrentItemNumber(); // modified by ravikanth for ITS 0187368
    EAppFileManagerStartState FileOperationStopMediaType() { return this->m_currentCopyStopState; } // modified for ITS 0214738

    // { added by Puneet 2012.12.28 for CR 14749
private slots:
    void onThumbNailGenerated();
    //}

private:
    void InitialPath();
    void initListModels();
    //VIDEO
    void StartVideoInfoModelCreating();

    //PICTURE
    void StartPictureInfoModelCreating();

    //AUDIO
    void StartAudioInfoModelCreatingWithoutNavigation();
    void DeleteMusicFilesByPathList(QList<QString>* list);
    void CopyMusicFilesByPathList(QList<QString>* list);

    //CURRENT PLAY
    void SaveCurrentPlayData(QString param, int type);
    void SetCurrentPlayItem(bool avoidFocusUpdate = false);
    void SaveCurrentPlayDataByPathWithoutUpdateUI(QString fullFileName,int state);

    void checkQueue();
    void PrepareFolderInfo( const QString & path );

    void PrepareCheckedList( BaseListModel * model );

    bool getRootState();
// removed by Dmitry 27.07.13

    void setCurrentState(EAppFileManagerState state);
	void setDisplayOSDView(); //added by edo.lee 2012.12.10 for statusbar
    void setCurrentPath(const QString & path); // modified by Dmitry 22.05.13
    void resetUSBMountPath(QString currentPath); //modified by ravikanth 27.06.13 for ITS 0176629
	
	// modified by ravikanth 30.06.13 for ITS 0177250
    void deleteAllModelItems();
    void deleteSelectedModelItems();
    void resetMountPath();
    ETrackerAbstarctor_VolumeType volumeType(QString currentPath); // modified by ravikanth 02-09-13
    void SetFileOperationModeType(); // modified for ITS 0212431

private:
    FileManagerEngine* appEngine;

// modified by Dmitry 22.05.13
    QString currentPath;
    QString currentCopyPath;
    QString previousPath;
    QString newPath;
// modified by Dmitry 22.05.13

    EAppFileManagerState currentState;
// removed by Dmitry 17.08.13

    //video and picture folder
    int numberOfItemInCurrentFolder;
    int lastIndexOfFolderInCurrentFolder;
    int indexOfCurrentRequestFolder;

// removed by Dmitry 22.05.13

    int currentMusicFilter;

    QScopedPointer<AudioListModel>  musicListModel;
    QScopedPointer<VideoListModel>  videoListModel;
    QScopedPointer<PictureListModel>  pictureListModel;
// removed by Dmitry 22.08.13
    QScopedPointer<FileOperationsHelper> copyFileManager;
// removed by Dmitry 14.08.13 for ITS0183845

// modified by Dmitry 22.05.13
    // show current play
    QString currentPlayPath;
    QString currentPlayName;
    QString currentPlayAudioAlbum;
    QString currentPlayAudioArtist;
// modified by Dmitry 22.05.13

    QDeclarativeContext *m_pCtxt;
    QQueue<QString> folderQueue;

    bool isInitialized;

    QList<QString> checkedList;
    QString checkedPath;

    bool m_rootState;
    QString m_pCurrentFolder;
// removed by Dmitry 27.07.13
    EShowCountState m_showCountState;
// removed by Dmitry 27.07.13
    QScopedPointer<UI_Main> uiControl;
    //USBController *fileController;
    bool frontInstance;
    bool m_pBrowseMode;
    QTranslator *m_pTranslator;
    EAppFileManagerState m_entryState; // added by Dmitry 17.08.13

    // { added by kihyung 2012.08.02 for CR 12191
    QStringList m_tUSBMountPath; 
    QList<int>  m_tUSBPortList;
    // } added by kihyung

    int m_countofCopyingFiles; // added by wspark 2012.09.21 for CR6084
	bool m_isFolder; // added by eugene 2012.12.12 for NewUX - Photo #5-2
	QDeclarativeView* m_pOSDview;//added by edo.lee 2012.12.08 for statusbar

// removed by Dmitry 22.05.13
	bool m_emptyState; // added by wspark 2012.12.13 for CR16396
    bool m_bIsMenuVisible; // added by Dmitry 15.04.13 for ISV79935

    // modified by ravikanth 10-08-13 for ITS 0183707
    int m_copyModeType;
    bool m_editModeFromAVP; // modified by ravikanth for ITS 0184642
    bool m_fmLaunchUnsupportedFileCopy;
    EAppFileManagerStartState m_currentCopyStopState; // modified for ITS 0214738


private:
    static const int stateTable[18][4];

public:
// removed by Dmitry 21.08.13

    Q_ENUMS( EFileType )
    enum EFileType
    {
       eAudio = 0,
       eVideo,
       ePicture,
       undefined //added by aettie 2013.03.28 for Static defect 7545
    };

    Q_ENUMS( EMusicFilter )
    enum EMusicFilter
    {
       eArtist = 0,
       eAlbum,
       eSong
    };

    Q_ENUMS( EModeType )
    enum EModeType
    {
       eFileManager = 0,
       eFileList
    };
};

#endif // DHAVN_APPFILEMANAGER_STATEMANAGER_H
