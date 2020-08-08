/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
 *
 * PROGRAMMERS : Pawan Gupta/Sowmini Philip/Venkataramana/Mohana  *
 * DATE       :  26 JUNE 2012                                     *
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
#include <DHAVN_IPodController_setCurrentPlayingTrack.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace ExtendedLingo;

CSetCurrentPlayingTrack::CSetCurrentPlayingTrack(int index,int transId,  bool isTidEnabled)
    :CIAPPacket(0x04,0x001E,transId, isTidEnabled), m_newCurrentPlayingtrackIndex(index)
{

}

QByteArray CSetCurrentPlayingTrack::marshall()
{
    QByteArray command;
    int i = -1;



    command[++i] = 0x55;
    command[++i] = 0x05;
    command[++i] = 0x04;
    command[++i] = 0x00;
    command[++i] = 0x37;

    if( m_isTidEnabled )
    {

        command[++i] = ((m_TransactionID & 0xff00) >> 8);
        command[++i] = m_TransactionID & 0x00ff;
        /*
        command[++i] = m_TransactionID & 0x00FF;
        m_TransactionID = m_TransactionID >> 8;
        command[++i] =  m_TransactionID & 0x00FF;
        */
    }



    command[i+4] = m_newCurrentPlayingtrackIndex & 0x00FF ;
    for(int j = (i+3);j >=(i+1);j--)
    {
        m_newCurrentPlayingtrackIndex = m_newCurrentPlayingtrackIndex >> 8;
        command[j] = m_newCurrentPlayingtrackIndex & 0x00FF ;
    }
    i = i + 4;

    command[1] = i-1;
    command.append(CIpodControllerGlobal::checkSum(command, i));

    m_CommandBytes = command;
    return command;
}

void  CSetCurrentPlayingTrack::unmarshall(QByteArray)
{

}


