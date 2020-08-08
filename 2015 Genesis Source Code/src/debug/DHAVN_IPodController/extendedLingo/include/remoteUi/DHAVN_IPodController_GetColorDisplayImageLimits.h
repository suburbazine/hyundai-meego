#ifndef DHAVN_IPODCONTROLLER_GETCOLORDISPLAYIMAGELIMITS_H
#define DHAVN_IPODCONTROLLER_GETCOLORDISPLAYIMAGELIMITS_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{

    class CGetColorDisplayImageLimits : public CIAPPacket
    {
    public:
        CGetColorDisplayImageLimits(int transId,  bool isTidEnabled);
        QByteArray marshall()        ;
         void  unmarshall(QByteArray)        ;
    };

}


#endif // DHAVN_IPODCONTROLLER_GETCOLORDISPLAYIMAGELIMITS_H
