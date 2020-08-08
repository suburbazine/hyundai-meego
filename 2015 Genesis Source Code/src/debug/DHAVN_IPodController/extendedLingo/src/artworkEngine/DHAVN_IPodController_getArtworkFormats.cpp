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
#include <DHAVN_IPodController_getArtworkFormats.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QByteArray>

using namespace ExtendedLingo;


CGetArtworkFormats::CGetArtworkFormats(int transId,  bool isTidEnabled):CIAPPacket(CIAPCmds::ExtenededLingoId,CIAPCmds::L4_0E_GetArtworkFormats,transId,isTidEnabled),m_TrackIndex(-1),m_FormatId(-1),m_TimeOffset(-1)
{

}

QByteArray CGetArtworkFormats::marshall()
{
    QByteArray cmd;
    int i =-1;

    cmd[++i] = 0x55;
    cmd[++i] = 0x00;
    cmd[++i] = CIAPCmds::ExtenededLingoId;
    cmd[++i] = 0x00;
    cmd[++i] = CIAPCmds::L4_0E_GetArtworkFormats;
    if( m_isTidEnabled ){
        cmd[++i] = ((m_TransactionID & 0xff00) >> 8);
        cmd[++i] = m_TransactionID & 0x00ff;
    }
    cmd[1] = i-1;
    cmd[i+1] = CIpodControllerGlobal::checkSum(cmd,i);

    m_CommandBytes = cmd;
    return cmd;
}

void CGetArtworkFormats::unmarshall(QByteArray)
{

}
