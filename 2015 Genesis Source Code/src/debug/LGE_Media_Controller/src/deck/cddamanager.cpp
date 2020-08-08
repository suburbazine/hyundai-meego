#include "cddamanager.h"
#include "util.h"
#include "common.h"
#include "deckmanager.h"
#include "packetbuffer.h"
#include "mediamanager.h"

#define CD_BLOCKS_PER_SECOND 75

CDDAManager* CDDAManager::m_pInstance = 0;

CDDAManager* CDDAManager::GetInstance()
{
    if(m_pInstance == 0) {
        m_pInstance = new CDDAManager();
    }
    
    return m_pInstance;
}

void CDDAManager::DeleteInstance()
{
    if(m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

CDDAManager::~CDDAManager()
{
    MC_LOG << "CDDAManager::~CDDAManager()" << LOG_ENDL;
}

void CDDAManager::Init()
{
    MC_LOG << "CDDAManager::Init" << LOG_ENDL;
    
    m_tStatus = CDDAManagerStatus();

    m_nStartTrackNo     = 0;
    m_nLastTrackNo      = 0;
    m_nTotalTrackNum    = 0;
    m_nPlayTrackNo      = 0;
    m_nCurrTrackIndex   = -1;
    m_nNextTrackNo      = 0; // added by cychoi 20103.08.14 for ITS 184423
    
    m_nTotalBlockNum    = 0;
    m_nCurrBlockNo      = 0;
    m_nUserBlockNo      = -1; // added by cychoi 2013.09.06 for ITS 178797
    m_ePlayerState      = eMP_STATE_NONE;
    m_eLastPlayerState  = eMP_STATE_NONE; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State

    m_bLoadComplete     = false;

    // { modified by cychoi 2013.09.06 for ITS 178797
    m_eAppTocFlag       = CDDA_DECK_REQ_NONE;
    m_eAppToc2Flag      = CDDA_DECK_REQ_NONE;
    m_eAppCDTextFlag    = CDDA_DECK_REQ_NONE;
    m_eAppCDTextInfoFlag= CDDA_DECK_REQ_NONE;
    // } modified by cychoi 2013.09.06
    m_tToc.valid        = 0;
    m_tToc2.valid       = 0;
    m_tCDText.valid     = 0;
    m_tCDTextInfo.valid = 0;
    m_nAppTrackNo       = -1;
    m_nAppDataType      = -1;
    m_nAppBlockNo       = -1;
    m_nAppFrameNo       = -1;
    m_eAppGNTocFlag     = CDDA_DECK_REQ_NONE; // modified by cychoi 2013.09.06 for 178797
    m_nGNTocSize        = 0;

    // { modified by cychoi 2013.09.06 for ITS 178797
    m_eTocFlag          = CDDA_DECK_REQ_NONE;
    m_eToc2Flag         = CDDA_DECK_REQ_NONE;
    m_eCDTextInfoFlag   = CDDA_DECK_REQ_NONE;
    // } modified by cychoi 2013.09.06
    
    m_nUserRandomMode   = -1;
    m_nUserRepeatMode   = -1;
    // { modified by cychoi 2013.11.15 for ITS 205008
    m_nDelayedRandomMode = -1;
    m_nDelayedRepeatMode = -1;
    m_bDelayedModeSet   = false;
    // } modified by cychoi 2013.11.15
    // { modified by cychoi 2013.11.18 for ITS 209699
    m_nDelayedScanMode  = -1;
    m_bDelayedScanModeSet = false;
    // } modified by cychoi 2013.11.18
    m_bFirstPlay        = false;
    m_nUserTimeSearch   = -1;
    m_bREWMode          = false;
    m_bFFREWStateSigSend = false; // added by sjhyun 2013.09.14 for ITS 189282

    //m_bSearchingMode    = false; // commented by cychoi 20103.06.11 for ITS 173068
    
    m_tTrackList.clear();
    //m_tBackupList.clear(); // commented by cychoi 2014.01.24 for MLT spec out
    m_tUserReqTrackList.clear();

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    InitRetryGNTocInfo();
    InitRetryTocInfo();
    InitRetryToc2Info();
    InitRetryCDTextInfo();
    InitRetryPlayback();
    InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
    m_nRetryPlaybackMediaState = eMP_STATE_NONE;
#endif
// } added by cychoi 2013-06-05

#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
    m_nDiscPlayRetryCount = 0;
#endif

    // { modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050 // { commented by cychoi 2013.07.13 for MLT spec out
    m_ePlayDirection = CDDA_PLAY_NONE; // added by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
    // } modified by cychoi 2014.08.29 // } commented by cychoi 2013.07.13
}

void CDDAManager::StartMedia()
{
    MC_LOG << "CDDAManager::StartMedia" << LOG_ENDL;

    Init();
    
    m_ePlayerState = eMP_STATE_NONE;
    m_eLastPlayerState = eMP_STATE_NONE; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    InitRetryCDTextInfo();
#endif
// } added by cychoi 2013-06-05
    // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
    m_pPacket->RequestCDTextInfo();
    m_eCDTextInfoFlag = CDDA_DECK_REQ_SENT; // addied by cychoi 2013.09.06 for ITS 178797
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    StartRetryCDTextInfoTimer(200);
#endif
// } added by cychoi 2013-06-05
    // } modified by cychoi 2014.04.25
    InitRetryTocInfo();
    // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
    m_pPacket->RequestTOCStatus();
    m_eTocFlag = CDDA_DECK_REQ_SENT; // addied by cychoi 2013.09.06 for ITS 178797
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    StartRetryTocInfoTimer(200);
#endif
// } added by cychoi 2013-06-05
    // } modified by cychoi 2014.04.25

#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
    m_nDiscPlayRetryCount = 0;
#endif
}

void CDDAManager::EndMedia()
{
    
}

void CDDAManager::ProcessPacket(StatusData *pPacket)
{
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
#if _DECK_DIAGNOSTIC_
    if(ExtractResetStart(pPacket) == true)
    {
        return;
    }
#endif
#endif // CDDA_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05

    // CDDA 상태.
    if(ExtractCDDAStatus(pPacket) == true) {
        if(m_nUserTimeSearch >= 0) SetCurrentMediaPosition(m_nUserTimeSearch);
        return;
    }

    // TOC: track 수. 전체 재생시간.
    if(ExtractTOCStatus(pPacket) == true) {
        // moved by cychoi 2013.09.06 for ITS 178797
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
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
            m_nDiscDetectRetryCount = 0;
            m_nDiscPlayRetryCount = 0;
#endif
            MC_LOG << "CDDA LOAD COMPLETE!!!" << LOG_ENDL;
            m_bLoadComplete = true;
            DeckManager::GetInstance()->EmitDiscStateChanged(PLAYER_CDDAINSERTED);
            m_pPacket->RequestCDStatus();

// { modified by cychoi 2013.06.20 for SW reset on disc loading
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
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
#endif // CDDA_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05
// } modified by cychoi 2013.06.20
        }
        return;
    }

    // CD-TEXT
    if(ExtractCDTextStatus(pPacket) == true) {
        return;
    }

    // CD-TEXT INFO.
    if(ExtractCDTextInfoStatus(pPacket) == true) {
        return;
    }

    if(ExtractReqError(pPacket) == true)
        return;
}

int CDDAManager::ChangeMediaMode(MEDIA_PLAYER_MODE eMode)
{
    m_bControllerChanged = true;
    MediaManager::GetInstance()->SetMediaMode(MP_MODE_DISC, eMode);
    return true;
}

QString CDDAManager::GetCurrentFileName()
{
    if(m_tTrackList.isEmpty())
    {
        return QString();
    }
    else 
    {
        int nTrackIndex = CheckDiscValid() == true ? m_nPlayTrackNo - m_nStartTrackNo : 0; // modified by cychoi 20103.10.15 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
        if(nTrackIndex >= 0 && nTrackIndex < m_tTrackList.count()) {
            CDDATrackInfo tTrack = m_tTrackList.at(nTrackIndex);
            return tTrack.m_strTitle;
        }
        else {
            return QString();
        }
    }
}

int CDDAManager::GetCurrentIndex()
{
    int nCurrIdx = 0;
    
    // { commented by cychoi 2013.07.13 for MLT spec out
    //if(IsMLTListMode() == true) {
    //    nCurrIdx = GetMLTTrackIndex(m_nPlayTrackNo);
    //}
    //else
    // } commented by cychoi 2013.07.13
    {
        nCurrIdx = m_nPlayTrackNo - m_nStartTrackNo;

        if(nCurrIdx < 0) {
            MC_LOG << "CDDAManager::GetCurrentIndex(" << nCurrIdx << "). m_nPlayTrackNo: " << m_nPlayTrackNo << ", m_nStartTrackNo: " << m_nStartTrackNo << LOG_ENDL;
            nCurrIdx = 0; 
        }
        else if(nCurrIdx >= m_tTrackList.count()) {
            MC_LOG << "CDDAManager::GetCurrentIndex(" << nCurrIdx << "). m_nPlayTrackNo: " << m_nPlayTrackNo << ", m_nStartTrackNo: " << m_nStartTrackNo << LOG_ENDL;
            nCurrIdx = m_tTrackList.count() - 1;
        }
    }

    //MC_LOG << "CDDAManager::GetCurrentIndex(" << nCurrIdx << "). m_nPlayTrackNo: " << m_nPlayTrackNo << ", m_nStartTrackNo: " << m_nStartTrackNo << LOG_ENDL;

    return nCurrIdx;
}

void CDDAManager::SetCurrentIndex(int nIndex)
{
    MC_LOG << "CDDAManager::SetCurrentIndex(" << nIndex << ")" << LOG_ENDL;
    m_nCurrTrackIndex = nIndex;
}

int CDDAManager::GetCurrentMediaState()
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

int CDDAManager::GetCurrentMediaDuration()
{
    int nDuration = 0;
    int nCurrIdx  = 0;
            
    // { commented by cychoi 2013.07.13 for MLT spec out
    //if(IsMLTListMode() == true) {
    //    nCurrIdx = GetMLTTrackIndex(m_nPlayTrackNo);
    //}
    //else
    // } commented by cychoi 2013.07.13
    // { added by cychoi 2013.10.08 for ITS 190742 CDDA duration
    if(GetCurrentMediaState() == PLAYER_SEARCH &&
       m_tUserReqTrackList.count() > 0)
    {
        nCurrIdx = m_tUserReqTrackList.last();
    }
    else
    // } added by cychoi 2013.10.08
    {
        nCurrIdx = CheckDiscValid() == true ? m_nPlayTrackNo - m_nStartTrackNo : 0; // modified by cychoi 20103.10.15 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
    }

    if(nCurrIdx >= 0 && nCurrIdx < m_tTrackList.count()) {
        nDuration = m_tTrackList.at(nCurrIdx).m_nDuration;
    }
    else {
        MC_LOG << "CDDAManager::GetCurrentMediaDuration() Error: m_nPlayTrackNo(" << m_nPlayTrackNo << ") - m_nStartTrackNo(" << m_nStartTrackNo << ")" << LOG_ENDL;
    }

    return nDuration;
}

int CDDAManager::GetCurrentMediaPosition()
{
    return m_tStatus.m_nPlaybackTime;
}

void CDDAManager::SetCurrentMediaPosition(int position)
{
    if(position < 0) {
        MC_LOG << "CDDAManager::SetCurrentMediaPosition(" << position << LOG_ENDL;
        return;
    }
    
    switch(m_tStatus.m_nPlayStatus)
    {
    case eDM_CD_PLAY_COND_PLAYING:
    case eDM_CD_PLAY_COND_PLAYING_SLOW:
    case eDM_CD_PLAY_COND_PLAYING_FAST:
    case eDM_CD_PLAY_COND_SEARCHING_IN:
        {
            int nMin = (position / 1000) / 60;
            int nSec = (position / 1000) % 60;
            MC_LOG << "CDDAManager::SetCurrentMediaPosition(" << position << ")->PlayStatus: " << m_tStatus.m_nPlayStatus << LOG_ENDL;
            m_nUserTimeSearch = -1;
            // { modified by cychoi 20103.08.14 for ITS 184423
            if(m_nNextTrackNo == 0 || m_nNextTrackNo == m_nPlayTrackNo)
            {
                m_pPacket->TimeSearch(eDM_MODE_CD_DA, m_nPlayTrackNo, nMin, nSec);
                m_nNextTrackNo = 0;
                m_bAutoTrackChanged = false; // added by cychoi 2014.03.10 for ITS 228844, ITS 228868
            }
            else if(m_nNextTrackNo > 0)
            {
                m_nUserTimeSearch = position;
            }
            // } modified by cychoi 20103.08.14
        }
        break;
    case eDM_CD_PLAY_COND_PAUSING:
    case eDM_CD_PLAY_COND_SEARCHING_OUT:
        MC_LOG << "CDDAManager::SetCurrentMediaPosition(" << position << ")->PLAY_COND_SEARCHING_OUT" << LOG_ENDL;
        m_nUserTimeSearch = position;
        break;
    default:
        break;
    }        
}

int CDDAManager::GetDeviceState()
{
    DeckManager *p = DeckManager::GetInstance();
    
    return p->GetDeviceState();
}

void CDDAManager::GetCurrentMediaInfo(MediaInfo *pMediaInfo)
{
    if(pMediaInfo) {
    }
}

void CDDAManager::GetDeviceInfo(DeviceInfo *pDeviceInfo)
{
    if(pDeviceInfo) {
    }
}

void CDDAManager::GetPlaylist(PlayList *pPlaylist)
{
    MC_LOG << "CDDAManager::GetPlaylist()" << LOG_ENDL;
    
    if(pPlaylist) 
    {
        int nIndex;
        // { commented by cychoi 2013.07.13 for MLT spec out
        //if(IsMLTListMode() == true) {
        //    nIndex = GetMLTTrackIndex(m_nPlayTrackNo);
        //}
        //else
        // } commented by cychoi 2013.07.13
        {
            nIndex = CheckDiscValid() == true ? m_nPlayTrackNo - m_nStartTrackNo : 0; // modified by cychoi 20103.10.15 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
        }
        if(nIndex < 0) nIndex = 0;
        
        pPlaylist->index        = nIndex;
        pPlaylist->position     = (double)m_tStatus.m_nPlaybackTime;
        pPlaylist->PlaylistName = QString(); //m_strAlbumTitle; // modified by cychoi 2014.01.24 for unused var
        pPlaylist->mode         = GetCurrentMediaState();
    
        CDDATrackInfo tTrack;
        for(int i = 0; i < m_tTrackList.count(); i++) {
            tTrack = m_tTrackList.at(i);
            pPlaylist->list->append(tTrack.m_strTitle);
        }
    }
}

void CDDAManager::SetPlayList(PlayList *pPlayList)
{
    // { modified by cychoi 2014.01.24 for MLT spec out
    Q_UNUSED(pPlayList);
    PRINT_NOT_SUPPORTED;
    //int bRestoreFromBackup = false;

    // Backup from backup list.
    //if(pPlayList == 0) {
    //    bRestoreFromBackup = true;
    //}
    //else if(pPlayList->list == 0) {
    //    bRestoreFromBackup = true;
    //}
    //else if(pPlayList->list->count() <= 0) {
    //    bRestoreFromBackup = true;
    //}
    // Create MLT Playlist.
    //else if(m_tBackupList.count() > 0) {
    //    m_tTrackList = m_tBackupList;
    //    m_tBackupList.clear();
    //}

    //MC_LOG << "CDDAManager::SetPlayList(): bRestoreFromBackup: " << bRestoreFromBackup << LOG_ENDL;

    // Create MLT Playlist.
    //if(bRestoreFromBackup == false)
    //{
        // { commented by cychoi 2013-07-15 for Prevent fix
        //if(pPlayList == 0) {
        //    MC_HIGH << "CDDAManager::SetPlayList(): pPlayList is NULL" << LOG_ENDL;
        //    return;
        //}
        //if(pPlayList->list == 0) {
        //    MC_HIGH << "CDDAManager::SetPlayList(): pPlayList->list is NULL" << LOG_ENDL;
        //    return;
        //}
        // } commented by cychoi 2013-07-15
        
    //    m_tBackupList.clear();
    //    m_tBackupList = m_tTrackList;

    //    m_tTrackList.clear();
        
    //    for(int i = 0; i < pPlayList->list->count(); i++)
    //    {
    //        bool    bOK;
    //        QString strTrack  = pPlayList->list->at(i);
    //        int     nTrackIdx = strTrack.toInt(&bOK, 10);
            
    //        if(bOK == true) {
    //            CDDATrackInfo tInfo = (CDDATrackInfo)m_tBackupList.at(nTrackIdx);
    //            MC_LOG << "CDDAManager::SetPlayList(): " << i << "/" << pPlayList->list->count() << ": " << tInfo.m_nTrackIndex << LOG_ENDL;
    //            m_tTrackList.append(tInfo);
    //        }
    //    }

    //    SetCurrentIndex(pPlayList->index);
    //}
    // Restore from backup.
    //else if(m_tBackupList.size() > 0)
    //{
    //    m_tTrackList.clear();
    //    m_tTrackList = m_tBackupList;
    //    m_tBackupList.clear();
    //}
    // } commented by cychoi 2014.01.24
}

bool CDDAManager::SeekableMedia()
{
    return true;
}

void CDDAManager::SetBrightness(double fBrightness)
{
    DeckManager::GetInstance()->SetBrightness((int)fBrightness);
}

void CDDAManager::SetContrast(double fContrast)
{
    DeckManager::GetInstance()->SetContrast((int)fContrast);
}

void CDDAManager::SetTint(double fTint)
{
    DeckManager::GetInstance()->SetTint((int)fTint);
}

void CDDAManager::SetBrightness(MEDIA_DISPLAY displayPlane, double fBrightness)
{
    Q_UNUSED(displayPlane);
    SetBrightness(fBrightness);
}

void CDDAManager::SetContrast(MEDIA_DISPLAY displayPlane, double fContrast)
{
    Q_UNUSED(displayPlane);
    SetContrast(fContrast);
}

void CDDAManager::SetTint(MEDIA_DISPLAY displayPlane, double fTint)
{
    Q_UNUSED(displayPlane);
    SetTint(fTint);
}

double CDDAManager::GetBrightness()
{
    return (double)DeckManager::GetInstance()->GetBrightness();
}

double CDDAManager::GetContrast()
{
    return (double)DeckManager::GetInstance()->GetContrast();
}

double CDDAManager::GetTint()
{
    return (double)DeckManager::GetInstance()->GetTint();
}

void CDDAManager::SetSystemState(int nSystemState)
{
    m_nSystemState = nSystemState;
    // { added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
    switch(m_nSystemState)
    {
        case SYSTEM_BT_CALL_START:
            m_bBTCallStart = true;
            MC_HIGH << "CDDAManager::SetSystemState(): SYSTEM_BT_CALL_START" << LOG_ENDL;
            break;
        case SYSTEM_BT_CALL_END:
            m_bBTCallStart = false;
            MC_HIGH << "CDDAManager::SetSystemState(): SYSTEM_BT_CALL_END" << LOG_ENDL;
            break;
        default:
            break;
    }
    // } added by cychoi 2013.08.01
}

QWidget* CDDAManager::GetVideoWidget()
{
    return DeckManager::GetInstance()->GetVideoWidget();
}

void CDDAManager::EnableSubTitle(bool bEnable)
{
    Q_UNUSED(bEnable);
}

void CDDAManager::SetPlayerEngine(PLAYER_ENGINE_TYPE eType)
{
    Q_UNUSED(eType);
}

void CDDAManager::SetFontSize(const int fontSize)
{
    Q_UNUSED(fontSize);
}

bool CDDAManager::ClearDisplay(bool isBackground)
{
    Q_UNUSED(isBackground);
    return false;
}

void CDDAManager::SetHue(double value)
{
    Q_UNUSED(value);
}

void CDDAManager::SetHue(MEDIA_DISPLAY displayPlane, double value)
{
    Q_UNUSED(displayPlane);
    Q_UNUSED(value);
}

double CDDAManager::GetHue()
{
    return 0.0;
}

void CDDAManager::GetCurrentFileFolderIndex(int *pFileIdx, int *pTotalFiles)
{
    *pFileIdx    = GetCurrentIndex();
    *pTotalFiles = m_tTrackList.count();
}

void CDDAManager::AudioDragStart()
{
    
}

void CDDAManager::AudioDragEnd()
{
    
}

void CDDAManager::Load()
{
    if(DeckManager::GetInstance()->IsExistCDInDeck() == true)
    {
        MC_LOG << "CDDAManager::Load()" << LOG_ENDL;
        m_pPacket->Loading();
    }
}

void CDDAManager::Eject(uint mode)
{
    if(DeckManager::GetInstance()->IsExistCDInDeck() == true || mode == 1)
    {
        // { added by cychoi 2013.09.29 for PV1 Disc sound mute for a while
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        InitRetryGNTocInfo();
        InitRetryTocInfo();
        InitRetryToc2Info();
        InitRetryCDTextInfo();
        InitRetryPlayback();
        InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
#endif
        // } added by cychoi 2013.09.29
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
        m_nDiscDetectRetryCount = 0;
        m_nDiscPlayRetryCount = 0;
#endif

        if(GetRepeatMode(m_tStatus.m_nRepeatMode) != REPEATALL) {
            RepeatPlay(REPEATALL);
        }
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }

        MC_HIGH << "CDDAManager::Eject(" << mode << ")" << LOG_ENDL;
        DeckManager::GetInstance()->SetEject(true);
        m_pPacket->FunctionStop();
        m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
        DeckManager::GetInstance()->Eject(mode);
        Init();
        m_pPacket->RequestCommonStatus();
    }
}

void CDDAManager::EnterKey()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void CDDAManager::CursorKey(uint direction)
{
    Q_UNUSED(direction);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void CDDAManager::ReturnKey()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void CDDAManager::SwitchPlayMode(bool bCDDAMode)
{
    DeckManager *pDeckMgr = DeckManager::GetInstance();

    if(pDeckMgr == 0)                   return;
    if(pDeckMgr->IsMixedCD() == false)  return;
    
    DECK_MIXED_CD_MODE eSwitchMode = pDeckMgr->GetSwitchMixedMode();
    
    if(bCDDAMode == true && eSwitchMode == eDM_SWITCH_CD_DA_MODE) {
        if(m_pPacket->PlayModeSwitching(eSwitchMode) > 0) {
            pDeckMgr->SetSwitchMixedMode(eDM_SWITCH_COMP_AUDIO);
            MC_LOG << "CDDAManager::SwitchPlayMode to CDDA" << LOG_ENDL;
        }
    }
    else if(eSwitchMode == eDM_SWITCH_COMP_AUDIO) {
        if(m_pPacket->PlayModeSwitching(eSwitchMode) > 0) {
            pDeckMgr->SetSwitchMixedMode(eDM_SWITCH_CD_DA_MODE);
            MC_LOG << "CDDAManager::SwitchPlayMode to CA" << LOG_ENDL;
        }
    }
}

int CDDAManager::RequestTOC()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't TOC operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppTocFlag == CDDA_DECK_REQ_NONE) {
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        InitRetryTocInfo();
#endif
// } added by cychoi 2013-06-05
        if(m_pPacket->RequestTOCStatus() > 0) 
        {
            m_eAppTocFlag = CDDA_DECK_REQ_SENT;
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
            StartRetryTocInfoTimer(200);
#endif
// } added by cychoi 2013-06-05
            return 1;
        }
// } added by cychoi 2013-06-05
    }
    else if(m_eAppTocFlag == CDDA_DECK_REQ_DONE) {
        emit OnTOCReceived(&m_tToc);
        return 1;
    }

    return -1;
    // } modified by cychoi 2013.09.06
}

int CDDAManager::RequestTOC2(int nBlockNo)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't TOC2 operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppToc2Flag == CDDA_DECK_REQ_NONE || m_tToc2.block_no != nBlockNo) {
        m_tToc2.valid = 0;
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        InitRetryToc2Info();
#endif
        if(m_pPacket->RequestTOCStatus2(nBlockNo) > 0) 
        {
            m_eAppToc2Flag = CDDA_DECK_REQ_SENT;
            m_nUserBlockNo = nBlockNo;
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
            StartRetryToc2InfoTimer(200);
#endif
            return 1;
        }

        m_eAppToc2Flag = CDDA_DECK_REQ_NONE;
    }
    else if(m_eAppToc2Flag == CDDA_DECK_REQ_DONE) {
        emit OnTOC2Received(&m_tToc2);
        return 1;
    }

    return -1;
    // } modified by cychoi 2013.09.06
}

int CDDAManager::RequestGraceNoteTOC()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't GraceNote TOC operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppGNTocFlag == CDDA_DECK_REQ_NONE && m_bLoadComplete == true) {
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        InitRetryGNTocInfo();
#endif
// } added by cychoi 2013-06-05
        if(m_pPacket->RequestTOCStatus() > 0) 
        {
            m_eAppGNTocFlag = CDDA_DECK_REQ_SENT;
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
            StartRetryGNTocInfoTimer(200);
#endif
// } added by cychoi 2013-06-05
            return 1;
        }
    }
    else if(m_eAppGNTocFlag == CDDA_DECK_REQ_DONE) {
        emit OnGraceNoteTOCReceived(m_nGNTocSize, (void*)m_pGNTocData);
        return 1;
    }

    return -1;
    // } modified by cychoi 2013.09.06
}

void CDDAManager::TenKey(int nKeyID, int pressStatus)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Ten Key operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    int nOper1 = 0;
    int nOper2 = 0;
    
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

int CDDAManager::RequestCDText(int nTrackNo, int nDataType, int nBlockNo, int nFrameNo)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't CD Text operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppCDTextFlag == CDDA_DECK_REQ_NONE ||
       m_nAppTrackNo != nTrackNo ||
       m_nAppDataType != nDataType ||
       m_nAppBlockNo != nBlockNo ||
       m_nAppFrameNo != nFrameNo)
    {
        CDTextCommand tCmd;
        tCmd.nTrackNo   = DEC2BCD2((UINT1)nTrackNo);
        tCmd.nDataType  = nDataType;
        tCmd.nBlockNo   = nBlockNo;
        tCmd.nFrameNo   = nFrameNo;
        m_tCDText.valid = 0;
        
        if(m_pPacket->RequestCDText(&tCmd) > 0) 
        {
            m_eAppCDTextFlag= CDDA_DECK_REQ_SENT;
            m_nAppTrackNo   = nTrackNo;
            m_nAppDataType  = nDataType;
            m_nAppBlockNo   = nBlockNo;
            m_nAppFrameNo   = nFrameNo;
            return 1;
        }

        m_eAppCDTextFlag = CDDA_DECK_REQ_NONE;
    }
    else if(m_eAppCDTextFlag == CDDA_DECK_REQ_DONE) {
        emit OnCDTextReceived(&m_tCDText);
        return 1;
    }

    return -1;
    // } modified by cychoi 2013.09.06
}

int CDDAManager::RequestCDTextInfo()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't CD Text Info operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppCDTextInfoFlag == CDDA_DECK_REQ_NONE) {
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        InitRetryCDTextInfo();
#endif
// } added by cychoi 2013-06-05
        if(m_pPacket->RequestCDTextInfo() > 0) 
        {
            m_eAppCDTextInfoFlag = CDDA_DECK_REQ_SENT;
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
            StartRetryCDTextInfoTimer(200);
#endif
// } added by cychoi 2013-06-05
            return 1;
        }
    }
    else if(m_eAppCDTextInfoFlag == CDDA_DECK_REQ_DONE) {
        emit OnCDTextInfoReceived(&m_tCDTextInfo);
        return 1;
    }

    return -1;
    // } modified by cychoi 2013.09.06
}

int CDDAManager::RequestDirTree(unsigned int nMode, unsigned int nFolderNo, unsigned int nBlockNo)
{
    Q_UNUSED(nMode);
    Q_UNUSED(nFolderNo);
    Q_UNUSED(nBlockNo);

    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int CDDAManager::RequestTag()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int CDDAManager::RequestTag2(int nInfoType)
{
    Q_UNUSED(nInfoType);

    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int CDDAManager::RequestAllTagInfo()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int CDDAManager::RequestFileName(unsigned int nFolderNo, unsigned int nFileNo)
{
    Q_UNUSED(nFolderNo);
    Q_UNUSED(nFileNo);
    
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
    return -1;
}

int CDDAManager::RequestFolderName(unsigned int nFolderNo)
{
    Q_UNUSED(nFolderNo);
    
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
    return -1;
}

int CDDAManager::GetTotalFileFolder(TotalFileFolderCount *pCount)
{
    Q_UNUSED(pCount);

    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int CDDAManager::GetCDStatus(CDDAStatusResponse *pCDStatus)
{
    if(pCDStatus == 0) return -1;

    pCDStatus->playStatus           = m_tStatus.m_nPlayStatus;
    pCDStatus->ROMTrack             = m_tStatus.m_bCDRomTrack;
    pCDStatus->CDTEXTPresent        = m_tStatus.m_bCDTextExist;
    pCDStatus->randomModeStatus     = m_tStatus.m_nRandomMode;
    pCDStatus->repeatModeStatus     = m_tStatus.m_nRepeatMode;
    pCDStatus->scanModeStatus       = m_tStatus.m_nScanMode;
    pCDStatus->trackNo              = m_tStatus.m_nTrackNo;
    pCDStatus->playbackTime         = m_tStatus.m_nPlaybackTime / 1000; // in secs.
    pCDStatus->startTrackNo         = m_nStartTrackNo;

    return 1;
}

int CDDAManager::GetCAStatus(CAStatusResponse *pCAStatus)
{
    Q_UNUSED(pCAStatus);

    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int CDDAManager::GetCurrentScreenState()
{
    switch(m_tStatus.m_nDisplayScreen)
    {
    case eDM_DISPLAY_SCREEN_SETTING:     return PLAYER_SETTING_DISPLAY;
    case eDM_DISPLAY_SCREEN_DEFAULT:     return PLAYER_DEFAULTSCREEN_DISPLAY;
    default:                             
        MC_LOG << "CDDAManager::GetCurrentScreenState Error: " << m_tStatus.m_nDisplayScreen << LOG_ENDL;
        return PLAYER_DEFAULTSCREEN_DISPLAY;
    }
}


void CDDAManager::DiscTestPlay() //added by wonseok.heo for DISC IN OUT TEST
{
    int nUserTrackIndex = m_nCurrTrackIndex;
    // { commented by cychoi 2013.07.13 for MLT spec out
    //int bMLTListMode = false;
    // } commented by cychoi 2013.07.13

    // { commented by cychoi 2013.07.13 for MLT spec out
    //if(IsMLTListMode() == true) {
    //    bMLTListMode = true;
    //    if(nUserTrackIndex >= 0 && nUserTrackIndex < m_tTrackList.count())
    //        nUserTrackIndex = m_tTrackList.at(nUserTrackIndex).m_nTrackIndex;
    //}
    // } commented by cychoi 2013.07.13

    MC_LOG << "CDDAManager::m_nPlayTrackNo: " << m_nPlayTrackNo << ", m_nStartTrackNo: " << m_nStartTrackNo << ", nUserTrackIndex: " << nUserTrackIndex << LOG_ENDL;

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-05

    BCD4              tBCD4;
    BCD2              tBCD2;
    DirectContCommand tCmd;

    tBCD4.word          = DEC2BCD4(m_nStartTrackNo + nUserTrackIndex);
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

    MC_LOG << "CDDAManager::Play()->DirectCont()" << LOG_ENDL;
    m_pPacket->DirectCont(&tCmd);

    m_nCurrTrackIndex = -1;

    // { modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050 // { commented by cychoi 2013.07.13 for MLT spec out
    m_ePlayDirection = CDDA_PLAY_CURR; // added by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
    // } modified by cychoi 2014.08.29 // } commented by cychoi 2013.07.13

    m_eLastPlayerState = eMP_STATE_PLAYING; // added by cychoi 2013.10.13 for ITS 193952 Sync Stop State

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    m_nRetryTestPlayback = true;
    StartRetryPlaybackTimer(7500); // CD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-05
}

void CDDAManager::Play()
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

    // { added by cychoi 20103.10.15 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
    if(CheckDiscValid() == false)
    {
        return;
    }
    // } added by cychoi 20103.10.15

    int nUserTrackIndex = m_nCurrTrackIndex;
    // { commented by cychoi 2013.07.13 for MLT spec out
    //int bMLTListMode = false;
    // } commented by cychoi 2013.07.13
    
    // { commented by cychoi 2013.07.13 for MLT spec out
    //if(IsMLTListMode() == true) {
    //    bMLTListMode = true;
    //    if(nUserTrackIndex >= 0 && nUserTrackIndex < m_tTrackList.count())
    //        nUserTrackIndex = m_tTrackList.at(nUserTrackIndex).m_nTrackIndex;
    //}
    // } commented by cychoi 2013.07.13

    //MC_LOG << "CDDAManager::m_nPlayTrackNo: " << m_nPlayTrackNo << ", m_nStartTrackNo: " << m_nStartTrackNo << ", nUserTrackIndex: " << nUserTrackIndex << LOG_ENDL;
    
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-05
    
    // { modified by cychoi 20103.06.11 for ITS 173068
    if(nUserTrackIndex >= 0 && ((m_nPlayTrackNo - m_nStartTrackNo) != nUserTrackIndex) && m_bFirstPlay == true) {
    //if(nUserTrackIndex >= 0 && ((m_nPlayTrackNo - m_nStartTrackNo) != nUserTrackIndex || m_bSearchingMode == true) && m_bFirstPlay == true) {
    // } modified by cychoi 20103.06.11
        BCD4              tBCD4;
        BCD2              tBCD2;
        DirectContCommand tCmd;

        tBCD4.word          = DEC2BCD4(m_nStartTrackNo + nUserTrackIndex);
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

        MC_LOG << "CDDAManager::Play()->DirectCont() m_nPlayTrackNo: " << m_nPlayTrackNo << ", m_nStartTrackNo: " << m_nStartTrackNo << ", nUserTrackIndex: " << nUserTrackIndex << LOG_ENDL;
        //MC_LOG << "CDDAManager::Play()->DirectCont()" << LOG_ENDL;
        m_pPacket->DirectCont(&tCmd);

        m_nCurrTrackIndex = -1;
        m_bAutoTrackChanged = false; // modified by shkim 20103.10.01 for AVP mute on Play() call in switchToTuned // modified by cychoi 20103.09.26 for AVP not mute on Play() call // modified by cychoi 20103.08.11 for Play mute
        m_bUserTrackChanged = true; // modified by cychoi 20103.08.11 for ITS 183835 No OSD on track change automatically // added by cychoi 20103.07.22 for OnUserTrackChanged signal
        
        // { moved by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050 // { commented by cychoi 2013.07.13 for MLT spec out
        m_ePlayDirection = CDDA_PLAY_CURR; // added by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
        // } moved by cychoi 2014.08.29 // } commented by cychoi 2013.07.13
    }
    else {
        MC_LOG << "CDDAManager::Play()->Play() m_nPlayTrackNo: " << m_nPlayTrackNo << ", m_nStartTrackNo: " << m_nStartTrackNo << ", nUserTrackIndex: " << nUserTrackIndex << LOG_ENDL;
        //MC_LOG << "CDDAManager::Play()->Play()" << LOG_ENDL;
        m_nCurrTrackIndex = -1;
        m_bFirstPlay      = true;
        // { added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
        if(m_bBTCallStart == false)
            m_pPacket->Play();
        else
            m_pPacket->Pause();
        // { modified by cychoi 20103.10.06 for Play Time OSD is not displayed on AV Mode Change (only MP3 and CDDA)
        if(m_bAutoTrackChanged == true)
        {
            m_bAutoTrackChanged = GetCurrentMediaState() == PLAYER_PLAY; // modified by cychoi 20103.09.26 for AVP not mute on Play() call // modified by cychoi 20103.08.13 for ITS 183912 Sound mute on toggle play // modified by cychoi 20103.08.13 for Play mute
        }
        // } modified by cychoi 20103.10.06
        m_bUserTrackChanged = GetCurrentMediaState() == PLAYER_STOP; // modified by cychoi 20103.08.11 for ITS 183835 No OSD on track change automatically // added by cychoi 20103.07.22 for OnUserTrackChanged signal
        // } added by cychoi 2013.08.01
        m_bDelayedModeSet = (m_nDelayedRandomMode != -1) || (m_nDelayedRepeatMode != -1); // added by cychoi 2013.11.15 for ITS 205008
        // { commented by cychoi 2013.07.13 for MLT spec out
        //if(IsMLTListMode() == true) {
        //    MC_LOG << "CDDAManager::nTrackNo 1: " << GetMLTTrackIndex(m_nPlayTrackNo) << LOG_ENDL;
        //    emit OnTrackChanged(GetMLTTrackIndex(m_nPlayTrackNo));
        //} //{added by Michael.Kim 2013.06.29 for ITS Issue #0176472
        // } commented by cychoi 2013.07.13
        
        // { moved by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050 // { commented by cychoi 2013.07.13 for MLT spec out
        m_ePlayDirection = CDDA_PLAY_NEXT; // added by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
        // } moved by cychoi 2014.08.29 // } commented by cychoi 2013.07.13
    }

    // { commented by cychoi 2013.07.13 for MLT spec out
    //m_ePlayDirection = CDDA_PLAY_CURR; // added by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
    // } commented by cychoi 2013.07.13

    m_nFFREWRate = -1; // added by cychoi 20103.06.17 for ITS 174004
    m_nFFREWState = PLAYER_PLAY; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
    m_eLastPlayerState = eMP_STATE_PLAYING; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    m_nRetryTestPlayback = false;
    StartRetryPlaybackTimer(7500); // CD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-05
}

void CDDAManager::Pause()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't pause operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09
    MC_LOG << "CDDAManager::Pause()" << LOG_ENDL;
    m_pPacket->Pause();
}

void CDDAManager::FunctionStop()
{
    MC_LOG << "CDDAManager::FunctionStop()" << LOG_ENDL;
    m_pPacket->FunctionStop();   
    m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
}

void CDDAManager::Stop()
{    
    MC_LOG << "CDDAManager::Stop()" << LOG_ENDL;
    m_pPacket->Stop();  
    m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
}

void CDDAManager::Next(int nSkipLevel)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't next operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    // { added by cychoi 20103.10.15 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
    if(CheckDiscValid() == false)
    {
        return;
    }
    // } added by cychoi 20103.10.15

    if(GetCurrentMediaState() == PLAYER_SEARCH) {
        MC_LOG << "CDDAManager::Next() RETURN ON SEARCH MODE" << LOG_ENDL;
        // { added by cychoi 2015-05-19 for QE_Smoke Test Ver_45버전(130519) 37 fail
        int nNewTrackIdx;
        if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMOFF)
        {
            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
            nNewTrackIdx = GetNextTrackIdx(nSkipLevel, m_tUserReqTrackList.count() == 0 ? GetCurrentIndex() : m_tUserReqTrackList.last());
            // } modified by cychoi 2013-07-10
        }
        else
        {
            nNewTrackIdx = GetRandomTrackIdx();
        }
        //int nNewTrackIdx = GetNextTrackIdx(nSkipLevel + m_tUserReqTrackList.count());
        // } added by cychoi 2015-05-19
        m_tUserReqTrackList.append(nNewTrackIdx);
        m_nNextTrackNo = nNewTrackIdx + m_nStartTrackNo; // modified by cychoi 20103.08.14 for ITS 184423
        emit OnTrackBufferred(false, nNewTrackIdx, nNewTrackIdx, m_tTrackList.count());
        // { added by cychoi 2014.01.06 for ITS 217985
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
        // } added by cychoi 2014.01.06
        return;
    }
    
// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-16

	// added by edo.lee 2013.01.25
	if(GetScanMode(m_tStatus.m_nScanMode) != SCANOFF) {
	    m_tStatus.m_nScanMode = eDM_CD_SCAN_NONE; 
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackScanMode = m_tStatus.m_nScanMode;
#endif
// } modified by cychoi 2013.06.20
	    Scan(SCANOFF);
	}
    // added by edo.lee 
    if(GetCurrentMediaState() == PLAYER_STOP) {
        MC_LOG << "CDDAManager::Next(" << nSkipLevel << ") at Stop State" << LOG_ENDL;
        Play();
    }

    if(nSkipLevel == 0) nSkipLevel = 1;

    // { added by cychoi 20103.06.11 for ITS 173068
    int nNewTrackIdx;
    if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMOFF)
    {
        // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
        nNewTrackIdx = GetNextTrackIdx(nSkipLevel, GetCurrentIndex());
        // } modified by cychoi 2013-07-10
    }
    else
    {
        nNewTrackIdx = GetRandomTrackIdx();
    }
    //int nNewTrackIdx = GetNextTrackIdx(nSkipLevel);
    // } added by cychoi 20103.06.11
    int nNewTrackNum = m_nStartTrackNo + m_tTrackList.at(nNewTrackIdx).m_nTrackIndex;

    //MC_LOG << "CDDAManager::Next(" << nSkipLevel << ") : nNewTrackIdx: " << nNewTrackIdx << ", nNewTrackNum: " << nNewTrackNum << LOG_ENDL;
    
    // { added by cychoi 2013-07-25 for Smoke Test Track change on Random mode
    // { modified by cychoi 2013.07.13 for MLT spec out
    if(nSkipLevel > 1)
    //if(nSkipLevel > 1 || IsMLTListMode() == true)
    // } modified by cychoi 2013.07.13
    // } added by cychoi 2013-07-25
    {
        BCD4              tBCD4;
        BCD2              tBCD2;
        DirectContCommand tCmd;

        tBCD4.word      = DEC2BCD4(nNewTrackNum);
        tBCD2.BCD.upper = tBCD4.BCD.upper2;
        tBCD2.BCD.lower = tBCD4.BCD.lower2;

        tCmd.nFileDesign    = 0;
        tCmd.nPGPLDesggn    = 0;
        tCmd.nUndefined1    = 0;
        tCmd.n1stCateUpper  = 0;
        tCmd.n1stCateLower  = 0;
        tCmd.nUndefined2    = 0;
        tCmd.n2ndCateUpper  = tBCD4.BCD.lower1;
        tCmd.n2ndCateLower  = tBCD2.byte;

        MC_LOG << "CDDAManager::Next(" << nSkipLevel << ") -> DirectCont() nNewTrackIdx: " << nNewTrackIdx << ", nNewTrackNum: " << nNewTrackNum << LOG_ENDL;
        m_pPacket->DirectCont(&tCmd);

        emit OnTrackBufferred(false, nNewTrackIdx, nNewTrackIdx, m_tTrackList.count()); // added by cychoi 20103.07.20 for new OSD display
    }
    else 
    {
        MC_LOG << "CDDAManager::Next() -> TrackUp()" << LOG_ENDL;
        m_pPacket->TrackUp(eDM_NEXT_TRACK);

        emit OnTrackBufferred(false, -1, -1, m_tTrackList.count()); // added by cychoi 20103.07.20 for new OSD display
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    // { modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050 // { commented by cychoi 2013.07.13 for MLT spec out
    m_ePlayDirection = CDDA_PLAY_NEXT; // added by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
    // } modified by cychoi 2014.08.29 // } commented by cychoi 2013.07.13
    
// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    m_nRetryTestPlayback = false;
    StartRetryPlaybackTimer(7500); // CD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-16
}

void CDDAManager::Prev(int nSkipLevel, int nForced)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't prev operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    // { added by cychoi 20103.10.15 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
    if(CheckDiscValid() == false)
    {
        return;
    }
    // } added by cychoi 20103.10.15

    if(GetCurrentMediaState() == PLAYER_SEARCH) {
        MC_LOG << "CDDAManager::Prev() RETURN ON SEARCH MODE" << LOG_ENDL;
        // { added by cychoi 2015-05-19 for QE_Smoke Test Ver_45버전(130519) 37 fail
        int nNewTrackIdx;
        if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMOFF)
        {
            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
            nNewTrackIdx = GetPrevTrackIdx(nSkipLevel, m_tUserReqTrackList.count() == 0 ? GetCurrentIndex() : m_tUserReqTrackList.last());
            // } modified by cychoi 2013-07-10
        }
        else
        {
            nNewTrackIdx = GetRandomTrackIdx();
        }
        //int nNewTrackIdx = GetPrevTrackIdx(nSkipLevel + m_tUserReqTrackList.count());
        // } added by cychoi 2015-05-19
        m_tUserReqTrackList.append(nNewTrackIdx);
        m_nNextTrackNo = nNewTrackIdx + m_nStartTrackNo; // modified by cychoi 20103.08.14 for ITS 184423
        emit OnTrackBufferred(true, nNewTrackIdx, nNewTrackIdx, m_tTrackList.count());
        // { added by cychoi 2014.01.06 for ITS 217985
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
        // } added by cychoi 2014.01.06
        return;
    }

// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-16

	// added by edo.lee 2013.01.25
	if(GetScanMode(m_tStatus.m_nScanMode) != SCANOFF) {
	    m_tStatus.m_nScanMode = eDM_CD_SCAN_NONE; 
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackScanMode = m_tStatus.m_nScanMode;
#endif
// } modified by cychoi 2013.06.20
	    Scan(SCANOFF);
	}
    // added by edo.lee 

    if(GetCurrentMediaState() == PLAYER_STOP) {
        MC_LOG << "CDDAManager::Prev(" << nSkipLevel << ", " << nForced << ") at Stop State" << LOG_ENDL;
        Play();
    }

    if(nSkipLevel == 0) nSkipLevel = 1;

    if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF && m_tStatus.m_nPlaybackTime < 3000) {
        Next(nSkipLevel);

        // { modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050 // { commented by cychoi 2013.07.13 for MLT spec out
        m_ePlayDirection = CDDA_PLAY_NEXT; // added by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
        // { modified by cychoi 2014.08.29 // } commented by cychoi 2013.07.13
    }
    else
    {
        // { added by cychoi 20103.06.11 for ITS 173068
        int nNewTrackIdx;
        if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMOFF)
        {
            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
            nNewTrackIdx = GetPrevTrackIdx(nSkipLevel, GetCurrentIndex());
            // } modified by cychoi 2013-07-10
        }
        else
        {
            nNewTrackIdx = GetRandomTrackIdx();
        }
        //int nNewTrackIdx = GetPrevTrackIdx(nSkipLevel);
        // } added by cychoi 20103.06.11
        int nNewTrackNum = m_nStartTrackNo + m_tTrackList.at(nNewTrackIdx).m_nTrackIndex;

        MC_LOG << "CDDAManager::Prev(" << nSkipLevel << ") : nNewTrackIdx: " << nNewTrackIdx << ", nNewTrackNum: " << nNewTrackNum << ", m_nPlayTrackNo: " << m_nPlayTrackNo << ", m_nStartTrackNo: " << m_nStartTrackNo << ", nForced: " << nForced << LOG_ENDL; //added by Michael.Kim 2013.05.02 for ITS Issue #0164335
        
        // { added by cychoi 2013-07-25 for Smoke Test Track change on Random mode
        // { modified by cychoi 2013.07.13 for MLT spec out
        if(nSkipLevel > 1 || nForced != 0)
        //if(nSkipLevel > 1 || nForced != 0 || IsMLTListMode() == true)
        // } modified by cychoi 2013.07.13
        // } added by cychoi 2013-07-25
        {
            BCD4              tBCD4;
            BCD2              tBCD2;
            DirectContCommand tCmd;

            tBCD4.word      = DEC2BCD4(nNewTrackNum);
            tBCD2.BCD.upper = tBCD4.BCD.upper2;
            tBCD2.BCD.lower = tBCD4.BCD.lower2;

            tCmd.nFileDesign    = 0;
            tCmd.nPGPLDesggn    = 0;
            tCmd.nUndefined1    = 0;
            tCmd.n1stCateUpper  = 0;
            tCmd.n1stCateLower  = 0;
            tCmd.nUndefined2    = 0;
            tCmd.n2ndCateUpper  = tBCD4.BCD.lower1;
            tCmd.n2ndCateLower  = tBCD2.byte;

            MC_LOG << "CDDAManager::Prev(" << nSkipLevel << ") -> DirectCont() nNewTrackIdx: " << nNewTrackIdx << ", nNewTrackNum: " << nNewTrackNum << ", m_nPlayTrackNo: " << m_nPlayTrackNo << ", m_nStartTrackNo: " << m_nStartTrackNo << LOG_ENDL;
            m_pPacket->DirectCont(&tCmd);

            emit OnTrackBufferred(true, nNewTrackIdx, nNewTrackIdx, m_tTrackList.count()); // added by cychoi 20103.07.20 for new OSD display
        }
        else {
            MC_LOG << "CDDAManager::Prev() -> TrackDown()" << LOG_ENDL;
            m_pPacket->TrackDown(eDM_PREV_TRACK);

            emit OnTrackBufferred(true, -1, -1, m_tTrackList.count()); // added by cychoi 20103.07.20 for new OSD display
        }

        // { modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050 // { commented by cychoi 2013.07.13 for MLT spec out
        m_ePlayDirection = CDDA_PLAY_PREV; // added by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
        // { modified by cychoi 2014.08.29 // } commented by cychoi 2013.07.13
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    
// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    m_nRetryTestPlayback = false;
    StartRetryPlaybackTimer(7500); // CD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-16
}

void CDDAManager::Scan(int nMode)
{
    // { commented by cychoi 2013.07.13 for MLT spec out
    //if(IsMLTListMode() == true && nMode != SCANOFF) return; // modified by cychoi 20103.07.11 for SmokeTest scan off when entered MLT mode
    // } commented by cychoi 2013.07.13

    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't scan operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    // { added by cychoi 20103.10.15 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
    if(CheckDiscValid() == false)
    {
        return;
    }
    // } added by cychoi 20103.10.15

    // { modified by cychoi 2013.11.18 for ITS 209699
    if(GetCurrentMediaState() == PLAYER_SEARCH) 
    {
        if(nMode == SCANALL)
        {
            m_nDelayedScanMode = nMode;
            m_bDelayedScanModeSet = true;
            return;
        }
        else
        {
            m_nDelayedScanMode = -1;
        }
    }
    else
    {
        m_nDelayedScanMode = -1;
    }
    // } modified by cychoi 2013.11.18

    MC_LOG << "CDDAManager::Scan(" << nMode << ")" << LOG_ENDL;

    switch(nMode)
    {
    case SCANALL:
        // RANDOM FILE -> RANDOM OFF.
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }
        m_pPacket->ScanOn(eDM_SCAN_TRACK);
        break;
    case SCANFOLDER:
        MC_LOG << "CDDAManager::Scan() SCANFOLDER is invalid option" << LOG_ENDL;
        break;
    case SCANFILE:
        MC_LOG << "CDDAManager::Scan() SCANFILE is invalid option" << LOG_ENDL;
        break;
    default:
        m_pPacket->ScanOff();
        break;
    }

    // { modified by cychoi 2013.10.01 for ITS 192388
    // { added by wonseok.heo for ITS 189165 unMute
    if(nMode == SCANALL)
    {
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
    }
    // } added by wonseok.heo for ITS 189165 unMute
    // } modified by cychoi 2013.10.01
}

void CDDAManager::RandomPlay(int nMode, int bPlay)
{  
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't random play operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    Q_UNUSED(bPlay);

    // { added by cychoi 20103.10.15 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
    if(CheckDiscValid() == false)
    {
        return;
    }
    // } added by cychoi 20103.10.15
    
    // { commented by cychoi 2013.07.13 for MLT spec out
    //if(IsMLTListMode() == true && nMode != RANDOMOFF) return; // modified by cychoi 20103.07.11 for SmokeTest random off when entered MLT mode
    // } commented by cychoi 2013.07.13
    
    MC_LOG << "CDDAManager::RandomPlay(" << nMode << ")" << LOG_ENDL;

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
        MC_LOG << "CDDAManager::RandomPlay(): nMode(" << nMode << ") != nPrevMode(" << nPrevMode << ")" << LOG_ENDL;
        
        if(nMode == RANDOMOFF) {
            m_nUserRandomMode       = -1;
            m_tStatus.m_nRandomMode = eDM_CD_RANDOM_OFF;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
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
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
            m_nRetryPlaybackRandomMode = m_tStatus.m_nRandomMode;
#endif
// } modified by cychoi 2013.06.20
        }
        else if(nMode == RANDOMFOLDER) {
            MC_LOG << "CDDAManager::RandomPlay() RANDOMFOLDER is invalid option" << LOG_ENDL;
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
            m_tStatus.m_nScanMode = eDM_CD_SCAN_NONE; // added by kihyung 2012.12.20 for 62783(not to emit OnScanCompleted() signal)
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
            m_nRetryPlaybackScanMode = m_tStatus.m_nScanMode;
#endif
// } modified by cychoi 2013.06.20
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

        // { modified by cychoi 2013.10.01 for ITS 192388
        // { added by wonseok.heo for ITS 189165 unMute
        if(GetCurrentMediaState() == PLAYER_PAUSE)
        {
            m_bAutoTrackChanged = false;
            m_bUserTrackChanged = true;
        }
        // } added by wonseok.heo for ITS 189165 unMute
        // } modified by cychoi 2013.10.01
    }
    else {
        MC_LOG << "CDDAManager::RandomPlay(): nMode(" << nMode << ") == nPrevMode(" << nPrevMode << LOG_ENDL;
    }
}

void CDDAManager::RepeatPlay(int nMode, int bPlay)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't repeat play operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    Q_UNUSED(bPlay);
    
    // { added by cychoi 20103.10.15 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
    if(CheckDiscValid() == false)
    {
        return;
    }
    // } added by cychoi 20103.10.15

    MC_LOG << "CDDAManager::RepeatPlay(" << nMode << ")" << LOG_ENDL;

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
    
    int nPrevMode = GetRepeatMode(m_tStatus.m_nRepeatMode);
    
    if(nPrevMode != nMode)
    {
        MC_LOG << "CDDAManager::RepeatPlay(): nMode(" << nMode << ") != nPrevMode(" << nPrevMode << ")" << LOG_ENDL;

        switch(nMode)
        {
        case REPEATFILE:
            // RANDOM FILE -> RANDOM OFF.
            if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
                RandomPlay(RANDOMOFF);
            }
            m_pPacket->RepeatOn(eDM_REPEAT_TRACK);
            break;
        default:
            m_pPacket->RepeatOff();
            break;
        }   

        // { modified by cychoi 2013.10.01 for ITS 192388
        // { added by wonseok.heo for ITS 189165 unMute
        if(GetCurrentMediaState() == PLAYER_PAUSE)
        {
            m_bAutoTrackChanged = false;
            m_bUserTrackChanged = true;
        }
        // } added by wonseok.heo for ITS 189165 unMute
        // } modified by cychoi 2013.10.01

        m_nUserRepeatMode = nMode;
    }
    else {
        MC_LOG << "CDDAManager::RepeatPlay(): nMode(" << nMode << ") != nPrevMode(" << nPrevMode << ")" << LOG_ENDL;
    }
}

void CDDAManager::FastForward(int nRate)
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
        return;
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
    
    // { modified by cychoi 2013.10.21 for ITS 197534 Delay FF on Search Mode by HK repeatition press
    if(m_tUserReqTrackList.count() == 0)
    {
        m_pPacket->FastUp(eSpeed);
    }
    // } modified by cychoi 2013.10.21

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
        /*else if(nRate == FAST_SPEED_X16) {//Unreachable Code 2013.10.06
            m_nFFREWState = PLAYER_FASTFORWARD_X16;
        }*/
        else if(nRate == FAST_SPEED_X20) {
            m_nFFREWState = PLAYER_FASTFORWARD_X20;
        }
        // } commented by cychoi 2013-06-07
        // } modified by cychoi 2013.09.22
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    // { modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050 // { commented by cychoi 2013.07.13 for MLT spec out
    m_ePlayDirection = CDDA_PLAY_FF; // added by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
    // } modified by cychoi 2014.08.29 // } commented by cychoi 2013.07.13
}

void CDDAManager::Rewind(int nRate)
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
        return;
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

    // { modified by cychoi 2013.10.21 for ITS 197534 Delay FF on Search Mode by HK repeatition press
    if(m_tUserReqTrackList.count() == 0)
    {
        m_pPacket->FastDown(eSpeed);
    }
    // } modified by cychoi 2013.10.21

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
        /*else if(nRate == FAST_SPEED_X16) {//Unreachable Code 2013.10.06
            m_nFFREWState = PLAYER_REWIND_X16;
        }*/
        else if(nRate == FAST_SPEED_X20) {
            m_nFFREWState = PLAYER_REWIND_X20;
        }
        // } commented by cychoi 2013-06-07
        // } modified by cychoi 2013.09.22
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
    // { modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050 // { commented by cychoi 2013.07.13 for MLT spec out
    m_ePlayDirection = CDDA_PLAY_REW; // added by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
    // } modified by cychoi 2014.08.29 // } commented by cychoi 2013.07.13
}

void CDDAManager::NextFolder(PlayList *pPlayList)
{
    Q_UNUSED(pPlayList);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void CDDAManager::PrevFolder(PlayList *pPlayList)
{
    Q_UNUSED(pPlayList);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
void CDDAManager::HandleRetryGNTocInfo()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request GraceNote toc info status after disc detection is completed. */
        return;
    }

    MC_HIGH << "CDDAManager::HandleRetryGNTocInfo()" << LOG_ENDL;

    m_tRetryGNTocInfoTimer.stop();

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppGNTocFlag == CDDA_DECK_REQ_SENT)
    {
        if(m_pPacket->RequestTOCStatus() > 0)
        {
            /* If we will do not receive the response on retry, we do not know what to do. */
        }
        else
        {
            /* If we will fail the request for retry, we do not know what to do. */
            m_eAppGNTocFlag = CDDA_DECK_REQ_NONE;
        }
    }
    // } modified by cychoi 2013.09.06
}

void CDDAManager::HandleRetryTocInfo()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request toc info status after disc detection is completed. */
        return;
    }

    MC_HIGH << "CDDAManager::HandleRetryTocInfo()" << LOG_ENDL;

    m_tRetryTocInfoTimer.stop();

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppTocFlag == CDDA_DECK_REQ_SENT)
    {
        if(m_pPacket->RequestTOCStatus() > 0)
        {
            /* If we will do not receive the response on retry, we do not know what to do. */
        }
        else
        {
            /* If we will fail the request for retry, we do not know what to do. */
            m_eAppTocFlag = CDDA_DECK_REQ_NONE;
        }
    }

    if(m_eTocFlag == CDDA_DECK_REQ_SENT)
    {
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
        if(HandleErrorDisc(false) == true)
        {
            return;
        }
#endif
        // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
        m_pPacket->RequestTOCStatus();
        /* We will retry 3 times if we will do not receive the response on retry. */
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
        StartRetryTocInfoTimer(200);
#endif
        // } modified by cychoi 2014.04.25
    }
    // } modified by cychoi 2013.09.06
}

void CDDAManager::HandleRetryToc2Info()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request toc2 info status after disc detection is completed. */
        return;
    }

    MC_HIGH << "CDDAManager::HandleRetryToc2Info()" << LOG_ENDL;

    m_tRetryToc2InfoTimer.stop();

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppToc2Flag == CDDA_DECK_REQ_SENT)
    {
        if(m_pPacket->RequestTOCStatus2(m_nUserBlockNo) > 0)
        {
            /* If we will do not receive the response on retry, we do not know what to do. */
        }
        else
        {
            /* If we will fail the request for retry, we do not know what to do. */
            m_eAppToc2Flag = CDDA_DECK_REQ_NONE;
        }
    }

    if(m_eToc2Flag == CDDA_DECK_REQ_SENT)
    {
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
        if(HandleErrorDisc(false) == true)
        {
            return;
        }
#endif
        // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
        m_pPacket->RequestTOCStatus2(m_nCurrBlockNo);
        /* We will retry 3 times if we will do not receive the response on retry. */
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
        StartRetryToc2InfoTimer(200);
#endif
        // } modified by cychoi 2014.04.25
    }
    // } modified by cychoi 2013.09.06
}

void CDDAManager::HandleRetryCDTextInfo()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request toc info status after disc detection is completed. */
        return;
    }

    MC_HIGH << "CDDAManager::HandleRetryCDTextInfo()" << LOG_ENDL;

    m_tRetryCDTextInfoTimer.stop();

    // { added by cychoi 2014.04.25 for Retry on Request Function Fail
    if(m_eCDTextInfoFlag == CDDA_DECK_REQ_SENT)
    {
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
        if(HandleErrorDisc(false) == true)
        {
            return;
        }
#endif
        m_pPacket->RequestCDTextInfo();
        /* We will retry 3 times if we will do not receive the response on retry. */
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
        StartRetryCDTextInfoTimer(200);
#endif
    }
    // } added by cychoi 2014.04.25

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppCDTextInfoFlag == CDDA_DECK_REQ_SENT)
    {
        if(m_pPacket->RequestCDTextInfo() > 0)
        {
            /* If we will do not receive the response on retry, we do not know what to do. */
        }
        else
        {
            /* If we will fail the request for retry, we do not know what to do. */
            m_eAppCDTextInfoFlag = CDDA_DECK_REQ_NONE;
        }
    }
    // } modified by cychoi 2013.09.06
}

void CDDAManager::HandleRetryPlayback()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request playback after disc detection is completed. */
        return;
    }
    
    MC_HIGH << "CDDAManager::HandleRetryPlayback(): m_nRetryPlaybackMediaState=" << m_nRetryPlaybackMediaState << LOG_ENDL;

    m_tRetryPlaybackTimer.stop();
        
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
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
        m_nRetryTestPlayback ? DiscTestPlay() : Play();
        break;
    case PLAYER_SEARCH:
        m_nRetryTestPlayback ? DiscTestPlay() : Play();
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
        m_nDelayedScanMode = GetScanMode(m_nRetryPlaybackScanMode);
        m_bDelayedScanModeSet = m_nDelayedScanMode != SCANOFF;
        if(m_bDelayedScanModeSet == false)
        {
            m_nDelayedRandomMode = GetRandomMode(m_nRetryPlaybackRandomMode);
            m_nDelayedRepeatMode = GetRepeatMode(m_nRetryPlaybackRepeatMode);
            m_bDelayedModeSet = (m_nDelayedRandomMode != RANDOMOFF) || (m_nDelayedRepeatMode != REPEATOFF);
        }
        else
        {
            m_nRetryPlaybackRepeatMode = eDM_CD_REPEAT_OFF;
            m_nRetryPlaybackRandomMode = eDM_CD_RANDOM_OFF;
        }
        // } modified by cychoi 2014.02.03

#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
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

CDDAManager::CDDAManager(QObject *parent) : QObject(parent),
m_pPacket(0),
m_nSystemState(0),
m_nFFREWRate(-1),
m_nFFREWState(PLAYER_PLAY), // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error // added by cychoi 2013-06-07 for ITS 172685
m_bFFREWStateSigSend(false), // added by sjhyun 2013.09.14 for ITS 189282
//m_bFirstBoot(false), // commented by cychoi 20130.07.11 for keeping playback options
m_bControllerChanged(false),
// { modified by cychoi 2013.06.22 for SW reset on disc loading
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
m_nRetryTestPlayback(false), // added by cychoi 2014.01.09 for StaticTest 18462
m_nRetryPlaybackMediaState(eMP_STATE_NONE),
m_nRetryPlaybackRandomMode(eDM_CD_RANDOM_OFF),
m_nRetryPlaybackRepeatMode(eDM_CD_REPEAT_OFF),
m_nRetryPlaybackScanMode(eDM_CD_SCAN_OFF),
#endif
// } modified by cychoi 2013.06.22
m_bAutoTrackChanged(true), // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
m_bUserTrackChanged(false), // added by cychoi 20103.07.22 for OnUserTrackChanged signal
m_bBTCallStart(false) // added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
{
    Init();
    m_pPacket = Packet::GetInstace();

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    connect(&m_tRetryGNTocInfoTimer, SIGNAL(timeout()), this, SLOT(HandleRetryGNTocInfo())); 
    connect(&m_tRetryTocInfoTimer, SIGNAL(timeout()), this, SLOT(HandleRetryTocInfo())); 
    connect(&m_tRetryToc2InfoTimer, SIGNAL(timeout()), this, SLOT(HandleRetryToc2Info())); 
    connect(&m_tRetryCDTextInfoTimer, SIGNAL(timeout()), this, SLOT(HandleRetryCDTextInfo())); 
    connect(&m_tRetryPlaybackTimer, SIGNAL(timeout()), this, SLOT(HandleRetryPlayback())); 
#endif
// } added by cychoi 2013-06-05

    // { added by cychoi 2013-07-15 for Prevent fix
    m_nTotalPlayTimeFrm = 0;
    m_nTotalPlayTimeMin = 0;
    m_nTotalPlayTimeSec = 0;
    m_bREWMode          = false;
    m_pGNTocData[0]     = 0;
    // } added by cychoi 2013-07-15
}

void CDDAManager::RandomNotice(int nMode)
{
    switch(nMode)
    {
    case RANDOMFILE:
        m_pPacket->RandomOn(eDM_RANDOM_TRACK);
        break;
    case RANDOMFOLDER:
        MC_LOG << "CDDAManager::RandomNotice() RANDOMFOLDER is invalid option" << LOG_ENDL;
        break;
    default:
        m_pPacket->RandomOff();
        break;
    }
}

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
#if _DECK_DIAGNOSTIC_
bool CDDAManager::ExtractResetStart(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_RESET_START) return false;

    MC_HIGH << "CDDAManager::ExtractResetStart: m_nRetryPlaybackMediaState=" << m_nRetryPlaybackMediaState << LOG_ENDL;

// { modified by cychoi 2014.04.14 for HW Event PQ launch DVD mode unconditionally after Deck SW reset
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
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

    // { modified by cychoi 2013.11.25 for ITS 210845
    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eTocFlag == CDDA_DECK_REQ_SENT)
    {
        StartRetryTocInfoTimer(400); // delay 400ms
    }

    if(m_eToc2Flag == CDDA_DECK_REQ_SENT)
    {
        StartRetryToc2InfoTimer(400); // delay 400ms
    }

    if(m_eCDTextInfoFlag == CDDA_DECK_REQ_SENT)
    {
        StartRetryCDTextInfoTimer(400); // delay 400ms
    }

    if(m_eAppGNTocFlag == CDDA_DECK_REQ_SENT)
    {
        StartRetryGNTocInfoTimer(600); // delay 100ms + Playback retry delay 500ms
    }

    if(m_eAppTocFlag == CDDA_DECK_REQ_SENT)
    {
        StartRetryTocInfoTimer(600); // delay 100ms + Playback retry delay 500ms
    }

    if(m_eAppToc2Flag == CDDA_DECK_REQ_SENT)
    {
        StartRetryToc2InfoTimer(600); // delay 100ms + Playback retry delay 500ms
    }

    if(m_eAppCDTextInfoFlag == CDDA_DECK_REQ_SENT)
    {
        StartRetryCDTextInfoTimer(600); // delay 100ms + Playback retry delay 500ms
    }
    // } modified by cychoi 2013.09.06
    // } modified by cychoi 2013.11.25

    return true;
}
#endif
#endif // CDDA_DECK_REQ_RETRY_ENABLED
// } added by cychoi 2013-06-05

bool CDDAManager::ExtractCDDAStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_CD_STATUS) return false;

    // MC_LOG << "CDDAManager::ExtractCDDAStatus" << LOG_ENDL;

    if(m_bLoadComplete == false) return false;

    CDStatus *pStatus = &pPacket->m_tData.m_tCD;

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-05
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscPlayRetryCount = 0;
#endif

    // { modified by cychoi 2013.12.26 for ITS 217306 DVD black
    // { added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
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
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackMediaState = m_ePlayerState;
#endif
        return true;
    }
    // } added by cychoi 2013.10.06
    // } modified by cychoi 2013.12.26

    ////////////////////////////////////////////////////////
    // 트랙 변경 알림. 
    if(pStatus->nTrackNo != 0xFF) {
        int nTrackNo = BCD2DEC2(pStatus->nTrackNo);
        // { added by cychoi 20103.10.24 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
        if(DeckManager::GetInstance()->GetCDDAMediaMode() == eDM_MODE_CDROM)
        {
            if(nTrackNo == 0 || nTrackNo < m_nStartTrackNo || nTrackNo > m_nLastTrackNo)
            {
                MC_HIGH << "CDDAManager::ExtractCDDAStatus() -> Invalid CD" << LOG_ENDL;
                Stop();
                return false;
            }
        }
        // } added by cychoi 20103.10.24
        // { modified by cychoi 2013.11.14 for ITS 209100 Skip Track Changed on Search Mode
        DECK_CD_PLAY_COND ePlayStatus = (DECK_CD_PLAY_COND)pStatus->nPlayStatus;
        if(ePlayStatus == eDM_CD_PLAY_COND_SEARCHING_IN ||
           ePlayStatus == eDM_CD_PLAY_COND_SEARCHING_OUT)
        {
            if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFILE ||
               GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF)
            {
                // Track is not valid on Search Mode, please keep current Track.
                nTrackNo = m_nPlayTrackNo;
            }
        }
        // } modified by cychoi 2013.11.14
        if(m_tStatus.m_nTrackNo != nTrackNo || m_nPlayTrackNo != nTrackNo || m_bControllerChanged == true) {
            bool bTrackChanged = (bool)(m_tStatus.m_nTrackNo != nTrackNo || m_nPlayTrackNo != nTrackNo); // added by cychoi 2014.04.29 for ITS 233500
            // { commented by cychoi 2013.07.13 for MLT spec out
            // { added by cychoi 20103.06.11 for ITS 173068
#if 0
            if(IsMLTListMode() == true && m_nFFREWRate != -1)
            {
                m_nFFREWRate   = -1;
                // Please find prev/next MLT in the MLT list.
                int nNewTrackIdx;
                if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMOFF)
                {
                    if(m_bREWMode == true)
                    {
                        // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
                        nNewTrackIdx = GetPrevTrackIdx(1, GetCurrentIndex());
                        // } modified by cychoi 2013-07-10
                    }
                    else
                    {
                        // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
                        nNewTrackIdx = GetNextTrackIdx(1, GetCurrentIndex());
                        // } modified by cychoi 2013-07-10
                    }
                }
                else
                {
                    nNewTrackIdx = GetRandomTrackIdx();
                }
                int nNewTrackNum = m_nStartTrackNo + m_tTrackList.at(nNewTrackIdx).m_nTrackIndex;

                if(nNewTrackNum != nTrackNo)
                {
                    BCD4              tBCD4;
                    BCD2              tBCD2;
                    DirectContCommand tCmd;
                    
                    tBCD4.word          = DEC2BCD4(nNewTrackNum);
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
                    
                    MC_LOG << "CDDAManager::ExtractCDDAStatus()->DirectCont() 1: nNewTrackNum=" << nNewTrackNum << LOG_ENDL;
                    SetCurrentMediaPosition(0);
                    m_pPacket->DirectCont(&tCmd);
                }
                else
                {
                    SetCurrentMediaPosition(0);
                    Play();
                }
                emit OnNormalPlay(); // added by Michael.Kim 2013.05.23 for additional signal for FF/Rew functionality.
                m_nUserRepeatMode = -1; // added by Michael.Kim 2013.05.23 repeat off on normal play
                RepeatPlay(REPEATALL); // added by Michael.Kim 2013.05.23 repeat off on normal play
            }
#endif
            // } added by cychoi 20103.06.11
            // } commented by cychoi 2013.07.13
            // removed by cychoi 20103.06.11 for ITS 173068
            // { modified by cychoi 2013.07.13 for MLT spec out
            if(m_nUserRandomMode >= 0 && m_tStatus.m_nPlayStatus != eDM_CD_PLAY_COND_PLAYING_FAST) {
            //else if(m_nUserRandomMode >= 0 && m_tStatus.m_nPlayStatus != eDM_CD_PLAY_COND_PLAYING_FAST) {
            // } modified by cychoi 2013.07.13
                // { modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050
                // { modified by shkim for ITS 206865 CDDA Random Mode Issue 2013.11.07
                if(bTrackChanged == true &&
                   (m_ePlayDirection == CDDA_PLAY_NEXT || m_ePlayDirection == CDDA_PLAY_PREV))
                {
                    MC_LOG << "CDDAManager:: when track changed execute Random Mode. " << LOG_ENDL;
                    RandomNotice(m_nUserRandomMode);
                    m_nUserRandomMode = -2;
                }
                // } modified by shkim for ITS 206865 CDDA Random Mode Issue 2013.11.07
                else
                {
                    EmitTrackChange(nTrackNo, bTrackChanged); // HMC Issue. No TP Nessage & Track Info on Cluster after changed mode from USB to Disc
                }
                if(m_nFFREWRate == -1)
                {
                    m_ePlayDirection = CDDA_PLAY_NEXT;
                }
                // } modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050
            }
            // { modified by cychoi 20103.06.11 for ITS 173068
            else {
            //else if(m_bSearchingMode == false) {
            // } modified by cychoi 20103.06.11
                // { commented by cychoi 2013.07.13 for MLT spec out
                // { added by cychoi 20103.06.11 for ITS 173068
#if 0
                if(IsMLTListMode() == true)
                {
                    // Please find next MLT in the MLT list.
                    int nNewTrackIdx = -1;
                    if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMOFF)
                    {
                        // { modified by cychoi 2013.07.09 for ITS 176771 & 173068 MLT play error (Smoke Test failuer)
                        int nPlayTrackIndex = GetMLTTrackIndex(m_nPlayTrackNo);
                        if(m_ePlayDirection == CDDA_PLAY_NEXT)
                        {
                            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
                            nNewTrackIdx = GetNextTrackIdx(1, GetCurrentIndex());
                            // } modified by cychoi 2013-07-10
                        }
                        else if(m_ePlayDirection == CDDA_PLAY_PREV)
                        {
                            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
                            nNewTrackIdx = GetPrevTrackIdx(1, GetCurrentIndex());
                            // } modified by cychoi 2013-07-10
                        }
                        else if(m_ePlayDirection == CDDA_PLAY_CURR)
                        {
                            nNewTrackIdx = GetMLTTrackIndex(nTrackNo);
                        }
                        m_ePlayDirection = CDDA_PLAY_NEXT;
                        MC_LOG << "CDDAManager::ExtractCDDAStatus() Play TrackIndex=" << nPlayTrackIndex << ", New TrackIndex=" << nNewTrackIdx << LOG_ENDL;
                        // } modified by cychoi 20103.07.09
                    }
                    else
                    {
                        nNewTrackIdx = GetRandomTrackIdx();
                    }
                    int nNewTrackNum = m_nStartTrackNo + m_tTrackList.at(nNewTrackIdx).m_nTrackIndex;

                    if(nNewTrackNum != nTrackNo)
                    {
                        BCD4              tBCD4;
                        BCD2              tBCD2;
                        DirectContCommand tCmd;
                        
                        tBCD4.word          = DEC2BCD4(nNewTrackNum);
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
                        
                        MC_LOG << "CDDAManager::ExtractCDDAStatus()->DirectCont() 2: nNewTrackNum=" << nNewTrackNum << LOG_ENDL;
                        m_pPacket->DirectCont(&tCmd);

// { added by cychoi 2013-07-13 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
                        StartRetryPlaybackTimer(7500); // CD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-07-13

                        return true;
                    }
                }
#endif
                // } added by cychoi 20103.06.11
                // } commented by cychoi 2013.07.13
                EmitTrackChange(nTrackNo, bTrackChanged); // modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050
            }
        }
        else {
            if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFILE && m_tStatus.m_nPlaybackTime == 0)
            {
                MC_LOG << "CDDAManager::nTrackNo 2: " << pStatus->nTrackNo << LOG_ENDL;
                // { commented by cychoi 2013.07.13 for MLT spec out
                //if(IsMLTListMode() == true) {
                //    emit OnTrackChanged(GetMLTTrackIndex(m_nPlayTrackNo));
                //}else
                // } commented by cychoi 2013.07.13
                {
                    // { added by cychoi 20103.07.22 for add new signal
                    if(m_bUserTrackChanged == true)
                    {
                        emit OnUserTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
                        m_bUserTrackChanged = false;
                    }
                    // } added by cychoi 20103.07.22
                    // { added by cychoi 2014.04.06 for ITS 233500
                    else
                    {
                        emit OnAutoTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
                    }
                    // } added by cychoi 2014.04.06
                    emit OnTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
                }
                emit OnDurationChanged(GetCurrentMediaDuration());
                // removed by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
            }
        }
    }

    // 상태 변경 알림.
    int                 bStartTime  = false;
    DECK_CD_PLAY_COND   ePlayStatus = (DECK_CD_PLAY_COND)pStatus->nPlayStatus;
    if(ePlayStatus != m_tStatus.m_nPlayStatus) {
        MEDIA_PLAYER_STATE ePlayerState = (MEDIA_PLAYER_STATE)DeckManager::GetInstance()->PlayCondToPlayerState(ePlayStatus);

        if(ePlayerState != m_ePlayerState) 
        {
            if(m_ePlayerState != eMP_STATE_PLAYING && ePlayerState == eMP_STATE_PLAYING) bStartTime = true;
            //if(m_bSearchingMode == false) { // commented by cychoi 20103.06.11 for ITS 173068
                m_ePlayerState = ePlayerState;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
                m_nRetryPlaybackMediaState = m_ePlayerState;
#endif
// } modified by cychoi 2013.06.20
                if(m_tUserReqTrackList.count() > 0 && ePlayerState == eMP_STATE_PLAYING) {
                    int nNewTrack = m_tUserReqTrackList.takeLast();
                    m_tUserReqTrackList.clear();
                    // { modified by cychoi 20103.08.23 for ITS 185414 ID3 Tag Display
                    if(m_nPlayTrackNo == (m_nStartTrackNo + nNewTrack))
                    {
                        emit OnStateChanged(GetCurrentMediaState());
                        // { modified by cychoi 2013.10.21 for ITS 197534 Delay FF on Search Mode by HK repeatition press
                        if(m_nFFREWRate != -1)
                        {
                            DECK_FAST_SPEED eSpeed = (DECK_FAST_SPEED)m_nFFREWRate;
                            m_bREWMode == false ? m_pPacket->FastUp(eSpeed) : m_pPacket->FastDown(eSpeed);
                        }
                        else
                        {
                            // { added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
                            if(ePlayerState == eMP_STATE_PLAYING && // added by cychoi 20103.07.05 for Seek Up/Down & FF/REW mute
                               m_bAutoTrackChanged == false)
                            {
                                MC_HIGH << "CDDAManager::ExtractCDDAStatus(): emit OnRealSoundChanged() [1]" << LOG_ENDL;
                                emit OnRealSoundChanged();
                                m_bAutoTrackChanged = true;
                            }
                            // } added by cychoi 20103.06.13
                        }
                        // } modified by cychoi 20103.10.21
                    }
                    else
                    {
                        MC_LOG << "CDDAManager::ExtractCDDAStatus(): nCurrTrack: " << GetCurrentIndex() << ", nNewTrack: " << nNewTrack << LOG_ENDL;
                        SetCurrentIndex(nNewTrack);
                        m_nNextTrackNo = nNewTrack + m_nStartTrackNo; // modified by cychoi 20103.08.14 for ITS 184423
                        Play();
                        // { modified by cychoi 2013.10.21 for ITS 197534 Delay FF on Search Mode by HK repeatition press
                        if(m_nFFREWRate != -1)
                        {
                            DECK_FAST_SPEED eSpeed = (DECK_FAST_SPEED)m_nFFREWRate;
                            m_bREWMode == false ? m_pPacket->FastUp(eSpeed) : m_pPacket->FastDown(eSpeed);
                        }
                        else
                        {
                            // { added by cychoi 20103.07.25 for ITS 181748
                            m_bAutoTrackChanged = false;
                            m_bUserTrackChanged = true;
                            // } added by cychoi 20103.07.25
                        }
                        // } modified by cychoi 20103.10.21
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
                        MC_HIGH << "CDDAManager::ExtractCDDAStatus(): emit OnRealSoundChanged() [2]" << LOG_ENDL;
                        emit OnRealSoundChanged();
                        m_bAutoTrackChanged = true;
                    }
                    // } added by cychoi 20103.06.13
                }
            //} // commented by cychoi 20103.06.11 for ITS 173068

            m_bFFREWStateSigSend = (m_ePlayerState == eMP_STATE_FAST_PLAYING); // added by cychoi 2013.10.27 for ITS 198856 
            // { modified by cychoi 2013.11.15 for ITS 205008
            if(m_ePlayerState != eMP_STATE_STOPPED)
            {
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
                // { modified by cychoi 2013.11.18 for ITS 209699
                if(m_ePlayerState == eMP_STATE_PLAYING ||
                   m_ePlayerState == eMP_STATE_FAST_PLAYING) 
                {
                    if(m_bDelayedScanModeSet == true)
                    {
                        Scan(m_nDelayedScanMode);
                        m_nDelayedScanMode = -1;
                        m_bDelayedScanModeSet = false;
                    }
                }
                // } modified by cychoi 2013.11.18
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
                emit OnNormalPlay();
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
            if(ePlayerState == eMP_STATE_PLAYING && m_bAutoTrackChanged == false)
            {
                MC_HIGH << "CDDAManager::ExtractCDDAStatus(): emit OnRealSoundChanged() [3]" << LOG_ENDL;
                emit OnRealSoundChanged();
                m_bAutoTrackChanged = true;
            }
        }
        // } modified by wonseok.heo for ITS 191745 2013.09.27
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

    // 기타
    m_tStatus.m_bCDRomTrack     = pStatus->nCDROM;
    m_tStatus.m_bCDTextExist    = pStatus->nCDText;
    m_tStatus.m_bDTSCD          = pStatus->nDTS;
    m_tStatus.m_nDiscNo         = BCD2DEC2(pStatus->nDiscNo);
    m_tStatus.m_nInterval       = pStatus->nInterval;

    m_tStatus.m_bLFE            = pStatus->nLFE;
    m_tStatus.m_nChannel        = (DECK_CD_CHANNEL_MODE)pStatus->nChannel;
    m_tStatus.m_nAudioOutput    = (DECK_CD_AUDIO_OUTPUT_MODE)pStatus->nAudioOutputMode;
    m_tStatus.m_bSCMSDetect     = pStatus->nSCMSDetection;
    m_tStatus.m_bJumpDetect     = pStatus->nJumpinessDetection;
    m_tStatus.m_bPlaySpeed      = pStatus->nPlayWidthSpeed;
    m_tStatus.m_bEmphasisExist  = pStatus->nEmphasis;
    m_tStatus.m_nDirectStopMode = (DECK_DIRECT_STOP_MODE)pStatus->nDirectStopMode;
 
    ////////////////////////////////////////////////////////
    // 재생 시간 변경 알림.
    if(pStatus->nPlayStatus != eDM_CD_PLAY_COND_SEARCHING_OUT && m_nUserTimeSearch < 0) {
        if(pStatus->nPlaybackTimeMin10 != 0xF && pStatus->nPlaybackTimeMin1 != 0xF) 
        {
            int nMin  = BCD2DEC1(pStatus->nPlaybackTimeMin10) * 10 + BCD2DEC1(pStatus->nPlaybackTimeMin1);
            int nSec  = BCD2DEC1(pStatus->nPlaybackTimeSec10) * 10 + BCD2DEC1(pStatus->nPlaybackTimeSec1);
            int nTime = (nMin * 60 + nSec) * 1000;
            
            if(m_tStatus.m_nPlaybackTime != nTime) {
                if(bStartTime == true) DeckManager::GetInstance()->SetStartPlayTime(nTime);
                DeckManager::GetInstance()->SetLifeTime(nTime);
                m_tStatus.m_nPlaybackTime = nTime;        
                //MC_LOG << "CDDAManager::P: " << m_tStatus.m_nPlayStatus << ", Time: " << nTime << LOG_ENDL;
                emit OnPositionChanged(nTime);
            }
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
    //MC_LOG << "CDDAManager::m_tStatus.m_nRandomMode: " << m_tStatus.m_nRandomMode << "-" << pStatus->nRandomMode << LOG_ENDL;
    //MC_LOG << "CDDAManager::m_tStatus.m_nRepeatMode: " << m_tStatus.m_nRepeatMode << "-" << pStatus->nRepeatMode << LOG_ENDL;
    //MC_LOG << "CDDAManager::m_tStatus.m_nScanMode :  " << m_tStatus.m_nScanMode << "-" << pStatus->nScanMode << LOG_ENDL;
    DECK_CD_RANDOM_MODE eRandomMode = (DECK_CD_RANDOM_MODE)pStatus->nRandomMode;
    DECK_CD_REPEAT_MODE eRepeatMode = (DECK_CD_REPEAT_MODE)pStatus->nRepeatMode;
    DECK_CD_SCAN_MODE   eScanMode   = (DECK_CD_SCAN_MODE)pStatus->nScanMode;
    //{ modified by cychoi 20130.07.11 for keeping playback options
    if(m_tStatus.m_nRandomMode != eRandomMode && m_nUserRandomMode == -1) {
        m_tStatus.m_nRandomMode = eRandomMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackRandomMode = m_tStatus.m_nRandomMode;
#endif
// } modified by cychoi 2013.06.20
        emit OnModeChanged(GetRandomMode(m_tStatus.m_nRandomMode));
    }  
    if(m_tStatus.m_nRepeatMode != eRepeatMode) {
        m_tStatus.m_nRepeatMode = eRepeatMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackRepeatMode = m_tStatus.m_nRepeatMode;
#endif
// } modified by cychoi 2013.06.20
        emit OnModeChanged(GetRepeatMode(m_tStatus.m_nRepeatMode));
    }
    if(m_tStatus.m_nScanMode != eScanMode) {
        if((m_tStatus.m_nScanMode != eDM_CD_SCAN_OFF && m_tStatus.m_nScanMode != eDM_CD_SCAN_NONE) && eScanMode == eDM_CD_SCAN_OFF) {
            emit OnScanCompleted();
        }
        m_tStatus.m_nScanMode = eScanMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackScanMode = m_tStatus.m_nScanMode;
#endif
// } modified by cychoi 2013.06.20
        emit OnModeChanged(GetScanMode(m_tStatus.m_nScanMode));
    }

    if(m_nUserRandomMode == -2) m_nUserRandomMode = -1;
    //} modified by cychoi 20130.07.11

// { added by cychoi 2013-07-13 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    switch(m_ePlayerState)
    {
    case eMP_STATE_PLAYING:
    case eMP_STATE_SEARCHING:
    case eMP_STATE_FAST_PLAYING: 
        StartRetryPlaybackTimer(7500); // CD Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
        break;
    case eMP_STATE_STOPPED:
    case eMP_STATE_PAUSED:
    default:
        break;
    }
#endif
// } added by cychoi 2013-07-13

    //if(m_bAutoTrackChanged == false) m_bAutoTrackChanged = true; // comented by cychoi 20103.07.22 for add new signal

    return true;
}

bool CDDAManager::ExtractTOCStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_TOC_STATUS) return false;

    TOCStatus *pStatus = &pPacket->m_tData.m_tTOC;

    if(m_bLoadComplete == true &&
       (m_eAppGNTocFlag == CDDA_DECK_REQ_SENT || m_eAppTocFlag == CDDA_DECK_REQ_SENT)) // modified by cychoi 2013.09.06 for ITS 178797
    {
        if(m_eAppGNTocFlag == CDDA_DECK_REQ_SENT) { // modified by cychoi 2013.09.06 for ITS 178797
// { modified by cychoi 2013.09.06 for ITS 178797
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
            InitRetryGNTocInfo();
#endif
// } added by cychoi 2013-06-05
// } modified by cychoi 2013.09.06
            m_nGNTocSize = 0;
            for(int nIdx = 0; nIdx < m_tTrackList.count(); nIdx++) {
                m_pGNTocData[m_nGNTocSize] = m_tTrackList.at(nIdx).m_nAddress;
                m_nGNTocSize++;
                if(m_nGNTocSize >= 199) break;
            }
            m_pGNTocData[m_nGNTocSize] = AddressToSectors(m_nTotalPlayTimeMin, m_nTotalPlayTimeSec, m_nTotalPlayTimeFrm); 
            m_nGNTocSize++;
            emit OnGraceNoteTOCReceived(m_nGNTocSize, (void*)m_pGNTocData);
            
            m_eAppGNTocFlag = CDDA_DECK_REQ_DONE; // modified by cychoi 2013.09.06 for ITS 178797
        }
        else {
// { modified by cychoi 2013.09.06 for ITS 178797
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
            InitRetryTocInfo();
#endif
// } added by cychoi 2013-06-05
// } modified by cychoi 2013.09.06
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
            m_nDiscDetectRetryCount = 0;
#endif
            // FOR APP
            m_tToc.valid            = pStatus->nValid;
            m_tToc.start_track      = m_nStartTrackNo;
            m_tToc.last_track       = m_nLastTrackNo;
            m_tToc.total_play_frame = m_nTotalPlayTimeFrm;
            m_tToc.total_play_min   = m_nTotalPlayTimeMin;
            m_tToc.total_play_sec   = m_nTotalPlayTimeSec;
            m_eAppTocFlag           = m_tToc.valid ? CDDA_DECK_REQ_DONE : CDDA_DECK_REQ_NONE; // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error // modified by cychoi 2013.09.06 for ITS 178797

            emit OnTOCReceived(&m_tToc);
        }
    }
    else if(m_eTocFlag == CDDA_DECK_REQ_SENT)
    {
// { modified by cychoi 2013.09.06 for ITS 178797
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
        InitRetryTocInfo();
#endif
// } added by cychoi 2013-06-05
// } modified by cychoi 2013.09.06
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
        m_nDiscDetectRetryCount = 0;
#endif

        if(pStatus->nValid == 0) {
            return false;
        }

        m_nStartTrackNo     = BCD2DEC1(pStatus->nStartTrackNo10) * 10 + BCD2DEC1(pStatus->nStartTrackNo1);  // 1
        m_nLastTrackNo      = BCD2DEC1(pStatus->nLastTrackNo10)  * 10 + BCD2DEC1(pStatus->nLastTrackNo1);   // max track number
        m_nTotalTrackNum    = m_nLastTrackNo - m_nStartTrackNo + 1;

        m_nTotalPlayTimeFrm = BCD2DEC1(pStatus->nTotalPlayTimeFrm10) * 10 + BCD2DEC1(pStatus->nTotalPlayTimeFrm1);
        m_nTotalPlayTimeMin = BCD2DEC1(pStatus->nTotalPlayTimeMin10) * 10 + BCD2DEC1(pStatus->nTotalPlayTimeMin1);
        m_nTotalPlayTimeSec = BCD2DEC1(pStatus->nTotalPlayTimeSec10) * 10 + BCD2DEC1(pStatus->nTotalPlayTimeSec1);

        m_nTotalBlockNum = (m_nTotalTrackNum + 9) / 10;
        m_nCurrBlockNo   = 0;

        // FOR APP
        m_tToc.valid = 1;
        m_tToc.start_track      = m_nStartTrackNo;
        m_tToc.last_track       = m_nLastTrackNo;
        m_tToc.total_play_frame = m_nTotalPlayTimeFrm;
        m_tToc.total_play_min   = m_nTotalPlayTimeMin;
        m_tToc.total_play_sec   = m_nTotalPlayTimeSec;
        m_eTocFlag              = CDDA_DECK_REQ_DONE; // modified by cychoi 2013.09.06 for ITS 178797

        m_nPlayTrackNo          = m_nStartTrackNo; // 임시 초기화.

        MC_LOG << "CDDAManager::m_nStartTrackNo:" << m_nStartTrackNo << ", m_nLastTrackNo:" << m_nLastTrackNo << ", m_nTotalBlockNum:" << m_nTotalBlockNum << LOG_ENDL;

        // { added by cychoi 2013.09.06 for ITS 178797
        // TOC Status 2 요청.
        if(m_bLoadComplete == false) {
            MC_LOG << "CDDAManager::m_pPacket->RequestTOCStatus2(" << m_nCurrBlockNo << ")" << LOG_ENDL;
            // { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
            InitRetryToc2Info();
#endif
            // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
            m_pPacket->RequestTOCStatus2(m_nCurrBlockNo);
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
            m_nDiscDetectRetryCount = 0;
#endif
            m_eToc2Flag = CDDA_DECK_REQ_SENT;
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
            StartRetryToc2InfoTimer(200);
#endif
            // } modified by cychoi 2014.04.25
        }
        // } added by cychoi 2013-06-05
        // } added by cychoi 2013.09.06
}
    
    return true;
}

int CDDAManager::ExtractTOC2Status(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_TOC_STATUS2) return 0;

    TOCStatus2 *pStatus = &pPacket->m_tData.m_tTOC2;

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    InitRetryToc2Info();
#endif
// } added by cychoi 2013-06-05
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
#endif

    if(m_bLoadComplete == true && m_eAppToc2Flag == CDDA_DECK_REQ_SENT &&
       m_nUserBlockNo == pStatus->nBlockNo) // modified by cychoi 2013.09.06 for ITS 178797
    {
        MC_LOG << "CDDAManager::ExtractTOC2Status 1" << LOG_ENDL;
        
        m_tToc2.valid    = 1;
        m_tToc2.block_no = pStatus->nBlockNo;

        int nIdx = 0;
        for(int nTrack = 0; nTrack < 10; nTrack++) { 
            m_tToc2.play_start[nTrack][0] = BCD2DEC2(pStatus->pPlayStartAddr[nIdx++]);
            m_tToc2.play_start[nTrack][1] = BCD2DEC2(pStatus->pPlayStartAddr[nIdx++]);
            m_tToc2.play_start[nTrack][2] = BCD2DEC2(pStatus->pPlayStartAddr[nIdx++]);
        }

        m_eAppToc2Flag = CDDA_DECK_REQ_DONE; // modified by cychoi 2013.09.06 for ITS 178797
        
        emit OnTOC2Received(&m_tToc2);

        return 1;
    }
    else if(m_eToc2Flag == CDDA_DECK_REQ_SENT)
    {
        MC_LOG << "CDDAManager::ExtractTOC2Status 2: " << pStatus->nValid << LOG_ENDL;
        
        if(pStatus->nValid == 0) {
            return 0;
        }   
        
        int nBlockNo  = pStatus->nBlockNo;
        int nTrackNum = m_nTotalTrackNum - nBlockNo * 10;
        if(nTrackNum > 10) nTrackNum = 10;

        int nIdx = 0;
        for(int nTrack = 0; nTrack < nTrackNum; nTrack++) 
        {
            CDDATrackInfo tTrack;

            tTrack.m_nTrackIndex = nBlockNo * 10 + nTrack;
            // { modified by cychoi 2013-07-18 for ITS 180455 Track numbering problem
            tTrack.m_strTitle    = QString("Track %1").arg(1 + tTrack.m_nTrackIndex);
            //tTrack.m_strTitle    = QString("Track %1").arg(m_nStartTrackNo + tTrack.m_nTrackIndex);
            // { modified by cychoi 2013-07-18
            tTrack.m_nMinAddr    = BCD2DEC2(pStatus->pPlayStartAddr[nIdx++]);
            tTrack.m_nSecAddr    = BCD2DEC2(pStatus->pPlayStartAddr[nIdx++]);
            tTrack.m_nFrmAddr    = BCD2DEC2(pStatus->pPlayStartAddr[nIdx++]);
            tTrack.m_nAddress    = AddressToSectors(tTrack.m_nMinAddr, tTrack.m_nSecAddr, tTrack.m_nFrmAddr); 

            m_tTrackList.append(tTrack);
        }

        m_nCurrBlockNo++;
        if(m_nCurrBlockNo < m_nTotalBlockNum) {
            // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
            m_pPacket->RequestTOCStatus2(m_nCurrBlockNo);
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
            m_nDiscDetectRetryCount = 0;
#endif
            m_eToc2Flag = CDDA_DECK_REQ_SENT; // added by cychoi 2013.09.06 for ITS 178797
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
            StartRetryToc2InfoTimer(200);
#endif
// } added by cychoi 2013-06-05
            // } modified by cychoi 2014.04.25
            return 1;
        }
        else {
            int             nPlayTime;
            CDDATrackInfo   tTrack1;
            CDDATrackInfo   tTrack2;

            nTrackNum = m_tTrackList.count();

            for(nIdx = 0; nIdx < nTrackNum - 1; nIdx++)
            {
                tTrack1 = m_tTrackList.at(nIdx);
                tTrack2 = m_tTrackList.at(nIdx+1);

                tTrack1.m_nLength = AddressToSectors(tTrack2.m_nMinAddr, tTrack2.m_nSecAddr, tTrack2.m_nFrmAddr) - tTrack1.m_nAddress;

                nPlayTime = tTrack1.m_nLength / CD_BLOCKS_PER_SECOND;
                tTrack1.m_nDuration = nPlayTime * 1000;
                tTrack1.m_tPlayTime.setHMS(0, nPlayTime/60, nPlayTime%60);

                m_tTrackList.replace(nIdx, tTrack1);
            }

            if(nTrackNum > 0)
            {
                tTrack1 = m_tTrackList.at(nIdx);
                tTrack1.m_nLength = AddressToSectors(m_nTotalPlayTimeMin, m_nTotalPlayTimeSec, m_nTotalPlayTimeFrm) - tTrack1.m_nAddress;

                nPlayTime = tTrack1.m_nLength / CD_BLOCKS_PER_SECOND;
                tTrack1.m_nDuration = nPlayTime * 1000;
                tTrack1.m_tPlayTime.setHMS(0, nPlayTime/60, nPlayTime%60);

                m_tTrackList.replace(nIdx, tTrack1);
            }

            m_eToc2Flag = CDDA_DECK_REQ_DONE; // added by cychoi 2013.09.06 for ITS 178797
            
            return 2;
        }
    }

    return 1;
}

bool CDDAManager::ExtractCDTextStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_CD_TEXT_STATUS) return false;

    CDTextStatus *pStatus = &pPacket->m_tData.m_tCDText;

    m_tCDText.valid      = pStatus->nValid;
    m_tCDText.track_no   = BCD2DEC2(pStatus->nTrackNo);
    m_tCDText.chr_valid  = pStatus->nCHRValid;
    m_tCDText.conti_flag = pStatus->nContinue;
    m_tCDText.data_type  = pStatus->nAccordingToData;
    m_tCDText.block_no   = pStatus->nBlockNo;
    m_tCDText.frame_no   = pStatus->nFrameNo;
    
    for(int nIdx = 0; nIdx < 16; nIdx++) {
        m_tCDText.chr_code[nIdx] = (char)pStatus->pCHRCode[nIdx];
    }

    m_eAppCDTextFlag = CDDA_DECK_REQ_DONE; // added by cychoi 2013.09.06 for ITS 178797
    emit OnCDTextReceived(&m_tCDText);
    
    return true;
}

bool CDDAManager::ExtractCDTextInfoStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_CD_TEST_INFOR_STATUS) return false;

    CDTextInfoStatus *pStatus = &pPacket->m_tData.m_tCDTextInfo;

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    InitRetryCDTextInfo();
#endif
// } added by cychoi 2013-06-05
#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
#endif
    
    m_tCDTextInfo.valid = pStatus->nValid;

    for(int nIdx = 0; nIdx < 8; nIdx++) {
        m_tCDTextInfo.chr_code[nIdx]  = pStatus->ppCharLangCode[nIdx][0];
        m_tCDTextInfo.lang_code[nIdx] = pStatus->ppCharLangCode[nIdx][1];
    }

    if(m_bLoadComplete == true) {
        // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
        if(m_eAppCDTextInfoFlag == CDDA_DECK_REQ_SENT)
        {
            m_eAppCDTextInfoFlag = CDDA_DECK_REQ_DONE; // added by cychoi 2013.09.06 for ITS 178797
            emit OnCDTextInfoReceived(&m_tCDTextInfo);
        }
        else if(m_eCDTextInfoFlag == CDDA_DECK_REQ_SENT)
        {
            m_eCDTextInfoFlag = CDDA_DECK_REQ_DONE;
        }
        // } modified by cychoi 2014.04.25
    }
    
    return true;
}

bool CDDAManager::ExtractReqError(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_REQ_ERR) return false;

    ReqError *pStatus = &pPacket->m_tData.m_tReqError;

    MC_LOG << "CDDAManager::ExtractReqError: 0x" << pStatus->nErrorDetail << LOG_ENDL;

    // { added by cychoi 20103.08.23 for ITS 184879 & 186162
    if(m_bAutoTrackChanged == false)
    {
        MC_HIGH << "CDDAManager::ExtractReqError(): emit OnRealSoundChanged()" << LOG_ENDL;
        emit OnRealSoundChanged();
        m_bAutoTrackChanged = true;
    }
    // } added by cychoi 20103.08.23

#ifdef CDDA_DECK_REQ_RETRY_ENABLED
    InitRetryGNTocInfo();
    InitRetryTocInfo();
    InitRetryToc2Info();
    InitRetryCDTextInfo();
    InitRetryPlayback();
    InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
#endif

    return true;
}

int CDDAManager::GetRandomMode(DECK_CD_RANDOM_MODE eMode)
{
    switch(eMode)
    {
    case eDM_CD_RANDOM_OFF:         return RANDOMOFF;
    case eDM_CD_RANDOM_TRACK:       return RANDOMFILE;
    default:                        return RANDOMOFF;
    }
}

int CDDAManager::GetRepeatMode(DECK_CD_REPEAT_MODE eMode)
{
    switch(eMode)
    {
    case eDM_CD_REPEAT_OFF:         return REPEATALL;
    case eDM_CD_REPEAT_TRACK:       return REPEATFILE;
    default:                        return REPEATALL;
    }
}

int CDDAManager::GetScanMode(DECK_CD_SCAN_MODE eMode)
{
    switch(eMode)
    {
    case eDM_CD_SCAN_OFF:           return SCANOFF;
    case eDM_CD_SCAN_TRACK:         return SCANALL;
    default:                        return SCANOFF;
    }
}

UINT4 CDDAManager::AddressToSectors(int nMinAddr, int nSecAddr, int nFrameAddr)
{
    UINT4 nSectors = nMinAddr * CD_BLOCKS_PER_SECOND * 60 + nSecAddr * CD_BLOCKS_PER_SECOND + nFrameAddr;

    return nSectors;
}

void CDDAManager::EmitError(int nError)
{
    DeckManager *p = DeckManager::GetInstance();
    if(p) {
        p->EmitError(nError);
    }
}

// { commented by cychoi 2013.07.13 for MLT spec out
#if 0
bool CDDAManager::IsMLTListMode()
{
    if(m_tBackupList.count() > 0) 
        return true;
    else
        return false;
}

int CDDAManager::GetMLTTrackIndex(int nTrackNum)
{
    for(int i = 0; i < m_tTrackList.count(); i++) {
        CDDATrackInfo tInfo = m_tTrackList.at(i);
        if(nTrackNum == (int)tInfo.m_nTrackIndex + m_nStartTrackNo) {
            return i;
        }
    }

    return -1;  // added by cychoi 20103.06.11 for ITS 173068
}
#endif
// } commented by cychoi 2013.07.13

// { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
int CDDAManager::GetNextTrackIdx(int nSkiplevel, int moveTrackIndex)
{
    int nTrackIdx = moveTrackIndex; //GetCurrentIndex();
    
    int nNewTrackIdx = nTrackIdx + nSkiplevel % m_tTrackList.count();
    
    if(nNewTrackIdx >= m_tTrackList.count()) {
        nNewTrackIdx = nNewTrackIdx - m_tTrackList.count();
    }
    
    return nNewTrackIdx;
}

int CDDAManager::GetPrevTrackIdx(int nSkiplevel, int moveTrackIndex)
{
    int nTrackIdx = moveTrackIndex; //GetCurrentIndex();
    
    int nNewTrackIdx = nTrackIdx - nSkiplevel % m_tTrackList.count();
    
    if(nNewTrackIdx < 0) {
        nNewTrackIdx = m_tTrackList.count() + nNewTrackIdx;
    }
    
    return nNewTrackIdx;
}
// } modified by cychoi 2013-07-10

// { added by cychoi 2015-05-19 for QE_Smoke Test Ver_45버전(130519) 37 fail
int CDDAManager::GetRandomTrackIdx()
{
    int nNewTrackIdx;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    nNewTrackIdx = qrand() % m_tTrackList.count();

    return nNewTrackIdx;
}
// } added by cychoi 2015-05-19

// { added by cychoi 20103.10.15 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
bool CDDAManager::CheckDiscValid()
{
    if(DeckManager::GetInstance()->GetCDDAMediaMode() == eDM_MODE_CDROM)
    {
        if(m_nPlayTrackNo == 0 || m_nPlayTrackNo < m_nStartTrackNo || m_nPlayTrackNo > m_nLastTrackNo)
        {
            MC_HIGH << "CDDAManager::CheckDiscValid() -> Invalid CD" << LOG_ENDL;
            return false;
        }
    }

    return true;
}
// } added by cychoi 20103.10.15

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CDDA_DECK_REQ_RETRY_ENABLED
int CDDAManager::InitRetryGNTocInfo()
{
    if(m_tRetryGNTocInfoTimer.isActive() == true)
    {
        m_tRetryGNTocInfoTimer.stop();
    }

    return 1;
}

int CDDAManager::StartRetryGNTocInfoTimer(int ms) // modified by cychoi 2013.09.06 for ITS 178797
{
    if(m_tRetryGNTocInfoTimer.isActive() == true)
    {
        m_tRetryGNTocInfoTimer.stop();
    }
    m_tRetryGNTocInfoTimer.start(ms);

    return 1;
}

int CDDAManager::InitRetryTocInfo()
{
    if(m_tRetryTocInfoTimer.isActive() == true)
    {
        m_tRetryTocInfoTimer.stop();
    }

    return 1;
}

int CDDAManager::StartRetryTocInfoTimer(int ms) // modified by cychoi 2013.09.06 for ITS 178797
{
    if(m_tRetryTocInfoTimer.isActive() == true)
    {
        m_tRetryTocInfoTimer.stop();
    }
    m_tRetryTocInfoTimer.start(ms);

    return 1;
}

int CDDAManager::InitRetryToc2Info()
{
    if(m_tRetryToc2InfoTimer.isActive() == true)
    {
        m_tRetryToc2InfoTimer.stop();
    }

    return 1;
}

int CDDAManager::StartRetryToc2InfoTimer(int ms) // modified by cychoi 2013.09.06 for ITS 178797
{
    if(m_tRetryToc2InfoTimer.isActive() == true)
    {
        m_tRetryToc2InfoTimer.stop();
    }
    m_tRetryToc2InfoTimer.start(ms);

    return 1;
}

int CDDAManager::InitRetryCDTextInfo()
{
    if(m_tRetryCDTextInfoTimer.isActive() == true)
    {
        m_tRetryCDTextInfoTimer.stop();
    }

    return 1;
}

int CDDAManager::StartRetryCDTextInfoTimer(int ms) // modified by cychoi 2013.09.06 for ITS 178797
{
    if(m_tRetryCDTextInfoTimer.isActive() == true)
    {
        m_tRetryCDTextInfoTimer.stop();
    }
    m_tRetryCDTextInfoTimer.start(ms);

    return 1;
}

int CDDAManager::InitRetryPlayback()
{
    if(m_tRetryPlaybackTimer.isActive() == true)
    {
        m_tRetryPlaybackTimer.stop();
    }

    // removed by cychoi 2014.02.03 for play mode on SW Reset

    return 1;
}

// { added by cychoi 2014.02.03 for play mode on SW Reset
int CDDAManager::InitRetryPlaybackMode()
{
    m_nRetryPlaybackRandomMode = eDM_CD_RANDOM_OFF;
    m_nRetryPlaybackRepeatMode = eDM_CD_REPEAT_OFF;
    m_nRetryPlaybackScanMode = eDM_CD_SCAN_OFF;

    return 1;
}
// } added by cychoi 2014.02.03

int CDDAManager::StartRetryPlaybackTimer(int ms)
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

#ifdef CDDA_DECK_REQ_RETRY_LIMITATION_ENABLED
bool CDDAManager::HandleErrorDisc(bool bRetryToPlay)
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

    MC_HIGH << "CDDAManager::HandleErrorDisc: " << m_nDiscPlayRetryCount << ", " << m_nDiscDetectRetryCount << ", " << bReset << LOG_ENDL;
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
int CDDAManager::GetInternalMediaState()
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

// { added by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050
void CDDAManager::EmitTrackChange(int nTrackNo, bool bTrackChanged)
{
    Q_UNUSED(bTrackChanged);
    m_tStatus.m_nTrackNo = nTrackNo;
    m_nPlayTrackNo       = nTrackNo;
    m_nNextTrackNo       = 0; // modified by cychoi 20103.08.14 for ITS 184423
#if 0
    if(ePrevPlayerState != eMP_STATE_STOPPED && ePrevPlayerState != eMP_STATE_NONE) {
        MC_LOG << "CDDAManager::ePrevPlayerState: " << ePrevPlayerState << ". emit OnCurrentMediaStatusChanged()" << LOG_ENDL;
        emit OnCurrentMediaStatusChanged(PLAYER_EndOfMedia);
    }
#endif
    // IF FF/REW AND REPEATFILE ---> REPEATOFF
    // { modified by cychoi 2013.07.13 for MLT spec out
    if(m_ePlayerState == eMP_STATE_FAST_PLAYING && m_nUserRepeatMode == REPEATFILE) {
    //if(m_ePlayerState == eMP_STATE_FAST_PLAYING && m_nUserRepeatMode == REPEATFILE && IsMLTListMode() == false) {
    // } modified by cychoi 2013.07.13
        m_nUserRepeatMode = -1;
        RepeatPlay(REPEATALL);
    }
    m_bControllerChanged = false;
    // { commented by cychoi 2013.07.13 for MLT spec out
    //if(IsMLTListMode() == true) {
    //    MC_LOG << "CDDAManager::nTrackNo 1: " << GetMLTTrackIndex(m_nPlayTrackNo) << LOG_ENDL;
    //    emit OnTrackChanged(GetMLTTrackIndex(m_nPlayTrackNo));
    //}
    //else
    // } commented by cychoi 2013.07.13
    {
        MC_LOG << "CDDAManager::nTrackNo 1: " << m_nPlayTrackNo - m_nStartTrackNo << LOG_ENDL;
        // { added by cychoi 20103.07.22 for add new signal
        if(m_bUserTrackChanged == true)
        {
            emit OnUserTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
            m_bUserTrackChanged = false;
        }
        // } added by cychoi 20103.07.22
        // { added by cychoi 2014.04.29 for ITS 233500
        else
        {
            if(bTrackChanged == true)
            {
                emit OnAutoTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
            }
        }
        // } added by cychoi 2014.04.29
        emit OnTrackChanged(m_nPlayTrackNo - m_nStartTrackNo);
    }
    emit OnDurationChanged(GetCurrentMediaDuration());
    // removed by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
}
// } added by cychoi 2014.08.29
