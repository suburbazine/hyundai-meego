#ifndef SETCURRENTPLAYINGTRACK_H
#define SETCURRENTPLAYINGTRACK_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

namespace ExtendedLingo
{
    class CSetCurrentPlayingTrack : public CIAPPacket
    {
        public:
            CSetCurrentPlayingTrack(int index, int transId,bool isTidEnabled);
            QByteArray marshall();
            void  unmarshall(QByteArray);
        private:
            int m_newCurrentPlayingtrackIndex;
    };

}


#endif // SETCURRENTPLAYINGTRACK_H
