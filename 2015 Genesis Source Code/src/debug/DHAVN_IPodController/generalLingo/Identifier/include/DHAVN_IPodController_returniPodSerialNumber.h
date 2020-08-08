#ifndef DHAVN_IPODCONTROLLER_RETURNIPODSERIALNUMER_H
#define DHAVN_IPODCONTROLLER_RETURNIPODSERIALNUMER_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace GeneralLingo
{
    class CReturniPodSerialNumber : public CIAPPacket
    {
        public:
           CReturniPodSerialNumber( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QString getSerialNumber();
        private:
            QString m_SerialNumber;
    };

} // end of namespace GeneralLingo

#endif // DHAVN_IPODCONTROLLER_RETURNIPODSERIALNUMER_H
