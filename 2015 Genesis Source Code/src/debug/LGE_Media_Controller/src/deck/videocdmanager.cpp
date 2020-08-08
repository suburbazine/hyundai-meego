#include "videocdmanager.h"
#include "util.h"
#include "common.h"
#include "deckmanager.h"
#include "mediamanager.h"
#include "iplayerengine.h"

#define VCD_BLOCKS_PER_SECOND 75

VideoCDManager* VideoCDManager::m_pInstance = 0;

VideoCDManager* VideoCDManager::GetInstance()
{
    if(m_pInstance == 0) {
        m_pInstance = new VideoCDManager();
    }
    
    return m_pInstance;
}

void VideoCDManager::DeleteInstance()
{
    if(m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

VideoCDManager::~VideoCDManager()
{
    MC_LOG << "VideoCDManager::~VideoCDManager()" << LOG_ENDL;
}

void VideoCDManager::Init()
{
    MC_LOG << "VideoCDManager::Init" << LOG_ENDL;
    
    m_tStatus = VideoCDManagerStatus();

    m_nStartTrackNo     = 0;
    m_nLastTrackNo      = 0;
    m_nTotalTrackNum    = 0;
    m_nPlayTrackNo      = 0;
    m_nCurrTrackIndex   = -1;
    
    m_nTotalBlockNum    = 0;
    m_nCurrBlockNo      = 0;
    m_nUserBlockNo      = -1; // added by cychoi 2013.09.03 for ITS 187102 AVP died
    m_ePlayerState      = eMP_STATE_NONE;
    m_eLastPlayerState  = eMP_STATE_NONE; // added by cychoi 2013.09.13 for ITS 185685 Infinite VCD Loading Screen

    m_bLoadComplete     = false;

    // { added by cychoi 2013-07-15 for Prevent fix
    m_nTotalPlayTimeFrm = 0;
    m_nTotalPlayTimeMin = 0;
    m_nTotalPlayTimeSec = 0;
    m_bREWMode          = false;
     // } added by cychoi 2013-07-15
    m_bFFREWStateSigSend = false; // added by sjhyun 2013.09.14 for ITS 189282

    // FOR APP
    // { modified by cychoi 2013.09.06 for ITS 178797
    m_eAppTocFlag       = VCD_DECK_REQ_NONE;
    m_eAppToc2Flag      = VCD_DECK_REQ_NONE;
    // } modified by cychoi 2013.09.06
    m_tToc.valid        = 0;
    m_tToc2.valid       = 0;

    // { modified by cychoi 2013.09.06 for ITS 178797
    m_eTocFlag          = VCD_DECK_REQ_NONE;
    m_eToc2Flag         = VCD_DECK_REQ_NONE;
    // } modified by cychoi 2013.09.06

    m_nUserRandomMode   = -1;
    m_nUserRepeatMode   = -1;
    // { modified by cychoi 2013.11.15 for ITS 205008
    m_nDelayedRandomMode = -1;
    m_nDelayedRepeatMode = -1;
    m_bDelayedModeSet   = false;
    // } modified by cychoi 2013.11.15
    m_bFirstPlay        = false;

    m_tTrackList.clear();
    m_tUserReqTrackList.clear();

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    InitRetryTocInfo();
    InitRetryToc2Info();
    InitRetryPlayback();
    InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
    m_nRetryPlaybackMediaState = eMP_STATE_NONE;
#endif
// } added by cychoi 2013-06-05

#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
    m_nDiscPlayRetryCount = 0;
#endif
}

void VideoCDManager::StartMedia()
{   
    MC_LOG << "VideoCDManager::StartMedia" << LOG_ENDL;
    
    Init();
    
    m_ePlayerState = eMP_STATE_NONE;
    m_eLastPlayerState  = eMP_STATE_NONE; // added by cychoi 2013.09.13 for ITS 185685 Infinite VCD Loading Screen
    
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    InitRetryTocInfo();
#endif
// } added by cychoi 2013-06-05
    // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
    m_pPacket->RequestTOCStatus();
    m_eTocFlag = VCD_DECK_REQ_SENT; // added by cychoi 2013.09.06 for ITS 178797
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    StartRetryTocInfoTimer(200); // modified by cychoi 2013.09.06 for ITS 178797
#endif
// } added by cychoi 2013-06-05
    // } modified by cychoi 2014.04.25

#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
    m_nDiscPlayRetryCount = 0;
#endif
}

void VideoCDManager::EndMedia()
{
}

void VideoCDManager::ProcessPacket(StatusData *pPacket)
{
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
#if _DECK_DIAGNOSTIC_
    if(ExtractResetStart(pPacket) == true)
    {
        return;
    }
#endif
#endif // VCD_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05

    // VIDEO-CD 상태.
    if(ExtractVideoCDStatus(pPacket) == true)
        return;

    // TOC: track 수. 전체 재생시간.
    if(ExtractTOCStatus(pPacket) == true) {
        // moved by cychoi 2013.09.03 for ITS 187102 AVP died
        return;
    }

    // TOC2: 각 track의 재생시간.
    int nRet = ExtractTOC2Status(pPacket);
    if(nRet == 1) {
        return;
    }
    else if(nRet == 2) {
        if(m_bLoadComplete == false)
        {
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
            m_nDiscDetectRetryCount = 0;
            m_nDiscPlayRetryCount = 0;
#endif
            MC_LOG << "VCD LOAD COMPLETE!!!" << LOG_ENDL;
            m_bLoadComplete = true;
            DeckManager::GetInstance()->EmitDiscStateChanged(PLAYER_VCDINSERTED);

// { modified by cychoi 2013.06.20 for SW reset on disc loading
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
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
#endif // VCD_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05
// } modified by cychoi 2013.06.20
        }
        return;
    }

    if(ExtractReqError(pPacket) == true)
        return;
}

int VideoCDManager::ChangeMediaMode(MEDIA_PLAYER_MODE eMode)
{
    m_bControllerChanged = true;
    MediaManager::GetInstance()->SetMediaMode(MP_MODE_DISC, eMode);
    return true;
}

QString VideoCDManager::GetCurrentFileName()
{
    // MC_LOG << "VideoCDManager::GetCurrentFileName" << LOG_ENDL;

    int nTrackIndex = m_nPlayTrackNo - m_nStartTrackNo;
    
    if(m_tTrackList.isEmpty())
    {
        return QString();
    }
    else if(nTrackIndex >= 0 && nTrackIndex < m_tTrackList.count())
    {
        VCDTrackInfo tTrack = m_tTrackList.at(nTrackIndex);
        return tTrack.m_strTitle;
    }
    else {
        return QString();
    }
}

int VideoCDManager::GetCurrentIndex()
{
    int nCurrIdx = m_nPlayTrackNo - m_nStartTrackNo;

    if(nCurrIdx < 0) {
        nCurrIdx = 0; 
        // MC_LOG << "VideoCDManager::GetCurrentIndex() Error: m_nPlayTrackNo(" << m_nPlayTrackNo << ") - m_nStartTrackNo(" << m_nStartTrackNo << ")" << LOG_ENDL;
    }
    else if(nCurrIdx >= m_tTrackList.count()) {
        nCurrIdx = m_tTrackList.count() - 1;
        // MC_LOG << "VideoCDManager::GetCurrentIndex() Error: m_nPlayTrackNo(" << m_nPlayTrackNo << ") - m_nStartTrackNo(" << m_nStartTrackNo << ")" << LOG_ENDL;
    }
    else {
        // MC_LOG << "VideoCDManager::GetCurrentIndex() " << nCurrIdx << ": m_nPlayTrackNo(" << m_nPlayTrackNo << ") - m_nStartTrackNo(" << m_nStartTrackNo << ")" << LOG_ENDL;
    }
    
    return nCurrIdx;
}

void VideoCDManager::SetCurrentIndex(int nIndex)
{
    MC_LOG << "VideoCDManager::SetCurrentIndex(" << nIndex << ")" << LOG_ENDL;
    m_nCurrTrackIndex = nIndex;
}

int VideoCDManager::GetCurrentMediaState()
{
    // { added by cychoi 2013.09.13 for ITS 185685 Infinite VCD Loading Screen
    if(m_eLastPlayerState == eMP_STATE_STOPPED)
    {
        return PLAYER_STOP; // modified by cychoi 2013.09.22
    }
    // } added by cychoi 2013.09.13

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

int VideoCDManager::GetCurrentMediaDuration()
{
    int nDuration = 0;
    int nCurrIdx  = m_nPlayTrackNo - m_nStartTrackNo;

    // MC_LOG << "VideoCDManager::GetCurrentMediaDuration: " << m_nPlayTrackNo << ", " << m_tTrackList.count() << LOG_ENDL;

    if(nCurrIdx >= 0 && nCurrIdx < m_tTrackList.count()) {
        nDuration = m_tTrackList.at(nCurrIdx).m_nDuration;
    }
    else {
        MC_LOG << "VideoCDManager::GetCurrentMediaDuration() Error: m_nPlayTrackNo(" << m_nPlayTrackNo << ") - m_nStartTrackNo(" << m_nStartTrackNo << ")" << LOG_ENDL;
    }
    
    return nDuration;
}

int VideoCDManager::GetCurrentMediaPosition()
{
    return m_tStatus.m_nPlaybackTime;
}

void VideoCDManager::SetCurrentMediaPosition(int position)
{
    if(position < 0) {
        MC_LOG << "VideoCDManager::SetCurrentMediaPosition(" << position << ")" << LOG_ENDL;
        return;
    }
    
    int nMin = (position / 1000) / 60;
    int nSec = (position / 1000) % 60;
    
    m_pPacket->TimeSearch(eDM_MODE_VIDEO_CD_1_1, m_nPlayTrackNo, nMin, nSec);
}

int VideoCDManager::GetDeviceState()
{
    DeckManager *p = DeckManager::GetInstance();
    
    return p->GetDeviceState();
}

void VideoCDManager::GetCurrentMediaInfo(MediaInfo *pMediaInfo)
{
    if(pMediaInfo) {
    }
}

void VideoCDManager::GetDeviceInfo(DeviceInfo *pDeviceInfo)
{
    if(pDeviceInfo) {
    }
}

void VideoCDManager::GetPlaylist(PlayList *pPlaylist)
{
    if(pPlaylist) 
    {
        int nIndex = m_nPlayTrackNo - m_nStartTrackNo;
        if(nIndex < 0) nIndex = 0;
        
        pPlaylist->index        = nIndex;
        pPlaylist->position     = (double)m_tStatus.m_nPlaybackTime;
        pPlaylist->PlaylistName = QString(); //m_strAlbumTitle; // commented by cychoi 2014.01.24 for unused var
        pPlaylist->mode         = GetCurrentMediaState();
    
        VCDTrackInfo tTrack;
        for(int i = 0; i < m_tTrackList.count(); i++) {
            tTrack = m_tTrackList.at(i);
            pPlaylist->list->append(tTrack.m_strTitle);
        }
    }
}

void VideoCDManager::SetPlayList(PlayList *pPlayList)
{
    Q_UNUSED(pPlayList);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

bool VideoCDManager::SeekableMedia()
{
    return false;
}

void VideoCDManager::SetBrightness(double fBrightness)
{
    DeckManager::GetInstance()->SetBrightness((int)fBrightness);
}

void VideoCDManager::SetContrast(double fContrast)
{
    DeckManager::GetInstance()->SetContrast((int)fContrast);
}

void VideoCDManager::SetTint(double fTint)
{
    DeckManager::GetInstance()->SetTint((int)fTint);
}

void VideoCDManager::SetBrightness(MEDIA_DISPLAY displayPlane, double fBrightness)
{
    Q_UNUSED(displayPlane);
    SetBrightness(fBrightness);
}

void VideoCDManager::SetContrast(MEDIA_DISPLAY displayPlane, double fContrast)
{
    Q_UNUSED(displayPlane);
    SetContrast(fContrast);
}

void VideoCDManager::SetTint(MEDIA_DISPLAY displayPlane, double fTint)
{
    Q_UNUSED(displayPlane);
    SetTint(fTint);
}

double VideoCDManager::GetBrightness()
{
    return (double)DeckManager::GetInstance()->GetBrightness();
}

double VideoCDManager::GetContrast()
{
    return (double)DeckManager::GetInstance()->GetContrast();
}

double VideoCDManager::GetTint()
{
    return (double)DeckManager::GetInstance()->GetTint();
}

void VideoCDManager::SetSystemState(int nSystemState)
{
    m_nSystemState = nSystemState;
    // { added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
    switch(m_nSystemState)
    {
        case SYSTEM_BT_CALL_START:
            m_bBTCallStart = true;
            MC_HIGH << "VideoCDManager::SetSystemState(): SYSTEM_BT_CALL_START" << LOG_ENDL;
            break;
        case SYSTEM_BT_CALL_END:
            m_bBTCallStart = false;
            MC_HIGH << "VideoCDManager::SetSystemState(): SYSTEM_BT_CALL_END" << LOG_ENDL;
            break;
        default:
            break;
    }
    // } added by cychoi 2013.08.01
}

QWidget* VideoCDManager::GetVideoWidget()
{
    return DeckManager::GetInstance()->GetVideoWidget();
}

void VideoCDManager::EnableSubTitle(bool bEnable)
{
    Q_UNUSED(bEnable);
}

void VideoCDManager::SetPlayerEngine(PLAYER_ENGINE_TYPE eType)
{
    Q_UNUSED(eType);
}

void VideoCDManager::SetFontSize(const int fontSize)
{
    Q_UNUSED(fontSize);
}

bool VideoCDManager::ClearDisplay(bool isBackground)
{
    return DeckManager::GetInstance()->ClearDisplay(isBackground);
}

void VideoCDManager::SetHue(double value)
{
    Q_UNUSED(value);
}

void VideoCDManager::SetHue(MEDIA_DISPLAY displayPlane, double value)
{
    Q_UNUSED(displayPlane);
    Q_UNUSED(value);
}

double VideoCDManager::GetHue()
{
    return 0.0;
}

void VideoCDManager::GetCurrentFileFolderIndex(int *pFileIdx, int *pTotalFiles)
{
    *pFileIdx    = GetCurrentIndex();
    *pTotalFiles = m_tTrackList.count();
}

void VideoCDManager::AudioDragStart()
{
    
}

void VideoCDManager::AudioDragEnd()
{
    
}

void VideoCDManager::FrameAdvance()
{
    m_pPacket->FrameAdvance();
}

void VideoCDManager::FrameBack()
{
    m_pPacket->FrameBack();
}


void VideoCDManager::SubTitle(uint mode)
{
    Q_UNUSED(mode);
    
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::Load()
{
    if(DeckManager::GetInstance()->IsExistCDInDeck() == true)
    {
        MC_LOG << "VideoCDManager::Load()" << LOG_ENDL;
        m_pPacket->Loading();
    }
}

void VideoCDManager::Eject(uint mode)
{
    if(DeckManager::GetInstance()->IsExistCDInDeck() == true || mode == 1)
    {
        // { added by cychoi 2013.09.29 for PV1 Disc sound mute for a while
#ifdef VCD_DECK_REQ_RETRY_ENABLED
        InitRetryTocInfo();
        InitRetryToc2Info();
        InitRetryPlayback();
        InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
#endif
        // } added by cychoi 2013.09.29
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
        m_nDiscDetectRetryCount = 0;
        m_nDiscPlayRetryCount = 0;
#endif

        if(GetRepeatMode(m_tStatus.m_nRepeatMode) != REPEATALL) {
            RepeatPlay(REPEATALL);
        }
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }
        
        MC_HIGH << "VideoCDManager::Eject(" << mode << ")" << LOG_ENDL;
        DeckManager::GetInstance()->SetEject(true);
        m_pPacket->FunctionStop();
        m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.09.13 for ITS 185685 Infinite VCD Loading Screen
        DeckManager::GetInstance()->Eject(mode);
        Init();
        m_pPacket->RequestCommonStatus();
    }
}

void VideoCDManager::SlowForward(uint nMode)
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

void VideoCDManager::SlowRewind(uint nMode)
{
    Q_UNUSED(nMode);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::PageUp()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::PageDown()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::Titlemenu(uint nSetting)
{
    Q_UNUSED(nSetting);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::EnterKey()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Enter Key operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    MC_LOG << "VideoCDManager::EnterKey()" << LOG_ENDL;
    m_pPacket->Enter();
}

void VideoCDManager::ReturnKey()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Return Key operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    MC_LOG << "VideoCDManager::ReturnKey()" << LOG_ENDL;
    m_pPacket->Return();
}

void VideoCDManager::CursorKey(uint direction)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Cursor Key operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    MC_LOG << "VideoCDManager::CursorKey(" << direction << ")" << LOG_ENDL;
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

void VideoCDManager::TopMenu(uint setting)
{
    Q_UNUSED(setting);

    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::SwitchSubTitle()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::SwitchAngle()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::SwitchAudio()
{
    m_pPacket->Audio();
}

void VideoCDManager::Search()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::InitializeDeckPrioritySettings(DeckPrioritySettings *pSettings)
{
    DeckManager::GetInstance()->InitializeDeckPrioritySettings(pSettings);
}

void VideoCDManager::InitializeDVDSettings(DVDSettings *pSettings)
{
    Q_UNUSED(pSettings);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

int VideoCDManager::RequestTOC()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't TOC operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

// { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppTocFlag == VCD_DECK_REQ_NONE) {
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
        InitRetryTocInfo();
#endif
// } added by cychoi 2013-06-05
        if(m_pPacket->RequestTOCStatus() > 0)
        {
            m_eAppTocFlag = VCD_DECK_REQ_SENT;
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
            StartRetryTocInfoTimer(200);
#endif
// } added by cychoi 2013-06-05
            return 1;
        }
    }
    else if(m_eAppTocFlag == VCD_DECK_REQ_DONE) {
        emit OnTOCReceived(&m_tToc);
        return 1;
    }
// } modified by cychoi 2013.09.06

    return -1;
}

int VideoCDManager::RequestTOC2(int nBlockNo)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't TOC2 operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

// { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppToc2Flag == VCD_DECK_REQ_NONE || nBlockNo != m_tToc2.block_no) {
#ifdef VCD_DECK_REQ_RETRY_ENABLED
        InitRetryToc2Info();
#endif
        if(m_pPacket->RequestTOCStatus2(nBlockNo) > 0) {
            m_eAppToc2Flag = VCD_DECK_REQ_SENT;
            m_nUserBlockNo = nBlockNo; // added by cychoi 2013.09.03 for ITS 187102 AVP died
#ifdef VCD_DECK_REQ_RETRY_ENABLED
            StartRetryToc2InfoTimer(200);
#endif
            return 1;
        }

        m_eAppToc2Flag = VCD_DECK_REQ_NONE;
    }
    else if(m_eAppToc2Flag == VCD_DECK_REQ_DONE) {
        emit OnTOC2Received(&m_tToc2);
        return 1;
    }
// } modified by cychoi 2013.09.06

    return -1;
}

void VideoCDManager::TenKey(int nKeyID, int pressStatus)
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

void VideoCDManager::DualMonoChange()
{
    m_pPacket->DualMonoChange();
}

int VideoCDManager::GetVCDStatus(VCDStatusResponse *pResponse)
{
    if(pResponse == 0) return -1;

    pResponse->playStatus           = (int)m_tStatus.m_nPlayStatus;
    pResponse->version              = (int)m_tStatus.m_nVersion;
    pResponse->ROMTrack             = 0; // ?
    pResponse->CDTEXTPresent        = DeckManager::GetInstance()->IsExistCDText();
    pResponse->randomModeStatus     = (int)m_tStatus.m_nRandomMode;
    pResponse->repeatModeStatus     = (int)m_tStatus.m_nRepeatMode;
    pResponse->scanModeStatus       = (int)m_tStatus.m_nScanMode;
    pResponse->trackNo              = m_tStatus.m_nTrackNo;
    pResponse->playbackTime         = m_tStatus.m_nPlaybackTime / 1000; // in sec
    pResponse->startTrackNo         = m_nStartTrackNo; // added by cychoi 2013.07.07 for ISV 85460 DVD OSD mismatch

    return 1;
}

int VideoCDManager::GetDVDStatus(DVDStatusResponse *pResponse)
{
    Q_UNUSED(pResponse);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int VideoCDManager::GetDVDPrioritySettings(DeckPrioritySettings *pSettings)
{
    Q_UNUSED(pSettings);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
    return -1;
}

int VideoCDManager::RequestAudioLanguages()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
    return -1;
}

void VideoCDManager::SetAudioLanguage(int nAudioCode, int nIndex)
{
    Q_UNUSED(nAudioCode);
    Q_UNUSED(nIndex);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

int VideoCDManager::RequestSubtitleLanguages()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
    return -1;
}

void VideoCDManager::SetSubtitleLanguage(int nSubtitleCode)
{
    Q_UNUSED(nSubtitleCode);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::SetAngle(int nAngleNumber)
{
    Q_UNUSED(nAngleNumber);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::SetPBC(uint nMode)
{
    DeckManager *p = DeckManager::GetInstance();

    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't PBC operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    if(p) {
        InitialSetting1Command tCmd;
        p->GetInitSetting1Cmd(tCmd);
        if(nMode == 0)
            tCmd.nPBC = 1; // 미표시.
        else
            tCmd.nPBC = 0; // 표시.
        m_pPacket->InitializationSetting1(&tCmd);
        // { added by cychoi 2013.10.17 for SmokeTest Sound Mute on PBC Off
        if(nMode == 0)
        {
            m_bUserTrackChanged = true;
            m_bRealSoundChanged = false;
        }
        // } added by cychoi 2013.10.17
    }
}

void VideoCDManager::SetOSD(uint nMode)
{
    DeckManager::GetInstance()->SetOSD(nMode);
}

void VideoCDManager::SearchTitleChapter(uint nTitle, uint nChapter)
{
    Q_UNUSED(nTitle);
    Q_UNUSED(nChapter);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::CancelAudioLangRequest()
{
    PRINT_NOT_SUPPORTED;
}

void VideoCDManager::CancelSubtitleLangRequest()
{
    PRINT_NOT_SUPPORTED;
}

int VideoCDManager::GetCurrentScreenState()
{
    switch(m_tStatus.m_nDisplayScreen)
    {
    case eDM_DISPLAY_SCREEN_PLAY:        return PLAYER_PLAY_DISPLAY;
    case eDM_DISPLAY_SCREEN_SETTING:     return PLAYER_SETTING_DISPLAY;
    case eDM_DISPLAY_SCREEN_ROOT_MENU:   return PLAYER_TOPMENU_DISPLAY;
    case eDM_DISPLAY_SCREEN_DEFAULT:     return PLAYER_DEFAULTSCREEN_DISPLAY;
    default:                             
        MC_LOG << "VideoCDManager::GetCurrentScreenState Error: " << m_tStatus.m_nDisplayScreen << LOG_ENDL;
        return PLAYER_DEFAULTSCREEN_DISPLAY;
    }
}

void VideoCDManager::Play()
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
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-05
    
    if(m_nCurrTrackIndex >= 0 &&
       (m_ePlayerState == eMP_STATE_PLAYING || m_ePlayerState == eMP_STATE_PAUSED || m_ePlayerState == eMP_STATE_SEARCHING) && 
       (m_nPlayTrackNo - m_nStartTrackNo) != m_nCurrTrackIndex &&
       m_bFirstPlay == true) 
    {
        if(GetCurrentMediaState() == PLAYER_SEARCH) {
            MC_LOG << "VideoCDManager::Play() RETURN ON SEARCH MODE" << LOG_ENDL;
            m_tUserReqTrackList.clear();
            m_tUserReqTrackList.append(m_nCurrTrackIndex);
            return;
        }
        
        BCD4              tBCD4;
        BCD2              tBCD2;
        DirectContCommand tCmd;

        // MC_LOG << "VideoCDManager::1 m_nStartTrackNo: " << m_nStartTrackNo << ", m_nCurrTrackIndex: " << m_nCurrTrackIndex << LOG_ENDL;

        tBCD4.word          = DEC2BCD4(m_nStartTrackNo + m_nCurrTrackIndex);
        tBCD2.BCD.upper     = tBCD4.BCD.upper2;
        tBCD2.BCD.lower     = tBCD4.BCD.lower2;

        tCmd.nFileDesign    = 0;
        tCmd.nPGPLDesggn    = 0;
        tCmd.nUndefined1    = 0;
        tCmd.n1stCateUpper  = 0;
        tCmd.n1stCateLower  = 0;
        tCmd.nUndefined2    = 0;
        tCmd.n2ndCateUpper  = tBCD4.BCD.lower1;
        tCmd.n2ndCateLower  = tBCD2.byte;
        
        MC_LOG << "VideoCDManager::Play()->DirectCont()" << LOG_ENDL;
        m_pPacket->DirectCont(&tCmd);

        m_nCurrTrackIndex = -1;
        m_bAutoTrackChanged = false; // modified by cychoi 20103.08.11 for Play mute
        m_bUserTrackChanged = true; // modified by cychoi 20103.08.11 for ITS 183835 No OSD on track change automatically // added by cychoi 20103.07.22 for OnUserTrackChanged signal
        m_bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
    }
    else 
    {
        // MC_LOG << "2 m_nStartTrackNo: " << m_nStartTrackNo << ", m_nCurrTrackIndex: " << m_nCurrTrackIndex << LOG_ENDL;
        MC_LOG << "VideoCDManager::Play()->Play()" << LOG_ENDL;
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
        m_bDelayedModeSet = (m_nDelayedRandomMode != -1) || (m_nDelayedRepeatMode != -1); // added by cychoi 2013.11.15 for ITS 205008
    }

    m_nFFREWRate = -1; // added by cychoi 20103.06.17 for ITS 174004
    m_nFFREWState = PLAYER_PLAY; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
    m_eLastPlayerState = eMP_STATE_PLAYING; // added by cychoi 2013.09.13 for ITS 185685 Infinite VCD Loading Screen
    
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    StartRetryPlaybackTimer(7500); // VCD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-05
}

void VideoCDManager::Pause()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't pause operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09
    MC_LOG << "VideoCDManager::Pause()" << LOG_ENDL;
    m_pPacket->Pause(); // modified by cychoi 2013.10.23 for ITS 198062 Keep the play state on BT call if pause is forbidden.
}

void VideoCDManager::FunctionStop()
{
    MC_LOG << "VideoCDManager::FunctionStop()" << LOG_ENDL;
    m_tUserReqTrackList.clear();
    m_pPacket->FunctionStop();

    IPlayerEngine *pPlayer = DeckManager::GetInstance()->GetPlayerEngine();
    if(pPlayer) {
        pPlayer->Stop();
        MC_LOG << "VideoCDManager::FunctionStop()--> GPlayer->Stop()" << LOG_ENDL;
    }

    m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.09.13 for ITS 185685 Infinite VCD Loading Screen
}

void VideoCDManager::Stop()
{
    MC_LOG << "VideoCDManager::Stop()" << LOG_ENDL;
    m_tUserReqTrackList.clear();
    m_pPacket->Stop();

    IPlayerEngine *pPlayer = DeckManager::GetInstance()->GetPlayerEngine();
    if(pPlayer) {
        pPlayer->Stop();
        MC_LOG << "VideoCDManager::FunctionStop()--> GPlayer->Stop()" << LOG_ENDL;
    }

    m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.09.13 for ITS 185685 Infinite VCD Loading Screen
}

void VideoCDManager::Next(int nSkipLevel)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't next operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    if(GetCurrentMediaState() == PLAYER_SEARCH) {
        MC_LOG << "VideoCDManager::Next() RETURN ON SEARCH MODE" << LOG_ENDL;
        // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
        int nNewTrackIdx = GetNextTrackIdx(nSkipLevel, m_tUserReqTrackList.count() == 0 ? GetCurrentIndex() : m_tUserReqTrackList.last());
        // } modified by cychoi 2013-07-10
        m_tUserReqTrackList.append(nNewTrackIdx);
        emit OnTrackBufferred(false, nNewTrackIdx, nNewTrackIdx, m_nTotalTrackNum);
        // { added by cychoi 2014.01.06 for ITS 217985
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
        m_bRealSoundChanged = false;
        // } added by cychoi 2014.01.06
        return;
    }

// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-16
    
    if(GetCurrentMediaState() == PLAYER_STOP) {
        MC_LOG << "VideoCDManager::Next(" << nSkipLevel << ") at Stop State" << LOG_ENDL;
        Play();
    }
    else {
        MC_LOG << "VideoCDManager::Next(" << nSkipLevel << ")" << LOG_ENDL;
    }
    
    if(nSkipLevel <= 1) {
        m_pPacket->TrackUp(eDM_NEXT_TRACK);

        emit OnTrackBufferred(false, -1, -1, m_nTotalTrackNum); // added by cychoi 20103.07.20 for new OSD display
    }
    else
    {
        BCD4              tBCD4;
        BCD2              tBCD2;
        DirectContCommand tCmd;
        // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
        int               nNewTrackIdx = GetNextTrackIdx(nSkipLevel, GetCurrentIndex());
        // } modified by cychoi 2013-07-10

        tBCD4.word          = DEC2BCD4(nNewTrackIdx + m_nStartTrackNo);
        tBCD2.BCD.upper     = tBCD4.BCD.upper2;
        tBCD2.BCD.lower     = tBCD4.BCD.lower2;

        tCmd.nFileDesign    = 0;
        tCmd.nPGPLDesggn    = 0;
        tCmd.nUndefined1    = 0;
        tCmd.n1stCateUpper  = 0;
        tCmd.n1stCateLower  = 0;
        tCmd.nUndefined2    = 0;
        tCmd.n2ndCateUpper  = tBCD4.BCD.lower1;
        tCmd.n2ndCateLower  = tBCD2.byte;
        
        m_pPacket->DirectCont(&tCmd);

        emit OnTrackBufferred(false, nNewTrackIdx, nNewTrackIdx, m_nTotalTrackNum); // added by cychoi 20103.07.20 for new OSD display
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    m_bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute

// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    StartRetryPlaybackTimer(7500); // VCD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-16
}

void VideoCDManager::Prev(int nSkipLevel, int nForced)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't prev operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    if(GetCurrentMediaState() == PLAYER_SEARCH) {
        MC_LOG << "VideoCDManager::Prev() RETURN ON SEARCH MODE" << LOG_ENDL;
        // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
        int nNewTrackIdx = GetPrevTrackIdx(nSkipLevel, m_tUserReqTrackList.count() == 0 ? GetCurrentIndex() : m_tUserReqTrackList.last());
        // } modified by cychoi 2013-07-10
        m_tUserReqTrackList.append(nNewTrackIdx);
        emit OnTrackBufferred(true, nNewTrackIdx, nNewTrackIdx, m_nTotalTrackNum);
        // { added by cychoi 2014.01.06 for ITS 217985
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
        m_bRealSoundChanged = false;
        // } added by cychoi 2014.01.06
        return;
    }

// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-16

    if(GetCurrentMediaState() == PLAYER_STOP) {
        MC_LOG << "VideoCDManager::Prev(" << nSkipLevel << ", " << nForced << ") at Stop State" << LOG_ENDL;
        Play();
    }
    else {
        MC_LOG << "VideoCDManager::Prev(" << nSkipLevel << ", " << nForced << ")" << LOG_ENDL;
    }
    
    if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF && m_tStatus.m_nPlaybackTime < 3000) 
    {
        Next(nSkipLevel);
    }
    else 
    {
        if(nSkipLevel <= 1 && nForced == 0 && (m_tStatus.m_nPlaybackTime < 3000 || m_tStatus.m_nPBCOff == 0)) {
            m_pPacket->TrackDown(eDM_PREV_TRACK);

            emit OnTrackBufferred(true, -1, -1, m_nTotalTrackNum); // added by cychoi 20103.07.20 for new OSD display
        }
        else {
            // VCD는 3초 이상에서도 이전 TRACK으로 이동한다.
            if(nSkipLevel <= 1 && nForced == 0 && m_tStatus.m_nPlaybackTime >= 3000 && m_tStatus.m_nPBCOff == 1) {
                MC_LOG << "VideoCDManager::Prev() ---> TimeSearch(" << m_nPlayTrackNo << ", 00:00)" << LOG_ENDL;
                m_pPacket->TimeSearch(eDM_MODE_VIDEO_CD_1_1, m_nPlayTrackNo, 0, 0);
                m_bAutoTrackChanged = false; // added by cychoi 20103.07.05 for DQA sound mute on first prev after inserted VCD Disc
                m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
                return;
            }
            
            BCD4              tBCD4;
            BCD2              tBCD2;
            DirectContCommand tCmd;
            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
            int               nNewTrackIdx = GetPrevTrackIdx(nSkipLevel, GetCurrentIndex());
            // } modified by cychoi 2013-07-10

            tBCD4.word          = DEC2BCD4(nNewTrackIdx + m_nStartTrackNo);
            tBCD2.BCD.upper     = tBCD4.BCD.upper2;
            tBCD2.BCD.lower     = tBCD4.BCD.lower2;

            tCmd.nFileDesign    = 0;
            tCmd.nPGPLDesggn    = 0;
            tCmd.nUndefined1    = 0;
            tCmd.n1stCateUpper  = 0;
            tCmd.n1stCateLower  = 0;
            tCmd.nUndefined2    = 0;
            tCmd.n2ndCateUpper  = tBCD4.BCD.lower1;
            tCmd.n2ndCateLower  = tBCD2.byte;
            
            m_pPacket->DirectCont(&tCmd);

            emit OnTrackBufferred(true, nNewTrackIdx, nNewTrackIdx, m_nTotalTrackNum); // added by cychoi 20103.07.20 for new OSD display
        }
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    m_bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute

// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    StartRetryPlaybackTimer(7500); // VCD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-16
}

void VideoCDManager::Scan(int nMode)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't scan operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    MC_LOG << "VideoCDManager::RandomPlay(" << nMode << ")" << LOG_ENDL;
    
    switch(nMode)
    {
    case SCANALL:
        // RANDOM MODE ---> RANDOM OFF
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }
        m_pPacket->ScanOn(eDM_SCAN_TRACK);
        break;
    case SCANFOLDER:
        MC_LOG << "VideoCDManager::Scan() SCANFOLDER is invalid option" << LOG_ENDL;
        break;
    case SCANFILE:
        MC_LOG << "VideoCDManager::Scan() SCANFILE is invalid option" << LOG_ENDL;
        break;
    default:
        m_pPacket->ScanOff();
        break;
    }

    if(nMode != SCANOFF) {
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }

        // { added by cychoi 2013.10.01 for ITS 192388
        if(nMode == SCANALL)
        {
            m_bAutoTrackChanged = false;
            m_bUserTrackChanged = true;
        }
        // } added by cychoi 2013.10.01
    }
}

void VideoCDManager::RandomPlay(int nMode, int bPlay)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't random play operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    Q_UNUSED(bPlay);
    
    MC_LOG << "VideoCDManager::RandomPlay(" << nMode << ")" << LOG_ENDL;

    // { modified by cychoi 2013.11.15 for ITS 205008
    if(m_bFirstPlay == false)
    {
        m_nDelayedRandomMode = nMode;
        return;
    }
    else
    {
        m_nDelayedRandomMode = -1;
    }
    // } modified by cychoi 2013.11.15
    
    int nPrevMode = GetRandomMode(m_tStatus.m_nRandomMode);

    if(nPrevMode != nMode) 
    {
        MC_LOG << "VideoCDManager::RandomPlay(): nMode(" << nMode << ") != nPrevMode(" << nPrevMode << ")" << LOG_ENDL;
        
        if(nMode == RANDOMOFF) {
            m_nUserRandomMode       = -1;
            m_tStatus.m_nRandomMode = eDM_CD_RANDOM_OFF;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef VCD_DECK_REQ_RETRY_ENABLED
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
        else if(nMode == RANDOMFILE) {
            m_tStatus.m_nRandomMode = eDM_CD_RANDOM_TRACK;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef VCD_DECK_REQ_RETRY_ENABLED
            m_nRetryPlaybackRandomMode = m_tStatus.m_nRandomMode;
#endif
// } modified by cychoi 2013.06.20
        }
        else if(nMode == RANDOMFOLDER) {
            MC_LOG << "VideoCDManager::RandomPlay() RANDOMFOLDER is invalid option" << LOG_ENDL;
            return;
        }
        else {
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
        MC_LOG << "VideoCDManager::RandomPlay(): nMode(" << nMode << ") != nPrevMode(" << nPrevMode << ")" << LOG_ENDL;
    }
}

void VideoCDManager::RepeatPlay(int nMode, int bPlay)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't repeat play operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    Q_UNUSED(bPlay);
    
    MC_LOG << "VideoCDManager::RepeatPlay(" << nMode << ")" << LOG_ENDL;

    // { modified by cychoi 2013.11.15 for ITS 205008
    if(m_bFirstPlay == false)
    {
        m_nDelayedRepeatMode = nMode;
        return;
    }
    else
    {
        m_nDelayedRepeatMode = -1;
    }
    // } modified by cychoi 2013.11.15
    
    switch(nMode)
    {
    case REPEATFILE:
        // RANDOM MODE ---> RANDOM OFF
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }
        m_pPacket->RepeatOn(eDM_REPEAT_TRACK);
        break;
    default:
        m_pPacket->RepeatOff();
        break;
    }

    m_nUserRepeatMode = nMode;

    if(nMode == REPEATFILE) {
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }
    }

    // { added by cychoi 2013.10.01 for ITS 192388
    if(GetCurrentMediaState() == PLAYER_PAUSE)
    {
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
    }
    // } added by cychoi 2013.10.01
}

void VideoCDManager::FastForward(int nRate)
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

void VideoCDManager::Rewind(int nRate)
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

void VideoCDManager::NextFolder(PlayList *pPlayList)
{
    Q_UNUSED(pPlayList);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void VideoCDManager::PrevFolder(PlayList *pPlayList)
{
    Q_UNUSED(pPlayList);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
void VideoCDManager::HandleRetryTocInfo()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request toc info status after disc detection is completed. */
        return;
    }

    MC_HIGH << "VideoCDManager::HandleRetryTocInfo()" << LOG_ENDL;

    m_tRetryTocInfoTimer.stop();

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppTocFlag == VCD_DECK_REQ_SENT)
    {
        if(m_pPacket->RequestTOCStatus() > 0)
        {
            /* If we will do not receive the response on retry, we do not know what to do. */
        }
        else
        {
            /* If we will fail the request for retry, we do not know what to do. */
            m_eAppTocFlag = VCD_DECK_REQ_NONE; // added by cychoi 20103.09.04 for AVP Request <-> Deck Controller Internal Request
        }
    }

    if(m_eTocFlag == VCD_DECK_REQ_SENT)
    {
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
        if(HandleErrorDisc(false) == true)
        {
            return;
        }
#endif
        // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
        m_pPacket->RequestTOCStatus();
        /* We will retry 3 times if we will do not receive the response on retry. */
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
        StartRetryTocInfoTimer(200);
#endif
        // } modified by cychoi 2014.04.25
    }
    // } modified by cychoi 2013.09.06
}

void VideoCDManager::HandleRetryToc2Info()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request toc2 info status after disc detection is completed. */
        return;
    }

    MC_HIGH << "VideoCDManager::HandleRetryToc2Info()" << LOG_ENDL;

    m_tRetryToc2InfoTimer.stop();

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppToc2Flag == VCD_DECK_REQ_SENT)
    {
        if(m_pPacket->RequestTOCStatus2(m_nUserBlockNo) > 0)
        {
            /* If we will do not receive the response on retry, we do not know what to do. */
        }
        else
        {
            /* If we will fail the request for retry, we do not know what to do. */
            m_eAppToc2Flag = VCD_DECK_REQ_NONE; // added by cychoi 20103.09.04 for AVP Request <-> Deck Controller Internal Request
        }
    }

    if(m_eToc2Flag == VCD_DECK_REQ_SENT)
    {
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
        if(HandleErrorDisc(false) == true)
        {
            return;
        }
#endif
        // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
        m_pPacket->RequestTOCStatus2(m_nCurrBlockNo);
        /* We will retry 3 times if we will do not receive the response on retry. */
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
        StartRetryToc2InfoTimer(200);
#endif
        // } modified by cychoi 2014.04.25
    }
    // } modified by cychoi 2013.09.06
}

void VideoCDManager::HandleRetryPlayback()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request playback after disc detection is completed. */
        return;
    }

    MC_HIGH << "VideoCDManager::HandleRetryPlayback(): m_nRetryPlaybackMediaState=" << m_nRetryPlaybackMediaState << LOG_ENDL;

    m_tRetryPlaybackTimer.stop();
        
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
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
        // { modified by cychoi 2014.02.03 for play mode on SW Reset
        m_nDelayedRandomMode = GetRandomMode(m_nRetryPlaybackRandomMode);
        m_nDelayedRepeatMode = GetRepeatMode(m_nRetryPlaybackRepeatMode);
        m_bDelayedModeSet = (m_nDelayedRandomMode != RANDOMOFF) || (m_nDelayedRepeatMode != REPEATOFF);
        // } modified by cychoi 2014.02.03
        
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
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

VideoCDManager::VideoCDManager(QObject *parent) : QObject(parent),
m_bLoadComplete(false),
m_pPacket(0),
m_nSystemState(0),
m_nFFREWRate(-1),
m_nFFREWState(PLAYER_PLAY), // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error // added by cychoi 2013-06-07 for ITS 172685
m_bFFREWStateSigSend(false), // added by sjhyun 2013.09.14 for ITS 189282
//m_bFirstBoot(false), // commented by cychoi 20130.07.11 for keeping playback options
m_bControllerChanged(false),
// { modified by cychoi 2013.06.22 for SW reset on disc loading
#ifdef VCD_DECK_REQ_RETRY_ENABLED
m_nRetryPlaybackMediaState(eMP_STATE_NONE),
m_nRetryPlaybackRandomMode(eDM_CD_RANDOM_OFF),
m_nRetryPlaybackRepeatMode(eDM_CD_REPEAT_OFF),
m_nRetryPlaybackScanMode(eDM_CD_SCAN_OFF),
#endif
// } modified by cychoi 2013.06.22
m_bAutoTrackChanged(true), // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
m_bUserTrackChanged(false), // added by cychoi 20103.07.22 for OnUserTrackChanged signal
m_bRealSoundChanged(false), // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
m_bBTCallStart(false) // added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
{
    Init();
    m_pPacket = Packet::GetInstace();

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    connect(&m_tRetryTocInfoTimer, SIGNAL(timeout()), this, SLOT(HandleRetryTocInfo())); 
    connect(&m_tRetryToc2InfoTimer, SIGNAL(timeout()), this, SLOT(HandleRetryToc2Info())); 
    connect(&m_tRetryPlaybackTimer, SIGNAL(timeout()), this, SLOT(HandleRetryPlayback())); 
#endif
// } added by cychoi 2013-06-05
}

void VideoCDManager::RandomNotice(int nMode)
{
    MC_LOG << "VideoCDManager::RandomNotice(" << nMode << ")" << LOG_ENDL;
    
    switch(nMode)
    {
    case RANDOMFILE:
        m_pPacket->RandomOn(eDM_RANDOM_TRACK);
        break;
    case RANDOMFOLDER:
        MC_LOG << "VideoCDManager::RandomNotice() RANDOMFOLDER is invalid option" << LOG_ENDL;
        break;
    default:
        m_pPacket->RandomOff();
        break;
    }
}

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
#if _DECK_DIAGNOSTIC_
bool VideoCDManager::ExtractResetStart(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_RESET_START) return false;

    MC_HIGH << "VideoCDManager::ExtractResetStart: m_nRetryPlaybackMediaState=" << m_nRetryPlaybackMediaState << LOG_ENDL;

// { modified by cychoi 2014.04.14 for HW Event PQ launch DVD mode unconditionally after Deck SW reset
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
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
    if(m_eTocFlag == VCD_DECK_REQ_SENT)
    {
        StartRetryTocInfoTimer(400); // delay 400ms
    }

    if(m_eToc2Flag == VCD_DECK_REQ_SENT)
    {
        StartRetryToc2InfoTimer(400); // delay 400ms
    }

    if(m_eAppTocFlag == VCD_DECK_REQ_SENT)
    {
        StartRetryTocInfoTimer(600); // delay 100ms + Playback retry delay 500ms
    }

    if(m_eAppToc2Flag == VCD_DECK_REQ_SENT)
    {
        StartRetryToc2InfoTimer(600); // delay 100ms + Playback retry delay 500ms
    }
    // } added by cychoi 2013.09.06
    // } modified by cychoi 2013.11.25

    return true;
}
#endif
#endif // VCD_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05

bool VideoCDManager::ExtractVideoCDStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_VIDEO_CD_STATUS) return false;

    VideoCDStatus *pStatus = &pPacket->m_tData.m_tVideoCD;
    int bRealSoundChanged = false; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-05
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscPlayRetryCount = 0;
#endif

    // { modified by cychoi 2013.12.26 for ITS 217306 DVD black
    // { added by cychoi 20103.09.11 for ITS 188786
    if((MediaManager::GetInstance()->GetPrevMediaMode() == MP_MODE_DISC &&
        MediaManager::GetInstance()->GetMediaMode() != MP_MODE_DISC) ||
       (m_eLastPlayerState == eMP_STATE_STOPPED))
    {
        int nPrevState = GetInternalMediaState();
        if(nPrevState == PLAYER_STOP)
        {
            return true;
        }

        // { modified by cychoi 2013.09.13 for ITS 185685 Infinite VCD Loading Screen
        if(m_eLastPlayerState == eMP_STATE_STOPPED)
        {
            m_tStatus.m_nPlayStatus = eDM_CD_PLAY_COND_HALTING;
        }
        else
        {
            m_tStatus.m_nPlayStatus = (DECK_CD_PLAY_COND)pStatus->nPlayStatus;
        }
        // } modified by cychoi 2013.09.13
        m_ePlayerState = (MEDIA_PLAYER_STATE)DeckManager::GetInstance()->PlayCondToPlayerState(m_tStatus.m_nPlayStatus);
#ifdef VCD_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackMediaState = m_ePlayerState;
#endif
        DeckManager::GetInstance()->ControlGPlayerEngine(GetCurrentMediaState(), nPrevState);
        return true;
    }
    // } added by cychoi 2013.09.11
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
#ifdef VCD_DECK_REQ_RETRY_ENABLED
            m_nRetryPlaybackMediaState = m_ePlayerState;
#endif
// } modified by cychoi 2013.06.20
            DeckManager::GetInstance()->ControlGPlayerEngine(GetCurrentMediaState(), nPrevState);
            if(m_tUserReqTrackList.count() > 0 && ePlayerState == eMP_STATE_PLAYING) {
                int nNewTrack = m_tUserReqTrackList.takeLast();
                m_tUserReqTrackList.clear();
                // { modified by cychoi 20103.08.23 for ITS 185414 ID3 Tag Display
                if(m_nPlayTrackNo == (m_nStartTrackNo + nNewTrack))
                {
                    emit OnStateChanged(GetCurrentMediaState());
                    // { added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
                    if(ePlayerState == eMP_STATE_PLAYING && // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
                       m_bAutoTrackChanged == false)
                    {
                        MC_HIGH << "VideoCDManager::ExtractVideoCDStatus(): bRealSoundChanged = true [1]" << LOG_ENDL;
                        bRealSoundChanged = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                        //emit OnRealSoundChanged(); // commented by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                        m_bAutoTrackChanged = true;
                    }
                    // } added by cychoi 20103.06.13
                }
                else
                {
                    MC_LOG << "VideoCDManager::ExtractVideoCDStatus(): nCurrTrack: " << GetCurrentIndex() << ", nNewTrack: " << nNewTrack << LOG_ENDL;
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
                // { rollback by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error 
                // { added by sjhyun 2013.09.14 for ITS 189282
                //if(ePlayerState != eMP_STATE_SEARCHING || m_nFFREWRate == -1)
                {
                    emit OnStateChanged(GetCurrentMediaState());
                }
                // } added by sjhyun 2013.09.14
                // } rollback by cychoi 2013.09.22
                // { added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
                if(ePlayerState == eMP_STATE_PLAYING && // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
                   m_bAutoTrackChanged == false)
                {
                    MC_HIGH << "VideoCDManager::ExtractVideoCDStatus(): bRealSoundChanged = true [2]" << LOG_ENDL;
                    bRealSoundChanged = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                    //emit OnRealSoundChanged(); // commented by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                    m_bAutoTrackChanged = true;
                }
                // } added by cychoi 20103.06.13
            }

            m_bFFREWStateSigSend = (m_ePlayerState == eMP_STATE_FAST_PLAYING); // added by cychoi 2013.10.27 for ITS 198856 
            // { modified by cychoi 2013.11.15 for ITS 205008
            if(m_bDelayedModeSet == true)
            {
                if(m_nDelayedRandomMode != -1)
                {
                    m_tStatus.m_nRandomMode = eDM_CD_RANDOM_NONE;
                    RandomPlay(m_nDelayedRandomMode, false);
                    m_nDelayedRandomMode = -1;
                }
                
                if(m_nDelayedRepeatMode != -1)
                {
                    m_tStatus.m_nRepeatMode = eDM_CD_REPEAT_NONE;
                    RepeatPlay(m_nDelayedRepeatMode, false);
                    m_nDelayedRepeatMode = -1;
                }
            
               m_bDelayedModeSet = false;
            }
            // } modified by cychoi 2013.11.15
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
            // { modified by cychoi 2013.11.07 for SmokeTest DISC cancel REW on reach first track
            if(m_tStatus.m_nTrackNo == m_nStartTrackNo)
            {
                m_nFFREWRate = -1;
                m_nFFREWState = PLAYER_PLAY; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
                m_bFFREWStateSigSend = false; // added by cychoi 2013.10.27 for ITS 198856 
                OnNormalPlay(); // added by cychoi 2013-05-24 Added additional signal for FF/Rew functionality.
            }
            // } modified by cychoi 2013.11.07
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
        // { modified by wonseok.heo for ITS 191745 2013.09.27
        if( m_nFFREWState == PLAYER_PLAY){
            MEDIA_PLAYER_STATE ePlayerState = (MEDIA_PLAYER_STATE)DeckManager::GetInstance()->PlayCondToPlayerState(ePlayStatus);
            if(ePlayerState == eMP_STATE_PLAYING &&
                    m_bAutoTrackChanged == false)
            {
                MC_HIGH << "VideoCDManager::ExtractVideoCDStatus(): bRealSoundChanged = true [3]" << LOG_ENDL;
                bRealSoundChanged = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                //emit OnRealSoundChanged(); // commented by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
                m_bAutoTrackChanged = true;
            }
        }
        //}  modified by wonseok.heo for ITS 191745 2013.09.27
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
    m_tStatus.m_nVersion        = (DECK_VIDEO_CD_VER)pStatus->nVer;
    m_tStatus.m_nPBCOff         = pStatus->nPBC;
    m_tStatus.m_nDiscNo         = BCD2DEC2(pStatus->nDiscNo);
    m_tStatus.m_nAudioOutput    = (DECK_VCD_AUDIO_OUTPUT_MODE)pStatus->nAudioOutputMode;
    m_tStatus.m_bExistEmphasis  = pStatus->nEmphasis;

    ////////////////////////////////////////////////////////
    // 트랙 변경 알림.
    if(pStatus->nTrackNo != 0xFF) {
        int nTrackNo = BCD2DEC2(pStatus->nTrackNo);
        // { modified by cychoi 2013.11.14 for ITS 209100 Skip Track Changed on Search Mode
        //if(ePlayStatus == eDM_CD_PLAY_COND_SEARCHING_IN ||
        //   ePlayStatus == eDM_CD_PLAY_COND_SEARCHING_OUT)
        //{
        //    if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFILE)
        //    {
        //        // Track is not valid on Search Mode, please keep current Track.
        //        nTrackNo = m_nPlayTrackNo;
        //    }
        //}
        // } modified by cychoi 2013.11.14
        if(m_nStartTrackNo <= nTrackNo && nTrackNo <= m_nLastTrackNo) {
            if(m_tStatus.m_nTrackNo != nTrackNo || m_nPlayTrackNo != nTrackNo || m_bControllerChanged == true) {
                //bool bTrackChanged = (bool)(m_tStatus.m_nTrackNo != nTrackNo || m_nPlayTrackNo != nTrackNo); // added by cychoi 2014.04.29 for ITS 233500
                if(m_nUserRandomMode >= 0 && m_tStatus.m_nPlayStatus != eDM_CD_PLAY_COND_PLAYING_FAST) {
                    RandomNotice(m_nUserRandomMode);
                    m_nUserRandomMode = -2;
                }
                else {
                    m_tStatus.m_nTrackNo = nTrackNo;
                    m_nPlayTrackNo       = nTrackNo;
                    //if(m_nPlayTrackNo < m_nStartTrackNo) m_nPlayTrackNo = m_nStartTrackNo;
                    #if 0
                    if(ePrevPlayerState != eMP_STATE_STOPPED && ePrevPlayerState != eMP_STATE_NONE) {
                        MC_LOG << "VideoCDManager::ePrevPlayerState: " << ePrevPlayerState << ". emit OnCurrentMediaStatusChanged()" << LOG_ENDL;
                        emit OnCurrentMediaStatusChanged(PLAYER_EndOfMedia);
                    }
                    #endif
                    // IF FF/REW AND REPEATFILE ---> REPEATOFF
                    if(m_ePlayerState == eMP_STATE_FAST_PLAYING && m_nUserRepeatMode == REPEATFILE) {
                        m_nUserRepeatMode = -1;
                        RepeatPlay(REPEATALL);
                    }
                    //MC_HIGH << "nTrackNo: " << pStatus->nTrackNo << LOG_ENDL;
                    m_bControllerChanged = false;
                    // { added by cychoi 20103.07.22 for add new signal
                    if(m_bUserTrackChanged == true)
                    {
                        emit OnUserTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
                        m_bUserTrackChanged = false;
                    }
                    // } added by cychoi 20103.07.22
                    // { added by cychoi 2014.04.29 for ITS 233500
                    //else
                    //{
                    //    if(bTrackChanged == true)
                    //    {
                    //        emit OnAutoTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
                    //    }
                    //}
                    // } added by cychoi 2014.04.29
                    emit OnTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
                    //emit OnDurationChanged(GetCurrentMediaDuration());
                    // removed by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
                }
            }
            else {
                if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFILE && m_tStatus.m_nPlaybackTime == 0)
                {
                    MC_LOG << "VideoCDManager::nTrackNo2: " << pStatus->nTrackNo << LOG_ENDL;
                    // { added by cychoi 20103.07.22 for add new signal
                    if(m_bUserTrackChanged == true)
                    {
                        emit OnUserTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
                        m_bUserTrackChanged = false;
                    }
                    // } added by cychoi 20103.07.22
                    // { added by cychoi 2014.04.06 for ITS 233500
                    //else
                    //{
                    //    emit OnAutoTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
                    //}
                    // } added by cychoi 2014.04.06
                    emit OnTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
                    // removed by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
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
                            emit OnUserTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
                            m_bUserTrackChanged = false;
                        }
                        // { added by cychoi 2014.04.06 for ITS 233500
                        //else
                        //{
                        //    emit OnAutoTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
                        //}
                        // } added by cychoi 2014.04.06
                    }
                }
                // } added by cychoi 2013.10.11
            }
        }
    }
    // { added by cychoi 2013.09.04 for ITS 186733
    else {
        m_tStatus.m_nTrackNo = 0xFF;
        m_nPlayTrackNo = 0xFF;
    }
    // { added by cychoi 2013.09.04

    ////////////////////////////////////////////////////////
    // 재생 시간 변경 알림.
    if(pStatus->nPlaybackTimeHour10 != 0xF && pStatus->nPlaybackTimeHour1 != 0xF &&
       pStatus->nPlaybackTimeMin10  != 0xF && pStatus->nPlaybackTimeMin1  != 0xF) 
    {
        int nHour = BCD2DEC1(pStatus->nPlaybackTimeHour10) * 10 + BCD2DEC1(pStatus->nPlaybackTimeHour1);
        int nMin  = BCD2DEC1(pStatus->nPlaybackTimeMin10)  * 10 + BCD2DEC1(pStatus->nPlaybackTimeMin1);
        int nSec  = BCD2DEC1(pStatus->nPlaybackTimeSec10)  * 10 + BCD2DEC1(pStatus->nPlaybackTimeSec1);
        int nTime = (nHour * 3600 + nMin * 60 + nSec) * 1000;
        
        if(m_tStatus.m_nPlaybackTime != nTime) {
            if(bStartTime == true) DeckManager::GetInstance()->SetStartPlayTime(nTime);
            DeckManager::GetInstance()->SetLifeTime(nTime);
            m_tStatus.m_nPlaybackTime = nTime;
            emit OnPositionChanged(nTime);
            // { added by cychoi 20103.09.01 for ITS 183910 & 183912 Sound mute if DVD Audio
            if(m_bRealSoundChanged == true)
            {
                MC_HIGH << "VideoCDManager::ExtractVideoCDStatus(): emit OnRealSoundChanged() [1]" << LOG_ENDL;
                emit OnRealSoundChanged();
                m_bRealSoundChanged = false;
            }
            // } added by cychoi 20103.09.01
        }
    }

    ////////////////////////////////////////////////////////
    // 화면 상태 변경 알림.
    DECK_DISPLAY_SCREEN_MODE eDisplay  = (DECK_DISPLAY_SCREEN_MODE)pStatus->nDisplay;
    if(m_tStatus.m_nDisplayScreen != eDisplay) {
        m_tStatus.m_nDisplayScreen = eDisplay;
        emit OnScreenStateChanged(GetCurrentScreenState());
    }

    ////////////////////////////////////////////////////////
    // 모드 변경 알림.
    // MC_LOG << "VideoCDManager::m_tStatus.m_nRandomMode: " << m_tStatus.m_nRandomMode << "-" << pStatus->nRandomMode << LOG_ENDL;
    // MC_LOG << "VideoCDManager::m_tStatus.m_nRepeatMode: " << m_tStatus.m_nRepeatMode << "-" << pStatus->nRepeatMode << LOG_ENDL;
    // MC_LOG << "VideoCDManager::m_tStatus.m_nScanMode :  " << m_tStatus.m_nScanMode << "-" << pStatus->nScanMode << LOG_ENDL;
    DECK_CD_RANDOM_MODE eRandomMode = (DECK_CD_RANDOM_MODE)pStatus->nRandomMode;
    DECK_CD_REPEAT_MODE eRepeatMode = (DECK_CD_REPEAT_MODE)pStatus->nRepeatMode;
    DECK_CD_SCAN_MODE   eScanMode   = (DECK_CD_SCAN_MODE)pStatus->nScanMode;
    //{ modified by cychoi 20130.07.11 for keeping playback options
    if(m_tStatus.m_nRandomMode != eRandomMode && m_nUserRandomMode == -1) {
        m_tStatus.m_nRandomMode = eRandomMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef VCD_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackRandomMode = m_tStatus.m_nRandomMode;
#endif
// } modified by cychoi 2013.06.20
        emit OnModeChanged(GetRandomMode(m_tStatus.m_nRandomMode));
    }      
    if(m_tStatus.m_nRepeatMode != eRepeatMode) {
        m_tStatus.m_nRepeatMode = eRepeatMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef VCD_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackRepeatMode = m_tStatus.m_nRepeatMode;
#endif
// } modified by cychoi 2013.06.20
        emit OnModeChanged(GetRepeatMode(m_tStatus.m_nRepeatMode));
    }
    if(m_tStatus.m_nScanMode != eScanMode) {
        if((m_tStatus.m_nScanMode != eDM_CD_SCAN_OFF&& m_tStatus.m_nScanMode != eDM_CD_SCAN_NONE) && eScanMode == eDM_CD_SCAN_OFF) {
            emit OnScanCompleted();
        }
        m_tStatus.m_nScanMode = eScanMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef VCD_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackScanMode = m_tStatus.m_nScanMode;
#endif
// } modified by cychoi 2013.06.20
        emit OnModeChanged(GetScanMode(m_tStatus.m_nScanMode));
    }

    if(m_nUserRandomMode == -2) m_nUserRandomMode = -1;
    //} modified by cychoi 20130.07.11

// { added by cychoi 2013-07-13 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    if(m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_PLAY)
    {
        switch(m_ePlayerState)
        {
        case eMP_STATE_PLAYING:
        case eMP_STATE_SEARCHING:
        case eMP_STATE_FAST_PLAYING: 
            StartRetryPlaybackTimer(7500); // VCD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
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
        // { added by cychoi 20103.09.11 for ITS 188943
        // { commented by cychoi 2014.04.14 for HW Event DV2-1 DVD sound truncated
        //if(m_tStatus.m_nDisplayScreen == eDM_DISPLAY_SCREEN_PLAY)
        //{
        //    m_bRealSoundChanged = true;
        //}
        //else
        // } commented by cychoi 2014.04.14
        {
            MC_HIGH << "VideoCDManager::ExtractVideoCDStatus(): emit OnRealSoundChanged() [2]" << LOG_ENDL;
            emit OnRealSoundChanged();
            m_bRealSoundChanged = false;
        }
        // } added by cychoi 20103.09.11
    }
    // } added by cychoi 2013.09.01

    return true;
}

bool VideoCDManager::ExtractTOCStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_TOC_STATUS) return false;

    TOCStatus *pStatus = &pPacket->m_tData.m_tTOC;

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    InitRetryTocInfo();
#endif
// } added by cychoi 2013-06-05
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
#endif

    // { modified by cychoi 2013.09.03 for ITS 187102 AVP died
    if(m_bLoadComplete == true && m_eAppTocFlag == VCD_DECK_REQ_SENT) // modified by cychoi 2013.09.06 for ITS 178797
    {
        // FOR APP
        m_tToc.valid            = pStatus->nValid;
        m_tToc.start_track      = m_nStartTrackNo;
        m_tToc.last_track       = m_nLastTrackNo;
        m_tToc.total_play_frame = m_nTotalPlayTimeFrm;
        m_tToc.total_play_min   = m_nTotalPlayTimeMin;
        m_tToc.total_play_sec   = m_nTotalPlayTimeSec;
        m_eAppTocFlag           = m_tToc.valid ? VCD_DECK_REQ_DONE : VCD_DECK_REQ_NONE; // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error // modified by cychoi 2013.09.06 for ITS 178797
        
        emit OnTOCReceived(&m_tToc);
    }
    else if(m_eTocFlag == VCD_DECK_REQ_SENT)
    {
        if(pStatus->nValid == 0) {
            return false;
        }

        m_nStartTrackNo     = BCD2DEC1(pStatus->nStartTrackNo10) * 10 + BCD2DEC1(pStatus->nStartTrackNo1);  // 1
        m_nLastTrackNo      = BCD2DEC1(pStatus->nLastTrackNo10)  * 10 + BCD2DEC1(pStatus->nLastTrackNo1);   // max track number
        // { modified by cychoi 2013.06.06 for SmokeTest VCD flick error
        m_nTotalTrackNum    = m_nLastTrackNo - 1;
        //m_nTotalTrackNum    = m_nLastTrackNo - m_nStartTrackNo + 1;
        // } modified by cychoi 2013.06.06

        m_nPlayTrackNo      = m_nStartTrackNo; // 임시 초기화.

        m_nTotalPlayTimeFrm = BCD2DEC1(pStatus->nTotalPlayTimeFrm10) * 10 + BCD2DEC1(pStatus->nTotalPlayTimeFrm1);
        m_nTotalPlayTimeMin = BCD2DEC1(pStatus->nTotalPlayTimeMin10) * 10 + BCD2DEC1(pStatus->nTotalPlayTimeMin1);
        m_nTotalPlayTimeSec = BCD2DEC1(pStatus->nTotalPlayTimeSec10) * 10 + BCD2DEC1(pStatus->nTotalPlayTimeSec1);

        m_nTotalBlockNum    = (m_nTotalTrackNum + 9) / 10;
        m_nCurrBlockNo      = 0;
        m_eTocFlag          = VCD_DECK_REQ_DONE; // added by cychoi 2013.09.06 for ITS 178797

        MC_LOG << "VideoCDManager::m_nStartTrackNo:" << m_nStartTrackNo << ", m_nLastTrackNo:" << m_nLastTrackNo << ", m_nTotalBlockNum:" << m_nTotalBlockNum <<
                  ", m_nTotalPlayTimeFrm:" << m_nTotalPlayTimeFrm << ", m_nTotalPlayTimeMin:" << m_nTotalPlayTimeMin << ", m_nTotalPlayTimeSec:" << m_nTotalPlayTimeSec << LOG_ENDL;

        // TOC Status 2 요청.
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
        InitRetryToc2Info();
#endif
// } added by cychoi 2013-06-05
        // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
        m_pPacket->RequestTOCStatus2(m_nCurrBlockNo);
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
        m_nDiscDetectRetryCount = 0;
#endif
        m_eToc2Flag = VCD_DECK_REQ_SENT; // added by cychoi 2013.09.06 for ITS 178797
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
        StartRetryToc2InfoTimer(200); // modified by cychoi 2013.09.06 for ITS 178797
#endif
// } added by cychoi 2013-06-05
        // } modified by cychoi 2014.04.25
    }
    // } modified by cychoi 2013.09.03
    
    return true;
}

int VideoCDManager::ExtractTOC2Status(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_TOC_STATUS2) return 0;

    TOCStatus2 *pStatus = &pPacket->m_tData.m_tTOC2;

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
    InitRetryToc2Info();
#endif
// } added by cychoi 2013-06-05
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
#endif

    // { modified by cychoi 2013.09.03 for ITS 187102 AVP died
    if(m_bLoadComplete == true && m_eAppToc2Flag == VCD_DECK_REQ_SENT && // modified by cychoi 2013.09.06 for ITS 178797
       m_nUserBlockNo == pStatus->nBlockNo)
    {
        m_tToc2.valid    = 1;
        m_tToc2.block_no = pStatus->nBlockNo;

        int nIdx = 0;
        for(int nTrack = 0; nTrack < 10; nTrack++) { 
            m_tToc2.play_start[nTrack][0] = BCD2DEC2(pStatus->pPlayStartAddr[nIdx++]);
            m_tToc2.play_start[nTrack][1] = BCD2DEC2(pStatus->pPlayStartAddr[nIdx++]);
            m_tToc2.play_start[nTrack][2] = BCD2DEC2(pStatus->pPlayStartAddr[nIdx++]);
        }

        m_eAppToc2Flag = VCD_DECK_REQ_DONE; // modified by cychoi 2013.09.06 for ITS 178797
        m_nUserBlockNo = -1;

        emit OnTOC2Received(&m_tToc2);
        
        return 1;
    }
    else if(m_eToc2Flag == VCD_DECK_REQ_SENT)
    {
        if(pStatus->nValid == 0) {
            return false;
        }

        int nBlockNo  = pStatus->nBlockNo;
        int nTrackNum = m_nTotalTrackNum - nBlockNo * 10;
        if(nTrackNum > 10) nTrackNum = 10;

        int nIdx   = 0;
        for(int nTrack = 0; nTrack < nTrackNum; nTrack++) 
        {
            VCDTrackInfo tTrack;

            tTrack.m_strTitle = QString("Track %1").arg(m_nStartTrackNo + nBlockNo * 10 + nTrack);
            tTrack.m_nMinAddr = pStatus->pPlayStartAddr[nIdx++];
            tTrack.m_nSecAddr = pStatus->pPlayStartAddr[nIdx++];
            tTrack.m_nFrmAddr = pStatus->pPlayStartAddr[nIdx++];
            tTrack.m_nAddress = AddressToSectors(tTrack.m_nMinAddr, tTrack.m_nSecAddr, tTrack.m_nFrmAddr); 

            m_tTrackList.append(tTrack);
        }

        m_nCurrBlockNo++;
        if(m_nCurrBlockNo < m_nTotalBlockNum) {
            // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
            m_pPacket->RequestTOCStatus2(m_nCurrBlockNo);
#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
            m_nDiscDetectRetryCount = 0;
#endif
            m_eToc2Flag = VCD_DECK_REQ_SENT; // added by cychoi 2013.09.06 for ITS 178797
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
            StartRetryToc2InfoTimer(200); // modified by cychoi 2013.09.06 for ITS 178797
#endif
// } added by cychoi 2013-06-05
            // } modified by cychoi 2014.04.25
            return 1;
        }
        else {
            int             nPlayTime;
            VCDTrackInfo    tTrack1;
            VCDTrackInfo    tTrack2;

            nTrackNum = m_tTrackList.count();

            for(nIdx = 0; nIdx < nTrackNum - 1; nIdx++)
            {
                tTrack1 = m_tTrackList.at(nIdx);
                tTrack2 = m_tTrackList.at(nIdx+1);

                tTrack1.m_nLength = AddressToSectors(tTrack2.m_nMinAddr, tTrack2.m_nSecAddr, tTrack2.m_nFrmAddr) - tTrack1.m_nAddress;

                nPlayTime = tTrack1.m_nLength / VCD_BLOCKS_PER_SECOND;
                tTrack1.m_nDuration = nPlayTime * 1000;
                tTrack1.m_tPlayTime.setHMS(0, nPlayTime/60, nPlayTime%60);

                m_tTrackList.replace(nIdx, tTrack1);
            }

            if(nTrackNum > 0)
            {
                tTrack1 = m_tTrackList.at(nIdx);
                tTrack1.m_nLength = AddressToSectors(m_nTotalPlayTimeMin, m_nTotalPlayTimeSec, m_nTotalPlayTimeFrm) - tTrack1.m_nAddress;

                nPlayTime = tTrack1.m_nLength / VCD_BLOCKS_PER_SECOND;
                tTrack1.m_nDuration = nPlayTime * 1000;
                tTrack1.m_tPlayTime.setHMS(0, nPlayTime/60, nPlayTime%60);

                m_tTrackList.replace(nIdx, tTrack1);
            }

            m_eToc2Flag = VCD_DECK_REQ_DONE; // added by cychoi 2013.09.06 for ITS 178797

            return 2;
        }
    }
    // } modified by cychoi 2013.09.03

    return 1;
}

bool VideoCDManager::ExtractReqError(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_REQ_ERR) return false;

    ReqError *pStatus = &pPacket->m_tData.m_tReqError;

    MC_LOG << "VideoCDManager::ExtractReqError: 0x" << pStatus->nErrorDetail << LOG_ENDL;

    // { added by cychoi 20103.08.23 for ITS 184879 & 186162
    if(m_bAutoTrackChanged == false)
    {
        MC_HIGH << "VideoCDManager::ExtractReqError(): m_bRealSoundChanged = true" << LOG_ENDL;
        m_bRealSoundChanged = true; // added by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
        //emit OnRealSoundChanged(); // commented by cychoi 2013.09.01 for ITS 183910 & 183912 Sound mute/Unmute
        m_bAutoTrackChanged = true;
    }
    // } added by cychoi 20103.08.23

    // { modified by cychoi 2013.10.27 for ITS 198856
    // { modified by wonseok.heo for ITS 191745 2013.09.27
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
    // } modified by wonseok.heo for ITS 191745 2013.09.27
    // } modified by cychoi 2013.10.27

#ifdef VCD_DECK_REQ_RETRY_ENABLED
    InitRetryTocInfo();
    InitRetryToc2Info();
    InitRetryPlayback();
    InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
#endif

    emit OnError(PLAYER_COMMANDNOTSUPPORTED);
    
    return true;
}

int VideoCDManager::GetRandomMode(DECK_CD_RANDOM_MODE eMode)
{
    switch(eMode)
    {
    case eDM_CD_RANDOM_OFF:         return RANDOMOFF;
    case eDM_CD_RANDOM_TRACK:       return RANDOMFILE;
    default:                        return RANDOMOFF;
    }
}

int VideoCDManager::GetRepeatMode(DECK_CD_REPEAT_MODE eMode)
{
    switch(eMode)
    {
    case eDM_CD_REPEAT_OFF:         return REPEATALL;
    case eDM_CD_REPEAT_TRACK:       return REPEATFILE;
    default:                        return REPEATALL;
    }
}

int VideoCDManager::GetScanMode(DECK_CD_SCAN_MODE eMode)
{
    switch(eMode)
    {
    case eDM_CD_SCAN_OFF:           return SCANOFF;
    case eDM_CD_SCAN_TRACK:         return SCANALL;
    default:                        return SCANOFF;
    }
}

UINT4 VideoCDManager::AddressToSectors(int nMinAddr, int nSecAddr, int nFrameAddr)
{
    UINT4 nSectors = nMinAddr * VCD_BLOCKS_PER_SECOND * 60 + nSecAddr * VCD_BLOCKS_PER_SECOND + nFrameAddr;

    return nSectors;
}

void VideoCDManager::EmitError(int nError)
{
    DeckManager *p = DeckManager::GetInstance();
    if(p) {
        p->EmitError(nError);
    }
}

// { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
int VideoCDManager::GetNextTrackIdx(int nSkiplevel, int moveTrackIndex)
{
    int nTrackIdx = moveTrackIndex; //GetCurrentIndex();
    
    int nNewTrackIdx = nTrackIdx + nSkiplevel % m_tTrackList.count();
    
    if(nNewTrackIdx >= m_tTrackList.count()) {
        nNewTrackIdx = nNewTrackIdx - m_tTrackList.count();
    }
    
    return nNewTrackIdx;
}

int VideoCDManager::GetPrevTrackIdx(int nSkiplevel, int moveTrackIndex)
{
    int nTrackIdx = moveTrackIndex; //GetCurrentIndex();
    
    int nNewTrackIdx = nTrackIdx - nSkiplevel % m_tTrackList.count();
    
    if(nNewTrackIdx < 0) {
        nNewTrackIdx = m_tTrackList.count() + nNewTrackIdx;
    }
    
    return nNewTrackIdx;
}
// } modified by cychoi 2013-07-10

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef VCD_DECK_REQ_RETRY_ENABLED
int VideoCDManager::InitRetryTocInfo()
{
    if(m_tRetryTocInfoTimer.isActive() == true)
    {
        m_tRetryTocInfoTimer.stop();
    }

    return 1;
}

int VideoCDManager::StartRetryTocInfoTimer(int ms) // modified by cychoi 2013.09.06 for ITS 178797
{
    if(m_tRetryTocInfoTimer.isActive() == true)
    {
        m_tRetryTocInfoTimer.stop();
    }
    m_tRetryTocInfoTimer.start(ms); // modified by cychoi 2013.09.06 for ITS 178797

    return 1;
}

int VideoCDManager::InitRetryToc2Info()
{
    if(m_tRetryToc2InfoTimer.isActive() == true)
    {
        m_tRetryToc2InfoTimer.stop();
    }

    return 1;
}

int VideoCDManager::StartRetryToc2InfoTimer(int ms) // modified by cychoi 2013.09.06 for ITS 178797
{
    if(m_tRetryToc2InfoTimer.isActive() == true)
    {
        m_tRetryToc2InfoTimer.stop();
    }
    m_tRetryToc2InfoTimer.start(ms); // modified by cychoi 2013.09.06 for ITS 178797

    return 1;
}

int VideoCDManager::InitRetryPlayback()
{
    if(m_tRetryPlaybackTimer.isActive() == true)
    {
        m_tRetryPlaybackTimer.stop();
    }

    // removed by cychoi 2014.02.03 for play mode on SW Reset

    return 1;
}

// { added by cychoi 2014.02.03 for play mode on SW Reset
int VideoCDManager::InitRetryPlaybackMode()
{
    m_nRetryPlaybackRandomMode = eDM_CD_RANDOM_OFF;
    m_nRetryPlaybackRepeatMode = eDM_CD_REPEAT_OFF;
    m_nRetryPlaybackScanMode = eDM_CD_SCAN_OFF;

    return 1;
}
// } added by cychoi 2014.02.03

int VideoCDManager::StartRetryPlaybackTimer(int ms)
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

#ifdef VCD_DECK_REQ_RETRY_LIMITATION_ENABLED
bool VideoCDManager::HandleErrorDisc(bool bRetryToPlay)
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

    MC_HIGH << "VideoCDManager::HandleErrorDisc: " << m_nDiscPlayRetryCount << ", " << m_nDiscDetectRetryCount << ", " << bReset << LOG_ENDL;
    // Please handle as Error Disc on TOC or Play Request is failed 3 times
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
int VideoCDManager::GetInternalMediaState()
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
