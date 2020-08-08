/*
 * util.c
 *
 *  Created on: 2011. 9. 3.
 *      Author: yongwoo1.park
 */


#include <errno.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syslog.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <QtEndian>
#include <QDateTime>
#include "util.h"
//#include "protocol_common.h"

#define LOG_NAME "/log_data/log/RDS_AF_freq_event"

MMusUtil * MMusUtil::mmusUtil = NULL;

MMusUtil::MMusUtil()
{
	LOG_INIT_V2("DaemonMMus");
	first_time = 1;
	memset(&start, 0x00, sizeof(timeval));
	memset(&before, 0x00, sizeof(timeval));
}

MMusUtil::~MMusUtil()
{
	if(mmusUtil != NULL)
		delete mmusUtil;
}

/*!
 *	\brief make quint16 by combine two quint8
 */
quint16 MMusUtil::
gf_combine_quint8(quint8 a, quint8 b)
{
	quint16 a1 = a;
	quint16 b1 = b;
	return (a1 << 8) | b1;
}

void MMusUtil::print_tty(unsigned int level, const QString & msg)
{
	if(!msg.isNull()){
		if (level == CLogUtil::PRIORITY_INFO){
			LOG_HIGH << msg << LOG_ENDL;
		}
		else if (level == CLogUtil::PRIORITY_HI){
			LOG_HIGH << msg << LOG_ENDL;
		}
	}
}

void MMusUtil::gf_dbus_print_debug(const char * name, const QList<QVariant> & data){
#ifdef __DEBUG_MESSAGE
    QByteArray serialize;
    uchar dest[2];
    quint16 cmd = (quint16) data.at(0).toInt();
    quint16 data_size = (quint16) data.count() - 1;
    qToBigEndian(cmd, dest);
    serialize.append(dest[0]).append(dest[1]);
    qToBigEndian(data_size, dest);
    serialize.append(dest[0]).append(dest[1]);
    for (int i = 1; i < data.count(); ++i)
            serialize.append( (quint8)data.at(i).toInt());

    gf_print_debug(name, serialize);
#endif
}

/*!
 * \brief print QByteArray messages to console
 */
void MMusUtil::gf_print_debug(const char * name, const QByteArray & data){
#ifdef	__DEBUG_MESSAGE
    QString debugMsg;
    uint64_t secs, usecs;
    struct timeval current;
    char string[8192];

    debugMsg = QString(" ") + QString(name) + "(" + QString::number(data.count()) + ")" + QString(" : ");

    for (int i = 0; i < data.count(); ++i){
        QString str = QString::number( (unsigned char)data.at(i), 16);
        if ( str.length() == 1)
            str.prepend("0");
        debugMsg += str;
        debugMsg += " ";
    }


    if (first_time) {
		 first_time = 0;
		 gettimeofday(&start, NULL);
     }

    gettimeofday(&current, NULL);

	
	if ( current.tv_sec > start.tv_sec ){
		secs = current.tv_sec - start.tv_sec;
		while (current.tv_usec < start.tv_usec) {
			  secs --;
			  current.tv_usec += 1000000;
		}
		usecs = current.tv_usec - start.tv_usec;

		snprintf(string, 8192, "[%02llu.%06llu] [%d]", secs, usecs,
				  getpid());
	}
	else{
		snprintf(string, 8192, "[%s] [%d]", "unknown",
				  getpid());
	}
    debugMsg.prepend(string);
#ifdef _SIMULATION
    fflush(stdout);

    fprintf(stderr, qPrintable(debugMsg));
    fprintf(stderr, "\n");
    fflush(stderr);
#else
//                syslog(LOG_INFO|LOG_LOCAL0, qPrintable(debugMsg));
    if(data.at(0) == (char)0x83 && data.at(1) == (char)0x37)
		gf_print_to_file(CLogUtil::PRIORITY_INFO, debugMsg);
	else
		print_tty(CLogUtil::PRIORITY_INFO, debugMsg);
	
#endif  // End of _SIMULATION
#endif  // End of __DEBUG_MESSAGE
}

void MMusUtil::gf_print_to_file(unsigned int level, const QString & msg){
	int fd;
	int written;
	QString	log_text;
	QDateTime dateTime = QDateTime::currentDateTime();
	log_text.append(dateTime.toString("hh:mm:ss.zzz") + ": ");
	log_text.append(msg);

	fd = open(LOG_NAME, O_WRONLY | O_CREAT | O_APPEND, S_IWUSR|S_IRUSR);
	if (fd < 0) {
		goto fail;
	}
	
	written = write(fd, qPrintable(log_text), log_text.count());
	if ( written > 0)
		write(fd, "\n", 1);

	close(fd);

fail:
	print_tty(level, msg);
}

/*!
 * \brief write like print msg to console or syslog
*/
void MMusUtil::gf_err(int sys, const char * fmt, ...){
    va_list ap;

    va_start(ap, fmt);
    if ( sys)
        err_doit(1, CLogUtil::PRIORITY_HI, fmt, ap);
    else
        err_doit(0, CLogUtil::PRIORITY_HI, fmt, ap);
    va_end(ap);
}

void MMusUtil::gf_debug(const char * fmt, ...){
#ifdef __DEBUG_MESSAGE
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, CLogUtil::PRIORITY_INFO, fmt, ap);
    va_end(ap);
#endif
}



/*!
 * \brief write data buffer to file descriptor until (n) size is written
 */
ssize_t MMusUtil::
gf_writen(int fd, const void *vptr, size_t n)
{
	size_t          nleft;
	const char      *ptr;
	struct timeval	current, diff;
	uint64_t		diff_usec;
	int 			nwritten;

	memset(&current, 0, sizeof(current));
	memset(&diff, 0, sizeof(diff));
	gettimeofday(&current, NULL);

	/*current time - before time*/	
	timersub(&current, &before, &diff);
	if(diff.tv_sec == 0 && diff.tv_usec < 10000) {
		diff_usec = 10000 - diff.tv_usec;
		if(diff_usec > 10000){
			gf_debug("diff usec : %06luus, cmd no delay", diff_usec);
		} else {
			gf_debug("diff tv_usec : %06luus, cmd delay more %06luus", diff.tv_usec, diff_usec);
			usleep( diff_usec );
		}
	} else if(diff.tv_sec < 0) {
			gf_debug("cmd delay 10ms");
			usleep(10000);
	}

	ptr = (const char *) vptr;
	nleft = n;
	while (nleft > 0) {
		nwritten = write(fd, ptr, nleft);
			if ( nwritten < 0){
				//gf_debug("nwritten is minus, errno : %s", strerror(errno));
				if (errno == EINTR)
					nwritten = 0;	
				else
					return -1;
			}

			nleft -= nwritten;
			ptr   += nwritten;
	}
	
	before.tv_sec = current.tv_sec;
	before.tv_usec = current.tv_usec;

	return(n);
}

/*!
 * \brief Read "n" bytes from a descriptor
 */
ssize_t MMusUtil::
gf_readn(int fd, void *vptr, size_t n)
{
        size_t  nleft;
        ssize_t nread;
        char    *ptr;

        ptr = (char *)vptr;
        nleft = n;
        while (nleft > 0) {
                if ( (nread = read(fd, ptr, nleft)) < 0) {
                        if (errno == EINTR)
                                nread = 0;              /* and call read() again */
                        else
                                return(-1);
                } else if (nread == 0)
                        break;                          /* EOF */

                nleft -= nread;
                ptr   += nread;
        }
        return(n - nleft);              /* return >= 0 */
}

void MMusUtil::
err_doit(int errnoflag, unsigned int level, const char *fmt, va_list ap)
{
        int             errno_save, n;
        char            buf[256];

        errno_save = errno;             // value caller might want printed
        vsnprintf(buf, 256, fmt, ap);

        n = strlen(buf);
        if (errnoflag)
                snprintf(buf + n, 256 - n, ": %s", strerror(errno_save));

#ifdef _SIMULATION
        fflush(stdout);

        fprintf(stderr, buf);
	fprintf(stderr, "\n");
        fflush(stderr);
#else
        //syslog(level, buf);
		QString msg(buf);
        print_tty(level, msg);
#endif
}


