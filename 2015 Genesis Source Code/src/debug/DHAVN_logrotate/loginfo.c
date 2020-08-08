#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "loginfo.h"
#include "logrotate.h"

#define	REALLOC_STEP		10
#define LOG_DIR				"/log_data/log/"

/*count set { }*/
int numLogs;
struct logInfo ** logs;
enum {
		STATE_DEFAULT = 2,
		STATE_SKIP_LINE = 4,
		STATE_LOAD_SCRIPT = 8
};

static char *isolateLine(char **strt, char **buf, size_t length) {
	char *endtag, *start, *tmp;
	start = *strt;
	endtag = start;
	while (endtag - *buf < length && *endtag != '\n') {
		endtag++;}
	if (endtag - *buf > length)
		return NULL;
	tmp = endtag - 1;
	while (isspace(*endtag))
		endtag--;
	char *key = strndup(start, endtag - start + 1);
	*strt = tmp;
	return key;
}

static char *isolateValue(const char *fileName, int lineNum, char *key,
			char **startPtr, char **buf, size_t length)
{
    char *chptr = *startPtr;

    while (chptr - *buf < length && isblank(*chptr))
	chptr++;
    if (chptr - *buf < length && *chptr == '=') {
	chptr++;
	while ( chptr - *buf < length && isblank(*chptr))
	    chptr++;
    }

    if (chptr - *buf < length && *chptr == '\n') {
		return NULL;
    }

	*startPtr = chptr;
	return isolateLine(startPtr, buf, length);
}

static char *isolateWord(char **strt, char **buf, size_t length) {
	char *endtag, *start;
	
	start = *strt;
	while (start - *buf < length && isblank(*start))
		start++;
	endtag = start;
	while (endtag - *buf < length && isalpha(*endtag)) {
		endtag++;}
	if (endtag - *buf > length)
		return NULL;
	char *key = strndup(start, endtag - start);
	*strt = endtag;
	return key;
}

static void copyLogInfo(struct logInfo *to, struct logInfo *from)
{
    memset(to, 0, sizeof(*to));
	
	to->numFiles = from->numFiles;
	to->flags = from->flags;
	to->rotateSize = from->rotateSize;
	to->rotatePeriod = from->rotatePeriod;
	to->rotateCount = from->rotateCount;
	to->compress_options_count = from->compress_options_count;
	to->compress_prog = from->compress_prog;
	to->compress_ext = from->compress_ext;
	to->compress_options_list = from->compress_options_list;
	to->pre = from->pre;
	to->post = from->post;
	to->files = from->files;
}

static int compar(const void *p, const void *q)
{
    return strcoll(*((char **) p), *((char **) q));
}

void free_2d_array(char **array, int lines_count)
{
    int i;
	for (i = 0; i < lines_count; ++i)
			free(array[i]);
	free(array);
}

static int get_logFile(const char *path, struct logInfo * log)
{
	struct dirent *dp;
	char **namelist, **p;
	int files_count, i;
	DIR *dirp;

	if ((dirp = opendir(path)) == NULL)
	    return 1;
	
	files_count = 0;
	namelist = NULL;
	while ((dp = readdir(dirp)) != NULL) {
		if (dp->d_name[0] == '.' &&
					   	(!dp->d_name[1] || (dp->d_name[1] == '.' && !dp->d_name[2])))
		continue;
		/* Realloc memory for namelist array if necessary */
		if (files_count % REALLOC_STEP == 0) {
		    p = (char **) realloc(namelist,
					  (files_count +
					   REALLOC_STEP) * sizeof(char *));
		    if (p) {
			namelist = p;
			memset(namelist + files_count, '\0',
			       REALLOC_STEP * sizeof(char *));
		    } else {
			free_2d_array(namelist, files_count);
			closedir(dirp);
			return 1;
		    }
		}
		/* Alloc memory for file name */
		if ((namelist[files_count] =
		     (char *) malloc(strlen(dp->d_name) + 1))) {
		    strcpy(namelist[files_count], dp->d_name);
		    files_count++;
		} else {
		    free_2d_array(namelist, files_count);
		    closedir(dirp);
		    return 1;
		}
	}
	closedir(dirp);

	if (files_count > 0) 
	    qsort(namelist, files_count, sizeof(char *), compar);

	log->numFiles = files_count;
	log->files = (char **) malloc(files_count * sizeof(char *));
	for(i = 0 ; i < files_count ; i++) {
			log->files[i] = (char *)malloc(strlen(path) + strlen(namelist[i]) + 1);
			snprintf(log->files[i], strlen(path) + strlen(namelist[i]) + 1,"%s%s", path, namelist[i]);
	}
	
	free_2d_array(namelist, files_count);
    return 0;
}

static int newLogInfo()
{
		int i;
		struct logInfo ** p;
		struct logInfo defConfig = {
				.files = NULL,
				.numFiles = 0,
				.flags = 0,
				.rotateSize = 0,
				.rotatePeriod = 1,
				.rotateCount = 1,
				.pre = NULL,
				.post = NULL,
				.compress_prog = "/bin/tar",
				.compress_ext = ".tar",
				.compress_options_list = "cfP",
				.compress_options_count = 1
		};

		if (numLogs == 0){
				logs = (struct logInfo **)malloc(sizeof(struct logInfo *));
		}
		else {
				p = (struct logInfo **)realloc(logs,
								(numLogs + 1) * sizeof(struct logInfo *));
				if (p) 
						logs = p;
				else {
						for(i = 0; i < numLogs ; i++)
								free(logs[i]);
						free(logs);
						return 1;
				}
		}

		if (logs[numLogs] = malloc(sizeof(struct logInfo)))
				copyLogInfo(logs[numLogs], &defConfig);
		else {
				for(i = 0; i < numLogs ; i++)
						free(logs[i]);
				free(logs);
				return 1;
		}

		return 0;	
}

int readConfigFile(const char * configFile) {
		int fd;
		int lineNum = 1;
		int multiplier;
		int state = STATE_DEFAULT;
		char *buf,  *endtag, *key = NULL;
		char **scriptDest = NULL;
		char *start, *chptr;
		off_t length;
		struct stat sb;

		fd = open(configFile, O_RDONLY);
		if (fd < 0) {
				return 1;
		}
		
		if (fstat(fd, &sb)) {
				close(fd);
				return 1;
		}
		
		length = sb.st_size;
		if (length == 0) {
				close(fd);
				return 1;
		}
#ifdef MAP_POPULATE
		buf = mmap(NULL, (size_t) length, PROT_READ,
						MAP_PRIVATE | MAP_POPULATE, fd, (off_t) 0);
#else /* MAP_POPULATE */
		buf = mmap(NULL, (size_t) length, PROT_READ,
						MAP_PRIVATE, fd, (off_t) 0);
#endif /* MAP_POPULATE */

		if (buf == MAP_FAILED) {
				close(fd);
				return 1;
		}
#ifdef MADV_DONTFORK
		madvise(buf, (size_t)(length + 2),
						MADV_SEQUENTIAL | MADV_WILLNEED | MADV_DONTFORK);
#else /* MADV_DONTFORK */
		madvise(buf, (size_t)(length + 2),
						MADV_SEQUENTIAL | MADV_WILLNEED);
#endif /* MADV_DONTFORK */

		start = buf;
		numLogs = 0;
		for (start = buf; start - buf < length; start++) {
				if (key) {
						free(key);
						key = NULL;
				}
				switch(state) {
						case STATE_DEFAULT : 
								if (isblank(*start))
										continue;
								/* Skip comment */
								if (*start == '#') {
										state = STATE_SKIP_LINE;
										continue;
								}

								if (isalpha(*start)) {
										if ((key = isolateWord(&start, &buf, length)) == NULL)
												continue;
										if (!strcmp(key, "compress")) {
												logs[numLogs]->flags = LOG_FLAG_COMPRESS;
										} else if (!strcmp(key, "size") ) {
												unsigned long long size = 0;
												char *opt = key;

												if ((key = isolateValue(configFile, lineNum, opt, &start,
																				&buf, length)) != NULL) {
														int l = strlen(key) - 1;
														if (key[l] == 'k' || key[l] == 'K') {
																key[l] = '\0';
																multiplier = 1024;
														} else if (key[l] == 'M' || key[l] == 'm') {
																key[l] = '\0';
																multiplier = 1024 * 1024;
														} else if (key[l] == 'G') {
																key[l] = '\0';
																multiplier = 1024 * 1024 * 1024;
														} else if (!isdigit(key[l])) {
																free(opt);
																goto error;

														} else 
																multiplier = 1;

														size = (unsigned long long)multiplier * (unsigned long long)strtoul(key, &chptr, 0);
														logs[numLogs]->rotateSize = size;
														if (*chptr) {
																free(opt);
																goto error;
														}
														free(opt);
												}
												else {
														free(opt);
														continue;
												} 
										} else if (!strcmp(key, "rotate")) {
												free(key);
												if ((key = isolateValue(configFile, lineNum, "rotate count", &start,
																				&buf, length)) != NULL) {
														logs[numLogs]->rotateCount = strtoul(key, &chptr, 0);
														if (*chptr || logs[numLogs]->rotateCount < 0) {
																goto error;
														}
														else 
															continue;
												}
										} else if (!strcmp(key, "period")) {
												free(key);
												if ((key = isolateValue(configFile, lineNum, "rotate period", &start,
																				&buf, length)) != NULL) {
														logs[numLogs]->rotatePeriod = strtoul(key, &chptr, 0);
														if (*chptr || logs[numLogs]->rotateCount < 0) {
																goto error;
														}
														else continue;
												}
										} else if (!strcmp(key, "prerotate")) {
												scriptDest = &logs[numLogs]->pre;
												state = STATE_LOAD_SCRIPT;
										} else if (!strcmp(key, "postrotate")) {
												scriptDest = &logs[numLogs]->post;
												state = STATE_LOAD_SCRIPT;
										} else if (!strcmp(key, "default")) {
											if(newLogInfo())
												goto error;
											if(get_logFile(LOG_DIR, logs[numLogs]))
												return 1;
										} else {
											if (*start != '\n')
												state = STATE_SKIP_LINE;
										}
								} else if (*start == '/') {
										endtag = start;
										while (endtag - buf < length && *endtag != '{')
												endtag++;
										if (endtag - buf > length)
												continue;
										if (newLogInfo())
												goto error;
										char *key = strndup(start, endtag - start);
										start = endtag;
									
										key = strtok(key, " ");
										while(key != NULL) {
												logs[numLogs]->numFiles++;
												logs[numLogs]->files = realloc(logs[numLogs]->files, sizeof(char *) * logs[numLogs]->numFiles);
												logs[numLogs]->files[logs[numLogs]->numFiles - 1] = malloc(strlen(key) + 1);
												strncpy(logs[numLogs]->files[logs[numLogs]->numFiles - 1], key, strlen(key));
												logs[numLogs]->files[logs[numLogs]->numFiles -1][strlen(key)] = '\0';
												key = strtok(NULL, " ");
										}
								} else if (*start == '{') {
										if(newLogInfo())
												return 1;
								} else if (*start == '}') {
										numLogs++;
								}
								break;

						case STATE_LOAD_SCRIPT :
								if (*start == '\n')
										continue;
								if ((key = isolateWord(&start, &buf, length)) == NULL)
										continue;

								if (strcmp(key, "endscript") == 0) {
										state = STATE_DEFAULT;
										continue;
								} else {
								start = endtag = start - strlen(key);
								while (endtag - buf < length && *endtag != '\n')
										endtag++;
								if (endtag - buf > length)
										continue;
								*scriptDest = malloc(endtag - start + 1);
								if(*scriptDest == NULL)
									goto error;
								strncpy(*scriptDest, start,
												endtag - start);
								(*scriptDest)[endtag - start] = '\0';
								start = endtag;
								}
								break;

						case STATE_SKIP_LINE :
								if (*start == '\n')
										state = STATE_DEFAULT;
								else
										continue;
								break;

error:
								if (key)
										free(key);
								munmap(buf, (size_t) length);
								close(fd);
								return 1;

				}	
		}

	if(key)
		free(key);
	close(fd);
	return 0;
}

