#ifndef BTADAPTOR_H
#define BTADAPTOR_H

#include<QDebug>
#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;


class BTAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.lge.car.upgradebt")
    Q_CLASSINFO("D-Bus Introspection", ""
                "  <interface name=\"com.lge.car.upgradebt\">\n"
                "    <signal name=\"LgeDBus_reqBTDisconnect\"/>\n"
                "       <arg direction = \"out\" type=\"y\"/>\n"
                "  </interface>\n"
                "")

public:
    BTAdaptor(QObject *parent);
    virtual ~BTAdaptor();

public: // PROPERTIES

Q_SIGNALS: // SIGNALS
    void LgeDBus_reqBTDisconnect();


};

#endif // BTADAPTOR_H
