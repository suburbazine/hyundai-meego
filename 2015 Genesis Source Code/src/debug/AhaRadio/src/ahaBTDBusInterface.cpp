#include <QList>
#include <QByteArray>
#include "ahaBTDBusInterface.h"

CAhaBTDBusInterface::CAhaBTDBusInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
: QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
    LOG_INIT_V2("Aha");
    LOG_TRACE << "[BT][DBUS] CAhaBTDBusInterface"<< LOG_ENDL;
}

CAhaBTDBusInterface::~CAhaBTDBusInterface()
{
    LOG_TRACE << "[BT][DBUS] ~CAhaBTDBusInterface"<< LOG_ENDL;
}

