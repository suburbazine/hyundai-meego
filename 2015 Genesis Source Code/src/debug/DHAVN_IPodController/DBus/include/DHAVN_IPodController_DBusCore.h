#ifndef DHAVN_IPODCONTROLLER_DBUSCORE_H
#define DHAVN_IPODCONTROLLER_DBUSCORE_H

#include <QString>
#include <QObject>
#include <QDBusConnection>
#include <DHAVN_IPodController_DBusAdaptor.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_CommonVars.h>




class CIPodController_DBus : public QObject
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CIPodController_DBus(QObject* parent=NULL);
    ~CIPodController_DBus();
    void IpodConnected();
public Q_SLOTS:

     QString GetHidPath(const QString& ipodNumber);
     qint16 GetApplicationSessionID(const QString& AppId);

signals:

    void IpodAppSessionClosed(const QString& ipodNumber, const QString& appId);
    void IPodDisconnected(const QString& ipodNumber);

private slots:

    void OnIpodAppSessionClosed(const QString& ipodNumber, const QString& appId);
    void OnIPodDisconnected(const QString& ipodNumber);
};


class CIPodControllerDBus_Initializer : public QObject
{
private:
    USE_LOG_UTIL
public:
    CIPodControllerDBus_Initializer(QObject* parent) : QObject(parent), m_DBusConnection(QDBusConnection::sessionBus())
    {
        LOG_INIT_V2(gControllerLogName);
        InitializeDBus(parent);
    }
    ~CIPodControllerDBus_Initializer()
    {
        m_DBusConnection.unregisterObject("/");
        m_DBusConnection.unregisterService("org.lge.dbusTest");
        delete m_DBusCore;
    }
    void ipodConnected()
    {
        m_DBusCore->IpodConnected();
    }

private:
    void InitializeDBus(QObject* parent)
    {
        //LOG_TRACE << "CIPodControllerDBus_Initializer::InitializeDBus() Entry" << LOG_ENDL;
        m_DBusCore = new CIPodController_DBus(parent);

        new DbusTestAdaptor(m_DBusCore);

        bool ret = m_DBusConnection.registerService("org.lge.dbusTest");
        //LOG_INFO << "registerService - org.lge.dbusTest with retval =  " << ret << LOG_ENDL;
        ret = m_DBusConnection.registerObject("/", m_DBusCore);
        //LOG_INFO << "registerObject - org.lge.dbusTest with retval = " << ret << LOG_ENDL;
        QDBusError error = m_DBusConnection.lastError();
        //LOG_INFO << "CIPodControllerDBus_Initializer::InitializeDBus dbus last message: " <<
        //            error.message().toAscii().data() << LOG_ENDL;
        //LOG_TRACE << "CIPodControllerDBus_Initializer::InitializeDBus() Exit" << LOG_ENDL;
    }
private:
CIPodController_DBus* m_DBusCore;
QDBusConnection m_DBusConnection;
};

#endif // DHAVN_IPODCONTROLLER_DBUSCORE_H
