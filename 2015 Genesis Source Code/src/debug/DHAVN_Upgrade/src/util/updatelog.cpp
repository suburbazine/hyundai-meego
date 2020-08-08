#include "updatelog.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <QDateTime>
#include "common.h"
#include <QMutex>

UpdateLog * UpdateLog::updateLog = NULL;

UpdateLog::UpdateLog(){
	opened = true;
#ifndef	CONSOLE_DEBUG
	openLog();
#endif
}

QMutex mutex;

void UpdateLog::openLog(){
	const char * title = "---------- UpdateLog ---------";
        fd = open(UPGRADE_LOG_FILE, O_WRONLY | O_CREAT | O_SYNC | O_APPEND, S_IWUSR|S_IRUSR|S_IROTH|S_IRGRP);
	if ( fd < 0)
		opened = false;	
	else{
		if ( fcntl(fd, F_SETFD, FD_CLOEXEC) != -1){
			log(title);
			if ( g_isReadonly)
				log("ReadOnly Partition");
			else
				log("ReadWrite Partition");
		}
                int result = ::chmod(UPGRADE_LOG_FILE, 00755);  // SANGWOO_TEMP
                Q_UNUSED(result);	// do not care about result
	}
}

UpdateLog::~UpdateLog(){
	if ( fd > 0 )
		::close(fd);	
}

void UpdateLog::closeLog(){
#ifndef	CONSOLE_DEBUG
	if ( !opened)
		return;

	fflush(NULL);

	if ( fd > 0 )
		::close(fd);	

	opened = false;
#endif
}



char buf[UPGRADE_LOG_BUF_SIZE];


void UpdateLog::log(const char * fmt, ...){
	if ( !opened){
		openLog();
		
		if ( !opened)
			return;
	}

	va_list ap;
	
       QString prefix;
       prefix.reserve(1024);
        
        try {
            prefix = getCurrentDate();
        }
        catch (...) {
            prefix = "[time:no data]";
        }

	snprintf(buf, UPGRADE_LOG_BUF_SIZE - 1, qPrintable(prefix));

	va_start(ap, fmt);

	vsnprintf(buf + prefix.count(), UPGRADE_LOG_BUF_SIZE - prefix.count(), fmt, ap);

	va_end(ap);

#ifndef	CONSOLE_DEBUG
	writen(fd, (const void *)buf, strlen(buf));
	writen(fd, (const void *)"\n", 1);
#else
	writen(1, (const void *)buf, strlen(buf));
	writen(1, (const void *)"\n", 1);
#endif
        prefix.squeeze();
}

void UpdateLog::exception_log(const char * file, const char * func, int line){
	if ( !opened){
		openLog();

		if ( !opened )
			return;
	}

	char buf[UPGRADE_LOG_BUF_SIZE];
	QString prefix = getCurrentDate();
	snprintf(buf, UPGRADE_LOG_BUF_SIZE - 1, "Exception : %s:%s(%d)", file, func, line);
	prefix.append(buf);	
#ifndef	CONSOLE_DEBUG
	writen(fd, (const void *)qPrintable(prefix), prefix.length());
	writen(fd, (const void *)"\n", 1);
#else
	writen(1, (const void *)qPrintable(prefix), prefix.length());
	writen(1, (const void *)"\n", 1);
#endif
}

QString UpdateLog::getCurrentDate() const{

        
        QString str;
        str.reserve(1024);
	str.append("[");
//	str.append(QDateTime::currentDateTime().toString("MM-dd-yy hh:mm:ss"));

        QDateTime time = QDateTime::currentDateTime();

        if (!time.isNull() && time.isValid())
            str.append(time.toString("MM-dd-yy hh:mm:ss"));

	str.append("] ");

     
	return str;
}

ssize_t
UpdateLog::writen(int fd, const void *vptr, size_t n)
{
	size_t          nleft, nwritten;
	const char      *ptr;

	ptr = (const char *) vptr;
	nleft = n;
	while (nleft > 0) {
			if ( (nwritten = write(fd, ptr, nleft)) <= 0){
					opened = false;
					return(nwritten);               // error
			}

			nleft -= nwritten;
			ptr   += nwritten;
	}
	
	return(n);
}
