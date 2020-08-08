#ifndef	PARTITION_VERSION_H_
#define	PARTITION_VERSION_H_

#include "abstractversion.h"

class DiscoveryPartition;
class UpdateElement;
class QFileInfo;

class PartitionVersion : public AbstractVersion{
Q_OBJECT
public:
	PartitionVersion(const QString & aMountPath, QObject * obj = 0);

	void getUpdatePartitions(const QString & rootfs, const QString & systemVersion, const QString & updateVersion); 

	const QString & imageSizeStrings() { return m_imageSizeStrings; }
	
	void filterUpdateList();
	
	bool				mainPartitionUpdateExist() { return m_mainPartitionUpdate; }

private:
	DiscoveryPartition * m_discoveryPartition;
	bool					m_mainPartitionUpdate;

	void collectUSBPartitionImages(const QString & updateVersion);
	QString m_imageSizeStrings;

	bool isIgnorePartition(const QString & str);
	void createUpdateInfo(const QFileInfo &, int index, const QString & updateVersion);
};

#endif
