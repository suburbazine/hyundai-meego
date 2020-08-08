#include "compaudiomanager.h"
#include "util.h"
#include "deckmanager.h"
#include "mediamanager.h"
#include "performance.h"
#include <QTextCodec>
#include <QTextStream>

#define CA_USE_PRELOAD_DIR_TREE         0

CompAudioManager* CompAudioManager::m_pInstance = 0;

CompAudioManager* CompAudioManager::GetInstance()
{
    if(m_pInstance == 0) {
        m_pInstance = new CompAudioManager();
    }
    
    return m_pInstance;
}

void CompAudioManager::DeleteInstance()
{
    if(m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

CompAudioManager::~CompAudioManager()
{
    MC_LOG << "CompAudioManager::~CompAudioManager()" << LOG_ENDL;
}

void CompAudioManager::Init()
{
    MC_LOG << "CompAudioManager::Init" << LOG_ENDL;
    
    m_tStatus  = CompAudioManagerStatus();
    m_tDirTree = CompAudioDirTreeStatus();

    m_nCurrFileIndex        = -1;
    m_nCurrFolderIdx        = 0;
    m_nCurrFolderNo         = NULL32; // added by cychoi 2013.11.17 for ITS 208698 MP3 Play Info on Cluster 

    m_ePlayerState          = eMP_STATE_NONE;
    m_eLastPlayerState      = eMP_STATE_NONE; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
    m_bLoadComplete         = false;

    // { modified by cychoi 2013-05-18 for ISV 77101
    //m_bRequestFileName      = false;
    //m_bRequestFolderName    = false;
    m_nRequestFileNameFolderNo   = NULL32;
    m_nRequestFileNameFileNo     = NULL32;
    m_nRequestFolderNameFolderNo = NULL32;
    // } modified by cychoi 2013-05-18
    // { modified by cychoi 2013.09.06 for ITS 178797
    m_eAppRequestTagInfo       = CA_DECK_REQ_NONE;
    m_eAppRequestTag2Info      = CA_DECK_REQ_NONE;
    m_eAppRequestAllTagInfo    = CA_DECK_REQ_NONE;
    // } modified by cychoi 2013.09.06
    // { added by cychoi 2013.11.25 for ITS 210845
#ifdef CA_DECK_REQ_RETRY_ENABLED
    m_eRequestFileFolderTotalNum = CA_DECK_REQ_NONE;
#endif
    // } added by cychoi 2013.11.25
    m_tTagInfo.valid        = 0;
    m_tTag2Info.valid       = 0;
    m_tAllTagInfo.valid     = 0;
    m_tAllTagInfo.exist     = 0; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
    m_tAllTagInfo.out_info_type = -1;
    m_tFileName.valid       = 0;
    m_tFolderName.valid     = 0;

    m_nRandomMode           = RANDOMOFF;
    m_nRepeatMode           = REPEATALL;
    m_nUserRandomMode       = -1;
    m_nUserRepeatMode       = -1;
    // { modified by cychoi 2013.11.15 for ITS 205008
    m_nDelayedRandomMode    = -1;
    m_nDelayedRepeatMode    = -1;
    m_bDelayedModeSet       = false;
    // } modified by cychoi 2013.11.15
    // { modified by cychoi 2013.11.18 for ITS 209699
    m_nDelayedScanMode      = -1;
    m_bDelayedScanModeSet   = false;
    // } modified by cychoi 2013.11.18
    m_bREWMode              = false;
    m_bFFREWStateSigSend    = false; // added by sjhyun 2013.09.14 for ITS 189282

    m_eDirTreeMode          = RDTM_NONE;
    m_bFirstPlay            = false;

    m_tFileSizeInFolder.clear();
    m_tUserReqTrackList.clear();

    InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    InitRetryFileFolderTotalNum(); // added by cychoi 2013.11.25 for ITS 210845
    InitRetryTagInfo();
    InitRetryPlayback();
    InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
    m_nRetryPlaybackMediaState = eMP_STATE_NONE;
#endif
// } added by cychoi 2013-06-05
    
#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
    m_nDiscPlayRetryCount = 0;
#endif

    m_scanStatus            = false; // added by wonseok.heo 2013.10.03 for smoke test random repeat
// { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
    m_ePlayDirection = CA_PLAY_NONE;
// } added by cychoi 2013-07-11
}

void CompAudioManager::StartMedia()
{
    MC_LOG << "CompAudioManager::StartMedia" << LOG_ENDL;

    Init();

    m_ePlayerState = eMP_STATE_NONE;
    m_eLastPlayerState = eMP_STATE_NONE; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State

    InitDirTree();
    StartDirTree();

#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
    m_nDiscPlayRetryCount = 0;
#endif
}

void CompAudioManager::EndMedia()
{
}

void CompAudioManager::ProcessPacket(StatusData *pPacket)
{
    // { added by cychoi - To avoid MP3 list read error.
#if _DECK_DIAGNOSTIC_
    if(ExtractResetStart(pPacket) == true)
    {
        return;
    }
#endif
    // } added by cychoi

    // COMP-AUDIO 상태.
    if(ExtractCompAudioStatus(pPacket) == true) {
        return;
    }

    int nRet = ExtractDirTreeStatus(pPacket);
    if(nRet == 1) return;
    if(nRet == 2) {
        return;
    }

    if(ExtractTag2Status(pPacket) == true)              return;
    if(ExtractFolderNameStatus(pPacket) == true)        return;
    if(ExtractFileNameStatus(pPacket) == true)          return;
    if(ExtractFolderFile4Status(pPacket) == true)       return;
    if(ExtractTagStatus(pPacket) == true)               return;
    if(ExtractFileFolderTotalNumber(pPacket) == true) {
        if(m_bLoadComplete == false) {
#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
            m_nDiscDetectRetryCount = 0;
            m_nDiscPlayRetryCount = 0;
#endif
            MC_LOG << "CA LOAD COMPLETE!!!" << LOG_ENDL;
            m_bLoadComplete = true;
            DeckManager::GetInstance()->EmitDiscStateChanged(PLAYER_CAINSERTED);

// { modified by cychoi 2013.06.20 for SW reset on disc loading
// { added by cychoi - To avoid MP3 list read error.
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
// } added by cychoi
// } modified by cychoi 2013.06.20
        }
        return;
    }

    if(ExtractReqError(pPacket) == true)
        return;
}

int CompAudioManager::ChangeMediaMode(MEDIA_PLAYER_MODE eMode)
{
    m_bControllerChanged = true;
    MediaManager::GetInstance()->SetMediaMode(MP_MODE_DISC, eMode);
    return true;
}

QString CompAudioManager::GetCurrentFileName()
{
    int   nLen = 0;
    QChar pName[64];
    
    if(m_tStatus.m_bUnicode) 
    {
        nLen = CharToUnicode((UINT1*)m_tStatus.m_pFileName, pName, 16);
    }
    else 
    {
        for(nLen = 0; nLen < 32; nLen++) {
            pName[nLen] = m_tStatus.m_pFileName[nLen];
            if(pName[nLen] == 0) break;
        }
    }

    if(m_tStatus.m_nType == 0) {
        pName[nLen++] = '.';
        pName[nLen++] = 'M';
        pName[nLen++] = 'P';
        pName[nLen++] = '3';
    }else{
        pName[nLen++] = '.';
        pName[nLen++] = 'W';
        pName[nLen++] = 'M';
        pName[nLen++] = 'A';
    }
    m_strFileName.setUnicode(pName, nLen);

    return m_strFileName;    
}

int CompAudioManager::GetCurrentIndex()
{
    if(m_tStatus.m_nFileNo <= 0) {
        MC_LOG << "CompAudioManager::GetCurrentIndex() return 0" << LOG_ENDL;
        return 0;
    }
    else {
        //MC_LOG << "CompAudioManager::GetCurrentIndex() return " << m_tStatus.m_nFileNo - 1 << LOG_ENDL;
        return m_tStatus.m_nFileNo - 1;
    }
}

void CompAudioManager::SetCurrentIndex(int nIndex)
{
    MC_LOG << "CompAudioManager::SetCurrentIndex(" << nIndex << ")" << LOG_ENDL;
    m_nCurrFileIndex = nIndex;
}

int CompAudioManager::GetCurrentMediaState()
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

int CompAudioManager::GetCurrentMediaDuration()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
    
    return 0;
}

int CompAudioManager::GetCurrentMediaPosition()
{
    return m_tStatus.m_nPlayBackTime;
}

void CompAudioManager::SetCurrentMediaPosition(int position)
{
    Q_UNUSED(position);

    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

int CompAudioManager::GetDeviceState()
{
    DeckManager *p = DeckManager::GetInstance();
    
    return p->GetDeviceState();
}

void CompAudioManager::GetCurrentMediaInfo(MediaInfo *pMediaInfo)
{
    if(pMediaInfo) {
    }
}

void CompAudioManager::GetDeviceInfo(DeviceInfo *pDeviceInfo)
{
    Q_UNUSED(pDeviceInfo);
}

void CompAudioManager::GetPlaylist(PlayList *pPlaylist)
{
    pPlaylist->index        = m_tStatus.m_nFileNo - 1;
    pPlaylist->position     = (double)m_tStatus.m_nPlayBackTime;
    pPlaylist->PlaylistName = QString();
    pPlaylist->mode         = GetCurrentMediaState();

    for(int i = 0; i < m_nTotalFileNum; i++)
    {
        pPlaylist->list->append(QString("Track %1").arg(i+1));
    }
}

void CompAudioManager::SetPlayList(PlayList *pPlaylist)
{
    Q_UNUSED(pPlaylist);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

bool CompAudioManager::SeekableMedia()
{
    return false;
}

void CompAudioManager::SetBrightness(double fBrightness)
{
    DeckManager::GetInstance()->SetBrightness((int)fBrightness);
}

void CompAudioManager::SetContrast(double fContrast)
{
    DeckManager::GetInstance()->SetContrast((int)fContrast);
}

void CompAudioManager::SetTint(double fTint)
{
    DeckManager::GetInstance()->SetTint((int)fTint);
}

void CompAudioManager::SetBrightness(MEDIA_DISPLAY displayPlane, double fBrightness)
{
    Q_UNUSED(displayPlane);
    SetBrightness(fBrightness);
}

void CompAudioManager::SetContrast(MEDIA_DISPLAY displayPlane, double fContrast)
{
    Q_UNUSED(displayPlane);
    SetContrast(fContrast);
}

void CompAudioManager::SetTint(MEDIA_DISPLAY displayPlane, double fTint)
{
    Q_UNUSED(displayPlane);
    SetTint(fTint);
}

double CompAudioManager::GetBrightness()
{
    return (double)DeckManager::GetInstance()->GetBrightness();
}

double CompAudioManager::GetContrast()
{
    return (double)DeckManager::GetInstance()->GetContrast();
}

double CompAudioManager::GetTint()
{
    return (double)DeckManager::GetInstance()->GetTint();
}

void CompAudioManager::SetSystemState(int nSystemState)
{
    m_nSystemState = nSystemState;
    // { added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
    switch(m_nSystemState)
    {
        case SYSTEM_BT_CALL_START:
            m_bBTCallStart = true;
            MC_HIGH << "CompAudioManager::SetSystemState(): SYSTEM_BT_CALL_START" << LOG_ENDL;
            break;
        case SYSTEM_BT_CALL_END:
            m_bBTCallStart = false;
            MC_HIGH << "CompAudioManager::SetSystemState(): SYSTEM_BT_CALL_END" << LOG_ENDL;
            break;
        default:
            break;
    }
    // } added by cychoi 2013.08.01
}

QWidget* CompAudioManager::GetVideoWidget()
{
    return DeckManager::GetInstance()->GetVideoWidget();
}

void CompAudioManager::EnableSubTitle(bool bEnable)
{
    Q_UNUSED(bEnable);
}

void CompAudioManager::SetPlayerEngine(PLAYER_ENGINE_TYPE eType)
{
    Q_UNUSED(eType);
}

void CompAudioManager::SetFontSize(const int fontSize)
{
    Q_UNUSED(fontSize);
}

bool CompAudioManager::ClearDisplay(bool isBackground)
{
    Q_UNUSED(isBackground);
    return false;
}

void CompAudioManager::SetHue(double value)
{
    Q_UNUSED(value);
}

void CompAudioManager::SetHue(MEDIA_DISPLAY displayPlane, double value)
{
    Q_UNUSED(displayPlane);
    Q_UNUSED(value);
}

double CompAudioManager::GetHue()
{
    return 0.0;
}

void CompAudioManager::GetCurrentFileFolderIndex(int *pFileIdx, int *pTotalFiles)
{
    int nFileIdx    = -1;
    int nTotalFiles = -1;
    
// { modified by cychoi 2013.12.27 for ITS 217395
#if USE_FILE_INDEX_IN_FOLDER
    if(m_tFileSizeInFolder.contains(m_tStatus.m_nCurrFolderNo) == true) 
    {
        FILE_SIZE_INFO tInfo = m_tFileSizeInFolder.value(m_tStatus.m_nCurrFolderNo);

        nFileIdx    = m_tStatus.m_nFileNo - tInfo.m_nStartNum;
        nTotalFiles = tInfo.m_nFileSize;
    }
#else
    nFileIdx    = GetCurrentIndex();
    nTotalFiles = m_nTotalFileNum;
#endif
// } modified by cychoi 2013.12.27

    *pFileIdx = nFileIdx;
    *pTotalFiles = nTotalFiles;
}

void CompAudioManager::AudioDragStart()
{
    
}

void CompAudioManager::AudioDragEnd()
{
    
}

void CompAudioManager::Load()
{
    if(DeckManager::GetInstance()->IsExistCDInDeck() == true)
    {
        MC_LOG << "CompAudioManager::Load()" << LOG_ENDL;
        Packet::GetInstace()->Loading();
    }
}

void CompAudioManager::Eject(uint mode)
{
    if(DeckManager::GetInstance()->IsExistCDInDeck() == true || mode == 1)
    {
        // { added by cychoi 2013.09.29 for PV1 Disc sound mute for a while
#ifdef CA_DECK_REQ_RETRY_ENABLED
        InitRetryTagInfo();
        InitRetryPlayback();
        InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
#endif
        // } added by cychoi 2013.09.29
#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
        m_nDiscDetectRetryCount = 0;
        m_nDiscPlayRetryCount = 0;
#endif

        if(GetRepeatMode(m_tStatus.m_nRepeatMode) != REPEATALL) {
            RepeatPlay(REPEATALL);
        }
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }
        
        MC_HIGH << "CompAudioManager::Eject(" << mode << ")" << LOG_ENDL;
        DeckManager::GetInstance()->SetEject(true);
        m_pPacket->FunctionStop();
        m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
        DeckManager::GetInstance()->Eject(mode);
        Init();
        m_pPacket->RequestCommonStatus();
    }
}

void CompAudioManager::EnterKey()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void CompAudioManager::CursorKey(uint direction)
{
    Q_UNUSED(direction);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void CompAudioManager::ReturnKey()
{   
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);
}

void CompAudioManager::SwitchPlayMode(bool bCDDAMode)
{
    DeckManager *pDeckMgr = DeckManager::GetInstance();

    if(pDeckMgr == 0)                   return;
    if(pDeckMgr->IsMixedCD() == false)  return;
    
    DECK_MIXED_CD_MODE eSwitchMode = pDeckMgr->GetSwitchMixedMode();
    
    if(bCDDAMode == true && eSwitchMode == eDM_SWITCH_CD_DA_MODE) {
        if(m_pPacket->PlayModeSwitching(eSwitchMode) > 0) {
            pDeckMgr->SetSwitchMixedMode(eDM_SWITCH_COMP_AUDIO);
            MC_LOG << "CompAudioManager::SwitchPlayMode to CDDA" << LOG_ENDL;
        }
    }
    else if(eSwitchMode == eDM_SWITCH_COMP_AUDIO) {
        if(m_pPacket->PlayModeSwitching(eSwitchMode) > 0) {
            pDeckMgr->SetSwitchMixedMode(eDM_SWITCH_CD_DA_MODE);
            MC_LOG << "CompAudioManager::SwitchPlayMode to CA" << LOG_ENDL;
        }
    }
}

int CompAudioManager::RequestTOC()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int CompAudioManager::RequestTOC2(int nBlockNo)
{
    Q_UNUSED(nBlockNo);
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int CompAudioManager::RequestGraceNoteTOC()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

void CompAudioManager::TenKey(int nKeyID, int pressStatus)
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

int CompAudioManager::RequestCDText(int nTrackNo, int nDataType, int nBlockNo, int nFrameNo)
{
    Q_UNUSED(nTrackNo);
    Q_UNUSED(nDataType);
    Q_UNUSED(nBlockNo);
    Q_UNUSED(nFrameNo);

    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int CompAudioManager::RequestCDTextInfo()
{
    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int CompAudioManager::RequestDirTree(unsigned int nMode, unsigned int nFolderNo, unsigned int nBlockNo)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Dir Tree operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

    if(m_eDirTreeMode == RDTM_NONE || m_eDirTreeMode == RDTM_COMPLETE) 
    {
        DirTreeCommand tCmd;
        BCD4 tBCD4;
        BCD2 tBCD2U;
        BCD2 tBCD2L;

        tBCD4.word          = DEC2BCD4(nFolderNo);
        tBCD2U.BCD.upper    = tBCD4.BCD.upper1;
        tBCD2U.BCD.lower    = tBCD4.BCD.lower1;
        tBCD2L.BCD.upper    = tBCD4.BCD.upper2;
        tBCD2L.BCD.lower    = tBCD4.BCD.lower2;
        
        tCmd.nMode          = nMode;
        tCmd.nUndefined     = 0;
        tCmd.nFolderNoUpper = tBCD2U.byte;
        tCmd.nFolderNoLower = tBCD2L.byte;
        tCmd.nBlcokNo       = nBlockNo;

        m_nChildIdx         = 0;
        m_nMaxChildNum      = 0;

        if(nBlockNo == 0) {
            memset(m_tDvdcDirTree.folder_name, 0x00, sizeof(char) * 130);
        }

        InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
        MC_LOG << "CompAudioManager::RequestDirTree(" << nMode << "," << nFolderNo << "," << nBlockNo << ")" << LOG_ENDL;
        if(m_pPacket->RequestDirTreeStatus(&tCmd) > 0) {
            StartRetryDirTreeTimer(nFolderNo, 0, nBlockNo); // added by cychoi - To avoid MP3 list read error.
            m_eDirTreeMode = RDTM_DIR_TREE;
            return 1;
        }
        else  {
            return -1;
        }
    }
    else {
        return -1;
    }
}

int CompAudioManager::RequestTag()
{   
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't TAG operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppRequestTagInfo == CA_DECK_REQ_NONE || m_tTagInfo.exist == 0) {
        if(m_pPacket->RequestTagStatus() > 0) 
        {
            m_eAppRequestTagInfo = CA_DECK_REQ_SENT;
            return 1;
        }

        m_eAppRequestTagInfo = CA_DECK_REQ_NONE;
    }
    else if(m_eAppRequestTagInfo == CA_DECK_REQ_DONE){
        emit OnTagReceived(&m_tTagInfo);
        return 1;
    }

    return -1;
    // } modified by cychoi 2013.09.06
}

int CompAudioManager::RequestTag2(int nInfoType)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't TAG2 operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

    if(m_tAllTagInfo.out_info_type >= 0) return -1;
    
    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppRequestTag2Info == CA_DECK_REQ_NONE || m_tTag2Info.out_info_type != nInfoType) {
        m_tTag2Info.valid = 0;
        if(m_pPacket->RequestTagStatus2(nInfoType) > 0)
        {
            m_eAppRequestTag2Info = CA_DECK_REQ_SENT;
            return 1;
        }

        m_eAppRequestTag2Info = CA_DECK_REQ_NONE;
    }
    else if(m_eAppRequestTag2Info == CA_DECK_REQ_DONE){
        emit OnTag2Received(&m_tTag2Info);
        return 1;
    }

    return -1;
    // } modified by cychoi 2013.09.06
}

int CompAudioManager::RequestAllTagInfo()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't All TAG operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

    MC_LOG << "CompAudioManager::RequestAllTagInfo()" << LOG_ENDL;
    
    // { modified by cychoi 2013.09.06 for ITS 178797
    if(GetCurrentMediaState() != PLAYER_STOP &&
       ((m_eAppRequestAllTagInfo == CA_DECK_REQ_NONE && m_tAllTagInfo.out_info_type < 0) ||
        (m_eAppRequestAllTagInfo == CA_DECK_REQ_DONE && m_tAllTagInfo.exist == 0))) { // added by cychoi 2013.12.17 for ITS 216033
        m_eAppRequestAllTagInfo     = CA_DECK_REQ_NONE; // added by cychoi 2013.12.17 for ITS 216033
        m_tAllTagInfo.valid         = 0;
        m_tAllTagInfo.exist         = 0; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
        m_tAllTagInfo.out_info_type = 0;
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
        InitRetryTagInfo();
#endif
// } added by cychoi 2013-06-05
        if(m_pPacket->RequestTagStatus2(m_tAllTagInfo.out_info_type) > 0) {
            m_eAppRequestAllTagInfo = CA_DECK_REQ_SENT;
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
            StartRetryTagInfoTimer(200);
#endif
// } added by cychoi 2013-06-05
            return 1;
        }
    }
    else if(m_eAppRequestAllTagInfo == CA_DECK_REQ_DONE) {
        emit OnAllTagReceived(&m_tAllTagInfo);
        return 1;
    }

    return -1;
    // } modified by cychoi 2013.09.06
}

int CompAudioManager::RequestFileName(unsigned int nFolderNo, unsigned int nFileNo)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't File Name operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

    // { modified by cychoi 2013-05-18 for ISV 77101
    if(m_nRequestFileNameFileNo == NULL32)
    //if(m_bRequestFileName == false) 
    // } modified by cychoi 2013-05-18
    {
        if(NextFileName(nFolderNo, nFileNo, 0) > 0) {
            // { modified by cychoi 2013-05-18 for ISV 77101
            m_nRequestFileNameFolderNo = nFolderNo;
            m_nRequestFileNameFileNo = nFileNo;
            //m_bRequestFileName = true;
            // } modified by cychoi 2013-05-18
            return 1;
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }
}

int CompAudioManager::RequestFolderName(unsigned int nFolderNo)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't Folder Name operation on HIGH TEMPERATURE, just return.
        return -1;
    }
    // } added by cychoi 2014.04.09

    // { modified by cychoi 2013-05-18 for ISV 77101
    if(m_nRequestFolderNameFolderNo == NULL32)
    //if(m_bRequestFolderName == false) 
    // } modified by cychoi 2013-05-18
    {
        if(NextFolderName(nFolderNo, 0) > 0) {
            // { modified by cychoi 2013-05-18 for ISV 77101
            m_nRequestFolderNameFolderNo = nFolderNo;
            //m_bRequestFolderName = true;
            // } modified by cychoi 2013-05-18
            return 1;
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }
}


int CompAudioManager::GetTotalFileFolder(TotalFileFolderCount *pCount)
{
    if(pCount == 0) return -1;

    pCount->valid = 1;
    pCount->totalFolders = m_nTotalFolderNum;
    pCount->totalFiles   = m_nTotalFileNum;

    return 1;
}

int CompAudioManager::GetCDStatus(CDDAStatusResponse *pCDStatus)
{
    Q_UNUSED(pCDStatus);

    PRINT_NOT_SUPPORTED;
    // EmitError(PLAYER_COMMANDNOTSUPPORTED);

    return -1;
}

int CompAudioManager::GetCAStatus(CAStatusResponse *pCAStatus)
{
    if(pCAStatus == 0) return -1;

    pCAStatus->playStatus           = m_tStatus.m_nPlayStatus;
    pCAStatus->ROMTrack             = 0; // ?
    pCAStatus->CDTEXTPresent        = DeckManager::GetInstance()->IsExistCDText();
    pCAStatus->randomModeStatus     = m_tStatus.m_nRandomMode;
    pCAStatus->repeatModeStatus     = m_tStatus.m_nRepeatMode;
    pCAStatus->scanModeStatus       = m_tStatus.m_nScanMode;
    pCAStatus->currentFolderNo      = m_tStatus.m_nCurrFolderNo;
    pCAStatus->currentFileNo        = m_tStatus.m_nFileNo;
    pCAStatus->playbackTime         = m_tStatus.m_nPlayBackTime / 1000;// in secs.

    memcpy(pCAStatus->fileName, m_tStatus.m_pFileName, sizeof(char) * 32);

    return 1;
}

int CompAudioManager::GetCurrentScreenState()
{
    switch(m_tStatus.m_nDisplayScreen)
    {
    case eDM_DISPLAY_SCREEN_PLAY:        return PLAYER_DIR_TREE_DISPLAY;
    case eDM_DISPLAY_SCREEN_SETTING:     return PLAYER_SETTING_DISPLAY;
    case eDM_DISPLAY_SCREEN_ROOT_MENU:   return PLAYER_TAG_SCREEN_DISPLAY;
    case eDM_DISPLAY_SCREEN_DEFAULT:     return PLAYER_DEFAULTSCREEN_DISPLAY;
    default:                             
        MC_LOG << "CompAudioManager::GetCurrentScreenState Error: " << m_tStatus.m_nDisplayScreen << LOG_ENDL;
        return PLAYER_DEFAULTSCREEN_DISPLAY;
    }
}

void CompAudioManager::Play()
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
#ifdef CA_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-05

    if(m_nCurrFileIndex >= 0 && m_tStatus.m_nFileNo - 1 != m_nCurrFileIndex && m_bFirstPlay == true) 
    {
        BCD4              tBCD4;
        BCD2              tBCD2;
        DirectContCommand tCmd;

        tBCD4.word          = DEC2BCD4(m_nCurrFileIndex + 1);
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

        MC_LOG << "CompAudioManager::Play()->DirectCont()" << LOG_ENDL;
        m_pPacket->DirectCont(&tCmd);

        m_nCurrFileIndex = -1;
        m_bAutoTrackChanged = false; // modified by shkim 20103.10.01 for AVP mute on Play() call in switchToTuned // modified by cychoi 20103.09.26 for AVP not mute on Play() call // modified by cychoi 20103.08.11 for Play mute
        m_bUserTrackChanged = true; // modified by cychoi 20103.08.11 for ITS 183835 No OSD on track change automatically // added by cychoi 20103.07.22 for OnUserTrackChanged signal
        
        // { moved by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050
        // { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
        m_ePlayDirection = CA_PLAY_CURR;
        // } added by cychoi 2013-07-11
        // } moved by cychoi 2014.08.29
    }
    else {
        MC_LOG << "CompAudioManager::Play()->Play()" << LOG_ENDL;
        m_nCurrFileIndex = -1;
        m_bFirstPlay     = true;
        // { added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
        if(m_bBTCallStart == false)
            m_pPacket->Play();
        else
            m_pPacket->Pause();
        // } added by cychoi 2013.08.01
        // { modified by cychoi 20103.10.06 for Play Time OSD is not displayed on AV Mode Change (only MP3 and CDDA)
        if(m_bAutoTrackChanged == true)
        {
            m_bAutoTrackChanged = GetCurrentMediaState() == PLAYER_PLAY; // modified by cychoi 20103.09.26 for AVP not mute on Play() call // modified by cychoi 20103.08.13 for ITS 183913 Sound mute on toggle play // modified by cychoi 20103.08.13 for Play mute
        }
        // } modified by cychoi 20103.10.06
        m_bUserTrackChanged = GetCurrentMediaState() == PLAYER_STOP; // modified by cychoi 20103.08.11 for ITS 183835 No OSD on track change automatically // added by cychoi 20103.07.22 for OnUserTrackChanged signal
        m_bDelayedModeSet = (m_nDelayedRandomMode != -1) || (m_nDelayedRepeatMode != -1); // added by cychoi 2013.11.15 for ITS 205008
        
        // { moved by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050
        // { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
        m_ePlayDirection = CA_PLAY_NEXT;
        // } added by cychoi 2013-07-11
        // } moved by cychoi 2014.08.29
    }

// { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
    m_ePlayDirection = CA_PLAY_CURR;
// } added by cychoi 2013-07-11

    m_nFFREWRate = -1; // added by cychoi 20103.06.17 for ITS 174004
    m_nFFREWState = PLAYER_PLAY; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
    m_eLastPlayerState = eMP_STATE_PLAYING; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    StartRetryPlaybackTimer(7500); // Compression Audio Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-05
}

void CompAudioManager::Pause()
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't pause operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09
    MC_LOG << "CompAudioManager::Pause()" << LOG_ENDL;
    m_pPacket->Pause();
}

void CompAudioManager::FunctionStop()
{
    MC_LOG << "CompAudioManager::FunctionStop()" << LOG_ENDL;
    m_pPacket->FunctionStop();
    m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
}

void CompAudioManager::Stop()
{
    // { removed by kihyung 2013.2.24 for ISV 72323
    /*
    if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFILE || GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFOLDER) {
        RepeatPlay(REPEATALL);
        emit OnModeChanged(REPEATALL);
    }
    if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMFILE) {
        RandomPlay(RANDOMOFF);
        emit OnModeChanged(RANDOMOFF);
    }
    */
    // } removed by kihyung 2013.2.24 for ISV 72323
    
    MC_LOG << "CompAudioManager::Stop()" << LOG_ENDL;
    m_pPacket->Stop();
    m_eLastPlayerState = eMP_STATE_STOPPED; // added by cychoi 2013.10.06 for ITS 193952 Sync Stop State
}

void CompAudioManager::Next(int nSkipLevel)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't next operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    if(GetCurrentMediaState() == PLAYER_SEARCH) 
    {
        MC_LOG << "CompAudioManager::Next() RETURN ON SEARCH MODE" << LOG_ENDL;

        // { modified by cychoi 2015-05-19 for QE_Smoke Test Ver_45버전(130519) 37 fail
        int nNewTrackIdx;
        if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMOFF)
        {
            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
            nNewTrackIdx = GetNextTrackIdx(nSkipLevel, m_tUserReqTrackList.count() == 0 ? GetCurrentIndex() : m_tUserReqTrackList.last());
            // } modified by cychoi 2013-07-10
        }
        else
        {
            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
            nNewTrackIdx = GetRandomTrackIdx(m_tUserReqTrackList.count() == 0 ? GetCurrentIndex() : m_tUserReqTrackList.last());
            // } modified by cychoi 2013-07-10
        }
        //int nNewTrackIdx = GetNextTrackIdx(nSkipLevel + m_tUserReqTrackList.count());
        // } modified by cychoi 2015-05-19
        m_tUserReqTrackList.append(nNewTrackIdx);
        EmitTrackBuffered(false, nNewTrackIdx);
        // { added by cychoi 2014.01.06 for ITS 217985
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
        // } added by cychoi 2014.01.06
        return;
    }
    
// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-16

	// added by edo.lee 2013.01.25
	if(GetScanMode(m_tStatus.m_nScanMode) != SCANOFF) {
        m_tStatus.m_nScanMode = eDM_COMP_SCAN_NONE; 
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackScanMode = m_tStatus.m_nScanMode;
#endif
// } modified by cychoi 2013.06.20
        Scan(SCANOFF);
    }
    // added by edo.lee 

    if(GetCurrentMediaState() == PLAYER_STOP) {
        MC_LOG << "CompAudioManager::Next(" << nSkipLevel << ") at Stop State" << LOG_ENDL;
        Play();
    }
    else {
        MC_LOG << "CompAudioManager::Next(" << nSkipLevel << ")" << LOG_ENDL;
    }    
    
    
    if(nSkipLevel <= 1) {
        // { modified by cychoi 2013.06.13 for ITS 150493 folder random
        // { modified by wonseok.heo 2013.08.30 NO CR
        if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMFILE){
            m_pPacket->TrackUp(eDM_NEXT_FILE_IN_FOLDER);
        }else{
            if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATALL){
                m_pPacket->TrackUp(eDM_NEXT_FILE_BEYOND_FOLDER);
            }else{
                m_pPacket->TrackUp(eDM_NEXT_FILE_IN_FOLDER);
            }
            //m_pPacket->TrackUp(eDM_NEXT_FILE_BEYOND_FOLDER);
        }
        //} modified by wonseok.heo 2013.08.30 NO CR
        // } modified by cychoi 2013.06.13

        EmitTrackBuffered(false, -1); // added by cychoi 20103.07.20 for new OSD display
    }
    else 
    {
        BCD4              tBCD4;
        BCD2              tBCD2;
        DirectContCommand tCmd;
        // { modified by cychoi 2013.06.13 for ITS 150493 folder random
        int nNewTrackIdx;
        if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMOFF)
        {
            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
            nNewTrackIdx = GetNextTrackIdx(nSkipLevel, GetCurrentIndex());
            // } modified by cychoi 2013-07-10
        }
        else
        {
            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
            nNewTrackIdx = GetRandomTrackIdx(GetCurrentIndex());
            // } modified by cychoi 2013-07-10
        }
        //int               nNewTrackIdx = GetNextTrackIdx(nSkipLevel);
        // } modified by cychoi 2013.06.13

        tBCD4.word          = DEC2BCD4(nNewTrackIdx + 1);
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

        EmitTrackBuffered(false, nNewTrackIdx); // added by cychoi 20103.07.20 for new OSD display
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
// { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
    m_ePlayDirection = CA_PLAY_NEXT;
// } added by cychoi 2013-07-11

// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    StartRetryPlaybackTimer(7500); // Compression Audio Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-16
}

void CompAudioManager::Prev(int nSkipLevel, int nForced)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't prev operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    if(GetCurrentMediaState() == PLAYER_SEARCH) 
    {
        MC_LOG << "CompAudioManager::Prev() RETURN ON SEARCH MODE" << LOG_ENDL;

        // { modified by cychoi 2015-05-19 for QE_Smoke Test Ver_45버전(130519) 37 fail
        int nNewTrackIdx;
        if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMOFF)
        {
            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
            nNewTrackIdx = GetPrevTrackIdx(nSkipLevel, m_tUserReqTrackList.count() == 0 ? GetCurrentIndex() : m_tUserReqTrackList.last());
            // } modified by cychoi 2013-07-10
        }
        else
        {
            // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
            nNewTrackIdx = GetRandomTrackIdx(m_tUserReqTrackList.count() == 0 ? GetCurrentIndex() : m_tUserReqTrackList.last());
            // } modified by cychoi 2013-07-10
        }
        //int nNewTrackIdx = GetPrevTrackIdx(nSkipLevel + m_tUserReqTrackList.count());
        // } modified by cychoi 2015-05-19
        m_tUserReqTrackList.append(nNewTrackIdx);
        EmitTrackBuffered(true, nNewTrackIdx);
        // { added by cychoi 2014.01.06 for ITS 217985
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
        // } added by cychoi 2014.01.06
        return;
    }
    
// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-16

	// added by edo.lee 2013.01.25
	if(GetScanMode(m_tStatus.m_nScanMode) != SCANOFF) {
	    m_tStatus.m_nScanMode = eDM_COMP_SCAN_NONE; 
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackScanMode = m_tStatus.m_nScanMode;
#endif
// } modified by cychoi 2013.06.20
	    Scan(SCANOFF);
	}
    // added by edo.lee 

    if(GetCurrentMediaState() == PLAYER_STOP) {
        MC_LOG << "CompAudioManager::Prev(" << nSkipLevel << ", " << nForced << ") at Stop State" << LOG_ENDL;
        Play();
    }
    else {
        MC_LOG << "CompAudioManager::Prev(" << nSkipLevel << ", " << nForced << ")" << LOG_ENDL;
    }
    
    if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF && m_tStatus.m_nPlayBackTime < 3000) {
        Next(nSkipLevel);
// { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
        m_ePlayDirection = CA_PLAY_NEXT;
// } added by cychoi 2013-07-11
    }
    else {
        if(nSkipLevel <= 1 && nForced == 0) {
            // { modified by cychoi 2013.06.13 for ITS 150493 folder random
            // { modified by wonseok.heo 2013.08.30 NO CR
            if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMFILE){
                m_pPacket->TrackDown(eDM_PREV_FILE_IN_FOLDER);
            }else{
                if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATALL){
                    m_pPacket->TrackDown(eDM_PREV_FILE_BEYOND_FOLDER);
                }else{
                    m_pPacket->TrackDown(eDM_PREV_FILE_IN_FOLDER);
                }
               // m_pPacket->TrackDown(eDM_PREV_FILE_BEYOND_FOLDER);
            }
            //} modified by wonseok.heo 2013.08.30 NO CR
            // } modified by cychoi 2013.06.13

            EmitTrackBuffered(true, -1); // added by cychoi 20103.07.20 for new OSD display
        }
        else 
        {
            BCD4              tBCD4;
            BCD2              tBCD2;
            DirectContCommand tCmd;
            // { modified by cychoi 2013.06.13 for ITS 150493 folder random
            int nNewTrackIdx;
            if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMOFF)
            {
                // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
                nNewTrackIdx = GetPrevTrackIdx(nSkipLevel, GetCurrentIndex());
                // } modified by cychoi 2013-07-10
            }
            else
            {
                // { modified by cychoi 2013-07-10 for ITS 179045 Track Up <-> Down press repeatedly
                nNewTrackIdx = GetRandomTrackIdx(GetCurrentIndex());
                // } modified by cychoi 2013-07-10
            }
            //int               nNewTrackIdx = GetPrevTrackIdx(nSkipLevel);
            // } modified by cychoi 2013.06.13

            tBCD4.word          = DEC2BCD4(nNewTrackIdx + 1);
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

            EmitTrackBuffered(true, nNewTrackIdx); // added by cychoi 20103.07.20 for new OSD display
        }

// { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
        m_ePlayDirection = CA_PLAY_PREV;
// } added by cychoi 2013-07-11
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal

// { added by cychoi 2013-06-16 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    StartRetryPlaybackTimer(7500); // Compression Audio Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
#endif
// } added by cychoi 2013-06-16
}

void CompAudioManager::Scan(int nMode)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't scan operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    // { modified by cychoi 2013.11.18 for ITS 209699
    if(GetCurrentMediaState() == PLAYER_SEARCH) 
    {
        if(nMode == SCANFILE || nMode == SCANALL)
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

    MC_LOG << "CompAudioManager::Scan(" << nMode << ")" << LOG_ENDL;

    switch(nMode)
    {
    case SCANFILE:
        // RANDOM MODE ---> RANDOM OFF
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }
        m_scanStatus = true; // added by wonseok.heo 2013.10.03 for smoke test random repeat
        m_pPacket->ScanOn(eDM_SCAN_FILE_IN_FOLDER);
        break;
    case SCANFOLDER:
        MC_LOG << "CompAudioManager::Scan() SCANFOLDER is invalid option" << LOG_ENDL;
        break;
    case SCANALL:
        // RANDOM MODE ---> RANDOM OFF
        if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
            RandomPlay(RANDOMOFF);
        }
        else if(GetRepeatMode(m_tStatus.m_nRepeatMode) != REPEATALL) {
            RepeatPlay(REPEATALL);
        }
        m_scanStatus = true; // added by wonseok.heo 2013.10.03 for smoke test random repeat
        m_pPacket->ScanOn(eDM_SCAN_FILE_IN_DISC);
        break;
    default:
        // { added by wonseok.heo 2013.10.03 for smoke test random repeat
        if(m_scanStatus == true){
            m_scanStatus = false;
        }// } added by wonseok.heo 2013.10.03 for smoke test random repeat
        m_pPacket->ScanOff();
        break;
    }

    // { modified by cychoi 2013.10.01 for ITS 192388
    // { added by wonseok.heo for ITS 189165 unMute
    if(nMode == SCANFILE || nMode == SCANALL)
    {
        m_bAutoTrackChanged = false;
        m_bUserTrackChanged = true;
    }
    // } added by wonseok.heo for ITS 189165 unMute
    // { modified by cychoi 2013.10.01
}

void CompAudioManager::RandomPlay(int nMode, int bPlay)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't random play operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    Q_UNUSED(bPlay);
    
    MC_LOG << "CompAudioManager::RandomPlay(" << nMode << ")" << LOG_ENDL;

    RandomPlay_Impl(nMode);
}

void CompAudioManager::RepeatPlay(int nMode, int bPlay)
{
    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(DeckManager::GetInstance()->IsDeckBlocked() == true)
    {
        // We can't repeat play operation on HIGH TEMPERATURE, just return.
        return;
    }
    // } added by cychoi 2014.04.09

    Q_UNUSED(bPlay);
    
    MC_LOG << "CompAudioManager::RepeatPlay(" << nMode << ")" << LOG_ENDL;

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

    if(m_nRepeatMode != nMode) 
    {
        MC_LOG << "CompAudioManager::RepeatPlay(): nMode(" << nMode << ") != nPrevMode(" << m_nRepeatMode << ")" << LOG_ENDL;
        
        switch(nMode)
        {
        case REPEATFILE:
            // RANDOM MODE ---> RANDOM OFF
            if(GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF) {
                RandomPlay(RANDOMOFF);
            }
            m_nRepeatMode = nMode;
            m_pPacket->RepeatOn(eDM_REPEAT_FILE);
            break;
        case REPEATFOLDER:
            m_nRepeatMode = nMode;
            if(m_nRandomMode != RANDOMOFF) 
            {
                // RANDOM IN DISC ---> RANDOM IN FOLDER
                if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMFOLDER) {
                    m_nRandomMode = RANDOMOFF;
                    RandomPlay(RANDOMFILE);
                }
                m_pPacket->RepeatOn(eDM_REPEAT_FOLDER_UNDER_FOLDER);// added by yongkyun.lee 20130619 for : NO CR play mode 
            }
            else
            {
                m_pPacket->RepeatOn(eDM_REPEAT_FOLDER_UNDER_FOLDER);
            }
            break;
        case REPEATALL:
            m_nRepeatMode = nMode;
            if(m_nRandomMode != RANDOMOFF)
            {
                // RANDOM IN FOLDER ---> RANDOM IN DISC
                if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMFILE) {
                    m_nRandomMode = RANDOMOFF;
                    RandomPlay_Impl(RANDOMFILE, FALSE);
                }
            }
            else
            {
                m_pPacket->RepeatOff();
            }
            break;
        default:
            MC_LOG << "CompAudioManager::RepeatPlay() " << nMode << " is invalid option" << LOG_ENDL;
            return;
        }    
        m_nUserRepeatMode = nMode;

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
    else
    {
        MC_LOG << "CompAudioManager::RepeatPlay(): nMode(" << nMode << ") != nPrevMode(" << m_nRepeatMode << ")" << LOG_ENDL;
    }
}

void CompAudioManager::FastForward(int nRate)
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
        // { commented by cychoi 2013.10.06 for Prevent Fix
        //else if(nRate == FAST_SPEED_X16) {
        //    m_nFFREWState = PLAYER_FASTFORWARD_X16;
        //}
        // } commented by cychoi 2013.10.06
        else if(nRate == FAST_SPEED_X20) {
            m_nFFREWState = PLAYER_FASTFORWARD_X20;
        }
        // } commented by cychoi 2013-06-07
        // } modified by cychoi 2013.09.22
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
// { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
    m_ePlayDirection = CA_PLAY_FF;
// } added by cychoi 2013-07-11
}

void CompAudioManager::Rewind(int nRate)
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
        // { commented by cychoi 2013.10.06 for Prevent Fix
        //else if(nRate == FAST_SPEED_X16) {
        //    m_nFFREWState = PLAYER_REWIND_X16;
        //}
        // } commented by cychoi 2013.10.06
        else if(nRate == FAST_SPEED_X20) {
            m_nFFREWState = PLAYER_REWIND_X20;
        }
        // } commented by cychoi 2013-06-07
        // } modified by cychoi 2013.09.22
    }

    m_bAutoTrackChanged = false; // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
    m_bUserTrackChanged = true; // added by cychoi 20103.07.22 for OnUserTrackChanged signal
// { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
    m_ePlayDirection = CA_PLAY_REW;
// } added by cychoi 2013-07-11
}

void CompAudioManager::NextFolder(PlayList *pPlayList)
{
    Q_UNUSED(pPlayList);
}

void CompAudioManager::PrevFolder(PlayList *pPlayList)
{
    Q_UNUSED(pPlayList);
}

// { added by cychoi - To avoid MP3 list read error.
void CompAudioManager::HandleRetryDirTree()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request folder & file name status after disc detection is completed. */
        return;
    }

    MC_LOG << "CompAudioManager::HandleRetryDirTree()" << LOG_ENDL;

    m_tRetryDirTreeTimer.stop();

    switch(m_eDirTreeMode)
    {
        case RDTM_DIR_TREE:
            if(m_nRetryDirTreeBlockNo != NULL32 && m_nRetryDirTreeFolderNo != NULL32)
            {
                DirTreeCommand tCmd;
                BCD4 tBCD4;
                BCD2 tBCD2U;
                BCD2 tBCD2L;
                
                tBCD4.word          = DEC2BCD4(m_nRetryDirTreeFolderNo);
                tBCD2U.BCD.upper    = tBCD4.BCD.upper1;
                tBCD2U.BCD.lower    = tBCD4.BCD.lower1;
                tBCD2L.BCD.upper    = tBCD4.BCD.upper2;
                tBCD2L.BCD.lower    = tBCD4.BCD.lower2;
                
                tCmd.nMode          = 0;
                tCmd.nUndefined     = 0;
                tCmd.nFolderNoUpper = tBCD2U.byte;
                tCmd.nFolderNoLower = tBCD2L.byte;
                tCmd.nBlcokNo       = m_nRetryDirTreeBlockNo;
                
                m_nChildIdx         = 0;
                m_nMaxChildNum      = 0;
                
                if(m_nRetryDirTreeBlockNo == 0)
                {
                    memset(m_tDvdcDirTree.folder_name, 0x00, sizeof(char) * 130);
                }

                MC_LOG << "CompAudioManager::HandleRetryDirTree RequestDirTreeStatus: m_nRetryDirTreeBlockNo: " << m_nRetryDirTreeBlockNo << LOG_ENDL;
                if(m_pPacket->RequestDirTreeStatus(&tCmd) > 0)
                {
                    /* If we will do not receive the response on retry, we do not know what to do. */
                }
                else
                {
                    /* If we will fail the request for retry, we do not know what to do. */
                }
            }
            break;
        case RDTM_CURR_FOLDER_NAME:
            if(m_nRetryDirTreeFolderNo != NULL32)
            {
                MC_LOG << "CompAudioManager::HandleRetryDirTree NextFolderName m_nRetryDirTreeFolderNo: " << m_nRetryDirTreeFolderNo << LOG_ENDL;
                if(NextFolderName(m_nRetryDirTreeFolderNo, 0) > 0)
                {
                    /* If we will do not receive the response on retry, we do not know what to do. */
                }
                else
                {
                    /* If we will fail the request for retry, we do not know what to do. */
                }
            }
            break;
        case RDTM_CHILD_FILE_NAME:
            if(m_nRetryDirTreeFileNo != NULL32 && m_nRetryDirTreeFolderNo != NULL32)
            {
                MC_LOG << "CompAudioManager::HandleRetryDirTree NextFileName: m_nRetryDirTreeFileNo: " << m_nRetryDirTreeFileNo << "(" << m_nRetryDirTreeFolderNo << ")" << LOG_ENDL;
                if(NextFileName(m_nRetryDirTreeFolderNo, m_nRetryDirTreeFileNo, m_nRetryDirTreeBlockNo) > 0)
                {
                    /* If we will do not receive the response on retry, we do not know what to do. */
                }
                else
                {
                    /* If we will fail the request for retry, we do not know what to do. */
                }
            }
            break;
        case RDTM_CHILD_FOLDER_NAME:
            if(m_nRetryDirTreeFolderNo != NULL32 && m_nRetryDirTreeBlockNo != NULL32)
            {
                MC_LOG << "CompAudioManager::HandleRetryDirTree NextFolderName m_nRetryDirTreeFolderNo: " << m_nRetryDirTreeFolderNo << "(" << m_nRetryDirTreeBlockNo << ")" << LOG_ENDL;
                if(NextFolderName(m_nRetryDirTreeFolderNo, m_nRetryDirTreeBlockNo) > 0)
                {
                    /* If we will do not receive the response on retry, we do not know what to do. */
                }
                else
                {
                    /* If we will fail the request for retry, we do not know what to do. */
                }
            }
            break;
        // { modified by cychoi 2013.09.06 for ITS 178797
        case RDTM_COMPLETE:
            // The below signals should be sent to AVP on Deck Reset. If AVP receive this signals, AVP should retry DirTree Request.
            emit OnDirTreeReceived(&m_tDvdcDirTree);
            emit OnDirTreeCompleted();
            break;
        // } modified by cychoi 2013.09.06
        default:
            /* Never happen */
            break;
    }
}
// } added by cychoi

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
// { added by cychoi 2013.11.25 for ITS 210845
void CompAudioManager::HandleRetryFileFolderTotalNum()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request tag info status after disc detection is completed. */
        return;
    }

    MC_HIGH << "CompAudioManager::HandleRetryFileFolderTotalNum()" << LOG_ENDL;
 
    m_tRetryFileFolderTotalNumTimer.stop();

    if(m_eRequestFileFolderTotalNum == CA_DECK_REQ_SENT)
    {
#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
        if(HandleErrorDisc(false) == true)
        {
            return;
        }
#endif
        // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
        m_pPacket->RequestFileFolderTotalNum();
        /* We will retry 3 times if we will do not receive the response on retry. */
#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
        StartRetryFileFolderTotalNumTimer(200);
#endif
        // } modified by cychoi 2014.04.25
    }
}
// } added by cychoi 2013.11.25

void CompAudioManager::HandleRetryTagInfo()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request tag info status after disc detection is completed. */
        return;
    }

    MC_HIGH << "CompAudioManager::HandleRetryTagInfo()" << LOG_ENDL;
 
    m_tRetryTagInfoTimer.stop();

    m_tAllTagInfo.valid         = 0;
    m_tAllTagInfo.exist         = 0; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
    m_tAllTagInfo.out_info_type = 0;
    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppRequestAllTagInfo == CA_DECK_REQ_SENT)
    {
        if(m_pPacket->RequestTagStatus2(m_tAllTagInfo.out_info_type) > 0)
        {
            /* If we will do not receive the response on retry, we do not know what to do. */
        }
        else
        {
            /* If we will fail the request for retry, we do not know what to do. */
            m_eAppRequestAllTagInfo = CA_DECK_REQ_NONE; // added by cychoi 2013.09.05 for ITS 188273
        }
    }
    // } modified by cychoi 2013.09.06
}

void CompAudioManager::HandleRetryPlayback()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    if(tStatus.m_nDiscDetect != eDM_DISC_DETECT_COMPLETED)
    {
        /* Please request playback after disc detection is completed. */
        return;
    }

    MC_HIGH << "CompAudioManager::HandleRetryPlayback(): m_nRetryPlaybackMediaState=" << m_nRetryPlaybackMediaState << LOG_ENDL;

    m_tRetryPlaybackTimer.stop();
            
#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
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
            m_nRetryPlaybackRepeatMode = eDM_COMP_REPEAT_OFF;
            m_nRetryPlaybackRandomMode = eDM_COMP_RANDOM_OFF;
        }
        // } modified by cychoi 2014.02.03
                
#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
        if(m_tStatus.m_nPlayBackTime != 0)
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

CompAudioManager::CompAudioManager(QObject *parent) : QObject(parent),
m_bLoadComplete(false),    
m_pPacket(0),
//m_pTree(0), // commented by cychoi 2014.01.24 for unused var
m_nTotalFolderNum(0),
m_nTotalFileNum(0),
m_nSystemState(0),
m_nFFREWRate(-1),
m_nFFREWState(PLAYER_PLAY), // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error // added by cychoi 2013-06-07 for ITS 172685
m_bFFREWStateSigSend(false), // added by sjhyun 2013.09.14 for ITS 189282
m_strFileName(QString()),
//m_bFirstBoot(false), // commented by cychoi 20130.07.11 for keeping playback options
m_bControllerChanged(false),
// { modified by cychoi 2013.06.22 for SW reset on disc loading
#ifdef CA_DECK_REQ_RETRY_ENABLED
m_nRetryPlaybackMediaState(eMP_STATE_NONE),
m_nRetryPlaybackRandomMode(eDM_COMP_RANDOM_OFF),
m_nRetryPlaybackRepeatMode(eDM_COMP_REPEAT_OFF),
m_nRetryPlaybackScanMode(eDM_COMP_SCAN_OFF),
#endif
// } modified by cychoi 2013.06.22
m_bAutoTrackChanged(true), // added by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
m_bUserTrackChanged(false), // added by cychoi 20103.07.22 for OnUserTrackChanged signal
m_bBTCallStart(false) // added by cychoi 2013.08.01 for performing pause instead of play on BT call (PowerOff mode)
{
    m_tDvdcDirTree.folder_name = new char[130];
    for(int i = 0; i < 10; i++) {
        m_tDvdcDirTree.entry[i] = new DvdcDirEntry;
        m_tDvdcDirTree.entry[i]->file_folder_name = new char[130];
    }
        
    Init();
    //m_pTree   = new CTree(); // commented by cychoi 2014.01.24 for unused var
    m_pPacket = Packet::GetInstace();

    connect(&m_tRetryDirTreeTimer, SIGNAL(timeout()), this, SLOT(HandleRetryDirTree())); // added by cychoi - To avoid MP3 list read error.
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    connect(&m_tRetryFileFolderTotalNumTimer, SIGNAL(timeout()), this, SLOT(HandleRetryFileFolderTotalNum())); // added by cychoi 2013.11.25 for ITS 210845
    connect(&m_tRetryTagInfoTimer, SIGNAL(timeout()), this, SLOT(HandleRetryTagInfo()));
    connect(&m_tRetryPlaybackTimer, SIGNAL(timeout()), this, SLOT(HandleRetryPlayback()));
#endif
// } added by cychoi 2013-06-05

    // { added by cychoi 2013-07-15 for Prevent fix
    //m_bRequestDirTree              = 0; // modified by cychoi 2013.09.06 for ITS 178797
    m_nChildIdx                    = 0;
    m_nMaxChildNum                 = 0;
    m_tDvdcDirTree.valid           = 0;
    m_tDvdcDirTree.curr_folder     = 0;
    m_tDvdcDirTree.block_no        = 0;
    m_tDvdcDirTree.unicode         = 0;
    m_tDvdcDirTree.chld_folder_num = 0;
    m_tDvdcDirTree.chld_file_num   = 0;
    m_bREWMode                     = false;
    // } added by cychoi 2013-07-15
}

void CompAudioManager::RandomNotice(int nMode)
{
    MC_LOG << "CompAudioManager::RandomNotice(" << nMode << ")" << LOG_ENDL;
    
    switch(nMode)
    {
    case RANDOMFILE:
        m_pPacket->RandomOn(eDM_RANDOM_FILE_IN_FOLDER);
        break;
    case RANDOMFOLDER:
        m_pPacket->RandomOn(eDM_RANDOM_FILE_IN_DISC);
        break;
    default:
        m_pPacket->RandomOff();
        break;
    }
}

void CompAudioManager::RandomPlay_Impl(int nMode, int bSignalEmit)
{
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

    if(m_nRandomMode != nMode) 
    {
        MC_LOG << "CompAudioManager::RandomPlay_Impl(): nMode(" << nMode << ") != nPrevMode(" << m_nRandomMode << ")" << LOG_ENDL;
        
        if(nMode == RANDOMOFF) 
        {
            m_nUserRandomMode       = -1;
            m_nRandomMode           = RANDOMOFF;
            m_tStatus.m_nRandomMode = eDM_COMP_RANDOM_OFF;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CA_DECK_REQ_RETRY_ENABLED
            m_nRetryPlaybackRandomMode = m_tStatus.m_nRandomMode;
#endif
// } modified by cychoi 2013.06.20

            // REPEAT MODE ---> ALL
            if(m_nRepeatMode == REPEATFILE) { // modified by wonseok.heo for 173678 2013.06.13
                RepeatPlay(REPEATALL);
                if(bSignalEmit) emit OnModeChanged(m_nRepeatMode);
            }
            // { added by wonseok.heo 2013.08.30 NO CR
            else if(m_nRepeatMode == REPEATFOLDER){
                m_nRepeatMode = -1;
                RepeatPlay(REPEATFOLDER);
                if(bSignalEmit) emit OnModeChanged(m_nRepeatMode);
            }
            //} added by wonseok.heo 2013.08.30 NO CR
            
            RandomNotice(nMode);
            if(bSignalEmit) emit OnModeChanged(m_nRandomMode);
            // { added by cychoi 2013.10.01 for ITS 192388
            if(GetCurrentMediaState() == PLAYER_PAUSE)
            {
                m_bAutoTrackChanged = false;
                m_bUserTrackChanged = true;
            }
            // } added by cychoi 2013.10.01
            return;
        }
        else if(nMode == RANDOMFILE) 
        {
            // SCAN ---> OFF
            if(GetScanMode(m_tStatus.m_nScanMode) != SCANOFF) {
                m_tStatus.m_nScanMode = eDM_COMP_SCAN_NONE; // added by kihyung 2012.12.20 for 62783(not to emit OnScanCompleted() signal)
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CA_DECK_REQ_RETRY_ENABLED
                m_nRetryPlaybackScanMode = m_tStatus.m_nScanMode;
#endif
// } modified by cychoi 2013.06.20
                Scan(SCANOFF);
            }

            // REPEAT FILE ---> ALL
            if(m_nRepeatMode == REPEATFILE) {
                RepeatPlay(REPEATALL);
            }

            // REPEAT FOLDER ---> RANDOM FILE
            if(m_nRepeatMode == REPEATFOLDER) {
                m_tStatus.m_nRandomMode = eDM_COMP_RANDOM_FILES_IN_FOLDER;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CA_DECK_REQ_RETRY_ENABLED
                m_nRetryPlaybackRandomMode = m_tStatus.m_nRandomMode;
#endif
// } modified by cychoi 2013.06.20
                m_nUserRandomMode       = RANDOMFILE;
                MC_LOG << "CompAudioManager::RandomPlay_Impl() -> RANDOMFILE" << LOG_ENDL;
            }
            // REPEAT ALL ---> RANDOM ALL
            else {
                m_tStatus.m_nRandomMode = eDM_COMP_RANDOM_FILES_IN_DISC;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CA_DECK_REQ_RETRY_ENABLED
                m_nRetryPlaybackRandomMode = m_tStatus.m_nRandomMode;
#endif
// } modified by cychoi 2013.06.20
                m_nUserRandomMode       = RANDOMFOLDER;
                MC_LOG << "CompAudioManager::RandomPlay_Impl() -> RANDOMFOLDER" << LOG_ENDL;
            }
            m_nRandomMode = RANDOMFILE;
            if(bSignalEmit) emit OnModeChanged(m_nRandomMode);
        }
        else 
        {
            m_nUserRandomMode = -1;
            MC_LOG << "CompAudioManager::RandomPlay_Impl() RANDOMFOLDER is invalid option" << LOG_ENDL;
            return;
        }

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
        MC_LOG << "CompAudioManager::RandomPlay_Impl(): nMode(" << nMode << ") != nPrevMode(" << m_nRandomMode << ")" << LOG_ENDL;
    }
}

int CompAudioManager::InitDirTree()
{
    m_nTotalFolderNum = 0;
    m_nTotalFileNum   = 0;

// { added by cychoi 2013.11.25 for ITS 210845
#ifdef CA_DECK_REQ_RETRY_ENABLED
    InitRetryFileFolderTotalNum();
    // { modified by cychoi 2014.04.25 for Retry on Request Function Fail
    m_pPacket->RequestFileFolderTotalNum();
    m_eRequestFileFolderTotalNum = CA_DECK_REQ_SENT;
    StartRetryFileFolderTotalNumTimer(200);
    // } modified by cychoi 2014.04.25
#else
    m_pPacket->RequestFileFolderTotalNum();
#endif
// } modified by cychoi 2013.11.25
    
    return true;
}

int CompAudioManager::StartDirTree()
{
    return 0;
}

int CompAudioManager::NextDirTree()
{
    DirTreeCommand tCmd;

    BCD4 tBCD4;
    BCD2 tBCD2U;
    BCD2 tBCD2L;

    tBCD4.word = DEC2BCD4(m_tDirTree.m_nCurrFolderNo);
    tBCD2U.BCD.upper = tBCD4.BCD.upper1;
    tBCD2U.BCD.lower = tBCD4.BCD.lower1;
    tBCD2L.BCD.upper = tBCD4.BCD.upper2;
    tBCD2L.BCD.lower = tBCD4.BCD.lower2;
    
    tCmd.nMode          = 0;
    tCmd.nUndefined     = 0;
    tCmd.nFolderNoUpper = tBCD2U.byte;
    tCmd.nFolderNoLower = tBCD2L.byte;
    tCmd.nBlcokNo       = m_tDirTree.m_nCurrBlockNo;

    // MC_LOG << "CompAudioManager::curr folder: " << m_tDirTree.m_nCurrFolderNo << ", curr block: " << m_tDirTree.m_nCurrBlockNo << LOG_ENDL;

    m_pPacket->RequestDirTreeStatus(&tCmd);

    return 0;
}

// { added by cychoi - To avoid MP3 list read error.
int CompAudioManager::InitRetryDirTree()
{
    if(m_tRetryDirTreeTimer.isActive() == true)
    {
        m_tRetryDirTreeTimer.stop();
    }

    m_nRetryDirTreeFolderNo = NULL32;
    m_nRetryDirTreeFileNo = NULL32;
    m_nRetryDirTreeBlockNo = NULL32;

    return 1;
}

int CompAudioManager::StartRetryDirTreeTimer(unsigned int nFolderNo, unsigned int nFileNo, unsigned int nBlockNo)
{
    m_nRetryDirTreeFolderNo = nFolderNo;
    m_nRetryDirTreeFileNo = nFileNo;
    m_nRetryDirTreeBlockNo = nBlockNo;

    if(m_tRetryDirTreeTimer.isActive() == true)
    {
        m_tRetryDirTreeTimer.stop();
    }
    m_tRetryDirTreeTimer.start(500);

    return 1;
}
// } added by cychoi - To avoid MP3 list read error.

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
// { added by cychoi 2013.11.25 for ITS 210845
int CompAudioManager::InitRetryFileFolderTotalNum()
{
    if(m_tRetryFileFolderTotalNumTimer.isActive() == true)
    {
        m_tRetryFileFolderTotalNumTimer.stop();
    }

    return 1;
}

int CompAudioManager::StartRetryFileFolderTotalNumTimer(int ms)
{
    if(m_tRetryFileFolderTotalNumTimer.isActive() == true)
    {
        m_tRetryFileFolderTotalNumTimer.stop();
    }
    m_tRetryFileFolderTotalNumTimer.start(ms);

    return 1;
}
// } added by cychoi 2013.11.25

int CompAudioManager::InitRetryTagInfo()
{
    if(m_tRetryTagInfoTimer.isActive() == true)
    {
        m_tRetryTagInfoTimer.stop();
    }

    return 1;
}

int CompAudioManager::StartRetryTagInfoTimer(int ms) // modified by cychoi 2013.09.06 for ITS 178797
{
    if(m_tRetryTagInfoTimer.isActive() == true)
    {
        m_tRetryTagInfoTimer.stop();
    }
    m_tRetryTagInfoTimer.start(ms); // modified by cychoi 2013.09.06 for ITS 178797

    return 1;
}

int CompAudioManager::InitRetryPlayback()
{
    if(m_tRetryPlaybackTimer.isActive() == true)
    {
        m_tRetryPlaybackTimer.stop();
    }

    // removed by cychoi 2014.02.03 for play mode on SW Reset

    return 1;
}

// { added by cychoi 2014.02.03 for play mode on SW Reset
int CompAudioManager::InitRetryPlaybackMode()
{
    m_nRetryPlaybackRandomMode = eDM_COMP_RANDOM_OFF;
    m_nRetryPlaybackRepeatMode = eDM_COMP_REPEAT_OFF;
    m_nRetryPlaybackScanMode = eDM_COMP_SCAN_OFF;

    return 1;
}
// } added by cychoi 2014.02.03

int CompAudioManager::StartRetryPlaybackTimer(int ms)
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

int CompAudioManager::NextFileName(unsigned int nFolderNo, unsigned int nFileNo, unsigned int nBlockNo)
{
    Q_UNUSED(nBlockNo);
    BCD4            tBCD4;
    BCD2            tBCD2;
    FileNameCommand tCmd;

    tCmd.nFileDesign    = 0;
    tCmd.nByteValue     = 1; // BLOCK NUMBER AT OPERAND5
    tCmd.nUndefined1    = 0;

    // FOLDER NUMBER
    tBCD4.word          = DEC2BCD4(nFolderNo);
    tBCD2.BCD.upper     = tBCD4.BCD.upper2;
    tBCD2.BCD.lower     = tBCD4.BCD.lower2;
    tCmd.nFolderNoUpper = tBCD4.BCD.lower1;
    tCmd.nFolderNoLower = tBCD2.byte;

    tCmd.nUndefined2    = 0;

    // FILE NUMBER
    tBCD4.word          = DEC2BCD4(nFileNo);
    tBCD2.BCD.upper     = tBCD4.BCD.upper2;
    tBCD2.BCD.lower     = tBCD4.BCD.lower2;
    tCmd.nFileNoUpper   = tBCD4.BCD.lower1;
    tCmd.nFileNoLower   = tBCD2.byte;

    tCmd.nBlockNoByteLength = DS_FILE_NAME_CHAR_LENGTH;

    MC_LOG << "CompAudioManager::NextFileName(" << nFileNo << "-" << tBCD4.word << "(" << tBCD4.BCD.upper1 << "," << tBCD4.BCD.lower1 << "," << tBCD4.BCD.upper2 << "," << tBCD4.BCD.lower2 << ")" << LOG_ENDL;

    return m_pPacket->RequestFileNameStatus(&tCmd);
}

int CompAudioManager::NextFolderName(unsigned int nFolderNo, unsigned int nBlockNo)
{
    Q_UNUSED(nBlockNo);
    BCD4                tBCD4;
    BCD2                tBCD2;
    FolderNameCommand   tCmd;

    tCmd.nUndefined1    = 0;
    tCmd.nByteValue     = 1;
    tCmd.nUndefined2    = 0;

    // FOLDER NUMBER
    tBCD4.word          = DEC2BCD4(nFolderNo);
    tBCD2.BCD.upper     = tBCD4.BCD.upper2;
    tBCD2.BCD.lower     = tBCD4.BCD.lower2;
    tCmd.nFolderNoUpper = tBCD4.BCD.lower1;
    tCmd.nFolderNoLower = tBCD2.byte;
    
    tCmd.nBlockNoByteLength = DS_FOLDER_NAME_CHAR_LENGTH;

    MC_LOG << "CompAudioManager::NextFolderName(" << nFolderNo << "-" << tBCD4.word << "(" << tBCD4.BCD.upper1 << "," << tBCD4.BCD.lower1 << "," << tBCD4.BCD.upper2 << "," << tBCD4.BCD.lower2 << ")" << LOG_ENDL;

    return m_pPacket->RequestFolderNameStatus(&tCmd);
}

// { added by cychoi - To avoid MP3 list read error.
#if _DECK_DIAGNOSTIC_
bool CompAudioManager::ExtractResetStart(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_RESET_START) return false;

    MC_HIGH << "CompAudioManager::ExtractResetStart: m_nRetryPlaybackMediaState=" << m_nRetryPlaybackMediaState << LOG_ENDL;

// { modified by cychoi 2014.04.14 for HW Event PQ launch DVD mode unconditionally after Deck SW reset
#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
    //m_nDiscDetectRetryCount = 0;
    //m_nDiscPlayRetryCount = 0;
#endif
// } modified by cychoi 2014.04.14

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    // { modified by cychoi 2013.08.09 for ITS 181303 Retry on Search Mode
    if(m_nRetryPlaybackMediaState == eMP_STATE_SEARCHING)
    {
        m_nRetryPlaybackMediaState = eMP_STATE_PLAYING;
    }
    // } modified by cychoi 2013.08.09
    StartRetryPlaybackTimer(500);
#endif
// } added by cychoi 2013-06-05

    // { modified by cychoi 2013.09.06 for ITS 178797
    MC_HIGH << "CompAudioManager::ExtractResetStart: m_eDirTreeMode=" << m_eDirTreeMode << LOG_ENDL;
    if(m_eDirTreeMode == RDTM_NONE || m_eDirTreeMode == RDTM_COMPLETE)
    {
        if(m_eDirTreeMode == RDTM_COMPLETE)
        {
            // The below signals should be sent to AVP on Deck Reset. If AVP receive this signals, AVP should retry DirTree Request.
            StartRetryDirTreeTimer(m_nRetryDirTreeFolderNo, m_nRetryDirTreeFileNo, m_nRetryDirTreeBlockNo);
            //emit OnDirTreeReceived(&m_tDvdcDirTree);
            //emit OnDirTreeCompleted();
        }
    }
    else
    {
        StartRetryDirTreeTimer(m_nRetryDirTreeFolderNo, m_nRetryDirTreeFileNo, m_nRetryDirTreeBlockNo);
    }
    // } modified by cychoi 2013.09.06

    // { added by cychoi 2013.11.25 for ITS 210845
    if(m_eRequestFileFolderTotalNum == CA_DECK_REQ_SENT)
    {
        StartRetryFileFolderTotalNumTimer(400); // delay 400ms
    }
    // } added by cychoi 2013.11.25

    // { modified by cychoi 2013.09.06 for ITS 178797
    if(m_eAppRequestAllTagInfo == CA_DECK_REQ_SENT)
    {
        StartRetryTagInfoTimer(600); // delay 100ms + Playback retry delay 500ms
    }
    // } modified by cychoi 2013.09.06

    return true;
}
#endif
// } added by cychoi

bool CompAudioManager::ExtractCompAudioStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_COMP_AUDIO_STATUS) return false;

    // MC_LOG << "CompAudioManager::ExtractCompAudioStatus" << LOG_ENDL;

    CompAudioStatus *pStatus       = &pPacket->m_tData.m_tCompAudio;
    int              bChangedFFRew = false; //added by jeeeun for SANITY_CM_AG079 

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    InitRetryPlayback();
#endif
// } added by cychoi 2013-06-05
#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
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
#ifdef CA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackMediaState = m_ePlayerState;
#endif
        return true;
    }
    // } added by cychoi 2013.10.06
    // } modified by cychoi 2013.12.26

    ////////////////////////////////////////////////////////
    // 상태 변경 알림.
    int                 bStartTime  = false;
    DECK_CD_PLAY_COND   ePlayStatus = (DECK_CD_PLAY_COND)pStatus->nPlayStatus;
    if(ePlayStatus != m_tStatus.m_nPlayStatus) {
        MEDIA_PLAYER_STATE ePlayerState = (MEDIA_PLAYER_STATE)DeckManager::GetInstance()->PlayCondToPlayerState(ePlayStatus);

        if(ePlayerState != m_ePlayerState) {
            if(m_ePlayerState != eMP_STATE_PLAYING && ePlayerState == eMP_STATE_PLAYING) bStartTime = true;
            m_ePlayerState = ePlayerState;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CA_DECK_REQ_RETRY_ENABLED
            m_nRetryPlaybackMediaState = m_ePlayerState;
#endif
// } modified by cychoi 2013.06.20
            if(m_tUserReqTrackList.count() > 0 && ePlayerState == eMP_STATE_PLAYING) {
                int nNewTrack = m_tUserReqTrackList.takeLast();
                m_tUserReqTrackList.clear();
                // { modified by cychoi 20103.08.23 for ITS 185414 ID3 Tag Display
                if(m_tStatus.m_nFileNo == (nNewTrack + 1))
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
                            MC_HIGH << "CompAudioManager::ExtractCompAudioStatus: emit OnRealSoundChanged() [1]" << LOG_ENDL;
                            emit OnRealSoundChanged();
                            m_bAutoTrackChanged = true;
                        }
                        // } added by cychoi 20103.06.13
                    }
                    // } modified by cychoi 2013.10.21
                }
                else
                {
                    MC_LOG << "CompAudioManager::ExtractCompAudioStatus(): nCurrTrack: " << GetCurrentIndex() << ", nNewTrack: " << nNewTrack << LOG_ENDL;
                    SetCurrentIndex(nNewTrack);
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
                    // } modified by cychoi 2013.10.21
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
                    MC_HIGH << "CompAudioManager::ExtractCompAudioStatus: emit OnRealSoundChanged() [2]" << LOG_ENDL;
                    emit OnRealSoundChanged();
                    m_bAutoTrackChanged = true;
                }
                // } added by cychoi 20103.06.13
            }

            m_bFFREWStateSigSend = (m_ePlayerState == eMP_STATE_FAST_PLAYING); // added by cychoi 2013.10.27 for ITS 198856 
            // { modified by cychoi 2013.11.15 for ITS 205008
            if(m_ePlayerState != eMP_STATE_STOPPED)
            {
                if(m_bDelayedModeSet == true)
                {
                    if(m_nDelayedRandomMode != -1)
                    {
                        m_nRandomMode = -1;
                        RandomPlay_Impl(m_nDelayedRandomMode, false);
                        m_nDelayedRandomMode = -1;
                    }
                    
                    if(m_nDelayedRepeatMode != -1)
                    {
                        m_nRepeatMode = -1;
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
           m_nFFREWRate != -1) { //modified by wonseok.heo 2013.07.07
        //if(eDM_CD_PLAY_COND_PLAYING_FAST == m_tStatus.m_nPlayStatus && 
        //   eDM_CD_PLAY_COND_PLAYING_FAST != ePlayStatus) {
        // } added by cychoi 20103.06.17
            // { modified by cychoi 2013.11.11 for SmokeTest DISC cancel REW on reach first track
            bool bOnNormalPlaySend = false;
            if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFOLDER)
            {
                int nFileIdx  = 0;
                int nFileSize = 0;
            
// { modified by cychoi 2013.12.27 for ITS 217395
#if USE_FILE_INDEX_IN_FOLDER
                GetCurrentFileFolderIndex(&nFileIdx, &nFileSize);
#else
                GetMoveFileFolderIndex(m_tStatus.m_nFileNo - 1, &nFileIdx, &nFileSize);
#endif
// } modified by cychoi 2013.12.27
                if(nFileIdx == 0) 
                {
                    bOnNormalPlaySend = true;
                }
            }
            else
            {
                if(m_tStatus.m_nFileNo == 1)
                {
                    bOnNormalPlaySend = true;
                }
            }
            if(bOnNormalPlaySend == true)
            {
                m_nFFREWRate = -1;
                m_nFFREWState = PLAYER_PLAY; // added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
                m_bFFREWStateSigSend = false; // added by cychoi 2013.10.27 for ITS 198856 
    			bChangedFFRew = true; //added by jeeeun for SANITY_CM_AG079  
                emit OnNormalPlay(); // added by Michael.Kim 2013.05.23 additional signal for FF/Rew functionality.
            }
            // } modified by cychoi 2013.11.11
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
                MC_HIGH << "CompAudioManager::ExtractCompAudioStatus: emit OnRealSoundChanged() [3]" << LOG_ENDL;
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

    m_tStatus.m_nPlayStatus         = ePlayStatus;
    m_tStatus.m_nType               = (DECK_COMP_AUDIO_FILE_TYPE)pStatus->nType;
    m_tStatus.m_nTagExist           = (DECK_COMP_AUDIO_TAG_EXIST)pStatus->nTag;
    m_tStatus.m_nDiscNo             = pStatus->nDiscNo;
    m_tStatus.m_nCurrFolderNo       = BCD2DEC2(pStatus->nCurrFolderNoUpper) * 100 + BCD2DEC2(pStatus->nCurrFolderNoLower);
    m_nCurrFolderNo                 = m_tStatus.m_nCurrFolderNo; // added by cychoi 2013.11.17 for ITS 208698 MP3 Play Info on Cluster 
    
    m_tStatus.m_nMPegVer            = (DECK_COMP_AUDIO_MPEG_VER)pStatus->nVer;
    m_tStatus.m_nMPegLayer          = (DECK_COMP_AUDIO_MPEG_LAYER)pStatus->nLayer;
    m_tStatus.m_bNoErrorSecurity    = pStatus->nErrorSecurity;
    m_tStatus.m_nBitrate            = pStatus->nBitRate;
    m_tStatus.m_nSampling           = pStatus->nSampling;
    m_tStatus.m_nExtension          = pStatus->nExtension;
    m_tStatus.m_nChannel            = (DECK_COMP_CHANNEL_MODE)pStatus->nChannelMode;
    m_tStatus.m_nModeExtension      = pStatus->nModeExtention;
    m_tStatus.m_bCopyRight          = pStatus->nCopyRight;
    m_tStatus.m_bOriginal           = pStatus->nOriginal;
    m_tStatus.m_nEmphasis           = pStatus->nEmphasis;
    m_tStatus.m_bUnsupported        = pStatus->nUnsupported;
    m_tStatus.m_nDirectStopMode     = (DECK_DIRECT_STOP_MODE)pStatus->nDirectStopMode;
    m_tStatus.m_nAudioOutput        = (DECK_CD_AUDIO_OUTPUT_MODE)pStatus->nAudioOutputMode;

    memcpy(m_tStatus.m_pFileName, pStatus->pFileName, 32);
    m_tStatus.m_bUnicode            = pStatus->nUnicode;

    // 화면 상태 변경 알림.
    DECK_DISPLAY_SCREEN_MODE eDisplay  = (DECK_DISPLAY_SCREEN_MODE)pStatus->nDisplayCondition;
    if(m_tStatus.m_nDisplayScreen != eDisplay) {
        m_tStatus.m_nDisplayScreen = eDisplay;
        emit OnScreenStateChanged(GetCurrentScreenState());
    }

    ////////////////////////////////////////////////////////
    // 트랙 변경 알림.
    if(pStatus->nFileNoUpper != 0xFF && pStatus->nFileNoLower != 0xFF) {
        int nTrackNo = BCD2DEC2(pStatus->nFileNoUpper) * 100 + BCD2DEC2(pStatus->nFileNoLower);
        // { modified by cychoi 2013.11.14 for ITS 209100 Skip Track Changed on Search Mode
        //if(ePlayStatus == eDM_CD_PLAY_COND_SEARCHING_IN ||
        //   ePlayStatus == eDM_CD_PLAY_COND_SEARCHING_OUT)
        //{
        //    if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFILE ||
        //       GetRandomMode(m_tStatus.m_nRandomMode) != RANDOMOFF)
        //    {
        //        // Track is not valid on Search Mode, please keep current Track.
        //        nTrackNo = m_tStatus.m_nFileNo;
        //    }
        //}
        // } modified by cychoi 2013.11.14
        if(m_tStatus.m_nFileNo != nTrackNo || m_bControllerChanged == true) {
            bool bTrackChanged = (bool)(m_tStatus.m_nFileNo != nTrackNo); // added by cychoi 2014.04.29 for ITS 233500
            //{ added by yongkyun.lee 20130424 for : ITS 147594/ 157595 not random when ff
            //if(m_nUserRandomMode >= 0 && m_tStatus.m_nPlayStatus != eDM_CD_PLAY_COND_PLAYING_FAST ) {
            if(m_nUserRandomMode >= 0 && m_tStatus.m_nPlayStatus != eDM_CD_PLAY_COND_PLAYING_FAST && !bChangedFFRew) 
            {
            //} added by yongkyun.lee 20130424 
// { added by cychoi 2013-07-11 for ITS 179174 TunedPlay & ITS 179168 FF/REW on Random mode
                if(bTrackChanged == true && // modified by cychoi 2014.08.29 for ITS 206865 CDDA Random Mode Issue
                   (m_ePlayDirection == CA_PLAY_NEXT || m_ePlayDirection == CA_PLAY_PREV))
                {
                    RandomNotice(m_nUserRandomMode);
                    m_nUserRandomMode = -2;
                }
                else
                {
                    EmitTrackChange(nTrackNo, bTrackChanged, bChangedFFRew); // modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050
                }
                if(m_nFFREWRate == -1)
                {
                    m_ePlayDirection = CA_PLAY_NEXT;
                }
// } added by cychoi 2013-07-11
            }
            else {
                EmitTrackChange(nTrackNo, bTrackChanged, bChangedFFRew); // modified by cychoi 2014.08.29 for ITS 247048, ITS 247049, ITS 247050
            }
        }
        else {
            if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFILE && m_tStatus.m_nPlayBackTime == 0)
            {
                MC_LOG << "CompAudioManager::nTrackNo2: " << m_tStatus.m_nFileNo - 1 << LOG_ENDL;
                // { added by cychoi 20103.07.22 for add new signal
                if(m_bUserTrackChanged == true)
                {
                    emit OnUserTrackChanged(m_tStatus.m_nFileNo - 1);
                    m_bUserTrackChanged = false;
                }
                // } added by cychoi 20103.07.22
                // { added by cychoi 2014.04.06 for ITS 233500
                else
                {
                    emit OnAutoTrackChanged(m_tStatus.m_nFileNo - 1);
                }
                // } added by cychoi 2014.04.06
                emit OnTrackChanged(m_tStatus.m_nFileNo - 1);
                // removed by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
            }
        }
        // MC_LOG << "CompAudioManager::ExtractCompAudioStatus: m_nFileNo is " << nTrackNo << LOG_ENDL;
    }
    else {
        // MC_LOG << "CompAudioManager::ExtractCompAudioStatus: m_nFileNo is searching..." << LOG_ENDL;
    }

    ////////////////////////////////////////////////////////
    // 재생 시간 변경 알림.    
    if(pStatus->nPlaybackTimeMin10 != 0xF && pStatus->nPlaybackTimeMin1 != 0xF) 
    {
        int nHour = BCD2DEC1(pStatus->nPlaybackTimeHour10) * 10 + BCD2DEC1(pStatus->nPlaybackTimeHour1);
        int nMin  = BCD2DEC1(pStatus->nPlaybackTimeMin10)  * 10 + BCD2DEC1(pStatus->nPlaybackTimeMin1);
        int nSec  = BCD2DEC1(pStatus->nPlaybackTimeSec10)  * 10 + BCD2DEC1(pStatus->nPlaybackTimeSec1);
        int nTime = (nHour * 3600 + nMin * 60 + nSec) * 1000;
    
        if(m_tStatus.m_nPlayBackTime != nTime) {
            if(bStartTime == true) DeckManager::GetInstance()->SetStartPlayTime(nTime);
            DeckManager::GetInstance()->SetLifeTime(nTime);
            m_tStatus.m_nPlayBackTime = nTime;
            // MC_LOG << "CompAudioManager::P: " << m_tStatus.m_nPlayStatus << ", Time: " << nTime << LOG_ENDL;
            emit OnPositionChanged(nTime);
        }
    }

    ////////////////////////////////////////////////////////
    // 모드 변경 알림.
    // MC_LOG << "CompAudioManager::m_tStatus.m_nRandomMode: " << m_tStatus.m_nRandomMode << "-" << pStatus->nRandomMode << LOG_ENDL;
    // MC_LOG << "CompAudioManager::m_tStatus.m_nRepeatMode: " << m_tStatus.m_nRepeatMode << "-" << pStatus->nRepeatMode << LOG_ENDL;
    // MC_LOG << "CompAudioManager::m_tStatus.m_nScanMode :  " << m_tStatus.m_nScanMode << "-" << pStatus->nScanMode << LOG_ENDL;
    DECK_COMP_RANDOM_MODE eRandomMode = (DECK_COMP_RANDOM_MODE)pStatus->nRandomMode;
    DECK_COMP_REPEAT_MODE eRepeatMode = (DECK_COMP_REPEAT_MODE)pStatus->nRepeatMode;
    DECK_COMP_SCAN_MODE   eScanMode   = (DECK_COMP_SCAN_MODE)pStatus->nScanMode;
    //{ modified by cychoi 20130.07.11 for keeping playback options
    if(m_tStatus.m_nRandomMode != eRandomMode && m_nUserRandomMode == -1) {
        m_tStatus.m_nRandomMode = eRandomMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackRandomMode = m_tStatus.m_nRandomMode;
#endif
// } modified by cychoi 2013.06.20

        // { modified by cychoi 2013.11.15 for ITS 205008
        // { added by wonseok.heo for ITS 205008  2013.11.01
        //if(eRandomMode != eDM_COMP_RANDOM_OFF){
        //    m_nRandomMode = -1;
        //    int nRandomMode = GetRandomMode(m_tStatus.m_nRandomMode);
        //    RandomPlay(RANDOMOFF);

        //    if(nRandomMode == m_nRandomMode )
        //        emit OnModeChanged(m_nRandomMode);
        //}else{
            emit OnModeChanged(m_nRandomMode);
        //}
        // } added by wonseok.heo for ITS 205008  2013.11.01
        // } modified by cychoi 2013.11.15
    }  
    if(m_tStatus.m_nRepeatMode != eRepeatMode) {
        m_tStatus.m_nRepeatMode = eRepeatMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackRepeatMode = m_tStatus.m_nRepeatMode;
#endif
// } modified by cychoi 2013.06.20
        //{ modified by yongkyun.lee 2013-08-25 for : folder repeat -> list another folder song select -> Repeat all icon update
        // { added by wonseok.heo 2013.08.30 NO CR
        if(m_nRepeatMode == REPEATFOLDER && GetRepeatMode(eRepeatMode) == REPEATALL &&
           GetRandomMode(eRandomMode) == RANDOMOFF && GetScanMode(eScanMode) == SCANOFF){ //modified by wonseok.heo for smokeTest scan issue 2013.09.06
            int nRepeatMode = GetRepeatMode(eRepeatMode);
            m_nRepeatMode = -1;
            RepeatPlay(REPEATFOLDER);
            if(nRepeatMode == m_nRepeatMode){
                emit OnModeChanged(m_nRepeatMode);
            }
        }
        else{
            //{ added by wonseok.heo 2013.10.03 for smoke test random repeat
            // { modified by wonseok.heo for smokeTest scan issue 2013.09.06
            if( ( m_nRepeatMode == REPEATFOLDER && GetRepeatMode(eRepeatMode) == REPEATALL && m_scanStatus == true) ||
                    (m_nRepeatMode == REPEATFILE && GetRepeatMode(eRepeatMode) == REPEATALL)){
                // } added by wonseok.heo 2013.10.03 for smoke test random repeat
                m_nRepeatMode = GetRepeatMode(eRepeatMode);
            }
            // } modified by wonseok.heo for smokeTest scan issue 2013.09.06
        //} modified by yongkyun.lee 2013-08-25 
        emit OnModeChanged(m_nRepeatMode);
        }
        // } added by wonseok.heo 2013.08.30 NO CR
    }
    if(m_tStatus.m_nScanMode != eScanMode) {
        if((m_tStatus.m_nScanMode != eDM_COMP_SCAN_OFF && m_tStatus.m_nScanMode != eDM_COMP_SCAN_NONE) && eScanMode == eDM_COMP_SCAN_OFF) {
            emit OnScanCompleted();
        }
        m_tStatus.m_nScanMode = eScanMode;
// { modified by cychoi 2013.06.20 for SW reset on disc loading
#ifdef CA_DECK_REQ_RETRY_ENABLED
        m_nRetryPlaybackScanMode = m_tStatus.m_nScanMode;
#endif
// } modified by cychoi 2013.06.20
        emit OnModeChanged(GetScanMode(m_tStatus.m_nScanMode));
    }

    if(m_nUserRandomMode == -2) m_nUserRandomMode = -1;
    //} modified by cychoi 20130.07.11

// { added by cychoi 2013-07-13 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    switch(m_ePlayerState)
    {
    case eMP_STATE_PLAYING:
    case eMP_STATE_SEARCHING:
    case eMP_STATE_FAST_PLAYING: 
        StartRetryPlaybackTimer(7500); // Compression Audio Long Access 5s MAX (temperature: +5 ~ +35) 7s MAX (temperature: -20 ~ +80)
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

int CompAudioManager::ExtractDirTreeStatus(StatusData *pPacket)
{
    int         bRequestComplete = false;
    int         nCurrFolderNo;
    int         nCurrBlockNo;
    
    if(pPacket->m_nCode != DS_CODE_DIR_TREE_STATUS) return 0;

    DirTreeStatus *pStatus = &pPacket->m_tData.m_tDirTree;

    nCurrFolderNo = BCD2DEC1(pStatus->nCurrFolderNo100) * 100 + BCD2DEC2(pStatus->nCurrFolderNo10_1);
    nCurrBlockNo  = BCD2DEC2(pStatus->nBlockNo);

    if(m_eDirTreeMode == RDTM_DIR_TREE) 
    {
        m_tDvdcDirTree.valid             = pStatus->nValid;
        m_tDvdcDirTree.curr_folder       = nCurrFolderNo;
        m_tDvdcDirTree.block_no          = nCurrBlockNo;
        m_tDvdcDirTree.unicode           = pStatus->nUnicode;
        m_tDvdcDirTree.chld_folder_num   = BCD2DEC1(pStatus->nChildFolderNo100) * 100 + BCD2DEC2(pStatus->nChildFolderNo10_1);
        m_tDvdcDirTree.chld_file_num     = BCD2DEC1(pStatus->nChildFileNo100)   * 100 + BCD2DEC2(pStatus->nChildFileNo10_1);

        int nStartFileNo = -1;
        for(int nIdx = 0; nIdx < 10; nIdx++)
        {
            DirTreeFileFolderInfo *pInfo = (DirTreeFileFolderInfo*)pStatus->pFileFolderInfo[nIdx];
            
            m_tDvdcDirTree.entry[nIdx]->type                  = pInfo->nType;
            m_tDvdcDirTree.entry[nIdx]->playing_now_or_next   = pInfo->nPlayFile;
            m_tDvdcDirTree.entry[nIdx]->longname              = pInfo->nLength;
            m_tDvdcDirTree.entry[nIdx]->file_folder_num       = BCD2DEC1(pInfo->nFileFolderNo100) * 100 + BCD2DEC2(pInfo->nFileFolderNo10_1);

            if((pInfo->nType == eDM_FF_TYPE_MP3_FILE || pInfo->nType == eDM_FF_TYPE_WMA_FILE) && nStartFileNo == -1) {
                nStartFileNo = m_tDvdcDirTree.entry[nIdx]->file_folder_num;
            }
        }

        if(m_tFileSizeInFolder.contains(nCurrFolderNo) == false && nStartFileNo >= 0) {
            FILE_SIZE_INFO tInfo;
            tInfo.m_nStartNum = nStartFileNo;
            tInfo.m_nFileSize = m_tDvdcDirTree.chld_file_num;
            m_tFileSizeInFolder.insert(nCurrFolderNo, tInfo);
            // { added by cychoi 2013.11.17 for ITS 208698 MP3 Play Info on Cluster 
            if(nCurrFolderNo == m_tStatus.m_nCurrFolderNo)
            {
                if(m_tStatus.m_nFileNo > 0)
                {
                    emit OnTrackUpdated(m_tStatus.m_nFileNo - 1);
                    m_nCurrFolderNo = NULL32;
                }
            }
            // } added by cychoi 2013.11.17
        }

        m_nMaxChildNum = (m_tDvdcDirTree.chld_folder_num + m_tDvdcDirTree.chld_file_num) - nCurrBlockNo * 10;
        if(m_nMaxChildNum > 10)     m_nMaxChildNum = 10;
        else if(m_nMaxChildNum < 0) m_nMaxChildNum = 0;

        if(nCurrBlockNo == 0) {
            InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
            MC_LOG << "CompAudioManager::ExtractDirTreeStatus: NextFolderName: m_nChildIdx: " << m_nChildIdx << "(" << nCurrFolderNo << ")" << LOG_ENDL;
            if(NextFolderName(nCurrFolderNo, 0) > 0) { // 현재 폴더 명칭.
                StartRetryDirTreeTimer(nCurrFolderNo, 0, nCurrBlockNo); // added by cychoi - To avoid MP3 list read error.
                m_eDirTreeMode = RDTM_CURR_FOLDER_NAME;
            }else{
                bRequestComplete = true;
            }
        }
        else if(m_nChildIdx < m_nMaxChildNum) {
            if(m_tDvdcDirTree.entry[m_nChildIdx]->type == eDM_FF_TYPE_FOLER) { // 자식 폴더 명칭.
                InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
                MC_LOG << "CompAudioManager::ExtractDirTreeStatus: NextFolderName: m_nChildIdx: " << m_nChildIdx << "(" << m_tDvdcDirTree.entry[m_nChildIdx]->file_folder_num << ")" << LOG_ENDL;
                if(NextFolderName(m_tDvdcDirTree.entry[m_nChildIdx]->file_folder_num, 0) > 0) {
                    StartRetryDirTreeTimer(m_tDvdcDirTree.entry[m_nChildIdx]->file_folder_num, 0, nCurrBlockNo); // added by cychoi - To avoid MP3 list read error.
                    m_eDirTreeMode = RDTM_CHILD_FOLDER_NAME;
                }else{
                    bRequestComplete = true;
                }
            }
            else {  // 자식 파일 명칭.
                InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
                MC_LOG << "CompAudioManager::ExtractDirTreeStatus: NextFileName: m_nChildIdx: " << m_nChildIdx << "(" << m_tDvdcDirTree.entry[m_nChildIdx]->file_folder_num << ")" << LOG_ENDL;
                if(NextFileName(nCurrFolderNo, m_tDvdcDirTree.entry[m_nChildIdx]->file_folder_num, 0) > 0) {
                    StartRetryDirTreeTimer(nCurrFolderNo, m_tDvdcDirTree.entry[m_nChildIdx]->file_folder_num, nCurrBlockNo); // added by cychoi - To avoid MP3 list read error.
                    m_eDirTreeMode = RDTM_CHILD_FILE_NAME;
                }else{
                    bRequestComplete = true;
                }
            }
        }
        else {
            // 빈 폴더.
            InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
            bRequestComplete = true;
        }

        if(bRequestComplete == true) {
            m_eDirTreeMode = RDTM_COMPLETE;
            emit OnDirTreeReceived(&m_tDvdcDirTree);
            emit OnDirTreeCompleted();
            MC_LOG << "CompAudioManager::OnDirTreeCompleted()" << LOG_ENDL;
        }

        MC_LOG << "CompAudioManager::ExtractDirTreeStatus(): m_nMaxChildNum: " << m_nMaxChildNum << LOG_ENDL;
        
        return 1;
    }
    else 
    {        
        return 1;
    }   
}

bool CompAudioManager::ExtractTagStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_TAG_STATUS) return false;

    MC_LOG << "CompAudioManager::ExtractTagStatus" << LOG_ENDL;

    TagStatus *pStatus = &pPacket->m_tData.m_tTag;

    m_tTagInfo.valid         = pStatus->nValid;
    m_tTagInfo.exist         = pStatus->nExist;
    m_tTagInfo.unicode_info  = pStatus->nUnicodeInfo;
    m_tTagInfo.genre         = pStatus->nGenre;

    memcpy(m_tTagInfo.title,        pStatus->pTitle,   sizeof(char)*30);
    memcpy(m_tTagInfo.artist,       pStatus->pArtist,  sizeof(char)*30);
    memcpy(m_tTagInfo.album,        pStatus->pAlbum,   sizeof(char)*30);
    memcpy(m_tTagInfo.year,         pStatus->pYear,    sizeof(char)*4);
    memcpy(m_tTagInfo.comment,      pStatus->pComment, sizeof(char)*30);

    m_eAppRequestTagInfo = m_tTagInfo.valid ? CA_DECK_REQ_DONE : CA_DECK_REQ_NONE; // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error // modified by cychoi 2013.09.06 for ITS 178797
    emit OnTagReceived(&m_tTagInfo);
    
    return true;
}

bool CompAudioManager::ExtractTag2Status(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_TAG_STATUS2) return false;

    TagStatus2 *pStatus = &pPacket->m_tData.m_tTag2;
    int         nLen;
    char        pString[66] = {0, }; // modified by cychoi 2013-07-18 for ITS 180453 UTF-16(LITTLE Endian) code value error. 2 bytes for Null chars

// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    InitRetryTagInfo();
#endif
// } added by cychoi 2013-06-05

    if(m_tAllTagInfo.out_info_type >= 0) 
    {
        nLen = pStatus->nTagInfoSize;
        if(nLen > 64)     nLen = 64;
        else if(nLen < 0) nLen = 0;
        
        switch(pStatus->nOutputInfoType) 
        {
        case 0: // TITLE
            MC_LOG << "CompAudioManager::ExtractTag2Status() Title: " << pStatus->nValid << ", " << pStatus->nExist << ", " << pStatus->nTagInfoSize << ", " << pStatus->nUnicodeTextInfo << LOG_ENDL;
            m_tAllTagInfo.valid = 1; // added by cychoi 2013.10.03 for SmokeTest Infinite ID3 Tag Req error 
            // { added by cychoi 2013.10.03 for SmokeTest Infinite ID3 Tag Req error 
            if(pStatus->nValid == 0) {
                m_tAllTagInfo.title = QString();
                m_tAllTagInfo.valid = 0;
            }
            // } added by cychoi 2013.10.03
            else if(pStatus->nExist == 0 || pStatus->nTagInfoSize <= 0) {
                m_tAllTagInfo.title = QString();
            }
            else {
                memcpy(pString, pStatus->pTagInfo, sizeof(char)*nLen);
                m_tAllTagInfo.title = DeckStringToUnicode(pStatus->nUnicodeTextInfo, pString, nLen);
                m_tAllTagInfo.exist  = 1;
            }
            m_tAllTagInfo.out_info_type = 1;
            break;
        case 1: // ARTIST
            MC_LOG << "CompAudioManager::ExtractTag2Status() Artist: " << pStatus->nValid << ", " << pStatus->nExist << ", " << pStatus->nTagInfoSize << ", " << pStatus->nUnicodeTextInfo << LOG_ENDL;
            // { added by cychoi 2013.10.03 for SmokeTest Infinite ID3 Tag Req error 
            if(pStatus->nValid == 0) {
                m_tAllTagInfo.artist = QString();
                m_tAllTagInfo.valid = 0;
            }
            // } added by cychoi 2013.10.03
            else if(pStatus->nExist == 0 || pStatus->nTagInfoSize <= 0) {
                m_tAllTagInfo.artist = QString();
            }
            else {
                memcpy(pString, pStatus->pTagInfo, sizeof(char)*nLen);
                m_tAllTagInfo.artist = DeckStringToUnicode(pStatus->nUnicodeTextInfo, pString, nLen);
                m_tAllTagInfo.exist  = 1;
            }
            m_tAllTagInfo.out_info_type = 2;
            break;
        case 2: // ALBUM
            MC_LOG << "CompAudioManager::ExtractTag2Status() Album: " << pStatus->nValid << ", " << pStatus->nExist << ", " << pStatus->nTagInfoSize << ", " << pStatus->nUnicodeTextInfo << LOG_ENDL;
            // { added by cychoi 2013.10.03 for SmokeTest Infinite ID3 Tag Req error 
            if(pStatus->nValid == 0) {
                m_tAllTagInfo.album = QString();
                m_tAllTagInfo.valid = 0;
            }
            // } added by cychoi 2013.10.03
            else if(pStatus->nExist == 0 || pStatus->nTagInfoSize <= 0) {
                m_tAllTagInfo.album = QString();
            }
            else {
                memcpy(pString, pStatus->pTagInfo, sizeof(char)*nLen);
                m_tAllTagInfo.album = DeckStringToUnicode(pStatus->nUnicodeTextInfo, pString, nLen);
                m_tAllTagInfo.exist = 1;
            }
            m_tAllTagInfo.out_info_type = 5;
            break;
        case 5: // GENRE
            // { added by cychoi 2013.10.03 for SmokeTest Infinite ID3 Tag Req error 
            if(pStatus->nValid == 0) {
                m_tAllTagInfo.genre = QString();
                m_tAllTagInfo.valid = 0;
            }
            // } added by cychoi 2013.10.03
            else if(pStatus->nExist == 0 || pStatus->nTagInfoSize <= 0) {
                m_tAllTagInfo.genre = QString();
            }
            else {
                memcpy(pString, pStatus->pTagInfo, sizeof(char)*nLen);
                m_tAllTagInfo.genre = DeckStringToUnicode(pStatus->nUnicodeTextInfo, pString, nLen);
                m_tAllTagInfo.exist = 1;
            }
            MC_LOG << "CompAudioManager::ExtractTag2Status() Genre: " << pStatus->nValid << ", " << pStatus->nExist << ", " << pStatus->nTagInfoSize << ", " << pStatus->nUnicodeTextInfo << LOG_ENDL;
            m_tAllTagInfo.out_info_type = -1;
            break;
        default:
            break;
        }

        if(m_tAllTagInfo.out_info_type > 0) {
            // { modified by cychoi 2013.09.06 for ITS 178797
            if(m_pPacket->RequestTagStatus2(m_tAllTagInfo.out_info_type) > 0)
            {
                m_eAppRequestAllTagInfo = CA_DECK_REQ_SENT;
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
                StartRetryTagInfoTimer(200);
#endif
// } added by cychoi 2013-06-05
            }
            // } modified by cychoi 2013.09.06
        }
        else if(m_tAllTagInfo.out_info_type < 0) {
            MC_LOG << "CompAudioManager::ExtractTag2Status() emit OnAllTagReceived()" << LOG_ENDL;
            // { modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
            // { modified by cychoi 2013.09.06 for ITS 178797
            if(GetCurrentMediaState() == PLAYER_STOP || m_tAllTagInfo.valid == 0) { // modified by cychoi 2013.10.03 for SmokeTest Infinite ID3 Tag Req error 
                m_eAppRequestAllTagInfo        = CA_DECK_REQ_NONE;
                m_tAllTagInfo.out_info_type    = -1;
                m_tAllTagInfo.exist            = 0;
                m_tAllTagInfo.valid            = 0;
            }else{
                m_eAppRequestAllTagInfo        = CA_DECK_REQ_DONE;
                emit OnAllTagReceived(&m_tAllTagInfo);
            }
            // } modified by cychoi 2013.09.06
            // } modified by cychoi 2013.09.22
        }
    }
    else
    {
        MC_LOG << "CompAudioManager::ExtractTag2Status()" << LOG_ENDL;
        
        m_tTag2Info.valid           = pStatus->nValid;
        m_tTag2Info.exist           = pStatus->nExist;
        m_tTag2Info.unicode_text    = pStatus->nUnicodeTextInfo;
        m_tTag2Info.id3v2           = pStatus->nID3v2;
        m_tTag2Info.id3v1           = pStatus->nID3v1;
        m_tTag2Info.out_info_type   = pStatus->nOutputInfoType;
        m_tTag2Info.tag_info_size   = pStatus->nTagInfoSize;

        memcpy(m_tTag2Info.tag_info, pStatus->pTagInfo, sizeof(char)*64);

        m_eAppRequestTag2Info = m_tTag2Info.valid ? CA_DECK_REQ_DONE : CA_DECK_REQ_NONE; // modified by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error // modified by cychoi 2013.09.06 for ITS 178797
        emit OnTag2Received(&m_tTag2Info);
    }
    
    return true;
}

bool CompAudioManager::ExtractFileNameStatus(StatusData *pPacket)
{
    int bRequestComplete            = false;
    int nChildIdx                   = m_nChildIdx;
    int nNextChildIdx               = nChildIdx + 1;
    RequestDirTreeMode eDirTreeMode = m_eDirTreeMode;

    if(pPacket->m_nCode != DS_CODE_FILE_NAME_STATUS) return false;

    FileNameStatus *pStatus = &pPacket->m_tData.m_tFileName;

    MC_LOG << "CompAudioManager::ExtractFileNameStatus: " << pStatus->nContinue << "-" << pStatus->nUnicode << "-" << pStatus->nByteNo << "-" << pStatus->nBlockNoOrByteNo << LOG_ENDL;

    // { modified by cychoi 2013-10-17 for ISV 92215 // modified by cychoi 2013-05-18 for ISV 77101
    if(pStatus->nValid == 0) 
    {
        if(m_nRequestFileNameFolderNo != NULL32 &&
           m_nRequestFileNameFileNo != NULL32) {
            m_tFileName.valid = pStatus->nValid;
            m_nRequestFileNameFolderNo = NULL32;
            m_nRequestFileNameFileNo = NULL32;
            emit OnFileNameReceived(&m_tFileName);
            return true;
        }
    }
    else 
    {
        m_tFileName.valid       = pStatus->nValid;
        m_tFileName.folder_no   = BCD2DEC1(pStatus->nFolderNoUpper) * 100 + BCD2DEC2(pStatus->nFolderNoLower);
        m_tFileName.file_no     = BCD2DEC1(pStatus->nFileNoUpper)   * 100 + BCD2DEC2(pStatus->nFileNoLower);
        m_tFileName.unicode     = pStatus->nUnicode;

        if(m_nRequestFileNameFolderNo == (unsigned int)m_tFileName.folder_no &&
           m_nRequestFileNameFileNo == (unsigned int)m_tFileName.file_no) 
        {
            if(pStatus->nCHRValid == 1) {
                memcpy((void*)m_tFileName.name, pStatus->pCHRCode, sizeof(char)*DS_FILE_NAME_CHAR_LENGTH);
                m_tFileName.name[DS_FILE_NAME_CHAR_LENGTH]   = 0;
                m_tFileName.name[DS_FILE_NAME_CHAR_LENGTH+1] = 0;
            }else{
                m_tFileName.name[0] = 0;
                m_tFileName.name[1] = 0;
            }
            
            m_nRequestFileNameFolderNo = NULL32;
            m_nRequestFileNameFileNo = NULL32;
            emit OnFileNameReceived(&m_tFileName);
            return true;
        }
    }

    if(eDirTreeMode == RDTM_CHILD_FOLDER_NAME || eDirTreeMode == RDTM_CHILD_FILE_NAME)
    {
        if(nNextChildIdx < m_nMaxChildNum) {
            if(m_tDvdcDirTree.entry[nNextChildIdx]->type == eDM_FF_TYPE_FOLER) { // 자식 폴더 명칭.
                InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
                MC_LOG << "CompAudioManager::ExtractFileNameStatus: NextFolderName: nNextChildIdx: " << nNextChildIdx << "(" << m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num << ")" << LOG_ENDL;
                if(NextFolderName(m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num, 0) > 0) {
                    StartRetryDirTreeTimer(m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num, 0, m_tDvdcDirTree.block_no); // added by cychoi - To avoid MP3 list read error.
                    m_eDirTreeMode = RDTM_CHILD_FOLDER_NAME;
                }else{
                    bRequestComplete = true;
                }
            }else{  // 자식 파일 명칭.
                InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
                MC_LOG << "CompAudioManager::ExtractFileNameStatus: NextFileName: nNextChildIdx: " << nNextChildIdx << "(" << m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num << ")" << LOG_ENDL;
                if(NextFileName(m_tDvdcDirTree.curr_folder, m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num, 0) > 0) {
                    StartRetryDirTreeTimer(m_tDvdcDirTree.curr_folder, m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num, m_tDvdcDirTree.block_no); // added by cychoi - To avoid MP3 list read error.
                    m_eDirTreeMode = RDTM_CHILD_FILE_NAME;
                }else{
                    bRequestComplete = true;
                }
            }
        }else{
            InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
            bRequestComplete = true;
        }
    }
    
    if(pStatus->nValid == 0) 
    {
        if(eDirTreeMode == RDTM_CHILD_FILE_NAME) {
            m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[0] = 0;
            m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[1] = 0;
            m_nChildIdx++;
        }
    }
    else 
    {
        if(eDirTreeMode == RDTM_CHILD_FILE_NAME) 
        {
            if(pStatus->nCHRValid == 1) {
                memcpy((void*)m_tDvdcDirTree.entry[nChildIdx]->file_folder_name, pStatus->pCHRCode, sizeof(char)*DS_FILE_NAME_CHAR_LENGTH);
                m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[DS_FILE_NAME_CHAR_LENGTH]   = 0;
                m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[DS_FILE_NAME_CHAR_LENGTH+1] = 0;
                MC_LOG << "CompAudioManager::ExtractFileNameStatus: " << nChildIdx << ": " << qPrintable(QString(m_tDvdcDirTree.entry[nChildIdx]->file_folder_name)) << LOG_ENDL;
            }else{
                m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[0] = 0;
                m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[1] = 0;
            }
            m_nChildIdx++;
        }
        else {
            MC_LOG << "CompAudioManager::ExtractFileNameStatus: ERROR" << LOG_ENDL;
        }
    }
    // } modified by cychoi 2013-10-17 // modified by cychoi 2013-05-18

    if(bRequestComplete == true) {
        m_eDirTreeMode = RDTM_COMPLETE;
        emit OnDirTreeReceived(&m_tDvdcDirTree);
        emit OnDirTreeCompleted();
        MC_LOG << "CompAudioManager::OnDirTreeCompleted()" << LOG_ENDL;
    }
    
    return true;   
}

bool CompAudioManager::ExtractFolderNameStatus(StatusData *pPacket)
{
    int bRequestComplete            = false;
    int nChildIdx                   = m_nChildIdx;
    int nNextChildIdx;
    RequestDirTreeMode eDirTreeMode = m_eDirTreeMode;
    
    if(pPacket->m_nCode != DS_CODE_FOLDER_NAME_STATUS) return false;

    FolderNameStatus *pStatus = &pPacket->m_tData.m_tFolderName;

    MC_LOG << "CompAudioManager::ExtractFolderNameStatus: " << pStatus->nContinue << "-" << pStatus->nUnicode << "-" << pStatus->nByteNo << "-" << pStatus->nBlockNoOrByteNo << LOG_ENDL;

    if(eDirTreeMode == RDTM_CHILD_FOLDER_NAME) {
        nNextChildIdx = m_nChildIdx + 1;
    }else{
        nNextChildIdx = m_nChildIdx;
    }

    // { modified by cychoi 2013-10-17 for ISV 92215 // modified by cychoi 2013-05-18 for ISV 77101
    if(pStatus->nValid == 0) {
        if(m_nRequestFolderNameFolderNo != NULL32) {
            m_tFolderName.valid = pStatus->nValid;
            m_nRequestFolderNameFolderNo = NULL32;
            emit OnFolderNameReceived(&m_tFolderName);
            return true;
        }
    }
    else {
        m_tFolderName.valid     = pStatus->nValid;
        m_tFolderName.folder_no = BCD2DEC1(pStatus->nFolderNoUpper) * 100 + BCD2DEC2(pStatus->nFolderNoLower);
        m_tFolderName.unicode   = pStatus->nUnicode;

        if(m_nRequestFolderNameFolderNo == (unsigned int)m_tFolderName.folder_no) {
            if(pStatus->nCHRValid == 1) {
                memcpy((void*)m_tFolderName.name, pStatus->pCHRCode, sizeof(char) * DS_FOLDER_NAME_CHAR_LENGTH);
                m_tFolderName.name[DS_FOLDER_NAME_CHAR_LENGTH]   = 0;
                m_tFolderName.name[DS_FOLDER_NAME_CHAR_LENGTH+1] = 0;
            }
            else {
                m_tFolderName.name[0] = 0;
                m_tFolderName.name[1] = 0;
            }
            
            m_nRequestFolderNameFolderNo = NULL32;
            emit OnFolderNameReceived(&m_tFolderName);
            return true;
        }
    }

    if(eDirTreeMode == RDTM_CURR_FOLDER_NAME || eDirTreeMode == RDTM_CHILD_FOLDER_NAME)
    {
        if(nNextChildIdx < m_nMaxChildNum) {
            if(m_tDvdcDirTree.entry[nNextChildIdx]->type == eDM_FF_TYPE_FOLER) { // 자식 폴더 명칭.
                InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
                MC_LOG << "CompAudioManager::ExtractFolderNameStatus: NextFolderName: nNextChildIdx: " << nNextChildIdx << "(" << m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num << ")" << LOG_ENDL;
                if(NextFolderName(m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num, 0) > 0) {
                    StartRetryDirTreeTimer(m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num, 0, m_tDvdcDirTree.block_no); // added by cychoi - To avoid MP3 list read error.
                    m_eDirTreeMode = RDTM_CHILD_FOLDER_NAME;
                }else{
                    bRequestComplete = true;
                }
            }else{  // 자식 파일 명칭.
                InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
                MC_LOG << "CompAudioManager::ExtractFolderNameStatus: NextFileName: nNextChildIdx: " << nNextChildIdx << "(" << m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num << ")" << LOG_ENDL;
                if(NextFileName(m_tDvdcDirTree.curr_folder, m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num, 0) > 0) {
                    StartRetryDirTreeTimer(m_tDvdcDirTree.curr_folder, m_tDvdcDirTree.entry[nNextChildIdx]->file_folder_num, m_tDvdcDirTree.block_no); // added by cychoi - To avoid MP3 list read error.
                    m_eDirTreeMode = RDTM_CHILD_FILE_NAME;
                }else{
                    bRequestComplete = true;
                }
            }
        }else{
            InitRetryDirTree(); // added by cychoi - To avoid MP3 list read error.
            bRequestComplete = true;
        }
    }

    if(pStatus->nValid == 0) {
        if(eDirTreeMode == RDTM_CURR_FOLDER_NAME) {
            m_tDvdcDirTree.folder_name[0] = 0;
            m_tDvdcDirTree.folder_name[1] = 0;
        }
        else if(eDirTreeMode == RDTM_CHILD_FOLDER_NAME) {
            m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[0] = 0;
            m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[1] = 0;
            m_nChildIdx++;
        }
    }
    else {
        if(eDirTreeMode == RDTM_CURR_FOLDER_NAME) {
            if(pStatus->nCHRValid == 1) {
                memcpy((void*)m_tDvdcDirTree.folder_name, pStatus->pCHRCode, sizeof(char) * DS_FOLDER_NAME_CHAR_LENGTH);
                m_tDvdcDirTree.folder_name[DS_FOLDER_NAME_CHAR_LENGTH]   = 0;
                m_tDvdcDirTree.folder_name[DS_FOLDER_NAME_CHAR_LENGTH+1] = 0;
            }
            else {
                m_tDvdcDirTree.folder_name[0] = 0;
                m_tDvdcDirTree.folder_name[1] = 0;
            }
        }
        else if(eDirTreeMode == RDTM_CHILD_FOLDER_NAME) {
            if(pStatus->nCHRValid == 1) {
                memcpy((void*)m_tDvdcDirTree.entry[nChildIdx]->file_folder_name, pStatus->pCHRCode, sizeof(char) * DS_FOLDER_NAME_CHAR_LENGTH);
                m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[DS_FOLDER_NAME_CHAR_LENGTH]   = 0;
                m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[DS_FOLDER_NAME_CHAR_LENGTH+1] = 0;
                MC_LOG << "CompAudioManager::ExtractFolderNameStatus: " << nChildIdx << ": " << qPrintable(QString(m_tDvdcDirTree.entry[nChildIdx]->file_folder_name)) << LOG_ENDL;
            }
            else {
                m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[0] = 0;
                m_tDvdcDirTree.entry[nChildIdx]->file_folder_name[1] = 0;
            }
            m_nChildIdx++;
        }
        else {
            MC_LOG << "CompAudioManager::ExtractFolderNameStatus: ERROR" << LOG_ENDL;
        }
    }
    // } modified by cychoi 2013-10-17 for ISV 92215 // modified by cychoi 2013-05-18 for ISV 77101

    if(bRequestComplete == true) {
        m_eDirTreeMode = RDTM_COMPLETE;
        emit OnDirTreeReceived(&m_tDvdcDirTree);
        emit OnDirTreeCompleted();
        MC_LOG << "CompAudioManager::OnDirTreeCompleted()" << LOG_ENDL;
    }

    return true;   
}

bool CompAudioManager::ExtractFolderFile4Status(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_FOLDERFILE4_STATUS) return false;

    MC_LOG << "CompAudioManager::ExtractFolderFile4Status" << LOG_ENDL;

    FolderFile4Status *pStatus = &pPacket->m_tData.m_tFolderFile4;

    if(pStatus->nValid == 0) return true;
    
    return true;   
}

bool CompAudioManager::ExtractFileFolderTotalNumber(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_FILEFONDER_TOTAL_NUMBER) return false;

    FileFolderTotalNum *pStatus = &pPacket->m_tData.m_tFileFolderTotalNum;

    if(pStatus->nValid == 0) {
        m_pPacket->RequestFileFolderTotalNum();
        return true;
    }

// { added by cychoi 2013.11.25 for ITS 210845
#ifdef CA_DECK_REQ_RETRY_ENABLED
    InitRetryFileFolderTotalNum();
#endif
// } added by cychoi 2013.11.25
#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
    m_nDiscDetectRetryCount = 0;
#endif

    m_nTotalFileNum   = BCD2DEC1(pStatus->nTotalFileNo100) * 100   + BCD2DEC2(pStatus->nTotalFileNo10_1);
    m_nTotalFolderNum = BCD2DEC1(pStatus->nTotalFolderNo100) * 100 + BCD2DEC2(pStatus->nTotalFolderNo10_1);

    MC_LOG << "CompAudioManager::ExtractFileFolderTotalNumber: " << m_nTotalFolderNum << ", " << m_nTotalFileNum << LOG_ENDL;

// { added by cychoi 2013.11.25 for ITS 210845
#ifdef CA_DECK_REQ_RETRY_ENABLED
    m_eRequestFileFolderTotalNum = CA_DECK_REQ_DONE;
#endif
// } added by cychoi 2013.11.25

    return true;
}

bool CompAudioManager::ExtractReqError(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_REQ_ERR) return false;

    ReqError *pStatus = &pPacket->m_tData.m_tReqError;

    MC_LOG << "CompAudioManager::ExtractReqError: 0x" << pStatus->nErrorDetail << LOG_ENDL;

    // { added by cychoi 20103.08.23 for ITS 184879 & 186162
    if(m_bAutoTrackChanged == false)
    {
        MC_HIGH << "CompAudioManager::ExtractReqError(): emit OnRealSoundChanged()" << LOG_ENDL;
        emit OnRealSoundChanged();
        m_bAutoTrackChanged = true;
    }
    // } added by cychoi 20103.08.23

#ifdef CA_DECK_REQ_RETRY_ENABLED
    InitRetryFileFolderTotalNum(); // added by cychoi 2013.11.25 for ITS 210845
    InitRetryTagInfo();
    InitRetryPlayback();
    InitRetryPlaybackMode(); // added by cychoi 2014.02.03 for play mode on SW Reset
#endif

    return true;
}

int CompAudioManager::GetRandomMode(DECK_COMP_RANDOM_MODE eMode)
{
    switch(eMode)
    {
    case eDM_COMP_RANDOM_OFF:                   return RANDOMOFF;
    case eDM_COMP_RANDOM_FILES_IN_FOLDER:       return RANDOMFILE;
    case eDM_COMP_RANDOM_FILES_IN_DISC:         return RANDOMFOLDER;
    case eDM_COMP_RANDOM_FOLDER:                return RANDOMFOLDER;
    case eDM_COMP_RANDOM_FILE_IN_FOLDER:        return RANDOMFILE;
    default:                                    return RANDOMOFF;
    }
}

int CompAudioManager::GetRepeatMode(DECK_COMP_REPEAT_MODE eMode)
{
    switch(eMode)
    {
    case eDM_COMP_REPEAT_OFF:                   return REPEATALL;
    case eDM_COMP_REPEAT_FILE:                  return REPEATFILE;
    case eDM_COMP_REPEAT_FILES_IN_FOLDER:       return REPEATFOLDER;
    case eDM_COMP_REPEAT_FILE_IN_FOLDER:        return REPEATFOLDER;
    default:                                    return REPEATALL;
    }
}

int CompAudioManager::GetScanMode(DECK_COMP_SCAN_MODE eMode)
{
    switch(eMode)
    {
    case eDM_COMP_SCAN_OFF:                     return SCANOFF;
    case eDM_COMP_SCAN_FILES_IN_FOLDER:         return SCANFILE;
    case eDM_COMP_SCAN_FILES_IN_DISC:           return SCANALL;
    case eDM_COMP_SCAN_FOLDER:                  return SCANFOLDER;
    case eDM_COMP_SCAN_FILE_IN_FOLDER:          return SCANFILE;
    default:                                    return SCANOFF;
    }
}


int CompAudioManager::CharToUnicode(UINT1 *pIn, QChar *pOut, int nUniSize)
{
    UINT1   p[2];
    UINT2  *q = (UINT2*)p;
    int     i, j = 0;
    
    for(i = 0; i < nUniSize; i++, j = j + 2)
    {
        p[0] = pIn[j+1];
        p[1] = pIn[j];
        pOut[i] = *q;

        if(pOut[i] == 0) {
            break;
        }
    }

    if(i >= nUniSize) {
        pOut[i] = 0;
    }
    
    return i;
}

QString CompAudioManager::DeckStringToUnicode(int nType, char *pString, int nLen)
{
    Q_UNUSED(nLen);
    
    if(nType == 1 ) { // 1: UTF-16(BIG Endian)
        return QString(QString::fromUtf16((unsigned short*)pString));
    }
    else if(nType == 2) { // 2: UTF-16(LITTLE Endian)
        return QString(QString::fromUtf16((unsigned short*)pString));
    }
    else if(nType == 3) { // 3: UTF8
        return QString(QString::fromUtf8(pString));
    }
    else { // 0: NOT UNICODE 
        QTextCodec *codec = QTextCodec::codecForName("EUC-KR");
        QString     result;
        QTextStream in(pString);

        in.setAutoDetectUnicode(false);
        in.setCodec(codec);

        result = in.readAll ();

        return result;
    }
}

void CompAudioManager::EmitError(int nError)
{
    DeckManager *p = DeckManager::GetInstance();
    if(p) {
        p->EmitError(nError);
    }
}

void CompAudioManager::EmitTrackBuffered(int bPrev, int nTotalIdx)
{
    int nFileIdx    = -1;
    int nTotalFiles = -1;

    // { added by cychoi 20103.07.20 for new OSD display
    if(nTotalIdx == -1)
    {
        emit OnTrackBufferred(bPrev, nTotalIdx, -1, -1);
        return;
    }
    // } added by cychoi 20103.07.20

// { modified by cychoi 2013.12.27 for ITS 217395
#if USE_FILE_INDEX_IN_FOLDER
    QHash<int,FILE_SIZE_INFO>::iterator i;
    for (i = m_tFileSizeInFolder.begin(); i != m_tFileSizeInFolder.end(); ++i) {
        FILE_SIZE_INFO tInfo = i.value();
        if(tInfo.m_nStartNum <= nTotalIdx + 1 && nTotalIdx + 1 < tInfo.m_nStartNum + tInfo.m_nFileSize) {
            nFileIdx    = nTotalIdx + 1 - tInfo.m_nStartNum;
            nTotalFiles = tInfo.m_nFileSize;
            break;
        }
    }
#else
    nFileIdx = nTotalIdx;
    nTotalFiles = m_nTotalFileNum;
#endif
// } modified by cychoi 2013.12.27
    
    emit OnTrackBufferred(bPrev, nTotalIdx, nFileIdx, nTotalFiles);
}

// { added by cychoi 2013-07-15 for ITS 179045 Track Up <-> Down press repeatedly
void CompAudioManager::GetMoveFileFolderIndex(int moveTrackIndex, int *pFileIdx, int *pTotalFiles)
{
    int nFileIdx    = -1;
    int nTotalFiles = -1;
    
    if(m_tFileSizeInFolder.contains(m_tStatus.m_nCurrFolderNo) == true) 
    {
        FILE_SIZE_INFO tInfo = m_tFileSizeInFolder.value(m_tStatus.m_nCurrFolderNo);

        nFileIdx    = (moveTrackIndex + 1) - tInfo.m_nStartNum;
        nTotalFiles = tInfo.m_nFileSize;
    }

    *pFileIdx = nFileIdx;
    *pTotalFiles = nTotalFiles;
}
// } added by cychoi 2013-07-15

// { modified by cychoi 2013-07-15 for ITS 179045 Track Up <-> Down press repeatedly
int CompAudioManager::GetNextTrackIdx(int nSkiplevel, int moveTrackIndex)
{
    if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFOLDER) 
    {
        int nFileIdx  = 0;
        int nFileSize = 0;
        int nTrackIdx = moveTrackIndex; //GetCurrentIndex();

        GetMoveFileFolderIndex(nTrackIdx, &nFileIdx, &nFileSize);
        if(nFileIdx < 0 || nFileSize < 0) 
        {
            return nTrackIdx;
        }

        int nNewFileIdx = nFileIdx + nSkiplevel % nFileSize;
        if(nNewFileIdx >= nFileSize) {
            nNewFileIdx = nNewFileIdx % nFileSize;
        }
        
        return nTrackIdx + (nNewFileIdx - nFileIdx);
    }
    else
    {
        int nTrackIdx    = moveTrackIndex; //GetCurrentIndex();
        int nNewTrackIdx = nTrackIdx + nSkiplevel % m_nTotalFileNum;
        
        if(nNewTrackIdx >= m_nTotalFileNum) {
            nNewTrackIdx = nNewTrackIdx - m_nTotalFileNum;
        }
        
        return nNewTrackIdx;
    }
}

int CompAudioManager::GetPrevTrackIdx(int nSkiplevel, int moveTrackIndex)
{
    if(GetRepeatMode(m_tStatus.m_nRepeatMode) == REPEATFOLDER)
    {
        int nFileIdx  = 0;
        int nFileSize = 0;
        int nTrackIdx = moveTrackIndex; //GetCurrentIndex();

        GetMoveFileFolderIndex(nTrackIdx, &nFileIdx, &nFileSize);
        if(nFileIdx < 0 || nFileSize < 0) 
        {
            return nTrackIdx;
        }

        int nNewFileIdx = nFileIdx - nSkiplevel % nFileSize;
        if(nNewFileIdx < 0) {
            nNewFileIdx = nFileSize + (nNewFileIdx % nFileSize);
        }
        
        return nTrackIdx - (nFileIdx - nNewFileIdx);
    }
    else
    {
        int nTrackIdx    = moveTrackIndex; //GetCurrentIndex();
        int nNewTrackIdx = nTrackIdx - nSkiplevel % m_nTotalFileNum;
        
        if(nNewTrackIdx < 0) {
            nNewTrackIdx = m_nTotalFileNum + nNewTrackIdx;
        }
        
        return nNewTrackIdx;
    }
}

// { added by cychoi 2015-05-19 for QE_Smoke Test Ver_45버전(130519) 37 fail
int CompAudioManager::GetRandomTrackIdx(int moveTrackIndex)
{
    if(GetRandomMode(m_tStatus.m_nRandomMode) == RANDOMFILE) 
    {
        int nFileIdx  = 0;
        int nFileSize = 0;
        int nTrackIdx = moveTrackIndex; //GetCurrentIndex();

        GetMoveFileFolderIndex(nTrackIdx, &nFileIdx, &nFileSize);
        if(nFileIdx < 0 || nFileSize < 0) 
        {
            return nTrackIdx;
        }

        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

        // { modified by cychoi 2013.06.13 for ITS 150493 folder random
        int nNewFileIdx = nFileIdx + qrand() % nFileSize;
        if(nNewFileIdx >= nFileSize) {
            nNewFileIdx = nNewFileIdx % nFileSize;
        }

        return nTrackIdx + (nNewFileIdx - nFileIdx);
        // } modified by cychoi 2013.06.13 for ITS 150493 folder random
    }
    else
    {
        int nNewTrackIdx;

        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

        nNewTrackIdx = qrand() % m_nTotalFileNum;

        return nNewTrackIdx;
    }
}
// } added by cychoi 2015-05-19
// } modified by cychoi 2013-07-15

#ifdef CA_DECK_REQ_RETRY_LIMITATION_ENABLED
bool CompAudioManager::HandleErrorDisc(bool bRetryToPlay)
{
    // { modified by cychoi 2014.04.14 for HW Event PQ launch DVD mode unconditionally after Deck SW reset
    bool bReset = false;

    if(bRetryToPlay == true)
    {
        if(m_tStatus.m_nPlayBackTime != 0)
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

    MC_HIGH << "CompAudioManager::HandleErrorDisc: " << m_nDiscPlayRetryCount << ", " << m_nDiscDetectRetryCount << ", " << bReset << LOG_ENDL;
    // Please handle as Error Disc on FileFolderTotalNum or Play Request is failed 3 times
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
int CompAudioManager::GetInternalMediaState()
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
void CompAudioManager::EmitTrackChange(int nTrackNo, bool bTrackChanged, bool bChangedFFRew)
{
    Q_UNUSED(bTrackChanged);
    // { modified by cychoi 2013.09.06 for ITS 178797
    m_eAppRequestTagInfo    = CA_DECK_REQ_NONE;
    m_eAppRequestTag2Info   = CA_DECK_REQ_NONE;
    m_eAppRequestAllTagInfo = CA_DECK_REQ_NONE;
    // } modified by cychoi 2013.09.06
    // { added by cychoi 2013.09.22 for SmokeTest MP3 ID3 Tag display error
    m_tAllTagInfo.exist         = 0;
    m_tAllTagInfo.valid         = 0;
    // } added by cychoi 2013.09.22
    m_tAllTagInfo.out_info_type = -1;
// { added by cychoi 2013-06-05 for enabling to retry deck request on fail
#ifdef CA_DECK_REQ_RETRY_ENABLED
    InitRetryTagInfo();
#endif
// } added by cychoi 2013-06-05
    m_tStatus.m_nFileNo  = nTrackNo;
#if 0
    if(ePrevPlayerState != eMP_STATE_STOPPED && ePrevPlayerState != eMP_STATE_NONE) {
        MC_LOG << "CompAudioManager::ePrevPlayerState: " << ePrevPlayerState << ". emit OnCurrentMediaStatusChanged()" << LOG_ENDL;
        emit OnCurrentMediaStatusChanged(PLAYER_EndOfMedia);
    }
#endif
    // IF FF/REW AND REPEATFILE ---> REPEATOFF
    //if(m_ePlayerState == eMP_STATE_FAST_PLAYING && m_nUserRepeatMode == REPEATFILE) { //commented by jeeeun for SANITY_CM_AG079
    if((m_ePlayerState == eMP_STATE_FAST_PLAYING || bChangedFFRew == true) && m_nUserRepeatMode == REPEATFILE) { //added by jeeeun for SANITY_CM_AG079
        m_nUserRepeatMode = -1;
        RepeatPlay(REPEATALL);
    }
    m_bControllerChanged = false;
    // { added by cychoi 20103.07.22 for add new signal
    if(m_bUserTrackChanged == true)
    {
        emit OnUserTrackChanged(m_tStatus.m_nFileNo - 1);
        m_bUserTrackChanged = false;
    }
    // } added by cychoi 20103.07.22
    // { added by cychoi 2014.04.29 for ITS 233500
    else
    {
        if(bTrackChanged == true)
        {
            emit OnAutoTrackChanged(m_tStatus.m_nFileNo - 1);
        }
    }
    // } added by cychoi 2014.04.29
    emit OnTrackChanged(m_tStatus.m_nFileNo - 1);
    // removed by cychoi 20103.06.13 for Seek Up/Down & FF/REW mute
}
// } added by cychoi 2014.08.29
