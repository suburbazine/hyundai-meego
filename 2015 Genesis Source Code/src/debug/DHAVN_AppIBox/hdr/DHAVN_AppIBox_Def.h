#ifndef DHAVN_APPIBOX_DEF_H
#define DHAVN_APPIBOX_DEF_H

#define Y_OFFSET_FOR_FRONT_DISPLAY      0       // Front display y co-ordinate value
#define Y_OFFSET_FOR_REAR_DISPLAY       720     //  Rear display y co-ordinate value
#define SCREEN_WIDTH                    1280    // Display width in single monitor
#define SCREEN_HEIGHT                   720     // Display height in single monitor

//#define PRINT_CURRENT_TIME_IBOX

enum DB_SETTINGS_KEY_T
{
   DB_KEY_PHOTO_FRAME = 0,                             /**for general*/
   DB_KEY_PHOTO_FRAME_IMAGE,
   DB_KEY_SUMMER_TIME,
   DB_KEY_GPS_TIME_SETTINGS ,
   DB_KEY_DISPLAY_CLOCK_AT_AUDIO_END,
   DB_KEY_DISPLAY_CLOCK,
   DB_KEY_CLOCK_TYPE,
   DB_KEY_TIME_TYPE,
   DB_KEY_CALENDAR_TYPE,
   DB_KEY_LANGUAGE_TYPE,
   DB_KEY_TEMPERATURE_TYPE,
   DB_KEY_APPROVAL,
   DB_KEY_LOCKREARMONITOR_DISPLAY,                     /**for system*/
   DB_KEY_LOCKREARMONITOR_FUNCTION,
   DB_KEY_VIDEO_BRIGHTNESS,                            /**for screen*/
   DB_KEY_VIDEO_SATURATION,
   DB_KEY_VIDEO_HUE,
   DB_KEY_VIDEO_CONTRAST,
   DB_KEY_IMAGE_BRIGHTNESS,
   DB_KEY_EXPOSURE,
   DB_KEY_ASPECT_RADIO,
   DB_KEY_DVD_SUBTITLE_LANGUAGE,                       /**for DVD*/
   DB_KEY_DVD_AUDIO_LANGUAGE,
   DB_KEY_DVD_ANGLE,
   DB_KEY_SOUND_BALANCE,                               /**for Sound*/
   DB_KEY_SOUND_FADER,
   DB_KEY_SOUND_LOWTONE,
   DB_KEY_SOUND_MIDTONE,
   DB_KEY_SOUND_HIGHTONE,
   DB_KEY_SOUND_VOLUME_RATIO,
   DB_KEY_SOUND_POWERBASS,
   DB_KEY_SOUND_POWERTREBLE,
   DB_KEY_SOUND_SURROUND,
   DB_KEY_SOUND_ACTIVE,
   DB_KEY_SOUND_SPEED,
   DB_KEY_SOUND_BEEP,
   DB_KEY_SOUND_VEQ,
   DB_KEY_VOICE_VOICECOMMAND,
   DB_KEY_KEYPAD,
   DB_KEY_WINDOWS_INTERLOCKING,
   DB_KEY_DISTANCE_UNIT,
   DB_KEY_APPROACH_SENSOR,
   DB_KEY_QUANTUMLOGIC,
   DB_KEY_DATEFORMAT_TYPE,
   DB_KEY_DIVX_REG_STATE,
   DB_KEY_VIDEO_SCREENSETTINGS,
   DB_KEY_FRONT_SCREENBRIGHTNESS,
   DB_KEY_REAR_SCREENBRIGHTNESS,
   DB_KEY_FIRST_CAPITAL,
   DB_KEY_ENGLISH_KEYPAD,
   DB_KEY_KOREAN_KEYPAD,
   DB_KEY_ARABIC_KEYPAD,
   DB_KEY_CHINA_KEYPAD,
   DB_KEY_EUROPE_KEYPAD,
   DB_KEY_RUSSIAN_KEYPAD,
   DB_KEY_CURRENT_REGION,
   DB_KEY_AUX_VIDEOIN,
   DB_KEY_BT_VOLUME_LEVEL,
   DB_KEY_BT_RINGTONE,
   DB_KEY_BL_VOLUME_LEVEL,
   DB_KEY_BL_RINGTONE,
   DB_KEY_AUTO_TIMEZONE,
   DB_KEY_AUTO_TIMEZONE_SET,
   DB_KEY_SUMMER_TIME_SET,
   DB_KEY_VR_VOLUME_LEVEL,
   DB_KEY_VR_PHONE_PRIORITY,
   DB_KEY_REAR_ON,
   DB_KEY_HYUNDAY_KEYPAD,
   DB_SETTINGS_KEY_MAX
};

enum RESPONSE_FG_T
{
   RF_INVALID,
   RF_FRONT,
   RF_REAR,
};

enum IBOX_SCREEN_ID_T  {
    IBOX_NONE_SCREEN,
    IBOX_FRONT_SCREEN,
    IBOX_REAR_SCREEN,
};

/**
 * Different applications and modes running defined for the IBox.
 * The numbers for the enum values need to match the values defined
 * by the MOST Manager for the IBox.
 */
typedef enum
{
    EIBOX_MODE_GLOBAL_SEARCH = 0x01,            // 1
    EIBOX_MODE_HOME = 0x02,                     // 2
    EIBOX_MODE_GLOBAL_POPUP = 0x05,             // 5
    EIBOX_MODE_CLOCK = 0x06,                    // 6
    EIBOX_MODE_PHOTO_FRAME = 0x07,              // 7
    EIBOX_MODE_MAP = 0x08,                      // 8
    EIBOX_MODE_NAVI = 0x09,                     // 9
    EIBOX_MODE_CLIMATE = 0x0A,                  // 10
    EIBOX_MODE_CAMERA = 0x0B,                   // 11
    EIBOX_MODE_VUI = 0x0C,                      // 12
    EIBOX_MODE_FM = 0x11,                       // 17
    EIBOX_MODE_FM1 = 0x12,                      // 18
    EIBOX_MODE_FM2 = 0x13,                      // 19
    EIBOX_MODE_AM = 0x14,                       // 20
    EIBOX_MODE_LW = 0x15,                       // 21
    EIBOX_MODE_MW = 0x16,                       // 22
    EIBOX_MODE_BSMING = 0x1A,                   // 26
    EIBOX_MODE_DMB_TV = 0x21,                   // 33
    EIBOX_MODE_DMB_RADIO = 0x22,                // 34
    EIBOX_MODE_DMB1_TV = 0x23,                  // 35
    EIBOX_MODE_DMB1_RADIO = 0x24,               // 36
    EIBOX_MODE_DMB2_TV = 0x25,                  // 37
    EIBOX_MODE_DMB2_RADIO = 0x26,               // 38
    EIBOX_MODE_DMB_SEARCHING = 0x2A,            // 42
    EIBOX_MODE_XM = 0x31,                       // 49
    EIBOX_MODE_XM1 = 0x32,                      // 50
    EIBOX_MODE_XM2 = 0x33,                      // 51
    EIBOX_MODE_XM3 = 0x34,                      // 52
    EIBOX_MODE_CD_DA = 0x41,                    // 65
    EIBOX_MODE_VCD = 0x42,                      // 66
    EIBOX_MODE_CD_AUDIO = 0x43,                 // 67
    EIBOX_MODE_CD_VIDEO = 0x44,                 // 68
    EIBOX_MODE_CD_IMAGE = 0x45,                 // 69
    EIBOX_MODE_DISC_READING = 0x4A,             // 74
    EIBOX_MODE_DISC_ERROR = 0x4B,               // 75
    EIBOX_MODE_DVD_A = 0x51,                    // 81
    EIBOX_MODE_DVD_V = 0x52,                    // 82
    EIBOX_MODE_DVD_AUDIO = 0x53,                // 83
    EIBOX_MODE_DVD_VIDEO = 0x54,                // 84
    EIBOX_MODE_DVD_IMAGE = 0x55,                // 85
    EIBOX_MODE_IPOD = 0x61,                     // 97
    EIBOX_MODE_USB = 0x62,                      // 98
    EIBOX_MODE_AUX = 0x63,                      // 99
    EIBOX_MODE_USB_AUDIO = 0x64,                // 100
    EIBOX_MODE_USB_VIDEO = 0x65,                // 101
    EIBOX_MODE_USB_IMAGE = 0x66,                // 102
    EIBOX_MODE_AUX_AUDIO = 0x67,                // 103
    EIBOX_MODE_AUX_VIDEO = 0x68,                // 104
    EIBOX_MODE_IPOD_READING = 0x6A,             // 106
    EIBOX_MODE_IPOD_NO_FILE = 0x6B,             // 107
    EIBOX_MODE_USB_READING = 0x6C,              // 108
    EIBOX_MODE_USB_NO_FILE = 0x6D,              // 109
    EIBOX_MODE_AUX_NO_SIGNAL = 0x6E,            // 110
    EIBOX_MODE_JUKE_BOX_AUDIO = 0x71,           // 113
    EIBOX_MODE_JUKE_BOX_VIDEO = 0x72,           // 114
    EIBOX_MODE_JUKE_BOX_IMAGE = 0x73,           // 115
    EIBOX_MODE_VIRTUAL_CDC_CD_DA = 0x74,        // 116
    EIBOX_MODE_VIRTUAL_CDC_AUDIO = 0x75,        // 117
    EIBOX_MODE_MORELIKETHIS = 0x76,             // 118
    EIBOX_MODE_BT_PHONE_MAIN = 0x80,            // 128
    EIBOX_MODE_BT_DIAL = 0x81,                  // 129
    EIBOX_MODE_BT_PHONEBOOK = 0x82,             // 130
    EIBOX_MODE_BT_CALL_HISTORY = 0x83,          // 131
    EIBOX_MODE_BT_FAVORITE = 0x84,              // 132
    EIBOX_MODE_BLUETOOTH_STREAMING = 0x86,      // 134 (BT)
    EIBOX_MODE_MODEM_PHONE_MAIN = 0x90,         // 144
    EIBOX_MODE_MODEM_DIAL = 0x91,               // 145
    EIBOX_MODE_MODEM_PHONEBOOK = 0x92,          // 146
    EIBOX_MODE_MODEM_CALL_HISTORY = 0x93,       // 147
    EIBOX_MODE_MODEM_FAVORITE = 0x94,           // 148
    EIBOX_MODE_PROVISIONING = 0xA0,             // 160
    EIBOX_MODE_EMERGENCY_CALL = 0xA1,           // 161 (SOS)
    EIBOX_MODE_ACN = 0xA2,                      // 162
    EIBOX_MODE_SLOWDOWN = 0xA3,                 // 163
    EIBOX_MODE_IMMOBILIZATION = 0xA4,           // 164
    EIBOX_MODE_CENTER_CALL = 0xA5,              // 165
    EIBOX_MODE_BLUELINK_ASSISTENT = 0xA6,       // 166
    EIBOX_MODE_BLUELINK = 0xA8,                 // 168
    EIBOX_MODE_DTC_ALERT = 0xA9,                // 169
    EIBOX_MODE_DRIVINGRESTRICTION_BASIC = 0xAA, // 170
    EIBOX_MODE_DRIVINGRESTRICTION_DLNA = 0xAB,  // 171
    EIBOX_MODE_EMAIL = 0xB1,                    // 177
    EIBOX_MODE_INTERNET = 0xB2,                 // 178
    EIBOX_MODE_DLNA = 0xB4,                     // 180
    EIBOX_MODE_DLNA_LIST = 0xB5,                // 181
    EIBOX_MODE_DOWNLOAD_CENTER = 0xB8,          // 184
    EIBOX_MODE_MEDIA_PLAYER = 0xB9,             // 185
    EIBOX_MODE_MUSIC_APP = 0xBA,                // 186
    EIBOX_MODE_ROSETTA = 0xBB,                  // 187
    EIBOX_MODE_DRIVING_INFO = 0xBC,             // 188
    EIBOX_MODE_DAILY_ROUTE_GUIDANCE = 0xBD,     // 189
    EIBOX_MODE_SEND_TO_CAR = 0xBE,              // 190 (POI)
    EIBOX_MODE_VOICE_LOCAL_SEARCH = 0xBF,       // 191
    EIBOX_MODE_APPLICATIONS = 0xC1,             // 193
    EIBOX_MODE_SHARED_LOCATION = 0xC2,          // 194
    EIBOX_MODE_SEND_MY_LOCATION = 0xC3,         // 195
    EIBOX_MODE_FIND_DEALER = 0xC6,              // 198
    EIBOX_MODE_VEHICLE_DIAGNOSTICS = 0xC7,      // 199
    EIBOX_MODE_MAINTENANCE_ALERT = 0xC8,        // 200
    EIBOX_MODE_SCHEDULED_DIAGNOSTICS = 0xC9,    // 201
    EIBOX_MODE_ROADSIDE_ASSIST = 0xCA,          // 202
    EIBOX_MODE_SCHEDULE_SERVICE = 0xCB,         // 203
    EIBOX_MODE_DOWNLOADED_APPS = 0xCF,          // 207
    EIBOX_MODE_SOUNDSETTING = 0xD2,             // 210
    EIBOX_MODE_WIFI_SETTING = 0xD4,             // 212
    EIBOX_MODE_INTERNET_SETTING = 0xD5,         // 213
    EIBOX_MODE_APP_SETTING = 0xD6,              // 214
    EIBOX_MODE_BLUELINK_SETTING = 0xD7,         // 215
    EIBOX_MODE_IBOX_ENGINEERING_MODE = 0xDF,    // 223
    EIBOX_MODE_POWER_OFF = 0xEF,                // 239

    EIBOX_MODE_IBOX_DRS = 0xFE,                 // 254
    EIBOX_MODE_INVALID = 0xFF                   // 255
} IBOX_MODE_ID_T;

#endif // DHAVN_APPIBOX_DEF_H
