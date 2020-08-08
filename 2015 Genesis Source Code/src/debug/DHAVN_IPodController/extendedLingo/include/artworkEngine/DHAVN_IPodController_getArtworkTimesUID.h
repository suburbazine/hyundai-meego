#ifndef DHAVN_IPODCONTROLLER_GETARTWORKTIMESUID_H
#define DHAVN_IPODCONTROLLER_GETARTWORKTIMESUID_H
#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

namespace ExtendedLingo
{
    class CGetArtworkTimesUID : public CIAPPacket
    {
    private:
        USE_LOG_UTIL
        public:
           CGetArtworkTimesUID(int transId,bool isTidEnabled, int formatID, quint64 uid);  // Modified by pawan 2013.05.15.
           QByteArray marshall();
            void  unmarshall(QByteArray);
            int getArtworkTimeInfo();
        private:
            int m_FormatID;
            quint64 m_Uid;    // Added by pawan 2013.05.15.
    };
}
#endif // DHAVN_IPODCONTROLLER_GETARTWORKTIMESUID_H
