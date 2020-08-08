#ifndef AHAPREDEFINEDVALUES_H
#define AHAPREDEFINEDVALUES_H

#include <QVector>
#include <stdint.h>
#include <stdio.h>
#include <QFileInfo>
#include <QDir>

const QString c_AppId = "com.ahamobile.link.v1";
const QString c_BundleSeedId = "LL29PAF236";
const QString c_BundleAppId = "com.ahamobile.shoutapp";
const quint32 c_Max_Payload_Length_USB = 2000;

#define AHA_TRANSACTION_ID_LIMIT 32767

#define AHA_STATION_LOGO_PATH   "/app/data/AppAhaRadio/cache/Aha/StationLogo/"
#define AHA_PARTNER_LOGO_PATH   "/app/data/AppAhaRadio/cache/Aha/PartnerLogo/"
#define AHA_ALBUM_ART_PATH      "/app/data/AppAhaRadio/cache/Aha/AlbumArt/"
#define AHA_STATION_PRESET_PATH "/app/data/AppAhaRadio/cache/Aha/Presets/"
#define AHA_CONTENT_IMAGES_PATH "/app/data/AppAhaRadio/cache/Aha/ContentImages/"


#define TIME_INITCOMPLETION     4000    //4s: time limit between ipod->init and initcompleted response received.
#define TIME_REPLYWAIT          2000    //2s: time limit between sending request and receiving reply.
#define TIME_REPLYWAIT_BT       5000    //2s: time limit between sending request and receiving reply.
#define TIME_PEERDELAY_LONG     4000    //4s: time limit before start sending startPeerSession request.
#define TIME_PEERDELAY_SHORT    2000    //2s: time limit before start sending startPeerSession request.
#define TIME_PEER_RESPONSE_WAIT 20000   //20s: time limit between sending startPeerRequest and startpeersessiondecoded.
#define TIME_APPLAUNCH          10000   //10s:time limit to wait for application to launch.
#define MAX_PEER_RETRY          15      //15x: number of peer retry before we give up.
#define TIME_QUERY_RESPONSE_WAIT   20000      //10s: time limit before query response
#define TIME_BUFFERING_INTERVAL_WAIT   10000
#define TIME_RETRY_PEER_SESSION 4000    //4s: every 4s send startPeerSession signal on Android phone.

#define TIME_PING_INTERVAL 5000     //5 secs
#define TIME_PING_WAIT     30000    //30 sec, time to wait until we start to ping
#define MAX_PING_RETRY     5       //
//wsuk.kim 131120 REVERT. remain receiving station popup.   #define MAX_STATION_COUNT 35

#define ALBUM_ART_WIDTH 232//128 //or 92
#define ALBUM_ART_HEIGHT 219//128

#define STATION_LOGO_WIDTH 74//92
#define STATION_LOGO_HEIGHT 74//92

#define PARTNER_LOGO_WIDTH 150
#define PARTNER_LOGO_HEIGHT 50

#define RATING_IMAGE_WIDTH 150
#define RATING_IMAGE_HEIGHT 50

#define LIKE_IMAGE_WIDTH 60
#define LIKE_IMAGE_HEIGHT 60

//#define DEBUG_COMMON_ENABLED
//#define DEBUG_ENCODER_ENABLED
//#define DEBUG_DECODER_ENABLED
//#define DEBUG_STATEHANDLER_ENABLED
//#define DEBUG_USBFRAME_ENABLED

//#define TAN_DEBUG

#ifdef TAN_DEBUG
    #define TAN_PRINTF printf
#else
    #define TAN_PRINTF(x,...)
#endif

#ifdef DEBUG_COMMON_ENABLED
    #define DEBUG_COMMON_UTIL TAN_PRINTF
#else
    #define DEBUG_COMMON_UTIL(x,...)
#endif

#ifdef DEBUG_ENCODER_ENABLED
    #define DEBUG_ENCODER TAN_PRINTF
#else
    #define DEBUG_ENCODER(x,...)
#endif

#ifdef DEBUG_DECODER_ENABLED
    #define DEBUG_DECODER TAN_PRINTF
#else
    #define DEBUG_DECODER(x,...)
#endif

#ifdef DEBUG_STATEHANDLER_ENABLED
    #define DEBUG_STATEHANDLER TAN_PRINTF
#else
    #define DEBUG_STATEHANDLER(x,...)
#endif

#ifdef DEBUG_USBFRAME_ENABLED
    #define DEBUG_USBFRAME TAN_PRINTF
#else
    #define DEBUG_USBFRAME(x,...)
#endif

#define START_AHA_PACKAGE 0xB6B6
#define AHA_HEADER_LENGTH 10


#define MAX_CONTENT_PER_QUERY 10
#define MAX_PRESET_STATION_PER_QUERY 5


//start session package 0x01
#define AHA_PROTOCOL_VERSION 11205 //1.12.5
#define AHA_START_SESSION_FLAGS 0
#define AHA_MAX_LENGTH_RETURN 0
#define AHA_MAKE_STRING "hyundai"
#define AHA_MODEL_STRING ""
#define AHA_HU_SERIAL_STRING "SN123"
#define AHA_DELAY_TRACK 0
#define AHA_HU_MODEL_STRING "HU123"
#define AHA_CAR_YEAR_STRING "2012"
#define AHA_VIN_STRING "VIN1"
#define AHA_MILEAGE 135
#define SHOUT_FORMAT 2
//all values are little endian

//Query Stations 0x04
#define AHA_MAX_NUM_OF_PRESET_STATION 0

//Query binary data 0x09
#define N_MAX_DATA_FRAGMENT 200
#define MAX_LEN_OF_DATA_RET 5000


struct NMEAGPGGA{
 float UTCTime;
 float Latitude;
 unsigned char NSIndicator; //(0:N, 1:S)
 float Longitude;
 unsigned char EWIndicator; //(0:E, 1:W)
 unsigned char PosFix;
 unsigned char SatelUsed;
 float HDOP;
 float Altitude;
 unsigned char AltitudeUnit; //(0:M) always 0
 float GeoAlt;
 unsigned char GeoAltUnit; //(0:M) always 0
 float DGPSTime;
 unsigned char DGPStation;
};

struct NMEAGPRMC{
 float UTCTime;
 unsigned char Status; //(0:V, 1:A)
 float Latitude;
 unsigned char NSIndicator; //(0:N, 1:S)
 float Longitude;
 unsigned char EWIndicator; //(0:E, 1:W)
 float SpdOverGnd;
 float CrsOverGnd;
 unsigned long UTCDate;
 float MagVar;
 unsigned char MagVarEWInd; // always 0
};

struct NMEAGPS{
 NMEAGPRMC GPS_RMC;
 NMEAGPGGA GPS_GGA;
};

//공유 메모리 키
#define NAVI_GPSNMEA_SHARED_MEMORY  "NaviNMEAGPSData"
//


typedef enum
{
    HEADUNIT_POWER_OFF,
    HEADUNIT_POWER_ON
}HEADUNIT_POWER_STATE_T;


typedef enum
{
    AHA_GPS_NONE, //hsryu_0312_navi_exception
    AHA_GPS_NO_NAVI,
    AHA_GPS_WEAK,
    AHA_GPS_OK
}AHA_GPS_STATUS_T;

typedef enum
{
    EAP_PANDORA_PROTOCOL_INDEX = 0x01,
    EAP_AHA_PROTOCOL_INDEX = 0x02
}EAP_PROTOCOL_INDEX_ENUM;

//borrowed from pandora middleware
typedef enum
{
    DEVICE_TYPE_NONE = 0,                           /** No active device */
    DEVICE_TYPE_SMARTPHONE = 1,                /** Device type is Smartphone */
    DEVICE_TYPE_IPHONE_USB_FRONT = 2,       /** Device type is iPhone, which is connected through front USB */
    DEVICE_TYPE_IPHONE_USB_REAR = 3,        /** Device type is iPhone, which is connected through rear USB */
    DEVICE_TYPE_MAX = 4                              /** Max number of devices possible */
}EDeviceType;

typedef enum
{
    REQ_TYPE_NONE = 0,      /** No Request */
    REQ_TYPE_CONNECT = 1,    /** Connect request */
    REQ_TYPE_TRACK_INFO = 2,    /** Track info request */
    REQ_TYPE_PLAY = 3,        /** Play request */
    REQ_TYPE_PAUSE = 4,        /** Pause request */
    REQ_TYPE_SKIP = 5,        /** Skip request */
    REQ_TYPE_RATE = 6,        /** Rate request */
    REQ_TYPE_BOOKMARK_SONG = 7,    /** Bookmark song request */
    REQ_TYPE_BOOKMARK_ARTIST = 8,    /** Bookmark Artist request */
    REQ_TYPE_STATION_LIST = 9,        /** Station List request */
    REQ_TYPE_STATION_SELECT = 10,    /** Station Select request */
    REQ_TYPE_SEARCH = 11,            /** Extended Search request */
    REQ_TYPE_SEARCH_AUTO_COMPLETE = 12,    /** Auto complete search request */
    REQ_TYPE_SEARCH_SELECT = 13,        /** Search Select request */
    REQ_TYPE_DISCONNECT = 14,            /** Disconnected request */
    REQ_TYPE_EXPLAIN = 15,       /** Explanation for station request*/
    REQ_TYPE_STATION_COUNT = 16, /** Station count request */
    REQ_TYPE_SORT = 17,          /** Sort Request*/
    REQ_TYPE_DELETE_STATION = 18,  /** Station delete request */
} ERequestType;

typedef enum
{
    OSD_AV_OFF, //hsryu_0228 ITS # 0158377
    OSD_TRACK_INFO,
    OSD_CANNOT_SKIP,
    OSD_CANNOT_SKIP_BACK,
    OSD_CANNOT_REW15,
    OSD_CANNOT_FW30,
    OSD_CANNOT_PLAY_BT_CALL, //hsryu_0423_block_play_btcall
    OSD_TRACK_REW15,
    OSD_TRACK_FW30,
    OSD_TUNE_CURRENT,   //0404
    OSD_TUNE_UPDOWN,    //wsuk.kim OSD_TUNE
    OSD_NO_NETWORK,      //wsuk.kim 131204 ITS_212572 state network error, display error info on OSD.
    OSD_CONNECTING_TO_AHA       // connecting to aha...
}OSDDisplayType;

typedef enum
{
    E_SUCCESS = 0,
    E_INVALID_STATE = 1, /** Request made in wrong state of middleware */
    E_INIT_FAILED = 2,   /** Init failed */
    E_DEVICE_NOT_FOUND = 3, /** Device not found */
    E_BT_CONNECTION_FAILED = 4, /** Device connection failed */
    E_USB_CONNECTION_FAILED = 5, /** USB Connection failed */
    E_CHECK_AHA_APP_ON_DEVICE = 6, /** Check if Aha app running on device */
    E_NETWORK_FAILED = 7, /** Either connection with Aha device OR connection of
                          Aha device with network lost*/
    E_INVALID_TOKEN = 8,  /** Given token for station is not valid */
    E_INVALID_STRING = 9, /** Empty OR doesn't follow the input criteria */
    E_INVALID_STATION_REQ_RANGE = 10,   /** Requested Station list range is not valid */
    E_INVALID_VERSION = 11,    /** The API VERSION is incompatible */
    E_NO_STATIONS = 12,        /** No stations are found */
    E_NO_ACTIVE_STATIONS = 13,  /** No station is active */
    E_SKIP_LIMIT_REACHED = 14,  /** The maximun skip limit has been reached */
    E_STATION_LIMIT_REACHED = 15, /** The maximun station limit has been reached */
    E_TRACK_RATING_FAILED = 16,   /** Track Rating has failed */
    E_STATION_DELETE_FAILED = 17, /** Deletion of the station has failed */
    E_SEARCH_EXTENDED_FAILED = 18, /** Extended search has failed */
    E_SEARCH_SELECT_FAILED = 19, /** Select from search result has failed */
    E_BOOKMARK_FAILED = 20,     /** Bookmark has failed */
    E_TRACK_EXPLAIN_FAILED = 21,     /** Track Explain Request has failed */
    E_MEMORY = 22,   /** Required Memory not allocated*/
    E_INVALID_ARGUMENT = 23,     /** Argument Passed are invalid*/
    E_REQUEST_TIMEOUT = 24,      /** Timeout error */
    E_UNKNOWN_ERROR   = 25,      /** Some unknown error occured */
    E_SESSION_ALREADY_STARTED   = 26,      /** Session is already started */
    E_NO_ACTIVE_SESSION  = 27,      /** There is no active session */
    E_APP_URL_NOT_SUPPORTED   = 28,      /** App Url is not supported */
    E_STATION_DOES_NOT_EXIST  = 29,      /** Station Does Not exist */

    E_AHA_PLEASE_LOGIN = 100,
    E_AHA_PROT_NOT_SUPPORTED = 101,
    E_AHA_SESSION_REJECTED = 102,
    E_AHA_SESSION_CLOSED = 103,
    E_AHA_APP_NOT_OPENED = 104,
    E_REPEAT_BUFFERING = 105  //wsuk.kim 130904 ITS_0182092 repeat buffering
} EAhaError;

typedef enum
{
    SORT_TYPE_NONE = -1, /** No Sort Type */
    SORT_TYPE_DATE,    /** Sorting of station based on Date */
    SORT_TYPE_ALPHA /** Alphabatical sorting of stations */
} ESortType;

typedef struct
{
    quint32 nMusicToken; /** Identifier within search */
    quint8 nType;
    //TO DO: move these 3 to enum
   // bool bArtist;        /** Is this an Artist? */
   // bool bSong;            /** Is this a song? */
   // bool bOther;        /** Does it come in other category? (Not artist/ Not song)*/
    QString Label;        /** Artist Name/Track Name/ Other label*/
} TSSearchInfo;

typedef struct
{
    quint32  nStationToken;    /** Station identifier t*/
    QString StationArtPath;    /** Station Art Path*/
} TSStationArt;

typedef struct
{
    quint32  nStationToken;    /** Station identifier in Station List or Search Station List*/
    bool bQuickMix;   //Flag field for QuickMix/Delete/Shared
    QString StationLabel;    /** Station Name*/
} TSStationInfo;

//

typedef enum
{
    AHA_PRI_NONE_STATE,
    AHA_PRI_CONNECT_SYNCING_STATE,
    AHA_PRI_STATION_CHANGING_STATE,
    AHA_PRI_PRESET_QUERYING_STATE,
    AHA_PRI_CONTENT_CHANGING_STATE
}AHA_PRIMARY_STATE_T;

enum
{
    AHA_RATE_NONE_SELECTED,
    AHA_RATE_LIKE_SELECTED,
    AHA_RATE_DISLIKE_SELECTED,
    AHA_RATE_BOTH_SELECTED
};

enum
{
    AHA_IMAGE_LIKE_S,
    AHA_IMAGE_LIKE_U,
    AHA_IMAGE_DISLIKE_S,
    AHA_IMAGE_DISLIKE_U,
    AHA_IMAGE_LIKE_MAX_COUNT
};

typedef enum
{
    //0x0E
    AHA_PLAYBACK_STOP = 0,
    AHA_PLAYBACK_PAUSE = 1,
    AHA_PLAYBACK_PLAY = 2,
    AHA_PLAYBACK_BUFFERING = 3
}AHA_PLAYBACK_T;

//ITS_0228611
typedef enum
{
    AHA_NOTIFY_DISCONNECTED = -1,
    AHA_NOTIFY_RESUME_NORMAL= 0,
    AHA_NOTIFY_SERVER_LOST  = 1,
    AHA_NOTIFY_SERVER_DOWN  = 2,
    AHA_NOTIFY_CONNECTING   = 3
}AHA_NOTIFY_STATUS_T;

typedef enum
{
    AHA_PLAY_REQUEST,
    AHA_PAUSE_REQUEST,
    AHA_NEXT_REQUEST,
    AHA_BACK_REQUEST,
    AHA_REWIND15_REQUEST,
    AHA_FORWARD30_REQUEST,
    AHA_LIKE_VOTE_UNVOTE_REQUEST,
    AHA_DISLIKE_VOTE_UNVOTE_REQUEST,
    AHA_SELECT_STATION_TO_PLAY_REQUEST,
    AHA_SELECT_CONTENT_TO_PLAY_REQUEST,
    AHA_LOG_CALL_REQUEST,
    AHA_LOG_NAVIGATE_REQUEST
}AHA_REQUEST_TYPE_T;

typedef enum
{
    AHA_CONTENT_ID_BACK = 0,
    AHA_CONTENT_ID_SMART_BACK = 1,
    AHA_CONTENT_ID_NEXT = 2
}AHA_RESERVED_CONTENT_ID_T;

typedef enum
{
    AHA_BOOLEAN_PARAM = 0,
    AHA_UINT64_PARAM = 1,
    AHA_INT64_PARAM = 2 ,
    AHA_REAL_PARAM = 3,
    AHA_STRING_PARAM = 4
}AHA_PARAM_DATA_TYPE_T;

typedef enum
{
    STOPPED,
    PAUSED,
    PLAYING,
    BUFFERING
}PLAYBACKSTATE_T;

typedef enum
{
    QUERY_MODE_UNKNOWN_MODE,
    //parameters query
    QUERY_MODE_ACTIVE_STATION_PARAM,
    QUERY_MODE_PRESET_STATION_PARAM,
    QUERY_MODE_ACTIVE_CONTENT_PARAM,
    QUERY_MODE_LIST_CONTENT_PARAM,
    QUERY_MODE_COMPLETE = 5,

    QUERY_MODE_SIMPLE,
    QUERY_MODE_ALBUM_ART,
    QUERY_MODE_PARTNER_LOGO,
    QUERY_MODE_STATION_LOGO,
    QUERY_MODE_PRESET_LOGO = 10,

    QUERY_MODE_RATING_IMAGE,
    QUERY_MODE_LIKE_IMAGE,
    QUERY_MODE_BACK,
    QUERY_MODE_NEXT,
    QUERY_MODE_PAUSE = 15,

    QUERY_MODE_PLAY,
    QUERY_MODE_STOP,
    QUERY_MODE_REWIND15,
    QUERY_MODE_FORWARD30,
    QUERY_MODE_VOTE_LIKE = 20,

    QUERY_MODE_VOTE_DISLIKE,
    QUERY_MODE_UNVOTE,
    QUERY_MODE_LOG_NAVIGATE,
    QUERY_MODE_LOG_CALL,
    QUERY_MODE_PING = 25,

    QUERY_MODE_IGNORE_STATION_ID
}AHA_QUERY_MODE_T;

typedef enum
{
    AHA_ACT_VOTE_LIKE                = 0x03,
    AHA_ACT_RETWEET                  = 0x04,
    AHA_ACT_TWEET_CUR_LOC            = 0x05,
    AHA_ACT_UPLOAD_CURR_LOC_FACEBOOK = 0x06,
    AHA_ACT_VOTE_DISLIKE             = 0x07,
    AHA_ACT_UNVOTE                   = 0x08,
    AHA_ACT_LOG_NAVIGATION           = 0x09,
    AHA_ACT_LOG_CALL                 = 0x0A,
    AHA_ACT_PING                     = 0x0B
}AHA_ACT_ENUM_VALUE_T;

typedef enum
{
    AHA_CONT_PAR_LIKED                  = 0x01,
    AHA_CONT_PAR_CAN_PLAY               = 0x02,
    AHA_CONT_PAR_CAN_PAUSE              = 0x03,
    AHA_CONT_PAR_CAN_NEXT               = 0x04,
    AHA_CONT_PAR_CAN_BACK               = 0x05,
    AHA_CONT_PAR_CAN_CHANGE             = 0x06,
    AHA_CONT_PAR_CAN_TIMESHIFT          = 0x07,
    AHA_CONT_PAR_CAN_LIKE               = 0x08,
    AHA_CONT_PAR_CAN_DISLIKE            = 0x09,
    AHA_CONT_PAR_CAN_UNVOTE             = 0x0A,
    AHA_CONT_PAR_DISLIKED               = 0x0B,

    AHA_CONT_PAR_TOTAL_LIKES            = 0x1001,
    AHA_CONT_PAR_TIMESTAMP              = 0x1002,
    AHA_CONT_PAR_NUM_REVIEWS            = 0x1003,

    AHA_CONT_PAR_SPEED                  = 0x2001,
    AHA_CONT_PAR_NORMAL_SPEED           = 0x2002,

    AHA_CONT_PAR_LON                    = 0x3001,
    AHA_CONT_PAR_LAT                    = 0x3002,
    AHA_CONT_PAR_RATING                 = 0x3003,
    AHA_CONT_PAR_MAX_RATING             = 0x3004,

    AHA_CONT_PAR_AUDIO_URL              = 0x4001,
    AHA_CONT_PAR_TITLE                  = 0x4002,
    AHA_CONT_PAR_IMAGE_URL              = 0x4003,
    AHA_CONT_PAR_LOC_STRING             = 0x4004,
    AHA_CONT_PAR_USER_NAME              = 0x4005,
    AHA_CONT_PAR_ZIP                    = 0x4006,
    AHA_CONT_PAR_ADDRESS1               = 0x4007,
    AHA_CONT_PAR_STATE                  = 0x4008,
    AHA_CONT_PAR_CITY                   = 0x4009,
    AHA_CONT_PAR_PHONE                  = 0x400A,
    AHA_CONT_PAR_FULL_ADDRESS           = 0x400B,
    AHA_CONT_PAR_ADDRESS2               = 0x400C,
    AHA_CONT_PAR_COUNTRY_CODE           = 0x400D,
    AHA_CONT_PAR_LANG_CODE              = 0x400E,
    AHA_CONT_PAR_RATING_IMG_URL         = 0x400F
}AHA_CONTENT_PARAM_T;

typedef enum
{
    AHA_STT_PAR_LIKE_ALLOWED                        = 0x03,
    AHA_STT_PAR_READ_ONLY                           = 0x04,
    AHA_STT_PAR_TWITTER                             = 0x06,
    AHA_STT_PAR_LBS                                 = 0x07,
    AHA_STT_PAR_TIMESHIFT                           = 0x08,
    AHA_STT_PAR_P_TWITTER                           = 0x09,
    AHA_STT_PAR_DYNAMIC                             = 0x0A,
    AHA_STT_PAR_ON_DEMAND_SEARCH                    = 0x0B,
    AHA_STT_PAR_CONFIG                              = 0x0C,
    AHA_STT_PAR_FACEBOOK                            = 0x0D,
    AHA_STT_PAR_POST_TEXT                           = 0x0E,

    AHA_STT_PAR_RECORD_TIME                         = 0x1004,

    AHA_STT_PAR_ST_DESC                             = 0x4001,
    AHA_STT_PAR_ST_ICON_URL                         = 0x4002,
    AHA_STT_PAR_FULL_NAME                           = 0x4003,
    AHA_STT_PAR_SHORT_NAME                          = 0x4004,
    AHA_STT_PAR_PARTNER_URL                         = 0x4005,
    AHA_STT_PAR_LIKE_U_URL                          = 0x4006,
    AHA_STT_PAR_DISLIKE_U_URL                       = 0x4007,
    AHA_STT_PAR_LIKE_S_URL                          = 0x4008,
    AHA_STT_PAR_DISLIKE_S_URL                       = 0x4009,
    AHA_STT_PAR_ST_ID_STRING                        = 0x4010
}AHA_STATION_PARAM_T;

typedef enum
{
    AHA_ERR_NO_ERROR                            = 0x00,
    AHA_ERR_PROT_NOT_SUPPORTED                  = 0x01,
    AHA_ERR_SESSION_REJECTED                    = 0x02,
    AHA_ERR_CARINFO_REJECTED                    = 0x03,
    AHA_ERR_LOCATION_UNAVAILABLE                = 0x04,
    AHA_ERR_NETWORK_ERROR                       = 0x05,
    AHA_ERR_INVALID_OPERATION                   = 0x06,
    AHA_ERR_NOT_LOGGED_IN                       = 0x07,
    AHA_ERR_LOGIN_FAILED                        = 0x08,

    AHA_ERR_INVALID_STATION_ID                  = 0x12,
    AHA_ERR_INVALID_CONTENT_ID                  = 0x13,
    AHA_ERR_INVALID_PARAMETER                   = 0x14,
    AHA_ERR_INVALID_RESOURCE                    = 0x15,
    AHA_ERR_INVALID_ACTION                      = 0x16,
    AHA_ERR_NEED_MYFEEDS_STATION                = 0x17,

    AHA_ERR_UNAVAILABLE_RESOURCE                = 0x21,
    AHA_ERR_INCOMPLETE_UPLOAD                   = 0x22,
    AHA_ERR_INCONSISTENT_LENGTH                 = 0x23,
    AHA_ERR_CANNOT_TIMESHIFT                    = 0x31
}AHA_ERR_CODE_T;

typedef enum
{
    AHA_OPCODE_UNKNOWN_OPCODE                       = 0x00,
    AHA_OPCODE_START_PEER_SESSION                   = 0x01,
    AHA_OPCODE_VEHICLE_INFORMATION_UPDATE           = 0x02,
    AHA_OPCODE_QUERY_STATIONS                       = 0x04,
    AHA_OPCODE_QUERY_STATION_PARAMETERS             = 0x06,
    AHA_OPCODE_QUERY_STATION_CONTENT                = 0x07,
    AHA_OPCODE_QUERY_CONTENT_PARAMETERS             = 0x08,
    AHA_OPCODE_QUERY_BINARY_DATA                    = 0x09,
    AHA_OPCODE_POST_NEW_RECORDING                   = 0x0A,
    AHA_OPCODE_QUERY_CURRENT_STATION                = 0x0B,
    AHA_OPCODE_SELECT_STATION_FOR_PLAYBACK          = 0x0C,
    AHA_OPCODE_SELECT_CONTENT_FOR_PLAYBACK          = 0x0D,
    AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE         = 0x0E,
    AHA_OPCODE_TIME_SHIFT_CONTENT                   = 0x0F,
    AHA_OPCODE_REQUEST_MORE_CONTENT_FOR_STATION     = 0x10,
    AHA_OPCODE_PERFORM_ACTION                       = 0x11,
    AHA_OPCODE_REQUEST_CURRENT_CONTENT_INFORMATION  = 0x12,
    //client initiated.
    AHA_OPCODE_NOTIFY_PLAYBACK_OF_NEW_STATION       = 0x0101,
    AHA_OPCODE_NOTIFY_NEW_CONTENT_PLAYBACK_STATE    = 0x0102,
    AHA_OPCODE_NOTIFY_NEW_CONTENT_AVAILABLE         = 0x0103,
    AHA_OPCODE_NOTIFY_CONTENT_DELETED               = 0x0104,
    AHA_OPCODE_NOTIFY_CLIENT_STATUS_UPDATE          = 0x0105,
    AHA_OPCODE_NOTIFY_CLIENT_POSITION_UPDATE        = 0x0106,
    AHA_OPCODE_NOTIFY_3rd_PARTY_CONNECTIVITY_CHANGE = 0x0107,    //this is not implemented in client yet.
    AHA_OPCODE_NOTIFY_SUBSCRIBED_STATION_LIST_CHANGE= 0x0108,
    AHA_OPCODE_NOTIFY_PENDING_REQUEST               = 0x0109
}AHA_OPCODE_T;

typedef struct
{
    qint32    nTrackToken;          /** Track identifier for current station*/
    qint32    nAlbumArtLength;      /**  ALbum art length field */
    qint16    nDuration;            /** Duration of current station (Seconds)*/
    qint16    nElapsed;             /** Current position of track (Seconds)*/
    quint8    nRating;              /** 1 for Thumb up, 2 for Thumb Down, 0 for no rating*/
    bool    bAllowRating;           /** Is it allowed to rate this track */
    bool    bAllowLike;             /** is it allowed to like */
    bool    bAllowDislike;          /** is it allowed to dislike */
    bool    bAllowSkip;             /** Is it allowed to skip this track */
    bool    bAllowSkipBack;         /** Is it allowed to skip back to previous track from this track */
    bool    bAllowCall;             /** is it allowed to call*/
    bool    bAllowNavigate;         /** is it allowed to navigate*/
    bool    bAllowTimeShift;        /** is it allowed to timeshift this track*/
    bool    bAllowBookmark;         /** Is it allowed to bookmark this track */
    bool    bAllowExplain;          /** Is it allowed to explain this track */
    bool    bAd;                    /** Is this track an Ad */
    bool    bTrackBookmarked;       /** Is this track already bookmarked */
    bool    bArtistBookmarked;      /** Is this artist already bookmarked */   
    QString Title;                  /** Track title */
    QString Artist;                 /** Artist for track */
    QString Album ;                 /** Track Album    */
    QString AlbumArtUrl ;           /** The url of album art where received image will be saved */
    QString CurrentStation;         /** the current station name */
    quint32 StationToken;           /** the current station token */
} AhaTSTrackInfo;



typedef struct _AhaStationBriefInfo_T{
    uint64_t nStationID;
    QString strStationFullName;
    QString strStationIconURL;
    uint8_t nIsLBS;
}AhaStationBriefInfo_T;

typedef struct _AhaContentBriefInfo_T{
    uint64_t nContentID;
    QString strTitle;
}AhaContentBriefInfo_T;

typedef struct _TransactionRecordTable_T{
    uint16_t nTransactionID;
    uint16_t nOpCode;
    AHA_QUERY_MODE_T nQueryMode;
}TransactionRecordTable_T;

typedef struct _AhaClientInfo_T
{
    uint8_t nGPSAvailable;
    QString strAppVersion;
    QString strDevModel;
    QString strDevSoftVersion;

    uint64_t nActiveStationID;
    uint64_t nActiveContentID;
    uint8_t nPlaybackState;
    uint32_t nElapsedSecs;

    QVector <uint64_t> nQVectPresetStationID;
    QVector <uint64_t> nQVectListContentID;

    void clear()
    {
        nGPSAvailable = 0;
        strAppVersion.clear();
        strDevModel.clear();
        strDevSoftVersion.clear();

        nActiveStationID = 0;
        nActiveContentID = 0;
        nPlaybackState = 0;
        nElapsedSecs = 0;
        nQVectPresetStationID.clear();
        nQVectListContentID.clear();
    }
}AhaClientInfo_T;

typedef struct _AhaStationInfo_T
{
    uint64_t nStationID;
    uint8_t nLikeAllowed;
    uint8_t nReadOnly;
    uint8_t nIsTwitter;
    uint8_t nIsLBS;
    uint8_t nCanTimeShift;
    uint8_t nIsDynamic;
    uint8_t nIsFacebook;
    uint8_t nCanPostText;
    uint64_t nRecordTime;
//  QString strStationDesc;
    QString strStationIconURL;
    QString strStationFullName;
//  QString strStationShortName;
    QString strPartnerURL;
    QString strLikeUnselectedURL;
    QString strDislikeUnselectedURL;
    QString strLikeSelectedURL;
    QString strDislikeSelectedURL;
//  QString strStationID;

    void clear()
    {
        nStationID = 0;
        nLikeAllowed = 0;
        nReadOnly = 0;
        nIsTwitter = 0;
        nIsLBS = 0;
        nCanTimeShift = 0;
        nIsDynamic = 0;
        nIsFacebook = 0;
        nCanPostText = 0;
        nRecordTime = 0;

        strStationIconURL.clear();
        strStationFullName.clear();
        strPartnerURL.clear();
        strLikeSelectedURL.clear();
        strDislikeSelectedURL.clear();
        strLikeUnselectedURL.clear();
        strDislikeUnselectedURL.clear();
    }
}AhaStationInfo_T;

typedef struct _AhaContentInfo_T
{
    uint64_t nContentID;
    uint8_t nLiked;
    uint8_t nCanPause;
    uint8_t nCanNext;
    uint8_t nCanBack;
    uint8_t nCanChange;
    uint8_t nCanTimeShift;
    uint8_t nCanLike;
    uint8_t nCanDislike;
    uint8_t nCanUnvote;
    uint8_t nDisliked;

    double dbLon;
    double dbLat;
    double dbRating;

    QString strTitle;
    QString strImageURL;
    QString strZip;
    QString strAddress1;
    QString strState;
    QString strCity;
    QString strPhone;
    QString strFullAddress;
    QString strAddress2;
    QString strCountryCode;
    QString strLangCode;
    QString strRatingImageURL;

    QVector <QString> strQVectContentInfoString;

    void clear()
    {
        nLiked	= 0;
        nCanPause = 0;
        nCanNext = 0;
        nCanBack = 0;
        nCanChange	= 0;
        nCanTimeShift = 0;
        nCanLike = 0;
        nCanDislike = 0;
        nCanUnvote = 0;
        nDisliked = 0;

        dbLon = 0;
        dbLat = 0;
        dbRating = 0;

        strTitle.clear();
        strImageURL.clear();
        strZip.clear();
        strAddress1.clear();
        strState.clear();
        strCity.clear();
        strPhone.clear();
        strFullAddress.clear();
        strAddress2.clear();
        strCountryCode.clear();
        strLangCode.clear();
        strRatingImageURL.clear();
        strQVectContentInfoString.clear();
    }
}AhaContentInfo_T;

typedef struct _AhaImageInfo_T
{
    QByteArray imageDataByte;
    uint8_t nIsDownloadComplete;
    uint8_t nFragmentCount;
    uint32_t nOffset;
    uint16_t nTransactionID;
}AhaImageInfo_T;


#endif // AHAPREDEFINEDVALUES_H
