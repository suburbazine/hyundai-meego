/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IpodController                                    *
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
#include <DHAVN_IPodController_returniPodSerialNumber.h>
#include <QByteArray>
using namespace GeneralLingo;

CReturniPodSerialNumber::CReturniPodSerialNumber( bool isTidEnabled )
    :CIAPPacket(0x00,0x0C,-1, isTidEnabled), m_SerialNumber("")
{

}

QByteArray CReturniPodSerialNumber::marshall()
{
        QByteArray command;
        return command;
}

QString CReturniPodSerialNumber::getSerialNumber()
{
    return m_SerialNumber.fromUtf8(m_SerialNumber.toAscii().data());
}

void  CReturniPodSerialNumber::unmarshall(QByteArray returnCommand)
{
	// Remove the checksum byte from the array in order to use mid to obtain the record string
	//chop(1) removes 1 byte from the end of the array
	returnCommand.chop(1);
	if( ((int)returnCommand[0] == 0x55) &&
			((int)returnCommand[2] == 0x00) &&
			((int)returnCommand[3] == 0x0C) ){

                if( m_isTidEnabled  )
		{
			m_TransactionID = 0x0000;
			m_TransactionID = m_TransactionID | returnCommand[4];
			m_TransactionID = m_TransactionID <<8;
			m_TransactionID = m_TransactionID | returnCommand[5];
			m_SerialNumber = QString(returnCommand.mid(6,-1)); // bytearray from 12 to the end
		}
		else
			m_SerialNumber = QString(returnCommand.mid(4,-1)); // bytearray from 12 to the end
		//qDebug() <<  "\n\nm_Title =  " << m_Title;
	}
}




