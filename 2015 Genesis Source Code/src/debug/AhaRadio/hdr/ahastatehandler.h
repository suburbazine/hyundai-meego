#ifndef AHASTATEafterR_H
#define AHASTATEafterR_H

#include <QVector>
#include <DHAVN_IPodAppController_CommonVars.h>
#include <DHAVN_IPodAppController_Def.h>
#include "ahaUSBFrame.h"
#include "ahapredefinedvalues.h"
#include "ahaBTFrameTxRx.h"

class CAhaStateHandler : public QObject
{
    Q_OBJECT
public:
    CAhaStateHandler();
    ~CAhaStateHandler();

    EAhaError Connect(EDeviceType inDeviceType);      //connect to ipod by calling init ipod function
    int Disconnect();   //deinit ipod

    void processRequest(AHA_REQUEST_TYPE_T nRequestAction);   //will check if nRequestAction is allowed.
    EAhaError setDeviceType(EDeviceType inDeviceType);
    EDeviceType getDeviceType(){ return m_nDeviceType; }

    //hsryu_0502_factory_reset
    void ReadyForClearCache();
    void ClearCacheData();

    void clearCurrentTrackInfo();
    void clearTrackViewDisplay();
    void clearContentImagesDisplay();
    void cancelTrackViewImageDownload();
    void requestApplicationLaunch();

    void createTimers();
    void destroyTimers();
    QTimer* m_AppLaunchWaitTimer;
    QTimer* m_StartPeerDelayTimer;
    QTimer* m_StartPeerResponseTimer;
    QTimer* m_BufferingintervalTimer;    //wsuk.kim 130904 ITS_0182092 repeat buffering
    QTimer* m_QueryResponseTimer;

    QTimer* m_DelayedUnmuteTimer;

    CAhaUSBFrameTxRx* m_AhaUSBFrameTxRx;
    CAhaBTFrameTxRx* m_AhaBTFrameTxRx;
    CAhaAbsFrameTxRx* m_AhaActiveFrameTxRx;

    AHA_PRIMARY_STATE_T m_PriState;

    uint64_t m_nRequestedContentID;

    AhaTSTrackInfo m_CurrentTrackInfo;

    //hsryu_0612_update_optimize
    AhaTSTrackInfo m_PrevTrackInfo;

    void connectDecodedSlots();
    void connectImageRelatedSlots();
    void connectActiveTxRxSlots();

    void disconnectDecodedSlots();
    void disconnectImageRelatedSlots();
    void disconnectActiveTxRxSlots();

    //local function to implement state transition
    //after after get some response

    void queryingPresetStations();
    void queryingContentList();
    void queryingActiveStation();
    void queryingActiveContent(bool bClearTrackDisplay=false);
    void startPeerSession(bool bResetCount, int nDelayTime);
    void selectContentToPlay(uint64_t nContentID);
    void selectStationToPlay(uint64_t nStationID);

    void ShowCurrentLikeDislikeIcon();
    void bufferingTimerstop();  //wsuk.kim 130926 ITS_191339 repeat press play/pause key, occur repeat buffering popup.
    void stopTimers();   // added by Ryu : stop timers at Trackview : ITS0224000

    //youngsam.kwon
    bool m_bPeerConnected;
    int m_PresetQueryIndex;
    int m_LikeQueryIndex;
    QString m_strDislikeUnselectedImageFileName;

    uint8_t m_nLikeSelected;
    uint8_t m_nDislikeSelected;
    uint8_t getCurrentRateState(uint8_t nLikeSelected, uint8_t nDislikeSelected);

public slots:
    void ahaAppInitCompletedSlot();
    void ahaAppInitTimeOutSlot();
    void ahaPingTimeOutSlot();
    void ahaSessionClosedSlot();
    void ahaSessionOpenedSlot();
    void ahaAppNotOpenedSlot();

    //timer slots
    void appLaunchTimeOutSlot();
    void peerDelayTimeOutSlot();
    void startPeerResponseTimeOutSlot();
    void bufferIntervalTimeOutSlot();    //wsuk.kim 130904 ITS_0182092 repeat buffering
    void queryResponseTimeOutSlot();
    void delayUnmuteTimeOutSlot();

    void onAlbumArtProcessFinishedSlot(QString &albumArtFilePath);
    void onStationLogoProcessFinishedSlot(QString &stationLogoFilePath);
    void onPartnerLogoProcessFinishedSlot(QString &partnerLogoFilePath);
    void onPresetLogoProcessFinishedSlot(QString &presetLogoFilePath);
    void onRatingImageProcessFinishedSlot(QString &ratingImageFilePath);
    void onLikeImageProcessFinishedSlot(QString &likeImageFilePath);

    //slots to receive notification from ahausbframe when a response is decoded
    void startPeerSessionDecodedSlot(uint16_t nErrorCode);
    void queryStationsDecodedSlot(uint16_t nErrorCode);
    void queryStationContentDecodedSlot(uint16_t nErrorCode);
    void activeStationParamDecodedSlot(uint16_t nErrorCode);
    void presetStationParamDecodedSlot(uint16_t nErrorCode, int nStartPresetStationIndex);
    void activeContentParamDecodedSlot(uint16_t nErrorCode);
    void listContentParamDecodedSlot(uint16_t nErrorCode, int nStartContentIndex);
    void queryAlbumArtDecodedSlot(uint16_t nErrorCode);
    void queryStationLogoDecodedSlot(uint16_t nErrorCode);
    void queryPartnerLogoDecodedSlot(uint16_t nErrorCode);
    void queryPresetLogoDecodedSlot(uint16_t nErrorCode);
    void queryRatingImageDecodedSlot(uint16_t nErrorCode);
    void queryLikeImageDecodedSlot(uint16_t nErrorCode);
    void queryCurrentStationDecodedSlot(uint16_t nErrorCode);
    void changeClientPlaybackDecodedSlot(uint16_t nErrorCode);
    void timeShiftContentDecodedSlot(uint16_t nErrorCode);
    void performActionDecodedSlot(uint16_t nErrorCode);
    void selectStationForPlaybackDecodedSlot(uint16_t nErrorCode);
    void selectContentForPlaybackDecodedSlot(uint16_t nErrorCode);
    void currentContentInfoDecodedSlot(uint16_t nErrorCode);
    void notifyPlaybackOfNewStationDecodedSlot(uint16_t nErrorCode);
    void notifyNewContentPlaybackStateDecodedSlot(uint16_t nErrorCode);
    void notifyNewContentAvailableDecodedSlot(uint16_t nErrorCode, uint64_t nStationID);
    void notifyContentDeletedDecodedSlot(uint16_t nErrorCode);
    void notifyClientStatusUpdateDecodedSlot(uint16_t nErrorCode, uint32_t nConnStatusCode);

    void OnTimeout_BanRefreshEvent();
signals:

    void ahaSampleRateChanged(int nSampleRate);

    void albumArtProcessFinished(QString &albumArtFilePath);
    void stationLogoProcessFinished(QString &stationLogoFilePath);
    void partnerLogoProcessFinished(QString &partnerLogoFilePath);
    void presetLogoProcessFinished(QString &presetLogoFilePath);
    void ratingImageProcessFinished(QString &ratingImageFilePath);
    void likeImageProcessFinished(QString &likeImageFilePath);

    //signals to be emitted to DHAVN_AppAha
    void Result(const ERequestType inRequestType, const EAhaError inAhaError);
    void TrackInfo(const AhaTSTrackInfo& inTrackInfo);
    void TrackUpdated(const quint32 inTrackToken);
    void AlbumArt(const QString &inAlbumArt);
    void PartnerLogo(const QString &strPartnerLogoPath);
    void RatingImage(const QString &strRatingImagePath);
    void StationLogo(const QString &strStationLogoPath);
    void StationLogo(const uint8_t, const qulonglong, const QString&); // wewill
    void contentListLoaded(QVector <AhaContentBriefInfo_T> &qVectContentListInfo);
    void stationListLoaded(QVector <AhaStationBriefInfo_T> &qVectStationListInfo);
    void LikeImage(const int nIndex, const QString &strLikeImageFilePath);
    void UpdateLikeDislikeIcon(const uint8_t nRateState, const uint8_t nCanUnvote);

    void NoActiveStation();

    void OSDInfoReady(QString &strOSDString);

    //notify that the active content id has changed.
    void notifyActiveContentIDChanged(qulonglong nActiveContentID);
    void notifyPlaybackStateChanged(uint64_t nContentID, uint8_t nPlaybackState);

    void RepeatPressedSkipBack(const bool bRepeatState);   //wsuk.kim ISV_77919 repeat_key
//wsuk.kim no_network
    void NetworkResume();
    void NetworkStatus(const int nNetworkStatus);
//wsuk.kim no_network
    void unmute();

private:
    /** Logger instance */
 //wsuk.kim 130904 ITS_0182092 repeat buffering
    void bufferingIntervalCheckStart(uint8_t play_status);
    uint16_t m_nBufferingCount;
    uint32_t m_nNetworkStatus;
//wsuk.kim 130904 ITS_0182092 repeat buffering

    EDeviceType m_nDeviceType;

    USE_LOG_UTIL
};

#endif // AHASTATEafterR_H
