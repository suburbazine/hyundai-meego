#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include "i2c_rw.h"
#include "ucs_daemon.h"

int i2c_io_read(int dev_fd, unsigned dev_addr, unsigned index, unsigned char *buf, int rx_size)
{
	int ret;
	int cnt_idx;
	unsigned char addr_buf[32] = {0, }; 
	struct i2c_rdwr_ioctl_data 	msgset;
	struct i2c_msg 				msgs[2];

	msgset.msgs = msgs;
	msgset.nmsgs = 2;

	msgs[0].addr = dev_addr;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = addr_buf;

	addr_buf[0] = index;

	msgs[1].addr = dev_addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = rx_size;
	msgs[1].buf = buf;

	for(cnt_idx = 0; cnt_idx < NUM_OF_IC_CHECK; cnt_idx++) {
		ret = ioctl(dev_fd, I2C_RDWR, (struct i2c_rdwr_ioctl_data *) &msgset);
		if(!ret)
			break;
		usleep(MDELAY_OF_IC_CHECK*1000);
	}

	return ret;
}

int i2c_io_write(int dev_fd, unsigned dev_addr, unsigned index, unsigned char *buf, int tx_size)
{
	int ret;
	int idx;
	int cnt_idx;
	char cmd_buf[32] = {0, };
	struct i2c_rdwr_ioctl_data 	msgset;
	struct i2c_msg 				msgs[1];

	msgset.msgs = msgs;
	msgset.nmsgs = 1;

	msgs[0].addr = dev_addr;
	msgs[0].flags = 0;
	msgs[0].len = tx_size + 1;
	msgs[0].buf = cmd_buf;

	cmd_buf[0] = index;

	for(idx = 0; idx < tx_size; idx++) {
		cmd_buf[idx+1] = buf[idx];
	}

	for(cnt_idx = 0; cnt_idx < NUM_OF_IC_CHECK; cnt_idx++) {
		ret = ioctl(dev_fd, I2C_RDWR, (struct i2c_rdwr_ioctl_data *) &msgset);
		if(!ret)
			break;
		usleep(MDELAY_OF_IC_CHECK*1000);
	}

	return ret;
}

int open_dev_ucs8100x(char *filename, int *builtin_state)
{
	int fd;

	fd = open(SMSC_USB_IC_I2C_DEV_FILE, O_RDWR);
	if( fd < 0 ) {
		syslog(LOG_ERR, "> Could not open device file.");
		return fd;
	}

	/* set device for slave */
	if(!ioctl(fd, I2C_SLAVE, SMSC_USB_IC_ADDR_FRONT))
		*builtin_state += DH_UCS_FRONT_BUILTIN;
		
	if(!ioctl(fd, I2C_SLAVE, SMSC_USB_IC_ADDR_REAR)) 
		*builtin_state += DH_UCS_REAR_BUILTIN;

	return fd;
}

void close_dev_ucs8100x(int fd)
{
	close(fd);
}
