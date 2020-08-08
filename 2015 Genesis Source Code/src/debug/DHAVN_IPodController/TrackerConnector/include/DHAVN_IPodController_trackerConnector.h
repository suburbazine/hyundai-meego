
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  10 October 2011                                   *
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
#ifndef TRACKERCONNECTOR_H
#define TRACKERCONNECTOR_H
#include <DHAVN_IPodController_TrackerInfo.h>
#include <DHAVN_IPodController_eventloopTimer.h>
#include <DHAVN_IPodController_Def.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QMap>
#include <QEventLoop>
#include <QVariantMap>
#include <QTime>

#include <QDBusMessage>
#include <QtDBus>
#include <QDBusError>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_eventloopTimer.h>    // added by sungha.choi 2013.04.12

#include <DHAVN_AppFramework_AppEngineBase.h>

class CIPodDBFetchTaskHandler;
class CArtworkFetchTaskHandler;
class iPodControllerPrivateData;

const QString doubleQuote("\"");
const QString doubleQuoteReplacement("\"\"");

#define REPEAT_FOLDER        2   //Repeat folder
#define EXEC_TEST

class CTrackerConnector : public QObject
{
    Q_OBJECT
private:
    USE_LOG_UTIL

public:
            CTrackerConnector(QObject* parent);
            ~CTrackerConnector();
	    bool cleanUpOnDisconnect();

private:
            int waitForResponceFromExitCode(int timeOut, int nExitCode);
            int updatePlayListsToTrackerE();
            void insertDBPropertiesToTableE();
            void insertiPodTracksMetaInfoToMasterTable(QList<SiPodDBTrackInfo> MtrackList, QString tableName);
            bool FileCopy(QString SrcFile,QString DestFile);
            void emitIndexDBusSignal(int startCount, int endCount);
            void CreateDataBaseAndTable();
            void getIndexAndUID(QString filePath, int& index, quint64& uid);   // Added by pawan 2013.05.16.
            int completeSyncIndexbasedE();
            void TempDBcreationE();
            void fetchAllTrackTitlesE(int trackCount);
            bool isUIDSyncRequiredE();
            void emitAutoPlayWaitForDone(bool,int);
            void DBCompareAndDelete();
            void dropOldTable();
            void playlistTableSync();
            void DeletePlaylistData();
            int EmptyTableCheck(int onDeviceTrackCount);

            void DeleteTmpfsDB();
            bool createConnection( QString str ); // Added by jonghwan.cho@lge.com	2013.03.27.
            bool isWaitingForResponse();    // added by sungha.choi 2013.04.12
            int isWaitingForResponseFromSignal();
            void checkSortingOrder(QString &strCompletetitle, LANGUAGE_T language); //added by junam 2013.06.07 iPod quick scroll
            void chopExceptionWords(QString &str);
public slots:
            void startDBSyncEC();
            void getListCompleteSlot(QMap<int,QString> namesOfCategoryContents,int length);
            void stopCurrentTask();
#ifdef COVERART_TEST
            void getArtworkEC(QStringList albumNameList, QStringList artistNameList,int index);
#else
            void getArtworkEC(QStringList albumNameList, QStringList artistNameList, bool forceFlag=false);
#endif
            void autoPlayComplete();
            void requestQuitSlot(); // added by sungha.choi 2013.04.16
            void OnChangeDBIndexingStatus(bool pause);

signals:
	    void mainLoopQuit();
            void mainLoopQuit(int nExitCode);
            void initDBSyncComplete(bool status);
#ifdef COVERART_TEST
            void artworkFetched(QStringList listt, QStringList Album, QStringList Artist);
#else
            void artworkFetched(QStringList listt);
#endif
            void initAutoPlayReady(bool,int);
            void dbThreadQuit();
            void IPODConnected(QString,int,int,int,int);
            void IpodTracksAfterSync(int);
            void IPodPlayableSongCount (int MasterTableCount, int deviceCount);
            void requestQuit(); // added by sungha.choi 2013.04.16            
            void OnCurrentTrackCountChanged(int);//added by junam 2013.04.23 for index DB
            void OnCurrentPlayingTrackCount(int); // Added by pawan 2013.05.01 
            void OnDBIndexingStatusChanged(bool pause=true);

public:
            void syncWithiTunesDBIndexedBasedE();
            void syncWithiTunesDBUIDBasedE();
            void cleanUPOnSyncComplete();
            bool isReadyToDelete(); // added by sungha.choi 2013.04.12
            int getSortingOrder(){return m_sortingOrder;}//added by junam 2013.06.07 iPod quick scroll

private:
            int  m_nRunningLoopFromSignal;
            bool m_ReadyToDeleteFromSignal;
            QList<SiPodDBTrackInfo> m_MusicTrackInfo;
            CTrackerAbstractor* m_trackerAbstractor;
            QMap<int,QString> m_PlaylistNames;
            QMap<int,QString> m_PlayListTrackNames;
            QEventLoop *m_pLoop;    // added by sungha.choi 2013.04.12
            CEventloopTimer *m_pWaitTimer;
            bool m_isRunningLoop;   // added by sungha.choi 2013.04.13
            bool m_isDisconnected;
            QDBusConnection  m_DbusConnection;
            CIPodDBFetchTaskHandler* m_iPodDBFetchTaskHandler;
            CArtworkFetchTaskHandler* m_artworkFetchTaskHandler;
            iPodControllerPrivateData* m_iPodPrivateData;
            bool SyncFlag;
            bool queryflag;
            QString m_serialNumber;
            bool m_cleanUpdone;
            int m_SyncType;
            bool m_dateUpdateString;
            bool m_DBCreationFlag;
            QTime m_ScanTime;
            QTime m_TrackerUpdateTime;
            QTime m_DBUpdateTime;
            bool m_coverArtRequestInProgress;
            bool m_ReadyToDelete;   // added by sungha.choi 2013.04.12

public:
            int m_TotalTracks;
            enum Song_type
            {
                    NoneType = -1,
                    MusicTracks,
                    AudioBook,
                    PodCast,
                    PlayList,
                    iTunesU
            };
            Song_type song_type;
            double m_ExtendedLingoVersion;
            QWaitCondition m_waitForAutoplayCond;
            QMutex  m_waitForAutoplayMutex;

            enum Sync_Type_ForGraceNote
            {
                    syncNotStarted =-1,
                    completeSync=0,
                    NoSync,
                    ReSync,
                    SyncCompleted
            };
            Sync_Type_ForGraceNote m_SyncTypeForTable;

            QSqlDatabase dBHandle; // Added by jonghwan.cho@lge.com	2013.03.27.
            int m_sortingOrder;
            int m_currentLanguage;
            bool m_PauseDBIndexing;
};


#endif // TRACKERCONNECTOR_H
