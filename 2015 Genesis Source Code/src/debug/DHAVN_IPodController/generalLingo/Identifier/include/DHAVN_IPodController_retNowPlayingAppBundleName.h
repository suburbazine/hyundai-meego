#ifndef DHAVN_IPODCONTROLLER_GETNOWPLAYINGAPPBUNDLENAME_H
#define DHAVN_IPODCONTROLLER_GETNOWPLAYINGAPPBUNDLENAME_H
#include <DHAVN_IPodController_iAPPacket.h>

namespace GeneralLingo
{
    class CRetNowPlayingAppBundleName : public CIAPPacket
    {
        public:
           CRetNowPlayingAppBundleName( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QString getNowPlayingAppBundleName();
        private:
            QString m_NowPlayingAppBundleName;
    };

} // end of namespace GeneralLingo



#endif // DHAVN_IPODCONTROLLER_GETNOWPLAYINGAPPBUNDLENAME_H
