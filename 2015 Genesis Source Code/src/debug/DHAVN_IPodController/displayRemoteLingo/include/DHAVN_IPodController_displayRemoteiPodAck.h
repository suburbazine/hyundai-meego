#ifndef DHAVN_IPODCONTROLLER_DISPLAYREMOTEIPODACK_H
#define DHAVN_IPODCONTROLLER_DISPLAYREMOTEIPODACK_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace DisplayRemoteLingo
{

class CDisplayRemoteIPodAck: public CIAPPacket
{
private:
    USE_LOG_UTIL
public:
    CDisplayRemoteIPodAck(int transIdArgs, bool isTidEnabled);
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
#endif // DHAVN_IPODCONTROLLER_DISPLAYREMOTEIPODACK_H
