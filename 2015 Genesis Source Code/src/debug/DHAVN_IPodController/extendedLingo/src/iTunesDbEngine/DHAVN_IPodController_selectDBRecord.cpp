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
#include <DHAVN_IPodController_selectDBRecord.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace ExtendedLingo;

CSelectDBRecord::CSelectDBRecord(QString categoryValue,
                        int indexValue,int transId,  bool isTidEnabled)
                        :CIAPPacket(0x04,0x0017,transId, isTidEnabled), m_Category(categoryValue),m_Index(indexValue)
{

}

QByteArray CSelectDBRecord::marshall()
{
        QByteArray command;
        //qDebug() << "In CSelectDBRecord::marshall \n ";
        //Packing format =  Big Endian

        if(!m_Category.isEmpty()){
	int i = -1;
            command[++i] = 0x55;
            command[++i] = 0x0A;
            command[++i] = 0x04;
            command[++i] = 0x00;
            command[++i] = 0x17;
                if( m_isTidEnabled )
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
            int indexValue =  m_Index;
            //Byte 7,8,9,10 is the start m_Index int value . 10 holds the LSB(bits 7:0)
                command[i+4] = indexValue & 0x00FF ;
                for(int j = (i+3);j >=(i+1);j--)
                {
                    indexValue = indexValue >> 8;
                    command[j] = indexValue & 0x00FF ;
                }
		i = i + 4;
		command[1] = i-1;
		command.append(CIpodControllerGlobal::checkSum(command, i));
        }
        m_CommandBytes = command;
        return command;
}
/*
void CSelectDBRecord::setCategoryAndIndex( QString categoryValue,int indexValue)
{
    m_Category = categoryValue;
    m_Index  = indexValue;
}
*/
void  CSelectDBRecord::unmarshall(QByteArray )
{
}




