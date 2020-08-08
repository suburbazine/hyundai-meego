#ifndef CDDAMANAGER_H
#define CDDAMANAGER_H

#include <QObject>
#include <QTime>
#include <QTimer> // added by cychoi 2013-06-05 To avoid CDDA playback stop error.
#include "imediainterface.h"
#include "iaudiointerface.h"
#include "icdinterface.h"
#include "packet.h"
#include "deck_def.h"

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
// 1. CDDA GraceNote toc info read error.
// 2. CDDA toc info read error.
// 3. CDDA toc2 info read error.
// 4. CDDA CD Text info read error.
// 5. CDDA playback stop error.
#define CDDA_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05
#define CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED // added by cychoi 2013.11.25 for enabling to handle error disc on detection timeout

// { modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050 // { commented by cychoi 2013.07.13 for MLT spec out
// { added by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
typedef enum
{
    CDDA_PLAY_NEXT,
    CDDA_PLAY_PREV,
    CDDA_PLAY_FF,
    CDDA_PLAY_REW,
    CDDA_PLAY_CURR,
    CDDA_PLAY_NONE
}CDDAPlayDirection;
// } added by cychoi 2013.07.09
// } modified by cychoi 2014.08.29 // } commented by cychoi 2013.07.13

// { added by cychoi 2013.09.06 for ITS 178797
typedef enum
{
    CDDA_DECK_REQ_NONE, // No request
    CDDA_DECK_REQ_SENT, // Request sent
    CDDA_DECK_REQ_DONE  // Response received
}CDDADeckReqState;
// } added by cychoi 2013.09.06

class CDDAManagerStatus
{
public:
    CDDAManagerStatus()
    {
        m_nPlayStatus       = eDM_CD_PLAY_COND_HALTING;
        m_bCDRomTrack       = false;
        m_bCDTextExist      = false;
        m_bDTSCD            = false;
        m_nRandomMode       = eDM_CD_RANDOM_NONE;
        m_nRepeatMode       = eDM_CD_REPEAT_NONE;
        m_nScanMode         = eDM_CD_SCAN_NONE;
        m_nDiscNo           = 0;
        m_nTrackNo          = 0;
        m_bLFE              = false;
        m_nChannel          = eDM_CD_CHANNEL_INVALID;
        m_nPlaybackTime     = 0;
        m_nInterval         = 0;
        m_nAudioOutput      = eDM_CD_AUDIO_OUTPUT_ANALOG_2CH;
        m_nDisplayScreen    = eDM_DISPLAY_SCREEN_PLAY;
        m_bSCMSDetect       = false;
        m_bJumpDetect       = false;
        m_bPlaySpeed        = false;
        m_bEmphasisExist    = false;
        m_nDirectStopMode   = eDM_DIRECT_STOP_OFF;
    }

    DECK_CD_PLAY_COND           m_nPlayStatus;
    int                         m_bCDRomTrack;
    int                         m_bCDTextExist;
    int                         m_bDTSCD;
    DECK_CD_RANDOM_MODE         m_nRandomMode;
    DECK_CD_REPEAT_MODE         m_nRepeatMode;
    DECK_CD_SCAN_MODE           m_nScanMode;
    int                         m_nDiscNo;
    int                         m_nTrackNo;
    int                         m_bLFE;
    DECK_CD_CHANNEL_MODE        m_nChannel;
    int                         m_nPlaybackTime;
    int                         m_nInterval;
    DECK_CD_AUDIO_OUTPUT_MODE   m_nAudioOutput;
    DECK_DISPLAY_SCREEN_MODE    m_nDisplayScreen;
    int                         m_bSCMSDetect;
    int                         m_bJumpDetect;
    int                         m_bPlaySpeed;   // 특정 속도를 가지고 play중인가.
    int                         m_bEmphasisExist;
    DECK_DIRECT_STOP_MODE       m_nDirectStopMode;
};

class CDDATrackInfo
{
public:
    CDDATrackInfo()
    {
        m_nAddress      = 0;
        m_nLength       = 0;
        m_nMinAddr      = 0;
        m_nSecAddr      = 0;
        m_nFrmAddr      = 0;
        m_nDuration     = 0;
        m_nTrackIndex   = 0;
        m_tPlayTime     = QTime(0,0,0);
    }

    CDDATrackInfo(const CDDATrackInfo &mi)
    {
        m_nAddress      = mi.m_nAddress;
        m_nLength       = mi.m_nLength;
        m_nMinAddr      = mi.m_nMinAddr;
        m_nSecAddr      = mi.m_nSecAddr;
        m_nFrmAddr      = mi.m_nFrmAddr;
        m_nDuration     = mi.m_nDuration;
        m_nTrackIndex   = mi.m_nTrackIndex;
        m_tPlayTime     = mi.m_tPlayTime;
        m_strTitle      = mi.m_strTitle;
        m_strPlayer     = mi.m_strPlayer;
        m_strLyricist   = mi.m_strLyricist;
        m_strComposer   = mi.m_strComposer;
        m_strArranger   = mi.m_strArranger;
    }

    UINT4       m_nAddress;
    UINT4       m_nLength;

    UINT4       m_nMinAddr;
    UINT4       m_nSecAddr;
    UINT4       m_nFrmAddr;

    UINT4       m_nDuration;

    UINT4       m_nTrackIndex;
    
    QTime       m_tPlayTime;
    QString     m_strTitle;
    QString     m_strPlayer;
    QString     m_strLyricist;
    QString     m_strComposer;
    QString     m_strArranger;
};

class CDDAManager : public QObject, public ICDInterface, public IMediaInterface, public IAudioInterface
{
    Q_OBJECT
public:
    static CDDAManager* GetInstance();
    static void         DeleteInstance();
    
    virtual        ~CDDAManager();

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

    void            DiscTestPlay(); //added by wonseok.heo for in out test

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
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    void            HandleRetryGNTocInfo();
    void            HandleRetryTocInfo();
    void            HandleRetryToc2Info();
    void            HandleRetryCDTextInfo();
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
    void            OnFileNameReceived(DvdcFileName *pFileName);
    void            OnFolderNameReceived(DvdcFolderName *pFolderName);
    void            OnGraceNoteTOCReceived(int nSize, void *pTocData);

private:
    CDDAManager(QObject *parent = 0);
    void            RandomNotice(int nMode);

    ///////////////////////////////////////////////////////////
    //
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
#if _DECK_DIAGNOSTIC_
    bool            ExtractResetStart(StatusData *pPacket);
#endif
#endif // CDDA_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05
    bool            ExtractCDDAStatus(StatusData *pPacket);
    bool            ExtractTOCStatus(StatusData *pPacket);
    int             ExtractTOC2Status(StatusData *pPacket);
    bool            ExtractCDTextStatus(StatusData *pPacket);
    bool            ExtractCDTextInfoStatus(StatusData *pPacket);
    bool            ExtractReqError(StatusData *pPacket);

    int             GetRandomMode(DECK_CD_RANDOM_MODE eMode);
    int             GetRepeatMode(DECK_CD_REPEAT_MODE eMode);
    int             GetScanMode(DECK_CD_SCAN_MODE eMode);
    UINT4           AddressToSectors(int nMinAddr, int nSecAddr, int nFrameAddr);
    void            EmitError(int nError);
    // { commented by cychoi 2013.07.13 for MLT spec out
    //bool            IsMLTListMode();
    //int             GetMLTTrackIndex(int nTrackNum);
    // } commented by cychoi 2013.07.13

    // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
    int             GetNextTrackIdx(int nSkiplevel, int moveTrackIndex);
    int             GetPrevTrackIdx(int nSkiplevel, int moveTrackIndex);
    // } modified by cychoi 2013-07-10
    int             GetRandomTrackIdx(); // added by cychoi 2015-05-19 for QE_Smoke Test Ver_45버전(130519) 37 fail

    bool            CheckDiscValid(); // added by cychoi 20103.10.15 for ITS 167582 Invalid CDDA+VCD Mixed CD handling

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    int             InitRetryGNTocInfo();
    int             StartRetryGNTocInfoTimer(int ms); // modified by cychoi 2013.09.06 for ITS 178797
    int             InitRetryTocInfo();
    int             StartRetryTocInfoTimer(int ms); // modified by cychoi 2013.09.06 for ITS 178797
    int             InitRetryToc2Info();
    int             StartRetryToc2InfoTimer(int ms); // modified by cychoi 2013.09.06 for ITS 178797
    int             InitRetryCDTextInfo();
    int             StartRetryCDTextInfoTimer(int ms); // modified by cychoi 2013.09.06 for ITS 178797
    int             InitRetryPlayback();
    int             InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
    int             StartRetryPlaybackTimer(int ms);
#endif
// } added by cychoi 2013-06-05
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
    bool            HandleErrorDisc(bool bRetryToPlay);
#endif

    int             GetInternalMediaState(); // added by cychoi 2013.12.26 for ITS 217306 DVD black
    void            EmitTrackChange(int nTrackNo, bool bTrackChanged); // added by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050

    ///////////////////////////////////////////////////////////
    //
    static CDDAManager      *m_pInstance;

    int                      m_bLoadComplete;

    Packet                  *m_pPacket;
    CDDAManagerStatus        m_tStatus;

    int                      m_nStartTrackNo;   // 1
    int                      m_nLastTrackNo;    // max track number.
    int                      m_nTotalTrackNum;
    int                      m_nPlayTrackNo;    // m_nStartTrackNo ~ m_nLastTrackNo
    int                      m_nCurrTrackIndex;
    int                      m_nNextTrackNo; // added by cychoi 20103.08.14 for ITS 184423

    int                      m_nTotalPlayTimeFrm;
    int                      m_nTotalPlayTimeMin;
    int                      m_nTotalPlayTimeSec;

    int                      m_nTotalBlockNum;
    int                      m_nCurrBlockNo;
    int                      m_nUserBlockNo; // added by cychoi 2013.09.06 for ITS 178797

    QList<CDDATrackInfo>     m_tTrackList;
    //QList<CDDATrackInfo>     m_tBackupList; // commented by cychoi 2014.01.24 for MLT spec out
    //QString                  m_strAlbumTitle; // commented by cychoi 2014.01.24 for unused var

    MEDIA_PLAYER_STATE       m_ePlayerState;
    MEDIA_PLAYER_STATE       m_eLastPlayerState; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
    int                      m_nSystemState;

    int                      m_nUserRandomMode;
    int                      m_nUserRepeatMode;
    // { modified by cychoi 2013.11.15 for ITS 205008
    int                      m_nDelayedRandomMode;
    int                      m_nDelayedRepeatMode;
    int                      m_bDelayedModeSet;
    // } modified by cychoi 2013.11.15
    // { modified by cychoi 2013.11.18 for ITS 209699
    int                      m_nDelayedScanMode;
    int                      m_bDelayedScanModeSet;
    // } modified by cychoi 2013.11.18
    int                      m_nFFREWRate;
    int                      m_nFFREWState; // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error  // added by cychoi 2013-06-07 for ITS 172685
    int                      m_bREWMode;
    bool                     m_bFFREWStateSigSend; // added by sjhyun 2013.09.14 for ITS 189282
    int                      m_nUserTimeSearch;

    // FOR APP
    // { modified by cychoi 2013.09.06 for ITS 178797
    CDDADeckReqState         m_eAppTocFlag;
    CDDADeckReqState         m_eAppToc2Flag;
    CDDADeckReqState         m_eAppCDTextFlag;
    CDDADeckReqState         m_eAppCDTextInfoFlag;
    // } modified by cychoi 2013.09.06
    int                      m_nAppTrackNo;
    int                      m_nAppDataType;
    int                      m_nAppBlockNo;
    int                      m_nAppFrameNo;
    CDDADeckReqState         m_eAppGNTocFlag; // modified by cychoi 2013.09.06 for ITS 178797

    // { modified by cychoi 2013.09.06 for ITS 178797
    CDDADeckReqState         m_eTocFlag;
    CDDADeckReqState         m_eToc2Flag;
    CDDADeckReqState         m_eCDTextInfoFlag;
    // } modified by cychoi 2013.09.06
    
    DvdcTOC                  m_tToc;
    DvdcTOC2                 m_tToc2;
    DvdcCdTextInfo           m_tCDTextInfo;
    DvdcCDText               m_tCDText;

    int                      m_nGNTocSize;
    int                      m_pGNTocData[200];

    //int                      m_bFirstBoot; // commented by cychoi 20130.07.11 for keeping playback options
    int                      m_bFirstPlay;

    int                      m_bControllerChanged;
    //int                      m_bSearchingMode; // commented by cychoi 20103.06.11 for ITS 173068

    QList<int>               m_tUserReqTrackList;

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    QTimer                   m_tRetryGNTocInfoTimer;
    QTimer                   m_tRetryTocInfoTimer;
    QTimer                   m_tRetryToc2InfoTimer;
    QTimer                   m_tRetryCDTextInfoTimer;
    QTimer                   m_tRetryPlaybackTimer;
    int                      m_nRetryTestPlayback;
    MEDIA_PLAYER_STATE       m_nRetryPlaybackMediaState;
    DECK_CD_RANDOM_MODE      m_nRetryPlaybackRandomMode;
    DECK_CD_REPEAT_MODE      m_nRetryPlaybackRepeatMode;
    DECK_CD_SCAN_MODE        m_nRetryPlaybackScanMode;
#endif
// } added by cychoi 2013-06-05

#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
    int                      m_nDiscDetectRetryCount;
    int                      m_nDiscPlayRetryCount;
#endif

    int                      m_bAutoTrackChanged; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    int                      m_bUserTrackChanged; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    // { modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050 // { commented by cychoi 2013.07.13 for MLT spec out
    CDDAPlayDirection        m_ePlayDirection; // added by cychoi 20103.07.09 for ITS 176771 & 173068 MLT play error
    // } modified by cychoi 2014.08.29 // } commented by cychoi 2013.07.13

    int                      m_bBTCallStart; // added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
};

#endif // CDDAMANAGER_H
