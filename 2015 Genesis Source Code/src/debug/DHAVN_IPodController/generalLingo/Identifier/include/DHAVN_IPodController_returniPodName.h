#ifndef DHAVN_IPODCONTROLLER_RETURNIPODNAME_H
#define DHAVN_IPODCONTROLLER_RETURNIPODNAME_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{
    class CReturniPodName : public CIAPPacket
    {
        public:
           CReturniPodName( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QString getiPodName();
        private:
            QString m_IPodName;
    };

} // end of namespace GeneralLingo

#endif // DHAVN_IPODCONTROLLER_RETURNIPODNAME_H

