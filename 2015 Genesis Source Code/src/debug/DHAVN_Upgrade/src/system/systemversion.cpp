#include "systemversion.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <unistd.h>
#include <fcntl.h>

#include "meegoeuid.h"
#include "common.h"
#include "systeminfo.h"


SystemVersion::SystemVersion(const QString & mountPath, QObject * obj)
: QObject(obj){
	m_variant = -1;
	m_mountPath = mountPath;
	findRootDevice();

	m_headUnitInfo = new SystemInfo;
	m_updateInfo = new SystemInfo;
}

SystemVersion::~SystemVersion(){
	delete m_headUnitInfo;
	delete m_updateInfo;
}

bool SystemVersion::compareWithHUVersion(const QString & ver, int & result){
	SystemInfo target;
	bool ok = target.setVersion(ver);

	if ( !ok )
		return false;

	if ( target > *m_headUnitInfo )
		result = -1;
	else if ( target == *m_headUnitInfo)
		result = 0;
	else
		result = 1;

	return true;
}

const QString & SystemVersion::getSystemVersionString(){
	return m_headUnitInfo->m_RawVersion;
}

bool SystemVersion::setUpdateVersion(const QString & ver, const QString & buildDate){
	m_updateInfo->setBuildDate(buildDate);
	return m_updateInfo->setVersion(ver);
}

bool SystemVersion::getSystemVersion(){
	return m_headUnitInfo->readVersionFromSystem();
}

unsigned char SystemVersion::getSystemMajorVersion(){
	return m_headUnitInfo->m_SoftwareMajor;
}

unsigned char SystemVersion::getSystemMinorVersion(){
	return m_headUnitInfo->m_SoftwareMinor;
}

QString SystemVersion::getSystemEventVersion(){
	return m_headUnitInfo->m_CertStep;
}

const QString & SystemVersion::findRootDevice(){
	if ( m_currentRootDevice.isEmpty()){
		int len = 0;
		char cmdline[1024];
		int fd = open("/proc/cmdline", O_RDONLY);
		if ( fd >= 0){
			len = ::read(fd, cmdline, sizeof(cmdline) - 1);
			close(fd);	
		}
		cmdline[len > 0 ? len : 0] = 0;	

		QString cmdlineString(cmdline);
		QStringList token = cmdlineString.split(" ");
		for (int i = 0; i < token.count(); ++i){
			if ( token.at(i).startsWith("root")){
				QStringList rootfsToken = token.at(i).split("=");
				if ( rootfsToken.count() == 2){
					m_currentRootDevice = rootfsToken.at(1);
					ULOG->log("findRootFileBlockDevice : %s", qPrintable(m_currentRootDevice));
					break;	
				}
			}
		}
	}

	return m_currentRootDevice;
}

bool SystemVersion::getSystemVariant(){
	MeegoEUID euid;
	if ( euid.success()){
		m_variantMsg = QDBusMessage::createMethodCall("com.lge.car.micom",
				"/SysInfo", "com.lge.car.micom.sysinfo", "LgeDBus_AskSysOption");
		return QDBusConnection::sessionBus().callWithCallback(
			m_variantMsg, this, SLOT(variantReceived(const QByteArray &)), SLOT(variantError()));
	}
	return false;
}

void SystemVersion::variantReceived(const QByteArray & ar){
	ULOG->log("Receive Variant : length %d", ar.count());

	if ( ar.count() == 9){
		m_variant = (int) ar.at(1);
		ULOG->log("Variant Code : %d", m_variant);
	}
}

void SystemVersion::variantError(){
	ULOG_EXCEPTION;
	ULOG->log("Variant Error");
}
