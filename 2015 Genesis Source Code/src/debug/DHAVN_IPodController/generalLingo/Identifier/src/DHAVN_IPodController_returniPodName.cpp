/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IpodController                                    *
 *
 * PROGRAMMERS : Jungae kim     *
 * DATE       :  16 August 2012                                   *
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
#include <DHAVN_IPodController_returniPodName.h>
#include <QByteArray>
#include <DHAVN_IPodController_CommonVars.h>
using namespace GeneralLingo;

CReturniPodName::CReturniPodName( bool isTidEnabled )
    :CIAPPacket(0x00,0x08,-1, isTidEnabled), m_IPodName("")
{
//    LOG_INIT_V2(gControllerLogName);

}

QByteArray CReturniPodName::marshall()
{
        QByteArray command;
        return command;
}

QString CReturniPodName::getiPodName()
{
    return m_IPodName.fromUtf8(m_IPodName.toAscii().data());
}

void  CReturniPodName::unmarshall(QByteArray returnCommand)
{
	// Remove the checksum byte from the array in order to use mid to obtain the record string
	//chop(1) removes 1 byte from the end of the array
	returnCommand.chop(1);
	if( ((int)returnCommand[0] == 0x55) &&
			((int)returnCommand[2] == 0x00) &&
			((int)returnCommand[3] == 0x08) ){

                if( m_isTidEnabled  )
		{
			m_TransactionID = 0x0000;
			m_TransactionID = m_TransactionID | returnCommand[4];
			m_TransactionID = m_TransactionID <<8;
			m_TransactionID = m_TransactionID | returnCommand[5];
			m_IPodName = QString(returnCommand.mid(6,-1)); // bytearray from 12 to the end
		}
		else
			m_IPodName = QString(returnCommand.mid(4,-1)); // bytearray from 12 to the end
//                //LOG_INFO <<  "\n\nm_IPodName =  " << m_IPodName << LOG_ENDL;
	}
}




