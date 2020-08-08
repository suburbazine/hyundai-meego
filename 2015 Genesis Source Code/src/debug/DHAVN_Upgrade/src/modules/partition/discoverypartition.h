#ifndef	DISCOVERY_PARTITION_H_
#define	DISCOVERY_PARTITION_H_

#include <QObject>



class DiscoveryPartition : public QObject {
Q_OBJECT
public:
	DiscoveryPartition(QObject * obj = 0);

private:
	bool m_spareBootPartitionExist;

	bool checkSpareBoot();

	void setBootPartitionInfo(bool isReadOnly, bool spareExist);
	void setPartitionInfo(bool isReadOnly);
	void changePartitionNameForVariant(int variant);

public:
	void setRootPartitionInfo(const QString & rootfs);
	void setCurrentVersion(const QString & currentVersion);
};


#endif
