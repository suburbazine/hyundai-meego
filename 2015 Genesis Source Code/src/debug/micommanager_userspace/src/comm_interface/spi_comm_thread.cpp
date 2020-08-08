/*
 * CSpiCommThread.cpp
 *
 *  Created on: 2011. 9. 3.
 *      Author: yongwoo1.park
 */

#include "spi_comm_thread.h"
#include "protocol_common.h"
#include <stdio.h>
#include <errno.h>
#include <syslog.h>

#define SPI_BUF_SIZE	2048

CSpiCommThread::CSpiCommThread(int fd, QObject * obj)
: QThread(obj), fd(fd){
}

CSpiCommThread::~CSpiCommThread() {}

/*!
 * \brief Receive datas from spi device driver
 *
 * Receive datas from spi device driver and store in buf and emit signal
 */
void CSpiCommThread::run(){
    QByteArray buf;
    char data[SPI_BUF_SIZE];
    int result, read_size;

    openlog("MMus",LOG_PID,LOG_LOCAL0);

    while(true){
		read_size = read(fd, data, SPI_BUF_SIZE);	// block call
/*
		QByteArray debug_data;
		for (int q = 0; q < read_size; q++)
			debug_data.append( (quint8) data[q]);
*/
		if ( read_size < 0)
			if ( errno == EINTR)
				continue;

		if ( read_size <= 0){
			//mmusUtil->gf_err( read_size < 0 ?SYS_ERR:MY_ERR, "(FATAL)SPI Read Thread Broken\n");
			// use syslog directly..
			syslog(LOG_CRIT, "(FATAL)SPI Read Thread Broken");
			break;
		}
		buf.append(data, read_size);
//		mmusUtil->gf_print_debug("READ RAW", buf);
		emit printDebug(buf);

		result = tokenize(buf);
		if ( result > 0){
			emit dataArrived(buf.left(result));
			buf.remove(0, result);
		}
	}

	closelog();
}

int CSpiCommThread::tokenize(const QByteArray & data){
        // TODO: how do i know message is broken?(check cmd and data_len)
	if ( data.count() >= 4 ){
		quint16 a = (quint8) data.at(2);
		quint16 b = (quint8) data.at(3);
		quint16 len = (a << 8) | b;
		if ( data.count() >= 4 + len)
			return len + 4;
	}
	return 0;
}
