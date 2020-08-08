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
#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_setPlayStatusChangeNotification.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
CSetPlayStatusChangeNotification::CSetPlayStatusChangeNotification(int transId,bool isTidEnabled,bool isUIDBased, bool isSyncInProgress)
    :CIAPPacket(0x04,0x0026,transId, isTidEnabled),m_isUIDBased(isUIDBased),m_IsSyncInProgress(isSyncInProgress)
{
//LOG_INIT_V2(gControllerLogName);
}

QByteArray CSetPlayStatusChangeNotification::marshall()
{
    QByteArray command;
//    //LOG_TRACE << "In CSetPlayStatusChangeNotification::marshalll " << m_isUIDBased << LOG_ENDL;
    //Packing format =  Big Endian
//    command[++i] = 0x00; //not required for USB.Hence omitted here
    int i = -1;
    command[++i] = 0x55;
    command[++i] = 0x09;
    command[++i] = 0x04;
    command[++i] = 0x00;
    command[++i] = 0x26;
    if( m_isTidEnabled )
    {
        command[++i] = ((m_TransactionID & 0xff00) >> 8);
        command[++i] = m_TransactionID & 0x00ff;
    }
    command[++i] = 0x00;
    command[++i] = 0x00;

    if( (m_isUIDBased == true) /*&& (m_IsSyncInProgress == false)*/) // Modified by pawan 2013.05.04.
    {
//        //LOG_TRACE << "CSetPlayStatusChangeNotification Marshall If cond "<<LOG_ENDL;
            // ATS error fix: set the 11 bit

            command[++i] = 0x19;  // Modified by pawan 2013.05.04.
        // Modified by jonghwan.cho@lge.com 2013.04.09
        // Modified by jonghwan.cho@lge.com 2013.04.11
        // for up to iOS 6.1 version
        command[++i] = 0x17;
        //command[++i] = 0x1D;
    }
    else
    {
//        //LOG_TRACE << "CSetPlayStatusChangeNotification Marshall Else "<<LOG_ENDL;
            command[++i] = 0x08;
			// ATS error fix: set the 11 bit
            command[++i] = 0x17;
    }
    command[1] = i-1;
    command.append(CIpodControllerGlobal::checkSum(command, i));
    m_CommandBytes = command;
    return command;
}

void  CSetPlayStatusChangeNotification::unmarshall(QByteArray)
{

}


