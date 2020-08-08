/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                    General and extended lingo command class to *
 *                    unmarsal the Ack command                    *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_StorageLingoiPodAck.h>
using namespace StorageLingo;

QByteArray CIPodStorageLingoAck::marshall()
{
    return NULL;
}

void CIPodStorageLingoAck::unmarshall(QByteArray returnCommand)
{
	if( ((int)returnCommand[0] == 0x55) &&
			((int)returnCommand[2] == 0x0C) &&
			((int)returnCommand[3] == 0x00) )
	{
		if((int)returnCommand[1] == 7)
		{
			m_AckCommand = returnCommand[7];
			m_AckStatus = returnCommand[6];
			m_TransID = 0x0000;
			m_TransID = m_TransID | returnCommand[4];
			m_TransID = m_TransID <<8;
			m_TransID = m_TransID | returnCommand[5];
		}
		else
		{
			m_AckCommand = returnCommand[5];
			m_AckStatus = returnCommand[4];
		}
	}
}

/******************************************************************************
// Function Name                                            Parameters
// CIPodStorageLingoAck::getAcknowledgedCommand()         (int) (OUT), (void) (IN)
//
// Explanation
// return the ack command which was sent
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIPodStorageLingoAck::getAcknowledgedCommand()
{
    return m_AckCommand;
}

/******************************************************************************
// Function Name                                            Parameters
// CIPodStorageLingoAck::getAckStatus                                      (int) (OUT), (void) (IN)
//
// Explanation
// return the command status of the sent command
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
****************************************************************************/
int CIPodStorageLingoAck::getAckStatus()
{
    return m_AckStatus;
}

int CIPodStorageLingoAck::getTransactionID()
{
    return m_TransID;
}
