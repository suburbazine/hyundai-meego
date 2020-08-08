#include "DHAVN_PopUpPlugin.h"
#include "DHAVN_PopUpCPopup.h"

#include <QtDeclarative/qdeclarative.h>
#include <DHAVN_PopUp_Shared.h>

void PopUpPluginPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<CPopUp>(uri, 1, 0, "CPopUp");
    qmlRegisterType<DHAVN_POPUP>("PopUpConstants", 1, 0, "EPopUp");
}

Q_EXPORT_PLUGIN2(QmlPopUpPlugin, PopUpPluginPlugin)

