#include "controller.h"
#include "updateenvironment.h"
#include "common.h"
#include "updatelauncher.h"

#include "checkpartition.h"
#include "reboot.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <fcntl.h>

#define CONFIGURATION_FILE			"/.lge.upgrade.xml"

Controller::Controller(QObject * obj)
: QObject(obj){
	m_errorCode = -1;
	m_stopUpdate = false;
	m_updateList = NULL;
	m_checkPartition = NULL;
	m_updateLauncher = NULL;
	m_updateState = u_CheckEnvironment;
}

Controller::~Controller(){ }

void Controller::stopUpdate(){
	ULOG->log("Stop UPDATE");
	m_stopUpdate = true;

	if ( (int) m_updateState < (int) u_StartUpdates ){
		if ( m_updateState == u_PrepareFiles && m_checkPartition)
			m_checkPartition->stop();	

		emit updateStopped();
	}
}

void Controller::startUpdate(){
	if ( !m_stopUpdate ){
		m_updateState = u_StartUpdates;
		m_updateLauncher = new UpdateLauncher(*m_updateList, this);

		connect(m_updateLauncher, SIGNAL(updateProgressChanged(const QString &, const QString &, unsigned int, unsigned int)), SIGNAL(updateProgressChanged(const QString &, const QString &, unsigned int, unsigned int)));
		connect(m_updateLauncher, SIGNAL(updateProgressChangedDirect(const QString &, const QString &, unsigned int, unsigned int)), SIGNAL(updateProgressChangedDirect(const QString &, const QString &, unsigned int, unsigned int)));
		connect(m_updateLauncher, SIGNAL(updateAllFinished()), SLOT(cleanupUpdate()));
		m_updateLauncher->startUpdate();
	}
}

void Controller::checkAvailableUpdate(){
	if ( m_stopUpdate )
		return;

	m_updateState = u_CheckAvailableUpdate;
        ULOG->log("Check Available Update....");

	m_updateList = UpdateEnvironment::instance()->getAvailableUpdates();

	unsigned int partitionWeight = 0;
	unsigned int weightAll = 0;

	for (int i = 0; i < m_updateList->count(); ++i){
		if ( m_updateList->at(i)->type() == UpdateElement::et_PARTITION)
			partitionWeight += m_updateList->at(i)->weight() * 100;
		else
			weightAll += m_updateList->at(i)->weight() * 100;
	}

	partitionWeight = partitionWeight * 2;
	
	weightAll += partitionWeight;

	emit availableUpdates(m_updateList->count() + countOfPartitionUpdate(), weightAll);

	if ( m_updateList->count() == 0){
		emit cannotProceedUpdate(tr("STR_UPGRADE_NO_AVAILABLE"));
		return;
	}
	else{
		checkPartitionsChecksum();	
	}
}

void Controller::cleanupUpdate(){
	if ( m_updateLauncher )
		m_updateLauncher->cleanup();
	
	int fd = ::open("/dev/sda", O_RDONLY | O_NONBLOCK);
        if ( fd >= 0 ){
		::sync();
		::fsync(fd);
		::fdatasync(fd);
		::sync();
		if ( ioctl(fd, BLKFLSBUF, NULL))
			ULOG->log("BLKFLSBUF Failed");
	}
	else{
		ULOG->log("Open /dev/sda failed");
	}
	::sync();

        if ( fd >= 0 )
            ::close(fd);

	QString failList;

	for (int i = 0; i < m_updateList->count(); ++i){
		if (m_updateList->at(i)->status() == UpdateElement::es_FAIL){
			failList.append( m_updateList->at(i)->name() + " : " +
				m_updateList->at(i)->description() + "\n");
		}
	}
        //updateFinished(!isMicomUpdateExist(), failList);
        updateFinished(!isMicomAndFLCDUpdateExist(), failList);
}

void Controller::checkPartitionsChecksum(){
	m_updateState = u_PrepareFiles;

	if ( /*updateMode() == (int)Configuration::UserMode &&*/ countOfPartitionUpdate()){
		m_checkPartition = new CheckPartition;
		m_checkPartition->setUpdateList(*m_updateList);
		connect(m_checkPartition, SIGNAL(updateProgressChanged(const QString &, const QString &, unsigned int, unsigned int)), SIGNAL(updateProgressChanged(const QString &, const QString &, unsigned int, unsigned int)));
		connect(m_checkPartition, SIGNAL(finished(int)), this, SLOT(checkPartitionFinished(int)));
		//connect(m_checkPartition, SIGNAL(finished(int)), m_checkPartition, SLOT(deleteLater()));
		m_checkPartition->start();
	}
	else{
		startUpdate();
	}
}

void Controller::checkPartitionFinished(int exitCode){
	ULOG->log("Check Partition Finished : %d", exitCode);	

	if ( exitCode == 0){
		startUpdate();
	}
	else{
		emit cannotProceedUpdate(tr("STR_UPGRADE_FILE_CORRUPTED"));
	}
}

int Controller::countOfPartitionUpdate(){
	if ( m_updateList == NULL)
		return 0;

	int count = 0;
	for (int i = 0; i < m_updateList->count(); ++i){
		if ( m_updateList->at(i)->type() == UpdateElement::et_PARTITION)
			++count;
	}

	return count;
}

void Controller::usbDetached(){
	ULOG->log("USB Detached------------------");
	m_stopUpdate = true;

	if ( (int) m_updateState < (int) u_StartUpdates ){
		if ( m_updateState == u_PrepareFiles && m_checkPartition){
			m_checkPartition->stop();	
		}

		emit confirmUSBDetached(); 
	}
}

int Controller::updateMode(){
	return UpdateEnvironment::instance()->getUpdateMode();
}

Controller::ReturnCode Controller::checkUpdateEnvironment(const QString & mountPath){
	if(!QFile::exists(mountPath + CONFIGURATION_FILE)){
		if ( ::mount("/dev/mmcblk0p1", qPrintable(mountPath), "vfat", MS_RDONLY, NULL) == 0)	{
			ULOG->log("SD card remount to /navi for update");
			::sleep(1);
		}
	}

	UpdateEnvironment * env = UpdateEnvironment::instance(mountPath);
	
	if ( !env->testConnection()){
		m_errorCode = CODE_DBUS;
		return e_UpdateEnvFailed;
	}

	if ( !env->getSystemVariant()){
		m_errorCode = CODE_VARIANT;
		return e_UpdateEnvFailed;
	}

	if ( !env->getSystemVersion()){
		m_errorCode = CODE_SYSTEMVERSION;
		return e_UpdateEnvFailed;
	}

	if ( !env->getRootDevice()){
		m_errorCode = CODE_ROOTDEVICE;
		return e_UpdateEnvFailed;
	}

	if ( !env->readConfiguration()){
		m_errorCode = CODE_CONFIGURATION_CORRUPTED;
		return e_UpdateEnvFailed;	
	}

	if ( !env->checkUser() || !env->checkEssentialCheckSum())
		return e_FileCorrupted;

	if ( !env->checkRuntimeFiles())
		return e_NoEssentialFiles;

	if ( !env->checkVersionRequire())
		return e_VersionRequireUnmatch;

	// for get partition image size for updatekey
	env->getUpdatePartitions();
	
	if ( !env->checkUpdateKey())
		return e_UpdateKeyCorrupted;

	if ( g_variantCode == VA::KOREA || g_variantCode == VA::NA)
		env->restartMOSTmanager();

	return e_ValidateOk;
}

bool Controller::isMicomAndFLCDUpdateExist(){
        if ( m_updateList ){

          for (int j = 0; j< m_updateList->count(); ++j){

               if ( m_updateList->at(j)->name().toLower() == "front_lcd" &&
               ( /*m_updateList->at(i)->status() == UpdateElement::es_DONE ||*/
                   m_updateList->at(j)->status() == UpdateElement::es_FAIL))
                   {

                     return true;
                   }
          }

          for (int i = 0; i< m_updateList->count(); ++i){

                        if ( m_updateList->at(i)->name().toLower() == "micom" &&
                                ( m_updateList->at(i)->status() == UpdateElement::es_DONE ||
                                        m_updateList->at(i)->status() == UpdateElement::es_FAIL))
                        {
                                if ( m_updateList->at(i)->description() == "SAME VERSION")
                                        return false;

                                return true;
                        }


                }
        }
        return true;
}

bool Controller::isMicomUpdateExist(){
	if ( m_updateList ){
		//for (int i = 0; m_updateList->count(); ++i){
          for (int i = 0; i< m_updateList->count(); ++i){
			if ( m_updateList->at(i)->name().toLower() == "micom" &&
				( m_updateList->at(i)->status() == UpdateElement::es_DONE ||
					m_updateList->at(i)->status() == UpdateElement::es_FAIL))
			{
				if ( m_updateList->at(i)->description() == "SAME VERSION")
					return false;

				return true;
			}
		}
	}
	return false;
}

void Controller::reboot(){
   
        Reboot reboot(isMicomUpdateExist(),false); 
}

// SANGWOO_TEMP
void Controller::failCaserebot(){
   
        Reboot reboot(isMicomUpdateExist(),true); 
}
