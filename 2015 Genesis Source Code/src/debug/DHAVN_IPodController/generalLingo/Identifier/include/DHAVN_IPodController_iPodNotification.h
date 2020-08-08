#ifndef DHAVN_IPODCONTROLLER_IPODNOTIFICATION_H
#define DHAVN_IPODCONTROLLER_IPODNOTIFICATION_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>   // Added by pawan 2013.05.15.

namespace GeneralLingo
{
class CiPodNotification : public CIAPPacket
{
private:  // Added by pawan 2013.05.15.
    USE_LOG_UTIL;
public:
    CiPodNotification( bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
    QString getBundleName();
    bool isDBChanged(){return m_IsDBChanged ;};   // Added by pawan 2013.05.15.
    void setDBChanged(bool value){m_IsDBChanged = value;}; // Added by pawan 2013.05.15.

    int getNotificationType();

private:
    QString m_BundleName;
    bool m_IsDBChanged;  // Added by pawan 2013.05.15.
    int m_NotificationType; //added by Tan

};

} // end of namespace GeneralLingo

#endif // DHAVN_IPODCONTROLLER_IPODNOTIFICATION_H
