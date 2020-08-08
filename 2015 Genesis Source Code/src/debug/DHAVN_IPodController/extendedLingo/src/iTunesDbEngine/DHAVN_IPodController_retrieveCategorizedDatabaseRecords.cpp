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
#include <DHAVN_IPodController_retrieveCategorizedDatabaseRecords.h>
#include <QByteArray>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace ExtendedLingo;

CRetrieveCategorizedDatabaseRecords::CRetrieveCategorizedDatabaseRecords(QString categoryValue,
                                                                int startRecordValue,
                                                                int readCountValue,
                                                                int transId,  bool isTidEnabled)
:CIAPPacket(0x04,0x0018,transId, isTidEnabled), m_Category(categoryValue),m_StartRecord(startRecordValue),m_ReadCount(readCountValue)
{
}

QByteArray CRetrieveCategorizedDatabaseRecords::marshall()
{
        QByteArray command;
        int startIndex = m_StartRecord;

///    qDebug() << "In CRetrieveCategorizedDatabaseRecords::marshall \n ";
        //Packing format =  Big Endian
        if(!m_Category.isEmpty()){
		int i = -1;
            command[++i] = 0x55;
            command[++i] = 0x0E;// 0x0C;
            command[++i] = 0x04;
            command[++i] = 0x00;
            command[++i] = 0x1A;
            if( m_isTidEnabled  )
	    {
                command[++i] = ((m_TransactionID & 0xff00) >> 8);
                command[++i] = m_TransactionID & 0x00ff;
	    }
            if(!m_Category.compare("PlayList",Qt::CaseInsensitive))
                command[++i] = 0x01 ;
            else if(!m_Category.compare("Artist",Qt::CaseInsensitive))
                command[++i] = 0x02;
            else if(!m_Category.compare("Album",Qt::CaseInsensitive))
                command[++i] = 0x03;
            else if(!m_Category.compare("Genre",Qt::CaseInsensitive))
                command[++i] = 0x04;
            else if(!m_Category.compare("Track",Qt::CaseInsensitive))
                command[++i] = 0x05;
            else if(!m_Category.compare("Composer",Qt::CaseInsensitive))
                command[++i] = 0x06;
            else if(!m_Category.compare("Audiobook",Qt::CaseInsensitive))
                command[++i] = 0x07;
            else if(!m_Category.compare("Podcast",Qt::CaseInsensitive))
                command[++i] = 0x08;
            else if(!m_Category.compare("iTunesU",Qt::CaseInsensitive))
                command[++i] = 0x0B;

            //Byte 9,10,11,12 is the start index int value . 10 holds the LSB(bits 7:0)
                command[i+4] = startIndex & 0x00FF ;
                for(int j = (i+3);j >=(i+1);j--)
                {
                    startIndex = startIndex >> 8;
                    command[j] = startIndex & 0x00FF ;
                }
		i = i + 4;
            //Byte 13,14,15,16 is the start index int value. 14 holds the LSB(bits 7:0)
                command[i+4] = m_ReadCount & 0x00FF ;
                for(int j = (i+3);j >=(i+1);j--)
                {
                    m_ReadCount = m_ReadCount >> 8;
                    command[j] = m_ReadCount & 0x00FF ;
                }
		i = i + 4;
		command[1] = i-1;
		command.append(CIpodControllerGlobal::checkSum(command, i));
        }
        m_CommandBytes = command;
        return command;
}

void CRetrieveCategorizedDatabaseRecords::setParameters( QString categoryValue,int startRecordValue,int readCountValue)
{
    m_Category = categoryValue;
    m_StartRecord = startRecordValue;
    m_ReadCount = readCountValue;
}

void  CRetrieveCategorizedDatabaseRecords::unmarshall(QByteArray)
{
}



