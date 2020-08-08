/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef PLAYENGINETASKHANDLER_H
#define PLAYENGINETASKHANDLER_H
#include <QString>
#include <QEventLoop>
#include <QThread>
#include <cstdlib>
#include <DHAVN_IPodController_taskHandler.h>
#include <DHAVN_IPodController_PrivateData.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_cthread.h>
#include <QTimer>
#include <DHAVN_IPodController_eventloopTimer.h>    // added by sungha.choi 2013.04.12
#include <DHAVN_iPodController_Logger.h>
#include <DHAVN_AudioSourceType.h>
#include <DHAVN_GSTAudioRouterInterface.h>
#include <DHAVN_IPodController_playStatusChangeNotification.h>

using namespace ExtendedLingo;

#ifndef COMMON_PLAYTASKHANDLER_INCLUDES
#define COMMON_PLAYTASKHANDLER_INCLUDES
#define RANDOM_FILE  1
#define RANDOM_FOLDER  2
#define RANDOM_OFF  0
#define SCAN_FILE  1
#define SCAN_FOLDER  2
#define SCAN_OFF  0
#define REPEAT_OFF  0   //Repeat is off
#define REPEAT_FILE  1    // Added by pawan 2013.05.28.
#define REPEAT_FOLDER  2   //Repeat folder
#define NO_OF_META_INFO  8    // Added by pawan 2013.05.04.
#define MSTOPPED  0
#define PLAYING  1
#define PAUSED   2
#define MPLAYING 10
#define MPAUSED 11
#endif

#define EXEC_TEST

#if 0
struct SMediaInfo
{
    QString          Title;
    QString          Artist;
    QString          Album;
    QString          Genre;
    QString          fileName;
    int              width;
    int              height;
    struct  CoverArt
    {
        int          type;
        void        *image;
    };
};
#endif

#ifdef ONDEMANDCATEGORY
const QString CurrentCategory = PLAYLISTCATEGORY;
#endif

class CIPodDBIndexFetchTaskHandler;

class CPlayEngineTaskHandler : public CTaskHandler
{
    Q_OBJECT
private:
    USE_LOG_UTIL

public:
        CPlayEngineTaskHandler(QObject* parent, iPodLogger *iLog, int repeatMode=REPEAT_OFF, int randomMode=RANDOM_OFF);    // modified by sungha.choi 2013.04.12
    ~CPlayEngineTaskHandler();
    void InitializePlayEngineTaskHandler();
    bool playE();
    bool PlayiPodFiles(int nTrackIndex=0);
    bool playControlE(QString action,bool nano1gpodcastFlag = false);
    bool pauseForTrackerE();
    bool setRepeatE(int mode);
    bool setShuffleE(int mode);
    bool setRandomMode(int mode);
    bool setScanModeE(int mode);
    bool setRepeatMode(int mode);


    bool isAcceptingTuneDial();

public:
    bool getCurrentPlayingTrackIndex(int& index);
    bool setPosition(int position); // Added by sungha.choi 2013.04.11.
    void emitDurationMilliSecondsE();
    void emitStateChangeForNonIDPS();
    void emitCurrentMediaInfoE(MediaInfo*);

    QString getCurrentFileName();
    int getCurrentIndex();
    bool setCurrentIndexE(int);
    int getCurrentMediaPosition();

    //This function does not reset the position in ipod. It only resets the value of member variable
    //to zero. The m_CurrentMediaPosition will be updated from ipod.
    void resetCurrentMediaPositionVariable();

    int getCurrentMediaDuration()               ;
    void getCurrentMediaInfo(MediaInfo *pMediaInfo)               ;
    void setPlayListE(QList<QString> *playlist);
    bool isPlayListSet();

    int getCurrentPlayStateE();
    int getNextIndex();
    int getNextIndexForConvertedDevice(int nStartIndex);
    int getPrevIndexForConvertedDevice(int nStartIndex);
    int getPrevIndex();
    void setAutoPlayIndex(int);
    int getAutoPlayIndex();
    void resetCurrentMediaInfo();

    void setPauseState();
    void setPlayState();
    /* Stops the current task */
    void stopCurrentTask();
    /* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
   * This can be called if the task handler is not required to operate.
   * This will improve the performance.
   */
    void stopTaskHandler();
    int  getMCurrentIndex();
    void setm_IsSyncInProgress(bool,int);
    bool getm_IsSyncInProgress(){return m_IsSyncInProgress;} //added by junam 2013.04.24 for Prev() in sync state
    void setAutoPlaylistE();
    int  GetScanMode();
    void scanEmition(QString action);
    bool createConnection(QString str); //Added by jonghwan.cho@lge.com	2013.03.27.
    bool isReadyToDelete(); //Added by sungha.choi 2013.04.12
    bool requestData(ETrackerAbstarctor_Query_Ids queryId, uint reqUID, int playlistIndex, int startIndex, int nSize); //Added by junam 2013.04.13 for AutoStart
    void GetMetadataFromDatabase(int index, QStringList &item);
    int  getTotalNumberOfTracks() {return m_NumberOfPlayingTracks;} //Added by junam 2013.04.24 for Prev() in sync state
    void executeOtherMPIndexChangeEvent(CPlayStatusChangeNotification& cmd);
    void getCurrentUIDTrackInfoAndUpdateHMI();
    void updateCurrentIndexTrackInfoAndUpdateHMI(int index);
    void responseToTuneDialRequest(int index, ETrackerAbstarctor_Query_Ids queryId, uint reqUID);
    void executeTrackIndexChangeEvent(CPlayStatusChangeNotification& cmd);
    void executeTrackUIDChangeEvent(CPlayStatusChangeNotification& cmd);
    void executePlaylistChangeEvent(CPlayStatusChangeNotification& cmd);
    void executePositionChangeEvent(CPlayStatusChangeNotification& cmd, int caseID);
    void executeStateChangeEvent(int state);
    void setPlayListCategoryItems(QString category="Playlist", int firstDepth=-1, int secondDepth=-1);
    int startAutoplay(CiOSAppTaskHandler* iOSTH);

    void RemoveDuplicateQueueType(RequestType_T nRequestType);
    int GetLastQueueOfThisType(RequestType_T nRequestType);
    void GetPBTrackInfoWithQueue(RequestType_T nRequestType, int index,
                                 ETrackerAbstarctor_Query_Ids queryId, uint reqUID, bool bAppend=true);
    void QuitPBLoopAndProcessNextRequest();
    QList<PBTrackInfoRequest_T> m_PBTrackInfoQueue;
    PBTrackInfoRequest_T m_PBRequestInProgress;


private:
    int  m_nRunningLoopFromSignal;
    bool m_ReadyToDeleteFromSignal;
    void setRemoteEventNotificationsE();
    void setUIDPlayListE(QList<QString> *listUris);
    int getMaxUIDsPerCmd();
    void getIndexAndUID(QString filePath, int& index, quint64& uid);
    void playCategoryTrack(QString);
    void playCurrentSelection(int);

    bool isAudioBookPodcast(quint64 uidval=0);

    void handlePlaybackStopped();

public:
    int waitForResponceFromExitCode(int timeOut, int nExitCode);
    void delayMetadataIndexQuery(int nDuration=1000);

private:
    void AcknoledgeTheIPod(int commandID, int Status);
    int isWaitingForResponseFromSignal();
    void clearPipeline();
    void doClearPipeline();
    bool isWaitingForResponse();    // added by sungha.choi 2013.04.12
    QString getFilePathFromDBUsingUID(quint64 uid);
    void createFilePath(const QString& songType, const quint64 uid);   // Added by pawan 2013.05.04.
    bool getlistofPlaylistsQuery(QString category, QVector<QStringList> &dataList, int startIndex, int size);
    bool getAllSongsbyPlaylistQuery(QString category, QVector<QStringList> &dataList, int playlistIndex, int& StartIndex, int& size);
    void getOnDemandMetaInfoForIndexDevice(int index, bool onlyUpdateToHMI = true);
    void delayGetOnDemandMetaInfoForIndexDevice();

    void RetryFailedPBTrackInfoRequest();

public slots:
    virtual void packetAvailable(int lingoID,int commandID,const QByteArray& buf );
    void authorisationFinished(bool);
    void startPipeline();
    void setPcmSource(QString source);
    void scanPlaylist();
    void getPlayStatusATS();
    void Notified();
    void onCurrentTrackCountChangedSlot(int count);  // Modified by pawan 2013.05.28.
    void CurrentUIDMediaInfoRetrievedSlot();  // Added by pawan 2013.05.04.
    void CurrentPBMediaInfoRetrievedSlot();
    void stopPipe(); // Added by jonghwan.cho@lge.com 2013.05.19.
    int playNext();

    void handlePlayIpodFilesSlot();
    void handlePlaybackStoppedSlot();
    void delayMetadataIndexQuerySlot();
    void delayGetOnDemandMetaInfoSlot();
    void PBTrackInfoTimeOutSlot();

signals:
    void mainLoopQuit(int nExitCode);
    void mainLoopQuit();
    void OnPositionChanged(int);
    void OnStateChanged(int);
    void OnError(int);
    void OnScanCompleted();
    void OnTrackChanged(int index);
    void OnModeChanged(int index);
    void OnDurationChanged(int mseconds);
    void OnCurrentMediaInfo(MediaInfo* info);
    void fetchCoversForBasicPlayer(QString albumName, QString artistName, bool forceFlag=false);
    void RequestComplete( QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, int offset, uint reqUID );
    void RequestComplete( QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID );
    void OnCurrentPlayingTrackCount(int); // Added by pawan 2013.05.03.
    void CurrentUIDMediaInfoRetrieved();  // Added by pawan 2013.05.04.
    void CurrentPBMediaInfoRetrieved();
    void OnChangeDBIndexingStatus(bool pause);

public:
    bool m_FirstTimePlay;
    QList<QString> m_PlaylistURIs;
    //CR10930 : Making it public as it will be accessed from outside
    int m_CurrentIndex;
    int m_NumberOfPlayingTracks;//added by junam 2013.04.13 for AutoStart
    int m_NumberOfPlayingTracksReceivedFromIPod;
    int m_newNumberOfTracks;  // Added by pawan 2013.05.15.
    QList<int> m_ControllerPlaylist;
    int m_CurrentPlaylistIndex;
    int getNumberOfPlayingTracks();   // Added by pawan 2013.05.06.

private:
    void setStatusChangeNotificationsE(bool dowait = true);

private:
    enum EPlayStatusNotifyState {
        eDefault = 0,
        eStateNotifySent,
        eStateNotifyReceived
    };
    // Added by jonghwan.cho@lge.com for iOS 6.1 2013.04.16.
    enum ERemoteEventNotifyState {
        eEventNotifyDefault = 0,
        eEventNotifySent,
        eEventNotifyReceived
    };

private:
    bool m_bDBSelectionHasChanged;
    ERemoteEventNotifyState   m_EventNotifyState;
    bool m_CommandStatus;
    bool m_StateNotificationSet;
    bool m_IsGetCurrentFileName;
    int m_CurrentMediaPosition;
    int m_CurrentMediaDuration;
    bool m_bCurrentTrackHasChapter;
    int m_nIPodReturnedMediaDuration;
    QString m_CurrentFileInfoString;
    MediaInfo m_MediaInfo;
    MediaInfo m_TuneMediaInfo;

    int m_nMetadataRetryCount;

    quint64 m_PBRetrievedUID;
public:
    int m_ScanMode;
    bool isCurrentTrackHasChapter();
    void startPlayIpodFilesTimer();
    void stopPlayIpodFilesTimer();
private:

    QTimer* m_PlayIpodFilesTimer;
    QTimer* m_HandleStopTimer;
    QTimer* m_DelayMetadataIndexQueryTimer;
    QTimer* m_PBTrackInfoTimer;

    QTimer* m_DelayGetOnDemandMetaInfoTimer;

    QString m_ControllerPlaylistType;
    bool m_TopLevelSelected;
    int m_TopLevelIndex;
    QEventLoop *m_pLoop;    // added by sungha.choi 2013.04.12
    CEventloopTimer *m_pWaitTimer;
    bool m_isRunningLoop;   // added by sungha.choi 2013.04.13
    bool m_emitStateForNONIDPS;
    QByteArray m_CommandBuffer;
    int m_RandomPlayCount;
    int m_ScanPlayCount;
    int m_AutoplayIndex;
    bool isNotPassOnToNextSong;
    bool m_AuthenticationPassed;
    bool m_isDisconnected;
    bool m_isDurationRequested;
    int m_CurrentIndexEmitted;

    int m_SampleRate; ///* AudioPipeline related variables */
    bool m_audioConnected;
    QString m_audioSource;
//    bool m_bPipeCreated;
//    CPipeThread *m_pipeThread;
    iPodControllerPrivateData* m_iPodPrivateData;
    bool m_bPipeCreated;
    int m_SetPlaylistCount;
    bool m_IsSyncInProgress;
    int m_SyncType;
    int m_FirstPositionChangeCount;
    bool m_scanFlag;
    QTime m_scanPauseTimer;
    int m_scanElapseTime;
    bool m_pauseScanTimeFlag;
    bool m_SentOneByteForm;
    qint16 m_LocalTransId;
    QDateTime m_CurrentTime;

    EPlayStatusNotifyState m_PlayStatusNotifyState;
    int m_CurrentCommand;
    QTimer* m_PlayEngineTimer;
    int m_ExpectedCommandID;
    CIPodDBIndexFetchTaskHandler* m_iPodDBIndexFetchTaskHandler;
    bool m_toDelete;

    iPodLogger *piPodLog;    // added by sungha.choi 2013.04.12
    bool m_ReadyToDelete;    // added by sungha.choi 2013.04.12
    EAudioSource m_eCurrentSrcType;
    QList<QByteArray>          m_MetaDataArray;   // Added by pawan 2013.05.04.
    QList<QByteArray>          m_TuneMetaDataArray;   // Added by pawan 2013.05.04.
    QString m_FirstTimePlayFileName;              // Added by pawan 2013.05.04.
    int m_PlaylistCount; // Added by wooju.lee@lge.com 2013.05.23.
    int m_CurrentTrackIndexReceived;    // Added by pawan 2013.05.28.
    int m_PlayNextSongOnFF;

    bool m_IsPlaylistSetByDH;
    bool m_PreparePlaylistStatus;



    QList <QByteArray> m_ControllerPlayListUID;
    bool m_IsPlayListSpecificCategoryIsSet;
    QString m_CategoryType;
    quint64 m_CurrentPlayingUID;

    GSTAudioRouterDBus* m_pAudioRouter;

public:
    bool m_bCoverArtUpdated;

    int m_CurrentIndexToPlay;
    bool IsPlaylistSpecificCategoryIsSet();
    int m_RandomMode; // Modified by wooju.lee@lge.com 2013.06.04.
    int m_RepeatMode;
    bool m_JumpInProgress;
    int m_playState;
    int m_playStateNotificationFromIpod;
    bool m_playControlStopSeekFlag; // Added FF End functionality for classic
    QSqlDatabase	dBHandle;

signals:
    void dbChangedAfterDeletion(); // Added by pawan 2013.05.15.
};

#endif // PLAYENGINETASKHANDLER_H
