/*
 * spi_msg_parser.cpp
 *
 *  Created on: 2011. 9. 3.
 *      Author: yongwoo1.park@lge.com
 */

#include <QVariant>
#include <QtEndian>
#include <QtDBus>
#include "msg_processor.h"
#include "protocol_common.h"
#include "object_abstractinterface.h"

CMsgProcessor * CMsgProcessor::msgProcessor = NULL;


class CMsgProcessorPrivate{
public:
    CMsgProcessorPrivate(CMsgProcessor * d);
    ~CMsgProcessorPrivate();

    void    writeToMicom(quint16 cmd, const QList<QVariant> &, const QDBusMessage & msg);
    void    writeToMicom(quint16 cmd, const QList<QVariant> &);
    void    writeToMicom(quint16 cmd, const QDBusConnection & conn,
                              const QDBusMessage & reply, const QList<QVariant> & args);
    void    writeToDBus(quint16 cmd, const QByteArray &);
    void		writeToSPI(quint16 cmd, const QList<QVariant> &);
private:
    inline void		writeToSPI(const QList<QVariant> &);
    inline int			validateMessage(quint16, int argCount);
    inline SRequestQueue *	getRequestFromQueue(quint16 cmd);
    inline void                 removeRequestFromQueue(SRequestQueue * s){
        m_queue.removeOne(s);
    }

    inline int			getIndexOfCommand(quint16 cmd){
        int index = 0;
        while(true){
            if ( g_CmdTable[index].command == 0)
                    break;
            else if ( g_CmdTable[index].command == cmd)
                    return index;
            index++;
        }
        //gf_err(MY_ERR, "Cannot find registered command(CMsgProcessorPrivate::getIndexOfCommand) : %04X\n", cmd);
        return -1;
    }
    void				append(SRequestQueue *);

private:
	CMsgProcessor * d;
	MMusUtil	  * mmusUtil;
	QList<SRequestQueue *> m_queue;
};


CMsgProcessorPrivate::CMsgProcessorPrivate(CMsgProcessor * d)
: d(d){
	mmusUtil = MMusUtil::instance();
}

CMsgProcessorPrivate::~CMsgProcessorPrivate(){}



/*!
 * \brief validate messages which come from SPI
 *
 * return value : index of g_CmdTable, if failed return -1
 */
int CMsgProcessorPrivate::validateMessage(quint16 cmd, int argCount){
	Q_UNUSED(argCount);
    int index = getIndexOfCommand(cmd);

    if ( index >= 0){
#ifdef	__DEBUG_MESSAGE
        printf("Command : %s\n", GET_COMMAND_NAME(index));
#endif
        return index;
    }

//    gf_err(MY_ERR, "Validate Failed : index(%d), data_size(expect : %d, but %d)\n", index, arg_count, argCount);

    return -1;	//fail
}

SRequestQueue * CMsgProcessorPrivate::getRequestFromQueue(quint16 cmd){
    SRequestQueue * q = NULL;
    for (int i = 0; i < m_queue.count(); ++i){
        q = m_queue.at(i);
        if ( q->expectCmd == cmd )
             return m_queue.at(i);
    }
    return NULL;
}

void CMsgProcessorPrivate::writeToSPI(const QList<QVariant> & data){
    quint16 cmd = (quint16) data.at(0).toInt();
    QList<QVariant> d;
    d.append(data);
    d.removeFirst();

    writeToSPI(cmd, d);
}

void CMsgProcessorPrivate::writeToSPI(quint16 cmd, const QList<QVariant> & data){
    uchar dest[2];
    QByteArray transferStream;

    quint16 data_size = (quint16) data.count();

    qToBigEndian(cmd, dest);
    transferStream.append(dest[0]).append(dest[1]);

    qToBigEndian(data_size, dest);
    transferStream.append(dest[0]).append(dest[1]);

    for (int i = 0; i < data.count(); ++i)
        transferStream.append( (quint8) data.at(i).toInt());

    mmusUtil->gf_print_debug("MW to Micom", transferStream);
    int written = mmusUtil->gf_writen(d->fd, transferStream.data(), transferStream.count());

    if ( written != transferStream.count())
         mmusUtil->gf_err(MY_ERR, "Write to micom failed(expect %d, but %d\n", transferStream.count(), written);
}

/*!
 * \brief Send Datas to appropiate DBus
 */
void CMsgProcessorPrivate::writeToDBus(quint16 cmd, const QByteArray & data){
    QList<QVariant> returned_list;
    int index = getIndexOfCommand(cmd);

    //mmusUtil->gf_debug("SRequestQueue : %d", m_queue.count()); 
    if ( index >= 0){
        SRequestQueue * q = getRequestFromQueue(cmd);
		QByteArray array;	
    	returned_list.append( (int)cmd);
		if ( data.count() > 250){
			for (int i = 4; i < data.count(); ++i)
				array.append( data.at(i));
			returned_list.append( array);
		}
		else{
			for (int i = 4; i < data.count(); ++i)
					returned_list.append( (int) data.at(i));
		}

        if ( q ){
            removeRequestFromQueue(q);
            //mmusUtil->gf_dbus_print_debug("MW to DBUs(Response)", returned_list);
            q->reply.setArguments(returned_list);
            if ( q->connection )
                q->connection->send(q->reply);
            else
                QDBusConnection::sessionBus().send(q->reply);

            if ( q->connection )
                delete q->connection;

            delete q;
        }
        else if ( IS_EVENT_MESSAGE(cmd) || cmd == 0x8610 || cmd == 0x8613 || cmd == 0X8620 ){
            QDBusMessage signal;
            //mmusUtil->gf_dbus_print_debug("MW to DBUs(Event)", returned_list);

            signal = QDBusMessage::createSignal("/",
                            GET_INTERFACE_NAME(index), GET_METHOD_NAME(index) );
            signal.setArguments(returned_list);
            QDBusConnection::sessionBus().send(signal);
        }
    }
    else{
	/*for( int i=0; i<m_queue.count(); i++ )
		gf_debug("cmd : 0X%llu", m_queue.at(i)->expectCmd);*/	
        SRequestQueue * q = getRequestFromQueue(cmd);
        if ( q ){
            removeRequestFromQueue(q);
    		returned_list.append( (int)cmd);
			if ( data.count()  > 4){
					QByteArray ar;
					for (int i = 4; i < data.count(); ++i)
						ar.append( (uchar) data.at(i));
					q->reply << ar;
			}
/*
            if ( data.count() - 4 > 1){
                QByteArray ar;
                for (int i = 4; i < data.count(); ++i)
                    ar.append( (uchar) data.at(i));
                q->reply << ar;
            }
            else if(data.count() - 4 == 1){
                uchar oneData;
                oneData = (uchar) data.at(4);
                q->reply << oneData;
            }
*/
			for (int i = 4; i < data.count(); ++i)
				returned_list.append( (int) data.at(i));

            //mmusUtil->gf_dbus_print_debug("MW to DBUs(Response)", returned_list);
            
			if ( q->connection ){
                q->connection->send(q->reply);
                delete q->connection;
            }
            else{
                mmusUtil->gf_err(MY_ERR, "WriteToDBus Failed : connection is not exist");
            }
            delete q;
        }
        else{
            mmusUtil->gf_err(MY_ERR, "Cannot find registered command : %04X\n", cmd);
        }
    }
}


/*!
 * \brief Send QByteArray datas to Micom
 */
void CMsgProcessorPrivate::writeToMicom(quint16 cmd, const QList<QVariant> & args, const QDBusMessage & msg){
    int index = validateMessage(cmd, args.count() - 1);

    if ( index >= 0){
        if (NEED_RESPONSE(index)){
            msg.setDelayedReply(true);
            SRequestQueue * q = new SRequestQueue;
            q->reply = msg.createReply();
	    //QDBusConnection::sessionBus().send(q->reply);
            q->connection = NULL;
            q->expectCmd = cmd | 0x8000;
            m_queue.append(q);
        }
        writeToSPI(args);
    }
}

void CMsgProcessorPrivate::writeToMicom(quint16 cmd, const QDBusConnection & conn,
                                        const QDBusMessage & reply, const QList<QVariant> & args){
    SRequestQueue * q = new SRequestQueue;
    q->reply = reply;
    q->expectCmd = cmd | 0x8000;
    q->connection = new QDBusConnection(conn);
    m_queue.append(q);

    writeToMicom(cmd, args);
}

void CMsgProcessorPrivate::writeToMicom(quint16 cmd, const QList<QVariant> & args){
    writeToSPI(cmd, args);
}


CMsgProcessor::CMsgProcessor(int fd, QObject * obj)
: QObject(obj), fd(fd){
	p = new CMsgProcessorPrivate(this);
	mmusUtil = MMusUtil::instance();
#ifdef	IBOX_NOTIFY_FUNCTION
	m_pVolumeMostInterface = new QDBusInterface("com.dhavn.mostmanager.qtdbus", "/hmi", "", QDBusConnection::sessionBus());
#endif
}

CMsgProcessor::~CMsgProcessor() {
#ifdef	IBOX_NOTIFY_FUNCTION
	if ( m_pVolumeMostInterface )
		delete m_pVolumeMostInterface;	
#endif
}

/*!
 * \brief parse datas which come from SPI Interface
 *
 *	parsing incomed SPI data and send to appropriate DBus Receiver(Other Application)
 */
void CMsgProcessor::parseIncomingSpiData(const QByteArray & data){
    if ( data.count() >= 4){	// assume that (command + data_size)data is exist
        quint16 cmd = mmusUtil->gf_combine_quint8(data.at(0), data.at(1));
        QByteArray onlyData = data;
        onlyData.remove(0, 4);

        mmusUtil->gf_print_debug("MICOM to MW", data);

#ifdef	IBOX_NOTIFY_FUNCTION
//	here handles IBox Message
		if ( cmd == (quint16) 0x8310 && onlyData.count() == 2){
			QDBusReply<bool> reply;
			if ( m_pVolumeMostInterface->isValid()){
				QList<QVariant> argumentList;
				uint sourceType;
				
				switch(onlyData.at(0)){
				case 0x00:
				case 0x06:
				case 0x07:
				case 0x08:
					sourceType = 0x01;
					break;
				case 0x01:
					sourceType = 0x02;
					break;
				case 0x02:
				case 0x03:
				case 0x09:
					sourceType = 0x04;
					break;
				default:
					sourceType = 0x03;
				}

				argumentList << (const uint) sourceType << (const uint) 0x0F << (const uint) onlyData.at(1);
				reply = m_pVolumeMostInterface->asyncCallWithArgumentList(QLatin1String("SetVolumeStatus"), argumentList);
			}	
		}
#endif

        for (int i = 0; i < objectList.count(); ++i){
            if (objectList.at(i)->handleEvent(cmd, onlyData))
                return;
        }

        p->writeToDBus(cmd, data);
    }
    else{
        mmusUtil->gf_err(MY_ERR, "SPI data size is too short(%d)\n", data.count());
    }
}


/*!
 * \brief parse datas which come from DBus for micom_manager_table
 */
void CMsgProcessor::processDBusCall(const QDBusMessage & msg){
        const QList<QVariant> & args = msg.arguments();

	if ( args.count()){ // larger than CMD
                quint16 cmd = (quint16) args.at(0).toInt();
                //mmusUtil->gf_dbus_print_debug("DBUS to MW", args);
		p->writeToMicom(cmd, args, msg);
	}
	else{
		mmusUtil->gf_err(MY_ERR, "ERROR: Empty DBus argument is arrived\n");
	}
}

void CMsgProcessor::outputDebug(const QByteArray & data){
	//mmusUtil->gf_print_debug("READ RAW", data);
}

/*!
  \brief parse datas which come from DBus and need to response to Application
*/
void CMsgProcessor::processDBusCallWithResponse(quint16 cmd, const QDBusConnection & conn,
                                    const QDBusMessage & reply, const QDBusMessage & msg){
 
	p->writeToMicom(cmd, conn, reply, msg.arguments());
}
/*
    \brief parse datas which come from DBus and don't need to response
*/
void CMsgProcessor::processDBusCallNoResponse(quint16 cmd, const QDBusMessage & msg){
    p->writeToMicom(cmd, msg.arguments());
}

void CMsgProcessor::addObjects(ObjectAbstractInterface * oai){
    objectList.append(oai);
}
