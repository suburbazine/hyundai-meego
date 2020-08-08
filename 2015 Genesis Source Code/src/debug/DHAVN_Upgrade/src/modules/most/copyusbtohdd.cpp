#include "copyusbtohdd.h"
#include <openssl/md5.h>
#include <QStringList>
#include <QDir>

#include <stdio.h>
#include <unistd.h>

#include "common.h"

// Copy USB File to HDD by 512kb
//

#include <QDebug>

#define SSD_BUF		524288

CopyUsbToHdd::CopyUsbToHdd(const QString & from, const QString & to, QObject * obj)
: QThread(obj){

	m_fileSize = 0;
	m_copiedSize = 0;
	m_iboxFileExist = true;
	checksumContained = false;
	QString checksumFileName;

	qDebug() << "from : " << from;

	if ( !from.isEmpty()){
		QDir dir(from);
		dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::Hidden);

		m_to = to;
		m_fileList = dir.entryInfoList();
		
		for (int i = 0; i < m_fileList.count(); ++i){
			qDebug() << "file name : " << m_fileList.at(i).fileName();
			if ( m_fileList.at(i).fileName() == "transfer.md5"){
				checksumContained = true;
				checksumFileName = m_fileList.at(i).absoluteFilePath();
				m_fileList.removeAt(i);
			}
		}
		
		qDebug() << "filecount : " << m_fileList.count();
		for (int i = 0; i < m_fileList.count(); ++i){
			m_fileSize += m_fileList.at(i).size();
			qDebug() << "filesize : " << m_fileSize;
			m_md5SumList.append(new unsigned char[MD5_DIGEST_LENGTH]);
			m_calcSumList.append(new unsigned char[MD5_DIGEST_LENGTH]);
		}

		if ( checksumContained)
			readMD5Sum(checksumFileName);
	}
}

CopyUsbToHdd::~CopyUsbToHdd(){
	while( !m_md5SumList.isEmpty())
		delete m_md5SumList.takeFirst();

	while( !m_calcSumList.isEmpty())
		delete m_calcSumList.takeFirst();
}

unsigned char * CopyUsbToHdd::getMD5Sum(int index){
	if ( index >= 0 && index < m_fileList.count())
		return m_md5SumList.at(index);

	return NULL;
}

bool CopyUsbToHdd::isFilesExist(){
	if ( m_fileList.count())
		return true;

	return false;
}

int CopyUsbToHdd::getFileCount(){
	return m_fileList.count();
}

qulonglong CopyUsbToHdd::getFileSize(int index){
	if ( index >= 0 && index < m_fileList.count())
		return m_fileList.at(index).size();

	return 0;
}

QString CopyUsbToHdd::getFileName(int index, bool isShort){
	if ( index >= 0 && index < m_fileList.count()){
		if ( !isShort){
			return m_to + "/" + m_fileList.at(index).fileName();
		}
		else{
			return m_fileList.at(index).fileName();
		}
	}

	return QString();
}

void CopyUsbToHdd::readMD5Sum(const QString & info){
	QFile f(info);
	if (f.open(QIODevice::ReadOnly)){
		char buf[1024];
		qint64 lineLength;
		while ( (lineLength = f.readLine(buf, sizeof(buf))) != -1){
			int i;
			QString md5SumInfo = QString(buf).trimmed();
			QStringList section = md5SumInfo.split(" : ");
			if ( md5SumInfo.isEmpty() || md5SumInfo.startsWith("#") || section.count() != 2)
				continue;

			for (i = 0; i < m_fileList.count(); ++i){
				//ULOG->log("MD5Sum File : %s, %s", qPrintable(m_fileList.at(i).fileName()), qPrintable(section.at(1)));
				if (m_fileList.at(i).fileName() == QFileInfo(section.at(0)).fileName()){
					int index = 0;
					int md5_index = 0;
					QByteArray ar = section.at(1).toAscii();	
					char buf[3] = {0};
					char test_buf[MD5_DIGEST_LENGTH * 2 + 1] = {0};
					char * ptest = test_buf;
				
	
					while( md5_index < MD5_DIGEST_LENGTH){
						QString hexString;
						bool ok;
						buf[0] = (char) ar.at(index++);
						//ULOG->log("buf[0] : %c", buf[0]);
						buf[1] = (char) ar.at(index++);
						//ULOG->log("buf[1] : %c", buf[1]);
						hexString = buf;
						unsigned char num = (unsigned char)hexString.toInt(&ok, 16);
						//ULOG->log("convert : %02X", num);	
						m_md5SumList[i][md5_index++] = num;
					}
					for (int j = 0; j < MD5_DIGEST_LENGTH; ++j){
						snprintf(ptest, 2, "%02X", m_md5SumList[i][j]);
						ptest += 2;
					}
					break;
				}
			}

			if (i == m_fileList.count()) {
				ULOG->log("%s does not exist in the USB.", qPrintable(QFileInfo(section.at(0)).fileName()));
				m_iboxFileExist = false;
			}
		}
	}	
}

bool CopyUsbToHdd::compareMD5(){
	if (!m_iboxFileExist)
		return false;

	for (int i = 0; i < m_fileList.count(); ++i)
		for ( int j = 0; j < MD5_DIGEST_LENGTH; ++j)
			if ( m_md5SumList[i][j] != m_calcSumList[i][j]){
				ULOG->log("Checksum Calculate Error");
				return false;
			}

	return true;
}

void CopyUsbToHdd::run(){
	char * buf = new char[SSD_BUF];
	qint64			readn = -1;
	m_copiedSize = 0;	
	quint64			writen;

	// does need to use mutex?

	if ( !isFilesExist()){
		delete [] buf;
		return;
	}

	for (int i = 0; i < m_fileList.count(); ++i){
		MD5_CTX			c;
	
		MD5_Init(&c);

		QFile			source(m_fileList.at(i).absoluteFilePath());
		source.open(QIODevice::ReadOnly);
		QFile			destination(m_to +"/" +  m_fileList.at(i).fileName());
		destination.open(QIODevice::WriteOnly | QIODevice::Truncate);

		while( ( readn = source.read(buf, SSD_BUF)) > 0){
			fprintf(stderr, "MD5_Update : %d\n", readn);
			writen = readn;
			MD5_Update(&c, buf, (ssize_t) readn);

			while(1){
				writen -= destination.write(buf, writen);
				if ( writen <= 0)
					break;
			}
			m_copiedSize += readn;
			emit copied(m_copiedSize, m_fileSize);
		}

		MD5_Final(m_calcSumList[i], &c);

		source.close();
		destination.close();
	}
	qDebug() << "Copy End";
	delete [] buf;
}
