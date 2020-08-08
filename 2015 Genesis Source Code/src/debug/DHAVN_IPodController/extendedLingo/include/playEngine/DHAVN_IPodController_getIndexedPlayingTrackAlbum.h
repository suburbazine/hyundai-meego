#ifndef DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKALBUM_H
#define DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKALBUM_H
#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CGetIndexedPlayingTrackAlbumName : public CIAPPacket
    {
        public:
        enum {RETURNCOMMANDID = 0x0025 };
           CGetIndexedPlayingTrackAlbumName( int nIndex,
                                                           int nTransId,bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
        private:
            int m_Index;
    };

} // end of namespace ExtendedLingo
#endif // DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKALBUM_H
