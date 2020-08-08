#ifndef	RPM_UPDATER_H_
#define	RPM_UPDATER_H_

#include <QTimer>
#include <QXmlStreamReader>
#include "abstractupdater.h"

class UpdateType;
class StdinProcess;
class UpdateElement;

typedef struct{
	QString package_name;
	int		progress;	
} PackageProgress;

class RPMUpdater : public AbstractUpdater{
Q_OBJECT
public:
	enum  Mode{
		NoneMode,
		UpdateMode,
		InstallMode,
		DownloadMode,
	};
	RPMUpdater(QObject * obj = 0);
	~RPMUpdater();

	void startUpdate(const QList<UpdateType> & updateList, bool needUpdate);

	void start();
	void stop();
	bool finish();

	void setUpdateList(const QList<UpdateElement *> & list);

private:
	Mode				mode;
	StdinProcess *		currentInstance;
	bool					m_stopFlag;
	bool					m_appVRFlag;

	QList<UpdateElement *> m_updatePackageList;
	QList<UpdateElement *> m_installPackageList;

	QList<PackageProgress * > m_updatePackageProgress;
	QList<PackageProgress * > m_installPackageProgress;

	QList<PackageProgress * > * getCurrentPackageList();

	void updatePackages();
	void installPackages();
	void downloadPackages();

	int 				time;
	QTimer *			timer;
	QXmlStreamReader	reader;

	void parseZypperData();
	void processProgress(const QXmlStreamAttributes & attr);
	void addTrackPackage(const QXmlStreamAttributes & attr);
	void checkErrorMsg(const QXmlStreamAttributes & attr);

private slots:
	void processStdin(const QString &);
	void drawProgress();
};


#endif
