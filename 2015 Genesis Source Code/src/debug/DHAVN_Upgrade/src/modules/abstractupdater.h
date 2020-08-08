#ifndef	ABSTRACT_UPDATER_H_
#define	ABSTRACT_UPDATER_H_

#include <QObject>
#include "updateelement.h"

class AbstractUpdater : public QObject{
Q_OBJECT
public:
	enum LauncherMode {
		RPMLauncher, PartitionLauncher,
		MOSTLauncher, FirmwareLauncher, CountOfLauncher
	};

	AbstractUpdater(LauncherMode aMode, QObject * obj);

	virtual void setUpdateList(const QList<UpdateElement *> & list) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual bool finish() = 0;
	LauncherMode mode() { return m_mode; }
	int getWeightByName(const QString & name, const QList<UpdateElement *> & list){
		for (int i = 0; i < list.count(); ++i){
			if ( list.at(i)->name() == name){
				int w = list.at(i)->weight();

				if ( w == 0) return 1;
				else return w;
			}
		}
		return 1;
	}

protected:
	unsigned int getLauncherStatus(AbstractUpdater::LauncherMode mode);
	bool removeDir(const QString & name);
	void copyDir(const QString & from, const QString & to);

private:
	LauncherMode m_mode;

signals:
	void updateProgressChanged(const QString & name,
								const QString & description,
								unsigned int progress,
								unsigned int weight_progress);

	void updateProgressChangedDirect(const QString & name,
								const QString & description,
								unsigned int progress,
								unsigned int weight_progress);
	void finished(int);
};

#endif
