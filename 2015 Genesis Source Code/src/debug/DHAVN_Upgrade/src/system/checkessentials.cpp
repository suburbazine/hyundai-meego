#include "checkessentials.h"
#include "common.h"
#include <openssl/md5.h>
#include <unistd.h>
#include <QFile>
#include <QProcess>
#include <QDebug>

CheckEssentials::CheckEssentials(const QString & mountPoint, 
									const QString & checkSum){
	m_essentialValidate = false;
	QString md5Sum = getEssentialChecksum(mountPoint + ESSENTIAL_R_PATH);	
	qDebug() << "<MD5SUM> : " << md5Sum;
	if ( md5Sum.toLower() == checkSum.toLower()){
		qDebug() << "ok";
		// extract files
		::chdir(UPGRADE_DIR);

		QProcess process;
		QString command = QString("tar xmf ") + mountPoint + ESSENTIAL_R_PATH;
		process.start(command);
		if ( process.waitForFinished())
			m_essentialValidate = true;
	}		
}

bool CheckEssentials::isEssentialValidate(){
	return m_essentialValidate;
}

QString CheckEssentials::getEssentialChecksum(const QString & essentialPath){
	QFile source(essentialPath);
	if ( !source.exists()){
		return QString();
	}

	char * buf = new char[10240];
	unsigned char md5_digest[MD5_DIGEST_LENGTH];
	char md5_digest_str[MD5_DIGEST_LENGTH * 2 + 1];
	memset(md5_digest_str, 0, MD5_DIGEST_LENGTH * 2 + 1);

	MD5_CTX	c;
	MD5_Init(&c);
	int readn;
	int index = 0;

	source.open(QIODevice::ReadOnly);
	while( ( readn = source.read(buf, 10240)) > 0){
		MD5_Update(&c, buf, (ssize_t) readn);
	}
	delete [] buf;
	MD5_Final(md5_digest, &c);
	source.close();

	for (int i = 0; i < MD5_DIGEST_LENGTH; ++i){
		snprintf(md5_digest_str + index, 3, "%02x", md5_digest[i]);
		
		index += 2;
	}

	return QString(md5_digest_str);
}


