#include <QtDBus>
#include <QDBusError>
#include "DHAVN_AppFramework_Logger_Def.h"
#include "DHAVN_AppFramework_IPC.h"

/** AppFrameworkIPCPriate class definition */
class AppFrameworkIPCPrivate
{

private:

    /** Constructor */
    AppFrameworkIPCPrivate( AppFrameworkIPC *pQPtr, QObject *pParent )
        :m_pParent(pParent), m_myAppId(0), q_ptr(pQPtr)  {}


    /** Destructor */
    virtual ~AppFrameworkIPCPrivate() {}

public:
    QObject* m_pParent;
    int m_myAppId;
    QHash<int, QDBusInterface*> m_ifaces;

    //Q-Pointer to the AppFrameworkIPC class
    AppFrameworkIPC * const q_ptr;
    Q_DECLARE_PUBLIC(AppFrameworkIPC)
};

AppFrameworkIPC::AppFrameworkIPC(QObject *pParent)
: QDBusAbstractAdaptor(pParent)
{
    d_ptr = new AppFrameworkIPCPrivate(this, pParent);
    setAutoRelaySignals(true);
}

AppFrameworkIPC::~AppFrameworkIPC()
{
    delete d_ptr;
}

void AppFrameworkIPC::Reserved_1() {}
void AppFrameworkIPC::Reserved_2() {}

bool AppFrameworkIPC::RegisterService(const int appId)
{
    QString serviceName = QString(AFW_IPC_SERVICE).arg(appId);
    if( !QDBusConnection::sessionBus().registerService(serviceName) ) {
        Logger::Instance()->Log(QString("[AFW_IPC] registerService : %1").arg(QDBusConnection::sessionBus().lastError().message()));
        return false;
    }
    Logger::Instance()->Log(QString("[AFW_IPC] serviced : %1").arg(serviceName));
    if( !QDBusConnection::sessionBus().registerObject(AFW_IPC_PATH, this,
        QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)
        //QDBusConnection::ExportAdaptors);
        ) {
        Logger::Instance()->Log(QString("[AFW_IPC] registerObject : %1").arg(QDBusConnection::sessionBus().lastError().message()));
        return false;
    }
    d_ptr->m_myAppId = appId;
    return true;
}

bool AppFrameworkIPC::UnregisterService(const int appId)
{
	QString serviceName = QString(AFW_IPC_SERVICE).arg(appId);

	QDBusConnection::sessionBus().unregisterObject(AFW_IPC_PATH, QDBusConnection::UnregisterTree);
//	Logger::Instance()->Log(QString("[AFW_IPC] unregisterObject : %1").arg(QDBusConnection::sessionBus().lastError().message()));
	
    if( !QDBusConnection::sessionBus().unregisterService(serviceName) ) {
        Logger::Instance()->Log(QString("[AFW_IPC] unregisterService : %1").arg(QDBusConnection::sessionBus().lastError().message()));
        return false;
    }
	Logger::Instance()->Log(QString("[AFW_IPC] unregistered service : %1").arg(serviceName));
	return true;
}


void AppFrameworkIPC::ListenService(const int appId)
{
    QString serviceName = QString(AFW_IPC_SERVICE).arg(appId);
    QDBusConnection::sessionBus().connect(serviceName, AFW_IPC_PATH, AFW_IPC_INTERFACE,
                                          "BroadcastEvent", this, SLOT(ReceiveEvent(Event, int)));
    Logger::Instance()->Log(QString("[AFW_IPC] listening : %1").arg(serviceName));
}

void AppFrameworkIPC::DisconnectService(const int appId)
{
    QString serviceName = QString(AFW_IPC_SERVICE).arg(appId);
    QDBusConnection::sessionBus().disconnect(serviceName, AFW_IPC_PATH, AFW_IPC_INTERFACE,
                                          "BroadcastEvent", this, SLOT(ReceiveEvent(Event, int)));
    Logger::Instance()->Log(QString("[AFW_IPC] disconnect : %1").arg(serviceName));
}

void AppFrameworkIPC::SendEvent(const int targetAppId, Event &event, DISPLAY_T aDisplayType )
{
    QDBusInterface *iface = NULL;
    if(d_ptr->m_ifaces.contains(targetAppId) && d_ptr->m_ifaces[targetAppId]->isValid()) {
        iface = d_ptr->m_ifaces[targetAppId];
    }
    else {
        QString serviceName;
        if( targetAppId == APP_ALL )
            serviceName = QString(AFW_IPC_SERVICE).arg(d_ptr->m_myAppId);
        else
            serviceName = QString(AFW_IPC_SERVICE).arg(targetAppId);
        iface = new QDBusInterface(serviceName, AFW_IPC_PATH, AFW_IPC_INTERFACE,
                                   QDBusConnection::sessionBus(), this);
        if(!iface->isValid()) {
            Logger::Instance()->Log(QString("[AFW_IPC] interface invalid : %1").arg(QDBusConnection::sessionBus().lastError().message()));
            return;
        }
        d_ptr->m_ifaces[targetAppId] = iface;
        Logger::Instance()->Log(QString("[AFW_IPC] get interface : %1").arg(serviceName));
    }

    if( targetAppId == APP_ALL ) {
        QDBusMessage msg = QDBusMessage::createSignal(AFW_IPC_PATH, AFW_IPC_INTERFACE, "BroadcastEvent");
        msg << QVariant::fromValue<Event>(event);
        msg << QVariant::fromValue<int>(aDisplayType);
        iface->connection().send(msg);
        //QDBusConnection::sessionBus().send(msg);
        Logger::Instance()->Log(QString("[AFW_IPC] --> Broadcasted"));
    }
    else {
        QList<QVariant> arg;
        arg << QVariant::fromValue<Event>(event);
        arg << QVariant::fromValue<int>(aDisplayType);
        iface->callWithArgumentList(QDBus::NoBlock, "ReceiveEvent", arg);
        Logger::Instance()->Log(QString("[AFW_IPC] --> Sent"));
    }
}

void AppFrameworkIPC::RegisterApp( const int targetAppId, const int nDisplay, const int appId)
{
    if( targetAppId == APP_ALL )
        return;

    QDBusInterface *iface = NULL;
    if(d_ptr->m_ifaces.contains(targetAppId) && d_ptr->m_ifaces[targetAppId]->isValid()) {
        iface = d_ptr->m_ifaces[targetAppId];
    }
    else {
        QString serviceName = QString(AFW_IPC_SERVICE).arg(targetAppId);
        iface = new QDBusInterface(serviceName, AFW_IPC_PATH, AFW_IPC_INTERFACE,
                                   QDBusConnection::sessionBus(), this);
        if(!iface->isValid()) {
            Logger::Instance()->Log(QString("[AFW_IPC] interface invalid : %1").arg(QDBusConnection::sessionBus().lastError().message()),Logger::PRIORITY_LOW);
            return;
        }
        d_ptr->m_ifaces[targetAppId] = iface;
        Logger::Instance()->Log(QString("[AFW_IPC] get interface : %1").arg(serviceName));
    }

    QList<QVariant> arg;
    arg << QVariant::fromValue<int>(nDisplay);
    arg << QVariant::fromValue<int>(appId);
    iface->callWithArgumentList(QDBus::NoBlock, "Registered", arg);
    Logger::Instance()->Log(QString("[AFW_IPC] --> Sent"));
}

void AppFrameworkIPC::ReceiveEvent( const Event &aArg1, const int &aArg2 )
{
    Logger::Instance()->Log(QString("[AFW_IPC] <-- Received"));
    QMetaObject::invokeMethod(d_ptr->m_pParent, "NotifyFromUIStateManagerBase",
                              Q_ARG(Event, aArg1), Q_ARG(uint, (uint)aArg2));
}

void AppFrameworkIPC::Registered( const int &aArg1, const int &aArg2 )
{
    Logger::Instance()->Log(QString("[AFW_IPC] <-- Registered"));
    QMetaObject::invokeMethod(d_ptr->m_pParent, "RegisteredFromOtherApp",
                              Q_ARG(int, aArg1), Q_ARG(int, aArg2));
}
