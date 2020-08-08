#ifndef DHAVN_APPMEDIAPLAYER_LISTMODEL_H
#define DHAVN_APPMEDIAPLAYER_LISTMODEL_H

#include <QtCore>
#include <DHAVN_TrackerAbstractor.h>
#include <DHAVN_FileOperationsHelper.h>
#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include "util/DHAVN_AppMediaPlayer_MediaManager.h"//added by junam 2012.08.14 for 3PDMS 136415 


static const QString JUKEBOX_ROOT = "/usr_data/media/Music";
// static const QString USB_ROOT  = "/media";

static const int RESERVED_SCREEN_COUNT = 5; // change value from 14 by junam 2012.10.12 for CR14040
static const int ITEMS_PER_SCREEN = 16; // change value by junam from 16 2012.10.12 for CR14040
static const int FETCHED_BLOCK_ITEM_COUNT = RESERVED_SCREEN_COUNT * ITEMS_PER_SCREEN;
//{changed by junam 2013.08.06 for ONDEMAND_PLAYLIST
static const int MAX_LIMIT_BLOCK = 3;
//static const int SCREEN_COUNT_REMAIN_TO_EDGE = 4;
//static const int ITEM_COUNT_REMAIN_TO_EDGE = SCREEN_COUNT_REMAIN_TO_EDGE * ITEMS_PER_SCREEN; // 24
//}changed by junam

class AppMediaPlayer_Controller_Audio;

class LVEnums : public QObject
{
    Q_OBJECT
    Q_ENUMS(POPUP_TYPE_STATE_T);
    Q_ENUMS(OPERATION_TYPE_T);
    Q_ENUMS(SEARCH_CATEGORY_ID_T);
    Q_ENUMS(FOCUSED_ELEMENT_T);

public:

    typedef enum
    {
        POPUP_TYPE_UNDEFINED = -1,
        POPUP_TYPE_COPY,                          // 0
        POPUP_TYPE_COPY_ALL,                      // 1
        POPUP_TYPE_COPYING,                       // 2
        POPUP_TYPE_COPY_COMPLETE,                 // 3
        POPUP_TYPE_CANCEL_COPY_QUESTION,          // 4
        POPUP_TYPE_COPY_CANCELED,                 // 5
        POPUP_TYPE_MOVE,                          // 6
        POPUP_TYPE_MOVE_ALL,                      // 7
        POPUP_TYPE_MOVING,                        // 8
        POPUP_TYPE_MOVE_COMPLETE,                 // 9
        POPUP_TYPE_MOVE_CANCELED,                 // 10
        POPUP_TYPE_DELETE,                        // 11
        POPUP_TYPE_DELETING,                      // 12
        POPUP_TYPE_DELETE_COMPLETE,               // 13
        POPUP_TYPE_EDIT_MODE,                     // 14
        POPUP_TYPE_PL_FIRST_TIME,                 // 15
        POPUP_TYPE_PL_CREATE_NEW,                 // 16
        POPUP_TYPE_PL_ADD_ALL_FILES,              // 17
        POPUP_TYPE_PL_CHOOSE_PLAYLIST,            // 18
        POPUP_TYPE_PL_ADD_COMPLETE,               // 19
        POPUP_CREATE_FOLDER_INCORRECT_CHARACTER,  // 20
        POPUP_CREATE_FOLDER_EMPTY_NAME,           // 21
        POPUP_CREATE_FOLDER_NAME_IS_TOO_LONG,     // 22
        POPUP_FOLDER_ALREADY_EXIST,               // 23
        POPUP_FILE_ALREADY_EXIST,                 // 24
        POPUP_TYPE_RENAME,                        // 25
        POPUP_TYPE_REPLACE,                       // 26
        POPUP_TYPE_CAPACITY_ERROR,                // 27
        POPUP_TYPE_UNAVAILABLE_FILE,              // 28
        POPUP_TYPE_CORRUPT_FILE,                  // 29
        POPUP_TYPE_JUKEBOX_ERROR,                 // 30
        POPUP_TYPE_CAPACITY_VIEW,                 // 31
        POPUP_TYPE_CAPACITY_ERROR_MANAGE,         // 32
        POPUP_QUICK_FOLDER_DELETE,                // 33
        POPUP_QUICK_FILE_DELETE,                  // 34
        POPUP_QUICK_PLAYLIST_DELETE,              // 35
        POPUP_QUICK_PLAYLIST_FILE_DELETE,         // 36
        POPUP_TYPE_TIP_ADD_PLAYLIST,              // 37
        POPUP_TYPE_CLEAR_JUKEBOX,                 // 38
        POPUP_TYPE_FILE_IS_USED,                  // 39
        POPUP_TYPE_FOLDER_IS_USED,                // 40
        POPUP_TYPE_CANCEL_MOVE_QUESTION,          // 41
        POPUP_TYPE_GRACENOTE_INDEXING,            // 42
        POPUP_TYPE_IPOD_INDEXING,                 // 43 added by junam 2012.08.07 for CR 12811
        POPUP_TYPE_NO_MUSIC_FILES,                // 44 added by Alexey Edelev 2012.09.06 CR9413
        POPUP_TYPE_LOADING_DATA,                  // 45 add by lssanh 2012.09.10 for CR9362
        POPUP_TYPE_FORMATING,                     // 46 added by lssanh 2012.09.12 for CR13482
        POPUP_TYPE_FORMAT_COMPLETE,               // 47 added by lssanh 2012.09.12 for CR13482        
        POPUP_TYPE_DETAIL_FILE_INFO,              // 48 added by yongkyun.lee@lge.com    2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
        POPUP_TYPE_IPOD_AVAILABLE_FILES_INFO,     // 49 added by jungae 2012.10.10 CR 13753
        POPUP_TYPE_MLT_NO_MATCH_FOUND,            // 50 added by dongjin 2013.02.04 for ISV 70377
        POPUP_TYPE_COPY_CANCEL_INFO,               // 51
        POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM,        // 52 // modified by ravikanth 16-04-13
        POPUP_TYPE_FILE_CANNOT_DELETE,             // 53 // modified by ravikanth 27-04-13
        POPUP_TYPE_DELETE_INCOMPLETE,              // 54 // modified by ravikanth 21.06.13 for ITS 0174571
        POPUP_TYPE_CANCEL_COPY_FOR_DELETE_CONFIRM,          // 55 // modified by ravikanth 21-07-13 for copy cancel confirm on delete
        POPUP_TYPE_CANCEL_COPY_FOR_CLEAR_JUKEBOX,			// 56
        POPUP_TYPE_BT_DURING_CALL,		// 57 	//added by hyochang.ryu 20130731
        POPUP_TYPE_PLAY_UNAVAILABLE_FILE,         //58 //added by junam 2013.08.29 for ITS_KOR_185043
        POPUP_TYPE_HIGH_TEMPERATURE               // 59 // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling

    } POPUP_TYPE_STATE_T;

    typedef enum
    {
        OPERATION_NONE,
        OPERATION_COPY,
        OPERATION_MOVE,
        OPERATION_RENAME,
        OPERATION_DELETE,
        OPERATION_CLEAR,
        OPERATION_CREATE_FOLDER,
        OPERATION_ADD_TO_PLAYLIST
    } OPERATION_TYPE_T;

    typedef enum
    {
        CATEGORY_ALL = 0,
        CATEGORY_ARTIST,//{move  by yongkyun.lee@lge.com   2012.09.06 for:CR 12864 [Req changes] Music GUI 1.0.8 Search screen changes  
        CATEGORY_SONG,
        CATEGORY_ALBUM,
        //CATEGORY_ARTIST ////{move  by yongkyun.lee@lge.com   2012.09.06 for:CR 12864 [Req changes] Music GUI 1.0.8 Search screen changes 
    } SEARCH_CATEGORY_ID_T;

    typedef enum
    {
        FOCUS_NONE = 0,
        FOCUS_STATUS_BAR,
        FOCUS_MODE_AREA,
        FOCUS_CONTENT,
        FOCUS_BOTTOM_MENU,
        FOCUS_POPUP,
        FOCUS_OPTION_MENU,
        FOCUS_PLAYBACK_CONTROL,
        FOCUS_KEYPAD
    } FOCUSED_ELEMENT_T;
};

typedef struct
{
    QStringList imageSource;
    QStringList itemName; //contains real name without conversation.
    QStringList itemTitle; //Title to display
    QStringList itemSource;
    QStringList itemArtist; //added by junam 2012.10.12 for CR14040
    QList<wchar_t> itemQuickIndex; // added by jaehwan 2013.10.29 to improve qucikscroll performance
    QList<bool> isFolder;
    QList<bool> isItemCheckBoxMarked;
    QList<bool> isSelectable;
} LIST_DATA_INFO;

typedef struct
{
    QString tabId;
    QString queryStr;
    int itemIndex;
    int depth;
    //{removed by junam 2013.08.30 for ITS_KOR_187328
    //QString tmpTabId;
    //int tmpDepth; //[KOR][ISV][79672][B](aettie.ji)
    //}removed by junam
} REQUEST_DATA;

const QStringList SUPPORTED_MIME_TYPE (QStringList()<<"*.asf"<<"*.m4a"<<"*.mp3"<<"*.oga"<<"*.ogg"<<"*.wav"<<"*.wma"); // modified by eugene 2013.04.17

typedef struct
{
    QString artistName;
    QStringList albumsName;
} ARTIST_ALBUMS;

/* States for iPod special data could be added if necessary */
typedef enum
{
    UNDEFINED_STATE,  // 0
    PLAYLIST_DEPTH_0, // 1 Playlists
    PLAYLIST_DEPTH_1, // 2 Songs
    ARTIST_DEPTH_0,   // 3 Artists
    ARTIST_DEPTH_1,   // 4 Albums
    ARTIST_DEPTH_2,   // 5 Songs
    SONG_DEPTH_0,     // 6 Songs
    ALBUM_DEPTH_0,    // 7 Albums
    ALBUM_DEPTH_1,    // 8 Songs
    GENRE_DEPTH_0,    // 9 Genres
    GENRE_DEPTH_1,    // 10 Artists
    GENRE_DEPTH_2,    // 11 Albums
    GENRE_DEPTH_3,    // 12 Songs
    FOLDER_DEPTH_0,   // 13 Folders & songs
    FOLDER_DEPTH_1,    // 14 Songs

	// { Added by junam 2012.09.03 for CR13014
    ITUNES_DEPTH_0, //15
    ITUNES_DEPTH_1,
    ITUNES_DEPTH_2,

    PODCAST_DEPTH_0, //18
    PODCAST_DEPTH_1,
    PODCAST_DEPTH_2,

    AUDIOBOOK_DEPTH_0, //21
    AUDIOBOOK_DEPTH_1,
    AUDIOBOOK_DEPTH_2,

    COMPOSER_DEPTH_0, //24
    COMPOSER_DEPTH_1,
    COMPOSER_DEPTH_2,
	// } Added by junam

    BOTTOM_DEPTH, //27 //added by junam 2012.11.29 for CR16170

} CATEGORY_STATE_T;

// { added by jaehwan 11.11 for quick index table
typedef struct
{
   wchar_t letter;
   int     index;
} QUICK_INDEX_ITEM_T;
//}

class CListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    CListModel(AppMediaPlayer_Controller_Audio &aController, QMap<QString, QString> & imageMap, QObject *parent = 0); //changed by junam 2013.10.28 for 2nd depth coverart
    ~CListModel();

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount (const QModelIndex &parent = QModelIndex()) const;
    bool setData (const QModelIndex &index, const QVariant &value, int role);
    //void setCurrentCategoryId(QString tabId); //added by aettie.ji 2012.12.18 for new ux

    Q_INVOKABLE void clear();
	//modified by aettie 2013.04.05 for QA 41
    Q_INVOKABLE void requestListData(QString tabId, int itemIndex, int historyStack, bool isRunFromFileManager = false, bool fromCategoryOrBackkey = false);//Changed by Alexey Edelev. Fix bug 13058. 2012.09.13
    // { changed by junam 2012.11.29 for CR16170
    //Q_INVOKABLE void requestUpdateListData();
	//modified by aettie 2013.04.05 for QA 41
    Q_INVOKABLE int requestUpdateListData(bool fromCategoryOrBackkey = false);
    // } changed by junam
    Q_INVOKABLE void resetRequestData();

    Q_INVOKABLE bool showEditPopup();
    Q_INVOKABLE void tipPopup();
    Q_INVOKABLE void capacityView();
    Q_INVOKABLE void popupEventHandler(int popupType, int msg = -1);
    Q_INVOKABLE void ErrorMessage(int message, QVariant ext1 = 0, QVariant ext2 = 0);

    Q_INVOKABLE void toggleItemCheckbox(int itemIndex);
    Q_INVOKABLE void enableAllCheckBoxes(bool isEnabled);
    Q_INVOKABLE void FileInfoPopUp();// added by yongkyun.lee@lge.com   2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
    Q_INVOKABLE bool isAnyoneMarked();
    Q_INVOKABLE void resetFileOperationData();
    Q_INVOKABLE void startFileOperation(QString destPath = "", QString plPath = "");
    Q_INVOKABLE void handleSelectAllItems();
    Q_INVOKABLE void backupFileOperationData();
    Q_INVOKABLE void onItemLongPress(int itemIdx);
    /* TODO: Need to remove curPath flag */
    Q_INVOKABLE void handleTextEntered(QString enteredStr, QString curPath);
    Q_INVOKABLE void onQuickEdit(int index);
    Q_INVOKABLE void onNewFolderBtnClicked();

    Q_INVOKABLE bool isPlaylistsExist() const;
    Q_INVOKABLE void updatePopupList();
    Q_INVOKABLE void editPlayListsCatalog(int index, int operation, QString plName);

    Q_INVOKABLE QString getUrl(int index);
    Q_INVOKABLE bool isSelectable(int index); // added by ruindmby 2012.08.27 for CR 12133
    Q_INVOKABLE void copyPlayingSongToJukebox();
    Q_INVOKABLE void addPlayingTrackToPlaylist(QString playlistStr);
    Q_INVOKABLE void  isPopupID(int popupType);//added by yongkyun.lee 2012.12.07 for file info Popup update
    //Q_INVOKABLE bool IsAlbumTrackList(); //added by aettie.ji 2012.12.21 for new ux

    // { removed by junam 2012.07.26 for CR 11105
    //Q_INVOKABLE void requestOnDemandCoverArt(int top, int bottom);
    // } removed by junam

    // { add by wspark 2012.07.25 for CR12226
    // showing popup differently according to copyAll Status.
    Q_INVOKABLE bool isCopyAll() { return m_isCopyAll; }
    Q_INVOKABLE void setCopyAll(bool value) { m_isCopyAll = value; }
    // } add by wspark

    // { added by dongjin 2012.11.16 for CR14033
    Q_PROPERTY(bool isFocusChange READ getFocusChange WRITE setFocusChange);
    bool getFocusChange() { return m_isFocusChange; }
    void setFocusChange(bool value) { m_isFocusChange = value; }
    // } added by dongjin
    Q_PROPERTY(int operation READ operation WRITE setOperation);
    //Q_PROPERTY(const int FETCHED_BLOCK_ITEM_COUNT READ fetchedBlockItemCount); // removed by junam 2012.09.21 for CR13485
    Q_PROPERTY(const QString JUKEBOX_ROOT READ jukeboxRoot);
    Q_PROPERTY(int displayMode READ getDisplayMode WRITE setDisplayMode);
    Q_PROPERTY(int currentPlayingItem READ getCurrentPlayingItem WRITE setCurrentPlayingItem NOTIFY currentPlayingItemChanged);    
   Q_PROPERTY (int folderHistoryStack READ getFolderHistoryStack WRITE setFolderHistoryStack); 	//added by aettie 2013.04.10 for QA 41
    Q_PROPERTY (int currentFocuseIndex READ getCurrentFocuseIndex WRITE setCurrentFocuseIndex); //added by junam 2013.06.28
    Q_PROPERTY (bool moving READ moving WRITE movingSetter); //added by junam 2013.07.15 for ISV_NA_87444

    // { added by junam 2012.9.26 for disable file in copy mode
    Q_PROPERTY(bool isCopyMode READ getCopyMode WRITE setCopyMde);
    bool getCopyMode() { return m_bCopyMode; }
    void setCopyMde(bool bMode) { m_bCopyMode = bMode;}
    // } added by junam

    Q_INVOKABLE void clearHistoryStack();//added by junam 2013.09.23

    //int fetchedBlockItemCount() const { return FETCHED_BLOCK_ITEM_COUNT; } // removed by junam 2012.09.21 for CR13485
    QString jukeboxRoot() const { return JUKEBOX_ROOT; }

    int operation() const { return m_fileOperationData.operation; }
    void setOperation(int val) { m_fileOperationData.operation = (LVEnums::OPERATION_TYPE_T)val; }

    int getDisplayMode() { return m_displayMode; }
    void setDisplayMode(int mode) { m_displayMode = mode; }

    int getCurrentPlayingItem() { return m_currentPlayingItem; }
    void setCurrentPlayingItem( int idx );

//added by aettie 2013.04.10 for QA 41
    int getFolderHistoryStack() { return m_folderHistoryStack; }
    void setFolderHistoryStack( int cnt ){m_folderHistoryStack = cnt;}

    void setDisplayTip(bool displayTip){ m_isDisplayTip = displayTip; }
    bool isDisplayTip() const { return m_isDisplayTip; }

    //{removed by junam 2013.10.29 for playlist population
    //Q_INVOKABLE void scrollToSelectAlpha(QString letter);
    //Q_INVOKABLE void handleListScrollEvent(int currY, int topItemIndex, int bottomItemIndex); //removed by junam 2011.08.29 for CR12711
    //Q_INVOKABLE void handleListScrollEvent(int idx); //added by junam 2011.08.29 for CR12711
    //}removed by junam
    Q_INVOKABLE void resetPartialFetchData();
    Q_INVOKABLE int getTotalItemCount() const { return m_nTotalItemCount; }
    Q_INVOKABLE int GetCountryVariant() { return int(AppEngineBase::GetCountryVariant()); }
    Q_INVOKABLE int GetLanguage() { return int(AppEngineBase::GetLanguage()); }
    Q_INVOKABLE int getCurrentRequestCount() const { return m_currentRequestCount; } //added by yungi 2013.1.24 for ITS 153711
    Q_INVOKABLE int getFileURLCount() const { return CheckBoxSelectable_count; } //added by yungi 2013.03.06 for New UX FileCount
    Q_INVOKABLE void setFileURLCount(int count) { CheckBoxSelectable_count = count; }; //added by yungi 2013.03.06 for New UX FileCount
    Q_INVOKABLE void setIsCreatingUrlList(bool flag) { m_fileOperationData.bCreatingUrlList = flag; }; // added by eugene.seo 2013.04.05

    CTrackerAbstractor* getTrackerAbstactor() { return m_pListTrackerAbstractor; }

    void clearJukebox();

//{Commented by Alexey Edelev 2012.10.08
//    Q_INVOKABLE QString getFocusedItemName(int index); // added by junggil 2012.10.04 for CR13505 Media Apps - delta UIS 2.15 to 2.17
//}Commented by Alexey Edelev 2012.10.08
    Q_INVOKABLE QString getCountInfo(int currentIndex, bool isSongList);
//{added by HWS 2013.03.24 for New UX
    Q_INVOKABLE QString getCountInfoFirst(int currentIndex, bool isSongList); //HWS
    Q_INVOKABLE QString getListStatus(); //HWS
    Q_INVOKABLE QString getListFolders(); //HWS
    Q_INVOKABLE QString getCompareFolders(int currentIndex, bool isSongList); //HWS
//}added by HWS 2013.03.24 for New UX
   // Q_INVOKABLE QString getCountInfoFolderFiles(bool isSongList); //added by aettie.ji 2012.12.20 for new ux
    //Q_INVOKABLE QString getInitialAlpha(int currentIndex); // removed by junam 2013.11.18 for deprecated code

    // { added by sungkim for CR9616
    void clearPopup();
    void showPopupByExt(int type, QVariant extParam, QVariant extParam2);
    // } added by sungkim for CR9616
    // deleted by junggil 2012.07.18 for CR11957    Q_INVOKABLE int getCurrentScrollItemIndex() { return m_CurrentScrollItemIndex; }

    // { added by junam 2012.10.12 for CR14040
    Q_INVOKABLE void onComponentCompleted(int idx);
    Q_INVOKABLE void onComponentDestruction(int idx);    
    //{ modified by jaehwan 2013.12.14 for ISV 96345
    bool getOnDemandCoverArt( QStringList& albums, QStringList& artists, QStringList& files, int fetchCount = 1);
    //} modified by jaehwan

    void fillAlbumCover(const QStringList& albums, const QStringList& covers);
    //bool isOdcaMode(void){ return (m_currentCategoryState == ALBUM_DEPTH_0 );} //removed by junam 2012.10.25 for CR14451
    // } added by junam

    //Q_INVOKABLE void readyOnDemandCoverArt(int idx);//removed by junam 2012.10.12 for CR14040

    QString getExtractSongTitle(QString songTitle); //added by changjin 2012.09.14 for CR 13127

    Q_INVOKABLE int RemainedCapacity(); // added by ruindmby 2012.09.26 for CR#11543

    // { added by wspark 2012.11.06 for showing no files popup after deleting all jukebox audio.
    bool getOperationCompleteforTA() { return m_OperationCompleteforTA; };
    void setOperationCompleteforTA(bool value) { m_OperationCompleteforTA = value; };
    // } added by wspark
    //added by yongkyun.lee 2012.11.07 for Miner controll
    void cancelOperation();
	Q_INVOKABLE void cancelFileOperation(); // modified by eugene 2013.01.10 for ISV 69609
    void MinerControler(bool miner);    
    //added by yongkyun 
    // { added by lssanh 2013.02.07 2USB
    void cancelOperationChangeSource(bool isRemoved = false);	
    // } added by lssanh 2013.02.07 2USB
// private slots:
//    void onUpdateAlbumCover( QString album, QString coverURI);//removed by junam 2012.10.12 for CR14040

	// { added by eugene 2012.11.29 for CR 16076
    Q_INVOKABLE bool getCopyCompletedMode() { return isCopyCompletedMode; };
	Q_INVOKABLE void setCopyCompletedMode(bool value) { isCopyCompletedMode = value; };
	// } added by eugene 2012.11.29 for CR 16076
    int getPopType() { return m_popupType;};//added by yongkyun.lee 2012.12.07 for file info Popup update

    Q_INVOKABLE int getCategoryState() const { return m_currentCategoryState;} // added by junam 2012.12.17 for CR16785
    //Q_INVOKABLE bool getIsFolderFilesCount() const { return m_isFolderFilesCount;} // added by aettie.ji 2012.12.20 for new ux
   // Q_INVOKABLE bool getIsFoldersCount() const { return m_isFoldersCount;} // added by aettie.ji 2012.12.20 for new ux

    void cancelFileCopyOperation(bool isRemoved=false); // modified by ravikanth 16-04-13
    void fileOperationStateChanged(bool isCopyInProgress); // modified by ravikanth 16-04-13
    Q_INVOKABLE int getSkipFileCount() { return m_skipFileCount;} // modified by ravikanth 22-04-13
    Q_INVOKABLE int getCopyFileCount() { return m_copyFileCount;} // modified by ravikanth 22-04-13
    void onDatabaseUpdate(int count); //changed by junam 2013.05.14 for add count argument
    bool requestCoverTimerStart(int iTime = 1000); //added by junam 2013.06.12 for hold coverloading at indexing
    Q_INVOKABLE bool isCheckBoxMarked(int index); // modified by ravikanth 26-06-13 for ITS 0175972
    Q_INVOKABLE int currentCopyReplaceCount(){ return m_currentReplaceCopyCount; } // modified by ravikanth 07-07-13 for ITS 0178184
    Q_INVOKABLE bool isQuickViewVisible(void);// added by junam 2013.06.10 for ITS_179109
    bool isInCopyState() { return isCopyingMode; } // modified by ravikanth 12-07-13 for ITS 0178982	
    Q_INVOKABLE bool getCopyFromMainPlayer() { return copyFromMainPlayer;} // [KOR][ITS][179014][comment](aettie.ji)
    Q_INVOKABLE void setCopyFromMainPlayer(bool val) { copyFromMainPlayer = val;} // [KOR][ITS][179014][comment](aettie.ji)
    Q_INVOKABLE bool isCategoryTabAvailable();  // { added by sungha.choi 2013.08.05 for ITS 182674
    void setCategoryTabAvailable(int device, bool available);
    void onCategoryTabAvailable(bool bUSB1Mounted, bool bUSB2Mounted);   // } added by sungha.choi 2013.08.12 for ITS 182674_3rd
    Q_INVOKABLE QVariantList getAlphabeticList(int sortOrder); //added by junam 2013.08.20 for quick sort
    Q_INVOKABLE QVariantList getHiddenAlphabeticList(int sortOrder); //added by jaehwan 2013.10.26 ISV 90617
    Q_INVOKABLE bool IsFileSupported(int iIndex); //added by junam 2013.08.29 for ITS_KOR_185043
    Q_INVOKABLE QString getListIndexWithLetter(QString letter); //added by jaehwan.lee 2013.10.23

    void setCopySpeed(int nSpeed); // added by kihyung 2013.10.08 for ITS 0193747
    Q_INVOKABLE bool isFileOperationInProgress();
    Q_INVOKABLE void RequestListDataOnContentChanged(); // modified to avoid list update in edit state on copy complete 

    void SetFileOperationModeType(); // modified for ITS 0212431

    Q_INVOKABLE QString onItemClick( int index ); // moved by cychoi 2014.03.07 for ITS 228996 CDDA list flickering on item selection

 private:
    void createPlaylist(int Index, int contentPlayed); // modified by kihyung 2013.4.3
    void createPlaylistFromFolderList(QString folderPath, int index);
    void setAudioPlaylistsData(QString playlistPath = QString());

    CATEGORY_STATE_T calcCategoryState();
    void createUrlListFromMarkedItems();
    void invokePendingFileOperation();    
    bool isFileOperationValid();
    bool isItemUsed();
    void addNewFolderToModel(QString folderPath);
    void resetQuickEditData();
    void invokeRenameOperation(QString enteredStr);
    void invokeQuickDelete();

    void updateCountInfoString(QString category, int count = -1, int filesCount = -1);
	
    /* The proper way is to use only 1 function, but list model should fill all data fields */
    void addChunkToListModel(QVector<QStringList> &fileList);
    wchar_t getIndexChar(QString title); // added by jaehwan 2013.10.29 to improve qucikscroll performance
    bool isVisibleLetter(QString letter); //added by jaehwan 2013.11.12 for quick scroll spec change.
    void makeQuckIndexTable(); //added by jaehwan 2013.11.12 for quick scroll spec change.
    void updateListData(QVector<QStringList> &fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID);//changed by junam 2013.10.24
    void replaceListData(QVector<QStringList> &fileList, ETrackerAbstarctor_Query_Ids queryId); //added by junam 2013.06.25 for ITS175216

    bool playlistRead(QString fileName, QStringList &srcList, QStringList &trackList );
    bool playlistsWrite(QString fileName, QStringList &srcList, QStringList &trackList);
    bool playlistList( int device, QStringList &pathList, QStringList &nameList);
    bool playlistAdd(QString fileName, QStringList &srcAdd, QStringList &trackAdd );
    bool playlistRemove(QString fileName, QStringList &srcRem, QStringList &trackRem );
    void updateActivePlaylist();

    //Suryanto Tan: 2013.03.08.
    //Modified for ISV 73836 (list view error when rotating jog key)

    bool getEmptyBlock(int idx, int &offset, int &limit);//remove bCheckLimit by junam 2013.07.01 for ITS0175969

    //end of modified for ISV 73836

    bool isCoverFileSizeValid(QString); // added by wspark 2012.09.20 for CR13633
    USBController* GetUSBController(); // added by eugeny.novikov 2012.12.10 for CR 16564 fastboot
    DISPLAY_T GetOSDDisplay(); //added by yungi 2013.2.20 for Music Copy Completed not match OSD
    void setCopyInProgress(bool isCopyInProgress, int usbType); // modified by eugene.seo 2013.05.29

    //{added by junam 2013.06.28
    int getCurrentFocuseIndex() { return m_currentFocuseIndex; }
    void setCurrentFocuseIndex( int idx );
    //}added by junam

    //{added by junam 2013.07.19 for ISV_NA_87444
    void movingSetter(bool name) { m_moving = name; }
    bool moving() { return m_moving; }
    //}added by junam

    //{added by junam 2013.07.23 for ONDEMAND_PLAYLIST
    CIpodController* GetIPodController();
    //}added by junam
    void deleteSelectedModelItems();
    int indexOfFile(QString file, QStringList &list);//added by junam 2013.10.19

    //added by junam 2013.10.28 for 2nd depth coverart
    bool isCoverArtVisible(CATEGORY_STATE_T state){ return (state == ALBUM_DEPTH_0
                          || state == ARTIST_DEPTH_1 || state == COMPOSER_DEPTH_1 || state == GENRE_DEPTH_2);}

    bool checkUpdateResponse(ETrackerAbstarctor_Query_Ids queryId); //added by junam 2013.12.31 for ITS_CHN_216630

public slots:
    //{changed by junam 2014.03.09 for ISV74498
    //void ListRequestComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId);
    //void partialRequestComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, int offset);
    //void ListRequestCompleteWithVolumeType(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType, uint reqUID=0);
    void ListRequestComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID=0);
    void partialRequestComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, int offset=0, uint reqUID = 0);
    void ListRequestCompleteWithVolumeType(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType, uint reqUID=0);
    //}changed by junam
    void replaceFileRequest(QString file, int totalFilesForCopy); // modified by ravikanth 07-07-13 for ITS 0178184
    void onOperationComplete(); // modified by ravikanth 23-04-13
    void onOperationComplete(int skipCount, int copyFilesCount); // modified by ravikanth 22-04-13
    void capacityError(int fileCount, qint64 fileSize);
    void HandleError(int errorCode);
    void startJukeBoxFormat();
    void onProgress(int size, int totalFiles, int percent, QString fileDeleted);
    void onFileMoved(QString fileName);
    int setPlaybackIcon(); // modified by Dmitry 31.07.13
    void setCurrPlayingItemPos(QString album, QString artist, QString filename); // modified by kihyung 2013.09.13 for ISV 89552

    //void trimDownMetaInfo(); //added by junam 2012.09.21 for CR13485

    // { added by junam 2012.07.15 for CR 10719
    QString getMetaInfo( int itemIndex, const QStringList &itemList) const;
    bool getMetaInfo( int itemIndex, const QList<bool> &itemList) const;

    bool setMetaInfo( int itemIndex, QStringList &itemList, QString value);
    bool setMetaInfo( int itemIndex, QList<bool> &itemList, bool value);
    // } added by junam

    // { added by lssanh 2012.09.12 for CR13482
    void FormattingComleteHandler(int status);
    // } added by lssanh 2012.09.12 for CR13482

    void onFileSystemChanged();

    void onOperationDisplay(DISPLAY_T disp);//added by edo.lee 2013.03.22
    void onCopyOpertaionExit(); // modified by ravikanth 19-04-13

    //void onCurrentTrackCountChanged(int count); //removed by junam 2013.05.01 for database update

    void onOperationIncomplete(); // modified by ravikanth 21.06.13 for ITS 0174571

private slots:
    void onCategoryTabAvailableTimeOver(); //added by junam 2013.11.01 for category disabled state
    void showCopyCompletePopup(); // modified for ITS 0208724

Q_SIGNALS:
    void closeList();
    void disableListBtn();
    void enableListBtn();
    void finishEditMode();
    void cancelEditMode();
    void showJBoxEmptyStr(bool isVisible);
    void setCurrentPlayingItemPosition();
    void resetSelectAllItems(); // modified by ravikanth for ITS 0188110
    // deleted by junggil 2012.07.18 for CR11957    void setCurrentScrollingItemPosition();

    //PopUp
    void signalShowPopup(int type, QVariant extParam, QVariant extParam2);
    // added by yongkyun.lee@lge.com    2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
    void signalFileInfo( QVariant folderName, QVariant fileName , QVariant bitRate , QVariant fileFormat, QVariant createDate);
    void hidePopup(bool restore); // modified by eugene.seo 2013.04.10
    void signalUpdatePopupList(QStringList plNames, QStringList plSources);
    void progressToPopup(int index, int total, int percentage);
    void editPopupClosed();
    void copyingPopupClosed();
    void copyAllConfirmed();
    void copyCancelInfo(); //added by yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
    void contentRequestAllComplete(); // modified by ravikanth 28-04-13 to remove copy cancel confirm popup
    void deleteAllConfirmed(); // modified by ravikanth 25-08-13 for ITS 0184119 
    void restartTimerForListUpdate();


    //KeyPad
    void showKeyPad(QString title);
    void hideKeyPad();
    void updateTextItemView(QString title, int historyStack); //changed by junam 2013.09.06 for ITS_KOR_188332
    void signalUpdateCountInfo();

        //void updateClusterIndex(uint, uint); //removed by juanm 2013.11.20 deprecated code

    void manageJB();
    void setSelectedAlphaOnTop(int alphaIndex);
    void signalQuickScrollInfo(bool quickInfo);
	// void signalListLoadingIcon(bool visible); // added by lssanh 2013.02.08 for loading // removed by kihyung 2013.05.24

    //void signalIconSize(int size);//removed by junam 2012.10.12 for CR14040
    void activateTab(int tabId, bool isVisible, bool isSelected); // added by eugeny.novikov 2012.11.20 for CR 15408
    void currentPlayingItemChanged();    
    void contentSize(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, int count); //added by junam 2012.08.14 for 3PDMS 136415
    
    void resetEnteredStr(); // added by junggil 2012.10.05 for SANITY_CM_AJ196 ~ 201
    void clearSelection(); //added by junam 2013.03.14 for list item selection crash
    void moveTopHistoryStack(); //added by junam 2013.05.14 for iPod re-sync
    //added by aettie 20130808 for category dimming(ITS 182674)
    void categoryTabUpdated(bool updated);
    //void flickingEnabled(bool bEnabled); //removed by junam 2013.10.29 for playlist population
    void listItemsUpdated();
    void emptyItemViewList(int historyStack); //added by junam 2013.10.30

private:
    Q_DISABLE_COPY( CListModel );

    CTrackerAbstractor   *m_pListTrackerAbstractor;
    AppMediaPlayer_Controller_Audio &m_AudioController;
    FileOperationsHelper *m_pFileOperationsHelper;
    QTimer               *m_pFormatTimer;

    LIST_DATA_INFO        m_listMetaInfo;
    QList<QUICK_INDEX_ITEM_T> m_quickIndexTab; // added by jaehwan 2013.11.11 for quick index table
    QStringList           m_hiddenLetterList; // added by jaehwan 2013.11.11 for quick index table
    QStringList           m_letterList; // added by jaehwan 2013.11.11 for quick index table
    QList<QString>        m_lastRequestStack;
    QList<int>                  m_lastRequestIndexStack;  //added by junam 2013.05.20 for OnDemandPlayList
    REQUEST_DATA          m_currentRequest;
    int                   m_currentRequestCount;
    QString               m_playlistSource;
    ETrackerAbstarctor_VolumeType m_volumeType;
    int                   m_displayMode;
    bool                  m_isDisplayTip;
    int                   m_currentPlayingItem;
    int                  m_currentPlayingItemPreparation; //added by junam 2014.03.09 for ISV74498
    bool                 m_bCopyMode; // added by junam 2012.9.26 for disable file in copy mode
    int                   m_popupType; //added by yongkyun.lee 2012.12.07 for file info Popup update
    int                   m_folderHistoryStack; //added by aettie 2013.04.10 for QA 41
    QString             m_currentPlayingPath; //added by aettie 2013.04.10 for QA 41
    bool                  m_bCategoryTabAvailableJukeBox, m_bCategoryTabAvailableUSB1, m_bCategoryTabAvailableUSB2;      // { added by sungha.choi 2013.08.05 for ITS 182674


    //{added by junam 2013.06.28
    int m_currentFocuseIndex;
    QString m_currentFocuseItemName;
    QString m_currentFocuseItemTitle;
    //}added by junam

    enum CListModelRoles
    {
        imageRole = Qt::UserRole + 1,
        titleRole,
        urlRole,
        firstCharRole,
        checkBoxMarked,
        selectableRole,
        imageVisibleRole //added by junam 2012.10.12 for CR14040
		//imageVisibleRole, 
        //trackNumber	//added by aettie.ji 2012.12.21 for new ux
    };

    struct QuickEditData
    {
        int              index;
        QString          itemURL;
        QString          itemTitle;
        CATEGORY_STATE_T category;
        LVEnums::OPERATION_TYPE_T operation;
    } m_quickEdit;

    /* category - indicates current category and depth in it
     * itemNames - Stores the list of checked item names in Edit mode.
                   Used for building Url list.
     * fileURLs - Starts with "file://" and contains full list of files to operate with.
     * extraParams - stores current artist or/and genre for corresponding requests.
     * trackTitles - are used for Add To Playlist feature only.
     * bCreatingUrlList - indicates creation Url list is in progress.
     * destPath - destination folder path.
     * checkedIdxList - list stores absolute indexes for checked items. Used for restoring
                        checkboxes during scroll.
     * operation - current file operation to be invoked. */
    struct FileOperationData
    {
        CATEGORY_STATE_T category;
        QStringList      itemNames;
        QStringList      fileURLs;
        QStringList      extraParams;
        QStringList      trackTitles;
        bool             bCreatingUrlList;
        QString          destPath;
        QString          plPath;
        QList<int>       checkedIdxList;
        LVEnums::OPERATION_TYPE_T operation;

    } m_fileOperationData;

    // { modified by eugeny.novikov 2012.08.24 for CR 9731
    /* Partial fetching feature.
     * 1. MP requests FETCHED_BLOCK_ITEM_COUNT items from m_iBlockOffset index.
     * 2. m_iBlockOffset - absolute index of first item in a fetched block
     *                     from whole List view data [0, m_nTotalItemCount].
     * 3. m_nTotalItemCount - total item count in selected category.
     * 4. m_iTopItemIdx - relative top item index in current fetched block.
     * 5. m_iBottomItemIdx - relative bottom item index in current fetched block.
     * 6. m_quickScrollIdx - used when user clicks on alphabetic list at the right
     *                       for fast scroll to the clicked letter. If its equal to -1,
     *                       then corresponding responds from TA should be handled as usual.
     * 7. m_nCurrentY - absolute Y axis value for tracking scroll direction.
     * 8. m_bScrollRequestInProgress - used only for requests to TA for fetching
     *                                 next part of data during scrolling. */
    // } modified by eugeny.novikov
    int m_nCurrentY;
    int m_nTotalItemCount;
    int m_nTotalItemCountPreparation;//added by junam 2014.03.09 for ISV74498
    int m_iTopItemIdx;
    //int m_iBottomItemIdx;//removed by junam 2014.03.09 for ISV74498
    int m_quickScrollIdx; // added by eugeny.novikov 2012.08.24 for CR 9731
    int m_iScrollQueryOffset; //added by junam 2012.10.12 for CR14040
    int m_iScrollQueryLimit; //added by junam 2013.10.12 for scroll performance

    int m_countInfo;
    int m_filesCount;
    QString m_categoryInfo;
    bool m_isCopyAll; // add by wspark 2012.07.25 for CR12226.
    bool m_isFocusChange; // added by dongjin 2012.11.16 for CR14033
    int m_currentIndex; // added by eugene.seo 2013.01.07 for scroll issue


    QStringList actuallyDeletedFiles;

    //QTimer m_trimDownMetaInfoTimer;// added by junam 2012.09.21 for CR13485

    // { added by junam 2012.10.12 for CR14040
    CATEGORY_STATE_T    m_currentCategoryState;    
    QList<int>  m_itemIndexsOnScreen;
    QTimer m_requestCoverTimer;
    // } added by junam

    bool m_OperationCompleteforTA; // added by wspark 2012.11.06 for showing no files popup after deleting all jukebox audio.
    bool isCopyCompletedMode; // added by eugene 2012.11.29 for CR 16076
    //bool m_isFolderFilesCount;      //added by aettie.ji 2012.12.20 for new ux
    //bool m_isFoldersCount;  //added by aettie.ji 2012.12.20 for new ux
    bool isCopyingMode; // added by eugene 2013.01.10 for ISV 69609
    int CheckBoxSelectable_count; //added by yungi 2013.03.06 for New UX FileCount

    QString m_selectedFileFromList;

    int m_skipFileCount; // modified by ravikanth 22-04-13
    int m_copyFileCount; // modified by ravikanth 22-04-13

    QString m_currentReplaceFileName; // modified by ravikanth 07-07-13 for ITS 0178185
    int m_currentReplaceCopyCount; // modified by ravikanth 07-07-13 for ITS 0178184

    bool copyFromMainPlayer; //[KOR][ITS][179014][comment](aettie.ji)

    bool m_moving;//added by junam 2013.07.19 for ISV_NA_87444
    int m_currentDeletedFileCount;

    QMap<QString, QString> & m_imageMap; //added by junam 2013.10.28 for 2nd depth coverart]
    QTimer          m_categoryTabAvailableTimer; //added by junam 2013.11.01 for category disabled state
    bool m_bUpdateListOnEditExit; // modified to avoid list update in edit state on copy complete 
};

#endif // DHAVN_APPMEDIAPLAYER_LISTMODEL_H
