#ifndef DHAVN_IPODCONTROLLER_REQUESTIDENTIFY_H
#define DHAVN_IPODCONTROLLER_REQUESTIDENTIFY_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace GeneralLingo
{

class CRequestIdentify : public CIAPPacket
{
public:
    CRequestIdentify(int transId, bool isTidEnabled);

    QByteArray marshall();
    void       unmarshall(QByteArray);
};

}
#endif // DHAVN_IPODCONTROLLER_REQUESTIDENTIFY_H
