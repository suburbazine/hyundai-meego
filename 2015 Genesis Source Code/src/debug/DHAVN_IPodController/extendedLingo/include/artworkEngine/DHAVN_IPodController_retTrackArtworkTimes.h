#ifndef DHAVN_IPODCONTROLLER_RETTRACKARTWORKTIMES_H
#define DHAVN_IPODCONTROLLER_RETTRACKARTWORKTIMES_H


#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CRetTrackArtworkTimes : public CIAPPacket
    {
        public:
           CRetTrackArtworkTimes(bool isTidEnabled);
           QByteArray marshall();
            void  unmarshall(QByteArray);
            int getTimeOffset();

        private:
            int m_TimeOffset;
    };
}
#endif // DHAVN_IPODCONTROLLER_RETTRACKARTWORKTIMES_H
