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
#include <DHAVN_IPodController_getCurrentPlayingTrackIndex.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace ExtendedLingo;

CGetCurrentPlayingTrackIndex::CGetCurrentPlayingTrackIndex(int transId,  bool isTidEnabled)
    :CIAPPacket(0x04,0x001E,transId, isTidEnabled)
{
        //cout << "in CGetCurrentPlayingTrackIndex constructor\n" ;
}

QByteArray CGetCurrentPlayingTrackIndex::marshall()
{
    QByteArray command;
int i = -1;

    //Packing format =  Big Endian
        //command.resize(8);
        command[++i] = 0x55;
        command[++i] = 0x05;
        command[++i] = 0x04;
        command[++i] = 0x00;
        command[++i] = 0x1E;
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

void  CGetCurrentPlayingTrackIndex::unmarshall(QByteArray)
{

}

