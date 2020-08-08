#include <DHAVN_IPodController_RequestIdentify.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace GeneralLingo;

CRequestIdentify::CRequestIdentify(int transId, bool isTidEnabled)
    :CIAPPacket(0x00,0x00,transId, isTidEnabled)
{
}

QByteArray CRequestIdentify::marshall()
{
    // There is no body
    return NULL;
}

void CRequestIdentify::unmarshall(QByteArray)
{
    // There is no body
    return;
}
