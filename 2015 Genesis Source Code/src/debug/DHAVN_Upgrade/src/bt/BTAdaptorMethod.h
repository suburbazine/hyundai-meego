#ifndef BTADAPTORMETHOD_H
#define BTADAPTORMETHOD_H

#include "BTAdaptor.h"
#include <QDBusContext>
#include <QDeclarativePropertyMap>
#include<QDebug>
class BTAdaptorMethod : public QObject ,public QDBusContext{
    Q_OBJECT
    BTAdaptor *btAdaptor;
public:
    BTAdaptorMethod(QObject * obj = 0);

    void Request_Disconnect();

Q_SIGNALS:
    void LgeDBus_reqBTDisconnect();

};

#endif // BTADAPTORMETHOD_H
