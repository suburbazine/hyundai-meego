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
#include "err_monitor.h"
#include "ucs_daemon.h"
#include "udevmon.h"
#include "i2c_rw.h"

polling_info_t	poll_info;
alert_info_t 	alert_info;
usb_mem_info_t	f_usb_mem_info;
usb_mem_info_t	r_usb_mem_info;

int usb_port_rescan_state = 0;
int usb_port_reset_running = 0;
int usb_device_remove_running = 0;

void ucs_pin_toggle(void)
{	
	static long err_detect_cnt_front = 0;

	unsigned char tx_buf[4] = {0, };
	unsigned char rx_buf_front[4] = {0, };
	unsigned char rx_buf_rear[4] = {0, };

	/* check state register */
	if( poll_info.builtin_front == TRUE ) {
		i2c_io_read(poll_info.i2c_fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_INT_STATE_REG, rx_buf_front, 1);	
	}
	if( poll_info.builtin_rear == TRUE ) {
		i2c_io_read(poll_info.i2c_fd, SMSC_USB_IC_ADDR_REAR, SMSC_USB_IC_INT_STATE_REG, rx_buf_rear, 1);	
	}

	/* UCS81002 will reset in this condition. error state or alert detected */
	if( (poll_info.builtin_front == TRUE && rx_buf_front[0] != UCS_ERR_CLEARED_STATE) 
		|| (alert_info.alert_state == 1) ) {

		err_detect_cnt_front++;

		i2c_io_read(poll_info.i2c_fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, rx_buf_front, 1);
		//syslog(LOG_INFO, "Current, front USB switch state: 0x%02X", rx_buf_front[0]);

		tx_buf[0] = rx_buf_front[0] ^ UCS_PWR_SWITCH_OFF;
		i2c_io_write(poll_info.i2c_fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1); 

		usleep(100000);  /* delay for actual power down of ucs81002 */

		tx_buf[0] = rx_buf_front[0];
		i2c_io_write(poll_info.i2c_fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1); 
		syslog(LOG_INFO, 
				"Front UCS IC power enable toggle. error detect count:%d alert_state:%d", 
				err_detect_cnt_front, alert_info.alert_state);

		alert_info.alert_state = 0;

		/* error bit clear */
		tx_buf[0] = 0x00;
		if( poll_info.builtin_front == TRUE ) {
			i2c_io_write(poll_info.i2c_fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_INT_STATE_REG, tx_buf, 1);	
			//syslog(LOG_INFO, "ucs front interrupt state: 0x%02X", rx_buf_front[0]);
		}

	}
}


/*----------- Threads -----------*/

/*
 * This function polling UCS81002 error bit state use I2C.
 * Not polling error state register after DV2.6.87 (recommend by SMSC)
 */
void *check_ucs_interrupt_state(void *arg)
{
	poll_info = *(polling_info_t*)arg;

	if( poll_info.delay < 10 || poll_info.delay > 1800000 ) 
		poll_info.delay = 10;

	while( 1 ) {
		ucs_pin_toggle();
		sleep(poll_info.delay);
	}
}

/*
 * This function check interrupt of ALERT# pin.
 * In error state, ALERT# pin going low.
 */
void *check_ucs_alert_pin(void *arg)
{
	int read_len;
	char read_buf[16] = {0, };
	
	alert_info = *(alert_info_t*)arg;

	while(1) {
		read_len = read(alert_info.irq_fd, read_buf, sizeof(read_buf));
		if(read_len > 1) {
			syslog(LOG_INFO, "UCS81002 ALERT# interrupt detected!");
			alert_info.alert_state = 1;
			ucs_pin_toggle();
		}
	}
}

/*
 * This function check USB port error state.
 */
void *check_usb_port_error_state(void *arg)
{
	int cnt = 0;
	int fd_err_cnt, fd_port_rst;
	unsigned int num_of_usb_port_error = 0;
	unsigned int num_of_usb_port_reset = 0;
	char buf_err_cnt[4] = {0, };
	char buf_port_rst[4] = {0, };

	/* front/rear USB memory device info clear */
	f_usb_mem_info.dev_name[0] = 0;
	r_usb_mem_info.dev_name[0] = 0;
	r_usb_mem_info.err_state = f_usb_mem_info.err_state = 0;
	r_usb_mem_info.reset_cnt = f_usb_mem_info.reset_cnt = 0;

	fd_err_cnt = open(DH_USB_ERROR_COUNT, O_RDWR);
	if( (fd_err_cnt < 0) ) {
		syslog(LOG_ERR, "Could not open procfs for USB port error count.");
		return NULL; 
	}

	fd_port_rst = open(DH_USB_PORT_RESET, O_RDONLY);
	if( (fd_port_rst < 0) ) {
		syslog(LOG_ERR, "Could not open procfs for USB port reset.");
		close(fd_err_cnt);
		return NULL; 
	}


	while(1) {
		memset(buf_err_cnt, 0, sizeof(buf_err_cnt));
		memset(buf_port_rst, 0, sizeof(buf_port_rst));

		lseek(fd_err_cnt, 0, SEEK_SET);
		lseek(fd_port_rst, 0, SEEK_SET);

		read(fd_err_cnt, buf_err_cnt, sizeof(buf_err_cnt));
		read(fd_port_rst, buf_port_rst, sizeof(buf_port_rst));

		num_of_usb_port_error = atoi(buf_err_cnt);
		num_of_usb_port_reset = atoi(buf_port_rst);

		/* USB bus rescan, if error state of USB is danger */
		if( ( num_of_usb_port_error >= MAX_USB_ERROR_STATE_CNT ) ) {
			syslog(LOG_ERR, "USB port under the stress condition! num_of_usb_port_error:%d reset count:%d", 
					num_of_usb_port_error, ++cnt);

			/* TODO
			 * Pre-Processing for error condition Root Hub
			 */

			/* clear error state */
			lseek(fd_err_cnt, 0, SEEK_SET);
			write(fd_err_cnt, "1", 1);
			num_of_usb_port_reset = 0;

			/* TODO
			 * Post-Processing for error condition Root Hub
			 */

			usb_port_reset(DH_USB_RESET_REQ_PORT_REAR);
			syslog(LOG_ERR, "Rear USB port reset complete! (reset only rear, req. by HMC)"); 
			
			continue;
		}

		/* USB port reset, for request by application */
		if( num_of_usb_port_reset > 0  && num_of_usb_port_reset < 3) {
			usb_port_reset_running = 1;
			syslog(LOG_INFO, "request port reset. (val:%d)", num_of_usb_port_reset);
			usb_port_reset(num_of_usb_port_reset);
			sleep(1);
			usb_port_reset_running = 0;
		}

		usleep(50000);
	}

	close(fd_err_cnt);
	close(fd_port_rst);
}

