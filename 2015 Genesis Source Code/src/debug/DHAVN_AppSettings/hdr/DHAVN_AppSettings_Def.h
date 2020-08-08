#ifndef DHAVN_APPSETTINGS_DEF_H
#define DHAVN_APPSETTINGS_DEF_H


#define SETTINGS_FILE_DB "/app/data/app_settings/SettingsScreenSettings.db"
#define SETTINGS_SYSTEM_SETTINGS_INFO_FILE_PATH "/app/data/app_settings/SystemSettingsInfo.ini"
#define SETTINGS_HM_DB "settings_screen_app"
#define SETTINGS_BT_RINGTONE_PATH "/usr_data/ringtones/"

#include <QObject>
#include <uistate_manager_def.h>
#include <DHAVN_AppSettings_Shared.h>

struct DB_SETTINGS_KEYS_T;

class SettingsDef : public QObject
{
    Q_OBJECT

    Q_ENUMS( DB_SETTINGS_KEY_T
             SETTINGS_LANGUAGE_T
             SETTINGS_EVT_KEY_STATUS_T
             SETTINGS_DISPLAY_T
             SETTINGS_EVT_KEY_T
             SETTINGS_DATEFORMAT_T
             SETTINGS_DIVX_T
             SETTINGS_ENGLISH_KEYPAD_T
             SETTINGS_KOREAN_KEYPAD_T
             SETTINGS_ARABIC_KEYPAD_T
             SETTINGS_CHINA_KEYPAD_T
             SETTINGS_EUROPE_KEYPAD_T
             SETTINGS_RUSSIAN_KEYPAD_T
             SETTINGS_POPUP_TYPE
             SETTINGS_HYUNDAI_KEYPAD_T
             SETTINGS_TIME_TYPE
             SETTINGS_VIDEO_DEFAULT_SET_SEND_T
             SETTINGS_SEND_MOST_DISPLAY_T
             SETTINGS_DISTANCE_UNIT_T
             SETTINGS_TOAST_POPUP_TYPE_T
             )

public:


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
        DB_KEY_TEMPERATURE_TYPE,   //10
        DB_KEY_APPROVAL,
        DB_KEY_LOCKREARMONITOR_DISPLAY,                     /**for system*/
        DB_KEY_LOCKREARMONITOR_FUNCTION,
        DB_KEY_VIDEO_BRIGHTNESS,                            /**for screen*/
        DB_KEY_VIDEO_SATURATION,
        DB_KEY_VIDEO_HUE,
        DB_KEY_VIDEO_CONTRAST,
        DB_KEY_IMAGE_BRIGHTNESS,
        DB_KEY_EXPOSURE,
        DB_KEY_ASPECT_RADIO,  //20
        DB_KEY_DVD_SUBTITLE_LANGUAGE,                       /**for DVD*/
        DB_KEY_DVD_AUDIO_LANGUAGE,
        DB_KEY_DVD_ANGLE,
        DB_KEY_SOUND_BALANCE,                               /**for Sound*/
        DB_KEY_SOUND_FADER,
        DB_KEY_SOUND_LOWTONE,
        DB_KEY_SOUND_MIDTONE,
        DB_KEY_SOUND_HIGHTONE,
        DB_KEY_SOUND_VOLUME_RATIO,
        DB_KEY_SOUND_POWERBASS, //30
        DB_KEY_SOUND_POWERTREBLE,
        DB_KEY_SOUND_SURROUND,
        DB_KEY_SOUND_ACTIVE,
        DB_KEY_SOUND_SPEED,
        DB_KEY_SOUND_BEEP,
        DB_KEY_SOUND_VEQ,
        DB_KEY_VOICE_VOICECOMMAND,
        DB_KEY_KEYPAD,
        DB_KEY_WINDOWS_INTERLOCKING,
        DB_KEY_DISTANCE_UNIT,  //40
        DB_KEY_APPROACH_SENSOR,
        DB_KEY_QUANTUMLOGIC,
        DB_KEY_DATEFORMAT_TYPE,
        DB_KEY_DIVX_REG_STATE,
        DB_KEY_VIDEO_SCREENSETTINGS,
        DB_KEY_FRONT_SCREENBRIGHTNESS,
        DB_KEY_REAR_SCREENBRIGHTNESS,
        DB_KEY_FIRST_CAPITAL,
        DB_KEY_ENGLISH_KEYPAD,
        DB_KEY_KOREAN_KEYPAD,   //50
        DB_KEY_ARABIC_KEYPAD,
        DB_KEY_CHINA_KEYPAD,
        DB_KEY_EUROPE_KEYPAD,
        DB_KEY_RUSSIAN_KEYPAD,
        DB_KEY_CURRENT_REGION,
        DB_KEY_AUX_VIDEOIN,
        DB_KEY_BT_VOLUME_LEVEL,
        DB_KEY_BT_RINGTONE,
        DB_KEY_BL_VOLUME_LEVEL,
        DB_KEY_BL_RINGTONE,   //60
        DB_KEY_AUTO_TIMEZONE,
        DB_KEY_AUTO_TIMEZONE_SET,
        DB_KEY_SUMMER_TIME_SET,
        DB_KEY_VR_VOLUME_LEVEL,
        DB_KEY_VR_PHONE_PRIORITY,
        DB_KEY_REAR_ON,
        DB_KEY_HYUNDAY_KEYPAD,
        DB_KEY_SCROLL_TICKER,
        DB_KEY_LREAR,
        DB_KEY_RREAR,   //70
        DB_KEY_VOICE_APPLE_SIRI,
        DB_KEY_VIP_SOUND,
        DB_KEY_RESET_ALL_COMPLETE,
        DB_KEY_MAX
    };

    enum SETTINGS_LANGUAGE_T
    {
        SETTINGS_LANGUAGE_UNKNOWN = LANGUAGE_UNKNOWN,
        SETTINGS_LANGUAGE_EN      = LANGUAGE_EN,     //< English
        SETTINGS_LANGUAGE_KO      = LANGUAGE_KO,     //< Korean
        SETTINGS_LANGUAGE_ZH_MA   = LANGUAGE_ZH_CN,   //< Chinese Mandarin
        SETTINGS_LANGUAGE_ZH_CN, //= LANGUAGE_ZH_YUE, //< Chinese mainland
        SETTINGS_LANGUAGE_PT, //=  LANGUAGE_PT,    //< Europeen Portuguese
        SETTINGS_LANGUAGE_EN_UK, //=  LANGUAGE_EN_UK, //< English (UK)
        SETTINGS_LANGUAGE_EN_US, //=  LANGUAGE_EN_US, //< English (US)
        SETTINGS_LANGUAGE_FR,   //=  LANGUAGE_FR,    //< European French
        SETTINGS_LANGUAGE_IT, //=  LANGUAGE_IT,    //< Italian
        SETTINGS_LANGUAGE_DE, //=  LANGUAGE_DE,    //< German
        SETTINGS_LANGUAGE_ES, //=  LANGUAGE_ES,    //< European Spanish
        SETTINGS_LANGUAGE_RU, //=  LANGUAGE_RU,    //< Russian
        SETTINGS_LANGUAGE_NL, //=  LANGUAGE_NL,    //< Dutch
        SETTINGS_LANGUAGE_SV, //=  LANGUAGE_SV,    //< Swedish
        SETTINGS_LANGUAGE_PL, //=  LANGUAGE_PL,    //< Polish
        SETTINGS_LANGUAGE_TR, //=  LANGUAGE_TR,    //< Turkish
        SETTINGS_LANGUAGE_CS, //=  LANGUAGE_CS,    //< Czech
        SETTINGS_LANGUAGE_DA, //=  LANGUAGE_DA,    //< Danish
        SETTINGS_LANGUAGE_SK, //=  LANGUAGE_SK,    //< Slovakia
        SETTINGS_LANGUAGE_AR, //=  LANGUAGE_AR    //< Arabic
        SETTINGS_LANGUAGE_FR_NA, //=  LANGUAGE_FR_NA    //< French (US)
        SETTINGS_LANGUAGE_ES_NA //=  LANGUAGE_ES_NA    //< Spanish (US)

    };

    enum SETTINGS_EVT_KEY_STATUS_T
    {
        SETTINGS_EVT_KEY_STATUS_PRESSED = KEY_STATUS_PRESSED,
        SETTINGS_EVT_KEY_STATUS_RELEASED = KEY_STATUS_RELEASED
    };

    enum SETTINGS_DISTANCE_UNIT_T
    {
        SETTINGS_DISTANCE_KM = 0,
        SETTINGS_DISTANCE_MILE
    };

    enum SETTINGS_RESET_TYPE_T
    {
        SETTINGS_RESET_TYPE_SOUND,
        SETTINGS_RESET_TYPE_SCREEN,
        SETTINGS_RESET_TYPE_GENERAL
    };

    enum SETTINGS_DISPLAY_T
    {
        SETTINGS_DISPLAY_FRONT,
        SETTINGS_DISPLAY_REAR,
        SETTINGS_DISPLAY_NONE
    };

    enum SETTINGS_POPUP_TYPE
    {
        SETTINGS_RESET_SOUND_SETTINGS,
        SETTINGS_RESET_SCREEN_SETTINGS,
        SETTINGS_RESET_GENERAL_SETTINGS,
        SETTINGS_RESET_NAVI_SETTINGS,
        SETTINGS_RESET_BT_SETTINGS,
        SETTINGS_CONFIRM_FORMAT_POPUP,
        SETTINGS_DVD_POPUP_TYPE,
        SETTINGS_TIME_PICKER_POPUP,
        SETTINGS_GPSTIME_POPUP,
        SETTINGS_VR_NOT_SUPPORT_POPUP,
        SETTINGS_JUKEBOX_IMAGE_DISABLE,
        SETTINGS_DRS_MESSAGE_POPUP

    };

    enum SETTINGS_TOAST_POPUP_TYPE_T
    {
        SETTINGS_TOAST_RESET_START = 0,
        SETTINGS_TOAST_RESET_COMPLETE,
        SETTINGS_TOAST_FORMAT_START,
        SETTINGS_TOAST_FORMAT_COMPLETE,
        SETTINGS_TOAST_LANGUAGE_CHANGING,
        SETTINGS_TOAST_TIME_SETTING_COMPLETE,
        SETTINGS_TOAST_DAYLIGHT_SAVINGS_TIME_CHANGING //added for ITS 217706 daylight savings time not sync
    };

    enum SETTINGS_EVT_KEY_T
    {
        SETTINGS_EVT_KEY_BACK
    };

    enum SETTINGS_DATEFORMAT_T
    {
        SETTINGS_DATEFORMAT_YYYYMMDD_DASH = 1,    //YYYY-MM-DD
        SETTINGS_DATEFORMAT_MMDDYYYY_DASH,        //MM-DD-YYYY
        SETTINGS_DATEFORMAT_DDMMYYYY_DASH,        //DD-MM-YYYY
        SETTINGS_DATEFORMAT_YYYYMMDD_SLASH,       //YYYY/MM/DD
        SETTINGS_DATEFORMAT_MMDDYYYY_SLASH,       //MM/DD/YYYY
        SETTINGS_DATEFORMAT_DDMMYYYY_SLASH,       //DD/MM/YYYY
        SETTINGS_DATEFORMAT_YYYYMMDD_DOT,         //YYYY.MM.DD
        SETTINGS_DATEFORMAT_MMDDYYYY_DOT,         //MM.DD.YYYY
        SETTINGS_DATEFORMAT_DDMMYYYY_DOT          //DD.MM.YYYY
    };

    enum SETTINGS_DIVX_T
    {
        SETTINGS_DIVX_REG = 0,
        SETTINGS_DIVX_DEREG = 1

    };
    enum SETTINGS_ENGLISH_KEYPAD_T
    {
        SETTINGS_ENGLISH_KEYPAD_QWERTY = 0,
        SETTINGS_ENGLISH_KEYPAD_ABCD
    };

    enum SETTINGS_KOREAN_KEYPAD_T
    {
        SETTINGS_KOREAN_KEYPAD_QWERTY = 0,
        SETTINGS_KOREAN_KEYPAD_LETTER
    };

    enum SETTINGS_ARABIC_KEYPAD_T
    {
        SETTINGS_ARABIC_QWERTY = 0,
        SETTINGS_ARABIC_TYPE2
    };

    enum SETTINGS_CHINA_KEYPAD_T
    {
        SETTINGS_CHINA_PINYIN = 0,
        SETTINGS_CHINA_VOCAL_SOUND,
        SETTINGS_CHINA_HAND_WRITING
    };

    enum SETTINGS_EUROPE_KEYPAD_T
    {
        SETTINGS_EUROPE_ENGLISH_QWERTY = 0,
        SETTINGS_EUROPE_ENGLISH_ABC,
        SETTINGS_EUROPE_QWERTZ,
        SETTINGS_EUROPE_AZERTY
    };

    enum SETTINGS_RUSSIAN_KEYPAD_T
    {
        SETTINGS_RUSSIAN_CYRILLIC_QWERTY = 0,
        SETTINGS_RUSSIAN_CYRILLIC_ABC
    };

    enum SETTINGS_HYUNDAI_KEYPAD_T
    {
        SETTINGS_HYUNDAI_KOREAN = 0,
        SETTINGS_HYUNDAI_ENGLISH_LATIN,
        SETTINGS_HYUNDAI_ARABIC,
        SETTINGS_HYUNDAI_CHINA,
        SETTINGS_HYUNDAI_EUROPE
    };

    enum SETTINGS_TIME_TYPE
    {
        SETTINGS_TIME_TYPE_AM = 0,
        SETTINGS_TIME_TYPE_PM
    };

    enum SETTINGS_VIDEO_DEFAULT_SET_ACK_T
    {
        SETTINGS_VDS_PROCESSING = 1,
        SETTINGS_VDS_RESULT,
    };

    enum SETTINGS_VIDEO_DEFAULT_SET_SEND_T
    {
        SETTINGS_VDS_S_ALL = 0,
        SETTINGS_VDS_S_ADJUST_SCREEN,
        SETTINGS_VDS_S_LCD_BRIGHTNESS,
        SETTINGS_VDS_S_ILLUMINATION,
        SETTINGS_VDS_S_VIDEO_RATIO,
        SETTINGS_VDS_S_DISPLAY_ONOFF,
        SETTINGS_VDS_S_INVALID = 0xFF,
    };

    enum SETTINGS_SEND_MOST_DISPLAY_T
    {
        SETTINGS_SMD_FRONT = 0,
        SETTINGS_SMD_LREAR,
        SETTINGS_SMD_RREAR,
        SETTINGS_SMD_ALL,
    };

    enum SETTINGS_DISPLAY_MASK_T
    {
        SETTINGS_DMASK_FRONT = 1,
        SETTINGS_DMASK_LREAR = 2,
        SETTINGS_DMASK_RREAR = 4,
        SETTINGS_DMASK_ALL = 7,
    };

    enum SETTINGS_EAUDIOSETUP_T
    {
        SETTINGS_SOUND_BASS = 0,
        SETTINGS_SOUND_MIDDLE,
        SETTINGS_SOUND_TREBLE,
        SETTINGS_SOUND_BALANCE,
        SETTINGS_SOUND_FADER,
        SETTINGS_SOUND_VARIABLE_EQ,
        SETTINGS_SOUND_SDVC,
        SETTINGS_SOUND_SURROUND,
        SETTINGS_SOUND_BEEP,
        SETTINGS_SOUND_VOLUMERATIO,
        //SETTINGS_SOUND_QUANTUMLOGIC, // 사양삭제. VIP SOUND로 변경 사용
        SETTINGS_SOUND_VIPSOUND, // = 10
        SETTINGS_SOUND_POWERBASS,
        SETTINGS_SOUND_WELCOME,
        SETTINGS_SOUND_BASS_MID_TREBLE_CENTER,
        SETTINGS_SOUND_BALANCE_FADER_CENTER,
        SETTINGS_SOUND_ALL_INIT
    };
};

struct DB_SETTINGS_KEYS_T
{
    SettingsDef::DB_SETTINGS_KEY_T keyId;
    const char *keyString;
};

// Table of settings variables
static const DB_SETTINGS_KEYS_T SETTINGS_DB_VARIABLES_KEYS[] =
{
    { SettingsDef::DB_KEY_PHOTO_FRAME,                  "mPhotoFrameKey"             },
    { SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE,            "mPhotoFrameImageKey"        },
    { SettingsDef::DB_KEY_SUMMER_TIME,                  "mSummerTimeKey"             },
    { SettingsDef::DB_KEY_GPS_TIME_SETTINGS,            "mGPSTimeSettingKey"         },
    { SettingsDef::DB_KEY_DISPLAY_CLOCK_AT_AUDIO_END,   "mDisplayClockAtAudioEndKey" },
    { SettingsDef::DB_KEY_DISPLAY_CLOCK,                "mDisplayClockKey"           },
    { SettingsDef::DB_KEY_CLOCK_TYPE,                   "mClockTypeKey"              },
    { SettingsDef::DB_KEY_TIME_TYPE,                    "mTimeTypeKey"               },
    { SettingsDef::DB_KEY_CALENDAR_TYPE,                "mCalendarTypeKey"           },
    { SettingsDef::DB_KEY_LANGUAGE_TYPE,                "mLanguageKey"               },
    { SettingsDef::DB_KEY_TEMPERATURE_TYPE,             "mTemperatureKey"            },
    { SettingsDef::DB_KEY_APPROVAL,                     "mApprovalKey"               },
    { SettingsDef::DB_KEY_LOCKREARMONITOR_DISPLAY,      "mDisplay"                   },
    { SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION,     "mFunction"                  },
    { SettingsDef::DB_KEY_VIDEO_BRIGHTNESS,             "mVideoBrightness"           },
    { SettingsDef::DB_KEY_VIDEO_SATURATION,             "mVideoSaturation"           },
    { SettingsDef::DB_KEY_VIDEO_HUE,                    "mVideoHue"                  },
    { SettingsDef::DB_KEY_VIDEO_CONTRAST,               "mVideoContrast"             },
    { SettingsDef::DB_KEY_IMAGE_BRIGHTNESS,             "mImegeBrightness"           },
    { SettingsDef::DB_KEY_EXPOSURE,                     "mExposure"                  },
    { SettingsDef::DB_KEY_ASPECT_RADIO,                 "mAspectRadio"               },
    { SettingsDef::DB_KEY_DVD_SUBTITLE_LANGUAGE,        "mDvdSubtitleLanguage"       },
    { SettingsDef::DB_KEY_DVD_AUDIO_LANGUAGE,           "mDvdAudioLanguage"          },
    { SettingsDef::DB_KEY_DVD_ANGLE,                    "mDvdAngle"                  },
    { SettingsDef::DB_KEY_SOUND_BALANCE,                "mSoundBalance"              },
    { SettingsDef::DB_KEY_SOUND_FADER,                  "mSoundFader"                },
    { SettingsDef::DB_KEY_SOUND_LOWTONE,                "mSoundLowTone"              },
    { SettingsDef::DB_KEY_SOUND_MIDTONE,                "mSoundMidTone"              },
    { SettingsDef::DB_KEY_SOUND_HIGHTONE,               "mSoundHighTone"             },
    { SettingsDef::DB_KEY_SOUND_VOLUME_RATIO,           "mSoundVolumeRatio"          },
    { SettingsDef::DB_KEY_SOUND_POWERBASS,              "mSoundPowerBass"            },
    { SettingsDef::DB_KEY_SOUND_POWERTREBLE,            "mSoundPowerTreble"          },
    { SettingsDef::DB_KEY_SOUND_SURROUND,               "mSoundSurround"             },
    { SettingsDef::DB_KEY_SOUND_ACTIVE,                 "mSoundActive"               },
    { SettingsDef::DB_KEY_SOUND_SPEED,                  "mSoundSpeed"                },
    { SettingsDef::DB_KEY_SOUND_BEEP,                   "mSoundBeep"                 },
    { SettingsDef::DB_KEY_SOUND_VEQ,                    "mVEQ"                       },
    { SettingsDef::DB_KEY_VOICE_VOICECOMMAND,           "mVoiceCommand"              },
    { SettingsDef::DB_KEY_KEYPAD,                       "mKeyPad"                    },
    { SettingsDef::DB_KEY_WINDOWS_INTERLOCKING,         "mWindowsInterlocking"       },
    { SettingsDef::DB_KEY_DISTANCE_UNIT,                "mDistanceUnit"              },
    { SettingsDef::DB_KEY_APPROACH_SENSOR,              "mApproachSensor"            },
    { SettingsDef::DB_KEY_QUANTUMLOGIC,              "mApproachSensor"            },
    { SettingsDef::DB_KEY_DATEFORMAT_TYPE,              "mDateFormat"                },
    { SettingsDef::DB_KEY_DIVX_REG_STATE,               "mDivxRegState"             },
    { SettingsDef::DB_KEY_VIDEO_SCREENSETTINGS,          "mScreenSettings"            },
    { SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS,        "mFrontScreenSettings"       },
    { SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS,         "mRearScreenSettings"        },
    { SettingsDef::DB_KEY_FIRST_CAPITAL,                 "mFirstCapital"              },
    { SettingsDef::DB_KEY_ENGLISH_KEYPAD,                "mEnglishKeypad"             },
    { SettingsDef::DB_KEY_KOREAN_KEYPAD,                 "mKoreanKeypad"              },
    { SettingsDef::DB_KEY_ARABIC_KEYPAD,                 "mArabicKeypad"              },
    { SettingsDef::DB_KEY_CHINA_KEYPAD,                  "mChinaKeypad"               },
    { SettingsDef::DB_KEY_EUROPE_KEYPAD,                 "mEuropeKeypad"              },
    { SettingsDef::DB_KEY_RUSSIAN_KEYPAD,                  "mRussianKeypad"               },
    { SettingsDef::DB_KEY_CURRENT_REGION,                "mCurrentRegion"             },
    { SettingsDef::DB_KEY_AUX_VIDEOIN,                   "mAuxVideoIn"                },
    { SettingsDef::DB_KEY_BT_VOLUME_LEVEL,               "mBtVolume"                  },
    { SettingsDef::DB_KEY_BT_RINGTONE,                   "mBtRingtone"                },
    { SettingsDef::DB_KEY_BL_VOLUME_LEVEL,               "mBlVolume"                  },
    { SettingsDef::DB_KEY_BL_RINGTONE,                   "mBlRingtone"                },
    { SettingsDef::DB_KEY_AUTO_TIMEZONE,                 "mAutoTimeZone"              },
    { SettingsDef::DB_KEY_AUTO_TIMEZONE_SET,             "mAutoTimeZoneSet"           },
    { SettingsDef::DB_KEY_SUMMER_TIME_SET,               "mSummerTimeSet"             },
    { SettingsDef::DB_KEY_VR_VOLUME_LEVEL,               "mVrVolume"                  },
    { SettingsDef::DB_KEY_VR_PHONE_PRIORITY,             "mVrPhonePriority"           },
    { SettingsDef::DB_KEY_REAR_ON,                          "mRearScreenOn"           },
    { SettingsDef::DB_KEY_HYUNDAY_KEYPAD,                "mHyndaiDefaultKeypad"       },
    { SettingsDef::DB_KEY_SCROLL_TICKER,                 "mGeneralScrollingTicker"       },
    { SettingsDef::DB_KEY_LREAR,                         "mLeftRear"       },
    { SettingsDef::DB_KEY_RREAR,                         "mRightRear"       },
    { SettingsDef::DB_KEY_VOICE_APPLE_SIRI,              "mAppleSiri"       },
    { SettingsDef::DB_KEY_VIP_SOUND,                     "mVipSound"       },
    { SettingsDef::DB_KEY_RESET_ALL_COMPLETE,            "mResetAllComplete" }
};

static const int SETTINGS_VARIABLES_DEFAULT_INT[][2]=
{
    { SettingsDef::DB_KEY_PHOTO_FRAME,                AppSettingsDef::SCREEN_SAVER_MODE_OFF },
    { SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE,          0                                 },
    { SettingsDef::DB_KEY_SUMMER_TIME,                0                                 },
    { SettingsDef::DB_KEY_GPS_TIME_SETTINGS,          1                                 },
    { SettingsDef::DB_KEY_DISPLAY_CLOCK_AT_AUDIO_END, 1                                 },
    { SettingsDef::DB_KEY_DISPLAY_CLOCK,              1                                 },
    { SettingsDef::DB_KEY_CLOCK_TYPE,                 1                                 },
    { SettingsDef::DB_KEY_TIME_TYPE,                  0                                 },
    { SettingsDef::DB_KEY_CALENDAR_TYPE,              0                                 },
    { SettingsDef::DB_KEY_LANGUAGE_TYPE,              SettingsDef::SETTINGS_LANGUAGE_UNKNOWN },
    { SettingsDef::DB_KEY_TEMPERATURE_TYPE,           0                                 },
    { SettingsDef::DB_KEY_APPROVAL,                   1                                 },
    { SettingsDef::DB_KEY_LOCKREARMONITOR_DISPLAY,    1                                 },
    { SettingsDef::DB_KEY_LOCKREARMONITOR_FUNCTION,   0                                 },
    { SettingsDef::DB_KEY_VIDEO_BRIGHTNESS,           0                                 },
    { SettingsDef::DB_KEY_VIDEO_SATURATION,           0                                 },
    { SettingsDef::DB_KEY_VIDEO_HUE,                  0                                 },
    { SettingsDef::DB_KEY_VIDEO_CONTRAST,             0                                 },
    { SettingsDef::DB_KEY_IMAGE_BRIGHTNESS,           5                                 },
    { SettingsDef::DB_KEY_EXPOSURE,                   0                                 },
    { SettingsDef::DB_KEY_ASPECT_RADIO,               1                                 },   //1 - 16:9, 2 - 4:3
    { SettingsDef::DB_KEY_DVD_SUBTITLE_LANGUAGE,      0                                 },
    { SettingsDef::DB_KEY_DVD_AUDIO_LANGUAGE,         0                                 },
    { SettingsDef::DB_KEY_DVD_ANGLE,                  0                                 },
    { SettingsDef::DB_KEY_SOUND_BALANCE,              10                                 },
    { SettingsDef::DB_KEY_SOUND_FADER,                10                                 },
    { SettingsDef::DB_KEY_SOUND_LOWTONE,              0                                 },
    { SettingsDef::DB_KEY_SOUND_MIDTONE,              0                                 },
    { SettingsDef::DB_KEY_SOUND_HIGHTONE,             0                                 },
    { SettingsDef::DB_KEY_SOUND_VOLUME_RATIO,         0                                 },
    { SettingsDef::DB_KEY_SOUND_POWERBASS,            1                                 },
    { SettingsDef::DB_KEY_SOUND_POWERTREBLE,          1                                 },
    { SettingsDef::DB_KEY_SOUND_SURROUND,             0                                 },
    { SettingsDef::DB_KEY_SOUND_ACTIVE,               0                                 },
    { SettingsDef::DB_KEY_SOUND_SPEED,                1                                 },
    { SettingsDef::DB_KEY_SOUND_BEEP,                 1                                 },
    { SettingsDef::DB_KEY_SOUND_VEQ,                  0                                 },
    { SettingsDef::DB_KEY_VOICE_VOICECOMMAND,         0              },
    { SettingsDef::DB_KEY_KEYPAD,                     0              },
    { SettingsDef::DB_KEY_WINDOWS_INTERLOCKING,       0              },
    { SettingsDef::DB_KEY_DISTANCE_UNIT,              1              },
    { SettingsDef::DB_KEY_APPROACH_SENSOR,            0              },
    { SettingsDef::DB_KEY_QUANTUMLOGIC,               0              },
    { SettingsDef::DB_KEY_DATEFORMAT_TYPE,            AppSettingsDef::SETTINGS_DATEFORMAT_YYYYMMDD_DASH},
    { SettingsDef::DB_KEY_DIVX_REG_STATE,             SettingsDef::SETTINGS_DIVX_DEREG  },
    { SettingsDef::DB_KEY_VIDEO_SCREENSETTINGS,       0              },
    { SettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS,     10              },
    { SettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS,      10              },
    { SettingsDef::DB_KEY_FIRST_CAPITAL,              1              },
    { SettingsDef::DB_KEY_ENGLISH_KEYPAD,             0              },
    { SettingsDef::DB_KEY_KOREAN_KEYPAD,              0              },
    { SettingsDef::DB_KEY_ARABIC_KEYPAD,              0              },
    { SettingsDef::DB_KEY_CHINA_KEYPAD,               0              },
    { SettingsDef::DB_KEY_EUROPE_KEYPAD,              0              },
    { SettingsDef::DB_KEY_RUSSIAN_KEYPAD,              0               },
    { SettingsDef::DB_KEY_CURRENT_REGION,            -1              },
    { SettingsDef::DB_KEY_AUX_VIDEOIN,                0              },
    { SettingsDef::DB_KEY_BT_VOLUME_LEVEL,           25              },
    { SettingsDef::DB_KEY_BT_RINGTONE,               0               },
    { SettingsDef::DB_KEY_BL_VOLUME_LEVEL,           25              },
    { SettingsDef::DB_KEY_BL_RINGTONE,               0               },
    { SettingsDef::DB_KEY_AUTO_TIMEZONE,             0               },
    { SettingsDef::DB_KEY_AUTO_TIMEZONE_SET,         0                },
    { SettingsDef::DB_KEY_SUMMER_TIME_SET,           0               },
    { SettingsDef::DB_KEY_VR_VOLUME_LEVEL,           25              },
    { SettingsDef::DB_KEY_VR_PHONE_PRIORITY,         AppSettingsDef::SETTINGS_PHONE_PRIORITY_BLUETOOTH },
    { SettingsDef::DB_KEY_REAR_ON ,                   1 },
    { SettingsDef::DB_KEY_HYUNDAY_KEYPAD,            0               },
    { SettingsDef::DB_KEY_SCROLL_TICKER,             0               },
    { SettingsDef::DB_KEY_LREAR,                      0      },
    { SettingsDef::DB_KEY_RREAR,                      2      },
    { SettingsDef::DB_KEY_VOICE_APPLE_SIRI,           0      },
    { SettingsDef::DB_KEY_VIP_SOUND,                  0      },
    { SettingsDef::DB_KEY_RESET_ALL_COMPLETE,            0 }
};

static const QString btRingtone[2][10]=
{
    {"bell01.wav","bell02.wav","bell03.wav","bell04.wav","bell05.wav","bell06.wav","bell07.wav","bell08.wav","bell09.wav","bell10.wav"},
    {"bell 01","bell 02","bell 03","bell 04","bell 05","bell 06","bell 07","bell 08","bell 09","bell 10"}
};

#endif // DHAVN_APPHOMESCREEN_DEF_H
