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
#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_playCurrentSelection.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
CPlayCurrentSelection::CPlayCurrentSelection(int index,int transId,bool isTidEnabled)
    :CIAPPacket(0x04,0x0028,transId, isTidEnabled),m_CurrentPlayingTrackIndex(index)
{
//LOG_INIT_V2(gControllerLogName);
}

QByteArray CPlayCurrentSelection::marshall()
{
	QByteArray command;
	//    //LOG_TRACE << "In CPlayCurrentSelection::marshall " << LOG_ENDL;
	//Packing format =  Big Endian
	int i = -1;
	//    command[0] = 0x00; //not required for USB.Hence omitted here
	command[++i] = 0x55;
	command[++i] = 0x09;
	command[++i] = 0x04;
	command[++i] = 0x00;
	command[++i] = 0x28;
	if( m_isTidEnabled )
	{
		command[++i] = ((m_TransactionID & 0xff00) >> 8);
		command[++i] = m_TransactionID & 0x00ff;
	}
	command[i+4] = m_CurrentPlayingTrackIndex & 0x00FF ;
	for(int j = (i+3);j >=(i+1);j--)
	{
		m_CurrentPlayingTrackIndex = m_CurrentPlayingTrackIndex >> 8;
		command[j] = m_CurrentPlayingTrackIndex & 0x00FF ;
	}
	i = i + 4;
	command[1] = i-1;
	command.append(CIpodControllerGlobal::checkSum(command, i));
	m_CommandBytes = command;
	return command;
}

void  CPlayCurrentSelection::unmarshall(QByteArray)
{

}
