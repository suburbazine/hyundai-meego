#ifndef DHAVN_IPODCONTROLLER_ACCACK_H
#define DHAVN_IPODCONTROLLER_ACCACK_H
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

class CAccAck : public CIAPPacket
{
private:
    USE_LOG_UTIL
public:
    CAccAck(int transIdArgs, int commandId, int status, bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
private:
    qint16 m_TransId;
    int m_AckCommandId;
    int m_Status;
};

}
#endif // DHAVN_IPODCONTROLLER_ACCACK_H
