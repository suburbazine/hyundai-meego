#ifndef DECKMANAGER_H
#define DECKMANAGER_H

#include <QObject>
#include <QTimer>
#include "packet.h"
#include "imediainterface.h"
#include "iaudiointerface.h"
#include "ivideointerface.h"
#include "deck_def.h"
#include "common.h"

class DeckCommonStatus
{
public:
    DeckCommonStatus() 
    {
        m_nDiscPos      = eDM_DISC_POS_NO_DISC;
        m_nMediaMode    = eDM_MODE_NONE;
        m_nHighTemp     = eDM_HIGH_TEMP_ESCAPE;
        m_nReadError    = eDM_READ_ERROR_ESCAPE;
        m_nMechaError   = eDM_MECHA_ERROR_ESCAPE;
        m_nConsumption  = eDM_CONSUMPTION_NORMAL;
        m_nRegionError  = eDM_REGION_NORMAL;
        m_nParental     = eDM_PARENTAL_NORMAL;
        m_nAllUnsupport = eDM_ONE_MORE_RELEVANT;
        m_nDeckVerNo    = 0;
        // { added by cychoi 2013-07-15 for Prevent fix
        m_nWorkMecha    = eDM_WORK_DURING_OTHER_OPER;
        m_nLoadingDir   = eDM_DIR_DURING_LOADING;
        m_nAtapiBus     = eDM_ATAPI_BUS_INVALID;
        m_nBusPrepare   = eDM_BUS_PREPARATION_COMPLETE;
        m_nDRMUnsupport = eDM_ONE_MORE_NON_DRM_FILE;
        m_nDiscDetect   = eDM_DISC_DETECT_NO_EXEUTED;
        m_nDispScreen   = eDM_DISPLAY_SCREEN_PLAY;
        m_nDeckVerUp    = eDM_VER_NON_UPGRADE;
        m_nVideoFormat  = eDM_VIDEO_FORMAT_NTSC;
        m_nJolietFormat = eDM_NON_JOLIET_FORMAT;
        m_nTemperature  = 0;
        // } added by cychoi 2013-07-15
    }

    DECK_DISC_POSITION          m_nDiscPos;
    DECK_WORKING_MECHANIC       m_nWorkMecha;
    DECK_LOADING_DIR            m_nLoadingDir;
    DECK_READ_ERROR             m_nReadError;
    DECK_MECHANIC_ERROR         m_nMechaError;
    DECK_HIGH_TEMP_DETECT       m_nHighTemp;
    DECK_ATAPI_BUS              m_nAtapiBus;
    DECK_LOW_CONSUMPTION_MODE   m_nConsumption;
    DECK_REGION_ERROR           m_nRegionError;
    DECK_PARENTAL_ERROR         m_nParental;
    DECK_BUS_IN_PREPARATION     m_nBusPrepare;
    DECK_ALL_UNSUPPORTED        m_nAllUnsupport;
    DECK_DRM_ALL_UNSUPPORTED    m_nDRMUnsupport;
    DECK_DISC_IDENTIFIECATION   m_nDiscDetect;
    DECK_MEDIA_MODE             m_nMediaMode;
    DECK_DISPLAY_SCREEN_MODE    m_nDispScreen;
    DECK_VERSION_UPGRADE        m_nDeckVerUp;
    DECK_VIDEO_FORMAT           m_nVideoFormat;
    DECK_JOLIET_FORMAT          m_nJolietFormat;
    int                         m_nTemperature;
    UINT4                       m_nDeckVerNo;
};

class DeckPictureSetting
{
public:
    DeckPictureSetting()
    {
        m_nContrastLevel    = 0;
        m_nColorLevel       = 0;
        m_nTintLevel        = 0;
        m_nBrightnessLevel  = 0;
        m_nSharpnessLevel   = 0;
        m_nGammaCorrection  = 0;
        m_nCoringFilter     = 0;
        m_nBlackLevel       = 0;
        m_nWhiteLevel       = 0;
    }

    int                         m_nContrastLevel;   // -7 ~ 7
    int                         m_nColorLevel;      // -7 ~ 7
    int                         m_nTintLevel;       // -7 ~ 7
    int                         m_nBrightnessLevel; // -7 ~ 7
    int                         m_nSharpnessLevel;  // -7 ~ 7
    int                         m_nGammaCorrection; // Off(0)/On(1)
    int                         m_nCoringFilter;    // Off(0)/On(1)
    int                         m_nBlackLevel;      // -7 ~ 7
    int                         m_nWhiteLevel;      // -7 ~ 7
};

class DeckInit1Setting
{
public:
    DeckInit1Setting() 
    {
        // { added by cychoi 2013-07-15 for Prevent fix
        m_bOSDOff           = 0;
        m_nRegionCode       = 0;
        m_nParentalLevel    = 0;
        m_nPrioSubTitleCode = 0;
        m_nPrioAudioCode    = 0;
        m_nPrioMenuLangCode = 0;
        m_nScreenSize       = eDM_SCREEN_SIZE_WIDE;
        m_nAngleDisplay     = eDM_ANGLE_MAKR_PERMITTED;
        m_nSubTitleDisplay  = eDM_SUBTITLE_DISPLAY_PERMITTED;
        m_nPinNumber        = 0;
        m_nOpenningScreen   = eDM_OPENING_ECLIPSE_LOGO;
        m_nScreenMode       = eDM_SCREEN_MODE_NTSC;
        m_nPictureMode      = eDM_PICTURE_MODE_NORMAL;
        m_nPBC              = 0;
        m_nIRE              = 0;
        m_nBackColor        = 0;
        m_nCountryCode      = 0;
        m_nPlayerMode       = eDM_PLAYER_MODE_DVD_VIDEO;
        m_nVideoOutput      = eDM_VIDEO_COMPOSITE_OUTPUT;
        // } added by cychoi 2013-07-15
    }

    int                         m_bOSDOff;
    int                         m_nRegionCode;
    int                         m_nParentalLevel;
    int                         m_nPrioSubTitleCode;
    int                         m_nPrioAudioCode;
    int                         m_nPrioMenuLangCode;
    DECK_SCREEN_SIZE            m_nScreenSize;
    DECK_ANGLE_DISPLAY          m_nAngleDisplay;
    DECK_SUBTITLE_DISPLAY       m_nSubTitleDisplay;
    int                         m_nPinNumber;
    DECK_OPENING_DISP_SCREEN    m_nOpenningScreen;
    DECK_SCREEN_MODE            m_nScreenMode;
    DECK_PICTURE_MODE           m_nPictureMode;
    int                         m_nPBC;
    int                         m_nIRE;
    int                         m_nBackColor;
    int                         m_nCountryCode;
    DECK_PLAYER_MODE            m_nPlayerMode;
    DECK_VIDEO_OUTPUT_FORMAT    m_nVideoOutput;
};

class DeckInit2aSetting
{
public:
    DeckInit2aSetting() 
    {
        // { added by cychoi 2013-07-15 for Prevent fix
        m_nAudioOutput    = eDM_AUDIO_3LINES_DEGITAL;
        m_nDualMono       = eDM_DUAL_MONO_STEREO;
        m_nPCMSetting     = eDM_PCM_SET_UNDER_48KHZ_16BIT;
        m_nDSPIFDownMix   = eDM_DSP_IF_DOWN_MIX_STEREO;
        m_n2chIFDownMix   = eDM_2CH_IF_DOWN_MIX_STEREO;
        m_nFrontSp        = eDM_SPEEKER_SET_LARGE;
        m_nCenterSp       = eDM_SPEEKER_SET_LARGE;
        m_nRearSp         = eDM_SPEEKER_SET_LARGE;
        m_nSubwoofer      = eDM_SUBWOOFER_SET_ON;
        m_nCenterSpDelay  = 0;
        m_nRearSpDelay    = 0;
        m_nCompLevel      = 0;
        m_nFrontLSpLevel  = 0;
        m_nFrontRSpLevel  = 0;
        m_nRearLSpLevel   = 0;
        m_nRearRSpLevel   = 0;
        m_nCenterSpLevel  = 0;
        m_nSubwooferLevel = 0;
        m_nDTSOutput      = eDM_DTS_MUTE_PLAY_SOUND;
        m_n6chStereo      = eDM_6CH_STEREO_OFF;
        m_nBaseMngr       = eDM_BASE_MNGR_MULTI_SOURCE_ON;
        m_nDownSampling   = eDM_DOWN_SAMPLING_OFF;
        m_nDolPro12       = eDM_DOL_PRO12_OFF;
        m_nCenterBlendLvl = 0;
        m_nDimension      = 0;
        m_nPanorama       = 0;
        m_nCenterWidth    = 0;
        // } added by cychoi 2013-07-15
    }

    DECK_AUDIO_OUTPUT_FORMAT        m_nAudioOutput;
    DECK_DUAL_MONO_CONDITION        m_nDualMono;
    DECK_PCM_SETTING_CONDITION      m_nPCMSetting;
    DECK_DSP_IF_DOWN_MIX_SETTING    m_nDSPIFDownMix;
    DECK_2CH_IF_DOWN_MIX_SETTING    m_n2chIFDownMix;
    DECK_SPEEKER_SETTING            m_nFrontSp;
    DECK_SPEEKER_SETTING            m_nCenterSp;
    DECK_SPEEKER_SETTING            m_nRearSp;
    DECK_SUBWOOFER_SETTING          m_nSubwoofer;
    int                             m_nCenterSpDelay;
    int                             m_nRearSpDelay;
    int                             m_nCompLevel;
    int                             m_nFrontLSpLevel;
    int                             m_nFrontRSpLevel;
    int                             m_nRearLSpLevel;
    int                             m_nRearRSpLevel;
    int                             m_nCenterSpLevel;
    int                             m_nSubwooferLevel;
    DECK_DTS_OUTPUT_SETTING         m_nDTSOutput;
    DECK_6CH_STEREO_SETTING         m_n6chStereo;
    DECK_BASE_MANAGEMENT            m_nBaseMngr;
    DECK_DOWN_SAMPLING_SETTING      m_nDownSampling;
    DECK_DOL_PRO12_SETTING          m_nDolPro12;
    int                             m_nCenterBlendLvl;
    int                             m_nDimension;
    int                             m_nPanorama;
    int                             m_nCenterWidth;
};

class DeckInit3Setting
{
public:
    DeckInit3Setting() 
    {
        // { added by cychoi 2013-07-15 for Prevent fix
        m_nNoFileFolder   = 0;
        m_nPriorityID3Tag = 0;
        m_nMP3Play        = 0;
        m_nWMAPlay        = 0;
        m_nPVDSVD         = 0;
        m_nRandomTrackUp  = 0;
        m_nRepeatCancel   = 0;
        m_nRandomCancel   = 0;
        m_nFastUpPlay     = 0;
        m_nFastDownPlay   = 0;
        m_nTrackDownTime  = 0;
        m_nSingleSession  = 0;
        m_nDVDAudioPlay   = 0;
        m_nHaltByHighTemp = 0;
        m_nTitleEnd       = 0;
        m_nEjectRsume     = 0;
        m_nDirPriority    = 0;
        m_nPlayTimeDisplay= 0;
        // } added by cychoi 2013-07-15
    }

    int                             m_nNoFileFolder;
    int                             m_nPriorityID3Tag;
    int                             m_nMP3Play;
    int                             m_nWMAPlay;
    int                             m_nPVDSVD;
    int                             m_nRandomTrackUp;
    int                             m_nRepeatCancel;
    int                             m_nRandomCancel;
    int                             m_nFastUpPlay;
    int                             m_nFastDownPlay;
    int                             m_nTrackDownTime;
    int                             m_nSingleSession;
    int                             m_nDVDAudioPlay;
    int                             m_nHaltByHighTemp;
    int                             m_nTitleEnd;
    int                             m_nEjectRsume;
    int                             m_nDirPriority;
    int                             m_nPlayTimeDisplay;
};

#define DECK_DISC_DETECTION_ERR_HANDLING // added by cychoi 2013.10.30 for Disc Detection Retry & Limitation

class IPlayerEngine;

class DeckManager : public QObject
{
    Q_OBJECT
public:
    static DeckManager* GetInstance();
    static void         DeleteInstance();
    
    ~DeckManager();

    IMediaInterface*    GetMediaInstance();
    IAudioInterface*    GetAudioInstance();
    IVideoInterface*    GetVideoInstance();

    void                OpenDeckProtocol();
    void                CloseDeckProtocol();

    void                InitializeDeckPrioritySettings(DeckPrioritySettings *pSettings);
    void                SetEject(bool bEject);
    int                 PlayCondToPlayerState(DECK_CD_PLAY_COND ePlayCond);
    DECK_MEDIA_MODE     GetMediaMode();

    void                SetBrightness(int nBrightness);
    void                SetContrast(int nContrast);
    void                SetTint(int nTint);
    int                 GetBrightness();
    int                 GetContrast();
    int                 GetTint();

    void                SetScanMode(int nMode);
    void                SetRandomMode(int nMode);
    void                SetRepeatMode(int nMode);
    int                 GetScanMode();
    int                 GetRandomMode();
    int                 GetRepeatMode();

    QWidget*            GetVideoWidget();
    void                ControlGPlayerEngine(int nCurrState, int nPrevState);
    IPlayerEngine*      GetPlayerEngine();
    bool                ClearDisplay(bool isBackground);                
    void                EmitError(int nError);
    int                 GetError();
    int                 GetCurrentError(); // added by cychoi 2013.10.17 for ITS 196656 & ITS 194618

    int                 GetDeviceState();
    bool                IsDeckBlocked(); // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling

    int                 IsSupportMediaMode(DECK_MEDIA_MODE eMediaMode);
    int                 IsEject();
    int                 IsExistCDText();
    int                 IsMixedCD();
    int                 IsExistCDInDeck();
    DECK_MIXED_CD_MODE  GetSwitchMixedMode();
    void                SetSwitchMixedMode(DECK_MIXED_CD_MODE eMode);
    DECK_MEDIA_MODE     GetDVDMediaMode(); // added by cychoi 20103.09.26 for ITS 188493 DVD Navigation Still Picture
    DECK_MEDIA_MODE     GetCDDAMediaMode(); // added by cychoi 20103.10.10 for ITS 167582 Invalid CDDA+VCD Mixed CD handling

    void                SetOSD(uint nMode);

    void                GetCommonStatus(DeckCommonStatus &tStatus);
    void                GetInitSetting1(DeckInit1Setting &tSetting);
    void                GetInitSetting2a(DeckInit2aSetting &tSetting);
    void                GetInitSetting3(DeckInit3Setting &tSetting);
    void                GetInitSetting1Cmd(InitialSetting1Command &tCmd);
    void                GetInitSetting2aCmd(InitialSetting2aCommand &tCmd);
    void                GetInitSetting3Cmd(InitialSetting3Command &tCmd);
    void                SetInitSetting1Cmd(InitialSetting1Command &tCmd);
    void                SetInitSetting2aCmd(InitialSetting2aCommand &tCmd);
    void                SetInitSetting3Cmd(InitialSetting3Command &tCmd);

    void                Eject(uint mode);
#ifdef DECK_DISC_DETECTION_ERR_HANDLING
    void                HandleErrorDisc(int nError, bool bReset); // modified by cychoi 2014.04.14 for HW Event PQ launch DVD mode unconditionally after Deck SW reset
#endif

    void                CallDiscTestPlay(); //added by wonseok.heo for in out test

#if _DECK_DIAGNOSTIC_
    void                StartDiagnostic();
#endif

    void                SetStartPlayTime(int nPlaybackTime);
    void                SetLifeTime(int nPlaybackTime);
    int                 GetLifeTime();

    void                EmitDiscStateChanged(int nState);
    int                 GetDiscState(); // added by cychoi 2013-06-09 for ITS 172825 eject failure on FF_REW

signals:
    void                OnDiscStateChanged(int);
    void                OnError(int);
#if _DECK_DIAGNOSTIC_
    void                OnCommErrorWithDeckChanged(int);
#endif
    void                OnDeckPriorityLangChanged();

public slots:
    void                HandlePacketReceived();

private slots:
    void                HandleReload();
    void                HandleCommonStatus(); // added by cychoi 2013.10.04 for Disc Detect Performance on Boot-Up
#ifdef DECK_DISC_DETECTION_ERR_HANDLING
    void                HandleDiscDetectLimitation();
#endif
#if _DECK_DIAGNOSTIC_
    void                HandleDiagnostic();
#endif

private:
    DeckManager(QObject *parent = 0);

    void                PrintVersion();
    
    void                Init();
    void                RequestInitSetting();
    void                StartMedia();
    void                EndMedia();
    void                ProcessPacket(StatusData *pPacket);

    int                 CheckError();
    
    bool                ExtractCommonStatus(StatusData *pPacket);
    bool                ExtractPictureSettingStatus(StatusData *pPacket);
    bool                ExtractInitSetting1Status(StatusData *pPacket);
    bool                ExtractInitSetting2aStatus(StatusData *pPacket);
    bool                ExtractInitSetting3Status(StatusData *pPacket);
// { added by cychoi 2013.06.20 for SW reset on disc loading
#if _DECK_DIAGNOSTIC_
    bool                ExtractResetStart(StatusData *pPacket);
#endif
// } added by cychoi 2013.06.20

    int                 IsPlayState(int nState);
    int                 IsPauseState(int nState);
    int                 IsStopState(int nState);

    int                 GetDVDRegionCodeFromKernel();

// { added by cychoi 2013.06.20 for SW reset on disc loading
#if _DECK_DIAGNOSTIC_
public:
    void                SetResetRestart(bool bResetRestart) { m_bResetRestart = bResetRestart; }
    bool                GetResetRestart() { return m_bResetRestart; }
#endif
// } added by cychoi 2013.06.20

    // { added by cychoi 2013.10.04 for Disc Detect Performance on Boot-Up
    int                 InitCommonStatusTimer();
    int                 StartCommonStatusTimer();
    // } added by cychoi 2013.10.04

#ifdef DECK_DISC_DETECTION_ERR_HANDLING
    void                HandleMediaModeError(); // modified by cychoi 2014.01.16 for ITS 220385 Disc Detection Retry
    // { added by cychoi 2014.03.10 for ISV 98219
    void                HandleReadError();
    void                HandleDetectReadError();
    // } added by cychoi 2014.03.10
#endif

    static DeckManager         *m_pInstance;
    
    Packet                     *m_pPacket;
    IMediaInterface            *m_pMedia;
    IPlayerEngine              *m_pGPlayer;

    StatusData                  m_tStatusData;

    QTimer                      m_tCommonStatusTimer; // added by cychoi 2013.10.04 for Disc Detect Performance on Boot-Up
#ifdef DECK_DISC_DETECTION_ERR_HANDLING
    QTimer                      m_tDiscDetectLimitationTimer;
    int                         m_nDiscDetectRetryCount;
    // { added by cychoi 2014.03.10 for ISV 98219
    int                         m_nDiscReadRetryCount;
    int                         m_nDiscDetectReadRetryCount;
    // } added by cychoi 2014.03.10
#endif
    DeckCommonStatus            m_tDeckStatus;
    DeckCommonStatus            m_tPrevDeckStatus;
    DeckPictureSetting          m_tPicture;
    DeckInit1Setting            m_tInit1;
    DeckInit2aSetting           m_tInit2a;
    DeckInit3Setting            m_tInit3;
    DECK_ACTIVATION_CONDITION   m_nActiveCond;

    int                         m_nInitSetting;
    int                         m_bCDText;
    int                         m_bMixedCD;
    int                         m_bEject;
    int                         m_bEjectByError;
    int                         m_bReloadAfterError;
    int                         m_bLoadComplete;
    int                         m_nError;
    int                         m_nDeviceState;
    DECK_MIXED_CD_MODE          m_eMixedCDMode;
    QTimer                      m_tReloadTimer;
    int                         m_bReloadDisc;
    // removed by cychoi 2013.08.10 for unused code

    DECK_MEDIA_MODE             m_nDVDMediaMode; // added by cychoi 20103.09.26 for ITS 188493 DVD Navigation Still Picture
    DECK_MEDIA_MODE             m_nCDDAMediaMode; // added by cychoi 20103.10.10 for ITS 167582 Invalid CDDA+VCD Mixed CD handling

    int                         m_nScanMode;
    int                         m_nRandomMode;
    int                         m_nRepeatMode;

#if _DECK_DIAGNOSTIC_    
    int                         m_bDiagCommError;
    int                         m_bDiagReqSend;
    int                         m_nDiagResCount;
    int                         m_nDiagTimerCallCount;
    QTimer                      m_tDiagTimer;
    int                         m_bResetRestart; // moved by cychoi 2013.07.19 for remove compile warning // added by cychoi 2013.06.20 for SW reset on disc loading
#endif
    int                         m_nStartPlayTime;   // seconds.
    int                         m_nLifeTime;        // seconds.

    int                         m_nLastDiscState;
    int                         m_nDiscMechanicEjectCount; // moved by cychoi 2013.07.19 for remove compile warning // added by cychoi 2013.07.09 for auto loading after 3 times eject failure by other method
    int                         m_nDiscEjectCount;
    int                         m_nDiscLoadCount;

    int                         m_nCommonStatusReqCount; // added by cychoi 2013.10.19 for Common status request limitation
};

#endif // DECKMANAGER_H
