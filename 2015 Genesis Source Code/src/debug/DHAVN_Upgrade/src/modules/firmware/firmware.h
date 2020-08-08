#ifndef	FIRMWARE_H_
#define	FIRMWARE_H_

#include <QObject>
#include <QStringList>

class Firmware : public QObject{
Q_OBJECT
public:
	Firmware(QObject * obj = 0);

	void setBase(int base) { m_base = base; }
	void setFirmwareVersion(const QByteArray &);
	void setFileVersion(const QString &);
	void setVersionSeparator(QChar ch);

	bool valid() { return m_isValid; }

	int compare();

	enum DependencyCode {
		DEP_NONE = 0,
		DEP_MICOM = 1,
		DEP_MOST_END = 2,
                DEP_SSD = 3,    // SANGWOO_TEMP
		DEP_ALL_FINISHED = 8
	};

	static QString GetLauncherByName(const QString & name);
	static DependencyCode GetDependencyByName(const QString & name);
	static unsigned int GetWeightByName(const QString & name);

private:
	QByteArray m_firmwareVersion;
	QByteArray m_fileVersion;

	QChar m_versionSeparator;
	bool m_isValid;

	int m_base;
};

#endif
