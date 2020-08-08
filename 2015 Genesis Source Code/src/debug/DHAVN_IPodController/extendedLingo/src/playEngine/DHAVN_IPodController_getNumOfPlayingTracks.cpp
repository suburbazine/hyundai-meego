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
#include <DHAVN_IPodController_getNumOfPlayingTracks.h>
#include <QByteArray>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
 
using namespace ExtendedLingo;

CGetNumOfPlayingTracks::CGetNumOfPlayingTracks( int transId,  bool isTidEnabled)
        :CIAPPacket(0x04,0x0016,transId, isTidEnabled)
{
#ifdef DBUGLOGS
    LOG_INIT_V2("Media_IPodController");
#endif
}

QByteArray CGetNumOfPlayingTracks::marshall()
{
    QByteArray command;
#ifdef DBUGLOGS
    //LOG_TRACE << "In CGetNumOfPlayingTracks::marshall " << LOG_ENDL;
#endif
    int i = -1;
    //Packing format =  Big Endian
    command[++i] = 0x55;
    command[++i] = 0x05;
    command[++i] = 0x04;
    command[++i] = 0x00;
    command[++i] = 0x35;

    if( m_isTidEnabled )
    {
        command[++i] = ((m_TransactionID & 0xff00) >> 8);
        command[++i] = m_TransactionID & 0x00ff;
    }
#ifdef DBUGLOGS
    //LOG_TRACE << "CGetNumOfPlayingTracks::marshall::gid"<<LOG_ENDL;
#endif
    command[1] = i-1;
    command.append(CIpodControllerGlobal::checkSum(command, i));
    m_CommandBytes = command;
    return command;
}

void  CGetNumOfPlayingTracks::unmarshall(QByteArray)
{
}

