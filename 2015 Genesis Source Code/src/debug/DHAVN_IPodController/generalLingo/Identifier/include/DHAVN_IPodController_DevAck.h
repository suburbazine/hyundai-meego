#ifndef DHAVN_IPODCONTROLLER_DEVACK_H
#define DHAVN_IPODCONTROLLER_DEVACK_H

/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  30 Jan 2012                                   *
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
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_iAPPacket.h>

using namespace GeneralLingo;

class CDevAck : public CIAPPacket
{
public:
    CDevAck(int transIdArgs, qint8 ackCommand,  bool isTidEnabled) :m_TransId(transIdArgs), m_AckCommand(ackCommand)
    {
        m_isTidEnabled = isTidEnabled;
    }

    /******************************************************************************
    // Function Name                                            Parameters
    // CDevAck::marshall()                                 (QByteArray) (OUT), (void) (IN)
    //
    // Explanation
    // Construct the command to start the IDPS process
    // History
    // 30-Jan-2012      Pawan Gupta                        Added initial implementation
    *****************************************************************************/
    QByteArray marshall()
    {
        QByteArray command;
        command.resize(0);
        command[0] = 0x55;
        command[1] = 0x06;
        command[2] = 0x00;
        command[3] = 0x41;
        command[4] = (m_TransId & 0xff00) >> 8;
        command[5] = m_TransId & 0x00ff;
        command[6] = 0x00;
        command[7] = m_AckCommand;
        command[8] = CIpodControllerGlobal::checkSum(command, 7);

        m_CommandBytes = command;
        return command;
    }

    void  unmarshall(QByteArray)
    {

    }

private:
    qint16 m_TransId;
    qint8 m_AckCommand;
};


#endif // DHAVN_IPODCONTROLLER_DEVACK_H
