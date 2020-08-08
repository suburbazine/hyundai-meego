#ifndef	COPY_USB_TO_HDD_H_
#define	COPY_USB_TO_HDD_H_

#include <QThread>
#include <QFile>
#include <QFileInfoList>

class CopyUsbToHdd : public QThread{
Q_OBJECT

public:
	CopyUsbToHdd(const QString & from,
					const QString & to, QObject * obj = 0);
	virtual ~CopyUsbToHdd();
	unsigned char * getMD5Sum(int index);
	qulonglong		getFileSize(int index);
	QString			getFileName(int index, bool isShort = true);
	bool			isFilesExist();
	int				getFileCount();
	bool			checksumContained;
	bool			compareMD5();

private:
	void	run();

	QFileInfoList			m_fileList;
	QList<unsigned char *>	m_md5SumList;
	QList<unsigned char *>  m_calcSumList;
	QString m_to;
	qint64	m_fileSize;
	quint64	m_copiedSize;

	bool	m_iboxFileExist;
	void	readMD5Sum(const QString &);

signals:
	void	copied(quint64, quint64);
};

#endif
