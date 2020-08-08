#ifndef DHAVN_IPODCONTROLLER_SETEVENTNOTIFICATION_H
#define DHAVN_IPODCONTROLLER_SETEVENTNOTIFICATION_H
#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

namespace GeneralLingo
{

class CSetEventNotification: public CIAPPacket
{
public:
    /* Added for CR 10930 S*/ //CR 13730: ATS Warning Fix S
    CSetEventNotification(int transIdArgs,  bool isTidEnabled, bool isDBChangeSupported,bool isNowPlayingApplicationNameSupported, bool isNowPlayingApplicationDisplayNameStatus);
    /* Added for CR 10930 E*/ //CR 13730: ATS Warning Fix E
    QByteArray marshall();

    void  unmarshall(QByteArray);

private:
    bool m_DBChangeSupported;
    /* Added for CR 10930 S*/
    bool m_isNowPlayingApplicationBundleNameSupported;
    /* Added for CR 10930 E*/
    //CR 13730: ATS Warning Fix S
    bool m_isNowPlayingApplicationDisplayNameStatus;
    //CR 13730: ATS Warning Fix E
    bool m_isNowPlayingApplicationNameSupported;
};

}
#endif // DHAVN_IPODCONTROLLER_SETEVENTNOTIFICATION_H
