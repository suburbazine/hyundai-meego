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
#include <DHAVN_IPodController_getIndexedPlayingTrackAlbum.h>
#include <QByteArray>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace ExtendedLingo;

CGetIndexedPlayingTrackAlbumName::CGetIndexedPlayingTrackAlbumName(int nIndex,
                                                                int nTransId,  bool isTidEnabled)
                                                        :       CIAPPacket(0x04,0x0024,nTransId, isTidEnabled), m_Index(nIndex)
{

}

QByteArray CGetIndexedPlayingTrackAlbumName::marshall()
{
        QByteArray command;
	int i = -1;
 //  qDebug() << "In CGetIndexedPlayingTrackAlbumName::marshall \n ";
        //Packing format =  Big Endian
        //    command.resize(11);
            command[++i] = 0x55;
            command[++i] = 0x09;
            command[++i] = 0x04;
            command[++i] = 0x00;
            command[++i] = 0x24;
                if( m_isTidEnabled )
		{
                    command[++i] = ((m_TransactionID & 0xff00) >> 8);
                    command[++i] = m_TransactionID & 0x00ff;
		}

                command[i+4] = m_Index & 0x00FF ;
                for(int j = (i+3);j >=(i+1);j--)
                {
                    m_Index = m_Index >> 8;
                    command[j] = m_Index & 0x00FF ;
                }
		i = i + 4;
		command[1] = i-1;
		command.append(CIpodControllerGlobal::checkSum(command, i));
        m_CommandBytes = command;
        return command;
}


void  CGetIndexedPlayingTrackAlbumName::unmarshall(QByteArray)
{
}




