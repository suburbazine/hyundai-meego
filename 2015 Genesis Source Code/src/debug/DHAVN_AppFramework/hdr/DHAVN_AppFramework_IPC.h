#ifndef DHAVN_APPFRAMEWORKIPC_H
#define DHAVN_APPFRAMEWORKIPC_H

#include <QDBusAbstractAdaptor>
#include <QDBusInterface>
#include "DHAVN_AppFramework_Def.h"
#include "DHAVN_AppFramework_Event_Def.h"

#define AFW_IPC_SERVICE     "com.lge.car.afw.app%1"
#define AFW_IPC_PATH        "/event"
#define AFW_IPC_INTERFACE   "com.lge.car.afw.iface"

class AppFrameworkIPCPrivate;

class APP_FRAMEWORK_EXPORT AppFrameworkIPC : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.lge.car.afw.iface")
public:
    AppFrameworkIPC(QObject *pParent);
    ~AppFrameworkIPC();

    bool RegisterService(const int appId);
    void ListenService(const int appId);
    void SendEvent( const int targetAppId, Event &event, DISPLAY_T aDisplayType = DISPLAY_FRONT );
    void RegisterApp( const int targetAppId, const int nDisplay, const int appId );
	bool UnregisterService(const int appId);
	void DisconnectService(const int appId);

public Q_SLOTS:
    void ReceiveEvent( const Event &aArg1, const int &aArg2 );
    void Registered( const int &aArg1, const int &aArg2 );
Q_SIGNALS:
    void BroadcastEvent( const Event &aArg1, const int &aArg2 );

private:
    // future enhancement
    virtual void Reserved_1();
    virtual void Reserved_2();

private:
    //Private class object for AppFrameworkIPC
    AppFrameworkIPCPrivate * d_ptr;
    Q_DECLARE_PRIVATE(AppFrameworkIPC)
};

#endif // DHAVN_APPFRAMEWORKIPC_H
