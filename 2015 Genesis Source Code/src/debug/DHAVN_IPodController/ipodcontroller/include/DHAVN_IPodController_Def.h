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
 *                 Main Controller interface class which will be  *
 *                 used by the HMI, This class is the main        *
 *                 controller class and will create and handle    *
 *                 the functionality of all task handlers         *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef IPODCONTROLLER_H
#define IPODCONTROLLER_H

#include <QWidget>
#include <imediainterface.h>
#include <DHAVN_TrackerAbstractor_constants.h>
#include <DHAVN_TrackerAbstractor.h>
#include <QObject>
#include <DHAVN_LogUtil.h>
#include <DHAVN_AudioSourceType.h>
#include <QProcess>
#include <fcntl.h>

//#define ENABLE_TEST_BUG_PORT_RESET

// Task Handler
class CIdentifierTaskHandler;
class CAuthorisorTaskHandler;
class CPlayEngineTaskHandler;
class CIPodDBFetchTaskHandler;
class CRemoteUIModeTaskHandler;
class CArtworkFetchTaskHandler;
class CiTunesTagTaskHandler;
class CiOSAppTaskHandler;

// for Packet Processing
class CHIDRawPacketReader;
class CHIDRawPacketWriter;

class CIAPPacketWriter;
class CIAPPacketReader;

// Thread
class iAPThread;
class CiPodDBSyncThread;

class CUsbConfigurationSetter;

// for Detecting Device
class CDeviceDetector;
class CAudioDeviceDetector;
class CCoProcessorDataProvider;

// for Tracker
class CTrackerAbstractor;
class CTrackerConnector;

class iPodControllerPrivateData;

enum RequestType_T
{
    RequestType_None,
    RequestType_MetaDataIndex,
    RequestType_TuneDial,
    RequestType_MetaDataUID
};

typedef struct _PBTrackInfoRequest_T{
    RequestType_T nRequestType;
    ETrackerAbstarctor_Query_Ids queryId;
    int nRequestIndex;
    uint reqUID;
}PBTrackInfoRequest_T;


// HMI event Queue
//{
enum EHMIEvent
{
    EIdle = 0,
    EPlayEvent,         //1
    EPauseEvent,        //2
    EStopEvent,         //3
    ENextEvent,         //4
    EPrevEvent,         //5
    ERandomEvent,       //6
    ERepeatEvent,       //7
    EFFwdEvent,         //8
    ERwdEvent,          //9
    ESetPlaylistEvent,  //10
    ESetCurrentIndex,   //11
    ESetMediaPosEvent,  //12    // added by sungha.choi 2013.04.19
    ERequestDataEvent,   //13
    EPlayiPodFilesEvent
};
class CIpodControllerQueueData;
//}


//#ifdef DBUSBASEDAPPHANDLER
class CIPodControllerDBus_Initializer;
//#endif

enum EIPodErrors
{
    IPOD_ERROR_ALREADYWAITING = -7,
    IPOD_ERROR_TIMEOUT,
    IPOD_ERROR_NOTRESPONDING,
    IPOD_ERROR_UNSUPPORTED,
    IPOD_ERROR_DISCONNECT,
    IPOD_ERROR_UNKNOWN,
    IPOD_ERROR_GENERIC,
    IPOD_SUCCESS = 0
};

#ifndef IPODUSBPORTNUMBERS
#define IPODUSBPORTNUMBERS
enum EIPodPorts
{
    eIPOD_PORT_NONE = 0, // Default, means single ipod supported
    eIPOD_PORT_1 = 1,    // This Controller is dedicated for iPods connected to usb port 1
    eIPOD_PORT_2 = 2     // This Controller is dedicated for iPods connected to usb port 2
};
// { modified by jungae 2012.12.18 for LoadingData popup does not disappear problem
// iPod port number was miss matched between AVP and UISH.
// Port number is changed by UISH, so same has to be reflect IPodController.
extern QString iPodUsbPort1;  //DH: "2-2", KH/VI: "1-1"
static const QString iPodUsbPort2( "2-1" );
// } modified by jungae
// DH Front USB Path: /sys/devices/pci0000:00/0000:00:17.0/0000:01:00.0/0000:02:08.3/usb2/2-2
// KH/VI Front USB Path: /sys/devices/pci0000:00/0000:00:17.0/0000:01:00.0/0000:02:02.1/usb1/1-1
// DH/KH/VI Rear USB Path: /sys/devices/pci0000:00/0000:00:17.0/0000:01:00.0/0000:02:08.3/usb2/2-1/2-1.2

#endif //IPODUSBPORTNUMBERS

enum EIPodPopUp
{
    iPodiTunes_NONE = 0,
    iPodiTunes_SENDINGTAGS,
    iPodiTunes_SUCCESS,
    iPodiTunes_FAILED,
    iPodiTunes_MEMORYFULL,
    IPOD_POPUP_UNSUPPORTED,
    IPOD_POPUP_CONNECTION_ERROR
};

// Added for idedtify retry(CR 13200), this value can be adjusted depending on user scenario...
#define IDENTIFY_RETRY_TIMEOUT      3000    // 3000ms
#define IDENTIFY_RETRY_MAX_COUNT    40      // 40 * 3000min = 120000ms = 120sec = 2min
#define RESTART_UCS_MAX_COUNT       5       // added by sungha.choi 2013.03.07

#define LOGH LOG_HIGH << metaObject()->className() << "::" << __func__ << "()" <<

class CIpodController : public QObject,public IMediaInterface
{
    enum EControllerState
    {
        eStateIdle = 0,      	/* 0. eIdle is iPod disconnected state*/
        eStateiPodDetetcted, 	/* 1. eiPodDetetcted, detection is complete and HID is ready.*/
        eStateiAPReaderReady,   /* 2. eStateiAPReaderReady, when iAp is reader is ready */
        eStateiAPWriterReady,   /* 3. eStateiAPWorkerReady, when iAp is worker is ready */
        eStateiAPReady,         /* 4. eiAPReady, iAP reader/writer are ready. iAP channel is up.*/
        eStateIdentified,       /* 5. eIdentified, Identification is complete*/
        eStateAuthorized,       /* 6. eAuthorized, autherization is complete  */
        eStateiPodInitialized,  /* 7. eStateiPodInitialized, i,e both identication and autherization are completed */
        eStateEnteredInUIMode,  /* 8. eEnteredInUIMode, iPod is succesfuly place in Extented mode */
        eStateiPodReady,        /* 9. eiPodReady, Post extended mode initialization is done and ready for Play and DB fuctionality. */
        eStateAutoPlayReady,     /* 10. eStateAutoPlayReady, AutoPlay may now be unblocked. Sync will continue parallely*/
        eStateReadyToAutoPlayReady
    };

    enum EDeviceState
    {
        eDisconnected = 0,
        eConnected
    };

    Q_OBJECT
    //CR15593
private:
    USE_LOG_UTIL
public:
    CIpodController(QObject* parent = 0, EIPodPorts portNum = eIPOD_PORT_NONE, int nVehicleType=-1);//By default single ipod supported.
    ~CIpodController();
    //void start(void); // deleted by jungae, 2013.01.26 for ISV70478 side effect
    static CIpodController* GetInstance(void);
    
    static int          m_instanceCount; // added by jungae, 2013.01.26 for ISV70478 side effect
    static int		    m_ConnectionDeviceNum;

    virtual int         ChangeMediaMode(MEDIA_PLAYER_MODE eMode); //Not used in iPod Currently
    virtual QString     GetCurrentFileName()                    ;   // IPOD
    virtual int         GetCurrentIndex()                       ;   // IPOD    
    virtual void        SetCurrentIndex(int nIndex)             ;   // IPOD    
    virtual int         GetCurrentMediaState()                  ;   // IPOD    
    virtual int         GetCurrentMediaDuration()               ;   // IPOD    
    virtual int         GetCurrentMediaPosition()               ;   // IPOD    
    virtual void        SetCurrentMediaPosition(int position)   ;   // X    
    virtual int         GetDeviceState()                        ;   // X  
    virtual int         GetControllerState();

    virtual void        GetCurrentMediaInfo(MediaInfo *pMediaInfo) ;   // 
    virtual void        GetDeviceInfo(DeviceInfo *pDeviceInfo)     ;   // X  
    virtual void        GetPlaylist(PlayList *pPlaylist)           ;   //   

    virtual void        SetPlayList(struct PlayList*)           ;   // 
    virtual bool        SeekableMedia()                         ;   // 
    virtual void        SetBrightness(double)                      ;   // 
    virtual void        SetContrast(double)                        ;   // 
    virtual void        SetTint(double)                            ;   // 
    virtual void        SetBrightness(MEDIA_DISPLAY displayPlane, double value);
    virtual void        SetContrast(MEDIA_DISPLAY displayPlane, double value);
    virtual void        SetTint(MEDIA_DISPLAY displayPlane, double value);
    virtual double      GetBrightness()                         ;   // 
    virtual double      GetContrast()                           ;   // 
    virtual double      GetTint()                               ;   //
    virtual void        SetSystemState(int)                     ;   // 
    virtual QWidget*    GetVideoWidget()                        ;   // 
    virtual void        EnableSubTitle(bool bEnable)            ;
    virtual void 	    SetPlayerEngine(PLAYER_ENGINE_TYPE)	;
    virtual void        SetFontSize(const int fontSize)         ;
    virtual bool        ClearDisplay(bool isBackground)         ;

    virtual void        SetHue(double value)                    ;
    virtual void        SetHue(MEDIA_DISPLAY displayPlane, double value) ;
    virtual double      GetHue()                                ;
    virtual void        GetCurrentFileFolderIndex(int *pFileIdx, int *pTotalFiles) ;
    virtual void        AudioDragStart();      // Added by jonghwan.cho@lge.com 2013.06.10. for progress bar sound is broken.
    virtual void        AudioDragEnd();       // Added by jonghwan.cho@lge.com 2013.06.10. for progress bar sound is broken.

    //Temporary solution for list display - TO BE REMOVED ONCE TRACKER SOLUTION IS IN PLACE
    SAppleInfoList      GetIpodDBTracksInfo();
    int                 GetTotalNumberOfTracksOnAutoPlay();
    QString             getiPodSerialNumber();
    void                setTrackerAbsObject(CTrackerAbstractor* abs);
    bool                requestData( ETrackerAbstarctor_Query_Ids queryId, uint reqUID, int playlistIndex, int startIndex=-1, int nCount=-1 );

    bool isAcceptingTuneDial();

private:
#ifdef ENABLE_TEST_BUG_PORT_RESET
    QTimer* m_pPortResetTimer;
#endif

    QTimer* m_delayNextPrevTimer;
    QTimer* m_disableTuneOnSetIndexTimer;

    QTimer* m_resetOSDKeyTimer;

    bool m_bDelayNextPrev;


    void delayNextPrevExecution(int nDelay);
    void delayResetOSDKey(int nDelay);
    void disableTuneForFewMiliSecond(int nMiliSecond);
    void ReplaceEmptyMediaInfo(MediaInfo *pMediaInfo);

	//Tan. Added these functions to make sure AVP do not 
	//call IPod Functions directly. It ensure that AVP
	//logic is not hold by waitForResponce.

    void PlayiPodFilesFromQueue();

    void PlayFromQueue();
    void PauseFromQueue();
    void StopFromQueue();
    void NextFromQueue(int nSkipLevel);
    void PrevFromQueue(int nSkipLevel,int nForced = 0);
    void RandomPlayFromQueue(int mode, int AutoPlayFlag = 0 );
    void RepeatPlayFromQueue(int mode, int AutoPlayFlag = 0 );
    void FastForwardFromQueue(int rate);
    void RewindFromQueue(int rate);
    void SetCurrentIndexFromQueue(int nIndex);
    void SetCurrentMediaPositionFromQueue(int position);
    void SetPlayListFromQueue(PlayList *);
    bool requestDataFromQueue(ETrackerAbstarctor_Query_Ids queryId, uint reqUID, int playlistIndex, int startIndex, int nCount);
private slots:

    void delayNextPrevExecutionSlot();
    void ResetOSDKeyTimeOutSlot();
public slots:

#ifdef ENABLE_TEST_BUG_PORT_RESET
    void portResetTimerTimedOutSlot();
#endif
    virtual void        Play()                                  ;   // IPOD
    virtual void        Pause()                                 ;   // IPOD
    virtual void        FunctionStop()                          ;
    virtual void        Stop()                                  ;   // IPOD
    virtual void        Next(int nSkipLevel)                    ;   // IPOD
    virtual void        Prev(int nSkipLevel,int nForced = 0)                    ;   // IPOD
    virtual void        Scan(int mode)                          ;   // X
    virtual void        RandomPlay(int mode, int AutoPlayFlag = 0 )                    ;   // IPOD
    virtual void        RepeatPlay(int mode, int AutoPlayFlag = 0 )                    ;   // IPOD
    virtual void        FastForward(int rate)                   ;   // IPOD
    virtual void        Rewind(int rate)                        ;   // IPOD
    virtual void        NextFolder(struct PlayList *)           ;   // X
    virtual void        PrevFolder(struct PlayList *)           ;   // X

    void postExtendedModeInitDoneEC(bool status);//iPod is ready for sync and playback
    void startAutoPlay(bool,int);
    void fetchCoverArtFromIPodSlot(QStringList, QStringList, QString);
    void OnPositionChangedSlot(int msec);
    void startIdentification(bool type);

    void start(void); // added by jungae, 2013.01.26 for ISV70478 side effect
    void onIPodDeviceConnectedSLOT( QString, QString, EIPodPorts );

#ifdef CONNECT_BOOTUP
    void IPodConnectStateForBootupSlot(bool);
#endif

    void OnDBIndexingStatusChanged(bool pause);

    void changeDBIndexingStatus(bool pause);
    void stoppedSlot( int exitCode, QProcess::ExitStatus exitStatus );

public slots:
    void readerThreadStarted();
    void writerThreadStarted();

private:
    int convertPlayerEnginePlayState(int nPlayerEnginePlayState);
    void setTrackBufferForOSD(int isPrev);
    int  InitializeController();
    int  startiAPChannel(QString hidDevPath);
    void cleariAPChannel();
    void startiPodInitialization();
    int  startPostExtModeInitEC();

    int  stopTaskHandlers();
    int  destroyTaskHandler();
    void updateDigitalAudioPipeline(int);
    void updateHMIForBasicPlayerCoverArt(QString coverURL);

    void SetOSDCondition();
    void ResetOSDConditionAtSongEnd(int msec);

    //{ HMI event Queuing
    // The function which are using this functionality must not return
    // inbetween and must complete both the calls mentioned below.
    bool QueuingEvent(EHMIEvent newEvent, CIpodControllerQueueData& queueData);
    void ExecuteAlreadyQueuedEvent();
    //}

    void PerformScanCheck(int msec);

    void cleanDirectory(QString &directoryPath);

    QString getCategoryType(int category);

signals:
    void connected();
    void disconnected();
    void authorised(bool status);
    void Identified(bool status);
    void iPodReady(bool status);
    void autoPlayReady(bool);     // added by pawan 2013.04.30.
    void IPODDisConnected(QString,int);
    void disconnectCompletion();    // added by sungha.choi 2013.04.02
    void onIPodConnectedSignal( QString, QString, EIPodPorts, bool );


    void closeFileDescriptorSignal();



signals:    
    void OnPositionChanged(int);
    void OnDurationChanged(int);
    void OnStateChanged(int);
    void OnError(int, QString);
    void OnScanCompleted();
    void OnTrackChanged(int index);
    void OnAutoTrackChanged(int index); //emit index -1 to center the focus on the current playing item.
    void OnModeChanged(int index);
    void OnCurrentMediaInfo(MediaInfo*);
    void OnCurrentMediaStatusChanged(int);
    void IPODConnected(QString,int,int,int,int);
    void IPodPopUp(EIPodPopUp);
    void IPodTotalNumTracks(int);
    void OnCurrentTrackCountChanged(int);//added by junam 2013.04.23 for index DB
    void OnCurrentPlayingTrackCount(int); // Added by pawan 2013.05.01. 
    //void IPodPlayableSongCount(int,int);
    void coverArtFetched(QStringList);
    void OnGetDeviceName(QString); // added by jungae 2012.08.16 to get the iPod device name
    void SystemStateStatus(int); //Added for CR14001
    void RequestComplete( QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, int offset, uint reqUID );

    // Added by jonghwan.cho@lge.com   2013.05.21. for REW/FF Click
    void OnNormalPlay();
    void OnTrackBuffered( int, int, int, int ); // Added by jonghwan.cho@lge.com 2013.05.24. OSD Issue

    void OnRetryCoverForOtherMP(QString, QString);

    //Signal to tell AVP to enable or disable the basic controls.
    //enable = true -> if Main music player is playing in IPOD
    //enable = false -> if other music apps are playing in IPod
    void OnBasicControlEnableStatusChange(bool enable, QString AppName);
    void OnChangeDBIndexingStatus(bool pause);
    void mainLoopQuit(int);
    void emitDBIndexingChangeStatus(bool pause);

#ifdef SIRI_TIMEOUT_TEST
    void NotifyPlayStateForSiri();
    void LaunchedDefaultMusicApp( bool enable, QString AppName );
#endif
public slots:
    void restartUcs();  // added by sungha.choi 2013.03.07
    void CheckIpodConnectionStatusSlot5();
private slots:
    void OnCurrentMediaInfoSent(MediaInfo * mediaInfo);
    void iPodConnectedSlot(int vendorId, int productId);//iPod just connected but iAp is not ready.
    void detectionProcessFinished(bool status, QString hidDevPath);//iPod Connected and changed to iAP USB mode.

    void iAPWriterReady(bool status);//iAP writer is ready.
    void iAPReaderReady(bool status);//iAP reader is ready.

    void IdentificationProcessFinished(bool finished);// Identification process is finished.
    void authorisationProcessFinished(bool finished);//Authorized.
    void extendedModeSwitched(bool status);//Extended mode switched.

    void disconnectSignalCaught();
    void OnStateChangedSlot(int);
    void OnErrorSlot(int);
    void OnModeChangedSlot(int index);
    void onDBSyncThreadFinish();//Cleanup after thread finish.
    void onCoverArtFetchSlot(QStringList);
    void fetchCoversForBasicPlayer(QString albumName, QString artistName, bool forceFlag);
    void gotCoversForBasicPlayerFromTracker(QVector<QStringList> list,ETrackerAbstarctor_Query_Ids id);
    void gotCoversForBasicPlayerFromIPod(QStringList list);
    void ExecuteAlreadyQueuedEventSlot();
    //Fixed: CR15102, CR16762
    void catchOnStateChangeSignal(int state);
    void destroyTaskHandlerSlot();  // added by sungha.choi 2013.04.12
    void startUcs();    // added by sungha.choi 2013.04.12
    void OnCurrentPlayingTrackCountSlot(int);   // Added by pawan 2013.05.01.
    void EmitScanComplete();

    void RetryCoverForOtherMP(QString, QString);

    QString GetControllerStateString();
    QString GetPlayStateString(int nPlayState);
    QString GetnPlayStateString(int nPlayState);

public:
    void OnCurrentMediaStatusChangedSlot();
    void iPodReadySlot();
    int  getSortingOrder();//added by junam 2013.06.07 iPod quick scroll
    bool ExcuteGSTAudioRouterWrapperProcess();
    QStringList GetArguments();
    bool setManuallyAudioDevicePath(int);

    void CheckIpodConnectionStatus();
    void resetCurrentIPodPort();
public :
    bool m_IsIDPS;
    bool m_isLogoSupported;
    int  m_PlayState;
    bool m_startNotifierFlag; // added by jungae 2013.01.23, for ISV70478

    CIAPPacketWriter* m_Writer;
    CIAPPacketReader* m_Reader;

    /* for GSTAudioRouterWrapper communication */
    QString       m_AudioSource;
    int           m_SampleRate;
    EAudioSource  m_eCurrentAudioSourceType;
    QProcess*     m_pGSTAudioRouterWrapperProcess;
    /* for GSTAudioRouterWrapper communication */

    int m_nScanStartPosition;
    CiOSAppTaskHandler*        m_iOSAppTaskHandler;

    void PlayiPodFiles();

    void SetIsAhaPandoraMode(bool isAhaPandora);
private:

    CIdentifierTaskHandler*    m_Identifier;
    CAuthorisorTaskHandler*    m_Authorisor;
    CPlayEngineTaskHandler*    m_PlayEngineTaskHandler;
    CIPodDBFetchTaskHandler*   m_ITunesTaskHandler;
    CRemoteUIModeTaskHandler*  m_RemoteUiTaskHandler;
    CArtworkFetchTaskHandler*  m_artworkFetchTaskHandler;
    CiTunesTagTaskHandler*     m_iTunesTagTaskHandler;
    CTrackerAbstractor*        m_TrackerAbs;



    CDeviceDetector*           m_Detector;

    CIPodControllerDBus_Initializer* m_DBusInitializer;
    /*
    CIAPPacketWriter* m_Writer;
    CIAPPacketReader* m_Reader;
*/
    EControllerState           m_ControllerState;
    EDeviceState               m_DeviceState;

    //this flag is used to ensure that startiPodInitialization
    //is only called once!
    //NOTE: StartIPodInitialization will called
    //after both reader writer thread started.
    //So, set this flag to false before starting the thread.
    //set this flag to true  after calling startiPodInitialization function.
    bool m_bStartiPodInitializationFlag;
    bool m_bReaderThreadStartedFlag;
    bool m_bWriterThreadStartedFlag;

    CCoProcessorDataProvider*  m_DataProvider;
    static CIpodController*    m_pInstance;

    CTrackerConnector*         m_TrackerConnector;
    iPodControllerPrivateData* m_PrivateData;

    /* Thread references*/
    iAPThread*                 m_iAPReaderThread;
    iAPThread*                 m_iAPWriterThread;

    CiPodDBSyncThread*         m_iPodDBSyncThread;

    bool     m_IsPrev;
    int      m_vendorId;
    int      m_productId;
    bool     m_DBDeleteSyncFlag;
    int      m_FirstTimeiPodConnectedCount;
    int      m_PrevPlayState;
    int      m_scanMode;

    int      m_restartUcsCount;  // added by sungha.choi 2013.03.07
    bool     m_IsSendedFirstConnectionSignal;
    bool     m_bWaitForFirstConnectionSignal;

    // Removed by pawan 2013.05.04.
    //bool     m_FirstPlayListSet;  // Added by pawan 2013.05.01.
private slots:   // Added by pawan 2013.05.15.
    int ondbChangedAfterDeletion(); // Added by pawan 2013.05.15.

    void trackerConnectorDestroyedSlot();
    void dbThreadDestroyedSlot();

};
#endif //IPODCONTROLLER_H
