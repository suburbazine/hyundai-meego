#ifndef DHAVN_IPODCONTROLLER_REMOTEEVENTNOTIFICATION_H
#define DHAVN_IPODCONTROLLER_REMOTEEVENTNOTIFICATION_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace DisplayRemoteLingo
{
class CRemoteEventNotification : public CIAPPacket
{
private:
    USE_LOG_UTIL
public:
    CRemoteEventNotification(int transId, bool isTidEnabled);
    QByteArray marshall();
    void    unmarshall(QByteArray);
    int       GetNotificationType() { return m_NotificationType; }
    int       GetNotificationValue() { return m_NotificationValue; }
private:
    qint16 m_TransId;
    bool m_isTidEnabled;
    int m_Index;
    int m_NotificationType;
    int m_NotificationValue;
};
}

#endif // DHAVN_IPODCONTROLLER_REMOTEEVENTNOTIFICATION_H
