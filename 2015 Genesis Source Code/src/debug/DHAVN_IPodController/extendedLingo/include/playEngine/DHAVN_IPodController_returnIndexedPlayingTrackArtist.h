#ifndef DHAVN_IPODCONTROLLER_RETURNINDEXEDPLAYINGTRACKARTIST_H
#define DHAVN_IPODCONTROLLER_RETURNINDEXEDPLAYINGTRACKARTIST_H
#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CReturnIndexedPlayingTrackArtistName : public CIAPPacket
    {
        public:
           CReturnIndexedPlayingTrackArtistName( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QString getArtistName();
        private:
            QString m_ArtistName;
    };

} // end of namespace ExtendedLingo
#endif // DHAVN_IPODCONTROLLER_RETURNINDEXEDPLAYINGTRACKARTIST_H
