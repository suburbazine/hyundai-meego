#include "messageprocess.h"
#include "log_console.h"

#ifndef	SERVER_SOCK_PATH
#define SERVER_SOCK_PATH "/tmp/dhavn_wm_srv"
#endif

MessageProcess * MessageProcess::messageProcess = NULL;
QString MessageProcess::applicationName = "UNKNOWN";

MessageProcess::MessageProcess(QObject * obj)
: QObject(obj){
	m_sock = new QLocalSocket(this);
	m_regist = m_agreed = false;

	connect(m_sock, SIGNAL(readyRead()), SLOT(readData()));
	connect(m_sock, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), SLOT(checkState(QLocalSocket::LocalSocketState)));

	connectToServer();
}

void MessageProcess::checkState(QLocalSocket::LocalSocketState state){
	static QLocalSocket::LocalSocketState previousState = QLocalSocket::UnconnectedState;
	if ( previousState != state && (state == QLocalSocket::UnconnectedState || state == QLocalSocket::ClosingState)){
#ifdef	CONSOLE_DEBUG
                lprintf("[StatusBar] checkState : %s", state);
#endif
		previousState = state;
		connectToServer();
	}
}

bool MessageProcess::connectToServer(){
	if ( m_sock->state() == QLocalSocket::UnconnectedState ||
		m_sock->state() == QLocalSocket::ClosingState){
#ifdef	CONSOLE_DEBUG
                lprintf("[StatusBar] connectToServer");
#endif
		m_sock->connectToServer(SERVER_SOCK_PATH);
	}
	else if ( m_sock->state() == QLocalSocket::ConnectedState){
		return true;
	}
	
	return m_sock->waitForConnected();
}

bool MessageProcess::writeMessage(const QString & msg){
	if ( !connectToServer())
		return false;

#ifdef	CONSOLE_DEBUG
         lprintf("[StatusBar] write Message : %s", qPrintable(msg));
#endif
	QByteArray array( (const char *) (msg.utf16()), msg.size() * 2);
	int writen = m_sock->write(array);
	if ( writen == array.count()){
		m_sock->flush();
		return true;
	}

	return false;
}

void MessageProcess::registApp(){
	if ( !m_regist ){
#ifdef	CONSOLE_DEBUG
                lprintf("[StatusBar] try to regist App");
#endif
		if (writeMessage( QString("@EVT^REGIST^") + MessageProcess::applicationName))
			m_regist = true;
	}
}

#define WM_STH "@"
#define WM_US	"^"

void MessageProcess::readData(){
	QByteArray data = m_sock->readAll();
	QString str = QString::fromUtf16( (const ushort *)data.data(), data.count() / 2);

#ifdef	CONSOLE_DEBUG
        lprintf("[StatusBar] received Message : %s", qPrintable(str));
#endif
	if ( !str.isEmpty() && str.startsWith(WM_STH)){
		if ( str.count(WM_STH)  >= 1){
			QStringList commandList = str.split(WM_STH);
			for (int i = 0; i < commandList.count(); ++i){
				if ( !commandList.at(i).isEmpty()){
					QString command = commandList.at(i);
					QStringList list = command.split(WM_US);
					if ( list.at(0).startsWith("STA")){
						list.removeFirst();
						emit statusCommandReceived(list);
					}
					else if ( list.at(0).startsWith("AGREED")){
						emit agreed();
						m_agreed = true;
					}
					else if ( list.at(0).startsWith("BACKAGREE")){
						emit backToAgree();
						m_agreed = false;
					}
					else if ( list.at(0).startsWith("EVT")){
						list.removeFirst();
						emit eventCommandReceived(list);
					}
				}
			}
		}
	}
}
