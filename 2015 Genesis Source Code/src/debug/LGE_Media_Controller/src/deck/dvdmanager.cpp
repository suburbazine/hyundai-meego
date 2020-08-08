#include "dvdmanager.h"
#include "deckmanager.h"
#include "util.h"
#include "mediamanager.h"
#include "iplayerengine.h"

#define DVD_DURATION_VALUE          0
#define DVD_CHECK_PREV_SETUP_COUNT  3

enum DVD_SETTING_STEP
{
    DSS_START_STEP,
    DSS_SUBTITLE_START,
    DSS_SUBTITLE_END,
    DSS_AUDIO_START,
    DSS_AUDIO_END,
    DSS_ANGLE_START,
    DSS_ANGLE_END,
    DSS_COMPLETE
};

DVDManager* DVDManager::m_pInstance = 0;

DVDManager* DVDManager::GetInstance()
{
    if(m_pInstance == 0) {
        m_pInstance = new DVDManager();
    }
    
    return m_pInstance;
}

void DVDManager::DeleteInstance()
{
    if(m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

DVDManager::~DVDManager()
{
    MC_LOG << "~DVDManager()" << LOG_ENDL;
}

void DVDManager::Init()
{
    MC_LOG << "DVDManager::Init" << LOG_ENDL;
    
    m_tStatus = DVDManagerStatus();

    m_nPlayTrackNo          = 0;
    m_nCurrTrackIndex       = -1;
    m_ePlayerState          = eMP_STATE_NONE;
    m_eLastPlayerState      = eMP_STATE_NONE; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
#ifdef DVD_NAVIGATION_STILL_PICTURE_RETRY_DISABLED
    m_nStartPlaybackTime    = 0;
    m_bEnterKey             = false;
#endif
    m_bSubTitleSearchMode   = false;
    m_bAudioSearchMode      = false;
    m_bDVDSettingMode       = false;
    m_nDVDSettings          = DSS_START_STEP;
    m_nSubTitleCodeNum      = 0;
    m_nAudioCodeNum         = 0;
    m_bLoadComplete         = false;
    m_nUserRandomMode       = -1;
    m_nUserRepeatMode       = -1;
    m_nUserTimeSearch       = -1;
    m_bPauseAtSetting       = false;
    m_bFirstPlay            = false;
    // { commented by cychoi 2013.10.30 for ITS 204969
    //m_bTopMenu              = false; // added by jeeeun for SANITY_CM_AG127 disc menu fail    
    //m_titleSetting          = -1;    // added by jeeeun for SANITY_CM_AG127 disc menu fail    
    // } commented by cychoi 2013.10.30

    // { added by cychoi 2013-07-15 for Prevent fix
    m_bREWMode              = false;
    m_nAngleNumber          = 0;
    m_nPrevAudioLangNo      = -1;
    m_nPrevSubtitleLangNo   = -1;
    m_nPrevAngleNo          = -1;
    m_tDVDSettings.nSubtitleCode = 0;
    m_tDVDSettings.nAudioCode    = 0;
    m_tDVDSettings.nAudioIndex   = 0;
    m_tDVDSettings.nAngleNumber  = 0;
    // } added by cychoi 2013-07-15
    m_bFFREWStateSigSend    = false; // added by sjhyun 2013.09.14 for ITS 189282

    m_eVRInfoFlag           = DVD_DECK_REQ_NONE; // added by cychoi 2013.09.06 for ITS 178797

    m_tAudioTask.clear();
    m_tSubtitleTask.clear();
    m_tAngleTask.clear();
    m_tSearchTask.clear();
// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifndef DVD_NEW_NAVIGATION_ENABLED
    m_tUserReqTrackList.clear();
#endif
// } modified by cychoi 2013.06.30

    m_nReqSwitchAudio = 0; // added by cychoi 2013.10.25 for ITS 195688 Audio repeated request

    memset(m_pSubTitleCodeList, 0xFF, sizeof(int)*100);
    memset(m_pAudioCodeList, 0xFF, sizeof(int)*100);
    memset(m_pAudioAttrList, 0xFF, sizeof(int)*100);
    memset(m_pAudioChnlList, 0xFF, sizeof(int)*100);

#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
    InitStillPictureTimer();
    m_nStillPictureState = -1;
#endif

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    InitRetryVRInfo();
    InitRetryPlayback();
    InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
    m_nRetryPlaybackMediaState = eMP_STATE_NONE;
#endif
// } added by cychoi 2013-06-05

#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
    m_nDiscPlayRetryCount = 0;
#endif
}

void DVDManager::StartMedia()
{   
    MC_LOG << "DVDManager::StartMedia" << LOG_ENDL;
    
    Init();

    m_ePlayerState = eMP_STATE_NONE;
    m_eLastPlayerState = eMP_STATE_NONE; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State

    m_pPacket->ParentalChangePermission(eDM_PERMIT_NOT_CHANGE);
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    InitRetryVRInfo();
#endif
// } added by cychoi 2013-06-05
    // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
    m_pPacket->RequestDVDAudioVideoVRStatus();
    m_eVRInfoFlag = DVD_DECK_REQ_SENT; // added by cychoi 2013.09.06 for ITS 178797
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    StartRetryVRInfoTimer(200); // modified by cychoi 2013.09.06 for ITS 178797
#endif
// } added by cychoi 2013-06-05
    // } modified by cychoi 2014.04.25
    
#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
    m_nDiscPlayRetryCount = 0;
#endif
}

void DVDManager::EndMedia()
{
}

void DVDManager::ProcessPacket(StatusData *pPacket)
{
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
#if _DECK_DIAGNOSTIC_
    if(ExtractResetStart(pPacket) == true)
    {
        return;
    }
#endif
#endif // DVD_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05

    // DVD 상태.
    if(ExtractDVDStatus(pPacket) == true) {
        if(m_bLoadComplete == false) {
#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
            m_nDiscDetectRetryCount = 0;
            m_nDiscPlayRetryCount = 0;
#endif
            MC_LOG << "DVD LOAD COMPLETE!!!" << LOG_ENDL;
            m_bLoadComplete = true;
            
            if(DeckManager::GetInstance()->GetMediaMode() == eDM_MODE_DVD_AUDIO) {
                DeckManager::GetInstance()->EmitDiscStateChanged(PLAYER_DVDAUDIOINSERTED);
            }
            else {
                DeckManager::GetInstance()->EmitDiscStateChanged(PLAYER_DVDVIDEOINSERTED);
            }

// { modified by cychoi 2013.06.20 for SW reset on disc loading
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
#if _DECK_DIAGNOSTIC_
            if(DeckManager::GetInstance()->GetResetRestart() == true)
            {
                if(m_nRetryPlaybackMediaState != eMP_STATE_NONE)
                {
                    StatusData packet;
                    packet.m_nCode = DS_CODE_RESET_START;
                    ExtractResetStart(&packet);
                }
                DeckManager::GetInstance()->SetResetRestart(false);
            }
#endif
#endif // DVD_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05
// } modified by cychoi 2013.06.20
        }
        else {
            if(m_bAudioSearchMode == true)      AudioLangSearchMode(); // 오디오 언어 리스트 검색 모드.
            if(m_bSubTitleSearchMode == true)   SubtitleLangSearchMode(); // 자막 언어 리스트 검색 모드.
            if(m_tAudioTask.count() > 0)        SetAudioLangMode();
            if(m_tSubtitleTask.count() > 0)     SetSubtitleLangMode();
            if(m_tAngleTask.count() > 0)        SetAngleNumberMode();
            if(m_tSearchTask.count() > 0)       SearchTitleChapterMode();
            if(m_bDVDSettingMode == true)       InitDVDSetting();
            if(m_nUserTimeSearch >= 0)          SetCurrentMediaPosition(m_nUserTimeSearch);
        }
        return;
    }
    
    if(ExtractParentalChangeRequest(pPacket) == true) 
        return;
    if(ExtractDirectTouch(pPacket) == true)
        return;
    if(ExtractReqError(pPacket) == true)
        return;
}

int DVDManager::ChangeMediaMode(MEDIA_PLAYER_MODE eMode)
{
    m_bControllerChanged = true;
    MediaManager::GetInstance()->SetMediaMode(MP_MODE_DISC, eMode);
    return true;
}

QString DVDManager::GetCurrentFileName()
{
    return QString("%1").arg(m_tStatus.m_nTrackChapterNo);
}

int DVDManager::GetCurrentIndex()
{
    return m_nPlayTrackNo - 1;
}

void DVDManager::SetCurrentIndex(int nIndex)
{
    MC_LOG << "DVDManager::SetCurrentIndex(" << nIndex << ")" << LOG_ENDL;
    m_nCurrTrackIndex = nIndex;
}

int DVDManager::GetCurrentMediaState()
{
    // { added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
    if(m_eLastPlayerState == eMP_STATE_STOPPED)
    {
        return PLAYER_STOP;
    }
    // } added by cychoi 2013.10.06

    switch(m_ePlayerState)
    {
    case eMP_STATE_STOPPED:        return PLAYER_STOP;
    case eMP_STATE_PLAYING:        return PLAYER_PLAY;
    case eMP_STATE_PAUSED:         return PLAYER_PAUSE;
    case eMP_STATE_SEARCHING:      return PLAYER_SEARCH;
    case eMP_STATE_FAST_PLAYING: 
    {
        if(m_bREWMode == false)    return PLAYER_FASTFORWARD;
        else                       return PLAYER_REWIND;
    }
    default:                       return PLAYER_STOP;
    }
}

int DVDManager::GetCurrentMediaDuration()
{
    int nDuration = DVD_DURATION_VALUE;
    
    return nDuration;
}

int DVDManager::GetCurrentMediaPosition()
{
    return m_tStatus.m_nPlaybackTime;
}

void DVDManager::SetCurrentMediaPosition(int position)
{
    if(position < 0) {
        MC_LOG << "DVDManager::SetCurrentMediaPosition(" << position << ")" << LOG_ENDL;
        return;
    }
    
    if(DeckManager::GetInstance()->GetMediaMode() == eDM_MODE_DVD_VIDEO) 
    {
        switch(m_tStatus.m_nPlayStatus)
        {
        case eDM_CD_PLAY_COND_PLAYING:
        case eDM_CD_PLAY_COND_PLAYING_SLOW:
        case eDM_CD_PLAY_COND_PAUSING:
        case eDM_CD_PLAY_COND_PLAYING_FAST:
        case eDM_CD_PLAY_COND_SEARCHING_IN:
            {
                int nHour = (position / 1000) / 3600;
                int nMin  = ((position / 1000) / 60) % 60;
                int nSec  = (position / 1000) % 60;
                MC_LOG << "DVDManager::SetCurrentMediaPosition(" << position << ")->PlayStatus: " << m_tStatus.m_nPlayStatus << LOG_ENDL;
                m_nUserTimeSearch = -1;
                m_pPacket->TimeSearch(eDM_MODE_DVD_VIDEO, nHour, nMin, nSec);
            }
            break;
        case eDM_CD_PLAY_COND_SEARCHING_OUT:
            MC_LOG << "DVDManager::SetCurrentMediaPosition(" << position << ")->PLAY_COND_SEARCHING_OUT" << LOG_ENDL;
            m_nUserTimeSearch = position;
            break;
        default:
            break;
        }
    }
}

int DVDManager::GetDeviceState()
{
    DeckManager *p = DeckManager::GetInstance();
    
    return p->GetDeviceState();
}

void DVDManager::GetCurrentMediaInfo(MediaInfo *pMediaInfo)
{
    if(pMediaInfo) {
    }
}

void DVDManager::GetDeviceInfo(DeviceInfo *pDeviceInfo)
{
    if(pDeviceInfo) {
    }
}

void DVDManager::GetPlaylist(PlayList *pPlaylist)
{
    if(pPlaylist) 
    {
        int nIndex = m_nPlayTrackNo - 1;
        if(nIndex < 0) nIndex = 0;
        
        pPlaylist->index        = nIndex;
        pPlaylist->position     = (double)m_tStatus.m_nPlaybackTime;
        pPlaylist->PlaylistName = QString();
        pPlaylist->mode         = GetCurrentMediaState();

        if(DeckManager::GetInstance()->GetMediaMode() == eDM_MODE_DVD_AUDIO) {
            for(int i = 0; i < m_tStatus.m_nTotalChapterNo; i++) {
                pPlaylist->list->append(QString("Track %1").arg(i+1));
            }
        }else{
            for(int i = 0; i < m_tStatus.m_nTotalChapterNo; i++) {
                pPlaylist->list->append(QString("Chapter %1").arg(i+1));
            }
        }
    }
}

void DVDManager::SetPlayList(PlayList *pPlaylist)
{
    Q_UNUSED(pPlaylist);

    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

bool DVDManager::SeekableMedia()
{
    return true;
}

void DVDManager::SetBrightness(double fBrightness)
{
    DeckManager::GetInstance()->SetBrightness((int)fBrightness);
}

void DVDManager::SetContrast(double fContrast)
{
    DeckManager::GetInstance()->SetContrast((int)fContrast);
}

void DVDManager::SetTint(double fTint)
{
    DeckManager::GetInstance()->SetTint((int)fTint);
}

void DVDManager::SetBrightness(MEDIA_DISPLAY displayPlane, double fBrightness)
{
    Q_UNUSED(displayPlane);
    SetBrightness(fBrightness);
}

void DVDManager::SetContrast(MEDIA_DISPLAY displayPlane, double fContrast)
{
    Q_UNUSED(displayPlane);
    SetContrast(fContrast);
}

void DVDManager::SetTint(MEDIA_DISPLAY displayPlane, double fTint)
{
    Q_UNUSED(displayPlane);
    SetTint(fTint);
}

double DVDManager::GetBrightness()
{
    return DeckManager::GetInstance()->GetBrightness();
}

double DVDManager::GetContrast()
{
    return DeckManager::GetInstance()->GetContrast();
}

double DVDManager::GetTint()
{
    return DeckManager::GetInstance()->GetTint();
}

void DVDManager::SetSystemState(int nSystemState)
{
    m_nSystemState = nSystemState;
    // { added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
    switch(m_nSystemState)
    {
        case SYSTEM_BT_CALL_START:
            m_bBTCallStart = true;
            MC_HIGH << "DVDManager::SetSystemState(): SYSTEM_BT_CALL_START" << LOG_ENDL;
            break;
        case SYSTEM_BT_CALL_END:
            m_bBTCallStart = false;
            MC_HIGH << "DVDManager::SetSystemState(): SYSTEM_BT_CALL_END" << LOG_ENDL;
            break;
        default:
            break;
    }
    // } added by cychoi 2013.08.01
}

QWidget* DVDManager::GetVideoWidget()
{
    return DeckManager::GetInstance()->GetVideoWidget();
}

void DVDManager::EnableSubTitle(bool bEnable)
{
    Q_UNUSED(bEnable);
}

void DVDManager::SetPlayerEngine(PLAYER_ENGINE_TYPE eType)
{
    Q_UNUSED(eType);
}

void DVDManager::SetFontSize(const int fontSize)
{
    Q_UNUSED(fontSize);
}

bool DVDManager::ClearDisplay(bool isBackground)
{
    return DeckManager::GetInstance()->ClearDisplay(isBackground);
}

void DVDManager::SetHue(double value)
{
    Q_UNUSED(value);
}

void DVDManager::SetHue(MEDIA_DISPLAY displayPlane, double value)
{
    Q_UNUSED(displayPlane);
    Q_UNUSED(value);
}

double DVDManager::GetHue()
{
    return 0.0;
}

void DVDManager::GetCurrentFileFolderIndex(int *pFileIdx, int *pTotalFiles)
{
    *pFileIdx    = GetCurrentIndex();
    *pTotalFiles = m_tStatus.m_nTotalChapterNo;
}

void DVDManager::AudioDragStart()
{
    
}

void DVDManager::AudioDragEnd()
{
    
}

void DVDManager::FrameAdvance()
{
    MC_LOG << "DVDManager::FrameAdvance()" << LOG_ENDL;
    
    m_pPacket->FrameAdvance();
}


void DVDManager::FrameBack()
{
    MC_LOG << "DVDManager::FrameBack()" << LOG_ENDL;
    
    m_pPacket->FrameBack();
}


void DVDManager::SubTitle(uint mode)
{
    if(mode == 1)
        m_pPacket->SubtitleOn();
    else
        m_pPacket->SubTitleOff();
}

void DVDManager::Load()
{
    if(DeckManager::GetInstance()->IsExistCDInDeck() == true)
    {
        MC_LOG << "DVDManager::Load()" << LOG_ENDL;
        m_pPacket->Loading();
    }
}

void DVDManager::Eject(uint mode)
{
    if(DeckManager::GetInstance()->IsExistCDInDeck() == true || mode == 1)
    {
#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
        InitStillPictureTimer();
#endif

        // { added by cychoi 2013.09.29 for PV1 Disc sound mute for a while
#ifdef DVD_DECK_REQ_RETRY_ENABLED
        InitRetryVRInfo();
        InitRetryPlayback();
        InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
#endif
        // } added by cychoi 2013.09.29
#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
        m_nDiscDetectRetryCount = 0;
        m_nDiscPlayRetryCount = 0;
#endif

        if(GetRepeatMode(m_tStatus.m_nRepeatMode) != REPEATALL) {
            RepeatPlay(REPEATALL);
        }
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }
        
        MC_HIGH << "DVDManager::Eject(" << mode << ")" << LOG_ENDL;
        DeckManager::GetInstance()->SetEject(true);
        m_pPacket->FunctionStop();
        m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
        DeckManager::GetInstance()->Eject(mode);
        Init();
        m_pPacket->RequestCommonStatus();
    }
}

void DVDManager::SlowForward(uint nMode)
{
    switch(nMode){
    case SLOW_FORWARD_X2:
        m_pPacket->SlowForward(eDM_SLOW_FORWARD_X2);
        break;
    case SLOW_FORWARD_X4:
        m_pPacket->SlowForward(eDM_SLOW_FORWARD_X4);
        break;
    case SLOW_FORWARD_X8:
        m_pPacket->SlowForward(eDM_SLOW_FORWARD_X8);
        break;
    case SLOW_FORWARD_X16:
        m_pPacket->SlowForward(eDM_SLOW_FORWARD_X16);
        break;
    default:
        break;
    }
}

void DVDManager::SlowRewind(uint nMode)
{
    switch(nMode){
    case SLOW_BACKWARD_X4:
        m_pPacket->SlowBackward(eDM_SLOW_BACKWARD_X4);
        break;
    case SLOW_BACKWARD_X8:
        m_pPacket->SlowBackward(eDM_SLOW_BACKWARD_X8);
        break;
    case SLOW_BACKWARD_X16:
        m_pPacket->SlowBackward(eDM_SLOW_BACKWARD_X16);
        break;
    case SLOW_BACKWARD_X24:
        m_pPacket->SlowBackward(eDM_SLOW_BACKWARD_X24);
        break;
    default:
        break;
    }
}

void DVDManager::PageUp()
{
    m_pPacket->PageUp();
}

void DVDManager::PageDown()
{
    m_pPacket->PageDown();
}

void DVDManager::Titlemenu(uint nSetting)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Title Menu operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    MC_LOG << "DVDManager::Titlemenu(" << nSetting << ")" << LOG_ENDL;
    
    if(nSetting == 0) {
        m_pPacket->TitleMenu(eDM_TITLE_MENU_TOGGLE);
    }
    else {
        m_pPacket->TitleMenu(eDM_TITLE_MENU_ALWAYS);
    }
}

void DVDManager::EnterKey()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Enter Key operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    //{ modified by cychoi 20103.08.13 for ITS 183910 Sound mute on Enter Key
    m_bAutoTrackChanged = false;
    m_bUserTrackChanged = true;
    //} modified by cychoi 20103.08.13
    m_pPacket->Enter();
}

void DVDManager::ReturnKey()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Return Key operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    m_pPacket->Return();
}

void DVDManager::CursorKey(uint direction)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Cursor Key operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    MC_LOG << "DVDManager::CursorKey: " << direction << LOG_ENDL;
    
    switch(direction)
    {
    case CURSOR_LEFT:
        m_pPacket->Cursor(eDM_CURSOR_LEFT_PUSH);
        break;
    case CURSOR_RIGHT:
        m_pPacket->Cursor(eDM_CURSOR_RIGHT_PUSH);
        break;
    case CURSOR_UP:
        m_pPacket->Cursor(eDM_CURSOR_UP_PUSH);
        break;
    case CURSOR_DOWN:
        m_pPacket->Cursor(eDM_CURSOR_DOWN_PUSH);
        break;
    default:
        return;
    }
}

void DVDManager::TopMenu(uint setting)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Top Menu operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    MC_LOG << "DVDManager::TopMenu(" << setting << ")" << LOG_ENDL;
    
    // { commented by cychoi 2013.10.30 for ITS 204969
    // { added by jeeeun for SANITY_CM_AG127 disc menu fail
    //m_bTopMenu = true; 
    //m_titleSetting = setting;
    // } added by jeeeun
    // } commented by cychoi 2013.10.30

    if(setting == 0) {
        m_pPacket->TopMenu(eDM_TOP_MEMU_TOGGLE);
    }
    else {
        m_pPacket->TopMenu(eDM_TOP_MEMU_ALWAYS);
    }
}

void DVDManager::SwitchSubTitle()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Sub Title operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    m_pPacket->Subtitle();
}

void DVDManager::SwitchAngle()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Angle operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    m_pPacket->Angle();
}

void DVDManager::SwitchAudio()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Audio operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    // { added by cychoi 2013.10.25 for ITS 195688 Audio repeated request
    if(m_nReqSwitchAudio == 0)
    {
        m_pPacket->Audio();
    }
    m_nReqSwitchAudio++;
    // } added by cychoi 2013.10.25
    m_bAudioSwitched = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
}

void DVDManager::Search()
{
    m_pPacket->SearchCalling();
}

void DVDManager::InitializeDeckPrioritySettings(DeckPrioritySettings *pSettings)
{
    DeckManager::GetInstance()->InitializeDeckPrioritySettings(pSettings);
}

void DVDManager::InitializeDVDSettings(DVDSettings *pSettings)
{
    if(pSettings == 0) {
        MC_LOG << "DVDManager::InitializeDVDSettings() pSettings == 0" << LOG_ENDL;
        return;
    }
    
    if(m_bDVDSettingMode == false) {
        MC_LOG << "DVDManager::START DVD SETTINGS nAngleNumber:" << pSettings->nAngleNumber << ", nAudioCode:" << pSettings->nAudioCode << "(" << pSettings->nAudioIndex << "), nSubtitleCode:" << pSettings->nSubtitleCode << LOG_ENDL;
        m_tDVDSettings.nAngleNumber  = pSettings->nAngleNumber;
        m_tDVDSettings.nAudioCode    = pSettings->nAudioCode;
        m_tDVDSettings.nAudioIndex   = pSettings->nAudioIndex;
        m_tDVDSettings.nSubtitleCode = pSettings->nSubtitleCode;
        m_nDVDSettings    = DSS_START_STEP;
        m_bDVDSettingMode = true;

        RequestDVDStatus();
    }
    else {
        MC_LOG << "DVDManager::InitializeDVDSettings() previous setting in progress!!!" << LOG_ENDL;
    }
}

int DVDManager::RequestTOC()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int DVDManager::RequestTOC2(int nBlockNo)
{
    Q_UNUSED(nBlockNo);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

void DVDManager::TenKey(int nKeyID, int pressStatus)
{
    int nOper1 = 0;
    int nOper2 = 0;
    
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Ten Key operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    if(pressStatus == KEY_PRESSED)
    {
        switch(nKeyID)
        {
        case KEY_ID_0:
            nOper1 = 0x00;
            nOper2 = eDM_KEY_0;
            break;
        case KEY_ID_1:
            nOper1 = 0x00;
            nOper2 = eDM_KEY_1;
            break;
        case KEY_ID_2:
            nOper1 = 0x00;
            nOper2 = eDM_KEY_2;
            break;
        case KEY_ID_3:
            nOper1 = 0x00;
            nOper2 = eDM_KEY_3;
            break;
        case KEY_ID_4:
            nOper1 = 0x00;
            nOper2 = eDM_KEY_4;
            break;
        case KEY_ID_5:
            nOper1 = 0x00;
            nOper2 = eDM_KEY_5;
            break;
        case KEY_ID_6:
            nOper1 = 0x00;
            nOper2 = eDM_KEY_6;
            break;
        case KEY_ID_7:
            nOper1 = 0x00;
            nOper2 = eDM_KEY_7;
            break;
        case KEY_ID_8:
            nOper1 = eDM_KEY_8;
            nOper2 = 0x00;
            break;
        case KEY_ID_9:
            nOper1 = eDM_KEY_9;
            nOper2 = 0x00;
            break;
        case KEY_ID_CLEAR: 
            nOper1 = eDM_KEY_CLEAR;
            nOper2 = 0x00;
            break;
        }
    }
    
    m_pPacket->Key10(nOper1, nOper2);
}

void DVDManager::DualMonoChange()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

int DVDManager::GetVCDStatus(VCDStatusResponse *pResponse)
{
    Q_UNUSED(pResponse);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int DVDManager::GetDVDStatus(DVDStatusResponse *pResponse)
{
    if(pResponse == 0) return -1;

    pResponse->playStatus           = m_tStatus.m_nPlayStatus;
    pResponse->currentContentType   = m_tStatus.m_nContent;
    pResponse->pictureStatus        = m_tStatus.m_bPictureExist;
    pResponse->randomModeStatus     = m_tStatus.m_nRandomMode;
    pResponse->repeatModeStatus     = m_tStatus.m_nRepeatMode;
    pResponse->scanModeStatus       = m_tStatus.m_nScanMode;

    pResponse->groupOrTitleNum      = m_tStatus.m_nGrpTitlePGPLNo; // Group no. for DVD-A and Title num for DVD-V.
    pResponse->trackOrChapterNo     = m_tStatus.m_nTrackChapterNo; // Ongoing track(DVD-A)/chapter(DVD-V)
    pResponse->playbackTime         = m_tStatus.m_nPlaybackTime / 1000; // elpased Time.
    pResponse->multiAngle           = m_tStatus.m_bMultiAngleOff; // Multi angle playback on/off (DVD-V)
    pResponse->subTitleNum          = m_tStatus.m_nSubTitleNo; // id of current subtitle set.
    pResponse->totalSubTitleNum     = m_tStatus.m_nTotalSubTitleNo; // Total subtitle present.
    pResponse->subTitleCode         = m_tStatus.m_nSubTitleCode; // Sub title code of ongoing video.
    pResponse->audioSetting         = m_tStatus.m_nAudioNo; // Curent audio setting
    pResponse->totalAudioSetting    = m_tStatus.m_nTotalAudioNo; // Total audio setting
    pResponse->audioCode            = m_tStatus.m_nAudioCode; // Audio code for current audio setting
    pResponse->audioAttribute       = m_tStatus.m_nAudioAttr; // Represents audio attribute of the ongoing DVD-Audio/Video.
    pResponse->channel              = m_tStatus.m_nChannel;
    pResponse->LFEExist             = m_tStatus.m_bLFEExist; // added by cychoi 2013.07.20 for ITS 181110 Audio attribute
    pResponse->angleNo              = m_tStatus.m_nAngleNo;
    pResponse->totalAngleNo         = m_tStatus.m_nTotalAngleNo;
    pResponse->displayState         = m_tStatus.m_nDisplayScreen; // Represent current display state.
    pResponse->totalChaptorOrTrack  = m_tStatus.m_nTotalChapterNo; // Total number of chapters(DVD-V) or track(DVD-A)
    pResponse->totalGroupOrTitle    = m_tStatus.m_nTotalGrpTlePGPLNo; // Total number of Title(DVD-V)or Group(DVD-A)

    #if 0
    MC_LOG << "DVDManager::playStatus          : " << pResponse->playStatus << LOG_ENDL;
    MC_LOG << "DVDManager::currentContentType  : " << pResponse->currentContentType << LOG_ENDL;
    MC_LOG << "DVDManager::pictureStatus       : " << pResponse->pictureStatus << LOG_ENDL;
    MC_LOG << "DVDManager::randomModeStatus    : " << pResponse->randomModeStatus << LOG_ENDL;
    MC_LOG << "DVDManager::repeatModeStatus    : " << pResponse->repeatModeStatus << LOG_ENDL;
    MC_LOG << "DVDManager::scanModeStatus      : " << pResponse->scanModeStatus << LOG_ENDL;
    MC_LOG << "DVDManager::groupOrTitleNum     : " << pResponse->groupOrTitleNum << LOG_ENDL;
    MC_LOG << "DVDManager::trackOrChapterNo    : " << pResponse->trackOrChapterNo << LOG_ENDL;
    MC_LOG << "DVDManager::playbackTime        : " << pResponse->playbackTime << LOG_ENDL;
    MC_LOG << "DVDManager::multiAngle          : " << pResponse->multiAngle << LOG_ENDL;
    MC_LOG << "DVDManager::subTitleNum         : " << pResponse->subTitleNum << LOG_ENDL;
    MC_LOG << "DVDManager::totalSubTitleNum    : " << pResponse->totalSubTitleNum << LOG_ENDL;
    MC_LOG << "DVDManager::subTitleCode        : " << pResponse->subTitleCode << LOG_ENDL;
    MC_LOG << "DVDManager::audioSetting        : " << pResponse->audioSetting << LOG_ENDL;
    MC_LOG << "DVDManager::totalAudioSetting   : " << pResponse->totalAudioSetting << LOG_ENDL;
    MC_LOG << "DVDManager::audioCode           : " << pResponse->audioCode << LOG_ENDL;
    MC_LOG << "DVDManager::audioAttribute      : " << pResponse->audioAttribute << LOG_ENDL;
    MC_LOG << "DVDManager::angleNo             : " << pResponse->angleNo << LOG_ENDL;
    MC_LOG << "DVDManager::totalAngleNo        : " << pResponse->totalAngleNo << LOG_ENDL;
    MC_LOG << "DVDManager::displayState        : " << pResponse->displayState << LOG_ENDL;
    MC_LOG << "DVDManager::totalChaptorOrTrack : " << pResponse->totalChaptorOrTrack << LOG_ENDL;
    MC_LOG << "DVDManager::totalGroupOrTitle   : " << pResponse->totalGroupOrTitle << LOG_ENDL;
    #endif
    
    return 1;
}

int DVDManager::GetDVDPrioritySettings(DeckPrioritySettings *pSettings)
{
    if(pSettings == 0) {
        MC_HIGH << "GetDVDPrioritySettings() pSettings is 0" << LOG_ENDL;
        return -1;
    }

    DeckInit1Setting tDeckSetting;
    DeckManager::GetInstance()->GetInitSetting1(tDeckSetting);

    pSettings->captionLanguage  = tDeckSetting.m_nPrioSubTitleCode;
    pSettings->audioLangauge    = tDeckSetting.m_nPrioAudioCode;
    pSettings->dvdAngle         = 1;
    pSettings->menuLanguage     = tDeckSetting.m_nPrioMenuLangCode;
    
    return 1;
}


int DVDManager::RequestAudioLanguages()
{
#if 0
    if(m_tAudioTask.count() > 0) {
        MC_LOG << "DVDManager::RequestAudioLanguages(): m_tAudioTask.count(): " << m_tAudioTask.count() << LOG_ENDL;
        return -1;
    }
    
    switch(m_tStatus.m_nPlayStatus)
    {
    case eDM_CD_PLAY_COND_PLAYING:
    case eDM_CD_PLAY_COND_PLAYING_SLOW:
    case eDM_CD_PLAY_COND_SEARCHING_OUT:
    case eDM_CD_PLAY_COND_PAUSING:
    case eDM_CD_PLAY_COND_PLAYING_FAST:
    case eDM_CD_PLAY_COND_SEARCHING_IN:
        break;
    default:
        MC_LOG << "DVDManager::RequestAudioLanguages(): m_nPlayStatus: " << m_tStatus.m_nPlayStatus << LOG_ENDL;
        EmitError(PLAYER_INVALIDSTATE);
        return -1;
    }

    switch(m_tStatus.m_nDisplayScreen)
    {
    case eDM_DISPLAY_SCREEN_PLAY:
        break;
    //case eDM_DISPLAY_SCREEN_ROOT_MENU:
    //case eDM_DISPLAY_SCREEN_TITLE_MENU:
    default:
        MC_LOG << "DVDManager::RequestAudioLanguages(): m_nDisplayScreen: " << m_tStatus.m_nDisplayScreen << LOG_ENDL;
        // 없음으로 넘기도록 변경함.
        // EmitError(PLAYER_INVALIDSTATE);
        // return -1;
        break;
    }

    // 현재 재생중에 음성 개수가 변경된 경우.
    if(m_bAudioSearchMode == false && m_tStatus.m_nTotalAudioNo != m_nAudioCodeNum) {
        MC_LOG << "DVDManager::RequestAudioLanguages(): changed audio size: " << m_nAudioCodeNum << "->" << m_tStatus.m_nTotalAudioNo << LOG_ENDL;
        m_nAudioCodeNum = 0;
        memset(m_pAudioCodeList, 0xFF, sizeof(int)*100);
        memset(m_pAudioAttrList, 0xFF, sizeof(int)*100);
        memset(m_pAudioChnlList, 0xFF, sizeof(int)*100);
    }

    // 음성이 없는 경우.
    if(m_bAudioSearchMode == false && m_tStatus.m_nTotalAudioNo <= 0) {
        m_nAudioCodeNum     = 1;
        m_pAudioCodeList[0] = 0;
        m_pAudioAttrList[0] = m_tStatus.m_nAudioAttr;
        m_pAudioChnlList[0] = m_tStatus.m_nChannel;
        EmitOnTotalAudioLangReceived();
        return 1;
    }
    // 음성이 하나만 있는 경우.
    else if(m_bAudioSearchMode == false && m_tStatus.m_nTotalAudioNo == 1) {
        m_nAudioCodeNum     = 1;
        m_pAudioCodeList[0] = m_tStatus.m_nAudioCode;
        m_pAudioAttrList[0] = m_tStatus.m_nAudioAttr;
        m_pAudioChnlList[0] = m_tStatus.m_nChannel;
        EmitOnTotalAudioLangReceived();
        return 1;
    }
    
    if(m_bAudioSearchMode == false && m_nAudioCodeNum == 0) {
        if(m_tStatus.m_nPlayStatus == eDM_CD_PLAY_COND_PAUSING) m_bPauseAtSetting = true;
        if(m_pPacket->Audio() > 0) {
            m_bAudioSearchMode = true;
            return 1;
        }
        else {
            return -1;
        }
    }
    else if(m_bAudioSearchMode == false && m_nAudioCodeNum > 0) {
        EmitOnTotalAudioLangReceived();
        return 1;
    }
    else {
        return 0;
    }
#else
    return 0;
#endif
}

void DVDManager::SetAudioLanguage(int nAudioCode, int nIndex)
{
#if 0    
    SetAudioLanguage_Impl(nAudioCode, nIndex);
#else
    Q_UNUSED(nAudioCode);
    Q_UNUSED(nIndex);
#endif
}

int DVDManager::RequestSubtitleLanguages()
{
#if 0    
    if(m_tSubtitleTask.count() > 0) {
        MC_LOG << "DVDManager::RequestSubtitleLanguages(): m_tSubtitleTask.count(): " << m_tSubtitleTask.count() << LOG_ENDL;
        return -1;
    }
    
    switch(m_tStatus.m_nPlayStatus)
    {
    case eDM_CD_PLAY_COND_PLAYING:
    case eDM_CD_PLAY_COND_PLAYING_SLOW:
    case eDM_CD_PLAY_COND_SEARCHING_OUT:
    case eDM_CD_PLAY_COND_PAUSING:
    case eDM_CD_PLAY_COND_PLAYING_FAST:
    case eDM_CD_PLAY_COND_SEARCHING_IN:
        break;
    default:
        MC_LOG << "DVDManager::RequestSubtitleLanguages(): m_nPlayStatus: " << m_tStatus.m_nPlayStatus << LOG_ENDL;
        EmitError(PLAYER_INVALIDSTATE);
        return -1;
    }

    switch(m_tStatus.m_nDisplayScreen)
    {
    case eDM_DISPLAY_SCREEN_PLAY:
        break;
    //case eDM_DISPLAY_SCREEN_ROOT_MENU:
    //case eDM_DISPLAY_SCREEN_TITLE_MENU:
    default:
        MC_LOG << "DVDManager::RequestSubtitleLanguages(): m_nDisplayScreen: " << m_tStatus.m_nDisplayScreen << LOG_ENDL;
        // 없음으로 넘기도록 변경함.
        //EmitError(PLAYER_INVALIDSTATE);
        //return -1;
        break;
    }

    // 현재 재생중에 자막 개수가 변경된 경우.
    if(m_bSubTitleSearchMode == false && (m_tStatus.m_nTotalSubTitleNo + 1 != m_nSubTitleCodeNum)) {
        MC_LOG << "DVDManager::RequestSubtitleLanguages(): changed subtitle size: " << m_nSubTitleCodeNum << "->" <<  m_tStatus.m_nTotalSubTitleNo + 1 << LOG_ENDL;
        m_nSubTitleCodeNum = 0;
        memset(m_pSubTitleCodeList, 0xFF, sizeof(int)*100);
    }

    // 자막이 없는 경우.
    if(m_tStatus.m_nTotalSubTitleNo <= 0 || (m_tStatus.m_nTotalSubTitleNo == 1 && m_tStatus.m_nSubTitleCode == 0)) {
        MC_LOG << "DVDManager::RequestSubtitleLanguages(): no subtitle(" << m_tStatus.m_nTotalSubTitleNo << "/" m_tStatus.m_nSubTitleNo << "/" m_tStatus.m_nSubTitleCode << ")" << LOG_ENDL;
        m_pSubTitleCodeList[0] = 0;
        m_nSubTitleCodeNum     = 1;
        EmitOnTotalSubtitleLangReceived();
        return 1;
    }
    
    if(m_bSubTitleSearchMode == false && m_nSubTitleCodeNum == 0) {
        if(m_tStatus.m_nPlayStatus == eDM_CD_PLAY_COND_PAUSING) m_bPauseAtSetting = true;
        m_nPrevSubtitleLangNo = 0;
        if(m_pPacket->Subtitle() > 0) {
            m_bSubTitleSearchMode = true;
            return 1;
        }
        else {
            return -1;
        }
    }
    else if(m_bSubTitleSearchMode == false && m_nSubTitleCodeNum > 0) {
        EmitOnTotalSubtitleLangReceived();
        return 1;
    }
    else {
        MC_LOG << "DVDManager::RequestSubtitleLanguages(): m_bSubTitleSearchMode: " << m_bSubTitleSearchMode << LOG_ENDL;
        return 0;
    }
#else
    return 0;
#endif
}

void DVDManager::SetSubtitleLanguage(int nSubtitleCode)
{
#if 0      
    SetSubtitleLanguage_Impl(nSubtitleCode);
#else
    Q_UNUSED(nSubtitleCode);
#endif
}

void DVDManager::SetAngle(int nAngleNumber)
{
#if 0    
    SetAngle_Impl(nAngleNumber);
#else
    Q_UNUSED(nAngleNumber);
#endif
}

void DVDManager::SetPBC(uint nMode)
{
    Q_UNUSED(nMode);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void DVDManager::SetOSD(uint nMode)
{
    DeckManager::GetInstance()->SetOSD(nMode);
}

void DVDManager::SearchTitleChapter(uint nTitle, uint nChapter)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Search Title Chapter operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    if(nTitle == 0) {
        nTitle = 1;
        MC_LOG << "DVDManager::SearchTitleChapter() nTitle is 0. changed to 1" << LOG_ENDL;
    }
    if(nChapter == 0) {
        nChapter = 1;
        MC_LOG << "DVDManager::SearchTitleChapter() nChapter is 0. nChapter to 1" << LOG_ENDL;
    }
    
    MC_LOG << "DVDManager::SearchTitleChapter(" << nTitle << ", " << nChapter << ")" << LOG_ENDL;

    if((int)nTitle > m_tStatus.m_nTotalGrpTlePGPLNo) {
        MC_LOG << "DVDManager::SearchTitleChapter() nTitle(" << nTitle << ") > m_nTotalGrpTlePGPLNo(" << m_tStatus.m_nTotalGrpTlePGPLNo << ")" << LOG_ENDL;
        emit OnSearchCompleted(0, 0, 0);
        return;
    }
    
    if(m_tStatus.m_nGrpTitlePGPLNo != (int)nTitle || m_tStatus.m_nTrackChapterNo != (int)nChapter) 
    {
        MC_LOG << "DVDManager::SearchTitleChapter() screen: " << m_tStatus.m_nDisplayScreen << ", play status: " << m_tStatus.m_nPlayStatus << LOG_ENDL;

        m_tSearchMutex.lock();
        TitleChapterTask tTask;
        int bRequest = true;

        if(m_tSearchTask.count() > 0) {
            tTask = m_tSearchTask.last();
            if(tTask.m_nTitleNo == (int)nTitle && tTask.m_nChapterNo == (int)nChapter) {
                MC_LOG << "DVDManager::SearchTitleChapter(" << nTitle << ", " << nChapter << ") equal to previous request" << LOG_ENDL;
                bRequest = false;
            }
        }

        if(bRequest == true) {
            tTask.m_nTitleNo   = nTitle;
            tTask.m_nChapterNo = nChapter;
            m_tSearchTask.append(tTask);
            if(m_tStatus.m_nDisplayScreen != eDM_DISPLAY_SCREEN_PLAY) {
                if(m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_ROOT_MENU) {
                    MC_LOG << "DVDManager::SearchTitleChapter() --> FunctionStop()" << LOG_ENDL;
                    m_pPacket->FunctionStop();
                }
                else if(m_tStatus.m_nPlayStatus == eDM_CD_PLAY_COND_HALTING) {
                    MC_LOG << "DVDManager::SearchTitleChapter() --> RequestDVDAudioVideoVRStatus()" << LOG_ENDL;
                    m_pPacket->RequestDVDAudioVideoVRStatus();
                }
                else {
                    MC_LOG << "DVDManager::SearchTitleChapter() --> Stop()" << LOG_ENDL;
                    m_pPacket->Stop();
                }
            }
            else if(m_tStatus.m_nTotalGrpTlePGPLNo == m_tStatus.m_nGrpTitlePGPLNo) {
                MC_LOG << "DVDManager::SearchTitleChapter() --> Stop()" << LOG_ENDL;
                m_pPacket->Stop();
            }
            else if(m_tStatus.m_nPlayStatus == eDM_CD_PLAY_COND_PAUSING) {
                MC_LOG << "DVDManager::SearchTitleChapter() --> RequestDVDAudioVideoVRStatus()" << LOG_ENDL;
                m_pPacket->RequestDVDAudioVideoVRStatus();
            }
#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
            else if(m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_PLAY)
            {
                if(GetCurrentMediaState() == PLAYER_PLAY &&
                   m_nStartPlaybackTime == m_tStatus.m_nPlaybackTime)
                {
                    m_pPacket->RequestDVDAudioVideoVRStatus();
                    InitStillPictureTimer();
                }
            }
#endif
        }
        m_tSearchMutex.unlock();
    }
    else 
    {
        MC_LOG << "DVDManager::SearchTitleChapter(" << m_tStatus.m_nGrpTitlePGPLNo << ", " << m_tStatus.m_nTrackChapterNo << ", " << m_tStatus.m_nTotalChapterNo << ") end" << LOG_ENDL;
        emit OnSearchCompleted(m_tStatus.m_nGrpTitlePGPLNo, m_tStatus.m_nTrackChapterNo, m_tStatus.m_nTotalChapterNo);
    }
}

void DVDManager::CancelAudioLangRequest()
{
    if(m_bAudioSearchMode == true) {
        MC_LOG << "DVDManager::CancelAudioLangRequest()" << LOG_ENDL;
        m_bAudioSearchMode = false;
        m_nAudioCodeNum    = 0;
    }
}

void DVDManager::CancelSubtitleLangRequest()
{
    if(m_bSubTitleSearchMode == true) {
        MC_LOG << "DVDManager::CancelSubtitleLangRequest()" << LOG_ENDL;
        m_bSubTitleSearchMode = false;
        m_nSubTitleCodeNum    = 0;
    }
}

int DVDManager::GetCurrentScreenState()
{
    switch(m_tStatus.m_nDisplayScreen)
    {
    case eDM_DISPLAY_SCREEN_PLAY:        return PLAYER_PLAY_DISPLAY;
    case eDM_DISPLAY_SCREEN_SETTING:     return PLAYER_SETTING_DISPLAY;
    case eDM_DISPLAY_SCREEN_ROOT_MENU:   return PLAYER_ROOTMENU_DISPLAY;
    case eDM_DISPLAY_SCREEN_TITLE_MENU:  return PLAYER_TITLEMENU_DISPLAY;
    case eDM_DISPLAY_SCREEN_DEFAULT:     return PLAYER_DEFAULTSCREEN_DISPLAY;
    default:                             
        MC_LOG << "DVDManager::GetCurrentScreenState Error: " << m_tStatus.m_nDisplayScreen << LOG_ENDL;
        return PLAYER_DEFAULTSCREEN_DISPLAY;
    }
}

void DVDManager::Play()
{
    // { added by cychoi 2013-06-09 for ITS 172825 eject failure on FF_REW
    if(DeckManager::GetInstance()->GetDiscState() == PLAYER_DISCEJECTING ||
       DeckManager::GetInstance()->IsDeckBlocked() == true) // modified by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    {
        // We can't play operation if disc is ejecting, just return.
        // We can't play operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2013-06-09

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-05

    if(m_nCurrTrackIndex >= 0 &&
       m_tStatus.m_nGrpTitlePGPLNo != 0xFF &&
       (m_nPlayTrackNo - 1) != m_nCurrTrackIndex &&
       m_bFirstPlay == true) 
    {
// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifndef DVD_NEW_NAVIGATION_ENABLED
        if(GetCurrentMediaState() == PLAYER_SEARCH) {
            MC_LOG << "DVDManager::Play() RETURN ON SEARCH MODE" << LOG_ENDL;
            m_tUserReqTrackList.clear();
            m_tUserReqTrackList.append(m_nCurrTrackIndex);
            return;
        }
#endif
// } modified by cychoi 2013.06.30
        
        BCD4                tBCD4;
        BCD2                tBCD2;
        DirectContCommand   tCmd;

        tCmd.nFileDesign    = 0;
        tCmd.nPGPLDesggn    = 0;
        tCmd.nUndefined1    = 0;
        tCmd.nUndefined2    = 0;

        // TITLE/GROUP
        tBCD4.word          = DEC2BCD4(m_tStatus.m_nGrpTitlePGPLNo);
        tBCD2.BCD.upper     = tBCD4.BCD.upper2;
        tBCD2.BCD.lower     = tBCD4.BCD.lower2;
        
        tCmd.n1stCateUpper  = tBCD4.BCD.lower1;
        tCmd.n1stCateLower  = tBCD2.byte;

        // CHAPTER/TRACK
        tBCD4.word          = DEC2BCD4(m_nCurrTrackIndex + 1);
        tBCD2.BCD.upper     = tBCD4.BCD.upper2;
        tBCD2.BCD.lower     = tBCD4.BCD.lower2;

        tCmd.n2ndCateUpper  = tBCD4.BCD.lower1;
        tCmd.n2ndCateLower  = tBCD2.byte;

        MC_LOG << "DVDManager::Play()->DirectCont()" << LOG_ENDL;
        m_pPacket->DirectCont(&tCmd);

        m_nCurrTrackIndex = -1;
        m_bAutoTrackChanged = false; // modified by cychoi 20103.08.11 for Play mute
        m_bUserTrackChanged = true; // modified by cychoi 20103.08.11 for ITS 183835 No OSD on track change automatically // added by cychoi 20103.07.22 for OnUserTrackChanged signal
        m_bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
    }
    else {
#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
        if(EnterKeyOnStillPicture() == true)
        {
            return;
        }
#endif

        MC_LOG << "DVDManager::Play()->Play()" << LOG_ENDL;
        m_nCurrTrackIndex = -1;
        m_bFirstPlay      = true;
        // { modified by cychoi 2013.08.29 for ITS 187026 DVD Title/Disc Menu Loading Screen // { added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
        if(m_bBTCallStart == false)
        {
            m_pPacket->Play();
        }
        else
        {
            m_pPacket->Pause(); // modified by cychoi 2013.10.23 for ITS 196358 Keep the play state on BT call if pause is forbidden.
        }
        // } modified by cychoi 2013.08.29 // } added by cychoi 2013.08.01
        m_bAutoTrackChanged = GetCurrentMediaState() == PLAYER_PLAY; // modified by cychoi 20103.08.13 for ITS 183912 Sound mute on toggle play // modified by cychoi 20103.08.13 for Play mute
        m_bUserTrackChanged = GetCurrentMediaState() == PLAYER_STOP; // modified by cychoi 20103.08.11 for ITS 183835 No OSD on track change automatically // added by cychoi 20103.07.22 for OnUserTrackChanged signal
        m_bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
    }

    m_nFFREWRate = -1; // added by cychoi 20103.06.17 for ITS 174004
    m_nFFREWState = PLAYER_PLAY; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
    m_eLastPlayerState = eMP_STATE_PLAYING; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    StartRetryPlaybackTimer(7500); // DVD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-05
}

void DVDManager::Pause()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't pause operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09
    MC_LOG << "DVDManager::Pause()" << LOG_ENDL;
#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
    if(EnterKeyOnStillPicture() == true)
    {
        return;
    }
#endif

    m_pPacket->Pause(); // modified by cychoi 2013.10.23 for ITS 196358 Keep the play state on BT call if pause is forbidden.
}

void DVDManager::FunctionStop()
{
    MC_LOG << "DVDManager::FunctionStop()" << LOG_ENDL;

// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifndef DVD_NEW_NAVIGATION_ENABLED
    m_tUserReqTrackList.clear();
#endif
// } modified by cychoi 2013.06.30

    m_pPacket->FunctionStop();

    IPlayerEngine *pPlayer = DeckManager::GetInstance()->GetPlayerEngine();
    if(pPlayer) {
        pPlayer->Stop();
        MC_LOG << "DVDManager::FunctionStop()--> GPlayer->Stop()" << LOG_ENDL;
    }

    m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
}

void DVDManager::Stop()
{
    MC_LOG << "DVDManager::Stop()" << LOG_ENDL;

// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifndef DVD_NEW_NAVIGATION_ENABLED
    m_tUserReqTrackList.clear();
#endif
// } modified by cychoi 2013.06.30

    m_pPacket->Stop();
    
    IPlayerEngine *pPlayer = DeckManager::GetInstance()->GetPlayerEngine();
    if(pPlayer) {
        pPlayer->Stop();
        MC_LOG << "DVDManager::Stop()--> GPlayer->Stop()" << LOG_ENDL;
    }

    m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
}

void DVDManager::Next(int nSkipLevel)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't next operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
    if(EnterKeyOnStillPicture() == true)
    {
        return;
    }
#endif

// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
// 1. Play the first CHAPTER of the first TITLE if the last CHAPTER of the last TITLE.
// 2. Play the first CHAPTER of the next TITLE if the last CHAPTER.
#ifndef DVD_NEW_NAVIGATION_ENABLED
    if(GetCurrentMediaState() == PLAYER_SEARCH) {
        MC_LOG << "DVDManager::Next() RETURN ON SEARCH MODE" << LOG_ENDL;
        // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
        int nNewTrackIdx = GetNextTrackIdx(nSkipLevel, m_tUserReqTrackList.count() == 0 ? GetCurrentIndex() : m_tUserReqTrackList.count());
        // } modified by cychoi 2013-07-10
        m_tUserReqTrackList.append(nNewTrackIdx);
        emit OnTrackBufferred(false, nNewTrackIdx, nNewTrackIdx, m_tStatus.m_nTotalChapterNo);
        // { added by cychoi 2014.01.06 for ITS 217985
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
        m_bRealSoundChanged = false;
        // } added by cychoi 2014.01.06
        return;
    }
#endif
// } modified by cychoi 2013.06.30

// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-16

    if(GetCurrentMediaState() == PLAYER_STOP) {
        MC_LOG << "DVDManager::Next(" << nSkipLevel << ") at Stop State" << LOG_ENDL;
        Play();
    }
    else if(GetCurrentScreenState() != PLAYER_PLAY_DISPLAY) {
        // { modified by cychoi 2013.09.04 for ITS 186733
        if(m_tStatus.m_nTrackChapterNo == 0 ||
           m_tStatus.m_nTrackChapterNo == 0xFFF)
        {
            MC_LOG << "DVDManager::Next() RETURN ON NON-PLAY MODE(SCREEN " << GetCurrentScreenState() << ")" << LOG_ENDL;
            emit OnError(PLAYER_COMMANDNOTSUPPORTED);
            return;
        }
        // } modified by cychoi 2013.09.04
    }
    else {
        MC_LOG << "DVDManager::Next(" << nSkipLevel << ")" << LOG_ENDL;
    }

// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifndef DVD_NEW_NAVIGATION_ENABLED
    int nCurrntChapter = m_tStatus.m_nTrackChapterNo;
    int nTotalChapter  = m_tStatus.m_nTotalChapterNo;

    DECK_MEDIA_MODE eMediaMode = DeckManager::GetInstance()->GetMediaMode();

// 1. Play the first CHAPTER of the first TITLE if the last CHAPTER of the last TITLE.
// 2. Play the first CHAPTER of the next TITLE if the last CHAPTER.
    // TITLE MENU
    if(eMediaMode == eDM_MODE_DVD_VIDEO) {
        if(nCurrntChapter == nTotalChapter && m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_PLAY) {
            MC_LOG << "DVDManager::Next(" << nSkipLevel << "): It is last chapter(" << nCurrntChapter << "/" << nTotalChapter << "). Move to TitleMenu" << LOG_ENDL;
            Titlemenu(1);
            m_bAutoTrackChanged = false; // added by cychoi 20103.07.05 for DQA sound mute on first prev after inserted VCD Disc
            m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
            return;
        }
    }
#endif
// } modified by cychoi 2013.06.30
    
    if(nSkipLevel <= 1) { 
        m_pPacket->TrackUp(eDM_NEXT_CHAPTER_BEYOND_TITLE);

        emit OnTrackBufferred(false, -1, -1, m_tStatus.m_nTotalChapterNo); // added by cychoi 20103.07.20 for new OSD display
    }
    else
    {
        BCD4                tBCD4;
        BCD2                tBCD2;
        DirectContCommand   tCmd;
        // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
        int                 nNewTrackIdx = GetNextTrackIdx(nSkipLevel, GetCurrentIndex());
        // } modified by cychoi 2013-07-10

        tCmd.nFileDesign    = 0;
        tCmd.nPGPLDesggn    = 0;
        tCmd.nUndefined1    = 0;
        tCmd.nUndefined2    = 0;

        // TITLE/GROUP
        tBCD4.word          = DEC2BCD4(m_tStatus.m_nGrpTitlePGPLNo);
        tBCD2.BCD.upper     = tBCD4.BCD.upper2;
        tBCD2.BCD.lower     = tBCD4.BCD.lower2;
        
        tCmd.n1stCateUpper  = tBCD4.BCD.lower1;
        tCmd.n1stCateLower  = tBCD2.byte;

        // CHAPTER/TRACK
        tBCD4.word          = DEC2BCD4(nNewTrackIdx + 1);
        tBCD2.BCD.upper     = tBCD4.BCD.upper2;
        tBCD2.BCD.lower     = tBCD4.BCD.lower2;

        tCmd.n2ndCateUpper  = tBCD4.BCD.lower1;
        tCmd.n2ndCateLower  = tBCD2.byte;

        m_pPacket->DirectCont(&tCmd);

        emit OnTrackBufferred(false, -1, -1, m_tStatus.m_nTotalChapterNo); // modified by cychoi 20103.10.21 for Should not send next track to AVP before track change // added by cychoi 20103.07.20 for new OSD display
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    m_bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute

// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    StartRetryPlaybackTimer(7500); // DVD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-16
}

void DVDManager::Prev(int nSkipLevel, int nForced)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't prev operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
// 1. Go back to the beginning of the current CHAPTER if the first CHAPTER of the first TITLE.
// 2. Play the first CHAPTER of the previous TITLE if the first CHAPTER.
#ifndef DVD_NEW_NAVIGATION_ENABLED
    if(GetCurrentMediaState() == PLAYER_SEARCH) {
        MC_LOG << "DVDManager::Prev() RETURN ON SEARCH MODE" << LOG_ENDL;
        // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
        int nNewTrackIdx = GetPrevTrackIdx(nSkipLevel, m_tUserReqTrackList.count() == 0 ? GetCurrentIndex() : m_tUserReqTrackList.count());
        // } modified by cychoi 2013-07-10
        m_tUserReqTrackList.append(nNewTrackIdx);
        emit OnTrackBufferred(true, nNewTrackIdx, nNewTrackIdx, m_tStatus.m_nTotalChapterNo);
        // { added by cychoi 2014.01.06 for ITS 217985
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
        m_bRealSoundChanged = false;
        // } added by cychoi 2014.01.06
        return;
    }
#endif
// } modified by cychoi 2013.06.30
    
// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-16

    if(GetCurrentMediaState() == PLAYER_STOP) {
        MC_LOG << "DVDManager::Prev(" << nSkipLevel << ", " << nForced << ") at Stop State" << LOG_ENDL;
        Play();
    }
    else if(GetCurrentScreenState() != PLAYER_PLAY_DISPLAY) {
        // { modified by cychoi 2013.09.04 for ITS 186733
        if(m_tStatus.m_nTrackChapterNo == 0 ||
           m_tStatus.m_nTrackChapterNo == 0xFFF)
        {
            MC_LOG << "DVDManager::Prev() RETURN ON NON-PLAY MODE(SCREEN " << GetCurrentScreenState() << ")" << LOG_ENDL;
            emit OnError(PLAYER_COMMANDNOTSUPPORTED);
            return;
        }
        else
        {
            nForced = 0;
        }
        // } modified by cychoi 2013.09.04
    }
    else {
        MC_LOG << "DVDManager::Prev(" << nSkipLevel << ", " << nForced << ")" << LOG_ENDL;
    }

    int nCurrntChapter = m_tStatus.m_nTrackChapterNo;
// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifdef DVD_NEW_NAVIGATION_ENABLED
    int nCurrentGrpTitlePGPLNo = m_tStatus.m_nTotalGrpTlePGPLNo;
#endif
// } modified by cychoi 2013.06.30
    
    DECK_MEDIA_MODE eMediaMode = DeckManager::GetInstance()->GetMediaMode();
    MEDIA_PLAYER_STATE ePlayerState = (MEDIA_PLAYER_STATE)DeckManager::GetInstance()->PlayCondToPlayerState(m_tStatus.m_nPlayStatus);
    // ISV 62687
    if(eMediaMode == eDM_MODE_DVD_VIDEO && ePlayerState != eMP_STATE_SEARCHING) {
        // { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifdef DVD_NEW_NAVIGATION_ENABLED
        // Force to set the current position to 0 only if the first CHAPTER of the first TITLE
        if(nCurrentGrpTitlePGPLNo == 1 && nCurrntChapter == 1) {
#else
        if(nCurrntChapter == 1) {
#endif
// } modified by cychoi 2013.06.30
            SetCurrentMediaPosition(0);
            m_bAutoTrackChanged = false; // added by cychoi 20103.07.05 for DQA sound mute on first prev after inserted VCD Disc
            m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
            return;
        }
    }

    // { modified by kihyung 2013.3.12 for ISV 62685
    // DVD-Video는 3초 재한 없음.
    // if(nSkipLevel <= 1 && eMediaMode == eDM_MODE_DVD_AUDIO && nForced == 0) {
    if(nSkipLevel <= 1 && nForced == 0) {
    // } modified by kihyung 2013.3.12 for ISV 62685 
        m_pPacket->TrackDown(eDM_PREV_CHAPTER_BEYOND_TITLE);        

        emit OnTrackBufferred(true, -1, -1, m_tStatus.m_nTotalChapterNo); // added by cychoi 20103.07.20 for new OSD display
    }
    else 
    {
        // { modified by cychoi 20103.11.09 for ITS 207896 Flick previous track consecutively
        if(GetCurrentMediaState() == PLAYER_SEARCH)
        {
            m_pPacket->TrackDown(eDM_PREV_CHAPTER_BEYOND_TITLE);        
            
            emit OnTrackBufferred(true, -1, -1, m_tStatus.m_nTotalChapterNo);
        }
        else
        {
            BCD4                tBCD4;
            BCD2                tBCD2;
            DirectContCommand   tCmd;
            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
            int                 nNewTrackIdx = GetPrevTrackIdx(nSkipLevel, GetCurrentIndex());
            // } modified by cychoi 2013-07-10

            tCmd.nFileDesign    = 0;
            tCmd.nPGPLDesggn    = 0;
            tCmd.nUndefined1    = 0;
            tCmd.nUndefined2    = 0;

            // TITLE/GROUP
            tBCD4.word          = DEC2BCD4(m_tStatus.m_nGrpTitlePGPLNo);
            tBCD2.BCD.upper     = tBCD4.BCD.upper2;
            tBCD2.BCD.lower     = tBCD4.BCD.lower2;
            
            tCmd.n1stCateUpper  = tBCD4.BCD.lower1;
            tCmd.n1stCateLower  = tBCD2.byte;

            // CHAPTER/TRACK
            tBCD4.word          = DEC2BCD4(nNewTrackIdx + 1);
            tBCD2.BCD.upper     = tBCD4.BCD.upper2;
            tBCD2.BCD.lower     = tBCD4.BCD.lower2;

            tCmd.n2ndCateUpper  = tBCD4.BCD.lower1;
            tCmd.n2ndCateLower  = tBCD2.byte;

            MC_LOG << "prev to title: " << m_tStatus.m_nGrpTitlePGPLNo << ", chapter: " << m_tStatus.m_nTrackChapterNo << LOG_ENDL;

            m_pPacket->DirectCont(&tCmd);

            emit OnTrackBufferred(true, -1, -1, m_tStatus.m_nTotalChapterNo); // modified by cychoi 20103.10.21 for Should not send next track to AVP before track change // added by cychoi 20103.07.20 for new OSD display
        }
        // } modified by cychoi 20103.11.09
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    m_bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute

// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    StartRetryPlaybackTimer(7500); // DVD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-16
}

void DVDManager::Scan(int nMode)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't scan operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    MC_LOG << "DVDManager::Scan(" << nMode << ")" << LOG_ENDL;
    
    switch(nMode)
    {
    case SCANALL:
        // RANDOM MODE ---> RANDOM OFF
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }
        m_pPacket->ScanOn(eDM_SCAN_CHAPTER_IN_TITLE);
        break;
    case SCANFOLDER:
        MC_LOG << "DVDManager::Scan() SCANFOLDER is invalid option" << LOG_ENDL;
        break;
    case SCANFILE:
        MC_LOG << "DVDManager::Scan() SCANFILE is invalid option" << LOG_ENDL;
        break;
    default:
        m_pPacket->ScanOff();
        break;
    }

    // { added by cychoi 2013.10.01 for ITS 192388
    if(nMode == SCANALL)
    {
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
    }
    // } added by cychoi 2013.10.01
}

void DVDManager::RandomPlay(int nMode, int bPlay)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't random play operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    Q_UNUSED(bPlay);
    
    MC_LOG << "DVDManager::RandomPlay(" << nMode << ")" << LOG_ENDL;
    
    if(DeckManager::GetInstance()->GetMediaMode() == eDM_MODE_DVD_AUDIO) 
    {
        int nPrevMode = GetRandomMode(m_tStatus.m_nRandomMode);

        if(nPrevMode != nMode) 
        {
            MC_LOG << "DVDManager::RandomPlay(): nMode(" << nMode << ") != nPrevMode(" << nPrevMode << ")" << LOG_ENDL;
            
            switch(nMode)
            {
            case RANDOMFILE:
                m_tStatus.m_nRandomMode = eDM_DVD_RANDOM_CHAPTER_IN_TITLE;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef DVD_DECK_REQ_RETRY_ENABLED
                m_nRetryPlaybackRandomMode = m_tStatus.m_nRandomMode;
#endif
// } modified by cychoi 2013.06.20
                break;
            case RANDOMFOLDER:
                MC_LOG << "DVDManager::RandomPlay() RANDOMFOLDER is invalid option" << LOG_ENDL;
                return;
            default:
                m_nUserRandomMode       = -1;
                m_tStatus.m_nRandomMode = eDM_DVD_RANDOM_OFF;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef DVD_DECK_REQ_RETRY_ENABLED
                m_nRetryPlaybackRandomMode = m_tStatus.m_nRandomMode;
#endif
// } modified by cychoi 2013.06.20
                RandomNotice(nMode);
                emit OnModeChanged(nMode);
                // { added by cychoi 2013.10.01 for ITS 192388
                if(GetCurrentMediaState() == PLAYER_PAUSE)
                {
                    m_bAutoTrackChanged = false;
                    m_bUserTrackChanged = true;
                }
                // } added by cychoi 2013.10.01
                return;
            }
            
            m_nUserRandomMode = nMode;

            if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFILE) {
                RepeatPlay(REPEATALL);
            }
            if(GetScanMode(m_tStatus.m_nScanMode) != SCANOFF) {
                Scan(SCANOFF);
            }
        
            emit OnModeChanged(m_nUserRandomMode);

            // Spec 1. cancel pause mode when selecting repeat on, random on and scan on. 
            // Spec 2. keep pause mode when selecting repeat off, random off, scan off.
            // { added by kihyung 2012.07.11 for CR 5502
            if(GetCurrentMediaState() == PLAYER_PAUSE && m_nUserRandomMode != RANDOMOFF) {
                Play();
            }
            // } added by kihyung

            // { added by cychoi 2013.10.01 for ITS 192388
            if(GetCurrentMediaState() == PLAYER_PAUSE)
            {
                m_bAutoTrackChanged = false;
                m_bUserTrackChanged = true;
            }
            // } added by cychoi 2013.10.01
        }
        else
        {
            MC_LOG << "DVDManager::RandomPlay(): nMode(" << nMode << ") != nPrevMode(" << nPrevMode << ")" << LOG_ENDL;
        }
    }
 }

void DVDManager::RepeatPlay(int nMode, int bPlay)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't repeat play operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    Q_UNUSED(bPlay);
    
    MC_LOG << "DVDManager::RepeatPlay(" << nMode << ")" << LOG_ENDL;
    
    switch(nMode)
    {
    case REPEATFILE:
        // RANDOM MODE ---> RANDOM OFF
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }
        m_pPacket->RepeatOn(eDM_REPEAT_CHAPTER);
        break;
    case REPEATFOLDER:
        m_pPacket->RepeatOn(eDM_REPEAT_TITLE);
        break;
    default:
        m_pPacket->RepeatOff();
        break;
    }

    m_nUserRepeatMode = nMode;

    // { added by cychoi 2013.10.01 for ITS 192388
    if(GetCurrentMediaState() == PLAYER_PAUSE)
    {
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
    }
    // } added by cychoi 2013.10.01
}

void DVDManager::FastForward(int nRate)
{
    DECK_FAST_SPEED eSpeed;
    
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't FF operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    if(nRate == FAST_SPEED_X4) {
        eSpeed = eDM_FAST_SPEED_X4;
    }
    else if(nRate == FAST_SPEED_X16) {
        eSpeed = eDM_FAST_SPEED_X20;
    }
    else if(nRate == FAST_SPEED_X20) {
        eSpeed = eDM_FAST_SPEED_X20;
    }
    else {
        m_pPacket->Release();
        m_nFFREWRate = -1; // added by cychoi 20103.06.17 for ITS 174004
        m_nFFREWState = PLAYER_PLAY; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
        m_bFFREWStateSigSend = false; // added by sjhyun 2013.09.14 for ITS 189282
        return;
    }

    m_pPacket->FastUp(eSpeed);

    m_bREWMode = false;

    if(m_nFFREWRate != (int)eSpeed) 
    {
        m_nFFREWRate = (int)eSpeed;
        m_bFFREWStateSigSend = (nRate > FAST_SPEED_X4); // modified by cychoi 2013.10.27 for ITS 198856 // added by sjhyun 2013.09.14 for ITS 189282
        
        // { modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
        // { commented by cychoi 2013-06-07 for ITS 172685
        if(nRate == FAST_SPEED_X4) {
            m_nFFREWState = PLAYER_FASTFORWARD_X4;
        }
        else if(nRate == FAST_SPEED_X16) {
            m_nFFREWState = PLAYER_FASTFORWARD_X16;
        }
        else if(nRate == FAST_SPEED_X20) {
            m_nFFREWState = PLAYER_FASTFORWARD_X20;
        }
        // } commented by cychoi 2013-06-07
        // } modified by cychoi 2013.09.22
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    m_bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
}

void DVDManager::Rewind(int nRate)
{
    DECK_FAST_SPEED eSpeed;

    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't REW operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    if(nRate == FAST_SPEED_X4) {
        eSpeed = eDM_FAST_SPEED_X4;
    }
    else if(nRate == FAST_SPEED_X16) {
        eSpeed = eDM_FAST_SPEED_X20;
    }
    else if(nRate == FAST_SPEED_X20) {
        eSpeed = eDM_FAST_SPEED_X20;
    }
    else {
        m_pPacket->Release();
        m_nFFREWRate = -1; // added by cychoi 20103.06.17 for ITS 174004
        m_nFFREWState = PLAYER_PLAY; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
        m_bFFREWStateSigSend = false; // added by sjhyun 2013.09.14 for ITS 189282
        return;
    }

    m_pPacket->FastDown(eSpeed);

    m_bREWMode = true;

    if(m_nFFREWRate != (int)eSpeed) 
    {
        m_nFFREWRate = (int)eSpeed;
        m_bFFREWStateSigSend = (nRate > FAST_SPEED_X4); // modified by cychoi 2013.10.27 for ITS 198856 // added by sjhyun 2013.09.14 for ITS 189282
        
        // { modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
        // { commented by cychoi 2013-06-07 for ITS 172685
        if(nRate == FAST_SPEED_X4) {
            m_nFFREWState = PLAYER_REWIND_X4;
        }
        else if(nRate == FAST_SPEED_X16) {
            m_nFFREWState = PLAYER_REWIND_X16;
        }
        else if(nRate == FAST_SPEED_X20) {
            m_nFFREWState = PLAYER_REWIND_X20;
        }
        // } commented by cychoi 2013-06-07
        // } modified by cychoi 2013.09.22
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    m_bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
}

void DVDManager::NextFolder(PlayList *pPlayList)
{
    Q_UNUSED(pPlayList);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void DVDManager::PrevFolder(PlayList *pPlayList)
{
    Q_UNUSED(pPlayList);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
void DVDManager::HandleRetryVRInfo()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request toc info status after disc detection is completed. */
        return;
    }

    MC_HIGH << "DVDManager::HandleRetryVRInfo()" << LOG_ENDL;

    m_tRetryVRInfoTimer.stop();

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eVRInfoFlag == DVD_DECK_REQ_SENT)
    {
#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
        if(HandleErrorDisc(false) == true)
        {
            return;
        }
#endif
        // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
        m_pPacket->RequestDVDAudioVideoVRStatus();
        /* We will retry 3 times if we will do not receive the response on retry. */
#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
        StartRetryVRInfoTimer(200);
#endif
        // } modified by cychoi 2014.04.25
    }
    // } modified by cychoi 2013.09.06
}

void DVDManager::HandleRetryPlayback()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request playback after disc detection is completed. */
        return;
    }

    MC_HIGH << "DVDManager::HandleRetryPlayback(): m_nRetryPlaybackMediaState=" << m_nRetryPlaybackMediaState << LOG_ENDL;

    m_tRetryPlaybackTimer.stop();
        
#ifdef DVD_NAVIGATION_STILL_PICTURE_RETRY_DISABLED
    if(m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_PLAY)
    {
        if(GetCurrentMediaState() == PLAYER_PLAY &&
           m_nStartPlaybackTime == m_tStatus.m_nPlaybackTime)
        {
            /* If Still Picture, just return. */
            return;
        }
    }
#endif

#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
    if(HandleErrorDisc(true) == true)
    {
        return;
    }
#endif

    m_ePlayerState = m_nRetryPlaybackMediaState;
    //m_nRetryPlaybackMediaState = eMP_STATE_NONE; // commented by cychoi 2013.06.27 for Retry on SW reset
    int mediaState = GetCurrentMediaState();
    m_ePlayerState = eMP_STATE_NONE; // added by cychoi 2013.07.16 for Retry on SW reset
    switch(mediaState)
    {
    // { modified by cychoi 2013.08.09 for ITS 181303 Retry on Search Mode
    case PLAYER_PLAY:
        Stop();
        Play();
        break;
    case PLAYER_SEARCH:
        Play();
        break;
    // } modified by cychoi 2013.08.09
    case PLAYER_PAUSE:
        Stop();
        Pause();
        break;
    case PLAYER_FASTFORWARD:
        // TODO. Please check the below code
        Stop();
        Play();
        if(m_nFFREWRate == eDM_FAST_SPEED_X20)
        {
            FastForward(FAST_SPEED_X20);
        }
        else
        {
            FastForward(FAST_SPEED_X4);
        }
        break;
    case PLAYER_REWIND:
        // TODO. Please check the below code
        Stop();
        Play();
        if(m_nFFREWRate == eDM_FAST_SPEED_X20)
        {
            Rewind(FAST_SPEED_X20);
        }
        else
        {
            Rewind(FAST_SPEED_X4);
        }
        break;
    case PLAYER_STOP:
    default:
        // { commented by cychoi 2013.06.22 for SW reset on disc loading
        //Play();
        //InitRetryPlayback();
        // } commented by cychoi 2013.06.22
        break;
    }

    // TODO. Please check the below code
    if(mediaState == PLAYER_PLAY ||
       mediaState == PLAYER_SEARCH ||
       mediaState == PLAYER_FASTFORWARD ||
       mediaState == PLAYER_REWIND)
    {
        // removed by cychoi 2014.02.03 for play mode on SW Reset

#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
        if(m_tStatus.m_nPlaybackTime != 0)
        {
            InitRetryPlayback();
            InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
        }
#else
        InitRetryPlayback();
        InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
#endif
    }
}
#endif
// } added by cychoi 2013-06-05

#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
void DVDManager::HandleStillPicture()
{
    m_tStillPictureTimer.stop();

    MC_HIGH << "DVDManager::HandleStillPicture(): GetCurrentMediaState()=" << GetCurrentMediaState() << LOG_ENDL;

    if(m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_PLAY)
    {
        if(GetCurrentMediaState() == PLAYER_PLAY &&
           m_nStartPlaybackTime == m_tStatus.m_nPlaybackTime)
        {
            emit OnStateChanged(PLAYER_PAUSE);
            m_nStillPictureState = PLAYER_PAUSE;
            MC_HIGH << "DVDManager::HandleStillPicture(): PLAYER_PAUSE" << LOG_ENDL;
        }
    }
}
#endif

DVDManager::DVDManager(QObject *parent) : QObject(parent),
m_bLoadComplete(false),    
m_pPacket(0),
m_nSystemState(0),
m_nFFREWRate(-1),
m_nFFREWState(PLAYER_PLAY), // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error // added by cychoi 2013-06-07 for ITS 172685
m_bFFREWStateSigSend(false), // added by sjhyun 2013.09.14 for ITS 189282
//m_bFirstBoot(false), // commented by cychoi 20130.07.11 for keeping playback options
m_bControllerChanged(false),
// { modified by cychoi 2013.06.22 for SW reset on disc loading
#ifdef DVD_DECK_REQ_RETRY_ENABLED
m_nRetryPlaybackMediaState(eMP_STATE_NONE),
m_nRetryPlaybackRandomMode(eDM_DVD_RANDOM_OFF),
m_nRetryPlaybackRepeatMode(eDM_DVD_REPEAT_OFF),
m_nRetryPlaybackScanMode(eDM_DVD_SCAN_OFF),
#endif
// } modified by cychoi 2013.06.22
m_bAutoTrackChanged(true), // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
m_bUserTrackChanged(false), // added by cychoi 20103.07.22 for OnUserTrackChanged signal
m_bAudioSwitched(false), // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
m_bRealSoundChanged(false), // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
m_bBTCallStart(false) // added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
{
    Init();
    m_pPacket = Packet::GetInstace();

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    connect(&m_tRetryVRInfoTimer, SIGNAL(timeout()), this, SLOT(HandleRetryVRInfo())); 
    connect(&m_tRetryPlaybackTimer, SIGNAL(timeout()), this, SLOT(HandleRetryPlayback())); 
#endif
// } added by cychoi 2013-06-05
#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
    m_tStillPictureTimer.setSingleShot(true);
    connect(&m_tStillPictureTimer, SIGNAL(timeout()), this, SLOT(HandleStillPicture()));
#endif

    m_nReqSwitchAudio = 0; // added by cychoi 2013.10.25 for ITS 195688 Audio repeated request
}

void DVDManager::RandomNotice(int nMode)
{
    if(DeckManager::GetInstance()->GetMediaMode() == eDM_MODE_DVD_AUDIO) 
    {
        switch(nMode)
        {
        case RANDOMFILE:
            m_pPacket->RandomOn(eDM_RANDOM_TRACK_IN_GROUP);
            break;
        case RANDOMFOLDER:
            m_pPacket->RandomOn(eDM_RANDOM_GROUP);
            break;
        default:
            m_pPacket->RandomOff();
            break;
        }
    }
}

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
#if _DECK_DIAGNOSTIC_
bool DVDManager::ExtractResetStart(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_RESET_START) return false;

    MC_HIGH << "DVDManager::ExtractResetStart: m_nRetryPlaybackMediaState=" << m_nRetryPlaybackMediaState << LOG_ENDL;

// { modified by cychoi 2014.04.14 for HW Event PQ launch DVD mode unconditionally after Deck SW reset
#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
    //m_nDiscDetectRetryCount = 0;
    //m_nDiscPlayRetryCount = 0;
#endif
// } modified by cychoi 2014.04.14

    // { modified by cychoi 2013.08.09 for ITS 181303 Retry on Search Mode
    if(m_nRetryPlaybackMediaState == eMP_STATE_SEARCHING)
    {
        m_nRetryPlaybackMediaState = eMP_STATE_PLAYING;
    }
    // } modified by cychoi 2013.08.09
    StartRetryPlaybackTimer(500);
    // removed by cychoi 2013.06.20 for SW reset on disc loading

    // { modified by cychoi 2013.11.25 for ITS 210845
    // { added by cychoi 2013.09.06 for ITS 178797
    if(m_eVRInfoFlag == DVD_DECK_REQ_SENT)
    {
        StartRetryVRInfoTimer(400); // delay 400ms
    }
    // } added by cychoi 2013.09.06
    // } modified by cychoi 2013.11.25

    return true;
}
#endif
#endif // DVD_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05

bool DVDManager::ExtractDVDStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_DVD_AUDIOVIDEOVR_STATUS) return false;

    // MC_LOG << "DVDManager::ExtractDVDStatus" << LOG_ENDL;

    DVDAudioVideoVRStatus *pStatus = &pPacket->m_tData.m_tDVDAudioVideoVR;
    int bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute

#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
    InitStillPictureTimer();
#endif

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    InitRetryVRInfo();
    m_eVRInfoFlag = DVD_DECK_REQ_DONE; // added by cychoi 2013.09.06 for ITS 178797
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-05
#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
    m_nDiscPlayRetryCount = 0;
#endif

    // { modified by cychoi 2013.12.26 for ITS 217306 DVD black
    // { added by cychoi 2013.10.06 for ITS 193952 Sync Stop State // { added by cychoi 2013.09.11 for ITS 188786
    if((MediaManager::GetInstance()->GetPrevMediaMode() == MP_MODE_DISC &&
        MediaManager::GetInstance()->GetMediaMode() != MP_MODE_DISC) ||
       (m_eLastPlayerState == eMP_STATE_STOPPED))
    {
        int nPrevState = GetInternalMediaState();
        if(nPrevState == PLAYER_STOP)
        {
            return true;
        }

        if(m_eLastPlayerState == eMP_STATE_STOPPED)
        {
            m_tStatus.m_nPlayStatus = eDM_CD_PLAY_COND_HALTING;
        }
        else
        {
            m_tStatus.m_nPlayStatus = (DECK_CD_PLAY_COND)pStatus->nPlayStatus;
        }
        m_ePlayerState = (MEDIA_PLAYER_STATE)DeckManager::GetInstance()->PlayCondToPlayerState(m_tStatus.m_nPlayStatus);
#ifdef DVD_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackMediaState = m_ePlayerState;
#endif
        DeckManager::GetInstance()->ControlGPlayerEngine(GetCurrentMediaState(), nPrevState);
        return true;
    }
    // } added by cychoi 2013.10.06 // } added by cychoi 20103.09.11
    // } modified by cychoi 2013.12.26

    ////////////////////////////////////////////////////////
    // 상태 변경 알림.
    int                 bStartTime  = false;
    DECK_CD_PLAY_COND   ePlayStatus = (DECK_CD_PLAY_COND)pStatus->nPlayStatus;
    int                 nPrevState = GetCurrentMediaState(); // added by cychoi 2013.10.11 for ITS 195154 No OSD if operation is forbidden
    if(ePlayStatus != m_tStatus.m_nPlayStatus) {
        MEDIA_PLAYER_STATE ePlayerState = (MEDIA_PLAYER_STATE)DeckManager::GetInstance()->PlayCondToPlayerState(ePlayStatus);

        if(ePlayerState != m_ePlayerState) {
            if(m_ePlayerState != eMP_STATE_PLAYING && ePlayerState == eMP_STATE_PLAYING) bStartTime = true;
            //int nPrevState = GetCurrentMediaState(); // commented by cychoi 2013.10.11 for ITS 195154 No OSD if operation is forbidden
            m_ePlayerState = ePlayerState;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef DVD_DECK_REQ_RETRY_ENABLED
            m_nRetryPlaybackMediaState = m_ePlayerState;
#endif
// } modified by cychoi 2013.06.20
            DeckManager::GetInstance()->ControlGPlayerEngine(GetCurrentMediaState(), nPrevState);
// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifndef DVD_NEW_NAVIGATION_ENABLED
            if(m_tUserReqTrackList.count() > 0 && ePlayerState == eMP_STATE_PLAYING) {
                int nNewTrack = m_tUserReqTrackList.takeLast();
                m_tUserReqTrackList.clear();
                // { modified by cychoi 20103.08.23 for ITS 185414 ID3 Tag Display
                if(m_nPlayTrackNo == (nNewTrack + 1))
                {
                    emit OnStateChanged(GetCurrentMediaState());
                    // { modified by cychoi 20103.09.01 for ITS 183910 & 183912 Sound mute if DVD Audio
                    if(m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_PLAY)
                    {
                        // { added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
                        if(ePlayerState == eMP_STATE_PLAYING && // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
                           m_bAutoTrackChanged == false)
                        {
                            MC_HIGH << "DVDManager::ExtractDVDStatus(): emit OnRealSoundChanged() [1]" << LOG_ENDL;
                            // { modified by cychoi 2014.04.14 for HW Event DV2-1 DVD sound truncated
                            //bRealSoundChanged = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                            emit OnRealSoundChanged(); // commented by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                            // } modified by cychoi 2014.04.14
                            m_bAutoTrackChanged = true;
                        }
                        // } added by cychoi 20103.06.13
                    }
                    // } modified by cychoi 20103.09.01
                }
                else
                {
                    MC_LOG << "DVDManager::ExtractDVDStatus(): nCurrTrack: " << GetCurrentIndex() << ", nNewTrack: " << nNewTrack << LOG_ENDL;
                    SetCurrentIndex(nNewTrack);
                    Play();
                    // { added by cychoi 20103.07.25 for ITS 181748
                    m_bAutoTrackChanged = false;
                    m_bUserTrackChanged = true;
                    // } added by cychoi 20103.07.25
                    m_bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                }
                // } modified by cychoi 20103.08.23 
            }
            else {
#endif
// } modified by cychoi 2013.06.30
                // { rollback by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
                // { added by sjhyun 2013.09.14 for ITS 189282
                //if(ePlayerState != eMP_STATE_SEARCHING || m_nFFREWRate == -1)
                {
                    emit OnStateChanged(GetCurrentMediaState());
                }
                // } rollback by cychoi 2013.09.22
                // } added by sjhyun 2013.09.14
                // { modified by cychoi 20103.09.01 for ITS 183910 & 183912 Sound mute if DVD Audio
                if(m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_PLAY)
                {
                    // { added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
                    if(ePlayerState == eMP_STATE_PLAYING && // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
                       m_bAutoTrackChanged == false)
                    {
                        MC_HIGH << "DVDManager::ExtractDVDStatus(): emit OnRealSoundChanged() [2]" << LOG_ENDL;
                        // { modified by cychoi 2014.04.14 for HW Event DV2-1 DVD sound truncated
                        //bRealSoundChanged = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                        emit OnRealSoundChanged(); // commented by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                        // } modified by cychoi 2014.04.14
                        m_bAutoTrackChanged = true;
                    }
                    // } added by cychoi 20103.06.13
                }
                // } modified by cychoi 20103.09.01
// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifndef DVD_NEW_NAVIGATION_ENABLED
            }
#endif
// } modified by cychoi 2013.06.30

            m_bFFREWStateSigSend = (m_ePlayerState == eMP_STATE_FAST_PLAYING); // added by cychoi 2013.10.27 for ITS 198856 
        }

        // { added by cychoi 20103.06.17 for ITS 174004
        if((eDM_CD_PLAY_COND_PLAYING_FAST == m_tStatus.m_nPlayStatus ||
            eDM_CD_PLAY_COND_SEARCHING_IN == m_tStatus.m_nPlayStatus) && 
           eDM_CD_PLAY_COND_PLAYING == ePlayStatus &&
           m_bREWMode == true && // modified by sjhyun 2013.09.14 for ITS 189282
           m_nFFREWRate != -1) {
        //if(eDM_CD_PLAY_COND_PLAYING_FAST == m_tStatus.m_nPlayStatus && 
        //   eDM_CD_PLAY_COND_PLAYING_FAST != ePlayStatus) {
        // } added by cychoi 20103.06.17
            // { modified by cychoi 2013.11.26 for ITS 210745
            // { modified by cychoi 2013.11.07 for SmokeTest DISC cancel REW on reach first chapter
            m_nFFREWRate = -1;
            m_nFFREWState = PLAYER_PLAY; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
            m_bFFREWStateSigSend = false; // added by cychoi 2013.10.27 for ITS 198856 
            OnNormalPlay(); // added by cychoi 2013-05-24 Added additional signal for FF/Rew functionality.
            // } modified by cychoi 2013.11.07
            // } modified by cychoi 2013.11.26
        }
        // { modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
        // { added by sjhyun 2013.09.14 for ITS 189282
        else if(eDM_CD_PLAY_COND_PLAYING_FAST == m_tStatus.m_nPlayStatus &&
                eDM_CD_PLAY_COND_SEARCHING_IN == ePlayStatus &&
                m_nFFREWRate != -1)
        {
            m_bFFREWStateSigSend = true;
        }
        // } added by sjhyun 2013.09.14
        // } modified by cychoi 2013.09.22
    }
    // { added by cychoi 20103.07.24 for Seek Up/Down & FF/REW mute
    else
    {
        // { modified by cychoi 2013.09.10 for ITS 189300 Sound mute on REW // 20103.09.01 for ITS 183910 & 183912 Sound mute if DVD Audio
        if(m_nFFREWState == PLAYER_PLAY) // modified by cychoi 20103.09.26 for ITS 188493 DVD Navigation Still Picture
        {
            MEDIA_PLAYER_STATE ePlayerState = (MEDIA_PLAYER_STATE)DeckManager::GetInstance()->PlayCondToPlayerState(ePlayStatus);
            if(ePlayerState == eMP_STATE_PLAYING && 
               m_bAutoTrackChanged == false)
            {
                MC_HIGH << "DVDManager::ExtractDVDStatus(): bRealSoundChanged = true [3]" << LOG_ENDL;
                bRealSoundChanged = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                //emit OnRealSoundChanged(); // commented by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                m_bAutoTrackChanged = true;
            }
        }
        // } modified by cychoi 2013.09.10 // 20103.09.01

#ifdef DVD_NAVIGATION_STILL_PICTURE_RETRY_DISABLED
        if(DeckManager::GetInstance()->GetDVDMediaMode() == eDM_MODE_DVD_AUDIO)
        {
            m_bEnterKey = false;
        }
#endif
    }
    // } added by cychoi 20103.07.24

    // { modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
    // { added by cychoi 2013-06-07 for ITS 172685
    if(ePlayStatus == eDM_CD_PLAY_COND_PLAYING_FAST && // modified by cychoi 2013.11.29 for SmokeTest 16x icon display on DVD normal play
       m_nFFREWState != PLAYER_PLAY && m_bFFREWStateSigSend == true) { // modified by sjhyun 2013.09.14 for ITS 189282
        emit OnStateChanged(m_nFFREWState);
        // { modified by sjhyun 2013.09.14 for ITS 189282
        m_bFFREWStateSigSend = false;
        // } modified by sjhyun 2013.09.14
    }
    // } added by cychoi 2013-06-07
    // } modified by cychoi 2013.09.22

    m_tStatus.m_nPlayStatus     = ePlayStatus;
    m_tStatus.m_bPictureExist   = pStatus->nPicture;
    m_tStatus.m_nPGPL           = pStatus->nPGPL;
    m_tStatus.m_nContent        = (DECK_DVD_CONTENT)pStatus->nContents;
    m_tStatus.m_nDiscNo         = pStatus->nDiscNo;
    
    ////////////////////////////////////////////////////////
    // Title, Chapter 변경 알림.
    BCD2 tTitleNo;
    BCD2 tChapterNo;
    tTitleNo.byte   = pStatus->nGroupTitlePGPLNo;
    tChapterNo.byte = pStatus->nTrackChapterNo10_1;
    int  nTitleNo   = BCD2DEC2(pStatus->nGroupTitlePGPLNo);
    int  nChapterNo = BCD2DEC1(pStatus->nTrackChapterNo100)*100 + BCD2DEC2(pStatus->nTrackChapterNo10_1);
#ifdef DVD_NAVIGATION_STILL_PICTURE_RETRY_DISABLED
    int  bStillPictureStartTime = false;
    if(DeckManager::GetInstance()->GetDVDMediaMode() == eDM_MODE_DVD_VIDEO)
    {
        if(m_ePlayerState == eMP_STATE_PLAYING)
        {
            if(nPrevState == PLAYER_PLAY || nPrevState == PLAYER_SEARCH)
            {
                bStillPictureStartTime = bStartTime || m_bEnterKey;
                m_bEnterKey = false;
                //MC_LOG << "DVDManager::ExtractDVDStatus: bStillPictureStartTime1=" << bStillPictureStartTime << LOG_ENDL;
            }
        }
    }
#endif
    
    if((tTitleNo.BCD.lower != 0xF && tTitleNo.BCD.upper != 0xF) || 
       (pStatus->nTrackChapterNo100 != 0xF && tChapterNo.BCD.lower != 0xF && tChapterNo.BCD.upper != 0xF)) 
    {
        if(m_tStatus.m_nTrackChapterNo != nChapterNo || m_tStatus.m_nGrpTitlePGPLNo != nTitleNo || m_bControllerChanged == true) {
            //bool bTrackChanged = (bool)(m_tStatus.m_nTrackChapterNo != nChapterNo || m_tStatus.m_nGrpTitlePGPLNo != nTitleNo); // added by cychoi 2014.04.29 for ITS 233500
            if(m_nUserRandomMode >= 0 && m_tStatus.m_nPlayStatus != eDM_CD_PLAY_COND_PLAYING_FAST) {
                RandomNotice(m_nUserRandomMode);
                m_nUserRandomMode = -2;
            }
            else {
                if(nTitleNo != m_tStatus.m_nGrpTitlePGPLNo) {
                    // MC_LOG << "DVDManager::emit OnTitleGroupChanged(" << nTitleNo - 1 << ")" << LOG_ENDL;
                    emit OnTitleGroupChanged(nTitleNo - 1);
                }
                m_nPlayTrackNo              = nChapterNo;
                m_tStatus.m_nTrackChapterNo = nChapterNo;
                m_tStatus.m_nGrpTitlePGPLNo = nTitleNo;
                #if 0
                if(DeckManager::GetInstance()->GetMediaMode() == eDM_MODE_DVD_AUDIO) {
                    if(ePrevPlayerState != eMP_STATE_STOPPED && ePrevPlayerState != eMP_STATE_NONE) {
                        MC_LOG << "DVDManager::ePrevPlayerState: " << ePrevPlayerState << ". emit OnCurrentMediaStatusChanged()" << LOG_ENDL;
                        emit OnCurrentMediaStatusChanged(PLAYER_EndOfMedia);
                    }
                }
                #endif
                // IF FF/REW AND REPEATFILE ---> REPEATOFF
                if(m_ePlayerState == eMP_STATE_FAST_PLAYING && m_nUserRepeatMode == REPEATFILE) {
                    m_nUserRepeatMode = -1;
                    RepeatPlay(REPEATALL);
                }
                MC_LOG << "DVDManager::ExtractDVDStatus::OnTrackChanged(" << m_nPlayTrackNo - 1 << "). TITLE: " << nTitleNo << LOG_ENDL;
                m_bControllerChanged = false;
                // { added by cychoi 20103.07.22 for add new signal
                if(m_bUserTrackChanged == true)
                {
                    emit OnUserTrackChanged(m_nPlayTrackNo - 1);
                    m_bUserTrackChanged = false;
                }
                // } added by cychoi 20103.07.22
                // { added by cychoi 2014.04.29 for ITS 233500
                //else
                //{
                //    if(bTrackChanged == true)
                //    {
                //        emit OnAutoTrackChanged(m_nPlayTrackNo - 1);
                //    }
                //}
                // } added by cychoi 2014.04.29
                emit OnTrackChanged(m_nPlayTrackNo - 1);
                // { modified by cychoi 2014.05.15 for ITS 237445, ITS 237446 DVD black // { modified by cychoi 2014.04.14 for HW Event DV2-1 DVD sound truncated
                if(m_tStatus.m_nDisplayScreen != eDM_DISPLAY_SCREEN_PLAY)
                {
                    int nCurrState = GetCurrentMediaState();
                    if(nCurrState != PLAYER_STOP)
                    {
                        m_tStatus.m_nPlayStatus = eDM_CD_PLAY_COND_SEARCHING_OUT;
                        m_ePlayerState = eMP_STATE_SEARCHING;
#ifdef DVD_DECK_REQ_RETRY_ENABLED
                        m_nRetryPlaybackMediaState = m_ePlayerState;
#endif
                        DeckManager::GetInstance()->ControlGPlayerEngine(GetCurrentMediaState(), nCurrState);
                    }
                }
                // } modified by cychoi 2014.05.15 // } modified by cychoi 2014.04.14
                // removed by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
                m_nReqSwitchAudio = 0; // added by cychoi 2013.10.25 for ITS 195688 Audio repeated request
#ifdef DVD_NAVIGATION_STILL_PICTURE_RETRY_DISABLED
                if(DeckManager::GetInstance()->GetDVDMediaMode() == eDM_MODE_DVD_VIDEO)
                {
                    if(m_ePlayerState == eMP_STATE_PLAYING)
                    {
                        if(nPrevState == PLAYER_PLAY || nPrevState == PLAYER_SEARCH)
                        {
                            bStillPictureStartTime = true;
                            //MC_LOG << "DVDManager::ExtractDVDStatus: bStillPictureStartTime2=" << bStillPictureStartTime << LOG_ENDL;
                        }
                    }
                }
#endif
            }
        }
        // { added by cychoi 2013.10.11 for ITS 195154 No OSD if operation is forbidden
        else
        {
            if(nPrevState != PLAYER_SEARCH &&
               GetCurrentMediaState() == PLAYER_SEARCH) // modified by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
            {
                // Track donw: over 3 sec in current TRACK → current TRACK -> Send OnUserTrackChanged signal
                if(m_bUserTrackChanged == true)
                {
                    emit OnUserTrackChanged(m_nPlayTrackNo - 1);
                    m_bUserTrackChanged = false;
                }
                // { added by cychoi 2014.04.06 for ITS 233500
                //else
                //{
                //    emit OnAutoTrackChanged(m_nPlayTrackNo - 1);
                //}
                // } added by cychoi 2014.04.06
            }
        }
        // } added by cychoi 2013.10.11
    }
    // { modified by cychoi 2013.09.04 for ITS 186733
    else {
        m_tStatus.m_nTrackChapterNo = 0xFFF;
        m_tStatus.m_nGrpTitlePGPLNo = 0xFF;
    }
    // } modified by cychoi 2013.09.04

#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
    bool bStillPictureTimerStart = false;
#endif

    ////////////////////////////////////////////////////////
    // 재생 시간 변경 알림.
    if(pStatus->nPlaybackTimeHour10 != 0xF && pStatus->nPlaybackTimeHour1 != 0xF &&
       pStatus->nPlaybackTimeMin10  != 0xF && pStatus->nPlaybackTimeMin1  != 0xF) 
    {
        int nHour = BCD2DEC1(pStatus->nPlaybackTimeHour10) * 10 + BCD2DEC1(pStatus->nPlaybackTimeHour1);
        int nMin  = BCD2DEC1(pStatus->nPlaybackTimeMin10)  * 10 + BCD2DEC1(pStatus->nPlaybackTimeMin1);
        int nSec  = BCD2DEC1(pStatus->nPlaybackTimeSec10)  * 10 + BCD2DEC1(pStatus->nPlaybackTimeSec1);
        int nTime = (nHour * 3600 + nMin * 60 + nSec) * 1000;

        //MC_LOG << "DVDManager:: " << nHour << ":" << nMin, nSec << ":" << "->" << nTime << LOG_ENDL;
        
        if(m_tStatus.m_nPlaybackTime != nTime) {
#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
            if(m_nStillPictureState == PLAYER_PAUSE)
            {
                emit OnStateChanged(GetCurrentMediaState());
                m_nStillPictureState = -1;
            }
#endif
            int nPrevTime = m_tStatus.m_nPlaybackTime; // added by cychoi 20103.09.01 for ITS 183910 & 183912 Sound mute if DVD Audio
            if(bStartTime == true) DeckManager::GetInstance()->SetStartPlayTime(nTime);
            DeckManager::GetInstance()->SetLifeTime(nTime);
            m_tStatus.m_nPlaybackTime = nTime;
            emit OnPositionChanged(nTime);
            // { added by cychoi 20103.09.01 for ITS 183910 & 183912 Sound mute if DVD Audio
            if(m_bRealSoundChanged == true)
            {
                MC_HIGH << "DVDManager::ExtractDVDStatus(): emit OnRealSoundChanged() [4]" << LOG_ENDL;
                emit OnRealSoundChanged();
                m_bRealSoundChanged = false;
            }
            if(m_tStatus.m_nDisplayScreen != eDM_DISPLAY_SCREEN_PLAY)
            {
                if(m_ePlayerState == eMP_STATE_PLAYING && 
                   m_bAutoTrackChanged == false &&
                   (nPrevTime == 0 ||
                    nTime <= 1000 ||
                    nTime > nPrevTime + 1000 ||
                    nTime < nPrevTime - 1000))
                {
                    MC_HIGH << "DVDManager::ExtractDVDStatus(): bRealSoundChanged = true [1]" << LOG_ENDL;
                    bRealSoundChanged = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                    //emit OnRealSoundChanged(); // commented by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                    m_bAutoTrackChanged = true;
                }
            }
            // } added by cychoi 20103.09.01
        }
    }
#ifdef DVD_NAVIGATION_STILL_PICTURE_RETRY_DISABLED
    else
    {
        bStillPictureStartTime = false;
    }
#endif

    int bHighLight = m_tStatus.m_bHighLight; // added by cychoi 2013.08.12 for ITS 183907 DVD screen state (Highlight)
    // ETC
    m_tStatus.m_bMultiAngleOff      = pStatus->nMultiAngle;
    m_tStatus.m_bDownMixForbidden   = pStatus->nPCMDMix;
    m_tStatus.m_bHighLight          = pStatus->nHighlight;
    m_tStatus.m_bDolbySurround      = pStatus->nDSUR;
    m_tStatus.m_bLFEExist           = pStatus->nLFE;
    m_tStatus.m_nFrequency          = (DECK_DVD_QUANTI_FREQUENCY)pStatus->nFrequency;
    m_tStatus.m_nResolution         = (DECK_DVD_QUANTI_RESOLUTION)pStatus->nResolution;
    m_tStatus.m_nAudioOutput        = (DECK_VCD_AUDIO_OUTPUT_MODE)pStatus->nAudioOutput;

    //////////////////////////////////////////////////////////////////////
    // Subtitle No/Total Subtitle/Subtitle Code
    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    int nRelevantSubTitleNo         = pStatus->nRelevantSubTitleNo;
    m_tStatus.m_nTotalSubTitleNo    = BCD2DEC2(pStatus->nTotalSubTitleNo);
    int nSubTitleCode               = BCDTOISO639(pStatus->nSubTitleCodeUpper, pStatus->nSubTitleCodeLower);
    if(m_tStatus.m_nTotalSubTitleNo == 0)
    {
        nRelevantSubTitleNo = 0;
    }
    else
    {
        if(DeckManager::GetInstance()->GetDVDMediaMode() == eDM_MODE_DVD_AUDIO)
        {
            if(nRelevantSubTitleNo > m_tStatus.m_nTotalSubTitleNo)
            {
                nRelevantSubTitleNo = 1;
            }
        }
    }
    if(nRelevantSubTitleNo != m_tStatus.m_nSubTitleNo ||
       nSubTitleCode != m_tStatus.m_nSubTitleCode) {
        m_tStatus.m_nSubTitleNo   = nRelevantSubTitleNo;
        m_tStatus.m_nSubTitleCode = nSubTitleCode;
        MC_LOG << "DVDManager::ExtractDVDStatus(): m_nSubTitleCode: " << m_tStatus.m_nSubTitleCode << LOG_ENDL;
        emit OnSubtitleLangChanged(m_tStatus.m_nSubTitleCode);
    }
    //} modified by cychoi 2013.10.06

    //////////////////////////////////////////////////////////////////////
    // Audio
    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    m_tStatus.m_nTotalAudioNo       = BCD2DEC2(pStatus->nTotalAudioNo);
    m_tStatus.m_nAudioAttr          = (DECK_DVD_AUDIO_ATTRIBUTE)pStatus->nAudioAttr;
    m_tStatus.m_nChannel            = (DECK_CD_CHANNEL_MODE)pStatus->nChannel;
    int nAudioCode                  = BCDTOISO639(pStatus->nAudioCodeUpper, pStatus->nAudioCodeLower);
    int nRelevantAudioNo            = BCD2DEC2(pStatus->nRelevantAudioNo);
    if(m_tStatus.m_nTotalAudioNo == 0)
    {
        nRelevantAudioNo = 0;
    }
    //{ modified by cychoi 2013.10.15 for Active Audio Language Flickering (No Audio -> Language Code)
    if((m_tStatus.m_nAudioNo != nRelevantAudioNo ||
        m_tStatus.m_nAudioCode != nAudioCode) &&
       (nRelevantAudioNo == 0 ||
        m_tStatus.m_nChannel != eDM_CD_CHANNEL_INVALID))
    {
        m_tStatus.m_nAudioNo = nRelevantAudioNo;
        m_tStatus.m_nAudioCode = nAudioCode;
        MC_LOG << "DVDManager::ExtractDVDStatus(): m_nAudioNo: " << m_tStatus.m_nAudioNo << "(" << m_tStatus.m_nAudioCode << ", " << m_tStatus.m_nAudioAttr << ", " << m_tStatus.m_nChannel << LOG_ENDL;
        emit OnAudioLangChanged(m_tStatus.m_nAudioNo);
        // { modified by cychoi 2013.10.25 for ITS 195688 Audio repeated request
        if(m_nReqSwitchAudio > 1)
        {
            ContinueSwitchAudio();
        }
        else
        {
            m_nReqSwitchAudio = 0;
        }
        // } modified by cychoi 2013.10.25
        // { added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
        if(m_bAudioSwitched == true && m_nReqSwitchAudio == 0)
        {
            MC_HIGH << "DVDManager::ExtractDVDStatus(): bRealSoundChanged = true [2]" << LOG_ENDL;
            bRealSoundChanged = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
            //emit OnRealSoundChanged(); // commented by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
            m_bAudioSwitched = false;
        }
        // } added by cychoi 2013.09.01
    }
    //} modified by cychoi 2013.10.15
    //} modified by cychoi 2013.10.06

    //////////////////////////////////////////////////////////////////////
    // Angle/Total Angle
    m_tStatus.m_nTotalAngleNo       = BCD2DEC2(pStatus->nTotalAngleNo);
    int nRelevantAngleNo            = BCD2DEC1(pStatus->nRelevantAngleNo);
    if(nRelevantAngleNo != m_tStatus.m_nAngleNo) {
        m_tStatus.m_nAngleNo = nRelevantAngleNo;
        MC_LOG << "DVDManager::ExtractDVDStatus(): m_tStatus.m_nAngleNo: " << m_tStatus.m_nAngleNo << LOG_ENDL;
        emit OnAngleChanged(m_tStatus.m_nAngleNo);
    }

    int nTotalChapterNo10_1 = BCD2DEC1(pStatus->nTotalChapterNo100) + BCD2DEC2(pStatus->nTotalChapterNo10_1);
    // { modified by cychoi 2013-07-15 for Prevent fix
    if(nTotalChapterNo10_1 != m_tStatus.m_nTotalChapterNo && pStatus->nTotalChapterNo100 != 0xF) {
    //if(nTemp != m_tStatus.m_nTotalChapterNo && pStatus->nTotalChapterNo100 != 0xFF) {
    // } modified by cychoi 2013-07-15
        MC_LOG << "DVDManager::ExtractDVDStatus(): Total Chapter No: " << m_tStatus.m_nTotalChapterNo << "->" << nTotalChapterNo10_1 << LOG_ENDL;
        m_tStatus.m_nTotalChapterNo = nTotalChapterNo10_1;
    }

    int nTotalGroupTitlePGPLNo = BCD2DEC2(pStatus->nTotalGroupTitlePGPLNo);
    if(nTotalGroupTitlePGPLNo != m_tStatus.m_nTotalGrpTlePGPLNo && pStatus->nTotalGroupTitlePGPLNo != 0xFF) {
        MC_LOG << "DVDManager::ExtractDVDStatus(): Total Title No: " << m_tStatus.m_nTotalGrpTlePGPLNo << "->" << nTotalGroupTitlePGPLNo << LOG_ENDL;
        m_tStatus.m_nTotalGrpTlePGPLNo = nTotalGroupTitlePGPLNo;
    }

    ////////////////////////////////////////////////////////
    // 화면 상태 변경 알림.
    DECK_DISPLAY_SCREEN_MODE eDisplay = (DECK_DISPLAY_SCREEN_MODE)pStatus->nOnDisplay;
    // { added by cychoi 2013-06-07 for ITS 171361 DVD highlight navigation
    if(m_tStatus.m_bHighLight) { // { modified by cychoi 2013-11-09 for ITS 207558
        if(eDisplay == eDM_DISPLAY_SCREEN_DEFAULT || eDisplay == eDM_DISPLAY_SCREEN_PLAY)
        {
            eDisplay = eDM_DISPLAY_SCREEN_TITLE_MENU;
        }
    }
    //{ added by cychoi 2013.08.27 for ITS 183907 & 186734 DVD screen state (Highlight)
    else if(bHighLight) {
        // Let's ignore first screen state if current sccren has a button to highlight
        // For example: It is to ignore second screen state. (eDM_DISPLAY_SCREEN_PLAY)
        // eDM_DISPLAY_SCREEN_TITLE_MENU (Highlight) -> eDM_DISPLAY_SCREEN_PLAY -> eDM_DISPLAY_SCREEN_TITLE_MENU (Highlight)
        // { added by cychoi 20103.09.26 for ITS 188493 DVD Navigation Still Picture
        if(DeckManager::GetInstance()->GetDVDMediaMode() == eDM_MODE_DVD_AUDIO)
        {
            eDisplay = eDM_DISPLAY_SCREEN_DEFAULT;
        }
        // } added by cychoi 20103.09.26    
        // { added by yungi 2013.10.01 for ITS 192340
        else
        {
            if(eDisplay == eDM_DISPLAY_SCREEN_DEFAULT)
                eDisplay = eDM_DISPLAY_SCREEN_PLAY;
        }
        // } added by yungi
    }
    // { added by yungi 2013.10.01 for ITS 192340
    else
    {
        if(DeckManager::GetInstance()->GetDVDMediaMode() == eDM_MODE_DVD_VIDEO)
        {
            if(eDisplay == eDM_DISPLAY_SCREEN_DEFAULT)
            {
                // { modified by yungi 2013.11.09 for ITS 207825
                if(nPrevState != PLAYER_SEARCH && GetCurrentMediaState() == PLAYER_PLAY)
                    eDisplay = eDM_DISPLAY_SCREEN_PLAY;
                // } modified by yungi
            }
        }
    }
    // } added by yungi

    // } added by cychoi 2013-08-27
    // } added by cychoi 2013-06-07
    if(m_tStatus.m_nDisplayScreen != eDisplay) {
        m_tStatus.m_nDisplayScreen = eDisplay;
        emit OnScreenStateChanged(GetCurrentScreenState());
        m_tStatus.m_nAudioNo = 0xFF; // added by cychoi 2013.07.27 for ITS 181112 & 181175 Audio Language wrong display
        m_tStatus.m_nSubTitleNo = 0xFF;  // modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display // added by yungi 2013.08.26 for ITS 0186197
        m_tStatus.m_nAngleNo = 0xFF;   // added by yungi 2013.08.26 for ITS 0186197
        m_nReqSwitchAudio = 0; // added by cychoi 2013.10.25 for ITS 195688 Audio repeated request
    }

#ifdef DVD_NAVIGATION_STILL_PICTURE_RETRY_DISABLED
    if(m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_PLAY)
    {
        if(bStillPictureStartTime == true)
        {
            m_nStartPlaybackTime = m_tStatus.m_nPlaybackTime;
            MC_LOG << "DVDManager::ExtractDVDStatus: m_nStartPlaybackTime=" << m_nStartPlaybackTime << LOG_ENDL;
#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
            bStillPictureTimerStart = true;
            //StartStillPictureTimer();
#endif
        }
        else
        {
            m_nStartPlaybackTime = 0;
        }
    }
#endif

    ////////////////////////////////////////////////////////
    // 모드 변경 알림.
    // MC_LOG << "DVDManager::m_tStatus.m_nRandomMode: " << m_tStatus.m_nRandomMode << "-" << pStatus->nRandomMode << LOG_ENDL;
    // MC_LOG << "DVDManager::m_tStatus.m_nRepeatMode: " << m_tStatus.m_nRepeatMode << "-" << pStatus->nRepeatMode << LOG_ENDL;
    // MC_LOG << "DVDManager::m_tStatus.m_nScanMode :  " << m_tStatus.m_nScanMode << "-" << pStatus->nScanMode << LOG_ENDL;
    DECK_DVD_RANDOM_MODE eRandomMode = (DECK_DVD_RANDOM_MODE)pStatus->nRandomMode;
    DECK_DVD_REPEAT_MODE eRepeatMode = (DECK_DVD_REPEAT_MODE)pStatus->nRepeatMode;
    DECK_DVD_SCAN_MODE   eScanMode   = (DECK_DVD_SCAN_MODE)pStatus->nScanMode;
    //{ modified by cychoi 20130.07.11 for keeping playback options
    if(m_tStatus.m_nRandomMode != eRandomMode && m_nUserRandomMode == -1) {
        m_tStatus.m_nRandomMode = eRandomMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef DVD_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackRandomMode = m_tStatus.m_nRandomMode;
#endif
// } modified by cychoi 2013.06.20
        emit OnModeChanged(GetRandomMode(m_tStatus.m_nRandomMode));
    }  
    if(m_tStatus.m_nRepeatMode != eRepeatMode) {
        m_tStatus.m_nRepeatMode = eRepeatMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef DVD_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackRepeatMode = m_tStatus.m_nRepeatMode;
#endif
// } modified by cychoi 2013.06.20
        emit OnModeChanged(GetRepeatMode(m_tStatus.m_nRepeatMode));
    }
    if(m_tStatus.m_nScanMode != eScanMode) {
        if((m_tStatus.m_nScanMode != eDM_DVD_SCAN_OFF && m_tStatus.m_nScanMode != eDM_DVD_SCAN_NONE) && eScanMode == eDM_DVD_SCAN_OFF) {
            emit OnScanCompleted();
        }
        m_tStatus.m_nScanMode = eScanMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef DVD_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackScanMode = m_tStatus.m_nScanMode;
#endif
// } modified by cychoi 2013.06.20
        emit OnModeChanged(GetScanMode(m_tStatus.m_nScanMode));
    }

    if(m_nUserRandomMode == -2) m_nUserRandomMode = -1;
    //} modified by cychoi 20130.07.11

// { added by cychoi 2013-07-13 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    if(m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_PLAY)
    {
        switch(m_ePlayerState)
        {
        case eMP_STATE_PLAYING:
        case eMP_STATE_SEARCHING:
        case eMP_STATE_FAST_PLAYING: 
            StartRetryPlaybackTimer(7500); // DVD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
            break;
        case eMP_STATE_STOPPED:
        case eMP_STATE_PAUSED:
        default:
            break;
        }
    }
#endif
// } added by cychoi 2013-07-13

    //if(m_bAutoTrackChanged == false) m_bAutoTrackChanged = true; // comented by cychoi 20103.07.22 for add new signal

    // { added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
    if(bRealSoundChanged == true)
    {
        m_bRealSoundChanged = true;
    }
    // } added by cychoi 2013.09.01

#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
    if(bStillPictureTimerStart == true)
    {
        MC_LOG << "DVDManager::ExtractDVDStatus: StartStillPictureTimer()" << LOG_ENDL;
        StartStillPictureTimer();
    }
#endif

    return true;
}

bool DVDManager::ExtractParentalChangeRequest(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_PARENTAL_CHANGE_REQUEST) return false;

    MC_LOG << "DVDManager::ExtractParentalChangeRequest" << LOG_ENDL;

    m_pPacket->ParentalChangePermission(eDM_PERMIT_CHANGE);
    
    return true;
}

bool DVDManager::ExtractDirectTouch(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_DIRECT_TOUCH) return false;

    DirectTouch *pStatus = &pPacket->m_tData.m_tDirectTouch;

    if(pStatus->nValid == 0) return false;

    MC_LOG << "DVDManager::ExtractDirectTouch: " << pStatus->nSelectionDecision << LOG_ENDL;

    return true;
}

bool DVDManager::ExtractReqError(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_REQ_ERR) return false;

    ReqError *pStatus = &pPacket->m_tData.m_tReqError;

    MC_LOG << "DVDManager::ExtractReqError: 0x" << pStatus->nErrorDetail << LOG_ENDL;

    // { added by cychoi 20103.08.23 for ITS 184879 & 186162
    if(m_bAutoTrackChanged == false)
    {
        MC_HIGH << "DVDManager::ExtractReqError(): m_bRealSoundChanged = true" << LOG_ENDL;
        m_bRealSoundChanged = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
        //emit OnRealSoundChanged(); // commented by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
        m_bAutoTrackChanged = true;
    }
    // } added by cychoi 20103.08.23

#ifdef DVD_NAVIGATION_STILL_PICTURE_RETRY_DISABLED
    if(DeckManager::GetInstance()->GetDVDMediaMode() != eDM_MODE_DVD_AUDIO)
    {
#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
        //HandleStillPicture();
#endif
        m_bEnterKey = false;
    }
#endif

    // { modified by cychoi 2013.10.27 for ITS 198856
    if(m_nFFREWRate != -1 || m_nFFREWState != PLAYER_PLAY)
    {
        switch(m_ePlayerState)
        {
        case eMP_STATE_PLAYING:
        case eMP_STATE_STOPPED:
        case eMP_STATE_PAUSED:
        case eMP_STATE_SEARCHING:
            m_nFFREWRate = -1;
            m_nFFREWState = PLAYER_PLAY;
            m_bFFREWStateSigSend = false;
            emit OnStateChanged(GetCurrentMediaState());
            break;
        case eMP_STATE_FAST_PLAYING: 
        default:
            break;
        }
    }
    // } modified by cychoi 2013.10.27

    // { added by cychoi 2013.10.25 for ITS 195688 Audio repeated request
    if(m_nReqSwitchAudio > 1)
    {
        ContinueSwitchAudio();
    }
    else
    {
        m_nReqSwitchAudio = 0;
    }
    if(m_bAudioSwitched == true && m_nReqSwitchAudio == 0)
    {
        MC_LOG << "DVDManager::ExtractReqError(): m_nAudioNo: " << m_tStatus.m_nAudioNo << "(" << m_tStatus.m_nAudioCode << "-" << m_tStatus.m_nAudioAttr << "-" << m_tStatus.m_nChannel << ")" << LOG_ENDL;
        emit OnAudioLangChanged(m_tStatus.m_nAudioNo);
        m_bRealSoundChanged = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
        //emit OnRealSoundChanged(); // commented by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
        m_bAudioSwitched = false;
    }
    // } added by cychoi 2013.10.25

#ifdef DVD_DECK_REQ_RETRY_ENABLED
    InitRetryVRInfo();
    InitRetryPlayback();
    InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
#endif

    if(pStatus->nErrorDetail == 0x03)
    {
        if(m_tSearchTask.count() > 0) {
            m_tSearchMutex.lock();
            m_tSearchTask.clear();
            m_tSearchMutex.unlock();
            MC_LOG << "DVDManager::ExtractReqError() OnSearchCompleted(0, 0, 0) end" << LOG_ENDL;
            emit OnSearchCompleted(0, 0, 0);
        }
    }
    else if(pStatus->nErrorDetail == 0xFE) // operation is forbidden(during DVD-Audio/Video/VR, Video-CD Mode)
    {
        // { commented by cychoi 2013.10.30 for ITS 204969
        // { added by jeeeun for SANITY_CM_AG127 disc menu fail
        //if(m_bTopMenu == true){
        //    MC_LOG << "DVDManager::ExtractReqError() top Menu" << LOG_ENDL;
        //    m_bTopMenu = false;
        //    if(m_titleSetting == 0) {
        //        m_pPacket->TitleMenu(eDM_TITLE_MENU_TOGGLE);
        //    }
        //    else {
        //        m_pPacket->TitleMenu(eDM_TITLE_MENU_ALWAYS);
        //    }
        //}
		// } added by jeeeun
        // } commented by cychoi 2013.10.30
		
        // 자막 언어 요청이 실패 났을 경우.
        if(m_bSubTitleSearchMode == true) {
            MC_HIGH << "DVDManager::ExtractReqError() relevant subtitle code: 0, total: 1" << LOG_ENDL;
            m_bSubTitleSearchMode  = false;
            m_nSubTitleCodeNum     = 1;
            m_pSubTitleCodeList[0] = 0;
            EmitOnTotalSubtitleLangReceived();
        }

        if(m_bAudioSearchMode == true) {
            MC_HIGH << "DVDManager::ExtractReqError() relevant audio code: 0, total: 1" << LOG_ENDL;
            m_bAudioSearchMode  = false;
            m_nAudioCodeNum     = 1;
            m_pAudioCodeList[0] = 0;
            m_pAudioAttrList[0] = m_tStatus.m_nAudioAttr;
            m_pAudioChnlList[0] = m_tStatus.m_nChannel;
            EmitOnTotalAudioLangReceived();
        }

        if(m_tSearchTask.count() > 0) {
            int bComplete = false;
            m_tSearchMutex.lock();
            TitleChapterTask tTask = m_tSearchTask.first();
            if(tTask.m_bRequestFStop == false) {
                MC_HIGH << "DVDManager::ExtractReqError() --> FunctionStop()" << LOG_ENDL;
                tTask.m_bRequestFStop = true;
                m_tSearchTask.replace(0, tTask);
                m_pPacket->FunctionStop();
            }
            else {
                MC_HIGH << "DVDManager::ExtractReqError() SearchTitleChapter fail." << LOG_ENDL;
                m_tSearchTask.clear();
                bComplete = true;
            }
            m_tSearchMutex.unlock();

            if(bComplete == true) {
                MC_LOG << "DVDManager::ExtractReqError() OnSearchCompleted(0, 0, 0) end" << LOG_ENDL;
                emit OnSearchCompleted(0, 0, 0);
            }
        }
    }

    emit OnError(PLAYER_COMMANDNOTSUPPORTED);
    
    return true;
}

int DVDManager::InitDVDSetting()
{
    int bSuccess = true;
    
    if(m_bDVDSettingMode == false) {
        return false;
    }

    // check dvd status.
    if(m_tStatus.m_nPlayStatus == eDM_CD_PLAY_COND_HALTING) {
        bSuccess = false;
    }

    if(bSuccess == true) 
    {
        // Init Subtitle
        if(m_nDVDSettings == DSS_START_STEP) {
            m_nDVDSettings = DSS_SUBTITLE_START;
            if(SetSubtitleLanguage_Impl(m_tDVDSettings.nSubtitleCode) == false) {
                m_nDVDSettings = DSS_SUBTITLE_END;
            }
        }
        
        // Init Audio
        if(m_nDVDSettings == DSS_SUBTITLE_END) {
            m_nDVDSettings = DSS_AUDIO_START;
            if(SetAudioLanguage_Impl(m_tDVDSettings.nAudioCode, m_tDVDSettings.nAudioIndex) == false) {
                m_nDVDSettings = DSS_AUDIO_END;
            }
        }

        // Init Angle
        if(m_nDVDSettings == DSS_AUDIO_END) {
            m_nDVDSettings = DSS_ANGLE_START;
            if(SetAngle_Impl(m_tDVDSettings.nAngleNumber) == false) {
                m_nDVDSettings = DSS_ANGLE_END;    
            }
        }
    }
    else {
        m_nDVDSettings = DSS_ANGLE_END;
    }

    if(m_nDVDSettings == DSS_ANGLE_END) {
        emit OnDVDInitCompleted(bSuccess);
        m_bDVDSettingMode = false;
    }
    else {
        RequestDVDStatus();
    }

    return true;
}

int DVDManager::AudioLangSearchMode()
{
    unsigned int nIndex = (unsigned int)m_tStatus.m_nAudioNo - 1;
    
    if(m_tStatus.m_nTotalAudioNo == 0xFF || m_tStatus.m_nAudioNo == 0xFF) 
    {
        MC_LOG << "DVDManager::AudioLangSearchMode(): m_tStatus.m_nTotalAudioNo == 0xFF" << LOG_ENDL;
        
        m_bAudioSearchMode  = false;
        m_nAudioCodeNum     = 1;
        m_pAudioCodeList[0] = 0;
        m_pAudioAttrList[0] = m_tStatus.m_nAudioAttr;
        m_pAudioChnlList[0] = m_tStatus.m_nChannel;
        EmitOnTotalAudioLangReceived();
    }
    else if(nIndex < (unsigned int)m_tStatus.m_nTotalAudioNo) 
    {
        if(m_pAudioCodeList[nIndex] != m_tStatus.m_nAudioCode && m_tStatus.m_nChannel != eDM_CD_CHANNEL_INVALID) {
            m_pAudioCodeList[nIndex] = m_tStatus.m_nAudioCode;
            m_pAudioAttrList[nIndex] = m_tStatus.m_nAudioAttr;
            m_pAudioChnlList[nIndex] = m_tStatus.m_nChannel;
            m_nAudioCodeNum++;
            if(m_nAudioCodeNum >= m_tStatus.m_nTotalAudioNo) {
                EmitOnTotalAudioLangReceived();
                m_bAudioSearchMode = false;
            }
            else {
                MC_LOG << "DVDManager::AudioLangSearchMode(): request new Audio1" << LOG_ENDL;
                m_pPacket->Audio();
            }
        }
        else {
            MC_LOG << "DVDManager::AudioLangSearchMode(): nAudioCode: " << m_tStatus.m_nAudioCode << ", nChannel: " << m_tStatus.m_nChannel << LOG_ENDL;
            
            if(m_pAudioCodeList[nIndex] == m_tStatus.m_nAudioCode && m_tStatus.m_nChannel == eDM_CD_CHANNEL_INVALID) {
                MC_LOG << "DVDManager::AudioLangSearchMode(): request new Audio2" << LOG_ENDL;
                m_pPacket->Audio();
            }
        }
    }
    else 
    {
        MC_HIGH << "DVDManager::AudioLangSearchMode(): relevant audio no: " << nIndex << ", total: " << m_tStatus.m_nTotalAudioNo << LOG_ENDL;

        m_bAudioSearchMode  = false;
        m_nAudioCodeNum     = 1;
        m_pAudioCodeList[0] = 0;
        m_pAudioAttrList[0] = m_tStatus.m_nAudioAttr;
        m_pAudioChnlList[0] = m_tStatus.m_nChannel;
        EmitOnTotalAudioLangReceived();
    }
            
    return 1;
}

int DVDManager::SetAudioLangMode()
{
    if(m_tStatus.m_nChannel == eDM_CD_CHANNEL_INVALID) {
        //MC_LOG << "SetAudioLangMode(): m_tStatus.m_nChannel == eDM_CD_CHANNEL_INVALID" << LOG_ENDL;
        return 1;
    }

    if(m_tAudioTask.count() <= 0) {
        MC_LOG << "DVDManager::SetAudioLangMode(): m_tAudioTask.count() <= 0" << LOG_ENDL;
        return 1;
    }

    AudioTask tTask = m_tAudioTask.first();
    
    if(m_tStatus.m_nAudioNo != tTask.m_nAudioNo) {
        MC_LOG << "DVDManager::SetAudioLangMode(): Deck.AudioCode(" << m_tStatus.m_nAudioCode << ") != AudioCode(" << tTask.m_nAudioCode << ") && Deck.AudioNo(" << m_tStatus.m_nAudioNo << ") != AudioNo(" << tTask.m_nAudioNo << ")" << LOG_ENDL;
        if(m_nPrevAudioLangNo != m_tStatus.m_nAudioNo) {
            m_nPrevAudioLangNo = m_tStatus.m_nAudioNo;
            m_pPacket->Audio();
        }else{
            MC_LOG << "DVDManager::m_nPrevAudioLangNo == m_tStatus.m_nAudioNo(" << m_tStatus.m_nAudioNo << ")" << LOG_ENDL;
        }
    }
    else {
        int bRequest = false;
        m_tAudioMutex.lock();
        m_tAudioTask.removeFirst();
        if(m_tAudioTask.count() > 0) {
            bRequest = true;
            tTask = m_tAudioTask.takeLast();
            m_tAudioTask.clear();
            m_tAudioTask.append(tTask);
        }
        m_tAudioMutex.unlock();
        
        if(bRequest == true) {
            MC_LOG << "DVDManager::SetAudioLangMode(): request new audio " << tTask.m_nAudioNo << LOG_ENDL;
            if(m_nPrevAudioLangNo != m_tStatus.m_nAudioNo) {
                m_nPrevAudioLangNo = m_tStatus.m_nAudioNo;
                m_pPacket->Audio();
            }else{
                MC_LOG << "DVDManager::m_nPrevAudioLangNo == m_tStatus.m_nAudioNo(" << m_tStatus.m_nAudioNo << ")" << LOG_ENDL;
            }
        }
        else {
            MC_LOG << "DVDManager::SetAudioLangMode(): Deck.AudioCode(" << m_tStatus.m_nAudioCode << ") != AudioCode(" << tTask.m_nAudioCode << ") && Deck.AudioNo(" << m_tStatus.m_nAudioNo << ") != AudioNo(" << tTask.m_nAudioNo << ")" << LOG_ENDL;
            MC_LOG << "DVDManager::SetAudioLangMode(): end" << LOG_ENDL;
            if(m_bDVDSettingMode == false) {
                EmitOnAudioLangChanged(m_tStatus.m_nAudioNo);
            }else{
                m_nDVDSettings = DSS_AUDIO_END;
            }
        }
    }
    
    return 1;
}

int DVDManager::SubtitleLangSearchMode()
{
    // MC_LOG << "DVDManager::subtitle: %d. %d", m_tStatus.m_nSubTitleNo, m_tStatus.m_nSubTitleCode);
    
    unsigned int nIndex = (unsigned int)m_tStatus.m_nSubTitleNo;
    
    if(m_tStatus.m_nTotalSubTitleNo == 0xFF || m_tStatus.m_nSubTitleNo == 0xFF) 
    {
        MC_HIGH << "DVDManager::SubtitleLangSearchMode(): m_tStatus.m_nTotalSubTitleNo == 0xFF" << LOG_ENDL;
        m_bSubTitleSearchMode = false;
        m_nSubTitleCodeNum     = 1;
        m_pSubTitleCodeList[0] = 0;
        EmitOnTotalSubtitleLangReceived();
    }
    else if(nIndex <= (unsigned int)m_tStatus.m_nTotalSubTitleNo) // 자막은 off도 포함되어 있음.
    { 
        if(m_pSubTitleCodeList[nIndex] != m_tStatus.m_nSubTitleCode) {
            m_pSubTitleCodeList[nIndex] = m_tStatus.m_nSubTitleCode;
            m_nSubTitleCodeNum++;
            if(m_nSubTitleCodeNum >= m_tStatus.m_nTotalSubTitleNo + 1) {
                MC_LOG << "DVDManager::OnTotalSubtitleLangReceived: " << m_nSubTitleCodeNum << LOG_ENDL;
                EmitOnTotalSubtitleLangReceived();
                m_bSubTitleSearchMode = false;
            }
            else {
                MC_LOG << "DVDManager::SubtitleLangSearchMode(): request new Subtitle1" << LOG_ENDL;
                m_pPacket->Subtitle();
            }
        }
        else {
            MC_LOG << "DVDManager::m_tStatus.m_nTotalSubTitleNo: " << m_tStatus.m_nTotalSubTitleNo << "(" << nIndex << "), m_tStatus.m_nSubTitleCode: " << m_tStatus.m_nSubTitleCode << LOG_ENDL;
            m_nPrevSubtitleLangNo++;
            if(m_nPrevSubtitleLangNo > DVD_CHECK_PREV_SETUP_COUNT) {
                MC_LOG << "DVDManager::SubtitleLangSearchMode(): request new Subtitle2" << LOG_ENDL;
                m_nPrevSubtitleLangNo = 0;
                m_pPacket->Subtitle();
            }
        }
    }
    else 
    {
        MC_HIGH << "DVDManager::SubtitleLangSearchMode(): relevant subtitle no: " << nIndex << ", total: " << m_tStatus.m_nTotalSubTitleNo << LOG_ENDL;
        
        m_bSubTitleSearchMode  = false;
        m_nSubTitleCodeNum     = 1;
        m_pSubTitleCodeList[0] = 0;
        EmitOnTotalSubtitleLangReceived();
    }
            
    return 1;
}

int DVDManager::SetSubtitleLangMode()
{
    if(m_tSubtitleTask.count() <= 0) {
        MC_LOG << "DVDManager::SetSubtitleLangMode(): m_tSubtitleTask.count() <= 0" << LOG_ENDL;
        return 1;
    }
    
    int nTask = m_tSubtitleTask.first();
    
    if(m_tStatus.m_nSubTitleCode != nTask) {
        MC_LOG << "DVDManager::SetSubtitleLangMode() m_tStatus.m_nSubTitleCode(" << m_tStatus.m_nSubTitleCode << ") != m_nSubTitleCode(" << nTask << ")" << LOG_ENDL;
        if(m_nPrevSubtitleLangNo != m_tStatus.m_nSubTitleNo) {
            m_nPrevSubtitleLangNo = m_tStatus.m_nSubTitleNo;
            m_pPacket->Subtitle();
        }else{
            MC_LOG << "DVDManager::m_nPrevSubtitleLangNo == m_tStatus.m_nSubTitleNo(" << m_tStatus.m_nSubTitleNo << ")" << LOG_ENDL;
        }
    }
    else {
        int bRequest = false;
        m_tSubtitleMutex.lock();
        m_tSubtitleTask.removeFirst();
        if(m_tSubtitleTask.count() > 0) {
            bRequest = true;
            nTask = m_tSubtitleTask.takeLast();
            m_tSubtitleTask.clear();
            m_tSubtitleTask.append(nTask);
        }
        m_tSubtitleMutex.unlock();

        if(bRequest == true) {
            MC_LOG << "DVDManager::SetSubtitleLangMode(): request new subtitle " << nTask << LOG_ENDL;
            if(m_nPrevSubtitleLangNo != m_tStatus.m_nSubTitleNo) {
                m_nPrevSubtitleLangNo = m_tStatus.m_nSubTitleNo;
                m_pPacket->Subtitle();
            }else{
                MC_LOG << "DVDManager::m_nPrevSubtitleLangNo == m_tStatus.m_nSubTitleNo(" << m_tStatus.m_nSubTitleNo << ")" << LOG_ENDL;
            }
        }
        else {
            MC_LOG << "DVDManager::SetSubtitleLangMode() end: Deck.m_nSubTitleCode(" << m_tStatus.m_nSubTitleCode << ") == SubTitleCode(" << nTask << ")" << LOG_ENDL;
            if(m_bDVDSettingMode == false) {
                EmitOnSubtitleLangChanged(m_tStatus.m_nSubTitleCode);
            }else{
                m_nDVDSettings = DSS_SUBTITLE_END;
            }
        }
    }
    
    return 1;
}

int DVDManager::SetAngleNumberMode()
{

    if(m_tAngleTask.count() <= 0) {
        MC_LOG << "DVDManager::SetAngleNumberMode(): m_tAngleTask.count() <= 0" << LOG_ENDL;
        return 1;
    }

    int nTask = m_tAngleTask.first();
    
    if(m_tStatus.m_nAngleNo != nTask) {
        MC_LOG << "DVDManager::SetAngleNumberMode() m_tStatus.m_nAngleNo(" << m_tStatus.m_nAngleNo << ") != nAngleNumber(" << nTask << ")" << LOG_ENDL;
        if(m_nPrevAngleNo != m_tStatus.m_nAngleNo) {
            m_nPrevAngleNo = m_tStatus.m_nAngleNo;
            m_pPacket->Angle();
        }else{
            MC_LOG << "DVDManager::m_nPrevAngleNo == m_tStatus.m_nAngleNo(" << m_tStatus.m_nAngleNo << ")" << LOG_ENDL;
        }
    }
    else {
        int bRequest = false;
        m_tAngleMutex.lock();
        m_tAngleTask.removeFirst();
        if(m_tAngleTask.count() > 0) {
            bRequest = true;
            nTask = m_tAngleTask.takeLast();
            m_tAngleTask.clear();
            m_tAngleTask.append(nTask);
        }
        m_tAngleMutex.unlock();

        if(bRequest == true) {
            MC_LOG << "DVDManager::SetAngleNumberMode(): request new angle " << nTask << LOG_ENDL;
            if(m_nPrevAngleNo != m_tStatus.m_nAngleNo) {
                m_nPrevAngleNo = m_tStatus.m_nAngleNo;
                m_pPacket->Angle();
            }else{
                MC_LOG << "DVDManager::m_nPrevAngleNo == m_tStatus.m_nAngleNo(" << m_tStatus.m_nAngleNo << ")" << LOG_ENDL;
            }
        }
        else {
            MC_LOG << "DVDManager::SetAngleNumberMode() end: Deck.m_nAngleNo(" << m_tStatus.m_nAngleNo << ") == nAngleNo(" << nTask << ")" << LOG_ENDL;
            if(m_bDVDSettingMode == false) {
                EmitOnAngleChanged(m_tStatus.m_nAngleNo);
            }else{
                m_nDVDSettings = DSS_ANGLE_END;
            }
        }
    }
    
    return 1;
}

int DVDManager::SearchTitleChapterMode()
{
    if(m_tSearchTask.count() <= 0) {
        MC_LOG << "DVDManager::SearchTitleChapterMode(): m_tSearchTask.count() <= 0" << LOG_ENDL;
        return 1;
    }

    MC_LOG << "DVDManager::SearchTitleChapterMode() screen: " << m_tStatus.m_nDisplayScreen << LOG_ENDL;

    TitleChapterTask tTask = m_tSearchTask.first();
    if(tTask.m_nTitleNo != m_tStatus.m_nGrpTitlePGPLNo || tTask.m_nChapterNo != m_tStatus.m_nTrackChapterNo) 
    {
        MC_LOG << "DVDManager::SearchTitleChapterMode() nTitleNo(" << tTask.m_nTitleNo << "!=" << m_tStatus.m_nGrpTitlePGPLNo <<
                  "), nChapterNo(" << tTask.m_nChapterNo << "!=" << m_tStatus.m_nTrackChapterNo << ")" << LOG_ENDL;
        SearchTitleChapter_Impl(tTask.m_nTitleNo, tTask.m_nChapterNo);
    }
    else {
        int bRequest = false;
        m_tSearchMutex.lock();
        m_tSearchTask.removeFirst();
        if(m_tSearchTask.count() > 0) {
            bRequest = true;
            tTask = m_tSearchTask.takeLast();
            m_tSearchTask.clear();
            m_tSearchTask.append(tTask);
        }
        m_tSearchMutex.unlock();

        if(bRequest == true) {
            MC_LOG << "DVDManager::SearchTitleChapterMode() new request nTitleNo(" << tTask.m_nTitleNo << "), nChapterNo(" << tTask.m_nChapterNo << ")" << LOG_ENDL;
            SearchTitleChapter_Impl(tTask.m_nTitleNo, tTask.m_nChapterNo);
        }
        else {
            MC_LOG << "DVDManager::SearchTitleChapterMode(" << tTask.m_nTitleNo << ", " << tTask.m_nChapterNo << ", " << m_tStatus.m_nTotalChapterNo << ") end" << LOG_ENDL;
            emit OnSearchCompleted(tTask.m_nTitleNo, tTask.m_nChapterNo, m_tStatus.m_nTotalChapterNo);
        }
    }
    
    return 1;
}

int DVDManager::SearchTitleChapter_Impl(int nTitle, int nChapter)
{
    BCD4                tBCD4;
    BCD2                tBCD2;
    DirectContCommand   tCmd;

    tCmd.nFileDesign    = 0;
    tCmd.nPGPLDesggn    = 0;
    tCmd.nUndefined1    = 0;
    tCmd.nUndefined2    = 0;

    // TITLE/GROUP
    tBCD4.word          = DEC2BCD4(nTitle);
    tBCD2.BCD.upper     = tBCD4.BCD.upper2;
    tBCD2.BCD.lower     = tBCD4.BCD.lower2;
    
    tCmd.n1stCateUpper  = tBCD4.BCD.lower1;
    tCmd.n1stCateLower  = tBCD2.byte;

    // CHAPTER/TRACK
    tBCD4.word          = DEC2BCD4(nChapter);
    tBCD2.BCD.upper     = tBCD4.BCD.upper2;
    tBCD2.BCD.lower     = tBCD4.BCD.lower2;

    tCmd.n2ndCateUpper  = tBCD4.BCD.lower1;
    tCmd.n2ndCateLower  = tBCD2.byte;

    MC_LOG << "DVDManager::SearchTitleChapter_Impl(" << nTitle << ", " << nChapter << ")" << LOG_ENDL;
    
    m_pPacket->DirectCont(&tCmd);

    // { added by cychoi 2013.10.20 for ITS 197025 Syncronize OSD on Tune Wheel
    m_bAutoTrackChanged = false; // modified by cychoi 20103.08.11 for Play mute
    m_bUserTrackChanged = true; // modified by cychoi 20103.08.11 for ITS 183835 No OSD on track change automatically // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    m_bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
    m_nFFREWRate = -1; // added by cychoi 20103.06.17 for ITS 174004
    m_nFFREWState = PLAYER_PLAY; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
    m_eLastPlayerState = eMP_STATE_PLAYING; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
    
    // { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
    StartRetryPlaybackTimer(7500); // DVD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
    // } added by cychoi 2013-06-05
    // } added by cychoi 2013.10.20
        
    return 1;
}

int DVDManager::SetSubtitleLanguage_Impl(int nSubtitleCode)
{
    if(m_bSubTitleSearchMode == true) {
        MC_LOG << "DVDManager::SetSubtitleLanguage_Impl: m_bSubTitleSearchMode == true" << LOG_ENDL;
        return false;
    }
    if(m_nSubTitleCodeNum <= 0) {
        MC_LOG << "DVDManager::SetSubtitleLanguage_Impl: m_nSubTitleCodeNum : " << m_nSubTitleCodeNum << LOG_ENDL;
        return false;
    }

    int nIdx;
    for(nIdx = 0; nIdx < m_nSubTitleCodeNum; nIdx++) {
        if(nSubtitleCode == m_pSubTitleCodeList[nIdx]) break;
    }
    if(nIdx >= m_nSubTitleCodeNum) {
        MC_LOG << "DVDManager::SetSubtitleLanguage_Impl(" << nSubtitleCode << ") out of range..." << LOG_ENDL;
        return false;
    }

    // 현재 상태와 동일 한 경우.
    if(m_tSubtitleTask.count() == 0 && nSubtitleCode == m_tStatus.m_nSubTitleCode) {
        MC_LOG << "DVDManager::SetSubtitleLanguage_Impl: m_tStatus.m_nSubTitleCode(" << m_tStatus.m_nSubTitleCode << ") == nSubtitleCode(" << nSubtitleCode << ")" << LOG_ENDL;
        return false;
    }

    // 마지막 요청과 동일한 경우.
    m_tSubtitleMutex.lock();
    int bRequest = true;
    int nTask;
    if(m_tSubtitleTask.count() > 0) {
        nTask = m_tSubtitleTask.last();
        if(nTask == nSubtitleCode) {
            MC_LOG << "DVDManager::SetSubtitleLanguage_Impl(" << nSubtitleCode << ") equal to previous request" << LOG_ENDL;
            bRequest = false;
        }
    }

    if(bRequest == true) {
        if(m_tSubtitleTask.count() <= 0) {
            m_nPrevSubtitleLangNo = -1;
        }
        m_tSubtitleTask.append(nSubtitleCode);
        MC_LOG << "DVDManager::SetSubtitleLanguage_Impl() new Request " << nSubtitleCode << LOG_ENDL;
        
        // PAUSING상태이면, DVD 응답을 요청하여 처리가 되도록 한다.
        if(m_tStatus.m_nPlayStatus == eDM_CD_PLAY_COND_PAUSING) {
            m_bPauseAtSetting = true;
            MC_LOG << "DVDManager::SetSubtitleLanguage_Impl() --> RequestDVDAudioVideoVRStatus()" << LOG_ENDL;
            m_pPacket->RequestDVDAudioVideoVRStatus();
        }
    }  
    m_tSubtitleMutex.unlock();

    return true;
}

int DVDManager::SetAudioLanguage_Impl(int nAudioCode, int nIndex)
{
    if(m_bAudioSearchMode == true) {
        MC_LOG << "DVDManager::SetAudioLanguage_Impl: m_bAudioSearchMode == true" << LOG_ENDL;
        return false;
    }
    if(m_nAudioCodeNum <= 0) {
        MC_LOG << "DVDManager::SetAudioLanguage_Impl: m_nAudioCodeNum : " << m_nAudioCodeNum << LOG_ENDL;
        return false;
    }
    if(nIndex < 0 || nIndex >= m_nAudioCodeNum) {
        MC_LOG << "DVDManager::SetAudioLanguage_Impl: nIndex : " << nIndex << LOG_ENDL;
        return false;
    }

    if(nAudioCode != m_pAudioCodeList[nIndex]) {
        MC_LOG << "DVDManager::SetAudioLanguage_Impl(" << nAudioCode << ", " << nIndex << ") out of range..." << LOG_ENDL;
        return false;
    }

    // 현재 상태와 동일한 경우.
    if(m_tAudioTask.count() == 0 && m_tStatus.m_nAudioNo == nIndex + 1) {
        MC_LOG << "DVDManager::SetAudioLanguage_Impl: m_tStatus.m_nAudioCode(" << m_tStatus.m_nAudioCode << ") == nAudioCode(" << nAudioCode <<
                  ") m_nAudioNo(" << m_tStatus.m_nAudioNo << ") == nIndex(" << nIndex + 1 << ")!!!" << LOG_ENDL;
        return false;
    }

    // 마지막 요청과 동일한 경우.
    m_tAudioMutex.lock();
    int       bRequest = true;
    AudioTask tTask;
    if(m_tAudioTask.count() > 0) {
        tTask = m_tAudioTask.last();
        if(tTask.m_nAudioCode == nAudioCode && tTask.m_nAudioNo == nIndex + 1) {
            MC_LOG << "DVDManager::SetAudioLanguage_Impl(" << nAudioCode << ", " << nIndex << ") equal to previous request" << LOG_ENDL;
            bRequest = false;
        }
    }

    if(bRequest == true) {
        if(m_tAudioTask.count() <= 0) {
            m_nPrevAudioLangNo = -1;
        }
        tTask.m_nAudioCode = nAudioCode;
        tTask.m_nAudioNo   = nIndex + 1;
        m_tAudioTask.append(tTask);
        MC_LOG << "DVDManager::SetAudioLanguage_Impl() new Request " << nAudioCode << ", " << nIndex << LOG_ENDL;
        
        // PAUSING상태이면, DVD 응답을 요청하여 처리가 되도록 한다.
        if(m_tStatus.m_nPlayStatus == eDM_CD_PLAY_COND_PAUSING) {
            m_bPauseAtSetting = true;
            MC_LOG << "DVDManager::SetAudioLanguage_Impl() --> RequestDVDAudioVideoVRStatus()" << LOG_ENDL;
            m_pPacket->RequestDVDAudioVideoVRStatus();
        }
    }
    m_tAudioMutex.unlock();

    return true;
}

int DVDManager::SetAngle_Impl(int nAngleNumber)
{
    if(nAngleNumber <= 0 || nAngleNumber > m_tStatus.m_nTotalAngleNo) {
        MC_LOG << "DVDManager::SetAngle_Impl: m_tStatus.m_nTotalAngleNo: " << m_tStatus.m_nTotalAngleNo << ", nAngleNumber: " << nAngleNumber << LOG_ENDL;
        return false;
    }

    // 현재 상태와 동일 한 경우.
    if(m_tAngleTask.count() == 0 && nAngleNumber == m_tStatus.m_nAngleNo) {
        MC_LOG << "DVDManager::SetAngle_Impl: m_tStatus.m_nAngleNo(" << m_tStatus.m_nAngleNo << ") == nAngleNumber(" << nAngleNumber << ")" << LOG_ENDL;
        return false;
    }

    // 마지막 요청과 동일한 경우.
    m_tAngleMutex.lock();
    int bRequest = true;
    int nTask;
    if(m_tAngleTask.count() > 0) {
        nTask = m_tAngleTask.last();
        if(nTask == nAngleNumber) {
            MC_LOG << "DVDManager::SetAngle_Impl(" << nAngleNumber << ") equal to previous request" << LOG_ENDL;
            bRequest = false;
        }
    }

    if(bRequest == true) {
        if(m_tAngleTask.count() <= 0) {
            m_nPrevAngleNo = -1;
        }
        
        m_tAngleTask.append(nAngleNumber);
        MC_LOG << "DVDManager::SetAngle_Impl() new Request " << nAngleNumber << LOG_ENDL;

        // PAUSING상태이면, DVD 응답을 요청하여 처리가 되도록 한다.
        if(m_tStatus.m_nPlayStatus == eDM_CD_PLAY_COND_PAUSING) {
            m_bPauseAtSetting = true;
            MC_LOG << "DVDManager::SetAngle_Impl() --> RequestDVDAudioVideoVRStatus()" << LOG_ENDL;
            m_pPacket->RequestDVDAudioVideoVRStatus();
        }
    }

    m_tAngleMutex.unlock();

    return true;
}

void DVDManager::EmitOnTotalAudioLangReceived()
{
    if(m_bPauseAtSetting == true) {
        m_bPauseAtSetting = false;
        MC_LOG << "DVDManager::EmitOnTotalAudioLangReceived(): Pause()" << LOG_ENDL;
        Pause();
    }

    MC_LOG << "DVDManager::EmitOnTotalAudioLangReceived: " << m_nAudioCodeNum << LOG_ENDL;
    for(int i = 0; i < m_nAudioCodeNum; i++) {
        MC_LOG << "DVDManager::[" << i << "/" << m_nAudioCodeNum << "]: " << m_pAudioCodeList[i] << ", " << m_pAudioAttrList[i] << ", " << m_pAudioChnlList[i] << LOG_ENDL;
    }
    
    emit OnTotalAudioLangReceived(m_pAudioCodeList, m_pAudioAttrList, m_pAudioChnlList, m_nAudioCodeNum);
}

void DVDManager::EmitOnTotalSubtitleLangReceived()
{
    if(m_bPauseAtSetting == true) {
        m_bPauseAtSetting = false;
        MC_LOG << "DVDManager::EmitOnTotalSubtitleLangReceived(): Pause()" << LOG_ENDL;
        Pause();
    }

    MC_LOG << "DVDManager::EmitOnTotalSubtitleLangReceived: " << m_nSubTitleCodeNum << LOG_ENDL;
    for(int i = 0; i < m_nSubTitleCodeNum; i++) {
        MC_LOG << "DVDManager::[" << i << "/" << m_nSubTitleCodeNum << "]: " << m_pSubTitleCodeList[i] << LOG_ENDL;
    }
    
    emit OnTotalSubtitleLangReceived(m_pSubTitleCodeList, m_nSubTitleCodeNum);
}

void DVDManager::EmitOnAudioLangChanged(int nAudioNo)
{
    if(m_bPauseAtSetting == true) {
        m_bPauseAtSetting = false;
        MC_LOG << "DVDManager::EmitOnAudioLangChanged(): Pause()" << LOG_ENDL;
        Pause();
    }
    MC_LOG << "DVDManager::EmitOnAudioLangChanged" << LOG_ENDL;
    emit OnAudioLangChanged(nAudioNo);
}

void DVDManager::EmitOnSubtitleLangChanged(int nSubTitleCode)
{
    if(m_bPauseAtSetting == true) {
        m_bPauseAtSetting = false;
        MC_LOG << "DVDManager::EmitOnSubtitleLangChanged(): Pause()" << LOG_ENDL;
        Pause();
    }
    MC_LOG << "DVDManager::EmitOnSubtitleLangChanged" << LOG_ENDL;
    emit OnSubtitleLangChanged(nSubTitleCode);
}

void DVDManager::EmitOnAngleChanged(int nAngleNo)
{
    if(m_bPauseAtSetting == true) {
        m_bPauseAtSetting = false;
        MC_LOG << "DVDManager::EmitOnAngleChanged(): Pause()" << LOG_ENDL;
        Pause();
    }
    MC_LOG << "DVDManager::EmitOnAngleChanged" << LOG_ENDL;
    emit OnAngleChanged(nAngleNo);
}

int DVDManager::GetRandomMode(DECK_DVD_RANDOM_MODE eMode)
{
    switch(eMode)
    {
    case eDM_DVD_RANDOM_OFF:                    return RANDOMOFF;
    case eDM_DVD_RANDOM_CHAPTER_IN_TITLE:       return RANDOMFILE;
    default:                                    return RANDOMOFF;
    }
}

int DVDManager::GetRepeatMode(DECK_DVD_REPEAT_MODE eMode)
{
    switch(eMode)
    {
    case eDM_DVD_REPEAT_OFF:                    return REPEATALL;
    case eDM_DVD_REPEAT_CHAPTER:                return REPEATFILE;
    case eDM_DVD_REPEAT_TITLE:                  return REPEATFOLDER;
    default:                                    return REPEATALL;
    }
}

int DVDManager::GetScanMode(DECK_DVD_SCAN_MODE eMode)
{
    switch(eMode)
    {
    case eDM_DVD_SCAN_OFF:                      return SCANOFF;
    case eDM_DVD_SCAN_CHAPTER_IN_TITLE:         return SCANALL;
    default:                                    return SCANOFF;
    }
}

void DVDManager::EmitError(int nError)
{
    DeckManager *p = DeckManager::GetInstance();
    if(p) {
        p->EmitError(nError);
    }
}

void DVDManager::ProcessErrResponse()
{
}

void DVDManager::RequestDVDStatus()
{
    if(m_tStatus.m_nPlayStatus == eDM_CD_PLAY_COND_PAUSING) {
        MC_LOG << "DVDManager::RequestDVDStatus() --> RequestDVDAudioVideoVRStatus()1" << LOG_ENDL;
        m_pPacket->RequestDVDAudioVideoVRStatus();
    }

    if(m_tStatus.m_nPlayStatus == eDM_CD_PLAY_COND_HALTING) {
        MC_LOG << "DVDManager::RequestDVDStatus() --> RequestDVDAudioVideoVRStatus()2" << LOG_ENDL;
        m_pPacket->RequestDVDAudioVideoVRStatus();
    }
}

// { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
int DVDManager::GetNextTrackIdx(int nSkiplevel, int moveTrackIndex)
{
    int nTrackIdx = moveTrackIndex; //GetCurrentIndex();
    
    int nNewTrackIdx = nTrackIdx + nSkiplevel % m_tStatus.m_nTotalChapterNo;
    
    if(nNewTrackIdx >= m_tStatus.m_nTotalChapterNo) {
// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifdef DVD_NEW_NAVIGATION_ENABLED
        // Move to the next TITLE if we have reached the last CHAPTER in the current TITLE.
        // But we don't know the number of chapters of the next TITLE, so we have no choice but to move the last CHAPTER in the current TITLE
        nNewTrackIdx = m_tStatus.m_nTotalChapterNo - 1;
#else
        nNewTrackIdx = nNewTrackIdx - m_tStatus.m_nTotalChapterNo;
#endif
// } modified by cychoi 2013.06.30
    }
    
    return nNewTrackIdx;
}

int DVDManager::GetPrevTrackIdx(int nSkiplevel, int moveTrackIndex)
{
    int nTrackIdx = moveTrackIndex; //GetCurrentIndex();
    
    int nNewTrackIdx = nTrackIdx - nSkiplevel % m_tStatus.m_nTotalChapterNo;
    
    if(nNewTrackIdx < 0) {
// { modified by cychoi 2013.06.30 ITS 162637 DVD navigation
#ifdef DVD_NEW_NAVIGATION_ENABLED
        // Move to the previous TITLE if we have reached the first CHAPTER in the current TITLE.
        // But we don't know the number of chapters of the previous TITLE, so we have no choice but to move the first CHAPTER in the current TITLE
        nNewTrackIdx = 0;
#else
        nNewTrackIdx = m_tStatus.m_nTotalChapterNo + nNewTrackIdx;
#endif
// } modified by cychoi 2013.06.30
    }
    
    return nNewTrackIdx;
}
// } modified by cychoi 2013-07-10

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef DVD_DECK_REQ_RETRY_ENABLED
int DVDManager::InitRetryVRInfo()
{
    if(m_tRetryVRInfoTimer.isActive() == true)
    {
        m_tRetryVRInfoTimer.stop();
    }

    return 1;
}

int DVDManager::StartRetryVRInfoTimer(int ms) // modified by cychoi 2013.09.06 for ITS 178797
{
    if(m_tRetryVRInfoTimer.isActive() == true)
    {
        m_tRetryVRInfoTimer.stop();
    }
    m_tRetryVRInfoTimer.start(ms); // modified by cychoi 2013.09.06 for ITS 178797

    return 1;
}

int DVDManager::InitRetryPlayback()
{
    if(m_tRetryPlaybackTimer.isActive() == true)
    {
        m_tRetryPlaybackTimer.stop();
    }

    // removed by cychoi 2014.02.03 for play mode on SW Reset

    return 1;
}

// { added by cychoi 2014.02.03 for play mode on SW Reset
int DVDManager::InitRetryPlaybackMode()
{
    m_nRetryPlaybackRandomMode = eDM_DVD_RANDOM_OFF;
    m_nRetryPlaybackRepeatMode = eDM_DVD_REPEAT_OFF;
    m_nRetryPlaybackScanMode = eDM_DVD_SCAN_OFF;

    return 1;
}
// } added by cychoi 2014.02.03

int DVDManager::StartRetryPlaybackTimer(int ms)
{
    if(m_tRetryPlaybackTimer.isActive() == true)
    {
        m_tRetryPlaybackTimer.stop();
    }
    m_tRetryPlaybackTimer.start(ms);

    return 1;
}
#endif
// } added by cychoi 2013-06-05

#ifdef DVD_NAVIGATION_STILL_PICTURE_ENABLED
int DVDManager::InitStillPictureTimer()
{
    if(m_tStillPictureTimer.isActive() == true)
    {
        m_tStillPictureTimer.stop();
    }

    return 1;
}

int DVDManager::StartStillPictureTimer()
{
    if(m_tStillPictureTimer.isActive() == true)
    {
        m_tStillPictureTimer.stop();
    }
    m_tStillPictureTimer.start(1500); // modified by cychoi 2013.10.26 for ITS 195754

    return 1;
}


bool DVDManager::EnterKeyOnStillPicture()
{
    if(m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_PLAY)
    {
        if(GetCurrentMediaState() == PLAYER_PLAY &&
           m_nStartPlaybackTime == m_tStatus.m_nPlaybackTime)
        {
            if(m_tStillPictureTimer.isActive() == true)
            {
                return false;
            }
            MC_LOG << "DVDManager::EnterKeyOnStillPicture()->EnterKey()" << LOG_ENDL;
            EnterKey();
            m_bEnterKey = true;
            InitStillPictureTimer(); // added by cychoi 20103.10.02 for ITS 188493 DVD Navigation Still Picture -  Handle Play state as Pause state
            return true;
        }
    }

    return false;
}
#endif

// { added by cychoi 2013.10.25 for ITS 195688 Audio repeated request
void DVDManager::ContinueSwitchAudio()
{
    m_nReqSwitchAudio--;
    m_nReqSwitchAudio = m_nReqSwitchAudio % m_tStatus.m_nTotalAudioNo;
    if(m_nReqSwitchAudio > 0)
    {
        m_pPacket->Audio();
    }
}
// } added by cychoi 2013.10.25

#ifdef DVD_DECK_REQ_RETRY_LIMITATION_ENABLED
bool DVDManager::HandleErrorDisc(bool bRetryToPlay)
{
    // { modified by cychoi 2014.04.14 for HW Event PQ launch DVD mode unconditionally after Deck SW reset
    bool bReset = false;

    if(bRetryToPlay == true)
    {
        if(m_tStatus.m_nPlaybackTime != 0)
        {
            return false;
        }
        else
        {
            if(m_nDiscPlayRetryCount < 3 || m_nDiscPlayRetryCount == 4)
            {
                // do Retry first after reset Deck (m_nDiscPlayRetryCount == 4)
                m_nDiscPlayRetryCount++;
                return false;
            }

            if(m_nDiscPlayRetryCount == 3)
            {
                // failed Retry 3 times then reset Deck
                bReset = true;
            }
            m_nDiscPlayRetryCount++;
        }
    }
    else
    {
        if(m_nDiscDetectRetryCount < 3 || m_nDiscDetectRetryCount == 4)
        {
            // do Retry first after reset Deck (m_nDiscDetectRetryCount == 4)
            m_nDiscDetectRetryCount++;
            return false;
        }
        
        if(m_nDiscDetectRetryCount == 3)
        {
            // failed Retry 3 times then reset Deck
            bReset = true;
        }
        m_nDiscDetectRetryCount++;
    }
    // } modified by cychoi 2014.04.14

    MC_HIGH << "DVDManager::HandleErrorDisc: " << m_nDiscPlayRetryCount << ", " << m_nDiscDetectRetryCount << ", " << bReset << LOG_ENDL;
    // Please handle as Error Disc on DVDStatus or Play Request is failed 3 times
    DeckManager *p = DeckManager::GetInstance();
    if(p)
    {
        p->HandleErrorDisc(PLAYER_NOMEDIA, bReset);
        return true;
    }

    return true;
}
#endif

// { added by cychoi 2013.12.26 for ITS 217306 DVD black
int DVDManager::GetInternalMediaState()
{
    switch(m_ePlayerState)
    {
    case eMP_STATE_STOPPED:        return PLAYER_STOP;
    case eMP_STATE_PLAYING:        return PLAYER_PLAY;
    case eMP_STATE_PAUSED:         return PLAYER_PAUSE;
    case eMP_STATE_SEARCHING:      return PLAYER_SEARCH;
    case eMP_STATE_FAST_PLAYING: 
    {
        if(m_bREWMode == false)    return PLAYER_FASTFORWARD;
        else                       return PLAYER_REWIND;
    }
    default:                       return PLAYER_STOP;
    }
}
// } added by cychoi 2013.12.26
