/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Sowmini / Venkat                                 *
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

#ifndef IPODCONTROLLER_PRIVATEDATA_H
#define IPODCONTROLLER_PRIVATEDATA_H

#include <DHAVN_IPodController_Def.h>
#include <QTime>
#include <qmutex.h>
#include <qtimer.h>
#include <DHAVN_IPodController_QueueData.h>

#include <DHAVN_IPodController_CommonVars.h>

#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusMessage>

const char * const IPODCTRL_DBUS_NAME           = "com.lg.qt.ipodctrl";  
const char * const IPODCTRL_DBUS_PATH           = "/ipodctrl";       
const char * const IPODCTRL_DBUS_INTERFACE_NAME = "com.lg.qt.interface.ipodctrl";   
const char * const IPODCTRL_DBUS_METHOD         = "control";  

class CTrackerAbstractor;

struct CoverArtData
{
    QStringList m_AlbumList;
    QStringList m_ArtistList;
    QString m_RequestedCoverAlbumName;
    QString m_RequestedCoverArtistName;
    bool m_IsOnDemandCoverArtRequestInProgress;
    bool m_IsBasicPlayerCoverArtRequestInProgress;
};

class iPodControllerPrivateData :public QObject
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    iPodControllerPrivateData(QObject* parent);
    ~iPodControllerPrivateData();
    MediaInfo getCurrentMediaInfo();
    void setCurrentMediaInfo(MediaInfo& info, bool setCoverArt = false);
    void resetCurrentMediaInfo();
    void clearCurrentiPodData();
    QString getCurrentSerialNumber();
    void setCurrentSerialNumber(QString serialNumber);
    void setiPodPortNum( EIPodPorts portNum = eIPOD_PORT_NONE);
    EIPodPorts getiPodPortNum();
    QString getPerformanceTime(int);
    void updateBasicCoverArtVars(QString album, QString artist)
    {
        m_CoverArtData.m_RequestedCoverAlbumName.clear();
        m_CoverArtData.m_RequestedCoverAlbumName.append(album);
        m_CoverArtData.m_RequestedCoverArtistName.clear();
        m_CoverArtData.m_RequestedCoverArtistName.append(artist);
    }

    int getUniqueExitCode();

    int getTransactionId(bool increment=true);
    void setTransactionId(qint16 transId);
    void setAppSessionId(QString, qint16);
    qint16 getSessionId();
    QString getAppId();
    void setiPodSoftwareVerion(int major, int minor, int revision, QString sortwareVerion);
    QString getiPodSortwareVersion();
    void SendAudioRouterFadeIn();
    void SendAudioRouterFadeOut();
    //Implemented by Tan for Aha
    void updateSampleRateFile(int);
    void removeSampleRateFile();
public:
    MediaInfo m_MediaInfo;

    bool m_bIsAhaPandoraMode;
    bool m_bIsPreviousAhaPandoraMode;

    bool m_bBreakNextLoop;

    int m_OSDKey;

    float m_fwVersion;
    float m_iOSVersion;
    bool m_isLogoSupported;//Will be updated from RemoteUi Taskhandler
    bool m_isIDPS;//Will be updated from Idenfier TaskHandler.
    float m_generalLingoVersion;
    float m_DigitalLingoVersion;
    double m_ExtendedLingoVersion;//Will be updated from RemoteUi Taskhandler
    double m_DisplayRemoteLingoVersion;
    int m_iPodProductID;

    int m_nRequestType;
    int m_nQueueExitCode;
    bool m_isAutoPlay_PlaylistReady;
    bool m_restartUcsFlag;
    bool m_portresetFlag;

    /* iPod DigitalAudio Related data */
    QString m_audioDeviceName;
    QString m_hidDevPath;
    quint32 m_MaxPayload;
    bool isUIDSupported;
    QString m_serialNumber;
    EIPodPorts m_currentPortNum;
    QTime m_CurrentTime;
    QTime m_AutoPlayTime;
    QString m_CurrentTimeString;
    QMutex m_Mutex;
    QTimer m_MutexTimer;
    int m_isWaitingMutexUnlock; // added by sungha.choi 2013.04.22
    CTrackerAbstractor* m_TrackerObj;
    CoverArtData m_CoverArtData;
    bool m_IsiOSAppCommunicationSupported;

    //To implement the Queue machanism for the HMI events
    //{
    QList <EHMIEvent> m_EventCommandQueue;
    CIpodControllerQueueData m_QueueData;
    EHMIEvent m_CurrentInProgressEvent;

    //CR15593
    QString m_CurrentIPodPrt;
    //}
    QString m_AppId;
    qint16 m_SessionId;

    // This bool will decide that current app in IPod is the Music player other
    // else
    bool m_IsCurrentPlayingMP;

    //this counter will be set to zero when the fetchign is done from play engine or uid engine for the
    // other app, and will be increment later to rety only 3 time.
    int m_CurrentArtworkRetyCountForOtherMP;

    bool m_IsRestartOfMPInProgress;

    bool m_IsTabOff;
    //{added by junam 2013.04.13 for AutoStart
    enum
    {
        COMMANDKEY_MEDIAINFO,
        COMMANDKEY_TUNEREQUEST,
        COMMANDKEY_MAXIMUM,
        COMMANDKEY_PLAYLIST_LIST,   // Added by wooju.lee@lge.com 2013.05.23.
        COMMANDKEY_PLAYLIST_SONG,   // Added by wooju.lee@lge.com 2013.05.23.
    };
    QMap<int, QMap<int,QString> * > m_CommandMap;
    //}added by junam
    int m_SLeepForPlayWhileFetching;  // Added by pawan 2013.05.03.
    bool m_IsConvertedFromUIDToIndex;  // added by pawan 2013.05.04.
    bool m_FirstPlayListSet;  // Added by pawan 2013.05.01.
    int m_TotalIpodTrackCount;         // Added by pawan 2013.05.06.


    int m_MajorVersion;
    int m_MinorVersion;
    int m_RevisionVersion;

    bool m_IsDBIndexingPaused;
    bool m_IsDBIndexingFinished;
    //this is temp timer for peformance merasurement locally
    QTime m_TempPerformanceTimer;
    //        quint64     m_UIDForCoverArt;      // Added by pawan 2013.05.15. // Removed by pawan 2013.05.16.
    bool m_IsBTCalling; // Added by jonghwan.cho@lge.com 2013.11.05
private:
    qint16  m_TransactionId;
    QString m_SoftwareVersion;
    QTimer  *m_pSendingFadeInTimer;
private slots:
    void    SendAudioRouterFadeInSlot();
public slots:
    void updateFileNameWithTrackIndexSlot(int nTrackIndex, quint64 uid);
};

#endif
