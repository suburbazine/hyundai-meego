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
#include <DHAVN_IPodAppController_DevAck.h>
#include <DHAVN_IPodAppController_global.h>
using namespace GeneralLingo;

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CDevAck::CDevAck(int transid, qint8 commandid, QObject* parent) : CIAPPacket(parent), m_TransId(transid)
{
    m_CommandID = commandid;
}

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CDevAck::marshall()
{
    QByteArray command;
    command.resize(0);
    command[0] = 0x55;
    command[1] = 0x06;
    command[2] = 0x00;
    command[3] = 0x41;
    command[4] = ((m_TransId & 0xff00) >> 8);
    command[5] =  m_TransId & 0x00ff;
    command[6] = 0x00;
    command[7] = m_CommandID;
    command[8] = CIpodAppControllerGlobal::checkSum(command, 7);
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
void  CDevAck::unmarshall(const QByteArray&)
{

}
