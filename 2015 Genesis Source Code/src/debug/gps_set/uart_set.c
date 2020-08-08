#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "command.h"
#include "uart_set.h"

static struct termios newtio;

int open_gps_uart(void)
{
	int fd;

	fd = open(DEV_GPS_UART, O_RDWR | O_NOCTTY);
	if( fd < 0 ) {
		printf("Could not open UART.\n");
		return 0;
	}

	return fd;
}

void close_gps_uart(int fd)
{
	if( fd < 0 ) {
		printf("Wrong fd number.\n");
		return;
	}
	close(fd);
}

int set_uart_comm(int fd, int set_num)
{

	memset(&newtio, 0, sizeof(newtio));

	/* UART comm. configuration setting(case1 : boot_mode, case2 : normal_mode)*/
	switch(set_num) {
		case BOOT:
			/*change B38400->B115200 for 2th board, delete | PARENB*/
			newtio.c_cflag = B38400 | CS8 | CLOCAL | CREAD | PARENB;
			newtio.c_oflag = 0;
			printf("Setting the UART comm for Boot_mode\n");
			break;

		case NORMAL:
			newtio.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
			newtio.c_oflag = ICANON;
			printf("Setting the UART comm for normal_mode\n");
			break;

		default:
			break;
	}
	newtio.c_iflag = 0;
	newtio.c_lflag = 0;

	newtio.c_cc[VMIN] = 1;
	newtio.c_cc[VTIME] = 0;		

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);

	return 0;
}
