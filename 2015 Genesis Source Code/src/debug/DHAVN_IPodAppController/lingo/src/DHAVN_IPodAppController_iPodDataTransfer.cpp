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
#include <DHAVN_IPodAppController_iPodDataTransfer.h>

/******************************************************************************


//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CIPodDataTransfer::marshall()
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
void CIPodDataTransfer::unmarshall(const QByteArray& returnCommand)
{
    //large packet
    if(((int)returnCommand[0] == 0x55) &&
            ((int)returnCommand[1] == 0x00) &&
            (int)returnCommand[5] == 0x43)
    {
        m_Len = 0x0000;
        m_Len = m_Len | returnCommand[2];
        m_Len = m_Len <<8;
        m_Len = m_Len | (returnCommand[3] & 0xFF);


        m_TransactionID = 0x0000;
        m_TransactionID = m_TransactionID | returnCommand[6];
        m_TransactionID = m_TransactionID <<8;
        m_TransactionID = m_TransactionID | (returnCommand[7] & 0xFF );

        m_DataArray.clear();
        qint16 sessionId = 0;
        for(int i = 8;i<= 9;i++)
        {
            sessionId <<= 8;     sessionId |= (int)returnCommand[i] & 0xFF;
        }
        if(sessionId == m_SessionId)
        {
            m_DataArray.append(returnCommand.mid(10, (returnCommand.count()-1)));
        }
    }
    //small packet
    else if( ((int)returnCommand[0] == 0x55) &&
             ((int)returnCommand[2] == 0x00) &&
             ((int)returnCommand[3] == 0x43) )
    {

        m_Len = (int)returnCommand[1];

        m_TransactionID = 0x0000;
        m_TransactionID = m_TransactionID | returnCommand[4];
        m_TransactionID = m_TransactionID <<8;
        m_TransactionID = m_TransactionID | (returnCommand[5] & 0xFF);

        m_DataArray.clear();
        qint16 sessionId = 0;
        for(int i = 6;i<= 7;i++)
        {
            sessionId <<= 8;     sessionId |= (int)returnCommand[i] & 0xFF;
        }
        if(sessionId == m_SessionId)
        {
            m_DataArray.append(returnCommand.mid(8, (returnCommand.count()-1)));
        }
    }
}

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CIPodDataTransfer::getData()
{
    return m_DataArray;
}
