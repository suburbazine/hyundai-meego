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
#include <DHAVN_IPodController_returnNumberCategorizedDatabaseRecords.h>
#include <QByteArray>
using namespace ExtendedLingo;

CReturnNumberCategorizedDatabaseRecords::CReturnNumberCategorizedDatabaseRecords( bool isTidEnabled)
    :CIAPPacket(0x04,0x0019,-1, isTidEnabled), m_RecordCount(0)
{
}

QByteArray CReturnNumberCategorizedDatabaseRecords::marshall()
{
        QByteArray command;
        return command;
}

int CReturnNumberCategorizedDatabaseRecords::getCount()
{
    return m_RecordCount;
}

void  CReturnNumberCategorizedDatabaseRecords::unmarshall(QByteArray returnCommand)
{
    //    DBGLOGFun2("CReturnNumberCategorizedDatabaseRecords unmarshall done brfore play IN");
    if( ((int)returnCommand[0] == 0x55) &&
        ((int)returnCommand[2] == 0x04) &&
        ((int)returnCommand[3] == 0x00) &&
        ((int)returnCommand[4] == 0x19) ){
            int value = 0;
            int nStartIndex = -1;
            int nEndIndex = -1;
            if( m_isTidEnabled  )
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
            m_RecordCount = value;
  }
        //DBGLOGFun("CReturnNumberCategorizedDatabaseRecords unmarshall done brfore pla OUT");
}

