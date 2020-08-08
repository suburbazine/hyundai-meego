#include "systeminfo.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "common.h"

SystemInfo::SystemInfo(){
	clear();
}

bool SystemInfo::readVersionFromSystem(){
	QFile file(HU_VERSION_FILE_PATH);

	if ( !file.open(QIODevice::ReadOnly))
		return false;

	QTextStream stream(&file);
	QString line;
	bool versionFound = false;
	bool dateFound = false;

	do {
		line = stream.readLine();

		if ( line.isEmpty()) continue;

		if ( ! versionFound ){
			versionFound = setVersion(line);
		}
		
		if ( !dateFound){
			QString dateStr = line.trimmed();
			QStringList dateToken = dateStr.split("-");
			if ( dateToken.count() == 3){
				dateFound = true;
				setBuildDate(dateStr);
			}
		}
	} while(!line.isNull());

	file.close();

	ULOG->log("HU System Version : %s", qPrintable(m_RawVersion));

	return versionFound;
}

void SystemInfo::clear(){
	m_CertVersion = m_HardwareVersion = m_SoftwareMajor = m_SoftwareMinor = SystemInfo::UNDEFINED_VERSION_VALUE;
}

bool SystemInfo::setVersion(const QString & ver){
	if ( ver.isNull() || ver.isEmpty())
		return false;

	QString version;

	if ( ver.contains("_")){
		QStringList partitionVersion = 
			ver.split("_");
		version = partitionVersion.at(0);
	}
	else{
		version = ver;
	}

	QStringList token = version.split(".");
	if ( token.count() >= 5 && token.at(0).contains(DHAVN_PROJECT_CODE)){
		m_RawVersion = version;
		setVersion(token);
		return true;
	}

	return false;
}

void SystemInfo::setVersion(const QStringList & versionStr){
	bool ok;

	clear();
	m_CertStep = versionStr.at(2).left(2).toUpper();

	unsigned char certVersion;
	unsigned char hardwareVersion;
	unsigned char softwareMajor;
	unsigned char softwareMinor;
	QString cert = versionStr.at(2);
	cert.remove(0,2);
	certVersion = (unsigned char) cert.toInt(&ok);
	if ( ok )
		m_CertVersion = certVersion;

	hardwareVersion = versionStr.at(3).toInt(&ok);
	if ( ok )
		m_HardwareVersion = hardwareVersion;

	softwareMajor = versionStr.at(4).toInt(&ok);
	if ( ok )
		m_SoftwareMajor = softwareMajor;

	if ( versionStr.count() >= 6){
		softwareMinor = versionStr.at(5).toInt(&ok);
		if ( ok )
			m_SoftwareMinor = softwareMinor;
	}
}
