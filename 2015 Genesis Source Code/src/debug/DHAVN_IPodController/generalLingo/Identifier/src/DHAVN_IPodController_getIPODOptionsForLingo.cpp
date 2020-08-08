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
#include <DHAVN_IPodController_getIPODOptionsForLingo.h>
#include <iostream>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace GeneralLingo;

CGetIPODOptionsForLingo::CGetIPODOptionsForLingo(int transIdArgs, LingoType type, bool isTidEnabled) : m_LingoType(type)
{
//     LOG_INIT_V2(gControllerLogName);
    m_isTidEnabled = isTidEnabled;
    m_TransId = transIdArgs;
    // //LOG_TRACE << "calling CGetIPODOptionsForLingo::CGetIPODOptionsForLingo()" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CGetIPODOptionsForLingo::marshall()                     (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command to get the ipod options for lingo
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CGetIPODOptionsForLingo::marshall()
{
//    //LOG_TRACE << "calling CGetIPODOptionsForLingo::marshall()" << LOG_ENDL;
    QByteArray command;
    int i = -1;

    //Packing format =  Big Endianll
    //command.resize(8);
    command[++i] = 0x55;
    command[++i] = 0x05;
    command[++i] = 0x00;
    command[++i] = 0x4B;
    if( m_isTidEnabled ){
        command[++i] = ((m_TransId & 0xff00) >> 8);
        command[++i] = m_TransId & 0x00ff;
    }
    command[++i] = m_LingoType;
    command[1] = i-1;
    //TODO calculate a valid checksum and insert
    command[i+1] = CIpodControllerGlobal::checkSum(command, i);
    m_CommandBytes = command;
    return command;
}

void  CGetIPODOptionsForLingo::unmarshall(QByteArray)
{

}

