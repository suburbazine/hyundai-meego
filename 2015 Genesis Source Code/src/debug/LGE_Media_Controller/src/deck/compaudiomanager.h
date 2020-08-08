#ifndef COMPAUDIOMANAGER_H
#define COMPAUDIOMANAGER_H

#include <QObject>
#include <QTime>
#include <QHash>
#include <QTimer> // added by cychoi - To avoid MP3 list read error.
#include "imediainterface.h"
#include "iaudiointerface.h"
#include "icdinterface.h"
#include "packet.h"
#include "ctree.h"
#include "deck_def.h"

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
// 1. MP3 list read error
// 2. MP3 tag info read error
// 3. MP3 playback stop error.
#define CA_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05
#define CA_DECK_REQ_RETRY_LIMITATION_ENABLED // added by cychoi 2013.11.25 for enabling to handle error disc on detection timeout

// { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
typedef enum
{
    CA_PLAY_NEXT,
    CA_PLAY_PREV,
    CA_PLAY_FF,
    CA_PLAY_REW,
    CA_PLAY_CURR,
    CA_PLAY_NONE
}CAPlayDirection;
// } added by cychoi 2013-07-11

// { added by cychoi 2013.09.06 for ITS 178797
typedef enum
{
    CA_DECK_REQ_NONE, // No request
    CA_DECK_REQ_SENT, // Request sent
    CA_DECK_REQ_DONE  // Response received
}CADeckReqState;
// } added by cychoi 2013.09.06

class CompAudioManagerStatus
{
public:
    CompAudioManagerStatus()
    {
        m_nPlayStatus   = eDM_CD_PLAY_COND_HALTING;
        m_nFileNo       = 0;
        m_nRandomMode   = eDM_COMP_RANDOM_NONE;
        m_nRepeatMode   = eDM_COMP_REPEAT_NONE;
        m_nScanMode     = eDM_COMP_SCAN_NONE;
        // { added by cychoi 2013-07-15 for Prevent fix
        m_nType            = eDM_COMP_TYPE_MP3_FILE;
        m_nTagExist        = eDM_TAG_NOT_READ_TAG;
        m_nDiscNo          = 0;
        m_nCurrFolderNo    = 0;
        m_nMPegVer         = eDM_COMP_AUDIO_MPEG_2;
        m_nMPegLayer       = eDM_COMP_AUDIO_MPEG_LAYER_3;
        m_bNoErrorSecurity = 0;
        m_nBitrate         = 0;
        m_nSampling        = 0;
        m_nExtension       = 0;
        m_nChannel         = eDM_COMP_CHANNEL_STEREO;
        m_nModeExtension   = 0;
        m_bCopyRight       = 0;
        m_bOriginal        = 0;
        m_nEmphasis        = 0;
        m_bUnsupported     = 0;
        m_nDirectStopMode  = eDM_DIRECT_STOP_OFF;
        m_nPlayBackTime    = 0;
        m_nAudioOutput     = eDM_CD_AUDIO_OUTPUT_ANALOG_2CH;
        m_nDisplayScreen   = eDM_DISPLAY_SCREEN_PLAY;
        m_pFileName[0]     = 0;
        m_bUnicode         = 0;
        // } added by cychoi 2013-07-15
    }

    DECK_CD_PLAY_COND           m_nPlayStatus;
    DECK_COMP_AUDIO_FILE_TYPE   m_nType;
    DECK_COMP_AUDIO_TAG_EXIST   m_nTagExist;
    DECK_COMP_RANDOM_MODE       m_nRandomMode;
    DECK_COMP_REPEAT_MODE       m_nRepeatMode;
    DECK_COMP_SCAN_MODE         m_nScanMode;
    int                         m_nDiscNo;
    int                         m_nCurrFolderNo;
    int                         m_nFileNo;
    DECK_COMP_AUDIO_MPEG_VER    m_nMPegVer;
    DECK_COMP_AUDIO_MPEG_LAYER  m_nMPegLayer;
    int                         m_bNoErrorSecurity;
    int                         m_nBitrate;
    int                         m_nSampling;
    int                         m_nExtension;
    DECK_COMP_CHANNEL_MODE      m_nChannel;
    int                         m_nModeExtension;
    int                         m_bCopyRight;
    int                         m_bOriginal;
    int                         m_nEmphasis;
    int                         m_bUnsupported;
    DECK_DIRECT_STOP_MODE       m_nDirectStopMode;
    int                         m_nPlayBackTime;
    DECK_CD_AUDIO_OUTPUT_MODE   m_nAudioOutput;
    DECK_DISPLAY_SCREEN_MODE    m_nDisplayScreen;
    char                        m_pFileName[32];
    int                         m_bUnicode;
};

class CompAudioDirTreeStatus
{
public:    
    CompAudioDirTreeStatus()
    {
        m_nHierachy         = 0;
        m_nCurrFolderNo     = -1;
        m_nCurrBlockNo      = 0;
        m_nMaxBlockNo       = -1;
        m_nChildFolderNum   = 0;
        m_nChildFileNum     = 0;
    }

    int                         m_nHierachy;
    int                         m_nCurrFolderNo;
    int                         m_nCurrBlockNo;
    int                         m_nMaxBlockNo;
    int                         m_nChildFolderNum;
    int                         m_nChildFileNum;
};

class CompAudioManager : public QObject, public ICDInterface, public IMediaInterface, public IAudioInterface
{
    Q_OBJECT
public:
    enum RequestDirTreeMode {
        RDTM_NONE,
        RDTM_DIR_TREE,
        RDTM_CURR_FOLDER_NAME,
        RDTM_CHILD_FILE_NAME,
        RDTM_CHILD_FOLDER_NAME,
        RDTM_COMPLETE,
        RDTM_REQUEST_FILE_SIZE
    };

    struct FILE_SIZE_INFO {
        int     m_nStartNum;
        int     m_nFileSize;
    };
    
    static CompAudioManager*    GetInstance();
    static void                 DeleteInstance();
    
    virtual        ~CompAudioManager();

    ///////////////////////////////////////////////////////////
    // FOR ICDInterface
    void            Init();
    void            StartMedia();
    void            EndMedia();
    void            ProcessPacket(StatusData *pPacket);

    ///////////////////////////////////////////////////////////
    // FOR IMediaInterface
    int             ChangeMediaMode(MEDIA_PLAYER_MODE eMode);
    QString         GetCurrentFileName();
    int             GetCurrentIndex();
    void            SetCurrentIndex(int nIndex);
    int             GetCurrentMediaState();
    int             GetCurrentMediaDuration();
    int             GetCurrentMediaPosition();
    void            SetCurrentMediaPosition(int position);
    int             GetDeviceState();

    void            GetCurrentMediaInfo(MediaInfo *pMediaInfo);
    void            GetDeviceInfo(DeviceInfo *pDeviceInfo);
    void            GetPlaylist(PlayList *pPlaylist);

    void            SetPlayList(PlayList *pPlayList);
    bool            SeekableMedia();
    void            SetBrightness(double nBrightness);
    void            SetContrast(double nContrast);
    void            SetTint(double nTint);
    void            SetBrightness(MEDIA_DISPLAY displayPlane, double fBrightness);
    void            SetContrast(MEDIA_DISPLAY displayPlane, double fContrast);
    void            SetTint(MEDIA_DISPLAY displayPlane, double fTint);
    double          GetBrightness();
    double          GetContrast();
    double          GetTint();
    void            SetSystemState(int nSystemState);
    QWidget*        GetVideoWidget();
    void            EnableSubTitle(bool bEnable);
    void            SetPlayerEngine(PLAYER_ENGINE_TYPE eType);
    void            SetFontSize(const int fontSize);
    bool            ClearDisplay(bool isBackground);
    void            SetHue(double value);
    void            SetHue(MEDIA_DISPLAY displayPlane, double value);
    double          GetHue();
    void            GetCurrentFileFolderIndex(int *pFileIdx, int *pTotalFiles);
    void            AudioDragStart();
    void            AudioDragEnd();    

    ///////////////////////////////////////////////////////////
    // FOR IAudioInteface
    void            Load();
    void            Eject(uint mode);
    void            EnterKey();
    void            CursorKey(uint direction);
    void            ReturnKey();
    void            SwitchPlayMode(bool bCDDAMode);
    int             RequestTOC();
    int             RequestTOC2(int nBlockNo);
    int             RequestGraceNoteTOC();
    void            TenKey(int nKeyID, int pressStatus);
    int             RequestCDText(int nTrackNo, int nDataType, int nBlockNo, int nFrameNo);
    int             RequestCDTextInfo();
    int             RequestDirTree(unsigned int nMode, unsigned int nFolderNo, unsigned int nBlockNo);
    int             RequestTag();
    int             RequestTag2(int nInfoType);
    int             RequestAllTagInfo();
    int             RequestFileName(unsigned int nFolderNo, unsigned int nFileNo);
    int             RequestFolderName(unsigned int nFolderNo);
    int             GetTotalFileFolder(TotalFileFolderCount *pCount);    
    int             GetCDStatus(CDDAStatusResponse *pCDStatus);
    int             GetCAStatus(CAStatusResponse *pCAStatus);

    ///////////////////////////////////////////////////////////
    // ETC
    int             GetCurrentScreenState();
    
public slots:    
    void            Play();
    void            Pause();
    void            FunctionStop();
    void            Stop();
    void            Next(int nSkipLevel);
    void            Prev(int nSkipLevel, int nForced = 0);
    void            Scan(int nMode);
    void            RandomPlay(int nMode, int bPlay = true);
    void            RepeatPlay(int nMode, int bPlay = true);
    void            FastForward(int nRate);
    void            Rewind(int nRate);
    void            NextFolder(PlayList *pPlayList);
    void            PrevFolder(PlayList *pPlayList);  
    void            HandleRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    void            HandleRetryFileFolderTotalNum(); // added by cychoi 2013.11.25 for ITS 210845
    void            HandleRetryTagInfo();
    void            HandleRetryPlayback();
#endif
// } added by cychoi 2013-06-05

signals:
    ///////////////////////////////////////////////////////////
    // FOR IMediaInferface    
    void            OnPositionChanged(int nPosition);
    void            OnDurationChanged(int nDuration);
    void            OnStateChanged(int nState);
    void            OnDiscStateChanged(int nDiscState);
    void            OnScreenStateChanged(int nScreenState);
    void            OnError(int nError);
    void            OnScanCompleted();
    void            OnTrackChanged(int nTrackIndex);
    void            OnTrackUpdated(int nTrackIndex); // added by cychoi 2013.11.17 for ITS 208698 MP3 Play Info on Cluster 
    void            OnModeChanged(int nMode);     
    void            OnCurrentMediaStatusChanged(int);
    void            OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize);
    void            OnNormalPlay(); // added by Michael.Kim 2013.05.23 additional signal for FF/Rew functionality.
    void            OnRealSoundChanged(); // added by cychoi 20103.06.11
    void            OnUserTrackChanged(int nTrackIndex); // added by cychoi 20103.07.19 for add new signal
    void            OnAutoTrackChanged(int nTrackIndex); // added by cychoi 2014.04.06 for ITS 233500
    
    ///////////////////////////////////////////////////////////
    // FOR IAudioInterface
    void            OnTOCReceived(DvdcTOC *pTOC);
    void            OnTOC2Received(DvdcTOC2 *pTOC2);
    void            OnCDTextReceived(DvdcCDText *pCDText);
    void            OnCDTextInfoReceived(DvdcCdTextInfo *pCDTextInfo);
    void            OnDirTreeReceived(DvdcDirTree *pDirTree);
    void            OnTagReceived(DvdcTagInfo *pTag);
    void            OnTag2Received(DvdcTag2Info *pTag2);
    void            OnAllTagReceived(DvdcAllTagInfo *pAllTag);
    void            OnFileNameReceived(DvdcFileName *pFileName);
    void            OnFolderNameReceived(DvdcFolderName *pFolderName);
    void            OnDirTreeCompleted();

private:
    CompAudioManager(QObject *parent = 0);
    void            RandomNotice(int nMode);
    void            RandomPlay_Impl(int nMode, int bSignalEmit = true);

    int             InitDirTree();
    int             StartDirTree();
    int             NextDirTree();

    // { added by cychoi - To avoid MP3 list read error.
    int             InitRetryDirTree();
    int             StartRetryDirTreeTimer(unsigned int nFolderNo, unsigned int nFileNo, unsigned int nBlockNo);
    // } added by cychoi
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    // { added by cychoi 2013.11.25 for ITS 210845
    int             InitRetryFileFolderTotalNum();
    int             StartRetryFileFolderTotalNumTimer(int ms);
    // } added by cychoi 2013.11.25

    int             InitRetryTagInfo();
    int             StartRetryTagInfoTimer(int ms); // modified by cychoi 2013.09.06 for ITS 178797


    int             InitRetryPlayback();
    int             InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
    int             StartRetryPlaybackTimer(int ms);
#endif
// } added by cychoi 2013-06-05

    int             NextFileName(unsigned int nFolderNo, unsigned int nFileNo, unsigned int nBlockNo);
    int             NextFolderName(unsigned int nFolderNo, unsigned int nBlockNo);

    // { added by cychoi - To avoid MP3 list read error.
#if _DECK_DIAGNOSTIC_
    bool            ExtractResetStart(StatusData *pPacket);
#endif
    // } added by cychoi
    bool            ExtractCompAudioStatus(StatusData *pPacket);
    int             ExtractDirTreeStatus(StatusData *pPacket);
    bool            ExtractTagStatus(StatusData *pPacket);
    bool            ExtractTag2Status(StatusData *pPacket);
    bool            ExtractFileNameStatus(StatusData *pPacket);
    bool            ExtractFolderNameStatus(StatusData *pPacket);
    bool            ExtractFolderFile4Status(StatusData *pPacket);
    bool            ExtractFileFolderTotalNumber(StatusData *pPacket);
    bool            ExtractReqError(StatusData *pPacket);

    int             GetRandomMode(DECK_COMP_RANDOM_MODE eMode);
    int             GetRepeatMode(DECK_COMP_REPEAT_MODE eMode);
    int             GetScanMode(DECK_COMP_SCAN_MODE eMode);
    int             CharToUnicode(UINT1 *pIn, QChar *pOut, int nUniSize);
    QString         DeckStringToUnicode(int nType, char *pString, int nLen);

    void            EmitError(int nError);
    void            EmitTrackBuffered(int bPrev, int nTotalIdx);

    // { added by cychoi 2013-07-15 for ITS 179045 Track Up <-> Down press repeatedly
    void            GetMoveFileFolderIndex(int moveTrackIndex, int *pFileIdx, int *pTotalFiles);
    // } added by cychoi 2013-07-15
    // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
    int             GetNextTrackIdx(int nSkiplevel, int moveTrackIndex);
    int             GetPrevTrackIdx(int nSkiplevel, int moveTrackIndex);
    int             GetRandomTrackIdx(int moveTrackIndex); // added by cychoi 2015-05-19 for QE_Smoke Test Ver_45버전(130519) 37 fail
    // } modified by cychoi 2013-07-10
#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
    bool            HandleErrorDisc(bool bRetryToPlay);
#endif

    int             GetInternalMediaState(); // added by cychoi 2013.12.26 for ITS 217306 DVD black
    void            EmitTrackChange(int nTrackNo, bool bTrackChanged, bool bChangedFFRew); // added by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050

    static CompAudioManager     *m_pInstance;
    
    int                          m_bLoadComplete;

    Packet                      *m_pPacket;
    //CTree                       *m_pTree; // commented by cychoi 2014.01.24 for unused var
    CompAudioManagerStatus       m_tStatus;
    CompAudioDirTreeStatus       m_tDirTree;

    int                          m_nTotalFolderNum;
    int                          m_nTotalFileNum;

    int                          m_nCurrFileIndex;
    int                          m_nCurrFolderIdx;
    int                          m_nCurrFolderNo; // added by cychoi 2013.11.17 for ITS 208698 MP3 Play Info on Cluster 

    MEDIA_PLAYER_STATE           m_ePlayerState;
    MEDIA_PLAYER_STATE           m_eLastPlayerState; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
    int                          m_nSystemState;

    int                          m_nRandomMode;
    int                          m_nRepeatMode;
    int                          m_nUserRandomMode;
    int                          m_nUserRepeatMode;
    // { modified by cychoi 2013.11.15 for ITS 205008
    int                          m_nDelayedRandomMode;
    int                          m_nDelayedRepeatMode;
    int                          m_bDelayedModeSet;
    // } modified by cychoi 2013.11.15
    // { modified by cychoi 2013.11.18 for ITS 209699
    int                          m_nDelayedScanMode;
    int                          m_bDelayedScanModeSet;
    // } modified by cychoi 2013.11.18
    int                          m_nFFREWRate;
    int                          m_nFFREWState; // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error  // added by cychoi 2013-06-07 for ITS 172685
    int                          m_bREWMode;
    bool                         m_bFFREWStateSigSend; // added by sjhyun 2013.09.14 for ITS 189282

    QString                      m_strFileName;
    //QTime                        m_tDebugTime; // commented by cychoi 2014.01.24 for unused var

    // FOR APP
    // { modified by cychoi 2013-05-18 for ISV 77101
    //int                                             m_bRequestFileName;
    //int                                             m_bRequestFolderName;
    unsigned int                 m_nRequestFileNameFolderNo;
    unsigned int                 m_nRequestFileNameFileNo;
    unsigned int                 m_nRequestFolderNameFolderNo;
    // } modified by cychoi 2013-05-18
    // { modified by cychoi 2013.09.06 for ITS 178797
    //CADeckReqState               m_bRequestDirTree; // modified by cychoi 2013.09.06 for ITS 178797
    CADeckReqState               m_eAppRequestTagInfo;
    CADeckReqState               m_eAppRequestTag2Info;
    CADeckReqState               m_eAppRequestAllTagInfo;
    // } modified by cychoi 2013.09.06
// { added by cychoi 2013.11.25 for ITS 210845
#ifdef CA_DECK_REQ_RETRY_ENABLED
    CADeckReqState               m_eRequestFileFolderTotalNum;
#endif
// } added by cychoi 2013.11.25

    int                          m_nChildIdx;
    int                          m_nMaxChildNum;
    RequestDirTreeMode           m_eDirTreeMode;
    
    DvdcDirTree                  m_tDvdcDirTree;
    DvdcTagInfo                  m_tTagInfo;
    DvdcTag2Info                 m_tTag2Info;
    DvdcAllTagInfo               m_tAllTagInfo;
    DvdcFileName                 m_tFileName;
    DvdcFolderName               m_tFolderName;

    //int                      m_bFirstBoot; // commented by cychoi 20130.07.11 for keeping playback options
    int                          m_bFirstPlay;
    int                          m_bControllerChanged;

    QHash<int,FILE_SIZE_INFO>    m_tFileSizeInFolder;
    QList<int>                   m_tUserReqTrackList;

    // { added by cychoi - To avoid MP3 list read error.
    QTimer                       m_tRetryDirTreeTimer;
    unsigned int                 m_nRetryDirTreeFolderNo;
    unsigned int                 m_nRetryDirTreeFileNo;
    unsigned int                 m_nRetryDirTreeBlockNo;
    // } added by cychoi
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    QTimer                       m_tRetryFileFolderTotalNumTimer; // added by cychoi 2013.11.25 for ITS 210845
    QTimer                       m_tRetryTagInfoTimer;
    QTimer                       m_tRetryPlaybackTimer;
    MEDIA_PLAYER_STATE           m_nRetryPlaybackMediaState;
    DECK_COMP_RANDOM_MODE        m_nRetryPlaybackRandomMode;
    DECK_COMP_REPEAT_MODE        m_nRetryPlaybackRepeatMode;
    DECK_COMP_SCAN_MODE          m_nRetryPlaybackScanMode;
#endif
// } added by cychoi 2013-06-05

#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
    int                          m_nDiscDetectRetryCount;
    int                          m_nDiscPlayRetryCount;
#endif

    int                          m_bAutoTrackChanged; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    int                          m_bUserTrackChanged; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
// { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
    CAPlayDirection              m_ePlayDirection;
// } added by cychoi 2013-07-11

    bool                          m_scanStatus; // added by wonseok.heo 2013.10.03 for smoke test random repeat
    int                          m_bBTCallStart; // added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
};

#endif // COMPAUDIOMANAGER_H
