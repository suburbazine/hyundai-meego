#include "dhavnlog.h"
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


static FILE *log;
struct timeval start;
static int first_time = 1;


void open_log(const char * filename){
	log = fopen(filename , "aw+");
}

void close_log(){
	fclose(log);
}

void lprintf(const char* fmt, ...)
{
	va_list args;
	struct timeval current;
	uint64_t secs, usecs;
	char string[8192];
	char msg[8192];
        time_t cur_time;
        struct tm *tm_data;

	if (first_time) {
		first_time = 0;
		gettimeofday(&start, NULL);
	}

        time( &cur_time );
        tm_data = localtime( &cur_time );

        gettimeofday(&current, NULL);

	secs = current.tv_sec - start.tv_sec;

	// if exceed 1 day, reset start time
	if ( secs > 86400){
		gettimeofday(&start, NULL);
		gettimeofday(&current, NULL);
		secs = current.tv_sec - start.tv_sec;
	}

	while (current.tv_usec < start.tv_usec) {
		secs --;
		current.tv_usec += 1000000;
	}
	usecs = current.tv_usec - start.tv_usec;

	va_start(args, fmt);
	vsnprintf(msg, 8192, fmt, args);
	va_end(args);

	if (msg[strlen(msg) - 1] == '\n')
		msg[strlen(msg) - 1] = '\0';

        snprintf(string, 8192, "[%04d-%02d-%02d %02d:%02d:%02d][%02llu.%06llu] [%d] %s\n",
                 tm_data->tm_year + 1900,
                 tm_data->tm_mon +1,
                 tm_data->tm_mday,
                 tm_data->tm_hour,
                 tm_data->tm_min,
                 tm_data->tm_sec,
                 secs, usecs, getpid(), msg);

	if (log) {
		fputs(string, log);
		fflush(log);
	} 

}


