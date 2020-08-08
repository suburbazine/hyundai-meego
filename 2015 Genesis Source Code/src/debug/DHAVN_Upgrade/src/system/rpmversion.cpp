#include "rpmversion.h"
#include "common.h"
#include "stdinprocess.h"

#include <QEventLoop>

RPMVersion::RPMVersion(const QString & aMountPath, QObject * obj): AbstractVersion(aMountPath, obj){
	currentInstance = NULL;
	mode = NoneMode;
}

void RPMVersion::filterUpdateList(){
	deleteLockFiles();
	changeRepository();
	getRepoPackages();	
}

void RPMVersion::getRepoPackages(){
	mode = GetPackageMode;	
	syncZypperCommand(Q_ZYPPER_XML_CMD + "pa");
}

void RPMVersion::deleteLockFiles(){
	QDir d(RPM_DB_DIR);
	QFileInfoList list = d.entryInfoList();
	for (int i = 0; i < list.count(); ++i){
		if ( list.at(i).isFile() && list.at(i).baseName().startsWith("__")){
			ULOG->log("Remove LockFile : %s", qPrintable(list.at(i).absoluteFilePath()));	
			QFile::remove(list.at(i).absoluteFilePath());
		}
	}
}

void RPMVersion::changeRepository(){
	removeRepository();
	removeAllRepository();
	addRepo(mountPath() + ZYPPER_USB_REPO_DIR_SUFFIX);
	cleanRepository();
	refreshRepository();
}

void RPMVersion::addRepo(const QString & p){
	QString path = p;
	syncZypperCommand(Q_ZYPPER_XML_CMD + "ar " + path.replace(QChar(' '), "\\ ") + " hu");
}

void RPMVersion::removeRepository(){
	currentInstance = StdinProcess::CreateInstance();
	connect(currentInstance, SIGNAL(output(const QString &)), SLOT(processStdin(const QString &)));
	syncZypperCommand(Q_ZYPPER_XML_CMD + "rr hu");
}

void RPMVersion::removeAllRepository(){
	QDir d(ZYPPER_REPO_DIR);
	QFileInfoList list = d.entryInfoList();	
	for ( int i = 0; i < list.count(); ++i)
		if ( list.at(i).isFile())
			QFile::remove(list.at(i).absoluteFilePath());
}

void RPMVersion::cleanRepository(){
	syncZypperCommand(Q_ZYPPER_XML_CMD + "clean -a -r hu");
}

void RPMVersion::refreshRepository(){
	syncZypperCommand(Q_ZYPPER_XML_CMD + "refresh");
}

void RPMVersion::syncZypperCommand(const QString & m_cmd){
	QEventLoop loop;
	connect(this, SIGNAL(endStream()), &loop, SLOT(quit()));
	currentInstance->execute(m_cmd);
	loop.exec();
}

void RPMVersion::parsePackage(const QString & pack){
	QStringList tokenize = pack.split("|");

	if ( tokenize.count() != 5)
		return;

	QString status = tokenize.at(0).trimmed();
	bool isUpdate = false;

	if ( status == "i")	// does not need to update
		return;
	else if ( status == "v")	// update
		isUpdate = true;
	else if ( status == "S")	// this is HEADER for zypper xmlout
		return;
	else
		isUpdate = false;

	/* For VR Upgrade */
	if(tokenize.at(2).contains("vr") && tokenize.at(3).contains("full")){
		isUpdate = true;
	} else if(tokenize.at(2).contains("vr") && tokenize.at(3).contains("binary")){
		return;
	}

	if ( !tokenize.at(2).trimmed().isEmpty()){
		ULOG->log("Add RPMS(%s) : %s, %s", qPrintable(tokenize.at(2).trimmed()), qPrintable(tokenize.at(3).trimmed()), (isUpdate?"UPDATE":"INSTALL"));
		UpdateElement * ue = UpdateElement::CreateRPMElement(tokenize.at(2).trimmed(), tokenize.at(3).trimmed(), (isUpdate?UpdateElement::rpm_UPDATE : UpdateElement::rpm_INSTALL));
		appendTarget(ue);
	}
	
}



void RPMVersion::processPackageList(){
	QXmlStreamReader::TokenType type;
	while( !reader.atEnd()){
		reader.readNext();
		type = reader.tokenType();
		if ( type == QXmlStreamReader::StartElement && reader.name().toString() == "stream"){
			QString list = reader.readElementText(QXmlStreamReader::SkipChildElements);
			list = list.trimmed();
			
			QStringList byLineFeed = list.split("\n");

			if ( byLineFeed.count() > 2){
				byLineFeed.removeFirst();
				byLineFeed.removeFirst();
				for ( int i = 0; i < byLineFeed.count(); ++i)
					parsePackage(byLineFeed.at(i));
			}
			break;
		}
	}
}

void RPMVersion::processStdin(const QString & output){
	reader.addData(output + "\n");

	if ( output.contains("</stream>")){
		emit endStream();
		if ( mode == GetPackageMode )
			processPackageList();
		
		reader.clear();
		mode = NoneMode;
	}
}
