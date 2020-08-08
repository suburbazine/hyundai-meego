#include "messageserver.h"
#include <unistd.h>

#ifndef SERVER_SOCK_PATH
#define SERVER_SOCK_PATH	"/tmp/upgrade_msg_srv"
#endif

MessageServer::MessageServer(QObject * obj)
: QObject(obj){
	started = false;
	mapper = NULL;
	server = NULL;
}

MessageServer::~MessageServer(){

}

void MessageServer::startServer(){
	if ( !started){
		server = new QLocalServer(this);
		unlink(SERVER_SOCK_PATH);
		server->listen(SERVER_SOCK_PATH);
		mapper = new QSignalMapper(this);

		connect(server, SIGNAL(newConnection()),
				SLOT(connectionIncome()));

		connect(mapper, SIGNAL(mapped(QObject *)),
			this, SLOT(readData(QObject *)));
		started = true;
	}
}

void MessageServer::connectionIncome(){
	QLocalSocket * sock;
	while( (sock = server->nextPendingConnection()) != NULL){
		connect(sock, SIGNAL(disconnected()),
			sock, SLOT(deleteLater()));
		connect(sock, SIGNAL(readyRead()), mapper, SLOT(map()));
		mapper->setMapping(sock, sock);
	}
}

void MessageServer::readData(QObject * obj){
	QLocalSocket * sock = qobject_cast<QLocalSocket *>(obj);
	QByteArray data = sock->readAll();
	if ( data.count() > 3){
		int moduleID = (int)data.at(0);
		int moduleProgress = (int)data.at(1);
		int moduleStatus = (int)data.at(2);
		data.remove(0, 3);
		emit messageArrived(moduleID, moduleProgress, moduleStatus, QString(data));
	}
}
