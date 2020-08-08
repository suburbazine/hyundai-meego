#ifndef DHAVN_IPODCONTROLLER_IPODACK_H
#define DHAVN_IPODCONTROLLER_IPODACK_H
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
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
#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace DigitalAudioLingo
{

class CIPodAck: public CIAPPacket
{
private:
    USE_LOG_UTIL
public:
    CIPodAck(int transIdArgs, bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray returnCommand);
//    int ReturnStatus();
    //int AckCommand();
private:
    int m_TransId;
    int m_Status;
    int m_CommadId;
};

}
#endif // DHAVN_IPODCONTROLLER_IPODACK_H
