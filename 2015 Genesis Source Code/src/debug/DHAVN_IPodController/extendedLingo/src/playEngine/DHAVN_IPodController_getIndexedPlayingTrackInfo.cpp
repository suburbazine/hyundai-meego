
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
#include <DHAVN_IPodController_getIndexedPlayingTrackInfo.h>
#include <QByteArray>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace ExtendedLingo;

CGetIndexedPlayingTrackInfo::CGetIndexedPlayingTrackInfo(int nIndex,int nInfoType,
                                                                int nTransId,  bool isTidEnabled)
                                                        :       CIAPPacket(0x04,0x000C,nTransId, isTidEnabled), m_Index(nIndex),m_InfoType(nInfoType)
{

}

QByteArray CGetIndexedPlayingTrackInfo::marshall()
{
        QByteArray command;
	int i = -1;
   //qDebug() << "In CGetIndexedPlayingTrackInfo::marshall \n ";
        //Packing format =  Big Endian
//            command.resize(15);
            command[++i] = 0x55;
            command[++i] = 0x0C;
            command[++i] = 0x04;
            command[++i] = 0x00;
            command[++i] = 0x0C;
                if( m_isTidEnabled )
		{
                    command[++i] = ((m_TransactionID & 0xff00) >> 8);
                    command[++i] = m_TransactionID & 0x00ff;
		}
            switch(m_InfoType)
            {
            case TYPEINFO:
            case TRACKDURATIONINFO:
                {
                    command[++i] = 0x00;
                    break;
                }
            case GENREINFO:

                {
                    command[++i] = 0x05;
                    break;
                }
            case COMPOSERINFO:
                {
                    command[++i] = 0x06;
                    break;
                }
            case ARTWORKCOUNT:
                    command[++i] = 0x07;
                    break;
            default:
                    command[++i] = 0x00;
            }
                command[i+4] = m_Index & 0x00FF ;
                for(int j = (i+3);j >=(i+1);j--)
                {
                    m_Index = m_Index >> 8;
                    command[j] = m_Index & 0x00FF ;
                }
		i = i + 4;
		command[++i] = 0x00;
		command[++i] = 0x00;
		command[1] = i-1;
		command.append(CIpodControllerGlobal::checkSum(command, i));
        m_CommandBytes = command;
        return command;
}


void  CGetIndexedPlayingTrackInfo::unmarshall(QByteArray)
{
}




