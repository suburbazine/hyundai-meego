/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : Station Controller                                *
 * PROGRAMMER : Vandana Ahuja                                     *
 * DATE       : 03/12/2011                                        *
 * VERSION    : 0.1                                               *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module Selects the command to be encoded for given request
 *  and 
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  03/12/2011  0.1 Vandana Ahuja   header file created
 *  20/06/2013  0.2 Radhakrushna Mohanty Modified
 *  
 ******************************************************************
 */


#ifndef DHAVN_PNDR_STATION_CONTROLLER_H
#define DHAVN_PNDR_STATION_CONTROLLER_H

#include <QObject>
#include <QTimer>

#include "DHAVN_Pndr_Defs.h"
#include "DHAVN_Pndr_Internal_Defs.h"
#include "DHAVN_PndrFrameEncoder.h"
#include "DHAVN_PndrFrameDecoder.h"
#include "DHAVN_PndrLinkConstants.h"
#include "DHAVN_LogUtil.h"
//Forward Declarations
class CPndrFrameEncoder;
class CPndrFrameTxRx;

#define STATION_ART_PATH  "/app/data/AppPandora/StationArt"
#define STATION_ART_FILE  "/StationArt"
#define STATION_ART_TYPE_JPEG  ".jpeg"
#define STATION_ART_TYPE_PNG  ".png"
#define STATION_ART_TYPE_RGB565  ".bmp"

/**
* This class send encoder the required data for encoding 
* and then send the encoded data through Frame Validator
*/
class CPndrStationController : public QObject
{
    Q_OBJECT

public:
    /** 
    * Class CPndrStationController constructor and destructor
    *   
    */
    CPndrStationController();
    ~CPndrStationController();

    void SetFrameTxRX(CPndrFrameTxRx*& inFrameTxRx);

    void SetEncoder(CPndrFrameEncoder* inFrameEncoder);

    void SetDecoder(CPndrFrameDecoder* inFrameDecoder);

    /**
     *******************************************************************
     * Request station Select
     *
     * Instructs the Mobile App to begin playing the given station. PNDR_UPDATE_STATION_ACTIVE will be sent
     *   when station changes.
     * @param[in]    inStationToken  station token
     *
     * @return     None
     *
     *******************************************************************
     */
     void RequestStationSelect(quint32 inStationToken);

    /**
     *******************************************************************
     * Request station sorting
     *
     * @param[in]    inSortType  The Sort type to be requested. It can
     *                              be alphabetical or date based
     *
     * @return     None
     * 
     *******************************************************************
     */
     void RequestStationSort(const ESortType inSortType);

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
     void RequestStationSortOrder();

    /**
     *******************************************************************
     * Request station count
     *
     * @param None
     *
     * @return     None
     * 
     *******************************************************************
     */
     void RequestStationCount();

    /**
     *******************************************************************
     * Request all station tokens
     *
     * @param None
     *
     * @return     None
     * 
     *******************************************************************
     */
     void RequestAllStationTokens();

    /**
     *******************************************************************
     * Request Station Information
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
     *******************************************************************
     */
     EPndrError RequestStationInfo(const qint8 inStartIndex,
                                  const qint8 inEndIndex);

     /**
      *******************************************************************
      * Request Current Station Information
      * @param[in]  inImgType The Station Art Image type
      * @return     EPndrError: Its value can be
      *                E_SUCCESS: If successfully connected
      *                E_INVALID_STATION_REQ_RANGE: Invalid range
      *
      *******************************************************************
      */
      void RequestCurrentStationInfo(const EImageType inImgType);

    /**
     *******************************************************************
     * Request for auto search complete
     *
     * @param[in]    inSearchWord    String to be searched
     *
     * @return     None
     * 
     *******************************************************************
     */
     void RequestAutoCompleteSearch(const QString& inSearchWord);

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
     void RequestExtendedSearch(const QString& inSearchWord);

    /**
     *******************************************************************
     * Request to get info for given search ID
     *
     * @param[in]    inSearchID    searchID for which the info 
     *                                  is requested
     *
     * @return     None
     * 
     *******************************************************************
     */
     void RequestSearchResultInfo(const quint32 inSearchID,const QList<quint32>& inStationTokens);

    /**
     *******************************************************************
     * Request to select an Item from search result 
     *
     * @param[in]    inSearchID    The search ID which is in use
     * @param[in]    inToken       The token id for item within search
     *
     * @return     None
     * 
     *******************************************************************
     */
     void RequestSearchSelect(const quint32 inToken);

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
       * Returns the current sort order
       *
       * @return  ESortType the current sort type
       *
       *******************************************************************
       */
      ESortType CurrentSortOrder();

      /**
       *******************************************************************
       * Returns the status of station info availability and fill the same
       *
       * @param inTrack, update station info , if available
       * @return  true , If available
       *
       *******************************************************************
       */
      bool IsStInfoAvail(TSTrackInfo& inTrack);

      /**
       *******************************************************************
       * Save active station info
       *
       * @param inStation, Active station info to be be saved
       * @return  none
       *
       *******************************************************************
       */
      void SetActiveStation(TSTrackInfo& inStation);

      /**
       *******************************************************************
       * Clear station info on station change
       *
       * @param none
       * @return  none
       *
       *******************************************************************
       */
      void ResetActiveStation();

      /**
       *******************************************************************
       * Request to get thye active station token
       *
       * @param none
       * @return  none
       *
       *******************************************************************
       */
      void GetActiveStationToken();

      /**
       *******************************************************************
       * Save station tokens , If not received .
       *
       * @param inTokenList , Token list
       * @return  none
       *
       *******************************************************************
       */
      void SaveStationTokens(const QList<quint32>& inTokenList);

	  /**
       *******************************************************************
       * Request for current station art .
       *
       * @param none
       * @return  none
       *
       *******************************************************************
       */
      void RequestCurrentStationArt();


      //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
	  void RequestBrandingImage(quint32 indata);
	  void RequestBrandingSegment(  const quint8 SegIndex,const quint8 totalSeg, quint8* inData,  int fzise);
      //} modified by yongkyun.lee 2014-08-04 


      /**
       *******************************************************************
       * Request for clear all cache info .
       *
       * @param none
       * @return  none
       *
       *******************************************************************
       */
      void ClearData();
public slots:

    /**
    *   Returns an array of station tokens as requested by PNDR_GET_STATION_TOKENS or
    *   PNDR_GET_ALL_STATION_TOKENS. used for capturing signal-> PNDR_RETURN_STATION_TOKENS
    */
    void OnRequestCompletion(const EPndrEvent inRequest,const quint8 inStart_index, const QList<quint32>& inTokenList);
    /**
    * Used for capturing signal for following    
    *   PNDR_UPDATE_STATION_ADDED
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const quint32 inData1, const quint8 inData2);

    /**
    * Used for capturing signal for following    
    *   PNDR_UPDATE_STATION_DELETED
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const quint32 inData1);

    /**
    * Used for capturing signal for following
    * PNDR_RETURN_STATIONS_ORDER
    */
    void OnRequestCompletion(const EPndrEvent inRequest, const quint8 inData);

    /**
      *******************************************************************
      * Slot for receiving the signal for completion of
      *              event request for track explain and Album Art
      *              PNDR_RETURN_STATION_ART_SEGMENT
      *
      *  @param[in]  inRequest   Request type for which this is the
      *                          response
      *  @param[in]  inStationToken     The station token
      *  @param[in]  inArtLength        The art length
      *  @param[in]  inSegmentindex     The segment index
      *  @param[in]  inTotalsegments     Total Segments
      *  @param[in]  inData     Data
      *
      * @return  None
      *
      *******************************************************************
      */

    void OnRequestCompletion(const EPndrEvent inRequest,const quint32 inStationToken,
                                                const quint32 inArtLength,
                                                const quint8 inSegmentindex,
                                                const quint8 inTotalsegments,
                                                const QByteArray& inData);
    void RequestStArt();
    //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
    void OnRequestbranding();
    //} modified by yongkyun.lee 2014-08-12 

Q_SIGNALS:
    void ReplyReceived(const EPndrEvent inRequest,const quint8 inData);
    //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
    void ReceivedBranding();
    void getBrandingImage();
    //} modified by yongkyun.lee 2014-08-12 

    /**
      * The signal emitted to state Handler when any Station Art
      * data is received.
      */
    void StationArtReceived(quint32 inStationToken);

public:

    /**
     *******************************************************************
     * Clears the previous StationArts if any from the folder before making
     * a new request
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    void ClearStationArtFolder();

    /**
     *******************************************************************
     * If StationArts presents in folder for specific token id
     *
     * @param inTokenId , station token  id
     *
     * @return  true , if avilable
     *
     *******************************************************************
     */
    bool IsStationArtExist(qint32 inTokenId);

     /**
     *******************************************************************
     * Fetch Station Art
     *
     * @param inStartIndex , inEndIndex , currently Not in use
     *
     * @return void
     *
     *******************************************************************
     */
    void FetchStationArt(int inStartIndex, int inEndIndex);

   //{ added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.
     /**
    *******************************************************************
    * Return the last SearchId
    *
    * @param m_nSearchId : the last search id.
    *
    * @return quint32
    *
    *******************************************************************
    */
    quint32 getLastSearchId(){return m_nSearchId;}
   //} added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.


private:
    USE_LOG_UTIL;
    /**
    * Caching of station token list to be used to get the list of station
    * for given range
    */
    /** list to store station tokenlist*/
    QList<quint32> m_StationTokenList;

    /** Search Identifier*/
    quint32  m_nSearchId;

    /** Instance of Encoder class */
    CPndrFrameEncoder* m_pFrameEncoder;

    /** Instance of Decoder class */
    CPndrFrameDecoder* m_pFrameDecoder;

    /** Instance of FrameTxRx class */
    CPndrFrameTxRx* m_pFrameTxRx;

    /** Token Id of Active Station */
    quint32 m_ActiveStationId;

    ESortType m_SortType;

    /** Bytearray to store the station art data */
    QByteArray  m_ByteArr;

    /** The station art image type */
    EImageType m_eImgType;

    /** Station token for which station art fetch is in progress */
    quint32 m_nArtStationToken;

    /** A List of stations whose station art has not been fetched yet */
    QList<quint32> m_PendingStationArtList;

    QList<quint32> m_QueueNextList;

    /** Active station info */
    TSStationInfo m_ActiveStation;

    QTimer m_Timer;
};


#endif //DHAVN_PNDR_STATION_CONTROLLER_H
