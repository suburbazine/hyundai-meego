#include <QList>
#include <QByteArray>
#include "DHAVN_PndrBTDBusInterface.h"

CPandoraBTDBusInterface::CPandoraBTDBusInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
: QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
    LOG_INIT_V2("Pandora");
    LOG_TRACE << "[BT][DBUS] CPandoraBTDBusInterface"<< LOG_ENDL;
}

CPandoraBTDBusInterface::~CPandoraBTDBusInterface()
{
    LOG_TRACE << "[BT][DBUS] ~CPandoraBTDBusInterface"<< LOG_ENDL;
}

