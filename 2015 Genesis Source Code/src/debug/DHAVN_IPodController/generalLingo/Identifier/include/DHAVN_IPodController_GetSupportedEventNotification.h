#ifndef DHAVN_IPODCONTROLLER_GETSUPPORTEDEVENTNOTIFICATION_H
#define DHAVN_IPODCONTROLLER_GETSUPPORTEDEVENTNOTIFICATION_H
#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

namespace GeneralLingo
{

class CGetSupportedEventNotification: public CIAPPacket
{
public:
    CGetSupportedEventNotification(int transIdArgs,  bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
};

}
#endif // DHAVN_IPODCONTROLLER_GETSUPPORTEDEVENTNOTIFICATION_H
