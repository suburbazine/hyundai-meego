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

#include "config.h"
#include "ucs_daemon.h"

void ignore_keyboard(void)
{
	int fd;
	char tx_buf[4] = {0, }; 

	if( !access(OPTS_IGNORE_KEYBOARD, F_OK) ) {
		snprintf(tx_buf, sizeof(tx_buf),  "%d", 1);
		syslog(LOG_INFO, "Ignore Keyboard option detected.");
	} else {
		snprintf(tx_buf, sizeof(tx_buf), "%d", 0);
	}
	fd = open(PARM_IGNORE_KEYBOARD, O_RDWR);
	if(fd >= 0) {
		write(fd, tx_buf, 1);
		close(fd);
	} else {
		syslog(LOG_ERR, "Could not open ignore_keyboard parameter sysfs.");
	}
}

int parse_delay_second(char *buf)
{
	int delay = 10;

	delay = atoi(buf);
	if(delay < 10)
		delay = 10;

	return delay;
}

char parse_ucs_mode(char *buf)
{
	char ucs_mode = UCS_MODE_CDP;

	if( strstr(buf, "cdp") )
		ucs_mode = UCS_MODE_CDP;
	else if( strstr(buf, "dcp") )
		ucs_mode = UCS_MODE_DCP;
	else if( strstr(buf, "dce") )
		ucs_mode = UCS_MODE_DCP;
	else if( strstr(buf, "sdp") )
		ucs_mode = UCS_MODE_DCP;
	else if( strstr(buf, "data") )
		ucs_mode = UCS_MODE_DATA_PATH_THROUGH;
	else 
		ucs_mode = UCS_MODE_CDP;

	return ucs_mode;
}

/* does not use i2c polling, guided by SMSC
 * so, this program need not polling and mode information. default mode is CDP.
 */
ucsd_conf_t get_ucsd_config(unsigned char *filename)
{
	ucsd_conf_t	conf_info;

	conf_info.delay = 60;
	conf_info.ucs_mode = UCS_MODE_CDP;

	return conf_info;
}
