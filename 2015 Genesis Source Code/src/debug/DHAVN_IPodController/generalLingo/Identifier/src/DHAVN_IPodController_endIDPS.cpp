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
#include <DHAVN_IPodController_endIDPS.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace GeneralLingo;

CEndIDPS::CEndIDPS(int transIdArgs, bool isTidEnabled) :  m_TransId(transIdArgs)
{
        m_isTidEnabled = isTidEnabled;

}

/******************************************************************************
// Function Name                                            Parameters
// CEndIDPS::marshall()                                    (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command for the end idps
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CEndIDPS::marshall()
{
    QByteArray command;
    int i = -1;
    //command.resize(8);
    command[++i] = 0x55;
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x3B;
    if( m_isTidEnabled ){
        command[++i] = (m_TransId & 0xff00) >> 8;
        command[++i] = m_TransId & 0x00ff;
    }
    command[++i] = 0;

    command[1] = i -1;
    //TODO calculate a valid checksum and insert
    command[i+1] = CIpodControllerGlobal::checkSum(command, i);
    m_CommandBytes = command;
    return command;
}

void  CEndIDPS::unmarshall(QByteArray)
{

}
