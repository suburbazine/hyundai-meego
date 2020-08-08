/**
 * DHAVN_AppFramework_Popup.h
 *
 */

#ifndef DHAVN_APPFRAMEWORK_POPUP_H
#define DHAVN_APPFRAMEWORK_POPUP_H
const int POPUP_TITLE_LENGHT = 64;            /** Title of popup size  */
const int POPUP_CONTENT_LENGHT = 512;         /** Message size */
const int POPUP_INVALID_INDEX = 0xFFFFFFFF;   /** Invalid index */
const int POPUP_BUTTON_MAX = 4;               /** Max numer of buttons */
const int POPUP_BUTTON_COUNT = POPUP_BUTTON_MAX;
const int POPUP_TEXT_LINE_MAX = 4;            /** Max numer of message lines */

enum PUPUP_TYPE_T
{
    POPUP_NONE = 0,        /** Hide current system popup */
    POPUP_MESSAGE_BOX,     /** Timeout simple text popup in transparency box */
    POPUP_TEXT,            /** Popup window with a title, text and buttons */
    POPUP_CUSTOM_TEXT,     /** Popup window with a title, text and buttons.
                //               Text style can be set to different value for all lines */
    POPUP_PROGRESS,        /** progress bar with button in pop-up window */

    // POPUP_WARNING will be deleted
    POPUP_WARNING,         /** Popup window with a icon, title, text and buttons */
    POPUP_LIST,             /** Popup window with a icon, title, list and buttons */
    POPUP_TOAST,
    POPUP_XM_ALERT,
	POPUP_NAVI
};

enum POPUP_STATUSBAR_T
{
    POPUP_STATUSBAR_EXIST = 1,
    POPUP_STATUSBAR_NOT_EXIST
};


/** Button ID or timeout */
enum POPUP_BUTTON_T
{
   POPUP_BUTTON_NONE = 0,
   POPUP_BUTTON_TIMEOUT,
   POPUP_BUTTON_0,
   POPUP_BUTTON_1,
   POPUP_BUTTON_2,
   POPUP_BUTTON_3
};

enum POPUP_BUTTON_TYPE_T
{
   POPUP_BUTTON_TYPE_OK,
   POPUP_BUTTON_TYPE_CANCEL,
   POPUP_BUTTON_TYPE_ICON,
   POPUP_BUTTON_TYPE_TEXT
};

enum POPUP_MESSAGE_BOX_SIZE_T
{
   POPUP_MESSAGE_BOX_SIZE_1 = 0,
   POPUP_MESSAGE_BOX_SIZE_2,
   POPUP_MESSAGE_BOX_SIZE_3
};
enum POPUP_USE_ANIMATION_T
{
    POPUP_ANIMATION = 0,
    POPUP_NO_ANIMATION = 1
};

enum FG_POPUP_T
{
    _CURRENT_POPUP_NONE =0,            /*!< no current popup */
    _COMMON_POPUP_NOT_HANDLE,          /*!< Show Common Popup for Not handling response */
    _TA_POPUP_FROM_RADIO,              /*!< pop up from TA request from radio app */
    _TA_POPUP_FROM_DAB,                /*!< pop up from TA request from DAB app */
    _USB_REMOVED_POPUP,                /*!< Show USB remove Popup */
    _IBOX_POPUP_REQUEST,               /*!< Show IBOX Popup */
    _PANDORA_REMOVED_POPUP,            /*!< Show PANDORA remove Popup */
    _BT_PANDORA_REMOVED_POPUP,         /*!< Show BT Pandora remove Popup */
    _IPOD_REMOVED_POPUP,               /*!< Ipod remove Popup */
    _IPOD_POPUP_NOT_SUPPORTED,         /*!< Show Popup IPod Not Supported */
    _IBOX_CLONE_QUERY_POPUP,           /*!< iBox clone query popup */
    _DMB_DISASTER_INFO_POPUP,     //11      /*!< iBox clone query popup */
    _AHARADIO_REMOVED_POPUP,            /*!< Show AHARADIO remove Popup */
    _BT_AHARADIO_REMOVED_POPUP,         /*!< Show BT Pandora remove Popup */
    _SDCARD_MOUNT_POPUP,
    _SDCARD_UNMOUNT_POPUP,
    _MOTO_ERORR_POPUP,
    _EPG_RADIO_POPUP,
    _XM_ALERT_POPUP,
    _NEWS_POPUP_FROM_RADIO,
    _ALARM_POPUP_FROM_RADIO,
    _VR_NOT_SUPPORT_LANGUAGE,  //21
    _EPG_XM_POPUP,
    _UPGRADE_NOTIFY_POPUP,
    _COMMON_POPUP_HANDLE,
    _NO_MEIDA_POPUP,
    _NAVI_INVALID_POPUP,
    _ePOPUP_SIRI_VR_NOT_READY,
    _ePOPUP_BT_HANG_UP_QUERY,
    _ePOPUP_NAVI_WARNING_FUEL,
    _ePOPUP_NAVI_CHAGNE_TIME_ZONE,
    _ePOPUP_NAVI_ALRAM,	// 31
    _ePOPUP_NAVI_RESEARCH,
    _ePOPUP_NAVI_GLOBAL,
    _ePOPUP_DAB_TA_ALARM,
    _ePOPUP_LOGIC_OFF_LOW_BATTERY,
    _SD_UPGRADE_NOTIFY_POPUP,
    _ePOPUP_INAVILD_IBOX,
    _ePOPUP_NAVI_AUTHORIZATION_FAIL,     // ׺ SD  н ǥϴ ˾ ID ߰..
    _ePOPUP_XM_ALERT_ACTIVE,
    _ePOPUP_LOGIC_ON_LOW_BATTERY,
    _ePOPUP_NOT_AVAILABLE_DURING_CONNECTECD_CAR_MODE,
    _ePOPUP_ANDROID_AUTO_BLOCK_FOR_BL,
    _ePOPUP_MEIDA_BLOCK_FOR_CARPLAY_SIRI,
    _ePOPUP_MAX_ID
};



struct POPUP_TEXT_STYLE_T
{
   wchar_t msg[ POPUP_CONTENT_LENGHT + 1 ];  /** Text line */
   int size;                                 /** Font size in pixel */
   int spacing;                              /** Spacing to next line */
   char argb[4];                             /** Color: Alfa channel, Red, Blue, Green */
};

struct POPUP_TYPE_MESSAGE_BOX_T
{
   POPUP_MESSAGE_BOX_SIZE_T size;
   wchar_t msg[ POPUP_CONTENT_LENGHT + 1 ];
   int statusbar;
};

struct POPUP_TYPE_TOAST_T
{
   wchar_t msg[ POPUP_CONTENT_LENGHT + 1 ];
   int statusbar;
};

struct POPUP_TYPE_XM_ALERT_T
{
    //head
    wchar_t head[ POPUP_TITLE_LENGHT + 1 ];
    //contents
    wchar_t contents[512 + 1 ];
    //date
    wchar_t date[ POPUP_TITLE_LENGHT + 1 ];
    //x
    int x;
    //y
    int y;
    //button1
    wchar_t btn1_txt[ POPUP_TITLE_LENGHT + 1 ];
    //button2
    wchar_t btn2_txt[ POPUP_TITLE_LENGHT + 1 ];
    int map_x;
    int map_y;
    int statusbar;
};

struct POPUP_TYPE_TEXT_T
{
   wchar_t title[ POPUP_TITLE_LENGHT + 1 ];                          /** Title of text popup */
   wchar_t msg[ POPUP_CONTENT_LENGHT + 1 ];                          /** Content of text popup */
   int btn_count;                                                    /** Buttons count */
   POPUP_BUTTON_TYPE_T btn[ POPUP_BUTTON_MAX ];                      /** Type of buttons */
   wchar_t btn_txt[ POPUP_BUTTON_MAX ][ POPUP_TITLE_LENGHT + 1 ];    /** Text/File url for buttons */
   int wrapMode;
   int elideMode;
   int statusbar;
   POPUP_USE_ANIMATION_T useAnimation;
};

struct POPUP_TYPE_CUSTOM_TEXT_T
{
   wchar_t title[ POPUP_TITLE_LENGHT + 1 ];                          /** Title of text popup */
   int msg_count;                                                    /** Count of text line */
   POPUP_TEXT_STYLE_T msg_style[ POPUP_TEXT_LINE_MAX ];              /** Text & style for each lines */
   int btn_count;                                                    /** Buttons count */
   POPUP_BUTTON_TYPE_T btn[ POPUP_BUTTON_MAX ];                      /** Type of buttons */
   wchar_t btn_txt[ POPUP_BUTTON_MAX ][ POPUP_TITLE_LENGHT + 1 ];    /** Text/File url for buttons */
   int statusbar;
};

struct POPUP_TYPE_LIST_T
{
   wchar_t title[ POPUP_TITLE_LENGHT + 1 ];                          /** Title of text popup */
   wchar_t list[ POPUP_CONTENT_LENGHT * 4 + 4 ];                     /** Content of list */
   int btn_count;                                                    /** Buttons count */
   POPUP_BUTTON_TYPE_T btn[ POPUP_BUTTON_MAX ];                      /** Type of buttons */
   wchar_t btn_txt[ POPUP_BUTTON_MAX ][ POPUP_TITLE_LENGHT + 1 ];    /** Text/File url for buttons */
   int statusbar;
};

struct POPUP_TYPE_NAVI_TEXT_T
{
    wchar_t title[ POPUP_TITLE_LENGHT + 1 ];                          /** Title of text popup */
    wchar_t msg[ POPUP_CONTENT_LENGHT + 1 ];                          /** Content of text popup */
    int btn_count;                                                    /** Buttons count */
    POPUP_BUTTON_TYPE_T btn[ POPUP_BUTTON_MAX ];                      /** Type of buttons */
    wchar_t btn_txt[ POPUP_BUTTON_MAX ][ POPUP_TITLE_LENGHT + 1 ];    /** Text/File url for buttons */
    wchar_t traffic_icon[ POPUP_CONTENT_LENGHT + 1 ];
    wchar_t road_icon[ POPUP_CONTENT_LENGHT + 1 ];
    wchar_t road_number[ POPUP_CONTENT_LENGHT + 1 ];
    int wrapMode;
    int elideMode;
    int statusbar;
    POPUP_USE_ANIMATION_T useAnimation;
};


/** Popup show */
struct POPUP_SHOW_T
{
   PUPUP_TYPE_T type;      /** Popup type */
   union
   {
      POPUP_TYPE_TEXT_T text;
      POPUP_TYPE_MESSAGE_BOX_T msg_box;
      POPUP_TYPE_CUSTOM_TEXT_T custom_text;
      POPUP_TYPE_LIST_T list;
      POPUP_TYPE_TOAST_T toast;
      POPUP_TYPE_XM_ALERT_T xm_alert;
	  POPUP_TYPE_NAVI_TEXT_T navitext;
   } popup;
   int source;
   int target;
   int timeout;
   bool ignoreautoclose; //Used Only GUI Change POPUP
   int guichagneButtonNum; //Used Only GUI Change POPUP
   FG_POPUP_T popup_type;
   int responsesource;
};

/** Popup response */
struct POPUP_RESPONSE_T
{
   PUPUP_TYPE_T type;      /** Popup type */
   POPUP_BUTTON_T btn;     /** Timeout or button ID */
   unsigned int index;     /** Index of StringList/RadioButton/CheckBox */
   int value;              /** checkbox/slider value */
   FG_POPUP_T popup_type;
   int responsesource;
};



//------------------------------------------------------
// !!!!!!! Will be deleted !!!!!!!!!
//------------------------------------------------------
struct POPUP_BASE_T
{
   PUPUP_TYPE_T type;
};
struct POPUP_WARNING_T
{
   char sMessageTitle[ POPUP_TITLE_LENGHT + 1];
   char sMessageContent[ POPUP_CONTENT_LENGHT + 1 ];
};
struct POPUP_WARNING_DATA_T
{
   POPUP_BASE_T header;
   POPUP_WARNING_T warning;
};
struct POPUP_MESSAGE_BOX_DATA_T
{
   POPUP_BASE_T header;
   char sMessageContent[ POPUP_CONTENT_LENGHT + 1 ];
};
//------------------------------------------------------

#endif // DHAVN_APPFRAMEWORK_POPUP_H
