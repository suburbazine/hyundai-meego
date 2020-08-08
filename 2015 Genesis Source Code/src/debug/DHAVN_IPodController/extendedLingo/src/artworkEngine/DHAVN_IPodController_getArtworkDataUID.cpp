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
#include <DHAVN_IPodController_getArtworkDataUID.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QByteArray>

using namespace ExtendedLingo;

CGetArtworkDataUID::CGetArtworkDataUID(int format,
                                                 int time,
                                                 int transId, bool isTidEnabled, quint64 uid)   // Modified by pawan 2013.05.15.
                                                     :CIAPPacket(CIAPCmds::ExtenededLingoId,CIAPCmds::L4_4E_GetTrackArtworkDataUID,transId,isTidEnabled),
                                                      m_FormatId(format),
                                                       m_TimeOffset(time), m_Uid(uid)   // Modified by pawan 2013.05.15.
{
#ifdef DBUGLOGS
        LOG_INIT_V2(gControllerLogName);
        //LOG_TRACE << "in CGetArtworkDataUID constructor" << LOG_ENDL;
#endif
}

QByteArray CGetArtworkDataUID::marshall()
{
    QByteArray cmd;
    int i =-1;

    cmd[++i] = 0x55;
    cmd[++i] = 0x00;
    cmd[++i] = CIAPCmds::ExtenededLingoId;
    cmd[++i] = 0x00;
    cmd[++i] = 0x4E;
    if(m_isTidEnabled)
    {
        cmd[++i] = ((m_TransactionID & 0xff00) >> 8);
        cmd[++i] = m_TransactionID & 0x00ff;
    }
	// { Added by pawan 2013.05.15.
    if(m_Uid > 0)
    {
        #ifdef DBUGLOGS
        //LOG_TRACE << "basicp  CGetArtworkDataUID in UID case" << LOG_ENDL;
#endif
        cmd[++i] = 0x00;
        cmd[++i] = (m_Uid >> 56) & 0x00FF ;
        cmd[++i] = (m_Uid >> 48) & 0x00FF ;
        cmd[++i] = (m_Uid >> 40) & 0x00FF ;
        cmd[++i] = (m_Uid >> 32) & 0x00FF ;
        cmd[++i] = (m_Uid >> 24) & 0x00FF ;
        cmd[++i] = (m_Uid >> 16) & 0x00FF ;
        cmd[++i] = (m_Uid >> 8) & 0x00FF ;
        cmd[++i] = m_Uid  & 0x00FF ;
    }


    cmd[++i] = (m_FormatId >> 8) & 0x00FF ;
    cmd[++i] = m_FormatId & 0x00FF ;

    cmd[++i] = (m_TimeOffset >> 24) & 0x00FF ;
    cmd[++i] = (m_TimeOffset >> 16) & 0x00FF ;
    cmd[++i] = (m_TimeOffset >> 8) & 0x00FF ;
    cmd[++i] = m_TimeOffset  & 0x00FF ;

    cmd[1] = i-1;
    cmd[i+1] = CIpodControllerGlobal::checkSum(cmd,i);

    m_CommandBytes = cmd;
    return cmd;
}

void  CGetArtworkDataUID::unmarshall(QByteArray)
{
}





