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
#include <DHAVN_IPodController_returnCategorizedDatabaseRecords.h>
#include <QByteArray>
using namespace ExtendedLingo;

CReturnCategorizedDatabaseRecords::CReturnCategorizedDatabaseRecords( bool isTidEnabled)
    :CIAPPacket(0x04,0x001B,-1, isTidEnabled), m_RecordString("")
{
    m_RecordIndex = -1; //uninitialized variable - coverity fix
}

QByteArray CReturnCategorizedDatabaseRecords::marshall()
{
        QByteArray command;
        return command;
}

QString CReturnCategorizedDatabaseRecords::getRecordString()
{
    return m_RecordString.fromUtf8(m_RecordString.toAscii().data());
}

int CReturnCategorizedDatabaseRecords::getRecordIndex()
{
    return m_RecordIndex;
}


void  CReturnCategorizedDatabaseRecords::unmarshall(QByteArray returnCommand)
{
    // Remove the checksum byte from the array in order to use mid to obtain the record string
    //chop(1) removes 1 byte from the end of the array
    returnCommand.chop(1);
    if( ((int)returnCommand[0] == 0x55) &&
        ((int)returnCommand[2] == 0x04) &&
        ((int)returnCommand[3] == 0x00) &&
        ((int)returnCommand[4] == 0x1B) ){
                int value = 0;
                int nStartIndex = -1;
                int nEndIndex = -1;

            if( m_isTidEnabled )
            {
                    m_TransactionID = 0x0000;
                    m_TransactionID = m_TransactionID | returnCommand[5];
                    m_TransactionID = m_TransactionID <<8;
                    m_TransactionID = m_TransactionID | returnCommand[6];
            }
            else
                    m_TransactionID = 6;
                if( m_isTidEnabled )
                {
                        nStartIndex = 7;
                        nEndIndex = 11;
                }
                else
                {
                        nStartIndex = 5;
                        nEndIndex = 9;
                }
                for(int i = nStartIndex;i< nEndIndex;i++)
                {
                        value <<= 8;     value |= (int)returnCommand[i] & 0xFF;
        }
    m_RecordIndex = value;
    //qDebug() <<  "Record index =  " << m_RecordIndex;
    m_RecordString = QString(returnCommand.mid(nEndIndex,-1)); // bytearray from 12 to the end
    //qDebug() <<  "Record string =  " << m_RecordString;
  }
}

