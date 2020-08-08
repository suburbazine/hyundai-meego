#ifndef DVDMANAGER_H
#define DVDMANAGER_H

#include <QObject>
#include <QTime>
#include <QMutex>
#include <QTimer> // added by cychoi 2013-06-05 To avoid CDDA playback stop error.
#include "imediainterface.h"
#include "ivideointerface.h"
#include "icdinterface.h"
#include "packet.h"
#include "deck_def.h"

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
// 1. DVD vr info read error.
// 2. DVD playback stop error.
#define DVD_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05
#define DVD_DECK_REQ_RETRY_LIMITATION_ENABLED // added by cychoi 2013.11.25 for enabling to handle error disc on detection timeout

// { added by cychoi 2013.06.30 for ITS 162637 DVD navigation
// The same UX as FS, KH and Portable DVD Player.
#define DVD_NEW_NAVIGATION_ENABLED
// } added by cychoi 2013.06.30

// { modified by cychoi 2013.11.13 for No support DVD Navigation Still Picture because Stil Picture Check API is not suported from DECK.
// Deck only support Still Off Notice (Activation： Finish the current still image and move to the next still image or the next moving image when playing the still image of DVD-VR)
// added by cychoi 20103.11.11 for DVD Navigation Still Picture
// added by cychoi 20103.09.26 for ITS 188493 DVD Navigation Still Picture
// added by cychoi 20103.10.02 for ITS 188493 DVD Navigation Still Picture -  Handle Play state as Pause state
// added by cychoi 20103.10.08 for SmokeTest Invalid Control Cue Icon Display
// added by cychoi 20103.10.15 for ITS 195479 Next should act as EnterKey if Still Picture
// added by cychoi 2013.10.25 for ITS 195754
#undef DVD_NAVIGATION_STILL_PICTURE_ENABLED
#define DVD_NAVIGATION_STILL_PICTURE_RETRY_DISABLED
// } modified by cychoi 2013.11.13

// { added by cychoi 2013.09.06 for ITS 178797
typedef enum
{
    DVD_DECK_REQ_NONE, // No request
    DVD_DECK_REQ_SENT, // Request sent
    DVD_DECK_REQ_DONE  // Response received
}DVDDeckReqState;
// } added by cychoi 2013.09.06


class DVDManagerStatus
{
public:
    DVDManagerStatus()
    {
        m_nPlayStatus       = eDM_CD_PLAY_COND_HALTING;
        m_nRandomMode       = eDM_DVD_RANDOM_NONE;
        m_nRepeatMode       = eDM_DVD_REPEAT_NONE;
        m_nScanMode         = eDM_DVD_SCAN_NONE;
        m_nGrpTitlePGPLNo   = 0xFF;
        m_nTrackChapterNo   = 0xFFF;
		m_nTotalGrpTlePGPLNo= -1; //added by jeeeun for not entering List 
        // { added by cychoi 2013-07-15 for Prevent fix
        m_bPictureExist     = 0;
        m_nPGPL             = 0;
        m_nContent          = eDM_DVD_VIDEO_CONTENT;
        m_nDiscNo           = 0xFF;
        m_nPlaybackTime     = 0;
        m_bMultiAngleOff    = 0;
        m_nSubTitleNo       = 0xFF; // modified by yungi 2013.08.26 for ITS 0186197
        m_nTotalSubTitleNo  = 0;
        m_nSubTitleCode     = 0;
        m_nAudioNo          = 0xFF; // added by cychoi 2013.07.27 for ITS 181112 & 181175 Audio Language wrong display
        m_nTotalAudioNo     = 0;
        m_nAudioCode        = 0;
        m_bDownMixForbidden = 0;
        m_nAudioAttr        = eDM_DVD_AUDIO_ATTR_NOT_APPLICABLE;
        m_nChannel          = eDM_CD_CHANNEL_INVALID;
        m_bHighLight        = 0;
        m_bDolbySurround    = 0;
        m_bLFEExist         = 0;
        m_nFrequency        = eDM_DVD_QUANTI_FREQUENCY_44KHZ;
        m_nResolution       = eDM_DVD_QUANTI_RESOLUTION_16BIT;
        m_nAngleNo          = 0xFF; // modified by yungi 2013.08.26 for ITS 0186197
        m_nTotalAngleNo     = 0;
        m_nAudioOutput      = eDM_VCD_AUDIO_OUTPUT_L_R;
        m_nDisplayScreen    = eDM_DISPLAY_SCREEN_PLAY;
        m_nTotalChapterNo   = 0;
        // } added by cychoi 2013-07-15
    }

    DECK_CD_PLAY_COND           m_nPlayStatus;
    int                         m_bPictureExist;
    int                         m_nPGPL;
    DECK_DVD_CONTENT            m_nContent;
    DECK_DVD_RANDOM_MODE        m_nRandomMode;
    DECK_DVD_REPEAT_MODE        m_nRepeatMode;
    DECK_DVD_SCAN_MODE          m_nScanMode;
    int                         m_nDiscNo;
    int                         m_nGrpTitlePGPLNo;  // Group/Title/PGPL No.
    int                         m_nTrackChapterNo;
    int                         m_nPlaybackTime;
    int                         m_bMultiAngleOff;  
    int                         m_nSubTitleNo;
    int                         m_nTotalSubTitleNo;
    int                         m_nSubTitleCode;
    int                         m_nAudioNo;
    int                         m_nTotalAudioNo;
    int                         m_nAudioCode;
    int                         m_bDownMixForbidden;
    DECK_DVD_AUDIO_ATTRIBUTE    m_nAudioAttr;
    DECK_CD_CHANNEL_MODE        m_nChannel;
    int                         m_bHighLight;
    int                         m_bDolbySurround;
    int                         m_bLFEExist;
    DECK_DVD_QUANTI_FREQUENCY   m_nFrequency;
    DECK_DVD_QUANTI_RESOLUTION  m_nResolution;
    int                         m_nAngleNo;
    int                         m_nTotalAngleNo;
    DECK_VCD_AUDIO_OUTPUT_MODE  m_nAudioOutput;                             
    DECK_DISPLAY_SCREEN_MODE    m_nDisplayScreen;
    int                         m_nTotalChapterNo;
    int                         m_nTotalGrpTlePGPLNo;
};

class AudioTask
{
public:
    AudioTask(int nAudioCode = -1, int nAudioNo = -1) 
    {
        m_nAudioCode = nAudioCode;
        m_nAudioNo   = nAudioNo;
    }
    
    AudioTask(const AudioTask &rhs)
    {
        m_nAudioCode = rhs.m_nAudioCode;
        m_nAudioNo   = rhs.m_nAudioNo;
    }
    
    int m_nAudioCode;
    int m_nAudioNo;
};

class TitleChapterTask
{
public:
    TitleChapterTask(uint nTitleNo = 1, uint nChapterNo = 1) 
    {
        m_nTitleNo      = nTitleNo;
        m_nChapterNo    = nChapterNo;
        m_bRequestFStop = 0;
    }
    
    TitleChapterTask(const TitleChapterTask &rhs)
    {
        m_nTitleNo      = rhs.m_nTitleNo;
        m_nChapterNo    = rhs.m_nChapterNo;
        m_bRequestFStop = rhs.m_bRequestFStop;
    }
    
    int m_nTitleNo;
    int m_nChapterNo;  
    int m_bRequestFStop;
};

class DVDManager : public QObject, public ICDInterface, public IMediaInterface, public IVideoInterface
{
    Q_OBJECT
public:
    static DVDManager*  GetInstance();
    static void         DeleteInstance();
    
    virtual        ~DVDManager();

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
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    void            HandleRetryVRInfo();
    void            HandleRetryPlayback();
#endif
// } added by cychoi 2013-06-05
#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
    void            HandleStillPicture();
#endif

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
    void            OnTitleGroupChanged(int nTitleGrpIndex);
    void            OnTotalSubtitleChanged(int nSubtitleNum);
    void            OnTotalAudioChanged(int nAudioNum);
    void            OnTotalAngleChanged(int nAngleNum);
    void            OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize);
    void            OnNormalPlay(); // added by cychoi 2013-05-24 Added additional signal for FF/Rew functionality.
    void            OnRealSoundChanged(); // added by cychoi 20103.06.11
    void            OnUserTrackChanged(int nTrackIndex); // added by cychoi 20103.07.19 for add new signal
    //void            OnAutoTrackChanged(int nTrackIndex); // added by cychoi 2014.04.06 for ITS 233500
    
    ///////////////////////////////////////////////////////////
    // FOR IVideoInterface
    void            OnTotalAudioLangReceived(int *pAudioCodeList, int *pAudioAttrList, int *pAudioChnlList, int nAudioCodeNum);
    void            OnTotalSubtitleLangReceived(int *pSubtitleCodeList, int nSubtitleCodeNum);
    void            OnAudioLangChanged(int nAudioNum);
    void            OnSubtitleLangChanged(int nSubtitleCode);
    void            OnAngleChanged(int nAngleNum);
    void            OnSearchCompleted(int nTitleNo, int nChapterNo, int nChapterSize);
    void            OnDVDInitCompleted(int bSuccess);
    
private:
    DVDManager(QObject *parent = 0);
    void            RandomNotice(int nMode);

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
#if _DECK_DIAGNOSTIC_
    bool            ExtractResetStart(StatusData *pPacket);
#endif
#endif // DVD_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05
    bool            ExtractDVDStatus(StatusData *pPacket);
    bool            ExtractParentalChangeRequest(StatusData *pPacket);
    bool            ExtractDirectTouch(StatusData *pPacket);
    bool            ExtractReqError(StatusData *pPacket);

    int             InitDVDSetting();
    int             AudioLangSearchMode();
    int             SetAudioLangMode();
    int             SubtitleLangSearchMode();
    int             SetSubtitleLangMode();
    int             SetAngleNumberMode();
    int             SearchTitleChapterMode();
    int             SearchTitleChapter_Impl(int nTitle, int nChapter);
    int             SetSubtitleLanguage_Impl(int nSubtitleCode);
    int             SetAudioLanguage_Impl(int nAudioCode, int nIndex);
    int             SetAngle_Impl(int nAngleNumber);
    
    void            EmitOnTotalAudioLangReceived();
    void            EmitOnTotalSubtitleLangReceived();
    void            EmitOnAudioLangChanged(int nAudioNo);
    void            EmitOnSubtitleLangChanged(int nSubTitleCode);
    void            EmitOnAngleChanged(int nAngleNo);

    int             GetRandomMode(DECK_DVD_RANDOM_MODE eMode);
    int             GetRepeatMode(DECK_DVD_REPEAT_MODE eMode);
    int             GetScanMode(DECK_DVD_SCAN_MODE eMode);
    void            EmitError(int nError);

    void            ProcessErrResponse();
    void            RequestDVDStatus();

    // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
    int             GetNextTrackIdx(int nSkiplevel, int moveTrackIndex);
    int             GetPrevTrackIdx(int nSkiplevel, int moveTrackIndex);
    // } modified by cychoi 2013-07-10
    
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    int             InitRetryVRInfo();
    int             StartRetryVRInfoTimer(int ms); // modified by cychoi 2013.09.06 for ITS 178797
    int             InitRetryPlayback();
    int             InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
    int             StartRetryPlaybackTimer(int ms);
#endif
// } added by cychoi 2013-06-05

#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
    int             InitStillPictureTimer();
    int             StartStillPictureTimer();
    bool            EnterKeyOnStillPicture();
#endif
    void            ContinueSwitchAudio(); // added by cychoi 2013.10.25 for ITS 195688 Audio repeated request

#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
    bool            HandleErrorDisc(bool bRetryToPlay);
#endif

    int             GetInternalMediaState(); // added by cychoi 2013.12.26 for ITS 217306 DVD black

    static DVDManager       *m_pInstance;

    int                      m_bLoadComplete;

    Packet                  *m_pPacket;
    DVDManagerStatus         m_tStatus;

    int                      m_nPlayTrackNo;
    int                      m_nCurrTrackIndex;

    MEDIA_PLAYER_STATE       m_ePlayerState;
    MEDIA_PLAYER_STATE       m_eLastPlayerState; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
    int                      m_nSystemState;

#ifdef DVD_NAVIGATION_STILL_PICTURE_RETRY_DISABLED
    int                      m_nStartPlaybackTime;
    bool                     m_bEnterKey;
#endif
#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
    QTimer                   m_tStillPictureTimer;
    int                      m_nStillPictureState;
#endif

    int                      m_nUserRandomMode;
    int                      m_nUserRepeatMode;
    int                      m_nFFREWRate;
    int                      m_nFFREWState; // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error  // added by cychoi 2013-06-07 for ITS 172685
    int                      m_bREWMode;
    bool                     m_bFFREWStateSigSend; // added by sjhyun 2013.09.14 for ITS 189282

    int                      m_bSubTitleSearchMode;
    int                      m_bAudioSearchMode;
    int                      m_nAngleNumber;
    int                      m_nSubTitleCodeNum;
    int                      m_nAudioCodeNum;
    int                      m_pSubTitleCodeList[100];
    int                      m_pAudioCodeList[100];
    int                      m_pAudioAttrList[100];
    int                      m_pAudioChnlList[100];

    int                      m_bPauseAtSetting;
    QList<AudioTask>         m_tAudioTask;
    QList<int>               m_tSubtitleTask;
    QList<int>               m_tAngleTask;
    QList<TitleChapterTask>  m_tSearchTask;
    QMutex                   m_tAudioMutex;
    QMutex                   m_tSubtitleMutex;
    QMutex                   m_tAngleMutex;
    QMutex                   m_tSearchMutex;
    int                      m_nPrevAudioLangNo;
    int                      m_nPrevSubtitleLangNo;
    int                      m_nPrevAngleNo;
    int                      m_nUserTimeSearch;

    int                      m_bDVDSettingMode;
    int                      m_nDVDSettings;
    DVDSettings              m_tDVDSettings;

    DVDDeckReqState          m_eVRInfoFlag; // added by cychoi 2013.09.06 for ITS 178797

    //int                      m_bFirstBoot; // commented by cychoi 2013.07.11 for keeping playback options
    int                      m_bFirstPlay;

    int                      m_bControllerChanged;
    // { commented by cychoi 2013.10.30 for ITS 204969
    //int                      m_bTopMenu;        // added by jeeeun for SANITY_CM_AG127 disc menu fail    
    //int                      m_titleSetting;  // added by jeeeun for SANITY_CM_AG127 disc menu fail
    // } commented by cychoi 2013.10.30

// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifndef DVD_NEW_NAVIGATION_ENABLED
    QList<int>               m_tUserReqTrackList;
#endif
// } modified by cychoi 2013.06.30

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    QTimer                   m_tRetryVRInfoTimer;
    QTimer                   m_tRetryPlaybackTimer;
    MEDIA_PLAYER_STATE       m_nRetryPlaybackMediaState;
    DECK_DVD_RANDOM_MODE     m_nRetryPlaybackRandomMode;
    DECK_DVD_REPEAT_MODE     m_nRetryPlaybackRepeatMode;
    DECK_DVD_SCAN_MODE       m_nRetryPlaybackScanMode;
#endif
// } added by cychoi 2013-06-05

#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
    int                      m_nDiscDetectRetryCount;
    int                      m_nDiscPlayRetryCount;
#endif

    int                      m_bAutoTrackChanged; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    int                      m_bUserTrackChanged; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    int                      m_bAudioSwitched; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
    int                      m_bRealSoundChanged; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute

    int                      m_nReqSwitchAudio; // added by cychoi 2013.10.25 for ITS 195688 Audio repeated request

    int                      m_bBTCallStart; // added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
};

#endif // DVDMANAGER_H
