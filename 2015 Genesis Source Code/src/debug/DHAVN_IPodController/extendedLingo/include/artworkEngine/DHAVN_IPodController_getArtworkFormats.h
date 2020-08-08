#ifndef DHAVN_IPODCONTROLLER_GETARTWORKFORMATS_H
#define DHAVN_IPODCONTROLLER_GETARTWORKFORMATS_H


#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

namespace ExtendedLingo
{
    class CGetArtworkFormats : public CIAPPacket
    {
        public:
           CGetArtworkFormats(int transId,bool isTidEnabled);
           QByteArray marshall();
            void  unmarshall(QByteArray);

        private:
            int m_TrackIndex;
            int m_FormatId;
            int m_TimeOffset;
    };
}

#endif // DHAVN_IPODCONTROLLER_GETARTWORKFORMATS_H
