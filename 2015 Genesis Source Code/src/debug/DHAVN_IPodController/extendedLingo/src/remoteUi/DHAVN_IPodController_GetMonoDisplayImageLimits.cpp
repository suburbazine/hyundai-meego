/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Mohana/ Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  17 Apr 2012                                   *
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
#include "DHAVN_IPodController_GetMonoDisplayImageLimits.h"
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace ExtendedLingo;

CGetMonoDisplayImageLimits::CGetMonoDisplayImageLimits(int transId, bool isTidEnabled)
{        
    m_TransactionID = transId;
    m_isTidEnabled = isTidEnabled;
}

QByteArray CGetMonoDisplayImageLimits::marshall()
{
    QByteArray command;
    int i = -1;

    //Packing format =  Big Endian
     //   command.resize(8);
        command[++i] = 0x55;
        command[++i] = 0x00;
        command[++i] = 0x04;//ExtendedLingo
        command[++i] = 0x00;
        command[++i] = 0x33;//GetMonoDisplayImageLimits
        if( m_isTidEnabled ){
            command[++i] = (m_TransactionID & 0x0000FF00)>>8;
            command[++i] =  m_TransactionID & 0x00FF;
        }
        command[1] = i - 1;
        command.append(CIpodControllerGlobal::checkSum(command, i));

    m_CommandBytes = command;
    return command;
}

void  CGetMonoDisplayImageLimits::unmarshall(QByteArray)
{

}

