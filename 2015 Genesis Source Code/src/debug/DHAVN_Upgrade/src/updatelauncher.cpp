#include "updatelauncher.h"
#include "rpmupdater.h"
#include "partition_updater.h"
#include "firmware_updater.h"
#include "most_updater.h"
#include "common.h"

UpdateLauncher::UpdateLauncher(const QList<UpdateElement *> & targets, QObject * obj)
: QObject(obj){
	for (int i = 0; i < AbstractUpdater::CountOfLauncher; ++i)
		m_status[i] = Ready;

	m_targets.append(targets);
	m_launchers[AbstractUpdater::RPMLauncher] = 
			new RPMUpdater(this);

	m_launchers[AbstractUpdater::PartitionLauncher] = 
			new PartitionUpdater(this);

	m_launchers[AbstractUpdater::FirmwareLauncher] =
			new FirmwareUpdater(this);
	
	m_launchers[AbstractUpdater::MOSTLauncher] = 
			new MOSTUpdater(this);

	for (int i = 0; i < AbstractUpdater::CountOfLauncher; ++i){
		connect(m_launchers[i], SIGNAL(finished(int)), SLOT(statusChanged(int)));
		connect(m_launchers[i], SIGNAL(updateProgressChanged(const QString &, const QString &, unsigned int, unsigned int)), SIGNAL(updateProgressChanged(const QString &, const QString &, unsigned int, unsigned int)));
		connect(m_launchers[i], SIGNAL(updateProgressChangedDirect(const QString &, const QString &, unsigned int, unsigned int)), SIGNAL(updateProgressChangedDirect(const QString &, const QString &, unsigned int, unsigned int)));

	}
}

void UpdateLauncher::cleanup(){
	ULOG->log("Start Cleanup");
	for (int i = 0; i < AbstractUpdater::CountOfLauncher; ++i)
		m_launchers[i]->finish();	
}

void UpdateLauncher::startUpdate(){
	m_launchers[AbstractUpdater::RPMLauncher]->setUpdateList(m_targets);
	m_launchers[AbstractUpdater::PartitionLauncher]->setUpdateList(m_targets);
	m_launchers[AbstractUpdater::FirmwareLauncher]->setUpdateList(m_targets);
	m_launchers[AbstractUpdater::MOSTLauncher]->setUpdateList(m_targets);
	
	m_status[AbstractUpdater::PartitionLauncher] = Updating;
	m_status[AbstractUpdater::RPMLauncher] = Updating;
	m_status[AbstractUpdater::FirmwareLauncher] = Updating;
	m_status[AbstractUpdater::MOSTLauncher] = Updating;

	m_launchers[AbstractUpdater::RPMLauncher]->start();
	m_launchers[AbstractUpdater::PartitionLauncher]->start();
	m_launchers[AbstractUpdater::FirmwareLauncher]->start();
	m_launchers[AbstractUpdater::MOSTLauncher]->start();
}

UpdateLauncher::UpdaterStatus UpdateLauncher::getUpdaterStatus(AbstractUpdater::LauncherMode mode){
	return m_status[mode];
}

void UpdateLauncher::statusChanged(int status){
	Q_UNUSED(status);

	if ( qobject_cast<RPMUpdater *>(sender())) m_status[AbstractUpdater::RPMLauncher] = UpdateLauncher::Finished;
	else if ( qobject_cast<PartitionUpdater *>(sender())) m_status[AbstractUpdater::PartitionLauncher] = UpdateLauncher::Finished;
	else if ( qobject_cast<FirmwareUpdater *>(sender())) m_status[AbstractUpdater::FirmwareLauncher] = UpdateLauncher::Finished;
	else if ( qobject_cast<MOSTUpdater *>(sender())) m_status[AbstractUpdater::MOSTLauncher] = UpdateLauncher::Finished;

	for (int i = 0; i < AbstractUpdater::CountOfLauncher; ++i){
		if ( m_status[i] != UpdateLauncher::Finished)
			ULOG->log("Update : %d not finished", i);
	}

	for (int i = 0; i < AbstractUpdater::CountOfLauncher; ++i){
		if ( m_status[i] != UpdateLauncher::Finished)
			return;
	}
	ULOG->log("All UPDATE Finished:UpdateLauncher");
	emit updateAllFinished();
}
