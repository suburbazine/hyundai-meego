#ifndef DHAVN_OPODCONTROLLER_EAPROTOCOLTOKEN_H
#define DHAVN_OPODCONTROLLER_EAPROTOCOLTOKEN_H

#include <DHAVN_IPodController_FIDToken.h>
#include <DHAVN_LogUtil.h>
namespace GeneralLingo
{

class CEAProtocolToken : public CFIDToken
{
private:
    USE_LOG_UTIL
public:
    CEAProtocolToken();
//#ifdef DHAVN_COMPILE_FOR_AHA // modified by jungae, 2012.12.01 for AhaRadio application
    QByteArray getPandoraToken();
    QByteArray getAhaToken();
//#else
//	QByteArray getToken();
//#endif
};
}

#endif // DHAVN_OPODCONTROLLER_EAPROTOCOLTOKEN_H
