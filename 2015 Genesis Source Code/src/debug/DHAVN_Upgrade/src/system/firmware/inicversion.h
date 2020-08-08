#ifndef	INIC_VERSION_H_
#define INIC_VERSION_H_

#include <QObject>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QTimer>

class InicVersion : public QObject{
Q_OBJECT
private:
	InicVersion(QObject * obj = 0);
	QByteArray m_inicVersion;
	static InicVersion * inicVersion;
	QDBusMessage m_inicMsg;
	bool m_received;

	QTimer * m_timer;

public:
	static QByteArray GetInicVersion();

	QByteArray versionData() { return m_inicVersion; }
	bool	received() { return m_received; }

private slots:
	void inicVersionReceived(const QByteArray &);
	void inicVersionError();

signals:
	void dataArrived();
};


#endif
