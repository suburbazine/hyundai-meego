#ifndef RETURNNUMOFPLAYINGTRACKS_H
#define RETURNNUMOFPLAYINGTRACKS_H

#include <DHAVN_IPodController_iAPPacket.h>
namespace ExtendedLingo
{
    class CReturnNumOfPlayingTracks : public CIAPPacket
    {
        public:
           CReturnNumOfPlayingTracks( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           int  getPlayListCount();
        private:
            int m_Index;
    };

} // end of namespace ExtendedLingo

#endif // RETURNNUMOFPLAYINGTRACKS
