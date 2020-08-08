#include "partitionversion.h"

#include <QDir>
#include <QFileInfoList>

#include "discoverypartition.h"
#include "systeminfo.h"
#include "common.h"
#include "partition.h"

PartitionVersion::PartitionVersion(const QString & aMountPath, QObject * obj)
: AbstractVersion(aMountPath, obj){
	m_mainPartitionUpdate = false;
	m_discoveryPartition = new DiscoveryPartition(this);
}

void PartitionVersion::filterUpdateList(){
	UpdateElement * rootfs = findUpdate("rootfs");
	UpdateElement * boot = findUpdate("boot");

	if ( rootfs == NULL && boot == NULL)
		return;

	if ( rootfs == NULL || boot == NULL){
		removeTarget(rootfs);
		removeTarget(boot);
	}
	else{
		m_mainPartitionUpdate = true;
	}
}

void PartitionVersion::getUpdatePartitions(const QString & rootfs, const QString & systemVersion, const QString & updateVersion){
	m_discoveryPartition->setRootPartitionInfo(rootfs); 
	m_discoveryPartition->setCurrentVersion(systemVersion);
	
	collectUSBPartitionImages(updateVersion);
}

void PartitionVersion::collectUSBPartitionImages(const QString & updateVersion){
	QDir d(mountPath() + PARTITION_USB_DIR_SUFFIX);
	QFileInfoList list = d.entryInfoList();

	for (int i = 0; i < list.count(); ++i){
		if ( list.at(i).isFile()){
			ULOG->log("USB Partitition File : %s", qPrintable(list.at(i).absoluteFilePath()));

			m_imageSizeStrings += QString::number( (uint) list.at(i).size());	

			for (int j = 0; j < PARTITION_COUNT; ++j){
				if ( list.at(i).baseName() == g_partition_info[j].partition_name && list.at(i).completeSuffix() == "img"){
					if ( isIgnorePartition(list.at(i).baseName()))	continue;
					
					createUpdateInfo(list.at(i), j, updateVersion);	
				}
			}
		}
		else if ( list.at(i).isDir()){
			if ( (g_isReadonly && list.at(i).baseName() == "NP") || (!g_isReadonly && list.at(i).baseName() == "OP")){
				QFileInfo file(list.at(i).absoluteFilePath() + "/rootfs.img");
				if ( file.exists()) {
					ULOG->log("USB Partition RootFS : %s", qPrintable(file.absoluteFilePath()));
					createUpdateInfo(file, 1, updateVersion);
				}
			}
		}
	}
}

void PartitionVersion::createUpdateInfo(const QFileInfo & info, int index, const QString & updateVersion){
	SystemInfo sysVersion, usbVersion;
	sysVersion.setVersion(g_partition_info[index].version);
	usbVersion.setVersion(updateVersion);

	if ( sysVersion != usbVersion){
		appendTarget(
			UpdateElement::CreatePartitionElement(
				g_partition_info[index].partition_name,
				g_partition_info[index].version,
				updateVersion, info.absoluteFilePath(),
				g_partition_info[index].weight));
	}	
}

bool PartitionVersion::isIgnorePartition(const QString & str){
	if ( str == "log_data" || str == "rootfs")
		return true;
	else if ( str.startsWith("gracenote_")){
		if ( QString("gracenote_") + g_variant_str[g_variantCode] != str)
			return true;
	}

	return false;
}
