
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
 *
 * PROGRAMMERS : Sowmini Philip / Venkataramana      *
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
#include <DHAVN_IPodController_getUIDTrackInfo.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace ExtendedLingo;

//mask = 0x4E - for title, album, artist, duration
CGetUIDTrackInfo::CGetUIDTrackInfo(quint64 uid, qint8 mask, int transId,bool isTidEnabled)
                                                    :CIAPPacket(0x04,0x003E,transId,isTidEnabled),
                                                m_UID(uid), m_Mask(mask)
{

}

QByteArray CGetUIDTrackInfo::marshall()
{
        QByteArray command;
        int i = -1;
        //Packing format =  Big Endian
        command[++i] = 0x55;
        command[++i] = 0x00;
        command[++i] = 0x04;
        command[++i] = 0x00;
        command[++i] = 0x3E;

        if( m_isTidEnabled )
        {
            command[++i] = ((m_TransactionID & 0xff00) >> 8);
            command[++i] = m_TransactionID & 0x00ff;
        }

        command[++i] = (m_UID >> 56) & 0x00FF ;
        command[++i] = (m_UID >> 48) & 0x00FF ;
        command[++i] = (m_UID >> 40) & 0x00FF ;
        command[++i] = (m_UID >> 32) & 0x00FF ;
        command[++i] = (m_UID >> 24) & 0x00FF ;
        command[++i] = (m_UID >> 16) & 0x00FF ;
        command[++i] = (m_UID >> 8) & 0x00FF ;
        command[++i] = m_UID  & 0x00FF ;



        command[++i] = m_Mask;

        command[1] = i-1;

        command.append(CIpodControllerGlobal::checkSum(command, i));
        m_CommandBytes = command;
        return command;
}

void  CGetUIDTrackInfo::unmarshall(QByteArray)
{
}



