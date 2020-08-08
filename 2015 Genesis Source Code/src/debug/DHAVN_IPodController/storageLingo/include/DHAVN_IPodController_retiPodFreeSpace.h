#ifndef RETIPODFREE_H
#define RETIPODFREE_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>
namespace StorageLingo
{
    class CRetiPodFreeSpace : public CIAPPacket
    {
        private:
            USE_LOG_UTIL
        public:
           CRetiPodFreeSpace( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           quint64 getFreeSpace();
        private:
            quint64 m_FreeSpace;
    };
} // end of namespace StorageLingo

#endif // RETIPODFREESPACE_H
