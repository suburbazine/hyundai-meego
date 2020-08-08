#ifndef AHABTDBUSINTERFACE_H
#define AHABTDBUSINTERFACE_H

#include <QObject>
#include <QtDBus/QDBusVariant>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusAbstractInterface>
#include <QList>
#include <QtDBus/QDBusArgument>
#include <QByteArray>
#include "DHAVN_LogUtil.h"

class CAhaBTDBusInterface : public QDBusAbstractInterface
{
    Q_OBJECT

    USE_LOG_UTIL;

public:
    static inline const char *staticInterfaceName()
    { return "com.lge.car.btcontroller.ahaif"; }
    static inline const char *staticServiceName()
    { return "com.lge.car.btcontroller"; }
    static inline const char *staticObjectName()
    { return "/aha"; }

public:

static CAhaBTDBusInterface* GetInstance();
~CAhaBTDBusInterface();
CAhaBTDBusInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

private:
        static CAhaBTDBusInterface *m_pInstance;


public Q_SLOTS:
        inline QDBusPendingReply<> SPPConnect()
        {
            LOG_TRACE << "In CAhaBTDBusInterface:: SPPConnect() Entry"<< LOG_ENDL;

                return asyncCall(QLatin1String("SPPConnect"));
        }

        inline QDBusPendingReply<> SPPDisconnect()
        {
            LOG_TRACE << "In CAhaBTDBusInterface:: SPPDisconnect() Entry"<< LOG_ENDL;

                return asyncCall(QLatin1String("SPPDisconnect"));
        }

        inline QDBusPendingReply<> SPPDataWriteToBT(const QByteArray baTxData)
        {
            LOG_TRACE << "In CAhaBTDBusInterface:: SPPDataWriteToBT().Data written: "
                      << baTxData.toHex()<< LOG_ENDL;

                return asyncCall(QLatin1String("SPPDataWriteToBT"), baTxData);
        }

 Q_SIGNALS:
    void SPPConnected(bool bResult);
    void SPPDisconnected(bool bResult);

    void SPPDataReadComplete(const QByteArray);
    void SPPDataWriteToBTStatus(const int error);
};

namespace com {
  namespace lge {
    namespace car {
      namespace btcontroller {
        typedef ::CAhaBTDBusInterface ahaif;
     }
    }
  }
}


#endif // AHABTDBUSINTERFACE_H
