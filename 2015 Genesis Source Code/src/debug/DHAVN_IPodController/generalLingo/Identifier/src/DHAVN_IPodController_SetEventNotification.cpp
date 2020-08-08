
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Sowmini Philip       *
 * DATE       :  12 November 2011                                   *
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
#include <QByteArray>
#include <DHAVN_IPodController_SetEventNotification.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace GeneralLingo;
CSetEventNotification::CSetEventNotification(int transIdArgs,  bool isTidEnabled, bool isDBChangeSupported,bool isNowPlayingApplicationNameSupported, bool isNowPlayingApplicationDisplayNameStatus) : CIAPPacket(0x00, 0x49, transIdArgs, isTidEnabled),
	/*
	 * Added by jonghwan.cho@lge.com	2013.02.28.
	 * for static test defect
	 */
	m_DBChangeSupported(false),
	m_isNowPlayingApplicationBundleNameSupported(false),
	m_isNowPlayingApplicationDisplayNameStatus(false),
	m_isNowPlayingApplicationNameSupported(false)
    /*
	 * Added by jonghwan.cho@lge.com
	 */
{
    m_DBChangeSupported = isDBChangeSupported;
    /* Added for CR 10930 S*/
    m_isNowPlayingApplicationNameSupported = isNowPlayingApplicationNameSupported;
    /* Added for CR 10930 E*/

    //CR 13730: ATS Warning Fix S
    m_isNowPlayingApplicationDisplayNameStatus = isNowPlayingApplicationDisplayNameStatus;
    //CR 13730: ATS Warning Fix E
}

QByteArray CSetEventNotification::marshall()
{
    QByteArray command;
    int i = -1;

    //Packing format =  Big Endianll
    //command.resize(7);
    command[++i] = 0x55;
    command[++i] = 0x0C;
    command[++i] = 0x00;
    command[++i] = 0x49;
    if( m_isTidEnabled ){
        command[++i] = (m_TransactionID & 0xff00) >> 8;
        command[++i] = m_TransactionID & 0x00ff;
    }
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x00;
    //CR 13730: ATS Warning Fix S
    if(m_isNowPlayingApplicationDisplayNameStatus)
    {
        command[++i] = 0x08;
    }
    else
    {
        command[++i] = 0x00;
    }
    //CR 13730: ATS Warning Fix E

    /* Added for CR 10930 S*/
    if(m_isNowPlayingApplicationNameSupported && m_DBChangeSupported)
    {
        command[++i] = 0x06;
    }
    else if(m_isNowPlayingApplicationNameSupported)
    {
        command[++i] = 0x04;
    }/* Added for CR 10930 E*/
    else if(m_DBChangeSupported)
    {
        command[++i] = 0x02;
    }
    else
    {
        command[++i] = 0x00;
    }
    command[++i] = 0x04;

    //TODO calculate a valid checksum and insert
    command[1] = i -1;
    command[i+1] = CIpodControllerGlobal::checkSum(command, i);

    m_CommandBytes = command;
    return command;
}

void  CSetEventNotification::unmarshall(QByteArray)
{

}
