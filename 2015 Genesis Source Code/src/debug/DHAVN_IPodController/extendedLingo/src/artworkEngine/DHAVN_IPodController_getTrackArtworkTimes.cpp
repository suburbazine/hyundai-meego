/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPOdController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  03 Dec 2011                                   *
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
#include <DHAVN_IPodController_getTrackArtworkTimes.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QByteArray>
 
using namespace ExtendedLingo;


CGetTrackArtworkTimes::CGetTrackArtworkTimes(int trackIndex, int formatId,int artIndex, int count, int transId,  bool isTidEnabled)
:CIAPPacket(CIAPCmds::ExtenededLingoId,CIAPCmds::L4_2A_GetTrackArtworkTimes,transId,isTidEnabled)
{
    m_FormatId = formatId;
    m_TransactionID = transId;
    m_TrackIndex = trackIndex;
    m_Count = count;
    m_ArtWorkIndex = artIndex;

}

QByteArray CGetTrackArtworkTimes::marshall()
{
    QByteArray cmd;
    int i =-1;

    cmd[++i] = 0x55;
    cmd[++i] = 0x00;
    cmd[++i] = CIAPCmds::ExtenededLingoId;
    cmd[++i] = 0x00;
    cmd[++i] = CIAPCmds::L4_2A_GetTrackArtworkTimes;
    if( m_isTidEnabled  ){
        cmd[++i] = ((0x0000FF00&m_TransactionID)) >> 8;
        cmd[++i] = 0x000000FF & m_TransactionID;
    }
    cmd[++i] = (m_TrackIndex >> 24) & 0x00FF ;
    cmd[++i] = (m_TrackIndex >> 16) & 0x00FF ;
    cmd[++i] = (m_TrackIndex >> 8) & 0x00FF ;
    cmd[++i] = m_TrackIndex  & 0x00FF ;

    cmd[++i] = (m_FormatId >> 8) & 0x00FF ;
    cmd[++i] = m_FormatId & 0x00FF ;

    cmd[++i] = (m_ArtWorkIndex >> 8) & 0x00FF ;
    cmd[++i] = m_ArtWorkIndex & 0x00FF ;

    cmd[++i] = (m_Count >> 8) & 0x00FF ;
    cmd[++i] = m_Count  & 0x00FF ;

    cmd[1] = i-1;
    cmd[i+1] = CIpodControllerGlobal::checkSum(cmd,i);

    m_CommandBytes = cmd;
    return cmd;
}

void CGetTrackArtworkTimes::unmarshall(QByteArray)
{

}

