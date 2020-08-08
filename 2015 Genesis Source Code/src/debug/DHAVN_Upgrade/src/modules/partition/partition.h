#ifndef	PARTITION_H_
#define	PARTITION_H_

#include "common.h"

#define DEV_NAME_MAX_LEN	14

enum PartitionName {
	BootPartition,
	RootPartition,
	VrPartition,
	GracenotePartition,
	UserDataPartition,
	CommonPartition
};

typedef struct {
	char			partition_name[30];
	const char *	mount_point;
	char			dev_name[15];
	char			version[30];
	unsigned int	weight;
} PartitionInfo;

extern PartitionInfo g_partition_info[PARTITION_COUNT];

#endif
