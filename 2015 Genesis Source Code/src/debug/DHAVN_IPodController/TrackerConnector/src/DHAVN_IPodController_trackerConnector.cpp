#include <DHAVN_IPodController_trackerConnector.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackTitle.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackArtist.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackAlbum.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackTitle.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackArtist.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackAlbum.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackInfo.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackInfo.h>

#include <DHAVN_IPodController_artworkFetchTaskHandler.h>
#include <DHAVN_IPodController_iPodDBFetchTaskHandler.h>
#include <DHAVN_IPodController_iPodDBUIDFetchTaskHandler.h>
#include <DHAVN_IPodController_iPodDBIndexFetchTaskHandler.h>

#include <DHAVN_IPodController_getDBTrackInfo.h>
#include <DHAVN_IPodController_getDBTrackInfo.h>
#include <QCryptographicHash>
#include <QDir>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_Performance.h>
#include <DHAVN_IPodController_PerformanceLog.h>
#include <DHAVN_IPodController_DBconnection.h>
#include <QtSql>
#include <qfile.h>
#include <unistd.h>
#include "DHAVN_EventLoop.h"
#include "DHAVN_EventLoopTimerExitCode.h"

#define SqliteDB

#define CHUNK_SIZE                     10
#define DBMAXCOUNT                     2

#define INITIAL_CHUNK_SIZE             5
#define UID_CHUNK_SIZE                 10

#define IPODCONTROLLER_DBUS_PATH       "/dbus/ipodcontroller/path"
#define IPODCONTROLLER_DBUS_INTERFACE  "dbus.ipodcontroller.interface"
#define IPODCONTROLLER_DBUS_SIGNAL     "IpodTrackerUpdate"
#define DISCONNECT_SIGNAL              "AppleDeviceDisconnected"
#define CONNECT_SIGNAL                 "AppleDeviceConnected"
#define IPODCONTROLLER_CONNECTION      "IpodDbusConnection"
#define EXTRA_DEBUG_LOGS               0

using namespace ExtendedLingo;

CTrackerConnector::CTrackerConnector(QObject* parent)
    :m_DbusConnection(IPODCONTROLLER_CONNECTION),m_TotalTracks(0),m_ExtendedLingoVersion(0)
{
    m_nRunningLoopFromSignal = 0;
    m_ReadyToDeleteFromSignal = true;
    m_iPodPrivateData = parent->findChild<iPodControllerPrivateData*>(gControllerPrivateData);

    switch(m_iPodPrivateData->m_currentPortNum)
    {
        case eIPOD_PORT_1:
            LOG_INIT_V2(giPodControllerLogName_1);
            break;
        case eIPOD_PORT_2:
            LOG_INIT_V2(giPodControllerLogName_2);
            break;
        default:
            LOG_INIT_V2(giPodControllerLogName);
            break;
    }
    //LOG_TRACE << "CTrackerConnector::CTrackerConnector() IN" << LOG_ENDL;

    song_type                   = NoneType;
    SyncFlag                    = false;
    queryflag                   = false;
    m_serialNumber.clear();
    m_cleanUpdone               = false;
    m_SyncType                  = 0;
    m_SyncTypeForTable          = syncNotStarted;
    m_coverArtRequestInProgress = false;

    m_PlaylistNames.clear();

    m_isDisconnected    = false;
    m_DBCreationFlag    = false;
    m_dateUpdateString  = false;
    m_trackerAbstractor = NULL;
    m_pLoop             = NULL;    // added by sungha.choi 2013.04.12
    m_pWaitTimer        = NULL;
    m_ReadyToDelete     = true;
    m_isRunningLoop     = false;   // added by sungha.choi 2013.04.13
    m_sortingOrder      = 0;
    m_currentLanguage   = -1;

    m_PauseDBIndexing = false;

    //LOG_TRACE << "CTrackerConnector::CTrackerConnector(), ExtendedLingoVersion = " << m_iPodPrivateData->m_ExtendedLingoVersion << LOG_ENDL;

    if(m_iPodPrivateData->m_ExtendedLingoVersion  >= (double)1.13)
    {
        CIPodDBUIDFetchTaskHandler* taskHandle = new CIPodDBUIDFetchTaskHandler(this,m_iPodPrivateData,
                                                                                ((CIpodController*)parent)->m_Writer,
                                                                                ((CIpodController*)parent)->m_Reader);
        m_iPodDBFetchTaskHandler = taskHandle;
    }
    else
    {
        CIPodDBIndexFetchTaskHandler* taskHandle = new CIPodDBIndexFetchTaskHandler(this,m_iPodPrivateData,
                                                                                    ((CIpodController*)parent)->m_Writer,
                                                                                    ((CIpodController*)parent)->m_Reader);
        m_iPodDBFetchTaskHandler = taskHandle;
    }

    m_artworkFetchTaskHandler = new CArtworkFetchTaskHandler(this,m_iPodPrivateData,
                                                             ((CIpodController*)parent)->m_Writer,
                                                             ((CIpodController*)parent)->m_Reader);

    connect(m_iPodDBFetchTaskHandler,SIGNAL(getListComplete(QMap<int,QString> ,int )),this, SLOT(getListCompleteSlot(QMap<int,QString> ,int)));

    m_DbusConnection = QDBusConnection::connectToBus(QDBusConnection::SystemBus,IPODCONTROLLER_CONNECTION);

    if(m_DbusConnection.isConnected() == true)
    {
        //LOG_TRACE << "   connected to  dbus daemon !" << LOG_ENDL;
    }
    else
    {
        //LOG_TRACE << "   NOT connected to  dbus daemon !" << LOG_ENDL;
    }

    //LOG_TRACE << "CTrackerConnector::CTrackerConnector() OUT" << LOG_ENDL;
}

CTrackerConnector::~CTrackerConnector()
{
    //LOG_TRACE << "CTrackerConnector::~CTrackerConnector() IN" << LOG_ENDL;

    if(m_pLoop)    // added by sungha.choi 2013.04.12
    {
        delete m_pLoop;
        m_pLoop = NULL;
    }

    if(m_pWaitTimer)
    {
        delete m_pWaitTimer;
        m_pWaitTimer = NULL;
    }

    cleanUpOnDisconnect();      // added by sungha.choi 2013.04.02

    if( m_iPodDBFetchTaskHandler )
    {
        delete m_iPodDBFetchTaskHandler;
        m_iPodDBFetchTaskHandler = NULL;
    }

    if( m_artworkFetchTaskHandler )
    {
        delete m_artworkFetchTaskHandler;
        m_artworkFetchTaskHandler = NULL;
    }

    if(dBHandle.isValid() == true)
    {
        if( dBHandle.isOpen() == true )
        {
            //LOG_TRACE << "  DBHandleLog closing DB handle from tracker connector" << LOG_ENDL;
            dBHandle.close();
        }
        //LOG_TRACE << "  DBHandleLog removing DB handle from tracker connector" << LOG_ENDL;
        QSqlDatabase::removeDatabase(dBHandle.connectionName());
    }

    //LOG_TRACE << "CTrackerConnector::~CTrackerConnector() OUT" << LOG_ENDL;
}

void CTrackerConnector::stopCurrentTask()
{
    //LOG_TRACE << "CTrackerConnector::stopCurrentTask() IN" << LOG_ENDL;
    m_isDisconnected = true;

    if(m_iPodDBFetchTaskHandler)    // added by sungha.choi 2013.04.15
    {
        m_iPodDBFetchTaskHandler->stopCurrentTask();
    }

    if(m_artworkFetchTaskHandler)    // added by sungha.choi 2013.04.15
    {
        m_artworkFetchTaskHandler->stopCurrentTask();
    }

    if(isWaitingForResponse() == true)
    {
        m_ReadyToDelete = false;
        m_pLoop->quit();
    }

    if(isWaitingForResponseFromSignal() > 0)  // added by sungha.choi 2013.04.26
    {
        m_ReadyToDeleteFromSignal = false;
        emit mainLoopQuit(ExitCode_AllExit);  // If signals will be added, emit the signal here.
    }
    //LOG_TRACE << "CTrackerConnector::stopCurrentTask() OUT" << LOG_ENDL;
}

bool CTrackerConnector::isReadyToDelete()  // added by sungha.choi 2013.04.12
{
    if(m_iPodDBFetchTaskHandler->isReadyToDelete() == true && m_artworkFetchTaskHandler->isReadyToDelete() == true)
        return (m_ReadyToDelete && m_ReadyToDeleteFromSignal);
    else
        return false;
}

bool CTrackerConnector::isWaitingForResponse()    // added by sungha.choi 2013.04.12
{
    return m_isRunningLoop;
}

int CTrackerConnector::isWaitingForResponseFromSignal()    // added by sungha.choi 2013.04.26
{
    return m_nRunningLoopFromSignal;
}

int CTrackerConnector::waitForResponceFromExitCode(int timeOut, int nExitCode)
{
    //////LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode Entry " << LOG_ENDL;
    int ret = 0;

    if(m_isDisconnected)
    {
        //LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode(), iPod Already disconnect" << LOG_ENDL;
        throw  IPOD_ERROR_DISCONNECT;
    }

    //QEventLoop localLoop;
    CIpodEventLoop localLoop(nExitCode);

    connect(this,SIGNAL(mainLoopQuit(int)),&localLoop,SLOT(mainLoopQuitSlot(int)));

    CEventloopTimerExitCode localWaitTimer;
    localWaitTimer.start(&localLoop,timeOut);

    m_nRunningLoopFromSignal ++;   // added by sungha.choi 2013.04.26
    ret = localLoop.exec();
    m_nRunningLoopFromSignal --;   // added by sungha.choi 2013.04.26

    if(m_nRunningLoopFromSignal == 0 && m_ReadyToDeleteFromSignal == false)  // added by sungha.choi 2013.04.26
    {
        m_ReadyToDeleteFromSignal = true;
    }

    localWaitTimer.stop();

    ////LOG_INFO << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode: Loop exit,ret= " << ret << LOG_ENDL;
    if((ret == IPOD_ERROR_DISCONNECT) || m_isDisconnected)
    {
        //LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode() Loop exit with Error on disconnect" << LOG_ENDL;
        throw  IPOD_ERROR_DISCONNECT;
    }
    ////LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode Exit " << LOG_ENDL;
    return ret;//Return IPOD_SUCCESS or IPOD_ERROR_TIMEOUT
}

void CTrackerConnector::DBCompareAndDelete()
{
    //LOG_TRACE << "CTrackerConnector::DBCompareAndDelete() IN" << LOG_ENDL;

    QString file_name ="";
    QDir dir(PERM_DBPATH);
    dir.setPath(PERM_DBPATH);
    QFileInfoList list;
    list.clear();
    list = dir.entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot );
    int dbCount = list.count();
    QString currentTime = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");

    if( dbCount >= DBMAXCOUNT )
    {
        //LOG_TRACE << "   dbCount = "<< dbCount << LOG_ENDL;
        bool flag = false;

        for( int i = 0 ; i < dbCount ; i++ )
        {
            if( list.at(i).fileName() == m_serialNumber ) // If m_serialNumber DB file exist
            {
                flag = true;
                break; // exit for loop
            }
        }

        //LOG_INFO << "   flag = " << flag << LOG_ENDL;
        if( flag && ( dbCount == DBMAXCOUNT ) )
        {
            //LOG_TRACE << "   DB already Present. No Changes Needed"<<LOG_ENDL;
        }
        else
        {
            //LOG_TRACE << "   Delete old DB and add new DB"<<LOG_ENDL;
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

            for( int i = 0 ; i < dbCount ; i++ )
            {
                QString DBname(list.at(i).absoluteFilePath());
                db.setDatabaseName( DBname );
                QSqlError dbOpenerr;
                QSqlQuery dbOpenquery;
                bool dbOpenFlag = false;

                if(( QFile::exists(DBname) ) == true ) {
                    if( db.isValid() == true ) {
                        dbOpenFlag =  db.open();
                    }
                    else
                        continue;
                }
                else
                    continue;

                if(!dbOpenFlag)
                {
                    dbOpenerr = dbOpenquery.lastError();
                    //LOG_INFO << "   DB Open Error is : " << dbOpenerr.text() << LOG_ENDL;
                }
                else //DB Opened Successfully
                {
                    QSqlQuery query;
                    QSqlError err;
                    QString dbTime;
                    dbTime.clear();

                    if( dBHandle.isValid() == true )
                        queryflag = query.exec("select DBCreatonTime from DbPropertyTable");
                    else
                        queryflag = false;

                    if(!queryflag)
                    {
                        err = query.lastError();
                        //LOG_INFO << "   Query error last error is : " << err.text() << LOG_ENDL;
                    }
                    else
                    {
                        while(query.next())
                        {
                            dbTime = query.value(0).toString();
                            //LOG_INFO << "   dbTime = " << dbTime << LOG_ENDL;
                        }

                        if((!(dbTime.isEmpty())) && (currentTime > dbTime))
                        {
                            //Current Time is more than dbTime then assign dbTime to CurrentTime
                            currentTime  = dbTime;
                            file_name    = list.at(i).fileName();
                            //LOG_INFO << "   DB file_name = " << file_name << LOG_ENDL;
                        }
                    }
                }
            }

            if(!file_name.isEmpty())
            {
                //LOG_INFO << "   Remove DB filename = " << file_name << LOG_ENDL;
                if( CIpodController::m_ConnectionDeviceNum == 1 )
                {
                    dir.remove(file_name);
                }
            }
        }
    }
    else
    {
        //LOG_TRACE<< "   else entered: Count < 5 ";
    }
    //LOG_TRACE << "CTrackerConnector::DBCompareAndDelete() OUT" << LOG_ENDL;
}

void CTrackerConnector::startDBSyncEC()
{
    //LOG_TRACE << "CTrackerConnector::startDBSync() IN"<<LOG_ENDL;

    try // added by sungha.choi 2013.04.02, blocking issue (handle to throw iPodDisconnect)
    {
        m_iPodPrivateData->m_IsDBIndexingFinished = false;


        double dVersion = m_iPodDBFetchTaskHandler->lingoProtocolVersion(4);

        bool bRtn = false;

        m_ExtendedLingoVersion = dVersion;
        //LOG_TRACE << "CTrackerConnector::startDBSync(), Extended Lingo Version = " <<  dVersion << LOG_ENDL;

        m_serialNumber = m_iPodDBFetchTaskHandler->getCurrentSerialNumberE();
        //LOG_TRACE << "CTrackerConnector::startDBSync(), m_serialNumber         = " <<  m_serialNumber << LOG_ENDL;
        m_iPodPrivateData->setCurrentSerialNumber(m_serialNumber);

        QString serialStr = m_serialNumber;
        QString path(PERM_DBPATH);
        QString completePath;

        completePath.append(path);
        completePath.append(serialStr);

        if(!QFile::remove(completePath))
        {
            //LOG_TRACE << "   DB file(" << completePath << ") is not removed error occured" << LOG_ENDL;
        }

        path.clear();
        path.append(TEMP_DBPATH);
        completePath.clear();
        completePath.append(path);
        completePath.append(serialStr);
        if(!QFile::remove(completePath))
        {
            //LOG_TRACE << "   DB file(" << completePath << ") is not removed error occured" << LOG_ENDL;
        }

        bRtn = createConnection( m_serialNumber );
        if( bRtn == false )
        {
            //LOG_TRACE << "CTrackerConnector:startDBSync() Create Connection Failed!!!!!!!!!!!!!!!!!!!!"<<LOG_ENDL;
            return;
        }

        if(dVersion >= (double)1.13)
        {
            m_iPodPrivateData->isUIDSupported = true;
            syncWithiTunesDBUIDBasedE();
        }
        else
        {
            m_iPodPrivateData->isUIDSupported = false;
            m_DBCreationFlag = true;
            syncWithiTunesDBIndexedBasedE();
        }

        if( m_isDisconnected )// added by sungha.choi 2013.04.15
        {
            throw  IPOD_ERROR_DISCONNECT;
        }

        //LOG_TRACE<<"   Reset DB for CArtworkFetchTaskHandler for UID"<<LOG_ENDL;
        m_iPodDBFetchTaskHandler->stopTaskHandler();

        //LOG_TRACE << "CTrackerConnector::startDBSync(), emit initDBSyncComplete(true)_"  << LOG_ENDL;
        m_iPodPrivateData->m_IsDBIndexingPaused = false;

        if( !m_isDisconnected ) // added by sungha.choi 2013.04.15
            emit initDBSyncComplete(true);
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE <<"CTrackerConnector::startDBSync() Exception Caught ******" << LOG_ENDL;
        //LOG_TRACE <<"CTrackerConnector::startDBSync() iPod DB Sync Incomplete ******" << LOG_ENDL;
    }
    //LOG_TRACE << "CTrackerConnector::startDBSync() OUT"<<LOG_ENDL;
}

void CTrackerConnector::syncWithiTunesDBUIDBasedE()
{
    //LOG_TRACE << "CTrackerConnector::syncWithiTunesDBUIDBasedE() IN" << LOG_ENDL;

    QString Existing_DB(PERM_DBPATH);
    QString DBname(TEMP_DBPATH);
    bool res = false;

    if( m_iPodDBFetchTaskHandler == NULL )
    {
        //LOG_TRACE << "CTrackerConnector::syncWithiTunesDBUIDBasedE() Error: m_iPodDBFetchTaskHandler is NULL" << LOG_ENDL;
        return;
    }

    if( m_serialNumber.isEmpty() == true )
    {
        m_serialNumber = m_iPodDBFetchTaskHandler->getCurrentSerialNumberE();
        //LOG_INFO << "   m_serialNumber = " << m_serialNumber << LOG_ENDL;
        m_iPodPrivateData->setCurrentSerialNumber( m_serialNumber );
    }

    //iPod Ext Lingo 1.14 StartAutoPlay
    if( m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
    {
        emitAutoPlayWaitForDone(true, m_SyncType);
    }

    DBname.append( m_serialNumber );
    Existing_DB.append( m_serialNumber );
    //copy from Filesystem to tmpfs
    QFile f(Existing_DB);

    if( f.exists() == true )
    {
        QFile f1(DBname);
        if( f1.exists() == true )
        {
            if( dBHandle.isValid() == true )
            {
                if( dBHandle.open() )
                {
                    //LOG_TRACE << " DBHandleLog  closing DB handle from syncWithiTunesDBUIDBasedE" << LOG_ENDL;
                    dBHandle.close();
                }
            }
            //LOG_TRACE << "  DBHandleLog removing DB handle (db file) from syncWithiTunesDBUIDBasedE" << LOG_ENDL;
            res = QFile::remove(DBname);
            if( res == true )
            {
                //LOG_INFO << "   File Remove operation from TempFS succeeded" << res << LOG_ENDL;
            }
            else
            {
                //LOG_INFO << "   File Remove operation from TempFS Failed: file not exists"<< DBname << LOG_ENDL;
            }
        }

        //LOG_INFO << "   File copy 1 Perm to Temp PERM_DBPATH = "<< Existing_DB << " TEMP_DBPATH = " << DBname << LOG_ENDL;
        res = FileCopy(PERM_DBPATH,TEMP_DBPATH);
        if( res )
        {
            //LOG_INFO << "   File copy to TempFS for Re-Sync operation succeeded" << LOG_ENDL;
            if(!createConnection(m_serialNumber))
            {
                //LOG_TRACE << "   Sqlite Database connection not established" << LOG_ENDL;
            }
            dropOldTable();
        }
        else
        {
            //LOG_TRACE << "   File copy to TempFS for Re-Sync operation Failed " << LOG_ENDL;
        }
    }
    if( m_isDisconnected )
    { // added by sungha.choi 2013.04.15
        throw  IPOD_ERROR_DISCONNECT;
    }

    m_SyncType = SYNC;
    m_ScanTime.start();
    int milliseconds     = 0;
    int scanUpdateTime   = 0;
    int trakerUpdateTime = 0;
    int dB_UpdateTime    = 0;
    int netTrackCount = m_iPodPrivateData->m_TotalIpodTrackCount;
    //LOG_TRACE << "   netTrackCount = " <<  netTrackCount << LOG_ENDL;

    if( m_isDisconnected )
    { // added by sungha.choi 2013.04.15
        throw  IPOD_ERROR_DISCONNECT;
    }

    if( netTrackCount > 0 )
    {
        m_SyncTypeForTable = completeSync;
        m_dateUpdateString = false;
        CreateDataBaseAndTable();

        int totalHit = netTrackCount/INITIAL_CHUNK_SIZE;
        if( netTrackCount % INITIAL_CHUNK_SIZE )
        {
            totalHit++;
        }

        int   startIndex           = 0;
        bool  isFirstChunkInMasterTable = true;
        int   dBusSignalStartIndex = 0;
        int   nCurrentTotalSong    = 0;
        int   emitcount            = 0;

        while(totalHit)
        {
            if( m_isDisconnected )
            { // added by sungha.choi 2013.04.15
                throw  IPOD_ERROR_DISCONNECT;
            }

            if(m_ScanTime.isNull())
                m_ScanTime.start();
            else
                m_ScanTime.restart();

            int getTrackCount = 0;
            int remainingCount = netTrackCount - (startIndex);

            if(remainingCount > INITIAL_CHUNK_SIZE)
            {
                getTrackCount = INITIAL_CHUNK_SIZE;
            }
            else
            {
                getTrackCount = netTrackCount - (startIndex);
            }

            totalHit--;
            PT_START_TIME(PT_OBJ_METADATA_FETCHING_UID, 0);

            while( m_coverArtRequestInProgress )
            {
                //LOG_TRACE << "basicp pausing on indexing for cover art" << LOG_ENDL;
                waitForResponceFromExitCode(500, ExitCode_PauseIndexingStatus);
            }
            while(m_PauseDBIndexing)
            {
                m_iPodPrivateData->m_IsDBIndexingPaused = true;
//                //LOG_TRACE << "PLAYLIST: Trackerconnector::StartDBSync() pausing the db indexing, emit OnDBIndexingStatusChanged(true)_"  << LOG_ENDL;
                emit OnDBIndexingStatusChanged(true);
                waitForResponceFromExitCode(500, ExitCode_PauseIndexingStatus);
            }

            m_iPodPrivateData->m_IsDBIndexingPaused = false;
            m_iPodDBFetchTaskHandler->returnAllTracksInfoE(startIndex, getTrackCount);
            startIndex = startIndex + INITIAL_CHUNK_SIZE;
            PT_END_TIME(PT_OBJ_METADATA_FETCHING_UID, 0);

            milliseconds   = m_ScanTime.elapsed();
            scanUpdateTime = scanUpdateTime + milliseconds;
#ifdef DBUGLOGS
            //LOG_INFO << "scanUpdateTime is " << scanUpdateTime << LOG_ENDL;
#endif
            milliseconds = 0;

            if(m_TrackerUpdateTime.isNull())
            {
                m_TrackerUpdateTime.start();
            }
            else
            {
                m_TrackerUpdateTime.restart();
            }
            QList <SiPodDBTrackInfo> musicTrackList = m_iPodDBFetchTaskHandler->getUIDBasedTrackList();

#ifdef DBUGLOGS
            //LOG_TRACE << "iPodPerformanceMeasurement:UID Based Scan END, Get all track info including podcast, audiobook all songs finished" << LOG_ENDL;
#endif
            milliseconds = m_TrackerUpdateTime.elapsed();
            trakerUpdateTime = trakerUpdateTime + milliseconds;

#ifdef DBUGLOGS
            //LOG_INFO << "trakerUpdateTime is " << trakerUpdateTime << LOG_ENDL;
#endif
            milliseconds = 0;
            // emit DBus signal for track count
            int musicTrackCount = musicTrackList.count();

            //Need to reduce the autoplaylist count to 5 which is same
            //as for the index based play devices, start the auto play and then go for full sync.
            if ((musicTrackCount == UID_CHUNK_SIZE) || (remainingCount <= INITIAL_CHUNK_SIZE) || isFirstChunkInMasterTable)
            {
                if(m_DBUpdateTime.isNull())
                    m_DBUpdateTime.start();
                else
                    m_DBUpdateTime.restart();
#ifdef DBUGLOGS
                //LOG_TRACE << "iPodPerformanceMeasurement:UID Based TrackerUpdate START"<<LOG_ENDL;
#endif
                PT_START_TIME(PT_OBJ_MASTERTABLE_UPDATE_METADATA, 0);
                //insert into master table
                insertiPodTracksMetaInfoToMasterTable(musicTrackList, "MasterTable");
                PT_END_TIME(PT_OBJ_MASTERTABLE_UPDATE_METADATA, 0);
#ifdef DBUGLOGS
                //LOG_TRACE << "iPodPerformanceMeasurement:UID Based TrackerUpdate END"<<LOG_ENDL;
#endif
                milliseconds =  m_DBUpdateTime.elapsed();
                dB_UpdateTime = dB_UpdateTime + milliseconds;
#ifdef DBUGLOGS
                //LOG_INFO << "   dB_UpdateTime is " << dB_UpdateTime << LOG_ENDL;
#endif
                milliseconds = 0;

                if(!m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
                {
                    if(isFirstChunkInMasterTable)
                    {
                        isFirstChunkInMasterTable = false;
                        if( ! m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
                        {
                            //iPod Ext Lingo 1.13 StartAutoPlay
                            m_iPodDBFetchTaskHandler->makeSelectionE("Track",0);
                            m_iPodDBFetchTaskHandler->pauseForTracker();
                            emitAutoPlayWaitForDone(true, m_SyncType);
                        }
                    }
                }

                PT_START_TIME(PT_OBJ_GRACENOTE, 0);
#ifdef DBUGLOGS
                //LOG_INFO << "   Emitting emitIndexDBusSignal with dBusSignalStartIndex =  " << dBusSignalStartIndex
                         << ", ans End Index = " << (dBusSignalStartIndex + (musicTrackCount-1)) << LOG_ENDL;
#endif
                emitIndexDBusSignal(dBusSignalStartIndex, (dBusSignalStartIndex + (musicTrackCount-1)));
                PT_END_TIME(PT_OBJ_GRACENOTE, 0);

                dBusSignalStartIndex = dBusSignalStartIndex+musicTrackCount;

                nCurrentTotalSong = nCurrentTotalSong + musicTrackCount;
                emitcount = emitcount + musicTrackCount;  // Added by pawan 2013.05.03.

                if( nCurrentTotalSong == 5  || emitcount > 50 )  // Added by pawan 2013.05.03.
                {
                    if(m_iPodPrivateData->m_ExtendedLingoVersion >= 1.14 || m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
                    {
//                        //LOG_TRACE << "CTrackerConnector::syncWithiTunesDBUIDBasedE(), emit OnCurrentTrackCountChanged(m_iPodPrivateData->m_TotalIpodTrackCount = "
//                               << m_iPodPrivateData->m_TotalIpodTrackCount << ")"  << LOG_ENDL;
                        emit OnCurrentTrackCountChanged(m_iPodPrivateData->m_TotalIpodTrackCount);
                    }
                    else
                    {
//                        //LOG_TRACE << "CTrackerConnector::syncWithiTunesDBUIDBasedE(), emit OnCurrentTrackCountChanged(nCurrentTotalSong = "
//                                 << nCurrentTotalSong << ")"  << LOG_ENDL;
                        emit OnCurrentTrackCountChanged(nCurrentTotalSong);
                    }

                    emitcount = 0;
                }

                m_iPodDBFetchTaskHandler->clearList();
            }
        }

        if(m_iPodPrivateData->m_ExtendedLingoVersion >= 1.14 || m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
        {
//            //LOG_TRACE << "CTrackerConnector::syncWithiTunesDBUIDBasedE(), emit OnCurrentTrackCountChanged(m_iPodPrivateData->m_TotalIpodTrackCount = "
//                     << m_iPodPrivateData->m_TotalIpodTrackCount << ")"  << LOG_ENDL;
            emit OnCurrentTrackCountChanged(m_iPodPrivateData->m_TotalIpodTrackCount);
        }
        else
        {
//            //LOG_TRACE << "CTrackerConnector::syncWithiTunesDBUIDBasedE(), emit OnCurrentTrackCountChanged(nCurrentTotalSong = "
//                 << nCurrentTotalSong << ")"  << LOG_ENDL;
            emit OnCurrentTrackCountChanged(netTrackCount);
        }

        //LOG_INFO << "   Final scanupdatetime is " << scanUpdateTime << LOG_ENDL;
        QString string = m_iPodPrivateData->getPerformanceTime(scanUpdateTime);
        PERFORMANCELOG ( "IPod UID Based Scan time is %s " , string.toAscii().data()  );
        string.clear();

        //LOG_INFO << "   Final trakerUpdateTime is " << trakerUpdateTime << LOG_ENDL;
        string = m_iPodPrivateData->getPerformanceTime(trakerUpdateTime);
        PERFORMANCELOG ( "IPod UID Based trakerUpdate time is %s " , string.toAscii().data()  );
        string.clear();

        //LOG_INFO << "   Final dB_UpdateTime is " << dB_UpdateTime << LOG_ENDL;
        string = m_iPodPrivateData->getPerformanceTime(dB_UpdateTime);
        PERFORMANCELOG ( "IPod UID Based DBUpdate time is %s " , string.toAscii().data()  );
        string.clear();

        //LOG_INFO << "   Final Total time is " << (scanUpdateTime + trakerUpdateTime + dB_UpdateTime) << LOG_ENDL;
        string = m_iPodPrivateData->getPerformanceTime((scanUpdateTime + trakerUpdateTime + dB_UpdateTime));
        PERFORMANCELOG ( "IPod UID Based Total time is %s " , string.toAscii().data()  );
        string.clear();

        if( m_isDisconnected )
        {
            throw  IPOD_ERROR_DISCONNECT;
        }

        // Changed for CR 11116. DB time should be inserted to table once the complete sync is done.
        // so that even though the device is disconnected in the middle of sync and reconnected, the device will go for complete sync  instead of not performing anything
        insertDBPropertiesToTableE();//Insert the time stamp
        //LOG_TRACE << "   PlayList Sync Started" << LOG_ENDL;

        if( m_isDisconnected )
        {
            throw  IPOD_ERROR_DISCONNECT;
        }

        bool demptyTableFlag= false;
        int  tablecheckCount = EmptyTableCheck(netTrackCount);

        if( tablecheckCount == 0 )
        {
            demptyTableFlag = true;
        }

        if( m_isDisconnected )
        {
            throw  IPOD_ERROR_DISCONNECT;
        }
    }

    if( m_isDisconnected )
    {
        throw  IPOD_ERROR_DISCONNECT;
    }

    //copy from tmpfs to app share
    QFile f1(Existing_DB);
    if (f1.exists())
    {
        res = QFile::remove(Existing_DB);
        if(res)
        {
            //LOG_INFO << "   File Remove operation from FS succeeded" << LOG_ENDL;
        }
        else
        {
            //LOG_TRACE << "   File Remove operation from FS Failed: file not exists"<<LOG_ENDL;
        }
    }

    //LOG_INFO << "   File copy 2 Temp to Perm PERM_DBPATH = "<< Existing_DB << " TEMP_DBPATH = " << DBname << LOG_ENDL;

    if(dBHandle.isValid())
    {
        if(dBHandle.open())
        {
            //LOG_TRACE << "  DBHandleLog closing DB handle from syncWithiTunesDBUIDBasedE 2" << LOG_ENDL;
            dBHandle.close();
        }
        //LOG_TRACE << "  DBHandleLog removing DB handle from syncWithiTunesDBUIDBasedE 2" << LOG_ENDL;
        dBHandle.removeDatabase(m_serialNumber);
    }

    if( m_isDisconnected )
    {
        throw  IPOD_ERROR_DISCONNECT;
    }

    res = FileCopy(TEMP_DBPATH,PERM_DBPATH);

    if(res)
    {
        //LOG_INFO << "   File copy to FS after Re-Sync operation succeeded" << res << LOG_ENDL;
        // 2013.01.03 Added by Neelima for CR13965 : Remove the DB from tmpfs
        DeleteTmpfsDB();

        if (!createConnection(m_serialNumber))
        {
            //LOG_TRACE << "   Sqlite Database connection not established" << LOG_ENDL;
        }
        dropOldTable();
    }
    else
    {
        //LOG_INFO << "   File copy to FS after Re-Sync operation Failed, check whether file exists = "<< DBname << LOG_ENDL;
    }

    PT_START_TIME(PT_OBJ_GRACENOTE, 0);
    m_SyncTypeForTable = SyncCompleted;
    //LOG_INFO << "   Emitting emitIndexDBusSignal with SyncCompleted signal =  " << m_SyncTypeForTable << LOG_ENDL;
    emitIndexDBusSignal(0, 0);
    PT_END_TIME(PT_OBJ_GRACENOTE, 0);
    //LOG_TRACE << "CTrackerConnector::syncWithiTunesDBUIDBased() OUT" << LOG_ENDL;
    return;
}

void CTrackerConnector::insertDBPropertiesToTableE()
{
    //LOG_TRACE << "CTrackerConnector::insertDBPropertiesToTableE() IN" << LOG_ENDL;
    //LOG_INFO << "   m_dateUpdateString = " << m_dateUpdateString <<LOG_ENDL;
    //LOG_INFO << "   m_DBCreationFlag   = " << m_DBCreationFlag <<LOG_ENDL;

    bool bRtn = false;

    SITunesDBInfo dBinfo;
    if((!m_DBCreationFlag) && (!m_dateUpdateString)) //only for UID Based Devices and only for Sync case
    {
        if(m_iPodDBFetchTaskHandler == NULL)
        {
            //LOG_TRACE << "CTrackerConnector::insertDBPropertiesToTable() m_iPodDBFetchTaskHandler is NULL" << LOG_ENDL;
            return;
        }

        if(IPOD_SUCCESS != m_iPodDBFetchTaskHandler->getiTunesDBInfoE(&dBinfo))
        {
            //LOG_TRACE << "   Failed to get DB info"<<LOG_ENDL;
            return;
        }
    }//perform only for UID devices

    bRtn = createConnection(m_serialNumber);

    if( bRtn == true )
    {
        QSqlQuery query(dBHandle);
        QString queryString;

        QString datetime = QDateTime::currentDateTime().currentDateTime().toString("yyyyMMddHHmmss");
        //LOG_INFO << "   dateTime = " << datetime <<LOG_ENDL;
        QString dateStr;

        if(!m_DBCreationFlag) //for UID Based devices
        {
            dateStr.append(QString("%1").arg(dBinfo.lastSyncTime.year));
            dateStr.append(QString("%1").arg(dBinfo.lastSyncTime.month));
            dateStr.append(QString("%1").arg(dBinfo.lastSyncTime.day));
            dateStr.append(QString("%1").arg(dBinfo.lastSyncTime.hours));
            dateStr.append(QString("%1").arg(dBinfo.lastSyncTime.min));
            dateStr.append(QString("%1").arg(dBinfo.lastSyncTime.sec));

        }
        else //for Index Based devices
        {
            dateStr.append("-1");
        }

        if(!m_dateUpdateString) // only in sync case of both UID and Index Based this part of code will get executed
        {
            queryString.append("delete from DbPropertyTable");
            if( dBHandle.isValid() == true )
                query.exec(queryString);

            queryString.clear();

            queryString.append("insert into DbPropertyTable values(\"-1\",\"");
            queryString.append(dateStr);
            queryString.append("\",\"");
            queryString.append(datetime);
            queryString.append("\")");
        }

        QSqlError err1;
        bool queryflag = false;
        if( dBHandle.isValid() == true )
            queryflag = query.exec( queryString );
        if(!queryflag)
        {
            err1 = query.lastError();
            //LOG_INFO << "   last error is : " << err1.text() << LOG_ENDL;
        }
    }
    //LOG_TRACE << "CTrackerConnector::insertDBPropertiesToTableE() OUT"<<LOG_ENDL;
}

void CTrackerConnector::emitIndexDBusSignal(int startCount, int endCount)
{
    ////LOG_TRACE << "CTrackerConnector::emitIndexDBusSignal() IN, startCount = " << startCount << ", endCount = " << endCount << LOG_ENDL;
    //Emit signal for the tracks which are inserted into the tracker
    if(m_SyncTypeForTable != syncNotStarted)
    {
        QString path        = IPODCONTROLLER_DBUS_PATH;
        QString interface   = IPODCONTROLLER_DBUS_INTERFACE;
        QString signalName  = IPODCONTROLLER_DBUS_SIGNAL;
        QDBusMessage signal = QDBusMessage::createSignal(path,interface,signalName);

        signal << m_serialNumber;
        signal << startCount;
        signal << endCount;
        signal << m_iPodPrivateData->getiPodPortNum();
        signal << m_SyncTypeForTable;

        if(m_DbusConnection.send(signal))
        {
#ifdef DBUGLOGS
            //LOG_TRACE << "CTrackerConnector::emitIndexDBusSignal: dbus sent succes for count = " << endCount << LOG_ENDL;
#endif
        }
        else
        {
            QDBusError error = m_DbusConnection.lastError();
#ifdef DBUGLOGS
            //LOG_INFO << "CTrackerConnector::updateMusicTracksToTracker dbus sent fail "
                     << error.message().toAscii().data()<< LOG_ENDL;
#endif
        }

        if( endCount == m_iPodPrivateData->m_TotalIpodTrackCount - 1 )
        {
            //LOG_TRACE << "CTrackerConnector::emitIndexDBusSignal(), emit IPODConnected()_ "  << ", m_serialNumber = " <<
            //            m_serialNumber << ", startCount = " << startCount << ", endCount = " << endCount << ", PortNumber = " <<
            //            m_iPodPrivateData->getiPodPortNum() << ", m_SyncTypeForTable = " << m_SyncTypeForTable << LOG_ENDL;
        }
        emit IPODConnected(m_serialNumber,startCount,endCount,m_iPodPrivateData->getiPodPortNum(),m_SyncTypeForTable);
    }
}

void CTrackerConnector::getArtworkEC(QStringList albumNameList, QStringList artistNameList, bool forceFlag)
{
    //LOG_TRACE << "basicp CTrackerConnector::getArtworkEC() IN, forceFlag = " << forceFlag << LOG_ENDL;
    QStringList artworkPathList;

    if( m_coverArtRequestInProgress )
    {
        //LOG_TRACE << "basicp CTrackerConnector::getArtworkEC   Already Inprogress!!!" << LOG_ENDL;
        //LOG_TRACE << "basicp CTrackerConnector::getArtworkEC   Exit-1" << LOG_ENDL;
    }
    else
    {
        m_coverArtRequestInProgress = true;

        for( int i=0; i<albumNameList.count() && (i < artistNameList.count()); i++)
        {
            try
            {
                //LOG_TRACE << "basicp  CTrackerConnector::getArtworkEC  album name = " << albumNameList[i] <<  "   artist name = " << artistNameList[i] <<LOG_ENDL;
                QString imagePath;
                int index      = -1;
                quint64 uid    = -1;
                getIndexAndUID(artistNameList[i], index, uid);

                bool isTrue = (m_iPodPrivateData->m_IsConvertedFromUIDToIndex && m_iPodPrivateData->m_IsCurrentPlayingMP);
                int  ret = m_artworkFetchTaskHandler->fetchArtworkE(index, imagePath, isTrue, forceFlag, uid);

                if(ret == IPOD_SUCCESS)
                    artworkPathList.append(imagePath);
                else
                    artworkPathList.append("");
            }
            catch( EIPodErrors ex )
            {
                //LOG_TRACE << "basicp CTrackerConnector::getArtworkEC(), Exeption caught!!!"<<LOG_ENDL;
                //artworkPathList.clear();
                //break;
                return;
            }
        }
        //LOG_TRACE << "basiccp CTrackerConnector::getArtworkEC(), emit artworkFetched()"  << LOG_ENDL;
        emit artworkFetched(artworkPathList);
        m_coverArtRequestInProgress = false;
    }
    //LOG_TRACE << "basicp CTrackerConnector::getArtworkEC() OUT" << LOG_ENDL;
}

void CTrackerConnector::getIndexAndUID(QString filePath, int& index, quint64& uid)
{
    QString uidStr;
    QString indexStr;

    int i  = filePath.lastIndexOf("/");
    int _i = filePath.indexOf("_");
    int i_ = filePath.lastIndexOf("_");


    if( _i == i_ )
    {
        uidStr.append(filePath.mid(_i+1, filePath.size()));
    }
    else
    {
        int len = i_ - (_i+1);
        uidStr.append(filePath.mid(_i+1, len));
    }

    bool ok = false;
    uid = uidStr.toULongLong(&ok);
    indexStr.append(filePath.mid(i+1, (_i-i-1)));
    index = indexStr.toInt(&ok);
}

void CTrackerConnector::CreateDataBaseAndTable()
{
    //LOG_TRACE << "CTrackerConnector::CreateDataBaseAndTable() IN" << LOG_ENDL;

    bool bRtn;
    bRtn = createConnection(m_serialNumber);

    if( bRtn == true )
    {
        QSqlQuery query(dBHandle);
        if( dBHandle.isValid() == false ) return;

        query.exec("create table MasterTable(T_index int primary key, T_UID varchar(150), T_Title varchar(50), T_Album varchar(50), T_Artist varchar(50), T_Genre varchar(50), T_Composer varchar(50), T_Type varchar(50), T_TrackCount int, T_TopLevelIndex int, T_TopLevelName varchar(50), T_TypeIndex int, T_Mldb int, T_FilePath varchar(50), T_Duration int,T_GraceNoteFlag int)");
        query.exec("create table DbPropertyTable(T_DBUid varchar(20), T_lastSyncDate varchar(20),DBCreatonTime varchar(20))");
        query.exec("create table PlayListProperty(T_PlayListNames varchar(50), T_PlayListCount int, T_PlayListIndex int)");
    }
    else
    {
        //LOG_TRACE << "   Sqlite Database connection not established" << LOG_ENDL;
    }
    //LOG_TRACE << "CTrackerConnector::CreateDataBaseAndTable() OUT" << LOG_ENDL;
}

void CTrackerConnector::getListCompleteSlot(QMap<int,QString> namesOfCategoryContents,int /*length*/)
{
    switch(song_type)
    {
        case MusicTracks :
        {
            m_PlayListTrackNames.clear();
            m_PlayListTrackNames = namesOfCategoryContents;
            break;
        }
        case PlayList :
        {
            m_PlaylistNames.clear();
            m_PlaylistNames = namesOfCategoryContents;
            break;
        }
        case AudioBook:
        case PodCast:
        case iTunesU:
        case NoneType:
        {
            break;
        }
        default :
            break;
    }
    emit mainLoopQuit(ExitCode_GetList);
}

void CTrackerConnector::syncWithiTunesDBIndexedBasedE()
{
    //LOG_TRACE << "CTrackerConnector::syncWithiTunesDBIndexedBasedE() IN" << LOG_ENDL;

    int  ret = -1;

    QString Existing_DB(PERM_DBPATH);

    if(m_serialNumber.isEmpty())
    {
        m_serialNumber = m_iPodDBFetchTaskHandler->getCurrentSerialNumberE();
        m_iPodPrivateData->setCurrentSerialNumber(m_serialNumber);
    }
    //LOG_TRACE << "   m_serialNumber = " << m_serialNumber << LOG_ENDL;

    QString DBname(TEMP_DBPATH);
    DBname.append(m_serialNumber);
    Existing_DB.append(m_serialNumber);
    QFile f(Existing_DB);

    if(dBHandle.isValid())
    {
        //LOG_TRACE << "   DBHandleLog closing DB handle from syncWithiTunesDBIndexedBasedE" << LOG_ENDL;
        dBHandle.close();
    }

    if (!createConnection(m_serialNumber))
    {
        //LOG_TRACE << "   Sqlite Database connection not established" << LOG_ENDL;
        return;
    }
    SyncFlag=true;

    if( m_isDisconnected )
    {
        throw  IPOD_ERROR_DISCONNECT;
    }

    if(SyncFlag)
    {
        ret = completeSyncIndexbasedE();
    }

    //LOG_TRACE << "CTrackerConnector::syncWithiTunesDBIndexedBasedE() OUT" << LOG_ENDL;
    return;
}

void CTrackerConnector::fetchAllTrackTitlesE(int TrackCount)
{
    //LOG_TRACE << "CTrackerConnector::fetchAllTrackTitlesE() IN, TrackCount = " << TrackCount << LOG_ENDL;
    int chunkSize      = 50;
    int currentChunk   = 0;
    int completedCount = 0;

    if(TrackCount <= 0)
    {
        //LOG_TRACE  << "CTrackerConnector::fetchAllTrackTitlesE(), No track in iPod!!" << LOG_ENDL;
        return;
    }

    m_MusicTrackInfo.clear();
    //LOG_TRACE << "   TempDBcreation: Get all track info including podcast, audiobook all songs started" << LOG_ENDL;

    while(completedCount < TrackCount)
    {
        if((TrackCount - completedCount) > chunkSize)
        {
            currentChunk = chunkSize;
        }
        else
        {
            currentChunk = TrackCount - completedCount;
            chunkSize = TrackCount - completedCount;
        }

        m_PlayListTrackNames.clear();
        song_type = MusicTracks;
        m_iPodDBFetchTaskHandler->getList("Track",completedCount,chunkSize);
        //LOG_INFO << "   TempDBcreation : currentChunk = " <<currentChunk <<", completedCount = "<<completedCount << ", chunkSize = "<< chunkSize <<LOG_ENDL;

        waitForResponceFromExitCode(7000, ExitCode_GetList);

        if(m_PlayListTrackNames.count() <= 0){
            //LOG_TRACE << "   TempDBcreation : Fetch List Timeout.Fetch the old content again" <<LOG_ENDL;
            continue;
        }

        int currentCount = m_PlayListTrackNames.count();
        int trackIndex = completedCount;
        for( int nIndex = 0; nIndex < currentCount; nIndex++ )
        {
            SiPodDBTrackInfo indexedPlayingTrackAppleInfo ;
            indexedPlayingTrackAppleInfo.m_trackIndex = trackIndex;
            indexedPlayingTrackAppleInfo.m_Title    = m_PlayListTrackNames.value(trackIndex);
            m_MusicTrackInfo.append(indexedPlayingTrackAppleInfo);
            trackIndex++;
        }
        completedCount += currentCount;
    }
    //LOG_TRACE << "CTrackerConnector::fetchAllTrackTitlesE() IN, TrackCount = " << TrackCount << LOG_ENDL;
}

int CTrackerConnector::completeSyncIndexbasedE()
{
    //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE() IN" << LOG_ENDL;

    int  dNetTrackCount    = 0;
    int  total_track_count = 0;
    bool res  = false;
    bool bRtn = false;
    queryflag = false;
    m_SyncTypeForTable = completeSync;

    QString path      = IPODCONTROLLER_DBUS_PATH;
    QString interface = IPODCONTROLLER_DBUS_INTERFACE;

    if(m_iPodDBFetchTaskHandler != NULL)
    {
        if(m_serialNumber.isEmpty())
        {
            m_serialNumber = m_iPodDBFetchTaskHandler->getCurrentSerialNumberE();
            m_iPodPrivateData->setCurrentSerialNumber(m_serialNumber);
        }

        //LOG_INFO << "   m_serialNumber = " << m_serialNumber << LOG_ENDL;

        bRtn = createConnection(m_serialNumber);
        if( bRtn == false )
        {
            //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE(), Create Connection Failed!!!!!!!!!!!!!! "<< LOG_ENDL;
            return IPOD_ERROR_GENERIC;
        }

        CreateDataBaseAndTable();

        m_dateUpdateString = false;
        insertDBPropertiesToTableE();

        dNetTrackCount = 0;
        dNetTrackCount = m_iPodPrivateData->m_TotalIpodTrackCount;
        //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE(), netTrackCount = " <<  dNetTrackCount << LOG_ENDL;

        QDBusMessage signal = QDBusMessage::createSignal(path,interface,CONNECT_SIGNAL);
        signal << m_serialNumber;
        if(m_DbusConnection.send(signal))
        {
            //LOG_INFO << "   dbus sent success = " << m_serialNumber.toAscii().data() << " connected" << LOG_ENDL;
        }
        else
        {
            QDBusError error = m_DbusConnection.lastError();
            //LOG_INFO << "   dbus sent fail = " << error.message().toAscii().data() << LOG_ENDL;
        }

        //Clear the Old List.
        m_MusicTrackInfo.clear();

        bool isNano2G = (m_iPodPrivateData->m_iPodProductID == NANO_2G_PRODUCT_ID ? true : false);
        bool isNano1G = (m_iPodPrivateData->m_iPodProductID == NANO_1G_PRODUCT_ID ? true : false);

        if( dNetTrackCount > 0 )
        {
            m_TotalTracks = dNetTrackCount;

            PT_START_TIME(PT_OBJ_METADATA_FETCHING_INDEX, 0);
            //LOG_TRACE << "iPodPerformanceMeasurement:Index Based Scan START Get all track info including podcast, audiobook all songs started" << LOG_ENDL;
            m_ScanTime.start();

            int milliseconds = 0;
            int completescan_milliseconds = 0;
            int dintialCount = 0;

            int dB_UpdateTime = 0;
            bool timerstarted_DB = false;

            if( isNano2G )
            {
                //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE(), m_iPodDBFetchTaskHandler->makeSelectionSortedOnArtistE(\"Track\"), 0, \"Artist\")" << LOG_ENDL;
                m_iPodDBFetchTaskHandler->makeSelectionSortedOnArtistE("Track",0,"Artist");
            }
            else {
                //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE(), m_iPodDBFetchTaskHandler->makeSelectionE(\"Track\"), 0)" << LOG_ENDL;
                m_iPodDBFetchTaskHandler->makeSelectionE( "Track", 0 );
            }
            m_iPodDBFetchTaskHandler->pauseForTracker();
            QList<SiPodDBTrackInfo> MusicTrackTitleInfo;
            for( int nCurrentIndex = 0 ; nCurrentIndex < dNetTrackCount ; nCurrentIndex++ )
            {
                while(m_coverArtRequestInProgress)
                {
                    //LOG_TRACE << "basicp pausing on indexing for cover art" << LOG_ENDL;
                    waitForResponceFromExitCode(500, ExitCode_PauseIndexingStatus);
                }

                while(m_PauseDBIndexing)
                {
                    m_iPodPrivateData->m_IsDBIndexingPaused = true;
                    //LOG_TRACE << "PLAYLIST:  Trackerconnector::completeSyncIndexbasedE(), emit OnDBIndexingStatusChanged(true)_"  << LOG_ENDL;
                    emit OnDBIndexingStatusChanged(true);
                    waitForResponceFromExitCode( 500, ExitCode_PauseIndexingStatus );
                }

                m_iPodPrivateData->m_IsDBIndexingPaused = false;
                SiPodDBTrackInfo indexedPlayingTrackAppleInfo;
                if(isNano1G)
                {
                    indexedPlayingTrackAppleInfo = m_iPodDBFetchTaskHandler->getIndexedTrackMetaDataE(nCurrentIndex);
                }
                else
                {
#ifdef DBUGLOGS
                    //LOG_INFO << "nCurrentIndex = : " << nCurrentIndex << "CHUNK_SIZE = : "<<CHUNK_SIZE <<"dNetTrackCount = : " <<dNetTrackCount << LOG_ENDL;
#endif
                    if(nCurrentIndex < INITIAL_CHUNK_SIZE + 1)
                    {
                        if( dintialCount == 0 )
                        {
                            int count = m_TotalTracks;
                            if(count >= INITIAL_CHUNK_SIZE)
                            {
                                count = INITIAL_CHUNK_SIZE;
                            }

                            fetchAllTrackTitlesE(count);

                            dintialCount++;
                            MusicTrackTitleInfo = m_MusicTrackInfo;
                            m_MusicTrackInfo.clear();
                        }
                        indexedPlayingTrackAppleInfo = m_iPodDBFetchTaskHandler->getIndexedTrackMetaDataE(nCurrentIndex);
                    }
                    else
                    {
                        if(m_iPodPrivateData->m_SLeepForPlayWhileFetching > 0)   // Added by pawan 2013.05.03.
                            usleep(m_iPodPrivateData->m_SLeepForPlayWhileFetching);
                        indexedPlayingTrackAppleInfo = m_iPodDBFetchTaskHandler->getIndexedTrackMetaDataE(nCurrentIndex, 5);
                        indexedPlayingTrackAppleInfo.m_Title.append(MusicTrackTitleInfo[nCurrentIndex].m_Title);
                    }
                }

                checkSortingOrder(indexedPlayingTrackAppleInfo.m_Title, AppEngineBase::GetLanguage()); //added by junam 2013.06.07 iPod quick scroll
                m_MusicTrackInfo.append(indexedPlayingTrackAppleInfo);

                if (((nCurrentIndex+1) % dNetTrackCount)==0)
                {
                    milliseconds = m_ScanTime.elapsed();
                    completescan_milliseconds = completescan_milliseconds + milliseconds;
                    QString performanceTime = m_iPodPrivateData->getPerformanceTime( milliseconds );
                    PERFORMANCELOG ( "Sync:IPod INDEX Based Scan time is %s " , performanceTime.toAscii().data()  );
                    performanceTime.clear();
                    milliseconds =0;
                }
                if((((nCurrentIndex+1) % CHUNK_SIZE)==0) || (nCurrentIndex == (dNetTrackCount-1)) || ((nCurrentIndex+1) == INITIAL_CHUNK_SIZE))
                {
                    if( m_MusicTrackInfo.count() > 0 )
                    {
                        PT_END_TIME(PT_OBJ_METADATA_FETCHING_INDEX, 0);
                        PT_START_TIME(PT_OBJ_MASTERTABLE_UPDATE_METADATA, 0);

                        //LOG_TRACE << "iPodPerformanceMeasurement:Index Based Scan END updating MusicTracks to MasterTable start" << LOG_ENDL;
                        //LOG_TRACE << "iPodPerformanceMeasurement:Index Based SQL DB update for MusicTracks START"<<LOG_ENDL;

                        if(!timerstarted_DB)
                        {
                            timerstarted_DB = true;
                            m_DBUpdateTime.start();
                        }
                        else
                        {
                            m_DBUpdateTime.restart();
                        }

                        insertiPodTracksMetaInfoToMasterTable( m_MusicTrackInfo, "MasterTable" );

                        milliseconds  = m_DBUpdateTime.elapsed();
                        dB_UpdateTime = dB_UpdateTime + milliseconds;
                        //LOG_INFO << "   dB_UpdateTime for MT is " << dB_UpdateTime << LOG_ENDL;

                        PT_END_TIME(PT_OBJ_MASTERTABLE_UPDATE_METADATA, 0);
                        //LOG_TRACE << "iPodPerformanceMeasurement:Index Based SQL DB update END" << LOG_ENDL;
                        //LOG_INFO  << "   m_MusicTrackInfo [" << m_MusicTrackInfo.length() << "]" << LOG_ENDL;

#if EXTRA_DEBUG_LOGS
                        for(int i = 0; i < m_MusicTrackInfo.length(); i++)
                        {
                            SiPodDBTrackInfo indexedPlayingTrackAppleInfo = m_MusicTrackInfo.at(i) ;
                            //LOG_INFO << "UID =" << indexedPlayingTrackAppleInfo.m_trackIndex << LOG_ENDL;
                            //LOG_INFO << "Title=" << indexedPlayingTrackAppleInfo.m_Title.toAscii().data()  << LOG_ENDL;
                            //LOG_INFO << "Artist" << indexedPlayingTrackAppleInfo.m_Artist.toAscii().data() << LOG_ENDL;
                            //LOG_INFO << "Album=" << indexedPlayingTrackAppleInfo.m_Album.toAscii().data() << LOG_ENDL;
                            //LOG_INFO << "Genre=" << indexedPlayingTrackAppleInfo.m_Genre.toAscii().data() << LOG_ENDL;
                            //LOG_INFO << "Composer=" << indexedPlayingTrackAppleInfo.m_Composer.toAscii().data() << LOG_ENDL;
                            //LOG_INFO << "Type=" << indexedPlayingTrackAppleInfo.m_Type.toAscii().data() << LOG_ENDL;

                        }
#endif
                        m_MusicTrackInfo.clear();
                    }

                    // Modified by jonghwan.cho@lge.com 2013.11.10.
                    //if( nCurrentIndex == (INITIAL_CHUNK_SIZE - 1) )
                    if( nCurrentIndex == (INITIAL_CHUNK_SIZE - 1) || dNetTrackCount < INITIAL_CHUNK_SIZE )
                    {
                        /*Initial chunck of data is inserted in mastertable, emit autoplayready*/
                        //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE(), emit initAutoPlayReady(true, SYNC) for the first 5 songs" << LOG_ENDL;
                        m_SyncType = SYNC;
                        emitAutoPlayWaitForDone(true, m_SyncType);

                        //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE(), emit OnCurrentPlayingTrackCount(nCurrentIndex = " << nCurrentIndex << ")"  << LOG_ENDL;
                        emit OnCurrentPlayingTrackCount(nCurrentIndex + 1); // Added by pawan 2013.05.03.
                        //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE(), emit OnCurrentTrackCountChanged(nCurrentIndex = " << nCurrentIndex << ")"  << LOG_ENDL;
                        emit OnCurrentTrackCountChanged(nCurrentIndex + 1);//added by junam 2013.04.23 for index DB

                        fetchAllTrackTitlesE(dNetTrackCount);
                        MusicTrackTitleInfo = m_MusicTrackInfo;
                        m_MusicTrackInfo.clear();
                    }

                    PT_START_TIME(PT_OBJ_GRACENOTE, 0);
                    if( ((nCurrentIndex + 1) % CHUNK_SIZE) == 0 )
                    {
                        emitIndexDBusSignal(((nCurrentIndex+1)-CHUNK_SIZE), nCurrentIndex);
                        total_track_count=nCurrentIndex+1;
                        //LOG_INFO << "   signal emitted from " << ((nCurrentIndex+1)-CHUNK_SIZE) <<" to " << nCurrentIndex << LOG_ENDL;

                    }
                    else {
                        emitIndexDBusSignal(total_track_count, nCurrentIndex);
                        //LOG_TRACE << "   signal emitted from " << total_track_count <<" to " << nCurrentIndex << LOG_ENDL;
                    }

                    PT_END_TIME(PT_OBJ_GRACENOTE, 0);
                    //Send a DBus signal once chunk is updated : End

                    //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE(), emit OnCurrentPlayingTrackCount(nCurrentIndex = " << nCurrentIndex << ")"  << LOG_ENDL;
                    emit OnCurrentPlayingTrackCount(nCurrentIndex + 1);//added by junam 2013.04.23 for index DB
                    //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE(), emit OnCurrentTrackCountChanged(nCurrentIndex = " << nCurrentIndex << ")"  << LOG_ENDL;
                    emit OnCurrentTrackCountChanged(nCurrentIndex + 1);
                }
            }

            QString string;
            //LOG_INFO << "   Final dB_UpdateTime is " << dB_UpdateTime << LOG_ENDL;
            string = m_iPodPrivateData->getPerformanceTime(dB_UpdateTime);
            PERFORMANCELOG ( "Sync:IPod INDEX Based DB Update time is %s " , string.toAscii().data()  );
            string.clear();
            completescan_milliseconds = completescan_milliseconds + dB_UpdateTime;
            string = m_iPodPrivateData->getPerformanceTime(completescan_milliseconds);
            PERFORMANCELOG ( "Sync:IPod INDEX Based Complete Scan time is %s " , string.toAscii().data()  );

            //LOG_TRACE << "iPodPerformanceMeasurement: Get all track info including podcast, audiobook all songs End" << LOG_ENDL;

            bool demptyTableFlag = false;
            int  tablecheckCount = EmptyTableCheck(dNetTrackCount);
            if( tablecheckCount == 0 )
            {
                demptyTableFlag = true;
            }

            if(dBHandle.isValid())
            {
                if(dBHandle.open())
                {
                    //LOG_TRACE << "  DBHandleLog closing DB handle from completeSyncIndexbasedE" << LOG_ENDL;
                    dBHandle.close();
                }
                //LOG_TRACE << "  DBHandleLog removing DB handle from completeSyncIndexbasedE" << LOG_ENDL;
                dBHandle.removeDatabase(dBHandle.connectionName());
            }

            QString Existing_DB(PERM_DBPATH);
            Existing_DB.append(m_serialNumber);
            QFile f1(Existing_DB);

            if (f1.exists())
            {
                if(res)
                {
                    //LOG_INFO << "   File Remove operation from FS succeeded" << LOG_ENDL;
                }
                else
                {
                    //LOG_TRACE << "   File Remove operation from FS Failed: file not exists"<<LOG_ENDL;
                }
            }

            //LOG_INFO << "   File copy 5 Temp to Perm PERM_DBPATH = " <<LOG_ENDL;
            res = FileCopy(TEMP_DBPATH,PERM_DBPATH);

            if(res)
            {
                //LOG_INFO << "   File copy to FS after Sync operation succeeded" << res << LOG_ENDL;

                DeleteTmpfsDB();

                if (!createConnection(m_serialNumber))
                {
                    //LOG_TRACE << "   Sqlite Database connection not established" << LOG_ENDL;
                }
                dropOldTable();
            }
            else
            {
                //LOG_TRACE << "   File copy to FS after Sync operation Failed" << LOG_ENDL;
            }

            PT_START_TIME(PT_OBJ_GRACENOTE, 0);
            m_SyncTypeForTable = SyncCompleted;
            //LOG_INFO << "   Emitting emitIndexDBusSignal with SyncCompleted signal =  " << SyncCompleted << LOG_ENDL;
            emitIndexDBusSignal(0, 0);
            PT_END_TIME(PT_OBJ_GRACENOTE, 0);
        }
    }
    else
    {
        //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE(), m_iPodDBFetchTaskHandler == NULL" << LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }

    //LOG_TRACE << "CTrackerConnector::completeSyncIndexbasedE() OUT" << LOG_ENDL;
    return dNetTrackCount;
}

bool CTrackerConnector::cleanUpOnDisconnect()
{
    //LOG_TRACE << "CTrackerConnector::cleanUpOnDisconnect() IN" << LOG_ENDL;

    bool result = true;
    if( m_cleanUpdone )
    {
        //LOG_TRACE << "   Already cleaned up!!" << LOG_ENDL;
        return result;
    }
    m_cleanUpdone = true;

    QDBusMessage signal = QDBusMessage::createSignal(IPODCONTROLLER_DBUS_PATH,IPODCONTROLLER_DBUS_INTERFACE,DISCONNECT_SIGNAL);
    signal << m_serialNumber;
    signal << m_iPodPrivateData->getiPodPortNum();

    if( m_DbusConnection.send(signal) )
    {
        //LOG_TRACE << "   dbus sent success" << LOG_ENDL;
    }
    else
    {
        QDBusError error = m_DbusConnection.lastError();
        //LOG_INFO << "   dbus sent fail, error message = " << error.message().toAscii().data() << LOG_ENDL;
        result = false;
    }

    //LOG_TRACE << "CTrackerConnector::cleanUpOnDisconnect() OUT" << LOG_ENDL;
    return result;
}

void CTrackerConnector::insertiPodTracksMetaInfoToMasterTable(QList<SiPodDBTrackInfo> MtrackList, QString tableName)
{
    ////LOG_TRACE << "CTrackerConnector::insertiPodTracksMetaInfoToMasterTable() IN" << LOG_ENDL;

    for(int kk=0 ; kk <MtrackList.count() ; kk++)
    {
        if(MtrackList[kk].m_Album.contains(doubleQuote))
        {
            MtrackList[kk].m_Album.replace(doubleQuote, doubleQuoteReplacement);
        }
        if(MtrackList[kk].m_Artist.contains(doubleQuote))
        {
            MtrackList[kk].m_Artist.replace(doubleQuote, doubleQuoteReplacement);
        }
        if(MtrackList[kk].m_Composer.contains(doubleQuote))
        {
            MtrackList[kk].m_Composer.replace(doubleQuote, doubleQuoteReplacement);
        }
        if(MtrackList[kk].m_Genre.contains(doubleQuote))
        {
            MtrackList[kk].m_Genre.replace(doubleQuote, doubleQuoteReplacement);
        }
        if(MtrackList[kk].m_Title.contains(doubleQuote))
        {
            MtrackList[kk].m_Title.replace(doubleQuote, doubleQuoteReplacement);
        }
    }

    QString   fpathURL;
    QSqlError err;
    queryflag     = false;
    int syncState = 0;

    bool bRet = createConnection(m_serialNumber);
    if( bRet == true )
    {
        ////LOG_TRACE << "CTrackerConnector::insertiPodTracksMetaInfoToMasterTable() connection True" << LOG_ENDL;
    }
    else
    {
        //LOG_TRACE << "CTrackerConnector::insertiPodTracksMetaInfoToMasterTable() connection FAILED!!!!" << LOG_ENDL;
        return;
    }

    QSqlQuery query(dBHandle);
    QString   queryString;
    int cnt         = 0;
    int total_count = MtrackList.count();

    for( int i = 0 ; i< total_count ; i++ )
    {
        if( cnt == 0 )
        {
            queryString = (QString("INSERT INTO \"%1\" (T_index, T_UID, T_Title, T_Album, T_Artist, T_Genre, T_Composer, T_Type, T_TrackCount, T_TopLevelIndex, T_TopLevelName, T_TypeIndex, T_Mldb, T_FilePath,  T_Duration, T_GraceNoteFlag)").arg(tableName));
        }

        SiPodDBTrackInfo trackAppleInfo = MtrackList[i];
        QString          index1;
        index1.setNum(trackAppleInfo.m_trackIndex);
        QString          UID;

        checkSortingOrder( trackAppleInfo.m_Title, AppEngineBase::GetLanguage() );//added by junam 2013.06.07 iPod quick scroll
        if(trackAppleInfo.m_UID != 0)
        {
            UID.setNum(trackAppleInfo.m_UID);
        }
        else
        {
            //UID.setNum(CIpodControllerGlobal::getUIDForStrings(trackAppleInfo.m_Album, trackAppleInfo.m_Artist, trackAppleInfo.m_Title, trackAppleInfo.m_trackDuration));
            static quint64 fakeuid = 1000000;
            UID.setNum(fakeuid);
            fakeuid++;
            if(fakeuid > 50000000)
            {
                fakeuid = 1000000;
            }
        }
        QString subQuery;
        QString temp("1");
        QString temp1("-1");

        fpathURL.clear();
        fpathURL.append(fVolumePath_IPod);
        fpathURL.append(fForwardSlash);
        fpathURL.append(m_serialNumber);
        fpathURL.append(fForwardSlash);
        fpathURL.append(trackAppleInfo.m_Type);
        fpathURL.append(fForwardSlash);
        fpathURL.append(index1);

        fpathURL.append(fUnderscoreChar);
        fpathURL.append(UID);
        if(trackAppleInfo.m_Type.compare(gIPODAllSongType))
        {
            fpathURL.append(fUnderscoreChar);
            fpathURL.append(temp1);
        }

        QString duration;
        duration.setNum(trackAppleInfo.m_trackDuration);
        subQuery.append(QString(" SELECT \"%1\", \"%2\", \"%3\", \"%4\",\"%5\", \"%6\", \"%7\",\"%8\",\"%9\", \"%10\", \"%11\", \"%12\", \"%13\", \"%14\", \"%15\", \"%16\"").arg(index1).arg(UID).arg(trackAppleInfo.m_Title).arg(trackAppleInfo.m_Album).arg(trackAppleInfo.m_Artist).arg(trackAppleInfo.m_Genre).arg(trackAppleInfo.m_Composer).arg(trackAppleInfo.m_Type).arg(temp).arg(temp1).arg(trackAppleInfo.m_Album).arg(index1).arg(temp1).arg(fpathURL).arg(duration).arg(syncState));
        
        queryString.append(subQuery);
        cnt++;

        if( cnt == 100 )
        {
            if( dBHandle.isValid() == true )
                queryflag = query.exec(queryString);
            else
                queryflag = false;

            if(!queryflag)
            {
                err = query.lastError();
                //LOG_INFO << "   Insert last error is = " << err.text() << LOG_ENDL;
            }

            cnt = 0;
            queryString.clear();
        }
        else if((cnt!=0) && (i!=(total_count-1)))
        {
            queryString.append(" UNION ALL");
            continue;
        }
        if(i==(total_count-1))
        {
            if( dBHandle.isValid() == true )
                queryflag = query.exec(queryString);
            else
                queryflag = false;

            if(!queryflag) {
                err = query.lastError();
                //LOG_INFO << "   last error is = " << err.text() << LOG_ENDL;
            }
        }
    }
    ////LOG_TRACE << "CTrackerConnector::insertiPodTracksMetaInfoToMasterTable() OUT" << LOG_ENDL;
}

bool CTrackerConnector::FileCopy( QString SrcFile, QString DestFile )
{
    //LOG_TRACE << "CTrackerConnector::FileCopy() IN" << LOG_ENDL;
    bool cpy = false;

    SrcFile.append(m_serialNumber);
    DestFile.append(m_serialNumber);
    cpy = QFile::copy(SrcFile,DestFile);
    //LOG_TRACE << "CTrackerConnector::FileCopy(), SrcFile = " << SrcFile << ", DestFile = " << DestFile << LOG_ENDL;
    sync();
    //LOG_TRACE << "CTrackerConnector::FileCopy() OUT" << LOG_ENDL;
    return cpy;
}

void CTrackerConnector::emitAutoPlayWaitForDone(bool status, int syncType)
{
    //LOG_TRACE << "CTrackerConnector::emitAutoPlayWaitForDone() IN" << LOG_ENDL;
    //LOG_TRACE << "CTrackerConnector::emitAutoPlayWaitForDone(), emit initAutoPlayReady( status = " << status << ", syncType = " << syncType << ")" << LOG_ENDL;
    emit initAutoPlayReady(status, syncType);

    waitForResponceFromExitCode(3000, ExitCode_AllExit);
    //LOG_TRACE << "CTrackerConnector::emitAutoPlayWaitForDone() OUT" << LOG_ENDL;
}

void CTrackerConnector::autoPlayComplete()
{
    //LOG_TRACE << "CTrackerConnector::autoPlayComplete() IN"<<LOG_ENDL;
    m_waitForAutoplayCond.wakeAll();
    //LOG_TRACE << "CTrackerConnector::autoPlayComplete() OUT"<<LOG_ENDL;
}

void CTrackerConnector::cleanUPOnSyncComplete()
{
    /* clear all global lists used in sync */
    m_MusicTrackInfo.clear();

    m_PlaylistNames.clear();
    m_PlayListTrackNames.clear();

    /*Clear task handler used for sync */
    m_iPodDBFetchTaskHandler->stopTaskHandler();
    m_artworkFetchTaskHandler->stopTaskHandler();
}

void CTrackerConnector::dropOldTable()
{
    //LOG_TRACE << "CTrackerConnector::dropOldTable() IN" << LOG_ENDL;

    QSqlQuery query(dBHandle);
    QSqlError err;
    query.clear();

    if( dBHandle.isValid() == true )
        queryflag = query.exec("drop table OldTable");
    else
        queryflag = false;

    if(!queryflag)
    {
        err = query.lastError();
        //LOG_INFO << "   Error while deleting the OldTable, error text =  " << err.text() << LOG_ENDL;
    }

    //LOG_TRACE << "CTrackerConnector::dropOldTable() OUT" << LOG_ENDL;
}

int CTrackerConnector::EmptyTableCheck(int onDeviceTrackCount)
{
    //LOG_TRACE << "CTrackerConnector::EmptyTableCheck() IN, onDeviceTrackCount = " << onDeviceTrackCount <<LOG_ENDL;

    QSqlQuery query(dBHandle);
    QSqlError err;
    int dtotaltrackcount_aftersync = -1;

    if( dBHandle.isValid() == true )
        queryflag = query.exec("select count(*) from MasterTable");
    else
        queryflag = false;

    if(queryflag)
    {
        while(query.next())
        {
            dtotaltrackcount_aftersync = query.value(0).toInt();
            //LOG_TRACE <<"   Track count in MasterTable after sync complete = "<< dtotaltrackcount_aftersync << LOG_ENDL;
        }
        // If MasterTable Count is zero. Empty signal to HMI saying No Payable File.
        if(dtotaltrackcount_aftersync == 0)
        {
            //LOG_TRACE << "CTrackerConnector::EmptyTableCheck(), emit IpodTracksAfterSync(0)_"  << ", Track count zero after sync complete = " << dtotaltrackcount_aftersync << LOG_ENDL;
            emit IpodTracksAfterSync(0);
            return 0;
        }
        else
        {
            if( dBHandle.isValid() == true )
                queryflag = query.exec("select count(*) from MasterTable where T_Duration = 0");
            else
                queryflag = false;

            if(queryflag)
            {
                int dtrackCount_durationZero = -1;
                while(query.next())
                {
                    dtrackCount_durationZero = query.value(0).toInt();
                    //LOG_TRACE <<"   Track count with duration zero after sync complete :"<< dtrackCount_durationZero << LOG_ENDL;
                }
                // Check the track duration for all the songs and if the count is same as Master table count
                if((dtrackCount_durationZero == dtotaltrackcount_aftersync))
                {
                    //LOG_TRACE << "CTrackerConnector::EmptyTableCheck(), emit IpodTracksAfterSync(0)_"  << ", Emitted Track count with duration zero to HMI = " << dtotaltrackcount_aftersync << LOG_ENDL;
                    emit IpodTracksAfterSync(0);
                    return 0;
                }
            }
            else
            {
                err = query.lastError();
                //LOG_INFO << "   error while retrieving count for T_Duration, error text = " << err.text() << LOG_ENDL;
            }

            //get count of only songs excluding audiobook,podcast,iTunesU
            if( dBHandle.isValid() == true )
                queryflag = query.exec("select count(*) from MasterTable where T_Type =  'AllSongs'");
            else
                queryflag = false;

            if( queryflag )
            {
                int dtotalSongCountDB = -1;

                while( query.next() )
                {
                    dtotalSongCountDB = query.value(0).toInt();
                    //LOG_TRACE <<"   Track count in MasterTable after sync complete = "<< dtotalSongCountDB << LOG_ENDL;
                }
                // if total songs in device and the songs that is appended in MasterTable are different
                if(dtotalSongCountDB != onDeviceTrackCount )
                {
                    //LOG_TRACE << "CTrackerConnector::EmptyTableCheck(), emit IPodPlayableSongCount( DB SongCount = " << dtotalSongCountDB <<
                    //            ", Device SongCount = " << onDeviceTrackCount << ")"  << LOG_ENDL;
                    emit IPodPlayableSongCount( dtotalSongCountDB, onDeviceTrackCount );
                }
            }
            else
            {
                err = query.lastError();
                //LOG_TRACE << "   error while retrieving AllSongs count for MasterTable, error text = " << err.text() << LOG_ENDL;
            }
        }

    }
    else
    {
        err = query.lastError();
        //LOG_TRACE << "   error while retrieving count for MasterTable, error text = " << err.text() << LOG_ENDL;
    }
    //LOG_TRACE << "CTrackerConnector::EmptyTableCheck() OUT" << LOG_ENDL;
    return dtotaltrackcount_aftersync;
}

void CTrackerConnector::DeleteTmpfsDB()
{
    //LOG_TRACE << "CTrackerConnector::DeleteTmfsDB() IN" << LOG_ENDL;

    QString tmpfs_DB(TEMP_DBPATH);
    tmpfs_DB.append(m_serialNumber);

    if( QFile::exists(tmpfs_DB) )
    {
        bool res = QFile::remove(tmpfs_DB);
        if( res )
        {
            //LOG_TRACE << "   File deleted from TempFS(" << tmpfs_DB << ")" << LOG_ENDL;
            //LOG_TRACE << "CTrackerConnector::DeleteTmpfsDB(), emit initDBSyncComplete( true )_"  << LOG_ENDL;
            emit initDBSyncComplete(true); // added by jungae 2013.01.03 for ISV#68168 sometimes iPod reading popdup is not disappeared
        }
        else
        {
            //LOG_TRACE << "   File deletion from TempFS(" << tmpfs_DB << ") Failed" << LOG_ENDL;
        }
    }

    //LOG_TRACE << "CTrackerConnector::DeleteTmfsDB() OUT" << LOG_ENDL;
}

bool CTrackerConnector::createConnection( QString str )
{

    ////LOG_TRACE << "DBHandleLog CTrackerConnector::createConnection IN" << LOG_ENDL;
    bool    retVal = false;
    QString dbName = TEMP_DBPATH;
    dbName.append(str);

    if(!QFile::exists(dbName))
    {
        dbName.clear();
        dbName.append(PERM_DBPATH);
        dbName.append(str);

        if(!QFile::exists(dbName))
        {
            dbName.clear();
            dbName.append(TEMP_DBPATH);
            dbName.append(str);
        }
    }

    if((QFile::exists(dbName))&&(QSqlDatabase::contains(str)))
    {
        dBHandle = QSqlDatabase::database(str);
        dBHandle.setDatabaseName(dbName);
        if(dBHandle.isOpen())
        {
            retVal = true;
        }
        else
        {
            if (!dBHandle.open())  { retVal = false; }
            else                   { retVal = true; }
        }
    }
    else
    {
        dBHandle = QSqlDatabase::addDatabase("QSQLITE",str);
        dBHandle.setDatabaseName(dbName);

        if (!dBHandle.open())  { retVal = false; }
        else                   { retVal = true; }
    }
    ////LOG_TRACE << "DBHandleLog CTrackerConnector::createConnection OUT" << LOG_ENDL;
    return retVal;
}

void CTrackerConnector::requestQuitSlot()
{
    if( isReadyToDelete() == true )
    {
        ////LOG_TRACE <<"CiPodDBSyncThread::requestQuitSlot isReadyToDelete == TRUE Thread ID = " << QThread::currentThreadId() << LOG_ENDL;
        emit dbThreadQuit();
    }
    else
    {
        ////LOG_TRACE <<"CiPodDBSyncThread::requestQuitSlot isReadyToDelete == FALSE Thread ID = " << QThread::currentThreadId()<< LOG_ENDL;
        emit requestQuit();
    }
}

void CTrackerConnector::OnChangeDBIndexingStatus(bool pause)
{
    //LOG_TRACE << "PLAYLIST: CTrackerConnector::OnChangeDBIndexingStatus() IN, pause = " << pause << LOG_ENDL;

    try
    {
    if(m_PauseDBIndexing == true && pause == true)
    {
        //LOG_TRACE << "PLAYLIST: CTrackerConnector::OnChangeDBIndexingStatus(), emit OnDBIndexingStatusChanged(true)_"  << LOG_ENDL;
        emit OnDBIndexingStatusChanged(true);
    }

    m_PauseDBIndexing = pause;
    if(!pause && (m_iPodPrivateData->m_IsConvertedFromUIDToIndex || m_iPodPrivateData->m_ExtendedLingoVersion >= 1.14))
    {
        sleep(1);
        m_iPodDBFetchTaskHandler->resetPreviousSelectionsE();
        int totalCount = m_iPodDBFetchTaskHandler->getCategorizedListCountE("Track");
        //LOG_TRACE << "PLAYLIST: total count after reset: " << totalCount << LOG_ENDL;
    }

    if(!pause)
    {
        m_iPodPrivateData->m_IsDBIndexingPaused = false;
        //LOG_TRACE << "PLAYLIST: CTrackerConnector::OnChangeDBIndexingStatus(), emit OnDBIndexingStatusChanged(false)_"  << LOG_ENDL;
        emit OnDBIndexingStatusChanged(false);
    }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE<<"Jonghwan, onChangeDBIndexingStatus exception caught!"<<LOG_ENDL;
        return;
    }

    //LOG_TRACE << "PLAYLIST: CTrackerConnector::OnChangeDBIndexingStatus() OUT" << LOG_ENDL;
}

void CTrackerConnector::chopExceptionWords(QString &str)
{
    if(str.startsWith("A ")|| str.startsWith("a "))
    {
        str.remove(0, 2);
    }
    else if(str.startsWith("An ")||str.startsWith("an "))
    {
        str.remove(0, 3);
    }
    else if(str.startsWith("The ")||str.startsWith("the "))
    {
        str.remove(0, 4);
    }
}

void CTrackerConnector::checkSortingOrder(QString &strCompletetitle, LANGUAGE_T language)
{
    //LOG_TRACE << "checkSortingOrder() IN" << m_currentLanguage <<LOG_ENDL;

    if(language != LANGUAGE_AR && language != LANGUAGE_KO)
    {
        m_sortingOrder = 0;
        return;
    }

    if (strCompletetitle.isEmpty())
        return;

    QString title = strCompletetitle.trimmed();

    //LOG_TRACE << "strCompletetitle: " << strCompletetitle<<LOG_ENDL;
    chopExceptionWords(title);
    //LOG_TRACE << "title: " << title<<LOG_ENDL;

    QChar firstchar = title.at(0);
    ushort firstCharCode = firstchar.unicode();
    //LOG_TRACE << "firstChar: " <<firstchar << LOG_ENDL;
    //LOG_TRACE << "firstCharCode: " <<firstCharCode << LOG_ENDL;

    if (m_sortingOrder)
        return;

    int iLanguage = m_currentLanguage;
    ushort uKeyCode;

    switch(language)
    {
    case LANGUAGE_AR:
        //LOG_TRACE << "LANGUAGE_AR" <<LOG_ENDL;
        uKeyCode = title.at(0).unicode();
        if((uKeyCode >= 0x0041 && uKeyCode <= 0x005A )||
                (uKeyCode >= 0x0061 && uKeyCode <= 0x007A))
        {
            iLanguage = 0;
        }
        else if (uKeyCode >= 0x0600 && uKeyCode <= 0x06FF )
        {
            iLanguage = 1;
        }
        break;
    case LANGUAGE_KO:
        //LOG_TRACE << "LANGUAGE_KO" <<LOG_ENDL;
        uKeyCode = title.at(0).unicode();
        if((uKeyCode >= 0x0041 && uKeyCode <= 0x005A )||
                (uKeyCode >= 0x0061 && uKeyCode <= 0x007A))
        {
            iLanguage = 0;
        }
        else if (uKeyCode >= 0xAC00 && uKeyCode <= 0xD7AF )
        {
            iLanguage = 1;
        }
        break;
    /*
    default:
        //LOG_TRACE << "LANGUAGE_DEFAULT" <<LOG_ENDL;
        m_sortingOrder = 0;
        break;
    */
    }

    if( iLanguage < m_currentLanguage )
    {
        //LOG_TRACE <<"   currentLanguage Found, uKeyCode = " << uKeyCode;
        m_sortingOrder = 1;
    }

    m_currentLanguage = iLanguage;

    //LOG_TRACE << "CTrackerConnector::checkSortingOrder() OUT" << m_currentLanguage << LOG_ENDL;
}
