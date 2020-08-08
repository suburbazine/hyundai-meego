#ifndef DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKINFO_H
#define DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKINFO_H

#include <DHAVN_IPodController_iAPPacket.h>

#define TRACKDURATIONINFO  1
#define GENREINFO          2
#define COMPOSERINFO       3
#define ARTWORKCOUNT       7
#define TYPEINFO           8
namespace ExtendedLingo
{
    class CGetIndexedPlayingTrackInfo : public CIAPPacket
    {
        public:
        enum {RETURNCOMMANDID = 0x000D };
           CGetIndexedPlayingTrackInfo( int nIndex,
                                                          int nInfoType,
                                                           int nTransId,bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
        private:
            int m_Index;
            int m_InfoType;
    };

} // end of namespace ExtendedLingo

#endif // DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKINFO_H
