#include "BTAdaptorMethod.h"

BTAdaptorMethod::BTAdaptorMethod(QObject *obj) :
    QObject(obj)
{
    btAdaptor = new BTAdaptor(this);
}

void BTAdaptorMethod::Request_Disconnect()
{
    emit LgeDBus_reqBTDisconnect();
}
