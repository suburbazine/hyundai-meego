#ifndef	RPM_VERSION_H_
#define	RPM_VERSION_H_

#include "abstractversion.h"
class StdinProcess;

#include <QXmlStreamReader>

class RPMVersion : public AbstractVersion {
Q_OBJECT
public:
	enum Mode {
		NoneMode,
		GetPackageMode
	};
	RPMVersion(const QString & aMountPath, QObject * obj = 0);
	void filterUpdateList();

private:
	void deleteLockFiles();
	void changeRepository();
	void getRepoPackages();


	void removeRepository();
	void removeAllRepository();
	void addRepo(const QString & path);
	void cleanRepository();
	void refreshRepository();

	void processPackageList();
	void parsePackage(const QString & pack);

	void syncZypperCommand(const QString & cmd);

private:
	StdinProcess * currentInstance;
	QXmlStreamReader reader;
	Mode mode;

private slots:
	void processStdin(const QString & output);

signals:
	void endStream();
};

#endif
