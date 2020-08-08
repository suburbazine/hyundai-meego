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
#include <DHAVN_IPodController_setUiMode.h>
#include <iostream>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace GeneralLingo;

CSetUiMode::CSetUiMode(int transIdArgs, bool isTidEnabled) :  CIAPPacket(0x00, 0x37, transIdArgs, isTidEnabled)
{
}

/******************************************************************************
// Function Name                                            Parameters
// CSetUiMode::marshall()                             (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command to enter into UI mode
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CSetUiMode::marshall()
{
    QByteArray command;
    int i = -1;

    //Packing format =  Big Endianll
    //command.resize(7);
    command[++i] = 0x55;
    command[++i] = 0x05;
    command[++i] = 0x00;
    command[++i] = 0x37;
    if( m_isTidEnabled ){
        command[++i] = (m_TransactionID & 0xff00) >> 8;
        command[++i] = m_TransactionID & 0x00ff;
    }
    command[++i] = 0x01;
    //TODO calculate a valid checksum and insert
    command[1] = i-1;
    command[i+1] = CIpodControllerGlobal::checkSum(command, i);
    m_CommandBytes = command;
    return command;
}

void  CSetUiMode::unmarshall(QByteArray)
{

}


