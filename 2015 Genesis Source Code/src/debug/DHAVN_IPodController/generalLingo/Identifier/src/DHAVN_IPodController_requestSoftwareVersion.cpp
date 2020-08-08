/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS:  Neelima Kasam                                    *
 * DATE       :  08 February 2012                                 *
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
#include <QByteArray>
#include <DHAVN_IPodController_requestSoftwareVersion.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>


using namespace GeneralLingo;

CRequestSoftwareVersion::CRequestSoftwareVersion(int transId, bool isTidEnabled)
        :CIAPPacket(0x00,0x0009,transId, isTidEnabled)
{
}

QByteArray CRequestSoftwareVersion::marshall()
{
    QByteArray command;
    int i = -1;
    //Packing format =  Big Endian
    command[++i] = 0x55;
    command[++i] = 0x05;
    command[++i] = 0x00;
    command[++i] = 0x09;
    if( m_isTidEnabled )
    {
        command[++i] = ((m_TransactionID & 0xff00) >> 8);
        command[++i] = m_TransactionID & 0x00ff;
    }
    command[1] = i-1;
    command.append(CIpodControllerGlobal::checkSum(command, i));
    m_CommandBytes = command;
    return command;
}

void  CRequestSoftwareVersion::unmarshall(QByteArray)
{
}

