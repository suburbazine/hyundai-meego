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

#include "ucs_daemon.h"
#include "i2c_rw.h"
#include "err_monitor.h"
#include "config.h"
#include "udevmon.h"

/*
 * 6th generation hardware have a UCS8100x for front UBS port.
 * If not built-in UCS8100x at rear, that HW is 5th or under version.
 * 6th or newer HW use M2 of rear for ALERT# function.
 */
int ucs_builtin_front = FALSE;
int ucs_builtin_rear  = FALSE;
int ucs_builtin_state = 0;
int dhavn_hw_version  = 7;
int variant_rear_usb  = 1;
int car_model_type = 0; /* 0:DH / 1:KH  / 2:VI */

/* checkou USB mode of DH, check variant code */
void set_ucs81002_mode_set_use_gpio(void)
{
	int fd, fd_sysfs;
	char buf[32] = {0, };
	gpio_info_t 	g_rear_usb_pwr_en;

	fd = open(REAR_USB_EN_INFO_FILE, O_RDWR);
	if( fd < 0 ) {
		syslog(LOG_INFO, "could not open variant_rear_usb file. rear usb default on!");
	} else {
		read(fd, buf, sizeof(buf)); 
		variant_rear_usb = atoi(buf);
		close(fd);
	}

	/* rear USB variant check (just over 8th hardware) */
	g_rear_usb_pwr_en.gpio_num = DH_GPIO_REAR_USB_EN;

	if( !variant_rear_usb && dhavn_hw_version >= 8 ) {
		system("echo \"Rear USB is No\" > /dev/kmsg");

		/* Remove rear usb device sysfs before power down */
		fd_sysfs = open(REAR_USB_REMOVE_SYS_FILE, O_WRONLY);
		if( fd_sysfs >= 0 ) {
			write(fd_sysfs, "1", 1);
			close(fd_sysfs);
			usleep(100*1000);
		}

		g_rear_usb_pwr_en.value = PIN_LOW;
		set_gpio_value_general(g_rear_usb_pwr_en);
	} else {
		g_rear_usb_pwr_en.value = PIN_HIGH;
		set_gpio_value_general(g_rear_usb_pwr_en);
	}
}

int set_ucs8100x_alert(void)
{
	int fd;
	fd = open(SMSC_USB_IC_IRQ_DEV_FILE, O_RDONLY);
	if( fd < 0 ) {
		syslog(LOG_ERR, "Could not open device node file for UCS Alert interrupt.");
	}
	return fd;
}

int ucs8100x_ic_init(int fd, unsigned char ucs_mode)
{
	unsigned char tx_buf[4] = {0, };
	unsigned char rx_buf[4] = {0, };
	unsigned char hw_rev_info[8] = {0, };
	unsigned char car_model_info[8] = {0, };
	unsigned char reg_val_current;
	unsigned char reg_val_general;
	int ucs_ic_front_only_flag = 0;
	int fd_hw_info; 
	int fd_car_info;
	int ret;
	
	gpio_info_t 	g_front_usb_pwr_en, g_front_usb_em_en, g_front_usb_m1, g_front_usb_m2;

	fd_car_info = open(CAR_TYPE_INFO_FILE, O_RDONLY);
	if( fd_car_info < 0 ){
		syslog(LOG_INFO, "could not get car model type information.");
		car_model_type = 0;
	} else {
		ret = read(fd_car_info, car_model_info, sizeof(car_model_info));
		if( ret > 0 ) {
			car_model_type = atoi(car_model_info);
		}
		close(fd_car_info);
	}

	fd_hw_info = open(HW_VERSION_INFO_FILE, O_RDONLY);
	if( fd_hw_info < 0 ){
		syslog(LOG_INFO, "could not get hardware version information.");
	} else {
		ret = read(fd_hw_info, hw_rev_info, sizeof(hw_rev_info));
		if( ret > 0 ) {
			dhavn_hw_version = atoi(hw_rev_info);
			if( dhavn_hw_version > 5 ) {
				ucs_ic_front_only_flag = 1;
			} else { 
				ucs_ic_front_only_flag = 0;
			}
		}
		close(fd_hw_info);
	}
	
	/* USB port power on use GPIO, if could not found i2c dev info */
	set_ucs81002_mode_set_use_gpio();

	/* Read interrupt register of UCS8100x */
	if( i2c_io_read(fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_INT_STATE_REG, rx_buf, 1) < 1 ) {
		ucs_builtin_front = FALSE;

		/* front USB setting value */
		g_front_usb_pwr_en.gpio_num = DH_GPIO_F_USB_PWR_EN;
		g_front_usb_pwr_en.value = PIN_HIGH;
		g_front_usb_em_en.gpio_num = DH_GPIO_F_USB_EM_EN;
		g_front_usb_em_en.value = PIN_HIGH;
		g_front_usb_m1.gpio_num = DH_GPIO_F_USB_M1;
		g_front_usb_m1.value = PIN_HIGH;
		g_front_usb_m2.gpio_num = DH_GPIO_F_USB_M2;
		g_front_usb_m2.value = PIN_HIGH;

		set_gpio_value_general(g_front_usb_pwr_en);
		set_gpio_value_general(g_front_usb_em_en);
		set_gpio_value_general(g_front_usb_m1);
		set_gpio_value_general(g_front_usb_m2);

		syslog(LOG_INFO, "Not found SMSC UCS8100x IC from front port. USB on use GPIO.");
	} else {
		ucs_builtin_front = TRUE;
		syslog(LOG_INFO, "Read Front UCS8100x interrupt state reg. [%02X]", rx_buf[0]);
	}

	if( ucs_builtin_front == TRUE ) { 
		tx_buf[0] = ucs_mode ^ UCS_PWR_SWITCH_OFF; /* Turn off UCS8100x */
		i2c_io_write(fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1); 

		usleep(100000);  /* delay for actual power down of ucs81002 */

		tx_buf[0] = ucs_mode; /* Turn on UCS8100x mode is CDP */
		i2c_io_write(fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1); 

		i2c_io_read(fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_GEN_STATE_REG, rx_buf, 1);
		reg_val_general = rx_buf[0];

		i2c_io_read(fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_CURRENT_REG, rx_buf, 1);
		reg_val_current = rx_buf[0];

		/* reset UCS81002 (avoid attach fail at booting time) */
		if( reg_val_current == 0x00 ) {
			tx_buf[0] = ucs_mode ^ UCS_PWR_SWITCH_OFF;
			i2c_io_write(fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1); 

			usleep(100000);  /* delay for actual power down of ucs81002 */

			tx_buf[0] = ucs_mode;
			i2c_io_write(fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1); 
		}
	}

	/* if exist rear UCS81002 IC */
	if( !ucs_ic_front_only_flag ) { 
		if( i2c_io_read(fd, SMSC_USB_IC_ADDR_REAR, SMSC_USB_IC_INT_STATE_REG, rx_buf, 1) < 1 ) {
			ucs_builtin_rear = FALSE;
			syslog(LOG_INFO, "Not found SMSC UCS8100x IC from rear port.");
		} else {
			ucs_builtin_rear = TRUE;
			syslog(LOG_INFO, "Read rear UCS8100x interrupt state reg. [%02X]", rx_buf[0]);
		}

		if( ucs_builtin_rear == TRUE ) {
			tx_buf[0] = ucs_mode ^ UCS_PWR_SWITCH_OFF; /* Turn off UCS8100x */
			i2c_io_write(fd, SMSC_USB_IC_ADDR_REAR, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1); 

			usleep(100000);  /* delay for actual power down of ucs81002 */

			tx_buf[0] = ucs_mode; /* Turn on UCS8100x mode is CDP */
			i2c_io_write(fd, SMSC_USB_IC_ADDR_REAR, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1); 
		}
	}

	return ucs_builtin_front + ucs_builtin_rear; 
}

int main(int argc, char **argv)
{
	const unsigned int num_of_thread = 5;
	int ucs_fd = 0;
	int thread_retval[num_of_thread];

	unsigned char sys_cmd[128] = {0, };

	pthread_t 		ucs_mon_thread[num_of_thread]; 
	polling_info_t	m_poll_info;
	alert_info_t 	m_alert_info;
	ucsd_conf_t 	m_conf_info;

	/* TODO: UCS8100x Error State Check Daemon Process
	 */
	if( daemon(1, 1) )
		return 1;

	/* Ignore Keyboard option check */
	ignore_keyboard();

	/* get UCS81002 set config. (default CDP on DH) */
	m_conf_info = get_ucsd_config(SMSC_USB_IC_CONF_FILE);

	openlog(PROGRAM_NAME, LOG_PID | LOG_CONS, LOG_DAEMON);

	ucs_fd = open_dev_ucs8100x(SMSC_USB_IC_I2C_DEV_FILE, &ucs_builtin_state);

	if(!ucs_builtin_state) {
		syslog(LOG_ERR, "Could not detect UCS8100x IC.");
	} else {
		switch(ucs_builtin_state) {
			case 1:
				ucs_builtin_front = TRUE;
				ucs_builtin_rear = FALSE;
				break;
			case 2:
				ucs_builtin_front = FALSE;
				ucs_builtin_rear = TRUE;
				break;
			case 3:
				ucs_builtin_front = TRUE;
				ucs_builtin_rear = TRUE;
				break;
			default:
				break;
		}
	}

	/* Front & rear IC check use read/write function. */
	ucs_builtin_state = ucs8100x_ic_init(ucs_fd, m_conf_info.ucs_mode);

	/* If UCS81002 built-in only at front port, that HW is 6th gen. or newer.
	 * Load IRQ driver and gpio mode change M2(rear) pin, for ALERT# function.
	 */
	m_alert_info.irq_fd = -1;
	if( ucs_builtin_state == 1 ) {
		snprintf(sys_cmd, sizeof(sys_cmd), "%s %s", SYS_CMD_MODPROBE, UCS_ALERT_DEV_MODULE);
		system(sys_cmd);
		m_alert_info.irq_fd = set_ucs8100x_alert();
	}

	/* ALERT# pin interrupt monitorring, if 6th board or newer */
	if( m_alert_info.irq_fd > -1 ) {
		//syslog(LOG_ERR|LOG_LOCAL0, "UCS81002 ALERT# pin was detected.");
		m_alert_info.alert_state = 0;
		m_alert_info.alert_monitor = 1;
		/* udev alert monitor thread */
		pthread_create(&ucs_mon_thread[0], NULL, check_ucs_alert_pin, (void*)&m_alert_info);
	} else {
		m_alert_info.alert_monitor = 0;
	}

	/* UCS8100x interrupt register monitorring */
	m_poll_info.i2c_fd = ucs_fd;
	m_poll_info.delay = m_conf_info.delay;
	m_poll_info.builtin_front = ucs_builtin_front;
	m_poll_info.builtin_rear = ucs_builtin_rear;

	/* Monitoring the USB portrs of DH-AVN */
	pthread_create(&ucs_mon_thread[1], NULL, check_usb_port_error_state, (void*)&m_poll_info);
	
	/* udevmonitor spy thread */
	pthread_create(&ucs_mon_thread[2], NULL, udevd_msg_checker, NULL);

	/* USB remove signal monitor thread */
	//pthread_create(&ucs_mon_thread[3], NULL, usb_remove_monitor, NULL);

	/* pthread join */
	if( m_alert_info.irq_fd > -1 ) {
		pthread_join(ucs_mon_thread[0], (void **)&thread_retval[0]);
	}
	pthread_join(ucs_mon_thread[1], (void **)&thread_retval[1]);  // USB port monitor
	pthread_join(ucs_mon_thread[2], (void **)&thread_retval[2]);  // udev monitor
	//pthread_join(ucs_mon_thread[3], (void **)&thread_retval[3]);  // remove monitor

	if( ucs_fd > 0 )
		close(ucs_fd);

	closelog();

	return 0;
}

