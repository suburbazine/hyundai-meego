#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

#include "loginfo.h"
#include "logrotate.h"

#define CONFIG_FILE			"/etc/logrotate.d/DH_logrotate"
#define BACKUP_PATH			"/log_data/backup/"
#define ROTATE_MANAGET_FILE	"/log_data/backup/rot_manager"
#define TMP_ROTATE_MANAGET_FILE	"/log_data/backup/tmp_rot_manager"

#define LOTATE_LOG(a, ...) syslog(LOG_INFO, a, ##__VA_ARGS__)

extern int numLogs;
extern struct logInfo ** logs;

static void free_array(struct logInfo **array, int lines_count)
{
	int i;
	for (i = 0; i < lines_count; ++i)
		free(array[i]);
	free(array);
}

static int runScript(char *script)
{
	int rc;

	if (!fork()) {
		execl("/bin/sh", "sh", "-c", script, NULL);
		exit(1);
	}

	wait(&rc);
	return rc;
}

static int file_validation(void)
{
	FILE *fp, *tmp_fp;
	char tmp_file[1024] = {0x00, };
	
	fp = fopen(ROTATE_MANAGET_FILE, "a+t");
	if(fp == NULL)
		return 1;
	
	tmp_fp = fopen(TMP_ROTATE_MANAGET_FILE, "a+t");
	if(tmp_fp == NULL){
		fclose(fp);
		return 1;
	}
	
	fseek(fp, SEEK_SET, 0);

	while(fgets(tmp_file, sizeof(tmp_file), fp) != NULL){
		char only_file_name[1024] = {0x00, };
		strncpy(only_file_name, tmp_file, strlen(tmp_file) - 1);

		if(access(only_file_name, F_OK) < 0){
			continue;
		}

		fprintf(tmp_fp, "%s", tmp_file);
	}

	if(rename(TMP_ROTATE_MANAGET_FILE, ROTATE_MANAGET_FILE) < 0){
		fclose(fp);
		fclose(tmp_fp);
		return 1;
	}

	fclose(fp);
	fclose(tmp_fp);
	return 0;
}

static int managed_rot_num(unsigned char *file_name, int rotNum)
{
	FILE *fp, *tmp_fp;
	int cnt = 0;
	char tmp_file[1024] = {0x00, };
	char first_file[1024] = {0x00, };
	
	fp = fopen(ROTATE_MANAGET_FILE, "a+t");
	if(fp == NULL)
		return 1;
	
	tmp_fp = fopen(TMP_ROTATE_MANAGET_FILE, "a+t");
	if(tmp_fp == NULL){
		fclose(fp);
		return 1;
	}

	fprintf(fp, "%s\n", file_name);
	
	fseek(fp, SEEK_SET, 0);

	while(fgets(tmp_file, sizeof(tmp_file), fp) != NULL){
		if(cnt == 0)
			strncpy(first_file, tmp_file, strlen(tmp_file) - 1);
		else
			fprintf(tmp_fp, "%s", tmp_file);
		cnt++;
	}

	if(cnt <= rotNum){
		unlink(TMP_ROTATE_MANAGET_FILE);

		fclose(fp);
		fclose(tmp_fp);
		return 0;
	}

	if(first_file[0] != 0x00)
		unlink(first_file);

	if(rename(TMP_ROTATE_MANAGET_FILE, ROTATE_MANAGET_FILE) < 0){
		fclose(fp);
		fclose(tmp_fp);
		return 1;
	}

	fclose(fp);
	fclose(tmp_fp);
	return 0;
}

static int copyTruncate(char *currLog, char *saveLog)
{
	char buf[BUFSIZ];			/* defined BUFSIZ?? */
	int fdcurr = -1, fdsave = -1;
	ssize_t cnt;
	struct stat curr;

	if (stat(currLog, &curr))
		return 1;

	fdsave = open(saveLog, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fdsave < 0) {
		return 1;
	}
	if (fchmod(fdsave, 0666)) {
		close(fdsave);
		return 1;
	}
	if (fchown(fdsave, curr.st_uid, curr.st_gid)) {
		close(fdsave);
		return 1;
	}

	if ((fdcurr = open(currLog, O_RDWR)) < 0) {
		close(fdsave);
		return 1;
	}

	while ((cnt = read(fdcurr, buf, sizeof(buf))) > 0) {
		if (write(fdsave, buf, cnt) != cnt) {

			close(fdcurr);
			close(fdsave);
			return 1;
		}
	}
	if (cnt != 0) {				/* failed save current log to new log */
		close(fdcurr);
		close(fdsave);
		return 1;
	}

	if (ftruncate(fdcurr, 0)) { /* if success, return 0 */
		close(fdcurr);
		close(fdsave);
		return 1;
	}

	if (fdcurr >= 0)
		close(fdcurr);

	if (fdsave >= 0) 
		close(fdsave);

	return 0;
}

static int compressLogFile(struct logInfo * log, char *filename, char *rotName)
{
	unsigned char *compressedName;
	char *fullCommand[5];
	int status;

	compressedName = alloca(strlen(rotName) + strlen(log->compress_ext) + 1);
	snprintf(compressedName, strlen(rotName) + strlen(log->compress_ext) + 1, 
			"%s%s", rotName, log->compress_ext);
	//printf("check compressedName : %s\n", compressedName);

	fullCommand[0] = log->compress_prog;
	fullCommand[1] = log->compress_options_list;
	fullCommand[log->compress_options_count + 1] = compressedName;
	fullCommand[log->compress_options_count + 2] = rotName;
	fullCommand[log->compress_options_count + 3] = NULL;
	
	file_validation();
	managed_rot_num(compressedName, log->rotateCount);

	if (copyTruncate(filename, rotName)) {
		return 1;
	}
	if (!fork()) {
		execvp(fullCommand[0], (void *) fullCommand);
		exit(1);
	}

	wait(&status);
	if (chmod(compressedName, 0666)) {
		return 1;
	}
	if(unlink(rotName))
		return 1;

	return 0;
}

static void get_rotLogName(struct logInfo *log, char *fileName, char **rotName, char *dirName) 
{
	struct timeval current_t;
	struct tm *current_tm;
	struct tm *past_tm;
	struct stat file_info;
	char current[5] = {0x00, };

	past_tm = localtime(&past_t.tv_sec);
	snprintf(*rotName, strlen(dirName)+strlen(fileName)+ 16,
			"%s%s.%04d%02d%02d_%02d%02d_", dirName, fileName,
								past_tm->tm_year + 1900,
								past_tm->tm_mon + 1, past_tm->tm_mday,
								past_tm->tm_hour, past_tm->tm_min);

	gettimeofday(&current_t, NULL);
	current_tm = localtime(&current_t.tv_sec);

	snprintf(current, 5, "%02d%02d", current_tm->tm_hour, current_tm->tm_min);
	strncat(*rotName, current, strlen(current));
	
	gettimeofday(&past_t, NULL);
	//printf("check rot name : %s\n", rotName);
}

int check_backup_file_count(struct logInfo *log) 
{
	DIR *dp;
	//struct dirent *entry;
	unsigned int sum = 0;

	dp = opendir(BACKUP_PATH);
	if(dp == NULL) 
		return 0;

	while(readdir(dp) != NULL) {
		sum += 1; 
	}
	closedir(dp);
	return sum;
}

void backup_file_remove(void)
{
	DIR *dp;
	struct dirent *entry;
	struct stat info;
	char buf[1024];

	dp = opendir(BACKUP_PATH);
	if(dp == NULL) 
		return;

	if(getcwd(buf, sizeof(buf)) == NULL) {
		closedir(dp);
		return;
	}

	LOTATE_LOG("remove backup file");
	while((entry = readdir(dp)) != NULL) {
		chdir(BACKUP_PATH);

		unlink((char *)entry->d_name);
		//if(unlink((char *)entry->d_name) < 0)
		//	LOTATE_LOG("Can not remove file name : %s", (char *)entry->d_name);
	}

	chdir(buf);
	closedir(dp);
}

int rotateLogs (struct logInfo *log) {
	int backup_file_total_count, i;
	long size;
	char *fileName, *rotName;
	char *tmp_fileName = NULL;
	char *dirName = BACKUP_PATH;
	struct stat fileInfo;

	if(log->pre)
		runScript(log->pre);

	/* check partition usage */
	backup_file_total_count = check_backup_file_count(log);
	if(backup_file_total_count > (log->rotateCount + 2 + 1)) 
		backup_file_remove();

	for(i = 0 ; i < log->numFiles ; i++) {
		if (stat (log->files[i], &fileInfo) == -1)
			continue;

		size = fileInfo.st_size;

		/*get fileName without dirName*/	
		fileName = alloca(strlen(log->files[i]) + 1);
		strncpy(fileName, log->files[i], strlen(log->files[i]));
		fileName[strlen(log->files[i])] = '\0';

		fileName = strtok(fileName, "/");
		while(fileName) {
			fileName = strtok(NULL, "/");
			if(fileName != NULL) 
				tmp_fileName = fileName;	
		}

		if(tmp_fileName != NULL)
			fileName = tmp_fileName;

		if(fileName == NULL)
			return 1;

		/*rotate or compress by size*/
		if (size > log->rotateSize){
			rotName = alloca(strlen(dirName) + strlen(fileName) + 32);
			get_rotLogName(log, fileName, &rotName, dirName);

			if( log->flags & LOG_FLAG_COMPRESS ){
				if(compressLogFile(log, (log->files[i]), rotName))
					return 1;
			}
#if 0
			else {
				if(copyTruncate(log->files[i], rotName)){
					return 1;
				}
			}
#endif
		}
	}

	if(log->post)
		runScript(log->post);	

	return 0;
}

int main (int argc, char ** argv) 
{
	int i, j;

	if (daemon(1, 1))
		return 1;

	
	gettimeofday(&past_t, NULL);

	if (readConfigFile(CONFIG_FILE)) {
		free_array(logs, numLogs);
		return 1;
	}

	while(1) {
		for(i = 0 ; i < numLogs ; i++) {
			if(rotateLogs (logs[i])) 
				continue;
		}
		sleep(logs[0]->rotatePeriod);
	}

	free_array(logs, numLogs);
	return 0;
}


