#include "rpmupdater.h"
#include "common.h"
#include "stdinprocess.h"
#include "updateenvironment.h"
#include <QDebug>
#include <QDir>
#include <sys/mount.h>

#define VERSION_TXT		"/etc/version.txt"
#define TMP_VERSION_TXT	"/etc/version.txt.tmp"

RPMUpdater::RPMUpdater(QObject * obj)
: AbstractUpdater(AbstractUpdater::RPMLauncher, obj){
	mode = NoneMode;
	currentInstance = NULL;
	m_stopFlag = false;
	m_appVRFlag = false;

	time = 0;
	timer = new QTimer(this);
	timer->setInterval(2000);
	connect(timer, SIGNAL(timeout()), SLOT(drawProgress()));
}

RPMUpdater::~RPMUpdater(){ 
	while (!m_updatePackageProgress.isEmpty())
		delete m_updatePackageProgress.takeFirst();

	while (!m_installPackageProgress.isEmpty())
		delete m_installPackageProgress.takeFirst();
}

void RPMUpdater::setUpdateList(const QList<UpdateElement *> & list){
	for (int i = 0; i < list.count(); ++i){
		if (list.at(i)->type() == UpdateElement::et_RPM){
			if ( list.at(i)->rpmType() == UpdateElement::rpm_UPDATE){
				if( list.at(i)->name().contains("vr")){
					m_appVRFlag = true;
					if(i == 0)
						m_updatePackageList.append(list.at(i));
					else
						m_updatePackageList.insert(0, list.at(i));
				} else
					m_updatePackageList.append(list.at(i));
			
			} else if ( list.at(i)->rpmType() == UpdateElement::rpm_INSTALL)
				m_installPackageList.append(list.at(i));
		}
	}
}

bool RPMUpdater::finish() { 
	QFile file(UpdateEnvironment::instance()->mountPath() + "/version.txt");

	if(!m_stopFlag){
		if(m_appVRFlag && file.exists()){
			QFile::rename(VERSION_TXT, TMP_VERSION_TXT);
			if(!file.copy(VERSION_TXT)){
				QFile::rename(TMP_VERSION_TXT, VERSION_TXT);
				return true;
			}
			QFile::remove(TMP_VERSION_TXT);
			ULOG->log("VR Upgrade, Version Changing");
		}
	}
	return true;
}

void RPMUpdater::start(){
	ULOG->log("Run RPMUpdater update : %d, install : %d", m_updatePackageList.count(), m_installPackageList.count());
	if ( m_updatePackageList.count() == 0 &&
			m_installPackageList.count() == 0){
		emit finished(0);
		return;
	}


	currentInstance = StdinProcess::CreateInstance();
	QObject::connect(currentInstance,
				SIGNAL(output(const QString &)), SLOT(processStdin(const QString &)));
	if ( m_updatePackageList.count()){
		mode = DownloadMode;
		downloadPackages();
	}
	else{
		installPackages();
	}
}

void RPMUpdater::stop(){
	m_stopFlag = true;
}

void RPMUpdater::updatePackages(){
	if(m_stopFlag)
		installPackages();
	else {
		mode = UpdateMode;

		QString command = Q_ZYPPER_XML_CMD + "--pkg-cache-dir " + UPGRADE_DIR + " update ";
		for (int i = 0; i < m_updatePackageList.count(); ++i){
			command.append(m_updatePackageList.at(i)->name()).append(" ");
		}
		currentInstance->execute(command);
	}
}

void RPMUpdater::installPackages(){
	mode = InstallMode;

	if ( m_installPackageList.count()){
		QString command = Q_ZYPPER_XML_CMD + "--pkg-cache-dir " + UPGRADE_DIR + " install ";
		for (int i = 0; i < m_installPackageList.count(); ++i){
			command.append(m_installPackageList.at(i)->name()).append(" ");
			emit updateProgressChanged( m_installPackageList.at(i)->name(),
										"Preparing install", 0, 10);
		}
		currentInstance->execute(command);
	}
	else{
		ULOG->log("RPMUpdater:installSoftware : No Install Update");
		emit finished(0);
	}
}

void RPMUpdater::downloadPackages(){
	QString command = Q_ZYPPER_XML_CMD + "--pkg-cache-dir " + UPGRADE_DIR + " update --download-only ";
	for (int i = 0; i < m_updatePackageList.count(); ++i){
		command.append(m_updatePackageList.at(i)->name()).append(" ");
		emit updateProgressChanged( m_updatePackageList.at(i)->name(),
									"Preparing Update", 0, 10);
	}
	currentInstance->execute(command);

	if(m_appVRFlag){
		::mount("/vr", "/vr", "ext4", MS_REMOUNT | MS_MGC_VAL, NULL);	
		//timer->start();
	}
}

void RPMUpdater::drawProgress(){
	time++;
	//emit updateProgressChangedDirect("appvr", "Unpacked...", time, 10);
}

void RPMUpdater::processStdin(const QString & output){
	ULOG->log(qPrintable(output));	
	reader.addData(output + "\n");

	if ( output.contains("</stream>")){
		Mode finishedMode = mode;
		reader.clear();
		mode = NoneMode;
		if (finishedMode == DownloadMode){
			updatePackages();
		}
		else if ( finishedMode == UpdateMode){
			installPackages();
		}
		else if ( finishedMode == InstallMode){
			ULOG->log("INSTALL FINISHED");
			emit finished(0);
		}
		else{
			ULOG_EXCEPTION;
		}
	}	
	else{
		if ( mode == DownloadMode || mode == UpdateMode || mode == InstallMode){
			parseZypperData();
		}
	}		
}

void RPMUpdater::parseZypperData(){
	QXmlStreamReader::TokenType type;
	while( !reader.atEnd()){
		reader.readNext();
		type = reader.tokenType();
		if ( type == QXmlStreamReader::StartElement){
			if ( reader.name().toString() == "solvable"){
				QXmlStreamAttributes attr = reader.attributes();
				addTrackPackage(attr);
			}
			else if ( reader.name().toString() == "progress"){
				QXmlStreamAttributes attr = reader.attributes();
				processProgress(attr);
			}
			else if ( reader.name().toString() == "message"){
				QXmlStreamAttributes attr = reader.attributes();
				checkErrorMsg(attr);
			}
		}
	}
}

QList<PackageProgress *> * RPMUpdater::getCurrentPackageList(){
	switch(mode){
	case DownloadMode:
	case UpdateMode:
		return &m_updatePackageProgress;
	case InstallMode:
		return &m_installPackageProgress;
	default:
		ULOG_EXCEPTION;
		break;
	}
	return NULL;
}

void RPMUpdater::addTrackPackage(const QXmlStreamAttributes & attr){
	if ( attr.hasAttribute("", "name")){

		if ( getCurrentPackageList() == NULL){
			ULOG_EXCEPTION;
			return;
		}

		PackageProgress * progress = new PackageProgress;
		progress->package_name = attr.value("", "name").toString();
		progress->progress = 0;
		getCurrentPackageList()->append(progress);	
	}	
}

void RPMUpdater::checkErrorMsg(const QXmlStreamAttributes & attr){
	for (int i = 0; i < attr.count(); ++i){
		if(attr.at(i).value().toString() == "error"){
			ULOG->log("rpmUpdater stopFlag set to true");
			m_stopFlag = true;
		}
	}
}

void RPMUpdater::processProgress(const QXmlStreamAttributes & attr){
	QString name = attr.value("", "name").toString();
	PackageProgress * progressPackage = NULL;

	if ( getCurrentPackageList() == NULL){
		ULOG_EXCEPTION;
		return;
	}

	for (int i = 0; i < getCurrentPackageList()->count(); ++i){
		if ( name.contains(getCurrentPackageList()->at(i)->package_name)){
			progressPackage = getCurrentPackageList()->at(i);
			break;
		}
	}

	if ( attr.hasAttribute("", "value")){
		if(timer->isActive())
			timer->stop();

		for (int i = 0; i < attr.count(); ++i){
			if (attr.at(i).name().toString() == "value"){
				if ( progressPackage != NULL){
					progressPackage->progress = attr.at(i).value().toString().toInt();
					emit updateProgressChanged(progressPackage->package_name, "Install...", progressPackage->progress, 10);
				}
			}
		}
	}
	else if ( progressPackage && attr.hasAttribute("", "done")){
		progressPackage->progress = 100;
		emit updateProgressChanged(progressPackage->package_name, "Finished", 100, 100);
	}
}


