/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.  
 *        All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :    Pandora internal definitions                   *
 * PROGRAMMER :    Vandana Ahuja                                  *
 * DATE       :    02/12/2011                                     *
 * VERSION    :    0.1                                            *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module defines common structure/enums/const definitions
 *  used by middleware ONLY
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS    
 * 1) 03/12/2011    0.1 Vandana Ahuja   header file created
 *
 ******************************************************************
 */

#ifndef DHAVN_PNDR_INTERNAL_DEFS_H
#define DHAVN_PNDR_INTERNAL_DEFS_H


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
    quint8     nRating;        /** 1 for Thumb up, 2 for Thumb Down, 0 for no rating*/
    bool    bAllowRating;    /** Is it allowed to rate this track */
    bool    bAllowSkip;        /** Is it allowed to skip this track */
    bool    bAllowBookmark;    /** Is it allowed to bookmark this track */
    bool    bAllowExplain;    /** Is it allowed to explain this track */
} TSPartTrackInfo;

/**
    * The enum will be used to identify the request made by application
    */
typedef enum
{
    EVENT_NONE = 0,        /** No request. Default value */
    EVENT_CONNECT = 1,    /** Connect request */
    EVENT_PART_TRACK_INFO = 2,    /** Track info request */
    EVENT_PLAY = 3,        /** Play request */
    EVENT_PAUSE = 4,        /** Pause request */
    EVENT_SKIP = 5,        /** Skip request */
    EVENT_RATE_POS = 6,        /** Rate request */
    EVENT_RATE_NEG = 7,        /** Rate request */
    EVENT_BOOKMARK_SONG = 8,    /** Bookmark song request */
    EVENT_BOOKMARK_ARTIST = 9,    /** Bookmark Artist request */
    EVENT_STATION_INFO = 10,        /** Station List request */
    EVENT_STATION_SELECT = 11,    /** Station Select request */
    EVENT_SEARCH = 12,            /** Extended Search request */
    EVENT_SEARCH_AUTO_COMPLETE = 13,   /** Auto complete search request */
    EVENT_SEARCH_SELECT = 14,        /** Search Select request */
    EVENT_DISCONNECT = 15,            /** Disconnected request */
    EVENT_TITLE = 16,  /** Title Request */
    EVENT_ARTIST = 17, /** Artist Request */
    EVENT_ALBUM = 18,  /** Album Request */
    EVENT_ALBUM_ART = 19,    /** To get Album Art, 1st need to send event*/
    EVENT_GET_ALBUM_ART = 20,  /** Album Art Request*/
    EVENT_SET_ELAPSED_POLLING = 21,    /** Request to send the elapsed time*/
    EVENT_EXPLAIN = 22,  /** Event for requesting explain info */
    EVENT_GET_EXPLAIN = 23,    /** Request for Explanation data*/
    EVENT_STATION_COUNT = 24,  /** Request to get station count */
    EVENT_STATION_TOKENS = 25, /** Request to get all station tokens*/
    EVENT_SORT = 26,  /** Request for sort */
    EVENT_SORT_ORDER =  27, /** Request For Sort Order */
    EVENT_STATION_ACTIVE = 28,  /** Request to get the active station */
    EVENT_STATION_DELETE = 29,  /** Request to delete a station */
    EVENT_SEARCH_RESULT_INFO = 30,   /** Request to get the info for each token */
    EVENT_TRACK_UPDATED = 31,   /** Track Updated event */
    EVENT_STATION_ADDED = 32,   /** Station Added event */
    EVENT_UNKNOWN = 33,             /** Some unknown event has been received */
    EVENT_GET_STATION_ART = 34,         /** station Art Request */
    EVENT_EXTENDED_TRACK_INFO = 35,    /** extended Track Info */
    EVENT_UPDATE_RATE = 36,            /* update track rating */
	EVENT_TRACK_COMPLETED = 37   /** Station Added event */
	, EVENT_PANDORA_STATUS = 38 // added by esjang 2013.07.02 for session already stared
	, EVENT_PANDORA_ACTIVE_STATION_TOKEN = 39 //added by esjang 2013.07.01 for session already started
}  EPndrEvent;

/**
    * The enum will be used to identify the byte positions in the frame
    */
typedef enum
{
  eStartBytePos = 0,
  eTypeBytePos,
  eSeqNoBytePos,
  ePayloadLenBytePos1,
  ePayloadLenBytePos2,
  ePayloadLenBytePos3,
  ePayloadLenBytePos4,
  ePayloadStartBytePos
}EFrameBytePos;

#endif //DHAVN_PNDR_INTERNAL_DEFS_H
