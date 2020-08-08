#include "mostversion.h"
#include "meegoeuid.h"
#include "most.h"

#include "copyusbtohdd.h"
#include "common.h"

#include <QDir>

#define MOST_VER_TIMER	3000

MOSTVersion::MOSTVersion(const QString & aMountPath, QObject * obj)
: AbstractVersion(aMountPath, obj){
	MeegoEUID euid;	

	m_iboxUpdate = m_ampUpdate = NULL;

	swdlInterface = new local::CSWDLAdaptor("com.dhavn.mostmanager.qtdbus", "/swdl", QDBusConnection::sessionBus());
	m_versionTimer = new QTimer(this);
	m_versionTimer->setSingleShot(true);

	diagnosisInterface = new local::CDiagnosisAdaptor("com.dhavn.mostmanager.qtdbus", "/diagnosis", QDBusConnection::sessionBus());

	QObject::connect(swdlInterface, SIGNAL(ReadHWIdentifier(uint, const QString &)), SLOT(amplifierHwIdentifier(uint, const QString &)));

	QObject::connect(diagnosisInterface, SIGNAL(MainSWVersionStatus(const QStringList &)), SLOT(iBoxSWVersionStatus(const QStringList &)));
}

void MOSTVersion::filterUpdateList(){
	if ( m_iboxUpdate )
		startGetIBoxVersion();

	if ( m_ampUpdate )
		startGetAmplifierVersion();
}

void MOSTVersion::startGetIBoxVersion(){
	QEventLoop loop;
	connect(m_versionTimer, SIGNAL(timeout()), &loop, SLOT(quit()));
	QObject::connect(diagnosisInterface, SIGNAL(MainSWVersionStatus(const QStringList &)), &loop, SLOT(quit()));

	diagnosisInterface->GetMainSWVersion();	
	m_versionTimer->start(MOST_VER_TIMER);

	loop.exec();
        disconnect(diagnosisInterface, SIGNAL(MainSWVersionStatus(const QStringList &)), 0, 0);
	disconnect(m_versionTimer, SIGNAL(timeout()), 0, 0);

	if ( m_iboxUpdate->version().toLower() == "none")
		m_iboxHuVariant = "KR";
	
	int fileCount = getFileCount(mountPath() + "/IBOX/" + m_iboxHuVariant, SWDLID::IBOX);

	if ( fileCount > 0){
		if ( m_iboxUpdate->version().toLower() == "none"){
			m_iboxUpdate->setPath(QString(IBOX_DIRECTORY) + "KR");

			if ( copyIboxFiles()){
				ULOG->log("IBOX Update Available");
				appendTarget(m_iboxUpdate);
				return;
			}
		}
		else if ( !m_iboxHuVersion.isEmpty() && !m_iboxHuDate.isEmpty() && !m_iboxHuVariant.isEmpty()){
			if (compareIboxVersion(m_iboxHuVersion, m_iboxHuDate)){
				m_iboxUpdate->setPath(IBOX_DIRECTORY + m_iboxHuVariant);
				if (copyIboxFiles()){
					ULOG->log("IBOX Update Available");
					appendTarget(m_iboxUpdate);
					return;
				}
			}
		}
	}
	ULOG->log("IBOX Update Not Available");
}

void MOSTVersion::startGetAmplifierVersion(){
	QEventLoop loop;
	connect(m_versionTimer, SIGNAL(timeout()), &loop, SLOT(quit()));
	QObject::connect(swdlInterface, SIGNAL(ReadHWIdentifier(uint, const QString &)), &loop, SLOT(quit()));

	swdlInterface->SetReadHWIdentifier(SWDLID::AMP, MOSTDef::ID_STATE_GET, "");
	m_versionTimer->start(MOST_VER_TIMER);
	loop.exec();
	disconnect(m_versionTimer, SIGNAL(timeout()), 0, 0);

	if ( !m_amplifierVersion.isEmpty() &&
			m_amplifierVersion.toLower() != m_ampUpdate->version().toLower() &&
			getFileCount(mountPath() + "/AMP", SWDLID::AMP)){
		m_ampUpdate->setPath(AMP_DIRECTORY);
		if ( copyAmplifierFiles()){
			ULOG->log("AMP Update Available");
			appendTarget(m_ampUpdate);
			return;
		}
	}
	ULOG->log("AMP Update Not Available");
}

bool MOSTVersion::compareIboxVersion(const QString & aIboxVer, const QString & aIboxDate){
	bool ok;
	QString iboxVer, fileVer;
	iboxVer = aIboxVer;
	fileVer = m_iboxUpdate->version();
	int iboxVerInt = iboxVer.remove(0, 1).toInt(&ok);
	if ( !ok ) return false;

	int fileVerInt = fileVer.remove(0, 1).toInt(&ok);
	if ( !ok ) return false;

	if ( iboxVerInt >= 198 && ( iboxVerInt != fileVerInt || aIboxDate != m_iboxUpdate->date()))
		return true;

	return false;
}

void MOSTVersion::setDeviceUpdateList(const QList<UpdateElement *> & list){
	for (int i = 0; i < list.count(); ++i){
		if ( list.at(i)->name() == "IBOX")
			m_iboxUpdate = list[i];
		else if ( list.at(i)->name() == "AMP")
			m_ampUpdate = list[i];	
	}
}

void MOSTVersion::amplifierHwIdentifier(uint id, const QString & msg){
	if ( id == SWDLID::AMP){
		if ( m_versionTimer->isActive()) m_versionTimer->stop();
		ULOG->log("AMP HU Version : %s", qPrintable(msg));
		m_amplifierVersion = msg.trimmed();
	}
}

void MOSTVersion::iBoxSWVersionStatus(const QStringList & list){
	if ( m_versionTimer->isActive())
		m_versionTimer->stop();

	for (int i = 0; i < list.count(); ++i){
		if ( list.at(i).startsWith("PR_DH_")){
			QStringList versionList = list.at(i).split(" ");
			if ( versionList.count() == 3){
				QString iboxDate = versionList.at(2);
				m_iboxHuVersion = versionList.at(1);

				QStringList iboxDateList = iboxDate.split(".");
				if ( iboxDateList.count() >= 2) m_iboxHuDate = iboxDateList.at(0);

				QStringList variantList = versionList.at(0).split("_");
				if ( variantList.count() >= 3){
					m_iboxHuVariant = variantList.at(2);
				}
			}
			ULOG->log("IBOX HU Version : %s, date : %s, variant : %s", qPrintable(m_iboxHuVersion), qPrintable(m_iboxHuDate), qPrintable(m_iboxHuVariant));
		}
	}
}

int MOSTVersion::getFileCount(const QString & mountPoint, int type){
	int count = 0;
       
        QDir mDir(mountPoint);
        if(!mDir.exists()){
            ULOG->log("getFileCount not exists : %s", qPrintable(mountPoint));
             return false;
        }

	QFileInfoList list = QDir(mountPoint).entryInfoList();
	bool existMD5 = false;

	for (int i = 0; i < list.count(); ++i){
		if ( list.at(i).isFile()){
			if ( list.at(i).fileName() == "transfer.md5")
				existMD5 = true;
			else
				count++;
		}
	}

	if ( type == SWDLID::IBOX && !existMD5)
		return false;

	return count;
}

bool MOSTVersion::copyIboxFiles(){
	QEventLoop loop;
	createFolder( QString(IBOX_DIRECTORY) + m_iboxHuVariant);
	CopyUsbToHdd * copyObject = new CopyUsbToHdd(mountPath() + "/IBOX/" + m_iboxHuVariant, QString(IBOX_DIRECTORY) + m_iboxHuVariant, this);
	connect(copyObject, SIGNAL(finished()), &loop, SLOT(quit()));
	copyObject->start();

	loop.exec();

	for (int i = 0; i < copyObject->getFileCount(); ++i){
		gf_append_file_checksum(&g_ibox_file_checksum, 
									copyObject->getFileName(i, false),
									QString(IBOX_DIRECTORY) + m_iboxHuVariant + "/",
									copyObject->getFileSize(i),
									copyObject->getMD5Sum(i));
			
	}

	QTimer::singleShot(0, copyObject, SLOT(deleteLater()));
	return copyObject->compareMD5();
}

bool MOSTVersion::copyAmplifierFiles(){
	QEventLoop loop;
        createFolder(QString(AMP_DIRECTORY));
	CopyUsbToHdd * copyObject = new CopyUsbToHdd(mountPath() + "/AMP", QString(AMP_DIRECTORY), this);
	connect(copyObject, SIGNAL(finished()), &loop, SLOT(quit()));
	copyObject->start();

	loop.exec();
	
	for (int i = 0; i < copyObject->getFileCount(); ++i){
		gf_append_file_checksum(&g_amplifier_file_checksum,
								copyObject->getFileName(i, false),
                                                                QString(AMP_DIRECTORY),
								copyObject->getFileSize(i),
								copyObject->getMD5Sum(i));
	}
	QTimer::singleShot(0, copyObject, SLOT(deleteLater()));
	return copyObject->compareMD5();
}


