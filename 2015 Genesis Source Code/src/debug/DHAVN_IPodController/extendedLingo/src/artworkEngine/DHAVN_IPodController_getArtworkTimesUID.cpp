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
#include <DHAVN_IPodController_getArtworkTimesUID.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QByteArray>

using namespace ExtendedLingo;

CGetArtworkTimesUID::CGetArtworkTimesUID(int transId,bool isTidEnabled, int formatID, quint64 uid):CIAPPacket(CIAPCmds::ExtenededLingoId,CIAPCmds::L4_4C_GetTrackArtworkTimesUID,transId,isTidEnabled), m_FormatID(formatID), m_Uid(uid) // Added by pawan 2013.05.15.
{
LOG_INIT_V2(gControllerLogName); // Added by pawan 2013.05.15.
}

QByteArray CGetArtworkTimesUID::marshall()
{
    QByteArray cmd;
    int i =-1;

    cmd[++i] = 0x55;
    cmd[++i] = 0x00;
    cmd[++i] = CIAPCmds::ExtenededLingoId;
    cmd[++i] = 0x00;
    cmd[++i] = 0x4C;
    if(m_isTidEnabled)
    {
        cmd[++i] = ((m_TransactionID & 0xff00) >> 8);
        cmd[++i] = m_TransactionID & 0x00ff;
    }
	
	// { Added by pawan 2013.05.15.
    if(m_Uid >0)
    {
        cmd[++i] = 0x00;
        QByteArray bytes;
        for(int j = 7 ; j >= 0 ;j--)
        {
                bytes[j] = (m_Uid >> 0) & 0x00FF;
                m_Uid >>= 8;
        }

        //LOG_TRACE << "basicp  CGetArtworkTimesUID in uid case: " << bytes.count() << LOG_ENDL;
//        cmd[++i] = (m_Uid >> 56) & 0x00FF ;
//        cmd[++i] = (m_Uid >> 48) & 0x00FF ;
//        cmd[++i] = (m_Uid >> 40) & 0x00FF ;
//        cmd[++i] = (m_Uid >> 32) & 0x00FF ;
//        cmd[++i] = (m_Uid >> 24) & 0x00FF ;
//        cmd[++i] = (m_Uid >> 16) & 0x00FF ;
//        cmd[++i] = (m_Uid >> 8) & 0x00FF ;
//        cmd[++i] = m_Uid  & 0x00FF ;
        cmd.append(bytes);
        i = i+8;
    }


    cmd[++i] = (m_FormatID >> 8) & 0x00FF ;
    cmd[++i] = m_FormatID & 0x00FF ;

    cmd[++i] = 0x00;
    cmd[++i] = 0x00;

    cmd[++i] = 0x00;
    cmd[++i] = 0x01;

    cmd[1] = i-1;
    cmd[i+1] = CIpodControllerGlobal::checkSum(cmd,i);

    m_CommandBytes = cmd;
    return cmd;
}

void CGetArtworkTimesUID::unmarshall(QByteArray)
{

}

