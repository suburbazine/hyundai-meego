/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPOdController                                    *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  18 May 2012                                   *
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
#include <DHAVN_IPodController_retArtworkTimesUID.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QByteArray>
using namespace ExtendedLingo;


CRetTrackArtworkTimesUID::CRetTrackArtworkTimesUID( bool isTidEnabled):CIAPPacket(CIAPCmds::ExtenededLingoId,CIAPCmds::L4_4D_RetTrackArtworkTimesUID,0,isTidEnabled)
{
#ifdef DBUGLOGS
    LOG_INIT_V2(gControllerLogName);
#endif
    m_TimeOffset = -1;
}

QByteArray CRetTrackArtworkTimesUID::marshall()
{
        return NULL;
}

void CRetTrackArtworkTimesUID::unmarshall(QByteArray cmd)
{
    if( ((int)cmd[0] == 0x55) &&
        ((int)cmd[2] == CIAPCmds::ExtenededLingoId) &&
        ((int)cmd[3] == 0x00) &&
        ((int)cmd[4] == 0x4D) ){

        int i = 0;  // Modified by pawan 2013.05.15.
        if(!m_isTidEnabled)
        {
		// { Added by pawan 2013.05.15.
            i = 5;
        }
        else
        {
            i = 7;
        }

        if((int8_t)cmd[i] == 0x00)
        {
            i = i+9;
        }
        else
        {
            i = i+5;
		// } Added by pawan 2013.05.15.
        }

        m_TimeOffset = 0 | ((0x000000FF & cmd[i])<<24) | ((0x000000FF & cmd[i+1])<<16) | ((0x000000FF & cmd[i+2])<<8) | ((0x000000FF & cmd[i+3]));
#ifdef DBUGLOGS
        //LOG_INFO << "CRetTrackArtworkTimesUID:: artworktime is = " << m_TimeOffset << LOG_ENDL;
#endif
    }

    m_CommandBytes = cmd;
}

int CRetTrackArtworkTimesUID::getTimeOffset()
{
    return m_TimeOffset;
}

