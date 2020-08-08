#include "discoverypartition.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

#include "partition.h"


DiscoveryPartition::DiscoveryPartition(QObject * obj) 
: QObject(obj){
	m_spareBootPartitionExist = checkSpareBoot();
	setBootPartitionInfo(g_isReadonly, m_spareBootPartitionExist);
	setPartitionInfo(g_isReadonly);

	if ( gf_check_variant() ){
		changePartitionNameForVariant(g_variantCode);
	}
	else{
		ULOG_EXCEPTION;
		ULOG->log("Failed to get Variant");
	}
}

void DiscoveryPartition::setCurrentVersion(const QString & currentVersion){
	QFile f("/upgrade/report");

	for (int i = 0; i < PARTITION_COUNT; ++i)
		strncpy(g_partition_info[i].version, qPrintable(currentVersion), currentVersion.length());

	if ( f.open(QIODevice::ReadOnly)){
		QTextStream in(&f);
		QString line;
		do {
			line = in.readLine();
			QString info = line.trimmed();

			if ( info.isEmpty()) continue;

			QStringList token = info.split("\t");
			if ( token.count() == 2){
				for (int i = 0; i < PARTITION_COUNT; ++i){
					if ( token.at(0) == g_partition_info[i].partition_name){
						memset(g_partition_info[i].version, 0, sizeof(g_partition_info[i].version));
						strncpy(g_partition_info[i].version, qPrintable(token.at(1)), token.at(1).length());
					}
				}
			}
		} while (!line.isNull());
		f.close();
	}
}

void DiscoveryPartition::changePartitionNameForVariant(int variant){
        ::strncat(g_partition_info[GracenotePartition].partition_name,"_", 30 - strlen(g_partition_info[GracenotePartition].partition_name));
	::strncat(g_partition_info[GracenotePartition].partition_name, g_variant_str[variant], 30 - strlen(g_partition_info[GracenotePartition].partition_name));
}

void DiscoveryPartition::setBootPartitionInfo(bool isReadOnly, bool spareExist){
	if ( spareExist ){
		if ( isReadOnly )
			strncpy(g_partition_info[BootPartition].dev_name, "/dev/sda5", DEV_NAME_MAX_LEN);
		else
			strncpy(g_partition_info[BootPartition].dev_name, "/dev/sda11", DEV_NAME_MAX_LEN);
	}
	else{
		strncpy(g_partition_info[BootPartition].dev_name, "/dev/sda1", DEV_NAME_MAX_LEN);
	}	
}

void DiscoveryPartition::setRootPartitionInfo(const QString & rootfs){
	QString dev = rootfs;
	char mainRootFS = '2';
	char spareRootFS = (g_isReadonly ? '6' : '5');
	dev.remove(SDA_PARTITION_NAME);	

	if ( dev == QString(mainRootFS))
		g_partition_info[RootPartition].dev_name[8] = spareRootFS;
	else if ( dev == QString(spareRootFS))
		g_partition_info[RootPartition].dev_name[8] = mainRootFS;
	else{
		ULOG_EXCEPTION;
		ULOG->log("Cannot find matching rootfs");
	}
}

void DiscoveryPartition::setPartitionInfo(bool isReadOnly){
	strncpy(g_partition_info[RootPartition].dev_name, "/dev/sda0", DEV_NAME_MAX_LEN);

	if ( isReadOnly){
		strncpy(g_partition_info[UserDataPartition].dev_name, "/dev/sda8", DEV_NAME_MAX_LEN);
		strncpy(g_partition_info[VrPartition].dev_name,	"/dev/sda11", DEV_NAME_MAX_LEN);
		strncpy(g_partition_info[GracenotePartition].dev_name, "/dev/sda12", DEV_NAME_MAX_LEN);
		strncpy(g_partition_info[CommonPartition].dev_name, "/dev/sda13", DEV_NAME_MAX_LEN);	
	}
	else{
		strncpy(g_partition_info[UserDataPartition].dev_name, "/dev/sda10", DEV_NAME_MAX_LEN);
		strncpy(g_partition_info[VrPartition].dev_name,	"/dev/sda6", DEV_NAME_MAX_LEN);
		strncpy(g_partition_info[GracenotePartition].dev_name, "/dev/sda7", DEV_NAME_MAX_LEN);
		strncpy(g_partition_info[CommonPartition].dev_name, "/dev/sda12", DEV_NAME_MAX_LEN);	
	}
}

bool DiscoveryPartition::checkSpareBoot(){
	QString spareBootFS = (g_isReadonly ? "/dev/sda5" : "/dev/sda11");
	return QFile::exists(spareBootFS);
}
