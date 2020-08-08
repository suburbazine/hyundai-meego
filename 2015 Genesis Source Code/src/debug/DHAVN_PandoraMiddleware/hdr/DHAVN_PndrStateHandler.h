/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :   Pandora Middleware state handler                *
 * PROGRAMMER :   Vandana Ahuja                                   *
 * DATE       :   03/12/2011                                      *
 * VERSION    :   0.1                                             *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module is responsible for handling the state machine for
 *  Pandora middleware
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 * 03/12/2011   0.1 Vandana Ahuja   header file created
 ******************************************************************
 */


#ifndef DHAVN_PNDR_STATE_HANDLER_H
#define DHAVN_PNDR_STATE_HANDLER_H

#include <QObject>
#include <QTimer>
#include <DHAVN_LogUtil.h>
#include "DHAVN_Pndr_Internal_Defs.h"
#include "DHAVN_Pndr_Defs.h"

//Forward Declarations
class CPndrDeviceManager;
class CPndrStationController;
class CPndrTrackController;
class CPndrTrackProperties;
class CPndrFrameEncoder;
class CPndrFrameDecoder;
class CPndrFrameTxRx;
class CPndrFrameValidator;
class CPndrCache;

/**
* This enumaration will be used for maintaining the primary sstate
* of pandora middleware
*/
typedef enum
{
	PRI_STATE_NONE = 0, /** Pandora is not initialized */
	PRI_STATE_INIT = 1, /** connect Pandora Initialization is complete */
	PRI_STATE_DEVICE_CONNECTED = 2 , /** Device is connected */
	PRI_STATE_PANDORA_CONNECTED = 3, /** Pandora is connected with device */
	PRI_STATE_PLAY_REQUESTED = 4, /** Play is requested, this state will be set
							  when station is selected or after pause play is selected */
	PRI_STATE_PLAYING = 5, /** Pandora Radio is playing */
	PRI_STATE_PAUSE_REQUESTED = 6,	/** Pause is requested */
	PRI_STATE_PAUSED = 7 /** Pandora Radio has been paused */
} EPriState;

/** 
* This enumaration will maintain secondary state for state handler
* This is required as during search or listing station also radio
* might be playing
*/
typedef enum
{
	SEC_STATE_NONE = 0, /** Request Completed*/
	SEC_STATE_STATION_LIST_REQUESTED = 1,	/** Station list requested*/
	SEC_STATE_CURRENT_STATION_INFO_REQUESTED = 2,	/** Active Station info requested*/
	SEC_STATE_STATION_SELECT_REQUESTED = 3,  /** Station Select requested */
	SEC_STATE_AUTO_COMPLETE_SEARCH_REQUESTED = 4,	/** Auto complete search requested */
	SEC_STATE_SEARCH_REQUESTED = 5, /** Search requested */
	SEC_STATE_SEARCH_SELECT_REQUESTED = 6,	/** Search select requested */
	SEC_STATE_THUMB_UP_REQUESTED = 7,	/** Thumbup requested */
	SEC_STATE_THUMB_DOWN_REQUESTED = 8, /** Thumb Down requested */
	SEC_STATE_BOOKMARK_SONG_REQUESTED = 9,	/** Bookmark Song requested */
	SEC_STATE_BOOKMARK_ARTIST_REQUESTED = 10,	 /** Bookmark Artist requested */
	SEC_STATE_TRACK_INFO_REQUESTED = 11, /** Track info requested */
	SEC_STATE_TRACK_EXPLAIN_REQUESTED = 12, /** Track explain requested */
	SEC_STATE_SKIP_REQUESTED = 13,	  /** Skip Requested*/
	SEC_STATE_STATION_DELETE_REQUESTED = 14,
        //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
	SEC_STATE_BRANDING_IMAGE_REQUESTED = 15,	
	SEC_STATE_BRANDING_SEGMENT_REQUESTED = 16
        //} modified by yongkyun.lee 2014-08-04 
} ESecState;

/**
* Some requestes are accomplished by multiple request to server. 
* Therefore Following states are required for maintaining each request completion.
*/


/**
* This class will maintain the state machine for Pandora middleware
*/
class CPndrStateHandler :  public QObject
{
    Q_OBJECT
	USE_LOG_UTIL	
	
	public:
    /**
    * Track Info request sub state
    */
    typedef enum
    {
        TRACK_INFO_NONE = 0,    /** Track info request is completed or not yet started */
        TRACK_INFO_TRK_INFO = 2,    /** Extended track info as provided by server recieved */
        TRACK_STATION_INFO = 4,
        TRACK_INFO_ALL_RECEIVED = 6 // 126/** All state received */

    } ETrackInfoState;


    typedef enum
    {
        STATE_NONE = 0x00,     /** No Request */
        SESSION_ALREADY_STARTED = 0x02,
        GET_STATUS = 0x04,    /** Track status on device */
        GET_ACTIVE_STATION = 0x08,    /** Active station token */
        GET_TRACK_INFO = 0x16  /** Track info requested */
    } EStatusOnAlreadyStartedSession;


Q_SIGNALS:
        /**
         *******************************************************************
         * This signal will be emitted for providing the TrackInfo
         *
         * @param[in] inTrackInfo: The structure containing the current
         *                                track information
         *
         * @return     None
         *******************************************************************
         */
         void TrackInfo(const TSTrackInfo& inTrackInfo);


        /**
         *******************************************************************
         * This signal will be emitted for providing the Track explanation
         *
         * @param[in] inTrackExplain: Track explanation string
         *
         * @return     None
         *******************************************************************
         */
         void TrackExplain(const QString& inTrackExplain);

         /**
          *******************************************************************
          * This signal will be emitted for providing the Track explanation
          *
          * @param[in] inTrackExplain: Track explanation string
          *
          * @return     None
          *******************************************************************
          */
          void AlbumArt(const QString& inAlbumArt);

        /**
         *******************************************************************
         * General reply for request where reply doesn't need any parameter.
         * Also if any request faileddue to device disconnect or network
         * connection failed
         *
         * @param[in] inRequestType: The Request which was made
         *            inPndrError:    The error if any
         *                    E_SUCCESS: If request was successful
         *                    E_NETWORK_FAILED: If network connection failed
         *
         * @return     None
         *******************************************************************
         */
         void Result(const ERequestType inRequestType, const EPndrError inPndrError);


         /**
          *******************************************************************
          * Session terminated signal.
          * Also if any request faileddue to device disconnect or network
          * connection failed
          *
          * @param[in] inRequestType: The Request which was made
          *            inPndrError:    The error if any
          *                    E_SUCCESS: If request was successful
          *                    E_NETWORK_FAILED: If network connection failed
          *
          * @return     None
          *******************************************************************
          */
         void SessionTerminateAcked();// added by esjang 
        
        
        /**
         *******************************************************************
         * Provide the station list based on the sort type requested
         *
         * @param[in] inStationInfoList    The list of StationInfo
         *
         * @return     EPndrError: Its value can be
         *                E_SUCCESS: If successfully connected
         *                E_INVALID_STATE: Device is Not connected OR
         *                                    No track is in Play/Pause status
         *******************************************************************
         */
         void StationInfoList(const QList<TSStationInfo>& inStationInfoList);

        /**
         *******************************************************************
         * The track is updated for various reason, eg, station select, skip
         * Its expected that app calls RequestTrackInfo in response to
         * this signal
         *
         * @param[in] inTrackToken: The Track Token of newly playing track
         *
         * @return     None
         *******************************************************************
         */
         void TrackUpdated(const quint32 inTrackToken);

          void StationUpdated(const quint32 inStationToken);
        /**
         *******************************************************************
         * This signal is raised when requested search is complete
         *
         * @param[in] inSearchInfoList: List of search ino
         *
         * @return     None
         *******************************************************************
         */
         void SearchComplete(const QList<TSSearchInfo>& inSearchInfoList);

        /**
         *******************************************************************
         * This signal is raised when station is added
         *
         * @param[in]    inIndexLocation: Index location where station is
         *                      added, considering base as 0
         *                  inStationInfo: Station information for newly added
         *                              station
         *
         * @return     None
         *******************************************************************
         */
         void StationAdded(const quint8 inIndexLocation, const TSStationInfo& inStationInfo);


        /**
         *******************************************************************
         * This signal is raised when station is added
         *
         * @param[in]    inToken: Token ID for deleted station
         *
         * @return     None
         *******************************************************************
         */
         void StationDeleted(const quint32 inToken);

        /**
         *******************************************************************
         * This signal is raised when elapsed time received
         *
         * @param[in]    inElapsedTime: Total elapsed time for current
         *                      song
         *
         * @return     None
         *******************************************************************
         */
         void UpdateElapsedTime(const quint16 inElapsedTime);

        /**
         *******************************************************************
         * This signal is raised total station count is received
         *
         * @param[in]    inStationCount: Total stations available
         *
         * @return     None
         *******************************************************************
         */
         void StationTokens(const quint8 inStationCount, const QList<quint32>& inTokenList);

         void StationSortOrder(ESortType inSortOrder);

        /**
         * The signal emitted to state Handler when any Station Art
         * data is received.
         */
         void StationArtReceived(quint32  inStationToken);

         /**
          *******************************************************************
          * This signal is raised on Audio sampling rate changed in iOS device.
          *
          * @param[in]    inSampleRate: Audio sample rate
          *
          * @return     None
          *******************************************************************
          */
         void AudioSamplingChanged(int inSampleRate);

         void  currentStationInfo(const TSTrackInfo& inTrackInfo);
         //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
         void NextBrandingImage();
         void getBrandingImage();
         //} modified by yongkyun.lee 2014-08-12 

public:
    /**
     * class constructor  .
     *
     * @param None
     *
     *******************************************************************
     */
    CPndrStateHandler();
    /**
     *******************************************************************
     * Destructor
     *
     *******************************************************************
     */
    ~CPndrStateHandler();

    /**
     *******************************************************************
     * Initialize all memory required ,
     *
     * @param      None
     *
     * @return     EPndrError,
     *             E_SUCCESS if successful, otherwise err code
     *
     *******************************************************************
     */
    EPndrError Init();

    /**
     *******************************************************************
     * Free all memory , if init failed,
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
     *******************************************************************
     * Check if requested operation is possible in current state or not
     *
     * @param None
     *
     * @return     EPndrError: Its value can be
     *                E_SUCCESS: If request can be made in current state
     *                E_INVALID_STATE: The requested operation
     *                  not possible in current state
     *
     *******************************************************************
     */
    EPndrError CanProceedWithRequest(const ERequestType inReqType);

    /**
     *******************************************************************
     * This function will connect to device and initiate
     *  the pandora connection
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
     * @return     EPndrError
     *
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
     * @return  None
     *
     *******************************************************************
     */
    void Disconnect();

    void SessionTerminate(); //added by esjang for certification 2013.05.15

    /**
     *******************************************************************
     * This function will will provide the Information
     * about current track being played/paused
     *
     * @param None 
     *
     * @return     None
     *
     *******************************************************************
     */
    void RequestTrackInfo();

    /**
     *******************************************************************
     * Request for explanation for currently played/paused song
     *
     * @param None
     *    
     * @return     None
     *
     *******************************************************************
     */
    void RequestExplanation();

    /**
     *******************************************************************
     * Request to Play the track. This will be possible ONLY
     * if track was paused before
     *
     * @param None
     *
     * @return     None
     *
     *******************************************************************
     */
    void Play();

    /**
     *******************************************************************
     * Request to Pause the current track being played.
     *
     * @param None
     *
     * @return     None
     *
     *******************************************************************
     */
    void Pause();

    /**
     *******************************************************************
     * Request to Skip the song
     *
     * @param None    
     *
     * @return     EPandoraError: Its value can be
     *                E_SUCCESS: If successfully connected
     *                E_INVALID_STATE: Device is Not connected OR
     *                                    No track is in Play/Pause status
     *
     * @see:    TrackUpdated
     *******************************************************************
     */
    void Skip();

    /**
     *******************************************************************
     * Request to Skip the song
     *
     * @param[in]    inRate    The Rating that user would like to give to 
     *                        current track
     *                        1:    ThumbUp
     *                        2:    ThumbDown
     *
     * @return     None
     *
     *******************************************************************
     */
    void Rate(const quint8 inRate);

    /**
     *******************************************************************
     * Request to Bookmark current Track
     *
     * @param None    
     *
     * @return     None
     *
     *******************************************************************
     */
    void BookmarkSong();

    /**
     *******************************************************************
     * Bookmark Artist of current Track
     *
     * @param None    
     *
     * @return     None
     *
     *******************************************************************
     */
    void BookmarkArtist();


    /**
     *******************************************************************
     * Request to set the station sort type. 
     *
     * @param[in]    inSortType    What is expected order for station List
     *
     * @return     None
     *
     *******************************************************************
     */
    void RequestStationSort(const ESortType inSortType);

    /**
     *******************************************************************
     * Request to get the station sort type.
     *
     * @param[in]    None
     *
     * @return     None
     *
     *******************************************************************
     */
    void RequestStationSortOrder();

    /**
     *******************************************************************
     * Request to get the station count.
     *
     * @param[in]    None
     *
     * @return     None
     *
     *******************************************************************
     */
    void RequestStationCount();

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
     *                E_INVALID_STATION_REQ_RANGE: Invalid range
     *
     *
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
     * @return     None
     * 
     *******************************************************************
     */
    void RequestStationSelect(const quint32 inStationToken);

    /**
     *******************************************************************
     * Request for auto search complete given a word
     *
     * @param[in]    inWordToSearch    The word to be searched
     *                                    max length: 248
     *
     * @return     None
     *
     *******************************************************************
     */
    void RequestSearchAutoComplete(const QString& inWordToSearch);

    /**
     *******************************************************************
     * Request for extended search for a given word
     *
     * @param[in]    inWordToSearch    The word to be searched
     *                                    max length: 248
     *
     * @return  None
     *
     *******************************************************************
     */
    void RequestSearch(const QString& inWordToSearch);

    /**
     *******************************************************************
     * Request to select the item from search
     * @param[in]    inSearchID    The searchId
     * @param[in]    inToken    The token to identify the track        
     *
     * @return     None
     *
     *******************************************************************
     */
    void RequestSearchSelect(const quint32    inToken);

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


	  //Below methods are required , when we receive session already started 
      void RequestStatus();
      void RequestActiveStation();
      void RequestTrackBasic();

      //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
	  void RequestBrandingImage(QString BIPath ,QString BIName );
	  void RequestBrandingSegment(const quint8 SegIndex,const quint8 totalSeg, quint8* inData,  int fzise);
      //} modified by yongkyun.lee 2014-08-04 

      /**
       *******************************************************************
       * Fetch Station Art on List Request is on
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
	 * @return	EPriState
	 *
	 *******************************************************************
	 */
	EPriState GetPriState();

	/**
	 *******************************************************************
	 * Returns the Secondary state of Pandora
	 *
	 * @return	ESecState
	 *
	 *******************************************************************
	 */
	ESecState GetSecState();

	/**
	 *******************************************************************
	 * Returns the Previous Secondary state of Pandora
	 *
	 * @return	ESecState
	 *
	 *******************************************************************
	 */
	ESecState GetSecPreviousState();
	//} added by cheolhwan 2014-04-09.

	//{ added by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
	/**
	 *******************************************************************
	 * Request to set the prefetching stations flag.
	 *
	 * @return	None
	 *
	 *******************************************************************
	 */
	void setPreFetchingStations(bool inPreFetchingStations);
	//} added by cheolhwan 2014-04-17. To fix the unlimited receiving station.
	

private :

    /**
     *******************************************************************
     * Initialize all states to default value,
     *
     * @param      None
     *
     * @return     None
     *
     *******************************************************************
     */
    void InitializeStates();

public slots:
    /**
    * Following signals will be emitted by decoder and listened here
    */
    
    /**
    * Used for capturing signal for following
    * PNDR_UPDATE_STATUS
    * PNDR_UPDATE_TRACK_BOOKMARK_SONG
    * PNDR_UPDATE_TRACK_BOOKMARK_ARTIST
    * PNDR_RETURN_STATION_COUNT
    * PNDR_UPDATE_STATIONS_ORDER
    * PNDR_UPDATE_NOTICE
    * PNDR_UPDATE_TRACK_RATING
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const quint8 inData);

    /**
    * Used for capturing signal for following
    * PNDR_UPDATE_TRACK_ELAPSED
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const quint16 inData);

    /**
    * Used for capturing signal for following
    * PNDR_RETURN_TRACK_ALBUM_ART_SEGMENT  // emitted by track controller
    * PNDR_RETURN_TRACK_EXPLAIN_SEGMENT     // emitted by track controller
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const QString& inData);

    /**
    * Used for capturing signal for following
    *   PNDR_RETURN_SEARCH_RESULT_INFO
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const quint32 inSearchId,const QList<TSSearchInfo>& inSearchList);
    
    /**
    * Used for capturing signal for following
    *   PNDR_UPDATE_STATION_ADDED
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const quint32 inData1, const quint8 inData2);

    /**
    * Used for capturing signal for following
    * PNDR_UPDATE_TRACK
    * PNDR_UPDATE_TRACK_ALBUM_ART
    * PNDR_UPDATE_TRACK_EXPLAIN // emitted by track controller
    * PNDR_UPDATE_STATION_DELETED
    * PNDR_UPDATE_STATION_ACTIVE
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const quint32 inData1);
    
    /**
    * Used for capturing signal for following
    *   PNDR_RETURN_TRACK_INFO_EXTENDED
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const TSTrackInfo& inData);
    
    /**
    * Used for capturing signal for following
    *   PNDR_RETURN_STATION_TOKENS
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const quint8 inData1,
                             const QList<quint32>& inData2);

    /**
    * Used for capturing signal for following
    *   PNDR_UPDATE_SEARCH
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const quint32 inData1,
                             const QList<quint32>& inData2);

    /**
    * Used for capturing signal for following
    * Station info
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const QList<TSStationInfo>& inData);


    //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
    void OnRequestBrandingImage();
    //} modified by yongkyun.lee 2014-08-04 
    Q_INVOKABLE void onBrandingImageTimer();//{ modified by yongkyun.lee 2014-10-07 for : Branding - Middleware
    /** Device connection slot */
    void OnDeviceStatusUpdated(const EDeviceType inDeviceType, bool inIsConnected);

    /**
    * On Error
    * This slot is ovoked whehever some error is emitted by decoder
    * @param[in]    inErrCode    The errorcode
    *
    * @return     None
    */
    void OnErrorReceived(EPndrError inErrCode);

    /**
     * Timer slot to be listened to
     * This slot is invoked when no response is received for given time
     */
    void OnTimeout();

    /**
     * This slot is invoked when any Station Art is received
     */
    //void onStationArtReceived(const TSStationArt& inStationArt);

    void onPndrSessionClosed(); //added by esjang 2013.06.27 for ISV #71393, ITS #167008


private:
    EPriState   m_ePriState; /** Store primary state of Pandora */
    ESecState   m_eSecState; /** Store Secondary state of Pandora */
    ESecState   m_eSecPreviousState; /** Store Previous Secondary state of Pandora */

    /** 
    * Variable to store the current data received for 
    * getting complete track info
    */
    ETrackInfoState     m_eTrackInfoState;

    /** Session status */
    EStatusOnAlreadyStartedSession m_eStatusOnSessionStart;
    
    /** Instance of Device Manager class */
    CPndrDeviceManager* m_pDeviceManager;

    /** Instance of Station Controller class */
    CPndrStationController* m_pStationCtrl;

    /** Instance of TrackController class */
    CPndrTrackController* m_pTrackCtrl;

    /** Instance of Track Properties class */
    CPndrTrackProperties* m_pTrackProperties;

    /** Instance of FrameDecoder class */
    CPndrFrameDecoder* m_pFrameDecoder;

    /** Instance of FrameEncoder class
      * This instance is not used here.But instantiaton is done here
      * so that all classes below do not have to check for memory allocation
      */
    CPndrFrameEncoder* m_pFrameEncoder;

    /** Instance of CPndrFrameValidator class
      * This instance is not used here.But instantiaton is done here
      * so that all classes below do not have to check for memory allocation
      */
    CPndrFrameValidator* m_pFrameValidator;

    /** Instance of CPndrFrameTxRx class */
    CPndrFrameTxRx* m_pFrameTxRx;

    /** Station cache */
    CPndrCache* m_pStCache;

   /** Dimension of album art*/
    quint16 m_nAlbumArtDimension;

    /** size of station art or genre station art image */
    quint16 m_nStationArtDimension;

    /** Type of image */
    EImageType m_eAlbumArtImgType;

    /** The track Info object */
    TSTrackInfo m_sTrackInfo;

    /** Timer to be used for waiting for reply*/
    QTimer*         m_pReplyWaitTimer;

    /** Fetch station Art request of station list */
    bool m_bFetchStationArt;

    char* m_Port;

	bool m_PreFetchingStations;  // added by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)

	quint32 m_LastStationToken; // added by cheolhwan 2014-04-25. ITS 234067. To ignore a duplicate of the message with an incorrect sequence number.

    //{ modified by yongkyun.lee 2014-10-07 for : Branding - Middleware
    QString m_BrandingImagePath;
    QString m_BrandingImageName;
    QTimer *m_BandingImageTimer;    
    quint8 m_BrandingImageSegIndex;
    QFile  *m_Brandingfile;
    char   *m_BrandingBuf;
    int    m_BrandingfileSize;
    //} modified by yongkyun.lee 2014-10-07 

};

#endif //DHAVN_PNDR_STATE_HANDLER_H
