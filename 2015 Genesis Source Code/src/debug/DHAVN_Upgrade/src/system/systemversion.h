#ifndef	SYSTEM_VERSION_H_
#define	SYSTEM_VERSION_H_

#include <QObject>
#include <QDBusMessage>

class SystemInfo;

class SystemVersion : public QObject{
Q_OBJECT
public:
	SystemVersion(const QString & mountPath, QObject * obj = 0);
	~SystemVersion();

	const QString & getSystemVersionString();
	bool			getSystemVariant();	
	bool			getSystemVersion();
	bool			setUpdateVersion(const QString &, const QString &);
	const QString &	getRootDevice() { return m_currentRootDevice; }

	bool			compareWithHUVersion(const QString & ver, int & result);

	unsigned char	getSystemMajorVersion();
	unsigned char	getSystemMinorVersion();
	QString 		getSystemEventVersion();

private:
	SystemInfo *		m_headUnitInfo;
	SystemInfo *		m_updateInfo;
//	SystemInfo *		m_requireInfo;

	int					m_variant;
	QString				m_currentRootDevice;

	QDBusMessage		m_variantMsg;

	QString				m_mountPath;

	const QString &		findRootDevice();

private slots:
	void				variantReceived(const QByteArray &);
	void				variantError();
};

#endif
