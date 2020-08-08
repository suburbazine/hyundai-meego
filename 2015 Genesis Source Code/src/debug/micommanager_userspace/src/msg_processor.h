/*
 * spi_msg_parser.h
 *
 *  Created on: 2011. 9. 3.
 *      Author: yongwoo1.park
 */

#ifndef MSG_PROCESSOR_H_
#define MSG_PROCESSOR_H_

#include <QObject>
#include <QtDBus>
#include "util.h"

typedef struct{
	QDBusMessage reply;
	quint16		 expectCmd;
    QDBusConnection * connection;
} SRequestQueue;

class ObjectAbstractInterface;
class CMsgProcessorPrivate;

class CMsgProcessor : public QObject{
	Q_OBJECT
public:
	static CMsgProcessor * instance(int fd = -1){
		if ( fd == -1 && msgProcessor){
			return msgProcessor;
		}
		else if ( fd != -1 && msgProcessor == NULL){
			msgProcessor = new CMsgProcessor(fd);
			return msgProcessor;
		}
		return NULL;
	}

private:
	static CMsgProcessor * msgProcessor;

private:
	CMsgProcessor(int fd, QObject * obj = 0);
	virtual ~CMsgProcessor();

private:
	int						fd;
	CMsgProcessorPrivate  * p;
    QList<ObjectAbstractInterface *>        objectList;
	MMusUtil			  * mmusUtil; 

#ifdef	IBOX_NOTIFY_FUNCTION
	QDBusInterface *		m_pVolumeMostInterface;
#endif

public:
	void					processDBusCall(const QDBusMessage & msg);
        void                                    processDBusCallNoResponse(quint16 cmd, const QDBusMessage &);
        void                                    processDBusCallWithResponse(quint16 cmd, const QDBusConnection & conn,
                                                                const QDBusMessage & reply, const QDBusMessage & msg = QDBusMessage());
        void                                    addObjects(ObjectAbstractInterface *);
//	void 					parseIncomingDBusCall(const QList<QVariant> &);

private slots:
	void					parseIncomingSpiData(const QByteArray &);
	void					outputDebug(const QByteArray &);

	friend class			CMsgProcessorPrivate;
};

#endif /* SPI_MSG_PARSER_H_ */
