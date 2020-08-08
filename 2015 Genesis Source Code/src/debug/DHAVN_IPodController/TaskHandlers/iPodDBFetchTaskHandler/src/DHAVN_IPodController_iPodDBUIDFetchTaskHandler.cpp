/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  21 Spet 2012                                   *
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
#include <DHAVN_IPodController_iPodDBUIDFetchTaskHandler.h>
#include <DHAVN_IPodController_ack.h>
#include <DHAVN_IPodController_retDBTrackInfo.h>
#include <DHAVN_IPodController_getDBTrackInfo.h>
#include <DHAVN_IPodController_getDBiTunesInfo.h>
#include <DHAVN_IPodController_retDBiTunesInfo.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QDate>
#include <QTime>
#include <DHAVN_EventLoop.h>

using namespace ExtendedLingo;

#define NUM_DBTRACK_INFO 6
#define LISTFETCH_TIME 2000

CIPodDBUIDFetchTaskHandler::CIPodDBUIDFetchTaskHandler(QObject* parent,
                                                       iPodControllerPrivateData* privateData,
                                                       CIAPPacketWriter* iapWriter,
                                                       CIAPPacketReader* iapReader):CIPodDBFetchTaskHandler(parent, privateData, iapWriter, iapReader, false)
{
    switch(privateData->m_currentPortNum){
    case eIPOD_PORT_1:
        LOG_INIT_V2(giPodControllerLogName_1);
        break;
    case eIPOD_PORT_2:
        LOG_INIT_V2(giPodControllerLogName_2);
        break;
    default:
         LOG_INIT_V2(giPodControllerLogName);
    }

    m_GetDBTrackCountForMetaData = 0;

    qMemSet(&m_itunesDBInfo, -1, sizeof(struct SITunesDBInfo));

    connect(m_iAPReader,SIGNAL(packetAvailable(int, int, QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
    connect(this, SIGNAL(sendiAPPacket(QByteArray)) ,m_iAPWriter, SLOT(sendiAPPacket(QByteArray)));
    connect(&m_ListFetchtimer,SIGNAL(timeout()),this,SLOT(getListTimeOut()) );
    m_ListFetchtimer.setSingleShot(true);
    connect(m_iAPReader,SIGNAL(packetAvailableForDBTaskHandler(int,int,QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
}
CIPodDBUIDFetchTaskHandler::~CIPodDBUIDFetchTaskHandler()
{
#if 0   // deleted by sungha.choi 2013.04.22
    if(m_iPodPrivateData)
    {
        m_iPodPrivateData->m_Mutex.unlock();
    }
#endif
    // { added by sungha.choi 2013.04.02
    m_tracksInfoMap.clear();
    // Removed by jonghwan.cho@lge.com 2013.05.06.
    //(&m_tracksInfoMap)->~QMap();
    // } added by sungha.choi 2013.04.02
   //LOG_TRACE << "CIPodDBUIDFetchTaskHandler::~CIPodDBUIDFetchTaskHandler " << LOG_ENDL;
}

void CIPodDBUIDFetchTaskHandler::stopCurrentTask()
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIPodDBUIDFetchTaskHandler::stopCurrentTask ENTRY " << LOG_ENDL;
#endif
    m_isDisconnected = true;
#if 0   // deleted by sungha.choi 2013.04.22
    if(m_iPodPrivateData)
    {
        m_iPodPrivateData->m_Mutex.unlock();
    }
#endif
    // { deleted by sungha.choi 2013.04.02 this routine move to destructor
    //m_tracksInfoMap.clear();
    //(&m_tracksInfoMap)->~QMap();
    // }
    //emit mainLoopQuit();
    if(isWaitingForResponse() == true)    // added by sungha.choi 2013.04.12
    {
        m_ReadyToDelete = false;
        m_pLoop->quit();
    }

#ifdef DBUGLOGS
    //LOG_TRACE << "CIPodDBUIDFetchTaskHandler::stopCurrentTask Exit " << LOG_ENDL;
#endif
}

void CIPodDBUIDFetchTaskHandler::returnAllTracksInfoE(int startIndex, int count )
{
#ifdef DBUGLOGS
        //LOG_TRACE << "CIPodDBUIDFetchTaskHandler::returnAllTracksInfo() Entry startindex = " << startIndex
                             << "count = " << count << LOG_ENDL;
#endif
        m_GetDBTrackCountForMetaData = count;
#ifdef DBUGLOGS
        //LOG_INFO << "m_GetDBTrackCountForMetaData = " << m_GetDBTrackCountForMetaData << LOG_ENDL;
#endif
        m_MetaDataArray.clear();
#ifdef DBUGLOGS
        //LOG_TRACE << "CIPodDBUIDFetchTaskHandler::returnAllTracksInfo() Lock " << LOG_ENDL;
#endif
        CGetDBTrackInfo cmd(startIndex,m_GetDBTrackCountForMetaData, m_iPodPrivateData->getTransactionId(),m_iPodPrivateData->m_isIDPS);
        cmd.marshall();
        m_ExpectedCommandID = 0x0041;
        emit sendiAPPacket(cmd.getCommandBytes());

        int ret = -6;
        ret = waitForResponceFromExitCode(7000, ExitCode_GetDBTrackInfo);

#ifdef DBUGLOGS
        //LOG_TRACE << "CIPodDBUIDFetchTaskHandler::returnAllTracksInfo() Un-Lock " << LOG_ENDL;
#endif
        SiPodDBTrackInfo indexedPlayingTrackAppleInfo ;

        for(int nCurrentIndex = 0;nCurrentIndex < m_MetaDataArray.count();nCurrentIndex++)
        {
            m_CommandBuffer.clear();
            m_CommandBuffer.append(m_MetaDataArray[nCurrentIndex]);
            if(m_CommandBuffer.length() > 0)
            {
                CRetDBTrackInfo cmd(m_iPodPrivateData->m_isIDPS);
                cmd.unmarshall(m_CommandBuffer);

                int dTrackIndex = cmd.getTrackIndex();
#ifdef DBUGLOGS
                //LOG_INFO << "dTrackIndex is " << dTrackIndex << LOG_ENDL;
#endif
                QMap<int,SiPodDBTrackInfo>::const_iterator i = m_tracksInfoMap.find(dTrackIndex);
                if(i != m_tracksInfoMap.end()){
                    indexedPlayingTrackAppleInfo = i.value();
                }

                indexedPlayingTrackAppleInfo.m_trackIndex = dTrackIndex;

                switch(cmd.getInfoType())
                {
                    case 0:
                    {
                        indexedPlayingTrackAppleInfo.m_Type.clear();
                        CapabityType trackType = cmd.getCaps();
                        switch(trackType){
                            case isAudioBook:
                                indexedPlayingTrackAppleInfo.m_Type = gIPODAudioBookType;
                                break;

                            case isPodcast:
                                indexedPlayingTrackAppleInfo.m_Type = gIPODPodcastType;
                                break;

                            case isITuneEpisode:
                                indexedPlayingTrackAppleInfo.m_Type = gIPODiTunesUType;
                                break;

                            default:
                                indexedPlayingTrackAppleInfo.m_Type = gIPODAllSongType;

                        }
                        break;

                    }
                    case 1:
                    {
                        indexedPlayingTrackAppleInfo.m_Title = cmd.getInfoData();
                        // { add by jungae 2012.07.23 for CR 11279
                        // Insert a "Unknown Title" when title information is empty on support a UID device
                        if(indexedPlayingTrackAppleInfo.m_Title.isEmpty())
                        {
                            indexedPlayingTrackAppleInfo.m_Title = gUnknownTitle;
                        }
                        // } add by jungae
                        break;
                    }
                    case 2:
                    {
                        indexedPlayingTrackAppleInfo.m_Artist   = cmd.getInfoData();
                        // { add by jungae 2012.07.23 for CR 11279
                        // Insert a "Unknown Artist" when artist name is empty on support a UID device
                        if(indexedPlayingTrackAppleInfo.m_Artist.isEmpty())
                        {
                            indexedPlayingTrackAppleInfo.m_Artist = gUnknownArtist;
                        }
                        // } add by jungae
                        break;
                    }
                    case 3:
                    {
                        indexedPlayingTrackAppleInfo.m_Album = cmd.getInfoData();
                        // { add by jungae 2012.07.23 for CR 11279
                        // Insert a "Unknown Album" when album information is empty on support a UID device
                        if(indexedPlayingTrackAppleInfo.m_Album.isEmpty())
                        {
                            indexedPlayingTrackAppleInfo.m_Album = gUnknownAlbum;
                        }
                        // } add by jungae
                        break;
                    }
                    case 4:
                    {
                        indexedPlayingTrackAppleInfo.m_Genre    = cmd.getInfoData();
                        /*
                          IPod is not sending the details from Capability info to determine whether the track is iTunesU, the return command of capability info from Ipod doesn't have a bit set to indicate that the track is iTunesU.
                          Hence we decide the iTunesU by its Genre. It is observed that for the iTunesU tracks, the Genre will be "iTunes U", hence we compare for the genre and set the track type as iTunes U.
                        */
                        if(indexedPlayingTrackAppleInfo.m_Genre.contains("iTunes") )
                        {
                            indexedPlayingTrackAppleInfo.m_Type.clear();
                            indexedPlayingTrackAppleInfo.m_Type =gIPODiTunesUType;
                        }
                        // { add by jungae 2012.07.23 for CR 11279
                        // Insert a "Unknown" when genre information is empty on support a UID device
                        if(indexedPlayingTrackAppleInfo.m_Genre.isEmpty())
                        {
                            indexedPlayingTrackAppleInfo.m_Genre = gUnknownGenre;
                        }
                        // } add by jungae
                        break;
                    }
                    case 5:
                    {
                        indexedPlayingTrackAppleInfo.m_Composer = cmd.getInfoData();
                        // { add by jungae 2012.07.23 for CR 11279
                        // Insert a "Unknown" when composer name is empty on support a UID device
                        if(indexedPlayingTrackAppleInfo.m_Composer.isEmpty())
                        {
                            indexedPlayingTrackAppleInfo.m_Composer = gUnknownComposer;
                        }
                        // } add by jungae
                        break;
                    }
                    case 6:
                    {
                        indexedPlayingTrackAppleInfo.m_trackDuration = cmd.getTrackDuration();
                        break;
                    }
                    case 7:
                    {
                        indexedPlayingTrackAppleInfo.m_UID = cmd.getUID();
                        break;
                    }
                    case 8:
                    {
                        indexedPlayingTrackAppleInfo.m_chaptorCount = cmd.getChaptorCount();
                        break;
                    }
                    case 13:
                    {
                        indexedPlayingTrackAppleInfo.m_albumTrackIndex = cmd.getAlbumTrackIndex();
                        break;
                    }
                }
                m_tracksInfoMap.insert(dTrackIndex ,indexedPlayingTrackAppleInfo );
            }
        }

        QMap<int,SiPodDBTrackInfo>::const_iterator i = m_tracksInfoMap.find(startIndex);

        while(i != m_tracksInfoMap.end())
        {
            SiPodDBTrackInfo metaInfo = (SiPodDBTrackInfo)i.value();
            m_TrackList.append(metaInfo);
            i++;
        }
#ifdef DBUGLOGS
        //LOG_TRACE << "CIPodDBUIDFetchTaskHandler::returnAllTracksInfo() m_TrackList Count =  " << m_TrackList.count() << LOG_ENDL;

        //LOG_TRACE << "CIPodDBUIDFetchTaskHandler::returnAllTracksInfo() Exit " << LOG_ENDL;
#endif
}

EIPodErrors CIPodDBUIDFetchTaskHandler::getiTunesDBInfoE(SITunesDBInfo *dbInfo)
{
    //LOG_TRACE<<"CIPodDBUIDFetchTaskHandler::getiTunesDBInfo: Entry"<<LOG_ENDL;
    qMemSet(&m_itunesDBInfo,-1,sizeof(struct SITunesDBInfo));
    //setMutexLock(); // added by sungha.choi 2013.04.22
    CGetDBiTunesInfo cmd(0x01,m_iPodPrivateData->getTransactionId(),m_iPodPrivateData->m_isIDPS);
    cmd.marshall();
    m_ExpectedCommandID = 0x003D;
    emit sendiAPPacket(cmd.getCommandBytes());
    waitForResponceFromExitCode(7000, ExitCode_GetDBiTunesInfo);
    if(m_itunesDBInfo.lastSyncTime.year == -1)
    {
        //LOG_TRACE<<"CIPodDBUIDFetchTaskHandler::getiTunesDBInfo: failed to fetch DBInfo"<<LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }
    qMemCopy(dbInfo,&m_itunesDBInfo,sizeof(struct SITunesDBInfo));
    //LOG_TRACE<<"CIPodDBUIDFetchTaskHandler::getiTunesDBInfo: Exit"<<LOG_ENDL;
    return IPOD_SUCCESS;
}


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
void CIPodDBUIDFetchTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIPodDBUIDFetchTaskHandler packetAvailable lingoID = " <<
                 lingoID<< "- CommandId =  "<< commandID <<
                " - m_ExpectedCommandID =  " << m_ExpectedCommandID <<" Entry" <<LOG_ENDL;
#endif
//CR15099: Local loop was getting quit because of unwanted commands 
//in the base class, so avoid the unexpected command packets to be 
//executed

#if 0   // deleted by sungha.choi 2013.04.05
    if(m_ExpectedCommandID != commandID)
    {
        //LOG_TRACE << "not my command" << LOG_ENDL;
        return;
    }
#endif

    try {    // added by sungha.choi 2013.04.16
    m_commandID = commandID;
    QString commandId;
    commandId.setNum(m_commandID,16);

    //TODO : instead of commandID -> should be MSB and LSB seperately sent
    //commandID shall be got after <<8 of MSB and | with LSB
    //if(lingoID == 0x04 && commandID == m_ExpectedCommandID )
    if(lingoID == 0x04)
    {
//        QVariantMap namesOfCategoryContents = QVariantMap();
        switch(commandID)
        {
        //RetDBTrackInfo
        case 0x0041:
        {
            m_MetaDataArray.append(buf);
            if(m_MetaDataArray.count() == (m_GetDBTrackCountForMetaData*gGETTRACKMETADATACOUNT) )
            {
				//modified by Tan 2013.04.26. 
				//Fixing database for 1.13 device.
				//emit this signal instead of mainLoopQuit.
                //emit getDBTrackInfoReturned();
                emit mainLoopQuit(ExitCode_GetDBTrackInfo);
            }
            break;
        }
        case 0x003D:
            {
#ifdef DBUGLOGS
            //LOG_TRACE<<"CIPodDBUIDFetchTaskHandler::packetavailable:got the CRetDBiTunesInfo"<<LOG_ENDL;
#endif
                CRetDBiTunesInfo cmd(m_iPodPrivateData->m_isIDPS);
                cmd.unmarshall(buf);
#ifdef DBUGLOGS
                //LOG_TRACE<<"CIPodDBUIDFetchTaskHandler::packetavailable:infotype="<<cmd.getInfoType()<<LOG_ENDL;
#endif
                if(0x01 == cmd.getInfoType() )
                {
#ifdef DBUGLOGS
                    //LOG_TRACE<<"CIPodDBUIDFetchTaskHandler::packetavailable: ifIN"<<LOG_ENDL;
#endif
                    QDate date(cmd.getLastSyncDate());
                    m_itunesDBInfo.lastSyncTime.year = date.year();
                    m_itunesDBInfo.lastSyncTime.month = date.month();
                    m_itunesDBInfo.lastSyncTime.day = date.day();
#ifdef DBUGLOGS
                    //LOG_TRACE<<"CIPodDBUIDFetchTaskHandler::packetavailable:day="<<m_itunesDBInfo.lastSyncTime.day<<", month="<<m_itunesDBInfo.lastSyncTime.month<<", year="<<m_itunesDBInfo.lastSyncTime.year<<LOG_ENDL;
#endif
                    QTime time = cmd.getLastSyncTime();
                    m_itunesDBInfo.lastSyncTime.sec = time.second();
                    m_itunesDBInfo.lastSyncTime.min = time.minute();
                    m_itunesDBInfo.lastSyncTime.hours = time.hour();
#ifdef DBUGLOGS
                    //LOG_TRACE<<"CIPodDBUIDFetchTaskHandler::packetavailable:sec="<<m_itunesDBInfo.lastSyncTime.sec<<",min"<<m_itunesDBInfo.lastSyncTime.min<<",hour="<<m_itunesDBInfo.lastSyncTime.hours<<LOG_ENDL;
#endif
                }
                emit mainLoopQuit(ExitCode_GetDBiTunesInfo);
                break;
            }
        //ACK = 0x01
        case 0x01:    // addded by sungha.choi 2013.04.05 for Timeout issue
        {
            CACK cmd( m_iPodPrivateData->m_isIDPS);
            cmd.unmarshall(buf);
            int ackCmd= cmd.getAcknowledgedCommand();
#ifdef DBUGLOGS
            //LOG_TRACE << "Ack-ing " << ackCmd<<",Status:"<<cmd.getAckStatus()<<LOG_ENDL;
#endif
            ////LOG_TRACE<<"m_Loop.isRunning="<<m_Loop.isRunning() <<LOG_ENDL;
            switch(ackCmd)
            {
                //CR 13730: ATS Warning Fix S
                //0x0017 - selectDBRecord
                case 0x0017:
                {
                    if(!cmd.getAckStatus())
                        m_CommandStatus = true;
                    emit mainLoopQuit(ExitCode_SelectDBRecord);
                    break;
                }
                case 0x0040:
                {
                    if(cmd.getAckStatus() != 0x0)
                    {
                        emit mainLoopQuit(ExitCode_GetDBTrackInfo);
                    }
                    break;
                }
                default:
                    CIPodDBFetchTaskHandler::packetAvailable(lingoID, commandID, buf);
                    break;
            }
            break;
        }
        default:
            CIPodDBFetchTaskHandler::packetAvailable(lingoID, commandID, buf);
            break;
        } //end of switch
    }
    else
        {
        CIPodDBFetchTaskHandler::packetAvailable(lingoID, commandID, buf);
#ifdef DBUGLOGS
        //LOG_TRACE << "iTunesDBTH Not my lingo...ignoring!" << LOG_ENDL;
#endif
        }
#ifdef DBUGLOGS
    //LOG_TRACE << "CIPodDBUIDFetchTaskHandler packetAvailable lingoID" <<
                  lingoID<< "- CommandId "<< commandID << " Exit" << LOG_ENDL;
#endif
    }   // end try
    catch(EIPodErrors ex){
        //LOG_TRACE << "CIPodDBUIDFetchTaskHandler::PacketAvailable Execption Caught *******"<< LOG_ENDL;
    }
    return;
}
