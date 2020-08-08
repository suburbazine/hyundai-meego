#ifndef MEDIAPLAYER_DEF_H
#define MEDIAPLAYER_DEF_H

#define USE_FILE_INDEX_IN_FOLDER        0

enum MEDIA_PLAYER_MODE
{
    MP_MODE_JUKEBOX,
    MP_MODE_USB,
    MP_MODE_DISC,
    MP_MODE_iPOD,
    MP_MODE_BT,
    MP_MODE_NONE
};

//////////////////////////////////////////////////////////////////////////////////
// RESPONSE CODE
#define MP_DS_CODE_TOC_STATUS              0x71
#define MP_DS_CODE_TOC_STATUS2             0x70
#define MP_DS_CODE_DIR_TREE_STATUS         0x74
#define MP_DS_CODE_TAG_STATUS              0x76
#define MP_DS_CODE_DVD_VR_TITLE_STATUS     0x77
#define MP_DS_CODE_DVD_VR_INFOR_STATUS     0x78
#define MP_DS_CODE_CD_TEXT_STATUS          0x7A
#define MP_DS_CODE_CD_TEXT_INFOR_STATUS    0x7B
#define MP_DS_CODE_FILE_NAME_STATUS        0x7C
#define MP_DS_CODE_FOLDER_NAME_STATUS      0x7D
#define MP_DS_CODE_PICTURE_SETTING_STATUS  0x7E
#define MP_DS_CODE_REQ_ERR                 0x7F
#define MP_DS_CODE_INIT_SET1_STATUS        0x80
#define MP_DS_CODE_INIT_SET2A_STATUS       0xC1
#define MP_DS_CODE_INIT_SET3_STATUS        0x82
#define MP_DS_CODE_COMMON_STATUS           0x83
#define MP_DS_CODE_DVD_AUDIOVIDEOVR_STATUS 0x84
#define MP_DS_CODE_CD_STATUS               0x86
#define MP_DS_CODE_VIDEO_CD_STATUS         0x87
#define MP_DS_CODE_COMP_AUDIO_STATUS       0x88
#define MP_DS_CODE_PARENTAL_CHANGE_REQUEST 0x89
#define MP_DS_CODE_TEST_SP_STATUS          0x8B
#define MP_DS_CODE_FOLDERFILE4_STATUS      0x8C
#define MP_DS_CODE_DIRECT_TOUCH            0x8D
#define MP_DS_CODE_BONUS_GRP_PIN_NUM_REQ   0x8E
#define MP_DS_CODE_BONUS_GRP_PIN_NUM_MAT   0x8F
#define MP_DS_CODE_FILEFONDER_TOTAL_NUMBER 0xA3
#define MP_DS_CODE_TAG_STATUS2             0xC6
#define MP_DS_CODE_RESET_START             0xFD
#define MP_DS_CODE_ACC_ON                  0xFE

enum MEDIA_MODE {
    SCANOFF,	        // 0. Scan is off
    SCANFILE,           // 1. Scan single folder
    SCANFOLDER,         // 2. Scan All folder ---> not used.
    
    RANDOMOFF,          // 3. Random is off
    RANDOMFILE,         // 4. Random single file from the list
    RANDOMFOLDER,       // 5. Random play from all files in the current drive

    REPEATOFF,          // 6. Repeat is off ---> not used.
    REPEATFILE,         // 7. Repeat single file
    REPEATFOLDER,       // 8. Repeat folder, album or playlist
    REPEATALL,          // 9. Repeat all the files
    
    SCANALL             // 10. Scan all the files
};

enum MEDIA_FAST_SPEED
{
    FAST_SPEED_X1           = 1,
    FAST_SPEED_X4           = 4,
    FAST_SPEED_X16          = 16,
    FAST_SPEED_X20          = 20
};


enum RIPPING_STATE
{
    RIPPING_STATE_START,        // ripping started
    RIPPING_STATE_PAUSE,        // ripping started
    RIPPING_STATE_RESUME,       // ripping resumed
    RIPPING_STATE_STOP          // ripping canceled
};

// modified by kihyung 2013.09.12 for ISV 90634
enum PLAYER_STATE
{
    PLAYER_PLAY = 0,            // 0
    PLAYER_PAUSE,               // 1
    PLAYER_STOP,                // 2
    PLAYER_REPEATPLAY,          // 3
    PLAYER_REPEATPAUSE,         // 4
    PLAYER_RANDOMPLAY,          // 5
    PLAYER_RANDOMPAUSE,         // 6
    PLAYER_FASTFORWARD,         // 7
    PLAYER_REWIND,              // 8
    PLAYER_NEXTTRACK,           // 9
    PLAYER_PREVTRACK,           // 10
    PLAYER_SCAN,                // 11
    PLAYER_SEARCH,              // 12
    PLAYER_FASTFORWARD_X4,      // 13
    PLAYER_FASTFORWARD_X16,     // 14
    PLAYER_FASTFORWARD_X20,     // 15
    PLAYER_REWIND_X4,           // 16
    PLAYER_REWIND_X16,          // 17
    PLAYER_REWIND_X20,          // 18

    PLAYER_CHANGE_PLAY_ICON = 1000, // 1000
    PLAYER_CHANGE_PAUSE_ICON        // 1001
};

enum SYSTEM_STATE
{
    SYSTEM_AV_OFF,
    SYSTEM_AV_ON,
    SYSTEM_POWER_OFF,
    SYSTEM_POWER_ON,
    SYSTEM_ACC_OFF,
    SYSTEM_ACC_ON,
    SYSTEM_IPODDEVICE_RESET_ON,     // added by jungae 2012.10.09 for CR14252
    SYSTEM_IPODDEVICE_RESET_OFF,    // added by jungae 2012.10.09 for CR14252
    SYSTEM_SLOW_COPY_SPEED,         // added by kihyung 2012.10.29
    SYSTEM_NORMAL_COPY_SPEED,       // added by kihyung 2012.10.29
    SYSTEM_IPODDEVICES_TABOFF,
    SYSTEM_IPODDEVICES_TABON,
    SYSTEM_CLEAR_DISPLAY_BG,
    SYSTEM_CLEAR_DISPLAY_FG,
    SYSTEM_USB1_MOUNTED,
    SYSTEM_USB2_MOUNTED,    
    SYSTEM_USB1_REMOVED,
    SYSTEM_USB2_REMOVED,
    SYSTEM_DO_NOT_SEND_METAINFO,
    SYSTEM_REQUEST_MUTE,
    SYSTEM_BT_CALL_START,
    SYSTEM_BT_CALL_END
};

enum DeviceState
{
    DiscNotPresent = 100,   // common for all disc
    DiscPresnt,             // common for all disc
    CDDADiscError,          // CDDA disc related error
    VCDDiscError,           // VCD related error
    DVDDiscError,           // DVD related error
    CADiscError,            // CA disc related error
    DiscError
};

enum DISC_STATE
{
    PLAYER_DVDVIDEOINSERTED,    // 0
    PLAYER_DVDAUDIOINSERTED,    // 1
    PLAYER_VCDINSERTED,         // 2
    PLAYER_CDDAINSERTED,        // 3
    PLAYER_CAINSERTED,          // 4
    PLAYER_DISCLOADED,          // 5
    PLAYER_DISCEJECTED,         // 6
    PLAYER_DISCLOADING,         // 7
    PLAYER_DISCEJECTING,        // 8
    PLAYER_DISCRELOADING,       // 9    
    PLAYER_DISCRELOADED,        // 10
    PLAYER_DISCREMOVED          // 11
};

enum SCREEN_STATE
{
    PLAYER_PLAY_DISPLAY,
    PLAYER_SETTING_DISPLAY,
    PLAYER_ROOTMENU_DISPLAY,
    PLAYER_TITLEMENU_DISPLAY,
    PLAYER_DEFAULTSCREEN_DISPLAY,
    PLAYER_TOPMENU_DISPLAY, 
    PLAYER_DIR_TREE_DISPLAY, 
    PLAYER_TAG_SCREEN_DISPLAY
};

enum MEDIA_PLAYER_ERROR
{
    /* USB/Jukebox Error*/
    eMP_ERR_UNSUPPORTED_FILE	= 0x01,
    eMP_ERR_STREAM,
    eMP_ERR_RESOURCE,
	eMP_ERR_SEEK_EVT_FAIL,
	eMP_ERR_OTHERDATAERROR,
	eMP_ERR_ELEMENT_CREATION_FAIL,
	eMP_ERR_UNSUPPORTED_OPERATION,
	eMP_ERR_MEDIA_INFO_UNAVAILABLE,
	eMP_ERR_SUBTITLE_FILE_NOTFOUND,
	eMP_ERR_IMAGE_THUMB_NAIL_UNAVAILABLE,
	eMP_ERR_IMAGE_COVER_ART_UNAVAILABLE,
	eMP_ERR_RATE_CHANGE_FAILED,
	eMP_ERR_VIDEO_THUMB_NAIL_UNAVAILABLE,
	eMP_ERR_MUTE_CHANGE_FAILED,
	eMP_ERR_FORMAT_ERROR,
	eMP_ERR_CANNOT_CONNECT_TO_XSERVER,
	eMP_ERR_CANNOT_OPEN_CONNECTION_TO_DISPLAY,
	eMP_ERR_OVERLAY_COLOR_CORRECTION_FAILED,
    eMP_ERR_UNSUPPORTED_ALL_FILE,
    eMP_ERR_AUTOAUDIOSINK_FAIL, // added by eugene.seo 2013.04.03
    eMP_ERR_DRM_UNAUTHORIZED_FILE_DRM,
    eMP_ERR_DIVX_AUDIO_NOT_SUPPORTED,
    eMP_ERR_DIVX_VIDEO_NOT_SUPPORTED,
    eMP_ERR_UNSUPPORTED_ALL_FILES_IN_FOLDER,
    eMP_ERR_NONE,
    
    /* FileInterface Error */
    eMP_SAME_FOLDER_EXIST_INCREATE,     // error for same folder exist at create.
    eMP_SAME_FOLDER_EXIST_INEDIT,       // error for same folder exist at edit.
    eMP_SAME_FILE_EXIST_INEDIT,         // error for same file exist at edit.
    eMP_SAME_FILE_EXIST_INCOPY,         // error for same file exist at copying destination
    eMP_LESS_SPACE_ERROR,               // less space error exist for copying
    eMP_CORRUPT_FILE_COPY_ERROR,        // corrupt file copy error
    eMP_UNSUPPORTED_FORMAT_COPY_ERROR,  // unsupported format copy error
    eMP_JUKEBOX_ERROR,                  // JukeBox  error
    eMP_USER_CANCEL_COPY_ERROR,  // user cancel copy //added by aettie.ji 2013.01.29 for ISV 71343
    
    /* DVD Controller Error */
    PLAYER_FILENOTSUPPORTED = 0x51,
    PLAYER_FILENOTFOUND,        // 
    PLAYER_COMMANDNOTSUPPORTED,
    PLAYER_INVALIDSTATE,
    PLAYER_DISCCORRUPTED,
    PLAYER_DISCNOTLOADED,
    PLAYER_HIGHTEMPERATURE,
    PLAYER_NOMEDIA,             // The is no current media. The player is in the StoppedState.
    PLAYER_DISC_READ_ERROR,
    PLAYER_DISC_MECHANIC_ERROR,
    PLAYER_DISC_LOW_CONSUMPTION,
    PLAYER_DISC_REGION_ERROR,
    PLAYER_DISC_PARENTAL_ERROR,
    PLAYER_DISC_DRM_ALL_UNSUPPORTED,
    PLAYER_NO_ERROR,
    
    /* iPod Controller Errors should start from 1000 */
    eMP_IPOD_REMOTE_UI_IMPOSSIBLE = 1000,
    eMP_IPOD_NO_ERROR
};

enum PLAYER_MEDIASTATUS 
{
    PLAYER_UnknownMediaStatus,  // The status of the media cannot be determined.
    PLAYER_NoMedia,             // The is no current media. The player is in the StoppedState.
    PLAYER_LoadingMedia,        // The current media is being loaded
    PLAYER_LoadedMedia,         // The current media has been loaded
    PLAYER_StalledMedia,        // Playback of the current media has stalled due to insufficient buffering or some other temporary interruption. The player is in the PlayingState or PausedState.
    PLAYER_BufferingMedia,      // The player is buffering data but has enough data buffered for playback to continue for the immediate future. The player is in the PlayingState or PausedState.
    PLAYER_BufferedMedia,       // The player has fully buffered the current media. The player is in the PlayingState or PausedState.
    PLAYER_EndOfMedia,          // Playback has reached the end of the current media.
    PLAYER_InvalidMedia         // The current media cannot be played. The player is in the StoppedState.
};


enum CURSOR_DIRECTION
{
    CURSOR_LEFT,
    CURSOR_RIGHT,
    CURSOR_UP,
    CURSOR_DOWN
};

enum TEN_KEY_ID
{
    KEY_ID_CLEAR,
    KEY_ID_0,
    KEY_ID_1,
    KEY_ID_2,
    KEY_ID_3,
    KEY_ID_4,
    KEY_ID_5,
    KEY_ID_6,
    KEY_ID_7,
    KEY_ID_8,
    KEY_ID_9
};

enum KEY_PRESS_STATUS
{
    KEY_PRESSED,
    KEY_RELEASED
};

enum SLOW_FORWARD_MODE
{
    SLOW_FORWARD_X2,
    SLOW_FORWARD_X4,
    SLOW_FORWARD_X8,
    SLOW_FORWARD_X16
};

enum SLOW_BACKWARD_MODE
{
    SLOW_BACKWARD_X4,
    SLOW_BACKWARD_X8,
    SLOW_BACKWARD_X16,
    SLOW_BACKWARD_X24
};

enum SYSTEM_STATE_MODE
{
    SYSTEM_STATE_AV_OFF,
    SYSTEM_STATE_AV_ON,
    SYSTEM_STATE_POWER_OFF,
    SYSTEM_STATE_POWER_ON,
    SYSTEM_STATE_ACC_OFF,
    SYSTEM_STATE_ACC_ON
};

enum IMAGE_ZOOM_LEVEL
{
    ZOOM_LEVEL_MINUS_4X,
    ZOOM_LEVEL_MINUS_2X,
    ZOOM_LEVEL_1X,
    ZOOM_LEVEL_PLUS_2X,
    ZOOM_LEVEL_PLUS_4X,
};

enum USB_DEVICE_STATE
{
    USB_CONNECTED,
    USB_DISCONNECTED
};

enum PLAYER_ENGINE_TYPE 
{ 
    GPLAYER_ENGINE,
    MPLAYER_ENGINE 
};

enum MEDIA_TYPE
{
    /*
    VIDEO_FILE = 0x01,
    AUDIO_FILE,
    IMAGE_FILE
    */
    UNSUPPORTED_FILE = 0x00,
    VIDEO_FILE,	
    AUDIO_FILE,	
    IMAGE_FILE,
    DVD_DEVICE,
    AUX_DEVICE
};

enum OVERWRITE_EXIST_TYPE
{
    OVERWRITE_ONE_EXIST_FILE_MODE	= 0x01,	
    OVERWRITE_ALL_EXIST_FILE_MODE,	
    NOT_OVERWRITE_EXIST_FILE_MODE,
    CANCEL_COPY_OPERATION_MODE
};

enum MEDIA_PLAYER_SUBTITLE_FONTSIZE
{
    eMP_SUBTITLEFONT_SMALL = 0x00,
    eMP_SUBTITLEFONT_NORMAL,
    eMP_SUBTITLEFONT_LARGE,
    eMP_SUBTITLEFONT_NONE    
};

enum MEDIA_DISPLAY 
{
    eMP_DISPLAY_B = 1, /* Overlay Plane */
    eMP_DISPLAY_A = 2  /* Sprite-C */
};

enum MEDIA_DRM_MESSAGES 
{ 
    eMP_DRM_NONE = 0x00, 
    eMP_DRM_RENTAL_STATUS, 
    eMP_DRM_RENTAL_EXPIRED, 
    eMP_DRM_NOT_AUTHORIZED 
}; 

enum MEDIA_UNSUPPORTED_REASON
{
    eMP_MEDIA_UNSUPPORTED_GENERAL,
    eMP_MEDIA_SUPPORTED,
    eMP_MEDIA_UNSUPPORTED_AUDIO,
    eMP_MEDIA_UNSUPPORTED_VIDEO
};

#include <QString>
#include <QStringList>
#include <QList>

enum PLAY_LIST_CATEGORY
{
    PL_CATEGORY_NONE,
    PL_CATEGORY_SONG,
    PL_CATEGORY_PLAYLIST,
    PL_CATEGORY_ALBUM,
    PL_CATEGORY_FOLDER,
    PL_CATEGORY_OTHER
};

struct PlayList
{
    QString          PlaylistName;
    int              index;
    double           position;
    int              mode; // not used.
    QList<QString>  *list;
    int              category;
    int              isMLT; //added by jeeeun for MLT play in FF/REW 2012.12.05
};

struct MediaInfo
{
    QString          Title;
    QString          Artist;
    QString          Album;
    QString          Genre;
    QString          fileName;
    QString          Composer;
    
    int              width;
    int              height;
    
    QString          fileformat; /*FileFormat----Only for Image-----JPEG,BMP,PNG,GIF*/
    QString          coverarturl;
    QString          thumbnailurl;
    int              fileduration;

    QString          bitrate;
    QString          createdate;
    QString          modificationdate;
};

struct DeviceInfo
{
    QStringList      ObjectPathList;
    QStringList      MountNameList;
    QStringList      SerialNumberList;
    // 
};

struct DvdcTOC
{
    int valid; // Indicate if the data in the structure is valid.
    int start_track; // id of the first track in the disc.
    int last_track; // id of the first track in the disc.
    int total_play_min;  // total play time: minutes
    int total_play_sec; ;  // total play time: seconds
    int total_play_frame; // total  play frames
};

struct DvdcTOC2
{
    int valid; // Indicate if the data in the structure is valid.
    int block_no; // block number passed in the request.
    int play_start[10][3]; //10: Number of tracks per block; 3: Minutes, Secs, Frames respectively.
};

struct DvdcDirEntry
{
    int type; // Indicates type of the entry such as file(WMA/MP3),folder,
    int playing_now_or_next; //
    int longname; //"LENGTH" field of command.
    int file_folder_num; // file/folder number in the disc.
    char *file_folder_name; // file or folder name.
};

struct DvdcDirTree
{
    int valid; // Indicate if the data in the structure is valid.
    int curr_folder; 
    int block_no; //current block number. One block contains 10 file/folder.
    char *folder_name;
    int unicode;  // Indicate if Unicode characters used.
    int chld_folder_num; // Number of child folders.
    int chld_file_num; // Number of child files.
    struct DvdcDirEntry *entry[10];
};

struct DvdcTagInfo
{
    int valid; // Indicate if the data in the structure is valid.
    int exist;
    int unicode_info;
    char title[30]; //Title of the music
    char artist[30]; // Name of the artist
    char album[30]; // album name
    char year[4]; // year
    char comment[30]; // comment
    int genre; // Genre of the music
}; 

struct DvdcTag2Info 
{
    int valid; // Indicate if the data in the structure is valid.
    int exist; // indicate presence of tag data for current file
    int unicode_text; 
    int id3v2; // Id3 version 2 Info.
    int id3v1; // Id3 version 1 Info.
    int out_info_type; // Enum representing output type such as Artist, Title etc.
    int tag_info_size; // Size of out put data. Max size id 64 bytes.
    char tag_info[64]; // Actual data
};

struct DvdcAllTagInfo
{
    int     valid;
    int     exist;
    int     out_info_type;
    QString title; //Title of the music
    QString artist; // Name of the artist
    QString album; // album name
    QString genre; // Genre of the music
}; 

struct DvdcFileName
{
    int valid; // Indicate if the data in the structure is valid.
    int unicode; 
    int folder_no; // folder number
    int file_no; // file number
    char name[130]; // file name characters.
};

struct DvdcFolderName
{
    int valid; // Indicate if the data in the structure is valid.
    int unicode; 
    int folder_no; // folder number
    char name[130]; // file name characters.
};

struct DvdcVRTitleEntry
{ 
    char rec_year[4];
    char rec_month[2];
    char rec_date[2];
    char rec_hour[2];
    char rec_min[2];
    char rec_sec[2];
    char rec_len_hour[2];
    char rec_len_min[2];
    char title_name[64]; //64 is max, defined in table on pg 162 of TAB2 spec.
};

struct DvdcVRTitle
{
    int valid; // Indicate if the data in the structure is valid.
    int pg_pl;
    int chr_code;
    int obtained_title_no;
    int obtained_charbyte_len;
    struct DvdcVRTitleEntry vr_entry[10];
};

struct DvdcVRInfo
{
    int valid; // Indicate if the data in the structure is valid.
    int chr_code;
    int total_pg_no;
    int total_pl_no;
};

struct DvdcCDText
{
    int valid; // Indicate if the data in the structure is valid.
    int chr_valid;
    int conti_flag;
    int data_type;
    int track_no; // Track no. passed in the request.
    int block_no;  // Block no. passed in the request.
    int frame_no; // Frame no. passed in the request.
    char chr_code[16]; // CD text 
};

struct  DvdcCdTextInfo
{
    int valid; // Indicate if the data in the structure is valid.
    int chr_code[8]; // Character encoding used for CD Text.
    int lang_code[8]; // Languages supported  for CD Text.
};

struct DvdcInit1 
{
    bool osd_on; // 0/1 decide if OSD is played by Video Decoder or not.
    int  region_code; //Designate Region code by value.
    int parental_level; // Designate Parental level by value 0-8
    int  subtitle_prio; // Designate Priority Subtitle code by value.
    int  audio_prio; // Designate Priority Audio code by value.
    int menu_lang_prio;// Designate Priority Menu language
    int screen_size; // Value representing screen size (0-3)
    int  angle_mark; //  Angle mark display when OSD ON.
    int subtitle_on; // Subtitle on/off specification.
    char pin[4]; //4 digit PIN number.
    int open_screen; // Opening screen selection. Valid at only OSD ON.
    int country_code; // Country code by ASCII code.
    bool dvd_audio_mode; // represents the player mode of DVD-Audio with DVD-Video contents.
    int video_out; // Video output type such as composite, s-type, RGB etc.
};

struct  DvdcInit2a
{
    int audio_output;  // Audio output configuration such as 3 line/SPDIF/PCM/STREAM
    int dual_mono; // represents the current condition of DUAL MONO SETTING.
    int pcm_setting;  // PCM frequency level setting
    int dsp_if_downmix; // represents the current condition of down mix setting to dsp i/f.
    int ch2_if_downmix; // represents the current condition of down mix setting to 2 ch i/f
    int front_spk; //represents the current condition of front speaker setting
    int center_spk; // //represents the current condition of centre speaker setting
    int rear_spk; //represents the current condition of rear speaker setting
    int sub_wfr; //represents the current condition of sub woofer setting
    int center_spk_delay;  //center speaker delay setting
    int rear_spk_delay; //center speaker delay setting
    int comp_level; // compression signal level
    int front_l_spk_level; // front left speaker level
    int front_r_spk_level;  // front right speaker level
    int rear_l_spk_level; // rear left speaker level
    int rear_r_spk_level; // rear right speaker level
    int center_spk_level; // center right speaker level
    int sub_wfr_level; // sub woofer level
    bool dts_out; // DTS output setting
    bool stereo_6ch; // 6 channel stereo on/off
    int bass_mgmt; // bass management on/off
    int down_sampling; // Down Sampling setting of 3 lines (Digital)output
    int dol_pro_12; // represents the Dolby ProLogic1/2 ON/OFF setting condition.
    int center_blend_lvl; //represents the distributing condition of Center ch to Front ch when DOLBY Surround(DOLBY ProLogic1) is ON.
    int dimension; // represents the DOLBY ProLogic2 dimension setting.
    bool panorama; // represents the DOLBY ProLogic2 panorama setting.
    int center_width; //represents the current condition of DOLBY ProLogic2 center width setting.
}; 

struct DvdcInit3 
{
    bool no_file_folder; // represents how to handle a folder  without file in it
    int id3_tag_prio; // represents the ID3 Tag priority reference version of MP3.
    bool mp3_play;  // represents how to handle MP-3 file during the MP-3/WMA playback.
    bool wma_play; // represents how to handle WMA file during the MP-3/WMA playback.
    bool pvd_svd; // represents a volume descriptor that file analysis gives priority.
    bool random_trackup_random; // represents an operation when TRACK UP notice is accepted during random playback.
    bool repeat_cancel; // represents repeat hold/cancel operation.
    bool random_cancel; // represents random hold/cancel operation.
    bool fastup_play; //represents an operation when a play fast-forwards to the end of a disc/folder.
    bool fastdown_play; //
    bool trackdown_time;
    bool single_session; 
    bool dvd_audio_compliance; 
    bool high_temp_stop;
    bool resume_data_eject; 
    bool title_end_stop; 
    bool dir_prio; 
    int playtime_disp;
};

struct DvdcFileFolderItem
{
    int type; // Enu for type of file/folder
    int playStatus; // 1: current/next file playing. 0:other
    int fileOrFolderNo;
    char fileOrFolderName[32];
};

struct DvdcFileFolderList
{
    int valid; // Indicate if the data in the structure is valid.
    int hierarchy;// indicates level of the current folder.
    int currentFolderNo;
    int blockNo;
    char currFolderName[32];
    int unicode;
    int childFolderNo;
    int childFileNo;
    DvdcFileFolderItem fileOrFolderList[3];
};

struct TotalFileFolderCount
{
    int valid; // Indicate if the data in the structure is valid.
    int totalFolders;
    int totalFiles;
};

struct DVDStatusResponse
{
    int playStatus;
    bool currentContentType;
    int pictureStatus;
    int randomModeStatus;
    int repeatModeStatus;
    int scanModeStatus;

    int groupOrTitleNum; // Group no. for DVD-A and Title num for DVD-V.
    int trackOrChapterNo; // Ongoing track(DVD-A)/chapter(DVD-V)
    int playbackTime; // elpased Time. in sec
    int multiAngle; // Multi angle playback on/off (DVD-V)
    int subTitleNum; // id of current subtitle set.
    int totalSubTitleNum; // Total subtitle present.
    int subTitleCode; // Sub title code of ongoing video.
    int audioSetting; // Curent audio setting
    int totalAudioSetting; // Total audio setting
    int audioCode; // Audio code for current audio setting
    int audioAttribute; // Represents audio attribute of the ongoing DVD-Audio/Video.
    int channel;
    int LFEExist; // added by cychoi 2013.07.20 for ITS 181110 Audio attribute
    int angleNo;
    int totalAngleNo;
    int displayState; // Represent current display state.
    int totalChaptorOrTrack; // Total number of chapters(DVD-V) or track(DVD-A)
    int totalGroupOrTitle; // Total number of Title(DVD-V)or Group(DVD-A)
};

struct CDDAStatusResponse
{
    int playStatus;
    bool ROMTrack;
    bool CDTEXTPresent;
    int randomModeStatus;
    int repeatModeStatus;
    int scanModeStatus;
    int trackNo;
    int playbackTime;// in secs.
    int startTrackNo;
};

struct VCDStatusResponse
{
    int playStatus;
    int version; // VCD 1.1 = 0x00, VCD 2.0 = 0x01, SVCD or Other = 0x02, UNDEFINED = 0x03
    bool ROMTrack;
    bool CDTEXTPresent;
    int randomModeStatus;
    int repeatModeStatus;
    int scanModeStatus;
    int trackNo;
    int startTrackNo; // Start track(VCD) added by cychoi 2013.07.07 for ISV 85460 DVD OSD mismatch
    int playbackTime;// in secs.
};

struct CAStatusResponse
{
    int playStatus;
    bool ROMTrack;
    bool CDTEXTPresent;
    int randomModeStatus;
    int repeatModeStatus;
    int scanModeStatus;
    int currentFolderNo;
    int currentFileNo;
    int playbackTime;// in secs.
    char fileName[32];
};

struct DVDCPictureSetting
{
    int contrast;
    int color;
    int tint;
    int brightness;
    int sharpness;
    int gammaCorrection;
    int coringFilter;
    int blackLevel;
    int whiteLevel;
};

struct DvdcCommonStatus
{
    int discPosition; // no disc, ejected, loading,loadinf completed etc.
    int wokingMechanics;
    int readError;
    int mechanicalError;
    int highTemperature;

    int mode; // ATAP or TAB2 mode.
    int powerMode; // normal/low power mode
    int regionErrorStatus;
    int parentalStatus;
    int drmStatus;
    int cddaStatus;
    int MediaType; // CDDA,VCD,Comp-Audio,CD-ROM,DVD-Video,DVD-Audio
    int cdPriority; // Priority in case of CDDA & Comp-Audio mixed disc.

    int displayStatus; // what is display(root menu, titlemenu etc).
    int deckVersion;

    int videoSystem; // PAL, NTSC
    int temperatureCode; //represents the temperature code of a deck unit ranging between 00h and FFh.
};

// This is for setting deck priority languages.
struct DeckPrioritySettings 
{
    int captionLanguage;
    int audioLangauge;
    int dvdAngle;
    int menuLanguage;
};

struct DVDSettings 
{
    int nSubtitleCode;
    int nAudioCode;
    int nAudioIndex;
    int nAngleNumber;
};



#endif
