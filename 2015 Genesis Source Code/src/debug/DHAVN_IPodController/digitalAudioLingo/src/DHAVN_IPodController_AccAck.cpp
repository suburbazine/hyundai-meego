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
#include <DHAVN_IPodController_AccAck.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace DigitalAudioLingo;

CAccAck::CAccAck(int transIdArgs, int commandId, int status, bool isTidEnabled) : m_TransId(transIdArgs), m_AckCommandId(commandId), m_Status(status)
{
    LOG_INIT_V2(gControllerLogName);
    //LOG_TRACE << "in CAccAck constructor " << LOG_ENDL;
    m_isTidEnabled = isTidEnabled;
    m_CommandID = commandId;
    m_Status = status;
}



QByteArray CAccAck::marshall()
{
    QByteArray command;
    int i = -1;

    //Packing format =  Big Endianll
    //command.resize(9);
    command[++i] = 0x55;
    command[++i] = 0x06;
    command[++i] = 0x0A;
    command[++i] = 0x00;

    if(m_isTidEnabled){
        command[++i] = ((m_TransId & 0xff00) >> 8);
        command[++i] = m_TransId & 0x00ff;
    }

    command[++i] = 0x00;// m_Status;
    command[++i] = 0x04;//m_CommandID;
    //TODO calculate a valid checksum and insert
    command[1] = i-1;
    command[i+1] = CIpodControllerGlobal::checkSum(command, i);

    m_CommandBytes = command;
    return command;
}

void  CAccAck::unmarshall(QByteArray /*returnCommand*/)
{

}



