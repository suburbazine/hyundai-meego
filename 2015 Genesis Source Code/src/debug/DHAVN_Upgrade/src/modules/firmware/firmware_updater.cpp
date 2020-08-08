#include "firmware_updater.h"
#include "updatelauncher.h"
#include "msg_client.h"
#include "messageevent.h"
#include "common.h"

#define FIRMWARE_LAUNCHER_BEAT	5000

FirmwareUpdater::FirmwareUpdater(QObject * obj)
: AbstractUpdater(AbstractUpdater::FirmwareLauncher, obj){
	m_launcherTimer = new QTimer(this);
	m_launcherTimer->setInterval(FIRMWARE_LAUNCHER_BEAT);
	m_launcherTimer->setSingleShot(true);
	connect(m_launcherTimer, SIGNAL(timeout()), SLOT(startLauncher()));
        bios_check = false;
	m_messageEvent = new MessageEvent(this);

	connect(m_messageEvent, SIGNAL(messageProgressRaw(int, int, int, const QString &)), SLOT(checkFirmwareUpdater(int, int, int, const QString &)));
}

FirmwareUpdater::~FirmwareUpdater(){ }

void FirmwareUpdater::checkFirmwareUpdater(int id, int status, int progress, const QString & msg){
	UpdateElement * element = getElementFromRunningList(id);
	if ( element == NULL){	// It is already handled by timeout
		element = getElementFromAll(id);
		Q_ASSERT(element);
		element->setDescription(msg);

                if(progress == 100 && (status == MODULE_STATUS_DONE ||status == MODULE_STATUS_OLD_VERSION  ))
                {
                    ULOG->log("Firmware timeout DONE : %s, %d, %s", qPrintable(element->name()), id, qPrintable(msg));
                    element->setStatus(UpdateElement::es_DONE);
                }else if ( status != MODULE_STATUS_FAIL){
                    ULOG->log("Firmware timeout restart : %s, %d, %s", qPrintable(element->name()), id, qPrintable(msg));
                    m_runningTargets.append(element); 
                }

		return;
	}

	ULOG->log("Firmware : %s, %d, %s", qPrintable(element->name()), id, qPrintable(msg));
	if ( status == MODULE_STATUS_DONE || status == MODULE_STATUS_OLD_VERSION || status == MODULE_STATUS_FAIL){
		m_runningTargets.removeAll(element);

		if ( status == MODULE_STATUS_OLD_VERSION)
			element->setDescription("SAME VERSION");
		else
			element->setDescription(msg);

		switch ( status ){
		case MODULE_STATUS_DONE:
		case MODULE_STATUS_OLD_VERSION:
			element->setStatus(UpdateElement::es_DONE);
			break;
		default:
			element->setStatus(UpdateElement::es_FAIL);
			break;
		}

		emit updateProgressChanged(element->name().toLower(), msg, 100, element->weight());
	}
	else{
		if ( element) element->resetAccessTime();
		emit updateProgressChanged(element->name().toLower(), msg, progress, element->weight());
	}
}

void FirmwareUpdater::stop(){
	if ( m_launcherTimer->isActive())
		m_launcherTimer->stop();
}

void FirmwareUpdater::setUpdateList(const QList<UpdateElement *> & list){
        //bios_check = false;
	for (int i = 0; i < list.count(); ++i){
                if(list.at(i)->name().toLower() =="bios")
                        bios_check = true;
		if ( list.at(i)->type() == UpdateElement::et_FIRMWARE){
			m_targets.append(list[i]);
			m_waitTargets.append(list[i]);
		}
	}
}

bool FirmwareUpdater::finish(){
	removeDir("/common_data/upgrade/HU");
	removeDir("/common_data/upgrade/sbin");
	return false;
}

void FirmwareUpdater::start(){
	if ( m_targets.count() == 0){
		ULOG->log("FirmwareUpdate list zero");
		emit finished(0);
		return;
	}
	m_launcherTimer->start();
}

void FirmwareUpdater::startLauncher(){
	ULOG->log("FirmwareUpdater: startLauncher wait(%d), running(%d)", m_waitTargets.count(), m_runningTargets.count());
	if ( isFinished()) return;

        for ( int j = 0 ; j < m_runningTargets.count(); ++j){
            ULOG->log("FirmwareUpdater: startLauncher : %s", qPrintable(m_runningTargets[j]->name()));
        }

	for (int i = 0; i < m_waitTargets.count(); ++i){
		bool runSuccess = false;		
		Firmware::DependencyCode dep = Firmware::GetDependencyByName(m_waitTargets[i]->name());

		switch(dep){
		case Firmware::DEP_NONE:
			runSuccess = true;
		break;

		case Firmware::DEP_MICOM:
			if ( !checkRunningFirmware(Firmware::DEP_MICOM)) runSuccess = true;
		break;

		case Firmware::DEP_MOST_END:
			if ( getLauncherStatus( AbstractUpdater::MOSTLauncher) == (unsigned int) UpdateLauncher::Finished){
				if ( m_waitTargets[i]->name().toLower() == "micom"){
					if ( !checkRunningFirmware(Firmware::DEP_MICOM) && !checkReadyFirmware(Firmware::DEP_MICOM)){
						runSuccess = true;
					}
				}
				else {	// INIC
					runSuccess = true;
				}
			}
		break;

                case Firmware::DEP_SSD: 
                {
                    UpdateLauncher::UpdaterStatus mostStatus = (UpdateLauncher::UpdaterStatus) getLauncherStatus( AbstractUpdater::MOSTLauncher);
                    UpdateLauncher::UpdaterStatus partitionStatus = (UpdateLauncher::UpdaterStatus) getLauncherStatus( AbstractUpdater::PartitionLauncher);
                    UpdateLauncher::UpdaterStatus rpmStatus = (UpdateLauncher::UpdaterStatus) getLauncherStatus( AbstractUpdater::RPMLauncher);
                  int waitTargetcont =0;
                  if (bios_check)
                      waitTargetcont=2;
                  else
                      waitTargetcont=1;

                    if ( mostStatus == UpdateLauncher::Finished &&
                                    partitionStatus == UpdateLauncher::Finished &&
                                    rpmStatus == UpdateLauncher::Finished){
                            if ( !checkRunningFirmware(Firmware::DEP_MOST_END) && !checkReadyFirmware(Firmware::DEP_MOST_END)){
                                   if ( m_waitTargets.count() == waitTargetcont && m_runningTargets.count() == 0)
                                       runSuccess = true;
                            }
                        }
                        ULOG->log("FirmwareUpdater::startLauncher DEP_SSD runSuccess : %d",runSuccess);
                }
                break;
		case Firmware::DEP_ALL_FINISHED:
		{
			UpdateLauncher::UpdaterStatus mostStatus = (UpdateLauncher::UpdaterStatus) getLauncherStatus( AbstractUpdater::MOSTLauncher);
			UpdateLauncher::UpdaterStatus partitionStatus = (UpdateLauncher::UpdaterStatus) getLauncherStatus( AbstractUpdater::PartitionLauncher);
			UpdateLauncher::UpdaterStatus rpmStatus = (UpdateLauncher::UpdaterStatus) getLauncherStatus( AbstractUpdater::RPMLauncher);
                        if (bios_check==false)
                            break;
			if ( mostStatus == UpdateLauncher::Finished &&
					partitionStatus == UpdateLauncher::Finished &&
					rpmStatus == UpdateLauncher::Finished) {
				if ( m_waitTargets.count() == 1 && m_runningTargets.count() == 0)
					runSuccess = true;
			}	
		}
		break;

		default: break;
		}

		if ( runSuccess){
			popWaitQueueAndRun(i);
			break;
		}
	}
	checkTimeoutRunningList();

	if ( m_waitTargets.count() != 0 || m_runningTargets.count() != 0) 
		m_launcherTimer->start();
	else
		isFinished();
}

void FirmwareUpdater::popWaitQueueAndRun(int i){
	UpdateElement * element = m_waitTargets.takeAt(i);
	m_runningTargets.append(element);

	QString launcher = Firmware::GetLauncherByName(element->name());

	if ( launcher.isEmpty()){
		ULOG_EXCEPTION;	ULOG->log("Cannot find launcher : %s", qPrintable(element->name()));
		return;
	}

	startLauncher(element, launcher);
}

void FirmwareUpdater::startLauncher(UpdateElement * element, const QString & launcher){
	element->resetAccessTime();
	QString command = launcher;
	command.append(" ");
	command.append(element->path());
	command.append(" ");

	if ( isCanDevice(element->name()))
		command.append(element->name().toUpper());
	else
		command.append(element->version());

	ULOG->log("FirmwareUpdaterr:startLauncher : %s", qPrintable(command));
	if ( ::system(qPrintable(command)) < 0){
		ULOG_EXCEPTION;
		ULOG->log("launch firmware updater failed : %s", qPrintable(command));
		m_runningTargets.removeAll(element);
	}
}

bool FirmwareUpdater::isCanDevice(const QString & aName){
	QString name = aName.trimmed().toLower();
	if ( name == "ccp" || name == "rrc" || name == "front_lcd" ||
		name == "rear_right_lcd" || name == "rear_left_lcd"){
		return true;
	}

	return false;
}

bool FirmwareUpdater::checkRunningFirmware(Firmware::DependencyCode dep){
	for (int i = 0; i < m_runningTargets.count(); ++i){
		Firmware::DependencyCode aDep = Firmware::GetDependencyByName(m_runningTargets[i]->name());
		if ( dep == aDep ) return true;
	}
	return false;
}

bool FirmwareUpdater::checkReadyFirmware(Firmware::DependencyCode dep){
	for ( int i = 0; i < m_waitTargets.count(); ++i){
		Firmware::DependencyCode aDep = Firmware::GetDependencyByName(m_waitTargets[i]->name());
		if ( dep == aDep) return true;
	}
	return false;
}

bool FirmwareUpdater::isFinished(){
	if ( m_runningTargets.count() == 0 && m_waitTargets.count() == 0){
		emit finished(0);
		return true;
	}
	return false;
}

void FirmwareUpdater::checkTimeoutRunningList(){
	for (int i = 0; i < m_runningTargets.count(); ++i){
		if ( m_runningTargets.at(i)->isFirmwareTimeout()){
                    ULOG->log("checkTimeoutRunningList : %s : ", qPrintable(m_runningTargets.at(i)->name()));
			m_runningTargets.at(i)->setDescription("wait timeout");
			m_runningTargets.at(i)->setStatus(UpdateElement::es_FAIL);
			emit updateProgressChanged(m_runningTargets.at(i)->name().toLower(), "wait timeout", 100, m_runningTargets.at(i)->weight());
			m_runningTargets.takeAt(i);
			--i;
			break;
		}
	}
}

UpdateElement * FirmwareUpdater::getElementFromRunningList(int id){
	QString packageName = MessageEvent::GetModuleIdToPackageName(id);
	for (int i = 0; i < m_runningTargets.count(); ++i){
		if ( m_runningTargets.at(i)->name().toLower() ==
			packageName.toLower()){
			return m_runningTargets[i];
		}	
	}
	return NULL;
}

UpdateElement * FirmwareUpdater::getElementFromAll(int id){
	QString packageName = MessageEvent::GetModuleIdToPackageName(id);
	for (int i = 0; i < m_targets.count(); ++i){
		if ( m_targets.at(i)->name().toLower() ==
			packageName.toLower()){
			return m_targets[i];
		}	
	}
	return NULL;
}

