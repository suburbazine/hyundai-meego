#ifndef DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKTITLE_H
#define DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKTITLE_H


#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CGetIndexedPlayingTrackTitle : public CIAPPacket
    {
        public:
        enum {RETURNCOMMANDID = 0x0021 };
           CGetIndexedPlayingTrackTitle( int nIndex,
                                                           int nTransId, bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
        private:
            int m_Index;
    };

} // end of namespace ExtendedLingo

#endif // DHAVN_IPODCONTROLLER_GETINDEXEDPLAYINGTRACKTITLE_H
