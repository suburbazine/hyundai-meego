#ifndef VIDEOCDMANAGER_H
#define VIDEOCDMANAGER_H

#include <QObject>
#include <QTime>
#include <QTimer> // added by cychoi 2013-06-05 To avoid CDDA playback stop error.
#include "imediainterface.h"
#include "ivideointerface.h"
#include "icdinterface.h"
#include "packet.h"
#include "deck_def.h"

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
// 1. VCD toc info read error.
// 2. VCD toc2 info read error.
// 3. VCD playback stop error.
#define VCD_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05
#define VCD_DECK_REQ_RETRY_LIMITATION_ENABLED // added by cychoi 2013.11.25 for enabling to handle error disc on detection timeout

// { added by cychoi 2013.09.06 for ITS 178797
typedef enum
{
    VCD_DECK_REQ_NONE, // No request
    VCD_DECK_REQ_SENT, // Request sent
    VCD_DECK_REQ_DONE  // Response received
}VCDDeckReqState;
// } added by cychoi 2013.09.06

class VideoCDManagerStatus
{
public:
    VideoCDManagerStatus()
    {
        m_nPlayStatus       = eDM_CD_PLAY_COND_HALTING;
        m_nRandomMode       = eDM_CD_RANDOM_NONE;
        m_nRepeatMode       = eDM_CD_REPEAT_NONE;
        m_nScanMode         = eDM_CD_SCAN_NONE;
        // { added by cychoi 2013-07-15 for Prevent fix
        m_nVersion          = eDM_VIDEO_CD_VER_1_1;
        m_nPBCOff           = 1;
        m_nDiscNo           = 0xFF;
        m_nTrackNo          = 0xFF;
        m_nPlaybackTime     = 0;
        m_nAudioOutput      = eDM_VCD_AUDIO_OUTPUT_L_R;
        m_nDisplayScreen    = eDM_DISPLAY_SCREEN_PLAY;
        m_bExistEmphasis    = 0;
        // } added by cychoi 2013-07-15
    }

    DECK_CD_PLAY_COND           m_nPlayStatus;
    DECK_VIDEO_CD_VER           m_nVersion;
    int                         m_nPBCOff;
    DECK_CD_RANDOM_MODE         m_nRandomMode;
    DECK_CD_REPEAT_MODE         m_nRepeatMode;
    DECK_CD_SCAN_MODE           m_nScanMode;
    int                         m_nDiscNo;
    int                         m_nTrackNo;
    int                         m_nPlaybackTime;
    DECK_VCD_AUDIO_OUTPUT_MODE  m_nAudioOutput;
    DECK_DISPLAY_SCREEN_MODE    m_nDisplayScreen;
    int                         m_bExistEmphasis;
};

class VCDTrackInfo
{
public:
    VCDTrackInfo()
    {
        m_nAddress  = 0;
        m_nLength   = 0;
        m_nMinAddr  = 0;
        m_nSecAddr  = 0;
        m_nFrmAddr  = 0;
        m_nDuration = 0;
        m_tPlayTime = QTime(0,0,0);
    }

    VCDTrackInfo(const VCDTrackInfo &mi)
    {
        m_nAddress      = mi.m_nAddress;
        m_nLength       = mi.m_nLength;
        m_nMinAddr      = mi.m_nMinAddr;
        m_nSecAddr      = mi.m_nSecAddr;
        m_nFrmAddr      = mi.m_nFrmAddr;
        m_nDuration     = mi.m_nDuration;
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
    
    QTime       m_tPlayTime;
    QString     m_strTitle;
    QString     m_strPlayer;
    QString     m_strLyricist;
    QString     m_strComposer;
    QString     m_strArranger;
};

class VideoCDManager : public QObject, public ICDInterface, public IMediaInterface, public IVideoInterface
{
    Q_OBJECT
public:
    static VideoCDManager*  GetInstance();
    static void             DeleteInstance();
    
    virtual        ~VideoCDManager();

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
    void            SetBrightness(double fBrightness);
    void            SetContrast(double fContrast);
    void            SetTint(double fTint);
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
    // FOR IVideoInteface    
    void            FrameAdvance();
    void            FrameBack();
    void            SubTitle(uint mode);
    void            Load();
    void            Eject(uint mode);
    void            SlowForward(uint nMode);
    void            SlowRewind(uint nMode);
    void            PageUp();
    void            PageDown();
    void            Titlemenu(uint nSetting);
    void            EnterKey();
    void            ReturnKey();
    void            CursorKey(uint direction);
    void            TopMenu(uint setting);
    void            SwitchSubTitle();  
    void            SwitchAngle();  
    void            SwitchAudio(); 
    void            Search();    
    void            InitializeDeckPrioritySettings(DeckPrioritySettings *pSettings);
    void            InitializeDVDSettings(DVDSettings *pSettings);
    int             RequestTOC();
    int             RequestTOC2(int nBlockNo);
    void            TenKey(int nKeyID, int pressStatus);
    void            DualMonoChange();
    int             GetVCDStatus(VCDStatusResponse *pResponse);
    int             GetDVDStatus(DVDStatusResponse *pResponse);
    int             GetDVDPrioritySettings(DeckPrioritySettings *pSettings);
    int             RequestAudioLanguages();
    void            SetAudioLanguage(int nAudioCode, int nIndex);
    int             RequestSubtitleLanguages();
    void            SetSubtitleLanguage(int nSubtitleCode);
    void            SetAngle(int nAngleNumber);
    void            SetPBC(uint nMode);
    void            SetOSD(uint nMode);
    void            SearchTitleChapter(uint nTitle, uint nChapter);
    void            CancelAudioLangRequest();
    void            CancelSubtitleLangRequest();
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
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    void            HandleRetryTocInfo();
    void            HandleRetryToc2Info();
    void            HandleRetryPlayback();
#endif
// } added by cychoi 2013-06-05

signals:
    ///////////////////////////////////////////////////////////
    // FOR IMediaInterface
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
    void            OnNormalPlay(); // added by cychoi 2013-05-24 Added additional signal for FF/Rew functionality.
    void            OnRealSoundChanged(); // added by cychoi 20103.06.11
    void            OnUserTrackChanged(int nTrackIndex); // added by cychoi 20103.07.19 for add new signal
    //void            OnAutoTrackChanged(int nTrackIndex); // added by cychoi 2014.04.06 for ITS 233500

    ///////////////////////////////////////////////////////////
    // FOR IVideoInterface
    void            OnTOCReceived(DvdcTOC *pToc);
    void            OnTOC2Received(DvdcTOC2 *pToc2);

private:
    VideoCDManager(QObject *parent = 0);
    void            RandomNotice(int nMode);

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
#if _DECK_DIAGNOSTIC_
    bool            ExtractResetStart(StatusData *pPacket);
#endif
#endif // VCD_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05
    bool            ExtractVideoCDStatus(StatusData *pPacket);
    bool            ExtractTOCStatus(StatusData *pPacket);
    int             ExtractTOC2Status(StatusData *pPacket);
    bool            ExtractReqError(StatusData *pPacket);

    int             GetRandomMode(DECK_CD_RANDOM_MODE eMode);
    int             GetRepeatMode(DECK_CD_REPEAT_MODE eMode);
    int             GetScanMode(DECK_CD_SCAN_MODE eMode);
    UINT4           AddressToSectors(int nMinAddr, int nSecAddr, int nFrameAddr);
    void            EmitError(int nError);
    // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
    int             GetNextTrackIdx(int nSkiplevel, int moveTrackIndex);
    int             GetPrevTrackIdx(int nSkiplevel, int moveTrackIndex);
    // } modified by cychoi 2013-07-10

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    int             InitRetryTocInfo();
    int             StartRetryTocInfoTimer(int ms); // modified by cychoi 2013.09.06 for ITS 178797
    int             InitRetryToc2Info();
    int             StartRetryToc2InfoTimer(int ms); // modified by cychoi 2013.09.06 for ITS 178797
    int             InitRetryPlayback();
    int             InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
    int             StartRetryPlaybackTimer(int ms);
#endif
// } added by cychoi 2013-06-05

#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
    bool            HandleErrorDisc(bool bRetryToPlay);
#endif

    int             GetInternalMediaState(); // added by cychoi 2013.12.26 for ITS 217306 DVD black

    static VideoCDManager   *m_pInstance;

    int                      m_bLoadComplete;

    Packet                  *m_pPacket;
    VideoCDManagerStatus     m_tStatus;

    int                      m_nStartTrackNo;   // 1
    int                      m_nLastTrackNo;    // max track number.
    int                      m_nTotalTrackNum;  // 전체 트랙 수.
    int                      m_nPlayTrackNo;    // 현재 재생중인 트랙 번호.
    int                      m_nCurrTrackIndex; // Play시 재생할 트랙 인덱스. m_nStartTrackNo를 더해서 사용해야 함.

    int                      m_nTotalPlayTimeFrm;
    int                      m_nTotalPlayTimeMin;
    int                      m_nTotalPlayTimeSec;

    int                      m_nTotalBlockNum;
    int                      m_nCurrBlockNo;
    int                      m_nUserBlockNo; // added by cychoi 2013.09.03 for ITS 187102 AVP died

    QList<VCDTrackInfo>      m_tTrackList;
    //QString                  m_strAlbumTitle; // commented by cychoi 2014.01.24 for unused var

    MEDIA_PLAYER_STATE       m_ePlayerState;
    MEDIA_PLAYER_STATE       m_eLastPlayerState; // added by cychoi 2013.09.13 for ITS 185685 Infinite VCD Loading Screen
    int                      m_nSystemState;

    int                      m_nUserRandomMode;
    int                      m_nUserRepeatMode;
    // { modified by cychoi 2013.11.15 for ITS 205008
    int                      m_nDelayedRandomMode;
    int                      m_nDelayedRepeatMode;
    int                      m_bDelayedModeSet;
    // } modified by cychoi 2013.11.15
    int                      m_nFFREWRate;
    int                      m_nFFREWState; // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error  // added by cychoi 2013-06-07 for ITS 172685
    int                      m_bREWMode;
    bool                     m_bFFREWStateSigSend; // added by sjhyun 2013.09.14 for ITS 189282

    // FOR APP
    // { modified by cychoi 2013.09.06 for ITS 178797
    VCDDeckReqState          m_eAppTocFlag;
    VCDDeckReqState          m_eAppToc2Flag;
    // } modified by cychoi 2013.09.06
    DvdcTOC                  m_tToc;
    DvdcTOC2                 m_tToc2;

    // { added by cychoi 2013.09.06 for ITS 178797
    VCDDeckReqState          m_eTocFlag;
    VCDDeckReqState          m_eToc2Flag;
    // } added by cychoi 2013.09.06

    //int                      m_bFirstBoot; // commented by cychoi 20130.07.11 for keeping playback options
    int                      m_bFirstPlay;

    int                      m_bControllerChanged;
    QList<int>               m_tUserReqTrackList;

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    QTimer                   m_tRetryTocInfoTimer;
    QTimer                   m_tRetryToc2InfoTimer;
    QTimer                   m_tRetryPlaybackTimer;
    MEDIA_PLAYER_STATE       m_nRetryPlaybackMediaState;
    DECK_CD_RANDOM_MODE      m_nRetryPlaybackRandomMode;
    DECK_CD_REPEAT_MODE      m_nRetryPlaybackRepeatMode;
    DECK_CD_SCAN_MODE        m_nRetryPlaybackScanMode;
#endif
// } added by cychoi 2013-06-05

#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
    int                      m_nDiscDetectRetryCount;
    int                      m_nDiscPlayRetryCount;
#endif

    int                      m_bAutoTrackChanged; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    int                      m_bUserTrackChanged; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    int                      m_bRealSoundChanged; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute

    int                      m_bBTCallStart; // added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
};

#endif // VIDEOCDMANAGER_H 
