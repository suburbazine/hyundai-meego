/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Sowmini Philip       *
 * DATE       :  12 November 2011                                   *
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
#include <DHAVN_IPodController_requestLingoProtocolVersion.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace GeneralLingo;

CRequestLingoProtocolVersion::CRequestLingoProtocolVersion( int lingo,int transId, bool isTidEnabled)
        :CIAPPacket(CIAPCmds::GeneralLingoId ,CIAPCmds::L0_0F_RequestLingoProtocolVersion,transId, isTidEnabled),m_ReqLingoId(lingo)
{
}

QByteArray CRequestLingoProtocolVersion::marshall()
{
    QByteArray command;
    int i = -1;
    //Packing format =  Big Endian
    command[++i] = 0x55;
    command[++i] = 0x05;
    command[++i] = 0x00;
    command[++i] = 0x0F;
    if( m_isTidEnabled )
    {
        command[++i] = ((m_TransactionID & 0xff00) >> 8);
        command[++i] = m_TransactionID & 0x00ff;
    }
    command[++i] = 0xFFFFFFFF& m_ReqLingoId;
    command[1] = i-1;
    command.append(CIpodControllerGlobal::checkSum(command, i));
    m_CommandBytes = command;
    return command;
}

void  CRequestLingoProtocolVersion::unmarshall(QByteArray)
{
}

