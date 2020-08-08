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
#include <DHAVN_IPodController_retDevAuthenticationInfoNonIDPS.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace std;
using namespace GeneralLingo;

CRetDevAuthenticationInfoNonIDPS::CRetDevAuthenticationInfoNonIDPS(int transIdArgs, QByteArray certificate, int /*cerLength*/, int count) : m_TransId(transIdArgs)
{
//    LOG_INIT_V2(gControllerLogName);
//    //LOG_TRACE << "CRetDevAuthenticationInfoNonIDPS::CRetDevAuthenticationInfoNonIDPS Entry " << LOG_ENDL;
    m_Certificate = certificate;

    m_CerLength = m_Certificate.length();
//    //LOG_INFO << "In CRetDevAuthenticationInfoNonIDPS Constructor : length of cer = " << m_Certificate.length() << LOG_ENDL;
    m_Count = count;
//    //LOG_TRACE << "CRetDevAuthenticationInfoNonIDPS::CRetDevAuthenticationInfoNonIDPS Exit " << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CRetDevAuthenticationInfoNonIDPS::marshall()                    (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command
// get the certificate from the data provider, and
// send the certificate into multiple sections as the length is large
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CRetDevAuthenticationInfoNonIDPS::marshall()
{
//    //LOG_TRACE << "CRetDevAuthenticationInfoNonIDPS::marshall Entry " << LOG_ENDL;
    QByteArray array = getCerData();

    int current_size = array.length();
    QByteArray command;

    //Packing format =  Big Endianll
    command.resize(7);
    command[0] = 0x55;
    command[1] = 6+current_size;
    command[2] = 0x00;
    command[3] = 0x15;
    //command[4] = 0x00;
    //command[5] = 0x01;
    command[4] = 0x02;
    command[5] = 0x00;


    command[6] = getCurrentSection();
    command[7] = getTotalSections();
    command.append(array);


//    //LOG_INFO << "CRetDevAuthenticationInfoNonIDPS::marshell: section =" << getCurrentSection()<<
//                ", total= " <<  getTotalSections() << LOG_ENDL;

    //TODO calculate a valid checksum and insert
    command.append(CIpodControllerGlobal::checkSum(command, current_size+7));
    m_CommandBytes = command;
//   //LOG_TRACE << "CRetDevAuthenticationInfoNonIDPS::marshall Exit " << LOG_ENDL;
    return command;
}

void  CRetDevAuthenticationInfoNonIDPS::unmarshall(QByteArray /*returnCommand*/)
{

}

/******************************************************************************
// Function Name                                            Parameters
// CRetDevAuthenticationInfoNonIDPS::getCerData                    (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command
// get the certificate data on the basis of the current section count
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CRetDevAuthenticationInfoNonIDPS::getCerData()
{
//    //LOG_INFO << "CRetDevAuthenticationInfoNonIDPS::getCerData:length of the temp array = " << m_Certificate.length() << LOG_ENDL;
    return m_Certificate.mid(m_Count*MAXCERLENGTHTOBESENT, MAXCERLENGTHTOBESENT);
}

int CRetDevAuthenticationInfoNonIDPS::getTotalSections()
{
    int num;
    num = m_CerLength/MAXCERLENGTHTOBESENT;
    return num;
}

int CRetDevAuthenticationInfoNonIDPS::getCurrentSection()
{

    return m_Count;
}
