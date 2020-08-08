/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
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
#include <DHAVN_IPodController_retDevAuthenticationInfo.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_Identifier.h>

using namespace GeneralLingo;

CRetDevAuthenticationInfo::CRetDevAuthenticationInfo(int transIdArgs, QByteArray certificate, int /*cerLength*/, int count, bool isTidEnabled, int maxPacketSize) :  m_TransId(transIdArgs)
{
//    LOG_INIT_V2(gControllerLogName);
//    //LOG_TRACE << "CRetDevAuthenticationInfo::CRetDevAuthenticationInfo Entry " << LOG_ENDL;
    m_Certificate = certificate;
    m_isTidEnabled = isTidEnabled;
    m_maxPacketSize = maxPacketSize;

    m_CerLength = m_Certificate.length();
//    //LOG_INFO << "CRetDevAuthenticationInfo::CRetDevAuthenticationInfo: length of certificate = " << m_Certificate.length() << LOG_ENDL;
    m_Count = count;
//    //LOG_TRACE << "CRetDevAuthenticationInfo::CRetDevAuthenticationInfo Exit " << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CRetDevAuthenticationInfo::marshall()                    (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command
// get the certificate from the data provider, and
// send the certificate into multiple sections as the length is large
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CRetDevAuthenticationInfo::marshall()
{
//    //LOG_TRACE << "CRetDevAuthenticationInfo::marshall Entry " << LOG_ENDL;
    QByteArray array = getCerData();

    int current_size = array.length();
    QByteArray command;
    int i = -1;

    //Packing format =  Big Endianll
    //command.resize(7);
    command[++i] = 0x55;
    int size=0;
    size = m_maxPacketSize;

    if(size > SMALLPACKETNUMBEROFBYTES)
    {
        command[++i] = 0x00;
        qint16 length =0;
        if( m_isTidEnabled )
            length = 8+current_size;
        else
            length = 6+current_size;
        command[++i] = ((length & 0xff00) >> 8);
        command[++i] = length & 0x00ff;
    }
    else
    {
        if( m_isTidEnabled )
            command[++i] = 8+current_size;
        else
            command[++i] = 6+current_size;
    }

    command[++i] = 0x00;
    command[++i] = 0x15;
    if( m_isTidEnabled ){
        command[++i] = ((m_TransId & 0xff00) >> 8);
        command[++i] = m_TransId & 0x00ff;
    }
    command[++i] = 0x02;
    command[++i] = 0x00;


    command[++i] = getCurrentSection();
    command[++i] = getTotalSections();
    command.append(array);
    i += current_size;
     if(size < SMALLPACKETNUMBEROFBYTES)
        command[1] = i-1;

////LOG_INFO << "CRetDevAuthenticationInfo::marshall: section = " << getCurrentSection()
//                << ", total = " << getTotalSections() << LOG_ENDL;
    //TODO calculate a valid checksum and insert
    command.append(CIpodControllerGlobal::checkSum(command, i));
    m_CommandBytes = command;
//    //LOG_TRACE << "CRetDevAuthenticationInfo::marshall Exit" << LOG_ENDL;
    return command;
}

void  CRetDevAuthenticationInfo::unmarshall(QByteArray /*returnCommand*/)
{

}

/******************************************************************************
// Function Name                                            Parameters
// CRetDevAuthenticationInfo::getCerData                    (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command
// get the certificate data on the basis of the current section count
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CRetDevAuthenticationInfo::getCerData()
{
//    //LOG_INFO << "CRetDevAuthenticationInfo::getCerData:length of the temp array= " << m_Certificate.length() << LOG_ENDL;

    int size = m_maxPacketSize;
    int maxCerLength=0;
    if(size >SMALLPACKETNUMBEROFBYTES)
        maxCerLength = MAXCERLENGTHLARGEPACKET;
    else
        maxCerLength = MAXCERLENGTHSMALLPACKET;
    return m_Certificate.mid(m_Count*maxCerLength, maxCerLength);
}

int CRetDevAuthenticationInfo::getTotalSections()
{
    int size = m_maxPacketSize;
    int maxCerLength=0;
//    size = 505;
    if(size >SMALLPACKETNUMBEROFBYTES)
        maxCerLength = MAXCERLENGTHLARGEPACKET;
    else
        maxCerLength = MAXCERLENGTHSMALLPACKET;
    int num;
    num = m_CerLength/maxCerLength;
    return num;
}

int CRetDevAuthenticationInfo::getCurrentSection()
{

    return m_Count;
}
