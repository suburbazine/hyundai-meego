#include "partition.h"

PartitionInfo g_partition_info[PARTITION_COUNT] = {
	{"boot", 		"/boot",		{0}, {0}, 15},
	{"rootfs",		"/",			{0}, {0}, 150},	// Unknown rootfs
	{"vr",			"/vr",			{0}, {0}, 160},
	{"gracenote", 	"/gracenote", 	{0}, {0}, 96},
	{"user_data",	"/usr_data",	{0}, {0}, 200},
	{"common_data", "/common_data", {0}, {0}, 200},
};


