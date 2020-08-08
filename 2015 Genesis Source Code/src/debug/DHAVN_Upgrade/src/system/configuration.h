#ifndef	CONFIGURATION_H_
#define	CONFIGURATION_H_

#include <QObject>
class UpdateElement;

class Configuration : public QObject {
Q_OBJECT
public:
	enum CompareMode {
		SameVersion,
		UpperVersion,
		LowerVersion,
		DiffVersion,
		NoVersion		
	};

	enum UpdateMode {
		UserMode,
		DeveloperMode,
		XMode
	};

	Configuration(QObject * obj = 0);

	bool readConfiguration(const QString & path);
	bool validateConfiguration();
	bool checkUser();
	bool checkEssentialKey(const QString & path);
	bool checkUpdateKey(const QString & imageStringSize);

	const QString & updateVersion() { return m_updateVersion; }
	const QString & buildDate()		{ return m_buildDate; }
	bool		isUserMode()		{ return (m_user == UserMode); }
	const QString & requireVersion() { return m_requireVersion; }

	CompareMode	compareMode() { return m_compareMode; }
	UpdateMode mode() { return m_user; }

	const QList<UpdateElement *> & configUpdateList() { return m_targetList; }

private:
	QString		m_buildDate;
	QString		m_requireVersion;

	QString		m_updateVersion;

	QString		m_essentialKey;
	QString		m_mainKey;

	CompareMode	m_compareMode;
	void		setCompareMode(const QString &);
	
	UpdateMode	m_user;
	QString		m_password;
	void		setUpdateMode(const QString &);

	QList<UpdateElement *> m_targetList;
};

#endif
