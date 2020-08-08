#ifndef DHAVN_IPODCONTROLLER_SETREMOTEEVENTNOTIFICATION_H
#define DHAVN_IPODCONTROLLER_SETREMOTEEVENTNOTIFICATION_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace DisplayRemoteLingo
{
class CSetRemoteEventNotification: public CIAPPacket
{
private:
    USE_LOG_UTIL
public:
    CSetRemoteEventNotification(int transId, bool isTidEnabled);
    QByteArray marshall();
    void    unmarshall(QByteArray);
private:
    qint16 m_TransId;
    bool m_isTidEnabled;
};

}

#endif  //  SETREMOTEEVENTNOTIFICATION_H

