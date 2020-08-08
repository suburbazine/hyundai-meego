/*
 ******************************************************************
 *        COPYRIGHT ?2011, LG Electronics.  
 *        All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :    Pandora Interface                              *
 * PROGRAMMER :    Vandana Ahuja                                  *
 * DATE       :    02/12/2011                                     *
 * VERSION    :    0.1                                            *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *        This module provides the interface to be used by
 *        Pandora application
 *        This is a very thin layer, which just check for the
 *        validity of input and check if requested operation is 
 *        possible in given state or not.
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS    
 * 1) 02/12/2011    0.1 Vandana Ahuja   header file created
 *
 ******************************************************************
 */


#ifndef DHAVN_PNDR_INTERFACE_H
#define DHAVN_PNDR_INTERFACE_H

#include <QObject>
#include <DHAVN_Pndr_Defs.h>
#include <DHAVN_Pndr_Mw_Global.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_PndrStateHandler.h>

//Forward Declarations

class DHAVN_PANDORA_MIDDLEWARE_SHARED_EXPORT CPndrInterface : public QObject
{
    Q_OBJECT

public:

    /**
     *******************************************************************
     * Constructor:
     *  Instanciating CPndrStateHandler Class
     *
     * @param      None
     *
     * @return     None
     *
     *******************************************************************
     */
    CPndrInterface();

    /**
     *******************************************************************
     * Destructor:
     *  Setting Member variable to NULL.

     * @param      None
     *
     * @return     None
     *
     *******************************************************************
     */

    ~CPndrInterface();

    /**
     *******************************************************************
     *  Init Function for memory allocation for statehandler object
     *
     *
     * @param      None
     *
     * @return     EPndrError: E_SUCCESS on Successful Memory Allocation
     *                         E_MEMORY on UnSuccessful Memory Alocation
     *
     *******************************************************************
     */

    EPndrError Init();
    /**
     *******************************************************************
     *  DeInit Function for releasing memory allocated by statehandler object
     *
     *
     * @param      None
     *
     * @return     None
     *
     *******************************************************************
     */

    void DeInit();

    /**
     *******************************************************************
     * This function will connect to device and initiate the pandora 
     * connection
     *
     *    TODO: Currently its not possible to get the device ID/name 
     *            by which it can be identified that which device to 
     *            connect if there are more than one device is 
     *            connected through usb
     *
     * @param[in]    inDeviceType: This can be iPhone or Smart Phone
     *                    By default if iPhone is EDeviceType then USB
     *                    will be tried for connection & if smart device 
     *                    is used as EDeviceType then BT will be tried 
     *                    for connection.
     *                  inAlbumArtDimension: The expected size of album art for
     *                      each music
     *                  inAlbumArtImgType: The expected image type of album 
     *                      art for each music, if its value is IMG_TYPE_NONE,
     *                      then default value as IMG_TYPE_JPEG will be used.
     *                  inStationArtDimension size of station art or genre station
     *                       art image
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is already connected
     *                                    the function has been called 
     *                                    incorrectly
     *
     * @see:    Result
     *******************************************************************
     */
    EPndrError Connect(const EDeviceType inDeviceType,
                       const quint16 inAlbumArtDimension,
                       const EImageType inAlbumArtImgType,
                       const quint16 inStationArtDimension);

    /**
     *******************************************************************
     * Disconnect any existing connection
     *
     * @param None
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is not connected
     *
     * @see:    Result
     *******************************************************************
     */
    EPndrError Disconnect();
    //{ added by esjang 2013.05.15 for certification issue, sessionterminate.
    /**
     *******************************************************************
     * SessionTerminate any existing connection
     *
     * @param None
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is not connected
     *
     * @see:    Result
     *******************************************************************
     */
    void SessionTerminate();
    //} added by esjang 2013.05.15 for certification issue, sessionterminate.

    //{ added by esjang 2013.07.01 for Session already started issue
    /**
     *******************************************************************
     * RequestPandoraStatus any existing connection
     *
     * @param None
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is not connected
     *
     * @see:    Result
     *******************************************************************
     */
    //void RequestPandoraStatus();
    //} added by esjang 2013.07.01 for Session already started issue

    /**
     *******************************************************************
     * This function will will provide the Information
     *                about current track being played/paused
     *
     * @param None
     *    
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is Not connected OR
     *                                    No track is played/paused
     *
     * @see:    TrackInfo
     *******************************************************************
     */
    EPndrError RequestTrackInfo();



    /**
     *******************************************************************
     * Request for explanation for currently played/paused
     *              song
     *
     * @param None
     *    
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is Not connected OR
     *                                    No track is played/paused
     *
     * @see:    TrackExplain
     *******************************************************************
     */
    EPndrError RequestExplanation();

    /**
     *******************************************************************
     * Request to Play the track. This will be possible ONLY
     *                if track was paused before
     *
     * @param None
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is Not connected OR
     *                                    Track was not paused before
     *
     * @see:    Result
     *******************************************************************
     */
    EPndrError Play();

    /**
     *******************************************************************
     * Request to Pause the current track being played.
     *
     * @param None
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is Not connected OR
     *                                    No track playing to be paused
     *
     * @see:    Result
     *******************************************************************
     */
    EPndrError Pause();

    /**
     *******************************************************************
     * Request to Skip the song
     *
     * @param None    
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is Not connected OR
     *                                    No track is in Play/Pause status
     *
     * @see:    TrackUpdated
     *******************************************************************
     */
    EPndrError Skip();

    //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
    EPndrError RequestBrandingImage(QString BIPath ,QString BIName );
    EPndrError RequestBrandingSegment(const quint8 SegIndex,const quint8 totalSeg, quint8* inData,  int fzise);
    //} modified by yongkyun.lee 2014-08-04 	
    /**
     *******************************************************************
     * Request to Skip the song
     *
     * @param[in]    inRate    The Rating that user would like to give to 
     *                        current track
     *                        1:    ThumbUp
     *                        2:    ThumbDown
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is Not connected OR
     *                                    No track is in Play/Pause status
     *
     * @see:    Result
     *******************************************************************
     */
    EPndrError Rate(const quint8 inRate);

    /**
     *******************************************************************
     * Request to Bookmark current Track
     *
     * @param None    
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is Not connected OR
     *                                    No track is in Play/Pause status
     *
     * @see:    Result
     *******************************************************************
     */
    EPndrError BookmarkSong();

    /**
     *******************************************************************
     * Bookmark Artist of current Track
     *
     * @param None    
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is Not connected OR
     *                                    No track is in Play/Pause status
     *
     * @see:    Result
     *******************************************************************
     */
    EPndrError BookmarkArtist();


    /**
     *******************************************************************
     * Request to Get the station list. 
     *

     * @param[in]    inSortType    What is expected order for station List
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is Not connected 
     *
     * @see:    StationTokens
     *******************************************************************
     */
    EPndrError RequestStationSort(const ESortType inSortType);

    /**
     *******************************************************************
     * Request station sort order
     *
     * @param[in]    None
     *
     * @return     None
     *
     *******************************************************************
     */
    EPndrError RequestStationSortOrder();

    /**
     *******************************************************************
     * Request station count
     *
     * @param[in]    None
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Invalid State of mw
     *
     *******************************************************************
     */
    EPndrError RequestStationCount();


    /**
     *******************************************************************
     * Request to Get the station list. 
     *
     * @param[in]    inStartIndex    Which is the start index for getting
     *                      the station list
     *                  inEndIndex      Which is the end index for getting
     *                      the station list
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is Not connected 
     *                E_INVALID_STATION_REQ_RANGE: Invalid range
     *
     * @see:    StationInfoList
     *******************************************************************
     */
    EPndrError RequestStationList(const qint8 inStartIndex,
                                  const qint8 inEndIndex);

    /**
     *******************************************************************
     * Request to select a station for start playing
     *
     * @param[in]    inStationToken    Token station which is selected 
     *                                    to be played
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is NOT connected OR
     *                                    No station list was requested 
     *                                    before 
     *                E_INVALID_TOKEN: Station Token is not valid
     *
     * @see:    Result
     *******************************************************************
     */
    EPndrError RequestStationSelect(const quint32 inStationToken);

    /**
     *******************************************************************
     * Request for auto search complete given a word
     *
     * @param[in]    inWordToSearch    The word to be searched
     *                                    max length: 248
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is NOT connected OR
     *                                    No search was requested before
     *                E_INVALID_STRING: Either string is empty or more than
     *                                    248 characters
     *
     * @see:    SearchComplete
     *******************************************************************
     */
    EPndrError RequestSearchAutoComplete(const QString& inWordToSearch);

    /**
     *******************************************************************
     * Request for extended search for a given word
     *
     * @param[in]    inWordToSearch    The word to be searched
     *                                    max length: 248
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is NOT connected OR
     *                                    No search was requested before
     *                E_INVALID_STRING: Either string is empty or more than
     *                                    248 characters
     *
     * @see:    SearchComplete
     *******************************************************************
     */
    EPndrError RequestSearch(const QString& inWordToSearch);

    /**
     *******************************************************************
     * Request to select the item from search
     *
     * @param[in]    inToken    The token to identify the track        
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is NOT connected OR
     *                                    No search was completed
     *                                    before 
     *                E_INVALID_TOKEN: Token is not valid
     *
     * @see:    TrackUpdated
     *******************************************************************
     */
    EPndrError RequestSearchSelect(const quint32 inToken);

    /**
     *******************************************************************
     * Provide the state handler ref which needs to
     *              be connected to for listening to signal
     *
     * @param None    
     *
     * @return     CPndrStateHandler&: object reference to through
     *                  which signals can be listened to by app
     *
     *******************************************************************
     */
    CPndrStateHandler& GetSignalProvider();

    /**
     *******************************************************************
     * Cancels any outstanding request of station Art when user goes back
     * from List view
     *
     * @return  void
     *
     *******************************************************************
     */
     void CancelStationArtRequest();

     /**
      *******************************************************************
      * Request to set the elapsed time polling
      * @param  true elapsed time polling is enabled
      *         false elapsed time polling is disabled
      * @return  void
      *
      *******************************************************************
      */
      void SetTrackElapsedPolling(bool inEnabled);

      /**
       *******************************************************************
       * Returns the current sort order
       *
       * @return  ESortType the current sort type
       *
       *******************************************************************
       */
      ESortType CurrentSortOrder();

      /**
       *******************************************************************
       * Fetch station Art when user comes to Station List       *
       *
       * @return  void
       *
       *******************************************************************
       */
       void FetchStationArt();

      //{ added by cheolhwan 2014-04-09.
      /**
       *******************************************************************
       * Returns the Primary state of Pandora
       *
       * @return  EPriState
       *
       *******************************************************************
       */
       EPriState GetPriState();

      /**
       *******************************************************************
       * Returns the Primary state of Pandora
       *
       * @return  ESecState
       *
       *******************************************************************
       */
       ESecState GetSecState();

      /**
       *******************************************************************
       * Returns the Previous Secondary state of Pandora
       *
       * @return  ESecState
       *
       *******************************************************************
       */
       ESecState GetSecPreviousState();
      //} added by cheolhwan 2014-04-09.

	  //{ added by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
	  /**
	   *******************************************************************
	   * Request to set the prefetching stations flag.
	   * @param  true prefetching stations is enabled
	   *		 false prefetching stations is disabled
	   * @return  void
	   *
	   *******************************************************************
	   */
	  void setPreFetchingStations(bool inPreFetchingStations);
	  //} added by cheolhwan 2014-04-17. To fix the unlimited receiving station.

private:
    CPndrStateHandler   m_CPndrStateHandler;
    USE_LOG_UTIL;
};

#endif //DHAVN_PNDR_INTERFACE_H
