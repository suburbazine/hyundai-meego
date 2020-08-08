#include "partition_updater.h"

#include <sys/mount.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <unistd.h>
#include <fcntl.h>

#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QEventLoop>
#include <QTextStream>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <getopt.h>
#include <linux/fs.h>

#include "stdinprocess.h"
#include "partition.h"
#include "updateenvironment.h"

#define TRIM_FOR_UPGRADE	0
#define TRIM_FOR_FACTORY	1

PartitionUpdater::PartitionUpdater(QObject * obj)
: AbstractUpdater(AbstractUpdater::PartitionLauncher, obj){
	m_updateEndCount = 0;
	m_countRunningProcess = 0;
	m_currentRunningIndex = 0;
}

PartitionUpdater::~PartitionUpdater(){}

void PartitionUpdater::stop(){}

void PartitionUpdater::startPartClone(){
	ULOG->log("Start PartClone : %d", m_currentRunningIndex);	
	int umount_try_count = 3;

	StdinProcess * sp = StdinProcess::CreateNewInstance();
	int index = findPartitionInfo(m_targets.at(m_currentRunningIndex)->name());
	if ( index < 0 ){
		emit finished(1);
		return;
	}

	sp->setObjectName(m_targets.at(m_currentRunningIndex)->name());
	connect(sp, SIGNAL(output(const QString &)), SLOT(processStdin(const QString &)));
	if ( index != (int)RootPartition ){
		ULOG->log("umount : %s", g_partition_info[index].mount_point);
		while( umount_try_count-- ){
			if ( ::umount( g_partition_info[index].mount_point ) != 0){
				ULOG_EXCEPTION;
				ULOG->log("unmount %s failed(%s)", g_partition_info[index].mount_point, strerror(errno));
				::sleep(2);	// TODO : replace sleep with some others
			}
			else{
				ULOG->log("umount %s success", g_partition_info[index].mount_point);
				break;
			}
		}
	}

	emit updateProgressChanged(m_targets.at(m_currentRunningIndex)->name(), "Partition Clone Started", 0, m_targets.at(m_currentRunningIndex)->weight());	

	sp->execute(QString(PARTCLONE_EXECUTABLE_NAME) + " " +
				PARTCLONE_RESTORE_OPTION +
				PARTCLONE_SOURCE_OPTION +
				SPACE_STR(m_targets.at(m_currentRunningIndex)->path()) + 
				PARTCLONE_OUTPUT_OPTION +
				g_partition_info[index].dev_name);

	connect(sp, SIGNAL(processFinished(const QString &)), SLOT(checkEndProcess(const QString &)));
	connect(sp, SIGNAL(doFinished()), SLOT(countProcess()));
	connect(sp, SIGNAL(doFinished()), sp, SLOT(deleteLater()));
}

void PartitionUpdater::processFailPartition(const QString & name){
	unsigned int weight = 0;

	for (int i = 0; i < m_targets.count(); ++i){
		if ( m_targets.at(i)->name() == name){
			weight = m_targets.at(i)->weight();

			if (m_targets.at(i)->status() == UpdateElement::es_READY){
				ULOG_EXCEPTION;
				ULOG->log("ERROR : %s", qPrintable(m_targets.at(i)->name()));
				m_targets.at(i)->setStatus(UpdateElement::es_FAIL);
				m_targets.at(i)->setDescription("abnormal terminated");
			}
			else{
                            if ( name == "vr" || name == "gracenote"){
                                 QDir dir;
                                if ( dir.exists("/trim")){
                                    if (name == "vr" ){
                                        if ( ::mount(g_partition_info[VrPartition].dev_name,"/trim","ext4", MS_SYNCHRONOUS, NULL) != 0){
                                             ULOG->log("vr trim mount failed");
                                        }else{
                                             ssd_trim_cmd(TRIM_FOR_UPGRADE);
                                             ULOG->log("vr trim mount success");
                                             if(::umount("/trim") != 0){
                                                    ULOG->log("vr trim umount failed");
                                              }
                                             ::sleep(2);
                                        }
                                    }
                                    if (name == "gracenote" ){
                                        if ( ::mount(g_partition_info[GracenotePartition].dev_name,"/trim","ext4", MS_SYNCHRONOUS, NULL) != 0){
                                             ULOG->log("GracenotePartition trim mount failed");
                                        }else{
                                             ssd_trim_cmd(TRIM_FOR_UPGRADE);
                                             ULOG->log("GracenotePartition trim mount success");
                                             if(::umount("/trim") != 0){
                                                    ULOG->log("GracenotePartition trim umount failed");
                                              }
                                             ::sleep(2);
                                        }
                                    }


                                }else{
                                       ULOG->log("trim folder not exists");
                                }
                            }



				return;
			}
		}
	}

	emit updateProgressChanged(name, "Partition Clone Failed", 100, weight);
	m_updateEndCount--;
	m_currentRunningIndex++;

	if ( m_updateEndCount > 0)
		startPartClone();
}

void PartitionUpdater::checkEndProcess(const QString & name){
	ULOG->log("Check End Process : %s", qPrintable(name));
	processFailPartition(name);
}

void PartitionUpdater::setUpdateList(const QList<UpdateElement *> & list){
	m_updateAllTargets.append(list);
	for ( int i = 0; i < list.count(); ++i)
		if ( list.at(i)->type() == UpdateElement::et_PARTITION)
			m_targets.append(list[i]);
}

void PartitionUpdater::start(){
	if ( m_targets.count() == 0){
		ULOG->log("PartitionUpdate list zero");
		emit finished(0);
		return;
	}

	m_updateEndCount = m_countRunningProcess = m_targets.count();
	startPartClone();	
}

bool PartitionUpdater::finish(){
	if ( m_targets.count() == 0) return true;

	ULOG->log("Change Maximum mount count and interval sda7, 8, 10");
	::system("tune2fs -c 0 -i 0 /dev/sda7");
	::system("tune2fs -c 0 -i 0 /dev/sda8");
	::system("tune2fs -c 0 -i 0 /dev/sda10");
	::system("sync");

	if ( needSwapOperation()){
		cloneBootPartition();
		if ( ::mount(CONST_BOOT_PARTITION_DEV, "/boot", "ext3", MS_SYNCHRONOUS, NULL) != 0){
			ULOG_EXCEPTION; ULOG->log("boot mount failed");
			return false;
		}
		if ( !mountUpdatePartition()) return false;
	
		QFile::copy(UpdateEnvironment::instance()->mountPath() + CONFIGURATION_PATH, (UPGRADE_DIR UPGRADE_MOUNT_DIR "/etc" CONFIGURATION_PATH));

                QDir dir;
                /*
                if ( !dir.exists("/trim"))
                                if ( !dir.mkdir("/trim"))
                                        ULOG->log("SANGWOO trim dir Cannot create mount directory(or already exist");


                if ( ::mount(g_partition_info[RootPartition].dev_name,"/trim","ext4", MS_SYNCHRONOUS, NULL) != 0){
                     ULOG->log("SANGWOO trim mount failed");
                }else{
                     ssd_trim_cmd(TRIM_FOR_UPGRADE);
                     ULOG->log("SANGWOO trim mount success");
                     if(::umount("/trim") != 0){
                            ULOG->log("SANGWOO trim umount failed");
                      }
                     ::sleep(2);
                }
                */

                if ( dir.exists("/trim")){
                    if ( ::mount(g_partition_info[RootPartition].dev_name,"/trim","ext4", MS_SYNCHRONOUS, NULL) != 0){
                         ULOG->log("root trim mount failed");
                    }else{
                         ssd_trim_cmd(TRIM_FOR_UPGRADE);
                         ssd_trim_cmd(TRIM_FOR_FACTORY);
                         ULOG->log("root trim mount success");
                         if(::umount("/trim") != 0){
                                ULOG->log("root trim umount failed");
                          }
                         ::sleep(2);
                    }



				}else{
					ULOG->log("trim not exists");
				}

				QStringList targetDevName = QString(g_partition_info[RootPartition].dev_name).split("/");
				QString changePartitionCommand = QString("change_extlinux sda. ") + targetDevName.at(targetDevName.count() - 1);
				ULOG->log("Change Partition : %s", qPrintable(changePartitionCommand));
				::system(qPrintable(changePartitionCommand));

	}

	recordNewVersion();
	copyMicomEmergency();
	recordFailFlag();	

	return true;
}
////////////////////////////////////////////////////////////////////
#ifndef FITRIM
struct fstrim_range {
        uint64_t start;
        uint64_t len;
        uint64_t minlen;
};
#define FITRIM		_IOWR('X', 121, struct fstrim_range)
#endif
int PartitionUpdater::ssd_trim_cmd(int cmd_option){
	int fd, verbose = 0;
	int idx, idx_start, idx_end = 0;
	struct fstrim_range range;
	struct stat sb;
	char *target_dir[] = {"/trim",
					"/rw_data", "/log_data", "/usr_data", "/vr",
					"/gracenote", "/common_data", "/", };

	if( cmd_option == TRIM_FOR_UPGRADE ) {
		idx_start = 0;
		idx_end = 0;
	} else {
		idx_start = 1;
		idx_end = 7;	/* num of mounted fs */
	}

	for( idx = idx_start; idx <= idx_end ; idx++ ) {
		//printf("WORK DIR: %s\n", target_dir[idx]);
		memset(&range, 0, sizeof(range));
		range.start = 0;
		range.len = ULLONG_MAX;

		fd = open(target_dir[idx], O_RDONLY);
		if (fd < 0){
			ULOG->log( "%s: open failed", qPrintable(target_dir[idx]));
			return 0;
		}


		if (ioctl(fd, FITRIM, &range)){
			ULOG->log( "%s: FITRIM ioctl failed", qPrintable(target_dir[idx]));

			close(fd);
			return 0;
		}


		if (verbose)
			/* TRANSLATORS: The standard value here is a very large number. */
			ULOG->log("%s: trimmed\n", qPrintable(target_dir[idx]));

		close(fd);
	}

	system("sync");

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
bool PartitionUpdater::mountUpdatePartition(){
	QDir dir;

	if ( !dir.exists(UPGRADE_DIR + QString(UPGRADE_MOUNT_DIR)))
		if ( !dir.mkdir(UPGRADE_DIR + QString(UPGRADE_MOUNT_DIR)))
			ULOG->log("Cannot create mount directory(or already exist");

	if ( ::mount(g_partition_info[RootPartition].dev_name, UPGRADE_DIR UPGRADE_MOUNT_DIR, "ext4", MS_SYNCHRONOUS, NULL) != 0){
		ULOG_EXCEPTION; ULOG->log("Failed to mount upgrade rootfs");
		return false;
	}
	
	if ( !dir.exists(UPGRADE_DIR + QString(UPGRADE_SWITCH_MOUNT_DIR))){
		if ( !dir.mkdir(UPGRADE_DIR + QString(UPGRADE_SWITCH_MOUNT_DIR))){
			ULOG->log("Cannot create mount upgrade directory");
			return false;
		}
	}

	return true;
}

void PartitionUpdater::cloneBootPartition(){
	ULOG->log("Partition clone Boot Partition");
	QEventLoop loop;
	StdinProcess * sp = StdinProcess::CreateNewInstance();
	
	connect(sp, SIGNAL(output(const QString &)), SLOT(logStdin(const QString &)));
	sp->execute(QString(PARTCLONE_EXECUTABLE_NAME) + " " +
			PARTCLONE_DEV_TO_DEV_OPTION +
			PARTCLONE_SOURCE_OPTION +
			SPACE_STR(g_partition_info[BootPartition].dev_name) + 	
			PARTCLONE_OUTPUT_OPTION +
			CONST_BOOT_PARTITION_DEV);

	connect(sp, SIGNAL(doFinished()), sp, SLOT(deleteLater()));
	connect(sp, SIGNAL(doFinished()), &loop, SLOT(quit()));

	loop.exec();
	ULOG->log("Partition clone Boot Partition End");
}


void PartitionUpdater::countProcess(){
	m_countRunningProcess--;
	ULOG->log("countProcess : %d", m_countRunningProcess);
	if ( m_countRunningProcess == 0)
		emit finished(0);
}

void PartitionUpdater::parseProgress(QObject * obj, const QString & str){
	QStringList token = str.split(",");
	for (int i = 0; i < token.count(); ++i){
		QString trimmed = token.at(i).trimmed();
		QStringList column = trimmed.split(":");
		if ( column.count() == 2){
			QString section = column.at(0).trimmed();
			QString value = column.at(1).trimmed();
			ULOG->log("section : %s, value : %s", qPrintable(section), qPrintable(value));	
			if ( section == "Completed"){
				value = value.remove("%");
				int percent = (int)value.toDouble();
				emit updateProgressChanged(obj->objectName(), "Copying...", percent, getWeightByName(obj->objectName(), m_targets));
			}
		}
	}
}

void PartitionUpdater::logStdin(const QString & output){
	ULOG->log("%s", qPrintable(output));
}

UpdateElement * PartitionUpdater::getUpdateElementByName(const QString & name){
	for (int i = 0; i < m_targets.count(); ++i)
		if ( m_targets.at(i)->name().toLower() == name.toLower()) return m_targets[i];

	return NULL;
}

bool PartitionUpdater::setFlag(const QString & name, UpdateElement::ElementStatus status){
	for (int i = 0; i < m_targets.count(); ++i){
		if (m_targets.at(i)->name() == name){
			if ( m_targets.at(i)->status() != status){
				m_targets.at(i)->setStatus(status);
				return true;
			}
		}
	}
	return false;
}

bool PartitionUpdater::needSwapOperation(){
	UpdateElement * rootfs = getUpdateElementByName("rootfs");
	UpdateElement * boot = getUpdateElementByName("boot");

	if ( rootfs && boot){
		if ( rootfs->status() == UpdateElement::es_DONE && boot->status() == UpdateElement::es_DONE) return true;
	}

	return false;
}

void PartitionUpdater::processStdin(const QString & output){
	QObject * obj = sender();
	if ( obj ){
		QString objName = obj->objectName();
		if ( output.contains("Cloned successfully")){
			if (!setFlag(objName, UpdateElement::es_DONE)) return;

			emit updateProgressChanged(objName, "Cloning Finished", 100, 100 * getWeightByName(objName, m_targets));
			ULOG->log("%s : Cloned End", qPrintable(objName));
			m_updateEndCount--;
			m_currentRunningIndex++;

			if ( m_updateEndCount > 0)
				startPartClone();
		}
		else if ( output.contains("Completed")){
			parseProgress(obj, output);
		}
		else if ( output.contains("error") || output.contains("fail")){
			ULOG_EXCEPTION;
			ULOG->log("ERROR : %s", qPrintable(output));

			processFailPartition(objName.toLower());
		}
	}
}

int PartitionUpdater::findPartitionInfo(const QString & name){
	for (int i = 0; i < PARTITION_COUNT; ++i){
		if ( QString(g_partition_info[i].partition_name) == name)
			return i;
	}
	ULOG_EXCEPTION;
	ULOG->log("cannot findPartitionInfo : %s", qPrintable(name));	
	return -1;
}


void PartitionUpdater::recordNewVersion(){
	QString fileName = (needSwapOperation() ? "/common_data/upgrade/mount/upgrade/report" : "/upgrade/report");
	ULOG->log("Record New version : %s", qPrintable(fileName));
	QFile f(fileName);
	writeReport(f);
}

void PartitionUpdater::recordFailFlag(){
	QString fileName = (needSwapOperation() ? "/common_data/upgrade/mount/etc/upgrade_fail" : "/etc/upgrade_fail");
	ULOG->log("Record Fail Flag : %s", qPrintable(fileName));

	bool found = false;
	for (int i = 0; i < m_updateAllTargets.count(); ++i){
		if ( m_updateAllTargets.at(i)->status() == UpdateElement::es_FAIL){
			found = true;
			ULOG->log("Found Fail Element");
			break;
		}
	}

	if ( found ){
		QFile f(fileName);
		f.open(QIODevice::WriteOnly | QIODevice::Truncate);
		ULOG->log("Write Fail Log");
		f.close();
	}
	else{
		if ( !needSwapOperation()){
			QFile::remove("/etc/upgrade_fail");
		}
	}
}


void PartitionUpdater::writeReport(QFile & file){
	QString version;

	if ( file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
		QTextStream out(&file);
		
		for ( int i = 0; i < m_targets.count(); ++i){
			QString partitionName = m_targets.at(i)->name();

			if ( partitionName.toLower() != "rootfs" &&
					partitionName.toLower() != "boot"){
				out << m_targets.at(i)->name() << "\t";

				if ( m_targets.at(i)->status() == UpdateElement::es_DONE)
					out << m_targets.at(i)->version() << "\n";	
				else
					out << UpdateEnvironment::instance()->getSystemVersionString() << "\n";
			}
		}

		file.close();
	}	
}

void PartitionUpdater::copyMicomEmergency(){
	if ( !g_isReadonly & needSwapOperation()){
		copyDir("/root/micom", "/upgrade/mount/root/micom");
	}
}

