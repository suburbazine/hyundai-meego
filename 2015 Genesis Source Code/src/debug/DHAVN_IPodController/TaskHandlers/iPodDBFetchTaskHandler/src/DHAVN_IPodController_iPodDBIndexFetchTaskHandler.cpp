
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
#include <DHAVN_IPodController_iPodDBIndexFetchTaskHandler.h>
#include <DHAVN_IPodController_ack.h>
#include <QDebug>
#include <DHAVN_IPodController_eventloopTimer.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackTitle.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackArtist.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackAlbum.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackTitle.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackArtist.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackAlbum.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackInfo.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackInfo.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_selectDBRecord.h>
#include <DHAVN_IPodController_getNumberCategorizedDatabaseRecords.h>
#include <DHAVN_IPodController_returnNumberCategorizedDatabaseRecords.h>
#include <DHAVN_IPodController_selectSortDBRecord.h>
#include <QDate>
#include <QTime>
#include <QElapsedTimer>
#include "DHAVN_EventLoop.h"

using namespace ExtendedLingo;

#define NUM_DBTRACK_INFO 6

CIPodDBIndexFetchTaskHandler::CIPodDBIndexFetchTaskHandler(QObject* parent,
                                                 iPodControllerPrivateData* privateData,
                                                 CIAPPacketWriter* iapWriter,
                                                           CIAPPacketReader* iapReader) : CIPodDBFetchTaskHandler(parent, privateData, iapWriter, iapReader, false)
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
    connect(m_iAPReader,SIGNAL(packetAvailable(int, int, QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
    connect(this, SIGNAL(sendiAPPacket(QByteArray)) ,m_iAPWriter, SLOT(sendiAPPacket(QByteArray)));
    connect(&m_ListFetchtimer,SIGNAL(timeout()),this,SLOT(getListTimeOut()) );
    m_ListFetchtimer.setSingleShot(true);

    m_metaDataList_IndexedBased.clear();
    m_MetaDataToFecthCount  = 0;
    m_metadata_Flag = false;
}
CIPodDBIndexFetchTaskHandler::~CIPodDBIndexFetchTaskHandler()
{
   //LOG_TRACE << "CIPodDBIndexFetchTaskHandler::~CIPodDBIndexFetchTaskHandler " << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
//  CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData              QString (OUT), (int) (IN)
//
// Explanation
// This method retrieves Metadata of a track, given its index under "Track" category
// This method shall be used by the Tracker Connector
// History
// 01-Jun-2012      Pradeep                        Added initial implementation
*****************************************************************************/
SiPodDBTrackInfo CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaDataE(int index, int metaDataCount)
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData Entry " << LOG_ENDL;
#endif

    SiPodDBTrackInfo trackInfo;
    m_MetaDataToFecthCount = metaDataCount;
    m_metadata_Flag = true;
    m_metaDataList_IndexedBased.clear();
    trackInfo.m_trackIndex = index;

    //setMutexLock(); // added by sungha.choi 2013.04.22

    if(m_MetaDataToFecthCount == 6)
    {
    CGetIndexedPlayingTrackTitle cmdTitle(index,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmdTitle.marshall();
    //m_ExpectedCommandID = 0x0021;
    emit sendiAPPacket(cmdTitle.getCommandBytes());
    }



    CGetIndexedPlayingTrackAlbumName cmdAlbum(index,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmdAlbum.marshall();
    //m_ExpectedCommandID = 0x0025;
    emit sendiAPPacket(cmdAlbum.getCommandBytes());



    CGetIndexedPlayingTrackArtistName cmdArtist(index,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmdArtist.marshall();
    //m_ExpectedCommandID = 0x0023;
    emit sendiAPPacket(cmdArtist.getCommandBytes());



    CGetIndexedPlayingTrackInfo cmdGenreInfo(index,GENREINFO, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmdGenreInfo.marshall();
    //m_ExpectedCommandID = 0x000D;
    emit sendiAPPacket(cmdGenreInfo.getCommandBytes());



    CGetIndexedPlayingTrackInfo cmdComposerInfo(index,COMPOSERINFO, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmdComposerInfo.marshall();
    //m_ExpectedCommandID = 0x000D;
    emit sendiAPPacket(cmdComposerInfo.getCommandBytes());


    CGetIndexedPlayingTrackInfo cmdTypeInfo(index,TYPEINFO, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmdTypeInfo.marshall();
    //m_ExpectedCommandID = 0x000D;
    emit sendiAPPacket(cmdTypeInfo.getCommandBytes());
    QElapsedTimer waitForResponceTimer;
    waitForResponceTimer.start();
    waitForResponceFromExitCode(7000, ExitCode_IndexedTrackMetadata);
#ifdef DBUGLOGS
    //LOG_INFO << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData m_metaDataList_IndexedBased.length= " << m_metaDataList_IndexedBased.length() << LOG_ENDL;
#endif

    for(int i=0 ; i < m_metaDataList_IndexedBased.count() ; i++)
    {

        m_CommandBuffer = m_metaDataList_IndexedBased[i];
        m_commandID = m_CommandBuffer[4];
#ifdef DBUGLOGS
        //LOG_INFO << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData m_commandID = " << m_commandID << LOG_ENDL;
#endif


        switch(m_commandID)
        {
        case 0x21: //ReturnTitle
        {
#ifdef DBUGLOGS
            //LOG_TRACE << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData Entered Title case"<< LOG_ENDL;
#endif
            CReturnIndexedPlayingTrackTitle cmd( m_iPodPrivateData->m_isIDPS);
            cmd.unmarshall(m_CommandBuffer);
            trackInfo.m_Title = cmd.getTitle();
#ifdef DBUGLOGS
            //LOG_INFO << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData Title = " << trackInfo.m_Title.toAscii().data() << LOG_ENDL;
#endif
            if(trackInfo.m_Title.isEmpty()) {
                // { modified by kihyung 2012.07.17 for CR 11279
                // trackInfo.m_Title = "Unknown";
                trackInfo.m_Title = gUnknownTitle;
                // } modified by kihyung
            }
            break;
        }
        case 0x25: //ReturnAlbum
        {
#ifdef DBUGLOGS
            //LOG_TRACE << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData Entered Album case"<< LOG_ENDL;
#endif
            CReturnIndexedPlayingTrackAlbumName cmd( m_iPodPrivateData->m_isIDPS);
            cmd.unmarshall(m_CommandBuffer);
            trackInfo.m_Album = cmd.getAlbumName();
#ifdef DBUGLOGS
            //LOG_INFO << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData Title = " << trackInfo.m_Album.toAscii().data() << LOG_ENDL;
#endif
            if(trackInfo.m_Album.isEmpty()) {
                // { modified by kihyung 2012.07.17 for CR 11279
                // trackInfo.m_Album = "Unknown";
                trackInfo.m_Album = gUnknownAlbum;
                // } modified by kihyung
            }
            break;
        }

        case 0x23: //ReturnArtist
        {
#ifdef DBUGLOGS
            //LOG_TRACE << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData Entered Artist case"<< LOG_ENDL;
#endif
            CReturnIndexedPlayingTrackArtistName cmd( m_iPodPrivateData->m_isIDPS);
            cmd.unmarshall(m_CommandBuffer);
            trackInfo.m_Artist = cmd.getArtistName();
#ifdef DBUGLOGS
            //LOG_INFO << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData Title = " << trackInfo.m_Artist.toAscii().data() << LOG_ENDL;
#endif
            if(trackInfo.m_Artist.isEmpty()) {
                // { modified by kihyung 2012.07.17 for CR 11279
                // trackInfo.m_Artist = "Unknown";
                trackInfo.m_Artist = gUnknownArtist;
                // } modified by kihyung
            }
            break;
        }

        case 0x0D:
        {
            CReturnIndexedPlayingTrackInfo cmd( m_iPodPrivateData->m_isIDPS);
            cmd.unmarshall(m_CommandBuffer);
            if(cmd.getInfoType() == 5)
            {
                trackInfo.m_Genre =  cmd.getGenre();
#ifdef DBUGLOGS
                //LOG_INFO << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData Genre = " << trackInfo.m_Genre.toAscii().data() << LOG_ENDL;
#endif
                if(trackInfo.m_Genre.isEmpty()) {
                    // { modified by kihyung 2012.07.17 for CR 11279
                    // trackInfo.m_Genre =  "Unknown";
                    trackInfo.m_Genre =  gUnknownGenre;
                    // } modified by kihyung
                }
            }
            else if(cmd.getInfoType() == 6)
            {
                trackInfo.m_Composer =  cmd.getComposer();
#ifdef DBUGLOGS
                //LOG_INFO << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData Composer = " << trackInfo.m_Composer.toAscii().data() << LOG_ENDL;
#endif
                if(trackInfo.m_Composer.isEmpty()) {
                    // { modified by kihyung 2012.07.17 for CR 11279
                    // trackInfo.m_Composer =  "Unknown";
                    trackInfo.m_Composer =  gUnknownComposer;
                    // } modified by kihyung
                }
            }

            else if(cmd.getInfoType() == 0x00)
            {
                int type  = cmd.getTypeInfo();
#ifdef DBUGLOGS
                //LOG_INFO << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData Type = " << type << LOG_ENDL;
#endif

                switch(type)
                {
                case CReturnIndexedPlayingTrackInfo::AUDIOBOOK :
                    trackInfo.m_Type = gIPODAudioBookType;
                    break;
                case CReturnIndexedPlayingTrackInfo::PODCAST :
                    trackInfo.m_Type = gIPODPodcastType;
                    break;
                case CReturnIndexedPlayingTrackInfo::ITUNESU :
                    trackInfo.m_Type = gIPODiTunesUType;
                    break;
                case CReturnIndexedPlayingTrackInfo::MUSICTRACKS :
                default:
                    trackInfo.m_Type = gIPODAllSongType;
                    break;

                }
                trackInfo.m_trackDuration = cmd.getTrackDuration();
            }

            break;
        }

        }

    }
    m_metaDataList_IndexedBased.clear();
    /*
IPod is not sending the details from Capability info to determine whether the track is iTunesU, the return command of capability info from Ipod doesn't have a bit set to indicate that the track is iTunesU.
Hence we decide the iTunesU by its Genre. It is observed that for the iTunesU tracks, the Genre will be "iTunes U", hence we compare for the genre and set the track type as iTunes U.
*/
    if(trackInfo.m_Genre.contains("iTunes") )
    {
        trackInfo.m_Type = gIPODiTunesUType;
    }
#ifdef DBUGLOGS
    //LOG_TRACE << "CIPodDBIndexFetchTaskHandler::getIndexedTrackMetaData Exit " << LOG_ENDL;
#endif
    return trackInfo;
}


void CIPodDBIndexFetchTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIPodDBIndexFetchTaskHandler packetAvailable lingoID = " <<
                 lingoID<< "- CommandId =  "<< commandID <<
                " - m_ExpectedCommandID =  " << m_ExpectedCommandID <<" Entry" << LOG_ENDL;
#endif
    //CIPodDBFetchTaskHandler::packetAvailable(lingoID, commandID, buf);    // duplicate packetavailable call, deleted by sungha.choi 2013.04.12
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
        //CR 13730: ATS Warning Fix S
        case 0x0019: //ReturnNumberCategorizedDatabaseRecords
            {
                CReturnNumberCategorizedDatabaseRecords retCmd( m_iPodPrivateData->m_isIDPS);
                retCmd.unmarshall(buf);
                listCount =  retCmd.getCount();     // re-enabled by sungha.choi 2013.04.05
                emit mainLoopQuit(ExitCode_GetNumberCategorizedDatabaseRecords);
                break;
            }// end of case 0x0019
        //CR 13730: ATS Warning Fix E
            //ReturnIndexedPlayingTrackInfo
        case 0x000D:
            //ReturnIndexedPlayingTrackTitle
        case 0x0021:
            //ReturnIndexedPlayingTrackArtistName
        case 0x0023:
            //ReturnIndexedPlayingTrackAlbumName
        case 0x0025:
            {
//            //LOG_INFO << "CIPodDBIndexFetchTaskHandler::PacketAvailable  m_metadata_Flag = "<< m_metadata_Flag << LOG_ENDL;
                if(m_metadata_Flag)
                {
                    m_metaDataList_IndexedBased.append(buf);
                    if (m_metaDataList_IndexedBased.count() == m_MetaDataToFecthCount)
                    {
                        m_metadata_Flag = false;
                        emit mainLoopQuit(ExitCode_IndexedTrackMetadata);
                        break;
                    }
                }
            else
                {
                    m_CommandBuffer.clear();
                    m_CommandBuffer = buf;
//                    //LOG_INFO << "CIPodDBIndexFetchTaskHandler::PacketAvailable m_CommandBuffer.length = " << m_CommandBuffer.length()
//                                     << "& commandId= "<< commandId << LOG_ENDL;
                    emit mainLoopQuit(ExitCode_IndexedTrackMetadata);
                    break;
                }
                break;
            }
        //ACK = 0x01
        case 0x01:
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
                    //0x0038 - selectSortDBRecord
                case 0x0038:
                {
                    if(!cmd.getAckStatus())
                        m_CommandStatus = true;
                    emit mainLoopQuit(ExitCode_SelectSortDBRecord);
                    break;
                }
                //CR 13730: ATS Warning Fix E
                    //ReturnIndexedPlayingTrackInfo
                case 0x000C:
                    //ReturnIndexedPlayingTrackTitle
                case 0x0020:
                    //ReturnIndexedPlayingTrackArtistName
                case 0x0022:
                    //ReturnIndexedPlayingTrackAlbumName
                case 0x0024:
                    {
                            m_CommandBuffer.clear();
                            emit mainLoopQuit();
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
    //LOG_TRACE << "CIPodDBIndexFetchTaskHandler packetAvailable lingoID" <<
                  lingoID<< "- CommandId "<< commandID << " Exit" << LOG_ENDL;
#endif
    }   // end try
    catch(EIPodErrors ex){
        //LOG_TRACE << "CIPodDBIndexFetchTaskHandler::PacketAvailable Execption Caught *******"<< LOG_ENDL;
    }
    return;
}
//CR 13730: ATS Warning Fix S
bool CIPodDBIndexFetchTaskHandler::playCommandCall(int index)
{
    m_CommandStatus = false;
    //setMutexLock(); // added by sungha.choi 2013.04.22

    bool isNano2G = (m_iPodPrivateData->m_iPodProductID == NANO_2G_PRODUCT_ID ? true : false);
    //setMutexLock(); // added by sungha.choi 2013.04.22
    if(isNano2G)
    {
        //LOG_TRACE << "CIPodDBFetchTaskHandler::playCommandCall CSelectSortDBRecord IN" << LOG_ENDL ;
        CSelectSortDBRecord playCommand("Track",index,"Artist",m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
        playCommand.marshall();
        emit sendiAPPacket(playCommand.getCommandBytes());
        //LOG_TRACE << "CIPodDsBIndexFetchTaskHandler::playCommandCall() Calling waitForResponceFromExitCode " << LOG_ENDL;
        waitForResponceFromExitCode(7000,ExitCode_SelectSortDBRecord);
        //LOG_TRACE << "CIPodDBFetchTaskHandler::playCommandCall CSelectSortDBRecord OUT" << LOG_ENDL ;
    }
    else
    {
        //LOG_TRACE << "CIPodDBFetchTaskHandler::playCommandCall CSelectDBRecord IN" << LOG_ENDL ;
        CSelectDBRecord playCommand("Track",index,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
        playCommand.marshall();
        emit sendiAPPacket(playCommand.getCommandBytes());
        //LOG_TRACE << "CIPodDBIndexFetchTaskHandler::playCommandCall() Calling waitForResponceFromExitCode " << LOG_ENDL;
        waitForResponceFromExitCode(7000,ExitCode_SelectDBRecord);
        //LOG_TRACE << "CIPodDBFetchTaskHandler::playCommandCall CSelectDBRecord OUT" << LOG_ENDL ;
    }
    return m_CommandStatus;
}
void CIPodDBIndexFetchTaskHandler::stopTaskHandler()
{
    if(!m_isDisconnected){//if ipod already disconneted, bellow method call may lead to crash.
        disconnect(m_iAPReader,SIGNAL(packetAvailable(int, int, QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
        disconnect(m_iAPReader,SIGNAL(packetAvailableForDBTaskHandler(int,int,QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
        disconnect(this, SIGNAL(sendiAPPacket(QByteArray)) ,m_iAPWriter, SLOT(sendiAPPacket(QByteArray)));
        disconnect(&m_ListFetchtimer,SIGNAL(timeout()),this,SLOT(getListTimeOut()) );
    }
    m_isDisconnected = true;
}
//CR 13730: ATS Warning Fix E
