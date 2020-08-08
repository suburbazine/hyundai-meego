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
#include <pthread.h>
#include <syslog.h>

#include <DHAVN_gpioCtl.h>

#include "control.h"
#include "ucs_daemon.h"
#include "udevmon.h"
#include "i2c_rw.h"
#include "err_monitor.h"

extern int car_model_type;
extern int dhavn_hw_version;
extern int variant_rear_usb;


/* UCS81002(Front USB) IC reset
 * reset delay 1 ~ 9 sec. request time is zero sec.
 * AUX signal will cut, if use delay 0.2 sec.
 */
void ucs81002_reset(unsigned int delay_sec)
{
	int fd; 
	unsigned char tx_buf[4] = {0, };

	/* front USB port power off */
	fd = open(SMSC_USB_IC_I2C_DEV_FILE, O_RDWR);
	if( fd >= 0 ) {
		tx_buf[0] = UCS_MODE_CDP ^ UCS_PWR_SWITCH_OFF;
		i2c_io_write(fd, SMSC_USB_IC_ADDR_FRONT, 
					 SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1); 
		
		if( delay_sec > 0 && delay_sec < 10 ) {
			sleep(delay_sec);
		}

		tx_buf[0] = UCS_MODE_CDP;
		i2c_io_write(fd, SMSC_USB_IC_ADDR_FRONT, 
					 SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1); 

		close(fd);
	} else {
		syslog(LOG_ERR, "\nCould not detect I2C device file.");
	}
}

int usb_device_connection_check(char *usb_path)
{
	if( access(usb_path, F_OK) ) {
		syslog(LOG_ERR, "Could not find USB device sysfs. (%s)", usb_path);
		return 0;
	} else {
		return 1;
	}
}

/* USB device remove use sysfs.
 * for USB disconnect and reconnect delay.
 */
int usb_device_sysfs_remove(int port_num)
{
	int fd;

	switch(port_num)
	{
		case DH_USB_RESET_REQ_PORT_FRONT:
			if( car_model_type == 0 )
				fd = open(DH_FRONT_USB_REMOVE_SYSFS, O_WRONLY);
			else
				fd = open(KH_FRONT_USB_REMOVE_SYSFS, O_WRONLY);

			break;

		case DH_USB_RESET_REQ_PORT_REAR:
			fd = open(DH_REAR_USB_REMOVE_SYSFS, O_WRONLY);
			break;

		default:
			fd = -1;
			break;
	}

	if( fd >= 0 ) {
		write(fd, "1", 1);
		close(fd);
		return 1;
	} else {
		syslog(LOG_INFO, "could not open USB remove sysfs file.");
	}

	return 0;
}

/* USB port power control 
 * Rear USB power can control over 8th HeadUnit.
 */
void usb_port_reset(unsigned int port_num)
{
	int fd, fd_port_power;
	unsigned char tx_buf[4] = {0, };
	gpio_info_t g_rear_usb_pwr_en;

	g_rear_usb_pwr_en.gpio_num = DH_GPIO_REAR_USB_EN;

	switch(port_num) 
	{
		case DH_USB_RESET_REQ_PORT_FRONT:
			syslog(LOG_INFO, "Front USB port off");

			/* check Car model - DH */
			if( car_model_type == 0 ) {
				if( !usb_device_connection_check(DH_USB2_EHCI_FRONT_DEV_DESC) )
					break;

				/* Remove USB device sysfs */
				usb_device_sysfs_remove(DH_USB_RESET_REQ_PORT_FRONT);
				sleep(3);

				snprintf(tx_buf, sizeof(tx_buf), "%d %d", F_USB_END_BUS_NUM_DH, 0);
				fd_port_power = open(DH_USB2_EHCI_FRONT_BOX_POWER, O_RDWR);
			/* check Car model - VI/KH */
			} else {
				if( !usb_device_connection_check(KH_USB1_EHCI_FRONT_DEV_DESC) )
					break;

				/* Remove USB device sysfs */
				usb_device_sysfs_remove(DH_USB_RESET_REQ_PORT_FRONT);
				sleep(3);

				snprintf(tx_buf, sizeof(tx_buf), "%d %d", F_USB_END_BUS_NUM_KH, 0);
				fd_port_power = open(KH_USB1_EHCI_FRONT_BOX_POWER, O_RDWR);
			}

			if(fd_port_power >= 0) {
				write(fd_port_power, tx_buf, 4);
				close(fd_port_power);
			}

			syslog(LOG_INFO, "Front USB port on");
			break;

		case DH_USB_RESET_REQ_PORT_REAR:
			/* Rear USB check, only rear on state */
			if( variant_rear_usb ) {
				if( dhavn_hw_version >= 8 || car_model_type != 0 ) {
					if( !usb_device_connection_check(DH_USB2_EHCI_REAR_DEV_DESC) )
						break;

					syslog(LOG_INFO, "Rear USB port off");

					/* Remove USB device sysfs */
					usb_device_sysfs_remove(DH_USB_RESET_REQ_PORT_REAR);
					sleep(3);

					fd_port_power = open(DH_USB2_EHCI_REAR_BOX_POWER, O_RDWR);
					if(fd_port_power >= 0) {
						snprintf(tx_buf, sizeof(tx_buf), "%d %d", R_USB_END_BUS_NUM, 0);
						write(fd_port_power, tx_buf, 4);
						close(fd_port_power);
					}

					syslog(LOG_INFO, "Rear USB port on");
				} else {
					syslog(LOG_INFO, "Not support rear USB power control");
				}
			}
			break;

		default:
			break;
	}

	/* clear port reset request value */
	fd = open(DH_USB_PORT_RESET, O_RDWR);
	if( fd >= 0 ) {
		write(fd, "0", 1);
		close(fd);
	}
}


void usb_error_state_count_clear(void)
{
	int fd;
	fd = open(DH_USB_ERROR_COUNT, O_RDWR);
	if( fd < 0 ) {
		syslog(LOG_INFO, "Could not open procfs file of USB error state counter");
		return;
	}
	write(fd, "1", 1);
	close(fd);
}
