/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Sowmini Philip / Venkataramana      *
 * DATE       :  2 Jan 2012                                   *
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
#include <DHAVN_IPodController_getiPodCaps.h>
#include <QByteArray>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
 
using namespace StorageLingo;

CGetiPodCaps::CGetiPodCaps( int transId, bool isTidEnabled)
        :CIAPPacket(0x0C,0x01,transId, isTidEnabled)
{
}

QByteArray CGetiPodCaps::marshall()
{
    QByteArray command;
    int i = -1;
    //Packing format =  Big Endian
    command[++i] = 0x55;
    command[++i] = 0x02;
    command[++i] = 0x0C;
    command[++i] = 0x01;
    if( m_isTidEnabled  )
    {
        command[++i] = ((m_TransactionID & 0xff00) >> 8);
        command[++i] = m_TransactionID & 0x00ff;
    }


    command[1] = i-1;
    command.append(CIpodControllerGlobal::checkSum(command, i));
    m_CommandBytes = command;
    return command;
}

void  CGetiPodCaps::unmarshall(QByteArray)
{
}

