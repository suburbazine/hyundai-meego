/**
 * DHAVN_AppFramework_Def.h
 *
 * Public definitions for the Application Framework.
 */

#ifndef APP_FRAMEWORK_EXPORT_H
#define APP_FRAMEWORK_EXPORT_H

const char * const UISH_EVENT_HANDLER_FRONT = "UIStateManagerAppListener";
const char * const UISH_EVENT_HANDLER_REAR  = "UIStateManagerAppRearListener";
const char * const UISH_EVENT_HANDLER_COMMON = "UIStateManagerAppCommonListener";
const char * const UISH_EVENT_REGISTER_FRONT = "UIStateManagerRegisterApp";
const char * const UISH_EVENT_REGISTER_REAR = "UIStateManagerRegisterRearApp";

#include <QtCore/qglobal.h>
#include <QDBusMetaType>

#define ENABLE_CARPLAY

#ifndef APP_FRAMEWORK_EXPORT
   #ifdef APP_FRAMEWORK_MAKEDLL
      /* We are building this library */
      #define APP_FRAMEWORK_EXPORT Q_DECL_EXPORT
   #else
      /* We are using this library */
      #define APP_FRAMEWORK_EXPORT Q_DECL_IMPORT
   #endif
#endif

//class DECLARE_META
//{
//public:

//   static void DECLARE_META_EVENT()
//   {
//      qRegisterMetaType< Event >();
//      qRegisterMetaTypeStreamOperators< Event >();
//   }
//};

//class DECLARE_APP
//{
//public:

//   static void APP_ARGS_PROCESSING( QDeclarativeView &aViewObj )
//   {
//      if ( argc > 1 && argv != NULL )
//      {
//         QString appParam( argv[ 1 ] );

//         if ( appParam == "fg" )
//         {
//            aViewObj.show();
//         }
//      }
//   }
//};

#define DECLARE_META_EVENT()                     \
{                                                \
   qRegisterMetaType< Event >();                 \
   qRegisterMetaTypeStreamOperators< Event >();  \
   qDBusRegisterMetaType< Event >();             \
}

#define APP_ARGS_PROCESSING( aViewObj )          \
{                                                \
   if ( argc > 1 && argv != NULL )               \
   {                                             \
      QString appParam( argv[ 1 ] );             \
                                                 \
      if ( appParam == "fg" )                    \
      {                                          \
         aViewObj.show();                        \
      }                                          \
   }                                             \
}                                                \

enum BLENDING_T
{
    APP_BLENDING_ON = 0,
    APP_BLENDING_OFF
};

/**
* List of possible languages.
* This is either the two digit language code as defined by ISO 639-1 or
* a combination of ISO 639-1 with a two or three digit macrolanguage code
* as defined by ISO 639-3.
*
* For reference see:
*     http://en.wikipedia.org/wiki/ISO-639
*     http://en.wikipedia.org/wiki/ISO_639_macrolanguage
*/


enum LANGUAGE_T
{
    LANGUAGE_UNKNOWN = 0,
    LANGUAGE_EN,     //< English
    LANGUAGE_KO,     //< Korean
    LANGUAGE_ZH_CN,  //< Peoples Republic Of China (Mandarin)
    LANGUAGE_ZH_CMN = LANGUAGE_ZH_CN, //< Peoples Republic Of China (Mandarin) to be backward compatible
    LANGUAGE_ZH_YUE, //< Peoples Republic Of China (Cantonese)
    LANGUAGE_PT,     //< Europeen Portuguese
    LANGUAGE_EN_GB,  //< English (UK)
    LANGUAGE_EN_US,  //< English (US)
    LANGUAGE_FR,     //< European French
    LANGUAGE_IT,     //< Italian
    LANGUAGE_DE,     //< German
    LANGUAGE_ES,     //< European Spanish
    LANGUAGE_RU,     //< Russian
    LANGUAGE_NL,     //< Dutch
    LANGUAGE_SV,     //< Swedish
    LANGUAGE_PL,     //< Polish
    LANGUAGE_TR,     //< Turkish
    LANGUAGE_CS,     //< Czech
    LANGUAGE_DA,     //< Danish
    LANGUAGE_SK,     //< Slovakia
    LANGUAGE_AR,     //< Arabic
    LANGUAGE_FR_NA,
    LANGUAGE_ES_NA
};

enum NAVI_ROUTE_T
{
    ALL_NAVI_OFF,
    OEM_NAVI_ON,
    CONNECTIVITY_NAVI_ON,
};

/*
enum LANGUAGE_T
{
    LANGUAGE_UNKNOWN = 0,
    LANGUAGE_KO,     //< Korean
    LANGUAGE_EN,     //< English (US)
    LANGUAGE_EN_GB,  //< English (UK)
    LANGUAGE_ZH_CN,  //< Peoples Republic Of China (Mandarin)
    LANGUAGE_ZH_YUE, //< Peoples Republic Of China (Cantonese)
    LANGUAGE_FR,     //< European French
    LANGUAGE_ES,     //< European Spanish
    LANGUAGE_AR,     //< Arabic
    LANGUAGE_CS,     //< Czech
    LANGUAGE_DA,     //< Danish
    LANGUAGE_DE,     //< German
    LANGUAGE_IT,     //< Italian
    LANGUAGE_NL,     //< Dutch
    LANGUAGE_PL,     //< Polish
    LANGUAGE_PT,     //< Europeen Portuguese
    LANGUAGE_RU,     //< Russian
    LANGUAGE_SK,     //< Slovakia
    LANGUAGE_SV,     //< Swedish
    LANGUAGE_TR,     //< Turkish
    LANGUAGE_EN_US,
    LANGUAGE_INVALID //< Invalid           
};
*/

/**
 * Application IDs
 * This list contains all the applications the UI State Manager knows.
 * New applications need to be added at the end of the list after the
 * other IDs and before APP_ALL and APP_MAX. Otherwise binary compatibilty
 * will be broken.
 */
enum APP_ID_T
{
   APP_NONE = 0,
   APP_UI_MANAGER,   //1
   APP_IDLE,             //2  Not Used
   APP_MAIN_MENU,    //3
   APP_AUDIO,           //4 Not Used
   APP_RADIO,           //5
   APP_VIDEO,           //6 Not Used
   APP_TV,               //7 Not Used
   APP_BTPHONE,       //8
   APP_INFO,             //9 hvac
   APP_MOBILETV,       //10 Not Used
   APP_SETTINGS,       //11
   APP_HVAC,            //12 Not Used
   APP_FMGR,           //13
   APP_NAVIGATION,    //14
   APP_VR,               //15
   APP_SETTINGS_BT,  //16 Not Used
   APP_PHOTO,         //17 Not Used
   APP_IBOX,            //18
   APP_UPGRADE,      //19
   APP_CAMERA,       //20
   APP_ENGINEERING, //21
   APP_STANDBY,      //22
   APP_XM_AUDIO,     //23
   APP_MEDIA_PLAYER, //24 Not Used
   APP_XM_DATA,       //25
   APP_DAB,             //26
   APP_PANDORA,      //27
   APP_AVP,             //28
   APP_STATUS_BAR,   //29 Not Used
   APP_SEAT_CONTROL, //30
   APP_E_MANUAL,      //31
   APP_MIRROR_LINK,  //32
   APP_AHA_RADIO,     //33
   APP_INTERACTIVE_GUIDE, //34 Not Used
   APP_MPAUTOTEST,  //35
   APP_GPS,             //36
   APP_MOSTMANAGER,    //37
   APP_PROJECTION,			// 38
   PROJECTIONDAEMON,    // 39
   LOCATIONMANAGER,		// 40
   // Add new applications before this line

   // CR1950 make sure that everthing still works fine after adding
   // new apps in enum. Is used as define in events to adress all as well,
   // because of that needs to be a fix value here.
   APP_TOTAL,
   APP_ALL          = 0x00000400, /**< Used to adress all applications, e.g. for "broadcast" events */
   APP_MAX          /**< @deprecated Do not use this to iterate over the application IDs */
};

/** This command (int user data) may be sent with any EVT */
typedef int EVT_COMMAND_T;
typedef int EVT_ID_T;

/**
* List of possible framework events.
*/
typedef enum
{
    EVT_NONE = 0,

    /* Common Events */
    EVT_REQUEST_BG,
    //1 Screen Widget Show Event(UISH Send to Apps), if SetAutoScreenVisibility was true, Using Auto Show by AFW
    //Display Type : Front , Rear
    EVT_REQUEST_FG,
    //2 Screen Widget Hide Event(UISH Send To Apps), Using APP_ENTRY_POINT_PAYLOAD_T Struct
    //Display Type : Front , Rear
    EVT_RESPONSE_FG,
    //3 Screen Widget Show Responese(Apps(AFW) Send to UISH), if SetAppHandlesResponseFg was false, Using a Auto Respoinse by AFW
    //Display Type : Front , Rear
    EVT_POST_EVENT,
    //4                  /**< API for applications to send events between them */
    //Display Type : Front , Rear, Common

    EVT_USB_MOUNTED,
    //5 USB Mount Event(UISH Send To Apps), Using APP_EVENT_DATA_MP_T Struct(Mount Path)
    //Display Type : Common
    EVT_USB_REMOVED,
    //6 USB UnMount Event(UISH Send To Apps),
    //Display Type : Common
    EVT_APP_LAUNCH, //7 Not Used
    EVT_LANGUAGE_CHANGED,
    //8 Language Change Event(Settins App Send to UISH & UISH Send To Apps), Using EVENT_LANGUAGE_CHANGED_DATA_T Sturct
    //Display Type : Common
    EVT_SHOW_POPUP,
    //9 System POPUP Event(UISH Send To Apps), Using AFW&POPUP Plugin, Using EVENT_SYSTEM_POPUP_T Struct, if Recive AFW/POPUP Plugin Sent to App Signal Emit
    //Display Type : Front , Rear
    EVT_HIDE_POPUP,
    //10 System POPUP Hide Event(UISH Send To Apps),
    //Display Type : Front , Rear
    EVT_VR_NOTIFY,    //11 Not Used
    EVT_DVD_MEDIA_INSERT,    //12 Not Used
    EVT_RESOURCE_GRANTED,               //13 Not Used
    EVT_RESOURCE_DENIED,                //14 Not Used
    EVT_RESOURCE_RELEASED,              //15 Not Used
    EVT_RESOURCE_RELEASED_BY_MANAGER,   //16 Not Used
    EVT_RESOURCE_LOST,                  //17 Not Used
    EVT_RESOURCE_ERROR,                 //18 Not Used
    EVT_RESOURCE_AVAILABLE,             //19 Not Used
    /* HW Key InPut */
    EVT_KEY_HU_BACK,
    //20 User Input FaceFlate Back HW Key,
    //Display Type : Common
    EVT_KEY_HU_HOME,
    //21 User Input FaceFlate Home HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_HU_MENU,
    //22 User Input FaceFlate Menu HW Key
    //Display Type : Common
    EVT_KEY_HU_DISPLAY,
    //23 User Input FaceFlate Display HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_HU_RADIO,
    //24 User Input FaceFlate Radio HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_HU_DMB,
    //25 User Input FaceFlate DMB HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_HU_PHONE,
    //26 User Input FaceFlate Phone HW Key
    //Display Type : Common
    EVT_KEY_HU_TUNE,
    // 27 User Input FaceFlate Tune Jog Dial
    //Display Type : Common
    EVT_KEY_HU_SEEK_NEXT,
    //28 User Input FaceFlate SEEK_NEXT HW Key
    //Display Type : Common
    EVT_KEY_HU_SEEK_PREV,
    //29 User Input FaceFlate SEEK_PREV HW Key
    //Display Type : Common
    EVT_KEY_HU_NAVI,
    //30 User Input FaceFlate NAVI HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_CCP_RADIO_DMB,
    //31 User Input CCP RADIO_DMB HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_CCP_MEDIA,
    //32 User Input CCP Media HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_CCP_MAP_INFORM,
    //33 User Input CCP MAP HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_CCP_DEST,
    //34 User Input CCP DEST HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_CCP_PHONE,
    //35 User Input CCP PHONE HW Key
    //Display Type : Common
    EVT_KEY_CCP_BLUELINK,
    //36 User Input CCP BLUELINK HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_CCP_JOG_DIAL, //37 Not Used
    EVT_KEY_RRC_BACK,
    //38 User Input RRC BACK HW Key
    //Display Type : Common
    EVT_KEY_RRC_HOME,
    //39 User Input RRC Home HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_RRC_MENU,
    //40 User Input RRC Menu HW Key
    //Display Type : Common
    EVT_KEY_RRC_MODE,
    //41 User Input RRC BACK HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_RRC_POWER,
    //42 User Input RRC RIGHT POWER HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_RRC_VOL_UP,
    //43 User Input RRC VOL_UP HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_RRC_VOL_DOWN,
    //44 User Input RRC VOL_DOWN HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_RRC_SEEK_NEXT,
    //45 User Input RRC SEEK_NEXT HW Key
    //Display Type : Common
    EVT_KEY_RRC_SEEK_PREV,
    //46 User Input RRC SEEK_PREV HW Key
    //Display Type : Common
    EVT_KEY_RRC_BLUELINK,
    //47 User Input RRC BLUELINK HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_RRC_NAVI,
    //48 User Input RRC NAVI HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_RRC_MUTE,
    //49 User Input RRC MUTE HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_RRC_JOG_DIAL, //50 Not Used
    EVT_KEY_SWRC_PTT,
    //51 User Input SWERC PTT HW Key
    //Display Type : Common
    EVT_KEY_SWRC_MODE,
    //52 User Input SWRC MODE HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_SWRC_CALL_SEND,
    //53 User Input SWRC CALL_SEND HW Key
    //Display Type : Common
    EVT_KEY_SWRC_CALL_END,
    //54 User Input SWRC CALL_END HW Key
    //Display Type : Common
    EVT_KEY_SWRC_MUTE,
    //55 User Input SWRC MUTE HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_SWRC_VOL_UP,
    //56 User Input SWRC VOL_UP HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_SWRC_VOL_DOWN,
    //57 User Input SWRC VOL_DOWN HW Key , UISH only using
    //Display Type : Common
    EVT_KEY_SWRC_SEEK_NEXT,
    //58 User Input SWRC SEEK_NEXT Key
    //Display Type : Common
    EVT_KEY_SWRC_SEEK_PREV,
    //59 User Input SWRC SEEK_PREV Key
    //Display Type : Common
    EVT_KEY_SOFT_BACK,
    //60 Apps Send to UISH
    //Display Type : Front, Rear
    EVT_KEY_SOFT_HOME,//61 Not Used
    EVT_KEY_SOFT_MENU,//62 Not Used
    EVT_KEY_SOFT_MP,//63 Not Used
    EVT_KEY_SOFT_NAVI,
    //64 Apps Send To UISH , Go to the Navi main gui entry
    //Display Type : Front, Rear
    EVT_KEY_SOFT_FMGR,
    //65 Apps Send To UISH , Go to the FileManager gui entry
    //Display Type : Front, Rear
    EVT_KEY_SOFT_VP,//66 Not Used
    EVT_KEY_SOFT_MOBILETV,
    //67 Apps Send To UISH , Go to the DMB gui entry
    //Display Type : Front, Rear
    EVT_KEY_SOFT_INFO,
    //68 Apps Send To UISH , Go to the INFO(Driving Mode) gui entry
    //Display Type : Front, Rear
    EVT_KEY_SOFT_SETTINGS,
    //69 Apps Send To UISH , Go to the Settings gui entry, Set APP_ENTRY_POINT_T Entry in Event Command Field
    //Display Type : Front, Rear
    EVT_KEY_SOFT_BTPHONE,
    //70 Apps Send To UISH , Go to the BT gui entry
    //Display Type : Front, Rear
    EVT_KEY_SOFT_HVAC,
    //71 Apps Send To UISH , Go to the HVAC(Air Conditional) gui entry
    //Display Type : Front, Rear
    EVT_KEY_SOFT_RADIO,
    //72 Apps Send To UISH , Go to the Radio(FM/AM) gui entry
    //Display Type : Front, Rear
    EVT_KEY_SOFT_STORE, //73 Not Used
    EVT_KEY_SOFT_NAVI_SETTINGS,
    //74 Apps Send To UISH , Go to the Navi Setting gui entry
    //Display Type : Front, Rear
    EVT_KEY_SOFT_BT_SETTINGS,
    //75 Apps Send To UISH , Go to the BT Setting gui entry
    //Display Type : Front, Rear
    EVT_KEY_SOFT_GALLERY,
    //76 Apps Send To UISH , Go to the Photo Gallery gui entry, Set APP_ENTRY_POINT_T Entry in Event Command Field
    //Display Type : Front, Rear

    EVT_KEY_HU_EJECT,
    //77 User Input Face Flate Eject HW Key
    //Display Type : Common
    EVT_KEY_HU_TUNE_LEFT,
    //78 User Input Face Flate Tune HW Key
    //Display Type : Common
    EVT_KEY_HU_TUNE_RIGHT,
    //79 User Input Face Flate Tune HW Key
    //Display Type : Common
    EVT_KEY_HU_TUNE_PRESSED,
    //80 User Input Face Flate Tune HW Key
    //Display Type : Common
    EVT_KEY_CCP_BACK,
    //81 User Input CCP Back HW Key
    //Display Type : Common
    EVT_KEY_CCP_HOME,
    //82 User Input CCP Back HW Key, UISH only using
    //Display Type : Common
    EVT_KEY_CCP_MENU,
    //83User Input CCP Menu HW Key
    //Display Type : Common
    EVT_KEY_CCP_JOG_DIAL_TOP_LEFT,
    //84 User Input CCP Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_CCP_JOG_DIAL_LEFT,
    //85 User Input CCP Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT,
    //86 User Input CCP Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_CCP_JOG_DIAL_UP,
    //87 User Input CCP Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_CCP_JOG_DIAL_CENTER,
    //88 User Input CCP Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_CCP_JOG_DIAL_DOWN,
    //89 User Input CCP Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT,
    //90 User Input CCP Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_CCP_JOG_DIAL_RIGHT,
    //91 User Input CCP Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT,
    //92 User Input CCP Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_RRC_JOG_DIAL_TOP_LEFT,
    //93 User Input RRC Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_RRC_JOG_DIAL_LEFT,
    //94 User Input RRC Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT,
    //95 User Input RRC Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_RRC_JOG_DIAL_UP,
    //96 User Input RRC Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_RRC_JOG_DIAL_CENTER,
    //97 User Input RRC Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_RRC_JOG_DIAL_DOWN,
    //98 User Input RRC Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT,
    //99 User Input RRC Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_RRC_JOG_DIAL_RIGHT,
    //100 User Input RRC Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT,
    //101 User Input RRC Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_SOFT_HOME_LONG,//102 Not Used
    EVT_KEY_HU_JOG_DIAL_ENCODER,
    //103 User Input Face Flate Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_CCP_JOG_DIAL_ENCODER,
    //104 User Input CCP Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_RRC_JOG_DIAL_ENCODER,
    //105 User Input RRC Jog Dial HW Key
    //Display Type : Common
    EVT_KEY_CCP_SENSOR,//106
    EVT_KEY_HU_AM_FM,
    //107 User Input Face Flate AM/FM HW Key, UISH only Used
    //Display Type : Common
    EVT_KEY_HU_MEDIA,
    //108 User Input Face Flate Media HW Key, UISH only Used
    //Display Type : Common
    EVT_KEY_HU_INFO,//109 Not Used
    EVT_KEY_HU_XM,
    //110 User Input Face Flate XM HW Key, UISH only Used
    //Display Type : Common
    EVT_KEY_HU_QIBLA,
    //111 User Input Face Flate QIBLA HW Key, UISH only Used
    //Display Type : Common
    EVT_KEY_HU_FM,
    //112 User Input Face Flate FM HW Key, UISH only Used
    //Display Type : Common
    EVT_KEY_HU_AM,
    //113 User Input Face Flate AM HW Key, UISH only Used
    //Display Type : Common
    EVT_KEY_CCP_INFO,//114 Not Used
    EVT_KEY_CCP_MODE,
    //115 User Input CCP Mode HW Key, UISH only Used
    //Display Type : Common
    EVT_KEY_CCP_RADIO_XM,
    //116 User Input CCP Radio/XM HW Key, UISH only Used
    //Display Type : Common
    EVT_KEY_CCP_QIBLA,
    //117 User Input CCP QIBLA HW Key, UISH only Used
    //Display Type : Common
    EVT_KEY_CCP_AM,
    //118 User Input CCP AM Key, UISH only Used
    //Display Type : Common
    EVT_KEY_CCP_FM,
    //119 User Input CCP AM Key, UISH only Used
    //Display Type : Common
    EVT_KEY_SOFT_XM,
    //120 Apps Send To UISH , Go to the XM Audio gui entry, Set APP_ENTRY_POINT_T Entry in Event Command Field
    //Display Type : Front, Rear
    EVT_KEY_SOFT_DAB,
    //121 Apps Send To UISH , Go to the Photo DAB gui entry, Set APP_ENTRY_POINT_T Entry in Event Command Field
    //Display Type : Front, Rear
    EVT_KEY_SOFT_CLOCK,
    //122 Apps Send To UISH , Go to the Photo Clock(Black) gui entry
    //Display Type : Front, Rear
    EVT_KEY_HU_POWER_ON,//123 Not Used
    EVT_KEY_HU_POWER_OFF,//124 Not Used
    EVT_DISC_STATUS,
    //125 App Send To UISH, Disc Status Event(AVP Send to UISH), Using EVENT_DISC_STATUS_T Struct
    //Display Type : Common
    EVT_NAVI_GPS_TIME_SET,//126
    EVT_CLOCK_TIME_SET,//127
    EVT_POPUP_RESPONSE,
    //128 System POPUP Response Event, Using POPUP_RESPONSE_T Struct
    //Display Type : Front, Rear
    EVT_REQUEST_PRE_FG,
    //129 When AFW recive EVT_REQUEST_FG, AFW Sent to App, This Event's mean was QML and Other resource Loding
    //Display Type : Front, Rear
    EVT_BT_INCOMING_CALL,
    //130 BT Call Staus Event, Using EVENT_BT_CALL_STATUS_DATA Struct
    //Display Type : Common
    EVT_BT_CALL_ACCEPTED,  //131 Not Used
    EVT_BT_CALL_REJECTED, //132  Not Used
    EVT_BT_INCOMING_CALL_OSD_UPDATE, //133 Not Used
    EVT_CAMERA_CLOSE, //134 Not Used
    EVT_KEY_SOFT_XM_AUDIO, //135
    EVT_KEY_SOFT_XM_DATA, //136
    EVT_KEY_SWRC_LONG_CALL_SEND,
    //137 Bt App Send to UISH,
    //Display Type : Front, Rear
    EVT_START_AUDIO_PLAY_IN_BG, //138 Not Used
    EVT_BT_CALL_STATUS,
    //139 Bt App Send To UISH, Using EVENT_BT_CALL_STATUS_DATA Struct,
    //Display Type : Common
    EVT_BT_CALL_ENDED,
    //140 Bt App Send To UISH,
    //Display Type : Common
    EVT_BT_MUSIC_PLAY, //141
    EVT_BT_MUSIC_STOP,//142
    EVT_BT_MUSIC_PAUSE,//143
    EVT_BT_MUSIC_PREV_TRACK,//144
    EVT_BT_MUSIC_NEXT_TRACK,//145
    EVT_BT_MUSIC_REPEAT_ALL_ON,//146
    EVT_BT_MUSIC_REPEAT_ALL_OFF,//147
    EVT_BT_MUSIC_REPEAT_ONE_ON,//148
    EVT_BT_MUSIC_REPEAT_ONE_OFF,//149
    EVT_BT_MUSIC_SHUFFLE_ON,//150
    EVT_BT_MUSIC_SHUFFLE_OFF,//151
    EVT_BT_MUSIC_SET_INFO,//152
    EVT_BT_MUSIC_GET_MODE,//153
    EVT_BT_MUSIC_CONN_CONFIG,//154
    EVT_BT_MUSIC_ENABLED,//155
    EVT_BT_MUSIC_DISABLED,//156
    EVT_BT_MUSIC_PLAYBACK_STARTED,//157
    EVT_BT_MUSIC_PLAYBACK_PAUSED,//158
    EVT_KEY_SOFT_PANDORA,
    //159 Apps Send To UISH , Go to the Pandora gui entry, Set APP_ENTRY_POINT_T Entry in Event Command Field
    //Display Type : Front, Rear
    EVT_TA_RDS_START,
    //160 UISH send to Apps, RDS TA Start Infomataion
    EVT_TA_RDS_STOP,
    //161 UISH Send to Apps, RDS TA End Infomataion
    EVT_DAB_REQUEST_RADIO, //162        /*!< DAB request radio to play */
    EVT_RADIO_RESPONSE_DAB,  //163       /*!< Radio responseto DAB with freqency */
    EVT_RADIO_END_REQUEST,   //164       /*!< DAB requst Radio for end playing */
    EVT_POPUP_STATE,          //165 Not Used
    EVT_VIDEOPLAY_STATE,      //166      /*!< Event  used to check Video Playing Status - To use in command field with EVT_POST_EVENT */
    EVT_ASPECTRATIO_CHANGE, //167
    EVT_INFO_DRIVING_MODE,        //168 Not Used  /*!< Event  used to launch AppInfo with Driving mode screen */
    EVT_INFO_VEHICLE_HEIGHT,       //169 Not Used /*!< Event  used to launch AppInfo with Vehicle height screen */
    EVT_TA_DAB_START,
    //170 UISH Send to Apps   DAB TA Start Infomataion
    EVT_TA_DAB_END,
    //171 UISH Send to Apps   RDS TA End Infomataion
    EVT_UPDATE_OSD_ON_TA,    //172 Not Used
    EVT_DAB_CURRENT_PLAY,     //173      /*!< DAB inform Radio whenever DAB current play changes */
    EVT_DAB_STATUS_REQUEST,    //174     /*!< DAB check linking possibility with Radio on weak signal */
    EVT_RADIO_STATUS_RESPONSE,  //175    /*!< Radio responds to DAB EVT_DAB_STATUS_REQUEST */
    EVT_KEY_AGREE_PRESSED,
    //176 StandBy App Send To UISH, Agree Press Event
    EVT_INFO_INTERACTIVE_GUIDE,    //177 Not Used
    EVT_INFO_E_MANUAL,
    //178 Apps Send To UISH , Go to the E-Manual gui entry, Set APP_ENTRY_POINT_T Entry in Event Command Field
    //Display Type : Front, Rear
    EVT_SOFT_ENG_MODE_SOFTWARE,  //179           /*!< Event  used to launch Engineering Mode software screen */
    EVT_SOFT_ENG_MODE_HARDWARE,   //180          /*!< Event  used to launch Engineering Mode hardware screen */
    EVT_SOFT_ENG_MODE_SW_UPDATE,  //181          /*!< Event  used to launch Engineering Mode software update screen */
    EVT_SOFT_ENG_MODE_DRIVE_RESTRICTION, //182   /*!< Event  used to launch Engineering Mode drive restriction screen */
    EVT_SOFT_ENG_MODE_DIAGNOSTIC,   //183        /*!< Event  used to launch Engineering Mode diagnostic screen */
    EVT_SOFT_ENG_MODE_VARIANT,      //184        /*!< Event  used to launch Engineering Mode variant screen */
    EVT_SOFT_ENG_MODE_SYS_CONFIG,  //185         /*!< Event  used to launch Engineering Mode system config screen */

    EVT_KEY_HOME_MENU_LOCK,         //186 Not Used        /*!< Event  used to lock home menu when driving mode */
    EVT_KEY_HOME_MENU_UNLOCK,      //187 Not Used         /*!< Event  used to unlock home menu when non driving mode */
    EVT_BLUELINK_CALL_ONGOING,      //188 UISH send To App, BlueLink Call is Ongoing/*!< Event to indicate Bluelink call is ongoing */
    EVT_BLUELINK_CALL_IDLE,          //189 UISH send To App, BlueLink Call is Idle/*!< Event to indicate Bluelink call is idle */
    EVT_KEY_SOFT_AVP,
    //190 Apps Send To UISH , Go to the AVP gui entry, Set APP_ENTRY_POINT_T Entry in Event Command Field
    //Display Type : Front, Rear
    EVT_ENGINEERING_SETTING_DRIVINGRESTRICTION, // 191 /*!< Event used to Control Driving Restrictions from Engineering App */
    EVT_ENGINEERING_SETTING_REARCAMERASIGNAL,  // 192  /*!< Event used to Control Rear Camera from Engineering App */
    EVT_KEY_SOFT_DAB_RADIO,
    //193 Apps Send To UISH , Go to the DAB/Radio gui entry, Set APP_ENTRY_POINT_T Entry in Event Command Field
    //Display Type : Front, Rear
    EVT_KEY_HU_DAB,
    //194 User Input Face Flate DAB Key, UISH only Used
    //Display Type : Common
    EVT_KEY_CCP_SETTINGS,
    //195 User Input CCP Setting Key, UISH only Used
    //Display Type : Common
    EVT_KEY_RRC_FM,
    //196 User Input RRC FM Key, UISH only Used
    //Display Type : Common
    EVT_KEY_RRC_AM,
    //197 User Input RRC AM Key, UISH only Used
    //Display Type : Common
    EVT_KEY_RRC_DAB,
    //198 User Input RRC DAB Key, UISH only Used
    //Display Type : Common
    EVT_KEY_RRC_RADIO_DMB,
    //199 User Input RRC RADIO_DMB Key, UISH only Used
    //Display Type : Common
    EVT_KEY_RRC_RADIO,
    //200 User Input RRC RADIO Key, UISH only Used
    //Display Type : Common
    EVT_KEY_RRC_DMB,
    //201 User Input RRC DMB Key, UISH only Used
    //Display Type : Common
    EVT_KEY_RRC_QIBLA,
    //202 User Input RRC QIBLA Key, UISH only Used
    //Display Type : Common
    EVT_KEY_RRC_CH_FW,
    //203 User Input RRC CH_FW Key
    //Display Type : Common
    EVT_KEY_RRC_MEDIA,
    //204 User Input RRC MEDIA Key, UISH only Used
    //Display Type : Common
    EVT_KEY_RRC_CH_BK,
    //205 User Input RRC CH_BK Key
    //Display Type : Common
    EVT_KEY_RRC_DISP,
    //206 User Input RRC DISP Key, UISH only Used
    //Display Type : Common
    EVT_KEY_HU_SETTINGS,
    //207 User Input Face Flate Setting Key, UISH only Used
    //Display Type : Common
    EVT_KEY_CCP_RADIO,
    //208 User Input CCP RADIO Key, UISH only Used
    //Display Type : Common
    EVT_KEY_RRC_POWER_L,
    //209 User Input RRC LEFT POWER Key, UISH only Used
    //Display Type : Common
    EVT_KEY_SOFT_SEAT_CONTROL,  //210 Not Used
    EVT_EPG_DAB_START,  //211 Not Used
    EVT_KEY_SOFT_AHA,
    //212 Apps Send To UISH , Go to the AHA Radio gui entry, Set APP_ENTRY_POINT_T Entry in Event Command Field
    //Display Type : Front, Rear
    EVT_EMERGENCY_ACTIVATED,  //213 UISH Send To Apps, Using IBOX_EMERGENCY_STATE_T Struct, /*!< Event to indicate Emergency is activated */
    EVT_EMERGENCY_DEACTIVATED,  //214  UISH Send To Apps,/*!< Event to indicate Emergency is deactivated */
    EVT_DRS_CHANGE, //215 Not Used
    EVT_KEY_DAB_REQUEST_FG, //216 Not Used
    /* Softkey events to start IBox applications */
    EVT_KEY_SOFT_IBOX_APP_BLUELINK_PHONE = 400, //400
    EVT_KEY_SOFT_IBOX_APP_BLUELINK_ASSISTENT, //401
    EVT_KEY_SOFT_IBOX_APP_BLUELINK_CENTER, //402
    EVT_KEY_SOFT_IBOX_APP_MAIL, //403
    EVT_KEY_SOFT_IBOX_APP_INTERNET, //404
    EVT_KEY_SOFT_IBOX_APP_SCHEDULE, //405
    EVT_KEY_SOFT_IBOX_APP_DNLA,    //406
    EVT_KEY_SOFT_IBOX_APP_YOUTUBE, //407
    EVT_KEY_SOFT_IBOX_APP_TWITTER, //408
    EVT_KEY_SOFT_IBOX_APP_TERMINAL_MODE, //409
    EVT_KEY_SOFT_IBOX_APP_STORE, //410
    EVT_KEY_SOFT_IBOX_APP_SMART_USB, //411
    EVT_KEY_SOFT_IBOX_APP_ECO_DRIVING_INFORMATION, //412
    EVT_KEY_SOFT_IBOX_APP_CONSUMABLES_MANAGEMENT, //413
    EVT_KEY_SOFT_IBOX_APP_POI_INFORMATION, //414
    EVT_KEY_SOFT_IBOX_APP_TETHERING,//415
    EVT_KEY_SOFT_IBOX_APP_APPLICATIONS,//416
    EVT_KEY_SOFT_IBOX_APP_WEATHER, //417
    EVT_KEY_SOFT_IBOX_APP_NEWS, //418
    EVT_KEY_SOFT_IBOX_APP_STOCK, //419
    EVT_KEY_SOFT_IBOX_APP_BLUELINK, //420
    EVT_KEY_SOFT_IBOX_APP_IDAS, //421
    EVT_KEY_SOFT_IBOX_APP_WIFI_SETTINGS, //422
    EVT_KEY_SOFT_IBOX_APP_INTERNET_SETTINGS, //423
    EVT_KEY_SOFT_IBOX_APP_APPLICATION_SETTINGS, //424
    EVT_KEY_SOFT_IBOX_APP_VEHICLE_DIAGNOSTICS, //425
    EVT_KEY_SOFT_IBOX_APP_MAINTENANCE_ALERT, //426
    EVT_KEY_SOFT_IBOX_APP_FIND_DEALER, //427
    EVT_KEY_SOFT_IBOX_APP_SCHEDULED_DIAGNOSTICS, //428
    EVT_KEY_SOFT_IBOX_APP_SCHEDULE_SERVICE, //429
    EVT_KEY_SOFT_IBOX_APP_SEND_MY_LOCATION, //430
    EVT_KEY_SOFT_IBOX_APP_SHARED_LOCATION, //431
    EVT_KEY_SOFT_IBOX_APP_BLUELINK_SETTING, //432
    EVT_KEY_SOFT_IBOX_APP_AUTO_DIAGNOSTICS, //433
    EVT_KEY_SOFT_IBOX_APP_DRS_LISTING_CLOSE,  //434 /*! < To close DRS pop and lauched iBox DLNA video listing */
    EVT_KEY_SOFT_IBOX_APP_ENGINEERING,  //435      /*! < To launch ibox with Engineering application */
    EVT_KEY_RRC_DEST,
    //436 User Input RRC DEST Key, UISH only Used
    //Display Type : Common
    EVT_KEY_CCP_SEEK_NEXT,
    //437 User Input CCP SEEK_NEXT KEY
    //Display Type : Common
    EVT_KEY_CCP_SEEK_PREV,
    //438 User Input CCP SEEK_PREV KEY
    //Display Type : Common
    EVT_KEY_RRC_SETTINGS,
    //439 User Input RRC Setting Key, UISH only Used
    //Display Type : Common
    EVT_KEY_PROJECTION_POPUP_ENTER,
    //440
    //Display Type : Common
    EVT_AV_MODE_CHANGE = 500,
    //500 UISH Send To Apps, AV Change Event
    //When App was BG Status UISHS Send this Event(Ths Event has Audio Mode Infomation), Using EVENT_MODE_CHANGED_DATA_T Stuct
    //Display Type : Common
    EVT_REQEUST_MUTE,
    //501 UISH Send To Apps, When User info Mute Key, UISH Send this event.
    //Display Type : Common
    EVT_REQEUST_UNMUTE,
    //502 UISH Send To Apps, When User info UnMute Key, UISH Send this event.
    //Display Type : Common
    EVT_STATUSBAR_INFO_CHANGED, //503 Not Used
    EVT_SETTINGS_CHANGED,
    //504 Setting App Send to UISH, and UISH Sent to Apps. When User change setting UISH send this Evnet. Using EVENT_SETTINGS_CHANGED_DATA_T Struct
    //Display Type : Front, Rear
    EVT_ACC_OFF,   //505 Not Used        /**< System off via volume key press */
    EVT_POPUP_HISTORY_RESPONSE, //506 Not Used
    EVT_REQUEST_VRTTS_VERSION,  //507 Not Used /**< To use in command field with EVT_POST_EVENT */
    EVT_RESPONSE_VRTTS_VERSION, //508 Not Used /**< To use in command field with EVT_POST_EVENT */
    EVT_BT_PAIRING_STATUS,
    //509 App Send To UISH, This Event has BT Pairing infomation, Using EVENT_BT_PAIRING_STATUS Stuct
    //Display Type : Common
    EVT_REQUEST_APP_EXIT,
    //510 UISH Send to App, App exit Event, Using in AFW
    //Display Type : Front, Rear
    EVT_UISH_INTERNAL, //511 Not Used
    EVT_HS_SCREEN_SWITCH,	//512 Not Used
    EVT_GLOBAL_SEARCH_EVENT,	//513 Not Used
    EVT_AVP_UI_CHANGE, //514 Not Used
    EVT_FB_OVERLAY_BLEND_ON, //515 Not Used
    EVT_FB_OVERLAY_BLEND_OFF, //516 Not Used
    EVT_CLONE_NOTIFICATION,
    //517 UISH Send To Apps, When Use Clone/Swap/DIH EMGD System call, Using EVENT_CLONE_NOTIFICATION_DATA_T sturcty
    //Display Type : Common
    EVT_DISPLAY_REAR_SCREEN_STATE,
    //518 UISH Send to Apps, When Rear LCD(Right or Left) ON/OFF status Changed this event sent to Apps, Using EVT_DISPLAY_REAR_SCREEN_STATE_DATA_T struct
    //Display Type : Front, Rear
    EVT_INFO_UI_CHANGE,	//519 Not Used
    EVT_START_VIDEO_TUNNING,
    //520 Lunch VIDE_TUNNING TEST APP(/app/bin/Video_tunning)
    EVT_AV_MODE_NOTIFY_FROM_VR,
    //521 UISH send to App, When AV mode Change by VR_COMMAND, UISH notify AV Mode to VR App. using EVT_COMMON_BASE_EVENT_DATA_T struct
    //Display Type : Common
    EVT_APP_IS_READY_TO_PLAY, //522 Not Used
    EVT_AV_TEMPORAL_MUTE,
    //523 UISH send to App Case : UISH Using Temporal Mute For POP NOISE,
    // App send to UISH : Need Mute Time For POP NOISE,
    //Display Type : Common
	EVT_AV_TEMPORAL_UNMUTE,
	//524 UISH send to App Case : Finish Temporal Mute For POP NOISE,
	// App send to UISH : Need UnMute For POP NOISE,
	//Display Type : Common
	EVT_BT_PANDORA_ENABLED, // 525 Not Used
	EVT_BT_PANDORA_DISABLED, // 526 Not Used
	EVT_BT_AHA_ENABLED, //527 Not Used
	EVT_BT_AHA_DISABLED, //528 Not Used

    // CAN (MICOM) related public events
    EVT_DRS_SHOW = 600,
    //600 UISH Send to Apps, Notify Driving Restriction Start
    //Display Type : Common
    EVT_DRS_HIDE,
    //601 UISH Send to Apps, Notify Driving Restriction End
    //Display Type : Common
    EVT_ALT_ON,
    //602 UISH Send to Apps, Notify DWS(ALT) ON
    //Display Type : Front
    EVT_ALT_OFF,
    //603 UISH Send to Apps, Notify DWS(ALT) OFF
    //Display Type : Front
    EVT_REQ_ENG_MODE_ENTER,
    //604 ENG App Lunch Event, Using Only UISH
    EVT_TERMINATE_ENG_MODE,
    //605 UISH Send to App, When exti ENG App, UISH Send this event to ENG App
    //Display Type : Front
    EVT_DIAGNOSTIC_TEST_START,  //606/**< Event from UISH to Radio/DMB/DAB/AVP to start the Diagnostic Test */
    EVT_DIAGNOSTIC_TEST_END,   //607 /**< Event from Radio/DMB/DAB/AVP to UISH to Notify End of the Diagnostic Test */
    EVT_FACTORY_RESET_REQUEST,  /**< Event from AppEngg-->UISH/UISH-->ToAllApps start the FactoryReset Test */
    //608 Eng App Sent to UISH, and UISH Sent to Apps
    //Display Type : Common
    EVT_FACTORY_RESET_RESPONSE, /**< Event from UISH-->AppEngg/ToAllApps-->UISH to Notify the FactoryReset Response */
    //608 Apps send to UISH, and UISH send to ENG App after summary
    //Display Type : Common
    EVT_START_DTC_REQ, //608
    EVT_SETTING_GPS_TIMEZONE_CHANGED, //608
    EVT_INITIAL_OK = 700,
    //700 Eng App Send to UISH,
    //Display Type : Common
    EVT_POWER_STATE_LOGIC_ON = 800,
    //800 UISH send to Apps,
    //Display Type : Common
    EVT_POWER_STATE_NORMAL_OFF,
    //801 UISH send to Apps,
    //Display Type : Common
    EVT_MEDIA_ENABLED = 900,
    EVT_JUKEBOX_ENABLED,
    EVT_USB1_ENABLED,
    EVT_USB2_ENABLED,
    EVT_IPOD1_ENABLED,
    EVT_IPOD2_ENABLED,
    EVT_MIRROR_LINK_ENABLED,

    // Navi related events
    EVT_NAVI_INIT_GPS = 1000,
    EVT_NAVI_INITIALIZE_REQUEST,
    EVT_NAVI_INITIALIZE_RESPONSE,
    EVT_NAVI_NOTIFY_CHANGE_ADMIN_DISTRICT,
    EVT_NAVI_USER_FUEL_INFO,
    EVT_NAVI_CPU_RESTRICTED,
    EVT_NAVI_CPU_NORMAL,
    EVT_REQUEST_NAVI_DISPLAY_STATUS, // request from navi app, to get the display status
    EVT_RESPONSE_NAVI_DISPLAY_STATUS,// response from UISH, containing display status
    EVT_NAVI_SHARED_IMAGE_INITIALIZE,
    EVT_NAVI_SHARED_IMAGE_UPDATE,
    EVT_NAVI_GLOBAL_SEARCH_START,
    EVT_NAVI_GLOBAL_SEARCH_RESPONSE,
    EVT_NAVI_POI,
    EVT_NAVI_SETTTING_UNIT,
    //IBOX POI event for Navi App
    EVT_NAVI_IBOX_POI,
    EVT_NAVI_COMMAND,
    EVT_NAVI_FULL_PATH,
    EVT_NAVI_FULL_PATH_TPEG,
    EVT_NAVI_OPEN_BROWSER_URL,
    EVT_NAVI_SETTTING_PHONE_PRIORITY,
    EVT_XM_REQUEST_NAVI,        /*!< XM application sends request to bring Navi foreground with entry point as command */
    EVT_NAVI_MANUAL_INPUT,      /*!< This event is used  VR app to launch navi*/
    EVT_NAVI_REQUEST_FULL_AV_MODE_CHANGE,	/*!< This event is used NAVIGATION when user wants to change from NAVI to Current AV app.. */
    EVT_NAVI_IBOX_REGISTRATION, /*! 1024 < Navi sends event to switch to iBox to registration */
    EVT_NAVI_IBOX_SIGN_IN,  /*! 1025 < Navi sends event to switch to iBox to sign-in */

    EVT_SOFT_KEY_MIRROR_LINK = 1100,
    EVT_BT_HOST_BD_ADDRESS,
    EVT_BT_DEV_BD_ADDRESS,
    EVT_BT_CONNECTION_STATUS,
    EVT_ML_REQUEST_BT_CONNECTION,
    EVT_ML_AUDIO_REQUEST,
    EVT_ML_AUDIO_START,
    EVT_ML_AUDIO_STOP,
    EVT_ML_STATUS_NOTIFY,

    EVT_DEVEL_RESERVED = 1200,
    EVT_DEVEL_RESERVED2, //USED
    EVT_DEVEL_RESERVED3,
    EVT_DEVEL_RESERVED4, //USED
    EVT_DEVEL_RESERVED5, //USED
    EVT_DEVEL_RESERVED6,
    EVT_DEVEL_RESERVED7,
    EVT_DEVEL_RESERVED8,
    EVT_DEVEL_RESERVED9, //USED
    EVT_DEVEL_RESERVED10,
    EVT_DEVEL_EC_CRASH,
    EVT_DEVEL_EC_PRESS_SOS_BTN,
    EVT_DEVEL_EC_PRESS_ETEST_BTN,
    EVT_DEVEL_EC_CONNECT_SOS_CALL,
    EVT_DEVEL_EC_CONNECT_ETEST_CALL,
    EVT_DEVEL_EC_PASSED_SECS_IN_ETEST,
    EVT_DEVEL_EC_ECALL_END,

    EVT_SUBMODE_CHANGED = 1300,

    EVT_SWAP_DC_ENABLED = 1400,
    EVT_SWAP_DC_DISABLED,
    EVT_CHANGE_OVERLAYPLANE,

    EVT_VR_INITIALIZE = 2000,  /**< Mediazen VR initialization event (UISH -> VR) */
    EVT_VR_INITIAL_OK,         /**< Mediazen VR initialization event (VR -> UISH) */
    EVT_VR_LIST_CREATE,        /** deprecated**/ /**< Request for Mediazen to recreate its variable VR list (App -> VR) */
    EVT_VR_END,
    EVT_VR_COMMAND,            /**< Generic VR command, where the sub-command will be put in event payload */
    EVT_VR_MODE_CHANGED,       /**< Notifies the VR Mode Change Events */
    EVT_LIST_CREATE_START,
    EVT_LIST_CREATE_END,
    EVT_VR_CANCEL,
    EVT_VR_RESTART,
    EVT_VR_STATUS_CHANGED,
    EVT_VR_START,
    EVT_VR_CANCEL_APP,
    EVT_CONNECTED_VR_REQUEST_START,

    // Text-To-Speech commands
    EVT_MIX_PLAY_START = 2500, /**< MIX request to open Mix channel */
    EVT_MIX_PLAY_STOP,         /**< MIX request to stop Mix play */
    EVT_MIX_PLAY_END,          /**< MIX request to close Mix play */

	EVT_CONNECTED_CAR_MOUNTED = 2800,
	EVT_CONNECTED_CAR_ENABLED,
	EVT_CONNECTED_CAR_REMOVED,
	EVT_KEY_SOFT_CARPLAY,
	EVT_KEY_SOFT_ANDROID_AUTO,
	EVT_REQUEST_CONNECTED_CAR_AUDIO,
	EVT_REQUEST_CONNECTED_CAR_GUI_MODE,
	EVT_RESPONSE_CONNECTED_CAR_GUI_MODE,
	EVT_CONNECTED_CAR_NAVI_STATUS,
	EVT_CONNECTED_CAR_CALL_STATUS,
	EVT_NAVI_ROUTE_GUIDANCE_STATUS,
	EVT_REQUEST_BT_DISCONNECT,
	EVT_CARPLAY_BACK_KEY,
	EVT_SET_ROUTE_GUIDE,
	EVT_CARPLAY_ALERT_START,
	EVT_CARPLAY_ALERT_END,
	EVT_REQUET_BT_SETTING_DEL,
	EVT_REQUEST_CONNECTED_CAR_RETURN_GUI_MODE,
	EVT_REJECT_REQUEST_GUI_MODE,
	EVT_AAP_VR_KEY_BLOCK_START,
	EVT_AAP_VR_KEY_BLOCK_END,
	EVT_CARPLAY_ALERT_MICOM_START,
	EVT_CARPLAY_ALERT_MICOM_END,
	EVT_AGREE_REAL_HIDE,
	EVT_RETURN_CONNECTED_CAR_AUDIO,
	EVT_CONFIRM_PROJECTION,

    EVT_UISH_SYNC_STATE_MACHINES = 3000, /**< UI State Manager internal event to synchronize state maschines */


    // device events
    EVT_IPOD_MOUNTED,
    EVT_IPOD_REMOVED,

    EVT_AUX_PLUGEDIN,
    EVT_AUX_REMOVED,

    EVT_CDDVD_MOUNTED,
    EVT_CDDVD_REMOVED,

    EVT_DEV_REMOVED,

    EVT_UISH_LONG_KEYPRESS_HOME,
    EVT_UISH_LONG_KEYPRESS_BACK,

    EVT_IPOD_UDEV_DETECTED,     /**< @deprecated */
    EVT_IPOD_UDEV_REMOVED,      /**< @deprecated */

    EVT_IPHONE_UDEV_DETECTED,   /**< @deprecated */
    EVT_IPHONE_UDEV_REMOVED,    /**< @deprecated */

    EVT_CALL_START,
    EVT_CALL_END,

    /**<  message back from AppUpgrade */
    EVT_RESPONSE_UPGRADE_SUCCESS,
    EVT_RESPONSE_UPGRADE_FAILURE,

    /**<  message from AppSettings and engineer mode */
    EVT_REQUEST_UPGRADE,

    /**<  message back from AppUpgrade */
    EVT_RESPONSE_UPGRADE_CANCEL,

    /**< dynamics messages */
    EVT_SOFT_DYNAMICS_RADIO,
    EVT_SOFT_DYNAMICS_DMB,
    EVT_SOFT_DYNAMICS_XM,
    EVT_SOFT_DYNAMICS_RDS,
    EVT_SOFT_DYNAMICS_NAVI,
    EVT_SOFT_DYNAMICS_SETTINGS,
    EVT_SOFT_DYNAMICS_MAP_CARE,

    /**< System options from micom */
    EVT_SYSTEM_OPTIONS,
    /**< System options set by AppEnginner*/
    EVT_SYSTEM_OPTIONS_SET,

    /**< entry point for settings from navi */
    EVT_NAVI_ENTRY_POINT_SETTINGS,

    EVT_ENG_REQUEST_CONNECTIVITY,
    EVT_ENG_RESPONSE_CONNECTIVITY,

    /**< dynamics messages. Added at last for making binary compatible */
    EVT_SOFT_DYNAMICS_HDRADIO,
    EVT_SOFT_DYNAMICS_DAB,
    EVT_SOFT_DYNAMICS_SOUND,                /*!< Event for Dynamics Sound */

	EVT_MMC_MOUNTED,
	EVT_MMC_REMOVED,

    EVT_NOTIFY_VR_STATUS,       /*!< Event for canceling Radio Freq changing while VR is foreground */
    EVT_SOFT_DYNAMICS_DEALER_MODE,
    EVT_SOFT_DYNAMICS_DAB_DEALER_MODE,
    EVT_SOFT_DYNAMICS_RDS_DEALER_MODE,
    EVT_SOFT_DYNAMICS_DMB_DEALER_MODE,
    EVT_SOFT_DYNAMICS_XM_DEALER_MODE,

    /**<  start range with 3100 */
    /**<  DSME stuff to all apps */
    EVT_REQUEST_SAVE_UNSAVED_DATA = 3100,
    EVT_RESPONSE_SAVE_UNSAVED_DATA,

    EVT_ALL_SLEEP_CANCEL_ACTIVATE,

    EVT_RADIO_FREQUENCY_CHANGE, /**< Request for APP_RADIO to change the frequency and frequency
                                 *   band. This event comes along with the payload defined by
                                 *   EVENT_RADIO_FREQUENCY_CHANGE_DATA_T
                                 */

    /**< CAN (MICOM) related internal events */
    EVT_DRS_ON = 3200,
    EVT_DRS_OFF,
    EVT_DWS_ON,
    EVT_DWS_OFF,
    EVT_REAR_GEAR_ENGAGED,
    EVT_REAR_GEAR_REMOVED,

    /**< Event used to finalize launch of Engg Mode app, request FG */
    EVT_APP_ENG_FG,

    /**< Event used to start the last active av app in background>*/
    EVT_RESTORE_LAST_AV_MODE,
    /**< Event used to update the OSD (statusbar)>*/
    EVT_UPDATE_OSD,

    /**< Events used to notify HVAC and CAMERA apps when IGN or ACC are changed> */
    EVT_IGN_CHANGED,
    EVT_ACC_CHANGED,

    EVT_POWER_STATE_LOGIC_OFF,      /*! < This event will be sent on ACC OFF */
    EVT_POWER_STATE_NORMAL_ON,      /*! < This event will be sent on ACC ON */

    /**< Events used to query uish acc/IGN status>*/
    EVT_REQUEST_ACC_STATUS,
    EVT_REQUEST_IGN_STATUS,
    /**< Events used to give response to apps with acc/IGN status>*/
    EVT_RESPONSE_ACC_STATUS,
    EVT_RESPONSE_IGN_STATUS,

    EVT_NAVI_GPS_TIME_GET,
    //Changes of BR 2785,2808,2832 in Plan A
    /**< Event used to notify AppStatusBar from StatusBarAdapter and vice versa> */

    EVT_XSAMPA_AVAILABLE, //Support for X-Sampa feature
    EVT_STATUS_BAR_UPDATE,// event for AppStatusBar

    /**< Events to notify apps about JukeBox Format process >**/
    EVT_FORMAT_JUKEBOX_START,
    EVT_FORMAT_JUKEBOX_END,
    /**< Event to inform RSE LOCK info status to App settings from UISH at bootup time >*/
    EVT_RSE_LOCK_STATUS,
    /**< Event for Beep Play with beep type as payload >*/
    EVT_BEEP_PLAY,

    EVT_NOT_PROCESSED, /*! < This event will be sent to HVAC when Camera is FG */

	EVT_REQUEST_ABNORMAL_TERMINATION,	/*! < for software watch dog timer > */
    
    EVT_REQUEST_NAVI_AUTHORIZATION_FAIL,    // 네비 SD 인증관련 실패시 표시하는 팝업 ID 추가..

    EVT_REQUEST_XM_ABNORMAL_TERMINATION,

    /**<  Start next range with 4000 */
    EVT_FM_LAUNCH_STATE = 4000,
    EVT_PLAY_VIDEO,
    EVT_SHOW_PHOTO,
    EVT_START_XTERM,
    EVT_HIDE_STATUS_BAR,
    EVT_SHOW_STATUS_BAR,
    EVT_SHOW_SETTINGS_SCREEN,
    EVT_AUTO_PLAY_IN_BG,
    EVT_STATUS_BAR_TOUCHED,
    /**< Events from appSettings to notify apps to reset settings >**/
    EVT_INITIALIZATION_REQUEST,
    EVT_FRONT_SCREEN_STATUS,
    /*!-- DHAVN_TELECA_ininsati_CR12683 -- 20120508 IBox and Video mixing issue --*/
    EVT_DISPLAYING_IBOX, /**< Event is sent to AVP to move video to other display >*/
    /*!-- DHAVN_TELECA_ininsiko_CR12753--20120810 TA Hardkey addition in A11 Spec  --*/
    EVT_KEY_HU_TA, /**< Event to inform HU TA key press >*/

    EVT_REQUEST_AUDIO_PATH = 5000,
    /*!DHAVN_TELECA_ininnmac_dvdSettings_CR_11552--*/
    EVT_VC_CONTENT,
    EVT_VC_ITEM_SELECTED,
    EVT_ENABLE_DVDSETTINGS,
    EVT_DISABLE_DVDSETTINGS,
    EVT_REQUEST_DVD_INFO,
    EVT_SHOW_DESKTOP_COMPLETE, /**< Event sent to Camera when Show desktop is complete >*/

    EVT_USB_PHOTO_STATUS, /**< Event sent by AVP Application when there is USB Photo active mode >*/
    EVT_POP_UP_CLOSED, /** <Event sent to the TopApp when system pop is getting close to ignore touch event>*/

//{ add by eunhye 2012.09.21 for CR 9243
    EVT_VIDEO_MOVE_START, /**< Event sent by AVP Application when there is video move start mode>*/
    EVT_VIDEO_MOVE_END, /**< Event sent by AVP Application when there is video move end mode >*/
//} add by eunhye 2012.09.21
    EVT_SOUND_STATE_IBOX,    /*!< When system audiobility changed to iBox; send this event */
    /*!-- DHAVN_TELECA_ininsati_CR14752 -- 20121710
            [DD]Juke box video screen is overlapping with camera screen --*/
    EVT_CAMERA_OFF_EVENT,    /*!< When Rear gear is removed; send this event to AVP & DMB app */
    EVT_MOVE_VIDEO_EVENT,    /**< Event is sent to AVP to move video to other display */
    EVT_PANDORA_STATUS,      /**< Event is sent from Pandora; to inform about its status */
    EVT_FRONT_LCD_ON,		/**< Event is sent from UISH to apps to inform front LCD backlight status */
    EVT_FRONT_LCD_OFF,   	/**< Event is sent from UISH to apps to inform front LCD backlight status */
    EVT_REQUEST_DIVX_REG_INFO, /* request for AVP to reseive Divx info*/
    EVT_RESPONSE_DIVX_REG_INFO, /* response from AVP*/
    EVT_MOTOR_ERROR_POPUP_SHOW,  /**< Show Motor Error Popup >*/
    EVT_AHA_STATUS,  /** Event is sent from Aha to inform about its status*/
    EVT_NAVI_GPS_AVAILABLE_STATUS,  /**< Event is sent from Navi to AppSettings to inform GPS available status */
    EVT_DISASTER_INFO_POPUP_SHOW,  /*!< Event to show DMB disaster info popup */
    EVT_VIDEO_FILE_SUPPORT,  /*<Event sent from AVP to return whether file supports or not*/

    EVT_NO_MEDIA_CONTENETS = 6000,  /**< Event is sent from AVP to UISH to switch back to previous AV mode */

    /** new events */
    EVT_REQUEST_SYSTEM_POPUP,
    EVT_CAMERA_ENABLE,
    EVT_CAMERA_DISABLE,
    EVT_EPG_REQUEST_FG,
    /* removed
    EVT_BLUELINK_CALL_START,
    EVT_BLUELINK_CALL_END,
    EVT_BT_CALL_ONGOING,
    */
    EVT_VIDEOGUIDE_REQUEST_PLAY,
    EVT_VIDEOGUIDE_PLAY_STATUS,
    EVT_FILEOPERATION_STOP, // added by lssanh 2013.02.07 2USB
    EVT_NAVI_BOOT_END, 
    EVT_APP_BOOT_END,
    EVT_AV_ON,
    EVT_AV_OFF,
    EVT_SHOW_LOCAL_POPUP,
    EVT_HIDE_LOCAL_POPUP,
    EVT_REQUEST_XM_CHANGE,
    EVT_JUKEBOX_CAPACITY_CHANGE,
    EVT_FM_MEDIA_SCAN_READ_START,
    EVT_FM_MEDIA_SCAN_READ_END,
    EVT_AGREE_SHOW_OSD,
    EVT_BL_CALL_STATUS,
    EVT_REQUEST_OUTGOING_CALL,
    EVT_SHOW_IBOX_RSSI_BAR,
    EVT_HIDE_IBOX_RSSI_BAR,
    EVT_BT_PREVIOUS_BACK,
    EVT_REQUEST_INTERACTIVE_STOP,
    EVT_UPDATE_CLUSTER,
    EVT_JUKEBOX_COPY_START,
    EVT_JUKEBOX_COPY_END,
    EVT_MP_AUTOTEST_START,
    EVT_MP_AUTOTEST_TERMINATE,
    EVT_BT_MUSIC_STREAMING_STARTED,
    EVT_BT_MUSIC_STREAMING_PAUSED,
    EVT_BT_MUSIC_GET_INFO,
    EVT_APP_NAVI_FG,
    EVT_APP_NAVI_BG,
    EVT_BT_MUSIC_TRACK_CHANGED,
    EVT_SOFT_DYNAMICS_AVP,
    EVT_CLUSTER_UPDATE_BLOCK,
    EVT_CLUSTER_UPDATE_UNBLOCK,
    EVT_BT_UI_CHANGE,
    EVT_SETTING_FG_RECEIVED,
    EVT_DISPLAY_ON,
    EVT_DISPLAY_OFF,
    EVT_BT_HANGUP_REQUEST,
    EVT_TOUCH_BACK_KEY,
    EVT_LAUNCH_GVOICE_IN_NAVI,
    EVT_NAVI_IBOX_GVOICE,
    EVT_REQUEST_SYSTEM_POPUP_CLOSE,
    EVT_NOTIFY_AV_MODE,
    EVT_REQUEST_POP_MUTE,
    EVT_REQUEST_POP_UNMUTE,
    EVT_NOTIFY_APPS_READY,
    EVT_NAVI_BACK_KEY,
    EVT_NAVI_BACK_CANCEL,
    EVT_FM_VIDEO_STATE_CHANGED,
    EVT_FM_VIDEO_HIDE,
    EVT_MP_SD_CARD_CHECK_REQ,
    EVT_MP_SD_CARD_CHECK_RES,
    EVT_NAVI_REQUEST_FG,
    EVT_TA_STATUS,
    EVT_SWRC_CALL,
    EVT_SWRC_CALL_LONG,
    EVT_USE_DISPLAY_SETTING_VALUE,
    EVT_NAVI_STATION_LIST_UPDATE,
    EVT_REQUEST_RENDER_OFF,
    EVT_REQUEST_RENDER_ON,
    EVT_NAVI_TA_VALUE_NOTIFY,
    EVT_REQUEST_SEND_TUNE,
    EVT_GLOBAL_NAVI_POPUP_BACK_KEY,
    EVT_MUTE_CANCEL, // 6069
    EVT_BL_INCOMMING_END,
    EVT_BT_INCOMMING_END,
    EVT_HIDE_BT_SWITCH_CALL_POPUP,
    EVT_BT_PAIRING_POPUP,
    EVT_BT_PHONEBOOK_DOWNLOAD_FAIL_POPUP,
    EVT_REQUEST_BT_GUI_STATUS_CHANGE,
    EVT_AUDIO_SOURCE_ACQUIRE,
    EVT_AUDIO_SOURCE_RELEASE,
    EVT_CONNECTED_CAR_HANGUP_REQUEST,
    EVT_RESPONSE_CONNECTED_MIX_CH,
    EVT_INFO_TO_CARPLAY_FROM_CLOCK,
    // for dh protocol test
    EVT_MP_DH_SET_ENABLE_REQ = 9000,
    EVT_MP_DH_SET_READY,
    EVT_MP_DEVICEID_REQ,
    EVT_MP_DEVICEID_RES,
    EVT_MP_GPS_START_REQ,
    EVT_MP_GPS_RESULT_VALUE_REQ,
    EVT_MP_GPS_RESULT_VALUE_RES,
    EVT_MP_GPS_END_REQ,
    EVT_MP_GYRO_START_REQ,
    EVT_MP_GYRO_RESULT_VALUE_REQ,
    EVT_MP_GYRO_RESULT_VALUE_RES,
    EVT_MP_GYRO_DR_REQ,
    EVT_MP_GYRO_DR_RES,
    EVT_MP_GYRO_SPEED_VALUE_REQ,
    EVT_MP_GYRO_SPEED_VALUE_RES,
    EVT_MP_BT_SEARCH_REQ,
    EVT_MP_BT_CONNECT_MAC_REQ,
    EVT_MP_BT_RESET_REQ,
    EVT_MP_BT_INCOMING_ACCEPT_REQ,
    EVT_MP_BT_CALL_DISCONNECT_REQ,
    EVT_MP_BT_VOLUME_CHANGE_REQ,
    EVT_MP_GET_FREQUENCY_REQ,
    EVT_MP_GET_FREQUENCY_RES,
    EVT_MP_FM_FREQ_FIX_REQ,
    EVT_MP_AM_FREQ_FIX_REQ,
    EVT_MP_TA_ON_REQ,
    EVT_MP_TA_OFF_REQ,
    EVT_MP_RDS_PS_NAME_REQ,
    EVT_MP_TUNER_ALGIN_START_REQ,
    EVT_MP_TUNER_ALGIN_VALUE_REQ,
    EVT_MP_DMB_START_REQ,
    EVT_MP_DMB_SEARCH_START_REQ,
    EVT_MP_DMB_SEARCH_END_REQ,
    EVT_MP_DMB_RESULT_VALUE_REQ,
    EVT_MP_DMB_RESULT_VALUE_RES,
    EVT_MP_DMB_BER_RESULT_VALUE_REQ,
    EVT_MP_DMB_BER_RESULT_VALUE_RES,
    EVT_MP_KEY_TEST_MODE_ON_REQ,
    EVT_MP_KEY_TEST_MODE_OFF_REQ,
    EVT_MP_REMOCON_VALUE_RES,
    EVT_MP_REMOCON_MODE_ON_REQ,
    EVT_MP_REMOCON_MODE_OFF_REQ,
    EVT_MP_KEY_TEST_RESULT_REQ,
    EVT_MP_KEY_TEST_RESULT_RES,
    EVT_MP_GET_MODE_REQ,
    EVT_MP_GET_MODE_RES,
    EVT_MP_MODE_CHANGE_REQ,
    EVT_MP_SEEK_UP_REQ,
    EVT_MP_SEEK_DOWN_REQ,
    EVT_MP_TRACK_UP_REQ,
    EVT_MP_TRACK_DOWN_REQ,
    EVT_MP_VOLUME_LEVEL_REQ,
    EVT_MP_VOLUME_LEVEL_RES,
    EVT_MP_MP_MODE_ON_REQ,
    EVT_MP_MP_MODE_OFF_REQ,
    EVT_MP_DTC_REQ,
    EVT_MP_DTC_RADIO_TUNER_MODULE_RES,
    EVT_MP_DTC_DMB_MODULE_RES,
    EVT_MP_DTC_DECK_RES,
    EVT_MP_DTC_XM_SIRIUS_MODULE_RES,
    EVT_MP_DTC_BT_MODULE_RES,
    EVT_MP_DTC_GPS_MODULE_RES,
    EVT_MP_DTC_IOCOMPAION_RES,
    EVT_MP_DTC_DAB_MODULE_RES,
    EVT_MP_DTC_DIRANA_RES,
    EVT_MP_DTC_RDS_TUNER_RES,
    EVT_MP_DTC_AUX_RES,
    EVT_MP_FACTORY_INIT_REQ,
    EVT_MP_WRITE_WIP_ID_REQ,
    EVT_MP_WRITE_WIP_ID_RES,
    EVT_MP_READ_WIP_ID_REQ,
    EVT_MP_READ_WIP_ID_RES,
    EVT_MP_VOLUME_LEVEL_UP_REQ,
    EVT_MP_VOLUME_LEVEL_UP_RES,
    EVT_MP_VOLUME_LEVEL_DOWN_REQ,
    EVT_MP_VOLUME_LEVEL_DOWN_RES,
    EVT_MP_RDS_PS_NAME_RES,
    EVT_MP_DAB_BER_RESULT_VALUE_REQ,
    EVT_MP_DAB_BER_RESULT_VALUE_RES,
    EVT_MP_HDRADIO_BER_RESULT_VALUE_REQ,
    EVT_MP_HDRADIO_BER_RESULT_VALUE_RES,
    EVT_MP_BT_CONNECT_MAC_RES,
    EVT_MP_XM_BROAD_NAME_REQ,
    EVT_MP_XM_BROAD_NAME_RES,
    EVT_MP_GPS_POSITION_REQ,
    EVT_MP_GPS_POSITION_RES,
    EVT_MP_BT_AUDIO_CH_OPEN_REQ,
    EVT_MP_BT_AUDIO_CH_CLOSE_REQ,
    EVT_MP_VR_START_REQ,
    EVT_MP_VR_END_REQ,
    EVT_MP_XM_SIGNAL_REQ,
    EVT_MP_XM_SIGNAL_RES,
    EVT_MP_IPOD_TRACK_NAME_REQ,
    EVT_MP_IPOD_TRACK_NAME_RES,
    EVT_MP_TRACK_CHANGE_REQ,
    EVT_MP_BOOTING_STATE_REQ,
    EVT_MP_BOOTING_STATE_RES,
    EVT_MP_BT_RSSI_REQ,
    EVT_MP_BT_RSSI_RES,
    EVT_MP_XM_RADIO_ID_REQ,
    EVT_MP_XM_RADIO_ID_RES,
    EVT_MP_DMB_CH_FIX_REQ,
    EVT_MP_DMB_CH_FIX_RES,
    EVT_MP_IBOX_DISP_CHECK_REQ,
    EVT_MP_IBOX_BELL_SOUND_CHECK_REQ,
    EVT_MP_IBOX_PHONE_SOUND_CHECK_REQ,
    EVT_MP_IBOX_SOUND_CHECK_END_REQ,
    EVT_MP_DMB_TPEG_VALUE_REQ,
    EVT_MP_DMB_TPEG_VALUE_RES,

    EVT_APP_ALIVE,
    EVT_TOUCH_ENABLE,
    EVT_AGREE_CLICK_STATUS,
    EVT_MP_WRITE_TRACE_DATE_REQ,
    EVT_MP_READ_TRACE_DATE_REQ,


/**<  Add new events above this line */
	EVT_NOTIFY_TRANSIT_APP,
	EVT_MP_ECALL_STATUS_REQ,	// 9115
	EVT_MP_ECALL_STATUS_RES,	// 9116

	EVT_MP_AA_CONNECT_REQ,
	EVT_MP_AA_CONNECT_RES,
	EVT_MP_AA_SKIP_TOUCH_REQ,
	EVT_MP_AA_SKIP_TOUCH_RES,
	EVT_MP_AA_MEDIA_PLAY_REQ,
	EVT_MP_AA_MEDIA_PLAY_RES,
	EVT_MP_CP_CONNECT_REQ,
	EVT_MP_CP_CONNECT_RES,
	EVT_MP_CP_SKIP_TOUCH_REQ,
	EVT_MP_CP_SKIP_TOUCH_RES,
	EVT_MP_CP_MEDIA_PLAY_REQ,
	EVT_MP_CP_MEDIA_PLAY_RES,

    EVT_MAX

} EVT_PUBLIC_ID_T;

enum KEY_STATUS_T
{
    KEY_STATUS_PRESSED,
    KEY_STATUS_RELEASED,
    KEY_STATUS_PRESSED_LONG,
    KEY_STATUS_PRESSED_CRITICAL,
    KEY_STATUS_CANCEL
};

enum KEY_JOG_STATUS_T {
	JOG_NONE_PRESSED = 0,
	CCP_JOG_CENTER_PRESSED = 1,
	CCP_JOG_LEFT_PRESSED = 2,
	CCP_JOG_RIGHT_PRESSED = 4,
	CCP_JOG_UP = 8,
	CCP_JOG_DOWN = 16,
	RRC_JOG_CENTER_PRESSED = 1024,
	RRC_JOG_LEFT_PRESSED = 2048,
	RRC_JOG_RIGHT_PRESSED =4096,
	RRC_JOG_UP = 4096,
	RRC_JOG_DOWN = 8192,
	RRC_SEEK_PREV = 16384,
	RRC_SEEK_NEXT = 32768,
	HU_SEEK_PREV = 65536,
	HU_SEEK_NEXT = 131072
};

/**
* Event message data base
*/
struct APP_EVENT_DATA_BASE_T
{
    APP_ID_T appIdReciever;
    EVT_ID_T reasonEvent;   /**< Is APP_ID_T in case of POST event */
    int dataSize;
    /*! -- DHAVN_TELECA_ininyodu_CR9956--20120526 Jog enabler information from soft key -- */
    //! true if event is result of JOG event else false.
    bool sIsJogEnabled;
    bool bTemporalMode;
    bool bReserved02;
    bool bReserved03;
    bool bReserved04;
    bool bReserved05;
    bool bReserved06;
    bool bReserved07;
};

/**
* Message data for key events.
*/
struct KEY_EVENT_DATA_T
{
    APP_EVENT_DATA_BASE_T header;

    KEY_STATUS_T keyStatus;
};

/**
* List of possible country variants.
*/
enum ECountryVariant
{
    eCVInvalid      = -1,
    eCVKorea        =  0,
    eCVNorthAmerica =  1,
    eCVChina        =  2,
    eCVGeneral      =  3,
    eCVMiddleEast   =  4,
    eCVEuropa       =  5,
    eCVCanada       =  6,
    eCVRussia       =  7,
    eCVAustralia    = 0xA0,     // 호주향 모드렉스 대응 Country Variant 추가 
    eCVInvalidMax   = 0xA1,  
    eCVTotalCount   =  9,       // eCVKorea 부터 eCVInvalidMax 전까지의 개수 
};

/**
 * List of possible Vehicle Variants
 */

enum EVehicleVariants
{
    eVVDH = 0,
    eVVKH = 1,
    eVVVI = 2,
};

/**
 * Country Variant enum 값과 국가부호 문자열을 Mapping
 */
typedef struct __stVARIANT_STRING
{
    ECountryVariant eCV;    // Country Variant from MICOM 
    QString strCV;          // Country Variant Unique String  
    QString strSOPCV;       // Country Variant Unique String for SOP 
} VARIANT_STRING, *pVARIANT_STRING;

const VARIANT_STRING stVariantString[] = 
{
    { eCVKorea,         "KR",   "KOR" },
    { eCVNorthAmerica,  "US",   "USA" },
    { eCVChina,         "CH",   "CHN" },
    { eCVGeneral,       "GE",   "GEN" },
    { eCVMiddleEast,    "ME",   "MES" },
    { eCVEuropa,        "EU",   "EUR" },
    { eCVCanada,        "CA",   "CAN" },
    { eCVRussia,        "RU",   "RUS" },
    { eCVAustralia,     "AU",   "AUS" },
    { eCVInvalidMax,    "MAX",  "MAX" },
};

/**
 * List of possible DVD Regions
 */
enum EDVDRegion
{
    eDVDREGNorthAmerica = 1,
    eDVDREGGeneral      = 2, /**< MiddleEast, Europe too */
    eDVDREGKorea        = 3,
    eDVDREGChina        = 6,
};

/**
 *  List of possible DVD Menu Language
 */
enum EDVDMenuLanguage
{
    eDVDMENULco    = 0x6B6F,
    eDVDMENULen    = 0x656E,
    eDVDMENULzh    = 0X7a68,
};

/**
 *  List of possible System Variants
 */

enum ESystemVariants
{
    eSYSVARmin      = 0,
    eSYSVAKdef      = 0x048f,
    eSYSVARmax      = 0xffffffff,
};

/**
 * the different types of country variant information
 */

struct S_APP_EVENT_DATA_SYSTEM_OPTIONS
{
    EVehicleVariants sVehicleVariant;
    ECountryVariant  sCountryVariant;
    ESystemVariants  sSystemVariant;
    EDVDRegion       sDvdRegion;
    EDVDMenuLanguage sDvdLanguage;
    bool             m_SysOptionFail;
};

/**
* Message data for key events.
*/
struct APP_EVENT_SYSTEM_OPTIONS_T
{
    APP_EVENT_DATA_BASE_T           header;

    S_APP_EVENT_DATA_SYSTEM_OPTIONS opt;
};
/**
* List of app screen ids.
*/
enum DISPLAY_T
{
    DISPLAY_NONE,               /**< No screen assigned to app */
    DISPLAY_FRONT,              /**< App running on front screen */
    DISPLAY_REAR,               /**< App running on rear screen */
    DISPLAY_FRONT_REAR,         /**< Apps running on both screens */
    DISPLAY_AUTO,                /**< For non VEXT suport */
	DISPLAY_COMMON
};


/*!
 * Error codes.
 */
enum ErrorCodes
{
    eErrorNone = 0,                 /*!< No Error */
    eErrorInvalid = -1,             /*!< Invalid request */
    eErrorNotFound = -2,            /*!< Unable to find object */
    eErrorGeneral = -3,             /*!< General error */
    eErrorCancel = -4,              /*!< Operation cancelled */
    eErrorNoMemory = -5,            /*!< Memory not available */
    eErrorBadArgument = -6,         /*!< Bad argument */
    eErrorNotReady = -7,            /*!< Not ready */
    eErrorTimeOut = -8,             /*!< Time out */
    eErrorPathNotFound = -9,        /*!< Unable to find specified path*/
    eErrorInUse = -10,              /*!< Resource/object in use */
    eErrorUnknown = -11,            /*!< Unknown error */
    eErrorCouldNotFG = -12          /*!< Could not come foreground */
};


////////////////////////////////////
//
//   FROM uistate_manager_def.h   //
//
////////////////////////////////////

/**
* List of display planes
*/
enum DISPLAY_PLANES_T
{
    OVERLAY_PLANE = 0,			/*!< Represents  Overlay Plane */
    SPRITEC_PLANE = 1,			/*!< Represents  SpriteC Plane */
    FRAME_BUFFER_PLANE = 2,		/*!< Represents  Frame buffer Plane */
    OVERLAY_SPRITEC = 3			/*!< Represents  Overlay SpriteC Plane */
};

/*!
* List of App entry points
*/
enum APP_ENTRY_POINT_T
{
    eAPP_NONE,                  /*!< Represents  INVALID Application */
    eAPP_MODE_DEFAULT,
    eAPP_MODE_LAST_STATE,
    eAPP_STANDBY_FRONT,
    eAPP_STANDBY_REAR,
    eAPP_CLOCK_ENABLE_SETTINGS,
    eAPP_CLOCK_DISABLE_SETTINGS,
    eAPP_CLOCK_PREVIOUS_MODE,
    eAPP_CLOCK_BLACK_MODE,
    eAPP_CLOCK_LOGIC_OFF_BLACK_MODE,

    //Home screen entry points
    eAPP_HOME_MAIN = 10,
    eAPP_HOME_MEDIA,
    eAPP_HOME_MEDIA_AUDIO,
    eAPP_HOME_MEDIA_VIDEO,
    eAPP_HOME_MEDIA_PHOTO,
    eAPP_HOME_SETTINGS,
    eAPP_HOME_SETTINGS_GENERAL,
    eAPP_HOME_BLUELINK,
    eAPP_HOME_BLUELINK_AUTO_DIAGNOSTICS,
    eAPP_HOME_BLUELINK_ENTERTAINMENT,
    eAPP_HOME_BLUELINK_NAVIGATION,
    eAPP_HOME_BLUELINK_POI,
    eAPP_HOME_BLUELINK_VEHICLE_INFO,
    eAPP_HOME_BLUETOOTH,
    eAPP_HOME_RADIO,
    eAPP_HOME_DAB,
    eAPP_HOME_DMB,
    eAPP_HOME_APPS,
    eAPP_HOME_INFO,
    eAPP_HOME_ENGINEERING_SIMPLE,
    eAPP_HOME_ENGINEERING_FULL,
    eAPP_HOME_ENGINEERING_DYNAMICS,
    eAPP_HOME_ENGINEERING_DIAGNOSTIC,
    eAPP_HOME_BLUELINK_PHONE,           /*!< Represents  entry point for Bluelink phone menu */
    eAPP_HOME_AVOFF,
    
    eAPP_STANDBY_LOADING,               // 동의함에 '로딩중' 화면추가..

    //AVP entry points
    eAPP_AVP_AUDIO = 60,                 /*!< Represents  AUDIO Application */
    eAPP_AVP_VIDEO,                 /*!< Represents  VIDEO Application */
    eAPP_AVP_PHOTO,                 /*!< Represents  PHOTO Application */

    eAPP_AVP_AUDIO_MODE_AUX,        /*!< Represents entry point for audio aux mode */
    eAPP_AVP_MODE_BLUETOOTH,        /*!< Represents entry point for bluetooth mode */
    eAPP_AVP_AUDIO_MODE_DISC,       /*!< Represents entry point for disc audio mode */
    eAPP_AVP_VIDEO_MODE_DISC,       /*!< Represents entry point for disc video mde */
    eAPP_AVP_AUDIO_MODE_IPOD1,      /*!< Represents entry point for  iPod1 */
    eAPP_AVP_AUDIO_MODE_IPOD2,      /*!< Represents entry point for  iPod2 */
    eAPP_AVP_PHOTO_MODE_JUKEBOX,    /*!< Represents entry point for  jukebox photo mode */
    eAPP_AVP_AUDIO_MODE_JUKEBOX,    /*!< Represents entry point for  jukebox audio mode */
    eAPP_AVP_VIDEO_MODE_JUKEBOX,    /*!< Represents entry point for  jukebox video mode */
    eAPP_AVP_AUDIO_MODE_USB1,       /*!< Represents entry point for  USB1 audio mode */
    eAPP_AVP_AUDIO_MODE_USB2,       /*!< Represents entry point for  USB2 audio mdoe */
    eAPP_AVP_VIDEO_MODE_USB1,       /*!< Represents entry point for  USB1 video mode */
    eAPP_AVP_VIDEO_MODE_USB2,       /*!< Represents entry point for  USB2 video mode */
    eAPP_AVP_PHOTO_MODE_USB1,       /*!< Represents entry point for  USB1 photo mode */
    eAPP_AVP_PHOTO_MODE_USB2,       /*!< Represents entry point for  USB2 photo mode */
    eAPP_AVP_VIDEO_MODE_AUX,        /*!< Represents entry point for video aux mode */
    eAPP_AVP_VR_MEDIA_SEARCH,       /*!< Represents entry point for Media Search Screen */
    eAPP_AVP_AUDIO_MODE_BT,         /*!< Represents entry point for audio bt mode */
    eAPP_AVP_VIDEO_MODE_DISC_DRS,
    eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS,
    eAPP_AVP_VIDEO_MODE_USB1_DRS,
    eAPP_AVP_VIDEO_MODE_USB2_DRS,
    eAPP_AVP_VIDEO_MODE_AUX_DRS,
    eAPP_AVP_PHOTO_MODE_JUKEBOX_DRS,
    eAPP_AVP_PHOTO_MODE_USB1_DRS,
    eAPP_APP_PHOTO_MODE_USB2_DRS,
    eAPP_MODE_DYNAMICS_DISC,
    eAPP_AVP_VIDEO_CAPTION_SETTING,
    eAPP_AVP_VIDEO_DIVX_SETTING,
    eAPP_AVP_DVD_SETTING,
    eAPP_AVP_VCD_PBC_MENU_SETTING,
    eAPP_AVP_VCD_SHORTCUT_SETTING,

    //Radio modes
    eAPP_RADIO = 100,           /*!< Represents  Radio Application */
    eAPP_RADIO_MODE_FM1,        /*!< Represents entry point for Radio FM1mode */
    eAPP_RADIO_MODE_FM2,        /*!< Represents entry point for Radio FM2 mode */
    eAPP_RADIO_MODE_AM,         /*!< Represents entry point for Radio AM mode */
    eAPP_MODE_RADIO_MAIN,       /*!< Represents Dynamics entry point for Radio main */
    eAPP_MODE_DYNAMICS_RADIO,   /*!< Represents Dynamics entry point for Radio */
    eAPP_MODE_DYNAMICS_RDS,     /*!< Represents Dynamics entry point for RDS */
    eAPP_MODE_DYNAMICS_HDRADIO, /*!< Represents Dynamics entry point for HD Radio */
    /*!-- DHAVN_TELECA_ininsiko_CR15128_14807--29thOct2012 HD Radio audio Mode Splitting Support  --*/
    eAPP_RADIO_MODE_HDR_FM1,    /*!< Represents entry point for HDFM1 Radio */
    eAPP_RADIO_MODE_HDR_FM2,    /*!< Represents entry point for HDFM2 Radio */
    eAPP_RADIO_MODE_HDR_AM,     /*!< Represents entry point for HDAM Radio */
    eAPP_MODE_DYNAMICS_RDS_DEALER_MODE,

    eAPP_DAB = 120,             /*!< Represents  DAB Application */
    eAPP_DAB_MODE_DAB1,         /*!< Represents entry point for DAB-DAB1 mode */
    eAPP_DAB_MODE_DAB2,         /*!< Represents entry point for DAB-DAB2 mode */
    eAPP_MODE_DYNAMICS_DAB,     /*!< Represents Dynamics entry point for DAB */
    eAPP_DAB_MODE_DAB3,         /*!< Represents entry point for DAB-DAB3 mode */
    eAPP_MODE_DYNAMICS_DAB_DEALER_MODE,

    eAPP_XM = 130,              /*!< Represents  XM Application */
    eAPP_XM_MODE_XM1,           /*!< Represents entry point for XM-XM1 */
    eAPP_XM_MODE_XM2,           /*!< Represents entry point for XM-XM2 mode */
    eAPP_XM_MODE_XM3,           /*!< Represents entry point for XM-XM3 mode */
    eAPP_MODE_DYNAMICS_XM,      /*!< Represents Dynamics entry point for XM */
    eAPP_XM_DATA,
    eAPP_XM_ACTIVE_FAVORITE,
    eAPP_XM_ACTIVE_SPORT,
    eAPP_MODE_DYNAMICS_XM_DEALER_MODE,

    eAPP_DMB = 140,             /*!< Represents  DMB Application */
    eAPP_DMB_MODE_DMB1,         /*!< Represents entry point for DMB-DMB1 mode */
    eAPP_DMB_MODE_DMB2,         /*!< Represents entry point for DMB-DMB2 mode */
    eAPP_MODE_DYNAMICS_DMB,     /*!< Represents Dynamics entry point for DMB */
    eAPP_DMB_MODE_DMB1_DRS,
    eAPP_DMB_MODE_DMB2_DRS,
    eAPP_MODE_DYNAMICS_DMB_DEALER_MODE,

    eAPP_PANDORA = 150,
    eAPP_PANDORA_MODE_BT,       /*!< Represents entry point for Pandora bt mode */
    eAPP_PANDORA_MODE_IPOD1,    /*!< Represents entry point for Pandora ipod1 mode */
    eAPP_PANDORA_MODE_IPOD2,    /*!< Represents entry point for Pandora ipod2 mode */

    eAPP_IBOX_BLUELINK_PHONE_MAIN = 156, /*!< Represents entry point for Bluelink phone main mode */
    eAPP_IBOX_BLUELINK_DIAL,            /*!< Represents entry point for Bluelink phone dial mode */
    eAPP_IBOX_BLUELINK_PHONEBOOK,       /*!< Represents entry point for Bluelink phone phonebook mode */
    eAPP_IBOX_BLUELINK_CALL_HISTORY,    /*!< Represents entry point for Bluelink phone call history mode */
    eAPP_IBOX_BLUELINK_FAVORITE,        /*!< Represents entry point for Bluelink phone favorite mode */

    eAPP_CAMERA_DIRECT = 170,   /*!< Represents entry point for Direct Camera */
    eAPP_CAMERA_PGS,            /*!< Represents entry point for PGS Camera */
    eAPP_CAMERA_AVM,            /*!< Represents entry point for AVM Camera */

    /*!-- [ DHAVN_TELECA_krinhesh_CR11626_CR12493--20120821 Launch BT phone app --*/
    eAPP_BTPHONE_MAIN = 180,    /*!< Represents entry point for BT phone main mode */
    eAPP_BTPHONE_PAIRING,    /*!< Represents entry point for BT phone pairing mode */
    eAPP_BTPHONE_CONNECT,    /*!< Represents entry point for BT phone connect mode */
    eAPP_BTPHONE_OUTGOING_CALL,    /*!< Represents entry point for BT phone outgoing call mode */
    eAPP_BTPHONE_CONTACTS_DOWNLOAD,    /*!< Represents entry point for BT phone contacts download mode */
    eAPP_BTPHONE_NAME_SEARCH,    /*!< Represents entry point for BT phone name search mode */
    eAPP_BTPHONE_NUMBER_SEARCH,    /*!< Represents entry point for BT phone number search mode */
    eAPP_BTPHONE_DIAL,    /*!< Represents entry point for BT phone dial mode */
    eAPP_BTPHONE_SIRI,    /*!< Represents entry point for BT phone dial mode */
    eAPP_BTPHONE_CALL_HISTORY_DOWNLOAD,

    eAPP_SETTINGS_BT_MAIN = 200,    /*!< Represents entry point for settings BT main mode */
    eAPP_SETTINGS_BT_FIRST,    /*!< Represents entry point for settings BT first mode */
    eAPP_SETTINGS_BT_SECOND,    /*!< Represents entry point for settings BT second mode */
    eAPP_SETTINGS_BT_DEVICE_DEL,
    /*!-- ] DHAVN_TELECA_krinhesh_CR11626_CR12493--20120821 Launch BT phone app --*/

    eAPP_AHARADIO = 210,
    eAPP_AHARADIO_MODE_BT,
    eAPP_AHARADIO_MODE_IPOD1,
    eAPP_AHARADIO_MODE_IPOD2,
    //! Max Mode for AVP

    eAPP_SETTINGS_MAIN = 220,	
    eAPP_SETTINGS_SOUND,				// eStart_Mode_Sound
    eAPP_SETTINGS_SCREEN,				// eStart_Mode_Screen
    eAPP_SETTINGS_SCREEN_VIDEO,			// eStart_Mode_Screen
    eAPP_SETTINGS_VR,					// eStart_Mode_VR
    eAPP_SETTINGS_SYSTEM,				// eStart_Mode_System	
    eAPP_SETTINGS_GENERAL,				// eStart_Mode_General
    eAPP_SETTINGS_GENERAL_PHOTO,		// eStart_Mode_General_Photo_Frame
    eAPP_SETTINGS_GENERAL_CLOCK,		// eStart_Mode_General_Clock
    eAPP_SETTINGS_GENERAL_LANGUAGE,		// eStart_Mode_General_Language
    eAPP_SETTINGS_GENERAL_KEYBOARD,		// eStart_Mode_General_Keyboard
    eAPP_SETTINGS_GENERAL_VIDEO_DVD,	// eStart_Mode_General_Video_Control
    eAPP_SETTINGS_GENERAL_VIDEO_DIVX,	// eStart_Mode_General_Video_Control
    eAPP_SETTINGS_DYNAMICS_SOUND,
    eAPP_SETTINGS_SCREEN_DISP,
    eAPP_SETTINGS_SCREEN_AVP_DIVX,
    eAPP_SETTINGS_SCREEN_AVP_DVD,
    eAPP_SETTINGS_SCREEN_AVP_AUX,
    eAPP_SETTINGS_SCREEN_MOBILETV,
    eAPP_SETTINGS_SCREEN_QUICK_GUIDE,

    eAPP_VIDEOGUIDE_LIST = 240,
    eAPP_VIDEOGUIDE_BTHELP_CONNECTION,
    eAPP_VIDEOGUIDE_BTHELP_REGISTRATION,
    eAPP_VIDEOGUIDE_GUIDE_PLAY,
    eAPP_VIDEOGUIDE_GUIDE_PLAY_DRS,
    eAPP_VIDEOGUIDE_VRHELP_USAGE,
    eAPP_VIDEOGUIDE_BTHELP_CONNECTION_DRS,
    eAPP_VIDEOGUIDE_BTHELP_REGISTRATION_DRS,
    eAPP_VIDEOGUIDE_VRHELP_USAGE_DRS,

    eAPP_SETTINGS_SOUND_FROM_HOME = 250,	// it will be modified the enum value someday..
	eAPP_SETTINGS_CONNECTIVITY, 
	eAPP_SETTINGS_SCREEN_AVP_DIVX_DRS,
	eAPP_SETTINGS_SCREEN_AVP_DVD_DRS,
	eAPP_SETTINGS_SCREEN_AVP_AUX_DRS,
	eAPP_SETTINGS_SCREEN_MOBILETV_DRS,
	eAPP_FILEMANAGER_JUKEBOX_PHOTO_DRS,
	eAPP_FILEMANAGER_USB1_PHOTO_DRS,
	eAPP_FILEMANAGER_USB2_PHOTO_DRS,

    eAPP_HOME_ROOT_NAVI = 260,
    eAPP_HOME_ROOT_RADIO,
    eAPP_HOME_ROOT_XM,
    eAPP_HOME_ROOT_MEDIA,
    eAPP_HOME_ROOT_DMB,
    eAPP_HOME_ROOT_SETTINGS,
    eAPP_HOME_ROOT_PHONE,
    eAPP_HOME_ROOT_XM_DATA,
    eAPP_HOME_ROOT_BLUELINK,
    eAPP_HOME_ROOT_DOWNLOAD,
    eAPP_HOME_ROOT_CONNECTIVITY,

    eAPP_HOME_MEDIA_JB_AUDIO = 280,
    eAPP_HOME_MEDIA_JB_VIDEO,
    eAPP_HOME_MEDIA_JB_PHOTO,
    eAPP_HOME_MEDIA_USB_AUDIO1,
    eAPP_HOME_MEDIA_USB_VIDEO1,
    eAPP_HOME_MEDIA_USB_PHOTO1,
    eAPP_HOME_MEDIA_USB_AUDIO2,
    eAPP_HOME_MEDIA_USB_VIDEO2,
    eAPP_HOME_MEDIA_USB_PHOTO2,
    eAPP_HOME_MEDIA_IPOD_AUDIO1,
    eAPP_HOME_MEDIA_IPOD_AUDIO2,
    eAPP_HOME_MEDIA_DISC,
    eAPP_HOME_MEDIA_BT_AUDIO,
    eAPP_HOME_MEDIA_AUX,
    eAPP_HOME_MEDIA_PANDORA1,
    eAPP_HOME_MEDIA_PANDORA2,
    eAPP_HOME_MEDIA_AHA1,
    eAPP_HOME_MEDIA_AHA2,
	eAPP_HOME_MEDIA_PANDORA_BT,
	eAPP_HOME_MEDIA_AHA_BT,
	

    eAPP_HOME_SETTING_SOUND = 300,
    eAPP_HOME_SETTING_DISPLAY,
    eAPP_HOME_SETTING_NAVIGATION,
    eAPP_HOME_SETTING_BT,
    eAPP_HOME_SETTING_VR,
    eAPP_HOME_SETTING_CLOCK,
    eAPP_HOME_SETTING_GENERAL,
    eAPP_HOME_SETTING_SYSTEM,
    eAPP_HOME_SETTING_HELP,
    eAPP_HOME_SETTING_WIFI,

    eAPP_HOME_BL_PHONE = 320,
    eAPP_HOME_BL_DRIVING,
    eAPP_HOME_BL_POI,
    eAPP_HOME_BL_SETTINGS,
    eAPP_HOME_BL_DIAG,
    eAPP_HOME_BL_CENTER,
    eAPP_HOME_BL_APPS,

    eAPP_HOME_SETTING_HELP_GUIDE = 340,
    eAPP_HOME_SETTING_HELP_MANUAL,

    eAPP_EMANUAL_MAIN=350,
    eAPP_EMANUAL_BTHELP_CONNECTION,
    eAPP_EMANUAL_VRHELP_USAGE,
    
    eAPP_EMANUAL_DRS=355,
    eAPP_EMANUAL_BTHELP_CONNECTION_DRS,
    eAPP_EMANUAL_VRHELP_USAGE_DRS,

    eAPP_FILEMANAGER_JUKEBOX_VIDEO = 360,
    eAPP_FILEMANAGER_JUKEBOX_PHOTO,
    eAPP_FILEMANAGER_USB1_VIDEO,
    eAPP_FILEMANAGER_USB1_PHOTO,
    eAPP_FILEMANAGER_USB2_VIDEO,
    eAPP_FILEMANAGER_USB2_PHOTO,
    eAPP_FILEMANAGER_SETTINGS,
    eAPP_FILEMANAGER_JUKEBOX_AUDIO,
    eAPP_FILEMANAGER_USB1_AUDIO,
    eAPP_FILEMANAGER_USB2_AUDIO,
    eAPP_FILEMANAGER_IPOD1,
    eAPP_FILEMANAGER_IPOD2,
    eAPP_FILEMANAGER_DISC_AUDIO,

    eAPP_STANDBY_REAR_NOTIFY,

	eAPP_HOME_ROOT_CARPLAY = 380,
	eAPP_HOME_MEDIA_CARPLAY_AV,

	eAPP_HOME_ROOT_ANDROID_AUTO,
	eAPP_HOME_MEDIA_ANDROID_AV,
	eAPP_POPUP_NOTY,

	eAPP_CARPLAY_AV = 400,
	eAPP_CARPLAY_NON_AV,
	eAPP_CARPLAY_CALL,
	eAPP_CARPLAY_CALL_WITH_AV,
	eAPP_CARPLAY_CALL_WITH_NON_AV,
	eAPP_CARPLAY_SIRI,
	eAPP_CARPLAY_SIRI_WITH_AV,
	eAPP_CARPLAY_SIRI_WITH_NON_AV,
	eAPP_CARPLAY_NAVI,
	eAPP_CARPLAY_NAVI_WITH_AV,
	eAPP_CARPLAY_NAVI_WITH_NON_AV,
	eAPP_CARPLAY_REQUEST_GUI,

	eAPP_ANDROID_AUTO_AV = 420,
	eAPP_ANDROID_AUTO_NON_AV,
	eAPP_ANDROID_AUTO_CALL,
	eAPP_ANDROID_AUTO_CALL_WITH_AV,
	eAPP_ANDROID_AUTO_CALL_WITH_NON_AV,
	eAPP_ANDROID_AUTO_VR,
	eAPP_ANDROID_AUTO_VR_WITH_AV,
	eAPP_ANDROID_AUTO_VR_WITH_NON_AV,
	eAPP_ANDROID_AUTO_NAVI,
	eAPP_ANDROID_AUTO_NAVI_WITH_AV,	// 400
	eAPP_ANDROID_AUTO_NAVI_WITH_NON_AV,
	eAPP_ANDROID_AUTO_REQUEST_GUI,

    eAPP_ECALL_SOS = 440,
    eAPP_ECALL_ETEST,

    eAPP_SETTINGS_SCREEN_PHOTO,
    eAPP_MODE_MAX                /*!< Anyother entry point to be added before this */

};

enum MODE_STATE_T
{
    MODE_STATE_NONE,  //0
    MODE_STATE_AVOFF,
    MODE_STATE_FM1,
    MODE_STATE_FM2,
    MODE_STATE_AM,
    MODE_STATE_DISC_AUDIO,
    MODE_STATE_DISC_VIDEO,
    MODE_STATE_USB1_AUDIO,
    MODE_STATE_USB1_VIDEO,
    MODE_STATE_USB2_AUDIO,
    MODE_STATE_USB2_VIDEO, // 10
    MODE_STATE_IPOD1_AUDIO,
    MODE_STATE_IPOD1_PANDORA,
    MODE_STATE_AHA_RADIO_IPOD1,
    MODE_STATE_IPOD2_AUDIO,
    MODE_STATE_IPOD2_PANDORA,
    MODE_STATE_AHA_RADIO_IPOD2,
    MODE_STATE_VCDC,
    MODE_STATE_JUKEBOX_AUDIO,
    MODE_STATE_JUKEBOX_VIDEO,
    MODE_STATE_BT_AUDIO,  //20
    MODE_STATE_BT_PANDORA,
    MODE_STATE_AHA_RADIO_BT,
    MODE_STATE_AUX1_AUDIO,
    MODE_STATE_AUX1_VIDEO,
    MODE_STATE_AUX2_AUDIO,
    MODE_STATE_AUX2_VIDEO,
    MODE_STATE_DMB1,
    MODE_STATE_DMB2,
    MODE_STATE_CMMB1,
    MODE_STATE_CMMB2, //30
    MODE_STATE_XM1,
    MODE_STATE_XM2,
    MODE_STATE_DAB1,
    MODE_STATE_DAB2,
    MODE_STATE_DAB3,
    MODE_STATE_HD_RADIO,
    MODE_STATE_IBOX_DLNA,
    MODE_STATE_TTS,
    MODE_STATE_RDS_TA,
    MODE_STATE_RDS_NEWS,  //40
    MODE_STATE_RDS_ALARM,
    MODE_STATE_DAB_FM,
    MODE_STATE_DAB_AM,
    MODE_STATE_IBOX_WEBPLAYER,
    MODE_STATE_IBOX_VR,
    MODE_STATE_HDR_FM1,
    MODE_STATE_HDR_FM2,
    MODE_STATE_HDR_AM,
    MODE_STATE_MIR_LINK_USB1,
    MODE_STATE_MIR_LINK_USB2, //50
    MODE_STATE_MIR_LINK_BT,
    MODE_STATE_VIDEO_GUIDE,
    MODE_STATE_XM3,
    MODE_STATE_DAB_TA,
    MODE_STATE_VIDEO_GUIDE_LIST,
    MODE_STATE_POWEROFF,
    MODE_STATE_DAB_TA_ALRAM,
	MODE_STATE_CARPLAY_AUDIO,
	MODE_STATE_ANDROID_AUTO_AUDIO,
	MODE_STATE_CARPLAY_ALERT,
    MODE_STATE_MAX
};

struct EVENT_AUDIO_PATH_REQUEST_DATA
{
    APP_EVENT_DATA_BASE_T header;
    MODE_STATE_T avState;
    quint16 sPlayPICode;  // Using DAB Only
};


/**
* List of clone status
*/
enum CLONE_STATUS_T
{
	CLONE_OFF = 0,		/*!< Represents  Clone mode OFF state */
	CLONE_ON_FRONT,		/*!< Represents  Clone mode ON state */
	CLONE_ON_REAR,		/*!< Represents  Clone mode ON state */
};

/**
* Message data for EVT_CLONE_NOTIFICATION event.
*/
struct EVENT_CLONE_NOTIFICATION_DATA_T
{
    /*!
     * base header struct
     */
    APP_EVENT_DATA_BASE_T sHeader;
    /*!
     * Represents clone mode status
     */
    CLONE_STATUS_T sCloneStatus;
};


/**
 *Please refer to the below table.
 *  Region Default Language LGE comment
 *
 *  Korea           Korean
 *  North America   English
 *  Middle east     Arabic
 *  China           Mandarin
 *  Europe          English UK English
 *  General         English
 *
 */
struct DEFAULT_REGION_LANGUAGE_T
{
    ECountryVariant region;
    LANGUAGE_T      language;
};

/**
* Message data for EVT_LANGUAGE_CHANGED event.
*/
struct EVENT_LANGUAGE_CHANGED_DATA_T
{
    APP_EVENT_DATA_BASE_T header;

    LANGUAGE_T languageId;
};

/*!
 *This structure will be used to inform
 * application for the entry point
 **/
struct APP_ENTRY_POINT_DATA_T
{
    //! Header
    APP_EVENT_DATA_BASE_T sHeader;

    //! Represents  the entry point for Application
    APP_ENTRY_POINT_T sAppEntryPoint;
};

enum REAR_LCD_STATUS_T
{
    REAR_LCD_RIGHT_OFF  = 0x01,
    REAR_LCD_LEFT_OFF   = 0x02, 
    REAR_LCD_ALL_OFF    = 0x03, // REAR_LCD_RIGHT_OFF | REAR_LCD_LEFT_OFF 
    REAR_LCD_RIGHT_ON   = 0x04,
    REAR_LCD_LEFT_ON    = 0x08,
    REAR_LCD_ALL_ON     = 0x0C, // REAR_LCD_RIGHT_ON | REAR_LCD_LEFT_ON
};


struct EVENT_SETTINGS_GPS_TIMEZONE_CHANGED_DATA_T
{
    int sValue;
};

#endif
