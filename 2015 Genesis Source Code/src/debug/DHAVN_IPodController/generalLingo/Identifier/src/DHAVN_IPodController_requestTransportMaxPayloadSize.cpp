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
#include <DHAVN_IPodController_requestTransportMaxPayloadSize.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>


using namespace GeneralLingo;

CRequestTransportMaxPayloadSize::CRequestTransportMaxPayloadSize( int transId, bool isTidEnabled)
        :CIAPPacket(CIAPCmds::GeneralLingoId ,CIAPCmds::L0_11_RequestTransportMaxPayloadSize,transId, isTidEnabled)
{
}

QByteArray CRequestTransportMaxPayloadSize::marshall()
{
    QByteArray command;
    int i = -1;
    //Packing format =  Big Endian
    command[++i] = 0x55;
    command[++i] = 0x05;
    command[++i] = CIAPCmds::GeneralLingoId ;
    command[++i] = CIAPCmds::L0_11_RequestTransportMaxPayloadSize;
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

void  CRequestTransportMaxPayloadSize::unmarshall(QByteArray)
{
}

