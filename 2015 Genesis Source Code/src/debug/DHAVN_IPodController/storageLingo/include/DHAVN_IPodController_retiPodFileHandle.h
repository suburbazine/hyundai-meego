#ifndef CRETIPODFILEHANDLE_H
#define CRETIPODFILEHANDLE_H

#include <DHAVN_IPodController_iAPPacket.h>
namespace StorageLingo
{
    class CRetiPodFileHandle : public CIAPPacket
    {
        public:
           CRetiPodFileHandle( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
	   int getFileHandle();
        private:
	    int m_Handle;
    };
} // end of namespace StorageLingo

#endif // CRETIPODFILEHANDLE_H
