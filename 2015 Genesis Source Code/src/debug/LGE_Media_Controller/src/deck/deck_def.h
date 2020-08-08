#ifndef _DECK_DEF_H
#define _DECK_DEF_H

enum DECK_MEDIA_MODE
{
    eDM_MODE_CD_DA          = 0x00,
    eDM_MODE_COMP_AUDIO     = 0x01,
    eDM_MODE_VIDEO_CD_1_1   = 0x02,
    eDM_MODE_VIDEO_CD_2_0   = 0x03,
    eDM_MODE_VIDEO_CD_SVCD  = 0x04,
    eDM_MODE_CDROM          = 0x05,
    eDM_MODE_DVD_VIDEO      = 0x06,
    eDM_MODE_DTS_CD         = 0x07,
    eDM_MODE_DVD_ROM        = 0x08,
    eDM_MODE_DVD_AUDIO      = 0x09,
    eDM_MODE_DVD_VR         = 0x10,
    eDM_MODE_COMP_VIDEO     = 0x11,
    eDM_MODE_NONE           = 0x12
};

enum DECK_FAST_SPEED
{
    eDM_FAST_SPEED_X2       = 0x00,
    eDM_FAST_SPEED_X4       = 0x40,
    eDM_FAST_SPEED_X8       = 0x80,
    eDM_FAST_SPEED_X20      = 0xC0      // CD-DA, Comp-Audio에서만 20배속, 나머지는 16배속
}; 

enum DECK_SEEK_FOR_CDROM_MODE
{
    eDM_SEEK_SPEED_X1       = 0x00,
    eDM_SEEK_SPEED_X2       = 0x40,
    eDM_SEEK_SPEED_X4       = 0x80,
    eDM_SEEK_SPEED_RES      = 0xC0
};

enum DECK_SLOW_FORWARD_MODE
{
    eDM_SLOW_FORWARD_X2     = 0x00,
    eDM_SLOW_FORWARD_X4     = 0x40,
    eDM_SLOW_FORWARD_X8     = 0x80,
    eDM_SLOW_FORWARD_X16    = 0xC0
};

enum DECK_SLOW_BACKWARD_MODE
{
    eDM_SLOW_BACKWARD_X4    = 0x00,
    eDM_SLOW_BACKWARD_X8    = 0x40,
    eDM_SLOW_BACKWARD_X16   = 0x80,
    eDM_SLOW_BACKWARD_X24   = 0xC0,
};

enum DECK_MIXED_CD_MODE
{
    eDM_SWITCH_CD_DA_MODE   = 0x00, // mixed disc에서 CD-DA로 전환
    eDM_SWITCH_COMP_AUDIO   = 0X80  // mixed disc에서 COMP-AUDIO로 전환.
};

enum DECK_TOP_MNEU_SETTING
{
    eDM_TOP_MEMU_TOGGLE     = 0x00,
    eDM_TOP_MEMU_ALWAYS     = 0x01
};

enum DECK_TITLE_MENU_SETTING
{
    eDM_TITLE_MENU_TOGGLE   = 0x00,
    eDM_TITLE_MENU_ALWAYS   = 0x01
};

enum DECK_CURSOR_MODE
{
    eDM_CURSOR_UP_PUSH      = 0x80,
    eDM_CURSOR_DOWN_PUSH    = 0x40,
    eDM_CURSOR_LEFT_PUSH    = 0x20,
    eDM_CURSOR_RIGHT_PUSH   = 0x10,
    eDM_CURSOR_RELEASE      = 0x00
};

enum DECK_PERMISSION_FLAG
{
    eDM_PERMIT_CHANGE       = 0x00,
    eDM_PERMIT_NOT_CHANGE   = 0x01
};

enum DECK_TEST_SP_MODE
{
    eDM_TEST_SP_OFF             = 0x00,
    eDM_TEST_SP_FRONT_LCH_SP    = 0x01,
    eDM_TEST_SP_CENTER_SP       = 0x02,
    eDM_TEST_SP_FRONT_RCH_SP    = 0x03,
    eDM_TEST_SP_REAR_RCH_SP     = 0x04,
    eDM_TEST_SP_REAR_LCH_SP     = 0x05,
    eDM_TEST_SP_SUB_WOOFER      = 0x06,
    eDM_TEST_SP_ON              = 0x07,
    eDM_TEST_SP_STATUS_REQUEST  = 0x0F
};

enum DECK_DIR_TREE_REQ_MODE
{
    eDM_DIR_TREE_REQ_FOLDER_NO  = 0x00,
    eDM_DIR_TREE_REQ_ABOVE_TREE = 0x02,
    eDM_DIR_TREE_REQ_FOR_FILE   = 0x04
};

enum DECK_CD_TEXT_PACK_TYPE
{
    eDM_CD_TEXT_ALBUM_TRACK_TITLE   = 0x00,
    eDM_CD_TEXT_PLAYER              = 0x01,
    eDM_CD_TEXT_LYRICIST            = 0x02,
    eDM_CD_TEXT_COMPOSER            = 0x03,
    eDM_CD_TEXT_ARRANGER            = 0x04,
    eDM_CD_TEXT_MESSAGE             = 0x05
};

enum DECK_CHAR_CODE
{
    eDM_CHAR_CODE_ASCII             = 0x00,
    eDM_CHAR_CODE_ISO8859_1         = 0x11,
    eDM_CHAR_CODE_JIS               = 0x12,
    eDM_CHAR_CODE_KS_C_5601         = 0x13,
    eDM_CHAR_CODE_KS_C_5700         = 0x14         
};

enum DECK_TAG2_OUTPUT_TYPE
{
    eDM_TAG_TYPE_TITLE              = 0x00,
    eDM_TAG_TYPE_ARTIST             = 0x01,
    eDM_TAG_TYPE_ALBUM              = 0x02,
    eDM_TAG_TYPE_YEAR               = 0x03,
    eDM_TAG_TYPE_COMMENT            = 0x04,
    eDM_TAG_TYPE_GENRE              = 0x05,
    eDM_TAG_TYPE_UNDEFINED1         = 0x06,
    eDM_TAG_TYPE_UNDEFINED2         = 0x07
};

/* DECK TRACK UP MODE */
/* DVD-Video, DVD-Audio, DVD-VR */
#define eDM_NEXT_CHAPTER_BEYOND_TITLE       0x00
#define eDM_NEXT_TRACK_BEYOND_GROUP         0x00
#define eDM_NEXT_CHAPTER                    0x00
#define eDM_NEXT_PGPL                       0x40
/* CD-DA, Video-Cd */
#define eDM_NEXT_TRACK                      0x00
/* COMP-Audio */
#define eDM_NEXT_FILE_BEYOND_FOLDER         0x00
#define eDM_NEXT_FOLDER_IN_ALL_TREE         0x40
#define eDM_NEXT_FILE_IN_FOLDER             0x80

/* DECK TRACK DOWN MODE */
/* DVD-Video, DVD-Audio, DVD-VR */
#define eDM_PREV_CHAPTER_BEYOND_TITLE       0x00
#define eDM_PREV_TRACK_BEYOND_GROUP         0x00
#define eDM_PREV_CHAPTER                    0x00
#define eDM_PREV_PGPL                       0x40
/* CD-DA, Video-Cd */
#define eDM_PREV_TRACK                      0x00
/* COMP-Audio */
#define eDM_PREV_FILE_BEYOND_FOLDER         0x00
#define eDM_PREV_FOLDER_IN_ALL_TREE         0x40
#define eDM_PREV_FILE_IN_FOLDER             0x80

/* DECK RANDOM MODE */
/* DVD-Video */
#define eDM_RANDOM_CHAPTER_IN_TITLE         0x00
#define eDM_RANDOM_CHAPTER_IN_DISC          0x40
#define eDM_RANDOM_TITLE                    0x80
/* DVD-Audio */
#define eDM_RANDOM_TRACK_IN_GROUP           0x00
#define eDM_RANDOM_TRACK_IN_DISC            0x40
#define eDM_RANDOM_GROUP                    0x80
/* DVD-VR */
#define eDM_RANDOM_CHAPTER_IN_PGPL          0x00
//#define eDM_RANDOM_CHAPTER_IN_DISC        0x40
#define eDM_RANDOM_PGPL                     0x80
/* CD-DA/VIDEO-CD */
#define eDM_RANDOM_TRACK                    0x00
/* COMP-Audio */
#define eDM_RANDOM_FILE_IN_FOLDER           0x00
#define eDM_RANDOM_FILE_IN_DISC             0x40
#define eDM_RANDOM_FOLDER                   0x80
#define eDM_RANDOM_FILE_BEYOUND_FOLDER      0xC0

/* DECK REPEAT MODE */
/* DVD-Video */
#define eDM_REPEAT_CHAPTER                  0x00
#define eDM_REPEAT_TITLE                    0x40
/* DVD-Audio */
//#define eDM_REPEAT_TRACK                  0x00
#define eDM_REPEAT_GROUP                    0x40
/* DVD-VR */
//#define eDM_REPEAT_CHAPTER                0x00
#define eDM_REPEAT_PGPL                     0x40
/* CD-DA/VIDEO-CD */
#define eDM_REPEAT_TRACK                    0x00
/* COMP-Audio */
#define eDM_REPEAT_FILE                     0x00
#define eDM_REPEAT_FOLDER_UNDER_FOLDER      0x40
#define eDM_REPEAT_FOLDER_BEYOUND_FOLDER    0x80


/* DECK SCAN MODE */
/* DVD-Video */
#define eDM_SCAN_CHAPTER_IN_TITLE           0x00
#define eDM_SCAN_CHAPTER_IN_DISC            0x40
#define eDM_SCAN_TITLE                      0x80
/* DVD-Audio */
#define eDM_SCAN_TRACK_IN_GROUP             0x00
#define eDM_SCAN_TRACK_IN_DISC              0x40
#define eDM_SCAN_GROUP                      0x80
/* DVD-VR */
#define eDM_SCAN_CHAPTER_IN_PGPL            0x00
//#define eDM_SCAN_CHAPTER_IN_DISC          0x40
#define eDM_SCAN_PGPL                       0x80
/* CD-DA/VIDEO-CD */
#define eDM_SCAN_TRACK                      0x00
/* COMP-Audio */
#define eDM_SCAN_FILE_IN_FOLDER             0x00
#define eDM_SCAN_FILE_IN_DISC               0x40
#define eDM_SCAN_FOLDER                     0x80
#define eDM_SCAN_FILE_BEYOUND_FOLDER        0xC0


/* DECK KEY 10 */
#define eDM_KEY_CLEAR                       0x80
#define eDM_KEY_9                           0x02        
#define eDM_KEY_8                           0x01        
#define eDM_KEY_7                           0x80        
#define eDM_KEY_6                           0x40        
#define eDM_KEY_5                           0x20        
#define eDM_KEY_4                           0x10        
#define eDM_KEY_3                           0x08        
#define eDM_KEY_2                           0x04        
#define eDM_KEY_1                           0x02
#define eDM_KEY_0                           0x01

/* Acception Condition */
/* Disc Condition : COMMON STATUS RESPONSE */
enum DECK_DISC_POSITION
{
    eDM_DISC_POS_NO_DISC            = 0x00,
    eDM_DISC_POS_EJECT_COMPLETE     = 0x01,
    eDM_DISC_POS_LOADING_EJECTING   = 0x02,
    eDM_DISC_POS_LOADING_COMPLETE   = 0x03
};

/* Acception Condition */
/* Disc Detection : COMMON STATUS RESPONSE */
enum DECK_DISC_IDENTIFIECATION
{
    eDM_DISC_DETECT_NO_EXEUTED      = 0x00,
    eDM_DISC_DETECT_DURING_COURSE   = 0x01,
    eDM_DISC_DETECT_COMPLETED       = 0x02,
    eDM_DISC_DETECT_NA_FAILED       = 0x03
};

/* Acception Condition */
/* Activation Condition */
enum DECK_ACTIVATION_CONDITION
{
    eDM_DISC_ACTIVE_COND_STOP       = 0x00,
    eDM_DISC_ACTIVE_COND_PLAY       = 0x02,
    eDM_DISC_ACTIVE_COND_SLOW_PLAY  = 0x03,
    eDM_DISC_ACTIVE_COND_SEARCH_OUT = 0x04,
    eDM_DISC_ACTIVE_COND_PAUSE      = 0x05,
    eDM_DISC_ACTIVE_COND_FAST       = 0x06,
    eDM_DISC_ACTIVE_COND_SEARCH_IN  = 0x07,
    eDM_DISC_ACTIVE_COND_ERROR_STOP = 0x0C
};

/* Acception Condition */
/* Display Screen */
enum DECK_DISPLAY_SCREEN_MODE
{
    eDM_DISPLAY_SCREEN_PLAY         = 0x00, // During the play of disc
    eDM_DISPLAY_SCREEN_SETTING      = 0x01, // Setting screen on display.
    eDM_DISPLAY_SCREEN_ROOT_MENU    = 0x02, // ROOT menu on display
    eDM_DISPLAY_SCREEN_TITLE_MENU   = 0x03, // TITLE menu on display
    eDM_DISPLAY_SCREEN_DEFAULT      = 0x08, // Default image
    eDM_DISPLAY_SCREEN_ZOOM         = 0x09  // zooming
};

/*
#define eDM_DISPLAY_SCREEN_PLAY             0x00    // DVD-Audio/Video/VR, Video-CD, COMP-Audio
#define eDM_DISPLAY_SCREEN_DIR_TREE         0x00    // COMP-Audio
#define eDM_DISPLAY_SCREEN_SETTING          0x01    // 
#define eDM_DISPLAY_SCREEN_ROOT_MENU        0x02    // DVD-Audio/Video
#define eDM_DISPLAY_SCREEN_MENU             0x02    // Video-CD
#define eDM_DISPLAY_SCREEN_TAG              0x02    // COMP-Audio
#define eDM_DISPLAY_SCREEN_TITLE_MENU       0x03    // DVD-Audio/Video
#define eDM_DISPLAY_SCREEN_DEFAULT          0x08    //
#define eDM_DISPLAY_SCREEN_ZOOM             0x09    // DVD-Audio/Video/VR Video-CD
*/

/* Acception Condition */
/* Environment anomaly detection */
/*
enum DECK_ENV_ANOMALY_DETECTION
{
    eDM_EVN_HIGH_TEMP               = 0x
};
*/

enum DECK_FILE_FOLDER_TYPE
{
    eDM_FF_TYPE_INVALID_DATA        = 0x00,
    eDM_FF_TYPE_FOLER               = 0x01,
    eDM_FF_TYPE_MP3_FILE            = 0x02,
    eDM_FF_TYPE_WMA_FILE            = 0x03
}; 

enum DECK_COMP_AUDIO_FILE_TYPE
{
    eDM_COMP_TYPE_MP3_FILE          = 0x00,
    eDM_COMP_TYPE_WMA_FILE          = 0x01
};

enum DECK_WORKING_MECHANIC
{
    eDM_WORK_DURING_OTHER_OPER      = 0x00, // other operation
    eDM_WORK_DURING_LAOD_EJECT      = 0x01  // during the load/eject
};

enum DECK_LOADING_DIR
{
    eDM_DIR_DURING_LOADING          = 0x00, // loading direction
    eDM_DIR_DURING_EJECTION         = 0x01  // ejecting direction
};


/* COMMON STATUS RESPONSE'S ERROR */
enum DECK_READ_ERROR
{
    eDM_READ_ERROR_ESCAPE           = 0x00, // The error is cleared
    eDM_READ_ERROR_STOP             = 0x01  // An error occurs. -> ERROR STOP operates.
};

enum DECK_MECHANIC_ERROR
{
    eDM_MECHA_ERROR_ESCAPE          = 0x00,
    eDM_MECHA_ERROR_STOP            = 0x01
};

enum DECK_HIGH_TEMP_DETECT
{
    eDM_HIGH_TEMP_ESCAPE            = 0x00, // high temperature being detected.
    eDM_HIGH_TEMP_STOP              = 0x01  // normal temperature
};

enum DECK_ATAPI_BUS
{
    eDM_ATAPI_BUS_INVALID           = 0x00, // bus right on the controller.
    eDM_ATAPI_BUS_VALID             = 0x01  // bus right on the video decoder.
};

enum DECK_LOW_CONSUMPTION_MODE
{
    eDM_CONSUMPTION_NORMAL          = 0x00,
    eDM_CONSUMPTION_LOW             = 0x01   
};

enum DECK_REGION_ERROR
{
    eDM_REGION_NORMAL               = 0x00,
    eDM_REGION_ERROR                = 0x01        
};

enum DECK_PARENTAL_ERROR
{
    eDM_PARENTAL_NORMAL             = 0x00,
    eDM_PARENTAL_ERROR              = 0x01      // limited audience
};

enum DECK_BUS_IN_PREPARATION
{
    eDM_BUS_PREPARATION_COMPLETE    = 0x00,
    eDM_BUS_IN_PREPARATION          = 0x01
};

enum DECK_ALL_UNSUPPORTED
{
    eDM_ONE_MORE_RELEVANT           = 0x00,
    eDM_NO_FILE_RELEVANT            = 0x01
};

enum DECK_DRM_ALL_UNSUPPORTED
{
    eDM_ONE_MORE_NON_DRM_FILE       = 0x00,
    eDM_ALL_DRM_FILES               = 0x01 // When this bit is set to 1,“ALL SUPPORTED” bit is also set to 1.
};

enum DECK_VERSION_UPGRADE
{
    eDM_VER_NON_UPGRADE             = 0x00,
    eDM_VER_UPGRADING               = 0x01,
    eDM_VER_UPGRADE_SUCCEEDED       = 0x02,
    eDM_VER_UPGRADE_FAILED          = 0x03
};

enum DECK_VIDEO_FORMAT
{
    eDM_VIDEO_FORMAT_NTSC           = 0x00,
    eDM_VIDEO_FORMAT_PAL            = 0x01
};

enum DECK_JOLIET_FORMAT
{
    eDM_NON_JOLIET_FORMAT           = 0x00,
    eDM_JOLIET_FORMAT               = 0x01
};

enum DECK_SCREEN_SIZE
{
    eDM_SCREEN_SIZE_PS              = 0x00,
    eDM_SCREEN_SIZE_LB              = 0x01,
    eDM_SCREEN_SIZE_WIDE            = 0x02
};

enum DECK_ANGLE_DISPLAY
{
    eDM_ANGLE_MAKR_PERMITTED        = 0x00,
    eDM_ANGLE_MARK_PROHIBITED       = 0x01
};

enum DECK_SUBTITLE_DISPLAY
{
    eDM_SUBTITLE_DISPLAY_PERMITTED  = 0x00,
    eDM_SUBTITLE_DISPLAY_PROHIBITED = 0x01
};

enum DECK_OPENING_DISP_SCREEN
{
    eDM_OPENING_ECLIPSE_LOGO        = 0x00,
    eDM_OPENING_UNDEFINED           = 0x01
};

enum DECK_SCREEN_MODE
{
    eDM_SCREEN_MODE_NTSC            = 0x00,
    eDM_SCREEN_MODE_PAL             = 0x01,
    eDM_SCREEN_MODE_AUTO            = 0x02
};

enum DECK_PICTURE_MODE
{
    eDM_PICTURE_MODE_NORMAL         = 0x00,
    eDM_PICTURE_MODE_H_PRE          = 0x01,
    eDM_PICTURE_MODE_N_FLIKER       = 0x02
};

enum DECK_PLAYER_MODE
{
    eDM_PLAYER_MODE_DVD_AUDIO       = 0x00,
    eDM_PLAYER_MODE_DVD_VIDEO       = 0x01
};

enum DECK_VIDEO_OUTPUT_FORMAT
{
    eDM_VIDEO_COMPOSITE_OUTPUT      = 0x00,
    eDM_VIDEO_S_TERMINAL_OUTPUT     = 0x01,
    eDM_VIDEO_COMPONENT_OUTPUT_INT  = 0x02,
    eDM_VIDEO_COMPONENT_OUTPUT_PRO  = 0X03,
    eDM_VIDEO_RGB_OUTPUT            = 0x04
};

enum DECK_AUDIO_OUTPUT_FORMAT
{
    eDM_AUDIO_3LINES_DEGITAL        = 0x00,
    eDM_AUDIO_SPDIF_NO_MPEG_STREAM  = 0x01,
    eDM_AUDIO_SPDIF_PCM             = 0x02,
    eDM_AUDIO_SPDIF_MPEG_STREAM     = 0x03
};

enum DECK_DUAL_MONO_CONDITION
{
    eDM_DUAL_MONO_STEREO            = 0x00,
    eDM_DUAL_MONO_L_MONO            = 0x01,
    eDM_DUAL_MONO_R_MONO            = 0x02,
    eDM_DUAL_MONO_MIX_MONO          = 0x03
};

enum DECK_PCM_SETTING_CONDITION
{
    eDM_PCM_SET_UNDER_48KHZ_16BIT   = 0x00,
    eDM_PCM_SET_96KHZ_24BIT         = 0x01,
    eDM_PCM_SET_192KHZ_24BIT        = 0X02
};

enum DECK_DSP_IF_DOWN_MIX_SETTING
{
    eDM_DSP_IF_DOWN_MIX_STEREO      = 0x00,
    eDM_DSP_IF_DOWN_MIX_LT_RT       = 0x01,
    eDM_DSP_IF_DOWN_MIX_SP_LVL1     = 0x02,
    eDM_DSP_IF_DOWN_MIX_SP_LVL2     = 0x03,
    eDM_DSP_IF_DOWN_MIX_SP_LVL3     = 0x04,
    eDM_DSP_IF_DOWN_MIX_HP_LVL1     = 0x05,
    eDM_DSP_IF_DOWN_MIX_HP_LVL2     = 0x06,
    eDM_DSP_IF_DOWN_MIX_HP_LVL3     = 0x07,
    eDM_DSP_IF_DOWN_MIX_6CH_MIX     = 0x08
};

enum DECK_2CH_IF_DOWN_MIX_SETTING
{
    eDM_2CH_IF_DOWN_MIX_STEREO      = 0x00,
    eDM_2CH_IF_DOWN_MIX_LT_RT       = 0x01,
    eDM_2CH_IF_DOWN_MIX_SP_LVL1     = 0x02,
    eDM_2CH_IF_DOWN_MIX_SP_LVL2     = 0x03,
    eDM_2CH_IF_DOWN_MIX_SP_LVL3     = 0x04,
    eDM_2CH_IF_DOWN_MIX_HP_LVL1     = 0x05,
    eDM_2CH_IF_DOWN_MIX_HP_LVL2     = 0x06,
    eDM_2CH_IF_DOWN_MIX_HP_LVL3     = 0x07
};

enum DECK_SPEEKER_SETTING
{
    eDM_SPEEKER_SET_LARGE           = 0x00,
    eDM_SPEEKER_SET_SMALL           = 0x01,
    eDM_SPEEKER_SET_OFF             = 0x02
};

enum DECK_SUBWOOFER_SETTING
{
    eDM_SUBWOOFER_SET_ON            = 0x00,
    eDM_SUBWOOFER_SET_OFF           = 0x00
};

enum DECK_DTS_OUTPUT_SETTING
{
    eDM_DTS_MUTE_PLAY_SOUND         = 0x00,
    eDM_DTS_OUPUT_PLAY_SOUND        = 0x01
};

enum DECK_6CH_STEREO_SETTING
{
    eDM_6CH_STEREO_OFF              = 0x00,
    eDM_6CH_STEREO_ON               = 0x01
};

enum DECK_BASE_MANAGEMENT
{
    eDM_BASE_MNGR_MULTI_SOURCE_ON   = 0x00,
    eDM_BASE_MNGR_ALL_SOURCE_ON     = 0x01,
    eDM_BASE_MNGR_OFF               = 0x02,
    eDM_BASE_MNGR_UNDEFINED         = 0x03
};

enum DECK_DOWN_SAMPLING_SETTING
{
    eDM_DOWN_SAMPLING_OFF           = 0x00,
    eDM_DOWN_SAMPLING_ON_96_88      = 0x01,
    eDM_DOWN_SAMPLING_ON_48_44      = 0x02,
    eDM_DOWN_SAMPLING_UNDEFINED     = 0x03
};

enum DECK_DOL_PRO12_SETTING
{
    eDM_DOL_PRO12_OFF               = 0x00,
    eDM_DOL_PRO12_SUR_AUTO_ON       = 0x01,
    eDM_DOL_PRO12_SUR_ON            = 0x02,
    eDM_DOL_PRO12_MUSIC_ON          = 0x03,
    eDM_DOL_PRO12_MOVIE_ON          = 0x04,
    eDM_DOL_PRO12_MATRIX_ON         = 0x05
};

enum DECK_CD_PLAY_COND
{
    eDM_CD_PLAY_COND_HALTING                = 0x00,
    eDM_CD_PLAY_COND_PLAYING                = 0x02,
    eDM_CD_PLAY_COND_PLAYING_SLOW           = 0x03,
    eDM_CD_PLAY_COND_SEARCHING_OUT          = 0x04,
    eDM_CD_PLAY_COND_PAUSING                = 0x05,
    eDM_CD_PLAY_COND_PLAYING_FAST           = 0x06,
    eDM_CD_PLAY_COND_SEARCHING_IN           = 0x07,
    eDM_CD_PLAY_COND_TYPE_ERROR             = 0x09,
    eDM_CD_PLAY_COND_DISC_ID_INCOMPLETE     = 0x0B,
    eDM_CD_PLAY_COND_HALTING_BY_ERROR       = 0x0C,
    eDM_CD_PLAY_COND_NOT_APPLICABLE_FORMAT  = 0x0E,
    eDM_CD_PLAY_COND_NO_BUS_RIGHT           = 0x0F
};

enum DECK_CD_RANDOM_MODE
{
    eDM_CD_RANDOM_OFF               = 0x00,
    eDM_CD_RANDOM_TRACK             = 0x01,
    eDM_CD_RANDOM_DISC              = 0x04,

    eDM_CD_RANDOM_NONE              = 0xFF //  변수 초기화를 위해서 추가함.
};

enum DECK_COMP_RANDOM_MODE
{
    eDM_COMP_RANDOM_OFF             = 0x00,
    eDM_COMP_RANDOM_FILES_IN_FOLDER = 0x01,
    eDM_COMP_RANDOM_FILES_IN_DISC   = 0x02,
    eDM_COMP_RANDOM_FOLDER          = 0x03,
    eDM_COMP_RANDOM_FILE_IN_FOLDER  = 0x04,
    eDM_COMP_RANDOM_DISC            = 0x05,

    eDM_COMP_RANDOM_NONE            = 0xFF //  변수 초기화를 위해서 추가함.
};

enum DECK_DVD_RANDOM_MODE
{
    eDM_DVD_RANDOM_OFF              = 0x00,
    eDM_DVD_RANDOM_CHAPTER_IN_TITLE = 0x01,
    eDM_DVD_RANDOM_CHAPTER_IN_DISC  = 0x02,
    eDM_DVD_RANDOM_TITLE            = 0x03,
    eDM_DVD_RANDOM_DISC             = 0x04,

    eDM_DVD_RANDOM_NONE             = 0xFF //  변수 초기화를 위해서 추가함. 
};

enum DECK_CD_REPEAT_MODE
{
    eDM_CD_REPEAT_OFF               = 0x00,
    eDM_CD_REPEAT_TRACK             = 0x01,
    eDM_CD_REPEAT_DISC              = 0x03,

    eDM_CD_REPEAT_NONE              = 0xFF //  변수 초기화를 위해서 추가함.
};

enum DECK_COMP_REPEAT_MODE
{
    eDM_COMP_REPEAT_OFF             = 0x00,
    eDM_COMP_REPEAT_FILE            = 0x01,
    eDM_COMP_REPEAT_FILES_IN_FOLDER = 0x02,
    eDM_COMP_REPEAT_FILE_IN_FOLDER  = 0x03,

    eDM_COMP_REPEAT_NONE            = 0xFF //  변수 초기화를 위해서 추가함.
};

enum DECK_DVD_REPEAT_MODE
{
    eDM_DVD_REPEAT_OFF              = 0x00,
    eDM_DVD_REPEAT_CHAPTER          = 0x01,
    eDM_DVD_REPEAT_TITLE            = 0x02,
    eDM_DVD_REPEAT_DISC             = 0x03,

    eDM_DVD_REPEAT_NONE             = 0xFF //  변수 초기화를 위해서 추가함.
};

enum DECK_CD_SCAN_MODE
{
    eDM_CD_SCAN_OFF                 = 0x00,
    eDM_CD_SCAN_TRACK               = 0x01,
    eDM_CD_SCAN_DISC                = 0x04,

    eDM_CD_SCAN_NONE                = 0xFF //  변수 초기화를 위해서 추가함.
};

enum DECK_COMP_SCAN_MODE
{
    eDM_COMP_SCAN_OFF               = 0x00,
    eDM_COMP_SCAN_FILES_IN_FOLDER   = 0x01,
    eDM_COMP_SCAN_FILES_IN_DISC     = 0x02,
    eDM_COMP_SCAN_FOLDER            = 0x03,
    eDM_COMP_SCAN_FILE_IN_FOLDER    = 0X04,
    eDM_COMP_SCAN_DISC              = 0x05,

    eDM_COMP_SCAN_NONE              = 0xFF //  변수 초기화를 위해서 추가함.
};

enum DECK_DVD_SCAN_MODE
{
    eDM_DVD_SCAN_OFF                = 0x00,
    eDM_DVD_SCAN_CHAPTER_IN_TITLE   = 0x01,
    eDM_DVD_SCAN_CHAPTER_IN_DISC    = 0x02,
    eDM_DVD_SCAN_TITLE              = 0x03,
    eDM_DVD_SCAN_DISC               = 0x04,

    eDM_DVD_SCAN_NONE               = 0xFF //  변수 초기화를 위해서 추가함.
};

enum DECK_CD_CHANNEL_MODE
{
    eDM_CD_CHANNEL_DUAL_MONO        = 0x00,
    eDM_CD_CHANNEL_C                = 0x01,
    eDM_CD_CHANNEL_L_R              = 0x02,
    eDM_CD_CHANNEL_L_R_C            = 0x03,
    eDM_CD_CHANNEL_L_R_S            = 0x04,
    eDM_CD_CHANNEL_L_R_C_S          = 0x05,
    eDM_CD_CHANNEL_L_R_LS_RS        = 0x06,
    eDM_CD_CHANNEL_L_R_C_LS_RS      = 0x07,
    eDM_CD_CHANNEL_INVALID          = 0x0F
};

enum DECK_COMP_CHANNEL_MODE
{
    eDM_COMP_CHANNEL_STEREO         = 0x00,
    eDM_COMP_CHANNEL_JOINT_STEREO   = 0x01,
    eDM_COMP_CHANNEL_DUAL_STEREO    = 0x02,
    eDM_COMP_CHANNEL_MONO           = 0x03
};

enum DECK_CD_AUDIO_OUTPUT_MODE
{
    eDM_CD_AUDIO_OUTPUT_ANALOG_2CH          = 0x00,
    eDM_CD_AUDIO_OUTPUT_ANALOG_MULTI_CH     = 0x01,
    eDM_CD_AUDIO_OUTPUT_DISTAL_STREAM       = 0x02,
    eDM_CD_AUDIO_OUTPUT_DISTAL_PCM          = 0x03
};

enum DECK_VCD_AUDIO_OUTPUT_MODE
{
    eDM_VCD_AUDIO_OUTPUT_L_R                = 0x00,
    eDM_VCD_AUDIO_OUTPUT_L_MONO             = 0x01,
    eDM_VCD_AUDIO_OUTPUT_R_MONO             = 0x02,
    eDM_VCD_AUDIO_OUTPUT_MIX_MONO           = 0x03
};

enum DECK_DIRECT_STOP_MODE
{
    eDM_DIRECT_STOP_OFF                     = 0x00,
    eDM_DIRECT_STOP_PLAYING                 = 0x01,
    eDM_DIRECT_STOP_ABNORMAL_COMPLETE       = 0x03,
    eDM_DIRECT_STOP_NORMAL_COMPLETE         = 0x07
};

enum DECK_VIDEO_CD_VER
{
    eDM_VIDEO_CD_VER_1_1                    = 0x00,
    eDM_VIDEO_CD_VER_2_0                    = 0x01,
    eDM_VIDEO_CD_VER_SVCD                   = 0x02,
    eDM_VIDEO_CD_VER_UNDEFINED              = 0x03
};

enum DECK_COMP_AUDIO_TAG_EXIST
{
    eDM_TAG_NOT_READ_TAG                    = 0x00,
    eDM_TAG_NO_INFOR                        = 0x01,
    eDM_TAG_INFOR_EXIST                     = 0x02,
    eDM_TAG_SEARCHING_INFO                  = 0x03
};

enum DECK_COMP_AUDIO_MPEG_VER
{
    eDM_COMP_AUDIO_MPEG_2                   = 0x00,    
    eDM_COMP_AUDIO_MPEG_1                   = 0x01
};

enum DECK_COMP_AUDIO_MPEG_LAYER
{
    eDM_COMP_AUDIO_MPEG_LAYER_3             = 0x01,
    eDM_COMP_AUDIO_MPEG_LAYER_2             = 0x02,
    eDM_COMP_AUDIO_MPEG_LAYER_1             = 0x03,
};

enum DECK_DVD_CONTENT
{
    eDM_DVD_VIDEO_CONTENT                   = 0x00,
    eDM_DVD_AUDIO_CONTENT                   = 0x01,
    eDM_DVD_VR_CONTENT                      = 0x02,
    eDM_DVD_CONTENT_UNDEFINED               = 0x03
};

enum DECK_DVD_AUDIO_ATTRIBUTE
{
    eDM_DVD_AUDIO_ATTR_LPCM                 = 0x00,
    eDM_DVD_AUDIO_ATTR_DOLBY_DIGITAL        = 0x01,
    eDM_DVD_AUDIO_ATTR_DTS                  = 0x02,
    eDM_DVD_AUDIO_ATTR_MPEG                 = 0x03,
    eDM_DVD_AUDIO_ATTR_SDDS                 = 0x04,
    eDM_DVD_AUDIO_ATTR_PCM                  = 0x05,
    eDM_DVD_AUDIO_ATTR_NOT_APPLICABLE       = 0x07
};

enum DECK_DVD_QUANTI_FREQUENCY
{
    eDM_DVD_QUANTI_FREQUENCY_44KHZ          = 0x00,
    eDM_DVD_QUANTI_FREQUENCY_48KHZ          = 0x01,
    eDM_DVD_QUANTI_FREQUENCY_96KHZ          = 0x02,
    eDM_DVD_QUANTI_FREQUENCY_88KHZ          = 0x03,
    eDM_DVD_QUANTI_FREQUENCY_176KHZ         = 0x04,
    eDM_DVD_QUANTI_FREQUENCY_192KHZ         = 0x05,
    eDM_DVD_QUANTI_FREQUENCY_ABOVE          = 0x07
};

enum DECK_DVD_QUANTI_RESOLUTION
{
    eDM_DVD_QUANTI_RESOLUTION_16BIT         = 0x00,
    eDM_DVD_QUANTI_RESOLUTION_20BIT         = 0x01,
    eDM_DVD_QUANTI_RESOLUTION_24BIT         = 0x02,
    eDM_DVD_QUANTI_RESOLUTION_ABOVE         = 0x03
};

// removed by cychoi 2013.08.10 for unused code

#endif
