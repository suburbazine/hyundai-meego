#include <QDeclarativeContext>
#include <QtDeclarative>
#include <QDebug>
#include <QObject>
#include <DHAVN_AppFramework_Event_Def.h>
#include <QmlStatusBar/osdwriter.h> ///modified by edo.lee 2013.04.04
#include "DHAVN_AppMediaPlayer_Shared.h"
#include <uistate_manager_def.h>
#include <DHAVN_AppSettings_Shared.h>
#include "util/audio/DHAVN_AppMediaPlayer_CATreeModel.h"
#include <DHAVN_AppFramework_AppEngineBase.h>
#include "engine/DHAVN_AppMediaPlayer_EngineAudio.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/audio/DHAVN_AppMediaPlayer_Utils.h"
#include "util/osd/OSD_Manager.h"

#define USE_NEW_SETTING_ENTRY_POINT     1 // modified by kihyung 2013.1.26

// { added by  yongkyun.lee 2012.11.21 for  xSampa check.
#define VR_MINING_MASK_DISCON 0x8000 // device disconnect
#define VR_MINING_MASK_JK     0x3000 // 
#define VR_MINING_MASK_USB1   0x0600 // 
#define VR_MINING_MASK_USB2   0x00c0 // 
#define VR_MINING_MASK_IPOD1  0x0018 // 
#define VR_MINING_MASK_IPOD2  0x0003 // 

#define VR_BIT_MASK_JUKEBOX   0x1000
#define VR_BIT_MASK_USB1      0x0200
#define VR_BIT_MASK_USB2      0x0040
#define VR_BIT_MASK_IPOD1     0x0010
#define VR_BIT_MASK_IPOD2     0x0001
// } added by  yongkyun.lee 

OPTION_MENU_T SHUFFLE_MENU[] =
{
    { MP::OPTION_MENU_SHUFFLE_MAIN,   "STR_MEDIA_SHUFFLE",     false, false, false, false, true,  NULL,  0,  "" }, // changed by eugeny - 12-09-15
    { MP::OPTION_MENU_SHUFFLE_ON,     "STR_MEDIA_RANDOM_ON",   false, true,  false, true,  true,  NULL,  0,  "" },
    { MP::OPTION_MENU_SHUFFLE_OFF,    "STR_MEDIA_RANDOM_OFF",  false, true,  false, true,  true,  NULL,  0,  "" }
};

// { changed by eugeny - 12-09-15
OPTION_MENU_T REPEAT_MENU[] =
{
    { MP::OPTION_MENU_REPEAT_MAIN,     "STR_MEDIA_REPEAT",      false,  false, false, true, true,  NULL,  0,  "" },
    { MP::OPTION_MENU_REPEAT_ALL,      "STR_MEDIA_REPEAT_ALL",  false,  true,  false, true, true,  NULL,  0,  "" },
    { MP::OPTION_MENU_REPEAT_ONE,      "STR_MEDIA_REPEAT_ONE",  false,  true,  false, true, true,  NULL,  0,  "" }
};

OPTION_MENU_T REPEAT_MENU_FOLDER[] =
{
    { MP::OPTION_MENU_REPEAT_MAIN,     "STR_MEDIA_REPEAT",        false,  false, false, true, true,  NULL,  0,  "" },
    { MP::OPTION_MENU_REPEAT_ALL,      "STR_MEDIA_REPEAT_ALL",    false,  true,  false, true, true,  NULL,  0,  "" },
    { MP::OPTION_MENU_REPEAT_FOLDER,   "STR_MEDIA_REPEAT_FOLDER", false,  true,  false, true, true,  NULL,  0,  "" },
    { MP::OPTION_MENU_REPEAT_ONE,      "STR_MEDIA_REPEAT_ONE",    false,  true,  false, true, true,  NULL,  0,  "" }
};

OPTION_MENU_T SCAN_MENU_FOLDER[] =
{
    { MP::OPTION_MENU_SCAN_MAIN,     "MENU",             false,  false, false, true, true,  NULL,  0,  "" },
    { MP::OPTION_MENU_SCAN_ALL,      "STR_MEDIA_ALL",    false,  false,  false, true, true,  NULL,  0,  "" }, //modified by wonseok.heo for ITS 176846 2013.06.28
    { MP::OPTION_MENU_SCAN_FOLDER,   "STR_MEDIA_FOLDER", false,  false,  false, true, true,  NULL,  0,  "" }, //modified by wonseok.heo for ITS 176846 2013.06.28
};
// } changed by eugeny - 12-09-15

OPTION_MENU_T JUKEBOX_MAIN_MENU[] =
{
    // { modified by eugene 2012.12.13 for New UX #12
    { MP::OPTION_MENU_JUKEBOX,         "MENU",                         false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_VIEW,            "STR_MEDIA_FLOW_VIEW",          false, false, false, true, false, NULL,          0,  "" },
    { MP::OPTION_MENU_LIST,			  "STR_MEDIA_LIST", 			  false, false, false, true, true, NULL,		  0,  "" },
    { MP::OPTION_MENU_SHUFFLE,         "STR_MEDIA_SHUFFLE",            false, false, false, true, true, SHUFFLE_MENU,  sizeof(SHUFFLE_MENU)/sizeof(OPTION_MENU_T), "" },
    { MP::OPTION_MENU_REPEAT,          "STR_MEDIA_REPEAT",             false, false, false, true, true, REPEAT_MENU,   sizeof(REPEAT_MENU)/sizeof(OPTION_MENU_T),  "" },
    //{ MP::OPTION_MENU_ADD_TO_PLAYLIST, "STR_MEDIA_ADD_TO_PLAYLIST",	  false, false, false, true, true, NULL,		  0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList
    //{ MP::OPTION_MENU_SEARCH,		  "STR_MEDIA_SEARCH",			  false, false, false, true, true, NULL,		  0,  "" }, //deleted by eunhye 2013.2.13 for UX Scenario 13. Search
    { MP::OPTION_MENU_SCAN,            "STR_MEDIA_SCAN",               false, false, false, true, true, NULL,          0,  "" },
    //{ MP::OPTION_MENU_MORE_LIKE_THIS,  "STR_MEDIA_MORE_LIKE_THIS",     false, false, false, true, true, NULL,          0,  "" }, // modified by ravikanth 14-07-13 for MLT feature is spec out
    // added by yongkyun.lee@lge.com    2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
    //{ MP::OPTION_MENU_FILE_INFO,       "STR_MEDIA_FILE_INFO",          false, false, false, true, true, NULL,          0,  "" },//deleted by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList
    //{ MP::OPTION_MENU_MORE_INFO,       "STR_MEDIA_MORE_INFO",          true,  false, false, true, true, NULL,          0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList
    { MP::OPTION_MENU_SOUND_SETTING,   "STR_MEDIA_SOUND_SETING",       false, false, false, true, true, NULL,          0,  "" }
    // } modified by eugene 2012.12.13 for New UX #12
};

OPTION_MENU_T USB_MAIN_MENU[] =
{
    // { modified by eugene 2012.12.13 for New UX #12
    { MP::OPTION_MENU_USB,             "MENU",                          false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_VIEW,            "STR_MEDIA_FLOW_VIEW",           false, false, false, true, false, NULL,          0,  "" },
    { MP::OPTION_MENU_LIST,			  "STR_MEDIA_LIST", 			   false, false, false, true, true, NULL,		   0,  "" },
    { MP::OPTION_MENU_SHUFFLE,		  "STR_MEDIA_SHUFFLE",			   false, false, false, true, true, SHUFFLE_MENU,  sizeof(SHUFFLE_MENU)/sizeof(OPTION_MENU_T), "" },
    { MP::OPTION_MENU_REPEAT,		  "STR_MEDIA_REPEAT",			   false, false, false, true, true, REPEAT_MENU,   sizeof(REPEAT_MENU)/sizeof(OPTION_MENU_T),  "" },
    //{ MP::OPTION_MENU_ADD_TO_PLAYLIST, "STR_MEDIA_ADD_TO_PLAYLIST",	   false, false, false, true, true, NULL,		   0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList
    { MP::OPTION_MENU_COPY_TO_JUKEBOX, "STR_MEDIA_MNG_COPY_TO_JUKEBOX", false, false, false, true, true, NULL,		   0,  "" },
    // added by yongkyun.lee@lge.com    2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
    //{ MP::OPTION_MENU_SEARCH,		  "STR_MEDIA_SEARCH",			   false, false, false, true, true, NULL,		   0,  "" }, //deleted by eunhye 2013.2.13 for UX Scenario 13. Search
    { MP::OPTION_MENU_SCAN,			  "STR_MEDIA_SCAN", 			   false, false, false, true, true, NULL,		   0,  "" },
    //{ MP::OPTION_MENU_MORE_LIKE_THIS,  "STR_MEDIA_MORE_LIKE_THIS",      false, false, false, true, true, NULL,          0,  "" }, // modified by ravikanth 14-07-13 for MLT feature is spec out
    //{ MP::OPTION_MENU_FILE_INFO,       "STR_MEDIA_FILE_INFO",           false, false, false, true, true, NULL,          0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox FileInfo deleted
    //{ MP::OPTION_MENU_MORE_INFO,       "STR_MEDIA_MORE_INFO",           true,  false, false, true, true, NULL,          0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox MoreInfo deleted
    { MP::OPTION_MENU_SOUND_SETTING,   "STR_MEDIA_SOUND_SETING",        false, false, false, true, true, NULL,          0,  "" }
    // } modified by eugene 2012.12.13 for New UX #12
};

OPTION_MENU_T IPOD_MAIN_MENU[] =
{
    //{changed by junam 2013.07.23 for ITS_KOR_181304
    { MP::OPTION_MENU_IPOD,             "MENU",                         false, false, false, true, true, NULL,          0,  "" },
    //{ MP::OPTION_MENU_VIEW,             "STR_MEDIA_FLOW_VIEW",          false, false, false, true, false,NULL,          0,  "" },
    //}changed by junam

    { MP::OPTION_MENU_LIST,			   "STR_MEDIA_LIST",			   false, false, false, true, false,NULL,		   0,  "" },
    { MP::OPTION_MENU_SHUFFLE,          "STR_MEDIA_SHUFFLE",            false, false, false, true, true, SHUFFLE_MENU,  sizeof(SHUFFLE_MENU)/sizeof(OPTION_MENU_T), "" },
    { MP::OPTION_MENU_REPEAT,           "STR_MEDIA_REPEAT",             false, false, false, true, true, REPEAT_MENU,   sizeof(REPEAT_MENU)/sizeof(OPTION_MENU_T),  "" },
    //{ MP::OPTION_MENU_SEARCH,           "STR_MEDIA_SEARCH",             false, false, false, true, true, NULL,          0,  "" }, //deleted by eunhye 2013.2.13 for UX Scenario 13. Search
    { MP::OPTION_MENU_SCAN,			   "STR_MEDIA_SCAN",			   false, false, false, true, true, NULL,		   0,  "" },
    //{ MP::OPTION_MENU_MORE_LIKE_THIS,   "STR_MEDIA_MORE_LIKE_THIS",	   false, false, false, true, false,NULL,		   0,  "" }, //deleted by aettie.ji 2013.02.04 for ISV 61842,61749
    // added by yongkyun.lee@lge.com    2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
    //{ MP::OPTION_MENU_FILE_INFO,       "STR_MEDIA_FILE_INFO",           false, false, false, true, true, NULL,          0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox FileInfo deleted
    //{ MP::OPTION_MENU_MORE_INFO,        "STR_MEDIA_MORE_INFO",          true,  false, false, true, true, NULL,          0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox MoreInfo deleted
    { MP::OPTION_MENU_SOUND_SETTING,    "STR_MEDIA_SOUND_SETING",       false, false, false, true, true, NULL,          0,  "" }
    // } modified by eugene 2012.12.13 for New UX #12
};

//{added by junam 2013.07.23 for ITS_KOR_181304
OPTION_MENU_T IPOD_MUSC_APP_MENU[] =
{
    { MP::OPTION_MENU_IPOD,             "MENU",                         false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_IPOD_MUISC_APP,			   "STR_MEDIA_RETURN_TO_IPOD",			   false, false, false, true, true, NULL,		   0,  "" }
};
//}added by junam

OPTION_MENU_T DISC_MAIN_MENU[] =
{
    // { modified by eugene 2012.12.13 for New UX #12
    { MP::OPTION_MENU_DISC,             "MENU",                         false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_LIST,			   "STR_MEDIA_LIST",			   false, false, false, true, true, NULL,		   0,  "" },
    { MP::OPTION_MENU_SHUFFLE,          "STR_MEDIA_SHUFFLE",            false, false, false, true, true, SHUFFLE_MENU,  sizeof(SHUFFLE_MENU)/sizeof(OPTION_MENU_T), "" },
    { MP::OPTION_MENU_REPEAT,           "STR_MEDIA_REPEAT",             false, false, false, true, true, REPEAT_MENU,   sizeof(REPEAT_MENU)/sizeof(OPTION_MENU_T),  "" },
    { MP::OPTION_MENU_SCAN,             "STR_MEDIA_SCAN",               false, false, false, true, true, NULL,          0,  "" },
    //{ MP::OPTION_MENU_MORE_LIKE_THIS,   "STR_MEDIA_MORE_LIKE_THIS",     false, false, false, true, true, NULL,          0,  "" },
    // added by yongkyun.lee@lge.com    2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
    // deleted by Michael.Kim 2013.07.13 for Spec Update
    //{ MP::OPTION_MENU_FILE_INFO,       "STR_MEDIA_FILE_INFO",           false, false, false, true, true, NULL,          0,  "" }, //deleted by yungi 2013.2.15 for UX Scenario 3.Jukebox MoreInfo deleted
    // { modified by yongkyun.lee@lge.com     2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
    //{ MP::OPTION_MENU_MORE_INFO,        "STR_MEDIA_MORE_INFO",          true,  false, false, true, true, NULL,          0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox MoreInfo deleted
    //{ MP::OPTION_MENU_FILE_INFO,        "STR_MEDIA_FILE_INFO",          true,  false, false, true, true, NULL,          0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox FileInfo deleted
    // } modified by  yongkyun.lee@lge.com
    { MP::OPTION_MENU_SOUND_SETTING,    "STR_MEDIA_SOUND_SETING",       false, false, false, true, true, NULL,          0,  "" }
    // } modified by eugene 2012.12.13 for New UX #12
};
//{added by lyg 2012.08.30 for UX Scenario nothing
OPTION_MENU_T DISC_MP3_MAIN_MENU[] =
{
    // { modified by eugene 2012.12.13 for New UX #12
    { MP::OPTION_MENU_DISC,             "MENU",                         false, false, false, true, true, NULL,          0,  "" },
    //{ modified by yongkyun.lee 20130413 for : NO CR MP3 List
    //{ MP::OPTION_MENU_LIST,             "STR_MEDIA_LIST",               false, false, false, true, true,NULL,          0,  "" },
    { MP::OPTION_MENU_LIST,             "STR_MEDIA_LIST",               false, false, false, true, false,NULL,          0,  "" },
    //} modified by yongkyun.lee 20130413
    { MP::OPTION_MENU_SHUFFLE,          "STR_MEDIA_SHUFFLE",            false, false, false, true, true, SHUFFLE_MENU,  sizeof(SHUFFLE_MENU)/sizeof(OPTION_MENU_T), "" },
    { MP::OPTION_MENU_REPEAT,           "STR_MEDIA_REPEAT",             false, false, false, true, true, REPEAT_MENU,   sizeof(REPEAT_MENU)/sizeof(OPTION_MENU_T),  "" },
    { MP::OPTION_MENU_SCAN,             "STR_MEDIA_SCAN",               false, false, false, true, true, NULL,          0,  "" },
    //   { MP::OPTION_MENU_MORE_LIKE_THIS,   "STR_MEDIA_MORE_LIKE_THIS",     false, false, false, true, true, NULL,          0,  "" }, // removed by eugeny.novikov 2012.11.13 for CR 15299
    // added by yongkyun.lee@lge.com    2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
    //{ MP::OPTION_MENU_FILE_INFO,       "STR_MEDIA_FILE_INFO",           false, false, false, true, true, NULL,          0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox FileInfo deleted
    //{ MP::OPTION_MENU_MORE_INFO,        "STR_MEDIA_MORE_INFO",          true,  false, false, true, true, NULL,          0,  "" }, // added by wspark 2012.09.17 for CR13605 //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox MoreInfo deleted
    { MP::OPTION_MENU_SOUND_SETTING,    "STR_MEDIA_SOUND_SETING",       false, false, false, true, true, NULL,          0,  "" }
    // } modified by eugene 2012.12.13 for New UX #12
};
//}aded by lyg 2012.08.30 for UX Scenario nothing
OPTION_MENU_T DISC_DVD_MAIN_MENU[] =
{
    // { modified by kihyung 2012.08.30 for DVD-Audio
    /*
       { MP::OPTION_MENU_DISC,             "MENU",                         false, false, false, true, true, NULL,          0,  "" },
       { MP::OPTION_MENU_LIST,             "STR_MEDIA_LIST",               false, false, false, true, true, NULL,          0,  "" },
       { MP::OPTION_MENU_TITLE_MENU,       "STR_MEDIA_TITLE_MENU",         false, false, false, true, true, SHUFFLE_MENU,  sizeof(SHUFFLE_MENU)/sizeof(OPTION_MENU_T), "" },
       { MP::OPTION_MENU_DISK_MENU,        "STR_MEDIA_DISK_MENU",          false, false, false, true, true, REPEAT_MENU,   sizeof(REPEAT_MENU)/sizeof(OPTION_MENU_T),  "" },
       { MP::OPTION_MENU_DVD_SETTING,      "STR_SETTING_DVD_SETTING",      false, false, false, true, true, NULL,          0,  "" },
       { MP::OPTION_FULL_SCREEN,           "Full Screen",                  false, false, false, true, true, NULL,          0,  "" },
       { MP::OPTION_MENU_DISPLAY_SETTING,  "Display Setting",              false, false, false, true, true, NULL,          0,  "" },
       { MP::OPTION_MENU_SOUND_SETTING,    "STR_MEDIA_SOUND_SETING",       false, false, false, true, true, NULL,          0,  "" }
       */
    { MP::OPTION_MENU_DISC,             "MENU",                         false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_LIST,             "STR_MEDIA_LIST",               false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_TITLE_MENU,       "STR_MEDIA_TITLE_MENU",         false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_DISK_MENU,        "STR_MEDIA_DISK_MENU",          false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_DVD_SETTING,      "STR_SETTING_DVD_SETTING",      false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_FULL_SCREEN,           "STR_MEDIA_FULL_SCREEN",        false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_DISPLAY_SETTING,  "STR_MEDIA_DISPLAY_SETTINGS",   false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_SOUND_SETTING,    "STR_MEDIA_SOUND_SETING",       false, false, false, true, true, NULL,          0,  "" }
    // } modified by kihyung
};

OPTION_MENU_T JUKEBOX_LIST_VIEW_MENU[] =
{
    // { modified by eugene 2012.12.13 for New UX #12
    { MP::OPTION_MENU_JUKEBOX_LIST,      "MENU",                        false, false, false, true, true, NULL,  0,  "" },
    { MP::OPTION_MENU_NOW_PLAYING,       "STR_MEDIA_NOW_PLAYING",       false, false, false, true, true, NULL,  0,  "" },
    //{ MP::OPTION_MENU_ADD_TO_PLAYLIST,	"STR_MEDIA_ADD_TO_PLAYLIST",   false, false, false, true, true, NULL,  0,  "" },   //deleted by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList
    //{ MP::OPTION_MENU_SEARCH,            "STR_MEDIA_SEARCH",            false, false, false, true, true, NULL,  0,  "" }, //deleted by eunhye 2013.2.13 for UX Scenario 13. Search
    //{ MP::OPTION_MENU_MOVE,              "STR_MEDIA_MNG_MOVE",          false, false, false, true, true, NULL,  0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 2.Jukebox Move
    { MP::OPTION_MENU_DELETE,            "STR_MEDIA_MNG_DELETE",        false, false, false, true, true, NULL,  0,  "" },
    { MP::OPTION_MENU_CLEAR_JUKEBOX,     "STR_MEDIA_MNG_CLEAR_JUKEBOX", false, false, false, true, true, NULL,  0,  "" },
    { MP::OPTION_MENU_CAPACITY,          "STR_MEDIA_MNG_CAPACITY",      false, false, false, true, true, NULL,  0,  "" }
    // { modified by eugene 2012.12.13 for New UX #12
};

OPTION_MENU_T USB_LIST_VIEW_MENU[] =
{
    // { modified by eugene 2012.12.13 for New UX #12
    { MP::OPTION_MENU_USB_LIST,          "MENU",                           false, false, false, true, true, NULL,  0,  "" },
    { MP::OPTION_MENU_NOW_PLAYING,       "STR_MEDIA_NOW_PLAYING",          false, false, false, true, true, NULL,  0,  "" },
    //{ MP::OPTION_MENU_ADD_TO_PLAYLIST,	"STR_MEDIA_ADD_TO_PLAYLIST",	  false, false, false, true, true, NULL,  0,  "" }, //deleted by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList
    { MP::OPTION_MENU_COPY_TO_JUKEBOX,   "STR_MEDIA_MNG_COPY_TO_JUKEBOX",  false, false, false, true, true, NULL,  0,  "" },
    //{ MP::OPTION_MENU_SEARCH,            "STR_MEDIA_SEARCH",               false, false, false, true, true, NULL,  0,  "" } //deleted by eunhye 2013.2.13 for UX Scenario 13. Search
    // { modified by eugene 2012.12.13 for New UX #12
};

OPTION_MENU_T DISC_LIST_VIEW_MENU[] =
{
    { MP::OPTION_MENU_DISC_LIST,         "MENU",                         false, false, false, true, true, NULL,  0,  "" },
    { MP::OPTION_MENU_NOW_PLAYING,       "STR_MEDIA_NOW_PLAYING",        false, false, false, true, true, NULL,  0,  "" }
};

OPTION_MENU_T IPOD_LIST_VIEW_MENU[] =
{
    { MP::OPTION_MENU_IPOD_LIST,         "MENU",                         false, false, false, true, true, NULL,  0,  "" },
    { MP::OPTION_MENU_NOW_PLAYING,       "STR_MEDIA_NOW_PLAYING",        false, false, false, true, true, NULL,  0,  "" },
    //{ MP::OPTION_MENU_SEARCH,            "STR_MEDIA_SEARCH",             false, false, false, true, true, NULL,  0,  "" }, //deleted by eunhye 2013.2.13 for UX Scenario 13. Search
    { MP::OPTION_MENU_EDIT_CATEGORY,     "STR_MEDIA_EDIT_CATEGORY",      false, false, false, true, true, NULL,  0,  "" }
};

OPTION_MENU_T MLT_MAIN_MENU[] =
{
    // { modifed by kihyung 2013.1.7. Removed shuffle menu.
    { MP::OPTION_MENU_MLT,                 "MENU",                         false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_LIST,                "STR_MEDIA_LIST",               false, false, false, true, true, NULL,          0,  "" }, // added by dongjin 2013.03.08 for ISV 64892
    { MP::OPTION_MENU_EXIT_MORE_LIKE_THIS, "STR_MEDIA_MORE_LIKE_THIS_OUT", false, false, false, true, true, NULL,          0,  "" },
    { MP::OPTION_MENU_REPEAT,              "STR_MEDIA_REPEAT",             false, false, false, true, true, REPEAT_MENU,   sizeof(REPEAT_MENU)/sizeof(OPTION_MENU_T),  "" },
    { MP::OPTION_MENU_SOUND_SETTING,       "STR_MEDIA_SOUND_SETING",       false, false, false, true, true, NULL,          0,  "" }
    // } modifed by kihyung 2013.1.7. Removed shuffle menu.
};

OPTION_MENU_T LIST_OPERATION_MENU[] =
{
    { MP::OPTION_MENU_IPOD_LIST,         "MENU",                         false, false, false, true, true, NULL,  0,  "" },
    { MP::OPTION_MENU_CANCEL_OPERATION,  "STR_MEDIA_MNG_CANCEL_COPY",    false, false, false, true, true, NULL,  0,  "" },
};

OPTION_MENU_T BLUETOOTH_MENU[] =
{
    { MP::OPTION_MENU_IPOD_LIST,          "MENU",                         false, false, false, true, true, NULL,  0,  "" },
    // { modified by radhakrushna.mohanty@teleca.com 2012.08.03 for CR 12587
    //{ MP::OPTION_MENU_CONNECTION_SETTING, "STR_MEDIA_CONNECTION_SETTING", false, false, false, true, true, NULL,  0,  "" },// removed by edo.lee 2013.01.06
    { MP::OPTION_MENU_SOUND_SETTING,      "STR_MEDIA_SOUND_SETING",       false, false, false, true, true, NULL,  0,  "" },
    // } radhakrushna.mohanty@teleca.com
};

// { added by wspark 2012.09.14 for New UX, AUX
OPTION_MENU_T AUX_AUDIO_MENU[] =
{
    { MP::OPTION_MENU_AUX,                "MENU",                         false, false, false, true, true, NULL,  0,  "" },
    { MP::OPTION_MENU_SOUND_SETTING,      "STR_MEDIA_SOUND_SETING",       false, false, false, true, true, NULL,  0,  "" },
};
// } added by wspark

// added by minho 20121221 for CR16795 Add to show up Chinese keypad popup in option menu 
OPTION_MENU_T KEYPAD_AUDIO_MENU[] = 
{
    { MP::OPTION_MENU_SEARCH,              "MENU",                         false, false, false, true, true, NULL,  0,  "" },
    { MP::OPTION_MENU_KEYPAD_SETTING,      "STR_MEDIA_KEYPAD_SETTING",       false, false, false, true, true, NULL,  0,  "" },
};
// added by minho 20121221 

// { modified by kihyung 2013.1.9 for ISV 69132
// Shared data provider:
extern SharedDataProvider *m_pSharedAudioData;
extern SharedDataProvider *m_pSharedPhotoData;
extern SharedDataProvider *m_pSharedVideoData;
// } modified by kihyung 2013.1.9 for ISV 69132
static const QString KCountryVariantId( "-cv:%1" ); //added by aettie 20130605 for Gracenote restart


AppMediaPlayer_EngineAudio::AppMediaPlayer_EngineAudio( AppMediaPlayer_EngineMain &aEngineMain )
    : m_EngineMain( aEngineMain ),
      m_AudioController( aEngineMain ),
      m_pTranslator( NULL ),
      m_mode ( MP::UNDEFINED ),
      m_pGraceNote( new OrgLgeGracenoteCDLookupDBusInterface("org.lge.GracenoteCDLookupDBus", "/", QDBusConnection::sessionBus(), 0)),
      m_tuneSelectTimer( new QTimer(this)),
      m_tuneDoublePressTimer( new QTimer(this)), //added by WJL 2014.04.15 for ITS 228616
      //m_tunePressDelayTimer(new QTimer(this)), // removed by junam 2013.11.29 for tune
      //m_tuneRequestTimer(this),//removed by junam 2013.03.10 for ISV73458
      //{ added by yongkyun.lee 2012.12.19 for CR 14219 ,POPUP/list close
      m_isAccOn( true ),
      m_isFrontLCD(true),
      //} added by yongkyun.lee
      //m_pQCanController( new CQCANController_ApplnTxRx(this)), //delete by ys-2013.09.30
      m_OptionMenuModel(NULL),
      m_trackerUpdateInProgress(false) // modified by ravikanth 20-04-13
    //m_pClusterUpdateTimer(new QTimer(this)) // added by wspark 2013.04.02 // deleted by wspark 2013.04.10 for cluster update
{
    // { removed by kihyung 2013.1.9 for ISV 69132
    // m_pSharedAudioData = new SharedDataProvider(MEDIAPLAYER_SHARED_MEMORY_KEY);
    // m_pSharedPhotoData = new SharedDataProvider(PHOTOVIEWER_SHARED_MEMORY_KEY);
    // m_pSharedVideoData = new SharedDataProvider(VIDEOPLAYER_SHARED_MEMORY_KEY);
    // } removed by kihyung 2013.1.9 for ISV 69132
    QDeclarativeContext *context = m_EngineMain.GetScreenContext(AppEngineQMLConstants::APP_FRONT_SCREEN);
    context->setContextProperty("AudioController", &m_AudioController);
    context->setContextProperty("AudioListViewModel", m_AudioController.GetListViewModel());
    context->setContextProperty("TreeModel", m_AudioController.GetTreeModel());
    context->setContextProperty("PathViewModel", m_AudioController.GetPathViewModel());
    //context->setContextProperty("SearchEngine", m_AudioController.GetSearchEngine()); //ys-201010 delete search engine
    // added by edo.lee 2013.02.04
    context = m_EngineMain.GetScreenContext(AppEngineQMLConstants::APP_REAR_SCREEN);
    context->setContextProperty("AudioController", &m_AudioController);
    context->setContextProperty("AudioListViewModel", m_AudioController.GetListViewModel());
    context->setContextProperty("TreeModel", m_AudioController.GetTreeModel());
    context->setContextProperty("PathViewModel", m_AudioController.GetPathViewModel());
    //context->setContextProperty("SearchEngine", m_AudioController.GetSearchEngine()); //ys-201010 delete search engine
    // modified by edo.lee
    qmlRegisterUncreatableType<OptionMenuModel>( "OptionMenuModel", 1, 0, "OptionMenuModel", "OptionMenuModel Can not be created from qml" );
    qmlRegisterType<MP>( "AudioControllerEnums", 1, 0, "MP" );
    qmlRegisterType<LVEnums>( "ListViewEnums", 1, 0, "LVEnums" );

    setOptionMenuModel(new OptionMenuModel(JUKEBOX_MAIN_MENU, sizeof(JUKEBOX_MAIN_MENU)/sizeof(OPTION_MENU_T)));
    m_BTSetInfo= false;		//hyochang.ryu 20130821 for P1#4

    m_bAudioOff  = false;
    m_bSaveState = false;
    m_AudioController.setPlayerActive(false);
    m_bMenuVisible = false; //Add by changjin 2012.07.03 : Jog and Tune event should disable on Menu screen
    m_bBasicView = true; // added by kihyung 2012.07.09 for CR 11165
    //m_bSearchState=false; // added by lyg 2012.08.14 for tuneOSD
    //m_bMltState=false; // added by lyg 2012.08.14 for tuneOSD
    m_focusIndex = 0; //added by junam 2013.12.16 for ITS_ME_215036
    m_focusPlaybackControl = false; // added by junggil 2012.07.18 for CR12028
    //m_bPlayedbyAVModeChanged = false; // removed by ruinmdby 2012.08.20
    m_nLastAVModeEvent = MODE_STATE_NONE; // added by ruinmdby 2012.08.20
    //{Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side
    ////{Changed by Alexey Edelev 2012.10.09 Fix for CR14251
    m_TabClicked = false;// added by yongkyun.lee@lge.com  2012.10.30 : : CR 13733  : Sound Path
    //    m_AvModeChangeMutex = false;// added by yongkyun.lee@lge.com  2012.09.25 : : CR 13733  : Sound Path
    ////}Changed by Alexey Edelev 2012.10.09 Fix for CR14251

    //{changed by junam 2013.09.07 for ITS_KOR_185529
    //m_bSeekHardKeyPressed = false; // added by junam 2012.10.08 for CR14112
    m_bNextHardKeyPressed = false;
    m_bPrevHardKeyPressed = false;
    //}changed by junam
    m_maintainPause = false;

    //{modified by aettie 2013.03.19 for Gracenote issue
    //m_bGracenoteIndexingStatus = false;  //added by aettie.ji 2013.02.22 for ISV 71131 etc.
	//deleted by aettie 20131010 unused code
   // m_bGracenoteIndexingStatus_Jukebox = false;
    //m_bGracenoteIndexingStatus_DISC = false;
    //m_bGracenoteIndexingStatus_USB1= false;
    //m_bGracenoteIndexingStatus_USB2= false;
    //}modified by aettie 2013.03.19 for Gracenote issue
    m_bIsKeypadSettings = false; // added by eugeny.novikov 2012.11.13 for CR 15121
    m_vrMiningMask =0x4000; // added by  yongkyun.lee 2012.11.21 for  xSampa check.
    //added by edo.lee 2013.02.01
    m_btDeviceName = "Unknown";	//"No Information";	//modified by hyochang.ryu 20130724
    m_isMetaInfo = false;
    m_isRemoteCtrl = false;
    //added by edo.lee
    m_isBackPressByJog = false;//added by edo.lee 2013.06.07
    m_isFrontSoftBackPressed = false; //added by edo.lee 2013.03.30
    m_isRearSoftBackPressed = false; //added by edo.lee 2013.03.30
    m_MP3ListView = false;// added by yongkyun.lee 20130413 for : NO CR MP3 List
    //m_BtTrackButton = 0; // commented by cychoi 2013.08.13 for unused code // added by hyochang.ryu 20130517 for BT OSD
    m_bCategoryEditMode = false;//added by junam 2013.06.24 for ISV_KR_85467
    m_bTuneEnabled = true;//added by junam 2013.06.28 for ISV86231
    m_BtCallAfterScan = false; //added by sh.kim 2013.08.09 for ITS 183042
    m_bBTMusicInfoAfterBTCall = false; // added by cychoi 2014.03.05 for ITS 228131, ITS 228134 Request BT Music Info after BT call
    //{ modified by yongkyun.lee 2013-09-04 for :  BT Mode key - pause
    m_modeStateHistory.prevMode = MODE_STATE_NONE;
    m_modeStateHistory.currMode = MODE_STATE_NONE;   
    m_modeStateHistory.firstPlayOnModeChange = true;//{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
    //} modified by yongkyun.lee 2013-09-04 
    //m_bKeepFFREWOnBTCall = false; //remove by edo.lee 2013.09.25
    AppMediaPlayer_SettingsStorage::GetInstance()->LoadAudioData(&m_AudioController);
    AppMediaPlayer_SettingsStorage::GetInstance()->LoadUSBAudioExtended(&m_AudioController); // modified by kihyung 2013.07.20 for ITS 0179774
    AppMediaPlayer_SettingsStorage::GetInstance()->LoadCommon();
    if(AppMediaPlayer_SettingsStorage::GetInstance()->GetJukeboxIndexingFinished() == true) {
        m_AudioController.setIsMinerFinished(true);
        m_AudioController.setJukeBoxCategoryTabAvailable();   // added by sungha.choi 2013.08.12 for ITS 182674_2nd
    }
    // added by kihyung 2013.07.25 for ITS 0180896
    if(AppMediaPlayer_SettingsStorage::GetInstance()->GetJukebxoScanningFinished() == false) {
        MP_HIGH << "JUKEBOX SCAN IS NOT FINISHED...-> STOP SCAN" << LOG_ENDL;
        m_EngineMain.stopTrackerAbsScan(true);
    }

    connect(&m_AudioController, SIGNAL(CommandOnBT(int)), this, SLOT(SendCommandToBT(int)) , Qt::QueuedConnection);// modified by yongkyun.lee 2013-08-28 for : BT OSD sync
    connect(&m_AudioController,SIGNAL(setStatusDataByIndex(MP::DEVICE_T, DATA_TYPE_T, int)),
            this,SLOT(onSetStatusDataByIndex(MP::DEVICE_T, DATA_TYPE_T, int)));
    connect(&m_AudioController,SIGNAL(setAppState(MP::DEVICE_T)),
            this,SLOT(onSetAppState(MP::DEVICE_T)));
    connect(m_pGraceNote, SIGNAL(OnMusicIDCDDataReceived(bool)), this, SLOT(MusicIDCDDataReceived(bool)));
	//deleted by aettie 20131010 unused code
   // connect(m_pGraceNote, SIGNAL(CoverArtRetrieved(bool, QString)),
   //         this, SLOT(onCoverArtRetrieved(bool,QString)));
    m_tuneSelectTimer->setSingleShot( true );
    m_tuneSelectTimer->setInterval( 5000 );
    connect( m_tuneSelectTimer, SIGNAL(timeout()), this, SLOT(onTuneSelectTimer()) );

    m_tuneDoublePressTimer->setSingleShot( true );
    m_tuneDoublePressTimer->setInterval( 5000 );
    connect( m_tuneDoublePressTimer, SIGNAL(timeout()), this, SLOT(onTuneDoublePressTimer()) );

    //{removed by junam 2013.11.29 for tune
    //m_tunePressDelayTimer->setSingleShot(true);
    //m_tunePressDelayTimer->setInterval(1000);
    //connect( m_tunePressDelayTimer, SIGNAL(timeout()), this, SLOT(slotSwitchToTune( )));
    //}removed by junam

    connect(m_EngineMain.GetMediaManager(), SIGNAL(discLoaded(bool)), this, SLOT(onDiscLoaded(bool)));
    connect(m_EngineMain.GetMediaManager(), SIGNAL(deviceAttached(MEDIA_SOURCE_T, MEDIA_TYPE_T, bool)), this, SLOT(onDeviceAttached(MEDIA_SOURCE_T,MEDIA_TYPE_T,bool)));
    connect(m_EngineMain.GetMediaManager(), SIGNAL(deviceRemoved(MEDIA_SOURCE_T, MEDIA_TYPE_T)), this, SLOT(onDeviceRemoved(MEDIA_SOURCE_T,MEDIA_TYPE_T)));
    connect(m_EngineMain.GetMediaManager(), SIGNAL(catreedeviceRemoved()), this, SLOT(oncatreeRemoved())); //added by wonseok.heo for ITS 205774 2013.11.01
    //deleted by aettie 20131010 unused code
	//connect(&m_AudioController, SIGNAL(signalSetGracenoteVolumeType(int, QString, QString)), this, SLOT(SetGracenoteVolumeType(int, QString, QString)));
    //connect(&m_AudioController, SIGNAL(signalGracenoteDeviceStateChanged(int, QString, QString, int, int, int, bool)), this, SLOT(onSetGracenoteDeviceStateChanged(int, QString, QString, int, int, int, bool)));
    connect(&m_AudioController, SIGNAL(signalGracenoteHighlightedVolume(int, QString, QString)), this, SLOT(onSetGracenoteHighlightedVolume(int, QString, QString)));
    connect(&m_AudioController, SIGNAL(tocReceived(QString)), this, SLOT(onTocReceived(QString)));
    connect(&m_AudioController, SIGNAL(signalNoContent()), this, SLOT(notifyFormatJukeboxEnd())); // added by edo.lee 2012.12.21
    connect(&m_AudioController, SIGNAL(disableHardKey(bool)), this, SLOT(onSetHardKeyDisable(bool))); //added by jeeeun for MLT play in FF/REW 2012.12.05

    connect(this, SIGNAL(UpdateStateTrackOSD(int)), &m_AudioController, SIGNAL(UpdateStateTrackOSD(int))); //added by junam 2013.05.18 for ISV_KR81848

    connect(&m_AudioController, SIGNAL(iPodPlaying(int, QString, bool)),this, SLOT(OniPodPlaying(int, QString, bool)));
    connect(m_pGraceNote, SIGNAL(GracenoteIndexingStatus(bool, int)), this, SLOT(onGracenoteIndexingStatus(bool, int)));
    connect(m_pGraceNote, SIGNAL(GracenoteKilled()), this, SLOT(onGracenoteKilled())); //added by aettie for Gracenote killed handling
    connect(&m_AudioController, SIGNAL(iPodDeviceConnectInfo(int, QString, bool)),this, SLOT(onIPodDeviceConnectInfo(int, QString, bool))); // added by jungae 2012.12.26, for send the connection information to AppBtPhone

    connect(&m_EngineMain, SIGNAL(scrollAccStop()), this, SIGNAL(scrollAccStop())); // added by lssanh 2013.02.26 ISV73837
    connect(&m_EngineMain, SIGNAL(signalACCchanged(bool)), this, SLOT(OnACCStatusChanged(bool))); // modified by ravikanth 05-10-13 for ITS 0193972
#ifdef DUAL_KEY
     connect(this, SIGNAL(signalTransformEVT2QML_KEY(EVT_ID_T )), &m_EngineMain, SLOT(transformEVT2QML_KEY(EVT_ID_T)));
#endif
    connect(&m_EngineMain, SIGNAL(signalAgreeOSD()), this, SLOT(onSignalAgreeOSD()));
    //connect(&m_EngineMain, SIGNAL(signalPowerNormalOff()), this, SLOT(OnPowerNormalOff())); // modified for ITS 0210855 // removed by Michael.Kim for ITS 212424
    connect(&m_EngineMain, SIGNAL(signalPowerStateLogicOff()), this, SLOT(OnPowerStateLogicOff()));
    connect(&m_EngineMain, SIGNAL(closeCopyCancelConfirmPopup()), this, SIGNAL(closeCopyCancelConfirmPopup())); // modified for ITS 0217509

    //    connect(&m_EngineMain, SIGNAL(jukeboxCopyInProgressChanged(bool)), this, SIGNAL(jukeboxCopyInProgress())); // modified by ravikanth 20-04-13
/* //delete by ys-2013.09.30
    if (m_pQCanController)
    {
        m_pQCanController->Init();
    }
*/
    //{ deleted by yongkyun.lee 20130206 for : NO VR: VR is not using Mining  sig.
    //if ( m_AudioController.GetTrackerExtractor() )
    //{
    //    // { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
    //    //connect( m_AudioController.GetTrackerExtractor(), SIGNAL( minerFinished(bool) ),
    //    //         this, SLOT( NotifyVRTrackerChanged(bool) ) );
    //    connect( m_AudioController.GetTrackerExtractor(), SIGNAL( minerFinished(bool,QString) ),
    //             this, SLOT( NotifyVRTrackerChanged(bool,QString) ) );
    //    // { modified by  yongkyun.lee
    //}
    //{ deleted by yongkyun.lee 20130206
    //OSDview = new QDeclarativeView(); //delete by ys 2013.09.30 //added by edo.lee 2012.12.08 for statusbar
    // { deleted by wspark 2013.04.10 for cluster update
    /*
    // { added by wspark 2013.04.02
    m_pClusterUpdateTimer->setSingleShot( true );
    m_pClusterUpdateTimer->setInterval( 1000 );
    connect( m_pClusterUpdateTimer, SIGNAL(timeout()), this, SLOT(onClusterUpdateTimer()) );
    // } added by wspark
    */
    // } deleted by wspark

    m_pSharedAudioData->SetStatusDateByIndex ( DATA_TYPE_RANDOM_ENABLE, ENABLE); //added by edo.lee 2013.05.06 for VR random disable
    //temporally deleted for gracenote indexing spec out 2013 07 26
    //m_pGraceNote->jukeBoxMLT(); //added by Puneet for fixing MLT Option not highlited for JB/USB1/USB2 issue,if GPP is launched before AVP, 2013.06.12

    m_bRewReachedFirst = false; //added by junam 2013.08.09 for initialize flag
    //deleted by yongkyun.lee 2013-10-11 for : ITS 191526    
}

void AppMediaPlayer_EngineAudio::SendCommandToBT(int nCommand)
{
    MP_LOG << "AppMusicPlayer::MusicAppEngine::SendCommandToBT" << nCommand << LOG_ENDL;
    Event event( EVT_POST_EVENT, GetThisAppId(), APP_BTPHONE, nCommand );

    m_EngineMain.NotifyUISH(event, DISPLAY_FRONT); // modified by lssanh 2013.04.24 cannot send BT command when going to video
}

AppMediaPlayer_EngineAudio::~AppMediaPlayer_EngineAudio()
{
    // { removed by kihyung 2013.1.9 for ISV 69132
    // delete m_pSharedAudioData;
    // delete m_pSharedPhotoData;
    // delete m_pSharedVideoData;
    // } removed by kihyung 2013.1.9 for ISV 69132
    delete m_pTranslator;
    delete m_pGraceNote;
    //delete m_pQCanController; //delete by ys-2013.09.30
    delete m_OptionMenuModel;
    setOptionMenuModel(NULL);
}

void*
AppMediaPlayer_EngineAudio::GetUIState()
{
    return NULL;
}

// { modified by ravikanth 25-04-13 for ITS 0175705
EVT_ID_T AppMediaPlayer_EngineAudio::resetEventForListView(EVT_ID_T eventId)
{
    MP_LOG << LOG_ENDL;
    switch(eventId)
    {
    case EVT_KEY_HU_SEEK_NEXT:
        eventId = EVT_KEY_HU_SEEK_PREV;
        break;

    case EVT_KEY_RRC_SEEK_NEXT:
        eventId = EVT_KEY_RRC_SEEK_PREV;
        break;

    case EVT_KEY_SWRC_SEEK_NEXT:
        eventId = EVT_KEY_SWRC_SEEK_PREV;
        break;

    case EVT_KEY_HU_SEEK_PREV:
        eventId = EVT_KEY_HU_SEEK_NEXT;
        break;

    case EVT_KEY_RRC_SEEK_PREV:
        eventId = EVT_KEY_RRC_SEEK_NEXT;
        break;

    case EVT_KEY_SWRC_SEEK_PREV:
        eventId = EVT_KEY_SWRC_SEEK_NEXT;
        break;

    // { Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
    case EVT_KEY_CCP_SEEK_NEXT:
        eventId = EVT_KEY_CCP_SEEK_PREV;
        break;

    case EVT_KEY_CCP_SEEK_PREV:
        eventId = EVT_KEY_CCP_SEEK_NEXT;
        break;
    // } Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY

    // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
    //case EVT_KEY_RRC_CH_FW:
    //    eventId = EVT_KEY_RRC_CH_BK;
    //    break;

    //case EVT_KEY_RRC_CH_BK:
    //    eventId = EVT_KEY_RRC_CH_FW;
    //    break;
    // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY

    default:
        break;
    }
    return eventId;
}
// } modified by ravikanth 25-04-13 for ITS 0175705

bool
AppMediaPlayer_EngineAudio::EventHandler( Event& aEvent, DISPLAY_T display )
{
    bool result = true;

    m_AudioController.setEventId(aEvent.GetEventId()); // added by eugene 2012.11.26 for ISV #59906
    // modified by ravikanth 25-04-13 for ITS 0175705
    EVT_ID_T currentEvent = aEvent.GetEventId();
    // deleted by aettie 2013.08.01 for ITS 0181682
    // if(m_AudioController.isListView() || m_stateUI == "mltView") //added by Michael.Kim 2013.06.26 for ITS Issue #0176476
    //     currentEvent = resetEventForListView(currentEvent);
    MP_LOG << currentEvent << LOG_ENDL;
    switch ( currentEvent )
    {
    case EVT_REQUEST_PRE_FG:		// modified by Sergey 07.09.2013 for PRE_FG supoort
    {
        if(isAudioOff() == true){
            setAudioOff(false);
        }

        //added by suilyou 20140325 ITS 0230241
        setMaintainPause(false);
        
        //{ added by hyochang.ryu 20130529
        APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();

        //moved by kihyung 2013.08.01 for static test
        
        if(pEventData)
        {
            MP_LOG << "[Audio] EVT_REQUEST_PRE_FG Entry = " << pEventData->sAppEntryPoint << LOG_ENDL;

            //moved by kihyung 2013.08.01 for static test
            //{ added by wonseok.heo for Disc IN OUT TEST 2013.07.20
            if(pEventData->sAppEntryPoint == eAPP_MODE_DYNAMICS_DISC && m_EngineMain.getInOutTestMode() ==false )
            {

                m_EngineMain.setInOutTestMode(true);
                m_EngineMain.InOutTestEject();
                emit sendTestModeStatus(1);
                MP_LOG << " eAPP_MODE_DYNAMICS_DISC ==" << QString::number(pEventData->sAppEntryPoint) << "m_DiscInOutTestMode = " <<m_EngineMain.getInOutTestMode() << LOG_ENDL;
            } // } added by wonseok.heo for Disc IN OUT TEST 2013.07.20
            if(pEventData->sHeader.bTemporalMode==false)
            {
                if(m_AudioController.isBasicView()==false)
                {
                    MP_LOG << "go BasicView" << LOG_ENDL;
                    m_AudioController.setBasicView(true);
                }
            }
            /*if( (pEventData->sAppEntryPoint == eAPP_AVP_AUDIO_MODE_BT ||pEventData->sAppEntryPoint == eAPP_AVP_MODE_BLUETOOTH))
                m_AudioController.onBtRequestTimerStart();   //moved from bottom
            */ //re moved by hyochang.ryu 20130622 for ITS162878

            //m_AudioController.cancelFFRW();//added by edo.lee 2013.05.31 moved to enginemain by suilyou 20130927
            //remove by edo.lee 2013.07.09
            /* if(m_EngineMain.getIsFromPhoto())
            {
                onTuneTimerStop();
                //modified by edo.lee 2013.07.07
                if(DISPLAY_FRONT == display)
                    ClearOSD(true);
                else
                    ClearOSD(false);
                //modified by edo.lee 2013.07.07
            }*/

            // {added by Michael.Kim 2013.06.22 for ISV #163968
            if((m_mode == MP::DISC) && (m_AudioController.getDiscType() == MP::MP3_CD || m_AudioController.getDiscType() == MP::AUDIO_CD)) {
                if(m_AudioController.GetMediaNum() == 0)
                    m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_MAX_TRACK_NUM, 1);
            }
            //{ deleted by yongkyun.lee 2013-08-28 for : BT OSD sync
            //{remove by edo.lee 2013.03.19
            //APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
            //if( m_AudioController.isPaused()&& (pEventData->sAppEntryPoint == eAPP_AVP_AUDIO_MODE_BT ||pEventData->sAppEntryPoint == eAPP_AVP_MODE_BLUETOOTH))
            //{
            //    m_AudioController.Play();
            //}
            //}remove by edo.lee
            //m_EngineMain.setIsFromOtherAV(true); //remove by edo.lee 2013.03.11

            m_nLastAVModeEvent = mapToModeState(pEventData->sAppEntryPoint); // added by kihyung 2013.06.02

            //{added by junam 2013.12.19 for LIST_ENTRY_POINT
            switch(pEventData->sAppEntryPoint)
            {
            case eAPP_AVP_AUDIO_MODE_JUKEBOX:
            case eAPP_AVP_AUDIO_MODE_USB1:
            case eAPP_AVP_AUDIO_MODE_USB2:
            case eAPP_AVP_AUDIO_MODE_IPOD1:
            case eAPP_AVP_AUDIO_MODE_IPOD2:
            case eAPP_AVP_AUDIO_MODE_DISC: // added by oseong.kwon 2014.02.10 for ITS 223602
                MP_MEDIUM<<"UISH => closeListView" << LOG_ENDL;
                emit closeListView();
                break;

            case eAPP_FILEMANAGER_JUKEBOX_AUDIO:
            case eAPP_FILEMANAGER_USB1_AUDIO:
            case eAPP_FILEMANAGER_USB2_AUDIO:
            case eAPP_FILEMANAGER_DISC_AUDIO: // added by oseong.kwon 2014.02.10 for ITS 223602
                MP_MEDIUM<<"UISH => startMusicList" << LOG_ENDL;
                emit startMusicList();
                break;
            case eAPP_FILEMANAGER_IPOD1:
            case eAPP_FILEMANAGER_IPOD2:
                MP_LOG<<"eAPP_FILEMANAGER_IPODx case" <<LOG_ENDL; // TWJ_ITS 235708
                if (m_EngineMain.getisBTCall() == true)
                {
                    emit closeListView();
                    Event event( EVT_KEY_SOFT_AVP,m_EngineMain.GetThisAppId(), APP_UI_MANAGER,
                               ( pEventData->sAppEntryPoint == eAPP_FILEMANAGER_IPOD1)? eAPP_AVP_AUDIO_MODE_IPOD1:eAPP_AVP_AUDIO_MODE_IPOD2 );
                    emit NotifyUISH(event);
                }
                else
                {
                    //2014.10.16: Modified by Tan: 3rd party Music App, should not show list
                    if(m_AudioController.isBasicControlEnableStatus())
                    {
                        MP_HIGH << "TanDebug1: " << "startMusicList OK " <<LOG_ENDL;
                        emit startMusicList();
                    }
                    else
                    {
                        MP_HIGH << "TanDebug1: " << "startMusicList CANCEL" <<LOG_ENDL;
                        emit closeListView();
                        Event event( EVT_KEY_SOFT_AVP,m_EngineMain.GetThisAppId(), APP_UI_MANAGER,
                                     (pEventData->sAppEntryPoint == eAPP_FILEMANAGER_IPOD1)? eAPP_AVP_AUDIO_MODE_IPOD1:eAPP_AVP_AUDIO_MODE_IPOD2 );
                        emit NotifyUISH(event);
                    }
                }
                break;
            default: // make compiler happy
                break;
            }
            //}added by junam

            //{ added for ITS 218508
            if (m_mode == MP::IPOD1 || m_mode == MP::IPOD2)
            {
                if (m_EngineMain.getisBTCall() == true)
                {
                    emit setDraggable(false);
                }
                else
                {
                    emit setDraggable(true);
                    m_EngineMain.GetNotifier()->onChangeBasicControlEnable( m_AudioController.isBasicControlEnableStatus() ); //HMC 2014.08.20
                }
            }
            else
            {
                emit setDraggable(true);
                m_EngineMain.GetNotifier()->onChangeBasicControlEnable(true); //HMC 2014.08.20
            }
            //} added for ITS 218508
        }
        break;
    }
    case EVT_POST_EVENT:
    {
        MP_HIGH << "[Audio] EVT_POST_EVENT source = " << aEvent.GetSource()<< ", command = " << aEvent.GetCommand() << LOG_ENDL;

        if ( aEvent.GetSource() == APP_BTPHONE || aEvent.GetSource() == APP_AVP )
        {
            MP_MEDIUM << "EVT_BT_MUSIC_CMD =" <<  aEvent.GetCommand() << LOG_ENDL;
            if( aEvent.GetCommand() == EVT_BT_MUSIC_SET_INFO )
            {
                //MP_HIGH << "EVT_BT_MUSIC_SET_INFO" << LOG_ENDL;

                if ( !aEvent.Data().isEmpty() )
                {
                    BTMUSIC_CURRENT_PLAY_INFO *pData = (BTMUSIC_CURRENT_PLAY_INFO *)aEvent.Data().data();

                    // removed by cychoi 2013.08.13 for unused code

                    QString track_count = QString::number( pData->iTrackNumber ) + "/" + QString::number( pData->iTotalTrackNumber );
                    QString title = QString::fromUtf8( pData->cstrTitle );
                    QString artist = QString::fromUtf8( pData->cstrArtist );
                    QString album = QString::fromUtf8( pData->cstrAlbumName );
                    
                    MP_LOG<< "EVT_POST_EVENT="  <<  title << album<<  artist << LOG_ENDL;
                    //if(m_AudioController.GetMediaMode() == MP::BLUETOOTH )//{ modified by yongkyun.lee 2013-10-18 for : NOCR BT song info
                        setBTSetInfo(true) ;	//hyochang.ryu 20130821 for P1#4
                        m_AudioController.BTMusicInfoChanged( title, album, artist,track_count);
                    }
            }
            //{ modified by yongkyun.lee 20130427 for : BT sync
            // removed by cychoi 2013.08.13 for unused code
            else if( aEvent.GetCommand() == EVT_BT_MUSIC_STREAMING_STARTED
                     || aEvent.GetCommand() == EVT_BT_MUSIC_STREAMING_PAUSED
                     || aEvent.GetCommand() == EVT_BT_MUSIC_PLAYBACK_STARTED
                     || aEvent.GetCommand() == EVT_BT_MUSIC_PLAYBACK_PAUSED
                     || aEvent.GetCommand() == EVT_BT_MUSIC_STOP )
            {
                //MP_HIGH << "BT event:" << aEvent.GetCommand() << LOG_ENDL;
                m_AudioController.commandFromBT(aEvent.GetCommand());
                //{ added by hyochang.ryu 20130917 for GALAXY Reconnect "Unknow"
                if(aEvent.GetCommand() == EVT_BT_MUSIC_PLAYBACK_STARTED) 
                {
                    // { added by cychoi 2014.06.03 for Get Info - 1. on TrackChanged 2. delayed 0.5sec from PlayStarted
                    m_AudioController.onBtResponseTimerStop();
                    m_AudioController.onBtDelayedGetInfoTimerStop();
                    // } added by cychoi 2014.06.03
                    m_AudioController.onBtPeriodicGetInfoTimerStop(); // added by cychoi 2014.06.05 for Get Info - every 10sec after PlayStarted (some phones don't send TrackChanged)
                    // { added by cychoi 2014.03.05 for ITS 228131, ITS 228134 Request BT Music Info after BT call
                    if(m_EngineMain.getisBTCall() == true && m_EngineMain.getisBLCall() == false) // modified by oseong.kwon 2014.11.18 for ITS 252671
                    {
                        m_bBTMusicInfoAfterBTCall = true;
                    }
                    // } added by cychoi 2014.03.05
                    else
                    {
                        m_EngineMain.setPlayOnBTCall(); // added by oseong.kwon 2014.11.18 for ITS 252671
                        //m_AudioController.onBtRequestIncCount();	//deleted by hyochang.ryu 20130926 for side toggle
                        // { modified by cychoi 2014.06.03 for Get Info - 1. on TrackChanged 2. delayed 0.5sec from PlayStarted
                        m_AudioController.onBtDelayedGetInfoTimerStart();
                        //m_AudioController.onBtRequestTimerStart();
                        // } modified by cychoi 2014.06.03
                    }
                    m_AudioController.onBtPeriodicGetInfoTimerStart(); // added by cychoi 2014.06.05 for Get Info - every 10sec after PlayStarted (some phones don't send TrackChanged)
                }
                //} added by hyochang.ryu 20130917 for GALAXY Reconnect "Unknow"
            }
            //} modified by yongkyun.lee 20130427
            //{ added by yongkyun.lee 20130426 for : BT sync
            else if(aEvent.GetCommand() == EVT_BT_MUSIC_NEXT_TRACK || aEvent.GetCommand() == EVT_BT_MUSIC_PREV_TRACK) //added by hyochang.ryu 20130524
            {
                m_AudioController.onBtResponseTimerStop();// deleted by yongkyun.lee 2013-10-10 for :  ITS 191703 BT getinfo 0.5sec
                m_AudioController.onBtRequestDecCount(); // modified by yongkyun.lee 2013-08-28 for : BT OSD sync
                m_AudioController.onBtDelayedGetInfoTimerStop(); // added by cychoi 2014.06.03 for Get Info - 1. on TrackChanged 2. delayed 0.5sec from PlayStarted
                m_AudioController.onBtPeriodicGetInfoTimerStop(); // added by cychoi 2014.06.05 for Get Info - every 10sec after PlayStarted (some phones don't send TrackChanged)
                // { added by cychoi 2014.03.05 for ITS 228131, ITS 228134 Request BT Music Info after BT call
                if(m_EngineMain.getisBTCall() == true && m_EngineMain.getisBLCall() == false) // modified by oseong.kwon 2014.11.18 for ITS 252671
                {
                    m_bBTMusicInfoAfterBTCall = true;
                }
                // } added by cychoi 2014.03.05
                else
                {
                    m_EngineMain.setPlayOnBTCall(); // added by oseong.kwon 2014.11.18 for ITS 252671
                    m_AudioController.onBtRequestTimerStart();
                }
            }
            //} added by yongkyun.lee 20130426
            //{added by junam 2013.08.27
            else if(aEvent.GetCommand() == EVT_BT_MUSIC_CONN_CONFIG)
            {
                QString strDeviceName1 = m_AudioController.getIPODDeviceName(MEDIA_SOURCE_IPOD1);
                QByteArray strByteArray1 = strDeviceName1.toUtf8();
                QString strDeviceName2 = m_AudioController.getIPODDeviceName(MEDIA_SOURCE_IPOD2);
                QByteArray strByteArray2 = strDeviceName2.toUtf8();

                EVENT_IPOD_CONNECT_INFO_DATA_T pEventData;
                pEventData.iPortNum = 1;
                snprintf(pEventData.cDeviceName, sizeof(pEventData.cDeviceName), "%s", strByteArray1.data());
                pEventData.bConnectStatus = m_AudioController.isApple1Ready();
                Event postEvent1(EVT_POST_EVENT, GetThisAppId(), APP_BTPHONE,
                                 QByteArray((char*)&pEventData, sizeof(pEventData)));
                m_EngineMain.notifyUISH(postEvent1);

                pEventData.iPortNum = 2;
                snprintf(pEventData.cDeviceName, sizeof(pEventData.cDeviceName), "%s", strByteArray2.data());
                pEventData.bConnectStatus = m_AudioController.isApple2Ready();
                Event postEvent2(EVT_POST_EVENT, GetThisAppId(), APP_BTPHONE,
                                 QByteArray((char*)&pEventData, sizeof(pEventData)));
                m_EngineMain.notifyUISH(postEvent2);
            }
            //}added by junam 2013.08.27
        }
        else if( aEvent.GetSource() == APP_FMGR)
        {
            if( aEvent.GetCommand() == EVT_FORMAT_JUKEBOX_START )
            {
                MP_MEDIUM << "from FMGR EVT_FORMAT_JUKEBOX_START" << LOG_ENDL;
                m_AudioController.pauseMiner();
                m_AudioController.ResetDeviceInfo( MP::JUKEBOX );
                if ( m_mode == MP::JUKEBOX )
                {
                    m_pGraceNote->CloseCollectionIDs();
                    m_AudioController.Stop();
                    m_AudioController.clearPlayMode();
                }
                m_AudioController.InitiateJukeBoxFormat();
                result = false;
            }
            else if( aEvent.GetCommand() == EVT_FORMAT_JUKEBOX_END )
            {
                MP_MEDIUM << "from FMGR EVT_FORMAT_JUKEBOX_END" << LOG_ENDL;
                m_AudioController.resumeMiner();

                m_EngineMain.GetMediaManager()->SetSourceAvailable(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_MUSIC, false);
                m_EngineMain.GetMediaManager()->SetSourceAvailable(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_VIDEO, false);
                m_EngineMain.GetMediaManager()->SetSourceAvailable(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_PHOTO, false);

                m_EngineMain.GetNotifier()->setEnableMedia(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_MAX);
            }
            // {  modified by ravikanth 16-04-13
            else if( aEvent.GetCommand() == EVT_JUKEBOX_COPY_START )
            {
                m_AudioController.GetListViewModel()->fileOperationStateChanged(true);
            }
            else if( aEvent.GetCommand() == EVT_JUKEBOX_COPY_END )
            {
                MP_MEDIUM<<"from FMGR [EVT_JUKEBOX_COPY_END]" << LOG_ENDL;
				// modified for ISV on 04-11-13, copy complete notification after copy start
                if(!m_AudioController.GetListViewModel()->isFileOperationInProgress())
                    m_EngineMain.setCopyInProgress(false, 0);
                m_AudioController.GetListViewModel()->fileOperationStateChanged(false);
            }
            else if( aEvent.GetCommand() == EVT_FILEOPERATION_STOP )
            {
                MP_MEDIUM << "from FMGR EVT_FILEOPERATION_STOP" << LOG_ENDL;
                m_AudioController.GetListViewModel()->cancelOperationChangeSource();
                m_EngineMain.setCopyInProgress(false, 0); // modified by eugene.seo 2013.05.29
            }
            //  } modified by ravikanth 16-04-13
        }//{changed by junam 2013.06.06 for factory test additional fix
        else if ( aEvent.GetSource() == APP_MPAUTOTEST)
        {
            if( aEvent.GetCommand() == EVT_MP_IPOD_TRACK_NAME_REQ )
            {
                if(m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2)
                {
                    MediaInfo info;
                    m_AudioController.GetCurrentMediaInfo(&info);
                    m_EngineMain.notifyCurrentTrackName(info.Title);
                }
                else
                {
                    m_EngineMain.notifyCurrentTrackName(QString());
                }

                //{ added by wonseok.heo 2013.06.12 for factory test
            }else if( aEvent.GetCommand() == EVT_MP_TRACK_CHANGE_REQ )
            {

                m_EngineMain.notifyDiscCurrentTrackName(aEvent);

            }
            //} added by wonseok.heo 2013.06.12 for factory test

        }//}changed by junam
        //else
        //{
        //    MP_HIGH << "aEvent.GetSource = " << aEvent.GetSource() << "aEvent.GetCommand() = " << aEvent.GetCommand() << LOG_ENDL;
        //}
        break;
    }

    case EVT_AV_MODE_CHANGE:
    {
        MP_MEDIUM << "[Audio] EVT_AV_MODE_CHANGE" << LOG_ENDL;

        if ( !aEvent.Data().isEmpty() )
        {
            EVENT_MODE_CHANGED_DATA_T* pData = (EVENT_MODE_CHANGED_DATA_T*) aEvent.Data().data();
            
            if (pData != NULL)
            {
                //added by edo.lee 2013.05.01
                if(m_EngineMain.getIsCompleteModeChange() == true)
                {
                    MP_LOG<< " now is proceding to EVT_REQUEST_FG" << LOG_ENDL;
                    break;
                }
                //added by edo.lee
                //{ modified by yongkyun.lee 2013-10-07 for : ITS 193970
                if(OSDManager::instance()->getAUXDisconnect() && ( pData->sMode == MODE_STATE_AUX1_AUDIO || pData->sMode == MODE_STATE_AUX2_AUDIO))
                {
                    MP_LOG << " video aux -> audio aux " << LOG_ENDL;
                    break;
                }
                //} modified by yongkyun.lee 2013-10-07 
                //{ added by yongkyun.lee 20130326 for : ISV 77220
                if (m_AudioController.isLongPress() ||m_nLastAVModeEvent==MODE_STATE_BT_AUDIO )		  //{ added by hyochang.ryu 20130620 for protect REW/FF BT
                {
                    emit clearSeekMode();
                }
                //} added by yongkyun.lee 20130326
                
                // removed by kihyung 2013.07.17
                //{ modified by yongkyun.lee 2013-09-08 for : BT OSD sync
                if((m_nLastAVModeEvent != MODE_STATE_BT_AUDIO || isAudioOff()) && (pData->sMode == MODE_STATE_BT_AUDIO))
                {
                    m_AudioController.onBtRequestIncCount();
                    //m_AudioController.onBtRequestTimerStart();
                }
                //} modified by yongkyun.lee 2013-09-08
                
                m_nLastAVModeEvent = pData->sMode;
                
                // { added by wspark 2013.03.01 for ISV 61633
                switch (pData->sMode)
                {
                // removed by kihyung 2013.07.17
                case MODE_STATE_BT_AUDIO:
                    m_AudioController.DelayedUnmute4BT();	//added by hyochang.ryu 20130913 for ITS189483
                case MODE_STATE_AUX1_AUDIO:
                case MODE_STATE_AUX2_AUDIO:
                case MODE_STATE_JUKEBOX_AUDIO:
                case MODE_STATE_USB1_AUDIO:
                case MODE_STATE_USB2_AUDIO:
                case MODE_STATE_IPOD1_AUDIO:
                case MODE_STATE_IPOD2_AUDIO:
                case MODE_STATE_DISC_AUDIO:
                    // { added by kihyung 2013.08.24 for ITS 0186192
                    if(isStopped() == true && m_AudioController.getDevice() != MP::UNDEFINED)
                    {
                        MP_LOG << "EVT_AV_MODE_CHANGE. Previous mode is stopped -> setDevice(MP::UNDEFINED)" << LOG_ENDL;
                        m_AudioController.setDevice(MP::UNDEFINED);
                    }
                    // } added by kihyung 2013.08.24 for ITS 0186192
                    setAudioOff(false);
                    break;
                default: // make compiler happy
                    break;
                }
                // } added by wspark
                
                if(pData->sMode == MODE_STATE_AVOFF || pData->sMode == MODE_STATE_NONE) {
                    m_EngineMain.GetNotifier()->cancelFileOperation();
                }

                if( getLastAVMode() == pData->sMode) //added by edo.lee 2013.06.13
                {
                    if(m_AudioController.getDiscType() == MP::AUDIO_CD)
                    {
                        // if(m_EngineMain.GetNotifier()->GetAVOffMode() == false)
                        if(m_EngineMain.getIsFromOtherAV() == true && !isAudioOff()) //modified by edo.lee 2013.06.12
                        {
                            emit setDisplayOSD(true); //added by Michael.Kim 2013.05.22 for AV On/Off OSD Implentation
                            emit updateOSD(OSD_EVENT_AUDIO_TRACK_INFO); //added by Michael.Kim 2013.05.22 for AV On/Off OSD Implentation
                        }
                    }
                }
                // removed by kihyung 2013.07.17
                
                MP_HIGH << "Requested mode:" << pData->sMode << LOG_ENDL;
                m_AudioController.setPlayerActive(true);

                switch (pData->sMode)
                {
                case MODE_STATE_BT_AUDIO:
                    // { removed by kihyung 2013.08.01 for static test
                    //    if(!m_AudioController.GetBTAvrcpAbove13())	//added by hyochang.ryu 20130719 for ISV84517
                    //        m_AudioController.Play();
                    // } removed by kihyung 2013.08.01 for static test
                case MODE_STATE_JUKEBOX_AUDIO:
                case MODE_STATE_USB1_AUDIO:
                case MODE_STATE_USB2_AUDIO:
                case MODE_STATE_IPOD1_AUDIO:
                case MODE_STATE_IPOD2_AUDIO:
                case MODE_STATE_AUX1_AUDIO:
                case MODE_STATE_AUX2_AUDIO:
                case MODE_STATE_DISC_AUDIO:
                    DisplayOSD(true); //added by edo.lee 2013.06.13
                    setAudioOff(false);
                    m_AudioController.SetMediaMode(AudioUtils::DeviceType(pData->sMode));
                    m_EngineMain.SetAVOffMode(false);
                    break;

                case MODE_STATE_AVOFF:
                    m_EngineMain.SetAVOffMode(true); // added by kihyung 2013.5.20
                case MODE_STATE_NONE:
                    setAudioOff(true);
                    m_pGraceNote->CloseCollectionIDs();
                    //m_EngineMain.SetAVOffMode(true); // remove by kihyung 2013.3.14
                    m_AudioController.cancelFFRW(); // modified by Dmitry 23.04.13
                    m_AudioController.setScanMode(SCANOFF); //added by Michael.Kim 2013.05.18 ISV #80040
                    //emit closeListView(); //moved by junam for 2013.06.26

                default:
                    MP_LOG << "Other media app is active, pause any playing activities." << LOG_ENDL;
                    PathViewModel *pPathView = m_AudioController.GetPathViewModel();
                    if(pPathView && m_EngineMain.isPowerOff() == false && m_EngineMain.isAVOff() == false && pData->sMode != MODE_STATE_NONE) {
                        MP_LOG << "ROWS pPathView is NOT NULL" << LOG_ENDL;
                        pPathView->clearModelData();
                    }
                    
                    m_AudioController.clearPlayMode();

                    // { modified by kihyung 2013.07.18 for ITS 0180268
                    /*
                    DeviceHistory *pHistory = m_AudioController.GetDeviceHistory(m_mode);
                    if(pHistory && pHistory->playList.isMLT == true) // modified by kihyung 2013.07.02 for ITS 0177327
                        m_AudioController.ExitFromMLT();
                    else
                        m_AudioController.setIsPlayFromMLT(false);
                    */
                    // } modified by kihyung 2013.07.18 for ITS 0180268

                    m_AudioController.iPodDeviceOff();// moved before Pause() by junam 2012.12.13

                    onTuneTimerStop(); // added by lssanh 2013.01.30 ISV69821
                    // { modified by cychoi 2013.11.15 for ITS 205008
                    // { added by wonseok.heo for ITS 205008 2013.11.01
                    //if(m_mode == MP::DISC)
                    //    m_AudioController.Pause();
                    //else
                    m_AudioController.functionStop();
                    // } added by wonseok.heo for ITS 205008 2013.11.01
                    // } modified by cychoi 2013.11.15

                    m_bSaveState = false;
                    m_AudioController.setPlayerActive(false);
                    m_AudioController.setDevice(MP::UNDEFINED); // added by kihyung 2013.07.20 for ITS 0179388
                    emit closeListView(); //moved by junam for 2013.06.26
                    break;
                }

#ifdef BOOT_LOADING_SCREEN
                if(m_EngineMain.GetLastPlayerMode() == APP_MEDIA_MODE_AUDIO && m_EngineMain.m_tBootInfo.bisReadyToPlay == false)
                {
                    MP_HIGH<<"ConnectAVP2UISHReadyToPlay - isReadytoPlay" << LOG_ENDL;
                    m_EngineMain.setAVModeChange(true);
                    //m_EngineMain.m_tBootInfo.bisAVModeChange = true; //ys-20131010 TA scan finish after AV_Mode_change
                    emit isReadytoPlay(); //ys-20131008 boot
                }
#endif
            }
        }
        break;
    }

        // { added by kihyung 2013.06.29
    case EVT_NOTIFY_AV_MODE:
    {
        if(!aEvent.Data().isEmpty())
        {
            EVT_COMMON_BASE_EVENT_DATA_T *pData = (EVT_COMMON_BASE_EVENT_DATA_T*)aEvent.Data().data();
            MODE_STATE_T                  eNextMode = (MODE_STATE_T)pData->iEventData01;
            MODE_STATE_T                  eCurrMode = mapToModeState(m_EngineMain.GetLastAVEntryPoint(false));


            PassNotifyAvModeToiPod(eNextMode);

            //{ modified by yongkyun.lee 2013-09-04 for :  BT Mode key - pause
            m_modeStateHistory.prevMode = m_modeStateHistory.currMode;
            m_modeStateHistory.currMode = eNextMode;
            m_modeStateHistory.firstPlayOnModeChange = true;//{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
            //} modified by yongkyun.lee 2013-09-04

            MP_HIGH << "[Audio] EVT_NOTIFY_AV_MODE :: prevMode =" << m_modeStateHistory.prevMode << "->, currMode =" <<  m_modeStateHistory.currMode << LOG_ENDL;
            //{ modified by yongkyun.lee 2013-09-09 for : AUX video->audio OSD
            if( m_modeStateHistory.prevMode == MODE_STATE_AUX1_VIDEO &&  eNextMode == MODE_STATE_AUX1_AUDIO)
            {
                OSDManager::instance()->setAUXDisconnect(true);
                break;// modified by yongkyun.lee 2013-10-07 for :  ITS 193970
            }
            //} modified by yongkyun.lee 2013-09-09

            //{ modified by yongkyun.lee 2013-09-30 for : ISV 80360
            if((m_AudioController.getScanMode() != SCANOFF) && (eCurrMode == MODE_STATE_DISC_AUDIO && eCurrMode != eNextMode))
                m_AudioController.setScanMode(SCANOFF);
            //} modified by yongkyun.lee 2013-09-30 

            if(eNextMode != eCurrMode)
            {
                if(m_EngineMain.GetAVMediaMode(m_EngineMain.GetLastAVEntryPoint(false)) == APP_MEDIA_MODE_AUDIO)
                {
                    if(eCurrMode != MODE_STATE_BT_AUDIO )//|| (eCurrMode == MODE_STATE_BT_AUDIO && m_AudioController.GetBTAvrcpAbove13())) //{ modified by yongkyun.lee 2013-08-30 for : BT sound mute
                    {
                        // { modified by cychoi 2013.11.17 for ITS 205008
                        // { added by wonseok.heo for ITS 205008 2013.11.01
                        //if(m_mode == MP::DISC)
                        //    m_AudioController.Pause();
                        //else
                            m_AudioController.functionStop();
						// } added by wonseok.heo for ITS 205008 2013.11.01
                        // } modified by cychoi 2013.11.17
                    }
                        
                    /* deleted by hyochang.ryu 20130831 for Blank Tab
                    if(eCurrMode == MODE_STATE_BT_AUDIO) {
                        emit deactivateBTTab();	//added by hyochang.ryu 20130822 for ITS185528
                    }*/
                }

                // { added by kihyung 2013.08.20 for ITS 0185499
                MP::DEVICE_T nNextDevice = AudioUtils::DeviceType(eNextMode);
                if(nNextDevice < MP::ALL_DEVICES && nNextDevice != MP::AUX)	//added by hyochang.ryu 20131007 for ITS194229
                {
                    m_AudioController.EmitOnMediaInfoChanged(nNextDevice, FALSE, FALSE);
                    m_AudioController.updateCoverFlowMediaInfo(nNextDevice, FALSE, true);
                }
                // } added by kihyung 2013.08.20 for ITS 0185499

                //{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
                //if ((eCurrMode == MODE_STATE_BT_AUDIO ) && (eNextMode ==MODE_STATE_BT_PANDORA || eNextMode==MODE_STATE_AHA_RADIO_BT))
                //{
                //    m_EngineMain.setMuteForBTStream(eNextMode);
                //}
                //} modified by yongkyun.lee 2014-03-13

                //{ added ITS 210706,7
                if ((eNextMode == MODE_STATE_IPOD1_AUDIO || eNextMode == MODE_STATE_IPOD2_AUDIO) && (m_EngineMain.getisBTCall() == true))
                {
                    emit setDraggable(false);
                    //if(!isForeground())
                    {
                        OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_EMPTY_TITLE);  // Change to Unknown by Tan 07.25 . ITS 235835 no osd
                    }
                }
                else
                {
                    emit setDraggable(true);
                    m_EngineMain.GetNotifier()->onChangeBasicControlEnable(true); //HMC 2014.08.20
                }
                //} added ITS 210706,7
            }
            //{ commented by yongkyun.lee 2013-09-08 for : BT OSD sync
            //if(eNextMode == MODE_STATE_BT_AUDIO) {
            //    m_AudioController.onBtRequestTimerStart();
            //}
            //} commented by yongkyun.lee 2013-09-08
            // } modified by kihyung 2013.07.31
        }
        break;
    }
        // } added by kihyung 2013.06.29

        // { added by Sergey Vetugov for CR#13599
    case EVT_AUTO_PLAY_IN_BG:
    {
        MP_LOG << "[Audio] EVT_AUTO_PLAY_IN_BG" << LOG_ENDL;    // removed by sangmin.seol 2014.07.08 reduce high log for performance

        if (!aEvent.Data().isEmpty())
        {
            EVENT_AUTO_PLAY_BG_DATA_T* eventData = (EVENT_AUTO_PLAY_BG_DATA_T*)aEvent.Data().data();
            if (eventData)
            {
                // { added by wspark 2012.10.15 for CAN message in BG mode.
                APP_MEDIA_MODE_T notifier_mode = APP_MEDIA_MODE_MAX;
                switch(eventData->sMode)
                {
                case MODE_STATE_JUKEBOX_AUDIO:
                case MODE_STATE_USB1_AUDIO:
                case MODE_STATE_USB2_AUDIO:
                case MODE_STATE_IPOD1_AUDIO:
                case MODE_STATE_IPOD2_AUDIO:
                case MODE_STATE_AUX1_AUDIO:
                case MODE_STATE_AUX2_AUDIO:
                case MODE_STATE_BT_AUDIO:
                case MODE_STATE_DISC_AUDIO:
                    notifier_mode = APP_MEDIA_MODE_AUDIO;
                    break;
                case MODE_STATE_DISC_VIDEO:
                case MODE_STATE_USB1_VIDEO:
                case MODE_STATE_USB2_VIDEO:
                case MODE_STATE_JUKEBOX_VIDEO:
                case MODE_STATE_AUX1_VIDEO:
                case MODE_STATE_AUX2_VIDEO:
                    notifier_mode = APP_MEDIA_MODE_VIDEO;
                    break;
                default:
                    notifier_mode = APP_MEDIA_MODE_MAX;
                    break;
                }
                m_EngineMain.GetNotifier()->SetActiveMode( notifier_mode );
                // } added by wspark
                Restore(eventData->sMode);
            }
        }
        break;
    }
        // } added by Sergey Vetugov for CR#13599

    case EVT_START_AUDIO_PLAY_IN_BG:
    {
        MP_LOG << "[Audio] EVT_START_AUDIO_PLAY_IN_BG" << m_mode << LOG_ENDL;
        if (m_mode != MP::UNDEFINED)
        {
            m_AudioController.SetMediaMode(m_mode);
        }
        break;
    }

    case EVT_REQUEST_APP_EXIT:
    {
        MP_LOG << "[Audio] handle event EVT_REQUEST_APP_EXIT" << LOG_ENDL;
        SaveSettings();
        break;
    }

        // removed by Dmitry
        /*****************************************
    //in the listview
    //track up released --> previouse (oppsed to main player)
    //seek down released --> next (oppsed to main player)

    //track up long pressed --> FF (same to main player)
    //seek down long pressed --> REW (same to main player)
    ******************************************/

    case EVT_KEY_HU_SEEK_NEXT:
    //case EVT_KEY_RRC_CH_FW:         // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
    case EVT_KEY_CCP_SEEK_NEXT:     // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
    case EVT_KEY_RRC_SEEK_NEXT:
    case EVT_KEY_SWRC_SEEK_NEXT:
    {
        MP_LOG << "EVT_KEY_HU_SEEK_NEXT" << LOG_ENDL;

        //{changed by junam 2013.08.27
        if(m_AudioController.isControllerDisable(MP::CTRL_DISABLE_PLAYQUE))
            break;
        //}changed by junam

        //{ Added by Radhakrushna Mohanty 2012.08.18 CR11181
        //if(processedInAUXMode(aEvent)) break;	//deleted by hyochang.ryu 20131009 for ITS194476
        //} Added by Radhakrushna Mohanty 2012.08.18 CR11181
        //if(processedInBTMode(aEvent)) break;	//rollback 20130706 //added by hyochang.ryu 20130704
        //[KR][ITS][183091][comment](aettie.ji) 20130809
        //  emit menuOptionClose(); // added by lssanh 2013.02.20 ISV72878 + option menu close
        emit scrollAccStop(); // added by lssanh 2013.02.26 ISV73837

#ifdef DUAL_KEY
        if(!aEvent.Data().isEmpty() && ((KEY_EVENT_DATA_T*)aEvent.Data().data())->keyStatus != KEY_STATUS_CANCEL)   // DUAL_KEY sangmin.seol 2013.11.27
#endif
        onTuneTimerStop(); // added by Sergey 19.05.2013

        // modified by Dmitry 23.04.13
        if (!aEvent.Data().isEmpty() && !isAudioOff())
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            MP_LOG << "keyStatus = " << eventData->keyStatus << LOG_ENDL;

            switch ( eventData->keyStatus )
            {
#ifndef DUAL_KEY
            case KEY_STATUS_CANCEL:   // modified by yongkyun.lee 2013-07-08 for : ITS 178517
#endif
            case KEY_STATUS_RELEASED:
                //{changed by junam 2013.09.07 for ITS_KOR_185529
                //if (!m_bSeekHardKeyPressed ) // modified by dongjin 2013.01.21 for ISV 69116
#ifndef DUAL_KEY
                if (!m_bNextHardKeyPressed ) // modified by dongjin 2013.01.21 for ISV 69116
                {
                    emit resetPlaybackControl();//{ modified by yongkyun.lee 2013-10-17 for : ITS 196623
                    break;
                }
#endif
                if(currentEvent==EVT_KEY_RRC_SEEK_NEXT)
                    m_EngineMain.sendVRCancelSignal();
                //setSeekHardKeyPressed(false);
                //remove by edo.lee 2013.09.25
                //if(m_bKeepFFREWOnBTCall){
				//	m_bKeepFFREWOnBTCall = false;
				//	break;
                //}
                // added by Dmitry 11.10.13 for ITS0194940
                if (!m_AudioController.isLongPress())
                    emit closeUnsupportedPopup();
                setNextHardKeyPressed(false);
                //}changed by junam
                //{ added by junam 2013.05.18 for ISV_KR81848
                if( m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2 )
                {
                    emit UpdateStateTrackOSD(false);
                }
                //}added by junam
                if (!m_AudioController.cancelFFRW())
                {
                    m_AudioController.setFFRW(false , AppEngineQMLConstants::KEY_STATUS_RELEASED ,AppEngineQMLConstants::JOG_RIGHT );
                    if ( !m_AudioController.isLongPress() )
                    {
                        emit seekNextHardkeyPressed();
                    }
                }
                m_AudioController.setIsLongPress(0); // modified by Michael.Kim 2013.05.10 for ITS Issue for 0167251
                break;

            case KEY_STATUS_PRESSED_LONG:
                //{added by junam 2013.07.09 for ITS178979
#ifndef DUAL_KEY
                if (!m_bNextHardKeyPressed ) //changed by junam 2013.09.07 for ITS_KOR_185529
                {
                    MP_LOG<<"Long press event without press event will be ignored" << LOG_ENDL;
                    break;
                }
#endif
                //}added by junam
                m_AudioController.setIsLongPress(1); // added by kihyung 2013.2.5 for ISV 72327
                m_AudioController.setFFRW(true ,AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED  ,AppEngineQMLConstants::JOG_RIGHT );
                if( m_AudioController.GetMediaMode() != MP::BLUETOOTH )		//added by hyochang.ryu 20130628
                    m_EngineMain.beep(eBeepType1);
                break;

            case KEY_STATUS_PRESSED_CRITICAL:
                //{ added by yongkyun.lee 20130531 for : ITS 170510
                //if (!(m_AudioController.GetPlayerState() == PLAYER_FASTFORWARD_X4 || m_AudioController.GetPlayerState() == PLAYER_FASTFORWARD) ||
                //!m_bSeekHardKeyPressed )//|| m_AudioController.GetListViewModel()->getPopType()>= 0) //modified by edo.lee 2013.05.29
                if (!(m_AudioController.GetPlayerState() == PLAYER_FASTFORWARD_X4 ||
                      m_AudioController.GetPlayerState() == PLAYER_FASTFORWARD    ||
                      m_AudioController.GetPlayerState() == PLAYER_SEARCH           )
#ifndef DUAL_KEY
                        || !m_bNextHardKeyPressed
#endif
                   )//changed by junam 2013.09.07 for ITS_KOR_185529

                    //} added by yongkyun.lee 20130531
                {
                    break;
                }
                m_AudioController.setIsLongPress(1); // added by kihyung 2013.2.5 for ISV 72327
                if( m_AudioController.GetMediaMode() != MP::BLUETOOTH )		//added by hyochang.ryu 20130628
                    m_EngineMain.beep(eBeepType1);
                m_AudioController.setFFRW(true ,AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED ,AppEngineQMLConstants::JOG_RIGHT  );
                break;

            case KEY_STATUS_PRESSED:
                if(processedInAUXMode(aEvent)) break;	//added by hyochang.ryu 20131009 for ITS194476
                //added by suilyou 20130910 for ITS 0186508 START
#ifndef DUAL_KEY
                if(getPrevHardKeyPressed())
                    break;
#endif
                //added by suilyou 20130910 for ITS 0186508 END
                //[KR][ITS][183091][comment](aettie.ji) 20130809
                emit menuOptionClose();
#ifndef DUAL_KEY
                setNextHardKeyPressed(true); //changed by junam 2013.09.07 for ITS_KOR_185529
#endif
                m_AudioController.setFFRW(false ,AppEngineQMLConstants::KEY_STATUS_PRESSED ,AppEngineQMLConstants::JOG_RIGHT  );
                break;
#ifdef DUAL_KEY
            case KEY_STATUS_CANCEL:
                if (!m_AudioController.isLongPress())
                    emit closeUnsupportedPopup();

                m_AudioController.setIsLongPress(0); //added by junam 2014.01.10 for ITS_ME_218697

                if( m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2 )
                {
                    emit UpdateStateTrackOSD(false);
                }

                if (!m_AudioController.cancelFFRW())
                {
                    m_AudioController.setFFRW(false , AppEngineQMLConstants::KEY_STATUS_CANCELED ,AppEngineQMLConstants::JOG_RIGHT );
                }
                break;
#endif
            default:
                break;
            }
        }
        else {
            MP_LOG << "aEvent.Data().isEmpty(): " << aEvent.Data().isEmpty() << "isAudioOff(): " << isAudioOff() << LOG_ENDL;
        }
        // modified by Dmitry 23.04.13
        result = true;
        break;
    }
    case EVT_KEY_HU_SEEK_PREV:
    //case EVT_KEY_RRC_CH_BK:         // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
    case EVT_KEY_CCP_SEEK_PREV:     // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
    case EVT_KEY_RRC_SEEK_PREV:
    case EVT_KEY_SWRC_SEEK_PREV:
        MP_LOG << "EVT_KEY_HU_SEEK_PREV" << LOG_ENDL;

        //{changed by junam 2013.08.27
        if(m_AudioController.isControllerDisable(MP::CTRL_DISABLE_PLAYQUE))
            break;
        //}changed by junam

        //{ Added by Radhakrushna Mohanty 2012.08.18 CR11181
        //if(processedInAUXMode(aEvent)) break;	//deleted by hyochang.ryu 20131009 for ITS194476
        //} Added by Radhakrushna Mohanty 2012.08.18 CR11181
        //if(processedInBTMode(aEvent)) break;	//rollback 20130706 //added by hyochang.ryu 20130704
        //[KR][ITS][183091][comment](aettie.ji) 20130809
        //        emit menuOptionClose(); // added by lssanh 2013.02.20 ISV72878 + option menu close
        emit scrollAccStop(); // added by lssanh 2013.02.26 ISV73837

#ifdef DUAL_KEY
        if(!aEvent.Data().isEmpty() && ((KEY_EVENT_DATA_T*)aEvent.Data().data())->keyStatus != KEY_STATUS_CANCEL)   // DUAL_KEY sangmin.seol 2013.11.27
#endif
        onTuneTimerStop(); // added by Sergey 19.05.2013

        // modified by Dmitry 23.04.13
        if (!aEvent.Data().isEmpty() && !isAudioOff())
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            switch ( eventData->keyStatus )
            {
#ifndef DUAL_KEY
            case KEY_STATUS_CANCEL:   // modified by yongkyun.lee 2013-07-08 for : ITS 178517
#endif
            case KEY_STATUS_RELEASED:
                //{changed by junam 2013.09.07 for ITS_KOR_185529
                //if (!m_bSeekHardKeyPressed)
#ifndef DUAL_KEY
                if (!m_bPrevHardKeyPressed)
                {
                    emit resetPlaybackControl();//{ modified by yongkyun.lee 2013-10-17 for : ITS 196623
                    break;
                }
#endif
                if(currentEvent==EVT_KEY_RRC_SEEK_PREV)
                    m_EngineMain.sendVRCancelSignal();
                //setSeekHardKeyPressed(false);
                // added by Dmitry 11.10.13 for ITS0194940
                if (!m_AudioController.isLongPress())
                    emit closeUnsupportedPopup();
                setPrevHardKeyPressed(false);
                //}changed by junam
                //{ added by junam 2013.05.18 for ISV_KR81848
                if( m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2 )
                {
                    emit UpdateStateTrackOSD(false);
                }
                //}added by junam
                if (!m_AudioController.cancelFFRW())
                {
                    m_AudioController.setFFRW(false , AppEngineQMLConstants::KEY_STATUS_RELEASED ,AppEngineQMLConstants::JOG_LEFT );
                    if ( !m_AudioController.isLongPress() )
                    {
                        emit seekPrevHardkeyPressed();
                    }
                }
                m_AudioController.setIsLongPress(0); // modified by Michael.Kim 2013.05.10 for ITS Issue for 0167251
                setRewReachedFirst(false); // added by Sergey 28.05.2013
                break;

            case KEY_STATUS_PRESSED_LONG:
                //{added by junam 2013.07.09 for ITS178979
#ifndef DUAL_KEY
                if (!m_bPrevHardKeyPressed ) //changed by junam 2013.09.07 for ITS_KOR_185529
                {
                    MP_LOG<<"Long press event without press event will be ignored" << LOG_ENDL;
                    break;
                }
#endif
                //}added by junam
                m_AudioController.setIsLongPress(-1); // added by kihyung 2013.2.5 for ISV 72327
                // removed by Dmitry 28.05.13
                m_AudioController.setFFRW(true ,AppEngineQMLConstants::KEY_STATUS_LONG_PRESSED  ,AppEngineQMLConstants::JOG_LEFT ); // added by yongkyun.lee 20130225 for : ISV 73823
                if( m_AudioController.GetMediaMode() != MP::BLUETOOTH )		//added by hyochang.ryu 20130628
                    m_EngineMain.beep(eBeepType1);
                break;

            case KEY_STATUS_PRESSED_CRITICAL:
                //{ added by yongkyun.lee 20130531 for :  ITS 170510
                // if (!(m_AudioController.GetPlayerState() == PLAYER_REWIND_X4 || m_AudioController.GetPlayerState() == PLAYER_REWIND ) ||
                //     !m_bSeekHardKeyPressed )//|| m_AudioController.GetListViewModel()->getPopType()>= 0)  modified by edo.lee 2013.05.29
                if (!(m_AudioController.GetPlayerState() == PLAYER_REWIND_X4 ||
                      m_AudioController.GetPlayerState() == PLAYER_REWIND    ||
                      m_AudioController.GetPlayerState() == PLAYER_SEARCH      )
        #ifndef DUAL_KEY
                        ||! m_bPrevHardKeyPressed
        #endif
                        )//changed by junam 2013.09.07 for ITS_KOR_185529
                    //} added by yongkyun.lee 20130531
                    break;
                m_AudioController.setIsLongPress(-1); // added by kihyung 2013.2.5 for ISV 72327
                if( m_AudioController.GetMediaMode() != MP::BLUETOOTH )		//added by hyochang.ryu 20130628
                    m_EngineMain.beep(eBeepType1);
                m_AudioController.setFFRW(true ,AppEngineQMLConstants::KEY_STATUS_CRITICAL_PRESSED ,AppEngineQMLConstants::JOG_LEFT  );
                break;

            case KEY_STATUS_PRESSED:
                if(processedInAUXMode(aEvent)) break;	//added by hyochang.ryu 20131009 for ITS194476
                //added by suilyou 20130910 for ITS 0186508 START
#ifndef DUAL_KEY
                if(getNextHardKeyPressed())
                    break;
#endif
                //added by suilyou 20130910 for ITS 0186508 END
                //[KR][ITS][183091][comment](aettie.ji) 20130809
                emit menuOptionClose();
#ifndef DUAL_KEY
                setPrevHardKeyPressed(true);//changed by junam 2013.09.07 for ITS_KOR_185529
#endif
                m_AudioController.setFFRW(false ,AppEngineQMLConstants::KEY_STATUS_PRESSED ,AppEngineQMLConstants::JOG_LEFT  );
                break;
#ifdef DUAL_KEY
            case KEY_STATUS_CANCEL:
                if (!m_AudioController.isLongPress())
                    emit closeUnsupportedPopup();

                m_AudioController.setIsLongPress(0); //added by junam 2014.01.10 for ITS_ME_218697

                if( m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2 )
                {
                    emit UpdateStateTrackOSD(false);
                }

                if (!m_AudioController.cancelFFRW())
                {
                    m_AudioController.setFFRW(false , AppEngineQMLConstants::KEY_STATUS_CANCELED ,AppEngineQMLConstants::JOG_LEFT );
                }
                break;
#endif
            default:
                break;
            }
        }
        else {
            MP_LOG << "aEvent.Data().isEmpty(): " << aEvent.Data().isEmpty() << "m_bAudioOff: " << m_bAudioOff << LOG_ENDL;
        }
        // modified by Dmitry 23.04.13
        result = true;
        break;

    case EVT_KEY_HU_BACK:
    case EVT_KEY_CCP_BACK:
    case EVT_KEY_RRC_BACK:
    {
        MP_LOG << "EVT_KEY_HU_BACK" << LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();

            if ( eventData->keyStatus == KEY_STATUS_PRESSED )
            {
                //m_isFrontBack = display == DISPLAY_FRONT ? true : false; remove by edo.lee 2013.03.21
                emit backKeyPressed();
            }
        }
        result = true;
        break;
    }

    case EVT_KEY_HU_MENU:
    case EVT_KEY_CCP_MENU:
    case EVT_KEY_RRC_MENU:
    {
        MP_LOG << "EVT_KEY_HU_MENU" << LOG_ENDL;
        if ( !aEvent.Data().isEmpty() )
        {
            KEY_EVENT_DATA_T *eventData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            //{ added by aettie 2013.08.01 for ITS 181900
            if ( eventData->keyStatus == KEY_STATUS_PRESSED)
            {
                // { modified by wspark 2012.08.02 for CR12020
                //emit menuKeyPressed();
                emit menuKeyPressed(true);
                // } modified by wspark
            }
            else if ( eventData->keyStatus == KEY_STATUS_RELEASED )
            {
                emit scrollAccStop(); // added by lssanh 2013.02.26 ISV73837
                m_AudioController.cancelFFRW(); // modified by Dmitry 23.04.13
            }
            //} added by aettie 2013.08.01 for ITS 181900
        }
        result = true;
        break;
    }

    case EVT_KEY_SOFT_MENU:
    {
        MP_LOG << "EVT_KEY_SOFT_MENU" << LOG_ENDL;
        // { modified by wspark 2012.08.02 for CR12020
        //emit menuKeyPressed();
        emit menuKeyPressed(false);
        emit scrollAccStop(); // added by lssanh 2013.02.26 ISV73837
        m_AudioController.cancelFFRW(); // modified by Dmitry 23.04.13
        // } modified by wspark
        result = true;
        break;
    }
        //{ added by yongkyun.lee 20130322 for : NO CR .VR Auto Play
    case EVT_VR_NOTIFY:
    {
        //APP_ENTRY_POINT_DATA_T *pEventData = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
        //remove by edo.lee 2013.07.15 STATIC TEST
        if( m_AudioController.isPaused())
        {
            //deleted by yongkyun.lee 2013-11-25 for :  ITS 210541 rollback
            m_AudioController.Play();
        }
        //removed by edo.lee 2013.04.22
        //DisplayOSD(true);
        //m_EngineMain.GetNotifier()->displayOSD(OSD_EVENT_AUDIO_TRACK_INFO);
        break;
    }
        //} added by yongkyun.lee 20130322

    case EVT_VR_COMMAND:
    {
        MP_MEDIUM << "EVT_VR_COMMAND" << LOG_ENDL;

        if ( !aEvent.Data().isEmpty() )
        {
            EVENT_EVT_VR_COMMAND_DATA_T *eventData = (EVENT_EVT_VR_COMMAND_DATA_T*)aEvent.Data().data();

            MP_LOG << "EVT_VR_COMMAND: commandId: " << eventData->commandId << LOG_ENDL;
            MP_LOG << "EVT_VR_COMMAND: eventData->sVRPayload.data: " << (int)eventData->sVRPayload.data[0] << LOG_ENDL;

            switch ( eventData->commandId )
            {
            case AV_MODE_CHANGE:
            {
                DisplayOSD(true); //added by edo.lee 2013.01.14
                MP::DEVICE_T id = MP::JUKEBOX;
                switch(eventData->sVRPayload.data[0])
                {
                case AV_MODE_JUKEBOX_MUSIC:
                    id = MP::JUKEBOX;
                    break;
                case AV_MODE_USB_MUSIC:
                    if (m_EngineMain.GetMediaManager()->IsSourceAvailable(MEDIA_SOURCE_USB1, MEDIA_TYPE_MUSIC))
                    {
                        id = MP::USB1;
                    }
                    else if (m_EngineMain.GetMediaManager()->IsSourceAvailable(MEDIA_SOURCE_USB2, MEDIA_TYPE_MUSIC))
                    {
                        id = MP::USB2;
                    }
                    break;
                case AV_MODE_CD_MP3:
                case AV_MODE_CD_AUDIO:
                    id = MP::DISC;
                    break;
                case AV_MODE_IPOD_MUSIC:
                    if (m_EngineMain.GetMediaManager()->IsSourceAvailable(MEDIA_SOURCE_IPOD2, MEDIA_TYPE_MUSIC))
                    {
                        id = MP::IPOD2;
                    }
                    else /*if (m_EngineMain.GetMediaManager()->IsSourceAvailable(MEDIA_SOURCE_IPOD1, MEDIA_TYPE_MUSIC))*/
                    {
                        id = MP::IPOD1;
                    }
                    break;
                case AV_MODE_BT_AUDIO:
                    id = MP::BLUETOOTH;
                    break;
                case AV_MODE_AUX:
                    id = MP::AUX;
                    break;
                default:
                    id = MP::JUKEBOX;
                    break;
                }
                m_AudioController.SetMediaMode(id);
            }
                break;

            case AV_MODE_CONTROL_PLAY:
            {
                if (eventData->sVRPayload.data[0] == 0)
                {
                    // { modified by kihyung 2013.3.14 for ISV 69006
                    if(m_AudioController.getScanMode() == SCANOFF) {
                        emit pauseCommandFromVR();
                    }
                    else {
                        m_AudioController.setScanMode(SCANOFF);
                    }
                    // } modified by kihyung 2013.3.14 for ISV 69006
                }
                else
                {
                    emit playCommandFromVR();
                }
            }
                break;

            case AV_MODE_CONTROL_NEXT_MEDIA:
            {
                DisplayOSD(true); //added by edo.lee 2013.01.14
                ( eventData->sVRPayload.data[0] == 0 ) ? emit nextCommandFromVR() : emit prevCommandFromVR();
            }
                break;

                // { changed by eugeny - 12-09-15
            case AV_MODE_CONTROL_REPEAT:
            {
                // { added by kihyung 2013.4.4 for ISV 76793
                if(m_AudioController.getPlayState() != PLAYER_PLAY) {
                    m_AudioController.Play();
                }
                // } added by kihyung 2013.4.4 for ISV 76793
                
                if ((int)eventData->sVRPayload.data[0] == 0)
                {
                    m_AudioController.setRepeatRandomMode(REPEATALL, -1);
                }
                else
                {
                    m_AudioController.setRepeatRandomMode(REPEATFILE, -1);
                }

                // { added by Sergey 20.04.2013 for ISV#79535
                if(!isForeground())
                {
                    DisplayOSD(true);
                    emit updateOSD(OSD_EVENT_AUDIO_TRACK_INFO);
                }
                // } added by Sergey 20.04.2013 for ISV#79535
            }
                break;

            case AV_MODE_CONTROL_RANDOM:
            case AV_MODE_CONTROL_ALBUM_RANDOM:
            {
                // { added by kihyung 2013.4.4 for ISV 76793
                if(m_AudioController.getPlayState() != PLAYER_PLAY) {
                    m_AudioController.Play();
                }
                // } added by kihyung 2013.4.4 for ISV 76793
                
                if ((int)eventData->sVRPayload.data[0] == 0)
                {
                    m_AudioController.setRepeatRandomMode(-1, RANDOMOFF);
                }
                else
                {
                    m_AudioController.setRepeatRandomMode(-1, RANDOMFILE);
                }

                // { added by Sergey 20.04.2013 for ISV#79535
                if(!isForeground())
                {
                    DisplayOSD(true);
                    emit updateOSD(OSD_EVENT_AUDIO_TRACK_INFO);
                }
                // } added by Sergey 20.04.2013 for ISV#79535
            }
                break;
                // } changed by eugeny - 12-09-15

            case AV_MODE_CONTROL_AUDIO_CD_TRACK:
            {
                m_AudioController.setCurrentIndex((int)eventData->sVRPayload.data[0]);
            }
                break;

            default:
                break;
            }
        }
    }
        break;
#ifndef DUAL_KEY
    case EVT_KEY_CCP_JOG_DIAL_CENTER:
    case EVT_KEY_RRC_JOG_DIAL_CENTER:
        //{ modified by yongkyun.lee 2013-08-15 for :  ITS 183677
        if(m_AudioController.GetFfRewState())
            break;
        //} modified by yongkyun.lee 2013-08-09
#endif
    case EVT_KEY_CCP_JOG_DIAL_ENCODER:
    case EVT_KEY_RRC_JOG_DIAL_ENCODER:
    {
        MP_LOG << "DUAL_KEY JOG forwarding to QML through signalJogNavigation" << LOG_ENDL;
//        KEY_EVENT_DATA_T *ed = (KEY_EVENT_DATA_T*)aEvent.Data().data();
//        emit signalJogNavigation(signalTransformEVT2QML_KEY(currentEvent),
//                                  (ed->keyStatus)+1001, display==DISPLAY_FRONT?false:true);
        if( (int)aEvent.GetCommand() > 0 )
        {
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                      AppEngineQMLConstants::KEY_STATUS_PRESSED, aEvent.GetEventId()==EVT_KEY_CCP_JOG_DIAL_ENCODER?false:true, (int)aEvent.GetCommand());
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                      AppEngineQMLConstants::KEY_STATUS_RELEASED, aEvent.GetEventId()==EVT_KEY_CCP_JOG_DIAL_ENCODER?false:true, (int)aEvent.GetCommand());
        }
        else
        {
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                      AppEngineQMLConstants::KEY_STATUS_PRESSED, aEvent.GetEventId()==EVT_KEY_CCP_JOG_DIAL_ENCODER?false:true, abs((int)aEvent.GetCommand()));
            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                      AppEngineQMLConstants::KEY_STATUS_RELEASED, aEvent.GetEventId()==EVT_KEY_CCP_JOG_DIAL_ENCODER?false:true, abs((int)aEvent.GetCommand()));
        }
        MP_LOG << "m_AudioController.isLongPress() "<< m_AudioController.isLongPress() << " m_focusPlaybackControl " << m_focusPlaybackControl << LOG_ENDL;
        //if(processedInBTMode(aEvent)) break;	//moved tunehandler 20130828 //deleted 20130710 //added by hyochang.ryu 20130704
#ifndef DUAL_KEY
        if( m_AudioController.isLongPress() == -1 || m_AudioController.isLongPress() ==1 ) // modified by wonseok.heo for ITS 192411 2013.10.09
            return result; //added by HWS 2013.05.02
#endif
        if( m_focusPlaybackControl ) // modified by Sergey 17.05.2013
        {
            if(m_AudioController.isControllerDisable(MP::CTRL_DISABLE_TUNE, true)) //added junam 2013.08.27 for disable tune // modified for ITS 196266, 217989
                break;

            tuneHandler( aEvent.GetEventId(), aEvent );
        }
        else //added by edo.lee 2013.03.30
        {
            if(m_isBackPressByJog)
            {
                if(aEvent.GetEventId() == EVT_KEY_CCP_JOG_DIAL_CENTER )
                    m_isFrontSoftBackPressed = true;
                else if(aEvent.GetEventId() == EVT_KEY_RRC_JOG_DIAL_CENTER )
                    m_isRearSoftBackPressed = true;
            }
        }//added by edo.lee 2013.03.30
        break;
    }

    case EVT_KEY_HU_JOG_DIAL_ENCODER:
    {
        MP_LOG << "EVT_KEY_HU_JOG_DIAL_ENCODER" << LOG_ENDL;
        if(processedInBTMode(aEvent)) break;	//added by hyochang.ryu 20130704

        if(m_AudioController.isControllerDisable(MP::CTRL_DISABLE_TUNE)) //added junam 2013.08.27 for disable tune
            break;

        DISPLAY_T disp = m_EngineMain.getFrontScreenView();
        MP_LOG << "TUNE GetActiveMode = "<<m_EngineMain.GetNotifier()->GetActiveMode() << "isBasicView = " << m_AudioController.isBasicView() << "getCoverFlowEnable()" <<m_AudioController.getCoverFlowEnable() << LOG_ENDL;
        MP_LOG << "getFrontScreenView =" << disp << "getAVPEntryPoint = " <<m_EngineMain.GetAVPEntryPoint(disp) << LOG_ENDL;
        if(m_AudioController.isForeground()
                &&!m_AudioController.isBasicView()
                &&m_AudioController.getCoverFlowEnable()
                &&!m_AudioController.isListView()   //added by honggi.shin, 2013.11.06, for avoid to meet conditions in ListView
                &&(m_EngineMain.getCloneState()!=CLONE_OFF||(m_EngineMain.GetAVPEntryPoint(disp)==eAPP_AVP_AUDIO_MODE_JUKEBOX
                   ||m_EngineMain.GetAVPEntryPoint(disp)==eAPP_AVP_AUDIO_MODE_USB1
                   ||m_EngineMain.GetAVPEntryPoint(disp)==eAPP_AVP_AUDIO_MODE_USB2)))
        {
            if(m_EngineMain.getCloneState()==CLONE_ON_REAR)
            {
                MP_MEDIUM << " CLONE_ON_REAR" << LOG_ENDL;
                if( (int)aEvent.GetCommand() > 0 )
                {
                    MP_MEDIUM << " RIGHT" << LOG_ENDL;
                    emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                              AppEngineQMLConstants::KEY_STATUS_PRESSED, true, (int)aEvent.GetCommand());
                    emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                              AppEngineQMLConstants::KEY_STATUS_RELEASED, true, (int)aEvent.GetCommand());
                }
                else
                {
                    MP_MEDIUM << " LEFT" << LOG_ENDL;
                    emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                             AppEngineQMLConstants::KEY_STATUS_PRESSED, true, abs((int)aEvent.GetCommand()));
                    emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                             AppEngineQMLConstants::KEY_STATUS_RELEASED, true, abs((int)aEvent.GetCommand()));
                }
                return true;
            }
            else
            {
                MP_MEDIUM << " NOT CLONE" << LOG_ENDL;
                if( (int)aEvent.GetCommand() > 0 )
                {
                    MP_MEDIUM << " RIGHT" << LOG_ENDL;
                    emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                              AppEngineQMLConstants::KEY_STATUS_PRESSED, false, (int)aEvent.GetCommand());
                    emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                              AppEngineQMLConstants::KEY_STATUS_RELEASED, false, (int)aEvent.GetCommand());
                }
                else
                {
                    MP_MEDIUM << " LEFT" << LOG_ENDL;
                    emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                             AppEngineQMLConstants::KEY_STATUS_PRESSED, false, abs((int)aEvent.GetCommand()));
                    emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                             AppEngineQMLConstants::KEY_STATUS_RELEASED, false, abs((int)aEvent.GetCommand()));
                }
                return true;
            }
        }

    }
    case EVT_KEY_HU_TUNE_PRESSED:
    {
        MP_LOG << "EVT_KEY_HU_TUNE_PRESSED" << LOG_ENDL;

#ifndef DUAL_KEY    // { modified by sangmin.seol 2014.03.11 ITS 0225385 H/U Tune Key Cancel Fix
        if( m_AudioController.isLongPress() == -1 || m_AudioController.isLongPress() ==1 ) // modified by wonseok.heo for ITS 192411 2013.10.09
            return result; //added by HWS 2013.05.02
#endif

        //{ Added by Radhakrushna Mohanty 2012.08.18 CR11181
        if(processedInAUXMode(aEvent) || processedInDiscMode()) break; // added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
        //} Added by Radhakrushna Mohanty 2012.08.18 CR11181
        //{removed by junam 2013.04.13 for AutoStart
        //{ added by eunhye 2012.8.31 for CR13047
        //        if(processedIpodIndexing(aEvent.GetEventId()) == true) {
        //            MP_LOG << "processedIpodIndexing() return true ---> break" << LOG_ENDL;
        //            break;
        //        }
        //} added by eunhye
        //}removed by junam

        //added bv suilyou 20131023 for tune navigation in coverflow START



        DISPLAY_T disp = m_EngineMain.getFrontScreenView();
        MP_LOG << "TUNE GetActiveMode = "<<m_EngineMain.GetNotifier()->GetActiveMode() << "isBasicView = " << m_AudioController.isBasicView() << "getCoverFlowEnable()" <<m_AudioController.getCoverFlowEnable() << LOG_ENDL;
        MP_LOG << "getFrontScreenView =" << disp << LOG_ENDL;
        if(m_AudioController.isForeground()
                &&!m_AudioController.isBasicView()
                &&m_AudioController.getCoverFlowEnable()
                &&!m_AudioController.isListView()   //added by honggi.shin, 2013.11.06, for avoid to meet conditions in ListView
                &&(m_EngineMain.getCloneState()!=CLONE_OFF||(m_EngineMain.GetAVPEntryPoint(disp)==eAPP_AVP_AUDIO_MODE_JUKEBOX
                   ||m_EngineMain.GetAVPEntryPoint(disp)==eAPP_AVP_AUDIO_MODE_USB1
                   ||m_EngineMain.GetAVPEntryPoint(disp)==eAPP_AVP_AUDIO_MODE_USB2)))
        {
            KEY_EVENT_DATA_T *pData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            if(m_EngineMain.getCloneState()==CLONE_ON_REAR)
            {
                if(pData->keyStatus==KEY_STATUS_PRESSED)
                {
                    MP_MEDIUM << "KEY_STATUS_PRESSED CLONE_ON_REAR" << LOG_ENDL;
                    emit signalJogNavigation(AppEngineQMLConstants::JOG_CENTER,
                                              AppEngineQMLConstants::KEY_STATUS_PRESSED, true, 1);
                    return true;
                }
                else if(pData->keyStatus==KEY_STATUS_RELEASED)
                {
                    MP_MEDIUM << "KEY_STATUS_RELEASED CLONE_ON_REAR" << LOG_ENDL;
                    emit signalJogNavigation(AppEngineQMLConstants::JOG_CENTER,
                                              AppEngineQMLConstants::KEY_STATUS_RELEASED, true, 1);
                    return true;
                }
            }
            else
            {
                    if(pData->keyStatus==KEY_STATUS_PRESSED)
                    {
                        MP_MEDIUM << "KEY_STATUS_PRESSED " << LOG_ENDL;
                        emit signalJogNavigation(AppEngineQMLConstants::JOG_CENTER,
                                                  AppEngineQMLConstants::KEY_STATUS_PRESSED, false, 1);
                        return true;
                    }
                    else if(pData->keyStatus==KEY_STATUS_RELEASED)
                    {
                        MP_MEDIUM << "KEY_STATUS_RELEASED " << LOG_ENDL;
                        emit signalJogNavigation(AppEngineQMLConstants::JOG_CENTER,
                                                  AppEngineQMLConstants::KEY_STATUS_RELEASED, false, 1);
                        return true;
                    }
             }
        }
        //added bv suilyou 20131023 for tune navigation in coverflow END

        tuneHandler( aEvent.GetEventId(), aEvent );

        break;
    }

    case EVT_TA_DAB_START: // added by kihyung 2013.08.01 for ITS 0174593
    case EVT_TA_RDS_START: // added by kihyung 2013.08.01 for ITS 0174593
    case EVT_CALL_START:
    {
        MP_HIGH << "[Audio] EVT_CALL_START" << LOG_ENDL;
        //deleted by yongkyun.lee 2013-10-11 for : ITS 191526
        //added by sh.kim 2013.08.09 for ITS 183042
        if(m_AudioController.getScanMode() != SCANOFF){
            MP_LOG << " : current Scan Mode At Bt Call :: " << m_AudioController.getScanMode() << LOG_ENDL;
            m_BtCallAfterScan = false;
            m_AudioController.setScanMode(SCANOFF);
        }
        //added by sh.kim 2013.08.09 for ITS 183042
        //{added by junam 2013.07.06 for ITS178188


        if(m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2)
        {
            m_AudioController.clearPlayMode();

            //Added by Tan.2013.11.12. When call started need to cancel FFRW and set long press to false.
            m_AudioController.cancelFFRW(false);
            m_AudioController.setIsLongPress(false);
            // { Added by jonghwan.cho@lge.com 2013.11.05.
            if( currentEvent == EVT_CALL_START )
            {
                switch( m_AudioController.GetMediaMode() )
                {
                    case MP::IPOD1 :
                        m_AudioController.GetControllers().GetIPod1Controller()->SetSystemState(SYSTEM_BT_CALL_START);
                        break;
                    case MP::IPOD2 :
                        m_AudioController.GetControllers().GetIPod2Controller()->SetSystemState(SYSTEM_BT_CALL_START);
                        break;
                    default :
                        break;
                }
                emit setDraggable(false);  // added ITS 210706,7 only EVT_CALL for 227432
            }
            // } Added by jonghwan.cho@lge.com 2013.11.05.

        }
        //}added by junam

        // { added by wonseok.heo for  ITS 178767 2013.07.10
        if(m_EngineMain.isPowerOff() == true || m_EngineMain.getOnBoot()){ //added by edo.lee 2013.09.23 ITS 0189921 
            m_bPlayAfterBTCall = true;
            m_bPlayAfterBTCalliPod = true; // ITS 235835
            MP_HIGH<<"[Audio] EVT_CALL_START - isPoweroff "<<m_EngineMain.isPowerOff() << LOG_ENDL;
			if(m_EngineMain.isPowerOff() == true)
                 setMaintainPause(true); 
        }
        //{ modified by yongkyun.lee 2013-08-27 for : ITS 186746
        else if(m_AudioController.GetPlayerState() >= PLAYER_FASTFORWARD_X4 ||
                m_AudioController.GetPlayerState() == PLAYER_FASTFORWARD ||
                m_AudioController.GetPlayerState() == PLAYER_REWIND )
		{
            if(m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2 )
            {
                //cancel FF RW moved. ITS 241378 next song cancel
                m_AudioController.Pause();//remove by edo.lee 2013.09.25 <<-- Tan 2013.11.01 Ipod need this to pause during call.
                m_bPlayAfterBTCalliPod = true;
            }
			m_bPlayAfterBTCall = true;
		}
        //{ added by hyochang.ryu 21030815 for ITS184433
        else if(m_AudioController.GetMediaMode() == MP::BLUETOOTH)
        {
            m_bPlayAfterBTCall = false;
            m_bPlayAfterBTCalliPod = true; // ITS 235835
            //m_bPlayAfterBTCall = m_AudioController.IsPlaying() ? true : false;	//false;	//modified by hyochang.ryu 20131002
            if(m_AudioController.IsPlaying())
            {
                //{ added by hyochang.ryu 20131017 for BL call
                if(m_EngineMain.getisBLCall() || aEvent.GetEventId() != EVT_CALL_START) // modified by oseong.kwon 2014.10.06 for 249683
                {
                    m_bPlayAfterBTCall = true;	
                    m_AudioController.Pause();
                }
                //else
                //} added by hyochang.ryu 20131017 for BL call
                //m_AudioController.commandFromBT(EVT_BT_MUSIC_STREAMING_PAUSED);
            }
        }
        //} added by hyochang.ryu 21030815 for ITS184433
        else
        {
            if ((m_AudioController.getScanMode() != SCANOFF) || m_AudioController.IsPlaying() || m_EngineMain.getIsTermOnTrackChange())//modified by edo.lee 2013.09.24 ITS  0190407//deleted by hyochang.ryu 20130713	//&& m_AudioController.GetMediaMode() != MP::BLUETOOTH)//modified by wonseok.heo for ISV 86878 86525 86524 2013.07.12 modified by edo.lee 2013.03.07
            {
                MP_LOG<< "[Audio] Pause!!" << LOG_ENDL;
                m_AudioController.Pause();
                m_bPlayAfterBTCall = true; //added by edo.lee 2013.06.20
                m_bPlayAfterBTCalliPod = true; // ITS 235835
                m_EngineMain.setPlayAfterBTCallState(true);//Added by taihyun.ahn 2013.10.15
            }
            else
            {
                if ((m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2 ) && m_AudioController.isPaused())
                {
                    m_bPlayAfterBTCalliPod = false; // ITS 235835
                }
                else
                {
                    m_bPlayAfterBTCalliPod = true; // ITS 235835
                }
            }
        }
        break;
    }
    case EVT_POWER_STATE_NORMAL_ON:
    {
        MP_LOG << "[Audio] EVT_POWER_STATE_NORMAL_ON "  << LOG_ENDL;
        //setMaintainPause(false);              // removed by sangmin.seol For ITS 0232871 maintain pause state when BT Call started in power off state
		break;
    }
    case EVT_TA_DAB_END: // added by kihyung 2013.08.01 for ITS 0174593
    case EVT_TA_RDS_STOP: // added by kihyung 2013.08.01 for ITS 0174593
    case EVT_CALL_END:
    {
        MP_HIGH << "[Audio] EVT_CALL_END "  << LOG_ENDL;
        //deleted by yongkyun.lee 2013-10-11 for : ITS 191526
        m_BtCallAfterScan = false; //added by sh.kim 2013.08.09 for ITS 183042

        //{ modified by yongkyun.lee 2013-10-10 for : ITS 193552
        if(m_AudioController.GetMediaMode() == MP::BLUETOOTH)
        {
            m_AudioController.closeBTCallPopup();
            //{ added by hyochang.ryu 20131017 for BL call
            if(m_EngineMain.getisBLCall() || aEvent.GetEventId() != EVT_CALL_END) // modified by oseong.kwon 2014.10.06 for 249683
            {
                m_EngineMain.setisBLCall(false);
                if(m_bPlayAfterBTCall && m_EngineMain.getPlayOnBTCall()) // modified by oseong.kwon 2014.11.18 for ITS 252671
                    m_AudioController.Play();
            }
            //} added by hyochang.ryu 20131017 for BL call
            // { added by cychoi 2014.03.05 for ITS 228131, ITS 228134 Request BT Music Info after BT call
            else
            {
                if(m_bBTMusicInfoAfterBTCall == true)
                {
                    m_AudioController.onBtRequestTimerStart();
                    m_bBTMusicInfoAfterBTCall = false;
                }
            }
            // } added by cychoi 2014.03.05
        }
        //} modified by yongkyun.lee 2013-10-10 
        // { added by cychoi 2014.03.05 for ITS 228131, ITS 228134 Request BT Music Info after BT call
        else
        {
            if(m_EngineMain.getisBLCall())
            {
                m_EngineMain.setisBLCall(false);
            }
        }
        // } added by cychoi 2014.03.05

        //{changed by junam 2013.08.16 for ITS_KOR_184332
        if(m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2)
        {
            m_AudioController.closeBTCallPopup();  // added for ITS 206297

            // { Added by jonghwan.cho@lge.com 2013.11.05.
            if( currentEvent == EVT_CALL_END )
            {
                switch( m_AudioController.GetMediaMode() )
                {
                    case MP::IPOD1 :
                        m_AudioController.GetControllers().GetIPod1Controller()->SetSystemState(SYSTEM_BT_CALL_END);
                        break;
                    case MP::IPOD2 :
                        m_AudioController.GetControllers().GetIPod2Controller()->SetSystemState(SYSTEM_BT_CALL_END);
                        break;
                    default :
                        break;
                }
                emit setDraggable(true); // added ITS 210706,7 only EVT_CALL for 227432
            }
            // } Added by jonghwan.cho@lge.com 2013.11.05.

            if(m_AudioController.IsPlaying() == false && m_bPlayAfterBTCalliPod == true) // modified for ITS 222964
            {
                m_EngineMain.setIsTermOnTrackChange(true);
                if(currentEvent == EVT_CALL_END)//2014.12.04 Tan: only allow this sleep for event call end.
                {
                    usleep(2000*1000); //when call is ended, iPod is not ready to accept play, wait a bit.
                }
                m_AudioController.Play();
            }
            else
            {
                MP_HIGH<<"Do not autoplay: m_bPlayAfterBTCalliPod"<<m_bPlayAfterBTCalliPod << LOG_ENDL;
                //MP_HIGH<<"Do not autoplay: m_AudioController.IsPlaying()"<<m_AudioController.IsPlaying() << LOG_ENDL;
            }

        }
        // modified by yongkyun.lee 2013-10-10 for : ITS 193552
        else if (m_AudioController.IsPlaying() == false && m_AudioController.GetMediaMode() != MP::BLUETOOTH && isMaintainPause()==false)	//modified by hyochang.ryu 201310105 for ITS 184360	//modified by edo.lee 2013.03.07
        {
            MP_HIGH <<m_bPlayAfterBTCall <<m_EngineMain.getPlayOnBTCall() << LOG_ENDL;
            // modified by yongkyun.lee 2013-10-10 for : ITS 193552
            if(m_bPlayAfterBTCall && m_EngineMain.getPlayOnBTCall())	//deleted by hyochang.ryu 201310105 for ITS 184360	//modify by edo.lee 2013.09.05
            {
                m_EngineMain.setIsTermOnTrackChange(true);
                m_AudioController.Play();
                //{ deleted by hyochang.ryu 20131015 for ITS 184360
                //if(m_AudioController.GetMediaMode() == MP::BLUETOOTH)
                //    m_AudioController.commandFromBT(EVT_BT_MUSIC_STREAMING_STARTED);
                //} deleted by hyochang.ryu 20131015 for ITS 184360
            }
        }
        //}changed by junam
        m_EngineMain.setPlayAfterBTCallState(false);//Added by taihyun.ahn 2013.10.15
        m_bPlayAfterBTCall = false;//added by sh.kim 2013.08.09 for ITS 183042
        m_bPlayAfterBTCalliPod = false; // ITS 235835
        setMaintainPause(false); 
        //Removed by Puneet on 2013.07.11 for ITS #0179376 , Moved to Engine Main.
        //       m_EngineMain.setPlayOnBTCall(); //added by edo.lee 2013.06.27 for BT CALL
        break;
    }

    case EVT_BT_MUSIC_ENABLED:
    {
        //{ modified by yongkyun.lee 2013-10-16 for : ITS 195511
        if(getBTSetInfo())
        {
            if ( !aEvent.Data().isEmpty() )
            {
                QString deviceName = "";
                QString iPodDeviceName = "";
                if(sizeof(BTMUSIC_ENABLE_INFO) == aEvent.Data().size())
                {
                    BTMUSIC_ENABLE_INFO *param = (BTMUSIC_ENABLE_INFO*)(aEvent.Data().data());
                    deviceName = QString::fromUtf8(param->cstrDeviceName);
                    m_isMetaInfo = param->bIsMetaInfo;
                    m_AudioController.SetBTAvrcpAbove13(m_isMetaInfo);
                    m_isRemoteCtrl = true;
                }
                else
                {
                    deviceName = QString::fromUtf8( aEvent.Data().data(), -1 );
                }
                m_btDeviceName  = m_AudioController.encodeString(deviceName) ;
                emit updateBTDeviceName(m_btDeviceName, m_isMetaInfo, m_isRemoteCtrl); //{ modified by yongkyun.lee 2014-01-10 for : ITS 218980
            }
            MP_HIGH <<"EVT_BT_MUSIC_ENABLED=device Name 1:"<< m_btDeviceName  << LOG_ENDL;
            break;
        }
        //} modified by yongkyun.lee 2013-10-16 
        // Don't auto play , when bt phone connected to HU ,
        // Play should start only when user launches BT music.
        m_EngineMain.GetMediaManager()->SetSourceAvailable(MEDIA_SOURCE_BLUETOOTH, MEDIA_TYPE_MUSIC, true );

        //m_BtTrackButton = 0; // commented by cychoi 2013.08.13 for unused code // added by hyochang.ryu 20130517 for BT OSD
        /* Temporary code to support backward compatibility while BTApp implementing its part.
        * Should get info from BTMUSIC_ENABLE_INFO only when BTApp will finish its part.
        */
        
        MP_MEDIUM<< "EVT_BT_MUSIC_ENABLED="  <<  " , "  << " , "<<  " , " << LOG_ENDL;
        m_AudioController.BTMusicInfoChanged( "", "", "",0); //rollback //deleted by hyochang.ryu no more //added by yongkyun.lee 2013.05.08
        if ( !aEvent.Data().isEmpty() )
        {
            QString deviceName = "";
            QString iPodDeviceName = ""; // added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT
            //bool isMetaInfo = true;
            //bool isRemoteCtrl = true; //added by edo.lee 2012.09.18 for New UX : Music (LGE) # 43

            if(sizeof(BTMUSIC_ENABLE_INFO) == aEvent.Data().size())
            {
                BTMUSIC_ENABLE_INFO *param = (BTMUSIC_ENABLE_INFO*)(aEvent.Data().data());
                deviceName = QString::fromUtf8(param->cstrDeviceName);
                m_isMetaInfo = param->bIsMetaInfo;
                m_AudioController.SetBTAvrcpAbove13(m_isMetaInfo);// added by yongkyun.lee 20130427 for : BT sync
                m_isRemoteCtrl = true;	//modified by hyochang.ryu 20130518 param->bIsRemoteCtrl;//added by edo.lee 2012.09.18 for New UX : Music (LGE) # 43
            }
            else
            {
                deviceName = QString::fromUtf8( aEvent.Data().data(), -1 );
            }
            m_btDeviceName  = m_AudioController.encodeString(deviceName) ;

            emit updateBTDeviceName(m_btDeviceName, m_isMetaInfo, m_isRemoteCtrl); //modifeid by edo.lee 2012.09.18 for New UX : Music (LGE) # 43
            // removed by cychoi 2013.08.13 for unused code
        }
        //emit activateBTTab(MP::BLUETOOTH,true,false); //{ modified by yongkyun.lee 2013-11-08 for : NOCR 
        setBTSetInfo(true) ;//{ modified by yongkyun.lee 2013-10-16 for : ITS 195511
        MP_HIGH <<"device Name 2:"<< m_btDeviceName  << LOG_ENDL;
        //m_AudioController.onBtRequestTimerStart();   //added by hyochang.ryu 20130622 for ITS162878	//moved to EVT_REQUEST_FG hyochang.ryu 20130529
        break;
    }

    case EVT_BT_MUSIC_DISABLED:
    {
        MP_LOG << "EVT_BT_MUSIC_DISABLED" << LOG_ENDL;
        m_AudioController.BTPluggedOUT();
        //m_AudioController.onBTFunc(false);		//deleted 20130906 for ISV 90755 //added by hyochang.ryu 20130601
        //m_EngineMain.GetMediaManager()->SetSourceAvailable(MEDIA_SOURCE_BLUETOOTH, MEDIA_TYPE_MUSIC, false );    //deleted by hyochang.ryu 20131003 for ITS193574
        MP_MEDIUM<< "EVT_BT_MUSIC_DISABLED="  <<  " , "  << " , "<<  " , " << LOG_ENDL;
        m_AudioController.BTMusicInfoChanged( "", "", "",0); //added by edo.lee 2013.03.22

        setBTSetInfo(false) ;	//hyochang.ryu 20130821 for P1#4
        
        //m_BtTrackButton = 0; // commented by cychoi 2013.08.13 for unused code // added by hyochang.ryu 20130517 for BT OSD
        // Main menu update:
        HomeScreenDef::MEDIA_SOURCE_T device = HomeScreenDef::MEDIA_SOURCE_BLUETOOTH;

        Event event(EVT_POST_EVENT, m_EngineMain.GetThisAppId(), APP_MAIN_MENU,
                    HomeScreenDef::DEVICE_DISCONNECTED,
                    QByteArray((char*)&device, sizeof(device))); // modified by lssanh 2013.02.21 IPC payload
        m_EngineMain.notifyUISH(event); // modified by Sergey for SwapDC
        m_AudioController.m_bSameDName = false; // added by jungae 2012.10.09, for CR14252, iPod reset when device name is same with BT
        //emit activateBTTab(MP::BLUETOOTH,false,false);  //{ modified by yongkyun.lee 2013-11-08 for : NOCR 
        break;
    }

    case EVT_FORMAT_JUKEBOX_START:
    {
        MP_LOG << "EVT_FORMAT_JUKEBOX_START" << LOG_ENDL;
        // modified by ravikanth 19.06.13 for ITS 0158940 & ITS 0173782
        // cancel copy if any before format
        CancelCopyJukebox();
        //m_AudioController.GetListViewModel()->cancelOperation();  //  removed by sangmin.seol 2013.11.12 for ITS-0207981 For copying popup close and Showing copy canceled popup
        CancelAudioCopy();      //  added by sangmin.seol 2013.11.12 for ITS-0207981 For copying popup close and Showing copy canceled popup
        m_AudioController.pauseMiner();
        m_AudioController.ResetDeviceInfo( MP::JUKEBOX );
        if ( m_mode == MP::JUKEBOX )
        {
            m_pGraceNote->CloseCollectionIDs();
            // m_AudioController.resetAudioMode(); // removed by kihyung 2012.11.03 for booting disable
            m_AudioController.Stop();
            m_AudioController.clearPlayMode();
        }
        m_AudioController.InitiateJukeBoxFormat();
        result = false;
        break;
    }

    case EVT_FORMAT_JUKEBOX_END:
    {
        MP_LOG << "EVT_FORMAT_JUKEBOX_END" << LOG_ENDL;
        m_AudioController.resumeMiner();
        result = false;
        break;
    }

        //{Commented by Alexey Edelev 2012.10.08
        //    // { added by junggil 2012.10.04 for CR13505 Media Apps - delta UIS 2.15 to 2.17
        //    case EVT_SETTINGS_CHANGED:
        //    {
        //        MP_LOG << "AppMediaPlayer_EngineAudio:HandleEvent(): EVT_SETTINGS_CHANGED" << LOG_ENDL;
        //
        //        if ( aEvent.Data().isEmpty() )
        //        {
        //            MP_LOG << "Settings interface: EVT_SETTINGS_CHANGED payload is empty"
        //                << "if you're seeing this log, make sure not to waste time but"
        //                << "notify AppSettings team." << LOG_ENDL;
        //            break;
        //        }
        //
        //        EVENT_SETTINGS_CHANGED_DATA_T *eventData = (EVENT_SETTINGS_CHANGED_DATA_T*)aEvent.Data().data();
        //        result = handleSettingsCommand( eventData );
        //        break;
        //    }
        //    // } added by junggil
        //}Commented by Alexey Edelev 2012.10.08

        // { modified by kihyung 2013.2.12
        /*
    // added by minho 20121114 for not sync status bar when turn on camera.
    case EVT_MOVE_VIDEO_EVENT:
    {
        MP_LOG << "EVT_MOVE_VIDEO_EVENT" << LOG_ENDL;

        m_EngineMain.setCamInFG(true);
        
        // Move video widget to opposite display.
        DISPLAY_T display = (DISPLAY_T)aEvent.GetCommand();
        m_AudioController.moveWidget((display == DISPLAY_FRONT) ? DISPLAY_REAR : DISPLAY_FRONT);
        m_AudioController.hideVideo();
        break;
    }
    
    case EVT_CAMERA_OFF_EVENT:
    {
        // { modified by kihyung
        m_EngineMain.setCamInFG(false);
        // m_bCamInFG = false;
        // m_AudioController.SetCamMode( m_bCamInFG );//{ added by yongkyun.lee 2012.12.19 for CR 14219 ,POPUP/list close
        // } modified by kihyung
        break;
    }
    */
        // { removed by kihyung 2013.2.14
        /*
    case EVT_CAMERA_ENABLE:
    {
        MP_LOG << "EVT_CAMERA_ENABLE" << LOG_ENDL;

        m_EngineMain.setCamInFG(true);
        
        // Move video widget to opposite display.
        //DISPLAY_T display = (DISPLAY_T)aEvent.GetCommand();
        //m_AudioController.moveWidget((display == DISPLAY_FRONT) ? DISPLAY_REAR : DISPLAY_FRONT);
        //m_AudioController.hideVideo();
        //m_EngineMain.setGstPipelineStatus(true);
        break;
    }

    case EVT_CAMERA_DISABLE:
    {
        MP_LOG << "EVT_CAMERA_DISABLE" << LOG_ENDL;

        m_EngineMain.setCamInFG(false);
        break;
    }
    */
        // } removed by kihyung
        // } modified by kihyung 2013.2.12

        // added by minho 20121114
        //{ added by yongkyun.lee 2012.12.19 for CR 14219 ,POPUP/list close
    case EVT_ACC_CHANGED:
    {
        EVENT_EVT_CAR_IF_STATUS_T *paccstatus = (EVENT_EVT_CAR_IF_STATUS_T *)aEvent.Data().data();
        if(paccstatus->status == 0)
        {
            MP_LOG << "ACC OFF" << LOG_ENDL;
            m_isAccOn = false;//emit accStatus(0);
            m_EngineMain.GetNotifier()->cancelFileOperation(); // added by lssanh 2013.02.07 2USB
            // modified by ravikanth 12-07-13 for ITS 0173814, 0178982
            if(m_AudioController.GetListViewModel()->isInCopyState())
                m_AudioController.GetListViewModel()->cancelFileOperation();
            emit menuOptionClose(); //added by junam 2013.08.12 for ITS_KOR_183027
        }
        else
        {
            MP_LOG << "ACC ON" << LOG_ENDL;
            m_isAccOn = true;//emit accStatus(1);
            //added by edo.lee 2013.06.08
            //DisplayOSD(true);
            //m_EngineMain.GetNotifier()->displayOSD(OSD_EVENT_AUDIO_TRACK_INFO);
         //   showCurrentOSD(); //removed by suilyou ITS 0210790
        }
        m_AudioController.SetAccMode( m_isAccOn );
        break;
    }

    case EVT_FRONT_LCD_ON:
    {
        MP_LOG << "F LCD ON" << LOG_ENDL;
        m_isFrontLCD = true;
        m_AudioController.SetFrontLcdMode( m_isFrontLCD );
        break;
    }

    case EVT_FRONT_LCD_OFF:
    {
        MP_LOG << "F LCD OFF" << LOG_ENDL;
        m_isFrontLCD = false;
        m_AudioController.SetFrontLcdMode( m_isFrontLCD );
        break;
    }

        //{added by junam 2013.09.10 for ITS_KOR_148300
    case EVT_NOTIFY_VR_STATUS:
    {
        MP_LOG<<"EVT_NOTIFY_VR_STATUS : "<<  aEvent.GetCommand() << LOG_ENDL;
        if(aEvent.GetCommand() == 1)
        {
            if(m_AudioController.getScanMode() != SCANOFF)
                m_AudioController.setScanMode(SCANOFF);
        }
        break;
    }
        //}added by junam
#ifdef DUAL_KEY
    case EVT_KEY_CCP_JOG_DIAL_LEFT:
    case EVT_KEY_CCP_JOG_DIAL_RIGHT:
    case EVT_KEY_CCP_JOG_DIAL_CENTER:
    case EVT_KEY_CCP_JOG_DIAL_UP:
    case EVT_KEY_CCP_JOG_DIAL_DOWN:
    {
        MP_LOG << "DUAL_KEY JOG forwarding to QML through signalJogNavigation" << LOG_ENDL;
        KEY_EVENT_DATA_T *ed = (KEY_EVENT_DATA_T*)aEvent.Data().data();
        emit signalJogNavigation(signalTransformEVT2QML_KEY(currentEvent),
                                  (ed->keyStatus)+1001, false, 1);
        break;
    }
    case EVT_KEY_RRC_JOG_DIAL_LEFT:
    case EVT_KEY_RRC_JOG_DIAL_RIGHT:
    case EVT_KEY_RRC_JOG_DIAL_CENTER:
    case EVT_KEY_RRC_JOG_DIAL_UP:
    case EVT_KEY_RRC_JOG_DIAL_DOWN:
    {
        MP_LOG << "DUAL_KEY JOG forwarding to QML through signalJogNavigation" << LOG_ENDL;
        KEY_EVENT_DATA_T *ed = (KEY_EVENT_DATA_T*)aEvent.Data().data();
        emit signalJogNavigation(signalTransformEVT2QML_KEY(currentEvent),
                                  (ed->keyStatus)+1001, true, 1);
        break;
    }
//    case EVT_KEY_CCP_JOG_DIAL_ENCODER:
//    {
//        if( (int)aEvent.GetCommand() > 0 )
//        {
//            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
//                                      AppEngineQMLConstants::KEY_STATUS_RELEASED, false); // modified by Dmitry 21.05.13
//        }
//        else
//        {
//            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
//                                      AppEngineQMLConstants::KEY_STATUS_RELEASED, false); // modified by Dmitry 21.05.13
//        }
//        break;
//    }
//    case EVT_KEY_RRC_JOG_DIAL_ENCODER:
//    {
//        if( (int)aEvent.GetCommand() > 0 )
//        {
//            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
//                                      AppEngineQMLConstants::KEY_STATUS_PRESSED, true); // modified by Dmitry 21.05.13
//        }
//        else
//        {
//            emit signalJogNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
//                                      AppEngineQMLConstants::KEY_STATUS_PRESSED, true); // modified by Dmitry 21.05.13
//        }
//        break;
//    }
#endif
    default:
        MP_LOG << "Unhandled event " << aEvent.GetEventId() << LOG_ENDL;
        result = false;
        break;
    }
    
    return result;
}

void
AppMediaPlayer_EngineAudio::SetMediaMode (MP::DEVICE_T eMode)
{
    m_AudioController.SetMediaMode( eMode );
}

int
AppMediaPlayer_EngineAudio::GetMediaNum()
{
    return m_AudioController.GetMediaNum();
}

QString
AppMediaPlayer_EngineAudio::GetCurrentFilePath()
{
    return m_AudioController.GetCurrentFilePath();
}

QString
AppMediaPlayer_EngineAudio::GetCurrentFileName()
{
    return m_AudioController.GetCurrentFileName();
}

QString
AppMediaPlayer_EngineAudio::GetFilesCountInfo (int device)
{
    //{added by junam 2013.08.30 for ITS_KOR_186080
    if(device < MP::ALL_DEVICES)
        return m_AudioController.GetFilesCountInfo(device);
    //}added by junam

    return m_AudioController.GetFilesCountInfo();
}
/*
//{added by aettie.ji 2012.12.18 for new ux
QString
AppMediaPlayer_EngineAudio::GetCurrentCategoryId()
{
    return m_AudioController.GetCurrentCategoryId();
}
//}added by aettie.ji 2012.12.18 for new ux
*/
void
AppMediaPlayer_EngineAudio::GetDiscInfo()
{
    MP_MEDIUM << LOG_ENDL;
    m_AudioController.GetDiscInfo();
}

// { removed by eugeny.novikov 2012.11.10 for CR 14795
//void
//AppMediaPlayer_EngineAudio::onTitleClicked(int title)
//{
//   MP_LOG << "title = "<< title << LOG_ENDL;
//   m_AudioController.selectTitleChapter(title,0);
//}
// } removed by eugeny.novikov


void AppMediaPlayer_EngineAudio::PassNotifyAvModeToiPod(MODE_STATE_T eNextMode)
{
    bool bAhaPandoraModeiPod1;
    bool bAhaPandoraModeiPod2;

    if(eNextMode == MODE_STATE_AHA_RADIO_IPOD1 || eNextMode == MODE_STATE_IPOD1_PANDORA)
    {
        bAhaPandoraModeiPod1 = true;
        bAhaPandoraModeiPod2 = false;
    }
    else if(eNextMode == MODE_STATE_AHA_RADIO_IPOD2 || eNextMode == MODE_STATE_IPOD2_PANDORA)
    {
        bAhaPandoraModeiPod1 = false;
        bAhaPandoraModeiPod2 = true;
    }
    else
    {
        bAhaPandoraModeiPod1 = false;
        bAhaPandoraModeiPod2 = false;
    }

    CIpodController* iPodController1 = m_AudioController.GetControllers().GetIPod1Controller();
    if(iPodController1)
    {
        iPodController1->SetIsAhaPandoraMode(bAhaPandoraModeiPod1);
    }

    CIpodController* iPodController2 = m_AudioController.GetControllers().GetIPod2Controller();
    if(iPodController2)
    {
        iPodController2->SetIsAhaPandoraMode(bAhaPandoraModeiPod2);
    }
}

void
AppMediaPlayer_EngineAudio::Play()
{
    MP_MEDIUM << LOG_ENDL; // added by kihyung 2012.08.01 for CR 12268
    m_AudioController.Play();
}

void
AppMediaPlayer_EngineAudio::Pause()
{
    MP_MEDIUM << LOG_ENDL; // added by kihyung 2012.08.01 for CR 12268
    m_AudioController.Pause();
}

void
AppMediaPlayer_EngineAudio::Stop()
{
    MP_MEDIUM << LOG_ENDL; // added by kihyung 2012.08.01 for CR 12268
    m_AudioController.setIsRunFromFileManager(false);//changed by Alexey Edelev. Fix bug 13058. 2012.09.13
    m_AudioController.Stop();
}

//{ added by hyochang.ryu 20130517
void
AppMediaPlayer_EngineAudio::Toggle()
{
    MP_LOG << LOG_ENDL; // added by kihyung 2012.08.01 for CR 12268
    m_AudioController.Toggle();
}
//} added by hyochang.ryu 20130517

//{ added by hyochang.ryu 20130705
void
AppMediaPlayer_EngineAudio::ResumeBT()
{
    m_AudioController.ResumeBT();
}
//} added by hyochang.ryu 20130705

void
AppMediaPlayer_EngineAudio::NextTrack(bool isUnsurpported) // modified by sangmin.seol 2013-12-13 for : ITS 0215489
{
    m_AudioController.setIsRunFromFileManager(false);//changed by Alexey Edelev. Fix bug 13058. 2012.09.13
    emit setDisplayOSD( true );//added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD
    //m_bPlayAfterBTCall = true;//remove by edo.lee 2013.06.27
    m_AudioController.NextTrack(isUnsurpported); // modified by sangmin.seol 2013-12-13 for : ITS 0215489
    //{ commented by cychoi 2013.08.13 for unused code
    //{ added by hyochang.ryu 20130517 for BT OSD
    //if(m_AudioController.GetMediaMode() == MP::BLUETOOTH)
    //    m_BtTrackButton++;
    //} added by hyochang.ryu 20130517 for BT OSD
    //} commented by cychoi 2013.08.13 for unused code
}

void
// { modified by lssanh 2012.09.18 for No35 car DQA#79
/*
AppMediaPlayer_EngineAudio::PrevTrack(bool isVRPrev)
{
    m_AudioController.setIsRunFromFileManager(false);//changed by Alexey Edelev. Fix bug 13058. 2012.09.13
    m_AudioController.PrevTrack(isVRPrev);
}
*/
AppMediaPlayer_EngineAudio::PrevTrack(bool isDirectPrev)
{
    m_AudioController.setIsRunFromFileManager(false);//changed by Alexey Edelev. Fix bug 13058. 2012.09.13
    //{Added by radhakrushna 2012.14.12 cr 16773
    //{ removed by eunhye 2013.02.22 for ISV 70720
    //if(MP::DISC != m_mode)
    //{
    //} removed by eunhye 2013.02.22
    emit setDisplayOSD( true );//added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD
    
    //{ removed by eunhye 2013.02.22 for ISV 70720
    /*if(m_AudioController.getPlayingPosition() > 3000)
        {
        //Assume that track change will not occure because duration is more tahn 3 sec,
        //So display OSD for same track .
        //There is no notifiaction from mediacontroller , if track starts from begining.
        emit updateOSD(OSD_EVENT_AUDIO_TRACK_INFO);
        }
    }*/
    //} removed by eunhye 2013.02.22
    //}Added by radhakrushna 2012.14.12 cr 16773
    //m_bPlayAfterBTCall = true;//remove by edo.lee 2013.06.27
    m_AudioController.PrevTrack(isDirectPrev);
    //{ commented by cychoi 2013.08.13 for unused code
    //{ added by hyochang.ryu 20130517 for BT OSD
    //if(m_AudioController.GetMediaMode() == MP::BLUETOOTH)
    //    m_BtTrackButton++;
    //} added by hyochang.ryu 20130517 for BT OSD
    //} commented by cychoi 2013.08.13 for unused code
}
// } modified by lssanh 2012.09.18 for No35 car DQA#79
void
AppMediaPlayer_EngineAudio::setPlayingPosition( int seconds )
{
    m_AudioController.setPlayingPosition( seconds );
}

int
AppMediaPlayer_EngineAudio::getPlayingPosition() const
{
    return m_AudioController.getPlayingPosition();
}

int
AppMediaPlayer_EngineAudio::getPlayingDuration() const
{
    return m_AudioController.getPlayingDuration();
}

// { changed by eugeny - 12-09-15
//void
//AppMediaPlayer_EngineAudio::setRandomPlayMode( bool needIncrement, int nRandomStatus )
//{
//    m_AudioController.setRandomPlayMode( needIncrement, nRandomStatus );
//}

//int
//AppMediaPlayer_EngineAudio::getRandomMode() const
//{
//    return m_AudioController.getRandomMode();
//}

//bool
//AppMediaPlayer_EngineAudio::isRandomOff() const
//{
//    return m_AudioController.isRandomOff();
//}

//void
//AppMediaPlayer_EngineAudio::setScanMode()
//{
//    m_AudioController.setScanMode();
//}

//void
//AppMediaPlayer_EngineAudio::resetScan()
//{
//    m_AudioController.resetScan();
//}

//void
//AppMediaPlayer_EngineAudio::setRepeatPlayMode( bool needIncrement, int nRepeatStatus, bool byUser)
//{
//    m_AudioController.setRepeatPlayMode( needIncrement, nRepeatStatus, byUser );
//}

//int
//AppMediaPlayer_EngineAudio::getRepeatMode() const
//{
//    return m_AudioController.getRepeatMode();
//}
// } changed by eugeny - 12-09-15

//{ modified by yongkyun.lee 2013-08-21 for : BT Mode key - unknown OSD
bool
AppMediaPlayer_EngineAudio::getBTSetInfo()
{
    return m_BTSetInfo;
}
//} modified by yongkyun.lee 2013-08-21 

void
AppMediaPlayer_EngineAudio::setBTSetInfo(bool set) 
{
    m_BTSetInfo = set;
}
//hyochang.ryu 20130821 for P1#4

bool
AppMediaPlayer_EngineAudio::isPaused() const
{
    return m_AudioController.isPaused();
}

void
AppMediaPlayer_EngineAudio::HandleFF4X ()
{
    m_AudioController.FastForward(FAST_SPEED_X4);
}

void
AppMediaPlayer_EngineAudio::HandleFF20X ()
{
    // { modified by kihyung 2012.07.11 for CR 10859
    // m_AudioController.FastForward(FAST_SPEED_X20);
    // { modified by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
    if (m_mode == MP::DISC && m_AudioController.getDiscType() == MP::DVD_AUDIO)
    {
        m_AudioController.FastForward(FAST_SPEED_X16); // DVD-Audio 16x
    }
    else if( m_mode != MP::IPOD1 && m_mode != MP::IPOD2 )
    {
        m_AudioController.FastForward(FAST_SPEED_X20); // CDDA, USB, Jukebox 20x
    }
    // } modified by eugeny.novikov
    // } modified by kihyung
}

void
AppMediaPlayer_EngineAudio::HandleRW4X()
{
    m_AudioController.Rewind(FAST_SPEED_X4);
}

void
AppMediaPlayer_EngineAudio::HandleRW20X()
{
    // { modified by kihyung 2012.07.11 for CR 10859
    // m_AudioController.Rewind(FAST_SPEED_X20);
    // { modified by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
    if (m_mode == MP::DISC && m_AudioController.getDiscType() == MP::DVD_AUDIO)
    {
        m_AudioController.Rewind(FAST_SPEED_X16);
    }
    else if( m_mode != MP::IPOD1 && m_mode != MP::IPOD2 )
    {
        m_AudioController.Rewind(FAST_SPEED_X20);
    }
    // } modified by eugeny.novikov
    // } modified by kihyung
}

void AppMediaPlayer_EngineAudio::playIpodFiles()
{
    m_AudioController.PlayiPodFiles();
}

void
AppMediaPlayer_EngineAudio::normalPlay ()
{
    m_AudioController.setIsRunFromFileManager(false);//changed by Alexey Edelev. Fix bug 13058. 2012.09.13
    m_AudioController.NormalPlay();
}

// removed by Dmitry 23.04.13

void
AppMediaPlayer_EngineAudio::requestMLTList()
{
    m_AudioController.requestMLTList();
}

void
AppMediaPlayer_EngineAudio::playFromMLTList(int index)
{
    m_AudioController.playFromMLTList( index );
}

void
AppMediaPlayer_EngineAudio::createPlayList( QStringList filePaths )
{
    m_AudioController.setIsRunFromFileManager(false);//changed by Alexey Edelev. Fix bug 13058. 2012.09.13
    m_AudioController.createPlayList( filePaths, MP::DEFAULT );
}
// { removed by junam 2012.11.02 for unusing code.
//void
//AppMediaPlayer_EngineAudio::updatePopupList()
//{
//    m_AudioController.updatePopupList();
//}
//} removed by junam

void
AppMediaPlayer_EngineAudio::setAlbumThumbnail (QUrl thumbnail)
{
    m_AudioController.setAlbumThumbnail( thumbnail );
}

//{changed by junam 2013.11.06 for coverflow update
void AppMediaPlayer_EngineAudio::requestCovers ()///bool bFirst)
{
    //PathViewModel *pPathView = m_AudioController.GetPathViewModel();

    //MP_LOG<<"bFrist = "<< bFirst << "PathView empty = "<<pPathView->isEmpty() << LOG_ENDL;
    //if( bFirst || pPathView->isEmpty() ) //}changed by junam
    //    m_AudioController.requestCovers();
    //else
    //m_AudioController.onRequestCoverArtForFlowView();
    m_AudioController.requestCovers();
}
//}changed by junam

bool
AppMediaPlayer_EngineAudio::tuneWheel( bool bTuneForward )
{
    return m_AudioController.tuneWheel(bTuneForward);
}

void
AppMediaPlayer_EngineAudio::switchToTuned()
{
    m_AudioController.switchToTuned();
}

bool
AppMediaPlayer_EngineAudio::IsPlaying() const
{
    return m_AudioController.IsPlaying();
}

bool
AppMediaPlayer_EngineAudio::isStopped() const
{
    return  m_AudioController.isStopped();
}

int
AppMediaPlayer_EngineAudio::getSongNameListTempCount()
{
    return  m_AudioController.getSongNameListTempCount();
}

bool
AppMediaPlayer_EngineAudio::isMinerHandleRequired ( int percentage)
{
    return  m_AudioController.isMinerHandleRequired ( percentage);
}

void AppMediaPlayer_EngineAudio::tuneReset()
{
    m_AudioController.tuneReset();
}

void AppMediaPlayer_EngineAudio::updateCDTuneInfo()
{
    m_AudioController.updateCDTuneInfo();
}

//{ add by yongkyun.lee@lge.com  2012.08.02  for CR 12118
void AppMediaPlayer_EngineAudio::setStatusBarOnScreen(DISPLAY_T display )
{
    //modified by edo.lee 2013.04.04
    /*    MP_LOG << ", onScreen = " << display << LOG_ENDL;

    #if STATUSBAR_NEW

    if( display == DISPLAY_REAR)
    {
        OSDview = m_EngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_REAR_SCREEN);
    }
    else
    {
        OSDview = m_EngineMain.GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN);
    }
    //} added by yongkyun.lee 20130215

    QString info = "";
    info = "@OP^SSHOW" ;
    MP_LOG<<"OSD String "<<OSDview<<info << LOG_ENDL;
    //OSDview->setWindowTitle(info); EDO
    OSDWriter::SendOSD(info);
    //QCoreApplication::flush(); //added by edo.lee 2013.01.16
    #else
        int aDisplayId = display;

        Event aEvent( EVT_SHOW_STATUS_BAR, m_EngineMain.GetThisAppId(), APP_STATUS_BAR,
                     QByteArray( (char*)&aDisplayId,sizeof(aDisplayId) ) ); // modified by lssanh 2013.02.21 IPC payload

        emit NotifyUISH( aEvent );
    #endif

*///modified by edo.lee 2013.04.04

}
//} add by yongkyun.lee@lge.com

// added by edo.lee 2013.01.10
void 
AppMediaPlayer_EngineAudio::ClearOSD(bool isFront)
{
    MP_LOG << LOG_ENDL;
    //modified by edo.lee 2013.07.09
    m_EngineMain.GetNotifier()->ClearOSD(isFront);
    //}modified by edo.lee 2013.03.11
    //}
}
//added by edo.lee 

void
AppMediaPlayer_EngineAudio::ShowUI(DISPLAY_T display)
{
    MP_HIGH << m_EntryPoint << display << LOG_ENDL;

    m_currentScreen = display;

    emit  fgReceived(); // added by Sergey 02.08.2103 for ITS#181512
    m_AudioController.setForeground(true);
    m_AudioController.setPlayerActive(true);
    m_AudioController.setOperationDisplay(display == DISPLAY_FRONT ? DISPLAY_REAR:DISPLAY_FRONT); //added by edo.lee 2013.03.22

    //added by edo.lee 2013.08.21 ITS 0185455
    if (m_tuneSelectTimer->isActive())
    {
        m_tuneSelectTimer->stop();
        onTuneSelectTimer();
        showCurrentOSD();
    }
    //added by edo.lee 2013.08.21
    OSDManager::instance()->clearOSDPub(display);//added by edo.lee 2013.09.04 ITS 0187591

    //	ClearOSD(); // removed by Sergey 25.04.2013 not to clear OSD on another display
    setStatusBarOnScreen(display );//add by yongkyun.lee@lge.com  2012.08.02  for CR 12118

    switch (m_EntryPoint)
    {
    case eAPP_MODE_DYNAMICS_DISC: // added by wonseok.heo for Disc IN OUT TEST 2013.07.20
    case eAPP_AVP_AUDIO_MODE_JUKEBOX:
    case eAPP_FILEMANAGER_JUKEBOX_AUDIO: //added by junam 2013.12.19 for LIST_ENTRY_POINT
        m_AudioController.SetMediaMode(MP::JUKEBOX);
        break;
    case eAPP_AVP_AUDIO_MODE_USB1:
    case eAPP_FILEMANAGER_USB1_AUDIO: //added by junam 2013.12.19 for LIST_ENTRY_POINT
        m_AudioController.SetMediaMode(MP::USB1);
        break;
    case eAPP_AVP_AUDIO_MODE_USB2:
    case eAPP_FILEMANAGER_USB2_AUDIO: //added by junam 2013.12.19 for LIST_ENTRY_POINT
        m_AudioController.SetMediaMode(MP::USB2);
        break;
    case eAPP_AVP_AUDIO_MODE_DISC:
    case eAPP_FILEMANAGER_DISC_AUDIO: // added by oseong.kwon 2014.02.10 for ITS 223602
        if(m_AudioController.getDiscType() == MP::DVD_AUDIO) {
            m_AudioController.showVideo(display);
        }
        // { added by eunhye 2013.04.25
        if(m_AudioController.getDiscType() == MP::MP3_CD)// modified by eunhye 2013.04.30
            emit mp3ListReadState(m_MP3ListView);
        // } added by eunhye 2013.04.25
        m_AudioController.SetMediaMode(MP::DISC);
        break;
    case eAPP_AVP_AUDIO_MODE_IPOD1:
    case eAPP_FILEMANAGER_IPOD1: //added by junam 2013.12.19 for LIST_ENTRY_POINT
        m_AudioController.SetMediaMode(MP::IPOD1);
        // removed by Sergey 02.08.2103 for ITS#181512
        break;
    case eAPP_AVP_AUDIO_MODE_IPOD2:
    case eAPP_FILEMANAGER_IPOD2: //added by junam 2013.12.19 for LIST_ENTRY_POINT
        m_AudioController.SetMediaMode(MP::IPOD2);
        // removed by Sergey 02.08.2103 for ITS#181512
        break;
    case eAPP_AVP_AUDIO_MODE_BT:
    case eAPP_AVP_MODE_BLUETOOTH:
        m_AudioController.SetMediaMode(MP::BLUETOOTH);
        //m_AudioController.onBtRequestTimerStart(); // added by yongkyun.lee 20130426 for : BT sync
        break;
    case eAPP_AVP_AUDIO_MODE_AUX:
        m_AudioController.SetMediaMode(MP::AUX);
        break;
    case eAPP_AVP_AUDIO:
    case eAPP_MODE_LAST_STATE:
        m_AudioController.SetMediaMode(m_mode);
        break;
    default:
        break;
    }
}
//remove by edo.lee 2013.03.21
//void
//AppMediaPlayer_EngineAudio::setIsFrontView(bool isFront)
//{
//	MP_LOG<<isFront << LOG_ENDL;
//	m_isFrontBack = isFront;
//}

// added by kihyung 2013.09.12 for ISV 90605
void AppMediaPlayer_EngineAudio::SetNotifyAllUnsupported()
{
    MP::DEVICE_T device = m_AudioController.getDevice();
    if(device >= MP::ALL_DEVICES) return;

    MP_LOG << device << LOG_ENDL;

    //{ added by hyochang.ryu 20131003 for ITS193574
    if(device==MP::BLUETOOTH)    
    m_EngineMain.GetMediaManager()->SetSourceAvailable(AudioUtils::DeviceType(device),MEDIA_TYPE_MUSIC,true);
    else
    //} added by hyochang.ryu 20131003 for ITS193574
    m_EngineMain.GetMediaManager()->SetSourceAvailable(AudioUtils::DeviceType(device),MEDIA_TYPE_MUSIC,false);
    m_EngineMain.GetNotifier()->UpdateSDPMediaCount(AudioUtils::DeviceType(device), MEDIA_TYPE_MUSIC, 0);
    m_EngineMain.GetNotifier()->setEnableMedia(AudioUtils::DeviceType(device), MEDIA_TYPE_MAX);
}


//added by edo.lee 2013.03.16
void
AppMediaPlayer_EngineAudio::HandleBackKey()
{
    MP_LOG << LOG_ENDL;
    Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_UI_MANAGER );
    MP_LOG << m_EngineMain.getIsFrontBack() << LOG_ENDL;
    emit NotifyUISH (event, m_EngineMain.getIsFrontBack() ? DISPLAY_FRONT:DISPLAY_REAR);
}

void
AppMediaPlayer_EngineAudio::HandleSoftIpod()
{
    MP_LOG << LOG_ENDL;
    if ((m_AudioController.playerMode() == MP::IPOD1 || m_AudioController.playerMode() == MP::IPOD2) &&  m_EngineMain.getisBTCall() == true)
    {
        Event event( EVT_KEY_SOFT_AVP, GetThisAppId(), APP_UI_MANAGER, (m_AudioController.playerMode() == MP::IPOD1)?eAPP_AVP_AUDIO_MODE_IPOD1:eAPP_AVP_AUDIO_MODE_IPOD2 );
        if (m_EngineMain.getCloneState() == CLONE_ON_REAR || ( m_EngineMain.getCloneState() == CLONE_OFF && !m_EngineMain.getIsSwapEnabled()))
        {
            MP_LOG << " DISPLAY_REAR" << LOG_ENDL;
            emit NotifyUISH (event, DISPLAY_REAR);
        }
        else
        {
            MP_LOG << " DISPLAY_FRONT" << LOG_ENDL;
            emit NotifyUISH (event, DISPLAY_FRONT);
        }
    }
    else
    {
        Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_UI_MANAGER );
        MP_LOG << m_EngineMain.getIsFrontBack() << LOG_ENDL;
        emit NotifyUISH (event, m_EngineMain.getIsFrontBack() ? DISPLAY_FRONT:DISPLAY_REAR);
    }
}

void
AppMediaPlayer_EngineAudio::LaunchBTSettings()
{
    m_EngineMain.GetLogger().Log( "AppMusicPlayer::AppMediaPlayer_EngineAudio:LaunchSettingsBT: Start" );

    Event event( EVT_KEY_SOFT_BT_SETTINGS, GetThisAppId(), APP_UI_MANAGER );

    emit NotifyUISH(event);
}

void
AppMediaPlayer_EngineAudio::LaunchKeypadSettings(bool isFront) //modified by edo.lee 2013.05.06
{
    MP_LOG << LOG_ENDL; // modified by eugeny.novikov 2012.11.13 for CR 15121
    // { modified by kihyung 2013.1.26
#if USE_NEW_SETTING_ENTRY_POINT
    APP_EVENT_DATA_BASE_T pEventData;
    pEventData.sIsJogEnabled = false;

    Event softEvent(EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER, eAPP_SETTINGS_GENERAL_KEYBOARD,
                    QByteArray((char *)&pEventData, sizeof(pEventData))); // modified by lssanh 2013.02.21 IPC payload
    //emit NotifyUISH(softEvent, m_EngineMain.getCurrScreen());
    emit NotifyUISH(softEvent,isFront?DISPLAY_FRONT:DISPLAY_REAR); //modified by edo.lee 2013.05.06
#else
    AppSettingsStartParameter startState;
    startState.startState = AppSettingsDef::eStart_Mode_General_Keyboard;
    Event postEvent(EVT_POST_EVENT, GetThisAppId(), APP_SETTINGS, QByteArray((char *) &startState, sizeof(AppSettingsStartParameter)));
    emit NotifyUISH(postEvent );
    
    Event event(EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER);
    emit NotifyUISH(event);
#endif
    m_bIsKeypadSettings = true; // added by eugeny.novikov 2012.11.13 for CR 15121
    // } modified by kihyung 2013.1.26
}

void AppMediaPlayer_EngineAudio::MoreInfoState(MP::DEVICE_T state)
{
    // { removed by kihyung 2013.3.20 for performance
    /*
    bool tmp = false;

    switch (state)
    {
        case MP::JUKEBOX:
            tmp = JUKEBOX_MAIN_MENU[7].bSelected;
            break;

        case MP::USB1:
        case MP::USB2:
            tmp = USB_MAIN_MENU[10].bSelected;
            break;

        case MP::IPOD1:
        case MP::IPOD2:
            break;

        case MP::DISC:
            if (m_AudioController.getDiscType() == MP::AUDIO_CD)
            {
                tmp = DISC_MAIN_MENU[7].bSelected;
            }
            else if (m_AudioController.getDiscType() == MP::MP3_CD)
            {
                tmp = DISC_MP3_MAIN_MENU[6].bSelected;
            }
            break;
            
        default:
            break;
    }
    emit moreInfoChanged(tmp);
    */
    // { removed by kihyung 2013.3.20 for performance
}

void AppMediaPlayer_EngineAudio::onSetAppState(MP::DEVICE_T state)
{
    MP_LOG << state << LOG_ENDL;
    m_mode = state;
    MoreInfoState(state); //added by eunhye 2012.09.10 for CR 13134
    switch ( state )
    {
    case MP::JUKEBOX:
    case MP::USB1:
    case MP::USB2:
    case MP::IPOD1:
    case MP::IPOD2:
    {
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_PLAY_STATUS, m_AudioController.IsPlaying());
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_RANDOM, m_AudioController.IsRandom());
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_REPEAT, m_AudioController.IsRepeat());
        break;
    }

    case MP::DISC:
    {
        // { modified by eugeny.novikov@teleca.com 2012.08.09 for CR 12186
        INSERTED_DISC_TYPE_T type = (m_AudioController.getDiscType() == MP::AUDIO_CD)  ? INSERTED_DISC_TYPE_CDDA :
                                                                                         (m_AudioController.getDiscType() == MP::DVD_AUDIO) ? INSERTED_DISC_TYPE_DVDAUDIO
                                                                                                                                            : INSERTED_DISC_TYPE_CA;
        // } modified by eugeny.novikov

        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_INSERTED_DISC, type);
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_PLAY_STATUS, m_AudioController.IsPlaying());
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_RANDOM, m_AudioController.IsRandom());
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_REPEAT, m_AudioController.IsRepeat());
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_MAX_TRACK_NUM, m_AudioController.GetMediaNum());
        m_pSharedAudioData->SetStatusDateByIndex(DATA_TYPE_DISC_MEDIA_LOADING , MEDIA_DEVICE_SCAN_END);
        break;
    }

    case MP::BLUETOOTH:
    case MP::UNDEFINED:
    default:
        break;
    }

    m_tuneSelectTimer->stop();
}

void AppMediaPlayer_EngineAudio::onSetStatusDataByIndex (MP::DEVICE_T state, DATA_TYPE_T index, int aValue )
{
    MP_LOG << m_mode << state << index << aValue  << LOG_ENDL;

    //{ added by yongkyun.lee 20130320 for : 70310
    //if (state == m_mode)
    if (state == m_mode || index == DATA_TYPE_USB1_MEDIA_LOADING || index == DATA_TYPE_USB2_MEDIA_LOADING ||
            index == DATA_TYPE_IPOD1_MEDIA_LOADING || index == DATA_TYPE_IPOD2_MEDIA_LOADING) //added by edo.lee 2013.05.20
        //} added by yongkyun.lee 20130320
    {
        m_pSharedAudioData->SetStatusDateByIndex ( index, aValue );
    }
}

// { remvoed by kihyung 2012.11.26. removed spec.
/*
void
AppMediaPlayer_EngineAudio::stopAVWidgets()
{
   HOME_MEDIA_T stData;
   stData.nType = HomeScreenDef::MEDIA_TYPE_INVALID;

   QByteArray aData = QByteArray::fromRawData( (char*)&stData, sizeof(stData) );

   Event event( EVT_POST_EVENT, GetThisAppId(), APP_MAIN_MENU, HomeScreenDef::SET_PLAYING_MEDIA, aData );
   emit NotifyUISH( event );
}
*/
// } remvoed by kihyung 2012.11.26. removed spec.

// { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
//void
//AppMediaPlayer_EngineAudio::NotifyVRTrackerChanged( bool isMinerFinished )
//{
// { modified by yongkyun.lee@lge.com  2012.08.28 for :CR13323 ,VR Create START/END
//#if 1
//    MP::DEVICE_T newDevice;
//    newDevice = (m_AudioController.m_VRTrackerUSB == MP::UNDEFINED) ?
//        MP::JUKEBOX: m_AudioController.m_VRTrackerUSB;
//    
//    sendVRTrackerChanged(isMinerFinished , makeDataVRTracker(newDevice) );
//    return;
//#else
//    MP_LOG << "AppMediaPlayer_EngineAudio::NotifyVRTrackerChanged, isMinerFinished = " << isMinerFinished << LOG_ENDL;
//
//    EVT_PUBLIC_ID_T evtType = isMinerFinished ? EVT_LIST_CREATE_END : EVT_LIST_CREATE_START;
//
//    Event event( evtType, GetThisAppId(), APP_VR, EDATA_CREATE_TYPE_SEARCH_MUSIC );
//    emit NotifyUISH( event );
//#endif
// } modified by  yongkyun.lee@lge.com  
//
//}
//{ deleted by yongkyun.lee 20130206 for : NO VR: VR is not using Mining  sig.
/*void
AppMediaPlayer_EngineAudio::NotifyVRTrackerChanged( bool isMinerFinished , QString usbMiningPath)
{
    static MP::DEVICE_T newDevice;

    MP_LOG << "[VR]path=" << usbMiningPath << LOG_ENDL;
    if(!isMinerFinished)
        newDevice = (MP::DEVICE_T)getVolumeId (usbMiningPath );

    DeviceMiningBitMask( newDevice , VR_BIT_TE );
    sendVRTrackerChanged(isMinerFinished , newDevice, false);
    return;
}*/
// } modified by  yongkyun.lee  
//} deleted by yongkyun.lee 20130206

// { added  by  yongkyun.lee 2012.11.21 for  xSampa check.
int AppMediaPlayer_EngineAudio::getVolumeId( QString path )
{
    int volume =  MP::JUKEBOX;
#if 0
    QString usb1MountPath = m_AudioController.getMountPath( MP::USB1);
    QString usb2MountPath = m_AudioController.getMountPath( MP::USB2);
    QString ipod1MountPath= m_AudioController.getMountPath( MP::IPOD1);
    QString ipod2MountPath= m_AudioController.getMountPath( MP::IPOD2);

    MP_LOG<< "[VR]usb1  =" <<usb1MountPath  << LOG_ENDL;
    MP_LOG<< "[VR]usb2  =" <<usb2MountPath  << LOG_ENDL;
    MP_LOG<< "[VR]ipod1 =" <<ipod1MountPath  << LOG_ENDL;
    MP_LOG<< "[VR]ipod2 =" <<ipod2MountPath  << LOG_ENDL;
#endif//
    if (path.startsWith("/rw_data/media/front_usb") || path.startsWith("/media/front_usb"))
    {
        volume = MP::USB2;
    }
    else if (path.startsWith("/rw_data/media/rear_usb") || path.startsWith("/media/rear_usb"))
    {
        volume = MP::USB1;
    }
    else if ( path.startsWith("/dev/bus/usb/002/004"))
    {
        volume = MP::IPOD1;
    }
    else if ( path.startsWith("/dev/bus/usb/002/005"))
    {
        volume = MP::IPOD2;
    }

    MP_LOG << "[VR]volume =" << volume << LOG_ENDL;
    return volume;
}



int AppMediaPlayer_EngineAudio::DeviceConverter(int deviceType)
{
    MP::DEVICE_T retType = MP::UNDEFINED ;
    switch(deviceType)
    {
    case 0://e_JukeBox
        retType = MP::JUKEBOX;
        break;
    case 1://e_USB1
        retType = MP::USB1;
        break;
    case 2://e_USB2
        retType = MP::USB2;
        break;
    case 4://e_IPOD1
        retType = MP::IPOD1;
        break;
    case 5://e_IPOD2
        retType = MP::IPOD2;
        break;
    case 3://e_Disc
        retType = MP::DISC;
        break;
    case 6://e_CA
        retType = MP::UNDEFINED;//??
        break;
    }
    return retType;
}
//{ deleted by yongkyun.lee 20130206 for : NO VR: VR is not using Mining  sig.
/*
bool AppMediaPlayer_EngineAudio::DeviceMiningBitMask(MP::DEVICE_T newDevice ,vrBitShift isShift)
{
    bool retval = true;

    MP_LOG << "[VR]DeviceMiningBitMask::isShift= " << isShift << " newDevice= " << newDevice << " m_vrMiningMask = " << m_vrMiningMask << LOG_ENDL;
    if(isShift == VR_BIT_DISCONNECT)
    {
        m_vrMiningMask |= (VR_MINING_MASK_DISCON);
        MP_LOG << "[VR]DeviceMiningBitMask::DISCONNECT , m_vrMiningMask = " << m_vrMiningMask << LOG_ENDL;
        return false;
    }

    switch(newDevice)
    {
    case MP::JUKEBOX:
        m_vrMiningMask |= (VR_BIT_MASK_JUKEBOX << isShift );
        retval = false;
        break;
    case MP::USB1:
        m_vrMiningMask |= (VR_BIT_MASK_USB1 << isShift);
        retval = false;
        break;
    case MP::USB2:
        m_vrMiningMask |= (VR_BIT_MASK_USB2 << isShift);
        retval = false;
        break;
    case MP::IPOD1:
        m_vrMiningMask |= (VR_BIT_MASK_IPOD1 << isShift);
        retval = false;
        break;
    case MP::IPOD2:
        m_vrMiningMask |= (VR_BIT_MASK_IPOD2 << isShift);
        retval = false;
        break;
    default:
        break;
    }
    
    MP_LOG << "[VR]DeviceMiningBitMask::retval= " << retval << " m_vrMiningMask = " << m_vrMiningMask << LOG_ENDL;

    return true;
}


bool AppMediaPlayer_EngineAudio::DeviceUnminingBitMask(MP::DEVICE_T newDevice ,vrBitShift isShift)
{
    bool retval = true;

    switch(newDevice)
    {
    case MP::JUKEBOX:
        m_vrMiningMask &= ~(VR_BIT_MASK_JUKEBOX << isShift);
        retval = false;
        break;
    case MP::USB1:
        m_vrMiningMask &= ~(VR_BIT_MASK_USB1 << isShift);
        retval = false;
        break;
    case MP::USB2:
        m_vrMiningMask &= ~(VR_BIT_MASK_USB2 << isShift);
        retval = false;
        break;
    case MP::IPOD1:
        m_vrMiningMask &= ~(VR_BIT_MASK_IPOD1 << isShift);
        retval = false;
        break;
    case MP::IPOD2:
        m_vrMiningMask &= ~(VR_BIT_MASK_IPOD2 << isShift);
        retval = false;
        break;
    default:
        break;
    }
    
    MP_LOG << "[VR]DeviceUnminingBitMask::retval= " << retval << " m_vrMiningMask = " << m_vrMiningMask << LOG_ENDL;

    return true;
}



bool AppMediaPlayer_EngineAudio::CheckMiningBitMask(MP::DEVICE_T newDevice)
{
    bool retval = false;

    MP_LOG << "[VR]CheckMiningBitMask::m_vrMiningMask = " << m_vrMiningMask << LOG_ENDL;
    if(m_vrMiningMask & VR_MINING_MASK_DISCON)
    {
        MP_LOG << "[VR]CheckMiningBitMask::DISCONNECT , m_vrMiningMask = " << m_vrMiningMask << LOG_ENDL;
        return true;
    }

    switch(newDevice)
    {
    case MP::JUKEBOX:
        if((m_vrMiningMask & (VR_BIT_MASK_JUKEBOX << VR_BIT_TE)) && (m_vrMiningMask & (VR_BIT_MASK_JUKEBOX << VR_BIT_XSMAPA)))
            retval = true;
        break;
    case MP::USB1:
        if((m_vrMiningMask & (VR_BIT_MASK_USB1 << VR_BIT_TE))    && (m_vrMiningMask & (VR_BIT_MASK_USB1 << VR_BIT_XSMAPA)))
            retval = true;
        break;
    case MP::USB2:
        if((m_vrMiningMask & (VR_BIT_MASK_USB2 << VR_BIT_TE))    && (m_vrMiningMask & (VR_BIT_MASK_USB2 << VR_BIT_XSMAPA)))
            retval = true;
        break;
    case MP::IPOD1:
        if((m_vrMiningMask & (VR_BIT_MASK_IPOD1 << VR_BIT_TE))   && (m_vrMiningMask & (VR_BIT_MASK_IPOD1 << VR_BIT_XSMAPA)))
            retval = true;
        break;
    case MP::IPOD2:
        if((m_vrMiningMask & (VR_BIT_MASK_IPOD2 << VR_BIT_TE))   && (m_vrMiningMask & (VR_BIT_MASK_IPOD2 << VR_BIT_XSMAPA)))
            retval = true;
        break;
    default :
        return false;
    }
    MP_LOG << "[VR]CheckMiningBitMask::----->>>> retval= " << retval << " m_vrMiningMask = " << m_vrMiningMask << LOG_ENDL;

   return retval;

}

bool AppMediaPlayer_EngineAudio::ClearMiningMask(MP::DEVICE_T newDevice)
{
    m_vrMiningMask &= ~VR_MINING_MASK_DISCON;

    switch(newDevice)
    {
    case MP::JUKEBOX:
        m_vrMiningMask &= ~(VR_MINING_MASK_JK);
        //m_vrMiningMask &= ~(VR_BIT_MASK_JUKEBOX << VR_BIT_DEVICE);
        break;
    case MP::USB1:
        //m_vrMiningMask &= ~(VR_MINING_MASK_JK);//TEST CODE
        m_vrMiningMask &= ~(VR_MINING_MASK_USB1);
        m_vrMiningMask &= ~(VR_BIT_MASK_USB1 << VR_BIT_DEVICE);
        break;
    case MP::USB2:
        //m_vrMiningMask &= ~(VR_MINING_MASK_JK);//TEST CODE
        m_vrMiningMask &= ~(VR_MINING_MASK_USB2);
        m_vrMiningMask &= ~(VR_BIT_MASK_USB2 << VR_BIT_DEVICE);
        break;
    case MP::IPOD1:
        m_vrMiningMask &= ~(VR_MINING_MASK_IPOD1);
        m_vrMiningMask &= ~(VR_BIT_MASK_IPOD1 << VR_BIT_DEVICE);
        break;
    case MP::IPOD2:
        m_vrMiningMask &= ~(VR_MINING_MASK_IPOD2);
        m_vrMiningMask &= ~(VR_BIT_MASK_IPOD2 << VR_BIT_DEVICE);
        break;
    default :
        return false;
    }
    MP_LOG << "[VR]ClearMiningMask:: m_vrMiningMask = " << m_vrMiningMask << LOG_ENDL;
    return true;
}



bool AppMediaPlayer_EngineAudio::MiningMask(MP::DEVICE_T newDevice ,vrBitShift isShift)
{
    bool retval = true;
    //MP_LOG << "[VR]MiningMask::NotifyVRTrackerChanged===========>>> isShift= " << isShift << " m_vrMiningMask = " << m_vrMiningMask << LOG_ENDL;
    if(isShift == VR_BIT_ALL_MASK)
    {
        DeviceMiningBitMask(newDevice , VR_BIT_TE);
        DeviceMiningBitMask(newDevice , VR_BIT_XSMAPA);
        retval = DeviceMiningBitMask(newDevice , VR_BIT_DEVICE);
    }
    else
    {
        retval = DeviceMiningBitMask(newDevice , isShift);
    }

    //MP_LOG << "[VR]MiningMask :: NotifyVRTrackerChanged===========>>> ret= " << retval << " m_vrMiningMask = " << m_vrMiningMask << LOG_ENDL;
    return retval;
}
// } added  by  yongkyun.lee 

// { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
// { add by yongkyun.lee@lge.com  2012.08.28  for :CR13323 ,VR Create START/END
//void
//AppMediaPlayer_EngineAudio::sendVRTrackerChanged(bool isMinerFinished ,MusicDBChangedStatus *changeDB)
//{
//    EVT_PUBLIC_ID_T evtType = isMinerFinished ? EVT_LIST_CREATE_END : EVT_LIST_CREATE_START;
//
//    // { modified by yongkyun.lee 2012.10.31
//    MP_LOG << "[VR]:NotifyVRTrackerChanged: " << \
//        (evtType ==EVT_LIST_CREATE_END) ? "EVT_LIST_CREATE_END" : "EVT_LIST_CREATE_START" << LOG_ENDL;
//
//    if( evtType == EVT_LIST_CREATE_END)
//        m_AudioController.m_VRTrackerUSB = MP::UNDEFINED;
//    // { move by yongkyun.lee@lge.com  2012.09.12  for :CR13323,Change VR data struct type. 
//    MP_LOG << "[VR]NotifyVRTrackerChanged: Jukebox = " << changeDB->bJukeboxData " USB = " << changeDB->bUSBData << " Ipod = " <<changeDB->bIpodData << "IpodConstatus = " << changeDB->eIpodConnectStatus << LOG_ENDL;
//    // } modified by yongkyun.lee 2012.10.31
//
//    MP_LOG << "[VR]NotifyVRTrackerChanged: changeDB.szUSBMountPath    = " << changeDB->szUSBMountPath << LOG_ENDL;
//    MP_LOG << "[VR]NotifyVRTrackerChanged: changeDB.szIpodSerial      = " << changeDB->szIpodSerial << LOG_ENDL;
//    // } move by yongkyun.lee@lge.com 
//    // { modified by yongkyun.lee@lge.com    2012.09.12 : CR13323,Change VR data struct type.
//    //Event event( evtType, GetThisAppId(), APP_VR, EDATA_CREATE_TYPE_SEARCH_MUSIC ,  QByteArray::fromRawData( (char*)&changeDB, sizeof( MusicDBChangedStatus ) ));
//    Event event( evtType, GetThisAppId(), APP_VR, EDATA_CREATE_TYPE_SEARCH_MUSIC ,  QByteArray::fromRawData( (char*)changeDB, sizeof( MusicDBChangedStatus ) ));
//    // } modified by yongkyun.lee@lge.com
//    emit NotifyUISH( event );
//}
void AppMediaPlayer_EngineAudio::sendVRTrackerChanged(bool isMinerFinished ,MP::DEVICE_T newDevice ,bool isXsmapa)
{
    MusicDBChangedStatus *changeDB;

    EVT_PUBLIC_ID_T evtType = isMinerFinished ? EVT_LIST_CREATE_END : EVT_LIST_CREATE_START;

    MP_LOG << "[VR]NotifyVRTrackerChanged : " << (evtType ==EVT_LIST_CREATE_END) ? "EVT_LIST_CREATE_END" : "EVT_LIST_CREATE_START" " Device=" << newDevice << " isXsmapa = " << isXsmapa << LOG_ENDL;

    if( (evtType == EVT_LIST_CREATE_END) && !CheckMiningBitMask(newDevice) )
    {
        MP_LOG << "[VR]NotifyVRTrackerChanged: CheckMiningBitMask RETURN===>>"  << LOG_ENDL;
        return;
    }
    else if(evtType == EVT_LIST_CREATE_START)
        DeviceUnminingBitMask(newDevice,VR_BIT_TE );
        
    changeDB = makeDataVRTracker(newDevice);

    MP_LOG << "[VR]NotifyVRTrackerChanged: Jukebox = " << changeDB->bJukeboxData " USB = " << changeDB->bUSBData << " Ipod = " <<changeDB->bIpodData << "IpodConstatus = " << changeDB->eIpodConnectStatus << LOG_ENDL;
    MP_LOG << "[VR]NotifyVRTrackerChanged: changeDB.szUSBMountPath    = " << changeDB->szUSBMountPath << LOG_ENDL;
    MP_LOG << "[VR]NotifyVRTrackerChanged: changeDB.szIpodSerial      = " << changeDB->szIpodSerial << LOG_ENDL;
    Event event( evtType, GetThisAppId(), APP_VR, EDATA_CREATE_TYPE_SEARCH_MUSIC ,  QByteArray::fromRawData( (char*)changeDB, sizeof( MusicDBChangedStatus ) ));
    // { modified by  yongkyun.lee 2012.11.21 for  CR 14584 When say "Music search" in VR App, "There is no connection for media" comment is coming
    //emit NotifyUISH( event );
    m_EngineMain.sendVRMining(event);
    // { modified by  yongkyun.lee 2012.11.21

    //if( ((m_vrMiningMask & VR_MINING_MASK_DISCON)||(MP::JUKEBOX == newDevice) )&&  \
    //    (evtType == EVT_LIST_CREATE_END))
    if(evtType == EVT_LIST_CREATE_END)
           ClearMiningMask( newDevice);

}
// } modified by  yongkyun.lee

MusicDBChangedStatus *
AppMediaPlayer_EngineAudio::makeDataVRTracker( MP::DEVICE_T newDevice)
{
// { modified by yongkyun.lee@lge.com    2012.09.12 : CR13323,Change VR data struct type.
    QString sPath;

    memset((char*)&m_changeDB, 0x00, sizeof(MusicDBChangedStatus));

    switch(newDevice)
    {
    case MP::USB1:
    case MP::USB2:
        m_changeDB.bUSBData     = 0x01;
        break;
    case MP::IPOD1:
        m_changeDB.bIpodData    = 0x01;
        m_changeDB.eIpodConnectStatus= eIPOD_FRONT;
        break;
    case MP::IPOD2:
        m_changeDB.bIpodData    = 0x01;
        m_changeDB.eIpodConnectStatus= eIPOD_REAR ;
        break;
    case MP::JUKEBOX:
    default:
        m_changeDB.bJukeboxData = 0x01;
        break;
    }
    // { modified by yongkyun.lee@lge.com    2012.09.20 : Bug 13901 - mount path is invalid.
    //sPath = m_AudioController.GetActiveDevice(MP::USB1);
    //strncpy((char*)&m_changeDB.szUSBMountPath,sPath.toAscii(), VR_MOUNT_PATH_LEN-1);
    
    //sPath = m_AudioController.GetActiveDevice(MP::IPOD1);
    //strncpy((char*)&m_changeDB.szIpodSerial  ,sPath.toAscii(), VR_MOUNT_PATH_LEN-1);
    sPath = m_AudioController.GetMountPathFromUSB(newDevice  );
    strncpy((char*)m_changeDB.szUSBMountPath,sPath.toAscii(), VR_MOUNT_PATH_LEN-1);

    sPath = m_AudioController.GetMountPathFromIpod(newDevice );
    strncpy((char*)m_changeDB.szIpodSerial  ,sPath.toAscii(), VR_MOUNT_PATH_LEN-1);

    // } modified by  yongkyun.lee@lge.com
    return &m_changeDB;
}
*/
//} deleted by yongkyun.lee 20130206

void AppMediaPlayer_EngineAudio::launchSettings(bool bDVDSetting, bool isDvdTabDimmed, bool isFront)
{
    // { modified by kihyung 2013.1.26
#if USE_NEW_SETTING_ENTRY_POINT
    APP_EVENT_DATA_BASE_T pEventData;
    pEventData.sIsJogEnabled = false;

    int nEntryPoint;
    if(bDVDSetting == true) nEntryPoint = eAPP_SETTINGS_GENERAL_VIDEO_DVD;
    else                    nEntryPoint = eAPP_SETTINGS_SCREEN_AVP_DVD;
    
    Event softEvent(EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER, nEntryPoint, QByteArray((char *)&pEventData, sizeof(pEventData))); // modified by lssanh 2013.02.21 IPC payload
    //emit NotifyUISH(softEvent, m_EngineMain.getCurrScreen());
    emit NotifyUISH(softEvent,isFront ? DISPLAY_FRONT:DISPLAY_REAR); //modified by edo.lee 2013.05.06
#else
    AppSettingsDef::EAppSettingsStartState nState;
    AppSettingsStartParameter param;
    if(bDVDSetting == true) {
        nState = AppSettingsDef::eStart_Mode_General_Video_Control;
    }
    else {
        nState = AppSettingsDef::eStart_Mode_Screen;
    }
    
    param.startState = nState;
    param.transparentBG = true;
    param.divxDvdTabDimmed = isDvdTabDimmed; // modified by ravikanth

    Event postEvent(EVT_POST_EVENT, m_EngineMain.GetThisAppId(), APP_SETTINGS, QByteArray((char*)&param, sizeof(param))); // modified by lssanh 2013.02.21 IPC payload
    emit NotifyUISH(postEvent);

    APP_EVENT_SETTINGS_DATA_T pEventData;
    pEventData.header.sIsJogEnabled = false;
    pEventData.sStartState = nState;
    
    Event softEvent(EVT_KEY_SOFT_SETTINGS, m_EngineMain.GetThisAppId(), APP_UI_MANAGER, QByteArray((char*)&pEventData, sizeof(pEventData))); // modified by lssanh 2013.02.21 IPC payload
    emit NotifyUISH(softEvent);
#endif    
    // } modified by kihyung 2013.1.26
}

//Added by Alexey Edelev 2012.09.06. CR 9413
//{
bool
AppMediaPlayer_EngineAudio::IsSourceAvailable( const QString& tabId )
{
    AppMediaPlayer_MediaManager * mediaMnagerPtr = m_EngineMain.GetMediaManager();
    if(tabId == "Jukebox")
    {
        return mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_MUSIC);
    }
    if(tabId == "USB1")
    {
        return mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_USB1, MEDIA_TYPE_MUSIC);
    }
    if(tabId == "USB2")
    {
        return mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_USB2, MEDIA_TYPE_MUSIC);
    }
    if(tabId == "Disc")
    {
        return (mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_DVDVIDEO, MEDIA_TYPE_MUSIC)
                | mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_VCD, MEDIA_TYPE_MUSIC)
                | mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_MP3, MEDIA_TYPE_MUSIC)
                | mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_CDDA, MEDIA_TYPE_MUSIC)
                | mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_DVDAUDIO, MEDIA_TYPE_MUSIC));
    }
    if(tabId == "iPod1")
    {
        return !m_AudioController.iPod1ContentSizeReady() || mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_IPOD1, MEDIA_TYPE_MUSIC);//Changed by Alexey Edelev. 2012.09.08 CR13600, CR13610
    }
    if(tabId == "iPod2")
    {
        return !m_AudioController.iPod2ContentSizeReady() || mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_IPOD2, MEDIA_TYPE_MUSIC);//Changed by Alexey Edelev. 2012.09.08 CR13600, CR13610
    }
    if(tabId == "AUX")
    {
        return (mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_AUX1, MEDIA_TYPE_MUSIC)
                | mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_AUX2, MEDIA_TYPE_MUSIC));
    }
    if(tabId == "Bluetooth")
    {
        return mediaMnagerPtr->IsSourceAvailable(MEDIA_SOURCE_BLUETOOTH, MEDIA_TYPE_MUSIC);
    }

    return false;
}
//}
//Added by Alexey Edelev 2012.09.06. CR 9413

/*ipod, */
//{ deleted by yongkyun.lee 20130206 for : NO VR: VR is not using Mining  sig.
/*// { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
//void
//AppMediaPlayer_EngineAudio::VRTrackerChanged( bool isMinerFinished , MP::DEVICE_T newDevice )
//{
//    MP_LOG << "[VR] " << LOG_ENDL;
//    sendVRTrackerChanged(isMinerFinished , makeDataVRTracker(newDevice) );
//    return;
//}
// } add by yongkyun.lee@lge.com 
void
AppMediaPlayer_EngineAudio::VRTrackerChanged( bool isMinerFinished , int isShift , MP::DEVICE_T newDevice )
{
    MP_LOG << "[VR] "<< isMinerFinished <<" " << isShift <<" "<< newDevice  << LOG_ENDL;
    MiningMask( newDevice ,(vrBitShift)isShift );
    sendVRTrackerChanged(isMinerFinished , newDevice, false);
    return;
}
// } modified by  yongkyun.lee 
*/
//} deleted by yongkyun.lee 20130206

void
AppMediaPlayer_EngineAudio::MusicIDCDDataReceived(bool status)
{
    MP_LOG << status << LOG_ENDL;
    if (status)
    {
        m_AudioController.LoadGracenoteData();
    }
    //{ added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
    else
    {
        m_AudioController.SetCDDAGraceNote(GRACENOTE_INDEXING_NO_INFO);
    }
    //} added by yongkyun.lee 20130416
}
//deleted by aettie 20131010 unused code
//void
//AppMediaPlayer_EngineAudio::onCoverArtRetrieved(bool status, QString imagePath)
//{
//    MP_LOG << "status =" << status << "coverPath =" << imagePath; << LOG_ENDL;

//    emit updateBTCoverArt(status, imagePath);
//}

void AppMediaPlayer_EngineAudio::tuneHandler( int eventId, const Event &aEvent )
{
    MP_LOG << "Event command =" << aEvent.GetCommand() << LOG_ENDL;
    //added by suilyou 20131205 ITS HandleUSBStateChanged START
    if(m_AudioController.GetPlayerState()==PLAYER_STOP&&m_AudioController.getScanMode()==true)
        return;
    //added by suilyou 20131205 ITS HandleUSBStateChanged END
    // { added by kihyung 2013.4.3
    if(m_AudioController.playerMode() == MP::JUKEBOX && m_EngineMain.isTAScanComplete(eJukeBox) == false) {
        MP_LOG << "JUKEBOX is running TA Scan. return..." << LOG_ENDL;
        return;
    }
    else if(m_AudioController.playerMode() == MP::USB1 && m_EngineMain.isTAScanComplete(eUSB_FRONT) == false) {
        MP_LOG << "USB1 is running TA Scan. return..." << LOG_ENDL;
        return;
    }
    else if(m_AudioController.playerMode() == MP::USB2 && m_EngineMain.isTAScanComplete(eUSB_REAR) == false) {
        MP_LOG << "USB2 is running TA Scan. return..." << LOG_ENDL;
        return;
    }
    // } added by kihyung 2013.4.3

    // { modified by sangmin.seol 2014.03.11 ITS 0225385 H/U Tune Key Cancel Fix
    //m_AudioController.cancelFFRW(); // modified by Dmitry 23.04.13
    if (!m_AudioController.cancelFFRW())
    {
        emit cancelPressedControlCue();
    }
    // } modified by sangmin.seol 2014.03.11 ITS 0225385 H/U Tune Key Cancel Fix

    //{added by edo.lee 2013.07.02 ITS 0176308
    bool bFrontAudio = false;
    if(m_EngineMain.GetViewR(DISPLAY_FRONT) == APP_MEDIA_MODE_AUDIO && !m_EngineMain.getIsSwapEnabled()
            || m_EngineMain.GetViewR(DISPLAY_REAR) == APP_MEDIA_MODE_AUDIO && m_EngineMain.getIsSwapEnabled())
        bFrontAudio = true;
    //}added by edo.lee 2013.07.02

    switch ( eventId )
    {
    // { added by junam 2012.12.10 for CR16482
    case EVT_KEY_HU_JOG_DIAL_ENCODER:
        if(m_AudioController.GetMediaMode() == MP::BLUETOOTH && m_AudioController.isPlayerActive())
        {
            OSDManager::instance()->displayOSD(OSD_EVENT_BT_UNSUPPORTED);
            break;
        }
        //} added by hyochang.ryu 20130829

        emit menuOptionClose(); //added by edo.lee 2013.07.12
        
        if(bFrontAudio && focusIndex() && //add focusIndex by junam 2013.12.16 for ITS_ME_215036
                ((m_stateUI == "listView"  && m_AudioController.GetListViewModel()->getPopType() < 0 )
                || ( !m_AudioController.isBasicView() && !m_AudioController.isListView()) )) // added by sangmin.seol 2013.11.10 ITS-0207999 send signalTuneNavigation signal to coverflowview for change focus to content-view if focused in modearea
        {
            if( (int)aEvent.GetCommand() > 0 )
            {
                emit signalTuneNavigation(AppEngineQMLConstants::JOG_WHEEL_RIGHT,
                                          AppEngineQMLConstants::KEY_STATUS_PRESSED); // modified by Dmitry 21.05.13
            }
            else
            {
                emit signalTuneNavigation(AppEngineQMLConstants::JOG_WHEEL_LEFT,
                                          AppEngineQMLConstants::KEY_STATUS_PRESSED); // modified by Dmitry 21.05.13
            }
            break;
        }

    case EVT_KEY_CCP_JOG_DIAL_ENCODER:
    case EVT_KEY_RRC_JOG_DIAL_ENCODER:
        //{ modified by yongkyun.lee 2013-08-27 for : ITS186692
        if(m_AudioController.GetMediaMode() == MP::BLUETOOTH && m_AudioController.isPlayerActive())
        {
            OSDManager::instance()->displayOSD(OSD_EVENT_BT_UNSUPPORTED);
            break;
        }
        //} modified by yongkyun.lee 2013-08-27
        //{added by junam 2013.06.28 for ISV86231
        if(m_bTuneEnabled == false)
            break;
        //}added by junam
        if ( m_AudioController.tuneWheel( aEvent.GetCommand() >= 0) )
        {
            m_tuneSelectTimer->start();
            m_tuneDoublePressTimer->start();
        }
        break;

    case EVT_KEY_CCP_JOG_DIAL_CENTER:
    case EVT_KEY_RRC_JOG_DIAL_CENTER: // added by junggil 2012.07.06 for CR11679 : [HMC] [0703_2] Not working RRC operation when turning or tapping RRC jog.
    {
        //{ modified by yongkyun.lee 2013-10-10 for : ITS 193552
        //} added by hyochang.ryu 20130829
        if(isForeground() && m_stateUI != "listView" && m_AudioController.GetListViewModel()->getPopType() < 0 && m_AudioController.playerMode() != MP::BLUETOOTH) //modified by edo.lee 2013.03.22
            emit setDisplayOSD(true);
        // } 	added by edo.lee 2013.01.31
    }
    case EVT_KEY_HU_TUNE_PRESSED:
        if(bFrontAudio && focusIndex() //add focusIndex by junam 2013.12.16 for ITS_ME_215036
                && m_stateUI == "listView" && m_AudioController.GetListViewModel()->getPopType() < 0)
        {
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T *pData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                if ( (KEY_STATUS_T)pData->keyStatus == KEY_STATUS_RELEASED)
                {
                    emit signalTuneReleased();
                }//{added by junam 2013.07.12 for ITS_177486
                else if((KEY_STATUS_T)pData->keyStatus == KEY_STATUS_PRESSED)
                {
                    emit signalTunePressed();
                }//}added by junam
                // { DUAL_KEY added for TunePressed cancel.
                else if((KEY_STATUS_T)pData->keyStatus == KEY_STATUS_CANCEL)
                {
                    emit signalTuneCanceled();
                }
                // } DUAL_KEY added for TunePressed cancel.
            }
			//added by aettie.ji for its 191556 20130925
            if (m_tuneSelectTimer->isActive())
            {
                m_tuneSelectTimer->stop();
                onTuneSelectTimer();
            }
        }
        else if((m_stateUI == "listView" || m_stateUI == "mltView" )  && m_AudioController.GetListViewModel()->getPopType() < 0)
        {
            // modified by Dmitry 25.10.13 for ITS0197279
            // { modified by honggi.shin 2013.11.10 for ITS0207798, show OSD if no selection.
            KEY_EVENT_DATA_T *pData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            if(pData->keyStatus == KEY_STATUS_RELEASED)
            {
                if (!selectTune() )
                {
                    if(m_AudioController.playerMode() == MP::BLUETOOTH)
                        m_AudioController.showCurrentBTOSD();
                    // { modified by cychoi 2014.03.26 for ITS 213525, 213526, 213527 CDDA OSD on language change
                    else if(m_AudioController.playerMode() == MP::DISC)
                        m_AudioController.showCurrentDiscOSD();
                    else
                        showCurrentOSD();
                    // } modified by cychoi 2014.03.26
                }
            }
            // } modified by honggi.shin 2013.11.10 END
        }
        else
        {
            // { modified by yungi 2013.2.18 for No CR Push Tune Not Displayed OSD
            KEY_EVENT_DATA_T *pData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
            // { modified by yungi 2013.3.06 for No CR Push Tune Not Displayed OSD
            if( !aEvent.Data().isEmpty() )
            {
                KEY_EVENT_DATA_T *pData = (KEY_EVENT_DATA_T*)aEvent.Data().data();
                if ( (KEY_STATUS_T)pData->keyStatus == KEY_STATUS_RELEASED && m_tuneSelectTimer->isActive() )
                {
                    emit signalTuneReleased();
                }
                else if((KEY_STATUS_T)pData->keyStatus == KEY_STATUS_PRESSED && m_tuneSelectTimer->isActive())
                {
                    emit signalTunePressed();
                }
                // { DUAL_KEY added for TunePressed cancel.
                else if((KEY_STATUS_T)pData->keyStatus == KEY_STATUS_CANCEL && m_tuneSelectTimer->isActive())
                {
                    emit signalTuneCanceled();
                }
                // } DUAL_KEY added for TunePressed cancel.
            }
            if(pData->keyStatus == KEY_STATUS_RELEASED &&
                    eventId != EVT_KEY_CCP_JOG_DIAL_CENTER  && eventId != EVT_KEY_RRC_JOG_DIAL_CENTER )
                // { modified by yungi 2013.3.06 for No CR Push Tune Not Displayed OSD
            {
                //emit setDisplayOSD(true); //added by yungi 2012.11.06 for No CR - Not display OSD, when any mode tune knob push
                if (!selectTune() /*&& m_AudioController.GetListViewModel()->getPopType() < 0*/)  // Modified by sangmin.seol 2014.03.25 for ITS 0227774 Call showCurrentOSD even if popup showing.
                {
                    if(m_AudioController.playerMode() == MP::BLUETOOTH)
                        m_AudioController.showCurrentBTOSD();
                    // { modified by cychoi 2014.03.26 for ITS 213525, 213526, 213527 CDDA OSD on language change
                    else if(m_AudioController.playerMode() == MP::DISC)
                        m_AudioController.showCurrentDiscOSD();
                    else if((m_AudioController.playerMode() == MP::IPOD1||m_AudioController.playerMode() == MP::IPOD2) && m_tuneDoublePressTimer->isActive())
                    {
                        //MP_TRANSITION << "m_tuneDoublePressTimer->isActive()" << LOG_ENDL; //added by WJL 2014.04.15 for ITS 228616
                    }
                    else if((m_AudioController.playerMode() == MP::IPOD1||m_AudioController.playerMode() == MP::IPOD2) && m_AudioController.isBasicControlEnableStatus() == false)
                    {
                        //MP_TRANSITION << "showCurrentIPOD3rdAppOSD" << LOG_ENDL;
                        m_AudioController.showCurrentIPOD3rdAppOSD(); //added by WJL 2014.04.15 for ITS 229118
                    }
                    else
                        showCurrentOSD();
                    // } modified by cychoi 2014.03.26
                    //emit setDisplayOSD(true);
                    //{deleted by yungi 2012.12.07 for CR 16259
                    //if(m_mode != MP::DISC) // added by junam 2012.11.28 for CR14466
                    //{
                    //emit updateOSD(OSD_EVENT_AUDIO_TRACK_INFO); //removed by edo.lee 2013.01.08
                    //}
                    //}deleted by yungi 2012.12.07 for -CR 16259
                }

            }
            //added by edo.lee 2013.04.01
            //if(pData->keyStatus == KEY_STATUS_RELEASED)
            //	m_EngineMain.setUnMuteState(true); //remove by edo.lee 2013.04.20 for Mute
            //added by edo.lee 2013.04.01
            // } modified by yungi 2013.2.18 for No CR Push Tune Not Displayed OSD
        }
        break;

    default:
        break;
    }
}
// { added by junam 2012.09.12 for CR13632 
bool AppMediaPlayer_EngineAudio::selectTune(void)
{
    MP::DEVICE_T device = m_AudioController.getDevice();

    MP_LOG << device << LOG_ENDL;

    if ( m_tuneSelectTimer->isActive() )
    {
        m_tuneSelectTimer->stop();

        if(device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2)
        {
            //{added by junam 2013.11.28 for ITS_CHN_211039
            if( !m_AudioController.IsFileSupported(m_AudioController.getPendingMedia()))
            {
                onTuneSelectTimer();
                showCurrentOSD();
                if(m_AudioController.isForeground())
                    emit showPopup(LVEnums::POPUP_TYPE_PLAY_UNAVAILABLE_FILE);
                return true;
            }
            //}added by junam
        }

        m_AudioController.switchToTuned();        
        
        //{ removed by junam 2013.11.29 for tune
        //if(device == MP::IPOD1 || device == MP::IPOD2)
        //{
        //    m_AudioController.switchToTuned();
        //}
        //else
        //{
        //    if(m_tunePressDelayTimer->isActive())
        //    {
        //        MP_LOG << "TUNE Timer restart" << LOG_ENDL;
        //        m_tunePressDelayTimer->stop();
        //        m_AudioController.setSavedMedia(m_AudioController.getPendingMedia());
        //        m_tunePressDelayTimer->start();
        //    }
        //    else
        //    {
        //        MP_LOG << "TUNE Timer start" << LOG_ENDL;
        //        m_AudioController.setSavedMedia(m_AudioController.getPendingMedia());
        //        m_tunePressDelayTimer->start();
        //    }
        //}
        //}removed by junam
        return true;
    }
    return false;
}
// } added by junam 

//{ modified by yongkyun.lee 2013-11-06 for : ITS 206866
bool AppMediaPlayer_EngineAudio::isTuneTimer()
{
    return m_tuneSelectTimer->isActive(); // modified by cychoi 2013.11.11 for Prevent fix
}
//} modified by yongkyun.lee 2013-11-06 

// { added by junggil 2012.07.26 for CR11681
void AppMediaPlayer_EngineAudio::onTuneTimerStop()
{
    if ( m_tuneSelectTimer->isActive() )
    {
        m_tuneSelectTimer->stop();
        //{removed by junam 2013.03.10 for ISV73458
        //if ( m_tuneRequestTimer.isActive() )
        //    m_tuneRequestTimer.stop();
        // } removed by junam
        onTuneSelectTimer();
    }
}
// } add by junggil
//added by edo.lee 2013.04.05 78867
//{changed by junam 2013.06.28 for ISV86231
//void AppMediaPlayer_EngineAudio::onTuneTimerStopByMenu()
void AppMediaPlayer_EngineAudio::setTuneEnabled(bool bEnabled)
{
    MP_LOG << "enter ="<< bEnabled << LOG_ENDL;
    m_bTuneEnabled = bEnabled;
    if(bEnabled == false) //}changed by junam
    {
        if ( m_tuneSelectTimer->isActive() )
        {
            m_tuneSelectTimer->stop();
            onTuneSelectTimer();
            //DisplayOSD(true);
            MP_LOG<<"show OSD" << LOG_ENDL;
            //m_EngineMain.GetNotifier()->displayOSD(OSD_EVENT_AUDIO_TRACK_INFO);
            showCurrentOSD();
        }
    }
}

//added by WJL 2014.04.15 for ITS 228616
void AppMediaPlayer_EngineAudio::onTuneDoublePressTimer()
{
    MP_HIGH << "m_tuneDoublePressTimer->stop()" <<LOG_ENDL;
    m_tuneDoublePressTimer->stop();
}

//added by edo.lee 2013.04.05 78867
void AppMediaPlayer_EngineAudio::onTuneSelectTimer()
{
    MP_LOG << "Tumer off" << LOG_ENDL;

    // { changed by junam 2012.8.29 for CR12745
    // { modified by junggil 2012.07.31 for CR12069
    //emit tuneTimerOff(m_AudioController.m_artistTune,m_AudioController.m_titleTune );
    //emit tuneTimerOff( m_AudioController.m_artistTune, m_AudioController.m_titleTune, m_AudioController.m_albumTune, m_AudioController.m_folderTune,
    //                       m_AudioController.m_genreTune, m_AudioController.m_composerTune, m_AudioController.m_coverartTune );
    // } modified by junggil
    // { added by junggil 2012.09.04 for Update DISC info
    if(m_mode == MP::DISC)
    {
        m_AudioController.requestCurrentFolderName();
        //{ modified by yongkyun.lee 2013-09-02 for : tune error
        if(m_AudioController.m_titleTune.isEmpty())
            m_AudioController.m_titleTune = m_AudioController.GetCurrentFileName();
        emit tuneTimerOff( (m_AudioController.m_artistTune), m_AudioController.m_titleTune, m_AudioController.m_albumTune, m_AudioController.m_folderTune, m_AudioController.m_coverartTune ); // modified by wonseok.heo for 2013.06.12 for ITS 146611
        //} modified by yongkyun.lee 2013-09-02
        //}changed by junam
        m_AudioController.setstrUpdatedMediaInfo((m_AudioController.m_artistTune+m_AudioController.m_titleTune+m_AudioController.m_albumTune+m_AudioController.m_folderTune));//{ modified by yongkyun.lee 2013-09-26 for : NOCR- MP3CD Tune 
    }
    else
    {
        MediaInfo info;
        m_AudioController.GetCurrentMediaInfo(&info);
        QString folder = m_AudioController.getFolderName(info.fileName);

        //{changed by junam 2013.11.08 for ITS_KOR_207717
        //emit tuneTimerOff( info.Artist, info.Title, info.Album, folder, info.coverarturl);
        emit tuneTimerOff( info.Artist.isEmpty() ? QString("Unknown Artist") : info.Artist,
                          info.Title.isEmpty() ? QString("Unknown Title") : info.Title,
                          info.Album.isEmpty() ? QString("Unknown Album") : info.Album,
                          folder, info.coverarturl);
        //}changed by junam

        m_AudioController.setstrUpdatedMediaInfo((info.Artist+info.Title+info.Album+folder));
    }
}

void AppMediaPlayer_EngineAudio::onDiscLoaded(bool isReloaded)
{
    MP_LOG << LOG_ENDL;
    // { removed by junam 2014.01.17 deprecated code
    //if (!isReloaded)
    //    m_AudioController.setFirstEntry(MP::DISC, true);
    // } removed by junam
    Q_UNUSED(isReloaded);
}
//{added by aettie 2013.03.19 for Gracenote issue
void AppMediaPlayer_EngineAudio::GracenoteInvalidOnCDTocReceived(bool gracenoteState)
{
    m_bToc = gracenoteState;
}
QString AppMediaPlayer_EngineAudio::GetCDToc()
{
    return m_sToc;
}
bool AppMediaPlayer_EngineAudio::IsGracenoteInvalidOnCDTocReceived()
{
    return m_bToc;
}
//}added by aettie 2013.03.19 for Gracenote issue
void AppMediaPlayer_EngineAudio::onTocReceived(QString toc)
{
    MP_LOG << toc << LOG_ENDL;
    m_sToc = toc; //modified by aettie 20130518
    if (m_pGraceNote)
    {
        m_sToc = toc;
        //{ Modified by Radhakrushna Mohanty CR#12784
        if(m_pGraceNote->isValid())
        {
            m_pGraceNote->DeleteGracenoteMediaDataForCD(m_sToc);  //modified by aettie 20130518
            m_pGraceNote->FetchGracenoteMediaDataForCD(m_sToc);
            GracenoteInvalidOnCDTocReceived(true);  //added by aettie 2013.03.19 for Gracenote issue
        }
        else
        {
            // query to tracker , reboot audio cd case handled.
            MP_LOG<<"GraceNote is invalid , query to tracker " << LOG_ENDL;
            m_AudioController.LoadGracenoteData();
            GracenoteInvalidOnCDTocReceived(false);	//added by aettie 2013.03.19 for Gracenote issue
        }
        //} Modified by Radhakrushna Mohanty CR#12784

    }
}

void
AppMediaPlayer_EngineAudio::LoadSettings()
{
    // Do nothing. We load settings only once on player start-up.
}

void
AppMediaPlayer_EngineAudio::Restore(MODE_STATE_T mode)
{
    MP_LOG << LOG_ENDL;
    // modified by ruindmby 2012.08.09 for CR 12782
    if (MODE_STATE_DISC_AUDIO == mode && MP::DISC_NONE == m_AudioController.getDiscType())
    {
        m_AudioController.SetDiscPostponeRestore(true);
    }
    else
    {
        m_AudioController.setPlayerActive(true);

        MP::DEVICE_T target = AudioUtils::DeviceType(mode);

        if (target != MP::UNDEFINED)
        {
            m_AudioController.SetMediaMode(target);

            if(MP::DISC != m_mode) // added by junam 2012.11.28 for CR14466
            {
                emit updateOSD(OSD_EVENT_AUDIO_TRACK_INFO);
            }

        }
        setAudioOff(false); // added by wspark 2012.10.24 for CR14406
    }
    // modified by ruindmby 2012.08.09 for CR 12782

}

// modified by kihyung 2013.08.09 for ITS 0183077
void
AppMediaPlayer_EngineAudio::SaveSettings(bool bSync)
{
    AppMediaPlayer_SettingsStorage::GetInstance()->SaveAudioSettings(&m_AudioController); //modified by jaehwan.lee 2014.05.23 to fix "data load fail"
    // removed by kihyung 2013.11.22 for ITS 0210669
    // after acc off/on, jukebox always play the first file.
    // AppMediaPlayer_SettingsStorage::GetInstance()->SavePlaylist(&m_AudioController, bSync); 
    AppMediaPlayer_SettingsStorage::GetInstance()->SaveUSBAudioExtended(&m_AudioController, bSync); // modified by kihyung 2013.07.20 for ITS 0179774
}

void
AppMediaPlayer_EngineAudio::onDeviceAttached(MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType, bool autoplay)
{
    if ( aType != MEDIA_TYPE_MUSIC ) return;

    MP_LOG << "aSource =" << aSource << "autoplay =" << autoplay << LOG_ENDL;
    
    // emit closeListView(); // removed by kihyung 2013.06.29 for ITS 0176298

    if(autoplay) emit setDisplayOSD(true); // added by yungi 2011.11.07 for AVP display the OSD track info when user inserts the USB.
    
    switch (aSource)
    {
    case MEDIA_SOURCE_USB1:
        m_AudioController.handleUSBPluggedIn(
                    m_EngineMain.GetMediaManager()->GetMountPath(aSource),
                    m_AudioController.isForeground(),
                    eUSB_PORT1,
                    m_EngineMain.GetMediaManager()->GetID(aSource));
        break;

    case MEDIA_SOURCE_USB2:
        m_AudioController.handleUSBPluggedIn(
                    m_EngineMain.GetMediaManager()->GetMountPath(aSource),
                    m_AudioController.isForeground(),
                    eUSB_PORT2,
                    m_EngineMain.GetMediaManager()->GetID(aSource));
        break;

    case MEDIA_SOURCE_IPOD1:
        m_AudioController.handleIPODPluggedIn(
                    m_EngineMain.GetMediaManager()->GetMountPath(aSource),
                    m_AudioController.isForeground(),
                    eUSB_PORT1,
                    m_EngineMain.GetMediaManager()->GetID(aSource));
        break;

    case MEDIA_SOURCE_IPOD2:
        m_AudioController.handleIPODPluggedIn(
                    m_EngineMain.GetMediaManager()->GetMountPath(MEDIA_SOURCE_IPOD2),
                    m_AudioController.isForeground(),
                    eUSB_PORT2,
                    m_EngineMain.GetMediaManager()->GetID(MEDIA_SOURCE_IPOD2));
        break;

    case MEDIA_SOURCE_CDDA:
        //{ added by yongkyun.lee 20130416 for :  CDDA OSD + GraceNote
		//deleted by aettie 20131010 unused code
        //m_bGracenoteIndexingStatus_DISC = false;
        m_AudioController.SetCDDAGraceNote(GRACENOTE_INDEXING_READY);
        //} added by yongkyun.lee 20130416
    case MEDIA_SOURCE_MP3:
    case MEDIA_SOURCE_DVDAUDIO:
        m_AudioController.handleCDPluggedIn(aSource);
        break;

    case MEDIA_SOURCE_JUKEBOX:
        m_AudioController.handleJukeboxDefault();
        break;

        // added by hyochang.ryu 20130717 for ITS180465
    case MEDIA_SOURCE_AUX1:
    case MEDIA_SOURCE_AUX2:
        //m_AudioController.onResourceGranted();		//rollback 20130723
        //m_AudioController.setAudioPathRPM();		//added by hyochang.ryu 20130723
        m_AudioController.handleAUXPluggedIn();	//added by hyochang.ryu 20130802 for ISV88443
        break;
        //} added by hyochang.ryu 20130717 for ITS180465
    default:
        break;
    }
}
// { added by wonseok.heo for ITS 205774 2013.11.01
void
AppMediaPlayer_EngineAudio::oncatreeRemoved() 
{
    m_AudioController.catreeRelease(); 
}
// } added by wonseok.heo for ITS 205774 2013.11.01

void
AppMediaPlayer_EngineAudio::onDeviceRemoved( MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType )
{
    if ( aType != MEDIA_TYPE_MUSIC ) return;
    MP_LOG << "aSource =" << aSource << LOG_ENDL;
    emit setDisplayOSD(true); // added by yungi 2011.11.08 for AVP display the OSD track info when user inserts the USB.
    MP::DEVICE_T removedDevice = AudioUtils::DeviceType(aSource);


    // { removed by junam 2012.12.05 for moving to activateTab() function
    // { modified by eugeny.novikov 2012.10.11 for CR 14229
    //{ Modified by Radhakrushna 20121009 cr 14261
    //emit handleDeviceRemoved(removedDevice);
    //emit closeListView();
    //} Modified by Radhakrushna 20121009 cr 14261
    // } modified by eugeny.novikov
    // } removed by junam

    //remove by edo.lee 2013.01.10
    //QString info = "";
    //info = "@OP^CLEAR" ;
    //MP_LOG<<"OSD String "<<OSDview<<info << LOG_ENDL;
    //if(OSDview)
    //	OSDview->setWindowTitle(info);
    // } modified by edo.lee
    //ClearOSD(); //remove by edo.lee 2013.01.11

    switch (removedDevice)
    {
    case MP::USB1:
    case MP::USB2:
        if( removedDevice == MP::USB1)
        {
            onSetStatusDataByIndex (removedDevice, DATA_TYPE_USB1_MUSIC_COUNT , 0);
        }
        else
        {
            onSetStatusDataByIndex (removedDevice, DATA_TYPE_USB2_MUSIC_COUNT , 0);
        }
        onSetStatusDataByIndex (removedDevice, DATA_TYPE_DEVICE_STATUS, CONNECT_STATUS_NOT_CONNECTED);
        m_AudioController.handleUSBPluggedOut(removedDevice);
        //VRTrackerChanged(true,(int)VR_BIT_DISCONNECT,removedDevice);   //{ deleted by yongkyun.lee 20130206 for : NO VR: VR is not using Mining  sig
        break;

    case MP::DISC:
        onSetStatusDataByIndex (MP::DISC, DATA_TYPE_DEVICE_STATUS, CONNECT_STATUS_NOT_CONNECTED);
        if (m_pGraceNote)
        {
            m_pGraceNote->DeleteGracenoteMediaDataForCD(m_sToc);
        }
		//deleted by aettie 20131010 unused code
        //emit endGracenoteIndex(MP::DISC); //added by aettie.ji 20130904 for gracenote logo
        m_AudioController.onCDEjected();
        m_MP3ListView = false;// added by yongkyun.lee 20130413 for : NO CR MP3 List
        break;

    case MP::IPOD1:
    case MP::IPOD2:
        m_AudioController.handleIPODPluggedOut(removedDevice);
        //VRTrackerChanged(true,(int)VR_BIT_DISCONNECT,removedDevice);//{ deleted by yongkyun.lee 20130206 for : NO VR: VR is not using Mining  sig.
        break;

        // added by minho 20120910
        // { for CR13503 AUX tab is showing in source row of play back after switching off the AUX
    case MP::AUX:
        m_AudioController.handleAUXPluggedOut(removedDevice);
        break;
        // } added by minho

    default:
        break;
    }

    //setLastAVMode(MODE_STATE_NONE); // remove by edo.lee 2013.08.23
}

void
AppMediaPlayer_EngineAudio::clearTemps()
{
    m_AudioController.clearTemps();
}
void AppMediaPlayer_EngineAudio::OniPodPlaying(int portNum, QString devName, bool connectStatus )
{
    MP_LOG << "portNum=" << portNum << "devName=" << devName << "connectStatus=" << connectStatus << LOG_ENDL;

    EVENT_IPOD_STATE_CHG_INFO_T stEventData;
    memset( &stEventData, 0x00, sizeof(stEventData));
    
    stEventData.iPortNo = portNum;
    
    Event evtStateChg( EVT_POST_EVENT,
                       GetThisAppId(),
                       APP_BTPHONE,
                       EVT_BT_MUSIC_SHUFFLE_ON,
                       QByteArray::fromRawData((char*)&stEventData, sizeof(stEventData)));
    
    emit NotifyUISH (evtStateChg);

}
//modified by edo.lee 2013.03.30

#ifdef SIRI_TIMEOUT_TEST
void AppMediaPlayer_EngineAudio::SendToBTDefaultMusicApp(MP::DEVICE_T source, bool bEnable, QString stAppName)
{
    MP_LOG << " source = " << source << ", bEnable = " << bEnable << ", stAppName = " << stAppName << LOG_ENDL;

    EVENT_IPOD_DEFAULT_MUSIC_APP_T  TEventData;
    memset( &TEventData, 0x00, sizeof(TEventData));

    switch( source )
    {
    case MP::IPOD1 :
        TEventData.iPortNum = 1;
        break;
    case MP::IPOD2 :
        TEventData.iPortNum = 2;
        break;
    default :
        TEventData.iPortNum = 0;
        break;
    }

    TEventData.bIsDefaultMusicApp = bEnable;
    memcpy( &TEventData.cMusicAppName, stAppName.toUtf8().data(), stAppName.length());

    Event evtDefaultMusicApp( EVT_POST_EVENT,
                              GetThisAppId(),
                              APP_BTPHONE,
                              EVT_BT_MUSIC_SHUFFLE_OFF,
                              QByteArray::fromRawData((char*)&TEventData, sizeof(TEventData)));

    emit NotifyUISH (evtDefaultMusicApp);
}
#endif

//modified by aettie 20130620 for back key event
void
AppMediaPlayer_EngineAudio::HandleSoftBackKey(bool isFront, bool bRRC) 
{
    MP_LOG << LOG_ENDL;
    DISPLAY_T appDisp = DISPLAY_FRONT;
    if(!isFront) appDisp = DISPLAY_REAR;
    MP_LOG<<"Back_key : EVT_SOFT_BACK_KEY m_isBackPressByJog"<<m_isBackPressByJog << LOG_ENDL;
    MP_LOG<<"Back_key : EVT_SOFT_BACK_KEY m_isFrontSoftBackPressed"<<m_isFrontSoftBackPressed << LOG_ENDL;
    MP_LOG<<"Back_key : EVT_SOFT_BACK_KEY m_isRearSoftBackPressed"<<m_isRearSoftBackPressed << LOG_ENDL;
    /*if(m_isBackPressByJog)
    {
        if(m_isFrontSoftBackPressed)
        {
            appDisp = DISPLAY_FRONT;
            m_isFrontSoftBackPressed = false;
                    MP_LOG<<"Back_key : CCP EVT_SOFT_BACK_KEY m_isBackPressByJog"<<appDisp << LOG_ENDL;

        }else if(m_isRearSoftBackPressed)
        {
            appDisp = DISPLAY_REAR;
            m_isRearSoftBackPressed = false;
                    MP_LOG<<"Back_key : RRC EVT_SOFT_BACK_KEY m_isBackPressByJog"<<appDisp << LOG_ENDL;

        }*/
    MP_LOG<<"Back_key : bRRC= "<<bRRC << LOG_ENDL;

    if(m_isBackPressByJog)
    {
        if(!bRRC)
        {
            appDisp = DISPLAY_FRONT;
            m_isFrontSoftBackPressed = false;
            MP_LOG<<"Back_key : CCP EVT_SOFT_BACK_KEY m_isBackPressByJog"<<appDisp << LOG_ENDL;
        }
        else if(bRRC)
        {
            appDisp = DISPLAY_REAR;
            m_isRearSoftBackPressed = false;
            MP_LOG<<"Back_key : RRC EVT_SOFT_BACK_KEY m_isBackPressByJog"<<appDisp << LOG_ENDL;
        }
        m_isBackPressByJog = false;

        Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_UI_MANAGER );
        emit NotifyUISH (event, appDisp);
        // removed by cychoi 2013.12.29 for ITS 217565
    }
    else
    {
        MP_LOG<<"Back_key : EVT_TOUCH_BACK_KEY appDisp = "<<appDisp << LOG_ENDL;
        Event event( EVT_TOUCH_BACK_KEY, GetThisAppId(), APP_UI_MANAGER );
        emit NotifyUISH (event, DISPLAY_FRONT);
    }
}
//modified by aettie 20130620 for back key event
//modified by edo.lee 2013.03.30
//deleted by aettie 20131010 unused code
//void
//AppMediaPlayer_EngineAudio::SetGracenoteVolumeType(int type, QString id, QString path)
//{
//    if (m_pGraceNote)
//    {
//        MP_LOG << type << LOG_ENDL;
//        m_pGraceNote->EnableMLTForVolume(type, id, path);
//    }
//}

//void
//AppMediaPlayer_EngineAudio::onSetGracenoteDeviceStateChanged(int volume, QString id, QString path,
//                                                             int startIndex, int endIndex, int syncType, bool isMounted)
//{
//    MP_LOG <<"aettiequeue = onSetGracenoteDeviceStateChanged = "<< volume << LOG_ENDL;
    //temporally deleted for gracenote indexing spec out 2013 07 26
    /* if (m_pGraceNote)
    {
        if (volume == MP::USB1 || volume == MP::USB2)
        {
            if (isMounted)
            {
                m_pGraceNote->USBConnected(id, path, volume);
            }
            else
            {
                m_pGraceNote->USBDisConnected(id, volume);
            }
        }
        //deleted by aettie.ji 2013.02.04 for ISV 61842,61749
       //else if (volume == MP::IPOD1 || volume == MP::IPOD2)
      //  {
        //    if (isMounted)
          //  {
            //    m_pGraceNote->IPODConnected(id, startIndex, endIndex, syncType, volume);
            //}
            //else
           // {
             //   m_pGraceNote->IPODDisConnected(id, volume);
           // }
       // }
        //deleted by aettie.ji 2013.02.04 for ISV 61842,61749
    }*/
    //temporally deleted for gracenote indexing spec out 2013 07 26
//}

void
AppMediaPlayer_EngineAudio::onSetGracenoteHighlightedVolume(int volume, QString id, QString path)
{
    if (m_pGraceNote)
    {
        m_pGraceNote->HighLightedVolume(volume, id, path);
        //{added by aettie 2013.03.19 for Gracenote issue
        if(volume==MP::DISC){
            if(!IsGracenoteInvalidOnCDTocReceived()){
                m_pGraceNote->DeleteGracenoteMediaDataForCD(GetCDToc());  //modified by aettie 20130518
                m_pGraceNote->FetchGracenoteMediaDataForCD(GetCDToc());
                GracenoteInvalidOnCDTocReceived(true); //added for Gracenote cd toc searching 2013 07 26
            }
        }
        //}added by aettie 2013.03.19 for Gracenote issue
    }
}
//{ deleted by yongkyun.lee 20130206 for : NO VR: VR is not using Mining  sig.
/*// { modified by  yongkyun.lee 2012.11.21 for  xSampa check.         
//void AppMediaPlayer_EngineAudio::onXSampaDataUpdated()
//{
//    MP_LOG << LOG_ENDL;
//
//    EVT_PUBLIC_ID_T evtType = EVT_XSAMPA_AVAILABLE;
//
//    Event event( evtType, GetThisAppId(), APP_VR, EDATA_CREATE_TYPE_SEARCH_MUSIC );
//    emit NotifyUISH( event );
//}
void AppMediaPlayer_EngineAudio::onXSampaDataUpdated(int deviceType)
{
    MP_LOG<<"[VR]" << LOG_ENDL;

    EVT_PUBLIC_ID_T evtType = EVT_XSAMPA_AVAILABLE;
    // { added  by  yongkyun.lee 2012.11.21 for  xSampa check.
    MP::DEVICE_T deviceT = (MP::DEVICE_T)DeviceConverter(deviceType);
    MiningMask( deviceT ,VR_BIT_XSMAPA);
    sendVRTrackerChanged( true , deviceT, true);
    // } added  by  yongkyun.lee

    Event event( evtType, GetThisAppId(), APP_VR, EDATA_CREATE_TYPE_SEARCH_MUSIC );
    emit NotifyUISH( event );
}
// { modified by  yongkyun.lee
*/
//} deleted by yongkyun.lee 20130206

// { added by kihyung 2012.08.22 for new UX MUSIC(LGE) #3


//added by aettie for Gracenote killed handling
//{modified by aettie 20130605 for Gracenote restart
void AppMediaPlayer_EngineAudio::onGracenoteKilled()
{
    MP_LOG <<  "GracenoteKilled "  << LOG_ENDL;
	//deleted by aettie 20131010 unused code
    //emit killedGracenote();
    MP_LOG <<  "sleep(2)"  << LOG_ENDL;
    sleep(2);
    MP_LOG <<  "AppEngineBase::GetCountryVariant() = " <<AppEngineBase::GetCountryVariant() << LOG_ENDL;
    
    // Start gracenot application. pass country variant as parameter
    ECountryVariant ucCountVariant = AppEngineBase::GetCountryVariant();
    QString countryVariantArg = ( ucCountVariant != eCVInvalid ) ? QString( KCountryVariantId ).arg( ucCountVariant ) : "";
    MP_LOG <<  "countryVariantArg = " <<countryVariantArg << LOG_ENDL;

    // -- Grancenote plugin RElaunch with supprate process --
    QString path = QString( "/app/bin/DHAVN_GracenotePluginProcess" );
    MP_LOG <<  "countryVariantArg = " <<path << LOG_ENDL;
    QStringList argList;
    argList << countryVariantArg;
    m_AudioController.StartProcessWithQProcess( path, argList );
}
//}modified by aettie 20130605 for Gracenote restart

//{modified by aettie.ji 20130904 for gracenote logo
void AppMediaPlayer_EngineAudio::onGracenoteIndexingStatus(bool status, int deviceType)
{
    MP_LOG << status << " " << deviceType << LOG_ENDL;
	//deleted by aettie 20131010 unused code
    //if(status == true) {
    //    if(deviceType==MP::DISC) m_bGracenoteIndexingStatus_DISC = true;

    //    MP_LOG <<"emit startGracenoteIndex(deviceType) = "<< deviceType << LOG_ENDL;

     //emit startGracenoteIndex(deviceType);
    //}
    //else {
        if(deviceType==MP::DISC){
            if(!status)
                m_AudioController.SetCDDAGraceNote(GRACENOTE_INDEXING_READY); //CDDA OSD + GraceNote
            //m_bGracenoteIndexingStatus_DISC = false;
        }

    //}
}
//}modified by aettie.ji 20130904 for gracenote logo
//deleted by aettie 20131010 unused code
//{added by aettie 2013.03.19 for Gracenote issue
/*bool AppMediaPlayer_EngineAudio::getGracenoteIndexingStatus(int devType)
{
    switch (devType){
    case MP::JUKEBOX:
        return m_bGracenoteIndexingStatus_Jukebox;
    case MP::USB1:
        return m_bGracenoteIndexingStatus_USB1;
    case MP::USB2:
        return m_bGracenoteIndexingStatus_USB2;
    case MP::DISC:
        return m_bGracenoteIndexingStatus_DISC;
    default:
        return false;
    }
} */
//}added by aettie 2013.03.19 for Gracenote issue
// { added by jungae 2012.12.26, for send the connection information to AppBtPhone
void AppMediaPlayer_EngineAudio::onIPodDeviceConnectInfo(int portNum, QString devName, bool connectStatus)
{
    MP_LOG << "portNum=" << portNum << "devName=" << devName << "connectStatus=" << connectStatus << LOG_ENDL;
    
    EVENT_IPOD_CONNECT_INFO_DATA_T pEventData;

    pEventData.iPortNum = portNum;
    /*
     * Modified by jonghwan.cho@lge.com	2013.03.15.
     * for notifying to BTPhone about connected Korean device name
    //sprintf(pEventData.cDeviceName, "%s", devName.toAscii().data());
     */
    //sprintf(pEventData.cDeviceName, "%s", devName.toUtf8().data());
    snprintf(pEventData.cDeviceName, sizeof(pEventData.cDeviceName), "%s", devName.toUtf8().data());     // modified by sungha.choi 2013.07.15 for static test

    pEventData.bConnectStatus = connectStatus;

    Event postEvent(EVT_POST_EVENT, GetThisAppId(), APP_BTPHONE,
                    QByteArray((char*)&pEventData, sizeof(pEventData))); // modified by lssanh 2013.02.21 IPC payload
    //{changed by junam 2013.08.07 for ipod connect notification signal
    //emit NotifyUISH( postEvent );
    m_EngineMain.notifyUISH(postEvent);
    //}changed by junam


    
    // { added by jungae 2013.02.27, for send the connection information to AppRadio, AppXMAudio
    Event postEventXM(EVT_POST_EVENT, GetThisAppId(), APP_XM_AUDIO,
                      QByteArray((char*)&pEventData, sizeof(pEventData)));
    //{changed by junam 2013.08.07 for ipod connect notification signal
    //emit NotifyUISH( postEventXM );
    m_EngineMain.notifyUISH(postEventXM);
    //}changed by junam
    
    Event postEventHD(EVT_POST_EVENT, GetThisAppId(), APP_RADIO,
                      QByteArray((char*)&pEventData, sizeof(pEventData)));
    //{changed by junam 2013.08.07 for ipod connect notification signal
    //emit NotifyUISH( postEventHD );
    m_EngineMain.notifyUISH(postEventHD);
    //}changed by junam

    // } added by jungae
    
}
// } added by jungae

void
AppMediaPlayer_EngineAudio::LaunchHMISettings (bool isFront)//modified by edo.lee 2013.05.06
{
    MP_LOG << LOG_ENDL;
    // { modified by kihyung 2013.1.26
#if USE_NEW_SETTING_ENTRY_POINT
    APP_EVENT_DATA_BASE_T pEventData;
    pEventData.sIsJogEnabled = false;

    Event softEvent(EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER, eAPP_SETTINGS_SOUND,
                    QByteArray((char *)&pEventData, sizeof(pEventData))); // modified by lssanh 2013.02.21 IPC payload
    //emit NotifyUISH(softEvent, m_EngineMain.getCurrScreen());
    emit NotifyUISH(softEvent, isFront?DISPLAY_FRONT:DISPLAY_REAR);//modified by edo.lee 2013.05.06
#else
    AppSettingsStartParameter param;
    param.startState = AppSettingsDef::eStart_Mode_Sound;

    Event postEvent(EVT_POST_EVENT, GetThisAppId(), APP_SETTINGS, QByteArray((char*)&param, sizeof(param))); // modified by lssanh 2013.02.21 IPC payload
    Event softEvent(EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER);

    emit NotifyUISH(postEvent);
    emit NotifyUISH(softEvent);
#endif
    // } modified by kihyung 2013.1.26
}

// { added by lssanh 2012.08.24 for audio path
void
AppMediaPlayer_EngineAudio::NofityAudioPathChanged (int id_device)
{
    MODE_STATE_T avState = MODE_STATE_NONE;
    
    //    LOG () <<  "Send Sound path to UISH";//Commented by Alexey Edelev 2012.10.19 CR14799
    //{Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side
    //    //{Changed by Alexey Edelev 2012.10.09 Fix for CR14251
    // { add by yongkyun.lee@lge.com  2012.10.30 : : CR 13733  : Sound Path

    // } add by yongkyun.lee@lge.com
    //     // { add by yongkyun.lee@lge.com  2012.09.25 : : CR 13733  : Sound Path
    //    if( !m_AvModeChangeMutex)
    //    {
    ////        m_AvModeChangeMutex = true;//Commented by Alexey Edelev 2012.10.19 CR14799
    //        return;
    //    }
    ////    m_AvModeChangeMutex = true;//Commented by Alexey Edelev 2012.10.19 CR14799(Code does not make sense)
    //    // } add by yongkyun.lee@lge.com
    //}Changed by Alexey Edelev 2012.10.09 Fix for CR14251
    //}Removed by Alexey Edelev 2012.10.26 This must be fixed on UISH side
    MP_LOG <<  "NofityAudioPathChanged Send Sound path:device=" << id_device << LOG_ENDL;//Added by Alexey Edelev 2012.10.19 CR14799
    switch (id_device)
    {
    case MP::JUKEBOX:
        avState = MODE_STATE_JUKEBOX_AUDIO;
        break;

    case MP::USB1:
        avState = MODE_STATE_USB1_AUDIO;
        break;

    case MP::USB2:
        avState = MODE_STATE_USB2_AUDIO;
        break;

//    case MP::DISC:
//        avState = MODE_STATE_DISC_AUDIO;
//        break;

    case MP::IPOD1:
        avState = MODE_STATE_IPOD1_AUDIO;
        break;

    case MP::IPOD2:
        avState = MODE_STATE_IPOD2_AUDIO;
        break;

//    case MP::AUX:
//        avState = MODE_STATE_AUX1_AUDIO;
//        break;
        //added by edo.lee 2012.09.04 for cr 6401
//    case MP::BLUETOOTH:
//        avState = MODE_STATE_BT_AUDIO;
//        break;
        //added by edo.lee

    default:
        MP_LOG << "Warning! Unknown id" << LOG_ENDL;
        break;
    }


    EVENT_AUDIO_PATH_REQUEST_DATA message = { { APP_ALL, EVT_REQUEST_AUDIO_PATH, 0, 0 }, avState };

    message.header.dataSize = sizeof( message );
    Event event( EVT_REQUEST_AUDIO_PATH, GetThisAppId(),
                 APP_UI_MANAGER, QByteArray( (char*)&message, sizeof( message ) ) ); // modified by lssanh 2013.02.21 IPC payload

    emit NotifyUISH( event );
}
// } added by lssanh 2012.08.24 for audio path

// { changed by eugeny - 12-09-15
void
//AppMediaPlayer_EngineAudio::onSetOptionModel(int id_state)
AppMediaPlayer_EngineAudio::SetOptionModel(QString UIState, bool editSate) // modified by ravikanth 17-05-13 for ISV 83180
{
    MP_LOG << UIState <<" editSate = "<<editSate << LOG_ENDL;

    OPTION_MENU_T* optionsMenuData = NULL;
    int optionsMenuSize = 0;

    int fileOperation = m_AudioController.GetListViewModel()->operation();
    bool copyCompleteMode = m_AudioController.GetListViewModel()->getCopyCompletedMode(); // added by eugene 2012.11.29 for CR 16076
    QString prevUIState = topScreen();
    QString scanOptionStr = (m_AudioController.getScanMode() == SCANOFF) ? "STR_MEDIA_SCAN"
                                                                         : "STR_MEDIA_STOP_SCAN";

    SHUFFLE_MENU[1].bSelected = (m_AudioController.getRandomMode() != RANDOMOFF);
    SHUFFLE_MENU[2].bSelected = (m_AudioController.getRandomMode() == RANDOMOFF);

    // Add To Playlist option
    //JUKEBOX_MAIN_MENU[5].bEnabled = true;  // modified by eugene 2012.12.13 for New UX #12 //deleted by eunhye 2013.2.13 for UX Scenario 13. Search
    USB_MAIN_MENU[5].bEnabled = true; // modified by eugene 2012.12.13 for New UX #12

    emit scrollAccStop(); // added by lssanh 2013.02.26 ISV73837
    if (m_AudioController.GetDeviceHistory(m_mode)->ContentPlayed == MP::FOLDER &&
        (m_AudioController.GetMediaMode() != MP::DISC || m_AudioController.getDiscType() != MP::AUDIO_CD)) // modified by wonseok.heo for ITS 218310 2014.01.07 // modified by shkim 2013.11.12 for CDDA Repeat Mode default error
    {
        REPEAT_MENU_FOLDER[1].bSelected = (m_AudioController.getRepeatMode() == REPEATALL ||
                                           m_AudioController.getRepeatMode() == REPEATOFF);
        REPEAT_MENU_FOLDER[2].bSelected = (m_AudioController.getRepeatMode() == REPEATFOLDER);
        REPEAT_MENU_FOLDER[3].bSelected = (m_AudioController.getRepeatMode() == REPEATFILE);

        SCAN_MENU_FOLDER[1].bSelected = (m_AudioController.getScanMode() == SCANALL);
        SCAN_MENU_FOLDER[2].bSelected = (m_AudioController.getScanMode() == SCANFILE);
    }
    else
    {
        REPEAT_MENU[1].bSelected = (m_AudioController.getRepeatMode() == REPEATALL ||
                                    m_AudioController.getRepeatMode() == REPEATOFF);
        REPEAT_MENU[2].bSelected = (m_AudioController.getRepeatMode() == REPEATFILE);

        if (m_AudioController.GetDeviceHistory(m_mode)->ContentPlayed == MP::ALBUM)
            REPEAT_MENU[1].sText = "STR_MEDIA_REPEAT_ALBUM";
        else if (m_AudioController.GetDeviceHistory(m_mode)->ContentPlayed == MP::PLAYLIST)
            REPEAT_MENU[1].sText = "STR_MEDIA_REPEAT_PLAYLIST";
        else
            REPEAT_MENU[1].sText = "STR_MEDIA_REPEAT_ALL";
    }

    if (m_AudioController.isPlayFromMLT())
    {
        optionsMenuData = MLT_MAIN_MENU;
        optionsMenuSize = sizeof(MLT_MAIN_MENU)/sizeof(OPTION_MENU_T);
    }
    else if (UIState == "listView" || (UIState == "keypad" && prevUIState == "listView"))
    {
        if ((fileOperation == LVEnums::OPERATION_COPY && editSate)   || fileOperation == LVEnums::OPERATION_MOVE ||
                fileOperation == LVEnums::OPERATION_DELETE || fileOperation == LVEnums::OPERATION_ADD_TO_PLAYLIST) // modified by ravikanth 17-05-13 for ISV 83180
        {
            optionsMenuData = LIST_OPERATION_MENU;
            optionsMenuSize = sizeof(LIST_OPERATION_MENU)/sizeof(OPTION_MENU_T);
        }
        // { modified yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
        else if (prevUIState == "jukebox")
            //else if (prevUIState == "jukebox" || copyCompleteMode == true) // modified by eugene 2012.11.29 for CR 16076
            // } modified yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
        {
			// modified for ISV to disable menu options, if list is empty
            if( m_AudioController.GetListViewModel()->getTotalItemCount() == 0)
            {
                JUKEBOX_LIST_VIEW_MENU[1].bEnabled = false;
                JUKEBOX_LIST_VIEW_MENU[2].bEnabled = false;
                JUKEBOX_LIST_VIEW_MENU[3].bEnabled = false;
            }
            else
            {
                JUKEBOX_LIST_VIEW_MENU[1].bEnabled = true;
                JUKEBOX_LIST_VIEW_MENU[2].bEnabled = true;
                JUKEBOX_LIST_VIEW_MENU[3].bEnabled = true;
            }
            optionsMenuData = JUKEBOX_LIST_VIEW_MENU;
            optionsMenuSize = sizeof(JUKEBOX_LIST_VIEW_MENU)/sizeof(OPTION_MENU_T);
        }
        else if (prevUIState == "usb")
        {
            // { modified by ravikanth 13-04-13
            if( m_AudioController.GetListViewModel()->getTotalItemCount() == 0)
            {
                USB_LIST_VIEW_MENU[2].bEnabled = false;
            }
            else
            {
                USB_LIST_VIEW_MENU[2].bEnabled = true;
            }
            // } modified by ravikanth 13-04-13
            optionsMenuData = USB_LIST_VIEW_MENU;
            optionsMenuSize = sizeof(USB_LIST_VIEW_MENU)/sizeof(OPTION_MENU_T);
        }
        else if (prevUIState == "disc")
        {
            optionsMenuData = DISC_LIST_VIEW_MENU;
            optionsMenuSize = sizeof(DISC_LIST_VIEW_MENU)/sizeof(OPTION_MENU_T);
        }
        else if (prevUIState == "ipod")
        {
            //{changed by junam 2013.08.27
            //if(editSate)
            //    IPOD_LIST_VIEW_MENU[2].sText = "STR_MEDIA_CANCEL_EDIT";
            //else
            //    IPOD_LIST_VIEW_MENU[2].sText = "STR_MEDIA_EDIT_CATEGORY";
            optionsMenuData = IPOD_LIST_VIEW_MENU;
            optionsMenuSize = sizeof(IPOD_LIST_VIEW_MENU)/sizeof(OPTION_MENU_T);
            if(editSate)
                optionsMenuSize--;
            //}changed by junam
        }
    }
    else if (UIState == "jukebox")
    {
        // { modified by  yongkyun.lee 2012.10.08 for  More info increment value in Option menu
        //Q_ASSERT(JUKEBOX_MAIN_MENU[9].nId == MP::OPTION_MENU_MORE_INFO);
        //JUKEBOX_MAIN_MENU[9].bSelected = m_AudioController.getShowInfo();
        //Q_ASSERT(JUKEBOX_MAIN_MENU[8].nId == MP::OPTION_MENU_MORE_INFO); //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox MoreInfo deleted
        //JUKEBOX_MAIN_MENU[8].bSelected = m_AudioController.getShowInfo();//deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox MoreInfo deleted
        // } modified by  yongkyun.lee

        if (m_AudioController.GetDeviceHistory(m_mode)->ContentPlayed == MP::FOLDER)
        {
            JUKEBOX_MAIN_MENU[4].vAttachedMenu = REPEAT_MENU_FOLDER; // modified by eugene 2012.12.13 for New UX #12
            JUKEBOX_MAIN_MENU[4].nCountItemAttachedMenu = sizeof(REPEAT_MENU_FOLDER)/sizeof(OPTION_MENU_T); // modified by eugene 2012.12.13 for New UX #12
            if(m_AudioController.getScanMode() == SCANOFF)
            {
                JUKEBOX_MAIN_MENU[5].vAttachedMenu = SCAN_MENU_FOLDER; // modified by eugene 2012.12.13 for New UX #12 //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
                JUKEBOX_MAIN_MENU[5].nCountItemAttachedMenu = sizeof(SCAN_MENU_FOLDER)/sizeof(OPTION_MENU_T); // modified by eugene 2012.12.13 for New UX #12 //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
            }
            else
            {
                JUKEBOX_MAIN_MENU[5].vAttachedMenu = NULL; // modified by eugene 2012.12.13 for New UX #12 //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
                JUKEBOX_MAIN_MENU[5].nCountItemAttachedMenu = 0; // modified by eugene 2012.12.13 for New UX #12 //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
            }
            JUKEBOX_MAIN_MENU[5].sText = scanOptionStr; // modified by eugene 2012.12.13 for New UX #12 //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
        }
        else
        {
            JUKEBOX_MAIN_MENU[4].vAttachedMenu = REPEAT_MENU; // modified by eugene 2012.12.13 for New UX #12
            JUKEBOX_MAIN_MENU[4].nCountItemAttachedMenu = sizeof(REPEAT_MENU)/sizeof(OPTION_MENU_T); // modified by eugene 2012.12.13 for New UX #12
            JUKEBOX_MAIN_MENU[5].vAttachedMenu = NULL; // modified by eugene 2012.12.13 for New UX #12 //modified by eunhye 2013.2.13 for UX Scenario 13. Search
            JUKEBOX_MAIN_MENU[5].nCountItemAttachedMenu = 0; // modified by eugene 2012.12.13 for New UX #12 //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
            JUKEBOX_MAIN_MENU[5].sText = scanOptionStr; // modified by eugene 2012.12.13 for New UX #12 //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
        }
        optionsMenuData = JUKEBOX_MAIN_MENU;
        optionsMenuSize = sizeof(JUKEBOX_MAIN_MENU)/sizeof(OPTION_MENU_T);
    }
    else if (UIState == "usb")
    {
        // { modified by  yongkyun.lee 2012.10.08 for  More info increment value in Option menu
        //Q_ASSERT(USB_MAIN_MENU[10].nId == MP::OPTION_MENU_MORE_INFO);
        //USB_MAIN_MENU[10].bSelected = m_AudioController.getShowInfo();
        //Q_ASSERT(USB_MAIN_MENU[11].nId == MP::OPTION_MENU_MORE_INFO);   deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox MoreInfo deleted
        //USB_MAIN_MENU[11].bSelected = m_AudioController.getShowInfo();  deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox MoreInfo deleted
        // } modified by  yongkyun.lee

        if (m_AudioController.GetDeviceHistory(m_mode)->ContentPlayed == MP::FOLDER)
        {
            USB_MAIN_MENU[4].vAttachedMenu = REPEAT_MENU_FOLDER; // modified by eugene 2012.12.13 for New UX #12
            USB_MAIN_MENU[4].nCountItemAttachedMenu = sizeof(REPEAT_MENU_FOLDER)/sizeof(OPTION_MENU_T); // modified by eugene 2012.12.13 for New UX #12

            if(m_AudioController.getScanMode() == SCANOFF)
            {
                USB_MAIN_MENU[6].vAttachedMenu = SCAN_MENU_FOLDER; // modified by eugene 2012.12.13 for New UX #12 //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
                USB_MAIN_MENU[6].nCountItemAttachedMenu = sizeof(SCAN_MENU_FOLDER)/sizeof(OPTION_MENU_T); // modified by eugene 2012.12.13 for New UX #12 //modified by eunhye 2013.2.13 for UX Scenario 13. Search
            }
            else
            {
                USB_MAIN_MENU[6].vAttachedMenu = NULL; // modified by eugene 2012.12.13 for New UX #12 //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
                USB_MAIN_MENU[6].nCountItemAttachedMenu = 0; // modified by eugene 2012.12.13 for New UX #12 //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
            }
            USB_MAIN_MENU[6].sText = scanOptionStr; // modified by eugene 2012.12.13 for New UX #12 //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
        }
        else
        {
            USB_MAIN_MENU[4].vAttachedMenu = REPEAT_MENU; // modified by eugene 2012.12.13 for New UX #12
            USB_MAIN_MENU[4].nCountItemAttachedMenu = sizeof(REPEAT_MENU)/sizeof(OPTION_MENU_T); // modified by eugene 2012.12.13 for New UX #12
            USB_MAIN_MENU[6].vAttachedMenu = NULL; // modified by eugene 2012.12.13 for New UX #12  //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
            USB_MAIN_MENU[6].nCountItemAttachedMenu = 0; // modified by eugene 2012.12.13 for New UX #12  //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
            USB_MAIN_MENU[6].sText = scanOptionStr; // modified by eugene 2012.12.13 for New UX #12  //modified by yungi 2013.2.4 for UX Scenario 1.Jukebox PlayList //modified by eunhye 2013.2.13 for UX Scenario 13. Search
        }

        optionsMenuData = USB_MAIN_MENU;
        optionsMenuSize = sizeof(USB_MAIN_MENU)/sizeof(OPTION_MENU_T);
    }
    else if (UIState == "disc")
    {
        if (m_AudioController.getDiscType() == MP::DVD_AUDIO)
        {
            DISC_DVD_MAIN_MENU[4].bEnabled = false;//added by eunhye 2013.04.19
            DISC_DVD_MAIN_MENU[5].bEnabled = false;
            DISC_DVD_MAIN_MENU[6].bEnabled = false;

            optionsMenuData = DISC_DVD_MAIN_MENU;
            optionsMenuSize = sizeof(DISC_DVD_MAIN_MENU)/sizeof(OPTION_MENU_T);
        }
        else if (m_AudioController.getDiscType() == MP::MP3_CD)
        {
            // { modified by eugeny.novikov 2012.11.13 for CR 15299
            //DISC_MP3_MAIN_MENU[6].bSelected = m_AudioController.getShowInfo(); //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox MoreInfo deleted

            // { added by junam 2012.09.24 for CR12977
            DISC_MP3_MAIN_MENU[1].bEnabled      = m_MP3ListView; // added by yongkyun.lee 20130413 for : NO CR MP3 List
            DISC_MP3_MAIN_MENU[3].vAttachedMenu = REPEAT_MENU_FOLDER; // modified by eugene 2012.12.13 for New UX #12
            DISC_MP3_MAIN_MENU[3].nCountItemAttachedMenu = sizeof(REPEAT_MENU_FOLDER)/sizeof(OPTION_MENU_T); // modified by eugene 2012.12.13 for New UX #12

            if(m_AudioController.getScanMode() == SCANOFF)
            {
                DISC_MP3_MAIN_MENU[4].vAttachedMenu = SCAN_MENU_FOLDER; // modified by eugene 2012.12.13 for New UX #12
                DISC_MP3_MAIN_MENU[4].nCountItemAttachedMenu = sizeof(SCAN_MENU_FOLDER)/sizeof(OPTION_MENU_T); // modified by eugene 2012.12.13 for New UX #12
            }
            else
            {
                DISC_MP3_MAIN_MENU[4].vAttachedMenu = NULL; // modified by eugene 2012.12.13 for New UX #12
                DISC_MP3_MAIN_MENU[4].nCountItemAttachedMenu = 0; // modified by eugene 2012.12.13 for New UX #12
            }
            DISC_MP3_MAIN_MENU[4].sText = scanOptionStr; // modified by eugene 2012.12.13 for New UX #12
            // } added by junam

            optionsMenuData = DISC_MP3_MAIN_MENU;
            optionsMenuSize = sizeof(DISC_MP3_MAIN_MENU)/sizeof(OPTION_MENU_T);
            // } modified by eugeny.novikov 2012.11.13
        }
        else
        {
            DISC_MAIN_MENU[4].sText = scanOptionStr;// modified by eugene 2012.12.13 for New UX #12
            //DISC_MAIN_MENU[5].bEnabled = true; // OPTION_MENU_MORE_LIKE_THIS // modified by ravikanth 14-07-13 for MLT feature is spec out
            //DISC_MAIN_MENU[7].bSelected = m_AudioController.getShowInfo(); //added by XXXX 2012.11.27 for CR14693 //deleted by yungi 2013.2.4 for UX Scenario 3.Jukebox MoreInfo deleted

            optionsMenuData = DISC_MAIN_MENU;
            optionsMenuSize = sizeof(DISC_MAIN_MENU)/sizeof(OPTION_MENU_T);
        }
    }
    else if (UIState == "ipod")
    {
        //{added by junam 2013.07.23 for ITS_KOR_181304
        if(m_AudioController.isBasicControlEnableStatus() == false)
        {
            optionsMenuData = IPOD_MUSC_APP_MENU;
            optionsMenuSize = sizeof(IPOD_MUSC_APP_MENU)/sizeof(OPTION_MENU_T);
        }
        else //}added by junam
        {
            //{chagned by junam 2013.07.18 for flowview disable
            //IPOD_MAIN_MENU[5].sText = scanOptionStr; // modified by eugene 2012.12.13 for New UX #12 //modified by eunhye 2013.2.13 for UX Scenario 13. Search
            for(int i=0; i < sizeof(IPOD_MAIN_MENU)/sizeof(OPTION_MENU_T); i++)
            {
                if(IPOD_MAIN_MENU[i].nId == MP::OPTION_MENU_SCAN)
                {
                    IPOD_MAIN_MENU[i].sText = scanOptionStr;
                    break;
                }
            }
            //}changed by junam

            optionsMenuData = IPOD_MAIN_MENU;
            optionsMenuSize = sizeof(IPOD_MAIN_MENU)/sizeof(OPTION_MENU_T);
        }
    }
    else if (UIState == "bluetooth")
    {
        optionsMenuData = BLUETOOTH_MENU;
        optionsMenuSize = sizeof(BLUETOOTH_MENU)/sizeof(OPTION_MENU_T);
    }
    // { added by wspark 2012.09.14 for New UX, AUX
    else if (UIState == "aux")
    {
        optionsMenuData = AUX_AUDIO_MENU;
        optionsMenuSize = sizeof(AUX_AUDIO_MENU)/sizeof(OPTION_MENU_T);

    }
    // } added by wspark
    // added by minho 20121221 for CR16795 Add to show up Chinese keypad popup in option menu
    else if (UIState == "searchView")
    {
        optionsMenuData = KEYPAD_AUDIO_MENU;
        optionsMenuSize = sizeof(KEYPAD_AUDIO_MENU)/sizeof(OPTION_MENU_T);
    }
    // added by minho 20121221

    if (m_OptionMenuModel)
    {
        m_OptionMenuModel->updateModel(optionsMenuData, optionsMenuSize);
    }
    else
    {
        setOptionMenuModel(new OptionMenuModel(optionsMenuData, optionsMenuSize));
    }
}
// } changed by eugeny - 12-09-15

void AppMediaPlayer_EngineAudio::HideUI(DISPLAY_T display)
{
    MP_HIGH << LOG_ENDL;
    //onTuneTimerStop();// added by yongkyun.lee 20130322 for : NO CR
    //added by edo.lee 2013.08.20 ISV 88604
    if (m_tuneSelectTimer->isActive())
    {
        m_tuneSelectTimer->stop();
        onTuneSelectTimer();
        showCurrentOSD();
    }
    //added by edo.lee 2013.08.20 ISV 88604
    m_AudioController.setOperationDisplay(DISPLAY_FRONT_REAR); //added by edo.lee 2013.03.22
    m_EngineMain.setPlayInfoDisplay(DISPLAY_FRONT_REAR);//added by edo.lee 2013.04.26
    // { added by junggil 2012.09.05 for keep the FF or REW mode when change to another screen.
    if (m_AudioController.isLongPress())
    {
        //{changed by junam 2013.09.07 for ITS_KOR_185529
        //if (m_EngineMain.getCamInFG() == true && m_bSeekHardKeyPressed )
        if (m_EngineMain.getCamInFG() == true && (m_bNextHardKeyPressed || m_bPrevHardKeyPressed) ) //}changed by junam
        {
            // Keep Seek Mode
        }
        else
        {
            // { changed by junam to use clearSeekMode() instead
            //emit releaseLongPress();
            emit clearSeekMode();
            // } changed by junam
        }
        // } modified by dongjin
    }
    // } added by junggil
    // { modified by eugeny.novikov 2012.11.20 for CR 15758
    if (m_AudioController.isPlayerActive())
    {
        m_bSaveState = true;
    }
    // } modified by eugeny.novikov 2012.11.20
    switch(m_EngineMain.getCloneState())
    {
        case CLONE_OFF:
            m_AudioController.setForeground(false);
            break;
        case CLONE_ON_FRONT:
        case CLONE_ON_REAR:
            break;
        default:
            break;
    }

    // { added by kihyung 2012.2.5 for DVD-Audio
    if(m_AudioController.GetMediaMode() == MP::DISC && m_AudioController.getDiscType() == MP::DVD_AUDIO) {
        m_AudioController.hideVideo();
    }
    // } added by kihyung

    //if(OSDview)			OSDview->setWindowTitle("@OP^CLEAR"); // remove by edo.lee 2013.01.10
    //ClearOSD();//remove by edo.lee 2013.04.20
    emit bgReceived(); // added by ruindmby 2012.09.21 for CR 13802
    //m_bPlayedbyAVModeChanged = false; // removed by ruinmdby 2012.08.20

    //Wooju. 2013.08.28 Temporary revert this back because it cause hang during bt call.
    //{added by junam 2013.08.26 for move to HideUI
    /*
    if(m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2)
    {
        if(m_EngineMain.getisBTCall())
        {
            m_AudioController.emitShowPlayerView();
        }
    }
    */
    //Wooju. 2013.08.28 Temporary revert this back because it cause hang in process.
    //}added by junam

    m_AudioController.GetListViewModel()->setCopyCompletedMode(false); // added by eugene 2012.11.29 for CR 16076
}


void AppMediaPlayer_EngineAudio::setOptionMenuModel( OptionMenuModel *optionMenuModel )
{
    if(m_OptionMenuModel != optionMenuModel)
    {
        m_OptionMenuModel = optionMenuModel;
        emit changeOptionMenuModel();
    }
}

// { added by changjin 2012.12.10 for CR 16364
QString AppMediaPlayer_EngineAudio::getLastPathOfString(QString fullString)
{
    int nIndex = fullString.lastIndexOf('/');

    return fullString.remove(0, nIndex + 1);
}
// } added by changjin 2012.12.10

QString AppMediaPlayer_EngineAudio::makeElidedString( const QString &fullString,
                                                      const QString &fontFamily,
                                                      int pixelSize, int maxWidth )
{
    QFont mFont;
    mFont.setFamily(fontFamily);
    //mFont.setPixelSize(pixelSize);
    mFont.setPointSize(pixelSize);	//modified by aettie.ji 2012.11.28 for uxlaunch update

    QFontMetrics fontMetrics(mFont);

    return fontMetrics.elidedText(fullString, Qt::ElideRight, maxWidth);
}

// { added by junggil 2012.10.04 for CR13505 Media Apps - delta UIS 2.15 to 2.17 
int AppMediaPlayer_EngineAudio::getStringWidth( const QString &fullString,
                                                const QString &fontFamily,
                                                int pixelSize )
{
    //MP_LOG << "JGMA fullString = " << fullString << LOG_ENDL; //removed by sangmin.seol 2014.06.27 reduce log for performance
    QFont mFont;
    mFont.setFamily(fontFamily);
    //mFont.setPixelSize(pixelSize);
    mFont.setPointSize(pixelSize);	//modified by aettie.ji 2012.11.28 for uxlaunch update

    QFontMetrics fontMetrics(mFont);

    return fontMetrics.width(fullString, -1);
}
// } added by junggil 

QString AppMediaPlayer_EngineAudio::popScreen()
{
    if( m_screenStack.isEmpty() )
        return "";
    else
        return m_screenStack.pop();
}

void AppMediaPlayer_EngineAudio::pushScreen(QString strScreen)
{
    m_screenStack.push( strScreen );
}

QString AppMediaPlayer_EngineAudio::topScreen()
{
    if( m_screenStack.isEmpty() )
        return "";
    else
        return m_screenStack.top();
}

void AppMediaPlayer_EngineAudio::setAudioPathRPM()
{
    m_AudioController.setAudioPathRPM();
}

void AppMediaPlayer_EngineAudio::notifyFormatJukeboxBegin()
{
    MP_LOG << LOG_ENDL;
    //modified by edo.lee 2013.02.15
    //Event event(EVT_FORMAT_JUKEBOX_START, GetThisAppId(), APP_ALL);
    Event event( EVT_POST_EVENT, GetThisAppId(), APP_AVP, EVT_FORMAT_JUKEBOX_START);
    emit NotifyUISH(event);
}

void AppMediaPlayer_EngineAudio::notifyFormatJukeboxEnd()
{
    MP_LOG << LOG_ENDL;

    Event event(EVT_FORMAT_JUKEBOX_END, GetThisAppId(), APP_ALL);
    // { modified by lssanh 2013.01.30 ISV71769
    //m_AudioController.ResetDeviceInfo(m_AudioController.playerMode());//added by edo.lee 2013.01.09
    m_AudioController.ResetDeviceInfo(MP::JUKEBOX);
    // } modified by lssanh 2013.01.30 ISV71769
    emit NotifyUISH(event);

    //m_EngineMain.notifyEnableMedia(MEDIA_SOURCE_JUKEBOX,false,false,false);//remove by edo.lee 2013.02.15
}

bool AppMediaPlayer_EngineAudio::IsGracenoteValid()
{
    MP_LOG << LOG_ENDL;

    if (m_pGraceNote)
    {
        return m_pGraceNote->isValid();
    }

    return false;
}

// { modified by kihyung 2013.07.20 for ITS 0179388  
bool AppMediaPlayer_EngineAudio::IsAudioMode(MODE_STATE_T nMode)
{
    switch(nMode)
    {
    case MODE_STATE_JUKEBOX_AUDIO:
    case MODE_STATE_USB1_AUDIO:
    case MODE_STATE_USB2_AUDIO:
    case MODE_STATE_IPOD1_AUDIO:
    case MODE_STATE_IPOD2_AUDIO:
    case MODE_STATE_AUX1_AUDIO:
    case MODE_STATE_AUX2_AUDIO:
    case MODE_STATE_BT_AUDIO:
    case MODE_STATE_DISC_AUDIO:
        return true;
    default:
        return false;
    }
}
// } modified by kihyung

// removed by ruinmdby 2012.08.20
//bool AppMediaPlayer_EngineAudio::IsPlayedbyAVModeChanged()
//{
//    MP_LOG << LOG_ENDL;
//    return m_bPlayedbyAVModeChanged;
//}
// } added by kihyung
//
// { added by kihyung 2012.08.01 for CR 9376
//void AppMediaPlayer_EngineAudio::SetPlayedByAVModeChanged(bool bPlayed)
//{
//    MP_LOG << LOG_ENDL;
//    m_bPlayedbyAVModeChanged = bPlayed;
//}
// removed by ruinmdby 2012.08.20

//{ Added by radhakrushna Mohanty 2012.08.18 CR 11181
bool AppMediaPlayer_EngineAudio::processedInAUXMode(Event &aEvent)
{
    bool ret = false;

    //Display OSD , if Media source is AUX , AUX is running in BG and player is active .
    if(m_AudioController.GetMediaMode() == MP::AUX)
    {
        ret = true;
        // { modified by lssanh 2012.12.27 for ITS 151342
        //if (m_AudioController.isPlayerActive() &&  !m_AudioController.isForeground() )
        if (m_AudioController.isPlayerActive())
            // } modified by lssanh 2012.12.27 for ITS 151342
        {

            switch(aEvent.GetEventId())
            {
            case EVT_KEY_CCP_JOG_DIAL_ENCODER:
            case EVT_KEY_RRC_JOG_DIAL_ENCODER:
            case EVT_KEY_HU_JOG_DIAL_ENCODER:
            case EVT_KEY_HU_SEEK_PREV:
            case EVT_KEY_RRC_SEEK_PREV:
            case EVT_KEY_SWRC_SEEK_PREV:
            case EVT_KEY_HU_SEEK_NEXT:
            case EVT_KEY_RRC_SEEK_NEXT:
            case EVT_KEY_SWRC_SEEK_NEXT:
            case EVT_KEY_CCP_SEEK_PREV: // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
            case EVT_KEY_CCP_SEEK_NEXT: // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
            //case EVT_KEY_RRC_CH_BK:     // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
            //case EVT_KEY_RRC_CH_FW:     // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                m_EngineMain.setIsFROnTune(true);		//added by hyochang.ryu 20130822 for ITS184441
                OSDManager::instance()->displayOSD(OSD_EVENT_UNSUPPORTED_ACTION);
                break;

            case EVT_KEY_HU_TUNE_PRESSED:
                m_EngineMain.setIsFROnTune(false);		//added by hyochang.ryu 20130822 for ITS184441
                // { modified by cychoi 2014.02.21 for ITS 226585 No current OSD on AUX mode
                OSDManager::instance()->displayOSD(OSD_EVENT_AUX_EXTERNAL_DEVICE_CONNECTED);
                //showCurrentOSD();
                // } modified by cychoi 2014.02.21
                break;

            default:
                MP_LOG << "Unsupported event" << LOG_ENDL;
                break;
            }
        }
    }

    return ret;
}
//} Added by radhakrushna Mohanty 2012.08.18 CR 11181

//{ added by hyochang.ryu 20130704 for ITS178310
bool AppMediaPlayer_EngineAudio::processedInBTMode(Event &aEvent)
{
    bool ret = false;

    if(m_AudioController.GetMediaMode() == MP::BLUETOOTH)
    {
        ret = true;
        if (m_AudioController.isPlayerActive())
        {
            switch(aEvent.GetEventId())
            {
            //{ modified by hyochang.ryu 20130731
            case EVT_KEY_CCP_JOG_DIAL_CENTER:
            case EVT_KEY_RRC_JOG_DIAL_CENTER:
                if(m_EngineMain.getisBTCall()==true)//deleted by yongkyun.lee 2013-10-11 for : ITS 191526
                    m_AudioController.showBTCallPopup();
                break;
                //} modified by hyochang.ryu 20130731
            case EVT_KEY_HU_JOG_DIAL_ENCODER:
                /*	//modified by hyochang.ryu 20100710
                case EVT_KEY_CCP_JOG_DIAL_ENCODER:
                case EVT_KEY_RRC_JOG_DIAL_ENCODER:
                case EVT_KEY_HU_SEEK_PREV:
                case EVT_KEY_RRC_SEEK_PREV:
                case EVT_KEY_SWRC_SEEK_PREV:
                case EVT_KEY_HU_SEEK_NEXT:
                case EVT_KEY_RRC_SEEK_NEXT:
                case EVT_KEY_SWRC_SEEK_NEXT:
                */ //deleted by hyochang.ryu 20130706
                OSDManager::instance()->displayOSD(OSD_EVENT_BT_UNSUPPORTED);
                break;
            default:
                MP_LOG << "Unsupported event" << LOG_ENDL;
                break;
            }
        }
    }

    //{removed by junam 2013.07.12 for music app
    //if(m_AudioController.isControllerDisable())
    //    ret = true;
    //}removed by junam

    return ret;
}
//} added by hyochang.ryu 20130704 for ITS178310

//{ added by eunhye 2012.8.31 for CR13047
bool AppMediaPlayer_EngineAudio::processedIpodIndexing(int nEventID)
{
    bool ret = false;

    //Display OSD , if Media source is IPOD , IPOD is not finished indexding .
    if(m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2)
    {
        if(m_AudioController.isMediaSyncfinished() == false)
        {
            MP_LOG << "m_AudioController.isMediaSyncfinished() return false" << LOG_ENDL;
            switch(nEventID)
            {
            case EVT_KEY_CCP_JOG_DIAL_ENCODER:
            case EVT_KEY_RRC_JOG_DIAL_ENCODER:
                MP_LOG << "CASE 1" << LOG_ENDL;
                ret = false;
                break;
            case EVT_KEY_HU_JOG_DIAL_ENCODER:
            case EVT_KEY_HU_TUNE_PRESSED:
            case EVT_KEY_HU_SEEK_PREV:
            case EVT_KEY_RRC_SEEK_PREV:
            case EVT_KEY_SWRC_SEEK_PREV:
            case EVT_KEY_HU_SEEK_NEXT:
            case EVT_KEY_RRC_SEEK_NEXT:
            case EVT_KEY_SWRC_SEEK_NEXT:
            case EVT_KEY_CCP_SEEK_PREV: // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
            case EVT_KEY_CCP_SEEK_NEXT: // Added by sangmin.seol 2014.02.11 Add CCP_SEEK KEY
            //case EVT_KEY_RRC_CH_BK:     // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
            //case EVT_KEY_RRC_CH_FW:     // removed by sangmin.seol 2014.03.11 DH removed RRC_CH KEY
                /*
                    if (m_AudioController.isPlayerActive() &&  !m_AudioController.isForeground() )
                    {
                        emit updateOSD(OSD_EVENT_IPOD_READING);
                    }
                    */
                MP_LOG << "CASE 2" << LOG_ENDL;
                ret = true;
                break;
            default:
                MP_LOG << "Unsupported event" << LOG_ENDL;
                break;
            }
        }
        else {
            MP_LOG << "m_AudioController.isMediaSyncfinished() return true" << LOG_ENDL;
        }
    }

    MP_LOG << " return valuse: " << ret << LOG_ENDL;
    
    return ret;
}
//} added by eunhye 2012.8.31 

// { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
bool AppMediaPlayer_EngineAudio::processedInDiscMode()
{
    if(m_AudioController.GetMediaMode() == MP::DISC)
    {
        return m_EngineMain.IsDeckBlocked();
    }

    return false;
}
// } added by cychoi 2014.04.09

//{ added by ravikanth
void AppMediaPlayer_EngineAudio::onChapterScreenLaunched(bool show)
{
    // todo - move value to global space
    if(show)
    {
        m_AudioController.SetVideoSizeWrtDisplay(727, 319, 500, 300);
    }
    else
    {
        m_AudioController.SetVideoSizeWrtDisplay(0,0, 1280, 720);
    }
}
//} added by ravikanth

//{ added by jeeeun for MLT play in FF/REW 2012.12.05
void AppMediaPlayer_EngineAudio::onSetHardKeyDisable(bool isTrackChanged)
{
    //{changed by junam 2013.09.07 for ITS_KOR_185529
    //setSeekHardKeyPressed(isTrackChanged);
    setNextHardKeyPressed(isTrackChanged);
    setPrevHardKeyPressed(isTrackChanged);
    //}changed by junam
}
//} added by jeeeun for MLT play in FF/REW 2012.12.05

//added by ruindmby 2012.09.17 for CR11770
void AppMediaPlayer_EngineAudio::ResyncGracenote(int targetVolume)
{
    if (IsGracenoteValid())
    {
        m_pGraceNote->ResyncAfterCopyOperation(targetVolume);
    }
}

//added by ruindmby 2012.09.17 for CR11770

// { added by ravikanth - 12-09-26
void AppMediaPlayer_EngineAudio::storeDVDdata()
{
    m_AudioController.storeDVDdata();
}

void AppMediaPlayer_EngineAudio::restoreDVDdata()
{
    m_AudioController.restoreDVDdata();
}
// } added by ravikanth - 12-09-26

//{ added by  yongkyun.lee 2012.11.12 for  Check Video/audio OSD
void AppMediaPlayer_EngineAudio::DisplayOSD(bool osd)
{
    //MP_LOG << LOG_ENDL;
    emit setDisplayOSD( osd );//added by  yongkyun.lee 2012.11.05 for  Check Video/audio OSD
}
//}added by  yongkyun.lee
//added by edo.lee 2013.06.26
//{Commented by Alexey Edelev 2012.10.08
//// { added by junggil 2012.10.04 for CR13505 Media Apps - delta UIS 2.15 to 2.17
//bool AppMediaPlayer_EngineAudio::handleSettingsCommand( EVENT_SETTINGS_CHANGED_DATA_T *eventData )
//{
//    MP_LOG << ", sValue = " << eventData->sValue << LOG_ENDL;		// 1
//    MP_LOG << ", sKey = " << eventData->sKey << LOG_ENDL;		// 68 DB_KEY_SCROLL_TICKER

//    if( eventData->sKey == 68 ) // Temp code : AppSettingsDef::DB_KEY_SCROLL_TICKER )
//    {
//        m_bScrollingTicker = (eventData->sValue == 1);
//        return true;
//    }
//    else
//    	return false;
//}
//// } added by junggil
//}Commented by Alexey Edelev 2012.10.08

// { deleted by wspark 2013.04.10 for cluster update
/*
// { added by wspark 2013.04.02
void AppMediaPlayer_EngineAudio::onClusterUpdateTimer()
{
    emit notifyBtCallProgressing(false);
}
// } added by wspark
*/
// } deleted by wspark

//{ added by yongkyun.lee 20130413 for : NO CR MP3 List 
void
AppMediaPlayer_EngineAudio::setMP3ListReadState(bool listRead)
{
    //if (getStateUI() == "disc" && m_AudioController.getDiscType() == MP::MP3_CD)//removed by eunhye 2013.04.25
    //{//removed by eunhye 2013.04.25
    DISC_MP3_MAIN_MENU[1].bEnabled = m_MP3ListView = listRead;
    //}//removed by eunhye 2013.04.25
    emit mp3ListReadState(listRead);
}
//} added by yongkyun.lee 20130413 

// { modified by ravikanth 16-04-13
void AppMediaPlayer_EngineAudio::CancelCopyJukebox()
{
    MP_LOG<< "EVT_POST_EVENT sent to FM" << LOG_ENDL;
    Event postEvent( EVT_POST_EVENT,
                     m_EngineMain.GetThisAppId(),
                     APP_FMGR,
                     EVT_FILEOPERATION_STOP);
    m_EngineMain.NotifyUISH (postEvent);
    setCopyToJukeboxStatus(false, 0); // modified by eugene.seo 2013.05.29
}

void AppMediaPlayer_EngineAudio::CancelAudioCopy(bool isRemoved)
{
    m_AudioController.GetListViewModel()->cancelFileCopyOperation(isRemoved);
}

void AppMediaPlayer_EngineAudio::setCopyToJukeboxStatus(bool isInProgress, int usbType) // modified by eugene.seo 2013.05.29
{
    m_EngineMain.setCopyInProgress(isInProgress, usbType); // modified by eugene.seo 2013.05.29
    if(isInProgress)
    {
        notifyCopyToJukeboxBegin();
    }
    else
    {
        notifyCopyToJukeboxEnd();
    }
}

void AppMediaPlayer_EngineAudio::notifyCopyToJukeboxBegin()
{
    Event postEvent( EVT_POST_EVENT, m_EngineMain.GetThisAppId(), APP_FMGR, EVT_JUKEBOX_COPY_START);
    m_EngineMain.NotifyUISH(postEvent);

    // modified by ravikanth 11-07-13 for ITS 0177070
    // event to UISH
    Event event( EVT_JUKEBOX_COPY_START, GetThisAppId(), APP_UI_MANAGER );
    m_EngineMain.NotifyUISH (event);
}

void AppMediaPlayer_EngineAudio::notifyCopyToJukeboxEnd()
{
    Event postEvent( EVT_POST_EVENT, m_EngineMain.GetThisAppId(), APP_FMGR, EVT_JUKEBOX_COPY_END);
    m_EngineMain.NotifyUISH(postEvent);

    // modified by ravikanth 11-07-13 for ITS 0177070
    // event to UISH
    Event event( EVT_JUKEBOX_COPY_END, GetThisAppId(), APP_UI_MANAGER );
    m_EngineMain.NotifyUISH (event);
}
// } modified by ravikanth 16-04-13

// { modified by ravikanth 18-04-13
bool AppMediaPlayer_EngineAudio::loadingOnCopy()
{
    // removed by kihyung 2013.09.09 for ITS 0186027
    /*
    if(m_mode == MP::JUKEBOX)
    {
        return m_trackerUpdateInProgress; // modified by ravikanth 20-04-13
    }
    */
    return false;
}
// } modified by ravikanth 18-04-13

// { modified by ravikanth 20-04-13
void AppMediaPlayer_EngineAudio::setRescanProgressLoading()
{
    // removed by kihyung 2013.09.09 for ITS 0186027
    /*
    MP_LOG << LOG_ENDL;
    m_trackerUpdateInProgress = true;
    emit jukeboxCopyInProgress();
    */
}

void AppMediaPlayer_EngineAudio::ResetProgressLoading()
{
    // removed by kihyung 2013.09.09 for ITS 0186027
    /*
    MP_LOG << LOG_ENDL;
    m_trackerUpdateInProgress = false;
    emit jukeboxCopyInProgress();
    */
}
// } modified by ravikanth 20-04-13

// { added by edo.lee 2013.08.12 ISV 88908
void AppMediaPlayer_EngineAudio::setFileManager(bool isFront)
{
    OSDManager::instance()->setFileManagerScreen(isFront ? DISPLAY_FRONT : DISPLAY_REAR);
}
void AppMediaPlayer_EngineAudio::setFileManagerBG()
{
    OSDManager::instance()->setFileManagerToBG();
}

// { added by edo.lee 2013.08.12

void AppMediaPlayer_EngineAudio::hideBTMusic()
{
    MP_LOG << LOG_ENDL;
    emit hideBasicBTMusic();
}
//added by edo.lee 2013.06.22
void AppMediaPlayer_EngineAudio::SetSystemMute()
{
    MP_LOG << LOG_ENDL;
    m_AudioController.SetSystemMute();
}
//added by edo.lee 2013.06.22

void AppMediaPlayer_EngineAudio::setRewReachedFirst(bool value)
{
    m_bRewReachedFirst = value;
    emit rewReachedFirstChanged(m_bRewReachedFirst);
} // added by Sergey 28.05.2013

// { added by wonseok.heo 2013.07.04 disc in out test
void AppMediaPlayer_EngineAudio::testModeOff(){
    MP_LOG << LOG_ENDL;
    m_AudioController.setInOuteStepCnt(0);
    m_AudioController.setInOutTestCnt(0);
    m_EngineMain.setInOutTestMode(false);
    m_EngineMain.InOutTestEject();

}

void AppMediaPlayer_EngineAudio::testModeOn(){
    MP_LOG << LOG_ENDL;
    m_EngineMain.setInOutTestMode(true);
    m_EngineMain.InOutTestEject();
    emit sendTestModeStatus(1);
}
// } added by wonseok.heo 2013.07.04 disc in out test

//{ added by wonseok.heo for ISV 86609 2013.07.20
bool AppMediaPlayer_EngineAudio::isBTPaused() const
{
    return m_bPlayAfterBTCall;
} // } added by wonseok.heo for ISV 86609 2013.07.20
//added by sh.kim 2013.08.09 for ITS 183042
bool AppMediaPlayer_EngineAudio::isBtCallAfterScan() const
{
    return m_BtCallAfterScan;
}
//added by sh.kim 2013.08.09 for ITS 183042
// { added by kihyung 2013.06.02
MODE_STATE_T AppMediaPlayer_EngineAudio::mapToModeState(APP_ENTRY_POINT_T aEntryPoint)
{
    MODE_STATE_T result = MODE_STATE_MAX;

    switch(aEntryPoint)
    {
    case eAPP_MODE_DYNAMICS_DISC: // added by wonseok.heo for Disc IN OUT TEST 2013.07.20
    case eAPP_FILEMANAGER_JUKEBOX_AUDIO: //added by junam 2013.12.19 for LIST_ENTRY_POINT
    case eAPP_AVP_AUDIO_MODE_JUKEBOX:   result = MODE_STATE_JUKEBOX_AUDIO;  break;

    case eAPP_FILEMANAGER_USB1_AUDIO: //added by junam 2013.12.19 for LIST_ENTRY_POINT
    case eAPP_AVP_AUDIO_MODE_USB1:      result = MODE_STATE_USB1_AUDIO;     break;

    case eAPP_FILEMANAGER_USB2_AUDIO: //added by junam 2013.12.19 for LIST_ENTRY_POINT
    case eAPP_AVP_AUDIO_MODE_USB2:      result = MODE_STATE_USB2_AUDIO;     break;

    case eAPP_FILEMANAGER_IPOD1: //added by junam 2013.12.19 for LIST_ENTRY_POINT
    case eAPP_AVP_AUDIO_MODE_IPOD1:     result = MODE_STATE_IPOD1_AUDIO;    break;

    case eAPP_FILEMANAGER_IPOD2: //added by junam 2013.12.19 for LIST_ENTRY_POINT
    case eAPP_AVP_AUDIO_MODE_IPOD2:     result = MODE_STATE_IPOD2_AUDIO;    break;

    case eAPP_FILEMANAGER_DISC_AUDIO: // added by oseong.kwon 2014.02.10 for ITS 223602
    case eAPP_AVP_AUDIO_MODE_DISC:      result = MODE_STATE_DISC_AUDIO;     break;

    case eAPP_AVP_AUDIO_MODE_AUX:       result = MODE_STATE_AUX1_AUDIO;     break;
    case eAPP_AVP_AUDIO_MODE_BT:        result = MODE_STATE_BT_AUDIO;       break;
    case eAPP_AVP_MODE_BLUETOOTH:       result = MODE_STATE_BT_AUDIO;	    break;
    default:
        MP_LOG << "Unsupported entry point:" << aEntryPoint << LOG_ENDL;
        break;
    }

    return result;
}
// } added by kihyung 2013.06.02


void AppMediaPlayer_EngineAudio::setOSDMetaData(OSD_DEVICE device, QString title, int shuffle , int repeat, int scan, bool isTune)
{
    emit setOSDMetaDataChanged( device,  title,  shuffle, repeat, scan,  isTune);
}
void AppMediaPlayer_EngineAudio::updatePosition(OSD_DEVICE device , int position, bool isLong)
{
    emit updatePositionChanged( device,  position, isLong);
}
void AppMediaPlayer_EngineAudio::updatePlayingMode(OSD_DEVICE device ,int shuffle , int repeat, int scan)
{
    emit updatePlayingModeChanged( device,  shuffle, repeat, scan);
}

void AppMediaPlayer_EngineAudio::showCurrentOSD()
{
    //emit showCurrentOSDChanged();
    MP::DEVICE_T device = m_AudioController.getDevice();
    bool isIPodDuringPhoneCall = ((device == MP::IPOD1) || (device == MP::IPOD2)) && m_EngineMain.getisBTCall();
    if(isIPodDuringPhoneCall)
    {
        OSDManager::instance()->displayOSD(OSD_EVENT_IPOD_EMPTY_TITLE);
    }
    else
    {
        emit showCurrentOSDChanged();
    }
}

void AppMediaPlayer_EngineAudio::updateOSDMetaData(OSD_DEVICE device , QString title)
{
    emit updateOSDMetaDataChanged(device , title);
}
// { added by wonseok.heo for ITS 197433 2013.10.28
void AppMediaPlayer_EngineAudio::updateCDDAOSDMetaData(OSD_DEVICE device , QString title)
{
    emit updateCDDAOSDMetaDataChanged(device , title);
} // } added by wonseok.heo for ITS 197433 2013.10.28

void AppMediaPlayer_EngineAudio::deleteUserData()
{
    MP_LOG << LOG_ENDL;
    //added by shkim for ITS 158365 Delete User Data for Factory Reset
    if(m_AudioController.GetControllers().GetUSBController())
    {
        QString volumeName = QUrl("/usr_data").path();
        m_AudioController.GetControllers().GetUSBController()->StartDiscFormat(volumeName);
    }
    //added by shkim for ITS 158365 Delete User Data for Factory Reset
}

// { added by Sergey 13.09.2013 for ITS#188762
void AppMediaPlayer_EngineAudio::resetAll()
{
    // Modified by jonghwan.cho@lge.com 2013.11.11 for ITS#0208038
    //factoryReset();
}
// } added by Sergey 13.09.2013 for ITS#188762


//{added by junam 2013.08.07 for factory reset
void AppMediaPlayer_EngineAudio::factoryReset()
{
    QDir iPodDataDir("/app/data/AppMedia/tmp/iPodData/");
    if(iPodDataDir.exists())
    {
        QStringList entryList = iPodDataDir.entryList();
        foreach(QString entry, entryList)
            iPodDataDir.remove(entry);
    }

    QDir iPodDBPathDir("/app/data/AppMedia/share/iPodDBPath/");
    if(iPodDBPathDir.exists())
    {
        QStringList entryList = iPodDBPathDir.entryList();
        foreach(QString entry, entryList)
        {
            if(entry.endsWith(".xml"))
                continue;
            iPodDBPathDir.remove(entry);
        }
    }

    QDir tempIPodDataDir("/app/data/AppMedia/tmp/tempIPodData/");
    if(tempIPodDataDir.exists())
    {
        QStringList entryList = tempIPodDataDir.entryList();
        foreach(QString entry, entryList)
            tempIPodDataDir.remove(entry);
    }
}
//}added by junam

//{ modified by yongkyun.lee 2013-09-04 for :  BT Mode key - pause
MODE_STATE_T AppMediaPlayer_EngineAudio::getModeStateHistoryPrev()
{
    return m_modeStateHistory.currMode ;
}

MODE_STATE_T AppMediaPlayer_EngineAudio::getModeStateHistoryCurr()

{
    return m_modeStateHistory.prevMode ;
}

//{ modified by yongkyun.lee 2014-03-13 for : BT streaming Mute-play control
bool AppMediaPlayer_EngineAudio::getModeStateHistoryFMode()
{
    return m_modeStateHistory.firstPlayOnModeChange ;
}

void AppMediaPlayer_EngineAudio::setModeStateHistoryFMode(bool val)
{
     m_modeStateHistory.firstPlayOnModeChange = val;
}
//} modified by yongkyun.lee 2014-03-13

bool AppMediaPlayer_EngineAudio::getModeStateAVP()
{
    switch(m_modeStateHistory.prevMode)
    {
    case MODE_STATE_DISC_AUDIO     :
    case MODE_STATE_DISC_VIDEO     :
    case MODE_STATE_USB1_AUDIO     :
    case MODE_STATE_USB1_VIDEO     :
    case MODE_STATE_USB2_AUDIO     :
    case MODE_STATE_USB2_VIDEO     :
    case MODE_STATE_IPOD1_AUDIO    :
    case MODE_STATE_IPOD1_PANDORA  :
    case MODE_STATE_AHA_RADIO_IPOD1:
    case MODE_STATE_IPOD2_AUDIO    :
    case MODE_STATE_IPOD2_PANDORA  :
    case MODE_STATE_AHA_RADIO_IPOD2:
    case MODE_STATE_VCDC           :
    case MODE_STATE_JUKEBOX_AUDIO  :
    case MODE_STATE_JUKEBOX_VIDEO  :
    case MODE_STATE_BT_AUDIO       :
    //{ modified by yongkyun.lee 2014-01-28 for : ITS 221785
    //case MODE_STATE_BT_PANDORA     :
    //case MODE_STATE_AHA_RADIO_BT   :
    //} modified by yongkyun.lee 2014-01-28 
    case MODE_STATE_AUX1_AUDIO     :
    case MODE_STATE_AUX1_VIDEO     :
    case MODE_STATE_AUX2_AUDIO     :
    case MODE_STATE_AUX2_VIDEO     :
        return true;
    default :
        return false;
    }
}


//} modified by yongkyun.lee 2013-09-04 

// modified by ravikanth 05-10-13 for ITS 0193972
void AppMediaPlayer_EngineAudio::OnACCStatusChanged(bool accOff)
{
    if(accOff)
    {
        MP_LOG << "ACC OFF" << LOG_ENDL;
        m_isAccOn = false;//emit accStatus(0);
        //m_EngineMain.GetNotifier()->cancelFileOperation(); // added by lssanh 2013.02.07 2USB
        //// modified by ravikanth 12-07-13 for ITS 0173814, 0178982
        //if(m_AudioController.GetListViewModel()->isInCopyState())
        //    m_AudioController.GetListViewModel()->cancelFileOperation();
        //emit menuOptionClose(); //added by junam 2013.08.12 for ITS_KOR_183027
        //cancelJukeboxFormat();
    }
    else
    {
        MP_LOG << "ACC ON" << LOG_ENDL;
        m_isAccOn = true;//emit accStatus(1);
        //showCurrentOSD(); // moved by suilyou ITS 0210790 to onSignalAgreeOSD
    }
    m_AudioController.SetAccMode( m_isAccOn );
}
void AppMediaPlayer_EngineAudio::onSignalAgreeOSD()
{
    MP_LOG << LOG_ENDL;
    showCurrentOSD();
}

// modified by ravikanth 08-10-13 for format cancel
void AppMediaPlayer_EngineAudio::cancelJukeboxFormat()
{
    MP_LOG << LOG_ENDL;
    if(m_AudioController.GetControllers().GetUSBController())
    {
        m_AudioController.GetControllers().GetUSBController()->CancelFormatOperation();
    }
}

// modified for ITS 0212002
void AppMediaPlayer_EngineAudio::OnPowerStateLogicOff()
{
    MP_LOG << "On ACC OFF" << LOG_ENDL;
    // modified by ravikanth 12-07-13 for ITS 0173814, 0178982
    if(m_AudioController.GetListViewModel()->isInCopyState())
        m_AudioController.GetListViewModel()->cancelFileOperation();
    emit menuOptionClose(); //added by junam 2013.08.12 for ITS_KOR_183027
    cancelJukeboxFormat();
}

//{added by junam 2013.12.19 for LIST_ENTRY_POINT
void AppMediaPlayer_EngineAudio::showListView(bool bShow)
{
    APP_ENTRY_POINT_T entryPoint = eAPP_NONE;
    if(bShow)
    {
        MP_LOG<<" request list => UISH" << LOG_ENDL;
        switch(m_AudioController.GetMediaMode())
        {
        case MP::JUKEBOX: entryPoint = eAPP_FILEMANAGER_JUKEBOX_AUDIO; break;
        case MP::USB1: entryPoint = eAPP_FILEMANAGER_USB1_AUDIO; break;
        case MP::USB2: entryPoint = eAPP_FILEMANAGER_USB2_AUDIO; break;
        case MP::IPOD1: entryPoint = eAPP_FILEMANAGER_IPOD1; break;
        case MP::IPOD2: entryPoint = eAPP_FILEMANAGER_IPOD2; break;
        case MP::DISC: entryPoint = eAPP_FILEMANAGER_DISC_AUDIO; break; // added by oseong.kwon 2014.02.10 for ITS 223602
        default:
            emit startMusicList();
            return;
        }
    }
    else
    {
        MP_LOG<<" request main => UISH" << LOG_ENDL;
        switch(m_AudioController.GetMediaMode())
        {
        case MP::JUKEBOX: entryPoint = eAPP_AVP_AUDIO_MODE_JUKEBOX; break;
        case MP::USB1: entryPoint = eAPP_AVP_AUDIO_MODE_USB1; break;
        case MP::USB2: entryPoint = eAPP_AVP_AUDIO_MODE_USB2; break;
        case MP::IPOD1: entryPoint = eAPP_AVP_AUDIO_MODE_IPOD1; break;
        case MP::IPOD2: entryPoint = eAPP_AVP_AUDIO_MODE_IPOD2; break;
        case MP::DISC: entryPoint = eAPP_AVP_AUDIO_MODE_DISC; break; // added by oseong.kwon 2014.02.10 for ITS 223602
        default:
            emit closeListView();
            return;
        }
    }
    Event event( EVT_KEY_SOFT_AVP,m_EngineMain.GetThisAppId(), APP_UI_MANAGER, entryPoint );
    emit NotifyUISH(event);
}
//}added by junam

// { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
void AppMediaPlayer_EngineAudio::showDeckHighTempPopup()
{
    MP_LOG << LOG_ENDL;
    emit menuOptionClose();
    m_AudioController.showDeckHighTempPopup();
}


void AppMediaPlayer_EngineAudio::closeDeckHighTempPopup()
{
    m_AudioController.closeDeckHighTempPopup();
}

void AppMediaPlayer_EngineAudio::sendDeckErrorToUISH()
{
    MP_LOG << LOG_ENDL;
    m_EngineMain.sendDeckErrorToUISH();
}
// } added by cychoi 2014.04.09

