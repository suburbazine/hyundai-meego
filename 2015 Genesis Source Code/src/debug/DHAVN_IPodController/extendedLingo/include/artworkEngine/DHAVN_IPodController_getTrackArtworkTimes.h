#ifndef DHAVN_IPODCONTROLLER_GETTRACKARTWORKTIMES_H
#define DHAVN_IPODCONTROLLER_GETTRACKARTWORKTIMES_H


#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

namespace ExtendedLingo
{
    class CGetTrackArtworkTimes : public CIAPPacket
    {
        public:
           CGetTrackArtworkTimes(int trackIndex, int formatId,int artIndex, int count, int transId, bool isTidEnabled);
           QByteArray marshall();
            void  unmarshall(QByteArray);

        private:
            int m_TrackIndex;
            int m_FormatId;
            int m_Count;
            int m_ArtWorkIndex;
    };
}

#endif // DHAVN_IPODCONTROLLER_GETTRACKARTWORKTIMES_H
