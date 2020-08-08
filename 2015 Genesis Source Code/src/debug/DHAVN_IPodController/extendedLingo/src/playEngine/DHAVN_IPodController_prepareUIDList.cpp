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
#include <DHAVN_IPodController_prepareUIDList.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include<QList>
#include<QString>

CPrepareUIDList::CPrepareUIDList(int sectCurrent,int sectMax,int transId,QByteArray& UIDs,bool isTidEnabled)
    :CIAPPacket(0x04,0x004A,transId,isTidEnabled),m_SectCurrent(sectCurrent),m_SectMax(sectMax)
{
	m_UIDBytes.clear();
	m_UIDBytes.append(UIDs);
}

QByteArray CPrepareUIDList::marshall()
{
    QByteArray command;

    //Packing format =  Big Endian

    //command.resize(11);
	int i = -1;

    command[++i] = 0x55;
    command[++i] = 0x07;
    command[++i] = 0x04;
    command[++i] = 0x00;
    command[++i] = 0x4A;
	if( m_isTidEnabled )
	{
        command[++i] = ((m_TransactionID & 0xff00) >> 8);
        command[++i] = m_TransactionID & 0x00ff;
	}
    command[i+2] = m_SectCurrent & 0x00FF ;
    m_SectCurrent = m_SectCurrent >> 8;
    command[++i] = m_SectCurrent & 0x00FF ;
    i = i + 1;
    command[i+2] = m_SectMax & 0x00FF ;
    m_SectMax = m_SectMax >> 8;
    command[++i] = m_SectMax & 0x00FF ;
    i = i + 1;
    command.append(m_UIDBytes);
	i = i + m_UIDBytes.size();
	command[1] = i-1;
	command.append(CIpodControllerGlobal::checkSum(command, i));
    m_CommandBytes = command;
    return command;
}

void  CPrepareUIDList::unmarshall(QByteArray )
{
}




