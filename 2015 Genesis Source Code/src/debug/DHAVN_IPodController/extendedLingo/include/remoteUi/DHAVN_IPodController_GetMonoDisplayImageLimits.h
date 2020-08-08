#ifndef DHAVN_IPODCONTROLLER_GETMONODISPLAYIMAGELIMITS_H
#define DHAVN_IPODCONTROLLER_GETMONODISPLAYIMAGELIMITS_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CGetMonoDisplayImageLimits : public CIAPPacket
    {
        public:
            CGetMonoDisplayImageLimits(int transId,  bool isTidEnabled);
            QByteArray marshall()        ;
            void  unmarshall(QByteArray)        ;
    };
}

#endif // GETMONODISPLAYIMAGELIMITS_H
