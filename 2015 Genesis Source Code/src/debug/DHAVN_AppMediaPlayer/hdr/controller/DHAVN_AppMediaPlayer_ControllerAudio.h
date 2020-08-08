#if !defined( APP_MEDIA_PLAYER_CONTROLLER_AUDIO_H )
#define APP_MEDIA_PLAYER_CONTROLLER_AUDIO_H

#include <QtDBus/QtDBus>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QDeclarativeView>

#include <DHAVN_IPodController_Def.h>
#include <DHAVN_TrackerAbstractor.h>
// #include <DHAVN_QRPMClient.h> // removed by kihyung 2012.11.03 for booting disable
#include <DHAVN_SharedDataProvider.h>
#include <DHAVN_AppFramework_Event_Def.h>
#include <imediainterface.h>
#include <iaudiointerface.h>
#include <QMutex>

#include "controller/DHAVN_AppMediaPlayer_ControllerBase.h"
#include "controller/DHAVN_AppMediaPlayer_Controllers.h"
#include "util/audio/DHAVN_AppMediaPlayer_CATreeModel.h"
#include "util/audio/DHAVN_AppMediaPlayer_PathViewModel.h"
#include "util/audio/DHAVN_AppMediaPlayer_PlaylistResults.h"
#include "util/audio/DHAVN_AppMediaPlayer_CTrackerExtractorClient.h"
//#include "util/audio/DHAVN_AppMediaPlayer_SearchEngine.h" //ys-20131010 delete seach engine
#include "util/audio/DHAVN_AppMediaPlayer_ListModel.h"
//#include "util/audio/DHAVN_AppMediaPlayer_GLUtils.h"

#define MLT_SERVICE         "org.lge.moreLikeThisWrapper"
#define MLT_OBJECT          "/"
#define MLT_INTERFACE       "org.lge.moreLikeThisWrapper"
#define DEVICE_HISTORY_MAX_COUNT 5 // added by eugeny.novikov 2012.08.15 for CR 12578
#define ID3_TAG_GENRE_MAX   126     // added by wspark 2012.09.17 for CR13605


// { modified by eugeny.novikov 2012.11.30 for CR 16133
// { added by  yongkyun.lee 2012.11.21 for  xSampa check.
 typedef enum
 {
    VR_BIT_TE,
    VR_BIT_XSMAPA,
    VR_BIT_DEVICE,   
    VR_BIT_ALL_MASK,
    VR_BIT_NO_MASK,    
    VR_BIT_DISCONNECT
 } vrBitShift;
// } added by  yongkyun.lee

//{ added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
typedef enum
{
   GRACENOTE_INDEXING_READY,
   GRACENOTE_INDEXING_DONE,
   GRACENOTE_INDEXING_NO_INFO,
}GNIndexing;
// added by yongkyun.lee 20130416
//remove by edo.lee 2013.05.14
/*typedef enum
{
	PREV_TRACK_INFO = -1,
	NEXT_TRACK_INFO = 1
	
}Trackinfo_Point;*/
//remove by edo.lee 2013.05.14

// { added by cychoi 2013.06.02 for sound noise when dragged progress bar
typedef enum
{
    DISC_MUTE,
    DISC_UNMUTE_READY,
    DISC_UNMUTE
}DiscMuteState;
// } added by cychoi 2013.06.02

// { added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
typedef enum
{
    ID3TAG_NONE,
    ID3TAG_REQUESTED,
    ID3TAG_RECEIVED
}ID3TagState;
// } added by cychoi 2013.09.22

class MP : public QObject
{
    Q_OBJECT
    Q_ENUMS(DEVICE_T);
    Q_ENUMS(MODE_REPEAT_RANDOM_T);
    Q_ENUMS(AUDIO_OPTION_MODE_T);
    Q_ENUMS(AUDIO_DISC_TYPE_T); // { added by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
    Q_ENUMS(PLAYBACK_CONTENT_T); // added by eugeny - 12-09-15
    Q_ENUMS(CONTROLLER_DISABLE_T); //added by junam 2013.07.12 for music app

public:

    typedef enum
    {
        JUKEBOX,
        USB1,
        USB2,
        DISC,
        IPOD1,
        IPOD2,
        BLUETOOTH,
        AUX,
        ALL_DEVICES,
        UNDEFINED
    } DEVICE_T;
// } modified by eugeny.novikov 2012.11.30

    typedef enum
    {
        SCANOFF,
        SCANFILE,
        SCANFOLDER,

        RANDOMOFF,
        RANDOMFILE,
        RANDOMFOLDER,

        REPEATOFF,
        REPEATFILE,
		// { added by eugeny - 12-09-15
        REPEATFOLDER, 
        REPEATALL,

        SCANALL
		// } added by eugeny - 12-09-15
    } MODE_REPEAT_RANDOM_T;

    typedef enum
    {
       OPTION_MENU_JUKEBOX = 0,
       OPTION_MENU_JUKEBOX_LIST,
       OPTION_MENU_USB,
       OPTION_MENU_USB_LIST,
       OPTION_MENU_DISC,
       OPTION_MENU_DISC_LIST,
       OPTION_MENU_IPOD,
       OPTION_MENU_IPOD_LIST,
       OPTION_MENU_SHUFFLE_MAIN,
       OPTION_MENU_REPEAT_MAIN,
       OPTION_MENU_SCAN,
	   // { added by eugeny - 12-09-15
       OPTION_MENU_SCAN_MAIN,
       OPTION_MENU_SCAN_ALL,
       OPTION_MENU_SCAN_FOLDER,
	   // } added by eugeny - 12-09-15
       OPTION_MENU_LIST,
       OPTION_MENU_MORE_LIKE_THIS,
       OPTION_MENU_SEARCH,
       OPTION_MENU_ADD_TO_PLAYLIST,
       OPTION_MENU_MORE_INFO,
       OPTION_MENU_SOUND_SETTING,
       OPTION_MENU_REPEAT_ALL,
       OPTION_MENU_REPEAT_FOLDER, // added by eugeny - 12-09-15
       OPTION_MENU_REPEAT_ONE,
       OPTION_MENU_REPEAT_OFF, // added by eugeny - 12-09-15
       OPTION_MENU_SHUFFLE_ON,
       OPTION_MENU_SHUFFLE_OFF,
       OPTION_MENU_CLEAR_JUKEBOX,
       OPTION_MENU_FILE_INFO,
       OPTION_MENU_COPY_TO_JUKEBOX,
       OPTION_MENU_NOW_PLAYING,
       OPTION_MENU_MOVE,
       OPTION_MENU_DELETE,
       OPTION_MENU_CANCEL_OPERATION,
       OPTION_MENU_CAPACITY,
       OPTION_MENU_VIEW,
       OPTION_MENU_SHUFFLE,
       OPTION_MENU_REPEAT,
       OPTION_MENU_EDIT_CATEGORY,
       OPTION_MENU_USB_BASIC,
       OPTION_MENU_JUKEBOX_BASIC,
       OPTION_MENU_BASIC_PLAYER,
       OPTION_MENU_TITLE_MENU,
       OPTION_MENU_DISK_MENU,
       OPTION_MENU_DVD_SETTING,
       OPTION_FULL_SCREEN,
       OPTION_MENU_DISPLAY_SETTING,
       OPTION_MENU_EXIT_MORE_LIKE_THIS,
       OPTION_MENU_MLT,
       OPTION_MENU_CONNECTION_SETTING,
       // modified by minho 20121221 for CR16795 Add to show up Chinese keypad popup in option menu
       // OPTION_MENU_AUX // added by wspark 2012.09.14 for New UX, AUX
       OPTION_MENU_AUX, // added by wspark 2012.09.14 for New UX, AUX
       OPTION_MENU_KEYPAD_SETTING,
       // modified by minho 20121221 
        OPTION_MENU_IPOD_MUISC_APP //added by junam 2013.07.23 for ITS_KOR_181304
    } AUDIO_OPTION_MODE_T;

    // { added by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
    typedef enum
    {
        DISC_NONE,
        AUDIO_CD,
        MP3_CD,
        DVD_AUDIO
    } AUDIO_DISC_TYPE_T;
    // } added by eugeny.novikov

// { added by eugeny - 12-09-15
    // { added by eugeny.novikov@teleca.com 2012.08.20 for CR 13012
    typedef enum
    {
        DEFAULT,
        ALBUM,
        FOLDER,
        PLAYLIST,
        SONG,
        GENRE,
        ARTIST,  //{ added by junam 2013.05.18 for ISV_KR82727
        PODCAST,
        ITUNES,
        AUDIOBOOK,
        COMPOSER, //}added by junam
        MORELIKETHIS, //added by junam 2013.06.11 for ITS173073
    } PLAYBACK_CONTENT_T;
    // } added by eugeny.novikov
// } added by eugeny - 12-09-15

    //{added by junam 2013.07.12 for music app
    typedef enum
    {
        CTRL_DISABLE_PRGRESSBAR,
        CTRL_DISABLE_PLAYQUE,
        CTRL_DISABLE_TUNE,
        CTRL_DISABLE_MENU,
        CTRL_DISABLE_FLOWVIEW,
        CTRL_DISABLE_LIST,
        CTRL_DISABLE_LIST_PLAYLIST,
        CTRL_DISABLE_LIST_ARTIST,
        CTRL_DISABLE_LIST_ALBUM,
        CTRL_DISABLE_LIST_SONG,
        CTRL_DISABLE_LIST_ITUNES,
        CTRL_DISABLE_LIST_PODCAST,
        CTRL_DISABLE_LIST_AUDIOBOOK,
        CTRL_DISABLE_LIST_COMPOSER,
        CTRL_DISABLE_LIST_GENRE,
    } CONTROLLER_DISABLE_T;
    //}added by junam
};
// { added by eugeny - 12-09-15

struct DeviceHistory
{
    // Device info:
    QString         DeviceID;   // unique device ID (usb, iPod, disc)
    QString         MountPath;

    // Playlist info:
    int                     Index; // current track index
    int                     Quantity; // total tracks in playlist
    PlayList                playList; // full playlist for the device
    MP::PLAYBACK_CONTENT_T  ContentPlayed;
    int                     UpdatedFSInfo;   // added by kihyung 2013.08.08 for ITS 0181967 

    // Track info:
    QString                 Path;
    PLAYER_STATE            State;
    QString                 Title;
    QString                 Artist;
    QString                 Album;
    QString                 Genre;
    QString                 Cover;
    QString                 Composer; // add by junggil 2012.06.29 for CR08958 : Composer info is not displayed on activating more info in Basic play back screen
    QString                 Folder; // added by junggil 2012.08.02 for CR12564
    int                     Position;       // seconds
    int                     Duration;       // seconds

    int                     repeatPlayMode; //added by yungi 2013.03.28 for ITS 160237
    int                     randomPlayMode; //added by yungi 2013.03.28 for ITS 160237

    bool                    bShowInfo;
	QDateTime               InsertTime; // added by eugene.seo 2013.03.11

    MEDIA_TYPE_T            mediaType; // true : audio, false 

    REQUEST_DATA            listReqData;

public:
    DeviceHistory()
    {
        playList.list = NULL;
        Index = 0;
        Quantity = 0;
        Position = 0;
        Duration = 0;
        UpdatedFSInfo = false;  // added by kihyung 2013.08.08 for ITS 0181967 
        State = PLAYER_STOP;
        bShowInfo = false;
        repeatPlayMode = REPEATALL;
        randomPlayMode = RANDOMOFF;
        ContentPlayed = MP::DEFAULT;    // added by sungha.choi 2013.07.15 for static test
        mediaType = MEDIA_TYPE_MAX; // added by sungha.choi 2013.07.15 for static test
    }
};

typedef struct
{
    bool m_nTuneFF;
    bool m_nTuneBack;
    int  idx_tune;
    bool m_nTuneRequest;
    bool m_nTuneStart;
    //{added by junam 2013.03.10 for ISV73458
    int     coverRequestIndex;
    QString coverRequestAlbum;
    QString coverRequestArtist;
    QString coverRequestURI;
    //}added by junam
} TUNE_IN_PROGRESS_STATE;

typedef struct
{
    int minerInProgress;
    int minerInProgressPercentageStep;
    //bool isMinerCleanRequired; //removed by junam 2013.11.06 for coverflow update
    bool isMinerFinished; // modified by eugene.seo 2013.04.15
    bool isUSB1MinerFinished; // added by eugene.seo 2013.04.15
    bool isUSB2MinerFinished; // added by eugene.seo 2013.04.15
} MINER_IN_PROGRESS_STATE;

typedef struct
{
    QString trackTitle;
    QString albumTitle;
    QString artist;
    QString genre;
    QString cover;
	QString composer; //added by lyg  2012.08.30  for 3DPMS_136434 Audio Cd File info
} CD_META_INFO;

//{ Deleted by yongkyun.lee@lge.com  2012.09.11 : source code cleanup for VR 
//typedef enum
//{ //0x01 : artist ,  0x02: album
//	VR_SEARCH_NONE,
//	VR_SEARCH_ARTIST,
//	VR_SEARCH_ALBUM
//} VR_SEARCH_TYPE_T;
//} Deleted by yongkyun.lee@lge.com  2012.09.11 : source code cleanup for VR 

class AppMediaPlayer_Controller_Audio : public AppMediaPlayer_Controller_Base
{
    Q_OBJECT
    Q_PROPERTY(bool isRunFromFileManager READ isRunFromFileManager WRITE setIsRunFromFileManager NOTIFY isRunFromFileManagerChanged)//Added by Alexey Edelev. Fix bug 13058. 2012.09.13
public:

    AppMediaPlayer_Controller_Audio( AppMediaPlayer_EngineMain &engineMain );

    ~AppMediaPlayer_Controller_Audio();

    IMediaInterface*    GetMediaInterface();
    MP::DEVICE_T        GetMediaMode();
    PLAYER_STATE        GetPlayerState();
    PathViewModel*      GetPathViewModel();
    CListModel*         GetListViewModel();
    CTrackerExtractorClient* GetTrackerExtractor();
    //SearchEngine*       GetSearchEngine(); //ys-201010 delete search engine

//removed by ruindmby 2012.08.06 for CR12391
    //void setIsMultipleUSBDevice( MP::DEVICE_T mode );
    //void setIsMultipleiPodDevice( MP::DEVICE_T mode );
//removed by ruindmby 2012.08.06 for CR12391
//{ added by yongkyun.lee 2012.12.19 for CR 14219 ,POPUP/list close
    void SetAccMode(bool accval)  ;
    void SetFrontLcdMode(bool flcdval);
    //void SetCamMode(bool camval); // removed by kihyung 2013.1.8
//} added by yongkyun.lee
    void DelayedUnmute4BT();		//added by hyochang.ryu 20130913 for ITS189483
    //deleted by yongkyun.lee 2013-11-25 for : ITS 210541 rollback
    void SetMediaMode(MP::DEVICE_T mode);
    int GetMediaNum();
    // { add by yongkyun.lee@lge.com    2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
    QString GetCurrentFolderName();
    QString GetCurrentBitRate();
    QString GetCurrentCreateDate();
    QString GetCurrentFormat();
    // } add by yongkyun.lee@lge.com 
// delete eugene.seo 2012.12.10 
    QString GetMarqueeString( const QString &fullString
                                                      /*, const QString &fontFamily,int pixelSize, int maxWidth*/ );   // added by hyochang.ryu 20130807 for ITS179637

    QString GetCurrentFilePath();
    QString GetCurrentArtist();
    QString GetCurrentGenre();
    QString GetCurrentCover(); //added by junam 2013.11.11 for ITS_NA_208040

    QString GetMountPathFromUSB(MP::DEVICE_T source );
    QString GetMountPathFromIpod(MP::DEVICE_T source );

    QString GetCurrentMP3FileName(); // added by cychoi 2014.02.05 for MP3 file name
    QString GetCurrentFileName();
    QString GetCurrentSongName();
    QString GetCurrentAlbumName();
    //QString GetCurrentCategoryId(); //added by aettie.ji 2012.12.18 for new ux
    //void setCurrentCategory( QString tabId ); //added by aettie.ji 2012.12.18 for new ux
    QString GetFilesCountInfo();
    QString GetFilesCountInfo(int device); //added by junam 2013.08.30 for ITS_KOR_186080
    QString GetFilesCountInfo(int iIndex, int nCount); //added by junam 2012.10.06 for folder tune count
    bool GetCurrentMediaInfo(MediaInfo *pInfo); //Added by junam for CR9528 
    // { removed by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
//    int GetFilesCount();    // Add by Naeo 2012.07.09 for CR 10970
    // } removed by eugeny.novikov
    // { added by wspark 2013.01.23
    void saveLastPlayedInfo();
    
    CTrackerAbstractor* GetTrackerAbstractor() { return m_pTrackerAbstractor; } 
    
    void setLastPlayedIndex( int value ) { m_lastPlayedIndex = value; };
    int lastPlayedIndex() { return m_lastPlayedIndex; }
    void playLastSavedFile(int index, int position);
    int lastPlayedPos() { return m_lastPlayedPos; }
    void setLastPlayedPos(int value) { m_lastPlayedPos = value; };
    // } added by wspark
    void showCurrentBTOSD();
    void showCurrentDiscOSD(); // added by cychoi 2014.03.26 for ITS 213525, 213526, 213527 CDDA OSD on language change
    void showCurrentIPOD3rdAppOSD(); //added by WJL 2014.04.15 for ITS 229118

    void Play();
    void Pause();
    void Stop();
    void functionStop(); // added by kihyung 2012.07.31 for C 11949
    void Toggle();		//added by hyochang.ryu 20130517 for BT Toggle
    void Continue();
    void NextTrack(int isUnsurpported = false); // modified by sangmin.seol 2013-12-13 for : ITS 0215489
    // { modified by lssanh 2012.09.18 for No35 car DQA#79	
    //void PrevTrack(bool isVRPrev = false);
    void PrevTrack(bool isDirectPrev = false);
    // } modified by lssanh 2012.09.18 for No35 car DQA#79
    void FastForward(MEDIA_FAST_SPEED speed);
    void Rewind(MEDIA_FAST_SPEED speed);

    void PlayiPodFiles();
    void NormalPlay();
	void RequestTrackInfo(bool bPrev, int trackIndex);//modified by edo.lee 2013.05.14

    void setPlayingPosition( int seconds );
    int  getPlayingPosition() const;
    int  getPlayingDuration() const;

    void GetDiscInfo();
    QStringList getDiscInfoList();    // Add by Naeo 2012.07.09 for CR 10970
    //modified by ruindmby 2012.08.19 for CR 13087
    Q_INVOKABLE void selectTitleChapter( int title, int chapter );
    //modified by ruindmby 2012.08.19 for CR 13087

	// { added by eugeny - 12-09-15
    Q_INVOKABLE void resetRepeatOne(); //20131016
	Q_INVOKABLE void updateRepeatRandomIcon(int nRepeatMode, int nRandomMode); // added by kihyung.lee 2013.08.31 for ITS 0187529 
    Q_INVOKABLE void setRepeatRandomMode(int nRepeatState, int nRandomState);
    Q_INVOKABLE void setScanMode(int scanMode);
	Q_INVOKABLE void setListMode(bool listMode); // modified by eugene.seo 2012.10.23 for Function_USB_0190
	Q_INVOKABLE int getCurrentCategory(int deviceID); // modified by eugene.seo 2013.02.07 for unsupported audio play in each repeat mode
    Q_INVOKABLE void toggleRepeatMode();
    Q_INVOKABLE void toggleRandomMode();
    Q_INVOKABLE int getRepeatMode() const { return m_repeatPlayMode; }
    Q_INVOKABLE int getRandomMode() const { return m_randomPlayMode; }
    Q_INVOKABLE int getScanMode() const { return m_scanMode; }
	// } added by eugeny - 12-09-15
    Q_INVOKABLE int GetMediaDuration(int device); // added by cychoi 2013.10.08 for ITS 190742 CDDA duration
	
    // {added by sam - 2013.10.01
    Q_INVOKABLE void saveCategoryTab(QString currentCategoryTab);
    Q_INVOKABLE QString  getCategoryTab(QString currentCategory);
    // }added by sam

    // { modified by wspark 2012.12.10 for sending bluetooth opstate
    //Q_INVOKABLE void auxTabActivated(); // added by wspark 2012.11.07 for sending aux opstate.
    Q_INVOKABLE void sendOpStateForNotifier(int tabId);
    // } modified by wspark
    Q_INVOKABLE bool IsNoSourceByDeviceid(int deviceid); //added by edo.lee 2012.11.28 for ISV 64487 
    Q_INVOKABLE bool IsNoSourceBySourceid(int sourceid); //added by edo.lee 2012.11.28 for ISV 64487 


    Q_INVOKABLE bool isSeekableMedia(); //added by changjin 2012.12.26 for ISV 61843
    Q_INVOKABLE void restoreLastPlay(); // added by wspark 2013.01.23

    Q_INVOKABLE void setDiscMuteState(bool bDiscMuteState); // added by cychoi 2013.06.02 for sound noise when dragged progress bar
    Q_INVOKABLE void setDragMuteState(bool isMute);//added by edo.lee 2013.06.13
    Q_INVOKABLE void SaveAudioData();//added by junam 2013.07.04 for ITS172937
    Q_INVOKABLE bool isControllerDisable(int ctrl, bool bSendOSD=true); //changed by junam 2013.07.12 for music app
    Q_INVOKABLE void updateCoverFlowMediaInfo(int device, bool bChangeUnknown, bool bEmpty); // added by kihyung 2013.08.21 for ITS 0185499 
    Q_INVOKABLE void setIsShowPopup(bool isShow){m_isShowPopup = isShow;}//added by edo.lee 2013.09.02 ITS 0184880
    bool IsShowPopup(){return m_isShowPopup;}//added by edo.lee 2013.09.02 ITS 0184880
	
    void clearPlayMode();
    void SetDefaultPlayMode(MP::PLAYBACK_CONTENT_T category, bool bSameFile = false); // modified by kihyung 2013.08.28 for ITS 0186131 
	
    Q_INVOKABLE int GetCurrentMediaPosition(int device){ return (device < MP::ALL_DEVICES)? m_history[device].Position : 0 ;}//added by junam 2013.10.25 for ITS_KOR_195270


    bool isPaused() const;
    bool isStopped() const;
    //{ modified by yongkyun.lee 2013-12-13 for : ITS 213657
    void iPodMountedTimer();
    //} modified by yongkyun.lee 2013-12-13 
    void requestMLTList();
    void playFromMLTList(int index);
    int getSongNameListTempCount();
    void clearTemps();
    bool isMinerHandleRequired ( int percentage);
    bool isMinerFinished();  // added by kihyung 2012.07.24 for CR 9673
    bool isUSB1MinerFinished();  // modified by eugene.seo 2013.04.15        
    bool isUSB2MinerFinished();  // added by eugene.seo 2013.04.15    
    void setDiscSource();    // modified by yongkyun.lee 2013-09-05 for : ITS 188359

    // { removed by kihyung 2012.11.03 for booting disable
    // void requestAudioModeChange(TAudioMode eAudioMode, TAudioSink eAudioSink); 
    // void resetAudioMode();
    // } removed by kihyung 2012.11.03 for booting disable

    // { modified by wspark 2013.03.23 for ISV 76947
    //Q_PROPERTY (int PlayerMode READ playerMode WRITE setPlayerMode);
    Q_PROPERTY (int PlayerMode READ playerMode WRITE setPlayerMode NOTIFY isPlayerModeChanged);
    // } modified by wspark
    Q_PROPERTY (bool isSourceChanged READ isSourceChanged WRITE SetIsSourceChanged); // added by kihyung 2013.2.12 for MEDIA HK Crash
    Q_PROPERTY (int m_NewState READ newState WRITE setNewState); // added by eugene 2012.11.26 for ISV #59906
    Q_PROPERTY (QUrl folder READ folder WRITE setFolder);

    Q_PROPERTY (QString album READ album WRITE setAlbum);
    Q_PROPERTY (QString song READ GetCurrentSongName); //added by junam 2013.03.25 for ISV77023
    Q_PROPERTY (QString artist READ GetCurrentArtist); //added by junam 2013.06.07 for mlt focus
    Q_PROPERTY (QString filePath READ GetCurrentFilePath); //added by junam 2013.07.01 for mlt play icon
    Q_PROPERTY (QString coverart READ GetCurrentCover); //added by junam 2013.11.11 for ITS_NA_208040

    // { removed by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
//    Q_PROPERTY (bool isAudioCD READ isAudioCD);
//    Q_PROPERTY (bool isAudioDVD READ isAudioDVD);
    // } removed by eugeny.novikov
    Q_PROPERTY (int DiscType READ getDiscType NOTIFY isDiscTypeChanged); // added by minho 20121221 for Removed error log when inserted DVD_AUDIO // added by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
    Q_PROPERTY (bool isSearch READ isSearch WRITE setIsSearch);
    Q_PROPERTY (bool isPlayFromMLT READ isPlayFromMLT WRITE setIsPlayFromMLT NOTIFY isPlayFromMLTChanged); // modified by Dmitry 27.04.13
    Q_PROPERTY (bool isBTStreaming READ isBTStreaming WRITE setIsBTStreaming);// modified by yungi 2014.02.25 for ITS 195248 // modified by yongkyun.lee 2013-10-14 for : ITS 195248
    Q_PROPERTY (bool isRestoring READ isRestoring);

    Q_PROPERTY ( QStringList pathsTemp READ getPathsTemp);

//{ added by yongkyun.lee 2012.12.19 for CR 14219 ,POPUP/list close
    Q_PROPERTY (int m_isAccOn    READ getAccMode);
    Q_PROPERTY (int m_isFrontLCD READ getFrontLcdMode);
    Q_PROPERTY (int m_bCamInFG   READ getCamMode);
//} added by yongkyun.lee

    Q_PROPERTY ( int currentIndex READ GetCurrentTrackIndex WRITE setCurrentIndex);

    Q_PROPERTY ( int isLongPress READ isLongPress WRITE setIsLongPress );
    Q_PROPERTY ( bool isEnableErrorPopup READ isEnableErrorPopup WRITE setIsEnableErrorPopup );
    Q_PROPERTY (bool isBasicView READ isBasicView WRITE setBasicView NOTIFY isBasicViewChanged); // added by eugeny.novikov 2012.10.25 for CR 14047
    //void setFirstEntry(MP::DEVICE_T source, bool bFlag){ m_bFirstEntry[source] = bFlag;}//removed by junam 2014.01.17 deprecated code
    
    // { modified by eugene.seo 2013.04.15
    Q_PROPERTY (bool isMinerFinished READ isMinerFinished WRITE setIsMinerFinished); 
    void setIsMinerFinished(bool isMinerFinished) { m_minerState.isMinerFinished = isMinerFinished; };
    Q_PROPERTY (bool isUSB1MinerFinished READ isUSB1MinerFinished WRITE setIsUSB1MinerFinished);
    void setIsUSB1MinerFinished(bool isUSB1MinerFinished) { m_minerState.isUSB1MinerFinished = isUSB1MinerFinished; };
	// } modified by eugene.seo 2013.04.15
	// { added by eugene.seo 2013.04.15
	Q_PROPERTY (bool isUSB2MinerFinished READ isUSB2MinerFinished WRITE setIsUSB2MinerFinished);
    void setIsUSB2MinerFinished(bool isUSB2MinerFinished) { m_minerState.isUSB2MinerFinished = isUSB2MinerFinished; };
    // } added by eugene.seo 2013.04.15

    // { added by kihyung 2012.08.30 for DVD-Audio
    Q_PROPERTY(QString trackNumber READ trackNumber NOTIFY trackNumberChanged);
    Q_PROPERTY(QString groupNumber READ groupNumber NOTIFY groupNumberChanged);
    QString trackNumber() { return m_strTrackNumber; }
    QString groupNumber() { return m_strGroupNumber; }
    // } added by kihyung
    // { added by wspark 2012.09.10 for CR12707
    Q_PROPERTY( bool isForwardDirection READ isForwardDirection);
    bool isForwardDirection() { return m_isForwardDirection;  };
    // } added by wspark

    Q_PROPERTY(int iPodSortingOrder READ getIPodSortingOrder NOTIFY iPodSortingOrderChanged);//added by junam 2013.06.07 iPod quick scroll    
    Q_PROPERTY (QVariantList categoryList READ categoryList WRITE categoryListSetter); //added by junam 2013.07.04 for ITS172937
    Q_PROPERTY (bool isBasicControlEnableStatus READ isBasicControlEnableStatus);//added by junam 2013.07.23 for ITS_KOR_181304

    int playerMode() const { return (int)getDevice(); };
    void setPlayerMode(const int &mode);
    Q_INVOKABLE bool GetFfRewState();    //{ modified by yongkyun.lee 2013-08-15 for :  ITS 183677 // DUAL_KEY Modified to Q_INVOKABLE

    bool isSourceChanged() const { return m_isSourceChanged; };  // added by kihyung 2013.2.12 for MEDIA HK Crash
    void SetIsSourceChanged(const bool isChanged);  // added by kihyung 2013.2.12 for MEDIA HK Crash

	// { added by eugene 2012.11.26 for ISV #59906
	int newState() const { return m_NewState; }; 
    void setNewState(const int state);
    void setEventId(const int eventID);
	// } added by eugene 2012.11.26 for ISV #59906

    QUrl folder() const;
    void setFolder(const QUrl &folder);

    QString album() const;
    void setAlbum(const QString &album);
//    void setAll(); // removed by eugeny.novikov 2012.08.15 for CR 12578

    // { removed by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
//    bool isAudioCD() { return m_discType == MEDIA_SOURCE_CDDA; }
//    bool isAudioDVD() { return m_discType == MEDIA_SOURCE_DVDAUDIO; }
    // } removed by eugeny.novikov
    int getDiscType() const; // added by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
//{ added by yongkyun.lee 2012.12.19 for CR 14219 ,POPUP/list close
    bool getAccMode() const;
    Q_INVOKABLE bool getFrontLcdMode() const;
    Q_INVOKABLE bool getCamMode();//modified by edo.lee 2013.08.28
//} added by yongkyun.lee 
    bool isPlayFromMLT() const { return m_isPlayFromMLT; }
    void setIsPlayFromMLT(bool isPlayFromMLT); // modified by Dmitry 27.04.13

    bool isBTStreaming() const; // modified by yungi 2014.02.25 for ITS 195248 // modified by yongkyun.lee 2013-10-14 for : ITS 195248
    void setIsBTStreaming(bool isBTStreaming);  // modified by yungi 2014.02.25 for ITS 195248  // modified by yongkyun.lee 2013-10-14 for : ITS 195248

    void showOSD() { m_bShowOSD = true; }
    bool isSearch() const { return m_searchData.bIsSearch; }
    void setIsSearch(bool f) { m_searchData.bIsSearch = f; }

    bool isRestoring() const { return m_bRestoring; }

    QStringList getPathsTemp() const;

    int isLongPress() { return m_isLongPressed; };
    void setIsLongPress(int isLongPress) { m_isLongPressed = isLongPress; };

    bool isEnableErrorPopup() const;
    void setIsEnableErrorPopup( bool bEnable );

    // removed by cychoi 2013.12.29 for ITS 217565
	
    bool IsRandom() { return (m_randomPlayMode != RANDOMOFF); }
    // { modified by yongkyun.lee@lge.com 2012.09.24:Bug 13915 - In shared memory "repeat" state mismatch after booting .
    //bool IsRepeat() { return (m_repeatPlayMode != REPEATFOLDER); }
    bool IsRepeat() { return (m_repeatPlayMode == REPEATFILE); }/*VR: file = on , etc =off */
    // } modified by  yongkyun.lee@lge.com  
    // { added by eugeny.novikov 2012.10.25 for CR 14047
    void setBasicView(bool bBasicView);
    bool isBasicView() const { return m_bIsBasicView; }
    // } added by eugeny.novikov

    bool IsDiscPlaying() const; // added by cychoi 2013.10.10 for ITS 194757 Pause on FF/REW
    Q_INVOKABLE bool IsDiscNormalPlaying() const; // added by wonseok.heo for ITS 220389 2014.01.16
    Q_INVOKABLE bool IsPlaying() const; // modified by ruindmby 2012.09.18 for CR 11848

    void setCurrentIndex( int index );
    int GetCurrentMltTrackIndex();//{ added by yongkyun.lee 20130219 for : ISV 73129
    int GetCurrentTrackIndex();
    int GetCurrentTrackNumber();

    void getMusicAlbum(QVector<QStringList> &fileList); //changed by junam 2013.11.06 for coverflow update
    void getMusicCoverArt(QVector<QStringList> fileList, uint reqUID); //add "reqUID" by junam 2012.10.25 for CR14451

    void getMusicAlbumCoverArtComplete(QVector<QStringList> fileList);

    void updateFSFolderList(MP::DEVICE_T device);  // added by kihyung 2013.08.08 for ITS 0181967 
    int  updateDevicePlayList( MP::DEVICE_T device, QStringList &filePaths, bool bJukeboxCopied = false);  // modified by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB
    bool createDevicePlayList( MP::DEVICE_T device, QStringList &filePaths, MP::PLAYBACK_CONTENT_T contentPlayed, bool bRescan, int nStartIdx = 0 );  // modified by kihyung 2013.08.02 for ITS 0180215
    void createPlayList( QStringList filePaths, MP::PLAYBACK_CONTENT_T contentPlayed ); // modified by kihyung 2013.4.3
    void createPlayListFromList (QStringList fileNameList, int index, MP::PLAYBACK_CONTENT_T contentPlayed ); // modified by kihyung 2013.4.3
    //{changed by junam 2013.07.22 for ONDEMAND_PLAYLIST
    //void createEmptyPlayList(int index );
    void createEmptyPlayList(MP::PLAYBACK_CONTENT_T contentPlayed, int index , int index0=-1); 
    //}changed by junam
    void setLastPlayList(); // added by eugene.seo 2013.04.10
    void ID3InfoFromURL( QString SearchURL ,int isAlbum); // add by yongkyun.lee@lge.com  2012.08.14  for : 11187
    void createPlayListFromVR(QString SearchWord,QString SearchURL, int isAlbum);
    void getDepthOrderList(QString rootDir, QStringList &playList); // added by junam for CR13012 2012.09.19
    void createPlaylistFromFolderTab(QStringList &filePaths, int nStartIdx = 0); // added by eugeny - 12-09-15
    //void updatePopupList(); //removed by junam 2012.11.02 for unusing code.

    void setAlbumThumbnail (QUrl thumbnail);

    /* Request all covers from mount point Jukebox or USB */
    void requestCovers ();//bool bCleanup = true); //changed by junam 2013.11.06 for coverflow update
    void setAudioPathRPM ( );
    void LoadGracenoteData();
    //{ added by yongkyun.lee 20130702 for : NO CR - MP3 CD song info update
    void DisplayCDInfo( int trackIndex , bool updateTitle );
    //void DisplayCDInfo( int trackIndex );
    //{ added by yongkyun.lee 20130702
    void setAppleDeviceStatus (int source, bool status); // modified by kihyung 2013.2.13
    bool getAppleDeviceStatus (int source);

    bool tuneWheel( bool bTuneForward );
    void switchToTuned();
    //void switchToTuned4Timer(int index); // removed by junam 2013.11.29 for tune 
    int    getPendingMedia(){return m_nPendingMedia;} // added by suilyou 20131030 ITS 0204905
    void setSavedMedia(int num){m_nSavedMedia = num;} // added by suilyou 20131030 ITS 0204905
    int    getSavedMedia(){return m_nSavedMedia;}// added by suilyou 20131030 ITS 0204905
    void clearMP3Info( );// modified by yongkyun.lee 2013-08-22 for :31.6 Ver. Smoke Test 
	
    void updateMP3Info(QString mp3Name, int index); // modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display //added by wonseok.heo 2013.05.17
    void updateCDTuneInfo();
    void tuneReset();
    bool getTuneTrackNamesFromCATree();

    static QString DeckStringToUnicode(int nType, char *pString); // modified by kihyung 2012.09.01 for Arabic string.    
    static QString encodeString (QString inputString); //added static by junam 08.22 for CR9771
    void ResetDeviceInfo(int deviceId);
    void ResetExtendDeviceInfo(); // added by eugeny.novikov 2012.08.15 for CR 12578

    void PauseBT();
    void ResumeBT();	//added by hyochang.ryu 20130705
    //{ added by yongkyun.lee 20130427 for : BT sync
    void SetBTAvrcpAbove13( bool val ){ m_bBTAvrcpAbove13 = val ;}
    bool GetBTAvrcpAbove13(  )        { return m_bBTAvrcpAbove13 ;}
    //} added by yongkyun.lee 20130427 

    QString               m_artistTune;
    QString               m_titleTune;
    // { added by junggil 2012.07.31 for CR12069
    QString               m_albumTune;
    QString               m_folderTune;
    QString               m_folderTuneTemp;    
    QString               m_genreTune;
    QString               m_composerTune;
    QString               m_coverartTune;
    // } added by junggil

    bool m_bSameDName; // added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT

    Q_PROPERTY(bool isForeground READ isForeground WRITE setForeground NOTIFY isForegroundChanged) // added by Dmitry 16.05.13
    bool isForeground ();
    void setForeground (bool m_Foreground);
    void commandFromBT( int );
    void BTPluggedOUT();
    void showBTCallPopup();	//added by hyochang.ryu 20130731 for ITS181088
    void closeBTCallPopup();	//{ added by hyochang.ryu 20130913 for ITS190345
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    void showDeckHighTempPopup();
    void closeDeckHighTempPopup();
    // } added by cychoi 2014.04.09

    void handleJukeboxDefault();
    void handleUSBPluggedIn(QString path, bool isAutoPlay, USB_PORT_T usbPort, QString UUID);
    void handleUSBPluggedOut(MP::DEVICE_T deviceToRemove);
    void handleIPODPluggedIn(QString path, bool isAutoPlay, USB_PORT_T usbPort, QString id);
    void handleIPODPluggedOut(MP::DEVICE_T deviceToRemove);
    void handleCDPluggedIn(MEDIA_SOURCE_T discType);
    void handleAUXPluggedIn();	//added by hyochang.ryu 20130802 for ISV88443
    void handleAUXPluggedOut(MP::DEVICE_T deviceToRemove);

    bool isPlayerActive();
    void setPlayerActive(bool m_bPlAct);
    void setDevice(MP::DEVICE_T device); // added by kihyung 2013.07.20 for ITS 0179388
    MP::DEVICE_T getDevice() const; // added by kihyung 2013.07.20 for ITS 0179388

    void sendFolderChunkRequestOnDec( int folder, int block );
    CATreeModel* GetTreeModel() { return m_pTreeModel; }
    CTrackerAbstractor* getTrackerAbstractor();

    void resumeMiner();
    void pauseMiner();
	bool isJukeboxMiningFinished(); // added by eugene 2012.11.14 for jukebox miner status
    void RequestTrackerJukeboxData();   // added by sangmin.seol 2013.11.14 for fix freeze in copy popup showing state when formated jukebox

    void InitiateJukeBoxFormat(void);
    void setDeviceId(MP::DEVICE_T source, QString id, QString mountPath);
    QString getMountPath(MP::DEVICE_T device) { return m_history[device].MountPath;}
    // { deleted by jungae 2012.12.18, changed specification so, do not need it anymore
    // QString getiPodDeviceName(void){ return m_iPodDeviceName; }  // added by jungae 2012.09.03 for CR 13162
    // } deleted by jungae 
    DeviceHistory* GetDeviceHistory(int device);
    QList<DeviceHistory*> GetExtendDeviceHistory(); // added by eugeny.novikov 2012.08.15 for CR 12578
    QString DeviceName(int type);
    void RestoreFromHistory(MP::DEVICE_T device); // modified by kihyung 2013.07.08 for ITS 0178696 
    void RestoreFromHistoryAfterTAScan(ETrackerAbstarctor_VolumeType volumeType, MP::DEVICE_T device, bool bRescan = false, bool bPlayAfterStop = false); // modified by kihyung 2013.08.02 for ITS 0180215
/*
    Q_INVOKABLE void startPlayFromSearch(int searchCategory, QString url, QString song,
                                         QString album, QString artist);
*/
    //Q_INVOKABLE void readyOnDemandCoverArt(int index); //removed by junam for CR12458
    //Q_INVOKABLE void requestOnDemandCoverArt(); // removed by junam 2012.10.25 for CR14451
    Q_INVOKABLE bool AutoplayAllowed();
    Q_INVOKABLE void ExitFromMLT();

    // modified by ruindmby 2012.08.06 for CR12391
    Q_INVOKABLE bool isMultipleiPodDevice();
    Q_INVOKABLE bool isMultipleUSBDevice();
    // modified by ruindmby 2012.08.06 for CR12391

    // { added by kihyung 2012.08.09 for CR 12741
    Q_INVOKABLE bool isMediaSyncfinished();
    // } added by kihyung
    bool isDatabaseReady();//added by junam 2013.05.02
    Q_INVOKABLE bool isFlowViewEnable();//added by junam 2013.05.03 for start mode selection

    Q_INVOKABLE bool isTAScanComplete(int nDevice); // added by kihyung 2013.3.11

    bool isApple1Ready() { return m_bIsApple1Ready; }
    bool isApple2Ready() { return m_bIsApple2Ready; }
    //Added by Alexey Edelev. 2012.09.08 CR13600, CR13610
    //{
    bool iPod1ContentSizeReady() { return m_iPod1ContentSizeReady; }
    bool iPod2ContentSizeReady() { return m_iPod2ContentSizeReady; }
    //}
    //Added by Alexey Edelev. 2012.09.08 CR13600, CR13610

    // void requestContentSize(MP::DEVICE_T source); // removed by kihyung 2013.2.17
    void sendUpdateOSD(unsigned int nEventType);

    bool IsAppleConnectError() { return m_bIsAppleConnectError; } // added by kihyung 2012.07.09 for CR 10343

	Q_INVOKABLE void BTMusicInfoChanged(QString title, QString album, QString artist ,QString count); //modified by hyochang.ryu 20130516 //added by eunkoo 2012.07.26 for CR11898
    Q_INVOKABLE void NotSupportBTOSD();		//added by hyochang.ryu 20130902 for ITS187310
    //void muteBeforePlay(); // commented by cychoi 2014.05.12 for deprecated code //added by hyochang.ryu 20130525

    void SetDiscPostponeRestore(bool value) { m_bDiscPostponeRestore = value; } // added by ruindmby 2012.08.09 for CR 12782
    QString charToUnicode( char *pIn, int bUnicode ); // added by jaden1.lee 2012.08.13 CR 11655

    // { added by kihyung 2012.08.30 for DVD-Audio
    Q_INVOKABLE void titleMenu();
    Q_INVOKABLE void topMenu();
    
    void showVideo(DISPLAY_T aDisplay);
    void hideVideo();
    void moveWidget(DISPLAY_T aDisplay); 
    void SetVideoWidgetSize(int ax, int ay, int aw, int ah); 
    void SetVideoSizeWrtDisplay(int ax, int ay, int aw, int ah);
    void clearDisplay(bool isBg);
    // } added by kihyung

    void StartiPodReset(); // added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT

    QString getFolderName( QString filePath); //added by junam 2012.8.29 for CR12745

    // { modified by kihyung 2013.12.17 for ITS 0214009 
    // Q_INVOKABLE void setSkipTrack(bool isSkip){m_skipTrack = isSkip;}//added by edo.lee 2013.08.30 for Mute after unsupported file 
    Q_INVOKABLE void setSkipTrack();
    // } modified by kihyung 2013.12.17 for ITS 0214009 

    Q_INVOKABLE void invokeMethod(QObject *pObj,QString member)
                            { QMetaObject::invokeMethod(pObj, member.toLocal8Bit(), Qt::QueuedConnection); }// added by junam 20120904 for CR13381
    Q_INVOKABLE void invokeMethod(QObject *pObj,QString member, QString arg1)
                            { QMetaObject::invokeMethod(pObj, member.toLocal8Bit(), Qt::QueuedConnection, Q_ARG(QString, arg1)); } //added by junam 2013.11.16
    Q_INVOKABLE void invokeMethod(QObject *pObj,QString member, QVariant arg1)
                            { QMetaObject::invokeMethod(pObj, member.toLocal8Bit(), Qt::QueuedConnection, Q_ARG(QVariant, arg1)); }
    Q_INVOKABLE void invokeMethod(QObject *pObj,QString member, QVariant arg1, QVariant arg2)
                            { QMetaObject::invokeMethod(pObj, member.toLocal8Bit(), Qt::QueuedConnection, Q_ARG(QVariant, arg1), Q_ARG(QVariant, arg2)); }

    //{Added by Alexey Edelev. Fix bug 13058. 2012.09.13
    bool isRunFromFileManager() { return m_isRunFromFileManager; }
    void setIsRunFromFileManager(bool isRunFromFileManager) { if(isRunFromFileManager != m_isRunFromFileManager ) { m_isRunFromFileManager = isRunFromFileManager; emit isRunFromFileManagerChanged();}}
    //}Added by Alexey Edelev. Fix bug 13058. 2012.09.13
    // { added by junam 2012.9.10 for CR13244
    Q_PROPERTY(bool bShowInfo READ getShowInfo WRITE setShowInfo);
    bool getShowInfo(void) const { if(getDevice() >= MP::ALL_DEVICES) return false; return m_history[getDevice()].bShowInfo; }
    void setShowInfo(bool bShow) { if(getDevice() >= MP::ALL_DEVICES) return; m_history[getDevice()].bShowInfo = bShow;}
	// } added by junam
	
	// { added by ravikanth - 12-09-26
    void storeDVDdata();
    void restoreDVDdata();
	// } added by ravikanth - 12-09-26
    bool isListView() { return m_isListView; }; // added by wspark 2012.11.06 for showing no files popup after deleting all jukebox audio.
	
	// { added by junam 2012.12.04 for CR15344	
    void iPodDeviceOff(void){ if(m_pIMedia && (getDevice() == MP::IPOD1 || getDevice() == MP::IPOD2))\
        {m_pIMedia->SetSystemState(SYSTEM_IPODDEVICES_TABOFF);}}
	// } added by junam
    void requestAllPlaylist(int nDevice, bool bRescan = false, bool bJukeboxCopied = false); // modified by sangmin.seol 2013.10.22 JUKEBOX PlayerView Counter Update after copy from USB
    PLAYER_STATE getPlayState(){ return m_PlayerState;}//added by yongkyun.lee 20130319 for : ISV 76019
    void setOperationDisplay(DISPLAY_T disp){emit operationDisplay(disp);} //added by edo.lee 2013.03.22

    // { modified by wspark 2013.04.30 for cluster IOT
    //Q_INVOKABLE void resumeTpMessage(); // added by wspark 2013.04.09 for cluster IOT #34
    Q_INVOKABLE void resumeTpMessage(QString tp, int deviceId);
    // } modified by wspark
    void  setMP3ListReadState(bool listRead);// added by yongkyun.lee 20130413 for :NO CR MP3 List 
    void sendClearTpMessage() { emit clearTpMessage(); } // added by wspark 2013.04.17
    //{ added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
    GNIndexing GetCDDAGraceNote( ){return m_CDDAGraceNote; }
    Q_INVOKABLE int GetCDDAGraceNoteStatus(); // added by wonseok.heo for ITS 197433 2013.10.28
    void SetCDDAGraceNote(GNIndexing val );
    void updateMediaInfo();
    //} added by yongkyun.lee 20130416 
// added by Dmitry 23.04.13
    Q_INVOKABLE bool cancelFFRW(bool resumeNormalPlay=true);
    void setFFRW(bool isFFRW, int status, int arrow);
// added by Dmitry 23.04.13
    void onBtRequestDecCount();// modified by yongkyun.lee 2013-08-28 for : BT OSD sync 
    void onBtRequestIncCount();// modified by yongkyun.lee 2013-08-28 for : BT OSD sync 
    //{ added by yongkyun.lee 20130426 for : BT sync
    void onBtRequestTimerStart();
    void onBtRequestTimerStop();
    //} added by yongkyun.lee 20130426 
    /*//{ added by cychoi 2013.08.25 for BT Music OSD display. Need more investigation later.
    void BTOSDDisplayTimerStart();
    void BTOSDDisplayTimerStop();
    //} added by cychoi 2013.08.25*/
	void setMediaInfoToHistory(MEDIA_SOURCE_T device, QString filename, QString foldername, int position, int random=-1, int repeat=-1);//modified by edo.lee 2013.07.05
    bool StartProcessWithQProcess( QString &pPath, QStringList &pFirstArg ); //added by aettie 20130605 for Gracenote restart

    void setTrackChange(int trackNum); // added by wonseok.heo 2013.06.05 for factory test
    void SetSystemMute();//added by edo.lee 2013.06.22

    void setInOuteStepCnt(int cnt);//added by wonseok.heo 2013.07.04 disc in out test
    void setInOutTestCnt(int cnt);//added by wonseok.heo 2013.07.04 disc in out test

    int getOSDevice(MP::DEVICE_T device);
    //{added by junam 2013.07.19 for ITS172937
    void categoryListSetter(QVariantList name) { m_categoryList = name; }
    QVariantList categoryList() { return m_categoryList; }
    //}added by junam
    bool isBasicControlEnableStatus(){return m_bBasicControlEnableStatus;}//added by junam 2013.07.23 for ITS_KOR_181304

    // { modified by kihyung 2013.08.27 for ITS 0186708
    bool EmitOnMediaInfoChanged(MP::DEVICE_T device, bool bChangeUnknown = true, bool bUpdateCoverFlow = true);  // modified by kihyung 2013.08.20 for ITS 0185499 
    bool EmitOnMediaInfoTempChanged(MP::DEVICE_T device, QString title, QString album, QString artist, QString genre, QString cover, QString composer, QString folder, QString file);
    bool EmitOnTuneSearch(QString m_coverart, QString m_artist, QString m_song, QString m_album, QString m_folder, QString m_countInfo , bool m_samefile);    
    bool EmitOnTuneMetaData(QString artist, QString song, QString album, QString folder, bool isColor);
    // } modified by kihyung 2013.08.27 for ITS 0186708
    
    bool EmitOnCoverFlowMediaInfoChanged(MP::DEVICE_T device, bool bChangeUnknown = true, bool bEmpty = true);  // added by kihyung 2013.08.21 for ITS 0185499  
    void setJukeBoxCategoryTabAvailable();   // added by sungha.choi 2013.08.12 for ITS 182674_2nd
    bool isUSBMounted(MP::DEVICE_T device);   // added by sungha.choi 2013.08.12 for ITS 182674_3rd
    void emitShowPlayerView(){ emit showPlayerView();} //added by junam 2013.08.26
    void setstrUpdatedMediaInfo(QString value);    // added by puneet 2013.08.29
    void sendToBTCommand(int btcmd );//modified by yongkyun.lee 2013-09-09 for : BT CMD LOG
    Q_INVOKABLE bool IsFileSupported(QString filePath); //added by junam 2013.09.13 for ITS_KOR_189335
    bool IsFileSupported(int index); //added by junam 2013.11.28 for ITS_CHN_211039
    void setJukeboxUpdate(bool isUpdate){ m_jukeboxUpdate = isUpdate;} //added by junam 2013.10.03 for ITS_KOR_192927
    int getCurrentAutoPlayItem() { return m_autoTrackChangeIndex;}
    void startLongBtnStart(){m_pLongBtnTimer->start();}// added by edo.lee 2013.10.12 pop noise
    Q_INVOKABLE bool getCoverFlowEnable(){return  m_bCoverFlowEnabled;} // added by suilyou ITS 0205778
    Q_INVOKABLE void setCoverFlowEnable(bool set){m_bCoverFlowEnabled = set;}// added by suilyou ITS 0205778
    void catreeRelease(); //added by wonseok.heo for ITS 205774 2013.11.01

    Q_INVOKABLE void coverflowItemClick(QString filePath); //added by junam 2013.11.16 for coverflow click

public slots:
    void HandleMediaStatusChanged (int mediaStatus);
    void onMediaStatusChanged(int mediaStatus); //added by junam 2012.10.08 for CR14264
    void HandleError(int errorCode, QString filename); // modified by kihyung 2013.4.7 for IPOD
    void ipod1Authorised(bool status);
    void ipod2Authorised(bool status);
    //{added by junam 2013.05.02
    void onIPodReady(bool status);
    void onAutoPlayReady(bool status);
    //}added by junam
    void ipodDisconnectedSlot(QString deviceId, int volume);
    //void ipodConnectedSlot(QString deviceId, int startIndex, int endIndex, int volume, int syncType); //removed by junam 2013.07.10 for calcel ff
    void onGetDeviceName1(QString name); 
    void onGetDeviceName2(QString name); 
    void onSystemStateStatus(int status); // added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT
    void displayiPodTaggingOSD(EIPodPopUp nMessageId);

    void HandleRequestComplete (QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID); //add reqUID by junam 2012.10.25 for CR14451
    void HandleRequestComplete( QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, int offset, uint reqUID ); //added by junam 2013.04.13 for AutoStart
    void HandleRequestCompleteWithVolumeType( QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint );

    void ScanComplete();
    void OnSearchCompleted(int nTitleNo, int nChapterNo, int nChapterSize);
    void PlaylistDataReceived (PlaylistResults results);

    void onCDEjected();
    void onGraceNoteTOCReceived(int nSize, void *pData);
    void parseReceivedDirTree( DvdcDirTree *dirTree );
    // { added by jaden1.lee 2012.08.13 CR 11655
    void requestCurrentFolderName( void );
    void receivedCurrentFolderName(DvdcFolderName *pFolderName );
    // } added by jaden1.lee
    void updateCurrentMP3FolderName(); // added by cychoi 2013.10.07 for ISV 92215 Folder Display on Track Change

    void onPositionChanged(int millisec);
    void onDurationChanged(int millisec);
    void onTrackChanged(int index);
    void onDeckTrackChanged(int index);
    void onDeckTrackUpdated(int index); // added by cychoi 2013.11.17 for ITS 208698 MP3 Play Info on Cluster 
    void onMediaInfoChanged(MediaInfo* track);
    void onMediaInfoChangedEx(MediaInfo* track, bool bEmitEnabled = true); // added by oseong.kwon 2014.03.03 for ITS 227602
    void onStateChanged(int state);
    void onNormalPlay(); // added by Sergey 28.05.2013
    void onTagReceived(DvdcTagInfo *pTag);
    void onAllTagReceived(DvdcAllTagInfo *pTag); // added by kihyung 2012.11.22
    void onModeChanged(int); // added by kihyung 2012.07.06 for HMC_DH__SANITY_DISC282, HMC_DH__SANITY_DISC283
    void onRealSoundChanged();//added by edo.lee 2013.04.21 for Mute
    void OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize);//(int bPrev, int index);//added by edo.lee 2013.05.14
    // modified by  yongkyun.lee 2012.11.21 for  xSampa check.
    //void onMinerFinished(bool isFinished);
    void onMinerFinished(bool isFinished, QString onMinerFinished);
    // modified by  yongkyun.lee 
    bool onMinerInProgress( int percentage );   // modified by sungha.choi 2013.08.12 for ITS 182674_3rd
    void onAutoTrackChanged(int index);

    Q_INVOKABLE void onReleaseResources( MEDIAPLAYER_RESOURCES_T resources );
    Q_INVOKABLE void onResourceGranted();
    // { removed by eugeny.novikov@teleca.com 2012.08.01 for CR 11079
//    void onNeedToUseQDir(QString mountPath);
    // } removed by eugeny.novikov
    void onIPodTotalNumTracks(int total);
    //{changed by junam 2013.03.10 for ISV73458
    //void onTuneRequestTimer(void); //added by junam 2012.8.29 for CR12745
    void onTuneMetaInfoTimeOver(void);
    //void onTuneCoverArtTimeOver(void); //removed by junam 2013.10.17 for deprecated code
    //}changed by junam
    // void onScreenStateChanged( int nScreenState ); // modified by ravikanth - 12-09-25 - cr 13789 // remvoed by kihyung 2012.11.26. removed spec.

    void onRequestCoverArtForListView(); //added by junam 2012.10.12 for CR14040 
    void onRequestCoverArtForFlowView(); //added by junam 2012.10.25 for CR14451
    void onRequestFirstSong(); // added by eugene.seo 2013.01.18 for 25000 over USB
    //{removed by junam 2013.03.10 for ISV73458
    //void onTuneDelayCoverart(); // added by lssanh 2013.01.24 ISV70714
    //}removed by junam
    void onBtResponseTimerExpired(); // added by wspark 2013.02.07 for ISV 72168
    void onBtResponseTimerStop(); //modified by yongkyun.lee 2013-10-10 for :  ITS 191703 BT getinfo 0.5sec
    // { added by cychoi 2014.06.03 for Get Info - 1. on TrackChanged 2. delayed 0.5sec from PlayStarted
    void onBtDelayedGetInfoTimerExpired();
    void onBtDelayedGetInfoTimerStop();
    void onBtDelayedGetInfoTimerStart();
    // } added by cychoi 2014.06.03
    // { added by cychoi 2014.06.05 for Get Info - every 10sec after PlayStarted (some phones don't send TrackChanged)
    void onBtPeriodicGetInfoTimerExpired();
    void onBtPeriodicGetInfoTimerStop();
    void onBtPeriodicGetInfoTimerStart();
    // } added by cychoi 2014.06.03
    void onBtRequestTimerExpired();//added by yongkyun.lee 20130426 for : BT sync  
    // { modified by cychoi 2014.05.12 for duplicated code (added by hyochang.ryu 20130627 for BT delayed play, 20131031 for ITS 205112)
    void onBtDelayedPlayTimerExpired();
    //void onBtDelayedPlayTimer1Expired();
    // } modified by cychoi 2014.05.12
    void onBtDelayedMuteTimerExpired();	//added by hyochang.ryu 20130913 for ITS189483
    void oniPodMountTimerExpired(); // modified by yongkyun.lee 2013-12-13 for : ITS 213657
     //void onBTOSDDisplayTimerExpired(); // added by cychoi 2013.08.25 for BT Music OSD display. Need more investigation later.
    //void onMuteTimerExpired(); // commented by cychoi 2014.05.12 for deprecated code //added by edo.lee 2013.04.21 for Mute
    void onIPodDeviceConnectedSlot(QString strNodePath, QString strSerial, EIPodPorts ePortNum, bool isFirst);
    QString getIPODDeviceName(MEDIA_SOURCE_T source);
    void onBTFunc(bool _pauseplay);		//added by hyochang.ryu 20130601

    void onDiscloadRequest();//added by wonseok.heo 2013.07.04 disc in out test
    void onAudioSinkError(); // added by kihyung 2013.07.04 for ISV 85455
    void onUserTrackChanged(int trackIndex);
	void onLongBtnTimerExpired();// added by edo.lee 2013.10.12 pop noise
    void onScanTimerExpired(); // added by jaehwan.lee 2014.02.27 to fix ITS 227260
#ifdef SIRI_TIMEOUT_TEST
    void onNotifyPlayStateForSiri();
    void onLaunchedDefaultMusicApp(bool enable, QString AppName);
#endif
    bool getIsForeground(){return m_isForeground;} //added by suilyou ITS 0205229

private slots:
    void onCurrentTrackCountChanged(int count); //added by junam 2013.04.23 for index DB
    void onDatabaseUpdate(int count);  //added by junam 2013.05.01 for database update
    void onBasicControlEnableStatusChanged(bool enable, QString AppName);//added by junam 2013.07.12 for music app
	void UpdateClusterTrackIndex(uint current, uint total);

signals:
Q_SIGNALS:
    void disableHardKey(bool isDisable); //added by jeeeun for MLT play in FF/REW 2012.12.05
    void tuneSameFileNoti( bool );   // added by yongkyun.lee  2012.10.17  for Current file is white color
    //void updatePathView( QVariant isVisible ); //removed by junam for 2012.12.06 flow view focus
    void showPathView( QVariant isVisible );  //added by yongkyun.lee 2012.12.11 for CR 16366 No display cover flow on MLT list.
    // modified by minho 20120821
    // { for NEW UX: Added active tab on media
    //void activateTab(int tabId);
    void activateTab( int tabId, bool isVisible, bool isSelected );
    void serch_TabClicked(); //  add by yongkyun.lee@lge.com  2012.10.30 : : CR 13733  : Sound Path
    // } modified by minho
    // { modified by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
//    void setDiscTabName();
    void updateUIForDisc();
    // } modified by eugeny.novikov
    //void closePopUp(); //removed by junam 2013.09.23 for not using code
    void closePopUpLoader();// { added by  yongkyun.lee 2012.11.13 for ISV  62700
    void dvdInfochanged(int TitleNum, int ChapterNum,int totalChapter, int totalTitle );
    void getdvdInfo(int TitleNum, int ChapterNum,int totalChapter, int totalTitle ); // added by ravikanth 12-09-24
    void playbackStopped();
    void resetCurrentIndex(); // added by eugene 2012.11.26 for ISV #59906
    void playbackStarted();
    void playbackForcedStart();//added by junam 2013.07.24 for iPOD FF/RW icon
    void playbackPaused();
    void playbackTuned(); // added by wspark 2012.08.14 for CR11620
    void bluetoothPlaybackStarted();
    void bluetoothPlaybackPaused();
    void playListReadyAutoStart();
	void playListStartSameSong(); //added by edo.lee 2012.12.14 ISV 64893 
    void CommandOnVideoDec (int);
    void CommandOnBT (int);
    void FastForwardComanndOnVideoDec(bool, int);
    void Error();
    void VolumeChanged(int volume);
	// { modified by eugeny - 12-09-15
//    void setRandom (int randomMode);
//    void setRepeat (int repeatMode);
    void setRandomRepeatIcons(int repeatIcon, int randomIcon);
	// } modified by eugeny - 12-09-15
    void resetSpeedIcon();
    void resetPlayIconBT(); // added by kihyung 2012.08.04 for CR 12585 - [3PDMS 132371] [Domestic] Toggle error with BT music Play /Pause
    void setSpeedIcon(int rate);
    void startScan();
    void stopScan();
    //{ removed by junam 2012.11.02 for unusing code.
    //void clearThumbnailList();
    //void addThumbnailListItems( QStringList names, QStringList icons );
    //} removed by junam
    void setStatusDataByIndex( MP::DEVICE_T state, DATA_TYPE_T index, int aValue );
    void setAppState(MP::DEVICE_T state);

    //{changed by junam 2013.07.01 for mlt play icon
    //void sendMoreLikeThisList ( QStringList names, QStringList artists  ); //modified by aettie.ji 2013.02.18 for ux optimization
    void sendMoreLikeThisList ( QStringList names, QStringList artists, QStringList files ); 
    //}changed by junam

    void showNoFoundPopup();
    void showPopupError(bool bUnsupportedAll); // modified by kihyung 2013.09.12 for ISV 90605
    void sigShowBTCallPopup();	//added by hyochang.ryu 20130731 for ITS181088
    void sigCloseBTCallPopup();	//{ added by hyochang.ryu 20130913 for ITS190345
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    void sigShowDeckHighTempPopup();
    void sigCloseDeckHighTempPopup();
    // } added by cychoi 2014.04.09
    void changeIPodCategory(int categoryIndex); // added by kihyung 2013.4.7 for IPOD
    void showPopupLoadingData(); // add by lssanh 2012.09.10 for CR9362
    void allTagReceived(); //added by yungi 2012.11.30 for CR16108
    void showIPodRemovedPopup();
    void updateCDTrackMetadata ( QString trackName, QString trackAlbum, QString trackArtist );
    void setHighlightCover (int coverIndex, bool isScroll);
    //{ added by junam 2013.11.06 for coverflow update
    void clearCoverflowIndex(int count);
    void setCoverflowIndex(int index); 
    //}added by junam
    void tuneTextColorChanged (bool isTuneColor);
	//deleted by aettie 20131010 unused code
   // void signalSetGracenoteVolumeType(int type, QString id, QString path);
    //void signalGracenoteDeviceStateChanged(int volume, QString id, QString path, int startIndex,
    //                                       int endIndex, int syncType, bool isMounted);
    void signalGracenoteHighlightedVolume(int volume,  QString id, QString path);
    void tocReceived(QString toc);
    void signalCloseSearch();
    //void showDiscEjectedPopup(); //removed by junam 2012.10.16 for CR14562
    // void iPodSyncStep(int syncType); // commented by kihyung 2012.08.04 for CR 11725
    void mediaSyncfinished(bool bFinished); // added by kihyung 2012.08.04 for CR 11725
    void updateFolderName(QString folderName); // added by jaden1.lee 2012.08.13 CR 11655
    // { modified by yongkyun.lee@lge.com  2012.08.28 for :CR13323 ,VR Create START/END
    
    // { added by  yongkyun.lee 2012.11.21 for  xSampa check.
    //void deviceMinerFinished(bool , MP::DEVICE_T); // add by yongkyun.lee@lge.com  2012.08.24for :CR13323
    void deviceMinerFinished(bool  , int  , MP::DEVICE_T ); 
    // } added by  yongkyun.lee 
    //void deviceMinerFinished(bool); // add by yongkyun.lee@lge.com  2012.08.24for :CR13323
    // } modified by  yongkyun.lee@lge.com  
    // { added by kihyung 2012.08.30 for DVD-Audio
    void trackNumberChanged();
    void groupNumberChanged();
    // } added by kihyung
    void isRunFromFileManagerChanged();//Added by Alexey Edelev. Fix bug 13058. 2012.09.13
    // void updateDvdTrackInfo(int state); // modified by ravikanth - 12-09-25 - cr 13789 // remvoed by kihyung 2012.11.26. removed spec.
    void clearSeekMode(); //added by junam 2012.10.10 for clear seek mode
    void iPodPlayableSongCount(int MasterTableCount, int DeviceCount); // added by jungae 2012.10.10 CR 13753
    // { added by eugeny.novikov 2012.10.25 for CR 14047
    void isBasicViewChanged();
    void isDiscTypeChanged(); // added by minho 20121221 for Removed error log when inserted DVD_AUDIO
    void enableModeArea();
    // } added by eugeny.novikov
    // { modified by wspark 2013.03.27 for ISV 77371
    //void updateMP3CDTrackInfo(int track); // added by wspark 2012.11.07 for updating mp3cd track info correctly.
    void updateTrackInfo(int track);
    // } modified by wspark
    //void auxAudioActivated(); // deleted by wspark 2012.12.10 for sending bluetooth opstate

    void tuneCounter(QString count);

    //{added by junam 2013.03.26 for ISV72425
    //void tuneMetaData( QString artist, QString song, QString album, QString folder, QString genre, QString composer, bool isColor );
    void tuneMetaData( QString artist, QString song, QString album, QString folder, bool isColor );
    //}added by junam

    void tuneCoverart( QString coverart );

    void dirTreeFetched( bool bFetched); // added by cychoi 2013.11.18 for ITS 208698 MP3 Play Info on Cluster 

    void iPodPlaying( int portNum, QString deviceName, bool connect );//added by jhcho 2013.02.20
    void UpdateStateTrackOSD(int speedCheck); // added by yungi 2012.11.14 for ISV 62667,ISV 62596
    //void isLoadingScreenChanged(); // removed by junam 2013.12.10 for code clean

    void showInhibitionIcon(bool bShow); // added by kihyung 2012.12.04 for ISV 62683
    void signalNoContent();//added by edo.lee 2012.12.21
    void iPodDeviceConnectInfo(int portNum, QString deviceName, bool connect); // added by jungae 2012.12.26, for send the connection information to AppBtPhone
    void releaseResource();//added by edo.lee 2012.12.27 ITS 150907
    void operationDisplay(DISPLAY_T disp);//added by edo.lee 2013.03.22
	void isPlayerModeChanged(); // added by wspark 2013.03.23 for ISV 76947
    void updateSongsCount(); // added by Dmitry 20.04.13 for ISV77651
    void changeFastFowardRewind(bool isFFRew, int status, int arrow); // added by Dmitry 23.04.13
    void isPlayFromMLTChanged(); // modified by Dmitry 27.04.13
    void showPlayerView();//added by junam 2013.05.15 for disbable flow view during re-sync
    void mp3fileInfo( QString m_coverart, QString m_artist, QString m_song, QString m_album, QString m_folder, QString m_countInfo ); //added by wonseok.heo 2013.05.17
    void isForegroundChanged(); // added by Dmitry 16.05.13
    void iPodSortingOrderChanged(); //added by junam 2013.06.07 iPod quick scroll

    void clearRepeatRandom(); //added by wonseok.heo for ITS 172869 2013.06.08

    void sendPositon(int discStatus); //added by wonseok.heo 2013.05.26   for disc inout test
    void sendPlayStatus(int playstatus , int trackNum); //added by wonseok.heo  2013.05.26  for disc inout test
    void wrongDisc(); //added by wonseok.heo  2013.05.26  for disc inout test
    void basicControlEnableStatusChanged(bool enable, QString AppName);//added by junam 2013.07.12 for music app
    void hideiPod3rdPartyTxt();
    void restoreIndexCountText();
    void clearAllJog(); // added by suilyou ITS 0206832
private:
    void ConnectJukeboxSignal();
    void ConnectUSBSignals();
    void ConnectDeckSignals();
    void ConnectIpod1Signal();
    void ConnectIpod2Signal();
    void ConnectIpod1SignalInRuntime();
    void ConnectIpod2SignalInRuntime();
    void disconnectControllers();
    // { removed by ruinivbe 2012.08.30 CR 12924
    // static bool isUtf8 (const char *str);  //added static by junam 08.22 for CR9771
    // } removed by ruinivbe
    void RequestComplete( QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID); //add reqUID by junam 2012.10.25 for CR14451

    CIpodController* GetIPodController(MP::DEVICE_T type);

    //{removed by junam 2013.11.06 for coverflow update
    //bool isCoverCleanRequired() const;
    //void setIsCoverCleanRequired( bool b_minerInProgress );
    //}removed by junam

    bool isFilePathValid( QString filePath, MP::DEVICE_T device); // modified by kihyung 2013.08.06 for ITS 0182926 
    MP::DEVICE_T GetSignalSource();
    void SetPlaylist(MP::DEVICE_T device, int index = 0);

    // [History]
    void ResetAllHistory();
    void ResetPlaylistInfo(int deviceId, bool bTrack = true); //add bTrack by junam 2013.08.17
    void ResetTrackInfo(int deviceId);
    void RestoreUI(MP::DEVICE_T device);
    void RestoreEmptyUI(MP::DEVICE_T device);  // modified by kihyung 2013.08.08 for ITS 0183321
    bool IsPlaylistEmpty();
    bool IsPlaylistValid(MP::DEVICE_T device);
    void CopyHistory(DeviceHistory &dst, const DeviceHistory &src); // modified by eugeny.novikov 2012.08.15 for CR 12578
    void CopyHistoryFromUBSExtended(DeviceHistory &dst, const DeviceHistory &src, QString mountPath);
    void CopyHistoryToUSBExtended(DeviceHistory &dst, const DeviceHistory &src);

    // Holds current audio mode to avoid request audio mode change in the same audio mode
    // TAudioMode m_currAudioMode; // removed by kihyung 2012.11.03 for booting disable
    void setSource( MP::DEVICE_T eMode );
    void setGracenoteHighlightedVolume(MP::DEVICE_T device);

    /* It's used for beforeIndexing feature.
     * It's allow request list of song, using qDir, while Tracker indexing new device.
     */
    void RequestBeforeIndexingPlaylist(QString mountPath);

    int getIPodSortingOrder();//changed by junam 2013.08.21 for iPod sorting    
    bool IsAlbumSupported(QStringList &filePaths); //changed by junam 2013.10.17 for ITS_KOR_196128
    //{ modified by yongkyun.lee 2013-12-13 for : ITS 213657
    void iPodMountedTimerStart();
    void iPodMountedTimerStop();
    //} modified by yongkyun.lee 2013-12-13 
    int m_InOutStepCnt; //added by wonseok.heo 2013.07.04 disc in out test
    int m_InOutTestCnt; //added by wonseok.heo 2013.07.04 disc in out test
private:
    IMediaInterface      *m_pIMedia;
    IUSBExMediaInterface *m_pIMediaEx;  // added by kihyung 2013.08.08 for ITS 0181967 
    
    JukeboxController    *m_pJukeboxController;
    USBController        *m_pUsbController;
    DeckController       *m_pDeckController;
    CIpodController      *m_pIpod1Controller;
    CIpodController      *m_pIpod2Controller;
    CTrackerAbstractor   *m_pTrackerAbstractor;
    CTrackerExtractorClient *m_pTrackerExtractor;
    IAudioInterface      *m_pIAudio;
    IVideoInterface      *m_pIVideo;
    GPlayerEngine                        *m_pGPlayerEngine; //added by junam 2013.09.13 for ITS_KOR_189335

    PLAYER_STATE          m_PlayerState;
	int 		          m_NewState; // added by eugene 2012.11.26 for ISV #59906
	int 				  m_EventId; // added by eugene 2012.11.26 for ISV #59906
    MP::DEVICE_T          m_device;
    MP::DEVICE_T          m_deviceValid;//{ modified by yongkyun.lee 2013-08-30 for : BT sound mute
    bool                  m_isSourceChanged; // added by kihyung 2013.2.12 for MEDIA HK Crash

//{ added by yongkyun.lee 2012.12.19 for CR 14219 ,POPUP/list close
    bool                  m_isAccOn ;
    bool                  m_isFrontLCD ;
    //bool                m_bCamInFG;
//} added by yongkyun.lee     
    int                   m_randomPlayMode;
    int                   m_scanMode;
    int                   m_discScanPlayingMode; // added by wonseok.heo for ITS 191295 2013.09.26
    int                   m_repeatPlayMode;
	bool				  m_isListView; // modified by eugene.seo 2012.10.23 for Function_USB_0190
	int				      m_currentCategoryUSB1; // modified by eugene.seo 2013.02.07 for unsupported audio play in each repeat mode	
	int				      m_currentCategoryUSB2; // modified by eugene.seo 2013.02.07 for unsupported audio play in each repeat mode
	int				      m_currentCategoryJukebox; // added by eugene.seo 2013.02.07 for unsupported audio play in each repeat mode

    /* Temp variable */
    QUrl                  m_folderPath;
    QString               m_albumName;
    QUrl                  m_albumThumbnail;
    QStringList           m_songNameList;

    QStringList           m_songNameListTemp;
    QStringList           m_pathsTemp;
    QString               m_albumNameTemp;

    QStringList           m_pathsTune;
    QStringList           m_tuneTrackNames;
	// { removed by junam 2012.11.02 for unusing code.
    //QString               m_coverartTemp;
    //QStringList           m_artistTemp;
    //QStringList           m_albumTemp;
    //QStringList           m_genreTemp;
    //QStringList           m_composerTemp;
    //QString               m_dynamicTemp; // added by junggil 2012.08.02 for CR12564
	//} removed by junam
    // { modified by junggil 2012.08.10 for CR12745
    //int                   m_lastTuneSelectIndex;
    //QStringList            m_filePathTuneList; //removed by junam 2012.12.14 not using.
    // } modified by junggil
    // } add by junggil
    QList<int>            m_tuneTrackIDs;

    QStringList           m_albumNameList;

    TUNE_IN_PROGRESS_STATE m_tuneState;
    ETrackerAbstarctor_VolumeType volumeT;

    bool                  m_isPlayFromMLT;
    bool                  m_isBTStreaming;// modified by yongkyun.lee 2013-10-14 for : ITS 195248
    CATreeModel          *m_pTreeModel;
    // QRPMClient        *rpmClientObj; // removed by kihyung 2012.11.03 for booting disable
    PathViewModel        *m_pPathViewModel;
    //SearchEngine         *m_pSearchEngine;
    CListModel           *m_pListViewModel;

    // bool               m_isPrev; // removed by kihyung 2013.08.28
    bool                  m_NoOSD;// add by yongkyun.lee@lge.com 2012.07.14 CR 11922
    MINER_IN_PROGRESS_STATE    m_minerState;
    int                   m_isLongPressed;

    // QDBusInterface       *m_pMoreLikeThis; // removed by kihyung 2013.07.19
    QStringList           m_moreLikeThisList;
    QStringList           m_moreLikeThisList_artist; //added by aettie.ji 2013.02.18 for ux optimization
    QStringList           m_mltFilePathesList;
    QList<CD_META_INFO>   m_listCDContent;

    // { removed by eugeny.novikov 2012.08.15 for CR 12578
//    QStringList m_listDiscTitle;    // Add by Naeo 2012.07.09 for CR 10970
    // } removed by eugeny.novikov

// { deleted by junggil 2012.07.18 for CR11957
// CR9842 : Cover art is not displayed for juke box and usb audio files.
// CR10562 : New Controller api is required to refresh track meta-information
//    QStringList 		  m_newAlbums;
//    QStringList 		  m_newArtists;
//    QStringList 		  m_newFiles;
// } add by junggil
//
     /* tuning stuff */
    int m_nPendingMedia;    /**! -1 if no track is expected to be
                                * selected on switchToTuned() or track
                                * otherwise. */
    int m_nSavedMedia; // added by suilyou 20131030 ITS 0204905

    // QString m_lastSong; // removed by kihyung 2013.08.28
    bool m_isForeground;

    bool m_bIsApple1Ready;
    bool m_bIsApple2Ready;
    //{added by junam 2013.05.02
    bool m_bIsDatabase1Ready;
    bool m_bIsDatabase2Ready;
    //}added by junam
    bool m_bIsBTPlaying;

    bool m_bBTAvrcpAbove13;// added by yongkyun.lee 20130427 for : BT sync
    bool m_bIsAppleConnectError; // added by kihyung 2012.07.09 for CR 10343
    // removed by cychoi 2013.12.29 for ITS 217565

	int m_savePosition; //added by edo.lee 2012.12.14
	bool m_isShowPopup;//added by edo.lee 2013.09.02 ITS 0184880

    // removed by kihyung 2013.08.28
    /*
    enum RND_FLAFS
    {
        NONE,
        REQ_NEW_LIST,
        RND_IN_PROGRESS
    } m_rndFlag ;
    */

    /* Device history record for quick data access */
    DeviceHistory m_history[MP::ALL_DEVICES];
    // { added by eugeny.novikov 2012.08.15 for CR 12578
    // Both lists should always contain (DEVICE_HISTORY_MAX_COUNT-1) items at any time
    QList<DeviceHistory> m_usbDevicesHistory;
    // } added by eugeny.novikov
    bool m_bRestoring;

    // "play" command is not available if other media app is active
    bool m_bPlayerActive;

    MP::DEVICE_T                   m_pendingSource;
    bool                           m_isResourcesOwned;
    MEDIA_SOURCE_T  m_discType;
    bool m_bShowOSD;
    struct SEARCH_DATA
    {
        bool bIsSearch;
        int matchField;   // Used only when track has been choosen from ALL search tab
        QString trackUrl;
    } m_searchData;

    // { removed by eugeny.novikov@teleca.com 2012.08.01 for CR 11079
    // [Before Indexing]
//    QStringList m_beforeIndexingDevices;
    // } removed by eugeny.novikov

    int     m_enableErrorPopupState;
    // DeviceHistory m_preMLTHistory[MP::ALL_DEVICES]; // modified by kihyung 2013.4.3 for ISV 78555 // removed by kihyung 2013.07.15

    MediaInfo m_deckInfo;

    // removed by ruindmby 2012.08.06 for CR12391
    //bool                   m_bisMultipleiPodDevice;
    //bool                   m_bisMultipleUSBDevice;
    // modified by ruindmby 2012.08.06 for CR12391
    bool                   m_bRequestCoverOnBasicView; // added by kihyung 2012.07.09 for CR 11165

    // { removed by kihyung 2013.2.17
    /*
    // { added by kihyung 2012.07.24 for CR 9673
    int                             m_nContentSize;
    MP::DEVICE_T                    m_eContentSizeDevice;
    ETrackerAbstarctor_VolumeType   m_eContentSizeVolType;
    // } added by kihyung
    */
    // } removed by kihyung 2013.2.17
     
    bool m_bDiscPostponeRestore; // added by ruindmby 2012.08.09 for CR 12782
    bool m_bUpdateFileFolderIndex;

    // { added by kihyung 2012.08.30 for DVD-Audio
    QWidget    *m_pVideoWidget; 
    bool        m_frontScreen;
    QString     m_strTrackNumber;
    QString     m_strGroupNumber;
    // } added by kihyung

    QString m_iPod1DeviceName; // added by 2012.09.03 for CR 13162
    QString m_iPod2DeviceName; // added by jungae 2012.12.26, for send the connection information to AppBtPhone
    
    bool m_isRunFromFileManager;//Added by Alexey Edelev. Fix bug 13058. 2012.09.13
    //Added by Alexey Edelev. 2012.09.08 CR13600, CR13610
    //{
    bool m_iPod1ContentSizeReady;
    bool m_iPod2ContentSizeReady;
    //}
    //Added by Alexey Edelev. 2012.09.08 CR13600, CR13610
	bool m_isForwardDirection; // added by wspark 2012.09.10 for CR12707

    //QString m_filePathFromWholeList; // added by eugeny - 12-09-15
	// { added by ravikanth - 12-09-26
    DVDStatusResponse m_tempDVDStatus;
    bool m_isDVDinfoRestored;
	// } added by ravikanth - 12-09-26
    bool m_bIsBasicView; // added by eugeny.novikov 2012.10.25 for CR 14047
    //bool m_bIsLoadingScreen; // removed by junam 2013.12.10 for code clean
    //bool m_bFirstEntry[MP::ALL_DEVICES]; //removed by junam 2014.01.17 deprecated code
    bool m_btSending;	//added by edo.lee 2012.12.28 ITS 150290
    // QTimer *m_requestFirstSongTimer; // added by eugene.seo 2013.01.18 for 25000 over USB // removed by kihyung 2013.2.17
    // { added by wspark 2013.01.23
    int m_lastPlayedIndex;
    int m_lastPlayedPos;
	DeviceHistory tempHistory; // added by eugene.seo 2013.04.10
    // } added by wspark
    //{removed by junam 2013.03.10 for ISV73458
    //QTimer *m_tuneCoverDelayTimer; // added by lssanh 2013.01.24 ISV70714
    //}removed by juanam
    QString m_DelayCoverArt; // added by lssanh 2013.01.24 ISV70714
    QTimer *m_pBtResponseTimer; // added by wspark 2013.02.07 for ISV 72168
    QTimer *m_pBtDelayedGetInfoTimer; // added by cychoi 2014.06.03 for Get Info - 1. on TrackChanged 2. delayed 0.5sec from PlayStarted
    QTimer *m_pBtPeriodicGetInfoTimer; // added by cychoi 2014.06.05 for Get Info - every 10sec after PlayStarted (some phones don't send TrackChanged)
    QTimer *m_pBtRequestTimer;// added by yongkyun.lee 20130426 for : BT sync
    int m_iTrackInfoReqCount;	//added by hyochang.ryu 20130611 for BT speedup
    // { modified by cychoi 2014.05.12 for duplicated code (added by hyochang.ryu 20130627 for BT delayed play, 20131031 for ITS 205112)
    QTimer *m_pBtDelayedPlayTimer;
    //QTimer *m_pBtDelayedPlayTimer1;
    // } modified by cychoi 2014.05.12
    QTimer *m_pBtDelayedMuteTimer;	//added by hyochang.ryu 20130913 for ITS189483
    QTimer *m_iPodMountTimer;//{ modified by yongkyun.lee 2013-12-13 for : ITS 213657
    bool m_bBTDelayedPlayOnIPodMount; // added by cychoi 2014.04.03 for ITS 233324
    //QTimer *m_pMuteTimer; // commented by cychoi 2014.05.12 for deprecated code //added by edo.lee 2013.04.21 for Mute
    QTimer *m_pLongBtnTimer;// added by edo.lee 2013.10.12 pop noise
    QTimer *m_pScanTimer;// added by jaehwan.lee 2014.02.27 to fix ITS 227260
    /*//{ added by cychoi 2013.08.25 for BT Music OSD display. Need more investigation later.
    QTimer *m_pBTOSDDisplayTimer;
    bool m_bBTOSDDisplayEnabled;
    //} added by cychoi 2013.08.25*/
    //{added by junam 2013.03.10 for ISV73458
    QTimer m_tuneMetaInfoTimer;
    //QTimer m_tuneCoverArtTimer;//removed by junam 2013.10.17 for deprecated code
    //}added by junam

    QMutex m_tMediaInfoMutex; // added by kihyung 2013.2.12 for MEDIA HK Crash
    QMutex m_tSetSrcMutex; // added by kihyung 2013.2.12 for MEDIA HK Crash

    // { added by kihyung 2013.2.17
    int m_bJukeboxDelayedPlay;
    int m_bUSB1DelayedPlay;
    int m_bUSB2DelayedPlay;
    // { added by kihyung 2013.2.17    
    QString m_USB1MiningPath; // added by eugene.seo 2013.04.16
    QString m_USB2MiningPath; // added by eugene.seo 2013.04.16
    // int m_currentRepeatmode; // added by eunhye 2013.03.30 for ISV 78066 // removed by kihyung 2013.05.27 for ITS 148293

    int m_nIPOD1ErrIndex; // added by kihyung 2013.4.7 for IPOD
    int m_nIPOD2ErrIndex; // added by kihyung 2013.4.7 for IPOD
    int nAcount; // added by eugene.seo 2013.04.15
    GNIndexing m_CDDAGraceNote;//added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
    int m_iPodChangingPosition;//added by edo.lee 2013.04.25
    int m_moveTrackIndex;//added by edo.lee 2013.05.09
    bool m_isChangeToSame;//added by edo.lee 2013.05.18
    int m_skipTrackCount;//added by edo.lee 2013.05.18

	int m_seekIndex;//added by edo.lee 2013.05.30
	int m_nFolderSize;

    DiscMuteState m_eDiscMuteState; // added by cychoi 2013.06.02 for sound noise when dragged progress bar
    int m_iPodSortingOrder; //added by junam 2013.06.07 iPod quick scroll

	bool m_bNewUSB1Connected; //added by edo.lee 2013.06.10
	bool m_bNewUSB2Connected; //added by edo.lee 2013.06.10

    QTimer *m_DiscLoadRequest; //added by wonseok.heo 2013.07.04 disc in out test 
    QTimer *m_pAudioSinkTimer; // added by kihyung 2013.07.04 for ISV 85455
    int     m_nLastErrorCode; // added by kihyung 2013.12.17 for ITS 0214009 

    QString m_tuneTitle;
	bool m_isCreatNewList;
	bool m_isChangedDevice;
    bool m_bBasicControlEnableStatus; //added by junam 2013.07.12 for music app    
    QVariantList m_categoryList;//added by junam 2013.07.19 for ITS172937

    QString m_strUpdatedMediaInfo; // added by kihyung 2013.08.27 for ITS 0186708 
    bool m_BTSetMediaMode;	//added by LYK 20130828
    bool m_skipTrack;//added by edo.lee 2013.08.30
    bool m_WillUnMute;
    ID3TagState m_eID3TagState; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
    int m_nID3TagTrackIdx; // added by cychoi 2014.01.07 for keep MP3 ID3 Tag if same song 
    int m_nID3TagRxCount; // added by cychoi 2013.12.17 for ITS 216033
    bool m_jukeboxUpdate; //added by junam 2013.10.03 for ITS_KOR_192927
    int m_autoTrackChangeIndex;
    bool m_bFF;// added by edo.lee 2013.10.12 pop noise
    bool m_bCoverFlowEnabled;
// removed by Dmitry 13.10.13
    bool m_bFirstDiscOSD; // added by oseong.kwon 2014.02.04 for OSD update on TrackChange

    QMap<QString, QString> m_imageMap; //added by junam 2013.10.28 for 2nd depth coverart
    QString m_coverflowClickedItem; //added by junam 2013.11.16 for coverflow click
};

#endif // DHAVN_MP_AUDIOCONTROLLER_H
