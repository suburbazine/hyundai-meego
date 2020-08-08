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
#include <DHAVN_IPodController_requestiPodSerialNumber.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>


using namespace GeneralLingo;

CRequestiPodSerialNumber::CRequestiPodSerialNumber( int transId, bool isTidEnabled)
        :CIAPPacket(0x00,0x0B,transId, isTidEnabled)
{
}

QByteArray CRequestiPodSerialNumber::marshall()
{
    QByteArray command;
   // //LOG_TRACE<<"In CRequestiPodSerialNumber::marshall \n "<<LOG_ENDL;
    int i = -1;
    //Packing format =  Big Endian
    command[++i] = 0x55;
    command[++i] = 0x02;
    command[++i] = 0x00;
    command[++i] = 0x0B;
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

void  CRequestiPodSerialNumber::unmarshall(QByteArray)
{
}

