#include <DHAVN_AppHomeScreen_InitData.h>
#include <DHAVN_AppHomeScreen_AppEngine.h>

#define IDX_SETTING_IN_A_TYPE          3
#define IDX_SETTING_IN_B_TYPE          4
#define IDX_SETTING_IN_C_TYPE          4
#define IDX_SETTING_IN_D_TYPE          5

struct IMAGE_PATH_T
{
    HSDefP::IMG_T nImageId;
    QString sImgPath;
};

struct COLORS_T
{
    HSDefP::COLOR_T nColor;
    QString sColor;
};

struct MENU_MODEL_T
{
    HSDefP::VIEW_ID_T nViewId;
    HSDefP::APP_ID_T nAppId;
    QString sItemText;
    QString sIcon;
    int nCVFront;
    int nCVRear;
    QList<HSDefP::APP_DATA_T> lDataListFront;
    QList<HSDefP::APP_DATA_T> lDataListRear;
};

struct VIEW_LAYOUT_CONSTS_T
{
    HSDefP::IMG_T nBGImgId;
    HSDefP::IMG_T nBGMaskImgId;
    QPoint nBGMaskPosition;
    QPoint nItemPosition[ COUNT_ITEMS_ON_SCREEN ];
};

const VIEW_LAYOUT_CONSTS_T VIEW_CONSTS[ COUNT_VIEW_LAYOUT ] =
{

    // Main Menu layout for 3
    { HSDefP::IMG_BG_MAIN, HSDefP::IMG_NONE,
      QPoint( 0, HSDefP::BG_MAIN_MASK_Y ),
      { QPoint( 127+378+410, 287 ), QPoint( 127+378, 287+74 ), QPoint( 127, 287 ) }
    },

    // Main Menu layout for 4
    { HSDefP::IMG_BG_MAIN, HSDefP::IMG_NONE,
      QPoint( 0, HSDefP::BG_MAIN_MASK_Y ),
      { QPoint( 96+254+330+272, 264 ), QPoint( 96+254+330, 264+92 ), QPoint( 96+254, 264+92 ), QPoint( 96, 264 ) }
    },

    // Main Menu layout for 5
    { HSDefP::IMG_BG_MAIN, HSDefP::IMG_NONE,
      QPoint( 0, HSDefP::BG_MAIN_MASK_Y ),
      { QPoint( 120+206, 125 ), QPoint( 120+206+179+253, 125 ),
        QPoint( 120+206+179+253+168, 125+162 ), QPoint( 120+206+179, 125+162+74 ), QPoint( 120, 125+162 )
      }
    },

    // Main Menu layout for 6
    { HSDefP::IMG_BG_MAIN, HSDefP::IMG_NONE,
      QPoint( 0, HSDefP::BG_MAIN_MASK_Y ),
      { QPoint( 120+133, 103+31 ), QPoint( 120+133+252+45, 103 ), QPoint( 120+133+252+45+297, 103+31 ),
        QPoint( 120+133+252+45+297+79, 103+31+163 ), QPoint( 120+133+252, 103+31+163+74 ), QPoint( 120, 103+31+163 )
      }
    },

    // Main Menu layout for 7
    { HSDefP::IMG_BG_MAIN, HSDefP::IMG_NONE,
      QPoint( 0, HSDefP::BG_MAIN_MASK_Y ),
      { QPoint( 277, 134 ), QPoint( 550, 103 ), QPoint( 823, 134 ),
        QPoint( 995, 274 ), QPoint( 702, 371 ), QPoint( 332, 371 ), QPoint( 59, 274 ) }
    },

    // Main Menu layout for 8
    { HSDefP::IMG_BG_MAIN, HSDefP::IMG_NONE,
      QPoint( 0, HSDefP::BG_MAIN_MASK_Y ),
      { QPoint( 222, 137 ), QPoint( 432, 103 ), QPoint( 668, 103 ), QPoint( 878, 137 ),
        QPoint( 985, 279 ), QPoint( 688, 366 ), QPoint( 69+153+125, 366 ), QPoint( 69, 279 ) }
    },

    // Main Menu layout for 9
    { HSDefP::IMG_BG_MAIN, HSDefP::IMG_NONE,
      QPoint( 0, HSDefP::BG_MAIN_MASK_Y ),
      { QPoint( 39+183, 103+34 ), QPoint( 39+183+24+186, 103 ), QPoint( 39+183+24+186+81+155, 103 ), QPoint( 39+183+24+186+81+155+132+78, 103+34 ),
        QPoint( 39+183+24+186+81+155+132+78+137, 103+34+138 ), QPoint( 39+183+24+186+81+155+132, 103+34+138+111 ), QPoint( 39+183+24+186+81, 103+34+138+111+13 ),
        QPoint( 39+183+24, 103+34+138+111 ), QPoint( 39, 103+34+138 ) }
    }
};

const IMAGE_PATH_T IMAGE_PATHS[ ] =
{
    { HSDefP::IMG_NONE,                             "" },

    /*************************
            * Main images           *
            *************************/
    { HSDefP::IMG_BG_MAIN,                          "/app/share/images/AppHome/bg_home.png" },

    /*************************
            * Icon/Full menu images *
            *************************/
    { HSDefP::IMG_ICON_APPLICATION,                 "/app/share/images/AppHome/2dep/icon_2dep_app" },
    { HSDefP::IMG_ICON_AUX,                         "/app/share/images/AppHome/2dep/icon_2dep_aux" },
    { HSDefP::IMG_ICON_BLUELINK,                    "/app/share/images/AppHome/ico_home_bluelink" },
    { HSDefP::IMG_ICON_BLUELINK_ASSISTANT,          "/app/share/images/AppHome/ico_home_google" },
    { HSDefP::IMG_ICON_BLUELINK_CENTER,             "/app/share/images/AppHome/2dep/icon_2dep_blcenter" }, /** temp */
    { HSDefP::IMG_ICON_BLUELINK_PHONE,              "/app/share/images/AppHome/2dep/icon_2dep_blphone" },
    { HSDefP::IMG_ICON_BLUELINK_SETTINGS,           "/app/share/images/AppHome/2dep/icon_2dep_blset" },
    { HSDefP::IMG_ICON_BLUELINK_PHONE_DIAL,         "/app/share/images/AppHome/ico_home_bt_dial" },
    { HSDefP::IMG_ICON_BLUETOOTH,                   "/app/share/images/AppHome/2dep/icon_2dep_bt" },
    { HSDefP::IMG_ICON_BT,                          "/app/share/images/AppHome/ico_home_phone" },
    { HSDefP::IMG_ICON_BT_PANDORA,                  "/app/share/images/AppHome/2dep/icon_2dep_bt_pandora" },
    { HSDefP::IMG_ICON_BT_AHA,                      "/app/share/images/AppHome/2dep/icon_2dep_bt_aha" },
    { HSDefP::IMG_ICON_BT_SET,                      "/app/share/images/AppHome/2dep/icon_2dep_phone_set" },
    { HSDefP::IMG_ICON_DISC,                        "/app/share/images/AppHome/2dep/icon_2dep_disc" },
    { HSDefP::IMG_ICON_DMB,                         "/app/share/images/AppHome/ico_home_dmb" },
    { HSDefP::IMG_ICON_DRIVING_INFO,                "/app/share/images/AppHome/2dep/icon_2dep_driving_info" },
    { HSDefP::IMG_ICON_E_MANUAL,                    "/app/share/images/AppHome/ico_home_emanual" },
    { HSDefP::IMG_ICON_GENERAL,                     "/app/share/images/AppHome/2dep/icon_2dep_setting" },
    { HSDefP::IMG_ICON_INFO,                        "/app/share/images/AppHome/2dep/icon_2dep_help" },
    { HSDefP::IMG_ICON_VIDEO_QUICK_GUIDE,           "/app/share/images/AppHome/ico_home_interactive_guide" },
    { HSDefP::IMG_ICON_JBOX,                        "/app/share/images/AppHome/ico_home_jukebox" },
    { HSDefP::IMG_ICON_MEDIA,                       "/app/share/images/AppHome/ico_home_media" },
    { HSDefP::IMG_ICON_NAVI,                        "/app/share/images/AppHome/ico_home_navi" },
    { HSDefP::IMG_ICON_NETWORK,                     "/app/share/images/AppHome/2dep/icon_2dep_wifi" },
    { HSDefP::IMG_ICON_PANDORA,                     "/app/share/images/AppHome/2dep/icon_2dep_pandora" },
    { HSDefP::IMG_ICON_RADIO,                       "/app/share/images/AppHome/ico_home_radio" },
    { HSDefP::IMG_ICON_SCREEN,                      "/app/share/images/AppHome/2dep/icon_2dep_display" },
    { HSDefP::IMG_ICON_SEND_TO_CAR,                 "/app/share/images/AppHome/2dep/icon_2dep_send2car" },
    { HSDefP::IMG_ICON_SETTING,                     "/app/share/images/AppHome/ico_home_setting" },
    { HSDefP::IMG_ICON_SOUND,                       "/app/share/images/AppHome/2dep/icon_2dep_sound" },
    { HSDefP::IMG_ICON_SYSTEM,                      "/app/share/images/AppHome/2dep/icon_2dep_system_info" },
    { HSDefP::IMG_ICON_USB,                         "/app/share/images/AppHome/ico_home_usb" },
    { HSDefP::IMG_ICON_VEHICLE_DIAGNOSTICS,         "/app/share/images/AppHome/2dep/icon_2dep_diagnostics" },
    { HSDefP::IMG_ICON_VOIREC,                      "/app/share/images/AppHome/2dep/icon_2dep_vr" },
    { HSDefP::IMG_ICON_XM,                          "/app/share/images/AppHome/ico_home_sxm" },
    { HSDefP::IMG_ICON_XM_DATA,                     "/app/share/images/AppHome/ico_home_sxm_data" },
    { HSDefP::IMG_ICON_AUDIO_JUKEBOX,               "/app/share/images/AppHome/2dep/icon_2dep_juke_audio" },
    { HSDefP::IMG_ICON_AUDIO_USB,                   "/app/share/images/AppHome/2dep/icon_2dep_usb_audio" },
    { HSDefP::IMG_ICON_PHOTO_USB,                   "/app/share/images/AppHome/2dep/icon_2dep_usb_photo" },
    { HSDefP::IMG_ICON_PHOTO_JUKEBOX,               "/app/share/images/AppHome/2dep/icon_2dep_juke_img" },
    { HSDefP::IMG_ICON_VIDEO_USB,                   "/app/share/images/AppHome/2dep/icon_2dep_usb_video" },
    { HSDefP::IMG_ICON_VIDEO_JUKEBOX,               "/app/share/images/AppHome/2dep/icon_2dep_juke_video" },

    /*********************
    * Title area images *
    *********************/
    { HSDefP::IMG_TITLE_BG,                         "/app/share/images/general/bg_title.png" },
    { HSDefP::IMG_TITLE_BTN,                        "/app/share/images/AppHome/btn_title_back" },

    { HSDefP::IMG_ICON_AHA,                         "/app/share/images/AppHome/2dep/icon_2dep_aha" },

    { HSDefP::IMG_ICON_IPOD,                      "/app/share/images/AppHome/2dep/icon_2dep_ipod" },

    { HSDefP::IMG_ICON_CLOCK,                       "/app/share/images/AppHome/2dep/icon_2dep_clock" },
    { HSDefP::IMG_ICON_NAVI_SETTING,                "/app/share/images/AppHome/2dep/icon_2dep_navi" },

    // Home Ver 2 icons
    { HSDefP::IMG_ICON_MAIN_PHONE_180,                "/app/share/images/AppHome/main/ico_home_180_phone" },
    { HSDefP::IMG_ICON_MAIN_PHONE_196,                "/app/share/images/AppHome/main/ico_home_196_phone" },
    { HSDefP::IMG_ICON_MAIN_PHONE_226,                "/app/share/images/AppHome/main/ico_home_226_phone" },
    { HSDefP::IMG_ICON_MAIN_PHONE_250,                "/app/share/images/AppHome/main/ico_home_250_phone" },

    { HSDefP::IMG_ICON_MAIN_APPS_180,                "/app/share/images/AppHome/main/ico_home_180_app" },

    { HSDefP::IMG_ICON_MAIN_BLUELINK_180,                "/app/share/images/AppHome/main/ico_home_180_bluelink" },

    { HSDefP::IMG_ICON_MAIN_SETTING_180,                "/app/share/images/AppHome/main/ico_home_180_setting" },
    { HSDefP::IMG_ICON_MAIN_SETTING_196,                "/app/share/images/AppHome/main/ico_home_196_setting" },
    { HSDefP::IMG_ICON_MAIN_SETTING_232,                "/app/share/images/AppHome/main/ico_home_232_setting" },
    { HSDefP::IMG_ICON_MAIN_SETTING_238,                "/app/share/images/AppHome/main/ico_home_238_setting" },

    { HSDefP::IMG_ICON_MAIN_MEDIA_180,                "/app/share/images/AppHome/main/ico_home_180_media" },
    { HSDefP::IMG_ICON_MAIN_MEDIA_196,                "/app/share/images/AppHome/main/ico_home_196_media" },
    { HSDefP::IMG_ICON_MAIN_MEDIA_226,                "/app/share/images/AppHome/main/ico_home_226_media" },
    { HSDefP::IMG_ICON_MAIN_MEDIA_234,                "/app/share/images/AppHome/main/ico_home_234_media" },
    { HSDefP::IMG_ICON_MAIN_MEDIA_246,                "/app/share/images/AppHome/main/ico_home_246_media" },
    { HSDefP::IMG_ICON_MAIN_MEDIA_250,                "/app/share/images/AppHome/main/ico_home_250_media" },
    { HSDefP::IMG_ICON_MAIN_MEDIA_270,                "/app/share/images/AppHome/main/ico_home_270_media" },

    { HSDefP::IMG_ICON_MAIN_DMB_234,                "/app/share/images/AppHome/main/ico_home_234_dmb" },
    { HSDefP::IMG_ICON_MAIN_DMB_246,                "/app/share/images/AppHome/main/ico_home_246_dmb" },

    { HSDefP::IMG_ICON_MAIN_RADIO_232,                "/app/share/images/AppHome/main/ico_home_232_radio" },
    { HSDefP::IMG_ICON_MAIN_RADIO_234,                "/app/share/images/AppHome/main/ico_home_234_radio" },
    { HSDefP::IMG_ICON_MAIN_RADIO_238,                "/app/share/images/AppHome/main/ico_home_238_radio" },
    { HSDefP::IMG_ICON_MAIN_RADIO_246,                "/app/share/images/AppHome/main/ico_home_246_radio" },
    { HSDefP::IMG_ICON_MAIN_RADIO_250,                "/app/share/images/AppHome/main/ico_home_250_radio" },
    { HSDefP::IMG_ICON_MAIN_RADIO_270,                "/app/share/images/AppHome/main/ico_home_270_radio" },

    { HSDefP::IMG_ICON_MAIN_NAVI_226,                "/app/share/images/AppHome/main/ico_home_226_navi" },
    { HSDefP::IMG_ICON_MAIN_NAVI_232,                "/app/share/images/AppHome/main/ico_home_232_navi" },
    { HSDefP::IMG_ICON_MAIN_NAVI_234,                "/app/share/images/AppHome/main/ico_home_234_navi" },
    { HSDefP::IMG_ICON_MAIN_NAVI_246,                "/app/share/images/AppHome/main/ico_home_246_navi" },

    { HSDefP::IMG_ICON_MAIN_XM_DATA_180,                "/app/share/images/AppHome/main/ico_home_180_sxmdata" },
    { HSDefP::IMG_ICON_MAIN_XM_DATA_226,                "/app/share/images/AppHome/main/ico_home_226_sxmdata" },

    { HSDefP::IMG_ICON_MAIN_XM_234,                "/app/share/images/AppHome/main/ico_home_234_sxm" },
    { HSDefP::IMG_ICON_MAIN_XM_246,                "/app/share/images/AppHome/main/ico_home_246_sxm" },
    { HSDefP::IMG_ICON_MAIN_XM_254,                "/app/share/images/AppHome/main/ico_home_254_sxm" },

    /** Add new path before this line */
    { HSDefP::IMG_MAX,                              "" }
};

const COLORS_T COLORS[] =
{
    { HSDefP::COLOR_BLACK,           "#000000" },
    { HSDefP::COLOR_BRIGHT_GREY,     "#FAFAFA" },
    { HSDefP::COLOR_SUB_TEXT_GREY,   "#D4D4D4" },
    { HSDefP::COLOR_GREY,            "#C1C1C1" },
    { HSDefP::COLOR_DIMMED_GREY,     "#9E9E9E" },
    { HSDefP::COLOR_DISABLE_GREY,    "#5B5B5B" },
    { HSDefP::COLOR_FOCUS_GREY,      "#484848" },
    { HSDefP::COLOR_BUTTON_GREY,     "#2F2F2F" },
    { HSDefP::COLOR_PROGRESS_BLUE,   "#0087EF" },
    { HSDefP::COLOR_DIMMED_BLUE,     "#447CAD" },
    { HSDefP::COLOR_128_191_255,     "#3CB3FF" },
    { HSDefP::COLOR_112_112_112,     "#3F4D5C" },
    { HSDefP::COLOR_41_41_41,        "#292929" },

    { HSDefP::COLOR_MAX,             ""        }
};

const MENU_MODEL_T MENU_MODEL_MAIN[] =
{
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_NAVI,                "STR_HOME_NAVIGATION",      IMAGE_PATHS[ HSDefP::IMG_ICON_NAVI ].sImgPath,      CV_ALL,                                 CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,               "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_RADIO ].sImgPath,     CV_ALL,                                 CV_ALL,   DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SIRIUS_XM_FROM_HOME, "STR_XM",                   IMAGE_PATHS[ HSDefP::IMG_ICON_XM ].sImgPath,        CV_NORTH_AMERICA,                       CV_NORTH_AMERICA,                             DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,             "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MEDIA ].sImgPath,     CV_ALL,                                 CV_ALL,   DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_DMB,                 "STR_COMMON_OSD_DMB",       IMAGE_PATHS[ HSDefP::IMG_ICON_DMB ].sImgPath,       CV_KOREA,                               CV_INVALID,                             DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BT_PHONE,            "STR_HOME_BLUETOOTH_PHONE", IMAGE_PATHS[ HSDefP::IMG_ICON_BT ].sImgPath,        CV_ALL,                                 CV_MIDDLE_EAST,                         DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,             "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_SETTING ].sImgPath,   CV_ALL,                                 CV_ALL,   DataList, DataList },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_DMB,                 "STR_COMMON_OSD_DMB",       IMAGE_PATHS[ HSDefP::IMG_ICON_DMB ].sImgPath,       CV_INVALID,                             CV_KOREA,                               DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_XM_DATA,             "STR_HOME_XMDATA",          IMAGE_PATHS[ HSDefP::IMG_ICON_XM_DATA ].sImgPath,   CV_NORTH_AMERICA,                       CV_NORTH_AMERICA,                             DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_BLUELINK, HSDefP::APP_ID_INVALID,             "STR_BLUELINK",             IMAGE_PATHS[ HSDefP::IMG_ICON_BLUELINK ].sImgPath,  CV_KOREA | CV_NORTH_AMERICA | CV_CHINA, CV_KOREA | CV_NORTH_AMERICA | CV_CHINA,                    DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM, DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_APP_STORE,      "STR_HOME_APPLICATION",   IMAGE_PATHS[ HSDefP::IMG_ICON_APPLICATION ].sImgPath,     CV_ALL,                                 CV_ALL,   DataList << HSDefP::APP_DATA_INVALID /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList << HSDefP::APP_DATA_INVALID },

    { HSDefP::VIEW_ID_MAX,       HSDefP::APP_ID_MAX,         "",                     IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,            CV_INVALID, CV_INVALID, DataList, DataList }
};

const MENU_MODEL_T MENU_MODEL_MAIN_KOREA[] =
{
    // front
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BT_PHONE,   "STR_HOME_BLUETOOTH_PHONE", IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_PHONE_180 ].sImgPath,        CV_KOREA,   CV_INVALID, DataList, DataList },
    //{ HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_APP_STORE,  "STR_HOME_APPLICATION",     IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_APPS_180 ].sImgPath,     CV_KOREA,   CV_KOREA,   DataList /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList },
    { HSDefP::VIEW_ID_BLUELINK, HSDefP::APP_ID_INVALID,    "STR_BLUELINK",             IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_BLUELINK_180 ].sImgPath,  CV_KOREA,   CV_INVALID,   DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM, DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM },
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,    "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_180 ].sImgPath,   CV_KOREA,   CV_INVALID,   DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,    "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_226 ].sImgPath,     CV_KOREA,   CV_INVALID,   DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_DMB,        "STR_COMMON_OSD_DMB",       IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_DMB_246 ].sImgPath,       CV_KOREA,   CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,      "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_246 ].sImgPath,     CV_KOREA,   CV_INVALID,   DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_NAVI,       "STR_HOME_NAVIGATION",      IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_NAVI_226 ].sImgPath,      CV_KOREA,   CV_INVALID,   DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI },

    // rear
    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,    "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_180 ].sImgPath,     CV_INVALID,   CV_KOREA,   DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_BLUELINK, HSDefP::APP_ID_INVALID,    "STR_BLUELINK",             IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_BLUELINK_180 ].sImgPath,  CV_INVALID,   CV_KOREA,   DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM, DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM },
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,    "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_180 ].sImgPath,   CV_INVALID,   CV_KOREA,   DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_DMB,        "STR_COMMON_OSD_DMB",       IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_DMB_234 ].sImgPath,       CV_INVALID,   CV_KOREA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,      "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_270 ].sImgPath,     CV_INVALID,   CV_KOREA,   DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_NAVI,       "STR_HOME_NAVIGATION",      IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_NAVI_234 ].sImgPath,      CV_INVALID,   CV_KOREA,   DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI },

    { HSDefP::VIEW_ID_MAX,       HSDefP::APP_ID_MAX,         "",                     IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,            CV_INVALID, CV_INVALID, DataList, DataList }
};

const MENU_MODEL_T MENU_MODEL_MAIN_NA[] =
{
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_XM_DATA,             "STR_HOME_XMDATA",          IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_XM_DATA_180 ].sImgPath,   CV_NORTH_AMERICA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BT_PHONE,            "STR_HOME_BLUETOOTH_PHONE", IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_PHONE_180 ].sImgPath,     CV_NORTH_AMERICA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    //{ HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_APP_STORE,          "STR_HOME_APPLICATION",     IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_APPS_180 ].sImgPath,       CV_NORTH_AMERICA, CV_INVALID, DataList /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList },
    { HSDefP::VIEW_ID_BLUELINK, HSDefP::APP_ID_INVALID,             "STR_BLUELINK",             IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_BLUELINK_180 ].sImgPath,  CV_NORTH_AMERICA, CV_INVALID, DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM, DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM },
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,             "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_180 ].sImgPath,   CV_NORTH_AMERICA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,             "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_226 ].sImgPath,     CV_NORTH_AMERICA, CV_INVALID, DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SIRIUS_XM_FROM_HOME, "STR_XM",                   IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_XM_246 ].sImgPath,        CV_NORTH_AMERICA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,               "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_246 ].sImgPath,     CV_NORTH_AMERICA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_NAVI,                "STR_HOME_NAVIGATION",      IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_NAVI_226 ].sImgPath,      CV_NORTH_AMERICA, CV_INVALID, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI },

    //rear
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_XM_DATA,             "STR_HOME_XMDATA",          IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_XM_DATA_180 ].sImgPath,   CV_INVALID, CV_NORTH_AMERICA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    //{ HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BT_PHONE,            "STR_HOME_BLUETOOTH_PHONE", IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_PHONE_180 ].sImgPath,     CV_NORTH_AMERICA, CV_INVALID, DataList, DataList },
    //{ HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_APP_STORE,          "STR_HOME_APPLICATION",     IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_APPS_180 ].sImgPath,       CV_NORTH_AMERICA, CV_INVALID, DataList /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList },
    { HSDefP::VIEW_ID_BLUELINK, HSDefP::APP_ID_INVALID,             "STR_BLUELINK",             IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_BLUELINK_180 ].sImgPath,  CV_INVALID, CV_NORTH_AMERICA, DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM, DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM },
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,             "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_180 ].sImgPath,   CV_INVALID, CV_NORTH_AMERICA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,             "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_226 ].sImgPath,     CV_INVALID, CV_NORTH_AMERICA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SIRIUS_XM_FROM_HOME, "STR_XM",                   IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_XM_246 ].sImgPath,        CV_INVALID, CV_NORTH_AMERICA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,               "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_246 ].sImgPath,     CV_INVALID, CV_NORTH_AMERICA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_NAVI,                "STR_HOME_NAVIGATION",      IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_NAVI_226 ].sImgPath,      CV_INVALID, CV_NORTH_AMERICA, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI },


    { HSDefP::VIEW_ID_MAX,       HSDefP::APP_ID_MAX,         "",                     IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,            CV_INVALID, CV_INVALID, DataList, DataList }
};

const MENU_MODEL_T MENU_MODEL_MAIN_CANADA[] =
{
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_XM_DATA,             "STR_HOME_XMDATA",          IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_XM_DATA_180 ].sImgPath,  CV_CANADA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BT_PHONE,            "STR_HOME_BLUETOOTH_PHONE", IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_PHONE_180 ].sImgPath,    CV_CANADA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,             "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_180 ].sImgPath,  CV_CANADA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,             "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_226 ].sImgPath,    CV_CANADA, CV_INVALID, DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    //{ HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BT_PHONE,            "STR_HOME_BLUETOOTH_PHONE", IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_PHONE_180 ].sImgPath,    CV_CANADA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    //{ HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,             "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_180 ].sImgPath,  CV_CANADA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SIRIUS_XM_FROM_HOME, "STR_XM",                   IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_XM_246 ].sImgPath,       CV_CANADA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,               "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_246 ].sImgPath,    CV_CANADA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_NAVI,                "STR_HOME_NAVIGATION",      IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_NAVI_226 ].sImgPath,     CV_CANADA, CV_INVALID, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI },

    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,             "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_196 ].sImgPath,     CV_INVALID, CV_CANADA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,             "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_196 ].sImgPath,   CV_INVALID, CV_CANADA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SIRIUS_XM_FROM_HOME, "STR_XM",                   IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_XM_234 ].sImgPath,        CV_INVALID, CV_CANADA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,               "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_270 ].sImgPath,     CV_INVALID, CV_CANADA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_NAVI,                "STR_HOME_NAVIGATION",      IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_NAVI_234 ].sImgPath,      CV_INVALID, CV_CANADA, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI },

    { HSDefP::VIEW_ID_MAX,       HSDefP::APP_ID_MAX,         "",                     IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,            CV_INVALID, CV_INVALID, DataList, DataList }
};

const MENU_MODEL_T MENU_MODEL_MAIN_CHINA[] =
{
    //{ HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_APP_STORE,    "STR_HOME_APPLICATION",     IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_APPS_180 ].sImgPath,      CV_CHINA, CV_CHINA,   DataList /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BT_PHONE,      "STR_HOME_BLUETOOTH_PHONE", IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_PHONE_180 ].sImgPath,     CV_CHINA, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_BLUELINK, HSDefP::APP_ID_INVALID,       "STR_BLUELINK",             IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_BLUELINK_180 ].sImgPath,  CV_CHINA, CV_CHINA,   DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM, DataList << HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM },
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,       "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_180 ].sImgPath,   CV_CHINA, CV_CHINA,   DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },

    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,       "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_234 ].sImgPath,     CV_CHINA, CV_CHINA,   DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,         "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_270 ].sImgPath,     CV_CHINA, CV_CHINA,   DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_NAVI,          "STR_HOME_NAVIGATION",      IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_NAVI_234 ].sImgPath,      CV_CHINA, CV_CHINA,   DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI },

    { HSDefP::VIEW_ID_MAX,       HSDefP::APP_ID_MAX,         "",                     IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,            CV_INVALID, CV_INVALID, DataList, DataList }
};

const MENU_MODEL_T MENU_MODEL_MAIN_EU_ME[] =
{
    // front
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BT_PHONE,     "STR_HOME_BLUETOOTH_PHONE", IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_PHONE_196 ].sImgPath,     CV_EUROPE|CV_MIDDLE_EAST, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,      "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_196 ].sImgPath,   CV_EUROPE|CV_MIDDLE_EAST, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,      "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_234 ].sImgPath,     CV_EUROPE|CV_MIDDLE_EAST, CV_INVALID, DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,        "STR_HOME_RADIO",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_270 ].sImgPath,     CV_EUROPE,                CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,        "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_270 ].sImgPath,     CV_MIDDLE_EAST,           CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_NAVI,         "STR_HOME_NAVIGATION",      IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_NAVI_234 ].sImgPath,      CV_EUROPE|CV_MIDDLE_EAST, CV_INVALID, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI },

    // rear
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,      "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_232 ].sImgPath,   CV_INVALID, CV_EUROPE|CV_MIDDLE_EAST, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,      "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_250 ].sImgPath,     CV_INVALID, CV_EUROPE|CV_MIDDLE_EAST, DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,        "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_250 ].sImgPath,     CV_INVALID, CV_EUROPE|CV_MIDDLE_EAST, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_NAVI,         "STR_HOME_NAVIGATION",      IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_NAVI_232 ].sImgPath,      CV_INVALID, CV_EUROPE|CV_MIDDLE_EAST, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI, DataList << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI },

    { HSDefP::VIEW_ID_MAX,      HSDefP::APP_ID_MAX,         "",                     IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,            CV_INVALID, CV_INVALID, DataList, DataList }
};

const MENU_MODEL_T MENU_MODEL_MAIN_EU_ME_NO_NAVI[] =
{
    // front
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,      "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_232 ].sImgPath,   CV_EUROPE|CV_MIDDLE_EAST, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BT_PHONE,     "STR_HOME_BLUETOOTH_PHONE", IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_PHONE_250 ].sImgPath,     CV_EUROPE|CV_MIDDLE_EAST, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,      "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_250 ].sImgPath,     CV_EUROPE|CV_MIDDLE_EAST, CV_INVALID, DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,        "STR_HOME_RADIO",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_232 ].sImgPath,     CV_EUROPE,                CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,        "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_232 ].sImgPath,     CV_MIDDLE_EAST,           CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },

    // rear
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,      "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_238 ].sImgPath,   CV_INVALID, CV_EUROPE|CV_MIDDLE_EAST, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,      "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_270 ].sImgPath,     CV_INVALID, CV_EUROPE|CV_MIDDLE_EAST, DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,        "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_238 ].sImgPath,     CV_INVALID, CV_EUROPE|CV_MIDDLE_EAST, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },

    { HSDefP::VIEW_ID_MAX,      HSDefP::APP_ID_MAX,         "",                     IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,            CV_INVALID, CV_INVALID, DataList, DataList }
};

const MENU_MODEL_T MENU_MODEL_MAIN_GENERAL[] =
{
    { HSDefP::VIEW_ID_SETTINGS, HSDefP::APP_ID_INVALID,      "STR_HOME_SETTINGS",        IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_SETTING_232 ].sImgPath,   CV_GENERAL, CV_GENERAL, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BT_PHONE,     "STR_HOME_BLUETOOTH_PHONE", IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_PHONE_250 ].sImgPath,     CV_GENERAL, CV_GENERAL, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_MEDIA,    HSDefP::APP_ID_INVALID,      "STR_HOME_MEDIA",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_MEDIA_250 ].sImgPath,     CV_GENERAL, CV_GENERAL, DataList << HSDefP::APP_DATA_ENABLED_MEDIA, DataList << HSDefP::APP_DATA_ENABLED_MEDIA },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_RADIO,        "STR_HOME_FM_AM",           IMAGE_PATHS[ HSDefP::IMG_ICON_MAIN_RADIO_232 ].sImgPath,     CV_GENERAL, CV_GENERAL, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },

    { HSDefP::VIEW_ID_MAX,       HSDefP::APP_ID_MAX,         "",                     IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,            CV_INVALID, CV_INVALID, DataList, DataList }

};

const MENU_MODEL_T MENU_MODEL_MEDIA[] =
{
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_JBOX,        "STR_HOME_AUDIO_JUKEBOX", IMAGE_PATHS[ HSDefP::IMG_ICON_AUDIO_JUKEBOX ].sImgPath,     CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_AUIDIO, DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_AUIDIO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_VIDEO_JBOX,        "STR_HOME_VIDEO_JUKEBOX", IMAGE_PATHS[ HSDefP::IMG_ICON_VIDEO_JUKEBOX ].sImgPath,     CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_VIDEO , DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_VIDEO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_PHOTO_JBOX,        "STR_HOME_PHOTO_JUKEBOX", IMAGE_PATHS[ HSDefP::IMG_ICON_PHOTO_JUKEBOX ].sImgPath,     CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_IMAGE /*<< HSDefP::APP_DATA_ENABLED_PARKED*/ , DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_IMAGE },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_USB1,        "STR_HOME_AUDIO_USB_FRONT",    IMAGE_PATHS[ HSDefP::IMG_ICON_AUDIO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB1_AUDIO, DataList << HSDefP::APP_DATA_AVAILABLE_USB1_AUDIO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_VIDEO_USB1,        "STR_HOME_VIDEO_USB_FRONT",    IMAGE_PATHS[ HSDefP::IMG_ICON_VIDEO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB1_VIDEO, DataList << HSDefP::APP_DATA_AVAILABLE_USB1_VIDEO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_PHOTO_USB1,        "STR_HOME_PHOTO_USB_FRONT",    IMAGE_PATHS[ HSDefP::IMG_ICON_PHOTO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB1_IMAGE /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList << HSDefP::APP_DATA_AVAILABLE_USB1_IMAGE  },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_USB2,        "STR_HOME_AUDIO_USB_REAR",    IMAGE_PATHS[ HSDefP::IMG_ICON_AUDIO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB2_AUDIO, DataList << HSDefP::APP_DATA_AVAILABLE_USB2_AUDIO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_VIDEO_USB2,        "STR_HOME_VIDEO_USB_REAR",    IMAGE_PATHS[ HSDefP::IMG_ICON_VIDEO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB2_VIDEO, DataList << HSDefP::APP_DATA_AVAILABLE_USB2_VIDEO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_PHOTO_USB2,        "STR_HOME_PHOTO_USB_REAR",    IMAGE_PATHS[ HSDefP::IMG_ICON_PHOTO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB2_IMAGE /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList << HSDefP::APP_DATA_AVAILABLE_USB2_IMAGE },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_IPOD1,       "STR_OSD_AUDIO_IPOD_FRONT",    IMAGE_PATHS[ HSDefP::IMG_ICON_IPOD ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_IPOD1, DataList << HSDefP::APP_DATA_AVAILABLE_IPOD1 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_IPOD2,       "STR_OSD_AUDIO_IPOD_REAR",    IMAGE_PATHS[ HSDefP::IMG_ICON_IPOD ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_IPOD2, DataList << HSDefP::APP_DATA_AVAILABLE_IPOD2 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AHA1,              "STR_HOME_AHA_FRONT",          IMAGE_PATHS[ HSDefP::IMG_ICON_AHA ].sImgPath,               CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_AHA1, DataList  << HSDefP::APP_DATA_AVAILABLE_AHA1 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AHA2,              "STR_HOME_AHA_REAR",          IMAGE_PATHS[ HSDefP::IMG_ICON_AHA ].sImgPath,               CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_AHA2, DataList  << HSDefP::APP_DATA_AVAILABLE_AHA2 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_PANDORA1,          "STR_HOME_PANDORA_FRONT",      IMAGE_PATHS[ HSDefP::IMG_ICON_PANDORA ].sImgPath,           CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_PANDORA1, DataList << HSDefP::APP_DATA_AVAILABLE_PANDORA1 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_PANDORA2,          "STR_HOME_PANDORA_REAR",      IMAGE_PATHS[ HSDefP::IMG_ICON_PANDORA ].sImgPath,           CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_PANDORA2, DataList << HSDefP::APP_DATA_AVAILABLE_PANDORA2 },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_DISC,        "STR_HOME_DISC_CD",   IMAGE_PATHS[ HSDefP::IMG_ICON_DISC ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_DISC_CD, DataList << HSDefP::APP_DATA_AVAILABLE_DISC_CD },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_DISC,        "STR_HOME_DISC_MP3",  IMAGE_PATHS[ HSDefP::IMG_ICON_DISC ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_DISC_MP3, DataList << HSDefP::APP_DATA_AVAILABLE_DISC_MP3 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_DISC,        "DVD",       IMAGE_PATHS[ HSDefP::IMG_ICON_DISC ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_DISC_DVD_AUDIO, DataList << HSDefP::APP_DATA_AVAILABLE_DISC_DVD_AUDIO },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_VIDEO_DISC,        "VCD",   IMAGE_PATHS[ HSDefP::IMG_ICON_DISC ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_DISC_VCD, DataList << HSDefP::APP_DATA_AVAILABLE_DISC_VCD },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_VIDEO_DISC,        "DVD",   IMAGE_PATHS[ HSDefP::IMG_ICON_DISC ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_DISC_DVD_VIDEO, DataList << HSDefP::APP_DATA_AVAILABLE_DISC_DVD_VIDEO },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_BLUETOOTH,   "STR_HOME_BLUETOOTH_AUDIO",     IMAGE_PATHS[ HSDefP::IMG_ICON_BLUETOOTH ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_BT << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_BT, DataList << HSDefP::APP_DATA_AVAILABLE_BT << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_BT },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_BT_AHA,            "STR_HOME_BT_AHA",          IMAGE_PATHS[ HSDefP::IMG_ICON_BT_AHA ].sImgPath,               CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_BT_AHA, DataList  << HSDefP::APP_DATA_AVAILABLE_BT_AHA },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_BT_PANDORA,        "STR_HOME_BT_PANDORA",      IMAGE_PATHS[ HSDefP::IMG_ICON_BT_PANDORA ].sImgPath,           CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_BT_PANDORA, DataList << HSDefP::APP_DATA_AVAILABLE_BT_PANDORA },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUX,               "STR_HOME_AUX",          IMAGE_PATHS[ HSDefP::IMG_ICON_AUX ].sImgPath,               CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_AUX, DataList << HSDefP::APP_DATA_AVAILABLE_AUX },

    { HSDefP::VIEW_ID_MAX,       HSDefP::APP_ID_MAX,               "",                 IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,           CV_INVALID, CV_INVALID, DataList, DataList }
};

const MENU_MODEL_T MENU_MODEL_MEDIA_FRONT[] =
{
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_JBOX,        "STR_HOME_AUDIO_JUKEBOX", IMAGE_PATHS[ HSDefP::IMG_ICON_AUDIO_JUKEBOX ].sImgPath,     CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_AUIDIO, DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_AUIDIO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_VIDEO_JBOX,        "STR_HOME_VIDEO_JUKEBOX", IMAGE_PATHS[ HSDefP::IMG_ICON_VIDEO_JUKEBOX ].sImgPath,     CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_VIDEO , DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_VIDEO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_PHOTO_JBOX,        "STR_HOME_PHOTO_JUKEBOX", IMAGE_PATHS[ HSDefP::IMG_ICON_PHOTO_JUKEBOX ].sImgPath,     CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_IMAGE /*<< HSDefP::APP_DATA_ENABLED_PARKED*/ , DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_IMAGE },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_USB1,        "STR_HOME_AUDIO_USB",    IMAGE_PATHS[ HSDefP::IMG_ICON_AUDIO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB1_AUDIO, DataList << HSDefP::APP_DATA_AVAILABLE_USB1_AUDIO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_VIDEO_USB1,        "STR_HOME_VIDEO_USB",    IMAGE_PATHS[ HSDefP::IMG_ICON_VIDEO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB1_VIDEO, DataList << HSDefP::APP_DATA_AVAILABLE_USB1_VIDEO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_PHOTO_USB1,        "STR_HOME_PHOTO_USB",    IMAGE_PATHS[ HSDefP::IMG_ICON_PHOTO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB1_IMAGE /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList << HSDefP::APP_DATA_AVAILABLE_USB1_IMAGE  },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_USB2,        "STR_HOME_AUDIO_USB_REAR",    IMAGE_PATHS[ HSDefP::IMG_ICON_AUDIO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB2_AUDIO, DataList << HSDefP::APP_DATA_AVAILABLE_USB2_AUDIO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_VIDEO_USB2,        "STR_HOME_VIDEO_USB_REAR",    IMAGE_PATHS[ HSDefP::IMG_ICON_VIDEO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB2_VIDEO, DataList << HSDefP::APP_DATA_AVAILABLE_USB2_VIDEO },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_PHOTO_USB2,        "STR_HOME_PHOTO_USB_REAR",    IMAGE_PATHS[ HSDefP::IMG_ICON_PHOTO_USB ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_USB2_IMAGE /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList << HSDefP::APP_DATA_AVAILABLE_USB2_IMAGE },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_IPOD1,       "iPod",    IMAGE_PATHS[ HSDefP::IMG_ICON_IPOD ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_IPOD1, DataList << HSDefP::APP_DATA_AVAILABLE_IPOD1 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_IPOD2,       "STR_OSD_AUDIO_IPOD_REAR",    IMAGE_PATHS[ HSDefP::IMG_ICON_IPOD ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_IPOD2, DataList << HSDefP::APP_DATA_AVAILABLE_IPOD2 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AHA1,              "aha",          IMAGE_PATHS[ HSDefP::IMG_ICON_AHA ].sImgPath,               CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_AHA1, DataList  << HSDefP::APP_DATA_AVAILABLE_AHA1 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AHA2,              "STR_HOME_AHA_REAR",          IMAGE_PATHS[ HSDefP::IMG_ICON_AHA ].sImgPath,               CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_AHA2, DataList  << HSDefP::APP_DATA_AVAILABLE_AHA2 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_PANDORA1,          "Pandora",      IMAGE_PATHS[ HSDefP::IMG_ICON_PANDORA ].sImgPath,           CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_PANDORA1, DataList << HSDefP::APP_DATA_AVAILABLE_PANDORA1 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_PANDORA2,          "STR_HOME_PANDORA_REAR",      IMAGE_PATHS[ HSDefP::IMG_ICON_PANDORA ].sImgPath,           CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_PANDORA2, DataList << HSDefP::APP_DATA_AVAILABLE_PANDORA2 },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_DISC,        "STR_HOME_DISC_CD",   IMAGE_PATHS[ HSDefP::IMG_ICON_DISC ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_DISC_CD, DataList << HSDefP::APP_DATA_AVAILABLE_DISC_CD },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_DISC,        "STR_HOME_DISC_MP3",  IMAGE_PATHS[ HSDefP::IMG_ICON_DISC ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_DISC_MP3, DataList << HSDefP::APP_DATA_AVAILABLE_DISC_MP3 },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_DISC,        "DVD",       IMAGE_PATHS[ HSDefP::IMG_ICON_DISC ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_DISC_DVD_AUDIO, DataList << HSDefP::APP_DATA_AVAILABLE_DISC_DVD_AUDIO },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_VIDEO_DISC,        "VCD",   IMAGE_PATHS[ HSDefP::IMG_ICON_DISC ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_DISC_VCD, DataList << HSDefP::APP_DATA_AVAILABLE_DISC_VCD },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_VIDEO_DISC,        "DVD",   IMAGE_PATHS[ HSDefP::IMG_ICON_DISC ].sImgPath,              CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_DISC_DVD_VIDEO, DataList << HSDefP::APP_DATA_AVAILABLE_DISC_DVD_VIDEO },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUDIO_BLUETOOTH,   "STR_HOME_BLUETOOTH_AUDIO",     IMAGE_PATHS[ HSDefP::IMG_ICON_BLUETOOTH ].sImgPath,         CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_BT << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_BT, DataList << HSDefP::APP_DATA_AVAILABLE_BT << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_BT },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_BT_AHA,            "STR_HOME_BT_AHA",          IMAGE_PATHS[ HSDefP::IMG_ICON_BT_AHA ].sImgPath,               CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_BT_AHA, DataList  << HSDefP::APP_DATA_AVAILABLE_BT_AHA },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_BT_PANDORA,        "STR_HOME_BT_PANDORA",      IMAGE_PATHS[ HSDefP::IMG_ICON_BT_PANDORA ].sImgPath,           CV_NORTH_AMERICA | CV_CANADA, CV_NORTH_AMERICA | CV_CANADA,                   DataList << HSDefP::APP_DATA_AVAILABLE_BT_PANDORA, DataList << HSDefP::APP_DATA_AVAILABLE_BT_PANDORA },

    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_AUX,               "STR_HOME_AUX",          IMAGE_PATHS[ HSDefP::IMG_ICON_AUX ].sImgPath,               CV_ALL, CV_ALL,   DataList << HSDefP::APP_DATA_AVAILABLE_AUX, DataList << HSDefP::APP_DATA_AVAILABLE_AUX },

    { HSDefP::VIEW_ID_MAX,       HSDefP::APP_ID_MAX,               "",                 IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,           CV_INVALID, CV_INVALID, DataList, DataList }
};

const MENU_MODEL_T MENU_MODEL_SETTINGS[] =
{
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SETTINGS_NAVI,          "STR_HOME_NAVIGATION",         IMAGE_PATHS[ HSDefP::IMG_ICON_NAVI_SETTING ].sImgPath,          CV_ALL, CV_ALL, DataList  << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI, DataList  << HSDefP::APP_DATA_AVAILABLE_NAVI << HSDefP::APP_DATA_ENABLED_NAVI },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SETTINGS_SOUND,         "STR_HOME_SOUND",              IMAGE_PATHS[ HSDefP::IMG_ICON_SOUND ].sImgPath,         CV_ALL, CV_ALL, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SETTINGS_SCREEN,        "STR_HOME_DISPLAY",            IMAGE_PATHS[ HSDefP::IMG_ICON_SCREEN ].sImgPath,        CV_ALL, CV_ALL, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SETTINGS_BLUETOOTH,     "STR_HOME_BLUETOOTH",          IMAGE_PATHS[ HSDefP::IMG_ICON_BT_SET ].sImgPath,     CV_ALL, CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SETTINGS_VR,            "STR_HOME_VOICE_RECOGNITION",  IMAGE_PATHS[ HSDefP::IMG_ICON_VOIREC ].sImgPath,        CV_KOREA | CV_NORTH_AMERICA | CV_EUROPE | CV_CANADA, CV_KOREA | CV_NORTH_AMERICA | CV_EUROPE | CV_CANADA, DataList << HSDefP::APP_DATA_ENABLED_VR, DataList << HSDefP::APP_DATA_ENABLED_VR },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SETTINGS_CLOCK,         "STR_HOME_CLOCK",              IMAGE_PATHS[ HSDefP::IMG_ICON_CLOCK ].sImgPath,         CV_ALL, CV_ALL, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SETTINGS_GENERAL_BASIC, "STR_HOME_GENERAL",            IMAGE_PATHS[ HSDefP::IMG_ICON_GENERAL ].sImgPath,       CV_ALL, CV_ALL, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    //{ HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_WIFI_SETTINGS,          "STR_HOME_WIFI",               IMAGE_PATHS[ HSDefP::IMG_ICON_NETWORK ].sImgPath,       CV_KOREA | CV_NORTH_AMERICA| CV_CHINA, CV_KOREA | CV_CHINA, DataList /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList },
    //{ HSDefP::VIEW_ID_INFO,     HSDefP::APP_ID_INVALID,                "STR_BT_HELP",                 IMAGE_PATHS[ HSDefP::IMG_ICON_INFO ].sImgPath,          CV_ALL,                                 CV_KOREA | CV_CHINA | CV_MIDDLE_EAST,   DataList, DataList },
    { HSDefP::VIEW_ID_INVALID,     HSDefP::APP_ID_E_MANUAL,                "STR_HOME_E_MANUAL",                 IMAGE_PATHS[ HSDefP::IMG_ICON_INFO ].sImgPath,          CV_ALL,                                 CV_ALL,   DataList << HSDefP::APP_DATA_ENABLED_HELP, DataList << HSDefP::APP_DATA_ENABLED_HELP },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SETTINGS_GENERAL_SYSTEM,"STR_SETTING_SYSTEM_INFO",     IMAGE_PATHS[ HSDefP::IMG_ICON_SYSTEM ].sImgPath,        CV_ALL, CV_ALL, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },

    { HSDefP::VIEW_ID_MAX,                HSDefP::APP_ID_MAX,                    "",                            IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,               CV_INVALID, CV_INVALID, DataList, DataList }
};

const MENU_MODEL_T MENU_MODEL_BLUELINK[] =
{
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_MODEM_DIAL,             "STR_BLUELINK_PHONE",       IMAGE_PATHS[ HSDefP::IMG_ICON_BLUELINK_PHONE ].sImgPath,        CV_KOREA,                               CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_DRIVING_INFO,           "STR_DRIVING_INFO",         IMAGE_PATHS[ HSDefP::IMG_ICON_DRIVING_INFO ].sImgPath,          CV_KOREA | CV_NORTH_AMERICA | CV_CHINA, CV_KOREA | CV_NORTH_AMERICA | CV_CHINA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SEND_TO_CAR,            "STR_SEND_TO_CAR",          IMAGE_PATHS[ HSDefP::IMG_ICON_SEND_TO_CAR ].sImgPath,           CV_KOREA | CV_CHINA, CV_KOREA | CV_CHINA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_SEND_TO_CAR,            "STR_SEND_TO_CAR_NA",       IMAGE_PATHS[ HSDefP::IMG_ICON_SEND_TO_CAR ].sImgPath,        CV_NORTH_AMERICA , CV_NORTH_AMERICA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_VEHICLE_DIAGNOSTICS,    "STR_HOME_VEHICLE_DIAG",    IMAGE_PATHS[ HSDefP::IMG_ICON_VEHICLE_DIAGNOSTICS ].sImgPath,   CV_KOREA | CV_NORTH_AMERICA | CV_CHINA, CV_KOREA | CV_NORTH_AMERICA | CV_CHINA, DataList << HSDefP::APP_DATA_INVALID /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BLUELINK_CENTER,        "STR_BLUELINK_CENTER",      IMAGE_PATHS[ HSDefP::IMG_ICON_BLUELINK_CENTER ].sImgPath,       CV_KOREA,                               CV_INVALID, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_APP_STORE,              "STR_HOME_APPLICATION",     IMAGE_PATHS[ HSDefP::IMG_ICON_APPLICATION ].sImgPath,           CV_NORTH_AMERICA,            CV_NORTH_AMERICA, DataList << HSDefP::APP_DATA_INVALID /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_APP_STORE,              "STR_HOME_APPLICATION_KOR", IMAGE_PATHS[ HSDefP::IMG_ICON_APPLICATION ].sImgPath,           CV_KOREA,            CV_KOREA, DataList << HSDefP::APP_DATA_INVALID /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,  HSDefP::APP_ID_BLUELINK_SETTINGS,      "STR_BLUELINK_SETTING",     IMAGE_PATHS[ HSDefP::IMG_ICON_BLUELINK_SETTINGS ].sImgPath,     CV_KOREA | CV_NORTH_AMERICA | CV_CHINA, CV_KOREA | CV_NORTH_AMERICA | CV_CHINA, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },

    { HSDefP::VIEW_ID_MAX,                          HSDefP::APP_ID_MAX,                 "",                           IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,                      CV_INVALID, CV_INVALID, DataList, DataList }
};

const MENU_MODEL_T MENU_MODEL_INFO[] =
{
    //{ HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_DRIVING_MODE,       "STR_HOME_DRIVING_MODE",        IMAGE_PATHS[ HSDefP::IMG_ICON_DRIVING_MODE ].sImgPath,        CV_ALL, CV_INVALID,                           DataList << HSDefP::APP_DATA_ENABLED_DRIVING_MODE, DataList },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_VIDEO_QUICK_GUIDE,  "STR_HOME_VIDEO_QUICK_GUIDE",   IMAGE_PATHS[ HSDefP::IMG_ICON_VIDEO_QUICK_GUIDE ].sImgPath,   CV_ALL, CV_ALL, DataList << HSDefP::APP_DATA_INVALID, DataList << HSDefP::APP_DATA_INVALID },
    { HSDefP::VIEW_ID_INVALID,   HSDefP::APP_ID_E_MANUAL,           "STR_HOME_E_MANUAL",            IMAGE_PATHS[ HSDefP::IMG_ICON_E_MANUAL ].sImgPath,            CV_ALL, CV_ALL, DataList << HSDefP::APP_DATA_INVALID /*<< HSDefP::APP_DATA_ENABLED_PARKED*/, DataList << HSDefP::APP_DATA_INVALID },

    { HSDefP::VIEW_ID_MAX,       HSDefP::APP_ID_MAX,                "",                             IMAGE_PATHS[ HSDefP::IMG_NONE ].sImgPath,                     CV_INVALID, CV_INVALID, DataList, DataList }
};

InitData::InitData( AppData *pAppData )
    : m_pAppData( pAppData ),
      m_nCountryVariant( CV_INVALID )
{
    switch( AppEngineBase::GetCountryVariant() )
    {
    case eCVKorea: m_nCountryVariant = CV_KOREA; break;
    case eCVCanada: m_nCountryVariant = CV_CANADA; break;
    case eCVNorthAmerica: m_nCountryVariant = CV_NORTH_AMERICA; break;
    case eCVChina: m_nCountryVariant = CV_CHINA; break;
    case eCVMiddleEast: m_nCountryVariant = CV_MIDDLE_EAST; break;
    case eCVEuropa: m_nCountryVariant = CV_EUROPE; break;
    case eCVRussia: m_nCountryVariant = CV_EUROPE; break;
    case eCVGeneral:
    default: m_nCountryVariant = CV_GENERAL;
    }

    m_pAppData->SetValue( HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_PANDORA, QVariant( m_nCountryVariant & ( CV_NORTH_AMERICA ) ) );
    m_pAppData->SetValue( HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_AHA, QVariant( m_nCountryVariant & ( CV_NORTH_AMERICA ) ) );
}

QString
InitData::GetImage( int nID )
{
    if( ( 0 < nID ) &&
            ( nID < HSDefP::IMG_MAX ) &&
            ( (int)IMAGE_PATHS[nID].nImageId == nID ) )
    {
        return IMAGE_PATHS[nID].sImgPath;
    }
    else
    {
        return IMAGE_PATHS[HSDefP::IMG_NONE].sImgPath;
    }
}

QString
InitData::GetColor( int nColor )
{
    if( ( 0 < nColor ) &&
            ( nColor < HSDefP::COLOR_MAX ) &&
            ( (int)COLORS[ nColor ].nColor == nColor ) )
    {
        return COLORS[ nColor ].sColor;
    }
    else
    {
        return COLORS[ HSDefP::COLOR_BRIGHT_GREY ].sColor; //as default
    }
}

void
InitData::FillAppList( IconsModel *pModel, int nDisplay, HSDefP::VIEW_ID_T nViewId )
{
    if( NULL == pModel )
    {
        CR_LOG("Inavild model" );
        return;
    }

    QList<IconItem*> list;
    QList<HSDefP::APP_DATA_T> lData;
    MENU_MODEL_T *pMenuModel = NULL;
    int iInsertedSettingIdx = -1;

    switch( nViewId )
    {
    case HSDefP::VIEW_ID_MAIN:
    {
        if (m_nCountryVariant == CV_KOREA)
            pMenuModel = (MENU_MODEL_T *)MENU_MODEL_MAIN_KOREA;

        else if (m_nCountryVariant == CV_NORTH_AMERICA)
            pMenuModel = (MENU_MODEL_T *)MENU_MODEL_MAIN_NA;

        else if (m_nCountryVariant == CV_CHINA)
            pMenuModel = (MENU_MODEL_T *)MENU_MODEL_MAIN_CHINA;

        else if (m_nCountryVariant == CV_MIDDLE_EAST || m_nCountryVariant == CV_EUROPE)
        {
            if (AppEngineBase::GetSystemVariant() & 0x1)
                pMenuModel = (MENU_MODEL_T *)MENU_MODEL_MAIN_EU_ME;
            else
                pMenuModel = (MENU_MODEL_T *)MENU_MODEL_MAIN_EU_ME_NO_NAVI;
        }

        else if (m_nCountryVariant == CV_CANADA)
            pMenuModel = (MENU_MODEL_T *)MENU_MODEL_MAIN_CANADA;

        else
            pMenuModel = (MENU_MODEL_T *)MENU_MODEL_MAIN_GENERAL;

        break;
    }
    case HSDefP::VIEW_ID_MEDIA: {
        if (AppEngineBase::GetSystemVariant() & 0x00008000)  //enabled rear usb?
        {
            pMenuModel = (MENU_MODEL_T *)MENU_MODEL_MEDIA;
        }
        else
        {
            pMenuModel = (MENU_MODEL_T *)MENU_MODEL_MEDIA_FRONT;
        }
        break;
    }
    case HSDefP::VIEW_ID_SETTINGS: pMenuModel = (MENU_MODEL_T *)MENU_MODEL_SETTINGS; break;
    case HSDefP::VIEW_ID_BLUELINK: pMenuModel = (MENU_MODEL_T *)MENU_MODEL_BLUELINK; break;
    case HSDefP::VIEW_ID_INFO: pMenuModel = (MENU_MODEL_T *)MENU_MODEL_INFO; break;
    default: break;
    }

    if ( nViewId == HSDefP::VIEW_ID_MAIN )
    {
        pModel->SetMainMenuFlag( true );
    }
    else
    {
        pModel->SetMainMenuFlag( false );
    }

    if( NULL != pMenuModel )
    {
        int nIndex = 0;

        while( HSDefP::VIEW_ID_MAX != pMenuModel[ nIndex ].nViewId )
        {
            QList<HSDefP::APP_DATA_T> lDataList;
            if( DISPLAY_FRONT == nDisplay ) lDataList = pMenuModel[ nIndex ].lDataListFront;
            else lDataList = pMenuModel[ nIndex ].lDataListRear;

            lData.append( lDataList );

            if( ( ( DISPLAY_FRONT == nDisplay ) &&
                 ( m_nCountryVariant & pMenuModel[ nIndex ].nCVFront ) ) ||
                    ( ( DISPLAY_REAR == nDisplay ) &&
                     ( m_nCountryVariant & pMenuModel[ nIndex ].nCVRear ) ) )
            {
                bool bShouldBeAdded = true;
                bool bShouldBeAdded2ndStep = true;

                for( int i = 0; i < lDataList.size(); i++ )
                {
                    if( ( lDataList[ i ] > HSDefP::APP_DATA_AVAILABLE_MINIMUM ) &&
                            ( lDataList[ i ] < HSDefP::APP_DATA_AVAILABLE_MAXIMUM ) )
                    {
                        bShouldBeAdded = m_pAppData->GetValue( lDataList[ i ] ).toBool();

                        /*
                        if ( bShouldBeAdded == true )
                        {
                            //! If it is disc, check it is audio disc or video disc
                            if ( lDataList[ i ] == HSDefP::APP_DATA_AVAILABLE_DISC )
                            {
                                if ( lDataList.contains( HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_DISC ) == true )
                                {
                                    bShouldBeAdded2ndStep = m_pAppData->GetValue( HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_DISC ).toBool();
                                }
                                else if ( lDataList.contains( HSDefP::APP_DATA_ENABLED_MEDIA_VIDEO_DISC ) == true )
                                {
                                    bShouldBeAdded2ndStep = m_pAppData->GetValue( HSDefP::APP_DATA_ENABLED_MEDIA_VIDEO_DISC ).toBool();
                                }
                            }
                        }
                        */
                        break;
                    }
                }

                if( !bShouldBeAdded || !bShouldBeAdded2ndStep )
                {
                    nIndex++;
                    continue;
                }

                /*
                if ( pMenuModel[ nIndex ].nViewId == HSDefP::VIEW_ID_SETTINGS )
                {
                    iInsertedSettingIdx = list.size();
                }
                */

                bool bEnable = GetEnable( lDataList );
                int disableCount = GetDisableCount ( lDataList );
                bool bEnableInDrivingMode = GetEnableInDrivingMode( lDataList );

                IconItem *pIconItem = new IconItem( pMenuModel[ nIndex ].nViewId,
                                                   pMenuModel[ nIndex ].nAppId,
                                                   pMenuModel[ nIndex ].sItemText,
                                                   pMenuModel[ nIndex ].sIcon,
                                                   bEnable,
                                                   disableCount,
                                                   bEnableInDrivingMode,
                                                   lDataList  );

                connect( m_pAppData, SIGNAL( ValueChanged( HSDefP::APP_DATA_T, QVariant ) ),
                        pIconItem, SLOT( UpdateDataField( HSDefP::APP_DATA_T, QVariant ) ) , Qt::UniqueConnection );

                list.append( pIconItem );
            }

            nIndex++;
        }
    }

    //setSettingViewLayout( &list, iInsertedSettingIdx );

    if ( !list.isEmpty() )
    {
        pModel->AppendItems( list );
    }

    if ( !lData.isEmpty() )
    {
        pModel->SetDataList( lData );
    }
}

int InitData::getViewLayoutIdx( bool isMainMenu, int nCountItems )
{
    int iResultIdx = 0;

    if ( isMainMenu == true )
    {
        switch( nCountItems )
        {
        case HSDefP::VIEW_LAYOUT_3: iResultIdx = 0; break;
        case HSDefP::VIEW_LAYOUT_4: iResultIdx = 1; break;
        case HSDefP::VIEW_LAYOUT_5: iResultIdx = 2; break;
        case HSDefP::VIEW_LAYOUT_6: iResultIdx = 3; break;
        case HSDefP::VIEW_LAYOUT_7: iResultIdx = 4; break;
        case HSDefP::VIEW_LAYOUT_8: iResultIdx = 5; break;
        case HSDefP::VIEW_LAYOUT_9: iResultIdx = 6; break;
        default: break;
        }
    }
    /*
    else
    {
        switch( nCountItems )
        {
        case 1: iResultIdx = 18; break;
        case HSDefP::VIEW_LAYOUT_E: iResultIdx = 4; break;
        case HSDefP::VIEW_LAYOUT_F: iResultIdx = 5; break;
        case HSDefP::VIEW_LAYOUT_G: iResultIdx = 6; break;
        case HSDefP::VIEW_LAYOUT_H: iResultIdx = 7; break;
        case HSDefP::VIEW_LAYOUT_I: iResultIdx = 8; break;
        case HSDefP::VIEW_LAYOUT_J: iResultIdx = 9; break;
        case HSDefP::VIEW_LAYOUT_K: iResultIdx = 10; break;
        case HSDefP::VIEW_LAYOUT_L: iResultIdx = 11; break;
        case HSDefP::VIEW_LAYOUT_M: iResultIdx = 12; break;
        case HSDefP::VIEW_LAYOUT_N: iResultIdx = 13; break;
        case HSDefP::VIEW_LAYOUT_O: iResultIdx = 14; break;
        default: break;
        }
    }
    */

    return iResultIdx;
}

/*
void InitData::setSettingViewLayout( QList<IconItem*> *iconList, int curSettingIdx )
{
    int iExpectedSettingIdx = -1;

    if ( iconList )
    {
        switch( iconList->size() )
        {
        case HSDefP::VIEW_LAYOUT_A: iExpectedSettingIdx = IDX_SETTING_IN_A_TYPE; break;
        case HSDefP::VIEW_LAYOUT_B: iExpectedSettingIdx = IDX_SETTING_IN_B_TYPE; break;
        case HSDefP::VIEW_LAYOUT_C: iExpectedSettingIdx = IDX_SETTING_IN_C_TYPE; break;
        case HSDefP::VIEW_LAYOUT_D: iExpectedSettingIdx = IDX_SETTING_IN_D_TYPE; break;
        default: break;
        }

        if ( ( iExpectedSettingIdx >= 0 ) && ( iExpectedSettingIdx < iconList->size() ) &&
                ( curSettingIdx != iExpectedSettingIdx ) && ( curSettingIdx >= 0 ) &&
                ( curSettingIdx < iconList->size() ) )
        {
            iconList->move( curSettingIdx, iExpectedSettingIdx );
        }
    }
}
*/


int
InitData::GetDisableCount( const QList<HSDefP::APP_DATA_T> &lData )
{
    int count = 0;

    for( int i = 0; i < lData.size(); i++ )
    {
        if ( lData[i] > HSDefP::APP_DATA_ENABLED_MINIMUM && lData[i] < HSDefP::APP_DATA_ENABLED_MAXIMUM )
        {
            if ( false == GetAppData()->GetValue(lData[ i ]).toBool() )
                count += 1;
        }
    }
    return count;
}

bool
InitData::GetEnable ( const QList<HSDefP::APP_DATA_T> &lData )
{
    for( int i = 0; i < lData.size(); i++ )
    {
        if ( lData[i] > HSDefP::APP_DATA_ENABLED_MINIMUM && lData[i] < HSDefP::APP_DATA_ENABLED_MAXIMUM )
        {
            if ( false == GetAppData()->GetValue(lData[ i ]).toBool() )
                return false;
        }
    }
    return true;
}

bool
InitData::GetEnableInDrivingMode ( const QList<HSDefP::APP_DATA_T> &lData )
{
    for( int i = 0; i < lData.size(); i++ )
    {
        if( lData[ i ] == HSDefP::APP_DATA_ENABLED_PARKED )
            return false;
    }
    return true;
}

void
InitData::FillViewModel( int nDisplay, IconsModel *pModel, HSDefP::VIEW_ID_T nViewId, int nTopMarginDelta )
{
    if( NULL != pModel )
    {
        FillAppList( pModel, nDisplay, nViewId );
        UpdateMargins( pModel, nTopMarginDelta );

        connect( m_pAppData, SIGNAL( ValueChanged( HSDefP::APP_DATA_T, QVariant ) ),
                pModel, SLOT( UpdateDataField( HSDefP::APP_DATA_T, QVariant ) ) , Qt::UniqueConnection );
    }
}

void
InitData::UpdateMargins( IconsModel *pModel, int nTopMarginDelta )
{
    if( NULL != pModel )
    {
        int iViewLayoutIdx = getViewLayoutIdx( pModel->IsMainMenu(), pModel->rowCount() );

        if ( ( iViewLayoutIdx >= 0 ) && ( iViewLayoutIdx < COUNT_VIEW_LAYOUT ) )
        {
            for( int nIndex = 0; nIndex < pModel->rowCount(); nIndex++ )
            {
                pModel->SetPosition( nIndex,
                                    VIEW_CONSTS[ iViewLayoutIdx ].nItemPosition[ nIndex ].x(),
                                    VIEW_CONSTS[ iViewLayoutIdx ].nItemPosition[ nIndex ].y() + nTopMarginDelta );
            }
        }
    }
}

QString
InitData::GetBG( int nCountItems, bool isMainMenu )
{
    if( nCountItems <= COUNT_ITEMS_ON_SCREEN )
    {
        int iViewLayoutIdx = getViewLayoutIdx( isMainMenu, nCountItems );

        if ( ( iViewLayoutIdx >= 0 ) && ( iViewLayoutIdx < COUNT_VIEW_LAYOUT ) )
        {
            return GetImage( VIEW_CONSTS[ iViewLayoutIdx ].nBGImgId );
        }
    }

    return QString();
}

QString InitData::GetBGMask( int nCountItems, bool isMainMenu )
{
    if( nCountItems <= COUNT_ITEMS_ON_SCREEN )
    {
        int iViewLayoutIdx = getViewLayoutIdx( isMainMenu, nCountItems );

        if ( ( iViewLayoutIdx >= 0 ) && ( iViewLayoutIdx < COUNT_VIEW_LAYOUT ) )
        {
            return GetImage( VIEW_CONSTS[ iViewLayoutIdx ].nBGMaskImgId );
        }
    }

    return QString();
}

QPoint
InitData::GetBGMaskPosition( int nCountItems, bool isMainMenu )
{
    if( nCountItems <= COUNT_ITEMS_ON_SCREEN )
    {
        int iViewLayoutIdx = getViewLayoutIdx( isMainMenu, nCountItems );

        if ( ( iViewLayoutIdx >= 0 ) && ( iViewLayoutIdx < COUNT_VIEW_LAYOUT ) )
        {
            return QPoint( VIEW_CONSTS[ iViewLayoutIdx ].nBGMaskPosition.x(),
                          VIEW_CONSTS[ iViewLayoutIdx ].nBGMaskPosition.y() );
        }
    }

    return QPoint();
}
