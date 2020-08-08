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
#include <DHAVN_IPodController_getDBiTunesInfo.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace ExtendedLingo;

CGetDBiTunesInfo::CGetDBiTunesInfo(int infoType, int transId, bool isTidEnabled)
:CIAPPacket(0x04,0x003C,transId,isTidEnabled)
{
    m_infoType = infoType;
}

QByteArray CGetDBiTunesInfo::marshall()
{
        QByteArray command;
        int i = -1;
        //Packing format =  Big Endian
	command[++i] = 0x55;
        command[++i] = 0x04;
	command[++i] = 0x04;
	command[++i] = 0x00;
        command[++i] = 0x3C;

        if( m_isTidEnabled )
	{
            command[++i] = ((m_TransactionID & 0xff00) >> 8);
            command[++i] = m_TransactionID & 0x00ff;
	}

        command[++i] = m_infoType;
	command[1] = i-1;
	command.append(CIpodControllerGlobal::checkSum(command, i));
	m_CommandBytes = command;
	return command;
}

void  CGetDBiTunesInfo::unmarshall(QByteArray)
{
}



