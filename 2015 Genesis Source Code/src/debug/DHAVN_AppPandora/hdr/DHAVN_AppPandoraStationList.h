/*
 ******************************************************************
 *        COPYRIGHT ?2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :                                   *
 * PROGRAMMER : Vandana Ahuja                                     *
 * DATE       : 21/12/2011                                        *
 * VERSION    : 0.1                                               *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module will be responsible for collecting data from
 *  Pandora MW to be displayed in station list view.
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  21/12/2011  0.1 Vandana Ahuja   header file created
 *
 ******************************************************************
 */

#ifndef DHAVN_AppPandoraStationList_H
#define DHAVN_AppPandoraStationList_H

#include <DHAVN_Pndr_Defs.h>
#include <DHAVN_AppPandoraSortTypes_def.h>
#include <DHAVN_PndrInterface.h>
#include <DHAVN_LogUtil.h>
#include <QList>
#include <QMap>

#include "DHAVN_AppPandoraDeviceListInfo.h"

class CPndrStateHandler ;

class PandoraListReqType : public QObject
{
    Q_OBJECT
    Q_ENUMS(EStationListRequestType)
public:
    enum EStationListRequestType
    {
        ENONELIST = 0x00,
        EFIRSTLIST = 0x01,
        ENEXTLIST = 0x02,
        EPREVLIST = 0x04,
        EQUICKLIST = 0x08
    };
};


class CAppPandoraStationList : public QObject
{
    Q_OBJECT
public:

    /**
     *******************************************************************
     * Station list constructor
     *
     * @param[in] inPndrInterface   interface reference to Pandora MW
     *
     * @return  None
     *
     *******************************************************************
     */
    CAppPandoraStationList(/*CPndrInterface&  inPndrInterface*/);

    void SetInterface(CPndrInterface*  m_PndrInterface);
    void SaveState(DeviceInfo* inDevice);

    void RetriveState(DeviceInfo* inDevice);

    /**
     *******************************************************************
     * Station list Destructor
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    ~CAppPandoraStationList();


    /**
     *******************************************************************
     * Request from QML layer for getting the 1st n number of stations
     *
     * @param[in] inMaxCount Max number of stations that QML layer can
     *                          receive
     * @param[in] isAlphaSort Station list is requested in alphabatical
     *                        order if its value is true else date wise
     *                        sorting will be applied
     * @return None
     *
     *******************************************************************
     */
     Q_INVOKABLE void GetFirstStationListSlot(const int inMaxCount, quint32 inActiveStationToken);

    /**
     *******************************************************************
     * Request from QML layer for getting the next stations of reqtype
     *
     * @param[in] inReqType : values can be any of the EStationListRequestType
     *
     * @return None
     *
     *******************************************************************
     */

     Q_INVOKABLE void GetNextStationList(int inReqType);

    /**
     *******************************************************************
     * Request from QML layer for getting Sorted stations list of
     *
     * @param[in] sortType : values can be any of the PandoraSortTypes
     *
     * @param[in] inActiveStationToken : current Active Station token
     *              for finding index in latest list
     * @return None
     *
     *******************************************************************
     */

     Q_INVOKABLE void GetSortedStationList(int sortType, quint32 inActiveStationToken);

    /**
     *******************************************************************
     * Request from QML layer for getting Total No. Of stations
     *
     * @return int : returns total no. of stations
     *
     *******************************************************************
     */

     Q_INVOKABLE int GetTotalStationsCount() {return m_nStationCount;}

    /**
     *******************************************************************
     * Request from QML layer for getting the current start index of the list
     *
     * @return int : returns current start index of the displayed list
     *
     *******************************************************************
     */

     Q_INVOKABLE int GetCurrentStartIndex() { return m_nStartIndex;}

    /**
     *******************************************************************
     * Request from QML layer for getting the current end index of the list
     *
     * @return int : returns current end index of the displayed list
     *
     *******************************************************************
     */

     Q_INVOKABLE int GetCurrentEndIndex() { return m_nEndIndex;}

    //{ added by esjang 2013.01.03 for ITS #150550
    /**
     *******************************************************************
     * Request from QML layer for getting the current Current Active Station index of the list
     *
     * @return int : returns current end index of the displayed list
     *
     *******************************************************************
     */

     Q_INVOKABLE int GetCurrentActiveIndex() { return m_nRelativeActiveStationIndex;}

    //} added by esjang 2013.01.03 for ITS  #150550

    /**
     *******************************************************************
     * Request from QML layer for selecting the station given the index
     * from station list
     *
     * @param[in] index index of selected station
     *
     * @return  None
     *
     *******************************************************************
     */
    Q_INVOKABLE void SelectStation(const int index);

    /**
     *******************************************************************
     * Request from QML layer for selecting the station given the name
     * from station list
     *
     * @param[in] name , name  of selected station
     *
     * @return  None
     *
     *******************************************************************
     */
    Q_INVOKABLE void SelectStation(const QString stationName);

    /**
     *******************************************************************
     * Cancels any outstanding request of station Art when user goes back
     * from List view
     *
     * @return  void
     *
     *******************************************************************
     */
    Q_INVOKABLE void CancelStationArtRequest();

   /**
    *******************************************************************
    * Clear the cache on back soft key .
    *
    * @param  None
    *
    * @return None
    *
    *******************************************************************
    */
    Q_INVOKABLE void ClearCache();

    /**
     *******************************************************************
     * Clear current request state .
     *
     * @param  None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void ClearCurrentRequest();

    /**
     *******************************************************************
     * returns the index for the given alphabet
     *
     * @param  int : index of the alphabet
     *
     * @return None
     *
     *******************************************************************
     */

     Q_INVOKABLE int GetIndexForAlphabet(QString alphabet, bool isBT );// modified by yongkyun.lee 2014-06-03 for :  ITS 238641

    /**
     *******************************************************************
     * Request from QML Layer to play shuffle
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
     Q_INVOKABLE void PlayShuffle();

    /**
     *******************************************************************
     * Request from QML layer for getting whether next stations are exist or not
     *
     * @return bool : returns true or false
     *
     *******************************************************************
     */
     Q_INVOKABLE bool IsNextItemExist();

    /**
     *******************************************************************
     * Request from QML layer for getting whether prev stations are exist or not
     *
     * @return bool : returns true or false
     *
     *******************************************************************
     */
     Q_INVOKABLE bool IsPrevItemExist();

    /**
     *******************************************************************
     * Request from QML layer for clearing the listreqtype
     *
     * @return None
     *
     *******************************************************************
     */
     Q_INVOKABLE void ResetListReqType(){ m_eListReqType = PandoraListReqType::ENONELIST;}

    /**
     *******************************************************************
     * Request from QML layer for reseting perticular request
     * @param inListReqType : To be reseted
     * @return None
     *
     *******************************************************************
     */
     Q_INVOKABLE void  ResetListReqType(qint8 inListReqType){if(inListReqType&m_eListReqType)m_eListReqType^=inListReqType;}

    /**
     *******************************************************************
     * Request from QML layer for getting station art image if avialable
     *
     * @return filePath if exist ortherwise NULL
     *
     *******************************************************************
     */
     Q_INVOKABLE QString IsStationArtPresent(int index);

    /**
     *******************************************************************
     * Request from QML layer for getting station art image if avialable
     *
     * @return filePath if exist ortherwise NULL
     *
     *******************************************************************
     */
     Q_INVOKABLE QString StationArtPresent(quint32 inToken);

//{ modified by yongkyun.lee 2014-06-18 for : ITS 240645
	void GetStationCount();
//} modified by yongkyun.lee 2014-06-18 

    /**
     *******************************************************************
     * Get Station name from the tune index
     * @param isForward , True : Forward , False : Reverse
     *
     * @return  void
     *
     *******************************************************************
     */
     Q_INVOKABLE void GetStationName(bool isForward);

    /**
     *******************************************************************
     * Tune off , Timer off
     * @param void
     *
     * @return  void
     *
     *******************************************************************
     */
     Q_INVOKABLE void TuneOff();

    /**
     *******************************************************************
     * Tune select on perticular station , Will change to that station
     * @param void
     *
     * @return  void
     *
     *******************************************************************
     */
     Q_INVOKABLE void TuneSelect();

    /**
     *******************************************************************
     * Update OSD on Station tunned , If app is in BG
     * @param void
     *
     * @return  void
     *
     *******************************************************************
     */
     void UpdateTunnedStation_OSD();

     /**
      *******************************************************************
      * IsPendingRequest - Is there any on going request
      * @param None
      *
      * @return bool : True , If request is on
      *
      *******************************************************************
      */
     Q_INVOKABLE bool IsPendingRequest(){return m_eCurrentRequest;}

	/**
      *******************************************************************
      * FetchStationArt - Set fetch station art flag true , only when station list view is 
      * on FG
      * @param None
      *
      * @return void
      *
      *******************************************************************
      */
     Q_INVOKABLE void FetchStationArt();

      /**
      *******************************************************************
      * StationName - Station Name at perticular index
      *
      * @param index : index of the list
      *
      * @return QString : Name of station
      *
      *******************************************************************
      */
    //{ modified by yongkyun.lee 2014-03-28 for : List update
     Q_INVOKABLE QString StationName(int index);

     Q_INVOKABLE int StationListCount(){
        return m_StationInfo.count();
     }     

     Q_INVOKABLE int StationListSortType(){
	 return (int)m_eSortType;
     }
    //} modified by yongkyun.lee 2014-03-28 
     /**
      *******************************************************************
      * Request from QML layer for getting the key value of station name
      *
      * @param[in] inStr Station Name
      *
      * @return Key
      *
      *******************************************************************
      */
     Q_INVOKABLE QString Key(QString inStr,  bool isBT);// modified by yongkyun.lee 2014-06-03 for :  ITS 238641

     /**
      *******************************************************************
      * Request from QML layer for getting the current station shared status
      *
      * @param None
      *
      * @return bool , Current station shared status
      *
      *******************************************************************
      */
     //{ modified by yongkyun.lee 2014-03-28 for : List update
     Q_INVOKABLE bool IsSharedStation(int index);
     //} modified by yongkyun.lee 2014-03-28 

     /**
      *******************************************************************
      * Request from QML layer for getting the current station name
      *
      * @param None
      *
      * @return QString , Current station names
      *
      *******************************************************************
      */
     Q_INVOKABLE QString CurrentStationName();

     /**
      *******************************************************************
      * Request from QML layer for getting the prefetching station status
      *
      * @param None
      *
      * @return QString , Current station names
      *
      *******************************************************************
      */
     Q_INVOKABLE bool IsPreFetchingStations(){return m_PreFetchingStations;}

private:
    /**
     *******************************************************************
     * Request from QML layer for getting the next n number of stations
     *
     * @param[in] inMaxCount Max number of stations that QML layer can
     *                          receive
     *
     * @return None
     *
     *******************************************************************
     */
     void GetStationListSlot(const int startIndex, const int endIndex);

     /**
      *******************************************************************
      * Handle the error
      *
      * @param[in] inPndrError error code
      *
      * @return None
      *
      *******************************************************************
      */
     void HandleIfError(const EPndrError inPndrError);

     /**
     *******************************************************************
     * Reset values
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
     */
    void ResetValue();

    /**
     *******************************************************************
     * EvaluateAlphaSortedListIndexes - for finding the start indexes of alpha sorted
     * stations list
     *
     * @return none
     *
     *******************************************************************
     */

     void EvaluateAlphaSortedListIndexes();

     /**
      *******************************************************************
      * GetActiveStationIndex - for finding the current active station index
      * stations list
      *
      * @return qint8 : return current active station index
      *
      *******************************************************************
      */

     qint8 GetActiveStationIndex();

     /**
      *******************************************************************
      * GetStationTokenIndex - for finding the current station token index
       * @param quint32 : Station token
      *
      * @return qint8 : return station index from token list
      *
      *******************************************************************
      */
     qint8 GetStationIndex(const quint32 inToken);

     //{ added by cheolhwan 2014-04-17.  To fix the unlimited receiving station. (ITS 238629)
	 void setPreFetchingStations(bool inPreFetchingStations);
	 //} added by cheolhwan 2014-04-17.  To fix the unlimited receiving station.

signals:

    /**
     *******************************************************************
     * Signal to QML layer providing the station token list
     *
     * @param[in]   inListCount    Station token count
     * @return  None
     *
     *******************************************************************
     */
     void stationToken(const int inListCount );

    /**
     *******************************************************************
     * Signal to QML layer providing the data for requested station list
     *
     * @param[in]   inStartIndex    The index from which the station
     *                              list received
     * @param[in]   inStationList   The list of stations to be displayed
     *
     * @param[in]   inType          sorting type
     *
     * @param[in]   inCurrPlayIndex currentactive station index
     *
     * @param[in]   inReqType       stations request type
     * @return  None
     *
     *******************************************************************
     */
    void stationListReceived(const int inStartIndex, int inCount ,/*const QStringList& inStationList ,*/
                             const quint16 inType, qint8 inCurrPlayIndex,
                             qint8 inReqType /*, const QStringList& inStToken*/);


    /**
     *******************************************************************
     * Signal to QML layer to update view for station deletion
     *
     * @param[in] inIndex   The station index deleted
     *
     * @return  None
     *
     *******************************************************************
     */
    void stationDeleted(const int inIndex);

    /**
     *******************************************************************
     * Signal to QML layer to update view for insertion of new station
     *
     * @param[in] inIndex   The station index where new station to
     *                      be added
     * @param[in] inStationName The name of station to be added
     *
     * @return  None
     *
     *******************************************************************
     */
    void stationInserted(const int inIndex, const QString inStationName);

    /**
     *******************************************************************
     * Signal to QML layer to update station Art
     *
     * @param[in] inStationArtIndex   The station index whose station art is being emitted
     *
     * @param[in] inStationArtPath The path of station art
     *
     * @return  None
     *
     *******************************************************************
     */
    void stationArtReceived(const int inStationArtIndex, const QString inStationArtPath);

    void toastLoadingPopup();	 //{ modified by yongkyun.lee 2014-09-01 for : Pandora 4.25 TEST

	/**
     *******************************************************************
     * Signal to QML layer to update station Art on track view
     * @param[in] inStationArtPath The path of station art
     *
     * @return  None
     *
     *******************************************************************
     */
    void currStArt(QString inStationArtPath);

    /**
     *******************************************************************
     * Signal to QML updating the play state
     *
     * @param[in] true: play, false:pause
     *
     * @return  None
     *
     *******************************************************************
     */
    void setPlayState(const bool playStatus);

    /**
     *******************************************************************
     * Signal to QML updating the playstatus
     *
     * @param[in] playstatus: true:play, false:pause
     *
     * @return  None
     *
     *******************************************************************
     */
    void handleError(const quint16 inError);

    /**
     *******************************************************************
     * Signal to QML and Notifier updating the tunedStation info
     *
     * @param[in] token: Station Token
     * @param[ in ] inStationName : Station Name
     * @param[ in ] isCurr : True if current station otherwise false.
     *
     * @return  None
     *
     *******************************************************************
     */
    void tunedStation(quint32 token ,  QString inStationName , bool isCurr);

    /**
     *******************************************************************
     * Signal to QML and Notifier updating the request fail status
     *
     * @param   None
     * @return  None
     *
     *******************************************************************
     */
    void requestFail();

	/**
	*******************************************************************
	* preFetchingCompleted - Station pre fetching completed , Inform to QML
	* @param None
	*
	* @return void
	*
	*******************************************************************
	*/
    void preFetchingCompleted();

	/**
	*******************************************************************
	* preFetchingCompleted - Station pre fetching started , Inform to QML
	* @param None
	*
	* @return void
	*
	*******************************************************************
	*/
    void preFetchingStart();

     /**
     *******************************************************************
     * Signal to QML layer for notifying the active station token
     *
     * @param m_nActiveStationToken , Active station token
     *
     * @return None
     *
     *******************************************************************
     */
    void activeStationToken(quint32 inActiveStationToken);

    /**
     *******************************************************************
     * Signal to QML layer for notifying There is no station present to tune
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    void noStationPresent();


public slots:
    /**
     *******************************************************************
     * Slot to listen to signal from Pandora MW for receiving the
     * station info list
     *
     * @param[in] inStationInfoList Station list info.
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnStationInfoList(const QList<TSStationInfo>& inStationInfoList );

    /**
     *******************************************************************
     * Slot to listen to signal from Pandora MW for receiving the
     * station sort update along with the station count found
     *
     * @param[in] inStationCount The total count of station found
     *                            after sorting
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnStationTokens(const quint8 inStationCount, const QList<quint32>& inTokenList);

    /**
     *******************************************************************
     * Slot to listen to signal from Pandora MW for getting info on
     * station deletion
     *
     * @param[in] inToken   station identifier for deleted station
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnStationDeleted(const quint32 inToken);

    /**
     *******************************************************************
     * Slot to listen to signal from Pandora MW for getting info on
     * added station
     *
     * @param[in] inIndexLocation The index at which station is added
     * @param[in] inStationInfo   The station info for station which is
     *                              added
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnStationAdded(const quint8 inIndexLocation, const TSStationInfo& inStationInfo);

    /**
     *******************************************************************
     * Slot to listen to signal from Pandora MW for getting info on
     * station sort order
     *
     * @param[in] inType The station sort order
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnStationSortOrder(const ESortType inType);

    /**
     *******************************************************************
     * This slot is invoked when any Station Art is received *
     * @param   inStationArt The station Art Object
     *
     * @return  void
     *
     *******************************************************************
     */
    void OnStationArtReceived(quint32 inToken);

    /**
     *******************************************************************
     * Slot to listen to signal from Pandora MW for getting info on
     * station update
     *
     * @param[in] inStationToken   station identifier for new station
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnStationUpdated(const quint32 inStationToken);


    /**
     *******************************************************************
     * Slot to listen to signal from Pandora MW for getting info on
     * TrackInfoReceived , To start station prefetching for 1st time.
     *
     * @param[in] inTrackInfo   : Track Info
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnCurrentStationInfo(const TSTrackInfo& inTrackInfo );

private:
    /** Pandora MW interface reference */
    CPndrInterface* m_PndrInterface;

    /** Signal handler */
    CPndrStateHandler* m_pSignals ;

    /** Current outstanding request*/
    ERequestType m_eCurrentRequest;

    /** Total stations count */
    quint8  m_nStationCount;

    /** Current Active Station Token */
    quint32 m_nActiveStationToken;

    /* Current Active Station Index */
    qint8 m_nRelativeActiveStationIndex;

    /** Maximum count requested */
    quint8 m_nMaxCount;

    /**Start Index*/
    qint8 m_nStartIndex;

    /**End  Index*/
    qint8 m_nEndIndex;

    qint8 m_nReqStartingIndex;

     /** Caching of station info */
    QList<TSStationInfo> m_StationInfo;

	/** Caching of station label with token ID */
    QMap<quint32 , QString> m_StationLabel;

    /** Caching of station tokens */
    QList<quint32> m_StationTokens;

    /** Sort type for station */
    ESortType m_eSortType;

    QMap<char,int> m_AlphaListIndexTable;

    qint8 m_eListReqType;

    /** Is list first time getting launched */
    qint8 m_nIs1stTimeLaunch;

    /** Tunning Index */
    qint8 m_nTuningIndex;

    /** Tunning In Progress */
    bool m_bIsTuningInProgress;

     /** List Requested for tunning */
    bool m_RequestInprogress;

    bool m_VrReqToPlayShuffle;

    quint32 m_ShuffleStationToken;

    /* Prefetching stations */
    bool m_PreFetchingStations;

    /** Logger instance */
    USE_LOG_UTIL
};

#endif // DHAVN_AppPandoraStationList_H
