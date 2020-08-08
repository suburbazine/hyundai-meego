/*
 * Copyright (C) 2010 OKI SEMICONDUCTOR CO., LTD.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef __IOH_MLB_API_H__
#define __IOH_MLB_API_H__

#ifndef IOH_MLB_NUM_CHANNELS
#	define IOH_MLB_NUM_CHANNELS (31)
#endif	/* #ifndef IOH_MLB_NUM_CHANNELS */

#ifndef IOH_MLB_MAX_DEVICES
#	define IOH_MLB_MAX_DEVICES (1)
#endif	/* #ifndef IOH_MLB_MAX_DEVICES */

#ifndef IOH_MLB_FIRST_MINOR
#	define IOH_MLB_FIRST_MINOR (0)
#endif	/* #ifndef MLB_TEST_FIRST_MINOR */

enum {
	RX = 0,
	TX,
	TXRX,
};

enum {
	SYNCHRONOUS = 0,
	ISOCHRONOUS,
	ASYNCHRONOUS,
	CONTROL,
};

enum {
	CLK256FS = 0,
	CLK512FS,
	CLK1024FS,
	CLKRESERVED,
};

enum {
	PING_PONG = 0,
	CIRCULAR,
};

struct ioh_mlb_system_setting {
	int mlb_speed;
	int device_address;
};

struct ioh_mlb_channel_setting {
	unsigned short channel_address;
	int direction;
	union {
		int max_packet_size;		/* bytes */
		int max_message_size;		/* bytes */
		int physical_channel_counts;	/* number of physical channels */
	};
};

#define IOH_MLB_SYS_IOCTL_MAGIC		0xAA
#define IOH_MLB_CH_IOCTL_MAGIC		0x55

enum {
	SYS_IOCTL_SET_SYSTEM_NR = 0,
	SYS_IOCTL_START_NR,
	SYS_IOCTL_STOP_NR,
	SYS_IOCTL_WAIT_FOR_SYSTEM_START_NR,
	SYS_IOCTL_MLB_RESET,
};
#define IOH_MLB_SYS_IOCTL_SET_SYSTEM					\
	_IOW(IOH_MLB_SYS_IOCTL_MAGIC, SYS_IOCTL_SET_SYSTEM_NR, struct ioh_mlb_system_setting)
#define IOH_MLB_SYS_IOCTL_START					\
	_IO(IOH_MLB_SYS_IOCTL_MAGIC, SYS_IOCTL_START_NR)
#define IOH_MLB_SYS_IOCTL_STOP				\
	_IO(IOH_MLB_SYS_IOCTL_MAGIC, SYS_IOCTL_STOP_NR)
#define IOH_MLB_SYS_IOCTL_WAIT_FOR_SYSTEM_START				\
	_IO(IOH_MLB_SYS_IOCTL_MAGIC, SYS_IOCTL_WAIT_FOR_SYSTEM_START_NR)
#define IOH_MLB_SYS_IOCTL_MLB_RESET				\
	_IO(IOH_MLB_SYS_IOCTL_MAGIC, SYS_IOCTL_MLB_RESET)

enum {
	CH_IOCTL_SET_CONTROL_CHANNEL_NR = 0,
	CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL_NR,
	CH_IOCTL_SET_SYNCHRONOUS_CHANNEL_NR,
};
#define IOH_MLB_CH_IOCTL_SET_CONTROL_CHANNEL				\
	_IOW(IOH_MLB_CH_IOCTL_MAGIC, CH_IOCTL_SET_CONTROL_CHANNEL_NR, struct ioh_mlb_channel_setting)
#define IOH_MLB_CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL			\
	_IOW(IOH_MLB_CH_IOCTL_MAGIC, CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL_NR, struct ioh_mlb_channel_setting)
#define IOH_MLB_CH_IOCTL_SET_SYNCHRONOUS_CHANNEL			\
	_IOW(IOH_MLB_CH_IOCTL_MAGIC, CH_IOCTL_SET_SYNCHRONOUS_CHANNEL_NR, struct ioh_mlb_channel_setting)

#endif	/* #ifndef __IOH_MLB_API_H__ */
