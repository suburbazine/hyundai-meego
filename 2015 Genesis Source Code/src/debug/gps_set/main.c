#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/syslog.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdint.h>

#include "command.h"
#include "uart_set.h"
//#include "cmd_processor.h"
//#include "upgrade.h"
//#include "gpio_set.h"

int receive_cmd_timeout(int fd, unsigned char * tmp_data, int buf_size, int timeout);
int send_cmd_in_normal(int fd, int cmd, unsigned char *cmd_src, int cmd_size);
int receive_cmd(int fd, unsigned char *tmp_data);
int parse_cmd(int cmd, int res_size, unsigned char *res_data);

int main(int argc, char **argv)
{
	int fd;
	int fd_ver;
	int ret;
	int go_next;
	struct timeval start, end;
	uint64_t secs, usecs;
	unsigned char read_ver[8] = {0, };
	unsigned char read_init[2048] = {0, };

#if 0
	gettimeofday(&start, NULL);
#endif

    openlog("GPS", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	
	/* for version check */
	fd_ver = open("/sys/module/micom/parameters/hw_version", O_RDONLY);
	if( fd_ver < 0 ){
		printf("Could not open version file.\n");
		return -1;
	}

	ret = read(fd_ver, read_ver, sizeof(read_ver));
	if(ret < 0){
		printf("Could not read version file.\n");
		close(fd_ver);
		return -1;
	}

	if(atoi(&read_ver[0]) < 7){
		printf("Do not need to set, version : %d\n", atoi(&read_ver[0]));
		close(fd_ver);
		return -1;
	}

	close(fd_ver);
	
	fd = open_gps_uart();
	set_uart_comm(fd, NORMAL);
	sleep(1);	

	//read(fd, read_init, sizeof(read_init));
	ret = receive_cmd_timeout(fd, read_init, sizeof(read_init), 1);
	if(ret == 0) {
		printf("time out!! gps_set is fail.\n");
		close_gps_uart(fd);
		return -1;
	}

	syslog(LOG_INFO, "Start GPS set.\n");

	ret = send_cmd_in_normal(fd, CMD_SET, 0, 0);
	if(ret == 0)
		goto exit;
	
	ret = send_cmd_in_normal(fd, CMD_SET, 0, 0);
	if(ret == 0)
		goto exit;
	
	ret = send_cmd_in_normal(fd, CMD_SET, 0, 0);
	if(ret == 0)
		goto exit;

	syslog(LOG_INFO, "Setting command is writed.\n");
	
	ret = send_cmd_in_normal(fd, CMD_ACCELEROMETER, 0, 0);
	if(ret == 0)
		goto exit;
	
	ret = send_cmd_in_normal(fd, CMD_ACCELEROMETER, 0, 0);
	if(ret == 0)
		goto exit;
	
	ret = send_cmd_in_normal(fd, CMD_ACCELEROMETER, 0, 0);
	if(ret == 0)
		goto exit;

	syslog(LOG_INFO, "Accelerometer command is writed.\n");


exit:
	close_gps_uart(fd);

#if 0
	gettimeofday(&end, NULL);
	if( end.tv_sec >= start.tv_sec ){
		secs = end.tv_sec - start.tv_sec;
		while (end.tv_usec < start.tv_usec) {
			  secs --;
			  end.tv_usec += 1000000;
		}
		usecs = end.tv_usec - start.tv_usec;

		openlog("gps", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		syslog(LOG_INFO, "gps set : [%02llu.%06llu] [%d]", secs, usecs, getpid());
		closelog();
	}
#endif
	
	syslog(LOG_INFO, "End GPS set.\n");
	closelog();
	return 0;	
}

int receive_cmd_timeout(int fd, unsigned char * tmp_data, int buf_size, int timeout){
	unsigned int read_size = 0;
	unsigned int total_size = 0;
	struct timeval tv;

	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	fd_set readfds, allfds;
	FD_ZERO(&readfds);

	FD_SET(fd, &readfds);

	memset(tmp_data, 0x00, sizeof(tmp_data));
	allfds = readfds;
	
	if ( select(fd + 1, &allfds, NULL, NULL, &tv) > 0){
		if ( FD_ISSET(fd, &readfds)){
			read_size = read(fd, &tmp_data[total_size], buf_size);

			if(read_size < 1){
				printf("Could not read response\n");
				return 0;
			}
		}
	}
	else{
		return 0;	// timeout
	}
	
	return read_size;
}

int send_cmd_in_normal(int fd, int cmd, unsigned char *cmd_src, int cmd_size)
{
	int i = 0;
	int check, receive_size;
	unsigned char cmd_set[] = {0x81, 0x01, 0x70, 0x60,
								0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
								0x00, 0x00, 0x00, 0x00,
								0x8a, 0x8e, 0x64, 0x3e,
								0xf0, 0x85, 0x79, 0x3f,
								0x43, 0x82};
	unsigned char cmd_accelerometer[] = {
										 0x81, 0x01, 0x70, 0x51, 0x03, 0x0f,
										 0x01, 0x01, 0x00, 0x30,
										 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48,
										 0x43, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x80, 0x00, 0x3f,
										 0x10, 0xae, 0x47, 0x61, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00,
	   									 0x00, 0x20, 0x42, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x80,
	   									 0x00, 0x3f, 0x10, 0xae, 0x47, 0x61, 0x3e, 0x00, 0x00, 0x00,
										 0x00, 0x00, 0x00, 0x20, 0x42, 0x00, 0x00, 0x00, 0x40, 0x00, 
	   									 0x00, 0x80, 0x00, 0x3f, 0x10, 0xae, 0x47, 0x61, 0x3e, 0x00,
										 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x42,
	   									 0xD3, 0x4D, 0x02, 0x40,
										 0xe5, 0x80, 0x04, 0xd2, 0x3d,
										 0xed, 0x2f, 0x78, 0xbf,
										 0xbf, 0xea, 0x63, 0x3e,
										 0xd8, 0xa4, 0x7e, 0x3f,
										 0x65, 0x2e, 0xcd, 0x3d,
										 0x65, 0x6c, 0xbc, 0xbc,
										 0x00, 0x00, 0x00, 0x00,
										 0x77, 0x21, 0x65, 0x3e,
	   									 0x48, 0x80, 0x02, 0x79, 0x3f,
										 0x0a, 0x82};
	unsigned char cmd_query[] = {0x81, 0x02, 0x70, 0x51, 0x2b, 0x82};
	//unsigned char cmd_init_gps[] = {0x81, 0x01, 0x70, 0x71, 0x01, 0x1A, 0x82};
	unsigned char cmd_tmp[1024] = {0x00, };
	unsigned char tmp_data[2048] = {0x00, };

	unsigned char parser_code[4] = {0x00, };

	switch(cmd)
	{
		case CMD_SET :
			check = write(fd, cmd_set, sizeof(cmd_set));
			break;
		case CMD_ACCELEROMETER :
			check = write(fd, cmd_accelerometer, sizeof(cmd_accelerometer));
			break;
		case CMD_QUERY :
			check = write(fd, cmd_query, sizeof(cmd_query));
			break;
		default :
			memcpy(cmd_tmp, cmd_src, cmd_size);
			check = write(fd, cmd_tmp, cmd_size);
			break;
	}
	if(check < 0){
		printf("Could not write this command");
		return 0;
	}

	printf("WRITED CMD SIZE : %d\n", check);
	while(i < 5) {
		memset(tmp_data, 0x00, sizeof(tmp_data));
		//receive_size = read(fd, tmp_data, sizeof(tmp_data));
		receive_size = receive_cmd(fd, tmp_data);
		if(receive_size == 0)
			return 0;

		printf("RECEIVED DATA SIZE : %d\n", receive_size);

		check = parse_cmd(cmd, receive_size, tmp_data);
		if(check == 1)
			break;
		i++;
	}

	return receive_size;
}

int receive_cmd(int fd, unsigned char *tmp_data)
{
	unsigned int read_size = 0;
	unsigned int total_size = 0;

	memset(tmp_data, 0x00, 2048);
	while(1)
	{
		if(total_size > 2000)
			break;

		read_size = receive_cmd_timeout(fd, &tmp_data[total_size], 32, 1);
		total_size += read_size;
	
		if(read_size == 0){
			syslog(LOG_INFO, "Could not recieve ACK, timeout\n");
			return 0;
		} else if(read_size < 1){
			printf("Could not read response\n");
			return 0;
		}
		if(read_size < 32)
			break; 
		if(total_size > 2048)
			break;
	}
	
	return total_size;
}

int parse_cmd(int cmd, int res_size, unsigned char *res_data)
{
	int i, j, cmp;
	int sum = 0;
   	unsigned int cksum = 0;
	unsigned char parse_cmd;
	unsigned char buf[3] = {0, };

	switch(cmd){
		case CMD_SET:
			parse_cmd = 0x60;
			break;
		case CMD_ACCELEROMETER:
			parse_cmd = 0x51;
			break;
		default:
			parse_cmd = 0x00;
			break;
	}

	for(i = 0 ; i < res_size ; i++){

		//if(res_data[i] == 0x81){
		if(res_data[i] == 0x81 && res_data[i + 1] == 0x10){
	
			if(res_data[i + 3] != 0x70 || res_data[i + 4] != parse_cmd)
				continue;
#if 0
			if(cmp != 0 || res_data[i + 1 + (sizeof(buf)) + 1] != 0x00)
				continue;
			
			/*check vaild check_sum*/
			for(j = i ; j <= (i + 1 + sizeof(buf) + 3) ; j++)
				sum += res_data[j];
			
			cksum = sum << 24;
			
			if(cksum != 0 || res_data[i + 1 + sizeof(buf) + 3] != 0x82)
					continue;
#endif
			if(res_data[i + 5] != 0x00){
				return 1;
			}

#if 0	
			printf("check ack data : %d.[%02x] %d.[%02x] %d[%02x] %d[%02x] %d[%02x]\n", 
					i , res_data[i], i+1, res_data[i+1], i+2, res_data[i+2], i+3, res_data[i+3], i+4, res_data[i+4]);
#endif
			return 1;
		}
	}		

	return 0;	
}
