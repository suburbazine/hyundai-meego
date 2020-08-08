/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
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
#include <DHAVN_IPodController_ack.h>

QByteArray CACK::marshall()
{
    return NULL;
}

void CACK::unmarshall(QByteArray returnCommand)
{
	if( ((int)returnCommand[0] == 0x55) &&
			((int)returnCommand[2] == 0x00) &&
			((int)returnCommand[3] == 0x02) )
	{
		if((int)returnCommand[1] == 6)
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
                        m_TransID = 0x0000; // added by sungha.choi 2013.03.17 (no support transaction id)
		}
	}
        else if( ((int)returnCommand[0] == 0x55) &&
                   ((int)returnCommand[2] == 0x03) &&      // display remote lingo
                   ((int)returnCommand[3] == 0x00))
        {
            if((int)returnCommand[1] == 6)
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
				    m_TransID = 0x0000; // added by sungha.choi 2013.03.17 (no support transaction id)
            }
        }
	else
	{
		if( ((int)returnCommand[0] == 0x55) &&
				((int)returnCommand[1] == 0x08) &&
				((int)returnCommand[2] == 0x04) &&
				((int)returnCommand[3] == 0x00) &&
				((int)returnCommand[4] == 0x01) )
		{
			m_TransID = 0x0000;
			m_TransID = m_TransID | returnCommand[5];
			m_TransID = m_TransID <<8;
			m_TransID = m_TransID | returnCommand[6];
			m_AckStatus = returnCommand[7];
			qint16 command = 0;
			command = 0x0000;
			command = command | returnCommand[8];
			command = command <<8;
			command = command | returnCommand[9];
			m_AckCommand = 0x00  | command;
		}
		else
		{
			qint16 command = 0;
			command = 0x0000;
			command = command | returnCommand[6];
			command = command <<8;
			command = command | returnCommand[7];
			m_AckCommand = 0x00  | command;
			m_AckStatus = returnCommand[5];
                        m_TransID = 0x0000; // added by sungha.choi 2013.03.17 (no support transaction id)
		}
	}
}

/******************************************************************************
// Function Name                                            Parameters
// CACK::getAcknowledgedCommand()         (int) (OUT), (void) (IN)
//
// Explanation
// return the ack command which was sent
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CACK::getAcknowledgedCommand()
{
    return m_AckCommand;
}

/******************************************************************************
// Function Name                                            Parameters
// CACK::getAckStatus                                      (int) (OUT), (void) (IN)
//
// Explanation
// return the command status of the sent command
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
****************************************************************************/
int CACK::getAckStatus()
{
    return m_AckStatus;
}

int CACK::getTransactionID()
{
    return m_TransID;
}
