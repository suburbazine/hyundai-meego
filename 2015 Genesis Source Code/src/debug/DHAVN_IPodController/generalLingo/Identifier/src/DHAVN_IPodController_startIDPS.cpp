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
#include <DHAVN_IPodController_startIDPS.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace GeneralLingo;

CStartIDPS::CStartIDPS(int transIdArgs, bool isTidEnabled)
{
    m_TransId = transIdArgs;
    m_isTidEnabled = isTidEnabled;
}

/******************************************************************************
// Function Name                                            Parameters
// CStartIDPS::marshall()                                 (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command to start the IDPS process
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CStartIDPS::marshall()
{
    QByteArray command;
    int i = -1;

    //Packing format =  Big Endianll
    //command.resize(7);
    command[++i] = 0x55;
    command[++i] = 0x04;
    command[++i] = 0x00;
    command[++i] = 0x38;
    if( m_isTidEnabled ){
        command[++i] = (m_TransId & 0xff00) >> 8;
        command[++i] = m_TransId & 0x00ff;
    }
    //TODO calculate a valid checksum and insert
    command[1] = i -1;
    command[i+1] = CIpodControllerGlobal::checkSum(command, i);

    m_CommandBytes = command;
    return command;
}

void  CStartIDPS::unmarshall(QByteArray)
{

}
