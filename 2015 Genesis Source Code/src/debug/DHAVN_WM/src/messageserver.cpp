#include "messageserver.h"
#include <unistd.h>
#include <QStringList>
#include <QLocalSocket>
#include <QLocalServer>
#include "common.h"

#ifndef SERVER_SOCK_PATH
#define SERVER_SOCK_PATH	"/tmp/dhavn_wm_srv"
#endif

#include "dhavnlog.h"

MessageServer::MessageServer(QObject * obj)
    : QObject(obj){
    started = false;
    server = NULL;
    alreadySentAgree = false;
    languageStatus = false;
    tmpString="";
}

MessageServer::~MessageServer(){

}

void MessageServer::startServer(){
    if ( !started){
        server = new QLocalServer(this);
        unlink(SERVER_SOCK_PATH);
        server->listen(SERVER_SOCK_PATH);

        connect(server, SIGNAL(newConnection()),
                SLOT(connectionIncome()));

        started = true;
    }
}

void MessageServer::connectionIncome(){
    QLocalSocket * sock;
    while( (sock = server->nextPendingConnection()) != NULL){
        QObject::connect(sock, SIGNAL(disconnected()),
                         this, SLOT(removeSock()));
        connect(sock, SIGNAL(disconnected()),
                sock, SLOT(deleteLater()));
        connect(sock, SIGNAL(readyRead()), this, SLOT(readData()));
    }
}

void MessageServer::readData(){
    QLocalSocket * sock = qobject_cast<QLocalSocket *>( sender());
    QByteArray data = sock->readAll();
    QString string = QString::fromUtf16( (const ushort *)data.data(), data.count() / 2);
    string.replace("\\^", "\\wm_caret");
    string.replace("\\@", "\\wm_atsign");

    if ( !string.isEmpty() && string.startsWith(QChar(WM_STH))){
#ifdef	TRACE_SENDER
        lprintf("Command(%s) : %s", qPrintable(debugInfo[sock]), qPrintable(string));
#else
        //dmchoi_20140102 [ OSD 표시되는 구간에서 언어가 변경될 경우, OSD UI는 기존 언어 표시 형식을 따르고,
        //다음 번 표출 시에 변경된 언어에 맞게 UI를 변경하도록
       // lprintf("Command : %s", qPrintable(string));

        if(string.contains("@EVT:FR^LANG^")) {
            if(string.contains("@EVT:FR^LOCK")
               || string.contains("@EVT:F^LOCK")
               || string.contains("@EVT:R^LOCK")) {
                tmpString = "";
            } else {
                tmpString = string;
            }
            languageStatus = true;
        } else {
            languageStatus = false;
        }
        //dmchoi_20140102 ]

#endif
        if ( string.startsWith("@EVT^REGIST")){
            sendCurrentStatus(sock);
            clientList.append(sock);
#ifdef	TRACE_SENDER	
            QStringList tokens = string.split("^");
            if ( tokens.count() == 3){
                debugInfo[sock] = tokens.at(2);
            }
#endif
        }
        //dmchoi_20131202 [ITS : 0212019, 0212023 내용이 빈 OSD가 출력되는 issue
        if(string == "@OSD:F^COMMON^INFO^" || string == "@OSD:R^COMMON^INFO^") {
            return;
        } else {
            //dmchoi_20140102 [ OSD 표시되는 구간에서 언어가 변경될 경우, OSD UI는 기존 언어 표시 형식을 따르고,
            //다음 번 표출 시에 변경된 언어에 맞게 UI를 변경하도록
            if(languageStatus == false) {
               if(tmpString != "") {
                    emit messageArrived(tmpString);
                    tmpString = "";
               }
               emit messageArrived(string);
            }
            //dmchoi_20140102 ]
        }
    }
}


void MessageServer::sendAgreed(){
    if ( !alreadySentAgree ){
        alreadySentAgree = true;
    }

    sendBroadcastMessage("@AGREED");
}

void MessageServer::sendBackAgreed(){
    // Assume this method cannot be called at first time
    alreadySentAgree = false;
    sendBroadcastMessage("@BACKAGREE");
}

void MessageServer::sendCurrentStatus(QLocalSocket * sock){
    int index = 0;

    while( g_status_table[index].command != NULL){
        if ( g_status_table[index].show){
            QString args;
            args.append("@STA^");
            args.append(g_status_table[index].command);
            args.append("^");

            if ( g_status_table[index].argc == 2){
                args.append("1");
            }
            else {	// argc == 3
                args.append("1^");
                args.append(QString::number(g_status_table[index].value));
            }


            QByteArray array( (const char *) (args.utf16()), args.size() * 2);
            sock->write(array);
            sock->flush();
        }

        index++;
    }

    if ( alreadySentAgree ){
        QString agreeStr = "@AGREED";
        QByteArray array( (const char *) (agreeStr.utf16()), agreeStr.size() * 2);
        sock->write(array);
        sock->flush();
    }
}

void MessageServer::removeSock(){
    QLocalSocket * sock = qobject_cast<QLocalSocket *>( sender());
    if ( clientList.contains(sock)){
        clientList.removeAll(sock);
        lprintf("Sock removed successfully(%d)", clientList.count());
    }
    else{
        lprintf("Cannot find sock(%d)", clientList.count());
    }
}

void MessageServer::sendBroadcastMessage(const QString & msg){
    QLocalSocket * sock = NULL;
    for (int i = 0;	i < clientList.count(); ++i){
        sock = clientList.at(i);

        QByteArray array( (const char*)(msg.utf16()), msg.size() * 2);
        sock->write(array);
        sock->flush();
    }

    lprintf("Write Broadcast Message(%d)", clientList.count());
}
