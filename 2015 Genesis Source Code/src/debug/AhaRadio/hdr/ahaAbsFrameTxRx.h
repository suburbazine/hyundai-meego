#ifndef AHAABSFRAMETXRX_H
#define AHAABSFRAMETXRX_H

#include <QVector>
#include <QtGlobal>//han
#include <DHAVN_IPodAppController_CommonVars.h>
#include <DHAVN_IPodAppController_Def.h>

#include <stdio.h>
#include <unistd.h>
#include <QTimer>
#include "ahaCommonUtility.h"
#include "ahaImageUtility.h"
#include "ahaencoder.h"
#include "ahadecoder.h"
#include "ahapredefinedvalues.h"


class CAhaAbsFrameTxRx : public QObject
{
    Q_OBJECT
public:

        CAhaAbsFrameTxRx();
        ~CAhaAbsFrameTxRx();

    virtual EAhaError Init(); //create ipodcontroller instance and establish signal slot connection
    virtual void DeInit();//deinitialize variables created during init

    virtual EAhaError Connect(EDeviceType inDeviceType) = 0;
    virtual void Disconnect() = 0;
    virtual void requestApplicationLaunch() = 0;
    //added..
    virtual void sendData(const QByteArray data) = 0;

    void resetMemberVariables();
    void createTimers();
    void destroyTimers();
    void resetReplyWaitTimer(uint16_t nTransactionID);

    void     processPendingRequest();
    void     sendCommandRequest(uint16_t nOpCode, AHA_QUERY_MODE_T nQueryMode);
    void     encodeAndSendCommand(uint16_t nOpCode, AHA_QUERY_MODE_T nQueryMode);

//    QByteArray     encodingCommand(uint16_t nOpCode, AHA_QUERY_MODE_T nQueryMode);


    uint16_t decodeResponse(QByteArray &byteArray, uint16_t nOpCode, AHA_QUERY_MODE_T nQueryMode);
    uint16_t extractOpCodeFromTransactionTable(QByteArray &byteArray, uint16_t &nOpCode, AHA_QUERY_MODE_T &nQueryMode);
    void     addToTransactionTable(QByteArray &byteArray, AHA_QUERY_MODE_T nQueryMode);
    int      processSinglePackage(QByteArray &byteArray);

    void printAhaClientInfo();
    void resetAhaClientInfo();

    void printActiveStationInfo();
    void resetActiveStationInfo();

    void printActiveContentInfo();
    void resetActiveContentInfo();

    void printPresetStationsInfo();
    void printContentListInfo();

    void startAlbumArtDownload();
    void continueAlbumArtDownload();
    void resetAlbumArtInfo();

    void startPartnerLogoDownload();
    void continuePartnerLogoDownload();
    void resetPartnerLogoInfo();

    void startStationLogoDownload();
    void continueStationLogoDownload();
    void resetStationLogoInfo();

    void startPresetLogoDownload(int nIndex);
    void continuePresetLogoDownload(int nIndex);
    void resetPresetLogoInfo();

    void startRatingImageDownload();
    void continueRatingImageDownload();
    void resetRatingImageInfo();

    void startLikeImageDownload(int nIndex);
    void continueLikeImageDownload(int nIndex);
    void resetLikeImageInfo();
    QString getLikeImageFilePath(int nIndex);

    void cancelBinaryImageDownload(AHA_QUERY_MODE_T nQueryMode);
    void cancelRecievingInterruptQuery(AHA_QUERY_MODE_T nQueryMode);    //wsuk.kim ISV_75730 station_remain

    qulonglong getPresetStationID(int nIndex);//han
    uint8_t getLBSFlag(int nIndex);
    void startPeerSession();
    void pause();
    void stop();
    void play();
    void next();
    void back();
    void rewind15();
    void forward30();
    void voteLike();
    void voteDislike();
    void unvote();
    void logCall();
    void logNavigate();
    void ping(bool bResetCount);


    void queryCurrentStation(bool ignoreStationID = false);
    void queryPresetStationIDs();
    void queryStationContentIDs();
    void queryActiveStationParam();
    int  queryPresetStationsParam(int nStartStationIndex);
    void queryActiveContentParam();
    int  queryContentListParam(int nStartContentIndex);
    void queryCurrentContentDisplayInfo();
    void querySelectStationForPlayback(uint64_t nStationID);
    void querySelectContentForPlayback(uint64_t nContentID);

    AhaClientInfo_T*  getAhaClientInfo();
    AhaStationInfo_T* getAhaStationInfo();
    AhaContentInfo_T* getAhaContentInfo();

    AhaImageInfo_T* getAlbumArtInfo();
    AhaImageInfo_T* getPartnerLogoInfo();
    AhaImageInfo_T* getStationLogoInfo();
    AhaImageInfo_T* getPresetLogoInfo();
    AhaImageInfo_T* getRatingImageInfo();
    AhaImageInfo_T* getLikeImageInfo();

    /* member variable */
    AhaClientInfo_T  m_AhaClientInfo;
    AhaStationInfo_T m_ActiveStationInfo;
    AhaContentInfo_T m_ActiveContentInfo;

    AhaImageInfo_T m_AlbumArtInfo;
    AhaImageInfo_T m_StationLogoInfo;
    AhaImageInfo_T m_PartnerLogoInfo;
    AhaImageInfo_T m_PresetLogoInfo;
    AhaImageInfo_T m_RatingImageInfo;
    AhaImageInfo_T m_LikeImageInfo;

    QTimer* m_ReplyWaitTimer;
    QTimer* m_PingCountdownTimer;
    QTimer* m_PingIntervalTimer;

    QVector<AhaStationBriefInfo_T> m_PresetStationsInfo;
    QVector<AhaContentBriefInfo_T> m_ContentListsInfo;

    QVector<TransactionRecordTable_T> m_TransactionRecordTable;
    QVector<TransactionRecordTable_T> m_PendingTransactionQueue;

    QByteArray m_ReceivedData;
    bool m_bLongReceivedData;
    int  m_nLongReceivedData;

    bool m_bChannelBusy;
    bool m_bLaunchRequested;
    bool m_bInitCompletionReceived;
    bool m_TopProcessPlayStation; //hsryu_0612_update_optimize

    int m_PresetQueryIndex;
    int m_LikeQueryIndex;
    int m_nContentIndexToQuery;
    int m_nPresetStationIndexToQuery;

    uint64_t m_nRequestedStationID;
    uint64_t m_nRequestedContentID;

public slots:

    void onReplyWaitTimeOut();
    void onPingCountdownTimeOut();
    void onPingIntervalTimeOut();
//    void onInitCompletionWaitTimeOut();
    void onDataReceived(QByteArray dataArray);

signals:
    void albumArtProcessFinished(QString &albumArtFilePath);
    void stationLogoProcessFinished(QString &stationLogoFilePath);
    void partnerLogoProcessFinished(QString &partnerLogoFilePath);
    void presetLogoProcessFinished(QString &presetLogoFilePath);
    void ratingImageProcessFinished(QString &ratingImageFilePath);
    void likeImageProcessFinished(QString &likeImageFilePath);

    void ahaAppInitCompleted();
    void ahaAppInitTimeOut();
    void ahaPingTimeOut();
    void ahaSessionClosed();
    void ahaSampleRateChanged(int nSampleRate);
    void ahaSessionOpened();
    void ahaAppNotOpened();
    //void ahaAppDisconnected();

    //signals to state handler when a response is decoded
    void startPeerSessionDecoded(uint16_t nErrorCode);
    void queryStationsDecoded(uint16_t nErrorCode);
    void queryStationContentDecoded(uint16_t nErrorCode);
    void activeStationParamDecoded(uint16_t nErrorCode);
    void presetStationParamDecoded(uint16_t nErrorCode, int nStartPresetStationIndex);
    void activeContentParamDecoded(uint16_t nErrorCode);
    void listContentParamDecoded(uint16_t nErrorCode, int nStartContentIndex);
    void queryAlbumArtDecoded(uint16_t nErrorCode);
    void queryStationLogoDecoded(uint16_t nErrorCode);
    void queryPartnerLogoDecoded(uint16_t nErrorCode);
    void queryPresetLogoDecoded(uint16_t nErrorCode);
    void queryRatingImageDecoded(uint16_t nErrorCode);
    void queryLikeImageDecoded(uint16_t nErrorCode);
    void queryCurrentStationDecoded(uint16_t nErrorCode);
    void changeClientPlaybackDecoded(uint16_t nErrorCode);
    void timeShiftContentDecoded(uint16_t nErrorCode);
    void performActionDecoded(uint16_t nErrorCode);
    void selectStationForPlaybackDecoded(uint16_t nErrorCode);
    void selectContentForPlaybackDecoded(uint16_t nErrorCode);
    void currentContentInfoDecoded(uint16_t nErrorCode);
    void notifyPlaybackOfNewStationDecoded(uint16_t nErrorCode);
    void notifyNewContentPlaybackStateDecoded(uint16_t nErrorCode);
    void notifyNewContentAvailableDecoded(uint16_t nErrorCode, uint64_t nStationID);
    void notifyContentDeletedDecoded(uint16_t nErrorCode);
    void notifyClientStatusUpdateDecoded(uint16_t nErrorCode, uint32_t nConnStatusCode);

public:
    USE_LOG_UTIL
};

#endif // AHAABSFRAMETXRX_H
