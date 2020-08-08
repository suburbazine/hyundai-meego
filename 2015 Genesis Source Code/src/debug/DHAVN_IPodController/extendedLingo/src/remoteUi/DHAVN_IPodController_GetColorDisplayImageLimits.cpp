/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  10 Oct 2011                                   *
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
#include <DHAVN_IPodController_GetColorDisplayImageLimits.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace ExtendedLingo;

CGetColorDisplayImageLimits::CGetColorDisplayImageLimits(int transId, bool isTidEnabled)
{
        //cout << "in CGetCurrentPlayingTrackIndex constructor\n" ;
    m_TransactionID = transId;
    m_isTidEnabled = isTidEnabled;
}

QByteArray CGetColorDisplayImageLimits::marshall()
{
    QByteArray command;
    int i = -1;

    //Packing format =  Big Endian
     //   command.resize(8);
        command[++i] = 0x55;
        command[++i] = 0x00;
        command[++i] = 0x04;//ExtendedLingo
        command[++i] = 0x00;
        command[++i] = 0x39;//GetColorDisplayImageLimits
        if( m_isTidEnabled ){
            command[++i] = ((m_TransactionID & 0xff00) >> 8);
            command[++i] = m_TransactionID & 0x00ff;
        }
        command[1] = i - 1;
	command.append(CIpodControllerGlobal::checkSum(command, i));

    m_CommandBytes = command;
    return command;
}

void  CGetColorDisplayImageLimits::unmarshall(QByteArray)
{

}
