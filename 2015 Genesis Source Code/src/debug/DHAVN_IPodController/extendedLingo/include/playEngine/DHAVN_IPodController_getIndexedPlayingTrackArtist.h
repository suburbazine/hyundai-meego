#ifndef DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKARTIST_H
#define DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKARTIST_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CGetIndexedPlayingTrackArtistName : public CIAPPacket
    {
        public:
        enum {RETURNCOMMANDID = 0x0023 };
           CGetIndexedPlayingTrackArtistName( int nIndex,
                                                           int nTransId,bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
        private:
            int m_Index;
    };

} // end of namespace ExtendedLingo

#endif // DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKARTIST_H
