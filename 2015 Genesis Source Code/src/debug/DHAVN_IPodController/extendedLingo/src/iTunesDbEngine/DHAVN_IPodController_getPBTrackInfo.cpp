/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
 *
 * PROGRAMMERS : Sowmini Philip / Venkataramana      *
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
#include <DHAVN_IPodController_getPBTrackInfo.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace ExtendedLingo;

CGetPBTrackInfo::CGetPBTrackInfo(int startRecordValue,
                                                int readCountValue,
                                                int transId, bool isTidEnabled)
                                                    :CIAPPacket(0x04,0x0042,transId,isTidEnabled),
                                                m_StartIndex(startRecordValue),m_TrackCount(readCountValue),
                                                      m_Mask(0xFF) //added by junam 2013.04.13 for AutoStart
{

}

QByteArray CGetPBTrackInfo::marshall()
{
        QByteArray command;
        int start = m_StartIndex;
	int i = -1;
        //Packing format =  Big Endian
	command[++i] = 0x55;
	command[++i] = 0x0C;
	command[++i] = 0x04;
	command[++i] = 0x00;
	command[++i] = 0x42;

        if( m_isTidEnabled )
	{
            command[++i] = ((m_TransactionID & 0xff00) >> 8);
            command[++i] = m_TransactionID & 0x00ff;
	}
	//Byte 6,7,8,9 is the start index int value . 9 holds the LSB(bits 7:0)
	command[i+4] = start & 0x00FF ;
	for(int j = (i+3);j >=(i+1);j--)
	{
		start = start >> 8;
		command[j] = start & 0x00FF ;
	}
	i = i + 4;
	//Byte 10,11,12,13 is the count int value. 13 holds the LSB(bits 7:0)
	command[i+4] = m_TrackCount & 0x00FF ;
	for(int j = (i+3);j >=(i+1);j--)
	{
		m_TrackCount = m_TrackCount >> 8;
                command[j] = m_TrackCount & 0x00FF ;
	}
	i = i + 4;
//        command[++i] = 0xBF;
        //Modified from 0x7E to 0xFF for CR 13014 S
        //{changed by junam 2013.04.13 for AutoStart
	//command[++i] = 0xFF; // bit mask with capabilities, track name,album ,artist,genre,composer info, UID bits set to 1 (11111111)
        command[++i] = m_Mask;
        //}changed by junam
        //Modified from 0x7E to 0xFF for CR 13014 E
	command[1] = i-1;
	command.append(CIpodControllerGlobal::checkSum(command, i));
	m_CommandBytes = command;
	return command;
}

void  CGetPBTrackInfo::unmarshall(QByteArray)
{
}



