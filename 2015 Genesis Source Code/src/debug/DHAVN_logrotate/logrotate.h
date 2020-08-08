#ifndef LOGROTATE_H
#define LOGROTATE_H

#define LOG_FLAG_COMPRESS	(1<<0)

struct logInfo {
		char ** files;
		int numFiles;
		int flags;
		long rotateSize;
		long rotatePeriod;
		int rotateCount;
		char * pre, * post;
		char * compress_prog;
		char * compress_ext;
		char * compress_options_list;
		int compress_options_count;
};

struct timeval past_t;
unsigned char **file_list;

#endif
