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
#include <DHAVN_IPodController_getNumberCategorizedDatabaseRecords.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace ExtendedLingo;

CGetNumberCategorizedDatabaseRecords::CGetNumberCategorizedDatabaseRecords(int transId, bool isTidEnabled )
    :CIAPPacket(0x04,0x0018,transId, isTidEnabled)
{
#ifdef DBUGLOGS
    LOG_INIT_V2(gControllerLogName);
#endif
}

CGetNumberCategorizedDatabaseRecords::CGetNumberCategorizedDatabaseRecords(QString categoryValue,int transId, bool isTidEnabled)
            :CIAPPacket(0x04,0x0018,transId, isTidEnabled)
{
        m_Category = categoryValue;
}
/*
void CGetNumberCategorizedDatabaseRecords::setCategory(QString categoryValue)
{
        m_Category = categoryValue;
}
*/
QByteArray CGetNumberCategorizedDatabaseRecords::marshall()
{
	QByteArray command;
#ifdef DBUGLOGS
    //LOG_TRACE << "In CGetNumberCategorizedDatabaseRecords::marshall " << LOG_ENDL;
#endif
	//Packing format =  Big Endian
        if(!m_Category.isEmpty()){
		int i = -1;
            command[++i] = 0x55;
            command[++i] = 0x06;
            command[++i] = 0x04;
            command[++i] = 0x00;
            command[++i] = 0x18;

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
	    command[1] = i-1;
	    command.append(CIpodControllerGlobal::checkSum(command, i));
        }
        m_CommandBytes = command;
        return command;
}

void  CGetNumberCategorizedDatabaseRecords::unmarshall(QByteArray)
{

}
