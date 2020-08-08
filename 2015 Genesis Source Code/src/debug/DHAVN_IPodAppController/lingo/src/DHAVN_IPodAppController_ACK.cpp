/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :  IPodAppController                                *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  16 jan 2012                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodAppController_ACK.h>

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CACK::marshall()
{
    return NULL;
}

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CACK::unmarshall(const QByteArray& returnCommand)
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
// 06-jan-2012     Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CACK::getAcknowledgedCommand()
{
    return m_AckCommand;
}
