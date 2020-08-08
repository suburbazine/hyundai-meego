#ifndef	UPDATE_ENVIRONMENT_H_
#define	UPDATE_ENVIRONMENT_H_

#include <QObject>
#include "configuration.h"
class QDBusMessage;
class SystemVersion;
class FirmwareVersion;
class MOSTVersion;
class RPMVersion;
class PartitionVersion;
class UpdateElement;

class UpdateEnvironment : public QObject{
Q_OBJECT
private:
	UpdateEnvironment(const QString & mountPath);
	~UpdateEnvironment();

	SystemVersion *		m_systemVersion;
	FirmwareVersion *	m_firmwareVersion;
	MOSTVersion *		m_mostVersion;
	PartitionVersion *	m_partitionVersion;
	RPMVersion *		m_rpmVersion;
	
	Configuration *		m_configuration;

	QString				m_mountPath;
	bool				m_readConfigResult;

	void				appendEnvironment(const QString & envName,
											const QString & mountPoint,
											const QString & usbDir,
											const QString & string);
	bool				checkFileExist(const QString & fileName);
	static UpdateEnvironment * m_env;

public:
	static UpdateEnvironment * instance(){
		return m_env;
	}

	static UpdateEnvironment * instance(const QString & mountPath){
		if ( m_env == NULL)
			m_env = new UpdateEnvironment(mountPath);

		return m_env;
	}

	QList<UpdateElement *> * getAvailableUpdates();

	const QString & mountPath() { return m_mountPath; }

	void registMOSTMetaType();
	void restartMOSTmanager();
	void startMOSTmanager(const QString);

	const QString & getSystemVersionString();
	bool getSystemVersion();
	bool getSystemVariant();
	bool getRootDevice();
	bool readConfiguration();
	bool testConnection();

	bool checkUser();
	bool checkEssentialCheckSum();
	bool checkRuntimeFiles();
	bool checkVersionRequire();

	void getUpdatePartitions();

	bool checkUpdateKey();

	int getUpdateMode();
};


#endif
