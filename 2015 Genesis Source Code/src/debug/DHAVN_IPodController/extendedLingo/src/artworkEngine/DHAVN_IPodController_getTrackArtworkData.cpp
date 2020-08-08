/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
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
#include <DHAVN_IPodController_getTrackArtworkData.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QByteArray>

using namespace ExtendedLingo;

CGetTrackArtworkData::CGetTrackArtworkData(int track,
                                                 int format,
                                                 int time,
                                                 int transId, bool isTidEnabled)
                                                     :CIAPPacket(CIAPCmds::ExtenededLingoId,CIAPCmds::L4_10_GetTrackArtworkData,transId,isTidEnabled),
                                                      m_TrackIndex(track),
                                                      m_FormatId(format),
                                                      m_TimeOffset(time)
{
#ifdef DBUGLOGS
        LOG_INIT_V2(gControllerLogName);
        //LOG_TRACE << "in CGetTrackArtworkData constructor" << LOG_ENDL;
#endif
}

QByteArray CGetTrackArtworkData::marshall()
{
        QByteArray command;
        int i = -1;

        //Packing format =  Big Endian
        command[++i] = 0x55;
        command[++i] = 0x0C;
        command[++i] = CIAPCmds::ExtenededLingoId;
        command[++i] = 0x00;
        command[++i] = CIAPCmds::L4_10_GetTrackArtworkData;
        if( m_isTidEnabled ){
            command[++i] = ((0x0000FF00&m_TransactionID)) >> 8;
            command[++i] = 0x000000FF & m_TransactionID;
        }

        command[++i] = (m_TrackIndex >> 24) & 0x00FF ;
        command[++i] = (m_TrackIndex >> 16) & 0x00FF ;
        command[++i] = (m_TrackIndex >> 8) & 0x00FF ;
        command[++i] =  m_TrackIndex  & 0x00FF ;

        command[++i] = (m_FormatId >> 8) & 0x00FF ;
        command[++i] = m_FormatId & 0x00FF ;

        command[++i] = (m_TimeOffset >> 24) & 0x00FF ;
        command[++i] = (m_TimeOffset >> 16) & 0x00FF ;
        command[++i] = (m_TimeOffset >> 8) & 0x00FF ;
        command[++i] = m_TimeOffset  & 0x00FF ;

        command[1] = i-1;
        command[i+1] = CIpodControllerGlobal::checkSum(command,i);

        m_CommandBytes = command;
        return command;
}

void  CGetTrackArtworkData::unmarshall(QByteArray)
{
}




