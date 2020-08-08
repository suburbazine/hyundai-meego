#include "inicversion.h"
#include "common.h"

#include <QEventLoop>
#include <QTimer>

InicVersion * InicVersion::inicVersion = NULL;

InicVersion::InicVersion(QObject * obj)
: QObject(obj){
	QEventLoop loop;
	m_timer = new QTimer(this);
	m_timer->setInterval(3000);
	m_timer->setSingleShot(true);

	QObject::connect(m_timer, SIGNAL(timeout()), &loop, SLOT(quit()));
	QObject::connect(this, SIGNAL(dataArrived()), &loop, SLOT(quit()));

	m_received = false;
	m_inicMsg = QDBusMessage::createMethodCall("com.lge.car.micom", "/SysInfo", "com.lge.car.micom.sysinfo", "LgeDBus_AskMOST_INICVersion");
	QDBusConnection::sessionBus().callWithCallback(
		m_inicMsg, this, SLOT(inicVersionReceived(const QByteArray &)),
			SLOT(inicVersionError()));
	loop.exec();
}

QByteArray InicVersion::GetInicVersion(){
	if ( inicVersion == NULL)
		inicVersion = new InicVersion;

	if ( inicVersion->received())
		return inicVersion->versionData();
	
	return QByteArray();
}

void InicVersion::inicVersionReceived(const QByteArray & ar){
	if ( m_timer->isActive()) m_timer->stop();

	if ( ar.count() >= 3){
		m_inicVersion.clear();
		m_inicVersion.append(ar);

		if ( ar.count() > 3){
			ULOG->log("HU INIC Version : %02X.%02X.%02X.%02X", (unsigned char)ar.at(0), 
				(unsigned char) ar.at(1), (unsigned char) ar.at(2), (unsigned char) ar.at(3));
		}
		else{
			ULOG->log("HU INIC Version : %02X.%02X.%02X", (unsigned char)ar.at(0), 
				(unsigned char) ar.at(1), (unsigned char) ar.at(2));
		}
		m_received = true;
	}
	emit dataArrived();
}

void InicVersion::inicVersionError(){
	if ( m_timer->isActive()) m_timer->stop();

	m_received = false;
	ULOG_EXCEPTION;
	ULOG->log("INIC Version Error");

	emit dataArrived();
}

