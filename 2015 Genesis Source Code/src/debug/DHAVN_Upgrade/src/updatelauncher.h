#ifndef	UPDATE_LAUNCHER_H_
#define	UPDATE_LAUNCHER_H_

#include <QObject>
#include "abstractupdater.h"
#include "updateelement.h"

class UpdateLauncher : public QObject{
Q_OBJECT
public:
	enum UpdaterStatus { Ready, Updating, Finished };

	UpdateLauncher(const QList<UpdateElement *> & targets, QObject * obj = 0);

	void startUpdate();
	void cleanup();
	UpdaterStatus getUpdaterStatus(AbstractUpdater::LauncherMode mode);

private:
	AbstractUpdater * m_launchers[AbstractUpdater::CountOfLauncher];
	QList<UpdateElement *> m_targets;

	UpdaterStatus m_status[AbstractUpdater::CountOfLauncher];

private slots:
	void statusChanged(int);

signals:
	void updateProgressChanged(const QString & name,
								const QString & description,
								unsigned int progress,
								unsigned int weight_progress);
	void updateProgressChangedDirect(const QString & name,
								const QString & description,
								unsigned int progress,
								unsigned int weight_progress);
	void updateAllFinished();
};

#endif
