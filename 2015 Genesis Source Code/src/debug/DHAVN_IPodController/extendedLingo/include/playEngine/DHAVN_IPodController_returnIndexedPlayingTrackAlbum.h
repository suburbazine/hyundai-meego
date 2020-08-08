#ifndef DHAVN_IPODCONTROLLER_RETURNINDEXEDPLAYINGTRACKALBUM_H
#define DHAVN_IPODCONTROLLER_RETURNINDEXEDPLAYINGTRACKALBUM_H
#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CReturnIndexedPlayingTrackAlbumName : public CIAPPacket
    {
        public:
           CReturnIndexedPlayingTrackAlbumName( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QString getAlbumName();
        private:
            QString m_AlbumName;
    };

} // end of namespace ExtendedLingo
#endif // DHAVN_IPODCONTROLLER_RETURNINDEXEDPLAYINGTRACKALBUM_H
