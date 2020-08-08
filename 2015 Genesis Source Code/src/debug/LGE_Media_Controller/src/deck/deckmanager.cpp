#include "deckmanager.h"
#include "packetbuffer.h"
#include "nullmanager.h"
#include "dvdmanager.h"
#include "cddamanager.h"
#include "videocdmanager.h"
#include "compaudiomanager.h"
#include "util.h"
#include "iplayerengine.h"
#include "gplayerengine.h"

#include <QtCore/QDir>
#include <QGlib/Connect>
#include <QGlib/Error>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>
#include <QGst/Bus>
#include <QGst/Message>
#include <QGst/Query>
#include <QGst/ClockTime>
#include <QGst/Event>
#include <QGst/Init>
#include <stdio.h>

DeckManager* DeckManager::m_pInstance = 0;

DeckManager* DeckManager::GetInstance()
{
    if(m_pInstance == 0) {
        m_pInstance = new DeckManager();
    }

    return m_pInstance;
}

void DeckManager::DeleteInstance()
{
    if(m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}


DeckManager::~DeckManager()
{    
    MC_LOG << "DeckManager::~DeckManager()" << LOG_ENDL;
    
    Packet::DeleteInstace();
    // PacketBuffer::DeleteInstance();  
    // DVDManager::DeleteInstance();
    // VideoCDManager::DeleteInstance();
    // CompAudioManager::DeleteInstance();
    // CDDAManager::DeleteInstance();
}

IMediaInterface* DeckManager::GetMediaInstance()
{
    IMediaInterface *pMedia;
    
    switch(m_tDeckStatus.m_nMediaMode)
    {
    case eDM_MODE_DVD_VIDEO:
    case eDM_MODE_DVD_AUDIO:
        //MC_LOG << "DeckManager::GetMediaInstance: DVD" << LOG_ENDL;
        pMedia = DVDManager::GetInstance();
        break;
    case eDM_MODE_CD_DA:
    case eDM_MODE_DTS_CD:
        //MC_LOG << "DeckManager::GetMediaInstance: CDDA" << LOG_ENDL;
        pMedia = CDDAManager::GetInstance();
        break;        
    case eDM_MODE_VIDEO_CD_1_1:
    case eDM_MODE_VIDEO_CD_2_0:
    case eDM_MODE_VIDEO_CD_SVCD:
        //MC_LOG << "DeckManager::GetMediaInstance: VCD" << LOG_ENDL;
        pMedia = VideoCDManager::GetInstance();
        break;        
    case eDM_MODE_COMP_AUDIO:
        //MC_LOG << "DeckManager::GetMediaInstance: COMP-AUDIO" << LOG_ENDL;
        pMedia = CompAudioManager::GetInstance();
        break;        
    default:
        MC_LOG << "DeckManager::GetMediaInstance: NullManager" << LOG_ENDL;
        pMedia = NullManager::GetInstance();
        break;
    }

    return pMedia;
}

IAudioInterface* DeckManager::GetAudioInstance()
{
    IAudioInterface *pMedia = 0;
    
    switch(m_tDeckStatus.m_nMediaMode)
    {
    case eDM_MODE_CD_DA:
    case eDM_MODE_DTS_CD:
        //MC_LOG << "DeckManager::GetAudioInstance: CDDA" << LOG_ENDL;
        pMedia = CDDAManager::GetInstance();
        break;
    case eDM_MODE_COMP_AUDIO:
        //MC_LOG << "DeckManager::GetAudioInstance: COMP-AUDIO" << LOG_ENDL;
        pMedia = CompAudioManager::GetInstance();
        break;        
    default:
        MC_LOG << "DeckManager::GetAudioInstance: NullManager" << LOG_ENDL;
        pMedia = NullManager::GetInstance();
        break;
    }

    return pMedia;
}

IVideoInterface* DeckManager::GetVideoInstance()
{
    IVideoInterface *pMedia = 0;

    switch(m_tDeckStatus.m_nMediaMode)
    {
    case eDM_MODE_DVD_VIDEO:
    case eDM_MODE_DVD_AUDIO:
        //MC_LOG << "DeckManager::GetVideoInstance: DVD" << LOG_ENDL;
        pMedia = DVDManager::GetInstance();
        break;
    case eDM_MODE_VIDEO_CD_1_1:
    case eDM_MODE_VIDEO_CD_2_0:
    case eDM_MODE_VIDEO_CD_SVCD:
        //MC_LOG << "DeckManager::GetVideoInstance: VCD" << LOG_ENDL;
        pMedia = VideoCDManager::GetInstance();
        break;        
    default:
        MC_LOG << "DeckManager::GetVideoInstance: NullManager" << LOG_ENDL;
        break;
    }

    return pMedia;
}

void DeckManager::OpenDeckProtocol()
{
    m_pPacket = Packet::GetInstace();
    m_pPacket->Open();
    // { modified by cychoi 2013.10.04 for Disc Detect Performance on Boot-Up
    if(m_pPacket->RequestCommonStatus() > 0)
    {
        MC_LOG << "DeckManager::OpenDeckProtocol(): RequestCommonStatus() success." << LOG_ENDL;
    }
    else
    {
        MC_LOG << "DeckManager::OpenDeckProtocol(): RequestCommonStatus() failure." << LOG_ENDL;
        StartCommonStatusTimer(); 
    }
    // } modified by cychoi 2013.10.04
#if _DECK_DIAGNOSTIC_
    // { modified by cychoi 2014.01.28 for set timeout to 15s on OpenDeckProtocol
    //MC_HIGH << "DiagTimer started." << LOG_ENDL;
    m_tDiagTimer.start(15000);
    // } modified by cychoi 2014.01.28
#endif
}

void DeckManager::CloseDeckProtocol()
{
    m_pPacket->Stop();
    m_pPacket->Close();
#if _DECK_DIAGNOSTIC_
    if(m_tDiagTimer.isActive() == true)
    {
        //MC_LOG << "DeckManager::CloseDeckProtocol(): DiagTimer stopped." << LOG_ENDL;
        m_tDiagTimer.stop();
    }
#endif
}

void DeckManager::InitializeDeckPrioritySettings(DeckPrioritySettings *pSettings)
{
    // initialize subtitle, audio, menu language.
    InitialSetting1Command tCmd;
    DeckManager::GetInstance()->GetInitSetting1Cmd(tCmd);

    BCD2 tBCD2;
    
    tBCD2.BCD.upper             = (pSettings->captionLanguage / 100)/10;
    tBCD2.BCD.lower             = (pSettings->captionLanguage / 100)%10;
    tCmd.nSubTitleCodeUpper     = tBCD2.byte;
    tBCD2.BCD.upper             = (pSettings->captionLanguage % 100)/10;
    tBCD2.BCD.lower             = (pSettings->captionLanguage % 100)%10;
    tCmd.nSubTitleCodeLower     = tBCD2.byte;   

    tBCD2.BCD.upper             = (pSettings->audioLangauge / 100)/10;
    tBCD2.BCD.lower             = (pSettings->audioLangauge / 100)%10;
    tCmd.nAudioCodeUpper        = tBCD2.byte;
    tBCD2.BCD.upper             = (pSettings->audioLangauge % 100)/10;
    tBCD2.BCD.lower             = (pSettings->audioLangauge % 100)%10;
    tCmd.nAudioCodeLower        = tBCD2.byte;

    tBCD2.BCD.upper             = (pSettings->menuLanguage / 100)/10;
    tBCD2.BCD.lower             = (pSettings->menuLanguage / 100)%10;
    tCmd.nMenuLangCodeUpper     = tBCD2.byte;
    tBCD2.BCD.upper             = (pSettings->menuLanguage % 100)/10;
    tBCD2.BCD.lower             = (pSettings->menuLanguage % 100)%10;
    tCmd.nMenuLangCodeLower     = tBCD2.byte;
    
    m_pPacket->InitializationSetting1(&tCmd);
}

void DeckManager::SetEject(bool bEject)
{
    MC_LOG << "DeckManager::SetEject(" << bEject << ")" << LOG_ENDL;
    
    m_bEject = bEject;
    if(bEject == true) {
        m_tPrevDeckStatus   = DeckCommonStatus();
        m_bLoadComplete     = false;
        m_nDeviceState      = DiscNotPresent;
#ifdef DECK_DISC_DETECTION_ERR_HANDLING
        if(m_tDiscDetectLimitationTimer.isActive() == true)
        {
            MC_LOG << "DeckManager::SetEject m_tDiscDetectLimitationTimer stop" << LOG_ENDL;
            m_tDiscDetectLimitationTimer.stop();
        }
        m_nDiscDetectRetryCount = 0;
        // { added by cychoi 2014.03.10 for ISV 98219
        m_nDiscReadRetryCount = 0;
        m_nDiscDetectReadRetryCount = 0;
        // } added by cychoi 2014.03.10
#endif
    }
    else {
        m_nDeviceState      = DiscPresnt;
    }
}

int DeckManager::PlayCondToPlayerState(DECK_CD_PLAY_COND ePlayCond)
{
    MEDIA_PLAYER_STATE eState;

    switch(ePlayCond)
    {
    case eDM_CD_PLAY_COND_HALTING:
    case eDM_CD_PLAY_COND_TYPE_ERROR:
    case eDM_CD_PLAY_COND_DISC_ID_INCOMPLETE:
    case eDM_CD_PLAY_COND_HALTING_BY_ERROR:
    case eDM_CD_PLAY_COND_NOT_APPLICABLE_FORMAT:
    case eDM_CD_PLAY_COND_NO_BUS_RIGHT:
        eState = eMP_STATE_STOPPED;
        break;

    case eDM_CD_PLAY_COND_PLAYING:
    case eDM_CD_PLAY_COND_PLAYING_SLOW:
        eState = eMP_STATE_PLAYING;
        break;
        
    case eDM_CD_PLAY_COND_PLAYING_FAST:
        eState = eMP_STATE_FAST_PLAYING;
        break;

    case eDM_CD_PLAY_COND_SEARCHING_OUT:
    case eDM_CD_PLAY_COND_SEARCHING_IN:
        eState = eMP_STATE_SEARCHING;
        break;
    
    case eDM_CD_PLAY_COND_PAUSING:
        eState = eMP_STATE_PAUSED;
        break;
    
    default:
        eState = eMP_STATE_NONE;
        break;
    }

    return (int)eState;
}

DECK_MEDIA_MODE DeckManager::GetMediaMode()
{
    return m_tDeckStatus.m_nMediaMode;
}

void DeckManager::SetBrightness(int nBrightness)
{
    if(nBrightness == m_tPicture.m_nBrightnessLevel) return;

    MC_LOG << "DeckManager::SetBrightness: " << m_tPicture.m_nBrightnessLevel << "-" << nBrightness << LOG_ENDL;

    m_pPacket->PictureSetting(m_tPicture.m_nContrastLevel,
                              m_tPicture.m_nColorLevel,
                              m_tPicture.m_nTintLevel,
                              nBrightness,
                              m_tPicture.m_nSharpnessLevel,
                              m_tPicture.m_nGammaCorrection,
                              m_tPicture.m_nCoringFilter,
                              m_tPicture.m_nBlackLevel,
                              m_tPicture.m_nWhiteLevel);
    
}

void DeckManager::SetContrast(int nContrast)
{
    if(nContrast == m_tPicture.m_nContrastLevel) return;

    MC_LOG << "DeckManager::SetContrast: " << m_tPicture.m_nContrastLevel << "-" << nContrast << LOG_ENDL;

    m_pPacket->PictureSetting(nContrast,
                              m_tPicture.m_nColorLevel,
                              m_tPicture.m_nTintLevel,
                              m_tPicture.m_nBrightnessLevel,
                              m_tPicture.m_nSharpnessLevel,
                              m_tPicture.m_nGammaCorrection,
                              m_tPicture.m_nCoringFilter,
                              m_tPicture.m_nBlackLevel,
                              m_tPicture.m_nWhiteLevel);
}

void DeckManager::SetTint(int nTint)
{
    if(nTint == m_tPicture.m_nTintLevel) return;

    MC_LOG << "DeckManager::SetTint: " << m_tPicture.m_nTintLevel << "-" << nTint << LOG_ENDL;
    
    m_pPacket->PictureSetting(m_tPicture.m_nContrastLevel,
                              m_tPicture.m_nColorLevel,
                              nTint,
                              m_tPicture.m_nBrightnessLevel,
                              m_tPicture.m_nSharpnessLevel,
                              m_tPicture.m_nGammaCorrection,
                              m_tPicture.m_nCoringFilter,
                              m_tPicture.m_nBlackLevel,
                              m_tPicture.m_nWhiteLevel);    
}

int DeckManager::GetBrightness()
{
    // MC_LOG << "DeckManager::GetBrightness(): " << m_tPicture.m_nBrightnessLevel << LOG_ENDL;
    
    return m_tPicture.m_nBrightnessLevel;
}

int DeckManager::GetContrast()
{
    // MC_LOG << "DeckManager::GetContrast(): " << m_tPicture.m_nContrastLevel << LOG_ENDL;
    
    return m_tPicture.m_nContrastLevel;
}

int DeckManager::GetTint()
{
    // MC_LOG << "DeckManager::GetTint(): " << m_tPicture.m_nTintLevel << LOG_ENDL;
    
    return m_tPicture.m_nTintLevel;
}

void DeckManager::SetScanMode(int nMode)
{
    m_nScanMode = nMode;
}

void DeckManager::SetRandomMode(int nMode)
{
    m_nRandomMode = nMode;
}

void DeckManager::SetRepeatMode(int nMode)
{
    m_nRepeatMode = nMode;
}

int DeckManager::GetScanMode()
{
    return m_nScanMode;   
}

int DeckManager::GetRandomMode()
{
    return m_nRandomMode;
}

int DeckManager::GetRepeatMode()
{
    return m_nRepeatMode;
}

QWidget* DeckManager::GetVideoWidget()
{
    MC_LOG << "DeckManager::GetVideoWidget" << LOG_ENDL;

    if(m_pGPlayer) {
        GPlayerEngine *pGPlayer = (GPlayerEngine*)m_pGPlayer;
        return pGPlayer->m_pVideoWidget;
    }else{
        return 0;
    }
}

void DeckManager::ControlGPlayerEngine(int nCurrState, int nPrevState)
{
    if(IsPlayState(nPrevState) == false && IsPlayState(nCurrState) == true) {
        m_pGPlayer->SetURI(DVD_DEVICE_PATH_FOR_PE);
        m_pGPlayer->Play();
        MC_LOG << "DeckManager::ControlGPlayerEngine() ---> m_pGPlayer->Play()" << LOG_ENDL;
    }
    else if(IsPauseState(nPrevState) == false && IsPauseState(nCurrState) == true) {
        m_pGPlayer->Pause();
        MC_LOG << "DeckManager::ControlGPlayerEngine() ---> m_pGPlayer->Pause()" << LOG_ENDL;
    }
    else if(IsStopState(nPrevState) == false && IsStopState(nCurrState) == true) {
        m_pGPlayer->Stop();
        MC_LOG << "DeckManager::ControlGPlayerEngine() ---> m_pGPlayer->Stop()" << LOG_ENDL;
    }
    else {
        MC_LOG << "DeckManager::ControlGPlayerEngine() ---> NULL" << LOG_ENDL;
    }
}

bool DeckManager::ClearDisplay(bool isBackground)
{
    MC_LOG << "DeckManager::ClearDisplay(" << isBackground << ")" << LOG_ENDL;
    m_pGPlayer->ClearDisplay(isBackground);

    #if 0
    if(isBackground == true) {
        MC_LOG << "DeckManager::ClearDisplay() ---> m_pGPlayer->Pause()" << LOG_ENDL;
        m_pGPlayer->Pause();
    }
    else {
        MC_LOG << "DeckManager::ClearDisplay() ---> m_pGPlayer->Play()" << LOG_ENDL;
        m_pGPlayer->Play();
    }
    #endif
    
    return true;
}

IPlayerEngine* DeckManager::GetPlayerEngine()
{
    return m_pGPlayer;
}

void DeckManager::EmitError(int nError)
{
    MC_LOG << "DeckManager::EmitError(" << nError << "). m_bReloadAfterError : " << m_bReloadAfterError << ", m_bReloadDisc : " << m_bReloadDisc << LOG_ENDL;
    
    switch(nError) 
    {
    case PLAYER_NOMEDIA: 
    case PLAYER_DISCCORRUPTED:
    case PLAYER_DISC_READ_ERROR:
    case PLAYER_DISC_REGION_ERROR:
        if(m_bReloadAfterError == true && m_bReloadDisc == true) {
            return;
        }
        // removed by cychoi 2013.08.10 for unused code
        break;
    default:
        break;
    }
                
    m_nError = nError;
    emit OnError(nError);
}

int DeckManager::GetError()
{
    return m_nError;
}


int DeckManager::GetDeviceState()
{
    // { added by cychoi 2013.10.17 for ITS 196656 & ITS 194618
    if(GetCurrentError() != PLAYER_NO_ERROR)
    {
        return DiscError;
    }
    // } added by cychoi 2013.10.17

    return m_nDeviceState;
}

// { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
bool DeckManager::IsDeckBlocked()
{
    if(m_tDeckStatus.m_nHighTemp == eDM_HIGH_TEMP_STOP ||
       m_tDeckStatus.m_nConsumption == eDM_CONSUMPTION_LOW)
    {
        MC_HIGH << "Deck blocked" << LOG_ENDL;
        // should block Deck operation on High Temperature or Consumption Low
        return true;
    }

    return false;
}
// } added by cychoi 2014.04.09

int DeckManager::IsSupportMediaMode(DECK_MEDIA_MODE eMediaMode)
{
    switch(eMediaMode)
    {
    case eDM_MODE_DVD_VIDEO:
    case eDM_MODE_DVD_AUDIO:
    case eDM_MODE_CD_DA:
    case eDM_MODE_DTS_CD:
    case eDM_MODE_VIDEO_CD_1_1:
    case eDM_MODE_VIDEO_CD_2_0:
    case eDM_MODE_VIDEO_CD_SVCD:
    case eDM_MODE_COMP_AUDIO:
        return true;
    default:
        return false;
    }
}

int DeckManager::IsEject()
{
    return m_bEject;
}

int DeckManager::IsExistCDText()
{
    return m_bCDText;
}

int DeckManager::IsMixedCD()
{
    return m_bMixedCD;
}

int DeckManager::IsExistCDInDeck()
{
    if(m_tDeckStatus.m_nDiscPos == eDM_DISC_POS_NO_DISC) 
        return false;
    else
        return true;
}

DECK_MIXED_CD_MODE DeckManager::GetSwitchMixedMode()
{
    return m_eMixedCDMode;
}

void DeckManager::SetSwitchMixedMode(DECK_MIXED_CD_MODE eMode)
{
    m_eMixedCDMode = eMode;
}

// { added by cychoi 20103.09.26 for ITS 188493 DVD Navigation Still Picture
DECK_MEDIA_MODE DeckManager::GetDVDMediaMode()
{
    return m_nDVDMediaMode;
}
// } added by cychoi 20103.09.26

// { added by cychoi 20103.10.10 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
DECK_MEDIA_MODE DeckManager::GetCDDAMediaMode()
{
    return m_nCDDAMediaMode;
}
// } added by cychoi 20103.10.10

void DeckManager::SetOSD(uint nMode)
{
    InitialSetting1Command tCmd;
    GetInitSetting1Cmd(tCmd);
    if(nMode == 1) tCmd.nOSD = 0;
    else           tCmd.nOSD = 1;
    m_pPacket->InitializationSetting1(&tCmd);
}

void DeckManager::GetCommonStatus(DeckCommonStatus &tStatus)
{
    tStatus = m_tDeckStatus;
}

void DeckManager::GetInitSetting1(DeckInit1Setting &tSetting)
{
    tSetting = m_tInit1;
}

void DeckManager::GetInitSetting2a(DeckInit2aSetting &tSetting)
{
    tSetting = m_tInit2a;
}

void DeckManager::GetInitSetting3(DeckInit3Setting &tSetting)
{
    tSetting = m_tInit3;
}

void DeckManager::GetInitSetting1Cmd(InitialSetting1Command &tCmd)
{
    BCD2 tBCD2;
        
    tCmd.nOSD                    = m_tInit1.m_bOSDOff;
    tCmd.nRegionCode             = m_tInit1.m_nRegionCode;
    tCmd.nParentalLevel          = m_tInit1.m_nParentalLevel; // 1 ~ 8

    tBCD2.BCD.upper             = (m_tInit1.m_nPrioSubTitleCode / 100)/10;
    tBCD2.BCD.lower             = (m_tInit1.m_nPrioSubTitleCode / 100)%10;
    tCmd.nSubTitleCodeUpper     = tBCD2.byte;
    tBCD2.BCD.upper             = (m_tInit1.m_nPrioSubTitleCode % 100)/10;
    tBCD2.BCD.lower             = (m_tInit1.m_nPrioSubTitleCode % 100)%10;
    tCmd.nSubTitleCodeLower     = tBCD2.byte;   

    tBCD2.BCD.upper             = (m_tInit1.m_nPrioAudioCode / 100)/10;
    tBCD2.BCD.lower             = (m_tInit1.m_nPrioAudioCode / 100)%10;
    tCmd.nAudioCodeUpper        = tBCD2.byte;
    tBCD2.BCD.upper             = (m_tInit1.m_nPrioAudioCode % 100)/10;
    tBCD2.BCD.lower             = (m_tInit1.m_nPrioAudioCode % 100)%10;
    tCmd.nAudioCodeLower        = tBCD2.byte;

    tBCD2.BCD.upper             = (m_tInit1.m_nPrioMenuLangCode / 100)/10;
    tBCD2.BCD.lower             = (m_tInit1.m_nPrioMenuLangCode / 100)%10;
    tCmd.nMenuLangCodeUpper     = tBCD2.byte;
    tBCD2.BCD.upper             = (m_tInit1.m_nPrioMenuLangCode % 100)/10;
    tBCD2.BCD.lower             = (m_tInit1.m_nPrioMenuLangCode % 100)%10;
    tCmd.nMenuLangCodeLower     = tBCD2.byte;

    tCmd.nScreenSize            = m_tInit1.m_nScreenSize;
    tCmd.nAngle                 = m_tInit1.m_nAngleDisplay;
    tCmd.nSubTitle              = m_tInit1.m_nSubTitleDisplay;
    tCmd.nUndefined1            = 0;

    BCD4 tBCD4;
    tBCD4.word                  = m_tInit1.m_nPinNumber;
    tCmd.nPinNumber1            = tBCD4.BCD.upper1;
    tCmd.nPinNumber2            = tBCD4.BCD.lower1;
    tCmd.nPinNumber3            = tBCD4.BCD.upper2;
    tCmd.nPinNumber4            = tBCD4.BCD.lower2;

    tCmd.nOpenningScreen        = m_tInit1.m_nOpenningScreen;
    tCmd.nUndefined2            = 0;

    tCmd.nBackColor             = m_tInit1.m_nBackColor;
    tCmd.nUndefined3            = 0;
    tCmd.nIRE                   = m_tInit1.m_nIRE;
    tCmd.nPBC                   = m_tInit1.m_nPBC;
    tCmd.nVideoMode             = m_tInit1.m_nPictureMode;
    tCmd.nScreenMode            = m_tInit1.m_nScreenMode;

    // m_tInit1.m_nCountryCode     = ISO3166TOBCD('K', 'R');
    tCmd.nCountryCodeUpper      = m_tInit1.m_nCountryCode >> 8;
    tCmd.nCountryCodeLower      = m_tInit1.m_nCountryCode & 0xFF;
    
    tCmd.nUndefined4            = 0;
    tCmd.nPlayerMode            = m_tInit1.m_nPlayerMode;
    tCmd.nVideoOutType          = m_tInit1.m_nVideoOutput;
}

void DeckManager::GetInitSetting2aCmd(InitialSetting2aCommand &tCmd)
{
    tCmd.nAudioOutput       = m_tInit2a.m_nAudioOutput;
    tCmd.nUndefined1        = 0;
    tCmd.nDualMono          = m_tInit2a.m_nDualMono;
    tCmd.nPCMSetting        = m_tInit2a.m_nPCMSetting;
    tCmd.nDSPIFDownMix      = m_tInit2a.m_nDSPIFDownMix;
    tCmd.n2chIFDownMix      = m_tInit2a.m_n2chIFDownMix;
    tCmd.nFrontSp           = m_tInit2a.m_nFrontSp;
    tCmd.nCenterSp          = m_tInit2a.m_nCenterSp;
    tCmd.nRearSp            = m_tInit2a.m_nRearSp;
    tCmd.nSubWoofer         = m_tInit2a.m_nSubwoofer;
    tCmd.nCenterSpDelay     = m_tInit2a.m_nCenterSpDelay;
    tCmd.nRearSpDelay       = m_tInit2a.m_nRearSpDelay;
    tCmd.nCompLevel         = m_tInit2a.m_nCompLevel;
    tCmd.nUndefined2        = 0;
    tCmd.nFrontLSpLevel     = m_tInit2a.m_nFrontLSpLevel;
    tCmd.nFrontRSpLevel     = m_tInit2a.m_nFrontRSpLevel;
    tCmd.nRearLSpLevel      = m_tInit2a.m_nRearLSpLevel;
    tCmd.nRearRSpLevel      = m_tInit2a.m_nRearRSpLevel;
    tCmd.nCenterSpLevel     = m_tInit2a.m_nCenterSpLevel;
    tCmd.nDTSOutput         = m_tInit2a.m_nDTSOutput;
    tCmd.n6chStereo         = m_tInit2a.m_n6chStereo;
    tCmd.nBassManagement    = m_tInit2a.m_nBaseMngr;
    tCmd.nDownSampling      = m_tInit2a.m_nDownSampling;
    tCmd.nUndefined3        = 0;
    tCmd.nUndefined4        = 0;
    tCmd.nDOLPro12          = m_tInit2a.m_nDolPro12;
    tCmd.nCenterBlendLevel  = m_tInit2a.m_nCenterBlendLvl;
    tCmd.nUndefined5        = 0;
    tCmd.nDimension         = m_tInit2a.m_nDimension;
    tCmd.nPanorama          = m_tInit2a.m_nPanorama;
    tCmd.nCenterWidth       = m_tInit2a.m_nCenterWidth;
    tCmd.nUndefined6        = 0;
}

void DeckManager::GetInitSetting3Cmd(InitialSetting3Command &tCmd)
{
    tCmd.nNoFileFolder      = m_tInit3.m_nNoFileFolder;
    tCmd.nUndefined1        = 0;
    tCmd.nPriorityID3Tag    = m_tInit3.m_nPriorityID3Tag;
    tCmd.nMP3Play           = m_tInit3.m_nMP3Play;
    tCmd.nWMAPlay           = m_tInit3.m_nWMAPlay;
    tCmd.nUndefined2        = 0;
    tCmd.nPVDSVD            = m_tInit3.m_nPVDSVD;


    tCmd.nUndefined3        = 0;
    tCmd.nRandomTrackUp     = m_tInit3.m_nRandomTrackUp;
    tCmd.nRepeatCancel      = m_tInit3.m_nRepeatCancel;
    tCmd.nRandomCancel      = m_tInit3.m_nRandomCancel;
    tCmd.nFastUpPlay        = m_tInit3.m_nFastUpPlay;
    tCmd.nFastDownPlay      = m_tInit3.m_nFastDownPlay;
    tCmd.nTrackDownTime     = m_tInit3.m_nTrackDownTime;

    tCmd.nUndefined4        = 0;
    tCmd.nSingleSession     = m_tInit3.m_nSingleSession;
    tCmd.nDVDAudioPlay      = m_tInit3.m_nDVDAudioPlay;
    tCmd.nHighTempStop      = m_tInit3.m_nHaltByHighTemp;

    tCmd.nUndefined5        = 0;
    tCmd.nTitleEnd          = m_tInit3.m_nTitleEnd;
    tCmd.nEjectResume       = m_tInit3.m_nEjectRsume;

    tCmd.nUndefined6        = 0;
    tCmd.nDirPriority       = m_tInit3.m_nDirPriority;
    tCmd.nPlayTimeDisplay   = m_tInit3.m_nPlayTimeDisplay;
    tCmd.nUndefined7        = 0;

    tCmd.nUndefined8        = 0;
    tCmd.nUndefined9        = 0;
}

void DeckManager::SetInitSetting1Cmd(InitialSetting1Command &tCmd)
{
    Q_UNUSED(tCmd);
    /*
    tCmd.nOSD                       = 0;
    tCmd.nRegionCode                = 3;
    tCmd.nParentalLevel             = 7;
    
    m_tInit1.m_nPrioSubTitleCode    = ISO639TOBCD('E', 'N');
    tCmd.nSubTitleCodeUpper         = m_tInit1.m_nPrioSubTitleCode / 100;
    tCmd.nSubTitleCodeLower         = m_tInit1.m_nPrioSubTitleCode % 100;

    m_tInit1.m_nPrioAudioCode       = ISO639TOBCD('E', 'N');
    tCmd.nAudioCodeUpper            = m_tInit1.m_nPrioAudioCode / 100;
    tCmd.nAudioCodeLower            = m_tInit1.m_nPrioAudioCode % 100;

    m_tInit1.m_nPrioMenuLangCode    = ISO639TOBCD('E', 'N');
    tCmd.nMenuLangCodeUpper         = m_tInit1.m_nPrioMenuLangCode / 100;
    tCmd.nMenuLangCodeLower         = m_tInit1.m_nPrioMenuLangCode % 100;

    m_tInit1.m_nCountryCode         = ISO3166TOBCD('K', 'R');
    tCmd.nCountryCodeUpper          = m_tInit1.m_nCountryCode >> 8;
    tCmd.nCountryCodeLower          = m_tInit1.m_nCountryCode & 0xFF;

    tCmd.nAngle                     = 0;
    tCmd.nSubTitle                  = 0;
    */
}

void DeckManager::SetInitSetting2aCmd(InitialSetting2aCommand &tCmd)
{
    Q_UNUSED(tCmd);
}

void DeckManager::SetInitSetting3Cmd(InitialSetting3Command &tCmd)
{
    Q_UNUSED(tCmd);
}

void DeckManager::Eject(uint mode)
{
    MC_HIGH << "DeckManager::Eject(" << mode << ")" << LOG_ENDL;
    
    // { added by kihyung 2012.12.20 for CR 14702
    if(m_pPacket->Eject(mode) > 0) {
        EmitDiscStateChanged(PLAYER_DISCEJECTING);
    }
    // } added by kihyung
}

// { modified by cychoi 2014.04.14 for HW Event PQ launch DVD mode unconditionally after Deck SW reset
#ifdef DECK_DISC_DETECTION_ERR_HANDLING
void DeckManager::HandleErrorDisc(int nError, bool bReset)
{
    if(bReset == true)
    {
        m_bDiagReqSend = false;
        if(m_bDiagCommError == false)
        {
            m_bDiagCommError = true;
            emit OnCommErrorWithDeckChanged(m_bDiagCommError);
        }
    }
    else
    {
        EmitError(nError);

        if(m_bReloadAfterError == false || m_bReloadDisc == false)
        {
            DeckManager::GetInstance()->Eject(false);
            SetEject(true);
            m_bEjectByError = true;
        }
    }
}
#endif
// } modified by cychoi 2014.04.14

#if _DECK_DIAGNOSTIC_
void DeckManager::StartDiagnostic()
{
    emit OnCommErrorWithDeckChanged(m_bDiagCommError);
}
#endif

void DeckManager::SetStartPlayTime(int nPlaybackTime)
{
    m_nStartPlayTime = nPlaybackTime / 1000;
}

void DeckManager::SetLifeTime(int nPlaybackTime)
{
    int nTime = (nPlaybackTime / 1000) - m_nStartPlayTime;

    m_nStartPlayTime = (nPlaybackTime / 1000);
    
    if(nTime < 0) {
        MC_LOG << "DeckManager::SetLifeTime(): m_nStartPlayTime: " << m_nStartPlayTime << ", nPlaybackTime: " << nPlaybackTime << LOG_ENDL;
        return;
    }
    
    m_nLifeTime += nTime;
}

int DeckManager::GetLifeTime()
{
    return m_nLifeTime;
}

void DeckManager::EmitDiscStateChanged(int nState)
{
    MC_LOG << "DeckManager::EmitDiscStateChanged(" << nState << ")" << LOG_ENDL;
    m_nLastDiscState = nState;
    emit OnDiscStateChanged(nState);
}

// { added by cychoi 2013-06-09 for ITS 172825 eject failure on FF_REW
int DeckManager::GetDiscState()
{
    return m_nLastDiscState;
}
// } added by cychoi 2013-06-09

void DeckManager::HandlePacketReceived()
{
#if _DECK_DIAGNOSTIC_
    m_nDiagResCount++;
#endif

    PacketBuffer *pPacketBuffer = PacketBuffer::GetInstance();
    
    if(pPacketBuffer->TakePacket(&m_tStatusData) == false)  return;

    if(ExtractPictureSettingStatus(&m_tStatusData) == true) return;
    if(ExtractInitSetting1Status(&m_tStatusData) == true)   return;
    if(ExtractInitSetting2aStatus(&m_tStatusData) == true)  return;
    if(ExtractInitSetting3Status(&m_tStatusData) == true)   return;
    // { added by cychoi 2013.06.20 for SW reset on disc loading
#if _DECK_DIAGNOSTIC_
    if(ExtractResetStart(&m_tStatusData) == true)
    {
        return;
    }
#endif
    // } added by cychoi 2013.06.20

    if(!m_pMedia) {
        MC_HIGH << "m_pMedia is NULL" << LOG_ENDL;
        m_pMedia = GetMediaInstance();
    }
    
    // 공용 Packet 처리.
    if(ExtractCommonStatus(&m_tStatusData) == true) 
    {
        if(m_nInitSetting == 0) {
            RequestInitSetting();
        }

        if(m_bReloadAfterError == true && m_bReloadDisc == true)
        {
            if(m_tDeckStatus.m_nDiscPos == eDM_DISC_POS_LOADING_COMPLETE && IsEject() == true) 
            {
                MC_HIGH << "DeckManager::HandlePacketReceived() go to error reloaded" << LOG_ENDL;
                SetEject(false);
                m_bEjectByError   = false;
                m_nDiscEjectCount = 0;
                m_nDiscMechanicEjectCount = 0; // added by cychoi 2013.07.09 for auto loading after 3 times eject failure by other method
                m_nDeviceState    = DiscError;
                EmitDiscStateChanged(PLAYER_DISCRELOADED);
                return;
            }
            else if(m_tDeckStatus.m_nDiscPos != eDM_DISC_POS_LOADING_COMPLETE && IsEject() == false) 
            {
                MC_HIGH << "DeckManager::HandlePacketReceived() return from error reloaded" << LOG_ENDL;
                m_bReloadDisc = false;
                m_bReloadAfterError = false;
            }
        }

        if(CheckError() == true) 
        {
            MC_HIGH << "DeckManager::CheckError() == true. " << m_nError << ", " << m_bReloadAfterError << ", " << m_bReloadDisc << LOG_ENDL;
            switch(m_nError) 
            {
            case PLAYER_NOMEDIA: 
            case PLAYER_DISCCORRUPTED:
            case PLAYER_DISC_READ_ERROR:
            case PLAYER_DISC_REGION_ERROR:
                if(m_bReloadAfterError == false || m_bReloadDisc == false) {
                    // removed by cychoi 2013.08.10 for unused code
                    DeckManager::GetInstance()->Eject(false);
                    SetEject(true);
                    m_bEjectByError = true;
                }
                break;
            case PLAYER_DISC_MECHANIC_ERROR:
                if(m_bReloadAfterError == false || m_bReloadDisc == false) {
                    // removed by cychoi 2013.08.10 for unused code
                    DeckManager::GetInstance()->Eject(true);
                    SetEject(true);
                    m_bEjectByError = true;
                }
                break;
            case PLAYER_HIGHTEMPERATURE:
                m_pPacket->Stop();
                break;
            case PLAYER_DISC_LOW_CONSUMPTION:
            case PLAYER_DISC_PARENTAL_ERROR:
                m_pPacket->Stop();
                break;
            default:
                break;
            }
            
            return;
        }

        MC_HIGH << "DeckManager:: prev(" << m_tPrevDeckStatus.m_nDiscPos << "-" << m_tPrevDeckStatus.m_nLoadingDir << ")"
                << ", curr(" << m_tDeckStatus.m_nDiscPos << "-" << m_tDeckStatus.m_nLoadingDir << "). media: " << m_tDeckStatus.m_nMediaMode << LOG_ENDL;

        // loading 또는 ejecting 중인 경우.
        if(m_tDeckStatus.m_nDiscPos == eDM_DISC_POS_LOADING_EJECTING) {
            if(m_tPrevDeckStatus.m_nDiscPos != eDM_DISC_POS_LOADING_EJECTING) {
                if(m_tDeckStatus.m_nLoadingDir == eDM_DIR_DURING_LOADING) {
                    if(m_tReloadTimer.isActive()) {
                        MC_HIGH << "DeckManager::Removed Reload Timer...1" << LOG_ENDL;
                         m_tReloadTimer.stop();
                        m_bReloadDisc = false;
                    }
                    if(m_bReloadDisc == true) {
                        EmitDiscStateChanged(PLAYER_DISCRELOADING);
                    }else{
                        EmitDiscStateChanged(PLAYER_DISCLOADING);
                    }
                }
                else {
                    if(IsEject() == false) {
                        MC_HIGH << "DeckManager::Eject flag is false. for eject, other methods is used" << LOG_ENDL;
                        SetEject(true);
                        // { added by cychoi 2013.07.09 for auto loading after 3 times eject failure by other method
                        m_nDiscMechanicEjectCount++;
                        MC_HIGH << "DeckManager::m_nDiscMechanicEjectCount(" << m_nDiscMechanicEjectCount << ")" << LOG_ENDL;
                        // { added by cychoi 2013.07.09
                    }
                }
            }

            // { added by kihyung 2013.01.03 for check disc eject error
            if(m_tDeckStatus.m_nLoadingDir == eDM_DIR_DURING_LOADING)
            {
                if(m_nDiscEjectCount >= 3) {
                    MC_LOG << "DeckManager::m_nDiscEjectCount(" << m_nDiscEjectCount << ") ---> Load()" << LOG_ENDL;
                    m_nDiscEjectCount = 0;
                    m_bReloadDisc     = true;
                    m_pPacket->Loading();
                }
                // { added by cychoi 2013.07.09 for auto loading after 3 times eject failure by other method
                else if(m_nDiscMechanicEjectCount >= 3) {
                    MC_LOG << "DeckManager::m_nDiscMechanicEjectCount(" << m_nDiscMechanicEjectCount << ") ---> Load()" << LOG_ENDL;
                    m_nDiscMechanicEjectCount = 0;
                    m_bReloadDisc     = true;
                    m_pPacket->Loading();
                }
                // } added by cychoi 2013.07.09
            }
            // } added by kihyung 2013.01.03
        }
        // ejecting 완료.
        else if(m_tDeckStatus.m_nDiscPos == eDM_DISC_POS_EJECT_COMPLETE) {
            if(m_tPrevDeckStatus.m_nDiscPos != eDM_DISC_POS_EJECT_COMPLETE) {
                //if(IsEject() == true && m_bEjectByError == false) {
                if(IsEject() == true) {
                    if(m_tReloadTimer.isActive()) {
                        MC_HIGH << "DeckManager::Removed Reload Timer...2" << LOG_ENDL;
                        m_tReloadTimer.stop();
                    }
                    MC_HIGH << "DeckManager::Start Reload Timer..." << LOG_ENDL;
                    m_tReloadTimer.start(10000);
                    m_bReloadDisc = true;
                }
                EmitDiscStateChanged(PLAYER_DISCEJECTED);
                m_nDiscEjectCount = 0;                
                m_nDiscMechanicEjectCount = 0; // added by cychoi 2013.07.09 for auto loading after 3 times eject failure by other method
            }
            return;
        }
        // loading이 완료 안되었으면.
        else if(m_tDeckStatus.m_nDiscPos != eDM_DISC_POS_LOADING_COMPLETE) {
            // disc가 deck에서 제거되었으면.
            if(m_tDeckStatus.m_nDiscPos == eDM_DISC_POS_NO_DISC && m_tPrevDeckStatus.m_nDiscPos != eDM_DISC_POS_NO_DISC)
            {
                MC_HIGH << "DeckManager::Removed Disc from Deck." << LOG_ENDL;
                if(m_tReloadTimer.isActive()) {
                    MC_HIGH << "DeckManager::Removed Reload Timer...3" << LOG_ENDL;
                    m_tReloadTimer.stop();
                    m_bReloadDisc = false;
                }
                if(m_nLastDiscState != PLAYER_DISCEJECTED) {
                    EmitDiscStateChanged(PLAYER_DISCEJECTING);
                    EmitDiscStateChanged(PLAYER_DISCEJECTED);
                }
                EmitDiscStateChanged(PLAYER_DISCREMOVED);
            }
            // { rollback by cychoi 2013.07.12 for ITS 178539 No Disc LED On
            else if(m_tDeckStatus.m_nDiscPos == eDM_DISC_POS_NO_DISC && m_nInitSetting == 4) 
            {
                m_nInitSetting++;
                MC_LOG << "DeckManager::No disc in Booting..." << LOG_ENDL;
                EmitDiscStateChanged(PLAYER_DISCREMOVED);
            }
            // } rollback by cychoi 2013.07.12
            return;
        }
        // loading이 완료되었으면.
        else if(m_tDeckStatus.m_nDiscPos      == eDM_DISC_POS_LOADING_COMPLETE && 
                m_tPrevDeckStatus.m_nDiscPos  != eDM_DISC_POS_LOADING_COMPLETE &&
                IsEject() == true) 
        {
            Init();
            if(m_bReloadDisc == true) {
                m_bReloadDisc = false;
                // { commented by cychoi 2013.06.06 for ITS 169804 LED On/Off
                //if(m_nLastDiscState != PLAYER_DISCEJECTED) {
                //    EmitDiscStateChanged(PLAYER_DISCEJECTED);
                // } commented by cychoi 2013.06.06
                EmitDiscStateChanged(PLAYER_DISCRELOADED);
            }
            else{
                EmitDiscStateChanged(PLAYER_DISCLOADED);
            }
            SetEject(false);
            m_bEjectByError   = false;
            m_nDiscEjectCount = 0;
        }
        // { added by kihyung 2013.01.03 for check disc eject error
        else if(m_tDeckStatus.m_nDiscPos    == eDM_DISC_POS_LOADING_COMPLETE &&
                m_tDeckStatus.m_nLoadingDir == eDM_DIR_DURING_EJECTION) 
        {
            m_nDiscEjectCount++;
            MC_HIGH << "DeckManager::m_nDiscEjectCount(" << m_nDiscEjectCount << ")" << LOG_ENDL;
        }
        // } added by kihyung 2013.01.03

        if(IsEject() == false && m_bLoadComplete == false) {
            if(m_tDeckStatus.m_nDiscPos == eDM_DISC_POS_LOADING_COMPLETE && m_tDeckStatus.m_nMediaMode != eDM_MODE_NONE) {
                if(m_nInitSetting >= 4) {
                    StartMedia();
                }
            }
        }
        // { modified by cychoi 2013.06.20 for SW reset on disc loading
        else if(IsEject() == true && m_bLoadComplete == false && GetResetRestart() == true) {
            if(m_tDeckStatus.m_nDiscPos == eDM_DISC_POS_LOADING_COMPLETE && m_tDeckStatus.m_nMediaMode != eDM_MODE_NONE) {
                SetEject(false);
                EmitDiscStateChanged(PLAYER_DISCLOADING);
                EmitDiscStateChanged(PLAYER_DISCLOADED);
                if(m_nInitSetting >= 4) {
                    StartMedia();
                }
            }
        }
        // } modified by cychoi 2013.06.20
        
        return;
    }

    if(IsEject() == true) return;

    // media mode별 packet 처리.
    ProcessPacket(&m_tStatusData);
}


void DeckManager::HandleReload()
{
    MC_HIGH << "DeckManager:: Auto Reload..." << LOG_ENDL;
    m_tReloadTimer.stop();
    m_pPacket->Loading();
    m_bReloadAfterError = m_bEjectByError;
}

// { added by cychoi 2013.10.04 for Disc Detect Performance on Boot-Up
void DeckManager::HandleCommonStatus()
{
    // { added by cychoi 2013.10.19 for Common status request limitation
    if(m_nCommonStatusReqCount > 3)
    {
        InitCommonStatusTimer();
        MC_HIGH << "DeckManager::HandleCommonStatus(): RequestCommonStatus() limitation over." << LOG_ENDL;
        EmitError(PLAYER_FILENOTFOUND); // added by cychoi 2014.05.22 for Deck is not connected
        return;
    }
    // } added by cychoi 2013.10.19

    if(m_pPacket->RequestCommonStatus() > 0)
    {
        InitCommonStatusTimer();
        MC_LOG << "DeckManager::HandleCommonStatus(): RequestCommonStatus() success." << LOG_ENDL;
    }
    else
    {
        MC_LOG << "DeckManager::HandleCommonStatus(): RequestCommonStatus() failure." << LOG_ENDL;
        m_nCommonStatusReqCount++; // added by cychoi 2013.10.19 for Common status request limitation
    }
}
// } added by cychoi 2013.10.04

#ifdef DECK_DISC_DETECTION_ERR_HANDLING
void DeckManager::HandleDiscDetectLimitation()
{
    MC_HIGH << "DeckManager::HandleDiscDetectLimitation" << LOG_ENDL;
    // Please handle as Error Disc if Deck does not detect Disc during 60 secondes
    EmitError(PLAYER_NOMEDIA);
    m_nError = PLAYER_NOMEDIA;

    if(m_bReloadAfterError == false || m_bReloadDisc == false)
    {
        DeckManager::GetInstance()->Eject(false);
        SetEject(true);
        m_bEjectByError = true;
    }
}
#endif

#if _DECK_DIAGNOSTIC_
void DeckManager::HandleDiagnostic()
{
    int bSend = false;

    // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(IsDeckBlocked() == true)
    {
        MC_LOG << "Diagnostic blocked" << LOG_ENDL;
        if(m_bDiagReqSend == true)
        {
            m_bDiagReqSend = false;
            if(m_bDiagCommError == true)
            {
                m_bDiagCommError = false;
                emit OnCommErrorWithDeckChanged(m_bDiagCommError);
            }
        }
        // Deck operation was blocked, just return
        return;
    }
    // } added by cychoi 2014.04.09

    // Deck으로부터 일반적인 Respone가 오고 있지 않은 경우.
    if(m_bDiagReqSend == false && m_nDiagResCount == 0) 
    {
        // MC_HIGH << "RequestCommonStatus()" << LOG_ENDL;
        m_bDiagReqSend = true;
        m_pPacket->RequestCommonStatus();
    }
    // Respone를 요청했는데, 응답이 없는 경우.
    else if(m_bDiagReqSend == true && m_nDiagResCount == 0) 
    {
        // MC_HIGH << "There is no response..." << LOG_ENDL;
        m_bDiagReqSend = false;
        if(m_bDiagCommError == false) {
            m_bDiagCommError = true;
            emit OnCommErrorWithDeckChanged(m_bDiagCommError);
            bSend = true;
        }
    }
    else 
    {
        // MC_HIGH << "m_nDiagResCount: " << m_nDiagResCount << LOG_ENDL;
        if(m_bDiagCommError == true) {
            m_bDiagCommError = false;
            emit OnCommErrorWithDeckChanged(m_bDiagCommError);
            bSend = true;
        }
        m_nDiagResCount = 0;
    }

    m_nDiagTimerCallCount++;
    if(m_nDiagTimerCallCount >= 10) {
        m_nDiagTimerCallCount = 0;
        if(bSend == false) {
            emit OnCommErrorWithDeckChanged(m_bDiagCommError);
        }
    }
}
#endif    

DeckManager::DeckManager(QObject *parent) : QObject(parent),
m_pPacket(0),
m_pMedia(0),
m_pGPlayer(0),
m_nInitSetting(0),
m_bEject(true),
m_bEjectByError(false),
m_bReloadAfterError(false),
m_bLoadComplete(false),
m_nError(PLAYER_NO_ERROR),
m_nDeviceState(DiscNotPresent),
m_bReloadDisc(false),
// removed by cychoi 2013.08.10 for unused code
m_nScanMode(SCANOFF),
m_nRandomMode(RANDOMOFF),
m_nRepeatMode(REPEATALL),
#if _DECK_DIAGNOSTIC_  
m_bDiagCommError(false),
m_bDiagReqSend(false),
m_nDiagResCount(0),
m_nDiagTimerCallCount(0),
m_bResetRestart(false), // added by cychoi 2013.06.20 for SW reset on disc loading
#endif
m_nStartPlayTime(0),
m_nLifeTime(0),
m_nLastDiscState(PLAYER_DISCEJECTED),
m_nDiscMechanicEjectCount(0), // moved by cychoi 2013.07.19 for remove compile warning // added by cychoi 2013.07.09 for auto loading after 3 times eject failure by other method
m_nDiscEjectCount(0),
m_nDiscLoadCount(0)
{
    m_tDeckStatus     = DeckCommonStatus();
    m_tPrevDeckStatus = DeckCommonStatus();
    
    m_tDeckStatus.m_nMediaMode = eDM_MODE_NONE;
    
    PacketBuffer *pPacketBuffer = PacketBuffer::GetInstance();
    connect(pPacketBuffer, SIGNAL(NewPacketArrived()), this, SLOT(HandlePacketReceived()));

    m_pMedia   = NullManager::GetInstance();

    // #if USE_ENGINE_SINGLE_INSTANCE
    // m_pGPlayer = GPlayerEngine::GetInstance();
    // #else
    m_pGPlayer = new GPlayerEngine();
    // #endif
    
    connect(&m_tReloadTimer, SIGNAL(timeout()), this, SLOT(HandleReload()));
    connect(&m_tCommonStatusTimer, SIGNAL(timeout()), this, SLOT(HandleCommonStatus())); // added by cychoi 2013.10.04 for Disc Detect Performance on Boot-Up
#ifdef DECK_DISC_DETECTION_ERR_HANDLING
    m_tDiscDetectLimitationTimer.setSingleShot(true);
    m_tDiscDetectLimitationTimer.setInterval(30000);
    connect(&m_tDiscDetectLimitationTimer, SIGNAL(timeout()), this, SLOT(HandleDiscDetectLimitation()));
    m_nDiscDetectRetryCount = 0;
    // { added by cychoi 2014.03.10 for ISV 98219
    m_nDiscReadRetryCount = 0;
    m_nDiscDetectReadRetryCount = 0;
    // } added by cychoi 2014.03.10
#endif

#if _DECK_DIAGNOSTIC_
    m_tDiagTimer.setInterval(15000); // added by cychoi 2014.01.28 for init timeout to 15s on Constructor
    connect(&m_tDiagTimer, SIGNAL(timeout()), this, SLOT(HandleDiagnostic()));
    //m_tDiagTimer.start(6000);
    //MC_HIGH << "DiagTimer connected." << LOG_ENDL;
#endif

    PrintVersion();

    // { added by cychoi 2013-07-15 for Prevent fix
    m_tStatusData.m_nFH   = 0;
    m_tStatusData.m_nFDL  = 0;
    m_tStatusData.m_nCode = 0;
    m_tStatusData.m_Rsvd  = 0;
    m_tStatusData.m_nFCC  = 0;
    m_nActiveCond         = eDM_DISC_ACTIVE_COND_STOP;
    m_bCDText             = 0;
    m_bMixedCD            = 0;
    m_eMixedCDMode        = eDM_SWITCH_CD_DA_MODE;
    // } added by cychoi 2013-07-15
    m_nDVDMediaMode       = eDM_MODE_NONE; // added by cychoi 20103.09.26 for ITS 188493 DVD Navigation Still Picture
    m_nCDDAMediaMode      = eDM_MODE_NONE; // added by cychoi 20103.10.10 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
    m_nCommonStatusReqCount = 0; // added by cychoi 2013.10.19 for Common status request limitation
}

void DeckManager::PrintVersion()
{
    MC_LOG << "DECK VER: " << LGE_CONTROLLER_VER << " " << DECK_CONTROLLER_VER << LOG_ENDL;
}

void DeckManager::Init()
{
    MC_LOG << "DeckManager::Init()" << LOG_ENDL;

    m_bLoadComplete = false;
    m_bMixedCD      = false;
    
    switch(m_tDeckStatus.m_nMediaMode)
    {
    case eDM_MODE_DVD_VIDEO:
    case eDM_MODE_DVD_AUDIO:
        DVDManager::GetInstance()->Init();
        break;
    case eDM_MODE_CD_DA:
    case eDM_MODE_DTS_CD:
        CDDAManager::GetInstance()->Init();
        break;        
    case eDM_MODE_VIDEO_CD_1_1:
    case eDM_MODE_VIDEO_CD_2_0:
    case eDM_MODE_VIDEO_CD_SVCD:
        VideoCDManager::GetInstance()->Init();
        break;        
    case eDM_MODE_COMP_AUDIO:
        CompAudioManager::GetInstance()->Init();
        break;        
    default:
        NullManager::GetInstance()->Init();
    }
}

void DeckManager::RequestInitSetting()
{
    if(m_nInitSetting == 0) {
        m_nInitSetting = 1;
        m_pPacket->RequestInitialSetting1Status();
    }
}

void DeckManager::StartMedia()
{
    MC_LOG << "DeckManager::StartMedia() m_nInitSetting: " << m_nInitSetting << LOG_ENDL;
    
    m_bLoadComplete = true;

    InitCommonStatusTimer(); // added by cychoi 2013.10.04 for Disc Detect Performance on Boot-Up

    m_tPrevDeckStatus = DeckCommonStatus();

    switch(m_tDeckStatus.m_nMediaMode)
    {
    case eDM_MODE_DVD_VIDEO:
    case eDM_MODE_DVD_AUDIO:
        DVDManager::GetInstance()->StartMedia();
        break;
    case eDM_MODE_CD_DA:
    case eDM_MODE_DTS_CD:
        CDDAManager::GetInstance()->StartMedia();
        break;        
    case eDM_MODE_VIDEO_CD_1_1:
    case eDM_MODE_VIDEO_CD_2_0:
    case eDM_MODE_VIDEO_CD_SVCD:
        VideoCDManager::GetInstance()->StartMedia();
        break;        
    case eDM_MODE_COMP_AUDIO:
        CompAudioManager::GetInstance()->StartMedia();
        break;        
    default:
        NullManager::GetInstance()->StartMedia();
    }
}

void DeckManager::EndMedia()
{
    switch(m_tDeckStatus.m_nMediaMode)
    {
    case eDM_MODE_DVD_VIDEO:
    case eDM_MODE_DVD_AUDIO:
        DVDManager::GetInstance()->EndMedia();
        break;
    case eDM_MODE_CD_DA:
    case eDM_MODE_DTS_CD:
        CDDAManager::GetInstance()->EndMedia();
        break;        
    case eDM_MODE_VIDEO_CD_1_1:
    case eDM_MODE_VIDEO_CD_2_0:
    case eDM_MODE_VIDEO_CD_SVCD:
        VideoCDManager::GetInstance()->EndMedia();
        break;        
    case eDM_MODE_COMP_AUDIO:
        CompAudioManager::GetInstance()->EndMedia();
        break;        
    default:
        NullManager::GetInstance()->EndMedia();
    }
}

void DeckManager::ProcessPacket(StatusData *pPacket)
{
    switch(m_tDeckStatus.m_nMediaMode)
    {
    case eDM_MODE_DVD_VIDEO:
    case eDM_MODE_DVD_AUDIO:
        DVDManager::GetInstance()->ProcessPacket(pPacket);
        break;
    case eDM_MODE_CD_DA:
    case eDM_MODE_DTS_CD:
        CDDAManager::GetInstance()->ProcessPacket(pPacket);
        break;        
    case eDM_MODE_VIDEO_CD_1_1:
    case eDM_MODE_VIDEO_CD_2_0:
    case eDM_MODE_VIDEO_CD_SVCD:
        VideoCDManager::GetInstance()->ProcessPacket(pPacket);
        break;        
    case eDM_MODE_COMP_AUDIO:
        CompAudioManager::GetInstance()->ProcessPacket(pPacket);
        break;        
    default:
        NullManager::GetInstance()->ProcessPacket(pPacket);
    }
}

int DeckManager::CheckError()
{
    // { modified by cychoi 2013.10.17 for ITS 196656 & ITS 194618
    int bError = false;
    int nError = PLAYER_NO_ERROR;
    
    if(m_tDeckStatus.m_nHighTemp == eDM_HIGH_TEMP_STOP) {
        if(m_tPrevDeckStatus.m_nHighTemp != eDM_HIGH_TEMP_STOP)
        {
            MC_HIGH << "DeckManager::PLAYER_HIGHTEMPERATURE" << LOG_ENDL;
            bError = true;
            EmitError(PLAYER_HIGHTEMPERATURE);
        }
        nError = PLAYER_HIGHTEMPERATURE;
    }

    if(m_tDeckStatus.m_nReadError == eDM_READ_ERROR_STOP) {
        if(m_tPrevDeckStatus.m_nReadError != eDM_READ_ERROR_STOP)
        {
            MC_HIGH << "DeckManager::PLAYER_DISC_READ_ERROR" << LOG_ENDL;
            bError = true;
            EmitError(PLAYER_DISC_READ_ERROR);
        }
        nError = PLAYER_DISC_READ_ERROR;
    }

    if(m_tDeckStatus.m_nMechaError == eDM_MECHA_ERROR_STOP) {
        if(m_tPrevDeckStatus.m_nMechaError != eDM_MECHA_ERROR_STOP)
        {
            MC_HIGH << "DeckManager::PLAYER_DISC_MECHANIC_ERROR" << LOG_ENDL;
            bError = true;
            EmitError(PLAYER_DISC_MECHANIC_ERROR);
        }
        nError = PLAYER_DISC_MECHANIC_ERROR;
    }

    if(m_tDeckStatus.m_nConsumption == eDM_CONSUMPTION_LOW) {
        if(m_tPrevDeckStatus.m_nConsumption != eDM_CONSUMPTION_LOW)
        {
            MC_HIGH << "DeckManager::PLAYER_DISC_LOW_CONSUMPTION" << LOG_ENDL;
            bError = true;
            EmitError(PLAYER_DISC_LOW_CONSUMPTION);
        }
        nError = PLAYER_DISC_LOW_CONSUMPTION;
    }

    if(m_tDeckStatus.m_nRegionError == eDM_REGION_ERROR) {
        if(m_tPrevDeckStatus.m_nRegionError != eDM_REGION_ERROR)
        {
            MC_HIGH << "DeckManager::PLAYER_DISC_REGION_ERROR" << LOG_ENDL;
            bError = true;
            EmitError(PLAYER_DISC_REGION_ERROR);
        }
        nError = PLAYER_DISC_REGION_ERROR;
    }

    if(m_tDeckStatus.m_nParental == eDM_PARENTAL_ERROR) {
        if(m_tPrevDeckStatus.m_nParental != eDM_PARENTAL_ERROR)
        {
            MC_HIGH << "DeckManager::PLAYER_DISC_PARENTAL_ERROR" << LOG_ENDL;
            bError = true;
            EmitError(PLAYER_DISC_PARENTAL_ERROR);
        }
        nError = PLAYER_DISC_PARENTAL_ERROR;
    }

    if(m_tDeckStatus.m_nAllUnsupport == eDM_NO_FILE_RELEVANT) {
        if(m_tPrevDeckStatus.m_nAllUnsupport != eDM_NO_FILE_RELEVANT)
        {
            MC_HIGH << "DeckManager::PLAYER_NOMEDIA" << LOG_ENDL;
            bError = true;
            // { modified by kihyung 2012.07.11 for CR 7947
            // EmitError(PLAYER_NOMEDIA);
            EmitError(PLAYER_DISCCORRUPTED);
            // } modified by kihyung
        }
        nError = PLAYER_DISCCORRUPTED;
    }

    if(m_tDeckStatus.m_nDiscDetect == eDM_DISC_DETECT_NA_FAILED) {
        if(m_tPrevDeckStatus.m_nDiscDetect != eDM_DISC_DETECT_NA_FAILED)
        {
            MC_HIGH << "DeckManager::PLAYER_NOMEDIA: eDM_DISC_DETECT_NA_FAILED" << LOG_ENDL;
            bError = true;
            // { modified by kihyung 2012.08.28 for CR 12993
            // { modified by kihyung 2012.07.11 for CR 7947
            // EmitError(PLAYER_NOMEDIA);
            //EmitError(PLAYER_DISCCORRUPTED);
            // } modified by kihyung
            EmitError(PLAYER_DISC_READ_ERROR);
            // } modified by kihyung
        }
        nError = PLAYER_DISC_READ_ERROR;
    }

    //if(m_tDeckStatus.m_nDRMUnsupport == eDM_ALL_DRM_FILES) {
    //    MC_LOG << "DeckManager::PLAYER_DISC_DRM_ALL_UNSUPPORTED" << LOG_ENDL;
    //    EmitError(PLAYER_DISC_DRM_ALL_UNSUPPORTED);
    //}

    // 미지원 미디어.
    if(IsSupportMediaMode(m_tDeckStatus.m_nMediaMode) == false && m_tDeckStatus.m_nDiscDetect == eDM_DISC_DETECT_COMPLETED) 
    {
#ifdef DECK_DISC_DETECTION_ERR_HANDLING
        // { modified by cychoi 2014.01.16 for ITS 220385 Disc Detection Retry
        if((m_nError != PLAYER_DISCCORRUPTED) &&
           (m_tDeckStatus.m_nMediaMode != eDM_MODE_NONE ||
            m_nDiscDetectRetryCount >= 3))
        // } modified by cychoi 2014.01.16 for ITS 220385 Disc Detection Retry
#else
        if(m_nError != PLAYER_DISCCORRUPTED)
#endif
        {
            MC_HIGH << "DeckManager::PLAYER_NOMEDIA: MEIDA: " << m_tDeckStatus.m_nMediaMode << LOG_ENDL;
            bError = true;
            // { modified by kihyung 2012.08.28 for CR 12993
            // { modified by kihyung 2012.07.11 for CR 7947
            // EmitError(PLAYER_NOMEDIA);
            //EmitError(PLAYER_DISCCORRUPTED);
            // } modified by kihyung
            EmitError(PLAYER_NOMEDIA);
            // } modified by kihyung
            nError = PLAYER_NOMEDIA;
        }
    }

    // { modified by cychoi 2014.04.24 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(bError == false && nError == PLAYER_NO_ERROR)
    {
        if(m_nError != PLAYER_NO_ERROR)
        {
            EmitError(PLAYER_NO_ERROR);
        }
        m_nError = PLAYER_NO_ERROR;
    }
    // } modified by cychoi 2014.04.24

    if(nError == PLAYER_NO_ERROR)
    {
        if(m_nDeviceState == DiscError)
        {
            m_nDeviceState = DiscPresnt;
        }
    }
    // } modified by cychoi 2013.10.17

    return bError;
}

// { added by cychoi 2013.10.17 for ITS 196656 & ITS 194618
int DeckManager::GetCurrentError()
{
    int nError = PLAYER_NO_ERROR;
    
    // { added by cychoi 2014.04.25 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
    if(m_tDeckStatus.m_nHighTemp == eDM_HIGH_TEMP_STOP)
    {
        nError = PLAYER_HIGHTEMPERATURE;
    }
    // } added by cychoi 2014.04.25

    if(m_tDeckStatus.m_nReadError == eDM_READ_ERROR_STOP)
    {
        nError = PLAYER_DISC_READ_ERROR;
    }

    if(m_tDeckStatus.m_nRegionError == eDM_REGION_ERROR)
    {
        nError = PLAYER_DISC_REGION_ERROR;
    }

    if(m_tDeckStatus.m_nAllUnsupport == eDM_NO_FILE_RELEVANT)
    {
        nError = PLAYER_DISCCORRUPTED;
    }

    if(m_tDeckStatus.m_nDiscDetect == eDM_DISC_DETECT_NA_FAILED)
    {
        nError = PLAYER_DISC_READ_ERROR;
    }

    // 미지원 미디어.
    if(IsSupportMediaMode(m_tDeckStatus.m_nMediaMode) == false && m_tDeckStatus.m_nDiscDetect == eDM_DISC_DETECT_COMPLETED) 
    {
        if(nError != PLAYER_DISCCORRUPTED)
        {
            nError = PLAYER_NOMEDIA;
        }
    }

    return nError;
}
// } added by cychoi 2013.10.17

bool DeckManager::ExtractCommonStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_COMMON_STATUS) return false;

#if _DECK_DIAGNOSTIC_
    // { added by cychoi 2014.01.28 for set timeout to to 6s after received ExtractCommonStatus first time
    if(m_tDiagTimer.interval() == 15000)
    {
        //MC_HIGH << "DiagTimer restarted." << LOG_ENDL;
        m_tDiagTimer.start(6000);
    }
    // } added by cychoi 2014.01.28
    if(m_bDiagReqSend == true) {
        m_bDiagReqSend = false;
        if(m_bDiagCommError == true) {
            m_bDiagCommError = false;
            emit OnCommErrorWithDeckChanged(m_bDiagCommError);
        }
    }
#endif

    // MC_LOG << "DeckManager::ExtractCommonStatus" << LOG_ENDL;

    CommonStatus *pStatus = &pPacket->m_tData.m_tCommon;

    m_tPrevDeckStatus = m_tDeckStatus;

    m_tDeckStatus.m_nDiscPos      = (DECK_DISC_POSITION)pStatus->nDiscPosition;
    m_tDeckStatus.m_nWorkMecha    = (DECK_WORKING_MECHANIC)pStatus->nWorkingMecha;
    m_tDeckStatus.m_nLoadingDir   = (DECK_LOADING_DIR)pStatus->nEjectLoading;
    m_tDeckStatus.m_nReadError    = (DECK_READ_ERROR)pStatus->nReadError;
    m_tDeckStatus.m_nMechaError   = (DECK_MECHANIC_ERROR)pStatus->nMechaError;
    m_tDeckStatus.m_nHighTemp     = (DECK_HIGH_TEMP_DETECT)pStatus->nDetectHighTemp;
    m_tDeckStatus.m_nAtapiBus     = (DECK_ATAPI_BUS)pStatus->nATAPI;
    m_tDeckStatus.m_nConsumption  = (DECK_LOW_CONSUMPTION_MODE)pStatus->nLowConsumptionMode;
    m_tDeckStatus.m_nRegionError  = (DECK_REGION_ERROR)pStatus->nRegionError;
    m_tDeckStatus.m_nParental     = (DECK_PARENTAL_ERROR)pStatus->nParentalError;
    m_tDeckStatus.m_nBusPrepare   = (DECK_BUS_IN_PREPARATION)pStatus->nBusInPreparation;
    m_tDeckStatus.m_nAllUnsupport = (DECK_ALL_UNSUPPORTED)pStatus->nAllUnsupported;
    m_tDeckStatus.m_nDRMUnsupport = (DECK_DRM_ALL_UNSUPPORTED)pStatus->nDRMAllUnsupported;
    m_tDeckStatus.m_nDiscDetect   = (DECK_DISC_IDENTIFIECATION)pStatus->nIdentification;
    m_tDeckStatus.m_nDispScreen   = (DECK_DISPLAY_SCREEN_MODE)pStatus->nDisplay;
    m_tDeckStatus.m_nDeckVerUp    = (DECK_VERSION_UPGRADE)pStatus->nDeckVerUp;
    m_tDeckStatus.m_nVideoFormat  = (DECK_VIDEO_FORMAT)pStatus->nVideoFormat;
    m_tDeckStatus.m_nJolietFormat = (DECK_JOLIET_FORMAT)pStatus->nJoliet;
    m_tDeckStatus.m_nTemperature  = pStatus->nTempCode;

    char pFirmVer[5] = {pStatus->nDeckVerNo1, pStatus->nDeckVerNo2, pStatus->nDeckVerNo3, pStatus->nDeckVerNo4, 0};
    int  nFirmVer = atoi(pFirmVer);
    if(m_tDeckStatus.m_nDeckVerNo != (UINT4)nFirmVer) {
        m_tDeckStatus.m_nDeckVerNo = nFirmVer;
        MC_LOG << "DECK FIRMWARE VERSION: " << m_tDeckStatus.m_nDeckVerNo << LOG_ENDL;
    }

    // removed by cychoi 2013.08.10 for unused code

    DECK_MEDIA_MODE eMediaMode = eDM_MODE_NONE;
    
    if(pStatus->nCDDA) {
        eMediaMode = eDM_MODE_CD_DA;
    }
    else if(pStatus->nCompAudio) {
        eMediaMode = eDM_MODE_COMP_AUDIO;
    }
    else if(pStatus->nVideoCD == 0x01) {
        eMediaMode = eDM_MODE_VIDEO_CD_1_1;
    }
    else if(pStatus->nVideoCD == 0x02) {
        eMediaMode = eDM_MODE_VIDEO_CD_2_0;
    }
    else if(pStatus->nVideoCD == 0x03) {
        eMediaMode = eDM_MODE_VIDEO_CD_SVCD;
    }
    else if(pStatus->nCDROM) {
        eMediaMode = eDM_MODE_CDROM;
    }
    else if(pStatus->nDVDVideo) {
        eMediaMode = eDM_MODE_DVD_VIDEO;
    }
    else if(pStatus->nDTSCD) {
        eMediaMode = eDM_MODE_DTS_CD;
    }
    else if(pStatus->nDVDROM) {
        eMediaMode = eDM_MODE_DVD_ROM;
    }
    else if(pStatus->nDVDAudio) {
        eMediaMode = eDM_MODE_DVD_AUDIO;
    }
    else if(pStatus->nDVDVR) {
        eMediaMode = eDM_MODE_DVD_VR;
    }

    if(pStatus->nCompAudio == 1 && pStatus->nCDDA == 1) {
        if(pStatus->nCDPriorityMode) {
            eMediaMode     = eDM_MODE_COMP_AUDIO;
            m_eMixedCDMode = eDM_SWITCH_CD_DA_MODE;
            MC_LOG << "DeckManager:: MIXED CD(" << pStatus->nCDPriorityMode << ") PRIORITY CA" << LOG_ENDL;
        }
        else {
            eMediaMode     = eDM_MODE_CD_DA;
            m_eMixedCDMode = eDM_SWITCH_COMP_AUDIO;
            MC_LOG << "DeckManager:: MIXED CD(" << pStatus->nCDPriorityMode << ") PRIORITY CDDA" << LOG_ENDL;
        }

        m_bMixedCD = true;
    }
    else {
        m_bMixedCD = false;
    }

    if(pStatus->nCDText) {
        m_bCDText = true;
    }

    // media mode.
    if(eMediaMode != m_tDeckStatus.m_nMediaMode) {
        if(m_bMixedCD == true) {
            m_bLoadComplete = false;
        }
        m_tDeckStatus.m_nMediaMode = eMediaMode;
        
        MC_LOG << "DeckManager::m_tDeckStatus.m_nMediaMode: " << m_tDeckStatus.m_nMediaMode << LOG_ENDL;
    }

    // { added by cychoi 20103.09.26 for ITS 188493 DVD Navigation Still Picture
    if(eMediaMode == eDM_MODE_DVD_VIDEO)
    {
        if(pStatus->nDVDAudio)
        {
            m_nDVDMediaMode = eDM_MODE_DVD_AUDIO;
        }
        else
        {
            m_nDVDMediaMode = eDM_MODE_DVD_VIDEO;
        }
    }
    // } added by cychoi 20103.09.26

    // { added by cychoi 20103.10.10 for ITS 167582 Invalid CDDA+VCD Mixed CD handling
    if(eMediaMode == eDM_MODE_CD_DA)
    {
        if(pStatus->nCDROM)
        {
            m_nCDDAMediaMode = eDM_MODE_CDROM;
        }
        else
        {
            m_nCDDAMediaMode = eDM_MODE_CD_DA;
        }
    }
    // } added by cychoi 20103.10.10

#ifdef DECK_DISC_DETECTION_ERR_HANDLING
    if(m_tPrevDeckStatus.m_nDiscDetect == eDM_DISC_DETECT_NO_EXEUTED &&
       m_tDeckStatus.m_nDiscDetect == eDM_DISC_DETECT_DURING_COURSE)
    {
        if(m_tDiscDetectLimitationTimer.isActive() == false)
        {
            MC_LOG << "DeckManager::ExtractCommonStatus m_tDiscDetectLimitationTimer start" << LOG_ENDL;
            m_tDiscDetectLimitationTimer.start();
        }
        m_nDiscDetectRetryCount = 0; // modified by cychoi 2014.01.16 for ITS 220385 Disc Detection Retry
    }
    else if(m_tPrevDeckStatus.m_nDiscDetect == eDM_DISC_DETECT_DURING_COURSE &&
            m_tDeckStatus.m_nDiscDetect == eDM_DISC_DETECT_COMPLETED)
    {
        if(m_tDiscDetectLimitationTimer.isActive() == true)
        {
            MC_LOG << "DeckManager::ExtractCommonStatus m_tDiscDetectLimitationTimer stop" << LOG_ENDL;
            m_tDiscDetectLimitationTimer.stop();
        }
    }

    // { modified by cychoi 2014.01.16 for ITS 220385 Disc Detection Retry
    if(m_tDeckStatus.m_nDiscDetect == eDM_DISC_DETECT_COMPLETED)
    {
        HandleMediaModeError();

        HandleReadError(); // added by cychoi 2014.03.10 for ISV 98219
    }
    // } modified by cychoi 2014.01.16
    // { added by cychoi 2014.03.10 for ISV 98219
    else
    {
        HandleDetectReadError();
    }
    // } added by cychoi 2014.03.10
#endif

    return true;
}

bool DeckManager::ExtractPictureSettingStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_PICTURE_SETTING_STATUS) return false;

    MC_LOG << "DeckManager::ExtractPictureSettingStatus" << LOG_ENDL;

    PictureSettingStatus *pStatus = &pPacket->m_tData.m_tPictureSetting;

    m_tPicture.m_nContrastLevel     = pStatus->nContrast;
    m_tPicture.m_nColorLevel        = pStatus->nColor;
    m_tPicture.m_nTintLevel         = pStatus->nTint;
    m_tPicture.m_nBrightnessLevel   = pStatus->nBrightness;
    m_tPicture.m_nSharpnessLevel    = pStatus->nSharpness;
    m_tPicture.m_nGammaCorrection   = pStatus->nGammaCorrection;
    m_tPicture.m_nCoringFilter      = pStatus->nCoringFilter;
    m_tPicture.m_nBlackLevel        = pStatus->nBlackLevel;    
    m_tPicture.m_nWhiteLevel        = pStatus->nWhiteLevel;    
        
    return true;
}

bool DeckManager::ExtractInitSetting1Status(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_INIT_SET1_STATUS) return false;

    MC_LOG << "DeckManager::ExtractInitSetting1Status : m_nInitSetting: " << m_nInitSetting << LOG_ENDL;

    InitialSetting1Status *pStatus = &pPacket->m_tData.m_tInitSetting1;

    m_tInit1.m_bOSDOff              = pStatus->nOSD;
    m_tInit1.m_nRegionCode          = BCD2DEC1(pStatus->nRegionCode);
    m_tInit1.m_nParentalLevel       = BCD2DEC1(pStatus->nParentalLevel);
    m_tInit1.m_nScreenSize          = (DECK_SCREEN_SIZE)pStatus->nScreenSize;
    m_tInit1.m_nAngleDisplay        = (DECK_ANGLE_DISPLAY)pStatus->nAngleDisplay;
    m_tInit1.m_nSubTitleDisplay     = (DECK_SUBTITLE_DISPLAY)pStatus->nSubTitleDisplay;
    m_tInit1.m_nPinNumber           = BCD2DEC2(pStatus->nPinNumberUpper)*100    + BCD2DEC2(pStatus->nPinNumberLower);
    m_tInit1.m_nOpenningScreen      = (DECK_OPENING_DISP_SCREEN)pStatus->nOpenningScr;
    m_tInit1.m_nScreenMode          = (DECK_SCREEN_MODE)pStatus->nScreenMode;
    m_tInit1.m_nPictureMode         = (DECK_PICTURE_MODE)pStatus->nPictureMode;
    m_tInit1.m_nPBC                 = pStatus->nPBC;
    m_tInit1.m_nIRE                 = pStatus->nIRE;
    m_tInit1.m_nBackColor           = pStatus->nBackColor;
    m_tInit1.m_nCountryCode         = BCDTOISO3166(pStatus->nCountryCodeUpper, pStatus->nCountryCodeLower);
    m_tInit1.m_nPlayerMode          = (DECK_PLAYER_MODE)pStatus->nPlayerMode;
    m_tInit1.m_nVideoOutput         = (DECK_VIDEO_OUTPUT_FORMAT)pStatus->nVideoOutFormat;

    int bChanged = false;
    int nTemp = BCDTOISO639(pStatus->nSubTitleCodeUpper, pStatus->nSubTitleCodeLower);
    if(nTemp != m_tInit1.m_nPrioSubTitleCode) {
        if(pStatus->nSubTitleCodeUpper == 0xFF && pStatus->nSubTitleCodeLower == 0xFF) {
            MC_LOG << "DeckManager::ExtractInitSetting1Status() m_nPrioSubTitleCode: 0x" << nTemp << LOG_ENDL;
            nTemp = 0;
        }
        m_tInit1.m_nPrioSubTitleCode = nTemp;
        bChanged = true;
    }
    if(m_tInit1.m_nSubTitleDisplay == 1) {
        MC_LOG << "DeckManager::ExtractInitSetting1Status() m_tInit1.m_nSubTitleDisplay == 1" << LOG_ENDL;
        m_tInit1.m_nPrioSubTitleCode = 0;
    }
    
    nTemp = BCDTOISO639(pStatus->nAudioCodeUpper, pStatus->nAudioCodeLower);
    if(nTemp != m_tInit1.m_nPrioAudioCode) {
        m_tInit1.m_nPrioAudioCode = nTemp;
        bChanged = true;
    }
    
    nTemp = BCDTOISO639(pStatus->nMenuLangCodeUpper, pStatus->nMenuLangCodeLower);
    if(nTemp != m_tInit1.m_nPrioMenuLangCode) {
        m_tInit1.m_nPrioMenuLangCode = nTemp;
        bChanged = true;
    }

    if(bChanged == true) {
        MC_LOG << "DeckManager::ExtractInitSetting1Status() emit OnDeckPriorityLangChanged()" << LOG_ENDL;
        emit OnDeckPriorityLangChanged();
    }

    #if 0
    MC_LOG << "DeckManager::m_bOSDOff:                  " << m_tInit1.m_bOSDOff << LOG_ENDL;
    MC_LOG << "DeckManager::m_nRegionCode:              " << m_tInit1.m_nRegionCode << LOG_ENDL;
    MC_LOG << "DeckManager::m_nParentalLevel:           " << m_tInit1.m_nParentalLevel << LOG_ENDL;
    MC_LOG << "DeckManager::m_nPrioSubTitleCode:        " << m_tInit1.m_nPrioSubTitleCode << LOG_ENDL;
    MC_LOG << "DeckManager::m_nPrioAudioCode:           " << m_tInit1.m_nPrioAudioCode << LOG_ENDL;
    MC_LOG << "DeckManager::m_nPrioMenuLangCode:        " << m_tInit1.m_nPrioMenuLangCode << LOG_ENDL;
    MC_LOG << "DeckManager::m_nScreenSize:              " << m_tInit1.m_nScreenSize << LOG_ENDL;
    MC_LOG << "DeckManager::m_nAngleDisplay:            " << m_tInit1.m_nAngleDisplay << LOG_ENDL;
    MC_LOG << "DeckManager::m_nSubTitleDisplay:         " << m_tInit1.m_nSubTitleDisplay << LOG_ENDL;
    MC_LOG << "DeckManager::m_nPinNumber:               " << m_tInit1.m_nPinNumber << LOG_ENDL;
    MC_LOG << "DeckManager::m_nOpenningScreen:          " << m_tInit1.m_nOpenningScreen << LOG_ENDL;
    MC_LOG << "DeckManager::m_nScreenMode:              " << m_tInit1.m_nScreenMode << LOG_ENDL;
    MC_LOG << "DeckManager::m_nPictureMode:             " << m_tInit1.m_nPictureMode << LOG_ENDL;
    MC_LOG << "DeckManager::m_nPBC:                     " << m_tInit1.m_nPBC << LOG_ENDL;
    MC_LOG << "DeckManager::m_nIRE:                     " << m_tInit1.m_nIRE << LOG_ENDL;
    MC_LOG << "DeckManager::m_nBackColor:               " << m_tInit1.m_nBackColor << LOG_ENDL;
    MC_LOG << "DeckManager::m_nPlayerMode:              " << m_tInit1.m_nPlayerMode << LOG_ENDL;
    MC_LOG << "DeckManager::m_nVideoOutput:             " << m_tInit1.m_nVideoOutput << LOG_ENDL;
    #endif
    
    if(m_nInitSetting == 1) {
        m_nInitSetting = 2;
        /*
        // 2012.07.04
        // 초기 부팅시 SET에서 수행하도록 변경됨.
        if(m_tInit1.m_nCountryCode   != ISO3166TOBCD('K', 'R') || 
           m_tInit1.m_nBackColor     != 0 ||
           m_tInit1.m_nParentalLevel != 8 ||
           m_tInit1.m_bOSDOff        != 1 ||
           m_tInit1.m_nRegionCode    != 0)
        {    
            InitialSetting1Command tCmd;
            GetInitSetting1Cmd(tCmd);
            tCmd.nOSD        = 1;
            tCmd.nRegionCode = 0;
            m_pPacket->InitializationSetting1(&tCmd);
            MC_LOG << "DeckManager::m_pPacket->InitializationSetting1()" << LOG_ENDL;
        }
        */
        // } added by kihyung
        
        int nRegionCode = GetDVDRegionCodeFromKernel();
        if((nRegionCode >= 0 && nRegionCode != m_tInit1.m_nRegionCode) || 
            m_tInit1.m_nVideoOutput != 0x01 ||
            m_tInit1.m_nPBC         == 0    ||
            m_tInit1.m_nBackColor   != 0    ||
            m_tInit1.m_nPlayerMode  == 0    || // added by cychoi 2013.06.09 playback DVD-Video contents. (playback DVD-Audio contents in case of DVD-Audio contents only)
            m_tInit1.m_nSubTitleDisplay == 1 || // added by cychoi 2013.09.11 for ITS 186197 Always Subtitle On
            m_tInit1.m_bOSDOff      == 0) 
        {
            InitialSetting1Command tCmd;
            GetInitSetting1Cmd(tCmd);
            tCmd.nRegionCode   = nRegionCode;
            tCmd.nVideoOutType = 0x01;
            tCmd.nPBC          = 1;
            tCmd.nOSD          = 1;
            tCmd.nBackColor    = 0;
            tCmd.nPlayerMode   = 1; // added by cychoi 2013.06.09 playback DVD-Video contents. (playback DVD-Audio contents in case of DVD-Audio contents only)
            tCmd.nSubTitle     = 0; // added by cychoi 2013.09.11 for ITS 186197 Always Subtitle On
            m_pPacket->InitializationSetting1(&tCmd);
            MC_LOG << "DeckManager::m_pPacket->InitializationSetting1() for dvd region" << LOG_ENDL;
        }
        
        m_pPacket->RequestInitialSetting2aStatus();
    }

    return true;
}

bool DeckManager::ExtractInitSetting2aStatus(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_INIT_SET2A_STATUS) return false;

    MC_LOG << "DeckManager::ExtractInitSetting2aStatus: m_nInitSetting: " << m_nInitSetting << LOG_ENDL;

    InitialSetting2aStatus *pStatus = &pPacket->m_tData.m_tInitSetting2a;

    m_tInit2a.m_nAudioOutput        = (DECK_AUDIO_OUTPUT_FORMAT)pStatus->nAudioOutput;
    m_tInit2a.m_nDualMono           = (DECK_DUAL_MONO_CONDITION)pStatus->nDualMono;
    m_tInit2a.m_nPCMSetting         = (DECK_PCM_SETTING_CONDITION)pStatus->nPCMSetting;
    m_tInit2a.m_nDSPIFDownMix       = (DECK_DSP_IF_DOWN_MIX_SETTING)pStatus->nDSPIF;
    m_tInit2a.m_n2chIFDownMix       = (DECK_2CH_IF_DOWN_MIX_SETTING)pStatus->n2CHIF;
    m_tInit2a.m_nFrontSp            = (DECK_SPEEKER_SETTING)pStatus->nFrontSp;
    m_tInit2a.m_nCenterSp           = (DECK_SPEEKER_SETTING)pStatus->nCenterSp;
    m_tInit2a.m_nRearSp             = (DECK_SPEEKER_SETTING)pStatus->nRearSp;
    m_tInit2a.m_nSubwoofer          = (DECK_SUBWOOFER_SETTING)pStatus->nSubWoofer;
    m_tInit2a.m_nCenterSpDelay      = pStatus->nCenterSpDelay;
    m_tInit2a.m_nRearSpDelay        = pStatus->nRearSpDelay;
    m_tInit2a.m_nCompLevel          = pStatus->nCompLevel;
    m_tInit2a.m_nFrontLSpLevel      = pStatus->nFrontLSpLevel;
    m_tInit2a.m_nFrontRSpLevel      = pStatus->nFrontRSpLevel;
    m_tInit2a.m_nRearLSpLevel       = pStatus->nRearLSpLevel;
    m_tInit2a.m_nRearRSpLevel       = pStatus->nRearRSpLevel;
    m_tInit2a.m_nCenterSpLevel      = pStatus->nCenterSpLevel;
    m_tInit2a.m_nDTSOutput          = (DECK_DTS_OUTPUT_SETTING)pStatus->nDTSOutput;
    m_tInit2a.m_n6chStereo          = (DECK_6CH_STEREO_SETTING)pStatus->n6CHStereo;
    m_tInit2a.m_nBaseMngr           = (DECK_BASE_MANAGEMENT)pStatus->nBaseManagement;
    m_tInit2a.m_nDownSampling       = (DECK_DOWN_SAMPLING_SETTING)pStatus->nDownSampling;
    m_tInit2a.m_nDolPro12           = (DECK_DOL_PRO12_SETTING)pStatus->nDOLPro12;
    m_tInit2a.m_nCenterBlendLvl     = pStatus->nCenterBlandLevel;
    m_tInit2a.m_nDimension          = pStatus->nDimension;
    m_tInit2a.m_nPanorama           = pStatus->nPanorama;
    m_tInit2a.m_nCenterWidth        = pStatus->nCenterWidth;

    #if 0
    MC_LOG << "DeckManager::m_pPacket->InitializationSetting1()" << LOG_ENDL;
    MC_LOG << "DeckManager::m_tInit2a.m_nAudioOutput:   " << m_tInit2a.m_nAudioOutput);
    MC_LOG << "DeckManager::m_tInit2a.m_nDualMono:      " << m_tInit2a.m_nDualMono);
    MC_LOG << "DeckManager::m_tInit2a.m_nPCMSetting:    " << m_tInit2a.m_nPCMSetting);
    MC_LOG << "DeckManager::m_tInit2a.m_nDSPIFDownMix:  " << m_tInit2a.m_nDSPIFDownMix);
    MC_LOG << "DeckManager::m_tInit2a.m_n2chIFDownMix:  " << m_tInit2a.m_n2chIFDownMix);
    MC_LOG << "DeckManager::m_tInit2a.m_nFrontSp:       " << m_tInit2a.m_nFrontSp);
    MC_LOG << "DeckManager::m_tInit2a.m_nCenterSp:      " << m_tInit2a.m_nCenterSp);
    MC_LOG << "DeckManager::m_tInit2a.m_nRearSp:        " << m_tInit2a.m_nRearSp);
    MC_LOG << "DeckManager::m_tInit2a.m_nSubwoofer:     " << m_tInit2a.m_nSubwoofer);
    MC_LOG << "DeckManager::m_tInit2a.m_nCenterSpDelay: " << m_tInit2a.m_nCenterSpDelay);
    MC_LOG << "DeckManager::m_tInit2a.m_nRearSpDelay:   " << m_tInit2a.m_nRearSpDelay);
    MC_LOG << "DeckManager::m_tInit2a.m_nCompLevel:     " << m_tInit2a.m_nCompLevel);
    MC_LOG << "DeckManager::m_tInit2a.m_nFrontLSpLevel: " << m_tInit2a.m_nFrontLSpLevel);
    MC_LOG << "DeckManager::m_tInit2a.m_nFrontRSpLevel: " << m_tInit2a.m_nFrontRSpLevel);
    MC_LOG << "DeckManager::m_tInit2a.m_nRearLSpLevel:  " << m_tInit2a.m_nRearLSpLevel);
    MC_LOG << "DeckManager::m_tInit2a.m_nRearRSpLevel:  " << m_tInit2a.m_nRearRSpLevel);
    MC_LOG << "DeckManager::m_tInit2a.m_nCenterSpLevel: " << m_tInit2a.m_nCenterSpLevel);
    MC_LOG << "DeckManager::m_tInit2a.m_nDTSOutput:     " << m_tInit2a.m_nDTSOutput);
    MC_LOG << "DeckManager::m_tInit2a.m_n6chStereo:     " << m_tInit2a.m_n6chStereo);
    MC_LOG << "DeckManager::m_tInit2a.m_nBaseMngr:      " << m_tInit2a.m_nBaseMngr);
    MC_LOG << "DeckManager::m_tInit2a.m_nDownSampling:  " << m_tInit2a.m_nDownSampling);
    MC_LOG << "DeckManager::m_tInit2a.m_nDolPro12:      " << m_tInit2a.m_nDolPro12);
    MC_LOG << "DeckManager::m_tInit2a.m_nCenterBlndLvl: " << m_tInit2a.m_nCenterBlendLvl);
    MC_LOG << "DeckManager::m_tInit2a.m_nDimension:     " << m_tInit2a.m_nDimension);
    MC_LOG << "DeckManager::m_tInit2a.m_nPanorama:      " << m_tInit2a.m_nPanorama);
    MC_LOG << "DeckManager::m_tInit2a.m_nCenterWidth:   " << m_tInit2a.m_nCenterWidth);
    #endif

    if(m_nInitSetting == 2) {
        m_nInitSetting = 3;
        #if 1
        if(m_tInit2a.m_nAudioOutput != eDM_AUDIO_SPDIF_NO_MPEG_STREAM || m_tInit2a.m_nDownSampling != eDM_DOWN_SAMPLING_ON_48_44) {
            InitialSetting2aCommand tCmd;
            GetInitSetting2aCmd(tCmd);
            tCmd.nAudioOutput  = eDM_AUDIO_SPDIF_NO_MPEG_STREAM;
            tCmd.nDownSampling = eDM_DOWN_SAMPLING_ON_48_44;
            m_pPacket->InitializationSetting2a(&tCmd);
            MC_LOG << "DeckManager::m_pPacket->InitializationSetting2a()" << LOG_ENDL;
        }
        #endif
        m_pPacket->RequestInitialSetting3Status();
    }

    return true;
}

bool DeckManager::ExtractInitSetting3Status(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_INIT_SET3_STATUS) return false;

    MC_LOG << "DeckManager::ExtractInitSetting3Status: m_nInitSetting: " << m_nInitSetting << LOG_ENDL;

    InitialSetting3Status *pStatus = &pPacket->m_tData.m_tInitSetting3;

    m_tInit3.m_nNoFileFolder        = pStatus->nNoFileFolder;
    m_tInit3.m_nPriorityID3Tag      = pStatus->nPriorityID3Tag;
    m_tInit3.m_nMP3Play             = pStatus->nMP3Playback;
    m_tInit3.m_nWMAPlay             = pStatus->nWMAPlayback;
    m_tInit3.m_nPVDSVD              = pStatus->nPVDSVD;
    m_tInit3.m_nRandomTrackUp       = pStatus->nRandomTrackUp;
    m_tInit3.m_nRepeatCancel        = pStatus->nRepeatCancel;
    m_tInit3.m_nRandomCancel        = pStatus->nRandomCancel;
    m_tInit3.m_nFastUpPlay          = pStatus->nFastUpPlay;
    m_tInit3.m_nFastDownPlay        = pStatus->nFastDownPlay;
    m_tInit3.m_nTrackDownTime       = pStatus->nTrackDownTime;
    m_tInit3.m_nSingleSession       = pStatus->nSingleSession;
    m_tInit3.m_nDVDAudioPlay        = pStatus->nDVDAudioPlayback;
    m_tInit3.m_nHaltByHighTemp      = pStatus->nHaltByHighTemp;
    m_tInit3.m_nTitleEnd            = pStatus->nTitleEnd;
    m_tInit3.m_nEjectRsume          = pStatus->nEjectResume;
    m_tInit3.m_nDirPriority         = pStatus->nDirPriority;
    m_tInit3.m_nPlayTimeDisplay     = pStatus->nPlayTimeDisplay;

    #if 0
    MC_LOG << "DeckManager::m_tInit3.m_nNoFileFolder:    " << m_tInit3.m_nNoFileFolder);
    MC_LOG << "DeckManager::m_tInit3.m_nPriorityID3Tag:  " << m_tInit3.m_nPriorityID3Tag);
    MC_LOG << "DeckManager::m_tInit3.m_nMP3Play:         " << m_tInit3.m_nMP3Play);
    MC_LOG << "DeckManager::m_tInit3.m_nWMAPlay:         " << m_tInit3.m_nWMAPlay);
    MC_LOG << "DeckManager::m_tInit3.m_nPVDSVD:          " << m_tInit3.m_nPVDSVD);
    MC_LOG << "DeckManager::m_tInit3.m_nRandomTrackUp:   " << m_tInit3.m_nRandomTrackUp);
    MC_LOG << "DeckManager::m_tInit3.m_nRepeatCancel:    " << m_tInit3.m_nRepeatCancel);
    MC_LOG << "DeckManager::m_tInit3.m_nRandomCancel:    " << m_tInit3.m_nRandomCancel);
    MC_LOG << "DeckManager::m_tInit3.m_nFastUpPlay:      " << m_tInit3.m_nFastUpPlay);
    MC_LOG << "DeckManager::m_tInit3.m_nFastDownPlay:    " << m_tInit3.m_nFastDownPlay);
    MC_LOG << "DeckManager::m_tInit3.m_nTrackDownTime:   " << m_tInit3.m_nTrackDownTime);
    MC_LOG << "DeckManager::m_tInit3.m_nSingleSession:   " << m_tInit3.m_nSingleSession);
    MC_LOG << "DeckManager::m_tInit3.m_nDVDAudioPlay:    " << m_tInit3.m_nDVDAudioPlay);
    MC_LOG << "DeckManager::m_tInit3.m_nHaltByHighTemp:  " << m_tInit3.m_nHaltByHighTemp);
    MC_LOG << "DeckManager::m_tInit3.m_nTitleEnd:        " << m_tInit3.m_nTitleEnd);
    MC_LOG << "DeckManager::m_tInit3.m_nEjectRsume:      " << m_tInit3.m_nEjectRsume);
    MC_LOG << "DeckManager::m_tInit3.m_nDirPriority:     " << m_tInit3.m_nDirPriority);
    MC_LOG << "DeckManager::m_tInit3.m_nPlayTimeDisplay: " << m_tInit3.m_nPlayTimeDisplay);
    #endif

    MC_LOG << "DeckManager::m_tInit3.m_nNoFileFolder: " << m_tInit3.m_nNoFileFolder << ", m_tInit3.m_nDirPriority: " << m_tInit3.m_nDirPriority << LOG_ENDL;

    if(m_nInitSetting == 3) {
        m_nInitSetting = 4;

        if(pStatus->nRsvd4            != 14 ||
           pStatus->nRsvd6            != 2  ||
           m_tInit3.m_nNoFileFolder   != 0  ||
           m_tInit3.m_nDirPriority    != 1  ||
           m_tInit3.m_nTrackDownTime  != 1  || 
           m_tInit3.m_nRandomTrackUp  != 1  ||
           m_tInit3.m_nRandomCancel   != 1  ||
           m_tInit3.m_nRepeatCancel   != 0  ||
           m_tInit3.m_nEjectRsume     != 1  ||
           m_tInit3.m_nPriorityID3Tag != 1  ||
           m_tInit3.m_nDVDAudioPlay   != 1  || // added by cychoi 2013.06.09 Behave as compliant of DVD-Audio
           m_tInit3.m_nPlayTimeDisplay!= 1  || // added by cychoi 2013.07.01 for ITS 176302 Playback time error
           m_tInit3.m_nFastDownPlay   != 1) 
        {
            InitialSetting3Command tCmd;
            GetInitSetting3Cmd(tCmd);
            tCmd.nUndefined4     = 14;
            tCmd.nUndefined6     = 2;
            tCmd.nNoFileFolder   = 0; // 파일이 없는 폴더도 표시함.
            tCmd.nDirPriority    = 1; // 폴더 정보가 파일정보보다 먼저 온다.
            tCmd.nTrackDownTime  = 1; // 3초
            tCmd.nRandomTrackUp  = 1; // Track up시 Random하게 이동.
            tCmd.nRandomCancel   = 1; // RANDOM은 유지. 
            tCmd.nRepeatCancel   = 0; // REPEAT는 유지하고 필요한 경우 Repeat Off 명령어로 해제한다.
            tCmd.nEjectResume    = 1; // Store resume data.
            tCmd.nPriorityID3Tag = 1; // Tag Ver2
            tCmd.nFastDownPlay   = 1; // 정상속도로 재생한다.
            tCmd.nDVDAudioPlay   = 1; // added by cychoi 2013.06.09 Behave as compliant of DVD-Audio
            tCmd.nPlayTimeDisplay = 1; // added by cychoi 2013.07.01 for ITS 176302 Playback time error
            m_pPacket->InitializationSetting3(&tCmd);
            MC_LOG << "DeckManager::m_pPacket->InitializationSetting3()" << LOG_ENDL;
        }
    }

    // { modified by cychoi 2013.10.04 for Disc Detect Performance on Boot-Up
    if(m_pPacket->RequestCommonStatus() > 0)
    {
        MC_LOG << "DeckManager::ExtractInitSetting3Status(): RequestCommonStatus() success." << LOG_ENDL;
    }
    else
    {
        MC_LOG << "DeckManager::ExtractInitSetting3Status(): RequestCommonStatus() failure." << LOG_ENDL;
        StartCommonStatusTimer();
    }
    // } modified by cychoi 2013.10.04
    
    return true;
}

// { added by cychoi 2013.06.20 for SW reset on disc loading
#if _DECK_DIAGNOSTIC_
bool DeckManager::ExtractResetStart(StatusData *pPacket)
{
    if(pPacket->m_nCode != DS_CODE_RESET_START) return false;

    //{ modified by cychoi 2013.08.10 for ISV 88267 Delay retry until Deck is ready
    // modified by cychoi 2013.07.10 handle RESET_START in each Disc Manager after load complete
    if(m_bLoadComplete == true)
    {
        m_tPrevDeckStatus = DeckCommonStatus();
        m_tDeckStatus.m_nDiscDetect = eDM_DISC_DETECT_NO_EXEUTED;

        return false;
    }
    //} modified by cychoi 2013.08.10

    MC_HIGH << "DeckManager::ExtractResetStart" << LOG_ENDL;

    m_bResetRestart = true;

    // { modified by cychoi 2013.07.17 for ITS 180555 SW reset on disc loading
    m_tPrevDeckStatus = DeckCommonStatus();
    //{ modified by cychoi 2013.08.10 for ISV 88267 Delay retry until Deck is ready
    m_tDeckStatus.m_nDiscDetect = eDM_DISC_DETECT_NO_EXEUTED;
    //} modified by cychoi 2013.08.10
    // removed by cychoi 2013.08.10 for unused code
    m_bLoadComplete   = false;
    m_nInitSetting    = 0;
    if(IsEject() == true)
    {
        m_nDeviceState = DiscNotPresent;
        // { modified by cychoi 2014.04.14 for HW Event PQ launch DVD mode unconditionally after Deck SW reset
        if(m_nLastDiscState == PLAYER_DISCEJECTED ||
           m_nLastDiscState == PLAYER_DISCEJECTING)
        {
            if(m_tReloadTimer.isActive())
            {
                m_tReloadTimer.stop();
            }
            m_bReloadDisc = true;
            m_bReloadAfterError = m_bEjectByError;
        }
        // } modified by cychoi 2014.04.14
    }
    else
    {
        m_nDeviceState = DiscPresnt;
    }
    Init();
    // } modified by cychoi 2013.07.17

    return true;
}
#endif
// } added by cychoi 2013.06.20

// { added by cychoi 2013.10.04 for Disc Detect Performance on Boot-Up
int DeckManager::InitCommonStatusTimer()
{
    if(m_tCommonStatusTimer.isActive())
    {
        m_tCommonStatusTimer.stop();
    }

    return true;
}

int DeckManager::StartCommonStatusTimer()
{
    if(m_tCommonStatusTimer.isActive())
    {
        m_tCommonStatusTimer.stop();
    }

    m_tCommonStatusTimer.start(500);
    m_nCommonStatusReqCount = 0; // added by cychoi 2013.10.19 for Common status request limitation

    return true;
}
// } added by cychoi 2013.10.04

#ifdef DECK_DISC_DETECTION_ERR_HANDLING
// { modified by cychoi 2014.01.16 for ITS 220385 Disc Detection Retry
void DeckManager::HandleMediaModeError()
{
    if(IsEject() == true || m_bReloadAfterError == true)
    {
        // If Disc is ejecting or Disc was reloaded as Error Disc, just return
        return;
    }

    if(IsSupportMediaMode(m_tDeckStatus.m_nMediaMode) == false)
    {
        if(m_tPrevDeckStatus.m_nMediaMode == eDM_MODE_NONE ||
           IsSupportMediaMode(m_tPrevDeckStatus.m_nMediaMode) == true)
        {
            MC_HIGH << "DeckManager::HandleMediaModeError: " << m_tPrevDeckStatus.m_nMediaMode << ", " << m_tDeckStatus.m_nMediaMode << ", " << m_nDiscDetectRetryCount << LOG_ENDL;
            if(m_nDiscDetectRetryCount < 3)
            {
                // Please retry to detect media mode 3 times if previous mode is valid or NONE
                m_pPacket->DiscDetection(); // modified by cychoi 2014.03.10 for ISV 98219
                m_tDeckStatus.m_nMediaMode = m_tPrevDeckStatus.m_nMediaMode;
                m_nDiscDetectRetryCount++;
            }
        }
    }
}
// } modified by cychoi 2014.01.16

// { added by cychoi 2014.03.10 for ISV 98219
void DeckManager::HandleReadError()
{
    if(m_tDeckStatus.m_nReadError == eDM_READ_ERROR_STOP)
    {
        if(m_tPrevDeckStatus.m_nReadError != eDM_READ_ERROR_STOP)
        {
            MC_HIGH << "DeckManager::HandleReadError: " << m_tPrevDeckStatus.m_nReadError << ", " << m_tDeckStatus.m_nReadError << ", " << m_nDiscReadRetryCount << LOG_ENDL;
            if(m_nDiscReadRetryCount < 3)
            {
                // Please retry to read disc 3 times if previous read is OK
                m_pPacket->DiscDetection();
                m_tDeckStatus.m_nReadError = m_tPrevDeckStatus.m_nReadError;
                m_nDiscReadRetryCount++;
            }
        }
    }
    else
    {
        m_nDiscReadRetryCount = 0;
    }
}

void DeckManager::HandleDetectReadError()
{
    if(m_tDeckStatus.m_nDiscDetect == eDM_DISC_DETECT_NA_FAILED)
    {
        if(m_tPrevDeckStatus.m_nDiscDetect != eDM_DISC_DETECT_NA_FAILED)
        {
            MC_HIGH << "DeckManager::HandleDetectReadError: " << m_tPrevDeckStatus.m_nDiscDetect << ", " << m_tDeckStatus.m_nDiscDetect << ", " << m_nDiscDetectReadRetryCount << LOG_ENDL;
            if(m_nDiscDetectReadRetryCount < 3)
            {
                // Please retry to detect disc 3 times if previous detection is valid or NONE
                m_pPacket->DiscDetection();
                m_tDeckStatus.m_nDiscDetect = m_tPrevDeckStatus.m_nDiscDetect;
                m_nDiscDetectReadRetryCount++;
            }
        }
    }
    else
    {
        m_nDiscDetectReadRetryCount = 0;
    }
}
// } added by cychoi 2014.03.10
#endif

int DeckManager::IsPlayState(int nState)
{
    switch(nState)
    {
    case PLAYER_PLAY:
    case PLAYER_SEARCH:
    case PLAYER_FASTFORWARD: 
    case PLAYER_REWIND:
        return true;
    default:
        return false;
    }
}

int DeckManager::IsPauseState(int nState)
{
    switch(nState)
    {
    case PLAYER_PAUSE:
        return true;
    default:
        return false;
    }
}

int DeckManager::IsStopState(int nState)
{
    switch(nState)
    {
    case PLAYER_PLAY:
    case PLAYER_PAUSE:
    case PLAYER_SEARCH:
    case PLAYER_FASTFORWARD: 
    case PLAYER_REWIND:
        return false;
    default:
        return true;
    }
}

int DeckManager::GetDVDRegionCodeFromKernel()
{
    int nOperand;
    int nRegionCode;

    FILE *pFile = fopen("/proc/dvd_region", "r");
    if(pFile == 0) {
        return -1;
    }

    // modified by cychoi 2013-07-16 for Prevent fix
    int count = fscanf(pFile, "%4x", &nOperand);
    if(count == EOF) {
        fclose(pFile);
        return -1;
    }
    //fscanf(pFile, "%x", &nOperand);
    // } modified by cychoi 2013-07-16

    BCD2 tBCD;
    tBCD.byte = nOperand;
    
    nRegionCode = tBCD.BCD.upper - 8;
    MC_LOG << "DeckManager::GetDVDRegionCodeFromKernel(): nOperand: " << nOperand << ". " << nOperand << ", nRegionCode: " << nRegionCode << LOG_ENDL;

    fclose(pFile);

    return nRegionCode;
}

void DeckManager::CallDiscTestPlay() //added by wonseok.heo for in out tests
{
    CDDAManager::GetInstance()->DiscTestPlay();
}
