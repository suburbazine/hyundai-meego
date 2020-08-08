#ifndef TAB2PROTOCOL_H
#define TAB2PROTOCOL_H

#include <QObject>
#include "messageserialport.h"
#include "type.h"

//////////////////////////////////////////////////////////////////////////////////
// REQUEST CODE
#define DR_CODE_TOC_STATUS_REQUEST              0x31 
#define DR_CODE_TOC_STATUS2_REQUEST             0x30 
#define DR_CODE_DIR_TREE_STATUS_REQUEST         0x34 
#define DR_CODE_TAG_STATUS_REQUEST              0x36 
#define DR_CODE_DVD_VR_TITLE_STATUS_REQUEST     0x5C 
#define DR_CODE_DVD_VR_INFOR_STATUS_REQUEST     0x5D 
#define DR_CODE_CD_TEXT_STATUS_REQUEST          0x3A 
#define DR_CODE_CD_TEST_INFOR_STATUS_REQUEST    0x3B 
#define DR_CODE_FILE_NAME_STATUS_REQUEST        0x35 
#define DR_CODE_FOLDER_NAME_STATUS_REQUEST      0x37 
#define DR_CODE_PICTURE_SETTING_STATUS_REQUEST  0x5F 
#define DR_CODE_INIT_SET1_STATUS_REQUEST        0x6E 
#define DR_CODE_INIT_SET2A_STATUS_REQUEST       0xBF 
#define DR_CODE_INIT_SET3_STATUS_REQUEST        0x6D 
#define DR_CODE_COMMON_STATUS_REQUEST           0x63 
#define DR_CODE_DVD_AUDIOVIDEOVR_STATUS_REQUEST 0x64 
#define DR_CODE_CD_STATUS_REQUEST               0x66 
#define DR_CODE_VIDEO_CD_STATUS_REQUEST         0x67 
#define DR_CODE_COMP_AUDIO_STATUS_REQUEST       0x68 
#define DR_CODE_PARENTAL_CHANGE_REQUEST_REQUEST 0x69 
#define DR_CODE_TEST_SP_STATUS_REQUEST          0x6B 
#define DR_CODE_FOLDERFILE4_STATUS_REQUEST      0x3E 
#define DR_CODE_DIRECT_TOUCH_REQUEST            0x6C 
#define DR_CODE_BONUS_GRP_PIN_NUM_REQ_REQUEST   0x6A 
#define DR_CODE_FILEFONDER_TOTAL_NUMBER_REQUEST 0x97 
#define DR_CODE_TAG_STATUS2_REQUEST             0xB6 

//////////////////////////////////////////////////////////////////////////////////
// RESPONSE CODE
#define DS_CODE_TOC_STATUS              0x71
#define DS_CODE_TOC_STATUS2             0x70
#define DS_CODE_DIR_TREE_STATUS         0x74
#define DS_CODE_TAG_STATUS              0x76
#define DS_CODE_DVD_VR_TITLE_STATUS     0x77
#define DS_CODE_DVD_VR_INFOR_STATUS     0x78
#define DS_CODE_CD_TEXT_STATUS          0x7A
#define DS_CODE_CD_TEST_INFOR_STATUS    0x7B
#define DS_CODE_FILE_NAME_STATUS        0x7C
#define DS_CODE_FOLDER_NAME_STATUS      0x7D
#define DS_CODE_PICTURE_SETTING_STATUS  0x7E
#define DS_CODE_REQ_ERR                 0x7F
#define DS_CODE_INIT_SET1_STATUS        0x80
#define DS_CODE_INIT_SET2A_STATUS       0xC1
#define DS_CODE_INIT_SET3_STATUS        0x82
#define DS_CODE_COMMON_STATUS           0x83
#define DS_CODE_DVD_AUDIOVIDEOVR_STATUS 0x84
#define DS_CODE_CD_STATUS               0x86
#define DS_CODE_VIDEO_CD_STATUS         0x87
#define DS_CODE_COMP_AUDIO_STATUS       0x88
#define DS_CODE_PARENTAL_CHANGE_REQUEST 0x89
#define DS_CODE_TEST_SP_STATUS          0x8B
#define DS_CODE_FOLDERFILE4_STATUS      0x8C
#define DS_CODE_DIRECT_TOUCH            0x8D
#define DS_CODE_BONUS_GRP_PIN_NUM_REQ   0x8E
#define DS_CODE_BONUS_GRP_PIN_NUM_MAT   0x8F
#define DS_CODE_FILEFONDER_TOTAL_NUMBER 0xA3
#define DS_CODE_TAG_STATUS2             0xC6
#define DS_CODE_RESET_START             0xFD
#define DS_CODE_ACC_ON                  0xFE


//////////////////////////////////////////////////////////////////////////////////
// ETC
#define DS_FILE_NAME_CHAR_LENGTH        64
#define DS_FOLDER_NAME_CHAR_LENGTH      64

struct TOCStatus
{
    UINT1       nDiscNo                 : 7;
    UINT1       nValid                  : 1;

    UINT1       nStartTrackNo1          : 4;
    UINT1       nStartTrackNo10         : 4;
    
    UINT1       nLastTrackNo1           : 4;    
    UINT1       nLastTrackNo10          : 4;

    UINT1       nTotalPlayTimeMin1      : 4;    
    UINT1       nTotalPlayTimeMin10     : 4;

    UINT1       nTotalPlayTimeSec1      : 4;    
    UINT1       nTotalPlayTimeSec10     : 4;

    UINT1       nTotalPlayTimeFrm1      : 4;    
    UINT1       nTotalPlayTimeFrm10     : 4;
};

struct TOCStatus2
{
    UINT1       nDiscNo                 : 7;    
    UINT1       nValid                  : 1;

    UINT1       nBlockNo                : 4;    
    UINT1       nRsvd                   : 4;

    UINT1       pPlayStartAddr[3*10];

    /*
    struct {
        UINT1       nPlayStartAddrMin;
        UINT1       nPlayStartAddrSec;
        UINT1       nPlayStartAddrFrm;
    } PlayStartAddr[10];
    */
};

struct DirTreeFileFolderInfo{
    UINT1       nFileFolderNo100        : 4;
    UINT1       nPlayFile               : 1;
    UINT1       nLength                 : 1;
    UINT1       nType                   : 2;
    
    UINT1       nFileFolderNo10_1;
    UINT1       pFileFolderName[8];
};

struct DirTreeStatus
{
    UINT1       nUndefined              : 7;
    UINT1       nValid                  : 1;

    UINT1       nCurrFolderNo100        : 4;    
    UINT1       nHierachy               : 4;
    
    UINT1       nCurrFolderNo10_1;
    UINT1       nBlockNo;
    UINT1       pCurrFolderName[8];

    UINT1       nChildFolderNo100       : 4;
    UINT1       nRsvd1                  : 3;    
    UINT1       nUnicode                : 1;

    UINT1       nChildFolderNo10_1;

    UINT1       nChildFileNo100         : 4;    
    UINT1       nRsvd2                  : 4;
    
    UINT1       nChildFileNo10_1;

    UINT1       pFileFolderInfo[10][10];

    /*
    struct {
        UINT1       nFileFolderNo100        : 4;
        UINT1       nPlayFile               : 1;
        UINT1       nLength                 : 1;
        UINT1       nType                   : 2;
        
        UINT1       nFileFolderNo10_1;
        UINT1       pFileFolderName[8];
    } FileFolderInfo[10];
    */
};

struct TagStatus
{
    UINT1       nUndefined              : 4;
    UINT1       nUnicodeInfo            : 2;
    UINT1       nExist                  : 1;
    UINT1       nValid                  : 1;
    
    UINT1       pTitle[30];
    UINT1       pArtist[30];
    UINT1       pAlbum[30];
    UINT1       pYear[4];
    UINT1       pComment[30];
    // UINT1    nTrackNo[2];
    UINT1       nGenre;
};

struct DVDVRTitleStatus
{
    UINT1       nDiscNo                 : 7;
    UINT1       nValid                  : 1;
    
    UINT1       nCHRCode                : 5;
    UINT1       nRsvd                   : 2;
    UINT1       nPGPL                   : 1;

    UINT1       nTitleNum;
    UINT1       nCharByteLength;
    UINT1       nPGPLNo;
    UINT1       nDateYearF2Digit;
    UINT1       nDateYearL2Digit;
    UINT1       nDateMonth;
    UINT1       nDateDay;
    UINT1       nDateHour;
    UINT1       nDateMin;
    UINT1       nDateSec;
    UINT1       nTimeLengthHour;
    UINT1       nTimeLengthMin;

    // ...
};

struct DVDVRInfoStatus
{
    UINT1       nDiscNo                 : 7;
    UINT1       nValid                  : 1;
    
    UINT1       nCHRCode                : 5;
    UINT1       nRsvd                   : 3;
    
    UINT1       nTotalProgramNo;
    UINT1       nTotalPlayListNo;
};

struct CDTextStatus
{
    UINT1       nDiscNo                 : 7;
    UINT1       nValid                  : 1;
    
    UINT1       nTrackNo;

    UINT1       nAccordingToData        : 4;
    UINT1       nRsvd                   : 2;
    UINT1       nContinue               : 1;
    UINT1       nCHRValid               : 1;
    
    UINT1       nBlockNo;
    UINT1       nFrameNo;
    UINT1       pCHRCode[16];
};

struct CDTextInfoStatus
{
    UINT1       nDiscNo                 : 7;
    UINT1       nValid                  : 1;
    
    UINT1       ppCharLangCode[8][2];
};

struct FileNameStatus
{
    UINT1       nUndefined1             : 7;
    UINT1       nValid                  : 1;
    
    UINT1       nFolderNoUpper          : 4;
    UINT1       nByteNo                 : 1;
    UINT1       nUnicode                : 1;
    UINT1       nContinue               : 1;
    UINT1       nCHRValid               : 1;
    
    UINT1       nFolderNoLower;
    
    UINT1       nFileNoUpper            : 4;
    UINT1       nUndefined2             : 3;
    UINT1       nFile                   : 1;
    
    UINT1       nFileNoLower;
    UINT1       nBlockNoOrByteNo;
    UINT1       pCHRCode[DS_FILE_NAME_CHAR_LENGTH];
};

struct FolderNameStatus
{
    UINT1       nUndefined1             : 7;
    UINT1       nValid                  : 1;

    UINT1       nFolderNoUpper          : 4;
    UINT1       nByteNo                 : 1;
    UINT1       nUnicode                : 1;
    UINT1       nContinue               : 1;
    UINT1       nCHRValid               : 1;
    
    UINT1       nFolderNoLower;
    UINT1       nBlockNoOrByteNo;
    UINT1       pCHRCode[DS_FOLDER_NAME_CHAR_LENGTH];
};

struct PictureSettingStatus
{
    INT1        nContrast;
    INT1        nColor;
    INT1        nTint;
    INT1        nBrightness;
    INT1        nSharpness;
    UINT1       nGammaCorrection;
    UINT1       nCoringFilter;
    INT1        nBlackLevel;
    INT1        nWhiteLevel;
};

struct ReqError
{
    UINT1       nErrorDetail;
};

struct InitialSetting1Status
{
    UINT1       nParentalLevel          : 4;
    UINT1       nRegionCode             : 3;
    UINT1       nOSD                    : 1;
    
    UINT1       nSubTitleCodeUpper;
    UINT1       nSubTitleCodeLower;
    UINT1       nAudioCodeUpper;
    UINT1       nAudioCodeLower;
    UINT1       nMenuLangCodeUpper;
    UINT1       nMenuLangCodeLower;

    UINT1       nRsvd1                  : 4;
    UINT1       nSubTitleDisplay        : 1;
    UINT1       nAngleDisplay           : 1;
    UINT1       nScreenSize             : 2;
        
    UINT1       nPinNumberUpper;
    UINT1       nPinNumberLower;

    UINT1       nOpenningScr            : 4;
    UINT1       nRsvd2                  : 4;
    
    UINT1       nBackColor              : 1;
    UINT1       nRsvd3                  : 1;
    UINT1       nIRE                    : 1;
    UINT1       nPBC                    : 1;
    UINT1       nPictureMode            : 2;
    UINT1       nScreenMode             : 2;
    
    UINT1       nCountryCodeUpper;
    UINT1       nCountryCodeLower;
    
    UINT1       nVideoOutFormat         : 3;
    UINT1       nPlayerMode             : 1;
    UINT1       nRsvd4                  : 4;
};

struct InitialSetting2aStatus
{
    UINT1       nPCMSetting             : 2;
    UINT1       nDualMono               : 2;
    UINT1       nRsvd1                  : 2;
    UINT1       nAudioOutput            : 2;

    UINT1       n2CHIF                  : 4;        
    UINT1       nDSPIF                  : 4;

    UINT1       nSubWoofer              : 2;
    UINT1       nRearSp                 : 2;
    UINT1       nCenterSp               : 2;
    UINT1       nFrontSp                : 2;
    
    UINT1       nRearSpDelay            : 4;
    UINT1       nCenterSpDelay          : 4;

    UINT1       nUndefined              : 4;
    UINT1       nCompLevel              : 4;
    
    UINT1       nFrontRSpLevel          : 4;
    UINT1       nFrontLSpLevel          : 4;
    
    UINT1       nRearRSpLevel           : 4;
    UINT1       nRearLSpLevel           : 4;
    
    UINT1       nSubWooferLevel         : 4;
    UINT1       nCenterSpLevel          : 4;
    
    UINT1       nRsvd2                  : 2;
    UINT1       nDownSampling           : 2;
    UINT1       nBaseManagement         : 2;
    UINT1       n6CHStereo              : 1;
    UINT1       nDTSOutput              : 1;
    
    UINT1       nCenterBlandLevel       : 4;
    UINT1       nDOLPro12               : 3;
    UINT1       nRsvd3                  : 1;

    UINT1       nCenterWidth            : 3;
    UINT1       nPanorama               : 1;
    UINT1       nDimension              : 3;
    UINT1       nRsvd4                  : 1;
    
    UINT1       nRsvd5;
};

struct InitialSetting3Status
{
    UINT1       nPVDSVD                 : 1;
    UINT1       nRsvd2                  : 1;
    UINT1       nWMAPlayback            : 1;
    UINT1       nMP3Playback            : 1;
    UINT1       nPriorityID3Tag         : 2;
    UINT1       nRsvd1                  : 1;
    UINT1       nNoFileFolder           : 1;
    
    UINT1       nTrackDownTime          : 1;
    UINT1       nFastDownPlay           : 1;
    UINT1       nFastUpPlay             : 1;
    UINT1       nRandomCancel           : 1;
    UINT1       nRepeatCancel           : 1;
    UINT1       nRandomTrackUp          : 1;
    UINT1       nRsvd3                  : 2;


    UINT1       nHaltByHighTemp         : 1;
    UINT1       nDVDAudioPlayback       : 1;
    UINT1       nSingleSession          : 1;
    UINT1       nRsvd4                  : 5;

    UINT1       nEjectResume            : 1;
    UINT1       nTitleEnd               : 1;
    UINT1       nRsvd5                  : 6;
    
    UINT1       nRsvd7                  : 2;
    UINT1       nPlayTimeDisplay        : 1;
    UINT1       nDirPriority            : 1;
    UINT1       nRsvd6                  : 4;
    
    UINT1       nRsvd8;
    UINT1       nRsvd9;
};

struct CommonStatus
{
    UINT1       nDiscNo;
    
    UINT1       nDetectHighTemp         : 1;
    UINT1       nMechaError             : 1;
    UINT1       nReadError              : 1;
    UINT1       nOverCurrDetection      : 1;
    UINT1       nEjectLoading           : 1;
    UINT1       nWorkingMecha           : 1;
    UINT1       nDiscPosition           : 2;

    UINT1       nRsvd1                  : 1;    
    UINT1       nDRMAllUnsupported      : 1;
    UINT1       nAllUnsupported         : 1;
    UINT1       nBusInPreparation       : 1;
    UINT1       nParentalError          : 1;
    UINT1       nRegionError            : 1;
    UINT1       nLowConsumptionMode     : 1;    
    UINT1       nATAPI                  : 1;

    UINT1       nRsvd2;

    UINT1       nCDText                 : 1;
    UINT1       nCDROM                  : 1;
    UINT1       nVideoCD                : 2;
    UINT1       nCompAudio              : 1;
    UINT1       nCDDA                   : 1;
    UINT1       nIdentification         : 2;

    UINT1       nRsvd3                  : 1;
    UINT1       nCDPriorityMode         : 1;    // 1: CompAuido Priority 0: CDDA Priority
    UINT1       nCOMPVideo              : 1;
    UINT1       nDVDVR                  : 1;
    UINT1       nDVDAudio               : 1;
    UINT1       nDVDROM                 : 1;
    UINT1       nDTSCD                  : 1;
    UINT1       nDVDVideo               : 1;

    UINT1       nRsvd4                  : 2;
    UINT1       nDeckVerUp              : 2;
    UINT1       nDisplay                : 4;
    
    UINT1       nDeckVerNo1;
    UINT1       nDeckVerNo2;
    UINT1       nDeckVerNo3;
    UINT1       nDeckVerNo4;
    UINT1       nRsvd5;
    UINT1       nRsvd6;
    UINT1       nRsvd7;
    UINT1       nRsvd8;

    UINT1       nRsvd9                  : 6;
    UINT1       nJoliet                 : 1;
    UINT1       nVideoFormat            : 1;
    
    UINT1       nTempCode;
    UINT1       nRsvd10;
};

struct DVDAudioVideoVRStatus
{
    UINT1       nContents               : 2;
    UINT1       nPGPL                   : 1;
    UINT1       nPicture                : 1;
    UINT1       nPlayStatus             : 4;
    
    UINT1       nScanMode               : 3;
    UINT1       nRepeatMode             : 2;
    UINT1       nRandomMode             : 3;

    
    UINT1       nDiscNo;
    UINT1       nGroupTitlePGPLNo;

    UINT1       nTrackChapterNo100      : 4;
    UINT1       nRsvd1                  : 4;
    
    UINT1       nTrackChapterNo10_1;

    UINT1       nPlaybackTimeHour1      : 4;
    UINT1       nPlaybackTimeHour10     : 4;
    
    UINT1       nPlaybackTimeMin1       : 4;
    UINT1       nPlaybackTimeMin10      : 4;
    
    UINT1       nPlaybackTimeSec1       : 4;
    UINT1       nPlaybackTimeSec10      : 4;
    
    UINT1       nRelevantSubTitleNo     : 7;
    UINT1       nMultiAngle             : 1;
    
    UINT1       nTotalSubTitleNo;
    UINT1       nSubTitleCodeUpper;
    UINT1       nSubTitleCodeLower;
    UINT1       nRelevantAudioNo;
    UINT1       nTotalAudioNo;
    UINT1       nAudioCodeUpper;
    UINT1       nAudioCodeLower;

    UINT1       nChannel                : 4;
    UINT1       nAudioAttr              : 3;
    UINT1       nPCMDMix                : 1;  

    UINT1       nResolution             : 2;
    UINT1       nFrequency              : 3;
    UINT1       nLFE                    : 1;
    UINT1       nDSUR                   : 1;
    UINT1       nHighlight              : 1;

    UINT1       nTotalAngleNo           : 4;
    UINT1       nRelevantAngleNo        : 4;
    
    UINT1       nOnDisplay              : 4;
    UINT1       nAudioOutput            : 4;
    
    UINT1       nTotalChapterNo100      : 4;
    UINT1       nRsvd2                  : 4;
        
    UINT1       nTotalChapterNo10_1;
    UINT1       nTotalGroupTitlePGPLNo;
    UINT1       nRsvd3;
};

struct CDStatus
{
    UINT1       nDTS                    : 1;
    UINT1       nCDText                 : 1;
    UINT1       nCDROM                  : 1;
    UINT1       nRsvd1                  : 1;
    UINT1       nPlayStatus             : 4;
    
    UINT1       nScanMode               : 3;
    UINT1       nRepeatMode             : 2;
    UINT1       nRandomMode             : 3;
    
    UINT1       nDiscNo;
    UINT1       nTrackNo;
    
    UINT1       nChannel                : 4;
    UINT1       nRsvd3                  : 1;
    UINT1       nLFE                    : 1;
    UINT1       nRsvd2                  : 2;

    UINT1       nPlaybackTimeMin1       : 4;
    UINT1       nPlaybackTimeMin10      : 4;

    UINT1       nPlaybackTimeSec1       : 4;
    UINT1       nPlaybackTimeSec10      : 3;
    UINT1       nInterval               : 1;

    UINT1       nDisplay                : 4;
    UINT1       nAudioOutputMode        : 4;

    UINT1       nDirectStopMode         : 3;
    UINT1       nEmphasis               : 1;
    UINT1       nPlayWidthSpeed         : 1;
    UINT1       nJumpinessDetection     : 1;
    UINT1       nSCMSDetection          : 1;
    UINT1       nRsvd4                  : 1;
};

struct VideoCDStatus
{
    UINT1       nPBC                    : 1;
    UINT1       nRsvd1                  : 1;
    UINT1       nVer                    : 2;
    UINT1       nPlayStatus             : 4;

    UINT1       nScanMode               : 3;
    UINT1       nRepeatMode             : 2;
    UINT1       nRandomMode             : 3;
    
    UINT1       nDiscNo;
    UINT1       nTrackNo;
    
    UINT1       nPlaybackTimeHour1      : 4;
    UINT1       nPlaybackTimeHour10     : 4;
    
    UINT1       nPlaybackTimeMin1       : 4;
    UINT1       nPlaybackTimeMin10      : 4;
    
    UINT1       nPlaybackTimeSec1       : 4;
    UINT1       nPlaybackTimeSec10      : 3;
    UINT1       nRsvd2                  : 1;
    
    UINT1       nDisplay                : 4;
    UINT1       nAudioOutputMode        : 4;
    
    UINT1       nRsvd4                  : 3;
    UINT1       nEmphasis               : 1;
    UINT1       nRsvd3                  : 4;
};

struct CompAudioStatus
{
    UINT1       nTag                    : 2;
    UINT1       nType                   : 1;
    UINT1       nRsvd1                  : 1;
    UINT1       nPlayStatus             : 4;
    
    UINT1       nScanMode               : 3;
    UINT1       nRepeatMode             : 2;
    UINT1       nRandomMode             : 3;
    
    UINT1       nDiscNo;
    UINT1       nCurrFolderNoUpper;
    UINT1       nCurrFolderNoLower;
    UINT1       nFileNoUpper;
    UINT1       nFileNoLower;
    
    UINT1       nBitRate                : 4;
    UINT1       nErrorSecurity          : 1;
    UINT1       nLayer                  : 2;
    UINT1       nVer                    : 1;
    
    UINT1       nModeExtention          : 2;
    UINT1       nChannelMode            : 2;
    UINT1       nExtension              : 1;
    UINT1       nSampling               : 3;
    
    UINT1       nDirectStopMode         : 3;
    UINT1       nUnsupported            : 1;
    UINT1       nEmphasis               : 2;
    UINT1       nOriginal               : 1;
    UINT1       nCopyRight              : 1;
    
    UINT1       nPlaybackTimeHour1      : 4;    
    UINT1       nPlaybackTimeHour10     : 4;
    
    UINT1       nPlaybackTimeMin1       : 4;
    UINT1       nPlaybackTimeMin10      : 4;
    
    UINT1       nPlaybackTimeSec1       : 4;
    UINT1       nPlaybackTimeSec10      : 4;
    
    UINT1       nDisplayCondition       : 4;
    UINT1       nAudioOutputMode        : 4;
    
    UINT1       pFileName[32];

    UINT1       nRsvd2                  : 7;
    UINT1       nUnicode                : 1;
};


struct TestSPStatus
{
    UINT1       nMode;
};

struct FolderFile4Status
{
    UINT1       nUndefined              : 7;
    UINT1       nValid                  : 1;
    
    UINT1       nCurrFolderNo100        : 4;
    UINT1       nHierarchy              : 4;
    
    UINT1       nCurrFolderNo10_1;
    UINT1       nBlockNo;
    UINT1       nCurrFolderName[32];
    
    UINT1       nChildFolderNo100       : 4;
    UINT1       nRsvd1                  : 3;
    UINT1       nUnicode                : 1;
    
    UINT1       nChildFolderNo10_1;
    
    UINT1       nChildFileNo100         : 4;
    UINT1       nRsvd2                  : 4;
    
    UINT1       nChildFileNo10_1;
    struct {
        UINT1       nFileFolderNo100        : 4;
        UINT1       nPlayFile               : 1;
        UINT1       nType                   : 3;
        
        UINT1       nFileFolderNo10_1;
        UINT1       pFileFolderName[32];
    } FileFolderInfo[4];
};

struct DirectTouch
{
    UINT1       nValid                  : 1;
    UINT1       nUndefined              : 5;
    UINT1       nSelectionDecision      : 2;
};


struct BonusGrouPINNumMatching
{
    UINT1       nOKNG                   : 1;
    UINT1       nRsvd                   : 7;
};

struct FileFolderTotalNum
{
    UINT1       nDiscNo;

    UINT1       nTotalFolderNo100       : 4;
    UINT1       nUndefined1             : 3;
    UINT1       nValid                  : 1;
    
    UINT1       nTotalFolderNo10_1;

    UINT1       nTotalFileNo100         : 4;
    UINT1       nUndefined2             : 4;
    
    UINT1       nTotalFileNo10_1;
};

struct TagStatus2
{
    UINT1       nUndefined              : 7;
    UINT1       nValid                  : 1;
    
    UINT1       nOutputInfoType         : 3;
    UINT1       nID3v1                  : 1;
    UINT1       nID3v2                  : 1;
    UINT1       nUnicodeTextInfo        : 2;
    UINT1       nExist                  : 1;
    
    UINT1       nTagInfoSize;
    UINT1       pTagInfo[64];
};

struct StatusData
{
    unsigned char       m_nFH;
    unsigned char       m_nFDL;
    unsigned char       m_nCode;
    
    unsigned char       m_Rsvd;
    
    union {
        TOCStatus                  m_tTOC;
        TOCStatus2                 m_tTOC2;
        DirTreeStatus              m_tDirTree;
        TagStatus                  m_tTag;
        DVDVRTitleStatus           m_tDVDVRTitle;
        DVDVRInfoStatus            m_tDVDVRInfo;
        CDTextStatus               m_tCDText;
        CDTextInfoStatus           m_tCDTextInfo;
        FileNameStatus             m_tFileName;
        FolderNameStatus           m_tFolderName;
        PictureSettingStatus       m_tPictureSetting;
        ReqError                   m_tReqError;
        InitialSetting1Status      m_tInitSetting1;
        InitialSetting2aStatus     m_tInitSetting2a;
        InitialSetting3Status      m_tInitSetting3;
        CommonStatus               m_tCommon;
        DVDAudioVideoVRStatus      m_tDVDAudioVideoVR;
        CDStatus                   m_tCD;
        VideoCDStatus              m_tVideoCD;
        CompAudioStatus            m_tCompAudio;
        TestSPStatus               m_tTestSP;
        FolderFile4Status          m_tFolderFile4;
        DirectTouch                m_tDirectTouch;
        BonusGrouPINNumMatching    m_tBonusGrpPinNumMat;
        FileFolderTotalNum         m_tFileFolderTotalNum;
        TagStatus2                 m_tTag2;
    } m_tData;

    unsigned char m_nFCC;
};

struct DirectContCommand
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;
    
    UINT1       nDiscNo;
    
    UINT1       n1stCateUpper           : 4;
    UINT1       nUndefined1             : 2;
    UINT1       nPGPLDesggn             : 1;
    UINT1       nFileDesign             : 1;
    
    UINT1       n1stCateLower;

    UINT1       n2ndCateUpper           : 4;
    UINT1       nUndefined2             : 4;
    
    UINT1       n2ndCateLower;
};

struct DirectStopCommand
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;

    UINT1       nDiscNo;
    
    UINT1       n1stCateUpper           : 4;
    UINT1       nUndefined1             : 4;
    
    UINT1       n1stCateLower;
    
    UINT1       n2ndCateUpper           : 4;
    UINT1       nUndefined2             : 4;
    
    UINT1       n2ndCateLower;
};

struct InitialSetting1Command
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;

    UINT1       nParentalLevel          : 4;
    UINT1       nRegionCode             : 3;
    UINT1       nOSD                    : 1;
    
    
    UINT1       nSubTitleCodeUpper;
    UINT1       nSubTitleCodeLower;
    UINT1       nAudioCodeUpper;
    UINT1       nAudioCodeLower;
    UINT1       nMenuLangCodeUpper;
    UINT1       nMenuLangCodeLower;
    
    UINT1       nUndefined1             : 4;
    UINT1       nSubTitle               : 1;
    UINT1       nAngle                  : 1;
    UINT1       nScreenSize             : 2;

    UINT1       nPinNumber2             : 4;
    UINT1       nPinNumber1             : 4;
    
    UINT1       nPinNumber4             : 4;      
    UINT1       nPinNumber3             : 4;
  
    UINT1       nOpenningScreen         : 4;
    UINT1       nUndefined2             : 4;
    
    UINT1       nBackColor              : 1;
    UINT1       nUndefined3             : 1;
    UINT1       nIRE                    : 1;
    UINT1       nPBC                    : 1;
    UINT1       nVideoMode              : 2;
    UINT1       nScreenMode             : 2;
    
    UINT1       nCountryCodeUpper;
    UINT1       nCountryCodeLower;
    
    UINT1       nVideoOutType           : 3;
    UINT1       nPlayerMode             : 1;
    UINT1       nUndefined4             : 4;
};

struct InitialSetting2aCommand
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;
    
    UINT1       nPCMSetting             : 2;
    UINT1       nDualMono               : 2;
    UINT1       nUndefined1             : 2;
    UINT1       nAudioOutput            : 2;
    
    UINT1       n2chIFDownMix           : 4;
    UINT1       nDSPIFDownMix           : 4;
    
    UINT1       nSubWoofer              : 2;
    UINT1       nRearSp                 : 2;
    UINT1       nCenterSp               : 2;
    UINT1       nFrontSp                : 2;
    
    UINT1       nRearSpDelay            : 4;
    UINT1       nCenterSpDelay          : 4;
    
    UINT1       nUndefined2             : 4;
    UINT1       nCompLevel              : 4;
    
    UINT1       nFrontRSpLevel          : 4;
    UINT1       nFrontLSpLevel          : 4;
    
    UINT1       nRearRSpLevel           : 4;
    UINT1       nRearLSpLevel           : 4;
    
    UINT1       nSubWooferLevel         : 4;
    UINT1       nCenterSpLevel          : 4;

    UINT1       nUndefined3             : 2;
    UINT1       nDownSampling           : 2;
    UINT1       nBassManagement         : 2;
    UINT1       n6chStereo              : 1;
    UINT1       nDTSOutput              : 1;
    
    UINT1       nCenterBlendLevel       : 4;
    UINT1       nDOLPro12               : 3;
    UINT1       nUndefined4             : 1;
    
    UINT1       nCenterWidth            : 3;
    UINT1       nPanorama               : 1;
    UINT1       nDimension              : 3;
    UINT1       nUndefined5             : 1;

    UINT1       nUndefined6;
};

struct InitialSetting3Command
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;
    
    UINT1       nPVDSVD                 : 1;
    UINT1       nUndefined2             : 1;
    UINT1       nWMAPlay                : 1;
    UINT1       nMP3Play                : 1;
    UINT1       nPriorityID3Tag         : 2;
    UINT1       nUndefined1             : 1;
    UINT1       nNoFileFolder           : 1;
    
    UINT1       nTrackDownTime          : 1;
    UINT1       nFastDownPlay           : 1;
    UINT1       nFastUpPlay             : 1;
    UINT1       nRandomCancel           : 1;
    UINT1       nRepeatCancel           : 1;
    UINT1       nRandomTrackUp          : 1;
    UINT1       nUndefined3             : 2;
    
    UINT1       nHighTempStop           : 1;
    UINT1       nDVDAudioPlay           : 1;
    UINT1       nSingleSession          : 1;
    UINT1       nUndefined4             : 5;
    
    UINT1       nEjectResume            : 1;
    UINT1       nTitleEnd               : 1;
    UINT1       nUndefined5             : 6;

    UINT1       nUndefined7             : 2;
    UINT1       nPlayTimeDisplay        : 1;
    UINT1       nDirPriority            : 1;
    UINT1       nUndefined6             : 4;
    
    UINT1       nUndefined8;
    UINT1       nUndefined9;
};

struct BonusGroupPinNumberCommand 
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;

    UINT1       nPinNum100              : 4;
    UINT1       nPinNum1000             : 4;
    
    UINT1       nPinNum1                : 4;    
    UINT1       nPinNum10               : 4;
};

struct DirectStopSpeedSettingCommand
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;

    UINT1       nDiscNo                 : 6;
    UINT1       nMode                   : 2;
    
    UINT1       nUndefined2             : 4;
    UINT1       nUndefined1             : 3;
    UINT1       nMute                   : 1;
        
    UINT1       nUndefined3;
    
    UINT1       nUndefined5             : 4;
    UINT1       nUndefined4             : 4;
    
    UINT1       nTrackNo;
};

struct DirTreeCommand
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;

    UINT1       nUndefined              : 4;
    UINT1       nMode                   : 4;
    
    UINT1       nFolderNoUpper;
    UINT1       nFolderNoLower;
    UINT1       nBlcokNo;
};

struct FileNameCommand
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;

    UINT1       nFolderNoUpper          : 4;
    UINT1       nUndefined1             : 2;
    UINT1       nByteValue              : 1;
    UINT1       nFileDesign             : 1;
    
    UINT1       nFolderNoLower;
    
    UINT1       nFileNoUpper            : 4;
    UINT1       nUndefined2             : 4;
    
    UINT1       nFileNoLower;
    UINT1       nBlockNoByteLength; // block 번호(hex) 또는 파일명칭의 바이트 길이.
};

struct FolderNameCommand
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;

    UINT1       nFolderNoUpper          : 4;
    UINT1       nUndefined2             : 2;
    UINT1       nByteValue              : 1;
    UINT1       nUndefined1             : 1;
    
    UINT1       nFolderNoLower;
    UINT1       nBlockNoByteLength; // block 번호(hex) 또는 파일명칭의 바이트 길이.
};

struct CDTextCommand
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;

    UINT1       nDiscNo;
    UINT1       nTrackNo;
    
    UINT1       nDataType               : 4;
    UINT1       nUndefined              : 4;
    
    UINT1       nBlockNo;
    UINT1       nFrameNo;
};

struct FolderFile4Command
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;

    UINT1       nUndefined              : 4;
    UINT1       nMode                   : 4;
    
    UINT1       nFolderNoUpper;
    UINT1       nFolderNoLower;
    UINT1       nBlockNo;
};

struct DVDVRTitleCommand
{
    UINT1       nFH;
    UINT1       nFDL;
    UINT1       nCode;

    UINT1       nDiscNo;
    
    UINT1       nCharCode               : 5;
    UINT1       nUndefined              : 2;
    UINT1       nPGPL                   : 1;
    
    UINT1       nTopPGPLNo;
    UINT1       nTitleNumber;
    UINT1       nRsvd1;
    UINT1       nRsvd2;
};

class Tab2Driver;

class Tab2Protocol : public QObject, public MessageSerialPort
{
    Q_OBJECT
public:
    Tab2Protocol(QObject *parent = 0);
    virtual ~Tab2Protocol();

    int     Open();
    int     Close();
    
    int     SendMessage(const void *pData, int nLen);
    int     RecvMessage(void *pData, int nMaxLen);
    int     SendNRecvMessage(const void *pSendBuf, int nLen, void *pRecvBuf, int nMaxLen);

signals:
    void    ReadReady();

private:
    int     GetResponeCode(int RequestCode);

private:
    Tab2Driver  *m_pDriver;
};

#endif // TAB2DRIVER_H
