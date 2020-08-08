#include "updateenvironment.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QStringList>
#include <QFile>
#include <QDir>

#include "meegoeuid.h"
#include "systemversion.h"
#include "firmwareversion.h"
#include "partitionversion.h"
#include "mostversion.h"
#include "rpmversion.h"
#include "updateelement.h"
#include "common.h"

#include "DHAVN_MOSTManager_MOSTStructures.h"

UpdateEnvironment * UpdateEnvironment::m_env = NULL;

UpdateEnvironment::UpdateEnvironment(const QString & mountPath)
: QObject(0)
{
	registMOSTMetaType();
	m_mountPath = mountPath;
	m_systemVersion = new SystemVersion(m_mountPath, this);
	m_mostVersion = new MOSTVersion(m_mountPath, this);
	m_configuration = new Configuration(this);
	m_readConfigResult = m_configuration->readConfiguration(m_mountPath);

	m_firmwareVersion = new FirmwareVersion(m_mountPath, this);
	m_rpmVersion = new RPMVersion(m_mountPath, this);

	if ( m_readConfigResult ){
		m_readConfigResult = m_systemVersion->setUpdateVersion(m_configuration->updateVersion(), m_configuration->buildDate());
		m_mostVersion->setDeviceUpdateList(m_configuration->configUpdateList());
		m_firmwareVersion->setFirmwareUpdateList(m_configuration->configUpdateList(), m_configuration->isUserMode());
		if ( m_configuration->isUserMode()) m_rpmVersion->setMountPath("/upgrade");
	}

	m_partitionVersion = new PartitionVersion(m_mountPath, this);
}

UpdateEnvironment::~UpdateEnvironment(){ }

int UpdateEnvironment::getUpdateMode(){
	if ( m_readConfigResult ){
		return (int)m_configuration->mode();
	}

	return -1;
}

bool UpdateEnvironment::checkUser(){
	return m_configuration->checkUser();
}

QList<UpdateElement *> * UpdateEnvironment::getAvailableUpdates(){
	QList<UpdateElement *> * availableUpdates = new QList<UpdateElement *>();
        ::sleep(4);
	availableUpdates->append(m_partitionVersion->getUpdateList());

	availableUpdates->append(m_mostVersion->getUpdateList());

	availableUpdates->append(m_firmwareVersion->getUpdateList());

	if ( !m_partitionVersion->mainPartitionUpdateExist()){
		ULOG->log("Main Partition UPDATE UnAvailable");
		availableUpdates->append(m_rpmVersion->getUpdateList());
	}

	delete m_mostVersion;
	m_mostVersion = NULL;

	return availableUpdates;
}

bool UpdateEnvironment::checkUpdateKey(){
	if ( !m_configuration->isUserMode())
		return true;
	
	return m_configuration->checkUpdateKey(m_partitionVersion->imageSizeStrings());
}

void UpdateEnvironment::getUpdatePartitions(){
	m_partitionVersion->getUpdatePartitions(m_systemVersion->getRootDevice(), m_systemVersion->getSystemVersionString(), m_configuration->updateVersion());
}

const QString & UpdateEnvironment::getSystemVersionString(){
	return m_systemVersion->getSystemVersionString();
}

bool UpdateEnvironment::checkRuntimeFiles(){
	appendEnvironment("PATH", m_mountPath, "/sbin", "/sbin:");
	appendEnvironment("LD_LIBRARY_PATH", m_mountPath, "/lib", "/lib:");
	appendEnvironment("PATH", UPGRADE_DIR, "/sbin", "/sbin:");
	appendEnvironment("LD_LIBRARY_PATH", UPGRADE_DIR, "/lib", "/lib:");
	QStringList runtimeFiles;
	runtimeFiles << LSOF_KILL_FILE << PGREP_KILL_FILE <<
			CHANGE_PARTITION_FILE << CHANGE_EXTLINUX_FILE <<
			PARTCLONE_CHKIMG_FILE << PARTCLONE_EXT_FILE;

	for (int i = 0; i < runtimeFiles.count(); ++i)
		if ( !checkFileExist(runtimeFiles.at(i)))
			return false;

	return true;
}

void UpdateEnvironment::appendEnvironment(const QString & envName, const QString & mountPoint, const QString & usbDir, const QString & string){
	if ( QDir (mountPoint + usbDir).exists()){
		char * env = NULL;

		QString envStr = mountPoint + string;
		env = ::getenv(qPrintable(envName));

		if ( env )
			envStr.append(env);


		ULOG->log("Add Env(%s) : %s", qPrintable(envName), qPrintable(envStr));
		if ( ::setenv(qPrintable(envName), qPrintable(envStr), 1) != 0){
			ULOG_EXCEPTION; ULOG->log("setenv(%s) Failed", qPrintable(envName));
		}
	}
}

bool UpdateEnvironment::checkFileExist(const QString & fileName){
	QStringList folders;
	folders << m_mountPath + "/sbin" << "/common_data/upgrade/sbin" << "/sbin/" << "/usr/sbin/";
	for (int i = 0; i < folders.count(); ++i)
		if ( QFile::exists(folders.at(i) + fileName))
			return true;
	return false;
}

bool UpdateEnvironment::checkEssentialCheckSum(){
	return m_configuration->checkEssentialKey(m_mountPath);
}

bool UpdateEnvironment::readConfiguration(){
	return m_readConfigResult && m_configuration->validateConfiguration();
}

bool UpdateEnvironment::testConnection(){
	MeegoEUID euid;
	if ( euid.success())	
		return QDBusConnection::sessionBus().isConnected();

	return false;
}

bool UpdateEnvironment::checkVersionRequire(){
	if ( m_configuration->compareMode() == Configuration::NoVersion ||
			m_configuration->requireVersion().isNull() ||
			 m_configuration->requireVersion().isEmpty())
		return true;

	int result = 0;
	if ( !m_systemVersion->compareWithHUVersion(m_configuration->requireVersion(), result))
		return true;

	if ( m_configuration->compareMode() == Configuration::SameVersion && result != 0)
		return false;
	else if ( m_configuration->compareMode() == Configuration::UpperVersion && result != 1)
		return false;
	else if ( m_configuration->compareMode() == Configuration::LowerVersion && result != -1)
		return false;
	else if ( m_configuration->compareMode() == Configuration::DiffVersion && result == 0)
		return false;

	return true;
}

bool UpdateEnvironment::getSystemVersion(){
	return m_systemVersion->getSystemVersion();
}

bool UpdateEnvironment::getSystemVariant(){
	return m_systemVersion->getSystemVariant();
}

bool UpdateEnvironment::getRootDevice(){
	if ( m_systemVersion->getRootDevice().isNull() ||
			m_systemVersion->getRootDevice().isEmpty())
		return false;

	return true;
}

void UpdateEnvironment::registMOSTMetaType(){
	qDBusRegisterMetaType<STransferDataCMSAMS>();
	qDBusRegisterMetaType<SReporgrammingMode>();
	qDBusRegisterMetaType<SEraseMemory>();
	qDBusRegisterMetaType<SModuleVerisonList>();
	qDBusRegisterMetaType<SModuleVersionListInfo>();
	qDBusRegisterMetaType<SRequestDownload>();
	qDBusRegisterMetaType<SPrepareFileTransfer>();
	qDBusRegisterMetaType<SFileInfoforSWDL>();
	qDBusRegisterMetaType<STransferDataCMSAMS>();	
	qDBusRegisterMetaType<SCheckActivateModule>();
	qDBusRegisterMetaType<SRequestTransferExit>();
}

void UpdateEnvironment::restartMOSTmanager(){
	bool restartMOSTmanager = false;

	if(m_systemVersion->getSystemEventVersion().compare("PQ") == 0){
		if((int)m_systemVersion->getSystemMajorVersion() >= 25 && (int)m_systemVersion->getSystemMajorVersion() < 58)
			restartMOSTmanager = true;
	} else if(m_systemVersion->getSystemEventVersion().compare("SO") == 0){
		if((int)m_systemVersion->getSystemMajorVersion() == 3)
			restartMOSTmanager = true;
	}

	if(restartMOSTmanager){
		QString mostmanagerName = m_mountPath;
		mostmanagerName.append("/HU/DHAVN_MOSTmanager");
		
		::system("pgrep MOST | xargs kill -9");
		::sleep(1);
		
		startMOSTmanager(mostmanagerName);
	}
}

void UpdateEnvironment::startMOSTmanager(const QString path)
{
    if (fork()) {
        /* parent */
        return;
    }

	ULOG->log("Restart MOSTmanager : %s", qPrintable(path));
	
	::seteuid(MEEGO_USER_ID);
    execvp(path.toAscii(), NULL);
	::seteuid(ROOT_USER_ID);

	exit(EXIT_SUCCESS);
}
