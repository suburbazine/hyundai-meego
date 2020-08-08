#ifndef DHAVN_IPODAPPCONTROLLER_REQUESTAPPLAUNCH_H
#define DHAVN_IPODAPPCONTROLLER_REQUESTAPPLAUNCH_H

#include <DHAVN_IPodAppController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CRequesdtAppLaunch: public CIAPPacket
{
    USE_LOG_UTIL
public:
    CRequesdtAppLaunch(QString bundleId, int transIdArgs, QObject *parent, bool relaunch);
    QByteArray marshall();
    void  unmarshall(const QByteArray&);
private:
    QString m_BundleId;
    bool m_relaunch;
};

}

#endif // DHAVN_IPODAPPCONTROLLER_REQUESTAPPLAUNCH_H
