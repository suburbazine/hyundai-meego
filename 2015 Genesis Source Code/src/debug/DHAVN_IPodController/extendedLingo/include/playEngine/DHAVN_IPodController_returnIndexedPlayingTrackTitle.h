#ifndef DHAVN_IPODCONTROLLER_RETURNINDEXEDPLAYINGTRACKTITLE_H
#define DHAVN_IPODCONTROLLER_RETURNINDEXEDPLAYINGTRACKTITLE_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CReturnIndexedPlayingTrackTitle : public CIAPPacket
    {
        public:
           CReturnIndexedPlayingTrackTitle( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QString getTitle();
        private:
            QString m_Title;
    };

} // end of namespace ExtendedLingo

#endif // DHAVN_IPODCONTROLLER_RETURNINDEXEDPLAYINGTRACKTITLE_H
