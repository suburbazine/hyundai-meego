/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
 *
 * PROGRAMMERS : Sowmini Philip      *
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
#include <DHAVN_IPodController_playPreparedUIDList.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include<QList>
#include<QString>

CPlayPreparedUIDList::CPlayPreparedUIDList(int transId,QByteArray startUID,bool isTidEnabled)
    :CIAPPacket(0x04,0x004B,transId,isTidEnabled)
{
	m_startUIDBytes.clear();
	m_startUIDBytes.append(startUID);
}

QByteArray CPlayPreparedUIDList::marshall()
{
	QByteArray command;

	//Packing format =  Big Endian
	//command.resize(11);
	int i = -1;

	command[++i] = 0x55;
	command[++i] = 0x07;
	command[++i] = 0x04;
	command[++i] = 0x00;
	command[++i] = 0x4B;
	if( m_isTidEnabled )
	{
		command[++i] = ((m_TransactionID & 0xff00) >> 8);
		command[++i] = m_TransactionID & 0x00ff;
	}
	command[++i] = 0x00; //reserved
	command.append(m_startUIDBytes);
	i = i + m_startUIDBytes.size();
	command[1] = i-1;
	command.append(CIpodControllerGlobal::checkSum(command, i));
	m_CommandBytes = command;
	return command;
}

void  CPlayPreparedUIDList::unmarshall(QByteArray )
{
}




