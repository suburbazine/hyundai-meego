#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <syslog.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <DHAVN_gpioCtl.h>

#include "control.h"
#include "udevmon.h"
#include "ucs_daemon.h"
#include "err_monitor.h"

int usb_eth_dbg_enable = FALSE;
int wrong_alloc_usb_bus = 0;
int usb_port_front_pwr_state = DH_USB_POWER_ON;
int usb_port_rear_pwr_state = DH_USB_POWER_ON;
unsigned char usb_port_front_con_delay = 0;
unsigned char usb_port_rear_con_delay = 0;

extern int usb_port_reset_running;
extern int usb_device_remove_running;
extern int car_model_type;

extern usb_mem_info_t	f_usb_mem_info;
extern usb_mem_info_t	r_usb_mem_info;

/*
 * USB ethernet adaptor probe or remove.
 * for DH, only support MCS7830 model.
 */
void debug_eth_adaptor_loader(int state)
{
	if( state == 1 ) {
		system("modprobe mcs7830");
		system("/sbin/ifconfig eth0 100.1.1.100 netmask 255.255.255.0 up");
		system("/sbin/route add 100.1.1.1");
		syslog(LOG_INFO, "eth0 up! IP:100.1.1.100");
	} else {
		system("/sbin/ifconfig eth0 down");
		system("rmmod mcs7830");
		syslog(LOG_INFO, "eth0 down!");
	}
}

/*
 * USB generation version check.
 * Return val: 1 = 1.1 or 1.0 / 2 = 2.0 / 3 = 3.0
 */
int get_usb_generation_version(void)
{
	int fd;
	int gen_version = 2;
	char usb_gen_version[8] = {0, };

	/* DH OHCI path */
	if( car_model_type == 0 ) {
		fd = open(DH_USB_GEN_VERSION, O_RDONLY);

	/* KH/VI OHCI path */
	} else {
		fd = open(KH_USB_GEN_VERSION, O_RDONLY);
	}

	if( fd < 0 ) {
		syslog(LOG_INFO, "Could not open USB generation version sysfs.");
		gen_version = 0;
		return gen_version;
	} else {
		read(fd, usb_gen_version, sizeof(usb_gen_version)-1);
		close(fd);
	}

	if( strstr(usb_gen_version, "1.") )
		gen_version = 1;
	else if( strstr(usb_gen_version, "2.") )
		gen_version = 2;
	else if( strstr(usb_gen_version, "3.") )
		gen_version = 3;
	else 
		gen_version = 0;

	return gen_version;
}

void *udevd_msg_checker(void *arg)
{
	struct udev *udev = NULL;
	struct udev_device *dev;

   	struct udev_monitor *mon;
	int fd;

	/* Create the udev object */
	udev = udev_new();
	if (udev == NULL) {
		syslog(LOG_ERR, "could not create udev.");
		return NULL;
	}

	mon = udev_monitor_new_from_netlink(udev, "udev");
	udev_monitor_filter_add_match_subsystem_devtype(mon, "hidraw", NULL);
	udev_monitor_filter_add_match_subsystem_devtype(mon, "block",NULL);
	udev_monitor_filter_add_match_subsystem_devtype(mon, "net",NULL);
	udev_monitor_enable_receiving(mon);
	/* Get the file descriptor (fd) for the monitor.
	   This fd will get passed to select() */
	fd = udev_monitor_get_fd(mon);

	/* Begin polling for udev events. Events occur when devices
	   attached to the system are added, removed, or change state. 
	   udev_monitor_receive_device() will return a device
	   object representing the device which changed and what type of
	   change occured.

	   The select() system call is used to ensure that the call to
	   udev_monitor_receive_device() will not block.
	   
	   The monitor was set up earler in this file, and monitoring is
	   already underway.
	   
	   This section will run continuously, calling usleep() at the end
	   of each pass. This is to demonstrate how to use a udev_monitor
	   in a non-blocking way. */
	while (1) {
		/* Set up the call to select(). In this case, select() will
		   only operate on a single file descriptor, the one
		   associated with our udev_monitor. Note that the timeval
		   object is set to 0, which will cause select() to not
		   block. */
		fd_set fds;
		struct timeval tv;
		int ret;
		
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		
		ret = select(fd+1, &fds, NULL, NULL, &tv);
		
		/* Check if our file descriptor has received data. */
		if (ret > 0 && FD_ISSET(fd, &fds)) {
			/* Make the call to receive the device.
			   select() ensured that this will not block. */
			dev = udev_monitor_receive_device(mon);
			if (dev) {
				/* Detect USB type ethernet adaptor for debugging */
				if( !strcmp(udev_device_get_subsystem(dev), "net") && 
						!strcmp(udev_device_get_property_value(dev, "ID_MODEL_ID"), "7830")){

					/* check USB ethernet adaptor debuging */
					if( !access(USB_ETH_DBG_INFO_FILE, F_OK) || !access(FORCE_USB_ETH_DBG_INFO_FILE, F_OK) )
						usb_eth_dbg_enable = TRUE;
					else
						usb_eth_dbg_enable = FALSE;

					if( !strcmp(udev_device_get_action(dev), "add") ) {
						if( usb_eth_dbg_enable == TRUE ) {
							syslog(LOG_INFO, "DEBUG USB ETH ADAPTOR ATTACH!"); 
							debug_eth_adaptor_loader(1);
						}
					} else if( !strcmp(udev_device_get_action(dev), "remove") ) {
						syslog(LOG_INFO, "DEBUG USB ETH ADAPTOR REMOVE!"); 
						debug_eth_adaptor_loader(0);
					}
				}

				/* -- START of add message processing -- */
				/*
				 * check add state for block(USB memory) and sound(iPod) device.
				 * device reconnect, if detect OHCI mount.
				 */
				if( (!strcmp(udev_device_get_subsystem(dev), "block") || 
					 !strcmp(udev_device_get_subsystem(dev), "hidraw")) && 
					 !strcmp(udev_device_get_action(dev), "add") ) {
				
					/* attach, EHCI device (normal state) */
					if( strstr( udev_device_get_syspath(dev), DH_USB2_EHCI_ROOT_PATH) || 
						strstr( udev_device_get_syspath(dev), KH_USB1_EHCI_ROOT_PATH ) ) {

						syslog(LOG_INFO, "\ndevice added. node:%s subsys:%s devtype:%s", 
								udev_device_get_devnode(dev), udev_device_get_subsystem(dev), 
								udev_device_get_devtype(dev) );

						/* set block device information for I/O error */
						if( (!strcmp(udev_device_get_subsystem(dev), "block")) ) {
							/* devide front/rear device */	
							if( strstr( udev_device_get_syspath(dev), DH_USB2_EHCI_FRONT_PATH) || 
								strstr( udev_device_get_syspath(dev), KH_USB1_EHCI_FRONT_PATH) ) {
								strncpy(f_usb_mem_info.dev_name, udev_device_get_devnode(dev), 8);
								f_usb_mem_info.err_state = 0;
							} else if( strstr( udev_device_get_syspath(dev), DH_USB2_EHCI_REAR_PATH) ) {
								strncpy(r_usb_mem_info.dev_name, udev_device_get_devnode(dev), 8);
								r_usb_mem_info.err_state = 0;
							}

							/*
							syslog(LOG_INFO, "F_USB_INFO> %s (%d)", 
									f_usb_mem_info.dev_name, f_usb_mem_info.err_state);
							syslog(LOG_INFO, "R_USB_INFO> %s (%d)", 
									r_usb_mem_info.dev_name, r_usb_mem_info.err_state);
							*/
						}

						wrong_alloc_usb_bus = 0;
						usb_error_state_count_clear();

					} else {
						/* attach, not USB 2.0 device on front */
						/* DH rear USB use USB82642 Hub. That IC convert USB1.x to USB2.0 
						 * So, not need check USB1.1 or USB1.0 device. (Always USB2.0*/

						/* inclear wrong port alloc point (mmc_host device pass) */
						if( !strstr(udev_device_get_syspath(dev), "mmc_host") ) {

							syslog(LOG_INFO, "\nnode:%s subsys:%s syspath:%s devtype:%s action:%s", 
									udev_device_get_devnode(dev), udev_device_get_subsystem(dev), 
									udev_device_get_syspath(dev), udev_device_get_devtype(dev), 
									udev_device_get_action(dev));

							if( wrong_alloc_usb_bus < 2 ) {
								/* front USB port power off - Only USB 2.0 or 3.0 device */
								if( get_usb_generation_version() > 1 ) { 
									syslog(LOG_ERR, "\nEnumerate based on 1.1 but device is not. Retry connect!");
									ucs81002_reset(0);	// UCS81002 IC reset, delay 0 sec
									wrong_alloc_usb_bus++;
								} else {
									syslog(LOG_ERR, "\nUSB 1.1 device detect. it's real.");
									wrong_alloc_usb_bus = 0;
								}
							}
						}

					}
				} /* -- END of add message processing -- */

				udev_device_unref(dev);
			}	
		}
		usleep(10000);
	}

	udev_unref(udev);

	return NULL;       
}

/* USB port power management  
 * Not use this function because delay working in kernel.
 */
void *usb_remove_monitor(void *arg)
{
	int fd_usb_remove;
	int ret; 
	char usb_remove_devpath[8] = {0, };

	/* detect remove signal use the chardev of USB disconnect */
	fd_usb_remove = open(DH_USB_REMOVE_CDEV, O_RDONLY);
	if( fd_usb_remove >= 0 ) {
		while( 1 ) {
			memset(usb_remove_devpath, 0, sizeof(usb_remove_devpath));
			ret = read(fd_usb_remove, usb_remove_devpath, sizeof(usb_remove_devpath));

			/* Avoid, if request reset by AppMediaPlayer */
			if( usb_port_reset_running )
				continue;

			usb_device_remove_running = 1;
			/* detect remove signal for front */
			if( !strcmp(usb_remove_devpath, "2") ) {

				usb_port_reset(DH_USB_RESET_REQ_PORT_FRONT);

			/* detect remove signal for rear */
			} else if( !strcmp(usb_remove_devpath, "1.2.2") ) {

				usb_port_reset(DH_USB_RESET_REQ_PORT_REAR);

			}
			usb_device_remove_running = 0;
		}
		close(fd_usb_remove);
	}

	return NULL;
}

