/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
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
#include <QString>
#include <QVariantMap>

#include <DHAVN_IPodController_playEngineTaskHandler.h>
#include <DHAVN_IPodController_playCurrentSelection.h>
#include <DHAVN_IPodController_ack.h>
#include <DHAVN_IPodController_playControl.h>
#include <DHAVN_IPodController_setiPodStateInfo.h> // Added by sungha.choi 	2013.04.11.
#include <DHAVN_IPodController_setRepeat.h>
#include <DHAVN_IPodController_setShuffle.h>
#include <DHAVN_IPodController_eventloopTimer.h>
#include <DHAVN_IPodController_playStatusChangeNotification.h>
#include <DHAVN_IPodController_setPlayStatusChangeNotification.h>
#include <DHAVN_IPodController_getCurrentPlayingTrackIndex.h>
#include <DHAVN_IPodController_returnCurrentPlayingTrackIndex.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackTitle.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackTitle.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackArtist.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackAlbum.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackTitle.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackTitle.h> //added by junam 2013.04.15
#include <DHAVN_IPodController_returnIndexedPlayingTrackArtist.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackArtist.h> //added by junam 2013.04.15
#include <DHAVN_IPodController_returnIndexedPlayingTrackAlbum.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackAlbum.h> //added by junam 2013.04.15
#include <DHAVN_IPodController_getIndexedPlayingTrackInfo.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackInfo.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_iPodDBFetchTaskHandler.h>
#include <DHAVN_IPodController_getPlayStatus.h>
#include <DHAVN_IPodController_returnPlayStatus.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iPodNotification.h>  // Added by pawan 2013.05.15.
#include <DHAVN_IPodController_TrackNewAudioAttributes.h>
#include <DHAVN_IPodController_RetAccSampleRateCaps.h>
#include <DHAVN_IPodController_AccAck.h>
#include <DHAVN_IPodController_IPodAck.h>
#include <DHAVN_IPodController_authorisor.h>
#include <DHAVN_IPodController_deviceDetector.h>
#include <DHAVN_IPodController_PerformanceLog.h>
#include <DHAVN_IPodController_DBconnection.h>
#include <DHAVN_IPodController_iPodDBIndexFetchTaskHandler.h>
#include <DHAVN_IPodController_getNumOfPlayingTracks.h>      // Added by pawan 2013.05.06.
#include <DHAVN_IPodController_returnNumOfPlayingTracks.h>   // Added by pawan 2013.05.06.
#include <DHAVN_IPodController_setRemoteEventNotification.h>  // Added by jonghwan.cho@lge.com  2013.04.16.
#include <DHAVN_IPodController_remoteEventNotification.h>     // Added by jonghwan.cho@lge.com  2013.04.16.
#include <DHAVN_IPodController_returnCategorizedDatabaseRecords.h>      // Added by wooju.lee@lge.com 2013.05.23.
#include <DHAVN_IPodController_retrieveCategorizedDatabaseRecords.h>    // Added by wooju.lee@lge.com 2013.05.23.
#include <DHAVN_IPodController_setCurrentPlayingTrack.h>
#include "DHAVN_EventLoop.h"
#include "DHAVN_EventLoopTimerExitCode.h"
#include <DHAVN_IPodController_getUIDTrackInfo.h>
#include <DHAVN_IPodController_retUIDTrackInfo.h>
#include <DHAVN_IPodController_getPBTrackInfo.h>
#include <DHAVN_IPodController_retPBTrackInfo.h>
#include <DHAVN_IPodController_prepareUIDList.h>
#include <DHAVN_IPodController_playPreparedUIDList.h>
#include <DHAVN_IPodController_iOSAppTaskHandler.h>
#include <DHAVN_IPodController_RequestIdentify.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QElapsedTimer>
#include <QProcess>
#include <fcntl.h>

#define INITIAL_CHUNK_SIZE         5
#define SCAN_DURATION              12000
#define ONESECOND                  1000
#define USLEEPDELAY                2
#define DEFAULT_IAPCOMMAND_TIMEOUT 5000

#define BYTE                       8
#define MAX_PAYLOAD_SIZE           206

using namespace DigitalAudioLingo;
using namespace ExtendedLingo;
using namespace DisplayRemoteLingo; // Added by sungha.choi 	2013.04.11.

CPlayEngineTaskHandler::CPlayEngineTaskHandler(QObject* parent, iPodLogger *iLog, int repeatMode, int randomMode)
    : CTaskHandler(parent),m_ScanMode(SCAN_OFF),m_ControllerPlaylistType(gIPODAllSongType),m_TopLevelSelected(false),m_TopLevelIndex(-3),m_emitStateForNONIDPS(false),m_RandomPlayCount(0),m_AutoplayIndex(0),isNotPassOnToNextSong(true),m_IsSyncInProgress(true),m_toDelete(false),m_eCurrentSrcType(ENone)
{
    m_PBRetrievedUID = 0;
    m_bCoverArtUpdated = true;

    m_nMetadataRetryCount = 0;

    m_nIPodReturnedMediaDuration = 0;

    m_bDBSelectionHasChanged = true;

    m_RepeatMode = repeatMode;
    m_RandomMode = randomMode;

    m_ReadyToDeleteFromSignal = true;
    m_nRunningLoopFromSignal = 0;


    m_iPodPrivateData = parent->findChild<iPodControllerPrivateData*>(gControllerPrivateData);
    switch(m_iPodPrivateData->m_currentPortNum){
    case eIPOD_PORT_1:
        LOG_INIT_V2(giPodControllerLogName_1);
        m_eCurrentSrcType = EIpodFront;
        break;
    case eIPOD_PORT_2:
        LOG_INIT_V2(giPodControllerLogName_2);
        m_eCurrentSrcType = EIpodRear;
        break;
    default:
        LOG_INIT_V2(giPodControllerLogName);
    }

    m_CommandStatus = false;
    m_FirstTimePlay = true;
    m_CurrentIndex = -1 ;
    m_CurrentMediaPosition = 0;
    m_IsGetCurrentFileName = false;
    m_StateNotificationSet = false;
    m_isDisconnected = false;
    m_AuthenticationPassed = false;
    m_audioConnected = false;
    piPodLog = iLog;     // added by sungha.choi 2013.04.12
    m_pLoop = NULL;
    m_pWaitTimer = NULL;
    m_ReadyToDelete = true;
    m_isRunningLoop = false;    // added by sungha.choi 2013.04.13
    m_NumberOfPlayingTracks = 0;  // Added by pawan 2013.05.03.

    if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
    {
        m_NumberOfPlayingTracksReceivedFromIPod = 0;
    }
    else
    {
        m_NumberOfPlayingTracksReceivedFromIPod = m_iPodPrivateData->m_TotalIpodTrackCount;
    }
    m_newNumberOfTracks = 0;   // Added by pawan 2013.05.15.
    m_FirstTimePlayFileName = QString();  // Added by pawan 2013.05.04.
    if(m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_1)
    {
        m_audioSource = "plughw:2,0";
        ////LOG_TRACE << "carrie: CPlayEngineTaskHandler::CPlayEngineTaskHandler Entry Port1 source= " << m_audioSource << LOG_ENDL;
    }
    else if(m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_2)
    {
        m_audioSource = "plughw:2,0";
        ////LOG_TRACE << "carrie: CPlayEngineTaskHandler::CPlayEngineTaskHandler Entry Port2 source= " << m_audioSource << LOG_ENDL;
    }

//    m_pipeCreated = false;
//    m_pipeThread = NULL;
    m_bPipeCreated = false;
    m_SampleRate = 44100;//Default rate.
    m_ControllerPlaylist.clear();
    m_isDurationRequested = false;
    m_CurrentIndexEmitted = -1;
    m_SetPlaylistCount = 1;
    m_CurrentMediaDuration = 0;
    m_bCurrentTrackHasChapter = false;
    m_PlaylistURIs.clear();
    m_SyncType = 0;
    m_FirstPositionChangeCount = 0;
    m_ScanPlayCount = 0;
    m_scanFlag = false;
    m_pauseScanTimeFlag = false;
    m_scanElapseTime =0;
    m_CurrentPlaylistIndex = -1;
    m_MediaInfo.Title = " ";
    m_MediaInfo.Album = " ";
    m_MediaInfo.Artist = " ";
    m_CurrentFileInfoString.clear();
    m_LocalTransId = 0;
    m_PlayStatusNotifyState = eDefault;
    m_CurrentCommand = 0x00;
    m_JumpInProgress = false;
    m_CurrentTime = QDateTime();
    m_PlayEngineTimer = NULL;
    m_ExpectedCommandID = 0x00;
    m_CurrentTrackIndexReceived = -1;    // Added by pawan 2013.05.28.
    m_PlayEngineTimer = new QTimer(this);
    m_playControlStopSeekFlag = false; // Added FF End functionality for classic
    m_PlayNextSongOnFF = false;
    m_IsPlaylistSetByDH = false;
    m_IsPlayListSpecificCategoryIsSet = false;
    m_PreparePlaylistStatus = false;
    m_CategoryType = DHTrackCategory;
    m_CurrentPlayingUID = 0;

    m_PBTrackInfoQueue.clear();
    m_PBRequestInProgress.nRequestIndex = -1;
    m_PBRequestInProgress.nRequestType = RequestType_None;
    m_PBRequestInProgress.queryId = eDefault_Id;
    m_PBRequestInProgress.reqUID = 0;

    if(m_PlayEngineTimer)
    {
        connect(m_PlayEngineTimer, SIGNAL(timeout()), this, SLOT(getPlayStatusATS()));
    }

    m_PlayIpodFilesTimer = new QTimer(this);
    if(m_PlayIpodFilesTimer)
    {
        m_PlayIpodFilesTimer->setSingleShot(true);
        m_PlayIpodFilesTimer->setInterval(5000);
        connect(m_PlayIpodFilesTimer, SIGNAL(timeout()), this, SLOT(handlePlayIpodFilesSlot()));
    }

    m_HandleStopTimer = new QTimer(this);
    if(m_HandleStopTimer)
    {
        m_HandleStopTimer->setSingleShot(true);
        m_HandleStopTimer->setInterval(5000);
        connect(m_HandleStopTimer, SIGNAL(timeout()), this, SLOT(handlePlaybackStoppedSlot()));
    }

    m_DelayMetadataIndexQueryTimer = new QTimer(this);
    if(m_DelayMetadataIndexQueryTimer)
    {
        m_DelayMetadataIndexQueryTimer->setSingleShot(true);
        connect(m_DelayMetadataIndexQueryTimer, SIGNAL(timeout()), this, SLOT(delayMetadataIndexQuerySlot()));
    }

    m_DelayGetOnDemandMetaInfoTimer = new QTimer(this);
    if(m_DelayGetOnDemandMetaInfoTimer)
    {
        m_DelayGetOnDemandMetaInfoTimer->setSingleShot(true);
        connect(m_DelayGetOnDemandMetaInfoTimer, SIGNAL(timeout()), this, SLOT(delayGetOnDemandMetaInfoSlot()));
    }

    m_PBTrackInfoTimer = new QTimer(this);
    if(m_PBTrackInfoTimer)
    {
        m_PBTrackInfoTimer->setSingleShot(true);
        connect(m_PBTrackInfoTimer, SIGNAL(timeout()), this, SLOT(PBTrackInfoTimeOutSlot()));
    }

    qRegisterMetaType<QByteArray>("QByteArray");

    connect(((CIpodController*) parent)->m_Reader,SIGNAL(packetAvailable(int, int, QByteArray)),
            this, SLOT(packetAvailable(int,int,QByteArray)),
            Qt::QueuedConnection);
    connect(((CIpodController*) parent)->m_Reader,SIGNAL(largePacketAvailable(int, int, QByteArray)),
            this, SLOT(packetAvailable(int,int,QByteArray)),
            Qt::QueuedConnection);
    connect(((CIpodController*) parent)->m_Reader,SIGNAL(packetAvailableForPlayerTaskHandler(int,int,QByteArray)),
            this, SLOT(packetAvailable(int,int,QByteArray)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(sendiAPPacket(QByteArray)) ,
            ((CIpodController*) parent)->m_Writer, SLOT(sendiAPPacket(QByteArray)),
            Qt::QueuedConnection);
    m_iPodDBIndexFetchTaskHandler = new CIPodDBIndexFetchTaskHandler(this,m_iPodPrivateData,
                                                                     ((CIpodController*)parent)->m_Writer,
                                                                     ((CIpodController*)parent)->m_Reader);

    connect(this,SIGNAL(CurrentUIDMediaInfoRetrieved()),SLOT(CurrentUIDMediaInfoRetrievedSlot()));   // Added by pawan 2013.05.04.
    connect(this,SIGNAL(CurrentPBMediaInfoRetrieved()),SLOT(CurrentPBMediaInfoRetrievedSlot()));

    setObjectName(gPlayEngineTaskHandler);

    //Added by Tan for coverity.
    m_EventNotifyState = eEventNotifyDefault;
    m_SentOneByteForm = false;
    m_PlaylistCount = 0;

    m_playState = ePlayState_STOP;

    m_playStateNotificationFromIpod = -1;

    m_CurrentIndexToPlay = -1;

    m_pAudioRouter = new GSTAudioRouterDBus("org.lge.GSTAudioRouterDBus", "/", QDBusConnection::sessionBus(), 0);
}

void CPlayEngineTaskHandler::handlePlaybackStopped()
{
    m_playStateNotificationFromIpod = ePlayState_STOP;
    m_HandleStopTimer->start();
}

void CPlayEngineTaskHandler::handlePlaybackStoppedSlot()
{
    //LOG_HIGH << "CPlayEngineTaskHandler::handlePlaybackStoppedSlot() IN" <<LOG_ENDL;
    //IPOD gives us multiple ePlayState_STOP notification,
    //so we should handle it only once.
    try
    {
        if(m_playStateNotificationFromIpod == ePlayState_STOP)
        {
            bool isNowPlayingMP = false;
            CiOSAppTaskHandler* piOSAppTaskHandler = ((CIpodController*)parent())->m_iOSAppTaskHandler;
            if(piOSAppTaskHandler)
            {
                isNowPlayingMP = piOSAppTaskHandler->isNowPlayingApplicationIsMPE();
            }
            else
            {
                LOG_HIGH << "handlePlaybackStoppedSlot(), but iOSAppTaskHandler not exist"<<LOG_ENDL;
                return;
            }
            //Get active application, if it is music, restart play.

            // Modified by jonghwan.cho@lge.com 2013.11.05 for sound issue while BT incoming call.
            //if(isNowPlayingMP)
            if( isNowPlayingMP && m_iPodPrivateData->m_IsBTCalling == false )
            {
                LOG_HIGH<<"handlePlaybackStoppedSlot(), Restarting play"<<LOG_ENDL;
                m_CurrentIndexToPlay = 0;
                m_iPodDBIndexFetchTaskHandler->playCommandCall(m_CurrentIndexToPlay);
                if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
                {
                    //iOS7 sometimes the PlayStatusChangeNotification trackIndex does not come.
                    delayMetadataIndexQuery();
                }
            }
            else
            {
                LOG_HIGH<<"handlePlaybackStoppedSlot(), active app is not MP"<<LOG_ENDL;
            }
        }
    }
    catch(EIPodErrors ex)
    {
        ////LOG_TRACE << "Disconnection correctly catch in handlePlaybackStopped Slot" << LOG_ENDL;
        return;
    }
    //LOG_HIGH<<"CPlayEngineTaskHandler::handlePlaybackStoppedSlot() STOPPED handlePlaybackStoppedSlot OUT" <<LOG_ENDL;
}

void CPlayEngineTaskHandler::InitializePlayEngineTaskHandler()
{
    //CR 13730: ATS warning fix
    if(m_iPodPrivateData->m_isIDPS)
    {
        m_playState = getCurrentPlayStateE();
    }

    setShuffleE(RANDOM_OFF);
    setRandomMode(RANDOM_OFF);
    emit OnModeChanged(RANDOMOFF);

    setRepeatE(REPEAT_FOLDER);
    setRepeatMode(REPEAT_FOLDER);
    emit OnModeChanged(REPEATALL);


    //Nano2G first time play problem
    m_PlayEngineTimer->setSingleShot(true);

    // ATS issue fix: for the bit setting error
    setStatusChangeNotificationsE(true);

    // Added by jonghwan.cho@lge.com    2013.04.19.
    setRemoteEventNotificationsE();
}

CPlayEngineTaskHandler::~CPlayEngineTaskHandler()
{
    m_PBTrackInfoQueue.clear();
    m_PBRequestInProgress.nRequestIndex = -1;
    m_PBRequestInProgress.nRequestType = RequestType_None;
    m_PBRequestInProgress.queryId = eDefault_Id;
    m_PBRequestInProgress.reqUID = 0;

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
    /*
    * Modified by jonghwan.cho@lge.com 	2013.03.27.
    */
    if(m_iPodDBIndexFetchTaskHandler)
    {
        delete m_iPodDBIndexFetchTaskHandler;
        m_iPodDBIndexFetchTaskHandler = NULL;
    }

    if(m_PlayEngineTimer)
    {
        if( m_PlayEngineTimer->isActive() == true )
        {
            m_PlayEngineTimer->stop();
        }
        delete m_PlayEngineTimer;
        m_PlayEngineTimer = NULL;
    }

    if(m_PlayIpodFilesTimer)
    {
        m_PlayIpodFilesTimer->stop();
        delete m_PlayIpodFilesTimer;
        m_PlayIpodFilesTimer = NULL;
    }

    if(m_HandleStopTimer)
    {
        m_HandleStopTimer->stop();
        delete m_HandleStopTimer;
        m_HandleStopTimer = NULL;
    }

    if(m_DelayMetadataIndexQueryTimer)
    {
        m_DelayMetadataIndexQueryTimer->stop();
        delete m_DelayMetadataIndexQueryTimer;
        m_DelayMetadataIndexQueryTimer = NULL;
    }

    if(m_DelayGetOnDemandMetaInfoTimer)
    {
        m_DelayGetOnDemandMetaInfoTimer->stop();
        delete m_DelayGetOnDemandMetaInfoTimer;
        m_DelayGetOnDemandMetaInfoTimer = NULL;
    }

    if(m_PBTrackInfoTimer)
    {
        m_PBTrackInfoTimer->stop();
        delete m_PBTrackInfoTimer;
        m_PBTrackInfoTimer = NULL;
    }

    clearPipeline();

    if(dBHandle.isValid())
    {
        if( dBHandle.isOpen() == true )
        {
            //LOG_TRACE << "DBHandleLog closing DB handle from play engine task handler" << LOG_ENDL;
            dBHandle.close();
        }
        //LOG_TRACE << "  DBHandleLog removing DB handle from playenginetaskhandler" << LOG_ENDL;
        QSqlDatabase::removeDatabase(dBHandle.connectionName());
    }
    // Added by jonghwan.cho@lge.com	2013.03.27.
    //LOG_TRACE << "CPlayEngineTaskHandler::~CPlayEngineTaskHandler Exit " << LOG_ENDL;
}

/* Stops the current task */
void CPlayEngineTaskHandler::stopTaskHandler()
{
    //TODO
    if(m_iPodDBIndexFetchTaskHandler)
    {
        m_iPodDBIndexFetchTaskHandler->stopTaskHandler();
    }
}

// Added by jonghwan.cho@lge.com    2013.04.16.
// for iOS 6.1
void CPlayEngineTaskHandler::setRemoteEventNotificationsE()
{
    //LOG_TRACE << "CPlayEngineTaskHandler::setRemoteEventNotifications() Entry" << LOG_ENDL;

    m_CommandStatus = false;
    m_EventNotifyState = eEventNotifySent;

    CSetRemoteEventNotification cmd( m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());

    //LOG_TRACE << "CPlayEngineTaskHandler::setRemoteEventNotifications() Exit" << LOG_ENDL;
}
// Added by jonghwan.cho@lge.com    2013.04.16.

void CPlayEngineTaskHandler::stopCurrentTask()
{
    m_isDisconnected = true;
    m_iPodDBIndexFetchTaskHandler->stopCurrentTask();   // added by sungha.choi 2013.04.02, mainLoopQuit()
    piPodLog->WriteLog("Here 1-1 m_pLoop->quit() before CPlayEngineTaskHandler");
    if(isWaitingForResponse() == true)
    {
        m_ReadyToDelete = false;
        m_pLoop->quit();
        piPodLog->WriteLog("Here 1-2 m_pLoop->quit() after CPlayEngineTaskHandler");
    }

    if(isWaitingForResponseFromSignal() > 0)  // added by sungha.choi 2013.04.26
    {
        m_ReadyToDeleteFromSignal = false;
        emit mainLoopQuit(ExitCode_AllExit);  // If signals will be added, emit the signal here.
    }

    stopPipe();
    //LOG_TRACE << "CPlayEngineTaskHandler::stopCurrentTask Exit" << LOG_ENDL;
}

void CPlayEngineTaskHandler::QuitPBLoopAndProcessNextRequest()
{
    //LOG_TRACE << "CPlayEngineTaskHandler::QuitPBLoopAndProcessNextRequest() IN" << LOG_ENDL;

    m_PBTrackInfoTimer->stop();

    switch(m_PBRequestInProgress.nRequestType)
    {
        case RequestType_MetaDataIndex:
            //LOG_TRACE << "   RequestType_MetaDataIndex data received for index = "
            //        << m_PBRequestInProgress.nRequestIndex << LOG_ENDL;
            //check the media duration
            if(m_CurrentMediaDuration == 0xFFFFFFFF && m_iPodPrivateData->m_IsCurrentPlayingMP && m_nMetadataRetryCount < 20)
            {
                m_nMetadataRetryCount++;
                LOG_HIGH << "QuitPBLoopAndProcessNextRequest(), m_CurrentMediaDuration retry"<< m_nMetadataRetryCount <<LOG_ENDL;
                delayMetadataIndexQuery(3000);
            }
            else
            {
                m_nMetadataRetryCount = 0;
                //LOG_TRACE << "   16bit OK m_CurrentMediaDuration = " <<m_CurrentMediaDuration<<LOG_ENDL;
                //LOG_TRACE << "   16bit OK m_IsCurrentPlayingMP   = " <<m_iPodPrivateData->m_IsCurrentPlayingMP<<LOG_ENDL;
                updateCurrentIndexTrackInfoAndUpdateHMI(m_PBRequestInProgress.nRequestIndex);
            }
            break;
        case RequestType_TuneDial:
            //LOG_TRACE << "   RequestType_TuneDial data received for index = "
            //        << m_PBRequestInProgress.nRequestIndex << LOG_ENDL;
            responseToTuneDialRequest(m_PBRequestInProgress.nRequestIndex, m_PBRequestInProgress.queryId,
                                      m_PBRequestInProgress.reqUID);

            break;
        default:
            LOG_HIGH << "QuitPBLoopAndProcessNextRequest(), Error! PBRequestIsSent without request type"<<LOG_ENDL;
            break;
    }
    m_PBRequestInProgress.nRequestType = RequestType_None;
    GetPBTrackInfoWithQueue(RequestType_None, -1 , eDefault_Id, 0);
}

void CPlayEngineTaskHandler::delayMetadataIndexQuery(int nDuration)
{
    m_DelayMetadataIndexQueryTimer->start(nDuration);
}

void CPlayEngineTaskHandler::delayMetadataIndexQuerySlot()
{
    try
    {
        int index = -1;
        getCurrentPlayingTrackIndex(index);
        LOGH " getCurrentPlayingTrackIndex = " << index << LOG_ENDL;
        if(index != -1)
        {
            m_CurrentIndex = index;
            GetPBTrackInfoWithQueue(RequestType_MetaDataIndex, index, eDefault_Id , 0);
        }
    }
    catch(EIPodErrors ex)
    {
        ////LOG_TRACE << "Disconnection correctly catch in handlePlaybackStopped Slot" << LOG_ENDL;
        return;
    }
}

void CPlayEngineTaskHandler::RemoveDuplicateQueueType(RequestType_T nRequestType)
{
    int i;
    int length = m_PBTrackInfoQueue.length();
    //delete everything of this type.
    for(i = length-1; i >= 0; i--)
    {
        if(m_PBTrackInfoQueue.at(i).nRequestType == nRequestType)
        {
            m_PBTrackInfoQueue.removeAt(i);
        }
    }
}

int CPlayEngineTaskHandler::GetLastQueueOfThisType(RequestType_T nRequestType)
{
    int i;
    int matchIndex = -1;
    int length = m_PBTrackInfoQueue.length();
    //delete everything of this type.
    for(i = length-1; i >= 0; i--)
    {
        if(m_PBTrackInfoQueue.at(i).nRequestType == nRequestType)
        {
            matchIndex = i;
            break;
        }
    }
    return matchIndex;
}

void CPlayEngineTaskHandler::RetryFailedPBTrackInfoRequest()
{
    PBTrackInfoRequest_T thisRequest;
    thisRequest = m_PBRequestInProgress;
    m_PBRequestInProgress.nRequestType = RequestType_None;
    m_TuneMetaDataArray.clear();
    GetPBTrackInfoWithQueue(thisRequest.nRequestType, thisRequest.nRequestIndex ,
                            thisRequest.queryId, thisRequest.reqUID, false);
}

void CPlayEngineTaskHandler::PBTrackInfoTimeOutSlot()
{
    LOGH " ERROR: PBTrackInfoTimeOutSlot request for PBTrackInfo has timed out for index = "
             << m_PBRequestInProgress.nRequestIndex << LOG_ENDL;
    RetryFailedPBTrackInfoRequest();
}

void CPlayEngineTaskHandler::GetPBTrackInfoWithQueue(RequestType_T nRequestType,
                                                     int index,
                                                     ETrackerAbstarctor_Query_Ids queryId,
                                                     uint reqUID, bool bAppend)
{
    //this function will send the GetPBTrackInfo to the device.
    //if GetPBTrackInfo is in progress, queue the request and send it later.
    PBTrackInfoRequest_T queueRequest;

    if(index >= 0)
    {
        //LOG_TRACE << "   Queing PBTrackInfo (" << nRequestType << " = " <<index << ")" <<LOG_ENDL;

        if(bAppend)
        {
            //we do not need to keep duplicate request.
            //We need only the latest one of the same type
            RemoveDuplicateQueueType(nRequestType);
            queueRequest.nRequestIndex = index;
            queueRequest.nRequestType = nRequestType;
            queueRequest.reqUID = reqUID;
            queueRequest.queryId = queryId;
            m_PBTrackInfoQueue.append(queueRequest);
        }
        else
        {
            //this is when a request failed.
            //if the same request type is in the queue, do not need to resend the request.
            int nLastQueueOfThisType = GetLastQueueOfThisType(nRequestType);
            if(nLastQueueOfThisType == -1)
            {
                queueRequest.nRequestIndex = index;
                queueRequest.nRequestType = nRequestType;
                queueRequest.reqUID = reqUID;
                queueRequest.queryId = queryId;
                m_PBTrackInfoQueue.prepend(queueRequest);
                LOGH " PBTrackInfo Prepending failed request (" << nRequestType <<" = " << index <<")" <<LOG_ENDL;
            }
            else
            {
                //ok, we have the same type of request. so even when the previous request failed,
                //do not worry, just ignore it and send the latest command in the queue.
                LOGH " PBTrackInfo Skip request for (" << nRequestType <<" = " <<index <<")" <<LOG_ENDL;
            }
        }
    }

    //do not request, unless we are sure that there is no request in progress.
    if(m_PBTrackInfoQueue.length() > 0 && (m_PBRequestInProgress.nRequestType == RequestType_None))
    {
        m_PBRequestInProgress = m_PBTrackInfoQueue.at(0);
        m_PBTrackInfoQueue.removeAt(0);

        //LOG_TRACE<< "   Processing PBTrackInfo (" << m_PBRequestInProgress.nRequestType << " = "
        //         <<m_PBRequestInProgress.nRequestIndex << ")" <<LOG_ENDL;
        CGetPBTrackInfo cmdPB(m_PBRequestInProgress.nRequestIndex, 1, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
        cmdPB.setMask(0xFF);
        cmdPB.marshall();
        emit sendiAPPacket(cmdPB.getCommandBytes());
        //we start a timer here, if for some reasons, the RetPBTrackInfo not come,
        //we should process the next data in Queue.
        m_PBTrackInfoTimer->start(5000);
    }
}

bool CPlayEngineTaskHandler::isReadyToDelete()  // added by sungha.choi 2013.04.12
{
    //LOG_TRACE << "CPlayEngineTaskHandler::isReadyToDelete() m_ReadyToDelete = " << m_ReadyToDelete << LOG_ENDL;
    //LOG_TRACE << "CPlayEngineTaskHandler::isReadyToDelete() m_ReadyToDeleteFromSignal = " << m_ReadyToDeleteFromSignal << LOG_ENDL;
    if(m_iPodDBIndexFetchTaskHandler->isReadyToDelete() == true)
        return (m_ReadyToDelete && m_ReadyToDeleteFromSignal);
    else
        return false;
}

bool CPlayEngineTaskHandler::isWaitingForResponse()    // added by sungha.choi 2013.04.12
{
    return m_isRunningLoop;
}

void CPlayEngineTaskHandler::delayGetOnDemandMetaInfoForIndexDevice()
{
    //LOG_TRACE << "CPlayEngineTaskHandler::delayGetOnDemandMetaInfoForIndexDevice()" << LOG_ENDL;
    m_DelayGetOnDemandMetaInfoTimer->start(1000);
    //this timer will be stopped, if the status notification come after select db record.
}

void CPlayEngineTaskHandler::delayGetOnDemandMetaInfoSlot()
{
    int index = -1;

    //LOG_TRACE << "CPlayEngineTaskHandler::delayGetOnDemandMetaInfoSlot()" << LOG_ENDL;

    try
    {
        getCurrentPlayingTrackIndex(index);
        LOGH "   getCurrentPlayingTrackIndex = " << index << LOG_ENDL;
        if(index!=-1)
        {
            m_CurrentIndex = index;
            getOnDemandMetaInfoForIndexDevice(index);
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE <<"CPlayEngineTaskHandler::delayGetOnDemandMetaInfoSlot Execption Caught ******" << LOG_ENDL;
    }
}



void CPlayEngineTaskHandler::getOnDemandMetaInfoForIndexDevice(int index, bool onlyUpdateToHMI)
{
    m_DelayGetOnDemandMetaInfoTimer->stop();
    //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::getOnDemandMetaInfoForIndexDevice: index: " << index << "  update to HMI: " << onlyUpdateToHMI << LOG_ENDL;
    if(!m_iPodPrivateData->m_IsDBIndexingPaused && m_IsSyncInProgress)
    {
        //LOG_TRACE << "PLAYLIST: DB indexing going on so cant fetch data" << LOG_ENDL;
        return;
    }
    CGetIndexedPlayingTrackTitle cmdTitle(index,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmdTitle.marshall();
    emit sendiAPPacket(cmdTitle.getCommandBytes());
    waitForResponceFromExitCode(2000, ExitCode_GetIndexedPlayingTrackTitle);

    CGetIndexedPlayingTrackArtistName cmdArtist(index,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmdArtist.marshall();
    emit sendiAPPacket(cmdArtist.getCommandBytes());
    waitForResponceFromExitCode(2000, ExitCode_GetIndexedPlayingTrackArtistName);

    CGetIndexedPlayingTrackAlbumName cmdAlbum(index,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmdAlbum.marshall();
    emit sendiAPPacket(cmdAlbum.getCommandBytes());
    waitForResponceFromExitCode(2000, ExitCode_GetIndexedPlayingTrackAlbumName);

    if(onlyUpdateToHMI)
    {
        int durationInfo = 1;
        m_nIPodReturnedMediaDuration = 0;
        CGetIndexedPlayingTrackInfo cmdTypeInfo(index,durationInfo, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
        cmdTypeInfo.marshall();
        emit sendiAPPacket(cmdTypeInfo.getCommandBytes());
        waitForResponceFromExitCode(2000, ExitCode_GetIndexedPlayingTrackDuration);
        m_CurrentMediaDuration = m_nIPodReturnedMediaDuration;
        emit OnDurationChanged(m_CurrentMediaDuration);

        QString songType;
        songType.append("Playlist");
        m_TuneMediaInfo.fileName.clear();
        if(songType.length() > 0)
        {
            createFilePath(songType, 0);
            m_TuneMediaInfo.fileName.append(m_FirstTimePlayFileName);
        }
        //LOG_TRACE << "PLAYLIST: new file name is: " <<  m_TuneMediaInfo.fileName << LOG_ENDL;

        m_iPodPrivateData->setCurrentMediaInfo(m_TuneMediaInfo);

        m_MediaInfo.Album = m_TuneMediaInfo.Album;
        m_MediaInfo.Title = m_TuneMediaInfo.Title;
        m_MediaInfo.Artist = m_TuneMediaInfo.Artist;
        m_MediaInfo.fileName = m_TuneMediaInfo.fileName;

        emit OnCurrentMediaInfo(&m_TuneMediaInfo);
        emit fetchCoversForBasicPlayer(m_MediaInfo.Album, m_MediaInfo.fileName, true);  // Modified by pawan 2013.05.16.

        m_CurrentIndexEmitted = m_CurrentIndex;
        m_iPodPrivateData->setCurrentMediaInfo(m_MediaInfo);
        m_NumberOfPlayingTracks = getNumberOfPlayingTracks();
        emit OnCurrentPlayingTrackCount(m_NumberOfPlayingTracks);
        LOGH " index/number = " << m_CurrentIndex +1 << "/" << m_NumberOfPlayingTracks << LOG_ENDL;

    }
}

bool CPlayEngineTaskHandler::isCurrentTrackHasChapter()
{
    return m_bCurrentTrackHasChapter;
}

void CPlayEngineTaskHandler::CurrentPBMediaInfoRetrievedSlot()
{
    //LOG_TRACE << "CPlayEngineTaskHandler::CurrentPBMediaInfoRetrievedSlot() Entry 0x0043" << LOG_ENDL;

    QString songType = QString();
    quint64 currentUID = 0;
    songType.clear();

    for(int i = 0; i <  NO_OF_META_INFO_PB_Track_Info;i++)
    {
        m_CommandBuffer.clear();
        //LOG_TRACE<<"CPlayEngineTaskHandler: CurrentPBMediaInfoRetrievedSlot got back 0x0043 " << m_TuneMetaDataArray.count() <<LOG_ENDL;
        if(m_TuneMetaDataArray.count() > i)
            m_CommandBuffer = m_TuneMetaDataArray[i];
        if(m_CommandBuffer.length() > 0)
        {
            CRetPBTrackInfo cmd(m_iPodPrivateData->m_isIDPS);
            cmd.unmarshall(m_CommandBuffer);

            //LOG_TRACE<<"CurrentPBMediaInfoRetrievedSlot cmd.getInfoType(): "<<cmd.getInfoType()<<LOG_ENDL;

            switch(cmd.getInfoType())
            {
            case 0:
            {
                if(m_PBRequestInProgress.nRequestType == RequestType_MetaDataIndex)
                {
                    m_bCurrentTrackHasChapter = cmd.hasChapter() ;
                    LOG_HIGH << "m_bCurrentTrackHasChapter : " << m_bCurrentTrackHasChapter<<LOG_ENDL;
                }
                break;
            }
            case 1:
            {
                m_TuneMediaInfo.Title = cmd.getInfoData();
                break;
            }
            case 2:
            {
                m_TuneMediaInfo.Artist = cmd.getInfoData();
                break;
            }
            case 3:
            {
                m_TuneMediaInfo.Album = cmd.getInfoData();
                break;
            }
            case 4:
            {
                m_TuneMediaInfo.Genre = cmd.getInfoData();
                break;
            }
            case 5:
            {
                m_TuneMediaInfo.Composer = cmd.getInfoData();
                break;
            }
            case 6:
            {
                if(m_PBRequestInProgress.nRequestType == RequestType_MetaDataIndex)
                {
                    //LOG_TRACE << "CPlayEngineTaskHandler::CurrentPBMediaInfoRetrievedSlot emitting duration: " << cmd.getTrackDuration() <<LOG_ENDL;
                    m_CurrentMediaDuration = cmd.getTrackDuration();
                    if(m_CurrentMediaDuration != 0xFFFFFFFF)
                    {
                        emit OnDurationChanged(m_CurrentMediaDuration);
                    }
                }
                break;
            }
            case 7:
            {
                currentUID = cmd.getUID();
                m_PBRetrievedUID = currentUID;
                break;
            }
            }
        }
    }

    songType.append(gIPODAllSongType);
    m_TuneMediaInfo.fileName.clear();
    if(songType.length() > 0 /*&& currentUID != 0*/)
    {
        createFilePath(songType, currentUID);
        m_TuneMediaInfo.fileName.append(m_FirstTimePlayFileName);
    }
    //LOG_TRACE << "new file name is: " <<  m_TuneMediaInfo.fileName << LOG_ENDL;
    m_TuneMetaDataArray.clear();

    QuitPBLoopAndProcessNextRequest();

    //LOG_TRACE << " 0x0043  CPlayEngineTaskHandler::CurrentPBMediaInfoRetrievedSlot() Exit" << LOG_ENDL;
}

// { Added by pawan 2013.05.04.
void CPlayEngineTaskHandler::CurrentUIDMediaInfoRetrievedSlot()
{
    //LOG_TRACE << "CPlayEngineTaskHandler::CurrentUIDMediaInfoRetrievedSlot() Entry 0x003F" << LOG_ENDL;

    QString songType = QString();
    quint64 currentUID = 0;
    songType.clear();

    for(int i = 0; i <  NO_OF_META_INFO_UID_Track_Info;i++)
    {
        m_CommandBuffer.clear();
        //LOG_TRACE<<"CPlayEngineTaskHandler: m_MetaDataArray got back 0x003F" << m_MetaDataArray.count() <<LOG_ENDL;
        if(m_MetaDataArray.count() > i)
            m_CommandBuffer = m_MetaDataArray[i];
        if(m_CommandBuffer.length() > 0)
        {
            CRetUIDTrackInfo cmd(m_iPodPrivateData->m_isIDPS);
            cmd.unmarshall(m_CommandBuffer);
            currentUID = cmd.getUID();
            switch(cmd.getInfoType())
            {
            case 1:
            {
                m_MediaInfo.Title = cmd.getInfoData();
                break;
            }
            case 2:
            {
                m_MediaInfo.Artist = cmd.getInfoData();
                break;
            }
            case 3:
            {
                m_MediaInfo.Album = cmd.getInfoData();
                break;
            }
            case 6:
            {
                m_CurrentMediaDuration = cmd.getTrackDuration();
                emit OnDurationChanged(m_CurrentMediaDuration);
                break;
            }
            }
        }
    }

    songType.append(gIPODAllSongType);
    m_FirstTimePlayFileName.clear();
    m_FirstTimePlayFileName = getFilePathFromDBUsingUID(currentUID);
    if(m_FirstTimePlayFileName.length() <= 0)
    {
        if(songType.length() > 0 && currentUID != 0)
        {
            createFilePath(songType, currentUID);
            m_MediaInfo.fileName = m_FirstTimePlayFileName;
            if(!m_MediaInfo.fileName.contains("file://"))
                m_MediaInfo.fileName.prepend("file://");   // Added by pawan 2013.05.16.
        }
    }
    else
    {
        m_MediaInfo.fileName = m_FirstTimePlayFileName;
    }

    //LOG_INFO << " 0x003F CPlayEngineTaskHandler::getCurrentMediaInfo: Title -"
    //         << m_MediaInfo.Title.toAscii().data()
    //         << ", Album- " << m_MediaInfo.Album.toAscii().data()
      //       << ", Artist - " << m_MediaInfo.Artist.toAscii().data()
        //           <<", UID got from ipod for meta info: " << currentUID
          //      << "Duration - " << m_CurrentMediaDuration
            // << ", file name - " << m_MediaInfo.fileName.toAscii().data() << LOG_ENDL;

    m_iPodPrivateData->setCurrentMediaInfo(m_MediaInfo);
    emit OnCurrentMediaInfo(&m_MediaInfo);
    emit fetchCoversForBasicPlayer(m_MediaInfo.Album, m_MediaInfo.fileName);  // Modified by pawan 2013.05.16.

    m_CurrentIndexEmitted = m_CurrentIndex;
    m_MetaDataArray.clear();
    m_iPodPrivateData->setCurrentMediaInfo(m_MediaInfo);
    //LOG_TRACE << " CPlayEngineTaskHandler::CurrentUIDMediaInfoRetrievedSlot() Exit" << LOG_ENDL;
}

QString CPlayEngineTaskHandler::getFilePathFromDBUsingUID(quint64 uid)
{
        QString filePath = QString();
        createConnection(m_iPodPrivateData->getCurrentSerialNumber());
        QSqlQuery query(dBHandle);
        QString queryString("select T_FilePath from MasterTable where T_UID=");
        quint64 uidval = uid;
        QString uidstring;
        queryString.append("'");
        uidstring.setNum(uidval);
        queryString.append(uidstring);
        queryString.append("'");
        //LOG_TRACE << "devplay query string:" << queryString << LOG_ENDL;
        bool ret = query.exec(queryString);
        //LOG_TRACE << "devplay got the UID change notification:" << ret << LOG_ENDL;
        if(query.next())
        {
            filePath = query.value(0).toString();
            //LOG_TRACE << "devplay filepath is:" << filePath << LOG_ENDL;
        }
        return filePath;
}

void CPlayEngineTaskHandler::createFilePath(const QString& songType, const quint64 uid)
{
    m_FirstTimePlayFileName.clear();
    QString filePath = QString();

    filePath.clear();
    filePath.append(fVolumePath_IPod);
    filePath.append(fForwardSlash);
    filePath.append(m_iPodPrivateData->getCurrentSerialNumber());
    filePath.append(fForwardSlash);
    filePath.append(songType);
    filePath.append(fForwardSlash);
    if(uid <=0)
    {
        QString INDEX = QString();
        INDEX.clear();
        INDEX.setNum(m_CurrentIndex);
        filePath.append(INDEX);
    }
    else
    {
        filePath.append("-1");
    }
    filePath.append(fUnderscoreChar);

    QString UID = QString();
    UID.clear();
    UID.setNum(uid);
    filePath.append(UID);
    if(songType.compare(gIPODAllSongType))
    {
        filePath.append(fUnderscoreChar);
        filePath.append("-1");
    }

    // Set the file name, to return to AVP, in case of first time play
    m_FirstTimePlayFileName = filePath;

}
// } Added by pawan 2013.05.04.

int CPlayEngineTaskHandler::isWaitingForResponseFromSignal()    // added by sungha.choi 2013.04.26
{
    return m_nRunningLoopFromSignal;
}

int CPlayEngineTaskHandler::waitForResponceFromExitCode(int timeOut, int nExitCode)
{
    ////LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode Entry " << LOG_ENDL;
    int ret = 0;

    if(m_isDisconnected){
        LOG_HIGH << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode: iPod Already disconnect" << LOG_ENDL;
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
    if((ret == IPOD_ERROR_DISCONNECT) || m_isDisconnected){
        //LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode: Loop exit with Error on disconnect" << LOG_ENDL;
        throw  IPOD_ERROR_DISCONNECT;
    }
    ////LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode Exit " << LOG_ENDL;
    return ret;//Return IPOD_SUCCESS or IPOD_ERROR_TIMEOUT
}

void CPlayEngineTaskHandler::authorisationFinished(bool status)
{
    //LOG_INFO << "CPlayEngineTaskHandler::authorisationFinished: Loop status=" << status << LOG_ENDL;
    m_AuthenticationPassed = status;
}

bool CPlayEngineTaskHandler::isAudioBookPodcast(quint64 uidval)
{
    bool bIsAudioBookPodcast;
    //LOG_TRACE<<"isAudioBookPodcast uidval"<<uidval<<LOG_ENDL;
    QString fileName;

    if(uidval <= 0)
    {
        fileName.append(getCurrentFileName());
    }
    else
    {
        fileName.append(getFilePathFromDBUsingUID(uidval));
    }

    if(fileName.contains(gIPODPodcastType) || fileName.contains(gIPODAudioBookType) || fileName.contains(gIPODiTunesUType))
    {
        LOG_HIGH << "isAudioBookPodcast TRUE filename is "<< fileName <<LOG_ENDL;
        bIsAudioBookPodcast = true;
    }
    else
    {
        LOG_HIGH << "isAudioBookPodcast FALSE filename is "<< fileName <<LOG_ENDL;
        bIsAudioBookPodcast = false;
    }
    return bIsAudioBookPodcast;
}

bool CPlayEngineTaskHandler::PlayiPodFiles(int nTrackIndex)
{
    LOG_HIGH<<"PlayiPodFiles nTrackIndex = "<< nTrackIndex << LOG_ENDL;
    if(m_iPodDBIndexFetchTaskHandler)
    {
        m_CurrentIndexToPlay = nTrackIndex;
        m_iPodDBIndexFetchTaskHandler->resetPreviousSelectionsE();
        m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE("Track");
        m_iPodDBIndexFetchTaskHandler->playCommandCall(m_CurrentIndexToPlay);
        startPlayIpodFilesTimer();
        return true;
    }
    return false;
}

void CPlayEngineTaskHandler::startPlayIpodFilesTimer()
{
    LOG_HIGH << "startPlayIpodFilesTimer" << LOG_ENDL;
    if(m_PlayIpodFilesTimer)
    {
        m_PlayIpodFilesTimer->start();
    }
}

void CPlayEngineTaskHandler::stopPlayIpodFilesTimer()
{
    LOG_HIGH << "stopPlayIpodFilesTimer" << LOG_ENDL;
    if(m_PlayIpodFilesTimer)
    {
        m_PlayIpodFilesTimer->stop();
    }
}

void CPlayEngineTaskHandler::handlePlayIpodFilesSlot()
{
    try
    {
        CiOSAppTaskHandler* piOSAppTaskHandler = ((CIpodController*)parent())->m_iOSAppTaskHandler;
        if(piOSAppTaskHandler)
        {
            bool isNowPlayingMP = piOSAppTaskHandler->isNowPlayingApplicationIsMPE();
            if(isNowPlayingMP)
            {
                //it is already playing com.apple (I do not check music intentionally).
                //because user may have launched podcast in between.
                LOG_HIGH<<"handlePlayIpodFilesSlot DO NOTHING" <<LOG_ENDL;
            }
            else
            {
                if(m_CurrentIndexToPlay < 3)
                {
                    LOG_HIGH<<"handlePlayIpodFilesSlot"<<LOG_ENDL;
                    PlayiPodFiles(m_CurrentIndexToPlay+1);
                }
            }
        }
    }
    catch(EIPodErrors ex)
    {
        return;
    }
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::play                           void (OUT), void (IN)
//
// Explanation
// This method handles playing a track.
// Plays the first track in the Song list if no track has been selected.Else plays the paused track.
//
//
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
bool CPlayEngineTaskHandler::playE()
{
    int nIndexDevicePreviousIndex = m_CurrentIndex;
    LOGH ", m_FirstTimePlay = " << m_FirstTimePlay << "and m_CurrentIndexToPlay = " << m_CurrentIndexToPlay << ", m_CurrentIndex = " << m_CurrentIndex << " and m_AutoplayIndex = " << m_AutoplayIndex << LOG_ENDL;
    if(!m_StateNotificationSet)
        setStatusChangeNotificationsE();
    if(m_CurrentIndexToPlay <= -1)
    {
        LOG_HIGH << " AUTOPLAY: play returnign as the index is: " << m_CurrentIndexToPlay << LOG_ENDL;
        return false;
    }
    else
    {
        //LOG_TRACE<<"playE Setting m_CurrentIndex to "<< m_CurrentIndexToPlay <<LOG_ENDL;
        m_CurrentIndex = m_CurrentIndexToPlay;
        //LOG_TRACE << "CPlayEngineTaskHandler::play() when m_CurrentIndex > =0" << LOG_ENDL;
        //CR 14737: Multiple connects metadata missing S
        m_FirstTimePlay =  false;
        // Added by jonghwan.cho@lge.com 2013.05.17.
        if( m_ScanMode == SCAN_OFF && m_RepeatMode != REPEAT_FILE)  // Modified by pawan 2013.05.28.
            setRepeatE(REPEAT_FOLDER);    // Added by pawan 2013.05.09.
        //CR 14737: Multiple connects metadata missing E
        m_iPodPrivateData->m_SLeepForPlayWhileFetching = 200000;  // Added by pawan 2013.05.03.

        if(m_IsPlayListSpecificCategoryIsSet)
        {
            m_ControllerPlaylist.clear();
            playCategoryTrack(m_CategoryType);
            //For some indexed device, we have to update
            //the track index as the notification will not come!
            if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex == false)
            {
                delayGetOnDemandMetaInfoForIndexDevice();
            }
            else
            {
				//iOS7 sometimes the PlayStatusChangeNotification trackIndex does not come.
                delayMetadataIndexQuery();
            }
        }
        else if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
        {
            m_bCoverArtUpdated = false;
            if(m_PreparePlaylistStatus)
            {
                m_PreparePlaylistStatus = false;
                //LOG_TRACE << "CPlayEngineTaskHandler::play setUIDPlayListE playing using playprepareuidlist"<< LOG_ENDL;
                if(m_ControllerPlayListUID.length() > m_CurrentIndexToPlay)
                {
                    QDataStream ds(m_ControllerPlayListUID[m_CurrentIndexToPlay]);
                    quint64 UIDToBePlayed;
                    ds >> UIDToBePlayed;

                    //LOG_TRACE<<"UIDToBePlayed:m_CurrentPlayingUID = "<<UIDToBePlayed<<":"<<m_CurrentPlayingUID<<LOG_ENDL;
                    if(((UIDToBePlayed == m_CurrentPlayingUID)||m_CurrentPlayingUID == 0)  && isAudioBookPodcast(UIDToBePlayed) == false)
                    {
                        setPosition(0);
                    }
                    CPlayPreparedUIDList cmd(m_iPodPrivateData->getTransactionId(),m_ControllerPlayListUID[m_CurrentIndexToPlay], m_iPodPrivateData->m_isIDPS);
                    cmd.marshall();
                    emit sendiAPPacket(cmd.getCommandBytes());
                    waitForResponceFromExitCode(2000, ExitCode_PlayPreparedUIDList);
                    m_CurrentPlayingUID = UIDToBePlayed;
                    //iOS7 sometimes the PlayStatusChangeNotification trackIndex does not come.
                    delayMetadataIndexQuery();
                }
                else
                {
                    //LOG_HIGH<<"Could have crashed, but averted!"<<LOG_ENDL;
                }
            }
            else if(m_IsPlaylistSetByDH)
            {
                m_CurrentPlayingUID = 0;
                //LOG_TRACE << "CPlayEngineTaskHandler::play setUIDPlayListE playing using selectDBRecord"<< LOG_ENDL;
                m_IsPlaylistSetByDH = false;
                if(m_bDBSelectionHasChanged)//if category has been changed.
                {
                    m_iPodDBIndexFetchTaskHandler->resetPreviousSelectionsE();
                }
                m_bDBSelectionHasChanged = false;
                usleep(100000);
                m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE("Track");
                usleep(100000);
                m_CommandStatus = m_iPodDBIndexFetchTaskHandler->playCommandCall(m_CurrentIndexToPlay);
                //iOS7 sometimes the PlayStatusChangeNotification trackIndex does not come.
                delayMetadataIndexQuery();
            }
            else
            {
                //LOG_TRACE << "CPlayEngineTaskHandler::play setUIDPlayListE playing using current selection"<< LOG_ENDL;
                m_CommandStatus = false;

                CSetCurrentPlayingTrack cmd(m_CurrentIndexToPlay, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
                cmd.marshall();
                emit sendiAPPacket(cmd.getCommandBytes());
                waitForResponceFromExitCode(7000, ExitCode_SetCurrentPlayingTrack);
                m_playState = ePlayState_PLAY;
                OnStateChanged(MPLAYING);
                //iOS7 sometimes the PlayStatusChangeNotification trackIndex does not come.
                delayMetadataIndexQuery();
            }
        }
        else
        {
            //this is index device only.!
            if(m_IsPlaylistSetByDH)
            {
                //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::play playing usign select db record selection"<< LOG_ENDL;
                m_IsPlaylistSetByDH = false;
                m_iPodDBIndexFetchTaskHandler->resetPreviousSelectionsE();
                int totalCount = m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE("Track");
                //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::play total count after changing cat from playlist to tracks: " << totalCount << LOG_ENDL;
                m_CurrentIndexEmitted = -1;
                m_CommandStatus = m_iPodDBIndexFetchTaskHandler->playCommandCall(m_CurrentIndexToPlay);
                emit OnChangeDBIndexingStatus(false);
            }
            else
            {
                //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::play playing usign current selection: "<< m_CurrentIndexToPlay <<LOG_ENDL;

                if((nIndexDevicePreviousIndex == m_CurrentIndex)&&isAudioBookPodcast()==false)
                {
                    setPosition(0);
                }
                m_CommandStatus = false;
                CSetCurrentPlayingTrack cmd(m_CurrentIndexToPlay, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
                cmd.marshall();
                emit sendiAPPacket(cmd.getCommandBytes());
                waitForResponceFromExitCode(7000, ExitCode_SetCurrentPlayingTrack);
                m_playState = ePlayState_PLAY;
                OnStateChanged(MPLAYING);
            }
        }

        m_iPodPrivateData->m_SLeepForPlayWhileFetching = 0;  // Added by pawan 2013.05.03.

        bool flag = m_CommandStatus;
        //LOG_TRACE << "CPlayEngineTaskHandler::play() flag is " << flag<< LOG_ENDL;
    }
    if(m_CommandStatus == true)
        ++m_RandomPlayCount;

    //LOG_INFO << "CPlayEngineTaskHandler::play Track Index going to be played = "<< m_CurrentIndexToPlay << LOG_ENDL;
    //LOG_INFO << "CPlayEngineTaskHandler::play emitCurrentMediaInfo in CPlayEngineTaskHandler::play()"<< m_CurrentIndexToPlay << LOG_ENDL;

    if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex == false)
    {
        //LOG_TRACE <<"playE emitCurrentMediaInfoE"<<LOG_ENDL;
        MediaInfo currentMediaInfo;
        emitCurrentMediaInfoE(&currentMediaInfo);
    }

    isNotPassOnToNextSong = true;
    m_JumpInProgress = false;
    //LOG_INFO << "CPlayEngineTaskHandler::play() m_CommandStatus is  "<< m_CommandStatus << LOG_ENDL;
    return m_CommandStatus;
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::playControl                           void (OUT), QString (IN) - the control action
//
// Explanation
// This method performs actions on a track that is playing.
// Function does nothing if there is no currently playing track.
//
//
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
bool CPlayEngineTaskHandler::playControlE(QString action, bool nano1gpodcastFlag)
{
    //LOG_TRACE << "CPlayEngineTaskHandler::playControl() Entry  action is = " << action <<"current state : " << m_playState << LOG_ENDL;
    m_playControlStopSeekFlag = false; 	// Added to fix FF End functionality for classic devices

    if(m_ScanMode == SCAN_FILE)
    {
        // called only when SCAN is ON
        scanEmition(action);
    }

    if(!m_StateNotificationSet)
        setStatusChangeNotificationsE();
    // Added to fix FF End functionality for classic device
    if(!action.compare("StopSeek",Qt::CaseInsensitive))
    {
        m_playControlStopSeekFlag = true;
        //m_iPodPrivateData->SendAudioRouterFadeOut();
    }

    m_CommandStatus = false;
    m_CurrentCommand = 0;
    //set a timer for 25seconds. If no response was recv in 25secs the eventloop shud be woken up.else app will hang.

    bool isNano1Gor2G = (m_iPodPrivateData->m_iPodProductID == NANO_1G_PRODUCT_ID) || (m_iPodPrivateData->m_iPodProductID == NANO_2G_PRODUCT_ID);
    bool useDisplayRemote = (m_iPodPrivateData->m_DisplayRemoteLingoVersion >= 1.02);
    if(!action.compare("Play", Qt::CaseInsensitive) && !isNano1Gor2G && useDisplayRemote)
    {
        //LOG_TRACE << "CPlayEngineTaskHandler::playControl() play using display remote play" << LOG_ENDL;
        CSetiPodStateInfo setPosCmd(eSetDisplayRemotePlay, 0, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS ); //TODO place the rite trans id
        setPosCmd.marshall();
        emit sendiAPPacket(setPosCmd.getCommandBytes());
        waitForResponceFromExitCode(2000, ExitCode_SetiPodStateInfo);
    }
    else if(!action.compare("Pause", Qt::CaseInsensitive) && !isNano1Gor2G && useDisplayRemote)
    {
        //LOG_TRACE << "CPlayEngineTaskHandler::playControl() pause using display remote play" << LOG_ENDL;
        CSetiPodStateInfo setPosCmd(eSetDisplayRemotePause, 0, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS ); //TODO place the rite trans id
        setPosCmd.marshall();
        emit sendiAPPacket(setPosCmd.getCommandBytes());
        waitForResponceFromExitCode(2000, ExitCode_SetiPodStateInfo);
    }
    else
    {
        //LOG_TRACE << "CPlayEngineTaskHandler::playControl() command using extended play" << LOG_ENDL;
        CPlayControl playCmd(action, m_iPodPrivateData->getTransactionId(),  m_iPodPrivateData->m_isIDPS,nano1gpodcastFlag ); //TODO place the rite trans id
        playCmd.marshall();
        emit sendiAPPacket(playCmd.getCommandBytes());
        waitForResponceFromExitCode(7000, ExitCode_PlayControl);
    }

    if( !action.compare("Stop",Qt::CaseInsensitive) )
    {
        m_NumberOfPlayingTracksReceivedFromIPod = 0;
        //LOG_TRACE << "Action is stop" << LOG_ENDL;
        m_StateNotificationSet = false;
    }
    else if( !action.compare("FastForward", Qt::CaseInsensitive))
    {
        m_PlayNextSongOnFF = true;
    }
    else if(m_CommandStatus && !action.compare("Play", Qt::CaseInsensitive))
    {
        m_playState = ePlayState_PLAY;
    }
    else if(m_CommandStatus && !action.compare("Pause", Qt::CaseInsensitive))
    {
        m_playState = ePlayState_PAUSE;
    }
    //LOG_TRACE << "CPlayEngineTaskHandler::playControl() Exit "<< LOG_ENDL;
    return m_CommandStatus;
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::playControl                           void (OUT), QString (IN) - the control action
//
// Explanation
// This method performs actions on a track that is playing.
// Function does nothing if there is no currently playing track.
// Actions may be Play,Pause,FastForward,Rewind
//
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
bool CPlayEngineTaskHandler::setRepeatE(int mode)
{
    //LOG_TRACE << "setrepeatE : " << mode << LOG_ENDL;
    if(m_ScanMode == SCAN_FILE)
    {
        //LOG_INFO<< "CPlayEngineTaskHandler::setShuffle  scan ON " << mode << LOG_ENDL;
        m_ScanMode = SCAN_OFF;
        emit OnScanCompleted();
    }
    m_CommandStatus = false;

    // Added by jonghwan.cho@lge.com 2013.10.30.
    if( mode == REPEAT_FILE && m_RandomMode != RANDOM_OFF )
        setRandomMode(RANDOM_OFF);

    m_RepeatMode = mode;  // Added by pawan 2013.05.28.
    CSetRepeat repeatCmd(mode,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS ); //TODO place the rite trans id
    repeatCmd.marshall();
    emit sendiAPPacket(repeatCmd.getCommandBytes());
    waitForResponceFromExitCode(7000, ExitCode_SetRepeat);
    return m_CommandStatus;
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::setShuffle                           void (OUT), int (IN) - shuffle mode to set
//
// Explanation
// This method sets the shuffle mode for the apple device
// Modes may be
// REPEATOFF           0   //Repeat is off
// REPEATFILE          1   //Repeat single file
// REPEATFOLDER        2   //Repeat folder
//
//
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
bool CPlayEngineTaskHandler::setShuffleE(int mode)
{
    //LOG_TRACE << "CPlayEngineTaskHandler::setShuffle() Entry" <<LOG_ENDL;
    if(m_ScanMode == SCAN_FILE)
    {
        //LOG_INFO<< "CPlayEngineTaskHandler::setShuffle  scan ON " << mode << LOG_ENDL;
        m_ScanMode = SCAN_OFF;
        emit OnScanCompleted();
    }
    m_CommandStatus = false;

    if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
    {
        delayMetadataIndexQuery();
    }
    CSetShuffle shuffleCmd(mode,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS ); //TODO place the rite trans id
    shuffleCmd.marshall();
    emit sendiAPPacket(shuffleCmd.getCommandBytes());
    waitForResponceFromExitCode(7000, ExitCode_SetShuffle);

    //Get the curretn playing index as sometime
    //IPod does not send the current playing index
    //when shuffle state changes
    if(!isPlayListSet() && (mode != m_RandomMode))
    {
        if( mode == RANDOM_OFF)
        {
            m_RandomMode = RANDOM_OFF;
            //LOG_TRACE << "CCPlayEngineTaskHandler::setShuffleE RANDOM FILE OFF" << LOG_ENDL;
        }
        else if(( mode == RANDOM_FILE) || (mode == RANDOM_FOLDER) )
        {
            //LOG_TRACE << "CPlayEngineTaskHandler::setShuffleE RANDOM FILE SE" << LOG_ENDL;
            m_RandomMode = RANDOM_FILE;
        }
        else
        {
            //LOG_TRACE << "CPlayEngineTaskHandler::setShuffleE RANDOM FILE OFF" << LOG_ENDL;
        }
    }
    else
    {
        //LOG_TRACE << "devplay setshuffleE for shuffle and playlist is set" << LOG_ENDL;
    }
    //LOG_TRACE << "CPlayEngineTaskHandler::setShuffle() EXIT" <<LOG_ENDL;
    return m_CommandStatus;
}
void CPlayEngineTaskHandler::scanPlaylist()
{
    //LOG_TRACE << "CPlayEngineTaskHandler::scanPlaylist() Entry" <<LOG_ENDL;
    m_scanFlag = false;
    //{added by junam 2013.04.13 for AutoStart
    if(m_ControllerPlaylist.isEmpty())
    {
        if(m_ScanMode == SCAN_FILE)
        {
            if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
            {
                if(m_ScanPlayCount < m_NumberOfPlayingTracksReceivedFromIPod - 1)
                {
                    m_ScanPlayCount++;
                    playNext();
                }
                else
                {
                    m_ScanPlayCount = 0;
                    m_ScanMode = SCAN_OFF;
                    emit OnScanCompleted();
                    //*play the next song.
                    if(m_NumberOfPlayingTracksReceivedFromIPod == 1)
                    {
                        setPosition(0);
                    }
                    else
                    {
                        playNext();
                    }
                }
                return;
            }
            //LOG_INFO << "scanPlaylist m_ScanPlayCount = " << m_ScanPlayCount << " m_NumberOfPlayingTracks =  " <<m_NumberOfPlayingTracks <<LOG_ENDL;
            //CR 15650: Scan should start play the song from which scan has started
            if((m_ScanPlayCount != (m_NumberOfPlayingTracks-1)) &&  ((m_NumberOfPlayingTracks-1)>0))
            {
                //LOG_TRACE << "scanPlaylist if entered" <<LOG_ENDL;
                //LOG_INFO << "m_CurrentIndex = " <<m_CurrentIndex <<LOG_ENDL;
                if ( m_CurrentIndex +1 > ( m_NumberOfPlayingTracks-1))
                    setCurrentIndexE(0);
                else
                    setCurrentIndexE(m_CurrentIndex+1);
                m_ScanPlayCount++;

                if(m_pauseScanTimeFlag)
                    m_scanFlag = true;
                playE();
            }
            else //complete scan is done. Stop the scan and start playing the first song normally
            {
                //LOG_TRACE << "scanPlaylist else entered" <<LOG_ENDL;
                emit OnScanCompleted();
                m_ScanPlayCount = 0;
                m_ScanMode = SCAN_OFF;
                if ( m_CurrentIndex+1 > ( m_NumberOfPlayingTracks-1))
                    setCurrentIndexE(0);
                else
                    setCurrentIndexE(m_CurrentIndex + 1);
                playE();
            }
        }
    }
    else //}added by junam 2013.04.13 for AutoStart
    {
        if(m_ScanMode == SCAN_FILE)
        {
            //LOG_INFO << "scanPlaylist m_ScanPlayCount = " << m_ScanPlayCount << " m_ControllerPlaylist.count() =  " <<m_ControllerPlaylist.count() <<LOG_ENDL;
            //CR 15650: Scan should start play the song from which scan has started
            if((m_ScanPlayCount < (m_ControllerPlaylist.count()-1)) &&  ((m_ControllerPlaylist.count()-1)>0))
            {
                //LOG_TRACE << "scanPlaylist if entered" <<LOG_ENDL;
                //LOG_INFO << "m_CurrentIndex = " <<m_CurrentPlaylistIndex <<LOG_ENDL;
                ++m_CurrentPlaylistIndex;
                if ( m_CurrentPlaylistIndex > ( m_ControllerPlaylist.count()-1))
                {
                    m_CurrentPlaylistIndex =0;
                }
                m_ScanPlayCount++;
                setCurrentIndexE(m_CurrentPlaylistIndex);
                if(m_pauseScanTimeFlag)
                {
                    m_scanFlag = true;
                }
                playE();
            }
            else //complete scan is done. Stop the scan and start playing the first song normally
            {
                //LOG_TRACE << "scanPlaylist else entered" <<LOG_ENDL;
                emit OnScanCompleted();
                m_ScanPlayCount = 0;
                m_ScanMode = SCAN_OFF;
                if(m_ControllerPlaylist.count()==1)
                {
                    setPosition(0);
                }
                else
                {
                    //CR 15650: Scan should start play the song from which scan has started S
                    ++m_CurrentPlaylistIndex;
                    if ( m_CurrentPlaylistIndex > ( m_ControllerPlaylist.count()-1))
                    {
                        m_CurrentPlaylistIndex =0;
                    }
                    setCurrentIndexE(m_CurrentPlaylistIndex);
                    playE();
                }
            }
        }
    }
    //LOG_TRACE << "CPlayEngineTaskHandler::scanPlaylist() Exit" <<LOG_ENDL;
}
void CPlayEngineTaskHandler::scanEmition(QString action)
{
    //LOG_INFO<< "CPlayEngineTaskHandler::scanEmition Entered " << action << LOG_ENDL;
    if(!action.compare("Pause",Qt::CaseInsensitive))
    {
        //LOG_INFO<< "CPlayEngineTaskHandler::playControl Pause action = " << action << LOG_ENDL;
        m_scanFlag = true;
        m_pauseScanTimeFlag = true;
        m_scanElapseTime = m_scanPauseTimer.elapsed();
        //LOG_INFO << "CPlayEngineTaskHandler elapseTime = " << m_scanElapseTime << LOG_ENDL;
    }
    else  if(!action.compare("Play",Qt::CaseInsensitive))
    {
        //LOG_INFO<< "CPlayEngineTaskHandler::playControl Play action = " << action << LOG_ENDL;
        int time = SCAN_DURATION;
        if(m_pauseScanTimeFlag)
        {
            time = time- m_scanElapseTime;
            //LOG_INFO << "CPlayEngineTaskHandler time = " << time << LOG_ENDL;
        }
        m_scanElapseTime = 0;
        if(m_scanPauseTimer.isNull())
        {
            m_scanPauseTimer.start();
            //LOG_INFO << "CPlayEngineTaskHandler::PlayControl() m_scanPauseTimer START = " <<m_scanPauseTimer.toString() << LOG_ENDL;
        }
        else
        {
            m_scanPauseTimer.restart();
            //LOG_INFO << "CPlayEngineTaskHandler::PlayControl() m_scanPauseTimer RESTART = " <<m_scanPauseTimer.toString() << LOG_ENDL;
        }

    }
    else
    {
        // functionality for previous,Next,FF,Rewind and stop
        //LOG_INFO<< "CPlayEngineTaskHandler::playControl action " << action << LOG_ENDL;
        m_ScanMode = SCAN_OFF;
        emit OnScanCompleted();

    }
    //LOG_TRACE<< "CPlayEngineTaskHandler::scanEmition Exit " << LOG_ENDL;
}

int CPlayEngineTaskHandler::GetScanMode()
{
    //LOG_TRACE << "CPlayEngineTaskHandler::GetScanMode Entry" << LOG_ENDL;
    return m_ScanMode;
}

bool CPlayEngineTaskHandler::setScanModeE(int mode)
{
    //LOG_INFO << "CPlayEngineTaskHandler::setScanMode Entry mode = " << mode << "playlist count: " << m_ControllerPlaylist.count() << "and from ipod playlist count: " << m_NumberOfPlayingTracksReceivedFromIPod <<LOG_ENDL;
    bool res = true;
    switch(mode)
    {
    case SCAN_OFF :
    {
        m_ScanMode = SCAN_OFF;
        m_ScanPlayCount = 0;
        //Mainly for previous and Next functionality.
        //when scan is on and next/prev is pressed=> scan should be off and timer should be stopped
        //LOG_TRACE << "CCPlayEngineTaskHandler::SCAN FILE OFF" << LOG_ENDL;
        break;
    }
    case SCAN_FILE :
    case SCAN_FOLDER:
    {
        //LOG_TRACE << "CPlayEngineTaskHandler::SCAN FILE SET" << LOG_ENDL;

        m_ScanMode = SCAN_FILE;
        m_scanFlag = true;
        m_ScanPlayCount = 1;
        //LOG_INFO << " m_CurrentPlaylistIndex1 = " <<m_CurrentPlaylistIndex <<LOG_ENDL;
        //Scan should start from the next song
        //{added by junam 2013.04.13 for AutoStart
        if(m_ControllerPlaylist.isEmpty())
        {            
            if(m_NumberOfPlayingTracksReceivedFromIPod == 1)
            {
                setPosition(0);
                return true;
            }
            else if(playNext() == 0)
                return true;
            else
                return false;
        }
        else//}added by junam
        {
            if(m_ControllerPlaylist.count() <= 0)
            {
                m_ScanPlayCount = 0;
                m_ScanMode = SCAN_OFF;
                emit OnScanCompleted();
                return false;
            }
            if ( m_CurrentPlaylistIndex == ( m_ControllerPlaylist.count()-1))
            {
                m_CurrentPlaylistIndex =0;
            }
            else
            {
                m_CurrentPlaylistIndex = m_CurrentPlaylistIndex + 1;
            }
            //LOG_INFO << "m_CurrentPlaylistIndex2 = " <<m_CurrentPlaylistIndex <<LOG_ENDL;
            setCurrentIndexE(m_CurrentPlaylistIndex);
            playE();
        }
        break;
    }
    default:
    {
        //LOG_TRACE << "CPlayEngineTaskHandler::SCAN FILE OFF" << LOG_ENDL;
        res = false;
    }
    }
    //LOG_TRACE << "CPlayEngineTaskHandler::setScanMode Exit m_ScanMode =  " << m_ScanMode << " m_ScanPlayCount = " << m_ScanPlayCount<<LOG_ENDL;
    return res;
}

bool CPlayEngineTaskHandler::setRandomMode(int mode)
{
    bool res = true;
    if(m_ScanMode == SCAN_FILE)
    {
        //LOG_INFO<< "CPlayEngineTaskHandler::setRandomMode  scan ON " << mode << LOG_ENDL;
        m_ScanMode = SCAN_OFF;
        emit OnScanCompleted();
    }
    if( mode == RANDOM_OFF)
    {
        m_RandomMode = RANDOM_OFF;
        m_RandomPlayCount = 0;
        //LOG_TRACE << "CCPlayEngineTaskHandler::RANDOM FILE OFF" << LOG_ENDL;
    }
    else if(( mode == RANDOM_FILE) || (mode == RANDOM_FOLDER) )
    {
        //LOG_TRACE << "CPlayEngineTaskHandler::RANDOM FILE SE" << LOG_ENDL;
        m_RandomMode = RANDOM_FILE;
        m_RandomPlayCount = 0;
    }
    else
    {
        //LOG_TRACE << "CPlayEngineTaskHandler::RANDOM FILE OFF" << LOG_ENDL;
        res = false;
    }
    return res;
}

bool CPlayEngineTaskHandler::isAcceptingTuneDial()
{
    bool isOKtoTune;
    isOKtoTune = (m_PBRequestInProgress.nRequestType != RequestType_MetaDataIndex
                            && GetLastQueueOfThisType(RequestType_MetaDataIndex) == -1
                            && !m_DelayMetadataIndexQueryTimer->isActive());
    //LOG_TRACE << "IsAcceptingTuneDial: "<< isOKtoTune <<LOG_ENDL;
    return isOKtoTune;
}

bool CPlayEngineTaskHandler::setRepeatMode(int mode)
{
    //LOG_TRACE << "setrepeatMode : " << mode << LOG_ENDL;
    bool res = false;
    if(m_ScanMode == SCAN_FILE)
    {
        //LOG_INFO<< "CPlayEngineTaskHandler::setRandomMode  scan ON " << mode << LOG_ENDL;
        m_ScanMode = SCAN_OFF;
        emit OnScanCompleted();
    }
    if(mode != -1)
    {
        m_RepeatMode = mode;
        res = true;
    }
    return res;
}

int CPlayEngineTaskHandler::getPrevIndexForConvertedDevice(int nStartIndex)
{
    //This function returns the previous index for converted device.
    int nCurrentPlayTrackCount = m_NumberOfPlayingTracks;
    int nPrevIndex = nStartIndex - 1;
    if(nCurrentPlayTrackCount <= 1) //do not do prev for single song
    {
        nPrevIndex = -1;
    }
    else if(nPrevIndex < 0)
    {
        nPrevIndex = nCurrentPlayTrackCount - 1;
    }
    //LOG_TRACE<<"PrevPrevPrev nStartIndex: "<<nStartIndex <<" nPrevIndex: "<<nPrevIndex
    //        << "m_NumberOfPlayingTracks "<< m_NumberOfPlayingTracks<<LOG_ENDL;
    return nPrevIndex;

}

int CPlayEngineTaskHandler::getNextIndexForConvertedDevice(int nStartIndex)
{
    //This function returns the next index for converted device.
    int nCurrentPlayTrackCount = m_NumberOfPlayingTracks;
    int nNextIndex = nStartIndex + 1;
    if(nCurrentPlayTrackCount <= 1) //do not do next for single song
    {
        nNextIndex = -1;
    }
    else if(nNextIndex >= nCurrentPlayTrackCount)
    {
        nNextIndex = 0;
    }
    //LOG_TRACE<<"NextNextNext nStartIndex: "<<nStartIndex <<" nNextIndex: "<<nNextIndex<< " m_NumberOfPlayingTracks "<< m_NumberOfPlayingTracks<<LOG_ENDL;
    return nNextIndex;
}

int CPlayEngineTaskHandler::getNextIndex()
{
    int dIndex = -1;
    if(m_RandomMode == RANDOM_FILE) // 1
    {
        // Added by jonghwan.cho@lge.com	2013.03.29.
        int randomFd = open("/dev/random", O_RDONLY);
        if( randomFd == -1 ) {
            //LOG_INFO <<"CPlayEngineTaskHandler::getNextIndex(), /dev/random FAILED!!!, so return " << LOG_ENDL;
            return -1;
        }

        int randomValue = 0;
        int ret = read(randomFd, &randomValue, sizeof(randomValue));

        if(randomValue < 0 )
            randomValue = randomValue*(-1);
        if( ret >= 0 ) {
            dIndex = ( randomValue % m_ControllerPlaylist.count() );
        }

        if(randomFd >= 0)
            close(randomFd);
        // Added by jonghwan.cho@lge.com	2013.03.29.

        //LOG_INFO <<"CPlayEngineTaskHandler::m_RandomPlayCount= " << m_RandomPlayCount << LOG_ENDL;
        if(m_RandomPlayCount >= m_ControllerPlaylist.count())
        {
            // if repeat is off..TODO add check
            // REPEAT_FILE check is of no use, as it is set in the device.
            if( (m_RepeatMode == REPEAT_OFF) )
            {
                if(((CIpodController *) parent())->m_IsIDPS)
                    emit OnStateChanged(2);
                else
                    emit OnStateChanged(0);
                //m_CurrentIndex = m_ControllerPlaylist.at(0);Coverity: SECURE_CODING:Do not copy data from an unbounded source to a fixed-length array
                m_CurrentIndex = m_ControllerPlaylist[0];
                m_CurrentPlaylistIndex = 0 ;
                m_RandomPlayCount = 0;
                dIndex = -1;
            }
            // 'm_RandomPlayCount' value is 0 when REPEAT_OFF mode and REPEAT_FOLDER mode
            else if(m_RepeatMode == REPEAT_FOLDER ) // removed as it is already checked || m_RepeatMode == REPEAT_OFF)
            {
                m_RandomPlayCount = 0;
            } 
        }
    }
    else
    {
        dIndex =  getCurrentIndex();
        ++dIndex;
        //Fix for CR13659 : A check added for out of bound condition 
        //for the incremented index. With this fix the 1st song is played
        //when next is pressed on the last song.
        if (dIndex > (m_ControllerPlaylist.count()  - 1))
        {
            dIndex = 0  ;
        }
    }

    if (m_ScanMode == SCAN_FILE)
    {
        //LOG_INFO <<"CPlayEngineTaskHandler::Next Scan else "<< LOG_ENDL;
        m_ScanMode = SCAN_OFF;
        emit OnScanCompleted();
        dIndex =  getCurrentIndex();
        //LOG_INFO <<"CPlayEngineTaskHandler::Next Scan dIndex=  "<< dIndex <<"count= " << m_ControllerPlaylist.count() <<LOG_ENDL;
        ++dIndex;
        if (dIndex > (m_ControllerPlaylist.count()  - 1))
        {
            dIndex = 0  ;
        }
    }
    //LOG_INFO <<"CPlayEngineTaskHandler::Next Index = " << dIndex << LOG_ENDL;
    return dIndex;
}

void CPlayEngineTaskHandler::setAutoPlayIndex(int index)
{
    m_CurrentPlaylistIndex = index; // Added by pawan 2013.06.19.
    m_AutoplayIndex = index;
}

int CPlayEngineTaskHandler::getAutoPlayIndex()
{
    return m_AutoplayIndex;
}

int CPlayEngineTaskHandler::getPrevIndex()
{
    int dIndex = -1;
    if(m_RandomMode == RANDOM_FILE) // 1
    {
        //LOG_INFO <<"CPlayEngineTaskHandler::Prev random file ON "<< LOG_ENDL;
        // Added by jonghwan.cho@lge.com	2013.03.29.
        int randomFd = open("/dev/random", O_RDONLY);
        if( randomFd == -1 ) {
            //LOG_INFO <<"CPlayEngineTaskHandler::getPrevIndex(), /dev/random FAILED!!!, so return " << LOG_ENDL;
            return -1;
        }

        int randomValue = 0;
        int ret = read(randomFd, &randomValue, sizeof(randomValue));

        if(randomValue < 0 )
            randomValue = randomValue*(-1);
        if( ret >= 0 ) {
            dIndex = ( randomValue % m_ControllerPlaylist.count() );
        }
        if(randomFd >= 0)
            close(randomFd);
        // Added by jonghwan.cho@lge.com	2013.03.29.

        if(m_RandomPlayCount >= m_ControllerPlaylist.count())
        {
            // REPEAT_FILE check is of no use, as it is set in the device.
            if( (m_RepeatMode == REPEAT_OFF) )
            {
                // if repeat is off..TODO add check
                if(((CIpodController *) parent())->m_IsIDPS)
                    emit OnStateChanged(2);
                else
                    emit OnStateChanged(0);
                //m_CurrentIndex = m_ControllerPlaylist.at(0); Coverity: SECURE_CODING:Do not copy data from an unbounded source to a fixed-length array
                m_CurrentIndex = m_ControllerPlaylist[0];
                m_CurrentPlaylistIndex = 0 ;
                m_RandomPlayCount = 0;
                dIndex = -1;
            }
            // 'm_RandomPlayCount' value is 0 when REPEAT_OFF mode and REPEAT_FOLDER mode
            else if(m_RepeatMode == REPEAT_FOLDER)// removed as the check is already done and there is no need to perform it again || m_RepeatMode == REPEAT_OFF)
            {
                m_RandomPlayCount = 0;
            }
        }
    }
    else
    {
        //LOG_INFO <<"CPlayEngineTaskHandler::Prev else "<< LOG_ENDL;
        dIndex =  getCurrentIndex();
        --dIndex;
        // Add a negative value processing when REPEAT_OFF mode and REPEAT_FOLDER mode
        if(m_RepeatMode == REPEAT_FOLDER || m_RepeatMode == REPEAT_OFF)
        {
            //LOG_INFO <<"CPlayEngineTaskHandler::REPEAT FOLDER is ON "<< LOG_ENDL;
            if(dIndex < 0)
                dIndex =  m_ControllerPlaylist.count()  - 1 ;
        }
    }

    if (m_ScanMode == SCAN_FILE)
    {
        //LOG_INFO <<"CPlayEngineTaskHandler::Prev Scan else "<< LOG_ENDL;
        m_ScanMode = SCAN_OFF;
        emit OnScanCompleted();
        dIndex =  getCurrentIndex();
        --dIndex;
        if (dIndex < 0)
        {
            dIndex =  m_ControllerPlaylist.count()  - 1 ;
        }
    }

    //LOG_INFO <<"CPlayEngineTaskHandler::Prev Index = " << dIndex << LOG_ENDL;
    return dIndex;
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::setStatusChangeNotifications     void (OUT), void (IN)
//
// Explanation
// This method subscribes for status change notifications
// Receive notifications for state, track index and play position changes
//
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
void CPlayEngineTaskHandler::setStatusChangeNotificationsE(bool dowait)
{

    m_CommandStatus = false;
    m_SentOneByteForm = false;
    m_PlayStatusNotifyState = eStateNotifySent;
    CSetPlayStatusChangeNotification cmd( m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS, m_iPodPrivateData->m_IsConvertedFromUIDToIndex );  // Added by pawan 2013.05.04.
    cmd.marshall();
    //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(cmd);
    emit sendiAPPacket(cmd.getCommandBytes());
    if(dowait)
    {
        m_ExpectedCommandID = 0x0026;
        waitForResponceFromExitCode(7000, ExitCode_SetPlayStatusChangeNotification);
    }
    else
    {
        m_ExpectedCommandID = 0x0000;
    }
    if(m_CommandStatus)
        m_StateNotificationSet = true;

}

//Added by Tan for no track count problem Happens a lot in nano 7G
bool CPlayEngineTaskHandler::getCurrentPlayingTrackIndex(int& index)
{
    bool retval = false;
    int ret;
    CGetCurrentPlayingTrackIndex cmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());
    ret = waitForResponceFromExitCode(7000, ExitCode_GetCurrentPlayingTrackIndex);
    if(ret == 0 && m_CurrentTrackIndexReceived >= 0)
    {
        retval = true;
        index = m_CurrentTrackIndexReceived;
        //delete duplicate OnTrackChanged
        //emit OnTrackChanged(m_CurrentTrackIndexReceived);
    }
    return retval;
}

bool CPlayEngineTaskHandler::setPosition(int position)
{
    //when setting position, make sure that ipod can take the value.
    //value greater than ipod duration will return bad parameter.
    //here we set maximum to 1 second less than duration to ensure it play the end of song.
    if(position/1000 + 1 >= m_CurrentMediaDuration/1000)
    {
        position = (m_CurrentMediaDuration/1000) - 1;
        if(position < 0)
            position = 0;
        position = position * 1000;
    }
    LOG_HIGH<<"sending setPosition to ipod: "<< position <<LOG_ENDL;
    m_CommandStatus = false;
    CSetiPodStateInfo setPosCmd(eSetDisplayRemotePosition, position, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS ); //TODO place the rite trans id
    setPosCmd.marshall();
    emit sendiAPPacket(setPosCmd.getCommandBytes());
    waitForResponceFromExitCode(7000, ExitCode_SetiPodStateInfo);
    return m_CommandStatus;
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::getNowPlayingListExistence     void (OUT), void (IN)
//
// Explanation
// This method returns the title of currently playing track
// Receive notifications for state, track index and play position changes
//
// History
// 19-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
QString CPlayEngineTaskHandler::getCurrentFileName()
{
    //LOG_TRACE << "CPlayEngineTaskHandler::getCurrentFileName() Entry" << LOG_ENDL;
    m_CurrentFileInfoString.clear();
    int dIndex = m_PlaylistURIs.count()-1;
    //LOG_INFO << "CPlayEngineTaskHandler::getCurrentFileName() dIndex "<<dIndex << LOG_ENDL;
    //LOG_INFO << "CPlayEngineTaskHandler::getCurrentFileName() m_PlaylistURIs.count() "<<m_PlaylistURIs.count() << LOG_ENDL;
    //LOG_INFO << "CPlayEngineTaskHandler::getCurrentFileName() m_CurrentPlaylistIndex "<<m_CurrentPlaylistIndex << LOG_ENDL;
    if(m_CurrentPlaylistIndex >= 0)
    {
        if( (m_PlaylistURIs.count() > 0) && (m_CurrentPlaylistIndex <= dIndex) )
        {
            m_CurrentFileInfoString = m_PlaylistURIs.at(m_CurrentPlaylistIndex);
        }
    }
    //LOG_INFO << " CPlayEngineTaskHandler::getCurrentFileName = " << m_CurrentFileInfoString.toAscii().data() << LOG_ENDL;
    return m_CurrentFileInfoString;
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::getCurrentMediaInfo     void (OUT), void (IN)
//
// Explanation
// This method returns media info of currently playing track
// Receive notifications for state, track index and play position changes
//
// History
// 19-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
void CPlayEngineTaskHandler::getCurrentMediaInfo(MediaInfo *pMediaInfo)
{
//    //LOG_TRACE << "CPlayEngineTaskHandler::getCurrentMediaInfo Entry" << LOG_ENDL ;
    if( pMediaInfo != NULL ) {
        MediaInfo sMediaInfo = m_iPodPrivateData->getCurrentMediaInfo();

        pMediaInfo->Title    = sMediaInfo.Title;
        pMediaInfo->Artist   = sMediaInfo.Artist;
        pMediaInfo->Album    = sMediaInfo.Album;
        pMediaInfo->Genre    = sMediaInfo.Genre;
        pMediaInfo->Composer = sMediaInfo.Composer;
        pMediaInfo->fileName = sMediaInfo.fileName;
        pMediaInfo->coverarturl = sMediaInfo.coverarturl ;
    }
//    //LOG_TRACE << "CPlayEngineTaskHandler::getCurrentMediaInfo Exit" << LOG_ENDL ;
}

void CPlayEngineTaskHandler::resetCurrentMediaInfo()
{
    //Tan. Commented by Tan. Should not reset and send empty string
    //m_CurrentIndexEmitted = -1; //added by junam 2013.04.21 for request again.
    m_iPodPrivateData->resetCurrentMediaInfo();
    m_MediaInfo.Title = " ";
    m_MediaInfo.Album = " ";
    m_MediaInfo.Artist = " ";
    m_MediaInfo.Genre = " ";
    m_MediaInfo.Composer=" ";
    m_CurrentMediaDuration = 0;
    m_MediaInfo.fileName = " ";
    m_CurrentFileInfoString.clear();        
    /*Tan. Do not need this anymore.
    MediaInfo sMediaInfo = m_iPodPrivateData->getCurrentMediaInfo();
    emit OnCurrentMediaInfo(&sMediaInfo);
    */
}

void CPlayEngineTaskHandler::emitCurrentMediaInfoE(MediaInfo *pMediaInfo)
{
    //LOG_INFO << "Emit media info m_CurrentIndex  " << m_CurrentIndex << "   m_CurrentIndexEmitted:" << m_CurrentIndexEmitted << LOG_ENDL;

    if(m_CategoryType.compare(DHTrackCategory, Qt::CaseInsensitive) && !m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
    {
        //LOG_TRACE<<"Refuse to update the track info because m_CategoryType is "<< m_CategoryType <<LOG_ENDL;
        return;
    }
    //LOG_TRACE<<"===="<<m_CurrentIndexEmitted<<":"<<m_CurrentIndex<<LOG_ENDL;
    //LOG_TRACE<<"===="<<m_MediaInfo.Title<<LOG_ENDL;

    if((m_CurrentIndexEmitted != m_CurrentIndex) || (m_MediaInfo.Title == " ") )
    {

        if(pMediaInfo != NULL)
        {
            bool isResult = false; //added by junam 2013.04.21 for use database data.
            m_iPodPrivateData->resetCurrentMediaInfo();
            m_MediaInfo.Title = " ";
            m_MediaInfo.Album = " ";
            m_MediaInfo.Artist = " ";
            m_MediaInfo.Genre = " ";
            if(m_IsSyncInProgress ==  true)
            {
                QString sSerialNumber = m_iPodPrivateData->getCurrentSerialNumber();
                bool ok = createConnection(sSerialNumber);
                if (ok)
                {
                    QSqlQuery query(dBHandle);
                    QString queryString;
                    queryString.clear();

                    queryString = QString("SELECT T_Title,T_Album,T_Artist,T_Duration,T_Genre,T_Composer,T_FilePath from MasterTable where T_index=") + QString::number(m_CurrentIndex);

                    ////LOG_TRACE << "query is" << queryString << LOG_ENDL;
                    bool bValue = false;
                    if( dBHandle.isValid() == true )  // Added by jonghwan.cho@lge.com 	2013.03.20.
                        bValue = query.exec(queryString);
                    else
                        bValue = false;
                    if( bValue == true )
                    {
                        //LOG_TRACE << "query success" << LOG_ENDL;
                        while(query.next())
                        {
                            isResult = true; //added by junam 2013.04.21 for use database data.

                            QString title = query.value(0).toString();
                            QString album = query.value(1).toString();
                            QString artist = query.value(2).toString();
                            m_CurrentMediaDuration = query.value(3).toInt();
                            QString genre = query.value(4).toString();
                            QString composer =query.value(5).toString();
                            QString filePath = query.value(6).toString();

                            emit OnDurationChanged(m_CurrentMediaDuration);

                            //LOG_INFO << "query inside while m_CurrentMediaDuration =  " << m_CurrentMediaDuration <<" m_CurrentIndex = " << m_CurrentIndex << LOG_ENDL;

                            m_MediaInfo.Title = title;
                            m_MediaInfo.Album = album;
                            m_MediaInfo.Artist = artist;
                            m_MediaInfo.Genre = genre;
                            m_MediaInfo.Composer = composer;
                            m_MediaInfo.fileName = filePath;

                        }
                    }
                } //{added by junam 2013.04.21 for use database data.
                else
                {
                    //LOG_INFO << "Fail to read current info...at sync";
                    return;
                }

                if(isResult)
                { //}added by junam
                    pMediaInfo->Title = m_MediaInfo.Title;
                    pMediaInfo->Artist = m_MediaInfo.Artist;
                    pMediaInfo->Album  = m_MediaInfo.Album;
                    pMediaInfo->Genre  = m_MediaInfo.Genre;
                    pMediaInfo->Composer = m_MediaInfo.Composer;
                    pMediaInfo->fileName = m_MediaInfo.fileName;
                    m_iPodPrivateData->setCurrentMediaInfo(m_MediaInfo);
                    m_CurrentIndexEmitted = m_CurrentIndex; //added by junam 2013.04.21 for use database data.

                    /* Tan: 2013.06.24
                    Commenting this out. The emit currentmediainfo will be call
                    one time only from ipodcontroller on updateHMIForBasicCoverArt
                    Multiple emit causes problem in OSD.*/
                    //LOG_TRACE << "Succesfully emit media info 1111 " << m_MediaInfo.Title <<LOG_ENDL;
                    emit OnCurrentMediaInfo(pMediaInfo);
                    //LOG_INFO << "CPlayEngineTaskHandler::emitCurrentMediaInfo: Title -"
                    //         << m_MediaInfo.Title.toAscii().data()
                    //         << ", Album- " << m_MediaInfo.Album.toAscii().data()
                    //         << ", Genre- " << m_MediaInfo.Genre.toAscii().data()
                    //         << ", Artist - " << m_MediaInfo.Artist.toAscii().data()
                    //         << ", Composer - " << m_MediaInfo.Composer.toAscii().data()
                    //         << ", Filepath - " << m_MediaInfo.fileName.toAscii().data()<< LOG_ENDL;
                    emit fetchCoversForBasicPlayer(pMediaInfo->Album, pMediaInfo->fileName);  // Modified by pawan 2013.05.16.
                }

            }
            else
            {
                //QString sSerialNumber = (parent()->findChild<CITunesDBTaskHandler*>(giTunesTaskHandler))->getCurrentSerialNumber();
                QString sSerialNumber = m_iPodPrivateData->getCurrentSerialNumber();
                bool ok = createConnection(sSerialNumber);
                if (ok)
                {
                    QSqlQuery query(dBHandle);
                    QString queryString;
                    queryString.clear();
                    queryString = QString("SELECT T_Title,T_Album,T_Artist,T_Duration,T_Genre,T_Composer,T_FilePath from MasterTable where T_index=") + QString::number(m_CurrentIndex);
                    ////LOG_TRACE << "query is" << queryString << LOG_ENDL;
                    bool bValue = false;
                    if( dBHandle.isValid() == true )
                        bValue = query.exec(queryString);
                    else
                        bValue = false;

                    if( bValue == true )
                    {
                        //LOG_TRACE << "query success" << LOG_ENDL;
                        while(query.next())
                        {
                            //LOG_TRACE << "query inside while" << LOG_ENDL;
                            isResult = true;//added by junam 2013.04.21 for use database data.
                            QString title = query.value(0).toString();
                            QString album = query.value(1).toString();
                            QString artist = query.value(2).toString();
                            m_CurrentMediaDuration = query.value(3).toInt();
                            QString genre = query.value(4).toString();
                            QString composer =query.value(5).toString();
                            QString filePath = query.value(6).toString();

                            emit OnDurationChanged(m_CurrentMediaDuration);
                            //LOG_INFO << "query inside while m_CurrentMediaDuration =  " << m_CurrentMediaDuration <<" m_CurrentIndex = " << m_CurrentIndex << LOG_ENDL;

                            m_MediaInfo.Title = title;
                            m_MediaInfo.Album = album;
                            m_MediaInfo.Artist = artist;
                            m_MediaInfo.Genre = genre;
                            m_MediaInfo.Composer = composer;
                            m_MediaInfo.fileName = filePath;

                        }
                    }
                }//{added by junam 2013.04.21 for use database data.
                else
                {
                    //LOG_INFO << "Fail to read current info...";
                    return;
                }

                if(isResult)
                {//}added by junam
                    pMediaInfo->Title = m_MediaInfo.Title;
                    pMediaInfo->Artist = m_MediaInfo.Artist;
                    pMediaInfo->Album  = m_MediaInfo.Album;
                    pMediaInfo->Genre  = m_MediaInfo.Genre;
                    pMediaInfo->Composer = m_MediaInfo.Composer;
                    pMediaInfo->fileName = m_MediaInfo.fileName;
                    m_iPodPrivateData->setCurrentMediaInfo(m_MediaInfo);
                    m_CurrentIndexEmitted = m_CurrentIndex;//added by junam 2013.04.21 for use database data.

                    /* Tan: 2013.06.24
                    Commenting this out. The emit currentmediainfo will be call
                    one time only from ipodcontroller on updateHMIForBasicCoverArt
                    Multiple emit causes problem in OSD.*/
                    emit OnCurrentMediaInfo(pMediaInfo);

                    //LOG_TRACE << "Succesfully emit media info 22222 " << m_MediaInfo.Title <<LOG_ENDL;

                    //LOG_INFO << "CPlayEngineTaskHandler::getCurrentMediaInfo: Title -"
                    //         << m_MediaInfo.Title.toAscii().data()
                    //         << ", Album- " << m_MediaInfo.Album.toAscii().data()
                    //        << ", Genre- " << m_MediaInfo.Genre.toAscii().data()
                    //         << ", Artist - " << m_MediaInfo.Artist.toAscii().data()
                    //        << ", Composer - " << m_MediaInfo.Composer.toAscii().data()
                    //         << ", Filepath - " << m_MediaInfo.fileName.toAscii().data()<< LOG_ENDL;
                    emit fetchCoversForBasicPlayer(pMediaInfo->Album, pMediaInfo->fileName);  // Modified by pawan 2013.05.16.
                }
            }
        }
    }
    else
    {
        //Already Media Info is emitted for the Current Index
#ifdef DBUGLOGS
        //LOG_TRACE << "Already Media Info is emitted for the Current Index" << LOG_ENDL;
#endif
    }
#ifdef DBUGLOGS
    //LOG_TRACE << "Emit media info Exit" << LOG_ENDL;
#endif
}

int CPlayEngineTaskHandler::getCurrentMediaPosition()
{
    return m_CurrentMediaPosition;
}

void CPlayEngineTaskHandler::resetCurrentMediaPositionVariable()
{
    m_CurrentMediaPosition = 0;
}

int CPlayEngineTaskHandler::getCurrentMediaDuration()
{
    return m_CurrentMediaDuration;
}

int CPlayEngineTaskHandler::getMCurrentIndex()
{
    return m_CurrentIndex;
}

int CPlayEngineTaskHandler::getCurrentIndex()
{
    int dIndex = -1;
    if(isPlayListSet())
    {
        dIndex = m_CurrentPlaylistIndex;
        //LOG_INFO <<"CPlayEngineTaskHandler::get current Index playlist set = " << dIndex << LOG_ENDL;
    }
    else
    {
        dIndex = m_CurrentIndex;
        //LOG_INFO <<"CPlayEngineTaskHandler::get current Index = " << dIndex << " " << m_CurrentIndex <<  LOG_ENDL;
    }
    return dIndex;
}

bool CPlayEngineTaskHandler::setCurrentIndexE(int index)
{
    bool result = true;
    //LOG_INFO << "In CPlayEngineTaskHandler::setCurrentIndex " << index
    //         << ", m_ControllerPlaylist.count() = " << m_ControllerPlaylist.count()<< LOG_ENDL;

    if(isPlayListSet())
    {
        if( (index < m_ControllerPlaylist.count()) && (index >= 0))
        {
            m_CurrentPlaylistIndex = index;
            if(m_ControllerPlaylist.at(index) >= 0)
                m_CurrentIndexToPlay = m_ControllerPlaylist.at(index);
        }
        else
        {
            if(m_RepeatMode == REPEAT_FOLDER)
            {
                m_CurrentIndexToPlay = m_ControllerPlaylist.at(0);
                m_CurrentPlaylistIndex = 0 ;
            }
            else
            {
                //LOG_INFO << "In CPlayEngineTaskHandler::setCurrentIndex within bounds" << index << LOG_ENDL;
                result = false;
                m_CurrentIndexToPlay = m_ControllerPlaylist.at(0);
                m_CurrentPlaylistIndex = 0 ;
            }
        }
    }
    else
    {
        if(index < 0)
        {
            result = false;
            index = -1;
        }
        m_CurrentIndexToPlay = index;

    }
    //LOG_TRACE<<"setCurrentIndexE Setting m_CurrentIndex to "<<m_CurrentIndexToPlay<<LOG_ENDL;
    m_CurrentIndex = m_CurrentIndexToPlay;
    //LOG_INFO << "In CPlayEngineTaskHandler::setCurrentIndex = " << m_CurrentIndexToPlay
             //<< "m_CurrentPlaylistIndex = " << m_CurrentPlaylistIndex << "Exit"<< LOG_ENDL;
    return result;
}

int CPlayEngineTaskHandler::getCurrentPlayStateE()
{
    //LOG_TRACE << "In CPlayEngineTaskHandler::getCurrentPlayState Entry m_PlayStatusNotifyState = " <<
    //             m_PlayStatusNotifyState << LOG_ENDL;
    QDateTime currentSysytemTime = QDateTime::currentDateTime();
    int differenceTime = m_CurrentTime.msecsTo( currentSysytemTime);
    if((differenceTime>0) && (differenceTime < ONESECOND ))
    {
        int sleeptime = (ONESECOND-differenceTime);
        //LOG_TRACE << "In CPlayEngineTaskHandler::getCurrentPlayState sleeptime = "
        //          << sleeptime << LOG_ENDL;
        // 1 msec = 1000 microsec
        usleep((sleeptime+USLEEPDELAY)*ONESECOND);
    }
    int nState = -1 ;
    while(nState == -1)
    {
        CGetPlayStatus cmdState(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
        cmdState.marshall();
        emit sendiAPPacket(cmdState.getCommandBytes());
        waitForResponceFromExitCode(7000, ExitCode_GetPlayStatus);
        if(m_CommandBuffer.length() > 0)
        {
            CReturnPlayStatus cmd( m_iPodPrivateData->m_isIDPS );
            cmd.unmarshall(m_CommandBuffer);
            nState = cmd.getCurrentState();
        }
        //LOG_INFO << " CPlayEngineTaskHandler::getCurrentPlayState m_CommandBuffer.length= "
        //         << m_CommandBuffer.length() <<"  and state is: " << nState << LOG_ENDL;
    }
    m_CurrentTime = currentSysytemTime;
    m_playState = nState;
    return nState;
}

void CPlayEngineTaskHandler::emitStateChangeForNonIDPS()
{
    //LOG_TRACE << "In CPlayEngineTaskHandler::emitStateChangeForNonIDPS Entry m_PlayStatusNotifyState  = "
    //          << m_PlayStatusNotifyState << LOG_ENDL;
    QDateTime currentSysytemTime = QDateTime::currentDateTime();
    int differenceTime = m_CurrentTime.msecsTo( currentSysytemTime);

    if((differenceTime>0) && (differenceTime < ONESECOND ))
    {
        int sleeptime = (ONESECOND-differenceTime);
        //LOG_TRACE << "In CPlayEngineTaskHandler::emitStateChangeForNonIDPS sleeptime = "
        //          << sleeptime << LOG_ENDL;
        // 1 msec = 1000 microsec
        usleep((sleeptime+USLEEPDELAY)*ONESECOND);
    }

    if ( m_PlayStatusNotifyState == eStateNotifyReceived)
    {
        int currentMediaState = ((CIpodController *) parent())->GetCurrentMediaState();
        //LOG_TRACE << "In CPlayEngineTaskHandler::emitStateChangeForNonIDPS If - currentMediaState = "
        //          << currentMediaState<< LOG_ENDL;

        switch(currentMediaState)
        {
        case 1: //Pause
        {
            emit OnStateChanged(11);
            break;
        }
        case 0: //Play
        {
            emit OnStateChanged(1);
            m_CurrentIndexEmitted = -1;// added by junam for 2013.04.17 for emit info when play again
            break;
        }
        case 2: //Stop
        {
            emit OnStateChanged(2);
            break;
        }
        }
    }
    else
    {
        //LOG_TRACE << "In CPlayEngineTaskHandler::emitStateChangeForNonIDPS else starting timer" << LOG_ENDL;
        if(m_PlayEngineTimer && m_PlayEngineTimer->isActive())
        {
            m_PlayEngineTimer->stop();
        }
        else
        {
            m_CurrentTime = currentSysytemTime;
            //return; 	//Commented to fix Nano2G first time play problem
        }
        m_PlayEngineTimer->start(1000);
    }
    m_CurrentTime = currentSysytemTime;
    //LOG_TRACE << "In CPlayEngineTaskHandler::emitStateChangeForNonIDPS Exit" << LOG_ENDL;
}

void CPlayEngineTaskHandler::getPlayStatusATS()
{
    //LOG_TRACE << "In CPlayEngineTaskHandler::getPlayStatusATS  "<< LOG_ENDL;

    QByteArray cmd;
    cmd[0] = 0x55;
    cmd[1] = 0x03;
    cmd[2] = 0x04;
    cmd[3] = 0x00;
    cmd[4] = 0x1C;
    cmd[5] = 0xDD;
    CIAPPacket packet(cmd);
    m_emitStateForNONIDPS = true;

    emit sendiAPPacket(packet.getCommandBytes());

    m_PlayStatusNotifyState = eDefault;
}

void CPlayEngineTaskHandler::emitDurationMilliSecondsE()
{
    //LOG_INFO << "curretn index = "<< m_CurrentIndex << LOG_ENDL;
    CGetIndexedPlayingTrackInfo infoCmd(m_CurrentIndex,TRACKDURATIONINFO, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS ); //TODO place the rite trans id
    infoCmd.marshall();
    m_isDurationRequested = true;
    emit sendiAPPacket(infoCmd.getCommandBytes());
    waitForResponceFromExitCode(7000, ExitCode_GetIndexedPlayingTrackInfo);
}

bool CPlayEngineTaskHandler::isPlayListSet()
{
    bool result = false;
#ifdef DBUGLOGS
    //LOG_TRACE << "CPlayEngineTaskHandler::isPlayListSet() m_ControllerPlaylist.count() = " << m_ControllerPlaylist.count() << LOG_ENDL;
#endif
    if(m_ControllerPlaylist.count() > 0)
        result = true;
    return result;
}

int CPlayEngineTaskHandler::getMaxUIDsPerCmd()
{
    //LOG_TRACE<<"setUIDPlayListEmCPlayEngineTaskHandler::getMaxUIDsPerCmd Entry"<<LOG_ENDL;
    int dMaxUIDs = 0 ;
    if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex == true)
    {
        int dLingoIdPlusCmdId = 3; //1 = lingo id : 2 - cmd id for extended lingo cmds
        int dMaxPlayloadSize = MAX_PAYLOAD_SIZE;
        //LOG_TRACE<<"setUIDPlayListE CPlayEngineTaskHandler::getMaxUIDsPerCmd dMaxPlayloadSize "<< dMaxPlayloadSize <<LOG_ENDL;
        if(m_iPodPrivateData->m_isIDPS == true)
            dLingoIdPlusCmdId += 2; // 2 bytes trans id
        //LOG_TRACE<<"setUIDPlayListE CPlayEngineTaskHandler::getMaxUIDsPerCmd dLingoIdPlusCmdId "<< dLingoIdPlusCmdId <<LOG_ENDL;
        dMaxUIDs = (dMaxPlayloadSize - dLingoIdPlusCmdId) / BYTE; // if payload = 560 : (560 - 8) / 8 = no of UIDs : 8 bytes per UID
        //LOG_TRACE<<"setUIDPlayListE CPlayEngineTaskHandler::getMaxUIDsPerCmd dMaxUIDs "<< dMaxUIDs <<LOG_ENDL;
    }
    //LOG_TRACE<<"setUIDPlayListE CPlayEngineTaskHandler::getMaxUIDsPerCmd Exit"<<LOG_ENDL;
    return dMaxUIDs;
}

void CPlayEngineTaskHandler::getIndexAndUID(QString filePath, int& index, quint64& uid)
{
    QString uidStr;
    QString indexStr;
    int i = filePath.lastIndexOf("/");
    int _i = filePath.indexOf("_");
    int i_ = filePath.lastIndexOf("_");

    if(_i == i_)
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

void CPlayEngineTaskHandler::setPlayListCategoryItems(QString category, int firstDepth, int secondDepth)
{
    m_bDBSelectionHasChanged = true;
    if(!m_CategoryType.compare(category) && m_CurrentPlaylistIndex==firstDepth && !m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
    {
        //LOG_TRACE <<"PLAYLIST: same category is seleted again so no select db record" << LOG_ENDL;
    }
    else
    {
        m_IsPlayListSpecificCategoryIsSet = true;
    }
    int random = RANDOM_OFF;
    m_RandomMode = RANDOM_OFF;
    setRandomMode(random);
    setShuffleE(random);
    emit OnModeChanged(RANDOMOFF);

    m_CategoryType = category;
    m_ControllerPlaylist.clear();
    m_ControllerPlayListUID.clear();
    setCurrentIndexE(secondDepth);
    m_CurrentPlaylistIndex = firstDepth;
}

int CPlayEngineTaskHandler::startAutoplay(CiOSAppTaskHandler* iOSTH)
{
    //LOGH LOG_ENDL;

    int index;
    if (m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
    {
        //LOG_TRACE << "AUTOPLAY: startautoplay for the UID devices" << LOG_ENDL;
        bool isNowPlayingMP = iOSTH->isNowPlayingApplicationIsMPE();
        LOGH " isNowPlayingMP = " << isNowPlayingMP << LOG_ENDL;
        //LOG_INFO << "   now playing is MP = " << isNowPlayingMP << ", IPAD_MUSICAPP_APP_ID = " << IPAD_MUSICAPP_APP_ID << LOG_ENDL;
        if(isNowPlayingMP)
        {
            /*
            //we have checked that we do not need to restart play,
            //however, for some reason if basic control is OFF, we have to
            //turn it back on.
            if(iOSTH->isBasicControlEnabled() == false)
            {
                //LOG_HIGH <<"Turning BasicControl Back ON 4"<<LOG_ENDL;
                iOSTH->delayBasicControlEnable(true, "Music");
            }
            */

            //LOG_TRACE << "AUTOPLAY: now playing is MP" << LOG_ENDL;

            int totalSongs = 0;
            index = -1;
            getCurrentPlayingTrackIndex(index);
            //LOGH "  getCurrentPlayingTrackIndex = " << index << LOG_ENDL;
            if(index != -1)
            {
                //LOG_TRACE <<"AUTOPLAY: autoplay state of IPod is: " << m_playState << LOG_ENDL;
                setCurrentIndexE(index);
                GetPBTrackInfoWithQueue(RequestType_MetaDataIndex, index, eDefault_Id, 0);
                totalSongs = getNumberOfPlayingTracks();
                m_NumberOfPlayingTracks = totalSongs;
                emit OnCurrentPlayingTrackCount(totalSongs);
            }
            else
            {
                emit OnStateChanged(10);
                setCurrentIndexE(0);
                setAutoPlayIndex(0);
                //reset of prev selection and list count is already done in StartPostExtendedInit()
                //so just do the make selection
                //m_iPodDBIndexFetchTaskHandler->resetPreviousSelectionsE();
                //m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE(TRACKCATEGORY);
                m_iPodDBIndexFetchTaskHandler->makeSelectionE(TRACKCATEGORY, 0);
                m_playState = ePlayState_PLAY;//Play
            }
            //LOG_TRACE << "AUTOPLAY: playing song count: " << totalSongs << "  current index: " << index << LOG_ENDL;
        }
        else if (iOSTH->m_ApplicationBundleName1 == "" || iOSTH->m_ApplicationBundleName1 == " ")
        {
            LOG_HIGH << "Tan Nothing is playing. Start auto play forcefully" << LOG_ENDL;
            PlayiPodFiles();
        }
        else
        {
            LOG_HIGH << "AUTOPLAY: now playing is not MP" << LOG_ENDL;
            iOSTH->RestartPlay();

            int iter_count = 0;
            while(m_iPodPrivateData->m_IsRestartOfMPInProgress && iter_count < 10)
            {
                iter_count++;
                //LOG_TRACE << "AUTOPLAY: waiting to restartthe MP" << LOG_ENDL;
                waitForResponceFromExitCode(200, 0x00);
            }
            index = 0;
            bool ret = getCurrentPlayingTrackIndex(index);
            //LOGH " getCurrentPlayingTrackIndex = " << index << LOG_ENDL;
            setCurrentIndexE(index);
            setAutoPlayIndex(index);
            if(ret)
            {
                m_playState = ePlayState_PLAY;//Play
                //LOG_TRACE << "AUTOPLAY: play started so be happy :) index is: " << index << LOG_ENDL;
            }
        }
    }
    else
    {
        //LOG_TRACE << "AUTOPLAY: its Index device so lets play normally" <<LOG_ENDL;
        //m_iPodPrivateData->m_TotalIpodTrackCount = m_iPodDBIndexFetchTaskHandler->getTotalCountOfiPodTracksE();
        //LOG_HIGH << "   emit IPodTotalNumTracks( m_PrivateData->m_TotalIpodTrackCount = " << m_iPodPrivateData->m_TotalIpodTrackCount << " )" << LOG_ENDL;
        emit OnStateChanged(10);
        setCurrentIndexE(0);
        setAutoPlayIndex(0);
        m_FirstTimePlay = false;
        MediaInfo currentMediaInfo;
        emitCurrentMediaInfoE(&currentMediaInfo);
        playControlE("Play");
        m_playState = ePlayState_PLAY;//Play
    }
    return m_playState;
}

void CPlayEngineTaskHandler::setUIDPlayListE(QList<QString> *listUris)
{
    //LOG_INFO<<"CPlayEngineTaskHandler::setUIDPlayListE:Entry"<<LOG_ENDL;

    if(!m_iPodPrivateData->m_IsCurrentPlayingMP)
    {
        m_iPodPrivateData->m_IsRestartOfMPInProgress = true;
    }

    m_ControllerPlayListUID.clear();
    m_ControllerPlaylist.clear();
    if(listUris != NULL)
    {
        if((listUris->count() == m_iPodPrivateData->m_TotalIpodTrackCount) || listUris->count() == 0)
        {
            m_IsPlaylistSetByDH = true;
            //LOG_INFO<<"CPlayEngineTaskHandler::setUIDPlayListE either all song or no song"<<LOG_ENDL;
            return;
        }
    }

    QList<quint64> tempUIDPlayList;
    tempUIDPlayList.clear();
    QByteArray total_bytes;
    total_bytes.clear();
    if(listUris != NULL)
    {
        QList<QString> uris = *listUris;
        int dCountUirs = uris.count();

        if(dCountUirs > 0)
        {
            int total_count = dCountUirs;
            //LOG_TRACE << "CPlayEngineTaskHandler::setUIDPlayListE:Entry URIs count:" << dCountUirs << LOG_ENDL;

            for( int i = 0 ; i < total_count ; i++ )
            {
                quint64 value=0;
                int index=-1;
                getIndexAndUID(uris.at(i), index, value);

                if(value)
                {
                    tempUIDPlayList.append(value);
                    QByteArray bytes;
                    bytes.resize(sizeof(quint64));
                    for(int i = 7 ; i >=0 ;i--)
                    {
                        bytes[i] = (value >> 0) & 0x00FF;
                        value >>= 8;
                    }
                    total_bytes.append(bytes);
                    m_ControllerPlayListUID.append(bytes);
                }
            }

            if (total_bytes.size() > 0 )
            {
                int dNumberOfUIDs = total_bytes.size() / BYTE;
                int dUIDsPerPacket = getMaxUIDsPerCmd();
                int currentSect = 0;
                int maxSect = 0 ;
                int dEndPos = dNumberOfUIDs * BYTE;

                if( dNumberOfUIDs > dUIDsPerPacket )
                {
                    if(dUIDsPerPacket != 0)
                    {
                        maxSect = dNumberOfUIDs / dUIDsPerPacket;
                    }
                    else
                    {
                        LOG_HIGH<<"CRITICAL ERROR, setUIDPlayListE, dUIDsPerPacket is 0"<<LOG_ENDL;
                        return;
                    }
                    dEndPos = dUIDsPerPacket * 8;
                    if(dNumberOfUIDs % dUIDsPerPacket)
                        ++maxSect;
                }

                int dStartPos = currentSect;
                if(maxSect > 0)
                    --maxSect ;

                int nTransactionID = m_iPodPrivateData->getTransactionId();
                QByteArray uidsToSend;  // Added by jonghwan.cho@lge.com    2013.04.16.

                if(!m_PreparePlaylistStatus)   // added by pawan 2013.04.29.
                {
                    //LOG_TRACE << "setplaylist inside emission " << LOG_ENDL;
                    while( currentSect <= maxSect )
                    {
                        if(m_DelayMetadataIndexQueryTimer)
                            m_DelayMetadataIndexQueryTimer->stop();
                        m_PreparePlaylistStatus = false;
                        uidsToSend = total_bytes.mid(dStartPos, dEndPos);

                        CPrepareUIDList cmd(currentSect, maxSect, nTransactionID, uidsToSend, m_iPodPrivateData->m_isIDPS);
                        cmd.marshall();
                        emit sendiAPPacket(cmd.getCommandBytes());
                        waitForResponceFromExitCode(20000, ExitCode_PreparedUIDList);
                        dStartPos = dStartPos + dEndPos;
                        if(!m_PreparePlaylistStatus)
                        {
                            //LOG_TRACE << "CPlayEngineTaskHandler::setUIDPlayListE:Entry CPrepareUIDList failed " << LOG_ENDL;
                            break;
                        }
                        currentSect++;
                    }  // addded by pawan 2013.04.29.
                }
            }

            if(m_PreparePlaylistStatus)
            {
                if(((CIpodController *) parent())->m_IsIDPS)
                    emit OnStateChanged(2);
                else
                    emit OnStateChanged(0);

                m_CurrentPlaylistIndex = 0;
            }
        }

        if( (tempUIDPlayList.count() > 0 ) && m_PreparePlaylistStatus ) {
            //LOG_TRACE << "CPlayEngineTaskHandler::setUIDPlayListE:Entry CPlayEngineTaskHandler::setUIDPlayList - setUIDPlayList Success, setting new list " << LOG_ENDL;
        }
        else {
            m_ControllerPlayListUID.clear();
            //LOG_TRACE << "CPlayEngineTaskHandler::setUIDPlayListE:Entry CPlayEngineTaskHandler :: setUIDPlayList Failed to create table URIs with new list " << LOG_ENDL;
        }
    }
    //LOG_INFO<<"CPlayEngineTaskHandler::setUIDPlayList:Exit"<<LOG_ENDL;
}

void CPlayEngineTaskHandler::setPlayListE(QList<QString> *listUris)
{
    /* Usage of mid on QString
    QString x = "Nine pineapples";
    QString y = x.mid(5, 4);            // y == "pine"
    QString z = x.mid(5);               // z == "pineapples"
    */
    //LOG_TRACE << "CPlayEngineTaskHandler::setPlayListE Entry" << LOG_ENDL;
    if(listUris != NULL)
    {
        QList<QString> uris = *listUris;
        //this means prev playlist category was set so we need to do the make selction
        if(!m_iPodPrivateData->m_IsConvertedFromUIDToIndex && m_CategoryType.compare(DHTrackCategory, Qt::CaseInsensitive))
        {
            m_IsPlaylistSetByDH = true;
        }
        m_CategoryType = DHTrackCategory;

        int randomMode = RANDOM_OFF;
        setRandomMode(randomMode);
        m_RandomMode = RANDOM_OFF;
        setShuffleE(randomMode);
        emit OnModeChanged(RANDOMOFF);
        // { Added by pawan 2013.05.08.
        if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)  // Modified by pawan 2013.05.28.
        {
            //LOG_TRACE << "setUIDPlayListE its a converted device so, set to UID playlist" << LOG_ENDL;
            setUIDPlayListE(listUris);
            return;
        }
        else if(uris.count() <= 0 && !m_iPodPrivateData->m_IsConvertedFromUIDToIndex) //only for non converted devices
        {
            //LOG_TRACE << "devplay setplaylist, m_NumberOfPlayingTracks: " << m_NumberOfPlayingTracks << LOG_ENDL;
            m_NumberOfPlayingTracksReceivedFromIPod = m_iPodPrivateData->m_TotalIpodTrackCount;

            createConnection(m_iPodPrivateData->getCurrentSerialNumber());
            QSqlQuery query(dBHandle);
            QString queryString("select count(T_Index) from MasterTable");
            bool ret = query.exec(queryString);
            if(ret && query.next())
            {
             m_NumberOfPlayingTracks = query.value(0).toInt(&ret);
            }

            //LOG_TRACE << "devplay setplaylist, m_NumberOfPlayingTracks: " << m_NumberOfPlayingTracks << LOG_ENDL;
            m_ControllerPlaylist.clear();
            for(int i=0; i< m_NumberOfPlayingTracks; i++)
            {
             m_ControllerPlaylist.append(i);
            }
            return;
        }

     	// } Added by pawan 2013.05.08.
        m_ControllerPlaylist.clear();
        m_ControllerPlaylistType.clear();
        m_TopLevelSelected  = false;
        m_RandomPlayCount = 0;
        m_NumberOfPlayingTracks = 0;    // Added by pawan 2013.05.03.
        int dCountUirs = uris.count();
        int dIndexOf_ = -1;
        bool ok  = false;
        ////LOG_INFO << "uris[0]=" << uris.at(0) << "[" << QString("/"+gIPODPlayListType+"/") << "]" <<LOG_ENDL;

        if(dCountUirs > 0)
        {
            if(uris.at(0).contains(QString("/"+gIPODAllSongType+"/"), Qt::CaseSensitive))
                m_ControllerPlaylistType = gIPODAllSongType;
            else if(uris.at(0).contains(QString("/"+gIPODAudioBookType+"/"), Qt::CaseSensitive))
            {
                dIndexOf_ = uris.at(0).lastIndexOf("_");
                m_TopLevelIndex = (uris.at(0).mid(dIndexOf_ + 1)).toInt(&ok,10);
                m_ControllerPlaylistType = gIPODAudioBookType;
            }
            else if(uris.at(0).contains(QString("/"+gIPODPodcastType+"/"), Qt::CaseSensitive))
            {
                dIndexOf_ = uris.at(0).lastIndexOf("_");
                m_TopLevelIndex = (uris.at(0).mid(dIndexOf_ + 1)).toInt(&ok,10);
                m_ControllerPlaylistType = gIPODPodcastType;
            }
            // setPlaylist about iTunes category
            else if(uris.at(0).contains(QString("/"+gIPODiTunesUType+"/"), Qt::CaseSensitive))
            {
                dIndexOf_ = uris.at(0).lastIndexOf("_");
                m_TopLevelIndex = (uris.at(0).mid(dIndexOf_ + 1)).toInt(&ok,10);
                m_ControllerPlaylistType = gIPODiTunesUType;
            }
        }
        // { Added by pawan 2013.05.03.
        else  
        {                 
            //LOG_TRACE << " returning from set playlist as its null" << LOG_ENDL;
            return;
        }
        // } Added by pawan 2013.05.03.
        //LOG_INFO << "m_ControllerPlaylistType= " << m_ControllerPlaylistType.toAscii().data() << LOG_ENDL;
        ok = false;
        m_PlaylistURIs.clear();
        m_CurrentPlaylistIndex = -1 ;
        m_StateNotificationSet = false;
        m_CurrentIndex = -1;
        if(((CIpodController *) parent())->m_IsIDPS)
            emit OnStateChanged(2);
        else
            emit OnStateChanged(0);

        if(m_SetPlaylistCount == 1)
        {
            setShuffleE(RANDOM_OFF);
            //setRepeat(REPEAT_OFF);
            setRepeatE(REPEAT_FOLDER);
            m_SetPlaylistCount++;
        }
        for(int dUriIndex = 0; dUriIndex < dCountUirs; dUriIndex++ )
        {
            m_PlaylistURIs.append(uris.at(dUriIndex));
            int dLastIndexOfSlash = uris.at(dUriIndex).lastIndexOf("/");
            dIndexOf_ = uris.at(dUriIndex).lastIndexOf("_");
            int nTrackIndex = -1;

            // gIPODPlayListType case Commented for CR 13014
            if( /*(m_ControllerPlaylistType == gIPODPlayListType)  || */(m_ControllerPlaylistType == gIPODPodcastType)
                    || (m_ControllerPlaylistType == gIPODAudioBookType) || (m_ControllerPlaylistType == gIPODiTunesUType)) // add by jungae 2012.07.16 for CR11286, iTunes type addtiion
            {
                QString sTemp = uris.at(dUriIndex);
                //LOG_INFO << "sTemp= " << uris.at(dUriIndex) << LOG_ENDL;
                sTemp.chop(sTemp.length() - dIndexOf_);
                uris.replace(dUriIndex,sTemp);
                dIndexOf_ = uris.at(dUriIndex).lastIndexOf("_");
            }
            nTrackIndex = (uris.at(dUriIndex).mid((dLastIndexOfSlash + 1),(dIndexOf_ - dLastIndexOfSlash - 1))).toInt(&ok,10);
            // select a song from songslist, there are chances of podcast, audiobok,iTunes present in that list.And the uris.at(0) will contain m_ControllerPlaylistType as "AllSOngs"
            // In this case the index for podcast,audiobook,iTunes won't be fetched properly (i.e.,nTrackIndex = 0 and ok = false). In that case a cross check is been made to get the proper index of them and placed in playlist
            if(!ok)
            {
                QString sTemp = uris.at(dUriIndex);
                int underScoreCount = sTemp.count(QRegExp("_"));
                //LOG_INFO << "CPlayEngineTaskHandler::setPlayListE underScoreCount  = " <<underScoreCount
                //         << "  sTemp = " << sTemp << LOG_ENDL;
                if (underScoreCount == 2)
                {
                    sTemp.chop(sTemp.length() - dIndexOf_);
                    uris.replace(dUriIndex,sTemp);
                    dIndexOf_ = uris.at(dUriIndex).lastIndexOf("_");
                    nTrackIndex = (uris.at(dUriIndex).mid((dLastIndexOfSlash + 1),(dIndexOf_ - dLastIndexOfSlash - 1))).toInt(&ok,10);
                    //LOG_INFO << "CPlayEngineTaskHandler::setPlayListE nTrackIndex  = " << nTrackIndex <<LOG_ENDL;
                }
            }

            if(ok == true)
                m_ControllerPlaylist.insert(dUriIndex,nTrackIndex);
            else
                m_ControllerPlaylist.insert(dUriIndex,-1);
        }
        //Commented to remove extra logs

        m_NumberOfPlayingTracks = m_ControllerPlaylist.size(); // added by junam 2013.04.13 for AutoStart
    }
}

bool CPlayEngineTaskHandler::IsPlaylistSpecificCategoryIsSet()
{
    return m_IsPlayListSpecificCategoryIsSet;
}

// { Added by pawan 2013.05.06.
int CPlayEngineTaskHandler::getNumberOfPlayingTracks()
{
    CGetNumOfPlayingTracks cmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());
    waitForResponceFromExitCode(7000, ExitCode_GetNumOfPlayingTracks);
    return m_NumberOfPlayingTracksReceivedFromIPod;;
}
// } Added by pawan 2013.05.06.

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::packetAvailable                  void (OUT), (int,int,QByteArray) (IN)
//
// Explanation
// This method is a callback that is called when a command packet is available from the apple device.
// The task handler checks the lingoId and commandId input params and does the needful with the
// command bytes in the QByteArray
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
void CPlayEngineTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_INFO << "CPlayEngineTaskHandler packetAvailable: lingoID: " << lingoID
             << " - CommandId: " << commandID << LOG_ENDL;
#endif
    try {// added by sungha.choi 2013.04.16
        //TODO : instead of commandID -> should be MSB and LSB seperately sent
        //commandID shall be got after <<8 of MSB and | with LSB
        if(lingoID == 0x04)
        {
            switch(commandID)
            {
            //ACK = 0x01
            case 0x01:
            {
                CACK cmd( m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(buf);
                int ackCmd= cmd.getAcknowledgedCommand();
                //LOG_INFO << " Ack status of " << ackCmd << " is " << cmd.getAckStatus() <<LOG_ENDL;
                switch(ackCmd)
                {
                //PlayPreparedUIDList
                case 0x004B:
                {
                    //LOG_TRACE << "setUIDPlayListE got ack for playlist " << LOG_ENDL;
                    // get the status and do the needful
                    m_CommandStatus = !cmd.getAckStatus();
                    //LOG_TRACE << "setUIDPlayListE got ack for playlist: " << m_CommandStatus << LOG_ENDL;
                    emit mainLoopQuit(ExitCode_PlayPreparedUIDList);
                    break;
                }
                case 0x004A:
                {
                    //LOG_TRACE << "CPlayEngineTaskHandler::setUIDPlayListE: In case 4A " << LOG_ENDL;
                    switch(cmd.getAckStatus())
                    {
                    case 0x00:
                    case 0x13:
                    {
                        m_PreparePlaylistStatus = true;
                        //LOG_TRACE << "CPlayEngineTaskHandler::setUIDPlayListE:Entry  m_PreparePlaylistStatus set to true  " << LOG_ENDL;
                        emit mainLoopQuit(ExitCode_PreparedUIDList);
                        break;
                    }
                    default:
                        //LOG_TRACE << "CPlayEngineTaskHandler::setUIDPlayListE:Entry error is:" << cmd.getAckStatus() << LOG_ENDL;
                        break;
                    }
                    break;
                }
                //0x0028 - PlayCurrentSelection
                //Find the next category in the heirarchy
                case 0x0028:
                {
                    // get the status and do the needful
                    m_CommandStatus = !cmd.getAckStatus();
                    emit mainLoopQuit(ExitCode_PlayCurrentSelection);
                    break;
                }
                case 0x003E: //ack comes for the sent command getuidtrackinfo (not 0x003F, Retuidtrackinfo)
                {
                    m_CommandStatus = !cmd.getAckStatus();
                    //LOG_TRACE << "0x003F got ACK: " << m_CommandStatus << LOG_ENDL;
                    break;
                }
                case 0x0042:
                {
                    m_CommandStatus = !cmd.getAckStatus();
                    //LOG_TRACE << "0x0043 got ACK: " << m_CommandStatus << LOG_ENDL;
                    QuitPBLoopAndProcessNextRequest();
                    break;
                }
                //playControl
                case 0x0029:
                {
                    // get the status and do the needful
                    m_CommandStatus = !cmd.getAckStatus();
                    m_CurrentCommand = cmd.getAcknowledgedCommand();
                    emit mainLoopQuit(ExitCode_PlayControl);
                    //DBGLOG() << "Ack status is " << m_CommandStatus;
                    break;
                }
                //setRepeat
                case 0x0031:
                {
                    // get the status and do the needful
                    m_CommandStatus = !cmd.getAckStatus();
                    emit mainLoopQuit(ExitCode_SetRepeat);
                    break;
                }
                //get num of playing tracks failed
                case 0x0035:
                {
                    if(cmd.getAckStatus()!= 0x00)
                    {
                        emit mainLoopQuit(ExitCode_GetNumOfPlayingTracks);
                    }
                    break;
                }
                case 0x0037:
                {
                    //LOG_INFO << "SetCurrentPlayingTrack 0x0037 = " << cmd.getAckStatus() << LOG_ENDL;
                    m_CommandStatus = !cmd.getAckStatus();
                    emit mainLoopQuit(ExitCode_SetCurrentPlayingTrack);
                    break;
                }
                //setShuffle
                case 0x002E:
                {
                    // get the status and do the needful
                    m_CommandStatus = !cmd.getAckStatus();
                    emit mainLoopQuit(ExitCode_SetShuffle);
                    break;
                }
                case 0x001A://CReturnCategorizedDatabaseRecords
                {
                    m_CommandStatus = !cmd.getAckStatus();
                    if(!m_CommandStatus)
                    {
                        emit mainLoopQuit(ExitCode_RetrieveCategorizedDatabaseRecords);
                    }
                    break;
                }
                //SetPlayStatusChangeNotification
                case 0x0026:
                {
                    // get the status and do the needful
                    m_CommandStatus = !cmd.getAckStatus();
                    if(!m_CommandStatus)
                    {
                        if(m_SentOneByteForm == false)
                        {
                            m_SentOneByteForm = true;
                            //LOG_TRACE << "Sending one byte form of setPlayStatusNotification;" << LOG_ENDL;
                            //no trans id	int m_TransId = 8;
                            QByteArray cmd;
                            cmd[0] = 0x55;
                            cmd[1] = 0x04;
                            cmd[2] = 0x04;
                            cmd[3] = 0x00;
                            cmd[4] = 0x26;
                            cmd[5] = 0x01;

                            cmd.append(CIpodControllerGlobal::checkSum(cmd, 5));
                            CIAPPacket packet(cmd);
                            emit sendiAPPacket(packet.getCommandBytes());
                            //LOG_TRACE << "Sending one byte form of setPlayStatusNotification; Exit" << LOG_ENDL;
                        }
                    }
                    else
                    {
                        if(m_SentOneByteForm == true)
                        {
                            //LOG_TRACE << "Sending one byte form of setPlayStatusNotification; SUCCESS" << LOG_ENDL;
                            m_SentOneByteForm = false;
                        }
                        else
                        {
                            //LOG_TRACE << "Sending four byte form of setPlayStatusNotification; SUCCESS" << LOG_ENDL;

                        }
                        if(m_ExpectedCommandID == 0x0026)
                            emit mainLoopQuit(ExitCode_SetPlayStatusChangeNotification);
                    }
                    break;
                }
                }
                break;
            }

            case 0x001B: //CReturnCategorizedDatabaseRecords
            {
                CReturnCategorizedDatabaseRecords cmd( m_iPodPrivateData->m_isIDPS);
                cmd.unmarshall(buf);

                QMap<int,QString> *pValue = NULL;
                if(m_iPodPrivateData->m_CommandMap.contains(iPodControllerPrivateData::COMMANDKEY_PLAYLIST_LIST))
                    pValue = m_iPodPrivateData->m_CommandMap.value(iPodControllerPrivateData::COMMANDKEY_PLAYLIST_LIST);
                else if(m_iPodPrivateData->m_CommandMap.contains(iPodControllerPrivateData::COMMANDKEY_PLAYLIST_SONG))
                    pValue = m_iPodPrivateData->m_CommandMap.value(iPodControllerPrivateData::COMMANDKEY_PLAYLIST_SONG);
                if(pValue)
                {
                    pValue->insert( cmd.getRecordIndex(), cmd.getRecordString());
                    if(pValue->count() == m_PlaylistCount)
                    {
                        emit mainLoopQuit(ExitCode_RetrieveCategorizedDatabaseRecords);
                    }
                }
                break;
            }
            case 0x003F:
            {
                m_MetaDataArray.append(buf);
                //LOG_TRACE <<"devplay got the buffle for 0x003F and array count is: " << m_MetaDataArray.count() << LOG_ENDL;
                if(m_MetaDataArray.count() ==  NO_OF_META_INFO_UID_Track_Info )
                {
                    //LOG_TRACE<<"emitting devplay CPlayEngineTaskHandler: m_MetaDataArray got 0x003F" << m_MetaDataArray.count() <<LOG_ENDL;
                    emit CurrentUIDMediaInfoRetrieved();
                }
                break;  // Added by pawan 2013.05.06.
            } // { Added by pawan 2013.05.06.
            case CIAPCmds::L4_43_RetPBTrackInfo:
            {
                m_TuneMetaDataArray.append(buf);
                //LOG_TRACE <<"devplay got the buffle for 0x0043  and array count is: " << m_TuneMetaDataArray.count() << LOG_ENDL;
                if(m_TuneMetaDataArray.count() ==  NO_OF_META_INFO_PB_Track_Info )
                {
                    //LOG_TRACE<<"emitting devplay CPlayEngineTaskHandler: m_MetaDataArray got 0x0043 " << m_TuneMetaDataArray.count() <<LOG_ENDL;
                    emit CurrentPBMediaInfoRetrieved();
                }
                break;
            }
            case 0x0036: //ReturnNumberOfPlayingTracks
            {
                CReturnNumOfPlayingTracks cmd ( m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(buf);
                m_NumberOfPlayingTracksReceivedFromIPod = cmd.getPlayListCount();
                //LOG_TRACE<<"CPlayEngineTaskHandler: 0x0036 m_NumberOfPlayingTracks = " <<
                //            m_NumberOfPlayingTracksReceivedFromIPod <<LOG_ENDL;
                emit mainLoopQuit(ExitCode_GetNumOfPlayingTracks);
                break;
            }
                // } Added by pawan 2013.05.06.
                // } Added by pawan 2013.05.04.
            case 0x0027:
            {
                //Fixed: CR15102, CR16762: dont do anything as its taboff
                /*Tan: 2013.06.24 Commenting this out.
                Even if the tab is off, you should still emit to AVP
                They have to take care of that.
                */

                //if(m_iPodPrivateData->m_IsTabOff && !m_iPodPrivateData->m_IsCurrentPlayingMP)
                CPlayStatusChangeNotification cmd( m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(buf);
                m_PlayStatusNotifyState = eStateNotifyReceived;
                int notificationType = cmd.getNotificationType();

                bool isPlaybackRelatedNotification = ((notificationType == 0x00) ||(notificationType == 0x06));
                if(m_iPodPrivateData->m_bIsAhaPandoraMode && isPlaybackRelatedNotification)
                {
                    CPlayStatusChangeNotification cmd( m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    int notificationType = cmd.getNotificationType();
                    //LOG_TRACE << "CPlayEngineTaskHandler packetAvailableits tab off returing notificationType: " << notificationType << LOG_ENDL;
                    return;
                }

                switch(notificationType)
                {
                case 3: // REW stop
                {
                    //LOG_INFO << "CPlayEngineTaskHanlder: REW seek stop m_playControlStopSeekFlag = " << m_playControlStopSeekFlag <<LOG_ENDL;
                    if(!m_playControlStopSeekFlag)
                    {
                        m_RepeatMode = REPEAT_FOLDER;
                        //((CIpodController *) parent())->RepeatPlay(REPEATALL, 0);
                        //Do not remove playControlE("StopSeek"), because playControlE("StopSeek") in needed after song change via FF/REW.
                        playControlE("StopSeek");
                        //playControlE("pause");
                        // { added by jungae, 2013.01.16 for ITS154399,154398, 154400, 154397
                        // After pause for all NonUID devices, Setplaystatus change notification is not sent.
                        // So, sent the OnStateChanged signal(Playing status) after stop seek mode.
                        // it can changed the play icon from seek_icon to play_icon on REW
                        emit OnStateChanged(10);
                        // } added by jungae
                        ((CIpodController *) (parent()))->OnCurrentMediaStatusChangedSlot();
                    }
                    else
                    {
                        //LOG_INFO << " case 3 else " <<LOG_ENDL;
                        m_playControlStopSeekFlag = false;

                    }
                    break;

                }
                //seek stop : 	// Added to fix FF End functionality for classic devices
                case 2:
                {
                    //LOG_INFO << "CPlayEngineTaskHanlder: FF seek stop m_playControlStopSeekFlag = " << m_playControlStopSeekFlag <<LOG_ENDL;
                    if(!m_playControlStopSeekFlag)
                    {
                        m_RepeatMode = REPEAT_FOLDER;
                        //LOG_TRACE << "devplay setting FF next song bool true" << LOG_ENDL;
                        //((CIpodController *) parent())->RepeatPlay(REPEATALL, 0);
                        //Do not remove playControlE("StopSeek"), because playControlE("StopSeek") in needed after song change via FF/REW.
                        playControlE("StopSeek");
                        ((CIpodController *) (parent()))->OnCurrentMediaStatusChangedSlot();
                    }
                    else
                    {
                        m_PlayNextSongOnFF = false;
                        //LOG_INFO << " case 2 else " <<LOG_ENDL;
                        m_playControlStopSeekFlag = false;
                    }
                    break;
                }
                // track index change
                case 1:
                {
                    ((CIpodController*)parent())->m_nScanStartPosition = -1;
                    if(m_IsPlayListSpecificCategoryIsSet)
                    {
                        //LOG_TRACE << "PLAYLIST: going to play a new song so dont set the index" << LOG_ENDL;
                        return;
                    }
                    LOGH "getNotificationValue: " << cmd.getNotificationValue() << ", m_IsRestartOfMPInProgress: " << m_iPodPrivateData->m_IsRestartOfMPInProgress << LOG_ENDL;

                    if(!m_iPodPrivateData->m_IsCurrentPlayingMP && !m_iPodPrivateData->m_IsRestartOfMPInProgress)
                    {
                        executeOtherMPIndexChangeEvent(cmd);
                    }
                    else
                    {
                        executeTrackIndexChangeEvent(cmd);
                    }
                    break;
                }
                // { Added by pawan 2013.05.04.
                case 0x0A:
                {
                    //LOG_TRACE << "got the UID change notification index: " << cmd.getUID() << LOG_ENDL;
                    executeTrackUIDChangeEvent(cmd);
                    break;
                }
                case 0x0E:
                {
                    executePlaylistChangeEvent(cmd);
                    break;
                }
                //position change in seconds
                case 7:
                {
                    if(!m_iPodPrivateData->m_IsTabOff)
                        executePositionChangeEvent(cmd, 7);
                    break;
                }
                case 0:
                {
                    m_playState = ePlayState_STOP; //stop state
                    LOG_HIGH<<"STOPPED Playback Stopped!"<<LOG_ENDL;
                    handlePlaybackStopped();
                    /*
                    m_playState = ePlayState_STOP;
                    //executeStateChangeEvent(cmd.getNotificationValue());
                    */
                    break;
                }
                case 6:
                {
                    switch(cmd.getNotificationType())
                    {
                    case 0x02:
                        m_playState = ePlayState_STOP; //stop state
                        LOG_HIGH<<"STOPPED Playback Stopped!"<<LOG_ENDL;
                        handlePlaybackStopped();
                        break;
                    case 0x0A:
                        m_playState = ePlayState_PLAY; //play state
                        m_playStateNotificationFromIpod = ePlayState_PLAY;
                        emit OnStateChanged(cmd.getNotificationValue());
                        break;
                    case 0x0B:
                        m_playState = ePlayState_PAUSE; //pause state
                        m_playStateNotificationFromIpod = ePlayState_PAUSE;
                        emit OnStateChanged(cmd.getNotificationValue());
                        break;
                    default:
                        emit OnStateChanged(cmd.getNotificationValue());
                        break;
                    }

                    break;
                }
                case 4:
                {
                    if(!m_iPodPrivateData->m_IsTabOff)
                        executePositionChangeEvent(cmd, 4);
                    break;
                }
                default:
                    break;
                }
                break;
            }
            //ReturnIndexedTrackInfo
            case 0x000D:
            {
                CReturnIndexedPlayingTrackInfo cmd(m_iPodPrivateData->m_isIDPS);
                cmd.unmarshall(buf);
                if(cmd.getInfoType() == 0x00)
                {
                    m_nIPodReturnedMediaDuration = cmd.getTrackDuration();
                    emit mainLoopQuit(ExitCode_GetIndexedPlayingTrackDuration);
                }
                break;
            }
            //RetPlayStatus
            case 0x001D:
            {
                if(m_emitStateForNONIDPS == true)
                {
                    int nState = buf[13];
                    //LOG_INFO << "Stopped=0;Playing=1,Paused=2 : Current State is " << nState << LOG_ENDL;
                    m_emitStateForNONIDPS = false;
                    //LOG_INFO << "CPlayEngineTaskHandler::RetPlayStatus IDPS Stopped=0;Playing=1,Paused=2 : Current State is " << nState << LOG_ENDL;

                    switch(nState)
                    {
                    case PLAYING:
                    {
                        emit OnStateChanged(MPLAYING);
                        break;
                    }
                    case PAUSED:
                    {
                        emit OnStateChanged(MPAUSED);
                        break;
                    }
                    case MSTOPPED:
                    {
                        emit OnStateChanged(MSTOPPED);
                        break;
                    }
                    }
                }
                else
                {
                    m_CommandBuffer.clear();
                    m_CommandBuffer = buf;
                    emit mainLoopQuit(ExitCode_GetPlayStatus);
                }
                break;
            }
            //ReturnCurrnetPlayingTrackIndex
            case 0x001F:
            {
                CReturnCurrentPlayingTrackIndex cmd( m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(buf);
                m_AutoplayIndex = cmd.getCurrentIndex();
                m_CurrentTrackIndexReceived = m_AutoplayIndex;
                //If an index value is returned it means some trakc is paused.hence return true;
                if(m_AutoplayIndex != -1){
                    m_CommandStatus = true;
                }
                else
                {
                    m_AutoplayIndex = 0;
                    m_CommandStatus = false;
                }
                emit mainLoopQuit(ExitCode_GetCurrentPlayingTrackIndex);
                break;
            }
            //ReturnIndexedPlayingTrackTitle
            case 0x0021:
            {
                CReturnIndexedPlayingTrackTitle cmd( m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(buf);
                m_TuneMediaInfo.Title = cmd.getTitle();
                emit mainLoopQuit(ExitCode_GetIndexedPlayingTrackTitle);
                break;
            }
            //ReturnIndexedPlayingTrackArtistName
            case 0x0023:
            {
                CReturnIndexedPlayingTrackArtistName cmd( m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(buf);
                m_TuneMediaInfo.Artist = cmd.getArtistName();
                emit mainLoopQuit(ExitCode_GetIndexedPlayingTrackArtistName);
                break;
            }
            //ReturnIndexedPlayingTrackAlbumName
            case 0x0025:
            {
                CReturnIndexedPlayingTrackAlbumName cmd( m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(buf);
                m_TuneMediaInfo.Album = cmd.getAlbumName();
                emit mainLoopQuit(ExitCode_GetIndexedPlayingTrackAlbumName);
                break;
            }
            default:
                break;
            }
        }
        // { Added by pawan 2013.05.15.
        else if(lingoID == 0x00)
        {
            switch(commandID)
            {
            case CIAPCmds::L0_00_RequestIdentify :
            {
                //LOG_HIGH << "[WARNING] PE H/U Receviced General Lingo Command Request Identify ( 0x00 )!!!!!!!" << LOG_ENDL;
                //LOG_HIGH << "          So, we will do usb port resetting" << LOG_ENDL;
                CRequestIdentify recvCmd(m_iPodPrivateData->getTransactionId(false),m_iPodPrivateData->m_isIDPS);
                recvCmd.unmarshall(buf);

                if( m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_1 || m_iPodPrivateData->m_currentPortNum == eIPOD_PORT_2 )
                {
                    LOG_HIGH << "L0_00_RequestIdentify, emit reconnection() 2"  << LOG_ENDL;
                    ((CIpodController *)parent())->CheckIpodConnectionStatus();
                    ((CIpodController *)parent())->restartUcs();
                    return;
                }
            }
                break;
            case 0x4A:
            {
                if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
                {
                    //LOG_TRACE << "ios6 Playenegine got ipod notification and this is converted device" << LOG_ENDL;
                    CiPodNotification cmd(m_iPodPrivateData->m_isIDPS);
                    cmd.unmarshall(buf);
                    if(cmd.isDBChanged())
                    {
                        //LOG_TRACE << "ios6 db is changed" <<LOG_ENDL;
                        m_iPodDBIndexFetchTaskHandler->resetPreviousSelectionsE();
                        m_newNumberOfTracks = m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE("Track");
                        LOG_HIGH << "m_newNumberOfTracks: "<< m_newNumberOfTracks<< "  m_TotalIpodTrackCount(before): "<< m_iPodPrivateData->m_TotalIpodTrackCount<<LOG_ENDL;
                        if (m_newNumberOfTracks != m_iPodPrivateData->m_TotalIpodTrackCount)
                        {
                            //LOG_HIGH << "trackNumberChanged. emit dbChangedAfterDeletion"<<LOG_ENDL;
                            m_iPodPrivateData->m_TotalIpodTrackCount = m_newNumberOfTracks;
                            emit dbChangedAfterDeletion();
                        }
                    }
                    else
                    {
                        //LOG_TRACE << "ios6 db is not changed" <<LOG_ENDL;
                    }
                }
                break;
            }
            }
        }
        // } Added by pawan 2013.05.15.
        // Added by sungha.choi   2013.04.11.
        else if(lingoID == 0x03) // Display Remote Lingo
        {
            qDebug() << "Display Remote Lingo, commandID = " << commandID;
            switch(commandID)
            {
            case 0x00:  // ACK
            {
                CACK cmd( m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(buf);
                int ackCmd= cmd.getAcknowledgedCommand();
                //LOG_INFO << " Ack status of " << ackCmd << " is " << cmd.getAckStatus() <<LOG_ENDL;
                switch(ackCmd)
                {
                //0x0E - SetiPodStateInfo
                case 0x0E:
                {
                    // get the status and do the needful
                    m_CommandStatus = !cmd.getAckStatus();
                    emit mainLoopQuit(ExitCode_SetiPodStateInfo);
                    break;
                }
                default:
                    emit mainLoopQuit(ExitCode_default);
                    break;
                }
                break;
            }
            // Added by jonghwan.cho@lge.com    2013.04.16.
            // for iOS 6.1
            case 0x09: {  // RemoteEventNotification


                CRemoteEventNotification cmd( m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(buf);
                m_EventNotifyState = eEventNotifyReceived;

                //LOG_INFO << "shuffle RemoteEventNotification NotificationType() = " << cmd.GetNotificationType() << LOG_ENDL;
                //LOG_INFO << "shuffle RemoteEventNotification NotificationValue() = " << cmd.GetNotificationValue() << LOG_ENDL;

                int notificationType;
                int notificationValue;

                notificationType = cmd.GetNotificationType();
                notificationValue = cmd.GetNotificationValue();

                bool isPlaybackRelatedNotification = (notificationType == 0x03);

                if(m_iPodPrivateData->m_bIsAhaPandoraMode && isPlaybackRelatedNotification)
                {
                    //LOG_TRACE<<"its a taboff got remote event notification in play engine so returning" <<LOG_ENDL;
                    return;
                }


                switch( notificationType ) {
                case 0x03 : { // Play Status ( 1 Byte )
                    LOG_HIGH << "(STOP:0, PLAY:1, PAUSE:2, FF:3, REW:4) StatusNoti = "<< notificationValue << LOG_ENDL;
                    switch( notificationValue ) {
                    case ePlayState_STOP : // STOP
                        m_playState = ePlayState_STOP; //stop state
                        LOG_HIGH<<"STOPPED Playback Stopped!"<<LOG_ENDL;
                        handlePlaybackStopped();
                        /*
                        m_playState = ePlayState_STOP;
                        */
                        break;
                    case ePlayState_PLAY : // PLAY
                        m_playState = ePlayState_PLAY;
                        m_playStateNotificationFromIpod = ePlayState_PLAY;
                        emit OnStateChanged(0x0A);
                        if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
                        {
                            delayMetadataIndexQuery();
                        }
                        break;
                    case ePlayState_PAUSE : // PAUSED
                        m_playState = ePlayState_PAUSE;
                        m_playStateNotificationFromIpod = ePlayState_PAUSE;
                        emit OnStateChanged(0x0B);
                        break;
                    case ePlayState_FF : // FF
                        break;
                    case ePlayState_REW : // REW
                        break;
                    default:
                        break;
                    }
                } // case 0x03, Play Status
                    break;
                case 0x07 :  { // Shuffle Status ( 1 Byte )
                    int oldState = m_RandomMode;
                    if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
                    {
                        delayMetadataIndexQuery();
                    }
                    switch( notificationValue ) {
                    case eShuffleState_Disabled : // Disable
                        emit OnModeChanged(RANDOMOFF);
                        m_RandomMode = RANDOM_OFF;
                        break;
                    case eShuffleState_Enabled : // Enable
                        // Added by jonghwan.cho@lge.com 2013.10.29.
                        if( m_RandomMode != REPEAT_FOLDER )
                            ((CIpodController *)parent())->RepeatPlay( REPEATALL );
                        emit OnModeChanged(RANDOMFOLDER);
                        m_RandomMode = RANDOM_FOLDER;
                        break;
                    default :
                        break;
                    }
                    //Get the curretn playing index as sometime
                    //IPod does not send the current playing index
                    //when shuffle state changes
                    if(!isPlayListSet() && m_RandomMode != oldState)
                    {
                        //LOG_TRACE << "devplay got notification for shuffle and playlist is not set" << LOG_ENDL;
                        /*int index=0;
                        if(getCurrentPlayingTrackIndex(index))
                        {
                            m_CurrentIndex = index;
                        }*/
                    }
                    else
                    {
                        //LOG_TRACE << "devplay got notification for shuffle and playlist is set" << LOG_ENDL;
                    }
                } // case 0x07, Shuffle Status
                    break;
                case 0x08 :  { // Repeat Status  ( 1 Byte )
                    switch( notificationValue ) {
                    case eRepeatState_NONE : // NONE
                        ((CIpodController *)parent())->RepeatPlay( REPEATALL );
                        break;
                    case eRepeatState_ONE :  // Enable
                        // Added by jonghwan.cho@lge.com 2013.10.30.
                        if( m_RandomMode != RANDOM_OFF )
                            ((CIpodController *)parent())->RepeatPlay( REPEATALL );
                        else
                        {
                            emit OnModeChanged(REPEATFILE);
                            m_RepeatMode = REPEAT_FILE;  // Modified by pawan 2013.05.28.
                        }
                        break;
                    case eRepeatState_ALL : // Enable
                        emit OnModeChanged(REPEATALL);
                        m_RepeatMode = REPEAT_FOLDER;
                        break;
                    default :
                        break;
                    }
                } // case 0x08, Repeat Status
                    break;
                case 0x0E :  // Audiobok playback speed ( 1Byte )
                    // temporory empty state
                    break;
                case 0x00 :  // Track Position      ( 4 Byte )
                case 0x01 :  // Track Index         ( 4 Byte )
                case 0x02 :  // Chapter Information ( 8 Byte )
                case 0x04 :  // Mute/UI Volume     ( 2 Byte )
                case 0x05 :  // Power/Battery      ( 2 Byte )
                case 0x06 :  // Equalize state       ( 4 Byte )
                case 0x09 :  // Date/Time           ( 6 Byte )
                case 0x0A :  // Alarm               ( 3 Byte )
                case 0x0B :  // Back Light           ( 1 Byte )
                case 0x0C :  // Hold Switch          ( 1 Byte )
                case 0x0D :  // Sound Check         ( 1 Byte )
                case 0x0F :  // Track Position in Second ( 2 Byte )
                case 0x10 :  // Mute / UI / Absolute Volume ( 3 Byte )
                case 0x11 :  // Track capabilitites     ( 4 Byte )
                case 0x12 :  // Playback engine change  ( 4 Byte )
                    // number of tracks in new playlist
                default :
                    //LOG_TRACE << "CRemoteEventNotification::unmarshell: Unsupport not yet = " << notificationType << LOG_ENDL;
                    break;
                }
                // Added by jonghwan.cho@lge.com    2013.04.16.
                // for iOS 6.1
                m_EventNotifyState = eEventNotifyDefault;
            }
                break;
            default:
                break;
            }
        }
        else if(lingoID == 0x0A) // for Digital Audio Lingo
        {
            switch(commandID)
            {
            case 0x01:
                break;
            case 0x04:
            {
                CTrackNewAudioAttributes cmd(0,  m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(buf);
                m_LocalTransId = cmd.getTransactionId();
                m_SampleRate = cmd.GetSampleRate();
                AcknoledgeTheIPod(0x04,0);
                //LOG_INFO << "GST::CPlayEngineTaskHandler++: Track changed: the sample_rate = "<<m_SampleRate<< LOG_ENDL;
                if( m_iPodPrivateData->m_IsTabOff == false )
                {
                    m_bPipeCreated = false;
                    startPipeline();
                }
                //to be used by Aha.
                m_iPodPrivateData->updateSampleRateFile(m_SampleRate);
            }
                break;
            } //end of switch
        }
        else
        {
#ifdef DBUGLOGS
            //LOG_TRACE<<"CPlayEngineTaskHandler: Not my lingo...ignoring!"<<LOG_ENDL;
#endif
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CPlayEngineTaskHandler::requestData() Execption Caught *******" << LOG_ENDL;
    }
    return;
}

void CPlayEngineTaskHandler::executeStateChangeEvent(int state)
{
    //LOG_INFO<< "CPlayEngineTaskHandler::executeStateChangeEvent : State in play engine is " << state << LOG_ENDL;
//    if(state == 0x00)
//    {
//        //LOG_INFO<< "CPlayEngineTaskHandler::executeStateChangeEvent : setting the members to handle the stooped state of IPod" << LOG_ENDL;
//        m_PreparePlaylistStatus = false;
//        m_IsPlaylistSetByDH = true;

//        m_CurrentPlayingUID = 0;
//        m_CurrentIndex = 0;
//        m_CurrentPlaylistIndex = 0;
//        m_ControllerPlaylist.clear();
//        m_ControllerPlaylistType.clear();
//        m_ControllerPlayListUID.clear();
//        m_CategoryType = DHTrackCategory;
//        m_iPodDBIndexFetchTaskHandler->resetPreviousSelectionsE();
//        m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE("Track");

//        m_iPodPrivateData->m_IsRestartOfMPInProgress = true;
//        playE();
//        m_iPodPrivateData->m_IsRestartOfMPInProgress = false;
//    }
    emit OnStateChanged(state);
}

void CPlayEngineTaskHandler::executePositionChangeEvent(CPlayStatusChangeNotification& cmd, int caseID)
{
    //if position change event comes, it means it is not STOPPED.
    m_HandleStopTimer->stop();

    if(!m_iPodPrivateData->m_IsCurrentPlayingMP)
    {
        return;
    }
    if(++m_FirstPositionChangeCount == 1 )
    {
        //LOG_INFO<< "m_FirstPositionChangeCount " << m_FirstPositionChangeCount << LOG_ENDL;
        int milliseconds = m_iPodPrivateData->m_AutoPlayTime.elapsed();
        QString performanceTime = m_iPodPrivateData->getPerformanceTime( milliseconds );
        PERFORMANCELOG ( "IPod AutoPlay time is %s " , performanceTime.toAscii().data()  );
    }

    m_CurrentMediaPosition = cmd.getNotificationValue();

    switch(caseID)
    {
    case 7:
        m_CurrentMediaPosition = m_CurrentMediaPosition*1000;// converted to milliseconds
        break;
    default:
        break;
    }

    //Sometimes FF m_CurrentMediaPosition is greater than m_CurrentMediaDuration
    //So, that case we do not emit emit OnPositionChanged
    if(m_CurrentMediaDuration >= m_CurrentMediaPosition)
    {
        emit OnPositionChanged(m_CurrentMediaPosition);
    }

    //LOG_TRACE << "case : " << caseID  <<" position is: " << m_CurrentMediaPosition << "  duration is: " << m_CurrentMediaDuration << LOG_ENDL;
    bool endOfSong = (m_CurrentMediaPosition < m_CurrentMediaDuration) && ((m_CurrentMediaPosition + 2000) >= m_CurrentMediaDuration );
    if(isPlayListSet() && endOfSong)
    {
        //LOG_HIGH << "devplay song is ended so play next song: " << m_RepeatMode << LOG_ENDL;
        if(m_RepeatMode == REPEAT_FILE)
        {
            return;
        }
        if(m_ControllerPlaylist.count() == 1)
        {
            m_IsPlaylistSetByDH = true;
        }
        int index = getNextIndex();
        setCurrentIndexE(index);
        playControlE("Pause");
        playE();
        return;
    }
    if(((CIpodController *) parent())->m_PlayState == 4) { // REWIND
        if(m_CurrentMediaPosition == 0 )
        {
            int index = getCurrentIndex();
            setCurrentIndexE(index);
            playE();
        }
    }
}

void CPlayEngineTaskHandler::executePlaylistChangeEvent(CPlayStatusChangeNotification& cmd)
{
    m_NumberOfPlayingTracksReceivedFromIPod = cmd.getNotificationValue();
    m_iPodPrivateData->m_FirstPlayListSet = false;
    m_ControllerPlaylist.clear();
//    m_ControllerPlayListUID.clear();
    m_NumberOfPlayingTracks = cmd.getNotificationValue();
    if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
    {
        delayMetadataIndexQuery(2000);
    }
    else
    {
        emit OnCurrentPlayingTrackCount(cmd.getNotificationValue());
    }
    //LOG_TRACE << "devplay got the playlistchange notification: " << cmd.getNotificationValue() << LOG_ENDL;
}

void CPlayEngineTaskHandler::executeTrackUIDChangeEvent(CPlayStatusChangeNotification& cmd)
{
    if(!m_iPodPrivateData->m_IsCurrentPlayingMP && cmd.getUID() <= 0)
    {
        //LOG_TRACE <<"IOSAPP: devplay executeTrackUIDChangeEvent, other music app is playign with UID: " << cmd.getUID() << "so return" << LOG_ENDL;
        m_iPodPrivateData->m_FirstPlayListSet = false;
        m_ControllerPlaylist.clear();
        return;
    }
    m_PlayNextSongOnFF = false;
    quint64 uidval = cmd.getUID();
    if(uidval > 0)
    {
        //LOG_TRACE <<"executeTrackUIDChangeEvent m_CurrentPlayingUID " << cmd.getUID() << LOG_ENDL;
        m_CurrentPlayingUID = uidval;
    }
    //Tan, the track is updated using GetPBTrackInfo
    //This is because iTunesRadio in IOS7 will not gives us UID even when it is
    //index device.

    ////LOG_TRACE << "devplay 0x003F get meat from device" << LOG_ENDL;
    //getCurrentUIDTrackInfoAndUpdateHMI();
}


void CPlayEngineTaskHandler::getCurrentUIDTrackInfoAndUpdateHMI()
{
    //LOG_TRACE << "IOSAPP: CPlayEngineTaskHandler::getCurrentUIDTrackInfoAndUpdateHMI() CurrentPlayingUID: " << m_CurrentPlayingUID << LOG_ENDL;
    if(m_CurrentPlayingUID <= 0)
    {
        LOG_HIGH<<"CurrentPlayingUID not available"<<LOG_ENDL;
        m_CurrentIndexToPlay = 0;
        m_iPodDBIndexFetchTaskHandler->playCommandCall(m_CurrentIndexToPlay);
        return;
    }
    CGetUIDTrackInfo cmd(m_CurrentPlayingUID, 0x4E, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());
}

void CPlayEngineTaskHandler::responseToTuneDialRequest(int index,
                                                       ETrackerAbstarctor_Query_Ids queryId,
                                                       uint reqUID)
{
    QStringList item;
    QVector<QStringList> dataList;
    item.append(m_TuneMediaInfo.Title);
    item.append(m_TuneMediaInfo.Artist);
    item.append(m_TuneMediaInfo.Album);
    item.append(m_TuneMediaInfo.fileName);
    dataList.append(item);
    //LOG_TRACE << "emitting req complete 0x0043: index: " << m_PBRequestInProgress.nRequestIndex <<LOG_ENDL;
    emit RequestComplete( dataList, queryId, index, reqUID);
}

void CPlayEngineTaskHandler::updateCurrentIndexTrackInfoAndUpdateHMI(int index)
{
    m_iPodPrivateData->setCurrentMediaInfo(m_TuneMediaInfo);
    m_NumberOfPlayingTracks = getNumberOfPlayingTracks(); //Should be needed
    LOGH " index/number = " << index +1 << "/" << m_NumberOfPlayingTracks << LOG_ENDL;
    //LOG_TRACE <<"the file name is: " << m_TuneMediaInfo.fileName << LOG_ENDL;
    emit OnTrackChanged(index);
    emit OnCurrentPlayingTrackCount(m_NumberOfPlayingTracks); //Move

    //Suryanto Tan. 2013.10.19 : shuffle shows blinking.
    //Emit using ipodPrivateData->GetCurrentMediaInfo, because
    //Inside there, we ensure that the cover art is not updated to blank screen
    //emit OnCurrentMediaInfo(&m_TuneMediaInfo);

    if(!m_TuneMediaInfo.fileName.contains("file://"))
    {
        m_TuneMediaInfo.fileName.prepend("file://");
    }

    m_CurrentIndexEmitted = -1;
    m_TuneMetaDataArray.clear();
    m_iPodPrivateData->setCurrentMediaInfo(m_TuneMediaInfo);

    //Suryanto Tan. 2013.10.19 : shuffle shows blinking.
    MediaInfo mediaInfoToEmit;
    getCurrentMediaInfo(&mediaInfoToEmit);
    emit OnCurrentMediaInfo(&mediaInfoToEmit);

    //CurrentUID is 0 and converted device
    if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex && m_PBRetrievedUID == 0)
    {
        emit fetchCoversForBasicPlayer(m_TuneMediaInfo.Album, m_TuneMediaInfo.fileName, true);  // Modified by pawan 2013.05.16.
    }
    else
    {
        emit fetchCoversForBasicPlayer(m_TuneMediaInfo.Album, m_TuneMediaInfo.fileName);  // Modified by pawan 2013.05.16.
    }

    m_MediaInfo.Title.clear();
    m_MediaInfo.Title.append(m_TuneMediaInfo.Title);
    m_MediaInfo.Artist.clear();
    m_MediaInfo.Artist.append(m_TuneMediaInfo.Artist);
    m_MediaInfo.Album.clear();
    m_MediaInfo.Album.append(m_TuneMediaInfo.Album);
    m_MediaInfo.fileName.clear();
    m_MediaInfo.fileName.append(m_TuneMediaInfo.fileName);
}

void CPlayEngineTaskHandler::executeOtherMPIndexChangeEvent(CPlayStatusChangeNotification& cmd)
{
    //LOG_TRACE <<"IOSAPP: its Index change for other MP index: " << cmd.getNotificationValue()
    //          << " and : m_iPodPrivateData->m_IsRestartOfMPInProgress"
    //          << m_iPodPrivateData->m_IsRestartOfMPInProgress << LOG_ENDL;
    m_CurrentIndex = cmd.getNotificationValue();
    m_ControllerPlaylist.clear();
    m_iPodPrivateData->m_CurrentArtworkRetyCountForOtherMP = 0;
    delayMetadataIndexQuery();
    /*
    GetPBTrackInfoWithQueue(RequestType_MetaDataIndex, m_CurrentIndex, eDefault_Id, 0);
    //getCurrentIndexTrackInfoAndUpdateHMI(m_CurrentIndex);
    */
}

void CPlayEngineTaskHandler::executeTrackIndexChangeEvent(CPlayStatusChangeNotification& cmd)
{
    //LOG_TRACE << "IOSAPP devplay track index change FOR MP received index is: " << cmd.getNotificationValue() << LOG_ENDL;

    if(m_PlayNextSongOnFF && !m_iPodPrivateData->m_IsConvertedFromUIDToIndex && isPlayListSet())
    {
        int nNotificationIndex = cmd.getNotificationValue();
        //LOG_TRACE << "devplay playing next song as FF reach till end in INDEX case" << LOG_ENDL;
        if(nNotificationIndex != m_CurrentIndex)
        {
            m_PlayNextSongOnFF = false;
            int index = getNextIndex();
            setCurrentIndexE(index);
            playE();
            //LOGH " m_PlayNextSongOnFF && !m_iPodPrivateData->m_IsConvertedFromUIDToIndex && isPlayListSet() index = " << index << LOG_ENDL;
            emit OnTrackChanged(index);
        }
        return;
    }
    else if(m_PlayNextSongOnFF && isPlayListSet())
    {
        //LOG_TRACE << "devplay returning as this is FF next song case with converted device" << LOG_ENDL;
        return;
    }

    if(isPlayListSet())
    {
        //LOG_TRACE << "devplay playlist is set and m_JumpInProgress:" << m_JumpInProgress << "  m_CurrentIndex:" << m_CurrentIndex << "  cmd.getNotificationValue():" << cmd.getNotificationValue() << LOG_ENDL;
        if(m_CurrentIndex != cmd.getNotificationValue() && !m_JumpInProgress && m_iPodPrivateData->m_QueueData.m_NextLevel <=0)
        {
            //LOG_TRACE << "devplay not equal index" <<LOG_ENDL;

            ((CIpodController *) (parent()))->OnCurrentMediaStatusChangedSlot();

            m_CurrentIndex = cmd.getNotificationValue();

            if(m_ControllerPlaylist.count() > 0 && m_ControllerPlaylist.contains(m_CurrentIndex))
            {
                m_CurrentPlaylistIndex = m_ControllerPlaylist.indexOf(m_CurrentIndex);
            }
        }
        else
        {
            //LOG_TRACE << "devplay index are equal" << LOG_ENDL;
        }
        MediaInfo currentMediaInfo;
        emitCurrentMediaInfoE(&currentMediaInfo);
        isNotPassOnToNextSong = false;
    }
    else
    {
        //LOG_TRACE <<"executeTrackIndexChangeEvent Setting m_CurrentIndex to "<< cmd.getNotificationValue()<<LOG_ENDL;
        m_CurrentIndex = cmd.getNotificationValue();
        if(!m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
        {           
            //LOGH "isPlayListSet() == false && m_iPodPrivateData->m_IsConvertedFromUIDToIndex == false, m_CurrentIndex = " << m_CurrentIndex << LOG_ENDL;
            emit OnTrackChanged(m_CurrentIndex); //move duplicate OnTrackChanged
            getOnDemandMetaInfoForIndexDevice(m_CurrentIndex);
        }
        else
        {
            delayMetadataIndexQuery();
            //getCurrentIndexTrackInfoAndUpdateHMI(m_CurrentIndex);
            //emit OnCurrentPlayingTrackCount(m_NumberOfPlayingTracks);
        }
    }

    if(!m_ControllerPlaylist.isEmpty())
    {
        LOGH " index/number = " << m_CurrentPlaylistIndex +1 << "/" << m_NumberOfPlayingTracks << LOG_ENDL;
        emit OnTrackChanged(m_CurrentPlaylistIndex);
        emit OnCurrentPlayingTrackCount(m_NumberOfPlayingTracks);  // Added by pawan 2013.05.03.
    }
    //LOG_INFO<<"Track Index changed.current index = " << m_CurrentIndex << "[" << m_CurrentPlaylistIndex <<"]"<<LOG_ENDL;
    m_isDurationRequested = true;
}

/******************************* Audio Routing related APIs ***************************************/
void CPlayEngineTaskHandler::setPcmSource(QString source)
{
    //LOG_INFO<< "CPlayEngineTaskHandler++: setPcmSource: path= " << source << LOG_ENDL;
    if(10 != source.length()){
        return;
    }
    if( m_audioConnected)//Already connected, return.
        return;
    if(!m_audioSource.isEmpty()){
        m_audioSource.clear();
    }
    m_audioSource.append(source);
    //LOG_INFO<< "CPlayEngineTaskHandler++: setPcmSource: m_audioSource set to= " << m_audioSource.toAscii().data() << LOG_ENDL;

    m_audioConnected = true;
    //LOG_TRACE<<"CPlayEngineTaskHandler++::setPcmSource:Done!!!" << LOG_ENDL;
}

void CPlayEngineTaskHandler::AcknoledgeTheIPod(int commandID, int Status)
{

    //LOG_TRACE<<"GST:CPlayEngineTaskHandler++::AcknoledgeTheIPod Entry" << LOG_ENDL;
    CAccAck cmd(m_LocalTransId, commandID, Status, m_iPodPrivateData->m_isIDPS );
    cmd.marshall();

    //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(cmd);
    emit sendiAPPacket(cmd.getCommandBytes());
    //LOG_TRACE<<"GST:CPlayEngineTaskHandler++::AcknoledgeTheIPod Exit" << LOG_ENDL;
}

void CPlayEngineTaskHandler::startPipeline()
{
    //LOG_TRACE<<"CPlayEngineTaskHandler++::StartPipeline Entry" << LOG_ENDL;

    if( m_iPodPrivateData->m_IsTabOff == true )
    {
        //LOG_TRACE << "   m_IsTabOff = true, so return" << LOG_ENDL;
        return;
    }

    if( m_bPipeCreated == false )
    {
        m_audioSource.clear();
        m_audioSource = (parent()->findChild<CDeviceDetector*>(gDetector))->getiPodAudioDevPath();
        //LOG_TRACE << "   m_bPipeCreated flag = " << m_bPipeCreated << ", so we will acquire Pipeline: "
        //          << m_audioSource << LOG_ENDL;
        m_pAudioRouter->AquireAudioSource(m_audioSource, QString("%1").arg(m_SampleRate), m_eCurrentSrcType );
        m_bPipeCreated = true;
    }
    else
    {
        //LOG_TRACE << "   m_bPipeCreated flag = " << m_bPipeCreated << ", so we won't acquire Pipeline" << LOG_ENDL;
    }

    //LOG_TRACE<<"CPlayEngineTaskHandler++::StartPipeline Exit" << LOG_ENDL;
}

int CPlayEngineTaskHandler::playNext()
{
    CPlayControl playCmd("Next", m_iPodPrivateData->getTransactionId(),  m_iPodPrivateData->m_isIDPS,false ); //TODO place the rite trans id
    playCmd.marshall();
    emit sendiAPPacket(playCmd.getCommandBytes());
    return waitForResponceFromExitCode(7000, ExitCode_PlayControl);
}

void CPlayEngineTaskHandler::stopPipe()
{
    //LOG_TRACE<<"GST::CPlayEngineTaskHandler++::stopPipe IN" << LOG_ENDL;
    m_pAudioRouter->ReleaseAudioSource(m_eCurrentSrcType);
    m_bPipeCreated = false;
    //LOG_TRACE<<"GST::CPlayEngineTaskHandler++::stopPipe Exit" << LOG_ENDL;
}

void CPlayEngineTaskHandler::Notified()
{
    //LOG_TRACE<< "CPlayEngineTaskHandler::Notified() Entry" << LOG_ENDL;
    //LOG_TRACE<< "CPlayEngineTaskHandler::Notified() EXIT" << LOG_ENDL;
}

void CPlayEngineTaskHandler::onCurrentTrackCountChangedSlot(int count)
{
    //LOG_TRACE << "playengine::onCurrentTrackCountChangedSlot: " << count << LOG_ENDL;

    if(!m_iPodPrivateData->m_FirstPlayListSet && !m_iPodPrivateData->m_IsConvertedFromUIDToIndex && !m_CategoryType.compare(DHTrackCategory))
    {
        //LOG_TRACE << "playengine::onCurrentTrackCountChangedSlot: " << count << LOG_ENDL;
        m_ControllerPlaylist.clear();
        for(int i=0; i< count; i++)
        {
         m_ControllerPlaylist.append(i);
        }
    }

    if(m_ControllerPlaylist.count() > 0)
    {
        emit OnCurrentPlayingTrackCount(m_ControllerPlaylist.count());
        m_NumberOfPlayingTracks = m_ControllerPlaylist.count();
        LOGH " index/number = " << m_CurrentIndex +1 << "/" << m_NumberOfPlayingTracks << LOG_ENDL;
        //LOG_TRACE << "m_NumberOfPlayingTracks for playlist: " << m_NumberOfPlayingTracks << LOG_ENDL;
    }
    else if(count > 0 && !m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
    {
        try
        {
            m_NumberOfPlayingTracks = getNumberOfPlayingTracks();
        }
        catch(EIPodErrors ex)
        {
            //LOG_TRACE<< "Throw disconnect from getnumberofplayingtracks" <<LOG_ENDL;
            return;
        }

        //LOG_TRACE << "onCurrentTrackCountChangedSlot: m_NumberOfPlayingTracks: " << m_NumberOfPlayingTracks <<"and m_iPodPrivateData->m_TotalIpodTrackCount: " << m_iPodPrivateData->m_TotalIpodTrackCount << LOG_ENDL;
        if(m_NumberOfPlayingTracks == m_iPodPrivateData->m_TotalIpodTrackCount)
        {
            emit OnCurrentPlayingTrackCount(count);
        }
        else
        {
            m_NumberOfPlayingTracks = count;
        }
        //LOG_TRACE << "m_NumberOfPlayingTracks for non playlist: " << m_NumberOfPlayingTracks << LOG_ENDL;
    }
}

void CPlayEngineTaskHandler::setPlayState()
{
    /* If audio pipe is not runnig, start it */
    ////LOG_TRACE<<"GST::CPlayEngineTaskHandler++::setPlayState IN" << LOG_ENDL;

    if( ((CIpodController *) parent())->m_PlayState == 1/*PLAYING*/ ) {
        //LOG_TRACE << "GST CPlayEngineTaskHandler::setPlayState()  m_PlayState is PLAYER_PLAY, so we will make Pipeline()" << LOG_ENDL;
        startPipeline();
    }
    else
    {
        LOG_HIGH << "GST setPlayState() not PLAYER_PLAY" << LOG_ENDL;
    }

    ////LOG_TRACE<<"GST::CPlayEngineTaskHandler++::setPlayState OUT" << LOG_ENDL;
}
void CPlayEngineTaskHandler::setPauseState()
{
    //LOG_TRACE<<"GST::CPlayEngineTaskHandler++::setPauseState " << LOG_ENDL;
    stopPipe();
}

void CPlayEngineTaskHandler::clearPipeline()
{
    //LOG_TRACE<<"CPlayEngineTaskHandler++::clearPipeline Entry"<<LOG_ENDL;

    m_AuthenticationPassed = false;
    m_audioConnected = false;
    m_toDelete = true;
    stopPipe();
//    doClearPipeline();

    //LOG_TRACE<<"CPlayEngineTaskHandler++::clearPipeline Exit"<<LOG_ENDL;
}

void CPlayEngineTaskHandler::doClearPipeline()
{
    //LOG_TRACE<<"CPlayEngineTaskHandler++::doClearPipeline Entry"<<LOG_ENDL;
//    if(m_pipeCreated){
//        stopPipe();
//    }
//    m_pipeCreated = false;

    //LOG_TRACE<<"CPlayEngineTaskHandler++::doClearPipeline Exit"<<LOG_ENDL;

}

void CPlayEngineTaskHandler::setm_IsSyncInProgress(bool value, int type)
{
    //LOG_TRACE<<"CPlayEngineTaskHandler::setm_IsSyncInProgress Entry "<<LOG_ENDL;
    m_IsSyncInProgress = value;
    m_SyncType = type;
    //LOG_TRACE<<"CPlayEngineTaskHandler::setm_IsSyncInProgress Exit m_IsSyncInProgress = "<<
    //            m_IsSyncInProgress << ", m_SyncType " << m_SyncType << LOG_ENDL;
}

/*
 * Added by jonghwan.cho@lge.com	2013.03.27.
 */
bool CPlayEngineTaskHandler::createConnection(QString str)
{
    //LOG_TRACE << "DBHandleLog CPlayEngineTaskHandler::createConnection IN" << LOG_ENDL;
    bool retVal = false;
    QString dbName=TEMP_DBPATH;
    dbName.append(str);

    //If DB does not exist in /dev/shm use DB from FS
    if(!QFile::exists(dbName))
    {
        // Connection will be created with FS
        dbName.clear();
        dbName.append(PERM_DBPATH);
        dbName.append(str);

        //If DB is not present in both FS and tmpfs then create DB in tmpfs
        if(!QFile::exists(dbName))
        {
            // DB will be created in tmpfs
            dbName.clear();
            dbName.append(TEMP_DBPATH);
            dbName.append(str);
        }
    }

    /* Added by pradeep.honnawad@lge.com    2013.03.07
     * Below block of code is added to check whether the database exists, connection should be established
     * only if the DB file is present, else DB will not be created and Metadata will not appear in HU
     */

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
            if (!dBHandle.open())
            {
                retVal = false;
            }
            else
            {
                retVal = true;
            }
        }
    }
    else
    {
        dBHandle = QSqlDatabase::addDatabase("QSQLITE",str);
        dBHandle.setDatabaseName(dbName);
        if (!dBHandle.open())
        {
            retVal = false;
        }
        else
        {
            retVal = true;
        }
    }
    //LOG_TRACE << "DBHandleLog CPlayEngineTaskHandler::createConnection OUT" << LOG_ENDL;
    return retVal;
}

void CPlayEngineTaskHandler::GetMetadataFromDatabase(int index, QStringList &item)
{
    bool queryResultExist = false;
    QString sSerialNumber = m_iPodPrivateData->getCurrentSerialNumber();
    bool ok = createConnection(sSerialNumber);
    if (ok)
    {
        QSqlQuery query(dBHandle);
        QString queryString;
        queryString.clear();

        queryString = QString("SELECT T_Title,T_Album,T_Artist,T_FilePath from MasterTable where T_index=") + QString::number(index);

        bool bValue = false;
        if( dBHandle.isValid() == true )
            bValue = query.exec(queryString);
        else
            bValue = false;
        if( bValue == true )
        {
            //LOG_TRACE << "query success" << LOG_ENDL;
            while(query.next())
            {
                queryResultExist = true;
                //LOG_TRACE << "query inside while" << LOG_ENDL;
                //LOG_TRACE << "T_Title = "<<query.value(0).toString() << LOG_ENDL;
                //LOG_TRACE << "T_Album = "<<query.value(1).toString() << LOG_ENDL;
                //LOG_TRACE << "T_Artist = "<<query.value(2).toString() << LOG_ENDL;
                item.append(query.value(0).toString()); //T_Title
                item.append(query.value(2).toString()); //T_Artist
                item.append(query.value(1).toString()); //T_Album
                item.append(query.value(3).toString()); //T_FilePath
            }
        }
    }

    if(!queryResultExist)
    {
        item.clear();
        item.append("");
        item.append("");
        item.append("");
        item.append("");
    }
}

bool CPlayEngineTaskHandler::requestData( ETrackerAbstarctor_Query_Ids queryId, uint reqUID, int playlistIndex, int startIndex, int nSize )
{
#ifdef ONDEMANDCATEGORY
     QString CategoryStr = CurrentCategory;
#else
    QString CategoryStr = PLAYLISTCATEGORY;
#endif
    switch(queryId)
    {
    case eGetMusicSimpleMetaDataQuery_Id:
    {
	    // Modified by pawan 2013.05.28.
        if(!m_ControllerPlaylist.isEmpty())
        {
            if(!m_iPodPrivateData->m_IsConvertedFromUIDToIndex && m_iPodPrivateData->m_FirstPlayListSet)
            {
                //LOG_TRACE<<"TuneDial m_FirstPlayListSet and index device"<<LOG_ENDL;
                if(playlistIndex<m_ControllerPlaylist.count())
                {
                    QStringList item;
                    GetMetadataFromDatabase(m_ControllerPlaylist.at(playlistIndex), item);
                    QVector<QStringList> dataList;
                    dataList.append(item);
                    emit RequestComplete( dataList, queryId, playlistIndex, reqUID);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
            {
                return false;
            }
        }
        else
        {
            if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
            {
                //LOG_TRACE <<"sending 0x0043 the PBtrack info command : index: " << playlistIndex << LOG_ENDL;
                //get the data from IPod for these devices.
                //there are no
                if(isAcceptingTuneDial())
                {
                    GetPBTrackInfoWithQueue(RequestType_TuneDial, playlistIndex, queryId, reqUID);
                }
                return true;
            }
            else
            {
                //LOG_TRACE <<"PLAYLIST: get the tune data" << LOG_ENDL;
                getOnDemandMetaInfoForIndexDevice(playlistIndex, false);
                QStringList item;
                item.append(m_TuneMediaInfo.Title);
                item.append(m_TuneMediaInfo.Artist);
                item.append(m_TuneMediaInfo.Album);
                item.append(m_TuneMediaInfo.fileName);
                QVector<QStringList> dataList;
                dataList.append(item);
                //LOG_TRACE << "PLAYLIST: emitting req complete : index: " << playlistIndex <<LOG_ENDL;
                emit RequestComplete( dataList, queryId, playlistIndex, reqUID);
                return true;
            }
        }
        QStringList item;
        GetMetadataFromDatabase(playlistIndex, item);
        QVector<QStringList> dataList;
        dataList.append(item);
        emit RequestComplete( dataList, queryId, playlistIndex, reqUID);
        return true;
    }

    case eGetlistofPlaylistsQuery_Id:
    {
        emit OnChangeDBIndexingStatus(true);
        QVector<QStringList> dataList;
        getlistofPlaylistsQuery(CategoryStr, dataList, startIndex, nSize);

        emit RequestComplete( dataList, queryId, startIndex, reqUID);
        if(!m_CategoryType.compare(DHTrackCategory) || m_iPodPrivateData->m_ExtendedLingoVersion >= 1.13)
            emit OnChangeDBIndexingStatus(false);
        return true;
    }
    case eGetAllSongsbyPlaylistQuery_Id:
    {
        emit OnChangeDBIndexingStatus(true);
        QVector<QStringList> dataList;
        getAllSongsbyPlaylistQuery(CategoryStr, dataList, playlistIndex, startIndex, nSize);
        //wooju.lee@lge.com: iStart +1 is due to disable the playlist of all song
        emit RequestComplete( dataList, queryId, startIndex, reqUID);
        if(!m_CategoryType.compare(DHTrackCategory) || m_iPodPrivateData->m_ExtendedLingoVersion >= 1.13)
            emit OnChangeDBIndexingStatus(false);
        return true;
    }
    default:
        break;
    }
    return false;
}

bool CPlayEngineTaskHandler::getlistofPlaylistsQuery(QString category, QVector<QStringList> &dataList, int startIndex, int size)
{
    //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::getlistofPlaylistsQuery() Enter category: " << category << "  startIndex: " << startIndex << "  size: " << size << LOG_ENDL;
    QMap<int,QString> resultValue;
    int listCount = 0;
    if(startIndex == -1)
        startIndex = 0;
    if(1)
    {
        m_iPodDBIndexFetchTaskHandler->resetPreviousSelectionsE();
        listCount = m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE(category);
        //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::getlistofPlaylistsQuery() m_PlaylistCount = " << listCount << LOG_ENDL;

        if(size==-1)
        {
            size = listCount;
        }

        if(startIndex + size > listCount )
            m_PlaylistCount = listCount -  startIndex;
        else
            m_PlaylistCount = size;

        QStringList itemCount;
        if(!category.compare(PLAYLISTCATEGORY))
            itemCount.append(QString::number(listCount-1));
        else
            itemCount.append(QString::number(listCount));
        dataList.append(itemCount);

        if(m_PlaylistCount > 1)
        {
            m_iPodPrivateData->m_CommandMap.insert(iPodControllerPrivateData::COMMANDKEY_PLAYLIST_LIST, &resultValue );
            try
            {
                CRetrieveCategorizedDatabaseRecords cmd( m_iPodPrivateData->getTransactionId(),  m_iPodPrivateData->m_isIDPS);
                cmd.setParameters(category, startIndex, m_PlaylistCount);
                cmd.marshall();
                emit sendiAPPacket(cmd.getCommandBytes());
                waitForResponceFromExitCode(7000, ExitCode_RetrieveCategorizedDatabaseRecords );
            }
            catch(EIPodErrors ex)
            {
                //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::requestData() Execption Caught *******" << LOG_ENDL;
            }
            m_iPodPrivateData->m_CommandMap.remove(iPodControllerPrivateData::COMMANDKEY_PLAYLIST_LIST);
        }

        if(m_PlaylistCount > 1)
        {
            //wooju.lee@lge.com: idx=1 is due to disable the playlist of all song
            int iStart = startIndex;
            if(!category.compare(PLAYLISTCATEGORY))
                iStart = startIndex+1;
            for(int idx = iStart; idx < startIndex + m_PlaylistCount; idx++)
            {
                QStringList item;
                item.append(resultValue.value(idx));
                dataList.append(item);
            }
        }
    }
    //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::getlistofPlaylistsQuery() Exit" << LOG_ENDL;
    return true;
}

void CPlayEngineTaskHandler::playCurrentSelection(int index)
{
    CPlayCurrentSelection cmd(index, m_iPodPrivateData->getTransactionId(true), m_iPodPrivateData->m_isIDPS);
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());
}

void CPlayEngineTaskHandler::playCategoryTrack(QString category)
{
    bool isNano1Gor2G = (m_iPodPrivateData->m_iPodProductID == NANO_1G_PRODUCT_ID) || (m_iPodPrivateData->m_iPodProductID == NANO_2G_PRODUCT_ID);
    int trackIndex = m_CurrentIndex;
    m_IsPlayListSpecificCategoryIsSet = false;
    //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::playCategoryTrack m_CurrentPlaylistIndex: " << m_CurrentPlaylistIndex+1 <<"and m_CurrentIndex: " << trackIndex <<" and category: " << category << LOG_ENDL;
    int currentPlaylistIndex = m_CurrentPlaylistIndex;
    if(!category.compare(PLAYLISTCATEGORY))
    {
        currentPlaylistIndex = currentPlaylistIndex + 1;
    }
    emit OnChangeDBIndexingStatus(true);
    m_iPodDBIndexFetchTaskHandler->resetPreviousSelectionsE();
    int count = m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE(category);
    //LOG_TRACE << "PLAYLIST: count AT FIRST LEVEL = " << count <<LOG_ENDL;
    m_iPodDBIndexFetchTaskHandler->makeSelectionE(category, currentPlaylistIndex);

    if(!category.compare(PLAYLISTCATEGORY))
    {
        count = m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE("Track");
        //LOG_TRACE << "PLAYLIST: count AT SECOND LEVEL = " << count <<"index goign to be played: " << trackIndex <<LOG_ENDL;
        if(isNano1Gor2G)
        {
            //play using play current selection.
            //LOG_TRACE << "PLAYLIST play SECOND LEVEL using playcurrentselection"<<LOG_ENDL;
            playCurrentSelection(trackIndex);
        }
        else
        {
            //LOG_TRACE << "PLAYLIST play SECOND LEVEL  using selectdbrecord"<<LOG_ENDL;
            m_iPodDBIndexFetchTaskHandler->makeSelectionE("Track", trackIndex);
        }
    }

//    emit OnCurrentPlayingTrackCount(count);
    if(m_iPodPrivateData->m_IsConvertedFromUIDToIndex)
    {
        emit OnChangeDBIndexingStatus(false);
    }
}

bool CPlayEngineTaskHandler::getAllSongsbyPlaylistQuery(QString category, QVector<QStringList> &dataList, int playlistIndex, int& StartIndex, int& size)
{
    //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::getAllSongsbyPlaylistQuery() Enter playlistIndex: " << playlistIndex << "  StartIndex: " << StartIndex << "  size: " << size << LOG_ENDL;
    QMap<int,QString> resultValue;
    int listCount = 0;
    if(!category.compare(PLAYLISTCATEGORY))
    {
        playlistIndex = playlistIndex + 1;
    }
    if(1)
    {
        m_iPodDBIndexFetchTaskHandler->resetPreviousSelectionsE();
        if(category.compare(TRACKCATEGORY))
        {
            m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE(category);
            m_iPodDBIndexFetchTaskHandler->makeSelectionE(category, playlistIndex);
        }
        listCount = m_iPodDBIndexFetchTaskHandler->getCategorizedListCountE("Track");
        //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::getAllSongsbyPlaylistQuery() listCount = " << listCount << LOG_ENDL;

        QStringList itemCount;
        itemCount.append(QString::number(listCount));
        dataList.append(itemCount);

        if(StartIndex == -1)
        {
            StartIndex = 0;
        }
        if(size == -1)
        {
            size = listCount;
        }

        if(StartIndex + size > listCount )
            m_PlaylistCount = listCount -  StartIndex;
        else
            m_PlaylistCount = size;

        if(m_PlaylistCount > 0 && StartIndex < listCount)
        {
            m_iPodPrivateData->m_CommandMap.insert(iPodControllerPrivateData::COMMANDKEY_PLAYLIST_SONG, &resultValue );
            try
            {
                CRetrieveCategorizedDatabaseRecords cmd( m_iPodPrivateData->getTransactionId(),  m_iPodPrivateData->m_isIDPS);
                cmd.setParameters("Track", StartIndex, m_PlaylistCount);
                cmd.marshall();
                emit sendiAPPacket(cmd.getCommandBytes());
                waitForResponceFromExitCode(7000, ExitCode_RetrieveCategorizedDatabaseRecords);
            }
            catch(EIPodErrors ex)
            {
                //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::requestData() Execption Caught *******" << LOG_ENDL;
            }
            m_iPodPrivateData->m_CommandMap.remove(iPodControllerPrivateData::COMMANDKEY_PLAYLIST_SONG);
        }
        else
        {
            m_PlaylistCount = 0;
        }

        if(m_PlaylistCount > 0)
        {
            for(int idx=0; idx < m_PlaylistCount; idx++)
            {
                QStringList item;
                item.append(resultValue.value(idx+StartIndex));
                dataList.append(item);
            }
        }
    }
    //LOG_TRACE << "PLAYLIST: CPlayEngineTaskHandler::getAllSongsbyPlaylistQuery() Exit" << LOG_ENDL;
    return true;
}
