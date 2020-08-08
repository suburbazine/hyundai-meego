/*
 ******************************************************************
 *        COPYRIGHT  2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :                                   *
 * PROGRAMMER : Vandana Ahuja                                     *
 * DATE       : 21/12/2011                                        *
 * VERSION    : 0.1                                               *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module will be responsible for collecting data from
 *  Aha MW to be displayed in Track view.
 *

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  21/12/2011  0.1 Vandana Ahuja   header file created
 *
 ******************************************************************
 */

#ifndef DHAVN_APPAHATRACK_H
#define DHAVN_APPAHATRACK_H

#include "DHAVN_AppAha_Def.h"
#include "DHAVN_AppAhaDeviceListInfo.h"
#include "ahainterface.h"

#include <DHAVN_LogUtil.h>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#ifdef FRAMEWORK_ENABLED
#include <DHAVN_QRPMClient.h>
#endif

class TrackInfo;
class CAhaStateHandler;

class CAppAhaTrack : public QObject
{
    Q_OBJECT
public:
    explicit CAppAhaTrack(CAhaInterface& m_AhaInterface);
    ~CAppAhaTrack();

    static CAppAhaTrack* getInstance();

    //hsryu_0404_restore_playmode
    void ClearCache();

    void initialize();
    void SetPlayPauseState(int);

    void setStationName(QString stationName){ m_TrackInfo->setStationName(stationName); }

    Q_INVOKABLE void Play();
    Q_INVOKABLE void Pause();
    Q_INVOKABLE void PlayPause();
    Q_INVOKABLE void Skip();
    Q_INVOKABLE void SkipBack();
    Q_INVOKABLE void Forward30();
    Q_INVOKABLE void Rewind15();

    // Request from QML Layer to Rate up the current track
    Q_INVOKABLE void LikeButtonClicked();
    Q_INVOKABLE void DislikeButtonClicked();

    // Request from QML Layer for getting track info for current track
    Q_INVOKABLE void RequestTrackInfo();

    //hsryu_0326_like_dislike
    Q_INVOKABLE void DisplayCurrentLikeDislike();
    Q_INVOKABLE void ClearCurrentLikeDislike();

    // Request from QML Layer for getting play/pause status for current track
    Q_INVOKABLE qint16 TrackStatus(){return m_bPlayPause;} //hsryu_0612_initialize_flag_status_bt_call

    // New station is selected from station list.
    Q_INVOKABLE void ClearTrackInfo();

    // Aquire audio path while connecting aha.
    void SetAudioPathRPM (DeviceInfo::EConnectionType inDeviceType);

    QString GetCurrentTrackTitle();
    QString GetCurrentArtist(); //hsryu_0228 ITS # 0158377
    TrackInfo* getTrackInfo();


    // AV_MODE has changed , Other AV apps currently running So Pause aha . Play aha on FG .
    void HandleAVModeChange();
//wsuk.kim_130628_ITS_0177003 40->60... words
    void setElide60WordsArtistName(const QString & trackInfoArtistName);
    void setElide60WordsTitleName(const QString & trackInfoTitleName);
//wsuk.kim_130628_ITS_0177003 40->60... words
    Q_INVOKABLE void ignoreBufferingByKey(){ m_AhaInterface.m_AhaStateHandler.bufferingTimerstop(); }    //wsuk.kim 130926 ITS_191339 repeat press play/pause key, occur repeat buffering popup.
private:
    void HandleIfError(const EAhaError inAhaError);

signals:
    // Signal to QML Layer for informing about play success
    void playStarted();

    // Signal to QML Layer for informing about Pause success
    void pauseDone();

    // Signal to QML Layer for informing about Skip request failure
    void skipFailed();

    // Signal to QML Layer for informing about update of track
    void trackUpdated();

    // Signal to QML Layer for informing the current track information
    void trackInfoReceived();

    // Signal to QML Layer for informing the current track explaination
    void trackExplainReceived(const QString& inExplain);

    void updateLikeDislikeFilePath(QString currLikeIcon,
                                QString altLikeIcon,
                                QString currDislikeIcon,
                                QString altDislikeIcon);

    // Signal to QML updating the Album art
    void albumArtReceived(QString filePath);
    void partnerLogoReceived(QString filePath);
    void ratingImageReceived(QString filePath);
    void stationLogoReceived(QString filePath);

    void receivingStationShowPopup();    //wsuk.kim TUNE_TRACK

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


    // Signal to QML updating the unexpected behaviour
    void handleError(const EAhaError inError);

    //hsryu_0322_TRANSPORT_CAN
    void trackTitleReceived(QString title);
    //hsryu_0422_can_test
    void clearTpMessage();
public slots:

    /**
     *******************************************************************
     * The result of request from Aha MW
     * Non network related errors should be handled here
     *
     * @param[in] inRequestType The request for which the reply is
     *                          received
     * @param[in] inAhaError   The result of the request
     *******************************************************************
     */
    void OnRequestCompleted(const ERequestType inRequestType, const EAhaError inAhaError);

    /**
     *******************************************************************
     * This is the slot to listen to signal from Aha MW
     * for track update information
     *
     * @param[in] inTrackToken  The token ID for new track
     *******************************************************************
     */
    void OnTrackUpdated(const quint32 inTrackToken);


    /**
     *******************************************************************
     * This is the slot to listen to signal from Aha MW
     * for track information
     *
     * @param[in] inTrackInfo the track information
     *******************************************************************
     */
    void OnTrackInfoReceived(const AhaTSTrackInfo& inTrackInfo);
    /**
     *******************************************************************
     * This is the slot to listen to signal from Aha MW
     * for track album art
     *
     * @param[in] inAlbumArt the track album art path
     * @param[in] inTrackToken the track token
     *******************************************************************
     */
     void OnAlbumArtReceived(const QString& inAlbumArt);
     void OnPartnerLogoReceived(const QString &strPartnerLogo);
     void OnRatingImageReceived(const QString &strRatingImage);
     void OnStationLogoReceived(const QString &strStationLogo);
     void OnLikeImageReceived(const int nIndex, const QString &strLikeImageFilePath);
     void OnUpdateLikeDislikeIcon(const uint8_t nRateState, const uint8_t nCanUnvote);
     void OnRepeatPressedSkipBackReceived(const bool bRepeatState); //wsuk.kim ISV_77919 repeat_key
     QString GetCurrentStationFilePath(){ return m_StationLogoFilePath; }    //wsuk.kim 130902 ISV_90329 to display station art image that tune search.
private:
    USE_LOG_UTIL
    static CAppAhaTrack* _instance;

    // Aha MW interface reference
    CAhaInterface& m_AhaInterface;
    CAhaStateHandler* m_pSignals;

#ifdef FRAMEWORK_ENABLED
    /** QRPMClient  instance */
    QRPMClient           *m_pRpmClientObj;
#endif

    /** Cache the AhaTSTrackInfo */
    TrackInfo* m_TrackInfo;

    quint32 m_nTrackToken;

    quint8 nUserRating;
    qint16 m_bPlayPause;  //hsryu_0612_initialize_flag_status_bt_call
    bool bReceiveTrackInfo;     //wsuk.kim ISV_77919 repeat_key

    /** Album art file path */
    QString mAlbumArtFilePath;
    QString m_PartnerLogoFilePath;
    QString m_RatingImageFilePath;

    //hsryu_0326_like_dislike
    QString m_LikeSelectedFilePath;
    QString m_LikeUnselectedFilePath;
    QString m_DislikeSelectedFilePath;
    QString m_DislikeUnselectedFilePath;

    QString m_CurrentLikeUnSelected;
    QString m_CurrentLikeSelected;
    QString m_CurrentDisLikeUnSelected;
    QString m_CurrentDisLikeSelected;

    QString m_StationLogoFilePath;
};

#endif // DHAVN_APPAHATRACK_H
