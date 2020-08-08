/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :       Pndr_Constants                              *
 *
 * PROGRAMMER :       Snigdha Satapathy					*
 * DATE       :       28-November-2011                            *
 * VERSION    :       01                                          *
 *
 *----------------------------------------------------------------*
 * 
 * MODULE SUMMARY :   This header defines all the constants that  *
 * are to be used by the Pandora middleware                       *
 * 
 * This has been referenced from the Pandora-link-1.04 document   *
 *
 * The empty/unused structs and enums have been removed and only  *
 * those constants exposed by Pandora have been kept in this      *
 * document that are to be used by the Pandora middleware.        *
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *
 ******************************************************************
 */


#ifndef __PNDR_LINK_CONSTANTS_H__
#define __PNDR_LINK_CONSTANTS_H__

#include <stdint.h>

/**
* The valid value of api_version
* Used by Connect command while initiating a session between
* Accessory and Mobile App
*/

typedef uint16_t pndr_api_version;

//#define PNDR_API_VERSION_CURRENT ((pndr_api_version)0x01)
#define PNDR_API_VERSION_CURRENT ((pndr_api_version)0x03)

/**
* The boolean values used by Pandora
*/

typedef uint8_t pndr_bool;
#define PNDR_FALSE ((pndr_bool)0x00)
#define PNDR_TRUE ((pndr_bool)0x01)

/**
* The type of image used as album art
*/

typedef uint8_t pndr_image_type;
#define PNDR_IMAGE_NONE ((pndr_image_type)0x00)
#define PNDR_IMAGE_JPEG ((pndr_image_type)0x01)
#define PNDR_IMAGE_PNG ((pndr_image_type)0x02)
#define PNDR_IMAGE_RGB565 ((pndr_image_type)0x03)

/**
* It defines the type of search results
* whether it is Search by artist or song
*/

typedef uint8_t pndr_seed_type;
#define PNDR_SEED_ARTIST ((pndr_seed_type)0x00)
#define PNDR_SEED_SONG ((pndr_seed_type)0x01)
#define PNDR_SEED_OTHER ((pndr_seed_type)0x02)

/**
* It defines the type of sorting used for sorting stations
* whether it is Sort by date or name
*/

typedef uint8_t pndr_sort_type;
#define PNDR_SORT_BY_DATE ((pndr_sort_type)0x00)
#define PNDR_SORT_BY_NAME ((pndr_sort_type)0x01)

/**
* The rating of a track
*/

typedef uint8_t pndr_rating_type;
#define PNDR_RATING_NONE ((pndr_rating_type)0x00)
#define PNDR_RATING_POSITIVE ((pndr_rating_type)0x01)
#define PNDR_RATING_NEGATIVE ((pndr_rating_type)0x02)

typedef int8_t pndr_notice_code;

/**
 * An info note to notify the user that the
 * maximun skip limit has been reached
 * Free Pandora accounts permit 6 skips per hour per station, 
 * for up to 12 total skips per day across all station
 */
#define PNDR_NOTICE_SKIP_LIMIT_REACHED ((pndr_notice_code)0x00)

/**
 * An info note to notify the user that the
 * maximun no. of stations he can request has been reached
 * The maximum station limit for any user is 101
 */
#define PNDR_NOTICE_STATION_LIMIT_REACHED ((pndr_notice_code)0x01)

/**
 * An error note to notify the user that thumbs up /down 
 * failed with RPC network error
 */
#define PNDR_NOTICE_ERROR_TRACK_RATING ((pndr_notice_code)0x02)

/**
 * An error note to notify the user that station has been deleted
 */
#define PNDR_NOTICE_ERROR_STATION_DELETE ((pndr_notice_code)0x03)

/**
 * An error note to notify the user that there was an error 
 * while performing extended search
 */
#define PNDR_NOTICE_ERROR_SEARCH_EXTENDED ((pndr_notice_code)0x04)

/**
 * An error note to notify the user that there was an error
 * while creating a station from search results
 */
#define PNDR_NOTICE_ERROR_SEARCH_SELECT ((pndr_notice_code)0x05)

/**
 * An error note to notify the user that there was an error
 * while trying to bookmark a song or an artist
 */
#define PNDR_NOTICE_ERROR_BOOKMARK ((pndr_notice_code)0x06)

/**
 * An error note to notify the user that there was an error
 * in maintenance
 */
#define PNDR_NOTICE_ERROR_MAINTENANCE ((pndr_notice_code)0x07)

/**
 * An error note to notify the user that there was an error
 * in track explain
 */
#define PNDR_NOTICE_ERROR_TRACK_EXPLAIN ((pndr_notice_code)0x08)
/**
 * An error note to notify the user that a session was already
 * started
 */
#define PNDR_NOTICE_ERROR_SESSION_ALREADY_STARTED ((pndr_notice_code)0x09)
/**
 * An error note to notify the user that there was no active
 * session
 */
#define PNDR_NOTICE_ERROR_NO_ACTIVE_SESSION ((pndr_notice_code)0x0A)
/**
 * An error note to notify the user that
 * an unsupported URL scheme is used, or if the target app is not installed
 */
#define PNDR_NOTICE_ERROR_APP_URL_NOT_SUPPORTED ((pndr_notice_code)0x0B)

/**
 * An error note to notify the user that the requested station
 * does not exist
 */
#define PNDR_NOTICE_ERROR_STATION_DOES_NOT_EXIST ((pndr_notice_code)0x0C)


/**
* It defines the status of the Pandora App
*/

typedef uint8_t pndr_status_code;
#define PNDR_STATUS_PLAYING ((pndr_status_code)0x01)
#define PNDR_STATUS_PAUSED ((pndr_status_code)0x02)
#define PNDR_STATUS_INCOMPATIBLE_API_VERSION ((pndr_status_code)0x03)
#define PNDR_STATUS_SEE_DEVICE ((pndr_status_code)0x04)
#define PNDR_STATUS_NO_STATIONS ((pndr_status_code)0x05)
#define PNDR_STATUS_NO_STATION_ACTIVE ((pndr_status_code)0x06)

#define PNDR_STATUS_INSUFFICIENT_CONNECTIVITY ((pndr_status_code)0x07)
#define PNDR_STATUS_LICENSING_RESTRICTIONS ((pndr_status_code)0x08)
#define PNDR_STATUS_INVALID_LOGIN ((pndr_status_code)0x09)

/**
* The command codes used for communication between
* the Accessory and Mobile App
*/

typedef  uint8_t    pndr_command_code;

/**
* An integer value used for uniquely identifying
* a station/track/search-id
*/

typedef  uint32_t    pndr_token;
static const pndr_token PNDR_TRACK_NONE = 0x00000000;
static const pndr_token PNDR_STATION_NONE = 0x00000000;

/**
* A char array used to define the name of
* a station/track/album/artist/search input/seed description etc.
*/
typedef  char    pndr_label[248];


/**
* Station flags
*/

#define PNDR_STATION_FLAG_ALLOW_DELETE ((uint8_t)0x1)
#define PNDR_STATION_FLAG_IS_SHARED ((uint8_t)0x2)
#define PNDR_STATION_FLAG_IS_QUICKMIX ((uint8_t)0x4)


//Accessory to Mobile App:

/**
* The command used to connect Accessory with Mobile App
*/

//#define PNDR_CONNECT ((pndr_command_code)0x00)
#define PNDR_SESSION_START ((pndr_command_code)0x00)

/**
* The different flags used with Connect to simulate error scenarios
*/
/*
#define PNDR_CONNECT_FLAG_SIMULATE_BAD_CONNECTION ((uint16_t)0x01)
#define PNDR_CONNECT_FLAG_PAUSE_ON_CONNECT ((uint16_t)0x02)
#define PNDR_CONNECT_FLAG_SIMULATE_RANDOM_ERRORS ((uint16_t)0x04)
*/
#define PNDR_SESSION_FLAG_SIMULATE_BAD_CONNECTION ((uint16_t)0x01)
#define PNDR_SESSION_FLAG_PAUSE_ON_CONNECT ((uint16_t)0x02)
#define PNDR_SESSION_FLAG_SIMULATE_RANDOM_ERRORS ((uint16_t)0x04)


/**
* The command used to terminate session of Accessory with Mobile App
*/
#define PNDR_SESSION_TERMINATE ((pndr_command_code)0x05)


/**
* The command used to inform the Mobile App that the
* Accessory has a branding image for display on the Mobile App while connected
*/

#define PNDR_UPDATE_BRANDING_IMAGE ((pndr_command_code)0x01)

/**
* The command used by Accessory to return the branding image segments
*/

#define PNDR_RETURN_BRANDING_IMAGE_SEGMENT ((pndr_command_code)0x02)

/**
* The command used to request the current play status
*/

#define PNDR_GET_STATUS ((pndr_command_code)0x03)

/**
* This command requests the track token, album art length, flags, duration,
* elapsed play time in seconds, and rating for the current track to be returned
*/

#define PNDR_GET_TRACK_INFO ((pndr_command_code)0x10)

/**
* This command requests the title of the currently playing track to be returned
*/

#define PNDR_GET_TRACK_TITLE ((pndr_command_code)0x11)

/**
* This command requests the artist for the currently playing track to be returned
*/

#define PNDR_GET_TRACK_ARTIST ((pndr_command_code)0x12)

/**
* This command requests the album for the currenty playing track to be returned
*/

#define PNDR_GET_TRACK_ALBUM ((pndr_command_code)0x13)

/**
* This command requests the current album art to be returned
* via one or more consecutive PNDR_RETURN_TRACK_ALBUM_ART_SEGMENT commands.
*/

#define PNDR_GET_TRACK_ALBUM_ART ((pndr_command_code)0x14)

/**
* This command instructs the Mobile App to enable or disable automatic elapsed time updates
*/

#define PNDR_SET_TRACK_ELAPSED_POLLING ((pndr_command_code)0x15)


/**
* This command instructs the Mobile App to give extended track info
*/

#define PNDR_GET_TRACK_INFO_EXTENDED ((pndr_command_code)0x16)

/**
* This command instructs the Mobile App to set the status to PDNR_STATUS_PLAYING
*/

#define PNDR_EVENT_TRACK_PLAY ((pndr_command_code)0x30)

/**
* This command instructs the Mobile App to set the status to PDNR_STATUS_PAUSE.
*/

#define PNDR_EVENT_TRACK_PAUSE ((pndr_command_code)0x31)

/**
* This command instructs the Mobile App to skip to the next audio track
*/

#define PNDR_EVENT_TRACK_SKIP ((pndr_command_code)0x32)

/**
* This command instructs the Mobile App to give the current track a "thumbs up."
*/

#define PNDR_EVENT_TRACK_RATE_POSITIVE ((pndr_command_code)0x33)

/**
* This command instructs the Mobile App to give the current track a "thumbs down."
*/

#define PNDR_EVENT_TRACK_RATE_NEGATIVE ((pndr_command_code)0x34)


/**
* This command instructs the Mobile App to send informative text regarding why this track was chosen.
*/

#define PNDR_EVENT_TRACK_EXPLAIN ((pndr_command_code)0x35)

/**
* This command requests track explanation to be returned via one or more consecutive
* PNDR_RETURN_TRACK_EXPLAIN_SEGMENT commands
*/

#define PNDR_GET_TRACK_EXPLAIN ((pndr_command_code)0x36)

/**
* This command instructs the Mobile App to bookmark the current track
*/

#define PNDR_EVENT_TRACK_BOOKMARK_SONG ((pndr_command_code)0x37)

/**
* This command instructs the Mobile App to bookmark the current artist
*/

#define PNDR_EVENT_TRACK_BOOKMARK_ARTIST ((pndr_command_code)0x38)

/**
* This command requests the currently playing station token to be returned via PNDR_RETURN_STATION_ACTIVE
*/

#define PNDR_GET_STATION_ACTIVE ((pndr_command_code)0x40)

/**
* This command requests number of stations for the current user to be returned via PNDR_RETURN_STATION_COUNT
*/

#define PNDR_GET_STATION_COUNT ((pndr_command_code)0x41)

/**
* This command instructs the Mobile App to send the specified number of user's station tokens
* starting from the specified index to be returned via PNDR_RETURN_STATION_TOKENS.
*/

#define PNDR_GET_STATION_TOKENS ((pndr_command_code)0x42)

/**
* This command requests an array of all of the user's station tokens to be returned via PNDR_RETURN_STATION_TOKENS
*/

#define PNDR_GET_ALL_STATION_TOKENS ((pndr_command_code)0x43)

/**
* This command requests the name and flags for each station token
* in an array of station_tokens to be returned via PNDR_RETURN_STATION_INFO.
*/

#define PNDR_GET_STATION_INFO ((pndr_command_code)0x44)

/**
* This command requests the sort order for the stations to be returned via PNDR_RETURN_STATIONS_ORDER
*/

#define PNDR_GET_STATIONS_ORDER ((pndr_command_code)0x45)

/**
* This command instructs the Mobile App to sort stations in the given order
*/

#define PNDR_EVENT_STATIONS_SORT ((pndr_command_code)0x46)

/**
* This command instructs the Mobile App to begin playing the given station.
*/

#define PNDR_EVENT_STATION_SELECT ((pndr_command_code)0x47)


/**
* This command instructs the Mobile App to send the station art for the given stations.
*/

#define PNDR_GET_STATION_ART ((pndr_command_code)0x4B)


/**
* This command instructs the Mobile App to cancel the last request for station art
*/

#define PNDR_EVENT_CANCEL_STATION_ART ((pndr_command_code)0x4C)


/**
* This command requests auto-complete results for the given search text.
*/

#define PNDR_EVENT_SEARCH_AUTO_COMPLETE ((pndr_command_code)0x60)

/**
* This command requests extended search results for the given search text to the accessory.
*/

#define PNDR_EVENT_SEARCH_EXTENDED ((pndr_command_code)0x61)

/**
* This command requests the result info for each music token in the given array to be returned via
* PNDR_RETURN_SEARCH_RESULT_INFO
*/

#define PNDR_GET_SEARCH_RESULT_INFO ((pndr_command_code)0x62)

/**
* This command requests station creation based on a selection from a previously returned search results
*/

#define PNDR_EVENT_SEARCH_SELECT ((pndr_command_code)0x63)

/**
* This command indicates to the Mobile App that the user did not select a station
* from the given search results, and the search can be discarded
*/

#define PNDR_SEARCH_DISCARD ((pndr_command_code)0x64)


//Mobile App to Accessory:


/**
* This command requests the branding image to be returned via one or more consecutive
* PNDR_RETURN_TRACK_BRANDING_IMAGE_SEGMENT commands
*/

#define PNDR_GET_BRANDING_IMAGE ((pndr_command_code)0x80)

/**
* This command informs the Accessory of the current play status of the Mobile App
*/

#define PNDR_UPDATE_STATUS ((pndr_command_code)0x81)

/**
* This command Sends the current play status as requested via PNDR_GET_STATUS
*/

#define PNDR_RETURN_STATUS ((pndr_command_code)0x82)

/**
* This command instructs the accessory to present a notice or an alert
* The notice code corresponds to the type of error note that is to be seen
*/

#define PNDR_UPDATE_NOTICE ((pndr_command_code)0x83)

/**
* This command Informs the accessory that the currently playing track has changed
* and needs to be updated in the accessory UI.
*/

#define PNDR_UPDATE_TRACK ((pndr_command_code)0x90)

/**
* This command returns the currently playing track token, album art length, flags, duration,
* elapsed play time in seconds, and rating as requested by PNDR_GET_TRACK_INFO.
*/

#define PNDR_RETURN_TRACK_INFO ((pndr_command_code)0x91)

/** The flags returned as part of track info */
#define PNDR_TRACK_FLAG_ALLOW_RATING ((uint8_t)0x01)
#define PNDR_TRACK_FLAG_ALLOW_SKIP ((uint8_t)0x02)
#define PNDR_TRACK_FLAG_ALLOW_BOOKMARK ((uint8_t)0x04)
#define PNDR_TRACK_FLAG_ALLOW_EXPLAIN ((uint8_t)0x08)

#define PNDR_TRACK_FLAG_IS_AD ((uint8_t)0x01)
#define PNDR_TRACK_FLAG_IS_TRACK_BOOKMARKED ((uint8_t)0x02)
#define PNDR_TRACK_FLAG_IS_ARTIST_BOOKMARKED ((uint8_t)0x04)


/**
* This command returns the current track's title as requested by PNDR_GET_TRACK_TITLE
*/

#define PNDR_RETURN_TRACK_TITLE ((pndr_command_code)0x92)

/**
* This command returns the current track's artist as requested by PNDR_GET_TRACK_ARTIST
*/

#define PNDR_RETURN_TRACK_ARTIST ((pndr_command_code)0x93)

/**
* This command returns the current track's album as requested by PNDR_GET_TRACK_ALBUM
*/

#define PNDR_RETURN_TRACK_ALBUM ((pndr_command_code)0x94)

/**
* This command returns a segment of the album art image
* The parameters include the track token, the current segment index, total no. of segments and the image data
*/

#define PNDR_RETURN_TRACK_ALBUM_ART_SEGMENT ((pndr_command_code)0x95)

/**
* This command informs the accessory that the current track's album art has loaded
*/

#define PNDR_UPDATE_TRACK_ALBUM_ART ((pndr_command_code)0x96)

/**
* This command informs the accessory that the elapsed play time of the current track has changed
*/

#define PNDR_UPDATE_TRACK_ELAPSED ((pndr_command_code)0x97)

/**
* This command informs the accessory that the rating on the current track has changed
*/

#define PNDR_UPDATE_TRACK_RATING ((pndr_command_code)0x98)

/**
* This command Informs the accessory that the explain text for the current track
* has been loaded and is ready for display
*/

#define PNDR_UPDATE_TRACK_EXPLAIN ((pndr_command_code)0x99)

/**
* This command returns a segment of the track explanation
* The parameters include the track token, the current segment index,
* total no. of segments and the track info data
*/

#define PNDR_RETURN_TRACK_EXPLAIN_SEGMENT ((pndr_command_code)0x9a)

/**
* This command informs the accessory whether or not the current track's artist is bookmarked
*/

#define PNDR_UPDATE_TRACK_BOOKMARK_SONG ((pndr_command_code)0x9b)

/**
* This command informs the accessory whether or not the current track's artist is bookmarked
*/

#define PNDR_UPDATE_TRACK_BOOKMARK_ARTIST ((pndr_command_code)0x9c)

/**
* This command returns the extended track info
*/

#define PNDR_RETURN_TRACK_INFO_EXTENDED ((pndr_command_code)0x9d)

/**
* This command updates the track completed
*/

#define PNDR_UPDATE_TRACK_COMPLETED ((pndr_command_code)0x9e)


/**
* This command informs the accessory that the currently playing station has changed.
*/

#define PNDR_UPDATE_STATION_ACTIVE ((pndr_command_code)0xba)

/**
* This command returns the currently playing station as requested by PNDR_GET_STATION_ACTIVE
*/

#define PNDR_RETURN_STATION_ACTIVE ((pndr_command_code)0xb1)

/**
* This command returns the number of stations for this user as requested by PNDR_GET_STATION_COUNT
*/

#define PNDR_RETURN_STATION_COUNT ((pndr_command_code)0xb2)

/**
* This command returns an array of station tokens as requested by PNDR_GET_STATION_TOKENS or
* PNDR_GET_ALL_STATION_TOKENS.
*/

#define PNDR_RETURN_STATION_TOKENS ((pndr_command_code)0xb3)

/**
* This command returns an array of the name and flags for specified stations
* as requested by PNDR_GET_STATION_INFO
*/

#define PNDR_RETURN_STATION_INFO ((pndr_command_code)0xb4)

/**
* This command returns the station sort order as requested by PNDR_GET_STATIONS_ORDER
*/

#define PNDR_RETURN_STATIONS_ORDER ((pndr_command_code)0xb5)

/**
* This command informs the accessory that the ordering of stations has changed
*/

#define PNDR_UPDATE_STATIONS_ORDER ((pndr_command_code)0xb6)

/**
* This command informs the accessory that the station has been deleted
*/

#define PNDR_UPDATE_STATION_DELETED ((pndr_command_code)0xb7)

/**
* This command returns a segment of the station art
*/

#define PNDR_RETURN_STATION_ART_SEGMENT ((pndr_command_code)0xb8)

/**
* This command notifies the accessory of search results that match the given search id.
*/

#define PNDR_UPDATE_SEARCH ((pndr_command_code)0xd0)

/**
* This command returns search result data for one or
* more results as requested by PNDR_GET_SEARCH_RESULT_INFO
*/

#define PNDR_RETURN_SEARCH_RESULT_INFO ((pndr_command_code)0xd1)

/**
* This command informs the accessory that a new station has been added to this user's station list
*/

#define PNDR_UPDATE_STATION_ADDED ((pndr_command_code)0xd2)

/**
* This command can be used for sending a request for debugging purpose
*/

#define PNDR_ECHO_REQUEST ((pndr_command_code)0x7f)

/**
* This command can be used for sending a response for debugging purpose
*/

#define PNDR_ECHO_RESPONSE ((pndr_command_code)0xff)

/**
* The Frame Start,End and Escape bytes
*/

#define PNDR_FRAME_START  ((uint) 0x7E)
#define PNDR_FRAME_END  ((uint) 0x7C)
#define PNDR_FRAME_ESCAPE ((uint) 0x7D)


/**
* The Frame Type,i.e, DATA or ACK
*/
#define PNDR_FRAME_TYPE_DATA  ((uint) 0x00)
#define PNDR_FRAME_TYPE_ACK  ((uint) 0x01)


/**
* The sequence no. ,i.e, 0 for data frame and 1 for ACK frame
*/
#define PNDR_FRAME_SEQUENCE_0  ((uint) 0x00)
#define PNDR_FRAME_SEQUENCE_1  ((uint) 0x01)


/**
* The escape characters
*/
#define PNDR_FRAME_ESCAPE_START ((uint) 0x5E)
#define PNDR_FRAME_ESCAPE_END ((uint) 0x5C)
#define PNDR_FRAME_ESCAPE_ESCAPE ((uint) 0x5D)


#endif //__PNDR_LINK_CONSTANTS_H__



