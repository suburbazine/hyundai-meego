#ifndef RETURNCURRENTPLAYINGTRACKINDEX_H
#define RETURNCURRENTPLAYINGTRACKINDEX_H

#include <DHAVN_IPodController_iAPPacket.h>
namespace ExtendedLingo
{
    class CReturnCurrentPlayingTrackIndex : public CIAPPacket
    {
        public:
           CReturnCurrentPlayingTrackIndex( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           int getCurrentIndex();
        private:
            int m_Index;
    };

} // end of namespace ExtendedLingo

#endif // RETURNCURRENTPLAYINGTRACKINDEX_H
