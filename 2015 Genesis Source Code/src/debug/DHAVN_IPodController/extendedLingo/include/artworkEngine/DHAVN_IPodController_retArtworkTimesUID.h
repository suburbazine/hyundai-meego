#ifndef DHAVN_IPODCONTROLLER_RETARTWORKTIMESUID_H
#define DHAVN_IPODCONTROLLER_RETARTWORKTIMESUID_H


#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

namespace ExtendedLingo
{
class CRetTrackArtworkTimesUID : public CIAPPacket
{
#ifdef DBUGLOGS
    private:
    USE_LOG_UTIL
#endif
    public:
        CRetTrackArtworkTimesUID(bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
    int getTimeOffset();

private:
    int m_TimeOffset;
};
}
#endif // DHAVN_IPODCONTROLLER_RETARTWORKTIMESUID_H
