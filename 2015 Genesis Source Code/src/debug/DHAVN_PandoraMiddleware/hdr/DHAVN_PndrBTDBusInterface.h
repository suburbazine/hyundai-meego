#include <QObject>
#include <QtDBus/QDBusVariant>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusAbstractInterface>
#include <QList>
#include <QtDBus/QDBusArgument>
#include <QByteArray>
#include "DHAVN_LogUtil.h"

class CPandoraBTDBusInterface : public QDBusAbstractInterface
{
    Q_OBJECT

    USE_LOG_UTIL;

public:
    static inline const char *staticInterfaceName()
    { return "com.lge.car.btcontroller.pandoraif"; }
    static inline const char *staticServiceName()
    { return "com.lge.car.btcontroller"; }
    static inline const char *staticObjectName()
    { return "/pandora"; }
 
public:

static CPandoraBTDBusInterface* GetInstance();
~CPandoraBTDBusInterface();
CPandoraBTDBusInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

private:
        static CPandoraBTDBusInterface *m_pInstance;
	

public Q_SLOTS:
	inline QDBusPendingReply<> SPPConnect()
	{
            //LOG_TRACE << "In CPandoraBTDBusInterface:: SPPConnect() Entry"<< LOG_ENDL;

		return asyncCall(QLatin1String("SPPConnect"));
	}

	inline QDBusPendingReply<> SPPDisconnect()
	{
            //LOG_TRACE << "In CPandoraBTDBusInterface:: SPPDisconnect() Entry"<< LOG_ENDL;

		return asyncCall(QLatin1String("SPPDisconnect"));
	}

	inline QDBusPendingReply<> SPPDataWriteToBT(const QByteArray baTxData)
	{
            //LOG_TRACE << "In CPandoraBTDBusInterface:: SPPDataWriteToBT().Data written: "
            //          << baTxData.toHex()<< LOG_ENDL;

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
        typedef ::CPandoraBTDBusInterface pandoraif;
     }
    }
  }
}

