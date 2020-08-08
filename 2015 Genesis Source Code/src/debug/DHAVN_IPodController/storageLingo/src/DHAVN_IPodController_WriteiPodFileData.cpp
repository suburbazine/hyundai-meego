/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Sowmini Philip / Venkataramana      *
 * DATE       :  2 Jan 2012                                   
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
#include <DHAVN_IPodController_WriteiPodFileData.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace StorageLingo;

CWriteiPodFileData::CWriteiPodFileData(int transId, int handle, int offset,int dataSize,QByteArray data,  bool isTidEnabled)
{
        //cout << "in CGetCurrentPlayingTrackIndex constructor\n" ;
    m_TransactionID = transId;
    m_dataOffset = offset;
    m_dataSize = dataSize;
    m_plistData = data;
    m_Handle = handle;
    m_isTidEnabled = isTidEnabled;
}

QByteArray CWriteiPodFileData::marshall()
{
    QByteArray command;

    int i = -1;//Array index

    command[++i] = 0x55;
    command[++i] = 0x00;
    command[++i] = 0x0C;//Storagelingo
    command[++i] = 0x07;
    if( m_isTidEnabled ){
	    command[++i] = (m_TransactionID & 0xFF00) >> 8;
	    command[++i] =  m_TransactionID & 0x00FF;
    }

    command[i+4] = m_dataOffset & 0x00FF ;
    for(int j = (i+3);j >=(i+1);j--)
    {
	    m_dataOffset = m_dataOffset >> 8;
	    command[j] = m_dataOffset & 0x00FF ;
    }
    i = i + 4;
    command[++i] = (char)m_Handle & 0xFF; 

    command.append(m_plistData,m_dataSize);
    i += m_dataSize;
    command[1] = i - 1;

    command[i+1] = CIpodControllerGlobal::checkSum(command,i);
    m_CommandBytes = command;
    return command;
}

void  CWriteiPodFileData::unmarshall(QByteArray)
{

}
