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
#include <DHAVN_IPodController_retTrackArtworkTimes.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QByteArray>
using namespace ExtendedLingo;


CRetTrackArtworkTimes::CRetTrackArtworkTimes( bool isTidEnabled):CIAPPacket(CIAPCmds::ExtenededLingoId,CIAPCmds::L4_2B_RetTrackArtworkTimes,0,isTidEnabled)
{
    m_TimeOffset = -1;
}

QByteArray CRetTrackArtworkTimes::marshall()
{
	return NULL;
}

void CRetTrackArtworkTimes::unmarshall(QByteArray cmd)
{
    if( ((int)cmd[0] == 0x55) &&
        ((int)cmd[2] == CIAPCmds::ExtenededLingoId) &&
        ((int)cmd[3] == 0x00) &&
        ((int)cmd[4] == CIAPCmds::L4_2B_RetTrackArtworkTimes) ){

        int i = 5;
        if( m_isTidEnabled ){
            i = 7;
        }
        m_TimeOffset = 0 | ((0x000000FF & cmd[i])<<24) | ((0x000000FF & cmd[i+1])<<16) | ((0x000000FF & cmd[i+2])<<8) | ((0x000000FF & cmd[i+3]));

    }
    m_CommandBytes = cmd;
}

int CRetTrackArtworkTimes::getTimeOffset()
{
    return m_TimeOffset;
}
