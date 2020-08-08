/*
 ******************************************************************
 *        COPYRIGHT 2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :                                   *
 * PROGRAMMER : Vandana Ahuja                                     *
 * DATE       : 21/12/2011                                        *
 * VERSION    : 0.1                                               *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module will be responsible for collecting data from
 *  Pandora MW to be displayed in Track view.
 *

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  21/12/2011  0.1 Vandana Ahuja   header file created
 *
 ******************************************************************
 */

#ifndef DHAVN_APPPANDORATRACK_H
#define DHAVN_APPPANDORATRACK_H

#include "DHAVN_AppPandora_Def.h"
#include "DHAVN_AppPandoraDeviceListInfo.h"
#include <DHAVN_Pndr_Defs.h>
#include <DHAVN_PndrInterface.h>
#include <DHAVN_LogUtil.h>
#include <QDeclarativeView>
#include <QDeclarativeContext>


//#ifdef FRAMEWORK_ENABLED
//#include <DHAVN_QRPMClient.h>
//#endif

class TrackInfo;
class CPndrStateHandler;
class CAppPandoraBase;
class CAppPandoraTrack : public QObject
{
    Q_OBJECT

public:


    /**
     *******************************************************************
     * Track constructor
     *
     * @param[in] inPndrInterface   interface reference to Pandora MW
     *
     * @return  None
     *
     *******************************************************************
     */
    CAppPandoraTrack(CAppPandoraBase* inMainEngine);
    void SetInterface(CPndrInterface*  m_PndrInterface);

    /**
     *******************************************************************
     * Track destructor
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    ~CAppPandoraTrack();

    /**
     *******************************************************************
     * Register  the QDeclarativeView instance , to be used to register qml property
     *
     * @param [IN] inDeclarativeView Instance of QDeclarativeView
     *
     * @return None
     *
     *******************************************************************
     */

    void SetDeclarativeView(QDeclarativeView &inDeclarativeView){m_pDeclarativeView = &inDeclarativeView;}

    /**
     *******************************************************************
     * Request from QML Layer to play current Track
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void Play();


    /**
     *******************************************************************
     * Request from QML Layer to Pause current Track
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void Pause();


    /**
     *******************************************************************
     * Request from QML Layer to Skip current Track
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void Skip();


    /**
     *******************************************************************
     * Request from QML Layer to Rate up the current track
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void ThumbUp();


    /**
     *******************************************************************
     * Request from QML Layer to rate down the current track
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void ThumbDown();


    /**
     *******************************************************************
     * Request from QML Layer to Bookmark the current track
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void BookmarkSong();


    /**
     *******************************************************************
     * Request from QML Layer to Bookmark artist for current track
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void BookmarkArtist();

    /**
     *******************************************************************
     * Request from QML Layer to get elapsed time for current track
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void SetTrackElapsedPolling(bool inEnabled);


    /**
     *******************************************************************
     * Request from QML Layer for getting explaination for current track
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void RequestExplaination();

    /**
     *******************************************************************
     * Request from QML Layer for getting track info for current track
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void RequestTrackInfo();

    /**
     *******************************************************************
     * Request from QML Layer for getting play/pause status for current track
     *
     * @param None
     *
     * @return qint8 , 1 for playing , 0 for pause .
     *
     *******************************************************************
     */
    Q_INVOKABLE qint16 TrackStatus(){return nPlayPause;}

    /**
     *******************************************************************
     * New station is selected from station list.
     *
     * @param None
     *
     * @return None
     *
     *******************************************************************
     */
    Q_INVOKABLE void ClearTrackInfo();
    Q_INVOKABLE void onBrandingImageStart();// modified by yongkyun.lee 2014-08-25 for : Branding Image

  //{ added by esjang 2013.03.06 for DH Genesis GUI Design Guideline v1.1.2(2013.02.28)
   /**
    *******************************************************************
    * Return the current play status.
    *
    * @param  None
    *
    * @return true: Playing state
    *         false: Pause state        
    *
    *******************************************************************
    */
    Q_INVOKABLE bool IsPlaying() const;
//} added by esjang 2013.03.06 for DH Genesis GUI Design Guideline v1.1.2(2013.02.28)

    /**
     *******************************************************************
     * Request from QML layer for getting station art image if avialable
     *
     * @return filePath if exist ortherwise NULL
     *
     *******************************************************************
     */
    Q_INVOKABLE QString IsStationArtPresent();

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

	
    //{ added by cheolhwan 2014-03-06. When BT Pandora enter, if inTrackToken is 0, connetting view is displayed indefinitely or paused on trackview.
    /**
    *******************************************************************
    * Return Curretn Station Token.
    *
    * @param  None
    *
    * @return quint32
    *
    *******************************************************************
    */
    Q_INVOKABLE quint32 GetCurrentStationToken(){return m_nCurrentStationToken;}
    //} added by cheolhwan 2014-03-06. When BT Pandora enter, if inTrackToken is 0, connetting view is displayed indefinitely or paused on trackview.

    Q_INVOKABLE void CheckTrackInfo();//{ modified by yongkyun.lee 2014-08-21 for :  ITS 244978	
    Q_INVOKABLE bool IsNullTrackInof(){ return m_TrackInfo == NULL? true:false;} //{ modified by yongkyun.lee 2014-10-31 for : ITS 251486

    /**
    *******************************************************************
    * Aquire audio path while connecting pandora.
    *
    * @param  inDeviceType , Connected device type.
    *
    * @return None
    *
    *******************************************************************
    */
   // void SetAudioPathRPM (DeviceInfo::EConnectionType inDeviceType);

    TrackInfo* GetCurrentTrackInfo();

    /**
    *******************************************************************
    * AV_MODE has changed , Other AV apps currently running
    * So Pause pandora . Play pandora on FG .
    *
    * @param None
    * @return None
    *
    *******************************************************************
    */
    void HandleAVModeChange();
    void SaveState(DeviceInfo* inDevice);
    void RetriveState(DeviceInfo* inDevice);

public slots:
    Q_INVOKABLE void onPlayTimer();
    Q_INVOKABLE void startPlayTimer();

private:
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

signals:
    /**
     *******************************************************************
     * Signal to QML Layer for informing about play success
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    void playStarted();

    /**
     *******************************************************************
     * Signal to QML Layer for informing about Pause success
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    void pauseDone();

    /**
     *******************************************************************
     * Signal to QML Layer for informing about Skip request failure
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    void skipFailed();

    /**
     *******************************************************************
     * Signal to QML Layer for informing about update of track
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    void trackUpdated(const quint32 inTrackToken);

    /**
     *******************************************************************
     * Signal to QML Layer for informing the current track information
     *
     * @param[in] inTrackInfo the information for current track
     *
     * @return  None
     *
     *******************************************************************
     */
    void trackInfoReceived();

    void trackShardReceived(); //add by wonseok.heo for DH PE


    /**
     *******************************************************************
     * Signal to QML Layer for informing the current track explaination
     *
     * @param[in] inExplain the explaination for current track
     *
     * @return  None
     *
     *******************************************************************
     */
    void trackExplainReceived(const QString& inExplain);

    /**
     *******************************************************************
     * Signal to QML Layer for informing about success of Bookmark song
     * request
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    void songBookmarked();

    /**
     *******************************************************************
     * Signal to QML Layer for informing about success of bookmark artist
     * request
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    void artistBookmarked();

    /**
     *******************************************************************
     * Signal to QML Layer for informing about the current elapsed
     * time for currently playing track
     *
     * @param[in] inElapsedTime The elapsed time in Sec
     *
     * @return  None
     *
     *******************************************************************
     */
    void trackElapsedTime(const unsigned int inElapsedTime);

    /**
     *******************************************************************
     * Signal to QML Layer for informing about the rating done
     *
     * @param  [in] inUserRating , user rating for the current track .
     *
     * @return  None
     *
     *******************************************************************
     */
    void rated(quint8 inUserRating);

//{ added by esjang 2013.05.09 for certificate # 8.3
//    void derated();
//{ added by esjang 2013.05.09 for certificate # 8.3

     /**
     *******************************************************************
     * Signal to QML updating the Album art
     *
     * @param[in] filePath: The album art file path
     *
     * @return  None
     *
     *******************************************************************
     */
    void albumArtReceived(QString filePath);

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


    /**
     *******************************************************************
     * Signal to QML updating the unexpected behaviour
     *
     * @param[in] inError: The error code
     *
     * @return  None
     *
     *******************************************************************
     */
    void handleError(const quint16 inError);

    // { added by esjang 2012.11.20 CAN TP Message
    /**
     *******************************************************************
     * Signal to AppPandora informing the current track title
     *
     * @param[in] title: The current track title
     *
     * @return  None
     *
     *******************************************************************
     */
    //void trackTitleReceived(QString title);
    // } added by esjang 2012.11.20 CAN TP Message


    /**
     *******************************************************************
     * Signal to QML informing the current track is NONE , so clear info
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    void clearScreenonTrackNone(qint16 isPlaying);

public slots:

    /**
     *******************************************************************
     * The result of request from Pandora MW
     * Non network related errors should be handled here
     *
     * @param[in] inRequestType The request for which the reply is
     *                          received
     * @param[in] inPndrError   The result of the request
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnRequestCompleted(const ERequestType inRequestType, const EPndrError inPndrError);

    /**
     *******************************************************************
     * This is the slot to listen to signal from Pandora MW
     * for track update information
     *
     * @param[in] inTrackToken  The token ID for new track
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnTrackUpdated(const quint32 inTrackToken);

    /**
     *******************************************************************
     * This is the slot to listen to signal from Pandora MW
     * for listening to updat of elapsed time for current track
     *
     * @param[in] inElapsedTime elapsed time in sec
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnUpdateElapsedTime(const quint16 inElapsedTime);

    /**
     *******************************************************************
     * This is the slot to listen to signal from Pandora MW
     * for listening to track explain information
     *
     * @param[in] inTrackExplain the track explain information
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnTrackExplain(const QString& inTrackExplain);

    /**
     *******************************************************************
     * This is the slot to listen to signal from Pandora MW
     * for track information
     *
     * @param[in] inTrackInfo the track information
     *
     * @return  None
     *
     *******************************************************************
     */
    void OnTrackInfoReceived(const TSTrackInfo& inTrackInfo);
    /**
     *******************************************************************
     * This is the slot to listen to signal from Pandora MW
     * for track album art
     *
     * @param[in] inAlbumArt the track album art path
     * @param[in] inTrackToken the track token
     *
     * @return  None
     *
     *******************************************************************
     */
     void OnAlbumArtReceived(const QString& inAlbumArt);

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
     * This slot is invoked when any Station Art is received *
     * @param   inStationArt The station Art Object
     *
     * @return  void
     *
     *******************************************************************
     */
   //void OnStationArtReceived(const TSStationArt& inStationArt);

    /**
     *******************************************************************
     * This slot is invoked On device switching to make call flow break  *
     * @param   None
     *
     * @return  void
     *
     *******************************************************************
     */
    void devChanged(){ OnTrackUpdated(m_nTrackToken);}

    /**
     *******************************************************************
     * This slot is invoked When  a station gets deleted  *
     * @param   inStationToken , Deleted station tooken
     *
     * @return  void
     *
     *******************************************************************
     */
    void OnStationDeleted(const quint32 inStationToken);

private:
    USE_LOG_UTIL

    CAppPandoraBase* m_pMainEngine;
    /** Pandora MW interface reference */
    CPndrInterface* m_PndrInterface;
    /** Cache the TSTrackInfo */
    TrackInfo* m_TrackInfo;
    /** Updated track */
    quint32 m_nTrackToken;
    /** Declarative view instance to register the TrackInfo*/
    QDeclarativeView *m_pDeclarativeView;
    /** Signal handler */
    CPndrStateHandler* m_pSignals;
    /** current track plya/pause status  */
    qint16 nPlayPause;
    /** Album art file path */
    QString mAlbumArtFilePath;
    /** Current Station Token */
    quint32 m_nCurrentStationToken;
    /** UI request */
    ERequestType m_eRateReq;
    ERequestType m_eReq;

    bool m_BRImage;//{ modified by yongkyun.lee 2014-10-08 for : Branding - Middleware
    QTimer *m_PlayTimer;//{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control

};

#endif // DHAVN_APPPANDORATRACK_H
