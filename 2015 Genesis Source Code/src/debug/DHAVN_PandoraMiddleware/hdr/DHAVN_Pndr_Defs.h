/*
 ******************************************************************
 *        COPYRIGHT ?2011, LG Electronics.  
 *        All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :    Pandora definitions                            *
 * PROGRAMMER :    Vandana Ahuja                                  *
 * DATE       :    02/12/2011                                     *
 * VERSION    :    0.1                                            *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module defines the structure/enums/const definitions
 *  shared between app and middleware
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS    
 * 03/12/2011    0.1 Vandana Ahuja   header file created
 *
 ******************************************************************
 */


#ifndef DHAVN_PNDR_DEFS_H
#define DHAVN_PNDR_DEFS_H

#include <QObject>

/**
    * The enum used for defining the errors used by Pandora middleware
    * to update application about the possible errors.
    */
typedef enum
{
    E_SUCCESS = 0,
    E_INVALID_STATE = 1, /** Request made in wrong state of middleware */
    E_INIT_FAILED = 2,   /** Init failed */
    E_DEVICE_NOT_FOUND = 3, /** Device(IPod/BT) not found */
    E_BT_CONNECTION_FAILED = 4, /** Device connection failed */
    E_USB_CONNECTION_FAILED = 5, /** USB Connection failed */
    E_CHECK_PNDR_APP_ON_DEVICE = 6, /** Check if Pandora app running on device */
    E_NETWORK_FAILED = 7, /** Either connection with Pandora device OR connection of
                          Pandora device with network lost*/
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

    //E_SERVER_MAINTAINANCE  = 30,      /** Station Does Not exist */
    //E_LICENSING_RESTRICTIONS  = 31 ,//((pndr_status_code)0x08)
    //E_INVALID_LOGIN = 32 //((pndr_status_code)0x09)
	
	E_INSUFFICIENT_CONNECTIVITY = 30,   /** Insufficient connectivity */
    E_LICENSING_RESTRICTIONS = 31,      /** Licensing restriction */
    E_INVALID_LOGIN = 32,                /** Invalid Login */ 
    E_NOTICE_ERROR_MAINTENANCE = 33     /** Pandora Server Maintenance */
	

} EPndrError;

/**
    * This enumaration will be used for selecting the device
    */
typedef enum
{
    DEVICE_TYPE_NONE = 0,   /** No active device */
    DEVICE_TYPE_SMARTPHONE = 1,    /** Device type is Smartphone */
    DEVICE_TYPE_IPHONE_USB_FRONT = 2,        /** Device type is iPhone, which is
                                             connected through front USB */
    DEVICE_TYPE_IPHONE_USB_REAR = 3,        /** Device type is iPhone, which is
                                             connected through rear USB */
    DEVICE_TYPE_MAX = 4           /** Max number of devices possible */
} EDeviceType;

/**
    * This enum will be used while requesting for sort station
    * The order of enum values should not be changed since this is
    * as per the Pandora Document
    */
typedef enum
{
    SORT_TYPE_NONE = -1, /** No Sort Type */
    SORT_TYPE_DATE,    /** Sorting of station based on Date */
    SORT_TYPE_ALPHA /** Alphabatical sorting of stations */

} ESortType;

/**
    * This struct will be used to provide the track information
    * on request from application
    */
typedef struct
{
    qint32    nTrackToken;    /** Track identifier for current station*/
    qint32    nAlbumArtLength;    /**  ALbum art length field */
    qint16    nDuration;        /** Duration of current station (Seconds)*/
    qint16    nElapsed;        /** Current position of track (Seconds)*/
    quint8    nRating;        /** 1 for Thumb up, 2 for Thumb Down, 0 for no rating*/
    bool    bAllowRating;    /** Is it allowed to rate this track */
    bool    bAllowSkip;        /** Is it allowed to skip this track */
    bool    bAllowBookmark;    /** Is it allowed to bookmark this track */
    bool    bAllowExplain;    /** Is it allowed to explain this track */
    bool    bAd;    /** Is this track an Ad */
    bool    bTrackBookmarked;    /** Is this track already bookmarked */
    bool    bArtistBookmarked;    /** Is this artist already bookmarked */
    bool    bShared;             /** Is this station shared */
    QString Title;            /** Track title */
    QString Artist;            /** Artist for track */
    QString Album ;           /** Track Album    */
    QString AlbumArtUrl ;       /** The url of album art where received image will be saved */
    QString CurrentStation;   /** the current station name */
    quint32 StationToken;     /** the current station token */
} TSTrackInfo;

/**
    * This struct will be used to provide the station information to application
    */
typedef struct
{
    quint32  nStationToken;    /** Station identifier in Station List or Search Station List*/
    bool bQuickMix;   //Flag field for QuickMix/Delete/Shared
    QString StationLabel;    /** Station Name*/
    bool bShared;
} TSStationInfo;


/**
    * This struct will be used to provide the station art information to application
    */
typedef struct
{
    quint32  nStationToken;    /** Station identifier t*/
    QString StationArtPath;    /** Station Art Path*/
} TSStationArt;

/**
    * The data will contain each entry for search result
    */
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

/**
    * The enum will be used to identify the request made by application
    */
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
    REQ_TYPE_RATE_NONE= 19,    /* Update Track ratings none*/ //added  by esjang 2013.05.17
    REQ_TYPE_RATE_POS = 20,     /* Update Track ratings Positive*/
    REQ_TYPE_RATE_NEG = 21,      /* Update Track ratings Negative*/
//{ modified by yongkyun.lee 2014-08-25 for : Branding Image
    REQ_TYPE_BRANDING_IMAGE = 22,
    REQ_TYPE_BRANDING_SEGMENT = 23 
//} modified by yongkyun.lee 2014-08-04 
} ERequestType;

/**
    * Image type for Album Art
    */
typedef enum
{
    IMG_TYPE_NONE = 0x00,  /** No image type chosen */
    IMG_TYPE_JPEG,   /** Image in Jpeg format */
    IMG_TYPE_PNG,   /** Image in png format */
    IMG_TYPE_RGB565,   /** Image in RGB565 format */
} EImageType;

#endif//DHAVN_PNDR_DEFS_H
