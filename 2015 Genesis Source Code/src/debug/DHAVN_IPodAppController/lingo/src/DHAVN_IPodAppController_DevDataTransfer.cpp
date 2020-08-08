/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :  IPodAppController                                *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  16 jan 2012                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodAppController_DevDataTransfer.h>
#include <DHAVN_IPodAppController_global.h>
#include <DHAVN_IPodAppController_CommonVars.h>
using namespace GeneralLingo;

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CDevDataTransfer::CDevDataTransfer(QByteArray data, int sessionId, int transIdArgs, QObject *parent) : CIAPPacket(parent),m_TransId(transIdArgs)
{
    LOG_INIT_V2(gIPodAppControllerLogs);
    m_DataArray.clear();
    m_DataArray.append(data);
    m_TransId = transIdArgs;
    m_SessionId = sessionId;
}

/******************************************************************************
// Function Name                                            Parameters
// CDevDataTransfer::marshall()                                 (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command to start the IDPS process
// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CDevDataTransfer::marshall()
{
    QByteArray command;
    int i = -1;
    int length = 6 + m_DataArray.size();

    //LOG_TRACE << "marshall IN :length = " << length  << LOG_ENDL;

    //Large packet
    if(length > 255)
    {
        command.resize(10);
        command[++i] = 0x55;
        command[++i] = 0x00; //Packet Payload Length marker
        command[++i] = ((length & 0xff00) >> 8); //Len bytes MSB
        command[++i] =  length & 0x00ff; //Len Bytes LSB
    }
    else
    {
        //small packet
        command.resize(8);
        command[++i] = 0x55;
        command[++i] = length;
    }
    command[++i] = 0x00;
    command[++i] = 0x42;
    command[++i] = ((m_TransId & 0xff00) >> 8);
    command[++i] =  m_TransId & 0x00ff;
    command[++i] = ((m_SessionId & 0xff00) >> 8);
    command[++i] =  m_SessionId & 0x00ff;

    i = i +2 ;
//    command[++i] = ((m_SessionId & 0xff00) >> 8);
//    command[++i] =  m_SessionId & 0x00ff;
    command.append(m_DataArray);
    i += m_DataArray.count();
    command.append(CIpodAppControllerGlobal::checkSum(command, i));

    m_CommandBytes = command;
    return command;
}

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void  CDevDataTransfer::unmarshall(const QByteArray&)
{

}
